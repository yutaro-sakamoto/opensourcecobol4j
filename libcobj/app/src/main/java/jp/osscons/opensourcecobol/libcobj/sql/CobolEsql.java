package jp.osscons.opensourcecobol.libcobj.sql;

import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/**
 * COBOL の埋め込み SQL 操作（CONNECT、EXEC SQL、カーソル、トランザクション）のエントリポイント。
 *
 * <p>本クラスは API シグネチャを変えない薄い静的ファサードであり、実処理はすべて環境変数
 * {@code OCDB_DB_TYPE} から解決した {@link CobolEsqlBackendInterface} 実装インスタンスへ委譲する。これにより
 * 生成コード（{@code CobolEsql.xxx(...)} の静的呼び出し）は無変更のまま、DB ごとの実装差し替えを
 * 可能にする。
 */
public final class CobolEsql {

    /** ユーティリティクラスのインスタンス化を防ぐための private コンストラクタ。 */
    private CobolEsql() {}

    /** 環境変数から 1 度だけ解決する backend（遅延初期化）。 */
    private static CobolEsqlBackendInterface backend;

    /**
     * backend の解決に失敗した原因メッセージ。解決失敗（例: {@code OCDB_DB_TYPE} が未対応）を
     * 例外として生成 COBOL ランタイムへ伝播させると、Java の try/catch を持たないランタイムが
     * スタックトレースで異常終了する。これを避けるため失敗を 1 度だけ評価して記憶し、以後は
     * 例外を投げずに SQLCA へエラーを報告する。null は「まだ失敗していない」を表す。
     */
    private static String backendResolutionError;

    /**
     * テスト用: {@code OCDB_DB_TYPE} の値を上書きする seam（env に依存せず解決経路を駆動する）。
     * {@code null} のとき環境変数を使う（本番動作）。
     */
    private static String dbTypeOverrideForTest;

    /**
     * backend を遅延初期化して返す。{@code OCDB_DB_TYPE} の解決はプロセスで 1 回だけ行う。
     * アクセスを synchronized で直列化し、初回同時アクセス時も生成は 1 回に保たれる。
     *
     * <p>解決に失敗した場合は例外を投げず、原因を {@link #backendResolutionError} に記憶して
     * {@code null} を返す（以後の呼び出しで再解決・再 throw しない）。呼び出し側は {@code null} を
     * 「設定不正により使用不能」とみなし、SQLCA へエラーを報告する。
     */
    private static synchronized CobolEsqlBackendInterface backend() {
        if (backend == null && backendResolutionError == null) {
            try {
                String dbType =
                        dbTypeOverrideForTest != null
                                ? dbTypeOverrideForTest
                                : System.getenv("OCDB_DB_TYPE");
                backend = CobolEsqlBackendFactory.resolve(dbType);
            } catch (ClassNotFoundException | RuntimeException e) {
                backendResolutionError =
                        (e.getMessage() == null || e.getMessage().isEmpty())
                                ? "Unsupported OCDB_DB_TYPE"
                                : e.getMessage();
            }
        }
        return backend;
    }

    /**
     * backend を返す。解決失敗時は接続なし（{@code ECPG_NO_CONN}/{@code 08003}）を SQLCA に報告して
     * {@code null} を返す。CONNECT 以外の全エントリポイントの共通ガード。
     */
    private static CobolEsqlBackendInterface backendOrNoConn(CobolDataStorage sqlca) {
        CobolEsqlBackendInterface b = backend();
        if (b == null) {
            SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
        }
        return b;
    }

    // --- テスト支援（package-private。本番 API には露出しない）---

    /** テスト用: 解決済み backend・解決失敗状態・型上書きを破棄し、次回呼び出しで再解決させる。 */
    static synchronized void resetBackend() {
        backend = null;
        backendResolutionError = null;
        dbTypeOverrideForTest = null;
    }

    /** テスト用: backend を解決して返す（状態の検証・初期化に使う）。解決失敗時は {@code null}。 */
    static CobolEsqlBackendInterface backendForTest() {
        return backend();
    }

    /**
     * テスト用: {@code OCDB_DB_TYPE} の値を上書きして次回の backend 解決を駆動する（env 非依存）。
     * 未対応の値（例 {@code "mysql"}）を渡すと、実際の解決失敗経路（{@link #backend()} の catch）を
     * 通して設定不正時の挙動を検証できる。{@link #resetBackend()} で解除する。
     */
    static synchronized void setDbTypeForTest(String dbType) {
        dbTypeOverrideForTest = dbType;
        backend = null;
        backendResolutionError = null;
    }

    // -------------------------------------------------------
    // 接続
    // -------------------------------------------------------
    /**
     * user、password、dbname を個別のパラメータとして受け取り、データベース接続を確立する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param user ユーザー名フィールド
     * @param passwd パスワードフィールド
     * @param dbname データベース名フィールド
     */
    public static void connect(
            CobolDataStorage sqlca,
            AbstractCobolField user,
            AbstractCobolField passwd,
            AbstractCobolField dbname) {
        CobolEsqlBackendInterface b = backend();
        if (b == null) {
            // OCDB_DB_TYPE 不正などの設定エラーは接続失敗として報告する。
            SqlCA.setError(sqlca, SqlCA.ECPG_CONNECT, "08001", backendResolutionError);
            return;
        }
        b.connect(sqlca, user, passwd, dbname);
    }

    /**
     * commit を実行したうえで、デフォルトのデータベース接続を切断する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     */
    public static void disconnect(CobolDataStorage sqlca) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.disconnect(sqlca);
        }
    }

    // -------------------------------------------------------
    // 単純な SQL の実行（ホスト変数なし）
    // -------------------------------------------------------
    /**
     * ホスト変数パラメータを持たない SQL 文を実行する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param query SQL クエリ文字列
     */
    public static void exec(CobolDataStorage sqlca, String query) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.exec(sqlca, query);
        }
    }

    // -------------------------------------------------------
    // パラメータ付き SQL の実行
    // -------------------------------------------------------
    /**
     * COBOL のホスト変数をバインドしたパラメータ付き SQL 文を実行する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param query '?' プレースホルダを含む SQL クエリ文字列
     * @param params COBOL のホスト変数パラメータ
     */
    public static void execWithParams(
            CobolDataStorage sqlca, String query, AbstractCobolField... params) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.execWithParams(sqlca, query, params);
        }
    }

    // -------------------------------------------------------
    // WHERE CURRENT OF (位置付き UPDATE/DELETE)
    // -------------------------------------------------------
    /**
     * WHERE CURRENT OF を伴う UPDATE/DELETE（ホスト変数なし）を実行する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param query 末尾が {@code WHERE CURRENT OF} の SQL（カーソル名は含まない）
     * @param cursorName 位置付け対象の（修飾済み）カーソル名
     */
    public static void execWhereCurrentOf(CobolDataStorage sqlca, String query, String cursorName) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.execWhereCurrentOf(sqlca, query, cursorName);
        }
    }

    /**
     * WHERE CURRENT OF を伴う UPDATE/DELETE（ホスト変数あり）を実行する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param query 末尾が {@code WHERE CURRENT OF} の SQL（'?' プレースホルダを含む。カーソル名は含まない）
     * @param cursorName 位置付け対象の（修飾済み）カーソル名
     * @param params COBOL のホスト変数パラメータ
     */
    public static void execWithParamsWhereCurrentOf(
            CobolDataStorage sqlca, String query, String cursorName, AbstractCobolField... params) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.execWithParamsWhereCurrentOf(sqlca, query, cursorName, params);
        }
    }

    // -------------------------------------------------------
    // SELECT INTO
    // -------------------------------------------------------
    /**
     * SELECT INTO 文を実行し、結果を COBOL のホスト変数に書き戻す。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param query SELECT クエリ文字列
     * @param inputParams 入力のホスト変数パラメータ（WHERE 句のバインド）
     * @param resultParams 選択された列の値を受け取る出力ホスト変数
     */
    public static void selectInto(
            CobolDataStorage sqlca,
            String query,
            AbstractCobolField[] inputParams,
            AbstractCobolField[] resultParams) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.selectInto(sqlca, query, inputParams, resultParams);
        }
    }

    /**
     * OCCURS 配列に対する SELECT INTO 文を実行し、複数行を書き込む。
     *
     * @param sqlca SQLCA データストレージ
     * @param occursSize OCCURS 要素 1 つあたりのバイト数（ストライド）
     * @param occursMax OCCURS 要素の最大数
     * @param query SELECT クエリ文字列
     * @param inputParams 入力のホスト変数パラメータ
     * @param resultParams 出力ホスト変数（1 つの OCCURS 要素のフィールド群）
     */
    public static void selectIntoOccurs(
            CobolDataStorage sqlca,
            int occursSize,
            int occursMax,
            String query,
            AbstractCobolField[] inputParams,
            AbstractCobolField[] resultParams) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.selectIntoOccurs(sqlca, occursSize, occursMax, query, inputParams, resultParams);
        }
    }

    // -------------------------------------------------------
    // カーソル操作
    // -------------------------------------------------------
    /**
     * パラメータを持たない SQL カーソルを宣言する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param cursorName カーソル名
     * @param query カーソル用の SQL クエリ
     */
    public static void declareCursor(CobolDataStorage sqlca, String cursorName, String query) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.declareCursor(sqlca, cursorName, query);
        }
    }

    /**
     * ホスト変数パラメータを持つ SQL カーソルを宣言する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param cursorName カーソル名
     * @param query カーソル用の SQL クエリ
     * @param params カーソルをオープンする際にバインドするホスト変数パラメータ
     */
    public static void declareCursorWithParams(
            CobolDataStorage sqlca, String cursorName, String query, AbstractCobolField... params) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.declareCursorWithParams(sqlca, cursorName, query, params);
        }
    }

    /**
     * 事前に宣言されたカーソルをオープンする。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param cursorName オープンするカーソル名
     */
    public static void openCursor(CobolDataStorage sqlca, String cursorName) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.openCursor(sqlca, cursorName);
        }
    }

    /**
     * 事前に宣言されたカーソルをホスト変数パラメータ付きでオープンする。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param cursorName オープンするカーソル名
     * @param params カーソルクエリ用のホスト変数パラメータ
     */
    public static void openCursorWithParams(
            CobolDataStorage sqlca, String cursorName, AbstractCobolField... params) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.openCursorWithParams(sqlca, cursorName, params);
        }
    }

    /**
     * オープン済みのカーソルから次の行を取得し、COBOL のホスト変数に格納する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param cursorName fetch 対象のカーソル名
     * @param resultParams 列の値を受け取る出力ホスト変数
     */
    public static void fetchCursor(
            CobolDataStorage sqlca, String cursorName, AbstractCobolField... resultParams) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.fetchCursor(sqlca, cursorName, resultParams);
        }
    }

    /**
     * カーソルから複数行を取得し、OCCURS 配列フィールドに格納する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param cursorName fetch 対象のカーソル名
     * @param occursSize OCCURS 要素 1 つあたりのバイトサイズ
     * @param occursMax 取得する行の最大数
     * @param resultParams 結果パラメータフィールド（列ごとに 1 つ）
     */
    public static void fetchCursorOccurs(
            CobolDataStorage sqlca,
            String cursorName,
            int occursSize,
            int occursMax,
            AbstractCobolField... resultParams) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.fetchCursorOccurs(sqlca, cursorName, occursSize, occursMax, resultParams);
        }
    }

    /**
     * オープン済みのカーソルをクローズする。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param cursorName クローズするカーソル名
     */
    public static void closeCursor(CobolDataStorage sqlca, String cursorName) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.closeCursor(sqlca, cursorName);
        }
    }

    // -------------------------------------------------------
    // prepared statement の操作
    // -------------------------------------------------------
    /**
     * SQL 文を prepare し、COBOL のホスト変数参照を '?' プレースホルダに置き換える。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param stmtName prepared statement に割り当てる名前
     * @param queryField SQL クエリ文を保持する COBOL フィールド
     */
    public static void prepare(
            CobolDataStorage sqlca, String stmtName, AbstractCobolField queryField) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.prepare(sqlca, stmtName, queryField);
        }
    }

    /**
     * 事前に prepare された statement を実行する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param stmtName prepared statement の名前
     * @param params バインドするホスト変数パラメータ
     */
    public static void executePrepared(
            CobolDataStorage sqlca, String stmtName, AbstractCobolField... params) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.executePrepared(sqlca, stmtName, params);
        }
    }

    // -------------------------------------------------------
    // トランザクション
    // -------------------------------------------------------
    /**
     * デフォルト接続の現在の transaction を commit し、新しい transaction を開始する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     */
    public static void commit(CobolDataStorage sqlca) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.commit(sqlca);
        }
    }

    /**
     * デフォルト接続の現在の transaction を rollback し、新しい transaction を開始する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     */
    public static void rollback(CobolDataStorage sqlca) {
        CobolEsqlBackendInterface b = backendOrNoConn(sqlca);
        if (b != null) {
            b.rollback(sqlca);
        }
    }
}

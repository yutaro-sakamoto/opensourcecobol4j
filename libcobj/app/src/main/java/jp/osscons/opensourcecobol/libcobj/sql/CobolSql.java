package jp.osscons.opensourcecobol.libcobj.sql;

import java.nio.charset.Charset;
import java.sql.Connection;
import java.sql.ParameterMetaData;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.concurrent.ConcurrentHashMap;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/** COBOL の埋め込み SQL 操作（CONNECT、EXEC SQL、カーソル、トランザクション）のエントリポイント。 */
@SuppressWarnings("PMD.GuardLogStatement")
public final class CobolSql {

    private static final Logger LOG = LoggerFactory.getLogger(CobolSql.class);

    /** ユーティリティクラスのインスタンス化を防ぐための private コンストラクタ。 */
    private CobolSql() {}

    private static final Charset SHIFT_JIS = Charset.forName("SHIFT-JIS");

    /** ログ出力用に、空白文字（改行、タブ、連続するスペース）を単一のスペースにまとめる。 */
    private static String collapseWhitespace(String s) {
        return s.replaceAll("\\s+", " ").trim();
    }

    // 文が失敗した場合はエラーを SQLCA に記録し、トランザクションは PostgreSQL の
    // aborted 状態のままにする。回復 (ROLLBACK の発行) は COBOL プログラムの責任で、
    // ロールバックするまで以降の文は SQLSTATE 25P02 で拒否される (ECPG / PostgreSQL と同じ)。

    private static final ConcurrentHashMap<String, PreparedStatement> stmtCache =
            new ConcurrentHashMap<>();

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
        try {
            String userStr = fieldToString(user);
            String passwdStr = fieldToString(passwd);
            String dbnameStr = fieldToString(dbname);

            LOG.trace("CONNECT user={} dbname={}", userStr.trim(), dbnameStr.trim());
            SqlConnection conn = SqlConnection.connect(userStr, passwdStr, dbnameStr);
            SqlState.addConnection(conn.getId(), conn);
            // CONNECT 成功時は SQLERRMC を上書きしない (COBOL 初期値のスペースを残す。Open COBOL ESQL 4Jに合わせる)。
            SqlCA.setSuccessKeepErrmc(sqlca);
            LOG.debug("CONNECT successful (id={})", conn.getId());
        } catch (SQLException e) {
            LOG.error("CONNECT failed: {}", e.getMessage());
            SqlCA.setResultFromException(sqlca, e);
        } catch (Exception e) {
            LOG.error("CONNECT failed: {}", e.getMessage());
            SqlCA.setError(sqlca, SqlCA.ECPG_CONNECT, "08001", e.getMessage());
        }
    }

    /**
     * commit を実行したうえで、デフォルトのデータベース接続を切断する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     */
    public static void disconnect(CobolDataStorage sqlca) {
        try {
            SqlConnection conn = SqlState.getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            LOG.debug("DISCONNECT (id={})", conn.getId());
            // 切断前に commit する
            try (Statement stmt = conn.getConnection().createStatement()) {
                stmt.execute("COMMIT");
            } catch (SQLException ignored) {
                // 切断時の commit エラーは無視する
            }
            conn.close();
            SqlState.removeConnection(conn.getId());
            // 接続クローズで PreparedStatement も閉じられるため、キャッシュを破棄する。
            // 残すと再接続時に identity hashCode の再利用でクローズ済み statement を返す恐れがある。
            stmtCache.clear();
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            LOG.error("DISCONNECT failed: {}", e.getMessage());
            SqlCA.setResultFromException(sqlca, e);
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
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            Connection conn = sqlConn.getConnection();

            if (query == null || query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty query");
                return;
            }

            String trimmed = collapseWhitespace(query);
            LOG.debug("EXEC SQL: {}", trimmed);
            boolean isTxnControl =
                    "COMMIT".equalsIgnoreCase(trimmed)
                            || "ROLLBACK".equalsIgnoreCase(trimmed)
                            || "BEGIN".equalsIgnoreCase(trimmed);

            if (isTxnControl) {
                try (Statement stmt = conn.createStatement()) {
                    stmt.execute(query);
                }
                SqlCA.setSuccess(sqlca);
                if ("COMMIT".equalsIgnoreCase(trimmed) || "ROLLBACK".equalsIgnoreCase(trimmed)) {
                    SqlState.clearCursors();
                    sqlConn.beginTransaction();
                }
            } else {
                try (Statement stmt = conn.createStatement()) {
                    stmt.execute(query);
                    int updateCount = stmt.getUpdateCount();
                    if (updateCount >= 0) {
                        SqlCA.setErrd(sqlca, 2, updateCount);
                    }
                }
                SqlCA.setSuccess(sqlca);
            }
        } catch (SQLException e) {
            LOG.error("EXEC SQL failed: {} - {}", collapseWhitespace(query), e.getMessage());
            SqlCA.setResultFromException(sqlca, e);
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
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            Connection conn = sqlConn.getConnection();

            if (query == null || query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty query");
                return;
            }

            LOG.debug(
                    "EXEC SQL (params={}): {}",
                    params != null ? params.length : 0,
                    collapseWhitespace(query));

            PreparedStatement pstmt = getOrCreatePreparedStatement(conn, query);
            // getParameterMetaData (Describe) が失敗した場合 (例: テーブル不在) は、その例外を
            // そのまま下の catch で SQLCA に記録する。これにより真のエラー (例: 42P01) が報告される。
            ParameterMetaData metaData = pstmt.getParameterMetaData();
            if (params != null) {
                for (int i = 0; i < params.length; i++) {
                    CobolDataConverter.setParam(pstmt, i + 1, metaData, params[i]);
                }
            }
            pstmt.execute();
            int updateCount = pstmt.getUpdateCount();
            if (updateCount >= 0) {
                SqlCA.setErrd(sqlca, 2, updateCount);
            }

            SqlCA.setSuccess(sqlca);

            if ("COMMIT".equalsIgnoreCase(query) || "ROLLBACK".equalsIgnoreCase(query)) {
                SqlState.clearCursors();
                sqlConn.beginTransaction();
            }
        } catch (SQLException e) {
            LOG.error("EXEC SQL failed: {} - {}", collapseWhitespace(query), e.getMessage());
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // WHERE CURRENT OF (位置付き UPDATE/DELETE)
    // -------------------------------------------------------
    /**
     * WHERE CURRENT OF を伴う UPDATE/DELETE（ホスト変数なし）を実行する。
     *
     * <p>先読み（バルクフェッチ）でサーバカーソルが論理現在行より進んでいる場合に備え、実行前に
     * {@code FETCH BACKWARD} でカーソル位置を補正する（Open COBOL ESQL 4J と同じ挙動）。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param query 末尾が {@code WHERE CURRENT OF} の SQL（カーソル名は含まない）
     * @param cursorName 位置付け対象の（修飾済み）カーソル名
     */
    public static void execWhereCurrentOf(CobolDataStorage sqlca, String query, String cursorName) {
        String positioned = repositionForCurrentOf(sqlca, query, cursorName);
        if (positioned != null) {
            exec(sqlca, positioned);
        }
    }

    /**
     * WHERE CURRENT OF を伴う UPDATE/DELETE（ホスト変数あり）を実行する。
     * {@link #execWhereCurrentOf} と同様にカーソル位置を補正してから実行する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param query 末尾が {@code WHERE CURRENT OF} の SQL（'?' プレースホルダを含む。カーソル名は含まない）
     * @param cursorName 位置付け対象の（修飾済み）カーソル名
     * @param params COBOL のホスト変数パラメータ
     */
    public static void execWithParamsWhereCurrentOf(
            CobolDataStorage sqlca, String query, String cursorName, AbstractCobolField... params) {
        String positioned = repositionForCurrentOf(sqlca, query, cursorName);
        if (positioned != null) {
            execWithParams(sqlca, positioned, params);
        }
    }

    /**
     * WHERE CURRENT OF の実行前にサーバカーソルを論理現在行へ戻し、カーソル名を補った完全な SQL を返す。
     *
     * <p>先読みバッファに未供給行が {@code remaining} 行残っていれば、さらに overFetch（結果末尾に達し
     * サーバカーソルが末尾の先にある）なら +1 行ぶん {@code FETCH BACKWARD} してから、バッファを破棄する。
     * 接続なし/カーソル未登録のときは SQLCA にエラーを設定し null を返す。
     */
    private static String repositionForCurrentOf(
            CobolDataStorage sqlca, String query, String cursorName) {
        SqlConnection sqlConn = SqlState.getDefaultConnection();
        if (sqlConn == null) {
            SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
            return null;
        }
        SqlCursor cursor = SqlState.getCursor(cursorName);
        if (cursor == null) {
            // 未登録カーソル（Open COBOL ESQL 4J は OCDB_EMPTY を返す）。
            SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Cursor not found: " + cursorName);
            return null;
        }
        try {
            int backward = cursor.remainingBuffered() + (cursor.overFetch ? 1 : 0);
            if (backward > 0) {
                Connection conn = sqlConn.getConnection();
                try (Statement stmt = conn.createStatement()) {
                    stmt.execute("FETCH BACKWARD " + backward + " FROM " + cursorName);
                }
            }
            // 位置補正後は先読みバッファを無効化し、次の FETCH は補正後の位置から取り直す。
            cursor.clearBuffer();
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
            return null;
        }
        return query + " " + cursorName;
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
    private static void fetchOccursRows(
            ResultSet rs,
            AbstractCobolField[] resultParams,
            int occursSize,
            int occursMax,
            CobolDataStorage sqlca)
            throws SQLException {
        if (rs == null || !rs.next()) {
            SqlCA.setCode(sqlca, SqlCA.ECPG_NOT_FOUND);
            SqlCA.setState(sqlca, "02000");
            SqlCA.clearErrmc(sqlca);
            if (rs != null) {
                rs.close();
            }
            return;
        }
        int rowCount = 0;
        boolean sawNullWithoutIndicator = false;
        do {
            if (rowCount >= occursMax) {
                break;
            }
            int storageOffset = rowCount * occursSize;
            for (int i = 0; i < resultParams.length; i++) {
                byte[] value = CobolDataConverter.getValueFromResultSet(rs, i + 1);
                CobolDataStorage fieldStorage =
                        resultParams[i].getDataStorage().getSubDataStorage(storageOffset);
                int fieldSize = resultParams[i].getSize();
                if (value != null) {
                    CobolDataConverter.stringToCobolRaw(
                            resultParams[i], fieldStorage, fieldSize, value);
                } else {
                    fieldStorage.memset((byte) 0, fieldSize);
                    sawNullWithoutIndicator = true;
                }
            }
            rowCount++;
        } while (rs.next());
        rs.close();
        SqlCA.setErrd(sqlca, 2, rowCount);
        if (sawNullWithoutIndicator) {
            SqlCA.setMissingIndicator(sqlca);
        } else {
            SqlCA.setSuccess(sqlca);
        }
    }

    private static void processSelectIntoResults(
            ResultSet rs, AbstractCobolField[] resultParams, CobolDataStorage sqlca)
            throws SQLException {
        if (rs == null || !rs.next()) {
            SqlCA.setCode(sqlca, SqlCA.ECPG_NOT_FOUND);
            SqlCA.setState(sqlca, "02000");
            SqlCA.clearErrmc(sqlca);
            if (rs != null) {
                rs.close();
            }
            return;
        }

        if (resultParams == null || resultParams.length == 0) {
            rs.close();
            SqlCA.setSuccess(sqlca);
            return;
        }

        // GROUP OCCURS パターン（単一の GROUP 結果フィールドで複数行）かどうかを確認する
        if (resultParams.length == 1
                && resultParams[0].getAttribute().getType() == CobolFieldAttribute.COB_TYPE_GROUP) {
            AbstractCobolField groupField = resultParams[0];
            int elementSize = groupField.getSize();
            CobolDataStorage baseStorage = groupField.getDataStorage();
            int columnCount = rs.getMetaData().getColumnCount();
            int rowIndex = 0;
            boolean sawNullWithoutIndicator = false;

            do {
                // 各列を要素内の正しい位置に書き込む
                int colOffset = 0;
                for (int col = 1; col <= columnCount; col++) {
                    byte[] value = CobolDataConverter.getValueFromResultSet(rs, col);
                    int colSize = rs.getMetaData().getColumnDisplaySize(col);
                    CobolDataStorage elementStorage =
                            baseStorage.getSubDataStorage(rowIndex * elementSize + colOffset);
                    if (value != null) {
                        // 列サイズに合わせてパディングまたは切り詰めを行う
                        if (value.length >= colSize) {
                            elementStorage.memcpy(value, colSize);
                        } else {
                            elementStorage.memset((byte) ' ', colSize);
                            elementStorage.memcpy(value, value.length);
                        }
                    } else {
                        elementStorage.memset((byte) 0, colSize);
                        sawNullWithoutIndicator = true;
                    }
                    colOffset += colSize;
                }
                rowIndex++;
            } while (rs.next());
            rs.close();
            SqlCA.setErrd(sqlca, 2, rowIndex);
            if (sawNullWithoutIndicator) {
                SqlCA.setMissingIndicator(sqlca);
            } else {
                SqlCA.setSuccess(sqlca);
            }
            return;
        }

        // 単一行: 列を個々の結果フィールドに書き込む
        int columnCount = rs.getMetaData().getColumnCount();
        boolean sawNullWithoutIndicator = false;
        for (int i = 0; i < resultParams.length && i < columnCount; i++) {
            byte[] value = CobolDataConverter.getValueFromResultSet(rs, i + 1);
            if (value != null) {
                CobolDataConverter.stringToCobol(resultParams[i], value);
            } else {
                resultParams[i].getDataStorage().memset((byte) 0, resultParams[i].getSize());
                sawNullWithoutIndicator = true;
            }
        }
        rs.close();
        if (sawNullWithoutIndicator) {
            SqlCA.setMissingIndicator(sqlca);
        } else {
            SqlCA.setSuccess(sqlca);
        }
    }

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
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            Connection conn = sqlConn.getConnection();

            if (query == null || query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty query");
                return;
            }

            LOG.debug("SELECT INTO: {}", collapseWhitespace(query));

            if (inputParams != null && inputParams.length > 0) {
                PreparedStatement pstmt = getOrCreatePreparedStatement(conn, query);
                ParameterMetaData metaData = pstmt.getParameterMetaData();
                for (int i = 0; i < inputParams.length; i++) {
                    CobolDataConverter.setParam(pstmt, i + 1, metaData, inputParams[i]);
                }
                pstmt.execute();
                processSelectIntoResults(pstmt.getResultSet(), resultParams, sqlca);
            } else {
                try (Statement stmt = conn.createStatement()) {
                    stmt.execute(query);
                    processSelectIntoResults(stmt.getResultSet(), resultParams, sqlca);
                }
            }
        } catch (SQLException e) {
            LOG.error("SELECT INTO failed: {} - {}", collapseWhitespace(query), e.getMessage());
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    /**
     * OCCURS 配列に対する SELECT INTO 文を実行し、複数行を書き込む。
     *
     * @param sqlca SQLCA データストレージ
     * @param query SELECT クエリ文字列
     * @param inputParams 入力のホスト変数パラメータ
     * @param resultParams 出力ホスト変数（1 つの OCCURS 要素のフィールド群）
     * @param occursSize OCCURS 要素 1 つあたりのバイト数（ストライド）
     * @param occursMax OCCURS 要素の最大数
     */
    public static void selectIntoOccurs(
            CobolDataStorage sqlca,
            String query,
            AbstractCobolField[] inputParams,
            AbstractCobolField[] resultParams,
            int occursSize,
            int occursMax) {
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            Connection conn = sqlConn.getConnection();
            if (query == null || query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty query");
                return;
            }

            if (inputParams != null && inputParams.length > 0) {
                PreparedStatement pstmt = getOrCreatePreparedStatement(conn, query);
                ParameterMetaData metaData = pstmt.getParameterMetaData();
                for (int i = 0; i < inputParams.length; i++) {
                    CobolDataConverter.setParam(pstmt, i + 1, metaData, inputParams[i]);
                }
                pstmt.execute();
                fetchOccursRows(pstmt.getResultSet(), resultParams, occursSize, occursMax, sqlca);
            } else {
                try (Statement stmt = conn.createStatement()) {
                    stmt.execute(query);
                    fetchOccursRows(
                            stmt.getResultSet(), resultParams, occursSize, occursMax, sqlca);
                }
            }
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
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
        try {
            if (cursorName == null || cursorName.isEmpty() || query == null || query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty cursor name or query");
                return;
            }
            LOG.debug("DECLARE CURSOR {} FOR: {}", cursorName, collapseWhitespace(query));
            SqlCursor existing = SqlState.getCursor(cursorName);
            if (existing != null && existing.isOpened) {
                SqlCA.setError(
                        sqlca, SqlCA.ECPG_WARNING_PORTAL_EXISTS, "42P03", "Cursor already opened");
                return;
            }
            SqlCursor cursor = new SqlCursor(cursorName, query, 0);
            SqlState.addCursor(cursorName, cursor);
            SqlCA.setSuccess(sqlca);
        } catch (Exception e) {
            SqlCA.setError(sqlca, SqlCA.ECPG_PGSQL, "     ", e.getMessage());
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
        try {
            if (cursorName == null || cursorName.isEmpty() || query == null || query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty cursor name or query");
                return;
            }
            SqlCursor existing = SqlState.getCursor(cursorName);
            if (existing != null && existing.isOpened) {
                SqlCA.setError(
                        sqlca, SqlCA.ECPG_WARNING_PORTAL_EXISTS, "42P03", "Cursor already opened");
                return;
            }
            SqlCursor cursor = new SqlCursor(cursorName, query, params != null ? params.length : 0);
            cursor.params = params;
            SqlState.addCursor(cursorName, cursor);
            SqlCA.setSuccess(sqlca);
        } catch (Exception e) {
            SqlCA.setError(sqlca, SqlCA.ECPG_PGSQL, "     ", e.getMessage());
        }
    }

    /**
     * 事前に宣言されたカーソルをオープンする。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param cursorName オープンするカーソル名
     */
    public static void openCursor(CobolDataStorage sqlca, String cursorName) {
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            SqlCursor cursor = SqlState.getCursor(cursorName);
            if (cursor == null) {
                SqlCA.setError(
                        sqlca,
                        SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                        "34000",
                        "Cursor not found: " + cursorName);
                return;
            }
            LOG.debug("OPEN CURSOR {}", cursorName);
            Connection conn = sqlConn.getConnection();
            cursor.open(conn, null);
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            LOG.error("OPEN CURSOR {} failed: {}", cursorName, e.getMessage());
            SqlCA.setResultFromException(sqlca, e);
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
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            SqlCursor cursor = SqlState.getCursor(cursorName);
            if (cursor == null) {
                SqlCA.setError(
                        sqlca,
                        SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                        "34000",
                        "Cursor not found: " + cursorName);
                return;
            }
            cursor.open(sqlConn.getConnection(), params);
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            LOG.error("OPEN CURSOR {} failed: {}", cursorName, e.getMessage());
            SqlCA.setResultFromException(sqlca, e);
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
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            SqlCursor cursor = SqlState.getCursor(cursorName);
            if (cursor == null) {
                // 未 DECLARE のカーソル。PostgreSQL に問い合わせる相手がいないため、ここで
                // -602/34000 を返す (Open-COBOL-ESQL-4J も未登録カーソルは同様)。
                SqlCA.setError(
                        sqlca,
                        SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                        "34000",
                        "Cursor not found: " + cursorName);
                return;
            }
            if (!cursor.isOpened) {
                // 登録済みだが未 OPEN (OPEN 失敗を含む) のカーソルへの FETCH は、PostgreSQL に
                // 問い合わせず -602/34000 をローカルで返す (CLOSE の未 OPEN 扱いと対称。
                // Open COBOL ESQL 4J と同じ)。OPEN 失敗で aborted になったトランザクションへ
                // FETCH を送って 25P02 に化けるのを避ける。
                SqlCA.setError(
                        sqlca,
                        SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                        "34000",
                        "Cursor not open: " + cursorName);
                return;
            }
            LOG.trace("FETCH CURSOR {}", cursorName);
            // 正常な fetch が sqlcode=0 になるよう事前にクリアしておく。指標変数なしで NULL の列が
            // ある場合、fetch() が ECPG_MISSING_INDICATOR で上書きすることがある。
            SqlCA.setSuccess(sqlca);
            boolean hasRow = cursor.fetch(sqlConn.getConnection(), resultParams, sqlca);
            if (!hasRow) {
                SqlCA.setCode(sqlca, SqlCA.ECPG_NOT_FOUND);
                SqlCA.setState(sqlca, "02000");
                SqlCA.clearErrmc(sqlca);
            }
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    /**
     * カーソルから複数行を取得し、OCCURS 配列フィールドに格納する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param cursorName fetch 対象のカーソル名
     * @param resultParams 結果パラメータフィールド（列ごとに 1 つ）
     * @param occursSize OCCURS 要素 1 つあたりのバイトサイズ
     * @param occursMax 取得する行の最大数
     */
    public static void fetchCursorOccurs(
            CobolDataStorage sqlca,
            String cursorName,
            int occursSize,
            int occursMax,
            AbstractCobolField... resultParams) {
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            SqlCursor cursor = SqlState.getCursor(cursorName);
            if (cursor == null) {
                // 未 DECLARE のカーソル: -602/34000 を返す (Open COBOL ESQL 4Jと同様)。
                SqlCA.setErrd(sqlca, 2, 0);
                SqlCA.setError(
                        sqlca,
                        SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                        "34000",
                        "Cursor not found: " + cursorName);
                return;
            }
            if (!cursor.isOpened) {
                // 未 OPEN のカーソルへの FETCH は -602/34000 をローカルで返す (単一行 FETCH と対称)。
                SqlCA.setErrd(sqlca, 2, 0);
                SqlCA.setError(
                        sqlca,
                        SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                        "34000",
                        "Cursor not open: " + cursorName);
                return;
            }
            // OCCURS への複数行 FETCH は単一行 FETCH の先読みバッファを使わず、直接
            // FETCH FORWARD occursMax を発行する (Open COBOL ESQL 4J の FetchOccurs と同じ)。
            // 同一カーソルに対する単一行 FETCH の先読みバッファが残っているとサーバカーソル位置と
            // 食い違うため、ここで破棄しておく。
            cursor.clearBuffer();
            Connection conn = sqlConn.getConnection();
            String fetchSql = "FETCH FORWARD " + occursMax + " FROM " + cursor.name;
            try (Statement stmt = conn.createStatement();
                    ResultSet rs = stmt.executeQuery(fetchSql)) {
                if (!rs.next()) {
                    SqlCA.setSuccess(sqlca);
                    return;
                }
                int rowCount = 0;
                boolean sawNullWithoutIndicator = false;
                do {
                    if (rowCount >= occursMax) {
                        break;
                    }
                    int storageOffset = rowCount * occursSize;
                    for (int i = 0; i < resultParams.length; i++) {
                        byte[] value = CobolDataConverter.getValueFromResultSet(rs, i + 1);
                        CobolDataStorage fieldStorage =
                                resultParams[i].getDataStorage().getSubDataStorage(storageOffset);
                        int fieldSize = resultParams[i].getSize();
                        if (value != null) {
                            CobolDataConverter.stringToCobolRaw(
                                    resultParams[i], fieldStorage, fieldSize, value);
                        } else {
                            fieldStorage.memset((byte) 0, fieldSize);
                            sawNullWithoutIndicator = true;
                        }
                    }
                    rowCount++;
                } while (rs.next());
                SqlCA.setErrd(sqlca, 2, rowCount);
                if (sawNullWithoutIndicator) {
                    SqlCA.setMissingIndicator(sqlca);
                } else {
                    SqlCA.setSuccess(sqlca);
                }
            }
        } catch (SQLException e) {
            // フェッチ失敗時は取得行数 SQLERRD(3) を 0 にしてから PostgreSQL エラーを反映する。
            SqlCA.setErrd(sqlca, 2, 0);
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    /**
     * オープン済みのカーソルをクローズする。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     * @param cursorName クローズするカーソル名
     */
    public static void closeCursor(CobolDataStorage sqlca, String cursorName) {
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            SqlCursor cursor = SqlState.getCursor(cursorName);
            if (cursor == null) {
                // 未 DECLARE のカーソル: -602/34000 を返す (Open COBOL ESQL 4Jと同様)。
                SqlCA.setError(
                        sqlca,
                        SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                        "34000",
                        "Cursor not found: " + cursorName);
                return;
            }
            if (!cursor.isOpened) {
                // 登録済みだが未 OPEN のカーソルの CLOSE は成功扱い (Open-COBOL-ESQL-4J
                // も未 OPEN カーソルの CLOSE は 0 を返す)。
                SqlCA.setSuccess(sqlca);
                return;
            }
            cursor.close(sqlConn.getConnection());
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
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
        try {
            if (stmtName == null || stmtName.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty statement name");
                return;
            }
            String query;
            if (queryField != null
                    && queryField.getDataStorage() != null
                    && queryField.getSize() > 0) {
                // ホスト変数の COBOL 型に従って SQL テキストを抽出する。
                // CobolDataConverter は VARYING（バイナリの長さヘッダ + データ）、
                // 単純な英数字 / group、national などを処理するため、PREPARE は
                // 固定長フィールドだけでなく、適切な `PIC X(n) VARYING` のホスト変数でも動作する。
                query = CobolDataConverter.cobolToString(queryField).trim();
            } else {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty query");
                return;
            }
            if (query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty query");
                return;
            }

            // ホスト変数のプレースホルダを数えて置き換える
            int nParams = 0;
            StringBuilder replaced = new StringBuilder();
            for (int i = 0; i < query.length(); i++) {
                char c = query.charAt(i);
                if (c == ':' && i + 1 < query.length() && Character.isLetter(query.charAt(i + 1))) {
                    nParams++;
                    replaced.append('?');
                    i++;
                    while (i < query.length()
                            && query.charAt(i) != ' '
                            && query.charAt(i) != ','
                            && query.charAt(i) != ')') {
                        i++;
                    }
                    i--;
                } else {
                    replaced.append(c);
                }
            }

            SqlState.addPrepared(stmtName, replaced.toString(), nParams);
            SqlCA.setSuccess(sqlca);
        } catch (Exception e) {
            SqlCA.setError(sqlca, SqlCA.ECPG_PGSQL, "     ", e.getMessage());
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
        try {
            String[] prepared = SqlState.getPrepared(stmtName);
            if (prepared == null) {
                SqlCA.setError(
                        sqlca,
                        SqlCA.ECPG_INVALID_STMT,
                        "26000",
                        "Statement not found: " + stmtName);
                return;
            }

            String query = prepared[0];
            if (params != null && params.length > 0) {
                execWithParams(sqlca, query, params);
            } else {
                exec(sqlca, query);
            }
        } catch (Exception e) {
            SqlCA.setError(sqlca, SqlCA.ECPG_PGSQL, "     ", e.getMessage());
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
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            Connection conn = sqlConn.getConnection();
            try (Statement stmt = conn.createStatement()) {
                stmt.execute("COMMIT");
            }
            SqlCA.setSuccess(sqlca);
            SqlState.clearCursors();
            sqlConn.beginTransaction();
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    /**
     * デフォルト接続の現在の transaction を rollback し、新しい transaction を開始する。
     *
     * @param sqlca ステータス報告用の SQLCA データストレージ
     */
    public static void rollback(CobolDataStorage sqlca) {
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            Connection conn = sqlConn.getConnection();
            try (Statement stmt = conn.createStatement()) {
                stmt.execute("ROLLBACK");
            }
            SqlCA.setSuccess(sqlca);
            SqlState.clearCursors();
            sqlConn.beginTransaction();
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // ヘルパーメソッド
    // -------------------------------------------------------
    private static String storageToString(CobolDataStorage storage, int len) {
        if (storage == null || len <= 0) {
            return null;
        }
        return new String(storage.getByteArrayRef(0, len), storage.getIndex(), len, SHIFT_JIS);
    }

    private static String fieldToString(AbstractCobolField field) {
        if (field == null) {
            return null;
        }
        return storageToString(field.getDataStorage(), field.getSize());
    }

    private static PreparedStatement getOrCreatePreparedStatement(Connection conn, String query)
            throws SQLException {
        String cacheKey =
                Integer.toHexString(conn.hashCode()) + "-" + Integer.toHexString(query.hashCode());
        return stmtCache.computeIfAbsent(
                cacheKey,
                k -> {
                    try {
                        return conn.prepareStatement(query);
                    } catch (SQLException e) {
                        throw new RuntimeException(e);
                    }
                });
    }
}

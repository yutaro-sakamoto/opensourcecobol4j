package jp.osscons.opensourcecobol.libcobj.sql;

import java.nio.charset.Charset;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ParameterMetaData;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.ConcurrentHashMap;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * {@link CobolSqlBackend} の共通処理（DB 非依存）を実装し、DB 依存部分を {@code protected abstract}
 * フックとして切り出した抽象基底クラス（Template Method）。
 *
 * <p>旧 {@code SqlState}（接続/カーソル/prepared のグローバル registry）の状態を本クラスの
 * インスタンスフィールドへ集約し、旧 {@code SqlConnection}/{@code SqlCursor} の振る舞いを
 * DB 依存フックへ移している。SQLCA 構造体の byte エンコードは DB 非依存の COBOL ABI のため
 * {@link SqlCA} の static ヘルパを共有し、{@code 生エラー → (ECPG コード, 正規化 SQLSTATE)} 変換のみ
 * {@link #mapSqlException(SQLException)} フックとして DB ごとに実装する。
 *
 * <p>本クラスは将来の DB 実装（Db2/Oracle など）から継承される拡張点であり、{@code protected}
 * フック群と {@link DbSpec}/{@link Cursor}/{@link SqlErrorMapping} ネストクラスは「拡張点としての
 * 契約」として扱う。
 */
@SuppressWarnings("PMD.GuardLogStatement")
public abstract class AbstractCobolSqlBackend implements CobolSqlBackend {

    /** サブクラス（DB 依存バックエンド）から継承するための既定コンストラクタ。 */
    protected AbstractCobolSqlBackend() {}

    // 操作系ログ（CONNECT/EXEC SQL/カーソル/DISCONNECT/エラー）は、公開エントリポイント
    // CobolSql のロガー名で出す。運用者が有効化するロガー名（...sql.CobolSql）を維持するため。
    private static final Logger LOG = LoggerFactory.getLogger(CobolSql.class);

    // 低レベルの接続確立ログ（Connecting to.../Connected successfully）は別ロガーに出す。
    // CobolSql のロガーだけを debug 有効化しても、これらは出力されない（接続詳細を分離する）。
    private static final Logger CONN_LOG = LoggerFactory.getLogger(AbstractCobolSqlBackend.class);

    /** COBOL 固定長フィールドのバイト表現に用いる文字コード（接続パラメータの取り出し用）。 */
    private static final Charset SHIFT_JIS = Charset.forName("SHIFT-JIS");

    /** 単一接続モデルで用いるデフォルト接続の識別子（旧 SqlConnection と同じ値）。 */
    private static final String DEFAULT_CONN_ID = "OCDB_DEFAULT_DBNAME";

    /**
     * 指標変数なしで NULL 列を受け取ったときに、ホスト変数へ書き込む空の結果。型対応の
     * 変換 (stringToCobol/stringToCobolRaw) に空バイト列を渡すと、数値は 0、英数字は空白、
     * national は全角空白という型ごとに正しい表現になる。
     */
    protected static final byte[] EMPTY_RESULT = new byte[0];

    // === 旧 SqlState：グローバル registry → バックエンドのインスタンス状態へ ===
    // スレッドモデル: 生成 COBOL ランタイムは単一プロセス・単一スレッド実行を前提とする
    // (旧 SqlState もグローバル static registry だった)。このため接続/カーソル/prepared の
    // registry と defaultConnId は同期せず素の HashMap/フィールドのまま扱う。
    // マルチスレッド対応が必要になった場合は、ConcurrentHashMap 化と defaultConnId 更新を含む
    // 同期、または public API 全体の直列化を別途検討する (stmtCache の ConcurrentHashMap 化も
    // 同前提下の保守的な保護であり、現時点で全レジストリの同期を意味するものではない)。
    private final Map<String, Connection> connections = new HashMap<>();
    private final Map<String, Cursor> cursors = new HashMap<>();
    private final Map<String, String[]> prepared = new HashMap<>();
    private String defaultConnId;

    // === 旧 CobolSql：PreparedStatement キャッシュ（DB 非依存）===
    // backend は singleton 共有のため、ConcurrentHashMap で保護する。
    private final Map<Connection, Map<String, PreparedStatement>> stmtCache =
            new ConcurrentHashMap<>();

    // -------------------------------------------------------
    // DB 依存フック（PostgreSQL/Db2/Oracle で差し替え）
    // -------------------------------------------------------

    /**
     * パース済みの接続情報から JDBC URL 文字列を組み立てる。
     *
     * @param spec DB 非依存にパース済みの接続情報
     * @return JDBC URL
     */
    protected abstract String buildJdbcUrl(DbSpec spec);

    /**
     * 明示的なドライバロードが必要な DB のためのドライバクラス名。既定は {@code null}（不要）。
     * JDBC 4.0 以降のドライバは ServiceLoader で自動登録されるため通常は不要。
     *
     * @return ドライバの完全修飾クラス名、または不要なら {@code null}
     */
    protected String driverClass() {
        return null;
    }

    /**
     * 接続直後の DB 別初期化（autoCommit モード・初回トランザクション開始など）。
     *
     * @param c 確立した JDBC 接続
     * @throws SQLException データベースアクセスエラー
     */
    protected abstract void configureConnection(Connection c) throws SQLException;

    /**
     * 新しいトランザクションを開始する（DB 流儀を内包）。明示的な {@code BEGIN} を発行する DB は
     * ここで発行し、最初の文で暗黙にトランザクションが始まる DB（JDBC の {@code autoCommit=false}
     * など）では no-op でよい。
     *
     * @param c JDBC 接続
     * @throws SQLException データベースアクセスエラー
     */
    protected abstract void beginTransaction(Connection c) throws SQLException;

    /**
     * 現在のトランザクションを commit する（再 BEGIN は含まない、確定のみ）。コミットの発行手段だけを
     * DB 実装へ委ねる。例: PostgreSQL は {@code stmt.execute("COMMIT")}、JDBC の
     * {@code autoCommit=false} 流儀の DB は {@code c.commit()}。
     *
     * <p>「commit して新しいトランザクションを継続する」公開操作（{@link #commit(CobolDataStorage)}）と、
     * 「commit して接続を閉じる」操作（{@link #disconnect(CobolDataStorage)}）の双方が本フックを再利用する。
     * 再 BEGIN の要否は呼び出し側が {@link #beginTransaction(Connection)} で制御する。
     *
     * @param c JDBC 接続
     * @throws SQLException データベースアクセスエラー
     */
    protected abstract void commitTransaction(Connection c) throws SQLException;

    /**
     * 現在のトランザクションを rollback する（再 BEGIN は含まない、ロールバックのみ）。発行手段だけを
     * DB 実装へ委ねる。例: PostgreSQL は {@code stmt.execute("ROLLBACK")}、JDBC の
     * {@code autoCommit=false} 流儀の DB は {@code c.rollback()}。
     *
     * @param c JDBC 接続
     * @throws SQLException データベースアクセスエラー
     */
    protected abstract void rollbackTransaction(Connection c) throws SQLException;

    /**
     * カーソルをオープンする（DECLARE 等の DB 別の発行手段を内包）。成功時に {@code cur} を
     * オープン済みとして扱えるよう、SQL の発行のみを行う（状態の更新は共通フローが行う）。
     *
     * @param c JDBC 接続
     * @param cur カーソル状態
     * @param params OPEN ... USING のホスト変数（なければ {@code null}）
     * @throws SQLException データベースアクセスエラー
     */
    protected abstract void openCursorImpl(Connection c, Cursor cur, AbstractCobolField[] params)
            throws SQLException;

    /**
     * カーソルから次の 1 行を取得し、{@code out} へ書き込む。指標変数なしの NULL があれば
     * {@code sqlca} に {@code ECPG_MISSING_INDICATOR} を設定する。
     *
     * @param c JDBC 接続
     * @param cur カーソル状態
     * @param out 列の値を受け取る出力ホスト変数
     * @param sqlca 指標変数なしの NULL をフラグするための SQLCA ストレージ
     * @return 行が取得できた場合 true、これ以上行がなければ false
     * @throws SQLException データベースアクセスエラー
     */
    protected abstract boolean fetchRowImpl(
            Connection c, Cursor cur, AbstractCobolField[] out, CobolDataStorage sqlca)
            throws SQLException;

    /**
     * カーソルから複数行を取得し、OCCURS 配列へ書き込む。行数・NULL 処理を含め {@code sqlca} を更新する。
     *
     * @param c JDBC 接続
     * @param cur カーソル状態
     * @param occursSize OCCURS 要素 1 つあたりのバイトサイズ
     * @param occursMax 取得する行の最大数
     * @param resultParams 結果パラメータフィールド（列ごとに 1 つ）
     * @param sqlca SQLCA データストレージ
     * @throws SQLException データベースアクセスエラー
     */
    protected abstract void fetchOccursImpl(
            Connection c,
            Cursor cur,
            int occursSize,
            int occursMax,
            AbstractCobolField[] resultParams,
            CobolDataStorage sqlca)
            throws SQLException;

    /**
     * カーソルをクローズする（CLOSE 等の DB 別の発行手段を内包）。SQL の発行のみを行う。
     *
     * @param c JDBC 接続
     * @param cur カーソル状態
     * @throws SQLException データベースアクセスエラー
     */
    protected abstract void closeCursorImpl(Connection c, Cursor cur) throws SQLException;

    /**
     * WHERE CURRENT OF の実行前に、論理現在行へカーソル位置を補正する（PostgreSQL は先読み分の
     * FETCH BACKWARD と先読みバッファ破棄を内包）。
     *
     * @param c JDBC 接続
     * @param cur カーソル状態
     * @param sqlca SQLCA データストレージ
     * @throws SQLException データベースアクセスエラー
     */
    protected abstract void repositionForCurrentOf(Connection c, Cursor cur, CobolDataStorage sqlca)
            throws SQLException;

    /**
     * DB ごとの「生エラー → (ECPG 正規コード, 正規化 SQLSTATE)」変換。{@link SQLException} 全体を
     * 受け取るため、{@code getSQLState()} に加えて {@code getErrorCode()} も併用できる。
     *
     * <p>SQLCODE（整数）と SQLSTATE（文字列）の両方を 1 つのフックで一貫して返すことで、DB ごとに
     * 異なる生エラーを COBOL 側が見る ECPG 語彙へ正規化する。整数だけ変換して SQLSTATE 文字列を
     * 素通りさせると、SQLSTATE で分岐する COBOL が DB 依存の値を見てしまうため、両者を揃える。
     *
     * @param e DB から来た SQL 例外
     * @return ECPG 正規コードと正規化 SQLSTATE の組（{@link SqlErrorMapping}）
     */
    protected abstract SqlErrorMapping mapSqlException(SQLException e);

    // -------------------------------------------------------
    // 接続（共通フロー）
    // -------------------------------------------------------

    @Override
    public final void connect(
            CobolDataStorage sqlca,
            AbstractCobolField user,
            AbstractCobolField passwd,
            AbstractCobolField dbname) {
        try {
            String userStr = fieldToString(user);
            String passwdStr = fieldToString(passwd);
            String dbnameStr = fieldToString(dbname);

            // user/dbname は null になり得る (フィールド未指定 → fieldToString が null を返す)。
            // 引数の .trim() はログレベルに関わらず評価されるため、null セーフに整形する。
            LOG.trace("CONNECT user={} dbname={}", trimForLog(userStr), trimForLog(dbnameStr));
            DbSpec spec = buildSpec(userStr, passwdStr, dbnameStr);
            Connection conn = openConnection(spec);
            addConnection(DEFAULT_CONN_ID, conn);
            // CONNECT 成功時は SQLERRMC を上書きしない (COBOL 初期値のスペースを残す。Open COBOL ESQL 4Jに合わせる)。
            SqlCA.setSuccessKeepErrmc(sqlca);
            LOG.debug("CONNECT successful (id={})", DEFAULT_CONN_ID);
        } catch (SQLException e) {
            LOG.error("CONNECT failed: {}", e.getMessage());
            setResultFromException(sqlca, e);
        } catch (Exception e) {
            LOG.error("CONNECT failed: {}", e.getMessage());
            SqlCA.setError(sqlca, SqlCA.ECPG_CONNECT, "08001", e.getMessage());
        }
    }

    @Override
    public final void disconnect(CobolDataStorage sqlca) {
        try {
            String id = currentConnectionId();
            Connection conn = id == null ? null : connections.get(id);
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            LOG.debug("DISCONNECT (id={})", id);
            // 切断前に commit する（再 BEGIN はしない）。発行手段は DB 実装の commitTransaction に委ねる。
            try {
                commitTransaction(conn);
            } catch (SQLException ignored) {
                // 切断時の commit エラーは無視する
            }
            if (!conn.isClosed()) {
                conn.close();
            }
            removeConnection(id);
            // この接続でキャッシュした PreparedStatement を閉じてキャッシュから除く
            // (クローズ済み statement の再利用を防ぐ)。
            closeCachedStatements(conn);
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            LOG.error("DISCONNECT failed: {}", e.getMessage());
            setResultFromException(sqlca, e);
        }
    }

    /**
     * 接続文字列・環境変数フォールバック・末尾空白除去を DB 非依存に処理し、{@link DbSpec} を生成する。
     * URL の組み立てだけを {@link #buildJdbcUrl(DbSpec)} フックへ委ねる。
     */
    private DbSpec buildSpec(String user, String passwd, String dbname) {
        // 末尾の空白を除去する (COBOL のパディング)
        if (user != null) {
            user = stripTrailingSpaces(user);
        }
        if (passwd != null) {
            passwd = stripTrailingSpaces(passwd);
        }
        if (dbname != null) {
            dbname = stripTrailingSpaces(dbname);
        }

        // 環境変数によるフォールバック
        if (dbname == null || dbname.isEmpty()) {
            dbname = System.getenv("OCDB_DB_NAME");
        }
        if (user == null || user.isEmpty()) {
            user = System.getenv("OCDB_DB_USER");
        }
        if (passwd == null || passwd.isEmpty()) {
            passwd = System.getenv("OCDB_DB_PASS");
        }

        String encoding = System.getenv("OCDB_DB_CHAR");
        if (encoding == null || encoding.isEmpty()) {
            encoding = "UTF-8";
        }

        DbSpec spec = new DbSpec();
        if (dbname == null || dbname.isEmpty()) {
            // 旧 buildJdbcUrl の null/空ケース: jdbc:postgresql://localhost:5432/ 相当。
            spec.host = "localhost";
            spec.port = ":5432";
            spec.dbname = dbname == null ? "" : dbname;
        } else {
            String host = "localhost";
            String port = "";
            String db = dbname;
            // "dbname@host:port" を解析する
            int atIndex = db.lastIndexOf('@');
            if (atIndex >= 0) {
                host = db.substring(atIndex + 1);
                db = db.substring(0, atIndex);
            }
            int colonIndex = host.lastIndexOf(':');
            if (colonIndex >= 0) {
                port = ":" + host.substring(colonIndex + 1);
                host = host.substring(0, colonIndex);
            }
            if (host.isEmpty()) {
                host = "localhost";
            }
            spec.host = host;
            spec.port = port;
            spec.dbname = db;
        }
        spec.user = user;
        spec.passwd = passwd;
        spec.charset = encoding;
        return spec;
    }

    /** JDBC ドライバの（必要なら）明示ロードと接続確立、接続後初期化までを行う。 */
    private Connection openConnection(DbSpec spec) throws SQLException {
        String driver = driverClass();
        if (driver != null) {
            try {
                Class.forName(driver);
            } catch (ClassNotFoundException e) {
                throw new SQLException("JDBC driver not found: " + driver, e);
            }
        }

        String url = buildJdbcUrl(spec);

        Properties props = new Properties();
        if (spec.user != null && !spec.user.isEmpty()) {
            props.put("user", spec.user);
        }
        if (spec.passwd != null && !spec.passwd.isEmpty()) {
            props.put("password", spec.passwd);
        }
        props.put("encoding", spec.charset);

        CONN_LOG.debug("Connecting to {} (user={})", url, spec.user);
        Connection connection = DriverManager.getConnection(url, props);
        configureConnection(connection);
        CONN_LOG.debug("Connected successfully (id={})", DEFAULT_CONN_ID);
        return connection;
    }

    // -------------------------------------------------------
    // 単純な SQL の実行（ホスト変数なし）
    // -------------------------------------------------------

    @Override
    public final void exec(CobolDataStorage sqlca, String query) {
        try {
            Connection conn = getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            if (query == null || query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty query");
                return;
            }
            if (LOG.isDebugEnabled()) {
                LOG.debug("EXEC SQL: {}", collapseWhitespace(query));
            }
            try (Statement stmt = conn.createStatement()) {
                stmt.execute(query);
                int updateCount = stmt.getUpdateCount();
                if (updateCount >= 0) {
                    SqlCA.setRowCount(sqlca, updateCount);
                }
            }
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            LOG.error("EXEC SQL failed: {} - {}", collapseWhitespace(query), e.getMessage());
            setResultFromException(sqlca, e);
        }
    }

    @Override
    public final void execWithParams(
            CobolDataStorage sqlca, String query, AbstractCobolField... params) {
        try {
            Connection conn = getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            if (query == null || query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty query");
                return;
            }
            // collapseWhitespace は正規表現を伴うため、DEBUG 無効時に評価しないよう包む。
            if (LOG.isDebugEnabled()) {
                LOG.debug(
                        "EXEC SQL (params={}): {}",
                        params != null ? params.length : 0,
                        collapseWhitespace(query));
            }
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
                SqlCA.setRowCount(sqlca, updateCount);
            }
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            LOG.error("EXEC SQL failed: {} - {}", collapseWhitespace(query), e.getMessage());
            setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // WHERE CURRENT OF (位置付き UPDATE/DELETE)
    // -------------------------------------------------------

    @Override
    public final void execWhereCurrentOf(CobolDataStorage sqlca, String query, String cursorName) {
        String positioned = repositionForCurrentOfAndQualify(sqlca, query, cursorName);
        if (positioned != null) {
            exec(sqlca, positioned);
        }
    }

    @Override
    public final void execWithParamsWhereCurrentOf(
            CobolDataStorage sqlca, String query, String cursorName, AbstractCobolField... params) {
        String positioned = repositionForCurrentOfAndQualify(sqlca, query, cursorName);
        if (positioned != null) {
            execWithParams(sqlca, positioned, params);
        }
    }

    /**
     * WHERE CURRENT OF の実行前にカーソル位置を補正（{@link #repositionForCurrentOf} フック）し、
     * カーソル名を補った完全な SQL を返す。接続なし/カーソル未登録のときは SQLCA にエラーを設定し
     * {@code null} を返す。
     */
    private String repositionForCurrentOfAndQualify(
            CobolDataStorage sqlca, String query, String cursorName) {
        Connection conn = getDefaultConnection();
        if (conn == null) {
            SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
            return null;
        }
        Cursor cursor = cursors.get(cursorName);
        if (cursor == null) {
            // 未登録カーソル（Open COBOL ESQL 4J は OCDB_EMPTY を返す）。
            SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Cursor not found: " + cursorName);
            return null;
        }
        try {
            repositionForCurrentOf(conn, cursor, sqlca);
        } catch (SQLException e) {
            setResultFromException(sqlca, e);
            return null;
        }
        return query + " " + cursorName;
    }

    // -------------------------------------------------------
    // SELECT INTO
    // -------------------------------------------------------

    @Override
    public final void selectInto(
            CobolDataStorage sqlca,
            String query,
            AbstractCobolField[] inputParams,
            AbstractCobolField[] resultParams) {
        try {
            Connection conn = getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            if (query == null || query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty query");
                return;
            }
            if (LOG.isDebugEnabled()) {
                LOG.debug("SELECT INTO: {}", collapseWhitespace(query));
            }
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
            setResultFromException(sqlca, e);
        }
    }

    @Override
    public final void selectIntoOccurs(
            CobolDataStorage sqlca,
            int occursSize,
            int occursMax,
            String query,
            AbstractCobolField[] inputParams,
            AbstractCobolField[] resultParams) {
        try {
            Connection conn = getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
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
            setResultFromException(sqlca, e);
        }
    }

    private void processSelectIntoResults(
            ResultSet resultSet, AbstractCobolField[] resultParams, CobolDataStorage sqlca)
            throws SQLException {
        // 渡される ResultSet はキャッシュされた PreparedStatement のものであることがあり、
        // 文自体は閉じない。try-with-resources で、変換中の例外も含めどの経路でも
        // ResultSet を閉じる (サーバ側カーソルのリーク防止)。
        try (ResultSet rs = resultSet) {
            if (rs == null || !rs.next()) {
                SqlCA.setNotFound(sqlca);
                return;
            }
            if (resultParams == null || resultParams.length == 0) {
                SqlCA.setSuccess(sqlca);
                return;
            }
            // 各列を、対応する結果ホスト変数の COBOL 表現へ変換して書き込む。
            // OCCURS 配列への SELECT INTO は selectIntoOccurs が処理するため、ここは単一行。
            // 集団項目はコンパイル時に個別フィールドへ展開済みで、ここには展開後の
            // 個別フィールドだけが渡される。
            int columnCount = rs.getMetaData().getColumnCount();
            boolean sawNullWithoutIndicator = false;
            for (int i = 0; i < resultParams.length && i < columnCount; i++) {
                byte[] value = CobolDataConverter.getValueFromResultSet(rs, i + 1);
                if (value != null) {
                    CobolDataConverter.stringToCobol(resultParams[i], value);
                } else {
                    // 指標変数なしの NULL は型に応じた空値で埋める (memset 0 は packed/
                    // ゾーン10進で不正表現になる)。
                    CobolDataConverter.stringToCobol(resultParams[i], EMPTY_RESULT);
                    sawNullWithoutIndicator = true;
                }
            }
            if (sawNullWithoutIndicator) {
                SqlCA.setMissingIndicator(sqlca);
            } else {
                SqlCA.setSuccess(sqlca);
            }
        }
    }

    private void fetchOccursRows(
            ResultSet resultSet,
            AbstractCobolField[] resultParams,
            int occursSize,
            int occursMax,
            CobolDataStorage sqlca)
            throws SQLException {
        try (ResultSet rs = resultSet) {
            if (rs == null || !rs.next()) {
                // SELECT INTO ... OCCURS で結果 0 行は「該当なし」。
                SqlCA.setNotFound(sqlca);
                return;
            }
            writeOccursRows(rs, resultParams, occursSize, occursMax, sqlca);
        }
    }

    /**
     * 1 行目に位置付け済みの {@link ResultSet} から OCCURS 配列へ複数行を書き込み、行数・NULL 処理を
     * 含めて {@code sqlca} を更新する共通処理（SELECT INTO ... OCCURS とカーソルの複数行 FETCH で共用）。
     *
     * <p>呼び出し側は本メソッドの前に最初の {@code rs.next()} と 0 行時の扱い（SELECT INTO は
     * {@code NOT_FOUND}、カーソル FETCH の末尾は {@code SUCCESS}）を済ませておく。本メソッドは
     * 1 行以上ある前提で、{@code occursMax} 件まで各列を OCCURS のストライド {@code occursSize} で
     * 書き込み、{@code SQLERRD(3)} に取得行数を設定する。
     *
     * @param rs 1 行目に位置付け済みの ResultSet
     * @param resultParams 結果パラメータフィールド（列ごとに 1 つ）
     * @param occursSize OCCURS 要素 1 つあたりのバイトサイズ（ストライド）
     * @param occursMax 取得する行の最大数
     * @param sqlca SQLCA データストレージ
     * @throws SQLException データベースアクセスエラー
     */
    protected final void writeOccursRows(
            ResultSet rs,
            AbstractCobolField[] resultParams,
            int occursSize,
            int occursMax,
            CobolDataStorage sqlca)
            throws SQLException {
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
                    // 指標変数なしの NULL は型に応じた空値で埋める。raw な memset 0 は
                    // packed の符号ニブルやゾーン10進が不正表現になるため避ける。
                    CobolDataConverter.stringToCobolRaw(
                            resultParams[i], fieldStorage, fieldSize, EMPTY_RESULT);
                    sawNullWithoutIndicator = true;
                }
            }
            rowCount++;
        } while (rs.next());
        SqlCA.setRowCount(sqlca, rowCount);
        if (sawNullWithoutIndicator) {
            SqlCA.setMissingIndicator(sqlca);
        } else {
            SqlCA.setSuccess(sqlca);
        }
    }

    // -------------------------------------------------------
    // カーソル操作
    // -------------------------------------------------------

    @Override
    public final void declareCursor(CobolDataStorage sqlca, String cursorName, String query) {
        declareCursorInternal(sqlca, cursorName, query, null);
    }

    @Override
    public final void declareCursorWithParams(
            CobolDataStorage sqlca, String cursorName, String query, AbstractCobolField... params) {
        declareCursorInternal(sqlca, cursorName, query, params);
    }

    /**
     * カーソルを宣言して registry に登録する（パラメータ有無の共通処理）。{@code params} が
     * {@code null} の場合はパラメータなしの DECLARE と同じ（nParams=0・params 未バインド）。
     */
    private void declareCursorInternal(
            CobolDataStorage sqlca, String cursorName, String query, AbstractCobolField[] params) {
        try {
            if (cursorName == null || cursorName.isEmpty() || query == null || query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty cursor name or query");
                return;
            }
            if (LOG.isDebugEnabled()) {
                LOG.debug("DECLARE CURSOR {} FOR: {}", cursorName, collapseWhitespace(query));
            }
            Cursor existing = cursors.get(cursorName);
            if (existing != null && existing.isOpened) {
                SqlCA.setError(
                        sqlca, SqlCA.ECPG_WARNING_PORTAL_EXISTS, "42P03", "Cursor already opened");
                return;
            }
            Cursor cursor = new Cursor(cursorName, query, params != null ? params.length : 0);
            cursor.params = params;
            cursors.put(cursorName, cursor);
            SqlCA.setSuccess(sqlca);
        } catch (Exception e) {
            SqlCA.setError(sqlca, SqlCA.ECPG_PGSQL, "     ", e.getMessage());
        }
    }

    @Override
    public final void openCursor(CobolDataStorage sqlca, String cursorName) {
        openCursorInternal(sqlca, cursorName, null, true);
    }

    @Override
    public final void openCursorWithParams(
            CobolDataStorage sqlca, String cursorName, AbstractCobolField... params) {
        openCursorInternal(sqlca, cursorName, params, false);
    }

    private void openCursorInternal(
            CobolDataStorage sqlca, String cursorName, AbstractCobolField[] params, boolean log) {
        try {
            Connection conn = getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            Cursor cursor = cursors.get(cursorName);
            if (cursor == null) {
                SqlCA.setError(
                        sqlca,
                        SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                        "34000",
                        "Cursor not found: " + cursorName);
                return;
            }
            if (log) {
                LOG.debug("OPEN CURSOR {}", cursorName);
            }
            openCursorImpl(conn, cursor, params);
            cursor.isOpened = true;
            // 新たにオープンしたカーソルは先読みバッファを空から始める。
            cursor.clearBuffer();
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            LOG.error("OPEN CURSOR {} failed: {}", cursorName, e.getMessage());
            setResultFromException(sqlca, e);
        }
    }

    @Override
    public final void fetchCursor(
            CobolDataStorage sqlca, String cursorName, AbstractCobolField... resultParams) {
        try {
            Connection conn = getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            Cursor cursor = cursors.get(cursorName);
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
            // 登録済みだが未 OPEN (OPEN 失敗を含む) のカーソルでも短絡せず FETCH を DB へ送る
            // (Open COBOL ESQL 4J と同じ)。エラーは下の catch 経由で SQLCA に格納される。
            LOG.trace("FETCH CURSOR {}", cursorName);
            // 正常な fetch が sqlcode=0 になるよう事前にクリアしておく。指標変数なしで NULL の列が
            // ある場合、fetch が ECPG_MISSING_INDICATOR で上書きすることがある。
            SqlCA.setSuccess(sqlca);
            boolean hasRow = fetchRowImpl(conn, cursor, resultParams, sqlca);
            if (!hasRow) {
                SqlCA.setNotFound(sqlca);
            }
        } catch (SQLException e) {
            setResultFromException(sqlca, e);
        }
    }

    @Override
    public final void fetchCursorOccurs(
            CobolDataStorage sqlca,
            String cursorName,
            int occursSize,
            int occursMax,
            AbstractCobolField... resultParams) {
        try {
            Connection conn = getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            Cursor cursor = cursors.get(cursorName);
            if (cursor == null) {
                // 未 DECLARE のカーソル: -602/34000 を返す (Open COBOL ESQL 4Jと同様)。
                SqlCA.setRowCount(sqlca, 0);
                SqlCA.setError(
                        sqlca,
                        SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                        "34000",
                        "Cursor not found: " + cursorName);
                return;
            }
            // OCCURS への複数行 FETCH は単一行 FETCH の先読みバッファを使わない。
            // 同一カーソルに対する単一行 FETCH の先読みバッファが残っているとサーバカーソル位置と
            // 食い違うため、ここで破棄しておく。
            cursor.clearBuffer();
            // 取得行数 SQLERRD(3) を 0 で初期化してから委譲する。結果末尾 (0 行) の場合に
            // fetchOccursImpl が SQLCODE=0 のみ設定して早期 return しても、SQLERRD(3) が
            // 前回バッチの件数として残らないようにする (満杯バッチの次に 0 行 FETCH した際の
            // stale な行数による再処理・無限ループを防ぐ)。通常パスは実件数で上書きする。
            SqlCA.setRowCount(sqlca, 0);
            fetchOccursImpl(conn, cursor, occursSize, occursMax, resultParams, sqlca);
        } catch (SQLException e) {
            // フェッチ失敗時は取得行数 SQLERRD(3) を 0 にしてから DB エラーを反映する。
            SqlCA.setRowCount(sqlca, 0);
            setResultFromException(sqlca, e);
        }
    }

    @Override
    public final void closeCursor(CobolDataStorage sqlca, String cursorName) {
        try {
            Connection conn = getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            Cursor cursor = cursors.get(cursorName);
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
            closeCursorImpl(conn, cursor);
            cursor.isOpened = false;
            cursor.clearBuffer();
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // prepared statement の操作
    // -------------------------------------------------------

    @Override
    public final void prepare(
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

            // ホスト変数のプレースホルダ(:name)を数えて ? に置き換える。
            // 文字列リテラル / 引用識別子の内側と、PostgreSQL の型キャスト演算子 (::)
            // は対象外とする(これらの中の ':' をプレースホルダと誤認しない)。
            int nParams = 0;
            int len = query.length();
            StringBuilder replaced = new StringBuilder();
            char quote = 0; // 文字列リテラル/引用識別子の内側にいるときの引用符 (' または ")
            for (int i = 0; i < len; i++) {
                char c = query.charAt(i);
                if (quote != 0) {
                    // リテラル/引用識別子の内側はそのまま通す。'' / "" は埋め込みエスケープ。
                    replaced.append(c);
                    if (c == quote) {
                        if (i + 1 < len && query.charAt(i + 1) == quote) {
                            replaced.append(quote);
                            i++;
                        } else {
                            quote = 0;
                        }
                    }
                } else if (c == '\'' || c == '"') {
                    quote = c;
                    replaced.append(c);
                } else if (c == ':' && i + 1 < len && query.charAt(i + 1) == ':') {
                    // '::' は型キャスト演算子。ホスト変数ではないのでそのまま通す。
                    replaced.append("::");
                    i++;
                } else if (c == ':' && i + 1 < len && Character.isLetter(query.charAt(i + 1))) {
                    nParams++;
                    replaced.append('?');
                    i++;
                    // 名前を構成する文字の間だけ進める。空白・改行・演算子などはすべて終端。
                    // ':' も名前文字ではないため、:name 直後の '::' キャストは外側ループで処理される。
                    while (i < len && isHostVarNameChar(query.charAt(i))) {
                        i++;
                    }
                    i--;
                } else {
                    replaced.append(c);
                }
            }

            prepared.put(stmtName, new String[] {replaced.toString(), String.valueOf(nParams)});
            SqlCA.setSuccess(sqlca);
        } catch (Exception e) {
            SqlCA.setError(sqlca, SqlCA.ECPG_PGSQL, "     ", e.getMessage());
        }
    }

    @Override
    public final void executePrepared(
            CobolDataStorage sqlca, String stmtName, AbstractCobolField... params) {
        try {
            String[] preparedStmt = prepared.get(stmtName);
            if (preparedStmt == null) {
                SqlCA.setError(
                        sqlca,
                        SqlCA.ECPG_INVALID_STMT,
                        "26000",
                        "Statement not found: " + stmtName);
                return;
            }
            String query = preparedStmt[0];
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
     * commit して新しいトランザクションを継続する（確定 + 再 BEGIN）。{@link #commit(CobolDataStorage)}
     * の公開操作で使う。確定手段は {@link #commitTransaction(Connection)}、再開は
     * {@link #beginTransaction(Connection)} に委ねる（暗黙 BEGIN の DB では後者が no-op）。
     */
    private void doCommit(Connection c) throws SQLException {
        commitTransaction(c);
        beginTransaction(c);
    }

    /** rollback して新しいトランザクションを継続する（ロールバック + 再 BEGIN）。 */
    private void doRollback(Connection c) throws SQLException {
        rollbackTransaction(c);
        beginTransaction(c);
    }

    @Override
    public final void commit(CobolDataStorage sqlca) {
        try {
            Connection conn = getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            doCommit(conn);
            SqlCA.setSuccess(sqlca);
            clearCursors();
        } catch (SQLException e) {
            setResultFromException(sqlca, e);
        }
    }

    @Override
    public final void rollback(CobolDataStorage sqlca) {
        try {
            Connection conn = getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            doRollback(conn);
            SqlCA.setSuccess(sqlca);
            clearCursors();
        } catch (SQLException e) {
            setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // registry（旧 SqlState のインスタンス化）
    // -------------------------------------------------------

    /** 接続を登録する。最初に登録された接続がデフォルトになる。 */
    void addConnection(String id, Connection conn) {
        connections.put(id, conn);
        if (defaultConnId == null) {
            defaultConnId = id;
        }
    }

    /** ID を指定して接続を削除する。それがデフォルトだった場合は、新しいデフォルトが選択される。 */
    private void removeConnection(String id) {
        connections.remove(id);
        if (id != null && id.equals(defaultConnId)) {
            if (!connections.isEmpty()) {
                defaultConnId = connections.keySet().iterator().next();
            } else {
                defaultConnId = null;
            }
        }
    }

    /** 現在のデフォルト接続の識別子を返す（旧 SqlState.getDefaultConnection の選択規則）。 */
    private String currentConnectionId() {
        if (defaultConnId != null && connections.containsKey(defaultConnId)) {
            return defaultConnId;
        }
        if (!connections.isEmpty()) {
            return connections.keySet().iterator().next();
        }
        return null;
    }

    /** 現在のデフォルト接続の JDBC Connection を返す。未登録なら null。 */
    Connection getDefaultConnection() {
        String id = currentConnectionId();
        return id == null ? null : connections.get(id);
    }

    /** すべてのカーソルをクローズ済みとしてマークする（例: COMMIT や ROLLBACK の後）。 */
    private void clearCursors() {
        for (Cursor cursor : cursors.values()) {
            cursor.isOpened = false;
            // COMMIT/ROLLBACK でサーバカーソルは消えるため、先読みバッファも破棄する。
            cursor.clearBuffer();
        }
    }

    // -------------------------------------------------------
    // テスト支援（package-private。本番 API には露出しない）
    // -------------------------------------------------------

    /** テスト用: カーソルを直接登録する。 */
    final void addCursor(String name, Cursor cursor) {
        cursors.put(name, cursor);
    }

    /** テスト用: 登録済みカーソルを取得する。 */
    final Cursor getCursor(String name) {
        return cursors.get(name);
    }

    /** テスト用: prepared statement を直接登録する。 */
    final void addPrepared(String name, String query, int nParams) {
        prepared.put(name, new String[] {query, String.valueOf(nParams)});
    }

    /** テスト用: 登録済み prepared statement（[query, nParams]）を取得する。 */
    final String[] getPrepared(String name) {
        return prepared.get(name);
    }

    /** テスト用: 接続文字列のパース・env フォールバック結果（{@link DbSpec}）を返す。 */
    final DbSpec buildSpecForTest(String user, String passwd, String dbname) {
        return buildSpec(user, passwd, dbname);
    }

    /** テスト用: 登録済みの全 JDBC 接続を閉じ、レジストリと文キャッシュをクリアする。 */
    final void closeAllConnectionsForTest() {
        for (Connection c : connections.values()) {
            try {
                if (c != null && !c.isClosed()) {
                    c.close();
                }
            } catch (SQLException ignored) {
                // テスト後始末のエラーは無視する
            }
        }
        connections.clear();
        defaultConnId = null;
        stmtCache.clear();
    }

    // -------------------------------------------------------
    // SQLCA エラー報告（共通フロー + DB 別変換フック）
    // -------------------------------------------------------

    /**
     * {@link SQLException} から SQLCA を設定する。ECPG コードと SQLSTATE への変換を
     * {@link #mapSqlException(SQLException)} フックへ委譲し、構造体の書き込みは共通。
     *
     * @param sqlca SQLCA のデータストレージ
     * @param e SQL 例外
     */
    protected final void setResultFromException(CobolDataStorage sqlca, SQLException e) {
        if (sqlca == null) {
            return;
        }
        SqlErrorMapping mapping = mapSqlException(e);
        String sqlState = mapping.sqlState;
        if (sqlState == null) {
            sqlState = "     ";
        }
        String message = e.getMessage();
        if (message == null) {
            message = "";
        }
        SqlCA.setError(sqlca, mapping.ecpgCode, sqlState, message);
    }

    // -------------------------------------------------------
    // PreparedStatement キャッシュ
    // -------------------------------------------------------

    /** 接続ごとに SQL 文字列をキーとして PreparedStatement をキャッシュする。 */
    protected final PreparedStatement getOrCreatePreparedStatement(Connection conn, String query)
            throws SQLException {
        Map<String, PreparedStatement> perConnection =
                stmtCache.computeIfAbsent(conn, c -> new ConcurrentHashMap<>());
        PreparedStatement cached = perConnection.get(query);
        if (cached != null) {
            return cached;
        }
        // prepareStatement の SQLException はそのまま呼び出し元へ伝播させる。
        PreparedStatement pstmt = conn.prepareStatement(query);
        perConnection.put(query, pstmt);
        return pstmt;
    }

    /** 指定した接続でキャッシュした PreparedStatement をすべて閉じ、キャッシュから取り除く。 */
    private void closeCachedStatements(Connection conn) {
        Map<String, PreparedStatement> perConnection = stmtCache.remove(conn);
        if (perConnection == null) {
            return;
        }
        for (PreparedStatement pstmt : perConnection.values()) {
            try {
                pstmt.close();
            } catch (SQLException ignored) {
                // 既に閉じられている場合などのエラーは無視する
            }
        }
    }

    // -------------------------------------------------------
    // ヘルパー
    // -------------------------------------------------------

    /** ログ出力用に、空白文字（改行、タブ、連続するスペース）を単一のスペースにまとめる。 */
    private static String collapseWhitespace(String s) {
        return s.replaceAll("\\s+", " ").trim();
    }

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

    /** ログ出力用に末尾空白を除去する。null も安全に扱う (接続パラメータ未指定時のため)。 */
    private static String trimForLog(String s) {
        return s == null ? null : s.trim();
    }

    /** :name ホスト変数名を構成しうる文字 (COBOL データ名: 英数字・ハイフン、加えてアンダースコア) か。 */
    private static boolean isHostVarNameChar(char c) {
        return Character.isLetterOrDigit(c) || c == '-' || c == '_';
    }

    private static String stripTrailingSpaces(String str) {
        if (str == null) {
            return null;
        }
        // COBOL の固定長フィールド由来の末尾空白のみ除去する。
        // 値の途中に含まれる空白 (例: 空白を含むパスワード) は保持する。
        int end = str.length();
        while (end > 0 && str.charAt(end - 1) == ' ') {
            end--;
        }
        return str.substring(0, end);
    }

    // -------------------------------------------------------
    // 接続パラメータ・カーソル状態（DB 非依存）
    // -------------------------------------------------------

    /**
     * {@link #mapSqlException(SQLException)} の戻り値。DB の生エラーを、COBOL 側が見る
     * ECPG 正規コード（SQLCODE）と正規化済み SQLSTATE の組へ変換した結果を保持する。
     */
    protected static final class SqlErrorMapping {
        /** ECPG 正規エラーコード（SQLCODE に書かれる）。 */
        final int ecpgCode;

        /** 正規化済み SQLSTATE（5 桁、SQLSTATE に書かれる）。{@code null} のとき共通側で空白を補う。 */
        final String sqlState;

        /**
         * @param ecpgCode ECPG 正規エラーコード（SQLCODE）
         * @param sqlState 正規化済み SQLSTATE（5 桁、{@code null} 可）
         */
        protected SqlErrorMapping(int ecpgCode, String sqlState) {
            this.ecpgCode = ecpgCode;
            this.sqlState = sqlState;
        }
    }

    /**
     * {@link #buildJdbcUrl(DbSpec)} の入力。接続文字列 {@code dbname@host:port} のパース・
     * 環境変数フォールバック・末尾空白除去まで適用済みの、DB 非依存な接続情報。
     */
    protected static final class DbSpec {
        /** ホスト名（既定 "localhost"）。 */
        String host;

        /** ポート（":port" 形式、未指定なら空文字）。 */
        String port;

        /** データベース名。 */
        String dbname;

        /** 接続ユーザー名。 */
        String user;

        /** 接続パスワード。 */
        String passwd;

        /** 文字コード（OCDB_DB_CHAR、既定 UTF-8）。 */
        String charset;
    }

    /**
     * カーソルの状態保持オブジェクト（旧 {@code SqlCursor} の状態部）。DECLARE/FETCH/CLOSE の
     * 振る舞いはバックエンドのフックが担い、本クラスは状態のみを持つ。
     */
    protected static final class Cursor {

        /** DECLARE/OPEN/FETCH/CLOSE 文で使用されるカーソル名。 */
        String name;

        /** このカーソルに紐づく SQL クエリ。 */
        String query;

        /** クエリ中のホスト変数パラメータの数。 */
        int nParams;

        /** このカーソルが現在オープンされているかどうか。 */
        boolean isOpened;

        /** DECLARE 時にバインドされたホスト変数パラメータ。 */
        AbstractCobolField[] params;

        /**
         * 先読み（バルクフェッチ）バッファ。各要素は 1 行ぶんの列値配列で、SQL NULL の列は null。
         * PostgreSQL（サーバカーソル）の {@code FETCH FORWARD N} 先読みに使う。
         */
        List<byte[][]> fetchBuffer = new ArrayList<>();

        /** {@link #fetchBuffer} 内で次に供給する行の位置。 */
        int bufferPos;

        /**
         * 直近の先読みが要求件数より少ない行数で終わった（＝結果末尾に達した）かどうか。
         * WHERE CURRENT OF のカーソル位置補正に使う（Open COBOL ESQL 4J の overFetch 相当）。
         */
        boolean overFetch;

        Cursor(String name, String query, int nParams) {
            this.name = name;
            this.query = query;
            this.nParams = nParams;
            this.isOpened = false;
            this.params = null;
        }

        /** まだ COBOL 側へ供給していない（バッファに残っている）先読み行数を返す。 */
        int remainingBuffered() {
            return fetchBuffer.size() - bufferPos;
        }

        /** 先読みバッファと overFetch フラグをリセットする。 */
        void clearBuffer() {
            fetchBuffer = new ArrayList<>();
            bufferPos = 0;
            overFetch = false;
        }
    }
}

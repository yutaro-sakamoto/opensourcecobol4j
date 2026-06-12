package jp.osscons.opensourcecobol.libcobj.sql;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Properties;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/** JDBC の Connection を COBOL の接続識別子とともにラップする。 */
class SqlConnection {

    private static final Logger LOG = LoggerFactory.getLogger(SqlConnection.class);

    private String id;
    private Connection conn;

    SqlConnection(String id, Connection conn) {
        this.id = id;
        this.conn = conn;
    }

    /**
     * 接続識別子を取得する。
     *
     * @return 接続 ID 文字列
     */
    String getId() {
        return id;
    }

    /**
     * 内部で保持する JDBC 接続を取得する。
     *
     * @return JDBC の Connection
     */
    Connection getConnection() {
        return conn;
    }

    /**
     * 内部で保持する JDBC 接続が開いていればクローズする。
     *
     * @throws SQLException データベースアクセスエラーが発生した場合
     */
    void close() throws SQLException {
        if (conn != null && !conn.isClosed()) {
            conn.close();
        }
    }

    /**
     * SQL の BEGIN 文を実行して新しいトランザクションを開始する。
     *
     * @throws SQLException データベースアクセスエラーが発生した場合
     */
    void beginTransaction() throws SQLException {
        if (conn != null && !conn.isClosed()) {
            try (Statement stmt = conn.createStatement()) {
                stmt.execute("BEGIN");
            }
        }
    }

    static String buildJdbcUrl(String dbSpec) {
        if (dbSpec == null || dbSpec.isEmpty()) {
            return "jdbc:postgresql://localhost:5432/";
        }
        String host = "localhost";
        String port = "";
        String dbname = dbSpec;

        // "dbname@host:port" を解析する
        int atIndex = dbname.lastIndexOf('@');
        if (atIndex >= 0) {
            host = dbname.substring(atIndex + 1);
            dbname = dbname.substring(0, atIndex);
        }

        int colonIndex = host.lastIndexOf(':');
        if (colonIndex >= 0) {
            port = ":" + host.substring(colonIndex + 1);
            host = host.substring(0, colonIndex);
        }

        // dbname 自体に ":port" が含まれているか (フォーマット: "dbname@host:port") も確認する
        // 先に host を分割しているため、上記で既に処理済み

        if (host.isEmpty()) {
            host = "localhost";
        }

        return "jdbc:postgresql://" + host + port + "/" + dbname;
    }

    static SqlConnection connect(String user, String passwd, String dbname) throws SQLException {
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

        String url = buildJdbcUrl(dbname);

        String encoding = System.getenv("OCDB_DB_CHAR");
        if (encoding == null || encoding.isEmpty()) {
            encoding = "UTF-8";
        }

        Properties props = new Properties();
        if (user != null && !user.isEmpty()) {
            props.put("user", user);
        }
        if (passwd != null && !passwd.isEmpty()) {
            props.put("password", passwd);
        }
        props.put("encoding", encoding);

        LOG.debug("Connecting to {} (user={})", url, user);
        Connection connection = DriverManager.getConnection(url, props);
        connection.setAutoCommit(true);

        String connId = "OCDB_DEFAULT_DBNAME";
        SqlConnection sqlConn = new SqlConnection(connId, connection);

        // トランザクションを開始する
        sqlConn.beginTransaction();
        LOG.debug("Connected successfully (id={})", connId);

        return sqlConn;
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
}

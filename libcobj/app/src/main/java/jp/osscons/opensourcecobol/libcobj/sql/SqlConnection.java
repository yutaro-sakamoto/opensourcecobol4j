package jp.osscons.opensourcecobol.libcobj.sql;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Properties;

/** Wraps a JDBC Connection with a COBOL connection identifier. */
public class SqlConnection {

    private String id;
    private Connection conn;

    SqlConnection(String id, Connection conn) {
        this.id = id;
        this.conn = conn;
    }

    /**
     * Get the connection identifier.
     *
     * @return the connection ID string
     */
    public String getId() {
        return id;
    }

    /**
     * Get the underlying JDBC connection.
     *
     * @return the JDBC Connection
     */
    public Connection getConnection() {
        return conn;
    }

    /**
     * Close the underlying JDBC connection if it is open.
     *
     * @throws SQLException if a database access error occurs
     */
    public void close() throws SQLException {
        if (conn != null && !conn.isClosed()) {
            conn.close();
        }
    }

    /**
     * Begin a new transaction by executing a SQL BEGIN statement.
     *
     * @throws SQLException if a database access error occurs
     */
    public void beginTransaction() throws SQLException {
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

        // Parse "dbname@host:port"
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

        // Also check if dbname itself has ":port" (format: "dbname@host:port")
        // Already handled above since we split host first

        if (host.isEmpty()) {
            host = "localhost";
        }

        return "jdbc:postgresql://" + host + port + "/" + dbname;
    }

    static SqlConnection connect(String user, String passwd, String dbname) throws SQLException {
        // Strip trailing spaces (COBOL padding)
        if (user != null) {
            user = stripTrailingSpaces(user);
        }
        if (passwd != null) {
            passwd = stripTrailingSpaces(passwd);
        }
        if (dbname != null) {
            dbname = stripTrailingSpaces(dbname);
        }

        // Environment variable fallbacks
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

        Connection connection = DriverManager.getConnection(url, props);
        connection.setAutoCommit(true);

        String connId = "OCDB_DEFAULT_DBNAME";
        SqlConnection sqlConn = new SqlConnection(connId, connection);

        // Start transaction
        sqlConn.beginTransaction();

        return sqlConn;
    }

    private static String stripTrailingSpaces(String str) {
        if (str == null) {
            return null;
        }
        int end = str.indexOf(' ');
        if (end <= 0) {
            return str;
        }
        return str.substring(0, end);
    }
}

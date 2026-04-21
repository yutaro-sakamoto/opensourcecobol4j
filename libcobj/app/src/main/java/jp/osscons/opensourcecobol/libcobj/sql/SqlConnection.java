package jp.osscons.opensourcecobol.libcobj.sql;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Properties;

public class SqlConnection {

    private String id;
    private Connection conn;

    SqlConnection(String id, Connection conn) {
        this.id = id;
        this.conn = conn;
    }

    public String getId() {
        return id;
    }

    public Connection getConnection() {
        return conn;
    }

    public void close() throws SQLException {
        if (conn != null && !conn.isClosed()) {
            conn.close();
        }
    }

    public void beginTransaction() throws SQLException {
        if (conn != null && !conn.isClosed()) {
            Statement stmt = conn.createStatement();
            try {
                stmt.execute("BEGIN");
            } finally {
                stmt.close();
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
        if (user != null) user = stripTrailingSpaces(user);
        if (passwd != null) passwd = stripTrailingSpaces(passwd);
        if (dbname != null) dbname = stripTrailingSpaces(dbname);

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
        if (str == null) return null;
        int end = str.indexOf(' ');
        if (end <= 0) {
            return str;
        }
        return str.substring(0, end);
    }
}

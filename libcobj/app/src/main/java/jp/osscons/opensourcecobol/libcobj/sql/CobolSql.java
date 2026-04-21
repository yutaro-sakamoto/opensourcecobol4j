package jp.osscons.opensourcecobol.libcobj.sql;

import java.nio.charset.Charset;
import java.sql.Connection;
import java.sql.ParameterMetaData;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.concurrent.ConcurrentHashMap;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

public class CobolSql {

    private static final Charset SHIFT_JIS = Charset.forName("SHIFT-JIS");
    private static final ConcurrentHashMap<String, PreparedStatement> stmtCache =
            new ConcurrentHashMap<>();

    // -------------------------------------------------------
    // Connection
    // -------------------------------------------------------
    public static void connect(
            CobolDataStorage sqlca,
            CobolDataStorage user,
            int userLen,
            CobolDataStorage passwd,
            int passwdLen,
            CobolDataStorage dbname,
            int dbnameLen) {
        try {
            String userStr = storageToString(user, userLen);
            String passwdStr = storageToString(passwd, passwdLen);
            String dbnameStr = storageToString(dbname, dbnameLen);

            SqlConnection conn = SqlConnection.connect(userStr, passwdStr, dbnameStr);
            SqlState.addConnection(conn.getId(), conn);
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        } catch (Exception e) {
            SqlCA.setError(sqlca, SqlCA.ECPG_CONNECT, "08001", e.getMessage());
        }
    }

    public static void connectInformal(CobolDataStorage sqlca, CobolDataStorage connInfo, int len) {
        try {
            String info = storageToString(connInfo, len);
            if (info == null || info.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_CONNECT, "08001", "Connection info is NULL");
                return;
            }

            // Parse "user/passwd@dbname" format
            String user = null;
            String passwd = null;
            String dbname = null;

            int atIdx = info.lastIndexOf('@');
            String rest = info;
            if (atIdx >= 0) {
                dbname = info.substring(atIdx + 1);
                rest = info.substring(0, atIdx);
            }

            int slashIdx = rest.indexOf('/');
            if (slashIdx >= 0) {
                user = rest.substring(0, slashIdx);
                passwd = rest.substring(slashIdx + 1);
            } else {
                user = rest;
            }

            SqlConnection conn = SqlConnection.connect(user, passwd, dbname);
            SqlState.addConnection(conn.getId(), conn);
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        } catch (Exception e) {
            SqlCA.setError(sqlca, SqlCA.ECPG_CONNECT, "08001", e.getMessage());
        }
    }

    public static void connectShort(CobolDataStorage sqlca) {
        try {
            SqlConnection conn = SqlConnection.connect(null, null, null);
            SqlState.addConnection(conn.getId(), conn);
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        } catch (Exception e) {
            SqlCA.setError(sqlca, SqlCA.ECPG_CONNECT, "08001", e.getMessage());
        }
    }

    public static void disconnect(CobolDataStorage sqlca) {
        try {
            SqlConnection conn = SqlState.getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            // Commit before disconnect
            try {
                Statement stmt = conn.getConnection().createStatement();
                stmt.execute("COMMIT");
                stmt.close();
            } catch (SQLException ignored) {
                // Ignore commit errors on disconnect
            }
            conn.close();
            SqlState.removeConnection(conn.getId());
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // Simple SQL execution (no host vars)
    // -------------------------------------------------------
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

            Statement stmt = conn.createStatement();
            try {
                stmt.execute(query);
            } finally {
                stmt.close();
            }

            SqlCA.setSuccess(sqlca);

            if ("COMMIT".equalsIgnoreCase(query) || "ROLLBACK".equalsIgnoreCase(query)) {
                SqlState.clearCursors();
                sqlConn.beginTransaction();
            }
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // Parameterized SQL execution
    // -------------------------------------------------------
    public static void execWithParams(CobolDataStorage sqlca, String query, SqlParam... params) {
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

            PreparedStatement pstmt = getOrCreatePreparedStatement(conn, query);
            try {
                ParameterMetaData metaData = getParameterMetaData(pstmt);
                if (params != null) {
                    for (int i = 0; i < params.length; i++) {
                        CobolDataConverter.setParam(pstmt, i + 1, metaData, params[i]);
                    }
                }
                pstmt.execute();
            } catch (SQLException e) {
                throw e;
            }

            SqlCA.setSuccess(sqlca);

            if ("COMMIT".equalsIgnoreCase(query) || "ROLLBACK".equalsIgnoreCase(query)) {
                SqlState.clearCursors();
                sqlConn.beginTransaction();
            }
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // SELECT INTO
    // -------------------------------------------------------
    public static void selectInto(
            CobolDataStorage sqlca, String query, SqlParam[] inputParams, SqlParam[] resultParams) {
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

            ResultSet rs;
            if (inputParams != null && inputParams.length > 0) {
                PreparedStatement pstmt = getOrCreatePreparedStatement(conn, query);
                ParameterMetaData metaData = getParameterMetaData(pstmt);
                for (int i = 0; i < inputParams.length; i++) {
                    CobolDataConverter.setParam(pstmt, i + 1, metaData, inputParams[i]);
                }
                pstmt.execute();
                rs = pstmt.getResultSet();
            } else {
                Statement stmt = conn.createStatement();
                stmt.execute(query);
                rs = stmt.getResultSet();
            }

            if (rs == null || !rs.next()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NOT_FOUND, "02000", "No data found");
                if (rs != null) rs.close();
                return;
            }

            if (resultParams != null) {
                int columnCount = rs.getMetaData().getColumnCount();
                for (int i = 0; i < resultParams.length && i < columnCount; i++) {
                    byte[] value = CobolDataConverter.getValueFromResultSet(rs, i + 1);
                    if (value != null) {
                        CobolDataConverter.stringToCobol(resultParams[i], value);
                    } else {
                        resultParams[i].storage.memset((byte) 0, resultParams[i].length);
                    }
                }
            }
            rs.close();
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // Cursor operations
    // -------------------------------------------------------
    public static void declareCursor(CobolDataStorage sqlca, String cursorName, String query) {
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
            SqlCursor cursor = new SqlCursor(cursorName, query, 0);
            SqlState.addCursor(cursorName, cursor);
            SqlCA.setSuccess(sqlca);
        } catch (Exception e) {
            SqlCA.setError(sqlca, SqlCA.ECPG_PGSQL, "     ", e.getMessage());
        }
    }

    public static void declareCursorWithParams(
            CobolDataStorage sqlca, String cursorName, String query, SqlParam... params) {
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
            cursor.open(sqlConn.getConnection(), null);
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    public static void openCursorWithParams(
            CobolDataStorage sqlca, String cursorName, SqlParam... params) {
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
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    public static void fetchCursor(
            CobolDataStorage sqlca, String cursorName, SqlParam... resultParams) {
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
            boolean hasRow = cursor.fetch(sqlConn.getConnection(), resultParams);
            if (hasRow) {
                SqlCA.setSuccess(sqlca);
            } else {
                SqlCA.setError(sqlca, SqlCA.ECPG_NOT_FOUND, "02000", "No data");
            }
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    public static void closeCursor(CobolDataStorage sqlca, String cursorName) {
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
            cursor.close(sqlConn.getConnection());
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // Prepared statements
    // -------------------------------------------------------
    public static void prepare(
            CobolDataStorage sqlca, String stmtName, CobolDataStorage queryStorage, int queryLen) {
        try {
            if (stmtName == null || stmtName.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty statement name");
                return;
            }
            String query;
            if (queryStorage != null && queryLen > 0) {
                byte[] bytes = queryStorage.getByteArray(0, queryLen);
                query = new String(bytes, SHIFT_JIS).trim();
            } else {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty query");
                return;
            }

            // Count and replace host variable placeholders
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

    public static void executePrepared(
            CobolDataStorage sqlca, String stmtName, SqlParam... params) {
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
    // Transaction
    // -------------------------------------------------------
    public static void commit(CobolDataStorage sqlca) {
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            Connection conn = sqlConn.getConnection();
            Statement stmt = conn.createStatement();
            try {
                stmt.execute("COMMIT");
            } finally {
                stmt.close();
            }
            SqlCA.setSuccess(sqlca);
            SqlState.clearCursors();
            sqlConn.beginTransaction();
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    public static void rollback(CobolDataStorage sqlca) {
        try {
            SqlConnection sqlConn = SqlState.getDefaultConnection();
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            Connection conn = sqlConn.getConnection();
            Statement stmt = conn.createStatement();
            try {
                stmt.execute("ROLLBACK");
            } finally {
                stmt.close();
            }
            SqlCA.setSuccess(sqlca);
            SqlState.clearCursors();
            sqlConn.beginTransaction();
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // AT database variants (ID prefixed)
    // -------------------------------------------------------
    public static void idConnect(
            CobolDataStorage sqlca,
            CobolDataStorage atdb,
            int atdbLen,
            CobolDataStorage user,
            int userLen,
            CobolDataStorage passwd,
            int passwdLen,
            CobolDataStorage dbname,
            int dbnameLen) {
        try {
            String atdbStr = storageToString(atdb, atdbLen);
            String userStr = storageToString(user, userLen);
            String passwdStr = storageToString(passwd, passwdLen);
            String dbnameStr = storageToString(dbname, dbnameLen);

            SqlConnection conn = SqlConnection.connect(userStr, passwdStr, dbnameStr);
            String connId = atdbStr != null && !atdbStr.isEmpty() ? atdbStr : conn.getId();
            SqlState.addConnection(connId, conn);
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        } catch (Exception e) {
            SqlCA.setError(sqlca, SqlCA.ECPG_CONNECT, "08001", e.getMessage());
        }
    }

    public static void idExec(
            CobolDataStorage sqlca, CobolDataStorage atdb, int atdbLen, String query) {
        try {
            String atdbStr = storageToString(atdb, atdbLen);
            SqlConnection sqlConn = SqlState.getConnection(atdbStr);
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection: " + atdbStr);
                return;
            }
            Connection conn = sqlConn.getConnection();

            if (query == null || query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty query");
                return;
            }

            Statement stmt = conn.createStatement();
            try {
                stmt.execute(query);
            } finally {
                stmt.close();
            }
            SqlCA.setSuccess(sqlca);

            if ("COMMIT".equalsIgnoreCase(query) || "ROLLBACK".equalsIgnoreCase(query)) {
                SqlState.clearCursors();
                sqlConn.beginTransaction();
            }
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    public static void idExecParams(
            CobolDataStorage sqlca,
            CobolDataStorage atdb,
            int atdbLen,
            String query,
            int nParams,
            SqlParam... params) {
        try {
            String atdbStr = storageToString(atdb, atdbLen);
            SqlConnection sqlConn = SqlState.getConnection(atdbStr);
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection: " + atdbStr);
                return;
            }
            Connection conn = sqlConn.getConnection();

            if (query == null || query.isEmpty()) {
                SqlCA.setError(sqlca, SqlCA.ECPG_EMPTY, "YE002", "Empty query");
                return;
            }

            PreparedStatement pstmt = getOrCreatePreparedStatement(conn, query);
            ParameterMetaData metaData = getParameterMetaData(pstmt);
            if (params != null) {
                for (int i = 0; i < params.length; i++) {
                    CobolDataConverter.setParam(pstmt, i + 1, metaData, params[i]);
                }
            }
            pstmt.execute();
            SqlCA.setSuccess(sqlca);

            if ("COMMIT".equalsIgnoreCase(query) || "ROLLBACK".equalsIgnoreCase(query)) {
                SqlState.clearCursors();
                sqlConn.beginTransaction();
            }
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    public static void idDisconnect(CobolDataStorage sqlca, CobolDataStorage atdb, int atdbLen) {
        try {
            String atdbStr = storageToString(atdb, atdbLen);
            SqlConnection conn = SqlState.getConnection(atdbStr);
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection: " + atdbStr);
                return;
            }
            try {
                Statement stmt = conn.getConnection().createStatement();
                stmt.execute("COMMIT");
                stmt.close();
            } catch (SQLException ignored) {
                // Ignore commit errors on disconnect
            }
            conn.close();
            SqlState.removeConnection(atdbStr);
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    public static void idCommit(CobolDataStorage sqlca, CobolDataStorage atdb, int atdbLen) {
        try {
            String atdbStr = storageToString(atdb, atdbLen);
            SqlConnection sqlConn = SqlState.getConnection(atdbStr);
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection: " + atdbStr);
                return;
            }
            Connection conn = sqlConn.getConnection();
            Statement stmt = conn.createStatement();
            try {
                stmt.execute("COMMIT");
            } finally {
                stmt.close();
            }
            SqlCA.setSuccess(sqlca);
            SqlState.clearCursors();
            sqlConn.beginTransaction();
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    public static void idRollback(CobolDataStorage sqlca, CobolDataStorage atdb, int atdbLen) {
        try {
            String atdbStr = storageToString(atdb, atdbLen);
            SqlConnection sqlConn = SqlState.getConnection(atdbStr);
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection: " + atdbStr);
                return;
            }
            Connection conn = sqlConn.getConnection();
            Statement stmt = conn.createStatement();
            try {
                stmt.execute("ROLLBACK");
            } finally {
                stmt.close();
            }
            SqlCA.setSuccess(sqlca);
            SqlState.clearCursors();
            sqlConn.beginTransaction();
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // Helper methods
    // -------------------------------------------------------
    private static String storageToString(CobolDataStorage storage, int len) {
        if (storage == null || len <= 0) {
            return null;
        }
        byte[] bytes = storage.getByteArray(0, len);
        return new String(bytes, SHIFT_JIS);
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

    private static ParameterMetaData getParameterMetaData(PreparedStatement pstmt) {
        try {
            return pstmt.getParameterMetaData();
        } catch (SQLException e) {
            return null;
        }
    }
}

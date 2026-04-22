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

/** Entry point for COBOL embedded SQL operations (CONNECT, EXEC SQL, cursors, transactions). */
public class CobolSql {

    private static final Charset SHIFT_JIS = Charset.forName("SHIFT-JIS");
    private static final ConcurrentHashMap<String, PreparedStatement> stmtCache =
            new ConcurrentHashMap<>();

    // -------------------------------------------------------
    // Connection
    // -------------------------------------------------------
    /**
     * Establish a database connection using separate user, password, and dbname parameters.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param user user name storage
     * @param userLen byte length of user
     * @param passwd password storage
     * @param passwdLen byte length of passwd
     * @param dbname database name storage
     * @param dbnameLen byte length of dbname
     */
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

    /**
     * Establish a database connection using a "user/passwd@dbname" format string.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param connInfo connection info storage in "user/passwd@dbname" format
     * @param len byte length of connInfo
     */
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

    /**
     * Establish a database connection using only environment variable defaults.
     *
     * @param sqlca the SQLCA data storage for status reporting
     */
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

    /**
     * Disconnect the default database connection after committing.
     *
     * @param sqlca the SQLCA data storage for status reporting
     */
    public static void disconnect(CobolDataStorage sqlca) {
        try {
            SqlConnection conn = SqlState.getDefaultConnection();
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection");
                return;
            }
            // Commit before disconnect
            try (Statement stmt = conn.getConnection().createStatement()) {
                stmt.execute("COMMIT");
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
    /**
     * Execute a SQL statement with no host variable parameters.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param query the SQL query string
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

            try (Statement stmt = conn.createStatement()) {
                stmt.execute(query);
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
    /**
     * Execute a parameterized SQL statement with COBOL host variable bindings.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param query the SQL query string with '?' placeholders
     * @param params the COBOL host variable parameters
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
    /**
     * Execute a SELECT INTO statement, writing results back to COBOL host variables.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param query the SELECT query string
     * @param inputParams input host variable parameters (WHERE clause bindings)
     * @param resultParams output host variables to receive selected column values
     */
    private static void processSelectIntoResults(
            ResultSet rs, AbstractCobolField[] resultParams, CobolDataStorage sqlca)
            throws SQLException {
        if (rs == null || !rs.next()) {
            SqlCA.setError(sqlca, SqlCA.ECPG_NOT_FOUND, "02000", "No data found");
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

        // Check if this is a GROUP OCCURS pattern (single GROUP result field, multiple rows)
        if (resultParams.length == 1
                && resultParams[0].getAttribute().getType() == CobolFieldAttribute.COB_TYPE_GROUP) {
            AbstractCobolField groupField = resultParams[0];
            int elementSize = groupField.getSize();
            CobolDataStorage baseStorage = groupField.getDataStorage();
            int columnCount = rs.getMetaData().getColumnCount();
            int rowIndex = 0;

            do {
                // Write each column into the correct position within the element
                int colOffset = 0;
                for (int col = 1; col <= columnCount; col++) {
                    byte[] value = CobolDataConverter.getValueFromResultSet(rs, col);
                    int colSize = rs.getMetaData().getColumnDisplaySize(col);
                    CobolDataStorage elementStorage =
                            baseStorage.getSubDataStorage(rowIndex * elementSize + colOffset);
                    if (value != null) {
                        // Pad or truncate to column size
                        if (value.length >= colSize) {
                            elementStorage.memcpy(value, colSize);
                        } else {
                            elementStorage.memset((byte) ' ', colSize);
                            elementStorage.memcpy(value, value.length);
                        }
                    } else {
                        elementStorage.memset((byte) 0, colSize);
                    }
                    colOffset += colSize;
                }
                rowIndex++;
            } while (rs.next());
            rs.close();
            SqlCA.setErrd(sqlca, 2, rowIndex);
            SqlCA.setSuccess(sqlca);
            return;
        }

        // Single row: write columns to individual result fields
        int columnCount = rs.getMetaData().getColumnCount();
        for (int i = 0; i < resultParams.length && i < columnCount; i++) {
            byte[] value = CobolDataConverter.getValueFromResultSet(rs, i + 1);
            if (value != null) {
                CobolDataConverter.stringToCobol(resultParams[i], value);
            } else {
                resultParams[i].getDataStorage().memset((byte) 0, resultParams[i].getSize());
            }
        }
        rs.close();
        SqlCA.setSuccess(sqlca);
    }

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
            processSelectIntoResults(rs, resultParams, sqlca);
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    // -------------------------------------------------------
    // Cursor operations
    // -------------------------------------------------------
    /**
     * Declare a SQL cursor with no parameters.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param cursorName the cursor name
     * @param query the SQL query for the cursor
     */
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

    /**
     * Declare a SQL cursor with host variable parameters.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param cursorName the cursor name
     * @param query the SQL query for the cursor
     * @param params host variable parameters to bind when the cursor is opened
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
     * Open a previously declared cursor.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param cursorName the cursor name to open
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
            cursor.open(sqlConn.getConnection(), null);
            SqlCA.setSuccess(sqlca);
        } catch (SQLException e) {
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    /**
     * Open a previously declared cursor with host variable parameters.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param cursorName the cursor name to open
     * @param params host variable parameters for the cursor query
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
            SqlCA.setResultFromException(sqlca, e);
        }
    }

    /**
     * Fetch the next row from an open cursor into COBOL host variables.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param cursorName the cursor name to fetch from
     * @param resultParams output host variables to receive column values
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

    /**
     * Close an open cursor.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param cursorName the cursor name to close
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
    /**
     * Prepare a SQL statement, replacing COBOL host variable references with '?' placeholders.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param stmtName the name to assign to the prepared statement
     * @param queryField COBOL field containing the SQL query text
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
                byte[] bytes = queryField.getDataStorage().getByteArray(0, queryField.getSize());
                String rawStr = new String(bytes, SHIFT_JIS);
                // VARYING structure: leading numeric length field followed by data
                // Detect by checking if the string starts with digits
                int dataStart = 0;
                while (dataStart < rawStr.length() && Character.isDigit(rawStr.charAt(dataStart))) {
                    dataStart++;
                }
                if (dataStart > 0 && dataStart < rawStr.length()) {
                    // Skip the numeric length prefix
                    query = rawStr.substring(dataStart).trim();
                } else {
                    query = rawStr.trim();
                }
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

    /**
     * Execute a previously prepared statement.
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param stmtName the name of the prepared statement
     * @param params host variable parameters to bind
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
    // Transaction
    // -------------------------------------------------------
    /**
     * Commit the current transaction on the default connection and begin a new one.
     *
     * @param sqlca the SQLCA data storage for status reporting
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
     * Roll back the current transaction on the default connection and begin a new one.
     *
     * @param sqlca the SQLCA data storage for status reporting
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
    // AT database variants (ID prefixed)
    // -------------------------------------------------------
    /**
     * Establish a named database connection (AT db-name variant).
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param atdb connection identifier storage
     * @param atdbLen byte length of atdb
     * @param user user name storage
     * @param userLen byte length of user
     * @param passwd password storage
     * @param passwdLen byte length of passwd
     * @param dbname database name storage
     * @param dbnameLen byte length of dbname
     */
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

    /**
     * Execute a SQL statement on a named connection (AT db-name variant).
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param atdb connection identifier storage
     * @param atdbLen byte length of atdb
     * @param query the SQL query string
     */
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

            try (Statement stmt = conn.createStatement()) {
                stmt.execute(query);
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

    /**
     * Execute a parameterized SQL statement on a named connection (AT db-name variant).
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param atdb connection identifier storage
     * @param atdbLen byte length of atdb
     * @param query the SQL query string with '?' placeholders
     * @param nParams the expected number of parameters
     * @param params the COBOL host variable parameters
     */
    public static void idExecParams(
            CobolDataStorage sqlca,
            CobolDataStorage atdb,
            int atdbLen,
            String query,
            int nParams,
            AbstractCobolField... params) {
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

    /**
     * Disconnect a named connection after committing (AT db-name variant).
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param atdb connection identifier storage
     * @param atdbLen byte length of atdb
     */
    public static void idDisconnect(CobolDataStorage sqlca, CobolDataStorage atdb, int atdbLen) {
        try {
            String atdbStr = storageToString(atdb, atdbLen);
            SqlConnection conn = SqlState.getConnection(atdbStr);
            if (conn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection: " + atdbStr);
                return;
            }
            try (Statement stmt = conn.getConnection().createStatement()) {
                stmt.execute("COMMIT");
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

    /**
     * Commit the current transaction on a named connection (AT db-name variant).
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param atdb connection identifier storage
     * @param atdbLen byte length of atdb
     */
    public static void idCommit(CobolDataStorage sqlca, CobolDataStorage atdb, int atdbLen) {
        try {
            String atdbStr = storageToString(atdb, atdbLen);
            SqlConnection sqlConn = SqlState.getConnection(atdbStr);
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection: " + atdbStr);
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
     * Roll back the current transaction on a named connection (AT db-name variant).
     *
     * @param sqlca the SQLCA data storage for status reporting
     * @param atdb connection identifier storage
     * @param atdbLen byte length of atdb
     */
    public static void idRollback(CobolDataStorage sqlca, CobolDataStorage atdb, int atdbLen) {
        try {
            String atdbStr = storageToString(atdb, atdbLen);
            SqlConnection sqlConn = SqlState.getConnection(atdbStr);
            if (sqlConn == null) {
                SqlCA.setError(sqlca, SqlCA.ECPG_NO_CONN, "08003", "No connection: " + atdbStr);
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

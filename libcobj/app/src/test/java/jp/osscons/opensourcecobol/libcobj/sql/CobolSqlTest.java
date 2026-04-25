package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.io.InputStream;
import java.io.Reader;
import java.lang.reflect.Field;
import java.math.BigDecimal;
import java.net.URL;
import java.nio.ByteBuffer;
import java.sql.*;
import java.util.Calendar;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executor;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

@SuppressWarnings("deprecation")
class CobolSqlTest {

    private CobolDataStorage sqlca;

    private static AbstractCobolField makeAlphaField(int size, byte[] data) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        return CobolFieldFactory.makeCobolField(size, storage, attr);
    }

    private static AbstractCobolField makeNumericField(int size, byte[] data) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, size, 0, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        return CobolFieldFactory.makeCobolField(size, storage, attr);
    }

    @BeforeEach
    void setUp() throws Exception {
        // Allocate SQLCA (133 bytes)
        sqlca = new CobolDataStorage(136);

        // Reset SqlState static maps via reflection
        resetStaticField(SqlState.class, "connections", new java.util.HashMap<>());
        resetStaticField(SqlState.class, "cursors", new java.util.HashMap<>());
        resetStaticField(SqlState.class, "preparedStatements", new java.util.HashMap<>());
        resetStaticField(SqlState.class, "defaultConnId", null);

        // Clear CobolSql stmtCache
        Field cacheField = CobolSql.class.getDeclaredField("stmtCache");
        cacheField.setAccessible(true);
        ((ConcurrentHashMap<?, ?>) cacheField.get(null)).clear();
    }

    @SuppressWarnings("unchecked")
    private void resetStaticField(Class<?> clazz, String fieldName, Object value) throws Exception {
        Field f = clazz.getDeclaredField(fieldName);
        f.setAccessible(true);
        f.set(null, value);
    }

    // Helper to register a mock connection as default
    private MockConnection registerMockConnection() {
        MockConnection mockConn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("OCDB_DEFAULT_DBNAME", mockConn);
        SqlState.addConnection("OCDB_DEFAULT_DBNAME", sqlConn);
        return mockConn;
    }

    private int getSqlCode() {
        return ByteBuffer.wrap(sqlca.getByteArray(12, 4)).getInt();
    }

    private String getSqlState() {
        return new String(sqlca.getByteArray(128, 5));
    }

    // ============================================================
    // exec() tests
    // ============================================================

    @Test
    void testExec_NoConnection() {
        CobolSql.exec(sqlca, "SELECT 1");
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
        assertEquals("08003", getSqlState());
    }

    @Test
    void testExec_NullQuery() {
        registerMockConnection();
        CobolSql.exec(sqlca, null);
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testExec_EmptyQuery() {
        registerMockConnection();
        CobolSql.exec(sqlca, "");
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testExec_Commit() {
        registerMockConnection();
        // Also declare a cursor to test clearCursors
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT 1", 0));
        SqlState.getCursor("c1").isOpened = true;

        CobolSql.exec(sqlca, "COMMIT");
        assertEquals(0, getSqlCode());
        assertEquals("00000", getSqlState());
        // Cursor should be closed
        assertFalse(SqlState.getCursor("c1").isOpened);
    }

    @Test
    void testExec_Rollback() {
        registerMockConnection();
        CobolSql.exec(sqlca, "ROLLBACK");
        assertEquals(0, getSqlCode());
    }

    @Test
    void testExec_Begin() {
        registerMockConnection();
        CobolSql.exec(sqlca, "BEGIN");
        assertEquals(0, getSqlCode());
    }

    @Test
    void testExec_RegularStatement() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.updateCountValue = 3;
        CobolSql.exec(sqlca, "INSERT INTO t VALUES(1)");
        assertEquals(0, getSqlCode());
    }

    @Test
    void testExec_RegularStatement_SqlException() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.throwOnExecute = true;
        conn.mockStatement.throwSqlState = "42P01";
        CobolSql.exec(sqlca, "INSERT INTO t VALUES(1)");
        assertEquals(SqlCA.ECPG_UNKNOWN_ERROR, getSqlCode());
    }

    @Test
    void testExec_RegularStatement_RollbackSavepointFails() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.throwOnExecute = true;
        conn.mockStatement.throwSqlState = "42P01";
        conn.failRollbackSavepoint = true;
        CobolSql.exec(sqlca, "INSERT INTO bad_table VALUES(1)");
        assertEquals(SqlCA.ECPG_UNKNOWN_ERROR, getSqlCode());
    }

    // ============================================================
    // execWithParams() tests
    // ============================================================

    @Test
    void testExecWithParams_NoConnection() {
        CobolSql.execWithParams(
                sqlca, "INSERT INTO t VALUES(?)", makeNumericField(4, "0042".getBytes()));
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testExecWithParams_NullQuery() {
        registerMockConnection();
        CobolSql.execWithParams(sqlca, null, makeNumericField(4, "0042".getBytes()));
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testExecWithParams_EmptyQuery() {
        registerMockConnection();
        CobolSql.execWithParams(sqlca, "", makeNumericField(4, "0042".getBytes()));
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testExecWithParams_Success() {
        MockConnection conn = registerMockConnection();
        conn.mockPreparedStatement.updateCountValue = 1;
        CobolSql.execWithParams(
                sqlca, "INSERT INTO t VALUES(?)", makeNumericField(4, "0042".getBytes()));
        assertEquals(0, getSqlCode());
    }

    @Test
    void testExecWithParams_NullParams() {
        MockConnection conn = registerMockConnection();
        CobolSql.execWithParams(sqlca, "INSERT INTO t VALUES(1)", (AbstractCobolField[]) null);
        assertEquals(0, getSqlCode());
    }

    @Test
    void testExecWithParams_SqlException() {
        MockConnection conn = registerMockConnection();
        conn.mockPreparedStatement.throwOnExecute = true;
        conn.mockPreparedStatement.throwSqlState = "23505";
        CobolSql.execWithParams(
                sqlca, "INSERT INTO t VALUES(?)", makeNumericField(4, "0042".getBytes()));
        assertEquals(SqlCA.ECPG_DUPLICATE_KEY, getSqlCode());
    }

    @Test
    void testExecWithParams_CommitQuery() {
        MockConnection conn = registerMockConnection();
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT 1", 0));
        SqlState.getCursor("c1").isOpened = true;
        CobolSql.execWithParams(sqlca, "COMMIT");
        assertEquals(0, getSqlCode());
        assertFalse(SqlState.getCursor("c1").isOpened);
    }

    @Test
    void testExecWithParams_RollbackQuery() {
        MockConnection conn = registerMockConnection();
        CobolSql.execWithParams(sqlca, "ROLLBACK");
        assertEquals(0, getSqlCode());
    }

    // ============================================================
    // connect() tests
    // ============================================================

    @Test
    void testConnect_NullStorage() {
        // This exercises the storageToString null path + SqlConnection.connect exception
        CobolSql.connect(sqlca, null, 0, null, 0, null, 0);
        // Should get a connection error since DriverManager won't find driver
        assertEquals(SqlCA.ECPG_UNKNOWN_ERROR, getSqlCode());
    }

    @Test
    void testConnectInformal_NullInfo() {
        CobolSql.connectInformal(sqlca, null, 0);
        assertEquals(SqlCA.ECPG_CONNECT, getSqlCode());
    }

    @Test
    void testConnectInformal_EmptyInfo() {
        CobolDataStorage info = new CobolDataStorage(new byte[0]);
        CobolSql.connectInformal(sqlca, info, 0);
        assertEquals(SqlCA.ECPG_CONNECT, getSqlCode());
    }

    @Test
    void testConnectInformal_WithUserPassDb() {
        // Will fail because no real DB, but exercises the parsing logic
        byte[] data = "user/pass@dbname".getBytes();
        CobolDataStorage info = new CobolDataStorage(data);
        CobolSql.connectInformal(sqlca, info, data.length);
        assertEquals(SqlCA.ECPG_CONNECT, getSqlCode()); // connection will fail, but parsing works
    }

    @Test
    void testConnectInformal_UserOnly() {
        byte[] data = "testuser".getBytes();
        CobolDataStorage info = new CobolDataStorage(data);
        CobolSql.connectInformal(sqlca, info, data.length);
        assertEquals(SqlCA.ECPG_UNKNOWN_ERROR, getSqlCode());
    }

    @Test
    void testConnectShort() {
        CobolSql.connectShort(sqlca);
        assertEquals(SqlCA.ECPG_UNKNOWN_ERROR, getSqlCode()); // fails because no real DB
    }

    // ============================================================
    // disconnect() tests
    // ============================================================

    @Test
    void testDisconnect_NoConnection() {
        CobolSql.disconnect(sqlca);
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testDisconnect_Success() {
        MockConnection conn = registerMockConnection();
        CobolSql.disconnect(sqlca);
        assertEquals(0, getSqlCode());
        assertTrue(conn.closeCalled);
    }

    @Test
    void testDisconnect_CommitFails() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.throwOnExecute = true;
        conn.mockStatement.throwSqlState = "42000";
        // Disconnect should still succeed even if commit fails
        CobolSql.disconnect(sqlca);
        assertEquals(0, getSqlCode());
    }

    // ============================================================
    // declareCursor() tests
    // ============================================================

    @Test
    void testDeclareCursor_Success() {
        CobolSql.declareCursor(sqlca, "c1", "SELECT * FROM t");
        assertEquals(0, getSqlCode());
        assertNotNull(SqlState.getCursor("c1"));
    }

    @Test
    void testDeclareCursor_NullName() {
        CobolSql.declareCursor(sqlca, null, "SELECT 1");
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testDeclareCursor_EmptyName() {
        CobolSql.declareCursor(sqlca, "", "SELECT 1");
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testDeclareCursor_NullQuery() {
        CobolSql.declareCursor(sqlca, "c1", null);
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testDeclareCursor_EmptyQuery() {
        CobolSql.declareCursor(sqlca, "c1", "");
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testDeclareCursor_AlreadyOpened() {
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = true;
        SqlState.addCursor("c1", cursor);
        CobolSql.declareCursor(sqlca, "c1", "SELECT 2");
        assertEquals(SqlCA.ECPG_WARNING_PORTAL_EXISTS, getSqlCode());
    }

    @Test
    void testDeclareCursor_ExistingClosed() {
        // Re-declare closed cursor: should succeed
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = false;
        SqlState.addCursor("c1", cursor);
        CobolSql.declareCursor(sqlca, "c1", "SELECT 2");
        assertEquals(0, getSqlCode());
        assertEquals("SELECT 2", SqlState.getCursor("c1").query);
    }

    // ============================================================
    // declareCursorWithParams() tests
    // ============================================================

    @Test
    void testDeclareCursorWithParams_Success() {
        AbstractCobolField param = makeNumericField(4, "0001".getBytes());
        CobolSql.declareCursorWithParams(sqlca, "c1", "SELECT * FROM t WHERE id=?", param);
        assertEquals(0, getSqlCode());
        SqlCursor c = SqlState.getCursor("c1");
        assertNotNull(c);
        assertEquals(1, c.nParams);
        assertNotNull(c.params);
    }

    @Test
    void testDeclareCursorWithParams_NullName() {
        CobolSql.declareCursorWithParams(sqlca, null, "SELECT 1");
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testDeclareCursorWithParams_EmptyQuery() {
        CobolSql.declareCursorWithParams(sqlca, "c1", "");
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testDeclareCursorWithParams_AlreadyOpened() {
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = true;
        SqlState.addCursor("c1", cursor);
        CobolSql.declareCursorWithParams(sqlca, "c1", "SELECT 2");
        assertEquals(SqlCA.ECPG_WARNING_PORTAL_EXISTS, getSqlCode());
    }

    @Test
    void testDeclareCursorWithParams_NullParams() {
        CobolSql.declareCursorWithParams(sqlca, "c1", "SELECT 1", (AbstractCobolField[]) null);
        assertEquals(0, getSqlCode());
        assertEquals(0, SqlState.getCursor("c1").nParams);
    }

    // ============================================================
    // openCursor() tests
    // ============================================================

    @Test
    void testOpenCursor_NoConnection() {
        CobolSql.openCursor(sqlca, "c1");
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testOpenCursor_CursorNotFound() {
        registerMockConnection();
        CobolSql.openCursor(sqlca, "nonexistent");
        assertEquals(SqlCA.ECPG_WARNING_UNKNOWN_PORTAL, getSqlCode());
    }

    @Test
    void testOpenCursor_Success() {
        registerMockConnection();
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT 1", 0));
        CobolSql.openCursor(sqlca, "c1");
        assertEquals(0, getSqlCode());
        assertTrue(SqlState.getCursor("c1").isOpened);
    }

    @Test
    void testOpenCursor_SqlException() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.throwOnExecuteAfterN = 1; // savepoint ok, DECLARE fails
        conn.mockStatement.throwSqlState = "42P01";
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT 1", 0));
        CobolSql.openCursor(sqlca, "c1");
        assertEquals(SqlCA.ECPG_UNKNOWN_ERROR, getSqlCode());
    }

    // ============================================================
    // openCursorWithParams() tests
    // ============================================================

    @Test
    void testOpenCursorWithParams_NoConnection() {
        CobolSql.openCursorWithParams(sqlca, "c1", makeNumericField(4, "0001".getBytes()));
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testOpenCursorWithParams_CursorNotFound() {
        registerMockConnection();
        CobolSql.openCursorWithParams(sqlca, "nonexistent", makeNumericField(4, "0001".getBytes()));
        assertEquals(SqlCA.ECPG_WARNING_UNKNOWN_PORTAL, getSqlCode());
    }

    @Test
    void testOpenCursorWithParams_Success() {
        MockConnection conn = registerMockConnection();
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT * FROM t WHERE id=?", 1));
        CobolSql.openCursorWithParams(sqlca, "c1", makeNumericField(4, "0001".getBytes()));
        assertEquals(0, getSqlCode());
    }

    // ============================================================
    // fetchCursor() tests
    // ============================================================

    @Test
    void testFetchCursor_NoConnection() {
        CobolSql.fetchCursor(sqlca, "c1");
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testFetchCursor_CursorNotFound() {
        registerMockConnection();
        CobolSql.fetchCursor(sqlca, "nonexistent");
        assertEquals(SqlCA.ECPG_WARNING_UNKNOWN_PORTAL, getSqlCode());
    }

    @Test
    void testFetchCursor_CursorNotOpened() {
        registerMockConnection();
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = false;
        SqlState.addCursor("c1", cursor);
        CobolSql.fetchCursor(sqlca, "c1");
        assertEquals(SqlCA.ECPG_WARNING_UNKNOWN_PORTAL, getSqlCode());
    }

    @Test
    void testFetchCursor_NoRows() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.executeReturns = false;
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = true;
        SqlState.addCursor("c1", cursor);
        CobolSql.fetchCursor(sqlca, "c1");
        assertEquals(SqlCA.ECPG_NOT_FOUND, getSqlCode());
    }

    @Test
    void testFetchCursor_WithRows() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.executeReturns = true;
        MockResultSet rs = new MockResultSet(true, 1);
        rs.columnType = Types.VARCHAR;
        rs.stringValue = "Hello";
        conn.mockStatement.mockResultSet = rs;

        byte[] data = new byte[10];
        AbstractCobolField resultField = makeAlphaField(10, data);
        SqlCursor cursor = new SqlCursor("c1", "SELECT name FROM t", 0);
        cursor.isOpened = true;
        SqlState.addCursor("c1", cursor);

        CobolSql.fetchCursor(sqlca, "c1", resultField);
        assertEquals(0, getSqlCode());
    }

    // ============================================================
    // fetchCursorOccurs() tests
    // ============================================================

    @Test
    void testFetchCursorOccurs_NoConnection() {
        CobolSql.fetchCursorOccurs(sqlca, "c1", null, 10, 5);
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testFetchCursorOccurs_CursorNotFound() {
        registerMockConnection();
        CobolSql.fetchCursorOccurs(sqlca, "c1", null, 10, 5);
        assertEquals(SqlCA.ECPG_WARNING_UNKNOWN_PORTAL, getSqlCode());
    }

    @Test
    void testFetchCursorOccurs_CursorNotOpened() {
        registerMockConnection();
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = false;
        SqlState.addCursor("c1", cursor);
        CobolSql.fetchCursorOccurs(sqlca, "c1", null, 10, 5);
        assertEquals(SqlCA.ECPG_WARNING_UNKNOWN_PORTAL, getSqlCode());
    }

    @Test
    void testFetchCursorOccurs_NoRows() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.executeQueryResultSet = new MockResultSet(false, 1);
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = true;
        SqlState.addCursor("c1", cursor);

        byte[] data = new byte[40];
        AbstractCobolField field = makeAlphaField(10, data);
        CobolSql.fetchCursorOccurs(sqlca, "c1", new AbstractCobolField[] {field}, 10, 4);
        assertEquals(0, getSqlCode());
    }

    @Test
    void testFetchCursorOccurs_WithRows() {
        MockConnection conn = registerMockConnection();
        MockResultSet rs = new MockResultSet(true, 1);
        rs.columnType = Types.VARCHAR;
        rs.stringValue = "val";
        conn.mockStatement.executeQueryResultSet = rs;
        SqlCursor cursor = new SqlCursor("c1", "SELECT name FROM t", 0);
        cursor.isOpened = true;
        SqlState.addCursor("c1", cursor);

        byte[] data = new byte[40];
        AbstractCobolField field = makeAlphaField(10, data);
        CobolSql.fetchCursorOccurs(sqlca, "c1", new AbstractCobolField[] {field}, 10, 4);
        assertEquals(0, getSqlCode());
    }

    // ============================================================
    // closeCursor() tests
    // ============================================================

    @Test
    void testCloseCursor_NoConnection() {
        CobolSql.closeCursor(sqlca, "c1");
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testCloseCursor_CursorNotFound() {
        registerMockConnection();
        CobolSql.closeCursor(sqlca, "nonexistent");
        assertEquals(SqlCA.ECPG_WARNING_UNKNOWN_PORTAL, getSqlCode());
    }

    @Test
    void testCloseCursor_CursorNotOpened() {
        registerMockConnection();
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = false;
        SqlState.addCursor("c1", cursor);
        CobolSql.closeCursor(sqlca, "c1");
        assertEquals(SqlCA.ECPG_WARNING_UNKNOWN_PORTAL, getSqlCode());
    }

    @Test
    void testCloseCursor_Success() {
        registerMockConnection();
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = true;
        SqlState.addCursor("c1", cursor);
        CobolSql.closeCursor(sqlca, "c1");
        assertEquals(0, getSqlCode());
        assertFalse(SqlState.getCursor("c1").isOpened);
    }

    // ============================================================
    // selectInto() tests
    // ============================================================

    @Test
    void testSelectInto_NoConnection() {
        CobolSql.selectInto(sqlca, "SELECT 1", null, null);
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testSelectInto_NullQuery() {
        registerMockConnection();
        CobolSql.selectInto(sqlca, null, null, null);
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testSelectInto_EmptyQuery() {
        registerMockConnection();
        CobolSql.selectInto(sqlca, "", null, null);
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testSelectInto_NoInputParams_NoResults() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.executeReturns = true;
        conn.mockStatement.mockResultSet = new MockResultSet(false, 0);
        byte[] data = new byte[10];
        AbstractCobolField resultField = makeAlphaField(10, data);
        CobolSql.selectInto(
                sqlca,
                "SELECT name FROM t WHERE id=1",
                null,
                new AbstractCobolField[] {resultField});
        assertEquals(SqlCA.ECPG_NOT_FOUND, getSqlCode());
    }

    @Test
    void testSelectInto_NoInputParams_WithResults() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.executeReturns = true;
        MockResultSet rs = new MockResultSet(true, 1);
        rs.columnType = Types.VARCHAR;
        rs.stringValue = "Hello";
        conn.mockStatement.mockResultSet = rs;

        byte[] data = new byte[10];
        AbstractCobolField resultField = makeAlphaField(10, data);
        CobolSql.selectInto(
                sqlca,
                "SELECT name FROM t WHERE id=1",
                null,
                new AbstractCobolField[] {resultField});
        assertEquals(0, getSqlCode());
    }

    @Test
    void testSelectInto_WithInputParams() {
        MockConnection conn = registerMockConnection();
        conn.mockPreparedStatement.executeReturns = true;
        MockResultSet rs = new MockResultSet(true, 1);
        rs.columnType = Types.VARCHAR;
        rs.stringValue = "World";
        conn.mockPreparedStatement.mockResultSet = rs;

        byte[] data = new byte[10];
        AbstractCobolField resultField = makeAlphaField(10, data);
        AbstractCobolField inputField = makeNumericField(4, "0001".getBytes());
        CobolSql.selectInto(
                sqlca,
                "SELECT name FROM t WHERE id=?",
                new AbstractCobolField[] {inputField},
                new AbstractCobolField[] {resultField});
        assertEquals(0, getSqlCode());
    }

    @Test
    void testSelectInto_NullResultParams() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.executeReturns = true;
        // processSelectIntoResults with null resultParams and rs.next() returns true
        // The code checks: rs == null || !rs.next() -> false, then resultParams == null -> close +
        // success
        MockResultSet rs = new MockResultSet(true, 1);
        rs.columnType = Types.VARCHAR;
        rs.stringValue = "val";
        conn.mockStatement.mockResultSet = rs;

        CobolSql.selectInto(sqlca, "SELECT name FROM t", null, null);
        // processSelectIntoResults: resultParams is null, rs.next() returned true,
        // so it goes past the first check, then resultParams==null => setSuccess
        assertEquals(0, getSqlCode());
    }

    @Test
    void testSelectInto_EmptyResultParams() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.executeReturns = true;
        MockResultSet rs = new MockResultSet(true, 1);
        rs.columnType = Types.VARCHAR;
        rs.stringValue = "val";
        conn.mockStatement.mockResultSet = rs;

        CobolSql.selectInto(sqlca, "SELECT name FROM t", null, new AbstractCobolField[] {});
        assertEquals(0, getSqlCode());
    }

    @Test
    void testSelectInto_NullValueInResult() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.executeReturns = true;
        MockResultSet rs = new MockResultSet(true, 1);
        rs.columnType = Types.VARCHAR;
        rs.stringValue = null;
        conn.mockStatement.mockResultSet = rs;

        byte[] data = new byte[10];
        java.util.Arrays.fill(data, (byte) 'X');
        AbstractCobolField resultField = makeAlphaField(10, data);
        CobolSql.selectInto(
                sqlca, "SELECT name FROM t", null, new AbstractCobolField[] {resultField});
        assertEquals(0, getSqlCode());
        // data should be zeroed out
        assertEquals(0, data[0]);
    }

    // ============================================================
    // selectIntoOccurs() tests
    // ============================================================

    @Test
    void testSelectIntoOccurs_NoConnection() {
        CobolSql.selectIntoOccurs(sqlca, "SELECT 1", null, null, 10, 5);
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testSelectIntoOccurs_NullQuery() {
        registerMockConnection();
        CobolSql.selectIntoOccurs(sqlca, null, null, null, 10, 5);
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testSelectIntoOccurs_NoResults() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.executeReturns = true;
        conn.mockStatement.mockResultSet = new MockResultSet(false, 0);
        byte[] data = new byte[40];
        AbstractCobolField field = makeAlphaField(10, data);
        CobolSql.selectIntoOccurs(sqlca, "SELECT 1", null, new AbstractCobolField[] {field}, 10, 4);
        assertEquals(SqlCA.ECPG_NOT_FOUND, getSqlCode());
    }

    @Test
    void testSelectIntoOccurs_WithInputParams() {
        MockConnection conn = registerMockConnection();
        conn.mockPreparedStatement.executeReturns = true;
        MockResultSet rs = new MockResultSet(true, 1);
        rs.columnType = Types.VARCHAR;
        rs.stringValue = "val";
        conn.mockPreparedStatement.mockResultSet = rs;

        byte[] data = new byte[40];
        AbstractCobolField field = makeAlphaField(10, data);
        AbstractCobolField input = makeNumericField(4, "0001".getBytes());
        CobolSql.selectIntoOccurs(
                sqlca,
                "SELECT name FROM t WHERE id=?",
                new AbstractCobolField[] {input},
                new AbstractCobolField[] {field},
                10,
                4);
        assertEquals(0, getSqlCode());
    }

    @Test
    void testSelectIntoOccurs_NullValueInResult() {
        MockConnection conn = registerMockConnection();
        conn.mockStatement.executeReturns = true;
        MockResultSet rs = new MockResultSet(true, 1);
        rs.columnType = Types.VARCHAR;
        rs.stringValue = null;
        conn.mockStatement.mockResultSet = rs;

        byte[] data = new byte[40];
        java.util.Arrays.fill(data, (byte) 'X');
        AbstractCobolField field = makeAlphaField(10, data);
        CobolSql.selectIntoOccurs(
                sqlca, "SELECT name FROM t", null, new AbstractCobolField[] {field}, 10, 4);
        assertEquals(0, getSqlCode());
    }

    // ============================================================
    // commit() tests
    // ============================================================

    @Test
    void testCommit_NoConnection() {
        CobolSql.commit(sqlca);
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testCommit_Success() {
        registerMockConnection();
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT 1", 0));
        SqlState.getCursor("c1").isOpened = true;
        CobolSql.commit(sqlca);
        assertEquals(0, getSqlCode());
        assertFalse(SqlState.getCursor("c1").isOpened);
    }

    // ============================================================
    // rollback() tests
    // ============================================================

    @Test
    void testRollback_NoConnection() {
        CobolSql.rollback(sqlca);
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testRollback_Success() {
        registerMockConnection();
        CobolSql.rollback(sqlca);
        assertEquals(0, getSqlCode());
    }

    // ============================================================
    // prepare() tests
    // ============================================================

    @Test
    void testPrepare_NullName() {
        CobolSql.prepare(sqlca, null, null);
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testPrepare_EmptyName() {
        CobolSql.prepare(sqlca, "", null);
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testPrepare_NullQueryField() {
        CobolSql.prepare(sqlca, "stmt1", null);
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testPrepare_EmptyQueryField() {
        byte[] data = new byte[0];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(0, new CobolDataStorage(1), attr);
        CobolSql.prepare(sqlca, "stmt1", field);
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testPrepare_Success_NoParams() {
        byte[] data = "SELECT * FROM t".getBytes();
        AbstractCobolField field = makeAlphaField(data.length, data);
        CobolSql.prepare(sqlca, "stmt1", field);
        assertEquals(0, getSqlCode());
        String[] prepared = SqlState.getPrepared("stmt1");
        assertNotNull(prepared);
        assertEquals("SELECT * FROM t", prepared[0]);
        assertEquals("0", prepared[1]);
    }

    @Test
    void testPrepare_Success_WithHostVars() {
        byte[] data = "SELECT * FROM t WHERE id=:id AND name=:name".getBytes();
        AbstractCobolField field = makeAlphaField(data.length, data);
        CobolSql.prepare(sqlca, "stmt2", field);
        assertEquals(0, getSqlCode());
        String[] prepared = SqlState.getPrepared("stmt2");
        assertNotNull(prepared);
        assertTrue(prepared[0].contains("?"));
        assertEquals("2", prepared[1]);
    }

    @Test
    void testPrepare_WithVaryingPrefix() {
        byte[] data = "123SELECT * FROM t".getBytes();
        AbstractCobolField field = makeAlphaField(data.length, data);
        CobolSql.prepare(sqlca, "stmt3", field);
        assertEquals(0, getSqlCode());
        String[] prepared = SqlState.getPrepared("stmt3");
        assertNotNull(prepared);
        assertEquals("SELECT * FROM t", prepared[0]);
    }

    @Test
    void testPrepare_HostVarWithParenthesis() {
        byte[] data = "INSERT INTO t(id) VALUES(:id)".getBytes();
        AbstractCobolField field = makeAlphaField(data.length, data);
        CobolSql.prepare(sqlca, "stmt4", field);
        assertEquals(0, getSqlCode());
        String[] prepared = SqlState.getPrepared("stmt4");
        assertNotNull(prepared);
        assertTrue(prepared[0].contains("?"));
        assertTrue(prepared[0].contains(")"));
    }

    // ============================================================
    // executePrepared() tests
    // ============================================================

    @Test
    void testExecutePrepared_NotFound() {
        CobolSql.executePrepared(sqlca, "nonexistent");
        assertEquals(SqlCA.ECPG_INVALID_STMT, getSqlCode());
    }

    @Test
    void testExecutePrepared_NoParams() {
        registerMockConnection();
        SqlState.addPrepared("stmt1", "SELECT 1", 0);
        CobolSql.executePrepared(sqlca, "stmt1");
        assertEquals(0, getSqlCode());
    }

    @Test
    void testExecutePrepared_WithParams() {
        MockConnection conn = registerMockConnection();
        conn.mockPreparedStatement.updateCountValue = 1;
        SqlState.addPrepared("stmt1", "INSERT INTO t VALUES(?)", 1);
        CobolSql.executePrepared(sqlca, "stmt1", makeNumericField(4, "0042".getBytes()));
        assertEquals(0, getSqlCode());
    }

    // ============================================================
    // idConnect() tests
    // ============================================================

    @Test
    void testIdConnect() {
        byte[] atdb = "mydb".getBytes();
        byte[] user = "user".getBytes();
        byte[] passwd = "pass".getBytes();
        byte[] dbname = "testdb".getBytes();
        CobolSql.idConnect(
                sqlca,
                new CobolDataStorage(atdb),
                atdb.length,
                new CobolDataStorage(user),
                user.length,
                new CobolDataStorage(passwd),
                passwd.length,
                new CobolDataStorage(dbname),
                dbname.length);
        // Will fail because no real DB, exercises the code path
        assertEquals(SqlCA.ECPG_CONNECT, getSqlCode());
    }

    // ============================================================
    // idExec() tests
    // ============================================================

    @Test
    void testIdExec_NoConnection() {
        byte[] atdb = "mydb".getBytes();
        CobolSql.idExec(sqlca, new CobolDataStorage(atdb), atdb.length, "SELECT 1");
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testIdExec_NullQuery() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        CobolSql.idExec(sqlca, new CobolDataStorage(atdb), atdb.length, null);
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testIdExec_EmptyQuery() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        CobolSql.idExec(sqlca, new CobolDataStorage(atdb), atdb.length, "");
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testIdExec_Success() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        CobolSql.idExec(sqlca, new CobolDataStorage(atdb), atdb.length, "INSERT INTO t VALUES(1)");
        assertEquals(0, getSqlCode());
    }

    @Test
    void testIdExec_Commit() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT 1", 0));
        SqlState.getCursor("c1").isOpened = true;
        CobolSql.idExec(sqlca, new CobolDataStorage(atdb), atdb.length, "COMMIT");
        assertEquals(0, getSqlCode());
        assertFalse(SqlState.getCursor("c1").isOpened);
    }

    @Test
    void testIdExec_Rollback() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        CobolSql.idExec(sqlca, new CobolDataStorage(atdb), atdb.length, "ROLLBACK");
        assertEquals(0, getSqlCode());
    }

    // ============================================================
    // idExecParams() tests
    // ============================================================

    @Test
    void testIdExecParams_NoConnection() {
        byte[] atdb = "mydb".getBytes();
        CobolSql.idExecParams(
                sqlca,
                new CobolDataStorage(atdb),
                atdb.length,
                "INSERT INTO t VALUES(?)",
                1,
                makeNumericField(4, "0042".getBytes()));
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testIdExecParams_NullQuery() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        CobolSql.idExecParams(sqlca, new CobolDataStorage(atdb), atdb.length, null, 0);
        assertEquals(SqlCA.ECPG_EMPTY, getSqlCode());
    }

    @Test
    void testIdExecParams_Success() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        CobolSql.idExecParams(
                sqlca,
                new CobolDataStorage(atdb),
                atdb.length,
                "INSERT INTO t VALUES(?)",
                1,
                makeNumericField(4, "0042".getBytes()));
        assertEquals(0, getSqlCode());
    }

    @Test
    void testIdExecParams_NullParams() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        CobolSql.idExecParams(
                sqlca,
                new CobolDataStorage(atdb),
                atdb.length,
                "INSERT INTO t VALUES(1)",
                0,
                (AbstractCobolField[]) null);
        assertEquals(0, getSqlCode());
    }

    @Test
    void testIdExecParams_Commit() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT 1", 0));
        SqlState.getCursor("c1").isOpened = true;
        CobolSql.idExecParams(sqlca, new CobolDataStorage(atdb), atdb.length, "COMMIT", 0);
        assertEquals(0, getSqlCode());
        assertFalse(SqlState.getCursor("c1").isOpened);
    }

    // ============================================================
    // idDisconnect() tests
    // ============================================================

    @Test
    void testIdDisconnect_NoConnection() {
        byte[] atdb = "mydb".getBytes();
        CobolSql.idDisconnect(sqlca, new CobolDataStorage(atdb), atdb.length);
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testIdDisconnect_Success() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        CobolSql.idDisconnect(sqlca, new CobolDataStorage(atdb), atdb.length);
        assertEquals(0, getSqlCode());
        assertTrue(conn.closeCalled);
    }

    @Test
    void testIdDisconnect_CommitFails() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        conn.mockStatement.throwOnExecute = true;
        conn.mockStatement.throwSqlState = "42000";
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        CobolSql.idDisconnect(sqlca, new CobolDataStorage(atdb), atdb.length);
        // Should succeed even if commit fails
        assertEquals(0, getSqlCode());
    }

    // ============================================================
    // idCommit() tests
    // ============================================================

    @Test
    void testIdCommit_NoConnection() {
        byte[] atdb = "mydb".getBytes();
        CobolSql.idCommit(sqlca, new CobolDataStorage(atdb), atdb.length);
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testIdCommit_Success() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        CobolSql.idCommit(sqlca, new CobolDataStorage(atdb), atdb.length);
        assertEquals(0, getSqlCode());
    }

    // ============================================================
    // idRollback() tests
    // ============================================================

    @Test
    void testIdRollback_NoConnection() {
        byte[] atdb = "mydb".getBytes();
        CobolSql.idRollback(sqlca, new CobolDataStorage(atdb), atdb.length);
        assertEquals(SqlCA.ECPG_NO_CONN, getSqlCode());
    }

    @Test
    void testIdRollback_SuccessProper() {
        byte[] atdb = "mydb".getBytes();
        MockConnection conn = new MockConnection();
        SqlConnection sqlConn = new SqlConnection("mydb", conn);
        SqlState.addConnection("mydb", sqlConn);
        CobolSql.idRollback(sqlca, new CobolDataStorage(atdb), atdb.length);
        assertEquals(0, getSqlCode());
    }

    // ============================================================
    // Mock implementations
    // ============================================================

    static class MockStatement implements Statement {
        String lastExecutedSql;
        boolean executeReturns = false;
        ResultSet mockResultSet = null;
        ResultSet executeQueryResultSet = null;
        boolean throwOnExecute = false;
        String throwSqlState = "42000";
        int updateCountValue = -1;
        int executeCount = 0;
        int throwOnExecuteAfterN = -1;
        boolean failRollbackSavepoint = false;

        @Override
        public boolean execute(String sql) throws SQLException {
            lastExecutedSql = sql;
            executeCount++;
            if (throwOnExecuteAfterN > 0 && executeCount > throwOnExecuteAfterN) {
                throw new SQLException("mock error", throwSqlState);
            }
            if (throwOnExecute) {
                throw new SQLException("mock error", throwSqlState);
            }
            return executeReturns;
        }

        @Override
        public ResultSet executeQuery(String sql) throws SQLException {
            lastExecutedSql = sql;
            if (executeQueryResultSet != null) {
                return executeQueryResultSet;
            }
            if (mockResultSet != null) {
                return mockResultSet;
            }
            return new MockResultSet(false, 0);
        }

        @Override
        public ResultSet getResultSet() {
            return mockResultSet;
        }

        @Override
        public int getUpdateCount() {
            return updateCountValue;
        }

        @Override
        public int executeUpdate(String s) {
            return 0;
        }

        @Override
        public void close() {}

        @Override
        public int getMaxFieldSize() {
            return 0;
        }

        @Override
        public void setMaxFieldSize(int i) {}

        @Override
        public int getMaxRows() {
            return 0;
        }

        @Override
        public void setMaxRows(int i) {}

        @Override
        public void setEscapeProcessing(boolean b) {}

        @Override
        public int getQueryTimeout() {
            return 0;
        }

        @Override
        public void setQueryTimeout(int i) {}

        @Override
        public void cancel() {}

        @Override
        public SQLWarning getWarnings() {
            return null;
        }

        @Override
        public void clearWarnings() {}

        @Override
        public void setCursorName(String s) {}

        @Override
        public boolean getMoreResults() {
            return false;
        }

        @Override
        public void setFetchDirection(int d) {}

        @Override
        public int getFetchDirection() {
            return 0;
        }

        @Override
        public void setFetchSize(int r) {}

        @Override
        public int getFetchSize() {
            return 0;
        }

        @Override
        public int getResultSetConcurrency() {
            return 0;
        }

        @Override
        public int getResultSetType() {
            return 0;
        }

        @Override
        public void addBatch(String s) {}

        @Override
        public void clearBatch() {}

        @Override
        public int[] executeBatch() {
            return new int[0];
        }

        @Override
        public Connection getConnection() {
            return null;
        }

        @Override
        public boolean getMoreResults(int i) {
            return false;
        }

        @Override
        public ResultSet getGeneratedKeys() {
            return null;
        }

        @Override
        public int executeUpdate(String s, int i) {
            return 0;
        }

        @Override
        public int executeUpdate(String s, int[] i) {
            return 0;
        }

        @Override
        public int executeUpdate(String s, String[] n) {
            return 0;
        }

        @Override
        public boolean execute(String s, int i) {
            return false;
        }

        @Override
        public boolean execute(String s, int[] i) {
            return false;
        }

        @Override
        public boolean execute(String s, String[] n) {
            return false;
        }

        @Override
        public int getResultSetHoldability() {
            return 0;
        }

        @Override
        public boolean isClosed() {
            return false;
        }

        @Override
        public void setPoolable(boolean b) {}

        @Override
        public boolean isPoolable() {
            return false;
        }

        @Override
        public void closeOnCompletion() {}

        @Override
        public boolean isCloseOnCompletion() {
            return false;
        }

        @Override
        public <T> T unwrap(Class<T> iface) {
            return null;
        }

        @Override
        public boolean isWrapperFor(Class<?> iface) {
            return false;
        }
    }

    static class MockPreparedStatement extends MockStatement implements PreparedStatement {
        String preparedQuery;
        boolean throwOnExecute = false;
        String throwSqlState = "42000";
        boolean executeReturns = false;
        ResultSet mockResultSet = null;
        int updateCountValue = -1;

        @Override
        public boolean execute() throws SQLException {
            if (throwOnExecute) {
                throw new SQLException("mock prepared error", throwSqlState);
            }
            return executeReturns;
        }

        @Override
        public ResultSet getResultSet() {
            return mockResultSet;
        }

        @Override
        public int getUpdateCount() {
            return updateCountValue;
        }

        @Override
        public ResultSet executeQuery() {
            return mockResultSet;
        }

        @Override
        public int executeUpdate() {
            return 0;
        }

        @Override
        public void setNull(int i, int t) {}

        @Override
        public void setBoolean(int i, boolean v) {}

        @Override
        public void setByte(int i, byte v) {}

        @Override
        public void setShort(int i, short v) {}

        @Override
        public void setInt(int i, int v) {}

        @Override
        public void setLong(int i, long v) {}

        @Override
        public void setFloat(int i, float v) {}

        @Override
        public void setDouble(int i, double v) {}

        @Override
        public void setBigDecimal(int i, BigDecimal v) {}

        @Override
        public void setString(int i, String v) {}

        @Override
        public void setBytes(int i, byte[] v) {}

        @Override
        public void setDate(int i, Date v) {}

        @Override
        public void setTime(int i, Time v) {}

        @Override
        public void setTimestamp(int i, Timestamp v) {}

        @Override
        public void setAsciiStream(int i, InputStream s, int l) {}

        @Override
        @SuppressWarnings("deprecation")
        public void setUnicodeStream(int i, InputStream s, int l) {}

        @Override
        public void setBinaryStream(int i, InputStream s, int l) {}

        @Override
        public void clearParameters() {}

        @Override
        public void setObject(int i, Object v, int t) {}

        @Override
        public void setObject(int i, Object v) {}

        @Override
        public void addBatch() {}

        @Override
        public void setCharacterStream(int i, Reader r, int l) {}

        @Override
        public void setRef(int i, Ref v) {}

        @Override
        public void setBlob(int i, Blob v) {}

        @Override
        public void setClob(int i, Clob v) {}

        @Override
        public void setArray(int i, Array v) {}

        @Override
        public ResultSetMetaData getMetaData() {
            return null;
        }

        @Override
        public void setDate(int i, Date v, Calendar c) {}

        @Override
        public void setTime(int i, Time v, Calendar c) {}

        @Override
        public void setTimestamp(int i, Timestamp v, Calendar c) {}

        @Override
        public void setNull(int i, int t, String n) {}

        @Override
        public void setURL(int i, URL v) {}

        @Override
        public ParameterMetaData getParameterMetaData() {
            return null;
        }

        @Override
        public void setRowId(int i, RowId v) {}

        @Override
        public void setNString(int i, String v) {}

        @Override
        public void setNCharacterStream(int i, Reader v, long l) {}

        @Override
        public void setNClob(int i, NClob v) {}

        @Override
        public void setClob(int i, Reader r, long l) {}

        @Override
        public void setBlob(int i, InputStream s, long l) {}

        @Override
        public void setNClob(int i, Reader r, long l) {}

        @Override
        public void setSQLXML(int i, SQLXML v) {}

        @Override
        public void setObject(int i, Object v, int t, int s) {}

        @Override
        public void setAsciiStream(int i, InputStream s, long l) {}

        @Override
        public void setBinaryStream(int i, InputStream s, long l) {}

        @Override
        public void setCharacterStream(int i, Reader r, long l) {}

        @Override
        public void setAsciiStream(int i, InputStream s) {}

        @Override
        public void setBinaryStream(int i, InputStream s) {}

        @Override
        public void setCharacterStream(int i, Reader r) {}

        @Override
        public void setNCharacterStream(int i, Reader r) {}

        @Override
        public void setClob(int i, Reader r) {}

        @Override
        public void setBlob(int i, InputStream s) {}

        @Override
        public void setNClob(int i, Reader r) {}
    }

    static class MockResultSet implements ResultSet {
        private boolean hasNext;
        private final int columnCount;
        int columnType = Types.VARCHAR;
        String stringValue;
        BigDecimal bigDecimalValue;
        int intValue;
        long longValue;
        short shortValue;
        byte byteValue;
        double doubleValue;
        boolean boolValue;
        boolean wasNullFlag = false;
        int columnDisplaySize = 10;

        MockResultSet(boolean hasNext, int columnCount) {
            this.hasNext = hasNext;
            this.columnCount = columnCount;
        }

        @Override
        public boolean next() {
            if (hasNext) {
                hasNext = false;
                return true;
            }
            return false;
        }

        @Override
        public ResultSetMetaData getMetaData() {
            return new ResultSetMetaData() {
                @Override
                public int getColumnCount() {
                    return columnCount;
                }

                @Override
                public int getColumnType(int col) {
                    return columnType;
                }

                @Override
                public int getColumnDisplaySize(int col) {
                    return columnDisplaySize;
                }

                @Override
                public boolean isAutoIncrement(int col) {
                    return false;
                }

                @Override
                public boolean isCaseSensitive(int col) {
                    return false;
                }

                @Override
                public boolean isSearchable(int col) {
                    return false;
                }

                @Override
                public boolean isCurrency(int col) {
                    return false;
                }

                @Override
                public int isNullable(int col) {
                    return 0;
                }

                @Override
                public boolean isSigned(int col) {
                    return false;
                }

                @Override
                public String getColumnLabel(int col) {
                    return "";
                }

                @Override
                public String getColumnName(int col) {
                    return "";
                }

                @Override
                public String getSchemaName(int col) {
                    return "";
                }

                @Override
                public int getPrecision(int col) {
                    return 0;
                }

                @Override
                public int getScale(int col) {
                    return 0;
                }

                @Override
                public String getTableName(int col) {
                    return "";
                }

                @Override
                public String getCatalogName(int col) {
                    return "";
                }

                @Override
                public String getColumnTypeName(int col) {
                    return "";
                }

                @Override
                public boolean isReadOnly(int col) {
                    return false;
                }

                @Override
                public boolean isWritable(int col) {
                    return false;
                }

                @Override
                public boolean isDefinitelyWritable(int col) {
                    return false;
                }

                @Override
                public String getColumnClassName(int col) {
                    return "";
                }

                @Override
                public <T> T unwrap(Class<T> iface) {
                    return null;
                }

                @Override
                public boolean isWrapperFor(Class<?> iface) {
                    return false;
                }
            };
        }

        @Override
        public void close() {}

        @Override
        public boolean wasNull() {
            return wasNullFlag;
        }

        @Override
        public String getString(int col) {
            return stringValue;
        }

        @Override
        public boolean getBoolean(int col) {
            return boolValue;
        }

        @Override
        public byte getByte(int col) {
            return byteValue;
        }

        @Override
        public short getShort(int col) {
            return shortValue;
        }

        @Override
        public int getInt(int col) {
            return intValue;
        }

        @Override
        public long getLong(int col) {
            return longValue;
        }

        @Override
        public float getFloat(int col) {
            return 0;
        }

        @Override
        public double getDouble(int col) {
            return doubleValue;
        }

        @Override
        @SuppressWarnings("deprecation")
        public BigDecimal getBigDecimal(int col, int s) {
            return null;
        }

        @Override
        public byte[] getBytes(int col) {
            return null;
        }

        @Override
        public Date getDate(int col) {
            return null;
        }

        @Override
        public Time getTime(int col) {
            return null;
        }

        @Override
        public Timestamp getTimestamp(int col) {
            return null;
        }

        @Override
        public InputStream getAsciiStream(int col) {
            return null;
        }

        @Override
        @SuppressWarnings("deprecation")
        public InputStream getUnicodeStream(int col) {
            return null;
        }

        @Override
        public InputStream getBinaryStream(int col) {
            return null;
        }

        @Override
        public String getString(String col) {
            return null;
        }

        @Override
        public boolean getBoolean(String col) {
            return false;
        }

        @Override
        public byte getByte(String col) {
            return 0;
        }

        @Override
        public short getShort(String col) {
            return 0;
        }

        @Override
        public int getInt(String col) {
            return 0;
        }

        @Override
        public long getLong(String col) {
            return 0;
        }

        @Override
        public float getFloat(String col) {
            return 0;
        }

        @Override
        public double getDouble(String col) {
            return 0;
        }

        @Override
        @SuppressWarnings("deprecation")
        public BigDecimal getBigDecimal(String col, int s) {
            return null;
        }

        @Override
        public byte[] getBytes(String col) {
            return null;
        }

        @Override
        public Date getDate(String col) {
            return null;
        }

        @Override
        public Time getTime(String col) {
            return null;
        }

        @Override
        public Timestamp getTimestamp(String col) {
            return null;
        }

        @Override
        public InputStream getAsciiStream(String col) {
            return null;
        }

        @Override
        @SuppressWarnings("deprecation")
        public InputStream getUnicodeStream(String col) {
            return null;
        }

        @Override
        public InputStream getBinaryStream(String col) {
            return null;
        }

        @Override
        public SQLWarning getWarnings() {
            return null;
        }

        @Override
        public void clearWarnings() {}

        @Override
        public String getCursorName() {
            return null;
        }

        @Override
        public Object getObject(int col) {
            return null;
        }

        @Override
        public Object getObject(String col) {
            return null;
        }

        @Override
        public int findColumn(String col) {
            return 0;
        }

        @Override
        public Reader getCharacterStream(int col) {
            return null;
        }

        @Override
        public Reader getCharacterStream(String col) {
            return null;
        }

        @Override
        public BigDecimal getBigDecimal(int col) {
            return bigDecimalValue;
        }

        @Override
        public BigDecimal getBigDecimal(String col) {
            return null;
        }

        @Override
        public boolean isBeforeFirst() {
            return false;
        }

        @Override
        public boolean isAfterLast() {
            return false;
        }

        @Override
        public boolean isFirst() {
            return false;
        }

        @Override
        public boolean isLast() {
            return false;
        }

        @Override
        public void beforeFirst() {}

        @Override
        public void afterLast() {}

        @Override
        public boolean first() {
            return false;
        }

        @Override
        public boolean last() {
            return false;
        }

        @Override
        public int getRow() {
            return 0;
        }

        @Override
        public boolean absolute(int row) {
            return false;
        }

        @Override
        public boolean relative(int rows) {
            return false;
        }

        @Override
        public boolean previous() {
            return false;
        }

        @Override
        public void setFetchDirection(int d) {}

        @Override
        public int getFetchDirection() {
            return 0;
        }

        @Override
        public void setFetchSize(int r) {}

        @Override
        public int getFetchSize() {
            return 0;
        }

        @Override
        public int getType() {
            return 0;
        }

        @Override
        public int getConcurrency() {
            return 0;
        }

        @Override
        public boolean rowUpdated() {
            return false;
        }

        @Override
        public boolean rowInserted() {
            return false;
        }

        @Override
        public boolean rowDeleted() {
            return false;
        }

        @Override
        public void updateNull(int col) {}

        @Override
        public void updateBoolean(int col, boolean v) {}

        @Override
        public void updateByte(int col, byte v) {}

        @Override
        public void updateShort(int col, short v) {}

        @Override
        public void updateInt(int col, int v) {}

        @Override
        public void updateLong(int col, long v) {}

        @Override
        public void updateFloat(int col, float v) {}

        @Override
        public void updateDouble(int col, double v) {}

        @Override
        public void updateBigDecimal(int col, BigDecimal v) {}

        @Override
        public void updateString(int col, String v) {}

        @Override
        public void updateBytes(int col, byte[] v) {}

        @Override
        public void updateDate(int col, Date v) {}

        @Override
        public void updateTime(int col, Time v) {}

        @Override
        public void updateTimestamp(int col, Timestamp v) {}

        @Override
        public void updateAsciiStream(int col, InputStream s, int l) {}

        @Override
        public void updateBinaryStream(int col, InputStream s, int l) {}

        @Override
        public void updateCharacterStream(int col, Reader r, int l) {}

        @Override
        public void updateObject(int col, Object v, int s) {}

        @Override
        public void updateObject(int col, Object v) {}

        @Override
        public void updateNull(String col) {}

        @Override
        public void updateBoolean(String col, boolean v) {}

        @Override
        public void updateByte(String col, byte v) {}

        @Override
        public void updateShort(String col, short v) {}

        @Override
        public void updateInt(String col, int v) {}

        @Override
        public void updateLong(String col, long v) {}

        @Override
        public void updateFloat(String col, float v) {}

        @Override
        public void updateDouble(String col, double v) {}

        @Override
        public void updateBigDecimal(String col, BigDecimal v) {}

        @Override
        public void updateString(String col, String v) {}

        @Override
        public void updateBytes(String col, byte[] v) {}

        @Override
        public void updateDate(String col, Date v) {}

        @Override
        public void updateTime(String col, Time v) {}

        @Override
        public void updateTimestamp(String col, Timestamp v) {}

        @Override
        public void updateAsciiStream(String col, InputStream s, int l) {}

        @Override
        public void updateBinaryStream(String col, InputStream s, int l) {}

        @Override
        public void updateCharacterStream(String col, Reader r, int l) {}

        @Override
        public void updateObject(String col, Object v, int s) {}

        @Override
        public void updateObject(String col, Object v) {}

        @Override
        public void insertRow() {}

        @Override
        public void updateRow() {}

        @Override
        public void deleteRow() {}

        @Override
        public void refreshRow() {}

        @Override
        public void cancelRowUpdates() {}

        @Override
        public void moveToInsertRow() {}

        @Override
        public void moveToCurrentRow() {}

        @Override
        public Statement getStatement() {
            return null;
        }

        @Override
        public Object getObject(int col, Map<String, Class<?>> m) {
            return null;
        }

        @Override
        public Ref getRef(int col) {
            return null;
        }

        @Override
        public Blob getBlob(int col) {
            return null;
        }

        @Override
        public Clob getClob(int col) {
            return null;
        }

        @Override
        public Array getArray(int col) {
            return null;
        }

        @Override
        public Object getObject(String col, Map<String, Class<?>> m) {
            return null;
        }

        @Override
        public Ref getRef(String col) {
            return null;
        }

        @Override
        public Blob getBlob(String col) {
            return null;
        }

        @Override
        public Clob getClob(String col) {
            return null;
        }

        @Override
        public Array getArray(String col) {
            return null;
        }

        @Override
        public Date getDate(int col, Calendar c) {
            return null;
        }

        @Override
        public Date getDate(String col, Calendar c) {
            return null;
        }

        @Override
        public Time getTime(int col, Calendar c) {
            return null;
        }

        @Override
        public Time getTime(String col, Calendar c) {
            return null;
        }

        @Override
        public Timestamp getTimestamp(int col, Calendar c) {
            return null;
        }

        @Override
        public Timestamp getTimestamp(String col, Calendar c) {
            return null;
        }

        @Override
        public URL getURL(int col) {
            return null;
        }

        @Override
        public URL getURL(String col) {
            return null;
        }

        @Override
        public void updateRef(int col, Ref v) {}

        @Override
        public void updateRef(String col, Ref v) {}

        @Override
        public void updateBlob(int col, Blob v) {}

        @Override
        public void updateBlob(String col, Blob v) {}

        @Override
        public void updateClob(int col, Clob v) {}

        @Override
        public void updateClob(String col, Clob v) {}

        @Override
        public void updateArray(int col, Array v) {}

        @Override
        public void updateArray(String col, Array v) {}

        @Override
        public RowId getRowId(int col) {
            return null;
        }

        @Override
        public RowId getRowId(String col) {
            return null;
        }

        @Override
        public void updateRowId(int col, RowId v) {}

        @Override
        public void updateRowId(String col, RowId v) {}

        @Override
        public int getHoldability() {
            return 0;
        }

        @Override
        public boolean isClosed() {
            return false;
        }

        @Override
        public void updateNString(int col, String v) {}

        @Override
        public void updateNString(String col, String v) {}

        @Override
        public void updateNClob(int col, NClob v) {}

        @Override
        public void updateNClob(String col, NClob v) {}

        @Override
        public NClob getNClob(int col) {
            return null;
        }

        @Override
        public NClob getNClob(String col) {
            return null;
        }

        @Override
        public SQLXML getSQLXML(int col) {
            return null;
        }

        @Override
        public SQLXML getSQLXML(String col) {
            return null;
        }

        @Override
        public void updateSQLXML(int col, SQLXML v) {}

        @Override
        public void updateSQLXML(String col, SQLXML v) {}

        @Override
        public String getNString(int col) {
            return null;
        }

        @Override
        public String getNString(String col) {
            return null;
        }

        @Override
        public Reader getNCharacterStream(int col) {
            return null;
        }

        @Override
        public Reader getNCharacterStream(String col) {
            return null;
        }

        @Override
        public void updateNCharacterStream(int col, Reader r, long l) {}

        @Override
        public void updateNCharacterStream(String col, Reader r, long l) {}

        @Override
        public void updateAsciiStream(int col, InputStream s, long l) {}

        @Override
        public void updateBinaryStream(int col, InputStream s, long l) {}

        @Override
        public void updateCharacterStream(int col, Reader r, long l) {}

        @Override
        public void updateAsciiStream(String col, InputStream s, long l) {}

        @Override
        public void updateBinaryStream(String col, InputStream s, long l) {}

        @Override
        public void updateCharacterStream(String col, Reader r, long l) {}

        @Override
        public void updateBlob(int col, InputStream s, long l) {}

        @Override
        public void updateBlob(String col, InputStream s, long l) {}

        @Override
        public void updateClob(int col, Reader r, long l) {}

        @Override
        public void updateClob(String col, Reader r, long l) {}

        @Override
        public void updateNClob(int col, Reader r, long l) {}

        @Override
        public void updateNClob(String col, Reader r, long l) {}

        @Override
        public void updateNCharacterStream(int col, Reader r) {}

        @Override
        public void updateNCharacterStream(String col, Reader r) {}

        @Override
        public void updateAsciiStream(int col, InputStream s) {}

        @Override
        public void updateBinaryStream(int col, InputStream s) {}

        @Override
        public void updateCharacterStream(int col, Reader r) {}

        @Override
        public void updateAsciiStream(String col, InputStream s) {}

        @Override
        public void updateBinaryStream(String col, InputStream s) {}

        @Override
        public void updateCharacterStream(String col, Reader r) {}

        @Override
        public void updateBlob(int col, InputStream s) {}

        @Override
        public void updateBlob(String col, InputStream s) {}

        @Override
        public void updateClob(int col, Reader r) {}

        @Override
        public void updateClob(String col, Reader r) {}

        @Override
        public void updateNClob(int col, Reader r) {}

        @Override
        public void updateNClob(String col, Reader r) {}

        @Override
        public <T> T getObject(int col, Class<T> t) {
            return null;
        }

        @Override
        public <T> T getObject(String col, Class<T> t) {
            return null;
        }

        @Override
        public <T> T unwrap(Class<T> iface) {
            return null;
        }

        @Override
        public boolean isWrapperFor(Class<?> iface) {
            return false;
        }
    }

    static class MockConnection implements Connection {
        MockStatement mockStatement = new MockStatement();
        MockPreparedStatement mockPreparedStatement = new MockPreparedStatement();
        boolean closeCalled = false;
        boolean failRollbackSavepoint = false;

        @Override
        public Statement createStatement() {
            return mockStatement;
        }

        @Override
        public PreparedStatement prepareStatement(String sql) {
            mockPreparedStatement.preparedQuery = sql;
            return mockPreparedStatement;
        }

        @Override
        public CallableStatement prepareCall(String sql) {
            return null;
        }

        @Override
        public String nativeSQL(String sql) {
            return sql;
        }

        @Override
        public void setAutoCommit(boolean b) {}

        @Override
        public boolean getAutoCommit() {
            return false;
        }

        @Override
        public void commit() {}

        @Override
        public void rollback() {}

        @Override
        public void close() {
            closeCalled = true;
        }

        @Override
        public boolean isClosed() {
            return closeCalled;
        }

        @Override
        public DatabaseMetaData getMetaData() {
            return null;
        }

        @Override
        public void setReadOnly(boolean b) {}

        @Override
        public boolean isReadOnly() {
            return false;
        }

        @Override
        public void setCatalog(String s) {}

        @Override
        public String getCatalog() {
            return null;
        }

        @Override
        public void setTransactionIsolation(int i) {}

        @Override
        public int getTransactionIsolation() {
            return 0;
        }

        @Override
        public SQLWarning getWarnings() {
            return null;
        }

        @Override
        public void clearWarnings() {}

        @Override
        public Statement createStatement(int t, int c) {
            return mockStatement;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, int t, int c) {
            return mockPreparedStatement;
        }

        @Override
        public CallableStatement prepareCall(String sql, int t, int c) {
            return null;
        }

        @Override
        public Map<String, Class<?>> getTypeMap() {
            return null;
        }

        @Override
        public void setTypeMap(Map<String, Class<?>> m) {}

        @Override
        public void setHoldability(int h) {}

        @Override
        public int getHoldability() {
            return 0;
        }

        @Override
        public Savepoint setSavepoint() {
            return null;
        }

        @Override
        public Savepoint setSavepoint(String s) {
            return null;
        }

        @Override
        public void rollback(Savepoint s) {}

        @Override
        public void releaseSavepoint(Savepoint s) {}

        @Override
        public Statement createStatement(int t, int c, int h) {
            return mockStatement;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, int t, int c, int h) {
            return mockPreparedStatement;
        }

        @Override
        public CallableStatement prepareCall(String sql, int t, int c, int h) {
            return null;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, int f) {
            return mockPreparedStatement;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, int[] cols) {
            return mockPreparedStatement;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, String[] cols) {
            return mockPreparedStatement;
        }

        @Override
        public Clob createClob() {
            return null;
        }

        @Override
        public Blob createBlob() {
            return null;
        }

        @Override
        public NClob createNClob() {
            return null;
        }

        @Override
        public SQLXML createSQLXML() {
            return null;
        }

        @Override
        public boolean isValid(int timeout) {
            return true;
        }

        @Override
        public void setClientInfo(String k, String v) {}

        @Override
        public void setClientInfo(Properties p) {}

        @Override
        public String getClientInfo(String k) {
            return null;
        }

        @Override
        public Properties getClientInfo() {
            return null;
        }

        @Override
        public Array createArrayOf(String t, Object[] e) {
            return null;
        }

        @Override
        public Struct createStruct(String t, Object[] a) {
            return null;
        }

        @Override
        public void setSchema(String s) {}

        @Override
        public String getSchema() {
            return null;
        }

        @Override
        public void abort(Executor e) {}

        @Override
        public void setNetworkTimeout(Executor e, int ms) {}

        @Override
        public int getNetworkTimeout() {
            return 0;
        }

        @Override
        public <T> T unwrap(Class<T> iface) {
            return null;
        }

        @Override
        public boolean isWrapperFor(Class<?> iface) {
            return false;
        }
    }
}

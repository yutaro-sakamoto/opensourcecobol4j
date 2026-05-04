package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.lang.reflect.Field;
import java.nio.ByteBuffer;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;
import java.util.concurrent.ConcurrentHashMap;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.testcontainers.containers.PostgreSQLContainer;
import org.testcontainers.junit.jupiter.Container;
import org.testcontainers.junit.jupiter.Testcontainers;

@Testcontainers
class CobolSqlTest {

    @Container
    static PostgreSQLContainer<?> postgres =
            new PostgreSQLContainer<>("postgres:15")
                    .withDatabaseName("testdb")
                    .withUsername("test_user")
                    .withPassword("test_pass");

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
    @SuppressWarnings("PMD.AvoidAccessibilityAlteration")
    void setUp() throws Exception {
        sqlca = new CobolDataStorage(136);

        resetStaticField(SqlState.class, "connections", new java.util.HashMap<>());
        resetStaticField(SqlState.class, "cursors", new java.util.HashMap<>());
        resetStaticField(SqlState.class, "preparedStatements", new java.util.HashMap<>());
        resetStaticField(SqlState.class, "defaultConnId", null);

        Field cacheField = CobolSql.class.getDeclaredField("stmtCache");
        cacheField.setAccessible(true);
        ((ConcurrentHashMap<?, ?>) cacheField.get(null)).clear();
    }

    @SuppressWarnings({"unchecked", "PMD.AvoidAccessibilityAlteration"})
    @AfterEach
    void tearDown() throws Exception {
        // Close ALL connections registered in SqlState, not just the default
        try {
            Field connField = SqlState.class.getDeclaredField("connections");
            connField.setAccessible(true);
            java.util.Map<String, SqlConnection> allConns =
                    (java.util.Map<String, SqlConnection>) connField.get(null);
            for (SqlConnection sc : allConns.values()) {
                try {
                    Connection c = sc.getConnection();
                    if (c != null && !c.isClosed()) {
                        c.close();
                    }
                } catch (Exception ignored) {
                }
            }
        } catch (ReflectiveOperationException ignored) {
        }
        resetStaticField(SqlState.class, "connections", new java.util.HashMap<>());
        resetStaticField(SqlState.class, "cursors", new java.util.HashMap<>());
        resetStaticField(SqlState.class, "preparedStatements", new java.util.HashMap<>());
        resetStaticField(SqlState.class, "defaultConnId", null);

        Field cacheField = CobolSql.class.getDeclaredField("stmtCache");
        cacheField.setAccessible(true);
        ((ConcurrentHashMap<?, ?>) cacheField.get(null)).clear();
    }

    @SuppressWarnings({"unchecked", "PMD.AvoidAccessibilityAlteration"})
    private void resetStaticField(Class<?> clazz, String fieldName, Object value) throws Exception {
        Field f = clazz.getDeclaredField(fieldName);
        f.setAccessible(true);
        f.set(null, value);
    }

    private void connectToPostgres() throws Exception {
        String dbSpec =
                "testdb@"
                        + postgres.getHost()
                        + ":"
                        + postgres.getMappedPort(PostgreSQLContainer.POSTGRESQL_PORT);
        byte[] userBytes = postgres.getUsername().getBytes();
        byte[] passBytes = postgres.getPassword().getBytes();
        byte[] dbBytes = dbSpec.getBytes();
        AbstractCobolField userField = makeAlphaField(userBytes.length, userBytes);
        AbstractCobolField passField = makeAlphaField(passBytes.length, passBytes);
        AbstractCobolField dbField = makeAlphaField(dbBytes.length, dbBytes);
        CobolSql.connect(sqlca, userField, passField, dbField);
        assertEquals(0, getSqlCode(), "Connect failed: " + getSqlState());
    }

    private CobolDataStorage makeStorage(String value) {
        byte[] bytes = value.getBytes();
        CobolDataStorage s = new CobolDataStorage(bytes.length);
        s.memcpy(bytes, bytes.length);
        return s;
    }

    private int getSqlCode() {
        return ByteBuffer.wrap(sqlca.getByteArray(12, 4)).getInt();
    }

    private String getSqlState() {
        return new String(sqlca.getByteArray(128, 5));
    }

    // Helper to register a raw JDBC connection (bypassing CobolSql.connect)
    // Mimics what SqlConnection.connect() does: setAutoCommit(true) + BEGIN
    private Connection registerRealConnection() throws Exception {
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        // SqlConnection.connect sets autoCommit(true), then calls beginTransaction() which does
        // BEGIN.
        // In PostgreSQL, with autoCommit=true, explicit BEGIN starts a transaction that spans
        // multiple statements until COMMIT/ROLLBACK.
        realConn.setAutoCommit(true);
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("BEGIN");
        }
        SqlConnection sqlConn = new SqlConnection("OCDB_DEFAULT_DBNAME", realConn);
        SqlState.addConnection("OCDB_DEFAULT_DBNAME", sqlConn);
        return realConn;
    }

    // ============================================================
    // connect() / disconnect()
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testConnect_Success() throws Exception {
        connectToPostgres();
        assertNotNull(SqlState.getDefaultConnection(), "Default connection should not be null");

        // Verify the connection is usable by executing a simple query
        Connection c = SqlState.getDefaultConnection().getConnection();
        try (Statement stmt = c.createStatement();
                java.sql.ResultSet rs = stmt.executeQuery("SELECT 1")) {
            assertTrue(rs.next(), "Query should return a row");
            assertEquals(1, rs.getInt(1), "SELECT 1 should return 1");
        }
    }

    @Test
    void testConnect_NullStorage() {
        CobolSql.connect(sqlca, null, null, null);
        // With null storage and no env vars, should get a connection error
        assertNotEquals(
                0, getSqlCode(), "Connect with null storage should produce non-zero SQLCODE");
    }

    @Test
    void testDisconnect_NoConnection() {
        CobolSql.disconnect(sqlca);
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "Disconnect without connection should return ECPG_NO_CONN");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testDisconnect_Success() throws Exception {
        connectToPostgres();
        CobolSql.disconnect(sqlca);
        assertEquals(0, getSqlCode(), "Disconnect should succeed");
        assertNull(
                SqlState.getDefaultConnection(),
                "Default connection should be null after disconnect");
    }

    // ============================================================
    // exec() with real DB
    // ============================================================

    @Test
    void testExec_NoConnection() {
        CobolSql.exec(sqlca, "SELECT 1");
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "Exec without connection should return ECPG_NO_CONN");
    }

    @Test
    void testExec_NullQuery() throws Exception {
        registerRealConnection();
        CobolSql.exec(sqlca, null);
        assertEquals(
                SqlCA.ECPG_EMPTY, getSqlCode(), "Exec with null query should return ECPG_EMPTY");
    }

    @Test
    void testExec_EmptyQuery() throws Exception {
        registerRealConnection();
        CobolSql.exec(sqlca, "");
        assertEquals(
                SqlCA.ECPG_EMPTY, getSqlCode(), "Exec with empty query should return ECPG_EMPTY");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testExec_CreateTable() throws Exception {
        // Use a fresh connection to avoid stale state from other tests
        Connection freshConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        freshConn.setAutoCommit(true);
        try (Statement stmt = freshConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS exec_test");
        }
        freshConn.close();

        registerRealConnection();
        CobolSql.exec(sqlca, "CREATE TABLE exec_test (id INTEGER, name VARCHAR(20))");
        assertEquals(0, getSqlCode(), "CREATE TABLE failed with state: " + getSqlState());
        // Clean up
        CobolSql.exec(sqlca, "DROP TABLE exec_test");
        assertEquals(0, getSqlCode(), "DROP TABLE should succeed");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testExec_InsertAndCommit() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS exec_test");
            stmt.execute("CREATE TABLE exec_test (id INTEGER)");
        }

        CobolSql.exec(sqlca, "INSERT INTO exec_test VALUES (42)");
        assertEquals(0, getSqlCode(), "INSERT should succeed");

        CobolSql.exec(sqlca, "COMMIT");
        assertEquals(0, getSqlCode(), "COMMIT should succeed");

        // Clean up
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE exec_test");
        }
    }

    @Test
    void testExec_Rollback() throws Exception {
        registerRealConnection();
        CobolSql.exec(sqlca, "ROLLBACK");
        assertEquals(0, getSqlCode(), "ROLLBACK should succeed");
    }

    @Test
    void testExec_Begin() throws Exception {
        registerRealConnection();
        CobolSql.exec(sqlca, "BEGIN");
        assertEquals(0, getSqlCode(), "BEGIN should succeed");
    }

    @Test
    void testExec_InvalidTable() throws Exception {
        registerRealConnection();
        CobolSql.exec(sqlca, "INSERT INTO nonexistent_table VALUES (1)");
        assertNotEquals(0, getSqlCode(), "Insert into nonexistent table should fail");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testExec_CommitClearsCursors() throws Exception {
        registerRealConnection();
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT 1", 0));
        SqlState.getCursor("c1").isOpened = true;
        CobolSql.exec(sqlca, "COMMIT");
        assertEquals(0, getSqlCode(), "COMMIT should succeed");
        assertFalse(SqlState.getCursor("c1").isOpened, "Cursor should be closed after COMMIT");
    }

    // ============================================================
    // execWithParams() with real DB
    // ============================================================

    @Test
    void testExecWithParams_NoConnection() {
        CobolSql.execWithParams(
                sqlca, "INSERT INTO t VALUES(?)", makeNumericField(4, "0042".getBytes()));
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "ExecWithParams without connection should return ECPG_NO_CONN");
    }

    @Test
    void testExecWithParams_NullQuery() throws Exception {
        registerRealConnection();
        CobolSql.execWithParams(sqlca, null, makeNumericField(4, "0042".getBytes()));
        assertEquals(
                SqlCA.ECPG_EMPTY,
                getSqlCode(),
                "ExecWithParams with null query should return ECPG_EMPTY");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testExecWithParams_Success() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_exec");
            stmt.execute("CREATE TABLE param_exec (id INTEGER, name VARCHAR(20))");
        }

        CobolSql.execWithParams(
                sqlca,
                "INSERT INTO param_exec VALUES (?, ?)",
                makeNumericField(4, "0042".getBytes()),
                makeAlphaField(5, "Hello".getBytes()));
        assertEquals(0, getSqlCode(), "ExecWithParams INSERT should succeed");

        // Verify
        try (Statement stmt = realConn.createStatement();
                java.sql.ResultSet rs = stmt.executeQuery("SELECT id, name FROM param_exec")) {
            assertTrue(rs.next(), "Should have a row");
            assertEquals(42, rs.getInt(1), "Inserted id should be 42");
            assertEquals("Hello", rs.getString(2), "Inserted name should be Hello");
        }

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE param_exec");
        }
    }

    @Test
    void testExecWithParams_NullParams() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_exec2");
            stmt.execute("CREATE TABLE param_exec2 (id INTEGER)");
        }
        CobolSql.execWithParams(
                sqlca, "INSERT INTO param_exec2 VALUES (1)", (AbstractCobolField[]) null);
        assertEquals(0, getSqlCode(), "ExecWithParams with null params should succeed");
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE param_exec2");
        }
    }

    @Test
    void testExecWithParams_ConstraintViolation() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS uniq_test");
            stmt.execute("CREATE TABLE uniq_test (id INTEGER UNIQUE)");
            stmt.execute("INSERT INTO uniq_test VALUES (1)");
        }

        CobolSql.execWithParams(
                sqlca, "INSERT INTO uniq_test VALUES (?)", makeNumericField(4, "0001".getBytes()));
        assertEquals(
                SqlCA.ECPG_DUPLICATE_KEY,
                getSqlCode(),
                "Duplicate key should return ECPG_DUPLICATE_KEY");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE uniq_test");
        }
    }

    // ============================================================
    // selectInto() with real DB
    // ============================================================

    @Test
    void testSelectInto_NoConnection() {
        CobolSql.selectInto(sqlca, "SELECT 1", null, null);
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "SelectInto without connection should return ECPG_NO_CONN");
    }

    @Test
    void testSelectInto_NullQuery() throws Exception {
        registerRealConnection();
        CobolSql.selectInto(sqlca, null, null, null);
        assertEquals(
                SqlCA.ECPG_EMPTY,
                getSqlCode(),
                "SelectInto with null query should return ECPG_EMPTY");
    }

    @Test
    void testSelectInto_NoResults() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS sel_test");
            stmt.execute("CREATE TABLE sel_test (id INTEGER, name VARCHAR(20))");
        }

        byte[] data = new byte[20];
        AbstractCobolField resultField = makeAlphaField(20, data);
        CobolSql.selectInto(
                sqlca,
                "SELECT name FROM sel_test WHERE id = 999",
                null,
                new AbstractCobolField[] {resultField});
        assertEquals(
                SqlCA.ECPG_NOT_FOUND,
                getSqlCode(),
                "SelectInto with no results should return ECPG_NOT_FOUND");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE sel_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSelectInto_WithResults() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS sel_test");
            stmt.execute("CREATE TABLE sel_test (id INTEGER, name VARCHAR(20))");
            stmt.execute("INSERT INTO sel_test VALUES (1, 'World')");
        }

        byte[] data = new byte[20];
        AbstractCobolField resultField = makeAlphaField(20, data);
        CobolSql.selectInto(
                sqlca,
                "SELECT name FROM sel_test WHERE id = 1",
                null,
                new AbstractCobolField[] {resultField});
        assertEquals(0, getSqlCode(), "SelectInto should succeed");
        String result = new String(resultField.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("World", result, "Selected value should be World");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE sel_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSelectInto_WithInputParams() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS sel_test");
            stmt.execute("CREATE TABLE sel_test (id INTEGER, name VARCHAR(20))");
            stmt.execute("INSERT INTO sel_test VALUES (1, 'World')");
        }

        byte[] data = new byte[20];
        AbstractCobolField resultField = makeAlphaField(20, data);
        AbstractCobolField inputField = makeNumericField(4, "0001".getBytes());
        CobolSql.selectInto(
                sqlca,
                "SELECT name FROM sel_test WHERE id = ?",
                new AbstractCobolField[] {inputField},
                new AbstractCobolField[] {resultField});
        assertEquals(0, getSqlCode(), "SelectInto with params should succeed");
        String result = new String(resultField.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("World", result, "Selected value should be World");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE sel_test");
        }
    }

    @Test
    void testSelectInto_NullResultParams() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS sel_test");
            stmt.execute("CREATE TABLE sel_test (id INTEGER)");
            stmt.execute("INSERT INTO sel_test VALUES (1)");
        }

        CobolSql.selectInto(sqlca, "SELECT id FROM sel_test", null, null);
        assertEquals(0, getSqlCode(), "SelectInto with null result params should succeed");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE sel_test");
        }
    }

    @Test
    void testSelectInto_EmptyResultParams() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS sel_test");
            stmt.execute("CREATE TABLE sel_test (id INTEGER)");
            stmt.execute("INSERT INTO sel_test VALUES (1)");
        }

        CobolSql.selectInto(sqlca, "SELECT id FROM sel_test", null, new AbstractCobolField[] {});
        assertEquals(0, getSqlCode(), "SelectInto with empty result params should succeed");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE sel_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSelectInto_NullValueInResult() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS sel_test");
            stmt.execute("CREATE TABLE sel_test (name VARCHAR(20))");
            stmt.execute("INSERT INTO sel_test VALUES (NULL)");
        }

        byte[] data = new byte[10];
        java.util.Arrays.fill(data, (byte) 'X');
        AbstractCobolField resultField = makeAlphaField(10, data);
        CobolSql.selectInto(
                sqlca, "SELECT name FROM sel_test", null, new AbstractCobolField[] {resultField});
        assertEquals(0, getSqlCode(), "SelectInto with NULL value should succeed");
        assertEquals(
                0, resultField.getDataStorage().getByte(0), "NULL value should zero out storage");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE sel_test");
        }
    }

    // ============================================================
    // cursor lifecycle with real DB
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testDeclareCursor_Success() {
        CobolSql.declareCursor(sqlca, "c1", "SELECT * FROM t");
        assertEquals(0, getSqlCode(), "Declare cursor should succeed");
        assertNotNull(SqlState.getCursor("c1"), "Cursor should be registered");
    }

    @Test
    void testDeclareCursor_NullName() {
        CobolSql.declareCursor(sqlca, null, "SELECT 1");
        assertEquals(
                SqlCA.ECPG_EMPTY, getSqlCode(), "Declare with null name should return ECPG_EMPTY");
    }

    @Test
    void testDeclareCursor_NullQuery() {
        CobolSql.declareCursor(sqlca, "c1", null);
        assertEquals(
                SqlCA.ECPG_EMPTY, getSqlCode(), "Declare with null query should return ECPG_EMPTY");
    }

    @Test
    void testDeclareCursor_AlreadyOpened() {
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = true;
        SqlState.addCursor("c1", cursor);
        CobolSql.declareCursor(sqlca, "c1", "SELECT 2");
        assertEquals(
                SqlCA.ECPG_WARNING_PORTAL_EXISTS,
                getSqlCode(),
                "Declaring already-opened cursor should return ECPG_WARNING_PORTAL_EXISTS");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testDeclareCursor_ExistingClosed() {
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = false;
        SqlState.addCursor("c1", cursor);
        CobolSql.declareCursor(sqlca, "c1", "SELECT 2");
        assertEquals(0, getSqlCode(), "Re-declaring closed cursor should succeed");
        assertEquals("SELECT 2", SqlState.getCursor("c1").query, "Cursor query should be updated");
    }

    @Test
    void testOpenCursor_NoConnection() {
        CobolSql.openCursor(sqlca, "c1");
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "Open cursor without connection should return ECPG_NO_CONN");
    }

    @Test
    void testOpenCursor_CursorNotFound() throws Exception {
        registerRealConnection();
        CobolSql.openCursor(sqlca, "nonexistent");
        assertEquals(
                SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                getSqlCode(),
                "Open nonexistent cursor should return ECPG_WARNING_UNKNOWN_PORTAL");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testCursorLifecycle_RealDB() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS cur_test");
            stmt.execute("CREATE TABLE cur_test (id INTEGER, name VARCHAR(20))");
            stmt.execute("INSERT INTO cur_test VALUES (1, 'Alice')");
            stmt.execute("INSERT INTO cur_test VALUES (2, 'Bob')");
        }

        // Declare
        CobolSql.declareCursor(sqlca, "myc", "SELECT name FROM cur_test ORDER BY id");
        assertEquals(0, getSqlCode(), "Declare cursor should succeed");

        // Open
        CobolSql.openCursor(sqlca, "myc");
        assertEquals(0, getSqlCode(), "Open cursor should succeed");
        assertTrue(SqlState.getCursor("myc").isOpened, "Cursor should be opened");

        // Fetch first row
        byte[] data1 = new byte[20];
        AbstractCobolField field1 = makeAlphaField(20, data1);
        CobolSql.fetchCursor(sqlca, "myc", field1);
        assertEquals(0, getSqlCode(), "Fetch first row should succeed");
        String result1 = new String(field1.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("Alice", result1, "First row should be Alice");

        // Fetch second row
        byte[] data2 = new byte[20];
        AbstractCobolField field2 = makeAlphaField(20, data2);
        CobolSql.fetchCursor(sqlca, "myc", field2);
        assertEquals(0, getSqlCode(), "Fetch second row should succeed");
        String result2 = new String(field2.getDataStorage().getByteArray(0, 3)).trim();
        assertEquals("Bob", result2, "Second row should be Bob");

        // Fetch past end
        CobolSql.fetchCursor(sqlca, "myc");
        assertEquals(
                SqlCA.ECPG_NOT_FOUND, getSqlCode(), "Fetch past end should return ECPG_NOT_FOUND");

        // Close
        CobolSql.closeCursor(sqlca, "myc");
        assertEquals(0, getSqlCode(), "Close cursor should succeed");
        assertFalse(SqlState.getCursor("myc").isOpened, "Cursor should be closed");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE cur_test");
        }
    }

    @Test
    void testFetchCursor_NoConnection() {
        CobolSql.fetchCursor(sqlca, "c1");
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "Fetch without connection should return ECPG_NO_CONN");
    }

    @Test
    void testFetchCursor_CursorNotFound() throws Exception {
        registerRealConnection();
        CobolSql.fetchCursor(sqlca, "nonexistent");
        assertEquals(
                SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                getSqlCode(),
                "Fetch nonexistent cursor should return ECPG_WARNING_UNKNOWN_PORTAL");
    }

    @Test
    void testFetchCursor_CursorNotOpened() throws Exception {
        registerRealConnection();
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = false;
        SqlState.addCursor("c1", cursor);
        CobolSql.fetchCursor(sqlca, "c1");
        assertEquals(
                SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                getSqlCode(),
                "Fetch closed cursor should return ECPG_WARNING_UNKNOWN_PORTAL");
    }

    @Test
    void testCloseCursor_NoConnection() {
        CobolSql.closeCursor(sqlca, "c1");
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "Close cursor without connection should return ECPG_NO_CONN");
    }

    @Test
    void testCloseCursor_CursorNotFound() throws Exception {
        registerRealConnection();
        CobolSql.closeCursor(sqlca, "nonexistent");
        assertEquals(
                SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                getSqlCode(),
                "Close nonexistent cursor should return ECPG_WARNING_UNKNOWN_PORTAL");
    }

    @Test
    void testCloseCursor_CursorNotOpened() throws Exception {
        registerRealConnection();
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = false;
        SqlState.addCursor("c1", cursor);
        CobolSql.closeCursor(sqlca, "c1");
        assertEquals(
                SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                getSqlCode(),
                "Close not-opened cursor should return ECPG_WARNING_UNKNOWN_PORTAL");
    }

    // ============================================================
    // declareCursorWithParams
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testDeclareCursorWithParams_Success() {
        AbstractCobolField param = makeNumericField(4, "0001".getBytes());
        CobolSql.declareCursorWithParams(sqlca, "c1", "SELECT * FROM t WHERE id=?", param);
        assertEquals(0, getSqlCode(), "DeclareCursorWithParams should succeed");
        SqlCursor c = SqlState.getCursor("c1");
        assertNotNull(c, "Cursor should be registered");
        assertEquals(1, c.nParams, "Cursor should have 1 param");
    }

    @Test
    void testDeclareCursorWithParams_NullName() {
        CobolSql.declareCursorWithParams(sqlca, null, "SELECT 1");
        assertEquals(
                SqlCA.ECPG_EMPTY,
                getSqlCode(),
                "DeclareCursorWithParams with null name should return ECPG_EMPTY");
    }

    @Test
    void testDeclareCursorWithParams_EmptyQuery() {
        CobolSql.declareCursorWithParams(sqlca, "c1", "");
        assertEquals(
                SqlCA.ECPG_EMPTY,
                getSqlCode(),
                "DeclareCursorWithParams with empty query should return ECPG_EMPTY");
    }

    @Test
    void testDeclareCursorWithParams_AlreadyOpened() {
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = true;
        SqlState.addCursor("c1", cursor);
        CobolSql.declareCursorWithParams(sqlca, "c1", "SELECT 2");
        assertEquals(
                SqlCA.ECPG_WARNING_PORTAL_EXISTS,
                getSqlCode(),
                "Declaring already-opened cursor should return ECPG_WARNING_PORTAL_EXISTS");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testDeclareCursorWithParams_NullParams() {
        CobolSql.declareCursorWithParams(sqlca, "c1", "SELECT 1", (AbstractCobolField[]) null);
        assertEquals(0, getSqlCode(), "DeclareCursorWithParams with null params should succeed");
        assertEquals(0, SqlState.getCursor("c1").nParams, "Cursor should have 0 params");
    }

    // ============================================================
    // openCursorWithParams
    // ============================================================

    @Test
    void testOpenCursorWithParams_NoConnection() {
        CobolSql.openCursorWithParams(sqlca, "c1", makeNumericField(4, "0001".getBytes()));
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "OpenCursorWithParams without connection should return ECPG_NO_CONN");
    }

    @Test
    void testOpenCursorWithParams_CursorNotFound() throws Exception {
        registerRealConnection();
        CobolSql.openCursorWithParams(sqlca, "nonexistent", makeNumericField(4, "0001".getBytes()));
        assertEquals(
                SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                getSqlCode(),
                "OpenCursorWithParams for nonexistent cursor should return"
                        + " ECPG_WARNING_UNKNOWN_PORTAL");
    }

    // ============================================================
    // fetchCursorOccurs
    // ============================================================

    @Test
    void testFetchCursorOccurs_NoConnection() {
        CobolSql.fetchCursorOccurs(sqlca, "c1", 10, 5);
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "FetchCursorOccurs without connection should return ECPG_NO_CONN");
    }

    @Test
    void testFetchCursorOccurs_CursorNotFound() throws Exception {
        registerRealConnection();
        CobolSql.fetchCursorOccurs(sqlca, "c1", 10, 5);
        assertEquals(
                SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                getSqlCode(),
                "FetchCursorOccurs for nonexistent cursor should return"
                        + " ECPG_WARNING_UNKNOWN_PORTAL");
    }

    @Test
    void testFetchCursorOccurs_CursorNotOpened() throws Exception {
        registerRealConnection();
        SqlCursor cursor = new SqlCursor("c1", "SELECT 1", 0);
        cursor.isOpened = false;
        SqlState.addCursor("c1", cursor);
        CobolSql.fetchCursorOccurs(sqlca, "c1", 10, 5);
        assertEquals(
                SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                getSqlCode(),
                "FetchCursorOccurs for closed cursor should return ECPG_WARNING_UNKNOWN_PORTAL");
    }

    // ============================================================
    // selectIntoOccurs
    // ============================================================

    @Test
    void testSelectIntoOccurs_NoConnection() {
        CobolSql.selectIntoOccurs(sqlca, "SELECT 1", null, null, 10, 5);
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "SelectIntoOccurs without connection should return ECPG_NO_CONN");
    }

    @Test
    void testSelectIntoOccurs_NullQuery() throws Exception {
        registerRealConnection();
        CobolSql.selectIntoOccurs(sqlca, null, null, null, 10, 5);
        assertEquals(
                SqlCA.ECPG_EMPTY,
                getSqlCode(),
                "SelectIntoOccurs with null query should return ECPG_EMPTY");
    }

    // ============================================================
    // commit() / rollback()
    // ============================================================

    @Test
    void testCommit_NoConnection() {
        CobolSql.commit(sqlca);
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "Commit without connection should return ECPG_NO_CONN");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testCommit_Success() throws Exception {
        registerRealConnection();
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT 1", 0));
        SqlState.getCursor("c1").isOpened = true;
        CobolSql.commit(sqlca);
        assertEquals(0, getSqlCode(), "Commit should succeed");
        assertFalse(SqlState.getCursor("c1").isOpened, "Cursor should be closed after commit");
    }

    @Test
    void testRollback_NoConnection() {
        CobolSql.rollback(sqlca);
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "Rollback without connection should return ECPG_NO_CONN");
    }

    @Test
    void testRollback_Success() throws Exception {
        registerRealConnection();
        CobolSql.rollback(sqlca);
        assertEquals(0, getSqlCode(), "Rollback should succeed");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testRollback_UndoesInsert() throws Exception {
        // Create the table OUTSIDE any transaction so it persists after rollback
        Connection setupConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        setupConn.setAutoCommit(true);
        try (Statement stmt = setupConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rollback_test");
            stmt.execute("CREATE TABLE rollback_test (id INTEGER)");
        }
        setupConn.close();

        Connection realConn = registerRealConnection();

        CobolSql.exec(sqlca, "INSERT INTO rollback_test VALUES (1)");
        assertEquals(0, getSqlCode(), "INSERT should succeed");

        CobolSql.exec(sqlca, "ROLLBACK");
        assertEquals(0, getSqlCode(), "ROLLBACK should succeed");

        // Verify the insert was rolled back
        try (Statement stmt = realConn.createStatement();
                java.sql.ResultSet rs = stmt.executeQuery("SELECT COUNT(*) FROM rollback_test")) {
            assertTrue(rs.next(), "COUNT query should return a row");
            assertEquals(0, rs.getInt(1), "Rolled-back table should have 0 rows");
        }

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE rollback_test");
        }
    }

    // ============================================================
    // prepare() / executePrepared()
    // ============================================================

    @Test
    void testPrepare_NullName() {
        CobolSql.prepare(sqlca, null, null);
        assertEquals(
                SqlCA.ECPG_EMPTY, getSqlCode(), "Prepare with null name should return ECPG_EMPTY");
    }

    @Test
    void testPrepare_EmptyName() {
        CobolSql.prepare(sqlca, "", null);
        assertEquals(
                SqlCA.ECPG_EMPTY, getSqlCode(), "Prepare with empty name should return ECPG_EMPTY");
    }

    @Test
    void testPrepare_NullQueryField() {
        CobolSql.prepare(sqlca, "stmt1", null);
        assertEquals(
                SqlCA.ECPG_EMPTY,
                getSqlCode(),
                "Prepare with null query field should return ECPG_EMPTY");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testPrepare_Success_NoParams() {
        byte[] data = "SELECT * FROM t".getBytes();
        AbstractCobolField field = makeAlphaField(data.length, data);
        CobolSql.prepare(sqlca, "stmt1", field);
        assertEquals(0, getSqlCode(), "Prepare should succeed");
        String[] prepared = SqlState.getPrepared("stmt1");
        assertNotNull(prepared, "Prepared statement should be registered");
        assertEquals("SELECT * FROM t", prepared[0], "Prepared query should match");
        assertEquals("0", prepared[1], "Prepared param count should be 0");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testPrepare_Success_WithHostVars() {
        byte[] data = "SELECT * FROM t WHERE id=:id AND name=:name".getBytes();
        AbstractCobolField field = makeAlphaField(data.length, data);
        CobolSql.prepare(sqlca, "stmt2", field);
        assertEquals(0, getSqlCode(), "Prepare with host vars should succeed");
        String[] prepared = SqlState.getPrepared("stmt2");
        assertNotNull(prepared, "Prepared statement should be registered");
        assertTrue(prepared[0].contains("?"), "Host vars should be replaced with ?");
        assertEquals("2", prepared[1], "Prepared param count should be 2");
    }

    @Test
    void testExecutePrepared_NotFound() {
        CobolSql.executePrepared(sqlca, "nonexistent");
        assertEquals(
                SqlCA.ECPG_INVALID_STMT,
                getSqlCode(),
                "Execute nonexistent prepared should return ECPG_INVALID_STMT");
    }

    @Test
    void testExecutePrepared_NoParams() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS prep_test");
            stmt.execute("CREATE TABLE prep_test (id INTEGER)");
            stmt.execute("INSERT INTO prep_test VALUES (1)");
        }

        SqlState.addPrepared("stmt1", "SELECT 1", 0);
        CobolSql.executePrepared(sqlca, "stmt1");
        assertEquals(0, getSqlCode(), "ExecutePrepared should succeed");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE prep_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testExecutePrepared_WithParams() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS prep_test");
            stmt.execute("CREATE TABLE prep_test (id INTEGER)");
        }

        SqlState.addPrepared("stmt1", "INSERT INTO prep_test VALUES (?)", 1);
        CobolSql.executePrepared(sqlca, "stmt1", makeNumericField(4, "0042".getBytes()));
        assertEquals(0, getSqlCode(), "ExecutePrepared with params should succeed");

        try (Statement stmt = realConn.createStatement();
                java.sql.ResultSet rs = stmt.executeQuery("SELECT id FROM prep_test")) {
            assertTrue(rs.next(), "Should have a row");
            assertEquals(42, rs.getInt(1), "Prepared insert value should be 42");
        }

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE prep_test");
        }
    }

    // ============================================================
    // connectInformal / connectShort
    // ============================================================

    @Test
    void testConnectInformal_NullInfo() {
        CobolSql.connectInformal(sqlca, null, 0);
        assertEquals(
                SqlCA.ECPG_CONNECT,
                getSqlCode(),
                "ConnectInformal with null should return ECPG_CONNECT");
    }

    @Test
    void testConnectInformal_EmptyInfo() {
        CobolDataStorage info = new CobolDataStorage(new byte[0]);
        CobolSql.connectInformal(sqlca, info, 0);
        assertEquals(
                SqlCA.ECPG_CONNECT,
                getSqlCode(),
                "ConnectInformal with empty info should return ECPG_CONNECT");
    }

    @Test
    void testConnectShort() {
        CobolSql.connectShort(sqlca);
        // Will fail because no env vars set - exercises code path
        assertNotEquals(0, getSqlCode(), "ConnectShort without env vars should fail");
    }

    // ============================================================
    // idConnect / idExec / idDisconnect / idCommit / idRollback
    // ============================================================

    @Test
    void testIdExec_NoConnection() {
        byte[] atdb = "mydb".getBytes();
        CobolSql.idExec(sqlca, new CobolDataStorage(atdb), atdb.length, "SELECT 1");
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "IdExec without connection should return ECPG_NO_CONN");
    }

    @Test
    void testIdExec_NullQuery() throws Exception {
        byte[] atdb = "mydb".getBytes();
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.setAutoCommit(true);
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("BEGIN");
        }
        SqlState.addConnection("mydb", new SqlConnection("mydb", realConn));
        CobolSql.idExec(sqlca, new CobolDataStorage(atdb), atdb.length, null);
        assertEquals(
                SqlCA.ECPG_EMPTY, getSqlCode(), "IdExec with null query should return ECPG_EMPTY");
        realConn.close();
    }

    @Test
    void testIdExec_Success() throws Exception {
        byte[] atdb = "mydb".getBytes();
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.setAutoCommit(true);
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("BEGIN");
        }
        SqlState.addConnection("mydb", new SqlConnection("mydb", realConn));
        CobolSql.idExec(sqlca, new CobolDataStorage(atdb), atdb.length, "SELECT 1");
        assertEquals(0, getSqlCode(), "IdExec should succeed");
        realConn.close();
    }

    @Test
    void testIdDisconnect_NoConnection() {
        byte[] atdb = "mydb".getBytes();
        CobolSql.idDisconnect(sqlca, new CobolDataStorage(atdb), atdb.length);
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "IdDisconnect without connection should return ECPG_NO_CONN");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testIdDisconnect_Success() throws Exception {
        byte[] atdb = "mydb".getBytes();
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.setAutoCommit(true);
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("BEGIN");
        }
        SqlState.addConnection("mydb", new SqlConnection("mydb", realConn));
        CobolSql.idDisconnect(sqlca, new CobolDataStorage(atdb), atdb.length);
        assertEquals(0, getSqlCode(), "IdDisconnect should succeed");
        assertTrue(realConn.isClosed(), "Connection should be closed after idDisconnect");
    }

    @Test
    void testIdCommit_NoConnection() {
        byte[] atdb = "mydb".getBytes();
        CobolSql.idCommit(sqlca, new CobolDataStorage(atdb), atdb.length);
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "IdCommit without connection should return ECPG_NO_CONN");
    }

    @Test
    void testIdCommit_Success() throws Exception {
        byte[] atdb = "mydb".getBytes();
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.setAutoCommit(true);
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("BEGIN");
        }
        SqlState.addConnection("mydb", new SqlConnection("mydb", realConn));
        CobolSql.idCommit(sqlca, new CobolDataStorage(atdb), atdb.length);
        assertEquals(0, getSqlCode(), "IdCommit should succeed");
        realConn.close();
    }

    @Test
    void testIdRollback_NoConnection() {
        byte[] atdb = "mydb".getBytes();
        CobolSql.idRollback(sqlca, new CobolDataStorage(atdb), atdb.length);
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "IdRollback without connection should return ECPG_NO_CONN");
    }

    @Test
    void testIdRollback_Success() throws Exception {
        byte[] atdb = "mydb".getBytes();
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.setAutoCommit(true);
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("BEGIN");
        }
        SqlState.addConnection("mydb", new SqlConnection("mydb", realConn));
        CobolSql.idRollback(sqlca, new CobolDataStorage(atdb), atdb.length);
        assertEquals(0, getSqlCode(), "IdRollback should succeed");
        realConn.close();
    }

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
        assertEquals(
                SqlCA.ECPG_NO_CONN,
                getSqlCode(),
                "IdExecParams without connection should return ECPG_NO_CONN");
    }

    @Test
    void testIdExecParams_NullQuery() throws Exception {
        byte[] atdb = "mydb".getBytes();
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.setAutoCommit(true);
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("BEGIN");
        }
        SqlState.addConnection("mydb", new SqlConnection("mydb", realConn));
        CobolSql.idExecParams(sqlca, new CobolDataStorage(atdb), atdb.length, null, 0);
        assertEquals(
                SqlCA.ECPG_EMPTY,
                getSqlCode(),
                "IdExecParams with null query should return ECPG_EMPTY");
        realConn.close();
    }

    @Test
    void testIdExecParams_Success() throws Exception {
        byte[] atdb = "mydb".getBytes();
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.setAutoCommit(true);
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("BEGIN");
            stmt.execute("DROP TABLE IF EXISTS id_param_test");
            stmt.execute("CREATE TABLE id_param_test (id INTEGER)");
        }
        SqlState.addConnection("mydb", new SqlConnection("mydb", realConn));

        CobolSql.idExecParams(
                sqlca,
                new CobolDataStorage(atdb),
                atdb.length,
                "INSERT INTO id_param_test VALUES (?)",
                1,
                makeNumericField(4, "0042".getBytes()));
        assertEquals(0, getSqlCode(), "IdExecParams should succeed");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE id_param_test");
        }
        realConn.close();
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testIdExec_Commit() throws Exception {
        byte[] atdb = "mydb".getBytes();
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.setAutoCommit(true);
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("BEGIN");
        }
        SqlState.addConnection("mydb", new SqlConnection("mydb", realConn));
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT 1", 0));
        SqlState.getCursor("c1").isOpened = true;
        CobolSql.idExec(sqlca, new CobolDataStorage(atdb), atdb.length, "COMMIT");
        assertEquals(0, getSqlCode(), "IdExec COMMIT should succeed");
        assertFalse(SqlState.getCursor("c1").isOpened, "Cursor should be closed after COMMIT");
        realConn.close();
    }

    @Test
    void testIdExec_Rollback() throws Exception {
        byte[] atdb = "mydb".getBytes();
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.setAutoCommit(true);
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("BEGIN");
        }
        SqlState.addConnection("mydb", new SqlConnection("mydb", realConn));
        CobolSql.idExec(sqlca, new CobolDataStorage(atdb), atdb.length, "ROLLBACK");
        assertEquals(0, getSqlCode(), "IdExec ROLLBACK should succeed");
        realConn.close();
    }

    // ============================================================
    // Error handling
    // ============================================================

    @Test
    void testExec_TableNotExists() throws Exception {
        registerRealConnection();
        CobolSql.exec(sqlca, "SELECT * FROM table_that_does_not_exist");
        assertNotEquals(0, getSqlCode(), "Query on nonexistent table should fail");
    }

    @Test
    void testExecWithParams_TableNotExists() throws Exception {
        registerRealConnection();
        CobolSql.execWithParams(
                sqlca,
                "INSERT INTO table_that_does_not_exist VALUES (?)",
                makeNumericField(4, "0001".getBytes()));
        assertNotEquals(0, getSqlCode(), "Insert into nonexistent table should fail");
    }

    // ============================================================
    // connectInformal with real DB
    // ============================================================

    @Test
    void testConnectInformal_WithUserPassDb() {
        // NOTE: This test only verifies the error path because connectInformal parses
        // "user/passwd@dbname" format, and the @ character in the container's JDBC URL
        // conflicts with the @ delimiter used to separate user/pass from dbname.
        String info = postgres.getUsername() + "/" + postgres.getPassword();
        byte[] data = info.getBytes();
        CobolDataStorage infoStorage = new CobolDataStorage(data);
        CobolSql.connectInformal(sqlca, infoStorage, data.length);
        // Will fail because no dbname => uses env var or defaults => won't find the container DB
        assertNotEquals(0, getSqlCode(), "ConnectInformal without proper dbname should fail");
    }

    @Test
    void testConnectInformal_UserOnly() {
        // NOTE: This test only verifies the error path because without @dbname,
        // connectInformal falls through to connect with just a username
        byte[] data = "testuser".getBytes();
        CobolDataStorage info = new CobolDataStorage(data);
        CobolSql.connectInformal(sqlca, info, data.length);
        assertNotEquals(0, getSqlCode(), "ConnectInformal with user only should fail");
    }

    // ============================================================
    // selectIntoOccurs with real DB
    // ============================================================

    @Test
    void testSelectIntoOccurs_WithResults() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS occ_test");
            stmt.execute("CREATE TABLE occ_test (val VARCHAR(10))");
            stmt.execute("INSERT INTO occ_test VALUES ('AAA')");
            stmt.execute("INSERT INTO occ_test VALUES ('BBB')");
            stmt.execute("INSERT INTO occ_test VALUES ('CCC')");
        }

        // Create an array large enough for 3 rows of 10 bytes each
        byte[] data = new byte[30];
        AbstractCobolField field = makeAlphaField(10, data);
        CobolSql.selectIntoOccurs(
                sqlca,
                "SELECT val FROM occ_test ORDER BY val",
                null,
                new AbstractCobolField[] {field},
                10,
                3);
        assertEquals(0, getSqlCode(), "SelectIntoOccurs should succeed");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE occ_test");
        }
    }

    @Test
    void testSelectIntoOccurs_NoResults() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS occ_test");
            stmt.execute("CREATE TABLE occ_test (val VARCHAR(10))");
        }

        byte[] data = new byte[30];
        AbstractCobolField field = makeAlphaField(10, data);
        CobolSql.selectIntoOccurs(
                sqlca, "SELECT val FROM occ_test", null, new AbstractCobolField[] {field}, 10, 3);
        assertEquals(
                SqlCA.ECPG_NOT_FOUND,
                getSqlCode(),
                "SelectIntoOccurs with no results should return ECPG_NOT_FOUND");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE occ_test");
        }
    }

    @Test
    void testSelectIntoOccurs_WithInputParams() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS occ_test");
            stmt.execute("CREATE TABLE occ_test (id INTEGER, val VARCHAR(10))");
            stmt.execute("INSERT INTO occ_test VALUES (1, 'AAA')");
            stmt.execute("INSERT INTO occ_test VALUES (1, 'BBB')");
        }

        byte[] data = new byte[20];
        AbstractCobolField field = makeAlphaField(10, data);
        AbstractCobolField input = makeNumericField(4, "0001".getBytes());
        CobolSql.selectIntoOccurs(
                sqlca,
                "SELECT val FROM occ_test WHERE id = ?",
                new AbstractCobolField[] {input},
                new AbstractCobolField[] {field},
                10,
                2);
        assertEquals(0, getSqlCode(), "SelectIntoOccurs with input params should succeed");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE occ_test");
        }
    }

    // ============================================================
    // fetchCursorOccurs with real DB
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testFetchCursorOccurs_WithData() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS focc_test");
            stmt.execute("CREATE TABLE focc_test (val VARCHAR(10))");
            stmt.execute("INSERT INTO focc_test VALUES ('XX')");
            stmt.execute("INSERT INTO focc_test VALUES ('YY')");
        }

        CobolSql.declareCursor(sqlca, "focc", "SELECT val FROM focc_test ORDER BY val");
        assertEquals(0, getSqlCode(), "Declare cursor should succeed");

        CobolSql.openCursor(sqlca, "focc");
        assertEquals(0, getSqlCode(), "Open cursor should succeed");

        byte[] data = new byte[20];
        AbstractCobolField field = makeAlphaField(10, data);
        CobolSql.fetchCursorOccurs(sqlca, "focc", 10, 2, field);
        assertEquals(0, getSqlCode(), "FetchCursorOccurs should succeed");

        CobolSql.closeCursor(sqlca, "focc");
        assertEquals(0, getSqlCode(), "Close cursor should succeed");

        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE focc_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testFetchCursorOccurs_EmptyResult() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS focc_test2");
            stmt.execute("CREATE TABLE focc_test2 (val VARCHAR(10))");
        }

        CobolSql.declareCursor(sqlca, "focc2", "SELECT val FROM focc_test2");
        assertEquals(0, getSqlCode(), "Declare cursor should succeed");
        CobolSql.openCursor(sqlca, "focc2");
        assertEquals(0, getSqlCode(), "Open cursor should succeed");

        byte[] data = new byte[20];
        AbstractCobolField field = makeAlphaField(10, data);
        CobolSql.fetchCursorOccurs(sqlca, "focc2", 10, 2, field);
        assertEquals(0, getSqlCode(), "FetchCursorOccurs on empty should succeed");

        CobolSql.closeCursor(sqlca, "focc2");
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE focc_test2");
        }
    }

    // ============================================================
    // openCursorWithParams with real DB
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testOpenCursorWithParams_Success() throws Exception {
        Connection realConn = registerRealConnection();
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS ocp_test");
            stmt.execute("CREATE TABLE ocp_test (id INTEGER, val VARCHAR(10))");
            stmt.execute("INSERT INTO ocp_test VALUES (1, 'Hello')");
        }

        CobolSql.declareCursorWithParams(
                sqlca,
                "ocp",
                "SELECT val FROM ocp_test WHERE id = ?",
                makeNumericField(4, "0001".getBytes()));
        assertEquals(0, getSqlCode(), "DeclareCursorWithParams should succeed");

        CobolSql.openCursorWithParams(sqlca, "ocp", makeNumericField(4, "0001".getBytes()));
        assertEquals(0, getSqlCode(), "OpenCursorWithParams should succeed");

        byte[] data = new byte[10];
        AbstractCobolField field = makeAlphaField(10, data);
        CobolSql.fetchCursor(sqlca, "ocp", field);
        assertEquals(0, getSqlCode(), "FetchCursor should succeed");
        String val = new String(field.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("Hello", val, "Fetched value should be Hello");

        CobolSql.closeCursor(sqlca, "ocp");
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE ocp_test");
        }
    }

    // ============================================================
    // prepare with VARYING prefix
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testPrepare_WithVaryingPrefix() {
        byte[] data = "123SELECT * FROM t".getBytes();
        AbstractCobolField field = makeAlphaField(data.length, data);
        CobolSql.prepare(sqlca, "stmt3", field);
        assertEquals(0, getSqlCode(), "Prepare with varying prefix should succeed");
        String[] prepared = SqlState.getPrepared("stmt3");
        assertNotNull(prepared, "Prepared statement should be registered");
        assertEquals("SELECT * FROM t", prepared[0], "Prepared query should strip varying prefix");
    }

    @Test
    void testPrepare_EmptyQueryField() {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(0, new CobolDataStorage(1), attr);
        CobolSql.prepare(sqlca, "stmt1", field);
        assertEquals(
                SqlCA.ECPG_EMPTY,
                getSqlCode(),
                "Prepare with empty query field should return ECPG_EMPTY");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testPrepare_HostVarWithParenthesis() {
        byte[] data = "INSERT INTO t(id) VALUES(:id)".getBytes();
        AbstractCobolField field = makeAlphaField(data.length, data);
        CobolSql.prepare(sqlca, "stmt4", field);
        assertEquals(0, getSqlCode(), "Prepare with host var in parens should succeed");
        String[] prepared = SqlState.getPrepared("stmt4");
        assertNotNull(prepared, "Prepared statement should be registered");
        assertTrue(prepared[0].contains("?"), "Host var should be replaced with ?");
        assertTrue(prepared[0].contains(")"), "Parenthesis should be preserved");
    }

    // ============================================================
    // idExecParams with commit/rollback
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testIdExecParams_Commit() throws Exception {
        byte[] atdb = "mydb".getBytes();
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.setAutoCommit(true);
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("BEGIN");
        }
        SqlState.addConnection("mydb", new SqlConnection("mydb", realConn));
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT 1", 0));
        SqlState.getCursor("c1").isOpened = true;
        CobolSql.idExecParams(sqlca, new CobolDataStorage(atdb), atdb.length, "COMMIT", 0);
        assertEquals(0, getSqlCode(), "IdExecParams COMMIT should succeed");
        assertFalse(SqlState.getCursor("c1").isOpened, "Cursor should be closed after COMMIT");
        realConn.close();
    }

    @Test
    void testIdExecParams_NullParams() throws Exception {
        byte[] atdb = "mydb".getBytes();
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.setAutoCommit(true);
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("BEGIN");
            stmt.execute("DROP TABLE IF EXISTS id_null_test");
            stmt.execute("CREATE TABLE id_null_test (id INTEGER)");
        }
        SqlState.addConnection("mydb", new SqlConnection("mydb", realConn));
        CobolSql.idExecParams(
                sqlca,
                new CobolDataStorage(atdb),
                atdb.length,
                "INSERT INTO id_null_test VALUES (1)",
                0,
                (AbstractCobolField[]) null);
        assertEquals(0, getSqlCode(), "IdExecParams with null params should succeed");
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("DROP TABLE id_null_test");
        }
        realConn.close();
    }

    @Test
    void testIdConnect() {
        String dbSpec =
                "testdb@"
                        + postgres.getHost()
                        + ":"
                        + postgres.getMappedPort(PostgreSQLContainer.POSTGRESQL_PORT);
        byte[] atdb = "myconn".getBytes();
        byte[] user = postgres.getUsername().getBytes();
        byte[] passwd = postgres.getPassword().getBytes();
        byte[] dbname = dbSpec.getBytes();
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
        assertEquals(0, getSqlCode(), "idConnect failed: " + getSqlState());
    }

    // ============================================================
    // ExecWithParams commit/rollback paths
    // ============================================================

    @Test
    void testExecWithParams_CommitQuery() throws Exception {
        registerRealConnection();
        SqlState.addCursor("c1", new SqlCursor("c1", "SELECT 1", 0));
        SqlState.getCursor("c1").isOpened = true;
        CobolSql.execWithParams(sqlca, "COMMIT");
        // Accept both success (0) and "no transaction" warning (-604)
        assertTrue(
                getSqlCode() == 0 || getSqlCode() == SqlCA.ECPG_WARNING_NO_TRANSACTION,
                "Unexpected code: " + getSqlCode());
    }

    @Test
    void testExecWithParams_RollbackQuery() throws Exception {
        registerRealConnection();
        CobolSql.execWithParams(sqlca, "ROLLBACK");
        // Accept both success (0) and "no transaction" warning (-604)
        assertTrue(
                getSqlCode() == 0 || getSqlCode() == SqlCA.ECPG_WARNING_NO_TRANSACTION,
                "Unexpected code: " + getSqlCode());
    }

    @Test
    void testExecWithParams_EmptyQuery() throws Exception {
        registerRealConnection();
        CobolSql.execWithParams(sqlca, "", makeNumericField(4, "0042".getBytes()));
        assertEquals(
                SqlCA.ECPG_EMPTY,
                getSqlCode(),
                "ExecWithParams with empty query should return ECPG_EMPTY");
    }

    // ============================================================
    // Full integration: connect -> create -> insert -> select -> disconnect
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testFullLifecycle() throws Exception {
        connectToPostgres();

        // CREATE TABLE
        CobolSql.exec(sqlca, "CREATE TABLE lifecycle_test (id INTEGER, name VARCHAR(20))");
        assertEquals(0, getSqlCode(), "CREATE TABLE should succeed");

        // INSERT with params
        CobolSql.execWithParams(
                sqlca,
                "INSERT INTO lifecycle_test VALUES (?, ?)",
                makeNumericField(4, "0001".getBytes()),
                makeAlphaField(5, "Alice".getBytes()));
        assertEquals(0, getSqlCode(), "INSERT should succeed");

        // SELECT INTO
        byte[] nameData = new byte[20];
        AbstractCobolField nameField = makeAlphaField(20, nameData);
        CobolSql.selectInto(
                sqlca,
                "SELECT name FROM lifecycle_test WHERE id = 1",
                null,
                new AbstractCobolField[] {nameField});
        assertEquals(0, getSqlCode(), "SELECT INTO should succeed");
        String fetchedName = new String(nameField.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("Alice", fetchedName, "Fetched name should be Alice");

        // COMMIT
        CobolSql.commit(sqlca);
        assertEquals(0, getSqlCode(), "COMMIT should succeed");

        // Clean up
        CobolSql.exec(sqlca, "DROP TABLE lifecycle_test");
        assertEquals(0, getSqlCode(), "DROP TABLE should succeed");

        // Disconnect
        CobolSql.disconnect(sqlca);
        assertEquals(0, getSqlCode(), "DISCONNECT should succeed");
    }
}

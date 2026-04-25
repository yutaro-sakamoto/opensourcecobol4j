package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;
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
class SqlCursorTest {

    @Container
    static PostgreSQLContainer<?> postgres =
            new PostgreSQLContainer<>("postgres:15")
                    .withDatabaseName("testdb")
                    .withUsername("test_user")
                    .withPassword("test_pass");

    private Connection conn;

    @BeforeEach
    void setUp() throws Exception {
        conn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        conn.setAutoCommit(true);
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS cursor_test");
            stmt.execute("CREATE TABLE cursor_test (id INTEGER, name VARCHAR(20))");
            stmt.execute("INSERT INTO cursor_test VALUES (1, 'Alice')");
            stmt.execute("INSERT INTO cursor_test VALUES (2, 'Bob')");
            stmt.execute("INSERT INTO cursor_test VALUES (3, 'Carol')");
        }
        // Start a transaction (cursors require a transaction in PostgreSQL)
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("BEGIN");
        }
    }

    @AfterEach
    void tearDown() throws Exception {
        if (conn != null && !conn.isClosed()) {
            try (Statement stmt = conn.createStatement()) {
                stmt.execute("ROLLBACK");
            } catch (Exception ignored) {
            }
            try (Statement stmt = conn.createStatement()) {
                stmt.execute("DROP TABLE IF EXISTS cursor_test");
            } catch (Exception ignored) {
            }
            conn.close();
        }
    }

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

    // ---------- Constructor ----------

    @Test
    void testConstructor() {
        SqlCursor cursor = new SqlCursor("c1", "SELECT * FROM t", 2);
        assertEquals("c1", cursor.name);
        assertEquals("SELECT * FROM t", cursor.query);
        assertEquals(2, cursor.nParams);
        assertFalse(cursor.isOpened);
        assertNull(cursor.params);
    }

    // ---------- open/fetch/close lifecycle ----------

    @Test
    void testOpenFetchClose_NoParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT name FROM cursor_test ORDER BY id", 0);

        // Open
        cursor.open(conn, null);
        assertTrue(cursor.isOpened);

        // Fetch first row
        byte[] data1 = new byte[20];
        AbstractCobolField field1 = makeAlphaField(20, data1);
        boolean hasRow = cursor.fetch(conn, new AbstractCobolField[] {field1});
        assertTrue(hasRow);
        String fetched1 = new String(field1.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("Alice", fetched1);

        // Fetch second row
        byte[] data2 = new byte[20];
        AbstractCobolField field2 = makeAlphaField(20, data2);
        hasRow = cursor.fetch(conn, new AbstractCobolField[] {field2});
        assertTrue(hasRow);
        String fetched2 = new String(field2.getDataStorage().getByteArray(0, 3)).trim();
        assertEquals("Bob", fetched2);

        // Fetch third row
        byte[] data3 = new byte[20];
        AbstractCobolField field3 = makeAlphaField(20, data3);
        hasRow = cursor.fetch(conn, new AbstractCobolField[] {field3});
        assertTrue(hasRow);
        String fetched3 = new String(field3.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("Carol", fetched3);

        // Fetch past end
        hasRow = cursor.fetch(conn, null);
        assertFalse(hasRow);

        // Close
        cursor.close(conn);
        assertFalse(cursor.isOpened);
    }

    @Test
    void testOpen_WithParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur2", "SELECT name FROM cursor_test WHERE id = ?", 1);

        AbstractCobolField param = makeNumericField(4, "0002".getBytes());
        cursor.open(conn, new AbstractCobolField[] {param});
        assertTrue(cursor.isOpened);

        byte[] data = new byte[20];
        AbstractCobolField field = makeAlphaField(20, data);
        boolean hasRow = cursor.fetch(conn, new AbstractCobolField[] {field});
        assertTrue(hasRow);
        String fetched = new String(field.getDataStorage().getByteArray(0, 3)).trim();
        assertEquals("Bob", fetched);

        hasRow = cursor.fetch(conn, null);
        assertFalse(hasRow);

        cursor.close(conn);
        assertFalse(cursor.isOpened);
    }

    @Test
    void testOpen_WithStoredParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur3", "SELECT name FROM cursor_test WHERE id = ?", 1);
        cursor.params = new AbstractCobolField[] {makeNumericField(4, "0001".getBytes())};

        cursor.open(conn, null);
        assertTrue(cursor.isOpened);

        byte[] data = new byte[20];
        AbstractCobolField field = makeAlphaField(20, data);
        boolean hasRow = cursor.fetch(conn, new AbstractCobolField[] {field});
        assertTrue(hasRow);
        String fetched = new String(field.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("Alice", fetched);

        cursor.close(conn);
    }

    @Test
    void testFetch_WithNullResultParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur4", "SELECT name FROM cursor_test ORDER BY id", 0);
        cursor.open(conn, null);

        // Fetch with null resultParams should return true but not write anything
        boolean hasRow = cursor.fetch(conn, null);
        assertTrue(hasRow);

        cursor.close(conn);
    }

    @Test
    void testFetch_NullValue() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("INSERT INTO cursor_test VALUES (4, NULL)");
        }

        SqlCursor cursor = new SqlCursor("cur5", "SELECT name FROM cursor_test WHERE id = 4", 0);
        cursor.open(conn, null);

        byte[] data = new byte[20];
        java.util.Arrays.fill(data, (byte) 'X');
        AbstractCobolField field = makeAlphaField(20, data);
        boolean hasRow = cursor.fetch(conn, new AbstractCobolField[] {field});
        assertTrue(hasRow);
        // NULL value should zero out the storage
        assertEquals(0, field.getDataStorage().getByte(0));

        cursor.close(conn);
    }

    @Test
    void testFetch_EmptyResultSet() throws Exception {
        SqlCursor cursor = new SqlCursor("cur6", "SELECT name FROM cursor_test WHERE id = 999", 0);
        cursor.open(conn, null);

        boolean hasRow = cursor.fetch(conn, null);
        assertFalse(hasRow);

        cursor.close(conn);
    }

    @Test
    void testOpen_EmptyParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur7", "SELECT 1", 0);
        cursor.open(conn, new AbstractCobolField[0]);
        assertTrue(cursor.isOpened);
        cursor.close(conn);
    }

    @Test
    void testClose_SetsIsOpenedFalse() throws Exception {
        SqlCursor cursor = new SqlCursor("cur8", "SELECT 1", 0);
        cursor.open(conn, null);
        assertTrue(cursor.isOpened);
        cursor.close(conn);
        assertFalse(cursor.isOpened);
    }

    // ---------- Multiple columns ----------

    @Test
    void testFetch_MultipleColumns() throws Exception {
        SqlCursor cursor = new SqlCursor("cur9", "SELECT id, name FROM cursor_test ORDER BY id", 0);
        cursor.open(conn, null);

        byte[] idData = new byte[10];
        byte[] nameData = new byte[20];
        AbstractCobolField idField = makeAlphaField(10, idData);
        AbstractCobolField nameField = makeAlphaField(20, nameData);

        boolean hasRow = cursor.fetch(conn, new AbstractCobolField[] {idField, nameField});
        assertTrue(hasRow);

        String fetchedId = new String(idField.getDataStorage().getByteArray(0, 1)).trim();
        assertEquals("1", fetchedId);

        String fetchedName = new String(nameField.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("Alice", fetchedName);

        cursor.close(conn);
    }
}

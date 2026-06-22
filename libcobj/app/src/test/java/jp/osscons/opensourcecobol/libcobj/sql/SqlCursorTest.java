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
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testConstructor() {
        SqlCursor cursor = new SqlCursor("c1", "SELECT * FROM t", 2);
        assertEquals("c1", cursor.name, "Cursor name should be c1");
        assertEquals("SELECT * FROM t", cursor.query, "Cursor query should match");
        assertEquals(2, cursor.nParams, "Cursor nParams should be 2");
        assertFalse(cursor.isOpened, "New cursor should not be opened");
        assertNull(cursor.params, "New cursor params should be null");
    }

    // ---------- open/fetch/close lifecycle ----------

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testOpenFetchClose_NoParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT name FROM cursor_test ORDER BY id", 0);

        // Open
        cursor.open(conn, null);
        assertTrue(cursor.isOpened, "Cursor should be opened after open");

        // Fetch first row
        byte[] data1 = new byte[20];
        AbstractCobolField field1 = makeAlphaField(20, data1);
        boolean hasRow = cursor.fetch(conn, new AbstractCobolField[] {field1}, null);
        assertTrue(hasRow, "First fetch should return a row");
        String fetched1 = new String(field1.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("Alice", fetched1, "First row should be Alice");

        // Fetch second row
        byte[] data2 = new byte[20];
        AbstractCobolField field2 = makeAlphaField(20, data2);
        hasRow = cursor.fetch(conn, new AbstractCobolField[] {field2}, null);
        assertTrue(hasRow, "Second fetch should return a row");
        String fetched2 = new String(field2.getDataStorage().getByteArray(0, 3)).trim();
        assertEquals("Bob", fetched2, "Second row should be Bob");

        // Fetch third row
        byte[] data3 = new byte[20];
        AbstractCobolField field3 = makeAlphaField(20, data3);
        hasRow = cursor.fetch(conn, new AbstractCobolField[] {field3}, null);
        assertTrue(hasRow, "Third fetch should return a row");
        String fetched3 = new String(field3.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("Carol", fetched3, "Third row should be Carol");

        // Fetch past end
        hasRow = cursor.fetch(conn, null, null);
        assertFalse(hasRow, "Fetch past end should return false");

        // Close
        cursor.close(conn);
        assertFalse(cursor.isOpened, "Cursor should be closed after close");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testOpen_WithParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur2", "SELECT name FROM cursor_test WHERE id = ?", 1);

        AbstractCobolField param = makeNumericField(4, "0002".getBytes());
        cursor.open(conn, new AbstractCobolField[] {param});
        assertTrue(cursor.isOpened, "Cursor should be opened after open with params");

        byte[] data = new byte[20];
        AbstractCobolField field = makeAlphaField(20, data);
        boolean hasRow = cursor.fetch(conn, new AbstractCobolField[] {field}, null);
        assertTrue(hasRow, "Fetch should return a row");
        String fetched = new String(field.getDataStorage().getByteArray(0, 3)).trim();
        assertEquals("Bob", fetched, "Fetched row should be Bob");

        hasRow = cursor.fetch(conn, null, null);
        assertFalse(hasRow, "No more rows should be available");

        cursor.close(conn);
        assertFalse(cursor.isOpened, "Cursor should be closed");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testOpen_WithStoredParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur3", "SELECT name FROM cursor_test WHERE id = ?", 1);
        cursor.params = new AbstractCobolField[] {makeNumericField(4, "0001".getBytes())};

        cursor.open(conn, null);
        assertTrue(cursor.isOpened, "Cursor should be opened with stored params");

        byte[] data = new byte[20];
        AbstractCobolField field = makeAlphaField(20, data);
        boolean hasRow = cursor.fetch(conn, new AbstractCobolField[] {field}, null);
        assertTrue(hasRow, "Fetch should return a row");
        String fetched = new String(field.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("Alice", fetched, "Fetched row should be Alice");

        cursor.close(conn);
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testFetch_WithNullResultParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur4", "SELECT name FROM cursor_test ORDER BY id", 0);
        cursor.open(conn, null);

        // Fetch with null resultParams should return true but not write anything
        boolean hasRow = cursor.fetch(conn, null, null);
        assertTrue(hasRow, "Fetch with null resultParams should still return true");

        cursor.close(conn);
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testFetch_NullValue() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("INSERT INTO cursor_test VALUES (4, NULL)");
        }

        SqlCursor cursor = new SqlCursor("cur5", "SELECT name FROM cursor_test WHERE id = 4", 0);
        cursor.open(conn, null);

        byte[] data = new byte[20];
        java.util.Arrays.fill(data, (byte) 'X');
        AbstractCobolField field = makeAlphaField(20, data);
        // Allocate a SQLCA (133 bytes) so we can observe ECPG_MISSING_INDICATOR.
        CobolDataStorage sqlca = new CobolDataStorage(133);
        boolean hasRow = cursor.fetch(conn, new AbstractCobolField[] {field}, sqlca);
        assertTrue(hasRow, "Fetch should return a row even with NULL value");
        // NULL without indicator fills the host variable with its type-appropriate empty
        // value (alphanumeric => spaces), not raw 0x00.
        assertEquals(
                (byte) ' ',
                field.getDataStorage().getByte(0),
                "NULL fills an alphanumeric host variable with spaces (type-aware empty)");
        // ECPG semantics: NULL without indicator => sqlcode=-213, sqlstate="22002"
        assertEquals(
                SqlCA.ECPG_MISSING_INDICATOR,
                SqlCA.getCode(sqlca),
                "NULL without indicator should set SQLCODE to ECPG_MISSING_INDICATOR");
        byte[] state = sqlca.getByteArray(128, 5);
        assertEquals(
                "22002", new String(state), "NULL without indicator should set SQLSTATE to 22002");

        cursor.close(conn);
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testFetch_EmptyResultSet() throws Exception {
        SqlCursor cursor = new SqlCursor("cur6", "SELECT name FROM cursor_test WHERE id = 999", 0);
        cursor.open(conn, null);

        boolean hasRow = cursor.fetch(conn, null, null);
        assertFalse(hasRow, "Fetch on empty result set should return false");

        cursor.close(conn);
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testOpen_EmptyParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur7", "SELECT 1", 0);
        cursor.open(conn, new AbstractCobolField[0]);
        assertTrue(cursor.isOpened, "Cursor should be opened with empty params");
        cursor.close(conn);
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testClose_SetsIsOpenedFalse() throws Exception {
        SqlCursor cursor = new SqlCursor("cur8", "SELECT 1", 0);
        cursor.open(conn, null);
        assertTrue(cursor.isOpened, "Cursor should be opened");
        cursor.close(conn);
        assertFalse(cursor.isOpened, "Cursor should be closed after close");
    }

    // ---------- Multiple columns ----------

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testFetch_MultipleColumns() throws Exception {
        SqlCursor cursor = new SqlCursor("cur9", "SELECT id, name FROM cursor_test ORDER BY id", 0);
        cursor.open(conn, null);

        byte[] idData = new byte[10];
        byte[] nameData = new byte[20];
        AbstractCobolField idField = makeAlphaField(10, idData);
        AbstractCobolField nameField = makeAlphaField(20, nameData);

        boolean hasRow = cursor.fetch(conn, new AbstractCobolField[] {idField, nameField}, null);
        assertTrue(hasRow, "Fetch should return a row");

        String fetchedId = new String(idField.getDataStorage().getByteArray(0, 1)).trim();
        assertEquals("1", fetchedId, "Fetched id should be 1");

        String fetchedName = new String(nameField.getDataStorage().getByteArray(0, 5)).trim();
        assertEquals("Alice", fetchedName, "Fetched name should be Alice");

        cursor.close(conn);
    }
}

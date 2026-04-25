package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.lang.reflect.Field;
import java.util.HashMap;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class SqlStateTest {

    @BeforeEach
    @SuppressWarnings("PMD.AvoidAccessibilityAlteration")
    void resetStaticState() throws Exception {
        Field connectionsField = SqlState.class.getDeclaredField("connections");
        connectionsField.setAccessible(true);
        connectionsField.set(null, new HashMap<>());

        Field cursorsField = SqlState.class.getDeclaredField("cursors");
        cursorsField.setAccessible(true);
        cursorsField.set(null, new HashMap<>());

        Field preparedField = SqlState.class.getDeclaredField("preparedStatements");
        preparedField.setAccessible(true);
        preparedField.set(null, new HashMap<>());

        Field defaultField = SqlState.class.getDeclaredField("defaultConnId");
        defaultField.setAccessible(true);
        defaultField.set(null, null);
    }

    // ---------- Connection management ----------

    @Test
    void testAddAndGetConnection() {
        SqlConnection conn = new SqlConnection("test", null);
        SqlState.addConnection("test", conn);
        assertSame(
                conn, SqlState.getConnection("test"), "Should retrieve the same connection by id");
    }

    @Test
    void testFirstConnectionBecomesDefault() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlState.addConnection("c1", conn1);
        assertSame(
                conn1, SqlState.getConnection(null), "First connection should become the default");
    }

    @Test
    void testSecondConnectionDoesNotReplaceDefault() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlConnection conn2 = new SqlConnection("c2", null);
        SqlState.addConnection("c1", conn1);
        SqlState.addConnection("c2", conn2);
        assertSame(
                conn1, SqlState.getConnection(null), "Default should remain the first connection");
    }

    @Test
    void testGetConnectionById() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlConnection conn2 = new SqlConnection("c2", null);
        SqlState.addConnection("c1", conn1);
        SqlState.addConnection("c2", conn2);
        assertSame(conn2, SqlState.getConnection("c2"), "Should retrieve connection c2 by id");
    }

    @Test
    void testGetConnection_NonExistent() {
        assertNull(SqlState.getConnection("nope"), "Non-existent connection should return null");
    }

    @Test
    void testGetDefaultConnection_Empty() {
        assertNull(SqlState.getDefaultConnection(), "Default connection should be null when empty");
    }

    @Test
    void testRemoveConnection() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlState.addConnection("c1", conn1);
        SqlState.removeConnection("c1");
        assertNull(SqlState.getConnection("c1"), "Removed connection should return null");
    }

    @Test
    void testRemoveDefaultConnection_ReassignsDefault() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlConnection conn2 = new SqlConnection("c2", null);
        SqlState.addConnection("c1", conn1);
        SqlState.addConnection("c2", conn2);
        SqlState.removeConnection("c1");
        // default should be reassigned to c2
        assertSame(
                conn2,
                SqlState.getConnection(null),
                "Default should be reassigned to remaining connection");
    }

    @Test
    void testRemoveDefaultConnection_LastOne() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlState.addConnection("c1", conn1);
        SqlState.removeConnection("c1");
        assertNull(
                SqlState.getDefaultConnection(),
                "Default should be null after removing last connection");
    }

    @Test
    void testRemoveNonDefaultConnection_DefaultUnchanged() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlConnection conn2 = new SqlConnection("c2", null);
        SqlState.addConnection("c1", conn1);
        SqlState.addConnection("c2", conn2);
        SqlState.removeConnection("c2");
        assertSame(
                conn1,
                SqlState.getConnection(null),
                "Default should remain unchanged after removing non-default");
    }

    // ---------- Cursor management ----------

    @Test
    void testAddAndGetCursor() {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT 1", 0);
        SqlState.addCursor("cur1", cursor);
        assertSame(cursor, SqlState.getCursor("cur1"), "Should retrieve the same cursor by name");
    }

    @Test
    void testGetCursor_NonExistent() {
        assertNull(SqlState.getCursor("nope"), "Non-existent cursor should return null");
    }

    @Test
    void testRemoveCursor() {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT 1", 0);
        SqlState.addCursor("cur1", cursor);
        SqlState.removeCursor("cur1");
        assertNull(SqlState.getCursor("cur1"), "Removed cursor should return null");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testClearCursors() {
        SqlCursor c1 = new SqlCursor("c1", "q1", 0);
        SqlCursor c2 = new SqlCursor("c2", "q2", 0);
        c1.isOpened = true;
        c2.isOpened = true;
        SqlState.addCursor("c1", c1);
        SqlState.addCursor("c2", c2);
        SqlState.clearCursors();
        assertFalse(c1.isOpened, "Cursor c1 should be closed after clearCursors");
        assertFalse(c2.isOpened, "Cursor c2 should be closed after clearCursors");
    }

    @Test
    void testClearCursors_Empty() {
        assertDoesNotThrow(
                () -> SqlState.clearCursors(), "clearCursors on empty state should not throw");
    }

    // ---------- Prepared statement management ----------

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testAddAndGetPrepared() {
        SqlState.addPrepared("stmt1", "SELECT ?", 1);
        String[] result = SqlState.getPrepared("stmt1");
        assertNotNull(result, "Prepared statement should not be null");
        assertEquals("SELECT ?", result[0], "Prepared query should match");
        assertEquals("1", result[1], "Prepared param count should match");
    }

    @Test
    void testGetPrepared_NonExistent() {
        assertNull(
                SqlState.getPrepared("nope"), "Non-existent prepared statement should return null");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testAddPrepared_OverwritesExisting() {
        SqlState.addPrepared("stmt1", "SELECT 1", 0);
        SqlState.addPrepared("stmt1", "SELECT 2", 2);
        String[] result = SqlState.getPrepared("stmt1");
        assertEquals("SELECT 2", result[0], "Overwritten prepared query should be SELECT 2");
        assertEquals("2", result[1], "Overwritten prepared param count should be 2");
    }
}

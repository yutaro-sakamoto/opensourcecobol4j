package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.lang.reflect.Field;
import java.util.HashMap;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class SqlStateTest {

    @BeforeEach
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
        assertSame(conn, SqlState.getConnection("test"));
    }

    @Test
    void testFirstConnectionBecomesDefault() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlState.addConnection("c1", conn1);
        assertSame(conn1, SqlState.getConnection(null));
    }

    @Test
    void testSecondConnectionDoesNotReplaceDefault() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlConnection conn2 = new SqlConnection("c2", null);
        SqlState.addConnection("c1", conn1);
        SqlState.addConnection("c2", conn2);
        assertSame(conn1, SqlState.getConnection(null));
    }

    @Test
    void testGetConnectionById() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlConnection conn2 = new SqlConnection("c2", null);
        SqlState.addConnection("c1", conn1);
        SqlState.addConnection("c2", conn2);
        assertSame(conn2, SqlState.getConnection("c2"));
    }

    @Test
    void testGetConnection_NonExistent() {
        assertNull(SqlState.getConnection("nope"));
    }

    @Test
    void testGetDefaultConnection_Empty() {
        assertNull(SqlState.getDefaultConnection());
    }

    @Test
    void testRemoveConnection() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlState.addConnection("c1", conn1);
        SqlState.removeConnection("c1");
        assertNull(SqlState.getConnection("c1"));
    }

    @Test
    void testRemoveDefaultConnection_ReassignsDefault() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlConnection conn2 = new SqlConnection("c2", null);
        SqlState.addConnection("c1", conn1);
        SqlState.addConnection("c2", conn2);
        SqlState.removeConnection("c1");
        // default should be reassigned to c2
        assertSame(conn2, SqlState.getConnection(null));
    }

    @Test
    void testRemoveDefaultConnection_LastOne() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlState.addConnection("c1", conn1);
        SqlState.removeConnection("c1");
        assertNull(SqlState.getDefaultConnection());
    }

    @Test
    void testRemoveNonDefaultConnection_DefaultUnchanged() {
        SqlConnection conn1 = new SqlConnection("c1", null);
        SqlConnection conn2 = new SqlConnection("c2", null);
        SqlState.addConnection("c1", conn1);
        SqlState.addConnection("c2", conn2);
        SqlState.removeConnection("c2");
        assertSame(conn1, SqlState.getConnection(null));
    }

    // ---------- Cursor management ----------

    @Test
    void testAddAndGetCursor() {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT 1", 0);
        SqlState.addCursor("cur1", cursor);
        assertSame(cursor, SqlState.getCursor("cur1"));
    }

    @Test
    void testGetCursor_NonExistent() {
        assertNull(SqlState.getCursor("nope"));
    }

    @Test
    void testRemoveCursor() {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT 1", 0);
        SqlState.addCursor("cur1", cursor);
        SqlState.removeCursor("cur1");
        assertNull(SqlState.getCursor("cur1"));
    }

    @Test
    void testClearCursors() {
        SqlCursor c1 = new SqlCursor("c1", "q1", 0);
        SqlCursor c2 = new SqlCursor("c2", "q2", 0);
        c1.isOpened = true;
        c2.isOpened = true;
        SqlState.addCursor("c1", c1);
        SqlState.addCursor("c2", c2);
        SqlState.clearCursors();
        assertFalse(c1.isOpened);
        assertFalse(c2.isOpened);
    }

    @Test
    void testClearCursors_Empty() {
        assertDoesNotThrow(() -> SqlState.clearCursors());
    }

    // ---------- Prepared statement management ----------

    @Test
    void testAddAndGetPrepared() {
        SqlState.addPrepared("stmt1", "SELECT ?", 1);
        String[] result = SqlState.getPrepared("stmt1");
        assertNotNull(result);
        assertEquals("SELECT ?", result[0]);
        assertEquals("1", result[1]);
    }

    @Test
    void testGetPrepared_NonExistent() {
        assertNull(SqlState.getPrepared("nope"));
    }

    @Test
    void testAddPrepared_OverwritesExisting() {
        SqlState.addPrepared("stmt1", "SELECT 1", 0);
        SqlState.addPrepared("stmt1", "SELECT 2", 2);
        String[] result = SqlState.getPrepared("stmt1");
        assertEquals("SELECT 2", result[0]);
        assertEquals("2", result[1]);
    }
}

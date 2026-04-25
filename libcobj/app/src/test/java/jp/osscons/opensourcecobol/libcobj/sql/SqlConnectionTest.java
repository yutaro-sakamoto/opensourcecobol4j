package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.lang.reflect.Method;
import java.sql.*;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.Executor;
import org.junit.jupiter.api.Test;

class SqlConnectionTest {

    // ---------- buildJdbcUrl ----------

    @Test
    void testBuildJdbcUrl_Null() {
        assertEquals("jdbc:postgresql://localhost:5432/", SqlConnection.buildJdbcUrl(null));
    }

    @Test
    void testBuildJdbcUrl_Empty() {
        assertEquals("jdbc:postgresql://localhost:5432/", SqlConnection.buildJdbcUrl(""));
    }

    @Test
    void testBuildJdbcUrl_DbNameOnly() {
        assertEquals("jdbc:postgresql://localhost/mydb", SqlConnection.buildJdbcUrl("mydb"));
    }

    @Test
    void testBuildJdbcUrl_DbNameAtHost() {
        assertEquals("jdbc:postgresql://myhost/mydb", SqlConnection.buildJdbcUrl("mydb@myhost"));
    }

    @Test
    void testBuildJdbcUrl_DbNameAtHostWithPort() {
        assertEquals(
                "jdbc:postgresql://myhost:5433/mydb",
                SqlConnection.buildJdbcUrl("mydb@myhost:5433"));
    }

    @Test
    void testBuildJdbcUrl_DbNameAtEmptyHostWithPort() {
        // "mydb@:5433" => host is empty => defaults to localhost
        assertEquals(
                "jdbc:postgresql://localhost:5433/mydb", SqlConnection.buildJdbcUrl("mydb@:5433"));
    }

    @Test
    void testBuildJdbcUrl_DbNameAtHostNoPort() {
        assertEquals(
                "jdbc:postgresql://server1/testdb", SqlConnection.buildJdbcUrl("testdb@server1"));
    }

    // ---------- Constructor and getters ----------

    @Test
    void testGetId() {
        SqlConnection conn = new SqlConnection("myid", null);
        assertEquals("myid", conn.getId());
    }

    @Test
    void testGetConnection_Null() {
        SqlConnection conn = new SqlConnection("id", null);
        assertNull(conn.getConnection());
    }

    // ---------- stripTrailingSpaces (tested indirectly via buildJdbcUrl edge cases) ----------

    @Test
    void testBuildJdbcUrl_MultipleAtSigns() {
        // lastIndexOf('@') is used, so "a@b@c" => host="c", db="a@b"
        String result = SqlConnection.buildJdbcUrl("a@b@c");
        assertEquals("jdbc:postgresql://c/a@b", result);
    }

    // ---------- close() ----------

    @Test
    void testClose_NullConnection() throws Exception {
        SqlConnection conn = new SqlConnection("id", null);
        assertDoesNotThrow(() -> conn.close());
    }

    @Test
    void testClose_OpenConnection() throws Exception {
        MockConnection mock = new MockConnection();
        SqlConnection conn = new SqlConnection("id", mock);
        conn.close();
        assertTrue(mock.closeCalled);
    }

    @Test
    void testClose_AlreadyClosed() throws Exception {
        MockConnection mock = new MockConnection();
        mock.closedState = true;
        SqlConnection conn = new SqlConnection("id", mock);
        conn.close();
        assertFalse(mock.closeCalled); // should not call close() on already closed
    }

    // ---------- beginTransaction() ----------

    @Test
    void testBeginTransaction_NullConnection() throws Exception {
        SqlConnection conn = new SqlConnection("id", null);
        assertDoesNotThrow(() -> conn.beginTransaction());
    }

    @Test
    void testBeginTransaction_OpenConnection() throws Exception {
        MockConnection mock = new MockConnection();
        SqlConnection conn = new SqlConnection("id", mock);
        conn.beginTransaction();
        assertEquals("BEGIN", mock.lastExecutedSql);
    }

    @Test
    void testBeginTransaction_ClosedConnection() throws Exception {
        MockConnection mock = new MockConnection();
        mock.closedState = true;
        SqlConnection conn = new SqlConnection("id", mock);
        conn.beginTransaction();
        assertNull(mock.lastExecutedSql); // should not execute on closed
    }

    // ---------- stripTrailingSpaces via reflection ----------

    @Test
    void testStripTrailingSpaces_Null() throws Exception {
        Method m = SqlConnection.class.getDeclaredMethod("stripTrailingSpaces", String.class);
        m.setAccessible(true);
        assertNull(m.invoke(null, (String) null));
    }

    @Test
    void testStripTrailingSpaces_NoSpaces() throws Exception {
        Method m = SqlConnection.class.getDeclaredMethod("stripTrailingSpaces", String.class);
        m.setAccessible(true);
        assertEquals("hello", m.invoke(null, "hello"));
    }

    @Test
    void testStripTrailingSpaces_WithSpaces() throws Exception {
        Method m = SqlConnection.class.getDeclaredMethod("stripTrailingSpaces", String.class);
        m.setAccessible(true);
        assertEquals("hello", m.invoke(null, "hello   "));
    }

    @Test
    void testStripTrailingSpaces_LeadingSpace() throws Exception {
        Method m = SqlConnection.class.getDeclaredMethod("stripTrailingSpaces", String.class);
        m.setAccessible(true);
        // Space at index 0, end<=0 returns str
        assertEquals(" hello", m.invoke(null, " hello"));
    }

    // ---------- Mock Connection ----------

    private static class MockStatement implements Statement {
        String lastExecutedSql;

        @Override
        public boolean execute(String sql) {
            lastExecutedSql = sql;
            return false;
        }

        @Override
        public ResultSet executeQuery(String s) {
            return null;
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
        public ResultSet getResultSet() {
            return null;
        }

        @Override
        public int getUpdateCount() {
            return 0;
        }

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

    private static class MockConnection implements Connection {
        boolean closeCalled = false;
        boolean closedState = false;
        String lastExecutedSql;
        MockStatement mockStatement = new MockStatement();

        @Override
        public Statement createStatement() {
            return new MockStatement() {
                @Override
                public boolean execute(String sql) {
                    lastExecutedSql = sql;
                    MockConnection.this.lastExecutedSql = sql;
                    return false;
                }
            };
        }

        @Override
        public void close() {
            closeCalled = true;
            closedState = true;
        }

        @Override
        public boolean isClosed() {
            return closedState;
        }

        @Override
        public PreparedStatement prepareStatement(String sql) {
            return null;
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
            return createStatement();
        }

        @Override
        public PreparedStatement prepareStatement(String sql, int t, int c) {
            return null;
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
            return createStatement();
        }

        @Override
        public PreparedStatement prepareStatement(String sql, int t, int c, int h) {
            return null;
        }

        @Override
        public CallableStatement prepareCall(String sql, int t, int c, int h) {
            return null;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, int f) {
            return null;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, int[] cols) {
            return null;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, String[] cols) {
            return null;
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
            return !closedState;
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

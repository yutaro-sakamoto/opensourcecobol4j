package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.lang.reflect.Method;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;
import org.junit.jupiter.api.Test;
import org.testcontainers.containers.PostgreSQLContainer;
import org.testcontainers.junit.jupiter.Container;
import org.testcontainers.junit.jupiter.Testcontainers;

@Testcontainers
class SqlConnectionTest {

    @Container
    static PostgreSQLContainer<?> postgres =
            new PostgreSQLContainer<>("postgres:15")
                    .withDatabaseName("testdb")
                    .withUsername("test_user")
                    .withPassword("test_pass");

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
        assertEquals(
                "jdbc:postgresql://localhost:5433/mydb", SqlConnection.buildJdbcUrl("mydb@:5433"));
    }

    @Test
    void testBuildJdbcUrl_DbNameAtHostNoPort() {
        assertEquals(
                "jdbc:postgresql://server1/testdb", SqlConnection.buildJdbcUrl("testdb@server1"));
    }

    @Test
    void testBuildJdbcUrl_MultipleAtSigns() {
        String result = SqlConnection.buildJdbcUrl("a@b@c");
        assertEquals("jdbc:postgresql://c/a@b", result);
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

    // ---------- close() with real connection ----------

    @Test
    void testClose_NullConnection() throws Exception {
        SqlConnection conn = new SqlConnection("id", null);
        assertDoesNotThrow(() -> conn.close());
    }

    @Test
    void testClose_RealConnection() throws Exception {
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        assertFalse(realConn.isClosed());
        SqlConnection conn = new SqlConnection("id", realConn);
        conn.close();
        assertTrue(realConn.isClosed());
    }

    @Test
    void testClose_AlreadyClosedRealConnection() throws Exception {
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.close();
        assertTrue(realConn.isClosed());
        SqlConnection conn = new SqlConnection("id", realConn);
        assertDoesNotThrow(() -> conn.close());
    }

    // ---------- beginTransaction() with real connection ----------

    @Test
    void testBeginTransaction_NullConnection() throws Exception {
        SqlConnection conn = new SqlConnection("id", null);
        assertDoesNotThrow(() -> conn.beginTransaction());
    }

    @Test
    void testBeginTransaction_RealConnection() throws Exception {
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        SqlConnection conn = new SqlConnection("id", realConn);
        assertDoesNotThrow(() -> conn.beginTransaction());
        // Verify we can execute statements after BEGIN
        try (Statement stmt = realConn.createStatement()) {
            stmt.execute("SELECT 1");
        }
        realConn.close();
    }

    @Test
    void testBeginTransaction_ClosedRealConnection() throws Exception {
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.close();
        SqlConnection conn = new SqlConnection("id", realConn);
        // Should not throw - checks isClosed() first
        assertDoesNotThrow(() -> conn.beginTransaction());
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
        assertEquals(" hello", m.invoke(null, " hello"));
    }

    // ---------- connect() with real PostgreSQL ----------

    @Test
    void testConnect_RealPostgres() throws Exception {
        String dbSpec =
                "testdb@"
                        + postgres.getHost()
                        + ":"
                        + postgres.getMappedPort(PostgreSQLContainer.POSTGRESQL_PORT);
        SqlConnection conn =
                SqlConnection.connect(postgres.getUsername(), postgres.getPassword(), dbSpec);
        assertNotNull(conn);
        assertNotNull(conn.getConnection());
        assertFalse(conn.getConnection().isClosed());
        assertEquals("OCDB_DEFAULT_DBNAME", conn.getId());
        conn.close();
    }

    @Test
    void testConnect_InvalidCredentials() {
        String dbSpec =
                "testdb@"
                        + postgres.getHost()
                        + ":"
                        + postgres.getMappedPort(PostgreSQLContainer.POSTGRESQL_PORT);
        assertThrows(Exception.class, () -> SqlConnection.connect("bad_user", "bad_pass", dbSpec));
    }
}

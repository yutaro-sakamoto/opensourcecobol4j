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
        assertEquals(
                "jdbc:postgresql://localhost:5432/",
                SqlConnection.buildJdbcUrl(null),
                "Null input should produce default URL");
    }

    @Test
    void testBuildJdbcUrl_Empty() {
        assertEquals(
                "jdbc:postgresql://localhost:5432/",
                SqlConnection.buildJdbcUrl(""),
                "Empty input should produce default URL");
    }

    @Test
    void testBuildJdbcUrl_DbNameOnly() {
        assertEquals(
                "jdbc:postgresql://localhost/mydb",
                SqlConnection.buildJdbcUrl("mydb"),
                "DB name only should use localhost");
    }

    @Test
    void testBuildJdbcUrl_DbNameAtHost() {
        assertEquals(
                "jdbc:postgresql://myhost/mydb",
                SqlConnection.buildJdbcUrl("mydb@myhost"),
                "Should parse host from db@host format");
    }

    @Test
    void testBuildJdbcUrl_DbNameAtHostWithPort() {
        assertEquals(
                "jdbc:postgresql://myhost:5433/mydb",
                SqlConnection.buildJdbcUrl("mydb@myhost:5433"),
                "Should parse host and port from db@host:port format");
    }

    @Test
    void testBuildJdbcUrl_DbNameAtEmptyHostWithPort() {
        assertEquals(
                "jdbc:postgresql://localhost:5433/mydb",
                SqlConnection.buildJdbcUrl("mydb@:5433"),
                "Empty host should default to localhost");
    }

    @Test
    void testBuildJdbcUrl_DbNameAtHostNoPort() {
        assertEquals(
                "jdbc:postgresql://server1/testdb",
                SqlConnection.buildJdbcUrl("testdb@server1"),
                "Should parse host without port");
    }

    @Test
    void testBuildJdbcUrl_MultipleAtSigns() {
        String result = SqlConnection.buildJdbcUrl("a@b@c");
        assertEquals("jdbc:postgresql://c/a@b", result, "Should split on last @ sign");
    }

    // ---------- Constructor and getters ----------

    @Test
    void testGetId() {
        SqlConnection conn = new SqlConnection("myid", null);
        assertEquals("myid", conn.getId(), "getId should return the connection id");
    }

    @Test
    void testGetConnection_Null() {
        SqlConnection conn = new SqlConnection("id", null);
        assertNull(conn.getConnection(), "getConnection should return null when created with null");
    }

    // ---------- close() with real connection ----------

    @Test
    void testClose_NullConnection() throws Exception {
        SqlConnection conn = new SqlConnection("id", null);
        assertDoesNotThrow(() -> conn.close(), "close with null connection should not throw");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testClose_RealConnection() throws Exception {
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        assertFalse(realConn.isClosed(), "Connection should be open before close");
        SqlConnection conn = new SqlConnection("id", realConn);
        conn.close();
        assertTrue(realConn.isClosed(), "Connection should be closed after close");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testClose_AlreadyClosedRealConnection() throws Exception {
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        realConn.close();
        assertTrue(realConn.isClosed(), "Connection should be already closed");
        SqlConnection conn = new SqlConnection("id", realConn);
        assertDoesNotThrow(
                () -> conn.close(), "Closing already-closed connection should not throw");
    }

    // ---------- beginTransaction() with real connection ----------

    @Test
    void testBeginTransaction_NullConnection() throws Exception {
        SqlConnection conn = new SqlConnection("id", null);
        assertDoesNotThrow(
                () -> conn.beginTransaction(),
                "beginTransaction with null connection should not throw");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testBeginTransaction_RealConnection() throws Exception {
        Connection realConn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        SqlConnection conn = new SqlConnection("id", realConn);
        assertDoesNotThrow(
                () -> conn.beginTransaction(),
                "beginTransaction should not throw on real connection");
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
        assertDoesNotThrow(
                () -> conn.beginTransaction(),
                "beginTransaction on closed connection should not throw");
    }

    // ---------- stripTrailingSpaces via reflection ----------

    @Test
    @SuppressWarnings("PMD.AvoidAccessibilityAlteration")
    void testStripTrailingSpaces_Null() throws Exception {
        Method m = SqlConnection.class.getDeclaredMethod("stripTrailingSpaces", String.class);
        m.setAccessible(true);
        assertNull(m.invoke(null, (String) null), "stripTrailingSpaces of null should return null");
    }

    @Test
    @SuppressWarnings("PMD.AvoidAccessibilityAlteration")
    void testStripTrailingSpaces_NoSpaces() throws Exception {
        Method m = SqlConnection.class.getDeclaredMethod("stripTrailingSpaces", String.class);
        m.setAccessible(true);
        assertEquals(
                "hello",
                m.invoke(null, "hello"),
                "String without trailing spaces should be unchanged");
    }

    @Test
    @SuppressWarnings("PMD.AvoidAccessibilityAlteration")
    void testStripTrailingSpaces_WithSpaces() throws Exception {
        Method m = SqlConnection.class.getDeclaredMethod("stripTrailingSpaces", String.class);
        m.setAccessible(true);
        assertEquals("hello", m.invoke(null, "hello   "), "Trailing spaces should be stripped");
    }

    @Test
    @SuppressWarnings("PMD.AvoidAccessibilityAlteration")
    void testStripTrailingSpaces_LeadingSpace() throws Exception {
        Method m = SqlConnection.class.getDeclaredMethod("stripTrailingSpaces", String.class);
        m.setAccessible(true);
        assertEquals(" hello", m.invoke(null, " hello"), "Leading spaces should be preserved");
    }

    @Test
    @SuppressWarnings("PMD.AvoidAccessibilityAlteration")
    void testStripTrailingSpaces_EmbeddedSpace() throws Exception {
        Method m = SqlConnection.class.getDeclaredMethod("stripTrailingSpaces", String.class);
        m.setAccessible(true);
        // 値の途中の空白 (例: 空白を含むパスワード) は保持し、末尾のパディングのみ除去する
        assertEquals(
                "pass word",
                m.invoke(null, "pass word   "),
                "Embedded spaces should be preserved; only trailing padding is stripped");
    }

    // ---------- connect() with real PostgreSQL ----------

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testConnect_RealPostgres() throws Exception {
        String dbSpec =
                "testdb@"
                        + postgres.getHost()
                        + ":"
                        + postgres.getMappedPort(PostgreSQLContainer.POSTGRESQL_PORT);
        SqlConnection conn =
                SqlConnection.connect(postgres.getUsername(), postgres.getPassword(), dbSpec);
        assertNotNull(conn, "SqlConnection should not be null after connect");
        assertNotNull(conn.getConnection(), "JDBC connection should not be null");
        assertFalse(conn.getConnection().isClosed(), "Connection should be open");
        assertEquals(
                "OCDB_DEFAULT_DBNAME",
                conn.getId(),
                "Default connection id should be OCDB_DEFAULT_DBNAME");
        conn.close();
    }

    @Test
    void testConnect_InvalidCredentials() {
        String dbSpec =
                "testdb@"
                        + postgres.getHost()
                        + ":"
                        + postgres.getMappedPort(PostgreSQLContainer.POSTGRESQL_PORT);
        assertThrows(
                Exception.class,
                () -> SqlConnection.connect("bad_user", "bad_pass", dbSpec),
                "Invalid credentials should throw an exception");
    }
}

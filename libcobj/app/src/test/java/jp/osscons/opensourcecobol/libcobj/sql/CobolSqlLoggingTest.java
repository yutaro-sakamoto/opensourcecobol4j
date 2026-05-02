package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import com.github.valfirst.slf4jtest.LoggingEvent;
import com.github.valfirst.slf4jtest.TestLogger;
import com.github.valfirst.slf4jtest.TestLoggerFactory;
import java.lang.reflect.Field;
import java.nio.ByteBuffer;
import java.sql.Connection;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.slf4j.event.Level;
import org.testcontainers.containers.PostgreSQLContainer;
import org.testcontainers.junit.jupiter.Container;
import org.testcontainers.junit.jupiter.Testcontainers;

@Testcontainers
class CobolSqlLoggingTest {

    @Container
    static PostgreSQLContainer<?> postgres =
            new PostgreSQLContainer<>("postgres:15")
                    .withDatabaseName("testdb")
                    .withUsername("test_user")
                    .withPassword("test_pass");

    private CobolDataStorage sqlca;
    private TestLogger cobolSqlLogger;
    private TestLogger sqlConnectionLogger;

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

        cobolSqlLogger = TestLoggerFactory.getTestLogger(CobolSql.class);
        sqlConnectionLogger = TestLoggerFactory.getTestLogger(SqlConnection.class);
        TestLoggerFactory.clear();
    }

    @SuppressWarnings({"unchecked", "PMD.AvoidAccessibilityAlteration"})
    @AfterEach
    void tearDown() throws Exception {
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
        TestLoggerFactory.clear();
    }

    @SuppressWarnings("PMD.AvoidAccessibilityAlteration")
    private void resetStaticField(Class<?> clazz, String fieldName, Object value) throws Exception {
        Field f = clazz.getDeclaredField(fieldName);
        f.setAccessible(true);
        f.set(null, value);
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

    private void connectToPostgres() {
        String dbSpec =
                "testdb@"
                        + postgres.getHost()
                        + ":"
                        + postgres.getMappedPort(PostgreSQLContainer.POSTGRESQL_PORT);
        CobolDataStorage userStorage = makeStorage(postgres.getUsername());
        CobolDataStorage passStorage = makeStorage(postgres.getPassword());
        CobolDataStorage dbStorage = makeStorage(dbSpec);
        CobolSql.connect(
                sqlca,
                userStorage,
                postgres.getUsername().length(),
                passStorage,
                postgres.getPassword().length(),
                dbStorage,
                dbSpec.length());
    }

    @Test
    void testConnect_logsDebugMessage() {
        connectToPostgres();
        assertEquals(0, getSqlCode());

        List<LoggingEvent> events = cobolSqlLogger.getLoggingEvents();
        assertTrue(
                events.stream()
                        .anyMatch(
                                e ->
                                        e.getLevel() == Level.DEBUG
                                                && e.getMessage().contains("CONNECT")),
                "Expected DEBUG log containing 'CONNECT', got: " + events);
    }

    @Test
    void testConnect_SqlConnection_logsDebugMessage() {
        connectToPostgres();
        assertEquals(0, getSqlCode());

        List<LoggingEvent> events = sqlConnectionLogger.getLoggingEvents();
        assertTrue(
                events.stream()
                        .anyMatch(
                                e ->
                                        e.getLevel() == Level.DEBUG
                                                && e.getMessage().contains("Connecting to")),
                "Expected DEBUG log containing 'Connecting to', got: " + events);
    }

    @Test
    void testExec_logsDebugMessage() throws Exception {
        connectToPostgres();
        TestLoggerFactory.clear();

        CobolSql.exec(sqlca, "DROP TABLE IF EXISTS test_log_tbl");
        assertEquals(0, getSqlCode());

        List<LoggingEvent> events = cobolSqlLogger.getLoggingEvents();
        assertTrue(
                events.stream()
                        .anyMatch(
                                e ->
                                        e.getLevel() == Level.DEBUG
                                                && e.getMessage().contains("EXEC SQL")),
                "Expected DEBUG log containing 'EXEC SQL', got: " + events);
    }

    @Test
    void testExec_failure_logsError() throws Exception {
        connectToPostgres();
        TestLoggerFactory.clear();

        CobolSql.exec(sqlca, "INVALID SQL SYNTAX HERE");
        assertNotEquals(0, getSqlCode());

        List<LoggingEvent> events = cobolSqlLogger.getLoggingEvents();
        assertTrue(
                events.stream().anyMatch(e -> e.getLevel() == Level.ERROR),
                "Expected ERROR log on SQL failure, got: " + events);
    }

    @Test
    void testDisconnect_logsDebugMessage() throws Exception {
        connectToPostgres();
        TestLoggerFactory.clear();

        CobolSql.disconnect(sqlca);
        assertEquals(0, getSqlCode());

        List<LoggingEvent> events = cobolSqlLogger.getLoggingEvents();
        assertTrue(
                events.stream()
                        .anyMatch(
                                e ->
                                        e.getLevel() == Level.DEBUG
                                                && e.getMessage().contains("DISCONNECT")),
                "Expected DEBUG log containing 'DISCONNECT', got: " + events);
    }

    @Test
    void testConnect_doesNotLogPassword() {
        connectToPostgres();

        List<LoggingEvent> allEvents = new java.util.ArrayList<>(cobolSqlLogger.getLoggingEvents());
        allEvents.addAll(sqlConnectionLogger.getLoggingEvents());

        String password = postgres.getPassword();
        assertTrue(
                allEvents.stream().noneMatch(e -> e.getFormattedMessage().contains(password)),
                "Password should never appear in log output");
    }
}

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
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory;
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
    void setUp() throws Exception {
        sqlca = new CobolDataStorage(136);
        SqlState.resetThreadState();
        Field cacheField = CobolSql.class.getDeclaredField("stmtCache");
        cacheField.setAccessible(true);
        ((ConcurrentHashMap<?, ?>) cacheField.get(null)).clear();

        cobolSqlLogger = TestLoggerFactory.getTestLogger(CobolSql.class);
        sqlConnectionLogger = TestLoggerFactory.getTestLogger(SqlConnection.class);
        TestLoggerFactory.clear();
    }

    @AfterEach
    void tearDown() throws Exception {
        for (SqlConnection sc : SqlState.allConnections().values()) {
            try {
                Connection c = sc.getConnection();
                if (c != null && !c.isClosed()) {
                    c.close();
                }
            } catch (Exception ignored) {
            }
        }
        SqlState.resetThreadState();
        Field cacheField = CobolSql.class.getDeclaredField("stmtCache");
        cacheField.setAccessible(true);
        ((ConcurrentHashMap<?, ?>) cacheField.get(null)).clear();
        TestLoggerFactory.clear();
    }

    private CobolDataStorage makeStorage(String value) {
        byte[] bytes = value.getBytes();
        CobolDataStorage s = new CobolDataStorage(bytes.length);
        s.memcpy(bytes, bytes.length);
        return s;
    }

    private static AbstractCobolField makeAlphaField(int size, byte[] data) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        return CobolFieldFactory.makeCobolField(size, storage, attr);
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
        byte[] userBytes = postgres.getUsername().getBytes();
        byte[] passBytes = postgres.getPassword().getBytes();
        byte[] dbBytes = dbSpec.getBytes();
        AbstractCobolField userField = makeAlphaField(userBytes.length, userBytes);
        AbstractCobolField passField = makeAlphaField(passBytes.length, passBytes);
        AbstractCobolField dbField = makeAlphaField(dbBytes.length, dbBytes);
        CobolSql.connect(sqlca, userField, passField, dbField);
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

package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.lang.reflect.Proxy;
import java.sql.Connection;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

/**
 * {@link AbstractCobolEsqlBackend} のカーソル無効化ライフサイクル（{@code onCursorsInvalidated}
 * フックの配線）を、DB 接続なしで検証する。スタブバックエンドと {@link Proxy} 製の {@link Connection}
 * でフックの発火回数・発火順序（特に DISCONNECT では接続クローズより前）を確認する。
 */
class AbstractCobolEsqlBackendLifecycleTest {

    private static final String CONN_ID = "OCDB_DEFAULT_DBNAME";

    /** フック呼び出しと接続クローズの順序を記録するイベントログ。 */
    private List<String> events;

    private StubBackend backend;
    private CobolDataStorage sqlca;

    /** DB へ接続しないスタブバックエンド。ライフサイクルフックの呼び出しを記録するだけ。 */
    private static final class StubBackend extends AbstractCobolEsqlBackend {

        private final List<String> events;

        StubBackend(List<String> events) {
            this.events = events;
        }

        @Override
        public String id() {
            return "stub";
        }

        @Override
        protected String buildJdbcUrl(DbSpec spec) {
            return "jdbc:stub";
        }

        @Override
        protected void configureConnection(Connection c) {
            // no-op（接続後初期化なし）
        }

        @Override
        protected void beginTransaction(Connection c) {
            events.add("beginTransaction");
        }

        @Override
        protected void commitTransaction(Connection c) {
            events.add("commitTransaction");
        }

        @Override
        protected void rollbackTransaction(Connection c) {
            events.add("rollbackTransaction");
        }

        @Override
        protected void openCursorImpl(Connection c, Cursor cur, AbstractCobolField[] params) {
            // no-op（SQL は発行しない）
        }

        @Override
        protected boolean fetchRowImpl(
                Connection c, Cursor cur, AbstractCobolField[] out, CobolDataStorage sqlca) {
            return false;
        }

        @Override
        protected void fetchOccursImpl(
                Connection c,
                Cursor cur,
                int occursSize,
                int occursMax,
                AbstractCobolField[] resultParams,
                CobolDataStorage sqlca) {
            // no-op
        }

        @Override
        protected void closeCursorImpl(Connection c, Cursor cur) {
            events.add("closeCursorImpl");
        }

        @Override
        protected void repositionForCurrentOf(Connection c, Cursor cur, CobolDataStorage sqlca) {
            // no-op
        }

        @Override
        protected SqlErrorMapping mapSqlException(SQLException e) {
            return new SqlErrorMapping(SqlCA.ECPG_UNKNOWN_ERROR, e.getSQLState());
        }

        @Override
        protected void onCursorsInvalidated() {
            events.add("onCursorsInvalidated");
        }
    }

    @BeforeEach
    void setUp() {
        events = new ArrayList<>();
        backend = new StubBackend(events);
        sqlca = new CobolDataStorage(136);
    }

    /**
     * 動的プロキシ製の JDBC 接続。{@code close()} の呼び出しをイベントログへ記録し、
     * {@code isClosed()} は false を返す。それ以外のメソッドは型の既定値を返す。
     */
    private Connection stubConnection() {
        return (Connection)
                Proxy.newProxyInstance(
                        AbstractCobolEsqlBackendLifecycleTest.class.getClassLoader(),
                        new Class<?>[] {Connection.class},
                        (proxy, method, args) -> {
                            if ("close".equals(method.getName())) {
                                events.add("connectionClose");
                                return null;
                            }
                            Class<?> returnType = method.getReturnType();
                            if (returnType == boolean.class) {
                                return false;
                            }
                            if (returnType == int.class) {
                                return 0;
                            }
                            return null;
                        });
    }

    private void registerConnection() {
        backend.addConnection(CONN_ID, stubConnection());
    }

    private void registerOpenedCursor(String name) {
        AbstractCobolEsqlBackend.Cursor cursor =
                new AbstractCobolEsqlBackend.Cursor(name, "SELECT 1", 0);
        cursor.isOpened = true;
        backend.addCursor(name, cursor);
    }

    private int sqlCode() {
        return SqlCA.getCode(sqlca);
    }

    private long count(String event) {
        return events.stream().filter(event::equals).count();
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testCommit_NotifiesCursorInvalidation() {
        registerConnection();
        registerOpenedCursor("c1");
        backend.commit(sqlca);
        assertEquals(0, sqlCode(), "commit should succeed");
        assertEquals(1, count("onCursorsInvalidated"), "COMMIT should notify invalidation once");
        assertFalse(backend.getCursor("c1").isOpened, "cursor should be closed after COMMIT");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testRollback_NotifiesCursorInvalidation() {
        registerConnection();
        registerOpenedCursor("c1");
        backend.rollback(sqlca);
        assertEquals(0, sqlCode(), "rollback should succeed");
        assertEquals(1, count("onCursorsInvalidated"), "ROLLBACK should notify invalidation once");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testDisconnect_NotifiesCursorInvalidationBeforeConnectionClose() {
        registerConnection();
        registerOpenedCursor("c1");
        backend.disconnect(sqlca);
        assertEquals(0, sqlCode(), "disconnect should succeed");
        // 切断経路でもフックが発火し、かつ接続クローズより前であること（クローズ後の解放は
        // ドライバ依存の JDBC カスケードに巻き込まれ、解放順序を保証できない）。
        assertEquals(
                Arrays.asList("commitTransaction", "onCursorsInvalidated", "connectionClose"),
                events,
                "DISCONNECT should notify invalidation after commit and before connection close");
        assertFalse(backend.getCursor("c1").isOpened, "cursor should be closed after DISCONNECT");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testCloseCursor_DoesNotNotifyInvalidation() {
        registerConnection();
        registerOpenedCursor("c1");
        backend.closeCursor(sqlca, "c1");
        assertEquals(0, sqlCode(), "close cursor should succeed");
        assertEquals(1, count("closeCursorImpl"), "explicit CLOSE should reach closeCursorImpl");
        assertEquals(
                0,
                count("onCursorsInvalidated"),
                "explicit CLOSE of a single cursor is not a bulk invalidation");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testCommit_NoConnection_DoesNotNotify() {
        backend.commit(sqlca);
        assertEquals(SqlCA.ECPG_NO_CONN, sqlCode(), "commit without connection should fail");
        assertEquals(
                0, count("onCursorsInvalidated"), "failed commit should not notify invalidation");
    }
}

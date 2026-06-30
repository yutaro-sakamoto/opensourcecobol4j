package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.nio.ByteBuffer;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

/**
 * {@link CobolSqlFactory} の env 非依存な解決ロジック（resolve(String)）と、解決失敗時に
 * {@link CobolSql} ファサードが例外を投げず SQLCA へエラーを報告する挙動を検証する。
 */
class CobolSqlFactoryTest {

    @AfterEach
    void tearDown() {
        // 強制した解決失敗状態を解除し、テスト間でリークさせない。
        CobolSql.resetBackend();
    }

    private static int sqlCode(CobolDataStorage sqlca) {
        return ByteBuffer.wrap(sqlca.getByteArray(12, 4)).getInt();
    }

    private static String sqlState(CobolDataStorage sqlca) {
        return new String(sqlca.getByteArray(128, 5));
    }

    @Test
    void testResolve_Null_DefaultsToPostgresql() {
        assertInstanceOf(CobolSqlPostgresql.class, CobolSqlFactory.resolve((String) null));
    }

    @Test
    void testResolve_Empty_DefaultsToPostgresql() {
        assertInstanceOf(CobolSqlPostgresql.class, CobolSqlFactory.resolve(""));
    }

    @Test
    void testResolve_Postgresql() {
        assertInstanceOf(CobolSqlPostgresql.class, CobolSqlFactory.resolve("postgresql"));
    }

    @Test
    void testResolve_PostgresAlias() {
        assertInstanceOf(CobolSqlPostgresql.class, CobolSqlFactory.resolve("postgres"));
    }

    @Test
    void testResolve_CaseInsensitive() {
        assertInstanceOf(CobolSqlPostgresql.class, CobolSqlFactory.resolve("PostgreSQL"));
    }

    @Test
    void testResolve_Unsupported_Throws() {
        assertThrows(IllegalArgumentException.class, () -> CobolSqlFactory.resolve("mysql"));
    }

    @Test
    void testUnconfigured_ConnectReportsConnectError() {
        // 未対応の OCDB_DB_TYPE を上書きし、実際の解決失敗経路を駆動する。
        CobolSql.setDbTypeForTest("mysql");
        CobolDataStorage sqlca = new CobolDataStorage(136);
        // 例外を投げず、CONNECT は ECPG_CONNECT/08001 を SQLCA に報告する。
        assertDoesNotThrow(() -> CobolSql.connect(sqlca, null, null, null));
        assertEquals(SqlCA.ECPG_CONNECT, sqlCode(sqlca), "CONNECT should report ECPG_CONNECT");
        assertEquals("08001", sqlState(sqlca), "CONNECT should set SQLSTATE 08001");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testUnconfigured_AllOtherOperationsReportNoConnection() {
        CobolSql.setDbTypeForTest("mysql");
        CobolDataStorage sqlca = new CobolDataStorage(136);
        // CONNECT 以外の全エントリポイントは、backend 未解決＝接続なしとして例外を投げず
        // ECPG_NO_CONN/08003 を報告する（接続失敗時と同じ挙動）。
        assertNoConn(sqlca, "disconnect", () -> CobolSql.disconnect(sqlca));
        assertNoConn(sqlca, "exec", () -> CobolSql.exec(sqlca, "SELECT 1"));
        assertNoConn(sqlca, "execWithParams", () -> CobolSql.execWithParams(sqlca, "SELECT 1"));
        assertNoConn(
                sqlca,
                "execWhereCurrentOf",
                () -> CobolSql.execWhereCurrentOf(sqlca, "UPDATE t SET a=1 WHERE CURRENT OF", "c"));
        assertNoConn(
                sqlca,
                "execWithParamsWhereCurrentOf",
                () ->
                        CobolSql.execWithParamsWhereCurrentOf(
                                sqlca, "UPDATE t SET a=1 WHERE CURRENT OF", "c"));
        assertNoConn(sqlca, "selectInto", () -> CobolSql.selectInto(sqlca, "SELECT 1", null, null));
        assertNoConn(
                sqlca,
                "selectIntoOccurs",
                () -> CobolSql.selectIntoOccurs(sqlca, 10, 2, "SELECT 1", null, null));
        assertNoConn(sqlca, "declareCursor", () -> CobolSql.declareCursor(sqlca, "c", "SELECT 1"));
        assertNoConn(
                sqlca,
                "declareCursorWithParams",
                () -> CobolSql.declareCursorWithParams(sqlca, "c", "SELECT 1"));
        assertNoConn(sqlca, "openCursor", () -> CobolSql.openCursor(sqlca, "c"));
        assertNoConn(
                sqlca, "openCursorWithParams", () -> CobolSql.openCursorWithParams(sqlca, "c"));
        assertNoConn(sqlca, "fetchCursor", () -> CobolSql.fetchCursor(sqlca, "c"));
        assertNoConn(
                sqlca, "fetchCursorOccurs", () -> CobolSql.fetchCursorOccurs(sqlca, "c", 10, 2));
        assertNoConn(sqlca, "closeCursor", () -> CobolSql.closeCursor(sqlca, "c"));
        assertNoConn(sqlca, "prepare", () -> CobolSql.prepare(sqlca, "s", null));
        assertNoConn(sqlca, "executePrepared", () -> CobolSql.executePrepared(sqlca, "s"));
        assertNoConn(sqlca, "commit", () -> CobolSql.commit(sqlca));
        assertNoConn(sqlca, "rollback", () -> CobolSql.rollback(sqlca));
    }

    private void assertNoConn(CobolDataStorage sqlca, String name, Runnable op) {
        assertDoesNotThrow(op::run, name + " should not throw");
        assertEquals(SqlCA.ECPG_NO_CONN, sqlCode(sqlca), name + " should report ECPG_NO_CONN");
        assertEquals("08003", sqlState(sqlca), name + " should set SQLSTATE 08003");
    }
}

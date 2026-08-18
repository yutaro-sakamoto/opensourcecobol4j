package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

/**
 * {@link CobolEsqlBackendFactory} の env 非依存な解決ロジック（resolve(String)）と、解決に失敗した
 * とき {@link CobolEsql} ファサードが例外を伝播させる（SQL のエラーに読み替えない）ことを検証する。
 */
class CobolEsqlBackendFactoryTest {

    @AfterEach
    void tearDown() {
        // 上書きした OCDB_DB_TYPE を解除し、テスト間でリークさせない。
        CobolEsql.resetBackend();
    }

    @Test
    void testResolve_Null_DefaultsToPostgresql() {
        assertInstanceOf(
                CobolEsqlBackendPostgresql.class, CobolEsqlBackendFactory.resolve((String) null));
    }

    @Test
    void testResolve_Empty_DefaultsToPostgresql() {
        assertInstanceOf(CobolEsqlBackendPostgresql.class, CobolEsqlBackendFactory.resolve(""));
    }

    @Test
    void testResolve_Postgresql() {
        assertInstanceOf(
                CobolEsqlBackendPostgresql.class, CobolEsqlBackendFactory.resolve("postgresql"));
    }

    @Test
    void testResolve_CaseInsensitive() {
        assertInstanceOf(
                CobolEsqlBackendPostgresql.class, CobolEsqlBackendFactory.resolve("PostgreSQL"));
    }

    @Test
    void testResolve_Unsupported_Throws() {
        assertThrows(CobolRuntimeException.class, () -> CobolEsqlBackendFactory.resolve("mysql"));
    }

    @Test
    void testUnconfigured_ConnectThrows() {
        // 未対応の OCDB_DB_TYPE を上書きし、実際の解決失敗経路を駆動する。
        CobolEsql.setDbTypeForTest("mysql");
        CobolDataStorage sqlca = new CobolDataStorage(136);
        // 設定の誤りは SQLCA へ報告せず、そのまま呼び出し元へ伝播させる。
        assertThrows(CobolRuntimeException.class, () -> CobolEsql.connect(sqlca, null, null, null));
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testUnconfigured_AllOtherOperationsThrow() {
        CobolEsql.setDbTypeForTest("mysql");
        CobolDataStorage sqlca = new CobolDataStorage(136);
        // CONNECT 以外の全エントリポイントも同じく伝播させる（接続なしとして握りつぶさない）。
        assertThrowsUnsupported("disconnect", () -> CobolEsql.disconnect(sqlca));
        assertThrowsUnsupported("exec", () -> CobolEsql.exec(sqlca, "SELECT 1"));
        assertThrowsUnsupported(
                "execWithParams", () -> CobolEsql.execWithParams(sqlca, "SELECT 1"));
        assertThrowsUnsupported(
                "execWhereCurrentOf",
                () ->
                        CobolEsql.execWhereCurrentOf(
                                sqlca, "UPDATE t SET a=1 WHERE CURRENT OF", "c"));
        assertThrowsUnsupported(
                "execWithParamsWhereCurrentOf",
                () ->
                        CobolEsql.execWithParamsWhereCurrentOf(
                                sqlca, "UPDATE t SET a=1 WHERE CURRENT OF", "c"));
        assertThrowsUnsupported(
                "selectInto", () -> CobolEsql.selectInto(sqlca, "SELECT 1", null, null));
        assertThrowsUnsupported(
                "selectIntoOccurs",
                () -> CobolEsql.selectIntoOccurs(sqlca, 10, 2, "SELECT 1", null, null));
        assertThrowsUnsupported(
                "declareCursor", () -> CobolEsql.declareCursor(sqlca, "c", "SELECT 1"));
        assertThrowsUnsupported(
                "declareCursorWithParams",
                () -> CobolEsql.declareCursorWithParams(sqlca, "c", "SELECT 1"));
        assertThrowsUnsupported("openCursor", () -> CobolEsql.openCursor(sqlca, "c"));
        assertThrowsUnsupported(
                "openCursorWithParams", () -> CobolEsql.openCursorWithParams(sqlca, "c"));
        assertThrowsUnsupported("fetchCursor", () -> CobolEsql.fetchCursor(sqlca, "c"));
        assertThrowsUnsupported(
                "fetchCursorOccurs", () -> CobolEsql.fetchCursorOccurs(sqlca, "c", 10, 2));
        assertThrowsUnsupported("closeCursor", () -> CobolEsql.closeCursor(sqlca, "c"));
        assertThrowsUnsupported("prepare", () -> CobolEsql.prepare(sqlca, "s", null));
        assertThrowsUnsupported("executePrepared", () -> CobolEsql.executePrepared(sqlca, "s"));
        assertThrowsUnsupported("commit", () -> CobolEsql.commit(sqlca));
        assertThrowsUnsupported("rollback", () -> CobolEsql.rollback(sqlca));
    }

    private void assertThrowsUnsupported(String name, Runnable op) {
        assertThrows(CobolRuntimeException.class, op::run, name + " should propagate the failure");
    }
}

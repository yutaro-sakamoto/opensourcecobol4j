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
        // backend の解決失敗（例: OCDB_DB_TYPE 不正）を強制する。
        CobolSql.forceResolutionFailureForTest("Unsupported OCDB_DB_TYPE: mysql");
        CobolDataStorage sqlca = new CobolDataStorage(136);
        // 例外を投げず、CONNECT は ECPG_CONNECT/08001 を SQLCA に報告する。
        assertDoesNotThrow(() -> CobolSql.connect(sqlca, null, null, null));
        assertEquals(SqlCA.ECPG_CONNECT, sqlCode(sqlca), "CONNECT should report ECPG_CONNECT");
        assertEquals("08001", sqlState(sqlca), "CONNECT should set SQLSTATE 08001");
    }

    @Test
    void testUnconfigured_OtherOperationsReportNoConnection() {
        CobolSql.forceResolutionFailureForTest("Unsupported OCDB_DB_TYPE: mysql");
        CobolDataStorage sqlca = new CobolDataStorage(136);
        // CONNECT 以外は接続が無いため ECPG_NO_CONN/08003 を報告する（接続失敗時と同じ挙動）。
        assertDoesNotThrow(() -> CobolSql.exec(sqlca, "SELECT 1"));
        assertEquals(SqlCA.ECPG_NO_CONN, sqlCode(sqlca), "EXEC should report ECPG_NO_CONN");
        assertEquals("08003", sqlState(sqlca), "EXEC should set SQLSTATE 08003");

        assertDoesNotThrow(() -> CobolSql.commit(sqlca));
        assertEquals(SqlCA.ECPG_NO_CONN, sqlCode(sqlca), "COMMIT should report ECPG_NO_CONN");

        assertDoesNotThrow(() -> CobolSql.openCursor(sqlca, "c1"));
        assertEquals(SqlCA.ECPG_NO_CONN, sqlCode(sqlca), "OPEN CURSOR should report ECPG_NO_CONN");
    }
}

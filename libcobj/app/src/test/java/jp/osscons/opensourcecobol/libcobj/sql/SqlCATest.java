package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.nio.ByteBuffer;
import java.sql.SQLException;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class SqlCATest {

    private static final int SQLCA_SIZE = 133;
    private CobolDataStorage sqlca;

    @BeforeEach
    void setUp() {
        sqlca = new CobolDataStorage(SQLCA_SIZE);
    }

    // ---------- setCode / getCode ----------

    @Test
    void testSetAndGetCode_Zero() {
        SqlCA.setCode(sqlca, 0);
        assertEquals(0, SqlCA.getCode(sqlca), "SQLCODE should be 0 after setting 0");
    }

    @Test
    void testSetAndGetCode_NotFound() {
        SqlCA.setCode(sqlca, 100);
        assertEquals(100, SqlCA.getCode(sqlca), "SQLCODE should be 100 after setting 100");
    }

    @Test
    void testSetAndGetCode_Negative() {
        SqlCA.setCode(sqlca, -400);
        assertEquals(-400, SqlCA.getCode(sqlca), "SQLCODE should be -400 after setting -400");
    }

    @Test
    void testSetAndGetCode_LargeNegative() {
        SqlCA.setCode(sqlca, -9999);
        assertEquals(-9999, SqlCA.getCode(sqlca), "SQLCODE should be -9999 after setting -9999");
    }

    @Test
    void testSetCode_NullSqlca() {
        assertDoesNotThrow(
                () -> SqlCA.setCode(null, 100), "setCode with null sqlca should not throw");
    }

    @Test
    void testGetCode_NullSqlca() {
        assertEquals(0, SqlCA.getCode(null), "getCode with null sqlca should return 0");
    }

    // ---------- setState ----------

    @Test
    void testSetState_00000() {
        SqlCA.setState(sqlca, "00000");
        byte[] stateBytes = sqlca.getByteArray(128, 5);
        assertEquals("00000", new String(stateBytes), "SQLSTATE should be 00000");
    }

    @Test
    void testSetState_02000() {
        SqlCA.setState(sqlca, "02000");
        byte[] stateBytes = sqlca.getByteArray(128, 5);
        assertEquals("02000", new String(stateBytes), "SQLSTATE should be 02000");
    }

    @Test
    void testSetState_08001() {
        SqlCA.setState(sqlca, "08001");
        byte[] stateBytes = sqlca.getByteArray(128, 5);
        assertEquals("08001", new String(stateBytes), "SQLSTATE should be 08001");
    }

    @Test
    void testSetState_NullSqlca() {
        assertDoesNotThrow(
                () -> SqlCA.setState(null, "00000"), "setState with null sqlca should not throw");
    }

    @Test
    void testSetState_NullState() {
        assertDoesNotThrow(
                () -> SqlCA.setState(sqlca, null), "setState with null state should not throw");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetState_ShortString() {
        SqlCA.setState(sqlca, "AB");
        byte[] stateBytes = sqlca.getByteArray(128, 5);
        assertEquals('A', (char) stateBytes[0], "First char should be A");
        assertEquals('B', (char) stateBytes[1], "Second char should be B");
        assertEquals(' ', (char) stateBytes[2], "Third char should be space-padded");
        assertEquals(' ', (char) stateBytes[3], "Fourth char should be space-padded");
        assertEquals(' ', (char) stateBytes[4], "Fifth char should be space-padded");
    }

    // ---------- setErrmc / clearErrmc ----------

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetErrmc_ShortMessage() {
        SqlCA.setErrmc(sqlca, "hello");
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(5, len, "ERRMC length should be 5");
        byte[] msg = sqlca.getByteArray(18, 5);
        assertEquals("hello", new String(msg), "ERRMC message should be hello");
    }

    @Test
    void testSetErrmc_Exactly70Chars() {
        String msg70 = "A".repeat(70);
        SqlCA.setErrmc(sqlca, msg70);
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(70, len, "ERRMC length should be 70 for exactly 70 chars");
    }

    @Test
    void testSetErrmc_TruncatesOver70Chars() {
        String msg80 = "B".repeat(80);
        SqlCA.setErrmc(sqlca, msg80);
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(70, len, "ERRMC length should be truncated to 70");
    }

    @Test
    void testSetErrmc_NullMessage_CallsClearErrmc() {
        SqlCA.setErrmc(sqlca, "test");
        SqlCA.setErrmc(sqlca, null);
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(0, len, "ERRMC length should be 0 after setting null message");
    }

    @Test
    void testSetErrmc_EmptyMessage() {
        SqlCA.setErrmc(sqlca, "");
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(0, len, "ERRMC length should be 0 for empty message");
    }

    @Test
    void testSetErrmc_NullSqlca() {
        assertDoesNotThrow(
                () -> SqlCA.setErrmc(null, "test"), "setErrmc with null sqlca should not throw");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testClearErrmc() {
        SqlCA.setErrmc(sqlca, "some error");
        SqlCA.clearErrmc(sqlca);
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(0, len, "ERRMC length should be 0 after clear");
        for (int i = 0; i < 70; i++) {
            assertEquals(0, sqlca.getByte(18 + i), "ERRMC byte at offset " + i + " should be 0");
        }
    }

    @Test
    void testClearErrmc_NullSqlca() {
        assertDoesNotThrow(
                () -> SqlCA.clearErrmc(null), "clearErrmc with null sqlca should not throw");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetSuccessKeepErrmc() {
        // SQLERRMC のバイト列は保持しつつ、SQLCODE=0 / SQLSTATE=00000 / SQLERRML=0 にする。
        SqlCA.setErrmc(sqlca, "stale");
        SqlCA.setSuccessKeepErrmc(sqlca);
        assertEquals(SqlCA.ECPG_NO_ERROR, SqlCA.getCode(sqlca), "SQLCODE should be 0");
        byte[] stateBytes = sqlca.getByteArray(128, 5);
        assertEquals("00000", new String(stateBytes), "SQLSTATE should be 00000");
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(0, len, "SQLERRML should be 0");
        byte[] msg = sqlca.getByteArray(18, 5);
        assertEquals("stale", new String(msg), "SQLERRMC bytes should be left unchanged");
    }

    // ---------- setErrd ----------

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetErrd_AllIndices() {
        for (int i = 0; i < 6; i++) {
            SqlCA.setErrd(sqlca, i, (i + 1) * 100);
        }
        for (int i = 0; i < 6; i++) {
            int val = ByteBuffer.wrap(sqlca.getByteArray(96 + i * 4, 4)).getInt();
            assertEquals((i + 1) * 100, val, "ERRD[" + i + "] should be " + ((i + 1) * 100));
        }
    }

    @Test
    void testSetErrd_NegativeIndex() {
        assertDoesNotThrow(
                () -> SqlCA.setErrd(sqlca, -1, 42), "setErrd with negative index should not throw");
    }

    @Test
    void testSetErrd_IndexOutOfRange() {
        assertDoesNotThrow(
                () -> SqlCA.setErrd(sqlca, 6, 42),
                "setErrd with out-of-range index should not throw");
    }

    @Test
    void testSetErrd_NullSqlca() {
        assertDoesNotThrow(
                () -> SqlCA.setErrd(null, 0, 42), "setErrd with null sqlca should not throw");
    }

    // ---------- setSuccess ----------

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetSuccess() {
        SqlCA.setError(sqlca, -400, "08001", "connection error");
        SqlCA.setSuccess(sqlca);
        assertEquals(0, SqlCA.getCode(sqlca), "SQLCODE should be 0 after setSuccess");
        byte[] state = sqlca.getByteArray(128, 5);
        assertEquals("00000", new String(state), "SQLSTATE should be 00000 after setSuccess");
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(0, len, "ERRMC length should be 0 after setSuccess");
    }

    @Test
    void testSetSuccess_NullSqlca() {
        assertDoesNotThrow(
                () -> SqlCA.setSuccess(null), "setSuccess with null sqlca should not throw");
    }

    // ---------- setError ----------

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetError() {
        SqlCA.setError(sqlca, -402, "08001", "conn refused");
        assertEquals(-402, SqlCA.getCode(sqlca), "SQLCODE should be -402");
        byte[] state = sqlca.getByteArray(128, 5);
        assertEquals("08001", new String(state), "SQLSTATE should be 08001");
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(12, len, "ERRMC length should be 12");
    }

    @Test
    void testSetError_NullSqlca() {
        assertDoesNotThrow(
                () -> SqlCA.setError(null, -402, "08001", "test"),
                "setError with null sqlca should not throw");
    }

    // ---------- sqlStateToCode ----------

    @Test
    void testSqlStateToCode_NoError() {
        assertEquals(
                SqlCA.ECPG_NO_ERROR,
                SqlCA.sqlStateToCode("00000"),
                "00000 should map to ECPG_NO_ERROR");
    }

    @Test
    void testSqlStateToCode_NotFound() {
        assertEquals(
                SqlCA.ECPG_NOT_FOUND,
                SqlCA.sqlStateToCode("02000"),
                "02000 should map to ECPG_NOT_FOUND");
    }

    @Test
    void testSqlStateToCode_Empty() {
        assertEquals(
                SqlCA.ECPG_EMPTY, SqlCA.sqlStateToCode("YE002"), "YE002 should map to ECPG_EMPTY");
    }

    @Test
    void testSqlStateToCode_Connect_08001() {
        assertEquals(
                SqlCA.ECPG_CONNECT,
                SqlCA.sqlStateToCode("08001"),
                "08001 should map to ECPG_CONNECT");
    }

    @Test
    void testSqlStateToCode_Connect_08003() {
        assertEquals(
                SqlCA.ECPG_CONNECT,
                SqlCA.sqlStateToCode("08003"),
                "08003 should map to ECPG_CONNECT");
    }

    @Test
    void testSqlStateToCode_AuthFailure_28P01_Unknown() {
        // 認証失敗 (28P01/28000) は Open COBOL ESQL 4J と同じく明示マッピングを持たず、
        // 不明エラー (-9999) に落ちる。
        assertEquals(
                SqlCA.ECPG_UNKNOWN_ERROR,
                SqlCA.sqlStateToCode("28P01"),
                "28P01 has no explicit mapping (matches Open COBOL ESQL 4J)");
    }

    @Test
    void testSqlStateToCode_AuthFailure_28000_Unknown() {
        assertEquals(
                SqlCA.ECPG_UNKNOWN_ERROR,
                SqlCA.sqlStateToCode("28000"),
                "28000 has no explicit mapping (matches Open COBOL ESQL 4J)");
    }

    @Test
    void testSqlStateToCode_Trans() {
        assertEquals(
                SqlCA.ECPG_TRANS, SqlCA.sqlStateToCode("08007"), "08007 should map to ECPG_TRANS");
    }

    @Test
    void testSqlStateToCode_SubselectNotOne() {
        assertEquals(
                SqlCA.ECPG_SUBSELECT_NOT_ONE,
                SqlCA.sqlStateToCode("21000"),
                "21000 should map to ECPG_SUBSELECT_NOT_ONE");
    }

    @Test
    void testSqlStateToCode_DuplicateKey() {
        assertEquals(
                SqlCA.ECPG_DUPLICATE_KEY,
                SqlCA.sqlStateToCode("23505"),
                "23505 should map to ECPG_DUPLICATE_KEY");
    }

    @Test
    void testSqlStateToCode_InTransaction() {
        assertEquals(
                SqlCA.ECPG_WARNING_IN_TRANSACTION,
                SqlCA.sqlStateToCode("25001"),
                "25001 should map to ECPG_WARNING_IN_TRANSACTION");
    }

    @Test
    void testSqlStateToCode_NoTransaction() {
        assertEquals(
                SqlCA.ECPG_WARNING_NO_TRANSACTION,
                SqlCA.sqlStateToCode("25P01"),
                "25P01 should map to ECPG_WARNING_NO_TRANSACTION");
    }

    @Test
    void testSqlStateToCode_UnknownPortal() {
        assertEquals(
                SqlCA.ECPG_WARNING_UNKNOWN_PORTAL,
                SqlCA.sqlStateToCode("34000"),
                "34000 should map to ECPG_WARNING_UNKNOWN_PORTAL");
    }

    @Test
    void testSqlStateToCode_DataFormatError() {
        assertEquals(
                SqlCA.ECPG_DATA_FORMAT_ERROR,
                SqlCA.sqlStateToCode("42804"),
                "42804 should map to ECPG_DATA_FORMAT_ERROR");
    }

    @Test
    void testSqlStateToCode_PortalExists() {
        assertEquals(
                SqlCA.ECPG_WARNING_PORTAL_EXISTS,
                SqlCA.sqlStateToCode("42P03"),
                "42P03 should map to ECPG_WARNING_PORTAL_EXISTS");
    }

    @Test
    void testSqlStateToCode_Pgsql() {
        assertEquals(
                SqlCA.ECPG_PGSQL, SqlCA.sqlStateToCode("55P03"), "55P03 should map to ECPG_PGSQL");
    }

    @Test
    void testSqlStateToCode_Unknown() {
        assertEquals(
                SqlCA.ECPG_UNKNOWN_ERROR,
                SqlCA.sqlStateToCode("XXXXX"),
                "Unknown state should map to ECPG_UNKNOWN_ERROR");
    }

    @Test
    void testSqlStateToCode_Null() {
        assertEquals(
                SqlCA.ECPG_UNKNOWN_ERROR,
                SqlCA.sqlStateToCode(null),
                "Null state should map to ECPG_UNKNOWN_ERROR");
    }

    // ---------- setResultFromException ----------

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetResultFromException() {
        SQLException e = new SQLException("connection refused", "08001");
        SqlCA.setResultFromException(sqlca, e);
        assertEquals(
                SqlCA.ECPG_CONNECT,
                SqlCA.getCode(sqlca),
                "SQLCODE should be ECPG_CONNECT for 08001");
        byte[] state = sqlca.getByteArray(128, 5);
        assertEquals("08001", new String(state), "SQLSTATE should be 08001");
    }

    @Test
    void testSetResultFromException_NullSqlState() {
        SQLException e = new SQLException("some error", (String) null);
        SqlCA.setResultFromException(sqlca, e);
        byte[] state = sqlca.getByteArray(128, 5);
        assertEquals(
                "     ",
                new String(state),
                "SQLSTATE should be spaces when exception has null state");
    }

    @Test
    void testSetResultFromException_NullMessage() {
        SQLException e = new SQLException(null, "00000");
        SqlCA.setResultFromException(sqlca, e);
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(0, len, "ERRMC length should be 0 for null message");
    }

    @Test
    void testSetResultFromException_NullSqlca() {
        SQLException e = new SQLException("test", "00000");
        assertDoesNotThrow(
                () -> SqlCA.setResultFromException(null, e),
                "setResultFromException with null sqlca should not throw");
    }
}

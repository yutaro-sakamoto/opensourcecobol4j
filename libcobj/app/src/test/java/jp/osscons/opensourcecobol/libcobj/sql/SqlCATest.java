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
        assertEquals(0, SqlCA.getCode(sqlca));
    }

    @Test
    void testSetAndGetCode_NotFound() {
        SqlCA.setCode(sqlca, 100);
        assertEquals(100, SqlCA.getCode(sqlca));
    }

    @Test
    void testSetAndGetCode_Negative() {
        SqlCA.setCode(sqlca, -400);
        assertEquals(-400, SqlCA.getCode(sqlca));
    }

    @Test
    void testSetAndGetCode_LargeNegative() {
        SqlCA.setCode(sqlca, -9999);
        assertEquals(-9999, SqlCA.getCode(sqlca));
    }

    @Test
    void testSetCode_NullSqlca() {
        assertDoesNotThrow(() -> SqlCA.setCode(null, 100));
    }

    @Test
    void testGetCode_NullSqlca() {
        assertEquals(0, SqlCA.getCode(null));
    }

    // ---------- setState ----------

    @Test
    void testSetState_00000() {
        SqlCA.setState(sqlca, "00000");
        byte[] stateBytes = sqlca.getByteArray(128, 5);
        assertEquals("00000", new String(stateBytes));
    }

    @Test
    void testSetState_02000() {
        SqlCA.setState(sqlca, "02000");
        byte[] stateBytes = sqlca.getByteArray(128, 5);
        assertEquals("02000", new String(stateBytes));
    }

    @Test
    void testSetState_08001() {
        SqlCA.setState(sqlca, "08001");
        byte[] stateBytes = sqlca.getByteArray(128, 5);
        assertEquals("08001", new String(stateBytes));
    }

    @Test
    void testSetState_NullSqlca() {
        assertDoesNotThrow(() -> SqlCA.setState(null, "00000"));
    }

    @Test
    void testSetState_NullState() {
        assertDoesNotThrow(() -> SqlCA.setState(sqlca, null));
    }

    @Test
    void testSetState_ShortString() {
        SqlCA.setState(sqlca, "AB");
        byte[] stateBytes = sqlca.getByteArray(128, 5);
        assertEquals('A', (char) stateBytes[0]);
        assertEquals('B', (char) stateBytes[1]);
        assertEquals(' ', (char) stateBytes[2]);
        assertEquals(' ', (char) stateBytes[3]);
        assertEquals(' ', (char) stateBytes[4]);
    }

    // ---------- setErrmc / clearErrmc ----------

    @Test
    void testSetErrmc_ShortMessage() {
        SqlCA.setErrmc(sqlca, "hello");
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(5, len);
        byte[] msg = sqlca.getByteArray(18, 5);
        assertEquals("hello", new String(msg));
    }

    @Test
    void testSetErrmc_Exactly70Chars() {
        String msg70 = "A".repeat(70);
        SqlCA.setErrmc(sqlca, msg70);
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(70, len);
    }

    @Test
    void testSetErrmc_TruncatesOver70Chars() {
        String msg80 = "B".repeat(80);
        SqlCA.setErrmc(sqlca, msg80);
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(70, len);
    }

    @Test
    void testSetErrmc_NullMessage_CallsClearErrmc() {
        SqlCA.setErrmc(sqlca, "test");
        SqlCA.setErrmc(sqlca, null);
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(0, len);
    }

    @Test
    void testSetErrmc_EmptyMessage() {
        SqlCA.setErrmc(sqlca, "");
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(0, len);
    }

    @Test
    void testSetErrmc_NullSqlca() {
        assertDoesNotThrow(() -> SqlCA.setErrmc(null, "test"));
    }

    @Test
    void testClearErrmc() {
        SqlCA.setErrmc(sqlca, "some error");
        SqlCA.clearErrmc(sqlca);
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(0, len);
        for (int i = 0; i < 70; i++) {
            assertEquals(0, sqlca.getByte(18 + i));
        }
    }

    @Test
    void testClearErrmc_NullSqlca() {
        assertDoesNotThrow(() -> SqlCA.clearErrmc(null));
    }

    // ---------- setErrd ----------

    @Test
    void testSetErrd_AllIndices() {
        for (int i = 0; i < 6; i++) {
            SqlCA.setErrd(sqlca, i, (i + 1) * 100);
        }
        for (int i = 0; i < 6; i++) {
            int val = ByteBuffer.wrap(sqlca.getByteArray(96 + i * 4, 4)).getInt();
            assertEquals((i + 1) * 100, val);
        }
    }

    @Test
    void testSetErrd_NegativeIndex() {
        assertDoesNotThrow(() -> SqlCA.setErrd(sqlca, -1, 42));
    }

    @Test
    void testSetErrd_IndexOutOfRange() {
        assertDoesNotThrow(() -> SqlCA.setErrd(sqlca, 6, 42));
    }

    @Test
    void testSetErrd_NullSqlca() {
        assertDoesNotThrow(() -> SqlCA.setErrd(null, 0, 42));
    }

    // ---------- setSuccess ----------

    @Test
    void testSetSuccess() {
        SqlCA.setError(sqlca, -400, "08001", "connection error");
        SqlCA.setSuccess(sqlca);
        assertEquals(0, SqlCA.getCode(sqlca));
        byte[] state = sqlca.getByteArray(128, 5);
        assertEquals("00000", new String(state));
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(0, len);
    }

    @Test
    void testSetSuccess_NullSqlca() {
        assertDoesNotThrow(() -> SqlCA.setSuccess(null));
    }

    // ---------- setError ----------

    @Test
    void testSetError() {
        SqlCA.setError(sqlca, -402, "08001", "conn refused");
        assertEquals(-402, SqlCA.getCode(sqlca));
        byte[] state = sqlca.getByteArray(128, 5);
        assertEquals("08001", new String(state));
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(12, len);
    }

    @Test
    void testSetError_NullSqlca() {
        assertDoesNotThrow(() -> SqlCA.setError(null, -402, "08001", "test"));
    }

    // ---------- sqlStateToCode ----------

    @Test
    void testSqlStateToCode_NoError() {
        assertEquals(SqlCA.ECPG_NO_ERROR, SqlCA.sqlStateToCode("00000"));
    }

    @Test
    void testSqlStateToCode_NotFound() {
        assertEquals(SqlCA.ECPG_NOT_FOUND, SqlCA.sqlStateToCode("02000"));
    }

    @Test
    void testSqlStateToCode_Empty() {
        assertEquals(SqlCA.ECPG_EMPTY, SqlCA.sqlStateToCode("YE002"));
    }

    @Test
    void testSqlStateToCode_Connect_08001() {
        assertEquals(SqlCA.ECPG_CONNECT, SqlCA.sqlStateToCode("08001"));
    }

    @Test
    void testSqlStateToCode_Connect_08003() {
        assertEquals(SqlCA.ECPG_CONNECT, SqlCA.sqlStateToCode("08003"));
    }

    @Test
    void testSqlStateToCode_Connect_28P01() {
        assertEquals(SqlCA.ECPG_CONNECT, SqlCA.sqlStateToCode("28P01"));
    }

    @Test
    void testSqlStateToCode_Connect_28000() {
        assertEquals(SqlCA.ECPG_CONNECT, SqlCA.sqlStateToCode("28000"));
    }

    @Test
    void testSqlStateToCode_Trans() {
        assertEquals(SqlCA.ECPG_TRANS, SqlCA.sqlStateToCode("08007"));
    }

    @Test
    void testSqlStateToCode_SubselectNotOne() {
        assertEquals(SqlCA.ECPG_SUBSELECT_NOT_ONE, SqlCA.sqlStateToCode("21000"));
    }

    @Test
    void testSqlStateToCode_DuplicateKey() {
        assertEquals(SqlCA.ECPG_DUPLICATE_KEY, SqlCA.sqlStateToCode("23505"));
    }

    @Test
    void testSqlStateToCode_InTransaction() {
        assertEquals(SqlCA.ECPG_WARNING_IN_TRANSACTION, SqlCA.sqlStateToCode("25001"));
    }

    @Test
    void testSqlStateToCode_NoTransaction() {
        assertEquals(SqlCA.ECPG_WARNING_NO_TRANSACTION, SqlCA.sqlStateToCode("25P01"));
    }

    @Test
    void testSqlStateToCode_UnknownPortal() {
        assertEquals(SqlCA.ECPG_WARNING_UNKNOWN_PORTAL, SqlCA.sqlStateToCode("34000"));
    }

    @Test
    void testSqlStateToCode_DataFormatError() {
        assertEquals(SqlCA.ECPG_DATA_FORMAT_ERROR, SqlCA.sqlStateToCode("42804"));
    }

    @Test
    void testSqlStateToCode_PortalExists() {
        assertEquals(SqlCA.ECPG_WARNING_PORTAL_EXISTS, SqlCA.sqlStateToCode("42P03"));
    }

    @Test
    void testSqlStateToCode_Pgsql() {
        assertEquals(SqlCA.ECPG_PGSQL, SqlCA.sqlStateToCode("55P03"));
    }

    @Test
    void testSqlStateToCode_Unknown() {
        assertEquals(SqlCA.ECPG_UNKNOWN_ERROR, SqlCA.sqlStateToCode("XXXXX"));
    }

    @Test
    void testSqlStateToCode_Null() {
        assertEquals(SqlCA.ECPG_UNKNOWN_ERROR, SqlCA.sqlStateToCode(null));
    }

    // ---------- setResultFromException ----------

    @Test
    void testSetResultFromException() {
        SQLException e = new SQLException("connection refused", "08001");
        SqlCA.setResultFromException(sqlca, e);
        assertEquals(SqlCA.ECPG_CONNECT, SqlCA.getCode(sqlca));
        byte[] state = sqlca.getByteArray(128, 5);
        assertEquals("08001", new String(state));
    }

    @Test
    void testSetResultFromException_NullSqlState() {
        SQLException e = new SQLException("some error", (String) null);
        SqlCA.setResultFromException(sqlca, e);
        byte[] state = sqlca.getByteArray(128, 5);
        assertEquals("     ", new String(state));
    }

    @Test
    void testSetResultFromException_NullMessage() {
        SQLException e = new SQLException(null, "00000");
        SqlCA.setResultFromException(sqlca, e);
        short len = ByteBuffer.wrap(sqlca.getByteArray(16, 2)).getShort();
        assertEquals(0, len);
    }

    @Test
    void testSetResultFromException_NullSqlca() {
        SQLException e = new SQLException("test", "00000");
        assertDoesNotThrow(() -> SqlCA.setResultFromException(null, e));
    }
}

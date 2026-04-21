package jp.osscons.opensourcecobol.libcobj.sql;

import java.nio.ByteBuffer;
import java.sql.SQLException;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

public class SqlCA {

    public static final int SQLERRMC_LEN = 70;

    // Offsets within the SQLCA structure (total 133 bytes)
    private static final int OFFSET_SQLCAID = 0; // 8 bytes
    private static final int OFFSET_SQLCABC = 8; // 4 bytes (int)
    private static final int OFFSET_SQLCODE = 12; // 4 bytes (int)
    private static final int OFFSET_SQLERRML = 16; // 2 bytes (short)
    private static final int OFFSET_SQLERRMC = 18; // 70 bytes
    private static final int OFFSET_SQLERRP = 88; // 8 bytes
    private static final int OFFSET_SQLERRD = 96; // 24 bytes (6 ints)
    private static final int OFFSET_SQLWARN = 120; // 8 bytes
    private static final int OFFSET_SQLSTATE = 128; // 5 bytes

    // Error code constants (from ConstValues.scala)
    public static final int ECPG_NO_ERROR = 0;
    public static final int ECPG_NOT_FOUND = 100;
    public static final int ECPG_OUT_OF_MEMORY = -12;
    public static final int ECPG_UNSUPPORTED = -200;
    public static final int ECPG_TOO_MANY_ARGUMENTS = -201;
    public static final int ECPG_TOO_FEW_ARGUMENTS = -202;
    public static final int ECPG_TOO_MANY_MATCHES = -203;
    public static final int ECPG_DATA_FORMAT_ERROR = -204;
    public static final int ECPG_EMPTY = -212;
    public static final int ECPG_MISSING_INDICATOR = -213;
    public static final int ECPG_NO_CONN = -220;
    public static final int ECPG_NOT_CONN = -221;
    public static final int ECPG_INVALID_STMT = -230;
    public static final int ECPG_INFORMIX_DUPLICATE_KEY = -239;
    public static final int ECPG_UNKNOWN_DESCRIPTOR = -240;
    public static final int ECPG_INVALID_DESCRIPTOR_INDEX = -241;
    public static final int ECPG_UNKNOWN_DESCRIPTOR_ITEM = -242;
    public static final int ECPG_VAR_NOT_NUMERIC = -243;
    public static final int ECPG_VAR_NOT_CHAR = -244;
    public static final int ECPG_INFORMIX_SUBSELECT_NOT_ONE = -284;
    public static final int ECPG_PGSQL = -400;
    public static final int ECPG_TRANS = -401;
    public static final int ECPG_CONNECT = -402;
    public static final int ECPG_DUPLICATE_KEY = -403;
    public static final int ECPG_SUBSELECT_NOT_ONE = -404;
    public static final int ECPG_WARNING_UNKNOWN_PORTAL = -602;
    public static final int ECPG_WARNING_IN_TRANSACTION = -603;
    public static final int ECPG_WARNING_NO_TRANSACTION = -604;
    public static final int ECPG_WARNING_PORTAL_EXISTS = -605;
    public static final int ECPG_LOCK_ERROR = -606;
    public static final int ECPG_JDD_ERROR = -607;
    public static final int ECPG_UNKNOWN_ERROR = -9999;

    public static void setCode(CobolDataStorage sqlca, int code) {
        if (sqlca == null) return;
        sqlca.getSubDataStorage(OFFSET_SQLCODE).set(code);
    }

    public static int getCode(CobolDataStorage sqlca) {
        if (sqlca == null) return 0;
        return ByteBuffer.wrap(sqlca.getByteArray(OFFSET_SQLCODE, 4)).getInt();
    }

    public static void setState(CobolDataStorage sqlca, String state) {
        if (sqlca == null || state == null) return;
        byte[] stateBytes = state.getBytes();
        for (int i = 0; i < 5; i++) {
            if (i < stateBytes.length) {
                sqlca.setByte(OFFSET_SQLSTATE + i, stateBytes[i]);
            } else {
                sqlca.setByte(OFFSET_SQLSTATE + i, (byte) ' ');
            }
        }
    }

    public static void setErrmc(CobolDataStorage sqlca, String message) {
        if (sqlca == null) return;
        if (message == null) {
            clearErrmc(sqlca);
            return;
        }
        byte[] msgBytes = message.getBytes();
        int len = Math.min(msgBytes.length, SQLERRMC_LEN);
        // Set SQLERRML
        sqlca.getSubDataStorage(OFFSET_SQLERRML).set((short) len);
        // Set SQLERRMC
        for (int i = 0; i < SQLERRMC_LEN; i++) {
            if (i < len) {
                sqlca.setByte(OFFSET_SQLERRMC + i, msgBytes[i]);
            } else {
                sqlca.setByte(OFFSET_SQLERRMC + i, (byte) 0);
            }
        }
    }

    public static void setErrd(CobolDataStorage sqlca, int index, int value) {
        if (sqlca == null || index < 0 || index >= 6) return;
        sqlca.getSubDataStorage(OFFSET_SQLERRD + index * 4).set(value);
    }

    public static void clearErrmc(CobolDataStorage sqlca) {
        if (sqlca == null) return;
        sqlca.getSubDataStorage(OFFSET_SQLERRML).set((short) 0);
        for (int i = 0; i < SQLERRMC_LEN; i++) {
            sqlca.setByte(OFFSET_SQLERRMC + i, (byte) 0);
        }
    }

    public static void setSuccess(CobolDataStorage sqlca) {
        if (sqlca == null) return;
        setCode(sqlca, ECPG_NO_ERROR);
        setState(sqlca, "00000");
        clearErrmc(sqlca);
    }

    public static void setError(CobolDataStorage sqlca, int code, String state, String message) {
        if (sqlca == null) return;
        setCode(sqlca, code);
        setState(sqlca, state);
        setErrmc(sqlca, message);
    }

    public static void setResultFromException(CobolDataStorage sqlca, SQLException e) {
        if (sqlca == null) return;
        String sqlState = e.getSQLState();
        if (sqlState == null) {
            sqlState = "     ";
        }
        int code = sqlStateToCode(sqlState);
        String message = e.getMessage();
        if (message == null) {
            message = "";
        }
        setError(sqlca, code, sqlState, message);
    }

    public static int sqlStateToCode(String sqlState) {
        if (sqlState == null) {
            return ECPG_UNKNOWN_ERROR;
        }
        switch (sqlState) {
            case "00000":
                return ECPG_NO_ERROR;
            case "02000":
                return ECPG_NOT_FOUND;
            case "YE002":
                return ECPG_EMPTY;
            case "08001":
            case "08003":
                return ECPG_CONNECT;
            case "08007":
                return ECPG_TRANS;
            case "21000":
                return ECPG_SUBSELECT_NOT_ONE;
            case "23505":
                return ECPG_DUPLICATE_KEY;
            case "25001":
                return ECPG_WARNING_IN_TRANSACTION;
            case "25P01":
                return ECPG_WARNING_NO_TRANSACTION;
            case "34000":
                return ECPG_WARNING_UNKNOWN_PORTAL;
            case "42804":
                return ECPG_DATA_FORMAT_ERROR;
            case "42P03":
                return ECPG_WARNING_PORTAL_EXISTS;
            case "55P03":
                return ECPG_PGSQL;
            default:
                return ECPG_UNKNOWN_ERROR;
        }
    }
}

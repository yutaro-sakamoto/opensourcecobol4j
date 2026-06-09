package jp.osscons.opensourcecobol.libcobj.sql;

import java.nio.ByteBuffer;
import java.sql.SQLException;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/** Manages the SQLCA (SQL Communication Area) structure for COBOL embedded SQL. */
final class SqlCA {

    /** Private constructor to prevent instantiation of utility class. */
    private SqlCA() {}

    /** Maximum length of the SQLERRMC error message field. */
    static final int SQLERRMC_LEN = 70;

    // Offsets within the SQLCA structure (total 133 bytes)
    // OFFSET_SQLCAID (0), OFFSET_SQLCABC (8), OFFSET_SQLERRP (88), OFFSET_SQLWARN (120)
    // are defined in the SQLCA spec but not currently used in code.
    private static final int OFFSET_SQLCODE = 12; // 4 bytes (int)
    private static final int OFFSET_SQLERRML = 16; // 2 bytes (short)
    private static final int OFFSET_SQLERRMC = 18; // 70 bytes
    private static final int OFFSET_SQLERRD = 96; // 24 bytes (6 ints)
    private static final int OFFSET_SQLSTATE = 128; // 5 bytes

    /** No error. */
    static final int ECPG_NO_ERROR = 0;

    /** Row not found (SQLSTATE 02000). */
    static final int ECPG_NOT_FOUND = 100;

    /** Out of memory. */
    static final int ECPG_OUT_OF_MEMORY = -12;

    /** Unsupported feature. */
    static final int ECPG_UNSUPPORTED = -200;

    /** Too many host variable arguments. */
    static final int ECPG_TOO_MANY_ARGUMENTS = -201;

    /** Too few host variable arguments. */
    static final int ECPG_TOO_FEW_ARGUMENTS = -202;

    /** Too many matching rows. */
    static final int ECPG_TOO_MANY_MATCHES = -203;

    /** Data format error. */
    static final int ECPG_DATA_FORMAT_ERROR = -204;

    /** Empty query or statement. */
    static final int ECPG_EMPTY = -212;

    /** Missing indicator variable. */
    static final int ECPG_MISSING_INDICATOR = -213;

    /** No active connection. */
    static final int ECPG_NO_CONN = -220;

    /** Not connected. */
    static final int ECPG_NOT_CONN = -221;

    /** Invalid prepared statement. */
    static final int ECPG_INVALID_STMT = -230;

    /** Informix-compatible duplicate key error. */
    static final int ECPG_INFORMIX_DUPLICATE_KEY = -239;

    /** Unknown descriptor. */
    static final int ECPG_UNKNOWN_DESCRIPTOR = -240;

    /** Invalid descriptor index. */
    static final int ECPG_INVALID_DESCRIPTOR_INDEX = -241;

    /** Unknown descriptor item. */
    static final int ECPG_UNKNOWN_DESCRIPTOR_ITEM = -242;

    /** Variable is not numeric. */
    static final int ECPG_VAR_NOT_NUMERIC = -243;

    /** Variable is not character type. */
    static final int ECPG_VAR_NOT_CHAR = -244;

    /** Informix-compatible subselect returned more than one row. */
    static final int ECPG_INFORMIX_SUBSELECT_NOT_ONE = -284;

    /** PostgreSQL backend error. */
    static final int ECPG_PGSQL = -400;

    /** Transaction error. */
    static final int ECPG_TRANS = -401;

    /** Connection error. */
    static final int ECPG_CONNECT = -402;

    /** Duplicate key violation. */
    static final int ECPG_DUPLICATE_KEY = -403;

    /** Subselect returned more than one row. */
    static final int ECPG_SUBSELECT_NOT_ONE = -404;

    /** Unknown cursor (portal). */
    static final int ECPG_WARNING_UNKNOWN_PORTAL = -602;

    /** Already in a transaction. */
    static final int ECPG_WARNING_IN_TRANSACTION = -603;

    /** No active transaction. */
    static final int ECPG_WARNING_NO_TRANSACTION = -604;

    /** Cursor (portal) already exists. */
    static final int ECPG_WARNING_PORTAL_EXISTS = -605;

    /** Lock error. */
    static final int ECPG_LOCK_ERROR = -606;

    /** JDD (Java Database Driver) error. */
    static final int ECPG_JDD_ERROR = -607;

    /** Unrecognized error. */
    static final int ECPG_UNKNOWN_ERROR = -9999;

    /**
     * Set the SQLCODE field in the SQLCA structure.
     *
     * @param sqlca the SQLCA data storage
     * @param code the SQLCODE value to set
     */
    static void setCode(CobolDataStorage sqlca, int code) {
        if (sqlca == null) {
            return;
        }
        sqlca.getSubDataStorage(OFFSET_SQLCODE).set(code);
    }

    /**
     * Get the SQLCODE field from the SQLCA structure.
     *
     * @param sqlca the SQLCA data storage
     * @return the current SQLCODE value, or 0 if sqlca is null
     */
    static int getCode(CobolDataStorage sqlca) {
        if (sqlca == null) {
            return 0;
        }
        return ByteBuffer.wrap(
                        sqlca.getByteArrayRef(OFFSET_SQLCODE, 4),
                        sqlca.getIndex() + OFFSET_SQLCODE,
                        4)
                .getInt();
    }

    /**
     * Set the 5-character SQLSTATE field in the SQLCA structure.
     *
     * @param sqlca the SQLCA data storage
     * @param state the SQLSTATE string (e.g. "00000")
     */
    static void setState(CobolDataStorage sqlca, String state) {
        if (sqlca == null || state == null) {
            return;
        }
        byte[] stateBytes = state.getBytes();
        for (int i = 0; i < 5; i++) {
            if (i < stateBytes.length) {
                sqlca.setByte(OFFSET_SQLSTATE + i, stateBytes[i]);
            } else {
                sqlca.setByte(OFFSET_SQLSTATE + i, (byte) ' ');
            }
        }
    }

    /**
     * Set the SQLERRMC error message field in the SQLCA structure.
     *
     * @param sqlca the SQLCA data storage
     * @param message the error message (truncated to {@link #SQLERRMC_LEN} bytes)
     */
    static void setErrmc(CobolDataStorage sqlca, String message) {
        if (sqlca == null) {
            return;
        }
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

    /**
     * Set one of the six SQLERRD diagnostic values.
     *
     * @param sqlca the SQLCA data storage
     * @param index the SQLERRD index (0-5)
     * @param value the integer value to set
     */
    static void setErrd(CobolDataStorage sqlca, int index, int value) {
        if (sqlca == null || index < 0 || index >= 6) {
            return;
        }
        sqlca.getSubDataStorage(OFFSET_SQLERRD + index * 4).set(value);
    }

    /**
     * Clear the SQLERRMC and SQLERRML fields in the SQLCA structure.
     *
     * @param sqlca the SQLCA data storage
     */
    static void clearErrmc(CobolDataStorage sqlca) {
        if (sqlca == null) {
            return;
        }
        sqlca.getSubDataStorage(OFFSET_SQLERRML).set((short) 0);
        for (int i = 0; i < SQLERRMC_LEN; i++) {
            sqlca.setByte(OFFSET_SQLERRMC + i, (byte) 0);
        }
    }

    /**
     * Set the SQLCA to indicate successful completion (SQLCODE=0, SQLSTATE="00000").
     *
     * @param sqlca the SQLCA data storage
     */
    static void setSuccess(CobolDataStorage sqlca) {
        if (sqlca == null) {
            return;
        }
        setCode(sqlca, ECPG_NO_ERROR);
        setState(sqlca, "00000");
        clearErrmc(sqlca);
    }

    /**
     * Set the SQLCA to ECPG_MISSING_INDICATOR (sqlcode=-213, sqlstate="22002").
     *
     * <p>Signals "NULL value without indicator variable" — ECPG standard behavior when a fetched
     * column is SQL NULL and the host variable has no indicator companion. The COBOL field
     * itself is still written (typically zero-filled) so the row is considered processed;
     * application code is expected to check SQLCODE/SQLSTATE after the FETCH/SELECT.
     *
     * @param sqlca the SQLCA data storage
     */
    static void setMissingIndicator(CobolDataStorage sqlca) {
        setError(sqlca, ECPG_MISSING_INDICATOR, "22002", "Null value without indicator");
    }

    /**
     * Set the SQLCA to indicate an error with the given code, state, and message.
     *
     * @param sqlca the SQLCA data storage
     * @param code the SQLCODE error code
     * @param state the 5-character SQLSTATE
     * @param message the error message
     */
    static void setError(CobolDataStorage sqlca, int code, String state, String message) {
        if (sqlca == null) {
            return;
        }
        setCode(sqlca, code);
        setState(sqlca, state);
        setErrmc(sqlca, message);
    }

    /**
     * Populate the SQLCA from a SQLException, mapping the SQLSTATE to an ECPG error code.
     *
     * @param sqlca the SQLCA data storage
     * @param e the SQL exception
     */
    static void setResultFromException(CobolDataStorage sqlca, SQLException e) {
        if (sqlca == null) {
            return;
        }
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

    /**
     * Map a 5-character SQLSTATE string to the corresponding ECPG error code.
     *
     * @param sqlState the SQLSTATE string
     * @return the ECPG error code constant
     */
    static int sqlStateToCode(String sqlState) {
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
            case "28P01":
            case "28000":
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

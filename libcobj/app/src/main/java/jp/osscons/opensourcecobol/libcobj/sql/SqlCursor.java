package jp.osscons.opensourcecobol.libcobj.sql;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/** Represents a SQL cursor for iterating over query results in COBOL embedded SQL. */
class SqlCursor {

    /** Cursor name used in DECLARE/OPEN/FETCH/CLOSE statements. */
    String name;

    /** SQL query associated with this cursor. */
    String query;

    /** Number of host variable parameters in the query. */
    int nParams;

    /** Whether this cursor is currently open. */
    boolean isOpened;

    /** Host variable parameters bound at DECLARE time. */
    AbstractCobolField[] params;

    /**
     * Create a new cursor descriptor.
     *
     * @param name the cursor name
     * @param query the SQL query for this cursor
     * @param nParams the number of host variable parameters
     */
    SqlCursor(String name, String query, int nParams) {
        this.name = name;
        this.query = query;
        this.nParams = nParams;
        this.isOpened = false;
        this.params = null;
    }

    /**
     * Open this cursor by executing a DECLARE CURSOR statement.
     *
     * @param conn the JDBC connection
     * @param openParams host variable parameters for the query, or null to use stored params
     * @throws SQLException if a database access error occurs
     */
    void open(Connection conn, AbstractCobolField[] openParams) throws SQLException {
        String command = "DECLARE " + name + " CURSOR FOR " + query;

        // OPEN ... USING (openParams) を最優先し、なければ DECLARE 時に
        // 保存した this.params にフォールバック。どちらも空ならパラメータ
        // なしで Statement を実行する。
        AbstractCobolField[] bindParams;
        if (openParams != null && openParams.length > 0) {
            bindParams = openParams;
        } else if (this.params != null && this.params.length > 0) {
            bindParams = this.params;
        } else {
            bindParams = null;
        }

        if (bindParams != null) {
            try (PreparedStatement pstmt = conn.prepareStatement(command)) {
                java.sql.ParameterMetaData metaData = pstmt.getParameterMetaData();
                for (int i = 0; i < bindParams.length; i++) {
                    CobolDataConverter.setParam(pstmt, i + 1, metaData, bindParams[i]);
                }
                pstmt.execute();
            }
        } else {
            try (Statement stmt = conn.createStatement()) {
                stmt.execute(command);
            }
        }
        isOpened = true;
    }

    /**
     * Fetch the next row from this cursor and write results to COBOL host variables.
     *
     * <p>If any column comes back SQL NULL while indicator variables are not supported, sets
     * {@code sqlca} to ECPG_MISSING_INDICATOR (sqlcode=-213, sqlstate="22002"). The row data
     * is still written (zero-fill for NULL columns), matching ECPG behavior of "row fetched
     * but flagged".
     *
     * @param conn the JDBC connection
     * @param resultParams output host variables to receive column values
     * @param sqlca SQLCA storage to flag NULL-without-indicator (may be null)
     * @return true if a row was fetched, false if no more rows
     * @throws SQLException if a database access error occurs
     */
    boolean fetch(Connection conn, AbstractCobolField[] resultParams, CobolDataStorage sqlca)
            throws SQLException {
        String fetchSql = "FETCH FORWARD 1 FROM " + name;
        try (Statement stmt = conn.createStatement()) {
            boolean hasResult = stmt.execute(fetchSql);
            if (!hasResult) {
                return false;
            }
            ResultSet rs = stmt.getResultSet();
            if (rs == null || !rs.next()) {
                if (rs != null) {
                    rs.close();
                }
                return false;
            }

            if (resultParams != null) {
                int columnCount = rs.getMetaData().getColumnCount();
                boolean sawNullWithoutIndicator = false;
                for (int i = 0; i < resultParams.length && i < columnCount; i++) {
                    byte[] value = CobolDataConverter.getValueFromResultSet(rs, i + 1);
                    if (value != null) {
                        CobolDataConverter.stringToCobol(resultParams[i], value);
                    } else {
                        resultParams[i]
                                .getDataStorage()
                                .memset((byte) 0, resultParams[i].getSize());
                        sawNullWithoutIndicator = true;
                    }
                }
                if (sawNullWithoutIndicator) {
                    SqlCA.setMissingIndicator(sqlca);
                }
            }
            rs.close();
            return true;
        }
    }

    /**
     * Close this cursor by executing a CLOSE statement.
     *
     * @param conn the JDBC connection
     * @throws SQLException if a database access error occurs
     */
    void close(Connection conn) throws SQLException {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("CLOSE " + name);
        }
        isOpened = false;
    }
}

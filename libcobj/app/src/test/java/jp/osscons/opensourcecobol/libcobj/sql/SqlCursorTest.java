package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.io.InputStream;
import java.io.Reader;
import java.math.BigDecimal;
import java.net.URL;
import java.sql.*;
import java.util.Calendar;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.Executor;
import org.junit.jupiter.api.Test;

class SqlCursorTest {

    // ---------- Constructor ----------

    @Test
    void testConstructor() {
        SqlCursor cursor = new SqlCursor("c1", "SELECT * FROM t", 2);
        assertEquals("c1", cursor.name);
        assertEquals("SELECT * FROM t", cursor.query);
        assertEquals(2, cursor.nParams);
        assertFalse(cursor.isOpened);
        assertNull(cursor.params);
    }

    // ---------- open ----------

    @Test
    void testOpen_NoParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT 1", 0);
        MockConnection conn = new MockConnection();
        cursor.open(conn, null);
        assertTrue(cursor.isOpened);
        assertEquals("DECLARE cur1 CURSOR FOR SELECT 1", conn.lastStatement.lastExecutedSql);
    }

    @Test
    void testOpen_EmptyParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT 1", 0);
        MockConnection conn = new MockConnection();
        cursor.open(conn, new jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField[0]);
        assertTrue(cursor.isOpened);
    }

    // ---------- close ----------

    @Test
    void testClose() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT 1", 0);
        cursor.isOpened = true;
        MockConnection conn = new MockConnection();
        cursor.close(conn);
        assertFalse(cursor.isOpened);
        assertEquals("CLOSE cur1", conn.lastStatement.lastExecutedSql);
    }

    // ---------- fetch ----------

    @Test
    void testFetch_NoResult() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT 1", 0);
        MockConnection conn = new MockConnection();
        conn.lastStatement.executeReturns = false;
        boolean result = cursor.fetch(conn, null);
        assertFalse(result);
    }

    @Test
    void testFetch_WithResult_NoRows() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT 1", 0);
        MockConnection conn = new MockConnection();
        conn.lastStatement.executeReturns = true;
        conn.lastStatement.mockResultSet = new MockResultSet(false, 0);
        boolean result = cursor.fetch(conn, null);
        assertFalse(result);
    }

    @Test
    void testFetch_NullResultSet() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT 1", 0);
        MockConnection conn = new MockConnection();
        conn.lastStatement.executeReturns = true;
        conn.lastStatement.mockResultSet = null;
        boolean result = cursor.fetch(conn, null);
        assertFalse(result);
    }

    // ---------- open with stored params ----------

    @Test
    void testOpen_WithStoredParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT * FROM t WHERE id=?", 1);
        jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute attr =
                new jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute(
                        jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute
                                .COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        0,
                        null);
        jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage storage =
                new jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage("0001".getBytes());
        jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField param =
                jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory.makeCobolField(
                        4, storage, attr);
        cursor.params = new jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField[] {param};
        MockConnection conn = new MockConnection();
        conn.mockPreparedStatement = new MockPreparedStatement();
        cursor.open(conn, null);
        assertTrue(cursor.isOpened);
    }

    @Test
    void testOpen_WithOpenParams() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT * FROM t WHERE id=?", 1);
        jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute attr =
                new jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute(
                        jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute
                                .COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        0,
                        null);
        jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage storage =
                new jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage("0002".getBytes());
        jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField param =
                jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory.makeCobolField(
                        4, storage, attr);
        MockConnection conn = new MockConnection();
        conn.mockPreparedStatement = new MockPreparedStatement();
        cursor.open(conn, new jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField[] {param});
        assertTrue(cursor.isOpened);
    }

    // ---------- fetch with result data ----------

    @Test
    void testFetch_WithResultData() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT name FROM t", 0);
        MockConnection conn = new MockConnection();
        conn.lastStatement.executeReturns = true;
        MockResultSet rs = new MockResultSet(true, 1);
        conn.lastStatement.mockResultSet = rs;
        // Create a field to receive data
        byte[] data = new byte[10];
        jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute attr =
                new jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute(
                        jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute
                                .COB_TYPE_ALPHANUMERIC,
                        0,
                        0,
                        0,
                        null);
        jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage storage =
                new jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage(data);
        jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField resultField =
                jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory.makeCobolField(
                        10, storage, attr);
        boolean result =
                cursor.fetch(
                        conn,
                        new jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField[] {
                            resultField
                        });
        assertTrue(result);
    }

    @Test
    void testFetch_WithNullValue() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT name FROM t", 0);
        MockConnection conn = new MockConnection();
        conn.lastStatement.executeReturns = true;
        // ResultSet with null value
        MockResultSet rs =
                new MockResultSet(true, 1) {
                    @Override
                    public String getString(int col) {
                        return null;
                    }
                };
        conn.lastStatement.mockResultSet = rs;
        byte[] data = new byte[10];
        java.util.Arrays.fill(data, (byte) 'X');
        jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute attr =
                new jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute(
                        jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute
                                .COB_TYPE_ALPHANUMERIC,
                        0,
                        0,
                        0,
                        null);
        jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage storage =
                new jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage(data);
        jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField resultField =
                jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory.makeCobolField(
                        10, storage, attr);
        boolean result =
                cursor.fetch(
                        conn,
                        new jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField[] {
                            resultField
                        });
        assertTrue(result);
        // data should be zeroed (null value)
        assertEquals(0, data[0]);
    }

    // ---------- close sets isOpened ----------

    @Test
    void testClose_SetsIsOpenedFalse() throws Exception {
        SqlCursor cursor = new SqlCursor("cur1", "SELECT 1", 0);
        cursor.isOpened = true;
        MockConnection conn = new MockConnection();
        cursor.close(conn);
        assertFalse(cursor.isOpened);
    }

    // ============================================================
    // Mock implementations
    // ============================================================

    private static class MockStatement implements Statement {
        String lastExecutedSql;
        boolean executeReturns = false;
        ResultSet mockResultSet = null;

        @Override
        public boolean execute(String sql) {
            lastExecutedSql = sql;
            return executeReturns;
        }

        @Override
        public ResultSet getResultSet() {
            return mockResultSet;
        }

        @Override
        public ResultSet executeQuery(String s) {
            return null;
        }

        @Override
        public int executeUpdate(String s) {
            return 0;
        }

        @Override
        public void close() {}

        @Override
        public int getMaxFieldSize() {
            return 0;
        }

        @Override
        public void setMaxFieldSize(int i) {}

        @Override
        public int getMaxRows() {
            return 0;
        }

        @Override
        public void setMaxRows(int i) {}

        @Override
        public void setEscapeProcessing(boolean b) {}

        @Override
        public int getQueryTimeout() {
            return 0;
        }

        @Override
        public void setQueryTimeout(int i) {}

        @Override
        public void cancel() {}

        @Override
        public SQLWarning getWarnings() {
            return null;
        }

        @Override
        public void clearWarnings() {}

        @Override
        public void setCursorName(String s) {}

        @Override
        public int getUpdateCount() {
            return 0;
        }

        @Override
        public boolean getMoreResults() {
            return false;
        }

        @Override
        public void setFetchDirection(int d) {}

        @Override
        public int getFetchDirection() {
            return 0;
        }

        @Override
        public void setFetchSize(int r) {}

        @Override
        public int getFetchSize() {
            return 0;
        }

        @Override
        public int getResultSetConcurrency() {
            return 0;
        }

        @Override
        public int getResultSetType() {
            return 0;
        }

        @Override
        public void addBatch(String s) {}

        @Override
        public void clearBatch() {}

        @Override
        public int[] executeBatch() {
            return new int[0];
        }

        @Override
        public Connection getConnection() {
            return null;
        }

        @Override
        public boolean getMoreResults(int i) {
            return false;
        }

        @Override
        public ResultSet getGeneratedKeys() {
            return null;
        }

        @Override
        public int executeUpdate(String s, int i) {
            return 0;
        }

        @Override
        public int executeUpdate(String s, int[] i) {
            return 0;
        }

        @Override
        public int executeUpdate(String s, String[] n) {
            return 0;
        }

        @Override
        public boolean execute(String s, int i) {
            return false;
        }

        @Override
        public boolean execute(String s, int[] i) {
            return false;
        }

        @Override
        public boolean execute(String s, String[] n) {
            return false;
        }

        @Override
        public int getResultSetHoldability() {
            return 0;
        }

        @Override
        public boolean isClosed() {
            return false;
        }

        @Override
        public void setPoolable(boolean b) {}

        @Override
        public boolean isPoolable() {
            return false;
        }

        @Override
        public void closeOnCompletion() {}

        @Override
        public boolean isCloseOnCompletion() {
            return false;
        }

        @Override
        public <T> T unwrap(Class<T> iface) {
            return null;
        }

        @Override
        public boolean isWrapperFor(Class<?> iface) {
            return false;
        }
    }

    private static class MockResultSet implements ResultSet {
        private boolean hasNext;
        private final int columnCount;

        MockResultSet(boolean hasNext, int columnCount) {
            this.hasNext = hasNext;
            this.columnCount = columnCount;
        }

        @Override
        public boolean next() {
            if (hasNext) {
                hasNext = false;
                return true;
            }
            return false;
        }

        @Override
        public ResultSetMetaData getMetaData() {
            return new ResultSetMetaData() {
                @Override
                public int getColumnCount() {
                    return columnCount;
                }

                @Override
                public int getColumnType(int col) {
                    return Types.VARCHAR;
                }

                @Override
                public boolean isAutoIncrement(int col) {
                    return false;
                }

                @Override
                public boolean isCaseSensitive(int col) {
                    return false;
                }

                @Override
                public boolean isSearchable(int col) {
                    return false;
                }

                @Override
                public boolean isCurrency(int col) {
                    return false;
                }

                @Override
                public int isNullable(int col) {
                    return 0;
                }

                @Override
                public boolean isSigned(int col) {
                    return false;
                }

                @Override
                public int getColumnDisplaySize(int col) {
                    return 0;
                }

                @Override
                public String getColumnLabel(int col) {
                    return "";
                }

                @Override
                public String getColumnName(int col) {
                    return "";
                }

                @Override
                public String getSchemaName(int col) {
                    return "";
                }

                @Override
                public int getPrecision(int col) {
                    return 0;
                }

                @Override
                public int getScale(int col) {
                    return 0;
                }

                @Override
                public String getTableName(int col) {
                    return "";
                }

                @Override
                public String getCatalogName(int col) {
                    return "";
                }

                @Override
                public String getColumnTypeName(int col) {
                    return "";
                }

                @Override
                public boolean isReadOnly(int col) {
                    return false;
                }

                @Override
                public boolean isWritable(int col) {
                    return false;
                }

                @Override
                public boolean isDefinitelyWritable(int col) {
                    return false;
                }

                @Override
                public String getColumnClassName(int col) {
                    return "";
                }

                @Override
                public <T> T unwrap(Class<T> iface) {
                    return null;
                }

                @Override
                public boolean isWrapperFor(Class<?> iface) {
                    return false;
                }
            };
        }

        @Override
        public void close() {}

        @Override
        public boolean wasNull() {
            return false;
        }

        @Override
        public String getString(int col) {
            return null;
        }

        @Override
        public boolean getBoolean(int col) {
            return false;
        }

        @Override
        public byte getByte(int col) {
            return 0;
        }

        @Override
        public short getShort(int col) {
            return 0;
        }

        @Override
        public int getInt(int col) {
            return 0;
        }

        @Override
        public long getLong(int col) {
            return 0;
        }

        @Override
        public float getFloat(int col) {
            return 0;
        }

        @Override
        public double getDouble(int col) {
            return 0;
        }

        @Override
        @SuppressWarnings("deprecation")
        public BigDecimal getBigDecimal(int col, int s) {
            return null;
        }

        @Override
        public byte[] getBytes(int col) {
            return null;
        }

        @Override
        public Date getDate(int col) {
            return null;
        }

        @Override
        public Time getTime(int col) {
            return null;
        }

        @Override
        public Timestamp getTimestamp(int col) {
            return null;
        }

        @Override
        public InputStream getAsciiStream(int col) {
            return null;
        }

        @Override
        @SuppressWarnings("deprecation")
        public InputStream getUnicodeStream(int col) {
            return null;
        }

        @Override
        public InputStream getBinaryStream(int col) {
            return null;
        }

        @Override
        public String getString(String col) {
            return null;
        }

        @Override
        public boolean getBoolean(String col) {
            return false;
        }

        @Override
        public byte getByte(String col) {
            return 0;
        }

        @Override
        public short getShort(String col) {
            return 0;
        }

        @Override
        public int getInt(String col) {
            return 0;
        }

        @Override
        public long getLong(String col) {
            return 0;
        }

        @Override
        public float getFloat(String col) {
            return 0;
        }

        @Override
        public double getDouble(String col) {
            return 0;
        }

        @Override
        @SuppressWarnings("deprecation")
        public BigDecimal getBigDecimal(String col, int s) {
            return null;
        }

        @Override
        public byte[] getBytes(String col) {
            return null;
        }

        @Override
        public Date getDate(String col) {
            return null;
        }

        @Override
        public Time getTime(String col) {
            return null;
        }

        @Override
        public Timestamp getTimestamp(String col) {
            return null;
        }

        @Override
        public InputStream getAsciiStream(String col) {
            return null;
        }

        @Override
        @SuppressWarnings("deprecation")
        public InputStream getUnicodeStream(String col) {
            return null;
        }

        @Override
        public InputStream getBinaryStream(String col) {
            return null;
        }

        @Override
        public SQLWarning getWarnings() {
            return null;
        }

        @Override
        public void clearWarnings() {}

        @Override
        public String getCursorName() {
            return null;
        }

        @Override
        public Object getObject(int col) {
            return null;
        }

        @Override
        public Object getObject(String col) {
            return null;
        }

        @Override
        public int findColumn(String col) {
            return 0;
        }

        @Override
        public Reader getCharacterStream(int col) {
            return null;
        }

        @Override
        public Reader getCharacterStream(String col) {
            return null;
        }

        @Override
        public BigDecimal getBigDecimal(int col) {
            return null;
        }

        @Override
        public BigDecimal getBigDecimal(String col) {
            return null;
        }

        @Override
        public boolean isBeforeFirst() {
            return false;
        }

        @Override
        public boolean isAfterLast() {
            return false;
        }

        @Override
        public boolean isFirst() {
            return false;
        }

        @Override
        public boolean isLast() {
            return false;
        }

        @Override
        public void beforeFirst() {}

        @Override
        public void afterLast() {}

        @Override
        public boolean first() {
            return false;
        }

        @Override
        public boolean last() {
            return false;
        }

        @Override
        public int getRow() {
            return 0;
        }

        @Override
        public boolean absolute(int row) {
            return false;
        }

        @Override
        public boolean relative(int rows) {
            return false;
        }

        @Override
        public boolean previous() {
            return false;
        }

        @Override
        public void setFetchDirection(int d) {}

        @Override
        public int getFetchDirection() {
            return 0;
        }

        @Override
        public void setFetchSize(int r) {}

        @Override
        public int getFetchSize() {
            return 0;
        }

        @Override
        public int getType() {
            return 0;
        }

        @Override
        public int getConcurrency() {
            return 0;
        }

        @Override
        public boolean rowUpdated() {
            return false;
        }

        @Override
        public boolean rowInserted() {
            return false;
        }

        @Override
        public boolean rowDeleted() {
            return false;
        }

        @Override
        public void updateNull(int col) {}

        @Override
        public void updateBoolean(int col, boolean v) {}

        @Override
        public void updateByte(int col, byte v) {}

        @Override
        public void updateShort(int col, short v) {}

        @Override
        public void updateInt(int col, int v) {}

        @Override
        public void updateLong(int col, long v) {}

        @Override
        public void updateFloat(int col, float v) {}

        @Override
        public void updateDouble(int col, double v) {}

        @Override
        public void updateBigDecimal(int col, BigDecimal v) {}

        @Override
        public void updateString(int col, String v) {}

        @Override
        public void updateBytes(int col, byte[] v) {}

        @Override
        public void updateDate(int col, Date v) {}

        @Override
        public void updateTime(int col, Time v) {}

        @Override
        public void updateTimestamp(int col, Timestamp v) {}

        @Override
        public void updateAsciiStream(int col, InputStream s, int l) {}

        @Override
        public void updateBinaryStream(int col, InputStream s, int l) {}

        @Override
        public void updateCharacterStream(int col, Reader r, int l) {}

        @Override
        public void updateObject(int col, Object v, int s) {}

        @Override
        public void updateObject(int col, Object v) {}

        @Override
        public void updateNull(String col) {}

        @Override
        public void updateBoolean(String col, boolean v) {}

        @Override
        public void updateByte(String col, byte v) {}

        @Override
        public void updateShort(String col, short v) {}

        @Override
        public void updateInt(String col, int v) {}

        @Override
        public void updateLong(String col, long v) {}

        @Override
        public void updateFloat(String col, float v) {}

        @Override
        public void updateDouble(String col, double v) {}

        @Override
        public void updateBigDecimal(String col, BigDecimal v) {}

        @Override
        public void updateString(String col, String v) {}

        @Override
        public void updateBytes(String col, byte[] v) {}

        @Override
        public void updateDate(String col, Date v) {}

        @Override
        public void updateTime(String col, Time v) {}

        @Override
        public void updateTimestamp(String col, Timestamp v) {}

        @Override
        public void updateAsciiStream(String col, InputStream s, int l) {}

        @Override
        public void updateBinaryStream(String col, InputStream s, int l) {}

        @Override
        public void updateCharacterStream(String col, Reader r, int l) {}

        @Override
        public void updateObject(String col, Object v, int s) {}

        @Override
        public void updateObject(String col, Object v) {}

        @Override
        public void insertRow() {}

        @Override
        public void updateRow() {}

        @Override
        public void deleteRow() {}

        @Override
        public void refreshRow() {}

        @Override
        public void cancelRowUpdates() {}

        @Override
        public void moveToInsertRow() {}

        @Override
        public void moveToCurrentRow() {}

        @Override
        public Statement getStatement() {
            return null;
        }

        @Override
        public Object getObject(int col, Map<String, Class<?>> m) {
            return null;
        }

        @Override
        public Ref getRef(int col) {
            return null;
        }

        @Override
        public Blob getBlob(int col) {
            return null;
        }

        @Override
        public Clob getClob(int col) {
            return null;
        }

        @Override
        public Array getArray(int col) {
            return null;
        }

        @Override
        public Object getObject(String col, Map<String, Class<?>> m) {
            return null;
        }

        @Override
        public Ref getRef(String col) {
            return null;
        }

        @Override
        public Blob getBlob(String col) {
            return null;
        }

        @Override
        public Clob getClob(String col) {
            return null;
        }

        @Override
        public Array getArray(String col) {
            return null;
        }

        @Override
        public Date getDate(int col, Calendar c) {
            return null;
        }

        @Override
        public Date getDate(String col, Calendar c) {
            return null;
        }

        @Override
        public Time getTime(int col, Calendar c) {
            return null;
        }

        @Override
        public Time getTime(String col, Calendar c) {
            return null;
        }

        @Override
        public Timestamp getTimestamp(int col, Calendar c) {
            return null;
        }

        @Override
        public Timestamp getTimestamp(String col, Calendar c) {
            return null;
        }

        @Override
        public URL getURL(int col) {
            return null;
        }

        @Override
        public URL getURL(String col) {
            return null;
        }

        @Override
        public void updateRef(int col, Ref v) {}

        @Override
        public void updateRef(String col, Ref v) {}

        @Override
        public void updateBlob(int col, Blob v) {}

        @Override
        public void updateBlob(String col, Blob v) {}

        @Override
        public void updateClob(int col, Clob v) {}

        @Override
        public void updateClob(String col, Clob v) {}

        @Override
        public void updateArray(int col, Array v) {}

        @Override
        public void updateArray(String col, Array v) {}

        @Override
        public RowId getRowId(int col) {
            return null;
        }

        @Override
        public RowId getRowId(String col) {
            return null;
        }

        @Override
        public void updateRowId(int col, RowId v) {}

        @Override
        public void updateRowId(String col, RowId v) {}

        @Override
        public int getHoldability() {
            return 0;
        }

        @Override
        public void updateNString(int col, String v) {}

        @Override
        public void updateNString(String col, String v) {}

        @Override
        public void updateNClob(int col, NClob v) {}

        @Override
        public void updateNClob(String col, NClob v) {}

        @Override
        public NClob getNClob(int col) {
            return null;
        }

        @Override
        public NClob getNClob(String col) {
            return null;
        }

        @Override
        public SQLXML getSQLXML(int col) {
            return null;
        }

        @Override
        public SQLXML getSQLXML(String col) {
            return null;
        }

        @Override
        public void updateSQLXML(int col, SQLXML v) {}

        @Override
        public void updateSQLXML(String col, SQLXML v) {}

        @Override
        public String getNString(int col) {
            return null;
        }

        @Override
        public String getNString(String col) {
            return null;
        }

        @Override
        public Reader getNCharacterStream(int col) {
            return null;
        }

        @Override
        public Reader getNCharacterStream(String col) {
            return null;
        }

        @Override
        public void updateNCharacterStream(int col, Reader r, long l) {}

        @Override
        public void updateNCharacterStream(String col, Reader r, long l) {}

        @Override
        public void updateAsciiStream(int col, InputStream s, long l) {}

        @Override
        public void updateBinaryStream(int col, InputStream s, long l) {}

        @Override
        public void updateCharacterStream(int col, Reader r, long l) {}

        @Override
        public void updateAsciiStream(String col, InputStream s, long l) {}

        @Override
        public void updateBinaryStream(String col, InputStream s, long l) {}

        @Override
        public void updateCharacterStream(String col, Reader r, long l) {}

        @Override
        public void updateBlob(int col, InputStream s, long l) {}

        @Override
        public void updateBlob(String col, InputStream s, long l) {}

        @Override
        public void updateClob(int col, Reader r, long l) {}

        @Override
        public void updateClob(String col, Reader r, long l) {}

        @Override
        public void updateNClob(int col, Reader r, long l) {}

        @Override
        public void updateNClob(String col, Reader r, long l) {}

        @Override
        public void updateNCharacterStream(int col, Reader r) {}

        @Override
        public void updateNCharacterStream(String col, Reader r) {}

        @Override
        public void updateAsciiStream(int col, InputStream s) {}

        @Override
        public void updateBinaryStream(int col, InputStream s) {}

        @Override
        public void updateCharacterStream(int col, Reader r) {}

        @Override
        public void updateAsciiStream(String col, InputStream s) {}

        @Override
        public void updateBinaryStream(String col, InputStream s) {}

        @Override
        public void updateCharacterStream(String col, Reader r) {}

        @Override
        public void updateBlob(int col, InputStream s) {}

        @Override
        public void updateBlob(String col, InputStream s) {}

        @Override
        public void updateClob(int col, Reader r) {}

        @Override
        public void updateClob(String col, Reader r) {}

        @Override
        public void updateNClob(int col, Reader r) {}

        @Override
        public void updateNClob(String col, Reader r) {}

        @Override
        public <T> T getObject(int col, Class<T> t) {
            return null;
        }

        @Override
        public <T> T getObject(String col, Class<T> t) {
            return null;
        }

        @Override
        public boolean isClosed() {
            return false;
        }

        @Override
        public <T> T unwrap(Class<T> iface) {
            return null;
        }

        @Override
        public boolean isWrapperFor(Class<?> iface) {
            return false;
        }
    }

    private static class MockPreparedStatement extends MockStatement implements PreparedStatement {
        @Override
        public ResultSet executeQuery() {
            return null;
        }

        @Override
        public int executeUpdate() {
            return 0;
        }

        @Override
        public void setNull(int i, int t) {}

        @Override
        public void setBoolean(int i, boolean v) {}

        @Override
        public void setByte(int i, byte v) {}

        @Override
        public void setShort(int i, short v) {}

        @Override
        public void setInt(int i, int v) {}

        @Override
        public void setLong(int i, long v) {}

        @Override
        public void setFloat(int i, float v) {}

        @Override
        public void setDouble(int i, double v) {}

        @Override
        public void setBigDecimal(int i, java.math.BigDecimal v) {}

        @Override
        public void setString(int i, String v) {}

        @Override
        public void setBytes(int i, byte[] v) {}

        @Override
        public void setDate(int i, Date v) {}

        @Override
        public void setTime(int i, Time v) {}

        @Override
        public void setTimestamp(int i, Timestamp v) {}

        @Override
        public void setAsciiStream(int i, java.io.InputStream s, int l) {}

        @Override
        @SuppressWarnings("deprecation")
        public void setUnicodeStream(int i, java.io.InputStream s, int l) {}

        @Override
        public void setBinaryStream(int i, java.io.InputStream s, int l) {}

        @Override
        public void clearParameters() {}

        @Override
        public void setObject(int i, Object v, int t) {}

        @Override
        public void setObject(int i, Object v) {}

        @Override
        public boolean execute() {
            return false;
        }

        @Override
        public void addBatch() {}

        @Override
        public void setCharacterStream(int i, java.io.Reader r, int l) {}

        @Override
        public void setRef(int i, Ref v) {}

        @Override
        public void setBlob(int i, Blob v) {}

        @Override
        public void setClob(int i, Clob v) {}

        @Override
        public void setArray(int i, Array v) {}

        @Override
        public ResultSetMetaData getMetaData() {
            return null;
        }

        @Override
        public void setDate(int i, Date v, Calendar c) {}

        @Override
        public void setTime(int i, Time v, Calendar c) {}

        @Override
        public void setTimestamp(int i, Timestamp v, Calendar c) {}

        @Override
        public void setNull(int i, int t, String n) {}

        @Override
        public void setURL(int i, URL v) {}

        @Override
        public ParameterMetaData getParameterMetaData() throws SQLException {
            return null;
        }

        @Override
        public void setRowId(int i, RowId v) {}

        @Override
        public void setNString(int i, String v) {}

        @Override
        public void setNCharacterStream(int i, java.io.Reader v, long l) {}

        @Override
        public void setNClob(int i, NClob v) {}

        @Override
        public void setClob(int i, java.io.Reader r, long l) {}

        @Override
        public void setBlob(int i, java.io.InputStream s, long l) {}

        @Override
        public void setNClob(int i, java.io.Reader r, long l) {}

        @Override
        public void setSQLXML(int i, SQLXML v) {}

        @Override
        public void setObject(int i, Object v, int t, int s) {}

        @Override
        public void setAsciiStream(int i, java.io.InputStream s, long l) {}

        @Override
        public void setBinaryStream(int i, java.io.InputStream s, long l) {}

        @Override
        public void setCharacterStream(int i, java.io.Reader r, long l) {}

        @Override
        public void setAsciiStream(int i, java.io.InputStream s) {}

        @Override
        public void setBinaryStream(int i, java.io.InputStream s) {}

        @Override
        public void setCharacterStream(int i, java.io.Reader r) {}

        @Override
        public void setNCharacterStream(int i, java.io.Reader r) {}

        @Override
        public void setClob(int i, java.io.Reader r) {}

        @Override
        public void setBlob(int i, java.io.InputStream s) {}

        @Override
        public void setNClob(int i, java.io.Reader r) {}
    }

    private static class MockConnection implements Connection {
        MockStatement lastStatement = new MockStatement();
        MockPreparedStatement mockPreparedStatement;

        @Override
        public Statement createStatement() {
            return lastStatement;
        }

        @Override
        public PreparedStatement prepareStatement(String sql) {
            if (mockPreparedStatement != null) {
                return mockPreparedStatement;
            }
            return null;
        }

        @Override
        public CallableStatement prepareCall(String sql) {
            return null;
        }

        @Override
        public String nativeSQL(String sql) {
            return sql;
        }

        @Override
        public void setAutoCommit(boolean b) {}

        @Override
        public boolean getAutoCommit() {
            return false;
        }

        @Override
        public void commit() {}

        @Override
        public void rollback() {}

        @Override
        public void close() {}

        @Override
        public boolean isClosed() {
            return false;
        }

        @Override
        public DatabaseMetaData getMetaData() {
            return null;
        }

        @Override
        public void setReadOnly(boolean b) {}

        @Override
        public boolean isReadOnly() {
            return false;
        }

        @Override
        public void setCatalog(String s) {}

        @Override
        public String getCatalog() {
            return null;
        }

        @Override
        public void setTransactionIsolation(int i) {}

        @Override
        public int getTransactionIsolation() {
            return 0;
        }

        @Override
        public SQLWarning getWarnings() {
            return null;
        }

        @Override
        public void clearWarnings() {}

        @Override
        public Statement createStatement(int t, int c) {
            return lastStatement;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, int t, int c) {
            return null;
        }

        @Override
        public CallableStatement prepareCall(String sql, int t, int c) {
            return null;
        }

        @Override
        public Map<String, Class<?>> getTypeMap() {
            return null;
        }

        @Override
        public void setTypeMap(Map<String, Class<?>> m) {}

        @Override
        public void setHoldability(int h) {}

        @Override
        public int getHoldability() {
            return 0;
        }

        @Override
        public Savepoint setSavepoint() {
            return null;
        }

        @Override
        public Savepoint setSavepoint(String s) {
            return null;
        }

        @Override
        public void rollback(Savepoint s) {}

        @Override
        public void releaseSavepoint(Savepoint s) {}

        @Override
        public Statement createStatement(int t, int c, int h) {
            return lastStatement;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, int t, int c, int h) {
            return null;
        }

        @Override
        public CallableStatement prepareCall(String sql, int t, int c, int h) {
            return null;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, int f) {
            return null;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, int[] cols) {
            return null;
        }

        @Override
        public PreparedStatement prepareStatement(String sql, String[] cols) {
            return null;
        }

        @Override
        public Clob createClob() {
            return null;
        }

        @Override
        public Blob createBlob() {
            return null;
        }

        @Override
        public NClob createNClob() {
            return null;
        }

        @Override
        public SQLXML createSQLXML() {
            return null;
        }

        @Override
        public boolean isValid(int timeout) {
            return true;
        }

        @Override
        public void setClientInfo(String k, String v) {}

        @Override
        public void setClientInfo(Properties p) {}

        @Override
        public String getClientInfo(String k) {
            return null;
        }

        @Override
        public Properties getClientInfo() {
            return null;
        }

        @Override
        public Array createArrayOf(String t, Object[] e) {
            return null;
        }

        @Override
        public Struct createStruct(String t, Object[] a) {
            return null;
        }

        @Override
        public void setSchema(String s) {}

        @Override
        public String getSchema() {
            return null;
        }

        @Override
        public void abort(Executor e) {}

        @Override
        public void setNetworkTimeout(Executor e, int ms) {}

        @Override
        public int getNetworkTimeout() {
            return 0;
        }

        @Override
        public <T> T unwrap(Class<T> iface) {
            return null;
        }

        @Override
        public boolean isWrapperFor(Class<?> iface) {
            return false;
        }
    }
}

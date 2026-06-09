package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.sql.*;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.testcontainers.containers.PostgreSQLContainer;
import org.testcontainers.junit.jupiter.Container;
import org.testcontainers.junit.jupiter.Testcontainers;

@Testcontainers
class CobolDataConverterTest {

    @Container
    static PostgreSQLContainer<?> postgres =
            new PostgreSQLContainer<>("postgres:15")
                    .withDatabaseName("testdb")
                    .withUsername("test_user")
                    .withPassword("test_pass");

    private Connection conn;

    @BeforeEach
    void setUp() throws Exception {
        conn =
                DriverManager.getConnection(
                        postgres.getJdbcUrl(), postgres.getUsername(), postgres.getPassword());
        conn.setAutoCommit(true);
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS conv_test");
            stmt.execute("CREATE TABLE conv_test (id SERIAL PRIMARY KEY, val TEXT)");
        }
    }

    @AfterEach
    void tearDown() throws Exception {
        if (conn != null && !conn.isClosed()) {
            try (Statement stmt = conn.createStatement()) {
                stmt.execute("DROP TABLE IF EXISTS conv_test");
            }
            conn.close();
        }
    }

    // ============================================================
    // Helper to create COBOL fields
    // ============================================================

    private static AbstractCobolField makeField(
            int size, byte[] data, int type, int digits, int scale, int flags) {
        CobolFieldAttribute attr = new CobolFieldAttribute(type, digits, scale, flags, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        return CobolFieldFactory.makeCobolField(size, storage, attr);
    }

    // ============================================================
    // cobolToString: UNSIGNED NUMERIC
    // ============================================================

    @Test
    void testCobolToString_UnsignedNumeric_NoScale() {
        byte[] data = "1234".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        assertEquals("1234", CobolDataConverter.cobolToString(field), "Unsigned numeric 1234");
    }

    @Test
    void testCobolToString_UnsignedNumeric_WithScale() {
        byte[] data = "123456".getBytes();
        AbstractCobolField field =
                makeField(6, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 6, 2, 0);
        assertEquals(
                "1234.56",
                CobolDataConverter.cobolToString(field),
                "Unsigned numeric with scale should insert decimal point");
    }

    @Test
    void testCobolToString_UnsignedNumeric_LeadingZeros() {
        byte[] data = "0012".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        assertEquals(
                "12", CobolDataConverter.cobolToString(field), "Leading zeros should be stripped");
    }

    @Test
    void testCobolToString_UnsignedNumeric_AllZeros() {
        byte[] data = "0000".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        assertEquals("0", CobolDataConverter.cobolToString(field), "All zeros should produce 0");
    }

    // ============================================================
    // cobolToString: SIGNED TRAILING COMBINED
    // ============================================================

    @Test
    void testCobolToString_SignedTrailingCombined_Positive() {
        byte[] data = "1234".getBytes();
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        assertEquals(
                "1234",
                CobolDataConverter.cobolToString(field),
                "Positive signed trailing combined");
    }

    @Test
    void testCobolToString_SignedTrailingCombined_Negative() {
        byte[] data = new byte[] {'1', '2', '3', (byte) 0x74};
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        assertEquals(
                "-1234",
                CobolDataConverter.cobolToString(field),
                "Negative signed trailing combined");
    }

    // ============================================================
    // cobolToString: SIGNED TRAILING SEPARATE
    // ============================================================

    @Test
    void testCobolToString_SignedTrailingSeparate_Positive() {
        byte[] data = "1234+".getBytes();
        AbstractCobolField field =
                makeField(
                        5,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE);
        assertEquals(
                "1234",
                CobolDataConverter.cobolToString(field),
                "Positive signed trailing separate");
    }

    @Test
    void testCobolToString_SignedTrailingSeparate_Negative() {
        byte[] data = "1234-".getBytes();
        AbstractCobolField field =
                makeField(
                        5,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE);
        assertEquals(
                "-1234",
                CobolDataConverter.cobolToString(field),
                "Negative signed trailing separate");
    }

    // ============================================================
    // cobolToString: SIGNED LEADING SEPARATE
    // ============================================================

    @Test
    void testCobolToString_SignedLeadingSeparate_Positive() {
        byte[] data = "+1234".getBytes();
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        assertEquals(
                "1234",
                CobolDataConverter.cobolToString(field),
                "Positive signed leading separate (sign dropped, leading zeros stripped)");
    }

    @Test
    void testCobolToString_SignedLeadingSeparate_Negative() {
        byte[] data = "-1234".getBytes();
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        assertEquals(
                "-1234",
                CobolDataConverter.cobolToString(field),
                "Negative signed leading separate");
    }

    // ============================================================
    // cobolToString: SIGNED LEADING COMBINED
    // ============================================================

    @Test
    void testCobolToString_SignedLeadingCombined_Positive() {
        byte[] data = "1234".getBytes();
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        assertEquals(
                "1234",
                CobolDataConverter.cobolToString(field),
                "Positive signed leading combined");
    }

    @Test
    void testCobolToString_SignedLeadingCombined_Negative() {
        byte[] data = new byte[] {(byte) 0x71, '2', '3', '4'};
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        assertEquals(
                "-1234",
                CobolDataConverter.cobolToString(field),
                "Negative signed leading combined");
    }

    // ============================================================
    // cobolToString: NUMERIC DISPLAY edge cases (shared formatting path)
    // ============================================================

    @Test
    void testCobolToString_SignedTrailingSeparate_NegativeZero() {
        // A signed zero must render as plain "0" (no sign).
        byte[] data = "0000-".getBytes();
        AbstractCobolField field =
                makeField(
                        5,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE);
        assertEquals(
                "0", CobolDataConverter.cobolToString(field), "Negative zero should render as 0");
    }

    @Test
    void testCobolToString_UnsignedNumeric_FullyFractional() {
        // PIC V9(6) = all fractional digits: "000123" => 0.000123
        byte[] data = "000123".getBytes();
        AbstractCobolField field =
                makeField(6, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 6, 6, 0);
        assertEquals(
                "0.000123",
                CobolDataConverter.cobolToString(field),
                "Fully-fractional value should keep its leading 0.");
    }

    @Test
    void testCobolToString_SignedLeadingSeparate_FullyFractional() {
        // PIC SV9(6) LEADING SEPARATE: "-000123" => -0.000123
        byte[] data = "-000123".getBytes();
        AbstractCobolField field =
                makeField(
                        6,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        6,
                        6,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        assertEquals(
                "-0.000123",
                CobolDataConverter.cobolToString(field),
                "Fully-fractional leading separate should render -0.000123");
    }

    // ============================================================
    // cobolToString: PACKED DECIMAL (COMP-3)
    // ============================================================

    @Test
    void testCobolToString_SignedPacked_Positive() {
        // PIC S9(5) COMP-3 VALUE 12345: 0x01 0x23 0x45 0x0C => packed 01 23 4C
        byte[] data = new byte[] {(byte) 0x01, (byte) 0x23, (byte) 0x4C};
        AbstractCobolField field =
                makeField(
                        3,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        assertEquals("1234", CobolDataConverter.cobolToString(field), "Signed packed positive");
    }

    @Test
    void testCobolToString_SignedPacked_Negative() {
        byte[] data = new byte[] {(byte) 0x01, (byte) 0x23, (byte) 0x4D};
        AbstractCobolField field =
                makeField(
                        3,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        assertEquals("-1234", CobolDataConverter.cobolToString(field), "Signed packed negative");
    }

    @Test
    void testCobolToString_UnsignedPacked() {
        byte[] data = new byte[] {(byte) 0x12, (byte) 0x3F};
        AbstractCobolField field =
                makeField(2, data, CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 3, 0, 0);
        assertEquals("123", CobolDataConverter.cobolToString(field), "Unsigned packed");
    }

    // ============================================================
    // cobolToString: BINARY (COMP-5)
    // ============================================================

    @Test
    void testCobolToString_UnsignedBinary2Bytes() {
        byte[] data = new byte[2];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putShort((short) 1234);
        AbstractCobolField field =
                makeField(2, data, CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 4, 0, 0);
        assertEquals("1234", CobolDataConverter.cobolToString(field), "Unsigned binary 2 bytes");
    }

    @Test
    void testCobolToString_SignedBinary4Bytes() {
        byte[] data = new byte[4];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putInt(-5678);
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        9,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        assertEquals("-5678", CobolDataConverter.cobolToString(field), "Signed binary 4 bytes");
    }

    // ============================================================
    // cobolToString: ALPHANUMERIC
    // ============================================================

    @Test
    void testCobolToString_Alphanumeric() {
        byte[] data = "Hello     ".getBytes();
        AbstractCobolField field =
                makeField(10, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        assertEquals(
                "Hello",
                CobolDataConverter.cobolToString(field),
                "Alphanumeric should trim trailing spaces");
    }

    @Test
    void testCobolToString_Null() {
        assertEquals(
                "",
                CobolDataConverter.cobolToString(null),
                "Null field should produce empty string");
    }

    // ============================================================
    // cobolToString: FLOAT (COMP-2)
    // ============================================================

    @Test
    void testCobolToString_Float() {
        byte[] data = new byte[8];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putDouble(2.75);
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DOUBLE, 0, 0, 0);
        assertEquals("2.75", CobolDataConverter.cobolToString(field), "Double field conversion");
    }

    // ============================================================
    // Round-trip tests: cobolToString -> INSERT -> SELECT -> stringToCobol
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testRoundTrip_UnsignedNumeric() throws Exception {
        byte[] original = "0042".getBytes();
        AbstractCobolField field =
                makeField(4, original, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        String strVal = CobolDataConverter.cobolToString(field);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO conv_test (val) VALUES (?) RETURNING id")) {
            ps.setString(1, strVal);
            try (ResultSet rs = ps.executeQuery()) {
                assertTrue(rs.next(), "INSERT should return a row");
                int id = rs.getInt(1);

                try (Statement stmt = conn.createStatement();
                        ResultSet rs2 =
                                stmt.executeQuery("SELECT val FROM conv_test WHERE id = " + id)) {
                    assertTrue(rs2.next(), "SELECT should return a row");
                    byte[] resultData = CobolDataConverter.getValueFromResultSet(rs2, 1);
                    assertNotNull(resultData, "Result data should not be null");

                    byte[] storage = new byte[4];
                    AbstractCobolField outField =
                            makeField(
                                    4,
                                    storage,
                                    CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                                    4,
                                    0,
                                    0);
                    CobolDataConverter.stringToCobol(outField, resultData);
                    assertArrayEquals(
                            original,
                            outField.getDataStorage().getByteArray(0, 4),
                            "Round-trip should preserve unsigned numeric data");
                }
            }
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testRoundTrip_SignedTrailingCombined() throws Exception {
        byte[] original = new byte[] {'0', '1', '2', (byte) 0x73};
        AbstractCobolField field =
                makeField(
                        4,
                        original,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String strVal = CobolDataConverter.cobolToString(field);
        assertEquals("-123", strVal, "Signed trailing combined should produce -123");

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO conv_test (val) VALUES (?) RETURNING id")) {
            ps.setString(1, strVal);
            try (ResultSet rs = ps.executeQuery()) {
                assertTrue(rs.next(), "INSERT should return a row");
                int id = rs.getInt(1);

                try (Statement stmt = conn.createStatement();
                        ResultSet rs2 =
                                stmt.executeQuery("SELECT val FROM conv_test WHERE id = " + id)) {
                    assertTrue(rs2.next(), "SELECT should return a row");
                    byte[] resultData = CobolDataConverter.getValueFromResultSet(rs2, 1);
                    assertNotNull(resultData, "Result data should not be null");

                    byte[] storage = new byte[4];
                    AbstractCobolField outField =
                            makeField(
                                    4,
                                    storage,
                                    CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                                    4,
                                    0,
                                    CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
                    CobolDataConverter.stringToCobol(outField, resultData);
                    assertArrayEquals(
                            original,
                            outField.getDataStorage().getByteArray(0, 4),
                            "Round-trip should preserve signed trailing combined data");
                }
            }
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testRoundTrip_Alphanumeric() throws Exception {
        byte[] original = "Hello     ".getBytes();
        AbstractCobolField field =
                makeField(10, original, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        String strVal = CobolDataConverter.cobolToString(field);
        assertEquals("Hello", strVal, "Alphanumeric should trim to Hello");

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO conv_test (val) VALUES (?) RETURNING id")) {
            ps.setString(1, strVal);
            try (ResultSet rs = ps.executeQuery()) {
                assertTrue(rs.next(), "INSERT should return a row");
                int id = rs.getInt(1);

                try (Statement stmt = conn.createStatement();
                        ResultSet rs2 =
                                stmt.executeQuery("SELECT val FROM conv_test WHERE id = " + id)) {
                    assertTrue(rs2.next(), "SELECT should return a row");
                    byte[] resultData = CobolDataConverter.getValueFromResultSet(rs2, 1);
                    assertNotNull(resultData, "Result data should not be null");

                    byte[] storage = new byte[10];
                    AbstractCobolField outField =
                            makeField(
                                    10,
                                    storage,
                                    CobolFieldAttribute.COB_TYPE_ALPHANUMERIC,
                                    0,
                                    0,
                                    0);
                    CobolDataConverter.stringToCobol(outField, resultData);
                    byte[] written = outField.getDataStorage().getByteArray(0, 10);
                    assertEquals(
                            "Hello     ",
                            new String(written),
                            "Round-trip should preserve alphanumeric data with padding");
                }
            }
        }
    }

    // ============================================================
    // setParam() with real PreparedStatement
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetParam_IntegerType() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (id INTEGER)");
        }

        byte[] data = "0042".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (id) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, ps.getParameterMetaData(), field);
            ps.execute();
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT id FROM param_test")) {
            assertTrue(rs.next(), "Should have a row");
            assertEquals(42, rs.getInt(1), "Inserted integer value should be 42");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetParam_VarcharType() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (name VARCHAR(20))");
        }

        byte[] data = "Hello     ".getBytes();
        AbstractCobolField field =
                makeField(10, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (name) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, ps.getParameterMetaData(), field);
            ps.execute();
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT name FROM param_test")) {
            assertTrue(rs.next(), "Should have a row");
            assertEquals("Hello", rs.getString(1), "Inserted varchar value should be Hello");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetParam_NullMetadata() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (val TEXT)");
        }

        byte[] data = "1234".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (val) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, null, field);
            ps.execute();
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM param_test")) {
            assertTrue(rs.next(), "Should have a row");
            assertEquals("1234", rs.getString(1), "Value with null metadata should be 1234");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetParam_DecimalType() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (val DECIMAL(10,2))");
        }

        byte[] data = "123456".getBytes();
        AbstractCobolField field =
                makeField(6, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 6, 2, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (val) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, ps.getParameterMetaData(), field);
            ps.execute();
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM param_test")) {
            assertTrue(rs.next(), "Should have a row");
            assertEquals(1234.56, rs.getDouble(1), 0.001, "Decimal value should be 1234.56");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    // ============================================================
    // getValueFromResultSet() with real ResultSet
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_VarcharColumn() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val VARCHAR(20))");
            stmt.execute("INSERT INTO rs_test (val) VALUES ('Hello')");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNotNull(result, "Result should not be null");
            assertEquals("Hello", new String(result), "Varchar value should be Hello");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_IntegerColumn() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val INTEGER)");
            stmt.execute("INSERT INTO rs_test (val) VALUES (42)");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNotNull(result, "Result should not be null");
            assertEquals("42", new String(result), "Integer value should be 42");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_DecimalColumn() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val DECIMAL(10,2))");
            stmt.execute("INSERT INTO rs_test (val) VALUES (123.45)");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNotNull(result, "Result should not be null");
            assertEquals("123.45", new String(result), "Decimal value should be 123.45");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_NullValue() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val VARCHAR(20))");
            stmt.execute("INSERT INTO rs_test (val) VALUES (NULL)");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNull(result, "NULL column should return null");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_BigIntColumn() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val BIGINT)");
            stmt.execute("INSERT INTO rs_test (val) VALUES (9999999999)");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNotNull(result, "Result should not be null");
            assertEquals("9999999999", new String(result), "BigInt value should match");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_BooleanColumn() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val BOOLEAN)");
            stmt.execute("INSERT INTO rs_test (val) VALUES (TRUE)");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNotNull(result, "Result should not be null");
            // PostgreSQL may return "t" or "true" for boolean
            String boolStr = new String(result);
            assertTrue(
                    "true".equals(boolStr) || "t".equals(boolStr), "Boolean should be true or t");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_DoubleColumn() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val DOUBLE PRECISION)");
            stmt.execute("INSERT INTO rs_test (val) VALUES (2.75)");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNotNull(result, "Result should not be null");
            assertEquals(
                    2.75,
                    Double.parseDouble(new String(result)),
                    0.001,
                    "Double value should be 2.75");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_DateColumn() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val DATE)");
            stmt.execute("INSERT INTO rs_test (val) VALUES ('2024-01-15')");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNotNull(result, "Result should not be null");
            assertTrue(new String(result).contains("2024-01-15"), "Date should contain 2024-01-15");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_TimestampColumn() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val TIMESTAMP)");
            stmt.execute("INSERT INTO rs_test (val) VALUES ('2024-01-15 10:30:00')");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNotNull(result, "Result should not be null");
            assertTrue(
                    new String(result).contains("2024-01-15"),
                    "Timestamp should contain 2024-01-15");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_SmallIntColumn() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val SMALLINT)");
            stmt.execute("INSERT INTO rs_test (val) VALUES (123)");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNotNull(result, "Result should not be null");
            assertEquals("123", new String(result), "SmallInt value should be 123");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    // ============================================================
    // stringToCobol tests
    // ============================================================

    @Test
    void testStringToCobol_NullField() {
        assertDoesNotThrow(
                () -> CobolDataConverter.stringToCobol(null, "42".getBytes()),
                "stringToCobol with null field should not throw");
    }

    @Test
    void testStringToCobol_NullData() {
        byte[] storage = new byte[4];
        AbstractCobolField field =
                makeField(4, storage, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        assertDoesNotThrow(
                () -> CobolDataConverter.stringToCobol(field, null),
                "stringToCobol with null data should not throw");
    }

    @Test
    void testStringToCobol_UnsignedNumeric() {
        byte[] storage = new byte[4];
        AbstractCobolField field =
                makeField(4, storage, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        CobolDataConverter.stringToCobol(field, "42".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 4);
        assertEquals("0042", new String(result), "Unsigned numeric should be zero-padded");
    }

    @Test
    void testStringToCobol_Alphanumeric_Short() {
        byte[] storage = new byte[10];
        AbstractCobolField field =
                makeField(10, storage, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.stringToCobol(field, "Hi".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 10);
        assertEquals("Hi        ", new String(result), "Short alphanumeric should be space-padded");
    }

    @Test
    void testStringToCobol_Alphanumeric_Exact() {
        byte[] storage = new byte[5];
        AbstractCobolField field =
                makeField(5, storage, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.stringToCobol(field, "Hello".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 5);
        assertEquals("Hello", new String(result), "Exact-length alphanumeric");
    }

    // ============================================================
    // Additional stringToCobol tests for coverage
    // ============================================================

    @Test
    void testStringToCobol_SignedTrailingCombined() {
        byte[] storage = new byte[4];
        AbstractCobolField field =
                makeField(
                        4,
                        storage,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        CobolDataConverter.stringToCobol(field, "-123".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 4);
        // Last byte should have overpunch sign (0x40 added)
        assertEquals((byte) 0x73, result[3], "Last byte should have overpunch sign for negative");
    }

    @Test
    void testStringToCobol_SignedTrailingSeparate() {
        byte[] storage = new byte[5];
        AbstractCobolField field =
                makeField(
                        5,
                        storage,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE);
        CobolDataConverter.stringToCobol(field, "-42".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 5);
        assertEquals('-', (char) result[4], "Sign byte should be minus");
    }

    @Test
    void testStringToCobol_SignedTrailingSeparate_Positive() {
        byte[] storage = new byte[5];
        AbstractCobolField field =
                makeField(
                        5,
                        storage,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE);
        CobolDataConverter.stringToCobol(field, "42".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 5);
        assertEquals('+', (char) result[4], "Sign byte should be plus for positive");
    }

    @Test
    void testStringToCobol_SignedLeadingSeparate() {
        byte[] storage = new byte[5];
        AbstractCobolField field =
                makeField(
                        4,
                        storage,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        CobolDataConverter.stringToCobol(field, "-99".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 5);
        assertEquals('-', (char) result[0], "Leading sign byte should be minus");
    }

    @Test
    void testStringToCobol_SignedLeadingCombined() {
        byte[] storage = new byte[4];
        AbstractCobolField field =
                makeField(
                        4,
                        storage,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        CobolDataConverter.stringToCobol(field, "-42".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 4);
        // First byte should have overpunch sign
        assertTrue(
                (result[0] & 0xFF) >= 0x70, "First byte should have overpunch sign for negative");
    }

    @Test
    void testStringToCobol_SignedPacked() {
        byte[] storage = new byte[3];
        AbstractCobolField field =
                makeField(
                        3,
                        storage,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        CobolDataConverter.stringToCobol(field, "-1234".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 3);
        // Last nibble should be 0x0D for negative
        assertEquals(0x0D, result[2] & 0x0F, "Last nibble should be 0x0D for negative");
    }

    @Test
    void testStringToCobol_UnsignedPacked() {
        byte[] storage = new byte[2];
        AbstractCobolField field =
                makeField(2, storage, CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 3, 0, 0);
        CobolDataConverter.stringToCobol(field, "123".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 2);
        // Last nibble should be 0x0F for unsigned
        assertEquals(0x0F, result[1] & 0x0F, "Last nibble should be 0x0F for unsigned");
    }

    @Test
    void testStringToCobolRaw_NullField() {
        assertDoesNotThrow(
                () ->
                        CobolDataConverter.stringToCobolRaw(
                                null, new CobolDataStorage(10), 10, "42".getBytes()),
                "stringToCobolRaw with null field should not throw");
    }

    @Test
    void testStringToCobolRaw_NullStorage() {
        byte[] data = new byte[4];
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        assertDoesNotThrow(
                () -> CobolDataConverter.stringToCobolRaw(field, null, 4, "42".getBytes()),
                "stringToCobolRaw with null storage should not throw");
    }

    @Test
    void testStringToCobolRaw_NullData() {
        byte[] data = new byte[4];
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        assertDoesNotThrow(
                () -> CobolDataConverter.stringToCobolRaw(field, new CobolDataStorage(10), 4, null),
                "stringToCobolRaw with null data should not throw");
    }

    @Test
    void testStringToCobolRaw_UnsignedNumeric() {
        byte[] data = new byte[4];
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        CobolDataStorage target = new CobolDataStorage(10);
        CobolDataConverter.stringToCobolRaw(field, target, 4, "42".getBytes());
        byte[] result = target.getByteArray(0, 4);
        assertEquals("0042", new String(result), "Raw unsigned numeric should be zero-padded");
    }

    // ============================================================
    // Additional cobolToString tests for coverage
    // ============================================================

    @Test
    void testCobolToString_UnsignedPacked_OddDigits() {
        // PIC 9(3) COMP-3 = 123 => 0x12 0x3F
        byte[] data = new byte[] {(byte) 0x12, (byte) 0x3F};
        AbstractCobolField field =
                makeField(2, data, CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 3, 0, 0);
        assertEquals("123", CobolDataConverter.cobolToString(field), "Unsigned packed odd digits");
    }

    @Test
    void testCobolToString_UnsignedPacked_EvenDigits() {
        // PIC 9(4) COMP-3 = 1234 => 0x01 0x23 0x4F
        byte[] data = new byte[] {(byte) 0x01, (byte) 0x23, (byte) 0x4F};
        AbstractCobolField field =
                makeField(3, data, CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 4, 0, 0);
        assertEquals(
                "1234", CobolDataConverter.cobolToString(field), "Unsigned packed even digits");
    }

    @Test
    void testCobolToString_SignedPacked_WithScale() {
        // PIC S9(3)V99 COMP-3 VALUE 123.45 => digits: 12345, packed: 0x12 0x34 0x5C
        byte[] data = new byte[] {(byte) 0x12, (byte) 0x34, (byte) 0x5C};
        AbstractCobolField field =
                makeField(
                        3,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("123.45", result, "Signed packed with scale");
    }

    @Test
    void testCobolToString_UnsignedBinary_1Byte() {
        byte[] data = new byte[] {(byte) 42};
        AbstractCobolField field =
                makeField(1, data, CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 3, 0, 0);
        assertEquals("42", CobolDataConverter.cobolToString(field), "Unsigned binary 1 byte");
    }

    @Test
    void testCobolToString_UnsignedBinary_4Bytes() {
        byte[] data = new byte[4];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putInt(65536);
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 9, 0, 0);
        assertEquals("65536", CobolDataConverter.cobolToString(field), "Unsigned binary 4 bytes");
    }

    @Test
    void testCobolToString_UnsignedBinary_8Bytes() {
        byte[] data = new byte[8];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putLong(123456789L);
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 18, 0, 0);
        assertEquals(
                "123456789", CobolDataConverter.cobolToString(field), "Unsigned binary 8 bytes");
    }

    @Test
    void testCobolToString_SignedBinary_1Byte() {
        byte[] data = new byte[] {(byte) -42};
        AbstractCobolField field =
                makeField(
                        1,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        3,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        assertEquals("-42", CobolDataConverter.cobolToString(field), "Signed binary 1 byte");
    }

    @Test
    void testCobolToString_SignedBinary_2Bytes() {
        byte[] data = new byte[2];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putShort((short) -1234);
        AbstractCobolField field =
                makeField(
                        2,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        assertEquals("-1234", CobolDataConverter.cobolToString(field), "Signed binary 2 bytes");
    }

    @Test
    void testCobolToString_SignedBinary_8Bytes() {
        byte[] data = new byte[8];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putLong(-9999L);
        AbstractCobolField field =
                makeField(
                        8,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        assertEquals("-9999", CobolDataConverter.cobolToString(field), "Signed binary 8 bytes");
    }

    @Test
    void testCobolToString_SignedBinary_WithScale() {
        byte[] data = new byte[4];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putInt(12345);
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        9,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("123.45", result, "Signed binary with scale");
    }

    @Test
    void testCobolToString_SignedBinary_ScaleExceedsDigits() {
        byte[] data = new byte[2];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putShort((short) 5);
        AbstractCobolField field =
                makeField(
                        2,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        4,
                        4,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("0.0005", result, "Binary with scale exceeding digits");
    }

    @Test
    void testCobolToString_Alphabetic() {
        byte[] data = "Test  ".getBytes();
        AbstractCobolField field =
                makeField(6, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        assertEquals(
                "Test",
                CobolDataConverter.cobolToString(field),
                "Alphabetic should trim trailing spaces");
    }

    @Test
    void testCobolToString_Alphabetic_AllSpaces() {
        byte[] data = "      ".getBytes();
        AbstractCobolField field =
                makeField(6, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        assertEquals(
                "",
                CobolDataConverter.cobolToString(field),
                "All-spaces should produce empty string");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testCobolToString_National() {
        byte[] data = new byte[] {(byte) 0x82, (byte) 0xA0}; // Shift-JIS char for hiragana 'a'
        AbstractCobolField field =
                makeField(2, data, CobolFieldAttribute.COB_TYPE_NATIONAL, 0, 0, 0);
        String result = CobolDataConverter.cobolToString(field);
        assertNotNull(result, "National field result should not be null");
        assertFalse(
                result.isEmpty(), "National field conversion should produce a non-empty string");
        assertEquals(1, result.length(), "Shift-JIS 0x82A0 should decode to a single character");
    }

    @Test
    void testCobolToString_AlphanumericVarying() {
        // Header: 4 bytes for length, then data
        byte[] data = new byte[9]; // 4 header + 5 data
        ByteBuffer.wrap(data, 0, 4).putInt(5);
        System.arraycopy("Hello".getBytes(), 0, data, 4, 5);
        AbstractCobolField field =
                makeField(
                        9,
                        data,
                        CobolFieldAttribute.COB_TYPE_GROUP,
                        0,
                        0,
                        CobolFieldAttribute.COB_FLAG_VARYING);
        assertEquals(
                "Hello",
                CobolDataConverter.cobolToString(field),
                "Varying alphanumeric should extract Hello");
    }

    @Test
    void testCobolToString_UnsignedNumeric_WithScale_V99() {
        // PIC 9(2)V99 = 12.34 stored as "1234"
        byte[] data = "1234".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 2, 0);
        assertEquals(
                "12.34",
                CobolDataConverter.cobolToString(field),
                "Unsigned numeric V99 should insert decimal");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testCobolToString_SignedTrailingCombined_WithScale() {
        byte[] data = new byte[] {'1', '2', '3', (byte) 0x74};
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertTrue(
                result.startsWith("-"), "Signed trailing combined with scale should be negative");
        assertTrue(
                result.contains("."), "Signed trailing combined with scale should contain decimal");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testCobolToString_SignedTrailingSeparate_WithScale() {
        byte[] data = "1234-".getBytes();
        AbstractCobolField field =
                makeField(
                        5,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE);
        String result = CobolDataConverter.cobolToString(field);
        assertTrue(
                result.startsWith("-"), "Signed trailing separate with scale should be negative");
        assertTrue(
                result.contains("."), "Signed trailing separate with scale should contain decimal");
    }

    @Test
    void testCobolToString_SignedLeadingSeparate_WithScale() {
        byte[] data = "-1234".getBytes();
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        String result = CobolDataConverter.cobolToString(field);
        assertTrue(
                result.contains("."), "Signed leading separate with scale should contain decimal");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testCobolToString_SignedLeadingCombined_WithScale() {
        byte[] data = new byte[] {(byte) 0x71, '2', '3', '4'};
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        String result = CobolDataConverter.cobolToString(field);
        assertTrue(result.startsWith("-"), "Signed leading combined with scale should be negative");
        assertTrue(
                result.contains("."), "Signed leading combined with scale should contain decimal");
    }

    // ============================================================
    // Additional stringToCobol with scale coverage
    // ============================================================

    @Test
    void testStringToCobol_UnsignedNumeric_WithScale() {
        byte[] storage = new byte[6];
        AbstractCobolField field =
                makeField(6, storage, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 6, 2, 0);
        CobolDataConverter.stringToCobol(field, "12.34".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 6);
        assertEquals("001234", new String(result), "Unsigned numeric with scale");
    }

    @Test
    void testStringToCobol_SignedTrailingCombined_WithScale() {
        byte[] storage = new byte[4];
        AbstractCobolField field =
                makeField(
                        4,
                        storage,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        CobolDataConverter.stringToCobol(field, "-12.34".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 4);
        // Last byte should have overpunch
        assertTrue(
                (result[3] & 0xFF) >= 0x70,
                "Last byte should have overpunch for signed trailing combined with scale");
    }

    @Test
    void testStringToCobol_SignedPacked_WithScale() {
        byte[] storage = new byte[3];
        AbstractCobolField field =
                makeField(
                        3,
                        storage,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        CobolDataConverter.stringToCobol(field, "123.45".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 3);
        assertEquals(0x0C, result[2] & 0x0F, "Positive sign nibble should be 0x0C");
    }

    @Test
    void testStringToCobol_AlphanumericVarying() {
        byte[] storage = new byte[14]; // 4 header + 10 data
        AbstractCobolField field =
                makeField(
                        14,
                        storage,
                        CobolFieldAttribute.COB_TYPE_GROUP,
                        0,
                        0,
                        CobolFieldAttribute.COB_FLAG_VARYING);
        CobolDataConverter.stringToCobol(field, "Hi".getBytes());
        int len = ByteBuffer.wrap(field.getDataStorage().getByteArray(0, 4)).getInt();
        assertEquals(2, len, "Varying length header should be 2");
    }

    @Test
    void testStringToCobol_National() {
        byte[] storage = new byte[10];
        AbstractCobolField field =
                makeField(10, storage, CobolFieldAttribute.COB_TYPE_NATIONAL, 0, 0, 0);
        CobolDataConverter.stringToCobol(field, "AB".getBytes());
        // Should not throw
        assertNotNull(
                field.getDataStorage(), "Storage should not be null after national conversion");
    }

    // ============================================================
    // getValueFromResultSet with Time column
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_TimeColumn() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val TIME)");
            stmt.execute("INSERT INTO rs_test (val) VALUES ('10:30:00')");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNotNull(result, "Result should not be null");
            assertTrue(new String(result).contains("10:30:00"), "Time should contain 10:30:00");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_NullDecimal() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val DECIMAL(10,2))");
            stmt.execute("INSERT INTO rs_test (val) VALUES (NULL)");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNull(result, "NULL decimal should return null");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_NullTimestamp() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val TIMESTAMP)");
            stmt.execute("INSERT INTO rs_test (val) VALUES (NULL)");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNull(result, "NULL timestamp should return null");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_NullDate() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val DATE)");
            stmt.execute("INSERT INTO rs_test (val) VALUES (NULL)");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNull(result, "NULL date should return null");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_NullTime() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val TIME)");
            stmt.execute("INSERT INTO rs_test (val) VALUES (NULL)");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNull(result, "NULL time should return null");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_CharColumn() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val CHAR(5))");
            stmt.execute("INSERT INTO rs_test (val) VALUES ('ABC')");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNotNull(result, "Result should not be null");
            // CHAR pads with spaces
            assertTrue(new String(result).startsWith("ABC"), "CHAR value should start with ABC");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testGetValueFromResultSet_RealColumn() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS rs_test");
            stmt.execute("CREATE TABLE rs_test (val REAL)");
            stmt.execute("INSERT INTO rs_test (val) VALUES (1.5)");
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM rs_test")) {
            assertTrue(rs.next(), "Should have a row");
            byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
            assertNotNull(result, "Result should not be null");
            double val = Double.parseDouble(new String(result));
            assertEquals(1.5, val, 0.01, "Real value should be 1.5");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE rs_test");
        }
    }

    // ============================================================
    // setParam with various types via real DB
    // ============================================================

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetParam_BooleanType() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (val BOOLEAN)");
        }

        // Use alphanumeric field with "true" value
        byte[] data = "true      ".getBytes();
        AbstractCobolField field =
                makeField(10, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (val) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, ps.getParameterMetaData(), field);
            ps.execute();
        }

        // Verify the inserted value
        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM param_test")) {
            assertTrue(rs.next(), "Should have a row");
            assertTrue(rs.getBoolean(1), "Boolean value should be true");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetParam_BigintType() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (val BIGINT)");
        }

        byte[] data = "9999999".getBytes();
        AbstractCobolField field =
                makeField(7, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 7, 0, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (val) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, ps.getParameterMetaData(), field);
            ps.execute();
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM param_test")) {
            assertTrue(rs.next(), "Should have a row");
            assertEquals(9999999L, rs.getLong(1), "BigInt value should be 9999999");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetParam_DoubleType() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (val DOUBLE PRECISION)");
        }

        byte[] data = "314500".getBytes();
        AbstractCobolField field =
                makeField(6, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 6, 2, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (val) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, ps.getParameterMetaData(), field);
            ps.execute();
        }

        // Verify the inserted value
        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM param_test")) {
            assertTrue(rs.next(), "Should have a row");
            assertEquals(3145.0, rs.getDouble(1), 0.01, "Double value should be 3145.0");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetParam_RealType() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (val REAL)");
        }

        byte[] data = "015000".getBytes();
        AbstractCobolField field =
                makeField(6, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 6, 2, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (val) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, ps.getParameterMetaData(), field);
            ps.execute();
        }

        // Verify the inserted value
        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM param_test")) {
            assertTrue(rs.next(), "Should have a row");
            assertEquals(150.0, rs.getFloat(1), 0.01, "Real value should be 150.0");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetParam_DateType() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (val DATE)");
        }

        byte[] data = "2024-01-15".getBytes();
        AbstractCobolField field =
                makeField(10, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (val) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, ps.getParameterMetaData(), field);
            ps.execute();
        }

        // Verify the inserted value
        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM param_test")) {
            assertTrue(rs.next(), "Should have a row");
            assertEquals("2024-01-15", rs.getDate(1).toString(), "Date should be 2024-01-15");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetParam_TimestampType() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (val TIMESTAMP)");
        }

        byte[] data = "2024-01-15 10:30:00".getBytes();
        AbstractCobolField field =
                makeField(19, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (val) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, ps.getParameterMetaData(), field);
            ps.execute();
        }

        // Verify the inserted value
        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM param_test")) {
            assertTrue(rs.next(), "Should have a row");
            Timestamp ts = rs.getTimestamp(1);
            assertTrue(
                    ts.toString().startsWith("2024-01-15 10:30:00"),
                    "Timestamp should start with 2024-01-15 10:30:00");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetParam_SmallintType() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (val SMALLINT)");
        }

        byte[] data = "0042".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (val) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, ps.getParameterMetaData(), field);
            ps.execute();
        }

        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM param_test")) {
            assertTrue(rs.next(), "Should have a row");
            assertEquals(42, rs.getShort(1), "SmallInt value should be 42");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestsShouldIncludeAssert")
    void testSetParam_NumericType() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (val NUMERIC(10,2))");
        }

        byte[] data = "123456".getBytes();
        AbstractCobolField field =
                makeField(6, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 6, 2, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (val) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, ps.getParameterMetaData(), field);
            ps.execute();
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testSetParam_TimeType() throws Exception {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS param_test");
            stmt.execute("CREATE TABLE param_test (val TIME)");
        }

        byte[] data = "10:30:00  ".getBytes();
        AbstractCobolField field =
                makeField(10, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);

        try (PreparedStatement ps =
                conn.prepareStatement("INSERT INTO param_test (val) VALUES (?)")) {
            CobolDataConverter.setParam(ps, 1, ps.getParameterMetaData(), field);
            ps.execute();
        }

        // Verify the inserted value
        try (Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT val FROM param_test")) {
            assertTrue(rs.next(), "Should have a row");
            assertEquals("10:30:00", rs.getTime(1).toString(), "Time should be 10:30:00");
        }

        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE param_test");
        }
    }

    // ============================================================
    // Additional coverage: stringToCobol for scale branches
    // ============================================================

    @Test
    void testStringToCobol_UnsignedNumeric_NegativeScale() {
        // PIC 9(4)V99 with scale=2 (stored as -2 internally)
        byte[] storage = new byte[6];
        AbstractCobolField field =
                makeField(6, storage, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 6, 2, 0);
        CobolDataConverter.stringToCobol(field, "1234.56".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 6);
        assertEquals("123456", new String(result), "Scale should remove decimal point");
    }

    @Test
    void testStringToCobol_SignedTrailingSeparate_WithScale() {
        byte[] storage = new byte[7]; // 6 digits + sign
        AbstractCobolField field =
                makeField(
                        7,
                        storage,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        6,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE);
        CobolDataConverter.stringToCobol(field, "-12.34".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 7);
        assertEquals(
                '-', (char) result[6], "Trailing sign should be minus for negative with scale");
    }

    @Test
    void testStringToCobol_SignedLeadingSeparate_WithScale() {
        byte[] storage = new byte[5]; // sign + 4 digits
        AbstractCobolField field =
                makeField(
                        4,
                        storage,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        CobolDataConverter.stringToCobol(field, "-12.34".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 5);
        assertEquals('-', (char) result[0], "Leading sign should be minus for negative with scale");
    }

    @Test
    void testStringToCobol_SignedLeadingCombined_WithScale() {
        byte[] storage = new byte[4];
        AbstractCobolField field =
                makeField(
                        4,
                        storage,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        CobolDataConverter.stringToCobol(field, "-12.34".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 4);
        // First byte should have overpunch
        assertTrue(
                (result[0] & 0xFF) >= 0x70,
                "Leading combined overpunch should be present with scale");
    }

    @Test
    void testStringToCobol_SignedPacked_Positive() {
        byte[] storage = new byte[3];
        AbstractCobolField field =
                makeField(
                        3,
                        storage,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        CobolDataConverter.stringToCobol(field, "1234".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 3);
        assertEquals(0x0C, result[2] & 0x0F, "Positive sign nibble should be 0x0C");
    }

    @Test
    void testStringToCobol_UnsignedPacked_EvenDigits() {
        byte[] storage = new byte[3];
        AbstractCobolField field =
                makeField(3, storage, CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 4, 0, 0);
        CobolDataConverter.stringToCobol(field, "1234".getBytes());
        byte[] result = field.getDataStorage().getByteArray(0, 3);
        assertEquals(0x0F, result[2] & 0x0F, "Unsigned sign nibble should be 0x0F");
    }

    @Test
    void testStringToCobol_Binary_Write() {
        // Binary fields use writeAlphanumeric internally
        byte[] storage = new byte[4];
        AbstractCobolField field =
                makeField(4, storage, CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 9, 0, 0);
        CobolDataConverter.stringToCobol(field, "42".getBytes());
        assertNotNull(field.getDataStorage(), "Storage should not be null after binary write");
    }

    @Test
    void testStringToCobol_Float_Write() {
        byte[] storage = new byte[8];
        AbstractCobolField field =
                makeField(8, storage, CobolFieldAttribute.COB_TYPE_NUMERIC_DOUBLE, 0, 0, 0);
        CobolDataConverter.stringToCobol(field, "2.75".getBytes());
        assertNotNull(field.getDataStorage(), "Storage should not be null after float write");
    }

    @Test
    void testStringToCobol_JapaneseVarying() {
        byte[] storage = new byte[14]; // 4 header + 10 data
        AbstractCobolField field =
                makeField(
                        14,
                        storage,
                        CobolFieldAttribute.COB_TYPE_GROUP,
                        0,
                        0,
                        CobolFieldAttribute.COB_FLAG_VARYING
                                | CobolFieldAttribute.COB_FLAG_NATIONAL_VARYING);
        CobolDataConverter.stringToCobol(field, new byte[] {(byte) 0x82, (byte) 0xA0});
        assertNotNull(
                field.getDataStorage(), "Storage should not be null after Japanese varying write");
    }

    @Test
    void testCobolToString_JapaneseVarying() {
        byte[] data = new byte[8]; // 4 header + 4 data (2 chars of 2 bytes)
        ByteBuffer.wrap(data, 0, 4).putInt(2); // 2 characters
        data[4] = (byte) 0x82;
        data[5] = (byte) 0xA0;
        data[6] = (byte) 0x82;
        data[7] = (byte) 0xA2;
        AbstractCobolField field =
                makeField(
                        8,
                        data,
                        CobolFieldAttribute.COB_TYPE_GROUP,
                        0,
                        0,
                        CobolFieldAttribute.COB_FLAG_VARYING
                                | CobolFieldAttribute.COB_FLAG_NATIONAL_VARYING);
        String result = CobolDataConverter.cobolToString(field);
        assertNotNull(result, "Japanese varying result should not be null");
    }

    @Test
    void testCobolToString_Group() {
        byte[] data = "Hello     ".getBytes();
        AbstractCobolField field = makeField(10, data, CobolFieldAttribute.COB_TYPE_GROUP, 0, 0, 0);
        assertEquals(
                "Hello",
                CobolDataConverter.cobolToString(field),
                "Group type should trim trailing spaces");
    }

    @Test
    void testCobolToString_SignedPacked_PP() {
        // PIC S9(3)PP COMP-3 (scale = -2, digits = 5, stored digits = 3)
        // Value stored: 123, representing 12300
        byte[] data = new byte[] {(byte) 0x12, (byte) 0x3C};
        AbstractCobolField field =
                makeField(
                        2,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        -2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("12300", result, "Signed packed PP should produce 12300");
    }

    @Test
    @SuppressWarnings("PMD.JUnitTestContainsTooManyAsserts")
    void testCobolToString_SignedPacked_ScaleExceedsLength() {
        // PIC S9V9999 COMP-3 (scale = 4, digits = 5, 5 digits stored)
        // This exercises the -scale > length branch
        byte[] data = new byte[] {(byte) 0x00, (byte) 0x12, (byte) 0x3C};
        AbstractCobolField field =
                makeField(
                        3,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        4,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertNotNull(result, "Result should not be null");
        assertTrue(result.contains("."), "Result should contain decimal point");
    }

    @Test
    void testStringToCobolRaw_Packed_PP() {
        // PIC S9(3)PP COMP-3
        byte[] data = new byte[4];
        AbstractCobolField field =
                makeField(
                        2,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        -2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        CobolDataStorage target = new CobolDataStorage(10);
        CobolDataConverter.stringToCobolRaw(field, target, 2, "12300".getBytes());
        assertNotNull(target, "Target storage should not be null after packed PP write");
    }

    @Test
    void testCobolToString_SignedLeadingSeparate_NoScale() {
        byte[] data = "+0042".getBytes();
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals(
                "42", result, "Signed leading separate no scale (sign dropped, zeros stripped)");
    }

    @Test
    void testCobolToString_NullStorage() {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(10, (CobolDataStorage) null, attr);
        assertEquals(
                "",
                CobolDataConverter.cobolToString(field),
                "Null storage should produce empty string");
    }
}

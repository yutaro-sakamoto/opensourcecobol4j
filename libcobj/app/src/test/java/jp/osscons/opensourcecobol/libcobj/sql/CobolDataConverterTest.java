package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import java.io.InputStream;
import java.io.Reader;
import java.math.BigDecimal;
import java.net.URL;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.sql.*;
import java.util.Calendar;
import java.util.Map;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory;
import org.junit.jupiter.api.Test;

class CobolDataConverterTest {

    // ============================================================
    // Helper to create COBOL fields for testing
    // ============================================================

    /**
     * Create an AbstractCobolField with the given attributes and storage data.
     *
     * @param size field byte size
     * @param data raw byte data
     * @param type CobolFieldAttribute type constant
     * @param digits number of digits
     * @param scale scale value
     * @param flags attribute flags
     */
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
        // PIC 9(4) VALUE 1234
        byte[] data = "1234".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        assertEquals("1234", CobolDataConverter.cobolToString(field));
    }

    @Test
    void testCobolToString_UnsignedNumeric_WithScale() {
        // PIC 9(4)V99 => scale=2, so digits stored: "123456" representing 1234.56
        byte[] data = "123456".getBytes();
        AbstractCobolField field =
                makeField(6, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 6, 2, 0);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("1234.56", result);
    }

    @Test
    void testCobolToString_UnsignedNumeric_LeadingZeros() {
        byte[] data = "0012".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        assertEquals("12", CobolDataConverter.cobolToString(field));
    }

    @Test
    void testCobolToString_UnsignedNumeric_AllZeros() {
        byte[] data = "0000".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        assertEquals("0", CobolDataConverter.cobolToString(field));
    }

    // ============================================================
    // cobolToString: SIGNED TRAILING COMBINED
    // ============================================================

    @Test
    void testCobolToString_SignedTrailingCombined_Positive() {
        // Positive: last byte 0x30..0x39 ('0'..'9')
        byte[] data = "1234".getBytes();
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("1234", result);
    }

    @Test
    void testCobolToString_SignedTrailingCombined_Negative() {
        // Negative: last byte in 0x70..0x79 range
        byte[] data = "123".getBytes();
        byte[] fullData = new byte[4];
        System.arraycopy(data, 0, fullData, 0, 3);
        fullData[3] = (byte) 0x74; // overpunch for -4
        AbstractCobolField field =
                makeField(
                        4,
                        fullData,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("-1234", result);
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
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("1234", result);
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
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("-1234", result);
    }

    // ============================================================
    // cobolToString: SIGNED LEADING SEPARATE
    // ============================================================

    @Test
    void testCobolToString_SignedLeadingSeparate_Positive() {
        // readSignedLeadingSeparate reads length+1 bytes from storage.
        // field size=5 (sign + 4 digits), so storage needs 6 bytes.
        byte[] data = "+12340".getBytes(); // 6 bytes, last byte not used for 4-digit field
        AbstractCobolField field =
                makeField(
                        5,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("+12340", result);
    }

    @Test
    void testCobolToString_SignedLeadingSeparate_Negative() {
        byte[] data = "-12340".getBytes(); // 6 bytes
        AbstractCobolField field =
                makeField(
                        5,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("-12340", result);
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
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("1234", result);
    }

    @Test
    void testCobolToString_SignedLeadingCombined_Negative() {
        byte[] data = new byte[4];
        data[0] = (byte) 0x71; // overpunch for negative 1
        data[1] = '2';
        data[2] = '3';
        data[3] = '4';
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("-1234", result);
    }

    // ============================================================
    // cobolToString: UNSIGNED PACKED (COMP-3, unsigned)
    // ============================================================

    @Test
    void testCobolToString_UnsignedPacked_OddDigits() {
        // PIC 9(5) COMP-3: 5 digits => 3 bytes: 01 23 4F
        byte[] data = new byte[] {0x01, 0x23, 0x4F};
        AbstractCobolField field =
                makeField(3, data, CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 5, 0, 0);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("1234", result);
    }

    @Test
    void testCobolToString_UnsignedPacked_EvenDigits() {
        // PIC 9(4) COMP-3: 4 digits => 3 bytes: 01 23 4F
        // Even: first byte low nibble = first digit
        byte[] data = new byte[] {0x01, 0x23, 0x4F};
        AbstractCobolField field =
                makeField(3, data, CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 4, 0, 0);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("1234", result);
    }

    // ============================================================
    // cobolToString: SIGNED PACKED (COMP-3, signed)
    // ============================================================

    @Test
    void testCobolToString_SignedPacked_Positive() {
        // PIC S9(5) COMP-3: 01 23 4C (C=positive)
        byte[] data = new byte[] {0x01, 0x23, 0x4C};
        AbstractCobolField field =
                makeField(
                        3,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("1234", result);
    }

    @Test
    void testCobolToString_SignedPacked_Negative() {
        // PIC S9(5) COMP-3: 01 23 4D (D=negative)
        byte[] data = new byte[] {0x01, 0x23, 0x4D};
        AbstractCobolField field =
                makeField(
                        3,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("-1234", result);
    }

    @Test
    void testCobolToString_SignedPacked_WithScale() {
        // PIC S9(3)V99 COMP-3: 5 digits, scale=2 => 01 23 4C representing 123.4x
        byte[] data = new byte[] {0x01, 0x23, 0x4C};
        AbstractCobolField field =
                makeField(
                        3,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("12.34", result);
    }

    @Test
    void testCobolToString_SignedPacked_PP() {
        // PP = negative scale, e.g. PIC S9(3)PP COMP-3: digits=5, scale=-2
        // Only 3 stored digits, PP appends 2 zeros
        byte[] data = new byte[] {0x01, 0x2C}; // 12C => +12, then append "00" => 1200
        AbstractCobolField field =
                makeField(
                        2,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        -2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("1200", result);
    }

    // ============================================================
    // cobolToString: UNSIGNED BINARY NATIVE
    // ============================================================

    @Test
    void testCobolToString_UnsignedBinary_1Byte() {
        byte[] data = new byte[] {(byte) 0xFF};
        AbstractCobolField field =
                makeField(1, data, CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 3, 0, 0);
        assertEquals("255", CobolDataConverter.cobolToString(field));
    }

    @Test
    void testCobolToString_UnsignedBinary_2Bytes() {
        byte[] data = new byte[2];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putShort((short) 1000);
        AbstractCobolField field =
                makeField(2, data, CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 5, 0, 0);
        assertEquals("1000", CobolDataConverter.cobolToString(field));
    }

    @Test
    void testCobolToString_UnsignedBinary_4Bytes() {
        byte[] data = new byte[4];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putInt(123456);
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 9, 0, 0);
        assertEquals("123456", CobolDataConverter.cobolToString(field));
    }

    @Test
    void testCobolToString_UnsignedBinary_8Bytes() {
        byte[] data = new byte[8];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putLong(9876543210L);
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 18, 0, 0);
        assertEquals("9876543210", CobolDataConverter.cobolToString(field));
    }

    // ============================================================
    // cobolToString: SIGNED BINARY NATIVE
    // ============================================================

    @Test
    void testCobolToString_SignedBinary_Negative() {
        byte[] data = new byte[4];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putInt(-500);
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        9,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        assertEquals("-500", CobolDataConverter.cobolToString(field));
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
        assertEquals("123.45", result);
    }

    @Test
    void testCobolToString_SignedBinary_1Byte() {
        byte[] data = new byte[] {(byte) -5};
        AbstractCobolField field =
                makeField(
                        1,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        3,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        assertEquals("-5", CobolDataConverter.cobolToString(field));
    }

    @Test
    void testCobolToString_SignedBinary_2Bytes() {
        byte[] data = new byte[2];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putShort((short) -1000);
        AbstractCobolField field =
                makeField(
                        2,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        5,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        assertEquals("-1000", CobolDataConverter.cobolToString(field));
    }

    @Test
    void testCobolToString_SignedBinary_8Bytes() {
        byte[] data = new byte[8];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putLong(-9876543210L);
        AbstractCobolField field =
                makeField(
                        8,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        assertEquals("-9876543210", CobolDataConverter.cobolToString(field));
    }

    @Test
    void testCobolToString_SignedBinary_ScaleExceedsDigits() {
        // value=5, scale=2 => "0.05"
        byte[] data = new byte[4];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putInt(5);
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        9,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("0.05", result);
    }

    // ============================================================
    // cobolToString: ALPHABETIC
    // ============================================================

    @Test
    void testCobolToString_Alphabetic() {
        byte[] data = "Hello   ".getBytes();
        // COB_TYPE_ALPHANUMERIC maps to TYPE_ALPHABETIC in the default branch
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("Hello", result);
    }

    @Test
    void testCobolToString_Alphabetic_AllSpaces() {
        byte[] data = "    ".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        assertEquals("", CobolDataConverter.cobolToString(field));
    }

    // ============================================================
    // cobolToString: FLOAT
    // ============================================================

    @Test
    void testCobolToString_Float() {
        byte[] data = new byte[8];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putDouble(3.14);
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DOUBLE, 0, 0, 0);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("3.14", result);
    }

    // ============================================================
    // cobolToString: NATIONAL
    // ============================================================

    @Test
    void testCobolToString_National() {
        byte[] data = "ABC".getBytes();
        AbstractCobolField field =
                makeField(3, data, CobolFieldAttribute.COB_TYPE_NATIONAL, 0, 0, 0);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("ABC", result);
    }

    // ============================================================
    // cobolToString: ALPHANUMERIC VARYING
    // ============================================================

    @Test
    void testCobolToString_AlphanumericVarying() {
        // 4-byte header (big-endian length) + data
        byte[] data = new byte[9]; // 4 header + 5 max
        ByteBuffer.wrap(data).putInt(3); // actual length = 3
        data[4] = 'A';
        data[5] = 'B';
        data[6] = 'C';
        AbstractCobolField field =
                makeField(
                        9,
                        data,
                        CobolFieldAttribute.COB_TYPE_GROUP,
                        0,
                        0,
                        CobolFieldAttribute.COB_FLAG_VARYING);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("ABC", result);
    }

    // ============================================================
    // cobolToString: null / empty
    // ============================================================

    @Test
    void testCobolToString_NullField() {
        assertEquals("", CobolDataConverter.cobolToString(null));
    }

    // ============================================================
    // stringToCobol tests
    // ============================================================

    @Test
    void testStringToCobol_UnsignedNumeric() {
        byte[] data = new byte[4];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(4, storage, attr);
        CobolDataConverter.stringToCobol(field, "42".getBytes());
        assertEquals('0', (char) data[0]);
        assertEquals('0', (char) data[1]);
        assertEquals('4', (char) data[2]);
        assertEquals('2', (char) data[3]);
    }

    @Test
    void testStringToCobol_SignedTrailingCombined() {
        byte[] data = new byte[4];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(4, storage, attr);
        CobolDataConverter.stringToCobol(field, "-42".getBytes());
        // Last byte should have overpunch (0x40 added)
        assertTrue((data[3] & 0xFF) >= 0x70);
    }

    @Test
    void testStringToCobol_NullField() {
        assertDoesNotThrow(() -> CobolDataConverter.stringToCobol(null, "test".getBytes()));
    }

    @Test
    void testStringToCobol_NullData() {
        byte[] storage = new byte[4];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, new CobolDataStorage(storage), attr);
        assertDoesNotThrow(() -> CobolDataConverter.stringToCobol(field, null));
    }

    @Test
    void testStringToCobol_SignedPacked() {
        // PIC S9(5) COMP-3: 3 bytes
        byte[] data = new byte[3];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(3, storage, attr);
        CobolDataConverter.stringToCobol(field, "12345".getBytes());
        // Sign nibble should be 0x0C (positive)
        assertEquals(0x0C, data[2] & 0x0F);
    }

    @Test
    void testStringToCobol_UnsignedPacked() {
        byte[] data = new byte[3];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 5, 0, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(3, storage, attr);
        CobolDataConverter.stringToCobol(field, "12345".getBytes());
        assertEquals(0x0F, data[2] & 0x0F);
    }

    @Test
    void testStringToCobol_Alphabetic() {
        byte[] data = new byte[10];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(10, storage, attr);
        CobolDataConverter.stringToCobol(field, "Hi".getBytes());
        assertEquals('H', (char) data[0]);
        assertEquals('i', (char) data[1]);
        assertEquals(' ', (char) data[2]); // padded with spaces
    }

    @Test
    void testStringToCobol_SignedTrailingSeparate() {
        byte[] data = new byte[5]; // 4 digits + 1 sign
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(5, storage, attr);
        CobolDataConverter.stringToCobol(field, "-42".getBytes());
        assertEquals('-', (char) data[4]);
    }

    @Test
    void testStringToCobol_SignedLeadingSeparate() {
        byte[] data = new byte[5]; // 1 sign + 4 digits
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(5, storage, attr);
        CobolDataConverter.stringToCobol(field, "+42".getBytes());
        assertEquals('+', (char) data[0]);
    }

    @Test
    void testStringToCobol_SignedLeadingCombined() {
        byte[] data = new byte[4];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(4, storage, attr);
        CobolDataConverter.stringToCobol(field, "-42".getBytes());
        // First byte should have overpunch
        assertTrue((data[0] & 0xFF) >= 0x70);
    }

    @Test
    void testStringToCobol_AlphanumericVarying() {
        byte[] data = new byte[14]; // 4 header + 10 max
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_GROUP,
                        0,
                        0,
                        CobolFieldAttribute.COB_FLAG_VARYING,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(14, storage, attr);
        CobolDataConverter.stringToCobol(field, "Hello".getBytes());
        int storedLen = ByteBuffer.wrap(data, 0, 4).getInt();
        assertEquals(5, storedLen);
        assertEquals('H', (char) data[4]);
    }

    // ============================================================
    // stringToCobolRaw tests
    // ============================================================

    @Test
    void testStringToCobolRaw_NullField() {
        assertDoesNotThrow(
                () ->
                        CobolDataConverter.stringToCobolRaw(
                                null, new CobolDataStorage(10), 10, "test".getBytes()));
    }

    @Test
    void testStringToCobolRaw_NullStorage() {
        byte[] d = new byte[4];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, new CobolDataStorage(d), attr);
        assertDoesNotThrow(
                () -> CobolDataConverter.stringToCobolRaw(field, null, 4, "42".getBytes()));
    }

    @Test
    void testStringToCobolRaw_NullData() {
        byte[] d = new byte[4];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, new CobolDataStorage(d), attr);
        assertDoesNotThrow(
                () -> CobolDataConverter.stringToCobolRaw(field, new CobolDataStorage(4), 4, null));
    }

    @Test
    void testStringToCobolRaw_UnsignedNumeric() {
        byte[] target = new byte[4];
        CobolDataStorage targetStorage = new CobolDataStorage(target);
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, new CobolDataStorage(new byte[4]), attr);
        CobolDataConverter.stringToCobolRaw(field, targetStorage, 4, "99".getBytes());
        assertEquals('9', (char) target[2]);
        assertEquals('9', (char) target[3]);
    }

    // ============================================================
    // setParam tests with mock PreparedStatement
    // ============================================================

    @Test
    void testSetParam_NullMetaData_UsesSetString() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        byte[] data = "1234".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        CobolDataConverter.setParam(mock, 1, null, field);
        assertEquals("1234", mock.lastStringValue);
        assertEquals(1, mock.lastStringIndex);
    }

    @Test
    void testSetParam_StringType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.VARCHAR);
        byte[] data = "Hello   ".getBytes();
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals("Hello", mock.lastStringValue);
    }

    @Test
    void testSetParam_IntegerType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.INTEGER);
        byte[] data = "0042".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals(42, mock.lastIntValue);
    }

    @Test
    void testSetParam_DecimalType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.DECIMAL);
        byte[] data = "0042".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertNotNull(mock.lastBigDecimal);
        assertEquals(new BigDecimal("42"), mock.lastBigDecimal);
    }

    @Test
    void testSetParam_DoubleType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.DOUBLE);
        byte[] data = new byte[8];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putDouble(3.14);
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DOUBLE, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals(3.14, mock.lastDoubleValue, 0.001);
    }

    @Test
    void testSetParam_BooleanType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.BOOLEAN);
        byte[] data = "true    ".getBytes();
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertTrue(mock.lastBoolValue);
    }

    @Test
    void testSetParam_MetaDataThrows_FallsBackToString() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        ParameterMetaData throwingMeta =
                new MockParameterMetaData(Types.INTEGER) {
                    @Override
                    public int getParameterType(int param) throws SQLException {
                        throw new SQLException("unsupported");
                    }
                };
        byte[] data = "42  ".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, throwingMeta, field);
        assertEquals("42", mock.lastStringValue);
    }

    @Test
    void testSetParam_NumericType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.NUMERIC);
        byte[] data = "0042".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals(42.0, mock.lastDoubleValue, 0.001);
    }

    @Test
    void testSetParam_FloatType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.FLOAT);
        byte[] data = "0042".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals(42.0f, mock.lastFloatValue, 0.001f);
    }

    @Test
    void testSetParam_SmallintType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.SMALLINT);
        byte[] data = "0042".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals(42, mock.lastIntValue);
    }

    @Test
    void testSetParam_BigintType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.BIGINT);
        byte[] data = "0042".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals(new BigDecimal("42"), mock.lastBigDecimal);
    }

    @Test
    void testSetParam_DefaultType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.STRUCT);
        byte[] data = "test".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals("test", mock.lastStringValue);
    }

    // ============================================================
    // getValueFromResultSet tests with mock ResultSet
    // ============================================================

    @Test
    void testGetValueFromResultSet_Varchar() throws Exception {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.VARCHAR;
        rs.stringValue = "Hello";
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        // SHIFT_JIS encoding of ASCII is same as ASCII
        assertEquals("Hello", new String(result, "SHIFT-JIS"));
    }

    @Test
    void testGetValueFromResultSet_Varchar_Null() throws Exception {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.VARCHAR;
        rs.stringValue = null;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNull(result);
    }

    @Test
    void testGetValueFromResultSet_Decimal() throws Exception {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.DECIMAL;
        rs.bigDecimalValue = new BigDecimal("123.45");
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("123.45", new String(result));
    }

    @Test
    void testGetValueFromResultSet_Decimal_Null() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.DECIMAL;
        rs.bigDecimalValue = null;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNull(result);
    }

    @Test
    void testGetValueFromResultSet_Integer() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.INTEGER;
        rs.intValue = 42;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("42", new String(result));
    }

    @Test
    void testGetValueFromResultSet_Integer_WasNull() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.INTEGER;
        rs.intValue = 0;
        rs.wasNullFlag = true;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNull(result);
    }

    @Test
    void testGetValueFromResultSet_Boolean() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.BOOLEAN;
        rs.boolValue = true;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("true", new String(result));
    }

    @Test
    void testGetValueFromResultSet_Double() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.DOUBLE;
        rs.doubleValue = 3.14;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("3.14", new String(result));
    }

    @Test
    void testGetValueFromResultSet_Bigint() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.BIGINT;
        rs.longValue = 9876543210L;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("9876543210", new String(result));
    }

    @Test
    void testGetValueFromResultSet_Smallint() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.SMALLINT;
        rs.shortValue = (short) 123;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("123", new String(result));
    }

    @Test
    void testGetValueFromResultSet_Tinyint() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.TINYINT;
        rs.byteValue = (byte) 7;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("7", new String(result));
    }

    @Test
    void testGetValueFromResultSet_Char() throws Exception {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.CHAR;
        rs.stringValue = "X";
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("X", new String(result, "SHIFT-JIS"));
    }

    @Test
    void testGetValueFromResultSet_DefaultType() throws Exception {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.STRUCT;
        rs.stringValue = "struct-val";
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("struct-val", new String(result, "SHIFT-JIS"));
    }

    @Test
    void testGetValueFromResultSet_DefaultType_Null() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.STRUCT;
        rs.stringValue = null;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNull(result);
    }

    @Test
    void testGetValueFromResultSet_ExceptionReturnsNull() {
        // ResultSet that throws on getMetaData
        MockResultSet rs =
                new MockResultSet() {
                    @Override
                    public ResultSetMetaData getMetaData() throws SQLException {
                        throw new SQLException("broken");
                    }
                };
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNull(result);
    }

    @Test
    void testGetValueFromResultSet_Float() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.FLOAT;
        rs.doubleValue = 2.5;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("2.5", new String(result));
    }

    @Test
    void testGetValueFromResultSet_Real() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.REAL;
        rs.doubleValue = 1.5;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("1.5", new String(result));
    }

    @Test
    void testGetValueFromResultSet_Numeric() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.NUMERIC;
        rs.bigDecimalValue = new BigDecimal("999");
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("999", new String(result));
    }

    // ============================================================
    // cobolToString: JAPANESE VARYING
    // ============================================================

    @Test
    void testCobolToString_JapaneseVarying() {
        // 4-byte header (char count in big-endian) + data (2 bytes per char)
        byte[] data = new byte[12]; // 4 header + 8 max
        ByteBuffer.wrap(data).putInt(2); // 2 chars = 4 bytes
        data[4] = 'A';
        data[5] = 'B';
        data[6] = 'C';
        data[7] = 'D';
        AbstractCobolField field =
                makeField(
                        12,
                        data,
                        CobolFieldAttribute.COB_TYPE_GROUP,
                        0,
                        0,
                        CobolFieldAttribute.COB_FLAG_VARYING
                                | CobolFieldAttribute.COB_FLAG_NATIONAL_VARYING);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("ABCD", result);
    }

    // ============================================================
    // cobolToString: NUMERIC_FLOAT (not DOUBLE)
    // ============================================================

    @Test
    void testCobolToString_NumericFloat() {
        // COB_TYPE_NUMERIC_FLOAT may not be supported by CobolFieldFactory,
        // use COB_TYPE_NUMERIC_DOUBLE which maps to TYPE_FLOAT in the converter
        byte[] data = new byte[8];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putDouble(2.71);
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DOUBLE, 0, 0, 0);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("2.71", result);
    }

    // ============================================================
    // cobolToString: GROUP (non-varying)
    // ============================================================

    @Test
    void testCobolToString_Group() {
        byte[] data = "ABCDEF  ".getBytes();
        AbstractCobolField field = makeField(8, data, CobolFieldAttribute.COB_TYPE_GROUP, 0, 0, 0);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("ABCDEF", result);
    }

    // ============================================================
    // cobolToString: UNSIGNED_NUMERIC with scale (V99)
    // ============================================================

    @Test
    void testCobolToString_UnsignedNumeric_WithScaleV99() {
        // PIC 9(4)V99 => 6 bytes, scale=2
        byte[] data = "012345".getBytes();
        AbstractCobolField field =
                makeField(6, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 6, 2, 0);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("123.45", result);
    }

    // ============================================================
    // cobolToString: SIGNED_TRAILING_COMBINED with scale
    // ============================================================

    @Test
    void testCobolToString_SignedTrailingCombined_WithScale() {
        byte[] data = "12".getBytes();
        byte[] fullData = new byte[3];
        System.arraycopy(data, 0, fullData, 0, 2);
        fullData[2] = (byte) 0x73; // overpunch for -3
        AbstractCobolField field =
                makeField(
                        3,
                        fullData,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        3,
                        1,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("-12.3", result);
    }

    // ============================================================
    // cobolToString: SIGNED_TRAILING_SEPARATE with scale
    // ============================================================

    @Test
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
        assertEquals("-12.34", result);
    }

    @Test
    void testCobolToString_SignedTrailingSeparate_Zero() {
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
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("0", result);
    }

    // ============================================================
    // cobolToString: SIGNED_LEADING_COMBINED with scale
    // ============================================================

    @Test
    void testCobolToString_SignedLeadingCombined_WithScale() {
        byte[] data = new byte[4];
        data[0] = (byte) 0x71;
        data[1] = '2';
        data[2] = '3';
        data[3] = '4';
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
        assertEquals("-12.34", result);
    }

    @Test
    void testCobolToString_SignedLeadingCombined_Zero() {
        byte[] data = new byte[4];
        data[0] = (byte) 0x70;
        data[1] = '0';
        data[2] = '0';
        data[3] = '0';
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("0", result);
    }

    // ============================================================
    // cobolToString: SIGNED_LEADING_SEPARATE with positive scale
    // ============================================================

    @Test
    void testCobolToString_SignedLeadingSeparate_PositiveScale() {
        byte[] data = "+12340".getBytes();
        AbstractCobolField field =
                makeField(
                        5,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("+12340", result);
    }

    // ============================================================
    // cobolToString: UNSIGNED_BINARY default size
    // ============================================================

    @Test
    void testCobolToString_UnsignedBinary_3Bytes() {
        byte[] data = new byte[3];
        data[0] = 0x01;
        data[1] = 0x02;
        data[2] = 0x03;
        AbstractCobolField field =
                makeField(3, data, CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 7, 0, 0);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("0", result);
    }

    @Test
    void testCobolToString_SignedBinary_3Bytes() {
        byte[] data = new byte[3];
        AbstractCobolField field =
                makeField(
                        3,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        7,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("0", result);
    }

    @Test
    void testCobolToString_SignedBinary_ScaleWithPointInMiddle() {
        // value=12345, scale=2 => 123.45
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
        assertEquals("123.45", result);
    }

    @Test
    void testCobolToString_SignedBinary_NegativeWithScale() {
        byte[] data = new byte[4];
        ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN).putInt(-12345);
        AbstractCobolField field =
                makeField(
                        4,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        9,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("-123.45", result);
    }

    // ============================================================
    // cobolToString: NULL storage
    // ============================================================

    @Test
    void testCobolToString_NullStorage() {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, (CobolDataStorage) null, attr);
        assertEquals("", CobolDataConverter.cobolToString(field));
    }

    // ============================================================
    // cobolToString: SIGNED_PACKED even digits
    // ============================================================

    @Test
    void testCobolToString_SignedPacked_EvenDigits() {
        // PIC S9(4) COMP-3: 4 digits -> 3 bytes: X'01 23 4C'
        byte[] data = new byte[] {0x01, 0x23, 0x4C};
        AbstractCobolField field =
                makeField(
                        3,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("1234", result);
    }

    @Test
    void testCobolToString_SignedPacked_ScaleExceedsDigits() {
        // digits=3, scale=5 so -scale > length path
        byte[] data = new byte[] {0x01, 0x2C};
        AbstractCobolField field =
                makeField(
                        2,
                        data,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        3,
                        5,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("0.00012", result);
    }

    @Test
    void testCobolToString_UnsignedPacked_WithScale() {
        byte[] data = new byte[] {0x01, 0x23, 0x4F};
        AbstractCobolField field =
                makeField(3, data, CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 5, 2, 0);
        String result = CobolDataConverter.cobolToString(field);
        assertEquals("12.34", result);
    }

    // ============================================================
    // stringToCobol: write variants
    // ============================================================

    @Test
    void testStringToCobol_UnsignedNumeric_WithScale() {
        byte[] data = new byte[6];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 6, 2, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(6, storage, attr);
        CobolDataConverter.stringToCobol(field, "123.45".getBytes());
        // Should have 012345 in storage
        String stored = new String(data);
        assertEquals("012345", stored);
    }

    @Test
    void testStringToCobol_SignedTrailingCombined_Positive() {
        byte[] data = new byte[4];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(4, storage, attr);
        CobolDataConverter.stringToCobol(field, "42".getBytes());
        // Last byte should NOT have overpunch
        assertTrue((data[3] & 0xFF) < 0x70);
    }

    @Test
    void testStringToCobol_SignedTrailingCombined_WithScale() {
        byte[] data = new byte[5];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        5,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(5, storage, attr);
        CobolDataConverter.stringToCobol(field, "-12.34".getBytes());
        assertTrue((data[4] & 0xFF) >= 0x70);
    }

    @Test
    void testStringToCobol_SignedTrailingSeparate_WithScale() {
        byte[] data = new byte[6]; // 5 digits + 1 sign
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        5,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(6, storage, attr);
        CobolDataConverter.stringToCobol(field, "-12.34".getBytes());
        assertEquals('-', (char) data[5]);
    }

    @Test
    void testStringToCobol_SignedLeadingSeparate_WithScale() {
        byte[] data = new byte[6]; // 1 sign + 5 digits
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        5,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(6, storage, attr);
        CobolDataConverter.stringToCobol(field, "-12.34".getBytes());
        assertEquals('-', (char) data[0]);
    }

    @Test
    void testStringToCobol_SignedLeadingCombined_WithScale() {
        byte[] data = new byte[5];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        5,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(5, storage, attr);
        CobolDataConverter.stringToCobol(field, "-12.34".getBytes());
        assertTrue((data[0] & 0xFF) >= 0x70);
    }

    @Test
    void testStringToCobol_SignedPacked_Negative() {
        byte[] data = new byte[3];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(3, storage, attr);
        CobolDataConverter.stringToCobol(field, "-12345".getBytes());
        assertEquals(0x0D, data[2] & 0x0F);
    }

    @Test
    void testStringToCobol_UnsignedPacked_WithScale() {
        byte[] data = new byte[3];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 5, 2, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(3, storage, attr);
        CobolDataConverter.stringToCobol(field, "123.45".getBytes());
        assertEquals(0x0F, data[2] & 0x0F);
    }

    @Test
    void testStringToCobol_National() {
        byte[] data = new byte[10];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NATIONAL, 0, 0, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(10, storage, attr);
        CobolDataConverter.stringToCobol(field, "Hi".getBytes());
        assertEquals('H', (char) data[0]);
    }

    @Test
    void testStringToCobol_National_LongerThanField() {
        byte[] data = new byte[4];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NATIONAL, 0, 0, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(4, storage, attr);
        CobolDataConverter.stringToCobol(field, "ABCDEF".getBytes());
        assertEquals('A', (char) data[0]);
    }

    @Test
    void testStringToCobol_JapaneseVarying() {
        byte[] data = new byte[12]; // 4 header + 8 max
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_GROUP,
                        0,
                        0,
                        CobolFieldAttribute.COB_FLAG_VARYING
                                | CobolFieldAttribute.COB_FLAG_NATIONAL_VARYING,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(12, storage, attr);
        CobolDataConverter.stringToCobol(field, "AB".getBytes());
        int charCount = ByteBuffer.wrap(data, 0, 4).getInt();
        assertEquals(1, charCount); // 2 bytes / 2 = 1 char
    }

    @Test
    void testStringToCobol_JapaneseVarying_LongerThanField() {
        byte[] data = new byte[8]; // 4 header + 4 max
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_GROUP,
                        0,
                        0,
                        CobolFieldAttribute.COB_FLAG_VARYING
                                | CobolFieldAttribute.COB_FLAG_NATIONAL_VARYING,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(8, storage, attr);
        CobolDataConverter.stringToCobol(field, "ABCDEF".getBytes());
        int charCount = ByteBuffer.wrap(data, 0, 4).getInt();
        assertEquals(2, charCount); // 4 bytes / 2 = 2 chars
    }

    @Test
    void testStringToCobol_AlphanumericVarying_LongerThanField() {
        byte[] data = new byte[8]; // 4 header + 4 max
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_GROUP,
                        0,
                        0,
                        CobolFieldAttribute.COB_FLAG_VARYING,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(8, storage, attr);
        CobolDataConverter.stringToCobol(field, "ABCDEFGH".getBytes());
        int storedLen = ByteBuffer.wrap(data, 0, 4).getInt();
        assertEquals(4, storedLen);
    }

    @Test
    void testStringToCobol_Alphabetic_LongerThanField() {
        byte[] data = new byte[4];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(4, storage, attr);
        CobolDataConverter.stringToCobol(field, "ABCDEFGH".getBytes());
        assertEquals('A', (char) data[0]);
        assertEquals('D', (char) data[3]);
    }

    @Test
    void testStringToCobol_BinaryNative() {
        // Binary uses writeAlphanumeric
        byte[] data = new byte[4];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 9, 0, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(4, storage, attr);
        CobolDataConverter.stringToCobol(field, "42".getBytes());
        assertEquals('4', (char) data[0]);
    }

    @Test
    void testStringToCobol_Float() {
        byte[] data = new byte[8];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_DOUBLE, 0, 0, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(8, storage, attr);
        CobolDataConverter.stringToCobol(field, "3.14".getBytes());
        assertEquals('3', (char) data[0]);
    }

    @Test
    void testStringToCobol_SignedPacked_EvenDigits() {
        byte[] data = new byte[3];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(3, storage, attr);
        CobolDataConverter.stringToCobol(field, "1234".getBytes());
        assertEquals(0x0C, data[2] & 0x0F);
    }

    @Test
    void testStringToCobol_SignedPacked_WithPP() {
        // PIC S9(3)PP COMP-3: digits=5, scale=-2
        byte[] data = new byte[2];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        -2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(2, storage, attr);
        CobolDataConverter.stringToCobol(field, "12300".getBytes());
        assertEquals(0x0C, data[1] & 0x0F);
    }

    @Test
    void testStringToCobol_UnsignedPacked_WithPP() {
        byte[] data = new byte[2];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 5, -2, 0, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        AbstractCobolField field = CobolFieldFactory.makeCobolField(2, storage, attr);
        CobolDataConverter.stringToCobol(field, "12300".getBytes());
        assertEquals(0x0F, data[1] & 0x0F);
    }

    // ============================================================
    // stringToCobolRaw: additional tests
    // ============================================================

    @Test
    void testStringToCobolRaw_Alphabetic() {
        byte[] target = new byte[10];
        CobolDataStorage targetStorage = new CobolDataStorage(target);
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(10, new CobolDataStorage(new byte[10]), attr);
        CobolDataConverter.stringToCobolRaw(field, targetStorage, 10, "Hi".getBytes());
        assertEquals('H', (char) target[0]);
        assertEquals('i', (char) target[1]);
        assertEquals(' ', (char) target[2]);
    }

    @Test
    void testStringToCobolRaw_Packed() {
        byte[] target = new byte[3];
        CobolDataStorage targetStorage = new CobolDataStorage(target);
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(3, new CobolDataStorage(new byte[3]), attr);
        CobolDataConverter.stringToCobolRaw(field, targetStorage, 3, "12345".getBytes());
        assertEquals(0x0C, target[2] & 0x0F);
    }

    @Test
    void testStringToCobolRaw_Varying() {
        byte[] target = new byte[14];
        CobolDataStorage targetStorage = new CobolDataStorage(target);
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_GROUP,
                        0,
                        0,
                        CobolFieldAttribute.COB_FLAG_VARYING,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(14, new CobolDataStorage(new byte[14]), attr);
        CobolDataConverter.stringToCobolRaw(field, targetStorage, 14, "Hello".getBytes());
        int storedLen = ByteBuffer.wrap(target, 0, 4).getInt();
        assertEquals(5, storedLen);
    }

    @Test
    void testStringToCobolRaw_PackedWithPP() {
        byte[] target = new byte[2];
        CobolDataStorage targetStorage = new CobolDataStorage(target);
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        5,
                        -2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(2, new CobolDataStorage(new byte[2]), attr);
        CobolDataConverter.stringToCobolRaw(field, targetStorage, 2, "12300".getBytes());
        assertEquals(0x0C, target[1] & 0x0F);
    }

    // ============================================================
    // getValueFromResultSet: additional types
    // ============================================================

    @Test
    void testGetValueFromResultSet_Timestamp() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.TIMESTAMP;
        rs.timestampValue = new java.sql.Timestamp(0);
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        String str = new String(result);
        assertTrue(str.contains("-"));
    }

    @Test
    void testGetValueFromResultSet_Timestamp_Null() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.TIMESTAMP;
        rs.timestampValue = null;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNull(result);
    }

    @Test
    void testGetValueFromResultSet_Date() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.DATE;
        rs.dateValue = new java.sql.Date(0);
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
    }

    @Test
    void testGetValueFromResultSet_Date_Null() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.DATE;
        rs.dateValue = null;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNull(result);
    }

    @Test
    void testGetValueFromResultSet_Time() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.TIME;
        rs.timeValue = new java.sql.Time(0);
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
    }

    @Test
    void testGetValueFromResultSet_Time_Null() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.TIME;
        rs.timeValue = null;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNull(result);
    }

    @Test
    void testGetValueFromResultSet_Rowid() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.ROWID;
        rs.rowIdValue =
                new RowId() {
                    @Override
                    public byte[] getBytes() {
                        return new byte[] {1, 2, 3};
                    }

                    @Override
                    public String toString() {
                        return "rowid-123";
                    }
                };
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
        assertEquals("rowid-123", new String(result));
    }

    @Test
    void testGetValueFromResultSet_Rowid_Null() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.ROWID;
        rs.rowIdValue = null;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNull(result);
    }

    @Test
    void testGetValueFromResultSet_LongVarchar() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.LONGVARCHAR;
        rs.stringValue = "long text";
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
    }

    @Test
    void testGetValueFromResultSet_NChar() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.NCHAR;
        rs.stringValue = "nchar";
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
    }

    @Test
    void testGetValueFromResultSet_NVarchar() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.NVARCHAR;
        rs.stringValue = "nvarchar";
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
    }

    @Test
    void testGetValueFromResultSet_LongNVarchar() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.LONGNVARCHAR;
        rs.stringValue = "longnvarchar";
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNotNull(result);
    }

    @Test
    void testGetValueFromResultSet_Boolean_WasNull() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.BOOLEAN;
        rs.boolValue = false;
        rs.wasNullFlag = true;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNull(result);
    }

    @Test
    void testGetValueFromResultSet_Bigint_WasNull() {
        MockResultSet rs = new MockResultSet();
        rs.columnType = Types.BIGINT;
        rs.longValue = 0;
        rs.wasNullFlag = true;
        byte[] result = CobolDataConverter.getValueFromResultSet(rs, 1);
        assertNull(result);
    }

    // ============================================================
    // setParam: additional types
    // ============================================================

    @Test
    void testSetParam_DateType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.DATE);
        byte[] data = "2024-01-15        ".getBytes();
        AbstractCobolField field =
                makeField(18, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertNotNull(mock.lastDateValue);
    }

    @Test
    void testSetParam_TimeType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.TIME);
        byte[] data = "12:30:00          ".getBytes();
        AbstractCobolField field =
                makeField(18, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertNotNull(mock.lastTimeValue);
    }

    @Test
    void testSetParam_TimestampType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.TIMESTAMP);
        byte[] data = "2024-01-15 12:30:00     ".getBytes();
        AbstractCobolField field =
                makeField(24, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertNotNull(mock.lastTimestampValue);
    }

    @Test
    void testSetParam_BitType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.BIT);
        byte[] data = "true    ".getBytes();
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertTrue(mock.lastBoolValue);
    }

    @Test
    void testSetParam_RealType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.REAL);
        byte[] data = "0042".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals(42.0, mock.lastDoubleValue, 0.001);
    }

    @Test
    void testSetParam_TimeWithTimezone() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.TIME_WITH_TIMEZONE);
        byte[] data = "12:30:00          ".getBytes();
        AbstractCobolField field =
                makeField(18, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertNotNull(mock.lastTimeValue);
    }

    @Test
    void testSetParam_TimestampWithTimezone() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.TIMESTAMP_WITH_TIMEZONE);
        byte[] data = "2024-01-15 12:30:00     ".getBytes();
        AbstractCobolField field =
                makeField(24, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertNotNull(mock.lastTimestampValue);
    }

    @Test
    void testSetParam_NCharType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.NCHAR);
        byte[] data = "Hello   ".getBytes();
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals("Hello", mock.lastStringValue);
    }

    @Test
    void testSetParam_NVarcharType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.NVARCHAR);
        byte[] data = "World   ".getBytes();
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals("World", mock.lastStringValue);
    }

    @Test
    void testSetParam_LongVarcharType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.LONGVARCHAR);
        byte[] data = "text    ".getBytes();
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals("text", mock.lastStringValue);
    }

    @Test
    void testSetParam_ClobType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.CLOB);
        byte[] data = "clob    ".getBytes();
        AbstractCobolField field =
                makeField(8, data, CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals("clob", mock.lastStringValue);
    }

    @Test
    void testSetParam_TinyintType() throws Exception {
        MockPreparedStatement mock = new MockPreparedStatement();
        MockParameterMetaData meta = new MockParameterMetaData(Types.TINYINT);
        byte[] data = "0007".getBytes();
        AbstractCobolField field =
                makeField(4, data, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        CobolDataConverter.setParam(mock, 1, meta, field);
        assertEquals(7, mock.lastIntValue);
    }

    // ============================================================
    // Round-trip: cobolToString -> stringToCobol
    // ============================================================

    @Test
    void testRoundTrip_UnsignedNumeric() {
        byte[] original = "1234".getBytes();
        AbstractCobolField field =
                makeField(4, original, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0);
        String str = CobolDataConverter.cobolToString(field);
        byte[] target = new byte[4];
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0, null);
        AbstractCobolField targetField =
                CobolFieldFactory.makeCobolField(4, new CobolDataStorage(target), attr);
        CobolDataConverter.stringToCobol(targetField, str.getBytes());
        assertArrayEquals(original, target);
    }

    // ============================================================
    // Mock implementations
    // ============================================================

    /** Minimal mock for ParameterMetaData. */
    private static class MockParameterMetaData implements ParameterMetaData {
        private final int type;

        MockParameterMetaData(int type) {
            this.type = type;
        }

        @Override
        public int getParameterType(int param) throws SQLException {
            return type;
        }

        @Override
        public int getParameterCount() {
            return 1;
        }

        @Override
        public int isNullable(int param) {
            return 0;
        }

        @Override
        public boolean isSigned(int param) {
            return false;
        }

        @Override
        public int getPrecision(int param) {
            return 0;
        }

        @Override
        public int getScale(int param) {
            return 0;
        }

        @Override
        public String getParameterTypeName(int param) {
            return "";
        }

        @Override
        public String getParameterClassName(int param) {
            return "";
        }

        @Override
        public int getParameterMode(int param) {
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

    /** Minimal mock PreparedStatement that records which setXxx was called. */
    private static class MockPreparedStatement implements PreparedStatement {
        String lastStringValue;
        int lastStringIndex;
        int lastIntValue;
        BigDecimal lastBigDecimal;
        double lastDoubleValue;
        float lastFloatValue;
        boolean lastBoolValue;
        Date lastDateValue;
        Time lastTimeValue;
        Timestamp lastTimestampValue;

        @Override
        public void setString(int i, String s) {
            lastStringIndex = i;
            lastStringValue = s;
        }

        @Override
        public void setInt(int i, int v) {
            lastIntValue = v;
        }

        @Override
        public void setBigDecimal(int i, BigDecimal v) {
            lastBigDecimal = v;
        }

        @Override
        public void setDouble(int i, double v) {
            lastDoubleValue = v;
        }

        @Override
        public void setFloat(int i, float v) {
            lastFloatValue = v;
        }

        @Override
        public void setBoolean(int i, boolean v) {
            lastBoolValue = v;
        }

        @Override
        public void setDate(int i, Date v) {
            lastDateValue = v;
        }

        @Override
        public void setTime(int i, Time v) {
            lastTimeValue = v;
        }

        @Override
        public void setTimestamp(int i, Timestamp v) {
            lastTimestampValue = v;
        }

        // All remaining methods throw UnsupportedOperationException
        @Override
        public ResultSet executeQuery() {
            throw new UnsupportedOperationException();
        }

        @Override
        public int executeUpdate() {
            throw new UnsupportedOperationException();
        }

        @Override
        public void setNull(int i, int t) {}

        @Override
        public void setByte(int i, byte v) {}

        @Override
        public void setShort(int i, short v) {}

        @Override
        public void setLong(int i, long v) {}

        @Override
        public void setBytes(int i, byte[] v) {}

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
        public void setCharacterStream(int i, Reader r, int l) {}

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
        public ParameterMetaData getParameterMetaData() {
            return null;
        }

        @Override
        public void setRowId(int i, RowId v) {}

        @Override
        public void setNString(int i, String v) {}

        @Override
        public void setNCharacterStream(int i, Reader v, long l) {}

        @Override
        public void setNClob(int i, NClob v) {}

        @Override
        public void setClob(int i, Reader r, long l) {}

        @Override
        public void setBlob(int i, InputStream s, long l) {}

        @Override
        public void setNClob(int i, Reader r, long l) {}

        @Override
        public void setSQLXML(int i, SQLXML v) {}

        @Override
        public void setObject(int i, Object v, int t, int s) {}

        @Override
        public void setAsciiStream(int i, InputStream s, long l) {}

        @Override
        public void setBinaryStream(int i, InputStream s, long l) {}

        @Override
        public void setCharacterStream(int i, Reader r, long l) {}

        @Override
        public void setAsciiStream(int i, InputStream s) {}

        @Override
        public void setBinaryStream(int i, InputStream s) {}

        @Override
        public void setCharacterStream(int i, Reader r) {}

        @Override
        public void setNCharacterStream(int i, Reader r) {}

        @Override
        public void setClob(int i, Reader r) {}

        @Override
        public void setBlob(int i, InputStream s) {}

        @Override
        public void setNClob(int i, Reader r) {}

        @Override
        public ResultSet executeQuery(String s) {
            throw new UnsupportedOperationException();
        }

        @Override
        public int executeUpdate(String s) {
            throw new UnsupportedOperationException();
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
        public boolean execute(String s) {
            return false;
        }

        @Override
        public ResultSet getResultSet() {
            return null;
        }

        @Override
        public int getUpdateCount() {
            return 0;
        }

        @Override
        public boolean getMoreResults() {
            return false;
        }

        @Override
        public void setFetchDirection(int i) {}

        @Override
        public int getFetchDirection() {
            return 0;
        }

        @Override
        public void setFetchSize(int i) {}

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

        @Override
        public void setAsciiStream(int i, InputStream s, int l) {}

        @Override
        @SuppressWarnings("deprecation")
        public void setUnicodeStream(int i, InputStream s, int l) {}

        @Override
        public void setBinaryStream(int i, InputStream s, int l) {}
    }

    /** Minimal mock ResultSet for getValueFromResultSet tests. */
    private static class MockResultSet implements ResultSet {
        int columnType;
        String stringValue;
        BigDecimal bigDecimalValue;
        int intValue;
        long longValue;
        short shortValue;
        byte byteValue;
        double doubleValue;
        boolean boolValue;
        boolean wasNullFlag = false;
        Timestamp timestampValue;
        Date dateValue;
        Time timeValue;
        RowId rowIdValue;

        @Override
        public ResultSetMetaData getMetaData() throws SQLException {
            return new MockResultSetMetaData(columnType);
        }

        @Override
        public String getString(int col) {
            return stringValue;
        }

        @Override
        public BigDecimal getBigDecimal(int col) {
            return bigDecimalValue;
        }

        @Override
        public int getInt(int col) {
            return intValue;
        }

        @Override
        public long getLong(int col) {
            return longValue;
        }

        @Override
        public short getShort(int col) {
            return shortValue;
        }

        @Override
        public byte getByte(int col) {
            return byteValue;
        }

        @Override
        public double getDouble(int col) {
            return doubleValue;
        }

        @Override
        public boolean getBoolean(int col) {
            return boolValue;
        }

        @Override
        public boolean wasNull() {
            return wasNullFlag;
        }

        @Override
        public Timestamp getTimestamp(int col) {
            return timestampValue;
        }

        @Override
        public Date getDate(int col) {
            return dateValue;
        }

        @Override
        public Time getTime(int col) {
            return timeValue;
        }

        @Override
        public RowId getRowId(int col) {
            return rowIdValue;
        }

        // All remaining methods - minimal stubs
        @Override
        public boolean next() {
            return false;
        }

        @Override
        public void close() {}

        @Override
        public float getFloat(int col) {
            return 0;
        }

        @Override
        public byte[] getBytes(int col) {
            return null;
        }

        @Override
        @SuppressWarnings("deprecation")
        public BigDecimal getBigDecimal(int col, int s) {
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
        public <T> T unwrap(Class<T> iface) {
            return null;
        }

        @Override
        public boolean isClosed() {
            return false;
        }

        @Override
        public boolean isWrapperFor(Class<?> iface) {
            return false;
        }
    }

    /** Minimal mock ResultSetMetaData. */
    private static class MockResultSetMetaData implements ResultSetMetaData {
        private final int columnType;

        MockResultSetMetaData(int columnType) {
            this.columnType = columnType;
        }

        @Override
        public int getColumnType(int col) {
            return columnType;
        }

        @Override
        public int getColumnCount() {
            return 1;
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
    }
}

package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory;
import org.junit.jupiter.api.Test;

/**
 * Pure (no-database) unit tests for {@link CobolDataConverter} write/read conversion helpers.
 *
 * <p>Unlike {@link CobolDataConverterTest}, this class needs no PostgreSQL container: it only
 * exercises the host-variable byte<->string conversion logic ({@code stringToCobol} /
 * {@code cobolToString}), which is independent of any JDBC connection. The cases here focus on
 * the signed/scaled/packed write paths and round-trips that the container-based suite does not
 * cover.
 */
class CobolDataConverterPureTest {

    private static AbstractCobolField makeField(
            int size, byte[] data, int type, int digits, int scale, int flags) {
        CobolFieldAttribute attr = new CobolFieldAttribute(type, digits, scale, flags, null);
        CobolDataStorage storage = new CobolDataStorage(data);
        return CobolFieldFactory.makeCobolField(size, storage, attr);
    }

    /** Round-trip a SQL string value into a COBOL field and back to a string. */
    private static String roundTrip(
            int size, int type, int digits, int scale, int flags, String v) {
        AbstractCobolField field = makeField(size, new byte[size], type, digits, scale, flags);
        CobolDataConverter.stringToCobol(field, v.getBytes());
        return CobolDataConverter.cobolToString(field);
    }

    @Test
    void testWriteReadUnsignedNumeric() {
        assertEquals(
                "1234",
                roundTrip(4, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0, "1234"),
                "unsigned numeric round-trip");
    }

    @Test
    void testWriteReadUnsignedNumericWithScale() {
        assertEquals(
                "12.34",
                roundTrip(6, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 6, 2, 0, "12.34"),
                "unsigned numeric scaled round-trip");
    }

    @Test
    void testWriteReadSignedTrailingCombinedNegative() {
        assertEquals(
                "-1234",
                roundTrip(
                        4,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        "-1234"),
                "signed trailing combined negative round-trip");
    }

    @Test
    void testWriteReadSignedTrailingCombinedPositive() {
        assertEquals(
                "1234",
                roundTrip(
                        4,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        "1234"),
                "signed trailing combined positive round-trip");
    }

    @Test
    void testWriteReadSignedTrailingSeparateNegative() {
        assertEquals(
                "-1234",
                roundTrip(
                        5,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE,
                        "-1234"),
                "signed trailing separate negative round-trip");
    }

    @Test
    void testWriteReadSignedLeadingCombinedNegative() {
        assertEquals(
                "-1234",
                roundTrip(
                        4,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                                | CobolFieldAttribute.COB_FLAG_SIGN_LEADING,
                        "-1234"),
                "signed leading combined negative round-trip");
    }

    @Test
    void testWriteReadSignedPackedNegative() {
        assertEquals(
                "-1234",
                roundTrip(
                        3,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        4,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        "-1234"),
                "signed packed negative round-trip");
    }

    @Test
    void testWriteReadUnsignedPacked() {
        assertEquals(
                "123",
                roundTrip(2, CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 3, 0, 0, "123"),
                "unsigned packed round-trip");
    }

    @Test
    void testWriteReadSignedPackedWithScale() {
        assertEquals(
                "-12.34",
                roundTrip(
                        3,
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        4,
                        2,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        "-12.34"),
                "signed packed scaled round-trip");
    }

    @Test
    void testWriteReadZeroValue() {
        assertEquals(
                "0",
                roundTrip(4, CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 4, 0, 0, "0"),
                "zero round-trip");
    }
}

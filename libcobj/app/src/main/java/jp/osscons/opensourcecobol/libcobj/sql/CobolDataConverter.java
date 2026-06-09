package jp.osscons.opensourcecobol.libcobj.sql;

import java.math.BigDecimal;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.Charset;
import java.sql.ParameterMetaData;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Types;
import java.text.SimpleDateFormat;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;

/** Converts between COBOL host variable storage and Java/JDBC types. */
final class CobolDataConverter {

    /** Private constructor to prevent instantiation of utility class. */
    private CobolDataConverter() {}

    /**
     * COBOL ホスト変数の内部分類。CobolFieldAttribute から resolveHvarType で
     * 解決され、本クラス内のディスパッチにのみ使われる (公開しない)。
     */
    private enum HvarType {
        /** Unsigned numeric display (USAGE DISPLAY, no sign). */
        UNSIGNED_NUMERIC,
        /** Signed numeric with trailing separate sign character. */
        SIGNED_TRAILING_SEPARATE,
        /** Signed numeric with trailing combined (overpunch) sign. */
        SIGNED_TRAILING_COMBINED,
        /** Signed numeric with leading separate sign character. */
        SIGNED_LEADING_SEPARATE,
        /** Signed numeric with leading combined (overpunch) sign. */
        SIGNED_LEADING_COMBINED,
        /** Unsigned packed-decimal (COMP-3, no sign nibble). */
        UNSIGNED_PACKED,
        /** Signed packed-decimal (COMP-3). */
        SIGNED_PACKED,
        /** Unsigned binary native (COMP-5, big-endian). */
        UNSIGNED_BINARY_NATIVE,
        /** Signed binary native (COMP-5, big-endian). */
        SIGNED_BINARY_NATIVE,
        /** Alphabetic (PIC A). */
        ALPHABETIC,
        /** Group item (treated as alphanumeric). */
        GROUP,
        /** Floating-point double (COMP-2). */
        FLOAT,
        /** National character (PIC N). */
        NATIONAL,
        /** Alphanumeric varying-length string. */
        ALPHANUMERIC_VARYING,
        /** Japanese (DBCS) varying-length string. */
        JAPANESE_VARYING
    }

    private static final Charset SHIFT_JIS = Charset.forName("SHIFT-JIS");
    private static final int OCDB_VARCHAR_HEADER_BYTE = 4;

    /**
     * Resolve the internal HvarType from an AbstractCobolField's attributes.
     *
     * @param field the COBOL field
     * @return the HvarType for internal dispatch
     */
    private static HvarType resolveHvarType(AbstractCobolField field) {
        CobolFieldAttribute attr = field.getAttribute();
        int type = attr.getType();
        boolean hasSign = attr.isFlagHaveSign();
        boolean signSep = attr.isFlagSignSeparate();
        boolean signLead = attr.isFlagSignLeading();

        switch (type) {
            case CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY:
                if (!hasSign) {
                    return HvarType.UNSIGNED_NUMERIC;
                }
                if (signSep && signLead) {
                    return HvarType.SIGNED_LEADING_SEPARATE;
                }
                if (signSep) {
                    return HvarType.SIGNED_TRAILING_SEPARATE;
                }
                if (signLead) {
                    return HvarType.SIGNED_LEADING_COMBINED;
                }
                return HvarType.SIGNED_TRAILING_COMBINED;
            case CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED:
                return hasSign ? HvarType.SIGNED_PACKED : HvarType.UNSIGNED_PACKED;
            case CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY:
                return hasSign ? HvarType.SIGNED_BINARY_NATIVE : HvarType.UNSIGNED_BINARY_NATIVE;
            case CobolFieldAttribute.COB_TYPE_NUMERIC_FLOAT:
            case CobolFieldAttribute.COB_TYPE_NUMERIC_DOUBLE:
                return HvarType.FLOAT;
            case CobolFieldAttribute.COB_TYPE_NATIONAL:
                return HvarType.NATIONAL;
            case CobolFieldAttribute.COB_TYPE_GROUP:
                if (attr.isFlagVarying()) {
                    if ((attr.getFlags() & CobolFieldAttribute.COB_FLAG_NATIONAL_VARYING) != 0) {
                        return HvarType.JAPANESE_VARYING;
                    }
                    return HvarType.ALPHANUMERIC_VARYING;
                }
                return HvarType.GROUP;
            default:
                return HvarType.ALPHABETIC;
        }
    }

    /**
     * Convert a COBOL host variable to its string representation for SQL binding.
     *
     * @param field the COBOL host variable field
     * @return the string representation, or empty string if field is null
     */
    static String cobolToString(AbstractCobolField field) {
        if (field == null || field.getDataStorage() == null) {
            return "";
        }
        int length = field.getSize();
        // V99 → getScale()=2 → scale=-2; PP → getScale()=-2 → scale=0 (for non-packed)
        int scale = Math.min(0, -field.getAttribute().getScale());
        CobolDataStorage storage = field.getDataStorage();
        HvarType hvarType = resolveHvarType(field);

        // For VARYING, length = ARR size (total - 4 byte header)
        if (hvarType == HvarType.ALPHANUMERIC_VARYING || hvarType == HvarType.JAPANESE_VARYING) {
            length = field.getSize() - OCDB_VARCHAR_HEADER_BYTE;
        }

        // For packed decimal, use digit count instead of byte size.
        // Exclude PP digits (negative scale) since they are not physically stored.
        int rawScale = field.getAttribute().getScale();
        int packedLength = field.getAttribute().getDigits();
        int ppCount = 0;
        if (rawScale < 0) {
            ppCount = -rawScale;
            packedLength += rawScale; // subtract PP digits (rawScale is negative)
        }

        switch (hvarType) {
            case UNSIGNED_NUMERIC:
                return readNumericDisplay(length, scale, storage, DisplaySign.UNSIGNED);
            case SIGNED_TRAILING_SEPARATE:
                return readNumericDisplay(length, scale, storage, DisplaySign.TRAILING_SEPARATE);
            case SIGNED_TRAILING_COMBINED:
                return readNumericDisplay(length, scale, storage, DisplaySign.TRAILING_COMBINED);
            case SIGNED_LEADING_COMBINED:
                return readNumericDisplay(length, scale, storage, DisplaySign.LEADING_COMBINED);
            case SIGNED_LEADING_SEPARATE:
                return readNumericDisplay(length, scale, storage, DisplaySign.LEADING_SEPARATE);
            case UNSIGNED_PACKED:
                return appendTrailingZeros(
                        readUnsignedPacked(packedLength, scale, storage), ppCount);
            case SIGNED_PACKED:
                return appendTrailingZeros(readSignedPacked(packedLength, scale, storage), ppCount);
            case UNSIGNED_BINARY_NATIVE:
                return readUnsignedBinaryNative(length, scale, storage);
            case SIGNED_BINARY_NATIVE:
                return readSignedBinaryNative(length, scale, storage);
            case ALPHABETIC:
            case GROUP:
                return readAlphanumeric(length, scale, storage);
            case FLOAT:
                return readFloat(length, scale, storage);
            case NATIONAL:
                return readNational(length, scale, storage);
            case ALPHANUMERIC_VARYING:
                return readAlphanumericVarying(length, scale, storage);
            case JAPANESE_VARYING:
                return readJapaneseVarying(length, scale, storage);
            default:
                return readAlphanumeric(length, scale, storage);
        }
    }

    /** Sign representation of a PIC 9 DISPLAY (zoned decimal) item. */
    private enum DisplaySign {
        /** No operational sign. */
        UNSIGNED,
        /** Sign overpunched onto the leading digit. */
        LEADING_COMBINED,
        /** Sign overpunched onto the trailing digit. */
        TRAILING_COMBINED,
        /** Separate leading sign byte ('+' / '-'). */
        LEADING_SEPARATE,
        /** Separate trailing sign byte ('+' / '-'). */
        TRAILING_SEPARATE
    }

    /**
     * Read a PIC 9 DISPLAY (zoned decimal) value. All five variants (UNSIGNED, LEADING /
     * TRAILING COMBINED, LEADING / TRAILING SEPARATE) only differ in where the operational sign
     * lives; once the sign and the run of digit bytes are identified they share the same
     * formatting path.
     */
    private static String readNumericDisplay(
            int length, int scale, CobolDataStorage storage, DisplaySign sign) {
        // A leading separate sign occupies an extra byte ahead of the `length` digits.
        int byteLen = (sign == DisplaySign.LEADING_SEPARATE) ? length + 1 : length;
        byte[] data = storage.getByteArray(0, byteLen);
        boolean negative = false;
        int digitStart = 0;
        int digitCount = length;

        switch (sign) {
            case UNSIGNED:
                break;
            case LEADING_COMBINED:
                negative = isNegativeOverpunch(data[0]);
                if (negative) {
                    data[0] = clearOverpunch(data[0]);
                }
                break;
            case TRAILING_COMBINED:
                negative = isNegativeOverpunch(data[length - 1]);
                if (negative) {
                    data[length - 1] = clearOverpunch(data[length - 1]);
                }
                break;
            case LEADING_SEPARATE:
                negative = data[0] == (byte) '-';
                digitStart = 1; // the leading byte is the separate sign
                break;
            case TRAILING_SEPARATE:
                negative = data[length - 1] == (byte) '-';
                digitCount = length - 1; // the trailing byte is the separate sign
                break;
            default:
                break;
        }

        String digits = formatDisplayDigits(data, digitStart, digitCount, scale);
        if (negative && !"0".equals(digits)) {
            return "-" + digits;
        }
        return digits;
    }

    /**
     * Build the (sign-less) decimal string from {@code digitCount} digit bytes starting at
     * {@code digitStart} in {@code data}: zero-fill the working buffer, insert a decimal point
     * when {@code scale < 0}, and strip leading zeros.
     *
     * <p>A single leading pad byte is reserved so that a fully-fractional value (decimal places
     * &ge; digit count, e.g. {@code PIC SV9(6)}) still renders its leading {@code "0."}; in every
     * other case it is simply a leading zero that {@link #removeLeadingZeros} drops.
     */
    private static String formatDisplayDigits(byte[] data, int digitStart, int digitCount, int scale) {
        int realDataLength = (scale < 0) ? digitCount + 2 : digitCount + scale + 1;
        byte[] realData = new byte[realDataLength];
        java.util.Arrays.fill(realData, (byte) '0');
        System.arraycopy(data, digitStart, realData, 1, digitCount);

        if (scale < 0) {
            int pointIndex = realDataLength + scale - 1;
            if (pointIndex > 0 && pointIndex < realDataLength) {
                for (int i = realDataLength - 1; i > pointIndex; i--) {
                    realData[i] = realData[i - 1];
                }
                realData[pointIndex] = (byte) '.';
            }
        }

        return removeLeadingZeros(realData, false);
    }

    /** True if a zoned-decimal byte carries a negative overpunch sign (0x70-0x79). */
    private static boolean isNegativeOverpunch(byte b) {
        int v = b & 0xFF;
        return v >= 0x70 && v <= 0x79;
    }

    /** Strip the overpunch sign from a zoned-decimal byte, restoring the plain digit. */
    private static byte clearOverpunch(byte b) {
        return (byte) ((b & 0xFF) - 0x40);
    }

    private static String readUnsignedPacked(int length, int scale, CobolDataStorage storage) {
        int len = (length / 2) + 1;
        byte[] digits = unpackBcd(storage, length, len);
        return formatPackedResult(digits, length, scale, false);
    }

    private static String readSignedPacked(int length, int scale, CobolDataStorage storage) {
        int len = (length / 2) + 1;
        byte lastByte = storage.getByte(len - 1);
        boolean isNegative = (lastByte & 0x0F) == 0x0D;
        byte[] digits = unpackBcd(storage, length, len);
        return formatPackedResult(digits, length, scale, isNegative);
    }

    private static String appendTrailingZeros(String value, int count) {
        if (count <= 0) {
            return value;
        }
        // Insert zeros before the sign if negative, otherwise append
        boolean neg = value.startsWith("-");
        String abs = neg ? value.substring(1) : value;
        StringBuilder sb = new StringBuilder(abs);
        for (int i = 0; i < count; i++) {
            sb.append('0');
        }
        return neg ? "-" + sb.toString() : sb.toString();
    }

    private static byte[] unpackBcd(CobolDataStorage storage, int length, int len) {
        byte[] digits = new byte[length];
        for (int i = 0; i < len; i++) {
            int b = storage.getByte(i) & 0xFF;
            byte a0 = (byte) (((b & 0xF0) >> 4) + '0');
            byte a1 = (byte) ((b & 0x0F) + '0');
            if (length % 2 == 0) {
                if (i == 0) {
                    digits[0] = a1;
                } else if (i == len - 1) {
                    digits[length - 1] = a0;
                } else {
                    digits[2 * i - 1] = a0;
                    digits[2 * i] = a1;
                }
            } else {
                if (i == len - 1) {
                    digits[length - 1] = a0;
                } else {
                    digits[2 * i] = a0;
                    digits[2 * i + 1] = a1;
                }
            }
        }
        return digits;
    }

    private static String formatPackedResult(
            byte[] digits, int length, int scale, boolean isNegative) {
        byte[] tmpData;
        int tmpDataLen;

        if (-scale > length) {
            tmpDataLen = -scale + 2;
            tmpData = new byte[tmpDataLen];
            java.util.Arrays.fill(tmpData, (byte) '0');
            tmpData[1] = (byte) '.';
            System.arraycopy(digits, 0, tmpData, tmpDataLen - digits.length, digits.length);
        } else if (-scale > 0) {
            tmpDataLen = length + 1;
            tmpData = new byte[tmpDataLen];
            int intPartLen = tmpDataLen + scale - 1;
            System.arraycopy(digits, 0, tmpData, 0, intPartLen);
            tmpData[intPartLen] = (byte) '.';
            for (int i = 0; i < -scale; i++) {
                tmpData[intPartLen + 1 + i] = digits[digits.length + scale + i];
            }
            String noZeros = removeLeadingZeros(tmpData, false);
            if (isNegative && !"0".equals(noZeros)) {
                return "-" + noZeros;
            }
            return noZeros;
        } else {
            tmpDataLen = length + scale;
            tmpData = new byte[tmpDataLen];
            java.util.Arrays.fill(tmpData, (byte) '0');
            System.arraycopy(digits, 0, tmpData, 0, length);
        }

        String result = removeLeadingZeros(tmpData, false);
        if (isNegative && !"0".equals(result)) {
            return "-" + result;
        }
        return result;
    }

    private static String readUnsignedBinaryNative(
            int length, int scale, CobolDataStorage storage) {
        ByteBuffer bb = ByteBuffer.wrap(storage.getByteArray(0, length));
        bb.order(ByteOrder.BIG_ENDIAN);
        long value;
        switch (length) {
            case 1:
                value = bb.get() & 0xFFL;
                break;
            case 2:
                value = bb.getShort() & 0xFFFFL;
                break;
            case 4:
                value = bb.getInt() & 0xFFFFFFFFL;
                break;
            case 8:
                value = bb.getLong();
                break;
            default:
                value = 0;
                break;
        }
        return Long.toString(value);
    }

    private static String readSignedBinaryNative(int length, int scale, CobolDataStorage storage) {
        ByteBuffer bb = ByteBuffer.wrap(storage.getByteArray(0, length));
        bb.order(ByteOrder.BIG_ENDIAN);
        long value;
        switch (length) {
            case 1:
                value = bb.get();
                break;
            case 2:
                value = bb.getShort();
                break;
            case 4:
                value = bb.getInt();
                break;
            case 8:
                value = bb.getLong();
                break;
            default:
                value = 0;
                break;
        }
        String str = Long.toString(value);
        if (scale < 0) {
            boolean neg = value < 0;
            String abs = neg ? str.substring(1) : str;
            if (abs.length() <= -scale) {
                StringBuilder sb = new StringBuilder("0.");
                for (int i = 0; i < -scale - abs.length(); i++) {
                    sb.append('0');
                }
                sb.append(abs);
                return neg ? "-" + sb.toString() : sb.toString();
            } else {
                int pointPos = abs.length() + scale;
                String result = abs.substring(0, pointPos) + "." + abs.substring(pointPos);
                return neg ? "-" + result : result;
            }
        }
        return str;
    }

    private static String readAlphanumeric(int length, int scale, CobolDataStorage storage) {
        byte[] data = storage.getByteArray(0, length);
        String str = new String(data, SHIFT_JIS);
        int end = str.length();
        while (end > 0 && str.charAt(end - 1) == ' ') {
            end--;
        }
        return str.substring(0, end);
    }

    private static String readFloat(int length, int scale, CobolDataStorage storage) {
        ByteBuffer bb = ByteBuffer.wrap(storage.getByteArray(0, 8));
        bb.order(ByteOrder.BIG_ENDIAN);
        double value = bb.getDouble();
        return Double.toString(value);
    }

    private static String readNational(int length, int scale, CobolDataStorage storage) {
        byte[] data = storage.getByteArray(0, length);
        String str = new String(data, SHIFT_JIS);
        // National items are padded with the full-width (ideographic) space U+3000.
        int end = str.length();
        while (end > 0 && str.charAt(end - 1) == '\u3000') {
            end--;
        }
        return str.substring(0, end);
    }

    /** Read the big-endian length stored in the VARYING header (first 4 bytes). */
    private static int readVaryingHeaderLength(CobolDataStorage storage) {
        ByteBuffer bb = ByteBuffer.wrap(storage.getByteArray(0, OCDB_VARCHAR_HEADER_BYTE));
        bb.order(ByteOrder.BIG_ENDIAN);
        return bb.getInt();
    }

    private static String readAlphanumericVarying(int length, int scale, CobolDataStorage storage) {
        int lenSize = readVaryingHeaderLength(storage);
        byte[] data = storage.getByteArray(OCDB_VARCHAR_HEADER_BYTE, lenSize);
        return new String(data, SHIFT_JIS);
    }

    private static String readJapaneseVarying(int length, int scale, CobolDataStorage storage) {
        int charCount = readVaryingHeaderLength(storage);
        byte[] data = storage.getByteArray(OCDB_VARCHAR_HEADER_BYTE, charCount * 2);
        return new String(data, SHIFT_JIS);
    }

    private static String removeLeadingZeros(byte[] data, boolean hasLeadingSign) {
        int start = 0;
        if (hasLeadingSign && data.length > 0 && (data[0] == (byte) '-' || data[0] == (byte) '+')) {
            start = 1;
        }

        int i = start;
        while (i < data.length && data[i] == (byte) '0') {
            i++;
        }

        String digits;
        if (i == data.length) {
            digits = "0";
        } else if (data[i] == (byte) '.') {
            digits = "0" + new String(data, i, data.length - i);
        } else {
            digits = new String(data, i, data.length - i);
        }

        if (hasLeadingSign && data.length > 0 && data[0] == (byte) '-' && !"0".equals(digits)) {
            return "-" + digits;
        }
        return digits;
    }

    // -------------------------------------------------------
    // stringToCobol: Write SQL result data back to COBOL storage
    // -------------------------------------------------------
    /**
     * Write SQL result data back into COBOL host variable storage.
     *
     * @param field the target COBOL host variable field
     * @param resultData the raw byte data from the SQL result
     */
    /**
     * Write SQL result data to a specific COBOL storage location.
     * Used for OCCURS arrays where the storage offset varies per row.
     *
     * @param field the COBOL field (for type resolution)
     * @param storage the target storage location
     * @param length the field byte length
     * @param resultData the SQL result data as bytes
     */
    static void stringToCobolRaw(
            AbstractCobolField field, CobolDataStorage storage, int length, byte[] resultData) {
        if (field == null || storage == null || resultData == null) {
            return;
        }
        int scale = Math.min(0, -field.getAttribute().getScale());
        HvarType hvarType = resolveHvarType(field);
        // For VARYING, length = ARR size
        if (hvarType == HvarType.ALPHANUMERIC_VARYING || hvarType == HvarType.JAPANESE_VARYING) {
            length = field.getSize() - OCDB_VARCHAR_HEADER_BYTE;
        }
        // For packed decimal, use digit count (excluding PP) instead of byte size
        if (hvarType == HvarType.UNSIGNED_PACKED || hvarType == HvarType.SIGNED_PACKED) {
            int rawScale = field.getAttribute().getScale();
            length = field.getAttribute().getDigits();
            if (rawScale < 0) {
                length += rawScale;
                // PP: strip trailing zeros from SQL result, use positive scale for alignment
                resultData = stripTrailingDigits(resultData, -rawScale);
            }
        }
        stringToCobolInternal(hvarType, length, scale, storage, resultData);
    }

    /**
     * Write SQL result data back to a COBOL host variable field.
     *
     * @param field the target COBOL field
     * @param resultData the SQL result data as bytes
     */
    static void stringToCobol(AbstractCobolField field, byte[] resultData) {
        if (field == null || field.getDataStorage() == null || resultData == null) {
            return;
        }
        HvarType hvarType = resolveHvarType(field);
        int length = field.getSize();
        // For packed decimal, use digit count (excluding PP) instead of byte size
        if (hvarType == HvarType.UNSIGNED_PACKED || hvarType == HvarType.SIGNED_PACKED) {
            int rawScale = field.getAttribute().getScale();
            length = field.getAttribute().getDigits();
            if (rawScale < 0) {
                length += rawScale;
                // PP: strip trailing zeros from SQL result
                resultData = stripTrailingDigits(resultData, -rawScale);
            }
        }
        // For VARYING, length = ARR size (total - 4 byte header)
        if (hvarType == HvarType.ALPHANUMERIC_VARYING || hvarType == HvarType.JAPANESE_VARYING) {
            length = field.getSize() - OCDB_VARCHAR_HEADER_BYTE;
        }
        stringToCobolInternal(
                hvarType,
                length,
                -field.getAttribute().getScale(),
                field.getDataStorage(),
                resultData);
    }

    private static void stringToCobolInternal(
            HvarType hvarType, int length, int scale, CobolDataStorage storage, byte[] resultData) {
        switch (hvarType) {
            case UNSIGNED_NUMERIC:
                writeNumericDisplay(length, scale, storage, resultData, DisplaySign.UNSIGNED);
                break;
            case SIGNED_TRAILING_COMBINED:
                writeNumericDisplay(length, scale, storage, resultData, DisplaySign.TRAILING_COMBINED);
                break;
            case SIGNED_TRAILING_SEPARATE:
                writeNumericDisplay(length, scale, storage, resultData, DisplaySign.TRAILING_SEPARATE);
                break;
            case SIGNED_LEADING_SEPARATE:
                writeNumericDisplay(length, scale, storage, resultData, DisplaySign.LEADING_SEPARATE);
                break;
            case SIGNED_LEADING_COMBINED:
                writeNumericDisplay(length, scale, storage, resultData, DisplaySign.LEADING_COMBINED);
                break;
            case UNSIGNED_PACKED:
                writeUnsignedPacked(length, scale, storage, resultData);
                break;
            case SIGNED_PACKED:
                writeSignedPacked(length, scale, storage, resultData);
                break;
            case ALPHABETIC:
            case GROUP:
                writeAlphanumeric(length, scale, storage, resultData);
                break;
            case NATIONAL:
                writeNational(length, scale, storage, resultData);
                break;
            case ALPHANUMERIC_VARYING:
                writeAlphanumericVarying(length, scale, storage, resultData);
                break;
            case JAPANESE_VARYING:
                writeJapaneseVarying(length, scale, storage, resultData);
                break;
            case UNSIGNED_BINARY_NATIVE:
            case SIGNED_BINARY_NATIVE:
                writeAlphanumeric(length, scale, storage, resultData);
                break;
            case FLOAT:
                writeAlphanumeric(length, scale, storage, resultData);
                break;
            default:
                writeAlphanumeric(length, scale, storage, resultData);
                break;
        }
    }

    /**
     * Write a numeric value (rendered as an ASCII string, optionally with a leading '-' and a
     * '.') into a PIC 9 DISPLAY (zoned decimal) field. All five sign variants share this path;
     * they only differ in where the digit region sits and how the sign is applied.
     */
    private static void writeNumericDisplay(
            int length, int scale, CobolDataStorage storage, byte[] str, DisplaySign sign) {
        byte[] finalBuf = new byte[length];
        java.util.Arrays.fill(finalBuf, (byte) '0');
        boolean isNegative = str.length > 0 && str[0] == (byte) '-';
        int valueFirstIndex = isNegative ? 1 : 0;
        int indexOfDecimalPoint = indexOf(str, (byte) '.');
        if (indexOfDecimalPoint < 0) {
            indexOfDecimalPoint = str.length;
        }

        // Digit region within finalBuf; a separate sign byte sits just outside it
        // (leading -> index 0, trailing -> the last index).
        int regionStart = (sign == DisplaySign.LEADING_SEPARATE) ? 1 : 0;
        int regionEnd = (sign == DisplaySign.TRAILING_SEPARATE) ? length - 1 : length;

        placeDisplayDigits(
                finalBuf, str, valueFirstIndex, indexOfDecimalPoint, scale, regionStart, regionEnd);
        applyWriteSign(finalBuf, sign, isNegative);
        storage.memcpy(finalBuf, finalBuf.length);
    }

    /**
     * Place the (sign-stripped) digits of {@code str} into the digit region [{@code regionStart},
     * {@code regionEnd}) of {@code finalBuf}, aligned so that {@code scale} gives the implied
     * decimal position (scale &lt; 0 means {@code -scale} fractional digits; scale &ge; 0 is an
     * integer / trailing-P value).
     */
    private static void placeDisplayDigits(
            byte[] finalBuf,
            byte[] str,
            int valueFirstIndex,
            int indexOfDecimalPoint,
            int scale,
            int regionStart,
            int regionEnd) {
        if (scale >= 0) {
            for (int i = valueFirstIndex; i < indexOfDecimalPoint; i++) {
                int pos = i + regionEnd - (indexOfDecimalPoint + scale);
                if (pos >= regionStart && pos < regionEnd) {
                    finalBuf[pos] = str[i];
                }
            }
        } else {
            int fi = regionEnd + scale - 1;
            int si = indexOfDecimalPoint - 1;
            while (fi >= regionStart && si >= valueFirstIndex) {
                finalBuf[fi] = str[si];
                fi--;
                si--;
            }
            fi = regionEnd + scale;
            si = indexOfDecimalPoint + 1;
            while (fi < regionEnd && si < str.length) {
                finalBuf[fi] = str[si];
                fi++;
                si++;
            }
        }
    }

    /** Apply the operational sign to an already-formatted zoned-decimal buffer, in place. */
    private static void applyWriteSign(byte[] finalBuf, DisplaySign sign, boolean isNegative) {
        switch (sign) {
            case UNSIGNED:
                break;
            case LEADING_COMBINED:
                if (isNegative) {
                    finalBuf[0] = (byte) ((finalBuf[0] & 0xFF) + 0x40);
                }
                break;
            case TRAILING_COMBINED:
                if (isNegative) {
                    int last = finalBuf.length - 1;
                    finalBuf[last] = (byte) ((finalBuf[last] & 0xFF) + 0x40);
                }
                break;
            case LEADING_SEPARATE:
                finalBuf[0] = isNegative ? (byte) '-' : (byte) '+';
                break;
            case TRAILING_SEPARATE:
                finalBuf[finalBuf.length - 1] = isNegative ? (byte) '-' : (byte) '+';
                break;
            default:
                break;
        }
    }

    private static void writeUnsignedPacked(
            int length, int scale, CobolDataStorage storage, byte[] str) {
        int strStartIndex = 0;
        if (str.length > 0 && (str[0] == (byte) '+' || str[0] == (byte) '-')) {
            strStartIndex = 1;
        }
        int strPointIndex = indexOf(str, (byte) '.');
        if (strPointIndex < 0) {
            strPointIndex = str.length;
        }
        int dataPointIndex = length + scale;
        int realDataLength = (length / 2) + 1;

        storage.memset((byte) 0, realDataLength);
        storage.setByte(realDataLength - 1, (byte) 0x0F);

        for (int i = 0; i < length; i++) {
            int strIndex = i - dataPointIndex + strPointIndex;
            if (strIndex >= strPointIndex) {
                strIndex += 1;
            }
            byte digit;
            if (strIndex >= strStartIndex && strIndex < str.length) {
                digit = str[strIndex];
            } else {
                digit = (byte) '0';
            }
            int[] result = getPackedIndexAndByte(length, i, digit);
            int idx = result[0];
            byte byteValue = (byte) result[1];
            byte b = storage.getByte(idx);
            storage.setByte(idx, (byte) (b | byteValue));
        }
    }

    private static void writeSignedPacked(
            int length, int scale, CobolDataStorage storage, byte[] str) {
        int strStartIndex = 0;
        int sign = 1;
        if (str.length > 0 && str[0] == (byte) '-') {
            strStartIndex = 1;
            sign = -1;
        } else if (str.length > 0 && str[0] == (byte) '+') {
            strStartIndex = 1;
        }
        int strPointIndex = indexOf(str, (byte) '.');
        if (strPointIndex < 0) {
            strPointIndex = str.length;
        }
        int dataPointIndex = length + scale;
        int realDataLength = (length / 2) + 1;

        storage.memset((byte) 0, realDataLength);
        if (sign > 0) {
            storage.setByte(realDataLength - 1, (byte) 0x0C);
        } else {
            storage.setByte(realDataLength - 1, (byte) 0x0D);
        }

        for (int i = 0; i < length; i++) {
            int strIndex = i - dataPointIndex + strPointIndex;
            if (strIndex >= strPointIndex) {
                strIndex += 1;
            }
            byte digit;
            if (strIndex >= strStartIndex && strIndex < str.length) {
                digit = str[strIndex];
            } else {
                digit = (byte) '0';
            }
            int[] result = getPackedIndexAndByte(length, i, digit);
            int idx = result[0];
            byte byteValue = (byte) result[1];
            byte b = storage.getByte(idx);
            storage.setByte(idx, (byte) (b | byteValue));
        }
    }

    private static int[] getPackedIndexAndByte(int dataLen, int index, byte digit) {
        int d = (digit & 0xFF) - '0';
        if (dataLen % 2 == 0) {
            if (index % 2 == 0) {
                return new int[] {(index + 1) / 2, d};
            } else {
                return new int[] {(index + 1) / 2, d << 4};
            }
        } else {
            if (index % 2 == 0) {
                return new int[] {index / 2, d << 4};
            } else {
                return new int[] {index / 2, d};
            }
        }
    }

    private static void writeAlphanumeric(
            int length, int scale, CobolDataStorage storage, byte[] str) {
        if (str.length >= length) {
            storage.memcpy(str, length);
        } else {
            storage.memset((byte) ' ', length);
            storage.memcpy(str, str.length);
        }
    }

    private static void writeNational(int length, int scale, CobolDataStorage storage, byte[] str) {
        // length is already byte size (e.g. 10 for PIC N(5))
        for (int j = 0; j < length; j += 2) {
            storage.setByte(j, (byte) 0x30);
            storage.setByte(j + 1, (byte) 0x00);
        }
        int copyLen = Math.min(length, str.length);
        storage.memcpy(str, copyLen);
    }

    private static void writeAlphanumericVarying(
            int length, int scale, CobolDataStorage storage, byte[] str) {
        if (length <= 0) {
            return;
        }
        byte[] lengthBytes = new byte[4];
        if (str.length >= length) {
            ByteBuffer.wrap(lengthBytes).putInt(length);
            storage.memcpy(0, lengthBytes, OCDB_VARCHAR_HEADER_BYTE);
            storage.memcpy(OCDB_VARCHAR_HEADER_BYTE, str, length);
        } else {
            ByteBuffer.wrap(lengthBytes).putInt(str.length);
            storage.memset(OCDB_VARCHAR_HEADER_BYTE, (byte) ' ', length);
            storage.memcpy(0, lengthBytes, OCDB_VARCHAR_HEADER_BYTE);
            storage.memcpy(OCDB_VARCHAR_HEADER_BYTE, str, str.length);
        }
    }

    private static void writeJapaneseVarying(
            int length, int scale, CobolDataStorage storage, byte[] str) {
        byte[] lengthBytes = new byte[4];
        if (str.length >= length) {
            ByteBuffer.wrap(lengthBytes).putInt(length / 2);
            storage.memcpy(0, lengthBytes, OCDB_VARCHAR_HEADER_BYTE);
            storage.memcpy(OCDB_VARCHAR_HEADER_BYTE, str, length);
        } else {
            byte[] fillPair = new byte[] {(byte) 0x81, (byte) 0x40};
            for (int i = OCDB_VARCHAR_HEADER_BYTE;
                    i < OCDB_VARCHAR_HEADER_BYTE + length - 1;
                    i += 2) {
                storage.memcpy(i, fillPair, 2);
            }
            ByteBuffer.wrap(lengthBytes).putInt(str.length / 2);
            storage.memcpy(0, lengthBytes, OCDB_VARCHAR_HEADER_BYTE);
            storage.memcpy(OCDB_VARCHAR_HEADER_BYTE, str, str.length);
        }
    }

    /**
     * Strip trailing digits from a numeric string for PP (scaling position) fields. For example,
     * "999900" with count=2 becomes "9999". Handles optional leading sign.
     */
    private static byte[] stripTrailingDigits(byte[] data, int count) {
        if (count <= 0 || data.length == 0) {
            return data;
        }
        // Find the end of numeric content (skip sign at front if present)
        int start = 0;
        if (data[0] == (byte) '-' || data[0] == (byte) '+') {
            start = 1;
        }
        // Find decimal point position
        int dotPos = indexOf(data, (byte) '.');
        int endPos = dotPos >= 0 ? dotPos : data.length;
        // Strip 'count' digits from the integer part (before decimal)
        int intDigits = endPos - start;
        if (count >= intDigits) {
            // All integer digits would be stripped; return "0" with sign
            if (start > 0) {
                return new byte[] {data[0], (byte) '0'};
            }
            return new byte[] {(byte) '0'};
        }
        int newEnd = endPos - count;
        byte[] result = new byte[start + (newEnd - start) + (data.length - endPos)];
        if (start > 0) {
            result[0] = data[0];
        }
        System.arraycopy(data, start, result, start, newEnd - start);
        if (dotPos >= 0) {
            System.arraycopy(data, endPos, result, newEnd, data.length - endPos);
        }
        return result;
    }

    private static int indexOf(byte[] arr, byte target) {
        for (int i = 0; i < arr.length; i++) {
            if (arr[i] == target) {
                return i;
            }
        }
        return -1;
    }

    // -------------------------------------------------------
    // setParam: Bind COBOL value to PreparedStatement
    // -------------------------------------------------------
    /**
     * Bind a COBOL host variable value to a JDBC PreparedStatement parameter.
     *
     * @param stmt the JDBC prepared statement
     * @param index the 1-based parameter index
     * @param metaData parameter metadata for type inference (may be null)
     * @param field the COBOL host variable field to bind
     * @throws SQLException if a JDBC error occurs
     */
    static void setParam(
            PreparedStatement stmt, int index, ParameterMetaData metaData, AbstractCobolField field)
            throws SQLException {
        String str = cobolToString(field);
        if (metaData == null) {
            stmt.setString(index, str);
            return;
        }
        int paramType;
        try {
            paramType = metaData.getParameterType(index);
        } catch (SQLException e) {
            stmt.setString(index, str);
            return;
        }

        switch (paramType) {
            case Types.CHAR:
            case Types.VARCHAR:
            case Types.NCHAR:
            case Types.NVARCHAR:
            case Types.LONGVARCHAR:
            case Types.LONGNVARCHAR:
            case Types.CLOB:
            case Types.BLOB:
            case Types.ARRAY:
            case Types.OTHER:
            case Types.LONGVARBINARY:
            case Types.JAVA_OBJECT:
                stmt.setString(index, str);
                break;
            case Types.INTEGER:
            case Types.SMALLINT:
            case Types.TINYINT:
                stmt.setInt(index, Integer.parseInt(str));
                break;
            case Types.BIGINT:
            case Types.DECIMAL:
                stmt.setBigDecimal(index, new BigDecimal(str));
                break;
            case Types.NUMERIC:
                stmt.setDouble(index, Double.parseDouble(str));
                break;
            case Types.FLOAT:
                stmt.setFloat(index, Float.parseFloat(str));
                break;
            case Types.DOUBLE:
            case Types.REAL:
                stmt.setDouble(index, Double.parseDouble(str));
                break;
            case Types.DATE:
                stmt.setDate(index, java.sql.Date.valueOf(str));
                break;
            case Types.TIME:
            case Types.TIME_WITH_TIMEZONE:
                stmt.setTime(index, java.sql.Time.valueOf(str));
                break;
            case Types.TIMESTAMP:
            case Types.TIMESTAMP_WITH_TIMEZONE:
                stmt.setTimestamp(index, java.sql.Timestamp.valueOf(str));
                break;
            case Types.BOOLEAN:
            case Types.BIT:
                stmt.setBoolean(index, Boolean.valueOf(str));
                break;
            default:
                stmt.setString(index, str);
                break;
        }
    }

    // -------------------------------------------------------
    // getValueFromResultSet: Extract column value as SHIFT-JIS bytes
    // -------------------------------------------------------
    private static final String DATE_FORMAT_PATTERN = "yyyy-MM-dd HH:mm:ss";

    /**
     * Extract a column value from a ResultSet as SHIFT-JIS encoded bytes.
     *
     * @param rs the JDBC result set positioned on the current row
     * @param columnIndex the 1-based column index
     * @return the column value as a byte array, or null if the value is SQL NULL
     */
    static byte[] getValueFromResultSet(ResultSet rs, int columnIndex) {
        try {
            int colType = rs.getMetaData().getColumnType(columnIndex);
            String strValue;
            switch (colType) {
                case Types.CHAR:
                case Types.VARCHAR:
                case Types.NCHAR:
                case Types.NVARCHAR:
                case Types.LONGVARCHAR:
                case Types.LONGNVARCHAR:
                    {
                        String s = rs.getString(columnIndex);
                        if (s == null) {
                            return null;
                        }
                        return s.getBytes(SHIFT_JIS);
                    }
                case Types.DECIMAL:
                case Types.NUMERIC:
                    {
                        BigDecimal bd = rs.getBigDecimal(columnIndex);
                        if (bd == null) {
                            return null;
                        }
                        return bd.toString().getBytes();
                    }
                case Types.TIMESTAMP:
                    {
                        java.sql.Timestamp ts = rs.getTimestamp(columnIndex);
                        if (ts == null) {
                            return null;
                        }
                        return new SimpleDateFormat(DATE_FORMAT_PATTERN).format(ts).getBytes();
                    }
                case Types.DATE:
                    {
                        java.sql.Date d = rs.getDate(columnIndex);
                        if (d == null) {
                            return null;
                        }
                        return new SimpleDateFormat(DATE_FORMAT_PATTERN).format(d).getBytes();
                    }
                case Types.TIME:
                    {
                        java.sql.Time t = rs.getTime(columnIndex);
                        if (t == null) {
                            return null;
                        }
                        return t.toString().getBytes();
                    }
                case Types.TINYINT:
                    strValue = Byte.toString(rs.getByte(columnIndex));
                    break;
                case Types.SMALLINT:
                    strValue = Short.toString(rs.getShort(columnIndex));
                    break;
                case Types.INTEGER:
                    strValue = Integer.toString(rs.getInt(columnIndex));
                    break;
                case Types.BIGINT:
                    strValue = Long.toString(rs.getLong(columnIndex));
                    break;
                case Types.BOOLEAN:
                    strValue = Boolean.toString(rs.getBoolean(columnIndex));
                    break;
                case Types.FLOAT:
                case Types.DOUBLE:
                case Types.REAL:
                    strValue = Double.toString(rs.getDouble(columnIndex));
                    break;
                case Types.ROWID:
                    {
                        java.sql.RowId rowId = rs.getRowId(columnIndex);
                        if (rowId == null) {
                            return null;
                        }
                        return rowId.toString().getBytes();
                    }
                default:
                    {
                        String s = rs.getString(columnIndex);
                        if (s == null) {
                            return null;
                        }
                        return s.getBytes(SHIFT_JIS);
                    }
            }
            if (rs.wasNull()) {
                return null;
            }
            return strValue.getBytes();
        } catch (Exception e) {
            return null;
        }
    }
}

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
public final class CobolDataConverter {

    /** Private constructor to prevent instantiation of utility class. */
    private CobolDataConverter() {}

    /** Unsigned numeric display (USAGE DISPLAY, no sign). */
    public static final int TYPE_UNSIGNED_NUMERIC = 1;

    /** Signed numeric with trailing separate sign character. */
    public static final int TYPE_SIGNED_TRAILING_SEPARATE = 2;

    /** Signed numeric with trailing combined (overpunch) sign. */
    public static final int TYPE_SIGNED_TRAILING_COMBINED = 3;

    /** Signed numeric with leading separate sign character. */
    public static final int TYPE_SIGNED_LEADING_SEPARATE = 4;

    /** Signed numeric with leading combined (overpunch) sign. */
    public static final int TYPE_SIGNED_LEADING_COMBINED = 5;

    /** Unsigned packed-decimal (COMP-3, no sign nibble). */
    public static final int TYPE_UNSIGNED_PACKED = 8;

    /** Signed packed-decimal (COMP-3). */
    public static final int TYPE_SIGNED_PACKED = 9;

    /** Unsigned binary native (COMP-5, big-endian). */
    public static final int TYPE_UNSIGNED_BINARY_NATIVE = 13;

    /** Signed binary native (COMP-5, big-endian). */
    public static final int TYPE_SIGNED_BINARY_NATIVE = 14;

    /** Alphabetic (PIC A). */
    public static final int TYPE_ALPHABETIC = 16;

    /** Group item (treated as alphanumeric). */
    public static final int TYPE_GROUP = 22;

    /** Floating-point double (COMP-2). */
    public static final int TYPE_FLOAT = 23;

    /** National character (PIC N). */
    public static final int TYPE_NATIONAL = 24;

    /** Alphanumeric varying-length string. */
    public static final int TYPE_ALPHANUMERIC_VARYING = 30;

    /** Japanese (DBCS) varying-length string. */
    public static final int TYPE_JAPANESE_VARYING = 31;

    private static final Charset SHIFT_JIS = Charset.forName("SHIFT-JIS");
    private static final int SIGN_LENGTH = 1;
    private static final int OCDB_VARCHAR_HEADER_BYTE = 4;

    /**
     * Resolve the internal HVARTYPE constant from an AbstractCobolField's attributes.
     *
     * @param field the COBOL field
     * @return the HVARTYPE constant for internal dispatch
     */
    private static int resolveHvarType(AbstractCobolField field) {
        CobolFieldAttribute attr = field.getAttribute();
        int type = attr.getType();
        boolean hasSign = attr.isFlagHaveSign();
        boolean signSep = attr.isFlagSignSeparate();
        boolean signLead = attr.isFlagSignLeading();

        switch (type) {
            case CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY:
                if (!hasSign) {
                    return TYPE_UNSIGNED_NUMERIC;
                }
                if (signSep && signLead) {
                    return TYPE_SIGNED_LEADING_SEPARATE;
                }
                if (signSep) {
                    return TYPE_SIGNED_TRAILING_SEPARATE;
                }
                if (signLead) {
                    return TYPE_SIGNED_LEADING_COMBINED;
                }
                return TYPE_SIGNED_TRAILING_COMBINED;
            case CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED:
                return hasSign ? TYPE_SIGNED_PACKED : TYPE_UNSIGNED_PACKED;
            case CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY:
                return hasSign ? TYPE_SIGNED_BINARY_NATIVE : TYPE_UNSIGNED_BINARY_NATIVE;
            case CobolFieldAttribute.COB_TYPE_NUMERIC_FLOAT:
            case CobolFieldAttribute.COB_TYPE_NUMERIC_DOUBLE:
                return TYPE_FLOAT;
            case CobolFieldAttribute.COB_TYPE_NATIONAL:
                return TYPE_NATIONAL;
            case CobolFieldAttribute.COB_TYPE_GROUP:
                if (attr.isFlagVarying()) {
                    if ((attr.getFlags() & CobolFieldAttribute.COB_FLAG_NATIONAL_VARYING) != 0) {
                        return TYPE_JAPANESE_VARYING;
                    }
                    return TYPE_ALPHANUMERIC_VARYING;
                }
                return TYPE_GROUP;
            default:
                return TYPE_ALPHABETIC;
        }
    }

    /**
     * Convert a COBOL host variable to its string representation for SQL binding.
     *
     * @param field the COBOL host variable field
     * @return the string representation, or empty string if field is null
     */
    public static String cobolToString(AbstractCobolField field) {
        if (field == null || field.getDataStorage() == null) {
            return "";
        }
        int length = field.getSize();
        // V99 → getScale()=2 → scale=-2; PP → getScale()=-2 → scale=0 (for non-packed)
        int scale = Math.min(0, -field.getAttribute().getScale());
        CobolDataStorage storage = field.getDataStorage();
        int hvarType = resolveHvarType(field);

        // For VARYING, length = ARR size (total - 4 byte header)
        if (hvarType == TYPE_ALPHANUMERIC_VARYING || hvarType == TYPE_JAPANESE_VARYING) {
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
            case TYPE_UNSIGNED_NUMERIC:
                return readUnsignedNumeric(length, scale, storage);
            case TYPE_SIGNED_TRAILING_SEPARATE:
                return readSignedTrailingSeparate(length, scale, storage);
            case TYPE_SIGNED_TRAILING_COMBINED:
                return readSignedTrailingCombined(length, scale, storage);
            case TYPE_SIGNED_LEADING_SEPARATE:
                return readSignedLeadingSeparate(length, scale, storage);
            case TYPE_SIGNED_LEADING_COMBINED:
                return readSignedLeadingCombined(length, scale, storage);
            case TYPE_UNSIGNED_PACKED:
                return appendTrailingZeros(
                        readUnsignedPacked(packedLength, scale, storage), ppCount);
            case TYPE_SIGNED_PACKED:
                return appendTrailingZeros(readSignedPacked(packedLength, scale, storage), ppCount);
            case TYPE_UNSIGNED_BINARY_NATIVE:
                return readUnsignedBinaryNative(length, scale, storage);
            case TYPE_SIGNED_BINARY_NATIVE:
                return readSignedBinaryNative(length, scale, storage);
            case TYPE_ALPHABETIC:
            case TYPE_GROUP:
                return readAlphanumeric(length, scale, storage);
            case TYPE_FLOAT:
                return readFloat(length, scale, storage);
            case TYPE_NATIONAL:
                return readNational(length, scale, storage);
            case TYPE_ALPHANUMERIC_VARYING:
                return readAlphanumericVarying(length, scale, storage);
            case TYPE_JAPANESE_VARYING:
                return readJapaneseVarying(length, scale, storage);
            default:
                return readAlphanumeric(length, scale, storage);
        }
    }

    private static String readUnsignedNumeric(int length, int scale, CobolDataStorage storage) {
        byte[] data = storage.getByteArray(0, length);
        int realDataLength;
        if (scale < 0) {
            realDataLength = length + 1;
        } else {
            realDataLength = length + scale;
        }

        byte[] realData = new byte[realDataLength];
        java.util.Arrays.fill(realData, (byte) '0');
        System.arraycopy(data, 0, realData, 0, length);

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

    private static String readSignedTrailingCombined(
            int length, int scale, CobolDataStorage storage) {
        byte[] data = storage.getByteArray(0, length);

        int realDataLength;
        if (scale < 0) {
            realDataLength = SIGN_LENGTH + length + 1;
        } else {
            realDataLength = SIGN_LENGTH + length + scale;
        }

        byte[] realData = new byte[realDataLength];
        java.util.Arrays.fill(realData, (byte) '0');
        System.arraycopy(data, 0, realData, SIGN_LENGTH, length);

        byte signByte = realData[length + SIGN_LENGTH - 1];
        boolean isNegative = false;
        if ((signByte & 0xFF) >= 0x70 && (signByte & 0xFF) <= 0x79) {
            isNegative = true;
            realData[0] = (byte) '-';
            realData[length + SIGN_LENGTH - 1] = (byte) ((signByte & 0xFF) - 0x40);
        }

        if (scale < 0) {
            int pointIndex = realDataLength + scale - 1;
            if (pointIndex > 0 && pointIndex < realDataLength) {
                for (int i = realDataLength - 1; i > pointIndex; i--) {
                    realData[i] = realData[i - 1];
                }
                realData[pointIndex] = (byte) '.';
            }
        }

        return removeLeadingZeros(realData, isNegative);
    }

    private static String readSignedTrailingSeparate(
            int length, int scale, CobolDataStorage storage) {
        byte[] data = storage.getByteArray(0, length);
        boolean isNegative = data[length - 1] == (byte) '-';
        int digitLen = length - 1;

        int realDataLength;
        if (scale < 0) {
            realDataLength = digitLen + 1;
        } else {
            realDataLength = digitLen + scale;
        }

        byte[] realData = new byte[realDataLength];
        java.util.Arrays.fill(realData, (byte) '0');
        System.arraycopy(data, 0, realData, 0, digitLen);

        if (scale < 0) {
            int pointIndex = realDataLength + scale - 1;
            if (pointIndex > 0 && pointIndex < realDataLength) {
                for (int i = realDataLength - 1; i > pointIndex; i--) {
                    realData[i] = realData[i - 1];
                }
                realData[pointIndex] = (byte) '.';
            }
        }

        String result = removeLeadingZeros(realData, false);
        if (isNegative && !"0".equals(result)) {
            return "-" + result;
        }
        return result;
    }

    private static String readSignedLeadingSeparate(
            int length, int scale, CobolDataStorage storage) {
        byte[] data = storage.getByteArray(0, length + 1);
        String rawStr = new String(data);

        if (scale < 0) {
            int splitAt = length + scale;
            String fst = rawStr.substring(0, splitAt);
            String snd = rawStr.substring(splitAt);
            return fst + "." + snd;
        } else {
            StringBuilder sb = new StringBuilder(rawStr);
            for (int i = 0; i < scale; i++) {
                sb.append('0');
            }
            return sb.toString();
        }
    }

    private static String readSignedLeadingCombined(
            int length, int scale, CobolDataStorage storage) {
        byte[] data = storage.getByteArray(0, length);
        boolean isNegative = false;
        byte firstByte = data[0];
        if ((firstByte & 0xFF) >= 0x70 && (firstByte & 0xFF) <= 0x79) {
            isNegative = true;
            data[0] = (byte) ((firstByte & 0xFF) - 0x40);
        }

        int realDataLength;
        if (scale < 0) {
            realDataLength = length + 1;
        } else {
            realDataLength = length + scale;
        }

        byte[] realData = new byte[realDataLength];
        java.util.Arrays.fill(realData, (byte) '0');
        System.arraycopy(data, 0, realData, 0, length);

        if (scale < 0) {
            int pointIndex = realDataLength + scale - 1;
            if (pointIndex > 0 && pointIndex < realDataLength) {
                for (int i = realDataLength - 1; i > pointIndex; i--) {
                    realData[i] = realData[i - 1];
                }
                realData[pointIndex] = (byte) '.';
            }
        }

        String result = removeLeadingZeros(realData, false);
        if (isNegative && !"0".equals(result)) {
            return "-" + result;
        }
        return result;
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
        return new String(data, SHIFT_JIS);
    }

    private static String readAlphanumericVarying(int length, int scale, CobolDataStorage storage) {
        int lenSize = 0;
        for (int i = 0; i < OCDB_VARCHAR_HEADER_BYTE; i++) {
            lenSize = lenSize * 256 + (storage.getByte(i) & 0xFF);
        }
        byte[] data = storage.getByteArray(OCDB_VARCHAR_HEADER_BYTE, lenSize);
        return new String(data, SHIFT_JIS);
    }

    private static String readJapaneseVarying(int length, int scale, CobolDataStorage storage) {
        int charCount = 0;
        for (int i = 0; i < OCDB_VARCHAR_HEADER_BYTE; i++) {
            charCount = charCount * 256 + (storage.getByte(i) & 0xFF);
        }
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
    public static void stringToCobolRaw(
            AbstractCobolField field, CobolDataStorage storage, int length, byte[] resultData) {
        if (field == null || storage == null || resultData == null) {
            return;
        }
        int scale = Math.min(0, -field.getAttribute().getScale());
        int hvarType = resolveHvarType(field);
        // For VARYING, length = ARR size
        if (hvarType == TYPE_ALPHANUMERIC_VARYING || hvarType == TYPE_JAPANESE_VARYING) {
            length = field.getSize() - OCDB_VARCHAR_HEADER_BYTE;
        }
        // For packed decimal, use digit count (excluding PP) instead of byte size
        if (hvarType == TYPE_UNSIGNED_PACKED || hvarType == TYPE_SIGNED_PACKED) {
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
    public static void stringToCobol(AbstractCobolField field, byte[] resultData) {
        if (field == null || field.getDataStorage() == null || resultData == null) {
            return;
        }
        int hvarType = resolveHvarType(field);
        int length = field.getSize();
        // For packed decimal, use digit count (excluding PP) instead of byte size
        if (hvarType == TYPE_UNSIGNED_PACKED || hvarType == TYPE_SIGNED_PACKED) {
            int rawScale = field.getAttribute().getScale();
            length = field.getAttribute().getDigits();
            if (rawScale < 0) {
                length += rawScale;
                // PP: strip trailing zeros from SQL result
                resultData = stripTrailingDigits(resultData, -rawScale);
            }
        }
        // For VARYING, length = ARR size (total - 4 byte header)
        if (hvarType == TYPE_ALPHANUMERIC_VARYING || hvarType == TYPE_JAPANESE_VARYING) {
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
            int hvarType, int length, int scale, CobolDataStorage storage, byte[] resultData) {
        switch (hvarType) {
            case TYPE_UNSIGNED_NUMERIC:
                writeUnsignedNumeric(length, scale, storage, resultData);
                break;
            case TYPE_SIGNED_TRAILING_COMBINED:
                writeSignedTrailingCombined(length, scale, storage, resultData);
                break;
            case TYPE_SIGNED_TRAILING_SEPARATE:
                writeSignedTrailingSeparate(length, scale, storage, resultData);
                break;
            case TYPE_SIGNED_LEADING_SEPARATE:
                writeSignedLeadingSeparate(length, scale, storage, resultData);
                break;
            case TYPE_SIGNED_LEADING_COMBINED:
                writeSignedLeadingCombined(length, scale, storage, resultData);
                break;
            case TYPE_UNSIGNED_PACKED:
                writeUnsignedPacked(length, scale, storage, resultData);
                break;
            case TYPE_SIGNED_PACKED:
                writeSignedPacked(length, scale, storage, resultData);
                break;
            case TYPE_ALPHABETIC:
            case TYPE_GROUP:
                writeAlphanumeric(length, scale, storage, resultData);
                break;
            case TYPE_NATIONAL:
                writeNational(length, scale, storage, resultData);
                break;
            case TYPE_ALPHANUMERIC_VARYING:
                writeAlphanumericVarying(length, scale, storage, resultData);
                break;
            case TYPE_JAPANESE_VARYING:
                writeJapaneseVarying(length, scale, storage, resultData);
                break;
            case TYPE_UNSIGNED_BINARY_NATIVE:
            case TYPE_SIGNED_BINARY_NATIVE:
                writeAlphanumeric(length, scale, storage, resultData);
                break;
            case TYPE_FLOAT:
                writeAlphanumeric(length, scale, storage, resultData);
                break;
            default:
                writeAlphanumeric(length, scale, storage, resultData);
                break;
        }
    }

    private static void writeUnsignedNumeric(
            int length, int scale, CobolDataStorage storage, byte[] str) {
        byte[] finalBuf = new byte[length];
        java.util.Arrays.fill(finalBuf, (byte) '0');
        boolean isNegative = str.length > 0 && str[0] == (byte) '-';
        int valueFirstIndex = isNegative ? 1 : 0;
        int indexOfDecimalPoint = indexOf(str, (byte) '.');
        if (indexOfDecimalPoint < 0) {
            indexOfDecimalPoint = str.length;
        }

        if (scale >= 0) {
            for (int i = valueFirstIndex; i < indexOfDecimalPoint; i++) {
                int pos = i + finalBuf.length - (indexOfDecimalPoint + scale);
                if (pos >= 0 && pos < finalBuf.length) {
                    finalBuf[pos] = str[i];
                }
            }
        } else {
            int fi = length + scale - 1;
            int si = indexOfDecimalPoint - 1;
            while (fi >= 0 && si >= valueFirstIndex) {
                finalBuf[fi] = str[si];
                fi--;
                si--;
            }
            fi = length + scale;
            si = indexOfDecimalPoint + 1;
            while (fi < length && si < str.length) {
                finalBuf[fi] = str[si];
                fi++;
                si++;
            }
        }

        storage.memcpy(finalBuf, finalBuf.length);
    }

    private static void writeSignedTrailingCombined(
            int length, int scale, CobolDataStorage storage, byte[] str) {
        byte[] finalBuf = new byte[length];
        java.util.Arrays.fill(finalBuf, (byte) '0');
        boolean isNegative = str.length > 0 && str[0] == (byte) '-';
        int valueFirstIndex = isNegative ? 1 : 0;
        int indexOfDecimalPoint = indexOf(str, (byte) '.');
        if (indexOfDecimalPoint < 0) {
            indexOfDecimalPoint = str.length;
        }

        if (scale >= 0) {
            for (int i = valueFirstIndex; i < indexOfDecimalPoint; i++) {
                int pos = i + finalBuf.length - (indexOfDecimalPoint + scale);
                if (pos >= 0 && pos < finalBuf.length) {
                    finalBuf[pos] = str[i];
                }
            }
        } else {
            int fi = length + scale - 1;
            int si = indexOfDecimalPoint - 1;
            while (fi >= 0 && si >= valueFirstIndex) {
                finalBuf[fi] = str[si];
                fi--;
                si--;
            }
            fi = length + scale;
            si = indexOfDecimalPoint + 1;
            while (fi < length && si < str.length) {
                finalBuf[fi] = str[si];
                fi++;
                si++;
            }
        }

        if (isNegative) {
            int last = finalBuf.length - 1;
            finalBuf[last] = (byte) ((finalBuf[last] & 0xFF) + 0x40);
        }
        storage.memcpy(finalBuf, finalBuf.length);
    }

    private static void writeSignedTrailingSeparate(
            int length, int scale, CobolDataStorage storage, byte[] str) {
        byte[] finalBuf = new byte[length];
        java.util.Arrays.fill(finalBuf, (byte) '0');
        boolean isNegative = str.length > 0 && str[0] == (byte) '-';
        int valueFirstIndex = isNegative ? 1 : 0;
        int indexOfDecimalPoint = indexOf(str, (byte) '.');
        if (indexOfDecimalPoint < 0) {
            indexOfDecimalPoint = str.length;
        }
        int digitLen = length - 1;

        if (scale >= 0) {
            for (int i = valueFirstIndex; i < indexOfDecimalPoint; i++) {
                int pos = i + digitLen - (indexOfDecimalPoint + scale);
                if (pos >= 0 && pos < digitLen) {
                    finalBuf[pos] = str[i];
                }
            }
        } else {
            int fi = digitLen + scale - 1;
            int si = indexOfDecimalPoint - 1;
            while (fi >= 0 && si >= valueFirstIndex) {
                finalBuf[fi] = str[si];
                fi--;
                si--;
            }
            fi = digitLen + scale;
            si = indexOfDecimalPoint + 1;
            while (fi < digitLen && si < str.length) {
                finalBuf[fi] = str[si];
                fi++;
                si++;
            }
        }

        finalBuf[length - 1] = isNegative ? (byte) '-' : (byte) '+';
        storage.memcpy(finalBuf, finalBuf.length);
    }

    private static void writeSignedLeadingSeparate(
            int length, int scale, CobolDataStorage storage, byte[] str) {
        byte[] finalBuf = new byte[length];
        java.util.Arrays.fill(finalBuf, (byte) '0');
        boolean isNegative = str.length > 0 && str[0] == (byte) '-';
        int valueFirstIndex = isNegative ? 1 : 0;
        int indexOfDecimalPoint = indexOf(str, (byte) '.');
        if (indexOfDecimalPoint < 0) {
            indexOfDecimalPoint = str.length;
        }

        if (scale >= 0) {
            for (int i = valueFirstIndex; i < indexOfDecimalPoint; i++) {
                int pos = i + finalBuf.length - (indexOfDecimalPoint + scale);
                if (pos >= 0 && pos < finalBuf.length) {
                    finalBuf[pos] = str[i];
                }
            }
        } else {
            int fi = length + scale;
            int si = indexOfDecimalPoint - 1;
            while (fi >= 1 && si >= valueFirstIndex) {
                finalBuf[fi] = str[si];
                fi--;
                si--;
            }
            fi = length + scale + 1;
            si = indexOfDecimalPoint + 1;
            while (fi < length && si < str.length) {
                finalBuf[fi] = str[si];
                fi++;
                si++;
            }
        }

        finalBuf[0] = isNegative ? (byte) '-' : (byte) '+';
        storage.memcpy(finalBuf, finalBuf.length);
    }

    private static void writeSignedLeadingCombined(
            int length, int scale, CobolDataStorage storage, byte[] str) {
        byte[] finalBuf = new byte[length];
        java.util.Arrays.fill(finalBuf, (byte) '0');
        boolean isNegative = str.length > 0 && str[0] == (byte) '-';
        int valueFirstIndex = isNegative ? 1 : 0;
        int indexOfDecimalPoint = indexOf(str, (byte) '.');
        if (indexOfDecimalPoint < 0) {
            indexOfDecimalPoint = str.length;
        }

        if (scale >= 0) {
            for (int i = valueFirstIndex; i < indexOfDecimalPoint; i++) {
                int pos = i + finalBuf.length - (indexOfDecimalPoint + scale);
                if (pos >= 0 && pos < finalBuf.length) {
                    finalBuf[pos] = str[i];
                }
            }
        } else {
            int fi = length + scale - 1;
            int si = indexOfDecimalPoint - 1;
            while (fi >= 0 && si >= valueFirstIndex) {
                finalBuf[fi] = str[si];
                fi--;
                si--;
            }
            fi = length + scale;
            si = indexOfDecimalPoint + 1;
            while (fi < length && si < str.length) {
                finalBuf[fi] = str[si];
                fi++;
                si++;
            }
        }

        if (isNegative) {
            finalBuf[0] = (byte) ((finalBuf[0] & 0xFF) + 0x40);
        }
        storage.memcpy(finalBuf, finalBuf.length);
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
    public static void setParam(
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
    public static byte[] getValueFromResultSet(ResultSet rs, int columnIndex) {
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

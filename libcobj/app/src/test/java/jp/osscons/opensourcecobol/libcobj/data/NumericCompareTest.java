/*
 * Copyright (C) 2021-2022 TOKYO SYSTEM HOUSE Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3.0,
 * or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see the file COPYING.LIB.  If
 * not, write to the Free Software Foundation, 51 Franklin Street, Fifth Floor
 * Boston, MA 02110-1301 USA
 */
package jp.osscons.opensourcecobol.libcobj.data;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

class NumericCompareTest {

    // ========== Helper methods ==========

    /**
     * PIC 9(n) DISPLAY フィールドを作成する (符号なし)
     *
     * @param digits 桁数
     * @param scale 小数点以下桁数 (PPの場合は負)
     * @param value ASCII数字文字列 (例: "12345")
     */
    private static AbstractCobolField createDisplayField(int digits, int scale, String value) {
        return createDisplayField(digits, scale, 0, value);
    }

    /**
     * DISPLAY フィールドを作成する
     *
     * @param digits 桁数
     * @param scale 小数点以下桁数
     * @param flags フラグ (COB_FLAG_HAVE_SIGN等)
     * @param value ASCII数字文字列
     */
    private static AbstractCobolField createDisplayField(
            int digits, int scale, int flags, String value) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, digits, scale, flags, null);
        byte[] data = value.getBytes();
        return CobolFieldFactory.makeCobolField(data.length, new CobolDataStorage(data), attr);
    }

    /**
     * COMP-3 (packed decimal) フィールドを作成する
     *
     * @param digits 桁数
     * @param scale 小数点以下桁数
     * @param flags フラグ
     * @param data パックドデータ
     */
    private static AbstractCobolField createPackedField(
            int digits, int scale, int flags, byte[] data) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, digits, scale, flags, null);
        return CobolFieldFactory.makeCobolField(data.length, new CobolDataStorage(data), attr);
    }

    // ========== getDigitAt tests for DISPLAY fields ==========

    @Test
    void testGetDigitAt_Display_PIC9_5() {
        // PIC 9(5), value = 12345
        AbstractCobolField field = createDisplayField(5, 0, "12345");
        assertEquals(1, field.getDigitAt(4)); // 万の位
        assertEquals(2, field.getDigitAt(3)); // 千の位
        assertEquals(3, field.getDigitAt(2)); // 百の位
        assertEquals(4, field.getDigitAt(1)); // 十の位
        assertEquals(5, field.getDigitAt(0)); // 一の位
        assertEquals(0, field.getDigitAt(5)); // 範囲外
        assertEquals(0, field.getDigitAt(-1)); // 範囲外
    }

    @Test
    void testGetDigitAt_Display_PIC9V99() {
        // PIC 9(3)V99, value = 12345 => 123.45
        AbstractCobolField field = createDisplayField(5, 2, "12345");
        assertEquals(1, field.getDigitAt(2)); // 百の位
        assertEquals(2, field.getDigitAt(1)); // 十の位
        assertEquals(3, field.getDigitAt(0)); // 一の位
        assertEquals(4, field.getDigitAt(-1)); // 小数第一位
        assertEquals(5, field.getDigitAt(-2)); // 小数第二位
        assertEquals(0, field.getDigitAt(3)); // 範囲外
        assertEquals(0, field.getDigitAt(-3)); // 範囲外
    }

    @Test
    void testGetDigitAt_Display_Signed_TrailingEmbedded() {
        // PIC S9(5) SIGN TRAILING (embedded sign), positive value = 12345
        // 最後のバイトに符号が埋め込まれる: '5' = 0x35, 正の場合は0x35のまま
        AbstractCobolField field =
                createDisplayField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, "12345");
        assertEquals(1, field.getDigitAt(4));
        assertEquals(5, field.getDigitAt(0));
    }

    @Test
    void testGetDigitAt_Display_Signed_TrailingEmbedded_Negative() {
        // PIC S9(5) SIGN TRAILING (embedded sign), negative value
        // 最後のバイトが 0x75 (= '5' + 0x40) => 5, sign = negative
        byte[] data = new byte[] {0x31, 0x32, 0x33, 0x34, 0x75};
        AbstractCobolField field =
                createDisplayField(
                        5,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        new String(new byte[] {0x31, 0x32, 0x33, 0x34, 0x75}));
        assertEquals(1, field.getDigitAt(4));
        assertEquals(5, field.getDigitAt(0)); // 0x75 -> 5 (0x75 - 0x40 = 0x35, 0x35 - 0x30 = 5)
    }

    @Test
    void testGetDigitAt_Display_SignLeadingSeparate() {
        // PIC S9(5) SIGN LEADING SEPARATE, value = "+12345"
        int flags =
                CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                        | CobolFieldAttribute.COB_FLAG_SIGN_LEADING
                        | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE;
        AbstractCobolField field = createDisplayField(5, 0, flags, "+12345");
        // getFirstDataIndex = 1 (SIGN LEADING SEPARATE), getFieldSize = 5
        assertEquals(1, field.getDigitAt(4));
        assertEquals(2, field.getDigitAt(3));
        assertEquals(5, field.getDigitAt(0));
    }

    @Test
    void testGetDigitAt_Display_SignTrailingSeparate() {
        // PIC S9(5) SIGN TRAILING SEPARATE, value = "12345+"
        int flags =
                CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                        | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE;
        AbstractCobolField field = createDisplayField(5, 0, flags, "12345+");
        // getFirstDataIndex = 0, getFieldSize = 5, last byte is sign
        assertEquals(1, field.getDigitAt(4));
        assertEquals(5, field.getDigitAt(0));
    }

    @Test
    void testGetDigitAt_Display_PP() {
        // PIC 9999PP: digits=4, scale=-2
        // value "1234" represents 123400
        AbstractCobolField field = createDisplayField(4, -2, "1234");
        assertEquals(1, field.getDigitAt(5)); // 十万の位
        assertEquals(2, field.getDigitAt(4)); // 万の位
        assertEquals(3, field.getDigitAt(3)); // 千の位
        assertEquals(4, field.getDigitAt(2)); // 百の位
        assertEquals(0, field.getDigitAt(1)); // 十の位 (PP)
        assertEquals(0, field.getDigitAt(0)); // 一の位 (PP)
    }

    // ========== getDigitAt tests for COMP-3 fields ==========

    @Test
    void testGetDigitAt_Packed_PIC9_5() {
        // PIC 9(5) COMP-3, value = 12345
        // 5 digits, odd: packed as [0x12, 0x34, 0x5F] (unsigned = 0x0F)
        byte[] data = new byte[] {0x12, 0x34, 0x5F};
        AbstractCobolField field = createPackedField(5, 0, 0, data);
        assertEquals(1, field.getDigitAt(4));
        assertEquals(2, field.getDigitAt(3));
        assertEquals(3, field.getDigitAt(2));
        assertEquals(4, field.getDigitAt(1));
        assertEquals(5, field.getDigitAt(0));
        assertEquals(0, field.getDigitAt(5)); // 範囲外
        assertEquals(0, field.getDigitAt(-1)); // 範囲外
    }

    @Test
    void testGetDigitAt_Packed_PIC9V99() {
        // PIC 9(3)V99 COMP-3, value = 123.45
        // 5 digits, odd: packed as [0x12, 0x34, 0x5F]
        byte[] data = new byte[] {0x12, 0x34, 0x5F};
        AbstractCobolField field = createPackedField(5, 2, 0, data);
        assertEquals(1, field.getDigitAt(2)); // 百の位
        assertEquals(2, field.getDigitAt(1)); // 十の位
        assertEquals(3, field.getDigitAt(0)); // 一の位
        assertEquals(4, field.getDigitAt(-1)); // 小数第一位
        assertEquals(5, field.getDigitAt(-2)); // 小数第二位
        assertEquals(0, field.getDigitAt(3)); // 範囲外
    }

    @Test
    void testGetDigitAt_Packed_Signed() {
        // PIC S9(5) COMP-3, value = +12345
        // packed as [0x12, 0x34, 0x5C] (0x0C = positive)
        byte[] data = new byte[] {0x12, 0x34, 0x5C};
        AbstractCobolField field =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data);
        assertEquals(1, field.getDigitAt(4));
        assertEquals(5, field.getDigitAt(0));
    }

    @Test
    void testGetDigitAt_Packed_EvenDigits() {
        // PIC 9(6) COMP-3, value = 123456
        // 6 digits, even: packed as [0x01, 0x23, 0x45, 0x6F]
        // digit 0 (MSB) = 0x01 lower nibble = 1? Actually for even digits:
        // getByteIndex: (index + 1) / 2
        // getDigit parity: (digits + index) % 2
        // digit 0: (6+0)%2=0 -> lower nibble of byte[0+1)/2=0] = byte[0]&0x0F = 0x01 = 1 ???
        // Wait: for even digits, first byte upper nibble is padding (0)
        // Byte layout: [0x01, 0x23, 0x45, 0x6F]
        // digit 0: byte[(0+1)/2]=byte[0], (6+0)%2=0 -> byte[0]&0x0F = 0x01 => 1
        // digit 1: byte[(1+1)/2]=byte[1], (6+1)%2=1 -> (byte[1]>>>4)&0x0F = 0x02 => 2
        // digit 2: byte[(2+1)/2]=byte[1], (6+2)%2=0 -> byte[1]&0x0F = 0x03 => 3
        // digit 3: byte[(3+1)/2]=byte[2], (6+3)%2=1 -> (byte[2]>>>4)&0x0F = 0x04 => 4
        // digit 4: byte[(4+1)/2]=byte[2], (6+4)%2=0 -> byte[2]&0x0F = 0x05 => 5
        // digit 5: byte[(5+1)/2]=byte[3], (6+5)%2=1 -> (byte[3]>>>4)&0x0F = 0x06 => 6
        byte[] data = new byte[] {0x01, 0x23, 0x45, 0x6F};
        AbstractCobolField field = createPackedField(6, 0, 0, data);
        assertEquals(1, field.getDigitAt(5)); // digit index 0
        assertEquals(2, field.getDigitAt(4)); // digit index 1
        assertEquals(3, field.getDigitAt(3)); // digit index 2
        assertEquals(4, field.getDigitAt(2)); // digit index 3
        assertEquals(5, field.getDigitAt(1)); // digit index 4
        assertEquals(6, field.getDigitAt(0)); // digit index 5
    }

    @Test
    void testGetDigitAt_Packed_LargeNumber() {
        // PIC S9(10) COMP-3, value = +1234567890
        // 10 digits, even: packed as [0x01, 0x23, 0x45, 0x67, 0x89, 0x0C]
        byte[] data = new byte[] {0x01, 0x23, 0x45, 0x67, (byte) 0x89, 0x0C};
        AbstractCobolField field =
                createPackedField(10, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data);
        assertEquals(1, field.getDigitAt(9));
        assertEquals(2, field.getDigitAt(8));
        assertEquals(9, field.getDigitAt(1));
        assertEquals(0, field.getDigitAt(0));
    }

    // ========== numericCompareTo tests: COMP-3 vs COMP-3 ==========

    @Test
    void testCompare_Packed_Equal() {
        // PIC S9(5) COMP-3, both = +12345
        byte[] data1 = new byte[] {0x12, 0x34, 0x5C};
        byte[] data2 = new byte[] {0x12, 0x34, 0x5C};
        AbstractCobolField f1 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);
        AbstractCobolField f2 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data2);
        assertEquals(0, f1.numericCompareTo(f2));
    }

    @Test
    void testCompare_Packed_Greater() {
        // f1 = +12345, f2 = +12340
        byte[] data1 = new byte[] {0x12, 0x34, 0x5C};
        byte[] data2 = new byte[] {0x12, 0x34, 0x0C};
        AbstractCobolField f1 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);
        AbstractCobolField f2 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data2);
        assertTrue(f1.numericCompareTo(f2) > 0);
    }

    @Test
    void testCompare_Packed_Less() {
        // f1 = +10000, f2 = +12345
        byte[] data1 = new byte[] {0x10, 0x00, 0x0C};
        byte[] data2 = new byte[] {0x12, 0x34, 0x5C};
        AbstractCobolField f1 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);
        AbstractCobolField f2 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data2);
        assertTrue(f1.numericCompareTo(f2) < 0);
    }

    @Test
    void testCompare_Packed_DifferentSigns() {
        // f1 = +12345, f2 = -12345
        byte[] data1 = new byte[] {0x12, 0x34, 0x5C};
        byte[] data2 = new byte[] {0x12, 0x34, 0x5D};
        AbstractCobolField f1 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);
        AbstractCobolField f2 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data2);
        assertTrue(f1.numericCompareTo(f2) > 0);
    }

    @Test
    void testCompare_Packed_NegativeVsPositive() {
        // f1 = -12345, f2 = +00001
        byte[] data1 = new byte[] {0x12, 0x34, 0x5D};
        byte[] data2 = new byte[] {0x00, 0x00, 0x1C};
        AbstractCobolField f1 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);
        AbstractCobolField f2 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data2);
        assertTrue(f1.numericCompareTo(f2) < 0);
    }

    @Test
    void testCompare_Packed_ZeroVsZero_DifferentSigns() {
        // f1 = +0, f2 = -0, should be equal
        byte[] data1 = new byte[] {0x00, 0x00, 0x0C};
        byte[] data2 = new byte[] {0x00, 0x00, 0x0D};
        AbstractCobolField f1 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);
        AbstractCobolField f2 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data2);
        assertEquals(0, f1.numericCompareTo(f2));
    }

    @Test
    void testCompare_Packed_DifferentScales() {
        // f1: PIC S9(3)V99 COMP-3, value = 123.45
        // f2: PIC S9(5) COMP-3, value = 123
        byte[] data1 = new byte[] {0x12, 0x34, 0x5C};
        byte[] data2 = new byte[] {0x00, 0x12, 0x3C};
        AbstractCobolField f1 =
                createPackedField(5, 2, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);
        AbstractCobolField f2 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data2);
        // 123.45 > 123
        assertTrue(f1.numericCompareTo(f2) > 0);
    }

    @Test
    void testCompare_Packed_DifferentScales_Equal() {
        // f1: PIC S9(3)V99 COMP-3, value = 123.00
        // f2: PIC S9(5) COMP-3, value = 123
        byte[] data1 = new byte[] {0x12, 0x30, 0x0C};
        byte[] data2 = new byte[] {0x00, 0x12, 0x3C};
        AbstractCobolField f1 =
                createPackedField(5, 2, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);
        AbstractCobolField f2 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data2);
        assertEquals(0, f1.numericCompareTo(f2));
    }

    // ========== numericCompareTo tests: COMP-3 vs DISPLAY ==========

    @Test
    void testCompare_PackedVsDisplay_Equal() {
        // COMP-3: PIC S9(5) value = +12345
        byte[] packedData = new byte[] {0x12, 0x34, 0x5C};
        AbstractCobolField packed =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, packedData);

        // DISPLAY: PIC S9(5) value = +12345
        AbstractCobolField display =
                createDisplayField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, "12345");

        assertEquals(0, packed.numericCompareTo(display));
        assertEquals(0, display.numericCompareTo(packed));
    }

    @Test
    void testCompare_PackedVsDisplay_Greater() {
        // COMP-3: PIC S9(5) value = +12346
        byte[] packedData = new byte[] {0x12, 0x34, 0x6C};
        AbstractCobolField packed =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, packedData);

        // DISPLAY: PIC 9(5) value = 12345
        AbstractCobolField display = createDisplayField(5, 0, "12345");

        assertTrue(packed.numericCompareTo(display) > 0);
        assertTrue(display.numericCompareTo(packed) < 0);
    }

    @Test
    void testCompare_PackedVsDisplay_DifferentScales() {
        // COMP-3: PIC S9(3)V99, value = 123.45
        byte[] packedData = new byte[] {0x12, 0x34, 0x5C};
        AbstractCobolField packed =
                createPackedField(5, 2, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, packedData);

        // DISPLAY: PIC 9(5)V9999, value = 00123.4500
        AbstractCobolField display = createDisplayField(9, 4, "001234500");

        assertEquals(0, packed.numericCompareTo(display));
    }

    @Test
    void testCompare_PackedVsDisplay_SignedNegative() {
        // COMP-3: PIC S9(5) value = -12345
        byte[] packedData = new byte[] {0x12, 0x34, 0x5D};
        AbstractCobolField packed =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, packedData);

        // DISPLAY: PIC 9(5) value = 12345 (unsigned, treated as positive)
        AbstractCobolField display = createDisplayField(5, 0, "12345");

        assertTrue(packed.numericCompareTo(display) < 0);
    }

    // ========== numericCompareTo tests: DISPLAY vs DISPLAY (cross-scale) ==========

    @Test
    void testCompare_Display_DifferentScales() {
        // PIC 9(5)V99, value = 12345.67
        AbstractCobolField f1 = createDisplayField(7, 2, "1234567");
        // PIC 9(3)V9999, value = 123.4567
        AbstractCobolField f2 = createDisplayField(7, 4, "1234567");
        // 12345.67 > 123.4567
        assertTrue(f1.numericCompareTo(f2) > 0);
    }

    @Test
    void testCompare_Display_PP() {
        // PIC 9999PP: digits=4, scale=-2, value "0050" => 5000
        AbstractCobolField f1 = createDisplayField(4, -2, "0050");
        // PIC 9(6): digits=6, scale=0, value "005000"
        AbstractCobolField f2 = createDisplayField(6, 0, "005000");
        assertEquals(0, f1.numericCompareTo(f2));
    }

    @Test
    void testCompare_Display_PP_NotEqual() {
        // PIC 9999PP: digits=4, scale=-2, value "0050" => 5000
        AbstractCobolField f1 = createDisplayField(4, -2, "0050");
        // PIC 9(6): digits=6, scale=0, value "005001"
        AbstractCobolField f2 = createDisplayField(6, 0, "005001");
        assertTrue(f1.numericCompareTo(f2) < 0);
    }

    // ========== numericCompareTo tests: COMP-3 PP ==========

    @Test
    void testCompare_Packed_PP() {
        // PIC 9999PP COMP-3: digits=6, scale=-2
        // Compiler generates: size=3, digits=6, scale=-2
        // storedDigits = 6 + (-2) = 4
        // Physical: 3 bytes = [padding_nibble, d0, d1, d2, d3, sign_nibble]
        // For value 123400: stored digits 1,2,3,4
        // Bytes: [0x01, 0x23, 0x4F] (digits=6 even → padding nibble at start)
        byte[] data = new byte[] {0x01, 0x23, 0x4F};
        AbstractCobolField packed = createPackedField(6, -2, 0, data);

        assertEquals(1, packed.getDigitAt(5));
        assertEquals(2, packed.getDigitAt(4));
        assertEquals(3, packed.getDigitAt(3));
        assertEquals(4, packed.getDigitAt(2));
        assertEquals(0, packed.getDigitAt(1)); // PP
        assertEquals(0, packed.getDigitAt(0)); // PP

        // Compare with DISPLAY: PIC 9(6) value = 123400
        AbstractCobolField display = createDisplayField(6, 0, "123400");
        assertEquals(0, packed.numericCompareTo(display));
    }

    // ========== Large digit count tests ==========

    @Test
    void testCompare_Packed_LargeDigits() {
        // PIC S9(18) COMP-3
        // value = +123456789012345678
        // 18 digits, even: (18+2)/2 = 10 bytes
        byte[] data1 =
                new byte[] {
                    0x01, 0x23, 0x45, 0x67, (byte) 0x89, 0x01, 0x23, 0x45, 0x67, (byte) 0x8C
                };
        byte[] data2 =
                new byte[] {
                    0x01, 0x23, 0x45, 0x67, (byte) 0x89, 0x01, 0x23, 0x45, 0x67, (byte) 0x8C
                };
        AbstractCobolField f1 =
                createPackedField(18, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);
        AbstractCobolField f2 =
                createPackedField(18, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data2);
        assertEquals(0, f1.numericCompareTo(f2));
    }

    @Test
    void testCompare_Packed_LargeDigits_DifferentLastDigit() {
        // Same as above but last digit differs
        byte[] data1 =
                new byte[] {
                    0x01, 0x23, 0x45, 0x67, (byte) 0x89, 0x01, 0x23, 0x45, 0x67, (byte) 0x8C
                };
        byte[] data2 =
                new byte[] {
                    0x01, 0x23, 0x45, 0x67, (byte) 0x89, 0x01, 0x23, 0x45, 0x67, (byte) 0x9C
                };
        AbstractCobolField f1 =
                createPackedField(18, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);
        AbstractCobolField f2 =
                createPackedField(18, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data2);
        assertTrue(f1.numericCompareTo(f2) < 0);
    }

    // ========== Zero handling ==========

    @Test
    void testCompare_AllZeros() {
        // COMP-3: zero
        byte[] data1 = new byte[] {0x00, 0x00, 0x0C};
        AbstractCobolField f1 =
                createPackedField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);

        // DISPLAY: zero
        AbstractCobolField f2 =
                createDisplayField(5, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, "00000");

        assertEquals(0, f1.numericCompareTo(f2));
        assertEquals(0, f2.numericCompareTo(f1));
    }

    @Test
    void testCompare_ZeroDifferentScales() {
        // COMP-3: PIC S9(3)V99 = 000.00
        byte[] data1 = new byte[] {0x00, 0x00, 0x0C};
        AbstractCobolField f1 =
                createPackedField(5, 2, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);

        // DISPLAY: PIC 9(7) = 0000000
        AbstractCobolField f2 = createDisplayField(7, 0, "0000000");

        assertEquals(0, f1.numericCompareTo(f2));
    }

    // ========== Single digit fields ==========

    @Test
    void testCompare_SingleDigit() {
        // PIC 9 COMP-3, value = 5
        byte[] data1 = new byte[] {0x5F};
        AbstractCobolField f1 = createPackedField(1, 0, 0, data1);

        // PIC 9, value = 5
        AbstractCobolField f2 = createDisplayField(1, 0, "5");

        assertEquals(0, f1.numericCompareTo(f2));
    }

    @Test
    void testCompare_SingleDigit_Different() {
        // PIC 9 COMP-3, value = 3
        byte[] data1 = new byte[] {0x3F};
        AbstractCobolField f1 = createPackedField(1, 0, 0, data1);

        // PIC 9, value = 7
        AbstractCobolField f2 = createDisplayField(1, 0, "7");

        assertTrue(f1.numericCompareTo(f2) < 0);
        assertTrue(f2.numericCompareTo(f1) > 0);
    }

    // ========== Sign Leading Embedded tests ==========

    @Test
    void testGetDigitAt_Display_SignLeadingEmbedded_Negative() {
        // PIC S9(5) SIGN LEADING (embedded), negative value
        // Leading byte has sign embedded: 0x71 = '1' + 0x40 => digit 1, sign = negative
        int flags =
                CobolFieldAttribute.COB_FLAG_HAVE_SIGN | CobolFieldAttribute.COB_FLAG_SIGN_LEADING;
        byte[] data = new byte[] {0x71, 0x32, 0x33, 0x34, 0x35};
        AbstractCobolField field =
                createDisplayField(5, 0, flags, new String(data));
        assertEquals(1, field.getDigitAt(4)); // 0x71 - 0x40 = 0x31, - 0x30 = 1
        assertEquals(2, field.getDigitAt(3));
        assertEquals(5, field.getDigitAt(0));
    }

    // ========== Comparison involving different field widths ==========

    @Test
    void testCompare_DifferentWidths() {
        // COMP-3: PIC S9(3) = +123
        byte[] data1 = new byte[] {0x12, 0x3C};
        AbstractCobolField f1 =
                createPackedField(3, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);

        // DISPLAY: PIC 9(7) = 0000123
        AbstractCobolField f2 = createDisplayField(7, 0, "0000123");

        assertEquals(0, f1.numericCompareTo(f2));
        assertEquals(0, f2.numericCompareTo(f1));
    }

    @Test
    void testCompare_PackedVsDisplay_WithScale() {
        // COMP-3: PIC S9(5)V99 = +12345.67
        // 7 digits, odd: [0x12, 0x34, 0x56, 0x7C]
        byte[] data1 = new byte[] {0x12, 0x34, 0x56, 0x7C};
        AbstractCobolField f1 =
                createPackedField(7, 2, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);

        // DISPLAY: PIC 9(5)V99 = 1234567 (=> 12345.67)
        AbstractCobolField f2 = createDisplayField(7, 2, "1234567");

        assertEquals(0, f1.numericCompareTo(f2));
    }

    // ========== Negative number comparisons ==========

    @Test
    void testCompare_BothNegative_Packed() {
        // f1 = -100, f2 = -200. f1 > f2 (since -100 > -200)
        byte[] data1 = new byte[] {0x10, 0x0D};
        byte[] data2 = new byte[] {0x20, 0x0D};
        AbstractCobolField f1 =
                createPackedField(3, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);
        AbstractCobolField f2 =
                createPackedField(3, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data2);
        assertTrue(f1.numericCompareTo(f2) > 0);
    }

    @Test
    void testCompare_BothNegative_PackedVsDisplay() {
        // COMP-3: PIC S9(3) = -100
        byte[] data1 = new byte[] {0x10, 0x0D};
        AbstractCobolField f1 =
                createPackedField(3, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, data1);

        // DISPLAY: PIC S9(3) = -200 (trailing embedded sign: "20r" where 'r' = 0x72 = negative 2)
        // Wait, for trailing embedded sign on digit '0': '0' = 0x30, negative => 0x70
        byte[] data2 = new byte[] {0x32, 0x30, 0x70};
        AbstractCobolField f2 =
                createDisplayField(
                        3,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        new String(data2));
        assertTrue(f1.numericCompareTo(f2) > 0); // -100 > -200
    }

    // ========== SIGN LEADING SEPARATE DISPLAY-DISPLAY comparison ==========

    @Test
    void testCompare_Display_SignLeadingSeparate() {
        int flags =
                CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                        | CobolFieldAttribute.COB_FLAG_SIGN_LEADING
                        | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE;
        // f1 = +12345, f2 = +12346
        AbstractCobolField f1 = createDisplayField(5, 0, flags, "+12345");
        AbstractCobolField f2 = createDisplayField(5, 0, flags, "+12346");
        assertTrue(f1.numericCompareTo(f2) < 0);

        // f3 = -12345, f4 = +12345
        AbstractCobolField f3 = createDisplayField(5, 0, flags, "-12345");
        AbstractCobolField f4 = createDisplayField(5, 0, flags, "+12345");
        assertTrue(f3.numericCompareTo(f4) < 0);
        assertTrue(f4.numericCompareTo(f3) > 0);
    }

    @Test
    void testCompare_Display_SignLeadingSeparate_BothNegative() {
        int flags =
                CobolFieldAttribute.COB_FLAG_HAVE_SIGN
                        | CobolFieldAttribute.COB_FLAG_SIGN_LEADING
                        | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE;
        // -100 > -200
        AbstractCobolField f1 = createDisplayField(3, 0, flags, "-100");
        AbstractCobolField f2 = createDisplayField(3, 0, flags, "-200");
        assertTrue(f1.numericCompareTo(f2) > 0);
    }

    // ========== PP field vs PP field comparisons ==========

    @Test
    void testCompare_Display_PP_vs_PP() {
        // PIC 9999PP (digits=6, scale=-2) value "0050" => 5000
        AbstractCobolField f1 = createDisplayField(6, -2, "0050");
        // PIC 99PP (digits=4, scale=-2) value "50" => 5000
        AbstractCobolField f2 = createDisplayField(4, -2, "50");
        assertEquals(0, f1.numericCompareTo(f2));
    }

    @Test
    void testCompare_Display_PP_vs_PP_NotEqual() {
        // PIC 9999PP (digits=6, scale=-2) value "0051" => 5100
        AbstractCobolField f1 = createDisplayField(6, -2, "0051");
        // PIC 99PP (digits=4, scale=-2) value "50" => 5000
        AbstractCobolField f2 = createDisplayField(4, -2, "50");
        assertTrue(f1.numericCompareTo(f2) > 0);
    }

    @Test
    void testCompare_Packed_PP_vs_Packed_PP() {
        // PIC 9999PP COMP-3 (digits=6, scale=-2), value 5000 => stored digits 0,0,5,0
        // 3 bytes: [0x00, 0x05, 0x0F]
        byte[] data1 = new byte[] {0x00, 0x05, 0x0F};
        AbstractCobolField f1 = createPackedField(6, -2, 0, data1);

        // PIC 99PP COMP-3 (digits=4, scale=-2), value 5000 => stored digits 5,0
        // 2 bytes: [0x50, 0x0F]? Actually digits=4, even -> (4+2)/2=3? No.
        // size based on stored digits: storedDigits=2, packed size = (2+2)/2 = 2 bytes
        // But getDigit uses digits=4 for parity. digits=4 even:
        //   getByteIndex(0) = (0+1)/2=0, (4+0)%2=0 -> byte[0]&0x0F
        //   getByteIndex(1) = (1+1)/2=1, (4+1)%2=1 -> (byte[1]>>>4)&0x0F
        // So byte[0]=0x05 -> getDigit(0)=5, byte[1]=0x0F -> getDigit(1)=0
        byte[] data2 = new byte[] {0x05, 0x0F};
        AbstractCobolField f2 = createPackedField(4, -2, 0, data2);

        assertEquals(0, f1.numericCompareTo(f2));
    }

    // ========== Zero with different signs in DISPLAY ==========

    @Test
    void testCompare_Display_NegativeZero_vs_PositiveZero() {
        // -0 vs +0 should be equal
        byte[] data1 = new byte[] {0x30, 0x30, 0x70}; // "00" with negative sign on last byte
        AbstractCobolField f1 =
                createDisplayField(
                        3,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        new String(data1));

        AbstractCobolField f2 =
                createDisplayField(3, 0, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, "000");

        assertEquals(0, f1.numericCompareTo(f2));
    }
}

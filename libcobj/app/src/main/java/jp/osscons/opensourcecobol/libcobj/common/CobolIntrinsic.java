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
package jp.osscons.opensourcecobol.libcobj.common;

import java.math.BigDecimal;
import java.text.DateFormat;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Locale;
import java.util.Random;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolDecimal;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory;
import jp.osscons.opensourcecobol.libcobj.data.CobolNationalField;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolExceptionId;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;
import jp.osscons.opensourcecobol.libcobj.file.CobolFile;

/**
 * COBOLの組み込み関数（Intrinsic Functions）を実装するクラス。
 *
 * <p>COBOL 85拡張およびCOBOL 2002で定義された組み込み関数を提供する。
 * 日付・時刻関数、数学関数、文字列関数、変換関数などが含まれる。
 *
 * <p>使用例（COBOL）：
 * <pre>
 *     COMPUTE WS-RESULT = FUNCTION LENGTH(WS-STRING).
 *     MOVE FUNCTION CURRENT-DATE TO WS-DATE.
 *     COMPUTE WS-SQRT = FUNCTION SQRT(WS-NUM).
 * </pre>
 *
 * <p>libcob/intrinsic.cの各関数に対応する。
 */
public class CobolIntrinsic {

    private static int[] normalDays = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    private static int[] leapDays = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
    private static int[] normalMonthDays = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    private static int[] leapMonthDays = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    private static final int DEPTH_LEVEL = 8;
    private static final int sizeOfDouble = 8;
    private static int currEntry = 0;
    private static AbstractCobolField currField = null;
    private static AbstractCobolField[] calcField = new AbstractCobolField[DEPTH_LEVEL];
    private static Random random = new Random();
    private static byte[] localeBuff;
    private static final byte[] byteArray00 = "00".getBytes(AbstractCobolField.charSetSJIS);

    /** libcob/intrinsicのmake_double_entryの実装 */
    private static void makeDoubleEntry() {
        CobolDataStorage s = new CobolDataStorage(sizeOfDouble + 1);

        CobolFieldAttribute newAttr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DOUBLE,
                        18,
                        9,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField newField = CobolFieldFactory.makeCobolField(sizeOfDouble, s, newAttr);

        calcField[currEntry] = newField;
        currField = newField;
        ++currEntry;
        if (currEntry >= DEPTH_LEVEL) {
            currEntry = 0;
        }
    }

    /** libcob/intrinsicのmake_field_entryの実装 */
    private static void makeFieldEntry(AbstractCobolField f) {
        AbstractCobolField newField =
                CobolFieldFactory.makeCobolField(
                        f.getSize(), new CobolDataStorage(f.getSize() + 1), f.getAttribute());
        calcField[currEntry] = newField;
        currField = calcField[currEntry];

        ++currEntry;
        if (currEntry >= DEPTH_LEVEL) {
            currEntry = 0;
        }
    }

    /**
     * 指定された年がうるう年かどうかを判定する。
     *
     * @param year 判定対象の西暦年
     * @return うるう年の場合true、それ以外はfalse
     */
    private static boolean isLeapYear(int year) {
        return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
    }

    /**
     * 組み込み関数モジュールを初期化する。libcob/intrinsic.cのcob_init_intrinsicに対応。
     *
     * <p>計算用の一時フィールド配列を初期化する。ランタイム初期化時にCobolUtil.cob_initから呼び出される。
     */
    public static void init() {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        for (int i = 0; i < DEPTH_LEVEL; ++i) {
            calcField[i] = CobolFieldFactory.makeCobolField(256, new CobolDataStorage(256), attr);
        }
    }

    /**
     * CobolDecimalをdouble型に変換する。libcob/intrinsic.cのcob_intr_get_doubleに対応。
     *
     * <p>スケール（小数点位置）を考慮して数値を変換する。
     *
     * @param d 変換対象のCobolDecimal
     * @return double型に変換された値
     */
    private static double intrGetDouble(CobolDecimal d) {
        double v = d.getValue().doubleValue();
        int n = d.getScale();
        for (int i = 0; i < Math.abs(n); ++i) {
            if (n > 0) {
                v /= 10;
            } else {
                v *= 10;
            }
        }
        return v;
    }

    /**
     * フィールドの参照修飾（Reference Modification）を適用する。
     *
     * <p>COBOLの参照修飾「identifier(leftmost-character-position: length)」に対応。
     * フィールドの部分文字列を切り出す。
     *
     * @param f 参照修飾を適用するフィールド
     * @param offset 開始位置（1から始まる）
     * @param length 切り出す長さ（0の場合は末尾まで）
     */
    private static void calcRefMod(AbstractCobolField f, int offset, int length) {
        if (offset <= f.getSize()) {
            int calcoff = offset - 1;
            int size = f.getSize() - calcoff;
            if (length > 0 && length < size) {
                size = length;
            }
            f.setSize(size);
            if (calcoff > 0) {
                CobolDataStorage tmp = new CobolDataStorage(size);
                tmp.memcpy(f.getDataStorage().getSubDataStorage(calcoff), size);
                f.getDataStorage().memcpy(tmp, size);
            }
        }
    }

    /**
     * 2つの数値フィールドに対して二項演算を行う。
     *
     * <p>組み込み関数内で使用される算術演算を実行する。
     * 対応する演算子: '+' (加算), '-' (減算), '*' (乗算), '/' (除算), '^' (べき乗)
     *
     * @param f1 左オペランド
     * @param op 演算子（文字コード）
     * @param f2 右オペランド
     * @return 演算結果を格納した数値フィールド
     * @throws CobolStopRunException 演算中にエラーが発生した場合
     */
    public static AbstractCobolField intrBinop(AbstractCobolField f1, int op, AbstractCobolField f2)
            throws CobolStopRunException {
        CobolDecimal d1 = new CobolDecimal();
        CobolDecimal d2 = new CobolDecimal();
        d1.setField(f1);
        d2.setField(f2);

        switch ((char) op) {
            case '+':
                d1.add(d2);
                break;
            case '-':
                d1.sub(d2);
                break;
            case '*':
                d1.mul(d2);
                break;
            case '/':
                d1.div(d2);
                break;
            case '^':
                d1.pow(d2);
                break;
            default:
                break;
        }

        int attrsign = 0;
        if (d1.getValue().signum() < 0) {
            attrsign = CobolFieldAttribute.COB_FLAG_HAVE_SIGN;
        } else {
            attrsign = 0;
        }

        int size = sizeInBase10(d1.getValue());
        if (d1.getScale() > size) {
            size = d1.getScale();
        }
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        size,
                        d1.getScale(),
                        attrsign,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(size, (CobolDataStorage) null, attr);
        makeFieldEntry(field);
        d1.getDisplayField(currField, 0);
        return currField;
    }

    /**
     * FUNCTION LENGTH - フィールドの長さを返す。
     *
     * <p>COBOL文「FUNCTION LENGTH(field)」に対応。フィールドのバイト数を返す。
     *
     * @param srcfield 長さを取得するフィールド
     * @return フィールドの長さ（バイト数）を格納した数値フィールド
     */
    public static AbstractCobolField funcLength(AbstractCobolField srcfield) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 8, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, (CobolDataStorage) null, attr);
        makeFieldEntry(field);
        currField.setInt(srcfield.getSize());
        return currField;
    }

    /**
     * FUNCTION INTEGER - 引数以下の最大整数を返す（床関数）。
     *
     * <p>COBOL文「FUNCTION INTEGER(number)」に対応。正数の場合は整数部分、
     * 負数の場合は引数以下の最大整数（負の無限大方向に丸め）を返す。
     *
     * @param srcfield 数値フィールド
     * @return 整数値を格納した数値フィールド
     */
    public static AbstractCobolField funcInteger(AbstractCobolField srcfield) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        CobolDecimal d1 = new CobolDecimal();
        d1.setField(srcfield);
        if (d1.getValue().signum() >= 0) {
            try {
                d1.getField(currField, 0);
            } catch (CobolStopRunException e) {
                return null;
            }
            return currField;
        }

        boolean isScalePositive = d1.getScale() > 0;
        BigDecimal val = d1.getValue();
        for (int i = 0; i < Math.abs(d1.getScale()); ++i) {
            if (isScalePositive) {
                val = val.divide(BigDecimal.TEN);
            } else {
                val = val.multiply(BigDecimal.TEN);
            }
        }

        // Rouding to negative infinity
        BigDecimal[] vals = val.divideAndRemainder(BigDecimal.ONE);
        if (vals[1].signum() != 0) {
            vals[0] = vals[0].subtract(BigDecimal.ONE);
        }

        try {
            new CobolDecimal(vals[0], 0).getField(currField, 0);
        } catch (CobolStopRunException e) {
            return null;
        }
        return currField;
    }

    /**
     * FUNCTION INTEGER-PART - 数値の整数部分を返す。
     *
     * <p>COBOL文「FUNCTION INTEGER-PART(number)」に対応。小数点以下を切り捨てて整数部分のみを返す。
     *
     * @param srcfield 数値フィールド
     * @return 整数部分を格納した数値フィールド
     */
    public static AbstractCobolField funcIntegerPart(AbstractCobolField srcfield) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);

        makeFieldEntry(field);
        currField.moveFrom(srcfield);
        return currField;
    }

    /**
     * FUNCTION UPPER-CASE - 文字列を大文字に変換する。
     *
     * <p>COBOL文「FUNCTION UPPER-CASE(string)」に対応。小文字を大文字に変換する。
     *
     * @param offset 参照修飾の開始位置（0の場合は修飾なし）
     * @param length 参照修飾の長さ
     * @param srcfield 変換対象の文字列フィールド
     * @return 大文字に変換された文字列フィールド
     */
    public static AbstractCobolField funcUpperCase(
            int offset, int length, AbstractCobolField srcfield) {
        makeFieldEntry(srcfield);
        int size = srcfield.getSize();
        CobolDataStorage currStorage = currField.getDataStorage();
        CobolDataStorage srcStorage = srcfield.getDataStorage();
        for (int i = 0; i < size; ++i) {
            currStorage.setByte(i, (byte) Character.toUpperCase(srcStorage.getByte(i)));
        }
        if (offset > 0) {
            calcRefMod(currField, offset, length);
        }
        return currField;
    }

    /**
     * FUNCTION LOWER-CASE - 文字列を小文字に変換する。
     *
     * <p>COBOL文「FUNCTION LOWER-CASE(string)」に対応。大文字を小文字に変換する。
     *
     * @param offset 参照修飾の開始位置（0の場合は修飾なし）
     * @param length 参照修飾の長さ
     * @param srcfield 変換対象の文字列フィールド
     * @return 小文字に変換された文字列フィールド
     */
    public static AbstractCobolField funcLowerCase(
            int offset, int length, AbstractCobolField srcfield) {
        makeFieldEntry(srcfield);
        int size = srcfield.getSize();
        CobolDataStorage currStorage = currField.getDataStorage();
        CobolDataStorage srcStorage = srcfield.getDataStorage();
        for (int i = 0; i < size; ++i) {
            currStorage.setByte(i, (byte) Character.toLowerCase(srcStorage.getByte(i)));
        }
        if (offset > 0) {
            calcRefMod(currField, offset, length);
        }
        return currField;
    }

    /**
     * FUNCTION REVERSE - 文字列を逆順にする。
     *
     * <p>COBOL文「FUNCTION REVERSE(string)」に対応。文字列の文字順序を反転させる。
     *
     * @param offset 参照修飾の開始位置（0の場合は修飾なし）
     * @param length 参照修飾の長さ
     * @param srcfield 反転対象の文字列フィールド
     * @return 反転された文字列フィールド
     */
    public static AbstractCobolField funcReverse(
            int offset, int length, AbstractCobolField srcfield) {
        makeFieldEntry(srcfield);
        int size = srcfield.getSize();
        CobolDataStorage currStorage = currField.getDataStorage();
        CobolDataStorage srcStorage = srcfield.getDataStorage();
        for (int i = 0; i < size; ++i) {
            currStorage.setByte(i, srcStorage.getByte(srcfield.getSize() - i - 1));
        }
        if (offset > 0) {
            calcRefMod(currField, offset, length);
        }
        return currField;
    }

    /**
     * FUNCTION WHEN-COMPILED - コンパイル日時を返す。
     *
     * <p>COBOL文「FUNCTION WHEN-COMPILED」に対応。プログラムがコンパイルされた日時を返す。
     * 形式は「YYYYMMDDHHMMSSFF+HHMM」（21桁）。
     *
     * @param offset 参照修飾の開始位置（0の場合は修飾なし）
     * @param length 参照修飾の長さ
     * @param f コンパイル日時を格納したフィールド（生成コード側で設定）
     * @return コンパイル日時の文字列フィールド
     */
    public static AbstractCobolField funcWhenCompiled(
            int offset, int length, AbstractCobolField f) {
        makeFieldEntry(f);
        currField.getDataStorage().memcpy(f.getDataStorage(), f.getSize());
        if (offset > 0) {
            calcRefMod(currField, offset, length);
        }
        return currField;
    }

    /**
     * FUNCTION CURRENT-DATE - 現在の日時を返す。
     *
     * <p>COBOL文「FUNCTION CURRENT-DATE」に対応。システムの現在日時を
     * 「YYYYMMDDHHMMSSFF+HHMM」形式（21桁）で返す。
     *
     * @param offset 参照修飾の開始位置（0の場合は修飾なし）
     * @param length 参照修飾の長さ
     * @return 現在日時の文字列フィールド
     */
    public static AbstractCobolField funcCurrentDate(int offset, int length) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(21, (CobolDataStorage) null, attr);
        makeFieldEntry(field);
        // TODO Time Zoneを表示する機能を取り入れる

        String dateString =
                String.format(
                        "%4d%02d%02d%02d%02d%02d%02d00000",
                        CobolUtil.cal.get(Calendar.YEAR),
                        CobolUtil.cal.get(Calendar.MONTH) + 1,
                        CobolUtil.cal.get(Calendar.DAY_OF_MONTH),
                        CobolUtil.cal.get(Calendar.HOUR_OF_DAY),
                        CobolUtil.cal.get(Calendar.MINUTE),
                        CobolUtil.cal.get(Calendar.SECOND),
                        CobolUtil.cal.get(Calendar.MILLISECOND) / 10);
        currField.getDataStorage().memcpy(dateString.getBytes(AbstractCobolField.charSetSJIS));

        if (offset > 0) {
            calcRefMod(currField, offset, length);
        }
        return currField;
    }

    /**
     * FUNCTION CHAR - 文字コードに対応する文字を返す。
     *
     * <p>COBOL文「FUNCTION CHAR(integer)」に対応。引数の整数値（1-256）に対応する
     * 文字を返す。1は0x00、256は0xFFに対応。範囲外の場合はNULL文字を返す。
     *
     * @param srcfield 文字コード（1-256）を格納した数値フィールド
     * @return 対応する1バイト文字を格納したフィールド
     */
    public static AbstractCobolField funcChar(AbstractCobolField srcfield) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(1, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        int i = srcfield.getInt();
        if (i < 1 || i > 256) {
            currField.getDataStorage().setByte(0, (byte) 0);
        } else {
            currField.getDataStorage().setByte(0, (byte) (i - 1));
        }
        return currField;
    }

    /**
     * FUNCTION ORD - 文字の序数位置を返す。libcob/intrinsic.cのcob_intr_ordに対応。
     *
     * <p>COBOL文「FUNCTION ORD(character)」に対応。文字の内部表現（文字コード）に
     * 1を加えた値を返す。FUNCTION CHARの逆関数。
     *
     * @param srcfield 1バイト文字を格納したフィールド
     * @return 序数位置（1-256）を格納した数値フィールド
     */
    public static AbstractCobolField funcOrd(AbstractCobolField srcfield) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 8, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        currField.setInt(srcfield.getDataStorage().getByte(0) + 1);
        return currField;
    }

    /**
     * FUNCTION DATE-OF-INTEGER - 整数日付を標準日付に変換する。libcob/intrinsic.cのcob_intr_date_of_integerに対応。
     *
     * <p>COBOL文「FUNCTION DATE-OF-INTEGER(integer)」に対応。1601年1月1日を起点とした
     * 通算日数から標準日付形式（YYYYMMDD）に変換する。
     * 有効範囲: 1〜3067671（1601年1月1日〜9999年12月31日）
     *
     * @param srcdays 1601年1月1日からの通算日数を格納した数値フィールド
     * @return 標準日付形式（YYYYMMDD）の8桁数値フィールド
     */
    public static AbstractCobolField funcDateOfInteger(AbstractCobolField srcdays) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 8, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        CobolRuntimeException.setException(0);
        int days = srcdays.getInt();

        if (days < 1 || days > 3067671) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.getDataStorage().memset((byte) '0', 8);
            return currField;
        }

        int leapyear = 365;
        int baseyear = 1601;
        while (days > leapyear) {
            days -= leapyear;
            ++baseyear;
            if (isLeapYear(baseyear)) {
                leapyear = 366;
            } else {
                leapyear = 365;
            }
        }
        int i;
        for (i = 0; i < 13; ++i) {
            if (isLeapYear(baseyear)) {
                if (days <= leapDays[i]) {
                    days -= leapDays[i - 1];
                    break;
                }
            } else {
                if (days <= normalDays[i]) {
                    days -= normalDays[i - 1];
                    break;
                }
            }
        }
        String dateString = String.format("%04d%02d%02d", baseyear, i, days);
        currField.getDataStorage().memcpy(dateString.getBytes(AbstractCobolField.charSetSJIS));
        return currField;
    }

    /**
     * FUNCTION DAY-OF-INTEGER - 整数日付を年通日に変換する。libcob/intrinsic.cのcob_intr_day_of_integerに対応。
     *
     * <p>COBOL文「FUNCTION DAY-OF-INTEGER(integer)」に対応。1601年1月1日を起点とした
     * 通算日数から年通日形式（YYYYDDD）に変換する。DDDは年初からの通算日数（1-366）。
     * 有効範囲: 1〜3067671（1601年1月1日〜9999年12月31日）
     *
     * @param srcdays 1601年1月1日からの通算日数を格納した数値フィールド
     * @return 年通日形式（YYYYDDD）の7桁数値フィールド
     */
    public static AbstractCobolField funcDayOfInteger(AbstractCobolField srcdays) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 7, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(7, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        CobolRuntimeException.setException(0);
        int days = srcdays.getInt();

        if (days < 1 || days > 3067671) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.getDataStorage().memset((byte) '0', 8);
            return currField;
        }

        int leapyear = 365;
        int baseyear = 1601;
        while (days > leapyear) {
            days -= leapyear;
            ++baseyear;
            if (isLeapYear((baseyear))) {
                leapyear = 366;
            } else {
                leapyear = 365;
            }
        }
        String dateString = String.format("%04d%03d", baseyear, days);
        currField.getDataStorage().memcpy(dateString.getBytes(AbstractCobolField.charSetSJIS));
        return currField;
    }

    /**
     * FUNCTION INTEGER-OF-DATE - 標準日付を整数日付に変換する。libcob/intrinsic.cのcob_intr_integer_of_dateに対応。
     *
     * <p>COBOL文「FUNCTION INTEGER-OF-DATE(date)」に対応。標準日付形式（YYYYMMDD）を
     * 1601年1月1日を起点とした通算日数に変換する。FUNCTION DATE-OF-INTEGERの逆関数。
     * 有効範囲: 16010101〜99991231
     *
     * @param srcfield 標準日付形式（YYYYMMDD）の8桁数値フィールド
     * @return 1601年1月1日からの通算日数を格納した数値フィールド
     */
    public static AbstractCobolField funcIntegerOfDate(AbstractCobolField srcfield) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 8, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        CobolRuntimeException.setException(0);
        int indate = srcfield.getInt();
        int year = indate / 10000;
        if (year < 1601 || year > 9999) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }
        indate %= 10000;
        int month = indate / 100;
        if (month < 1 || month > 12) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }
        int days = indate % 100;
        if (days < 1 || days > 31) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }
        if (isLeapYear(year)) {
            if (days > leapMonthDays[month]) {
                CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
                currField.setInt(0);
                return currField;
            }
        } else {
            if (days > normalMonthDays[month]) {
                CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
                currField.setInt(0);
                return currField;
            }
        }

        int totaldays = 0;
        int baseyear = 1601;
        while (baseyear != year) {
            if (isLeapYear(baseyear)) {
                totaldays += 366;
            } else {
                totaldays += 365;
            }
            ++baseyear;
        }

        if (isLeapYear(baseyear)) {
            totaldays += leapDays[month - 1];
        } else {
            totaldays += normalDays[month - 1];
        }

        totaldays += days;
        currField.setInt(totaldays);
        return currField;
    }

    /**
     * FUNCTION INTEGER-OF-DAY - 年通日を整数日付に変換する。libcob/intrinsic.cのcob_intr_integer_of_dayに対応。
     *
     * <p>COBOL文「FUNCTION INTEGER-OF-DAY(julian-date)」に対応。年通日形式（YYYYDDD）を
     * 1601年1月1日を起点とした通算日数に変換する。FUNCTION DAY-OF-INTEGERの逆関数。
     * 有効範囲: 1601001〜9999365（またはうるう年の366）
     *
     * @param srcfield 年通日形式（YYYYDDD）の7桁数値フィールド
     * @return 1601年1月1日からの通算日数を格納した数値フィールド
     */
    public static AbstractCobolField funcIntegerOfDay(AbstractCobolField srcfield) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 8, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        CobolRuntimeException.setException(0);
        int indate = srcfield.getInt();
        int year = indate / 1000;
        if (year < 1601 || year > 9999) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }
        int days = indate % 1000;
        if (days < 1 || days > 365 + (isLeapYear(year) ? 1 : 0)) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }
        int totaldays = 0;
        int baseyear = 1601;
        while (baseyear != year) {
            if (isLeapYear(baseyear)) {
                totaldays += 366;
            } else {
                totaldays += 365;
            }
            ++baseyear;
        }
        totaldays += days;
        currField.setInt(totaldays);
        return currField;
    }

    /**
     * FUNCTION FACTORIAL - 階乗を計算する。libcob/intrinsic.cのcob_intr_factorialに対応。
     *
     * <p>COBOL文「FUNCTION FACTORIAL(integer)」に対応。引数nに対してn!（n階乗）を返す。
     * 0! = 1、負の値の場合は例外を発生させる。
     *
     * @param srcfield 非負整数を格納した数値フィールド
     * @return 階乗の結果を格納した数値フィールド
     */
    public static AbstractCobolField funcFactorial(AbstractCobolField srcfield) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 18, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        CobolRuntimeException.setException(0);
        int srcval = srcfield.getInt();
        if (srcval < 0) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }
        BigDecimal d = BigDecimal.ONE;
        for (int i = 2; i <= srcval; ++i) {
            d = d.multiply(new BigDecimal(i));
        }
        try {
            new CobolDecimal(d, 0).getField(currField, 0);
        } catch (CobolStopRunException e) {
            return null;
        }
        return currField;
    }

    private static CobolDecimal mathFunctionBefore1(AbstractCobolField srcfield) {
        CobolDecimal d1 = new CobolDecimal();
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        17,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);
        d1.setField(srcfield);
        makeFieldEntry(field);
        return d1;
    }

    private static CobolDecimal mathFunctionBefore2(AbstractCobolField srcfield) {
        CobolDecimal d1 = new CobolDecimal();
        d1.setField(srcfield);
        makeDoubleEntry();
        return d1;
    }

    private static AbstractCobolField mathFunctionAfter1(double mathd2) {
        if (Double.isNaN(mathd2)
                || mathd2 == Double.POSITIVE_INFINITY
                || mathd2 == Double.NEGATIVE_INFINITY) {
            currField.setInt(0);
            return currField;
        }
        long result = (long) mathd2;
        mathd2 -= result;
        for (int i = 0; i < 17; ++i) {
            mathd2 *= 10;
            int tempres = (int) mathd2;
            result *= 10;
            result += tempres;
            mathd2 -= tempres;
        }
        currField.getDataStorage().set(result);
        return currField;
    }

    private static AbstractCobolField mathFunctionAfter2(double mathd2) {
        if (Double.isNaN(mathd2)
                || mathd2 == Double.POSITIVE_INFINITY
                || mathd2 == Double.NEGATIVE_INFINITY) {
            currField.setInt(0);
            return currField;
        }
        currField.getDataStorage().set(mathd2);
        return currField;
    }

    /**
     * FUNCTION EXP - 自然対数の底eのべき乗を返す。libcob/intrinsic.cのcob_intr_expに対応。
     *
     * <p>COBOL文「FUNCTION EXP(number)」に対応。e^x（ネイピア数eのx乗）を返す。
     *
     * @param srcfield 指数を格納した数値フィールド
     * @return e^xの計算結果を格納した数値フィールド
     */
    public static AbstractCobolField funcExp(AbstractCobolField srcfield) {
        CobolDecimal d1 = mathFunctionBefore2(srcfield);
        double mathd2 = Math.pow(2.7182818284590452354, intrGetDouble(d1));
        return mathFunctionAfter2(mathd2);
    }

    /**
     * FUNCTION EXP10 - 10のべき乗を返す。libcob/intrinsic.cのcob_intr_exp10に対応。
     *
     * <p>COBOL文「FUNCTION EXP10(number)」に対応。10^x（10のx乗）を返す。
     *
     * @param srcfield 指数を格納した数値フィールド
     * @return 10^xの計算結果を格納した数値フィールド
     */
    public static AbstractCobolField funcExp10(AbstractCobolField srcfield) {
        CobolDecimal d1 = mathFunctionBefore2(srcfield);
        double mathd2 = Math.pow(10, intrGetDouble(d1));
        return mathFunctionAfter2(mathd2);
    }

    /**
     * FUNCTION ABS - 絶対値を返す。libcob/intrinsic.cのcob_intr_absに対応。
     *
     * <p>COBOL文「FUNCTION ABS(number)」に対応。引数の絶対値を返す。
     *
     * @param srcfield 数値フィールド
     * @return 絶対値を格納した数値フィールド
     */
    public static AbstractCobolField funcAbs(AbstractCobolField srcfield) {
        makeFieldEntry(srcfield);
        CobolDecimal d1 = srcfield.getDecimal();
        d1.setValue(d1.getValue().abs());
        try {
            d1.getField(currField, 0);
        } catch (CobolStopRunException e) {
            return null;
        }
        return currField;
    }

    /**
     * FUNCTION ACOS - 逆余弦（アークコサイン）を返す。libcob/intrinsic.cのcob_intr_acosに対応。
     *
     * <p>COBOL文「FUNCTION ACOS(number)」に対応。結果はラジアン単位。
     * 有効範囲: -1 ≤ number ≤ 1
     *
     * @param srcfield -1から1の範囲の数値フィールド
     * @return 逆余弦（ラジアン）を格納した数値フィールド
     */
    public static AbstractCobolField funcAcos(AbstractCobolField srcfield) {
        CobolDecimal d1 = mathFunctionBefore1(srcfield);
        double mathd2 = Math.acos(intrGetDouble(d1));
        return mathFunctionAfter1(mathd2);
    }

    /**
     * FUNCTION ASIN - 逆正弦（アークサイン）を返す。libcob/intrinsic.cのcob_intr_asinに対応。
     *
     * <p>COBOL文「FUNCTION ASIN(number)」に対応。結果はラジアン単位。
     * 有効範囲: -1 ≤ number ≤ 1
     *
     * @param srcfield -1から1の範囲の数値フィールド
     * @return 逆正弦（ラジアン）を格納した数値フィールド
     */
    public static AbstractCobolField funcAsin(AbstractCobolField srcfield) {
        CobolDecimal d1 = mathFunctionBefore1(srcfield);
        double mathd2 = Math.asin(intrGetDouble(d1));
        return mathFunctionAfter1(mathd2);
    }

    /**
     * FUNCTION ATAN - 逆正接（アークタンジェント）を返す。libcob/intrinsic.cのcob_intr_atanに対応。
     *
     * <p>COBOL文「FUNCTION ATAN(number)」に対応。結果はラジアン単位。
     *
     * @param srcfield 数値フィールド
     * @return 逆正接（ラジアン）を格納した数値フィールド
     */
    public static AbstractCobolField funcAtan(AbstractCobolField srcfield) {
        CobolDecimal d1 = mathFunctionBefore1(srcfield);
        double mathd2 = Math.atan(intrGetDouble(d1));
        return mathFunctionAfter1(mathd2);
    }

    /**
     * FUNCTION COS - 余弦（コサイン）を返す。libcob/intrinsic.cのcob_intr_cosに対応。
     *
     * <p>COBOL文「FUNCTION COS(number)」に対応。引数はラジアン単位。
     *
     * @param srcfield 角度（ラジアン）を格納した数値フィールド
     * @return 余弦値を格納した数値フィールド
     */
    public static AbstractCobolField funcCos(AbstractCobolField srcfield) {
        CobolDecimal d1 = mathFunctionBefore1(srcfield);
        double mathd2 = Math.cos(intrGetDouble(d1));
        return mathFunctionAfter1(mathd2);
    }

    /**
     * FUNCTION LOG - 自然対数を返す。libcob/intrinsic.cのcob_intr_logに対応。
     *
     * <p>COBOL文「FUNCTION LOG(number)」に対応。底eの対数（ln）を返す。
     * 引数は正の数である必要がある。
     *
     * @param srcfield 正の数値フィールド
     * @return 自然対数を格納した数値フィールド
     */
    public static AbstractCobolField funcLog(AbstractCobolField srcfield) {
        CobolDecimal d1 = mathFunctionBefore2(srcfield);
        double mathd2 = Math.log(intrGetDouble(d1));
        return mathFunctionAfter2(mathd2);
    }

    /**
     * FUNCTION LOG10 - 常用対数（10を底とする対数）を返す。libcob/intrinsic.cのcob_intr_log10に対応。
     *
     * <p>COBOL文「FUNCTION LOG10(number)」に対応。底10の対数を返す。
     * 引数は正の数である必要がある。
     *
     * @param srcfield 正の数値フィールド
     * @return 常用対数を格納した数値フィールド
     */
    public static AbstractCobolField funcLog10(AbstractCobolField srcfield) {
        CobolDecimal d1 = mathFunctionBefore2(srcfield);
        double mathd2 = Math.log10(intrGetDouble(d1));
        return mathFunctionAfter2(mathd2);
    }

    /**
     * FUNCTION SIN - 正弦（サイン）を返す。libcob/intrinsic.cのcob_intr_sinに対応。
     *
     * <p>COBOL文「FUNCTION SIN(number)」に対応。引数はラジアン単位。
     *
     * @param srcfield 角度（ラジアン）を格納した数値フィールド
     * @return 正弦値を格納した数値フィールド
     */
    public static AbstractCobolField funcSin(AbstractCobolField srcfield) {
        CobolDecimal d1 = mathFunctionBefore1(srcfield);
        double mathd2 = Math.sin(intrGetDouble(d1));
        return mathFunctionAfter1(mathd2);
    }

    /**
     * FUNCTION SQRT - 平方根を返す。libcob/intrinsic.cのcob_intr_sqrtに対応。
     *
     * <p>COBOL文「FUNCTION SQRT(number)」に対応。引数は非負である必要がある。
     *
     * @param srcfield 非負の数値フィールド
     * @return 平方根を格納した数値フィールド
     */
    public static AbstractCobolField funcSqrt(AbstractCobolField srcfield) {
        CobolDecimal d1 = mathFunctionBefore2(srcfield);
        double mathd2 = Math.sqrt(intrGetDouble(d1));
        return mathFunctionAfter2(mathd2);
    }

    /**
     * FUNCTION TAN - 正接（タンジェント）を返す。libcob/intrinsic.cのcob_intr_tanに対応。
     *
     * <p>COBOL文「FUNCTION TAN(number)」に対応。引数はラジアン単位。
     *
     * @param srcfield 角度（ラジアン）を格納した数値フィールド
     * @return 正接値を格納した数値フィールド
     */
    public static AbstractCobolField funcTan(AbstractCobolField srcfield) {
        CobolDecimal d1 = mathFunctionBefore2(srcfield);
        double mathd2 = Math.tan(intrGetDouble(d1));
        return mathFunctionAfter2(mathd2);
    }

    /**
     * FUNCTION NUMVAL - 文字列を数値に変換する。libcob/intrinsic.cのcob_intr_numvalに対応。
     *
     * <p>COBOL文「FUNCTION NUMVAL(string)」に対応。編集済み数字項目や数値文字列を
     * 数値に変換する。空白、符号（+/-）、小数点を認識。末尾のCR/DBは負号として扱う。
     *
     * @param srcfield 数値文字列を格納したフィールド
     * @return 数値に変換された数値フィールド
     */
    public static AbstractCobolField funcNumval(AbstractCobolField srcfield) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);

        CobolDataStorage s = srcfield.getDataStorage();
        boolean sign = false;
        boolean decimalSeen = false;
        long llval = 0;
        int integerDigits = 0;
        int decimalDigits = 0;
        StringBuilder integerBuff = new StringBuilder();
        StringBuilder decimalBuff = new StringBuilder();
        for (int i = 0; i < srcfield.getSize(); ++i) {
            if (i < srcfield.getSize() - 1) {
                if ((Character.toUpperCase(s.getByte(i)) == 'C'
                                && Character.toUpperCase(s.getByte(i + 1)) == 'R')
                        || (Character.toUpperCase(s.getByte(i)) == 'D'
                                && Character.toUpperCase(s.getByte(i + 1)) == 'B')) {
                    sign = true;
                    break;
                }
            }
            char c = (char) s.getByte(i);
            if (c == ' ' || c == '+') {
                continue;
            }
            if (c == '-') {
                sign = true;
                continue;
            }
            if (c == CobolModule.getCurrentModule().decimal_point) {
                decimalSeen = true;
                continue;
            }
            if (c >= '0' && c <= '9') {
                llval *= 10;
                llval += c - '0';
                if (decimalSeen) {
                    decimalBuff.append(c);
                    decimalDigits++;
                } else {
                    integerBuff.append(c);
                    integerDigits++;
                }
            }
            if (integerDigits + decimalDigits > 30) {
                break;
            }
        }
        if (integerDigits > 0) {
            integerBuff.setCharAt(0, '0');
        }
        if (decimalDigits > 0) {
            decimalBuff.setCharAt(0, '0');
        }
        if (sign) {
            llval = -llval;
        }
        if (integerDigits + decimalDigits <= 18) {
            attr.setScale(decimalDigits);
            makeFieldEntry(field);
            currField.getDataStorage().set(llval);
        } else {
            String dataString =
                    String.format(
                            "%s%s.%s",
                            sign ? "-" : "", integerBuff.toString(), decimalBuff.toString());
            double val = Double.parseDouble(dataString);
            makeDoubleEntry();
            currField.getDataStorage().set(val);
        }
        return currField;
    }

    /**
     * FUNCTION NUMVAL-C - 通貨記号付き文字列を数値に変換する。libcob/intrinsic.cのcob_intr_numval_cに対応。
     *
     * <p>COBOL文「FUNCTION NUMVAL-C(string, currency)」に対応。NUMVAL関数に加えて、
     * 通貨記号と桁区切りカンマを無視して数値に変換する。
     *
     * @param srcfield 通貨記号付き数値文字列を格納したフィールド
     * @param currency 通貨記号を格納したフィールド（nullの場合はデフォルトの通貨記号を使用）
     * @return 数値に変換された数値フィールド
     */
    public static AbstractCobolField funcNumvalC(
            AbstractCobolField srcfield, AbstractCobolField currency) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);

        boolean sign = false;
        boolean decimalSeen = false;
        long llval = 0;
        int integerDigits = 0;
        int decimalDigits = 0;
        StringBuilder integerBuff = new StringBuilder();
        StringBuilder decimalBuff = new StringBuilder();

        CobolDataStorage currencyData = null;
        if (currency != null) {
            if (currency.getSize() < srcfield.getSize()) {
                currencyData = currency.getDataStorage();
            }
        }
        for (int i = 0; i < srcfield.getSize(); ++i) {
            char c = (char) srcfield.getDataStorage().getByte(i);
            if (i < srcfield.getSize() - 1) {
                char cc = (char) srcfield.getDataStorage().getByte(i + 1);
                if ((Character.toUpperCase(c) == 'C' && Character.toUpperCase(cc) == 'R')
                        || (Character.toUpperCase(c) == 'D' && Character.toUpperCase(cc) == 'B')) {
                    sign = true;
                    break;
                }
            }
            if (currencyData != null) {
                if (i < srcfield.getSize() - currency.getSize()) {
                    if (currencyData.memcmp(
                                    srcfield.getDataStorage().getSubDataStorage(i),
                                    currency.getSize())
                            == 0) {
                        i += (currency.getSize() - 1);
                        continue;
                    }
                }
            }
            if (c == ' ' || c == '+') {
                continue;
            }
            if (c == '-') {
                sign = true;
                continue;
            }
            if (c == CobolModule.getCurrentModule().decimal_point) {
                decimalSeen = true;
                continue;
            }
            if (c == CobolModule.getCurrentModule().currency_symbol) {
                continue;
            }
            if (c >= '0' && c <= '9') {
                llval *= 10;
                llval += c - '0';
                if (decimalSeen) {
                    decimalBuff.append(c);
                    decimalDigits++;
                } else {
                    integerBuff.append(c);
                    integerDigits++;
                }
            }
            if (integerDigits + decimalDigits > 30) {
                break;
            }
        }
        if (integerDigits > 0) {
            integerBuff.setCharAt(0, '0');
        }
        if (decimalDigits > 0) {
            decimalBuff.setCharAt(0, '0');
        }
        if (sign) {
            llval = -llval;
        }
        if (integerDigits + decimalDigits <= 18) {
            attr.setScale(decimalDigits);
            makeFieldEntry(field);
            currField.getDataStorage().set(llval);
        } else {
            String dataString =
                    String.format(
                            "%s%s.%s",
                            sign ? "-" : "", integerBuff.toString(), decimalBuff.toString());
            double val = Double.parseDouble(dataString);
            makeDoubleEntry();
            currField.getDataStorage().set(val);
        }
        return currField;
    }

    /**
     * このメソッドは未実装
     *
     * @param n このメソッドは未実装
     * @param currency このメソッドは未実装
     * @return null
     */
    public static AbstractCobolField funcNumvalC(int n, AbstractCobolField currency) {
        // TODO
        return null;
    }

    /**
     * FUNCTION NUMVAL-C - 通貨記号付き文字列を数値に変換する（通貨記号省略版）。
     *
     * <p>通貨記号を省略した場合のオーバーロード。デフォルトの通貨記号を使用する。
     *
     * @param srcfield 通貨記号付き数値文字列を格納したフィールド
     * @param n 未使用パラメータ
     * @return 数値に変換された数値フィールド
     */
    public static AbstractCobolField funcNumvalC(AbstractCobolField srcfield, int n) {
        return funcNumvalC(srcfield, null);
    }

    /**
     * このメソッドは未実装
     *
     * @param n このメソッドは未実装
     * @param m このメソッドは未実装
     * @return null
     */
    public static AbstractCobolField funcNumvalC(int n, int m) {
        // TODO
        return null;
    }

    /**
     * FUNCTION ANNUITY - 年金係数を計算する。libcob/intrinsic.cのcob_intr_annuityに対応。
     *
     * <p>COBOL文「FUNCTION ANNUITY(rate, periods)」に対応。指定された利率と期間に基づいて、
     * 元本1に対する年金（均等返済）の係数を返す。利率が0の場合は1/periodsを返す。
     *
     * @param srcfield1 利率を格納した数値フィールド
     * @param srcfield2 期間（回数）を格納した数値フィールド
     * @return 年金係数を格納した数値フィールド
     */
    public static AbstractCobolField funcAnnuity(
            AbstractCobolField srcfield1, AbstractCobolField srcfield2) {
        makeDoubleEntry();
        CobolDecimal d1 = new CobolDecimal();
        CobolDecimal d2 = new CobolDecimal();
        d1.setField(srcfield1);
        d2.setField(srcfield2);

        double mathd1 = intrGetDouble(d1);
        double mathd2 = intrGetDouble(d2);
        if (mathd1 == 0) {
            mathd1 = 1.0 / mathd2;
            currField.getDataStorage().set(mathd1);
            return currField;
        }

        mathd1 /= (1.0 - Math.pow(mathd1 + 1.0, 0.0 - mathd2));
        currField.getDataStorage().set(mathd1);
        return currField;
    }

    private static int sizeInBase10(BigDecimal d1) {
        String s = d1.toPlainString();
        int begin = s.charAt(0) == '-' ? 0 : -1;
        int pointIndex = s.indexOf('.');
        int end = pointIndex < 0 ? s.length() : pointIndex;
        return end - begin - 1;
    }

    /**
     * FUNCTION SUM - 引数の合計を返す。libcob/intrinsic.cのcob_intr_sumに対応。
     *
     * <p>COBOL文「FUNCTION SUM(arg1, arg2, ...)」に対応。全ての引数の合計値を返す。
     *
     * @param params 引数の数
     * @param fields 合計する数値フィールドの可変長配列
     * @return 合計値を格納した数値フィールド
     */
    public static AbstractCobolField funcSum(int params, AbstractCobolField... fields) {
        CobolDecimal d1 = new CobolDecimal();
        CobolDecimal d2 = new CobolDecimal();
        d1.setValue(BigDecimal.ZERO);

        int scale = 0;
        for (AbstractCobolField f : fields) {
            if (f.getAttribute().getScale() > scale) {
                scale = f.getAttribute().getScale();
            }
            d2.setField(f);
            d1.add(d2);
        }

        int size = sizeInBase10(d1.getValue());
        AbstractCobolField field;
        if (size < 19) {
            CobolFieldAttribute attr =
                    new CobolFieldAttribute(
                            CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                            18,
                            scale,
                            CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                            null);
            field = CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);
        } else {
            if (d1.getScale() > size) {
                size = d1.getScale();
            }
            if (scale > size) {
                size = scale;
            }
            CobolFieldAttribute attr =
                    new CobolFieldAttribute(
                            CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                            size,
                            scale,
                            CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                            null);
            field = CobolFieldFactory.makeCobolField(size, (CobolDataStorage) null, attr);
        }
        makeFieldEntry(field);
        try {
            d1.getField(currField, 0);
        } catch (CobolStopRunException e) {
            return null;
        }
        return currField;
    }

    /**
     * FUNCTION ORD-MIN - 最小値の引数位置を返す。libcob/intrinsic.cのcob_intr_ord_minに対応。
     *
     * <p>COBOL文「FUNCTION ORD-MIN(arg1, arg2, ...)」に対応。引数の中で最小値を持つ
     * 引数の序数位置（1から始まる）を返す。
     *
     * @param params 引数の数
     * @param fields 比較するフィールドの可変長配列
     * @return 最小値を持つ引数の序数位置（1から始まる）
     */
    public static AbstractCobolField funcOrdMin(int params, AbstractCobolField... fields) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 8, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        if (fields.length <= 1) {
            currField.setInt(0);
            return currField;
        }

        AbstractCobolField basef = fields[0];
        int ordmin = 0;
        for (int i = 1; i < fields.length; ++i) {
            AbstractCobolField f = fields[i];
            if (f.compareTo(basef) < 0) {
                basef = f;
                ordmin = i;
            }
        }

        currField.setLong((long) ordmin + 1);
        return currField;
    }

    /**
     * FUNCTION ORD-MAX - 最大値の引数位置を返す。libcob/intrinsic.cのcob_intr_ord_maxに対応。
     *
     * <p>COBOL文「FUNCTION ORD-MAX(arg1, arg2, ...)」に対応。引数の中で最大値を持つ
     * 引数の序数位置（1から始まる）を返す。
     *
     * @param params 引数の数
     * @param fields 比較するフィールドの可変長配列
     * @return 最大値を持つ引数の序数位置（1から始まる）
     */
    public static AbstractCobolField funcOrdMax(int params, AbstractCobolField... fields) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 8, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        if (fields.length <= 1) {
            currField.setInt(0);
            return currField;
        }

        AbstractCobolField basef = fields[0];
        int ordmax = 0;
        for (int i = 1; i < fields.length; ++i) {
            AbstractCobolField f = fields[i];
            if (f.compareTo(basef) > 0) {
                basef = f;
                ordmax = i;
            }
        }

        currField.setLong((long) ordmax + 1);
        return currField;
    }

    /**
     * FUNCTION MIN - 最小値を返す。libcob/intrinsic.cのcob_intr_minに対応。
     *
     * <p>COBOL文「FUNCTION MIN(arg1, arg2, ...)」に対応。引数の中で最小の値を返す。
     *
     * @param params 引数の数
     * @param fields 比較するフィールドの可変長配列
     * @return 最小値を持つフィールドへの参照
     */
    public static AbstractCobolField funcMin(int params, AbstractCobolField... fields) {
        AbstractCobolField beasef = fields[0];
        for (int i = 1; i < fields.length; ++i) {
            AbstractCobolField f = fields[i];
            if (f.compareTo(beasef) < 0) {
                beasef = f;
            }
        }

        return beasef;
    }

    /**
     * FUNCTION MAX - 最大値を返す。libcob/intrinsic.cのcob_intr_maxに対応。
     *
     * <p>COBOL文「FUNCTION MAX(arg1, arg2, ...)」に対応。引数の中で最大の値を返す。
     *
     * @param params 引数の数
     * @param fields 比較するフィールドの可変長配列
     * @return 最大値を持つフィールドへの参照
     */
    public static AbstractCobolField funcMax(int params, AbstractCobolField... fields) {
        AbstractCobolField beasef = fields[0];
        for (int i = 1; i < fields.length; ++i) {
            AbstractCobolField f = fields[i];
            if (f.compareTo(beasef) > 0) {
                beasef = f;
            }
        }

        return beasef;
    }

    /**
     * FUNCTION MIDRANGE - 中央範囲（最大値と最小値の平均）を返す。libcob/intrinsic.cのcob_intr_midrangeに対応。
     *
     * <p>COBOL文「FUNCTION MIDRANGE(arg1, arg2, ...)」に対応。引数の最大値と最小値の
     * 算術平均（(max + min) / 2）を返す。
     *
     * @param params 引数の数
     * @param fields 数値フィールドの可変長配列
     * @return 中央範囲を格納した数値フィールド
     */
    public static AbstractCobolField funcMidrange(int params, AbstractCobolField... fields) {
        makeDoubleEntry();
        AbstractCobolField basemin = fields[0];
        AbstractCobolField basemax = fields[0];
        for (int i = 1; i < params; ++i) {
            AbstractCobolField f = fields[i];
            if (f.compareTo(basemin) < 0) {
                basemin = f;
            }
            if (f.compareTo(basemax) > 0) {
                basemax = f;
            }
        }

        CobolDecimal d1 = new CobolDecimal();
        CobolDecimal d2 = new CobolDecimal();
        d1.setField(basemin);
        d2.setField(basemax);
        d1.add(d2);
        d2 = new CobolDecimal(new BigDecimal(2), 0);
        try {
            d1.div(d2);
            d1.getField(currField, 0);
        } catch (CobolStopRunException e) {
            return null;
        }
        return currField;
    }

    /**
     * FUNCTION MEDIAN - 中央値を返す。libcob/intrinsic.cのcob_intr_medianに対応。
     *
     * <p>COBOL文「FUNCTION MEDIAN(arg1, arg2, ...)」に対応。引数を昇順にソートした時の
     * 中央値を返す。引数の数が偶数の場合は中央2つの値の平均を返す。
     *
     * @param params 引数の数
     * @param fields 数値フィールドの可変長配列
     * @return 中央値を格納した数値フィールド
     */
    public static AbstractCobolField funcMedian(int params, AbstractCobolField... fields) {
        if (fields.length == 1) {
            return fields[0];
        }

        AbstractCobolField[] fieldAlloc = new AbstractCobolField[fields.length];

        for (int i = 0; i < params; ++i) {
            fieldAlloc[i] = fields[i];
        }

        Arrays.sort(fieldAlloc, (a, b) -> a.compareTo(b));
        int i = params / 2;
        if (params % 2 != 0) {
            return fieldAlloc[i];
        } else {
            makeDoubleEntry();
            CobolDecimal d1 = new CobolDecimal();
            CobolDecimal d2 = new CobolDecimal();
            d1.setField(fieldAlloc[i]);
            d2.setField(fieldAlloc[i - 1]);
            d1.add(d2);
            d2 = new CobolDecimal(new BigDecimal(2), 0);
            try {
                d1.div(d2);
                d1.getField(currField, 0);
            } catch (CobolStopRunException e) {
                return null;
            }
            return currField;
        }
    }

    /**
     * FUNCTION MEAN - 算術平均を返す。libcob/intrinsic.cのcob_intr_meanに対応。
     *
     * <p>COBOL文「FUNCTION MEAN(arg1, arg2, ...)」に対応。全ての引数の算術平均を返す。
     *
     * @param pramas 引数の数
     * @param fields 数値フィールドの可変長配列
     * @return 算術平均を格納した数値フィールド
     */
    public static AbstractCobolField funcMean(int pramas, AbstractCobolField... fields) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);
        CobolDecimal d1 = new CobolDecimal(BigDecimal.ZERO);
        CobolDecimal d2 = new CobolDecimal();

        for (AbstractCobolField f : fields) {
            d2.setField(f);
            d1.add(d2);
        }

        d2 = new CobolDecimal(new BigDecimal(fields.length), 0);
        try {
            d1.div(d2);
        } catch (CobolStopRunException e) {
            return null;
        }

        CobolDataStorage storage = new CobolDataStorage(8);
        field.setDataStorage(storage);
        try {
            d1.getField(field, 0);
        } catch (CobolStopRunException e) {
            return null;
        }
        long n = storage.longValue();
        int i = 0;
        while (n != 0) {
            n /= 10;
            ++i;
        }
        field.setDataStorage(null);
        if (i <= 18) {
            attr.setScale(18 - i);
        }
        makeFieldEntry(field);
        try {
            d1.getField(currField, 0);
        } catch (CobolStopRunException e) {
            return null;
        }
        return currField;
    }

    /**
     * FUNCTION MOD - 整数除算の剰余を返す。libcob/intrinsic.cのcob_intr_modに対応。
     *
     * <p>COBOL文「FUNCTION MOD(arg1, arg2)」に対応。arg1をarg2で割った商を
     * 負の無限大方向に丸めた後の剰余を返す。arg1 - (arg2 * INTEGER(arg1 / arg2))に相当。
     *
     * @param srcfield1 被除数
     * @param srcfield2 除数
     * @return 剰余を格納した数値フィールド
     * @throws CobolStopRunException 計算中にエラーが発生した場合
     */
    public static AbstractCobolField funcMod(
            AbstractCobolField srcfield1, AbstractCobolField srcfield2)
            throws CobolStopRunException {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);
        CobolDecimal d1 = new CobolDecimal();
        CobolDecimal d2 = new CobolDecimal();
        makeFieldEntry(field);

        AbstractCobolField f1 = funcInteger(intrBinop(srcfield1, '/', srcfield2));
        d1.setField(srcfield2);
        d2.setField(f1);
        d2.mul(d1);
        d1.setField(srcfield1);
        d1.sub(d2);
        try {
            d1.getField(currField, 0);
        } catch (CobolStopRunException e) {
            return null;
        }
        return currField;
    }

    /**
     * FUNCTION RANGE - 最大値と最小値の差を返す。libcob/intrinsic.cのcob_intr_rangeに対応。
     *
     * <p>COBOL文「FUNCTION RANGE(arg1, arg2, ...)」に対応。引数の最大値から最小値を
     * 引いた値（範囲）を返す。
     *
     * @param params 引数の数
     * @param fields 数値フィールドの可変長配列
     * @return 範囲（最大値 - 最小値）を格納した数値フィールド
     * @throws CobolStopRunException 計算中にエラーが発生した場合
     */
    public static AbstractCobolField funcRange(int params, AbstractCobolField... fields)
            throws CobolStopRunException {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);
        CobolDecimal d1 = new CobolDecimal();
        CobolDecimal d2 = new CobolDecimal();

        AbstractCobolField basemin = fields[0];
        AbstractCobolField basemax = fields[0];
        for (int i = 1; i < fields.length; ++i) {
            AbstractCobolField f = fields[i];
            if (f.compareTo(basemin) < 0) {
                basemin = f;
            }
            if (f.compareTo(basemax) > 0) {
                basemax = f;
            }
        }

        attr.setScale(basemin.getAttribute().getScale());
        if (basemax.getAttribute().getScale() > attr.getScale()) {
            attr.setScale(basemax.getAttribute().getScale());
        }
        makeFieldEntry(field);
        d1.setField(basemax);
        d2.setField(basemin);
        d1.sub(d2);
        d1.getField(currField, 0);
        return currField;
    }

    /**
     * FUNCTION REM - 除算の剰余を返す。libcob/intrinsic.cのcob_intr_remに対応。
     *
     * <p>COBOL文「FUNCTION REM(arg1, arg2)」に対応。arg1をarg2で割った商の整数部分を
     * 切り捨てた後の剰余を返す。arg1 - (arg2 * INTEGER-PART(arg1 / arg2))に相当。
     * MOD関数とは丸めの方向が異なる。
     *
     * @param srcfield1 被除数
     * @param srcfield2 除数
     * @return 剰余を格納した数値フィールド
     * @throws CobolStopRunException 計算中にエラーが発生した場合
     */
    public static AbstractCobolField funcRem(
            AbstractCobolField srcfield1, AbstractCobolField srcfield2)
            throws CobolStopRunException {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);
        AbstractCobolField f1 = funcIntegerPart(intrBinop(srcfield1, '/', srcfield2));
        CobolDecimal d1 = new CobolDecimal();
        CobolDecimal d2 = new CobolDecimal();

        d1.setField(srcfield2);
        d2.setField(f1);
        d2.mul(d1);
        d1.setField(srcfield1);
        d1.sub(d2);

        attr.setScale(srcfield1.getAttribute().getScale());
        if (srcfield2.getAttribute().getScale() > attr.getScale()) {
            attr.setScale(srcfield2.getAttribute().getScale());
        }
        makeFieldEntry(field);
        d1.getField(currField, 0);
        return currField;
    }

    /**
     * FUNCTION RANDOM - 擬似乱数を返す。libcob/intrinsic.cのcob_intr_randomに対応。
     *
     * <p>COBOL文「FUNCTION RANDOM」または「FUNCTION RANDOM(seed)」に対応。
     * 0以上1未満の擬似乱数を返す。引数を指定した場合は乱数のシードを設定する。
     *
     * @param prams 引数の数（0または1）
     * @param fields シード値を格納したフィールドの可変長配列（オプション）
     * @return 0以上1未満の乱数を格納した数値フィールド
     */
    public static AbstractCobolField funcRandom(int prams, AbstractCobolField... fields) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);

        if (fields.length > 0) {
            AbstractCobolField f = fields[0];
            int seed = f.getInt();
            if (seed < 0) {
                seed = 0;
            }
            random.setSeed(seed);
        }

        int r = random.nextInt(1000000001);

        int exp10 = 1;
        int i = 0;
        for (i = 0; i < 10; ++i) {
            if (r / exp10 == 0) {
                break;
            }
            exp10 *= 10;
        }
        if (i == 0) {
            i = 1;
        }
        attr.setScale(i);
        makeFieldEntry(field);
        currField.getDataStorage().set((long) r);
        return currField;
    }

    /**
     * FUNCTION VARIANCE - 分散を返す。libcob/intrinsic.cのcob_intr_varianceに対応。
     *
     * <p>COBOL文「FUNCTION VARIANCE(arg1, arg2, ...)」に対応。引数の分散（各値と平均の
     * 差の2乗の平均）を返す。引数が1つの場合は0を返す。
     *
     * @param prams 引数の数
     * @param fields 数値フィールドの可変長配列
     * @return 分散を格納した数値フィールド
     * @throws CobolStopRunException 計算中にエラーが発生した場合
     */
    public static AbstractCobolField funcVariance(int prams, AbstractCobolField... fields)
            throws CobolStopRunException {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);

        if (fields.length == 1) {
            makeFieldEntry(field);
            currField.setInt(0);
            return currField;
        }

        CobolDecimal d1 = new CobolDecimal(BigDecimal.ZERO, 0);
        CobolDecimal d2 = new CobolDecimal();

        for (AbstractCobolField f : fields) {
            d2.setField(f);
            d1.add(d2);
        }

        d2.setValue(new BigDecimal(fields.length));
        d2.setScale(0);
        try {
            d1.div(d2);
        } catch (CobolStopRunException e) {
            return null;
        }

        CobolDecimal d4 = new CobolDecimal(BigDecimal.ZERO, 0);

        for (AbstractCobolField f : fields) {
            d2.setField(f);
            d2.sub(d1);
            d2.mul(d2);
            d4.add(d2);
        }

        CobolDecimal d3 = new CobolDecimal(new BigDecimal(fields.length), 0);
        try {
            d4.div(d3);
        } catch (CobolStopRunException e) {
            return null;
        }
        CobolDataStorage data = new CobolDataStorage(8);
        field.setDataStorage(data);
        d4.getField(field, 0);
        long n = field.getLong();
        int i = 0;
        while (n != 0) {
            n /= 10;
            ++i;
        }
        makeFieldEntry(field);
        if (i <= 18) {
            attr.setScale(18 - i);
        }
        d4.getField(currField, 0);
        return currField;
    }

    /**
     * FUNCTION STANDARD-DEVIATION - 標準偏差を返す。libcob/intrinsic.cのcob_intr_standard_deviationに対応。
     *
     * <p>COBOL文「FUNCTION STANDARD-DEVIATION(arg1, arg2, ...)」に対応。引数の標準偏差
     * （分散の平方根）を返す。引数が1つの場合は0を返す。
     *
     * @param prams 引数の数
     * @param fields 数値フィールドの可変長配列
     * @return 標準偏差を格納した数値フィールド
     * @throws CobolStopRunException 計算中にエラーが発生した場合
     */
    public static AbstractCobolField funcStandardDeviation(int prams, AbstractCobolField... fields)
            throws CobolStopRunException {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);

        makeDoubleEntry();

        if (fields.length == 1) {
            makeFieldEntry(field);
            currField.setInt(0);
            return currField;
        }

        CobolDecimal d1 = new CobolDecimal(BigDecimal.ZERO, 0);
        CobolDecimal d2 = new CobolDecimal();

        for (AbstractCobolField f : fields) {
            d2.setField(f);
            d1.add(d2);
        }

        d2.setValue(new BigDecimal(fields.length));
        d2.setScale(0);
        try {
            d1.div(d2);
        } catch (CobolStopRunException e) {
            return null;
        }

        CobolDecimal d4 = new CobolDecimal(BigDecimal.ZERO, 0);

        for (AbstractCobolField f : fields) {
            d2.setField(f);
            d2.sub(d1);
            d2.mul(d2);
            d4.add(d2);
        }

        CobolDecimal d3 = new CobolDecimal(new BigDecimal(fields.length), 0);
        try {
            d4.div(d3);
        } catch (CobolStopRunException e) {
            return null;
        }
        d4.getField(currField, 0);
        return funcSqrt(currField);
    }

    /**
     * FUNCTION PRESENT-VALUE - 現在価値を返す。libcob/intrinsic.cのcob_intr_present_valueに対応。
     *
     * <p>COBOL文「FUNCTION PRESENT-VALUE(rate, amount1, amount2, ...)」に対応。
     * 指定された割引率で将来の金額の現在価値を計算する。各金額を(1 + rate)^nで割り引いた
     * 合計を返す。
     *
     * @param prams 引数の数（2以上必要）
     * @param fields 利率と金額のフィールドの可変長配列（最初が利率、以降が金額）
     * @return 現在価値を格納した数値フィールド
     * @throws CobolStopRunException 計算中にエラーが発生した場合
     */
    public static AbstractCobolField funcPresentValue(int prams, AbstractCobolField... fields)
            throws CobolStopRunException {
        makeDoubleEntry();
        if (fields.length < 2) {
            System.err.println("Wrong number of parameters for FUNCTION PRESENT-VALUE");
            System.err.flush();
            currField.setInt(0);
            return currField;
        }
        AbstractCobolField f = fields[0];
        CobolDecimal d1 = new CobolDecimal();
        d1.setField(f);
        CobolDecimal d2 = new CobolDecimal(BigDecimal.ONE, 0);
        d1.add(d2);
        CobolDecimal d4 = new CobolDecimal(BigDecimal.ZERO, 0);

        for (int i = 1; i < fields.length; ++i) {
            f = fields[i];
            d2.setField(f);
            CobolDecimal d3 = new CobolDecimal(d1.getValue().add(BigDecimal.ZERO), d1.getScale());
            if (i > 1) {
                CobolDecimal d5 = new CobolDecimal(new BigDecimal(i), 0);
                d3.pow(d5);
            }
            d2.div(d3);
            d4.add(d2);
        }

        d4.getField(currField, 0);
        return currField;
    }

    /**
     * FUNCTION NATIONAL - 文字列を日本語（全角）に変換する。libcob/intrinsic.cのcob_intr_nationalに対応。
     *
     * <p>COBOL文「FUNCTION NATIONAL(string)」に対応。半角英数字を全角に変換する。
     * 日本語項目（PIC N）への変換に使用される。
     *
     * @param srcfield 変換対象の文字列フィールド
     * @return 日本語（全角）に変換された文字列フィールド
     */
    public static AbstractCobolField funcNational(AbstractCobolField srcfield) {
        int size = srcfield.getSize();
        byte[] pdata =
                CobolNationalField.han2zen(
                        srcfield.getDataStorage().getByteBuffer(size).array(), size);
        int ndata = CobolNationalField.workReturnSize;
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NATIONAL, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(ndata, (CobolDataStorage) null, attr);
        makeFieldEntry(field);
        currField.getDataStorage().memcpy(pdata, ndata);
        return currField;
    }

    /**
     * FUNCTION COMBINED-DATETIME - 日付と時刻を組み合わせた値を返す。libcob/intrinsic.cのcob_intr_combined_datetimeに対応。
     *
     * <p>COBOL文「FUNCTION COMBINED-DATETIME(date, time)」に対応。整数日付（1601年1月1日からの通算日数）と
     * 午前0時からの秒数を組み合わせて、単一の数値として返す。
     *
     * @param srcdays 整数日付（1-3067671）
     * @param srctime 午前0時からの秒数（1-86400）
     * @return 日時を組み合わせた12桁の数値フィールド（小数点位置5）
     */
    public static AbstractCobolField funcCombinedDatetime(
            AbstractCobolField srcdays, AbstractCobolField srctime) {
        int srdays;
        int srtime;
        String str;

        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 12, 5, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(12, (CobolDataStorage) null, attr);
        makeFieldEntry(field);
        CobolRuntimeException.setException(0);
        srdays = srcdays.getInt();
        if (srdays < 1 || srdays > 3067671) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.getDataStorage().memset(0, 12);
            return currField;
        }
        srtime = srctime.getInt();
        if (srtime < 1 || srtime > 86400) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.getDataStorage().memset(0, 12);
            return currField;
        }
        str = String.format("%7d%5d", srdays, srtime);
        byte[] buff = str.getBytes(AbstractCobolField.charSetSJIS);
        for (int i = 0; i < buff.length; i++) {
            if (buff[i] == ' ') {
                buff[i] = '0';
            }
        }
        currField.getDataStorage().memcpy(buff);
        return currField;
    }

    /**
     * FUNCTION CONCATENATE - 文字列を連結する。libcob/intrinsic.cのcob_intr_concatenateに対応。
     *
     * <p>COBOL文「FUNCTION CONCATENATE(str1, str2, ...)」に対応。全ての引数文字列を
     * 左から右に連結した文字列を返す。
     *
     * @param offset 参照修飾の開始位置（0の場合は修飾なし）
     * @param length 参照修飾の長さ
     * @param params 引数の数
     * @param fields 連結する文字列フィールドの可変長配列
     * @return 連結された文字列フィールド
     */
    public static AbstractCobolField funcConcatenate(
            int offset, int length, int params, AbstractCobolField... fields) {
        int calcsize = 0;
        int i;
        int index = 0;
        int size;
        byte[] data;

        for (i = 0; i < params; i++) {
            calcsize += fields[i].getSize();
        }
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(calcsize, (CobolDataStorage) null, attr);
        makeFieldEntry(field);
        data = new byte[calcsize];
        for (i = 0; i < params; i++) {
            size = fields[i].getSize();
            System.arraycopy(
                    fields[i].getDataStorage().getByteBuffer(size).array(), 0, data, index, size);
            index += size;
        }
        currField.setDataStorage(new CobolDataStorage(data));
        if (offset > 0) {
            calcRefMod(currField, offset, length);
        }
        return currField;
    }

    /**
     * FUNCTION DATE-TO-YYYYMMDD - 2桁年日付を4桁年日付に変換する。libcob/intrinsic.cのcob_intr_date_to_yyyymmddに対応。
     *
     * <p>COBOL文「FUNCTION DATE-TO-YYYYMMDD(date, interval, base-year)」に対応。
     * YYMMDD形式の日付をYYYYMMDD形式に変換する。スライディングウィンドウ方式で世紀を決定。
     *
     * @param params 引数の数（1-3）
     * @param fields 日付（YYMMDD）、間隔（デフォルト50）、基準年（デフォルト現在年）
     * @return 4桁年日付（YYYYMMDD）を格納した数値フィールド
     */
    public static AbstractCobolField funcDateToYyyymmdd(int params, AbstractCobolField... fields) {
        int year;
        int mmdd;
        int interval;
        int xqtyear;
        int maxyear;
        LocalDateTime timeptr;

        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 8, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, (CobolDataStorage) null, attr);
        makeFieldEntry(field);
        year = fields[0].getInt();
        mmdd = year % 10000;
        year /= 10000;
        if (params > 1) {
            interval = fields[1].getInt();
        } else {
            interval = 50;
        }
        if (params > 2) {
            xqtyear = fields[2].getInt();
        } else {
            timeptr = CobolUtil.localtime();
            xqtyear = 1900 + timeptr.getDayOfYear();
        }
        if (year < 0 || year > 999999) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }
        if (xqtyear < 1601 || xqtyear > 9999) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }
        maxyear = xqtyear + interval;
        if (maxyear < 1700 || maxyear > 9999) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }
        if (maxyear % 100 >= year) {
            year += 100 * (maxyear / 100);
        } else {
            year += 100 * ((maxyear / 100) - 1);
        }
        year *= 10000;
        year += mmdd;
        currField.setInt(year);
        return currField;
    }

    /**
     * FUNCTION DAY-TO-YYYYDDD - 2桁年通日を4桁年通日に変換する。libcob/intrinsic.cのcob_intr_day_to_yyyydddに対応。
     *
     * <p>COBOL文「FUNCTION DAY-TO-YYYYDDD(julian-date, interval, base-year)」に対応。
     * YYDDD形式の年通日をYYYYDDD形式に変換する。スライディングウィンドウ方式で世紀を決定。
     *
     * @param params 引数の数（1-3）
     * @param fields 年通日（YYDDD）、間隔（デフォルト50）、基準年（デフォルト現在年）
     * @return 4桁年通日（YYYYDDD）を格納した数値フィールド
     */
    public static AbstractCobolField funcDayToYyyyddd(int params, AbstractCobolField... fields) {
        int year;
        int days;
        int interval;
        int xqtyear;
        int maxyear;
        LocalDateTime timeptr;

        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 8, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, (CobolDataStorage) null, attr);
        makeFieldEntry(field);
        year = fields[0].getInt();
        days = year % 1000;
        year /= 1000;
        if (params > 1) {
            interval = fields[1].getInt();
        } else {
            interval = 50;
        }
        if (params > 2) {
            xqtyear = fields[2].getInt();
        } else {
            timeptr = CobolUtil.localtime();
            xqtyear = 1900 + timeptr.getDayOfYear();
        }

        if (year < 0 || year > 999999) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }
        if (xqtyear < 1601 || xqtyear > 9999) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }
        maxyear = xqtyear + interval;
        if (maxyear < 1700 || maxyear > 9999) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }
        if (maxyear % 100 >= year) {
            year += 100 * (maxyear / 100);
        } else {
            year += 100 * ((maxyear / 100) - 1);
        }
        year *= 1000;
        year += days;
        currField.setInt(year);
        return currField;
    }

    // cob_intr_exception_fileの実装
    /**
     * COBOLのFUNCTION EXCEPTION-FILEを実装する。
     *
     * <p>最後のファイル操作例外が発生したファイルのステータスと名前を返す。
     * 例外がない場合や、ファイル例外でない場合は"00"を返す。
     * ファイル例外の場合は、ステータス2桁 + ファイル名（SELECT句で指定した名前）を連結して返す。
     *
     * @return ファイルステータスとファイル名を連結した英数字フィールド
     */
    public static AbstractCobolField funcExceptionFile() {
        int flen;
        byte[] data;

        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(0, (CobolDataStorage) null, attr);
        if (CobolRuntimeException.getException() == 0
                || (CobolRuntimeException.getExceptionCode() & 0x0500) != 0x0500) {
            field.setSize(2);
            makeFieldEntry(field);
            currField.memcpy(byteArray00, 2);
        } else {
            flen = CobolFile.errorFile.getSelectName().length();
            field.setSize(flen + 2);
            makeFieldEntry(field);
            data = new byte[2 + flen];
            System.arraycopy(CobolFile.errorFile.getFileStatus(), 0, data, 0, 2);
            System.arraycopy(
                    CobolFile.errorFile.getSelectName().getBytes(AbstractCobolField.charSetSJIS),
                    0,
                    data,
                    2,
                    flen);
            currField.setDataStorage(new CobolDataStorage(data));
        }
        return currField;
    }

    // cob_intr_exception_locationの実装
    /**
     * COBOLのFUNCTION EXCEPTION-LOCATIONを実装する。
     *
     * <p>最後の例外が発生した位置情報を返す。
     * 形式: "プログラムID; パラグラフ名 OF セクション名; 行番号"
     * パラグラフ名やセクション名がない場合は省略される。
     * 例外がない場合は空白1文字を返す。
     *
     * @return 例外発生位置を示す英数字フィールド
     */
    public static AbstractCobolField funcExceptionLocation() {
        String buff;

        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(0, (CobolDataStorage) null, attr);
        currField = field;
        if (CobolRuntimeException.getException() != 1
                || CobolRuntimeException.getOrigProgramId() == null) {
            field.setSize(1);
            makeFieldEntry(field);
            currField.getDataStorage().setByte(0, ' ');
            return currField;
        }
        if (CobolRuntimeException.getOrigSection() != null
                && CobolRuntimeException.getOrigParagragh() != null) {
            buff =
                    String.format(
                            "%s; %s OF %s; %d",
                            CobolRuntimeException.getOrigProgramId(),
                            CobolRuntimeException.getOrigParagragh(),
                            CobolRuntimeException.getOrigSection(),
                            CobolRuntimeException.getOrigLine());
        } else if (CobolRuntimeException.getOrigSection() != null) {
            buff =
                    String.format(
                            "%s; %s; %d",
                            CobolRuntimeException.getOrigProgramId(),
                            CobolRuntimeException.getOrigSection(),
                            CobolRuntimeException.getOrigLine());
        } else if (CobolRuntimeException.getOrigParagragh() != null) {
            buff =
                    String.format(
                            "%s; %s; %d",
                            CobolRuntimeException.getOrigProgramId(),
                            CobolRuntimeException.getOrigParagragh(),
                            CobolRuntimeException.getOrigLine());
        } else {
            buff =
                    String.format(
                            "%s; ; %d",
                            CobolRuntimeException.getOrigProgramId(),
                            CobolRuntimeException.getOrigLine());
        }
        localeBuff = buff.getBytes(AbstractCobolField.charSetSJIS);
        field.setSize(localeBuff.length);
        currField.setDataStorage(new CobolDataStorage(localeBuff));
        return currField;
    }

    // cob_intr_exception_statementの実装
    /**
     * COBOLのFUNCTION EXCEPTION-STATEMENTを実装する。
     *
     * <p>最後の例外が発生した文の種類を返す。例外がない場合は空白31文字を返す。
     *
     * @return 例外発生文の種類を示す31桁の英数字フィールド
     */
    public static AbstractCobolField funcExceptionStatement() {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(31, (CobolDataStorage) null, attr);
        makeFieldEntry(field);
        byte[] data;
        if (CobolRuntimeException.getExceptionCode() != 0
                && CobolRuntimeException.getOrigStatement() != null) {
            data =
                    String.format("%-31s", CobolRuntimeException.getOrigStatement())
                            .getBytes(AbstractCobolField.charSetSJIS);
        } else {
            data = String.format("%-31s", "").getBytes(AbstractCobolField.charSetSJIS);
        }
        currField.setDataStorage(new CobolDataStorage(data));
        return currField;
    }

    private static final byte[] CONST_STRING_EXCEPTION_OBJECT =
            "EXCEPTION-OBJECT".getBytes(AbstractCobolField.charSetSJIS);

    // cob_intr_exception_statusの実装
    /**
     * COBOLのFUNCTION EXCEPTION-STATUSを実装する。
     *
     * <p>最後の例外の名前（例: "EC-I-O-FILE-NOT-FOUND"）を返す。
     * 例外がない場合は空白31文字を返す。
     *
     * @return 例外名を示す31桁の英数字フィールド
     */
    public static AbstractCobolField funcExceptionStatus() {
        byte[] exceptName;

        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(31, (CobolDataStorage) null, attr);
        makeFieldEntry(field);
        byte[] data = String.format("%-31s", "").getBytes(AbstractCobolField.charSetSJIS);
        currField.setDataStorage(new CobolDataStorage(data));
        if (CobolRuntimeException.getExceptionCode() != 0) {
            try {
                exceptName =
                        CobolRuntimeException.getExceptionName(
                                        CobolRuntimeException.getExceptionCode())
                                .getBytes(AbstractCobolField.charSetSJIS);
            } catch (Exception e) {
                exceptName = CONST_STRING_EXCEPTION_OBJECT;
            }
            currField.memcpy(exceptName, exceptName.length);
        }
        return currField;
    }

    // cob_intr_fraction_partの実装
    /**
     * COBOLのFUNCTION FRACTION-PARTを実装する。
     *
     * <p>引数の小数部を返す。例えば、3.14に対して0.14を返す。
     * 結果は18桁の精度を持つ数値フィールドとして返される。
     *
     * @param srcfield 小数部を取得する数値フィールド
     * @return 小数部を表す数値フィールド
     */
    public static AbstractCobolField funcFractionPart(AbstractCobolField srcfield) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        18,
                        18,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(8, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        currField.moveFrom(srcfield);
        return currField;
    }

    // cob_intr_seconds_from_formatted_timeの実装
    /**
     * COBOLのFUNCTION SECONDS-FROM-FORMATTED-TIMEを実装する。
     *
     * <p>フォーマット指定に従って時刻文字列を解析し、午前0時からの秒数を返す。
     * フォーマット文字列中の"hh"は時、"mm"は分、"ss"は秒を表す。
     * 例: フォーマット"hh:mm:ss"、値"14:30:45"に対して52245を返す。
     *
     * @param format 時刻のフォーマットを指定するフィールド（例: "hh:mm:ss"）
     * @param value フォーマットに従った時刻文字列
     * @return 午前0時からの秒数を表す数値フィールド
     */
    public static AbstractCobolField funcSecondsFromFormattedTime(
            AbstractCobolField format, AbstractCobolField value) {
        int n;
        int seconds = 0;
        int minutes = 0;
        int hours = 0;
        boolean secondsSeen = false;
        boolean minutesSeen = false;
        boolean hoursSeen = false;
        String p1;
        int p2;

        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 8, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        if (value.getSize() < format.getSize()) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            currField.setInt(0);
            return currField;
        }

        CobolDataStorage formatData = format.getDataStorage();
        CobolDataStorage valueData = value.getDataStorage();

        for (n = 0; n < format.getSize() - 1; n++) {
            p1 = new String(formatData.getByteArray(n, 2), AbstractCobolField.charSetSJIS);

            if ("hh".equals(p1) && !hoursSeen) {
                p2 =
                        Integer.parseInt(
                                new String(
                                        valueData.getByteArray(n, 2),
                                        AbstractCobolField.charSetSJIS));
                hours = p2;
                hoursSeen = true;
                continue;
            }
            if ("mm".equals(p1) && !minutesSeen) {
                p2 =
                        Integer.parseInt(
                                new String(
                                        valueData.getByteArray(n, 2),
                                        AbstractCobolField.charSetSJIS));
                minutes = p2;
                minutesSeen = true;
                continue;
            }
            if ("ss".equals(p1) && !secondsSeen) {
                p2 =
                        Integer.parseInt(
                                new String(
                                        valueData.getByteArray(n, 2),
                                        AbstractCobolField.charSetSJIS));
                seconds = p2;
                secondsSeen = true;
                continue;
            }
        }

        if (hoursSeen && minutesSeen && secondsSeen) {
            seconds += hours * 3600 + minutes * 60;
        } else {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
            seconds = 0;
        }
        currField.setInt(seconds);
        return currField;
    }

    // cob_intr_seconds_past_midnightの実装
    /**
     * COBOLのFUNCTION SECONDS-PAST-MIDNIGHTを実装する。
     *
     * <p>現在時刻における午前0時からの経過秒数を返す。
     * 計算式: (時 × 3600) + (分 × 60) + 秒
     *
     * @return 午前0時からの経過秒数を表す8桁の数値フィールド
     */
    public static AbstractCobolField funcSecondsPastMidnight() {
        int seconds;
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 8, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, (CobolDataStorage) null, attr);
        makeFieldEntry(field);
        LocalDateTime currDate = LocalDateTime.now();
        seconds = currDate.getHour() * 3600 + currDate.getMinute() * 60 + currDate.getSecond();
        currField.setInt(seconds);
        return currField;
    }

    // cob_intr_signの実装
    /**
     * COBOLのFUNCTION SIGNを実装する。
     *
     * <p>引数の符号を返す。負の場合は-1、ゼロの場合は0、正の場合は1を返す。
     *
     * @param srcfield 符号を判定する数値フィールド
     * @return -1、0、または1を表す数値フィールド
     */
    public static AbstractCobolField funcSign(AbstractCobolField srcfield) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY,
                        8,
                        0,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        currField.setInt(0);
        int n = srcfield.compareTo(currField);
        if (n < 0) {
            currField.setInt(-1);
        } else if (n > 0) {
            currField.setInt(1);
        }
        return currField;
    }

    // cob_intr_stored_char_lengthの実装
    /**
     * COBOLのFUNCTION STORED-CHAR-LENGTHを実装する。
     *
     * <p>後続する空白を除いた、引数の実際の文字数を返す。
     * 例: "HELLO     "（10文字）に対して5を返す。
     *
     * @param srcfield 長さを取得する英数字フィールド
     * @return 後続空白を除いた文字数を表す数値フィールド
     */
    public static AbstractCobolField funcStoredCharLength(AbstractCobolField srcfield) {
        int count;

        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 8, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(4, (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        CobolDataStorage storage = srcfield.getDataStorage();
        for (count = srcfield.getSize(); count > 0; count--) {
            if (storage.getByte(count - 1) != ' ') {
                break;
            }
        }

        currField.setInt(count);
        return currField;
    }

    /**
     * COBOLのFUNCTION SUBSTITUTEを実装する。
     *
     * <p>文字列中の指定された部分文字列を別の文字列に置換する。
     * 複数の置換パターンを同時に指定できる。大文字/小文字を区別して比較する。
     * 例: FUNCTION SUBSTITUTE("HELLO", "L", "X") → "HEXXO"
     *
     * @param offset 参照修飾の開始位置（0以下の場合は参照修飾なし）
     * @param length 参照修飾の長さ
     * @param params 置換パターンの数（検索文字列と置換文字列のペア数×2）
     * @param fields 元の文字列、検索文字列1、置換文字列1、検索文字列2、置換文字列2...
     * @return 置換後の英数字フィールド
     */
    public static AbstractCobolField funcSubstitute(
            int offset, int length, int params, AbstractCobolField... fields) {
        int i, j, k;
        int numreps = params / 2;
        AbstractCobolField[] f1 = new AbstractCobolField[numreps];
        AbstractCobolField[] f2 = new AbstractCobolField[numreps];
        CobolDataStorage src = fields[0].getDataStorage();
        CobolDataStorage fData1;
        int srcSize = fields[0].getSize();
        int fSize1;
        StringBuilder rtn = new StringBuilder();

        for (i = 0; i < params - 1; i++) {
            if ((i % 2) == 0) {
                f1[i / 2] = fields[i + 1];
            } else {
                f2[i / 2] = fields[i + 1];
            }
        }

        for (i = 0; i < srcSize; ) {
            for (j = 0; j < numreps; j++) {
                fData1 = f1[j].getDataStorage();
                fSize1 = f1[j].getSize();
                for (k = fSize1 - 1; k >= 0; k--) {
                    if (i + k >= srcSize || src.getByte(i + k) != fData1.getByte(k)) {
                        break;
                    }
                }
                if (k < 0) {
                    rtn.append(f2[j].getString());
                    i += fSize1;
                    break;
                }
            }
            if (j == numreps) {
                rtn.append((char) src.getByte(i));
                i++;
            }
        }

        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(rtn.length(), (CobolDataStorage) null, attr);
        makeFieldEntry(field);
        currField.setDataStorage(new CobolDataStorage(rtn.toString()));

        if (offset > 0) {
            calcRefMod(currField, offset, length);
        }
        return currField;
    }

    /**
     * COBOLのFUNCTION SUBSTITUTE-CASEを実装する。
     *
     * <p>文字列中の指定された部分文字列を別の文字列に置換する（大文字/小文字を区別しない）。
     * SUBSTITUTEとの違いは、検索時に大文字/小文字を区別しない点である。
     * 例: FUNCTION SUBSTITUTE-CASE("Hello", "L", "X") → "HeXXo"
     *
     * @param offset 参照修飾の開始位置（0以下の場合は参照修飾なし）
     * @param length 参照修飾の長さ
     * @param params 置換パターンの数（検索文字列と置換文字列のペア数×2）
     * @param fields 元の文字列、検索文字列1、置換文字列1、検索文字列2、置換文字列2...
     * @return 置換後の英数字フィールド
     */
    public static AbstractCobolField funcSubstituteCase(
            int offset, int length, int params, AbstractCobolField... fields) {
        int i, j, k;
        int numreps = params / 2;
        AbstractCobolField[] f1 = new AbstractCobolField[numreps];
        AbstractCobolField[] f2 = new AbstractCobolField[numreps];
        CobolDataStorage src = fields[0].getDataStorage();
        CobolDataStorage fData1;
        int srcSize = fields[0].getSize();
        int fSize1;
        StringBuilder rtn = new StringBuilder();

        for (i = 0; i < params - 1; i++) {
            if (i % 2 == 0) {
                f1[i / 2] = fields[i + 1];
            } else {
                f2[i / 2] = fields[i + 1];
            }
        }

        for (i = 0; i < srcSize; ) {
            for (j = 0; j < numreps; j++) {
                fData1 = f1[j].getDataStorage();
                fSize1 = f1[j].getSize();
                for (k = fSize1 - 1; k >= 0; k--) {
                    if (i + k >= srcSize
                            || Character.toLowerCase((char) fData1.getByte(k))
                                    != Character.toLowerCase((char) src.getByte(i + k))) {
                        break;
                    }
                }
                if (k < 0) {
                    rtn.append(f2[j].getString());
                    i += fSize1;
                    break;
                }
            }
            if (j == numreps) {
                rtn.append((char) src.getByte(i));
                i++;
            }
        }

        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(rtn.length(), (CobolDataStorage) null, attr);
        makeFieldEntry(field);

        currField.setDataStorage(new CobolDataStorage(rtn.toString()));
        if (offset > 0) {
            calcRefMod(currField, offset, length);
        }
        return currField;
    }

    // Equivalent to cob_intr_trim
    /**
     * COBOLのFUNCTION TRIMを実装する。
     *
     * <p>文字列の先頭および/または末尾の空白を除去する。
     * directionパラメータにより、除去する方向を指定できる。
     *
     * @param offset 参照修飾の開始位置（0以下の場合は参照修飾なし）
     * @param length 参照修飾の長さ
     * @param srcField 空白を除去する元の英数字フィールド
     * @param direction 除去方向（1: 先頭のみ/LEADING、2: 末尾のみ/TRAILING、その他: 両方）
     * @return 空白除去後の英数字フィールド（全て空白の場合は空白1文字）
     */
    public static AbstractCobolField funcTrim(
            int offset, int length, AbstractCobolField srcField, int direction) {
        makeFieldEntry(srcField);
        int i;
        int srcFieldSize = srcField.getSize();
        CobolDataStorage srcStorage = srcField.getDataStorage();
        for (i = 0; i < srcFieldSize; ++i) {
            if (srcStorage.getByte(i) != ' ') {
                break;
            }
        }
        if (i == srcFieldSize) {
            currField.setSize(1);
            currField.getDataStorage().setByte(0, (byte) ' ');
            return currField;
        }
        int beginIndex = 0;
        if (direction != 2) {
            while (srcStorage.getByte(beginIndex) == ' ') {
                ++beginIndex;
            }
        }
        int endIndex = srcFieldSize - 1;
        if (direction != 1) {
            while (srcStorage.getByte(endIndex) == ' ') {
                --endIndex;
            }
        }
        CobolDataStorage currStorage = currField.getDataStorage();
        currField.setSize(endIndex - beginIndex + 1);
        for (i = 0; i <= endIndex - beginIndex; ++i) {
            currStorage.setByte(i, srcStorage.getByte(beginIndex + i));
        }
        if (offset > 0) {
            calcRefMod(currField, offset, length);
        }
        return currField;
    }

    /**
     * COBOLのFUNCTION LOCALE-DATEを実装する（ロケール未指定版）。
     *
     * <p>日付をシステムのデフォルトロケールに基づいた形式で返す。
     * 入力はYYYYMMDD形式の8桁数値または文字列。
     *
     * @param offset 参照修飾の開始位置（0以下の場合は参照修飾なし）
     * @param length 参照修飾の長さ
     * @param srcField YYYYMMDD形式の日付
     * @param localeField 未使用（オーバーロード用ダミー）
     * @return ロケールに基づいた日付文字列
     */
    public static AbstractCobolField funcLocaleDate(
            int offset, int length, AbstractCobolField srcField, int localeField) {
        return funcLocaleDate(offset, length, srcField, null);
    }

    /**
     * COBOLのFUNCTION LOCALE-DATEを実装する。
     *
     * <p>日付を指定されたロケールに基づいた形式で返す。
     * 入力はYYYYMMDD形式の8桁数値または文字列。年は1601～9999の範囲で有効。
     * 無効な日付の場合、"0000000000"を返しEC-ARGUMENT-FUNCTION例外を設定する。
     *
     * @param offset 参照修飾の開始位置（0以下の場合は参照修飾なし）
     * @param length 参照修飾の長さ
     * @param srcField YYYYMMDD形式の日付（数値または文字列）
     * @param localeField ロケールを指定する文字列（例: "ja", "en"）、nullの場合はデフォルト
     * @return ロケールに基づいた日付文字列
     */
    public static AbstractCobolField funcLocaleDate(
            int offset, int length, AbstractCobolField srcField, AbstractCobolField localeField) {
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(
                        0,
                        (CobolDataStorage) null,
                        new CobolFieldAttribute(
                                CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 10, 0, 0, null));
        int inDate;

        // Convert the input field to an integer
        if (srcField.getAttribute().isTypeNumeric()) {
            inDate = srcField.getInt();
        } else {
            if (srcField.getSize() < 8) {
                return errorFuncLocaleDate(field);
            }
            int p = 0;
            inDate = 0;
            for (int len = 0; len < 8; ++len, ++p) {
                char c = (char) srcField.getDataStorage().getByte(p);
                if ('0' <= c && c <= '9') {
                    inDate = inDate * 10 + (c - '0');
                } else {
                    return errorFuncLocaleDate(field);
                }
            }
        }

        // Calculate the year, month, and days
        int year = inDate / 10000;
        if (year < 1601 || year > 9999) {
            return errorFuncLocaleDate(field);
        }
        inDate %= 10000;

        int month = inDate / 100;
        if (month < 1 || month > 12) {
            return errorFuncLocaleDate(field);
        }

        int days = inDate % 100;
        if (days < 1 || days > 31) {
            return errorFuncLocaleDate(field);
        }

        if (isLeapYear(year)) {
            if (days > leapMonthDays[month]) {
                return errorFuncLocaleDate(field);
            }
        } else {
            if (days > normalMonthDays[month]) {
                return errorFuncLocaleDate(field);
            }
        }

        // Create the date string
        Calendar cal = Calendar.getInstance();
        cal.set(year, month - 1, days);

        DateFormat df;
        if (localeField != null) {
            Locale locale = new Locale(localeField.getString());
            df = DateFormat.getDateInstance(DateFormat.SHORT, locale);
        } else {
            df = DateFormat.getDateInstance(DateFormat.SHORT);
        }
        String dateString = df.format(cal.getTime());

        // Return the result
        field.setSize(dateString.length());
        makeFieldEntry(field);
        currField.getDataStorage().memcpy(dateString.getBytes(AbstractCobolField.charSetSJIS));
        if (offset > 0) {
            calcRefMod(field, offset, length);
        }
        return currField;
    }

    private static AbstractCobolField errorFuncLocaleDate(AbstractCobolField field) {
        field.setSize(10);
        makeFieldEntry(field);
        currField.getDataStorage().memset((byte) '0', 10);
        CobolRuntimeException.setException(CobolExceptionId.COB_EC_ARGUMENT_FUNCTION);
        return currField;
    }

    /**
     * COBOLのFUNCTION LOCALE-TIMEを実装する（ロケール未指定版）。
     *
     * <p>時刻をシステムのデフォルトロケールに基づいた形式で返す。
     * 入力はHHMMSS形式の6桁数値または文字列。
     *
     * @param offset 参照修飾の開始位置（0以下の場合は参照修飾なし）
     * @param length 参照修飾の長さ
     * @param srcField HHMMSS形式の時刻
     * @param localeField 未使用（オーバーロード用ダミー）
     * @return ロケールに基づいた時刻文字列（"HH:mm:ss"形式）
     */
    public static AbstractCobolField funcLocaleTime(
            int offset, int length, AbstractCobolField srcField, int localeField) {
        return funcLocaleTime(offset, length, srcField, null);
    }

    /**
     * COBOLのFUNCTION LOCALE-TIMEを実装する。
     *
     * <p>時刻を指定されたロケールに基づいた形式で返す。
     * 入力はHHMMSS形式の6桁数値または文字列。
     * 時は0～24、分・秒は0～59の範囲で有効。無効な場合は例外を設定する。
     *
     * @param offset 参照修飾の開始位置（0以下の場合は参照修飾なし）
     * @param length 参照修飾の長さ
     * @param srcField HHMMSS形式の時刻（数値または文字列）
     * @param localeField ロケールを指定する文字列、nullの場合はデフォルト
     * @return ロケールに基づいた時刻文字列（"HH:mm:ss"形式）
     */
    public static AbstractCobolField funcLocaleTime(
            int offset, int length, AbstractCobolField srcField, AbstractCobolField localeField) {
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(
                        0,
                        (CobolDataStorage) null,
                        new CobolFieldAttribute(
                                CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 10, 0, 0, null));
        int inTime;

        // Convert the input field to an integer
        if (srcField.getAttribute().isTypeNumeric()) {
            inTime = srcField.getInt();
        } else {
            if (srcField.getSize() < 6) {
                return errorFuncLocaleDate(field);
            }
            int p = 0;
            inTime = 0;
            for (int len = 0; len < 6; ++len, ++p) {
                char c = (char) srcField.getDataStorage().getByte(p);
                if ('0' <= c && c <= '9') {
                    inTime = inTime * 10 + (c - '0');
                } else {
                    return errorFuncLocaleDate(field);
                }
            }
        }

        // Calculate the hours, minutes, and seconds
        int hours = inTime / 10000;
        if (hours < 0 || hours > 24) {
            return errorFuncLocaleDate(field);
        }
        inTime %= 10000;

        int minutes = inTime / 100;
        if (minutes < 0 || minutes > 59) {
            return errorFuncLocaleDate(field);
        }

        int seconds = inTime % 100;
        if (seconds < 0 || seconds > 59) {
            return errorFuncLocaleDate(field);
        }

        // Create the time string
        LocalTime time = LocalTime.of(hours, minutes, seconds);

        DateTimeFormatter formatter;
        String pattern = "HH:mm:ss";
        if (localeField != null) {
            Locale locale = new Locale(localeField.getString());
            formatter = DateTimeFormatter.ofPattern(pattern, locale);
        } else {
            formatter = DateTimeFormatter.ofPattern(pattern);
        }
        String timeString = time.format(formatter);

        // Return the result
        field.setSize(timeString.length());
        makeFieldEntry(field);
        currField.getDataStorage().memcpy(timeString.getBytes(AbstractCobolField.charSetSJIS));
        if (offset > 0) {
            calcRefMod(field, offset, length);
        }
        return currField;
    }

    /**
     * COBOLのFUNCTION LOCALE-TIME-FROM-SECONDSを実装する（ロケール未指定版）。
     *
     * <p>午前0時からの秒数を時刻文字列に変換する。
     *
     * @param offset 参照修飾の開始位置（0以下の場合は参照修飾なし）
     * @param length 参照修飾の長さ
     * @param srcField 午前0時からの秒数を表す数値フィールド
     * @param localeField 未使用（オーバーロード用ダミー）
     * @return ロケールに基づいた時刻文字列（"HH:mm:ss"形式）
     */
    public static AbstractCobolField funcLocaleTimeFromSeconds(
            int offset, int length, AbstractCobolField srcField, int localeField) {
        return funcLocaleTime(offset, length, srcField, null);
    }

    /**
     * COBOLのFUNCTION LOCALE-TIME-FROM-SECONDSを実装する。
     *
     * <p>午前0時からの秒数を時刻文字列に変換する。
     * 秒数から時・分・秒を計算し、指定されたロケールに基づいた形式で返す。
     * 計算式: 時=秒/3600、分=(秒%3600)/60、秒=秒%60
     *
     * @param offset 参照修飾の開始位置（0以下の場合は参照修飾なし）
     * @param length 参照修飾の長さ
     * @param srcField 午前0時からの秒数を表す数値フィールド
     * @param localeField ロケールを指定する文字列、nullの場合はデフォルト
     * @return ロケールに基づいた時刻文字列（"HH:mm:ss"形式）
     */
    public static AbstractCobolField funcLocaleTimeFromSeconds(
            int offset, int length, AbstractCobolField srcField, AbstractCobolField localeField) {
        AbstractCobolField field =
                CobolFieldFactory.makeCobolField(
                        0,
                        (CobolDataStorage) null,
                        new CobolFieldAttribute(
                                CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 10, 0, 0, null));
        int inTime;

        // Convert the input field to an integer
        if (srcField.getAttribute().isTypeNumeric()) {
            inTime = srcField.getInt();
        } else {
            return errorFuncLocaleDate(field);
        }

        // Calculate the hours, minutes, and seconds
        int hours = inTime / 3600;
        inTime %= 3600;
        int minutes = inTime / 60;
        int seconds = inTime % 60;

        // Create the time string
        LocalTime time = LocalTime.of(hours, minutes, seconds);

        DateTimeFormatter formatter;
        String pattern = "HH:mm:ss";
        if (localeField != null) {
            Locale locale = new Locale(localeField.getString());
            formatter = DateTimeFormatter.ofPattern(pattern, locale);
        } else {
            formatter = DateTimeFormatter.ofPattern(pattern);
        }
        String timeString = time.format(formatter);

        // Return the result
        field.setSize(timeString.length());
        makeFieldEntry(field);
        currField.getDataStorage().memcpy(timeString.getBytes(AbstractCobolField.charSetSJIS));
        if (offset > 0) {
            calcRefMod(field, offset, length);
        }
        return currField;
    }
}

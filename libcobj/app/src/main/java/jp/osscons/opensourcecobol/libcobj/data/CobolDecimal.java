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

import java.math.BigDecimal;
import java.math.RoundingMode;
import jp.osscons.opensourcecobol.libcobj.common.CobolModule;
import jp.osscons.opensourcecobol.libcobj.common.CobolUtil;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolExceptionId;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolExceptionInfo;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;

/**
 * COBOL数値計算用のBigDecimalラッパークラス。
 * COMPUTE、ADD、SUBTRACT、MULTIPLY、DIVIDE等の算術演算で使用される。
 * COBOLの固定小数点演算をJavaのBigDecimalで実現する。
 */
public class CobolDecimal {
    /** スケールがNaN（非数）であることを示す定数。0除算等のエラー時に設定される。 */
    public static final int DECIMAL_NAN = -128;

    /** 演算結果を四捨五入するオプションフラグ */
    public static final int COB_STORE_ROUND = 0x01;

    /** オーバーフロー時に元の値を保持するオプションフラグ */
    public static final int COB_STORE_KEEP_ON_OVERFLOW = 0x02;

    /** オーバーフロー時に切り捨てるオプションフラグ */
    public static final int COB_STORE_TRUNC_ON_OVERFLOW = 0x04;

    /** バイナリ演算の最大桁数 */
    public static final int COB_MAX_BINARY = 36;

    private static BigDecimal cobMexp = BigDecimal.ZERO;

    /** 演算用の作業領域1 */
    static CobolDecimal cobD1 = new CobolDecimal();

    /** 演算用の作業領域2 */
    static CobolDecimal cobD2 = new CobolDecimal();

    /** 演算用の作業領域3。除算の剰余格納にも使用される。 */
    static CobolDecimal cobD3 = new CobolDecimal();

    /** 演算用の作業領域4 */
    static CobolDecimal cobD4 = new CobolDecimal();

    /** 10のべき乗の配列。数値変換の最適化に使用される。 */
    private static BigDecimal[] cobMpze10 = new BigDecimal[COB_MAX_BINARY];

    /** パック10進数変換用の作業バッファ */
    static byte[] packedValue = new byte[20];

    /** パック10進数変換用の作業変数 */
    static int packedValueInt = 0;

    /** 数値演算用の静的変数を初期化する */
    public static void cobInitNumeric() {
        cobD1 = new CobolDecimal();
        cobD2 = new CobolDecimal();
        cobD3 = new CobolDecimal();
        cobD4 = new CobolDecimal();
        cobMexp = BigDecimal.ZERO;
        for (int i = 0; i < COB_MAX_BINARY; ++i) {
            cobMpze10[i] = BigDecimal.ZERO;
            cobMpze10[i] = BigDecimal.TEN.pow(i);
        }
        for (int i = 0; i < packedValue.length; ++i) {
            packedValue[i] = 0;
        }
        packedValueInt = 0;
    }

    // TODO cob_init_numeric周辺の初期化処理を正しく実装出来たら消す。
    static {
        for (int i = 0; i < COB_MAX_BINARY; ++i) {
            cobMpze10[i] = BigDecimal.ZERO;
            cobMpze10[i] = BigDecimal.TEN.pow(i);
        }
        for (int i = 0; i < packedValue.length; ++i) {
            packedValue[i] = 0;
        }
        packedValueInt = 0;
    }

    /** 保持する数値データ */
    BigDecimal value;

    /** 小数点位置。正の値は小数点以下の桁数、負の値はPICTURE句のP（整数部の桁落とし）を示す。 */
    int scale;

    /** 値とスケールは0に設定する */
    public CobolDecimal() {
        this.value = BigDecimal.ZERO;
        this.setScale(0);
    }

    /**
     * 値はvalueを指定し,スケールは0に設定する
     *
     * @param value 設定する値
     */
    public CobolDecimal(BigDecimal value) {
        this.setValue(value);
        this.setScale(0);
    }

    /**
     * 値とスケールを指定する
     *
     * @param value 設定する値
     * @param scale 設定するスケール
     */
    public CobolDecimal(BigDecimal value, int scale) {
        this(value);
        this.setScale(scale);
    }

    /**
     * long値を指定し、スケールは0に設定する
     *
     * @param n 設定する整数値
     */
    public CobolDecimal(long n) {
        if (n == 0L) {
            this.setValue(BigDecimal.ZERO);
        } else if (n == 1L) {
            this.setValue(BigDecimal.ONE);
        } else if (n == 10L) {
            this.setValue(BigDecimal.TEN);
        } else {
            this.setValue(new BigDecimal(n));
        }
        this.setScale(0);
    }

    /**
     * int値を指定し、スケールは0に設定する
     *
     * @param n 設定する整数値
     */
    public CobolDecimal(int n) {
        if (n == 0) {
            this.setValue(BigDecimal.ZERO);
        } else if (n == 1) {
            this.setValue(BigDecimal.ONE);
        } else if (n == 10) {
            this.setValue(BigDecimal.TEN);
        } else {
            this.setValue(new BigDecimal(n));
        }
        this.setScale(0);
    }

    /**
     * コピーコンストラクタ。otherの値とスケールをコピーして新しいインスタンスを生成する。
     *
     * @param other コピー元のCobolDecimal
     */
    public CobolDecimal(CobolDecimal other) {
        this.setValue(other.getValue());
        this.setScale(other.getScale());
    }

    /**
     * このオブジェクトの値を取得する
     *
     * @return このオブジェクトの値
     */
    public BigDecimal getValue() {
        return value;
    }

    /** 数値を0にリセットする。スケールは変更しない。 */
    public void decimalInit() {
        this.value = BigDecimal.ZERO;
    }

    /**
     * このオブジェクトの値を設定する
     *
     * @param value このオブジェクトに設定する値
     */
    public void setValue(BigDecimal value) {
        this.value = value;
    }

    /** this.valueの値を0にする */
    public void clear() {
        this.value = BigDecimal.ZERO;
    }

    /**
     * このオブジェクトのスケールを設定する
     *
     * @param scale このオブジェクトに設定するスケール
     */
    public void setScale(int scale) {
        this.scale = scale;
    }

    /**
     * このオブジェクトのスケールを取得する
     *
     * @return このオブジェクトのスケール
     */
    public int getScale() {
        return this.scale;
    }

    /**
     * このオブジェクトの値を設定し、スケールは0に設定する
     *
     * @param n このオブジェクトに設定する値
     */
    public void set(int n) {
        this.value = new BigDecimal(n);
        this.scale = 0;
    }

    /**
     * このオブジェクトの値を設定し、スケールは0に設定する
     *
     * @param n このオブジェクトに設定する値
     */
    public void set(long n) {
        this.value = new BigDecimal(n);
        this.scale = 0;
    }

    /**
     * 別のCobolDecimalから値とスケールをコピーする
     *
     * @param decimal コピー元のCobolDecimal
     */
    public void set(CobolDecimal decimal) {
        // TODO よりよいコピーの方法を考える
        this.value = decimal.value.add(BigDecimal.ZERO);
        this.setScale(decimal.getScale());
    }

    /**
     * COBOLフィールドから数値を取得して設定する
     *
     * @param f 数値を取得するCOBOLフィールド
     */
    public void setField(AbstractCobolField f) {
        CobolDecimal decimal = f.getDecimal();
        this.setValue(decimal.getValue());
        this.setScale(decimal.getScale());
    }

    /**
     * 2つのCobolDecimalがNaN（非数）かどうかをチェックする。
     * どちらかがNaNの場合、d1をNaNに設定する。
     *
     * @param d1 チェック対象の1つ目のCobolDecimal（NaN時に設定される）
     * @param d2 チェック対象の2つ目のCobolDecimal
     * @return どちらかがNaNの場合true、それ以外はfalse
     */
    private static boolean DECIMAL_CHECK(CobolDecimal d1, CobolDecimal d2) {
        if (d1.getScale() == DECIMAL_NAN || d2.getScale() == DECIMAL_NAN) {
            d1.setScale(DECIMAL_NAN);
            return true;
        }
        return false;
    }

    /**
     * このオブジェクトの示す数値に対して加算を行う
     *
     * @param n このオブジェクトに加算する値
     */
    public void add(int n) {
        this.value = this.value.add(new BigDecimal(n));
    }

    /**
     * このオブジェクトの示す数値に対して加算を行う
     *
     * @param decimal このオブジェクトに加算する値
     */
    public void add(CobolDecimal decimal) {
        if (DECIMAL_CHECK(this, decimal)) {
            return;
        }
        alignDecimal(this, decimal);
        this.setValue(this.getValue().add(decimal.getValue()));
    }

    /**
     * このオブジェクトの示す数値に対して減算を行う
     *
     * @param decimal このオブジェクトの示す数値から減算する値
     */
    public void sub(CobolDecimal decimal) {
        if (DECIMAL_CHECK(this, decimal)) {
            return;
        }
        alignDecimal(this, decimal);
        this.setValue(this.getValue().subtract(decimal.getValue()));
    }

    /**
     * このオブジェクトの示す数値に対して減算を行う
     *
     * @param n このオブジェクトの示す数値から減算する値
     */
    public void sub(int n) {
        this.value = this.value.subtract(new BigDecimal(n));
    }

    /**
     * このオブジェクトの示す数値に対して乗算を行う
     *
     * @param decimal このオブジェクトの示す数値に乗算する値
     */
    public void mul(CobolDecimal decimal) {
        if (DECIMAL_CHECK(this, decimal)) {
            return;
        }
        this.setScale(this.getScale() + decimal.getScale());
        this.setValue(this.getValue().multiply(decimal.getValue()));
    }

    /**
     * このオブジェクトの示す数値に対して乗算を行う
     *
     * @param n このオブジェクトの示す数値に乗算する値
     */
    public void mul(int n) {
        this.value = this.value.multiply(new BigDecimal(n));
    }

    /**
     * このオブジェクトの示す数値に対して除算を行う
     *
     * @param decimal このオブジェクトの示す数値を除算する値
     * @throws CobolStopRunException 0除算が発生し、エラー時終了フラグが設定されている場合
     */
    public void div(CobolDecimal decimal) throws CobolStopRunException {
        if (DECIMAL_CHECK(this, decimal)) {
            return;
        }
        if (decimal.getValue().signum() == 0) {
            this.setScale(DECIMAL_NAN);
            if (CobolUtil.cobErrorOnExitFlag) {
                CobolUtil.runtimeError("Detected division by zero.");
                CobolStopRunException.stopRunAndThrow(1);
                ;
            }
            return;
        }
        if (this.getValue().signum() == 0) {
            this.setScale(0);
            return;
        }
        this.setScale(this.getScale() - decimal.getScale());
        int shift = 37 + ((this.getScale() < 0) ? -this.getScale() : 0);
        this.shiftDecimal(shift);
        this.setValue(this.getValue().divide(decimal.getValue(), RoundingMode.DOWN));
    }

    /**
     * このオブジェクトの示す数値に対して除算を行う
     *
     * @param n このオブジェクトの示す数値を除算する値
     */
    public void div(int n) {
        this.value = this.value.divide(new BigDecimal(n), RoundingMode.DOWN);
    }

    /**
     * このオブジェクトの示す数値に対して累乗を行う
     *
     * @param decimal このオブジェクトの示す数値を累乗する値
     */
    // TODO 残りの実装
    public void pow(CobolDecimal decimal) {
        if (DECIMAL_CHECK(this, decimal)) {
            return;
        }

        if (decimal.getScale() == 0
                && decimal.getValue().compareTo(new BigDecimal(2147483647)) <= 0) {
            int n = decimal.getValue().intValue();
            this.value = this.value.pow(n);
            this.setScale(this.getScale() * n);
        } else {
            this.decimalSetDouble(Math.pow(this.decimalGetDouble(), decimal.decimalGetDouble()));
        }
    }

    /**
     * double値からCobolDecimalを設定する。スケール9で正規化される。
     *
     * @param v 設定するdouble値
     */
    private void decimalSetDouble(double v) {
        this.setValue(new BigDecimal(v * 1.0e9));
        this.setScale(9);
    }

    private double decimalGetDouble() {
        double v = this.getValue().doubleValue();
        int n = this.getScale();
        for (; n > 0; n--) {
            v /= 10;
        }
        for (; n < 0; n++) {
            v *= 10;
        }
        return v;
    }

    /**
     * このCobolDecimalの値をCOBOLフィールドに格納する。
     * フィールドの型に応じて適切な変換（DISPLAY、PACKED、BINARY等）を行う。
     *
     * @param f 値を格納する先のCOBOLフィールド
     * @param opt 格納オプション（COB_STORE_ROUND、COB_STORE_KEEP_ON_OVERFLOW等）
     * @return 正常終了時は0、オーバーフロー時は例外コード
     * @throws CobolStopRunException 致命的なエラーが発生した場合
     */
    public int getField(AbstractCobolField f, int opt) throws CobolStopRunException {
        if (this.getScale() == CobolDecimal.DECIMAL_NAN) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_SIZE_OVERFLOW);
            return CobolRuntimeException.code;
        }

        CobolDecimal d = new CobolDecimal(this);

        /* rounding */
        if ((opt & CobolDecimal.COB_STORE_ROUND) != 0) {
            if (f.getAttribute().getScale() < d.getScale()) {
                int sign = d.value.signum();
                if (sign != 0) {
                    d.shiftDecimal(f.getAttribute().getScale() - d.getScale() + 1);
                    if (sign > 0) {
                        d.add(5);
                    } else {
                        d.sub(5);
                    }
                }
            }
        }

        d.shiftDecimal(f.getAttribute().getScale() - d.getScale());

        // TODO 残りのパターンも実装
        switch (f.getAttribute().getType()) {
            case CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY:
                return d.getDisplayField(f, opt);
            case CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED:
                return d.getPackedField(f, opt);
            case CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY:
                return d.getBinaryField(f, opt);
            case CobolFieldAttribute.COB_TYPE_NUMERIC_FLOAT:
                System.out.println("getField: Float not implemented");
                throw new CobolRuntimeException(0, "getField: Float not implemented");
            case CobolFieldAttribute.COB_TYPE_NUMERIC_DOUBLE:
                return d.getDoubleField(f, opt);
            default:
                int digits = f.getAttribute().getDigits();
                CobolFieldAttribute attr = f.getAttribute();
                CobolFieldAttribute newAttr =
                        new CobolFieldAttribute(
                                CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                                digits,
                                attr.getScale(),
                                CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                                null);
                AbstractCobolField displayField =
                        CobolFieldFactory.makeCobolField(
                                digits, new CobolDataStorage(digits), newAttr);
                if (d.getField(displayField, opt) == 0) {
                    f.moveFrom(displayField);
                }
                return CobolExceptionInfo.code;
        }
    }

    /**
     * このCobolDecimalの値をCOBOL DOUBLE型フィールドに格納する
     *
     * @param f 値を格納する先のCOBOL DOUBLE型フィールド
     * @param opt 格納オプション（現在未使用）
     * @return 常に0（正常終了）
     */
    public int getDoubleField(AbstractCobolField f, int opt) {
        CobolDataStorage storage = new CobolDataStorage(8);
        double val = this.value.doubleValue();
        int scale = this.scale;
        for (int i = 0; i < Math.abs(scale); ++i) {
            if (scale > 0) {
                val /= 10;
            } else {
                val *= 10;
            }
        }
        storage.set(val);
        f.setDataStorage(storage);
        return 0;
    }

    /**
     * 小数点位置をシフトする。正の値で右シフト（10のn乗を乗算）、負の値で左シフト（10の-n乗で除算）。
     *
     * @param n シフト量（正:右シフト、負:左シフト）
     */
    public void shiftDecimal(int n) {
        if (n == 0) {
            return;
        }
        if (n > 0) {
            cobMexp = BigDecimal.TEN.pow(n);
            this.value = this.value.multiply(cobMexp);
        } else {
            cobMexp = BigDecimal.TEN.pow(-n);
            this.value = this.value.divide(cobMexp, RoundingMode.DOWN);
        }
        this.setScale(this.getScale() + n);
    }

    // libcob/numeric.cのalign_decimalの実装
    /**
     * 2つのCobolDecimalのスケールを揃える。スケールが小さい方を大きい方に合わせる。
     * libcob/numeric.cのalign_decimalに対応する。
     *
     * @param d1 スケールを揃える1つ目のCobolDecimal
     * @param d2 スケールを揃える2つ目のCobolDecimal
     */
    public void alignDecimal(CobolDecimal d1, CobolDecimal d2) {
        if (d1.getScale() < d2.getScale()) {
            d1.shiftDecimal(d2.getScale() - d1.getScale());
        } else if (d1.getScale() > d2.getScale()) {
            d2.shiftDecimal(d1.getScale() - d2.getScale());
        }
    }

    // libcob/numeric.cのcob_decimal_cmpの実装 引数で与えられたCobolDecimal型のインスタンスとの比較をする.
    /**
     * 引数で与えられたCobolDecimalと比較する。
     * libcob/numeric.cのcob_decimal_cmpに対応する。
     *
     * @param decimal 比較対象のCobolDecimal
     * @return this &lt; decimalなら負の値、this == decimalなら0、this &gt; decimalなら正の値
     */
    public int compareTo(CobolDecimal decimal) {
        alignDecimal(this, decimal);
        BigDecimal v1 = this.getValue().movePointLeft(this.getScale());
        BigDecimal v2 = decimal.getValue().movePointLeft(decimal.getScale());
        return v1.compareTo(v2);
    }

    // libcob/numeric.cのcob_decimal_get_displayの実装
    /**
     * このCobolDecimalの値をCOBOL DISPLAY型（数字表示型）フィールドに格納する。
     * libcob/numeric.cのcob_decimal_get_displayに対応する。
     *
     * @param f 値を格納する先のDISPLAY型フィールド
     * @param opt 格納オプション（COB_STORE_KEEP_ON_OVERFLOW等）
     * @return 正常終了時は0、オーバーフロー時は例外コード
     * @throws CobolStopRunException 致命的なエラーが発生した場合
     */
    public int getDisplayField(AbstractCobolField f, int opt) throws CobolStopRunException {
        int sign = this.value.signum();
        this.value = this.value.abs();
        String numString = this.value.toPlainString();
        int dPointIndex = numString.indexOf('.');
        numString = numString.replace(".", "");
        byte[] numBuffPtr = numString.getBytes(AbstractCobolField.charSetSJIS);
        if (dPointIndex < 0) {
            dPointIndex = numBuffPtr.length;
        }
        dPointIndex -= this.scale;
        int size = numBuffPtr.length;

        CobolDataStorage data = f.getDataStorage();
        int diff = f.getFieldSize() - size;

        if (diff < 0) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_SIZE_OVERFLOW);
            if ((opt & CobolDecimal.COB_STORE_KEEP_ON_OVERFLOW) > 0) {
                return CobolRuntimeException.code;
            }
        }

        int fFirstIndex = f.getFirstDataIndex();
        int fFieldSize = f.getFieldSize();
        int fPointIndex = fFieldSize - f.getAttribute().getScale();
        for (int i = 0; i < fFieldSize; i++) {
            int fIndex = fFirstIndex + i;
            int dIndex = i + dPointIndex - fPointIndex;
            if (0 <= dIndex && dIndex < numBuffPtr.length) {
                data.setByte(fIndex, numBuffPtr[dIndex]);
            } else {
                data.setByte(fIndex, (byte) '0');
            }
        }
        f.putSign(sign);
        return 0;
    }

    // libcob/numeric.cのcob_decimal_get_packedの実装
    /**
     * このCobolDecimalの値をCOBOL PACKED型（COMP-3、パック10進数）フィールドに格納する。
     * libcob/numeric.cのcob_decimal_get_packedに対応する。
     *
     * @param f 値を格納する先のPACKED型フィールド
     * @param opt 格納オプション（COB_STORE_KEEP_ON_OVERFLOW等）
     * @return 正常終了時は0、オーバーフロー時は例外コード
     */
    public int getPackedField(AbstractCobolField f, int opt) {
        int sign = this.value.signum();
        this.value = this.value.abs();
        String numString = this.value.toPlainString();
        int dPointIndex = numString.indexOf('.');
        numString = numString.replace(".", "");
        byte[] numBuffPtr = numString.getBytes(AbstractCobolField.charSetSJIS);
        if (dPointIndex < 0) {
            dPointIndex = numBuffPtr.length;
        }
        dPointIndex -= this.scale;
        int size = numBuffPtr.length;
        int fPointIndex = f.getAttribute().getDigits() - f.getAttribute().getScale();

        CobolDataStorage data = f.getDataStorage();
        int digits = f.getAttribute().getDigits();
        int diff = digits - size;
        if (diff < 0) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_SIZE_OVERFLOW);
            if ((opt & CobolDecimal.COB_STORE_KEEP_ON_OVERFLOW) > 0) {
                return CobolRuntimeException.code;
            }
        }
        data.fillBytes(0, f.getSize());
        for (int i = 0; i < digits; ++i) {
            int j = i - fPointIndex + dPointIndex;
            if (j < 0 || numBuffPtr.length <= j) {
                continue;
            }
            byte val = (byte) ((int) numBuffPtr[j] - '0');
            int index;
            if (digits % 2 == 0) {
                index = (i + 1) / 2;
            } else {
                index = i / 2;
            }
            byte b = data.getByte(index);
            if ((digits + i) % 2 == 0) {
                data.setByte(index, (b & 0xF0) | val);
            } else {
                data.setByte(index, (val << 4) | (b & 0x0F));
            }
        }

        int p = f.getSize() - 1;
        byte x = data.getByte(p);
        if (!f.getAttribute().isFlagHaveSign()) {
            data.setByte(p, (byte) ((x & 0xF0) | 0x0F));
        } else if (sign < 0) {
            data.setByte(p, (byte) ((x & 0xF0) | 0x0D));
        } else {
            data.setByte(p, (byte) ((x & 0xF0) | 0x0C));
        }

        return 0;
    }

    // libcob/numeric.cのcob_decimal_get_binaryの実装
    /**
     * このCobolDecimalの値をCOBOL BINARY型（COMP、COMP-5）フィールドに格納する。
     * libcob/numeric.cのcob_decimal_get_binaryに対応する。
     *
     * @param f 値を格納する先のBINARY型フィールド
     * @param opt 格納オプション（COB_STORE_KEEP_ON_OVERFLOW、COB_STORE_TRUNC_ON_OVERFLOW等）
     * @return 正常終了時は0、オーバーフロー時は例外コード
     */
    private int getBinaryField(AbstractCobolField f, int opt) {
        CobolDataStorage data = f.getDataStorage();
        CobolFieldAttribute attr = f.getAttribute();
        if (this.getValue().signum() == 0) {
            data.fillBytes(0, f.getSize());
            return 0;
        }
        int overflow = 0;
        int digits = attr.getDigits();
        int sign;
        if (attr.isFlagHaveSign()) {
            sign = 1;
        } else {
            sign = 0;
            if (this.value.signum() < 0) {
                this.value = this.value.abs();
            }
        }
        int bitnum = f.getSize() * 8 - sign;
        outer:
        {
            if (this.getValue().compareTo(new BigDecimal(2).pow(bitnum).subtract(BigDecimal.ONE))
                    > 0) {
                if ((opt & COB_STORE_KEEP_ON_OVERFLOW) != 0) {
                    break outer;
                }
                overflow = 1;
                if ((opt & COB_STORE_TRUNC_ON_OVERFLOW) != 0) {
                    this.setValue(this.getValue().remainder(cobMpze10[digits]));
                } else {
                    this.setValue(
                            this.getValue().remainder(new BigDecimal(2).pow(f.getSize() * 8)));
                }
            } else if ((opt != 0) && CobolModule.getCurrentModule().flag_binary_truncate != 0) {
                if (this.getValue().abs().compareTo(cobMpze10[digits].abs()) >= 0) {
                    if ((opt & COB_STORE_KEEP_ON_OVERFLOW) != 0) {
                        break outer;
                    }
                    overflow = 1;
                    if ((opt & COB_STORE_TRUNC_ON_OVERFLOW) != 0) {
                        this.setValue(this.getValue().remainder(cobMpze10[digits]));
                    } else {
                        this.setValue(
                                this.getValue()
                                        .remainder(new BigDecimal(2).pow(f.getFieldSize() * 8)));
                    }
                }
            }
            ((CobolNumericBinaryField) f).setLongValue(this.getValue().longValue());
            if (overflow == 0) {
                return 0;
            }
        }
        CobolRuntimeException.setException(CobolExceptionId.COB_EC_SIZE_OVERFLOW);
        return CobolExceptionInfo.code;
    }

    // libcob/numeric.cのnum_byte_memcpyの実装
    /**
     * CobolDataStorage間でバイト単位のコピーを行う。
     * libcob/numeric.cのnum_byte_memcpyに対応する。
     *
     * @param s1 コピー先のCobolDataStorage
     * @param s1StartIndex コピー先の開始インデックス
     * @param s2 コピー元のCobolDataStorage
     * @param s2StartIndex コピー元の開始インデックス
     * @param size コピーするバイト数
     */
    public static void numByteMemcpy(
            CobolDataStorage s1,
            int s1StartIndex,
            CobolDataStorage s2,
            int s2StartIndex,
            int size) {
        int i1 = s1StartIndex;
        int i2 = s2StartIndex;
        do {
            s1.setByte(i1++, s2.getByte(i2++));
        } while (--size != 0);
    }
}

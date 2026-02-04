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

/**
 * COBOLフィールドの属性情報を保持するクラス。
 * フィールドの型、桁数、小数点位置、各種フラグ、PICTURE文字列などを格納する。
 * COBOLからJavaへのトランスパイル時に生成されるコードで、各データ項目の定義に使用される。
 */
public class CobolFieldAttribute {

    /* field types */

    /** 未定義の型を示す定数 */
    public static final int COB_TYPE_UNKNOWN = 0x00;

    /** 集団項目（GROUP）を示す型定数。複数の基本項目をまとめた構造体のようなデータ型。 */
    public static final int COB_TYPE_GROUP = 0x01;

    /** 真偽値（BOOLEAN）を示す型定数 */
    public static final int COB_TYPE_BOOLEAN = 0x02;

    /** 数値型を示す基本定数。数値型かどうかの判定にビット演算で使用される。 */
    public static final int COB_TYPE_NUMERIC = 0x10;

    /** 数値表示型（PIC 9）を示す型定数。外部10進数形式で数値を格納する。 */
    public static final int COB_TYPE_NUMERIC_DISPLAY = 0x10;

    /** 2進数型（COMP/COMP-4/BINARY）を示す型定数。バイナリ形式で数値を格納する。 */
    public static final int COB_TYPE_NUMERIC_BINARY = 0x11;

    /** パック10進数型（COMP-3）を示す型定数。内部10進数形式で数値を格納する。 */
    public static final int COB_TYPE_NUMERIC_PACKED = 0x12;

    /** 単精度浮動小数点型（COMP-1）を示す型定数 */
    public static final int COB_TYPE_NUMERIC_FLOAT = 0x13;

    /** 倍精度浮動小数点型（COMP-2）を示す型定数 */
    public static final int COB_TYPE_NUMERIC_DOUBLE = 0x14;

    /** 数値編集型を示す型定数。PIC句に編集文字（Z, *, CR等）を含む数値項目。 */
    public static final int COB_TYPE_NUMERIC_EDITED = 0x24;

    /** 英数字型（PIC X）を示す型定数。任意の文字を格納できる。 */
    public static final int COB_TYPE_ALPHANUMERIC = 0x21;

    /** 英数字ALL定数を示す型定数。ALL "文字"などの繰り返し定数を表す。 */
    public static final int COB_TYPE_ALPHANUMERIC_ALL = 0x22;

    /** 英数字編集型を示す型定数。PIC句に編集文字を含む英数字項目。 */
    public static final int COB_TYPE_ALPHANUMERIC_EDITED = 0x23;

    /** 日本語型（PIC N）を示す型定数。日本語文字（全角文字）を格納する。 */
    public static final int COB_TYPE_NATIONAL = 0x40;

    /** 日本語編集型を示す型定数。編集文字を含む日本語項目。 */
    public static final int COB_TYPE_NATIONAL_EDITED = 0x41;

    /** 日本語ALL定数を示す型定数。ALL NC"文字"などの繰り返し定数を表す。 */
    public static final int COB_TYPE_NATIONAL_ALL = 0x42;

    /* field flags */

    /** フラグが指定されていないことを示す定数 */
    public static final int COB_FLAG_NOT_SPECIFIED = 0x00;

    /** 符号付き数値（SIGN句またはS付きPIC句）を示すフラグ */
    public static final int COB_FLAG_HAVE_SIGN = 0x01;

    /** 符号分離（SIGN IS SEPARATE）を示すフラグ。符号が独立した文字として格納される。 */
    public static final int COB_FLAG_SIGN_SEPARATE = 0x02;

    /** 符号先行（SIGN IS LEADING）を示すフラグ。符号がデータの先頭に配置される。 */
    public static final int COB_FLAG_SIGN_LEADING = 0x04;

    /** ゼロ時空白（BLANK WHEN ZERO）を示すフラグ。値が0の場合に空白で表示される。 */
    public static final int COB_FLAG_BLANK_ZERO = 0x08;

    /** 右詰め（JUSTIFIED RIGHT）を示すフラグ。データが右詰めで格納される。 */
    public static final int COB_FLAG_JUSTIFIED = 0x10;

    /** バイトスワップを示すフラグ。ビッグエンディアン/リトルエンディアン変換が必要なバイナリ項目。 */
    public static final int COB_FLAG_BINARY_SWAP = 0x20;

    /** 実バイナリを示すフラグ。COMP-5（ネイティブバイナリ）を表す。 */
    public static final int COB_FLAG_REAL_BINARY = 0x40;

    /** ポインタ型を示すフラグ。アドレスを格納するデータ項目。 */
    public static final int COB_FLAG_IS_POINTER = 0x80;

    /** 変数種別 */
    private int type;

    /** 数値の時,桁数を示す */
    private int digits;

    /** 数値の時,スケールを示す */
    private int scale;

    /** 様々なフラグ */
    private int flags;

    /** PICTURE句の文字列 */
    private String pic;

    /**
     * コンストラクタ
     *
     * @param type フィールドの種別
     * @param digits 数値型のとき,桁数を表す
     * @param scale 数値型の時,スケールを表す
     * @param flags 各種フラグ
     * @param pic PIC文字列
     */
    public CobolFieldAttribute(int type, int digits, int scale, int flags, String pic) {
        this.type = type;
        this.digits = digits;
        this.scale = scale;
        this.flags = flags;
        this.pic = pic;
    }

    /**
     * コピーコンストラクタ。指定されたCobolFieldAttributeの値をコピーして新しいインスタンスを生成する。
     *
     * @param other コピー元のCobolFieldAttributeインスタンス
     */
    public CobolFieldAttribute(CobolFieldAttribute other) {
        this.type = other.type;
        this.digits = other.digits;
        this.scale = other.scale;
        this.flags = other.flags;
        this.pic = other.pic;
    }

    /**
     * 変数種別を取得する
     *
     * @return このオブジェクトの変数種別
     */
    public int getType() {
        return type;
    }

    /**
     * 変数種別を取得する
     *
     * @param type このオブジェクトに設定する変数種別
     */
    public void setType(int type) {
        this.type = type;
    }

    /**
     * 桁数を取得する
     *
     * @return このオブジェクトの桁数
     */
    public int getDigits() {
        return digits;
    }

    /**
     * 桁数を設定する
     *
     * @param digits このオブジェクトに設定する桁数
     */
    public void setDigits(int digits) {
        this.digits = digits;
    }

    /**
     * このオブジェクトのスケールを取得する
     *
     * @return このオブジェクトのスケール
     */
    public int getScale() {
        return scale;
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
     * このオブジェクトのフラグを設定する
     *
     * @return このオブジェクトのフラグ
     */
    public int getFlags() {
        return this.flags;
    }

    /**
     * このオブジェクトのフラグを設定する
     *
     * @param flags このオブジェクトに設定するフラグ
     */
    public void setFlags(int flags) {
        this.flags = flags;
    }

    /**
     * このオブジェクトのPICTURE文字列を取得する
     *
     * @return このオブジェクトのPICTURE文字列
     */
    public String getPic() {
        return pic;
    }

    /**
     * このオブジェクトのPICTURE文字列を設定する
     *
     * @param pic このオブジェクトに設定するPICTURE文字列
     */
    public void setPic(String pic) {
        this.pic = pic;
    }

    /**
     * このフィールドが未定義型かどうかを判定する
     *
     * @return 未定義型の場合true
     */
    public boolean isTypeUnknown() {
        return type == COB_TYPE_UNKNOWN;
    }

    /**
     * このフィールドが集団項目（GROUP）かどうかを判定する
     *
     * @return 集団項目の場合true
     */
    public boolean isTypeGroup() {
        return type == COB_TYPE_GROUP;
    }

    /**
     * このフィールドが真偽値型かどうかを判定する
     *
     * @return 真偽値型の場合true
     */
    public boolean isTypeBoolean() {
        return type == COB_TYPE_BOOLEAN;
    }

    /**
     * このフィールドが数値型かどうかを判定する。
     * 数値表示型、バイナリ型、パック10進数型などすべての数値型でtrueを返す。
     *
     * @return 数値型の場合true
     */
    public boolean isTypeNumeric() {
        return (type & COB_TYPE_NUMERIC) > 0;
    }

    /**
     * このフィールドが数値表示型（PIC 9）かどうかを判定する
     *
     * @return 数値表示型の場合true
     */
    public boolean isTypeNumericDisplay() {
        return type == COB_TYPE_NUMERIC_DISPLAY;
    }

    /**
     * このフィールドがバイナリ型（COMP/COMP-4/BINARY）かどうかを判定する
     *
     * @return バイナリ型の場合true
     */
    public boolean isTypeNumericBinary() {
        return type == COB_TYPE_NUMERIC_BINARY;
    }

    /**
     * このフィールドがパック10進数型（COMP-3）かどうかを判定する
     *
     * @return パック10進数型の場合true
     */
    public boolean isTypeNumericPacked() {
        return type == COB_TYPE_NUMERIC_PACKED;
    }

    /**
     * このフィールドが単精度浮動小数点型（COMP-1）かどうかを判定する
     *
     * @return 単精度浮動小数点型の場合true
     */
    public boolean isTypeNumericFloat() {
        return type == COB_TYPE_NUMERIC_FLOAT;
    }

    /**
     * このフィールドが倍精度浮動小数点型（COMP-2）かどうかを判定する
     *
     * @return 倍精度浮動小数点型の場合true
     */
    public boolean isTypeNumericDouble() {
        return type == COB_TYPE_NUMERIC_DOUBLE;
    }

    /**
     * このフィールドが数値編集型かどうかを判定する
     *
     * @return 数値編集型の場合true
     */
    public boolean isTypeNumericEdited() {
        return type == COB_TYPE_NUMERIC_EDITED;
    }

    /**
     * このフィールドが英数字型（PIC X）かどうかを判定する
     *
     * @return 英数字型の場合true
     */
    public boolean isTypeAlphanum() {
        return type == COB_TYPE_ALPHANUMERIC;
    }

    /**
     * このフィールドが英数字ALL定数かどうかを判定する
     *
     * @return 英数字ALL定数の場合true
     */
    public boolean isTypeAlphanumAll() {
        return type == COB_TYPE_ALPHANUMERIC_ALL;
    }

    /**
     * このフィールドが英数字編集型かどうかを判定する
     *
     * @return 英数字編集型の場合true
     */
    public boolean isTypeAlphanumEdited() {
        return type == COB_TYPE_ALPHANUMERIC_EDITED;
    }

    /**
     * このフィールドが日本語型（PIC N）かどうかを判定する
     *
     * @return 日本語型の場合true
     */
    public boolean isTypeNational() {
        return type == COB_TYPE_NATIONAL;
    }

    /**
     * このフィールドが日本語編集型かどうかを判定する
     *
     * @return 日本語編集型の場合true
     */
    public boolean isTypeNationalEdited() {
        return type == COB_TYPE_NATIONAL_EDITED;
    }

    /**
     * このフィールドが日本語ALL定数かどうかを判定する
     *
     * @return 日本語ALL定数の場合true
     */
    public boolean isTypeNationalAll() {
        return type == COB_TYPE_NATIONAL_ALL;
    }

    /**
     * このフィールドが符号付きかどうかを判定する
     *
     * @return 符号付きの場合true
     */
    public boolean isFlagHaveSign() {
        return (flags & COB_FLAG_HAVE_SIGN) != 0;
    }

    /**
     * このフィールドの符号が分離されているかどうかを判定する
     *
     * @return 符号分離の場合true
     */
    public boolean isFlagSignSeparate() {
        return (flags & COB_FLAG_SIGN_SEPARATE) != 0;
    }

    /**
     * このフィールドの符号が先頭にあるかどうかを判定する
     *
     * @return 符号先行の場合true
     */
    public boolean isFlagSignLeading() {
        return (flags & COB_FLAG_SIGN_LEADING) != 0;
    }

    /**
     * このフィールドがBLANK WHEN ZERO指定かどうかを判定する
     *
     * @return BLANK WHEN ZERO指定の場合true
     */
    public boolean isFlagBlankZero() {
        return (flags & COB_FLAG_BLANK_ZERO) != 0;
    }

    /**
     * このフィールドがJUSTIFIED RIGHT指定かどうかを判定する
     *
     * @return JUSTIFIED RIGHT指定の場合true
     */
    public boolean isFlagJustified() {
        return (flags & COB_FLAG_JUSTIFIED) != 0;
    }

    /**
     * このフィールドがバイトスワップを必要とするかどうかを判定する
     *
     * @return バイトスワップが必要な場合true
     */
    public boolean isFlagBinarySwap() {
        return (flags & COB_FLAG_BINARY_SWAP) != 0;
    }

    /**
     * このフィールドが実バイナリ（COMP-5）かどうかを判定する
     *
     * @return 実バイナリの場合true
     */
    public boolean isFlagRealBinary() {
        return (flags & COB_FLAG_REAL_BINARY) != 0;
    }

    /**
     * このフィールドがポインタ型かどうかを判定する
     *
     * @return ポインタ型の場合true
     */
    public boolean isFlagIsPointer() {
        return (flags & COB_FLAG_IS_POINTER) != 0;
    }
}

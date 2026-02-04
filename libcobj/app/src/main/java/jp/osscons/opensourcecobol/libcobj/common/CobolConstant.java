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

import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory;

/**
 * COBOLの表意定数（Figurative Constants）およびランタイム定数を定義するクラス。
 *
 * <p>COBOLプログラムで使用される ZERO, SPACE, HIGH-VALUE, LOW-VALUE, QUOTE などの
 * 表意定数をJavaの静的フィールドとして提供する。また、日本語（全角）文字用の
 * 表意定数やバッファサイズ定数なども含む。
 *
 * <p>libcob/common.hで定義されている各種定数に対応する。
 */
public class CobolConstant {
    /** ALLキーワード用の属性。英数字型で全桁に同じ値を繰り返す場合に使用。 */
    public static final CobolFieldAttribute allAttr =
            new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC_ALL, 0, 0, 0, null);

    /** 数値定数"1"用の属性。1桁の数値型。 */
    public static final CobolFieldAttribute oneAttr =
            new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC, 1, 0, 0, null);

    /** Shift-JIS全角数字「０」のバイト表現。0x82 0x4f。 */
    public static final byte[] SJZERO = {(byte) 0x82, (byte) 0x4f};

    /** Shift-JIS全角スペースのバイト表現。0x81 0x40。 */
    public static final byte[] SJSPC = {(byte) 0x81, (byte) 0x40};

    /** Shift-JIS全角ブランク（スペースと同一）のバイト表現。0x81 0x40。 */
    public static final byte[] SJBLK = {(byte) 0x81, (byte) 0x40};

    /** Shift-JIS全角引用符「"」のバイト表現。0x81 0x68。 */
    public static final byte[] SJQUOT = {(byte) 0x81, (byte) 0x68};

    /** Shift-JIS全角スラッシュ「／」のバイト表現。0x81 0x5e。 */
    public static final byte[] SJSLAS = {(byte) 0x81, (byte) 0x5e};

    /** Shift-JIS全角文字のサイズ（バイト数）。常に2。 */
    public static final int SJCSIZ = 2;

    /** 全角数字「０」のバイト表現。SJZEROへのエイリアス。 */
    public static final byte[] ZENZERO = SJZERO;

    /** 全角スペースのバイト表現。SJSPCへのエイリアス。 */
    public static final byte[] ZENSPC = SJSPC;

    /** 全角ブランクのバイト表現。SJBLKへのエイリアス。 */
    public static final byte[] ZENBLK = SJBLK;

    /** 全角引用符のバイト表現。SJQUOTへのエイリアス。 */
    public static final byte[] ZENQUOT = SJQUOT;

    /** 全角スラッシュのバイト表現。SJSLASへのエイリアス。 */
    public static final byte[] ZENSLAS = SJSLAS;

    /** 全角文字のサイズ（バイト数）。SJCSIZへのエイリアス。 */
    public static final int ZENCSIZ = SJCSIZ;

    /** 表意定数ZERO/ZEROS/ZEROES。数字の"0"を表す。 */
    public static final AbstractCobolField zero = CobolFieldFactory.makeCobolField(1, "0", allAttr);

    /** 表意定数SPACE/SPACES。半角スペース文字を表す。 */
    public static final AbstractCobolField space =
            CobolFieldFactory.makeCobolField(1, " ", allAttr);

    /** 表意定数として使用されるブランク。spaceと同一の値。 */
    public static final AbstractCobolField blank =
            CobolFieldFactory.makeCobolField(1, " ", allAttr);

    /** 表意定数HIGH-VALUE/HIGH-VALUES。最大値0xFFを表す。 */
    public static final AbstractCobolField high =
            CobolFieldFactory.makeCobolField(1, CobolConstant.get0xFFStorage(), allAttr);

    /** 表意定数LOW-VALUE/LOW-VALUES。最小値0x00（NULL文字）を表す。 */
    public static final AbstractCobolField low = CobolFieldFactory.makeCobolField(1, "\0", allAttr);

    /** 表意定数QUOTE/QUOTES。引用符（"）を表す。 */
    public static final AbstractCobolField quote =
            CobolFieldFactory.makeCobolField(1, "\"", allAttr);

    /** 数値定数1。算術演算で使用される。 */
    public static final AbstractCobolField one = CobolFieldFactory.makeCobolField(1, "1", oneAttr);

    /** 全角数字「０」を表すフィールド。日本語項目のZERO相当。 */
    public static final AbstractCobolField zenZero =
            CobolFieldFactory.makeCobolField(ZENCSIZ, new CobolDataStorage(ZENZERO), allAttr);

    /** 全角スペースを表すフィールド。日本語項目のSPACE相当。 */
    public static final AbstractCobolField zenSpace =
            CobolFieldFactory.makeCobolField(ZENCSIZ, new CobolDataStorage(ZENSPC), allAttr);

    /** 全角ブランクを表すフィールド。zenSpaceと同一の値。 */
    public static final AbstractCobolField zenBlank =
            CobolFieldFactory.makeCobolField(ZENCSIZ, new CobolDataStorage(ZENBLK), allAttr);

    /** 全角引用符を表すフィールド。日本語項目のQUOTE相当。 */
    public static final AbstractCobolField zenQuote =
            CobolFieldFactory.makeCobolField(ZENCSIZ, new CobolDataStorage(ZENQUOT), allAttr);

    private static CobolDataStorage get0xFFStorage() {
        byte[] bytes = new byte[1];
        bytes[0] = (byte) 0xff;
        return new CobolDataStorage(bytes);
    }

    /**
     * 10のべき乗テーブル（long型）。
     * インデックスnに対して10^nの値を格納。数値変換や桁数計算に使用。
     * 範囲は10^0から10^18まで。
     */
    public static final long[] exp10LL = {
        1L,
        10L,
        100L,
        1000L,
        10000L,
        100000L,
        1000000L,
        10000000L,
        100000000L,
        1000000000L,
        10000000000L,
        100000000000L,
        1000000000000L,
        10000000000000L,
        100000000000000L,
        1000000000000000L,
        10000000000000000L,
        100000000000000000L,
        1000000000000000000L
    };

    /** 最小サイズのバッファ容量（256バイト）。小規模な一時領域に使用。 */
    static final int COB_MINI_BUFF = 256;

    /** 小サイズのバッファ容量（1024バイト）。 */
    static final int COB_SMALL_BUFF = 1024;

    /** 通常サイズのバッファ容量（2048バイト）。 */
    static final int COB_NORMAL_BUFF = 2048;

    /** 中サイズのバッファ容量（8192バイト）。 */
    static final int COB_MEDIUM_BUFF = 8192;

    /** 大サイズのバッファ容量（16384バイト）。 */
    static final int COB_LARGE_BUFF = 16384;

    /** 最小バッファの最大インデックス（COB_MINI_BUFF - 1）。 */
    static final int COB_MINI_MAX = COB_MINI_BUFF - 1;

    /** 小バッファの最大インデックス（COB_SMALL_BUFF - 1）。 */
    static final int COB_SMALL_MAX = COB_SMALL_BUFF - 1;

    /** 通常バッファの最大インデックス（COB_NORMAL_BUFF - 1）。 */
    static final int COB_NORMAL_MAX = COB_NORMAL_BUFF - 1;

    /** 中バッファの最大インデックス（COB_MEDIUM_BUFF - 1）。 */
    static final int COB_MEDIUM_MAX = COB_MEDIUM_BUFF - 1;

    /** 大バッファの最大インデックス（COB_LARGE_BUFF - 1）。 */
    static final int COB_LARGE_MAX = COB_LARGE_BUFF - 1;

    /** フィールドパラメータの最大数。CALL文やINTRINSIC関数の引数上限。 */
    static final int COB_MAX_FIELD_PARAMS = 64;

    /** 致命的エラーコード: 初期化未完了。 */
    static final int COB_FERROR_INITIALIZED = 0;

    /** ソースファイル名（デバッグ用）。現在未使用。 */
    static final String COB_SOURCE_FILE = null;

    /** パッケージバージョン番号。現在未使用。 */
    static final int COB_PACKAGE_VERSION = 0;

    /** パッチレベル番号。現在未使用。 */
    static final int COB_PATCH_LEVEL = 0;

    /** 外部COBOLライブラリの検索パス。環境変数COB_LIBRARY_PATHのデフォルト値。 */
    public static final String COB_LIBRARY_PATH = "";
}

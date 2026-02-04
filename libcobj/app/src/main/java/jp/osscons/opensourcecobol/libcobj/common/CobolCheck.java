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
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolExceptionId;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;

/**
 * COBOLの実行時境界チェックを行うユーティリティクラス。
 *
 * <p>配列（テーブル）の添字範囲チェック、OCCURS DEPENDING ON句の値チェックなど、
 * COBOLプログラムの実行時に発生しうる境界違反を検出する。
 * コンパイルオプション-fchecksが指定された場合にトランスパイラが境界チェックコードを生成する。
 *
 * <p>libcob/common.cのcob_check_subscript、cob_check_odoに対応する。
 */
public class CobolCheck {
    /**
     * 配列添字の範囲チェックを行う。
     *
     * <p>添字が指定された範囲外の場合、COB_EC_BOUND_SUBSCRIPT例外を設定し、
     * エラーメッセージを出力後にプログラムを終了する。
     *
     * @param i チェック対象の添字値
     * @param min 添字の最小値（通常は1）
     * @param max 添字の最大値（OCCURS句で指定された値）
     * @param name 配列項目名（エラーメッセージ表示用、Shift-JISエンコード）
     * @param len 項目名の長さ
     * @throws CobolStopRunException 添字が範囲外の場合
     */
    public static void checkSubscript(int i, int min, int max, byte[] name, int len)
            throws CobolStopRunException {
        if (i < min || max < i) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_BOUND_SUBSCRIPT);
            CobolUtil.runtimeError(
                    String.format(
                            "Subscript of '%s' out of bounds: %d",
                            new String(name, AbstractCobolField.charSetSJIS), i));
            CobolStopRunException.stopRunAndThrow(1);
        }
    }

    /**
     * 配列添字の範囲チェックを行う（long型オーバーロード）。
     *
     * @param i チェック対象の添字値（long型）
     * @param min 添字の最小値
     * @param max 添字の最大値
     * @param name 配列項目名（Shift-JISエンコード）
     * @param len 項目名の長さ
     * @throws CobolStopRunException 添字が範囲外の場合
     * @see #checkSubscript(int, int, int, byte[], int)
     */
    public static void checkSubscript(long i, int min, int max, byte[] name, int len)
            throws CobolStopRunException {
        CobolCheck.checkSubscript((int) i, min, max, name, len);
    }

    /**
     * 配列添字の範囲チェックを行う（CobolDataStorageオーバーロード）。
     *
     * @param i チェック対象の添字値（long型）
     * @param min 添字の最小値
     * @param max 添字の最大値
     * @param name 配列項目名を格納したデータストレージ
     * @param len 項目名の長さ
     * @throws CobolStopRunException 添字が範囲外の場合
     * @see #checkSubscript(int, int, int, byte[], int)
     */
    public static void checkSubscript(long i, int min, int max, CobolDataStorage name, int len)
            throws CobolStopRunException {
        CobolCheck.checkSubscript((int) i, min, max, name.getByteArrayRef(0, len), len);
    }

    /**
     * OCCURS DEPENDING ON句で指定された変数の値をチェックする。
     *
     * <p>OCCURS DEPENDING ON句で参照される変数の値が有効範囲外の場合、
     * COB_EC_BOUND_SUBSCRIPT例外を設定し、プログラムを終了する。
     *
     * @param i チェック対象の値
     * @param min 最小出現回数（OCCURS句のFROM値）
     * @param max 最大出現回数（OCCURS句のTO値）
     * @param name DEPENDING ON変数名（エラーメッセージ表示用）
     * @throws CobolStopRunException 値が範囲外の場合
     */
    public static void checkOdo(int i, int min, int max, String name) throws CobolStopRunException {
        if (i < min || max < i) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_BOUND_SUBSCRIPT);
            CobolUtil.runtimeError(
                    String.format("OCCURS DEPENDING ON '%s' out of bounds: %d", name, i));
            CobolStopRunException.stopRunAndThrow(1);
        }
    }

    /**
     * OCCURS DEPENDING ON句で指定された変数の値をチェックする（byte配列オーバーロード）。
     *
     * @param i チェック対象の値
     * @param min 最小出現回数
     * @param max 最大出現回数
     * @param name DEPENDING ON変数名（Shift-JISエンコード）
     * @throws CobolStopRunException 値が範囲外の場合
     * @see #checkOdo(int, int, int, String)
     */
    public static void checkOdo(int i, int min, int max, byte[] name) throws CobolStopRunException {
        CobolCheck.checkOdo(i, min, max, new String(name, AbstractCobolField.charSetSJIS));
    }
}

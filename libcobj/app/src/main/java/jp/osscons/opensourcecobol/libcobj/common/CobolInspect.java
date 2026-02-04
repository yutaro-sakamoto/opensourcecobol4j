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
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolExceptionId;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;

/**
 * COBOLのINSPECT文を実装するクラス。
 *
 * <p>INSPECT文は文字列内の文字を検査・置換する機能を提供する。以下の形式をサポート：
 * <ul>
 *   <li>INSPECT TALLYING - 文字や文字列の出現回数をカウント
 *   <li>INSPECT REPLACING - 文字や文字列を別の文字に置換
 *   <li>INSPECT CONVERTING - 文字変換テーブルに基づく変換
 * </ul>
 *
 * <p>libcob/strings.cのcob_inspect_xxx関数群に対応する。使用手順：
 * <ol>
 *   <li>{@link #init(AbstractCobolField, int)}で初期化
 *   <li>{@link #start()}で検査範囲を開始
 *   <li>{@link #before(AbstractCobolField)}や{@link #after(AbstractCobolField)}で範囲を絞り込み
 *   <li>{@link #all(AbstractCobolField, AbstractCobolField)}等で検査・置換を実行
 *   <li>{@link #finish()}で処理を完了
 * </ol>
 */
public class CobolInspect {
    private static final int INSPECT_ALL = 0;
    private static final int INSPECT_LEADING = 1;
    private static final int INSPECT_FIRST = 2;
    private static final int INSPECT_TRAILING = 3;

    private static AbstractCobolField inspectVar;
    private static CobolDataStorage inspectData;
    private static int inspectStart;
    private static int inspectEnd;
    private static int[] inspectMark = null;
    private static int lastsize = 0;
    private static int inspectSize;
    private static int inspectReplacing;
    private static int inspectSign;
    private static AbstractCobolField inspectVarCopy;

    private static AbstractCobolField figurative(AbstractCobolField f1, AbstractCobolField f2) {
        int size1 = 0;
        int size2 = f2.getSize();
        byte[] figptr = new byte[size2];
        int s = 0;
        for (int n = 0; n < size2; n++, s++) {
            figptr[s] = f1.getDataStorage().getByte(size1);
            size1++;
            if (size1 >= f1.getSize()) {
                size1 = 0;
            }
        }
        return CobolFieldFactory.makeCobolField(
                size2,
                new CobolDataStorage(figptr),
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null));
    }

    private static void common(AbstractCobolField f1, AbstractCobolField f2, int type)
            throws CobolStopRunException {
        if (f1 == null) {
            f1 = CobolConstant.low;
        }
        if (f2 == null) {
            f2 = CobolConstant.low;
        }

        int type1 = f1.getAttribute().getType();
        int type2 = f2.getAttribute().getType();

        if (type2 == CobolFieldAttribute.COB_TYPE_NATIONAL
                || type2 == CobolFieldAttribute.COB_TYPE_NATIONAL_EDITED) {
            if (f1 == CobolConstant.quote) {
                f1 = CobolConstant.zenQuote;
            } else if (f1 == CobolConstant.space) {
                f1 = CobolConstant.zenSpace;
            } else if (f1 == CobolConstant.zero) {
                f1 = CobolConstant.zenZero;
            }
        }
        if (type1 == CobolFieldAttribute.COB_TYPE_NATIONAL
                || type1 == CobolFieldAttribute.COB_TYPE_NATIONAL_EDITED) {
            if (f2 == CobolConstant.quote) {
                f2 = CobolConstant.zenQuote;
            } else if (f2 == CobolConstant.space) {
                f2 = CobolConstant.zenSpace;
            } else if (f2 == CobolConstant.zero) {
                f2 = CobolConstant.zenZero;
            }
        }
        if (inspectReplacing != 0 && f1.getSize() != f2.getSize()) {
            if (type1 == CobolFieldAttribute.COB_TYPE_ALPHANUMERIC_ALL) {
                f1 = figurative(f1, f2);
            } else {
                CobolRuntimeException.setException(CobolExceptionId.COB_EC_RANGE_INSPECT_SIZE);
                return;
            }
        }

        int mark = inspectStart;
        int len = inspectEnd - inspectStart;
        int n = 0;
        if (type == INSPECT_TRAILING) {
            for (int i = len - f2.getSize(); i >= 0; i--) {
                if (inspectData.memcmp(inspectStart + i, f2.getDataStorage(), f2.getSize()) == 0) {
                    int j;
                    for (j = 0; j < f2.getSize(); ++j) {
                        if (inspectMark[mark + i + j] != -1) {
                            break;
                        }
                    }
                    if (j == f2.getSize()) {
                        CobolDataStorage f1Storage = f1.getDataStorage();
                        for (j = 0; j < f2.getSize(); ++j) {
                            inspectMark[mark + i + j] =
                                    inspectReplacing != 0 ? f1Storage.getByte(j) : 1;
                        }
                        i -= f2.getSize() - 1;
                        n++;
                    }
                } else {
                    break;
                }
            }
        } else {
            for (int i = 0; i < (len - f2.getSize() + 1); ++i) {
                if (inspectData.memcmp(inspectStart + i, f2.getDataStorage(), f2.getSize()) == 0) {
                    int j;
                    for (j = 0; j < f2.getSize(); ++j) {
                        if (inspectMark[mark + i + j] != -1) {
                            break;
                        }
                    }
                    if (j == f2.getSize()) {
                        CobolDataStorage f1Storage = f1.getDataStorage();
                        for (j = 0; j < f2.getSize(); ++j) {
                            inspectMark[mark + i + j] =
                                    inspectReplacing != 0 ? f1Storage.getByte(j) : 1;
                        }
                        i += f2.getSize() - 1;
                        n++;
                        if (type == INSPECT_FIRST) {
                            break;
                        }
                    }
                } else if (type == INSPECT_LEADING) {
                    break;
                }
            }
        }

        if (n > 0 && inspectReplacing == 0) {
            f1.addInt(n);
        }
    }

    /**
     * INSPECT文の処理を初期化する。libcob/strings.cのcob_inspect_initに対応。
     *
     * <p>検査対象のフィールドと処理モード（TALLYINGまたはREPLACING）を設定し、
     * マーク配列を初期化する。この後start()を呼び出して検査範囲を設定する。
     *
     * @param var 検査対象のCOBOLフィールド
     * @param replacing 0の場合TALLYING、非0の場合REPLACING
     */
    public static void init(AbstractCobolField var, int replacing) {
        CobolInspect.inspectVarCopy = var;
        CobolInspect.inspectVar = CobolInspect.inspectVarCopy;
        CobolInspect.inspectSign = var.getSign();

        if (var.getAttribute().isTypeNumericDisplay()) {
            inspectVar.putSign(1);
        }

        CobolInspect.inspectReplacing = replacing;
        CobolInspect.inspectSize = var.getFieldSize();
        CobolInspect.inspectData = var.getDataStorage();
        CobolInspect.inspectStart = -1;
        CobolInspect.inspectEnd = -1;
        if (inspectSize > lastsize) {
            inspectMark = new int[inspectSize];
            lastsize = inspectSize;
        }
        for (int i = 0; i < inspectSize; ++i) {
            inspectMark[i] = -1;
        }
        CobolRuntimeException.setException(0);
    }

    /**
     * INSPECT文の検査範囲を開始する。libcob/strings.cのcob_inspect_startに対応。
     *
     * <p>検査範囲をフィールド全体（先頭から末尾まで）に設定する。
     * init()の後、before()やafter()を呼び出す前にこのメソッドを呼び出す必要がある。
     */
    public static void start() {
        inspectStart = 0;
        inspectEnd = inspectSize;
    }

    /**
     * BEFORE INITIAL句を処理する。libcob/strings.cのcob_inspect_beforeに対応。
     *
     * <p>指定された文字列が最初に出現する位置より前の範囲のみを検査対象とする。
     * 「INSPECT ... BEFORE INITIAL str」に対応。
     *
     * @param str 境界となる文字列
     */
    public static void before(AbstractCobolField str) {
        CobolDataStorage p2 = null;
        int fig = 0;

        switch (str.getAttribute().getType()) {
            case CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY:
                CobolDataStorage data = str.getDataStorage();
                int firstIndex = str.getFirstDataIndex();
                int size = str.getFieldSize();
                int n = 0;
                int i = 0;
                while (size > 1
                        && (data.getByte(firstIndex + i) == (byte) '0'
                                || data.getByte(firstIndex + i) == (byte) 0x70)) {
                    size--;
                    i++;
                }
                while (size-- > 0) {
                    int b = data.getByte(firstIndex + i++);
                    n = n * 10 + (b >= 0x70 ? b - 0x70 : b - 0x30);
                    fig++;
                }
                p2 =
                        new CobolDataStorage(
                                String.format("%d", n).getBytes(AbstractCobolField.charSetSJIS));
                break;
            default:
                fig = str.getSize();
                p2 = str.getDataStorage();
                break;
        }

        for (int p = inspectStart; p < inspectEnd - fig + 1; ++p) {
            if (inspectData.getSubDataStorage(p).memcmp(p2, fig) == 0) {
                inspectEnd = p;
                break;
            }
        }
    }

    /**
     * AFTER INITIAL句を処理する。libcob/strings.cのcob_inspect_afterに対応。
     *
     * <p>指定された文字列が最初に出現する位置より後の範囲のみを検査対象とする。
     * 「INSPECT ... AFTER INITIAL str」に対応。
     *
     * @param str 境界となる文字列
     */
    public static void after(AbstractCobolField str) {
        CobolDataStorage data = str.getDataStorage();
        int size = str.getSize();
        for (int p = inspectStart; p < inspectEnd - str.getSize() + 1; ++p) {
            if (inspectData.getSubDataStorage(p).memcmp(data, size) == 0) {
                inspectStart = p + size;
                return;
            }
        }
        inspectStart = inspectEnd;
    }

    /**
     * CHARACTERS句を処理する。libcob/strings.cのcob_inspect_charactersに対応。
     *
     * <p>TALLYING時は検査範囲内の文字数をカウントし、f1に加算する。
     * REPLACING時は検査範囲内の全文字をf1の内容で置換する。
     * 「INSPECT ... TALLYING counter FOR CHARACTERS」に対応。
     *
     * @param f1 TALLYINGの場合はカウンタ、REPLACINGの場合は置換文字
     * @throws CobolStopRunException ランタイムエラー発生時
     */
    public static void characters(AbstractCobolField f1) throws CobolStopRunException {
        int mark = inspectStart;
        int len = inspectEnd - inspectStart;
        if (inspectReplacing != 0) {
            for (int i = 0; i < len; ++i) {
                if (inspectMark[mark + i] == -1) {
                    CobolDataStorage data = f1.getDataStorage();
                    for (int j = 0; j < f1.getSize(); ++j) {
                        inspectMark[mark + i + j] = data.getByte(j);
                    }
                    i += f1.getSize() - 1;
                }
            }
        } else {
            int n = 0;
            for (int i = 0; i < len; ++i) {
                if (inspectMark[mark + i] == -1) {
                    inspectMark[mark + i] = 1;
                    n++;
                }
            }
            if (n > 0) {
                int type = inspectVar.getAttribute().getType();
                if (type == CobolFieldAttribute.COB_TYPE_NATIONAL
                        || type == CobolFieldAttribute.COB_TYPE_NATIONAL_EDITED) {
                    n = n / 2;
                }
                f1.addInt(n);
            }
        }
    }

    /**
     * ALL句を処理する。libcob/strings.cのcob_inspect_allに対応。
     *
     * <p>検査範囲内のf2の全出現をカウントまたは置換する。
     * 「INSPECT ... TALLYING counter FOR ALL f2」または
     * 「INSPECT ... REPLACING ALL f2 BY f1」に対応。
     *
     * @param f1 TALLYINGの場合はカウンタ、REPLACINGの場合は置換文字列
     * @param f2 検索対象の文字列
     * @throws CobolStopRunException ランタイムエラー発生時
     */
    public static void all(AbstractCobolField f1, AbstractCobolField f2)
            throws CobolStopRunException {
        common(f1, f2, INSPECT_ALL);
    }

    /**
     * LEADING句を処理する。libcob/strings.cのcob_inspect_leadingに対応。
     *
     * <p>検査範囲の先頭から連続するf2の出現をカウントまたは置換する。
     * 先頭以外に出現するf2は無視される。
     * 「INSPECT ... TALLYING counter FOR LEADING f2」に対応。
     *
     * @param f1 TALLYINGの場合はカウンタ、REPLACINGの場合は置換文字列
     * @param f2 検索対象の文字列
     * @throws CobolStopRunException ランタイムエラー発生時
     */
    public static void leading(AbstractCobolField f1, AbstractCobolField f2)
            throws CobolStopRunException {
        common(f1, f2, INSPECT_LEADING);
    }

    /**
     * FIRST句を処理する。libcob/strings.cのcob_inspect_firstに対応。
     *
     * <p>検査範囲内で最初に出現するf2のみをカウントまたは置換する。
     * 「INSPECT ... REPLACING FIRST f2 BY f1」に対応。
     *
     * @param f1 TALLYINGの場合はカウンタ、REPLACINGの場合は置換文字列
     * @param f2 検索対象の文字列
     * @throws CobolStopRunException ランタイムエラー発生時
     */
    public static void first(AbstractCobolField f1, AbstractCobolField f2)
            throws CobolStopRunException {
        common(f1, f2, INSPECT_FIRST);
    }

    /**
     * TRAILING句を処理する。libcob/strings.cのcob_inspect_trailingに対応。
     *
     * <p>検査範囲の末尾から連続するf2の出現をカウントまたは置換する。
     * 末尾以外に出現するf2は無視される。
     * 「INSPECT ... TALLYING counter FOR TRAILING f2」に対応。
     *
     * @param f1 TALLYINGの場合はカウンタ、REPLACINGの場合は置換文字列
     * @param f2 検索対象の文字列
     * @throws CobolStopRunException ランタイムエラー発生時
     */
    public static void trailing(AbstractCobolField f1, AbstractCobolField f2)
            throws CobolStopRunException {
        common(f1, f2, INSPECT_TRAILING);
    }

    /**
     * CONVERTING句を処理する。libcob/strings.cのcob_inspect_convertingに対応。
     *
     * <p>f1に含まれる各文字を、f2の対応する位置の文字に変換する。
     * 「INSPECT ... CONVERTING f1 TO f2」に対応。
     *
     * @param f1 変換元の文字セット
     * @param f2 変換先の文字セット（f1と同じ長さであること）
     */
    public static void converting(AbstractCobolField f1, AbstractCobolField f2) {
        int type1 = f1.getAttribute().getType();
        int len = inspectEnd - inspectStart;
        CobolDataStorage data = f2.getDataStorage();
        if (type1 == CobolFieldAttribute.COB_TYPE_NATIONAL
                || type1 == CobolFieldAttribute.COB_TYPE_NATIONAL_EDITED) {
            if (f2 == CobolConstant.quote) {
                f2 = CobolConstant.zenQuote;
            } else if (f2 == CobolConstant.space) {
                f2 = CobolConstant.zenSpace;
            } else if (f2 == CobolConstant.zero) {
                f2 = CobolConstant.zenZero;
            }
            for (int j = 0; j < f1.getSize(); j += 2) {
                for (int i = 0; i < len; i += 2) {
                    if (inspectMark[i] == -1
                            && inspectMark[i + 1] == -1
                            && f1.getDataStorage()
                                            .getSubDataStorage(j)
                                            .memcmp(
                                                    inspectData.getSubDataStorage(inspectStart + i),
                                                    2)
                                    == 0) {
                        inspectData.setByte(inspectStart + i, data.getByte(0));
                        inspectData.setByte(inspectStart + i + 1, data.getByte(1));
                    }
                    inspectMark[i] = 1;
                    inspectMark[i + 1] = 1;
                }
            }
        } else {
            for (int j = 0; j < f1.getSize(); ++j) {
                for (int i = 0; i < len; ++i) {
                    if (inspectMark[i] == -1
                            && inspectData.getByte(inspectStart + i)
                                    == f1.getDataStorage().getByte(j)) {
                        if (f2 == CobolConstant.quote
                                || f2 == CobolConstant.space
                                || f2 == CobolConstant.zero) {
                            inspectData.setByte(inspectStart + i, data.getByte(0));
                        } else {
                            inspectData.setByte(inspectStart + i, data.getByte(j));
                        }
                        inspectMark[i] = 1;
                    }
                }
            }
        }
    }

    /**
     * INSPECT文の処理を完了する。libcob/strings.cのcob_inspect_finishに対応。
     *
     * <p>REPLACINGモードの場合、マーク配列に記録された置換情報を実際のデータに反映する。
     * また、数値表示項目の符号を復元する。
     */
    public static void finish() {
        if (inspectReplacing != 0) {
            for (int i = 0; i < inspectSize; ++i) {
                if (inspectMark[i] != -1) {
                    inspectData.setByte(i, (byte) inspectMark[i]);
                }
            }
        }
        inspectVar.putSign(inspectSign);
    }

    /**
     * INSPECT用の内部バッファを初期化する。
     *
     * <p>ランタイム初期化時にCobolUtil.cob_initから呼び出される。
     * マーク配列を中サイズバッファ（COB_MEDIUM_BUFF）で初期化する。
     */
    public static void initString() {
        CobolInspect.inspectMark = new int[CobolConstant.COB_MEDIUM_BUFF];
        lastsize = CobolConstant.COB_MEDIUM_BUFF;
    }
}

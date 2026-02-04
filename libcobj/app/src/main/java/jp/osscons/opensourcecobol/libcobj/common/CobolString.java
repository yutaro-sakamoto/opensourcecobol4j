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
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolExceptionInfo;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;

/**
 * UNSTRING文のデリミタ情報を保持する内部クラス。
 */
class Dlm {
    /** デリミタとして使用するフィールド */
    public AbstractCobolField dlm;

    /** ALLキーワードの有無。非0の場合、連続するデリミタを1つとして扱う */
    public int all;
}

/**
 * COBOLのSTRING文およびUNSTRING文を実装するクラス。
 *
 * <p>STRING文は複数の文字列を連結し、UNSTRING文は文字列をデリミタで分割する。
 *
 * <p>STRING文の使用例（COBOL）：
 * <pre>
 *     STRING WS-FIRST DELIMITED BY SPACE
 *            "-" DELIMITED BY SIZE
 *            WS-LAST DELIMITED BY SPACE
 *         INTO WS-RESULT
 *         WITH POINTER WS-PTR.
 * </pre>
 *
 * <p>UNSTRING文の使用例（COBOL）：
 * <pre>
 *     UNSTRING WS-INPUT DELIMITED BY "," OR SPACE
 *         INTO WS-FIELD1 DELIMITER IN WS-DLM1 COUNT IN WS-CNT1
 *              WS-FIELD2 DELIMITER IN WS-DLM2 COUNT IN WS-CNT2
 *         WITH POINTER WS-PTR
 *         TALLYING IN WS-TALLY.
 * </pre>
 *
 * <p>libcob/strings.cのcob_string_xxx、cob_unstring_xxx関数群に対応する。
 */
public class CobolString {
    private static final int DLM_DEFAULT_NUM = 8;

    private static int udlmCount = 0;

    private static AbstractCobolField stringDst;
    private static AbstractCobolField stringPtr;
    private static AbstractCobolField stringDlm;
    private static AbstractCobolField stringDstCopy;
    private static AbstractCobolField stringPtrCopy;
    private static AbstractCobolField stringDlmCopy;
    private static int stringOffset;

    private static Dlm[] dlmList;
    private static AbstractCobolField unstringSrc;
    private static AbstractCobolField unstringPtr;
    private static AbstractCobolField unstringSrcCopy;
    private static AbstractCobolField unstringPtrCopy;
    private static int unstringOffset;
    private static int unstringCount;
    private static int unstringNdlms;

    /**
     * STRING文の処理を初期化する（dstがnull）。
     *
     * @param dst 未使用（nullとして処理）
     * @param ptr ポインタフィールド（WITH POINTER句）
     */
    public static void stringInit(int dst, AbstractCobolField ptr) {
        stringInit(null, ptr);
    }

    /**
     * STRING文の処理を初期化する（ptrがnull）。
     *
     * @param dst 結果を格納するフィールド（INTO句）
     * @param ptr 未使用（nullとして処理）
     */
    public static void stringInit(AbstractCobolField dst, int ptr) {
        stringInit(dst, null);
    }

    /**
     * STRING文の処理を初期化する（両方null）。
     *
     * @param dst 未使用（nullとして処理）
     * @param ptr 未使用（nullとして処理）
     */
    public static void stringInit(int dst, int ptr) {
        stringInit(null, null);
    }

    /**
     * STRING文の処理を初期化する。libcob/strings.cのcob_string_initに対応。
     *
     * <p>結果格納フィールドとポインタを設定し、初期位置を決定する。
     * ポインタが範囲外の場合、COB_EC_OVERFLOW_STRING例外を設定する。
     *
     * @param dst 結果を格納するフィールド（INTO句）。nullの場合エラー
     * @param ptr 現在位置を保持するフィールド（WITH POINTER句）。nullの場合は先頭から開始
     */
    public static void stringInit(AbstractCobolField dst, AbstractCobolField ptr) {
        stringDstCopy = dst;
        stringDst = stringDstCopy;
        stringPtr = null;
        if (ptr != null) {
            stringPtrCopy = ptr;
            stringPtr = stringPtrCopy;
        }
        stringOffset = 0;
        CobolRuntimeException.setException(0);

        if (stringPtr != null) {
            stringOffset = stringPtr.getInt() - 1;
            if (stringOffset < 0 || stringOffset >= stringDst.getSize()) {
                CobolRuntimeException.setException(CobolExceptionId.COB_EC_OVERFLOW_STRING);
            }
        }

        switch (stringDst.getAttribute().getType()) {
            case CobolFieldAttribute.COB_TYPE_NATIONAL:
            case CobolFieldAttribute.COB_TYPE_NATIONAL_EDITED:
                stringOffset *= 2;
                break;
            default:
                break;
        }
    }

    /**
     * STRING文のデリミタを設定する（デリミタなし）。
     *
     * @param dlm 未使用（nullとして処理、DELIMITED BY SIZE相当）
     */
    public static void stringDelimited(int dlm) {
        stringDelimited(null);
    }

    /**
     * STRING文のデリミタを設定する。libcob/strings.cのcob_string_delimitedに対応。
     *
     * <p>後続のstringAppendで追加されるソースに対するデリミタを設定する。
     * nullの場合はDELIMITED BY SIZE（全体を使用）として扱う。
     *
     * @param dlm デリミタ文字列（DELIMITED BY句）。nullの場合SIZE指定
     */
    public static void stringDelimited(AbstractCobolField dlm) {
        switch (stringDst.getAttribute().getType()) {
            case CobolFieldAttribute.COB_TYPE_NATIONAL:
            case CobolFieldAttribute.COB_TYPE_NATIONAL_EDITED:
                if (dlm == CobolConstant.quote) {
                    dlm = CobolConstant.zenQuote;
                } else if (dlm == CobolConstant.space) {
                    dlm = CobolConstant.zenSpace;
                } else if (dlm == CobolConstant.zero) {
                    dlm = CobolConstant.zenZero;
                }
                break;
            default:
                break;
        }
        stringDlm = null;
        if (dlm != null) {
            stringDlmCopy = dlm;
            stringDlm = stringDlmCopy;
        }
    }

    /**
     * STRING文でソース文字列を追加する（ソースがnull）。
     *
     * @param src 未使用（nullとして処理）
     */
    public static void stringAppend(int src) {
        stringAppend(null);
    }

    /**
     * STRING文でソース文字列を追加する。libcob/strings.cのcob_string_appendに対応。
     *
     * <p>事前にstringDelimitedで設定されたデリミタに従って、ソース文字列を
     * 結果フィールドに追加する。結果フィールドがオーバーフローした場合は
     * COB_EC_OVERFLOW_STRING例外を設定する。
     *
     * @param src 追加するソース文字列
     */
    public static void stringAppend(AbstractCobolField src) {
        if (CobolRuntimeException.code != 0) {
            return;
        }

        int srcSize = src.getSize();
        if (stringDlm != null) {
            int size = srcSize - stringDlm.getSize() + 1;
            CobolDataStorage srcData = src.getDataStorage();
            CobolDataStorage dlmData = stringDlm.getDataStorage();
            int dlmSize = stringDlm.getSize();
            for (int i = 0; i < size; ++i) {
                if (srcData.getSubDataStorage(i).memcmp(dlmData, dlmSize) == 0) {
                    srcSize = i;
                    break;
                }
            }
        }

        if (srcSize <= stringDst.getSize() - stringOffset) {
            stringDst
                    .getDataStorage()
                    .getSubDataStorage(stringOffset)
                    .memcpy(src.getDataStorage(), srcSize);
            stringOffset += srcSize;
        } else {
            int size = stringDst.getSize() - stringOffset;
            stringDst
                    .getDataStorage()
                    .getSubDataStorage(stringOffset)
                    .memcpy(src.getDataStorage(), size);
            stringOffset += size;
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_OVERFLOW_STRING);
        }
    }

    /**
     * STRING文の処理を完了する。libcob/strings.cのcob_string_finishに対応。
     *
     * <p>ポインタフィールドが指定されている場合、最終位置（+1）をポインタに格納する。
     * 日本語項目の場合はバイト位置を文字位置に変換する。
     */
    public static void stringFinish() {
        int type = stringDst.getAttribute().getType();
        if (type == CobolFieldAttribute.COB_TYPE_NATIONAL
                || type == CobolFieldAttribute.COB_TYPE_NATIONAL_EDITED) {

            stringOffset /= 2;
        }
        if (stringPtr != null) {
            stringPtr.setInt(stringOffset + 1);
        }
    }

    /**
     * UNSTRING文の処理を初期化する（両方null）。
     *
     * @param src 未使用（nullとして処理）
     * @param ptr 未使用（nullとして処理）
     * @param numDlm デリミタの数
     */
    public static void unstringInit(int src, int ptr, int numDlm) {
        unstringInit(null, null, numDlm);
    }

    /**
     * UNSTRING文の処理を初期化する（ptrがnull）。
     *
     * @param src ソース文字列
     * @param ptr 未使用（nullとして処理）
     * @param numDlm デリミタの数
     */
    public static void unstringInit(AbstractCobolField src, int ptr, int numDlm) {
        unstringInit(src, null, numDlm);
    }

    /**
     * UNSTRING文の処理を初期化する（srcがnull）。
     *
     * @param src 未使用（nullとして処理）
     * @param ptr ポインタフィールド
     * @param numDlm デリミタの数
     */
    public static void unstringInit(int src, AbstractCobolField ptr, int numDlm) {
        unstringInit(null, ptr, numDlm);
    }

    /**
     * UNSTRING文の処理を初期化する。libcob/strings.cのcob_unstring_initに対応。
     *
     * <p>ソース文字列、ポインタ、デリミタリストを初期化する。
     * ポインタが範囲外の場合、COB_EC_OVERFLOW_UNSTRING例外を設定する。
     *
     * @param src 分割対象のソース文字列
     * @param ptr 現在位置を保持するフィールド（WITH POINTER句）。nullの場合は先頭から開始
     * @param numDlm DELIMITED BY句で指定されるデリミタの数
     */
    public static void unstringInit(AbstractCobolField src, AbstractCobolField ptr, int numDlm) {
        unstringSrcCopy = src;
        unstringSrc = unstringSrcCopy;
        unstringPtr = null;
        if (ptr != null) {
            unstringPtrCopy = ptr;
            unstringPtr = unstringPtrCopy;
        }

        unstringOffset = 0;
        unstringCount = 0;
        unstringNdlms = 0;
        CobolRuntimeException.setException(0);

        if (dlmList == null) {
            if (numDlm <= DLM_DEFAULT_NUM) {
                dlmList = new Dlm[DLM_DEFAULT_NUM];
                udlmCount = DLM_DEFAULT_NUM;
            } else {
                dlmList = new Dlm[numDlm];
                udlmCount = numDlm;
            }
        } else {
            if (numDlm > udlmCount) {
                dlmList = new Dlm[numDlm];
                udlmCount = numDlm;
            }
        }

        for (int i = 0; i < dlmList.length; ++i) {
            dlmList[i] = new Dlm();
        }

        if (unstringPtr != null) {
            unstringOffset = unstringPtr.getInt() - 1;
            if (unstringOffset < 0 || unstringOffset >= unstringSrc.getSize()) {
                CobolRuntimeException.setException(CobolExceptionId.COB_EC_OVERFLOW_UNSTRING);
            }
        }

        switch (unstringSrc.getAttribute().getType()) {
            case CobolFieldAttribute.COB_TYPE_NATIONAL:
            case CobolFieldAttribute.COB_TYPE_NATIONAL_EDITED:
                unstringOffset *= 2;
                break;
            default:
                break;
        }
    }

    /**
     * UNSTRING文のデリミタを追加する（デリミタなし）。
     *
     * @param dlm 未使用（nullとして処理）
     * @param all ALLキーワードの有無（非0でALL指定）
     */
    public static void unstringDelimited(int dlm, int all) {
        unstringDelimited(null, all);
    }

    /**
     * UNSTRING文のデリミタを追加する。libcob/strings.cのcob_unstring_delimitedに対応。
     *
     * <p>DELIMITED BY句で指定されたデリミタをリストに追加する。
     * 複数のデリミタを「OR」で指定した場合は、このメソッドが複数回呼び出される。
     *
     * @param dlm デリミタ文字列（DELIMITED BY句）
     * @param all ALLキーワードの有無。非0の場合、連続するデリミタを1つとして扱う
     */
    public static void unstringDelimited(AbstractCobolField dlm, int all) {
        AbstractCobolField addDlm = null;

        switch (unstringSrc.getAttribute().getType()) {
            case CobolFieldAttribute.COB_TYPE_NATIONAL:
            case CobolFieldAttribute.COB_TYPE_NATIONAL_EDITED:
                if (dlm == CobolConstant.quote) {
                    dlm = CobolConstant.zenQuote;
                } else if (dlm == CobolConstant.space) {
                    dlm = CobolConstant.zenSpace;
                    addDlm = CobolConstant.zenBlank;
                } else if (dlm == CobolConstant.zero) {
                    dlm = CobolConstant.zenZero;
                }
                break;
            default:
                break;
        }

        dlmList[unstringNdlms].dlm = dlm;
        dlmList[unstringNdlms].all = all;
        unstringNdlms++;

        if (addDlm != null) {
            dlmList[unstringNdlms].dlm = addDlm;
            dlmList[unstringNdlms].all = all;
            unstringNdlms++;
        }
    }

    /**
     * UNSTRING文で分割結果を受け取る（cntがnull）。
     *
     * @param dst 結果格納フィールド（INTO句）
     * @param dlm デリミタ格納フィールド（DELIMITER IN句）
     * @param cnt 未使用（nullとして処理）
     */
    public static void unstringInto(AbstractCobolField dst, AbstractCobolField dlm, int cnt) {
        unstringInto(dst, dlm, null);
    }

    /**
     * UNSTRING文で分割結果を受け取る（dlmがnull）。
     *
     * @param dst 結果格納フィールド（INTO句）
     * @param dlm 未使用（nullとして処理）
     * @param cnt 文字数格納フィールド（COUNT IN句）
     */
    public static void unstringInto(AbstractCobolField dst, int dlm, AbstractCobolField cnt) {
        unstringInto(dst, null, cnt);
    }

    /**
     * UNSTRING文で分割結果を受け取る（dlm, cntがnull）。
     *
     * @param dst 結果格納フィールド（INTO句）
     * @param dlm 未使用（nullとして処理）
     * @param cnt 未使用（nullとして処理）
     */
    public static void unstringInto(AbstractCobolField dst, int dlm, int cnt) {
        unstringInto(dst, null, null);
    }

    /**
     * UNSTRING文で分割結果を受け取る（全てnull）。
     *
     * @param dst 未使用（nullとして処理）
     * @param dlm 未使用（nullとして処理）
     * @param cnt 未使用（nullとして処理）
     */
    public static void unstringInto(int dst, int dlm, int cnt) {
        unstringInto(null, null, null);
    }

    /**
     * UNSTRING文で分割結果を受け取る（dst, cntがnull）。
     *
     * @param dst 未使用（nullとして処理）
     * @param dlm デリミタ格納フィールド（DELIMITER IN句）
     * @param cnt 未使用（nullとして処理）
     */
    public static void unstringInto(int dst, AbstractCobolField dlm, int cnt) {
        unstringInto(null, dlm, null);
    }

    /**
     * UNSTRING文で分割結果を受け取る（dst, dlmがnull）。
     *
     * @param dst 未使用（nullとして処理）
     * @param dlm 未使用（nullとして処理）
     * @param cnt 文字数格納フィールド（COUNT IN句）
     */
    public static void unstringInto(int dst, int dlm, AbstractCobolField cnt) {
        unstringInto(null, null, cnt);
    }

    /**
     * UNSTRING文で分割結果を受け取る（dstがnull）。
     *
     * @param dst 未使用（nullとして処理）
     * @param dlm デリミタ格納フィールド（DELIMITER IN句）
     * @param cnt 文字数格納フィールド（COUNT IN句）
     */
    public static void unstringInto(int dst, AbstractCobolField dlm, AbstractCobolField cnt) {
        unstringInto(null, dlm, cnt);
    }

    /**
     * UNSTRING文で分割結果を受け取る。libcob/strings.cのcob_unstring_intoに対応。
     *
     * <p>ソース文字列から次のデリミタまでの部分を抽出し、dstに格納する。
     * dlmが指定されている場合は実際に検出されたデリミタを格納する。
     * cntが指定されている場合は抽出された文字数を格納する。
     *
     * @param dst 結果格納フィールド（INTO句）
     * @param dlm デリミタ格納フィールド（DELIMITER IN句）。nullの場合は格納しない
     * @param cnt 文字数格納フィールド（COUNT IN句）。nullの場合は格納しない
     */
    public static void unstringInto(
            AbstractCobolField dst, AbstractCobolField dlm, AbstractCobolField cnt) {
        if (CobolExceptionInfo.code != 0) {
            return;
        }
        if (unstringOffset >= unstringSrc.getSize()) {
            return;
        }

        CobolDataStorage srcData = unstringSrc.getDataStorage();
        int start = unstringOffset;
        int matchSize = 0;
        int dlmSize = 0;
        CobolDataStorage dlmData = null;
        if (unstringNdlms == 0) {
            matchSize = Math.min(dst.getFieldSize(), unstringSrc.getSize() - unstringOffset);
            dst.moveFrom(
                    CobolFieldFactory.makeCobolField(
                            matchSize,
                            srcData.getSubDataStorage(unstringOffset),
                            new CobolFieldAttribute(
                                    CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null)));
            unstringOffset += matchSize;
        } else {
            int srSize = unstringSrc.getSize();
            int s = srSize;
            boolean brkpt = false;
            for (int p = start; p < s; ++p) {
                for (int i = 0; i < unstringNdlms; ++i) {
                    int dlsize = dlmList[i].dlm.getSize();
                    CobolDataStorage dp = dlmList[i].dlm.getDataStorage();
                    if (p + dlsize > s) {
                        continue;
                    }
                    if (srcData.getSubDataStorage(p).memcmp(dp, dlsize) == 0) {
                        matchSize = (p - start);
                        dst.moveFrom(
                                CobolFieldFactory.makeCobolField(
                                        matchSize,
                                        srcData.getSubDataStorage(start),
                                        new CobolFieldAttribute(
                                                CobolFieldAttribute.COB_TYPE_ALPHANUMERIC,
                                                0,
                                                0,
                                                0,
                                                null)));
                        unstringOffset += matchSize + dlsize;
                        dlmData = dp;
                        dlmSize = dlsize;
                        if (dlmList[i].all != 0) {
                            for (p += dlsize; p < s; p += dlsize) {
                                if (p + dlsize > s) {
                                    break;
                                }
                                if (srcData.getSubDataStorage(p).memcmp(dp, dlsize) != 0) {
                                    break;
                                }
                                unstringOffset += dlsize;
                            }
                        }
                        brkpt = true;
                        break;
                    }
                }
                switch (unstringSrc.getAttribute().getType()) {
                    case CobolFieldAttribute.COB_TYPE_NATIONAL:
                    case CobolFieldAttribute.COB_TYPE_NATIONAL_EDITED:
                        p++;
                        break;
                    default:
                        break;
                }

                if (brkpt) {
                    break;
                }
            }
            if (!brkpt) {
                matchSize = unstringSrc.getSize() - unstringOffset;
                dst.moveFrom(
                        CobolFieldFactory.makeCobolField(
                                matchSize,
                                srcData.getSubDataStorage(start),
                                new CobolFieldAttribute(
                                        CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null)));
                unstringOffset = unstringSrc.getSize();
                dlmData = null;
            }
        }
        unstringCount++;

        if (dlm != null) {
            if (dlmData != null) {
                dlm.moveFrom(
                        CobolFieldFactory.makeCobolField(
                                dlmSize,
                                dlmData,
                                new CobolFieldAttribute(
                                        CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null)));
            } else if (dlm.getAttribute().isTypeNumeric()) {
                dlm.moveFrom(CobolConstant.zero);
            } else {
                dlm.moveFrom(CobolConstant.space);
            }
        }

        switch (unstringSrc.getAttribute().getType()) {
            case CobolFieldAttribute.COB_TYPE_NATIONAL:
            case CobolFieldAttribute.COB_TYPE_NATIONAL_EDITED:
                matchSize /= 2;
                break;
            default:
                break;
        }

        if (cnt != null) {
            cnt.setInt(matchSize);
        }
    }

    /**
     * UNSTRING文のTALLYING句を処理する（フィールドがnull）。
     *
     * @param f 未使用
     * @throws CobolStopRunException ランタイムエラー発生時
     */
    public static void unstringTallying(int f) throws CobolStopRunException {}

    /**
     * UNSTRING文のTALLYING句を処理する。libcob/strings.cのcob_unstring_tallyingに対応。
     *
     * <p>UNSTRING文で分割された回数（INTO句の実行回数）をフィールドに加算する。
     * 「UNSTRING ... TALLYING IN tally-field」に対応。
     *
     * @param f 分割回数を加算するフィールド（TALLYING IN句）
     * @throws CobolStopRunException ランタイムエラー発生時
     */
    public static void unstringTallying(AbstractCobolField f) throws CobolStopRunException {
        f.addInt(unstringCount);
    }

    /**
     * UNSTRING文の処理を完了する。libcob/strings.cのcob_unstring_finishに対応。
     *
     * <p>ソース文字列に未処理部分が残っている場合、COB_EC_OVERFLOW_UNSTRING例外を設定する。
     * ポインタフィールドが指定されている場合、最終位置（+1）をポインタに格納する。
     */
    public static void unstringFinish() {
        if (unstringOffset < unstringSrc.getSize()) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_OVERFLOW_UNSTRING);
        }

        switch (unstringSrc.getAttribute().getType()) {
            case CobolFieldAttribute.COB_TYPE_NATIONAL:
            case CobolFieldAttribute.COB_TYPE_NATIONAL_EDITED:
                unstringOffset /= 2;
                break;
            default:
                break;
        }

        if (unstringPtr != null) {
            unstringPtr.setInt(unstringOffset + 1);
        }
    }
}

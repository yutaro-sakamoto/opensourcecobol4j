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

import java.util.ArrayList;
import java.util.List;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/**
 * COBOLモジュール（プログラム）の実行時情報を管理するクラス。
 * libcob/common.hのcob_moduleに対応する。
 * モジュールスタックを使用してCALL/GO BACK時のコンテキスト管理を行う。
 */
public class CobolModule {

    /** モジュール呼び出しスタック。CALL文でプッシュ、GO BACK/EXIT PROGRAMでポップされる。 */
    private static List<CobolModule> moduleStack = new ArrayList<CobolModule>();

    /** 現在実行中のモジュール */
    private static CobolModule currentModule;

    /**
     * 現在実行中のモジュールを取得する
     *
     * @return 現在のモジュール。モジュールスタックが空の場合はnull。
     */
    public static CobolModule getCurrentModule() {
        return currentModule;
    }

    /**
     * モジュールスタックに追加する
     *
     * @param module キューに追加するモジュール
     */
    public static void push(CobolModule module) {
        currentModule = module;
        moduleStack.add(module);
    }

    /** モジュールスタックからモジュールを取り除く */
    public static void pop() {
        currentModule = moduleStack.remove(moduleStack.size() - 1);
    }

    /**
     * 呼び出し元プログラム名を取得する（COBOL CALLED-BY機能）
     *
     * @param data 結果格納用（未使用、互換性のため保持）
     * @return 成功時は1、呼び出し元がない場合は0、エラー時は-1
     */
    public static int calledBy(CobolDataStorage data) {
        AbstractCobolField param = CobolModule.getCurrentModule().cob_procedure_parameters.get(0);
        if (param != null) {
            if (moduleStack.size() >= 2) {
                String calledProgramName = moduleStack.get(moduleStack.size() - 2).program_id;
                if (calledProgramName == null) {
                    return -1;
                }
                int length = Math.min(param.getSize(), calledProgramName.length());
                param.getDataStorage().memcpy(calledProgramName, length);
            } else {
                param.getDataStorage().memset(' ', param.getSize());
                return 0;
            }
        }
        return 1;
    }

    /**
     * モジュールスタックが空かどうかを判定する
     *
     * @return スタックが空の場合true、そうでなければfalse
     */
    public static boolean isQueueEmpty() {
        return moduleStack.isEmpty();
    }

    // private CobolModule next;
    /** 照合順序（COLLATING SEQUENCE）を定義するバイト配列。nullの場合はデフォルト（バイナリ順）。 */
    public CobolDataStorage collating_sequence;

    // private AbstractCobolField cut_status;
    // private AbstractCobolField cursor_pos;
    /** DISPLAY文での符号表示形式。0:ASCII形式、非0:EBCDIC形式 */
    public int display_sign;

    /** 小数点文字。デフォルトは'.' */
    public char decimal_point;

    /** 通貨記号。デフォルトは'$' */
    public char currency_symbol;

    // private char numeric_separator;
    /** ファイル名マッピングフラグ */
    public int flag_filename_mapping;

    /** バイナリ切り捨てフラグ。バイナリ演算時の桁あふれ処理を制御する。 */
    public int flag_binary_truncate;

    /** DISPLAY文の整形出力フラグ */
    public int flag_pretty_display;

    // private int spare8;
    /** プログラムID（PROGRAM-ID句で指定された名前） */
    private String program_id;

    // private String packageName;

    /** CALL文の引数リスト。PROCEDURE DIVISION USINGで受け取るパラメータ。 */
    public List<AbstractCobolField> cob_procedure_parameters;

    /**
     * コンストラクタ
     *
     * @param next 次のモジュール（未使用、互換性のため保持）
     * @param collatingSequence 照合順序を定義するデータ
     * @param cutStatus CUTステータス（未使用）
     * @param cursorPos カーソル位置（未使用）
     * @param displaySign DISPLAY文での符号表示形式
     * @param decimalPoint 小数点文字
     * @param currencySymbol 通貨記号
     * @param numericSeparator 数値区切り文字（未使用）
     * @param flagFilenameMapping ファイル名マッピングフラグ
     * @param flagBinaryTruncate バイナリ切り捨てフラグ
     * @param flagPrettyDisplay DISPLAY整形出力フラグ
     * @param spare8 予備（未使用）
     * @param programId プログラムID
     */
    public CobolModule(
            CobolModule next,
            CobolDataStorage collatingSequence,
            AbstractCobolField cutStatus,
            AbstractCobolField cursorPos,
            int displaySign,
            char decimalPoint,
            char currencySymbol,
            char numericSeparator,
            int flagFilenameMapping,
            int flagBinaryTruncate,
            int flagPrettyDisplay,
            int spare8,
            String programId) {
        // this.next = next;
        this.collating_sequence = collatingSequence;
        // this.cut_status = cutStatus;
        // this.cursor_pos = cursorPos;
        this.display_sign = displaySign;
        this.decimal_point = decimalPoint;
        this.currency_symbol = currencySymbol;
        // this.numeric_separator = numericSeparator;
        this.flag_filename_mapping = flagFilenameMapping;
        this.flag_binary_truncate = flagBinaryTruncate;
        this.flag_pretty_display = flagPrettyDisplay;
        // this.spare8 = spare8;
        this.program_id = programId;

        this.cob_procedure_parameters = new ArrayList<AbstractCobolField>();
    }

    /**
     * プログラムIDを設定する
     *
     * @param programName 設定するプログラム名
     */
    public void setProgramId(String programName) {
        if (this.program_id != null) {
            this.program_id = null;
        }
        this.program_id = programName;
    }

    /**
     * CALL文のパラメータを設定する
     *
     * @param field CALL文で渡すパラメータ（可変長引数）
     */
    public void setParameters(AbstractCobolField... field) {
        cob_procedure_parameters.clear();
        for (AbstractCobolField f : field) {
            cob_procedure_parameters.add(f);
        }
    }

    /**
     * 現在のモジュールの小数点文字を取得する
     *
     * @return 小数点文字のASCIIコード値
     */
    public static int getDecimalPoint() {
        return currentModule.decimal_point;
    }
}

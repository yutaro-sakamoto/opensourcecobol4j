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

import java.time.DateTimeException;
import java.time.LocalDateTime;
import java.util.Calendar;
import java.util.Properties;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolExceptionId;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolExceptionInfo;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;
import jp.osscons.opensourcecobol.libcobj.file.CobolFile;

/**
 * COBOLランタイムのユーティリティ関数群を提供するクラス。
 *
 * <p>環境変数の取得・設定、実行時初期化、日時処理、エラーハンドリング、
 * 文字列操作など、COBOLプログラムの実行に必要な様々なユーティリティを提供する。
 *
 * <p>libcob/common.cの各種関数に対応する。プログラムの起動時に{@link #cob_init(String[], boolean)}を
 * 呼び出して初期化する必要がある。
 */
public class CobolUtil {
    /** IO処理でREWRITEを仮定するフラグ（環境変数COB_IO_ASSUME_REWRITEで制御） */
    private static boolean cob_io_assume_rewrite = false;

    /** 詳細出力モードフラグ（環境変数COB_VERBOSEで制御） */
    private static boolean cob_verbose = false;

    /** ランタイムエラーハンドラのリスト */
    private static HandlerList hdlrs = null;

    /** ランタイムエラーメッセージ */
    private static String runtime_err_str = null;

    /** 環境変数COB_DATEで設定されたローカル日時。nullの場合はシステム日時を使用。 */
    public static LocalDateTime cobLocalTm = null;

    /** 現在のローカル環境名（未使用） */
    public static String cobLocalEnv = null;

    /** コマンドライン引数。mainメソッドのargs配列がcob_initで設定される。 */
    public static String[] commandLineArgs = null;

    /** ACCEPT文でのコマンドライン引数の現在位置（1から開始）。 */
    public static int currentArgIndex = 1;

    /** 符号なしPACKED-DECIMALの符号ニブルにCを使用するフラグ（環境変数COB_NIBBLE_C_UNSIGNEDで制御）。 */
    public static boolean nibbleCForUnsigned = false;

    /** COBOLスイッチ（SW-1〜SW-8）の状態。インデックス0がSW-1に対応。 */
    public static boolean[] cobSwitch = new boolean[8];

    /** 保存されたCALLパラメータ数 */
    public static int cobSaveCallParams = 0;

    /** 詳細出力モードフラグ（公開用） */
    public static boolean verbose = false;

    /** 終了時エラーフラグ */
    public static boolean cobErrorOnExitFlag = false;

    /** カレンダーインスタンス。日時操作に使用。 */
    public static Calendar cal;

    /** 順編成ファイルの書き込みバッファサイズ（環境変数COB_FILE_SEQ_WRITE_BUFFER_SIZEで制御）。 */
    public static int fileSeqWriteBufferSize = 10;

    /** DISPLAY/ACCEPT文によるデータ出力時のエンコーディング */
    public static CobolEncoding terminalEncoding = CobolEncoding.SHIFT_JIS;

    /** 行トレースフラグ。READY TRACEで有効化、RESET TRACEで無効化。 */
    private static boolean lineTrace = false;

    /** 現在実行中のソースファイル名（デバッグ用） */
    private static String sourceFile;

    /** 現在実行中のソース行番号（デバッグ用） */
    private static int sourceLine;

    /** 現在実行中のプログラムID */
    private static String currProgramId;

    /** 現在実行中のセクション名 */
    private static String currSection;

    /** 現在実行中の段落名 */
    private static String currParagraph;

    /** 現在実行中のCOBOL文の内容 */
    private static String sourceStatement;

    abstract static class HandlerList {
        public HandlerList next = null;

        public abstract int proc(String s);
    }

    /** 致命的エラーコード: ランタイム未初期化 */
    public static final int FERROR_INITIALIZED = 0;

    /** 致命的エラーコード: コード生成エラー */
    public static final int FERROR_CODEGEN = 1;

    /** 致命的エラーコード: CHAINプログラムの再帰呼び出し */
    public static final int FERROR_CHAINING = 2;

    /** 致命的エラーコード: PERFORMスタックオーバーフロー */
    public static final int FERROR_STACK = 3;

    private static Properties envVarTable = new Properties();

    /**
     * 環境変数の値をチェックする。libcob/common.cのcob_check_envの実装。
     *
     * <p>指定された環境変数が期待される値と一致するかを確認する。
     *
     * @param name チェック対象の環境変数名
     * @param value 期待する値
     * @return 値が一致する場合は1、それ以外は0
     */
    public static int checkEnv(String name, String value) {
        if (name == null || value == null) {
            return 0;
        }

        String s = CobolUtil.getEnv(name);
        if (s != null) {
            if (s.contentEquals(value)) {
                return 1;
            }
        }
        return 0;
    }

    /**
     * REWRITE操作の仮定フラグを取得する。
     *
     * <p>環境変数COB_IO_ASSUME_REWRITEで設定されるフラグ。
     * trueの場合、WRITEをREWRITEとして扱う場合がある。
     *
     * @return REWRITEを仮定する場合はtrue
     */
    public static boolean cob_io_rewwrite_assumed() {
        return cob_io_assume_rewrite;
    }

    /**
     * 日本語項目の参照修飾をチェックする。
     *
     * <p>日本語項目（NATIONAL）の参照修飾（offset:length）が有効範囲内かを検証する。
     * 日本語項目は1文字2バイトなので、バイト単位の値を文字単位に変換してチェックする。
     *
     * @param offset 参照修飾の開始位置（バイト単位、1から開始）
     * @param length 参照修飾の長さ（バイト単位）
     * @param size 項目全体のサイズ（バイト単位）
     * @param name 項目名（Shift-JISエンコード、エラーメッセージ用）
     * @throws CobolStopRunException 範囲外の場合
     */
    public static void cobCheckRefModNational(int offset, long length, int size, byte[] name)
            throws CobolStopRunException {
        CobolUtil.cobCheckRefMod((offset + 1) / 2, length / 2, size / 2, name);
    }

    /**
     * 日本語項目の参照修飾をチェックする（String版）。
     *
     * @param offset 参照修飾の開始位置（バイト単位、1から開始）
     * @param length 参照修飾の長さ（バイト単位）
     * @param size 項目全体のサイズ（バイト単位）
     * @param name 項目名（エラーメッセージ用）
     * @throws CobolStopRunException 範囲外の場合
     * @see #cobCheckRefModNational(int, long, int, byte[])
     */
    public static void cobCheckRefModNational(int offset, long length, int size, String name)
            throws CobolStopRunException {
        CobolUtil.cobCheckRefMod((offset + 1) / 2, length / 2, size / 2, name);
    }

    /**
     * 参照修飾の境界チェックを行う（CobolDataStorage版）。
     *
     * @param offset 参照修飾の開始位置（1から開始）
     * @param length 参照修飾の長さ
     * @param size 項目全体のサイズ
     * @param name 項目名を格納したデータストレージ
     * @param nameLen 項目名の長さ
     * @throws CobolStopRunException 範囲外の場合
     * @see #cobCheckRefMod(int, long, int, String)
     */
    public static void cobCheckRefMod(
            int offset, long length, int size, CobolDataStorage name, int nameLen)
            throws CobolStopRunException {
        cobCheckRefMod(offset, length, size, name.getByteArrayRef(0, nameLen));
    }

    /**
     * 参照修飾の境界チェックを行う（byte配列版、長さ指定）。
     *
     * @param offset 参照修飾の開始位置（1から開始）
     * @param length 参照修飾の長さ
     * @param size 項目全体のサイズ
     * @param name 項目名（Shift-JISエンコード）
     * @param nameLen 項目名の長さ（未使用）
     * @throws CobolStopRunException 範囲外の場合
     * @see #cobCheckRefMod(int, long, int, String)
     */
    public static void cobCheckRefMod(int offset, long length, int size, byte[] name, int nameLen)
            throws CobolStopRunException {
        cobCheckRefMod(offset, length, size, name);
    }

    /**
     * 参照修飾の境界チェックを行う（byte配列版）。
     *
     * @param offset 参照修飾の開始位置（1から開始）
     * @param length 参照修飾の長さ
     * @param size 項目全体のサイズ
     * @param name 項目名（Shift-JISエンコード）
     * @throws CobolStopRunException 範囲外の場合
     * @see #cobCheckRefMod(int, long, int, String)
     */
    public static void cobCheckRefMod(int offset, long length, int size, byte[] name)
            throws CobolStopRunException {
        CobolUtil.cobCheckRefMod(
                offset, length, size, new String(name, AbstractCobolField.charSetSJIS));
    }

    /**
     * 参照修飾の境界チェックを行う（String版、長さ指定）。
     *
     * @param offset 参照修飾の開始位置（1から開始）
     * @param length 参照修飾の長さ
     * @param size 項目全体のサイズ
     * @param name 項目名
     * @param nameLen 項目名の長さ（未使用）
     * @throws CobolStopRunException 範囲外の場合
     * @see #cobCheckRefMod(int, long, int, String)
     */
    public static void cobCheckRefMod(int offset, long length, int size, String name, int nameLen)
            throws CobolStopRunException {
        cobCheckRefMod(offset, length, size, name);
    }

    /**
     * 参照修飾の境界チェックを行う。
     *
     * <p>COBOLの参照修飾（Reference Modification）の開始位置と長さが有効範囲内かを検証する。
     * COBOL文「WS-DATA(offset:length)」に対応するチェック。範囲外の場合は
     * COB_EC_BOUND_REF_MOD例外を設定し、プログラムを終了する。
     *
     * @param offset 参照修飾の開始位置（1から開始、項目の先頭が1）
     * @param length 参照修飾の長さ（1以上）
     * @param size 項目全体のサイズ
     * @param name 項目名（エラーメッセージ用）
     * @throws CobolStopRunException 開始位置または長さが範囲外の場合
     */
    public static void cobCheckRefMod(int offset, long length, int size, String name)
            throws CobolStopRunException {
        /* check the offset */
        if (offset < 1 || offset > size) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_BOUND_REF_MOD);
            CobolUtil.runtimeError(String.format("Offset of '%s' out of bounds: %d", name, offset));
            CobolStopRunException.stopRunAndThrow(1);
        }

        /* check the length */
        if (length < 1 || offset + length - 1 > size) {
            CobolRuntimeException.setException(CobolExceptionId.COB_EC_BOUND_REF_MOD);
            CobolUtil.runtimeError(String.format("Length of '%s' out of bounds: %d", name, length));
            CobolStopRunException.stopRunAndThrow(1);
        }
    }

    /**
     * BASED項目またはLINKAGE項目のアドレスチェックを行う。
     *
     * <p>BASED句またはLINKAGE SECTIONで定義された項目のアドレスがnullでないことを検証する。
     * nullの場合はエラーメッセージを出力し、プログラムを終了する。
     *
     * @param x チェック対象のデータストレージ
     * @param name 項目名（エラーメッセージ用）
     * @throws CobolStopRunException アドレスがnullの場合
     */
    public static void cobCheckBased(CobolDataStorage x, byte[] name) throws CobolStopRunException {
        if (x == null) {
            CobolUtil.runtimeError(String.format("BASED/LINKAGE item '%s' has NULL address", name));
            CobolStopRunException.stopRunAndThrow(1);
        }
    }

    /**
     * COBOLランタイムを初期化する。
     *
     * <p>プログラム起動時に最初に呼び出す。環境変数の読み込み、各種モジュールの初期化、
     * コマンドライン引数の保存などを行う。libcob/common.cのcob_initに対応。
     *
     * <p>読み込む環境変数：
     * <ul>
     *   <li>COB_SWITCH_1〜8 - COBOLスイッチの初期値（ON/OFF）
     *   <li>COB_DATE - 日付のオーバーライド（YYYY/MM/DD形式）
     *   <li>COB_VERBOSE - 詳細出力モード
     *   <li>COB_IO_ASSUME_REWRITE - REWRITE仮定フラグ
     *   <li>COB_NIBBLE_C_UNSIGNED - 符号なしPACKEDの符号ニブル
     *   <li>COB_FILE_SEQ_WRITE_BUFFER_SIZE - 順ファイル書き込みバッファサイズ
     *   <li>COB_TERMINAL_ENCODING - 端末エンコーディング
     * </ul>
     *
     * @param argv コマンドライン引数
     * @param cobInitialized 既に初期化済みの場合true（再初期化をスキップ）
     */
    public static void cob_init(String[] argv, boolean cobInitialized) {
        // TODO 未完成
        if (!cobInitialized) {
            CobolUtil.commandLineArgs = argv;
            CobolInspect.initString();
            CobolFile.cob_init_fileio();
            CobolIntrinsic.init();
            CobolUtil.envVarTable = new Properties();

            for (int i = 0; i < 8; ++i) {
                String envVariableName = String.format("COB_SWITCH_%d", i + 1);
                String envValue = CobolUtil.getEnv(envVariableName);
                if (envValue == null) {
                    CobolUtil.cobSwitch[i] = false;
                } else {
                    CobolUtil.cobSwitch[i] = "ON".equals(envValue);
                }
            }
        }

        cal = Calendar.getInstance();
        String s = CobolUtil.getEnv("COB_DATE");
        if (s != null) {
            Pattern p = Pattern.compile("([0-9]{4})/([0-9]{2})/([0-9]{2})");
            Matcher m = p.matcher(s);
            if (m.matches()) {
                date_time_block:
                if (m.groupCount() != 3) {
                    System.err.println("Warning: COB_DATE format invalid, ignored.");
                } else {
                    int year = Integer.parseInt(m.group(1));
                    int month = Integer.parseInt(m.group(2));
                    int dayOfMonth = Integer.parseInt(m.group(3));
                    cal.set(Calendar.YEAR, year);
                    cal.set(Calendar.MONTH, month - 1);
                    cal.set(Calendar.DAY_OF_MONTH, dayOfMonth);
                    LocalDateTime tm;
                    try {
                        tm = LocalDateTime.of(year, month, dayOfMonth, 0, 0);
                    } catch (DateTimeException e) {
                        break date_time_block;
                    }
                    cobLocalTm = tm;
                }
            } else {
                System.err.println("Warning: COB_DATE format invalid, ignored.");
            }
        }

        s = CobolUtil.getEnv("COB_VERBOSE");
        if (s != null && s.length() > 0 && (s.charAt(0) == 'y' || s.charAt(0) == 'Y')) {
            CobolUtil.cob_verbose = true;
        }

        s = CobolUtil.getEnv("COB_IO_ASSUME_REWRITE");
        if (s != null && s.length() > 0 && (s.charAt(0) == 'y' || s.charAt(0) == 'Y')) {
            CobolUtil.cob_io_assume_rewrite = true;
        }

        s = CobolUtil.getEnv("COB_NIBBLE_C_UNSIGNED");
        if (s != null && s.length() > 0 && (s.charAt(0) == 'y' || s.charAt(0) == 'Y')) {
            CobolUtil.nibbleCForUnsigned = true;
        }

        s = System.getenv("COB_FILE_SEQ_WRITE_BUFFER_SIZE");
        if (s != null) {
            int size = Integer.parseInt(s);
            if (size >= 0) {
                CobolUtil.fileSeqWriteBufferSize = size;
            }
        }

        s = System.getenv("COB_TERMINAL_ENCODING");
        CobolUtil.terminalEncoding = CobolEncoding.SHIFT_JIS;
        if (s != null) {
            Pattern p = Pattern.compile("[uU][tT][fF][_-]?8");
            Matcher m = p.matcher(s);
            if (m.matches()) {
                CobolUtil.terminalEncoding = CobolEncoding.UTF8;
            }
        }
    }

    /**
     * ローカル日時を取得する。libcob/common.cのcob_localtimeに対応。
     *
     * <p>環境変数COB_DATEで日付がオーバーライドされている場合、その日付と
     * 現在の時刻を組み合わせた日時を返す。オーバーライドされていない場合は
     * 現在の日時をそのまま返す。
     *
     * @return ローカル日時
     */
    public static LocalDateTime localtime() {
        LocalDateTime rt = LocalDateTime.now();
        if (CobolUtil.cobLocalTm != null) {
            CobolUtil.cobLocalTm =
                    CobolUtil.cobLocalTm
                            .withHour(rt.getHour())
                            .withMinute(rt.getMinute())
                            .withSecond(rt.getSecond());
            rt = CobolUtil.cobLocalTm;
        }
        return rt;
    }

    /**
     * 詳細モードでメッセージを出力する。libcob/cob_verbose_outputに対応。
     *
     * <p>環境変数COB_VERBOSEが設定されている場合のみ、標準出力にメッセージを出力する。
     * デバッグやトラブルシューティングに使用。
     *
     * @param s 出力するメッセージ
     */
    public static void verboseOutput(String s) {
        if (cob_verbose) {
            System.out.println("libcobj: " + s);
        }
    }

    /**
     * ランタイムエラーメッセージを出力する。libcob/common.cのcob_runtime_errorに対応。
     *
     * <p>エラーハンドラが登録されている場合はハンドラを呼び出し、
     * その後標準エラー出力にメッセージを出力する。ソースファイル情報が
     * 設定されている場合は、ファイル名と行番号も出力する。
     *
     * @param s エラーメッセージ
     */
    public static void runtimeError(String s) {
        if (hdlrs != null) {
            HandlerList h = hdlrs;
            if (runtime_err_str != null) {
                if (sourceFile != null) {
                    runtime_err_str = String.format("%s:%d: ", sourceFile, sourceLine);
                }
            }
            while (h != null) {
                if (runtime_err_str != null) {
                    h.proc(runtime_err_str);
                } else {
                    h.proc("Malloc error");
                }
                h = h.next;
            }
            hdlrs = null;
        }

        if (sourceFile != null) {
            System.err.print(String.format("%s:%d: ", sourceFile, sourceLine));
        }
        byte[] messageBytes = ("libcobj: " + s).getBytes(AbstractCobolField.charSetSJIS);
        System.err.write(messageBytes, 0, messageBytes.length);
        System.err.println();
        System.err.flush();
    }

    /**
     * 環境変数の値を取得し、COBOLフィールドに格納する。libcob/common.cのcob_get_environmentに対応。
     *
     * <p>ACCEPT文のFROM ENVIRONMENT句で使用される。環境変数が存在しない場合は
     * COB_EC_IMP_ACCEPT例外を設定し、スペースを返す。
     *
     * @param envname 環境変数名を格納したフィールド
     * @param envval 結果を格納するフィールド
     */
    public static void getEnvironment(AbstractCobolField envname, AbstractCobolField envval) {
        String p = CobolUtil.getEnv(envname.fieldToString());
        if (p == null) {
            CobolExceptionInfo.setException(CobolExceptionId.COB_EC_IMP_ACCEPT);
            p = " ";
        }
        envval.memcpy(p);
    }

    /**
     * 関数のパラメータ数をチェックする。libcob/common.cのCOB_CHK_PARMSマクロに対応。
     *
     * <p>現在は何も行わない（将来の拡張用）。
     *
     * @param funcName 関数名
     * @param numParams 期待するパラメータ数
     */
    public static void COB_CHK_PARMS(String funcName, int numParams) {}

    /**
     * COBOLスイッチの状態を取得する。libcob/common.cのcob_get_switchに対応。
     *
     * <p>COBOLのSW-1〜SW-8に対応するスイッチの状態を返す。
     *
     * @param n スイッチ番号（0〜7、SW-1が0に対応）
     * @return スイッチがONの場合true
     */
    public static boolean getSwitch(int n) {
        return CobolUtil.cobSwitch[n];
    }

    /**
     * COBOLスイッチの状態を設定する。libcob/common.cのcob_set_switchに対応。
     *
     * <p>COBOLのSET文でスイッチをON/OFFする際に呼び出される。
     *
     * @param n スイッチ番号（0〜7、SW-1が0に対応）
     * @param flag 0でOFF、1でON
     */
    public static void setSwitch(int n, int flag) {
        if (flag == 0) {
            CobolUtil.cobSwitch[n] = false;
        } else if (flag == 1) {
            CobolUtil.cobSwitch[n] = true;
        }
    }

    /**
     * 英数字項目の比較を行う。libcob/common.cのalnum_cmpsに対応。
     *
     * <p>2つのデータストレージの内容をバイト単位で比較する。
     * 照合順序（collating sequence）が指定されている場合は、その順序に従って比較する。
     *
     * @param s1 比較対象1
     * @param s2 比較対象2
     * @param size 比較するバイト数
     * @param col 照合順序テーブル（nullの場合はバイナリ順）
     * @return s1 &gt; s2なら正、s1 &lt; s2なら負、等しければ0
     */
    public static int alnumCmps(
            CobolDataStorage s1, CobolDataStorage s2, int size, CobolDataStorage col) {
        if (col != null) {
            for (int i = 0; i < size; ++i) {
                int ret =
                        col.getByte(Byte.toUnsignedInt(s1.getByte(i)))
                                - col.getByte(Byte.toUnsignedInt(s2.getByte(i)));
                if (ret != 0) {
                    return ret;
                }
            }
        } else {
            for (int i = 0; i < size; ++i) {
                int ret = Byte.toUnsignedInt(s1.getByte(i)) - Byte.toUnsignedInt(s2.getByte(i));
                if (ret != 0) {
                    return ret;
                }
            }
        }
        return 0;
    }

    /**
     * 日本語項目（NATIONAL）の比較を行う。libcob/common.cのnational_cmpsに対応。
     *
     * <p>2つのデータストレージの内容を2バイト単位（日本語1文字）で比較する。
     * 各2バイトをビッグエンディアンの16ビット値として解釈し、比較する。
     *
     * @param s1 比較対象1
     * @param s2 比較対象2
     * @param size 比較するバイト数（2の倍数であること）
     * @param col 照合順序テーブル（現在未使用）
     * @return s1 &gt; s2なら正、s1 &lt; s2なら負、等しければ0
     */
    public static int nationalCmps(
            CobolDataStorage s1, CobolDataStorage s2, int size, CobolDataStorage col) {
        int ret = 0;
        for (int i = 0; i < size && ret == 0; i += 2) {
            int b11 = Byte.toUnsignedInt(s1.getByte(i));
            int b12 = Byte.toUnsignedInt(s1.getByte(i + 1));
            int b21 = Byte.toUnsignedInt(s2.getByte(i));
            int b22 = Byte.toUnsignedInt(s2.getByte(i + 1));
            ret = ((b11 << 8) | b12) - ((b21 << 8) | b22);
        }
        return ret;
    }

    /**
     * 行トレースを有効化する。COBOL文「READY TRACE」に対応。
     *
     * <p>有効化後、setLocationが呼び出されるたびに現在の実行位置が標準エラーに出力される。
     */
    public static void readyTrace() {
        CobolUtil.lineTrace = true;
    }

    /**
     * 行トレースを無効化する。COBOL文「RESET TRACE」に対応。
     */
    public static void resetTrace() {
        CobolUtil.lineTrace = false;
    }

    /**
     * 致命的エラーを報告し、プログラムを終了する。
     *
     * <p>ランタイムの内部エラーが発生した場合に呼び出される。
     * エラーコードに応じたメッセージを出力し、終了コード1でプログラムを終了する。
     *
     * @param fatalError エラーコード（FERROR_xxx定数）
     * @throws CobolStopRunException 常にスロー
     */
    public static void fatalError(int fatalError) throws CobolStopRunException {
        switch (fatalError) {
            case CobolUtil.FERROR_INITIALIZED:
                CobolUtil.runtimeError("cob_init() has not been called");
                break;
            case CobolUtil.FERROR_CODEGEN:
                CobolUtil.runtimeError("Codegen error - Please report this");
                break;
            case CobolUtil.FERROR_CHAINING:
                CobolUtil.runtimeError("ERROR - Recursive call of chained program");
                break;
            case CobolUtil.FERROR_STACK:
                CobolUtil.runtimeError("Stack overflow, possible PERFORM depth exceeded");
                break;
            default:
                CobolUtil.runtimeError(String.format("Unknown failure : %d", fatalError));
                break;
        }
        CobolStopRunException.stopRunAndThrow(1);
    }

    /**
     * 現在の実行位置情報を設定する。
     *
     * <p>コンパイラオプション-gでデバッグ情報付きでコンパイルした場合、
     * 生成コードの各文の実行前にこのメソッドが呼び出される。
     * 行トレースが有効な場合は、実行位置を標準エラーに出力する。
     *
     * @param progId プログラムID（PROGRAM-ID）
     * @param sfile ソースファイル名
     * @param sline ソース行番号
     * @param csect 現在のセクション名
     * @param cpara 現在の段落名
     * @param cstatement 現在実行中のCOBOL文
     */
    public static void setLocation(
            String progId, String sfile, int sline, String csect, String cpara, String cstatement) {
        CobolUtil.sourceFile = sfile;
        CobolUtil.sourceLine = sline;
        currProgramId = progId;
        currSection = csect;
        currParagraph = cpara;
        sourceLine = sline;
        if (cstatement != null) {
            sourceStatement = cstatement;
        }
        if (CobolUtil.lineTrace) {
            System.err.println(
                    String.format(
                            "PROGRAM-ID: %s \tLine: %d \tStatement: %s",
                            progId, sline, cstatement == null ? "Unknown" : cstatement));
            System.err.flush();
        }
    }

    /**
     * 環境変数の値を取得する。
     *
     * <p>まず内部の環境変数テーブルを検索し、見つからない場合はシステムの環境変数を検索する。
     * DISPLAY ... UPON ENVIRONMENT-NAMEで設定された値は内部テーブルに格納される。
     *
     * @param envVarName 環境変数名
     * @return 環境変数の値。見つからない場合はnull
     */
    public static String getEnv(String envVarName) {
        String envVarInTable = CobolUtil.envVarTable.getProperty(envVarName);
        if (envVarInTable != null) {
            return envVarInTable;
        } else {
            return System.getenv(envVarName);
        }
    }

    /**
     * 環境変数の値を設定する。
     *
     * <p>内部の環境変数テーブルに値を格納する。DISPLAY ... UPON ENVIRONMENT-NAMEで使用される。
     *
     * @param envVarName 環境変数名
     * @param envVarValue 設定する値
     */
    public static void setEnv(String envVarName, String envVarValue) {
        CobolUtil.envVarTable.setProperty(envVarName, envVarValue);
    }

    /**
     * 環境変数の値を設定する（フィールド版）。
     *
     * <p>COBOLフィールドから環境変数名と値を取得し、内部テーブルに設定する。
     * 環境変数名の前後の空白は除去される。
     *
     * @param envVarName 環境変数名を格納したフィールド（前後の空白は無視される）
     * @param envVarValue 設定する値を格納したフィールド
     */
    public static void setEnv(AbstractCobolField envVarName, AbstractCobolField envVarValue) {
        CobolUtil.envVarTable.setProperty(envVarName.getString().trim(), envVarValue.getString());
    }

    /**
     * 文字列をShift-JISエンコードのバイト配列に変換する。
     *
     * <p>生成されたJavaコードでCOBOL文字列リテラルを初期化する際に使用される。
     *
     * @param s 変換する文字列
     * @return Shift-JISエンコードのバイト配列
     */
    public static byte[] stringToBytes(String s) {
        return s.getBytes(AbstractCobolField.charSetSJIS);
    }

    /**
     * 可変長引数のバイト値をバイト配列に変換する。
     *
     * <p>バイト配列リテラルを簡潔に記述するためのユーティリティメソッド。
     *
     * @param bytes バイト値（可変長引数）
     * @return バイト配列
     */
    public static byte[] toBytes(byte... bytes) {
        return bytes;
    }

    /**
     * 現在実行中のプログラムIDを取得する。
     *
     * @return プログラムID。setLocationで設定された値。
     */
    public static String getCurrProgramId() {
        return currProgramId;
    }

    /**
     * 現在実行中のセクション名を取得する。
     *
     * @return セクション名。setLocationで設定された値。
     */
    public static String getCurrSection() {
        return currSection;
    }

    /**
     * 現在実行中の段落名を取得する。
     *
     * @return 段落名。setLocationで設定された値。
     */
    public static String getCurrParagraph() {
        return currParagraph;
    }

    /**
     * 現在実行中のソース行番号を取得する。
     *
     * @return ソース行番号。setLocationで設定された値。
     */
    public static int getSourceLine() {
        return sourceLine;
    }

    /**
     * 現在実行中のCOBOL文を取得する。
     *
     * @return COBOL文の文字列表現。setLocationで設定された値。
     */
    public static String getSourceStatement() {
        return sourceStatement;
    }
}

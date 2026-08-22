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

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.OffsetDateTime;
import java.util.Properties;
import java.util.concurrent.atomic.AtomicBoolean;
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
 * COBOLランタイムの各種ユーティリティ機能を提供するクラス。<br>
 * libcobのcommon.cに相当し、ランタイムの初期化、環境変数の取得・設定、コマンドライン引数(CHAINING)の処理、<br>
 * ランタイムエラーの報告、SWITCHの設定・取得、参照修飾(reference modification)の境界チェック、<br>
 * 文字列比較などのヘルパー処理をまとめて提供する。
 */
public class CobolUtil {
    /** I/O操作でREWRITEを暗黙的に行うとみなすかどうかのフラグ(環境変数COB_IO_ASSUME_REWRITEで設定)。 */
    @SuppressWarnings("PMD.AvoidUsingVolatile")
    private static volatile boolean cob_io_assume_rewrite = false;

    /** 冗長出力を有効にするかどうかのフラグ(環境変数COB_VERBOSEで設定)。 */
    @SuppressWarnings("PMD.AvoidUsingVolatile")
    private static volatile boolean cob_verbose = false;

    /**
     * 実行単位(スレッド)ごとに保持するランタイムの状態。<br>
     * 実行位置、プログラムスイッチ、エラーハンドラなど、COBOLの実行単位に属する情報をまとめて保持する。
     */
    private static final class RunUnitState {
        /** DISPLAY UPON ENVIRONMENT-NAMEで設定された環境変数名。 */
        String cobLocalEnv = null;

        /** ACCEPT FROM ARGUMENT-VALUEで次に取得するコマンドライン引数のインデックス(1始まり)。 */
        int currentArgIndex = 1;

        /** プログラムスイッチ(SWITCH-1からSWITCH-8)の状態。環境変数COB_SWITCH_nで初期化される。 */
        final boolean[] cobSwitch = new boolean[8];

        /** CALL文の引数個数の退避値。 */
        int cobSaveCallParams = 0;

        /** 終了時にエラーが発生したことを示すフラグ。 */
        boolean cobErrorOnExitFlag = false;

        /** 行トレース(READY TRACE)が有効かどうか。 */
        boolean lineTrace = false;

        /** 現在実行中のソースファイル名。 */
        String sourceFile;

        /** 現在実行中のソース行番号。 */
        int sourceLine;

        /** 現在実行中のプログラムID。 */
        String currProgramId;

        /** 現在実行中のセクション名。 */
        String currSection;

        /** 現在実行中の段落(パラグラフ)名。 */
        String currParagraph;

        /** 現在実行中の文(ステートメント)の名称。 */
        String sourceStatement;

        /** SET ENVIRONMENTなどでプログラムが設定した環境変数のテーブル。 */
        final Properties envVarTable = new Properties();

        RunUnitState() {
            for (int i = 0; i < 8; ++i) {
                String envValue = System.getenv(String.format("COB_SWITCH_%d", i + 1));
                cobSwitch[i] = envValue != null && "ON".equals(envValue);
            }
        }
    }

    /** 実行単位ごとの状態。実行単位はスレッドごとに独立しているため、スレッドごとに保持する。 */
    private static final ThreadLocal<RunUnitState> runUnitState =
            ThreadLocal.withInitial(RunUnitState::new);

    /** cob_initによるランタイム設定の初期化が完了しているかどうか。 */
    private static final AtomicBoolean initialized = new AtomicBoolean(false);

    /** cob_initの排他制御に使用するロック。 */
    private static final Object INIT_LOCK = new Object();

    /** 環境変数COB_DATEで指定された固定日付を保持する。指定がない場合はnull。 */
    @SuppressWarnings("PMD.AvoidUsingVolatile")
    public static volatile LocalDateTime cobLocalTm = null;

    /** プログラムに渡されたコマンドライン引数(プログラム名を含まない)。 */
    @SuppressWarnings("PMD.AvoidUsingVolatile")
    public static volatile String[] commandLineArgs = new String[0];

    /** 符号なし数値のニブル定数Cの扱いを制御するフラグ(環境変数COB_NIBBLE_C_UNSIGNEDで設定)。 */
    @SuppressWarnings("PMD.AvoidUsingVolatile")
    public static volatile boolean nibbleCForUnsigned = false;

    /** 順編成ファイルの入出力バッファサイズ(レコード数)のデフォルト値。 */
    private static final int DEFAULT_FILE_SEQ_BUFFER_SIZE = 10;

    /** 順編成ファイルへの書き込みバッファサイズ(環境変数COB_FILE_SEQ_WRITE_BUFFER_SIZEで設定)。 */
    @SuppressWarnings("PMD.AvoidUsingVolatile")
    public static volatile int fileSeqWriteBufferSize = DEFAULT_FILE_SEQ_BUFFER_SIZE;

    /** 順編成ファイルからの読み込みバッファサイズ(環境変数COB_FILE_SEQ_READ_BUFFER_SIZEで設定)。 */
    @SuppressWarnings("PMD.AvoidUsingVolatile")
    public static volatile int fileSeqReadBufferSize = DEFAULT_FILE_SEQ_BUFFER_SIZE;

    /**
     * 環境変数COB_FILE_IDX_COMMIT_INTERVALに{@code INF}が指定されたか。デフォルト(環境変数が
     * 未指定)も{@code INF}である。COBOLには{@code COMMIT}文があり、コミットの時点は
     * プログラムが{@code COMMIT}文と{@code CLOSE}文で決めるのが本来の姿であるため、
     * ランタイムが勝手に途中でコミットすることはしない。
     *
     * <p>{@code true}のときINDEXEDファイルのOUTPUTモードでは途中コミットを行わず、{@code
     * COMMIT}文と{@code CLOSE}時のみコミットする。このとき{@link #fileIdxCommitInterval}は参照されない。
     */
    @SuppressWarnings("PMD.AvoidUsingVolatile")
    private static volatile boolean fileIdxCommitIntervalIsInf = true;

    /**
     * INDEXEDファイルをOUTPUTモードで書き込むときのコミット間隔(レコード数。環境変数COB_FILE_IDX_COMMIT_INTERVALで設定)。
     * {@link #fileIdxCommitIntervalIsInf}が{@code true}のときは参照されない。
     */
    @SuppressWarnings("PMD.AvoidUsingVolatile")
    private static volatile int fileIdxCommitInterval = 0;

    /**
     * 環境変数COB_FILE_IDX_COMMIT_INTERVALに{@code INF}が指定されたかを返す。
     *
     * @return {@code INF}が指定されていれば{@code true}。このときINDEXEDファイルのOUTPUTモードでは
     *     途中コミットを行わず、CLOSE時のみコミットする
     */
    public static boolean isFileIdxCommitIntervalInf() {
        return fileIdxCommitIntervalIsInf;
    }

    /**
     * INDEXEDファイルをOUTPUTモードで書き込むときのコミット間隔(レコード数)を返す。
     *
     * @return コミット間隔(レコード数)。{@link #isFileIdxCommitIntervalInf()}が{@code true}のときは無意味な値
     */
    public static int getFileIdxCommitInterval() {
        return fileIdxCommitInterval;
    }

    /** DISPLAY/ACCEPT文によるデータ出力時のエンコーディング */
    @SuppressWarnings("PMD.AvoidUsingVolatile")
    public static volatile CobolEncoding terminalEncoding = CobolEncoding.SHIFT_JIS;

    /** 致命的エラー種別: cob_init()が呼び出されていない。 */
    public static final int FERROR_INITIALIZED = 0;

    /** 致命的エラー種別: コード生成エラー(報告が必要な内部エラー)。 */
    public static final int FERROR_CODEGEN = 1;

    /** 致命的エラー種別: CHAININGプログラムの再帰呼び出し。 */
    public static final int FERROR_CHAINING = 2;

    /** 致命的エラー種別: スタックオーバーフロー(PERFORMの入れ子が深すぎる等)。 */
    public static final int FERROR_STACK = 3;

    // libcob/common.cのcob_check_envの実装
    /**
     * 指定した環境変数の値が、指定した値と一致するかどうかを判定する。
     *
     * @param name 環境変数名
     * @param value 比較対象の値
     * @return 環境変数が存在し値が一致する場合は1、それ以外(引数がnullの場合を含む)は0
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
     * I/O操作でREWRITEを暗黙的に行うとみなす設定が有効かどうかを返す。
     *
     * @return 暗黙のREWRITEが有効な場合はtrue、そうでない場合はfalse
     */
    public static boolean cob_io_rewwrite_assumed() {
        return cob_io_assume_rewrite;
    }

    /**
     * 日本語項目(NATIONAL)に対する参照修飾の境界チェックを行う。<br>
     * 1文字が2バイトであることを考慮し、各値を文字単位に換算してから境界チェックを実行する。
     *
     * @param offset 参照修飾の開始位置(バイト単位、1始まり)
     * @param length 参照修飾の長さ(バイト単位)
     * @param size 対象項目の全体サイズ(バイト単位)
     * @param name 対象項目の名前(バイト配列)
     * @throws CobolStopRunException 参照修飾が境界外でランタイムを停止する場合
     */
    public static void cobCheckRefModNational(int offset, long length, int size, byte[] name)
            throws CobolStopRunException {
        CobolUtil.cobCheckRefMod((offset + 1) / 2, length / 2, size / 2, name);
    }

    /**
     * 日本語項目(NATIONAL)に対する参照修飾の境界チェックを行う。<br>
     * 1文字が2バイトであることを考慮し、各値を文字単位に換算してから境界チェックを実行する。
     *
     * @param offset 参照修飾の開始位置(バイト単位、1始まり)
     * @param length 参照修飾の長さ(バイト単位)
     * @param size 対象項目の全体サイズ(バイト単位)
     * @param name 対象項目の名前
     * @throws CobolStopRunException 参照修飾が境界外でランタイムを停止する場合
     */
    public static void cobCheckRefModNational(int offset, long length, int size, String name)
            throws CobolStopRunException {
        CobolUtil.cobCheckRefMod((offset + 1) / 2, length / 2, size / 2, name);
    }

    /**
     * 参照修飾の境界チェックを行う。
     *
     * @param offset 参照修飾の開始位置(1始まり)
     * @param length 参照修飾の長さ
     * @param size 対象項目の全体サイズ
     * @param name 対象項目の名前を保持するデータストレージ
     * @param nameLen 対象項目の名前の長さ(バイト数)
     * @throws CobolStopRunException 参照修飾が境界外でランタイムを停止する場合
     */
    public static void cobCheckRefMod(
            int offset, long length, int size, CobolDataStorage name, int nameLen)
            throws CobolStopRunException {
        cobCheckRefMod(offset, length, size, name.getByteArrayRef(0, nameLen));
    }

    /**
     * 参照修飾の境界チェックを行う。
     *
     * @param offset 参照修飾の開始位置(1始まり)
     * @param length 参照修飾の長さ
     * @param size 対象項目の全体サイズ
     * @param name 対象項目の名前(バイト配列)
     * @param nameLen 対象項目の名前の長さ(バイト数)
     * @throws CobolStopRunException 参照修飾が境界外でランタイムを停止する場合
     */
    public static void cobCheckRefMod(int offset, long length, int size, byte[] name, int nameLen)
            throws CobolStopRunException {
        cobCheckRefMod(offset, length, size, name);
    }

    /**
     * 参照修飾の境界チェックを行う。バイト配列の名前をSHIFT_JISの文字列に変換して処理する。
     *
     * @param offset 参照修飾の開始位置(1始まり)
     * @param length 参照修飾の長さ
     * @param size 対象項目の全体サイズ
     * @param name 対象項目の名前(バイト配列)
     * @throws CobolStopRunException 参照修飾が境界外でランタイムを停止する場合
     */
    public static void cobCheckRefMod(int offset, long length, int size, byte[] name)
            throws CobolStopRunException {
        CobolUtil.cobCheckRefMod(
                offset, length, size, new String(name, AbstractCobolField.charSetSJIS));
    }

    /**
     * 参照修飾の境界チェックを行う。
     *
     * @param offset 参照修飾の開始位置(1始まり)
     * @param length 参照修飾の長さ
     * @param size 対象項目の全体サイズ
     * @param name 対象項目の名前
     * @param nameLen 対象項目の名前の長さ(バイト数)
     * @throws CobolStopRunException 参照修飾が境界外でランタイムを停止する場合
     */
    public static void cobCheckRefMod(int offset, long length, int size, String name, int nameLen)
            throws CobolStopRunException {
        cobCheckRefMod(offset, length, size, name);
    }

    /**
     * 参照修飾(reference modification)の開始位置と長さが項目の境界内に収まっているかをチェックする。<br>
     * 境界外の場合はCOB_EC_BOUND_REF_MOD例外を設定し、ランタイムエラーを出力して実行を停止する。
     *
     * @param offset 参照修飾の開始位置(1始まり)
     * @param length 参照修飾の長さ
     * @param size 対象項目の全体サイズ
     * @param name 対象項目の名前
     * @throws CobolStopRunException 参照修飾が境界外でランタイムを停止する場合
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
     * BASED項目またはLINKAGE項目が有効なアドレス(非null)を持つかをチェックする。<br>
     * アドレスがnullの場合はランタイムエラーを出力して実行を停止する。
     *
     * @param x チェック対象の項目を指すデータストレージ
     * @param name 対象項目の名前(バイト配列)
     * @throws CobolStopRunException アドレスがnullでランタイムを停止する場合
     */
    public static void cobCheckBased(CobolDataStorage x, byte[] name) throws CobolStopRunException {
        if (x == null) {
            CobolUtil.runtimeError(String.format("BASED/LINKAGE item '%s' has NULL address", name));
            CobolStopRunException.stopRunAndThrow(1);
        }
    }

    /**
     * ファイルI/Oのバッファサイズを指定する環境変数を読み込む。<br>
     * 環境変数が設定されていない場合や、0以上の整数として解釈できない場合はデフォルト値を返す。
     *
     * @param envVariableName 環境変数名
     * @param defaultValue 環境変数が指定されていない場合に使用する値
     * @return バッファサイズ(レコード数)
     */
    private static int bufferSizeFromEnv(String envVariableName, int defaultValue) {
        String s = CobolUtil.getEnv(envVariableName);
        if (s == null) {
            return defaultValue;
        }
        int size;
        try {
            size = Integer.parseInt(s.trim());
        } catch (NumberFormatException e) {
            size = -1;
        }
        if (size < 0) {
            System.err.println("Warning: " + envVariableName + " format invalid, ignored.");
            return defaultValue;
        }
        return size;
    }

    /**
     * INDEXEDファイルのOUTPUTモードにおけるコミット間隔を環境変数から読み取り、
     * {@link #fileIdxCommitIntervalIsInf}と{@link #fileIdxCommitInterval}に反映する。<br>
     * {@code INF}(大文字小文字不問)と未指定は「途中コミットしない」を意味する。<br>
     * {@code 0}は「毎回コミット」として{@code 1}に正規化する。<br>
     * 0以上の整数とも{@code INF}とも解釈できない場合は、警告を出力して未指定と同じ扱いにする。
     *
     * @param envVariableName 環境変数名
     */
    private static void readFileIdxCommitInterval(String envVariableName) {
        String s = CobolUtil.getEnv(envVariableName);
        if (s == null || "INF".equalsIgnoreCase(s.trim())) {
            CobolUtil.fileIdxCommitIntervalIsInf = true;
            return;
        }
        int interval;
        try {
            interval = Integer.parseInt(s.trim());
        } catch (NumberFormatException e) {
            interval = -1;
        }
        if (interval < 0) {
            System.err.println("Warning: " + envVariableName + " format invalid, ignored.");
            CobolUtil.fileIdxCommitIntervalIsInf = true;
            return;
        }
        CobolUtil.fileIdxCommitIntervalIsInf = false;
        CobolUtil.fileIdxCommitInterval = interval == 0 ? 1 : interval;
    }

    /**
     * COBOLランタイムを初期化する。<br>
     * 未初期化の場合は、コマンドライン引数の保存、各サブシステム(INSPECT/ファイルI/O/組み込み関数)の初期化、<br>
     * およびCOB_SWITCH_1〜COB_SWITCH_8環境変数によるスイッチ設定を行う。<br>
     * その後、COB_DATE、COB_VERBOSE、COB_IO_ASSUME_REWRITE、COB_NIBBLE_C_UNSIGNED、<br>
     * COB_FILE_SEQ_WRITE_BUFFER_SIZE、COB_FILE_SEQ_READ_BUFFER_SIZE、COB_TERMINAL_ENCODINGなどの<br>
     * 環境変数を読み込んでランタイム設定を反映する。
     *
     * @param argv コマンドライン引数(プログラム名を含まない)
     * @param cobInitialized すでに初期化済みかどうか。trueの場合はサブシステムの初期化をスキップする
     */
    public static void cob_init(String[] argv, boolean cobInitialized) {
        synchronized (INIT_LOCK) {
            cobInitInternal(argv, cobInitialized);
        }
    }

    private static void cobInitInternal(String[] argv, boolean cobInitialized) {
        if (!cobInitialized) {
            CobolUtil.commandLineArgs = argv == null ? new String[0] : argv;
            CobolFile.cob_init_fileio();
        }
        initialized.set(true);

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
                    // 範囲外の月日はGnuCOBOL 3.2(OSSC patch)と同様に無効として扱う。
                    // opensource COBOL 1.5はmktimeに任せており、2026/13/01を2027/01/01として
                    // 受け入れる一方、mktimeの戻り値が負になるエポック前の日付をすべて拒否する。
                    // 後者は再現しないため、範囲チェックのある3.2側に揃えている。
                    // また0年はISO暦では紀元前1年にあたり、ACCEPT FROM DATEの書式(年号内の年)と
                    // FUNCTION CURRENT-DATE(暦年)とで異なる値になるため、同様に無効とする。
                    if (year < 1 || month < 1 || month > 12 || dayOfMonth < 1 || dayOfMonth > 31) {
                        System.err.println("Warning: COB_DATE format invalid, ignored.");
                        break date_time_block;
                    }
                    // 範囲内であれば、その月に存在しない日はmktimeと同様に翌月へ繰り越す。
                    // (例: 2026/02/30は2026/03/02として扱われる。うるう年の判定はLocalDateに任せる)
                    cobLocalTm =
                            LocalDate.of(year, month, 1).plusDays(dayOfMonth - 1).atStartOfDay();
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

        CobolUtil.fileSeqWriteBufferSize =
                bufferSizeFromEnv("COB_FILE_SEQ_WRITE_BUFFER_SIZE", DEFAULT_FILE_SEQ_BUFFER_SIZE);
        CobolUtil.fileSeqReadBufferSize =
                bufferSizeFromEnv("COB_FILE_SEQ_READ_BUFFER_SIZE", DEFAULT_FILE_SEQ_BUFFER_SIZE);
        readFileIdxCommitInterval("COB_FILE_IDX_COMMIT_INTERVAL");

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
     * ランタイム設定が未初期化であれば初期化する。<br>
     * 生成されたmainメソッドを経由せず、Javaアプリケーション(Webアプリケーションなど)から
     * COBOLプログラムを直接呼び出す場合の入口で使用する。
     * 初期化は同一JVM内で1回だけ行われ、複数スレッドから同時に呼び出しても安全である。
     */
    public static void ensureInitialized() {
        if (!initialized.get()) {
            cob_init(new String[0], false);
        }
    }

    /**
     * ランタイム設定が初期化済みかどうかを返す。
     *
     * @return cob_initが呼び出されていればtrue
     */
    public static boolean isInitialized() {
        return initialized.get();
    }

    /** 現在のスレッドに紐づく実行単位の状態を破棄する。実行単位の終了時に呼び出す。 */
    public static void resetThreadState() {
        runUnitState.remove();
    }

    /**
     * 現在のスレッドのプログラムスイッチの配列を取得する。
     *
     * @return SWITCH-1からSWITCH-8に対応する長さ8の配列(変更は現在のスレッドにのみ反映される)
     */
    public static boolean[] getSwitches() {
        return runUnitState.get().cobSwitch;
    }

    /**
     * 現在のスレッドのCALL文の引数個数の退避値を取得する。
     *
     * @return 退避されている引数個数
     */
    public static int getSaveCallParams() {
        return runUnitState.get().cobSaveCallParams;
    }

    /**
     * 現在のスレッドのCALL文の引数個数の退避値を設定する。
     *
     * @param n 退避する引数個数
     */
    public static void setSaveCallParams(int n) {
        runUnitState.get().cobSaveCallParams = n;
    }

    /**
     * 現在のスレッドの終了時エラーフラグを取得する。
     *
     * @return 終了時にエラーが発生していればtrue
     */
    public static boolean getErrorOnExitFlag() {
        return runUnitState.get().cobErrorOnExitFlag;
    }

    /**
     * 現在のスレッドの終了時エラーフラグを設定する。
     *
     * @param flag 設定する値
     */
    public static void setErrorOnExitFlag(boolean flag) {
        runUnitState.get().cobErrorOnExitFlag = flag;
    }

    /**
     * 現在のスレッドのACCEPT FROM ARGUMENT-VALUEの引数インデックスを取得する。
     *
     * @return 次に取得する引数のインデックス(1始まり)
     */
    public static int getCurrentArgIndex() {
        return runUnitState.get().currentArgIndex;
    }

    /**
     * 現在のスレッドのACCEPT FROM ARGUMENT-VALUEの引数インデックスを設定する。
     *
     * @param n 次に取得する引数のインデックス(1始まり)
     */
    public static void setCurrentArgIndex(int n) {
        runUnitState.get().currentArgIndex = n;
    }

    /**
     * 現在のスレッドのDISPLAY UPON ENVIRONMENT-NAMEで設定された環境変数名を取得する。
     *
     * @return 環境変数名。未設定の場合はnull
     */
    public static String getLocalEnvName() {
        return runUnitState.get().cobLocalEnv;
    }

    /**
     * 現在のスレッドのDISPLAY UPON ENVIRONMENT-NAMEで設定された環境変数名を設定する。
     *
     * @param name 環境変数名
     */
    public static void setLocalEnvName(String name) {
        runUnitState.get().cobLocalEnv = name;
    }

    // libcob/common.cとcob_localtime
    /**
     * 現在のローカル日時をグリニッジ標準時からのオフセット付きで取得する。<br>
     * 環境変数COB_DATEで固定日付({@link #cobLocalTm})が指定されている場合は、日付部分だけをその値で<br>
     * 置き換える。時刻とオフセットは常にシステムクロックから取得するため、同一プロセス内でも<br>
     * 呼び出しのたびに新しい時刻が得られる。
     *
     * @return 取得したローカル日時
     */
    static OffsetDateTime localtime() {
        OffsetDateTime now = OffsetDateTime.now();
        if (CobolUtil.cobLocalTm == null) {
            return now;
        }
        return OffsetDateTime.of(
                CobolUtil.cobLocalTm.toLocalDate(), now.toLocalTime(), now.getOffset());
    }

    // libcob/cob_verbose_outputの実装
    /**
     * 冗長出力が有効な場合に、メッセージを標準出力へ出力する。<br>
     * 出力時には先頭に"libcobj: "が付加される。
     *
     * @param s 出力するメッセージ
     */
    public static void verboseOutput(String s) {
        if (cob_verbose) {
            System.out.println("libcobj: " + s);
        }
    }

    // libcob/fileio.cのcob_rintime_errorの実装
    /**
     * ランタイムエラーメッセージを出力する。<br>
     * ソースファイル名と行番号(判明している場合)を前置して、先頭に"libcobj: "を付けたメッセージを<br>
     * SHIFT_JISエンコードで標準エラー出力へ書き込む。
     *
     * @param s 出力するエラーメッセージ
     */
    public static void runtimeError(String s) {
        RunUnitState st = runUnitState.get();
        if (st.sourceFile != null) {
            System.err.print(String.format("%s:%d: ", st.sourceFile, st.sourceLine));
        }
        byte[] messageBytes = ("libcobj: " + s).getBytes(AbstractCobolField.charSetSJIS);
        System.err.write(messageBytes, 0, messageBytes.length);
        System.err.println();
        System.err.flush();
    }

    // libcob/common.c cob_get_environment
    /**
     * 環境変数の値を取得し、指定したフィールドへ格納する。<br>
     * 指定した名前の環境変数が存在しない場合はCOB_EC_IMP_ACCEPT例外を設定し、空白を格納する。
     *
     * @param envname 取得する環境変数名を保持するフィールド
     * @param envval 取得した値を格納するフィールド
     */
    public static void getEnvironment(AbstractCobolField envname, AbstractCobolField envval) {
        String p = CobolUtil.getEnv(envname.fieldToString());
        if (p == null) {
            CobolExceptionInfo.setException(CobolExceptionId.COB_EC_IMP_ACCEPT);
            p = " ";
        }
        envval.memcpy(p);
    }

    // libcob/common.cのCOB_CHK_PARMSの実装
    /**
     * サブルーチン呼び出し時に渡されたパラメータ数を検査する(libcobのCOB_CHK_PARMSに相当)。<br>
     * 現状の実装では検査処理は行わない。
     *
     * @param funcName 検査対象の関数(サブルーチン)名
     * @param numParams 期待されるパラメータ数
     */
    public static void COB_CHK_PARMS(String funcName, int numParams) {}

    // libcob/common.cのcob_get_switchの実装
    /**
     * 指定したプログラムスイッチの状態を取得する。
     *
     * @param n スイッチのインデックス(0始まり、SWITCH-1が0に対応する)
     * @return スイッチがONの場合はtrue、OFFの場合はfalse
     */
    public static boolean getSwitch(int n) {
        return runUnitState.get().cobSwitch[n];
    }

    // libcob/common.cのcob_set_switchの実装
    /**
     * 指定したプログラムスイッチの状態を設定する。
     *
     * @param n スイッチのインデックス(0始まり、SWITCH-1が0に対応する)
     * @param flag 設定する値。0でOFF、1でONに設定する(それ以外の値の場合は変更しない)
     */
    public static void setSwitch(int n, int flag) {
        if (flag == 0) {
            runUnitState.get().cobSwitch[n] = false;
        } else if (flag == 1) {
            runUnitState.get().cobSwitch[n] = true;
        }
    }

    // libcob/common.cのalnum_cmpsの実装
    /**
     * 2つの英数字データを指定したバイト数だけ比較する。<br>
     * 照合順序(collating sequence)が指定されている場合は、その変換表を適用してから各バイトを比較する。
     *
     * @param s1 比較対象の1つ目のデータストレージ
     * @param s2 比較対象の2つ目のデータストレージ
     * @param size 比較するバイト数
     * @param col 照合順序の変換表。nullの場合は変換せずバイト値で比較する
     * @return s1がs2より小さい場合は負、等しい場合は0、大きい場合は正の値
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

    // libcob/common.cのnational_cmpsの実装
    /**
     * 2つの日本語(NATIONAL)データを指定したバイト数だけ比較する。<br>
     * 2バイトを1文字として上位バイト・下位バイトを結合した値同士で比較する。
     *
     * @param s1 比較対象の1つ目のデータストレージ
     * @param s2 比較対象の2つ目のデータストレージ
     * @param size 比較するバイト数
     * @param col 照合順序の変換表(現状の実装では使用しない)
     * @return s1がs2より小さい場合は負、等しい場合は0、大きい場合は正の値
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

    /** 行トレース(READY TRACE)を有効にする。以降の実行位置がトレース出力される。 */
    public static void readyTrace() {
        runUnitState.get().lineTrace = true;
    }

    /** 行トレース(RESET TRACE)を無効にする。 */
    public static void resetTrace() {
        runUnitState.get().lineTrace = false;
    }

    /**
     * 致命的エラーを報告し、ランタイムの実行を停止する。<br>
     * 指定されたエラー種別に応じたメッセージをランタイムエラーとして出力した後、実行を停止する。
     *
     * @param fatalError 致命的エラーの種別。{@link #FERROR_INITIALIZED}、{@link #FERROR_CODEGEN}、{@link
     *     #FERROR_CHAINING}、{@link #FERROR_STACK}のいずれか
     * @throws CobolStopRunException 常にランタイムを停止するためにスローされる
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
     * 現在の実行位置(プログラムID、ソースファイル、行番号、セクション、段落、文)を設定する。<br>
     * これらの情報はランタイムエラー出力やトレースに利用される。<br>
     * 行トレースが有効な場合は、プログラムID・行番号・文をトレースとして標準エラー出力へ出力する。
     *
     * @param progId 現在のプログラムID
     * @param sfile 現在のソースファイル名
     * @param sline 現在のソース行番号
     * @param csect 現在のセクション名
     * @param cpara 現在の段落(パラグラフ)名
     * @param cstatement 現在の文(ステートメント)の名称。nullの場合は更新しない
     */
    public static void setLocation(
            String progId, String sfile, int sline, String csect, String cpara, String cstatement) {
        RunUnitState st = runUnitState.get();
        st.sourceFile = sfile;
        st.sourceLine = sline;
        st.currProgramId = progId;
        st.currSection = csect;
        st.currParagraph = cpara;
        if (cstatement != null) {
            st.sourceStatement = cstatement;
        }
        if (st.lineTrace) {
            System.err.println(
                    String.format(
                            "PROGRAM-ID: %s \tLine: %d \tStatement: %s",
                            progId, sline, cstatement == null ? "Unknown" : cstatement));
            System.err.flush();
        }
    }

    /**
     * 環境変数の値を取得する。<br>
     * まず{@link #setEnv}で設定された内部テーブルを参照し、存在しない場合はシステムの環境変数を参照する。
     *
     * @param envVarName 取得する環境変数名
     * @return 環境変数の値。存在しない場合はnull
     */
    public static String getEnv(String envVarName) {
        String envVarInTable = runUnitState.get().envVarTable.getProperty(envVarName);
        if (envVarInTable != null) {
            return envVarInTable;
        } else {
            return System.getenv(envVarName);
        }
    }

    /**
     * get environemnt variable
     *
     * @param envVarName the name of an environment variable.
     * @param envVarValue the value to be set to the environment variable.
     */
    public static void setEnv(String envVarName, String envVarValue) {
        runUnitState.get().envVarTable.setProperty(envVarName, envVarValue);
    }

    /**
     * Set environemnt variable
     *
     * @param envVarName the name of an environment variable. The leading and trailing spaces are
     *     ignored.
     * @param envVarValue the value of an environment variable to be set.
     */
    public static void setEnv(AbstractCobolField envVarName, AbstractCobolField envVarValue) {
        runUnitState
                .get()
                .envVarTable
                .setProperty(envVarName.getString().trim(), envVarValue.getString());
    }

    /**
     * 文字列をSHIFT_JISエンコードのバイト配列に変換する。
     *
     * @param s 変換する文字列
     * @return SHIFT_JISでエンコードしたバイト配列
     */
    public static byte[] stringToBytes(String s) {
        return s.getBytes(AbstractCobolField.charSetSJIS);
    }

    /**
     * 可変長引数で渡されたバイト列をバイト配列として返す。
     *
     * @param bytes バイト配列に変換するバイト列
     * @return 渡されたバイト列からなるバイト配列
     */
    public static byte[] toBytes(byte... bytes) {
        return bytes;
    }

    /**
     * 現在実行中のプログラムIDを取得する。
     *
     * @return 現在のプログラムID
     */
    public static String getCurrProgramId() {
        return runUnitState.get().currProgramId;
    }

    /**
     * 現在実行中のセクション名を取得する。
     *
     * @return 現在のセクション名
     */
    public static String getCurrSection() {
        return runUnitState.get().currSection;
    }

    /**
     * 現在実行中の段落(パラグラフ)名を取得する。
     *
     * @return 現在の段落名
     */
    public static String getCurrParagraph() {
        return runUnitState.get().currParagraph;
    }

    /**
     * 現在実行中のソース行番号を取得する。
     *
     * @return 現在のソース行番号
     */
    public static int getSourceLine() {
        return runUnitState.get().sourceLine;
    }

    /**
     * 現在実行中の文(ステートメント)の名称を取得する。
     *
     * @return 現在の文の名称
     */
    public static String getSourceStatement() {
        return runUnitState.get().sourceStatement;
    }

    /**
     * コマンドライン引数からCHAININGパラメータを設定する。
     *
     * @param data CHAININGパラメータのデータストレージ
     * @param parm パラメータのインデックス (1始まり)
     * @param size データストレージのサイズ
     */
    public static void chainSetUp(CobolDataStorage data, int parm, int size) {
        data.memset((byte) ' ', size);
        // C版ではcob_argvのインデックス0にプログラム名が含まれるため、cob_argv[parm]で引数を取得する。
        // Javaではcommand_lineArgsにプログラム名が含まれないため、parm - 1を使用する。
        int index = parm - 1;
        if (CobolUtil.commandLineArgs != null && index < CobolUtil.commandLineArgs.length) {
            byte[] argBytes =
                    CobolUtil.commandLineArgs[index].getBytes(AbstractCobolField.charSetSJIS);
            int len = argBytes.length;
            if (len <= size) {
                data.memcpy(argBytes, len);
            } else {
                data.memcpy(argBytes, size);
            }
        }
        // C版ではcob_call_params = cob_argc - 1 (プログラム名を除外)。
        // JavaではcommandLineArgs.lengthが既にプログラム名を除外した値になっている。
        CobolCallParams.set(
                (CobolUtil.commandLineArgs != null) ? CobolUtil.commandLineArgs.length : 0);
    }
}

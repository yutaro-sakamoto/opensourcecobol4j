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
package jp.osscons.opensourcecobol.libcobj.file;

import java.io.IOException;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.channels.NonWritableChannelException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import jp.osscons.opensourcecobol.libcobj.common.CobolModule;
import jp.osscons.opensourcecobol.libcobj.common.CobolUtil;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolExceptionId;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;

/** INDEXED, RELATIVE, SEQUENTIAL, LINE SEQUENTIAL等のCOBOLの ファイルを実装するための基底クラス */
public class CobolFile {
    /** ファイル編成: 順編成 (ORGANIZATION IS SEQUENTIAL) */
    protected static final int COB_ORG_SEQUENTIAL = 0;

    /** ファイル編成: 行順編成 (ORGANIZATION IS LINE SEQUENTIAL) */
    protected static final int COB_ORG_LINE_SEQUENTIAL = 1;

    /** ファイル編成: 相対編成 (ORGANIZATION IS RELATIVE) */
    protected static final int COB_ORG_RELATIVE = 2;

    /** ファイル編成: 索引編成 (ORGANIZATION IS INDEXED) */
    protected static final int COB_ORG_INDEXED = 3;

    /** ファイル編成: ソート用 (SORT文で使用) */
    protected static final int COB_ORG_SORT = 4;

    /** ファイル編成の最大値 */
    protected static final int COB_ORG_MAX = 5;

    /** アクセスモード: 順次アクセス (ACCESS MODE IS SEQUENTIAL) */
    protected static final int COB_ACCESS_SEQUENTIAL = 1;

    /** アクセスモード: 動的アクセス (ACCESS MODE IS DYNAMIC) */
    protected static final int COB_ACCESS_DYNAMIC = 2;

    /** アクセスモード: 乱アクセス (ACCESS MODE IS RANDOM) */
    protected static final int COB_ACCESS_RANDOM = 3;

    /** 入出力操作種別: OPEN */
    protected static final int COB_IO_OPEN = 0;

    /** 入出力操作種別: READ */
    protected static final int COB_IO_READ = 1;

    /** 入出力操作種別: WRITE */
    protected static final int COB_IO_WRITE = 2;

    /** 入出力操作種別: CLOSE */
    protected static final int COB_IO_CLOSE = 3;

    /** 入出力操作種別: DELETE (レコード削除) */
    protected static final int COB_IO_DELETE = 4;

    /** 入出力操作種別: REWRITE */
    protected static final int COB_IO_REWRITE = 5;

    /** 入出力操作種別: START (位置決め) */
    protected static final int COB_IO_START = 6;

    /** 入出力操作種別: COMMIT (トランザクションコミット) */
    protected static final int COB_IO_COMMIT = 7;

    /** 入出力操作種別: ROLLBACK (トランザクションロールバック) */
    protected static final int COB_IO_ROLLBACK = 8;

    /** 入出力操作種別: UNLOCK (レコードロック解除) */
    protected static final int COB_IO_UNLOCK = 9;

    /** 入出力操作種別: DELETE FILE (ファイル削除) */
    protected static final int COB_IO_DELETE_FILE = 10;

    /** オープンモード: クローズ状態 */
    public static final int COB_OPEN_CLOSED = 0;

    /** オープンモード: 入力 (OPEN INPUT) */
    public static final int COB_OPEN_INPUT = 1;

    /** オープンモード: 出力 (OPEN OUTPUT) */
    public static final int COB_OPEN_OUTPUT = 2;

    /** オープンモード: 入出力 (OPEN I-O) */
    public static final int COB_OPEN_I_O = 3;

    /** オープンモード: 追記 (OPEN EXTEND) */
    public static final int COB_OPEN_EXTEND = 4;

    /** オープンモード: ロック状態 (CLOSE WITH LOCK後) */
    public static final int COB_OPEN_LOCKED = 5;

    /** クローズオプション: 通常クローズ */
    public static final int COB_CLOSE_NORMAL = 0;

    /** クローズオプション: ロック付きクローズ (CLOSE WITH LOCK) */
    public static final int COB_CLOSE_LOCK = 1;

    /** クローズオプション: 巻き戻しなしクローズ (CLOSE WITH NO REWIND) */
    public static final int COB_CLOSE_NO_REWIND = 2;

    /** クローズオプション: ユニットクローズ */
    public static final int COB_CLOSE_UNIT = 3;

    /** クローズオプション: ユニット削除付きクローズ */
    public static final int COB_CLOSE_UNIT_REMOVAL = 4;

    /** WRITEオプション: 行数マスク（下位16ビット） */
    public static final int COB_WRITE_MASK = 0x0000ffff;

    /** WRITEオプション: 行単位で進める (ADVANCING n LINES) */
    public static final int COB_WRITE_LINES = 0x00010000;

    /** WRITEオプション: ページ単位で進める (ADVANCING PAGE) */
    public static final int COB_WRITE_PAGE = 0x00020000;

    /** WRITEオプション: チャネル指定 */
    public static final int COB_WRITE_CHANNEL = 0x00040000;

    /** WRITEオプション: 書き込み後に進める (AFTER ADVANCING) */
    public static final int COB_WRITE_AFTER = 0x00100000;

    /** WRITEオプション: 書き込み前に進める (BEFORE ADVANCING) */
    public static final int COB_WRITE_BEFORE = 0x00200000;

    /** WRITEオプション: ページ終端チェック (AT END-OF-PAGE) */
    public static final int COB_WRITE_EOP = 0x00400000;

    /** WRITEオプション: レコードロック付き書き込み */
    public static final int COB_WRITE_LOCK = 0x00800000;

    /** READオプション: 次レコード読み込み (READ NEXT) */
    public static final int COB_READ_NEXT = 0x01;

    /** READオプション: 前レコード読み込み (READ PREVIOUS) */
    public static final int COB_READ_PREVIOUS = 0x2;

    /** READオプション: 先頭レコード読み込み (READ FIRST) */
    public static final int COB_READ_FIRST = 0x04;

    /** READオプション: 末尾レコード読み込み (READ LAST) */
    public static final int COB_READ_LAST = 0x08;

    /** READオプション: レコードロック付き読み込み (READ WITH LOCK) */
    public static final int COB_READ_LOCK = 0x10;

    /** READオプション: ロックなし読み込み (READ WITH NO LOCK) */
    public static final int COB_READ_NO_LOCK = 0x20;

    /** READオプション: ロック保持読み込み (READ WITH KEPT LOCK) */
    public static final int COB_READ_KEPT_LOCK = 0x40;

    /** READオプション: ロック待機読み込み (READ WITH WAIT) */
    public static final int COB_READ_WAIT_LOCK = 0x80;

    /** READオプション: ロック無視読み込み (READ WITH IGNORE LOCK) */
    public static final int COB_READ_IGNORE_LOCK = 0x100;

    /** 環境変数名: ユーザー定義ファイルハンドラ */
    protected static final String TIS_DEFINE_USERFH = "OC_USERFH";

    /** 環境変数名: I-Oモードでファイル作成を許可 */
    protected static final String COB_IO_CREATES = "OC_IO_CREATES";

    /** 環境変数名: EXTENDモードでファイル作成を許可 */
    protected static final String COB_EXTEND_CREATES = "OC_EXTEND_CREATES";

    /** ファイルステータス 00: 正常終了 */
    protected static final int COB_STATUS_00_SUCCESS = 0;

    /** ファイルステータス 02: 正常終了（重複キー） */
    protected static final int COB_STATUS_02_SUCCESS_DUPLICATE = 2;

    /** ファイルステータス 04: 正常終了（レコード長不完全） */
    protected static final int COB_STATUS_04_SUCCESS_INCOMPLETE = 4;

    /** ファイルステータス 05: 正常終了（OPTIONALファイルが存在しない） */
    protected static final int COB_STATUS_05_SUCCESS_OPTIONAL = 5;

    /** ファイルステータス 07: 正常終了（ユニットなし） */
    protected static final int COB_STATUS_07_SUCCESS_NO_UNIT = 7;

    /** ファイルステータス 10: ファイル終端 */
    protected static final int COB_STATUS_10_END_OF_FILE = 10;

    /** ファイルステータス 14: キー範囲外 */
    protected static final int COB_STATUS_14_OUT_OF_KEY_RANGE = 14;

    /** ファイルステータス 21: キー順序不正 */
    protected static final int COB_STATUS_21_KEY_INVALID = 21;

    /** ファイルステータス 22: キー重複 */
    protected static final int COB_STATUS_22_KEY_EXISTS = 22;

    /** ファイルステータス 23: キーが存在しない */
    protected static final int COB_STATUS_23_KEY_NOT_EXISTS = 23;

    /** ファイルステータス 30: 永続エラー */
    protected static final int COB_STATUS_30_PERMANENT_ERROR = 30;

    /** ファイルステータス 31: ファイル名不整合 */
    protected static final int COB_STATUS_31_INCONSISTENT_FILENAME = 31;

    /** ファイルステータス 34: 境界違反 */
    protected static final int COB_STATUS_34_BOUNDARY_VIOLATION = 34;

    /** ファイルステータス 35: ファイルが存在しない */
    protected static final int COB_STATUS_35_NOT_EXISTS = 35;

    /** ファイルステータス 37: アクセス権限なし */
    protected static final int COB_STATUS_37_PERMISSION_DENIED = 37;

    /** ファイルステータス 38: ロック付きでクローズ済み */
    protected static final int COB_STATUS_38_CLOSED_WITH_LOCK = 38;

    /** ファイルステータス 39: 属性競合 */
    protected static final int COB_STATUS_39_CONFLICT_ATTRIBUTE = 39;

    /** ファイルステータス 41: ファイルは既にオープン済み */
    protected static final int COB_STATUS_41_ALREADY_OPEN = 41;

    /** ファイルステータス 42: ファイルがオープンされていない */
    protected static final int COB_STATUS_42_NOT_OPEN = 42;

    /** ファイルステータス 43: READが先行していない */
    protected static final int COB_STATUS_43_READ_NOT_DONE = 43;

    /** ファイルステータス 44: レコード長オーバーフロー */
    protected static final int COB_STATUS_44_RECORD_OVERFLOW = 44;

    /** ファイルステータス 46: READ失敗 */
    protected static final int COB_STATUS_46_READ_ERROR = 46;

    /** ファイルステータス 47: READ/START不可（オープンモード不正） */
    protected static final int COB_STATUS_47_INPUT_DENIED = 47;

    /** ファイルステータス 48: WRITE不可（オープンモード不正） */
    protected static final int COB_STATUS_48_OUTPUT_DENIED = 48;

    /** ファイルステータス 49: DELETE/REWRITE不可（オープンモード不正） */
    protected static final int COB_STATUS_49_I_O_DENIED = 49;

    /** ファイルステータス 51: レコードロック中 */
    protected static final int COB_STATUS_51_RECORD_LOCKED = 51;

    /** ファイルステータス 52: ページ終端 (END-OF-PAGE) */
    protected static final int COB_STATUS_52_EOP = 52;

    /** ファイルステータス 57: LINAGEエラー */
    protected static final int COB_STATUS_57_I_O_LINAGE = 57;

    /** ファイルステータス 61: ファイル共有競合 */
    protected static final int COB_STATUS_61_FILE_SHARING = 61;

    /** ファイルステータス 91: 機能が利用不可 */
    protected static final int COB_STATUS_91_NOT_AVAILABLE = 91;

    /**
     * File status 92: Version incompatibility.
     * Indicates that the file operation failed due to a version mismatch between the file and the program.
     */
    protected static final int COB_STATUS_92_VERSION_INCOMPATIBLE = 92;

    // ==============================================
    // The following constants must not be equal
    // to any of the above constants `COB_STATUS_*`

    /** エラーコード: ファイルまたはディレクトリが存在しない */
    protected static final int ENOENT = 1002;

    /** エラーコード: 不正なファイル記述子 */
    protected static final int EBADF = 1009;

    /** エラーコード: アクセス権限なし */
    protected static final int EACCESS = 1013;

    /** エラーコード: ディレクトリである */
    protected static final int EISDIR = 1021;

    /** エラーコード: 読み取り専用ファイルシステム */
    protected static final int EROFS = 1030;

    /** エラーコード: リソースが一時的に利用不可 */
    protected static final int EAGAIN = 1011;

    // ==============================================

    /** 内部エラー: LINAGE値不正 */
    protected static final int COB_LINAGE_INVALID = 16384;

    /** 内部エラー: 設定されていない */
    protected static final int COB_NOT_CONFIGURED = 32768;

    /** SELECT句オプション: FILE STATUS指定あり */
    public static final int COB_SELECT_FILE_STATUS = 0x01;

    /** SELECT句オプション: EXTERNAL指定あり */
    public static final int COB_SELECT_EXTERNAL = 0x02;

    /** SELECT句オプション: LINAGE指定あり */
    public static final int COB_SELECT_LINAGE = 0x04;

    /** SELECT句オプション: 分割キー指定あり */
    public static final int COB_SELECT_SPLITKEY = 0x08;

    /** ファイルステータスのサイズ */
    protected static final int FNSTATUSSIZE = 3;

    /** 最後にエラーが発生したファイル */
    public static CobolFile errorFile;

    /** 小バッファサイズ */
    protected static int COB_SMALL_BUFF = 1024;

    /** 小バッファ最大インデックス */
    protected static int COB_SMALL_MAX = COB_SMALL_BUFF - 1;

    /** ロックモード: 排他ロック */
    protected static final int COB_LOCK_EXCLUSIVE = 1;

    /** ロックモード: 手動ロック */
    protected static final int COB_LOCK_MANUAL = 2;

    /** ロックモード: 自動ロック */
    protected static final int COB_LOCK_AUTOMATIC = 4;

    /** ロックモード: 複数レコードロック */
    protected static final int COB_LOCK_MULTIPLE = 8;

    /** ロックモードマスク */
    protected static final int COB_LOCK_MASK = 0x7;

    /** 環境変数COB_FILE_PATHの値（ファイル検索パス） */
    protected static String cob_file_path = null;

    /** 環境変数COB_LS_NULLSの値（LINE SEQUENTIALでのNULL文字処理） */
    protected static String cob_ls_nulls = null;

    /** 環境変数COB_LS_FIXEDの値（LINE SEQUENTIALでの固定長レコード処理） */
    protected static String cob_ls_fixed = null;

    /** ファイルオープン時の環境変数展開用バッファ */
    protected static byte[] file_open_env = new byte[1024];

    /** オープン対象のファイル名 */
    protected static String file_open_name;

    /** ファイルオープン時のバッファ */
    protected static byte[] file_open_buff = new byte[1024];

    /** ファイル名検索時のプレフィックス一覧（DD_xxx, dd_xxx, xxx） */
    protected static final String[] prefix = {"DD_", "dd_", ""};

    /** プレフィックスの数 */
    protected static final int NUM_PREFIX = prefix.length;

    /** ページ終端(EOP)ステータス */
    protected static int eop_status = 0;

    /** 同期書き込みフラグ（0=無効、1=flush、2=fsync） */
    protected static int cob_do_sync = 0;

    private static List<CobolFile> file_cache = new ArrayList<CobolFile>();

    /** ファイルステータスコードから例外IDへのマッピング配列（10で割った値をインデックスとして使用） */
    protected static int[] status_exception = {
        0,
        CobolExceptionId.COB_EC_I_O_AT_END,
        CobolExceptionId.COB_EC_I_O_INVALID_KEY,
        CobolExceptionId.COB_EC_I_O_PERMANENT_ERROR,
        CobolExceptionId.COB_EC_I_O_LOGIC_ERROR,
        CobolExceptionId.COB_EC_I_O_RECORD_OPERATION,
        CobolExceptionId.COB_EC_I_O_FILE_SHARING,
        CobolExceptionId.COB_EC_I_O,
        CobolExceptionId.COB_EC_I_O,
        CobolExceptionId.COB_EC_I_O_IMP
    };

    /** SELECT句で指定されたファイル名 */
    public String select_name;

    /** ファイルステータス（2バイト: "00"～"99"） */
    public byte[] file_status;

    /** ASSIGN句で指定されたファイル名フィールド */
    protected AbstractCobolField assign;

    /** レコード領域 */
    public AbstractCobolField record;

    /** レコードサイズフィールド（可変長レコードの場合） */
    protected AbstractCobolField record_size;

    /** キー情報の配列（INDEXED/RELATIVEファイル用） */
    protected CobolFileKey[] keys;

    /** ファイルI/Oハンドラ */
    public FileIO file;

    /** ソート処理用構造体（SORT文で使用） */
    protected CobolSort filex;

    /** 索引ファイル用構造体（INDEXEDファイルで使用） */
    protected IndexedFile filei;

    /** LINAGE情報（LINAGE句使用時） */
    protected Linage linorkeyptr;

    /** ソート時の照合順序 */
    protected CobolDataStorage sort_collating;

    /** 外部ファイルハンドラへのポインタ */
    protected Object extfh_ptr;

    /** 最小レコード長 */
    protected int record_min;

    /** 最大レコード長 */
    public int record_max;

    /** キー数（INDEXED/RELATIVEファイル用） */
    protected int nkeys;

    /** ファイル編成（COB_ORG_*） */
    protected char organization;

    /** アクセスモード（COB_ACCESS_*） */
    protected char access_mode;

    /** ロックモード（COB_LOCK_*） */
    protected char lock_mode;

    /** 現在のオープンモード（COB_OPEN_*） */
    protected char open_mode;

    /** OPTIONALファイルかどうか */
    protected boolean flag_optional;

    /** 最後に使用されたオープンモード */
    public char last_open_mode;

    /** 特殊ファイル（1=標準入力、2=標準出力） */
    protected char special;

    /** ファイルが存在しないフラグ */
    protected boolean flag_nonexistent;

    /** ファイル終端フラグ */
    protected boolean flag_end_of_file;

    /** ファイル先頭フラグ */
    protected boolean flag_begin_of_file;

    /** 最初の読み込みフラグ */
    protected char flag_first_read;

    /** 読み込み完了フラグ（REWRITE/DELETE前に必要） */
    protected boolean flag_read_done;

    /** SELECT句の機能フラグ（COB_SELECT_*の組み合わせ） */
    public char flag_select_features;

    /** 改行が必要なフラグ（LINE SEQUENTIAL用） */
    protected boolean flag_needs_nl;

    /** ページ先頭処理が必要なフラグ（LINAGE用） */
    protected boolean flag_needs_top;

    /** ファイルバージョン */
    protected char file_version;

    /** ランタイムバッファ */
    protected static String runtime_buffer;

    /** ファイル名 */
    protected static String name;

    /** ステータス */
    protected static byte[] status;

    /**
     * LINAGE情報を取得する
     *
     * @return LINAGE情報
     */
    public Linage getLinorkeyptr() {
        return this.linorkeyptr;
    }

    /**
     * LINAGE情報を設定する
     *
     * @param ptr LINAGE情報
     */
    public void setLinorkeyptr(Linage ptr) {
        this.linorkeyptr = ptr;
    }

    /** デフォルトコンストラクタ */
    public CobolFile() {}

    /**
     * 指定されたパラメータでCobolFileインスタンスを生成する。
     *
     * @param selectName ファイルのSELECT名（COBOL SELECT句で指定された名前）
     * @param fileStatus ファイルステータスを格納するバイト配列（4バイト）
     * @param assign ASSIGN句で指定されたファイル名フィールド
     * @param record レコード領域を表すフィールド
     * @param recordSize 可変長レコードの場合のレコード長フィールド（固定長の場合はnull）
     * @param recordMin 最小レコード長
     * @param recordMax 最大レコード長
     * @param nkeys キーの数（INDEXED/RELATIVEファイルの場合）
     * @param keys キー情報の配列
     * @param organization ファイル編成（COB_ORG_SEQUENTIAL等）
     * @param accessMode アクセスモード（COB_ACCESS_SEQUENTIAL等）
     * @param lockMode ロックモード
     * @param openMode 現在のオープンモード
     * @param flagOptional OPTIONALファイルかどうか
     * @param lastOpenMode 最後のオープンモード
     * @param special 特殊ファイルフラグ
     * @param flagNonexistent ファイルが存在しないかどうか
     * @param flagEndOfFile ファイル終端に達したかどうか
     * @param flagBeginOfFile ファイル先頭にいるかどうか
     * @param flagFirstRead 最初の読み取りかどうか
     * @param flagReadDone 読み取りが完了したかどうか
     * @param flagSelectFeatures SELECT機能フラグ
     * @param flagNeedsNl 改行が必要かどうか
     * @param flagNeedsTop ページトップが必要かどうか
     * @param fileVersion ファイルバージョン
     */
    public CobolFile(
            String selectName,
            byte[] fileStatus,
            AbstractCobolField assign,
            AbstractCobolField record,
            AbstractCobolField recordSize,
            int recordMin,
            int recordMax,
            int nkeys,
            CobolFileKey[] keys,
            char organization,
            char accessMode,
            char lockMode,
            char openMode,
            boolean flagOptional,
            char lastOpenMode,
            char special,
            boolean flagNonexistent,
            boolean flagEndOfFile,
            boolean flagBeginOfFile,
            char flagFirstRead,
            boolean flagReadDone,
            char flagSelectFeatures,
            boolean flagNeedsNl,
            boolean flagNeedsTop,
            char fileVersion) {

        this.select_name = selectName;
        this.file_status = fileStatus;
        this.assign = assign;
        this.record = record;
        this.record_size = recordSize;
        this.record_min = recordMin;
        this.record_max = recordMax;
        this.nkeys = nkeys;
        this.keys = keys;
        this.file = new FileIO();
        this.organization = organization;
        this.access_mode = accessMode;
        this.lock_mode = lockMode;
        this.open_mode = openMode;
        this.flag_optional = flagOptional;
        this.special = special;
        this.flag_nonexistent = flagNonexistent;
        this.flag_end_of_file = flagEndOfFile;
        this.flag_begin_of_file = flagBeginOfFile;
        this.flag_first_read = flagFirstRead;
        this.flag_read_done = flagReadDone;
        this.flag_select_features = flagSelectFeatures;
        this.flag_needs_nl = flagNeedsNl;
        this.flag_needs_top = flagNeedsTop;
        this.file_version = fileVersion;
    }

    // libcob/fileio.cのsave_statusの実装 RETURN_STATUSマクロは実装できないため,本メソッドの呼び出し後の次の文はreturn;を書くこと.
    /**
     * ファイルステータスを保存する（libcob/fileio.cのsave_statusの実装）。
     *
     * @param status ファイルステータスコード（COB_STATUS_00_SUCCESS等）
     * @param fnstatus ステータスを格納するフィールド（nullの場合は格納しない）
     */
    protected void saveStatus(int status, AbstractCobolField fnstatus) {
        CobolFile.errorFile = this;
        if (status == 0) {
            this.file_status[0] = '0';
            this.file_status[1] = '0';
            if (fnstatus != null) {
                fnstatus.getDataStorage().setByte(0, (byte) '0');
                fnstatus.getDataStorage().setByte(1, (byte) '0');
            }
            CobolRuntimeException.code = 0;
            return;
        }

        if (status != COB_STATUS_52_EOP) {
            CobolRuntimeException.setException(status_exception[status / 10]);
        }
        this.file_status[0] = (byte) (status / 10 + '0');
        this.file_status[1] = (byte) (status % 10 + '0');
        if (fnstatus != null) {
            fnstatus.getDataStorage().setByte(0, this.file_status[0]);
            fnstatus.getDataStorage().setByte(1, this.file_status[1]);
        }
    }

    // libcob/fileio.のcob_invoke_funの実装
    /**
     * ファイル操作関数を呼び出す（libcob/fileio.cのcob_invoke_funの実装）。
     *
     * @param operate 操作種別
     * @param f ファイルオブジェクト
     * @param key キーフィールド
     * @param rec レコードストレージ
     * @param fnstatus ファイルステータスフィールド
     * @param openMode オープンモード
     * @param startCond START条件
     * @param readOpts 読み取りオプション
     * @return ステータスコード
     */
    public static int invokeFun(
            int operate,
            Object f,
            AbstractCobolField key,
            CobolDataStorage rec,
            AbstractCobolField fnstatus,
            String openMode,
            String startCond,
            String readOpts) {
        return 0;
    }

    // libcob/cob_cache_fileのj実装
    /**
     * ファイルをキャッシュに追加する（プログラム終了時のクリーンアップ用）。
     *
     * @param f キャッシュに追加するファイル
     */
    protected static void cacheFile(CobolFile f) {
        if (file_cache.contains(f)) {
            return;
        }
        file_cache.add(f);
    }

    // libcob/fileio.cのcob_file_linage_checkの実装 TODO 実装
    /**
     * LINAGE句のパラメータを検証する。
     *
     * @return 検証エラーがある場合はtrue、正常な場合はfalse
     */
    protected boolean file_linage_check() {
        Linage lingptr = getLinorkeyptr();
        lingptr.setLinLines(lingptr.getLinage().getInt());

        outer:
        {
            if (lingptr.getLinLines() < 1) {
                break outer;
            }
            if (lingptr.getLatfoot() != null) {
                lingptr.setLinFoot(lingptr.getLatfoot().getInt());
                if (lingptr.getLinFoot() < 1 || lingptr.getLinFoot() > lingptr.getLinLines()) {
                    break outer;
                }
            } else {
                lingptr.setLinFoot(0);
            }
            if (lingptr.getLattop() != null) {
                lingptr.setLinTop(lingptr.getLattop().getInt());
                if (lingptr.getLinTop() < 0) {
                    break outer;
                }
            } else {
                lingptr.setLinTop(0);
            }
            if (lingptr.getLatbot() != null) {
                lingptr.setLinBot(lingptr.getLatbot().getInt());
                if (lingptr.getLinBot() < 0) {
                    break outer;
                }
            } else {
                lingptr.setLinBot(0);
            }
            return false;
        }

        lingptr.getLinageCtr().setInt(0);
        return true;
    }

    // libcob/fileio.cのcob_linage_write_optの実装 TODO 実装
    /**
     * LINAGE句に基づいてWRITE操作のオプション処理を行う。
     *
     * @param opt WRITE操作オプション（WRITE BEFORE/AFTER等）
     * @return ファイルステータスコード
     * @throws CobolStopRunException 致命的なエラーが発生した場合
     */
    protected int linage_write_opt(int opt) throws CobolStopRunException {
        int i, n;
        Linage lingptr = this.getLinorkeyptr();

        if ((opt & COB_WRITE_PAGE) != 0) {
            i = lingptr.getLinageCtr().getInt();
            if (i == 0) {
                return COB_STATUS_57_I_O_LINAGE;
            }
            n = lingptr.getLinLines();
            for (; i < n; ++i) {
                this.file.putc((byte) '\n');
            }
            for (i = 0; i < lingptr.getLinBot(); i++) {
                this.file.putc((byte) '\n');
            }
            if (this.file_linage_check()) {
                return COB_STATUS_57_I_O_LINAGE;
            }
            for (i = 0; i < lingptr.getLinTop(); i++) {
                this.file.putc((byte) '\n');
            }
            lingptr.getLinageCtr().setInt(1);
        } else if ((opt & COB_WRITE_LINES) != 0) {
            n = lingptr.getLinageCtr().getInt();
            if (n == 0) {
                return COB_STATUS_57_I_O_LINAGE;
            }
            lingptr.getLinageCtr().addInt(opt & COB_WRITE_MASK);
            i = lingptr.getLinageCtr().getInt();
            if ((opt & COB_WRITE_EOP) != 0 && lingptr.getLinFoot() != 0) {
                if (i >= lingptr.getLinFoot()) {
                    eop_status = 1;
                }
            }
            if (i > lingptr.getLinLines()) {
                if ((opt & COB_WRITE_EOP) != 0) {
                    eop_status = 1;
                }
                for (; n < lingptr.getLinLines(); n++) {
                    this.file.putc((byte) '\n');
                }
                for (i = 0; i < lingptr.getLinBot(); i++) {
                    this.file.putc((byte) '\n');
                }
                if (this.file_linage_check()) {
                    return COB_STATUS_57_I_O_LINAGE;
                }
                lingptr.getLinageCtr().setInt(1);
                for (i = 0; i < lingptr.getLinTop(); i++) {
                    this.file.putc((byte) '\n');
                }
            } else {
                for (i = (opt & COB_WRITE_MASK) - 1; i > 0; i--) {
                    this.file.putc((byte) '\n');
                }
            }
        }
        return 0;
    }

    /**
     * JIS漢字コードを含むファイル名を変換する。
     *
     * @param name 変換元のファイル名
     * @param jbuf 出力バッファ
     * @param n バッファサイズ
     * @return 変換後のバイト配列
     */
    protected byte[] cb_get_jisword_buff(byte[] name, byte[] jbuf, int n) {
        int cs = 0;
        int ce = name.length - 1;
        int cp;
        boolean flagQuoted = false;
        int siz = 0;
        byte[] rt = null;
        int c;
        int p = 0;

        if (name[cs] == '\'' && name[ce] == '\'') {
            cs++;
            --ce;
            flagQuoted = true;
        }

        if (ce - cs >= 5
                && !(name[cs] == '_' && name[cs + 1] == '_' && name[cs + 2] == '_')
                && !(name[ce - 2] == '_' && name[ce - 1] == '_' && name[ce] == '_')) {
            cs += 3;
            ce -= 3;
            if (!flagQuoted) {
                siz = (ce - cs + 1) / 2 + 1;
            } else {
                siz = (ce - cs + 1) / 2 + 3;
            }
            if (jbuf != null) {
                rt = new byte[siz];
            } else {
                if (siz > n) {
                    c = siz - n;
                    siz -= c;
                    ce -= c * 2;
                }
                /*
                 * TODO fix
                 * for (int i = 0; i < n; ++i) {
                 * jbuf[i] = 0;
                 * }
                 */
                // TODO
                rt = null;
            }
            if (flagQuoted && siz > 2) {
                rt[0] = rt[siz - 2] = (byte) '\'';
                p = 1;
            } else {
                p = 0;
            }
            for (c = 0, cp = cs; cp <= ce; cp++, p += (c == 0 ? 1 : 0)) {
                if (name[cp] >= '0' && name[cp] <= '9') {
                    rt[p] |= (name[cp] - '0') << (c << 2);
                } else if (name[cp] >= 'A' && name[cp] <= 'F') {
                    rt[p] |= (name[cp] - 'A' + 10) << (c << 2);
                } else {
                    name[p] = '?';
                    cp += c;
                    c = 0;
                }
            }
        } else {
            if (jbuf != null) {
                rt = name.clone();
            } else {
                /*
                 * TODO fix
                 * for (int i = 0; i < n; ++i) {
                 * jbuf[i] = 0;
                 * }
                 * for (int i = 0; i < n - 1; ++i) {
                 * jbuf[i] = name[i];
                 * }
                 */
                // TODO fix
                rt = null;
            }
        }
        return rt;
    }

    // assert 0 <= openMode < 10
    private static String openModeToString(int openMode) {
        StringBuilder sb = new StringBuilder();
        return sb.append("0").append(openMode).toString();
    }

    private static String readOptsToString(int readOpts) {
        StringBuilder sb = new StringBuilder();
        if (readOpts < 10) {
            sb.append("0");
        }
        return sb.append(readOpts).toString();
    }

    private static String concatString(String... strs) {
        StringBuilder sb = new StringBuilder();
        for (String s : strs) {
            sb.append(s);
        }
        return sb.toString();
    }

    /**
     * This method is mainly for unlocking the indexed files.
     *
     * @return true if post-processing is successful, false otherwise.
     */
    protected boolean postProcess() {
        return true;
    }

    private void runPostProcess(AbstractCobolField fnstatus) {
        postProcess();
        // TODO: Implement error handling
        // boolean postProcessSucceeded = postProcess();
        // if(!postProcessSucceeded) {
        //    this.saveStatus(COB_STATUS_30_PERMANENT_ERROR, fnstatus);
        // }
    }

    /**
     * ファイルを開く（COBOL OPEN文の実行）。
     *
     * @param mode オープンモード（COB_OPEN_INPUT, COB_OPEN_OUTPUT, COB_OPEN_I_O, COB_OPEN_EXTEND）
     * @param sharing 共有モード
     * @param fnstatus ファイルステータスを格納するフィールド
     */
    public void open(int mode, int sharing, AbstractCobolField fnstatus) {
        String openMode = openModeToString(mode);
        if (invokeFun(COB_IO_OPEN, this, null, null, fnstatus, openMode, null, null) != 0) {
            this.last_open_mode = (char) Integer.parseInt(openMode);
            return;
        }
        this.last_open_mode = (char) Integer.parseInt(openMode);

        /* file was previously closed with lock */
        if (this.open_mode == COB_OPEN_LOCKED) {
            saveStatus(COB_STATUS_38_CLOSED_WITH_LOCK, fnstatus);
            return;
        }

        /* file is already open */
        if (((int) this.open_mode) != COB_OPEN_CLOSED) {
            saveStatus(COB_STATUS_41_ALREADY_OPEN, fnstatus);
            return;
        }

        this.last_open_mode = (char) mode;
        this.flag_nonexistent = false;
        this.flag_end_of_file = false;
        this.flag_begin_of_file = false;
        this.flag_first_read = 2;

        if (this.special != 0) {
            if (this.special == 1) {
                if (mode != COB_OPEN_INPUT) {
                    saveStatus(COB_STATUS_30_PERMANENT_ERROR, fnstatus);
                    return;
                }
                this.file.setIn(System.in);
                this.open_mode = (char) mode;
                this.saveStatus(COB_STATUS_00_SUCCESS, fnstatus);
                return;
            } else {
                if (mode != COB_OPEN_OUTPUT) {
                    this.saveStatus(COB_STATUS_30_PERMANENT_ERROR, fnstatus);
                    return;
                }
                this.file.setOut(System.out);
                this.open_mode = (char) mode;
                saveStatus(COB_STATUS_00_SUCCESS, fnstatus);
                return;
            }
        }

        /* obtain the file name */
        if (this.assign == null) {
            file_open_name = this.select_name;
        } else {
            file_open_name = this.assign.fieldToString();
        }

        byte[] src;
        byte[] dst;
        boolean simple;
        if (CobolModule.getCurrentModule().flag_filename_mapping != 0) {
            src = file_open_name.getBytes(AbstractCobolField.charSetSJIS);
            dst = file_open_buff;
            simple = true;
            int srcI = 0;
            int dstI = 0;
            while (srcI < src.length) {
                char c = (char) src[srcI];
                if (!Character.isLetterOrDigit(c) && c != '_' && c != '-') {
                    simple = false;
                }
                if (c == '$') {
                    int i;
                    for (i = 1; srcI + i < src.length; i++) {
                        char d = (char) src[srcI + i];
                        if (!Character.isLetterOrDigit(d) && d != '_' && c != '-') {
                            break;
                        }
                    }
                    for (int j = 0; j < i - 1; ++j) {
                        file_open_env[j] = src[srcI + 1 + j];
                    }
                    file_open_env[i - 1] = 0;
                    String p =
                            CobolUtil.getEnv(
                                    new String(
                                            Arrays.copyOfRange(file_open_env, 0, i - 1),
                                            AbstractCobolField.charSetSJIS));
                    if (p != null) {
                        byte[] pbytes = p.getBytes(AbstractCobolField.charSetSJIS);
                        for (int j = 0; j < pbytes.length; ++j) {
                            dst[dstI + j] = pbytes[j];
                        }
                        dstI += pbytes.length;
                    }
                    srcI += i;
                } else {
                    dst[dstI++] = src[srcI++];
                }
            }

            file_open_name =
                    new String(Arrays.copyOfRange(dst, 0, dstI), AbstractCobolField.charSetSJIS);

            byte[] fileOpenNameBytes = file_open_name.getBytes(AbstractCobolField.charSetSJIS);
            cb_get_jisword_buff(file_open_buff, fileOpenNameBytes, COB_SMALL_BUFF);

            if (simple) {
                int i;
                for (i = 0; i < NUM_PREFIX; i++) {
                    byte[] fileOpenBuff =
                            concatString(prefix[i], file_open_name)
                                    .getBytes(AbstractCobolField.charSetSJIS);
                    String p =
                            CobolUtil.getEnv(
                                    new String(fileOpenBuff, AbstractCobolField.charSetSJIS));
                    if (p != null) {
                        fileOpenNameBytes = p.getBytes(AbstractCobolField.charSetSJIS);
                        break;
                    }
                }

                if (i == NUM_PREFIX && cob_file_path != null) {
                    byte[] fileOpenBuff =
                            concatString(cob_file_path, "/", file_open_name)
                                    .getBytes(AbstractCobolField.charSetSJIS);
                    fileOpenNameBytes = fileOpenBuff;
                }
            }

            file_open_name = new String(fileOpenNameBytes, AbstractCobolField.charSetSJIS);
        }

        boolean wasNotExist = false;
        if (this.organization == COB_ORG_INDEXED) {
            if (!Files.exists(Paths.get(file_open_name))) {
                wasNotExist = true;
                if (mode != COB_OPEN_OUTPUT
                        && !this.flag_optional
                        && (mode != COB_OPEN_I_O || !"yes".equals(CobolUtil.getEnv(COB_IO_CREATES)))
                        && (mode != COB_OPEN_EXTEND
                                || !"yes".equals(CobolUtil.getEnv(COB_EXTEND_CREATES)))) {
                    saveStatus(COB_STATUS_35_NOT_EXISTS, fnstatus);
                    return;
                }
            }
        } else if (Files.notExists(Paths.get(file_open_name))) {
            wasNotExist = true;
            if (mode != COB_OPEN_OUTPUT
                    && !this.flag_optional
                    && (mode != COB_OPEN_I_O || CobolUtil.checkEnv(COB_IO_CREATES, "yes") == 0)
                    && (mode != COB_OPEN_EXTEND
                            || CobolUtil.checkEnv(COB_EXTEND_CREATES, "yes") == 0)) {
                saveStatus(COB_STATUS_35_NOT_EXISTS, fnstatus);
                return;
            }
        }
        cacheFile(this);

        try {
            switch (this.open_(file_open_name, mode, sharing)) {
                case 0:
                    this.open_mode = (char) mode;
                    if (this.flag_optional && wasNotExist) {
                        saveStatus(COB_STATUS_05_SUCCESS_OPTIONAL, fnstatus);
                        return;
                    } else {
                        saveStatus(COB_STATUS_00_SUCCESS, fnstatus);
                        return;
                    }
                case ENOENT:
                    if (mode == COB_OPEN_EXTEND || mode == COB_OPEN_OUTPUT) {
                        saveStatus(COB_STATUS_30_PERMANENT_ERROR, fnstatus);
                        return;
                    }
                    if (this.flag_optional) {
                        this.open_mode = (char) mode;
                        this.flag_nonexistent = true;
                        this.flag_end_of_file = true;
                        this.flag_begin_of_file = true;
                        saveStatus(COB_STATUS_05_SUCCESS_OPTIONAL, fnstatus);
                        return;
                    } else {
                        saveStatus(COB_STATUS_35_NOT_EXISTS, fnstatus);
                        return;
                    }
                case EACCESS:
                case EISDIR:
                case EROFS:
                    saveStatus(COB_STATUS_37_PERMISSION_DENIED, fnstatus);
                    return;
                case COB_STATUS_61_FILE_SHARING:
                    saveStatus(COB_STATUS_61_FILE_SHARING, fnstatus);
                    return;
                case COB_STATUS_91_NOT_AVAILABLE:
                    saveStatus(COB_STATUS_91_NOT_AVAILABLE, fnstatus);
                    return;
                case COB_STATUS_92_VERSION_INCOMPATIBLE:
                    saveStatus(COB_STATUS_92_VERSION_INCOMPATIBLE, fnstatus);
                    return;
                case COB_LINAGE_INVALID:
                    saveStatus(COB_STATUS_57_I_O_LINAGE, fnstatus);
                    return;
                default:
                    saveStatus(COB_STATUS_30_PERMANENT_ERROR, fnstatus);
                    return;
            }
        } catch (IOException e) {
            saveStatus(COB_STATUS_30_PERMANENT_ERROR, fnstatus);
            return;
        }
    }

    // protected long start;
    // protected long end;

    /**
     * ファイルを指定されたモードで拡張オープンする。 現在は未実装。
     *
     * @param mode オープンモード (COB_OPEN_INPUT, COB_OPEN_OUTPUT, COB_OPEN_I_O, COB_OPEN_EXTEND)
     * @param sharing 共有モード
     * @param fnstatus ファイルステータスを格納するフィールド
     */
    public void openEx(int mode, int sharing, AbstractCobolField fnstatus) {
        // this.open_("", mode, sharing);
    }

    /**
     * 指定されたファイル名でファイルをオープンする内部メソッド。 COBOL OPEN文の内部実装で、ファイルチャネルの確立、
     * ファイルロックの取得、LINAGE句のチェックを行う。
     *
     * @param filename オープンするファイルのパス
     * @param mode オープンモード (COB_OPEN_INPUT=1, COB_OPEN_OUTPUT=2, COB_OPEN_I_O=3, COB_OPEN_EXTEND=4)
     * @param sharing 共有モード (0: 共有ロック可, 非0: 排他ロック)
     * @return 0: 成功, ENOENT: ファイル不在, EACCESS: アクセス拒否, EBADF: 不正なファイル記述子,
     *     COB_STATUS_61_FILE_SHARING: ファイル共有競合, COB_LINAGE_INVALID: LINAGE値が不正
     * @throws IOException ファイル入出力エラーが発生した場合
     */
    public int open_(String filename, int mode, int sharing) throws IOException {
        FileChannel fp = null;
        try {
            switch (mode) {
                case COB_OPEN_INPUT:
                    fp = FileChannel.open(Paths.get(filename), StandardOpenOption.READ);
                    break;
                case COB_OPEN_OUTPUT:
                    fp =
                            FileChannel.open(
                                    Paths.get(filename),
                                    StandardOpenOption.WRITE,
                                    StandardOpenOption.CREATE,
                                    StandardOpenOption.TRUNCATE_EXISTING);
                    break;
                case COB_OPEN_I_O:
                    fp =
                            FileChannel.open(
                                    Paths.get(filename),
                                    StandardOpenOption.READ,
                                    StandardOpenOption.WRITE,
                                    StandardOpenOption.CREATE);
                    break;
                case COB_OPEN_EXTEND:
                    fp =
                            FileChannel.open(
                                    Paths.get(filename),
                                    StandardOpenOption.APPEND,
                                    StandardOpenOption.CREATE);
                    break;
                default:
                    break;
            }
        } catch (IOException e) {
            if (Files.notExists(Paths.get(filename))) {
                return ENOENT;
            } else {
                return EACCESS;
            }
        }

        FileLock fl = null;
        if (!filename.startsWith("/dev/")) {
            try {
                boolean isSharedLock;
                if (sharing != 0 || mode == COB_OPEN_OUTPUT) {
                    isSharedLock = false;
                } else {
                    isSharedLock = true;
                }
                fl = fp.tryLock(0L, Long.MAX_VALUE, isSharedLock);
            } catch (NonWritableChannelException e) {
                fp.close();
                return EBADF;
            } catch (ClosedChannelException e) {
                fp.close();
                return COB_STATUS_61_FILE_SHARING;
            }

            this.file.setChannel(fp, fl);
            if (fl == null || !fl.isValid()) {
                fp.close();
                return COB_STATUS_61_FILE_SHARING;
            }
        }

        this.file.setChannel(fp, fl);
        if ((this.flag_select_features & COB_SELECT_LINAGE) != 0) {
            if (this.file_linage_check()) {
                return COB_LINAGE_INVALID;
            }
            this.flag_needs_top = true;
            Linage lingptr = this.getLinorkeyptr();
            lingptr.getLinageCtr().setInt(1);
        }
        if ((this.organization == COB_ORG_SEQUENTIAL
                        || this.organization == COB_ORG_LINE_SEQUENTIAL)
                && (mode == COB_OPEN_OUTPUT || mode == COB_OPEN_EXTEND)
                && CobolUtil.fileSeqWriteBufferSize > 0) {
            this.file.prepareWriteBuffer(CobolUtil.fileSeqWriteBufferSize * this.record_max);
        }
        return 0;
    }

    /**
     * ファイルをクローズする。 COBOL CLOSE文に対応し、指定されたオプションに従ってファイルを閉じる。
     * ファイルがオープンされていない場合はCOB_STATUS_42_NOT_OPENを返す。
     *
     * @param opt クローズオプション (COB_CLOSE_NORMAL=0, COB_CLOSE_LOCK=1, COB_CLOSE_NO_REWIND=2)
     * @param fnstatus ファイルステータスを格納するフィールド
     */
    public void close(int opt, AbstractCobolField fnstatus) {
        String openMode = openModeToString(this.last_open_mode);
        if (invokeFun(COB_IO_CLOSE, this, null, null, fnstatus, openMode, null, null) != 0) {
            this.last_open_mode = (char) Integer.parseInt(openMode);
            return;
        }

        this.flag_read_done = false;
        if (this.special != 0) {
            this.open_mode = COB_OPEN_CLOSED;
            saveStatus(COB_STATUS_00_SUCCESS, fnstatus);
            return;
        }
        if (this.open_mode == COB_OPEN_CLOSED) {
            saveStatus(COB_STATUS_42_NOT_OPEN, fnstatus);
            return;
        }

        int ret;
        if (this.flag_nonexistent) {
            ret = COB_STATUS_00_SUCCESS;
        } else {
            ret = this.close_(opt);
        }

        if (ret == COB_STATUS_00_SUCCESS) {
            switch (opt) {
                case COB_CLOSE_LOCK:
                    this.open_mode = COB_OPEN_LOCKED;
                    break;
                default:
                    this.open_mode = COB_OPEN_CLOSED;
                    break;
            }
        }
        saveStatus(ret, fnstatus);
    }

    // public void closeEx(int opt, AbstractCobolField fnstatus) {
    // this.close_(opt);
    // }

    /**
     * ファイルをクローズする内部メソッド。 LINE SEQUENTIALファイルの場合は必要に応じて改行を出力し、
     * ファイルロックを解放してファイルを閉じる。
     *
     * @param opt クローズオプション (COB_CLOSE_NORMAL=0, COB_CLOSE_LOCK=1, COB_CLOSE_NO_REWIND=2)
     * @return COB_STATUS_00_SUCCESS: 正常終了, COB_STATUS_07_SUCCESS_NO_UNIT: 正常終了(リワインドなし)
     */
    public int close_(int opt) {
        switch (opt) {
            case COB_CLOSE_NORMAL:
            case COB_CLOSE_LOCK:
            case COB_CLOSE_NO_REWIND:
                if (this.organization == COB_ORG_LINE_SEQUENTIAL) {
                    if (this.flag_needs_nl
                            && ((this.flag_select_features & COB_SELECT_LINAGE) == 0)) {
                        this.flag_needs_nl = false;
                        this.file.putc((byte) '\n');
                    }
                }

                this.file.releaseLock();
                this.file.close();

                if (opt == COB_CLOSE_NO_REWIND) {
                    this.open_mode = COB_OPEN_CLOSED;
                    return COB_STATUS_07_SUCCESS_NO_UNIT;
                }
                return COB_STATUS_00_SUCCESS;
            default:
                this.file.flush();
                return COB_STATUS_07_SUCCESS_NO_UNIT;
        }
    }

    /**
     * ファイルの位置決めを行う。 COBOL START文に対応し、指定されたキーと条件に基づいてファイル内の読み取り位置を設定する。
     * INDEXED/RELATIVEファイルでSEQUENTIAL/DYNAMICアクセスモード時に使用される。
     *
     * @param cond 検索条件 (COB_EQ=等しい, COB_LT=より小さい, COB_LE=以下, COB_GT=より大きい, COB_GE=以上,
     *     COB_FI=最初, COB_LA=最後)
     * @param key 検索に使用するキーフィールド
     * @param fnstatus ファイルステータスを格納するフィールド
     */
    public void start(int cond, AbstractCobolField key, AbstractCobolField fnstatus) {
        String openMode = openModeToString(this.last_open_mode);
        String startCond = String.format("%01d", cond);
        if (invokeFun(COB_IO_START, this, null, null, fnstatus, openMode, startCond, null) != 0) {
            return;
        }

        this.flag_read_done = false;
        this.flag_first_read = 0;

        if (this.flag_nonexistent) {
            saveStatus(COB_STATUS_23_KEY_NOT_EXISTS, fnstatus);
            return;
        }

        if (this.open_mode == COB_OPEN_CLOSED
                || this.open_mode == COB_OPEN_OUTPUT
                || this.open_mode == COB_OPEN_EXTEND
                || this.access_mode == COB_ACCESS_RANDOM) {
            saveStatus(COB_STATUS_47_INPUT_DENIED, fnstatus);
            return;
        }

        int ret = this.start_(cond, key);
        if (ret == COB_STATUS_00_SUCCESS) {
            this.flag_end_of_file = false;
            this.flag_begin_of_file = false;
            this.flag_first_read = 1;
        }

        saveStatus(ret, fnstatus);
    }

    /**
     * ファイルの位置決めを行う拡張メソッド。 start_()を直接呼び出し、フック処理やステータス保存をスキップする。
     *
     * @param cond 検索条件 (COB_EQ, COB_LT, COB_LE, COB_GT, COB_GE, COB_FI, COB_LA)
     * @param key 検索に使用するキーフィールド
     * @param fnstatus ファイルステータスを格納するフィールド (未使用)
     */
    public void startEx(int cond, AbstractCobolField key, AbstractCobolField fnstatus) {
        this.start_(cond, key);
    }

    /**
     * ファイルの位置決めを行う内部メソッド。
     * サブクラスでオーバーライドして、ファイル編成固有の位置決め処理を実装する。
     *
     * @param cond 検索条件 (COB_EQ, COB_LT, COB_LE, COB_GT, COB_GE, COB_FI, COB_LA)
     * @param key 検索に使用するキーフィールド
     * @return 操作結果のステータスコード (COB_STATUS_00_SUCCESS等)
     */
    public int start_(int cond, AbstractCobolField key) {
        System.out.println("super.start");
        return 0;
    }

    /**
     * ファイルからレコードを読み込む。 COBOL READ文に対応し、キー指定による直接読み取り(key!=null)または
     * 順次読み取り(key==null)を行う。INDEXEDファイルではレコードロック制御も実施する。
     *
     * @param key 読み取りに使用するキーフィールド。nullの場合は順次読み取り(READ NEXT/PREVIOUS)
     * @param fnstatus ファイルステータスを格納するフィールド
     * @param readOpts 読み取りオプション (COB_READ_NEXT, COB_READ_PREVIOUS, COB_READ_LOCK, COB_READ_NO_LOCK等)
     */
    public void read(AbstractCobolField key, AbstractCobolField fnstatus, int readOpts) {
        byte[] sbuff = new byte[3];
        String openMode = openModeToString(this.last_open_mode);
        String readOptStr = readOptsToString(readOpts);
        if (invokeFun(COB_IO_READ, this, key, null, fnstatus, openMode, null, readOptStr) != 0) {
            for (int i = 0; i < 3; ++i) {
                sbuff[i] = 0;
            }

            if (fnstatus == null) {
                return;
            }

            for (int i = 0; i < 2; ++i) {
                sbuff[i] = fnstatus.getDataStorage().getByte(i);
            }
            int status = Integer.parseInt(new String(sbuff, AbstractCobolField.charSetSJIS));
            saveStatus(status, fnstatus);
            return;
        }

        this.flag_read_done = false;
        if (this.flag_nonexistent) {
            if (this.flag_first_read == 0) {
                saveStatus(COB_STATUS_23_KEY_NOT_EXISTS, fnstatus);
                runPostProcess(fnstatus);
                return;
            }
            this.flag_first_read = 0;
            saveStatus(COB_STATUS_10_END_OF_FILE, fnstatus);
            runPostProcess(fnstatus);
            return;
        }

        /* sequential read at the end of file is an error */
        if (key == null) {
            if (this.flag_end_of_file && (readOpts & COB_READ_PREVIOUS) == 0) {
                saveStatus(COB_STATUS_46_READ_ERROR, fnstatus);
                runPostProcess(fnstatus);
                return;
            }
            if (this.flag_begin_of_file && (readOpts & COB_READ_PREVIOUS) != 0) {
                saveStatus(COB_STATUS_46_READ_ERROR, fnstatus);
                runPostProcess(fnstatus);
                return;
            }
        }

        if (this.open_mode == COB_OPEN_CLOSED
                || this.open_mode == COB_OPEN_OUTPUT
                || this.open_mode == COB_OPEN_EXTEND) {
            saveStatus(COB_STATUS_47_INPUT_DENIED, fnstatus);
            runPostProcess(fnstatus);
            return;
        }

        if (this.organization == COB_ORG_INDEXED) {
            if (this.open_mode != COB_OPEN_I_O || (this.lock_mode & COB_LOCK_EXCLUSIVE) != 0) {
                readOpts &= ~COB_READ_LOCK;
            } else if ((this.lock_mode & COB_LOCK_AUTOMATIC) != 0
                    && (readOpts & COB_READ_NO_LOCK) == 0) {
                readOpts |= COB_READ_LOCK;
            }
        } else {
            readOpts &= ~COB_READ_LOCK;
        }

        int ret;
        if (key != null) {
            ret = this.read_(key, readOpts);
        } else {
            ret = this.readNext(readOpts);
        }

        switch (ret) {
            case COB_STATUS_00_SUCCESS:
                this.flag_first_read = 0;
                this.flag_read_done = true;
                this.flag_end_of_file = false;
                this.flag_begin_of_file = false;
                if (this.record_size != null && this.organization != COB_ORG_LINE_SEQUENTIAL) {
                    this.record_size.setInt(record.getSize());
                }
                break;
            case COB_STATUS_10_END_OF_FILE:
                if ((readOpts & COB_READ_PREVIOUS) != 0) {
                    this.flag_begin_of_file = true;
                } else {
                    this.flag_end_of_file = true;
                }
                break;
            default:
                break;
        }

        saveStatus(ret, fnstatus);
    }

    /**
     * ファイルから順次レコードを読み込む。 keyパラメータは無視され、read(null, fnstatus, readOpts)が呼び出される。
     *
     * @param key 未使用 (互換性のために残されている)
     * @param fnstatus ファイルステータスを格納するフィールド
     * @param readOpts 読み取りオプション (COB_READ_NEXT, COB_READ_PREVIOUS等)
     */
    public void read(int key, AbstractCobolField fnstatus, int readOpts) {
        this.read(null, fnstatus, readOpts);
    }

    /**
     * ファイルからレコードを読み込む拡張メソッド。 read_()を直接呼び出し、フック処理やステータス保存をスキップする。
     *
     * @param key 読み取りに使用するキーフィールド
     * @param fnstatus ファイルステータスを格納するフィールド (未使用)
     * @param readOpts 読み取りオプション
     */
    public void readEx(AbstractCobolField key, AbstractCobolField fnstatus, int readOpts) {
        this.read_(key, readOpts);
    }

    /**
     * 指定されたキーでレコードを読み込む内部メソッド。
     * サブクラスでオーバーライドして、ファイル編成固有の直接読み取り処理を実装する。
     *
     * @param key 読み取りに使用するキーフィールド
     * @param readOpts 読み取りオプション (COB_READ_LOCK等)
     * @return 操作結果のステータスコード (COB_STATUS_00_SUCCESS, COB_STATUS_23_KEY_NOT_EXISTS等)
     */
    public int read_(AbstractCobolField key, int readOpts) {
        System.out.println("super.read");
        return 0;
    }

    /**
     * 次のレコードを順次読み込む内部メソッド。
     * サブクラスでオーバーライドして、ファイル編成固有の順次読み取り処理を実装する。
     * COB_READ_PREVIOUSオプションが指定された場合は前のレコードを読み込む。
     *
     * @param readOpts 読み取りオプション (COB_READ_PREVIOUS: 前方読み, 0: 後方読み)
     * @return 操作結果のステータスコード (COB_STATUS_00_SUCCESS, COB_STATUS_10_END_OF_FILE等)
     */
    public int readNext(int readOpts) {
        System.out.println("super.readNext");
        return 0;
    }

    /**
     * ファイルにレコードを書き込む。 COBOL WRITE文に対応し、レコードサイズの検証を行った後に書き込みを実行する。
     * SEQUENTIALアクセスかつI-Oモードで環境変数COB_IO_ASSUME_REWRITEが設定されている場合は REWRITEとして処理する。
     *
     * @param rec 書き込むレコードのフィールド
     * @param opt 書き込みオプション (COB_WRITE_LINES, COB_WRITE_PAGE, COB_WRITE_BEFORE,
     *     COB_WRITE_AFTER等)
     * @param fnstatus ファイルステータスを格納するフィールド
     * @throws CobolStopRunException STOP RUN文が実行された場合
     */
    public void write(AbstractCobolField rec, int opt, AbstractCobolField fnstatus)
            throws CobolStopRunException {
        if (this.access_mode == COB_ACCESS_SEQUENTIAL
                && this.last_open_mode == COB_OPEN_I_O
                && CobolUtil.cob_io_rewwrite_assumed()) {
            this.rewrite(rec, opt, fnstatus);
            return;
        }

        String openMode = openModeToString(this.last_open_mode);
        if (invokeFun(
                        COB_IO_WRITE,
                        this,
                        null,
                        rec.getDataStorage(),
                        fnstatus,
                        openMode,
                        null,
                        null)
                != 0) {
            return;
        }

        this.flag_read_done = false;
        if (this.access_mode == COB_ACCESS_SEQUENTIAL) {
            if (this.open_mode == COB_OPEN_CLOSED
                    || this.open_mode == COB_OPEN_INPUT
                    || this.open_mode == COB_OPEN_I_O) {
                saveStatus(COB_STATUS_48_OUTPUT_DENIED, fnstatus);
                runPostProcess(fnstatus);
                return;
            }
        } else {
            if (this.open_mode == COB_OPEN_CLOSED
                    || this.open_mode == COB_OPEN_INPUT
                    || this.open_mode == COB_OPEN_EXTEND) {
                saveStatus(COB_STATUS_48_OUTPUT_DENIED, fnstatus);
                runPostProcess(fnstatus);
                return;
            }
        }

        int tmpsize = this.record.getSize();
        if (this.record_size != null) {
            this.record.setSize(this.record_size.getInt());
        } else {
            this.record.setSize(rec.getSize());
        }

        if (this.record.getSize() < this.record_min || this.record_max < this.record.getSize()) {
            saveStatus(COB_STATUS_44_RECORD_OVERFLOW, fnstatus);
            runPostProcess(fnstatus);
            return;
        }

        int ret = this.write_(opt);

        if (cob_do_sync != 0 && ret == 0) {
            cob_sync(this, cob_do_sync);
        }

        this.record.setSize(tmpsize);
        saveStatus(ret, fnstatus);
    }

    /**
     * ファイルにレコードを書き込む拡張メソッド。 write_()を直接呼び出し、フック処理やステータス保存をスキップする。
     *
     * @param rec 書き込むレコードのフィールド (未使用)
     * @param opt 書き込みオプション
     * @param fnstatus ファイルステータスを格納するフィールド (未使用)
     * @throws CobolStopRunException STOP RUN文が実行された場合
     */
    public void writeEx(AbstractCobolField rec, int opt, AbstractCobolField fnstatus)
            throws CobolStopRunException {
        this.write_(opt);
    }

    /**
     * レコードを書き込む内部メソッド。
     * サブクラスでオーバーライドして、ファイル編成固有の書き込み処理を実装する。
     *
     * @param opt 書き込みオプション (COB_WRITE_LINES, COB_WRITE_PAGE等)
     * @return 操作結果のステータスコード (COB_STATUS_00_SUCCESS等)
     * @throws CobolStopRunException STOP RUN文が実行された場合
     */
    public int write_(int opt) throws CobolStopRunException {
        System.out.println("super.write");
        return 0;
    }

    // libcob/fileio.cのcob_file_write_optの実装
    /**
     * 書き込みオプションに従って制御文字を出力する。 COBOL WRITE文のADVANCING句に対応し、
     * LINAGE指定がある場合はlinage_write_opt()を呼び出し、 ない場合は改行またはフォームフィード文字を出力する。
     *
     * @param opt 書き込みオプション (COB_WRITE_LINES: 改行出力, COB_WRITE_PAGE: 改ページ出力)
     * @return 操作結果のステータスコード (通常0)
     * @throws CobolStopRunException STOP RUN文が実行された場合
     */
    protected int file_write_opt(int opt) throws CobolStopRunException {
        if ((this.flag_select_features & COB_SELECT_LINAGE) != 0) {
            return linage_write_opt(opt);
        }
        if ((opt & COB_WRITE_LINES) != 0) {
            for (int i = opt & COB_WRITE_MASK; i > 0; i--) {
                this.file.putc((byte) '\n');
            }
        } else if ((opt & COB_WRITE_PAGE) != 0) {
            this.file.putc((byte) '\f');
        }
        return 0;
    }

    /**
     * ファイル内の現在のレコードを書き換える。 COBOL REWRITE文に対応し、直前にREADしたレコードを更新する。
     * I-Oモードでオープンされていない場合や、READが実行されていない場合はエラーとなる。
     * SEQUENTIALファイルではレコードサイズの変更は許可されない。
     *
     * @param rec 書き換えるレコードのフィールド
     * @param opt 書き込みオプション
     * @param fnstatus ファイルステータスを格納するフィールド
     */
    public void rewrite(AbstractCobolField rec, int opt, AbstractCobolField fnstatus) {
        String openMode = openModeToString(this.last_open_mode);
        if (invokeFun(COB_IO_REWRITE, this, null, null, fnstatus, openMode, null, null) != 0) {
            this.last_open_mode = (char) Integer.parseInt(openMode);
            return;
        }

        boolean readDone = this.flag_read_done;
        this.flag_read_done = false;

        if (this.open_mode == COB_OPEN_CLOSED || this.open_mode != COB_OPEN_I_O) {
            saveStatus(COB_STATUS_49_I_O_DENIED, fnstatus);
            runPostProcess(fnstatus);
            return;
        }
        if (this.access_mode == COB_ACCESS_SEQUENTIAL && !readDone) {
            saveStatus(COB_STATUS_43_READ_NOT_DONE, fnstatus);
            runPostProcess(fnstatus);
            return;
        }
        if (this.organization == COB_ORG_SEQUENTIAL) {
            if (this.record.getSize() != rec.getSize()) {
                saveStatus(COB_STATUS_44_RECORD_OVERFLOW, fnstatus);
                runPostProcess(fnstatus);
                return;
            }
            if (this.record_size != null) {
                if (this.record.getSize() != this.record_size.getInt()) {
                    saveStatus(COB_STATUS_44_RECORD_OVERFLOW, fnstatus);
                    runPostProcess(fnstatus);
                    return;
                }
            }
        }
        int ret = this.rewrite_(opt);
        if (cob_do_sync != 0 && ret == 0) {
            cob_sync(this, cob_do_sync);
        }

        saveStatus(ret, fnstatus);
    }

    /**
     * ファイル内の現在のレコードを書き換える拡張メソッド。 rewrite_()を直接呼び出し、フック処理やステータス保存をスキップする。
     *
     * @param rec 書き換えるレコードのフィールド (未使用)
     * @param opt 書き込みオプション
     * @param fnstatus ファイルステータスを格納するフィールド (未使用)
     */
    public void rewriteEx(AbstractCobolField rec, int opt, AbstractCobolField fnstatus) {
        this.rewrite_(opt);
    }

    /**
     * レコードを書き換える内部メソッド。
     * サブクラスでオーバーライドして、ファイル編成固有の書き換え処理を実装する。
     *
     * @param opt 書き込みオプション
     * @return 操作結果のステータスコード (COB_STATUS_00_SUCCESS等)
     */
    public int rewrite_(int opt) {
        System.out.println("super.rewrite");
        return 0;
    }

    /**
     * ファイルから現在のレコードを削除する。 COBOL DELETE文に対応し、直前にREADしたレコードを削除する。
     * I-Oモードでオープンされていない場合や、SEQUENTIALアクセスでREADが実行されていない場合はエラーとなる。
     *
     * @param fnstatus ファイルステータスを格納するフィールド
     */
    public void delete(AbstractCobolField fnstatus) {
        String openMode = openModeToString(this.last_open_mode);
        if (invokeFun(COB_IO_DELETE, this, null, null, fnstatus, openMode, null, null) != 0) {
            return;
        }

        boolean readDone = this.flag_read_done;
        this.flag_read_done = false;

        if (this.open_mode == COB_OPEN_CLOSED || this.open_mode != COB_OPEN_I_O) {
            saveStatus(COB_STATUS_49_I_O_DENIED, fnstatus);
            runPostProcess(fnstatus);
            return;
        }

        if (this.access_mode == COB_ACCESS_SEQUENTIAL && !readDone) {
            saveStatus(COB_STATUS_43_READ_NOT_DONE, fnstatus);
            runPostProcess(fnstatus);
            return;
        }

        int ret = this.delete_();

        if (cob_do_sync != 0 && ret == 0) {
            cob_sync(this, cob_do_sync);
        }
        saveStatus(ret, fnstatus);
    }

    /**
     * ファイルから現在のレコードを削除する拡張メソッド。 delete_()を直接呼び出し、フック処理やステータス保存をスキップする。
     *
     * @param fnstatus ファイルステータスを格納するフィールド (未使用)
     */
    public void deleteEx(AbstractCobolField fnstatus) {
        this.delete_();
    }

    /**
     * レコードを削除する内部メソッド。
     * サブクラスでオーバーライドして、ファイル編成固有の削除処理を実装する。
     *
     * @return 操作結果のステータスコード (COB_STATUS_00_SUCCESS等)
     */
    public int delete_() {
        System.out.println("super.delete");
        return 0;
    }

    /**
     * ファイルのレコードロックを解除する。 COBOL UNLOCK文に対応し、このファイルに対する全てのレコードロックを解除する。
     *
     * @param fnstatus ファイルステータスを格納するフィールド
     */
    public void unlock(AbstractCobolField fnstatus) {
        String openMode = openModeToString(this.last_open_mode);
        if (invokeFun(COB_IO_UNLOCK, this, null, null, fnstatus, openMode, null, null) != 0) {
            return;
        }
        this.unlock_();
        saveStatus(COB_STATUS_00_SUCCESS, fnstatus);
    }

    /** ファイルのロックを解除する内部メソッド。 ファイルがオープンされている場合、バッファをフラッシュする。 */
    public void unlock_() {
        if (this.open_mode != COB_OPEN_CLOSED && this.open_mode != COB_OPEN_LOCKED) {
            this.file.flush();
        }
    }

    /**
     * 全てのオープン中ファイルに対するトランザクションをコミットする。 COBOL COMMIT文に対応し、
     * 全ファイルのバッファをフラッシュしてレコードロックを解除する。 変更内容を確定し、他のプロセスから参照可能な状態にする。
     */
    public static void commit() {
        if (invokeFun(COB_IO_COMMIT, null, null, null, null, null, null, null) != 0) {
            return;
        }
        for (CobolFile l : file_cache) {
            l.unlock_();
        }
    }

    /**
     * 全てのオープン中ファイルに対するトランザクションをロールバックする。 COBOL ROLLBACK文に対応し、全ファイルのロックを解除する。
     * 注意: 現在の実装では実際のデータ巻き戻しは行われず、ロック解除のみが実行される。
     */
    public static void rollback() {
        if (invokeFun(COB_IO_ROLLBACK, null, null, null, null, null, null, null) != 0) {
            return;
        }
        for (CobolFile l : file_cache) {
            l.unlock_();
        }
    }

    /// libcob/fileio.cのcob_exit_fileioの実装 TODO 一部だけ実装したため残りを実装する
    /**
     * ファイルI/Oサブシステムを終了する。
     * オープン中のファイルがある場合は警告メッセージを出力する（暗黙的なCLOSE）。 プログラム終了時に呼び出される。
     */
    public static void exitFileIO() {
        for (CobolFile f : file_cache) {
            if (f.open_mode != COB_OPEN_CLOSED && f.open_mode != COB_OPEN_LOCKED) {
                String filename = f.assign.fieldToString();
                System.err.print(
                        String.format(
                                "WARNING - Implicit CLOSE of %s (\"%s\") %c",
                                f.select_name, filename, '\n'));
            }
        }
    }

    // libcob/fileio.cのcob_syncの実装
    /**
     * ファイルバッファをディスクに同期する。 環境変数COB_SYNCが設定されている場合に呼び出され、
     * ファイルの内容をディスクに強制的に書き込む。
     *
     * @param f 同期対象のファイル
     * @param mode 同期モード (1: flush, 2: fsync)
     */
    protected void cob_sync(CobolFile f, int mode) {
        // TODO
        // INDEXEDファイル実装時にやる
        // if (f.organization == COB_ORG_INDEXED) {
        // }
        if (f.organization != COB_ORG_SORT) {
            this.file.flush();
            if (mode == 2) {
                this.file.flush();
            }
        }
    }

    // libcob/fileio.cのcob_init_fileioの実装
    /**
     * ファイルI/Oサブシステムを初期化する。 環境変数からファイルI/O関連の設定を読み込む: - COB_SYNC:
     * 同期モード(Y/y=flush後sync, P/p=物理sync) - COB_FILE_PATH: ファイル検索パス - COB_LS_NULLS:
     * LINE SEQUENTIALでのNULL処理 - COB_LS_FIXED: LINE SEQUENTIALでの固定長モード
     */
    public static void cob_init_fileio() {
        String s = CobolUtil.getEnv("COB_SYNC");
        if (s != null) {
            if (s.charAt(0) == 'Y' || s.charAt(0) == 'y') {
                cob_do_sync = 1;
            }
            if (s.charAt(0) == 'P' || s.charAt(0) == 'p') {
                cob_do_sync = 2;
            }
        }

        cob_file_path = CobolUtil.getEnv("COB_FILE_PATH");
        if (cob_file_path != null) {
            if (cob_file_path.charAt(0) == '\0' || cob_file_path.charAt(0) == ' ') {
                cob_file_path = null;
            }
        }

        cob_ls_nulls = CobolUtil.getEnv("COB_LS_NULLS");
        cob_ls_fixed = CobolUtil.getEnv("COB_LS_FIXED");

        file_open_env = new byte[COB_SMALL_BUFF];
        // file_open_name = new byte[COB_SMALL_BUFF];
        file_open_buff = new byte[COB_SMALL_BUFF];
    }

    /**
     * デフォルトのファイルエラーハンドラ。 ファイル操作でエラーが発生した際に呼び出され、
     * ファイルステータスコードに応じたエラーメッセージを標準エラー出力に表示する。 FILE STATUS句のDECLARATIVES
     * USEがない場合のデフォルト動作。
     */
    public static void defaultErrorHandle() {
        byte[] fileStatus = CobolFile.errorFile.file_status;
        int status = (fileStatus[0] - '0') * 10 + (fileStatus[1] - '0');
        String msg;
        switch (status) {
            case COB_STATUS_10_END_OF_FILE:
                msg = "End of file";
                break;
            case COB_STATUS_14_OUT_OF_KEY_RANGE:
                msg = "Key out of range";
                break;
            case COB_STATUS_21_KEY_INVALID:
                msg = "Key order not ascending";
                break;
            case COB_STATUS_22_KEY_EXISTS:
                msg = "Record key already exists";
                break;
            case COB_STATUS_23_KEY_NOT_EXISTS:
                msg = "Record key does not exist";
                break;
            case COB_STATUS_30_PERMANENT_ERROR:
                msg = "Permanent file error";
                break;
            case COB_STATUS_35_NOT_EXISTS:
                msg = "File does not exist";
                break;
            case COB_STATUS_37_PERMISSION_DENIED:
                msg = "Permission denied";
                break;
            case COB_STATUS_41_ALREADY_OPEN:
                msg = "File already open";
                break;
            case COB_STATUS_42_NOT_OPEN:
                msg = "File not open";
                break;
            case COB_STATUS_43_READ_NOT_DONE:
                msg = "READ must be executed first";
                break;
            case COB_STATUS_44_RECORD_OVERFLOW:
                msg = "Record overflow";
                break;
            case COB_STATUS_46_READ_ERROR:
                msg = "Failed to read";
                break;
            case COB_STATUS_47_INPUT_DENIED:
                msg = "READ/START not allowed";
                break;
            case COB_STATUS_48_OUTPUT_DENIED:
                msg = "WRITE not allowed";
                break;
            case COB_STATUS_49_I_O_DENIED:
                msg = "DELETE/REWRITE not allowed";
                break;
            case COB_STATUS_51_RECORD_LOCKED:
                msg = "Record locked by another file connector";
                break;
            case COB_STATUS_52_EOP:
                msg = "A page overflow condition occurred";
                break;
            case COB_STATUS_57_I_O_LINAGE:
                msg = "LINAGE values invalid";
                break;
            case COB_STATUS_61_FILE_SHARING:
                msg = "File sharing conflict";
                break;
            case COB_STATUS_91_NOT_AVAILABLE:
                msg = "Runtime library is not configured for this operation";
                break;
            default:
                msg = "Unknown file error";
                break;
        }
        String filename = CobolFile.errorFile.assign.fieldToString();
        CobolUtil.runtimeError(
                String.format("%s (STATUS = %02d) File : '%s'", msg, status, filename));
    }

    /**
     * ファイルを物理的に削除する。 ファイルがオープン中の場合やロック状態の場合はエラーとなる。
     * ファイルが存在しない場合はCOB_STATUS_35_NOT_EXISTSを返す。
     *
     * @param fnstatus ファイルステータスを格納するフィールド
     */
    public void cob_delete_file(AbstractCobolField fnstatus) {
        String openMode = openModeToString(this.last_open_mode);
        if (invokeFun(COB_IO_DELETE_FILE, this, null, null, fnstatus, openMode, null, null) != 0) {
            return;
        }

        if (this.open_mode == COB_OPEN_LOCKED) {
            saveStatus(COB_STATUS_38_CLOSED_WITH_LOCK, fnstatus);
            return;
        }

        /* file is already open */
        if (this.open_mode != COB_OPEN_CLOSED) {
            saveStatus(COB_STATUS_41_ALREADY_OPEN, fnstatus);
            return;
        }

        if (this.special != 0) {
            saveStatus(COB_STATUS_30_PERMANENT_ERROR, fnstatus);
            return;
        }

        if (this.assign == null) {
            file_open_name = this.select_name;
        } else {
            file_open_name = this.assign.fieldToString();
        }

        byte[] src;
        byte[] dst;
        boolean simple;
        if (CobolModule.getCurrentModule().flag_filename_mapping != 0) {
            src = file_open_name.getBytes(AbstractCobolField.charSetSJIS);
            dst = file_open_buff;
            simple = true;
            int srcI = 0;
            int dstI = 0;
            while (srcI < src.length) {
                char c = (char) src[srcI];
                if (!Character.isLetterOrDigit(c) && c != '_' && c != '-') {
                    simple = false;
                }
                if (c == '$') {
                    int i;
                    for (i = 1; srcI + i < src.length; i++) {
                        char d = (char) src[srcI + i];
                        if (!Character.isLetterOrDigit(d) && d != '_' && c != '-') {
                            break;
                        }
                    }
                    for (int j = 0; j < i - 1; ++j) {
                        file_open_env[j] = src[srcI + 1 + j];
                    }
                    file_open_env[i - 1] = 0;
                    String p =
                            CobolUtil.getEnv(
                                    new String(
                                            Arrays.copyOfRange(file_open_env, 0, i - 1),
                                            AbstractCobolField.charSetSJIS));
                    if (p != null) {
                        byte[] pbytes = p.getBytes(AbstractCobolField.charSetSJIS);
                        for (int j = 0; j < pbytes.length; ++j) {
                            dst[dstI + j] = pbytes[j];
                        }
                        dstI += pbytes.length;
                    }
                    srcI += i;
                } else {
                    dst[dstI++] = src[srcI++];
                }
            }

            file_open_name =
                    new String(Arrays.copyOfRange(dst, 0, dstI), AbstractCobolField.charSetSJIS);

            byte[] fileOpenNameBytes = file_open_name.getBytes(AbstractCobolField.charSetSJIS);
            cb_get_jisword_buff(file_open_buff, fileOpenNameBytes, COB_SMALL_BUFF);

            if (simple) {
                int i;
                for (i = 0; i < NUM_PREFIX; i++) {
                    byte[] fileOpenBuff =
                            concatString(prefix[i], file_open_name)
                                    .getBytes(AbstractCobolField.charSetSJIS);
                    String p =
                            CobolUtil.getEnv(
                                    new String(fileOpenBuff, AbstractCobolField.charSetSJIS));
                    if (p != null) {
                        fileOpenNameBytes = p.getBytes(AbstractCobolField.charSetSJIS);
                        break;
                    }
                }

                if (i == NUM_PREFIX && cob_file_path != null) {
                    byte[] fileOpenBuff =
                            concatString(cob_file_path, "/", file_open_name)
                                    .getBytes(AbstractCobolField.charSetSJIS);
                    fileOpenNameBytes = fileOpenBuff;
                }
            }

            file_open_name = new String(fileOpenNameBytes, AbstractCobolField.charSetSJIS);
        }

        Path filePath;
        if (this.assign == null) {
            filePath = Paths.get(this.select_name);
        } else {
            filePath = Paths.get(this.assign.fieldToString());
        }
        try {
            saveStatus(COB_STATUS_00_SUCCESS, fnstatus);
            Files.delete(filePath);
            return;
        } catch (IOException e) {
            int mode = (int) this.last_open_mode;
            try {
                switch (this.open_(file_open_name, mode, 0)) {
                    case ENOENT:
                        saveStatus(COB_STATUS_35_NOT_EXISTS, fnstatus);
                        return;
                    case EACCESS:
                    case EISDIR:
                    case EROFS:
                        saveStatus(COB_STATUS_37_PERMISSION_DENIED, fnstatus);
                        return;
                    case EAGAIN:
                    case COB_STATUS_61_FILE_SHARING:
                        saveStatus(COB_STATUS_61_FILE_SHARING, fnstatus);
                        return;
                    case COB_STATUS_91_NOT_AVAILABLE:
                        saveStatus(COB_STATUS_91_NOT_AVAILABLE, fnstatus);
                        return;
                    case COB_STATUS_92_VERSION_INCOMPATIBLE:
                        saveStatus(COB_STATUS_92_VERSION_INCOMPATIBLE, fnstatus);
                        return;
                    case COB_LINAGE_INVALID:
                        saveStatus(COB_STATUS_57_I_O_LINAGE, fnstatus);
                        return;
                    default:
                        saveStatus(COB_STATUS_30_PERMANENT_ERROR, fnstatus);
                        return;
                }
            } catch (IOException e1) {
                saveStatus(COB_STATUS_30_PERMANENT_ERROR, fnstatus);
                return;
            }
        }
    }

    /**
     * ファイルのSELECT名を取得する。 COBOL SELECT句で指定されたファイル名を返す。
     *
     * @return SELECTで指定されたファイル名
     */
    public String getSelectName() {
        // CobolFile cobolFile = new CobolFile();
        return this.select_name;
    }

    /**
     * ファイルステータスを取得する。 FILE STATUS句で指定された2バイトのステータス値を返す。
     *
     * @return 2バイトのファイルステータス (例: "00"=成功, "10"=EOF)
     */
    public byte[] getFileStatus() {
        // CobolFile cobolFile = new CobolFile();
        return this.file_status;
    }

    private static Map<String, byte[]> externalFileStatusTable = new HashMap<String, byte[]>();

    /**
     * 外部ファイルのファイルステータス領域を取得する。 EXTERNAL句が指定されたファイルのステータス領域を返す。
     * 存在しない場合は新規に作成して返す。
     *
     * @param key 外部ファイルのキー (通常はSELECT名)
     * @return 2バイトのファイルステータス領域
     */
    public static byte[] getExternalFileStatus(String key) {
        byte[] bytes = externalFileStatusTable.get(key);
        if (bytes == null) {
            bytes = new byte[2];
            bytes[0] = 0;
            bytes[1] = 0;
            externalFileStatusTable.put(key, bytes);
        }
        return bytes;
    }

    private static Map<String, CobolFile> externalFileTable = new HashMap<String, CobolFile>();

    /**
     * 外部ファイルインスタンスを取得する。 EXTERNAL句が指定されたファイルを複数プログラム間で共有するために使用する。
     *
     * @param key 外部ファイルのキー (通常はSELECT名)
     * @return 登録されているCobolFileインスタンス、未登録の場合はnull
     */
    public static CobolFile getExternalFile(String key) {
        return externalFileTable.get(key);
    }

    /**
     * 外部ファイルインスタンスを登録する。 EXTERNAL句が指定されたファイルを複数プログラム間で共有するために使用する。
     *
     * @param key 外部ファイルのキー (通常はSELECT名)
     * @param value 登録するCobolFileインスタンス
     * @return 以前に同じキーで登録されていたインスタンス、なければnull
     */
    public static CobolFile putExternalFile(String key, CobolFile value) {
        return externalFileTable.put(key, value);
    }
}

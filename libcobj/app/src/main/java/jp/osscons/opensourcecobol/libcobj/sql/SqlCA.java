package jp.osscons.opensourcecobol.libcobj.sql;

import java.nio.ByteBuffer;
import java.sql.SQLException;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/** COBOL の埋め込み SQL で使用する SQLCA (SQL Communication Area) 構造体を管理する。 */
final class SqlCA {

    /** ユーティリティクラスのインスタンス化を防ぐための private コンストラクタ。 */
    private SqlCA() {}

    /** SQLERRMC エラーメッセージフィールドの最大長。 */
    static final int SQLERRMC_LEN = 70;

    // SQLCA 構造体内のオフセット (全体で 133 バイト)
    // OFFSET_SQLCAID (0), OFFSET_SQLCABC (8), OFFSET_SQLERRP (88), OFFSET_SQLWARN (120)
    // は SQLCA の仕様上定義されているが、現在のコードでは使用していない。
    private static final int OFFSET_SQLCODE = 12; // 4 バイト (int)
    private static final int OFFSET_SQLERRML = 16; // 2 バイト (short)
    private static final int OFFSET_SQLERRMC = 18; // 70 バイト
    private static final int OFFSET_SQLERRD = 96; // 24 バイト (int 6 個)
    private static final int OFFSET_SQLSTATE = 128; // 5 バイト

    /** エラーなし。 */
    static final int ECPG_NO_ERROR = 0;

    /** 行が見つからない (SQLSTATE 02000)。 */
    static final int ECPG_NOT_FOUND = 100;

    /** メモリ不足。 */
    static final int ECPG_OUT_OF_MEMORY = -12;

    /** 未対応の機能。 */
    static final int ECPG_UNSUPPORTED = -200;

    /** ホスト変数の引数が多すぎる。 */
    static final int ECPG_TOO_MANY_ARGUMENTS = -201;

    /** ホスト変数の引数が少なすぎる。 */
    static final int ECPG_TOO_FEW_ARGUMENTS = -202;

    /** 一致する行が多すぎる。 */
    static final int ECPG_TOO_MANY_MATCHES = -203;

    /** データ形式エラー。 */
    static final int ECPG_DATA_FORMAT_ERROR = -204;

    /** 空のクエリまたは文。 */
    static final int ECPG_EMPTY = -212;

    /** 指標変数が指定されていない。 */
    static final int ECPG_MISSING_INDICATOR = -213;

    /** 有効な接続がない。 */
    static final int ECPG_NO_CONN = -220;

    /** 未接続。 */
    static final int ECPG_NOT_CONN = -221;

    /** 不正な準備済みステートメント。 */
    static final int ECPG_INVALID_STMT = -230;

    /** Informix 互換の一意キー違反エラー。 */
    static final int ECPG_INFORMIX_DUPLICATE_KEY = -239;

    /** 不明なデスクリプタ。 */
    static final int ECPG_UNKNOWN_DESCRIPTOR = -240;

    /** 不正なデスクリプタインデックス。 */
    static final int ECPG_INVALID_DESCRIPTOR_INDEX = -241;

    /** 不明なデスクリプタ項目。 */
    static final int ECPG_UNKNOWN_DESCRIPTOR_ITEM = -242;

    /** 変数が数値型でない。 */
    static final int ECPG_VAR_NOT_NUMERIC = -243;

    /** 変数が文字型でない。 */
    static final int ECPG_VAR_NOT_CHAR = -244;

    /** Informix 互換の副問い合わせが 2 行以上を返した。 */
    static final int ECPG_INFORMIX_SUBSELECT_NOT_ONE = -284;

    /** PostgreSQL バックエンドエラー。 */
    static final int ECPG_PGSQL = -400;

    /** トランザクションエラー。 */
    static final int ECPG_TRANS = -401;

    /** 接続エラー。 */
    static final int ECPG_CONNECT = -402;

    /** 一意キー違反。 */
    static final int ECPG_DUPLICATE_KEY = -403;

    /** 副問い合わせが 2 行以上を返した。 */
    static final int ECPG_SUBSELECT_NOT_ONE = -404;

    /** 不明なカーソル (ポータル)。 */
    static final int ECPG_WARNING_UNKNOWN_PORTAL = -602;

    /** 既にトランザクション中。 */
    static final int ECPG_WARNING_IN_TRANSACTION = -603;

    /** 有効なトランザクションがない。 */
    static final int ECPG_WARNING_NO_TRANSACTION = -604;

    /** カーソル (ポータル) が既に存在する。 */
    static final int ECPG_WARNING_PORTAL_EXISTS = -605;

    /** ロックエラー。 */
    static final int ECPG_LOCK_ERROR = -606;

    /** JDD (Java Database Driver) エラー。 */
    static final int ECPG_JDD_ERROR = -607;

    /** 認識できないエラー。 */
    static final int ECPG_UNKNOWN_ERROR = -9999;

    /**
     * SQLCA 構造体の SQLCODE フィールドを設定する。
     *
     * @param sqlca SQLCA のデータストレージ
     * @param code 設定する SQLCODE 値
     */
    static void setCode(CobolDataStorage sqlca, int code) {
        if (sqlca == null) {
            return;
        }
        sqlca.getSubDataStorage(OFFSET_SQLCODE).set(code);
    }

    /**
     * SQLCA 構造体から SQLCODE フィールドを取得する。
     *
     * @param sqlca SQLCA のデータストレージ
     * @return 現在の SQLCODE 値。sqlca が null の場合は 0
     */
    static int getCode(CobolDataStorage sqlca) {
        if (sqlca == null) {
            return 0;
        }
        return ByteBuffer.wrap(
                        sqlca.getByteArrayRef(OFFSET_SQLCODE, 4),
                        sqlca.getIndex() + OFFSET_SQLCODE,
                        4)
                .getInt();
    }

    /**
     * SQLCA 構造体の 5 文字の SQLSTATE フィールドを設定する。
     *
     * @param sqlca SQLCA のデータストレージ
     * @param state SQLSTATE 文字列 (例: "00000")
     */
    static void setState(CobolDataStorage sqlca, String state) {
        if (sqlca == null || state == null) {
            return;
        }
        byte[] stateBytes = state.getBytes();
        for (int i = 0; i < 5; i++) {
            if (i < stateBytes.length) {
                sqlca.setByte(OFFSET_SQLSTATE + i, stateBytes[i]);
            } else {
                sqlca.setByte(OFFSET_SQLSTATE + i, (byte) ' ');
            }
        }
    }

    /**
     * SQLCA 構造体の SQLERRMC エラーメッセージフィールドを設定する。
     *
     * @param sqlca SQLCA のデータストレージ
     * @param message エラーメッセージ ({@link #SQLERRMC_LEN} バイトに切り詰められる)
     */
    static void setErrmc(CobolDataStorage sqlca, String message) {
        if (sqlca == null) {
            return;
        }
        if (message == null) {
            clearErrmc(sqlca);
            return;
        }
        byte[] msgBytes = message.getBytes();
        int len = Math.min(msgBytes.length, SQLERRMC_LEN);
        // SQLERRML を設定する
        sqlca.getSubDataStorage(OFFSET_SQLERRML).set((short) len);
        // SQLERRMC を設定する
        for (int i = 0; i < SQLERRMC_LEN; i++) {
            if (i < len) {
                sqlca.setByte(OFFSET_SQLERRMC + i, msgBytes[i]);
            } else {
                sqlca.setByte(OFFSET_SQLERRMC + i, (byte) 0);
            }
        }
    }

    /**
     * 処理行数を保持する SQLERRD の 0 始まりインデックス。COBOL では {@code SQLERRD(3)} に対応し、
     * 直前の SQL 文が処理した行数 (INSERT/UPDATE/DELETE の影響行数、FETCH した行数など) が入る。
     */
    private static final int SQLERRD_INDEX_ROW_COUNT = 2;

    /**
     * 6 個ある SQLERRD 診断値のうちの 1 つを設定する。
     *
     * @param sqlca SQLCA のデータストレージ
     * @param index SQLERRD のインデックス (0〜5)
     * @param value 設定する整数値
     */
    static void setErrd(CobolDataStorage sqlca, int index, int value) {
        if (sqlca == null || index < 0 || index >= 6) {
            return;
        }
        sqlca.getSubDataStorage(OFFSET_SQLERRD + index * 4).set(value);
    }

    /**
     * 直前の SQL 文が処理した行数を SQLERRD(3) に設定する。
     *
     * @param sqlca SQLCA のデータストレージ
     * @param rowCount 処理した行数
     */
    static void setRowCount(CobolDataStorage sqlca, int rowCount) {
        setErrd(sqlca, SQLERRD_INDEX_ROW_COUNT, rowCount);
    }

    /**
     * SQLCA 構造体の SQLERRMC および SQLERRML フィールドをクリアする。
     *
     * @param sqlca SQLCA のデータストレージ
     */
    static void clearErrmc(CobolDataStorage sqlca) {
        if (sqlca == null) {
            return;
        }
        sqlca.getSubDataStorage(OFFSET_SQLERRML).set((short) 0);
        for (int i = 0; i < SQLERRMC_LEN; i++) {
            sqlca.setByte(OFFSET_SQLERRMC + i, (byte) 0);
        }
    }

    /**
     * SQLCA を正常終了に設定するが、SQLERRMC のバイト列は変更しない (SQLERRML は 0 にする)。
     *
     * <p>CONNECT 成功時に使う。Open-COBOL-ESQL-4J は CONNECT 成功時に SQLERRMC を
     * 上書きせず、COBOL が初期化した値 (PIC X のスペース) をそのまま残すため、それに合わせる。
     * メッセージ長 SQLERRML は 0 にして「メッセージなし」と整合させる (直前のエラーの長さが
     * 残らないようにする)。
     *
     * @param sqlca SQLCA のデータストレージ
     */
    static void setSuccessKeepErrmc(CobolDataStorage sqlca) {
        if (sqlca == null) {
            return;
        }
        setCode(sqlca, ECPG_NO_ERROR);
        setState(sqlca, "00000");
        sqlca.getSubDataStorage(OFFSET_SQLERRML).set((short) 0);
    }

    /**
     * SQLCA を正常終了を示す状態に設定する (SQLCODE=0, SQLSTATE="00000")。
     *
     * @param sqlca SQLCA のデータストレージ
     */
    static void setSuccess(CobolDataStorage sqlca) {
        if (sqlca == null) {
            return;
        }
        setCode(sqlca, ECPG_NO_ERROR);
        setState(sqlca, "00000");
        clearErrmc(sqlca);
    }

    /**
     * SQLCA を ECPG_MISSING_INDICATOR に設定する (sqlcode=-213, sqlstate="22002")。
     *
     * <p>「指標変数なしの NULL 値」を通知する。これは、フェッチした列が SQL NULL であり、
     * かつホスト変数に対応する指標変数がない場合の ECPG 標準の動作である。COBOL フィールド
     * 自体には依然として値が書き込まれる (通常はゼロ埋め) ため行は処理済みとみなされる。
     * アプリケーション側のコードは FETCH/SELECT の後に SQLCODE/SQLSTATE を確認することが想定されている。
     *
     * @param sqlca SQLCA のデータストレージ
     */
    static void setMissingIndicator(CobolDataStorage sqlca) {
        setError(sqlca, ECPG_MISSING_INDICATOR, "22002", "Null value without indicator");
    }

    /**
     * SQLCA を、指定したコード・状態・メッセージのエラーを示す状態に設定する。
     *
     * @param sqlca SQLCA のデータストレージ
     * @param code SQLCODE のエラーコード
     * @param state 5 文字の SQLSTATE
     * @param message エラーメッセージ
     */
    static void setError(CobolDataStorage sqlca, int code, String state, String message) {
        if (sqlca == null) {
            return;
        }
        setCode(sqlca, code);
        setState(sqlca, state);
        setErrmc(sqlca, message);
    }

    /**
     * SQLException から SQLCA を設定し、SQLSTATE を ECPG エラーコードにマッピングする。
     *
     * @param sqlca SQLCA のデータストレージ
     * @param e SQL 例外
     */
    static void setResultFromException(CobolDataStorage sqlca, SQLException e) {
        if (sqlca == null) {
            return;
        }
        String sqlState = e.getSQLState();
        if (sqlState == null) {
            sqlState = "     ";
        }
        int code = sqlStateToCode(sqlState);
        String message = e.getMessage();
        if (message == null) {
            message = "";
        }
        setError(sqlca, code, sqlState, message);
    }

    /**
     * 5 文字の SQLSTATE 文字列を対応する ECPG エラーコードにマッピングする。
     *
     * @param sqlState SQLSTATE 文字列
     * @return ECPG エラーコード定数
     */
    static int sqlStateToCode(String sqlState) {
        if (sqlState == null) {
            return ECPG_UNKNOWN_ERROR;
        }
        switch (sqlState) {
            case "00000":
                return ECPG_NO_ERROR;
            case "02000":
                return ECPG_NOT_FOUND;
            case "YE002":
                return ECPG_EMPTY;
            case "08001":
            case "08003":
                return ECPG_CONNECT;
            case "08007":
                return ECPG_TRANS;
            case "21000":
                return ECPG_SUBSELECT_NOT_ONE;
            case "23505":
                return ECPG_DUPLICATE_KEY;
            case "25001":
                return ECPG_WARNING_IN_TRANSACTION;
            case "25P01":
                return ECPG_WARNING_NO_TRANSACTION;
            case "34000":
                return ECPG_WARNING_UNKNOWN_PORTAL;
            case "42804":
                return ECPG_DATA_FORMAT_ERROR;
            case "42P03":
                return ECPG_WARNING_PORTAL_EXISTS;
            case "55P03":
                return ECPG_PGSQL;
            default:
                return ECPG_UNKNOWN_ERROR;
        }
    }
}

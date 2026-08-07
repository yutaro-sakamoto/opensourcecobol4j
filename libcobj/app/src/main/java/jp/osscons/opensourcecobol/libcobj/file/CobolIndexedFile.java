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
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Optional;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import org.sqlite.SQLiteConfig;
import org.sqlite.SQLiteErrorCode;

/**
 * INDEXEDファイル（{@code ORGANIZATION IS INDEXED}）向けの{@link CobolFile}の具象実装。
 *
 * <p>各INDEXEDファイルは1つのSQLiteデータベースをバックエンドとする（1ファイル＝1データベースであり、xerialの{@code
 * sqlite-jdbc}ドライバ経由でアクセスする）。ファイルを開いている間は、SQLiteが{@code -wal}／{@code
 * -shm}（WALモードの場合）または{@code -journal}（ロールバックジャーナルモードの場合）を隣に作成する。詳細は{@link
 * IndexedJournalConfig}を参照。主キーは{@code table0}テーブルに、各副キーは{@code
 * tableI}に格納される。ファイル全体のロックは{@code file_lock}テーブルで、レコードロックは{@code table0}の{@code
 * locked_by}列で管理する。ファイルのメタデータ（レコードサイズとキーのレイアウト）は{@code metadata_*}テーブルに保持する。
 *
 * <p>このクラスは{@link CobolFile}の{@code *_}フックメソッド（{@link #open_}、{@link #read_}、{@link
 * #write_}、{@link #rewrite_}、{@link #delete_}、{@link
 * #start_}など）をオーバーライドし、各COBOLのI/O動詞を対応するSQL操作へ変換する。順次ナビゲーション（{@code READ NEXT}／{@code
 * READ PREVIOUS}）は{@link IndexedCursor}でエミュレートする。
 *
 * <p>すべてのSQLは明示的なトランザクション内で実行され（{@code setAutoCommit(false)}、{@code
 * TRANSACTION_SERIALIZABLE}）、各文は成功時にコミット、失敗時にロールバックする。
 */
public class CobolIndexedFile extends CobolFile {
    private Optional<IndexedCursor> cursor;
    private boolean updateWhileReading = false;
    private boolean indexedFirstRead = true;
    private boolean callStart = false;
    private boolean commitOnModification = true;
    private int fetchKeyIndex = -1;
    private byte[] previousLockedRecordKey = null;

    /** 他プロセスのトランザクションの終了を待つ時間(ミリ秒)。SQLiteの{@code busy_timeout}に設定する。 */
    private static final int BUSY_TIMEOUT_MILLIS = 5000;

    /**
     * {@code SQLITE_BUSY_SNAPSHOT}によるリトライの上限回数。
     *
     * <p>待機時間は1ミリ秒から{@link #MAX_SNAPSHOT_BACKOFF_MILLIS}まで1回ごとに延びる。
     * この回数と合わせた総待ち時間はおよそ4秒であり、通常の{@code SQLITE_BUSY}に適用される{@link
     * #BUSY_TIMEOUT_MILLIS}と同程度になる。
     */
    private static final int MAX_SNAPSHOT_RETRIES = 100;

    /** {@code SQLITE_BUSY_SNAPSHOT}のリトライ1回あたりの最大待機時間(ミリ秒)。 */
    private static final long MAX_SNAPSHOT_BACKOFF_MILLIS = 50;

    /** {@code START}/{@code READ}の比較条件：キーが等しい（{@code =}）。 */
    public static final int COB_EQ = 1;

    /** {@code START}/{@code READ}の比較条件：キーが小さい（{@code <}）。 */
    public static final int COB_LT = 2;

    /** {@code START}/{@code READ}の比較条件：キーが小さいか等しい（{@code <=}）。 */
    public static final int COB_LE = 3;

    /** {@code START}/{@code READ}の比較条件：キーが大きい（{@code >}）。 */
    public static final int COB_GT = 4;

    /** {@code START}/{@code READ}の比較条件：キーが大きいか等しい（{@code >=}）。 */
    public static final int COB_GE = 5;

    /** {@code START}/{@code READ}の比較条件：キーが等しくない（{@code <>}）。 */
    public static final int COB_NE = 6;

    private static String storedProcessUuid = null;
    private static String storedProcessId = null;

    /**
     * INDEXEDファイルのインスタンスを構築する。通常このコンストラクタは、生成されたJavaコードから{@link
     * CobolFileFactory#makeCobolFileInstance}を介して間接的に呼び出される。引数はCOBOLの{@code SELECT}／{@code
     * FD}宣言の属性に対応する。
     *
     * @param selectName {@code SELECT}句で指定された名前
     * @param fileStatus COBOLの{@code FILE STATUS}用ストレージ（2バイトのバッファ）
     * @param assign 割り当て名（ファイルパス）を保持するフィールド
     * @param record レコード領域を表すフィールド
     * @param recordSize 現在のレコード長を保持するフィールド
     * @param recordMin 最小レコード長
     * @param recordMax 最大レコード長
     * @param nkeys キーの数（主キー1つと副キーの合計）
     * @param keys キー記述子。インデックス{@code 0}が主キー
     * @param organization ファイル組織（このクラスでは{@code COB_ORG_INDEXED}）
     * @param accessMode アクセスモード（順次、動的、またはランダム）
     * @param lockMode ロックモードフラグ（{@code COB_LOCK_*}）
     * @param openMode 初期オープンモード
     * @param flagOptional ファイルが{@code OPTIONAL}と宣言されているか
     * @param lastOpenMode 直近のオープンモード
     * @param special 特殊ファイルフラグ
     * @param flagNonexistent ファイルが現在存在しないと判明しているか
     * @param flagEndOfFile カーソルがファイル終端に位置しているか
     * @param flagBeginOfFile カーソルがファイル先頭に位置しているか
     * @param flagFirstRead 次の読み込みが最初の読み込みか
     * @param flagReadDone 読み込みが実行済みか
     * @param flagSelectFeatures select機能フラグのOR（{@code FILE STATUS}、{@code LINAGE}、{@code
     *     EXTERNAL}）
     * @param flagNeedsNl 末尾の改行が必要か
     * @param flagNeedsTop 改ページ（先頭出し）が必要か
     * @param fileVersion ディスク上のフォーマットバージョン（{@code COB_FILE_VERSION}）
     */
    public CobolIndexedFile(
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
        super(
                selectName,
                fileStatus,
                assign,
                record,
                recordSize,
                recordMin,
                recordMax,
                nkeys,
                keys,
                organization,
                accessMode,
                lockMode,
                openMode,
                flagOptional,
                lastOpenMode,
                special,
                flagNonexistent,
                flagEndOfFile,
                flagBeginOfFile,
                flagFirstRead,
                flagReadDone,
                flagSelectFeatures,
                flagNeedsNl,
                flagNeedsTop,
                fileVersion);
    }

    private static String getProcessUuid() {
        if (CobolIndexedFile.storedProcessUuid == null) {
            CobolIndexedFile.storedProcessUuid = java.util.UUID.randomUUID().toString();
        }
        return CobolIndexedFile.storedProcessUuid;
    }

    private static String getProcessId() {
        if (CobolIndexedFile.storedProcessId == null) {
            CobolIndexedFile.storedProcessId =
                    String.valueOf(
                            java.lang.management.ManagementFactory.getRuntimeMXBean()
                                    .getName()
                                    .split("@")[0]);
        }
        return CobolIndexedFile.storedProcessId;
    }

    private static String getIndexName(int index) {
        return String.format("index%d", index);
    }

    private static String getSubIndexName(int index) {
        return String.format("subindex%d", index);
    }

    /**
     * 指定したインデックスのキーに対応するSQLiteテーブル名を返す。
     *
     * @param index キーのインデックス（主キーは{@code 0}、副キーは{@code 1}以上）
     * @return テーブル名（例：{@code "table0"}、{@code "table1"}、…）
     */
    public static String getTableName(int index) {
        return String.format("table%d", index);
    }

    /**
     * 指定したインデックスのキーに対応するカーソル名を返す。
     *
     * @param index キーのインデックス
     * @return カーソル名（例：{@code "cursor0"}、{@code "cursor1"}、…。現在は未使用だが互換性のため残されている）
     */
    public static String getCursorName(int index) {
        return String.format("cursor%d", index);
    }

    private static String getConstraintName(int index) {
        return String.format("constraint%d", index);
    }

    /**
     * 各更新文（{@code WRITE}／{@code REWRITE}／{@code DELETE}）がトランザクションを即座にコミットするかどうかを制御する。
     *
     * <p>デフォルトは{@code true}。{@code cobj-idx
     * load}のような一括ローダは、速度のためにレコードごとのコミットを抑制するため{@code false}に設定し、最後に{@link
     * #commitJdbcTransaction()}で一度だけコミットする。
     *
     * @param commitOnModification 更新のたびにコミットする場合は{@code true}、コミットを遅延させる場合は{@code false}
     */
    public void setCommitOnModification(boolean commitOnModification) {
        this.commitOnModification = commitOnModification;
    }

    /** Equivalent to DBT_SET in libcob/fileio.c */
    private byte[] DBT_SET(AbstractCobolField field) {
        return field.getDataStorage().getByteArray(0, field.getSize());
    }

    /** SQLiteがINDEXEDファイルの隣に作成する補助ファイルの接尾辞。 */
    private static final String[] SQLITE_AUXILIARY_SUFFIXES = {"-wal", "-shm", "-journal"};

    /**
     * INDEXEDファイルに付随するSQLiteの補助ファイル({@code -wal}／{@code -shm}／{@code -journal})を削除する。
     *
     * <p>通常これらはファイルを閉じた時点で削除される。残っているのは、ファイルを閉じずに終了したプロセスがあった場合である。
     * これらを消し残すとINDEXEDファイルを削除したはずのディレクトリにごみが残り、「1つのINDEXEDファイルはディスク上の1ファイル」という前提が崩れる。
     *
     * <p>ジャーナルモードの設定にかかわらず全種類を削除する。WALモードで運用した後に{@code
     * COB_INDEXED_JOURNAL_MODE=DELETE}へ切り替えた場合(およびその逆)も取り残されないようにするためである。
     *
     * @param filePath 削除された主ファイルのパス
     */
    @Override
    protected void deleteAuxiliaryFiles(Path filePath) {
        for (String suffix : SQLITE_AUXILIARY_SUFFIXES) {
            try {
                Files.deleteIfExists(Paths.get(filePath.toString() + suffix));
            } catch (IOException ignored) {
                // 補助ファイルが消せなくても主ファイルの削除は成功しているため、
                // DELETE FILE自体は成功として扱う。
            }
        }
    }

    @Override
    public int open_(String filename, int mode, int sharing) {
        IndexedFile p = new IndexedFile();
        this.filei = p;

        // If the file does not exist and the mode is COB_OPEN_INPUT, return ENOENT
        boolean fileExists = new java.io.File(filename).exists();
        if (mode == COB_OPEN_INPUT && !fileExists) {
            return ENOENT;
        }

        // Establish a connection to the database
        int getConnectionStatus = this.getConnection(filename);

        // If the connection could not be established, return the error code
        if (getConnectionStatus != COB_STATUS_00_SUCCESS) {
            return getConnectionStatus;
        }

        final boolean fileExistsOnOpen = fileExists;
        int status =
                retryOnSnapshotConflict(
                        () -> this.openWithConnection(filename, mode, fileExistsOnOpen));
        if (status != COB_STATUS_00_SUCCESS) {
            // The connection is only kept open for a successful OPEN. Closing it here also
            // covers the paths that used to leak it, such as an incompatible file version.
            try {
                p.connection.close();
            } catch (SQLException closeEx) {
                return COB_STATUS_30_PERMANENT_ERROR;
            }
        }
        return status;
    }

    /**
     * データベースへの接続が確立された後の{@code OPEN}処理を行う。
     *
     * <p>{@code
     * SQLITE_BUSY_SNAPSHOT}が発生した場合にやり直せるよう、接続の確立を含まない部分だけを切り出したもの。失敗しても接続はクローズしない。クローズは呼び出し元の{@link
     * #open_(String, int, int)}が行う。
     */
    private int openWithConnection(String filename, int mode, boolean fileExists) {
        IndexedFile p = this.filei;
        if (fileExists) {
            int code = this.checkVersionOld();
            if (code != COB_STATUS_00_SUCCESS) {
                return code;
            }
        }

        try {
            // Acquire a file lock
            boolean succeedToFileLock = this.acquireFileLock(filename, mode, fileExists);
            if (succeedToFileLock) {
                if (mode == COB_OPEN_OUTPUT) {
                    this.deleteAllTablesExceptForFileLockTable();
                }
                this.createAllTablesIfNotExists();
                if (mode == COB_OPEN_OUTPUT) {
                    this.writeMetaData(p);
                }
                p.connection.commit();
                this.setInitialParameters(filename);
                return COB_STATUS_00_SUCCESS;
            } else {
                return COB_STATUS_61_FILE_SHARING;
            }
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            return COB_STATUS_30_PERMANENT_ERROR;
        }
    }

    private int getConnection(String filename) {
        SqliteNativeLibrary.prepare();
        IndexedFile p = this.filei;
        // Establishes a connection to the SQLite database using the provided filename.
        SQLiteConfig config = new SQLiteConfig();
        config.setReadOnly(false);
        // Wait for finishing other processes' transactions up to 5 seconds.
        // This has to be part of the connection properties rather than a PRAGMA executed later,
        // because setAutoCommit(false) below issues BEGIN immediately and the timeout must
        // already be in effect by then.
        config.setBusyTimeout(BUSY_TIMEOUT_MILLIS);

        p.connection = null;
        try {
            p.connection =
                    DriverManager.getConnection("jdbc:sqlite:" + filename, config.toProperties());
            // PRAGMA journal_mode cannot be changed from within a transaction, and
            // setAutoCommit(false) issues BEGIN eagerly. It therefore has to be applied here,
            // while the connection is still in auto-commit mode.
            IndexedJournalConfig.applyTo(p.connection);
            p.connection.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE);
            p.connection.setAutoCommit(false);

            // Check if the file is accessible
            try (Statement st = p.connection.createStatement()) {
                st.execute("select 1");
            }
            p.connection.commit();
        } catch (SQLException e) {
            // The connection may already be open at this point. Leaving it behind would pin the
            // -wal/-shm files for the life of the JVM, which is exactly the debris this class
            // tries to avoid.
            closeConnectionQuietly(p);
            if (isBusy(e)) {
                return COB_STATUS_61_FILE_SHARING;
            } else {
                return COB_STATUS_30_PERMANENT_ERROR;
            }
        } catch (Exception e) {
            closeConnectionQuietly(p);
            return COB_STATUS_30_PERMANENT_ERROR;
        }
        return COB_STATUS_00_SUCCESS;
    }

    /** 接続が開いていれば閉じる。閉じられなくても、報告すべきは元の失敗のほうなので無視する。 */
    private static void closeConnectionQuietly(IndexedFile p) {
        if (p.connection == null) {
            return;
        }
        try {
            p.connection.close();
        } catch (SQLException ignored) {
            // 元の失敗を上書きしないよう、ここでの失敗は無視する。
        }
        p.connection = null;
    }

    /**
     * 例外がSQLiteのビジー状態を表すかどうかを判定する。
     *
     * <p>WALモードで発生する{@code SQLITE_BUSY_SNAPSHOT}も主エラーコードは{@code
     * SQLITE_BUSY}と同じ{@code 5}であるため、両者はここでまとめて扱われる。
     */
    private static boolean isBusy(SQLException e) {
        return e.getErrorCode() == SQLiteErrorCode.SQLITE_BUSY.code;
    }

    /** {@link #retryOnSnapshotConflict}に渡す、ステータスコードを返す入出力処理。 */
    private interface IndexedIoOperation {
        int run();
    }

    /**
     * {@code SQLITE_BUSY_SNAPSHOT}が発生した場合にトランザクションをロールバックして処理をやり直す。
     *
     * <p>WALモードでは、読み取りを行ってから書き込みを行うトランザクションの実行中に他の接続がコミットすると、書き込み時に{@code
     * SQLITE_BUSY_SNAPSHOT}が返る。これは{@code
     * busy_timeout}では再試行されないため、ここで明示的にやり直す必要がある。
     *
     * <p>{@link IndexedCursor}は取得位置を保持するステートフルなカーソルであるため、{@code READ
     * NEXT}のようにカーソルを進める処理をこのメソッドで丸ごと包んではならない。やり直しのたびにカーソルが進み、レコードを読み飛ばしてしまう。
     *
     * @param operation やり直しても副作用が変わらない処理
     * @return {@code operation}の戻り値。リトライ回数を使い切った場合は{@code
     *     COB_STATUS_61_FILE_SHARING}、ロールバックに失敗した場合は{@code COB_STATUS_30_PERMANENT_ERROR}
     */
    private int retryOnSnapshotConflict(IndexedIoOperation operation) {
        for (int attempt = 0; ; ++attempt) {
            try {
                return operation.run();
            } catch (SnapshotConflictException e) {
                if (!this.commitOnModification) {
                    // A bulk load keeps one transaction open across many statements, so rolling
                    // back here would silently discard every record written so far and the load
                    // would still report success. Report the conflict instead.
                    // This is currently unreachable: the first successful write of the bulk
                    // transaction takes the WAL writer lock and holds it until COMMIT, so no other
                    // connection can commit and cause a snapshot conflict afterwards.
                    return COB_STATUS_61_FILE_SHARING;
                }
                try {
                    this.filei.connection.rollback();
                } catch (SQLException rollbackEx) {
                    return COB_STATUS_30_PERMANENT_ERROR;
                }
                if (attempt >= MAX_SNAPSHOT_RETRIES) {
                    return COB_STATUS_61_FILE_SHARING;
                }
                try {
                    // Back off a little further on each attempt, with a per-process offset so
                    // that two contending processes do not resynchronise on the same schedule.
                    long backoff = Math.min(attempt + 1L, MAX_SNAPSHOT_BACKOFF_MILLIS);
                    Thread.sleep(backoff + (getProcessUuid().hashCode() & 3));
                } catch (InterruptedException interrupted) {
                    Thread.currentThread().interrupt();
                    return COB_STATUS_30_PERMANENT_ERROR;
                }
            }
        }
    }

    private int checkVersionOld() {
        IndexedFile p = this.filei;
        try (Statement st = p.connection.createStatement()) {
            String fileLockTableExistsSql =
                    "select exists(select 1 from sqlite_master where type = 'table' and name ="
                            + " 'file_lock')";
            ResultSet fileLockTableExistsResultSet = st.executeQuery(fileLockTableExistsSql);
            if (fileLockTableExistsResultSet.next()) {
                boolean fileLockTableExists = fileLockTableExistsResultSet.getInt(1) == 1;
                if (!fileLockTableExists) {
                    return COB_STATUS_92_VERSION_INCOMPATIBLE;
                }
            } else {
                return COB_STATUS_92_VERSION_INCOMPATIBLE; // file_lock table does not exist
            }

            boolean lockedByColumnExists = false;
            boolean processIdColumnExists = false;
            boolean lockedAtColumnExists = false;
            try (ResultSet rs = st.executeQuery("PRAGMA table_info('table0')")) {
                while (rs.next()) {
                    String columnName = rs.getString("name");
                    if ("locked_by".equals(columnName)) {
                        lockedByColumnExists = true;
                    } else if ("process_id".equals(columnName)) {
                        processIdColumnExists = true;
                    } else if ("locked_at".equals(columnName)) {
                        lockedAtColumnExists = true;
                    }
                    if (lockedByColumnExists && processIdColumnExists && lockedAtColumnExists) {
                        return COB_STATUS_00_SUCCESS; // All required columns exist
                    }
                }
            }
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            return COB_STATUS_30_PERMANENT_ERROR;
        }
        return COB_STATUS_92_VERSION_INCOMPATIBLE;
    }

    private String getOpenModeString(int mode) {
        switch (mode) {
            case COB_OPEN_INPUT:
                return "INPUT";
            case COB_OPEN_OUTPUT:
                return "OUTPUT";
            case COB_OPEN_I_O:
                return "I-O";
            case COB_OPEN_EXTEND:
                return "EXTEND";
            default:
                return null;
        }
    }

    private boolean acquireFileLock(String filename, int mode, boolean fileExists)
            throws SQLException {
        if (!checkFileIsLocked(filename, mode, fileExists)) {
            return false; // File is already locked
        }

        IndexedFile p = this.filei;

        // Insert a new lock record into the file_lock table
        String openMode = this.getOpenModeString(mode);
        if (openMode == null) {
            return false; // Invalid open mode
        }
        String insertSql =
                "insert into file_lock (locked_by, process_id, locked_at, open_mode) values (?, ?,"
                        + " datetime('now'), ?)";
        String processUuid = this.getProcessUuid();
        String processId = this.getProcessId();

        try (PreparedStatement statement = p.connection.prepareStatement(insertSql)) {
            statement.setString(1, processUuid);
            statement.setString(2, processId);
            statement.setString(3, openMode);
            int insertedRecordsCount = statement.executeUpdate();
            if (insertedRecordsCount != 1) {
                p.connection.rollback();
                return false;
            }
            p.connection.commit();
            return true;
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            p.connection.rollback();
            return false;
        }
    }

    private boolean checkFileIsLocked(String filename, int mode, boolean fileExists)
            throws SQLException {
        IndexedFile p = this.filei;
        try (Statement statement = p.connection.createStatement()) {
            if (mode == COB_OPEN_OUTPUT
                    || (!fileExists && (mode == COB_OPEN_EXTEND || mode == COB_OPEN_I_O))) {
                String query =
                        "select exists(select 1 from sqlite_master where type = 'table' and name ="
                                + " 'file_lock')";
                try (ResultSet rs = statement.executeQuery(query)) {
                    // If the file_lock table does not exist, create it and return true
                    if (!rs.next() || rs.getInt(1) == 0) {
                        statement.execute(
                                "CREATE TABLE if not exists file_lock (locked_by text primary key,"
                                    + " process_id text, locked_at timestamp, open_mode text"
                                    + " CONSTRAINT check_open_mode CHECK (open_mode IN ('INPUT',"
                                    + " 'OUTPUT', 'I-O', 'EXTEND')))");
                        return true;
                    }
                }
            }

            String query;
            if (mode == COB_OPEN_OUTPUT) {
                query = "select exists(select 1 from file_lock)";
            } else {
                query = "select exists(select 1 from file_lock where open_mode = 'OUTPUT')";
            }
            try (ResultSet rs = statement.executeQuery(query)) {
                // If the file is already locked, return false
                if (rs.next() && rs.getInt(1) == 1) {
                    p.connection.rollback();
                    return false;
                }
            }
            return true;
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            p.connection.rollback();
            return false;
        }
    }

    private void deleteAllTablesExceptForFileLockTable() throws SQLException {
        IndexedFile p = this.filei;
        try (Statement statement = p.connection.createStatement()) {
            for (int i = 0; i < this.nkeys; ++i) {
                statement.execute("drop table if exists " + getTableName(i));
            }
            statement.execute("drop table if exists metadata_string_int");
            statement.execute("drop table if exists metadata_key");
        }
    }

    private void createAllTablesIfNotExists() throws SQLException {
        IndexedFile p = this.filei;
        try (Statement statement = p.connection.createStatement()) {
            for (int i = 0; i < this.nkeys; ++i) {
                String tableName = getTableName(i);
                if (i == 0) {
                    statement.execute(
                            String.format(
                                    "create table if not exists %s ("
                                            + "key blob not null primary key,"
                                            + "value blob not null,"
                                            + "locked_by text,"
                                            + "process_id text,"
                                            + "locked_at timestamp"
                                            + ")",
                                    tableName));
                } else {
                    if (this.keys[i].getFlag() == 0) {
                        statement.execute(
                                String.format(
                                        "create table if not exists %s (key blob not null primary"
                                            + " key, value blob not null, constraint %s foreign key"
                                            + " (value) references %s (key))",
                                        tableName, getConstraintName(i), getTableName(0)));
                    } else {
                        statement.execute(
                                String.format(
                                        "create table if not exists %s (key blob not null, value"
                                            + " blob not null, dupNo integer not null, constraint"
                                            + " %s foreign key (value) references %s (key))",
                                        tableName, getConstraintName(i), getTableName(0)));
                    }
                    statement.execute(
                            String.format(
                                    "create index if not exists %s on %s(value)",
                                    getSubIndexName(i), tableName));
                }
                statement.execute(
                        String.format(
                                "create index if not exists %s on %s(key)",
                                getIndexName(i), tableName));
            }
        }
    }

    private void setInitialParameters(String filename) {
        IndexedFile p = this.filei;
        p.filenamelen = filename.length();
        p.last_dupno = new int[this.nkeys];
        p.rewrite_sec_key = new int[this.nkeys];

        int maxsize = 0;
        for (int i = 0; i < this.nkeys; ++i) {
            if (this.keys[i].getField().getSize() > maxsize) {
                maxsize = this.keys[i].getField().getSize();
            }
        }

        p.temp_key = new CobolDataStorage(maxsize + 4);
        p.key_index = 0;
        p.last_key = null;

        p.filename = filename;
        p.write_cursor_open = false;
        p.record_locked = false;

        p.key = DBT_SET(this.keys[0].getField());
        this.updateWhileReading = false;
        this.indexedFirstRead = true;
        this.callStart = false;

        this.fetchKeyIndex = -1;
    }

    // Write a metadata to the database
    private void writeMetaData(IndexedFile p) throws SQLException {
        Statement statement = p.connection.createStatement();
        // Create a table to store metadata
        statement.execute(
                "create table if not exists metadata_string_int (key text not null primary key,"
                        + " value integer not null)");
        statement.execute(
                "create table if not exists metadata_key (idx integer not null primary key, offset"
                        + " integer not null, size integer not null, duplicate boolean)");
        statement.close();

        // Store the size of a record
        PreparedStatement recordSizePreparedStmt =
                p.connection.prepareStatement(
                        "insert into metadata_string_int values ('record_size', ?)");
        recordSizePreparedStmt.setInt(1, this.record_max);
        recordSizePreparedStmt.execute();
        recordSizePreparedStmt.close();

        // Store information of all keys
        PreparedStatement keyPreparedStmt =
                p.connection.prepareStatement("insert into metadata_key values (?, ?, ?, ?)");
        for (int i = 0; i < this.nkeys; i++) {
            keyPreparedStmt.setInt(1, i);
            keyPreparedStmt.setInt(
                    2,
                    this.keys[i].getField().getDataStorage().getIndex()
                            - this.record.getDataStorage().getIndex());
            keyPreparedStmt.setInt(3, this.keys[i].getField().getSize());
            keyPreparedStmt.setBoolean(4, this.keys[i].getFlag() != 0);
            keyPreparedStmt.execute();
        }
        keyPreparedStmt.close();
    }

    @Override
    public int close_(int opt) {
        int status = retryOnSnapshotConflict(() -> this.closeOnce(opt));
        if (status != COB_STATUS_00_SUCCESS) {
            // closeOnce did not reach connection.close(). Closing here keeps the failure from
            // pinning the connection and the -wal/-shm files for the life of the JVM. The
            // file_lock row stays behind, which is what `cobj-idx unlock` exists for.
            closeConnectionQuietly(this.filei);
            this.fetchKeyIndex = -1;
        }
        return status;
    }

    /** {@link #close_(int)}の本体。{@code SQLITE_BUSY_SNAPSHOT}が発生した場合はそのままやり直せる。 */
    private int closeOnce(int opt) {
        IndexedFile p = this.filei;

        this.closeCursor();

        previousLockedRecordKey = null;

        try {
            try (Statement statement = p.connection.createStatement()) {
                // Close the file lock
                String deleteFileLockSql = "delete from file_lock where locked_by = ?";
                try (PreparedStatement deleteFileLockStatement =
                        p.connection.prepareStatement(deleteFileLockSql)) {
                    deleteFileLockStatement.setString(1, this.getProcessUuid());
                    deleteFileLockStatement.executeUpdate();
                }
                String unlockRecordsSql =
                        String.format(
                                "update %s set locked_by = null, process_id = null, locked_at ="
                                        + " null where locked_by = ?",
                                getTableName(0));
                try (PreparedStatement unlockRecordsStatement =
                        p.connection.prepareStatement(unlockRecordsSql)) {
                    unlockRecordsStatement.setString(1, this.getProcessUuid());
                    unlockRecordsStatement.executeUpdate();
                }
            }
            p.connection.commit();
            p.connection.close();
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            return COB_STATUS_30_PERMANENT_ERROR;
        }
        this.fetchKeyIndex = -1;
        return COB_STATUS_00_SUCCESS;
    }

    // Equivalent to indexed_start_internal in libcob/fileio.c
    /**
     * 指定したキーと比較条件に一致する最初のレコードにカーソルを位置づける。
     *
     * <p>キーフィールドのアドレスを宣言済みのキーと照合してどのキー（したがってどのSQLiteテーブル）を使うかを判定し、そのキー向けに新しい{@link
     * IndexedCursor}を生成して、1件のレコードをフェッチする。成功時には現在のキー／レコードバッファ（{@code p.key}／{@code
     * p.data}）が更新される。これは{@link #start_(int,
     * AbstractCobolField)}とランダムな{@link #read_(AbstractCobolField, int)}の共通実装である。
     *
     * @param cond 比較条件（{@link #COB_EQ}、{@link #COB_GT}、{@link #COB_GE}、{@link
     *     #COB_LT}、{@link #COB_LE}, {@link #COB_NE}のいずれか。
     * @param key 検索するキー値を保持するフィールド
     * @param readOpts 現在未使用
     * @param testLock レコードロックをテストするか（現状このメソッドでは未使用）
     * @return condが {@link #COB_NE}の場合 {@code COB_STATUS_23_KEY_NOT_EXISTS}。
     *     そうでない場合、レコードが見つかった場合は{@code COB_STATUS_00_SUCCESS}、一致するものがなかった場合は{@code COB_STATUS_23_KEY_NOT_EXISTS}、カーソル生成に失敗した場合は{@code
     *     COB_STATUS_30_PERMANENT_ERROR}
     */
    public int indexed_start_internal(
            int cond, AbstractCobolField key, int readOpts, boolean testLock) {
        IndexedFile p = this.filei;
        for (p.key_index = 0; p.key_index < this.nkeys; p.key_index++) {
            if (this.keys[p.key_index].getField().getDataStorage().isSame(key.getDataStorage())) {
                break;
            }
        }
        this.fetchKeyIndex = p.key_index;

        p.key = DBT_SET(key);

        boolean isDuplicate = this.keys[p.key_index].getFlag() != 0;

        this.cursor =
                IndexedCursor.createCursor(p.connection, p.key, p.key_index, isDuplicate, cond);
        if (!this.cursor.isPresent()) {
            return COB_STATUS_30_PERMANENT_ERROR;
        }

        IndexedCursor cursor = this.cursor.get();
        Optional<FetchResult> optionalResult = cursor.next();
        if (optionalResult.isPresent()) {
            FetchResult result = optionalResult.get();
            p.key = result.key;
            p.data = result.value;
            this.indexedFirstRead = false;
            return COB_STATUS_00_SUCCESS;
        } else {
            return COB_STATUS_23_KEY_NOT_EXISTS;
        }
    }

    @Override
    public int start_(int cond, AbstractCobolField key) {
        return retryOnSnapshotConflict(() -> this.startOnce(cond, key));
    }

    /**
     * {@link #start_(int, AbstractCobolField)}の本体。
     *
     * <p>カーソルはキーから作り直されるため、{@code SQLITE_BUSY_SNAPSHOT}が発生した場合はそのままやり直せる。
     */
    private int startOnce(int cond, AbstractCobolField key) {
        int ret = indexed_start_internal(cond, key, 0, false);
        if (ret == COB_STATUS_00_SUCCESS) {
            this.callStart = true;
        }
        try {
            IndexedFile p = this.filei;
            p.connection.commit();
        } catch (SQLException rollbackEx) {
            return COB_STATUS_30_PERMANENT_ERROR;
        }
        return ret;
    }

    private boolean shouldLockRecord(int readOpts) {
        if (this.open_mode != COB_OPEN_I_O) {
            return false;
        }
        if ((readOpts & CobolFile.COB_READ_LOCK) != 0) {
            return true;
        } else if ((readOpts & CobolFile.COB_READ_NO_LOCK) != 0) {
            return false;
        } else {
            return this.lock_mode != CobolFile.COB_LOCK_MANUAL;
        }
    }

    private boolean checkOtherProcessLockedRecord(byte[] key) throws SQLException {
        IndexedFile p = this.filei;
        String query =
                String.format(
                        "select exists(select 1 from %s where key = ? and locked_by != ?)",
                        getTableName(0));
        try (PreparedStatement selectStatement = p.connection.prepareStatement(query)) {
            selectStatement.setBytes(1, key);
            selectStatement.setString(2, this.getProcessUuid());
            try (ResultSet rs = selectStatement.executeQuery()) {
                return rs.next() && rs.getInt(1) == 1;
            }
        }
    }

    private boolean lockRecord(byte[] key) throws SQLException {
        IndexedFile p = this.filei;
        String updateSql =
                String.format(
                        "update %s set locked_by = ?, process_id = ?, locked_at = datetime('now')"
                                + " where key = ?",
                        getTableName(0));
        try (PreparedStatement updateStatement = p.connection.prepareStatement(updateSql)) {
            updateStatement.setString(1, this.getProcessUuid());
            updateStatement.setString(2, this.getProcessId());
            updateStatement.setBytes(3, key);
            int updatedRecordcount = updateStatement.executeUpdate();
            return updatedRecordcount == 1;
        }
    }

    private void unlockPreviousRecord() throws SQLException {
        if (previousLockedRecordKey == null) {
            previousLockedRecordKey = null;
            return;
        }
        IndexedFile p = this.filei;
        String updateSql =
                String.format(
                        "update %s set locked_by = null, process_id = null, locked_at = null where"
                                + " key = ? and locked_by = ?",
                        getTableName(0));
        try (PreparedStatement updateStatement = p.connection.prepareStatement(updateSql)) {
            updateStatement.setBytes(1, previousLockedRecordKey);
            updateStatement.setString(2, this.getProcessUuid());
            updateStatement.executeUpdate();
        }
        previousLockedRecordKey = null;
    }

    @Override
    protected boolean postProcess() {
        return retryOnSnapshotConflict(this::postProcessOnce) == COB_STATUS_00_SUCCESS;
    }

    /** {@link #postProcess()}の本体。{@code SQLITE_BUSY_SNAPSHOT}が発生した場合はそのままやり直せる。 */
    private int postProcessOnce() {
        try {
            unlockPreviousRecord();
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            return COB_STATUS_30_PERMANENT_ERROR;
        }
        return COB_STATUS_00_SUCCESS;
    }

    private void unlockPreviousRecord(byte[] key) throws SQLException {
        if (previousLockedRecordKey == null) {
            previousLockedRecordKey = key;
            return;
        }
        IndexedFile p = this.filei;
        String updateSql =
                String.format(
                        "update %s set locked_by = null, process_id = null, locked_at = null where"
                                + " key = ? and locked_by = ?",
                        getTableName(0));
        try (PreparedStatement updateStatement = p.connection.prepareStatement(updateSql)) {
            updateStatement.setBytes(1, previousLockedRecordKey);
            updateStatement.setString(2, this.getProcessUuid());
            updateStatement.executeUpdate();
        }
        previousLockedRecordKey = key;
    }

    private int read_internal(AbstractCobolField key, int readOpts) {
        boolean testLock = false;
        this.callStart = false;
        int ret = this.indexed_start_internal(COB_EQ, key, readOpts, testLock);
        if (ret != COB_STATUS_00_SUCCESS) {
            return ret;
        }

        return COB_STATUS_00_SUCCESS;
    }

    @Override
    public int read_(AbstractCobolField key, int readOpts) {
        IndexedFile p = this.filei;
        int retCode = read_internal(key, readOpts);
        if (retCode != COB_STATUS_00_SUCCESS) {
            // This releases the previous record lock with an UPDATE after the reads above, so it
            // can hit SQLITE_BUSY_SNAPSHOT. Without the retry a plain "no more records" would be
            // reported as a permanent error under concurrency.
            int unlockStatus = retryOnSnapshotConflict(this::releasePreviousRecordLock);
            if (unlockStatus != COB_STATUS_00_SUCCESS) {
                return unlockStatus;
            }
            return retCode;
        }
        byte[] primaryKey = DBT_SET(this.keys[0].getField());
        // Only the locking phase is retried. Re-running read_internal would be wasteful here and
        // is outright wrong for READ NEXT, so both verbs share this narrow retry scope.
        int lockStatus = retryOnSnapshotConflict(() -> lockReadRecord(primaryKey, readOpts));
        if (lockStatus != COB_STATUS_00_SUCCESS) {
            return lockStatus;
        }

        this.record.setSize(p.data.length);
        this.record.getDataStorage().memcpy(p.data, p.data.length);
        return COB_STATUS_00_SUCCESS;
    }

    /**
     * {@code READ}で読み込んだレコードに対するロック処理を行う。
     *
     * <p>{@link #read_(AbstractCobolField, int)}と{@link
     * #readNext(int)}が読み込みに成功した後に共通で実行する部分であり、レコードの取得そのものを含まないため、{@code
     * SQLITE_BUSY_SNAPSHOT}が発生してもそのままやり直せる。
     *
     * @param primaryKey 読み込んだレコードの主キー
     * @param readOpts {@code READ}のオプション
     * @return 正常時は{@code COB_STATUS_00_SUCCESS}
     */
    /**
     * 直前にロックしたレコードのロックを解放してコミットする。
     *
     * <p>{@code READ}／{@code READ NEXT}がレコードを取得できなかった場合の後始末であり、キーを保持したまま何度でもやり直せる({@link
     * #unlockPreviousRecord()}は更新が成功してから{@code previousLockedRecordKey}をクリアする)。
     *
     * @return 正常時は{@code COB_STATUS_00_SUCCESS}
     */
    private int releasePreviousRecordLock() {
        IndexedFile p = this.filei;
        try {
            unlockPreviousRecord();
            p.connection.commit();
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            return COB_STATUS_30_PERMANENT_ERROR;
        }
        return COB_STATUS_00_SUCCESS;
    }

    private int lockReadRecord(byte[] primaryKey, int readOpts) {
        IndexedFile p = this.filei;
        if (shouldLockRecord(readOpts)) {
            try {
                if (checkOtherProcessLockedRecord(primaryKey)) {
                    p.connection.rollback();
                    unlockPreviousRecord();
                    p.connection.commit();
                    return COB_STATUS_51_RECORD_LOCKED;
                }
                if (!lockRecord(primaryKey)) {
                    p.connection.rollback();
                    unlockPreviousRecord();
                    p.connection.commit();
                    return COB_STATUS_30_PERMANENT_ERROR;
                }
                unlockPreviousRecord(primaryKey);
                p.connection.commit();
            } catch (SQLException e) {
                SnapshotConflictException.rethrowIfSnapshotConflict(e);
                try {
                    p.connection.rollback();
                } catch (SQLException rollbackEx) {
                    return COB_STATUS_30_PERMANENT_ERROR;
                }
                return COB_STATUS_30_PERMANENT_ERROR;
            }
        } else {
            try {
                unlockPreviousRecord(primaryKey);
                p.connection.commit();
            } catch (SQLException e) {
                SnapshotConflictException.rethrowIfSnapshotConflict(e);
                try {
                    p.connection.rollback();
                } catch (SQLException rollbackEx) {
                    return COB_STATUS_30_PERMANENT_ERROR;
                }
                return COB_STATUS_30_PERMANENT_ERROR;
            }
        }
        return COB_STATUS_00_SUCCESS;
    }

    private int readNext_internal(int readOpts) {
        IndexedFile p = this.filei;
        // Immediately after START is called
        if (this.callStart) {
            this.callStart = false;
            this.indexedFirstRead = false;
            // If this statement is READ NEXT RECORD and
            // the START statement was like `START ~~ KEY IS <=`
            if ((readOpts & CobolFile.COB_READ_NEXT) != 0
                    && cursor.get().getComparator() == COB_LE) {
                AbstractCobolField specifiedKey = this.keys[p.key_index].getField();
                int specifiedKeySize = specifiedKey.getSize();
                int readKeySize = p.key.length;
                int length = Math.min(specifiedKeySize, readKeySize);
                // The key fetched by START is not the same as the specified key
                if (specifiedKey.getDataStorage().memcmp(p.key, length) != 0) {
                    // READ the next record
                    return readNext_internal(readOpts);
                }
            }
            return COB_STATUS_00_SUCCESS;
        }

        boolean isDuplicate = this.keys[p.key_index].getFlag() != 0;
        if (this.indexedFirstRead || this.flag_begin_of_file) {
            this.cursor =
                    IndexedCursor.createCursor(
                            p.connection, p.key, p.key_index, isDuplicate, COB_GE);
            if (!this.cursor.isPresent()) {
                return COB_STATUS_10_END_OF_FILE;
            }
            this.cursor.get().moveToFirst();
        } else if (this.flag_end_of_file) {
            this.cursor =
                    IndexedCursor.createCursor(
                            p.connection, p.key, p.key_index, isDuplicate, COB_LE);
            if (!this.cursor.isPresent()) {
                return COB_STATUS_30_PERMANENT_ERROR;
            }
            this.cursor.get().moveToLast();
        } else if (this.updateWhileReading) {
            this.updateWhileReading = false;
            if (!this.cursor.isPresent()) {
                return COB_STATUS_30_PERMANENT_ERROR;
            }
            IndexedCursor oldCursor = this.cursor.get();
            Optional<IndexedCursor> newCursor = oldCursor.reloadCursor();
            if (!newCursor.isPresent()) {
                this.cursor = Optional.of(oldCursor);
            } else {
                oldCursor.close();
                this.cursor = newCursor;
            }
        }

        if (!this.cursor.isPresent()) {
            return COB_STATUS_30_PERMANENT_ERROR;
        }

        IndexedCursor cursor = this.cursor.get();

        final CursorReadOption cursorOpt;
        if ((readOpts & COB_READ_PREVIOUS) != 0) {
            cursorOpt = CursorReadOption.PREV;
            cursor.setComparator(COB_LE);
        } else {
            cursorOpt = CursorReadOption.NEXT;
            cursor.setComparator(COB_GE);
        }

        Optional<FetchResult> optionalResult = cursor.read(cursorOpt);

        if (!optionalResult.isPresent()) {
            this.indexedFirstRead = false;
            return COB_STATUS_10_END_OF_FILE;
        }

        FetchResult result = optionalResult.get();
        p.key = result.key;
        p.data = result.value;

        this.indexedFirstRead = false;
        return COB_STATUS_00_SUCCESS;
    }

    @Override
    public int readNext(int readOpts) {
        IndexedFile p = this.filei;
        int retCode = readNext_internal(readOpts);
        if (retCode != COB_STATUS_00_SUCCESS) {
            // This releases the previous record lock with an UPDATE after the reads above, so it
            // can hit SQLITE_BUSY_SNAPSHOT. Without the retry a plain "no more records" would be
            // reported as a permanent error under concurrency.
            int unlockStatus = retryOnSnapshotConflict(this::releasePreviousRecordLock);
            if (unlockStatus != COB_STATUS_00_SUCCESS) {
                return unlockStatus;
            }
            return retCode;
        }
        byte[] primaryKey = DBT_SET(this.keys[0].getField());
        // Retrying readNext_internal would advance the stateful cursor a second time and skip a
        // record, so only the locking phase is retried.
        int lockStatus = retryOnSnapshotConflict(() -> lockReadRecord(primaryKey, readOpts));
        if (lockStatus != COB_STATUS_00_SUCCESS) {
            return lockStatus;
        }
        this.record.setSize(p.data.length);
        this.record.getDataStorage().memcpy(p.data, p.data.length);
        return COB_STATUS_00_SUCCESS;
    }

    private void closeCursor() {
        if (this.cursor != null) {
            if (this.cursor.isPresent()) {
                this.cursor.get().close();
            }
        }
    }

    private boolean keyExistsInTable(IndexedFile p, int index, byte[] key) {
        String query = String.format("select * from %s where key = ?", getTableName(index));
        try (PreparedStatement selectStatement = p.connection.prepareStatement(query)) {
            selectStatement.setBytes(1, key);
            selectStatement.setFetchSize(0);
            try (ResultSet rs = selectStatement.executeQuery()) {
                return rs.next();
            }
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            return false;
        }
    }

    private boolean isDuplicateColumn(int index) {
        return this.keys[index].getFlag() != 0;
    }

    private int getNextKeyDupNo(Connection conn, int index, byte[] key) {
        try {
            PreparedStatement selectStatement =
                    conn.prepareStatement(
                            String.format(
                                    "select ifnull(max(dupNo), -1) from %s", getTableName(index)));
            try (ResultSet rs = selectStatement.executeQuery()) {
                return rs.getInt(1) + 1;
            }
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            return 0;
        }
    }

    private int returnWith(IndexedFile p, boolean closeCursor, int index, int returnCode) {
        if (closeCursor) {
            this.closeCursor();
            p.write_cursor_open = false;
        }
        return returnCode;
    }

    private int indexed_write_internal(boolean rewrite, int opt) {
        return this.indexed_write_internal(rewrite, null, opt);
    }

    /** Equivalent to indexed_write_internal in libcob/fileio.c */
    private int indexed_write_internal(boolean rewrite, int[] dupNumbers, int opt) {
        IndexedFile p = this.filei;

        boolean closeCursor;
        p.write_cursor_open = true;
        closeCursor = true;

        if (this.nkeys > 1 && !rewrite) {
            if (this.check_alt_keys(false)) {
                return returnWith(p, closeCursor, 0, COB_STATUS_22_KEY_EXISTS);
            }
            p.key = DBT_SET(this.keys[0].getField());
        }

        if (keyExistsInTable(p, 0, p.key)) {
            return COB_STATUS_22_KEY_EXISTS;
        }

        // insert into the primary table
        p.data = DBT_SET(this.record);
        try (PreparedStatement insertStatement =
                p.connection.prepareStatement(
                        String.format(
                                "insert into %s (key, value, locked_by, process_id, locked_at) "
                                        + "values (?, ?, null, null, null)",
                                getTableName(0)))) {
            insertStatement.setBytes(1, p.key);
            insertStatement.setBytes(2, p.data);
            insertStatement.execute();
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            return returnWith(p, closeCursor, 0, COB_STATUS_51_RECORD_LOCKED);
        }

        p.data = p.key;

        // insert into sub tables
        for (int i = 1; i < this.nkeys; i++) {

            p.key = DBT_SET(this.keys[i].getField());
            try {
                if (!isDuplicateColumn(i) && keyExistsInTable(p, i, p.key)) {
                    return returnWith(p, closeCursor, 0, COB_STATUS_22_KEY_EXISTS);
                }

                PreparedStatement insertStatement;
                if (isDuplicateColumn(i)) {
                    int dupNo;
                    if (dupNumbers == null || dupNumbers[i] < 0 || i != this.fetchKeyIndex) {
                        dupNo = getNextKeyDupNo(p.connection, i, p.key);
                    } else {
                        dupNo = dupNumbers[i];
                    }
                    insertStatement =
                            p.connection.prepareStatement(
                                    String.format(
                                            "insert into %s values (?, ?, ?)", getTableName(i)));
                    insertStatement.setBytes(1, p.key);
                    insertStatement.setBytes(2, p.data);
                    insertStatement.setInt(3, dupNo);
                } else {
                    insertStatement =
                            p.connection.prepareStatement(
                                    String.format("insert into %s values (?, ?)", getTableName(i)));
                    insertStatement.setBytes(1, p.key);
                    insertStatement.setBytes(2, p.data);
                }
                insertStatement.execute();
                insertStatement.close();
            } catch (SQLException e) {
                SnapshotConflictException.rethrowIfSnapshotConflict(e);
                return returnWith(p, closeCursor, 0, COB_STATUS_51_RECORD_LOCKED);
            }
        }

        this.updateWhileReading = true;

        return returnWith(p, closeCursor, 0, COB_STATUS_00_SUCCESS);
    }

    @Override
    public int write_(int opt) {
        return retryOnSnapshotConflict(() -> this.writeOnce(opt));
    }

    /**
     * {@link #write_(int)}の本体。
     *
     * <p>キーは毎回{@code this.keys}から導出し直され、{@code p.last_key}にも同じ値が入るため、{@code
     * SQLITE_BUSY_SNAPSHOT}が発生した場合はそのままやり直せる。
     */
    private int writeOnce(int opt) {
        IndexedFile p = this.filei;

        p.key = DBT_SET(this.keys[0].getField());
        if (p.last_key == null) {
            p.last_key = new CobolDataStorage(p.key.length);

        } else if (this.access_mode == COB_ACCESS_SEQUENTIAL) {
            byte[] keyBytes = p.key;
            if (p.last_key.memcmp(keyBytes, keyBytes.length) > 0) {
                try {
                    unlockPreviousRecord();
                    p.connection.commit();
                } catch (SQLException e) {
                    SnapshotConflictException.rethrowIfSnapshotConflict(e);
                    return COB_STATUS_30_PERMANENT_ERROR;
                }
                return COB_STATUS_21_KEY_INVALID;
            }
        }

        byte[] keyBytes = p.key;
        p.last_key.memcpy(keyBytes, keyBytes.length);

        int ret = indexed_write_internal(false, opt);
        if (ret == COB_STATUS_00_SUCCESS) {
            try {
                unlockPreviousRecord();
                if (this.commitOnModification) {
                    p.connection.commit();
                }
            } catch (SQLException e) {
                SnapshotConflictException.rethrowIfSnapshotConflict(e);
                return COB_STATUS_30_PERMANENT_ERROR;
            }
        } else {
            try {
                p.connection.rollback();
                unlockPreviousRecord();
                p.connection.commit();
            } catch (SQLException rollbackEx) {
                return ret;
            }
        }
        return ret;
    }

    /** Equivalent to check_alt_keys in libcob/fileio.c */
    private boolean check_alt_keys(boolean rewrite) {
        IndexedFile p = this.filei;

        byte[] primaryKey = DBT_SET(this.keys[0].getField());
        for (int i = 1; i < this.nkeys; ++i) {
            if (this.keys[i].getFlag() == 0) {
                p.key = DBT_SET(this.keys[i].getField());
                if (rewrite) {
                    if (checkTable(p, i, p.key, primaryKey)) {
                        return true;
                    }
                } else {
                    if (keyExistsInTable(p, i, p.key)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    private static boolean checkTable(IndexedFile p, int index, byte[] key, byte[] primaryKey) {
        String query =
                String.format(
                        "select key from %s " + "where key = ? and value = ?", getTableName(index));
        try (PreparedStatement selectStatement = p.connection.prepareStatement(query)) {
            selectStatement.setBytes(1, key);
            selectStatement.setBytes(2, primaryKey);
            selectStatement.setFetchSize(0);
            try (ResultSet rs = selectStatement.executeQuery()) {
                return rs.next();
            }
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            return false;
        }
    }

    @Override
    /** Equivalent to indexed_rewrite in libcob/fileio.c */
    public int rewrite_(int opt) {
        return retryOnSnapshotConflict(() -> this.rewriteOnce(opt));
    }

    /**
     * {@link #rewrite_(int)}の本体。
     *
     * <p>主キーから対象レコードを特定し直すため、{@code SQLITE_BUSY_SNAPSHOT}が発生した場合はそのままやり直せる。
     *
     * <p>ただしやり直せるのは{@code
     * indexed_write_internal}に入る前までである。この中では{@code p.key}が副キーの値で上書きされるため、そこから先でやり直すと{@code
     * matchKeyHead}の比較対象が壊れる。実際には競合が起きうるのは最初の{@code lockRecord}の時点だけであり、{@code
     * p.key}はまだ書き換わっていない。
     */
    private int rewriteOnce(int opt) {
        IndexedFile p = this.filei;

        p.write_cursor_open = true;

        if (this.access_mode == COB_ACCESS_SEQUENTIAL
                && !IndexedCursor.matchKeyHead(p.key, DBT_SET(this.keys[0].getField()))) {
            try {
                unlockPreviousRecord();
                p.connection.commit();
            } catch (SQLException e) {
                SnapshotConflictException.rethrowIfSnapshotConflict(e);
                return COB_STATUS_30_PERMANENT_ERROR;
            }
            return COB_STATUS_21_KEY_INVALID;
        }

        byte[] currentKey = DBT_SET(this.keys[0].getField());
        try {
            if (checkOtherProcessLockedRecord(currentKey)) {
                unlockPreviousRecord();
                p.connection.commit();
                return COB_STATUS_51_RECORD_LOCKED;
            }
            if (!lockRecord(currentKey)) {
                p.connection.rollback();
                unlockPreviousRecord();
                p.connection.commit();
                return COB_STATUS_30_PERMANENT_ERROR;
            }
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            try {
                unlockPreviousRecord();
                p.connection.commit();
            } catch (SQLException rollbackEx) {
                return COB_STATUS_30_PERMANENT_ERROR;
            }
            return COB_STATUS_30_PERMANENT_ERROR;
        }

        p.key = DBT_SET(this.keys[0].getField());
        int[] dupNumbers = new int[this.nkeys];
        java.util.Arrays.fill(dupNumbers, -1);

        int deleteRet = this.indexed_delete_internal(true, dupNumbers);

        if (deleteRet != COB_STATUS_00_SUCCESS) {
            p.write_cursor_open = false;
            try {
                p.connection.rollback();
                unlockPreviousRecord();
                p.connection.commit();
            } catch (SQLException rollbackEx) {
                return deleteRet;
            }
            return deleteRet;
        }

        int writeRet = this.indexed_write_internal(true, dupNumbers, opt);
        if (writeRet == COB_STATUS_00_SUCCESS) {
            try {
                unlockPreviousRecord();
                p.connection.commit();
            } catch (SQLException e) {
                SnapshotConflictException.rethrowIfSnapshotConflict(e);
                p.write_cursor_open = false;
                return COB_STATUS_30_PERMANENT_ERROR;
            }
        } else {
            p.write_cursor_open = false;
            try {
                p.connection.rollback();
                unlockPreviousRecord();
                p.connection.commit();
            } catch (SQLException rollbackEx) {
                return COB_STATUS_30_PERMANENT_ERROR;
            }
        }
        return writeRet;
    }

    private int indexed_delete_internal(boolean rewrite) {
        return this.indexed_delete_internal(rewrite, null);
    }

    /** Equivalent to indexed_delete_internal in libcob/fileio.c */
    private int indexed_delete_internal(boolean rewrite, int[] dupNumbers) {
        IndexedFile p = this.filei;
        boolean closeCursor;

        p.write_cursor_open = true;
        closeCursor = true;

        if (this.access_mode != COB_ACCESS_SEQUENTIAL) {
            p.key = DBT_SET(this.keys[0].getField());
        }

        if (this.access_mode != COB_ACCESS_SEQUENTIAL && !keyExistsInTable(p, 0, p.key)) {
            return returnWith(p, closeCursor, 0, COB_STATUS_23_KEY_NOT_EXISTS);
        }

        // delete data from the primary table
        String query = String.format("delete from %s where key = ?", getTableName(0));
        try (PreparedStatement statement = p.connection.prepareStatement(query)) {
            statement.setBytes(1, p.key);
            statement.execute();
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            return returnWith(p, closeCursor, 0, COB_STATUS_30_PERMANENT_ERROR);
        }

        // delete data from sub tables
        for (int i = 1; i < this.nkeys; ++i) {
            // save the duplicate number of the record to be deleted
            if (isDuplicateColumn(i)) {
                try (PreparedStatement statement =
                        p.connection.prepareStatement(
                                String.format(
                                        "select dupNo from %s where value = ?", getTableName(i)))) {
                    statement.setBytes(1, p.key);
                    try (ResultSet rs = statement.executeQuery()) {
                        if (rs.next()) {
                            int dupNo = rs.getInt(1);
                            if (dupNumbers != null) {
                                dupNumbers[i] = dupNo;
                            }
                        }
                    }
                } catch (SQLException e) {
                    SnapshotConflictException.rethrowIfSnapshotConflict(e);
                    return returnWith(p, closeCursor, 0, COB_STATUS_30_PERMANENT_ERROR);
                }
            }
            // delete the record
            try (PreparedStatement statement =
                    p.connection.prepareStatement(
                            String.format("delete from %s where value = ?", getTableName(i)))) {
                statement.setBytes(1, p.key);
                statement.execute();
            } catch (SQLException e) {
                SnapshotConflictException.rethrowIfSnapshotConflict(e);
                return returnWith(p, closeCursor, 0, COB_STATUS_30_PERMANENT_ERROR);
            }
        }

        this.updateWhileReading = true;

        return COB_STATUS_00_SUCCESS;
    }

    @Override
    public int delete_() {
        return retryOnSnapshotConflict(this::deleteOnce);
    }

    /**
     * {@link #delete_()}の本体。
     *
     * <p>主キーから対象レコードを特定し直すため、{@code SQLITE_BUSY_SNAPSHOT}が発生した場合はそのままやり直せる。
     */
    private int deleteOnce() {
        IndexedFile p = this.filei;
        byte[] currentKey = DBT_SET(this.keys[0].getField());
        try {
            if (checkOtherProcessLockedRecord(currentKey)) {
                unlockPreviousRecord();
                p.connection.commit();
                return COB_STATUS_51_RECORD_LOCKED;
            }
            if (!lockRecord(currentKey)) {
                p.connection.rollback();
                unlockPreviousRecord();
                p.connection.commit();
                return COB_STATUS_30_PERMANENT_ERROR;
            }
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            try {
                unlockPreviousRecord();
                p.connection.commit();
            } catch (SQLException rollbackEx) {
                return COB_STATUS_30_PERMANENT_ERROR;
            }
            return COB_STATUS_30_PERMANENT_ERROR;
        }

        int ret = this.indexed_delete_internal(false);

        if (ret == COB_STATUS_00_SUCCESS) {
            try {
                unlockPreviousRecord();
                p.connection.commit();
            } catch (SQLException e) {
                SnapshotConflictException.rethrowIfSnapshotConflict(e);
                return COB_STATUS_30_PERMANENT_ERROR;
            }
        } else {
            try {
                p.connection.rollback();
                unlockPreviousRecord();
                p.connection.commit();
            } catch (SQLException rollbackEx) {
                return ret;
            }
        }
        return ret;
    }

    @Override
    public void unlock_() {
        System.err.println("Unlocking INDEXED file is not implemented");
    }

    /**
     * 現在のJDBCトランザクションを明示的にコミットする。
     *
     * <p>{@link #setCommitOnModification(boolean)}{@code
     * (false)}と組み合わせて使うことを想定している。文ごとのコミットを抑制した状態で一連の更新を行った後、このメソッドがそれらをまとめて1回のコミットでフラッシュする。
     */
    public void commitJdbcTransaction() {
        if (retryOnSnapshotConflict(this::commitJdbcTransactionOnce) != COB_STATUS_00_SUCCESS) {
            System.err.println("Failed to commit a transaction");
        }
    }

    /**
     * 未コミットのトランザクションを破棄する。
     *
     * <p>{@link #setCommitOnModification(boolean)}{@code
     * (false)}で一括ロードを行っている途中でエラーが発生した場合に、それまでの更新を取り消すために使う。{@link
     * #close(int, jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField)}は保留中の更新をコミットしてしまうため、その前に呼び出す必要がある。
     */
    public void rollbackJdbcTransaction() {
        IndexedFile p = this.filei;
        try {
            p.connection.rollback();
        } catch (SQLException e) {
            System.err.println("Failed to roll back a transaction");
        }
    }

    private int commitJdbcTransactionOnce() {
        IndexedFile p = this.filei;
        try {
            p.connection.commit();
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            return COB_STATUS_30_PERMANENT_ERROR;
        }
        return COB_STATUS_00_SUCCESS;
    }

    /**
     * Delete all records in the file
     *
     * @return true if all records are deleted successfully, otherwise false
     */
    public boolean deleteAllRecords() {
        return retryOnSnapshotConflict(
                        () ->
                                this.deleteAllRecordsOnce()
                                        ? COB_STATUS_00_SUCCESS
                                        : COB_STATUS_30_PERMANENT_ERROR)
                == COB_STATUS_00_SUCCESS;
    }

    private boolean deleteAllRecordsOnce() {
        IndexedFile p = this.filei;
        try {
            for (int i = this.nkeys - 1; i >= 0; --i) {
                Statement statement = p.connection.createStatement();
                statement.execute("delete from " + getTableName(i));
                statement.close();
            }
            if (this.commitOnModification) {
                p.connection.commit();
            }
            return true;
        } catch (SQLException e) {
            SnapshotConflictException.rethrowIfSnapshotConflict(e);
            return false;
        }
    }
}

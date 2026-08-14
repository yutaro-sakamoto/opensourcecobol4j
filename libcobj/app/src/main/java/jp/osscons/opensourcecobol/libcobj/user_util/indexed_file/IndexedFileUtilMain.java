package jp.osscons.opensourcecobol.libcobj.user_util.indexed_file;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import jp.osscons.opensourcecobol.libcobj.common.CobolModule;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;
import jp.osscons.opensourcecobol.libcobj.file.CobolFile;
import jp.osscons.opensourcecobol.libcobj.file.CobolFileFactory;
import jp.osscons.opensourcecobol.libcobj.file.CobolFileKey;
import jp.osscons.opensourcecobol.libcobj.file.CobolIndexedFile;
import jp.osscons.opensourcecobol.libcobj.file.IndexedJournalConfig;
import jp.osscons.opensourcecobol.libcobj.file.SqliteNativeLibrary;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.sqlite.SQLiteConfig;

/**
 * Main class of the utility tool `cobj-idx` to handle a indexed file of opensource COBOL 4J. This
 * tool is used to show information of the indexed file, load data to the indexed file, and unload
 * data from the indexed file.
 */
class IndexedFileUtilMain {
    private static final String version = jp.osscons.opensourcecobol.libcobj.Const.version;

    /** 他プロセスのトランザクションの終了を待つ時間(ミリ秒)。 */
    private static final int BUSY_TIMEOUT_MILLIS = 5000;

    /**
     * cobj-idxコマンドのエントリポイント。<br>
     * コマンドライン引数をパースし、指定されたサブコマンド({@code info} / {@code create} / {@code load} /
     * {@code unload} / {@code migrate} / {@code unlock})を実行する。
     *
     * @param args コマンドラインから入力された引数の配列
     */
    public static void main(String[] args) {
        SqliteNativeLibrary.prepare();

        // Build a command line parser
        Options options = new Options();
        options.addOption("h", "help", false, "Print this message.");
        options.addOption("v", "version", false, "Print the version");
        options.addOption("n", "new", false, "Delete all data before loading");
        options.addOption("f", "format", true, "Specify the format of the input and output data");
        options.addOption("s", "size", true, "Specify the record size of the indexed file.");
        options.addOption("k", "key", true, "Specify the key information of the indexed file.");
        CommandLineParser parser = new DefaultParser();
        CommandLine cmd;

        // Parse command line arguments
        try {
            cmd = parser.parse(options, args);
        } catch (ParseException e) {
            printHelpMessage();
            System.exit(1);
            return;
        }

        // Process -h, --help option
        if (cmd.hasOption("h")) {
            printHelpMessage();
            System.exit(0);
            return;
        }

        // Process -v, --version option
        if (cmd.hasOption("v")) {
            System.out.println(version);
            System.exit(0);
            return;
        }

        // Process -f, --format option
        UserDataFormat userDataFormat = UserDataFormat.SEQUENTIAL;
        String userDataFormatString = cmd.getOptionValue("f");
        if (userDataFormatString != null) {
            userDataFormatString = userDataFormatString.toLowerCase();
            if ("txt".equals(userDataFormatString)) {
                userDataFormat = UserDataFormat.LINE_SEQUENTIAL;
            } else if ("bin".equals(userDataFormatString)) {
                userDataFormat = UserDataFormat.SEQUENTIAL;
            } else {
                System.err.println(
                        String.format(
                                "error: '%s' is invalid value of -f and --format option.",
                                userDataFormatString));
                System.err.println(
                        "       possible values of -f and --format option is 'txt' and 'bin'.");
                System.exit(1);
            }
        }

        // If no sub command is specified, print help message and exit.
        String[] unrecognizedArgs = cmd.getArgs();
        if (unrecognizedArgs.length == 0) {
            printHelpMessage();
            System.exit(0);
            return;
        }

        // Dispatch sub commands
        String subCommand = unrecognizedArgs[0];
        if ("info".equals(subCommand)) {
            if (unrecognizedArgs.length != 2) {
                if (unrecognizedArgs.length < 2) {
                    System.err.println("error: no indexed file is specified.");
                } else {
                    System.err.println("error: too many indexed files are specified.");
                }
                System.exit(1);
            }
            String indexedFilePath = args[1];
            int exitCode = processInfoCommand(indexedFilePath);
            System.exit(exitCode);
        } else if ("create".equals(subCommand)) {
            try {
                validateCreateCommandArgs(unrecognizedArgs, cmd);
                String indexedFilePath = unrecognizedArgs[1];
                int recordSize = parseRecordSize(cmd);
                List<CobolFileKeyInfo> keyInfoList = parseKeyOptions(cmd);
                validateKeyInfoList(keyInfoList, recordSize);
                processCreateIndexedFile(indexedFilePath, recordSize, keyInfoList, cmd);
                System.exit(0);
            } catch (Exception e) {
                System.err.println("error: " + e.getMessage());
                System.exit(1);
            }
        } else if ("migrate".equals(subCommand)) {
            if (unrecognizedArgs.length < 2) {
                System.err.println("error: no indexed file is specified.");
                System.exit(1);
            }
            boolean hasError = false;
            for (int i = 1; i < unrecognizedArgs.length; i++) {
                String indexedFilePath = unrecognizedArgs[i];
                try {
                    migrateIndexedFile(indexedFilePath);
                } catch (Exception e) {
                    System.err.println("error: " + e.getMessage());
                    hasError = true;
                }
            }
            System.exit(hasError ? 1 : 0);
        } else if ("unlock".equals(subCommand)) {
            if (unrecognizedArgs.length < 2) {
                System.err.println("error: no indexed file is specified.");
                System.exit(1);
            }
            boolean hasError = false;
            for (int i = 1; i < unrecognizedArgs.length; i++) {
                String indexedFilePath = unrecognizedArgs[i];
                try {
                    unlockIndexedFile(indexedFilePath);
                } catch (Exception e) {
                    System.err.println("error: " + e.getMessage());
                    hasError = true;
                }
            }
            System.exit(hasError ? 1 : 0);
        } else if ("load".equals(subCommand)) {
            if (unrecognizedArgs.length < 2 || unrecognizedArgs.length > 3) {
                if (unrecognizedArgs.length < 2) {
                    System.err.println("error: no indexed file is specified.");
                } else {
                    System.err.println("error: too many indexed files are specified.");
                }
                System.exit(1);
            }
            String indexedFilePath = unrecognizedArgs[1];
            Optional<String> filePath;
            if (unrecognizedArgs.length == 3) {
                filePath = Optional.of(unrecognizedArgs[2]);
            } else {
                filePath = Optional.empty();
            }
            boolean deleteBeforeLoading = cmd.hasOption("n");
            int exitCode =
                    processLoadCommand(
                            indexedFilePath, deleteBeforeLoading, userDataFormat, filePath);
            System.exit(exitCode);

        } else if ("unload".equals(subCommand)) {
            if (unrecognizedArgs.length < 2 || unrecognizedArgs.length > 3) {
                if (unrecognizedArgs.length < 2) {
                    System.err.println("error: no indexed file is specified.");
                } else {
                    System.err.println("error: too many indexed files are specified.");
                }
                System.exit(1);
            }
            String indexedFilePath = unrecognizedArgs[1];
            Optional<String> filePath;
            if (unrecognizedArgs.length == 3) {
                filePath = Optional.of(unrecognizedArgs[2]);
            } else {
                filePath = Optional.empty();
            }
            int exitCode = processUnloadCommand(indexedFilePath, userDataFormat, filePath);
            System.exit(exitCode);
        } else {
            printHelpMessage();
            System.exit(1);
        }
    }

    /** cobj-idxコマンドのヘルプメッセージを標準出力へ出力する。 */
    private static void printHelpMessage() {
        System.out.println(
                "cobj-idx - A utility tool to handle an indexed file of opensource COBOL 4J");
        System.out.println();
        System.out.println("Usage:");
        System.out.println("cobj-idx <sub command> [options] <indexed file>");
        System.out.println();
        System.err.println("Sub commands:");
        System.out.println();
        System.out.println("cobj-idx info <indexed-file>");
        System.out.println("    Show information of the indexed file.");
        System.out.println();
        System.out.println(
                "cobj-idx create <indexed file> --size=<record size> --key=<key information>");
        System.out.println("    Create a new indexed file.");
        System.out.println("    The record size and key information are specified by the options.");
        System.out.println("    By default, this command does not overwrite the indexed file.");
        System.out.println("    To overwrite the indexed file, use the --new option.");
        System.out.println("    Example) cobj-idx create test.idx --size=100 --key=2,2:5,4:d15,5");
        System.out.println("             File name: test.idx");
        System.out.println("             Record size: 100");
        System.out.println("             Primary key: 2-3");
        System.out.println("             Alternate key (No Duplicates):5-8");
        System.out.println("             Alternate key (Duplicates): 15-19");
        System.out.println();
        System.out.println("cobj-idx load <indexed file>");
        System.out.println("    Load the data from stdin into the indexed file.");
        System.out.println("    The default format of the input data is SEQUENTIAL of COBOL.");
        System.out.println();
        System.out.println("cobj-idx load <indexed file> <input file>");
        System.out.println("    Load data from the input file into the indexed file.");
        System.out.println("    The default format of the input data is SEQUENTIAL of COBOL.");
        System.out.println();
        System.out.println("cobj-idx unload <indexed file>");
        System.out.println("    Write the records stored in the indexed file into stdout.");
        System.out.println("    The default format of the output data is SEQUENTIAL of COBOL.");
        System.out.println();
        System.out.println("cobj-idx unload <indexed file> <output file>");
        System.out.println(
                "    Write the records stored in the indexed file into the output file.");
        System.out.println("    The default format of the output data is SEQUENTIAL of COBOL.");
        System.out.println();
        System.out.println("cobj-idx migrate <indexed file> [<indexed file>...]");
        System.out.println(
                "    Migrate the indexed file whose version is older than 1.1.12 to the latest"
                        + " version.");
        System.out.println();
        System.out.println("cobj-idx unlock <indexed file> [<indexed file>...]");
        System.out.println("    Unlock all locks of the indexed file.");
        System.out.println();
        System.out.println("Options:");
        System.out.println();
        System.out.println("-f <format>, --format=<format>");
        System.out.println("    Specify the format of the input and output data.");
        System.out.println("    Possible values are 'txt' and the default value 'bin'");
        System.out.println(
                "    'bin' and 'txt' means SEQUENTIAL and LINE SEQUENTIAL respectively.");
        System.out.println(
                "    When doing a `load`, this option specifies the format of input data which will"
                        + " be inserted to an indexed file.");
        System.out.println(
                "    When doing an `unload`, this option specifies the format of output data which"
                        + " will be read from an indexed file.");
        System.out.println();
        System.out.println("-h --help");
        System.out.println("    Print this message.");
        System.out.println();
        System.out.println("-n, --new");
        System.out.println(
                "    Delete all data before inserting new data. This option is only valid when the"
                        + " sub command is 'load'.");
        System.out.println();
        System.out.println("-v, --version");
        System.out.println("    Print the version of cobj-idx.");
    }

    /**
     * {@code -k}オプション({@code --key})の値をパースし、{@link CobolFileKeyInfo}のリストを返す。<br>
     * キーは{@code offset,size}の組をコロン区切りで並べた文字列で指定され、
     * 先頭に{@code d}を付けた{@code dOFFSET,SIZE}は重複キーを意味する。例えば
     * {@code 2,2:5,4:d15,5}は「プライマリキーがオフセット2長さ2、重複なしのオルタネートキーが
     * オフセット5長さ4、重複ありのオルタネートキーがオフセット15長さ5」を表す。
     *
     * @param cmd パース済みのコマンドライン
     * @return キー情報のリスト
     * @throws Exception キーの指定が不正な場合や、プライマリキーが重複キーとして指定されている場合など
     */
    private static List<CobolFileKeyInfo> parseKeyOptions(CommandLine cmd) throws Exception {
        if (cmd.hasOption("k")) {
            ArrayList<CobolFileKeyInfo> keyInfoList = new ArrayList<>();

            String keyOption = cmd.getOptionValue("k");
            Pattern pattern =
                    Pattern.compile("[1-9][0-9]*,[1-9][0-9]*(:d?[1-9][0-9]*,[1-9][0-9]*)*");
            Matcher matcher = pattern.matcher(keyOption);

            if (!matcher.matches()) {
                Pattern patternPrimaryKeyDup =
                        Pattern.compile("d[1-9][0-9]*,[1-9][0-9]*(:d?[1-9][0-9]*,[1-9][0-9]*)*");
                Matcher matcherPrimaryKeyDup = patternPrimaryKeyDup.matcher(keyOption);
                if (matcherPrimaryKeyDup.matches()) {
                    throw new IllegalArgumentException(
                            "the first key (primary key) must not be a duplicate key.");
                }

                Pattern patternContains0 = Pattern.compile("[0-9]+,[0-9]+(:d?[0-9]+,[0-9]+)*");
                matcher = patternContains0.matcher(keyOption);
                if (matcher.matches()) {
                    String[] keys = keyOption.split(":");
                    for (String key : keys) {
                        String[] keyInfo = key.split(",");
                        String offsetString = keyInfo[0];
                        if (offsetString.startsWith("d")) {
                            offsetString = offsetString.substring(1);
                        }

                        if ("0".equals(offsetString)) {
                            throw new IllegalArgumentException(
                                    String.format(
                                            "key offsets must be greater than 0: %s", keyOption));
                        } else if (offsetString.startsWith("0")) {
                            throw new IllegalArgumentException(
                                    String.format(
                                            "key offsets must not start with 0: %s", keyOption));
                        }
                        String sizeString = keyInfo[1];
                        if ("0".equals(sizeString)) {
                            throw new IllegalArgumentException(
                                    String.format(
                                            "key sizes must be greater than 0: %s", keyOption));
                        } else if (sizeString.startsWith("0")) {
                            throw new IllegalArgumentException(
                                    String.format(
                                            "key sizes must not start with 0: %s", keyOption));
                        }
                    }
                }
                throw new IllegalArgumentException(
                        String.format("invalid key information: %s", keyOption));
            }
            String[] keys = keyOption.split(":");
            for (int i = 0; i < keys.length; i++) {
                String key = keys[i];
                boolean isFirstKey = (i == 0);
                String[] keyInfo = key.split(",");
                if (keyInfo.length != 2) {
                    throw new IllegalArgumentException(
                            String.format("invalid key information: %s", keyOption));
                }
                String offsetString = keyInfo[0];
                boolean duplicate = false;
                if (keyInfo[0].startsWith("d")) {
                    if (isFirstKey) {
                        throw new IllegalArgumentException(
                                String.format(
                                        "the first key (primary key) must not be a duplicate"
                                                + " key."));
                    }
                    offsetString = keyInfo[0].substring(1);
                    duplicate = true;
                }
                int offset = Integer.parseInt(offsetString);
                int size = Integer.parseInt(keyInfo[1]);

                CobolFileKeyInfo keyInfoObj = new CobolFileKeyInfo(offset, size, duplicate);
                keyInfoList.add(keyInfoObj);
            }
            return keyInfoList;
        } else {
            throw new IllegalArgumentException("no key information is specified.");
        }
    }

    /**
     * {@code info}サブコマンドを実行し、インデックスファイルのメタ情報(レコードサイズ、レコード件数、
     * プライマリキー・オルタネートキーの位置など)を標準出力へ出力する。
     *
     * @param indexedFilePath 情報を表示する対象のインデックスファイルのパス
     * @return 成功した場合は{@code 0}、失敗した場合は非ゼロの終了コード
     */
    private static int processInfoCommand(String indexedFilePath) {
        File indexedFile = new File(indexedFilePath);
        if (!indexedFile.exists()) {
            return ErrorLib.errorFileDoesNotExist(indexedFilePath);
        }
        if (!indexedFile.isFile()) {
            return ErrorLib.errorInvalidIndexedFile(indexedFilePath);
        }

        StringBuilder sb = new StringBuilder();

        try (Connection conn = openMetadataConnection(indexedFilePath);
                Statement stmt = conn.createStatement(); ) {
            // Retrieve the record size
            ResultSet rs =
                    stmt.executeQuery(
                            "select value from metadata_string_int where key = 'record_size'");
            if (!rs.next()) {
                return ErrorLib.errorInvalidIndexedFile(indexedFilePath);
            }
            int recordSize = rs.getInt("value");
            sb.append("Size of a record: " + recordSize + "\n");

            // Retrieve the number of records
            rs = stmt.executeQuery("select count(*) from table0");
            if (!rs.next()) {
                return ErrorLib.errorInvalidIndexedFile(indexedFilePath);
            }
            sb.append("Number of records: " + rs.getInt(1) + "\n");

            // Retrieve the number of keys
            rs =
                    stmt.executeQuery(
                            "select idx, offset, size, duplicate from metadata_key order by idx");
            while (rs.next()) {
                int idx = rs.getInt("idx");
                int offset = rs.getInt("offset") + 1;
                int size = rs.getInt("size");
                boolean duplicate = rs.getBoolean("duplicate");
                if (idx == 0) {
                    sb.append("Primary key position: ");
                } else {
                    sb.append("Alternate key position ");
                    if (duplicate) {
                        sb.append("(Duplicates): ");
                    } else {
                        sb.append("(No duplicate): ");
                    }
                }
                sb.append(offset + "-" + (offset + size - 1) + "\n");
            }

            System.out.print(sb.toString());
            return 0;
        } catch (SQLException e) {
            return ErrorLib.errorInvalidIndexedFile(indexedFilePath);
        }
    }

    /**
     * {@code migrate}サブコマンドの実体。<br>
     * opensource COBOL 4J 1.1.12より前のバージョンで作成されたインデックスファイルを最新形式へ移行する。
     * 具体的には{@code file_lock}テーブルを作成し、{@code table0}にファイルロック用の列
     * ({@code locked_by}、{@code process_id}、{@code locked_at})が存在しない場合にそれらを追加する。
     *
     * @param indexedFilePath 移行対象のインデックスファイルのパス
     * @throws Exception インデックスファイルへの接続に失敗した場合
     */
    private static void migrateIndexedFile(String indexedFilePath) throws Exception {
        try (Connection conn = openReadWriteConnection(indexedFilePath);
                Statement st = conn.createStatement()) {
            String createTableSql =
                    "CREATE TABLE IF NOT EXISTS file_lock (locked_by text primary key,process_id"
                            + " text,locked_at timestamp,open_mode text CONSTRAINT check_open_mode"
                            + " CHECK (open_mode IN ('INPUT', 'OUTPUT', 'I-O', 'EXTEND')))";
            st.executeUpdate(createTableSql);

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
                        break;
                    }
                }
            }
            if (!lockedByColumnExists) {
                st.executeUpdate("ALTER TABLE table0 ADD COLUMN locked_by text");
            }
            if (!processIdColumnExists) {
                st.executeUpdate("ALTER TABLE table0 ADD COLUMN process_id text");
            }
            if (!lockedAtColumnExists) {
                st.executeUpdate("ALTER TABLE table0 ADD COLUMN locked_at timestamp");
            }
        } catch (SQLException e) {
            throw new Exception(
                    String.format("failed to connect to the indexed file: %s", indexedFilePath), e);
        }
    }

    /**
     * {@code unlock}サブコマンドの実体。<br>
     * 指定されたインデックスファイルに対してかけられているすべてのロック情報を削除する。
     * {@code file_lock}テーブルのすべての行を削除し、{@code table0}のロック関連列をすべて{@code NULL}に戻す。
     *
     * @param indexedFilePath ロック解除対象のインデックスファイルのパス
     * @throws Exception インデックスファイルへの接続に失敗した場合
     */
    private static void unlockIndexedFile(String indexedFilePath) throws Exception {
        try (Connection conn = openReadWriteConnection(indexedFilePath);
                Statement st = conn.createStatement()) {
            st.executeUpdate("DELETE FROM file_lock");
            st.executeUpdate(
                    "UPDATE table0 SET locked_by = NULL, process_id = NULL, locked_at = NULL");
        } catch (SQLException e) {
            throw new Exception(
                    String.format("failed to connect to the indexed file: %s", indexedFilePath), e);
        }
    }

    /**
     * {@code load}サブコマンドを実行し、指定された入力ソース(標準入力またはファイル)から読み込んだ
     * データをインデックスファイルに書き込む。
     *
     * @param indexedFilePath ロード先のインデックスファイルのパス
     * @param deleteBeforeLoading ロード前に既存のレコードをすべて削除する場合は{@code true}
     * @param userDataFormat 入力データの形式
     * @param filePath 入力データのファイルパス。標準入力から読み込む場合は空の{@link Optional}
     * @return 成功した場合は{@code 0}、失敗した場合は非ゼロの終了コード
     */
    private static int processLoadCommand(
            String indexedFilePath,
            boolean deleteBeforeLoading,
            UserDataFormat userDataFormat,
            Optional<String> filePath) {
        File indexedFile = new File(indexedFilePath);
        if (!indexedFile.exists()) {
            return ErrorLib.errorFileDoesNotExist(indexedFilePath);
        }
        if (!indexedFile.isFile()) {
            return ErrorLib.errorInvalidIndexedFile(indexedFilePath);
        }
        Optional<CobolFile> cobolFileRet = createCobolFileFromIndexedFilePath(indexedFilePath);
        if (!cobolFileRet.isPresent()) {
            return ErrorLib.errorInvalidIndexedFile(indexedFilePath);
        }
        CobolIndexedFile cobolIndexedFile = (CobolIndexedFile) cobolFileRet.get();

        // Set the module
        CobolModule module =
                new CobolModule(null, null, null, null, 0, '.', '$', ',', 1, 1, 1, 0, null);
        CobolModule.push(module);

        // Open the indexed file
        CobolRuntimeException.code = 0;
        cobolIndexedFile.setCommitOnModification(false);
        cobolIndexedFile.open(CobolFile.COB_OPEN_EXTEND, 0, null);
        if (CobolRuntimeException.code != 0) {
            CobolModule.pop();
            return ErrorLib.errorIO();
        }

        // The indexed file has to be closed on every path from here on. Leaving the connection
        // open would keep the file_lock row of this process in the file, and in WAL mode it
        // would also leave the -wal/-shm files behind.
        try {
            return loadRecords(cobolIndexedFile, deleteBeforeLoading, userDataFormat, filePath);
        } finally {
            cobolIndexedFile.close(0, null);
            CobolModule.pop();
        }
    }

    /**
     * 入力ソースから読み込んだレコードを、既に{@code OPEN}済みのインデックスファイルへ書き込む。
     *
     * <p>ここでのエラーはすべて呼び出し元の{@code finally}で確実にクローズされる。文ごとのコミットは抑制されているため、
     * エラー時は{@link CobolIndexedFile#rollbackJdbcTransaction()}で保留中の更新を破棄する。そうしないと後続の{@code
     * close}がそれらをコミットしてしまい、中途半端にロードされた状態になる。
     */
    private static int loadRecords(
            CobolIndexedFile cobolIndexedFile,
            boolean deleteBeforeLoading,
            UserDataFormat userDataFormat,
            Optional<String> filePath) {
        if (deleteBeforeLoading) {
            cobolIndexedFile.deleteAllRecords();
        }

        RecordReader reader =
                RecordReader.getInstance(userDataFormat, cobolIndexedFile.record_max, filePath);
        reader.open();
        LoadResult loadResult = LoadResult.LoadResultSuccess;
        // Read records from stdin or a file and write them to the indexed file
        while (true) {
            loadResult = reader.read(cobolIndexedFile.record.getDataStorage());
            if (loadResult != LoadResult.LoadResultSuccess) {
                break;
            }

            // Write the record to the indexed file
            CobolRuntimeException.code = 0;
            try {
                cobolIndexedFile.write(cobolIndexedFile.record, 0, null);
            } catch (CobolStopRunException e) {
                loadResult = LoadResult.LoadResultOther;
                break;
            }
            if (CobolRuntimeException.code != 0) {
                loadResult = LoadResult.LoadResultOther;
                break;
            }
        }

        reader.close();

        if (loadResult == LoadResult.LoadResultDataSizeMismatch) {
            cobolIndexedFile.rollbackJdbcTransaction();
            return ErrorLib.errorDataSizeMismatch(cobolIndexedFile.record.getSize());
        } else if (loadResult == LoadResult.LoadResultOther) {
            cobolIndexedFile.rollbackJdbcTransaction();
            return ErrorLib.errorDuplicateKeys();
        } else {
            cobolIndexedFile.commitJdbcTransaction();
            return 0;
        }
    }

    /**
     * {@code unload}サブコマンドを実行し、インデックスファイル内のすべてのレコードを指定された出力先
     * (標準出力またはファイル)に書き出す。
     *
     * @param indexedFilePath アンロード元のインデックスファイルのパス
     * @param userDataFormat 出力データの形式。{@link UserDataFormat#LINE_SEQUENTIAL}の場合、各レコードは
     *     改行文字で区切られる。{@link UserDataFormat#SEQUENTIAL}の場合、各レコードは区切り文字なしで
     *     連結される
     * @param filePath 出力先のファイルパス。標準出力に書き出す場合は空の{@link Optional}
     * @return 成功した場合は{@code 0}、失敗した場合は非ゼロの終了コード
     */
    private static int processUnloadCommand(
            String indexedFilePath, UserDataFormat userDataFormat, Optional<String> filePath) {
        File indexedFile = new File(indexedFilePath);
        if (!indexedFile.exists()) {
            return ErrorLib.errorFileDoesNotExist(indexedFilePath);
        }
        if (!indexedFile.isFile()) {
            return ErrorLib.errorInvalidIndexedFile(indexedFilePath);
        }
        Optional<CobolFile> cobolFileRet = createCobolFileFromIndexedFilePath(indexedFilePath);
        if (!cobolFileRet.isPresent()) {
            return ErrorLib.errorInvalidIndexedFile(indexedFilePath);
        }
        CobolFile cobolFile = cobolFileRet.get();

        // Set the module
        CobolModule module =
                new CobolModule(null, null, null, null, 0, '.', '$', ',', 1, 1, 1, 0, null);
        CobolModule.push(module);

        // Open the indexed file
        CobolRuntimeException.code = 0;
        cobolFile.open(CobolFile.COB_OPEN_INPUT, 0, null);
        if (CobolRuntimeException.code != 0) {
            CobolModule.pop();
            return ErrorLib.errorIO();
        }

        // The indexed file has to be closed on every path from here on, otherwise the file_lock
        // row of this process stays in the file and, in WAL mode, the -wal/-shm files are left
        // behind as well.
        try {
            return unloadRecords(cobolFile, userDataFormat, filePath);
        } finally {
            cobolFile.close(CobolFile.COB_CLOSE_NORMAL, null);
            CobolModule.pop();
        }
    }

    /** 既に{@code OPEN}済みのインデックスファイルから全レコードを読み出し、出力先へ書き出す。 */
    private static int unloadRecords(
            CobolFile cobolFile, UserDataFormat userDataFormat, Optional<String> filePath) {
        // Read records from the indexed file and write them to stdout or a file
        boolean isIndexedFileEmpty = true;
        try (OutputStream stream = getOutputStream(filePath)) {
            while (true) {
                CobolRuntimeException.code = 0;
                cobolFile.read(0, null, 1);
                if (CobolRuntimeException.code == 0) {
                    isIndexedFileEmpty = false;
                    CobolDataStorage storage = cobolFile.record.getDataStorage();
                    stream.write(
                            storage.getRefOfData(), storage.getIndex(), cobolFile.record.getSize());
                    if (userDataFormat == UserDataFormat.LINE_SEQUENTIAL) {
                        stream.write('\n');
                    }
                } else if (CobolRuntimeException.code == 0x0501) {
                    break;
                } else {
                    return ErrorLib.errorIO();
                }
            }
            if (userDataFormat == UserDataFormat.SEQUENTIAL && !isIndexedFileEmpty) {
                stream.write('\n');
            }
        } catch (IOException e) {
            return ErrorLib.errorIO();
        }

        return 0;
    }

    private static OutputStream getOutputStream(Optional<String> filePath)
            throws FileNotFoundException {
        if (filePath.isPresent()) {
            return new FileOutputStream(filePath.get());
        } else {
            return System.out;
        }
    }

    /**
     * インデックスファイルを更新するための接続を開き、設定されたジャーナルモードを適用する。
     *
     * <p>{@code migrate}や{@code
     * unlock}が古い形式のファイルを開いた場合、ここで{@link IndexedJournalConfig}の設定に従ってWALモードへ(あるいはWALから)変換される。
     *
     * @param indexedFilePath インデックスファイルのパス
     * @return 読み書き可能な接続
     * @throws SQLException 接続またはジャーナルモードの適用に失敗した場合
     */
    private static Connection openReadWriteConnection(String indexedFilePath) throws SQLException {
        SQLiteConfig config = new SQLiteConfig();
        config.setReadOnly(false);
        config.setBusyTimeout(BUSY_TIMEOUT_MILLIS);
        Connection conn =
                DriverManager.getConnection(
                        "jdbc:sqlite:" + indexedFilePath, config.toProperties());
        // The connection is still in auto-commit mode here, which PRAGMA journal_mode requires.
        IndexedJournalConfig.applyTo(conn);
        return conn;
    }

    /**
     * インデックスファイルのメタデータを読み出すための接続を開く。この接続では{@code SELECT}しか実行しない。
     *
     * <p>書き込み可能な場合は読み書きモードで開く。WALモードのデータベースを読み取り専用({@code
     * SQLITE_OPEN_READONLY})で開くと、SQLiteが作成したWALインデックス({@code -shm})と{@code
     * -wal}をクローズ時に削除できず、{@code cobj-idx info}を実行しただけでファイルが残ってしまうためである。
     *
     * <p>書き込めない場合は読み取り専用で開く。それも失敗する場合(WALモードのデータベースは{@code
     * -shm}を作成するためディレクトリへの書き込み権限を必要とし、権限がないと{@code
     * SQLITE_READONLY_DIRECTORY}になる)は、{@code -wal}が存在せず復元すべき内容がないことを確認したうえで{@code
     * immutable=1}で開き直す。
     *
     * @param indexedFilePath インデックスファイルのパス
     * @return メタデータ読み出し用の接続
     * @throws SQLException いずれの方法でも接続できなかった場合
     */
    private static Connection openMetadataConnection(String indexedFilePath) throws SQLException {
        try {
            return openAndProbe("jdbc:sqlite:" + indexedFilePath, false);
        } catch (SQLException readWriteFailure) {
            try {
                return openAndProbe("jdbc:sqlite:" + indexedFilePath, true);
            } catch (SQLException readOnlyFailure) {
                if (new File(indexedFilePath + "-wal").exists()) {
                    // A write-ahead log is present and has to be taken into account, so falling
                    // back to an immutable connection would silently read stale data.
                    throw readOnlyFailure;
                }
                return openAndProbe(
                        "jdbc:sqlite:file:" + toFileUriPath(indexedFilePath) + "?immutable=1",
                        true);
            }
        }
    }

    /**
     * 接続を開き、実際に文を1つ実行して使用可能なことを確かめる。
     *
     * <p>sqlite-jdbcの{@code
     * getConnection}は遅延して開くため、たとえば書き込み権限のないディレクトリにあるWALモードのデータベースは、接続時ではなく最初の文の実行時に{@code
     * SQLITE_READONLY_DIRECTORY}で失敗する。ここで実際に文を実行しておかないと、呼び出し側のフォールバックが機能しない。
     *
     * @param url JDBC URL
     * @param readOnly 読み取り専用で開く場合は{@code true}
     * @return 使用可能な接続
     * @throws SQLException 接続できないか、文を実行できない場合
     */
    private static Connection openAndProbe(String url, boolean readOnly) throws SQLException {
        SQLiteConfig config = new SQLiteConfig();
        config.setReadOnly(readOnly);
        Connection conn = DriverManager.getConnection(url, config.toProperties());
        try (Statement statement = conn.createStatement()) {
            statement.execute("select 1");
        } catch (SQLException e) {
            try {
                conn.close();
            } catch (SQLException ignored) {
                // 使えない接続を閉じられなくても、報告すべきは元の失敗のほうである。
            }
            throw e;
        }
        return conn;
    }

    /**
     * ファイルパスを{@code file:} URIのパス部分に変換する。
     *
     * <p>{@code immutable=1}を指定するにはURI形式で開く必要があるが、{@code ?}や{@code
     * #}を含むパスはそのまま連結するとクエリ部分と解釈されてしまう。Windowsの絶対パスも{@code
     * ///}で始める必要がある。
     *
     * @param path ファイルパス
     * @return {@code file:}に続けて使えるパス文字列
     */
    private static String toFileUriPath(String path) {
        String normalized = path.replace(File.separatorChar, '/');
        // Windowsの絶対パス(C:/...)はfile:///C:/...の形にする必要がある。
        if (normalized.length() >= 2 && normalized.charAt(1) == ':') {
            normalized = "//" + normalized;
        }
        return normalized.replace("?", "%3f").replace("#", "%23");
    }

    private static Optional<CobolFile> createCobolFileFromIndexedFilePath(String indexedFilePath) {
        try (Connection conn = openMetadataConnection(indexedFilePath);
                Statement stmt = conn.createStatement(); ) {

            ResultSet rs =
                    stmt.executeQuery(
                            "select value from metadata_string_int where key = 'record_size'");
            if (!rs.next()) {
                return Optional.empty();
            }

            int recordSize = rs.getInt("value");

            // Create a record field and data storage
            AbstractCobolField recordField = createIndexedRecordField(recordSize);
            CobolDataStorage recordDataStorage = recordField.getDataStorage();

            // Retrive key information
            List<CobolFileKey> keyList = new ArrayList<>();
            rs =
                    stmt.executeQuery(
                            "select idx, offset, size, duplicate from metadata_key order by"
                                    + " idx");
            while (rs.next()) {
                int offset = rs.getInt("offset");
                int size = rs.getInt("size");
                boolean duplicate = rs.getBoolean("duplicate");
                addCobolFileKeyToList(keyList, recordDataStorage, offset, size, duplicate);
            }

            // Return a CobolFile instance
            return Optional.of(
                    createCobolFileInstance(indexedFilePath, recordSize, recordField, keyList));
        } catch (SQLException e) {
            return Optional.empty();
        }
    }

    /**
     * 指定されたパス・レコードサイズ・キー情報から新規に{@link CobolFile}のインスタンスを生成する。<br>
     * {@code create}サブコマンドの実装で、インデックスファイルをこれから作成する際に使用される。
     *
     * @param indexedFilePath 作成するインデックスファイルのパス
     * @param recordSize 1レコードのバイト数
     * @param keyInfoList インデックスファイルに設定するキー情報のリスト
     * @return 生成された{@link CobolFile}インスタンスを含む{@link Optional}。このメソッドは常に値を含む
     *     {@link Optional}を返す
     */
    private static Optional<CobolFile> createCobolFile(
            String indexedFilePath, Integer recordSize, List<CobolFileKeyInfo> keyInfoList) {
        // Create a record field
        AbstractCobolField recordField = createIndexedRecordField(recordSize);
        CobolDataStorage recordDataStorage = recordField.getDataStorage();

        // Retrive key information
        List<CobolFileKey> keyList = new ArrayList<>();
        for (CobolFileKeyInfo key : keyInfoList) {
            addCobolFileKeyToList(
                    keyList, recordDataStorage, key.offset - 1, key.size, key.duplicate);
        }

        // Return a CobolFile instance
        return Optional.of(
                createCobolFileInstance(indexedFilePath, recordSize, recordField, keyList));
    }

    private static AbstractCobolField createIndexedRecordField(int recordSize) {
        byte[] recordByteArray = new byte[recordSize];
        CobolDataStorage recordDataStorage = new CobolDataStorage(recordByteArray);
        return CobolFieldFactory.makeCobolField(
                recordSize, recordDataStorage, new CobolFieldAttribute(1, 0, 0, 0, null));
    }

    private static void addCobolFileKeyToList(
            List<CobolFileKey> keyList,
            CobolDataStorage recordStorage,
            int offset,
            int size,
            boolean duplicate) {

        CobolFileKey cobolFileKey = new CobolFileKey();
        cobolFileKey.setOffset(offset);
        cobolFileKey.setFlag(duplicate ? 1 : 0);
        AbstractCobolField keyField =
                CobolFieldFactory.makeCobolField(
                        size,
                        recordStorage.getSubDataStorage(offset),
                        new CobolFieldAttribute(33, 0, 0, 0, null));
        cobolFileKey.setField(keyField);

        keyList.add(cobolFileKey);
    }

    private static CobolFile createCobolFileInstance(
            String indexedFilePath,
            int recordSize,
            AbstractCobolField recordField,
            List<CobolFileKey> keyList) {
        byte[] fileStatus = new byte[4];
        byte[] indxedFilePathBytes = indexedFilePath.getBytes(AbstractCobolField.charSetSJIS);
        AbstractCobolField assignField =
                CobolFieldFactory.makeCobolField(
                        indxedFilePathBytes.length,
                        new CobolDataStorage(indxedFilePathBytes),
                        new CobolFieldAttribute(33, 0, 0, 0, null));
        CobolFileKey[] keyArray = new CobolFileKey[keyList.size()];
        keyList.toArray(keyArray);
        return CobolFileFactory.makeCobolFileInstance(
                "f",
                fileStatus,
                assignField,
                recordField,
                null,
                recordSize,
                recordSize,
                keyArray.length,
                keyArray,
                (char) 3,
                (char) 1,
                (char) 0,
                (char) 0,
                false,
                (char) 0,
                (char) 0,
                false,
                false,
                false,
                (char) 0,
                false,
                (char) 2,
                false,
                false,
                (char) 0);
    }

    private static void validateCreateCommandArgs(String[] unrecognizedArgs, CommandLine cmd) {
        if (unrecognizedArgs.length < 2) {
            throw new IllegalArgumentException("no indexed file is specified.");
        }
        if (!cmd.hasOption("s")) {
            throw new IllegalArgumentException("no record size is specified.");
        }
    }

    private static int parseRecordSize(CommandLine cmd) {
        String recordSizeString = cmd.getOptionValue("s");
        int recordSize;
        try {
            recordSize = Integer.parseInt(recordSizeString);
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException(
                    String.format("invalid record size: %s", recordSizeString), e);
        }
        if (recordSize <= 0) {
            throw new IllegalArgumentException(
                    String.format("invalid record size: %d", recordSize));
        }
        return recordSize;
    }

    private static void validateKeyInfoList(List<CobolFileKeyInfo> keyInfoList, int recordSize) {
        if (keyInfoList.isEmpty()) {
            throw new IllegalArgumentException("no key information is specified.");
        } else if (keyInfoList.get(0).duplicate) {
            throw new IllegalArgumentException(
                    "the first key (primary key) must not be a duplicate key.");
        }
        for (CobolFileKeyInfo keyInfo : keyInfoList) {
            if (keyInfo.offset <= 0
                    || keyInfo.size <= 0
                    || keyInfo.offset + keyInfo.size > recordSize + 1) {
                throw new IllegalArgumentException(
                        String.format(
                                "invalid key information: offset=%d, size=%d, record size=%d",
                                keyInfo.offset, keyInfo.size, recordSize));
            }
        }
        // Check if key ranges overlap
        for (int i = 0; i < keyInfoList.size(); i++) {
            CobolFileKeyInfo key1Info = keyInfoList.get(i);
            int key1IndexInf = key1Info.offset;
            int key1IndexSup = key1Info.offset + key1Info.size - 1;
            for (int j = 0; j < i; j++) {
                CobolFileKeyInfo key2Info = keyInfoList.get(j);
                int key2IndexInf = key2Info.offset;
                int key2IndexSup = key2Info.offset + key2Info.size - 1;
                if (!(key1IndexSup < key2IndexInf || key2IndexSup < key1IndexInf)) {
                    throw new IllegalArgumentException(
                            String.format(
                                    "keys overlap: %d,%d and %d,%d",
                                    key1Info.offset,
                                    key1Info.size,
                                    key2Info.offset,
                                    key2Info.size));
                }
            }
        }
    }

    private static void processCreateIndexedFile(
            String indexedFilePath,
            int recordSize,
            List<CobolFileKeyInfo> keyInfoList,
            CommandLine cmd) {
        Optional<CobolFile> cobolFile = createCobolFile(indexedFilePath, recordSize, keyInfoList);
        if (!cobolFile.isPresent()) {
            throw new IllegalArgumentException(
                    String.format(
                            "failed to create a cobol file from the indexed file: %s",
                            indexedFilePath));
        }
        CobolIndexedFile cobolIndexedFile = (CobolIndexedFile) cobolFile.get();
        boolean enableOverwriteOption = cmd.hasOption("n");
        boolean indexedFileAlreadyExists = new File(indexedFilePath).exists();
        if (enableOverwriteOption || !indexedFileAlreadyExists) {
            CobolModule module =
                    new CobolModule(null, null, null, null, 0, '.', '$', ',', 1, 1, 1, 0, null);
            CobolModule.push(module);
            cobolIndexedFile.open(CobolFile.COB_OPEN_OUTPUT, 0, null);
            if (CobolRuntimeException.code != 0) {
                throw new IllegalArgumentException("failed to open the indexed file.");
            }
            cobolIndexedFile.close(0, null);
        } else {
            throw new IllegalArgumentException(
                    String.format(
                            "%s already exists. Use -n or --new option to overwrite it.",
                            indexedFilePath));
        }
    }
}

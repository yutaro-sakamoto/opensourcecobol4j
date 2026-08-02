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
import java.io.InputStream;
import java.nio.file.FileAlreadyExistsException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.nio.file.attribute.PosixFilePermission;
import java.nio.file.attribute.PosixFilePermissions;
import java.util.Set;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.sqlite.SQLiteJDBCLoader;
import org.sqlite.util.OSInfo;

/**
 * INDEXEDファイルのバックエンドであるsqlite-jdbcのネイティブライブラリを、プロセス間で共有する1つのファイルに展開しておくためのユーティリティ。
 *
 * <p>sqlite-jdbcは既定では、最初の接続時にネイティブライブラリを{@code
 * java.io.tmpdir}へ{@code sqlite-<バージョン>-<UUID>-<ライブラリ名>}という名前で展開する。 このとき、展開に先立って同じ一時ディレクトリを走査し、{@code
 * .lck}ファイルを持たない同名パターンのファイルを「すでに終了したプロセスの残骸」とみなして削除する。
 *
 * <p>この一時ディレクトリはマシン上の全プロセスで共有されるため、INDEXEDファイルを扱うCOBOLプログラムを同時に複数起動すると、
 * 2つのプロセスが同じ残骸を削除しようとして競合する。削除できなかった側は、実行結果には何の影響もないにもかかわらず、{@code Failed to delete old native
 * lib}というエラーとスタックトレースを標準エラー出力へ書き出す。
 *
 * <p>そこでSQLiteへ最初に接続する前に、ネイティブライブラリをバージョンごとに1つだけ決まった場所へ展開し、その場所を{@code
 * org.sqlite.lib.path}と{@code org.sqlite.lib.name}でsqlite-jdbcに指示する。
 * こうするとsqlite-jdbc自身は展開も削除も行わなくなり、共有一時ディレクトリに削除対象のファイルが増えないため、競合そのものが起こらない。
 * 展開したファイルはバージョンが同じ限り再利用するので、実行のたびに一時ファイルが増えることもない。
 *
 * <p>準備に失敗した場合は何も設定しない。その場合sqlite-jdbcは従来どおり自身でライブラリを展開するため、動作に支障はない。
 */
public final class SqliteNativeLibrary {
    private static final String LIB_PATH_PROPERTY = "org.sqlite.lib.path";
    private static final String LIB_NAME_PROPERTY = "org.sqlite.lib.name";

    private static final Logger logger = LoggerFactory.getLogger(SqliteNativeLibrary.class);

    /** 準備処理を直列化するためのロック。クラスの内部ロックを外部に晒さないよう専用のオブジェクトを使う。 */
    private static final Object LOCK = new Object();

    private static boolean prepared = false;

    private SqliteNativeLibrary() {}

    /**
     * sqlite-jdbcのネイティブライブラリを共有の場所へ展開し、その場所をシステムプロパティでsqlite-jdbcに指示する。
     *
     * <p>SQLiteへの最初の接続より前に呼び出す必要がある。2回目以降の呼び出しは何もしない。 利用者が{@code
     * org.sqlite.lib.path}を明示的に指定している場合は、その指定を尊重して何もしない。
     */
    public static void prepare() {
        // 準備が終わるまでロックを保持する。先に接続へ進んだスレッドが、
        // まだ設定されていないシステムプロパティを読んでしまうことを防ぐ。
        synchronized (LOCK) {
            if (prepared) {
                return;
            }
            prepared = true;

            if (System.getProperty(LIB_PATH_PROPERTY) != null) {
                return;
            }

            try {
                // sqlite-jdbcが自身のネイティブライブラリを探すときと同じ名前とjar内の配置を使う
                String libraryName = System.mapLibraryName("sqlitejdbc");
                String nativeFolder = OSInfo.getNativeLibFolderPathForCurrentOS();
                String resource = "/org/sqlite/native/" + nativeFolder + "/" + libraryName;

                Path directory =
                        cacheDirectory(
                                SQLiteJDBCLoader.getVersion()
                                        + "-"
                                        + nativeFolder.replace('/', '-'));
                if (directory == null) {
                    return;
                }

                Path library = directory.resolve(libraryName);
                if (!Files.isRegularFile(library)) {
                    extract(resource, directory, library, libraryName);
                }

                if (Files.isRegularFile(library)) {
                    System.setProperty(LIB_PATH_PROPERTY, directory.toString());
                    System.setProperty(LIB_NAME_PROPERTY, libraryName);
                }
            } catch (Exception e) {
                // 準備できなければsqlite-jdbc自身の展開に任せるので、実行には支障がない。
                // 既定では出力されないdebugレベルにとどめ、調べたいときだけ表示できるようにする。
                logger.debug("Failed to prepare the shared sqlite-jdbc native library", e);
            }
        }
    }

    /**
     * 展開先のディレクトリを返す。使えるディレクトリを用意できなければ{@code null}を返す。
     *
     * <p>POSIX環境の{@code /tmp}のように、一時ディレクトリが他の利用者と共有されることがある。 決まった名前のファイルを読み込む以上、そこに別の利用者が書き込めては
     * ならないので、所有者だけが書き込めるディレクトリを作り、すでに存在する場合は所有者と許可属性を確認する。
     */
    private static Path cacheDirectory(String tag) throws IOException {
        Path directory =
                Paths.get(System.getProperty("java.io.tmpdir"))
                        .resolve("opensourcecobol4j-sqlitejdbc-" + tag);
        boolean posix = directory.getFileSystem().supportedFileAttributeViews().contains("posix");

        if (Files.exists(directory)) {
            if (!Files.isDirectory(directory)) {
                return null;
            }
            return !posix || isOwnedAndPrivate(directory) ? directory : null;
        }

        try {
            if (posix) {
                Files.createDirectories(
                        directory,
                        PosixFilePermissions.asFileAttribute(
                                PosixFilePermissions.fromString("rwx------")));
            } else {
                Files.createDirectories(directory);
            }
        } catch (FileAlreadyExistsException e) {
            // 別のプロセスが先に作った
            return !posix || isOwnedAndPrivate(directory) ? directory : null;
        }
        return directory;
    }

    private static boolean isOwnedAndPrivate(Path directory) throws IOException {
        if (!Files.getOwner(directory).getName().equals(System.getProperty("user.name"))) {
            return false;
        }
        Set<PosixFilePermission> permissions = Files.getPosixFilePermissions(directory);
        return !permissions.contains(PosixFilePermission.GROUP_WRITE)
                && !permissions.contains(PosixFilePermission.OTHERS_WRITE);
    }

    /**
     * jarの中のネイティブライブラリを展開する。
     *
     * <p>いったん一時的な名前で書き出してから最終的な名前へ移動する。中途半端な内容のファイルが読み込まれることはなく、
     * 複数のプロセスが同時に展開しても、移動が成功した1つが残るだけで済む。
     */
    private static void extract(String resource, Path directory, Path library, String libraryName)
            throws IOException {
        try (InputStream input = SqliteNativeLibrary.class.getResourceAsStream(resource)) {
            if (input == null) {
                // jar内の配置が想定と違う。sqlite-jdbc自身の展開に任せる
                return;
            }
            Path work = Files.createTempFile(directory, libraryName, ".tmp");
            try {
                Files.copy(input, work, StandardCopyOption.REPLACE_EXISTING);
                Files.move(work, library, StandardCopyOption.ATOMIC_MOVE);
            } catch (IOException | UnsupportedOperationException e) {
                // 他のプロセスが先に用意した場合などはそちらを使う
                Files.deleteIfExists(work);
            }
        }
    }
}

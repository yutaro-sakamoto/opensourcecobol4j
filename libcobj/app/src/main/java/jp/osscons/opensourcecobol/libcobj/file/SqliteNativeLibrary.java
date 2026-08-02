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
    /**
     * ネイティブライブラリを置いたディレクトリをsqlite-jdbcに伝えるためのシステムプロパティ。
     *
     * <p>sqlite-jdbcはライブラリを読み込むとき、まずこのプロパティが指す場所を見に行き、そこにあればそれを使って終わる。
     * 自前で展開する処理はその後ろにあるので、ここで見つけさせれば展開も後始末も行われない。
     */
    private static final String LIB_PATH_PROPERTY = "org.sqlite.lib.path";

    /**
     * ネイティブライブラリのファイル名をsqlite-jdbcに伝えるためのシステムプロパティ。
     *
     * <p>これを指定しない場合、sqlite-jdbcはmacOSでのみ{@code .dylib}を{@code
     * .jnilib}に読み替える。jarに入っているのは{@code .dylib}なので、読み替えられると見つからなくなる。
     * 名前も明示するのはそのためである。
     */
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
            // 失敗しても再試行しない。用意できない環境では毎回同じ結果になるだけなので、
            // 接続のたびに繰り返す意味がない。
            prepared = true;

            // 利用者が置き場所を明示している場合は、その指定を尊重してこのクラスは何もしない。
            if (System.getProperty(LIB_PATH_PROPERTY) != null) {
                return;
            }

            try {
                // ------------------------------------------------------------
                // 手順1: jarの中のどのファイルを取り出すのかを決める
                // ------------------------------------------------------------

                // sqlite-jdbcが自身のネイティブライブラリを探すときと同じ規則で名前と場所を組み立てる。
                // ここがずれると手順3で取り出せず、結局sqlite-jdbc自身の展開に任せることになる。
                // 例: Linux x86_64 なら "/org/sqlite/native/Linux/x86_64/libsqlitejdbc.so"
                String libraryName = System.mapLibraryName("sqlitejdbc");
                String nativeFolder = OSInfo.getNativeLibFolderPathForCurrentOS();
                String resource = "/org/sqlite/native/" + nativeFolder + "/" + libraryName;

                // ------------------------------------------------------------
                // 手順2: 置き場所となるディレクトリを用意する
                // ------------------------------------------------------------

                // 名前にバージョンとプラットフォームを含める。sqlite-jdbcを更新したときに
                // 古いライブラリを使い続けてしまわないようにするためと、
                // 一時ディレクトリが複数の環境から共有される場合に取り違えないようにするため。
                // 例: "opensourcecobol4j-sqlitejdbc-3.51.0.0-Linux-x86_64"
                Path directory =
                        cacheDirectory(
                                SQLiteJDBCLoader.getVersion()
                                        + "-"
                                        + nativeFolder.replace('/', '-'));
                // 安全に使えるディレクトリを用意できなかった場合(詳細はcacheDirectoryを参照)。
                // 何も設定せずに戻れば、sqlite-jdbcが従来どおり自身で展開する。
                if (directory == null) {
                    return;
                }

                // ------------------------------------------------------------
                // 手順3: まだ無ければ取り出す
                // ------------------------------------------------------------

                Path library = directory.resolve(libraryName);
                // 過去の実行、または今まさに動いている別のプロセスが、すでに置いてくれていることがある。
                // その場合は取り出す必要がない。バージョンごとに1つあれば足りるので使い回す。
                if (!Files.isRegularFile(library)) {
                    extract(resource, directory, library, libraryName);
                }

                // ------------------------------------------------------------
                // 手順4: 置き場所をsqlite-jdbcに伝える
                // ------------------------------------------------------------

                // 手順3で取り出せなかった場合(jar内の配置が想定と違うなど)は、ここでファイルが無いままになる。
                // 存在を確かめてから設定するのは、存在しない場所を指してしまうと、sqlite-jdbcが
                // そこを見て諦めるだけになり、かえって読み込みに失敗しかねないため。
                //
                // なお、ここで設定するのはこのプロセスのシステムプロパティであって、
                // 後続のプロセスに引き継がれるものではない。後続のプロセスが得をするのは、
                // 手順3で取り出したファイルが残っていて、取り出しを省けるという点である。
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
     * <p>sqlite-jdbcの既定の展開先はUUIDを含む名前で、他人には狙って作れない。 対してこのクラスは決まった名前のファイルを読み込むので、
     * 名前が事前に分かってしまう。 POSIX環境の{@code /tmp}のように一時ディレクトリが利用者間で共有される場合、
     * 別の利用者が先回りしてそこに細工したライブラリを置けてしまっては困る。 そこで所有者だけが書き込めるディレクトリを作り、
     * すでに存在する場合は所有者と許可属性を確認してから使う。 確認できないディレクトリは使わず、{@code null}を返して呼び出し元に諦めさせる。
     *
     * @param tag ディレクトリ名に含める、バージョンとプラットフォームの識別子
     * @return 使えるディレクトリ。用意できなければ{@code null}
     */
    private static Path cacheDirectory(String tag) throws IOException {
        Path directory =
                Paths.get(System.getProperty("java.io.tmpdir"))
                        .resolve("opensourcecobol4j-sqlitejdbc-" + tag);
        // 許可属性の考え方はOSによって異なる。POSIXの許可属性を扱えるかどうかで処理を分ける。
        // Windowsの一時ディレクトリは利用者ごとに分かれているため、この確認は行わない。
        boolean posix = directory.getFileSystem().supportedFileAttributeViews().contains("posix");

        // --- すでにある場合: 素性を確かめてから使う ---
        if (Files.exists(directory)) {
            // 同じ名前でディレクトリ以外のものが置かれている。細工されている可能性があるので使わない。
            if (!Files.isDirectory(directory)) {
                return null;
            }
            return !posix || isOwnedAndPrivate(directory) ? directory : null;
        }

        // --- 無い場合: 作る ---
        try {
            if (posix) {
                // 作成と同時に許可属性を与える。作ってから変更するのでは、その隙に
                // 他の利用者がファイルを置ける時間ができてしまう。
                Files.createDirectories(
                        directory,
                        PosixFilePermissions.asFileAttribute(
                                PosixFilePermissions.fromString("rwx------")));
            } else {
                Files.createDirectories(directory);
            }
        } catch (FileAlreadyExistsException e) {
            // 存在の確認と作成の間に、別のプロセスが先に作った。
            // 作ったのが同じ利用者とは限らないので、すでにある場合と同じように素性を確かめる。
            return !posix || isOwnedAndPrivate(directory) ? directory : null;
        }
        return directory;
    }

    /**
     * ディレクトリが自分の所有で、かつ他人が書き込めない状態かどうかを確かめる。
     *
     * <p>所有者が自分であることと、グループとその他に書き込み権限が無いことの両方を求める。 どちらか一方でも欠けると、中のファイルを別の利用者に差し替えられる余地が残る。
     */
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
     * <p>いったん一時的な名前で書き出してから最終的な名前へ移動する。 最終的な名前のファイルは移動によって現れるので、
     * 書き出し途中の中途半端な内容が読み込まれることはない。 複数のプロセスが同時にここへ来ても、それぞれ別の一時的な名前へ書き出すので互いを壊さず、
     * 移動が成功した1つだけが残る。
     *
     * @param resource jarの中のネイティブライブラリの位置
     * @param directory 展開先のディレクトリ
     * @param library 最終的なファイルの位置
     * @param libraryName ネイティブライブラリのファイル名。一時的な名前の元にする
     */
    private static void extract(String resource, Path directory, Path library, String libraryName)
            throws IOException {
        try (InputStream input = SqliteNativeLibrary.class.getResourceAsStream(resource)) {
            if (input == null) {
                // jar内の配置が想定と違う。sqlite-jdbcの更新で配置が変わるとここに来る。
                // 何も置かずに戻れば、呼び出し元はプロパティを設定せず、
                // sqlite-jdbc自身の展開に任せることになる(つまり修正前の動作に戻るだけ)。
                return;
            }
            // 他のプロセスと名前がぶつからないよう、一時的な名前を作ってもらう
            Path work = Files.createTempFile(directory, libraryName, ".tmp");
            try {
                Files.copy(input, work, StandardCopyOption.REPLACE_EXISTING);
                // ATOMIC_MOVEを指定して、他のプロセスから見て「無い」か「完全にある」かのどちらかにする
                Files.move(work, library, StandardCopyOption.ATOMIC_MOVE);
            } catch (IOException | UnsupportedOperationException e) {
                // 移動に失敗する主な理由は、別のプロセスが一足先に同じファイルを置き終えていること。
                // 内容は同じなので、そちらを使えばよく、ここでは書きかけを片付けるだけでよい。
                Files.deleteIfExists(work);
            }
        }
    }
}

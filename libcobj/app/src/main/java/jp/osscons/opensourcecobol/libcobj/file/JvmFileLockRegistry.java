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
 * License along with this library; see the file COPYING.LIB; if
 * not, write to the Free Software Foundation, 51 Franklin Street, Fifth Floor
 * Boston, MA 02110-1301 USA
 */
package jp.osscons.opensourcecobol.libcobj.file;

import java.io.IOException;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.channels.NonWritableChannelException;
import java.nio.channels.OverlappingFileLockException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.HashMap;
import java.util.Map;

/**
 * 同一JVM内の複数の実行単位(スレッド)による順編成・行順編成・相対編成ファイルのロックを管理する台帳。
 *
 * <p>OSのファイルロック({@link FileChannel#tryLock})はプロセス単位で管理されており、 同一JVM内で既にロックされている範囲を再度ロックしようとすると
 * {@link OverlappingFileLockException}がスローされる。 このクラスはファイルごとにJVM内での保持状況を記録し、
 * 別プロセスからのOPENと同じ規則(共有ロック同士は共存でき、排他ロックが絡む場合はFILE STATUS 61)で
 * JVM内のスレッド間のロックを判定する。OSのロックは最初の保持者が取得し、最後の保持者が解放するまで維持される。
 *
 * <p>OSのロックはこのクラスが専用に開いたチャネル上に取得するため、保持者のいずれかがファイルを
 * クローズしても他の保持者がいる間はロックが失われない。
 */
final class JvmFileLockRegistry {

    private JvmFileLockRegistry() {}

    /** ファイルごとのロックの保持状況 */
    private static final class Entry {
        /** OSのロックを保持するための専用チャネル */
        FileChannel channel;

        /** OSのロック */
        FileLock osLock;

        /** このJVM内での保持者の数 */
        int holders;

        /** 排他ロックかどうか */
        boolean exclusive;
    }

    /** ロックの保持者に渡す票。クローズ時に{@link #release(Lease)}へ渡す。 */
    static final class Lease {
        private final String key;

        private Lease(String key) {
            this.key = key;
        }
    }

    /** 正規化したパスをキーとするロックの台帳。このクラスのモニタで保護する。 */
    private static final Map<String, Entry> entries = new HashMap<>();

    private static String keyOf(String filename) {
        Path path = Paths.get(filename).toAbsolutePath();
        try {
            return path.toRealPath().toString();
        } catch (IOException e) {
            return path.normalize().toString();
        }
    }

    /**
     * ファイルのロックを取得する。
     *
     * @param filename ロックするファイルのパス
     * @param shared 共有ロックならtrue、排他ロックならfalse
     * @return 取得したロックの票。同一JVM内の他の実行単位またはほかのプロセスと競合して取得できない場合はnull
     * @throws NonWritableChannelException 排他ロックのためにファイルを書き込み用に開けない場合
     * @throws IOException チャネルのオープンまたはロックに失敗した場合
     */
    static synchronized Lease acquire(String filename, boolean shared) throws IOException {
        String key = keyOf(filename);
        Entry entry = entries.get(key);
        if (entry != null) {
            if (entry.exclusive || !shared) {
                return null;
            }
            entry.holders++;
            return new Lease(key);
        }

        FileChannel channel;
        try {
            channel =
                    shared
                            ? FileChannel.open(Paths.get(filename), StandardOpenOption.READ)
                            : FileChannel.open(Paths.get(filename), StandardOpenOption.WRITE);
        } catch (IOException e) {
            if (!shared) {
                NonWritableChannelException nwe = new NonWritableChannelException();
                nwe.initCause(e);
                throw nwe;
            }
            throw e;
        }
        FileLock osLock;
        try {
            osLock = channel.tryLock(0L, Long.MAX_VALUE, shared);
        } catch (OverlappingFileLockException e) {
            // このJVM内で台帳を介さずに取得されたロックと重なった場合
            osLock = null;
        } catch (IOException | RuntimeException e) {
            channel.close();
            throw e;
        }
        if (osLock == null || !osLock.isValid()) {
            channel.close();
            return null;
        }
        entry = new Entry();
        entry.channel = channel;
        entry.osLock = osLock;
        entry.holders = 1;
        entry.exclusive = !shared;
        entries.put(key, entry);
        return new Lease(key);
    }

    /**
     * ロックを解放する。このJVM内の最後の保持者が解放したときにOSのロックも解放する。
     *
     * @param lease {@link #acquire}で取得した票。nullの場合は何もしない
     */
    static synchronized void release(Lease lease) {
        if (lease == null) {
            return;
        }
        Entry entry = entries.get(lease.key);
        if (entry == null) {
            return;
        }
        entry.holders--;
        if (entry.holders > 0) {
            return;
        }
        entries.remove(lease.key);
        try {
            entry.osLock.release();
        } catch (IOException e) {
            System.err.println("Failed to release the lock of " + lease.key);
        }
        try {
            entry.channel.close();
        } catch (IOException e) {
            System.err.println("Failed to close the lock channel of " + lease.key);
        }
    }

    /**
     * 指定したファイルがこのJVM内でロックされているかどうかを返す(テスト用)。
     *
     * @param filename ファイルのパス
     * @return ロックされていればtrue
     */
    static synchronized boolean isLocked(String filename) {
        return entries.containsKey(keyOf(filename));
    }
}

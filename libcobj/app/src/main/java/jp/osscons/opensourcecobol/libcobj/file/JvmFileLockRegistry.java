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
import java.nio.channels.OverlappingFileLockException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 同一JVM内の複数の実行単位(スレッド)による順編成・行順編成・相対編成ファイルのロックを管理する台帳。
 *
 * <p>OSのファイルロック({@link FileChannel#tryLock})はプロセス単位で管理されており、 同一JVM内で既にロックされている範囲を再度ロックしようとすると
 * {@link OverlappingFileLockException}がスローされる。 このクラスはファイルごとにJVM内での保持状況を記録し、
 * 別プロセスからのOPENと同じ規則(共有ロック同士は共存でき、排他ロックが絡む場合はFILE STATUS 61)で
 * JVM内のスレッド間のロックを判定する。
 *
 * <p>OSのロックは最初の保持者がファイルのI/Oに使うチャネル上に取得する(Windowsではロックが必須ロックであり、
 * 別のハンドルで取得したロックは自分自身のI/Oも妨げるため、I/Oに使うチャネル自身で取得する必要がある)。
 * その保持者がクローズしても他の保持者が残っている間は、残った保持者のチャネルにOSのロックを取り直して維持する。
 */
final class JvmFileLockRegistry {

    private JvmFileLockRegistry() {}

    /** ロックの保持者に渡す票。クローズ時に{@link #release(Lease)}へ渡す。 */
    static final class Lease {
        private final String key;
        private final FileChannel channel;

        /** この保持者のチャネルに取得したOSのロック。OSのロックを持っていない場合はnull */
        private FileLock osLock;

        private Lease(String key, FileChannel channel) {
            this.key = key;
            this.channel = channel;
        }
    }

    /** ファイルごとのロックの保持状況 */
    private static final class Entry {
        /** このJVM内での保持者 */
        final List<Lease> holders = new ArrayList<>();

        /** 排他ロックかどうか */
        boolean exclusive;
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
     * @param channel ファイルのI/Oに使うチャネル。OSのロックはこのチャネル上に取得する
     * @param shared 共有ロックならtrue、排他ロックならfalse
     * @return 取得したロックの票。同一JVM内の他の実行単位またはほかのプロセスと競合して取得できない場合はnull
     * @throws java.nio.channels.NonWritableChannelException 読み取り専用のチャネルで排他ロックを取得しようとした場合
     * @throws IOException ロックに失敗した場合
     */
    static synchronized Lease acquire(String filename, FileChannel channel, boolean shared)
            throws IOException {
        String key = keyOf(filename);
        Entry entry = entries.get(key);
        if (entry != null) {
            if (entry.exclusive || !shared) {
                return null;
            }
            Lease lease = new Lease(key, channel);
            entry.holders.add(lease);
            return lease;
        }

        Lease lease = new Lease(key, channel);
        FileLock osLock;
        try {
            osLock = channel.tryLock(0L, Long.MAX_VALUE, shared);
        } catch (OverlappingFileLockException e) {
            // このJVM内で台帳を介さずに取得されたロックと重なった場合
            osLock = null;
        }
        if (osLock == null || !osLock.isValid()) {
            return null;
        }
        lease.osLock = osLock;
        entry = new Entry();
        entry.exclusive = !shared;
        entry.holders.add(lease);
        entries.put(key, entry);
        return lease;
    }

    /**
     * ロックを解放する。OSのロックを持っていた保持者が解放したときに他の保持者が残っていれば、 残った保持者のチャネルにOSのロックを取り直す。
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
        entry.holders.remove(lease);
        if (lease.osLock != null) {
            try {
                lease.osLock.release();
            } catch (IOException e) {
                System.err.println("Failed to release the lock of " + lease.key);
            }
            lease.osLock = null;
            for (Lease other : entry.holders) {
                try {
                    FileLock osLock = other.channel.tryLock(0L, Long.MAX_VALUE, !entry.exclusive);
                    if (osLock != null && osLock.isValid()) {
                        other.osLock = osLock;
                        break;
                    }
                } catch (IOException | RuntimeException e) {
                    System.err.println("Failed to hand over the lock of " + lease.key);
                }
            }
        }
        if (entry.holders.isEmpty()) {
            entries.remove(lease.key);
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

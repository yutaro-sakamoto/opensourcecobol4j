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

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

/** 実行単位間・プロセス間のファイルロック台帳のテスト。 */
class JvmFileLockRegistryTest {

    @TempDir Path tempDir;

    private String newFile() throws IOException {
        Path p = Files.createTempFile(tempDir, "lock", ".dat");
        Files.write(p, new byte[] {'x'});
        return p.toString();
    }

    /** 別のスレッド(別の実行単位)からロックを取得する。 */
    private static JvmFileLockRegistry.Lease acquireFromOtherThread(String file, boolean shared)
            throws Exception {
        ExecutorService executor = Executors.newSingleThreadExecutor();
        try {
            return executor.submit(() -> JvmFileLockRegistry.acquire(file, shared))
                    .get(30, TimeUnit.SECONDS);
        } finally {
            executor.shutdownNow();
        }
    }

    @Test
    void sharedLocksCoexistAcrossThreads() throws Exception {
        String file = newFile();
        JvmFileLockRegistry.Lease mine = JvmFileLockRegistry.acquire(file, true);
        JvmFileLockRegistry.Lease other = acquireFromOtherThread(file, true);
        assertNotNull(mine, "first shared lock");
        assertNotNull(other, "shared lock of another thread coexists");
        JvmFileLockRegistry.release(mine);
        assertTrue(JvmFileLockRegistry.isLocked(file), "still held by the other holder");
        JvmFileLockRegistry.release(other);
        assertFalse(JvmFileLockRegistry.isLocked(file), "released by the last holder");
    }

    @Test
    void exclusiveLockConflictsAcrossThreads() throws Exception {
        String file = newFile();
        JvmFileLockRegistry.Lease shared = JvmFileLockRegistry.acquire(file, true);
        assertNotNull(shared, "shared lock");
        assertNull(acquireFromOtherThread(file, false), "exclusive conflicts with shared");
        JvmFileLockRegistry.release(shared);

        JvmFileLockRegistry.Lease exclusive = JvmFileLockRegistry.acquire(file, false);
        assertNotNull(exclusive, "exclusive lock");
        assertNull(acquireFromOtherThread(file, true), "shared conflicts with exclusive");
        assertNull(acquireFromOtherThread(file, false), "exclusive conflicts with exclusive");
        JvmFileLockRegistry.release(exclusive);
        JvmFileLockRegistry.Lease again = acquireFromOtherThread(file, false);
        assertNotNull(again, "reacquire after release");
        JvmFileLockRegistry.release(again);
    }

    @Test
    void sameThreadMayReopenAndUpgrade() throws Exception {
        String file = newFile();
        JvmFileLockRegistry.Lease shared = JvmFileLockRegistry.acquire(file, true);
        assertNotNull(shared, "shared lock");
        JvmFileLockRegistry.Lease exclusive = JvmFileLockRegistry.acquire(file, false);
        assertNotNull(exclusive, "the same run unit may open the file again for output");
        assertNull(acquireFromOtherThread(file, true), "another thread is refused");

        JvmFileLockRegistry.release(exclusive);
        // 排他の保持者が解放されたので共有へ降格し、他の実行単位の共有オープンが通る
        JvmFileLockRegistry.Lease other = acquireFromOtherThread(file, true);
        assertNotNull(other, "downgraded to shared after the exclusive holder released");
        JvmFileLockRegistry.release(other);
        JvmFileLockRegistry.release(shared);
        assertFalse(JvmFileLockRegistry.isLocked(file), "all leases released");
    }

    @Test
    void upgradeIsRefusedWhileAnotherThreadShares() throws Exception {
        String file = newFile();
        JvmFileLockRegistry.Lease mine = JvmFileLockRegistry.acquire(file, true);
        JvmFileLockRegistry.Lease other = acquireFromOtherThread(file, true);
        assertNotNull(mine, "my shared lock");
        assertNotNull(other, "shared lock of another thread");
        assertNull(
                JvmFileLockRegistry.acquire(file, false),
                "exclusive re-open is refused while another thread shares the file");
        JvmFileLockRegistry.release(other);
        JvmFileLockRegistry.release(mine);
    }

    @Test
    void dataRegionIsNotCoveredByTheLock() throws Exception {
        String file = newFile();
        JvmFileLockRegistry.Lease exclusive = JvmFileLockRegistry.acquire(file, false);
        assertNotNull(exclusive, "exclusive lock");
        try (FileChannel io =
                FileChannel.open(
                        Paths.get(file), StandardOpenOption.READ, StandardOpenOption.WRITE)) {
            // 台帳のロックは実データの範囲を覆っていないので、同一JVMの別チャネルでも
            // データ範囲のロックが取れ(重なっていればOverlappingFileLockException)、
            // 読み書きもロックに妨げられない
            FileLock dataLock = io.tryLock(0L, 1024L, false);
            assertNotNull(dataLock, "a lock on the data region does not overlap");
            dataLock.release();
            io.write(ByteBuffer.wrap(new byte[] {'y'}), 0L);
            ByteBuffer buf = ByteBuffer.allocate(1);
            io.read(buf, 0L);
            assertEquals('y', buf.get(0), "reads and writes go through");
        }
        JvmFileLockRegistry.release(exclusive);
    }

    @Test
    void lockSurvivesTheClosingOfAHoldersOwnChannel() throws Exception {
        String file = newFile();
        // 保持者AとBが共有ロックを取り、Aが自分のI/Oチャネルを閉じても(=CLOSEしても)
        // ロックは台帳のチャネル上にあるため、他の実行単位への保護は途切れない
        JvmFileLockRegistry.Lease a = JvmFileLockRegistry.acquire(file, true);
        JvmFileLockRegistry.Lease b = acquireFromOtherThread(file, true);
        assertNotNull(a, "holder A");
        assertNotNull(b, "holder B");
        FileChannel aChannel = FileChannel.open(Paths.get(file), StandardOpenOption.READ);
        aChannel.close();
        JvmFileLockRegistry.release(a);
        assertNull(
                acquireFromOtherThread(file, false),
                "exclusive is still refused while B holds the file");
        JvmFileLockRegistry.release(b);
        JvmFileLockRegistry.Lease exclusive = acquireFromOtherThread(file, false);
        assertNotNull(exclusive, "exclusive succeeds after the last holder released");
        JvmFileLockRegistry.release(exclusive);
    }

    @Test
    void differentFilesDoNotConflict() throws Exception {
        String a = newFile();
        String b = newFile();
        JvmFileLockRegistry.Lease la = JvmFileLockRegistry.acquire(a, false);
        JvmFileLockRegistry.Lease lb = JvmFileLockRegistry.acquire(b, false);
        assertNotNull(la, "lock on a");
        assertNotNull(lb, "lock on b");
        JvmFileLockRegistry.release(la);
        JvmFileLockRegistry.release(lb);
    }
}

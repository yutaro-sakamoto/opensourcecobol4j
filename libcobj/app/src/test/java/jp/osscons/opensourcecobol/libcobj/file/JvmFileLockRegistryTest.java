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

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

/** 同一JVM内のファイルロック台帳のテスト。 */
class JvmFileLockRegistryTest {

    @TempDir Path tempDir;

    private String newFile() throws IOException {
        Path p = Files.createTempFile(tempDir, "lock", ".dat");
        Files.write(p, new byte[] {'x'});
        return p.toString();
    }

    /** 別のスレッド(別の実行単位)からロックを取得する。 */
    private static JvmFileLockRegistry.Lease acquireFromOtherThread(
            String file, FileChannel channel, boolean shared) throws Exception {
        ExecutorService executor = Executors.newSingleThreadExecutor();
        try {
            return executor.submit(() -> JvmFileLockRegistry.acquire(file, channel, shared))
                    .get(30, TimeUnit.SECONDS);
        } finally {
            executor.shutdownNow();
        }
    }

    private static FileChannel openRead(String file) throws IOException {
        return FileChannel.open(Paths.get(file), StandardOpenOption.READ);
    }

    private static FileChannel openWrite(String file) throws IOException {
        return FileChannel.open(Paths.get(file), StandardOpenOption.READ, StandardOpenOption.WRITE);
    }

    @Test
    void sharedLocksCoexist() throws Exception {
        String file = newFile();
        try (FileChannel ca = openRead(file);
                FileChannel cb = openRead(file)) {
            JvmFileLockRegistry.Lease a = JvmFileLockRegistry.acquire(file, ca, true);
            JvmFileLockRegistry.Lease b = JvmFileLockRegistry.acquire(file, cb, true);
            assertNotNull(a, "first shared lock");
            assertNotNull(b, "second shared lock coexists with the first");
            JvmFileLockRegistry.release(a);
            assertTrue(JvmFileLockRegistry.isLocked(file), "still held by the second holder");
            JvmFileLockRegistry.release(b);
            assertFalse(JvmFileLockRegistry.isLocked(file), "released by the last holder");
        }
    }

    @Test
    void exclusiveLockConflictsWithSharedAndExclusive() throws Exception {
        String file = newFile();
        try (FileChannel cs = openRead(file);
                FileChannel ce = openWrite(file);
                FileChannel other = openWrite(file)) {
            JvmFileLockRegistry.Lease shared = JvmFileLockRegistry.acquire(file, cs, true);
            assertNotNull(shared, "shared lock");
            assertNull(
                    acquireFromOtherThread(file, other, false), "exclusive conflicts with shared");
            JvmFileLockRegistry.release(shared);

            JvmFileLockRegistry.Lease exclusive = JvmFileLockRegistry.acquire(file, ce, false);
            assertNotNull(exclusive, "exclusive lock");
            assertNull(
                    acquireFromOtherThread(file, other, true), "shared conflicts with exclusive");
            assertNull(
                    acquireFromOtherThread(file, other, false),
                    "exclusive conflicts with exclusive");
            JvmFileLockRegistry.release(exclusive);
            JvmFileLockRegistry.Lease again = acquireFromOtherThread(file, other, false);
            assertNotNull(again, "reacquire after release");
            JvmFileLockRegistry.release(again);
        }
    }

    @Test
    void sameThreadMayReopenTheFileInAnyMode() throws Exception {
        String file = newFile();
        try (FileChannel cs = openRead(file);
                FileChannel ce = openWrite(file);
                FileChannel other = openWrite(file)) {
            JvmFileLockRegistry.Lease shared = JvmFileLockRegistry.acquire(file, cs, true);
            assertNotNull(shared, "shared lock");
            JvmFileLockRegistry.Lease exclusive = JvmFileLockRegistry.acquire(file, ce, false);
            assertNotNull(exclusive, "the same run unit may open the file again for output");
            ExecutorService executor = Executors.newSingleThreadExecutor();
            try {
                Future<JvmFileLockRegistry.Lease> attempt =
                        executor.submit(() -> JvmFileLockRegistry.acquire(file, other, true));
                assertNull(attempt.get(30, TimeUnit.SECONDS), "another thread is refused");
            } finally {
                executor.shutdownNow();
            }
            JvmFileLockRegistry.release(exclusive);
            JvmFileLockRegistry.release(shared);
            assertFalse(JvmFileLockRegistry.isLocked(file), "all leases released");
        }
    }

    @Test
    void lockOwnerCanStillWriteThroughItsChannel() throws Exception {
        String file = newFile();
        try (FileChannel ce = openWrite(file)) {
            JvmFileLockRegistry.Lease exclusive = JvmFileLockRegistry.acquire(file, ce, false);
            assertNotNull(exclusive, "exclusive lock");
            ce.write(ByteBuffer.wrap(new byte[] {'y'}), 0);
            JvmFileLockRegistry.release(exclusive);
        }
        assertTrue(Files.readAllBytes(Paths.get(file))[0] == 'y', "write went through");
    }

    @Test
    void lockIsHandedOverWhenTheFirstHolderCloses() throws Exception {
        String file = newFile();
        try (FileChannel ca = openRead(file);
                FileChannel cb = openRead(file);
                FileChannel other = openWrite(file)) {
            JvmFileLockRegistry.Lease a = JvmFileLockRegistry.acquire(file, ca, true);
            JvmFileLockRegistry.Lease b = JvmFileLockRegistry.acquire(file, cb, true);
            assertNotNull(a, "first shared lock");
            assertNotNull(b, "second shared lock");
            JvmFileLockRegistry.release(a);
            ca.close();
            assertNull(
                    acquireFromOtherThread(file, other, false),
                    "exclusive still refused while the second holder remains");
            JvmFileLockRegistry.release(b);
            JvmFileLockRegistry.Lease ex = acquireFromOtherThread(file, other, false);
            assertNotNull(ex, "exclusive lock after all holders released");
            JvmFileLockRegistry.release(ex);
        }
    }

    @Test
    void lockIsHeldAcrossThreads() throws Exception {
        String file = newFile();
        try (FileChannel ce = openWrite(file);
                FileChannel other = openRead(file)) {
            JvmFileLockRegistry.Lease exclusive = JvmFileLockRegistry.acquire(file, ce, false);
            assertNotNull(exclusive, "exclusive lock");
            ExecutorService executor = Executors.newSingleThreadExecutor();
            try {
                Future<JvmFileLockRegistry.Lease> attempt =
                        executor.submit(() -> JvmFileLockRegistry.acquire(file, other, true));
                assertNull(attempt.get(30, TimeUnit.SECONDS), "another thread sees the conflict");
                JvmFileLockRegistry.release(exclusive);
                Future<JvmFileLockRegistry.Lease> again =
                        executor.submit(() -> JvmFileLockRegistry.acquire(file, other, true));
                JvmFileLockRegistry.Lease lease = again.get(30, TimeUnit.SECONDS);
                assertNotNull(lease, "another thread can lock after release");
                JvmFileLockRegistry.release(lease);
            } finally {
                executor.shutdownNow();
            }
        }
    }
}

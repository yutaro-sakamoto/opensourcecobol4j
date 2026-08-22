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
import java.nio.file.Files;
import java.nio.file.Path;
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

    @Test
    void sharedLocksCoexist() throws Exception {
        String file = newFile();
        JvmFileLockRegistry.Lease a = JvmFileLockRegistry.acquire(file, true);
        JvmFileLockRegistry.Lease b = JvmFileLockRegistry.acquire(file, true);
        assertNotNull(a, "first shared lock");
        assertNotNull(b, "second shared lock coexists with the first");
        JvmFileLockRegistry.release(a);
        assertTrue(JvmFileLockRegistry.isLocked(file), "still held by the second holder");
        JvmFileLockRegistry.release(b);
        assertFalse(JvmFileLockRegistry.isLocked(file), "released by the last holder");
    }

    @Test
    void exclusiveLockConflictsWithSharedAndExclusive() throws Exception {
        String file = newFile();
        JvmFileLockRegistry.Lease shared = JvmFileLockRegistry.acquire(file, true);
        assertNotNull(shared, "shared lock");
        assertNull(JvmFileLockRegistry.acquire(file, false), "exclusive conflicts with shared");
        JvmFileLockRegistry.release(shared);

        JvmFileLockRegistry.Lease exclusive = JvmFileLockRegistry.acquire(file, false);
        assertNotNull(exclusive, "exclusive lock");
        assertNull(JvmFileLockRegistry.acquire(file, true), "shared conflicts with exclusive");
        assertNull(JvmFileLockRegistry.acquire(file, false), "exclusive conflicts with exclusive");
        JvmFileLockRegistry.release(exclusive);
        assertNotNull(JvmFileLockRegistry.acquire(file, false), "reacquire after release");
    }

    @Test
    void lockIsHeldAcrossThreads() throws Exception {
        String file = newFile();
        JvmFileLockRegistry.Lease exclusive = JvmFileLockRegistry.acquire(file, false);
        assertNotNull(exclusive, "exclusive lock");
        ExecutorService executor = Executors.newSingleThreadExecutor();
        try {
            Future<JvmFileLockRegistry.Lease> other =
                    executor.submit(() -> JvmFileLockRegistry.acquire(file, true));
            assertNull(other.get(30, TimeUnit.SECONDS), "another thread sees the conflict");
            JvmFileLockRegistry.release(exclusive);
            Future<JvmFileLockRegistry.Lease> again =
                    executor.submit(() -> JvmFileLockRegistry.acquire(file, true));
            JvmFileLockRegistry.Lease lease = again.get(30, TimeUnit.SECONDS);
            assertNotNull(lease, "another thread can lock after release");
            JvmFileLockRegistry.release(lease);
        } finally {
            executor.shutdownNow();
        }
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

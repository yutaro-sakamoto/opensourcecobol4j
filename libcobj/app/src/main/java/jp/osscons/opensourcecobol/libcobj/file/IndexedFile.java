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

import java.sql.Connection;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/**
 * Holds the runtime state of one opened INDEXED file.
 *
 * <p>An instance is created by {@link CobolIndexedFile#open_(String, int, int)} and kept in {@code
 * CobolFile.filei} for the lifetime of the open. It bundles the JDBC connection to the backing
 * SQLite database together with the current key/record buffers and other bookkeeping used while
 * executing COBOL I/O statements. This is a package-private plain data holder; the actual logic
 * lives in {@link CobolIndexedFile}.
 */
class IndexedFile {
    /** Index into the key array identifying the key most recently used (e.g. by {@code START}). */
    int key_index;

    /** The primary key value written by the previous {@code WRITE}, used for sequential ordering. */
    CobolDataStorage last_key;

    /** A scratch buffer large enough to hold the largest key, sized at open time. */
    CobolDataStorage temp_key;

    /** The JDBC connection to the SQLite database that backs this file. */
    Connection connection;

    /** The current key value (raw bytes) being read or written. */
    byte[] key;

    /** The current record value (raw bytes) being read or written. */
    byte[] data;

    /** Reserved for per-key read bookkeeping; not actively used by the current implementation. */
    byte[][] last_readkey;

    /**
     * Reserved per-key duplicate-number ({@code dupNo}) state; allocated at open time but not
     * actively read by the current implementation.
     */
    int[] last_dupno;

    /**
     * Reserved per-key state for REWRITE; allocated at open time but not actively used by the
     * current implementation, which instead threads duplicate numbers through a local array.
     */
    int[] rewrite_sec_key;

    /** The resolved path of the SQLite database file. */
    String filename;

    /** Reserved for record-lock bookkeeping. */
    Object record_lock;

    /** {@code true} while a write cursor is open during a WRITE/REWRITE/DELETE operation. */
    boolean write_cursor_open;

    /** Reserved lock identifier. */
    int lock_id;

    /** {@code true} while this process currently holds a record lock. */
    boolean record_locked;

    /** The length, in characters, of {@link #filename}. */
    int filenamelen;
}

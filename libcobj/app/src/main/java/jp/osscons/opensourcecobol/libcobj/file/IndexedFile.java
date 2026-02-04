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
 * INDEXEDファイルの内部状態を保持するクラス。
 *
 * <p>COBOL INDEXED FILE（索引ファイル）の実装において、SQLiteデータベースへの接続情報、
 * 現在のキー情報、最後に読み取ったキー値、ロック状態などの内部状態を管理する。 {@link CobolIndexedFile}がファイル操作の実行時にこのクラスのインスタンスを使用する。
 *
 * <p>libcob/fileio.cのstruct indexfileに対応する。
 */
class IndexedFile {
    /** 現在使用中のキーのインデックス。0が主キー、1以降が代替キー */
    int key_index;

    /** 最後に読み取ったキー値を保持するストレージ */
    CobolDataStorage last_key;

    /** キー操作時の一時バッファ */
    CobolDataStorage temp_key;

    /** SQLiteデータベースへのJDBC接続 */
    Connection connection;

    /** 現在のキー値（バイト配列形式） */
    byte[] key;

    /** 現在のレコードデータ（バイト配列形式） */
    byte[] data;

    /** 各キーごとに最後に読み取ったキー値を保持する配列 */
    byte[][] last_readkey;

    /** 各キーごとに最後に読み取った重複番号を保持する配列（DUPLICATES指定時に使用） */
    int[] last_dupno;

    /** REWRITE操作時に代替キーの更新が必要かどうかを示すフラグ配列 */
    int[] rewrite_sec_key;

    /** SQLiteデータベースファイルのパス */
    String filename;

    /** レコードロックに使用するオブジェクト */
    Object record_lock;

    /** 書き込み用カーソルがオープンされているかどうか */
    boolean write_cursor_open;

    /** 現在のファイルロックID */
    int lock_id;

    /** 現在のレコードがロックされているかどうか */
    boolean record_locked;

    /** ファイル名の長さ */
    int filenamelen;
}

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

import java.sql.SQLException;
import org.sqlite.SQLiteErrorCode;
import org.sqlite.SQLiteException;

/**
 * WALモードで発生した{@code SQLITE_BUSY_SNAPSHOT}を、リトライ処理を行う呼び出し元まで伝えるための例外。
 *
 * <p>WALモードでは、読み取りを行ってから書き込みを行うDEFERREDトランザクションの実行中に他の接続がコミットすると、
 * 書き込み時に{@code SQLITE_BUSY_SNAPSHOT}が返る。これは{@code PRAGMA
 * busy_timeout}では再試行されず、トランザクションをロールバックしてやり直すしかない。
 *
 * <p>{@link CobolIndexedFile}と{@link
 * IndexedCursor}の例外処理は、いずれもSQLの失敗をステータスコードや{@code
 * Optional.empty()}に潰してしまうため、この競合が「レコードが存在しない」「入出力エラー」として観測されてしまう。
 * それを防ぐため、この非チェック例外で競合だけを上位に貫通させる。
 *
 * <p>検査には拡張エラーコードを使う必要がある。{@code SQLException.getErrorCode()}は主エラーコードの{@code
 * 5}({@code SQLITE_BUSY})しか返さず、通常の{@code SQLITE_BUSY}と区別できない。
 */
class SnapshotConflictException extends RuntimeException {

    private static final long serialVersionUID = 1L;

    SnapshotConflictException(SQLException cause) {
        super(cause);
    }

    /**
     * 例外が{@code SQLITE_BUSY_SNAPSHOT}を表す場合、{@link SnapshotConflictException}に包んで送出する。
     *
     * <p>該当しない場合は何もせずに戻る。各{@code catch}節の先頭で呼び出すことを想定している。
     *
     * @param e 検査対象の例外
     */
    static void rethrowIfSnapshotConflict(Throwable e) {
        if (!(e instanceof SQLiteException)) {
            return;
        }
        SQLiteException sqliteException = (SQLiteException) e;
        if (sqliteException.getResultCode().code == SQLiteErrorCode.SQLITE_BUSY_SNAPSHOT.code) {
            throw new SnapshotConflictException(sqliteException);
        }
    }
}

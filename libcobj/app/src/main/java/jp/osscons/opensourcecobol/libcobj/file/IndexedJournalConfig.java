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
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import org.sqlite.SQLiteErrorCode;

/**
 * INDEXEDファイルのバックエンドであるSQLiteデータベースのジャーナルモードの設定。
 *
 * <p>環境変数{@code COB_INDEXED_JOURNAL_MODE}から取得する。指定できる値は{@code WAL}と{@code
 * DELETE}の2つで、大文字小文字は区別しない。既定値は{@code WAL}であり、{@code
 * DELETE}を指定するとopensource COBOL 4J 2.0.0以前と同じロールバックジャーナルモードで動作する。
 * それ以外の値を指定した場合は警告を標準エラー出力に表示したうえで{@code WAL}として扱う。
 * プロセス起動時に一度だけ環境変数を読み取り、以降はキャッシュした値を返す。
 *
 * <p>WALモードでは{@code
 * ファイル名-wal}と{@code ファイル名-shm}の2つの補助ファイルがINDEXEDファイルと同じディレクトリに作られる。
 * これらは最後の接続が正常にクローズされた時点でSQLiteが自動的に削除する。
 * ネットワークファイルシステム上ではWALに必要な共有メモリが利用できないため、
 * そのような環境では{@code DELETE}を指定する必要がある。
 *
 * <p>ジャーナルモードはSQLiteデータベースのヘッダに永続化されるため、
 * 環境変数を切り替えるだけでは既存のファイルのモードは変わらない。そのため{@link
 * #applyTo(Connection)}をファイルを開くたびに呼び出し、両方向の変換を行えるようにしている。
 */
public final class IndexedJournalConfig {

    /** ユーティリティクラスのインスタンス化を防ぐためのprivateコンストラクタ。 */
    private IndexedJournalConfig() {}

    /** ジャーナルモードを指定する環境変数名。 */
    public static final String JOURNAL_MODE_ENV = "COB_INDEXED_JOURNAL_MODE";

    /** WALモードを表す{@code PRAGMA journal_mode}の値。 */
    private static final String MODE_WAL = "wal";

    /** ロールバックジャーナルモードを表す{@code PRAGMA journal_mode}の値。 */
    private static final String MODE_DELETE = "delete";

    private static final String journalMode = readFromEnv();

    private static String readFromEnv() {
        // COBOLのSET ENVIRONMENTで実行中に書き換わりうるCobolUtil.getEnvではなく、
        // プロセス起動時の値で固定したいのでSystem.getenvを直接参照する。
        String value = System.getenv(JOURNAL_MODE_ENV);
        if (value == null) {
            return MODE_WAL;
        }
        String trimmed = value.trim();
        if (trimmed.isEmpty() || MODE_WAL.equalsIgnoreCase(trimmed)) {
            return MODE_WAL;
        }
        if (MODE_DELETE.equalsIgnoreCase(trimmed)) {
            return MODE_DELETE;
        }
        System.err.println(
                "Warning: " + JOURNAL_MODE_ENV + " must be WAL or DELETE, ignored: " + value);
        return MODE_WAL;
    }

    /**
     * SQLiteの{@code PRAGMA journal_mode}に設定する値を返す。
     *
     * @return {@code "wal"}または{@code "delete"}
     */
    public static String getJournalMode() {
        return journalMode;
    }

    /**
     * WALモードが有効かどうかを返す。
     *
     * @return WALモードなら{@code true}、ロールバックジャーナルモードなら{@code false}
     */
    public static boolean isWalEnabled() {
        return MODE_WAL.equals(journalMode);
    }

    /**
     * 設定されたジャーナルモードを接続に適用する。
     *
     * <p>{@code PRAGMA
     * journal_mode}はトランザクションの内側では変更できないため、必ず{@code Connection.setAutoCommit(false)}を呼び出す前に、
     * すなわち自動コミットが有効な状態で呼び出さなければならない。
     *
     * <p>他のプロセスが同じファイルを開いていると、変換は行われない。WALへの変換の場合、SQLiteはエラーを返さずに現在のモードをそのまま返す。
     * 一方WALから抜ける変換は排他ロックを必要とするため{@code
     * SQLITE_BUSY}になる。いずれの場合もファイルは現在のモードのまま正しく読み書きできるので、ここでは失敗として扱わない。
     * そうしないと、WALをやめようとした利用者が、他のプロセスがファイルを開いているというだけでファイルステータス61でOPENに失敗してしまう。
     *
     * @param connection 適用先の接続。自動コミットが有効であること
     * @throws SQLException ビジー以外の理由でPRAGMAの実行に失敗した場合
     */
    public static void applyTo(Connection connection) throws SQLException {
        try (Statement statement = connection.createStatement();
                ResultSet rs = statement.executeQuery("PRAGMA journal_mode = " + journalMode)) {
            // PRAGMA journal_modeは変換後のモードを1行返すが、変換が見送られた場合は現在のモードが
            // 返るだけなので、戻り値は検査しない。
            rs.next();
        } catch (SQLException e) {
            if (e.getErrorCode() != SQLiteErrorCode.SQLITE_BUSY.code) {
                throw e;
            }
        }
    }
}

package jp.osscons.opensourcecobol.libcobj.sql;

import java.util.HashMap;
import java.util.Map;

/** SQL 接続、カーソル、prepared statement を管理するグローバルなレジストリ。 */
final class SqlState {

    /** ユーティリティクラスのインスタンス化を防ぐための private コンストラクタ。 */
    private SqlState() {}

    private static Map<String, SqlConnection> connections = new HashMap<>();
    private static Map<String, SqlCursor> cursors = new HashMap<>();
    private static Map<String, String[]> preparedStatements = new HashMap<>();
    private static String defaultConnId = null;

    /**
     * 接続を登録する。最初に登録された接続がデフォルトになる。
     *
     * @param id 接続の識別子
     * @param conn SQL 接続
     */
    static void addConnection(String id, SqlConnection conn) {
        connections.put(id, conn);
        if (defaultConnId == null) {
            defaultConnId = id;
        }
    }

    /**
     * デフォルトの接続を取得する。
     *
     * @return デフォルトの接続。登録されていない場合は null
     */
    static SqlConnection getDefaultConnection() {
        if (defaultConnId != null) {
            return connections.get(defaultConnId);
        }
        if (!connections.isEmpty()) {
            return connections.values().iterator().next();
        }
        return null;
    }

    /**
     * ID を指定して接続を削除する。それがデフォルトだった場合は、新しいデフォルトが選択される。
     *
     * @param id 削除する接続の識別子
     */
    static void removeConnection(String id) {
        connections.remove(id);
        if (id != null && id.equals(defaultConnId)) {
            if (!connections.isEmpty()) {
                defaultConnId = connections.keySet().iterator().next();
            } else {
                defaultConnId = null;
            }
        }
    }

    /**
     * 名前を指定してカーソルを登録する。
     *
     * @param name カーソル名
     * @param cursor カーソルのディスクリプタ
     */
    static void addCursor(String name, SqlCursor cursor) {
        cursors.put(name, cursor);
    }

    /**
     * 名前を指定してカーソルを検索する。
     *
     * @param name カーソル名
     * @return カーソルのディスクリプタ。見つからない場合は null
     */
    static SqlCursor getCursor(String name) {
        return cursors.get(name);
    }

    /**
     * prepared statement を、その query とパラメータ数とともに登録する。
     *
     * @param name statement 名
     * @param query SQL query 文字列
     * @param nParams パラメータの個数
     */
    static void addPrepared(String name, String query, int nParams) {
        preparedStatements.put(name, new String[] {query, String.valueOf(nParams)});
    }

    /**
     * 名前を指定して prepared statement を検索する。
     *
     * @param name statement 名
     * @return [query, nParams] の2要素の配列。見つからない場合は null
     */
    static String[] getPrepared(String name) {
        return preparedStatements.get(name);
    }

    /** すべてのカーソルをクローズ済みとしてマークする（例: COMMIT や ROLLBACK の後）。 */
    static void clearCursors() {
        for (SqlCursor cursor : cursors.values()) {
            cursor.isOpened = false;
        }
    }
}

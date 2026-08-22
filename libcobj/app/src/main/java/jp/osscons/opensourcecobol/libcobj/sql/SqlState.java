package jp.osscons.opensourcecobol.libcobj.sql;

import java.util.HashMap;
import java.util.Map;

/** SQL 接続、カーソル、prepared statement を管理するレジストリ。実行単位(スレッド)ごとに独立している。 */
final class SqlState {

    /**
     * 実行単位(スレッド)ごとに保持する作業状態。<br>
     * 文の処理は複数のメソッド呼び出しにまたがって状態を共有するため、スレッドごとに独立させる。
     */
    private static final class State {
        Map<String, SqlConnection> connections = new HashMap<>();

        Map<String, SqlCursor> cursors = new HashMap<>();

        Map<String, String[]> preparedStatements = new HashMap<>();

        String defaultConnId = null;
    }

    /** 現在のスレッドの作業状態。実行単位はスレッドごとに独立しているため、スレッドごとに保持する。 */
    private static final ThreadLocal<State> state = ThreadLocal.withInitial(State::new);

    /** 現在のスレッドに紐づく作業状態を破棄する。実行単位の終了時に呼び出す。 */
    public static void resetThreadState() {
        state.remove();
    }

    /** ユーティリティクラスのインスタンス化を防ぐための private コンストラクタ。 */
    private SqlState() {}

    /**
     * 現在のスレッドに登録されているすべての接続を返す(テスト用)。
     *
     * @return 接続の識別子と接続の対応表
     */
    static Map<String, SqlConnection> allConnections() {
        return state.get().connections;
    }

    /**
     * 接続を登録する。最初に登録された接続がデフォルトになる。
     *
     * @param id 接続の識別子
     * @param conn SQL 接続
     */
    static void addConnection(String id, SqlConnection conn) {
        State st = state.get();
        st.connections.put(id, conn);
        if (st.defaultConnId == null) {
            st.defaultConnId = id;
        }
    }

    /**
     * デフォルトの接続を取得する。
     *
     * @return デフォルトの接続。登録されていない場合は null
     */
    static SqlConnection getDefaultConnection() {
        State st = state.get();
        if (st.defaultConnId != null) {
            return st.connections.get(st.defaultConnId);
        }
        if (!st.connections.isEmpty()) {
            return st.connections.values().iterator().next();
        }
        return null;
    }

    /**
     * ID を指定して接続を削除する。それがデフォルトだった場合は、新しいデフォルトが選択される。
     *
     * @param id 削除する接続の識別子
     */
    static void removeConnection(String id) {
        State st = state.get();
        st.connections.remove(id);
        if (id != null && id.equals(st.defaultConnId)) {
            if (!st.connections.isEmpty()) {
                st.defaultConnId = st.connections.keySet().iterator().next();
            } else {
                st.defaultConnId = null;
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
        State st = state.get();
        st.cursors.put(name, cursor);
    }

    /**
     * 名前を指定してカーソルを検索する。
     *
     * @param name カーソル名
     * @return カーソルのディスクリプタ。見つからない場合は null
     */
    static SqlCursor getCursor(String name) {
        State st = state.get();
        return st.cursors.get(name);
    }

    /**
     * prepared statement を、その query とパラメータ数とともに登録する。
     *
     * @param name statement 名
     * @param query SQL query 文字列
     * @param nParams パラメータの個数
     */
    static void addPrepared(String name, String query, int nParams) {
        State st = state.get();
        st.preparedStatements.put(name, new String[] {query, String.valueOf(nParams)});
    }

    /**
     * 名前を指定して prepared statement を検索する。
     *
     * @param name statement 名
     * @return [query, nParams] の2要素の配列。見つからない場合は null
     */
    static String[] getPrepared(String name) {
        State st = state.get();
        return st.preparedStatements.get(name);
    }

    /** すべてのカーソルをクローズ済みとしてマークする（例: COMMIT や ROLLBACK の後）。 */
    static void clearCursors() {
        State st = state.get();
        for (SqlCursor cursor : st.cursors.values()) {
            cursor.isOpened = false;
            // COMMIT/ROLLBACK でサーバカーソルは消えるため、先読みバッファも破棄する。
            cursor.clearBuffer();
        }
    }
}

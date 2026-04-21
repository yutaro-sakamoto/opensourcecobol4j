package jp.osscons.opensourcecobol.libcobj.sql;

import java.util.HashMap;
import java.util.Map;

public class SqlState {

    private static Map<String, SqlConnection> connections = new HashMap<>();
    private static Map<String, SqlCursor> cursors = new HashMap<>();
    private static Map<String, String[]> preparedStatements = new HashMap<>();
    private static String defaultConnId = null;

    public static void addConnection(String id, SqlConnection conn) {
        connections.put(id, conn);
        if (defaultConnId == null) {
            defaultConnId = id;
        }
    }

    public static SqlConnection getConnection(String id) {
        if (id == null) {
            return getDefaultConnection();
        }
        return connections.get(id);
    }

    public static SqlConnection getDefaultConnection() {
        if (defaultConnId != null) {
            return connections.get(defaultConnId);
        }
        if (!connections.isEmpty()) {
            return connections.values().iterator().next();
        }
        return null;
    }

    public static void removeConnection(String id) {
        connections.remove(id);
        if (id != null && id.equals(defaultConnId)) {
            if (!connections.isEmpty()) {
                defaultConnId = connections.keySet().iterator().next();
            } else {
                defaultConnId = null;
            }
        }
    }

    public static void addCursor(String name, SqlCursor cursor) {
        cursors.put(name, cursor);
    }

    public static SqlCursor getCursor(String name) {
        return cursors.get(name);
    }

    public static void removeCursor(String name) {
        cursors.remove(name);
    }

    public static void addPrepared(String name, String query, int nParams) {
        preparedStatements.put(name, new String[] {query, String.valueOf(nParams)});
    }

    public static String[] getPrepared(String name) {
        return preparedStatements.get(name);
    }

    public static void clearCursors() {
        for (SqlCursor cursor : cursors.values()) {
            cursor.isOpened = false;
        }
    }
}

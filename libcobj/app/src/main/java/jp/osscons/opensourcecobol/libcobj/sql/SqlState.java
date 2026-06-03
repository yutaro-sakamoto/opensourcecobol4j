package jp.osscons.opensourcecobol.libcobj.sql;

import java.util.HashMap;
import java.util.Map;

/** Global registry for SQL connections, cursors, and prepared statements. */
final class SqlState {

    /** Private constructor to prevent instantiation of utility class. */
    private SqlState() {}

    private static Map<String, SqlConnection> connections = new HashMap<>();
    private static Map<String, SqlCursor> cursors = new HashMap<>();
    private static Map<String, String[]> preparedStatements = new HashMap<>();
    private static String defaultConnId = null;

    /**
     * Register a connection. The first registered connection becomes the default.
     *
     * @param id the connection identifier
     * @param conn the SQL connection
     */
    static void addConnection(String id, SqlConnection conn) {
        connections.put(id, conn);
        if (defaultConnId == null) {
            defaultConnId = id;
        }
    }

    /**
     * Get the default connection.
     *
     * @return the default connection, or null if none registered
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
     * Remove a connection by ID. If it was the default, a new default is chosen.
     *
     * @param id the connection identifier to remove
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
     * Register a cursor by name.
     *
     * @param name the cursor name
     * @param cursor the cursor descriptor
     */
    static void addCursor(String name, SqlCursor cursor) {
        cursors.put(name, cursor);
    }

    /**
     * Look up a cursor by name.
     *
     * @param name the cursor name
     * @return the cursor descriptor, or null if not found
     */
    static SqlCursor getCursor(String name) {
        return cursors.get(name);
    }

    /**
     * Register a prepared statement with its query and parameter count.
     *
     * @param name the statement name
     * @param query the SQL query string
     * @param nParams the number of parameters
     */
    static void addPrepared(String name, String query, int nParams) {
        preparedStatements.put(name, new String[] {query, String.valueOf(nParams)});
    }

    /**
     * Look up a prepared statement by name.
     *
     * @param name the statement name
     * @return a two-element array [query, nParams], or null if not found
     */
    static String[] getPrepared(String name) {
        return preparedStatements.get(name);
    }

    /** Mark all cursors as closed (e.g. after COMMIT or ROLLBACK). */
    static void clearCursors() {
        for (SqlCursor cursor : cursors.values()) {
            cursor.isOpened = false;
        }
    }
}

package jp.osscons.opensourcecobol.libcobj.sql;

import java.util.HashMap;
import java.util.Map;

/** Global registry for SQL connections, cursors, and prepared statements. */
public class SqlState {

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
    public static void addConnection(String id, SqlConnection conn) {
        connections.put(id, conn);
        if (defaultConnId == null) {
            defaultConnId = id;
        }
    }

    /**
     * Get a connection by ID, or the default connection if id is null.
     *
     * @param id the connection identifier, or null for the default
     * @return the connection, or null if not found
     */
    public static SqlConnection getConnection(String id) {
        if (id == null) {
            return getDefaultConnection();
        }
        return connections.get(id);
    }

    /**
     * Get the default connection.
     *
     * @return the default connection, or null if none registered
     */
    public static SqlConnection getDefaultConnection() {
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

    /**
     * Register a cursor by name.
     *
     * @param name the cursor name
     * @param cursor the cursor descriptor
     */
    public static void addCursor(String name, SqlCursor cursor) {
        cursors.put(name, cursor);
    }

    /**
     * Look up a cursor by name.
     *
     * @param name the cursor name
     * @return the cursor descriptor, or null if not found
     */
    public static SqlCursor getCursor(String name) {
        return cursors.get(name);
    }

    /**
     * Remove a cursor by name.
     *
     * @param name the cursor name
     */
    public static void removeCursor(String name) {
        cursors.remove(name);
    }

    /**
     * Register a prepared statement with its query and parameter count.
     *
     * @param name the statement name
     * @param query the SQL query string
     * @param nParams the number of parameters
     */
    public static void addPrepared(String name, String query, int nParams) {
        preparedStatements.put(name, new String[] {query, String.valueOf(nParams)});
    }

    /**
     * Look up a prepared statement by name.
     *
     * @param name the statement name
     * @return a two-element array [query, nParams], or null if not found
     */
    public static String[] getPrepared(String name) {
        return preparedStatements.get(name);
    }

    /** Mark all cursors as closed (e.g. after COMMIT or ROLLBACK). */
    public static void clearCursors() {
        for (SqlCursor cursor : cursors.values()) {
            cursor.isOpened = false;
        }
    }
}

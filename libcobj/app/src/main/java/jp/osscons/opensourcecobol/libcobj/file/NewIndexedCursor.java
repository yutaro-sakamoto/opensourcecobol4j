package jp.osscons.opensourcecobol.libcobj.file;

import java.sql.Connection;
import java.util.Optional;

final class NewIndexedCursor {
    private final Connection conn;
    private byte[] key;
    private final int tableIndex;
    private final boolean isDuplicate;
    private int comparator;

    private NewIndexedCursor(
            Connection conn, byte[] key, int tableIndex, boolean isDuplicate, int comparator) {
        this.conn = conn;
        this.key = key;
        this.tableIndex = tableIndex;
        this.isDuplicate = isDuplicate;
        this.comparator = comparator;
    }

    static Optional<NewIndexedCursor> createCursor(
            Connection conn, byte[] key, int tableIndex, boolean isDuplicate, int comparator) {
        return Optional.of(new NewIndexedCursor(conn, key, tableIndex, isDuplicate, comparator));
    }

    static boolean matchKeyHead(byte[] originalKey, byte[] fetchKey) {
        if (originalKey.length > fetchKey.length) {
            return false;
        }
        for (int i = 0; i < originalKey.length; ++i) {
            if (originalKey[i] != fetchKey[i]) {
                return false;
            }
        }
        return true;
    }

    Optional<NewIndexedCursor> reloadCursor() {
        return Optional.of(this);
    }

    void close() {
        return;
    }

    Optional<FetchResult> read(CursorReadOption opt) {
        if (opt == CursorReadOption.NEXT) {
            return this.next();
        } else if (opt == CursorReadOption.PREV) {
            return this.prev();
        } else {
            return Optional.empty();
        }
    }

    Optional<FetchResult> next() {
        return Optional.empty();
    }

    Optional<FetchResult> prev() {
        return Optional.empty();
    }

    boolean moveToFirst() {
        return true;
    }

    boolean moveToLast() {
        return true;
    }
}

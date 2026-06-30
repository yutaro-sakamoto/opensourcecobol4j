package jp.osscons.opensourcecobol.libcobj.sql;

import java.sql.Connection;
import java.sql.ParameterMetaData;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/**
 * PostgreSQL 向けの {@link CobolSqlBackend} 実装。{@link AbstractCobolSqlBackend} の DB 依存フックを
 * PostgreSQL の流儀（明示 BEGIN によるトランザクション管理、サーバカーソル + {@code FETCH FORWARD}
 * 先読み、SQLSTATE ベースのエラー変換）で実装する。挙動は移行前の {@code CobolSql} と一致させている。
 */
final class CobolSqlPostgresql extends AbstractCobolSqlBackend {

    @Override
    public String id() {
        return "postgresql";
    }

    // -------------------------------------------------------
    // 接続・トランザクション
    // -------------------------------------------------------

    @Override
    protected String buildJdbcUrl(DbSpec s) {
        return "jdbc:postgresql://" + s.host + s.port + "/" + s.dbname;
    }

    @Override
    protected void configureConnection(Connection c) throws SQLException {
        // PostgreSQL は autoCommit(true) のまま明示 BEGIN で TX を管理する（移行前の挙動）。
        c.setAutoCommit(true);
        beginTransaction(c);
    }

    @Override
    protected void beginTransaction(Connection c) throws SQLException {
        if (c != null && !c.isClosed()) {
            try (Statement stmt = c.createStatement()) {
                stmt.execute("BEGIN");
            }
        }
    }

    @Override
    protected void commitTransaction(Connection c) throws SQLException {
        // PostgreSQL は autoCommit(true) + 明示 BEGIN/COMMIT で TX を管理するため、COMMIT を発行する。
        // 再 BEGIN は基底側（doCommit）/切断フローが beginTransaction で制御する。
        try (Statement stmt = c.createStatement()) {
            stmt.execute("COMMIT");
        }
    }

    @Override
    protected void rollbackTransaction(Connection c) throws SQLException {
        try (Statement stmt = c.createStatement()) {
            stmt.execute("ROLLBACK");
        }
    }

    // -------------------------------------------------------
    // カーソル
    // -------------------------------------------------------

    @Override
    protected void openCursorImpl(Connection c, Cursor cur, AbstractCobolField[] params)
            throws SQLException {
        String command = "DECLARE " + cur.name + " CURSOR FOR " + cur.query;

        // OPEN ... USING (params) を最優先し、なければ DECLARE 時に保存した cur.params に
        // フォールバック。どちらも空ならパラメータなしで Statement を実行する。
        AbstractCobolField[] bindParams;
        if (params != null && params.length > 0) {
            bindParams = params;
        } else if (cur.params != null && cur.params.length > 0) {
            bindParams = cur.params;
        } else {
            bindParams = null;
        }

        if (bindParams != null) {
            try (PreparedStatement pstmt = c.prepareStatement(command)) {
                ParameterMetaData metaData = pstmt.getParameterMetaData();
                for (int i = 0; i < bindParams.length; i++) {
                    CobolDataConverter.setParam(pstmt, i + 1, metaData, bindParams[i]);
                }
                pstmt.execute();
            }
        } else {
            try (Statement stmt = c.createStatement()) {
                stmt.execute(command);
            }
        }
    }

    @Override
    protected boolean fetchRowImpl(
            Connection c, Cursor cur, AbstractCobolField[] out, CobolDataStorage sqlca)
            throws SQLException {
        // バッファを使い切っていれば、OCESQL4J_FETCH_RECORDS 件をまとめて先読みする。
        if (cur.bufferPos >= cur.fetchBuffer.size()) {
            refill(c, cur);
        }
        if (cur.bufferPos >= cur.fetchBuffer.size()) {
            // 先読みしても行が無い＝これ以上の行は無い。
            return false;
        }

        byte[][] row = cur.fetchBuffer.get(cur.bufferPos);
        cur.bufferPos++;
        if (out != null) {
            boolean sawNullWithoutIndicator = false;
            for (int i = 0; i < out.length && i < row.length; i++) {
                byte[] value = row[i];
                if (value != null) {
                    CobolDataConverter.stringToCobol(out[i], value);
                } else {
                    // 指標変数なしの NULL は型に応じた空値で埋める。空バイト列を型対応の
                    // 変換に渡すと、数値=0/英数字=空白/national=全角空白になる。raw な
                    // memset 0 は packed の符号ニブルやゾーン10進で不正表現になるため避ける。
                    CobolDataConverter.stringToCobol(out[i], EMPTY_RESULT);
                    sawNullWithoutIndicator = true;
                }
            }
            if (sawNullWithoutIndicator) {
                SqlCA.setMissingIndicator(sqlca);
            }
        }
        return true;
    }

    /**
     * {@code FETCH FORWARD <OCESQL4J_FETCH_RECORDS> FROM name} を実行し、返った全行を
     * 先読みバッファへ格納する。
     */
    private void refill(Connection c, Cursor cur) throws SQLException {
        cur.fetchBuffer = new java.util.ArrayList<>();
        cur.bufferPos = 0;
        int fetchRecords = BulkFetchConfig.getFetchRecords();
        String fetchSql = "FETCH FORWARD " + fetchRecords + " FROM " + cur.name;
        try (Statement stmt = c.createStatement()) {
            boolean hasResult = stmt.execute(fetchSql);
            if (!hasResult) {
                cur.overFetch = false;
                return;
            }
            ResultSet rs = stmt.getResultSet();
            if (rs != null) {
                int columnCount = rs.getMetaData().getColumnCount();
                while (rs.next()) {
                    byte[][] row = new byte[columnCount][];
                    for (int i = 0; i < columnCount; i++) {
                        row[i] = CobolDataConverter.getValueFromResultSet(rs, i + 1);
                    }
                    cur.fetchBuffer.add(row);
                }
                rs.close();
            }
            int size = cur.fetchBuffer.size();
            // 要求件数より少ない行数しか取れなかった＝結果末尾に達した（サーバカーソルは末尾の先）。
            cur.overFetch = size > 0 && size < fetchRecords;
        }
    }

    @Override
    protected void fetchOccursImpl(
            Connection c,
            Cursor cur,
            int occursSize,
            int occursMax,
            AbstractCobolField[] resultParams,
            CobolDataStorage sqlca)
            throws SQLException {
        // 登録済みだが未 OPEN のカーソルでも短絡せず FETCH を PostgreSQL へ送り、
        // そのエラー (メッセージ・SQLSTATE) を SQLCA に反映させる (Open COBOL ESQL 4J と同じ)。
        String fetchSql = "FETCH FORWARD " + occursMax + " FROM " + cur.name;
        try (Statement stmt = c.createStatement();
                ResultSet rs = stmt.executeQuery(fetchSql)) {
            if (!rs.next()) {
                SqlCA.setSuccess(sqlca);
                return;
            }
            int rowCount = 0;
            boolean sawNullWithoutIndicator = false;
            do {
                if (rowCount >= occursMax) {
                    break;
                }
                int storageOffset = rowCount * occursSize;
                for (int i = 0; i < resultParams.length; i++) {
                    byte[] value = CobolDataConverter.getValueFromResultSet(rs, i + 1);
                    CobolDataStorage fieldStorage =
                            resultParams[i].getDataStorage().getSubDataStorage(storageOffset);
                    int fieldSize = resultParams[i].getSize();
                    if (value != null) {
                        CobolDataConverter.stringToCobolRaw(
                                resultParams[i], fieldStorage, fieldSize, value);
                    } else {
                        // 指標変数なしの NULL は型に応じた空値で埋める (memset 0 は
                        // packed/ゾーン10進で不正表現になる)。
                        CobolDataConverter.stringToCobolRaw(
                                resultParams[i], fieldStorage, fieldSize, EMPTY_RESULT);
                        sawNullWithoutIndicator = true;
                    }
                }
                rowCount++;
            } while (rs.next());
            SqlCA.setRowCount(sqlca, rowCount);
            if (sawNullWithoutIndicator) {
                SqlCA.setMissingIndicator(sqlca);
            } else {
                SqlCA.setSuccess(sqlca);
            }
        }
    }

    @Override
    protected void closeCursorImpl(Connection c, Cursor cur) throws SQLException {
        try (Statement stmt = c.createStatement()) {
            stmt.execute("CLOSE " + cur.name);
        }
    }

    @Override
    protected void repositionForCurrentOf(Connection c, Cursor cur, CobolDataStorage sqlca)
            throws SQLException {
        // 先読みバッファに未供給行が remaining 行残っていれば、さらに overFetch（結果末尾に達し
        // サーバカーソルが末尾の先にある）なら +1 行ぶん FETCH BACKWARD してから、バッファを破棄する。
        int backward = cur.remainingBuffered() + (cur.overFetch ? 1 : 0);
        if (backward > 0) {
            try (Statement stmt = c.createStatement()) {
                stmt.execute("FETCH BACKWARD " + backward + " FROM " + cur.name);
            }
        }
        // 位置補正後は先読みバッファを無効化し、次の FETCH は補正後の位置から取り直す。
        cur.clearBuffer();
    }

    // -------------------------------------------------------
    // エラー変換
    // -------------------------------------------------------

    @Override
    protected int sqlStateToCode(SQLException e) {
        // SQLSTATE → ECPG コードの変換表。エラー変換は backend 実装に閉じ込め、別クラスへは切り出さない。
        String sqlState = e.getSQLState();
        if (sqlState == null) {
            return SqlCA.ECPG_UNKNOWN_ERROR;
        }
        switch (sqlState) {
            case "00000":
                return SqlCA.ECPG_NO_ERROR;
            case "02000":
                return SqlCA.ECPG_NOT_FOUND;
            case "YE002":
                return SqlCA.ECPG_EMPTY;
            case "08001":
            case "08003":
                return SqlCA.ECPG_CONNECT;
            case "08007":
                return SqlCA.ECPG_TRANS;
            case "21000":
                return SqlCA.ECPG_SUBSELECT_NOT_ONE;
            case "23505":
                return SqlCA.ECPG_DUPLICATE_KEY;
            case "25001":
                return SqlCA.ECPG_WARNING_IN_TRANSACTION;
            case "25P01":
                return SqlCA.ECPG_WARNING_NO_TRANSACTION;
            case "34000":
                return SqlCA.ECPG_WARNING_UNKNOWN_PORTAL;
            case "42804":
                return SqlCA.ECPG_DATA_FORMAT_ERROR;
            case "42P03":
                return SqlCA.ECPG_WARNING_PORTAL_EXISTS;
            case "55P03":
                return SqlCA.ECPG_PGSQL;
            default:
                return SqlCA.ECPG_UNKNOWN_ERROR;
        }
    }
}

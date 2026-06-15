package jp.osscons.opensourcecobol.libcobj.sql;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/** COBOL の埋め込み SQL においてクエリ結果を反復処理するための SQL カーソルを表す。 */
class SqlCursor {

    /** DECLARE/OPEN/FETCH/CLOSE 文で使用されるカーソル名。 */
    String name;

    /** このカーソルに紐づく SQL クエリ。 */
    String query;

    /** クエリ中のホスト変数パラメータの数。 */
    int nParams;

    /** このカーソルが現在オープンされているかどうか。 */
    boolean isOpened;

    /** DECLARE 時にバインドされたホスト変数パラメータ。 */
    AbstractCobolField[] params;

    /**
     * 先読み（バルクフェッチ）バッファ。各要素は 1 行ぶんの列値配列で、SQL NULL の列は null。
     * {@code FETCH FORWARD N} で取得した行をここに溜め、{@link #fetch} 呼び出しごとに 1 行ずつ供給する。
     */
    private List<byte[][]> fetchBuffer = new ArrayList<>();

    /** {@link #fetchBuffer} 内で次に供給する行の位置。 */
    private int bufferPos;

    /**
     * 直近の先読みが要求件数より少ない行数で終わった（＝結果末尾に達した）かどうか。
     * WHERE CURRENT OF のカーソル位置補正に使う（Open COBOL ESQL 4J の overFetch 相当）。
     */
    boolean overFetch;

    /**
     * 新しいカーソル記述子を生成する。
     *
     * @param name カーソル名
     * @param query このカーソル用の SQL クエリ
     * @param nParams ホスト変数パラメータの数
     */
    SqlCursor(String name, String query, int nParams) {
        this.name = name;
        this.query = query;
        this.nParams = nParams;
        this.isOpened = false;
        this.params = null;
    }

    /**
     * DECLARE CURSOR 文を実行してこのカーソルをオープンする。
     *
     * @param conn JDBC コネクション
     * @param openParams クエリ用のホスト変数パラメータ。保存済みパラメータを使用する場合は null
     * @throws SQLException データベースアクセスエラーが発生した場合
     */
    void open(Connection conn, AbstractCobolField[] openParams) throws SQLException {
        String command = "DECLARE " + name + " CURSOR FOR " + query;

        // OPEN ... USING (openParams) を最優先し、なければ DECLARE 時に
        // 保存した this.params にフォールバック。どちらも空ならパラメータ
        // なしで Statement を実行する。
        AbstractCobolField[] bindParams;
        if (openParams != null && openParams.length > 0) {
            bindParams = openParams;
        } else if (this.params != null && this.params.length > 0) {
            bindParams = this.params;
        } else {
            bindParams = null;
        }

        if (bindParams != null) {
            try (PreparedStatement pstmt = conn.prepareStatement(command)) {
                java.sql.ParameterMetaData metaData = pstmt.getParameterMetaData();
                for (int i = 0; i < bindParams.length; i++) {
                    CobolDataConverter.setParam(pstmt, i + 1, metaData, bindParams[i]);
                }
                pstmt.execute();
            }
        } else {
            try (Statement stmt = conn.createStatement()) {
                stmt.execute(command);
            }
        }
        isOpened = true;
        // 新たにオープンしたカーソルは先読みバッファを空から始める。
        clearBuffer();
    }

    /** 先読みバッファと overFetch フラグをリセットする。 */
    void clearBuffer() {
        fetchBuffer = new ArrayList<>();
        bufferPos = 0;
        overFetch = false;
    }

    /**
     * まだ COBOL 側へ供給していない（バッファに残っている）先読み行数を返す。
     * WHERE CURRENT OF のカーソル位置補正に使う。
     *
     * @return 未供給の先読み行数
     */
    int remainingBuffered() {
        return fetchBuffer.size() - bufferPos;
    }

    /**
     * このカーソルから次の行をフェッチし、結果を COBOL のホスト変数へ書き込む。
     *
     * <p>指標変数がサポートされていない状況でいずれかの列が SQL NULL として返された場合、
     * {@code sqlca} を ECPG_MISSING_INDICATOR (sqlcode=-213, sqlstate="22002") に設定する。
     * 行データは（NULL の列はゼロ埋めで）書き込まれ、「行はフェッチされたがフラグが立てられた」
     * という ECPG の挙動に一致する。
     *
     * @param conn JDBC コネクション
     * @param resultParams 列の値を受け取る出力用ホスト変数
     * @param sqlca 指標変数なしの NULL をフラグするための SQLCA ストレージ（null でもよい）
     * @return 行がフェッチされた場合は true、これ以上行がない場合は false
     * @throws SQLException データベースアクセスエラーが発生した場合
     */
    boolean fetch(Connection conn, AbstractCobolField[] resultParams, CobolDataStorage sqlca)
            throws SQLException {
        // バッファを使い切っていれば、OCESQL4J_FETCH_RECORDS 件をまとめて先読みする。
        if (bufferPos >= fetchBuffer.size()) {
            refill(conn);
        }
        if (bufferPos >= fetchBuffer.size()) {
            // 先読みしても行が無い＝これ以上の行は無い。
            return false;
        }

        byte[][] row = fetchBuffer.get(bufferPos);
        bufferPos++;
        if (resultParams != null) {
            boolean sawNullWithoutIndicator = false;
            for (int i = 0; i < resultParams.length && i < row.length; i++) {
                byte[] value = row[i];
                if (value != null) {
                    CobolDataConverter.stringToCobol(resultParams[i], value);
                } else {
                    resultParams[i].getDataStorage().memset((byte) 0, resultParams[i].getSize());
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
     * 先読みバッファへ格納する。各列の値は {@link CobolDataConverter#getValueFromResultSet} で
     * 取得し（SQL NULL は null）、{@code fetch} 呼び出しごとに 1 行ずつ供給する。結果セットの
     * 全列を格納し、ホスト変数への振り分けは {@link #fetch} 側で行う。
     */
    private void refill(Connection conn) throws SQLException {
        fetchBuffer = new ArrayList<>();
        bufferPos = 0;
        int fetchRecords = BulkFetchConfig.getFetchRecords();
        String fetchSql = "FETCH FORWARD " + fetchRecords + " FROM " + name;
        try (Statement stmt = conn.createStatement()) {
            boolean hasResult = stmt.execute(fetchSql);
            if (!hasResult) {
                overFetch = false;
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
                    fetchBuffer.add(row);
                }
                rs.close();
            }
            int size = fetchBuffer.size();
            // 要求件数より少ない行数しか取れなかった＝結果末尾に達した（サーバカーソルは末尾の先）。
            overFetch = size > 0 && size < fetchRecords;
        }
    }

    /**
     * CLOSE 文を実行してこのカーソルをクローズする。
     *
     * @param conn JDBC コネクション
     * @throws SQLException データベースアクセスエラーが発生した場合
     */
    void close(Connection conn) throws SQLException {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("CLOSE " + name);
        }
        isOpened = false;
        clearBuffer();
    }
}

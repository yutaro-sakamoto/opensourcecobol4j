package jp.osscons.opensourcecobol.libcobj.sql;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
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
        String fetchSql = "FETCH FORWARD 1 FROM " + name;
        try (Statement stmt = conn.createStatement()) {
            boolean hasResult = stmt.execute(fetchSql);
            if (!hasResult) {
                return false;
            }
            ResultSet rs = stmt.getResultSet();
            if (rs == null || !rs.next()) {
                if (rs != null) {
                    rs.close();
                }
                return false;
            }

            if (resultParams != null) {
                int columnCount = rs.getMetaData().getColumnCount();
                boolean sawNullWithoutIndicator = false;
                for (int i = 0; i < resultParams.length && i < columnCount; i++) {
                    byte[] value = CobolDataConverter.getValueFromResultSet(rs, i + 1);
                    if (value != null) {
                        CobolDataConverter.stringToCobol(resultParams[i], value);
                    } else {
                        resultParams[i]
                                .getDataStorage()
                                .memset((byte) 0, resultParams[i].getSize());
                        sawNullWithoutIndicator = true;
                    }
                }
                if (sawNullWithoutIndicator) {
                    SqlCA.setMissingIndicator(sqlca);
                }
            }
            rs.close();
            return true;
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
    }
}

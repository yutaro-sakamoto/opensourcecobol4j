package jp.osscons.opensourcecobol.libcobj.sql;

import java.sql.SQLException;

/**
 * {@link AbstractCobolEsqlBackend#mapSqlException(SQLException)} の戻り値。DB の生エラーを、COBOL 側が見る
 * ECPG 正規コード（SQLCODE）と正規化済み SQLSTATE の組へ変換した結果を保持する。
 */
public final class SqlErrorMapping {
    /** ECPG 互換の正規エラーコード（SQLCODE に書かれる）。 */
    final int sqlCode;

    /** 正規化済み SQLSTATE（5 桁、SQLSTATE に書かれる）。{@code null} のとき共通側で空白を補う。 */
    final String sqlState;

    /**
     * 正規エラーコードと正規化済み SQLSTATE の組を生成する。
     *
     * @param sqlCode ECPG 互換の正規エラーコード（SQLCODE）
     * @param sqlState 正規化済み SQLSTATE（5 桁、{@code null} 可）
     */
    public SqlErrorMapping(int sqlCode, String sqlState) {
        this.sqlCode = sqlCode;
        this.sqlState = sqlState;
    }

    /**
     * 正規エラーコード（SQLCODE）を返す。
     *
     * @return ECPG 互換の正規エラーコード
     */
    public int getSqlCode() {
        return sqlCode;
    }

    /**
     * 正規化済み SQLSTATE を返す。
     *
     * @return 正規化済み SQLSTATE（5 桁）。未設定なら {@code null}
     */
    public String getSqlState() {
        return sqlState;
    }
}

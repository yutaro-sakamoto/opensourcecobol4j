package jp.osscons.opensourcecobol.libcobj.sql;

import java.util.Locale;

/**
 * 環境変数 {@code OCDB_DB_TYPE} を見て {@link CobolSqlBackend} 実装を 1 つ生成する Factory。
 *
 * <p>現状サポートするのは PostgreSQL のみで、未指定（null/空）の場合も後方互換のため PostgreSQL を
 * 返す。新しい DB を追加する際は、対応する {@link CobolSqlBackend} 実装を {@link #resolve(String)} の
 * 分岐に追加するだけでよい（コア側の他クラスは変更不要）。
 */
final class CobolSqlFactory {

    /** ユーティリティクラスのインスタンス化を防ぐための private コンストラクタ。 */
    private CobolSqlFactory() {}

    /**
     * 環境変数 {@code OCDB_DB_TYPE} からバックエンドを解決する。
     *
     * @return 解決された DB バックエンド
     */
    static CobolSqlBackend resolve() {
        return resolve(System.getenv("OCDB_DB_TYPE"));
    }

    /**
     * 環境変数値からバックエンドを解決する純粋関数（環境変数に依存しないテスト seam）。
     * 未指定（null/空）の場合は後方互換のため PostgreSQL を返す。
     *
     * @param t DB 種別文字列（{@code OCDB_DB_TYPE} の値）
     * @return 解決された DB バックエンド
     */
    static CobolSqlBackend resolve(String t) {
        if (t == null || t.isEmpty()) {
            return new CobolSqlPostgresql(); // 後方互換
        }
        switch (t.toLowerCase(Locale.ROOT)) {
            case "postgresql":
            case "postgres":
                return new CobolSqlPostgresql();
            default:
                throw new IllegalArgumentException("Unsupported OCDB_DB_TYPE: " + t);
        }
    }
}

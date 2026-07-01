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
            t = "postgresql"; // 後方互換：未指定は PostgreSQL
        }
        String className = backendClassName(t);
        try {
            Class<?> c = Class.forName(className);
            return (CobolSqlBackend) c.getDeclaredConstructor().newInstance();
        } catch (ReflectiveOperationException e) { // 実装クラスが無い/生成できない
            throw new IllegalArgumentException("Unsupported OCDB_DB_TYPE: " + t, e);
        }
    }

    /** バックエンド実装クラスが属するパッケージ。 */
    private static final String BACKEND_PACKAGE = "jp.osscons.opensourcecobol.libcobj.sql";

    /**
     * {@code OCDB_DB_TYPE} の値から、バックエンド実装の完全修飾クラス名を規約で組み立てる。
     *
     * <p>規約：値を小文字化し先頭 1 文字を大文字にして {@code CobolSql} に連結する。
     * 例）{@code "db2"} → {@code CobolSqlDb2} / {@code "postgresql"} → {@code CobolSqlPostgresql}。
     * これにより新しい DB を追加してもこの Factory は無変更で、環境変数値と実装クラス名を
     * 規約どおりに揃えるだけで動的ロードできる。Db2/Oracle など有償拡張が別 JAR で提供され
     * コンパイル時参照を持てないケースにも、リフレクションロードで一様に対応する（設計メモ §6/§12）。
     *
     * @param t DB 種別文字列（{@code OCDB_DB_TYPE} の値。null/空でないこと）
     * @return バックエンド実装の完全修飾クラス名
     */
    private static String backendClassName(String t) {
        String type = t.toLowerCase(Locale.ROOT);
        return BACKEND_PACKAGE + ".CobolSql"
                + Character.toUpperCase(type.charAt(0)) + type.substring(1);
    }
}

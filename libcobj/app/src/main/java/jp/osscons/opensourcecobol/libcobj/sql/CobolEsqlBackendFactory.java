package jp.osscons.opensourcecobol.libcobj.sql;

import java.util.Locale;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * 環境変数 {@code OCDB_DB_TYPE} を見て {@link CobolEsqlBackendInterface} 実装を 1 つ生成する Factory。
 *
 * <p>現状サポートするのは PostgreSQL のみで、未指定（null/空）の場合も後方互換のため PostgreSQL を
 * 返す。新しい DB を追加する際は、対応する {@link CobolEsqlBackendInterface} 実装を {@link #resolve(String)} の
 * 分岐に追加するだけでよい（コア側の他クラスは変更不要）。
 */
final class CobolEsqlBackendFactory {

    private static final Logger LOG = LoggerFactory.getLogger(CobolEsqlBackendFactory.class);

    /** ユーティリティクラスのインスタンス化を防ぐための private コンストラクタ。 */
    private CobolEsqlBackendFactory() {}

    /**
     * 環境変数 {@code OCDB_DB_TYPE} からバックエンドを解決する。
     *
     * @return 解決された DB バックエンド
     * @throws ClassNotFoundException {@code OCDB_DB_TYPE} に対応する実装クラスが無い/生成できない場合
     */
    static CobolEsqlBackendInterface resolve() throws ClassNotFoundException {
        return resolve(System.getenv("OCDB_DB_TYPE"));
    }

    /**
     * 環境変数値からバックエンドを解決する純粋関数（環境変数に依存しないテスト seam）。
     * 未指定（null/空）の場合は後方互換のため PostgreSQL を返す。
     *
     * @param t DB 種別文字列（{@code OCDB_DB_TYPE} の値）
     * @return 解決された DB バックエンド
     * @throws ClassNotFoundException {@code t} に対応する実装クラスが無い/生成できない場合
     */
    static CobolEsqlBackendInterface resolve(String t) throws ClassNotFoundException {
        String type = (t == null || t.isEmpty()) ? "postgresql" : t.toLowerCase(Locale.ROOT);
        if ("postgres".equals(type)) {
            type = "postgresql"; // 後方互換エイリアス
        }
        String className = backendClassName(type);
        try {
            Class<?> c = Class.forName(className);
            return (CobolEsqlBackendInterface) c.getDeclaredConstructor().newInstance();
        } catch (ReflectiveOperationException e) { // 実装クラスが無い/生成できない
            LOG.error("Unsupported OCDB_DB_TYPE: {}", t, e);
            throw new ClassNotFoundException("Unsupported OCDB_DB_TYPE: " + t, e);
        }
    }

    /** バックエンド実装クラスが属するパッケージ。 */
    private static final String BACKEND_PACKAGE = "jp.osscons.opensourcecobol.libcobj.sql";

    /**
     * 小文字化済みの DB 種別文字列から、バックエンド実装の完全修飾クラス名を規約で組み立てる。
     *
     * <p>規約：先頭 1 文字を大文字にして {@code CobolEsqlBackend} に連結する。
     * 例）{@code "db2"} → {@code CobolEsqlBackendDb2} / {@code "postgresql"} → {@code CobolEsqlBackendPostgresql}。
     * これにより新しい DB を追加してもこの Factory は無変更で、環境変数値と実装クラス名を
     * 規約どおりに揃えるだけで動的ロードできる。Db2/Oracle など有償拡張が別 JAR で提供され
     * コンパイル時参照を持てないケースにも、リフレクションロードで一様に対応する（設計メモ §6/§12）。
     *
     * @param type 小文字化・正規化済みの DB 種別文字列（null/空でないこと）
     * @return バックエンド実装の完全修飾クラス名
     */
    private static String backendClassName(String type) {
        return BACKEND_PACKAGE
                + ".CobolEsqlBackend"
                + Character.toUpperCase(type.charAt(0))
                + type.substring(1);
    }
}

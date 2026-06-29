package jp.osscons.opensourcecobol.libcobj.sql;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

/** {@link CobolSqlFactory} の env 非依存な解決ロジック（resolve(String)）を検証する。 */
class CobolSqlFactoryTest {

    @Test
    void testResolve_Null_DefaultsToPostgresql() {
        assertInstanceOf(CobolSqlPostgresql.class, CobolSqlFactory.resolve((String) null));
    }

    @Test
    void testResolve_Empty_DefaultsToPostgresql() {
        assertInstanceOf(CobolSqlPostgresql.class, CobolSqlFactory.resolve(""));
    }

    @Test
    void testResolve_Postgresql() {
        assertInstanceOf(CobolSqlPostgresql.class, CobolSqlFactory.resolve("postgresql"));
    }

    @Test
    void testResolve_PostgresAlias() {
        assertInstanceOf(CobolSqlPostgresql.class, CobolSqlFactory.resolve("postgres"));
    }

    @Test
    void testResolve_CaseInsensitive() {
        assertInstanceOf(CobolSqlPostgresql.class, CobolSqlFactory.resolve("PostgreSQL"));
    }

    @Test
    void testResolve_Unsupported_Throws() {
        assertThrows(IllegalArgumentException.class, () -> CobolSqlFactory.resolve("mysql"));
    }
}

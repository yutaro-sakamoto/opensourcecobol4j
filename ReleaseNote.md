## New Features

- **Embedded SQL (EXEC SQL) support for PostgreSQL** — a full port of **Open COBOL ESQL 4J** into `cobj` and `libcobj.jar`. COBOL programs can now run `EXEC SQL ... END-EXEC` statements directly, with no separate precompiler or runtime. (#847)
  - Supports `CONNECT`/`DISCONNECT`, `INSERT`/`UPDATE`/`DELETE`/`SELECT INTO`, cursors (`DECLARE`/`OPEN`/`FETCH`/`CLOSE`, including `WHERE CURRENT OF`), dynamic SQL (`PREPARE`/`EXECUTE`), and `COMMIT`/`ROLLBACK`.
  - Host variables, the SQLCA, packed/binary/national/Japanese data types, configurable bulk fetch, and prepared-statement caching.
  - The generated Java is dramatically simpler and more readable than before: each `EXEC SQL` statement compiles to a single high-level runtime call passing host variables as a typed array, instead of the verbose low-level helper calls of the original implementation.
  - See the [ESQL Guide](./doc/esql-guide.md) and [examples](./example/esql/).

## Documentation

- Improved Javadoc comments for the `data`, `common`, and `user_util` modules of libcobj. (#846, #841, #836)

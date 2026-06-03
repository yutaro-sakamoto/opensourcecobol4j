## Overview

This document describes how Embedded SQL (`EXEC SQL ... END-EXEC`) support is wired into opensource COBOL 4J. For end-user usage, see [esql-guide.md](./esql-guide.md).

ESQL support is split into two layers:

1. **Compiler layer (`cobj/`)**: lexes/parses `EXEC SQL` blocks, lands the result on the same AST as ordinary COBOL, and emits Java code that passes host variables to the runtime as `AbstractCobolField` arguments.
2. **Runtime layer (`libcobj/.../sql/`)**: called from the generated Java; drives JDBC (PostgreSQL) and writes results back into COBOL storage.

## Compiler layer

### Key files

| File | Role |
|---|---|
| `cobj/esql-scanner.l` | flex lexer. Tokenizes the SQL text between `EXEC SQL` and `END-EXEC`. |
| `cobj/esql-parser.y` | bison parser. Builds host-variable lists and a `cb_exec_sql` node. |
| `cobj/esql-common.h` | Shared types and prototypes between `esql-parser.y` / `esql-scanner.l` and `esql.c`. Deliberately does NOT include `tree.h` (its `YYSTYPE` would collide with the ESQL parser's). |
| `cobj/esql.c` | `esql_build_and_resolve()` lives here, along with host-var type resolution, GROUP expansion for SELECT INTO / FETCH OCCURS, and thin wrappers that build `cb_tree`s without exposing `tree.h` to the ESQL parser. |
| `cobj/codegen.c` (around `joutput_exec_sql`) | Expands a `cb_exec_sql` node into Java calls like `CobolSql.idExec(...)`. |
| `cobj/typeck.c` | Auto-injects `01 SQLCA GLOBAL.` into programs that contain `EXEC SQL`. |

### Parsing pipeline

```
EXEC SQL ... END-EXEC (COBOL source)
        │
        │  The COBOL preprocessor (cobj/pplex.l, cobj/ppparse.y)
        │  captures EXEC SQL ... END-EXEC as a single string.
        ▼
esql-scanner.l (flex)
        │
        │  Builds the SQL body, replacing host vars with '?'.
        │  Host vars, subscripts, and dotted qualifications are
        │  returned to the parser as ESQL_HOSTTOKEN / HOSTSUB_*.
        ▼
esql-parser.y (bison)
        │
        │  Assembles a cb_sql_host_var list, cursor name, and SQL
        │  body, then calls esql_build_and_resolve() in esql.c.
        ▼
esql.c
        │
        │  - resolve_host_var_type(): maps COBOL PIC / USAGE to
        │    HVARTYPE_*.
        │  - expand_group_host_vars(): if SELECT INTO got a GROUP,
        │    expands it into per-child bindings.
        │  - OCCURS detection: if the leaf has flag_occurs, promote
        │    to SELECT_INTO_OCCURS / FETCH_OCCURS.
        │  cb_build_exec_sql() returns the cb_exec_sql node.
        ▼
typeck.c / codegen.c
        │
        │  The cb_exec_sql node rides the same type-check / codegen
        │  path as any other COBOL statement. Each cb_reference in
        │  host_list / res_host_list goes through joutput_param,
        │  yielding AbstractCobolField arguments that include
        │  b_X.getSubDataStorage(...) for subscripted hosts.
        ▼
Java source (CobolSql.idExec / .idSelectInto / .idFetchCursor / ...)
```

### Host variable AST representation

ESQL host variables introduce no new AST node type. They ride on the standard COBOL `cb_reference`.

| Source form | Internal representation |
|---|---|
| `:VAR` | `cb_reference{ word: VAR }` |
| `:VAR(IDX)` | `cb_reference{ word: VAR, subs: [IDX] }` |
| `:VAR(I, J)` | `cb_reference{ word: VAR, subs: [J, I] }` (leaf→root order) |
| `:GRP.SUB` | `cb_reference{ word: SUB, chain: cb_reference{ word: GRP } }` |
| `:GRP.SUB(IDX)` | above + `subs: [IDX]` |
| `:GRP.SUB(GRP2.IDX)` | above + `subs: [cb_reference{ word: IDX, chain: GRP2 }]` |

As a result, the existing `joutput_param` / `joutput_data` in `codegen.c` (specifically the OCCURS resolution loop at `cobj/codegen.c:972`) produce correct `AbstractCobolField` arguments — including `b_X.getSubDataStorage(...)` — **without any ESQL-specific code path**.

### Scanner states

`esql-scanner.l` uses these exclusive states:

- `INITIAL` -- waiting for the first SQL keyword (`SELECT` / `INSERT` / `CONNECT` / ...).
- `ESQL_STATE` -- reading SQL body. Whitespace, comments, and tokens are appended to `esql_sqlbody`; host variables are replaced with `?`.
- `ESQL_HOSTSUB_STATE` -- pushed when a host token is immediately followed by `(`. Returns `(`, `)`, `,`, integer literals, and dotted identifiers as tokens **without writing them to `esql_sqlbody`**. Popped on `)`.
- `ESQL_DBNAME_STATE` -- captures `:NAME` after the `AT` keyword.
- `WHERE_CURRENT_OF` -- captures the cursor name after `WHERE CURRENT OF`.

The `HOSTWORD` regex includes dotted qualification (`("\."IDENT)*`). The `HOSTWORD/"("` trailing context decides whether to push `HOSTSUB_STATE`: it does so only when `(` follows directly.

### Subscript side channel `esql_pending_subs`

Subscript lists are passed between grammar actions via a static `cb_tree` variable, `esql_pending_subs`. The action that reduces `host_reference` stores the (reversed) subscript list there, and the very next parent rule's action — `esql_add_host_var()` — consumes and clears it. Because every `host_reference` reduction is followed immediately by exactly one consumer, no two host variables can collide. This is a small price paid to avoid widening the parser's `%union`.

### Building qualified references

`esql_build_qualified_ref()` in `cobj/esql.c` splits a dotted name like `"GRP.SUB.X"` with `strtok`, takes the rightmost segment as the leaf, and links the rest with `chain` (leaf → parent → grandparent). The resulting AST is identical in shape to plain COBOL `X OF SUB OF GRP`, so `cb_ref` resolves it through the same path.

### OCCURS handling

For `SELECT INTO` / `FETCH`, `esql_build_and_resolve()` checks whether the leaf host variable carries `flag_occurs`. If so, it promotes the command to `CB_SQL_SELECT_INTO_OCCURS` or `CB_SQL_FETCH_OCCURS` and records `occurs_size` (bytes per element) and `occurs_max` (element count) on the `cb_exec_sql` node. The runtime uses both numbers to scatter result rows into a contiguous COBOL array.

`expand_group_host_vars()` is responsible for the case where the caller passed a GROUP as a SELECT INTO target: the GROUP's children are expanded into individual host-variable bindings, which lets PostgreSQL see an N-column SELECT.

## Runtime layer

### Package layout (`jp.osscons.opensourcecobol.libcobj.sql`)

| Class | Visibility | Role |
|---|---|---|
| `CobolSql` | `public` | The single public API called from generated Java. Provides `idConnect`, `idExec`, `idExecParams`, `idSelectInto`, `idDeclareCursor`, `idOpenCursor`, `idFetchCursor`, `idCloseCursor`, `idPrepare`, `idExecPrepared`, `idCommit`, `idRollback`, `idSavepoint`, `idDisconnect`. |
| `SqlState` | package-private | Internal state: connection table (`addConnection`/`getConnection`), prepared-statement table, cursor table. |
| `SqlConnection` | package-private | Wraps a JDBC `Connection`; parses connection strings of the form `dbname@host:port`; resolves the default DB name. |
| `SqlCursor` | package-private | Holds cursor state (open/closed), `ResultSet`, `PreparedStatement`. |
| `SqlCA` | package-private | Writes SQLCA fields (`SQLCODE`, `SQLSTATE`, `SQLERRMC`, `SQLERRD`, ...) back into the corresponding `CobolDataStorage`. |
| `CobolDataConverter` | package-private | Converts between `AbstractCobolField` and JDBC `PreparedStatement.setXxx` / `ResultSet.getXxx`. Dispatches on `HVARTYPE_*`. |

All classes except `CobolSql` are package-private. They appear as SLF4J logger names but are not part of the external API.

### CobolSql signatures

Host variables arrive from generated Java as `AbstractCobolField[]`. Because the compiler has already resolved subscripts and qualifications, the runtime is **subscript- and qualification-agnostic**: it simply binds each array element to a JDBC parameter or result column via `CobolDataConverter`.

### Implicit SQLCA

`typeck.c` injects `01 SQLCA GLOBAL.` into the WORKING-STORAGE of any program that contains `EXEC SQL`, even without an explicit `EXEC SQL INCLUDE SQLCA`. `SQLERRD OCCURS 6` becomes one of the fields that codegen wires up via `b_SQLERRD__SQLCA.getSubDataStorage(...)`.

### NULL column notification (`ECPG_MISSING_INDICATOR`)

Compatible with ECPG, the runtime can return `SQLCODE = -22002` (`ECPG_MISSING_INDICATOR`) when a NULL column is read into a host variable without an indicator variable. The JUnit suites `CobolSqlTest` and `SqlCATest` cover the relevant cases.

## Tests

### Compiler / integration tests (`tests/`)

Autotest suites that drive a PostgreSQL container live under:

| Directory | Coverage |
|---|---|
| `tests/esql-basic.src/` | CONNECT / DISCONNECT / basic INSERT / SELECT |
| `tests/esql-cobol-data.src/` | COBOL data type × SQL type round-trips (numeric, packed decimal, alphanumeric, Japanese, VARYING, subscripted host vars) |
| `tests/esql-sql-data.src/` | SQL-side type variations |
| `tests/esql-sqlca.src/` | SQLCA field assertions |
| `tests/esql-misc.src/` | Cursors / PREPARE / EXECUTE / SAVEPOINT etc. |
| `tests/esql-utf8.src/` | UTF-8 build variants |

Each suite is generated by `make <name>` and runnable locally with `./<name>` (requires a PostgreSQL container). CI drives them through `.github/workflows/test-esql.yml`.

### libcobj unit tests (`libcobj/app/src/test/.../sql/`)

`CobolSqlTest`, `SqlStateTest`, `SqlCursorTest`, `SqlConnectionTest`, `CobolDataConverterTest`, `CobolSqlLoggingTest`, `SqlCATest` validate the runtime against a JDBC mock (testcontainers PostgreSQL).

## Design decisions

- **No ESQL-specific AST node**: host variables ride directly on `cb_reference->subs` / `cb_reference->chain`, so multi-dimensional subscripts, group qualification, and qualified subscripts all flow through the existing OCCURS resolution loop in `codegen.c`.
- **State-machine isolation for subscripts**: subscript and qualification parsing is contained in `ESQL_HOSTSUB_STATE`, which by construction never appends to the SQL body string. There is no way for a stray `(` or identifier to leak into a placeholder slot.
- **Drop dblibj**: the Open-COBOL-ESQL Scala dependency (dblibj) is removed; the runtime is pure Java, bundled inside `libcobj.jar` together with the PostgreSQL JDBC driver.
- **No `OF` qualification**: COBOL itself accepts `X OF Y`, but ESQL only accepts dotted qualification (`:Y.X`). This keeps the scanner state count small and avoids reserved-word collisions with `OF` inside SQL clauses.
- **Wrap host-variable lists in the generated Java**: the host variables passed to `CobolSql.*` calls are emitted by `joutput_sql_host_list_newline` (argument lists) and `joutput_sql_field_array` (`new AbstractCobolField[]{...}` literals) in `codegen.c`. Both insert a line break every `SQL_HOST_VAR_WRAP` (= 5) host variables instead of putting them all on one line, so a statement with many host variables stays readable in the generated source. This is purely cosmetic and does not change the arguments passed.

## Related documents

- [esql-guide.md](./esql-guide.md) -- end-user usage
- [esql-logging_JP.md](./esql-logging_JP.md) -- SLF4J logging configuration
- [esql-design_JP.md](./esql-design_JP.md) -- Japanese version of this document

# ESQL Examples for opensource COBOL 4J

This directory contains example COBOL programs demonstrating Embedded SQL (EXEC SQL) with PostgreSQL.

## Prerequisites

- opensource COBOL 4J installed
- PostgreSQL server running
- A database and user created (defaults: `testdb` / `main_user` / `password`)

## Setup

1. Edit `db-config.sh` to match your PostgreSQL connection settings:

```bash
DB_NAME=testdb
DB_HOST=localhost
DB_PORT=5432
DB_USER=main_user
DB_PASSWORD=password
```

2. Make sure the database exists:

```bash
createdb testdb
```

## Running the Examples

Run all examples:

```bash
make all
```

Run a specific example:

```bash
make sample    # Basic INSERT/SELECT
make cursor    # Cursor-based row iteration
make prepare   # Prepared statements
```

Clean up generated files:

```bash
make clean
```

## Examples

| File | Description |
|---|---|
| `sample.cbl` | Basic CONNECT, CREATE TABLE, INSERT, SELECT INTO, DROP, DISCONNECT |
| `cursor.cbl` | DECLARE CURSOR, OPEN, FETCH loop, CLOSE |
| `prepare.cbl` | PREPARE statement, EXECUTE with USING |

## How It Works

The COBOL source files contain placeholders like `<|DB_NAME|>` for database connection details. The Makefile copies each source file to a `*_run.cbl` file, replaces the placeholders with values from `db-config.sh`, then compiles and runs the program.

For more details on Embedded SQL in opensource COBOL 4J, see the [ESQL Guide](../../doc/esql-guide.md).

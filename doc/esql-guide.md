# Embedded SQL (ESQL) Guide for opensource COBOL 4J

## Overview

opensource COBOL 4J supports Embedded SQL (EXEC SQL) to interact with PostgreSQL databases directly from COBOL programs. COBOL host variables are used as bind parameters in SQL statements, enabling seamless data exchange between COBOL and SQL.

When a COBOL source file containing `EXEC SQL ... END-EXEC` statements is compiled with `cobj`, the compiler translates the embedded SQL into Java database calls via JDBC.

## Prerequisites

- **opensource COBOL 4J** installed (see [README.md](../README.md))
- **PostgreSQL** server (version 9.6 or later)
- **Java** (JDK 11 or later)
- The PostgreSQL JDBC driver is bundled in `libcobj.jar`

## Quick Start

Below is a minimal COBOL program that connects to PostgreSQL, inserts a row, reads it back, and disconnects.

```cobol
       IDENTIFICATION              DIVISION.
       PROGRAM-ID.                 QUICK-START.
       DATA                        DIVISION.
       WORKING-STORAGE             SECTION.

       EXEC SQL BEGIN DECLARE SECTION END-EXEC.
       01  DBNAME    PIC X(30) VALUE "testdb@localhost:5432".
       01  USERNAME  PIC X(30) VALUE "main_user".
       01  PASSWD    PIC X(10) VALUE "password".
       01  EMP-NAME  PIC X(20).
       EXEC SQL END DECLARE SECTION END-EXEC.

       PROCEDURE                   DIVISION.
           EXEC SQL
               CONNECT :USERNAME IDENTIFIED BY :PASSWD USING :DBNAME
           END-EXEC.

           MOVE "Alice" TO EMP-NAME.
           EXEC SQL
               INSERT INTO employees (name) VALUES (:EMP-NAME)
           END-EXEC.

           MOVE SPACES TO EMP-NAME.
           EXEC SQL
               SELECT name INTO :EMP-NAME FROM employees LIMIT 1
           END-EXEC.
           DISPLAY "Employee: " EMP-NAME.

           EXEC SQL COMMIT END-EXEC.
           EXEC SQL DISCONNECT ALL END-EXEC.
           STOP RUN.
```

Compile and run:

```bash
cobj -I /usr/lib/opensourcecobol4j/copy quick-start.cbl
java QUICK-START
```

## Supported SQL Statements

### CONNECT / DISCONNECT

```cobol
       EXEC SQL
           CONNECT :USERNAME IDENTIFIED BY :PASSWD USING :DBNAME
       END-EXEC.

       EXEC SQL
           DISCONNECT ALL
       END-EXEC.
```

The connection string format for `:DBNAME` is:

```
dbname@host:port
```

For example: `"testdb@localhost:5432"`.

### BEGIN / END DECLARE SECTION

Host variables used in SQL statements can be declared between `EXEC SQL BEGIN DECLARE SECTION END-EXEC` and `EXEC SQL END DECLARE SECTION END-EXEC`:

```cobol
       EXEC SQL BEGIN DECLARE SECTION END-EXEC.
       01  MY-VAR  PIC X(20).
       01  MY-NUM  PIC 9(5).
       EXEC SQL END DECLARE SECTION END-EXEC.
```

However, `BEGIN / END DECLARE SECTION` is **optional**. If omitted, all variables in WORKING-STORAGE SECTION and LINKAGE SECTION are treated as potential host variables. The `VARYING` clause for variable-length fields is also available without DECLARE SECTION.

```cobol
      * No DECLARE SECTION needed:
       WORKING-STORAGE SECTION.
       01  MY-VAR  PIC X(20).
       01  MY-NUM  PIC 9(5).
       PROCEDURE DIVISION.
           EXEC SQL
               INSERT INTO TBL VALUES (:MY-VAR, :MY-NUM)
           END-EXEC.
```

### SQLCA (SQL Communication Area)

The SQLCA is automatically defined when a program uses `EXEC SQL` statements. It provides `SQLCODE`, `SQLSTATE`, `SQLERRMC`, and other diagnostic fields that are updated after each SQL statement.

For backward compatibility, `EXEC SQL INCLUDE SQLCA END-EXEC` is still accepted but is no longer required.

The implicitly defined SQLCA has the following structure:

```cobol
       01  SQLCA GLOBAL.
           05  SQLCAID               PIC X(8).
           05  SQLCABC               PIC S9(9) COMP-5.
           05  SQLCODE               PIC S9(9) COMP-5.
           05  SQLERRM.
           49  SQLERRML              PIC S9(4) COMP-5.
           49  SQLERRMC              PIC X(70).
           05  SQLERRP               PIC X(8).
           05  SQLERRD OCCURS 6 TIMES
                                     PIC S9(9) COMP-5.
           05  SQLWARN.
               10 SQLWARN0           PIC X(1).
               10 SQLWARN1           PIC X(1).
               10 SQLWARN2           PIC X(1).
               10 SQLWARN3           PIC X(1).
               10 SQLWARN4           PIC X(1).
               10 SQLWARN5           PIC X(1).
               10 SQLWARN6           PIC X(1).
               10 SQLWARN7           PIC X(1).
           05  SQLSTATE              PIC X(5).
```

| Field | Description |
|-------|-------------|
| `SQLCODE` | Return code: 0 = success, 100 = no more rows, negative = error |
| `SQLSTATE` | 5-character SQL state code (e.g., `"00000"`, `"02000"`, `"08001"`) |
| `SQLERRMC` | Error message text (up to 70 characters) |
| `SQLERRML` | Length of error message in `SQLERRMC` |
| `SQLERRD(3)` | Number of rows affected by the last statement |

### SELECT INTO

Single-row select:

```cobol
       EXEC SQL
           SELECT emp_name, emp_salary
               INTO :EMP-NAME, :EMP-SALARY
               FROM employees
               WHERE emp_no = :EMP-NO
       END-EXEC.
```

Array fetch with OCCURS:

```cobol
       01  EMP-NAMES.
           05 EMP-NAME PIC X(20) OCCURS 10 TIMES.

       EXEC SQL
           SELECT emp_name INTO :EMP-NAME FROM employees
       END-EXEC.
```

### INSERT, UPDATE, DELETE

Use host variables prefixed with `:` as bind parameters:

```cobol
       EXEC SQL
           INSERT INTO employees (emp_no, emp_name, emp_salary)
               VALUES (:EMP-NO, :EMP-NAME, :EMP-SALARY)
       END-EXEC.

       EXEC SQL
           UPDATE employees SET emp_salary = :NEW-SALARY
               WHERE emp_no = :EMP-NO
       END-EXEC.

       EXEC SQL
           DELETE FROM employees WHERE emp_no = :EMP-NO
       END-EXEC.
```

### DECLARE CURSOR / OPEN / FETCH / CLOSE

```cobol
       EXEC SQL
           DECLARE emp_cursor CURSOR FOR
               SELECT emp_no, emp_name FROM employees
               ORDER BY emp_no
       END-EXEC.

       EXEC SQL OPEN emp_cursor END-EXEC.

       PERFORM UNTIL SQLCODE NOT = ZERO
           EXEC SQL
               FETCH emp_cursor INTO :EMP-NO, :EMP-NAME
           END-EXEC
           IF SQLCODE = ZERO
               DISPLAY "No: " EMP-NO " Name: " EMP-NAME
           END-IF
       END-PERFORM.

       EXEC SQL CLOSE emp_cursor END-EXEC.
```

### PREPARE / EXECUTE

```cobol
       01  SQL-STMT  PIC X(200).

       MOVE "INSERT INTO employees (emp_no, emp_name) VALUES (?, ?)"
           TO SQL-STMT.
       EXEC SQL
           PREPARE stmt1 FROM :SQL-STMT
       END-EXEC.

       EXEC SQL
           EXECUTE stmt1 USING :EMP-NO, :EMP-NAME
       END-EXEC.
```

### COMMIT / ROLLBACK

```cobol
       EXEC SQL COMMIT END-EXEC.
       EXEC SQL ROLLBACK END-EXEC.
```

## Host Variable Type Mappings

The following table shows how COBOL PIC clauses map to SQL types:

| COBOL PIC Clause | SQL Type | Notes |
|---|---|---|
| `PIC X(n)` | CHAR / VARCHAR | Alphanumeric string |
| `PIC 9(n)` | NUMERIC | Unsigned integer |
| `PIC S9(n)` | NUMERIC | Signed integer |
| `PIC 9(n)V9(m)` | DECIMAL | Fixed-point decimal |
| `PIC 9(n) USAGE COMP-3` | NUMERIC | Packed decimal |
| `PIC 9(n) USAGE COMP-5` | INTEGER / BIGINT | Native binary integer |
| `PIC N(n)` | NATIONAL CHARACTER | National (wide) character |

## SQLCA Error Handling

After each `EXEC SQL` statement, the SQLCA fields are updated:

| Field | Description |
|---|---|
| `SQLCODE` | 0 = success, +10 = not found, negative = error |
| `SQLSTATE` | 5-character SQL state code |
| `SQLERRMC` | Error message text |
| `SQLERRD(3)` | Number of rows affected |

Common SQLCODE values:

| SQLCODE | Meaning |
|---|---|
| 0 | Success |
| +10 | Record not found / end of cursor |
| -1 | Connection failed |
| -20 | Internal error |
| -30 | PostgreSQL error (see SQLSTATE and SQLERRMC) |

Example error handling:

```cobol
       IF SQLCODE NOT = ZERO
           DISPLAY "SQL Error: " SQLCODE
           DISPLAY "State: " SQLSTATE
           DISPLAY "Message: " SQLERRMC
           EXEC SQL ROLLBACK END-EXEC
           STOP RUN
       END-IF.
```

## Connection String Format

The connection string passed to CONNECT follows this format:

```
dbname@host:port
```

Example:

```cobol
       MOVE "testdb@localhost:5432" TO DBNAME.
```

## Environment Variables

| Variable | Description |
|---|---|
| `OCDB_DB_NAME` | Default database name |
| `OCDB_DB_USER` | Default database user |
| `OCDB_DB_PASS` | Default database password |
| `OCDB_DB_CHAR` | Character encoding for the database connection |

## Compilation

```bash
# Compile a COBOL program with EXEC SQL
cobj -I /usr/lib/opensourcecobol4j/copy program.cbl

# Run the compiled program
java program
```

The `-I` flag specifies the directory containing COPY files.

## Limitations

- GROUP-qualified host variables (e.g., `:GROUP.FIELD`) are not supported; use elementary items directly.
- UTF-8 variable names in SJIS mode are not supported; use the `--enable-utf8` build option for UTF-8 source files.
- Only PostgreSQL is supported as the target database.

## Examples

See the [example/esql/](../example/esql/) directory for complete working examples:

- **sample.cbl** -- Basic CONNECT, INSERT, SELECT, DISCONNECT
- **cursor.cbl** -- Cursor-based row iteration
- **prepare.cbl** -- Prepared statements with EXECUTE USING

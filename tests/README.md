Execute make command to generate the following test scripts.

* cobj-idx
* cobol\_utf8
* command-line-options
* data-rep
* file-lock
* file-lock2
* file-lock3
* file-lock4
* file-lock5
* i18n\_sjis
* i18n\_utf8
* jp-compat
* misc
* run
* syntax

For example, execute ./command-line-options in order to run command-line-options test.

## ESQL tests

The ESQL suites -- `esql-basic`, `esql-cobol-data`, `esql-misc`, `esql-sql-data`,
`esql-sqlca` and `esql-utf8` -- additionally need a PostgreSQL server, and
`embed_db_info.sh` in this directory has to name it. `.github/workflows/db-settings/`
holds one copy of that script per environment; copy the matching one over
`embed_db_info.sh` before running a suite.

On Windows, `win/start-test-postgresql.ps1` brings up a cluster to test against.
It initialises one on first use and reuses it afterwards, and `-Stop` shuts it
down. Pair it with `embed_db_info_windows.sh`:

```
powershell -File ..\win\start-test-postgresql.ps1
cp ../.github/workflows/db-settings/embed_db_info_windows.sh embed_db_info.sh
./esql-basic
```

`esql-utf8` needs a compiler configured with `--enable-utf8`, so it does not run
against the Shift_JIS Windows build.

## Renamed to Hagane COBOL

This is the first release under the new name **Hagane COBOL**. It was formerly developed as a fork of [opensource COBOL 4J](https://github.com/opensourcecobol/opensourcecobol4j), and the repository has moved from `yutaro-sakamoto/opensourcecobol4j` to [yutaro-sakamoto/Hagane-COBOL](https://github.com/yutaro-sakamoto/Hagane-COBOL). Version numbers restart at 1.0.0. Hagane COBOL 1.0.0 is based on opensource COBOL 4J 2.1.0, and the changes below are relative to it.

- The command names (`cobj`, `cobj-idx`, `cobj-api`), the runtime library `libcobj.jar`, the Java package `jp.osscons.opensourcecobol`, and the install paths (`/usr/lib/opensourcecobol4j/libcobj.jar` and `C:\opensourcecobol4j`) are unchanged.
- `cobj --version` and the header comment of the generated Java now say "Hagane COBOL".
- Docker images are published to GitHub Container Registry as `ghcr.io/yutaro-sakamoto/hagane-cobol` (`1.0.0`, `1.0.0-utf8` and `latest`), instead of Docker Hub.

## Breaking Changes

- **Programs must be recompiled.** Java translated by an earlier `cobj` (including opensource COBOL 4J 2.1.0) does not run with this `libcobj.jar`, because the runtime API used by the generated code has changed (#21, #48).
- **STOP RUN called from Java** now returns to the caller instead of calling `System.exit`. Only the `main()` of a generated program exits the JVM. (#48)
- **INDEXED files opened OUTPUT** are no longer committed after every WRITE (see "Faster INDEXED file writes" below). Set `COB_FILE_IDX_COMMIT_INTERVAL=1` to restore the old behavior. (#43)
- Java code written by hand against libcobj: `CobolUtil.cal` was removed (#39), and public static fields such as `CobolRuntimeException.code`, `CobolCallParams.callParams`, `CobolFile.errorFile` and `CobolUtil.cobSwitch` were replaced by accessors (#48).
- Amazon Linux 2023 is no longer supported. (#24)

## New Features

- **Multi-threading support.** Generated programs and `libcobj` can run COBOL programs from several threads of one JVM, such as the request threads of a Tomcat or Spring Boot server. Each thread gets its own COBOL run unit, file and record locks between threads follow the same rules as between processes, and `CobolRunUnit.end()` releases a thread's run unit in a thread pool. See [doc/multithreading.md](https://github.com/yutaro-sakamoto/Hagane-COBOL/blob/v1.0.0/doc/multithreading.md). (#48)
- **Source code in compiler diagnostics.** Errors and warnings of `cobj` show the surrounding source lines with line numbers and a `>` marker on the offending line. They can be turned off with `-fno-diagnostics-show-caret` and `-fno-diagnostics-show-line-numbers`. (#36)
- **Faster INDEXED file writes.** INDEXED files opened OUTPUT are committed by the `COMMIT` statement and at CLOSE instead of after every WRITE, which makes writing 100,000 records about 800 times faster. The new environment variable `COB_FILE_IDX_COMMIT_INTERVAL` commits every N writes. `ROLLBACK` now discards uncommitted writes. Records not yet committed are lost if the process is killed before CLOSE. (#43)
- **Buffered reads of sequential files.** SEQUENTIAL and LINE SEQUENTIAL files opened INPUT are read through a buffer (about 20 times faster for 200,000 records). The buffer size is set by the new environment variable `COB_FILE_SEQ_READ_BUFFER_SIZE` (records, default 10, `0` disables it). (#23)
- **SBOM.** Every release ships a CycloneDX 1.6 SBOM of `libcobj.jar` (`libcobj-sbom.json` and `libcobj-sbom.xml`). (#32)

## Performance

- PERFORM, GO TO and fall-through no longer allocate objects; a control-flow-heavy benchmark runs about 50% faster. (#21)
- INDEXED files cache the prepared SQL statements for every operation: sequential READ NEXT is about 1.7 times faster, and random reads by the primary key are 1.2 to 1.35 times faster. (#44)
- Faster internal byte copy and fill of data items. (#33)

## Bug Fixes

- Windows: fix crashes of `cobj.exe` with `-debug` and variable subscripts, reference modification or SEARCH ALL, fix jar paths with `-java-package`, and report PROGRAM-IDs that differ only in letter case as an error instead of overwriting the output. (#13)
- `FUNCTION CURRENT-DATE` returns the current time on every call and the real GMT offset. `COB_DATE` is normalized like the C implementation of opensource COBOL. (#39)
- `DATE-TO-YYYYMMDD` and `DAY-TO-YYYYDDD` returned a year off by 100 when the third argument was omitted. (#42)
- INDEXED files: the implicit CLOSE at STOP RUN lost data and left a stale lock; SQL errors are reported with the proper file status (34, 22, 51 or 30) instead of always 51. (#43)
- Programs that use INDEXED files and start at the same time no longer print spurious sqlite-jdbc stack traces. (#28)
- An invalid `COB_FILE_SEQ_WRITE_BUFFER_SIZE` now falls back to the default with a warning instead of aborting. (#23)
- `STRING ... WITH POINTER` into a PIC N item no longer aborts when the pointer is near the end. (#33)
- UTF-8 build: `cobj` no longer hangs on bytes that are not valid UTF-8, and `&`-concatenated national literals are no longer split wrongly. (#37)
- `cobj` builds with glibc 2.42 and newer GCC. (#16)

## Miscellaneous

- Updated the runtime dependencies of libcobj (guava, sqlite-jdbc, org.json, slf4j, postgresql, commons-cli) and the Gradle wrapper. (#16, #30, #32, #51)
- CI runs on Ubuntu 26.04 and AlmaLinux 9, runs the ESQL tests on Windows, runs tests/misc on the UTF-8 build, adds multi-threading and Spring Boot smoke tests, and runs the test suites in parallel. (#16, #19, #20, #28, #29, #31, #34, #35, #37, #48)

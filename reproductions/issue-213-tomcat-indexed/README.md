# Reproduction: INDEXED file OPEN fails on Tomcat (issue #213)

Reproduces [opensourcecobol/opensourcecobol4j#213](https://github.com/opensourcecobol/opensourcecobol4j/issues/213)
— "Open INDEX FILE(SQLite) Problem on Tomcat" — in CI, without needing a real
servlet container.

## Root cause

`libcobj.jar` bundles the SQLite JDBC driver and relies on the JDBC 4.0
auto-registration mechanism (`META-INF/services/java.sql.Driver` scanned by
`ServiceLoader`) to register `org.sqlite.JDBC` with `java.sql.DriverManager`.

- **Standalone JVM:** every class is on one application class loader, so the
  driver scan sees the SQLite driver and `DriverManager.getConnection("jdbc:sqlite:...")`
  in `CobolIndexedFile.getConnection` works.
- **Tomcat:** `DriverManager` lives on the system/bootstrap class loader and runs
  its one-time driver scan at container startup, before (and without visibility
  of) the web-app class loader that holds `WEB-INF/lib/libcobj.jar`. The SQLite
  driver is therefore never registered, and INDEXED-file OPEN fails with
  `No suitable driver found for jdbc:sqlite:...`.

The affected code is
`libcobj/app/src/main/java/jp/osscons/opensourcecobol/libcobj/file/CobolIndexedFile.java`,
method `getConnection(String filename)`.

## How the reproduction works

There are two reproductions, both driven from CI by
`.github/workflows/reproduce-issue-213.yml`:

1. **Real Tomcat** (`tomcat/`, authoritative) — deploys the generated program
   as a web application on a real Apache Tomcat and drives it over HTTP.
2. **Class-loader simulation** (fast pre-check) — recreates Tomcat's
   class-loader isolation with a plain JVM, no servlet container needed.

### 1. Real Tomcat (`tomcat/reproduce-tomcat.sh`)

`IndexedServlet` runs the generated COBOL program (`idxopen`) and returns its
`OPEN OUTPUT STATUS=xx` line in the HTTP response. The script:

- compiles `idxopen.cbl` and the servlet,
- assembles a web app (`WEB-INF/lib/libcobj.jar` + `WEB-INF/classes`),
- downloads and starts Tomcat 10.1, deploys the app, and requests `/repro/open`.

Result: `STATUS=30` (bug reproduced) vs `STATUS=00` once fixed. Tomcat's default
`JreMemoryLeakPreventionListener` initialises `DriverManager` under the shared
class loader at startup, which is exactly what leaves the web app's SQLite
driver unregistered.

> The program uses `GOBACK` (not `STOP RUN`) so it returns control to the
> servlet instead of calling `System.exit`, which would otherwise shut Tomcat
> down.

### 2. Class-loader simulation (`TomcatSim.java` + `reproduce.sh`)

`TomcatSim.java` recreates Tomcat's class-loader isolation with a plain JVM:

1. It is launched with **only itself** on the class path (no `libcobj.jar`), then
   touches `DriverManager` — forcing its one-time `ServiceLoader` scan while the
   SQLite driver is invisible, so nothing is registered.
2. It loads `libcobj.jar` + the compiled COBOL program in a child
   `URLClassLoader` (the simulated web-app class loader).
3. It runs the program, which calls `CobolIndexedFile.getConnection` →
   `DriverManager.getConnection` → fails, exactly as on Tomcat.

`reproduce.sh` compiles `idxopen.cbl`, runs it both standalone and under
`TomcatSim`, and compares the OPEN status:

- standalone → `OPEN OUTPUT STATUS=00`
- isolated   → `OPEN OUTPUT STATUS=30`

It exits non-zero while the bug is present, so it doubles as a regression test:
once `getConnection` registers the driver explicitly (e.g.
`Class.forName("org.sqlite.JDBC")`), the isolated run also returns `00` and the
script passes.

## Running locally

```bash
# build & install cobj + libcobj first (see repo README/CLAUDE.md)
./configure --prefix=$(pwd)/.local_install && make && make install

export COBJ=$(pwd)/.local_install/bin/cobj
export LIBCOBJ_JAR=$(pwd)/.local_install/lib/opensourcecobol4j/libcobj.jar

# class-loader simulation (fast)
bash reproductions/issue-213-tomcat-indexed/reproduce.sh

# real Tomcat (downloads Tomcat 10.1; needs curl + JAVA_HOME)
bash reproductions/issue-213-tomcat-indexed/tomcat/reproduce-tomcat.sh
```

Set `CATALINA_HOME` to reuse an existing Tomcat install instead of downloading.

In CI both run automatically via
`.github/workflows/reproduce-issue-213.yml` on every push.

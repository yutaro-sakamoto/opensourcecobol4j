#!/bin/bash
# Reproduces opensourcecobol/opensourcecobol4j#213 on a REAL Apache Tomcat.
#
# It deploys the COBOL4J-generated INDEXED-file program as a Tomcat web
# application (via IndexedServlet), starts Tomcat, and requests the servlet over
# HTTP. On Tomcat the bundled SQLite JDBC driver is never registered with
# DriverManager, so INDEXED OPEN returns STATUS=30 instead of STATUS=00.
#
# The script exits non-zero while the bug is present (reproduction / regression
# test); once libcobj registers the driver explicitly the servlet returns
# STATUS=00 and the script passes.
#
# Required environment variables:
#   COBJ         path to the cobj executable
#   LIBCOBJ_JAR  path to libcobj.jar
# Optional:
#   TOMCAT_VERSION  Tomcat 10.1.x version to download (default below)
#   CATALINA_HOME   use an existing Tomcat install instead of downloading
set -u

HERE="$(cd "$(dirname "$0")" && pwd)"
REPRO_DIR="$(cd "$HERE/.." && pwd)"
WORK="$(mktemp -d)"

: "${COBJ:?set COBJ to the cobj executable path}"
: "${LIBCOBJ_JAR:?set LIBCOBJ_JAR to the libcobj.jar path}"
TOMCAT_VERSION="${TOMCAT_VERSION:-10.1.34}"
PORT=8080
URL="http://localhost:${PORT}/repro/open"

CATALINA_HOME="${CATALINA_HOME:-}"

cleanup() {
  if [ -n "${CATALINA_HOME:-}" ] && [ -x "$CATALINA_HOME/bin/catalina.sh" ]; then
    "$CATALINA_HOME/bin/catalina.sh" stop 20 -force >/dev/null 2>&1 || true
  fi
  rm -rf "$WORK"
}
trap cleanup EXIT

# --- Obtain Tomcat -----------------------------------------------------------
if [ -z "$CATALINA_HOME" ]; then
  echo "== Downloading Apache Tomcat ${TOMCAT_VERSION} =="
  TARBALL="$WORK/tomcat.tar.gz"
  curl -fsSL --retry 3 \
    -o "$TARBALL" \
    "https://archive.apache.org/dist/tomcat/tomcat-10/v${TOMCAT_VERSION}/bin/apache-tomcat-${TOMCAT_VERSION}.tar.gz"
  tar zxf "$TARBALL" -C "$WORK"
  CATALINA_HOME="$WORK/apache-tomcat-${TOMCAT_VERSION}"
fi
export CATALINA_HOME
echo "Using Tomcat at: $CATALINA_HOME"

SERVLET_API="$CATALINA_HOME/lib/servlet-api.jar"
if [ ! -f "$SERVLET_API" ]; then
  echo "FATAL: servlet-api.jar not found under $CATALINA_HOME/lib" >&2
  exit 2
fi

# --- Compile the COBOL program and the servlet -------------------------------
echo "== Compiling idxopen.cbl with cobj =="
CLASSES="$WORK/webapp/WEB-INF/classes"
LIBDIR="$WORK/webapp/WEB-INF/lib"
mkdir -p "$CLASSES" "$LIBDIR"
( cd "$CLASSES" && CLASSPATH="$LIBCOBJ_JAR" "$COBJ" "$REPRO_DIR/idxopen.cbl" )
if [ ! -f "$CLASSES/idxopen.class" ]; then
  echo "FATAL: cobj did not produce idxopen.class" >&2
  exit 2
fi

echo "== Compiling IndexedServlet.java =="
javac -cp "$SERVLET_API:$CLASSES:$LIBCOBJ_JAR" -d "$CLASSES" "$HERE/IndexedServlet.java"

# --- Assemble the exploded web application -----------------------------------
echo "== Assembling web application =="
cp "$HERE/web.xml" "$WORK/webapp/WEB-INF/web.xml"
cp "$LIBCOBJ_JAR" "$LIBDIR/libcobj.jar"
rm -rf "$CATALINA_HOME/webapps/repro"
cp -r "$WORK/webapp" "$CATALINA_HOME/webapps/repro"

# --- Start Tomcat ------------------------------------------------------------
echo "== Starting Tomcat =="
"$CATALINA_HOME/bin/catalina.sh" start

echo "== Waiting for the servlet to respond =="
BODY=""
HTTP_CODE=000
for i in $(seq 1 60); do
  HTTP_CODE="$(curl -s -o "$WORK/body.txt" -w '%{http_code}' "$URL" 2>/dev/null || echo 000)"
  if [ "$HTTP_CODE" = "200" ]; then
    BODY="$(cat "$WORK/body.txt")"
    break
  fi
  sleep 1
done

echo
echo "===================================================================="
echo " HTTP status: $HTTP_CODE"
echo " Servlet response:"
echo "--------------------------------------------------------------------"
echo "$BODY"
echo "--------------------------------------------------------------------"

if [ "$HTTP_CODE" != "200" ]; then
  echo "FATAL: servlet did not respond with HTTP 200." >&2
  echo "---- catalina.out (tail) ----" >&2
  tail -n 60 "$CATALINA_HOME/logs/catalina.out" 2>/dev/null >&2 || true
  exit 2
fi

STATUS="$(printf '%s\n' "$BODY" | sed -n 's/^OPEN OUTPUT STATUS=//p' | head -n1)"
echo " Parsed OPEN status: ${STATUS:-<none>}"

if [ "$STATUS" = "00" ]; then
  echo
  echo "OK: INDEXED OPEN succeeded on Tomcat (STATUS=00)."
  echo "    Issue #213 appears to be FIXED."
  exit 0
fi

echo
echo "BUG REPRODUCED on real Tomcat: issue #213 is still present."
echo "  INDEXED-file OPEN returned STATUS=${STATUS:-<none>} because the SQLite JDBC"
echo "  driver is never registered with DriverManager under the web-app class"
echo "  loader."
exit 1

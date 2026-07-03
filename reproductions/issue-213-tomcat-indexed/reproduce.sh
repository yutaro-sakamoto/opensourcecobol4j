#!/bin/bash
# Reproduces opensourcecobol/opensourcecobol4j#213
# "Open INDEX FILE(SQLite) Problem on Tomcat".
#
# The bug: libcobj relies on JDBC 4.0 ServiceLoader auto-registration
# (META-INF/services/java.sql.Driver) to register the bundled SQLite driver.
# On Tomcat, DriverManager lives on the system class loader and runs its
# one-time driver scan without visibility of the web-app class loader, so the
# SQLite driver is never registered and INDEXED-file OPEN fails.
#
# This script demonstrates it with two JVM runs of the SAME compiled program:
#   1. standalone           -> OPEN OUTPUT STATUS=00 (works)
#   2. class-loader isolated -> OPEN OUTPUT STATUS=30 (bug reproduced)
#
# The script exits non-zero when the bug is present so the CI job is red until
# the bug is fixed (i.e. it doubles as a regression test): once libcobj
# registers the driver explicitly, the isolated run yields STATUS=00 and this
# script passes.
#
# Required environment variables:
#   COBJ         path to the cobj executable
#   LIBCOBJ_JAR  path to libcobj.jar
set -u

HERE="$(cd "$(dirname "$0")" && pwd)"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

: "${COBJ:?set COBJ to the cobj executable path}"
: "${LIBCOBJ_JAR:?set LIBCOBJ_JAR to the libcobj.jar path}"

echo "== Compiling idxopen.cbl with cobj =="
mkdir -p "$WORK/progdir"
( cd "$WORK/progdir" && CLASSPATH="$LIBCOBJ_JAR" "$COBJ" "$HERE/idxopen.cbl" )
if [ ! -f "$WORK/progdir/idxopen.class" ]; then
  echo "FATAL: cobj did not produce idxopen.class" >&2
  exit 2
fi

echo "== Compiling the Tomcat class-loader simulator =="
mkdir -p "$WORK/launcher"
javac -d "$WORK/launcher" "$HERE/TomcatSim.java"

echo
echo "===================================================================="
echo " Run 1: standalone JVM (libcobj.jar on the application class path)"
echo "===================================================================="
STANDALONE_OUT="$(cd "$WORK/progdir" && rm -f testidx.dat && \
  CLASSPATH="$LIBCOBJ_JAR:." java idxopen 2>&1)"
echo "$STANDALONE_OUT"
STANDALONE_STATUS="$(echo "$STANDALONE_OUT" | sed -n 's/^OPEN OUTPUT STATUS=//p')"

echo
echo "===================================================================="
echo " Run 2: Tomcat-like class-loader isolation"
echo "        (SQLite driver only in the isolated web-app class loader)"
echo "===================================================================="
ISOLATED_OUT="$(cd "$WORK/progdir" && rm -f testidx.dat && \
  java -cp "$WORK/launcher" TomcatSim "$LIBCOBJ_JAR" "$WORK/progdir" idxopen 2>&1)"
echo "$ISOLATED_OUT"
ISOLATED_STATUS="$(echo "$ISOLATED_OUT" | sed -n 's/^OPEN OUTPUT STATUS=//p')"

echo
echo "===================================================================="
echo " Result"
echo "===================================================================="
echo "  standalone OPEN status : ${STANDALONE_STATUS:-<none>}"
echo "  isolated   OPEN status : ${ISOLATED_STATUS:-<none>}"

if [ "$STANDALONE_STATUS" != "00" ]; then
  echo "UNEXPECTED: standalone run did not succeed (STATUS=$STANDALONE_STATUS)." >&2
  echo "The reproduction environment is broken; cannot assess the bug." >&2
  exit 2
fi

if [ "$ISOLATED_STATUS" = "00" ]; then
  echo
  echo "OK: INDEXED OPEN succeeds even under class-loader isolation."
  echo "    Issue #213 appears to be FIXED."
  exit 0
fi

echo
echo "BUG REPRODUCED: issue #213 is still present."
echo "  INDEXED-file OPEN works standalone (STATUS=00) but fails under"
echo "  Tomcat-style class-loader isolation (STATUS=$ISOLATED_STATUS) because the"
echo "  SQLite JDBC driver is never registered with DriverManager."
exit 1

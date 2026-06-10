#!/bin/bash
# JMHマイクロベンチマークのビルドと実行
#
# benchmark/jmh/ の Maven プロジェクトをビルドし、全ベンチマークを実行して
# benchmark/results/jmh_result.txt / jmh_result.json に保存する。
#
# 前提: $ROOT/.local_install に libcobj.jar がインストール済み

set -eu -o pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$SCRIPT_DIR/../.." && pwd)
JMH_DIR="$ROOT/benchmark/jmh"
RESULTS_DIR="$ROOT/benchmark/results"
LIBCOBJ_JAR="$ROOT/.local_install/lib/opensourcecobol4j/libcobj.jar"

mkdir -p "$RESULTS_DIR"

echo "=== JMHプロジェクトのビルド ==="
cd "$JMH_DIR"
MAVEN_OPTS="-Xmx512m" mvn -q -B package

echo "=== JMHベンチマーク実行 (fork=1, RAM制約環境向け設定) ==="
java -Xmx256m -cp "target/benchmarks.jar:$LIBCOBJ_JAR" org.openjdk.jmh.Main \
    -rf json -rff "$RESULTS_DIR/jmh_result.json" \
    | tee "$RESULTS_DIR/jmh_result.txt"

echo "完了: $RESULTS_DIR/jmh_result.txt"

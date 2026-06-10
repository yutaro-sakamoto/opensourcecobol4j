#!/bin/bash
# opensource COBOL 4J 性能計測の一発実行スクリプト
#
# 実行内容:
#   1. opensource COBOL 4J をリポジトリ直下の .local_install にローカルインストール
#      (グローバル領域には一切インストールしない)
#   2. マクロベンチマーク (COBOL→Java 変換したプログラムの実測)
#      - wall時間/CPU時間/最大RSS/GC統計/JFRプロファイル
#   3. JMHマイクロベンチマーク (libcobj ランタイムのホットパス)
#
# 使い方:
#   ./benchmark/run_all.sh            # ビルド済みならビルドをスキップ
#   FORCE_BUILD=1 ./benchmark/run_all.sh  # 強制リビルド
#   SKIP_JMH=1 ./benchmark/run_all.sh     # JMHをスキップ (マクロのみ)
#
# 結果は benchmark/results/ に出力される:
#   macro_summary.md / macro_summary.csv  - マクロベンチ集計
#   macro/<bench>/                        - 生データ (json, gc.log, jfr)
#   jmh_result.txt / jmh_result.json      - JMH結果

set -eu -o pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$SCRIPT_DIR/.." && pwd)

echo "##### 1. ローカルビルド & インストール #####"
if [ "${FORCE_BUILD:-0}" = "1" ] || [ ! -x "$ROOT/.local_install/bin/cobj" ]; then
    cd "$ROOT"
    export GRADLE_OPTS="-Xmx512m -Dorg.gradle.daemon=false"
    ./configure --prefix="$ROOT/.local_install"
    make
    make install
else
    echo "ビルド済み ($ROOT/.local_install/bin/cobj) のためスキップ"
fi

echo "##### 2. マクロベンチマーク #####"
bash "$SCRIPT_DIR/scripts/run_macro.sh"

if [ "${SKIP_JMH:-0}" != "1" ]; then
    echo "##### 3. JMHマイクロベンチマーク #####"
    bash "$SCRIPT_DIR/scripts/run_jmh.sh"
fi

echo "##### 完了 #####"
echo "結果: $SCRIPT_DIR/results/"

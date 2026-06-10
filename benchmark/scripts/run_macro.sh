#!/bin/bash
# マクロベンチマーク実行スクリプト
#
# benchmark/cobol/*.cbl をローカルインストール済みの cobj でコンパイルし、
# 各ベンチマークについて以下を計測して benchmark/results/ に保存する。
#   - wall時間/CPU時間/最大RSS (複数回実行、measure.py 使用)
#   - GCログ (-Xlog:gc)
#   - JFRプロファイル (settings=profile) と jfr view による要約
#
# 前提: $ROOT/.local_install にインストール済み (run_all.sh が面倒を見る)

set -eu -o pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$SCRIPT_DIR/../.." && pwd)
BENCH_DIR="$ROOT/benchmark"
WORK_DIR="$BENCH_DIR/work"
RESULTS_DIR="$BENCH_DIR/results"

export PATH="$ROOT/.local_install/bin:$PATH"
export CLASSPATH="$ROOT/.local_install/lib/opensourcecobol4j/libcobj.jar:."

# ベンチ毎の反復回数 (この環境で1回あたり概ね2〜8秒になるよう調整) と計測回数
BENCHES=(
    "bench_empty    1       5"
    "bench_comp3    1000000 5"
    "bench_binary   1000000 5"
    "bench_disp     1000000 5"
    "bench_move     5000000 5"
    "bench_table    5000    5"
    "bench_string   1000000 5"
    "bench_call     2000000 5"
    "bench_seqfile  100000  3"
    "bench_idxfile  2000    3"
    "bench_sort     200000  3"
)

JAVA_OPTS="-Xmx512m"

mkdir -p "$WORK_DIR" "$RESULTS_DIR"

echo "=== COBOLベンチマークのコンパイル ==="
cd "$WORK_DIR"
cobj -o . "$BENCH_DIR"/cobol/*.cbl

for spec in "${BENCHES[@]}"; do
    read -r prog n runs <<< "$spec"
    outdir="$RESULTS_DIR/macro/$prog"
    mkdir -p "$outdir"
    rm -f "$outdir"/run*.json
    echo "=== $prog (BENCH_N=$n, ${runs}回計測) ==="

    # ファイル系ベンチの残骸を毎回掃除
    rm -f "$WORK_DIR"/bench_seq.dat "$WORK_DIR"/bench_idx.dat "$WORK_DIR"/bench_sort.tmp

    # ウォームアップ1回 (計測対象外)
    BENCH_N=$n java $JAVA_OPTS "$prog" > /dev/null

    # 本計測
    for i in $(seq 1 "$runs"); do
        rm -f "$WORK_DIR"/bench_seq.dat "$WORK_DIR"/bench_idx.dat "$WORK_DIR"/bench_sort.tmp
        BENCH_N=$n python3 "$SCRIPT_DIR/measure.py" "$outdir/run$i.json" \
            java $JAVA_OPTS "$prog" > /dev/null
    done

    # GCログ採取 (1回)
    rm -f "$WORK_DIR"/bench_seq.dat "$WORK_DIR"/bench_idx.dat "$WORK_DIR"/bench_sort.tmp
    BENCH_N=$n java $JAVA_OPTS "-Xlog:gc:file=$outdir/gc.log" "$prog" > /dev/null

    # JFRプロファイル採取 (1回)
    rm -f "$WORK_DIR"/bench_seq.dat "$WORK_DIR"/bench_idx.dat "$WORK_DIR"/bench_sort.tmp
    BENCH_N=$n java $JAVA_OPTS \
        "-XX:StartFlightRecording=settings=profile,filename=$outdir/profile.jfr" \
        "$prog" > /dev/null 2> /dev/null

    # jfr view による要約
    for view in hot-methods allocation-by-site allocation-by-class; do
        jfr view --width 120 "$view" "$outdir/profile.jfr" \
            > "$outdir/jfr-$view.txt" 2>/dev/null || true
    done
    echo "$n" > "$outdir/bench_n.txt"
done

echo "=== 集計 ==="
python3 "$SCRIPT_DIR/summarize.py" "$RESULTS_DIR"
echo "完了: $RESULTS_DIR/macro_summary.md を参照"

#!/usr/bin/env python3
"""マクロベンチマーク結果の集計。

results/macro/<bench>/run*.json と gc.log を読み、
macro_summary.csv / macro_summary.md を生成する。

GCログ(-Xlog:gc)からは GC回数・総停止時間・概算割り当て量を抽出する。
概算割り当て量 = Σ(GC前ヒープ使用量 - 前回GC後ヒープ使用量) + 最終使用量。
"""

import csv
import json
import re
import statistics
import sys
from pathlib import Path

# 例: [0.123s][info][gc] GC(0) Pause Young (Normal) (G1 Evacuation Pause) 24M->4M(256M) 1.234ms
GC_RE = re.compile(
    r"GC\((\d+)\) Pause (\w+).*?(\d+)M->(\d+)M\(\d+M\)\s+([\d.]+)ms"
)


def parse_gc_log(path):
    if not path.exists():
        return None
    count = 0
    pause_ms = 0.0
    alloc_mb = 0
    last_after = 0
    for line in path.read_text().splitlines():
        m = GC_RE.search(line)
        if not m:
            continue
        count += 1
        before, after, ms = int(m.group(3)), int(m.group(4)), float(m.group(5))
        pause_ms += ms
        alloc_mb += max(0, before - last_after)
        last_after = after
    return {"gc_count": count, "gc_pause_ms": round(pause_ms, 1),
            "alloc_mb_approx": alloc_mb}


def main():
    results_dir = Path(sys.argv[1])
    macro = results_dir / "macro"
    rows = []
    for bench_dir in sorted(macro.iterdir()):
        if not bench_dir.is_dir():
            continue
        runs = []
        for f in sorted(bench_dir.glob("run*.json")):
            runs.append(json.loads(f.read_text()))
        if not runs:
            continue
        walls = [r["wall_s"] for r in runs]
        rss = [r["maxrss_kb"] for r in runs]
        n = (bench_dir / "bench_n.txt").read_text().strip() \
            if (bench_dir / "bench_n.txt").exists() else "?"
        row = {
            "bench": bench_dir.name,
            "bench_n": n,
            "runs": len(runs),
            "wall_median_s": round(statistics.median(walls), 3),
            "wall_min_s": round(min(walls), 3),
            "wall_max_s": round(max(walls), 3),
            "user_median_s": round(
                statistics.median(r["user_s"] for r in runs), 3),
            "sys_median_s": round(
                statistics.median(r["sys_s"] for r in runs), 3),
            "maxrss_mb": round(max(rss) / 1024, 1),
        }
        gc = parse_gc_log(bench_dir / "gc.log")
        if gc:
            row.update(gc)
        rows.append(row)

    if not rows:
        print("結果がありません", file=sys.stderr)
        return

    keys = ["bench", "bench_n", "runs", "wall_median_s", "wall_min_s",
            "wall_max_s", "user_median_s", "sys_median_s", "maxrss_mb",
            "gc_count", "gc_pause_ms", "alloc_mb_approx"]

    csv_path = results_dir / "macro_summary.csv"
    with open(csv_path, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=keys)
        w.writeheader()
        for r in rows:
            w.writerow({k: r.get(k, "") for k in keys})

    md_path = results_dir / "macro_summary.md"
    with open(md_path, "w") as f:
        f.write("# マクロベンチマーク集計\n\n")
        f.write("| " + " | ".join(keys) + " |\n")
        f.write("|" + "---|" * len(keys) + "\n")
        for r in rows:
            f.write("| " + " | ".join(str(r.get(k, "")) for k in keys)
                    + " |\n")
    print(f"書き出し: {csv_path}\n書き出し: {md_path}")
    for r in rows:
        print(r)


if __name__ == "__main__":
    main()

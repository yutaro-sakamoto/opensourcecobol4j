# opensource COBOL 4J 性能ベンチマーク

COBOL→Java 変換後のプログラムとランタイム (`libcobj`) の性能を計測するための
ベンチマーク一式。分析結果は [`../doc/perf-report.md`](../doc/perf-report.md) を参照。

## 一発実行

```bash
./benchmark/run_all.sh
```

これだけで以下がすべて行われる。

1. **ローカルインストール**: `./configure --prefix=$PWD/.local_install && make && make install`
   （グローバル領域は汚さない。ビルド済みならスキップ。`FORCE_BUILD=1` で強制リビルド）
2. **マクロベンチマーク** (`scripts/run_macro.sh`):
   `cobol/*.cbl` をトランスパイルし、ベンチごとに
   - ウォームアップ1回 + 本計測3〜5回 → wall時間 / CPU時間 / 最大RSS（中央値などを集計）
   - GCログ採取 (`-Xlog:gc`) → GC回数・総停止時間・概算割り当て量
   - JFRプロファイル採取 (`-XX:StartFlightRecording=settings=profile`) →
     `jfr view hot-methods / allocation-by-site / allocation-by-class`
3. **JMHマイクロベンチマーク** (`scripts/run_jmh.sh`):
   `jmh/` の Maven プロジェクトをビルドし、libcobj のホットパス
   （`CobolDataStorage` のバイト操作、`CobolDecimal` 算術、MOVE、PERFORM の
   ディスパッチ機構）を現実装 vs 最適化代替実装で比較する。
   `SKIP_JMH=1` でスキップ可能（Maven Central への接続が必要）。

結果は `results/` に出力される。

| 出力 | 内容 |
|---|---|
| `results/macro_summary.md` / `.csv` | マクロベンチ集計（時間・RSS・GC） |
| `results/macro/<bench>/run*.json` | 各回の生計測値 |
| `results/macro/<bench>/gc.log` | GCログ |
| `results/macro/<bench>/profile.jfr`, `jfr-*.txt` | JFR記録と要約 |
| `results/jmh_result.txt` / `.json` | JMH結果 |

## ベンチマーク一覧

各 COBOL プログラムは環境変数 `BENCH_N` で反復回数を変更できる
（未設定時は各プログラム内のデフォルト値）。検算値を最後に DISPLAY する。

| プログラム | 対象 | 既定 BENCH_N (run_macro.sh) |
|---|---|---|
| `bench_empty` | JVM起動+ランタイム初期化のベースライン | 1 |
| `bench_comp3` | COMP-3 (パック10進) の COMPUTE/ADD | 1,000,000 |
| `bench_binary` | COMP-5 (2進) の算術 | 1,000,000 |
| `bench_disp` | USAGE DISPLAY (ゾーン10進) の算術 | 1,000,000 |
| `bench_move` | 英数字・集団項目の MOVE | 5,000,000 |
| `bench_table` | OCCURS 表の添字アクセス | 5,000 (×2050要素アクセス) |
| `bench_string` | STRING / UNSTRING / INSPECT | 1,000,000 |
| `bench_call` | サブプログラム CALL | 2,000,000 |
| `bench_seqfile` | 順ファイル WRITE/READ | 100,000 レコード |
| `bench_idxfile` | 索引ファイル (SQLite) WRITE/READ/START | 2,000 レコード |
| `bench_sort` | SORT (RELEASE/RETURN) | 200,000 レコード |

## 個別実行の例

```bash
export ROOT=$(pwd)                       # リポジトリルートで
export PATH=$ROOT/.local_install/bin:$PATH
export CLASSPATH=$ROOT/.local_install/lib/opensourcecobol4j/libcobj.jar:.
cd benchmark/work
cobj -o . ../cobol/bench_comp3.cbl
BENCH_N=1000000 java bench_comp3                    # 実行
BENCH_N=1000000 java -Xlog:gc bench_comp3           # GCログ付き
BENCH_N=1000000 java \
  -XX:StartFlightRecording=settings=profile,filename=p.jfr bench_comp3
jfr view hot-methods p.jfr                          # プロファイル閲覧
```

## 計測環境に関する注意

- 本ベンチは RAM 約3GB の環境を想定し、ヒープを `-Xmx512m`（JMH は `-Xmx256m`）に
  制限している。環境が異なる場合は `scripts/run_macro.sh` の `JAVA_OPTS` を調整する。
- `bench_idxfile` は索引ファイルが SQLite ベースのため突出して時間がかかる
  （これ自体が主要な性能課題。レポート参照）。

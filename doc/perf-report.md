# opensource COBOL 4J 性能分析レポート

COBOL→Java 変換後のプログラムおよびランタイム (`libcobj`) の性能を、
(1) 多様な COBOL プログラムをトランスパイルした実測ベンチマークと
(2) ランタイムソースコードの解析
の2つのアプローチで分析し、性能が悪い箇所を優先度付きで一覧化したレポート。

すべての計測は `benchmark/run_all.sh` の**スクリプト一発で再現できる**
（手順詳細は [`../benchmark/README.md`](../benchmark/README.md) を参照）。

- 対象コミット: `1e8b7f3` (develop)
- 計測環境: 13th Gen Intel Core i5-1334U (12 論理コア) / RAM 約3GB / Linux 6.12 /
  OpenJDK 21.0.9 (G1GC, `-Xmx512m`)
- opensource COBOL 4J はリポジトリ直下の `.local_install/` にローカルインストールして使用

## 1. 計測方法

| 手法 | ツール | 取得指標 |
|---|---|---|
| マクロベンチマーク | `benchmark/cobol/*.cbl` (11本) を cobj で変換・実行 | wall時間(5回・ファイル系は3回の中央値)、CPU時間、最大RSS、GC回数/総停止時間/概算割り当て量 (`-Xlog:gc`) |
| CPU/割り当てプロファイル | JFR (`-XX:StartFlightRecording=settings=profile`) + `jfr view` | ホットメソッド、クラス別割り当て圧 |
| マイクロベンチマーク | JMH 1.37 (`benchmark/jmh/`) | ランタイムホットパスの ns/op。**現実装 vs 最適化代替実装**の対比で期待効果を定量化 |

ベンチマークは COBOL の主要言語要素を網羅する:
パック10進(COMP-3)・2進(COMP-5)・ゾーン10進(DISPLAY)の算術、MOVE、OCCURS表アクセス、
STRING/UNSTRING/INSPECT、CALL、順ファイル、索引ファイル、SORT。
各プログラムは検算値を出力し正しさを確認している。

## 2. マクロベンチマーク結果

`benchmark/results/macro_summary.md` より（wall は5回(ファイル系は3回)の中央値）:

| ベンチ | 反復回数 | wall中央値 | 最大RSS | GC回数 | GC停止 | 概算割り当て | 1反復あたり※ |
|---|---:|---:|---:|---:|---:|---:|---:|
| bench_empty (起動のみ) | 1 | 0.15s | 46MB | 0 | 0ms | ~0 | - |
| bench_comp3 (COMP-3算術) | 100万 | 1.33s | 246MB | 32 | 57ms | 2,799MB | 1.19µs / **2.8KB割り当て** |
| bench_binary (COMP-5算術) | 100万 | 1.28s | 240MB | 43 | 62ms | 3,218MB | 1.14µs / 3.2KB割り当て |
| bench_disp (ゾーン10進算術) | 100万 | 1.80s | 228MB | 42 | 70ms | 3,412MB | 1.65µs / 3.4KB割り当て |
| bench_move (MOVE×5/反復) | 500万 | 0.61s | 81MB | 3 | 7ms | 76MB | 18ns/MOVE |
| bench_table (OCCURS表) | 5千×2050要素 | 1.24s | 298MB | 33 | 68ms | 3,811MB | ~110ns/要素アクセス |
| bench_string (STRING/INSPECT等) | 100万 | 1.44s | 137MB | 15 | 42ms | 548MB | 1.29µs |
| bench_call (CALL) | 200万 | 0.80s | 239MB | 17 | 36ms | 1,129MB | 324ns / **565B割り当て**/CALL |
| bench_seqfile (順ファイル) | 10万件 | 4.44s | 79MB | 11 | 27ms | 284MB | **43µs/レコード** (sys 2.5s) |
| bench_idxfile (索引ファイル) | 2千件 | **17.3s** | 104MB | 2 | 10ms | 50MB | **約8.6ms/レコード** |
| bench_sort (SORT) | 20万件 | 3.90s | 324MB | 30 | 119ms | 2,192MB | 19µs/レコード |

※ 1反復あたりの値は起動時間 0.15s を差し引いて算出。数値は `run_all.sh` 最終実行時の
`benchmark/results/macro_summary.csv` と一致する（実行ごとに±10%程度の変動がある）。

**読み取れること**

1. **索引ファイルが桁違いに遅い**。2,000件の WRITE + ランダムREAD + スキャンに17秒
   (CPU時間は4.5秒のみ = 大半がディスク同期待ち。実行によっては47秒に達した)。
2. **計算系は1反復(COMPUTE+ADD+IF程度)あたり2.8〜3.4KBもヒープ割り当てする**。
   100万回のループで2.8〜3.8GBを割り当て、GCが数十回発生。単純なバッチでもRSSが250MB前後まで膨らむ。
3. 順ファイルは43µs/レコードで、sys時間が wall の過半を占める(システムコール過多)。
4. JVM起動+ランタイム初期化は約0.15秒で、短命プログラムの多重起動以外では問題にならない。

## 3. プロファイル結果 (JFR)

各ベンチの上位ホットメソッド / 割り当て元 (`benchmark/results/macro/*/jfr-*.txt`)。
比率はサンプリングのため実行ごとに±10pt程度変動するが、顔ぶれは安定している:

| ベンチ | CPU上位 | 割り当て圧上位 |
|---|---|---|
| bench_comp3 | `Long.getChars` 21%, `BigDecimal.valueOf` 19%, `BigDecimal.<init>(char[])` 8% | int[] 35%, HeapByteBuffer 32%, byte[]+BigDecimal 28% (いずれもBigDecimal/変換由来) |
| bench_disp | BigInteger 系 (`trustedStripLeadingZeroInts`/`shiftLeft`等) 計30%超, `Long.getChars` 8% | **BigInteger 45% + BigDecimal 23%**, int[] 21% |
| bench_table | **`BigDecimal.valueOf` 25%**, `BigInteger.shiftRight` 12%, `CobolDecimal.set(int)` 10% | int[] 41%, BigDecimal 31%, **HeapByteBuffer 23%** |
| bench_string | **`CobolInspect.common` 65%**, `CobolString.unstringInto` 6% | HeapByteBuffer 41%, int[] 37% |
| bench_move | **`ByteBuffer.wrap`+`limit` 計68%**, `moveAlphanumToAlphanum` 24% | **HeapByteBuffer 96%** |
| bench_call | `ByteBuffer` 系 29%, 数値変換系 (`storeCommonRegion`/`moveDisplayToBinary`) 29% | HeapByteBuffer 64%, **CobolModule 31%** |
| bench_seqfile | `FileChannelImpl.read` 31%, **`FileIO.getc` 23%** | HeapByteBuffer 47%, byte[] 40% |
| bench_sort | `Long.getChars` 13%, `numericCompareTo`+`compare` 13%, BigDecimal系 | byte[] 31%, int[] 27%, BigDecimal 10% |
| bench_idxfile | (CPUサンプル極小 = I/O待ちが支配的) | byte[] 21% ほか少量 |

## 4. JMHマイクロベンチマーク結果

`benchmark/results/jmh_result.txt` より (avgt, ns/op)。
「現実装」はランタイムの実コード、「代替」は同じ結果を返す最適化実装:

| 対象 | 現実装 | 最適化代替 | 倍率 |
|---|---:|---:|---:|
| COMPUTE AMT=PRICE×QTY×RATE (COMP-3, 生成コード列を再現) | 200 ns | 1.0 ns (スケール済みlong演算) | **約200倍** |
| ADD AMT TO TOTAL (ゾーン10進) | 214 ns | 0.64 ns (long加算) | **約330倍** |
| `CobolDataStorage.memcpy` 1000B | 180 ns | 16 ns (`System.arraycopy`) | **約11倍** |
| `CobolDataStorage.memcpy` 100B | 25 ns | 5.2 ns | 4.8倍 |
| `CobolDataStorage.memset` 100B | 13 ns | 8.3 ns (`Arrays.fill`) | 1.5倍 (1000Bではほぼ同等) |
| `CobolDataStorage.memcmp` 全バイト一致 1000B | 316 ns | 31 ns (`Arrays.mismatch`) | **約10倍** |
| `CobolDataStorage.memcmp` 先頭バイト不一致 | 1.8 ns | 2.5 ns | 現実装で十分 |
| PERFORM 1段落のディスパッチ | 2.3 ns | 0.49 ns (直接呼び出し) | 4.7倍 |
| パック10進の `getDecimal()` | 15 ns | - | (BigDecimal生成を含む) |
| ゾーン10進の `getInt()` | 9.7 ns | - | |
| ゾーン10進の `getString()` (DISPLAY経路) | 53 ns | - | |
| MOVE X(100)→X(100) | 27 ns | - | |
| MOVE ゾーン10進→パック / →2進 | 46 / 37 ns | - | |

(注: JMH 単体では JIT のエスケープ解析が効きやすく、実プログラムよりも好条件になる。
実プログラムでの割り当て圧は §2-3 の GC/JFR 計測が裏付ける。)

## 5. 優先度付き性能課題一覧

優先度は「インパクト(ホットパス度 × 測定された劣化量) ÷ 改修コスト」で評定。
改修規模: 小=数十行・局所的 / 中=数百行・単一モジュール / 大=複数モジュール横断。

---

### 【P1】索引ファイル: 1操作ごとの SQLite コミット (fsync)

- **症状**: WRITE/READ 1件あたり約9ms。2,000件の処理に17秒 (CPUはわずか4.5秒で、
  残りはディスク同期待ち)。数十万件規模のバッチでは時間単位の遅延になり実用上の障害となる。
- **原因**: `libcobj/.../file/CobolIndexedFile.java`
  - `write_()` (1083行) は `commitOnModification` (既定 true, 39行) により**1レコードごとに `connection.commit()`**を実行。SQLite の既定設定 (journal=DELETE, synchronous=FULL) ではコミットごとに複数回の fsync が走る。
  - **READ 系でも**ロック解放のため `read_()` (755行)・`readNext()` (897行) が毎回 `commit()` を呼ぶ。
  - レコードロックの確認/解放 (`checkOtherProcessLockedRecord`/`lockRecord`) も毎操作SQLを実行。
- **改修案・規模**:
  1. (小) 接続初期化 (280行付近) で `PRAGMA journal_mode=WAL; PRAGMA synchronous=NORMAL` を設定 — 互換性を保ったまま fsync 回数を大幅削減。
  2. (中) コミットをバッチ化: N操作ごと/CLOSE時にコミット (`commitOnModification=false` の経路は既にあるため、トランザクション境界の設計と異常時の整合性確認が主作業)。
  3. (中) 単一プロセス利用時 (`COB_FILE_LOCK` 不要時) はロックテーブル操作を省略。
- **期待効果**: バルク処理で **10〜100倍** (fsync ~1ms/件 → メモリ内コミット µs オーダー)。
  本レポート中、最も費用対効果が高い。

### 【P2】10進演算: BigDecimal + 文字列経由の変換

- **症状**: COMPUTE 1文あたり200ns (等価なlong演算の約200倍)、ゾーン10進の ADD は300倍超。
  計算ループ1反復で2.8〜3.4KB割り当てし、その大半が BigDecimal/BigInteger と
  数値⇔文字列変換の中間オブジェクトに由来する。
- **原因**: すべての算術が `CobolDecimal` = `BigDecimal` ラッパー経由で、さらに
  **フィールド⇔数値の変換が文字列を往復する**:
  - 読み出し: `AbstractCobolField.getDecimal()` (200行) はゾーン10進を `char[]` に展開して
    `new BigDecimal(char[])` で**文字列パース**。パック10進 (`CobolNumericPackedField.getDecimal()` 469行) も同様。
  - 書き戻し: `CobolDecimal.getDisplayField()` (576行)・`getPackedField()` (623行) は
    `BigDecimal.toPlainString()` → `String.getBytes(SJIS)` (CobolNumericPackedField 188行) と
    **文字列化+文字コード変換**してから桁を書く。JFR で `Long.getChars` が CPU 21% (comp3) を
    占めるのはこれが原因。
  - `getField()` (438行) は毎回 `new CobolDecimal(this)` の防御的コピーを生成。
  - 比較も同様: `AbstractCobolField.cmpInteger()` (430行) は**ループ条件 `I > N` の判定のたびに
    CobolDecimal×2 + BigDecimal を生成**。表アクセスベンチの割り当て3.8GBの主因。
- **改修案・規模**:
  1. (小〜中) `cmpInteger`/`cmpInt` の long 高速パス (≦18桁はフィールドから直接 long にデコードして比較)。全プログラムのループ条件で効く。
  2. (中) `getDecimal`/`getField` の文字列往復を廃止し、桁⇔long/BigDecimal を直接変換。
  3. (大) `CobolDecimal` に long ベースの高速パス (値とスケールが long に収まる間は BigDecimal を生成しない) を実装。C版 opensource COBOL も同様の2段構え。
- **期待効果**: 計算主体の処理で **2〜10倍** (理論上限は200倍だが、まず文字列往復と
  ループ条件の排除で2〜5倍、long化完遂で10倍超)。GC圧は90%以上削減見込み。
  SORT のキー比較 (`numericCompareTo`, P9) もこの改善で同時に速くなる。

### 【P3】順ファイル READ: 1バイトごとの read システムコール

- **症状**: 順ファイル 43µs/レコード、sys 時間が wall の過半 (2.5s/4.4s)。
  JFR では `FileChannelImpl.read` + `FileIO.getc` で50%超、割り当ては HeapByteBuffer 47% +
  byte[] 40%。
- **原因**: `libcobj/.../file/FileIO.java` — 行読みが `getc()` (309行) ベースで、
  読みバッファ機構は実装済みなのに **`USE_READ_BUFFER = false` (41行) で無効化**されている。
  無効時は**1バイトごとに `new byte[1]` + `ByteBuffer.wrap` + `FileChannel.read()`** (システムコール) が走る。
- **改修案・規模**: (小) `USE_READ_BUFFER=true` を有効化し、既存バッファ実装の正しさ
  (SEEK/巻き戻し・EOF境界・複数ファイル) をテストで確認して修正。書き込み側は既にバッファ済み。
- **期待効果**: READ フェーズ **5〜20倍**。順ファイル全体 (読み書き半々) で2〜3倍。

### 【P4】MOVE/初期化: `memcpy`/`memset` の1バイトループ

- **症状**: JMH 実測で 1000バイトの `memcpy` が `System.arraycopy` の **約11倍** 遅い (180ns vs 16ns)。
  比較 (`memcmp`) も**全バイト一致時は約10倍** 遅い (316ns vs 31ns @1000B。COBOL では
  等値比較・キー一致判定で「一致 = 最後まで走査」が頻出するため実害がある)。
- **原因**: `libcobj/.../data/CobolDataStorage.java` — `memcpy` (284行)・`memset` (364行)・
  `memcmp` (411行) ほか多数のオーバーロードが `for` ループで `setByte`/`getByte` を
  1バイトずつ呼ぶ。MOVE・INITIALIZE・比較・集団項目コピー・ファイルレコード転送など
  至る所で使われる。
- **改修案・規模**: (小) `System.arraycopy`/`Arrays.fill`/`Arrays.mismatch` への置き換え。
  十数メソッドの機械的修正で、既存ユニットテストで検証可能。最初に着手すべきクイックウィン。
  なお `memset` は JIT がバイトループをベクトル化するため 1000B では `Arrays.fill` と
  同等以上 (効果があるのは小サイズのみ)。置き換えの主目的は memcpy と memcmp。
- **期待効果**: 大きめの項目・集団項目の MOVE が **4〜11倍**、一致する比較が **最大10倍**。
  プログラム全体では数%〜数十% (レコード長の大きいバッチほど効果大)。

### 【P5】2進項目アクセス: 毎回の `ByteBuffer.wrap` 割り当て

- **症状**: 表アクセスベンチで HeapByteBuffer が割り当て圧の23%、MOVEベンチでは96%。
  MOVEベンチでは `ByteBuffer.wrap`/`limit` だけで CPU の68%を占める。
- **原因**: `CobolDataStorage.set(short/int/long)` (786〜805行) や各種 `getSwp*`/`setSwp*` が
  **アクセスのたびに `ByteBuffer.wrap(...)` で一時オブジェクトを生成**する。
  COMP/COMP-5 項目の読み書き、添字計算等の最頻出経路。
- **改修案・規模**: (小〜中) ビット演算による直接読み書き、または `VarHandle`
  (`MethodHandles.byteArrayViewVarHandle`) 化。`CobolDataStorage` 内で完結。
- **期待効果**: 2進項目中心の処理で **1.3〜2倍**、割り当て圧の大幅削減。

### 【P6】INSPECT: バイト単位の素朴な走査

- **症状**: STRING/UNSTRING/INSPECT ベンチで `CobolInspect.common` が **CPU 65%**。
- **原因**: `libcobj/.../common/CobolInspect.java` — `common()` (65行) が全位置×全パターンの
  二重ループで `memcmp` を繰り返し、マーク配列を毎回更新する。呼び出しごとに
  バッファ (`new byte[]`) も確保。
- **改修案・規模**: (中) 1文字パターン (実務で大半を占める `INSPECT ... REPLACING ALL 'x'` 等) の
  特化パス、マーク配列の再利用、TALLYING のみの場合のマーク省略。
- **期待効果**: INSPECT 多用処理で **2〜5倍**。

### 【P7】CALL: 呼び出しごとの `CobolModule` ほか一式の生成

- **症状**: CALL 1回 324ns・約565バイト割り当て。割り当て圧の31%が CobolModule
  (実行によっては50%超)。
- **原因**: 生成コードの `run_module()` は呼び出しのたびに
  `this.module = new CobolModule(null, null, ..., null)` (13引数) を生成し
  (cobj/codegen.c が出力、例: 生成された `bench_sub.java` 59行)、moduleスタックへ
  push/pop する。モジュール内容は定数なのに毎回作り直している。
- **改修案・規模**: (小) codegen を変更しインスタンスフィールドで遅延生成・再利用
  (初回のみ new)。ランタイム変更不要。
- **期待効果**: CALL 集約型で **1.2〜2倍**、GC圧削減。

### 【P8】制御フロー: PERFORM ごとの匿名クラス生成と Optional 連鎖

- **症状**: PERFORM 1回のディスパッチが直接呼び出しの約4.7倍 (2.3ns vs 0.49ns)。
  段落遷移ごとに `Optional.of` などの割り当てが発生する。
- **原因**: 生成コードは段落遷移を `Optional<CobolControl>` で返し、
  `CobolControl.perform()` (`libcobj/.../common/CobolControl.java` 139行) は
  **呼び出しごとに匿名 CobolControl を new** する (107行 `performThrough`)。
  GO TO を実現する柔軟な機構だが、定数的なオブジェクトを毎回生成している。
- **改修案・規模**: (中) `perform(contList, id)` の返すオブジェクトを contList 側にキャッシュ、
  `Optional` を null 許容の直接参照か int インデックスに置き換え (codegen + ランタイム両方の変更)。
- **期待効果**: 全プログラムに薄く効く **5〜15%**。P2 ほどの優先度はない。

### 【P9】SORT: キー比較のたびの数値デコード

- **症状**: 19µs/レコード。比較器 (`CobolFileSort$1.compare`) と `numericCompareTo`、
  BigDecimal 生成がプロファイル上位。20万件で2.2GB割り当て。
- **原因**: マージソートの比較のたびに数値キーを `getDecimal()` (= BigDecimal生成、P2と同根) で
  デコードして比較する。n log n 回繰り返される。
- **改修案・規模**: P2-1 (比較の long 高速パス) で大半が解決。追加で (中) ソートキーの
  事前デコード (decorate-sort-undecorate) も可能。
- **期待効果**: SORT 処理で **1.5〜3倍**。

### 【P10】DISPLAY/文字列化: 1文字ずつの StringBuilder と二重文字コード変換

- **症状**: 数値項目の `getString()` 53ns〜、DISPLAY のたびに SJIS→UTF-8 の二重変換と
  中間 byte[] 生成 (`CobolTerminal.display`)。
- **原因**: `CobolNumericField.getString()` が1文字ずつ `StringBuilder.append`、
  `CobolTerminal` が `new String(bytes, SJIS).getBytes(UTF-8)` を毎回実行。
- **改修案・規模**: (小) 出力ストリームのバッファリングと変換結果の直接書き出し。
- **期待効果**: DISPLAY 多用 (ログ出力的な使い方) のプログラムのみ顕著。優先度低。

---

### 優先度マトリクス (まとめ)

| 優先度 | 課題 | 測定根拠 | 改修規模 | 期待効果 |
|---|---|---|---|---|
| **P1** | 索引ファイルのレコード毎コミット | 約9ms/件 | 小〜中 | **10〜100倍** (索引ファイル処理) |
| **P2** | BigDecimal+文字列経由の10進演算 | COMPUTE 200ns vs 1.0ns / 割り当て2.8KB/反復 | 小(部分)〜大(全体) | **2〜10倍** (計算処理) |
| **P3** | 順ファイルREADのバイト毎syscall | sys 2.5s / READ系 50%超 | 小 | **5〜20倍** (READ) |
| **P4** | memcpy/memset/memcmpのバイトループ | memcpy約11倍・一致memcmp約10倍差 (1000B) | 小 | 4〜11倍 (大きいMOVE/比較) |
| **P5** | 2進項目のByteBuffer.wrap割り当て | 割り当て23〜96% | 小〜中 | 1.3〜2倍 |
| **P6** | INSPECTの素朴な走査 | CPU 65% (文字列処理) | 中 | 2〜5倍 (INSPECT) |
| **P7** | CALL毎のCobolModule生成 | 割り当て31% | 小 | 1.2〜2倍 (CALL) |
| **P8** | PERFORM/Optionalディスパッチ | 約4.7倍差 | 中 | 5〜15% (全体) |
| **P9** | SORT比較の数値デコード | CPU上位 | P2と共通 | 1.5〜3倍 (SORT) |
| **P10** | DISPLAY二重変換 | - | 小 | DISPLAY多用時のみ |

### 改修ロードマップ案

1. **クイックウィン (規模小・リスク低、まとめて数日規模)**
   - P4: `memcpy`/`memset`/`memcmp` → `System.arraycopy`/`Arrays.fill`/`Arrays.mismatch`
   - P3: 読みバッファ有効化 (`USE_READ_BUFFER`)
   - P1-1: SQLite PRAGMA (WAL/synchronous) 調整
   - P7: CALL毎の `CobolModule` 再利用 (codegen 1箇所)
   - P2-1: `cmpInteger` 等比較系の long 高速パス
2. **中期 (規模中)**
   - P1-2: 索引ファイルのトランザクションバッチ化
   - P5: 2進アクセスの VarHandle 化
   - P6: INSPECT の特化パス
   - P2-2: フィールド⇔数値変換の文字列往復排除
3. **大規模 (設計判断が必要)**
   - P2-3: `CobolDecimal` の long 高速パス導入 (≦18桁)
   - P8: 制御フロー機構の Optional 排除 (codegen 互換性に注意)

## 6. 補足

- **メモリ消費**: 最大RSSはヒープ割り当て churn に支配されており (計算系で250〜320MB)、
  P2/P5/P7 の割り当て削減がそのまま footprint 削減につながる。リーク兆候は観測されない。
- **起動時間**: 約0.15s (JVM+ランタイム初期化)。バッチ多重起動の用途では JVM 常駐化や
  AppCDS が別途有効だが、本レポートの範囲外。
- **memcmp の遅さは比較結果に依存する**: 先頭バイトで不一致になるケースは現実装でも
  1.8ns と十分速いが、**全バイト一致するケース (等値比較で頻出) は `Arrays.mismatch` 比で
  約10倍遅い**。P4 の置き換え対象に含めるべき (JMH の memcmp_current_equal / _mismatch で確認可能)。
- **付随して見つかった正しさの問題**: `AbstractCobolField.moveFrom(String)` を数値項目
  (ゾーン/パック10進) に対して呼ぶと、数値変換されず文字列の生バイトがそのまま格納される
  (例: パック項目に `"123.45"` → 値は `+3132332.>3`)。生成コードは数値リテラルを定数
  フィールド経由で MOVE するため通常は顕在化しないが、ランタイムを直接利用する
  Java コードからは踏みやすい。性能とは別件のため issue 化を推奨。
- **再現方法**: `./benchmark/run_all.sh` 一発。生データは `benchmark/results/` 配下
  (各ベンチの run*.json / gc.log / profile.jfr / jfr-*.txt、JMH の jmh_result.json)。

(計測日: 2026-06-10)

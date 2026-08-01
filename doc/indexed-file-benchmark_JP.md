# INDEXEDファイルのジャーナルモードによる性能差の計測

INDEXEDファイルはSQLiteをバックエンドとしている。opensource COBOL 4J 2.1.0 以降は既定でWAL(ライトアヘッドログ)ジャーナルモードを使用し、
`COB_INDEXED_JOURNAL_MODE=DELETE` を指定すると以前のバージョンと同じロールバックジャーナルの動作に戻る
([環境変数リファレンス](./environment_variables_JP.md) を参照)。

この文書では、その変更が自分のワークロードにとってどれだけの効果があるかを、手元の環境で確認するためのベンチマークを示す。
必要なものはインストール済みの opensource COBOL 4J だけである。`sqlite3` コマンドは任意で、実際にどちらのモードが有効だったかを確認するためだけに使う。

## 計測する内容

主キー、重複を許す副キー、40バイトの本体(1レコード50バイト)を持つ1つのINDEXEDファイルに対して、次の3つのフェーズを実行する。

| フェーズ | ワークロード | 見たいもの |
| --- | --- | --- |
| W1 | `OPEN OUTPUT`、N回の `WRITE`、`CLOSE` | 1つの `WRITE` ごとに1コミット。WALの効果が最も大きいケース |
| W2 | `OPEN I-O`、N回の (`READ` + `REWRITE`)、`CLOSE` | 1文の中で読んでから書くケース |
| W3 | `OPEN INPUT`、N回の `READ`、`CLOSE` | 読み取り専用。退行していないことの確認 |

キーは順番どおりではなく散らばった順序で辿るため、アクセスパターンがページキャッシュに対して不自然に有利にはならない。

各フェーズの時間はプログラム自身が `ACCEPT ... FROM TIME` で計測するので、シェルの計時コマンドを使う必要がなく、結果にJVMの起動時間も含まれない。
また、0以外の `FILE STATUS` をすべて数えて最後に表示する。処理が黙って行われなかった場合、そうしないと無意味な高速化として報告されてしまうためである。

## 実行方法

`CLASSPATH` に `libcobj.jar` を設定する(パスは環境に合わせること)。

```bash
export CLASSPATH=.:/usr/lib/opensourcecobol4j/libcobj.jar
```

以下のプログラムを `idxbench.cbl` として保存する。

```cobol
       IDENTIFICATION              DIVISION.
       PROGRAM-ID.                 idxbench.
       ENVIRONMENT                 DIVISION.
       INPUT-OUTPUT                SECTION.
       FILE-CONTROL.
       SELECT BENCH-FILE ASSIGN    "bench.dat"
           ORGANIZATION IS         INDEXED
           ACCESS MODE  IS         RANDOM
           RECORD KEY   IS         B-KEY
           ALTERNATE RECORD KEY IS B-ALT WITH DUPLICATES
           FILE STATUS  IS         B-STATUS.
       DATA                        DIVISION.
       FILE SECTION.
       FD  BENCH-FILE.
       01  B-REC.
           03  B-KEY               PIC  X(05).
           03  B-ALT               PIC  X(05).
           03  B-PAYLOAD           PIC  X(40).
       WORKING-STORAGE             SECTION.
       01  B-STATUS                PIC  99.
       01  WS-ENV                  PIC  X(10).
       01  WS-COUNT                PIC  9(05) VALUE 20000.
       01  WS-I                    PIC  9(05).
       01  WS-J                    PIC  9(05).
       01  WS-ERRORS               PIC  9(05) VALUE 0.
       01  WS-TIME.
           03  WS-HH               PIC  9(02).
           03  WS-MM               PIC  9(02).
           03  WS-SS               PIC  9(02).
           03  WS-CC               PIC  9(02).
       01  WS-T0                   PIC  9(08).
       01  WS-T1                   PIC  9(08).
       01  WS-DISP                 PIC  ZZZZZ9.99.
       01  WS-LABEL                PIC  X(24).
       PROCEDURE                   DIVISION.
       MAIN-RTN.
      *    The record count can be overridden with BENCH_RECORDS.
           MOVE SPACES TO WS-ENV.
           ACCEPT WS-ENV FROM ENVIRONMENT "BENCH_RECORDS".
           IF WS-ENV NOT = SPACES
               MOVE FUNCTION NUMVAL(WS-ENV) TO WS-COUNT
           END-IF.
           DISPLAY "records                  " WS-COUNT.

           MOVE "W1 WRITE (OPEN OUTPUT)" TO WS-LABEL.
           PERFORM MARK-START.
           OPEN OUTPUT BENCH-FILE.
           PERFORM VARYING WS-I FROM 1 BY 1 UNTIL WS-I > WS-COUNT
               PERFORM BUILD-RECORD
               WRITE B-REC
               PERFORM CHECK-STATUS
           END-PERFORM.
           CLOSE BENCH-FILE.
           PERFORM MARK-END.

           MOVE "W2 REWRITE (OPEN I-O)" TO WS-LABEL.
           PERFORM MARK-START.
           OPEN I-O BENCH-FILE.
           PERFORM VARYING WS-I FROM 1 BY 1 UNTIL WS-I > WS-COUNT
               PERFORM BUILD-RECORD
               READ BENCH-FILE
               PERFORM CHECK-STATUS
               MOVE "rewritten"  TO B-PAYLOAD
               REWRITE B-REC
               PERFORM CHECK-STATUS
           END-PERFORM.
           CLOSE BENCH-FILE.
           PERFORM MARK-END.

           MOVE "W3 READ (OPEN INPUT)" TO WS-LABEL.
           PERFORM MARK-START.
           OPEN INPUT BENCH-FILE.
           PERFORM VARYING WS-I FROM 1 BY 1 UNTIL WS-I > WS-COUNT
               PERFORM BUILD-RECORD
               READ BENCH-FILE
               PERFORM CHECK-STATUS
           END-PERFORM.
           CLOSE BENCH-FILE.
           PERFORM MARK-END.

           DISPLAY "non-zero file statuses   " WS-ERRORS.
           STOP RUN.

      *    Spread the keys so that the access pattern is not purely
      *    sequential. 7919 is prime, so this visits every key once.
       BUILD-RECORD.
           COMPUTE WS-J = FUNCTION MOD (WS-I * 7919, WS-COUNT) + 1.
           MOVE WS-J TO B-KEY.
           MOVE WS-I TO B-ALT.
           MOVE "payload"  TO B-PAYLOAD.

      *    A benchmark that silently skipped its work would report a
      *    meaningless speed-up, so every status is checked.
       CHECK-STATUS.
           IF B-STATUS NOT = 00
               ADD 1 TO WS-ERRORS
           END-IF.

      *    ACCEPT FROM TIME yields HHMMSScc. FUNCTION CURRENT-DATE is
      *    not usable here: it is captured once at program start-up and
      *    never advances during the run.
       MARK-START.
           ACCEPT WS-TIME FROM TIME.
           COMPUTE WS-T0 = WS-HH * 360000 + WS-MM * 6000
                         + WS-SS * 100 + WS-CC.

       MARK-END.
           ACCEPT WS-TIME FROM TIME.
           COMPUTE WS-T1 = WS-HH * 360000 + WS-MM * 6000
                         + WS-SS * 100 + WS-CC.
           IF WS-T1 < WS-T0
      *        The phase crossed midnight; add 24 hours.
               COMPUTE WS-T1 = WS-T1 + 8640000
           END-IF.
           COMPUTE WS-DISP = (WS-T1 - WS-T0) / 100.
           DISPLAY WS-LABEL " " WS-DISP " s".
```

一度コンパイルし、2つのモードを交互に3回ずつ実行する。
片方のモードをまとめて実行するのではなく交互に実行することで、温度やページキャッシュの状態の変化が両者に均等にかかるようにしている。

```bash
cobj idxbench.cbl

for i in 1 2 3; do
  for mode in WAL DELETE; do
    rm -f bench.dat bench.dat-wal bench.dat-shm
    echo "===== run $i, $mode ====="
    COB_INDEXED_JOURNAL_MODE=$mode BENCH_RECORDS=20000 java idxbench
    sqlite3 bench.dat 'pragma journal_mode;'   # 任意: モードの確認
  done
done
```

各モードについて3回の中央値を採る。レコード件数は `BENCH_RECORDS` で変更できる。`DELETE` の実行に時間がかかりすぎる場合は減らすとよい。

## 対照群: `cobj-idx load`

`cobj-idx load` はロード全体を1つのトランザクションで囲むため、ジャーナルモードによらずコミットは1回だけである。
両方のモードで実行してみると有用な裏付けになる。W1やW2では差が出るのにこちらでは差が出ないのであれば、
高速化は確かに文ごとのコミットに由来しており、両モードの他の性質によるものではないことが分かる。

```bash
# 20000件の固定長50バイトレコード。改行は入れない(cobj-idx load の既定はCOBOLのSEQUENTIAL)
python3 -c "
with open('load.bin','w') as f:
    for i in range(1, 20001):
        f.write('%05d%05d%-40s' % (i, i, 'payload'))
"

for mode in WAL DELETE; do
  rm -f loadbench.dat loadbench.dat-wal loadbench.dat-shm
  cobj-idx create loadbench.dat --size=50 --key=1,5
  echo "== $mode =="
  COB_INDEXED_JOURNAL_MODE=$mode /usr/bin/time -f '%e s' cobj-idx load loadbench.dat load.bin
done
```

## 結果の読み方

- `synchronous` は両モードとも SQLite の既定値 (`FULL`) のままである。したがってこれは耐久性を落として速度を得た比較ではなく、ジャーナルモードだけを揃えて比べたものである。
- 高速化はコミットごとのロールバックジャーナルの作成・fsync・削除を避けることによる。そのため W1 と W2 で効果が最も大きく、読み取りのみの W3 ではほとんど差が出ない。
- `cobj-idx load` はロード全体を1つのトランザクションで囲むため、ジャーナルモードによらずコミットは1回だけであり、ほとんど差が出ない。
  これは想定どおりの結果であり、高速化が本当に文ごとのコミットに由来していることの裏付けにもなる。
- ネットワークファイルシステム上での結果は意味を持たない。WALは共有メモリを必要とし、そこでは動作しないためである。その場合は `COB_INDEXED_JOURNAL_MODE=DELETE` を使うこと。
- 絶対値はストレージのfsyncの扱いに大きく依存する。書き込みキャッシュを持つSSDと、バリアが有効なハードディスクとでは1桁以上異なることがある。

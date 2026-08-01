# Measuring the Effect of the INDEXED File Journal Mode

INDEXED files are backed by SQLite. Since opensource COBOL 4J 2.1.0 they use the WAL
(write-ahead logging) journal mode by default, and `COB_INDEXED_JOURNAL_MODE=DELETE` restores the
rollback-journal behaviour of earlier versions (see
[Environment Variables Reference](./environment_variables.md)).

This page describes a self-contained benchmark you can run on your own machine to see what that
change is worth for your workload. It needs nothing but an installed opensource COBOL 4J; the
`sqlite3` command is optional and only used to confirm which mode was actually in effect.

## What it measures

The program runs three phases against one INDEXED file with a primary key, an alternate key with
duplicates, and a 40-byte payload (50 bytes per record).

| Phase | Workload | What it shows |
| --- | --- | --- |
| W1 | `OPEN OUTPUT`, N × `WRITE`, `CLOSE` | One commit per `WRITE` — the case WAL helps most |
| W2 | `OPEN I-O`, N × (`READ` + `REWRITE`), `CLOSE` | Read-then-write within each statement |
| W3 | `OPEN INPUT`, N × `READ`, `CLOSE` | Read-only; confirms reads do not regress |

Keys are visited in a scattered order rather than sequentially, so the access pattern is not
unrealistically friendly to the page cache.

The program times each phase itself with `ACCEPT ... FROM TIME`, so no shell timing utilities are
involved and the result does not include JVM start-up. It also counts every non-zero `FILE STATUS`
and prints the total: a benchmark that silently skipped its work would otherwise report a
meaningless speed-up.

## Running it

Set `CLASSPATH` to your `libcobj.jar` (adjust the path for your installation):

```bash
export CLASSPATH=.:/usr/lib/opensourcecobol4j/libcobj.jar
```

Save the program below as `idxbench.cbl`:

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

Compile it once, then run both modes alternately three times each. Alternating rather than running
all of one mode and then the other spreads any thermal or page-cache drift evenly across the two:

```bash
cobj idxbench.cbl

for i in 1 2 3; do
  for mode in WAL DELETE; do
    rm -f bench.dat bench.dat-wal bench.dat-shm
    echo "===== run $i, $mode ====="
    COB_INDEXED_JOURNAL_MODE=$mode BENCH_RECORDS=20000 java idxbench
    sqlite3 bench.dat 'pragma journal_mode;'   # optional: confirms the mode
  done
done
```

Take the median of the three runs for each mode. `BENCH_RECORDS` controls the record count; lower it
if the `DELETE` runs take uncomfortably long.

## The control: `cobj-idx load`

`cobj-idx load` wraps the entire load in a single transaction, so it commits once no matter which
journal mode is in effect. Running it both ways is a useful cross-check: if it shows no difference
while W1 and W2 do, the speed-up really is coming from per-statement commits and not from something
else about the two modes.

```bash
# 20000 fixed-length 50-byte records, no newlines (cobj-idx load defaults to COBOL SEQUENTIAL)
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

## Interpreting the result

- `synchronous` is left at SQLite's default (`FULL`) in both modes, so this is a like-for-like
  comparison of journal modes and not a trade of durability for speed.
- The gain comes from avoiding a rollback-journal create/fsync/delete cycle per commit, so it is
  largest for W1 and W2 and near zero for the read-only W3.
- `cobj-idx load` wraps the whole load in a single transaction, so it commits once regardless of
  journal mode and shows little difference. That is expected, and is a useful cross-check that the
  speed-up really does come from per-statement commits.
- Results on a network filesystem are meaningless: WAL needs shared memory and does not work there.
  Use `COB_INDEXED_JOURNAL_MODE=DELETE` in that case.
- The absolute numbers depend heavily on how your storage handles fsync. An SSD with a write cache
  and a spinning disk with barriers enabled can differ by more than an order of magnitude.

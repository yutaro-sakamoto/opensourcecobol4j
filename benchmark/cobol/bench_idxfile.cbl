      *> 索引編成ファイル (SQLite) の WRITE/READ/START ベンチマーク
       IDENTIFICATION DIVISION.
       PROGRAM-ID. bench_idxfile.
       ENVIRONMENT DIVISION.
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
           SELECT IDX-FILE ASSIGN TO "bench_idx.dat"
               ORGANIZATION IS INDEXED
               ACCESS MODE IS DYNAMIC
               RECORD KEY IS IR-KEY
               FILE STATUS IS WS-STATUS.
       DATA DIVISION.
       FILE SECTION.
       FD  IDX-FILE.
       01  IDX-REC.
           05 IR-KEY       PIC 9(8).
           05 IR-NAME      PIC X(30).
           05 IR-AMT       PIC 9(10).
       WORKING-STORAGE SECTION.
       01 WS-STATUS       PIC XX.
       01 WS-N-STR        PIC X(10).
       01 WS-N            PIC 9(9)  COMP-5.
       01 I               PIC 9(9)  COMP-5.
       01 WS-KEY          PIC 9(9)  COMP-5.
       01 WS-HIT          PIC 9(9)  COMP-5 VALUE 0.
       01 WS-SCAN         PIC 9(9)  COMP-5 VALUE 0.
       01 WS-EOF          PIC X VALUE "N".
       PROCEDURE DIVISION.
       MAIN-RTN.
           PERFORM INIT-N.
      *>   書き込みフェーズ (昇順キー)
           OPEN OUTPUT IDX-FILE.
           PERFORM VARYING I FROM 1 BY 1 UNTIL I > WS-N
               MOVE I TO IR-KEY
               MOVE "INDEXED BENCHMARK RECORD" TO IR-NAME
               MOVE I TO IR-AMT
               WRITE IDX-REC
           END-PERFORM.
           CLOSE IDX-FILE.
      *>   ランダムREADフェーズ (擬似乱数キー)
           OPEN INPUT IDX-FILE.
           MOVE 12345 TO WS-KEY.
           PERFORM VARYING I FROM 1 BY 1 UNTIL I > WS-N
               COMPUTE WS-KEY =
                   FUNCTION MOD(WS-KEY * 1103515245 + 12345, WS-N) + 1
               MOVE WS-KEY TO IR-KEY
               READ IDX-FILE
                   INVALID KEY CONTINUE
                   NOT INVALID KEY ADD 1 TO WS-HIT
               END-READ
           END-PERFORM.
      *>   START + READ NEXT による順次スキャン
           MOVE 1 TO IR-KEY.
           START IDX-FILE KEY IS >= IR-KEY
               INVALID KEY MOVE "Y" TO WS-EOF
           END-START.
           PERFORM UNTIL WS-EOF = "Y"
               READ IDX-FILE NEXT
                   AT END MOVE "Y" TO WS-EOF
                   NOT AT END ADD 1 TO WS-SCAN
               END-READ
           END-PERFORM.
           CLOSE IDX-FILE.
           DISPLAY "HIT=" WS-HIT " SCAN=" WS-SCAN.
           STOP RUN.
       INIT-N.
           ACCEPT WS-N-STR FROM ENVIRONMENT "BENCH_N" END-ACCEPT.
           IF WS-N-STR = SPACES
               MOVE 5000 TO WS-N
           ELSE
               COMPUTE WS-N = FUNCTION NUMVAL(WS-N-STR)
           END-IF.

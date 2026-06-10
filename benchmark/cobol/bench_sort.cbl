      *> SORT 文のベンチマーク (擬似乱数キーを RELEASE して整列)
       IDENTIFICATION DIVISION.
       PROGRAM-ID. bench_sort.
       ENVIRONMENT DIVISION.
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
           SELECT SORT-FILE ASSIGN TO "bench_sort.tmp".
       DATA DIVISION.
       FILE SECTION.
       SD  SORT-FILE.
       01  SORT-REC.
           05 SO-KEY       PIC 9(9).
           05 SO-DATA      PIC X(40).
       WORKING-STORAGE SECTION.
       01 WS-N-STR        PIC X(10).
       01 WS-N            PIC 9(9)  COMP-5.
       01 I               PIC 9(9)  COMP-5.
       01 WS-SEED         PIC 9(9)  COMP-5 VALUE 12345.
       01 WS-COUNT        PIC 9(9)  COMP-5 VALUE 0.
       01 WS-PREV         PIC 9(9)  COMP-5 VALUE 0.
       01 WS-ORDERED      PIC X VALUE "Y".
       01 WS-EOF          PIC X VALUE "N".
       PROCEDURE DIVISION.
       MAIN-RTN.
           PERFORM INIT-N.
           SORT SORT-FILE ON ASCENDING KEY SO-KEY
               INPUT PROCEDURE IS REL-RTN
               OUTPUT PROCEDURE IS RET-RTN.
           DISPLAY "COUNT=" WS-COUNT " ORDERED=" WS-ORDERED.
           STOP RUN.
       REL-RTN.
           PERFORM VARYING I FROM 1 BY 1 UNTIL I > WS-N
               COMPUTE WS-SEED =
                   FUNCTION MOD(WS-SEED * 1103515245 + 12345, 999999999)
               MOVE WS-SEED TO SO-KEY
               MOVE "SORT BENCHMARK PAYLOAD DATA" TO SO-DATA
               RELEASE SORT-REC
           END-PERFORM.
       RET-RTN.
           MOVE 0 TO WS-PREV.
           PERFORM UNTIL WS-EOF = "Y"
               RETURN SORT-FILE
                   AT END MOVE "Y" TO WS-EOF
                   NOT AT END
                       ADD 1 TO WS-COUNT
                       IF SO-KEY < WS-PREV
                           MOVE "N" TO WS-ORDERED
                       END-IF
                       MOVE SO-KEY TO WS-PREV
               END-RETURN
           END-PERFORM.
       INIT-N.
           ACCEPT WS-N-STR FROM ENVIRONMENT "BENCH_N" END-ACCEPT.
           IF WS-N-STR = SPACES
               MOVE 100000 TO WS-N
           ELSE
               COMPUTE WS-N = FUNCTION NUMVAL(WS-N-STR)
           END-IF.

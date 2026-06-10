      *> サブプログラム CALL を反復するベンチマーク
       IDENTIFICATION DIVISION.
       PROGRAM-ID. bench_call.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01 WS-N-STR        PIC X(10).
       01 WS-N            PIC 9(9)  COMP-5.
       01 I               PIC 9(9)  COMP-5.
       01 WS-ARG          PIC S9(9) COMP-5 VALUE 0.
       01 WS-ACC          PIC S9(18) COMP-5 VALUE 0.
       PROCEDURE DIVISION.
       MAIN-RTN.
           PERFORM INIT-N.
           PERFORM VARYING I FROM 1 BY 1 UNTIL I > WS-N
               MOVE I TO WS-ARG
               CALL "bench_sub" USING WS-ARG WS-ACC
               END-CALL
           END-PERFORM.
           DISPLAY "ACC=" WS-ACC.
           STOP RUN.
       INIT-N.
           ACCEPT WS-N-STR FROM ENVIRONMENT "BENCH_N" END-ACCEPT.
           IF WS-N-STR = SPACES
               MOVE 500000 TO WS-N
           ELSE
               COMPUTE WS-N = FUNCTION NUMVAL(WS-N-STR)
           END-IF.

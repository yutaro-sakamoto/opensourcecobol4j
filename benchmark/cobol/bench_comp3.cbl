      *> パック10進数 (COMP-3) の COMPUTE/ADD を反復するベンチマーク
       IDENTIFICATION DIVISION.
       PROGRAM-ID. bench_comp3.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01 WS-N-STR        PIC X(10).
       01 WS-N            PIC 9(9)  COMP-5.
       01 I               PIC 9(9)  COMP-5.
       01 WS-PRICE        PIC S9(7)V99  COMP-3 VALUE 123.45.
       01 WS-QTY          PIC S9(5)     COMP-3 VALUE 7.
       01 WS-RATE         PIC S9(3)V99  COMP-3 VALUE 1.08.
       01 WS-AMT          PIC S9(13)V99 COMP-3.
       01 WS-TOTAL        PIC S9(15)V99 COMP-3 VALUE 0.
       PROCEDURE DIVISION.
       MAIN-RTN.
           PERFORM INIT-N.
           PERFORM VARYING I FROM 1 BY 1 UNTIL I > WS-N
               COMPUTE WS-AMT = WS-PRICE * WS-QTY * WS-RATE
               ADD WS-AMT TO WS-TOTAL
               IF WS-TOTAL > 900000000000
                   SUBTRACT 900000000000 FROM WS-TOTAL
               END-IF
           END-PERFORM.
           DISPLAY "TOTAL=" WS-TOTAL.
           STOP RUN.
       INIT-N.
           ACCEPT WS-N-STR FROM ENVIRONMENT "BENCH_N" END-ACCEPT.
           IF WS-N-STR = SPACES
               MOVE 1000000 TO WS-N
           ELSE
               COMPUTE WS-N = FUNCTION NUMVAL(WS-N-STR)
           END-IF.

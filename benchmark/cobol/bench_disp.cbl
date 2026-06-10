      *> USAGE DISPLAY (ゾーン10進) 数値の算術を反復するベンチマーク
       IDENTIFICATION DIVISION.
       PROGRAM-ID. bench_disp.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01 WS-N-STR        PIC X(10).
       01 WS-N            PIC 9(9)  COMP-5.
       01 I               PIC 9(9)  COMP-5.
       01 WS-A            PIC S9(9)     VALUE 12345.
       01 WS-B            PIC S9(7)V99  VALUE 678.90.
       01 WS-C            PIC S9(13)V99 VALUE 0.
       01 WS-TOTAL        PIC S9(15)V99 VALUE 0.
       PROCEDURE DIVISION.
       MAIN-RTN.
           PERFORM INIT-N.
           PERFORM VARYING I FROM 1 BY 1 UNTIL I > WS-N
               COMPUTE WS-C = WS-A * WS-B / 100
               ADD WS-C TO WS-TOTAL
               ADD 1 TO WS-A
               IF WS-A > 99999
                   MOVE 12345 TO WS-A
               END-IF
               IF WS-TOTAL > 900000000000
                   SUBTRACT 900000000000 FROM WS-TOTAL
               END-IF
           END-PERFORM.
           DISPLAY "TOTAL=" WS-TOTAL.
           STOP RUN.
       INIT-N.
           ACCEPT WS-N-STR FROM ENVIRONMENT "BENCH_N" END-ACCEPT.
           IF WS-N-STR = SPACES
               MOVE 500000 TO WS-N
           ELSE
               COMPUTE WS-N = FUNCTION NUMVAL(WS-N-STR)
           END-IF.

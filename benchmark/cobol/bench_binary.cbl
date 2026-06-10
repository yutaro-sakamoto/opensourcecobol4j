      *> 2進数値 (COMP-5) の算術を反復するベンチマーク
       IDENTIFICATION DIVISION.
       PROGRAM-ID. bench_binary.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01 WS-N-STR        PIC X(10).
       01 WS-N            PIC 9(9)  COMP-5.
       01 I               PIC 9(9)  COMP-5.
       01 WS-A            PIC S9(9)  COMP-5 VALUE 12345.
       01 WS-B            PIC S9(9)  COMP-5 VALUE 678.
       01 WS-C            PIC S9(18) COMP-5 VALUE 0.
       01 WS-D            PIC S9(9)  COMP-5 VALUE 0.
       PROCEDURE DIVISION.
       MAIN-RTN.
           PERFORM INIT-N.
           PERFORM VARYING I FROM 1 BY 1 UNTIL I > WS-N
               COMPUTE WS-C = WS-A * WS-B + I
               COMPUTE WS-D = WS-C / 97
               ADD WS-D TO WS-A
               IF WS-A > 100000000
                   MOVE 12345 TO WS-A
               END-IF
           END-PERFORM.
           DISPLAY "A=" WS-A " D=" WS-D.
           STOP RUN.
       INIT-N.
           ACCEPT WS-N-STR FROM ENVIRONMENT "BENCH_N" END-ACCEPT.
           IF WS-N-STR = SPACES
               MOVE 1000000 TO WS-N
           ELSE
               COMPUTE WS-N = FUNCTION NUMVAL(WS-N-STR)
           END-IF.

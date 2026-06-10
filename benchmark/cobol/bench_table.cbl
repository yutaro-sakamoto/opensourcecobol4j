      *> OCCURS 表の添字アクセス (PERFORM VARYING) ベンチマーク
       IDENTIFICATION DIVISION.
       PROGRAM-ID. bench_table.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01 WS-N-STR        PIC X(10).
       01 WS-N            PIC 9(9)  COMP-5.
       01 K               PIC 9(9)  COMP-5.
       01 I               PIC 9(9)  COMP-5.
       01 J               PIC 9(9)  COMP-5.
       01 WS-SUM          PIC S9(18) COMP-5.
       01 TBL-1D.
          05 ELEM         PIC S9(9) COMP-5 OCCURS 1000.
       01 TBL-2D.
          05 ROW-E OCCURS 50.
             10 CELL      PIC S9(9) COMP-5 OCCURS 20.
       PROCEDURE DIVISION.
       MAIN-RTN.
           PERFORM INIT-N.
           PERFORM VARYING K FROM 1 BY 1 UNTIL K > WS-N
      *>       1次元表: 書き込みと合計
               PERFORM VARYING I FROM 1 BY 1 UNTIL I > 1000
                   COMPUTE ELEM(I) = I + K
               END-PERFORM
               MOVE 0 TO WS-SUM
               PERFORM VARYING I FROM 1 BY 1 UNTIL I > 1000
                   ADD ELEM(I) TO WS-SUM
               END-PERFORM
      *>       2次元表: 書き込みと合計
               PERFORM VARYING I FROM 1 BY 1 UNTIL I > 50
                   PERFORM VARYING J FROM 1 BY 1 UNTIL J > 20
                       COMPUTE CELL(I, J) = I * J
                   END-PERFORM
               END-PERFORM
           END-PERFORM.
           DISPLAY "SUM=" WS-SUM " CELL=" CELL(50, 20).
           STOP RUN.
       INIT-N.
           ACCEPT WS-N-STR FROM ENVIRONMENT "BENCH_N" END-ACCEPT.
           IF WS-N-STR = SPACES
               MOVE 1000 TO WS-N
           ELSE
               COMPUTE WS-N = FUNCTION NUMVAL(WS-N-STR)
           END-IF.

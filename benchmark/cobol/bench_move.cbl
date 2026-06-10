      *> 英数字項目・集団項目の MOVE を反復するベンチマーク
       IDENTIFICATION DIVISION.
       PROGRAM-ID. bench_move.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01 WS-N-STR        PIC X(10).
       01 WS-N            PIC 9(9)  COMP-5.
       01 I               PIC 9(9)  COMP-5.
       01 WS-SRC          PIC X(100) VALUE ALL "A".
       01 WS-DST          PIC X(100).
       01 WS-SHORT        PIC X(30).
       01 GRP-1.
          05 G1-NAME      PIC X(20)  VALUE "TARO YAMADA".
          05 G1-CODE      PIC 9(8)   VALUE 12345678.
          05 G1-ADDR      PIC X(50)  VALUE "TOKYO".
          05 G1-NOTE      PIC X(22)  VALUE "NOTE".
       01 GRP-2.
          05 G2-NAME      PIC X(20).
          05 G2-CODE      PIC 9(8).
          05 G2-ADDR      PIC X(50).
          05 G2-NOTE      PIC X(22).
       PROCEDURE DIVISION.
       MAIN-RTN.
           PERFORM INIT-N.
           PERFORM VARYING I FROM 1 BY 1 UNTIL I > WS-N
               MOVE WS-SRC TO WS-DST
               MOVE WS-DST TO WS-SHORT
               MOVE GRP-1 TO GRP-2
               MOVE SPACES TO WS-DST
               MOVE "SHORT LITERAL" TO WS-DST
           END-PERFORM.
           DISPLAY "DST=" WS-DST(1:13) " G2=" G2-NAME.
           STOP RUN.
       INIT-N.
           ACCEPT WS-N-STR FROM ENVIRONMENT "BENCH_N" END-ACCEPT.
           IF WS-N-STR = SPACES
               MOVE 1000000 TO WS-N
           ELSE
               COMPUTE WS-N = FUNCTION NUMVAL(WS-N-STR)
           END-IF.

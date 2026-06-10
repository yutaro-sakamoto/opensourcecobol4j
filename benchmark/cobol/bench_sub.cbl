      *> bench_call から呼ばれるサブプログラム
       IDENTIFICATION DIVISION.
       PROGRAM-ID. bench_sub.
       DATA DIVISION.
       LINKAGE SECTION.
       01 LK-ARG          PIC S9(9)  COMP-5.
       01 LK-ACC          PIC S9(18) COMP-5.
       PROCEDURE DIVISION USING LK-ARG LK-ACC.
       MAIN-RTN.
           ADD LK-ARG TO LK-ACC.
           IF LK-ACC > 1000000000000
               SUBTRACT 1000000000000 FROM LK-ACC
           END-IF.
           EXIT PROGRAM.

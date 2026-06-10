      *> 順編成ファイルの WRITE/READ ベンチマーク
       IDENTIFICATION DIVISION.
       PROGRAM-ID. bench_seqfile.
       ENVIRONMENT DIVISION.
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
           SELECT SEQ-FILE ASSIGN TO "bench_seq.dat"
               ORGANIZATION IS LINE SEQUENTIAL
               FILE STATUS IS WS-STATUS.
       DATA DIVISION.
       FILE SECTION.
       FD  SEQ-FILE.
       01  SEQ-REC.
           05 SR-KEY       PIC 9(8).
           05 SR-NAME      PIC X(30).
           05 SR-AMT       PIC 9(10).
           05 SR-FILLER    PIC X(32).
       WORKING-STORAGE SECTION.
       01 WS-STATUS       PIC XX.
       01 WS-N-STR        PIC X(10).
       01 WS-N            PIC 9(9)  COMP-5.
       01 I               PIC 9(9)  COMP-5.
       01 WS-COUNT        PIC 9(9)  COMP-5 VALUE 0.
       01 WS-SUM          PIC 9(18) COMP-5 VALUE 0.
       01 WS-EOF          PIC X VALUE "N".
       PROCEDURE DIVISION.
       MAIN-RTN.
           PERFORM INIT-N.
      *>   書き込みフェーズ
           OPEN OUTPUT SEQ-FILE.
           PERFORM VARYING I FROM 1 BY 1 UNTIL I > WS-N
               MOVE I TO SR-KEY
               MOVE "BENCHMARK RECORD NAME FIELD" TO SR-NAME
               MOVE I TO SR-AMT
               MOVE ALL "X" TO SR-FILLER
               WRITE SEQ-REC
           END-PERFORM.
           CLOSE SEQ-FILE.
      *>   読み込みフェーズ
           OPEN INPUT SEQ-FILE.
           MOVE "N" TO WS-EOF.
           PERFORM UNTIL WS-EOF = "Y"
               READ SEQ-FILE
                   AT END MOVE "Y" TO WS-EOF
                   NOT AT END
                       ADD 1 TO WS-COUNT
                       ADD SR-AMT TO WS-SUM
               END-READ
           END-PERFORM.
           CLOSE SEQ-FILE.
           DISPLAY "COUNT=" WS-COUNT " SUM=" WS-SUM.
           STOP RUN.
       INIT-N.
           ACCEPT WS-N-STR FROM ENVIRONMENT "BENCH_N" END-ACCEPT.
           IF WS-N-STR = SPACES
               MOVE 100000 TO WS-N
           ELSE
               COMPUTE WS-N = FUNCTION NUMVAL(WS-N-STR)
           END-IF.

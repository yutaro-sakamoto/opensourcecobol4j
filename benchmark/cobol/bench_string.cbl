      *> STRING / UNSTRING / INSPECT を反復するベンチマーク
       IDENTIFICATION DIVISION.
       PROGRAM-ID. bench_string.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01 WS-N-STR        PIC X(10).
       01 WS-N            PIC 9(9)  COMP-5.
       01 I               PIC 9(9)  COMP-5.
       01 WS-NAME         PIC X(20) VALUE "TARO".
       01 WS-CITY         PIC X(20) VALUE "TOKYO".
       01 WS-LINE         PIC X(80).
       01 WS-P1           PIC X(20).
       01 WS-P2           PIC X(20).
       01 WS-P3           PIC X(40).
       01 WS-CNT          PIC 9(9)  COMP-5.
       01 WS-TALLY-SUM    PIC 9(9)  COMP-5 VALUE 0.
       PROCEDURE DIVISION.
       MAIN-RTN.
           PERFORM INIT-N.
           PERFORM VARYING I FROM 1 BY 1 UNTIL I > WS-N
               MOVE SPACES TO WS-LINE
               STRING WS-NAME DELIMITED BY SPACE
                      ","     DELIMITED BY SIZE
                      WS-CITY DELIMITED BY SPACE
                      ",JAPAN,EXTRA FIELD" DELIMITED BY SIZE
                      INTO WS-LINE
               END-STRING
               UNSTRING WS-LINE DELIMITED BY ","
                   INTO WS-P1 WS-P2 WS-P3
               END-UNSTRING
               MOVE 0 TO WS-CNT
               INSPECT WS-LINE TALLYING WS-CNT FOR ALL "A"
               ADD WS-CNT TO WS-TALLY-SUM
               INSPECT WS-LINE REPLACING ALL "," BY ";"
           END-PERFORM.
           DISPLAY "LINE=" WS-LINE(1:30).
           DISPLAY "P2=" WS-P2 " TALLY=" WS-TALLY-SUM.
           STOP RUN.
       INIT-N.
           ACCEPT WS-N-STR FROM ENVIRONMENT "BENCH_N" END-ACCEPT.
           IF WS-N-STR = SPACES
               MOVE 100000 TO WS-N
           ELSE
               COMPUTE WS-N = FUNCTION NUMVAL(WS-N-STR)
           END-IF.

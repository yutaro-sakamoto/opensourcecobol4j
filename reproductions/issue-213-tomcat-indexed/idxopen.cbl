       IDENTIFICATION DIVISION.
       PROGRAM-ID. idxopen.
      ******************************************************************
      * Minimal INDEXED-file program used to reproduce
      * opensourcecobol/opensourcecobol4j#213
      * (INDEXED file OPEN fails on Tomcat due to SQLite JDBC driver
      *  not being registered under the web-app class loader).
      *
      * Standalone : OPEN OUTPUT STATUS=00
      * Tomcat-like: OPEN OUTPUT STATUS=30 (connection cannot be made)
      ******************************************************************
       ENVIRONMENT DIVISION.
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
           SELECT MYFILE ASSIGN TO "testidx.dat"
                  ORGANIZATION IS INDEXED
                  ACCESS MODE IS SEQUENTIAL
                  RECORD KEY IS R-KEY
                  FILE STATUS IS WS-STATUS.
       DATA DIVISION.
       FILE SECTION.
       FD MYFILE.
       01 R-REC.
          03 R-KEY   PIC X(03).
          03 R-DATA  PIC X(10).
       WORKING-STORAGE SECTION.
       01 WS-STATUS PIC X(02).
       PROCEDURE DIVISION.
       MAIN SECTION.
           OPEN OUTPUT MYFILE.
           DISPLAY "OPEN OUTPUT STATUS=" WS-STATUS.
           IF WS-STATUS = "00"
              MOVE "K01" TO R-KEY
              MOVE "HELLO" TO R-DATA
              WRITE R-REC
              DISPLAY "WRITE STATUS=" WS-STATUS
              CLOSE MYFILE
              DISPLAY "CLOSE STATUS=" WS-STATUS
           END-IF.
      *    GOBACK (not STOP RUN) so the program returns control to its
      *    caller instead of calling System.exit -- required when it runs
      *    inside a servlet container such as Tomcat.
           GOBACK.

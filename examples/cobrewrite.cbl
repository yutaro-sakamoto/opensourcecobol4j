       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      cobrewrite.
      *> Demonstrates COB_IO_ASSUME_REWRITE environment variable.
      *> COB_IO_ASSUME_REWRITE=Y makes WRITE act as REWRITE
      *> when the file is opened in I-O mode.
       ENVIRONMENT      DIVISION.
       INPUT-OUTPUT     SECTION.
       FILE-CONTROL.
       SELECT TEST-FILE ASSIGN "TEST-REWRITE".
       DATA             DIVISION.
       FILE             SECTION.
       FD TEST-FILE.
       01 TEST-REC      PIC X(4).
       PROCEDURE        DIVISION.
           OPEN I-O TEST-FILE.
           READ  TEST-FILE.
           READ  TEST-FILE.
           MOVE  "AAAA" TO TEST-REC.
           WRITE TEST-REC.
           CLOSE TEST-FILE.
           STOP RUN.

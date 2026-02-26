       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      coblsfixed.
      *> Demonstrates COB_LS_FIXED environment variable.
      *> COB_LS_FIXED=Y writes LINE SEQUENTIAL records
      *> at fixed length (no trailing space trimming).
       ENVIRONMENT      DIVISION.
       INPUT-OUTPUT     SECTION.
       FILE-CONTROL.
       SELECT TEST-FILE ASSIGN       "./ls-test-output.txt"
                        ORGANIZATION LINE SEQUENTIAL.
       DATA             DIVISION.
       FILE             SECTION.
       FD TEST-FILE.
       01 TEST-REC      PIC X(20).
       PROCEDURE        DIVISION.
           OPEN OUTPUT TEST-FILE.
           MOVE "AB" TO TEST-REC.
           WRITE TEST-REC.
           CLOSE TEST-FILE.
           DISPLAY "File written.".
           STOP RUN.

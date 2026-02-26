       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      cobfilepath.
      *> Demonstrates COB_FILE_PATH environment variable.
      *> COB_FILE_PATH changes the directory where files
      *> are created.
       ENVIRONMENT      DIVISION.
       INPUT-OUTPUT     SECTION.
       FILE-CONTROL.
       SELECT TEST-FILE ASSIGN "PATHTEST".
       DATA             DIVISION.
       FILE             SECTION.
       FD TEST-FILE.
       01 TEST-REC      PIC X(4).
       PROCEDURE        DIVISION.
           OPEN OUTPUT TEST-FILE.
           MOVE "DATA" TO TEST-REC.
           WRITE TEST-REC.
           CLOSE TEST-FILE.
           DISPLAY "File created successfully.".
           STOP RUN.

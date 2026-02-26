       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      ddfilename.
      *> Demonstrates DD_filename environment variable.
      *> DD_MYDATA maps the logical file name "MYDATA" to
      *> an actual file path.
       ENVIRONMENT      DIVISION.
       INPUT-OUTPUT     SECTION.
       FILE-CONTROL.
       SELECT TEST-FILE ASSIGN "MYDATA".
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

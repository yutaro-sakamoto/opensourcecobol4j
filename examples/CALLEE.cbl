       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      CALLEE.
      *> Called program for loadcaller.cbl example.
      *> Demonstrates COB_LOAD_CASE environment variable.
       PROCEDURE        DIVISION.
           DISPLAY "CALLEE was called successfully.".
           GOBACK.

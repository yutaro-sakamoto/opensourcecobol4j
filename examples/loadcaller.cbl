       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      loadcaller.
      *> Demonstrates COB_LOAD_CASE environment variable.
      *> COB_LOAD_CASE=UPPER converts the program name to
      *> uppercase before resolving the CALL target.
       PROCEDURE        DIVISION.
           CALL "callee"
           END-CALL.
           STOP RUN.

       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      ocextcreates.
      *> Demonstrates OC_EXTEND_CREATES environment variable.
      *> OC_EXTEND_CREATES=yes allows OPEN EXTEND to
      *> auto-create a file that does not exist.
       ENVIRONMENT      DIVISION.
       INPUT-OUTPUT     SECTION.
       FILE-CONTROL.
       SELECT TEST-FILE ASSIGN       "./TEST-EXT-FILE"
                        ORGANIZATION SEQUENTIAL
                        FILE STATUS  F-STATUS.
       DATA             DIVISION.
       FILE             SECTION.
       FD TEST-FILE.
       01 TEST-RECORD.
         03 TEST-KEY    PIC 9(10).
       WORKING-STORAGE  SECTION.
       77 F-STATUS      PIC X(02).
       PROCEDURE DIVISION.
           OPEN EXTEND TEST-FILE.
           DISPLAY "FILE STATUS: " F-STATUS.
           CLOSE TEST-FILE.
           STOP RUN.

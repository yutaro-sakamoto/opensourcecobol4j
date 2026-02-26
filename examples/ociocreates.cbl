       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      ociocreates.
      *> Demonstrates OC_IO_CREATES environment variable.
      *> OC_IO_CREATES=yes allows OPEN I-O to auto-create
      *> a file that does not exist.
       ENVIRONMENT      DIVISION.
       INPUT-OUTPUT     SECTION.
       FILE-CONTROL.
       SELECT TEST-FILE ASSIGN       "./TEST-IO-FILE"
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
           OPEN I-O TEST-FILE.
           DISPLAY "FILE STATUS: " F-STATUS.
           CLOSE TEST-FILE.
           STOP RUN.

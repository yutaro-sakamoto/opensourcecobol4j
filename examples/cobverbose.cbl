       IDENTIFICATION       DIVISION.
       PROGRAM-ID.          cobverbose.
      *> Demonstrates COB_VERBOSE environment variable.
      *> COB_VERBOSE=Y outputs verbose messages during SORT.
       ENVIRONMENT          DIVISION.
       INPUT-OUTPUT         SECTION.
       FILE-CONTROL.
           SELECT SORT-FILE  ASSIGN TO "S01"
               ORGANIZATION SEQUENTIAL.
           SELECT INPUT-FILE  ASSIGN TO "./sort-input.txt".
           SELECT OUTPUT-FILE ASSIGN TO "./sort-output.txt".
       DATA                 DIVISION.
       FILE                 SECTION.
       SD SORT-FILE.
       01 SORT-REC.
          02 SORT-KEY  PIC XX.
          02 SORT-DATA PIC XX.
       FD INPUT-FILE.
       01 INPUT-REC.
          02 XFLD1   PIC X(2).
          02 XFLD2   PIC X(2).
       FD OUTPUT-FILE.
       01 OUTPUT-REC.
          02 XFLD1   PIC X(2).
          02 XFLD2   PIC X(2).
       PROCEDURE            DIVISION.
          SORT SORT-FILE ON ASCENDING KEY SORT-KEY
             USING INPUT-FILE
             GIVING OUTPUT-FILE.
          STOP RUN.

       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      cobdate.
      *> Demonstrates COB_DATE environment variable.
      *> COB_DATE fixes the date returned by ACCEPT FROM DATE
      *> and FUNCTION CURRENT-DATE.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01  DATE-YMD.
         03  DATE-YYYY PIC X(4).
         03  DATE-MM   PIC X(2).
         03  DATE-DD   PIC X(2).
       01  CURR-DATE.
         03  CURR-YYYY PIC X(4).
         03  CURR-MM   PIC X(2).
         03  CURR-DD   PIC X(2).
         03  FILLER    PIC X(13).
       PROCEDURE        DIVISION.
           ACCEPT DATE-YMD FROM DATE YYYYMMDD.
           DISPLAY "ACCEPT FROM DATE: " DATE-YMD.
           MOVE FUNCTION CURRENT-DATE TO CURR-DATE.
           DISPLAY "CURRENT-DATE:     "
               CURR-YYYY CURR-MM CURR-DD.
           STOP RUN.

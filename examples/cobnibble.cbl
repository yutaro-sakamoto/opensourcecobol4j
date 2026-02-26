       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      cobnibble.
      *> Demonstrates COB_NIBBLE_C_UNSIGNED environment variable.
      *> COB_NIBBLE_C_UNSIGNED=Y treats nibble C in COMP-3
      *> as unsigned, making the unsigned field pass IS NUMERIC.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01  UNI01.
         03 SIGNED01.
           05 SDEC01 PIC S9(2) COMP-3.
         03 UNSIGNED01 REDEFINES SIGNED01.
           05 UDEC01 PIC  9(2) COMP-3.
       PROCEDURE        DIVISION.
           MOVE 1 TO SDEC01.
           IF SDEC01 IS NUMERIC THEN
               DISPLAY "Signed:   NUMERIC"
           ELSE
               DISPLAY "Signed:   NOT NUMERIC"
           END-IF.
           IF UDEC01 IS NUMERIC THEN
               DISPLAY "Unsigned: NUMERIC"
           ELSE
               DISPLAY "Unsigned: NOT NUMERIC"
           END-IF.
           STOP RUN.

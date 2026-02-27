       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      cobswitch.
      *> Demonstrates COB_SWITCH_1~8 environment variables.
      *> COB_SWITCH_n=ON sets the initial value of SWITCH-n.
       ENVIRONMENT DIVISION.
       CONFIGURATION SECTION.
       SPECIAL-NAMES.
           SWITCH-1 IS SW1
             ON STATUS IS SWIT1-ON
             OFF STATUS IS SWIT1-OFF
           SWITCH-2 IS SW2
             ON STATUS IS SWIT2-ON
             OFF STATUS IS SWIT2-OFF.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       PROCEDURE        DIVISION.
           DISPLAY "SWITCH-1: " NO ADVANCING.
           IF SWIT1-ON
              DISPLAY "ON"
           ELSE
              DISPLAY "OFF"
           END-IF.
           DISPLAY "SWITCH-2: " NO ADVANCING.
           IF SWIT2-ON
              DISPLAY "ON"
           ELSE
              DISPLAY "OFF"
           END-IF.
           STOP RUN.

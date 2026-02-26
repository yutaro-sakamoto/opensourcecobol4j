       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      cobswitch.
      *> Demonstrates COB_SWITCH_1~8 environment variables.
      *> COB_SWITCH_n=ON sets the initial value of SWITCH-n.
      *> Requires jp-compat config for switch-no-mnemonic.
       ENVIRONMENT DIVISION.
       CONFIGURATION SECTION.
       SPECIAL-NAMES.
           SWITCH-1
             ON IS SWIT1-ON
             OFF IS SWIT1-OFF
           SWITCH-2
             ON IS SWIT2-ON
             OFF IS SWIT2-OFF.
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

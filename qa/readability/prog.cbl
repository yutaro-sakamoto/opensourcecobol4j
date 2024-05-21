       IDENTIFICATION              DIVISION.
       PROGRAM-ID.                 prog.
       ENVIRONMENT                 DIVISION.  
       INPUT-OUTPUT                SECTION.
       FILE-CONTROL.
       DATA                        DIVISION.
       FILE                        SECTION.

       working-storage section.
       01 num1 pic 9.
       01 num2 pic 99.
       procedure division.
          move num1 to num2.
          if num1 = 0 then
            display "hellO"
          else
            display "world"
          end-if.
          display num1.


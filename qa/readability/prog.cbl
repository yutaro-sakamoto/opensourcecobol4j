       IDENTIFICATION              DIVISION.
       PROGRAM-ID.                 prog.
       ENVIRONMENT                 DIVISION.  
       INPUT-OUTPUT                SECTION.
       FILE-CONTROL.
       DATA                        DIVISION.
       FILE                        SECTION.

       working-storage section.
       01 num1 pic 9(5) value 12345.
       01 num2 pic 9(7) value 9876543.
       01 num3 pic 9(3) value 456.

       01 str1 pic x(5) value "12345".
       01 str2 pic x(10) value "world".
       01 str3 pic x(3) value "abc".

       procedure division.
       DISPLAY-VALUES.
           display num1.
           display num2.
           display num3.
           display str1.
           display str2.
           display str3.

       MVOE-VALUES.
           move num1 to num2.
           display num2.

           move num1 to num3.
           display num3.

           move str1 to str2.
           display str2.

           move str1 to str3.
           display str3.

           move str1 to num2.
           display num2.

           move num1 to str2.
           display str2.

       IF-STATEMENTS.
           if num1 = 12345 then
             display "OK 001"
           end-if.

           if str1 = "hello" then
             display "OK 002"
           end-if.

           if num1 NOT = str1 then
              display "OK 003"
           end-if.

       LABEL-TEST-GOTO.
           display "OK 004"
           GO TO LABEL-TEST-PERFORM.
       LABEL-PERFORM-PROC.
           display "OK 007".
       LABEL-PERFORM-THRU-PROC1.
           display "OK 005".
       LABEL-PERFORM-THRU-PROC2.
           display "OK 006".
       LABEL-TEST-PERFORM.
           perform LABEL-PERFORM-THRU-PROC1
              thru LABEL-PERFORM-THRU-PROC2.
           perform LABEL-PERFORM-PROC.
           display "OK 008".
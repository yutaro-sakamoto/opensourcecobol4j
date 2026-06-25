      ******************************************************************
      * INSERTTBL.cbl - Embedded SQL INSERT example
      *
      * Ported from the classic Open COBOL ESQL 4J sample program.
      * Employee names are written in romaji (ASCII) instead of the
      * original Japanese.
      *
      * This program demonstrates:
      *   - Connecting to PostgreSQL
      *   - Creating the EMP table
      *   - Inserting rows with literal values
      *   - Inserting rows with host variables (OCCURS table)
      *   - Committing and disconnecting
      *
      * Run this before FETCHTBL.cbl; the two programs share the EMP
      * table. Placeholders <|DB_NAME|>, <|DB_HOST|>, etc. are replaced
      * by the Makefile before compilation.
      ******************************************************************
       IDENTIFICATION              DIVISION.
       PROGRAM-ID.                 INSERTTBL_run.
      ******************************************************************
       DATA                        DIVISION.
      ******************************************************************
       WORKING-STORAGE             SECTION.
           EXEC SQL INCLUDE SQLCA END-EXEC.
       01  TEST-DATA.
         03 FILLER       PIC X(28) VALUE "0001HOKKAI TARO         0400".
         03 FILLER       PIC X(28) VALUE "0002AOMORI JIRO         0350".
         03 FILLER       PIC X(28) VALUE "0003AKITA SABURO        0300".
         03 FILLER       PIC X(28) VALUE "0004IWATE SHIRO         025p".
         03 FILLER       PIC X(28) VALUE "0005MIYAGI GORO         020p".
         03 FILLER       PIC X(28) VALUE "0006FUKUSHIMA ROKURO    0150".
         03 FILLER       PIC X(28) VALUE "0007TOCHIGI SHICHIRO    010p".
         03 FILLER       PIC X(28) VALUE "0008IBARAKI HACHIRO     0050".
         03 FILLER       PIC X(28) VALUE "0009GUNMA KURO          020p".
         03 FILLER       PIC X(28) VALUE "0010SAITAMA JURO        0350".
       01  TEST-DATA-R   REDEFINES TEST-DATA.
         03  TEST-TBL    OCCURS  10.
           05  TEST-NO             PIC S9(04).
           05  TEST-NAME           PIC  X(20) .
           05  TEST-SALARY         PIC S9(04).
       01  IDX                     PIC  9(02).

       01  DBNAME                  PIC  X(30) VALUE SPACE.
       01  USERNAME                PIC  X(30) VALUE SPACE.
       01  PASSWD                  PIC  X(10) VALUE SPACE.
       01  EMP-REC-VARS.
         03  EMP-NO                PIC S9(04) VALUE ZERO.
         03  EMP-NAME              PIC  X(20) .
         03  EMP-SALARY            PIC S9(04) VALUE ZERO.

      ******************************************************************
       PROCEDURE                   DIVISION.
      ******************************************************************
       MAIN-RTN.
           DISPLAY "*** INSERTTBL STARTED ***".

      *    CONNECT
           MOVE  "<|DB_NAME|>@<|DB_HOST|>:<|DB_PORT|>"
             TO DBNAME.
           MOVE  "<|DB_USER|>"
             TO USERNAME.
           MOVE  "<|DB_PASSWORD|>"
             TO PASSWD.
           EXEC SQL
               CONNECT :USERNAME IDENTIFIED BY :PASSWD USING :DBNAME
           END-EXEC.
           IF  SQLCODE NOT = ZERO PERFORM ERROR-RTN STOP RUN.

      *    DROP TABLE
           EXEC SQL
               DROP TABLE IF EXISTS EMP
           END-EXEC.
           IF  SQLCODE NOT = ZERO PERFORM ERROR-RTN.

      *    CREATE TABLE
           EXEC SQL
                CREATE TABLE EMP
                (
                    EMP_NO     NUMERIC(4,0) NOT NULL,
                    EMP_NAME   CHAR(20),
                    EMP_SALARY NUMERIC(4,0),
                    CONSTRAINT IEMP_0 PRIMARY KEY (EMP_NO)
                )
           END-EXEC.
           IF  SQLCODE NOT = ZERO PERFORM ERROR-RTN STOP RUN.

      *    INSERT ROWS USING LITERAL
           EXEC SQL
               INSERT INTO EMP VALUES (46, 'KAGOSHIMA ROKURO', -320)
           END-EXEC.
           IF  SQLCODE NOT = ZERO PERFORM ERROR-RTN.

           EXEC SQL
               INSERT INTO EMP VALUES (47, 'OKINAWA SHICHIRO', 480)
           END-EXEC.
           IF  SQLCODE NOT = ZERO PERFORM ERROR-RTN.

      *    INSERT ROWS USING HOST VARIABLE
           PERFORM VARYING IDX FROM 1 BY 1 UNTIL IDX > 10
              MOVE TEST-NO(IDX)     TO  EMP-NO
              MOVE TEST-NAME(IDX)   TO  EMP-NAME
              MOVE TEST-SALARY(IDX) TO  EMP-SALARY
              EXEC SQL
                 INSERT INTO EMP VALUES
                        (:EMP-NO,:EMP-NAME,:EMP-SALARY)
              END-EXEC
              IF  SQLCODE NOT = ZERO
                  PERFORM ERROR-RTN
                  EXIT PERFORM
              END-IF
           END-PERFORM.

      *    COMMIT
           EXEC SQL COMMIT WORK END-EXEC.

      *    DISCONNECT
           EXEC SQL
               DISCONNECT ALL
           END-EXEC.

      *    END
           DISPLAY "*** INSERTTBL FINISHED ***".
           STOP RUN.

      ******************************************************************
       ERROR-RTN.
      ******************************************************************
           DISPLAY "*** SQL ERROR ***".
           DISPLAY "SQLCODE: " SQLCODE " " NO ADVANCING.
           EVALUATE SQLCODE
              WHEN  +10
                 DISPLAY "Record not found"
              WHEN  -01
                 DISPLAY "Connection failed"
              WHEN  -20
                 DISPLAY "Internal error"
              WHEN  -30
                 DISPLAY "PostgreSQL error"
                 DISPLAY "ERRCODE: "  SQLSTATE
                 DISPLAY SQLERRMC
              *> TO RESTART TRANSACTION, DO ROLLBACK.
                 EXEC SQL
                     ROLLBACK
                 END-EXEC
              WHEN  OTHER
                 DISPLAY "Undefined error"
                 DISPLAY "ERRCODE: "  SQLSTATE
                 DISPLAY SQLERRMC
           END-EVALUATE.
      ******************************************************************

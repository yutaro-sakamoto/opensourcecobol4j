      ******************************************************************
      * FETCHTBL.cbl - Embedded SQL cursor FETCH example
      *
      * Ported from the classic Open COBOL ESQL 4J sample program.
      *
      * This program demonstrates:
      *   - Connecting to PostgreSQL
      *   - Counting rows with SELECT COUNT(*) INTO
      *   - Declaring and opening a cursor
      *   - Fetching rows in a loop and displaying them formatted
      *   - Closing the cursor, committing and disconnecting
      *
      * Run INSERTTBL.cbl first to populate the EMP table; the two
      * programs share that table. Placeholders <|DB_NAME|>,
      * <|DB_HOST|>, etc. are replaced by the Makefile before
      * compilation.
      ******************************************************************
       IDENTIFICATION              DIVISION.
       PROGRAM-ID.                 FETCHTBL_run.
      ******************************************************************
       DATA                        DIVISION.
      ******************************************************************
       WORKING-STORAGE             SECTION.
           EXEC SQL INCLUDE SQLCA END-EXEC.
       01  D-EMP-REC.
           05  D-EMP-NO            PIC  9(04).
           05  FILLER              PIC  X.
           05  D-EMP-NAME          PIC  X(20).
           05  FILLER              PIC  X.
           05  D-EMP-SALARY        PIC  --,--9.

       01  DBNAME                  PIC  X(30) VALUE SPACE.
       01  USERNAME                PIC  X(30) VALUE SPACE.
       01  PASSWD                  PIC  X(10) VALUE SPACE.
       01  EMP-REC-VARS.
           05  EMP-NO              PIC S9(04).
           05  EMP-NAME            PIC  X(20) .
           05  EMP-SALARY          PIC S9(04).
       01  EMP-CNT                 PIC  9(04).

      ******************************************************************
       PROCEDURE                   DIVISION.
      ******************************************************************
       MAIN-RTN.
           DISPLAY "*** FETCHTBL STARTED ***".

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

      *    SELECT COUNT(*) INTO HOST-VARIABLE
           EXEC SQL
               SELECT COUNT(*) INTO :EMP-CNT FROM EMP
           END-EXEC.
           DISPLAY "TOTAL RECORD: " EMP-CNT.

      *    DECLARE CURSOR
           EXEC SQL
               DECLARE C1 CURSOR FOR
               SELECT EMP_NO, EMP_NAME, EMP_SALARY
                      FROM EMP
                      ORDER BY EMP_NO
           END-EXEC.
           EXEC SQL
               OPEN C1
           END-EXEC.

      *    FETCH
           DISPLAY "---- -------------------- ------".
           DISPLAY "NO   NAME                 SALARY".
           DISPLAY "---- -------------------- ------".
           EXEC SQL
               FETCH C1 INTO :EMP-NO, :EMP-NAME, :EMP-SALARY
           END-EXEC.
           PERFORM UNTIL SQLCODE NOT = ZERO
              MOVE  EMP-NO        TO    D-EMP-NO
              MOVE  EMP-NAME      TO    D-EMP-NAME
              MOVE  EMP-SALARY    TO    D-EMP-SALARY
              DISPLAY D-EMP-REC
              EXEC SQL
                  FETCH C1 INTO :EMP-NO, :EMP-NAME, :EMP-SALARY
              END-EXEC
           END-PERFORM.

      *    CLOSE CURSOR
           EXEC SQL
               CLOSE C1
           END-EXEC.

      *    COMMIT
           EXEC SQL
               COMMIT WORK
           END-EXEC.

      *    DISCONNECT
           EXEC SQL
               DISCONNECT ALL
           END-EXEC.

      *    END
           DISPLAY "*** FETCHTBL FINISHED ***".
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

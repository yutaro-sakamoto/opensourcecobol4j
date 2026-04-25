      ******************************************************************
      * prepare.cbl - Prepared Statement example
      *
      * This program demonstrates:
      *   - Connecting to PostgreSQL
      *   - Creating a table
      *   - Preparing a SQL statement from a host variable
      *   - Executing the prepared statement with USING
      *   - Selecting data to verify the inserts
      *   - Cleaning up and disconnecting
      ******************************************************************
       IDENTIFICATION              DIVISION.
       PROGRAM-ID.                 prepare_run.
       DATA                        DIVISION.
       WORKING-STORAGE             SECTION.


       EXEC SQL BEGIN DECLARE SECTION END-EXEC.
       01  DBNAME                  PIC  X(30) VALUE SPACE.
       01  USERNAME                PIC  X(30) VALUE SPACE.
       01  PASSWD                  PIC  X(10) VALUE SPACE.

       01  SQL-STMT                PIC  X(200) VALUE SPACE.

       01  EMP-NO                  PIC  9(04) VALUE ZERO.
       01  EMP-NAME                PIC  X(20) VALUE SPACE.

       01  OUT-NO                  PIC  9(04) VALUE ZERO.
       01  OUT-NAME                PIC  X(20) VALUE SPACE.
       EXEC SQL END DECLARE SECTION END-EXEC.

       PROCEDURE                   DIVISION.
       MAIN-RTN.

      *---------------------------------------------------------------
      * Connect
      *---------------------------------------------------------------
           MOVE "<|DB_NAME|>@<|DB_HOST|>:<|DB_PORT|>"
             TO DBNAME.
           MOVE "<|DB_USER|>"
             TO USERNAME.
           MOVE "<|DB_PASSWORD|>"
             TO PASSWD.

           EXEC SQL
               CONNECT :USERNAME IDENTIFIED BY :PASSWD USING :DBNAME
           END-EXEC.
           IF SQLCODE NOT = ZERO
               DISPLAY "Connect failed: " SQLCODE
               STOP RUN
           END-IF.
           DISPLAY "Connected.".

      *---------------------------------------------------------------
      * Create table
      *---------------------------------------------------------------
           EXEC SQL
               DROP TABLE IF EXISTS esql_prepare_emp
           END-EXEC.

           EXEC SQL
               CREATE TABLE esql_prepare_emp (
                   emp_no   INTEGER,
                   emp_name VARCHAR(20)
               )
           END-EXEC.
           PERFORM CHECK-SQL.
           DISPLAY "Table created.".

      *---------------------------------------------------------------
      * Prepare a statement from a host variable
      *---------------------------------------------------------------
           MOVE "INSERT INTO esql_prepare_emp (emp_no, emp_name) VALU
      -    "ES (?, ?)"
             TO SQL-STMT.

           EXEC SQL
               PREPARE ins_stmt FROM :SQL-STMT
           END-EXEC.
           PERFORM CHECK-SQL.
           DISPLAY "Statement prepared.".

      *---------------------------------------------------------------
      * Execute the prepared statement multiple times
      *---------------------------------------------------------------
           MOVE 1 TO EMP-NO.
           MOVE "Alice" TO EMP-NAME.
           EXEC SQL
               EXECUTE ins_stmt USING :EMP-NO, :EMP-NAME
           END-EXEC.
           PERFORM CHECK-SQL.

           MOVE 2 TO EMP-NO.
           MOVE "Bob" TO EMP-NAME.
           EXEC SQL
               EXECUTE ins_stmt USING :EMP-NO, :EMP-NAME
           END-EXEC.
           PERFORM CHECK-SQL.

           MOVE 3 TO EMP-NO.
           MOVE "Charlie" TO EMP-NAME.
           EXEC SQL
               EXECUTE ins_stmt USING :EMP-NO, :EMP-NAME
           END-EXEC.
           PERFORM CHECK-SQL.
           DISPLAY "3 rows inserted via prepared statement.".

      *---------------------------------------------------------------
      * Verify by selecting a row
      *---------------------------------------------------------------
           MOVE 2 TO EMP-NO.
           EXEC SQL
               SELECT emp_no, emp_name
                   INTO :OUT-NO, :OUT-NAME
                   FROM esql_prepare_emp
                   WHERE emp_no = :EMP-NO
           END-EXEC.
           PERFORM CHECK-SQL.

           DISPLAY "Verified row:".
           DISPLAY "  No:   " OUT-NO.
           DISPLAY "  Name: " OUT-NAME.

      *---------------------------------------------------------------
      * Clean up
      *---------------------------------------------------------------
           EXEC SQL
               DROP TABLE esql_prepare_emp
           END-EXEC.
           DISPLAY "Table dropped.".

           EXEC SQL COMMIT END-EXEC.
           EXEC SQL DISCONNECT ALL END-EXEC.
           DISPLAY "Disconnected.".

           STOP RUN.

      ******************************************************************
       CHECK-SQL.
      ******************************************************************
           IF SQLCODE NOT = ZERO
               DISPLAY "SQL Error: " SQLCODE
               DISPLAY "State: " SQLSTATE
               DISPLAY "Message: " SQLERRMC
               EXEC SQL ROLLBACK END-EXEC
               EXEC SQL DISCONNECT ALL END-EXEC
               STOP RUN
           END-IF.

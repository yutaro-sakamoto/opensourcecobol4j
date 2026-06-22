      ******************************************************************
      * cursor.cbl - Cursor-based Embedded SQL example
      *
      * This program demonstrates:
      *   - Connecting to PostgreSQL
      *   - Creating a table and inserting data
      *   - Declaring a cursor for a SELECT query
      *   - Opening the cursor
      *   - Fetching rows in a loop
      *   - Closing the cursor
      *   - Cleaning up and disconnecting
      ******************************************************************
       IDENTIFICATION              DIVISION.
       PROGRAM-ID.                 cursor_run.
       DATA                        DIVISION.
       WORKING-STORAGE             SECTION.


       01  DBNAME                  PIC  X(30) VALUE SPACE.
       01  USERNAME                PIC  X(30) VALUE SPACE.
       01  PASSWD                  PIC  X(10) VALUE SPACE.

       01  EMP-NO                  PIC  9(04) VALUE ZERO.
       01  EMP-NAME                PIC  X(20) VALUE SPACE.
       01  EMP-SALARY              PIC  9(08) VALUE ZERO.

       01  F-NO                    PIC  9(04) VALUE ZERO.
       01  F-NAME                  PIC  X(20) VALUE SPACE.
       01  F-SALARY                PIC  9(08) VALUE ZERO.

       01  ROW-COUNT               PIC  9(04) VALUE ZERO.

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
      * Create table and insert test data
      *---------------------------------------------------------------
           EXEC SQL
               DROP TABLE IF EXISTS esql_cursor_emp
           END-EXEC.

           EXEC SQL
               CREATE TABLE esql_cursor_emp (
                   emp_no     INTEGER,
                   emp_name   VARCHAR(20),
                   emp_salary INTEGER
               )
           END-EXEC.
           PERFORM CHECK-SQL.

           MOVE 1 TO EMP-NO.
           MOVE "Alice" TO EMP-NAME.
           MOVE 50000 TO EMP-SALARY.
           EXEC SQL
               INSERT INTO esql_cursor_emp
                   VALUES (:EMP-NO, :EMP-NAME, :EMP-SALARY)
           END-EXEC.
           PERFORM CHECK-SQL.

           MOVE 2 TO EMP-NO.
           MOVE "Bob" TO EMP-NAME.
           MOVE 60000 TO EMP-SALARY.
           EXEC SQL
               INSERT INTO esql_cursor_emp
                   VALUES (:EMP-NO, :EMP-NAME, :EMP-SALARY)
           END-EXEC.
           PERFORM CHECK-SQL.

           MOVE 3 TO EMP-NO.
           MOVE "Charlie" TO EMP-NAME.
           MOVE 55000 TO EMP-SALARY.
           EXEC SQL
               INSERT INTO esql_cursor_emp
                   VALUES (:EMP-NO, :EMP-NAME, :EMP-SALARY)
           END-EXEC.
           PERFORM CHECK-SQL.

           MOVE 4 TO EMP-NO.
           MOVE "Diana" TO EMP-NAME.
           MOVE 65000 TO EMP-SALARY.
           EXEC SQL
               INSERT INTO esql_cursor_emp
                   VALUES (:EMP-NO, :EMP-NAME, :EMP-SALARY)
           END-EXEC.
           PERFORM CHECK-SQL.
           DISPLAY "4 rows inserted.".

      *---------------------------------------------------------------
      * Declare and open cursor
      *---------------------------------------------------------------
           EXEC SQL
               DECLARE emp_cur CURSOR FOR
                   SELECT emp_no, emp_name, emp_salary
                       FROM esql_cursor_emp
                       ORDER BY emp_no
           END-EXEC.

           EXEC SQL OPEN emp_cur END-EXEC.
           PERFORM CHECK-SQL.
           DISPLAY "Cursor opened.".

      *---------------------------------------------------------------
      * Fetch rows in a loop
      *---------------------------------------------------------------
           MOVE ZERO TO ROW-COUNT.
           PERFORM FETCH-LOOP UNTIL SQLCODE NOT = ZERO.
           DISPLAY "Total rows fetched: " ROW-COUNT.

      *---------------------------------------------------------------
      * Close cursor and clean up
      *---------------------------------------------------------------
           EXEC SQL CLOSE emp_cur END-EXEC.
           DISPLAY "Cursor closed.".

           EXEC SQL
               DROP TABLE esql_cursor_emp
           END-EXEC.
           DISPLAY "Table dropped.".

           EXEC SQL COMMIT END-EXEC.
           EXEC SQL DISCONNECT ALL END-EXEC.
           DISPLAY "Disconnected.".

           STOP RUN.

      ******************************************************************
       FETCH-LOOP.
      ******************************************************************
           EXEC SQL
               FETCH emp_cur INTO :F-NO, :F-NAME, :F-SALARY
           END-EXEC.
           IF SQLCODE = ZERO
               ADD 1 TO ROW-COUNT
               DISPLAY "  Row " ROW-COUNT ": "
                   "No=" F-NO " Name=" F-NAME " Salary=" F-SALARY
           END-IF.

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

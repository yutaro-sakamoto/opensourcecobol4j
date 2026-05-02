      ******************************************************************
      * sample.cbl - Basic Embedded SQL example
      *
      * This program demonstrates:
      *   - Connecting to PostgreSQL
      *   - Creating a table
      *   - Inserting rows using host variables
      *   - Selecting data back with SELECT INTO
      *   - Displaying results
      *   - Dropping the table and disconnecting
      *
      * Placeholders <|DB_NAME|>, <|DB_HOST|>, etc. are replaced by
      * the Makefile before compilation.
      ******************************************************************
       IDENTIFICATION              DIVISION.
       PROGRAM-ID.                 sample_run.
       DATA                        DIVISION.
       WORKING-STORAGE             SECTION.

      * SQL Communication Area

      * Host variables for database connection
       01  DBNAME                  PIC  X(30) VALUE SPACE.
       01  USERNAME                PIC  X(30) VALUE SPACE.
       01  PASSWD                  PIC  X(10) VALUE SPACE.

      * Host variables for employee data
       01  EMP-NO                  PIC  9(04) VALUE ZERO.
       01  EMP-NAME                PIC  X(20) VALUE SPACE.
       01  EMP-SALARY              PIC  9(08) VALUE ZERO.

      * Host variables for reading back
       01  OUT-NO                  PIC  9(04) VALUE ZERO.
       01  OUT-NAME                PIC  X(20) VALUE SPACE.
       01  OUT-SALARY              PIC  9(08) VALUE ZERO.

       PROCEDURE                   DIVISION.
       MAIN-RTN.

      *---------------------------------------------------------------
      * Step 1: Connect to PostgreSQL
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
           DISPLAY "Connected to database.".

      *---------------------------------------------------------------
      * Step 2: Create table
      *---------------------------------------------------------------
           EXEC SQL
               DROP TABLE IF EXISTS esql_sample_emp
           END-EXEC.

           EXEC SQL
               CREATE TABLE esql_sample_emp (
                   emp_no     INTEGER,
                   emp_name   VARCHAR(20),
                   emp_salary INTEGER
               )
           END-EXEC.
           IF SQLCODE NOT = ZERO
               DISPLAY "CREATE TABLE failed: " SQLCODE
               EXEC SQL ROLLBACK END-EXEC
               STOP RUN
           END-IF.
           DISPLAY "Table created.".

      *---------------------------------------------------------------
      * Step 3: Insert rows using host variables
      *---------------------------------------------------------------
           MOVE 1     TO EMP-NO.
           MOVE "Alice"  TO EMP-NAME.
           MOVE 50000  TO EMP-SALARY.
           EXEC SQL
               INSERT INTO esql_sample_emp
                   (emp_no, emp_name, emp_salary)
                   VALUES (:EMP-NO, :EMP-NAME, :EMP-SALARY)
           END-EXEC.
           PERFORM CHECK-SQL.

           MOVE 2     TO EMP-NO.
           MOVE "Bob"    TO EMP-NAME.
           MOVE 60000  TO EMP-SALARY.
           EXEC SQL
               INSERT INTO esql_sample_emp
                   (emp_no, emp_name, emp_salary)
                   VALUES (:EMP-NO, :EMP-NAME, :EMP-SALARY)
           END-EXEC.
           PERFORM CHECK-SQL.

           MOVE 3     TO EMP-NO.
           MOVE "Charlie" TO EMP-NAME.
           MOVE 55000  TO EMP-SALARY.
           EXEC SQL
               INSERT INTO esql_sample_emp
                   (emp_no, emp_name, emp_salary)
                   VALUES (:EMP-NO, :EMP-NAME, :EMP-SALARY)
           END-EXEC.
           PERFORM CHECK-SQL.
           DISPLAY "3 rows inserted.".

      *---------------------------------------------------------------
      * Step 4: Select a row back using SELECT INTO
      *---------------------------------------------------------------
           MOVE 2 TO EMP-NO.
           EXEC SQL
               SELECT emp_no, emp_name, emp_salary
                   INTO :OUT-NO, :OUT-NAME, :OUT-SALARY
                   FROM esql_sample_emp
                   WHERE emp_no = :EMP-NO
           END-EXEC.
           PERFORM CHECK-SQL.

           DISPLAY "Selected row:".
           DISPLAY "  No:     " OUT-NO.
           DISPLAY "  Name:   " OUT-NAME.
           DISPLAY "  Salary: " OUT-SALARY.

      *---------------------------------------------------------------
      * Step 5: Clean up - drop table, commit, disconnect
      *---------------------------------------------------------------
           EXEC SQL
               DROP TABLE esql_sample_emp
           END-EXEC.
           DISPLAY "Table dropped.".

           EXEC SQL COMMIT END-EXEC.

           EXEC SQL
               DISCONNECT ALL
           END-EXEC.
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

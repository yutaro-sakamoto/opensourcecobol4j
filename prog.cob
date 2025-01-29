       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       ENVIRONMENT      DIVISION.
       INPUT-OUTPUT     SECTION.
       FILE-CONTROL.
       SELECT テストファイル ASSIGN TO "./テスト_ファイル"
                        ORGANIZATION IS  INDEXED
                        ACCESS MODE  IS  SEQUENTIAL
                        RECORD KEY   IS  TEST-KEY.
       DATA             DIVISION.
       FILE             SECTION.
       FD テストファイル.
       01 TEST-KEY      PIC X(10).
       PROCEDURE        DIVISION.
           OPEN  OUTPUT テストファイル.
           MOVE "TEST000000" TO TEST-KEY.
           WRITE TEST-KEY.
           CLOSE テストファイル.
           OPEN  INPUT  テストファイル.
           READ  テストファイル
               NOT AT END
                   DISPLAY "OK" NO ADVANCING
           END-READ.
           CLOSE テストファイル.
           STOP RUN.
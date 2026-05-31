# Embedded SQL (ESQL) ガイド -- opensource COBOL 4J

## 概要

opensource COBOL 4Jは、COBOLプログラムからPostgreSQLデータベースに直接アクセスするためのEmbedded SQL (EXEC SQL) をサポートしています。COBOL のホスト変数をSQLステートメントのバインドパラメータとして使用することで、COBOLとSQLの間でシームレスにデータをやり取りできます。

`EXEC SQL ... END-EXEC` ステートメントを含むCOBOLソースファイルを `cobj` でコンパイルすると、コンパイラはEmbedded SQLをJDBCを介したJavaデータベース呼び出しに変換します。

## 前提条件

- **opensource COBOL 4J** がインストール済みであること（[README_JP.md](../README_JP.md) を参照）
- **PostgreSQL** サーバー（バージョン9.6以降）
- **Java**（JDK 11以降）
- PostgreSQL JDBCドライバーは `libcobj.jar` にバンドルされています

## クイックスタート

以下は、PostgreSQLに接続し、行を挿入して読み戻し、切断する最小限のCOBOLプログラムです。

```cobol
       IDENTIFICATION              DIVISION.
       PROGRAM-ID.                 QUICK-START.
       DATA                        DIVISION.
       WORKING-STORAGE             SECTION.

       EXEC SQL BEGIN DECLARE SECTION END-EXEC.
       01  DBNAME    PIC X(30) VALUE "testdb@localhost:5432".
       01  USERNAME  PIC X(30) VALUE "main_user".
       01  PASSWD    PIC X(10) VALUE "password".
       01  EMP-NAME  PIC X(20).
       EXEC SQL END DECLARE SECTION END-EXEC.

       PROCEDURE                   DIVISION.
           EXEC SQL
               CONNECT :USERNAME IDENTIFIED BY :PASSWD USING :DBNAME
           END-EXEC.

           MOVE "Alice" TO EMP-NAME.
           EXEC SQL
               INSERT INTO employees (name) VALUES (:EMP-NAME)
           END-EXEC.

           MOVE SPACES TO EMP-NAME.
           EXEC SQL
               SELECT name INTO :EMP-NAME FROM employees LIMIT 1
           END-EXEC.
           DISPLAY "Employee: " EMP-NAME.

           EXEC SQL COMMIT END-EXEC.
           EXEC SQL DISCONNECT ALL END-EXEC.
           STOP RUN.
```

コンパイルと実行:

```bash
cobj -I /usr/lib/opensourcecobol4j/copy quick-start.cbl
java QUICK-START
```

## サポートされるSQL文

### CONNECT / DISCONNECT

```cobol
       EXEC SQL
           CONNECT :USERNAME IDENTIFIED BY :PASSWD USING :DBNAME
       END-EXEC.

       EXEC SQL
           DISCONNECT ALL
       END-EXEC.
```

`:DBNAME` の接続文字列形式:

```
dbname@host:port
```

例: `"testdb@localhost:5432"`

### BEGIN / END DECLARE SECTION

`EXEC SQL BEGIN DECLARE SECTION END-EXEC` と `EXEC SQL END DECLARE SECTION END-EXEC` は後方互換のために受理されますが、**無視されます**。WORKING-STORAGE SECTIONおよびLINKAGE SECTIONの全変数は、DECLARE SECTIONの有無にかかわらず、常にSQL文のホスト変数として使用できます。

```cobol
       WORKING-STORAGE SECTION.
       01  MY-VAR  PIC X(20).
       01  MY-NUM  PIC 9(5).
       PROCEDURE DIVISION.
           EXEC SQL
               INSERT INTO TBL VALUES (:MY-VAR, :MY-NUM)
           END-EXEC.
```

### ホスト変数の表記

ホスト変数として以下の形式を受け付けます。

| 表記 | 意味 |
|---|---|
| `:VAR` | 単純なホスト変数 |
| `:VAR(IDX)` | OCCURS 要素。`IDX` は整数リテラルまたは COBOL 変数。 |
| `:VAR(I, J)` | 多次元 OCCURS 要素 |
| `:GRP.SUB` | 集団項目で修飾されたホスト変数 (`GRP` 配下の `SUB`) |
| `:GRP.SUB(IDX)` | 集団修飾 + 添字 |
| `:GRP.SUB(GRP2.IDX)` | 添字値も集団修飾された COBOL 変数 |

添字に使う変数自身は添字を持てない (`:VAR(IDX(1))` のような入れ子添字は不可) という制約があります。間接的な添字が必要なら、COBOL 側でいったん作業変数に MOVE してから渡してください。

```cobol
       01  GRP.
         03 ROW OCCURS 5.
           05 VAL PIC 9(4).
       01  GRP2.
         03 TMP-IDX PIC S9(2).
       01  IDX PIC S9(2).
       ...
           MOVE 3 TO TMP-IDX OF GRP2.
           EXEC SQL
               SELECT FIELD INTO :GRP.VAL(GRP2.TMP-IDX)
                 FROM TESTTABLE WHERE N = :IDX
           END-EXEC.
```

### SQLCA（SQL通信領域）

SQLCAは、`EXEC SQL` 文を使用するプログラムで自動的に定義されます。各SQL文の実行後に `SQLCODE`、`SQLSTATE`、`SQLERRMC` などの診断フィールドが利用可能になります。

後方互換性のため、`EXEC SQL INCLUDE SQLCA END-EXEC` は引き続き使用できますが、記述する必要はありません。

暗黙に定義されるSQLCAの構造は以下の通りです：

```cobol
       01  SQLCA GLOBAL.
           05  SQLCAID               PIC X(8).
           05  SQLCABC               PIC S9(9) COMP-5.
           05  SQLCODE               PIC S9(9) COMP-5.
           05  SQLERRM.
           49  SQLERRML              PIC S9(4) COMP-5.
           49  SQLERRMC              PIC X(70).
           05  SQLERRP               PIC X(8).
           05  SQLERRD OCCURS 6 TIMES
                                     PIC S9(9) COMP-5.
           05  SQLWARN.
               10 SQLWARN0           PIC X(1).
               10 SQLWARN1           PIC X(1).
               10 SQLWARN2           PIC X(1).
               10 SQLWARN3           PIC X(1).
               10 SQLWARN4           PIC X(1).
               10 SQLWARN5           PIC X(1).
               10 SQLWARN6           PIC X(1).
               10 SQLWARN7           PIC X(1).
           05  SQLSTATE              PIC X(5).
```

| フィールド | 説明 |
|-----------|------|
| `SQLCODE` | 戻りコード：0 = 成功、100 = データなし、負の値 = エラー |
| `SQLSTATE` | 5文字のSQL状態コード（例：`"00000"`, `"02000"`, `"08001"`） |
| `SQLERRMC` | エラーメッセージテキスト（最大70文字） |
| `SQLERRML` | `SQLERRMC` 内のエラーメッセージの長さ |
| `SQLERRD(3)` | 直前の文で影響を受けた行数 |

### SELECT INTO

単一行の取得:

```cobol
       EXEC SQL
           SELECT emp_name, emp_salary
               INTO :EMP-NAME, :EMP-SALARY
               FROM employees
               WHERE emp_no = :EMP-NO
       END-EXEC.
```

OCCURS を使った配列取得:

```cobol
       01  EMP-NAMES.
           05 EMP-NAME PIC X(20) OCCURS 10 TIMES.

       EXEC SQL
           SELECT emp_name INTO :EMP-NAME FROM employees
       END-EXEC.
```

### INSERT, UPDATE, DELETE

ホスト変数の前に `:` を付けてバインドパラメータとして使用します:

```cobol
       EXEC SQL
           INSERT INTO employees (emp_no, emp_name, emp_salary)
               VALUES (:EMP-NO, :EMP-NAME, :EMP-SALARY)
       END-EXEC.

       EXEC SQL
           UPDATE employees SET emp_salary = :NEW-SALARY
               WHERE emp_no = :EMP-NO
       END-EXEC.

       EXEC SQL
           DELETE FROM employees WHERE emp_no = :EMP-NO
       END-EXEC.
```

### DECLARE CURSOR / OPEN / FETCH / CLOSE

```cobol
       EXEC SQL
           DECLARE emp_cursor CURSOR FOR
               SELECT emp_no, emp_name FROM employees
               ORDER BY emp_no
       END-EXEC.

       EXEC SQL OPEN emp_cursor END-EXEC.

       PERFORM UNTIL SQLCODE NOT = ZERO
           EXEC SQL
               FETCH emp_cursor INTO :EMP-NO, :EMP-NAME
           END-EXEC
           IF SQLCODE = ZERO
               DISPLAY "No: " EMP-NO " Name: " EMP-NAME
           END-IF
       END-PERFORM.

       EXEC SQL CLOSE emp_cursor END-EXEC.
```

### PREPARE / EXECUTE

```cobol
       01  SQL-STMT  PIC X(200).

       MOVE "INSERT INTO employees (emp_no, emp_name) VALUES (?, ?)"
           TO SQL-STMT.
       EXEC SQL
           PREPARE stmt1 FROM :SQL-STMT
       END-EXEC.

       EXEC SQL
           EXECUTE stmt1 USING :EMP-NO, :EMP-NAME
       END-EXEC.
```

### COMMIT / ROLLBACK

```cobol
       EXEC SQL COMMIT END-EXEC.
       EXEC SQL ROLLBACK END-EXEC.
```

## ホスト変数の型マッピング

以下の表は、COBOLのPIC句がSQLの型にどのようにマッピングされるかを示しています:

| COBOL PIC句 | SQL型 | 備考 |
|---|---|---|
| `PIC X(n)` | CHAR / VARCHAR | 英数字文字列 |
| `PIC 9(n)` | NUMERIC | 符号なし整数 |
| `PIC S9(n)` | NUMERIC | 符号付き整数 |
| `PIC 9(n)V9(m)` | DECIMAL | 固定小数点数 |
| `PIC 9(n) USAGE COMP-3` | NUMERIC | パック10進数 |
| `PIC 9(n) USAGE COMP-5` | INTEGER / BIGINT | ネイティブバイナリ整数 |
| `PIC N(n)` | NATIONAL CHARACTER | 日本語文字 |

## SQLCAによるエラーハンドリング

各 `EXEC SQL` 文の実行後、SQLCAフィールドが更新されます:

| フィールド | 説明 |
|---|---|
| `SQLCODE` | 0 = 成功、+100 = レコードなし、負の値 = エラー |
| `SQLSTATE` | 5文字のSQL状態コード |
| `SQLERRMC` | エラーメッセージテキスト |
| `SQLERRD(3)` | 影響を受けた行数 |

主なSQLCODEの値:

| SQLCODE | 意味 |
|---|---|
| 0 | 成功 |
| +100 | レコードが見つからない / カーソルの終端 |
| -1 | 接続失敗 |
| -20 | 内部エラー |
| -30 | PostgreSQLエラー（SQLSTATEとSQLERRMCを参照） |

エラーハンドリングの例:

```cobol
       IF SQLCODE NOT = ZERO
           DISPLAY "SQL Error: " SQLCODE
           DISPLAY "State: " SQLSTATE
           DISPLAY "Message: " SQLERRMC
           EXEC SQL ROLLBACK END-EXEC
           STOP RUN
       END-IF.
```

## 接続文字列の形式

CONNECTに渡す接続文字列は以下の形式です:

```
dbname@host:port
```

例:

```cobol
       MOVE "testdb@localhost:5432" TO DBNAME.
```

## 環境変数

| 変数名 | 説明 |
|---|---|
| `OCDB_DB_NAME` | デフォルトのデータベース名 |
| `OCDB_DB_USER` | デフォルトのデータベースユーザー |
| `OCDB_DB_PASS` | デフォルトのデータベースパスワード |
| `OCDB_DB_CHAR` | データベース接続の文字エンコーディング |

## コンパイル方法

```bash
# EXEC SQLを含むCOBOLプログラムをコンパイル
cobj -I /usr/lib/opensourcecobol4j/copy program.cbl

# コンパイルしたプログラムを実行
java program
```

`-I` フラグは、COPYファイルが格納されているディレクトリを指定します。

## 制限事項

- COBOL 標準の `OF` 修飾 (`:VAR OF GRP`) はサポートされません。dotted 修飾 (`:GRP.VAR`) を使用してください。
- 添字の値に算術式 (`:VAR(I+1)`) を書いたり、添字値そのものが添字を持つホスト変数 (`:VAR(IDX(1))`) を書いたりすることはできません。間接的な添字が必要な場合は、COBOL 側でいったん作業変数に MOVE してから渡してください。
- SJISモードでのUTF-8変数名はサポートされていません。UTF-8ソースファイルの場合は `--enable-utf8` ビルドオプションを使用してください。
- 対象データベースはPostgreSQLのみサポートされています。

内部アーキテクチャ (どのように解析・コード生成されているか) については [esql-design_JP.md](./esql-design_JP.md) を参照してください。

## サンプル

[example/esql/](../example/esql/) ディレクトリに実行可能なサンプルがあります:

- **sample.cbl** -- 基本的なCONNECT、INSERT、SELECT、DISCONNECT
- **cursor.cbl** -- カーソルを使った行の反復処理
- **prepare.cbl** -- EXECUTE USINGによるプリペアドステートメント

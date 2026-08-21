# EXEC JAVA ガイド -- opensource COBOL 4J

## 概要

opensource COBOL 4J は `EXEC JAVA ... END-EXEC` をサポートしています。これは COBOL プログラムの中に Java のソースコードを直接記述するための文です。

`cobj` は COBOL を Java に変換するため、`EXEC JAVA` ブロックに書いた Java コードには特別な橋渡しが不要です。ブロックの内容は、生成される Java ソースの対応する位置にそのまま埋め込まれます。ブロックの中では、[埋め込み SQL](./esql-guide_JP.md) と同じホスト変数の記法 `:NAME` で COBOL のデータ項目を参照できます。

Java のライブラリを COBOL から呼び出したい場合や、COBOL では書きにくい小さな処理を、別の Java クラスと `CALL` 文に分けずにその場で書きたい場合に利用します。

専用のコンパイルオプションや追加のランタイムライブラリは必要ありません。

## クイックスタート

```cobol
       IDENTIFICATION DIVISION.
       PROGRAM-ID. QUICK-START.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01 WRK-NUM PIC 9(4) VALUE 0.
       PROCEDURE DIVISION.
           MOVE 10 TO WRK-NUM.
           EXEC JAVA
              int n = :WRK-NUM.getInt();
              n = n * 3;
              :WRK-NUM.setInt(n);
              System.out.println("java n=" + n);
           END-EXEC.
           DISPLAY "NUM=" WRK-NUM.
           STOP RUN.
```

コンパイルと実行:

```bash
cobj quick-start.cbl
java QUICK-START
```

実行結果:

```
java n=30
NUM=0030
```

## 構文

```cobol
           EXEC JAVA
               <Javaの文>
           END-EXEC.

           EXEC JAVA IMPORT
               <Javaのimport宣言>
           END-EXEC.

           EXEC JAVA CLASS-MEMBER
               <Javaのフィールド・メソッド・ネストクラスの宣言>
           END-EXEC.
```

- `EXEC JAVA` は PROCEDURE DIVISION 内の、COBOL の文が書ける位置であればどこにでも記述できます。残る 2 つの形式、[`EXEC JAVA IMPORT`](#クラスの-import) と [`EXEC JAVA CLASS-MEMBER`](#クラスメンバの宣言) は、その位置に加えて DATA DIVISION にも記述できます。
- `EXEC`、`JAVA`、`IMPORT`、`CLASS-MEMBER`、`END-EXEC` は他の COBOL 語と同様に大文字・小文字を区別せずに認識されます。その間に書いた Java コードの大文字・小文字はそのまま保持されます。
- ブロックの内容は、その文が現れる位置の生成 Java ソースへそのまま出力されます。コンパイラが行うのは、ブロックの全行に共通する行頭の字下げを取り除き、周囲の生成コードに合わせて字下げし直すことだけで、それ以外に Java コードを書き換えることはありません。
- ブロックの中身は Java の**文**の並びです。生成されるクラスのメソッド本体の中に埋め込まれるため、クラスやファイルのレベルでしか書けない宣言（メソッド宣言、フィールド宣言）は記述できません。`import` 宣言には専用の `EXEC JAVA IMPORT` ブロックがあります。[クラスの import](#クラスの-import)を参照してください。

## ホスト変数 (`:NAME`)

ブロック内に `:NAME` と書くと、COBOL のデータ項目 `NAME` を参照できます。生成される Java では、`:NAME` はその項目を保持する `AbstractCobolField` オブジェクトに置換されるため、libcobj の API を通じて COBOL のデータを読み書きできます。

```cobol
       01 WRK-STR PIC X(5) VALUE SPACE.
       01 WRK-SUB PIC 9(2) VALUE 5.
      *...
           EXEC JAVA
              :WRK-STR.moveFrom("abc");
              :WRK-SUB.setInt(:WRK-SUB.getInt() + 1);
           END-EXEC.
```

置換された結果は通常の Java の式なので、他のメソッドの引数やメソッド呼び出しのレシーバなど、式が書ける場所であればどこにでも記述できます。

### よく使うメソッド

ホスト変数はすべて [`AbstractCobolField`](https://opensourcecobol.github.io/opensourcecobol4j/javadoc/libcobj/index.html) のインスタンスです。`EXEC JAVA` ブロックからよく使うメソッドを以下に示します。

| メソッド | 説明 |
|---|---|
| `getInt()` | 項目の値を `int` として取得する。 |
| `getDouble()` | 項目の値を `double` として取得する。 |
| `getDecimal()` | 項目の値を `CobolDecimal` として取得する。 |
| `getString()` | 項目の値を `String` として取得する（英数字項目では末尾の空白も含む）。 |
| `getBytes()` | 項目のバイト列をそのまま取得する。 |
| `setInt(int)` | `int` の値を項目に格納する。 |
| `moveFrom(String)` | `String` を COBOL の `MOVE` の意味論（パディング・切り捨て）で項目に格納する。 |
| `moveFrom(int)`, `moveFrom(double)`, `moveFrom(BigDecimal)` | 数値を COBOL の `MOVE` の意味論で項目に格納する。 |
| `moveFrom(AbstractCobolField)` | 他の COBOL 項目の値を項目に格納する。 |
| `getSize()` | 項目のバイト数。 |

これらのうち `moveFrom(BigDecimal)`、`addInt(int)`、`subInt(int)` などは `CobolStopRunException` を宣言しています。これは生成されるメソッドが既に宣言している検査例外なので、そのまま送出させて構いません。[例外](#例外)を参照してください。

### `:NAME` が置換されない場所

コンパイラは Java の文字列リテラル、文字リテラル、行コメント (`//`)、ブロックコメント (`/* ... */`) を認識します。これらの内側では `:NAME` は書いたとおりに残り、`END-EXEC` もブロックの終端として扱われません。

```cobol
           EXEC JAVA
              // このコメント内の :WRK-A は置換されない
              /* :WRK-A も END-EXEC もここでは単なる文字列 */
              String s = ":WRK-A stays literal";
              char c = ':';
              System.out.println(s + c);
              :WRK-A.setInt(7);
           END-EXEC.
```

このプログラムは `:WRK-A stays literal:` を表示し、`WRK-A` に 7 を設定します。

### 未定義の名前はコンパイルエラー

`:NAME` がプログラムのデータ項目に解決できない場合、`cobj` はエラーを報告しコンパイルは失敗します。データ項目以外（段落名など）に解決された場合も同様です。

```
prog.cbl:8: Error: 'NO-SUCH-VAR' undefined
prog.cbl:8: Error: EXEC JAVA: ':PARA-NAME' is not a data item
```

エラーが報告される行は、ブロック内で参照が現れた行ではなく `EXEC JAVA` 文の開始行です。[`EXEC JAVA CLASS-MEMBER`](#クラスメンバの宣言) ブロックでも同様です (このブロックのホスト変数は、プログラム全体の解析が終わった時点で解決されます)。

## ブロック内での Java の書き方

### 使用できるクラス

生成される Java ソースは libcobj の各パッケージを import しているため、`AbstractCobolField`、`CobolDataStorage`、`CobolRuntimeException`、`CobolStopRunException` などは単純名で使用できます。`java.lang` のクラス（`String`、`Integer`、`System` など）も通常どおり使用できます。

それ以外のクラスは、完全修飾名で参照するか、[`EXEC JAVA IMPORT`](#クラスの-import) ブロックで import して単純名で使用します。

```cobol
           EXEC JAVA
              java.util.List<String> names = new java.util.ArrayList<>();
              names.add(:WRK-STR.getString().trim());
              System.out.println(names);
           END-EXEC.
```

### クラスの import

`EXEC JAVA` ブロックはメソッド本体の中に置かれるため、その中に `import` 宣言を書くことはできません。代わりに `EXEC JAVA IMPORT ~ END-EXEC` ブロックに import 宣言を記述します。

```cobol
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       EXEC JAVA IMPORT
          import java.util.ArrayList;
          import java.util.List;
          import static java.lang.Math.max;
       END-EXEC.
      *...
       PROCEDURE DIVISION.
           EXEC JAVA
              List<Integer> xs = new ArrayList<Integer>();
              xs.add(3);
              :WRK-N.setInt(max(xs.get(0), 5));
           END-EXEC.
```

- 宣言は、生成される `.java` ファイル先頭の import 部 (libcobj の import の後) に出力されます。
- ブロックは DATA DIVISION (`EXEC SQL` の宣言が書ける WORKING-STORAGE SECTION 等) にも、PROCEDURE DIVISION の文の位置にも記述できます。どこに書いても、そのプログラムの生成ファイル全体に効きます。可読性のため、プログラムの先頭付近に書くことを推奨します。
- ブロックの中身に書けるのは `import` 宣言 (`import a.b.C;`、`import static a.b.C.d;`、ワイルドカード形式 `import a.b.*;` / `import static a.b.C.*;`) と Java のコメントだけです。それ以外を書くとコンパイルエラーになります (生成ファイルの先頭に壊れたテキストが出力されると、`javac` のエラーが極めて分かりにくくなるためです)。import する名前の構成要素に Java の予約語は書けず、名前の途中に空白を入れることもできません (`import java . util . List;` はエラーになります)。名前に使えるのは ASCII の識別子のみです。宣言の間の余分な `;` は読み飛ばされます。
- ブロック内の誤った宣言はすべて報告されるため、1 回のコンパイルで問題を一覧できます。また、エラーが指す行はブロックの開始行ではなく、その宣言が書かれた行です。

  ```
  prog.cbl:7: Error: EXEC JAVA IMPORT: not an import declaration: 'int x = 1'
  prog.cbl:8: Error: EXEC JAVA IMPORT: invalid import declaration: 'import int.foo'
  prog.cbl:9: Error: EXEC JAVA IMPORT: missing ';' in import declaration: 'import java.util.Map'
  ```

- `EXEC JAVA IMPORT` ブロック内ではホスト変数 (`:NAME`) は使用できません。
- 同じ宣言が複数回現れた場合 (ブロックの重複など) は 1 つにまとめて出力されます。比較は正規化した後で行われるため、字句の間の空白、改行、コメントの違いがあってもまとめられます。通常の import と `static` import は別の宣言として扱われます。
- DATA DIVISION 内では、後ろにデータ記述項が続く場合は `EXEC SQL` と同様に分離符のピリオド付きで `END-EXEC.` と書いてください。

### クラスメンバの宣言

`EXEC JAVA` ブロックに書けるのは文だけなので、メソッドやフィールドの宣言はできません。生成クラスにメンバ (フィールド、メソッド、ネストクラスなど) を追加するには、`EXEC JAVA CLASS-MEMBER ~ END-EXEC` ブロックを使います。

```cobol
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       EXEC JAVA CLASS-MEMBER
          private int counter = 0;
          private int nextCounter() {
             return ++counter;
          }
          private String greet(String name) {
             return "hello, " + name + " #" + :WRK-N.getInt();
          }
       END-EXEC.
       01 WRK-N PIC 9(2) VALUE 0.
      *...
       PROCEDURE DIVISION.
           EXEC JAVA
              :WRK-N.setInt(nextCounter());
           END-EXEC.
```

- ブロックの内容は生成クラスのクラス本体に出力されます。`EXEC JAVA` と同様に `:NAME` はホスト変数のフィールドに置換され、そのフィールドは同じクラスのインスタンスフィールドなので、インスタンスメソッドの中から読み書きできます。上の例のように、ブロックがデータ項目の定義より前にあっても参照できます。
- ブロックは `EXEC JAVA IMPORT` と同じ場所 (DATA DIVISION、または PROCEDURE DIVISION の文の位置) に書けます。文の位置に書いた場合、その位置に実行コードは生成されません (宣言がクラス本体に出力されるだけです)。
- 宣言したメンバは同じプログラムのどの `EXEC JAVA` ブロックからも使えます。逆にメンバメソッド同士の呼び出しや、メンバメソッドからのホスト変数の読み書きもできます。
- ホスト変数の参照はメソッドの中でのみ行ってください。フィールド初期化子やインスタンス初期化子ブロックから参照しては**いけません**。メンバの宣言はクラス本体の中でホスト変数のフィールドより前に出力されるため、`javac` が `illegal forward reference` として拒否します。さらに、ホスト変数のオブジェクトはコンストラクタ本体が呼ぶ `init()` で生成され、フィールド初期化子・インスタンス初期化子はコンストラクタ本体より先に実行されるため、このエラーを回避する書き方 (`this.:WRK-N` のようにフィールドを修飾する) をしても値は `null` です。
- `static` メンバからはホスト変数を参照できません。ホスト変数のフィールドはインスタンスフィールドであり、`javac` が `non-static variable ... cannot be referenced from a static context` を報告します。
- `EXEC JAVA` と同様、本文は `cobj` では検査されません。メンバ宣言の誤りは生成された `.java` ファイルに対する `javac` のエラーとして報告されます。

### ローカル変数のスコープ

ブロック内で宣言した Java のローカル変数は、その `EXEC JAVA` ブロックの中でのみ有効です。したがって、同じプログラム内の 2 つのブロックで同じ名前の変数を宣言できます。

```cobol
           EXEC JAVA
              int n = 1;
              :WRK-A.setInt(n);
           END-EXEC.
           EXEC JAVA
              int n = 2;
              :WRK-A.setInt(:WRK-A.getInt() + n);
           END-EXEC.
```

値を次のブロックへ引き継ぎたい場合は、COBOL のデータ項目に格納してください。

### 制御の流れ

ブロックの直下に `return`、`break`、`continue` を**書いてはいけません**。生成される Java は COBOL の制御構造（`PERFORM`、`GO TO`、段落の実行順序）を Java の制御構造で実現しているため、これらのキーワードはその流れを壊し、実行順序の誤りや Java のコンパイルエラーを引き起こします。

ブロック内で完結する制御構造（`if`、`for` ループとそのループに対応する `break` など）は問題ありません。

### 例外

ブロックが埋め込まれる生成メソッドは `throws CobolRuntimeException, CobolStopRunException` として宣言されています。したがって次のようになります。

- `CobolRuntimeException`、`CobolStopRunException`、および非検査例外は、送出しても、そのまま伝播させても構いません。
- それ以外の検査例外はブロック内で捕捉しなければなりません。呼び出す Java コードが検査例外（`IOException`、`SQLException` など）を宣言している場合は、`try`/`catch` で囲んでください。

```cobol
           EXEC JAVA
              try {
                 java.nio.file.Files.writeString(
                    java.nio.file.Path.of("out.txt"), :WRK-STR.getString());
              } catch (java.io.IOException e) {
                 System.err.println("write failed: " + e);
              }
           END-EXEC.
```

## 制限事項と注意点

- **コロンの直後に英数字が続くとホスト変数参照とみなされます。** 三項演算子やラベルなどでコロンを別の意味で使う場合は、コロンの後に空白を入れてください。

  ```java
  int m = (n > 0) ? 1 : 0;      // OK: ':' の後に空白がある
  int m = (n > 0) ? 1 :0;       // NG: ':0' がホスト変数参照とみなされる
  ```

  メソッド参照 (`::`) は正しく認識され、そのまま出力されます。`String::valueOf` も `:WRK-STR::toString` も期待どおり動作します。

- **ハイフンは COBOL 語の一部とみなされます。** `:N-1` は「`:N` から 1 を引く」ではなく、データ項目 `N-1` の参照です。減算は空白を入れて書いてください。

  ```java
  int m = :N - 1;               // OK
  int m = :N-1;                 // データ項目 N-1 の参照
  ```

  COBOL の利用者語はハイフンで終われないため、名前の末尾のハイフンは名前に含まれません。

- **修飾参照は使用できません。** `X OF GRP` のような修飾は書けません。`:TBL(1)` のような添字も認識されず、括弧は Java のコードとしてそのまま出力されます。名前はプログラム内で一意に解決できる必要があります。修飾や添字が必要な項目は、いったん作業項目に `MOVE` してからその項目を参照してください。

- **固定形式のソースは 72 桁までしか読まれません。** `EXEC SQL` と同様に、`EXEC JAVA`、`EXEC JAVA IMPORT`、`EXEC JAVA CLASS-MEMBER` ブロックの内容も通常の COBOL のソース読み込み処理を通ります。固定形式では 8〜72 桁だけがプログラムとして扱われ、73 桁目以降は捨てられます。長い Java の行は末尾が失われ、多くの場合は行末の `;` が欠けて、原因の分かりにくい Java の構文エラーになります (`EXEC JAVA IMPORT` ブロックでは `EXEC JAVA IMPORT: missing ';' in import declaration` というエラーになります)。Java の各行を十分短く保つか、`-Wcolumn-overflow` オプションを付けて 72 桁を超える行を警告させるか、自由形式（`-free`）でコンパイルしてください。

- **ユーザの import は libcobj のクラスを隠すことがあります。** 生成ファイルは libcobj の各パッケージをオンデマンド (`.*`) で import しており、単一型 import はそれより常に優先されます。libcobj には `Const`、`FileIO`、`IndexedFile`、`Linage` などありふれた単純名のクラスがあるため、同じ単純名の無関係なクラスを import すると**生成コード側**の参照が壊れることがあります。その場合は import をやめて完全修飾名で参照してください。

- **`END-EXEC` の無いブロックはコンパイルエラーになります。** ブロックが開いたままソースが終わった場合 (`END-EXEC` の書き忘れや、閉じていない `/* ... */` コメントが `END-EXEC` を飲み込んだ場合)、`cobj` は `... statement is not terminated by END-EXEC` を報告して失敗します。メッセージには開いたままになったブロックの形式 (`EXEC JAVA`、`EXEC JAVA IMPORT`、`EXEC JAVA CLASS-MEMBER`) が表示されます。

- **Java コードは `cobj` では検査されません。** 記述した Java の誤り（構文エラー、存在しないメソッド、型の不一致）は、生成された `.java` ファイルに対して Java コンパイラが報告します。そのため報告される行番号は、COBOL ソースではなく生成された Java ソースのものです。

- **他プログラムのデータ項目は参照できません。** 参照できるのは、その `EXEC JAVA` 文を含むプログラムのデータ項目だけです。

## コンパイル方法

専用のオプションは不要です。

```bash
# EXEC JAVAを含むCOBOLプログラムをコンパイル
cobj program.cbl

# コンパイルしたプログラムを実行
java program
```

### 生成されるコード

ブロックがどのように展開されたかを確認したい場合は、カレントディレクトリに生成される Java ソース（`[PROGRAM-ID].java`）を読んでください。他の COBOL の文と同様に、ブロックは Java のブロック `{ ... }` として出力され、その上に元のソースファイル名と `EXEC JAVA` 文の行番号を示すコメントが付きます。`:NAME` は、そのデータ項目のフィールドオブジェクトに置換されます。

```java
        /* ej.cbl:7: EXEC JAVA */
        {
          int n = f_WRK_NUM.getInt();
          f_WRK_NUM.setInt(n * 3);
        }
```

Java のローカル変数のスコープがブロック内に限られるのは、この `{ ... }` があるためです。

残る 2 つの形式は、ファイル内の別の場所に出力されます。`EXEC JAVA IMPORT` ブロックの宣言はファイル先頭の import 部 (libcobj の import の後) に追加されます。`EXEC JAVA CLASS-MEMBER` ブロックは、`/* EXEC JAVA CLASS-MEMBER declarations */` というコメントの下に、そのプログラムの他の `CLASS-MEMBER` ブロックのメンバとまとめてクラス本体へ出力されます。

```java
  /* EXEC JAVA CLASS-MEMBER declarations */
  private int counter = 0;
  private int nextCounter() {
     return ++counter;
  }
```

このブロックを PROCEDURE DIVISION の文の位置に書いた場合、その位置には `/* prog.cbl:8: EXEC JAVA CLASS-MEMBER */` というコメントが残るだけで、実行されるコードはありません。

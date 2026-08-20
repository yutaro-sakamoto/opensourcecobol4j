# EXEC JAVA Guide for opensource COBOL 4J

## Overview

opensource COBOL 4J supports `EXEC JAVA ... END-EXEC`, a statement that lets you write Java source code directly inside a COBOL program.

Because `cobj` translates COBOL into Java, the Java code you write in an `EXEC JAVA` block does not need any bridge: the compiler copies the block verbatim into the generated Java source file, at the position corresponding to the statement. Inside the block you can refer to COBOL data items with the host variable notation `:NAME`, exactly as in [Embedded SQL](./esql-guide.md).

Typical uses are calling a Java library from COBOL, and writing a small piece of logic that is awkward to express in COBOL, without splitting it out into a separate Java class and a `CALL` statement.

No compiler option and no additional runtime library are required.

## Quick Start

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

Compile and run:

```bash
cobj quick-start.cbl
java QUICK-START
```

Output:

```
java n=30
NUM=0030
```

## Syntax

```cobol
           EXEC JAVA
               <Java statements>
           END-EXEC.

           EXEC JAVA IMPORT
               <Java import declarations>
           END-EXEC.

           EXEC JAVA CLASS-MEMBER
               <Java field, method and nested class declarations>
           END-EXEC.
```

- `EXEC JAVA` can be written in the PROCEDURE DIVISION, anywhere a COBOL statement is allowed. The other two forms, [`EXEC JAVA IMPORT`](#importing-classes) and [`EXEC JAVA CLASS-MEMBER`](#declaring-class-members), can be written there as well as in the DATA DIVISION.
- `EXEC`, `JAVA`, `IMPORT`, `CLASS-MEMBER` and `END-EXEC` are recognised case-insensitively, like other COBOL words. The Java code between them keeps its original case.
- The block body is emitted as-is into the generated Java source, at the position where the statement appears. The compiler only removes the leading indentation that all lines of the block have in common and re-indents the block to match the surrounding generated code; it does not otherwise rewrite your Java.
- The body is a sequence of Java **statements**: it ends up inside a method body of the generated class. Declarations that are only valid at class or file level (method declarations, field declarations) cannot be written here. `import` declarations have a dedicated block form, `EXEC JAVA IMPORT`; see [Importing classes](#importing-classes).

## Host Variables (`:NAME`)

Writing `:NAME` inside the block references the COBOL data item `NAME`. In the generated Java, `:NAME` is replaced with the `AbstractCobolField` object that holds that item, so you read and write the COBOL data through the libcobj API.

```cobol
       01 WRK-STR PIC X(5) VALUE SPACE.
       01 WRK-SUB PIC 9(2) VALUE 5.
      *...
           EXEC JAVA
              :WRK-STR.moveFrom("abc");
              :WRK-SUB.setInt(:WRK-SUB.getInt() + 1);
           END-EXEC.
```

Because the replacement is an ordinary Java expression, a host variable can appear anywhere an expression is allowed, including as an argument to another method or as the receiver of a method call.

### Frequently used methods

All host variables are instances of [`AbstractCobolField`](https://opensourcecobol.github.io/opensourcecobol4j/javadoc/libcobj/index.html). The methods below are the ones most useful from an `EXEC JAVA` block.

| Method | Description |
|---|---|
| `getInt()` | Read the item as an `int`. |
| `getDouble()` | Read the item as a `double`. |
| `getDecimal()` | Read the item as a `CobolDecimal`. |
| `getString()` | Read the item as a `String` (for an alphanumeric item, including its trailing spaces). |
| `getBytes()` | Read the raw bytes of the item. |
| `setInt(int)` | Store an `int` into the item. |
| `moveFrom(String)` | Store a `String` into the item with COBOL `MOVE` semantics (padding and truncation). |
| `moveFrom(int)`, `moveFrom(double)`, `moveFrom(BigDecimal)` | Store a number into the item with COBOL `MOVE` semantics. |
| `moveFrom(AbstractCobolField)` | Store another COBOL item into the item. |
| `getSize()` | The size of the item in bytes. |

Some of these (`moveFrom(BigDecimal)`, `addInt(int)`, `subInt(int)`, …) declare `CobolStopRunException`. That is a checked exception the generated method already declares, so you may let it propagate; see [Exceptions](#exceptions).

### Where `:NAME` is *not* substituted

The compiler tracks Java string literals, character literals, line comments (`//`) and block comments (`/* ... */`). Inside them, `:NAME` is left exactly as written, and `END-EXEC` is not treated as the end of the block:

```cobol
           EXEC JAVA
              // :WRK-A is not replaced in this comment
              /* :WRK-A and even END-EXEC are just text here */
              String s = ":WRK-A stays literal";
              char c = ':';
              System.out.println(s + c);
              :WRK-A.setInt(7);
           END-EXEC.
```

This program prints `:WRK-A stays literal:` and sets `WRK-A` to 7.

### Undefined names are compile errors

If `:NAME` cannot be resolved to a data item of the program, `cobj` reports an error and the compilation fails. The same applies when the name resolves to something that is not a data item (for example a paragraph name):

```
prog.cbl:8: Error: 'NO-SUCH-VAR' undefined
prog.cbl:8: Error: EXEC JAVA: ':PARA-NAME' is not a data item
```

The reported line is the line the `EXEC JAVA` statement starts on, not the line inside the block where the reference appears. The same holds for an [`EXEC JAVA CLASS-MEMBER`](#declaring-class-members) block, whose host variables are resolved once the whole program has been parsed.

## Writing Java in a block

### Available classes

The generated Java source imports the libcobj packages, so classes such as `AbstractCobolField`, `CobolDataStorage`, `CobolRuntimeException` and `CobolStopRunException` can be used by their simple names. `java.lang` classes (`String`, `Integer`, `System`, …) are available as usual.

Any other class is either referred to by its fully qualified name, or imported with an [`EXEC JAVA IMPORT`](#importing-classes) block so that its simple name can be used:

```cobol
           EXEC JAVA
              java.util.List<String> names = new java.util.ArrayList<>();
              names.add(:WRK-STR.getString().trim());
              System.out.println(names);
           END-EXEC.
```

### Importing classes

An `EXEC JAVA` block sits inside a method body, so an `import` declaration cannot be written there. Instead, put the imports in an `EXEC JAVA IMPORT ~ END-EXEC` block:

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

- The declarations are emitted into the `import` section at the top of the generated `.java` file, after the libcobj imports.
- The block can be written in the DATA DIVISION (for example in the WORKING-STORAGE SECTION, where `EXEC SQL` declarations are also allowed) or in the PROCEDURE DIVISION at any statement position. Wherever it appears, the imports apply to the whole generated file of the program that contains it. Writing it near the top of the program is recommended for readability.
- The body may contain only `import` declarations -- `import a.b.C;`, `import static a.b.C.d;` and the wildcard forms `import a.b.*;` / `import static a.b.C.*;` -- plus Java comments. Anything else is a compile-time error, because a stray statement at the top of the generated file would produce confusing `javac` errors. A Java keyword cannot appear as a segment of the imported name, and the name itself must not be split by spaces (`import java . util . List;` is rejected). Only ASCII identifiers are accepted in the imported name. A stray `;` between declarations is ignored.
- Every faulty declaration in the block is reported, so a single compilation lists them all, and each error points at the line of the declaration rather than at the line the block starts on:

  ```
  prog.cbl:7: Error: EXEC JAVA IMPORT: not an import declaration: 'int x = 1'
  prog.cbl:8: Error: EXEC JAVA IMPORT: invalid import declaration: 'import int.foo'
  prog.cbl:9: Error: EXEC JAVA IMPORT: missing ';' in import declaration: 'import java.util.Map'
  ```

- Host variables (`:NAME`) cannot be used inside an `EXEC JAVA IMPORT` block.
- The same declaration written more than once (for example in repeated blocks) is emitted only once. Declarations are compared after being normalised, so differences in the spacing between tokens, in line breaks, and in comments do not defeat this. A plain import and a `static` import of the same name count as different declarations.
- In the DATA DIVISION, terminate the block with `END-EXEC.` (with the separator period) when a data description entry follows, just like `EXEC SQL`.

### Declaring class members

An `EXEC JAVA` block contains statements, so it cannot declare methods or fields. To add members to the generated class -- fields, methods, nested classes and so on -- use an `EXEC JAVA CLASS-MEMBER ~ END-EXEC` block:

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

- The body is emitted into the class body of the generated class, verbatim except for host variable substitution: `:NAME` is replaced with the host variable field exactly as in `EXEC JAVA`, and works inside instance methods because those fields are instance fields of the same class. A data item can be referenced even when the block appears before its declaration, as in the example above.
- The block can be written in the same places as `EXEC JAVA IMPORT` (in the DATA DIVISION, or at a statement position in the PROCEDURE DIVISION). At a statement position it produces no executable code -- the declarations only go to the class body.
- Members declared this way can be used from any `EXEC JAVA` block of the same program, and the other way round: member methods can call each other and read or write host variables.
- Reference a host variable from methods only, **never** from a field initializer or an instance initializer block. The member declarations are emitted ahead of the host variable fields in the class body, so `javac` rejects such a reference with `illegal forward reference`. And the host variable objects are created by `init()`, which the constructor body calls -- field and instance initializers run before the constructor body -- so a reference that dodges that error (by qualifying the field, as in `this.:WRK-N`) reads `null`.
- `static` members cannot reference host variables: the host variable fields are instance fields, and `javac` reports `non-static variable ... cannot be referenced from a static context`.
- Like `EXEC JAVA`, the body is not validated by `cobj`; errors in the member declarations are reported by `javac` against the generated `.java` file.

### Scope of local variables

Java local variables declared in a block are visible only within that same `EXEC JAVA` block. Two blocks in the same program may therefore declare variables with the same name:

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

To carry a value from one block to the next, store it in a COBOL data item.

### Control flow

Do **not** write `return`, `break` or `continue` at the top level of a block. The generated Java implements COBOL control flow (`PERFORM`, `GO TO`, paragraph dispatch) with Java control structures, and these keywords break it, producing either a wrong execution order or a Java compilation error.

Control structures that are complete within the block (an `if`, a `for` loop and a `break` that belongs to that loop, and so on) are fine.

### Exceptions

The block runs inside a generated method declared as `throws CobolRuntimeException, CobolStopRunException`. Consequently:

- `CobolRuntimeException`, `CobolStopRunException` and unchecked exceptions may be thrown or allowed to propagate.
- Any other checked exception must be caught inside the block. If Java code you call declares one (`IOException`, `SQLException`, …), wrap the call in `try`/`catch`:

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

## Restrictions and Notes

- **A colon followed by a letter or a digit starts a host variable reference.** When you need a colon for another purpose (the conditional operator or a labelled statement), put a space after it:

  ```java
  int m = (n > 0) ? 1 : 0;      // OK: space after ':'
  int m = (n > 0) ? 1 :0;       // NG: ':0' is read as a host variable reference
  ```

  Method references (`::`) are recognised and left untouched: both `String::valueOf` and `:WRK-STR::toString` work as expected.

- **A hyphen is part of a COBOL word.** `:N-1` is a reference to the data item `N-1`, not `:N` minus 1. Write subtraction with spaces:

  ```java
  int m = :N - 1;               // OK
  int m = :N-1;                 // reference to the data item N-1
  ```

  A hyphen at the end of a name is not included in the name, because a COBOL user-defined word cannot end with a hyphen.

- **Qualification is not available.** `X OF GRP` cannot be written, and a subscript such as `:TBL(1)` is not recognised either -- the parenthesis is passed through as Java text. The name must be resolvable uniquely within the program. For an item that needs qualification or a subscript, `MOVE` it to a work item first and refer to that item.

- **Fixed-format source stops at column 72.** Like `EXEC SQL`, the body of an `EXEC JAVA`, `EXEC JAVA IMPORT` or `EXEC JAVA CLASS-MEMBER` block is read by the ordinary COBOL source reader, so in fixed format only columns 8-72 are part of the program; anything beyond column 72 is discarded. A long Java line therefore loses its tail -- typically the trailing `;`, which then shows up as a puzzling Java syntax error, or, in an `EXEC JAVA IMPORT` block, as `EXEC JAVA IMPORT: missing ';' in import declaration`. Keep each Java line short enough, compile with `-Wcolumn-overflow` to be warned about overflowing lines, or use the free source format (`-free`).

- **A user import can shadow a libcobj class.** The generated file imports the libcobj packages with on-demand (`.*`) imports, and a single-type import always takes precedence over them. libcobj contains classes with common simple names (`Const`, `FileIO`, `IndexedFile`, `Linage`, ...), so importing an unrelated class with the same simple name can break references in the *generated* code. If that happens, drop the import and use the fully qualified name instead.

- **A block without `END-EXEC` is a compile-time error.** If the source ends while a block is still open (a forgotten `END-EXEC`, or an unclosed `/* ... */` comment swallowing it), `cobj` reports `... statement is not terminated by END-EXEC` and fails. The message names the form of the block that was left open -- `EXEC JAVA`, `EXEC JAVA IMPORT` or `EXEC JAVA CLASS-MEMBER`.

- **The Java code is not checked by `cobj`.** Errors in the Java you write (a syntax error, an unknown method, a type mismatch) are reported by the Java compiler against the generated `.java` file, so the reported line numbers refer to that file, not to the COBOL source.

- **Host variables of a called program are not visible.** Only data items of the program that contains the `EXEC JAVA` statement can be referenced.

## Compilation

No dedicated option is needed:

```bash
# Compile a COBOL program with EXEC JAVA
cobj program.cbl

# Run the compiled program
java program
```

### The generated code

If you want to see how a block was expanded, read the generated Java source (`[PROGRAM-ID].java`, produced in the current directory). Like every other COBOL statement, the block is emitted as a Java block `{ ... }` preceded by a comment giving the source file and the line of the `EXEC JAVA` statement, and `:NAME` is replaced by the field object of that data item:

```java
        /* ej.cbl:7: EXEC JAVA */
        {
          int n = f_WRK_NUM.getInt();
          f_WRK_NUM.setInt(n * 3);
        }
```

The enclosing `{ ... }` is what limits the scope of the Java local variables you declare.

The other two forms of the block are emitted elsewhere in the file. An `EXEC JAVA IMPORT` block adds its declarations to the `import` section at the top of the file, after the libcobj imports. An `EXEC JAVA CLASS-MEMBER` block is emitted in the class body, under a `/* EXEC JAVA CLASS-MEMBER declarations */` comment, together with the members of every other `CLASS-MEMBER` block of the program:

```java
  /* EXEC JAVA CLASS-MEMBER declarations */
  private int counter = 0;
  private int nextCounter() {
     return ++counter;
  }
```

If such a block was written at a statement position in the PROCEDURE DIVISION, only the `/* prog.cbl:8: EXEC JAVA CLASS-MEMBER */` comment is left at that position; no code runs there.

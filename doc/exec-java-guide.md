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
```

- `EXEC JAVA` can be written in the PROCEDURE DIVISION, anywhere a COBOL statement is allowed.
- `EXEC`, `JAVA`, and `END-EXEC` are recognised case-insensitively, like other COBOL words. The Java code between them keeps its original case.
- The block body is emitted as-is into the generated Java source, at the position where the statement appears. The compiler only removes the leading indentation that all lines of the block have in common and re-indents the block to match the surrounding generated code; it does not otherwise rewrite your Java.
- The body is a sequence of Java **statements**: it ends up inside a method body of the generated class. Declarations that are only valid at class or file level (`import`, method declarations, field declarations) cannot be written here. See [Writing Java in a block](#writing-java-in-a-block).

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

The reported line is the line of the `EXEC JAVA` statement, not the line inside the block where the reference appears.

## Writing Java in a block

### Available classes

The generated Java source imports the libcobj packages, so classes such as `AbstractCobolField`, `CobolDataStorage`, `CobolRuntimeException` and `CobolStopRunException` can be used by their simple names. `java.lang` classes (`String`, `Integer`, `System`, …) are available as usual.

An `EXEC JAVA` block sits inside a method body, so you **cannot** add `import` statements. Refer to any other class by its fully qualified name:

```cobol
           EXEC JAVA
              java.util.List<String> names = new java.util.ArrayList<>();
              names.add(:WRK-STR.getString().trim());
              System.out.println(names);
           END-EXEC.
```

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

- **Fixed-format source stops at column 72.** Like `EXEC SQL`, the body of an `EXEC JAVA` block is read by the ordinary COBOL source reader, so in fixed format only columns 8-72 are part of the program; anything beyond column 72 is discarded. A long Java line therefore loses its tail -- typically the trailing `;`, which then shows up as a puzzling Java syntax error. Keep each Java line short enough, compile with `-Wcolumn-overflow` to be warned about overflowing lines, or use the free source format (`-free`).

- **A block without `END-EXEC` is a compile-time error.** If the source ends while an `EXEC JAVA` block is still open (a forgotten `END-EXEC`, or an unclosed `/* ... */` comment swallowing it), `cobj` reports `EXEC JAVA statement is not terminated by END-EXEC` and fails.

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

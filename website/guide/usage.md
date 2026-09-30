# Usage

## Compile and run

```bash
cobj [COBOL source file]
java [PROGRAM-ID]
```

`cobj` translates the COBOL program into `[PROGRAM-ID].java` and compiles it into `[PROGRAM-ID].class`
in the current directory.

For example, save the following program as `HELLO.cbl`:

```cobol
       IDENTIFICATION              DIVISION.
       PROGRAM-ID.                 HELLO.
       PROCEDURE                   DIVISION.
       MAIN-RTN.
           DISPLAY "HELLO WORLD!".
           STOP RUN.
```

Then compile and run it:

```bash
cobj HELLO.cbl
java HELLO
# HELLO WORLD!
```

::: tip Compile multiple files at once
When you compile several source files, pass them all to one `cobj` command. This is much faster than running `cobj` once per file.

```bash
cobj *.cbl
```
:::

## Learn more

- [API reference (Javadoc of libcobj.jar)](../javadoc/libcobj/index.html){target="_self"}
- [Embedded SQL (EXEC SQL) guide](https://github.com/yutaro-sakamoto/Hagane-COBOL/blob/main/doc/esql-guide.md)
- [Environment variables](https://github.com/yutaro-sakamoto/Hagane-COBOL/blob/main/doc/environment_variables.md)
- [Calling generated programs from multi-threaded Java applications](https://github.com/yutaro-sakamoto/Hagane-COBOL/blob/main/doc/multithreading.md)
- [Creating a Spring Boot application with cobj-api](https://github.com/yutaro-sakamoto/Hagane-COBOL/blob/main/doc/cobj-api_SpringBoot.md)
- [CHANGELOG](https://github.com/yutaro-sakamoto/Hagane-COBOL/blob/main/CHANGELOG.md)

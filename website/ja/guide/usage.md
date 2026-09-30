# 使い方

## コンパイルと実行

```bash
cobj [COBOLソースファイル]
java [PROGRAM-ID]
```

`cobj`はCOBOLプログラムを`[PROGRAM-ID].java`に変換し、カレントディレクトリに`[PROGRAM-ID].class`を作ります。

たとえば、次のプログラムを`HELLO.cbl`として保存します。

```cobol
       IDENTIFICATION              DIVISION.
       PROGRAM-ID.                 HELLO.
       PROCEDURE                   DIVISION.
       MAIN-RTN.
           DISPLAY "HELLO WORLD!".
           STOP RUN.
```

コンパイルして実行します。

```bash
cobj HELLO.cbl
java HELLO
# HELLO WORLD!
```

::: tip 複数のファイルはまとめてコンパイルする
複数のソースファイルをコンパイルするときは、1回の`cobj`コマンドにまとめて渡してください。ファイルごとに`cobj`を実行するよりずっと速くなります。

```bash
cobj *.cbl
```
:::

## さらに詳しく

- [APIリファレンス (libcobj.jarのJavadoc)](../../javadoc/libcobj/index.html){target="_self"}
- [埋め込みSQL (EXEC SQL)ガイド](https://github.com/yutaro-sakamoto/Hagane-COBOL/blob/main/doc/esql-guide_JP.md)
- [環境変数](https://github.com/yutaro-sakamoto/Hagane-COBOL/blob/main/doc/environment_variables_JP.md)
- [マルチスレッドのJavaアプリケーションから生成プログラムを呼び出す](https://github.com/yutaro-sakamoto/Hagane-COBOL/blob/main/doc/multithreading_JP.md)
- [cobj-apiを使ったSpring Bootアプリケーションの作成](https://github.com/yutaro-sakamoto/Hagane-COBOL/blob/main/doc/cobj-api_SpringBoot_JP.md)
- [CHANGELOG](https://github.com/yutaro-sakamoto/Hagane-COBOL/blob/main/CHANGELOG.md)

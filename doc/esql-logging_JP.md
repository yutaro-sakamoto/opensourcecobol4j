# ESQL ランタイムログ機能

## 概要

opensource COBOL 4JのESQLランタイム（`CobolSql` および内部実装クラスの `SqlConnection`）はSLF4Jを使用してログを出力します。SQL操作の実行状況、エラー情報をログレベルに応じて出力します。

なお、`CobolSql` は cobjが生成するJavaコードから呼び出される公開APIですが、`SqlConnection`、`SqlCA`、`SqlCursor`、`SqlState`、`CobolDataConverter` 等の他のクラスはすべてpackage-privateの内部実装です。SLF4Jのロガー名（完全修飾クラス名）としてはログ設定で利用できますが、外部から直接呼び出すことは想定されていません。

## ログレベル

| レベル | 出力内容 |
|--------|---------|
| ERROR | SQL実行失敗時のエラー情報（SQL文、エラーメッセージ） |
| DEBUG | SQL文の実行開始、接続成功/切断、カーソル操作 |
| TRACE | CONNECT のホスト変数値（ユーザー名・DB名）、FETCH操作（高頻度のため別レベル） |

## ログ出力箇所

### CobolSql

| メソッド | TRACEログ | DEBUGログ | ERRORログ |
|---------|-----------|-----------|-----------|
| `connect()` | ホスト変数値（ユーザー名、DB名） | 接続成功（接続id） | 接続失敗 |
| `disconnect()` | - | 切断開始（接続id） | 切断失敗 |
| `exec()` | - | SQL文（トリム済み） | SQL実行失敗 |
| `execWithParams()` | - | SQL文、パラメータ数 | SQL実行失敗 |
| `selectInto()` | - | SQL文 | SELECT INTO 実行失敗 |
| `declareCursor()` | - | カーソル名、SQL文 | - |
| `openCursor()` | - | カーソル名 | カーソルオープン失敗 |
| `fetchCursor()` | カーソル名 | - | - |

> [!NOTE]
> `execWhereCurrentOf()` / `execWithParamsWhereCurrentOf()` は専用のログ出力を持ちませんが、
> 内部でカーソル位置を補正したのち `exec()` / `execWithParams()` を呼び出すため、上表の
> `exec()` / `execWithParams()` のログが間接的に出力されます。

### SqlConnection

| メソッド | DEBUGログ |
|---------|-----------|
| `connect()` | JDBC URL とユーザー名、接続成功（接続id） |

## 設定方法

### デフォルト（slf4j-simple）

`libcobj.jar`にはslf4j-simpleが同梱されています。デフォルトではINFOレベル以上のログが標準エラー出力に出力されます。

DEBUGログを有効にするには、システムプロパティを設定します:

```bash
java -Dorg.slf4j.simpleLogger.defaultLogLevel=debug -cp libcobj.jar:. YourProgram
```

特定クラスのみ有効にする場合:

```bash
java -Dorg.slf4j.simpleLogger.log.jp.osscons.opensourcecobol.libcobj.sql.CobolSql=debug -cp libcobj.jar:. YourProgram
```

### Logback使用時

`slf4j-simple`を除外し、`logback-classic`をクラスパスに追加して`logback.xml`で設定します:

```xml
<configuration>
  <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
    <encoder>
      <pattern>%d{HH:mm:ss.SSS} [%thread] %-5level %logger{36} - %msg%n</pattern>
    </encoder>
  </appender>

  <logger name="jp.osscons.opensourcecobol.libcobj.sql" level="DEBUG"/>

  <root level="INFO">
    <appender-ref ref="STDOUT"/>
  </root>
</configuration>
```

## テスト

### ユニットテスト

ログ出力のユニットテストには`com.github.valfirst:slf4j-test`を使用しています。`CobolSqlLoggingTest`で各操作のログ出力レベル・メッセージ内容を検証しています。

### 統合テスト

実行時ログ（DEBUG/ERROR）の検証は、専用テスト `tests/esql-misc.src/logging.at` に一元化しています。このテストは1つのCOBOLプログラムでログ行を出力する各文（CONNECT、通常の `EXEC SQL`、パラメータ付き `EXEC SQL`、SELECT INTO、DECLARE/OPEN CURSOR、ERRORレベルで記録される失敗文、DISCONNECT）を実行し、標準エラー出力に出力される実行時ログを期待値と厳密に照合します。プログラム実行には `tests/atlocal.in` で定義された `RUN_MODULE_LOG` を使用します。`RUN_MODULE_LOG` はslf4j-simpleで `CobolSql` ロガーのDEBUGレベルを有効化し、ログ行を期待値と比較しやすくするため日時・スレッド名・ロガー名・レベルの角括弧表示を抑制します。

`tests/esql-*` のその他の統合テストは、機能の動作そのものの検証に専念し、実行時ログ（標準エラー出力）は `[ignore]` として検証対象から除外しています（プログラム実行には通常の `RUN_MODULE` を使用）。これにより、ログのフォーマットや文言を変更した場合でも、更新が必要なのは `logging.at` の1ファイルだけで済みます。

## 関連ドキュメント

- [esql-guide_JP.md](./esql-guide_JP.md) -- ユーザ向け使い方
- [esql-design_JP.md](./esql-design_JP.md) -- 内部アーキテクチャ
- [esql-logging.md](./esql-logging.md) -- 本ドキュメントの英語版

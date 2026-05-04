# ESQL ランタイムログ機能

## 概要

opensource COBOL 4JのESQLランタイム（`CobolSql` および内部実装クラスの `SqlConnection`）はSLF4Jを使用してログを出力します。SQL操作の実行状況、エラー情報をログレベルに応じて出力します。

なお、`CobolSql` は cobjが生成するJavaコードから呼び出される公開APIですが、`SqlConnection`、`SqlCA`、`SqlCursor`、`SqlState`、`CobolDataConverter` 等の他のクラスはすべてpackage-privateの内部実装です。SLF4Jのロガー名（完全修飾クラス名）としてはログ設定で利用できますが、外部から直接呼び出すことは想定されていません。

## ログレベル

| レベル | 出力内容 |
|--------|---------|
| ERROR | SQL実行失敗時のエラー情報（SQL文、エラーメッセージ） |
| DEBUG | SQL文の実行開始、接続/切断、カーソル操作 |
| TRACE | FETCH操作（高頻度のため別レベル） |

## ログ出力箇所

### CobolSql

| メソッド | DEBUGログ | ERRORログ |
|---------|-----------|-----------|
| `connect()` | 接続開始（ユーザー名、DB名）、接続成功 | 接続失敗 |
| `disconnect()` | 切断��始 | 切断失敗 |
| `exec()` | SQL文（トリム済み） | SQL実行失敗 |
| `execWithParams()` | SQL文、パラメータ数 | SQL実行失敗 |
| `selectInto()` | SQL文 | SQL実行失敗 |
| `declareCursor()` | カーソル名、SQL文 | - |
| `openCursor()` | カーソル名 | カーソルオープン失敗 |
| `fetchCursor()` | カーソル名（TRACEレベル） | - |

### SqlConnection

| メソッド | DEBUGログ |
|---------|-----------|
| `connect()` | JDBC URL、ユーザー名、接続成功 |

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

ログ出力のテストには`com.github.valfirst:slf4j-test`を使用しています。`CobolSqlLoggingTest`で各操作のログ出力レベル・メッセージ内容を検証しています。

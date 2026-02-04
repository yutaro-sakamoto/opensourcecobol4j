# termio パッケージ Javadocレビュー資料

## 概要

`jp.osscons.opensourcecobol.libcobj.termio` パッケージは、COBOLのDISPLAY文およびACCEPT文の実行時処理を提供する。

## CobolTerminal クラス

COBOLのターミナル入出力処理（DISPLAY文、ACCEPT文）を実装するクラス。

### COBOL文との対応

| メソッド名 | 対応するCOBOL文 |
|-----------|----------------|
| `display(boolean, boolean, AbstractCobolField...)` | `DISPLAY ... [UPON {STANDARD-OUTPUT\|STANDARD-ERROR}]` |
| `display(int, int, int, AbstractCobolField...)` | 同上（整数パラメータ版） |
| `accept(AbstractCobolField)` | `ACCEPT identifier` |
| `acceptDate(AbstractCobolField)` | `ACCEPT identifier FROM DATE` |
| `acceptDate_yyyymmdd(AbstractCobolField)` | `ACCEPT identifier FROM DATE YYYYMMDD` |
| `acceptDay(AbstractCobolField)` | `ACCEPT identifier FROM DAY` |
| `acceptDay_yyyyddd(AbstractCobolField)` | `ACCEPT identifier FROM DAY YYYYDDD` |
| `acceptDayOfWeek(AbstractCobolField)` | `ACCEPT identifier FROM DAY-OF-WEEK` |
| `acceptTime(AbstractCobolField)` | `ACCEPT identifier FROM TIME` |
| `displayEnvironment(AbstractCobolField)` | `DISPLAY identifier UPON ENVIRONMENT-NAME` |
| `displayEnvValue(AbstractCobolField)` | `DISPLAY identifier UPON ENVIRONMENT-VALUE` |
| `acceptEnvironment(AbstractCobolField)` | `ACCEPT identifier FROM ENVIRONMENT-VALUE` |
| `displayCommandLine(AbstractCobolField)` | `DISPLAY identifier UPON COMMAND-LINE` |
| `acceptCommandLine(AbstractCobolField)` | `ACCEPT identifier FROM COMMAND-LINE` |
| `displayArgNumber(AbstractCobolField)` | `DISPLAY identifier UPON ARGUMENT-NUMBER` |
| `acceptArgNumber(AbstractCobolField)` | `ACCEPT identifier FROM ARGUMENT-NUMBER` |
| `acceptArgValue(AbstractCobolField)` | `ACCEPT identifier FROM ARGUMENT-VALUE` |

### フィールドの説明

- `commlncnt`: コマンドライン文字列の長さを保持する。`displayCommandLine`で設定される。
- `commlnptr`: コマンドライン文字列のバイト配列を保持する。`displayCommandLine`で設定され、`acceptCommandLine`で参照される。
- `scan`: 標準入力からのデータ読み取りに使用するScannerインスタンス。遅延初期化される。

### 使用例（生成されたJavaコード）

```java
// DISPLAY "Hello" に相当
CobolTerminal.display(0, 1, 1, c_1_Hello);

// ACCEPT WS-DATE FROM DATE に相当
CobolTerminal.acceptDate(f_WS_DATE);

// DISPLAY WS-ENV-NAME UPON ENVIRONMENT-NAME に相当
CobolTerminal.displayEnvironment(f_WS_ENV_NAME);

// ACCEPT WS-ENV-VALUE FROM ENVIRONMENT-VALUE に相当
CobolTerminal.acceptEnvironment(f_WS_ENV_VALUE);
```

### 注意点

- 文字エンコーディングは `CobolUtil.terminalEncoding` に基づいて処理される（UTF-8またはShift_JIS）
- 環境変数操作の際、環境変数名は `CobolUtil.cobLocalEnv` に一時保存される
- 日時関連のACCEPT文では、`CobolUtil.cobLocalTm` が設定されている場合はその値が使用され、設定されていない場合は現在時刻が使用される
- `acceptTime()` は常に現在時刻を使用し、`cobLocalTm` の設定は考慮しない

## Javadoc更新履歴

### 更新日: 2026-02-04

**対象メソッド:**
- クラスレベルのJavadoc
- フィールド: `commlncnt`, `commlnptr`
- メソッド: `accept`, `jobOrCurrentLocalTime`, `acceptDate`, `acceptDate_yyyymmdd`, `acceptDay`, `acceptDay_yyyyddd`, `acceptDayOfWeek`, `acceptTime`, `displayEnvironment`, `displayEnvValue`, `acceptEnvironment`, `displayCommandLine`, `acceptCommandLine`, `displayArgNumber`, `acceptArgNumber`, `acceptArgValue`

**主な変更点:**
1. 「準備中」「TODO: 準備中」を意味のある説明に置換
2. COBOLの対応する文（例: `ACCEPT identifier FROM DATE`）を明記
3. パラメータの説明にCOBOLのPICTURE句の例を追加（例: PIC 9(6)）
4. `{@link}` タグを使用した関連メソッド・クラスへの参照を追加
5. フォーマットの一貫性を改善（`<p>` タグ後の改行）

**レビュー結果:**
- code-reviewer-jp エージェントによるレビュー実施
- 総合評価: ◎ 優秀
- 軽微なフォーマット改善を適用済み

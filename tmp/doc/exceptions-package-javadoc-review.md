# exceptionsパッケージ Javadocレビューガイド

## 概要

`jp.osscons.opensourcecobol.libcobj.exceptions`パッケージは、COBOL実行時の例外処理を担当するクラスを提供する。
このパッケージのクラスは、COBOLプログラムからJavaに変換されたコードで使用される。

## パッケージ構成

| クラス名 | 役割 |
|---------|------|
| CobolExceptionId | COBOL 2002標準のEC例外コードIDを定数として定義 |
| CobolExceptionTabCode | 例外IDから16ビット例外コードへのマッピングテーブル |
| CobolExceptionInfo | 例外情報を保持する静的クラス |
| CobolRuntimeException | COBOL実行時例外（RuntimeException継承） |
| CobolStopRunException | STOP RUN例外（Exception継承） |

## 例外処理の仕組み

### 1. STOP RUN処理

COBOLの`STOP RUN`文は、`CobolStopRunException`のスローに変換される。

```java
// 変換されたJavaコードでの使用例（STOP RUN文）
CobolStopRunException.throwException(b_RETURN_CODE.intValue());
```

mainメソッドまたはrun_moduleメソッドでキャッチされ、終了処理が実行される：

```java
try {
    execEntry(l_PROGRAM_NAME);
} catch(CobolStopRunException e) {
    CobolStopRunException.stopRun();  // ファイルクローズ等
    System.exit(e.getReturnCode());
}
```

### 2. ファイルI/O等のエラー処理

ファイル操作等では、`CobolRuntimeException.code`を使用してエラーを検出する：

```java
// 操作前に初期化
CobolRuntimeException.code = 0;
{
    h_SAMPLE_FILE.open(2, 1, null);
}
// 操作後にエラーチェック
if (CobolRuntimeException.code != 0) {
    // エラーハンドラを実行
    execProcedureDivision(l_Default_Error_Handler, l_Default_Error_Handler);
}
```

## EC例外コード体系

COBOL 2002標準で定義されているEC（Exception Condition）例外コードは16ビットで表現される：
- 上位バイト: 例外カテゴリ
- 下位バイト: 詳細な例外種別

### 主要な例外カテゴリ

| カテゴリ | コード範囲 | 説明 |
|---------|-----------|------|
| EC-ARGUMENT | 0x01xx | 引数関連エラー |
| EC-BOUND | 0x02xx | 境界チェックエラー |
| EC-DATA | 0x03xx | データエラー |
| EC-FLOW | 0x04xx | 制御フローエラー |
| EC-I-O | 0x05xx | 入出力エラー |
| EC-IMP | 0x06xx | 実装依存エラー |
| EC-OVERFLOW | 0x0Axx | オーバーフローエラー |
| EC-PROGRAM | 0x0Bxx | プログラム関連エラー |
| EC-SIZE | 0x10xx | サイズエラー |

### 実際に使用される例外コード

以下の例外コードは、libcobjまたは変換されたJavaコードで実際に使用される可能性がある：

- **COB_EC_ARGUMENT_FUNCTION (3)**: 組み込み関数の引数エラー
- **COB_EC_BOUND_REF_MOD (10)**: 部分参照の境界エラー
- **COB_EC_BOUND_SUBSCRIPT (12)**: 添字の境界エラー
- **COB_EC_I_O_* (32-42)**: 各種ファイルI/Oエラー
- **COB_EC_OVERFLOW_STRING (66)**: STRING文のオーバーフロー
- **COB_EC_OVERFLOW_UNSTRING (67)**: UNSTRING文のオーバーフロー
- **COB_EC_PROGRAM_NOT_FOUND (73)**: CALLでプログラムが見つからない
- **COB_EC_SIZE_OVERFLOW (111)**: 数値サイズオーバーフロー

## Javadoc記述のポイント

1. **例外コードID vs 例外コード値の区別**
   - CobolExceptionIdの定数: 例外コードID（配列インデックス）
   - CobolExceptionTabCode.code[id]: 例外コード値（16ビット値）

2. **Exception vs RuntimeException**
   - CobolStopRunException: Exception継承（正常終了を示す）
   - CobolRuntimeException: RuntimeException継承（実行時エラー）

3. **静的変数codeの使い方**
   - 操作前に0に初期化
   - 操作後に0以外ならエラー発生
   - エラーハンドラでエラー処理

## 関連クラス

- `CobolResolve`: cobExceptionマップで例外コードと例外名の対応を管理
- `CobolFile`: ファイルI/O時に例外コードを設定
- `CobolUtil`: エラー発生時のプログラム情報を取得

## 変更履歴

- 2026-02-04: 初版作成。全ての「TODO: 準備中」を適切な説明に置き換え
- 2026-02-04: コードレビュー後の修正
  - CobolRuntimeException.java: 誤字修正（CobolExceptioTabCode → CobolExceptionTabCode）
  - CobolRuntimeException.java: getException()メソッドの説明を実装に合わせて修正
  - CobolExceptionId.java: 未使用コードの説明を追加、表記を「Exception Condition（EC）」に統一

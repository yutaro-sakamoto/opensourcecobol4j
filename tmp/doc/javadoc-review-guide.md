# libcobj Javadocレビューガイド

このドキュメントは、libcobjのJavadocコメントをレビューする際に参考となる情報をまとめたものです。

## 1. libcobjの概要

libcobj.jarはopensource COBOL 4Jのランタイムライブラリであり、COBOLからJavaへのトランスパイル時に生成されるJavaコードが依存するライブラリです。

### 主要なパッケージ構成

| パッケージ | 説明 |
|-----------|------|
| `jp.osscons.opensourcecobol.libcobj.data` | COBOLの変数（フィールド）を表すクラス群 |
| `jp.osscons.opensourcecobol.libcobj.common` | 共通ユーティリティ、モジュール管理、制御構造 |
| `jp.osscons.opensourcecobol.libcobj.call` | CALL文に関連するクラス |
| `jp.osscons.opensourcecobol.libcobj.file` | ファイル操作に関連するクラス |
| `jp.osscons.opensourcecobol.libcobj.exceptions` | 例外クラス |
| `jp.osscons.opensourcecobol.libcobj.termio` | DISPLAY/ACCEPT文に関連するクラス |

## 2. 主要クラスの役割

### データ関連クラス (data パッケージ)

#### AbstractCobolField
- **役割**: 全てのCOBOL変数の抽象基底クラス
- **重要メンバ**:
  - `size`: データサイズ（バイト数）
  - `dataStorage`: データを格納するストレージ
  - `attribute`: 変数の属性情報（型、桁数、符号等）
- **主要メソッド**: `moveFrom()`, `getDecimal()`, `compareTo()`, `getSign()`, `putSign()`

#### CobolDataStorage
- **役割**: COBOL変数のデータを保持するバイト配列のラッパー
- **重要な概念**: 集団項目では複数のCobolDataStorageが同一のバイト配列を共有し、indexで開始位置を区別する
- **主要メソッド**: `getByte()`, `setByte()`, `memcpy()`, `memset()`, `memcmp()`

#### CobolFieldAttribute
- **役割**: COBOL変数の属性情報を保持
- **重要メンバ**:
  - `type`: 変数の型（COB_TYPE_NUMERIC_DISPLAY等）
  - `digits`: 数値の桁数
  - `scale`: 小数点位置
  - `flag`: 各種フラグ（符号、BLANK WHEN ZERO等）
  - `pic`: PICTURE句の文字列

#### CobolDecimal
- **役割**: 数値計算用のBigDecimalラッパー
- **用途**: ADD、SUBTRACT、MULTIPLY、DIVIDE等の算術演算

### 派生フィールドクラス

| クラス | COBOL型 | 説明 |
|--------|---------|------|
| CobolNumericField | PIC 9(n) | 数値表示型 |
| CobolNumericBinaryField | COMP, COMP-5 | バイナリ数値型 |
| CobolNumericPackedField | COMP-3 | パック10進数 |
| CobolAlphanumericField | PIC X(n) | 英数字型 |
| CobolNationalField | PIC N(n) | 日本語型 |
| CobolGroupField | 集団項目 | 集団項目 |

### 共通クラス (common パッケージ)

#### CobolModule
- **役割**: 実行中のCOBOLモジュール情報を管理
- **重要機能**: モジュールスタック管理、照合順序、decimal point設定

#### CobolControl
- **役割**: SECTION/PARAGRAPHの制御構造を実装
- **用途**: PERFORM文、GO TO文の実現

#### CobolConstant
- **役割**: COBOL定数（SPACE, ZERO, HIGH-VALUE等）の定義

#### CobolUtil
- **役割**: 各種ユーティリティ関数
- **主要機能**: 初期化、文字列変換、実行時エラー処理

## 3. COBOL特有の概念

### 符号の扱い
- **SIGN LEADING**: 符号がデータの先頭にある
- **SIGN TRAILING**: 符号がデータの末尾にある（デフォルト）
- **SIGN SEPARATE**: 符号が独立した1バイト（'+' or '-'）
- **EBCDIC形式**: 符号と数字が1バイトに埋め込まれる

### 数値型
- **DISPLAY型**: 各桁が1バイト（'0'-'9'）
- **BINARY型**: ネイティブバイナリ形式
- **PACKED型**: 2桁で1バイト + 符号ニブル（COMP-3）

### MOVE文の変換規則
- 異なる型間のMOVEは適切な変換が行われる
- 英数字から日本語項目への変換は半角→全角変換を含む

## 4. Javadoc記述のガイドライン

### 良いJavadocの例

```java
/**
 * thisの保持する数値データに指定された値を加算する
 *
 * @param n thisの保持する数値データに加算する整数値
 * @return 演算結果のステータス。正常終了時は0。
 * @throws CobolStopRunException オーバーフロー等で実行を停止する場合にスローされる
 */
public int addInt(int n) throws CobolStopRunException
```

### 注意点
1. COBOL用語は必要に応じて説明を追加する
2. 「準備中」「TODO: 準備中」は全て意味のある説明に置き換える
3. @param, @return, @throwsタグは具体的に記述する
4. 日本語として自然な表現を使用する

## 5. 更新状況

### 更新済みファイル
- AbstractCobolField.java - 主要なJavadocを更新済み

### 要更新ファイル
以下のファイルには「準備中」が残っています：

1. CobolDataStorage.java - バイナリ操作メソッド多数
2. CobolModule.java
3. CobolConstant.java
4. CobolControl.java
5. CobolUtil.java
6. その他多数（全67ファイル、2688箇所）

## 6. 参考資料

- doc/converted_Java_file_JP.md - Java変換の詳細な解説
- 生成されたJavaコード例: tmp/HELLO.java, tmp/FILETEST.java

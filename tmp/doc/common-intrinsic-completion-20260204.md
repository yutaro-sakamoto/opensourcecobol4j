# CobolIntrinsic.java Javadoc修正完了報告

作業日: 2026-02-04

## 概要

`jp.osscons.opensourcecobol.libcobj.common.CobolIntrinsic.java`の残りの「準備中」Javadocを全て修正し、commonパッケージ全体のJavadoc修正が完了した。

## 修正したメソッド

以下の18メソッドのJavadocを「準備中」から意味のある内容に修正した：

### 例外関連関数
| メソッド名 | COBOL関数 | 説明 |
|-----------|-----------|------|
| funcExceptionFile() | EXCEPTION-FILE | ファイルステータスとファイル名を返す |
| funcExceptionLocation() | EXCEPTION-LOCATION | 例外発生位置（プログラムID、セクション、パラグラフ、行番号）を返す |
| funcExceptionStatement() | EXCEPTION-STATEMENT | 例外発生文の種類を返す |
| funcExceptionStatus() | EXCEPTION-STATUS | 例外名を返す |

### 数値関連関数
| メソッド名 | COBOL関数 | 説明 |
|-----------|-----------|------|
| funcFractionPart() | FRACTION-PART | 小数部を返す |
| funcSign() | SIGN | 符号（-1, 0, 1）を返す |

### 時刻関連関数
| メソッド名 | COBOL関数 | 説明 |
|-----------|-----------|------|
| funcSecondsFromFormattedTime() | SECONDS-FROM-FORMATTED-TIME | フォーマット指定の時刻文字列を秒数に変換 |
| funcSecondsPastMidnight() | SECONDS-PAST-MIDNIGHT | 午前0時からの経過秒数を返す |
| funcLocaleDate() (2オーバーロード) | LOCALE-DATE | 日付をロケールに基づいた形式で返す |
| funcLocaleTime() (2オーバーロード) | LOCALE-TIME | 時刻をロケールに基づいた形式で返す |
| funcLocaleTimeFromSeconds() (2オーバーロード) | LOCALE-TIME-FROM-SECONDS | 秒数を時刻文字列に変換 |

### 文字列関連関数
| メソッド名 | COBOL関数 | 説明 |
|-----------|-----------|------|
| funcStoredCharLength() | STORED-CHAR-LENGTH | 後続空白を除いた文字数を返す |
| funcSubstitute() | SUBSTITUTE | 部分文字列の置換（大文字小文字区別） |
| funcSubstituteCase() | SUBSTITUTE-CASE | 部分文字列の置換（大文字小文字無視） |
| funcTrim() | TRIM | 前後の空白を除去 |

## Javadocの記述方針

各メソッドのJavadocには以下を含めた：

1. **対応するCOBOL関数名**: 「COBOLのFUNCTION XXXを実装する。」
2. **機能の説明**: `<p>`タグ内に詳細説明
3. **具体例**: 必要に応じて入出力の例を記載
4. **パラメータ説明**: 各`@param`に意味と有効値を記載
5. **戻り値説明**: `@return`に戻り値の型と内容を記載

## コードレビュー結果

code-reviewer-jpエージェントによるレビューを実施。主な評価：

- **良い点**: COBOL文との対応が明確、具体例の提供、例外条件の説明
- **改善提案**: @seeタグによる関連関数の相互参照（将来の改善として記録）

## 検証

修正後、commonパッケージ内の「準備中」検索結果が0件であることを確認済み。

```bash
grep -r "準備中" libcobj/app/src/main/java/jp/osscons/opensourcecobol/libcobj/common/
# No matches found
```

## commonパッケージの修正状況（最終）

全クラスの修正が完了：

- [x] CobolModule
- [x] CobolEncoding
- [x] CobolConstant
- [x] CobolCallParams
- [x] CobolCheck
- [x] CobolExternal
- [x] CobolUtil
- [x] GetAbstractCobolField
- [x] GetInt
- [x] CobolInspect
- [x] CobolString
- [x] CobolControl
- [x] CobolIntrinsic **← 今回完了**

## 参照

- javadoc-common-package.md - パッケージ全体のレビューガイド
- libcob/intrinsic.c - 元のC言語実装

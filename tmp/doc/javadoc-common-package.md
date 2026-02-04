# jp.osscons.opensourcecobol.libcobj.common パッケージ Javadocレビューガイド

## パッケージ概要

`jp.osscons.opensourcecobol.libcobj.common`パッケージは、COBOLプログラムの実行時に必要な共通機能を提供するクラス群を含む。COBOLからJavaへトランスパイルされたコードから直接呼び出される。

## 生成されたJavaコードでの使用例

以下は`TEST__COMMON.java`から抽出した使用例：

### CobolModule
```java
// モジュールの初期化と管理
this.module = new CobolModule(null, null, null, null, 0, '.', '$', ',', 1, 1, 1, 0, null);
CobolModule.push(module);
module.setProgramId("TEST__COMMON");
// ...
CobolModule.pop();
```

### CobolUtil
```java
// プログラム初期化
CobolUtil.cob_init(args, false);
// 文字列のバイト変換
public static final byte[] str_0_HELLO = CobolUtil.stringToBytes("HELLO WORLD");
```

### CobolString (STRING文)
```java
CobolString.stringInit(f_WS_STRING, f_WS_POINTER);
CobolString.stringDelimited(CobolConstant.space);
CobolString.stringAppend(CobolFieldFactory.makeCobolField(...));
CobolString.stringFinish();
```

### CobolInspect (INSPECT文)
```java
CobolInspect.init(f_WS_NAME, 0);
CobolInspect.start();
CobolInspect.all(f_WS_COUNT, c_5_L);
CobolInspect.finish();
```

### CobolConstant
```java
CobolString.stringDelimited(CobolConstant.space);
```

## 各クラスの役割

| クラス名 | 役割 | 対応するCOBOL機能 |
|----------|------|-------------------|
| CobolConstant | 定数定義（ZERO, SPACE, HIGH-VALUE等）| 表意定数 |
| CobolCallParams | CALL文のパラメータ数管理 | CALL文 |
| CobolCheck | 添字・範囲チェック | 配列アクセス、参照修飾 |
| CobolExternal | EXTERNAL項目管理 | EXTERNAL句 |
| CobolUtil | ユーティリティ関数群 | 環境変数、初期化等 |
| GetAbstractCobolField | フィールド取得用インターフェース | 遅延評価 |
| CobolEncoding | 文字エンコーディング定義 | - |
| GetInt | 整数取得用インターフェース | 遅延評価 |
| CobolInspect | INSPECT文の実装 | INSPECT文 |
| CobolString | STRING/UNSTRING文の実装 | STRING/UNSTRING文 |
| CobolControl | 制御フロー管理 | PERFORM, GO TO |
| CobolModule | モジュール（プログラム）管理 | CALL, GO BACK |
| CobolIntrinsic | 組み込み関数 | FUNCTION |

## 修正状況

- [x] CobolModule - 修正済み（事前に完了）
- [x] CobolEncoding - 修正済み（事前に完了）
- [x] CobolConstant - 修正完了
- [x] CobolCallParams - 修正完了
- [x] CobolCheck - 修正完了
- [x] CobolExternal - 修正完了
- [x] CobolUtil - 修正完了
- [x] GetAbstractCobolField - 修正完了
- [x] GetInt - 修正完了
- [x] CobolInspect - 修正完了
- [x] CobolString - 修正完了
- [x] CobolControl - 修正完了
- [x] CobolIntrinsic - 修正完了（2026-02-04に残りのメソッドも完了）

## 修正内容の概要

### CobolConstant.java
- クラスレベルのJavadocを追加（COBOLの表意定数との対応を説明）
- 全角文字（Shift-JIS）定数の説明を追加
- バッファサイズ定数、10のべき乗テーブル等の説明を追加

### CobolCallParams.java
- CALL文のパラメータ数管理の目的を説明

### CobolCheck.java
- checkSubscript, checkOdoメソッドの役割と引数を詳細に説明
- COBOLの配列（テーブル）やOCCURS DEPENDING ON句との対応を明記

### CobolExternal.java
- EXTERNAL句の目的と使用方法を説明
- 複数プログラム間でのデータ共有の仕組みを明記

### CobolUtil.java
- 全メソッドのJavadocを日本語で統一
- 環境変数との関連、libcob関数との対応を明記
- TDOD -> TODO のtypoを修正
- setEnvメソッドの英語コメントを日本語に統一

### GetAbstractCobolField.java, GetInt.java
- 関数型インターフェースとしての目的を説明
- 遅延評価の使用場面を具体的に記述

### CobolInspect.java
- INSPECT文の各オプション（ALL, LEADING, FIRST, TRAILING, CHARACTERS, CONVERTING）を説明
- 使用手順を明確に記載

### CobolString.java
- STRING/UNSTRING文の各オプションを詳細に説明
- COBOL文の使用例を追加

### CobolControl.java
- 継続渡しスタイル（CPS）による制御フロー実装を説明
- PERFORM文、GO TO文との対応を明記

### CobolIntrinsic.java
- クラスレベルのJavadocを追加
- 主要な組み込み関数（LENGTH, INTEGER, UPPER-CASE, LOWER-CASE, REVERSE等）の説明を追加
- 残りのメソッドは今後の作業として残存

## コードレビュー結果

2024年の修正後にコードレビューを実施。以下の問題を修正済み：

1. CobolUtil.java: setEnv()メソッドのJavadocが英語のみ＆説明誤り → 日本語で正しく修正
2. CobolInspect.java: start()メソッドの説明不足 → 詳細説明を追加
3. CobolConstant.java: 16進数表記の大文字/小文字混在 → 小文字に統一

## レビュー時の注意点

1. COBOLの文法・仕様との対応を確認
2. パラメータの説明が具体的かどうか
3. 例外がスローされる条件が明確か
4. libcob（C言語版opensource COBOL）との対応関係が記載されているか

## 今後の課題

1. ~~CobolIntrinsic.javaの残りのメソッド（日付・時刻関数、数学関数等）のJavadoc修正~~ → 2026-02-04完了
2. 複雑なアルゴリズム（CobolInspect.common()等）への内部コメント追加
3. @seeタグによる関連関数の相互参照追加（コードレビュー提案）

## 参考資料

- COBOL 85規格
- opensource COBOL 4J GitHub リポジトリ
- libcob/common.c, libcob/strings.c, libcob/intrinsic.c

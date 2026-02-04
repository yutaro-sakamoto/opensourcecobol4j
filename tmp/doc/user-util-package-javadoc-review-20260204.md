# user_util パッケージ Javadoc レビュー情報

**更新日**: 2026-02-04
**パッケージ**: `jp.osscons.opensourcecobol.libcobj.user_util`

## 概要

このパッケージは、opensource COBOL 4J の補助ユーティリティツールのためのクラスを提供します。
主に以下の2つのCLIツールから使用されます：

1. **cobj-idx** (`libcobj/bin/cobj-idx`): INDEXEDファイルの操作ユーティリティ
2. **cobj-api** (`libcobj/bin/cobj-api`): Spring Boot向けAPIコード生成ユーティリティ

## サブパッケージ構成

### indexed_file サブパッケージ

INDEXEDファイル操作のためのクラス群。`cobj-idx`コマンドのメイン実装。

| クラス/インターフェース | 役割 |
|------------------------|------|
| `IndexedFileUtilMain` | cobj-idxコマンドのメインクラス。info/create/load/unload/migrate/unlockサブコマンドを提供 |
| `RecordReader` | レコード読み取りの抽象化インターフェース |
| `RecordWriter` | レコード書き込みの抽象化インターフェース |
| `FileSeqRecordReader` | SEQUENTIALフォーマットのファイル読み取り実装 |
| `FileLineSeqRecordReader` | LINE SEQUENTIALフォーマットのファイル読み取り実装 |
| `StdinRecordReader` | 標準入力からのレコード読み取り（内部に`StdinLineSeqReader`と`StdinSeqReader`を含む） |
| `CobolFileKeyInfo` | INDEXEDファイルのキー情報（offset, size, duplicate）を保持 |
| `LoadResult` | ロード操作の結果を示すenum |
| `UserDataFormat` | 入出力データ形式を示すenum（SEQUENTIAL/LINE_SEQUENTIAL） |
| `ErrorLib` | エラーメッセージ出力のユーティリティ |

### cobj_api サブパッケージ

Spring Boot向けAPI連携コード生成のためのクラス群。`cobj-api`コマンドのメイン実装。

| クラス | 役割 |
|--------|------|
| `ApiFiles` | COBOLプログラムからSpring Boot用ControllerとRecordクラスを生成 |
| `ApiFilesOptions` | cobj-apiコマンドのオプション解析・管理 |

## cobj-idx コマンドの動作

### サブコマンド一覧

1. **info** - INDEXEDファイルの情報表示（レコードサイズ、レコード数、キー位置）
2. **create** - 新規INDEXEDファイルの作成
3. **load** - 外部データをINDEXEDファイルにロード
4. **unload** - INDEXEDファイルのデータを外部出力
5. **migrate** - バージョン1.1.12以前のINDEXEDファイルを最新形式に移行
6. **unlock** - ファイルロックの解除

### データフォーマット

- **SEQUENTIAL (bin)**: レコードが区切りなく連結された固定長フォーマット
- **LINE_SEQUENTIAL (txt)**: 各レコードが改行文字で区切られたフォーマット

### INDEXEDファイルの内部構造

opensource COBOL 4JのINDEXEDファイルはSQLiteデータベースとして実装されています。
主要なテーブル：
- `table0`: レコードデータとロック情報
- `metadata_string_int`: レコードサイズなどのメタデータ
- `metadata_key`: キー情報（位置、サイズ、重複許可）
- `file_lock`: ファイルレベルのロック情報

## cobj-api コマンドの動作

### 入力

cobjコマンドの`-info-json-dir`オプションで生成されたJSONファイル。
このJSONには、COBOLプログラムのPROGRAM-IDやPROCEDURE DIVISION USING句の引数情報が含まれます。

### 出力

- `<PROGRAM-ID>Controller.java`: Spring Boot RESTコントローラークラス
- `<PROGRAM-ID>Record.java`: Java Recordクラス（リクエスト/レスポンス用）

生成されるControllerはGETとPOSTメソッドを持ち、COBOLプログラムを呼び出します。

## Javadoc 更新内容

### 更新されたファイル

1. **RecordReader.java** - インターフェースとファクトリメソッドの説明追加、`@see`タグ追加
2. **RecordWriter.java** - インターフェースと各メソッドの説明追加
3. **FileSeqRecordReader.java** - クラスとコンストラクタの説明追加
4. **FileLineSeqRecordReader.java** - クラス、フィールド、コンストラクタの説明追加
5. **StdinRecordReader.java** - 基底クラス、内部クラス、ファクトリメソッドの説明追加
6. **ErrorLib.java** - クラス説明の拡充、全メソッドの説明を動詞句形式に統一
7. **CobolFileKeyInfo.java** - クラス、フィールド、コンストラクタの説明追加
8. **IndexedFileUtilMain.java** - main、processInfoCommand、processLoadCommand、createCobolFileメソッドの説明更新
9. **ApiFilesOptions.java** - filePathフィールドの説明追加

### 言語について

- **indexed_file サブパッケージ**: 英語でJavadocを記述
- **cobj_api サブパッケージ**: 既存の日本語コメントを維持（プロジェクト内の一貫性のため）

## レビュー時の確認ポイント

1. **技術的正確性**: INDEXEDファイルの仕組み（SQLiteベース）、COBOLのファイル形式（SEQUENTIAL/LINE SEQUENTIAL）の説明が正確か
2. **一貫性**: メソッドのJavadocスタイル（動詞句で開始）の統一
3. **参照の完全性**: `@see`タグで関連クラス/enumへの参照が適切か
4. **パラメータ説明**: 特にoffsetの1-based/0-basedの明記

## 関連ファイル

- `libcobj/bin/cobj-idx` - cobj-idxコマンドのシェルスクリプト
- `libcobj/bin/cobj-api` - cobj-apiコマンドのシェルスクリプト

## 今後の検討事項

1. ApiFilesとApiFilesOptionsの英語化（国際化対応として）
2. IndexedFileUtilMainの複雑なprivateメソッドへのJavadoc追加検討
3. スレッドセーフティの文書化（必要に応じて）

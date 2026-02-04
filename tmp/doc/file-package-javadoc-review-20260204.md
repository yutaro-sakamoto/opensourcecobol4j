# file パッケージ Javadoc レビューガイド

## 概要

`jp.osscons.opensourcecobol.libcobj.file` パッケージは、COBOL のファイル操作をJavaで実装するためのクラス群を提供する。COBOLプログラムからJavaに変換された際、ファイル入出力（OPEN, CLOSE, READ, WRITE, REWRITE, DELETE, START）を実行するために使用される。

## COBOLファイル編成との対応

| COBOLファイル編成 | 対応クラス | 定数値 |
|---|---|---|
| SEQUENTIAL | CobolSequentialFile | COB_ORG_SEQUENTIAL (0) |
| LINE SEQUENTIAL | CobolLineSequentialFile | COB_ORG_LINE_SEQUENTIAL (1) |
| RELATIVE | CobolRelativeFile | COB_ORG_RELATIVE (2) |
| INDEXED | CobolIndexedFile | COB_ORG_INDEXED (3) |
| SORT | CobolSort | COB_ORG_SORT (4) |

## 主要クラスの役割

### CobolFile (基底クラス)
- すべてのファイル操作クラスの基底クラス
- ファイルオープン、クローズ、読み書きの共通ロジックを提供
- ファイルステータスの管理（00=成功、10=EOF、23=キーなし、等）
- ファイルロック機構の管理

### CobolFileFactory
- ファクトリパターンを使用して、organizationに応じた適切なCobolFileサブクラスを生成
- COBOLからJavaへ変換されたコードで使用される

### CobolFileKey
- INDEXED/RELATIVEファイルのキー情報を保持
- 主キーおよび代替キーの定義に使用
- COBOL RECORD KEY句の実装

### FileIO
- 低レベルのファイルI/O操作を抽象化
- java.nio.channels.FileChannelのラッパー
- 読み書きバッファ管理、シーク操作、ファイルロックを提供

### Linage
- COBOL LINAGE句の実装
- 論理ページ制御（FOOTING, TOP, BOTTOM）
- LINE SEQUENTIALファイルの印刷出力制御に使用

### CobolSort / CobolFileSort
- COBOL SORT/MERGE文の実装
- メモリ内ソートおよび外部ソート（一時ファイル使用）をサポート
- SORT USING / SORT GIVING句の処理

## オープンモード定数

| 定数 | 値 | COBOL対応 |
|---|---|---|
| COB_OPEN_CLOSED | 0 | ファイル未オープン |
| COB_OPEN_INPUT | 1 | OPEN INPUT |
| COB_OPEN_OUTPUT | 2 | OPEN OUTPUT |
| COB_OPEN_I_O | 3 | OPEN I-O |
| COB_OPEN_EXTEND | 4 | OPEN EXTEND |
| COB_OPEN_LOCKED | 5 | CLOSE WITH LOCK後の状態 |

## アクセスモード定数

| 定数 | 値 | COBOL対応 |
|---|---|---|
| COB_ACCESS_SEQUENTIAL | 1 | ACCESS MODE IS SEQUENTIAL |
| COB_ACCESS_DYNAMIC | 2 | ACCESS MODE IS DYNAMIC |
| COB_ACCESS_RANDOM | 3 | ACCESS MODE IS RANDOM |

## ファイルステータスコード

主要なステータスコード：

| 定数 | 値 | 説明 |
|---|---|---|
| COB_STATUS_00_SUCCESS | 0 | 正常終了 |
| COB_STATUS_10_END_OF_FILE | 10 | ファイル終端 |
| COB_STATUS_22_KEY_EXISTS | 22 | キー重複 |
| COB_STATUS_23_KEY_NOT_EXISTS | 23 | キーが存在しない |
| COB_STATUS_35_NOT_EXISTS | 35 | ファイルが存在しない |
| COB_STATUS_41_ALREADY_OPEN | 41 | ファイルは既にオープン済み |
| COB_STATUS_42_NOT_OPEN | 42 | ファイルがオープンされていない |
| COB_STATUS_51_RECORD_LOCKED | 51 | レコードロック中 |
| COB_STATUS_61_FILE_SHARING | 61 | ファイル共有競合 |

## INDEXEDファイルの実装詳細

CobolIndexedFileはSQLiteデータベースを使用してINDEXEDファイルを実装：

- 主キーテーブル (table0): key, value, locked_by, process_id, locked_at
- 代替キーテーブル (tableN): key, value[, dupNo]
- ファイルロックテーブル (file_lock): プロセス間の排他制御
- IndexedCursor: SQLiteでカーソル操作をエミュレート

## ソート処理の実装

CobolFileSortは以下の2つのソート戦略をサポート：

1. **Java標準ライブラリソート** (SORT_STD_LIB = true)
   - java.util.Collections.sortを使用
   - メモリ効率が良い

2. **外部マージソート** (SORT_STD_LIB = false)
   - 大量データ向け
   - 一時ファイルを使用したマージソート

## 変換後Javaコードでの使用例

```java
// ファイル生成
h_SEQ_FILE = CobolFileFactory.makeCobolFileInstance(
  "SEQ-FILE",           // select_name
  h_SEQ_FILE_status,    // file_status
  c_seqfile,            // assign
  f_SEQ_FILE_record,    // record
  null,                 // record_size
  80, 80,               // record_min, record_max
  0, null,              // nkeys, keys
  (char)0,              // organization = SEQUENTIAL
  (char)1,              // access_mode = SEQUENTIAL
  ...
);

// ファイル操作
h_SEQ_FILE.open(2, 1, f_WS_SEQ_STATUS);  // OPEN OUTPUT
h_SEQ_FILE.write(f_SEQ_RECORD, 0, f_WS_SEQ_STATUS);  // WRITE
h_SEQ_FILE.close(0, f_WS_SEQ_STATUS);  // CLOSE
```

## レビュー時の注意点

1. **libcobとの対応関係**: 多くのメソッドはopensource COBOLのlibcob/fileio.cの実装に対応している
2. **エラーコードの一貫性**: ファイルステータスコードはCOBOL標準に準拠
3. **スレッドセーフティ**: 現時点では単一スレッドでの使用を想定
4. **文字エンコーディング**: Shift_JISをデフォルトとして使用

## 更新履歴

- 2026-02-04: 初版作成（file パッケージ Javadoc整備に伴う）

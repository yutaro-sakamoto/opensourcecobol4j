# 環境変数リファレンス

opensource COBOL 4J のランタイムで使用される環境変数について説明します。

## 一般設定

### COB_DATE

実行時の日付を固定するための環境変数です。

- **形式**: `YYYY/MM/DD`
- **例**: `COB_DATE=2024/01/15`
- **用途**: `CURRENT-DATE` などの組み込み関数が返す日付を固定できます。テストや再現性のある実行に便利です。

### COB_VERBOSE

詳細なデバッグ情報を出力するかどうかを制御します。

- **値**: `Y` または `y` で有効化
- **例**: `COB_VERBOSE=Y`
- **用途**: ランタイムの動作を詳細に確認したい場合に使用します。

### COB_TERMINAL_ENCODING

DISPLAY文およびACCEPT文で使用するターミナルのエンコーディングを指定します。

- **値**: `UTF-8` または `UTF8`（デフォルトは Shift_JIS）
- **例**: `COB_TERMINAL_ENCODING=UTF-8`
- **用途**: UTF-8環境でのターミナル入出力に使用します。

### COB_SWITCH_1 〜 COB_SWITCH_8

COBOLプログラム内で参照できるスイッチの初期値を設定します。

- **値**: `ON` で有効化（それ以外は無効）
- **例**: `COB_SWITCH_1=ON`
- **用途**: プログラムの動作を切り替えるためのフラグとして使用します。

## ファイル入出力

### COB_FILE_PATH

ファイルを検索するデフォルトのディレクトリパスを指定します。

- **例**: `COB_FILE_PATH=/data/cobol/files`
- **用途**: SELECT句で指定されたファイル名に対して、このパスを基準に検索します。

### DD_ファイル名 / dd_ファイル名

特定のファイルに対して実際のファイルパスを指定します。

- **形式**: `DD_<ファイル名>=<実際のパス>` または `dd_<ファイル名>=<実際のパス>`
- **例**: `DD_MASTER=/data/master.dat`
- **用途**: COBOLプログラム内のファイル名を実行時に別のパスにマッピングできます。

### COB_SYNC

ファイル操作後にディスクへの同期を行うかどうかを制御します。

- **値**: `Y` または `y` で有効化
- **例**: `COB_SYNC=Y`
- **用途**: データの永続性を確保したい場合に使用します。

### COB_LS_NULLS

行順編成ファイルでNULL文字の処理を制御します。

- **用途**: 行順編成ファイルの読み書き時のNULL文字の扱いを指定します。

### COB_LS_FIXED

行順編成ファイルを固定長として扱うかどうかを制御します。

- **用途**: 行順編成ファイルのレコード長の扱いを指定します。

### OC_IO_CREATES

I-O モードでファイルを開く際に、ファイルが存在しない場合に自動作成するかどうかを制御します。

- **値**: `yes` で有効化
- **例**: `OC_IO_CREATES=yes`
- **用途**: OPEN I-O時にファイルが存在しない場合の挙動を制御します。

### OC_EXTEND_CREATES

EXTEND モードでファイルを開く際に、ファイルが存在しない場合に自動作成するかどうかを制御します。

- **値**: `yes` で有効化
- **例**: `OC_EXTEND_CREATES=yes`
- **用途**: OPEN EXTEND時にファイルが存在しない場合の挙動を制御します。

### COB_FILE_SEQ_WRITE_BUFFER_SIZE

順編成ファイルの書き込みバッファサイズを指定します。

- **値**: 0以上の整数（デフォルト: 10）
- **例**: `COB_FILE_SEQ_WRITE_BUFFER_SIZE=100`
- **用途**: 書き込みパフォーマンスを調整できます。

### COB_IO_ASSUME_REWRITE

REWRITEの前にREADが必要かどうかの動作を制御します。

- **値**: `Y` または `y` で有効化
- **例**: `COB_IO_ASSUME_REWRITE=Y`
- **用途**: READなしでREWRITEを許可する場合に使用します。

## 一時ファイル

### TMPDIR / TMP / TEMP

ソート処理などで使用する一時ファイルのディレクトリを指定します。

- **優先順位**: `TMPDIR` > `TMP` > `TEMP`
- **例**: `TMPDIR=/tmp/cobol`
- **用途**: ソートや一時ファイルの作成場所を指定します。

## プログラム呼び出し

### COB_LOAD_CASE

CALL文で指定されるプログラム名の大文字・小文字変換を制御します。

- **値**:
  - `LOWER`: 小文字に変換
  - `UPPER`: 大文字に変換
- **例**: `COB_LOAD_CASE=LOWER`
- **用途**: プログラム名の検索時のケース変換を制御します。

### COB_LIBRARY_PATH

CALL文で呼び出されるプログラムを検索するクラスパスを指定します。

- **形式**: ディレクトリパス（区切り文字はシステム依存）
- **例**: `COB_LIBRARY_PATH=/opt/cobol/lib`
- **用途**: 動的に呼び出すプログラムの検索パスを追加します。

### COB_PACKAGE_PATH

CALL文で呼び出されるJavaクラスのパッケージパスを指定します。

- **例**: `COB_PACKAGE_PATH=com.example.cobol`
- **用途**: Javaパッケージ構造でのプログラム検索に使用します。

### COB_PRE_LOAD

プログラム開始時に事前ロードするモジュールを指定します。

- **注意**: 現在未実装

## 数値処理

### COB_NIBBLE_C_UNSIGNED

符号なしパック10進数のニブル値に関する処理を制御します。

- **値**: `Y` または `y` で有効化
- **例**: `COB_NIBBLE_C_UNSIGNED=Y`
- **用途**: 特定のCOBOL方言との互換性のために使用します。

## 使用例

```bash
# 日付を固定して実行
export COB_DATE=2024/01/15

# UTF-8環境で実行
export COB_TERMINAL_ENCODING=UTF-8

# ファイルパスを設定して実行
export COB_FILE_PATH=/data/cobol
export DD_MASTER=/data/master.dat

# プログラムを実行
java -cp libcobj.jar:. MAINPROG
```

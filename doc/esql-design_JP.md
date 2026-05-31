## 概要

このドキュメントは Embedded SQL (EXEC SQL ... END-EXEC) のサポートが opensource COBOL 4J の内部でどのように構成されているかを示します。ユーザ向けの使い方は [esql-guide_JP.md](./esql-guide_JP.md) を参照してください。

ESQL サポートは大きく次の 2 層に分かれます。

1. **コンパイラ層 (`cobj/` 配下)**: EXEC SQL ブロックを字句・構文解析し、COBOL 通常の AST と同じ木構造に乗せたうえで、ホスト変数を `AbstractCobolField` 引数として渡す Java コードを生成する。
2. **ランタイム層 (`libcobj/.../sql/` 配下)**: 生成 Java から呼ばれて JDBC (PostgreSQL) を駆動し、結果を COBOL 領域に書き戻す。

## コンパイラ層

### 主要ファイル

| ファイル | 役割 |
|---|---|
| `cobj/esql-scanner.l` | flex レキサ。EXEC SQL ... END-EXEC で囲まれた SQL テキストをトークン化する。 |
| `cobj/esql-parser.y` | bison パーサ。トークン列からホスト変数情報を取り出して `cb_exec_sql` ノードを構築する。 |
| `cobj/esql-common.h` | `esql-parser.y` / `esql-scanner.l` と `esql.c` が共有する型と関数のヘッダ。tree.h を含めない (`YYSTYPE` の衝突を避けるため)。 |
| `cobj/esql.c` | `esql_build_and_resolve()` を提供。ホスト変数の型解決、GROUP の子展開、SELECT INTO/FETCH OCCURS への昇格、`cb_tree` 構築のラッパ関数群を持つ。 |
| `cobj/codegen.c` (`joutput_exec_sql` 周辺) | `cb_exec_sql` ノードを Java の `CobolSql.idExec(...)` 等の呼び出しに展開する。 |
| `cobj/typeck.c` | SQLCA が必要なプログラムに `01 SQLCA GLOBAL.` を暗黙挿入する。 |

### 解析パイプライン

```
EXEC SQL ... END-EXEC (COBOL ソース)
        │
        │  preproc (cobj/pplex.l, cobj/ppparse.y) が
        │  EXEC SQL〜END-EXEC を 1 つの文字列として保存
        ▼
esql-scanner.l (flex)
        │
        │  ホスト変数を '?' に置換しつつ SQL 本体を構築。
        │  ホスト変数や添字、修飾名は ESQL_HOSTTOKEN / HOSTSUB_* で
        │  パーサに渡す。
        ▼
esql-parser.y (bison)
        │
        │  cb_sql_host_var のリストとカーソル名、SQL 本体を組み立て、
        │  esql.c の esql_build_and_resolve() を呼ぶ。
        ▼
esql.c
        │
        │  - resolve_host_var_type(): COBOL の PIC / USAGE を見て
        │    HVARTYPE_* を決める。
        │  - expand_group_host_vars(): SELECT INTO 等で GROUP が
        │    渡されたら子フィールドの個別バインドに展開する。
        │  - OCCURS 検出: leaf に flag_occurs があれば
        │    SELECT_INTO_OCCURS / FETCH_OCCURS に昇格させる。
        │  cb_build_exec_sql() で cb_exec_sql ノードを返す。
        ▼
typeck.c / codegen.c
        │
        │  cb_exec_sql ノードを通常の COBOL 文と同じ経路で型検査・
        │  コード生成する。host_list / res_host_list の各 cb_reference
        │  は joutput_param 経由で b_X.getSubDataStorage(...) を含む
        │  AbstractCobolField 引数に変換される。
        ▼
Java ソース (CobolSql.idExec / .idSelectInto / .idFetchCursor ...)
```

### ホスト変数の AST 表現

ESQL のホスト変数は専用のノード型を増やさず、COBOL 通常の参照 (`cb_reference`) にそのまま乗せます。

| 入力表記 | 内部表現 |
|---|---|
| `:VAR` | `cb_reference{ word: VAR }` |
| `:VAR(IDX)` | `cb_reference{ word: VAR, subs: [IDX] }` |
| `:VAR(I, J)` | `cb_reference{ word: VAR, subs: [J, I] }` (leaf→root の順) |
| `:GRP.SUB` | `cb_reference{ word: SUB, chain: cb_reference{ word: GRP } }` |
| `:GRP.SUB(IDX)` | 上記 + `subs: [IDX]` |
| `:GRP.SUB(GRP2.IDX)` | 上記 + `subs: [cb_reference{ word: IDX, chain: GRP2 }]` |

これにより `codegen.c` の `joutput_param` / `joutput_data` (具体的には `cobj/codegen.c:972` の OCCURS 解決ループ) が **ESQL 専用パスを増やさずに**正しい `b_X.getSubDataStorage(...)` を含む `AbstractCobolField` を生成します。

### スキャナの状態遷移

`esql-scanner.l` は次の排他状態を持ちます。

- `INITIAL`: EXEC SQL の最初のキーワード (`SELECT` / `INSERT` / `CONNECT` / ...) を待つ。
- `ESQL_STATE`: SQL 本体を読む。空白・コメント・トークンを `esql_sqlbody` に追記しつつ、ホスト変数だけ `?` に差し替える。
- `ESQL_HOSTSUB_STATE`: ホスト変数の直後に `(` が来たときに push される。`(`, `)`, `,`, 整数、修飾 IDENT を **SQL 本体に書き込まずに** トークンとして返す。`)` で pop する。
- `ESQL_DBNAME_STATE`: `AT :NAME` の `:NAME` を取りに行く専用状態。
- `WHERE_CURRENT_OF`: `WHERE CURRENT OF cursor` のカーソル名を捕まえる専用状態。

`HOSTWORD` の正規表現は dotted 修飾 (`("\."IDENT)*`) を含み、`HOSTWORD/"("` の trailing context で「`(` が直後に来るか」を見て `HOSTSUB_STATE` への分岐を決めています。

### サブチャネル `esql_pending_subs`

添字情報は文法アクション間で静的変数 `esql_pending_subs` (`cb_tree` 型) を経由して渡されます。`host_reference` を還元した直後に親規則の `esql_add_host_var()` が必ず消費・クリアするので、複数ホスト変数が交錯することはありません。union を拡張せず最小限の変更で済ませるための妥協です。

### 集団項目修飾の構築

`esql_build_qualified_ref()` (`cobj/esql.c`) は `"GRP.SUB.X"` のような dotted 名を `strtok` で分割し、右端を leaf として `cb_reference` を作り、`chain` を leaf → 親 → 祖父 と連結します。COBOL 通常の `X OF SUB OF GRP` と同じ AST 形になり、`cb_ref` の resolver も同じパスを通ります。

### OCCURS の扱い

SELECT INTO / FETCH では、`esql_build_and_resolve()` が leaf に `flag_occurs` を持つホスト変数を検出すると、コマンドを `CB_SQL_SELECT_INTO_OCCURS` / `CB_SQL_FETCH_OCCURS` に昇格させ、`occurs_size` (1 要素のバイト数) と `occurs_max` (要素数) を `cb_exec_sql` ノードに記録します。ランタイム側はこの 2 値を使って結果セットを配列の連続領域に書き込みます。

`expand_group_host_vars()` は SELECT INTO に GROUP がそのまま渡されたとき、子フィールドを個別のホスト変数に展開して並べる責任を持ちます (PostgreSQL 側からは N 列の SELECT として扱える)。

## ランタイム層

### パッケージ構成 (`jp.osscons.opensourcecobol.libcobj.sql`)

| クラス | 可視性 | 役割 |
|---|---|---|
| `CobolSql` | `public` | 生成 Java から呼ばれる唯一の公開 API。`idConnect`, `idExec`, `idExecParams`, `idSelectInto`, `idDeclareCursor`, `idOpenCursor`, `idFetchCursor`, `idCloseCursor`, `idPrepare`, `idExecPrepared`, `idCommit`, `idRollback`, `idSavepoint`, `idDisconnect` を提供。 |
| `SqlState` | package-private | 接続テーブル (`addConnection`/`getConnection`)、PREPARE テーブル、カーソルテーブルを保持する内部状態管理。 |
| `SqlConnection` | package-private | JDBC `Connection` のラッパ。接続文字列 `dbname@host:port` のパース、デフォルト DB 名解決などを担う。 |
| `SqlCursor` | package-private | カーソルの状態 (open/closed)、`ResultSet`、`PreparedStatement` の組を保持する。 |
| `SqlCA` | package-private | SQLCA フィールド (`SQLCODE`, `SQLSTATE`, `SQLERRMC`, `SQLERRD`, ...) を `CobolDataStorage` に書き戻す。 |
| `CobolDataConverter` | package-private | `AbstractCobolField` ⇔ JDBC `PreparedStatement.setXxx` / `ResultSet.getXxx` の変換を担当。`HVARTYPE_*` enum によって分岐する。 |

`SqlConnection`, `SqlCursor`, `SqlState`, `SqlCA`, `CobolDataConverter` はすべて package-private なため、SLF4J のロガー名としては利用できますが、外部から直接 import することは想定していません。

### CobolSql のシグネチャ

生成 Java から渡されるホスト変数は `AbstractCobolField[]` です。コンパイラ側で添字や修飾は解決済みのため、ランタイムは **添字や階層構造を意識せず**、配列の各要素を `CobolDataConverter` 経由で JDBC のパラメータ / 結果カラムに対応付けるだけです。

### SQLCA の自動定義

`typeck.c` は EXEC SQL を含む `PROGRAM-ID` に対し、明示的な `EXEC SQL INCLUDE SQLCA` がなくても `01 SQLCA GLOBAL.` を WORKING-STORAGE に自動挿入します。`SQLERRMC` などは `OCCURS 6` を含む構造なので、`b_SQLERRD__SQLCA.getSubDataStorage(...)` を生成時に組み立てる対象になります。

### NULL 列の通知 (ECPG_MISSING_INDICATOR)

ECPG 互換の `ECPG_MISSING_INDICATOR (-22002)` を `SQLCODE` として返す経路があり、ホスト変数側にインジケータが用意されていない状況で NULL を読もうとした場合に SQLCA 経由で通知します。JUnit テスト `CobolSqlTest`, `SqlCATest` に該当ケースが含まれます。

## テスト

### コンパイラ・統合テスト (`tests/`)

PostgreSQL コンテナを使う autotest スイートを以下のディレクトリで定義しています。

| ディレクトリ | 内容 |
|---|---|
| `tests/esql-basic.src/` | CONNECT / DISCONNECT / 基本 INSERT / SELECT |
| `tests/esql-cobol-data.src/` | COBOL データ型 × SQL 型のラウンドトリップ (数値・パック 10 進・英数字・日本語・VARYING・添字付き) |
| `tests/esql-sql-data.src/` | SQL 側の型バリエーション |
| `tests/esql-sqlca.src/` | SQLCA フィールドのアサーション |
| `tests/esql-misc.src/` | カーソル / PREPARE / EXECUTE / SAVEPOINT 等 |
| `tests/esql-utf8.src/` | UTF-8 ビルド時の対応 |

各スイートは `make <name>` で生成され、`./<name>` でローカル実行できます (PostgreSQL コンテナが必要)。CI からは `.github/workflows/test-esql.yml` 経由で実行されます。

### libcobj 単体テスト (`libcobj/app/src/test/.../sql/`)

`CobolSqlTest`, `SqlStateTest`, `SqlCursorTest`, `SqlConnectionTest`, `CobolDataConverterTest`, `CobolSqlLoggingTest`, `SqlCATest` が JDBC モック (testcontainers の PostgreSQL イメージ) に対する単体検証を行います。

## 設計上の意思決定

- **ESQL 専用 AST を増やさない**: ホスト変数は `cb_reference` の `subs`/`chain` にそのまま乗せ、`codegen.c` の既存パスで処理する。これにより多次元添字・集団項目修飾・修飾名添字が `joutput_param` / `joutput_data` の OCCURS 解決ループに自然に乗る。
- **scanner 状態を分離する**: 添字や修飾名の解析を `ESQL_HOSTSUB_STATE` に閉じ込めることで、SQL 本体への文字列追記 (`esql_sqlbody_append`) を **その状態では絶対に行わない** という構造的保証を作っている。`?` 以外がプレースホルダ位置に混入する事故が起きない。
- **dblibj を捨てる**: Open-COBOL-ESQL の Scala 実装 (dblibj) への依存を撤廃し、純粋な Java で `libcobj.jar` 内に閉じる。配布物が `libcobj.jar` 1 本になり、PostgreSQL JDBC ドライバもバンドルする。
- **`OF` 修飾を採用しない**: COBOL 本体は `X OF Y` も受け付けるが、ESQL の修飾は dotted 形式 (`:Y.X`) のみに統一する。スキャナの状態数を抑えるためと、Embedded SQL の文脈で `OF` が予約語衝突を招きにくくするため。

## 関連ドキュメント

- [esql-guide_JP.md](./esql-guide_JP.md) -- ユーザ向け使い方
- [esql-logging_JP.md](./esql-logging_JP.md) -- SLF4J ログ出力
- [esql-design.md](./esql-design.md) -- 本ドキュメントの英語版

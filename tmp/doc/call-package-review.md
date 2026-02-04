# jp.osscons.opensourcecobol.libcobj.call パッケージ Javadocレビュー用ドキュメント

## パッケージ概要

このパッケージはCOBOLのCALL文に関連する機能を提供する。COBOLからJavaに変換されたプログラム間の呼び出しを管理し、組み込み関数を提供する。

## クラス構成

### 1. CobolRunnable (インターフェース)

COBOLプログラムの実行単位が実装すべきインターフェース。cobj（COBOLトランスパイラ）が生成するJavaクラスはすべてこのインターフェースを実装する。

**メソッド:**
- `run(CobolDataStorage... storages)`: プログラムを実行。引数はCALL文のUSING句で渡されるデータ。
- `cancel()`: CANCEL文に対応。プログラムの状態をリセットする。キャンセルされたプログラムは次回のCALL時に再初期化される。
- `isActive()`: プログラムが実行中かどうかを返す。再帰呼び出しやアクティブなプログラムのキャンセル防止に使用。

**使用例（生成コード）:**
```java
public class SUB__TEST implements CobolRunnable {
    @Override
    public int run(CobolDataStorage... argStorages) {
        return SUB__TEST_(0, argStorages);
    }

    @Override
    public void cancel() {
        SUB__TEST_(-1);
    }

    @Override
    public boolean isActive() {
        return false;  // 通常はinitializedフラグで管理される
    }
}
```

### 2. CobolResolve

動的にCOBOLプログラム（Javaクラス）を読み込み、CALL文の機能を実装するクラス。

**主要機能:**
- プログラム名からCobolRunnableインスタンスを取得（リフレクション使用）
- コールスタックの管理（pushCallStackList/popCallStackList）
- CANCEL文の実装（cobCancel/fieldCancel）
- 環境変数による動作制御（COB_LOAD_CASE, COB_LIBRARY_PATH, COB_PACKAGE_PATH）

**使用例（生成コード）:**
```java
// プログラム開始時
CobolResolve.pushCallStackList("SUB__TEST");

// サブプログラムのCALL
call_SUB__PROGRAM = CobolResolve.resolve(null, "SUB-PROGRAM", call_SUB__PROGRAM);
b_RETURN_CODE.set(call_SUB__PROGRAM.run(b_WS_PARAM));

// CANCEL文
CobolResolve.fieldCancel(c_1_SUB);

// プログラム終了時
CobolResolve.popCallStackList();
```

### 3. CobolSystemRoutine

COBOL組み込み関数（システムルーチン）を提供するクラス。すべてstaticメソッド。

**提供する組み込み関数:**
| 関数名 | 機能 |
|--------|------|
| SYSTEM | OSコマンドを実行 |
| C$SLEEP | 指定秒数スリープ |
| CBL_AND | バイト単位の論理積 |
| CBL_OR | バイト単位の論理和 |
| CBL_XOR | バイト単位の排他的論理和 |
| CBL_NOR | バイト単位の否定論理和 |
| CBL_NIMP | バイト単位の否定含意（data1 AND NOT data2） |
| CBL_EQ | バイト単位の排他的否定論理和（XNOR） |
| CBL_NOT | バイト単位の論理否定 |
| CBL_XF4 | 8バイトのビット配列を1バイトに集約 |
| CBL_XF5 | 1バイトを8バイトのビット配列に展開 |
| CBL_X91 | スイッチやパラメータ数の取得・設定 |
| CBL_TOLOWER | 小文字変換 |
| CBL_TOUPPER | 大文字変換 |
| CBL_OC_NANOSLEEP | ナノ秒単位のスリープ |
| calledBy | 呼び出し元プログラム名を取得 |
| listDir | ディレクトリ一覧を取得 |

### 4. CobolCallStackList

CALLしたプログラムを階層構造で管理するクラス。木構造でコールスタックを表現する。

**構造:**
- parent: 呼び出し元プログラム
- children: 最初に呼び出された子プログラム
- sister: 同じ親から呼び出された兄弟プログラム

## 作業完了状況

### 修正済み

以下の「準備中」コメントをすべて意味のある説明に修正した：

#### CobolRunnable.java
- `cancel()`: プログラムの状態を初期化してキャンセルする説明を追加
- `isActive()`: CANCEL文との関連とプログラム実行状態を確認する目的を記述

#### CobolSystemRoutine.java
- 論理演算関数（CBL_OR, CBL_NOR, CBL_XOR, CBL_NIMP, CBL_EQ）のすべてのオーバーロード
- CBL_NOT, CBL_XF4, CBL_XF5, CBL_X91
- C$SLEEP（誤字「すう」も修正）
- CBL_TOLOWER, CBL_TOUPPER のオーバーロード
- CBL_OC_NANOSLEEP, calledBy, listDir
- SYSTEMの@throws説明

#### CobolResolve.java
- 4つのresolveメソッドオーバーロードの@throws CobolRuntimeException説明
- fieldCancelとcobCancelの@throws CobolStopRunException説明

## 既知の問題点（コードレビューで発見）

以下はJavadoc修正とは別のコード品質の問題として発見されたもの：

### CobolSystemRoutine.java
- **CBL_OC_NANOSLEEP内のデバッグ出力**: `System.out.println(nsecs);`（834行目付近）が残っている

### CobolResolve.java
- **getInstance()メソッド内の重複コード**: 447-448行目でインスタンスを2回生成している
  ```java
  runnable = (CobolRunnable) cons.newInstance();
  runnable = (CobolRunnable) c.getDeclaredConstructor().newInstance();
  ```
- **自動生成コメント**: 458行目の`// TODO 自動生成された catch ブロック`が残っている

これらはJavadoc修正の範囲外だが、別途対応が推奨される。

## レビュー観点

Javadocコメントをレビューする際は以下を確認：
1. メソッドの実装と説明が一致しているか
2. @param、@return、@throwsの記述が適切か
3. 日本語として自然で理解しやすいか
4. COBOLの知識がなくても概要が理解できるか

# data パッケージ Javadoc 更新作業記録

## 対象パッケージ
`jp.osscons.opensourcecobol.libcobj.data`

## 作業概要
「準備中」（TODO）となっているJavadocコメントを意味のある説明に更新する作業

## 完了済みファイル

### CobolFieldAttribute.java
- クラスJavadoc更新
- 全76個の「準備中」コメントを更新（COB_TYPE_*, COB_FLAG_*定数、isType*, isFlag*メソッド）

### CobolFieldFactory.java
- 3つのmakeCobolFieldメソッドのJavadoc更新

### CobolNumericBinaryField.java
- クラスJavadoc更新
- setLongValueメソッドのJavadoc更新

### CobolNumericPackedField.java
- checkNumericメソッド更新
- upper4bitsメソッド更新
- cmpPackedIntメソッド更新
- cmpPackedメソッド更新

### CobolAlphanumericField.java
- moveAlphanumToAlphanum静的メソッドのJavadoc更新

### CobolNumericField.java
- checkNumericメソッド更新
- storeCommonRegion（2つのオーバーロード）更新
- displayAddIntメソッド更新

### CobolDataStorage.java (部分完了)
- memcmp, setByte, setBytes, set, compareToBinary, addBinary, subBinary等のメソッド更新

## 残作業

### CobolDataStorage.java
以下の系統のメソッドに「準備中」コメントが残存（約150+メソッド）:
- cmpU8Binary, cmpS8Binary, ... cmpU64Binary, cmpS64Binary
- cmpAlignU8Binary, ... cmpAlignS64Binary
- addU8Binary, addS8Binary, ... addU64Binary, addS64Binary
- addAlignU8Binary, ... addAlignS64Binary
- subU8Binary, subS8Binary, ... subU64Binary, subS64Binary
- subAlignU8Binary, ... subAlignS64Binary
- cmpSwpU8Binary, ... cmpSwpS64Binary
- cmpSwpAlignU8Binary, ... cmpSwpAlignS64Binary
- addSwpU8Binary, ... addSwpS64Binary
- addSwpAlignU8Binary, ... addSwpAlignS64Binary
- subSwpU8Binary, ... subSwpS64Binary
- subSwpAlignU8Binary, ... subSwpAlignS64Binary
- setSwpU16Binary, ... setSwpS64Binary
- cmpNumdisp, cmpLongNumdisp, cmpSignNumdisp, cmpLongSignNumdisp
- getNumdisp, getEbcdicSign, getLongEbcdicSign
- bswap_16, bswap_32, bswap_64

### 未確認ファイル（要確認）
以下のファイルは未読み込み・未確認:
- CobolNationalField.java
- CobolNumericEditedField.java
- CobolAlphanumericEditedField.java
- CobolNationalEditedField.java
- CobolAlphanumericAllField.java
- CobolNationalAllField.java
- CobolNumericDoubleField.java

## メソッド命名パターン（CobolDataStorage.java）

バイナリ操作メソッドの命名規則:
- `cmp/add/sub`: 比較/加算/減算
- `U/S`: Unsigned/Signed（符号なし/符号あり）
- `8/16/24/32/40/48/56/64`: ビット数
- `Binary`: バイナリ形式
- `Align`: アラインメント考慮版
- `Swp`: スワップ（バイトオーダー変換）版

例: `addSwpAlignS32Binary` = バイトスワップ+アライン済み+符号付き32ビットバイナリへの加算

## 技術メモ

### COBOLデータ型とJavaクラスの対応
| COBOL定義 | Javaクラス |
|-----------|------------|
| PIC 9(n) | CobolNumericField |
| PIC X(n) | CobolAlphanumericField |
| PIC N(n) | CobolNationalField |
| PIC 9(n) COMP/COMP-4/BINARY | CobolNumericBinaryField |
| PIC 9(n) COMP-3 | CobolNumericPackedField |
| 数字編集項目 | CobolNumericEditedField |
| GROUP項目 | CobolGroupField |

### 主要フラグ（CobolFieldAttribute）
- COB_FLAG_HAVE_SIGN: 符号付き
- COB_FLAG_SIGN_SEPARATE: 符号分離
- COB_FLAG_SIGN_LEADING: 符号先行
- COB_FLAG_BLANK_ZERO: ゼロ時空白
- COB_FLAG_JUSTIFIED: 右寄せ
- COB_FLAG_BINARY_SWAP: バイトスワップ
- COB_FLAG_REAL_BINARY: 実バイナリ

## 最終更新日
2026-02-04

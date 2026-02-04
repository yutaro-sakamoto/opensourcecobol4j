package jp.osscons.opensourcecobol.libcobj.ui;

/**
 * COBOLプログラムの実行結果取得時に発生する例外。
 *
 * <p>この例外は、{@link CobolResultSet}から結果を取得する際に以下のような状況で発生します：
 * <ul>
 *   <li>インデックスが有効範囲外の場合（1未満または結果数を超える場合）</li>
 *   <li>結果の型と要求した型が一致しない場合（例：String型の結果に対して{@code getInt()}を呼び出した場合）</li>
 * </ul>
 *
 * @see CobolResultSet
 * @see CobolCallResult
 */
public class CobolResultSetException extends Exception {
    /**
     * 指定されたエラーメッセージで新しい例外を生成する。
     *
     * @param message エラーの詳細を説明するメッセージ
     */
    public CobolResultSetException(String message) {
        super(message);
    }
}

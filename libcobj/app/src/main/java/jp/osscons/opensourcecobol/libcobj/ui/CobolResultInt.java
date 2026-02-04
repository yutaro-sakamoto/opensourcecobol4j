package jp.osscons.opensourcecobol.libcobj.ui;

/**
 * int型のCOBOL呼び出し結果を保持するクラス。
 *
 * <p>このクラスは、COBOLプログラムの整数型パラメータの値を保持します。
 * COBOLの数値フィールドのうち、小数点以下の桁数（scale）が0のものがこのクラスで表現されます。
 * 例えば、{@code PIC 9(5)}や{@code PIC S9(8)}のような整数型フィールドが該当します。
 *
 * <p>opensource COBOL 4Jのトランスパイラは、COBOLプログラムの{@code execute()}メソッドの戻り値として
 * {@link CobolResultSet}を生成し、その中に各パラメータの値を適切な型で格納します。
 *
 * @see CobolCallResult
 * @see CobolResultSet
 */
public class CobolResultInt extends CobolCallResult {
    private int value;

    /**
     * 指定されたint値で新しいインスタンスを生成する。
     *
     * @param i 保持するint値
     */
    public CobolResultInt(int i) {
        this.value = i;
    }

    /**
     * 保持しているint値を取得する。
     *
     * <p>このオーバーライドされたメソッドは例外をスローしません。
     *
     * @return 保持しているint値
     */
    @Override
    public int getInt() throws CobolResultSetException {
        return this.value;
    }
}

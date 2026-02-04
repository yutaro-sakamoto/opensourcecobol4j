package jp.osscons.opensourcecobol.libcobj.ui;

/**
 * String型のCOBOL呼び出し結果を保持するクラス。
 *
 * <p>このクラスは、COBOLプログラムの英数字型パラメータの値を保持します。
 * COBOLの非数値フィールドがこのクラスで表現されます。
 * 例えば、{@code PIC X(10)}や{@code PIC A(20)}のような英数字型フィールドが該当します。
 *
 * <p>opensource COBOL 4Jのトランスパイラは、COBOLプログラムの{@code execute()}メソッドの戻り値として
 * {@link CobolResultSet}を生成し、その中に各パラメータの値を適切な型で格納します。
 *
 * @see CobolCallResult
 * @see CobolResultSet
 */
public class CobolResultString extends CobolCallResult {
    private String value;

    /**
     * 指定されたString値で新しいインスタンスを生成する。
     *
     * @param s 保持するString値
     */
    public CobolResultString(String s) {
        this.value = s;
    }

    /**
     * 保持しているString値を取得する。
     *
     * <p>このオーバーライドされたメソッドは例外をスローしません。
     *
     * @return 保持しているString値
     */
    @Override
    public String getString() throws CobolResultSetException {
        return this.value;
    }
}

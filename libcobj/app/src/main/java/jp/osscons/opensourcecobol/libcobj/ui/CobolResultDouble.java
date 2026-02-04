package jp.osscons.opensourcecobol.libcobj.ui;

/**
 * double型のCOBOL呼び出し結果を保持するクラス。
 *
 * <p>このクラスは、COBOLプログラムの小数点付き数値パラメータの値を保持します。
 * COBOLの数値フィールドのうち、小数点以下の桁数（scale）が1以上のものがこのクラスで表現されます。
 * 例えば、{@code PIC 9(3)V9(2)}や{@code PIC S9(5)V99}のような小数点付きフィールドが該当します。
 *
 * <p>opensource COBOL 4Jのトランスパイラは、COBOLプログラムの{@code execute()}メソッドの戻り値として
 * {@link CobolResultSet}を生成し、その中に各パラメータの値を適切な型で格納します。
 *
 * @see CobolCallResult
 * @see CobolResultSet
 */
public class CobolResultDouble extends CobolCallResult {
    private double value;

    /**
     * 指定されたdouble値で新しいインスタンスを生成する。
     *
     * @param d 保持するdouble値
     */
    public CobolResultDouble(double d) {
        this.value = d;
    }

    /**
     * 保持しているdouble値を取得する。
     *
     * <p>このオーバーライドされたメソッドは例外をスローしません。
     *
     * @return 保持しているdouble値
     */
    @Override
    public double getDouble() throws CobolResultSetException {
        return this.value;
    }
}

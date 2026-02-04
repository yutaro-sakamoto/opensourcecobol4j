package jp.osscons.opensourcecobol.libcobj.ui;

/**
 * COBOLプログラムの呼び出し結果を保持する基底クラス。
 *
 * <p>このクラスは、COBOLプログラムの実行結果を型安全に取得するための抽象的な基底クラスです。
 * 実際の値は、{@link CobolResultInt}、{@link CobolResultDouble}、{@link CobolResultString}
 * などのサブクラスに格納されます。
 *
 * <p>各getterメソッドはデフォルトで例外をスローします。サブクラスは自身が保持する型に対応する
 * getterメソッドのみをオーバーライドします。これにより、型の不一致を実行時に検出できます。
 *
 * @see CobolResultInt
 * @see CobolResultDouble
 * @see CobolResultString
 * @see CobolResultSet
 */
public class CobolCallResult {
    /**
     * 結果をint型として取得する。
     *
     * <p>このメソッドは基底クラスでは常に例外をスローします。
     * {@link CobolResultInt}でのみ正常に値を返します。
     *
     * @return int型の結果値
     * @throws CobolResultSetException この結果がint型でない場合
     */
    public int getInt() throws CobolResultSetException {
        throw new CobolResultSetException("The result type is not 'int'");
    }

    /**
     * 結果をdouble型として取得する。
     *
     * <p>このメソッドは基底クラスでは常に例外をスローします。
     * {@link CobolResultDouble}でのみ正常に値を返します。
     *
     * @return double型の結果値
     * @throws CobolResultSetException この結果がdouble型でない場合
     */
    public double getDouble() throws CobolResultSetException {
        throw new CobolResultSetException("The result type is not 'double'");
    }

    /**
     * 結果をString型として取得する。
     *
     * <p>このメソッドは基底クラスでは常に例外をスローします。
     * {@link CobolResultString}でのみ正常に値を返します。
     *
     * @return String型の結果値
     * @throws CobolResultSetException この結果がString型でない場合
     */
    public String getString() throws CobolResultSetException {
        throw new CobolResultSetException("The result type is not 'String'");
    }
}

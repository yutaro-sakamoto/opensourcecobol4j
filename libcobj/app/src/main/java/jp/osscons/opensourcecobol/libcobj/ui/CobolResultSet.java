package jp.osscons.opensourcecobol.libcobj.ui;

/**
 * COBOLプログラムの実行結果を保持するクラス。
 *
 * <p>このクラスは、COBOLプログラムの{@code execute()}メソッドの戻り値として使用され、
 * プログラムの戻りコード（RETURN-CODE）と、各パラメータの実行後の値を保持します。
 *
 * <p>結果へのアクセスは1ベースのインデックスを使用します。これはCOBOLの配列が
 * 1から始まる慣例に従っています。
 *
 * <p>使用例：
 * <pre>{@code
 * try {
 *     // COBOLプログラムを実行
 *     MyCobolProgram prog = new MyCobolProgram();
 *     CobolResultSet result = prog.execute(param1, param2);
 *
 *     // 戻りコードを確認
 *     int returnCode = result.getReturnCode();
 *
 *     // パラメータの値を取得（1ベースのインデックス）
 *     String value1 = result.getString(1);
 *     int value2 = result.getInt(2);
 * } catch (CobolResultSetException e) {
 *     // 型の不一致やインデックス範囲外のエラー処理
 *     System.err.println("結果取得エラー: " + e.getMessage());
 * }
 * }</pre>
 *
 * @see CobolCallResult
 * @see CobolResultSetException
 */
public class CobolResultSet {
    private CobolCallResult results[];
    private int returnCode;

    /**
     * 戻りコードと結果の配列で新しいインスタンスを生成する。
     *
     * <p>このコンストラクタは主にトランスパイラが生成するコードから呼び出されます。
     *
     * @param returnCode COBOLプログラムの戻りコード（RETURN-CODE）
     * @param results 各パラメータの結果を保持する{@link CobolCallResult}の可変長配列
     */
    public CobolResultSet(int returnCode, CobolCallResult... results) {
        this.returnCode = returnCode;
        this.results = results;
    }

    /**
     * インデックスが有効な範囲内かを検証する。
     *
     * <p>検証条件：
     * <ul>
     *   <li>結果が1件以上存在すること</li>
     *   <li>インデックスが1以上であること（1ベースのインデックス）</li>
     *   <li>インデックスが結果の総数以下であること</li>
     * </ul>
     *
     * @param index 検証するインデックス
     * @throws CobolResultSetException インデックスが有効範囲外の場合
     */
    private void checkIndexInValidRange(int index) throws CobolResultSetException {
        if (results.length == 0 || index < 1 || this.results.length < index) {
            throw new CobolResultSetException("The index is out of range.");
        }
    }

    /**
     * COBOLプログラムの戻りコード（RETURN-CODE）を取得する。
     *
     * @return プログラムの戻りコード
     */
    public int getReturnCode() {
        return this.returnCode;
    }

    /**
     * 指定されたインデックスの結果をString型として取得する。
     *
     * @param index 結果のインデックス（1から始まる）
     * @return 指定されたインデックスのString値
     * @throws CobolResultSetException インデックスが範囲外の場合、または結果がString型でない場合
     */
    public String getString(int index) throws CobolResultSetException {
        this.checkIndexInValidRange(index);
        return this.results[index - 1].getString();
    }

    /**
     * 指定されたインデックスの結果をint型として取得する。
     *
     * @param index 結果のインデックス（1から始まる）
     * @return 指定されたインデックスのint値
     * @throws CobolResultSetException インデックスが範囲外の場合、または結果がint型でない場合
     */
    public int getInt(int index) throws CobolResultSetException {
        this.checkIndexInValidRange(index);
        return this.results[index - 1].getInt();
    }

    /**
     * 指定されたインデックスの結果をdouble型として取得する。
     *
     * @param index 結果のインデックス（1から始まる）
     * @return 指定されたインデックスのdouble値
     * @throws CobolResultSetException インデックスが範囲外の場合、または結果がdouble型でない場合
     */
    public double getDouble(int index) throws CobolResultSetException {
        this.checkIndexInValidRange(index);
        return this.results[index - 1].getDouble();
    }
}

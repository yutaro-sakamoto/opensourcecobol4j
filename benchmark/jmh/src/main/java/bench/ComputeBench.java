package bench;

import java.util.concurrent.TimeUnit;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolDecimal;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory;
import jp.osscons.opensourcecobol.libcobj.common.CobolModule;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;
import org.openjdk.jmh.annotations.*;

/**
 * COMPUTE / ADD 文のホットパスを再現するベンチマーク。
 *
 * cobj が COMPUTE WS-AMT = WS-PRICE * WS-QTY * WS-RATE (COMP-3項目) に対して
 * 生成するコード列 (CobolDecimal の setField/mul/getField) をそのまま実行し、
 * 等価な long 演算 (スケール済み整数) と比較する。
 */
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.NANOSECONDS)
@Warmup(iterations = 3, time = 1)
@Measurement(iterations = 5, time = 1)
@Fork(1)
@State(Scope.Thread)
public class ComputeBench {

    AbstractCobolField fPrice; // PIC S9(7)V99 COMP-3
    AbstractCobolField fQty; // PIC S9(5)     COMP-3
    AbstractCobolField fRate; // PIC S9(3)V99  COMP-3
    AbstractCobolField fAmt; // PIC S9(13)V99 COMP-3
    AbstractCobolField fTotalDisp; // PIC S9(15)V99 (ゾーン10進)
    AbstractCobolField fAmtDisp; // PIC S9(13)V99 (ゾーン10進)
    CobolDecimal d0;
    CobolDecimal d1;
    CobolDecimal d2;

    long lPrice; // スケール2
    long lQty; // スケール0
    long lRate; // スケール2
    long lAmt;
    long lTotal;

    private static AbstractCobolField packed(int digits, int scale) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
                        digits,
                        scale,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        return CobolFieldFactory.makeCobolField(
                digits / 2 + 1, new CobolDataStorage(digits / 2 + 1), attr);
    }

    private static AbstractCobolField display(int digits, int scale) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY,
                        digits,
                        scale,
                        CobolFieldAttribute.COB_FLAG_HAVE_SIGN,
                        null);
        return CobolFieldFactory.makeCobolField(
                digits, new CobolDataStorage(digits), attr);
    }

    @Setup
    public void setup() {
        // 生成コードの run_module と同じモジュールコンテキストを用意する
        CobolModule.push(
                new CobolModule(null, null, null, null, 0, '.', '$', ',', 1, 1, 1, 0, null));
        CobolDecimal.cobInitNumeric();
        fPrice = packed(9, 2);
        fQty = packed(5, 0);
        fRate = packed(5, 2);
        fAmt = packed(15, 2);
        fTotalDisp = display(17, 2);
        fAmtDisp = display(15, 2);
        // 数値項目への moveFrom(String) は未変換で生バイトを書き込むため使えない。
        // moveFrom(int) はスケール込みの整数 (12345 → 123.45) として格納される。
        fPrice.moveFrom(12345); // 123.45
        fQty.moveFrom(7);
        fRate.moveFrom(108); // 1.08
        fTotalDisp.moveFrom(0);
        fAmtDisp.moveFrom(93328); // 933.28
        d0 = new CobolDecimal();
        d1 = new CobolDecimal();
        d2 = new CobolDecimal();
        lPrice = 12345;
        lQty = 7;
        lRate = 108;
    }

    /** 現実装: COMPUTE AMT = PRICE * QTY * RATE の生成コード列そのもの */
    @Benchmark
    public int computePacked_current() throws CobolStopRunException {
        d0.setField(fPrice);
        d2.set(fQty.getInt());
        d0.mul(d2);
        d1.setField(fRate);
        d0.mul(d1);
        // getField の戻り値 (ステータス) を return しデッドコード除去を防ぐ
        return d0.getField(fAmt, 4);
    }

    /** 最適化代替: スケール済み long 演算による等価計算 */
    @Benchmark
    public long computePacked_longBaseline() {
        lAmt = lPrice * lQty * lRate / 100;
        return lAmt;
    }

    /** 累積によるオーバーフローを避けるためイテレーションごとに累計をリセットする */
    @Setup(Level.Iteration)
    public void resetTotal() {
        fTotalDisp.moveFrom(0);
        lTotal = 0;
    }

    /** 現実装: ADD AMT TO TOTAL (ゾーン10進どうし) */
    @Benchmark
    public int addDisplay_current() throws CobolStopRunException {
        return fTotalDisp.add(fAmtDisp, 4);
    }

    /** 最適化代替: long 加算 */
    @Benchmark
    public long addDisplay_longBaseline() {
        lTotal += lAmt;
        return lTotal;
    }

    /** 数値項目からの CobolDecimal 取り出し (毎回 BigDecimal を生成する) */
    @Benchmark
    public CobolDecimal getDecimal_packed() {
        return fPrice.getDecimal();
    }

    /** ゾーン10進の getInt (PERFORM の添字評価等で頻出) */
    @Benchmark
    public int getInt_display() {
        return fTotalDisp.getInt();
    }
}

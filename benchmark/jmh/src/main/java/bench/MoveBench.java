package bench;

import java.util.concurrent.TimeUnit;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory;
import jp.osscons.opensourcecobol.libcobj.common.CobolModule;
import org.openjdk.jmh.annotations.*;

/**
 * MOVE 文のホットパス: 英数字項目間の moveFrom、数値型間の変換 MOVE、
 * および DISPLAY 文で使われる getString() を計測する。
 */
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.NANOSECONDS)
@Warmup(iterations = 3, time = 1)
@Measurement(iterations = 5, time = 1)
@Fork(1)
@State(Scope.Thread)
public class MoveBench {

    AbstractCobolField alnumSrc100;
    AbstractCobolField alnumDst100;
    AbstractCobolField alnumDst30;
    AbstractCobolField disp9;
    AbstractCobolField packed9;
    AbstractCobolField binary9;

    private static AbstractCobolField alnum(int size) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        CobolFieldAttribute.COB_TYPE_ALPHANUMERIC, 0, 0, 0, null);
        return CobolFieldFactory.makeCobolField(
                size, new CobolDataStorage(size), attr);
    }

    private static AbstractCobolField numeric(int type, int digits, int scale, int size) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(
                        type, digits, scale, CobolFieldAttribute.COB_FLAG_HAVE_SIGN, null);
        return CobolFieldFactory.makeCobolField(
                size, new CobolDataStorage(size), attr);
    }

    @Setup
    public void setup() {
        // 生成コードの run_module と同じモジュールコンテキストを用意する
        // (getString や moveDisplayToBinary が currentModule を参照するため)
        CobolModule.push(
                new CobolModule(null, null, null, null, 0, '.', '$', ',', 1, 1, 1, 0, null));
        alnumSrc100 = alnum(100);
        alnumDst100 = alnum(100);
        alnumDst30 = alnum(30);
        alnumSrc100.moveFrom("HELLO COBOL BENCHMARK DATA");
        disp9 = numeric(CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY, 9, 0, 9);
        packed9 = numeric(CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED, 9, 0, 5);
        binary9 = numeric(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 9, 0, 4);
        disp9.moveFrom(123456789);
    }

    /** MOVE X(100) TO X(100) (先頭バイトを返しデッドコード除去を防ぐ) */
    @Benchmark
    public byte moveAlnum100to100() {
        alnumDst100.moveFrom(alnumSrc100);
        return alnumDst100.getDataStorage().getByte(0);
    }

    /** MOVE X(100) TO X(30) (切り詰めあり) */
    @Benchmark
    public byte moveAlnum100to30() {
        alnumDst30.moveFrom(alnumSrc100);
        return alnumDst30.getDataStorage().getByte(0);
    }

    /** MOVE ゾーン10進 TO パック10進 (型変換MOVE) */
    @Benchmark
    public byte moveDisplayToPacked() {
        packed9.moveFrom(disp9);
        return packed9.getDataStorage().getByte(0);
    }

    /** MOVE ゾーン10進 TO 2進 (型変換MOVE) */
    @Benchmark
    public byte moveDisplayToBinary() {
        binary9.moveFrom(disp9);
        return binary9.getDataStorage().getByte(0);
    }

    /** DISPLAY で呼ばれる数値項目の文字列化 */
    @Benchmark
    public String getStringDisplay() {
        return disp9.getString();
    }

    /** 英数字項目の文字列化 */
    @Benchmark
    public String getStringAlnum() {
        return alnumSrc100.getString();
    }
}

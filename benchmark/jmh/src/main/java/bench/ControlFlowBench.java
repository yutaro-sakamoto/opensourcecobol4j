package bench;

import java.util.Optional;
import java.util.concurrent.TimeUnit;
import jp.osscons.opensourcecobol.libcobj.common.CobolControl;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;
import org.openjdk.jmh.annotations.*;

/**
 * 生成コードの制御フロー機構のオーバーヘッドを計測する。
 *
 * cobj は PERFORM 段落名 を
 *   CobolControl.perform(contList, labelId).run()
 * に変換する。perform() は呼び出しのたびに匿名 CobolControl を new し、
 * 各段落の run() は次の段落を Optional.of(...) で包んで返すため、
 * PERFORM 1回につき複数のオブジェクト割り当てが発生する。
 * 直接メソッド呼び出しした場合と比較する。
 */
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.NANOSECONDS)
@Warmup(iterations = 3, time = 1)
@Measurement(iterations = 5, time = 1)
@Fork(1)
@State(Scope.Thread)
public class ControlFlowBench {

    long counter;
    CobolControl[] contList;

    @Setup
    public void setup() {
        // 生成コードと同じ構造: 段落1つ + 終端 (pure)
        contList =
                new CobolControl[] {
                    new CobolControl(0, CobolControl.LabelType.label) {
                        @Override
                        public Optional<CobolControl> run()
                                throws CobolRuntimeException, CobolStopRunException {
                            counter++;
                            return Optional.of(contList[1]);
                        }
                    },
                    CobolControl.pure(),
                };
    }

    /** 現実装: PERFORM 段落 1回分 (匿名クラス生成 + Optional 連鎖) */
    @Benchmark
    public long performParagraph_current() throws Exception {
        CobolControl.perform(contList, 0).run();
        return counter;
    }

    /** 最適化代替: 直接メソッド呼び出し相当 (戻り値消費の方法も current と揃える) */
    @Benchmark
    public long performParagraph_directCall() {
        paragraph();
        return counter;
    }

    private void paragraph() {
        counter++;
    }
}

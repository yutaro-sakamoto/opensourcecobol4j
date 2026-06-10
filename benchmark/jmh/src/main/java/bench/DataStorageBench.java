package bench;

import java.util.Arrays;
import java.util.concurrent.TimeUnit;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.infra.Blackhole;

/**
 * CobolDataStorage のバイト操作 (memcpy/memset/memcmp) の現実装と、
 * System.arraycopy / Arrays.fill による理論上の最適実装を比較する。
 * MOVE文・初期化・比較のホットパスに相当する。
 */
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.NANOSECONDS)
@Warmup(iterations = 3, time = 1)
@Measurement(iterations = 5, time = 1)
@Fork(1)
@State(Scope.Thread)
public class DataStorageBench {

    @Param({"10", "100", "1000"})
    int size;

    CobolDataStorage src;
    CobolDataStorage dst;
    CobolDataStorage equal; // src と同一内容 (全バイト一致の比較ケース用)
    byte[] srcBytes;
    byte[] dstBytes;
    byte[] equalBytes;

    @Setup
    public void setup() {
        srcBytes = new byte[size];
        dstBytes = new byte[size];
        equalBytes = new byte[size];
        Arrays.fill(srcBytes, (byte) 'A');
        Arrays.fill(equalBytes, (byte) 'A');
        src = new CobolDataStorage(srcBytes);
        dst = new CobolDataStorage(dstBytes);
        equal = new CobolDataStorage(equalBytes);
    }

    /** 現実装: 1バイトずつの setByte/getByte ループ */
    @Benchmark
    public void memcpy_current(Blackhole bh) {
        dst.memcpy(src, size);
        bh.consume(dstBytes);
    }

    /** 最適化代替: System.arraycopy 一発 */
    @Benchmark
    public void memcpy_arraycopy(Blackhole bh) {
        System.arraycopy(srcBytes, 0, dstBytes, 0, size);
        bh.consume(dstBytes);
    }

    /** 現実装: 1バイトずつの setByte ループ (MOVE SPACES 相当) */
    @Benchmark
    public void memset_current(Blackhole bh) {
        dst.memset((byte) ' ', size);
        bh.consume(dstBytes);
    }

    /** 最適化代替: Arrays.fill 一発 */
    @Benchmark
    public void memset_fill(Blackhole bh) {
        Arrays.fill(dstBytes, 0, size, (byte) ' ');
        bh.consume(dstBytes);
    }

    /** 現実装・先頭バイト不一致 (最良ケース: 即リターン) */
    @Benchmark
    public int memcmp_current_mismatch() {
        return dst.memcmp(srcBytes, size);
    }

    /** 現実装・全バイト一致 (最悪ケース: 全バイト走査。等値比較は COBOL で頻出) */
    @Benchmark
    public int memcmp_current_equal() {
        return equal.memcmp(srcBytes, size);
    }

    /** 最適化代替・先頭バイト不一致: Arrays.mismatch ベース */
    @Benchmark
    public int memcmp_mismatch_firstByte() {
        int i = Arrays.mismatch(dstBytes, 0, size, srcBytes, 0, size);
        if (i < 0) {
            return 0;
        }
        return Byte.compareUnsigned(dstBytes[i], srcBytes[i]);
    }

    /** 最適化代替・全バイト一致: Arrays.mismatch ベース */
    @Benchmark
    public int memcmp_mismatch_equal() {
        int i = Arrays.mismatch(equalBytes, 0, size, srcBytes, 0, size);
        if (i < 0) {
            return 0;
        }
        return Byte.compareUnsigned(equalBytes[i], srcBytes[i]);
    }
}

import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolGroupField;

public class TestMoveFromBytes {

    static int testCount = 0;
    static int passCount = 0;

    static CobolGroupField makeGroupField(int size) {
        CobolFieldAttribute attr = new CobolFieldAttribute(
            CobolFieldAttribute.COB_TYPE_GROUP, 0, 0, 0, null);
        CobolDataStorage storage = new CobolDataStorage(size);
        return new CobolGroupField(size, storage, attr);
    }

    static void check(String label, CobolGroupField field, byte[] expected) {
        testCount++;
        byte[] allData = field.getDataStorage().getData(0);
        byte[] actual = java.util.Arrays.copyOf(allData, field.getSize());
        boolean ok = java.util.Arrays.equals(actual, expected);
        if (ok) {
            passCount++;
        }
        System.out.printf("TEST %02d %-40s %s  EXPECT=%s ACTUAL=%s%n",
            testCount, label,
            ok ? "OK" : "NG",
            toDispString(expected),
            toDispString(actual));
    }

    static String toDispString(byte[] bytes) {
        StringBuilder sb = new StringBuilder("(");
        for (byte b : bytes) {
            if (b >= 0x20 && b < 0x7F) {
                sb.append((char) b);
            } else {
                sb.append(String.format("\\x%02X", b & 0xFF));
            }
        }
        sb.append(")");
        return sb.toString();
    }

    public static void main(String[] args) {

        // ========================================
        // Group 1: moveFrom(byte array) directly
        // ========================================

        // Test: bytes.length == field.size (exact fit)
        {
            CobolGroupField f = makeGroupField(5);
            f.moveFrom(new byte[] {'A', 'B', 'C', 'D', 'E'});
            check("byte array exact fit (5==5)",
                f, new byte[] {'A', 'B', 'C', 'D', 'E'});
        }

        // Test: bytes.length > field.size (truncation)
        {
            CobolGroupField f = makeGroupField(3);
            f.moveFrom(new byte[] {'A', 'B', 'C', 'D', 'E'});
            check("byte array longer than field (5>3)",
                f, new byte[] {'A', 'B', 'C'});
        }

        // Test: bytes.length < field.size (padding with 0x20)
        {
            CobolGroupField f = makeGroupField(5);
            f.moveFrom(new byte[] {'A', 'B'});
            check("byte array shorter than field (2<5)",
                f, new byte[] {'A', 'B', ' ', ' ', ' '});
        }

        // Test: bytes.length == 0 (empty -> all spaces)
        {
            CobolGroupField f = makeGroupField(3);
            f.moveFrom(new byte[] {});
            check("byte array empty (0<3)",
                f, new byte[] {' ', ' ', ' '});
        }

        // Test: bytes.length == 1, field.size == 1
        {
            CobolGroupField f = makeGroupField(1);
            f.moveFrom(new byte[] {'X'});
            check("byte array single byte (1==1)",
                f, new byte[] {'X'});
        }

        // Test: bytes.length == 1, field.size > 1
        {
            CobolGroupField f = makeGroupField(5);
            f.moveFrom(new byte[] {'X'});
            check("byte array 1 byte into 5-byte field",
                f, new byte[] {'X', ' ', ' ', ' ', ' '});
        }

        // ========================================
        // Group 2: moveFrom(String) -> moveFrom(byte array)
        // This is the actual path from generated Java
        // (execute method of CALL target program)
        // ========================================

        // Test: String exact fit
        {
            CobolGroupField f = makeGroupField(5);
            f.moveFrom("ABCDE");
            check("String exact fit (5==5)",
                f, new byte[] {'A', 'B', 'C', 'D', 'E'});
        }

        // Test: String shorter than field (padding)
        {
            CobolGroupField f = makeGroupField(5);
            f.moveFrom("AB");
            check("String shorter than field (2<5)",
                f, new byte[] {'A', 'B', ' ', ' ', ' '});
        }

        // Test: String longer than field (truncation)
        {
            CobolGroupField f = makeGroupField(3);
            f.moveFrom("ABCDE");
            check("String longer than field (5>3)",
                f, new byte[] {'A', 'B', 'C'});
        }

        // Test: Empty string
        {
            CobolGroupField f = makeGroupField(3);
            f.moveFrom("");
            check("String empty (0<3)",
                f, new byte[] {' ', ' ', ' '});
        }

        // Test: String with spaces (preserves spaces)
        {
            CobolGroupField f = makeGroupField(5);
            f.moveFrom("A B");
            check("String with spaces 'A B' into 5",
                f, new byte[] {'A', ' ', 'B', ' ', ' '});
        }

        // ========================================
        // Group 3: Overwrite behavior
        // Verify padding overwrites previous data
        // (This is what commit 9ac28a3 fixed)
        // ========================================

        // Test: Short data overwrites previous longer data
        {
            CobolGroupField f = makeGroupField(5);
            f.moveFrom("XXXXX");
            f.moveFrom("AB");
            check("Overwrite: 'XXXXX' then 'AB'",
                f, new byte[] {'A', 'B', ' ', ' ', ' '});
        }

        // Test: Short bytes overwrite previous data
        {
            CobolGroupField f = makeGroupField(10);
            f.moveFrom("ZZZZZZZZZZ");
            f.moveFrom(new byte[] {'H', 'I'});
            check("Overwrite: 10-byte then 2-byte",
                f, new byte[] {'H', 'I', ' ', ' ', ' ',
                               ' ', ' ', ' ', ' ', ' '});
        }

        // Test: Exact fit after short (no stale data)
        {
            CobolGroupField f = makeGroupField(5);
            f.moveFrom("AB");
            f.moveFrom("12345");
            check("Overwrite: 'AB' then '12345'",
                f, new byte[] {'1', '2', '3', '4', '5'});
        }

        // ========================================
        // Summary
        // ========================================
        System.out.println();
        System.out.printf("Result: %d/%d passed%n", passCount, testCount);
        if (passCount != testCount) {
            System.exit(1);
        }
    }
}

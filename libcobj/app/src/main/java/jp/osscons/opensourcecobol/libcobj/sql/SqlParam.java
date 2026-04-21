package jp.osscons.opensourcecobol.libcobj.sql;

import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/** COBOL SQL parameter descriptor holding host variable metadata. */
public class SqlParam {
    /** HVARTYPE constant identifying the COBOL data type. */
    public final int type;

    /** Byte length of the host variable data. */
    public final int length;

    /** Decimal scale (negative means digits after the decimal point). */
    public final int scale;

    /** Raw COBOL data storage backing this parameter. */
    public final CobolDataStorage storage;

    /**
     * Create a new SQL parameter descriptor.
     *
     * @param type COBOL data type constant (see {@link CobolDataConverter})
     * @param length byte length of the host variable
     * @param scale decimal scale
     * @param storage raw COBOL data storage
     */
    public SqlParam(int type, int length, int scale, CobolDataStorage storage) {
        this.type = type;
        this.length = length;
        this.scale = scale;
        this.storage = storage;
    }
}

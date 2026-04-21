package jp.osscons.opensourcecobol.libcobj.sql;

import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

public class SqlParam {
    public final int type;
    public final int length;
    public final int scale;
    public final CobolDataStorage storage;

    public SqlParam(int type, int length, int scale, CobolDataStorage storage) {
        this.type = type;
        this.length = length;
        this.scale = scale;
        this.storage = storage;
    }
}

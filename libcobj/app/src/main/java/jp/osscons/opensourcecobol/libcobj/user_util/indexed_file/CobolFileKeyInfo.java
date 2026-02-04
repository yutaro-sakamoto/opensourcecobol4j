package jp.osscons.opensourcecobol.libcobj.user_util.indexed_file;

/**
 * Represents key information for an indexed file, including the key position, size, and whether
 * duplicate values are allowed. This class is used by the cobj-idx create command to define the
 * key structure of a new indexed file.
 */
class CobolFileKeyInfo {
    /** The 1-based byte offset of the key within the record. */
    public int offset;

    /** The size of the key in bytes. */
    public int size;

    /** Whether duplicate key values are allowed (only valid for alternate keys). */
    public boolean duplicate;

    /**
     * Constructs a new CobolFileKeyInfo with the specified key attributes.
     *
     * @param offset the 1-based byte offset of the key within the record
     * @param size the size of the key in bytes
     * @param duplicate true if duplicate key values are allowed, false otherwise
     */
    public CobolFileKeyInfo(int offset, int size, boolean duplicate) {
        this.offset = offset;
        this.size = size;
        this.duplicate = duplicate;
    }
}

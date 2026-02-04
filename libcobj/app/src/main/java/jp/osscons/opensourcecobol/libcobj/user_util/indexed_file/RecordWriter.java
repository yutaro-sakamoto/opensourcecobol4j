package jp.osscons.opensourcecobol.libcobj.user_util.indexed_file;

/**
 * Interface for writing records to various output destinations in the cobj-idx utility. This
 * interface provides a common abstraction for writing COBOL-style records to output sources.
 */
interface RecordWriter {
    /** Opens the underlying output destination for writing. */
    void open();

    /**
     * Writes a single record to the output destination.
     *
     * @param record the byte array containing the record data to be written
     * @return true if the write operation succeeded, false otherwise
     */
    boolean write(byte[] record);

    /** Closes the underlying output destination and releases associated resources. */
    void close();
}

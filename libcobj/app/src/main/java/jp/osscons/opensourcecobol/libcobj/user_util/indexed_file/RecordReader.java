package jp.osscons.opensourcecobol.libcobj.user_util.indexed_file;

import java.util.Optional;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/**
 * Interface for reading records from various input sources (stdin, files) in the cobj-idx utility.
 * This interface provides a common abstraction for reading COBOL-style records from sequential and
 * line-sequential formatted data sources.
 */
interface RecordReader {
    /** Opens the underlying input source for reading. */
    void open();

    /**
     * Reads a single record from the input source into the specified data storage.
     *
     * @param record the data storage where the read record data will be stored
     * @return the result of the read operation indicating success, end of data, or error
     * @see LoadResult
     */
    LoadResult read(CobolDataStorage record);

    /** Closes the underlying input source and releases associated resources. */
    void close();

    /**
     * Factory method that creates an appropriate RecordReader implementation based on the specified
     * data format and input source.
     *
     * @param userDataFormat the format of the input data (SEQUENTIAL for fixed-length concatenated
     *     records or LINE_SEQUENTIAL for newline-delimited records)
     * @param recordSize the size of each record in bytes
     * @param filePath optional file path; if present, reads from file; if empty, reads from stdin
     * @return a RecordReader implementation suitable for the specified format and source, or null
     *     if the format is not supported
     * @see LoadResult
     * @see UserDataFormat
     */
    static RecordReader getInstance(
            UserDataFormat userDataFormat, int recordSize, Optional<String> filePath) {
        if (filePath.isPresent()) {
            switch (userDataFormat) {
                case LINE_SEQUENTIAL:
                    return new FileLineSeqRecordReader(recordSize, filePath.get());
                case SEQUENTIAL:
                    return new FileSeqRecordReader(recordSize, filePath.get());
                default:
                    return null;
            }
        } else {
            return StdinRecordReader.getInstance(userDataFormat, recordSize);
        }
    }
}

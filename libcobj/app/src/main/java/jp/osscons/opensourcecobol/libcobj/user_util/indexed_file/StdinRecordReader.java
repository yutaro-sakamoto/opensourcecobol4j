package jp.osscons.opensourcecobol.libcobj.user_util.indexed_file;

import java.util.Scanner;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/**
 * A base RecordReader implementation that reads records from standard input. This class provides
 * common functionality for reading COBOL-style records from stdin, with concrete implementations
 * for different data formats (SEQUENTIAL and LINE_SEQUENTIAL).
 */
class StdinRecordReader implements RecordReader {
    /** The expected size of each record in bytes. */
    protected int recordSize;

    /** The scanner used to read input from standard input. */
    protected Scanner scan;

    /**
     * Constructs a new StdinRecordReader with the specified record size.
     *
     * @param recordSize the expected size of each record in bytes
     */
    protected StdinRecordReader(int recordSize) {
        this.recordSize = recordSize;
    }

    @Override
    public void open() {
        this.scan = new Scanner(System.in);
    }

    @Override
    public LoadResult read(CobolDataStorage record) {
        return null;
    }

    @Override
    public void close() {
        this.scan.close();
    }

    /**
     * A StdinRecordReader implementation for reading LINE SEQUENTIAL format data from stdin. Each
     * record is expected to be on a separate line.
     */
    static class StdinLineSeqReader extends StdinRecordReader {
        /**
         * Constructs a new StdinLineSeqReader with the specified record size.
         *
         * @param recordSize the expected size of each record in bytes (excluding newline)
         */
        StdinLineSeqReader(int recordSize) {
            super(recordSize);
        }

        @Override
        public LoadResult read(CobolDataStorage record) {
            if (scan.hasNextLine()) {
                byte[] readData = scan.nextLine().getBytes();
                if (readData.length != this.recordSize) {
                    return LoadResult.LoadResultDataSizeMismatch;
                }
                record.memcpy(readData, this.recordSize);
                return LoadResult.LoadResultSuccess;
            } else {
                return LoadResult.AtEnd;
            }
        }
    }

    /**
     * A StdinRecordReader implementation for reading SEQUENTIAL format data from stdin. Records are
     * concatenated without separators and are split based on the fixed record size.
     */
    static class StdinSeqReader extends StdinRecordReader {
        private boolean firstFetchFail;
        private byte[] readData;
        private int readDataOffset;

        /**
         * Constructs a new StdinSeqReader with the specified record size.
         *
         * @param recordSize the fixed size of each record in bytes
         */
        public StdinSeqReader(int recordSize) {
            super(recordSize);
        }

        @Override
        public void open() {
            super.open();
            this.firstFetchFail = false;
            this.readData = null;
            this.readDataOffset = 0;
        }

        @Override
        public LoadResult read(CobolDataStorage record) {
            if (firstFetchFail) {
                return LoadResult.LoadResultDataSizeMismatch;
            }
            if (readData == null) {
                if (!this.scan.hasNextLine()) {
                    return LoadResult.AtEnd;
                }
                this.readData = this.scan.next().getBytes();
                if (readData.length % this.recordSize != 0) {
                    this.firstFetchFail = true;
                    return LoadResult.LoadResultDataSizeMismatch;
                }
            }
            if (readDataOffset >= readData.length) {
                return LoadResult.AtEnd;
            }
            record.memcpy(this.readData, this.readDataOffset, this.recordSize);
            this.readDataOffset += this.recordSize;
            return LoadResult.LoadResultSuccess;
        }
    }

    /**
     * Factory method that creates an appropriate stdin reader based on the specified data format.
     *
     * @param userDataFormat the format of the input data (SEQUENTIAL or LINE_SEQUENTIAL)
     * @param recordSize the expected size of each record in bytes
     * @return a RecordReader implementation for reading from stdin in the specified format, or null
     *     if the format is not supported
     */
    static RecordReader getInstance(UserDataFormat userDataFormat, int recordSize) {
        switch (userDataFormat) {
            case LINE_SEQUENTIAL:
                {
                    return new StdinLineSeqReader(recordSize);
                }
            case SEQUENTIAL:
                {
                    return new StdinSeqReader(recordSize);
                }
            default:
                return null;
        }
    }
}

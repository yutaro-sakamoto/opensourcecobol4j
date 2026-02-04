package jp.osscons.opensourcecobol.libcobj.user_util.indexed_file;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/**
 * A RecordReader implementation that reads records from a file in COBOL LINE SEQUENTIAL format. In
 * this format, each record is stored on a separate line, terminated by a newline character.
 */
class FileLineSeqRecordReader implements RecordReader {
    /** The expected size of each record in bytes. */
    int recordSize;

    /** The buffered reader used to read lines from the file. */
    BufferedReader reader;

    private String filePath;

    /**
     * Constructs a new FileLineSeqRecordReader for reading line-sequential records from a file.
     *
     * @param recordSize the expected size of each record in bytes (excluding newline)
     * @param filePath the path to the file to read from
     */
    FileLineSeqRecordReader(int recordSize, String filePath) {
        this.recordSize = recordSize;
        this.filePath = filePath;
    }

    @Override
    public void open() {
        try {
            this.reader = new BufferedReader(new FileReader(this.filePath));
        } catch (IOException e) {
            this.reader = null;
        }
    }

    @Override
    public LoadResult read(CobolDataStorage record) {
        if (this.reader == null) {
            return LoadResult.LoadResultOther;
        }
        try {
            String line = this.reader.readLine();
            if (line == null) {
                return LoadResult.AtEnd;
            }
            byte[] readData = line.getBytes();
            if (readData.length != this.recordSize) {
                return LoadResult.LoadResultDataSizeMismatch;
            }
            record.memcpy(readData, this.recordSize);
            return LoadResult.LoadResultSuccess;
        } catch (IOException e) {
            return LoadResult.LoadResultOther;
        }
    }

    @Override
    public void close() {
        try {
            this.reader.close();
        } catch (IOException e) {
            return;
        }
    }
}

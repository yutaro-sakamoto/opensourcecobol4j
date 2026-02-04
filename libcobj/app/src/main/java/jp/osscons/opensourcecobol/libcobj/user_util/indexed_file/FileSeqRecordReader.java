package jp.osscons.opensourcecobol.libcobj.user_util.indexed_file;

import java.io.FileInputStream;
import java.io.IOException;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/**
 * A RecordReader implementation that reads records from a file in COBOL SEQUENTIAL format. In this
 * format, records are concatenated without any separator, and each record has a fixed size.
 */
public class FileSeqRecordReader implements RecordReader {
    private FileInputStream reader;
    private String filePath;
    private byte[] readData;

    /**
     * Constructs a new FileSeqRecordReader for reading sequential records from a file.
     *
     * @param recordSize the fixed size of each record in bytes
     * @param filePath the path to the file to read from
     */
    FileSeqRecordReader(int recordSize, String filePath) {
        this.filePath = filePath;
        this.readData = new byte[recordSize];
    }

    @Override
    public void open() {
        try {
            this.reader = new FileInputStream(this.filePath);
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
            int readSize = this.reader.read(this.readData);
            if (readSize == -1) {
                return LoadResult.AtEnd;
            }
            if (readSize != this.readData.length) {
                if (readSize == 1 && this.readData[0] == '\n') {
                    return LoadResult.AtEnd;
                } else if (readSize == 2 && this.readData[0] == '\r' && this.readData[1] == '\n') {
                    return LoadResult.AtEnd;
                } else {
                    return LoadResult.LoadResultDataSizeMismatch;
                }
            }
            record.memcpy(this.readData, this.readData.length);
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

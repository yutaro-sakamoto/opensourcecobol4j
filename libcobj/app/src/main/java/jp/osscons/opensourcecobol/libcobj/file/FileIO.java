/*
 * Copyright (C) 2021-2022 TOKYO SYSTEM HOUSE Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3.0,
 * or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see the file COPYING.LIB.  If
 * not, write to the Free Software Foundation, 51 Franklin Street, Fifth Floor
 * Boston, MA 02110-1301 USA
 */
package jp.osscons.opensourcecobol.libcobj.file;

import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.channels.NonReadableChannelException;
import java.nio.channels.NonWritableChannelException;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/** TODO: 準備中 */
class FileIO {

    private FileChannel fc;
    private FileLock fl = null;
    private boolean useStdOut = true;
    private boolean useStdIn = true;
    private boolean atEnd = false;

    private static final int BUFFER_MODE_NONE = 0;
    private static final int BUFFER_MODE_READ = 1;
    private static final int BUFFER_MODE_WRITE = 2;

    private int bufferMode = BUFFER_MODE_NONE;
    private int bufferSize = 0;
    private byte[] buffer;
    private int bufferDataStart = 0;
    private int bufferDataEnd = 0;

    /** TODO: 準備中 */
    FileIO() {
        this.useStdOut = true;
        this.useStdIn = true;
    }

    /**
     * TODO: 準備中
     *
     * @return TODO: 準備中
     */
    boolean isAtEnd() {
        return this.atEnd;
    }

    /**
     * TODO: 準備中
     *
     * @param fc TODO: 準備中
     * @param fl TODO: 準備中
     */
    void setChannel(FileChannel fc, FileLock fl) {
        this.fc = fc;
        this.fl = fl;
        this.useStdOut = false;
        this.useStdIn = false;
    }

    /**
     * TODO: 準備中
     *
     * @param ra TODO: 準備中
     * @param fl TODO: 準備中
     */
    void setRandomAccessFile(RandomAccessFile ra, FileLock fl) {
        this.useStdOut = false;
        this.useStdIn = false;
        this.fc = ra.getChannel();
        this.fl = fl;
    }

    /**
     * TODO: 準備中
     *
     * @param out TODO: 準備中
     */
    void setOut(PrintStream out) {
        this.useStdOut = true;
    }

    /**
     * TODO: 準備中
     *
     * @param in TODO: 準備中
     */
    void setIn(InputStream in) {
        this.useStdIn = true;
    }

    /**
     * TODO: 準備中
     *
     * @param bufferSize TODO: 準備中
     */
    void prepareBuffer(int bufferSize) {
        if (bufferSize > 0) {
            this.bufferSize = bufferSize;
            this.bufferMode = BUFFER_MODE_NONE;
            this.bufferDataStart = 0;
            this.bufferDataEnd = 0;
            if (this.buffer == null || this.buffer.length < bufferSize) {
                this.buffer = new byte[bufferSize];
            }
        }
    }

    private void destroyBuffer() {
        this.bufferSize = 0;
        this.bufferMode = BUFFER_MODE_NONE;
        this.bufferDataStart = 0;
        this.bufferDataEnd = 0;
    }

    private boolean flushWriteBuffer() {
        if (bufferMode == BUFFER_MODE_WRITE && bufferDataEnd > 0 && bufferSize > 0) {
            ByteBuffer bb = ByteBuffer.wrap(buffer, 0, bufferDataEnd);
            if (!writeByteBuffer(bb)) {
                return false;
            }
            bufferDataEnd = 0;
        }
        return true;
    }

    private int fillReadBuffer() throws IOException {
        bufferDataStart = 0;
        bufferDataEnd = 0;
        ByteBuffer bb = ByteBuffer.wrap(buffer, 0, bufferSize);
        int readBytes;
        try {
            readBytes = this.fc.read(bb);
        } catch (NonReadableChannelException e) {
            return -1;
        }
        if (readBytes <= 0) {
            return -1;
        }
        bufferDataEnd = readBytes;
        return readBytes;
    }

    private void transitionToRead() throws IOException {
        if (bufferMode == BUFFER_MODE_WRITE) {
            if (!flushWriteBuffer()) {
                throw new IOException("Failed to flush write buffer");
            }
        }
        if (bufferMode != BUFFER_MODE_READ) {
            bufferMode = BUFFER_MODE_READ;
            bufferDataStart = 0;
            bufferDataEnd = 0;
        }
    }

    private void transitionToWrite() throws IOException {
        if (bufferMode == BUFFER_MODE_READ) {
            int unread = bufferDataEnd - bufferDataStart;
            if (unread > 0) {
                this.fc.position(this.fc.position() - unread);
            }
        }
        if (bufferMode != BUFFER_MODE_WRITE) {
            bufferMode = BUFFER_MODE_WRITE;
            bufferDataStart = 0;
            bufferDataEnd = 0;
        }
    }

    /**
     * TODO: 準備中
     *
     * @param bytes TODO: 準備中
     * @param size TODO: 準備中
     * @return TODO: 準備中
     */
    int read(byte[] bytes, int size) {
        if (useStdIn) {
            // 標準入力を使う
            System.err.println("read stdin not implmented");
            return 0;
        } else {
            if (bufferSize > 0) {
                try {
                    transitionToRead();
                    int offset = 0;
                    int remaining = size;
                    while (remaining > 0) {
                        int available = bufferDataEnd - bufferDataStart;
                        if (available <= 0) {
                            if (fillReadBuffer() < 0) {
                                this.atEnd = true;
                                if (offset == 0) {
                                    return 0;
                                }
                                return 1;
                            }
                            available = bufferDataEnd - bufferDataStart;
                        }
                        int toCopy = Math.min(remaining, available);
                        System.arraycopy(buffer, bufferDataStart, bytes, offset, toCopy);
                        bufferDataStart += toCopy;
                        offset += toCopy;
                        remaining -= toCopy;
                    }
                } catch (IOException e) {
                    return 0;
                }
                return 1;
            }

            int readSize;
            ByteBuffer data = ByteBuffer.wrap(bytes);
            try {
                readSize = this.fc.read(data);
            } catch (IOException | NonReadableChannelException e) {
                return 0;
            }

            this.atEnd = readSize == -1;
            if (readSize == -1) {
                return 0;
            }
        }
        return 1;
    }

    /**
     * TODO: 準備中
     *
     * @param storage TODO: 準備中
     * @param size TODO: 準備中
     * @return TODO: 準備中
     * @throws IOException TODO: 準備中
     */
    int read(CobolDataStorage storage, int size) throws IOException {
        if (useStdIn) {
            return 0;
        } else {
            if (this.fc == null) {
                throw new IOException();
            }
            if (bufferSize > 0) {
                transitionToRead();
                int offset = 0;
                int remaining = size;
                while (remaining > 0) {
                    int available = bufferDataEnd - bufferDataStart;
                    if (available <= 0) {
                        if (fillReadBuffer() < 0) {
                            return offset;
                        }
                        available = bufferDataEnd - bufferDataStart;
                    }
                    int toCopy = Math.min(remaining, available);
                    for (int i = 0; i < toCopy; ++i) {
                        storage.setByte(offset + i, buffer[bufferDataStart + i]);
                    }
                    bufferDataStart += toCopy;
                    offset += toCopy;
                    remaining -= toCopy;
                }
                return size;
            }

            int i = 0;
            try {
                for (i = 0; i < size; ++i) {
                    byte[] b = new byte[1];
                    ByteBuffer bb = ByteBuffer.wrap(b);
                    if (this.fc.read(bb) != 1) {
                        return i;
                    }
                    storage.setByte(i, b[0]);
                }
            } catch (IOException | NonReadableChannelException e) {
                throw e;
            }
            return size;
        }
    }

    private boolean writeByteBuffer(ByteBuffer bb) {
        try {
            this.fc.write(bb);
        } catch (IOException | NonWritableChannelException e) {
            return false;
        }
        return true;
    }

    /**
     * TODO: 準備中
     *
     * @param bytes TODO: 準備中
     * @param size TODO: 準備中
     * @return TODO: 準備中
     */
    boolean write(byte[] bytes, int size) {
        if (this.fc == null) {
            return false;
        }
        if (bufferSize > 0) {
            try {
                transitionToWrite();
            } catch (IOException e) {
                return false;
            }
            if (size <= bufferSize - bufferDataEnd) {
                System.arraycopy(bytes, 0, buffer, bufferDataEnd, size);
                bufferDataEnd += size;
                return true;
            }
            if (!flushWriteBuffer()) {
                return false;
            }
            if (size <= bufferSize - bufferDataEnd) {
                System.arraycopy(bytes, 0, buffer, bufferDataEnd, size);
                bufferDataEnd += size;
                return true;
            }
            ByteBuffer bb = ByteBuffer.wrap(bytes, 0, size);
            return writeByteBuffer(bb);
        }
        ByteBuffer bb = ByteBuffer.wrap(bytes, 0, size);
        return writeByteBuffer(bb);
    }

    /**
     * TODO: 準備中
     *
     * @param storage TODO: 準備中
     * @param size TODO: 準備中
     * @return TODO: 準備中
     */
    boolean write(CobolDataStorage storage, int size) {
        if (this.fc == null) {
            return false;
        }
        if (bufferSize > 0) {
            try {
                transitionToWrite();
            } catch (IOException e) {
                return false;
            }
            if (size <= bufferSize - bufferDataEnd) {
                for (int i = 0; i < size; ++i) {
                    buffer[bufferDataEnd + i] = storage.getByte(i);
                }
                bufferDataEnd += size;
                return true;
            }
            if (!flushWriteBuffer()) {
                return false;
            }
            if (size <= bufferSize - bufferDataEnd) {
                for (int i = 0; i < size; ++i) {
                    buffer[bufferDataEnd + i] = storage.getByte(i);
                }
                bufferDataEnd += size;
                return true;
            }
            ByteBuffer bb = storage.getByteBuffer(size);
            return writeByteBuffer(bb);
        }
        ByteBuffer bb = storage.getByteBuffer(size);
        return writeByteBuffer(bb);
    }

    /**
     * TODO: 準備中
     *
     * @param val TODO: 準備中
     * @return TODO: 準備中
     */
    byte putc(byte val) {
        if (this.fc == null) {
            return 0;
        }
        if (bufferSize > 0) {
            try {
                transitionToWrite();
            } catch (IOException e) {
                return -1;
            }
            if (1 <= bufferSize - bufferDataEnd) {
                buffer[bufferDataEnd++] = val;
                return val;
            }
            if (!flushWriteBuffer()) {
                return -1;
            }
            if (1 <= bufferSize - bufferDataEnd) {
                buffer[bufferDataEnd++] = val;
                return val;
            }
        }
        byte[] arr = {val};
        if (writeByteBuffer(ByteBuffer.wrap(arr))) {
            return val;
        } else {
            return -1;
        }
    }

    /**
     * TODO: 準備中
     *
     * @return TODO: 準備中
     */
    int getc() {
        if (this.fc == null) {
            return 0;
        }
        if (bufferSize > 0) {
            try {
                transitionToRead();
                if (bufferDataStart >= bufferDataEnd) {
                    if (fillReadBuffer() < 0) {
                        return -1;
                    }
                }
                return buffer[bufferDataStart++] & 0xFF;
            } catch (IOException e) {
                return -1;
            }
        }
        try {
            byte[] b = new byte[1];
            ByteBuffer bb = ByteBuffer.wrap(b);
            if (this.fc.read(bb) == 1) {
                return b[0] & 0xFF;
            } else {
                return -1;
            }
        } catch (IOException | NonReadableChannelException e) {
            return -1;
        }
    }

    /** TODO: 準備中 */
    void close() {
        if (!useStdOut && !useStdIn && this.fc != null) {
            try {
                if (bufferMode == BUFFER_MODE_WRITE) {
                    flushWriteBuffer();
                }
                destroyBuffer();
                this.fc.close();
            } catch (IOException e) {
                return;
            }
        }
    }

    /** TODO: 準備中 */
    void flush() {
        if (!useStdOut) {
            try {
                if (bufferMode == BUFFER_MODE_WRITE) {
                    flushWriteBuffer();
                }
                this.fc.force(false);
            } catch (IOException e) {
                return;
            }
        }
    }

    /** TODO: 準備中 */
    static final int SEEK_SET = 0;

    /** TODO: 準備中 */
    static final int SEEK_CUR = 1;

    /**
     * TODO: 準備中
     *
     * @param offset TODO: 準備中
     * @param origin TODO: 準備中
     * @return TODO: 準備中
     */
    boolean seek(long offset, int origin) {
        if (!useStdOut && !useStdIn) {
            try {
                if (bufferMode == BUFFER_MODE_WRITE) {
                    flushWriteBuffer();
                }
                switch (origin) {
                    case FileIO.SEEK_SET:
                        this.fc.position(offset);
                        break;
                    case FileIO.SEEK_CUR:
                        long adjustment = 0;
                        if (bufferMode == BUFFER_MODE_READ) {
                            adjustment = bufferDataEnd - bufferDataStart;
                        }
                        this.fc.position(this.fc.position() + offset - adjustment);
                        break;
                    default:
                        return false;
                }
                bufferMode = BUFFER_MODE_NONE;
                bufferDataStart = 0;
                bufferDataEnd = 0;
            } catch (IOException e) {
                return false;
            }
        }
        return true;
    }

    /** TODO: 準備中 */
    void seekInit() {}

    /** TODO: 準備中 */
    void rewind() {
        if (!useStdOut && !useStdIn) {
            try {
                if (bufferMode == BUFFER_MODE_WRITE) {
                    flushWriteBuffer();
                }
                bufferMode = BUFFER_MODE_NONE;
                bufferDataStart = 0;
                bufferDataEnd = 0;
                this.fc.position(0L);
            } catch (IOException e) {
                return;
            }
        }
    }

    /** TODO: 準備中 */
    void releaseLock() {
        if ((!useStdOut || !useStdIn) && this.fl != null) {
            try {
                this.fl.release();
            } catch (IOException e) {
                return;
            }
        }
    }
}

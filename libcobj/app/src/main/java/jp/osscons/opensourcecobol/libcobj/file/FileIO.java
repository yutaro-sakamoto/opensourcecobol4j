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

/**
 * 低レベルファイルI/O操作を提供するクラス。
 *
 * <p>java.nio.channels.FileChannelのラッパーとして、COBOLファイル操作に必要な 読み書き、シーク、ファイルロック機能を提供する。
 * SEQUENTIAL、LINE SEQUENTIAL、RELATIVEファイルの実装で使用される。
 *
 * <p>標準入出力（stdin/stdout）への読み書きもサポートし、 バッファリングによるパフォーマンス最適化機能も備える。
 */
class FileIO {

    /** ファイルチャネル */
    private FileChannel fc;

    /** ファイルロック */
    private FileLock fl = null;

    /** 標準出力を使用するかどうか */
    private boolean useStdOut = true;

    /** 標準入力を使用するかどうか */
    private boolean useStdIn = true;

    /** ファイル終端に達したかどうか */
    private boolean atEnd = false;

    /** 読み取りバッファを使用するかどうか */
    private static final boolean USE_READ_BUFFER = false;

    /** 読み取りバッファのサイズ */
    private static final int READ_BUFFER_SIZE = 1024;

    /** 読み取りバッファ内の現在位置 */
    private int readBufferIndex;

    /** 読み取りバッファ */
    private byte[] readBuffer;

    /** 読み取りバッファ内の有効データの終端位置 */
    private int readBufferEndIndex;

    /** 書き込みバッファのサイズ */
    private int writeBufferSize = 0;

    /** 書き込みバッファ内の有効データの終端位置 */
    private int writeBufferEndIndex = 0;

    /** 書き込みバッファ */
    private byte[] writeBuffer;

    /** デフォルトコンストラクタ。標準入出力を使用する設定で初期化する。 */
    FileIO() {
        this.useStdOut = true;
        this.useStdIn = true;

        this.readBufferIndex = READ_BUFFER_SIZE;
        this.readBuffer = new byte[READ_BUFFER_SIZE];
        this.readBufferEndIndex = READ_BUFFER_SIZE;
    }

    /**
     * ファイル終端に達したかどうかを判定する。
     *
     * @return ファイル終端の場合true
     */
    boolean isAtEnd() {
        return this.atEnd;
    }

    /**
     * ファイルチャネルとファイルロックを設定する。
     *
     * @param fc 設定するファイルチャネル
     * @param fl 設定するファイルロック（ロック不要の場合はnull）
     */
    void setChannel(FileChannel fc, FileLock fl) {
        this.fc = fc;
        this.fl = fl;
        this.useStdOut = false;
        this.useStdIn = false;
    }

    /**
     * RandomAccessFileからファイルチャネルを設定する。
     *
     * @param ra 設定するRandomAccessFile
     * @param fl 設定するファイルロック（ロック不要の場合はnull）
     */
    void setRandomAccessFile(RandomAccessFile ra, FileLock fl) {
        this.useStdOut = false;
        this.useStdIn = false;
        this.fc = ra.getChannel();
        this.fl = fl;
    }

    /**
     * 標準出力を使用するように設定する。
     *
     * @param out 出力先（現在は使用されていない）
     */
    void setOut(PrintStream out) {
        this.useStdOut = true;
    }

    /**
     * 標準入力を使用するように設定する。
     *
     * @param in 入力元（現在は使用されていない）
     */
    void setIn(InputStream in) {
        this.useStdIn = true;
    }

    /**
     * 書き込みバッファを準備する。
     *
     * @param bufferSize バッファサイズ（バイト数）。0以下の場合はバッファリングしない
     */
    void prepareWriteBuffer(int bufferSize) {
        if (bufferSize > 0) {
            this.writeBufferSize = bufferSize;
            this.writeBufferEndIndex = 0;
            if (this.writeBuffer == null || this.writeBuffer.length < bufferSize) {
                this.writeBuffer = new byte[bufferSize];
            }
        }
    }

    private void destroyWriteBuffer() {
        this.writeBufferSize = 0;
        this.writeBufferEndIndex = 0;
    }

    /**
     * 指定されたバイト配列にデータを読み込む。
     *
     * @param bytes 読み込み先のバイト配列
     * @param size 読み込むバイト数
     * @return 成功時は1、失敗時は0
     */
    int read(byte[] bytes, int size) {
        if (useStdIn) {
            // 標準入力を使う
            System.err.println("read stdin not implmented");
            return 0;
        } else {
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
     * CobolDataStorageにデータを読み込む。
     *
     * @param storage 読み込み先のストレージ
     * @param size 読み込むバイト数
     * @return 実際に読み込んだバイト数
     * @throws IOException 読み込みエラー発生時
     */
    int read(CobolDataStorage storage, int size) throws IOException {
        if (useStdIn) {
            return 0;
        } else {
            if (this.fc == null) {
                throw new IOException();
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

    private boolean outputWriteBuffer() {
        if (writeBufferEndIndex > 0 && writeBufferSize > 0) {
            ByteBuffer bb = ByteBuffer.wrap(writeBuffer, 0, writeBufferEndIndex);
            if (!writeByteBuffer(bb)) {
                return false;
            }
            writeBufferEndIndex = 0;
        }
        return true;
    }

    /**
     * バイト配列をファイルに書き込む。
     *
     * @param bytes 書き込むデータ
     * @param size 書き込むバイト数
     * @return 成功時はtrue、失敗時はfalse
     */
    boolean write(byte[] bytes, int size) {
        if (this.fc == null) {
            return false;
        }
        if (writeBufferSize > 0 && size <= writeBufferSize - writeBufferEndIndex) {
            System.arraycopy(bytes, 0, writeBuffer, writeBufferEndIndex, size);
            writeBufferEndIndex += size;
            return true;
        }
        if (!outputWriteBuffer()) {
            return false;
        }
        if (writeBufferSize > 0 && size <= writeBufferSize - writeBufferEndIndex) {
            System.arraycopy(bytes, 0, writeBuffer, writeBufferEndIndex, size);
            writeBufferEndIndex += size;
            return true;
        }
        ByteBuffer bb = ByteBuffer.wrap(bytes, 0, size);
        return writeByteBuffer(bb);
    }

    /**
     * CobolDataStorageの内容をファイルに書き込む。
     *
     * @param storage 書き込むデータを保持するストレージ
     * @param size 書き込むバイト数
     * @return 成功時はtrue、失敗時はfalse
     */
    boolean write(CobolDataStorage storage, int size) {
        if (this.fc == null) {
            return false;
        }
        if (writeBufferSize > 0 && size <= writeBufferSize - writeBufferEndIndex) {
            for (int i = 0; i < size; ++i) {
                writeBuffer[writeBufferEndIndex + i] = storage.getByte(i);
            }
            writeBufferEndIndex += size;
            return true;
        }
        if (!outputWriteBuffer()) {
            return false;
        }
        if (writeBufferSize > 0 && size <= writeBufferSize - writeBufferEndIndex) {
            for (int i = 0; i < size; ++i) {
                writeBuffer[writeBufferEndIndex + i] = storage.getByte(i);
            }
            writeBufferEndIndex += size;
            return true;
        }
        ByteBuffer bb = storage.getByteBuffer(size);
        return writeByteBuffer(bb);
    }

    /**
     * 1バイトをファイルに書き込む。
     *
     * @param val 書き込むバイト値
     * @return 成功時は書き込んだ値、失敗時は-1
     */
    byte putc(byte val) {
        if (this.fc == null) {
            return 0;
        }
        if (writeBufferSize > 0 && 1 <= writeBufferSize - writeBufferEndIndex) {
            writeBuffer[writeBufferEndIndex++] = val;
            return val;
        }
        if (!outputWriteBuffer()) {
            return -1;
        }
        if (writeBufferSize > 0 && 1 <= writeBufferSize - writeBufferEndIndex) {
            writeBuffer[writeBufferEndIndex++] = val;
            return val;
        }
        byte[] arr = {val};
        if (writeByteBuffer(ByteBuffer.wrap(arr))) {
            return val;
        } else {
            return -1;
        }
    }

    /**
     * ファイルから1バイトを読み込む。
     *
     * @return 読み込んだバイト値。ファイル終端またはエラー時は-1
     */
    int getc() {
        if (this.fc == null) {
            return 0;
        }
        if (USE_READ_BUFFER) {
            if (readBufferIndex >= READ_BUFFER_SIZE) {
                this.readBufferIndex = 0;
                try {
                    ByteBuffer bb = ByteBuffer.wrap(readBuffer);
                    int readBytes = this.fc.read(bb);
                    if (readBytes <= 0) {
                        this.readBufferEndIndex = -1;
                    } else {
                        this.readBufferEndIndex = readBytes;
                    }
                } catch (IOException | NonReadableChannelException e) {
                    return -1;
                }
            }

            if (this.readBufferIndex >= this.readBufferEndIndex) {
                return -1;
            }

            return readBuffer[readBufferIndex++];

        } else {
            try {
                byte[] b = new byte[1];
                ByteBuffer bb = ByteBuffer.wrap(b);
                if (this.fc.read(bb) == 1) {
                    return b[0];
                } else {
                    return -1;
                }
            } catch (IOException | NonReadableChannelException e) {
                return -1;
            }
        }
    }

    /** ファイルを閉じる。バッファのフラッシュとファイルチャネルのクローズを行う。 */
    void close() {
        if (!useStdOut && !useStdIn && this.fc != null) {
            try {
                outputWriteBuffer();
                destroyWriteBuffer();
                this.fc.close();
            } catch (IOException e) {
                return;
            }
        }
    }

    /** バッファをフラッシュしてファイルに強制書き込みする。 */
    void flush() {
        if (!useStdOut) {
            try {
                outputWriteBuffer();
                this.fc.force(false);
            } catch (IOException e) {
                return;
            }
        }
    }

    /** シーク基準点: ファイル先頭からの絶対位置 */
    static final int SEEK_SET = 0;

    /** シーク基準点: 現在位置からの相対位置 */
    static final int SEEK_CUR = 1;

    /**
     * ファイル位置を移動する。
     *
     * @param offset 移動量（バイト数）
     * @param origin 基準点（SEEK_SET=先頭から、SEEK_CUR=現在位置から）
     * @return 成功時はtrue、失敗時はfalse
     */
    boolean seek(long offset, int origin) {
        if (!useStdOut && !useStdIn) {
            try {
                switch (origin) {
                    case FileIO.SEEK_SET:
                        this.fc.position(offset);
                        break;
                    case FileIO.SEEK_CUR:
                        this.fc.position(this.fc.position() + offset);
                        break;
                    default:
                        return false;
                }
            } catch (IOException e) {
                return false;
            }
        }
        return true;
    }

    /** シーク操作の初期化（現在は何もしない）。 */
    void seekInit() {}

    /** ファイル位置を先頭に戻す。 */
    void rewind() {
        if (!useStdOut && !useStdIn) {
            try {
                this.fc.position(0L);
            } catch (IOException e) {
                return;
            }
        }
    }

    /** ファイルロックを解放する。 */
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

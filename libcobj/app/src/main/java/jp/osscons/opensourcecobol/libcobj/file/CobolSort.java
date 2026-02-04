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

import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/**
 * COBOL SORT/MERGE文の処理を管理するクラス。
 *
 * <p>COBOL SORT文およびMERGE文の実行時に使用され、ソート対象レコードの管理、 一時ファイルの管理、マージソートのフェーズ制御などを行う。
 * メモリ内ソートと外部マージソートの両方をサポートし、 大量データの効率的なソートを実現する。
 *
 * <p>libcob/fileio.cのstruct cobsortに対応する。
 */
class CobolSort {
    /** ソート対象のCobolFileへの参照 */
    private CobolFile pointer;

    /** 再利用可能なCobolItemの空きリストの先頭 */
    private CobolItem empty;

    /** SORT-RETURNレジスタの値を保持するストレージ */
    private CobolDataStorage sortReturn;

    /** ファイルステータスフィールドへの参照 */
    private AbstractCobolField fnstatus;

    /** 安定ソートのための一意識別子カウンタ */
    private int unique;

    /** レコード取り出しフェーズかどうか。1の場合は取り出し中 */
    private int retrieving;

    /** 一時ファイルが使用されているかどうか。1の場合は使用中 */
    private int filesUsed;

    /** ソート対象レコードのサイズ（バイト数） */
    private int size;

    /** 読み取り用のレコードサイズ（サイズ情報を含む） */
    private int rSize;

    /** 書き込み用のレコードサイズ（ブロック区切りを含む） */
    private int wSize;

    /** メモリ内に保持可能なレコード数の上限 */
    private int memory;

    /** 外部マージソートで次の書き込み先ファイルのインデックス */
    private int destinationFile;

    /** 結果取り出し時に使用するキューのインデックス */
    private int retrievalQueue;

    /** マージソート用のキュー配列（4つのキューを使用） */
    private MemoryStruct[] queue = new MemoryStruct[4];

    /** 外部マージソート用の一時ファイル配列（4つのファイルを使用） */
    private FileStruct[] file = new FileStruct[4];

    /** デフォルトコンストラクタ。4つのキューと4つのファイル構造を初期化する。 */
    CobolSort() {
        for (int i = 0; i < 4; ++i) {
            this.queue[i] = new MemoryStruct();
            this.file[i] = new FileStruct();
        }
    }

    /**
     * ソート対象のCobolFileを取得する。
     *
     * @return ソート対象のCobolFile
     */
    CobolFile getPointer() {
        return pointer;
    }

    /**
     * ソート対象のCobolFileを設定する。
     *
     * @param pointer 設定するCobolFile
     */
    void setPointer(CobolFile pointer) {
        this.pointer = pointer;
    }

    /**
     * 再利用可能な空きCobolItemリストの先頭を取得する。
     *
     * @return 空きリストの先頭要素。リストが空の場合はnull
     */
    CobolItem getEmpty() {
        return empty;
    }

    /**
     * 再利用可能な空きCobolItemリストの先頭を設定する。
     *
     * @param empty 設定する空きリストの先頭要素
     */
    void setEmpty(CobolItem empty) {
        this.empty = empty;
    }

    /**
     * SORT-RETURNレジスタのストレージを取得する。
     *
     * @return SORT-RETURNレジスタのストレージ
     */
    CobolDataStorage getSortReturn() {
        return sortReturn;
    }

    /**
     * SORT-RETURNレジスタのストレージを設定する。
     *
     * @param sortReturn 設定するストレージ
     */
    void setSortReturn(CobolDataStorage sortReturn) {
        this.sortReturn = sortReturn;
    }

    /**
     * ファイルステータスフィールドを取得する。
     *
     * @return ファイルステータスフィールド
     */
    AbstractCobolField getFnstatus() {
        return fnstatus;
    }

    /**
     * ファイルステータスフィールドを設定する。
     *
     * @param fnstatus 設定するファイルステータスフィールド
     */
    void setFnstatus(AbstractCobolField fnstatus) {
        this.fnstatus = fnstatus;
    }

    /**
     * 一意識別子カウンタの現在値を取得する。
     *
     * @return 一意識別子カウンタの値
     */
    int getUnique() {
        return unique;
    }

    /**
     * 一意識別子カウンタの値を設定する。
     *
     * @param unique 設定する値
     */
    void setUnique(int unique) {
        this.unique = unique;
    }

    /**
     * レコード取り出しフェーズかどうかを取得する。
     *
     * @return 取り出しフェーズの場合は1、それ以外は0
     */
    int getRetrieving() {
        return retrieving;
    }

    /**
     * レコード取り出しフェーズのフラグを設定する。
     *
     * @param retrieving 設定するフラグ値
     */
    void setRetrieving(int retrieving) {
        this.retrieving = retrieving;
    }

    /**
     * 一時ファイルが使用されているかどうかを取得する。
     *
     * @return 使用中の場合は1、それ以外は0
     */
    int getFilesUsed() {
        return filesUsed;
    }

    /**
     * 一時ファイル使用フラグを設定する。
     *
     * @param filesUsed 設定するフラグ値
     */
    void setFilesUsed(int filesUsed) {
        this.filesUsed = filesUsed;
    }

    /**
     * レコードサイズを取得する。
     *
     * @return レコードサイズ（バイト数）
     */
    int getSize() {
        return size;
    }

    /**
     * レコードサイズを設定する。
     *
     * @param size 設定するレコードサイズ（バイト数）
     */
    void setSize(int size) {
        this.size = size;
    }

    /**
     * 読み取り用レコードサイズを取得する。
     *
     * @return 読み取り用レコードサイズ（バイト数）
     */
    int getrSize() {
        return rSize;
    }

    /**
     * 読み取り用レコードサイズを設定する。
     *
     * @param rSize 設定する読み取り用レコードサイズ（バイト数）
     */
    void setrSize(int rSize) {
        this.rSize = rSize;
    }

    /**
     * 書き込み用レコードサイズを取得する。
     *
     * @return 書き込み用レコードサイズ（バイト数）
     */
    int getwSize() {
        return wSize;
    }

    /**
     * 書き込み用レコードサイズを設定する。
     *
     * @param wSize 設定する書き込み用レコードサイズ（バイト数）
     */
    void setwSize(int wSize) {
        this.wSize = wSize;
    }

    /**
     * メモリ内に保持可能なレコード数の上限を取得する。
     *
     * @return レコード数の上限
     */
    int getMemory() {
        return memory;
    }

    /**
     * メモリ内に保持可能なレコード数の上限を設定する。
     *
     * @param memory 設定するレコード数の上限
     */
    void setMemory(int memory) {
        this.memory = memory;
    }

    /**
     * 次の書き込み先ファイルのインデックスを取得する。
     *
     * @return ファイルインデックス（0-3）
     */
    int getDestinationFile() {
        return destinationFile;
    }

    /**
     * 次の書き込み先ファイルのインデックスを設定する。
     *
     * @param destinationFile 設定するファイルインデックス（0-3）
     */
    void setDestinationFile(int destinationFile) {
        this.destinationFile = destinationFile;
    }

    /**
     * 結果取り出し時に使用するキューのインデックスを取得する。
     *
     * @return キューインデックス（0-3）
     */
    int getRetrievalQueue() {
        return retrievalQueue;
    }

    /**
     * 結果取り出し時に使用するキューのインデックスを設定する。
     *
     * @param retrievalQueue 設定するキューインデックス（0-3）
     */
    void setRetrievalQueue(int retrievalQueue) {
        this.retrievalQueue = retrievalQueue;
    }

    /**
     * マージソート用のキュー配列を取得する。
     *
     * @return キュー配列（4要素）
     */
    MemoryStruct[] getQueue() {
        return queue;
    }

    /**
     * マージソート用のキュー配列を設定する。
     *
     * @param queue 設定するキュー配列
     */
    void setQueue(MemoryStruct[] queue) {
        this.queue = queue;
    }

    /**
     * 一時ファイル配列を取得する。
     *
     * @return 一時ファイル配列（4要素）
     */
    FileStruct[] getFile() {
        return file;
    }

    /**
     * 一時ファイル配列を設定する。
     *
     * @param file 設定する一時ファイル配列
     */
    void setFile(FileStruct[] file) {
        this.file = file;
    }
}

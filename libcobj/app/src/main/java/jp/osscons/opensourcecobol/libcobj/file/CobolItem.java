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

import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;

/**
 * COBOL SORT処理における1つのソート対象レコードを表すクラス。
 *
 * <p>マージソートのキュー内で連結リストの要素として使用される。 各レコードのデータ、一意識別子、ブロック終端フラグなどの情報を保持し、
 * ソート処理中のレコードの追跡と安定ソートの実現に使用される。
 *
 * <p>libcob/fileio.cのstruct cobitemに対応する。
 */
class CobolItem {
    /** 連結リスト内の次の要素への参照 */
    private CobolItem next;

    /** ソート済みブロック（ラン）の終端を示すフラグ。1の場合はブロック終端 */
    private int endOfBlock;

    /** このレコードのサイズ（バイト数）。可変長レコードの場合に使用 */
    private int recordSize;

    /** 一時ファイルへの書き込み時に使用するブロック区切りバイト */
    private byte blockByte = 0;

    /** 安定ソートを実現するための一意識別子（8バイト）。同一キー値のレコードの順序を保持 */
    private CobolDataStorage unique = new CobolDataStorage(new byte[8]);

    /** ソート対象レコードの実データ */
    private CobolDataStorage item;

    /** デフォルトコンストラクタ。空のソートアイテムを作成する。 */
    CobolItem() {
        this.next = null;
        this.endOfBlock = 0;
    }

    /**
     * 連結リスト内の次の要素を取得する。
     *
     * @return 次の要素。末尾の場合はnull
     */
    CobolItem getNext() {
        return next;
    }

    /**
     * 連結リスト内の次の要素を設定する。
     *
     * @param next 設定する次の要素
     */
    void setNext(CobolItem next) {
        this.next = next;
    }

    /**
     * ブロック終端フラグを取得する。
     *
     * @return ブロック終端フラグ。1の場合はこの要素がブロックの最後
     */
    int getEndOfBlock() {
        return endOfBlock;
    }

    /**
     * ブロック終端フラグを設定する。
     *
     * @param endOfBlock 設定するブロック終端フラグ
     */
    void setEndOfBlock(int endOfBlock) {
        this.endOfBlock = endOfBlock;
    }

    /**
     * レコードサイズを取得する。
     *
     * @return レコードサイズ（バイト数）
     */
    int getRecordSize() {
        return recordSize;
    }

    /**
     * レコードサイズを設定する。
     *
     * @param recordSize 設定するレコードサイズ（バイト数）
     */
    void setRecordSize(int recordSize) {
        this.recordSize = recordSize;
    }

    /**
     * ブロック区切りバイトを取得する。
     *
     * @return ブロック区切りバイト
     */
    byte getBlockByte() {
        return blockByte;
    }

    /**
     * ブロック区切りバイトを設定する。
     *
     * @param blockByte 設定するブロック区切りバイト
     */
    void setBlockByte(byte blockByte) {
        this.blockByte = blockByte;
    }

    /**
     * 一意識別子を取得する。
     *
     * @return 一意識別子のストレージ
     */
    CobolDataStorage getUnique() {
        return unique;
    }

    /**
     * 一意識別子を設定する。
     *
     * @param unique 設定する一意識別子
     */
    void setUnique(CobolDataStorage unique) {
        this.unique = unique;
    }

    /**
     * レコードデータを取得する。
     *
     * @return レコードデータのストレージ
     */
    CobolDataStorage getItem() {
        return item;
    }

    /**
     * レコードデータを設定する。
     *
     * @param item 設定するレコードデータ
     */
    void setItem(CobolDataStorage item) {
        this.item = item;
    }
}

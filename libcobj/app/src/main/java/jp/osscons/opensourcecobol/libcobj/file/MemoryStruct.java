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

/**
 * COBOL SORT処理でメモリ内ソートに使用するキュー構造を表すクラス。
 *
 * <p>{@link CobolItem}の連結リストを管理し、ソート対象レコードを保持する。 {@link CobolSort}クラスで4つのMemoryStructインスタンスがキューとして使用され、
 * マージソートの各フェーズで入力キューと出力キューの役割を交互に切り替えながらソートを実行する。
 *
 * <p>libcob/fileio.cのstruct memory_structに対応する。
 */
class MemoryStruct {
    /** リストの先頭要素 */
    private CobolItem first;

    /** リストの末尾要素 */
    private CobolItem last;

    /** リスト内の要素数 */
    private int count;

    /** デフォルトコンストラクタ。空のリストを作成する。 */
    MemoryStruct() {
        this.first = null;
        this.last = null;
    }

    /**
     * リストの先頭要素を取得する。
     *
     * @return リストの先頭要素。リストが空の場合はnull
     */
    CobolItem getFirst() {
        return first;
    }

    /**
     * リストの先頭要素を設定する。
     *
     * @param first 設定する先頭要素
     */
    void setFirst(CobolItem first) {
        this.first = first;
    }

    /**
     * リストの末尾要素を取得する。
     *
     * @return リストの末尾要素。リストが空の場合はnull
     */
    CobolItem getLast() {
        return last;
    }

    /**
     * リストの末尾要素を設定する。
     *
     * @param last 設定する末尾要素
     */
    void setLast(CobolItem last) {
        this.last = last;
    }

    /**
     * リスト内の要素数を取得する。
     *
     * @return 要素数
     */
    int getCount() {
        return count;
    }

    /**
     * リスト内の要素数を設定する。
     *
     * @param count 設定する要素数
     */
    void setCount(int count) {
        this.count = count;
    }
}

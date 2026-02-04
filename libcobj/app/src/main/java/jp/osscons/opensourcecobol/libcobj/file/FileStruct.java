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
 * 外部マージソート処理で使用する一時ファイルの情報を保持するクラス。
 *
 * <p>COBOL SORT文の処理において、メモリに収まりきらない大量データをソートする際に、 一時ファイルを使用したマージソートを実行する。
 * このクラスは各一時ファイルのハンドルと、そのファイルに書き込まれたブロック数を管理する。 {@link CobolSort}クラスで4つのFileStructインスタンスが使用され、
 * マージソートの入力と出力を交互に切り替えながらソートを進める。
 */
class FileStruct {
    /** 一時ファイルへのI/Oハンドル */
    private FileIO fp;

    /** このファイルに書き込まれたブロック（ソート済みラン）の数 */
    private int count;

    /**
     * 一時ファイルのI/Oハンドルを取得する。
     *
     * @return 一時ファイルのI/Oハンドル。ファイルが作成されていない場合はnull
     */
    FileIO getFp() {
        return fp;
    }

    /**
     * 一時ファイルのI/Oハンドルを設定する。
     *
     * @param fp 設定する一時ファイルのI/Oハンドル
     */
    void setFp(FileIO fp) {
        this.fp = fp;
    }

    /**
     * ファイルに書き込まれたブロック数を取得する。
     *
     * @return ブロック数
     */
    int getCount() {
        return count;
    }

    /**
     * ファイルに書き込まれたブロック数を設定する。
     *
     * @param count 設定するブロック数
     */
    void setCount(int count) {
        this.count = count;
    }
}

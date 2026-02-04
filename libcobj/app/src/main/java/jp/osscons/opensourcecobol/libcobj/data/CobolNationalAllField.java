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
package jp.osscons.opensourcecobol.libcobj.data;

/**
 * COBOL ALL定数（日本語型）を表現するクラス。
 * ALL NC"文字"など、指定された全角文字またはリテラルを繰り返すALL定数を扱う。
 * 比較やMOVE時に、受け側のサイズに合わせて指定文字が繰り返し適用される。
 */
public class CobolNationalAllField extends CobolNationalField {
    /**
     * コンストラクタ
     *
     * @param size データを格納するバイト配列の長さ
     * @param dataStorage データを格納するバイト配列を扱うオブジェクト
     * @param attribute 変数に関する様々な情報を保持するオブジェクト
     */
    public CobolNationalAllField(
            int size, CobolDataStorage dataStorage, CobolFieldAttribute attribute) {
        super(size, dataStorage, attribute);
    }
}

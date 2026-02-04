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

/**
 * INDEXED/RELATIVEファイルのキーを構成するコンポーネントを表すクラス。
 *
 * <p>COBOLのRECORD KEY句で複合キーを定義する際に、各キー構成要素の情報を保持する。 複合キーの場合、複数のKeyComponentインスタンスで1つのキーを構成する。
 */
public class KeyComponent {
    /** このキー構成要素に対応するCOBOLフィールド */
    public AbstractCobolField field;

    /** レコードバッファ内でのこのキー構成要素の相対バイト位置 */
    public int rb;
}

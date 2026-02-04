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
 * COBOLファイルのキー情報を保持するクラス。
 *
 * <p>INDEXED/RELATIVEファイルの主キーおよび代替キー、またはSORT文のソートキーの定義を表す。 COBOL RECORD KEY句、ALTERNATE RECORD KEY句、SORT KEY句で定義された
 * キー情報（フィールド、オフセット、昇順/降順フラグ等）を保持する。 複合キーの場合は複数のKeyComponentで構成される。
 *
 * <p>libcob/common.hのstruct cob_file_keyに対応する。
 */
public class CobolFileKey {
    /** 複合キーを構成できる最大コンポーネント数 */
    public static final int COB_MAX_KEY_COMPONENTS = 8;

    /** このキーに対応するCOBOLフィールド */
    private AbstractCobolField field;

    /** キーのフラグ。SORT時は昇順(0)/降順(1)、INDEXEDファイルではDUPLICATES指定等 */
    private int flag;

    /** レコード内でのこのキーの開始オフセット（バイト位置） */
    private int offset;

    /** 複合キーを構成するコンポーネントの数 */
    private int countComponents;

    /** 複合キーの各コンポーネント情報の配列 */
    private KeyComponent[] component = new KeyComponent[COB_MAX_KEY_COMPONENTS];

    /**
     * キーに対応するCOBOLフィールドを取得する。
     *
     * @return キーフィールド
     */
    public AbstractCobolField getField() {
        return field;
    }

    /**
     * キーに対応するCOBOLフィールドを設定する。
     *
     * @param field 設定するキーフィールド
     */
    public void setField(AbstractCobolField field) {
        this.field = field;
    }

    /**
     * キーのフラグを取得する。
     *
     * @return フラグ値（SORT時: 0=昇順, 1=降順）
     */
    public int getFlag() {
        return flag;
    }

    /**
     * キーのフラグを設定する。
     *
     * @param flag 設定するフラグ値
     */
    public void setFlag(int flag) {
        this.flag = flag;
    }

    /**
     * レコード内でのキーの開始オフセットを取得する。
     *
     * @return オフセット（バイト位置）
     */
    public int getOffset() {
        return offset;
    }

    /**
     * レコード内でのキーの開始オフセットを設定する。
     *
     * @param offset 設定するオフセット（バイト位置）
     */
    public void setOffset(int offset) {
        this.offset = offset;
    }

    /**
     * 複合キーのコンポーネント数を取得する。
     *
     * @return コンポーネント数
     */
    public int getCountComponents() {
        return countComponents;
    }

    /**
     * 複合キーのコンポーネント数を設定する。
     *
     * @param countComponents 設定するコンポーネント数
     */
    public void setCountComponents(int countComponents) {
        this.countComponents = countComponents;
    }

    /**
     * 複合キーのコンポーネント配列を取得する。
     *
     * @return コンポーネント配列
     */
    public KeyComponent[] getComponent() {
        return component;
    }

    /**
     * 複合キーのコンポーネント配列を設定する。
     *
     * @param component 設定するコンポーネント配列
     */
    public void setComponent(KeyComponent[] component) {
        this.component = component;
    }

    /**
     * 複合キーの最大コンポーネント数を取得する。
     *
     * @return 最大コンポーネント数（8）
     */
    public static int getCobMaxKeyComponents() {
        return COB_MAX_KEY_COMPONENTS;
    }
}

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
 * Describes a single key (primary or alternate) of an INDEXED file.
 *
 * <p>The compiler ({@code cobj}) generates one {@code CobolFileKey} per {@code RECORD KEY} / {@code
 * ALTERNATE RECORD KEY} declared in a {@code SELECT} clause and passes the resulting array to
 * {@link CobolFileFactory#makeCobolFileInstance}. Each entry carries the COBOL field that holds the
 * key value, a duplicates flag, the byte offset of the key within the record, and—when the key is a
 * SPLIT KEY—the list of {@link KeyComponent}s that make it up.
 *
 * <p>In the SQLite storage used by {@link CobolIndexedFile}, the entry at index {@code 0} is the
 * primary key (stored in {@code table0}) and entries at index {@code i >= 1} are alternate keys
 * (stored in {@code tableI}).
 */
public class CobolFileKey {
    /** The maximum number of {@link KeyComponent}s a single SPLIT KEY may consist of. */
    public static final int COB_MAX_KEY_COMPONENTS = 8;

    private AbstractCobolField field;
    private int flag;
    private int offset;
    private int countComponents;
    private KeyComponent[] component = new KeyComponent[COB_MAX_KEY_COMPONENTS];

    /**
     * Returns the COBOL field that holds the value of this key.
     *
     * @return the key field
     */
    public AbstractCobolField getField() {
        return field;
    }

    /**
     * Sets the COBOL field that holds the value of this key.
     *
     * @param field the key field
     */
    public void setField(AbstractCobolField field) {
        this.field = field;
    }

    /**
     * Returns the duplicates flag of this key.
     *
     * @return {@code 0} if duplicate values are not allowed (primary key or {@code ALTERNATE RECORD
     *     KEY} without {@code WITH DUPLICATES}), or a non-zero value if duplicates are allowed
     *     ({@code ALTERNATE RECORD KEY ... WITH DUPLICATES})
     */
    public int getFlag() {
        return flag;
    }

    /**
     * Sets the duplicates flag of this key.
     *
     * @param flag {@code 0} to disallow duplicate values, or a non-zero value to allow them
     */
    public void setFlag(int flag) {
        this.flag = flag;
    }

    /**
     * Returns the byte offset of this key within the record.
     *
     * @return the 0-origin byte offset from the beginning of the record, or {@code -1} when the key
     *     is a SPLIT KEY (in which case the layout is described by {@link #getComponent()})
     */
    public int getOffset() {
        return offset;
    }

    /**
     * Sets the byte offset of this key within the record.
     *
     * @param offset the 0-origin byte offset from the beginning of the record, or {@code -1} for a
     *     SPLIT KEY
     */
    public void setOffset(int offset) {
        this.offset = offset;
    }

    /**
     * Returns the number of {@link KeyComponent}s that make up this key.
     *
     * @return the number of valid entries in the array returned by {@link #getComponent()}; a SPLIT
     *     KEY has two or more, an ordinary key has zero
     */
    public int getCountComponents() {
        return countComponents;
    }

    /**
     * Sets the number of {@link KeyComponent}s that make up this key.
     *
     * @param countComponents the number of valid entries in the component array
     */
    public void setCountComponents(int countComponents) {
        this.countComponents = countComponents;
    }

    /**
     * Returns the array of {@link KeyComponent}s describing a SPLIT KEY.
     *
     * <p>Only the first {@link #getCountComponents()} entries are meaningful.
     *
     * @return the component array (never {@code null}; its capacity is {@link
     *     #COB_MAX_KEY_COMPONENTS})
     */
    public KeyComponent[] getComponent() {
        return component;
    }

    /**
     * Sets the array of {@link KeyComponent}s describing a SPLIT KEY.
     *
     * @param component the component array
     */
    public void setComponent(KeyComponent[] component) {
        this.component = component;
    }

    /**
     * Returns the maximum number of components a SPLIT KEY may consist of.
     *
     * @return the value of {@link #COB_MAX_KEY_COMPONENTS}
     */
    public static int getCobMaxKeyComponents() {
        return COB_MAX_KEY_COMPONENTS;
    }
}

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
 * Represents a single component of a SPLIT KEY of an INDEXED file.
 *
 * <p>COBOL allows a record key to be composed of several non-contiguous fields (a SPLIT KEY, e.g.
 * {@code RECORD KEY IS K = F1 F2 ...}). Each constituent field is described by one {@code
 * KeyComponent}. The compiler ({@code cobj}) emits, for every key that has more than one component,
 * an array of {@code KeyComponent} and stores it into the owning {@link CobolFileKey} via {@link
 * CobolFileKey#setComponent(KeyComponent[])}.
 */
public class KeyComponent {
    /** The COBOL field that makes up this component of the key. */
    public AbstractCobolField field;

    /** The 0-origin byte offset of this component from the beginning of the record. */
    public int rb;
}

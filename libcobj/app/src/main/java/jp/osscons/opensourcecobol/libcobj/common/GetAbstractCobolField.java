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
package jp.osscons.opensourcecobol.libcobj.common;

import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;

/**
 * COBOLフィールドを遅延評価で取得するための関数型インターフェース。
 *
 * <p>COBOLの変数参照において、値の評価を実行時まで遅延させる必要がある場合に使用する。
 * 例えば、DEPENDING ON句で指定された変数によってサイズが決まる可変長テーブルの
 * 要素アクセスなど、実行時にフィールドを動的に決定する場面で利用される。
 *
 * <p>Java 8のSupplierインターフェースに類似するが、CobolStopRunExceptionをスローできる点が異なる。
 */
public interface GetAbstractCobolField {
    /**
     * COBOLフィールドを取得する。
     *
     * <p>実行時にフィールドを評価・取得する。フィールドの参照が無効な場合や
     * 実行時エラーが発生した場合はCobolStopRunExceptionがスローされる可能性がある。
     *
     * @return 取得されたCOBOLフィールド
     * @throws CobolStopRunException STOP RUN文の実行やランタイムエラー発生時
     */
    AbstractCobolField run() throws CobolStopRunException;
}

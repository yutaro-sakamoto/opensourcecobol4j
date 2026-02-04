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

import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;

/**
 * 整数値を遅延評価で取得するための関数型インターフェース。
 *
 * <p>COBOLの数値式や添字式において、値の評価を実行時まで遅延させる必要がある場合に使用する。
 * 例えば、テーブル要素の添字計算やOCCURS DEPENDING ON句で指定された変数の評価など、
 * 実行時に整数値を動的に決定する場面で利用される。
 *
 * <p>Java 8のIntSupplierインターフェースに類似するが、CobolStopRunExceptionをスローできる点が異なる。
 */
public interface GetInt {
    /**
     * 整数値を取得する。
     *
     * <p>実行時に整数値を評価・取得する。評価中にランタイムエラーが発生した場合は
     * CobolStopRunExceptionがスローされる可能性がある。
     *
     * @return 取得された整数値
     * @throws CobolStopRunException STOP RUN文の実行やランタイムエラー発生時
     */
    int run() throws CobolStopRunException;
}

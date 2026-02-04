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

/**
 * COBOL CALL文で渡されるパラメータ数を管理するクラス。
 *
 * <p>COBOLのCALL文でサブプログラムを呼び出す際、渡されたパラメータの数を追跡する。
 * これにより、呼び出されたプログラムはCALL文で実際に渡されたパラメータ数を
 * 確認することができる。libcob/common.cのcob_call_paramsに対応する。
 */
public class CobolCallParams {
    /**
     * 現在のCALL文で渡されたパラメータ数。
     *
     * <p>CALL文の実行前に設定され、呼び出されたプログラム内で参照される。
     * COBOL組み込み関数NUMBER-OF-CALL-PARAMETERSで使用される値に対応する。
     */
    public static int callParams = 0;
}

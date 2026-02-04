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
package jp.osscons.opensourcecobol.libcobj.exceptions;

/**
 * COBOL実行時の例外情報を保持するクラス。
 *
 * <p>このクラスは、COBOLプログラム実行中に発生した例外の16ビット例外コードを
 * 静的変数として保持する。主にCobolRuntimeExceptionと連携して使用される。
 *
 * @see CobolExceptionId
 * @see CobolExceptionTabCode
 * @see CobolRuntimeException
 */
public class CobolExceptionInfo {
    /**
     * 現在の例外コード。
     * 例外が発生していない場合は0、発生した場合は対応する16ビット例外コードが設定される。
     */
    public static int code = 0;

    /**
     * 指定された例外IDに対応する例外コードを設定する。
     *
     * <p>CobolExceptionIdの定数をインデックスとしてCobolExceptionTabCodeのテーブルを参照し、
     * 対応する16ビット例外コードを取得してcode変数に設定する。
     *
     * @param id CobolExceptionIdで定義された例外ID
     */
    public static void setException(int id) {
        CobolExceptionInfo.code = CobolExceptionTabCode.code[id];
    }
}

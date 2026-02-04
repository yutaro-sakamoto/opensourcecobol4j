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

import java.util.AbstractMap;
import java.util.HashMap;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.file.CobolFile;

/**
 * COBOLのEXTERNAL句で定義された外部共有データを管理するクラス。
 *
 * <p>EXTERNAL句を持つデータ項目やファイルは、複数のプログラム間で同一の記憶域を共有する。
 * このクラスは、外部項目の名前と記憶域のマッピングを管理し、プログラム間でデータの
 * 整合性を保つ。
 *
 * <p>COBOLの以下の構文に対応：
 * <pre>
 *     01 WS-DATA EXTERNAL PIC X(100).
 *     SELECT EXT-FILE ASSIGN TO "test.dat" EXTERNAL.
 * </pre>
 *
 * <p>libcob/common.cのcob_external_addr、external_hashに対応する。
 */
public final class CobolExternal {

    /** 外部ファイルへの参照 */
    private CobolFile extAllocFile;

    /** 外部データストレージへの参照 */
    private CobolDataStorage extAllocStorage;

    /** 外部項目名とCobolExternalインスタンスのマッピング */
    private static AbstractMap<String, CobolExternal> externalMap =
            new HashMap<String, CobolExternal>();

    /**
     * ファイル用のコンストラクタ。
     *
     * @param file 外部ファイル
     */
    private CobolExternal(CobolFile file) {
        this.extAllocFile = file;
    }

    /**
     * データストレージ用のコンストラクタ。
     *
     * @param storage 外部データストレージ
     * @param size ストレージのサイズ（バイト）
     */
    private CobolExternal(CobolDataStorage storage, int size) {
        this.extAllocStorage = storage;
    }

    /**
     * 外部ファイルのアドレス（参照）を取得する。
     *
     * <p>指定された名前の外部ファイルが既に存在する場合はその参照を返し、
     * 存在しない場合は新規作成して登録後、その参照を返す。
     *
     * @param name 外部ファイルの名前（EXTERNAL句で指定された名前）
     * @return 外部ファイルへの参照
     */
    public static CobolFile getFileAddress(String name) {
        if (externalMap.containsKey(name)) {
            return externalMap.get(name).extAllocFile;
        } else {
            CobolFile ret = new CobolFile();
            CobolExternal ext = new CobolExternal(ret);
            externalMap.put(name, ext);
            return ret;
        }
    }

    /**
     * 外部データストレージのアドレス（参照）を取得する。
     *
     * <p>指定された名前の外部データ領域が既に存在する場合はその参照を返し、
     * 存在しない場合は指定サイズで新規作成して登録後、その参照を返す。
     * 最初に取得した側のサイズで領域が確保されることに注意。
     *
     * @param name 外部データ項目の名前（EXTERNAL句で指定された名前）
     * @param size データ領域のサイズ（バイト）
     * @return 外部データストレージへの参照
     */
    public static CobolDataStorage getStorageAddress(String name, int size) {
        if (externalMap.containsKey(name)) {
            return externalMap.get(name).extAllocStorage;
        } else {
            CobolDataStorage ret = new CobolDataStorage(size);
            CobolExternal ext = new CobolExternal(ret, size);
            externalMap.put(name, ext);
            return ret;
        }
    }
}

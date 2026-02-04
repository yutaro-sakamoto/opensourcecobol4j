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
 * COBOLファイルインスタンスを生成するファクトリクラス。
 *
 * <p>COBOLからJavaへ変換されたコードで使用され、ファイル編成（ORGANIZATION）に応じて 適切なCobolFileサブクラスのインスタンスを生成する。
 *
 * <p>対応するファイル編成:
 *
 * <ul>
 *   <li>SEQUENTIAL - {@link CobolSequentialFile}
 *   <li>LINE SEQUENTIAL - {@link CobolLineSequentialFile}
 *   <li>RELATIVE - {@link CobolRelativeFile}
 *   <li>INDEXED - {@link CobolIndexedFile}
 * </ul>
 */
public class CobolFileFactory {

    /**
     * 指定されたパラメータに基づいてCobolFileインスタンスを生成する。
     *
     * <p>organization引数に応じて適切なサブクラスのインスタンスが返される。
     *
     * @param selectName ファイルのSELECT名（COBOL SELECT句で指定された名前）
     * @param fileStatus ファイルステータスを格納するバイト配列（4バイト）
     * @param assign ASSIGN句で指定されたファイル名フィールド
     * @param record レコード領域を表すフィールド
     * @param recordSize 可変長レコードの場合のレコード長フィールド（固定長の場合はnull）
     * @param recordMin 最小レコード長
     * @param recordMax 最大レコード長
     * @param nkeys キーの数（INDEXED/RELATIVEファイルの場合）
     * @param keys キー情報の配列
     * @param organization ファイル編成（COB_ORG_SEQUENTIAL等）
     * @param accessMode アクセスモード（COB_ACCESS_SEQUENTIAL等）
     * @param lockMode ロックモード
     * @param openMode 現在のオープンモード
     * @param flagOptional OPTIONALファイルかどうか
     * @param lastOpenMode 最後のオープンモード
     * @param special 特殊ファイルフラグ
     * @param flagNonexistent ファイルが存在しないかどうか
     * @param flagEndOfFile ファイル終端に達したかどうか
     * @param flagBeginOfFile ファイル先頭にいるかどうか
     * @param flagFirstRead 最初の読み取りかどうか
     * @param flagReadDone 読み取りが完了したかどうか
     * @param flagSelectFeatures SELECT機能フラグ
     * @param flagNeedsNl 改行が必要かどうか
     * @param flagNeedsTop ページトップが必要かどうか
     * @param fileVersion ファイルバージョン
     * @return 生成されたCobolFileインスタンス
     */
    public static CobolFile makeCobolFileInstance(
            String selectName,
            byte[] fileStatus,
            AbstractCobolField assign,
            AbstractCobolField record,
            AbstractCobolField recordSize,
            int recordMin,
            int recordMax,
            int nkeys,
            CobolFileKey[] keys,
            char organization,
            char accessMode,
            char lockMode,
            char openMode,
            boolean flagOptional,
            char lastOpenMode,
            char special,
            boolean flagNonexistent,
            boolean flagEndOfFile,
            boolean flagBeginOfFile,
            char flagFirstRead,
            boolean flagReadDone,
            char flagSelectFeatures,
            boolean flagNeedsNl,
            boolean flagNeedsTop,
            char fileVersion) {
        switch (organization) {
            case CobolFile.COB_ORG_SEQUENTIAL:
                return new CobolSequentialFile(
                        selectName,
                        fileStatus,
                        assign,
                        record,
                        recordSize,
                        recordMin,
                        recordMax,
                        nkeys,
                        keys,
                        organization,
                        accessMode,
                        lockMode,
                        openMode,
                        flagOptional,
                        lastOpenMode,
                        special,
                        flagNonexistent,
                        flagEndOfFile,
                        flagBeginOfFile,
                        flagFirstRead,
                        flagReadDone,
                        flagSelectFeatures,
                        flagNeedsNl,
                        flagNeedsTop,
                        fileVersion);
            case CobolFile.COB_ORG_LINE_SEQUENTIAL:
                return new CobolLineSequentialFile(
                        selectName,
                        fileStatus,
                        assign,
                        record,
                        recordSize,
                        recordMin,
                        recordMax,
                        nkeys,
                        keys,
                        organization,
                        accessMode,
                        lockMode,
                        openMode,
                        flagOptional,
                        lastOpenMode,
                        special,
                        flagNonexistent,
                        flagEndOfFile,
                        flagBeginOfFile,
                        flagFirstRead,
                        flagReadDone,
                        flagSelectFeatures,
                        flagNeedsNl,
                        flagNeedsTop,
                        fileVersion);
            case CobolFile.COB_ORG_RELATIVE:
                return new CobolRelativeFile(
                        selectName,
                        fileStatus,
                        assign,
                        record,
                        recordSize,
                        recordMin,
                        recordMax,
                        nkeys,
                        keys,
                        organization,
                        accessMode,
                        lockMode,
                        openMode,
                        flagOptional,
                        lastOpenMode,
                        special,
                        flagNonexistent,
                        flagEndOfFile,
                        flagBeginOfFile,
                        flagFirstRead,
                        flagReadDone,
                        flagSelectFeatures,
                        flagNeedsNl,
                        flagNeedsTop,
                        fileVersion);
            case CobolFile.COB_ORG_INDEXED:
                return new CobolIndexedFile(
                        selectName,
                        fileStatus,
                        assign,
                        record,
                        recordSize,
                        recordMin,
                        recordMax,
                        nkeys,
                        keys,
                        organization,
                        accessMode,
                        lockMode,
                        openMode,
                        flagOptional,
                        lastOpenMode,
                        special,
                        flagNonexistent,
                        flagEndOfFile,
                        flagBeginOfFile,
                        flagFirstRead,
                        flagReadDone,
                        flagSelectFeatures,
                        flagNeedsNl,
                        flagNeedsTop,
                        fileVersion);
            default:
                return new CobolFile(
                        selectName,
                        fileStatus,
                        assign,
                        record,
                        recordSize,
                        recordMin,
                        recordMax,
                        nkeys,
                        keys,
                        organization,
                        accessMode,
                        lockMode,
                        openMode,
                        flagOptional,
                        lastOpenMode,
                        special,
                        flagNonexistent,
                        flagEndOfFile,
                        flagBeginOfFile,
                        flagFirstRead,
                        flagReadDone,
                        flagSelectFeatures,
                        flagNeedsNl,
                        flagNeedsTop,
                        fileVersion);
        }
    }
}

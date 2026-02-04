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

import java.io.IOException;
import java.nio.ByteBuffer;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;

/**
 * COBOL SEQUENTIAL編成ファイルの実装クラス。
 *
 * <p>COBOL ORGANIZATION IS SEQUENTIALで定義されたファイルの入出力操作を提供する。
 * レコードはファイル内で連続して格納され、先頭から順次アクセスされる。 固定長レコードと可変長レコードの両方をサポートする。
 *
 * <p>可変長レコードの場合、各レコードの先頭にレコード長（4バイト）が格納される。
 */
public class CobolSequentialFile extends CobolFile {

    /**
     * SEQUENTIALファイルインスタンスを生成する。
     *
     * @param selectName ファイルのSELECT名
     * @param fileStatus ファイルステータスを格納するバイト配列
     * @param assign ASSIGN句で指定されたファイル名フィールド
     * @param record レコード領域を表すフィールド
     * @param recordSize 可変長レコードの場合のレコード長フィールド
     * @param recordMin 最小レコード長
     * @param recordMax 最大レコード長
     * @param nkeys キーの数（SEQUENTIALファイルでは未使用）
     * @param keys キー情報の配列（SEQUENTIALファイルでは未使用）
     * @param organization ファイル編成（COB_ORG_SEQUENTIAL）
     * @param accessMode アクセスモード
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
     */
    public CobolSequentialFile(
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
        super(
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

    @Override
    public int start_(int cond, AbstractCobolField key) {
        System.out.println("Seq.start");
        return 0;
    }

    @Override
    public int read_(AbstractCobolField key, int readOpts) {
        System.out.println("Seq.read");
        return 0;
    }

    @Override
    public int readNext(int readOpts) {
        byte[] sbuff = new byte[4];
        this.file.seekInit();

        if (this.record_min != this.record_max) {
            if (this.file.read(sbuff, 4) != 1) {
                if (this.file.isAtEnd()) {
                    return COB_STATUS_10_END_OF_FILE;
                } else {
                    return COB_STATUS_30_PERMANENT_ERROR;
                }
            }
            int size = ByteBuffer.wrap(sbuff).getInt();
            size = Math.max(size, this.record_min);
            size = Math.min(size, this.record_max);
            this.record.setSize(size);
        }

        try {
            int bytesread = this.file.read(this.record.getDataStorage(), this.record.getSize());
            if (bytesread != this.record.getSize()) {
                if (bytesread == 0) {
                    return COB_STATUS_10_END_OF_FILE;
                } else {
                    return COB_STATUS_04_SUCCESS_INCOMPLETE;
                }
            }
        } catch (IOException e) {
            return COB_STATUS_30_PERMANENT_ERROR;
        }
        return COB_STATUS_00_SUCCESS;
    }

    @Override
    public int write_(int opt) throws CobolStopRunException {
        int ret;
        byte[] sbuff = new byte[4];

        this.file.seekInit();

        /* WRITE AFTER */
        if ((opt & COB_WRITE_AFTER) != 0) {
            ret = file_write_opt(opt);
            if (ret != 0) {
                return ret;
            }
        }

        if (this.record_min != this.record_max) {
            ByteBuffer.wrap(sbuff).putInt(this.record.getSize());
            if (!this.file.write(sbuff, 4)) {
                return COB_STATUS_30_PERMANENT_ERROR;
            }
        }

        /* write the record */
        if (!this.file.write(this.record.getDataStorage(), this.record.getSize())) {
            return COB_STATUS_30_PERMANENT_ERROR;
        }

        /* WRITE BEFORE */
        if ((opt & COB_WRITE_BEFORE) != 0) {
            ret = this.file_write_opt(opt);
            if (ret != 0) {
                return ret;
            }
            this.flag_needs_nl = false;
        }
        return 0;
    }

    @Override
    public int rewrite_(int opt) {
        if (!this.file.seek(-this.record.getSize(), FileIO.SEEK_CUR)) {
            return COB_STATUS_30_PERMANENT_ERROR;
        }
        if (!this.file.write(this.record.getDataStorage(), this.record.getSize())) {
            return COB_STATUS_30_PERMANENT_ERROR;
        }
        return COB_STATUS_00_SUCCESS;
    }

    @Override
    public int delete_() {
        System.out.println("Seq.delete");
        return 0;
    }
}

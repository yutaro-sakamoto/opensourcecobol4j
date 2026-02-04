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
package jp.osscons.opensourcecobol.libcobj.data;

import java.math.BigDecimal;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * COBOL英数字編集型（PIC X編集）の変数を表現するクラス。
 * PIC句に編集文字（B、0、/等）を含む英数字項目を扱い、
 * MOVE時に指定された編集パターンに従って文字の挿入・整形を行う。
 */
public class CobolAlphanumericEditedField extends AbstractCobolField {
    /**
     * コンストラクタ
     *
     * @param size データを格納するバイト配列の長さ
     * @param dataStorage データを格納するバイト配列を扱うオブジェクト
     * @param attribute 変数に関する様々な情報を保持するオブジェクト
     */
    public CobolAlphanumericEditedField(
            int size, CobolDataStorage dataStorage, CobolFieldAttribute attribute) {
        super(size, dataStorage, attribute);
    }

    @Override
    public byte[] getBytes() {
        // TODO Auto-generated method stub
        return new byte[0];
    }

    @Override
    public String getString() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public CobolDecimal getDecimal() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void setDecimal(BigDecimal decimal) {
        // TODO Auto-generated method stub

    }

    @Override
    public int addPackedInt(int n) {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void moveFrom(AbstractCobolField src) {
        AbstractCobolField src1 = this.preprocessOfMoving(src);
        if (src1 == null) {
            return;
        }

        switch (src1.getAttribute().getType()) {
            case CobolFieldAttribute.COB_TYPE_NUMERIC_DISPLAY:
                CobolFieldAttribute attr = src1.getAttribute();
                int scale = src1.getAttribute().getScale();
                int digits = src1.getAttribute().getDigits();
                if (scale < 0 || scale > digits) {
                    CobolFieldAttribute newAttr =
                            new CobolFieldAttribute(
                                    CobolFieldAttribute.COB_TYPE_ALPHANUMERIC,
                                    attr.getDigits(),
                                    0,
                                    0,
                                    null);
                    int newSize = scale < 0 ? digits - scale : scale;
                    AbstractCobolField newSrc =
                            CobolFieldFactory.makeCobolField(
                                    newSize, new CobolDataStorage(newSize), newAttr);
                    newSrc.moveFrom(src1);
                    this.moveFrom(newSrc);
                } else {
                    CobolAlphanumericEditedField.moveAlphanumToEdited(this, src1);
                }
                break;
            case CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED:
            case CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY:
            case CobolFieldAttribute.COB_TYPE_NUMERIC_DOUBLE:
            case CobolFieldAttribute.COB_TYPE_NUMERIC_FLOAT:
                this.moveFrom(src1.getNumericField());
                break;
            case CobolFieldAttribute.COB_TYPE_GROUP:
                CobolAlphanumericField.moveAlphanumToAlphanum(this, src1);
                break;
            default:
                CobolAlphanumericEditedField.moveAlphanumToEdited(this, src1);
                break;
        }
    }

    /**
     * 英数字フィールドから英数字編集フィールドへのMOVE処理を行う。
     * PIC句の編集パターン（A、X、9、0、/、B等）に従って、ソースフィールドの
     * 文字データを編集先フィールドに転記する。編集文字に応じて、
     * 文字の挿入（0、/）や空白挿入（B）が行われる。
     *
     * @param dst 編集先の英数字編集フィールド
     * @param src 転記元のフィールド
     */
    public static void moveAlphanumToEdited(AbstractCobolField dst, AbstractCobolField src) {
        CobolDataStorage srcd = src.getDataStorage();
        CobolDataStorage dstd = dst.getDataStorage();
        int sign = src.getSign();
        int max = src.getFieldSize();
        int srcp = src.getFirstDataIndex();
        int dstp = 0;
        byte[] picBytes = dst.getAttribute().getPic().getBytes(AbstractCobolField.charSetSJIS);
        final int sizeOfInt = 4;

        for (int p = 0; p < picBytes.length; ) {
            byte c = picBytes[p++];
            ByteBuffer buf = ByteBuffer.wrap(picBytes, p, sizeOfInt);
            buf.order(ByteOrder.LITTLE_ENDIAN);
            int n = buf.getInt();
            p += sizeOfInt;

            for (; n > 0; --n) {
                switch (c) {
                    case 'A':
                    case 'X':
                    case '9':
                        dstd.setByte(dstp++, (srcp < max) ? srcd.getByte(srcp++) : (byte) ' ');
                        break;
                    case '0':
                    case '/':
                        dstd.setByte(dstp++, c);
                        break;
                    case 'B':
                        dstd.setByte(dstp++, (byte) ' ');
                        break;
                    default:
                        dstd.setByte(dstp++, (byte) '?'); /* invalid PIC */
                        break;
                }
            }
        }
        src.putSign(sign);
    }

    @Override
    public void moveFrom(CobolDataStorage dataStrage) {
        // TODO Auto-generated method stub

    }

    @Override
    public void moveFrom(byte[] bytes) {
        // TODO Auto-generated method stub

    }

    @Override
    public void moveFrom(String string) {
        // TODO Auto-generated method stub
    }

    @Override
    public void moveFrom(int number) {
        // TODO Auto-generated method stub
    }

    @Override
    public void moveFrom(double number) {
        // TODO Auto-generated method stub

    }

    @Override
    public void moveFrom(BigDecimal number) {
        // TODO Auto-generated method stub
    }
}

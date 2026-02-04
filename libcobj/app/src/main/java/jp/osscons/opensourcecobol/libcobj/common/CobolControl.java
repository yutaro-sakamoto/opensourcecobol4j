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

import java.util.Optional;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolStopRunException;

/**
 * COBOLの制御フロー（PERFORM文、GO TO文）を表現する抽象クラス。
 *
 * <p>COBOLのPERFORM文やGO TO文による制御の移動を、継続渡しスタイル（CPS: Continuation Passing Style）
 * を用いて実装する。各段落・節は{@link CobolControl}のサブクラスとして表現され、
 * {@link #run()}メソッドが次の実行ポイントを返す。
 *
 * <p>この実装により、COBOLの複雑な制御フロー（PERFORMのネスト、GO TOによる脱出等）を
 * Javaで自然に表現できる。
 *
 * @see CobolModule
 */
public abstract class CobolControl {
    /**
     * ラベルの種類を表す列挙型。
     * PERFORM THRU文の終了判定で、節（SECTION）と段落（PARAGRAPH）を区別するために使用。
     */
    public enum LabelType {
        /** 段落ラベル（PARAGRAPH）。節の中の個別の実行単位。 */
        label,
        /** 節ラベル（SECTION）。複数の段落を含む実行単位。 */
        section,
    }

    /**
     * この制御ポイントを実行し、次の制御ポイントを返す。
     *
     * <p>COBOLの段落や節に対応するコードを実行し、実行後に次に移動すべき
     * 制御ポイントを{@link Optional}でラップして返す。空の{@link Optional}を返すと
     * 実行が終了する。
     *
     * @return 次に実行すべき制御ポイント。実行終了時は空のOptional
     * @throws CobolRuntimeException 実行時エラー発生時
     * @throws CobolStopRunException STOP RUN文の実行時
     */
    public abstract Optional<CobolControl> run()
            throws CobolRuntimeException, CobolStopRunException;

    /** この制御ポイントの識別子。プログラム内で一意の値。 */
    public int contId = -1;

    /** この制御ポイントの種類（段落または節）。 */
    public LabelType type = LabelType.label;

    /**
     * デフォルトコンストラクタ。
     * contIdは-1、typeはLabelType.labelで初期化される。
     */
    public CobolControl() {
        this.contId = -1;
        this.type = LabelType.label;
    }

    /**
     * 制御ポイントの識別子と種類を指定するコンストラクタ。
     *
     * @param contId 制御ポイントの識別子
     * @param type 制御ポイントの種類（段落または節）
     */
    public CobolControl(int contId, LabelType type) {
        this.contId = contId;
        this.type = type;
    }

    /**
     * 何もしない制御ポイントを生成する。
     *
     * <p>PERFORM文の終了後に制御を戻す際など、実行を終了させる目的で使用する。
     *
     * @return 空のOptionalを返すCobolControlインスタンス
     */
    public static CobolControl pure() {
        return new CobolControl() {
            @Override
            public Optional<CobolControl> run()
                    throws CobolRuntimeException, CobolStopRunException {
                return Optional.empty();
            }
        };
    }

    /**
     * GO TO文に対応する制御ポイントを生成する。
     *
     * <p>指定された制御ポイントへ無条件にジャンプする。
     *
     * @param cont ジャンプ先の制御ポイント
     * @return GO TO文を表現するCobolControlインスタンス
     */
    public static CobolControl goTo(CobolControl cont) {
        return new CobolControl() {
            @Override
            public Optional<CobolControl> run()
                    throws CobolRuntimeException, CobolStopRunException {
                return cont.run();
            }
        };
    }

    /**
     * PERFORM THRU文に対応する制御ポイントを生成する。
     *
     * <p>指定された範囲（begin〜end）の段落・節を順次実行する。
     * COBOL文「PERFORM begin-paragraph THRU end-paragraph」に対応。
     * 終了判定では、endがセクションの場合はそのセクション内の全段落を実行する。
     *
     * @param contList 全制御ポイントの配列
     * @param begin 開始ラベルのインデックス
     * @param end 終了ラベルのインデックス
     * @return PERFORM THRU文を表現するCobolControlインスタンス
     */
    public static CobolControl performThrough(CobolControl[] contList, int begin, int end) {
        return new CobolControl() {
            @Override
            public Optional<CobolControl> run()
                    throws CobolRuntimeException, CobolStopRunException {
                Optional<CobolControl> nextCont = Optional.of(contList[begin]);
                LabelType endType = contList[end].type;
                int executedProgramId;
                do {
                    CobolControl cont = nextCont.get();
                    executedProgramId = cont.contId;
                    nextCont = cont.run();
                } while (nextCont.isPresent() && executedProgramId != end);

                if (endType == LabelType.section) {
                    while (nextCont.isPresent() && nextCont.get().type == LabelType.label) {
                        CobolControl cont = nextCont.get();
                        nextCont = cont.run();
                    }
                }
                return Optional.of(CobolControl.pure());
            }
        };
    }

    /**
     * PERFORM文に対応する制御ポイントを生成する。
     *
     * <p>単一の段落・節を実行する。COBOL文「PERFORM label-name」に対応。
     * 内部的にはperformThrough(contList, labelId, labelId)として実装。
     *
     * @param contList 全制御ポイントの配列
     * @param labelId 実行するラベルのインデックス
     * @return PERFORM文を表現するCobolControlインスタンス
     */
    public static CobolControl perform(CobolControl[] contList, int labelId) {
        return CobolControl.performThrough(contList, labelId, labelId);
    }
}

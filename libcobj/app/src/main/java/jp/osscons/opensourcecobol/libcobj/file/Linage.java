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
 * COBOL LINAGE句の情報を保持するクラス。
 *
 * <p>COBOL LINAGE句は印刷ファイルの論理ページ構造を定義するために使用される。 このクラスはページ本体行数、フッティング行位置、トップマージン、ボトムマージンなどの
 * 情報を保持し、WRITE文でのページ制御（ADVANCING句）を実現する。
 *
 * <p>LINAGE句の典型的な指定: {@code LINAGE IS 50 LINES WITH FOOTING AT 45 LINES AT TOP 3 LINES AT
 * BOTTOM 2}
 *
 * <p>libcob/common.hのstruct linageに対応する。
 */
public class Linage {
    /** LINAGE値を参照するフィールド（動的参照の場合） */
    private AbstractCobolField linage;

    /** LINAGE-COUNTERレジスタ。現在のページ内行位置を追跡 */
    private AbstractCobolField linageCtr;

    /** FOOTING AT値を参照するフィールド（動的参照の場合） */
    private AbstractCobolField latfoot;

    /** LINES AT TOP値を参照するフィールド（動的参照の場合） */
    private AbstractCobolField lattop;

    /** LINES AT BOTTOM値を参照するフィールド（動的参照の場合） */
    private AbstractCobolField latbot;

    /** ページ本体の行数（LINAGE IS n LINES） */
    private int linLines;

    /** フッティング領域の開始行位置（WITH FOOTING AT n） */
    private int linFoot;

    /** トップマージンの行数（LINES AT TOP n） */
    private int linTop;

    /** ボトムマージンの行数（LINES AT BOTTOM n） */
    private int linBot;

    /**
     * LINAGE参照フィールドを取得する。
     *
     * @return LINAGEフィールド
     */
    AbstractCobolField getLinage() {
        return linage;
    }

    /**
     * LINAGE参照フィールドを設定する。
     *
     * @param linage 設定するLINAGEフィールド
     */
    public void setLinage(AbstractCobolField linage) {
        this.linage = linage;
    }

    /**
     * LINAGE-COUNTERフィールドを取得する。
     *
     * @return LINAGE-COUNTERフィールド
     */
    AbstractCobolField getLinageCtr() {
        return linageCtr;
    }

    /**
     * LINAGE-COUNTERフィールドを設定する。
     *
     * @param linageCtr 設定するLINAGE-COUNTERフィールド
     */
    public void setLinageCtr(AbstractCobolField linageCtr) {
        this.linageCtr = linageCtr;
    }

    /**
     * FOOTING参照フィールドを取得する。
     *
     * @return FOOTINGフィールド
     */
    AbstractCobolField getLatfoot() {
        return latfoot;
    }

    /**
     * FOOTING参照フィールドを設定する。
     *
     * @param latfoot 設定するFOOTINGフィールド
     */
    public void setLatfoot(AbstractCobolField latfoot) {
        this.latfoot = latfoot;
    }

    /**
     * LINES AT TOP参照フィールドを取得する。
     *
     * @return LINES AT TOPフィールド
     */
    AbstractCobolField getLattop() {
        return lattop;
    }

    /**
     * LINES AT TOP参照フィールドを設定する。
     *
     * @param lattop 設定するLINES AT TOPフィールド
     */
    public void setLattop(AbstractCobolField lattop) {
        this.lattop = lattop;
    }

    /**
     * LINES AT BOTTOM参照フィールドを取得する。
     *
     * @return LINES AT BOTTOMフィールド
     */
    AbstractCobolField getLatbot() {
        return latbot;
    }

    /**
     * LINES AT BOTTOM参照フィールドを設定する。
     *
     * @param latbot 設定するLINES AT BOTTOMフィールド
     */
    public void setLatbot(AbstractCobolField latbot) {
        this.latbot = latbot;
    }

    /**
     * ページ本体の行数を取得する。
     *
     * @return ページ本体行数
     */
    int getLinLines() {
        return linLines;
    }

    /**
     * ページ本体の行数を設定する。
     *
     * @param linLines 設定するページ本体行数
     */
    public void setLinLines(int linLines) {
        this.linLines = linLines;
    }

    /**
     * フッティング領域の開始行位置を取得する。
     *
     * @return フッティング開始行位置
     */
    int getLinFoot() {
        return linFoot;
    }

    /**
     * フッティング領域の開始行位置を設定する。
     *
     * @param linFoot 設定するフッティング開始行位置
     */
    public void setLinFoot(int linFoot) {
        this.linFoot = linFoot;
    }

    /**
     * トップマージンの行数を取得する。
     *
     * @return トップマージン行数
     */
    int getLinTop() {
        return linTop;
    }

    /**
     * トップマージンの行数を設定する。
     *
     * @param linTop 設定するトップマージン行数
     */
    public void setLinTop(int linTop) {
        this.linTop = linTop;
    }

    /**
     * ボトムマージンの行数を取得する。
     *
     * @return ボトムマージン行数
     */
    int getLinBot() {
        return linBot;
    }

    /**
     * ボトムマージンの行数を設定する。
     *
     * @param linBot 設定するボトムマージン行数
     */
    public void setLinBot(int linBot) {
        this.linBot = linBot;
    }
}

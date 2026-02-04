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
package jp.osscons.opensourcecobol.libcobj.termio;

import java.io.InputStreamReader;
import java.io.PrintStream;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.charset.UnsupportedCharsetException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Scanner;
import jp.osscons.opensourcecobol.libcobj.common.CobolEncoding;
import jp.osscons.opensourcecobol.libcobj.common.CobolModule;
import jp.osscons.opensourcecobol.libcobj.common.CobolUtil;
import jp.osscons.opensourcecobol.libcobj.data.AbstractCobolField;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldAttribute;
import jp.osscons.opensourcecobol.libcobj.data.CobolFieldFactory;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolExceptionId;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolExceptionInfo;

/**
 * COBOLのDISPLAY文およびACCEPT文の実行時処理を提供するクラス。
 *
 * <p>このクラスは、COBOLプログラムからJavaに変換された際に使用される静的メソッドを提供し、
 * 標準入出力への表示、日時情報の取得、環境変数の操作、コマンドライン引数の処理を行う。
 * COBOLの特殊レジスタ（DATE, TIME, DAY等）やACCEPT文のFROM句、DISPLAY文のUPON句に
 * 対応するメソッドを実装している。
 *
 * <p>主な機能:
 *
 * <ul>
 *   <li>DISPLAY文: {@link #display(boolean, boolean, AbstractCobolField...)} による標準出力/標準エラー出力
 *   <li>ACCEPT文: {@link #accept(AbstractCobolField)} による標準入力からのデータ受け取り
 *   <li>日時取得: {@link #acceptDate}, {@link #acceptTime} などによるシステム日時の取得
 *   <li>環境変数: {@link #displayEnvironment}, {@link #acceptEnvironment} による環境変数の設定/取得
 *   <li>コマンドライン: {@link #acceptCommandLine}, {@link #acceptArgValue} によるコマンドライン引数の取得
 * </ul>
 */
public class CobolTerminal {

    /**
     * DISPLAY UPON COMMAND-LINE で設定されたコマンドライン文字列の長さ。
     *
     * <p>{@link #displayCommandLine} で設定され、{@link #acceptCommandLine} で参照される。
     */
    private static int commlncnt = 0;

    /**
     * DISPLAY UPON COMMAND-LINE で設定されたコマンドライン文字列のバイト配列。
     *
     * <p>{@link #displayCommandLine} で設定され、{@link #acceptCommandLine} で参照される。
     */
    private static byte[] commlnptr = null;

    /**
     * 標準出力または標準エラー出力にデータを出力する
     *
     * @param dispStdout trueなら標準出力に,それ以外は標準エラー出力に出力する
     * @param newline trueなら出力後に改行しない,それ以外の場合は改行する
     * @param fields 出力するCOBOL変数(可変長)
     */
    public static void display(boolean dispStdout, boolean newline, AbstractCobolField... fields) {
        PrintStream stream = dispStdout ? System.out : System.err;
        for (AbstractCobolField field : fields) {
            CobolFieldAttribute attr = field.getAttribute();
            if (attr.isTypeNumericBinary()
                    && CobolModule.getCurrentModule().flag_pretty_display == 0) {
                stream.print(field);
            } else if (attr.isTypeNumeric()) {
                stream.print(field);
            } else {
                displayAlnum(field, stream);
            }
        }
        if (newline) {
            stream.println("");
        }
    }

    private static void displayAlnum(AbstractCobolField f, PrintStream stream) {
        CobolDataStorage storage = f.getDataStorage();
        if (CobolUtil.terminalEncoding == CobolEncoding.UTF8) {
            byte[] utf8Bytes =
                    new String(storage.getByteArray(0, f.getSize()), AbstractCobolField.charSetSJIS)
                            .getBytes(StandardCharsets.UTF_8);
            stream.write(utf8Bytes, 0, utf8Bytes.length);
        } else {
            stream.write(storage.getRefOfData(), storage.getIndex(), f.getSize());
        }
    }

    /**
     * 標準出力または標準エラー出力にデータを出力する
     *
     * @param outorerr 0なら標準出力に,それ以外は標準エラー出力に出力する.
     * @param newline 0なら出力後に改行しない,それ以外の場合は改行する
     * @param varcnt 出力するCOBOL変数の数
     * @param fields 出力するCOBOL変数(可変長)
     */
    public static void display(
            int outorerr, int newline, int varcnt, AbstractCobolField... fields) {
        PrintStream stream = outorerr == 0 ? System.out : System.err;
        for (AbstractCobolField field : fields) {
            CobolFieldAttribute attr = field.getAttribute();
            if (attr.isTypeNumericBinary()
                    && CobolModule.getCurrentModule().flag_pretty_display == 0) {
                stream.print(field);
            } else if (attr.isTypeNumeric()) {
                stream.print(field);
            } else {
                displayAlnum(field, stream);
            }
        }
        if (newline == 1) {
            stream.println("");
        }
    }

    private static Scanner scan = null;

    /**
     * 標準入力からデータを受け取り、指定されたCOBOLフィールドに格納する。
     *
     * <p>COBOLの {@code ACCEPT identifier} 文に対応する。標準入力から1行読み込み、
     * 読み込んだデータを指定されたフィールドに移送する。文字エンコーディングは {@link
     * CobolUtil#terminalEncoding} の設定に従う。
     *
     * @param f 入力データを格納するCOBOLフィールド
     */
    public static void accept(AbstractCobolField f) {
        try {
            if (scan == null) {
                if (CobolUtil.terminalEncoding == CobolEncoding.UTF8) {
                    scan = new Scanner(new InputStreamReader(System.in, StandardCharsets.UTF_8));
                } else {
                    try {
                        scan =
                                new Scanner(
                                        new InputStreamReader(
                                                System.in, Charset.forName("Shift_JIS")));
                    } catch (UnsupportedCharsetException e) {
                        scan = new Scanner(System.in);
                    }
                }
            }

            String input = scan.nextLine();

            // PIC X(n)型のデータに変換
            AbstractCobolField field = CobolFieldFactory.makeCobolField(input);

            if (f.getAttribute().isTypeNumericDisplay() && field.getSize() > f.getSize()) {
                field.setSize(f.getSize());
            }

            f.moveFrom(field);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // Time

    /**
     * ジョブ時刻または現在時刻を取得する。
     *
     * <p>opensource COBOL 4Jの内部関数。{@link CobolUtil#cobLocalTm} が設定されている場合はその値を返し、
     * 設定されていない場合は現在の日時を返す。これにより、テストや特定のジョブ実行時に 日時を固定することが可能になる。
     *
     * @return ジョブ時刻が設定されている場合はその値、そうでなければ現在の日時
     */
    private static LocalDateTime jobOrCurrentLocalTime() {
        if (CobolUtil.cobLocalTm != null) {
            return CobolUtil.cobLocalTm;
        } else {
            return LocalDateTime.now();
        }
    }

    /**
     * 現在の日付をYYMMDD形式で取得し、指定されたCOBOLフィールドに格納する。
     *
     * <p>COBOLの {@code ACCEPT identifier FROM DATE} 文に対応する。
     * 日付は6桁の数字（YYMMDD形式: 2桁の年、2桁の月、2桁の日）で返される。
     *
     * @param f 日付を格納するCOBOLフィールド（6桁以上の数字フィールド、例: PIC 9(6)）
     */
    public static void acceptDate(AbstractCobolField f) {
        LocalDateTime date = jobOrCurrentLocalTime();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyMMdd");
        f.memcpy(date.format(formatter));
    }

    /**
     * 現在の日付をYYYYMMDD形式で取得し、指定されたCOBOLフィールドに格納する。
     *
     * <p>COBOLの {@code ACCEPT identifier FROM DATE YYYYMMDD} 文に対応する。
     * 日付は8桁の数字（YYYYMMDD形式: 4桁の年、2桁の月、2桁の日）で返される。
     *
     * @param f 日付を格納するCOBOLフィールド（8桁以上の数字フィールド、例: PIC 9(8)）
     */
    public static void acceptDate_yyyymmdd(AbstractCobolField f) {
        LocalDateTime date = jobOrCurrentLocalTime();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyyMMdd");
        f.memcpy(date.format(formatter));
    }

    /**
     * 現在の年間通日をYYDDD形式で取得し、指定されたCOBOLフィールドに格納する。
     *
     * <p>COBOLの {@code ACCEPT identifier FROM DAY} 文に対応する。
     * 日付は5桁の数字（YYDDD形式: 2桁の年、3桁の年間通日）で返される。
     * 年間通日は1月1日を001として、12月31日が365（閏年は366）となる。
     *
     * @param f 年間通日を格納するCOBOLフィールド（5桁以上の数字フィールド、例: PIC 9(5)）
     */
    public static void acceptDay(AbstractCobolField f) {
        LocalDateTime date = jobOrCurrentLocalTime();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyDDD");
        f.memcpy(date.format(formatter));
    }

    /**
     * 現在の年間通日をYYYYDDD形式で取得し、指定されたCOBOLフィールドに格納する。
     *
     * <p>COBOLの {@code ACCEPT identifier FROM DAY YYYYDDD} 文に対応する。
     * 日付は7桁の数字（YYYYDDD形式: 4桁の年、3桁の年間通日）で返される。
     * 年間通日は1月1日を001として、12月31日が365（閏年は366）となる。
     *
     * @param f 年間通日を格納するCOBOLフィールド（7桁以上の数字フィールド、例: PIC 9(7)）
     */
    public static void acceptDay_yyyyddd(AbstractCobolField f) {
        LocalDateTime date = jobOrCurrentLocalTime();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyyDDD");
        f.memcpy(date.format(formatter));
    }

    /**
     * 現在の曜日を1～7の数値で取得し、指定されたCOBOLフィールドに格納する。
     *
     * <p>COBOLの {@code ACCEPT identifier FROM DAY-OF-WEEK} 文に対応する。
     * 曜日は1桁の数字で返される（1=月曜日、2=火曜日、...、7=日曜日）。
     *
     * @param f 曜日を格納するCOBOLフィールド（1桁以上の数字フィールド、例: PIC 9(1)）
     */
    public static void acceptDayOfWeek(AbstractCobolField f) {
        LocalDateTime date = jobOrCurrentLocalTime();
        f.memcpy(String.format("%d", date.getDayOfWeek().getValue()));
    }

    /**
     * 現在の時刻をHHMMSSCC形式で取得し、指定されたCOBOLフィールドに格納する。
     *
     * <p>COBOLの {@code ACCEPT identifier FROM TIME} 文に対応する。
     * 時刻は8桁の数字（HHMMSSCC形式: 2桁の時、2桁の分、2桁の秒、2桁の1/100秒）で返される。
     * このメソッドは常に現在時刻を使用し、{@link CobolUtil#cobLocalTm} の設定は考慮しない。
     *
     * @param f 時刻を格納するCOBOLフィールド（8桁以上の数字フィールド、例: PIC 9(8)）
     */
    public static void acceptTime(AbstractCobolField f) {
        LocalDateTime date = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("HHmmssSS");
        f.memcpy(date.format(formatter));
    }

    // Environment

    /**
     * 環境変数名を設定する。
     *
     * <p>COBOLの {@code DISPLAY identifier UPON ENVIRONMENT-NAME} 文に対応する。
     * 指定されたフィールドの値を環境変数名として {@link CobolUtil#cobLocalEnv} に保存する。 この後 {@link
     * #displayEnvValue} で値を設定するか、{@link #acceptEnvironment} で値を取得できる。
     *
     * @param f 環境変数名を含むCOBOLフィールド
     */
    public static void displayEnvironment(AbstractCobolField f) {
        CobolUtil.cobLocalEnv = f.fieldToString();
    }

    /**
     * 環境変数に値を設定する。
     *
     * <p>COBOLの {@code DISPLAY identifier UPON ENVIRONMENT-VALUE} 文に対応する。 事前に {@link
     * #displayEnvironment} で設定された環境変数名に対して、指定されたフィールドの値を設定する。
     * 環境変数名が設定されていない場合は、{@link CobolExceptionId#COB_EC_IMP_DISPLAY} 例外が設定される。
     *
     * @param f 環境変数に設定する値を含むCOBOLフィールド
     */
    public static void displayEnvValue(AbstractCobolField f) {
        if (CobolUtil.cobLocalEnv == null || CobolUtil.cobLocalEnv.equals("")) {
            CobolExceptionInfo.setException(CobolExceptionId.COB_EC_IMP_DISPLAY);
            return;
        }
        CobolUtil.setEnv(CobolUtil.cobLocalEnv, f.getString());
    }

    /**
     * 環境変数の値を取得し、指定されたCOBOLフィールドに格納する。
     *
     * <p>COBOLの {@code ACCEPT identifier FROM ENVIRONMENT-VALUE} 文に対応する。 事前に {@link
     * #displayEnvironment} で設定された環境変数名の値を取得し、指定されたフィールドに格納する。
     * 環境変数が存在しない場合は、{@link CobolExceptionId#COB_EC_IMP_ACCEPT} 例外が設定され、 フィールドには空白が格納される。
     *
     * @param f 環境変数の値を格納するCOBOLフィールド
     */
    public static void acceptEnvironment(AbstractCobolField f) {
        String p = null;
        if (CobolUtil.cobLocalEnv != null) {
            p = CobolUtil.getEnv(CobolUtil.cobLocalEnv);
        }

        if (p == null) {
            // TODO setExceptionは暫定実装
            CobolExceptionInfo.setException(CobolExceptionId.COB_EC_IMP_ACCEPT);
            p = " ";
        }

        AbstractCobolField src = CobolFieldFactory.makeCobolField(p);
        // TODO hankakuMoveFromは暫定実装
        f.hankakuMoveFrom(src);
    }

    /**
     * コマンドライン文字列を設定する。
     *
     * <p>COBOLの {@code DISPLAY identifier UPON COMMAND-LINE} 文に対応する。
     * 指定されたフィールドの内容をコマンドライン文字列として内部バッファ（{@link #commlnptr}）に保存する。 保存された値は後続の {@link
     * #acceptCommandLine} 呼び出しで取得できる。
     *
     * @param f コマンドライン文字列を含むCOBOLフィールド
     */
    public static void displayCommandLine(AbstractCobolField f) {
        CobolTerminal.commlnptr = new byte[f.getSize()];
        CobolTerminal.commlncnt = f.getSize();
        for (int i = 0; i < CobolTerminal.commlncnt; ++i) {
            CobolTerminal.commlnptr[i] = f.getDataStorage().getByte(i);
        }
    }

    /**
     * コマンドライン文字列を取得し、指定されたCOBOLフィールドに格納する。
     *
     * <p>COBOLの {@code ACCEPT identifier FROM COMMAND-LINE} 文に対応する。 {@link #displayCommandLine}
     * で設定されたコマンドライン文字列がある場合はその値を返し、 ない場合はプログラム起動時のコマンドライン引数（{@link
     * CobolUtil#commandLineArgs}）を スペースで連結した文字列を返す。
     *
     * @param f コマンドライン文字列を格納するCOBOLフィールド
     */
    public static void acceptCommandLine(AbstractCobolField f) {
        if (CobolTerminal.commlncnt != 0) {
            f.memcpy(CobolTerminal.commlnptr, CobolTerminal.commlncnt);
            return;
        }

        f.memcpy(String.join(" ", CobolUtil.commandLineArgs));
    }

    /**
     * 現在の引数インデックスを設定する。
     *
     * <p>COBOLの {@code DISPLAY identifier UPON ARGUMENT-NUMBER} 文に対応する。 指定されたフィールドの値を現在の引数インデックス（{@link
     * CobolUtil#currentArgIndex}）として設定する。 このインデックスは後続の {@link #acceptArgValue}
     * 呼び出しで参照される引数の位置を決定する。 値が0未満または引数の総数を超える場合は、{@link CobolExceptionId#COB_EC_IMP_DISPLAY}
     * 例外が設定される。
     *
     * @param f 引数インデックス（0から引数の総数までの整数）を含むCOBOLフィールド
     */
    public static void displayArgNumber(AbstractCobolField f) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 9, 0, 0, null);
        byte[] data = new byte[4];
        AbstractCobolField temp =
                CobolFieldFactory.makeCobolField(data.length, new CobolDataStorage(data), attr);
        temp.moveFrom(f);
        int n = ByteBuffer.wrap(data).getInt();
        if (n < 0 || n > CobolUtil.commandLineArgs.length) {
            CobolExceptionInfo.setException(CobolExceptionId.COB_EC_IMP_DISPLAY);
            return;
        }
        CobolUtil.currentArgIndex = n;
    }

    /**
     * コマンドライン引数の総数を取得し、指定されたCOBOLフィールドに格納する。
     *
     * <p>COBOLの {@code ACCEPT identifier FROM ARGUMENT-NUMBER} 文に対応する。 プログラム起動時に渡されたコマンドライン引数（{@link
     * CobolUtil#commandLineArgs}）の 総数を取得し、指定されたフィールドに格納する。
     *
     * @param f 引数の総数を格納するCOBOLフィールド
     */
    public static void acceptArgNumber(AbstractCobolField f) {
        CobolFieldAttribute attr =
                new CobolFieldAttribute(CobolFieldAttribute.COB_TYPE_NUMERIC_BINARY, 9, 0, 0, null);
        byte[] data = new byte[4];
        ByteBuffer.wrap(data).putInt(CobolUtil.commandLineArgs.length);
        AbstractCobolField temp =
                CobolFieldFactory.makeCobolField(data.length, new CobolDataStorage(data), attr);
        f.moveFrom(temp);
    }

    /**
     * 現在の引数インデックスに対応するコマンドライン引数の値を取得し、指定されたCOBOLフィールドに格納する。
     *
     * <p>COBOLの {@code ACCEPT identifier FROM ARGUMENT-VALUE} 文に対応する。 {@link #displayArgNumber}
     * で設定された引数インデックス（{@link CobolUtil#currentArgIndex}）に対応する 引数の値を取得し、指定されたフィールドに格納する。取得後、引数インデックスは自動的に1増加する。
     * インデックスが引数の総数を超えている場合は、{@link CobolExceptionId#COB_EC_IMP_ACCEPT} 例外が設定される。
     *
     * @param f 引数の値を格納するCOBOLフィールド
     */
    public static void acceptArgValue(AbstractCobolField f) {
        if (CobolUtil.currentArgIndex > CobolUtil.commandLineArgs.length) {
            CobolExceptionInfo.setException(CobolExceptionId.COB_EC_IMP_ACCEPT);
            return;
        }
        f.memcpy(CobolUtil.commandLineArgs[CobolUtil.currentArgIndex - 1]);
        CobolUtil.currentArgIndex++;
    }
}

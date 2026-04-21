package jp.osscons.opensourcecobol.libcobj.sql;

import java.nio.ByteBuffer;
import jp.osscons.opensourcecobol.libcobj.call.CobolResolve;
import jp.osscons.opensourcecobol.libcobj.call.CobolRunnable;
import jp.osscons.opensourcecobol.libcobj.data.CobolDataStorage;
import jp.osscons.opensourcecobol.libcobj.exceptions.CobolRuntimeException;

public class CobolSql {

    private static CobolRunnable resolveRunner(String name) {
        try {
            return CobolResolve.resolve(null, name);
        } catch (CobolRuntimeException e) {
            throw new RuntimeException("Failed to resolve ESQL runtime: " + name, e);
        }
    }

    private static int callRunner(String name, CobolDataStorage... args) {
        CobolRunnable runner = resolveRunner(name);
        return runner.run(args);
    }

    private static CobolDataStorage intToStorage(int value) {
        CobolDataStorage s = new CobolDataStorage(4);
        byte[] bytes = ByteBuffer.allocate(4).putInt(value).array();
        s.setBytes(bytes, 4);
        return s;
    }

    private static CobolDataStorage stringToNullTerminated(String str) {
        byte[] data = str.getBytes();
        CobolDataStorage s = new CobolDataStorage(data.length + 1);
        s.setBytes(data, data.length);
        s.setByte(data.length, (byte) 0);
        return s;
    }

    public static void startSQL(CobolDataStorage sqlca) {
        callRunner("OCESQLStartSQL");
    }

    public static void endSQL(CobolDataStorage sqlca) {
        callRunner("OCESQLEndSQL");
    }

    public static void setParam(int type, int length, int scale, CobolDataStorage data) {
        callRunner(
                "OCESQLSetSQLParams",
                intToStorage(type),
                intToStorage(length),
                intToStorage(scale),
                data);
    }

    public static void setResultParam(int type, int length, int scale, CobolDataStorage data) {
        callRunner(
                "OCESQLSetResultParams",
                intToStorage(type),
                intToStorage(length),
                intToStorage(scale),
                data);
    }

    public static void exec(CobolDataStorage sqlca, String query) {
        CobolDataStorage queryStorage = stringToNullTerminated(query);
        callRunner("OCESQLExec", sqlca, queryStorage);
    }

    public static void execParams(CobolDataStorage sqlca, String query, int nParams) {
        CobolDataStorage queryStorage = stringToNullTerminated(query);
        callRunner("OCESQLExecParams", sqlca, queryStorage, intToStorage(nParams));
    }

    public static void execSelectIntoOne(
            CobolDataStorage sqlca, String query, int nParams, int nResults) {
        CobolDataStorage queryStorage = stringToNullTerminated(query);
        callRunner(
                "OCESQLExecSelectIntoOne",
                sqlca,
                queryStorage,
                intToStorage(nParams),
                intToStorage(nResults));
    }

    public static void execSelectIntoOccurs(
            CobolDataStorage sqlca, String query, int nParams, int nResults) {
        CobolDataStorage queryStorage = stringToNullTerminated(query);
        callRunner(
                "OCESQLExecSelectIntoOccurs",
                sqlca,
                queryStorage,
                intToStorage(nParams),
                intToStorage(nResults));
    }

    public static void connect(
            CobolDataStorage sqlca,
            CobolDataStorage user,
            int userLen,
            CobolDataStorage passwd,
            int passwdLen,
            CobolDataStorage dbname,
            int dbnameLen) {
        callRunner(
                "OCESQLConnect",
                sqlca,
                user,
                intToStorage(userLen),
                passwd,
                intToStorage(passwdLen),
                dbname,
                intToStorage(dbnameLen));
    }

    public static void connectInformal(CobolDataStorage sqlca, CobolDataStorage user, int userLen) {
        callRunner("OCESQLConnectInformal", sqlca, user, intToStorage(userLen));
    }

    public static void connectShort(CobolDataStorage sqlca) {
        callRunner("OCESQLConnectShort", sqlca);
    }

    public static void disconnect(CobolDataStorage sqlca) {
        callRunner("OCESQLDisconnect", sqlca);
    }

    public static void cursorDeclare(CobolDataStorage sqlca, String cursorName, String query) {
        callRunner(
                "OCESQLCursorDeclare",
                sqlca,
                stringToNullTerminated(cursorName),
                stringToNullTerminated(query));
    }

    public static void cursorDeclareParams(
            CobolDataStorage sqlca, String cursorName, String query, int nParams) {
        callRunner(
                "OCESQLCursorDeclareParams",
                sqlca,
                stringToNullTerminated(cursorName),
                stringToNullTerminated(query),
                intToStorage(nParams));
    }

    public static void cursorOpen(CobolDataStorage sqlca, String cursorName) {
        callRunner("OCESQLCursorOpen", sqlca, stringToNullTerminated(cursorName));
    }

    public static void cursorOpenParams(CobolDataStorage sqlca, String cursorName, int nParams) {
        callRunner(
                "OCESQLCursorOpenParams",
                sqlca,
                stringToNullTerminated(cursorName),
                intToStorage(nParams));
    }

    public static void cursorFetchOne(CobolDataStorage sqlca, String cursorName) {
        callRunner("OCESQLCursorFetchOne", sqlca, stringToNullTerminated(cursorName));
    }

    public static void cursorFetchOccurs(CobolDataStorage sqlca, String cursorName) {
        callRunner("OCESQLCursorFetchOccurs", sqlca, stringToNullTerminated(cursorName));
    }

    public static void cursorClose(CobolDataStorage sqlca, String cursorName) {
        callRunner("OCESQLCursorClose", sqlca, stringToNullTerminated(cursorName));
    }

    public static void prepare(
            CobolDataStorage sqlca, String stmtName, CobolDataStorage queryStorage, int queryLen) {
        callRunner(
                "OCESQLPrepare",
                sqlca,
                stringToNullTerminated(stmtName),
                queryStorage,
                intToStorage(queryLen));
    }

    public static void execPrepare(CobolDataStorage sqlca, String stmtName, int nParams) {
        callRunner(
                "OCESQLExecPrepare",
                sqlca,
                stringToNullTerminated(stmtName),
                intToStorage(nParams));
    }

    public static void commit(CobolDataStorage sqlca) {
        callRunner("OCESQLCommit", sqlca);
    }

    public static void rollback(CobolDataStorage sqlca) {
        callRunner("OCESQLRollback", sqlca);
    }

    /* AT database variants (ID prefixed) */
    public static void idConnect(
            CobolDataStorage sqlca,
            CobolDataStorage atdb,
            int atdbLen,
            CobolDataStorage user,
            int userLen,
            CobolDataStorage passwd,
            int passwdLen,
            CobolDataStorage dbname,
            int dbnameLen) {
        callRunner(
                "OCESQLIDConnect",
                sqlca,
                atdb,
                intToStorage(atdbLen),
                user,
                intToStorage(userLen),
                passwd,
                intToStorage(passwdLen),
                dbname,
                intToStorage(dbnameLen));
    }

    public static void idExec(
            CobolDataStorage sqlca, CobolDataStorage atdb, int atdbLen, String query) {
        callRunner(
                "OCESQLIDExec", sqlca, atdb, intToStorage(atdbLen), stringToNullTerminated(query));
    }

    public static void idExecParams(
            CobolDataStorage sqlca, CobolDataStorage atdb, int atdbLen, String query, int nParams) {
        callRunner(
                "OCESQLIDExecParams",
                sqlca,
                atdb,
                intToStorage(atdbLen),
                stringToNullTerminated(query),
                intToStorage(nParams));
    }

    public static void idDisconnect(CobolDataStorage sqlca, CobolDataStorage atdb, int atdbLen) {
        callRunner("OCESQLIDDisconnect", sqlca, atdb, intToStorage(atdbLen));
    }

    public static void idCommit(CobolDataStorage sqlca, CobolDataStorage atdb, int atdbLen) {
        callRunner("OCESQLIDCommit", sqlca, atdb, intToStorage(atdbLen));
    }

    public static void idRollback(CobolDataStorage sqlca, CobolDataStorage atdb, int atdbLen) {
        callRunner("OCESQLIDRollback", sqlca, atdb, intToStorage(atdbLen));
    }
}

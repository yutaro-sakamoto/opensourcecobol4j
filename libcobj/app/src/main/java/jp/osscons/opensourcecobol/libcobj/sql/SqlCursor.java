package jp.osscons.opensourcecobol.libcobj.sql;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class SqlCursor {

    String name;
    String query;
    int nParams;
    boolean isOpened;
    SqlParam[] params;

    public SqlCursor(String name, String query, int nParams) {
        this.name = name;
        this.query = query;
        this.nParams = nParams;
        this.isOpened = false;
        this.params = null;
    }

    public void open(Connection conn, SqlParam[] openParams) throws SQLException {
        String command = "DECLARE " + name + " CURSOR FOR " + query;

        if (openParams != null && openParams.length > 0) {
            PreparedStatement pstmt = conn.prepareStatement(command);
            try {
                java.sql.ParameterMetaData metaData = pstmt.getParameterMetaData();
                for (int i = 0; i < openParams.length; i++) {
                    CobolDataConverter.setParam(pstmt, i + 1, metaData, openParams[i]);
                }
                pstmt.execute();
            } finally {
                pstmt.close();
            }
        } else if (this.params != null && this.params.length > 0) {
            PreparedStatement pstmt = conn.prepareStatement(command);
            try {
                java.sql.ParameterMetaData metaData = pstmt.getParameterMetaData();
                for (int i = 0; i < this.params.length; i++) {
                    CobolDataConverter.setParam(pstmt, i + 1, metaData, this.params[i]);
                }
                pstmt.execute();
            } finally {
                pstmt.close();
            }
        } else {
            Statement stmt = conn.createStatement();
            try {
                stmt.execute(command);
            } finally {
                stmt.close();
            }
        }
        isOpened = true;
    }

    public boolean fetch(Connection conn, SqlParam[] resultParams) throws SQLException {
        String fetchSql = "FETCH FORWARD 1 FROM " + name;
        Statement stmt = conn.createStatement();
        try {
            boolean hasResult = stmt.execute(fetchSql);
            if (!hasResult) {
                return false;
            }
            ResultSet rs = stmt.getResultSet();
            if (rs == null || !rs.next()) {
                if (rs != null) rs.close();
                return false;
            }

            if (resultParams != null) {
                int columnCount = rs.getMetaData().getColumnCount();
                for (int i = 0; i < resultParams.length && i < columnCount; i++) {
                    byte[] value = CobolDataConverter.getValueFromResultSet(rs, i + 1);
                    if (value != null) {
                        CobolDataConverter.stringToCobol(resultParams[i], value);
                    } else {
                        resultParams[i].storage.memset((byte) 0, resultParams[i].length);
                    }
                }
            }
            rs.close();
            return true;
        } finally {
            stmt.close();
        }
    }

    public void close(Connection conn) throws SQLException {
        Statement stmt = conn.createStatement();
        try {
            stmt.execute("CLOSE " + name);
        } finally {
            stmt.close();
        }
        isOpened = false;
    }
}

import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;

/**
 * Servlet that runs the COBOL4J-generated INDEXED-file program (idxopen) inside
 * a real Tomcat web application, to reproduce
 * opensourcecobol/opensourcecobol4j#213.
 *
 * The generated program's DISPLAY statements go through CobolTerminal, which
 * writes to System.out at call time; this servlet temporarily redirects
 * System.out to a buffer so the "OPEN OUTPUT STATUS=xx" line can be returned in
 * the HTTP response and inspected by the CI script.
 *
 * On Tomcat the bundled SQLite JDBC driver (WEB-INF/lib/libcobj.jar) is never
 * registered with DriverManager -- Tomcat's JreMemoryLeakPreventionListener
 * initialises DriverManager under the shared class loader at startup -- so the
 * INDEXED OPEN fails with STATUS=30 instead of STATUS=00.
 */
public class IndexedServlet extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        resp.setContentType("text/plain; charset=UTF-8");

        ByteArrayOutputStream buf = new ByteArrayOutputStream();
        Throwable error = null;

        // System.setOut is JVM-global; serialise so concurrent requests do not
        // capture each other's output.
        synchronized (IndexedServlet.class) {
            PrintStream originalOut = System.out;
            try (PrintStream capture = new PrintStream(buf, true, StandardCharsets.UTF_8)) {
                System.setOut(capture);
                idxopen.main(new String[0]);
            } catch (Throwable t) {
                error = t;
            } finally {
                System.setOut(originalOut);
            }
        }

        PrintWriter w = resp.getWriter();
        w.print(buf.toString(StandardCharsets.UTF_8));
        if (error != null) {
            w.println("EXCEPTION: " + error);
        }
    }
}

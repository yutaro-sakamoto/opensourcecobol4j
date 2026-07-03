import java.io.File;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
import java.sql.DriverManager;
import java.util.Enumeration;

/**
 * Reproduces opensourcecobol/opensourcecobol4j#213 (Tomcat INDEXED file OPEN failure)
 * without a full servlet container.
 *
 * Tomcat isolates each web application in its own class loader (WEB-INF/lib),
 * while java.sql.DriverManager lives on the system/bootstrap class loader and
 * runs its one-time ServiceLoader driver scan (loadInitialDrivers) using the
 * class loader active at container startup -- which cannot see the web app's
 * bundled SQLite JDBC driver. As a result the JDBC 4.0 auto-registration
 * (META-INF/services/java.sql.Driver) never registers org.sqlite.JDBC, and
 * DriverManager.getConnection("jdbc:sqlite:...") fails with
 * "No suitable driver found".
 *
 * This launcher is started with ONLY itself on the classpath (no libcobj.jar),
 * so:
 *   1. Touching DriverManager here forces its ServiceLoader scan under a class
 *      loader that has no SQLite driver -> nothing registered.
 *   2. libcobj.jar + the generated COBOL program are then loaded in a child
 *      URLClassLoader (the simulated "web app" class loader).
 *   3. Running the program calls CobolIndexedFile.getConnection ->
 *      DriverManager.getConnection -> fails, exactly like on Tomcat.
 */
public class TomcatSim {
    public static void main(String[] args) throws Exception {
        String libcobjJar = args[0];
        String progDir = args[1];
        String progClass = args[2];

        // Step 1: force DriverManager's one-time driver scan now, while the
        // SQLite driver is NOT visible (mirrors Tomcat container startup).
        DriverManager.getDrivers();
        System.out.println("[TomcatSim] DriverManager initialized by system class loader.");
        System.out.print("[TomcatSim] Registered JDBC drivers visible to DriverManager:");
        Enumeration<java.sql.Driver> drivers = DriverManager.getDrivers();
        boolean any = false;
        while (drivers.hasMoreElements()) {
            any = true;
            System.out.print(" " + drivers.nextElement().getClass().getName());
        }
        System.out.println(any ? "" : " (none)");

        // Diagnostic: show the exact JDBC failure reported in issue #213.
        try {
            DriverManager.getConnection("jdbc:sqlite:testidx.dat");
        } catch (java.sql.SQLException e) {
            System.out.println("[TomcatSim] DriverManager.getConnection(jdbc:sqlite:...) -> "
                + e.getClass().getName() + ": " + e.getMessage());
        }

        // Step 2: build the isolated "web app" class loader containing
        // libcobj.jar (with the SQLite driver) and the generated program.
        URL[] urls = new URL[] {
            new File(libcobjJar).toURI().toURL(),
            new File(progDir).toURI().toURL()
        };
        URLClassLoader webappLoader =
            new URLClassLoader(urls, TomcatSim.class.getClassLoader());
        System.out.println("[TomcatSim] Running generated COBOL program '" + progClass
            + "' under the isolated web-app class loader...");

        // Step 3: run the generated program's main() reflectively in that loader.
        Thread.currentThread().setContextClassLoader(webappLoader);
        Class<?> prog = Class.forName(progClass, true, webappLoader);
        Method main = prog.getMethod("main", String[].class);
        main.invoke(null, (Object) new String[0]);
    }
}

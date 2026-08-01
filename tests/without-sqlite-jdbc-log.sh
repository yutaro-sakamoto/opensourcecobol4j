# Run a command with the sqlite-jdbc clean-up message removed from its stderr.
#
# Usage: sh without-sqlite-jdbc-log.sh COMMAND [ARG...]
#
# INDEXED files are backed by sqlite-jdbc, which unpacks its native library
# into the shared temporary directory on the first connection and, before
# doing so, deletes the leftovers of processes that already exited. When
# several COBOL programs using INDEXED files start at the same time, two of
# them can pick the same leftover to delete; the loser only finds the file
# gone, but reports that as
#
#   [main] ERROR org.sqlite.SQLiteJDBCLoader - Failed to delete old native lib
#   java.nio.file.NoSuchFileException: /tmp/sqlite-...-libsqlitejdbc.so
#           at ...
#
# on stderr, followed by the whole stack trace. The clean-up is best effort
# and its outcome does not affect the COBOL program, but autotest requires an
# empty stderr, so a test group running two JVMs at once fails on the message
# alone. It shows up once the file lock suites are run with -j, which puts
# enough JVMs in flight for the clean-ups to collide.
#
# Drop exactly that message and the stack trace that follows it. Everything
# else the command writes to stderr is passed through, so any other unexpected
# output still fails the test. The exit status of COMMAND is preserved.

# Several of these wrappers can run at once in one directory, so keep the
# name of the spool file unique to this process.
stderr_file=without-sqlite-jdbc-log.$$.stderr

"$@" 2>"$stderr_file"
command_status=$?

tab=$(printf '\t')
skipping=no

# What is passed through has to be byte for byte what the command wrote, or a
# test could pass on stderr that does not really match. `read` reports failure
# on a last line that is not newline terminated, so pick the line terminator up
# from whether it succeeded, and keep going once for that final line.
newline='
'
terminator=$newline

while IFS= read -r line || { terminator=''; test -n "$line"; }; do
    if test "$skipping" = yes; then
        # Swallow the exception and the stack trace reporting the failure.
        case $line in
            java.* | "Caused by: "* | "$tab"at\ * | "$tab"...\ * | "$tab"Suppressed:\ *)
                continue
                ;;
            *)
                skipping=no
                ;;
        esac
    fi

    case $line in
        *"ERROR org.sqlite.SQLiteJDBCLoader - Failed to delete old native lib")
            skipping=yes
            continue
            ;;
    esac

    printf '%s%s' "$line" "$terminator"
done <"$stderr_file" >&2

rm -f "$stderr_file"

exit $command_status

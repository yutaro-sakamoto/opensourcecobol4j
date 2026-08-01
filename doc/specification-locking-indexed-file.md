# INDEXED File Locking Specification

## LOCK MODE in the SELECT Statement

- The SELECT statement's LOCK MODE can only specify AUTOMATIC or MANUAL.
- When omitted, the default value for LOCK MODE is MANUAL
  - When `-lock-mode-automatic` is specified at compile time, the default value for omitted LOCK MODE becomes AUTOMATIC.

## Record Locking Behavior

- When a file is opened with OPEN I-O and a READ statement without WITH NO LOCK or WITH LOCK is executed:
  - If the SELECT statement's LOCK MODE is MANUAL, the record is not locked
  - If the SELECT statement's LOCK MODE is AUTOMATIC, the record is locked
- When a file is opened with OPEN I-O and a READ statement with WITH LOCK is executed, the record is locked
- When a file is opened with OPEN I-O and a READ statement with WITH NO LOCK is executed, the record is not locked
- When processing fails due to record locking, the file status becomes 51

## File Locking Behavior

- When a file is opened with OPEN OUTPUT, a file lock is applied to the file
- When processing fails due to file locking, the file status becomes 61

## Migrating Legacy INDEXED Files to the New Version

Legacy INDEXED files cannot be used directly with the new version of opensource COBOL 4J.
To convert INDEXED files created with legacy versions to be compatible with the new version, please use the following command:

```sh
cobj-idx migrate <<INDEXED_FILE>>
```

This command converts the specified INDEXED file to be compatible with the new version.

## Unlocking INDEXED Files

The following command can release all file locks and record locks on an INDEXED file:

```sh
cobj-idx unlock <<INDEXED_FILE>>
```

## Behavior When Opening Legacy INDEXED Files

When attempting to open legacy INDEXED files, the file status becomes 92.

## Journal Mode of INDEXED Files

INDEXED files are backed by SQLite, and since opensource COBOL 4J 2.1.0 they use the WAL
(write-ahead logging) journal mode by default. The mode is selected with the
`COB_INDEXED_JOURNAL_MODE` environment variable, which accepts `WAL` (the default) and `DELETE`;
`DELETE` restores the rollback-journal behaviour of earlier versions. See
[Environment Variables Reference](./environment_variables.md) for details.

The journal mode is recorded in the SQLite file header. The runtime applies the configured mode
every time it opens an INDEXED file, so switching the environment variable converts existing files
in both directions. If another process currently holds the file open, SQLite silently declines the
conversion and the file keeps its current mode; this is not an error, and the next OPEN that finds
the file unused converts it.

While an INDEXED file is open in WAL mode, SQLite maintains two auxiliary files beside it:

| File | Purpose |
| --- | --- |
| `<file>-wal` | The write-ahead log holding committed data not yet folded into the main file |
| `<file>-shm` | The shared-memory index of the log, used to coordinate concurrent processes |

Both are removed automatically when the last connection closes normally, so a cleanly closed
INDEXED file consists of a single file exactly as before. `DELETE FILE` removes them together with
the main file.

If a process terminates abnormally, the `-wal` file survives and holds committed records that are
not yet in the main file. The next OPEN recovers them, so no data is lost. Two consequences follow:

- Copying or moving an INDEXED file that has a `-wal` beside it must include the `-wal` file, or the
  records it holds are lost. Closing the file first is the more reliable option, as that leaves
  nothing to copy but the main file.
- A file left behind by an abnormally terminated process also keeps its file lock. `cobj-idx unlock`
  releases it, as described above.

WAL requires shared memory and therefore does not work on network filesystems such as NFS. Set
`COB_INDEXED_JOURNAL_MODE=DELETE` when INDEXED files are stored on one.

# Environment Variables Reference

This document describes the environment variables used by opensource COBOL 4J.

## Compiler Settings

Under construction.

## Runtime Settings

### General Settings

#### COB_DATE

Fixes the date returned at runtime.

- **Format**: `YYYY/MM/DD`
- **Example**: `COB_DATE=2024/01/15`
- **Purpose**: Fixes the date returned by built-in functions such as `CURRENT-DATE`. Useful for testing and reproducible execution.

#### COB_VERBOSE

Controls whether detailed debug information is output.

- **Value**: Enabled by `Y` or `y`
- **Example**: `COB_VERBOSE=Y`
- **Purpose**: Use when you want to check runtime behavior in detail.

#### COB_TERMINAL_ENCODING

Specifies the terminal encoding used by DISPLAY and ACCEPT statements.

- **Value**: `UTF-8` or `UTF8` (default is Shift_JIS)
- **Example**: `COB_TERMINAL_ENCODING=UTF-8`
- **Purpose**: Used for terminal I/O in UTF-8 environments.

#### COB_SWITCH_1 through COB_SWITCH_8

Sets the initial values of switches that can be referenced within COBOL programs.

- **Value**: Enabled by `ON` (any other value disables)
- **Example**: `COB_SWITCH_1=ON`
- **Purpose**: Used as flags to switch program behavior.

### File I/O

#### COB_FILE_PATH

Specifies the default directory path for file searches.

- **Example**: `COB_FILE_PATH=/data/cobol/files`
- **Purpose**: Files specified in SELECT clauses are searched relative to this path.

#### DD_filename / dd_filename

Specifies the actual file path for a specific file.

- **Format**: `DD_<filename>=<actual path>` or `dd_<filename>=<actual path>`
- **Example**: `DD_MASTER=/data/master.dat`
- **Purpose**: Maps file names in COBOL programs to different paths at runtime.

#### COB_SYNC

Controls whether disk synchronization is performed after file operations.

- **Value**: Enabled by `Y` or `y`
- **Example**: `COB_SYNC=Y`
- **Purpose**: Use when you want to ensure data persistence.

#### COB_LS_NULLS

Controls NULL character handling in line sequential files.

- **Purpose**: Specifies how NULL characters are handled when reading/writing line sequential files.

#### COB_LS_FIXED

Controls whether line sequential files are treated as fixed-length.

- **Purpose**: Specifies how record length is handled for line sequential files.

#### OC_IO_CREATES

Controls whether files are automatically created when opening in I-O mode if the file does not exist.

- **Value**: Enabled by `yes`
- **Example**: `OC_IO_CREATES=yes`
- **Purpose**: Controls behavior when a file does not exist during OPEN I-O.

#### OC_EXTEND_CREATES

Controls whether files are automatically created when opening in EXTEND mode if the file does not exist.

- **Value**: Enabled by `yes`
- **Example**: `OC_EXTEND_CREATES=yes`
- **Purpose**: Controls behavior when a file does not exist during OPEN EXTEND.

#### COB_FILE_SEQ_WRITE_BUFFER_SIZE

Specifies the write buffer size for sequential files.

- **Value**: Integer >= 0 (default: 10)
- **Example**: `COB_FILE_SEQ_WRITE_BUFFER_SIZE=100`
- **Purpose**: Adjusts write performance.

#### COB_IO_ASSUME_REWRITE

Controls whether READ is required before REWRITE.

- **Value**: Enabled by `Y` or `y`
- **Example**: `COB_IO_ASSUME_REWRITE=Y`
- **Purpose**: Use when allowing REWRITE without a preceding READ.

### Temporary Files

#### TMPDIR / TMP / TEMP

Specifies the directory for temporary files used by sort operations and similar processes.

- **Priority**: `TMPDIR` > `TMP` > `TEMP`
- **Example**: `TMPDIR=/tmp/cobol`
- **Purpose**: Specifies the location for sort and temporary file creation.

### Program Calls

#### COB_LOAD_CASE

Controls case conversion of program names specified in CALL statements.

- **Value**:
  - `LOWER`: Convert to lowercase
  - `UPPER`: Convert to uppercase
- **Example**: `COB_LOAD_CASE=LOWER`
- **Purpose**: Controls case conversion when searching for program names.

#### COB_LIBRARY_PATH

Specifies the classpath for searching programs called by CALL statements.

- **Format**: Directory path (delimiter is system-dependent)
- **Example**: `COB_LIBRARY_PATH=/opt/cobol/lib`
- **Purpose**: Adds search paths for dynamically called programs.

#### COB_PACKAGE_PATH

Specifies the package path for Java classes called by CALL statements.

- **Example**: `COB_PACKAGE_PATH=com.example.cobol`
- **Purpose**: Used for program searching within Java package structures.

#### COB_PRE_LOAD

Specifies modules to pre-load at program start.

- **Note**: Currently not implemented.

### Numeric Processing

#### COB_NIBBLE_C_UNSIGNED

Controls nibble value processing for unsigned packed-decimal numbers.

- **Value**: Enabled by `Y` or `y`
- **Example**: `COB_NIBBLE_C_UNSIGNED=Y`
- **Purpose**: Used for compatibility with specific COBOL dialects.

## Usage Examples

```bash
# Runtime settings example
export COB_DATE=2024/01/15
export COB_TERMINAL_ENCODING=UTF-8
export COB_FILE_PATH=/data/cobol
export DD_MASTER=/data/master.dat

# Run the program
java -cp libcobj.jar:. MAINPROG
```

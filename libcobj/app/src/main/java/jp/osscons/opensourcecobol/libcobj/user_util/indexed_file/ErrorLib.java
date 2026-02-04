package jp.osscons.opensourcecobol.libcobj.user_util.indexed_file;

/**
 * Utility class for reporting errors that occur during cobj-idx indexed file operations. All
 * methods return error code 1 and print diagnostic messages to stderr.
 */
class ErrorLib {
    /**
     * Reports an error when the specified indexed file does not exist and returns an error code.
     *
     * @param indexedFilePath the path to the indexed file that was not found
     * @return 1 as the error code
     */
    static int errorFileDoesNotExist(String indexedFilePath) {
        System.err.println("error: '" + indexedFilePath + "' does not exist.");
        return 1;
    }

    /**
     * Reports an error when the specified file is not a valid opensource COBOL 4J indexed file.
     *
     * @param indexedFilePath the path to the invalid indexed file
     * @return 1 as the error code
     */
    static int errorInvalidIndexedFile(String indexedFilePath) {
        System.err.println("error: '" + indexedFilePath + "' is not a valid indexed file.");
        return 1;
    }

    /**
     * Reports an error when an I/O operation fails while accessing the indexed file.
     *
     * @return 1 as the error code
     */
    static int errorIO() {
        System.err.println("error: IO error.");
        return 1;
    }

    /**
     * Reports an error when duplicate keys are detected during data loading.
     *
     * @return 1 as the error code
     */
    static int errorDuplicateKeys() {
        System.err.println("error: loading fails because of duplicate keys.");
        return 1;
    }

    /**
     * Reports an error when input records have sizes that do not match the expected record size.
     *
     * @param correctSize the expected record size in bytes that all records must have
     * @return 1 as the error code
     */
    static int errorDataSizeMismatch(int correctSize) {
        System.err.println("error: all record must have the length of " + correctSize + " bytes.");
        return 1;
    }
}

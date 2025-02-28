### Add

* Implement the environment variable `COB_TERMINAL_ENCODING`.
  * If the value of `COB_TERMINAL_ENCODING` is `UTF-8`, the DISPLAY statement outputs data after converting it to UTF-8. Otherwise, it outputs raw data (typically encoded in Shift JIS).
  * If the value of `COB_TERMINAL_ENCODING` is `UTF-8`, the ACCEPT statement processes input data as UTF-8 encoded. Otherwise, it processes input data as Shift JIS encoded.
* Fix the UTF-8 compatible compiler to recognize that the size of multibyte characters in COBOL source code as 2 bytes instead of 3 bytes.
  * This change allows users to write more multibyte characters on each line of the COBOL source code.

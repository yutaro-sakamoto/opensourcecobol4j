### Add

* Support a new environment variable `COB_TERMINAL_ENCODING`.
  * If the value of `COB_TERMINAL_ENCODING` is `UTF-8`, then DISPLAY statements convert the encoding of given data to UTF-8 and output it. Otherwise, DISPLAY statements output raw data (usually the encoding is Shift JIS).
  * If the value of `COB_TERMINAL_ENCODING` is `UTF-8`, then ACCEPT statements assume that the encoding of input data is UTF-8. Otherwise ACCEPT statements assume that the encoding of input data is Shift JIS.
* Fix the UTF-8 compatible compiler so that the compiler recognizes that the size of a multibyte character in COBOL source code is 2 bytes instead of 3 bytes.
  * This change allows users to write more multibyte characters in each line of COBOL source code.

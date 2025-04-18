### Fix

* Fix code generations for some WRITE statements.
  * Older versions of the compiler converts some WRITE statements into invalid Java code.
* Fix the compiler to handle source code with UTF-8 encoded half-width Katakana characters correctly.
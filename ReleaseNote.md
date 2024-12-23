## Add

  * Convert string literals containing SJIS characters to Java string literals.
  * The older versions convert string literals containing SJIS characters to a byte arrays with hexdecimal integers.

## Fix

* Fix comparisons of COMP data.
  * With older versions, comparisons of COMP data are invalid in some cases.
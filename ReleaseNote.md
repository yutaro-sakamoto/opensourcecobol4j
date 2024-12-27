### Add

* Convert string literals containing SJIS characters to Java string literals.
  * The older versions convert string literals containing SJIS characters into byte arrays with hexdecimal integers.
    * Older versions convert `"日本語"` into `CobolUtil.toBytes((byte)0x93, (byte)0xfa, (byte)0x96, (byte)0x7b, (byte)0x8c, (byte)0xea)`.
    * Latest version converts `"日本語"` to `CobolUtil.stringToBytes("日本語")`.

### Fix

* Fix comparisons of COMP data.
  * With older versions, comparisons of COMP data are invalid in some cases.
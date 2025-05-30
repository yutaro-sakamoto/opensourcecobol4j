### Add

* Add a sub command create for cobj-idx. (#648)
  * `cobj-idx create` creates a new indexed file.
  
### Fix

* Handle non-ASCII characters in EXTERNAL items and EXTERNAL file names correctly. (#624)
* Show error messages if unimplemented subroutines are used. (#628)
* Fix non-ASCII characters in external items and external file descriptors. (#629)
  * The older version does not convert `－` in EXTERNAL items and EXTERNAL file names into another valid character in Java.
  * The older version converts `ＢＣ` in EXTERNAL file names into invalid characters.
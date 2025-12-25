### New Features

* Implementation of CANCEL ALL (#748)
  * CANCEL ALL performs CANCEL processing on all modules in the call chain of 
the program that calls CANCEL ALL.
* Improve readability of generated code
  * Rename some variables to more descriptive names (#775)
  * Replace magic numbers with constants or calculations between constants (#768)

&emsp;&emsp;Before these changes

```java
  a_1 = new CobolFieldAttribute (18, 5, 0, 7, null);
```

&emsp;&emsp;After these changes

```java
  a_1_NumericPacked_Digits5_Scale0_HaveSign_SignSeparate_SignLeading = new CobolFieldAttribute(
    CobolFieldAttribute.COB_TYPE_NUMERIC_PACKED,
    /* digits= */ 5,
    /* scale= */ 0,
    CobolFieldAttribute.COB_FLAG_HAVE_SIGN
      | CobolFieldAttribute.COB_FLAG_SIGN_SEPARATE
      | CobolFieldAttribute.COB_FLAG_SIGN_LEADING,
    /* pic= */ null);
```

### Fixed

* Fix `-j` option and `-o` option (#754)
* Fix cobj-idx installation issues (#769)
* Fix configuration items related to maximum size of PIC N items (#770)
* Fix cobj command to detect duplicate labels (#774)
* Fix `doc/converted_Java_file_JP.md` (#759)

### Miscellaneous

* Remove unused code from libcobj.jar (runtime library) (#756)
* Remove unnecessary code from generated Java code to improve readability (#760)
* Improve Dev Container configuration (#761)
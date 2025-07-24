### Fix

* Fix OPEN operation for INDEXED files. (#676)
  * The runtime library now correctly sets the file status to 30 when attempting to open INDEXED files with invalid or corrupted file formats.


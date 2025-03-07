### Fix

* Fix bugs related to UTF-8
  * With older versions, DISPLAY statements output invalid UTF-8 data when the arguments are sub items of group fields.
  * With older versions, initilizing data with VALUE clauses fails in some cases.
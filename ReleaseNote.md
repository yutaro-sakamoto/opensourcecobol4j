### New Features

- Implemented POINTER type variables (#802, #808)
- Implemented PROCEDURE DIVISION CHAINING (#796)
  - Enables passing command-line arguments as parameters to COBOL programs
- Fixed generated Java code for OCCURS DEPENDING ON (#799)
- Added an error message for unimplemented MOVE statements that use ALPHABET in the SPECIAL NAMES paragraph (#807)

### Fixed

- Fixed a bug when specifying the result of a function call as an argument of a CALL statement (#797)
- Fixed the moveFrom method in the CobolGroup class (#813)

### Documentation

- Updated Javadoc comments for exception-related classes in libcobj.jar (#804)
- Updated Javadoc comments for the CobolTerminal class in libcobj.jar (#811)
- Updated README.md to document that compiling multiple programs at once reduces compilation time (#806)
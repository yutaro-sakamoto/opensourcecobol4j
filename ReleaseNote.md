### Added
* Add a new option `-variable`
  * With `-variable`, cobj allows 73 or more characters for each lines
* Add documentations that describe the structure of libcobj and generated Java files
* Add Visual Studio Code Dev Container of opensource COBOL 4J

### Fixed
* Fix a build error of opensource COBOL 4J on Windows 10
* Fix a build error of opensource COBOL 4J on Docker
* Fix `cobj-api`
  * `cobj-api` previously generated invalid constructors in record classses
* Fix typos of `cobj-idx` command
* Fix a command line option `-fserial-variable`

### Miscellaneous
* Improve the readability of variable names containing multi-byte characters
* Convert some EVALUATE statements to switch statements

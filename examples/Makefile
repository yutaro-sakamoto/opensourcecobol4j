# Makefile for opensource COBOL 4J environment variable examples
#
# Usage:
#   make compile-all   - Compile all example programs
#   make run-all       - Compile and run all examples
#   make compile-<name> - Compile a specific example
#   make run-<name>    - Compile and run a specific example
#   make clean         - Remove generated files

PROJECT_ROOT   = ..
COBJ           = $(PROJECT_ROOT)/.local_install/bin/cobj
LIBCOBJ_JAR    = $(PROJECT_ROOT)/.local_install/lib/opensourcecobol4j/libcobj.jar
CLASSPATH      = $(LIBCOBJ_JAR)
COBJ_FLAGS     = -std=cobol2002 -debug -Wall
COBJ_FLAGS_JP  = $(COBJ_FLAGS) -conf=$(PROJECT_ROOT)/config/jp-compat.conf
JAVA_FLAGS     = -cp $(CLASSPATH):.

PROGRAMS = cobdate cobverbose cobswitch cobfilepath ddfilename \
           ociocreates ocextcreates loadcaller cobrewrite \
           cobnibble coblsfixed

.NOTPARALLEL:
.PHONY: all compile-all run-all clean $(addprefix compile-,$(PROGRAMS)) $(addprefix run-,$(PROGRAMS))

all: compile-all

# ============================================================
# Compile targets
# ============================================================

compile-all: $(addprefix compile-,$(PROGRAMS))

compile-cobdate: cobdate.cbl
	$(COBJ) $(COBJ_FLAGS) cobdate.cbl

compile-cobverbose: cobverbose.cbl
	$(COBJ) $(COBJ_FLAGS) cobverbose.cbl

compile-cobswitch: cobswitch.cbl
	$(COBJ) $(COBJ_FLAGS_JP) cobswitch.cbl

compile-cobfilepath: cobfilepath.cbl
	$(COBJ) $(COBJ_FLAGS) cobfilepath.cbl

compile-ddfilename: ddfilename.cbl
	$(COBJ) $(COBJ_FLAGS) ddfilename.cbl

compile-ociocreates: ociocreates.cbl
	$(COBJ) $(COBJ_FLAGS) ociocreates.cbl

compile-ocextcreates: ocextcreates.cbl
	$(COBJ) $(COBJ_FLAGS) ocextcreates.cbl

compile-loadcaller: loadcaller.cbl CALLEE.cbl
	$(COBJ) $(COBJ_FLAGS) CALLEE.cbl
	$(COBJ) $(COBJ_FLAGS) loadcaller.cbl

compile-cobrewrite: cobrewrite.cbl
	$(COBJ) $(COBJ_FLAGS) cobrewrite.cbl

compile-cobnibble: cobnibble.cbl
	$(COBJ) $(COBJ_FLAGS) cobnibble.cbl

compile-coblsfixed: coblsfixed.cbl
	$(COBJ) $(COBJ_FLAGS) coblsfixed.cbl

# ============================================================
# Run targets
# ============================================================

run-all: $(addprefix run-,$(PROGRAMS))

# --- 1. COB_DATE ---
run-cobdate: compile-cobdate
	@echo "=========================================="
	@echo " COB_DATE"
	@echo "=========================================="
	@echo "--- Without COB_DATE (uses today) ---"
	@java $(JAVA_FLAGS) cobdate
	@echo "--- With COB_DATE=1970/01/02 ---"
	@COB_DATE=1970/01/02 java $(JAVA_FLAGS) cobdate
	@echo ""

# --- 2. COB_VERBOSE ---
run-cobverbose: compile-cobverbose
	@echo "=========================================="
	@echo " COB_VERBOSE"
	@echo "=========================================="
	@# 3 records x 4 bytes (2-byte key + 2-byte data)
	@printf "11CC33AA22BB" > sort-input.txt
	@echo "--- Without COB_VERBOSE ---"
	@java $(JAVA_FLAGS) cobverbose
	@echo "(no verbose output)"
	@echo "--- With COB_VERBOSE=Y ---"
	@COB_VERBOSE=Y java $(JAVA_FLAGS) cobverbose
	@rm -f sort-input.txt sort-output.txt S01
	@echo ""

# --- 3. COB_SWITCH_1~8 ---
run-cobswitch: compile-cobswitch
	@echo "=========================================="
	@echo " COB_SWITCH_1~8"
	@echo "=========================================="
	@echo "--- COB_SWITCH_1=ON COB_SWITCH_2=OFF ---"
	@COB_SWITCH_1=ON COB_SWITCH_2=OFF java $(JAVA_FLAGS) cobswitch
	@echo "--- COB_SWITCH_1=OFF COB_SWITCH_2=ON ---"
	@COB_SWITCH_1=OFF COB_SWITCH_2=ON java $(JAVA_FLAGS) cobswitch
	@echo ""

# --- 4. COB_FILE_PATH ---
run-cobfilepath: compile-cobfilepath
	@echo "=========================================="
	@echo " COB_FILE_PATH"
	@echo "=========================================="
	@rm -f PATHTEST
	@mkdir -p subdir
	@echo "--- Without COB_FILE_PATH ---"
	@java $(JAVA_FLAGS) cobfilepath
	@echo "Created: PATHTEST (size=$$(wc -c < PATHTEST) bytes)"
	@rm -f PATHTEST
	@echo "--- With COB_FILE_PATH=subdir ---"
	@COB_FILE_PATH=subdir java $(JAVA_FLAGS) cobfilepath
	@echo "Created: subdir/PATHTEST (size=$$(wc -c < subdir/PATHTEST) bytes)"
	@rm -f PATHTEST subdir/PATHTEST
	@rmdir subdir 2>/dev/null || true
	@echo ""

# --- 5. DD_filename ---
run-ddfilename: compile-ddfilename
	@echo "=========================================="
	@echo " DD_filename"
	@echo "=========================================="
	@rm -f MYDATA actual-output.dat
	@echo "--- Without DD_MYDATA ---"
	@java $(JAVA_FLAGS) ddfilename
	@echo "Created: MYDATA (size=$$(wc -c < MYDATA) bytes)"
	@rm -f MYDATA
	@echo "--- With DD_MYDATA=actual-output.dat ---"
	@DD_MYDATA=actual-output.dat java $(JAVA_FLAGS) ddfilename
	@echo "Created: actual-output.dat (size=$$(wc -c < actual-output.dat) bytes)"
	@rm -f MYDATA actual-output.dat
	@echo ""

# --- 6. OC_IO_CREATES ---
run-ociocreates: compile-ociocreates
	@echo "=========================================="
	@echo " OC_IO_CREATES"
	@echo "=========================================="
	@rm -f TEST-IO-FILE
	@echo "--- Without OC_IO_CREATES (expect status 35) ---"
	@java $(JAVA_FLAGS) ociocreates 2>/dev/null || true
	@rm -f TEST-IO-FILE
	@echo "--- With OC_IO_CREATES=yes (expect status 00) ---"
	@OC_IO_CREATES=yes java $(JAVA_FLAGS) ociocreates
	@rm -f TEST-IO-FILE
	@echo ""

# --- 7. OC_EXTEND_CREATES ---
run-ocextcreates: compile-ocextcreates
	@echo "=========================================="
	@echo " OC_EXTEND_CREATES"
	@echo "=========================================="
	@rm -f TEST-EXT-FILE
	@echo "--- Without OC_EXTEND_CREATES (expect status 35) ---"
	@java $(JAVA_FLAGS) ocextcreates 2>/dev/null || true
	@rm -f TEST-EXT-FILE
	@echo "--- With OC_EXTEND_CREATES=yes (expect status 00) ---"
	@OC_EXTEND_CREATES=yes java $(JAVA_FLAGS) ocextcreates
	@rm -f TEST-EXT-FILE
	@echo ""

# --- 8. COB_LOAD_CASE ---
run-loadcaller: compile-loadcaller
	@echo "=========================================="
	@echo " COB_LOAD_CASE"
	@echo "=========================================="
	@echo "--- Without COB_LOAD_CASE ---"
	@java $(JAVA_FLAGS) loadcaller
	@echo "--- With COB_LOAD_CASE=UPPER ---"
	@COB_LOAD_CASE=UPPER java $(JAVA_FLAGS) loadcaller
	@echo ""

# --- 9. COB_IO_ASSUME_REWRITE ---
run-cobrewrite: compile-cobrewrite
	@echo "=========================================="
	@echo " COB_IO_ASSUME_REWRITE"
	@echo "=========================================="
	@# 3 records x 4 bytes (PIC X(4))
	@printf "000011112222" > TEST-REWRITE
	@echo "--- Input file content ---"
	@cat TEST-REWRITE
	@echo ""
	@echo "--- With COB_IO_ASSUME_REWRITE=Y ---"
	@COB_IO_ASSUME_REWRITE=Y java $(JAVA_FLAGS) cobrewrite
	@echo "--- Output file content ---"
	@cat TEST-REWRITE
	@echo ""
	@rm -f TEST-REWRITE
	@echo ""

# --- 10. COB_NIBBLE_C_UNSIGNED ---
run-cobnibble: compile-cobnibble
	@echo "=========================================="
	@echo " COB_NIBBLE_C_UNSIGNED"
	@echo "=========================================="
	@echo "--- Without COB_NIBBLE_C_UNSIGNED ---"
	@java $(JAVA_FLAGS) cobnibble
	@echo "--- With COB_NIBBLE_C_UNSIGNED=Y ---"
	@COB_NIBBLE_C_UNSIGNED=Y java $(JAVA_FLAGS) cobnibble
	@echo ""

# --- 11. COB_LS_FIXED ---
run-coblsfixed: compile-coblsfixed
	@echo "=========================================="
	@echo " COB_LS_FIXED"
	@echo "=========================================="
	@rm -f ls-test-output.txt
	@echo "--- Without COB_LS_FIXED ---"
	@java $(JAVA_FLAGS) coblsfixed
	@echo "File size: $$(wc -c < ls-test-output.txt) bytes"
	@rm -f ls-test-output.txt
	@echo "--- With COB_LS_FIXED=Y ---"
	@COB_LS_FIXED=Y java $(JAVA_FLAGS) coblsfixed
	@echo "File size: $$(wc -c < ls-test-output.txt) bytes"
	@rm -f ls-test-output.txt
	@echo ""

# ============================================================
# Clean
# ============================================================

clean:
	rm -f *.java *.class
	rm -f sort-input.txt sort-output.txt S01
	rm -f PATHTEST MYDATA actual-output.dat
	rm -f TEST-IO-FILE TEST-EXT-FILE TEST-REWRITE
	rm -f ls-test-output.txt
	rm -rf subdir

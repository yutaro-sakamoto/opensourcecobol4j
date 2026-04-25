# Makefile for ESQL examples
# See db-config.sh to configure database connection settings.

-include db-config.sh

COBJ = cobj
# Auto-detect the COPY directory from cobj's install location
COPY_DIR = $(shell dirname $$(which $(COBJ)))/../share/opensource-cobol-4j-*/copy

# Replace database placeholders in a COBOL source file.
# Usage: $(call embed_db_info,filename)
define embed_db_info
	sed -e 's/<|DB_NAME|>/$(DB_NAME)/g' \
	    -e 's/<|DB_HOST|>/$(DB_HOST)/g' \
	    -e 's/<|DB_PORT|>/$(DB_PORT)/g' \
	    -e 's/<|DB_USER|>/$(DB_USER)/g' \
	    -e 's/<|DB_PASSWORD|>/$(DB_PASSWORD)/g' $1 > $1.tmp && mv $1.tmp $1
endef

.PHONY: all clean sample cursor prepare

all: sample cursor prepare

sample: sample.cbl
	cp sample.cbl sample_run.cbl
	$(call embed_db_info,sample_run.cbl)
	$(COBJ) -I $(COPY_DIR) sample_run.cbl
	java sample_run

cursor: cursor.cbl
	cp cursor.cbl cursor_run.cbl
	$(call embed_db_info,cursor_run.cbl)
	$(COBJ) -I $(COPY_DIR) cursor_run.cbl
	java cursor_run

prepare: prepare.cbl
	cp prepare.cbl prepare_run.cbl
	$(call embed_db_info,prepare_run.cbl)
	$(COBJ) -I $(COPY_DIR) prepare_run.cbl
	java prepare_run

clean:
	rm -f *.class *.java *_run.cbl

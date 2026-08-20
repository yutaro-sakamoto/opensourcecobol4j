/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 27 "parser.y"

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "cobj.h"
#include "tree.h"

#define yyerror			cb_error
#define YYDEBUG			1
#define YYERROR_VERBOSE		1

#define PENDING(x)		cb_warning (_("'%s' not implemented"), x)

#define emit_statement(x) \
  current_program->exec_list = cb_cons (x, current_program->exec_list)

#define push_expr(type, node) \
  current_expr = cb_build_list (cb_int (type), node, current_expr)

#define TERM_NONE		0
#define TERM_ACCEPT		1
#define TERM_ADD		2
#define TERM_CALL		3
#define TERM_COMPUTE		4
#define TERM_DELETE		5
#define TERM_DISPLAY		6
#define TERM_DIVIDE		7
#define TERM_EVALUATE		8
#define TERM_IF			9
#define TERM_MULTIPLY		10
#define TERM_PERFORM		11
#define TERM_READ		12
#define TERM_RECEIVE		13
#define TERM_RETURN		14
#define TERM_REWRITE		15
#define TERM_SEARCH		16
#define TERM_START		17
#define TERM_STRING		18
#define TERM_SUBTRACT		19
#define TERM_UNSTRING		20
#define TERM_WRITE		21
#define TERM_MAX		22

/* Global variables */

struct cb_program		*current_program = NULL;
struct cb_statement		*current_statement = NULL;
struct cb_label			*current_section = NULL;
struct cb_label			*current_paragraph = NULL;
size_t				functions_are_all = 0;
int				non_const_word = 0;

/* Local variables */

static struct cb_statement	*main_statement;

static cb_tree			current_expr;
static struct cb_field		*current_field;
static struct cb_field		*description_field;
static struct cb_file		*current_file;
static struct cb_key_component	*key_component_list;
static enum cb_storage		current_storage;

static size_t			check_unreached = 0;
static int			call_mode;
static int			size_mode;

static cb_tree			perform_stack = NULL;
static cb_tree			qualifier = NULL;

static cb_tree			fgc;
static cb_tree			bgc;
static cb_tree			scroll;
static cb_tree			save_tree_1;
static cb_tree			save_tree_2;
static cb_tree			dummy_tree;
static size_t			in_declaratives = 0;
static size_t			current_linage = 0;
static size_t			prog_end = 0;
static size_t			use_global_ind = 0;
static size_t			samearea = 1;
static size_t			organized_seen = 0;
static size_t			inspect_keyword = 0;
static int			next_label_id = 0;
static int			esql_sqlca_injected = 0;
static int			eval_level = 0;
static int			eval_inc = 0;
static int			eval_inc2 = 0;
static int			depth = 0;
static int			dispattrs = 0;
static struct cb_file		*linage_file;
static cb_tree			next_label_list = NULL;
static char			*stack_progid[32];
static int			term_array[TERM_MAX];
static int			eval_check[64][64];

/* Static functions */

static void
BEGIN_STATEMENT (const char *name, const size_t term)
{
	if (cb_warn_unreachable && check_unreached) {
		cb_warning (_("Unreachable statement '%s'"), (char *)name);
	}
	current_statement = cb_build_statement ((char *)name);
	CB_TREE (current_statement)->source_file = (unsigned char *)cb_source_file;
	CB_TREE (current_statement)->source_line = cb_source_line;
	emit_statement (CB_TREE (current_statement));
	if (term) {
		term_array[term]++;
	}
	main_statement = current_statement;
}

static void
BEGIN_IMPLICIT_STATEMENT (cb_tree node)
{
	current_statement = cb_build_statement (NULL);
	CB_TREE (current_statement)->source_file = CB_TREE (node)->source_file;
	CB_TREE (current_statement)->source_line = CB_TREE (node)->source_line;
	main_statement->body = cb_list_add (main_statement->body,
					    CB_TREE (current_statement));
}

static void
emit_entry (const char *name, const int encode, cb_tree using_list)
{
	cb_tree		l;
	cb_tree		label;
	cb_tree		x;
	struct cb_field	*f;
	int		parmnum;
	char		buff[256];

	sprintf (buff, "E$%s", name);
	label = cb_build_label (cb_build_reference (buff), NULL);
	if (encode) {
		CB_LABEL (label)->name = (unsigned char *)(cb_encode_program_id (name));
		CB_LABEL (label)->orig_name = (unsigned char *)name;
	} else {
		CB_LABEL (label)->name = (unsigned char *)name;
		CB_LABEL (label)->orig_name = (unsigned char *)current_program->orig_source_name;
	}
	CB_LABEL (label)->need_begin = 1;
	CB_LABEL (label)->is_entry = 1;
	emit_statement (label);

	parmnum = 1;
	for (l = using_list; l; l = CB_CHAIN (l)) {
		x = CB_VALUE (l);
		if (x != cb_error_node && cb_ref (x) != cb_error_node) {
			f = CB_FIELD (cb_ref (x));
			if (f->level != 01 && f->level != 77) {
				cb_error_x (x, _("'%s' not level 01 or 77"), cb_name (x));
			}
			if (!current_program->flag_chained) {
				if (f->storage != CB_STORAGE_LINKAGE) {
					cb_error_x (x, _("'%s' is not in LINKAGE SECTION"), cb_name (x));
				}
				if (f->flag_item_based || f->flag_external) {
					cb_error_x (x, _("'%s' can not be BASED/EXTERNAL"), cb_name (x));
				}
				f->flag_is_pdiv_parm = 1;
			} else {
				if (f->storage != CB_STORAGE_WORKING) {
					cb_error_x (x, _("'%s' is not in WORKING-STORAGE SECTION"), cb_name (x));
				}
				f->flag_chained = 1;
				f->param_num = parmnum;
				parmnum++;
			}
			if (f->redefines) {
				cb_error_x (x, _("'%s' REDEFINES field not allowed here"), cb_name (x));
			}
		}
	}

	/* Check dangling LINKAGE items */
	if (cb_warn_linkage) {
		for (f = current_program->linkage_storage; f; f = f->sister) {
			for (l = using_list; l; l = CB_CHAIN (l)) {
				x = CB_VALUE (l);
				if (x != cb_error_node && cb_ref (x) != cb_error_node) {
					if (f == CB_FIELD (cb_ref (x))) {
						break;
					}
				}
			}
			if (!l && !f->redefines) {
				cb_warning (_("LINKAGE item '%s' is not a PROCEDURE USING parameter"), f->name);
			}
		}
	}

	for (l = current_program->entry_list; l; l = CB_CHAIN (l)) {
		if (strcmp ((const char *)name, (const char *)(CB_LABEL(CB_PURPOSE(l))->name)) == 0) {
			cb_error_x (CB_TREE (current_statement), _("ENTRY '%s' duplicated"), name);
		}
	}

	current_program->entry_list = cb_list_append (current_program->entry_list,
							cb_build_pair (label, using_list));
}

static void
terminator_warning (const size_t termid)
{
	check_unreached = 0;
	if (cb_warn_terminator && term_array[termid]) {
		cb_warning_x (CB_TREE (current_statement),
			_("%s statement not terminated by END-%s"),
			current_statement->name, current_statement->name);
	}
	if (term_array[termid]) {
		term_array[termid]--;
	}
}

static void
terminator_error (void)
{
	check_unreached = 0;
	cb_error_x (CB_TREE (current_statement),
			_("%s statement not terminated by END-%s"),
			current_statement->name, current_statement->name);
}

static void
terminator_clear (const size_t termid)
{
	check_unreached = 0;
	if (term_array[termid]) {
		term_array[termid]--;
	}
}

static int
literal_value (cb_tree x)
{
	if (x == cb_space) {
		return ' ';
	} else if (x == cb_zero) {
		return '0';
	} else if (x == cb_quote) {
		return '"';
	} else if (x == cb_null) {
		return 0;
	} else if (CB_TREE_CLASS (x) == CB_CLASS_NUMERIC) {
		return cb_get_int (x);
	} else {
		return CB_LITERAL (x)->data[0];
	}
}

static void
setup_use_file (struct cb_file *fileptr)
{
	struct cb_file	*newptr;

	if (fileptr->organization == COB_ORG_SORT) {
		cb_error (_("USE statement invalid for SORT file"));
	}
	if (fileptr->global) {
		newptr = cobc_malloc (sizeof(struct cb_file));
		*newptr = *fileptr;
		newptr->handler = current_section;
		newptr->handler_prog = current_program;
		if (!use_global_ind) {
			current_program->local_file_list =
				cb_list_add (current_program->local_file_list,
					     CB_TREE (newptr));
		} else {
			current_program->global_file_list =
				cb_list_add (current_program->global_file_list,
					     CB_TREE (newptr));
		}
	} else {
		fileptr->handler = current_section;
	}
}


#line 356 "parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ACCEPT = 3,                     /* ACCEPT  */
  YYSYMBOL_ACCESS = 4,                     /* ACCESS  */
  YYSYMBOL_ADD = 5,                        /* ADD  */
  YYSYMBOL_ADDRESS = 6,                    /* ADDRESS  */
  YYSYMBOL_ADVANCING = 7,                  /* ADVANCING  */
  YYSYMBOL_AFTER = 8,                      /* AFTER  */
  YYSYMBOL_ALL = 9,                        /* ALL  */
  YYSYMBOL_ALLOCATE = 10,                  /* ALLOCATE  */
  YYSYMBOL_ALPHABET = 11,                  /* ALPHABET  */
  YYSYMBOL_ALPHABETIC = 12,                /* ALPHABETIC  */
  YYSYMBOL_ALPHABETIC_LOWER = 13,          /* "ALPHABETIC-LOWER"  */
  YYSYMBOL_ALPHABETIC_UPPER = 14,          /* "ALPHABETIC-UPPER"  */
  YYSYMBOL_ALPHANUMERIC = 15,              /* ALPHANUMERIC  */
  YYSYMBOL_ALPHANUMERIC_EDITED = 16,       /* "ALPHANUMERIC-EDITED"  */
  YYSYMBOL_ALSO = 17,                      /* ALSO  */
  YYSYMBOL_ALTER = 18,                     /* ALTER  */
  YYSYMBOL_ALTERNATE = 19,                 /* ALTERNATE  */
  YYSYMBOL_AND = 20,                       /* AND  */
  YYSYMBOL_ANY = 21,                       /* ANY  */
  YYSYMBOL_APPLY = 22,                     /* APPLY  */
  YYSYMBOL_ARE = 23,                       /* ARE  */
  YYSYMBOL_AREA = 24,                      /* AREA  */
  YYSYMBOL_ARGUMENT_NUMBER = 25,           /* "ARGUMENT-NUMBER"  */
  YYSYMBOL_ARGUMENT_VALUE = 26,            /* "ARGUMENT-VALUE"  */
  YYSYMBOL_AS = 27,                        /* AS  */
  YYSYMBOL_ASCENDING = 28,                 /* ASCENDING  */
  YYSYMBOL_ASSIGN = 29,                    /* ASSIGN  */
  YYSYMBOL_AT = 30,                        /* AT  */
  YYSYMBOL_AUTO = 31,                      /* AUTO  */
  YYSYMBOL_AUTOMATIC = 32,                 /* AUTOMATIC  */
  YYSYMBOL_BACKGROUND_COLOR = 33,          /* "BACKGROUND-COLOR"  */
  YYSYMBOL_BASED = 34,                     /* BASED  */
  YYSYMBOL_BEFORE = 35,                    /* BEFORE  */
  YYSYMBOL_BELL = 36,                      /* BELL  */
  YYSYMBOL_BINARY = 37,                    /* BINARY  */
  YYSYMBOL_BINARY_C_LONG = 38,             /* "BINARY-C-LONG"  */
  YYSYMBOL_BINARY_CHAR = 39,               /* "BINARY-CHAR"  */
  YYSYMBOL_BINARY_DOUBLE = 40,             /* "BINARY-DOUBLE"  */
  YYSYMBOL_BINARY_LONG = 41,               /* "BINARY-LONG"  */
  YYSYMBOL_BINARY_SHORT = 42,              /* "BINARY-SHORT"  */
  YYSYMBOL_BLANK = 43,                     /* BLANK  */
  YYSYMBOL_BLANK_LINE = 44,                /* "BLANK-LINE"  */
  YYSYMBOL_BLANK_SCREEN = 45,              /* "BLANK-SCREEN"  */
  YYSYMBOL_BLINK = 46,                     /* BLINK  */
  YYSYMBOL_BLOCK = 47,                     /* BLOCK  */
  YYSYMBOL_BOTTOM = 48,                    /* BOTTOM  */
  YYSYMBOL_BY = 49,                        /* BY  */
  YYSYMBOL_BYTE_LENGTH = 50,               /* "BYTE-LENGTH"  */
  YYSYMBOL_CALL = 51,                      /* CALL  */
  YYSYMBOL_CANCEL = 52,                    /* CANCEL  */
  YYSYMBOL_CH = 53,                        /* CH  */
  YYSYMBOL_CHAINING = 54,                  /* CHAINING  */
  YYSYMBOL_CHARACTER = 55,                 /* CHARACTER  */
  YYSYMBOL_CHARACTERS = 56,                /* CHARACTERS  */
  YYSYMBOL_CLASS = 57,                     /* CLASS  */
  YYSYMBOL_CLASS_NAME = 58,                /* CLASS_NAME  */
  YYSYMBOL_CLOSE = 59,                     /* CLOSE  */
  YYSYMBOL_CLOSE_NOFEED = 60,              /* "CLOSE-NOFEED"  */
  YYSYMBOL_CODE = 61,                      /* CODE  */
  YYSYMBOL_CODE_SET = 62,                  /* "CODE-SET"  */
  YYSYMBOL_COLLATING = 63,                 /* COLLATING  */
  YYSYMBOL_COL = 64,                       /* COL  */
  YYSYMBOL_COLS = 65,                      /* COLS  */
  YYSYMBOL_COLUMN = 66,                    /* COLUMN  */
  YYSYMBOL_COLUMNS = 67,                   /* COLUMNS  */
  YYSYMBOL_COMMA = 68,                     /* COMMA  */
  YYSYMBOL_COMMAND_LINE = 69,              /* "COMMAND-LINE"  */
  YYSYMBOL_COMMA_DELIM = 70,               /* "comma delimiter"  */
  YYSYMBOL_COMMIT = 71,                    /* COMMIT  */
  YYSYMBOL_COMMITMENT_CONTROL = 72,        /* "COMMITMENT-CONTROL"  */
  YYSYMBOL_COMMON = 73,                    /* COMMON  */
  YYSYMBOL_COMP = 74,                      /* COMP  */
  YYSYMBOL_COMPUTE = 75,                   /* COMPUTE  */
  YYSYMBOL_COMP_1 = 76,                    /* "COMP-1"  */
  YYSYMBOL_COMP_2 = 77,                    /* "COMP-2"  */
  YYSYMBOL_COMP_3 = 78,                    /* "COMP-3"  */
  YYSYMBOL_COMP_4 = 79,                    /* "COMP-4"  */
  YYSYMBOL_COMP_5 = 80,                    /* "COMP-5"  */
  YYSYMBOL_COMP_X = 81,                    /* "COMP-X"  */
  YYSYMBOL_CONCATENATE_FUNC = 82,          /* "FUNCTION CONCATENATE"  */
  YYSYMBOL_CONFIGURATION = 83,             /* CONFIGURATION  */
  YYSYMBOL_CONSTANT = 84,                  /* CONSTANT  */
  YYSYMBOL_CONTAINS = 85,                  /* CONTAINS  */
  YYSYMBOL_CONTENT = 86,                   /* CONTENT  */
  YYSYMBOL_CONTINUE = 87,                  /* CONTINUE  */
  YYSYMBOL_CONTROL = 88,                   /* CONTROL  */
  YYSYMBOL_CONTROLS = 89,                  /* CONTROLS  */
  YYSYMBOL_CONTROL_FOOTING = 90,           /* "CONTROL FOOTING"  */
  YYSYMBOL_CONTROL_HEADING = 91,           /* "CONTROL HEADING"  */
  YYSYMBOL_CONVERTING = 92,                /* CONVERTING  */
  YYSYMBOL_CORE_INDEX = 93,                /* "CORE-INDEX"  */
  YYSYMBOL_CORRESPONDING = 94,             /* CORRESPONDING  */
  YYSYMBOL_COUNT = 95,                     /* COUNT  */
  YYSYMBOL_CRT = 96,                       /* CRT  */
  YYSYMBOL_CURRENCY = 97,                  /* CURRENCY  */
  YYSYMBOL_CURRENT_DATE_FUNC = 98,         /* "FUNCTION CURRENT-DATE"  */
  YYSYMBOL_CURSOR = 99,                    /* CURSOR  */
  YYSYMBOL_CYCLE = 100,                    /* CYCLE  */
  YYSYMBOL_CYL_OVERFLOW = 101,             /* "CYL-OVERFLOW"  */
  YYSYMBOL_DATA = 102,                     /* DATA  */
  YYSYMBOL_DATE = 103,                     /* DATE  */
  YYSYMBOL_DAY = 104,                      /* DAY  */
  YYSYMBOL_DAY_OF_WEEK = 105,              /* "DAY-OF-WEEK"  */
  YYSYMBOL_DE = 106,                       /* DE  */
  YYSYMBOL_DEBUGGING = 107,                /* DEBUGGING  */
  YYSYMBOL_DECIMAL_POINT = 108,            /* "DECIMAL-POINT"  */
  YYSYMBOL_DECLARATIVES = 109,             /* DECLARATIVES  */
  YYSYMBOL_DEFAULT = 110,                  /* DEFAULT  */
  YYSYMBOL_DELETE = 111,                   /* DELETE  */
  YYSYMBOL_DELIMITED = 112,                /* DELIMITED  */
  YYSYMBOL_DELIMITER = 113,                /* DELIMITER  */
  YYSYMBOL_DEPENDING = 114,                /* DEPENDING  */
  YYSYMBOL_DESCENDING = 115,               /* DESCENDING  */
  YYSYMBOL_DETAIL = 116,                   /* DETAIL  */
  YYSYMBOL_DISK = 117,                     /* DISK  */
  YYSYMBOL_DISPLAY = 118,                  /* DISPLAY  */
  YYSYMBOL_DIVIDE = 119,                   /* DIVIDE  */
  YYSYMBOL_DIVISION = 120,                 /* DIVISION  */
  YYSYMBOL_DOWN = 121,                     /* DOWN  */
  YYSYMBOL_DUPLICATES = 122,               /* DUPLICATES  */
  YYSYMBOL_DYNAMIC = 123,                  /* DYNAMIC  */
  YYSYMBOL_EBCDIC = 124,                   /* EBCDIC  */
  YYSYMBOL_ELSE = 125,                     /* ELSE  */
  YYSYMBOL_END = 126,                      /* END  */
  YYSYMBOL_END_ACCEPT = 127,               /* "END-ACCEPT"  */
  YYSYMBOL_END_ADD = 128,                  /* "END-ADD"  */
  YYSYMBOL_END_CALL = 129,                 /* "END-CALL"  */
  YYSYMBOL_END_COMPUTE = 130,              /* "END-COMPUTE"  */
  YYSYMBOL_END_DELETE = 131,               /* "END-DELETE"  */
  YYSYMBOL_END_DISPLAY = 132,              /* "END-DISPLAY"  */
  YYSYMBOL_END_DIVIDE = 133,               /* "END-DIVIDE"  */
  YYSYMBOL_END_EVALUATE = 134,             /* "END-EVALUATE"  */
  YYSYMBOL_END_FUNCTION = 135,             /* "END FUNCTION"  */
  YYSYMBOL_END_IF = 136,                   /* "END-IF"  */
  YYSYMBOL_END_MULTIPLY = 137,             /* "END-MULTIPLY"  */
  YYSYMBOL_END_PERFORM = 138,              /* "END-PERFORM"  */
  YYSYMBOL_END_PROGRAM = 139,              /* "END PROGRAM"  */
  YYSYMBOL_END_READ = 140,                 /* "END-READ"  */
  YYSYMBOL_END_RETURN = 141,               /* "END-RETURN"  */
  YYSYMBOL_END_REWRITE = 142,              /* "END-REWRITE"  */
  YYSYMBOL_END_SEARCH = 143,               /* "END-SEARCH"  */
  YYSYMBOL_END_START = 144,                /* "END-START"  */
  YYSYMBOL_END_STRING = 145,               /* "END-STRING"  */
  YYSYMBOL_END_SUBTRACT = 146,             /* "END-SUBTRACT"  */
  YYSYMBOL_END_UNSTRING = 147,             /* "END-UNSTRING"  */
  YYSYMBOL_END_WRITE = 148,                /* "END-WRITE"  */
  YYSYMBOL_ENTRY = 149,                    /* ENTRY  */
  YYSYMBOL_ENVIRONMENT = 150,              /* ENVIRONMENT  */
  YYSYMBOL_ENVIRONMENT_NAME = 151,         /* "ENVIRONMENT-NAME"  */
  YYSYMBOL_ENVIRONMENT_VALUE = 152,        /* "ENVIRONMENT-VALUE"  */
  YYSYMBOL_EOL = 153,                      /* EOL  */
  YYSYMBOL_EOP = 154,                      /* EOP  */
  YYSYMBOL_EOS = 155,                      /* EOS  */
  YYSYMBOL_EQUAL = 156,                    /* EQUAL  */
  YYSYMBOL_EQUALS = 157,                   /* EQUALS  */
  YYSYMBOL_ERASE = 158,                    /* ERASE  */
  YYSYMBOL_ERROR = 159,                    /* ERROR  */
  YYSYMBOL_ESCAPE = 160,                   /* ESCAPE  */
  YYSYMBOL_EVALUATE = 161,                 /* EVALUATE  */
  YYSYMBOL_EVENT_STATUS = 162,             /* "EVENT-STATUS"  */
  YYSYMBOL_EXCEPTION = 163,                /* EXCEPTION  */
  YYSYMBOL_EXCLUSIVE = 164,                /* EXCLUSIVE  */
  YYSYMBOL_EXIT = 165,                     /* EXIT  */
  YYSYMBOL_EXTEND = 166,                   /* EXTEND  */
  YYSYMBOL_EXTERNAL = 167,                 /* EXTERNAL  */
  YYSYMBOL_FD = 168,                       /* FD  */
  YYSYMBOL_FILE_CONTROL = 169,             /* "FILE-CONTROL"  */
  YYSYMBOL_FILE_ID = 170,                  /* "FILE-ID"  */
  YYSYMBOL_FILLER = 171,                   /* FILLER  */
  YYSYMBOL_FINAL = 172,                    /* FINAL  */
  YYSYMBOL_FIRST = 173,                    /* FIRST  */
  YYSYMBOL_FOOTING = 174,                  /* FOOTING  */
  YYSYMBOL_FOR = 175,                      /* FOR  */
  YYSYMBOL_FOREGROUND_COLOR = 176,         /* "FOREGROUND-COLOR"  */
  YYSYMBOL_FOREVER = 177,                  /* FOREVER  */
  YYSYMBOL_FORMS_OVERLAY = 178,            /* "FORMS-OVERLAY"  */
  YYSYMBOL_FREE = 179,                     /* FREE  */
  YYSYMBOL_FROM = 180,                     /* FROM  */
  YYSYMBOL_FULL = 181,                     /* FULL  */
  YYSYMBOL_FUNCTION = 182,                 /* FUNCTION  */
  YYSYMBOL_FUNCTION_ID = 183,              /* "FUNCTION-ID"  */
  YYSYMBOL_FUNCTION_NAME = 184,            /* "FUNCTION"  */
  YYSYMBOL_GE = 185,                       /* GE  */
  YYSYMBOL_GENERATE = 186,                 /* GENERATE  */
  YYSYMBOL_GIVING = 187,                   /* GIVING  */
  YYSYMBOL_GLOBAL = 188,                   /* GLOBAL  */
  YYSYMBOL_GO = 189,                       /* GO  */
  YYSYMBOL_GOBACK = 190,                   /* GOBACK  */
  YYSYMBOL_GREATER = 191,                  /* GREATER  */
  YYSYMBOL_GROUP = 192,                    /* GROUP  */
  YYSYMBOL_HEADING = 193,                  /* HEADING  */
  YYSYMBOL_HIGHLIGHT = 194,                /* HIGHLIGHT  */
  YYSYMBOL_HIGH_VALUE = 195,               /* "HIGH-VALUE"  */
  YYSYMBOL_IDENTIFICATION = 196,           /* IDENTIFICATION  */
  YYSYMBOL_IF = 197,                       /* IF  */
  YYSYMBOL_IGNORE = 198,                   /* IGNORE  */
  YYSYMBOL_IGNORING = 199,                 /* IGNORING  */
  YYSYMBOL_IN = 200,                       /* IN  */
  YYSYMBOL_INDEX = 201,                    /* INDEX  */
  YYSYMBOL_INDEXED = 202,                  /* INDEXED  */
  YYSYMBOL_INDICATE = 203,                 /* INDICATE  */
  YYSYMBOL_INITIALIZE = 204,               /* INITIALIZE  */
  YYSYMBOL_INITIALIZED = 205,              /* INITIALIZED  */
  YYSYMBOL_INITIATE = 206,                 /* INITIATE  */
  YYSYMBOL_INPUT = 207,                    /* INPUT  */
  YYSYMBOL_INPUT_OUTPUT = 208,             /* "INPUT-OUTPUT"  */
  YYSYMBOL_INSPECT = 209,                  /* INSPECT  */
  YYSYMBOL_INTO = 210,                     /* INTO  */
  YYSYMBOL_INTRINSIC = 211,                /* INTRINSIC  */
  YYSYMBOL_INVALID = 212,                  /* INVALID  */
  YYSYMBOL_INVALID_KEY = 213,              /* "INVALID KEY"  */
  YYSYMBOL_IS = 214,                       /* IS  */
  YYSYMBOL_I_O = 215,                      /* "I-O"  */
  YYSYMBOL_I_O_CONTROL = 216,              /* "I-O-CONTROL"  */
  YYSYMBOL_JUSTIFIED = 217,                /* JUSTIFIED  */
  YYSYMBOL_KEY = 218,                      /* KEY  */
  YYSYMBOL_LABEL = 219,                    /* LABEL  */
  YYSYMBOL_LAST = 220,                     /* LAST  */
  YYSYMBOL_LAST_DETAIL = 221,              /* "LAST DETAIL"  */
  YYSYMBOL_LE = 222,                       /* LE  */
  YYSYMBOL_LEADING = 223,                  /* LEADING  */
  YYSYMBOL_LEFT = 224,                     /* LEFT  */
  YYSYMBOL_LENGTH = 225,                   /* LENGTH  */
  YYSYMBOL_LESS = 226,                     /* LESS  */
  YYSYMBOL_LEVEL_NUMBER_WORD = 227,        /* LEVEL_NUMBER_WORD  */
  YYSYMBOL_LEVEL88_NUMBER_WORD = 228,      /* LEVEL88_NUMBER_WORD  */
  YYSYMBOL_LIMIT = 229,                    /* LIMIT  */
  YYSYMBOL_LIMITS = 230,                   /* LIMITS  */
  YYSYMBOL_LINAGE = 231,                   /* LINAGE  */
  YYSYMBOL_LINAGE_COUNTER = 232,           /* "LINAGE-COUNTER"  */
  YYSYMBOL_LINE = 233,                     /* LINE  */
  YYSYMBOL_LINES = 234,                    /* LINES  */
  YYSYMBOL_LINKAGE = 235,                  /* LINKAGE  */
  YYSYMBOL_LITERAL = 236,                  /* "Literal"  */
  YYSYMBOL_LOCALE = 237,                   /* LOCALE  */
  YYSYMBOL_LOCALE_DT_FUNC = 238,           /* "FUNCTION LOCALE"  */
  YYSYMBOL_LOCAL_STORAGE = 239,            /* "LOCAL-STORAGE"  */
  YYSYMBOL_LOCK = 240,                     /* LOCK  */
  YYSYMBOL_LOWER_CASE_FUNC = 241,          /* "FUNCTION LOWER-CASE"  */
  YYSYMBOL_LOWLIGHT = 242,                 /* LOWLIGHT  */
  YYSYMBOL_LOW_VALUE = 243,                /* "LOW-VALUE"  */
  YYSYMBOL_MANUAL = 244,                   /* MANUAL  */
  YYSYMBOL_MEMORY = 245,                   /* MEMORY  */
  YYSYMBOL_MERGE = 246,                    /* MERGE  */
  YYSYMBOL_MINUS = 247,                    /* MINUS  */
  YYSYMBOL_MNEMONIC_NAME = 248,            /* "MNEMONIC NAME"  */
  YYSYMBOL_MODE = 249,                     /* MODE  */
  YYSYMBOL_MOVE = 250,                     /* MOVE  */
  YYSYMBOL_MULTIPLE = 251,                 /* MULTIPLE  */
  YYSYMBOL_MULTIPLY = 252,                 /* MULTIPLY  */
  YYSYMBOL_NATIONAL = 253,                 /* NATIONAL  */
  YYSYMBOL_NATIONAL_EDITED = 254,          /* "NATIONAL-EDITED"  */
  YYSYMBOL_NATIVE = 255,                   /* NATIVE  */
  YYSYMBOL_NE = 256,                       /* NE  */
  YYSYMBOL_NEGATIVE = 257,                 /* NEGATIVE  */
  YYSYMBOL_NEXT = 258,                     /* NEXT  */
  YYSYMBOL_NEXT_SENTENCE = 259,            /* "NEXT SENTENCE"  */
  YYSYMBOL_NO = 260,                       /* NO  */
  YYSYMBOL_NOMINAL = 261,                  /* NOMINAL  */
  YYSYMBOL_NOT = 262,                      /* NOT  */
  YYSYMBOL_NOT_END = 263,                  /* "NOT END"  */
  YYSYMBOL_NOT_EOP = 264,                  /* "NOT EOP"  */
  YYSYMBOL_NOT_EXCEPTION = 265,            /* "NOT EXCEPTION"  */
  YYSYMBOL_NOT_INVALID_KEY = 266,          /* "NOT INVALID KEY"  */
  YYSYMBOL_NOT_OVERFLOW = 267,             /* "NOT OVERFLOW"  */
  YYSYMBOL_NOT_SIZE_ERROR = 268,           /* "NOT SIZE ERROR"  */
  YYSYMBOL_NO_ADVANCING = 269,             /* "NO ADVANCING"  */
  YYSYMBOL_NUMBER = 270,                   /* NUMBER  */
  YYSYMBOL_NUMBERS = 271,                  /* NUMBERS  */
  YYSYMBOL_NUMERIC = 272,                  /* NUMERIC  */
  YYSYMBOL_NUMERIC_EDITED = 273,           /* "NUMERIC-EDITED"  */
  YYSYMBOL_NUMVALC_FUNC = 274,             /* "FUNCTION NUMVALC"  */
  YYSYMBOL_OBJECT_COMPUTER = 275,          /* "OBJECT-COMPUTER"  */
  YYSYMBOL_OCCURS = 276,                   /* OCCURS  */
  YYSYMBOL_OF = 277,                       /* OF  */
  YYSYMBOL_OFF = 278,                      /* OFF  */
  YYSYMBOL_OMITTED = 279,                  /* OMITTED  */
  YYSYMBOL_ON = 280,                       /* ON  */
  YYSYMBOL_ONLY = 281,                     /* ONLY  */
  YYSYMBOL_OPEN = 282,                     /* OPEN  */
  YYSYMBOL_OPTIONAL = 283,                 /* OPTIONAL  */
  YYSYMBOL_OR = 284,                       /* OR  */
  YYSYMBOL_ORDER = 285,                    /* ORDER  */
  YYSYMBOL_ORGANIZATION = 286,             /* ORGANIZATION  */
  YYSYMBOL_OTHER = 287,                    /* OTHER  */
  YYSYMBOL_OUTPUT = 288,                   /* OUTPUT  */
  YYSYMBOL_OVERFLOW = 289,                 /* OVERFLOW  */
  YYSYMBOL_OVERLINE = 290,                 /* OVERLINE  */
  YYSYMBOL_PACKED_DECIMAL = 291,           /* "PACKED-DECIMAL"  */
  YYSYMBOL_PADDING = 292,                  /* PADDING  */
  YYSYMBOL_PAGE = 293,                     /* PAGE  */
  YYSYMBOL_PAGE_FOOTING = 294,             /* "PAGE FOOTING"  */
  YYSYMBOL_PAGE_HEADING = 295,             /* "PAGE HEADING"  */
  YYSYMBOL_PARAGRAPH = 296,                /* PARAGRAPH  */
  YYSYMBOL_PERFORM = 297,                  /* PERFORM  */
  YYSYMBOL_PICTURE = 298,                  /* PICTURE  */
  YYSYMBOL_PLUS = 299,                     /* PLUS  */
  YYSYMBOL_POINTER = 300,                  /* POINTER  */
  YYSYMBOL_POSITION = 301,                 /* POSITION  */
  YYSYMBOL_POSITIVE = 302,                 /* POSITIVE  */
  YYSYMBOL_PRESENT = 303,                  /* PRESENT  */
  YYSYMBOL_PREVIOUS = 304,                 /* PREVIOUS  */
  YYSYMBOL_PRINTER = 305,                  /* PRINTER  */
  YYSYMBOL_PRINTING = 306,                 /* PRINTING  */
  YYSYMBOL_PROCEDURE = 307,                /* PROCEDURE  */
  YYSYMBOL_PROCEDURES = 308,               /* PROCEDURES  */
  YYSYMBOL_PROCEED = 309,                  /* PROCEED  */
  YYSYMBOL_PROGRAM = 310,                  /* PROGRAM  */
  YYSYMBOL_PROGRAM_ID = 311,               /* "PROGRAM-ID"  */
  YYSYMBOL_PROGRAM_NAME = 312,             /* "Program name"  */
  YYSYMBOL_PROGRAM_POINTER = 313,          /* "PROGRAM-POINTER"  */
  YYSYMBOL_PROMPT = 314,                   /* PROMPT  */
  YYSYMBOL_QUOTE = 315,                    /* QUOTE  */
  YYSYMBOL_RANDOM = 316,                   /* RANDOM  */
  YYSYMBOL_RD = 317,                       /* RD  */
  YYSYMBOL_READ = 318,                     /* READ  */
  YYSYMBOL_RECORD = 319,                   /* RECORD  */
  YYSYMBOL_RECORDING = 320,                /* RECORDING  */
  YYSYMBOL_RECORDS = 321,                  /* RECORDS  */
  YYSYMBOL_RECURSIVE = 322,                /* RECURSIVE  */
  YYSYMBOL_REDEFINES = 323,                /* REDEFINES  */
  YYSYMBOL_REEL = 324,                     /* REEL  */
  YYSYMBOL_REFERENCE = 325,                /* REFERENCE  */
  YYSYMBOL_RELATIVE = 326,                 /* RELATIVE  */
  YYSYMBOL_RELEASE = 327,                  /* RELEASE  */
  YYSYMBOL_REMAINDER = 328,                /* REMAINDER  */
  YYSYMBOL_REMOVAL = 329,                  /* REMOVAL  */
  YYSYMBOL_RENAMES = 330,                  /* RENAMES  */
  YYSYMBOL_REPLACING = 331,                /* REPLACING  */
  YYSYMBOL_REPORT = 332,                   /* REPORT  */
  YYSYMBOL_REPORTING = 333,                /* REPORTING  */
  YYSYMBOL_REPORTS = 334,                  /* REPORTS  */
  YYSYMBOL_REPORT_FOOTING = 335,           /* "REPORT FOOTING"  */
  YYSYMBOL_REPORT_HEADING = 336,           /* "REPORT HEADING"  */
  YYSYMBOL_REPOSITORY = 337,               /* REPOSITORY  */
  YYSYMBOL_REQUIRED = 338,                 /* REQUIRED  */
  YYSYMBOL_RESERVE = 339,                  /* RESERVE  */
  YYSYMBOL_RETURN = 340,                   /* RETURN  */
  YYSYMBOL_RETURNING = 341,                /* RETURNING  */
  YYSYMBOL_REVERSE_FUNC = 342,             /* "FUNCTION REVERSE"  */
  YYSYMBOL_REVERSE_VIDEO = 343,            /* "REVERSE-VIDEO"  */
  YYSYMBOL_REWIND = 344,                   /* REWIND  */
  YYSYMBOL_REWRITE = 345,                  /* REWRITE  */
  YYSYMBOL_RIGHT = 346,                    /* RIGHT  */
  YYSYMBOL_ROLLBACK = 347,                 /* ROLLBACK  */
  YYSYMBOL_ROUNDED = 348,                  /* ROUNDED  */
  YYSYMBOL_RUN = 349,                      /* RUN  */
  YYSYMBOL_SAME = 350,                     /* SAME  */
  YYSYMBOL_SCREEN = 351,                   /* SCREEN  */
  YYSYMBOL_SCREEN_CONTROL = 352,           /* "SCREEN-CONTROL"  */
  YYSYMBOL_SCROLL = 353,                   /* SCROLL  */
  YYSYMBOL_SD = 354,                       /* SD  */
  YYSYMBOL_SEARCH = 355,                   /* SEARCH  */
  YYSYMBOL_SECTION = 356,                  /* SECTION  */
  YYSYMBOL_SECURE = 357,                   /* SECURE  */
  YYSYMBOL_SEGMENT_LIMIT = 358,            /* "SEGMENT-LIMIT"  */
  YYSYMBOL_SELECT = 359,                   /* SELECT  */
  YYSYMBOL_SEMI_COLON = 360,               /* "semi-colon"  */
  YYSYMBOL_SENTENCE = 361,                 /* SENTENCE  */
  YYSYMBOL_SEPARATE = 362,                 /* SEPARATE  */
  YYSYMBOL_SEQUENCE = 363,                 /* SEQUENCE  */
  YYSYMBOL_SEQUENTIAL = 364,               /* SEQUENTIAL  */
  YYSYMBOL_SET = 365,                      /* SET  */
  YYSYMBOL_SHARING = 366,                  /* SHARING  */
  YYSYMBOL_SIGN = 367,                     /* SIGN  */
  YYSYMBOL_SIGNED = 368,                   /* SIGNED  */
  YYSYMBOL_SIGNED_INT = 369,               /* "SIGNED-INT"  */
  YYSYMBOL_SIGNED_LONG = 370,              /* "SIGNED-LONG"  */
  YYSYMBOL_SIGNED_SHORT = 371,             /* "SIGNED-SHORT"  */
  YYSYMBOL_SIZE = 372,                     /* SIZE  */
  YYSYMBOL_SIZE_ERROR = 373,               /* "SIZE ERROR"  */
  YYSYMBOL_SORT = 374,                     /* SORT  */
  YYSYMBOL_SORT_MERGE = 375,               /* "SORT-MERGE"  */
  YYSYMBOL_SOURCE = 376,                   /* SOURCE  */
  YYSYMBOL_SOURCE_COMPUTER = 377,          /* "SOURCE-COMPUTER"  */
  YYSYMBOL_SPACE = 378,                    /* SPACE  */
  YYSYMBOL_SPECIAL_NAMES = 379,            /* "SPECIAL-NAMES"  */
  YYSYMBOL_STANDARD = 380,                 /* STANDARD  */
  YYSYMBOL_STANDARD_1 = 381,               /* "STANDARD-1"  */
  YYSYMBOL_STANDARD_2 = 382,               /* "STANDARD-2"  */
  YYSYMBOL_START = 383,                    /* START  */
  YYSYMBOL_STATUS = 384,                   /* STATUS  */
  YYSYMBOL_STOP = 385,                     /* STOP  */
  YYSYMBOL_STRING = 386,                   /* STRING  */
  YYSYMBOL_SUBSTITUTE_FUNC = 387,          /* "FUNCTION SUBSTITUTE"  */
  YYSYMBOL_SUBSTITUTE_CASE_FUNC = 388,     /* "FUNCTION SUBSTITUTE-CASE"  */
  YYSYMBOL_SUBTRACT = 389,                 /* SUBTRACT  */
  YYSYMBOL_SUM = 390,                      /* SUM  */
  YYSYMBOL_SUPPRESS = 391,                 /* SUPPRESS  */
  YYSYMBOL_SYMBOLIC = 392,                 /* SYMBOLIC  */
  YYSYMBOL_SYNCHRONIZED = 393,             /* SYNCHRONIZED  */
  YYSYMBOL_TALLYING = 394,                 /* TALLYING  */
  YYSYMBOL_TAPE = 395,                     /* TAPE  */
  YYSYMBOL_TERMINATE = 396,                /* TERMINATE  */
  YYSYMBOL_TEST = 397,                     /* TEST  */
  YYSYMBOL_THAN = 398,                     /* THAN  */
  YYSYMBOL_THEN = 399,                     /* THEN  */
  YYSYMBOL_THRU = 400,                     /* THRU  */
  YYSYMBOL_TIME = 401,                     /* TIME  */
  YYSYMBOL_TIMES = 402,                    /* TIMES  */
  YYSYMBOL_TO = 403,                       /* TO  */
  YYSYMBOL_TOK_FALSE = 404,                /* "FALSE"  */
  YYSYMBOL_TOK_FILE = 405,                 /* "FILE"  */
  YYSYMBOL_TOK_INITIAL = 406,              /* "INITIAL"  */
  YYSYMBOL_TOK_NULL = 407,                 /* "NULL"  */
  YYSYMBOL_TOK_TRUE = 408,                 /* "TRUE"  */
  YYSYMBOL_TOP = 409,                      /* TOP  */
  YYSYMBOL_TRACKS = 410,                   /* TRACKS  */
  YYSYMBOL_TRAILING = 411,                 /* TRAILING  */
  YYSYMBOL_TRANSFORM = 412,                /* TRANSFORM  */
  YYSYMBOL_TRIM_FUNCTION = 413,            /* "FUNCTION TRIM"  */
  YYSYMBOL_TYPE = 414,                     /* TYPE  */
  YYSYMBOL_UNDERLINE = 415,                /* UNDERLINE  */
  YYSYMBOL_UNIT = 416,                     /* UNIT  */
  YYSYMBOL_UNLOCK = 417,                   /* UNLOCK  */
  YYSYMBOL_UNSIGNED = 418,                 /* UNSIGNED  */
  YYSYMBOL_UNSIGNED_INT = 419,             /* "UNSIGNED-INT"  */
  YYSYMBOL_UNSIGNED_LONG = 420,            /* "UNSIGNED-LONG"  */
  YYSYMBOL_UNSIGNED_SHORT = 421,           /* "UNSIGNED-SHORT"  */
  YYSYMBOL_UNSTRING = 422,                 /* UNSTRING  */
  YYSYMBOL_UNTIL = 423,                    /* UNTIL  */
  YYSYMBOL_UP = 424,                       /* UP  */
  YYSYMBOL_UPDATE = 425,                   /* UPDATE  */
  YYSYMBOL_UPON = 426,                     /* UPON  */
  YYSYMBOL_UPON_ARGUMENT_NUMBER = 427,     /* "UPON ARGUMENT-NUMBER"  */
  YYSYMBOL_UPON_COMMAND_LINE = 428,        /* "UPON COMMAND-LINE"  */
  YYSYMBOL_UPON_ENVIRONMENT_NAME = 429,    /* "UPON ENVIRONMENT-NAME"  */
  YYSYMBOL_UPON_ENVIRONMENT_VALUE = 430,   /* "UPON ENVIRONMENT-VALUE"  */
  YYSYMBOL_UPPER_CASE_FUNC = 431,          /* "FUNCTION UPPER-CASE"  */
  YYSYMBOL_USAGE = 432,                    /* USAGE  */
  YYSYMBOL_USE = 433,                      /* USE  */
  YYSYMBOL_USING = 434,                    /* USING  */
  YYSYMBOL_VALUE = 435,                    /* VALUE  */
  YYSYMBOL_VARYING = 436,                  /* VARYING  */
  YYSYMBOL_WAIT = 437,                     /* WAIT  */
  YYSYMBOL_WHEN = 438,                     /* WHEN  */
  YYSYMBOL_WHEN_COMPILED_FUNC = 439,       /* "FUNCTION WHEN-COMPILED"  */
  YYSYMBOL_WHEN_OTHER = 440,               /* "WHEN OTHER"  */
  YYSYMBOL_WITH = 441,                     /* WITH  */
  YYSYMBOL_WORD = 442,                     /* "Identifier"  */
  YYSYMBOL_WORDS = 443,                    /* WORDS  */
  YYSYMBOL_WORKING_STORAGE = 444,          /* "WORKING-STORAGE"  */
  YYSYMBOL_WRITE = 445,                    /* WRITE  */
  YYSYMBOL_YYYYDDD = 446,                  /* YYYYDDD  */
  YYSYMBOL_YYYYMMDD = 447,                 /* YYYYMMDD  */
  YYSYMBOL_ZERO = 448,                     /* ZERO  */
  YYSYMBOL_EXEC_SQL_STATEMENT = 449,       /* "EXEC SQL statement"  */
  YYSYMBOL_EXEC_JAVA_STATEMENT = 450,      /* "EXEC JAVA statement"  */
  YYSYMBOL_451_ = 451,                     /* '+'  */
  YYSYMBOL_452_ = 452,                     /* '-'  */
  YYSYMBOL_453_ = 453,                     /* '*'  */
  YYSYMBOL_454_ = 454,                     /* '/'  */
  YYSYMBOL_UNARY_SIGN = 455,               /* UNARY_SIGN  */
  YYSYMBOL_456_ = 456,                     /* '^'  */
  YYSYMBOL_457_ = 457,                     /* '.'  */
  YYSYMBOL_458_ = 458,                     /* '='  */
  YYSYMBOL_459_ = 459,                     /* ')'  */
  YYSYMBOL_460_ = 460,                     /* '('  */
  YYSYMBOL_461_ = 461,                     /* '>'  */
  YYSYMBOL_462_ = 462,                     /* '<'  */
  YYSYMBOL_463_ = 463,                     /* ':'  */
  YYSYMBOL_464_ = 464,                     /* '&'  */
  YYSYMBOL_YYACCEPT = 465,                 /* $accept  */
  YYSYMBOL_start = 466,                    /* start  */
  YYSYMBOL_467_1 = 467,                    /* $@1  */
  YYSYMBOL_nested_list = 468,              /* nested_list  */
  YYSYMBOL_source_element = 469,           /* source_element  */
  YYSYMBOL_program_definition = 470,       /* program_definition  */
  YYSYMBOL_471_2 = 471,                    /* $@2  */
  YYSYMBOL_472_3 = 472,                    /* $@3  */
  YYSYMBOL_program_mandatory = 473,        /* program_mandatory  */
  YYSYMBOL_474_4 = 474,                    /* $@4  */
  YYSYMBOL_475_5 = 475,                    /* $@5  */
  YYSYMBOL_function_definition = 476,      /* function_definition  */
  YYSYMBOL_477_6 = 477,                    /* $@6  */
  YYSYMBOL_478_7 = 478,                    /* $@7  */
  YYSYMBOL_nested_prog = 479,              /* nested_prog  */
  YYSYMBOL_end_program = 480,              /* end_program  */
  YYSYMBOL_end_mandatory = 481,            /* end_mandatory  */
  YYSYMBOL_end_function = 482,             /* end_function  */
  YYSYMBOL_identification_division = 483,  /* identification_division  */
  YYSYMBOL_484_8 = 484,                    /* $@8  */
  YYSYMBOL_function_division = 485,        /* function_division  */
  YYSYMBOL_program_name = 486,             /* program_name  */
  YYSYMBOL_as_literal = 487,               /* as_literal  */
  YYSYMBOL_program_type = 488,             /* program_type  */
  YYSYMBOL_program_type_clause = 489,      /* program_type_clause  */
  YYSYMBOL__init_or_recurs = 490,          /* _init_or_recurs  */
  YYSYMBOL_environment_division = 491,     /* environment_division  */
  YYSYMBOL_configuration_section = 492,    /* configuration_section  */
  YYSYMBOL_configuration_list = 493,       /* configuration_list  */
  YYSYMBOL_configuration_paragraph = 494,  /* configuration_paragraph  */
  YYSYMBOL_source_computer_paragraph = 495, /* source_computer_paragraph  */
  YYSYMBOL_source_computer_entry = 496,    /* source_computer_entry  */
  YYSYMBOL_with_debugging_mode = 497,      /* with_debugging_mode  */
  YYSYMBOL_computer_name = 498,            /* computer_name  */
  YYSYMBOL_object_computer_paragraph = 499, /* object_computer_paragraph  */
  YYSYMBOL_object_computer_entry = 500,    /* object_computer_entry  */
  YYSYMBOL_object_clauses_list = 501,      /* object_clauses_list  */
  YYSYMBOL_object_clauses = 502,           /* object_clauses  */
  YYSYMBOL_object_computer_memory = 503,   /* object_computer_memory  */
  YYSYMBOL_object_char_or_word = 504,      /* object_char_or_word  */
  YYSYMBOL_object_computer_sequence = 505, /* object_computer_sequence  */
  YYSYMBOL_object_computer_segment = 506,  /* object_computer_segment  */
  YYSYMBOL_repository_paragraph = 507,     /* repository_paragraph  */
  YYSYMBOL_opt_repository = 508,           /* opt_repository  */
  YYSYMBOL_repository_list = 509,          /* repository_list  */
  YYSYMBOL_repository_name = 510,          /* repository_name  */
  YYSYMBOL_repository_literal_list = 511,  /* repository_literal_list  */
  YYSYMBOL_special_names_paragraph = 512,  /* special_names_paragraph  */
  YYSYMBOL_opt_special_names = 513,        /* opt_special_names  */
  YYSYMBOL_special_name_list = 514,        /* special_name_list  */
  YYSYMBOL_special_name = 515,             /* special_name  */
  YYSYMBOL_mnemonic_name_clause = 516,     /* mnemonic_name_clause  */
  YYSYMBOL_517_9 = 517,                    /* $@9  */
  YYSYMBOL_518_10 = 518,                   /* $@10  */
  YYSYMBOL_special_name_mnemonic_on_off_list = 519, /* special_name_mnemonic_on_off_list  */
  YYSYMBOL_special_name_mnemonic_on_off_list_mandatory = 520, /* special_name_mnemonic_on_off_list_mandatory  */
  YYSYMBOL_special_name_mnemonic_on_off = 521, /* special_name_mnemonic_on_off  */
  YYSYMBOL_on_or_off = 522,                /* on_or_off  */
  YYSYMBOL_alphabet_name_clause = 523,     /* alphabet_name_clause  */
  YYSYMBOL_524_11 = 524,                   /* $@11  */
  YYSYMBOL_alphabet_definition = 525,      /* alphabet_definition  */
  YYSYMBOL_alphabet_literal_list = 526,    /* alphabet_literal_list  */
  YYSYMBOL_alphabet_literal = 527,         /* alphabet_literal  */
  YYSYMBOL_528_12 = 528,                   /* @12  */
  YYSYMBOL_alphabet_also_sequence = 529,   /* alphabet_also_sequence  */
  YYSYMBOL_alphabet_lits = 530,            /* alphabet_lits  */
  YYSYMBOL_alphabet_also_literal = 531,    /* alphabet_also_literal  */
  YYSYMBOL_symbolic_characters_clause = 532, /* symbolic_characters_clause  */
  YYSYMBOL_symbolic_characters_list = 533, /* symbolic_characters_list  */
  YYSYMBOL_char_list = 534,                /* char_list  */
  YYSYMBOL_integer_list = 535,             /* integer_list  */
  YYSYMBOL_class_name_clause = 536,        /* class_name_clause  */
  YYSYMBOL_class_item_list = 537,          /* class_item_list  */
  YYSYMBOL_class_item = 538,               /* class_item  */
  YYSYMBOL_locale_clause = 539,            /* locale_clause  */
  YYSYMBOL_currency_sign_clause = 540,     /* currency_sign_clause  */
  YYSYMBOL_decimal_point_clause = 541,     /* decimal_point_clause  */
  YYSYMBOL_cursor_clause = 542,            /* cursor_clause  */
  YYSYMBOL_crt_status_clause = 543,        /* crt_status_clause  */
  YYSYMBOL_screen_control = 544,           /* screen_control  */
  YYSYMBOL_event_status = 545,             /* event_status  */
  YYSYMBOL_input_output_section = 546,     /* input_output_section  */
  YYSYMBOL_547_13 = 547,                   /* $@13  */
  YYSYMBOL_548_14 = 548,                   /* $@14  */
  YYSYMBOL_file_control_paragraph = 549,   /* file_control_paragraph  */
  YYSYMBOL_file_control_sequence = 550,    /* file_control_sequence  */
  YYSYMBOL_file_control_entry = 551,       /* file_control_entry  */
  YYSYMBOL_552_15 = 552,                   /* $@15  */
  YYSYMBOL_select_clause_sequence = 553,   /* select_clause_sequence  */
  YYSYMBOL_select_clause = 554,            /* select_clause  */
  YYSYMBOL_assign_clause = 555,            /* assign_clause  */
  YYSYMBOL__device = 556,                  /* _device  */
  YYSYMBOL__ext_clause = 557,              /* _ext_clause  */
  YYSYMBOL_assignment_name = 558,          /* assignment_name  */
  YYSYMBOL_assignment_device_name_list = 559, /* assignment_device_name_list  */
  YYSYMBOL_access_mode_clause = 560,       /* access_mode_clause  */
  YYSYMBOL_access_mode = 561,              /* access_mode  */
  YYSYMBOL_alternative_record_key_clause = 562, /* alternative_record_key_clause  */
  YYSYMBOL_split_key_list = 563,           /* split_key_list  */
  YYSYMBOL_564_16 = 564,                   /* $@16  */
  YYSYMBOL_split_key = 565,                /* split_key  */
  YYSYMBOL_key_is_eq = 566,                /* key_is_eq  */
  YYSYMBOL_collating_sequence_clause = 567, /* collating_sequence_clause  */
  YYSYMBOL_file_status_clause = 568,       /* file_status_clause  */
  YYSYMBOL_file_or_sort = 569,             /* file_or_sort  */
  YYSYMBOL_lock_mode_clause = 570,         /* lock_mode_clause  */
  YYSYMBOL_lock_mode = 571,                /* lock_mode  */
  YYSYMBOL_lock_with = 572,                /* lock_with  */
  YYSYMBOL_lock_records = 573,             /* lock_records  */
  YYSYMBOL_organization_clause = 574,      /* organization_clause  */
  YYSYMBOL_organization = 575,             /* organization  */
  YYSYMBOL_padding_character_clause = 576, /* padding_character_clause  */
  YYSYMBOL_record_delimiter_clause = 577,  /* record_delimiter_clause  */
  YYSYMBOL_record_key_clause = 578,        /* record_key_clause  */
  YYSYMBOL_relative_key_clause = 579,      /* relative_key_clause  */
  YYSYMBOL_reserve_clause = 580,           /* reserve_clause  */
  YYSYMBOL_sharing_clause = 581,           /* sharing_clause  */
  YYSYMBOL_sharing_option = 582,           /* sharing_option  */
  YYSYMBOL_nominal_key_clause = 583,       /* nominal_key_clause  */
  YYSYMBOL_i_o_control_paragraph = 584,    /* i_o_control_paragraph  */
  YYSYMBOL_opt_i_o_control = 585,          /* opt_i_o_control  */
  YYSYMBOL_i_o_control_list = 586,         /* i_o_control_list  */
  YYSYMBOL_i_o_control_clause = 587,       /* i_o_control_clause  */
  YYSYMBOL_same_clause = 588,              /* same_clause  */
  YYSYMBOL_same_option = 589,              /* same_option  */
  YYSYMBOL_multiple_file_tape_clause = 590, /* multiple_file_tape_clause  */
  YYSYMBOL_multiple_file_list = 591,       /* multiple_file_list  */
  YYSYMBOL_multiple_file = 592,            /* multiple_file  */
  YYSYMBOL_multiple_file_position = 593,   /* multiple_file_position  */
  YYSYMBOL_apply_clause_list = 594,        /* apply_clause_list  */
  YYSYMBOL_apply_clause = 595,             /* apply_clause  */
  YYSYMBOL_data_division = 596,            /* data_division  */
  YYSYMBOL_file_section = 597,             /* file_section  */
  YYSYMBOL_598_17 = 598,                   /* $@17  */
  YYSYMBOL_599_18 = 599,                   /* $@18  */
  YYSYMBOL_file_description_sequence = 600, /* file_description_sequence  */
  YYSYMBOL_file_description = 601,         /* file_description  */
  YYSYMBOL_file_description_sequence_without_type = 602, /* file_description_sequence_without_type  */
  YYSYMBOL_file_type = 603,                /* file_type  */
  YYSYMBOL_file_description_entry = 604,   /* file_description_entry  */
  YYSYMBOL_605_19 = 605,                   /* @19  */
  YYSYMBOL_file_description_clause_sequence = 606, /* file_description_clause_sequence  */
  YYSYMBOL_file_description_clause = 607,  /* file_description_clause  */
  YYSYMBOL_block_contains_clause = 608,    /* block_contains_clause  */
  YYSYMBOL__records_or_characters = 609,   /* _records_or_characters  */
  YYSYMBOL_record_clause = 610,            /* record_clause  */
  YYSYMBOL_record_depending = 611,         /* record_depending  */
  YYSYMBOL_opt_from_integer = 612,         /* opt_from_integer  */
  YYSYMBOL_opt_to_integer = 613,           /* opt_to_integer  */
  YYSYMBOL_label_records_clause = 614,     /* label_records_clause  */
  YYSYMBOL_label_option = 615,             /* label_option  */
  YYSYMBOL_value_of_clause = 616,          /* value_of_clause  */
  YYSYMBOL_valueof_name = 617,             /* valueof_name  */
  YYSYMBOL_data_records_clause = 618,      /* data_records_clause  */
  YYSYMBOL_linage_clause = 619,            /* linage_clause  */
  YYSYMBOL_linage_sequence = 620,          /* linage_sequence  */
  YYSYMBOL_linage_lines = 621,             /* linage_lines  */
  YYSYMBOL_linage_footing = 622,           /* linage_footing  */
  YYSYMBOL_linage_top = 623,               /* linage_top  */
  YYSYMBOL_linage_bottom = 624,            /* linage_bottom  */
  YYSYMBOL_recording_mode_clause = 625,    /* recording_mode_clause  */
  YYSYMBOL_code_set_clause = 626,          /* code_set_clause  */
  YYSYMBOL_report_clause = 627,            /* report_clause  */
  YYSYMBOL_working_storage_section = 628,  /* working_storage_section  */
  YYSYMBOL_629_20 = 629,                   /* $@20  */
  YYSYMBOL_record_description_list = 630,  /* record_description_list  */
  YYSYMBOL_record_description_list_1 = 631, /* record_description_list_1  */
  YYSYMBOL_632_21 = 632,                   /* $@21  */
  YYSYMBOL_record_description_list_2 = 633, /* record_description_list_2  */
  YYSYMBOL_exec_sql_data_statement = 634,  /* exec_sql_data_statement  */
  YYSYMBOL_data_description = 635,         /* data_description  */
  YYSYMBOL_636_22 = 636,                   /* $@22  */
  YYSYMBOL_637_23 = 637,                   /* $@23  */
  YYSYMBOL_level_number = 638,             /* level_number  */
  YYSYMBOL_level_number_88 = 639,          /* level_number_88  */
  YYSYMBOL__maybe_next_level_number = 640, /* _maybe_next_level_number  */
  YYSYMBOL_entry_name = 641,               /* entry_name  */
  YYSYMBOL_const_name = 642,               /* const_name  */
  YYSYMBOL_const_global = 643,             /* const_global  */
  YYSYMBOL_lit_or_length = 644,            /* lit_or_length  */
  YYSYMBOL_constant_entry = 645,           /* constant_entry  */
  YYSYMBOL_data_description_clause_sequence = 646, /* data_description_clause_sequence  */
  YYSYMBOL_data_description_clause = 647,  /* data_description_clause  */
  YYSYMBOL_redefines_clause = 648,         /* redefines_clause  */
  YYSYMBOL_external_clause = 649,          /* external_clause  */
  YYSYMBOL_as_extname = 650,               /* as_extname  */
  YYSYMBOL_global_clause = 651,            /* global_clause  */
  YYSYMBOL_picture_clause = 652,           /* picture_clause  */
  YYSYMBOL_usage_clause = 653,             /* usage_clause  */
  YYSYMBOL_usage = 654,                    /* usage  */
  YYSYMBOL_sign_clause = 655,              /* sign_clause  */
  YYSYMBOL_occurs_key_spec = 656,          /* occurs_key_spec  */
  YYSYMBOL_occurs_clause = 657,            /* occurs_clause  */
  YYSYMBOL_occurs_to_integer = 658,        /* occurs_to_integer  */
  YYSYMBOL_occurs_depending = 659,         /* occurs_depending  */
  YYSYMBOL__occurs_keys = 660,             /* _occurs_keys  */
  YYSYMBOL_occurs_keys = 661,              /* occurs_keys  */
  YYSYMBOL_occurs_key = 662,               /* occurs_key  */
  YYSYMBOL_occurs_key_list = 663,          /* occurs_key_list  */
  YYSYMBOL_ascending_or_descending = 664,  /* ascending_or_descending  */
  YYSYMBOL__occurs_indexed = 665,          /* _occurs_indexed  */
  YYSYMBOL_occurs_indexed = 666,           /* occurs_indexed  */
  YYSYMBOL_occurs_index_list = 667,        /* occurs_index_list  */
  YYSYMBOL_occurs_index = 668,             /* occurs_index  */
  YYSYMBOL_justified_clause = 669,         /* justified_clause  */
  YYSYMBOL_synchronized_clause = 670,      /* synchronized_clause  */
  YYSYMBOL_left_or_right = 671,            /* left_or_right  */
  YYSYMBOL_blank_clause = 672,             /* blank_clause  */
  YYSYMBOL_based_clause = 673,             /* based_clause  */
  YYSYMBOL_value_clause = 674,             /* value_clause  */
  YYSYMBOL_value_cond_clause = 675,        /* value_cond_clause  */
  YYSYMBOL_676_24 = 676,                   /* $@24  */
  YYSYMBOL_value_item_list = 677,          /* value_item_list  */
  YYSYMBOL_value_item = 678,               /* value_item  */
  YYSYMBOL_false_is = 679,                 /* false_is  */
  YYSYMBOL_renames_clause = 680,           /* renames_clause  */
  YYSYMBOL_any_length_clause = 681,        /* any_length_clause  */
  YYSYMBOL_varying_clause = 682,           /* varying_clause  */
  YYSYMBOL_local_storage_section = 683,    /* local_storage_section  */
  YYSYMBOL_684_25 = 684,                   /* $@25  */
  YYSYMBOL_linkage_section = 685,          /* linkage_section  */
  YYSYMBOL_686_26 = 686,                   /* $@26  */
  YYSYMBOL_report_section = 687,           /* report_section  */
  YYSYMBOL_688_27 = 688,                   /* $@27  */
  YYSYMBOL_opt_report_description_list = 689, /* opt_report_description_list  */
  YYSYMBOL_report_description_list = 690,  /* report_description_list  */
  YYSYMBOL_report_description_entry = 691, /* report_description_entry  */
  YYSYMBOL_report_description_options = 692, /* report_description_options  */
  YYSYMBOL_report_description_option = 693, /* report_description_option  */
  YYSYMBOL_control_clause = 694,           /* control_clause  */
  YYSYMBOL_control_field_list = 695,       /* control_field_list  */
  YYSYMBOL__final = 696,                   /* _final  */
  YYSYMBOL_identifier_list = 697,          /* identifier_list  */
  YYSYMBOL_page_limit_clause = 698,        /* page_limit_clause  */
  YYSYMBOL_heading_clause = 699,           /* heading_clause  */
  YYSYMBOL_first_detail = 700,             /* first_detail  */
  YYSYMBOL_last_heading = 701,             /* last_heading  */
  YYSYMBOL_last_detail = 702,              /* last_detail  */
  YYSYMBOL_footing_clause = 703,           /* footing_clause  */
  YYSYMBOL_page_line_column = 704,         /* page_line_column  */
  YYSYMBOL_line_or_lines = 705,            /* line_or_lines  */
  YYSYMBOL_report_group_description_list = 706, /* report_group_description_list  */
  YYSYMBOL_report_group_description_entry = 707, /* report_group_description_entry  */
  YYSYMBOL_report_group_options = 708,     /* report_group_options  */
  YYSYMBOL_report_group_option = 709,      /* report_group_option  */
  YYSYMBOL_type_clause = 710,              /* type_clause  */
  YYSYMBOL_type_option = 711,              /* type_option  */
  YYSYMBOL_next_group_clause = 712,        /* next_group_clause  */
  YYSYMBOL_column_clause = 713,            /* column_clause  */
  YYSYMBOL_sum_clause_list = 714,          /* sum_clause_list  */
  YYSYMBOL_sum_clause = 715,               /* sum_clause  */
  YYSYMBOL_ref_id_exp = 716,               /* ref_id_exp  */
  YYSYMBOL_present_when_condition = 717,   /* present_when_condition  */
  YYSYMBOL_line_clause = 718,              /* line_clause  */
  YYSYMBOL_line_keyword_clause = 719,      /* line_keyword_clause  */
  YYSYMBOL_report_line_integer_list = 720, /* report_line_integer_list  */
  YYSYMBOL_line_or_plus = 721,             /* line_or_plus  */
  YYSYMBOL__numbers = 722,                 /* _numbers  */
  YYSYMBOL_source_clause = 723,            /* source_clause  */
  YYSYMBOL_group_indicate_clause = 724,    /* group_indicate_clause  */
  YYSYMBOL__indicate = 725,                /* _indicate  */
  YYSYMBOL_report_name = 726,              /* report_name  */
  YYSYMBOL_screen_section = 727,           /* screen_section  */
  YYSYMBOL_728_28 = 728,                   /* $@28  */
  YYSYMBOL_procedure_division = 729,       /* procedure_division  */
  YYSYMBOL_730_29 = 730,                   /* $@29  */
  YYSYMBOL_731_30 = 731,                   /* $@30  */
  YYSYMBOL_procedure_using_chaining = 732, /* procedure_using_chaining  */
  YYSYMBOL_733_31 = 733,                   /* $@31  */
  YYSYMBOL_734_32 = 734,                   /* $@32  */
  YYSYMBOL_procedure_param_list = 735,     /* procedure_param_list  */
  YYSYMBOL_procedure_param = 736,          /* procedure_param  */
  YYSYMBOL_procedure_type = 737,           /* procedure_type  */
  YYSYMBOL_size_optional = 738,            /* size_optional  */
  YYSYMBOL_procedure_optional = 739,       /* procedure_optional  */
  YYSYMBOL_procedure_returning = 740,      /* procedure_returning  */
  YYSYMBOL_procedure_declaratives = 741,   /* procedure_declaratives  */
  YYSYMBOL_742_33 = 742,                   /* $@33  */
  YYSYMBOL_procedure_list = 743,           /* procedure_list  */
  YYSYMBOL_procedure = 744,                /* procedure  */
  YYSYMBOL_section_header = 745,           /* section_header  */
  YYSYMBOL_paragraph_header = 746,         /* paragraph_header  */
  YYSYMBOL_invalid_statement = 747,        /* invalid_statement  */
  YYSYMBOL_section_name = 748,             /* section_name  */
  YYSYMBOL_opt_segment = 749,              /* opt_segment  */
  YYSYMBOL_statement_list = 750,           /* statement_list  */
  YYSYMBOL_751_34 = 751,                   /* @34  */
  YYSYMBOL_752_35 = 752,                   /* @35  */
  YYSYMBOL_statements = 753,               /* statements  */
  YYSYMBOL_statement = 754,                /* statement  */
  YYSYMBOL_accept_statement = 755,         /* accept_statement  */
  YYSYMBOL_756_36 = 756,                   /* $@36  */
  YYSYMBOL_accept_body = 757,              /* accept_body  */
  YYSYMBOL_opt_at_line_column = 758,       /* opt_at_line_column  */
  YYSYMBOL_line_number = 759,              /* line_number  */
  YYSYMBOL_column_number = 760,            /* column_number  */
  YYSYMBOL_opt_accp_attr = 761,            /* opt_accp_attr  */
  YYSYMBOL_accp_attrs = 762,               /* accp_attrs  */
  YYSYMBOL_accp_attr = 763,                /* accp_attr  */
  YYSYMBOL_end_accept = 764,               /* end_accept  */
  YYSYMBOL_add_statement = 765,            /* add_statement  */
  YYSYMBOL_766_37 = 766,                   /* $@37  */
  YYSYMBOL_add_body = 767,                 /* add_body  */
  YYSYMBOL_add_to = 768,                   /* add_to  */
  YYSYMBOL_end_add = 769,                  /* end_add  */
  YYSYMBOL_allocate_statement = 770,       /* allocate_statement  */
  YYSYMBOL_771_38 = 771,                   /* $@38  */
  YYSYMBOL_allocate_body = 772,            /* allocate_body  */
  YYSYMBOL_allocate_returning = 773,       /* allocate_returning  */
  YYSYMBOL_alter_statement = 774,          /* alter_statement  */
  YYSYMBOL_alter_options = 775,            /* alter_options  */
  YYSYMBOL__proceed_to = 776,              /* _proceed_to  */
  YYSYMBOL_call_statement = 777,           /* call_statement  */
  YYSYMBOL_778_39 = 778,                   /* $@39  */
  YYSYMBOL_call_using = 779,               /* call_using  */
  YYSYMBOL_780_40 = 780,                   /* $@40  */
  YYSYMBOL_call_param_list = 781,          /* call_param_list  */
  YYSYMBOL_call_param = 782,               /* call_param  */
  YYSYMBOL_call_type = 783,                /* call_type  */
  YYSYMBOL_call_returning = 784,           /* call_returning  */
  YYSYMBOL_call_on_exception = 785,        /* call_on_exception  */
  YYSYMBOL_786_41 = 786,                   /* $@41  */
  YYSYMBOL_call_not_on_exception = 787,    /* call_not_on_exception  */
  YYSYMBOL_788_42 = 788,                   /* $@42  */
  YYSYMBOL_end_call = 789,                 /* end_call  */
  YYSYMBOL_cancel_statement = 790,         /* cancel_statement  */
  YYSYMBOL_791_43 = 791,                   /* $@43  */
  YYSYMBOL_cancel_list = 792,              /* cancel_list  */
  YYSYMBOL_close_statement = 793,          /* close_statement  */
  YYSYMBOL_794_44 = 794,                   /* $@44  */
  YYSYMBOL_close_list = 795,               /* close_list  */
  YYSYMBOL_close_option = 796,             /* close_option  */
  YYSYMBOL_reel_or_unit = 797,             /* reel_or_unit  */
  YYSYMBOL_compute_statement = 798,        /* compute_statement  */
  YYSYMBOL_799_45 = 799,                   /* $@45  */
  YYSYMBOL_compute_body = 800,             /* compute_body  */
  YYSYMBOL_end_compute = 801,              /* end_compute  */
  YYSYMBOL_comp_equal = 802,               /* comp_equal  */
  YYSYMBOL_commit_statement = 803,         /* commit_statement  */
  YYSYMBOL_continue_statement = 804,       /* continue_statement  */
  YYSYMBOL_delete_statement = 805,         /* delete_statement  */
  YYSYMBOL_806_46 = 806,                   /* $@46  */
  YYSYMBOL_end_delete = 807,               /* end_delete  */
  YYSYMBOL_delete_file_statement = 808,    /* delete_file_statement  */
  YYSYMBOL_809_47 = 809,                   /* $@47  */
  YYSYMBOL_display_statement = 810,        /* display_statement  */
  YYSYMBOL_811_48 = 811,                   /* $@48  */
  YYSYMBOL_display_body = 812,             /* display_body  */
  YYSYMBOL_with_clause = 813,              /* with_clause  */
  YYSYMBOL_disp_attrs = 814,               /* disp_attrs  */
  YYSYMBOL_disp_attr = 815,                /* disp_attr  */
  YYSYMBOL_end_display = 816,              /* end_display  */
  YYSYMBOL_divide_statement = 817,         /* divide_statement  */
  YYSYMBOL_818_49 = 818,                   /* $@49  */
  YYSYMBOL_divide_body = 819,              /* divide_body  */
  YYSYMBOL_end_divide = 820,               /* end_divide  */
  YYSYMBOL_entry_statement = 821,          /* entry_statement  */
  YYSYMBOL_822_50 = 822,                   /* $@50  */
  YYSYMBOL_evaluate_statement = 823,       /* evaluate_statement  */
  YYSYMBOL_824_51 = 824,                   /* $@51  */
  YYSYMBOL_evaluate_subject_list = 825,    /* evaluate_subject_list  */
  YYSYMBOL_evaluate_subject = 826,         /* evaluate_subject  */
  YYSYMBOL_evaluate_condition_list = 827,  /* evaluate_condition_list  */
  YYSYMBOL_evaluate_case_list = 828,       /* evaluate_case_list  */
  YYSYMBOL_evaluate_case = 829,            /* evaluate_case  */
  YYSYMBOL_830_52 = 830,                   /* $@52  */
  YYSYMBOL_evaluate_other = 831,           /* evaluate_other  */
  YYSYMBOL_832_53 = 832,                   /* $@53  */
  YYSYMBOL_evaluate_when_list = 833,       /* evaluate_when_list  */
  YYSYMBOL_evaluate_object_list = 834,     /* evaluate_object_list  */
  YYSYMBOL_evaluate_object = 835,          /* evaluate_object  */
  YYSYMBOL_opt_evaluate_thru_expr = 836,   /* opt_evaluate_thru_expr  */
  YYSYMBOL_end_evaluate = 837,             /* end_evaluate  */
  YYSYMBOL_exit_statement = 838,           /* exit_statement  */
  YYSYMBOL_839_54 = 839,                   /* $@54  */
  YYSYMBOL_exit_body = 840,                /* exit_body  */
  YYSYMBOL_free_statement = 841,           /* free_statement  */
  YYSYMBOL_842_55 = 842,                   /* $@55  */
  YYSYMBOL_generate_statement = 843,       /* generate_statement  */
  YYSYMBOL_844_56 = 844,                   /* $@56  */
  YYSYMBOL_goto_statement = 845,           /* goto_statement  */
  YYSYMBOL_846_57 = 846,                   /* $@57  */
  YYSYMBOL_goto_depending = 847,           /* goto_depending  */
  YYSYMBOL_goback_statement = 848,         /* goback_statement  */
  YYSYMBOL_849_58 = 849,                   /* $@58  */
  YYSYMBOL_if_statement = 850,             /* if_statement  */
  YYSYMBOL_851_59 = 851,                   /* $@59  */
  YYSYMBOL_852_60 = 852,                   /* $@60  */
  YYSYMBOL_if_else_sentence = 853,         /* if_else_sentence  */
  YYSYMBOL_854_61 = 854,                   /* $@61  */
  YYSYMBOL_end_if = 855,                   /* end_if  */
  YYSYMBOL_initialize_statement = 856,     /* initialize_statement  */
  YYSYMBOL_857_62 = 857,                   /* $@62  */
  YYSYMBOL_initialize_filler = 858,        /* initialize_filler  */
  YYSYMBOL_initialize_value = 859,         /* initialize_value  */
  YYSYMBOL_initialize_replacing = 860,     /* initialize_replacing  */
  YYSYMBOL_initialize_replacing_list = 861, /* initialize_replacing_list  */
  YYSYMBOL_initialize_replacing_item = 862, /* initialize_replacing_item  */
  YYSYMBOL_initialize_category = 863,      /* initialize_category  */
  YYSYMBOL_initialize_default = 864,       /* initialize_default  */
  YYSYMBOL_initiate_statement = 865,       /* initiate_statement  */
  YYSYMBOL_866_63 = 866,                   /* $@63  */
  YYSYMBOL_inspect_statement = 867,        /* inspect_statement  */
  YYSYMBOL_868_64 = 868,                   /* $@64  */
  YYSYMBOL_send_identifier = 869,          /* send_identifier  */
  YYSYMBOL_inspect_list = 870,             /* inspect_list  */
  YYSYMBOL_inspect_item = 871,             /* inspect_item  */
  YYSYMBOL_inspect_tallying = 872,         /* inspect_tallying  */
  YYSYMBOL_873_65 = 873,                   /* $@65  */
  YYSYMBOL_tallying_list = 874,            /* tallying_list  */
  YYSYMBOL_tallying_item = 875,            /* tallying_item  */
  YYSYMBOL_inspect_replacing = 876,        /* inspect_replacing  */
  YYSYMBOL_replacing_list = 877,           /* replacing_list  */
  YYSYMBOL_replacing_item = 878,           /* replacing_item  */
  YYSYMBOL_rep_keyword = 879,              /* rep_keyword  */
  YYSYMBOL_replacing_region = 880,         /* replacing_region  */
  YYSYMBOL_inspect_converting = 881,       /* inspect_converting  */
  YYSYMBOL_inspect_region = 882,           /* inspect_region  */
  YYSYMBOL__initial = 883,                 /* _initial  */
  YYSYMBOL_merge_statement = 884,          /* merge_statement  */
  YYSYMBOL_885_66 = 885,                   /* $@66  */
  YYSYMBOL_move_statement = 886,           /* move_statement  */
  YYSYMBOL_887_67 = 887,                   /* $@67  */
  YYSYMBOL_move_body = 888,                /* move_body  */
  YYSYMBOL_multiply_statement = 889,       /* multiply_statement  */
  YYSYMBOL_890_68 = 890,                   /* $@68  */
  YYSYMBOL_multiply_body = 891,            /* multiply_body  */
  YYSYMBOL_end_multiply = 892,             /* end_multiply  */
  YYSYMBOL_open_statement = 893,           /* open_statement  */
  YYSYMBOL_894_69 = 894,                   /* $@69  */
  YYSYMBOL_open_list = 895,                /* open_list  */
  YYSYMBOL_open_mode = 896,                /* open_mode  */
  YYSYMBOL_open_sharing = 897,             /* open_sharing  */
  YYSYMBOL_open_option = 898,              /* open_option  */
  YYSYMBOL_perform_statement = 899,        /* perform_statement  */
  YYSYMBOL_900_70 = 900,                   /* $@70  */
  YYSYMBOL_perform_body = 901,             /* perform_body  */
  YYSYMBOL_902_71 = 902,                   /* $@71  */
  YYSYMBOL_end_perform = 903,              /* end_perform  */
  YYSYMBOL_perform_procedure = 904,        /* perform_procedure  */
  YYSYMBOL_perform_option = 905,           /* perform_option  */
  YYSYMBOL_perform_test = 906,             /* perform_test  */
  YYSYMBOL_perform_varying_list = 907,     /* perform_varying_list  */
  YYSYMBOL_perform_varying = 908,          /* perform_varying  */
  YYSYMBOL_read_statement = 909,           /* read_statement  */
  YYSYMBOL_910_72 = 910,                   /* $@72  */
  YYSYMBOL_read_into = 911,                /* read_into  */
  YYSYMBOL_with_lock = 912,                /* with_lock  */
  YYSYMBOL_read_key = 913,                 /* read_key  */
  YYSYMBOL_read_handler = 914,             /* read_handler  */
  YYSYMBOL_end_read = 915,                 /* end_read  */
  YYSYMBOL_release_statement = 916,        /* release_statement  */
  YYSYMBOL_917_73 = 917,                   /* $@73  */
  YYSYMBOL_return_statement = 918,         /* return_statement  */
  YYSYMBOL_919_74 = 919,                   /* $@74  */
  YYSYMBOL_end_return = 920,               /* end_return  */
  YYSYMBOL_rewrite_statement = 921,        /* rewrite_statement  */
  YYSYMBOL_922_75 = 922,                   /* $@75  */
  YYSYMBOL_write_lock = 923,               /* write_lock  */
  YYSYMBOL_end_rewrite = 924,              /* end_rewrite  */
  YYSYMBOL_rollback_statement = 925,       /* rollback_statement  */
  YYSYMBOL_search_statement = 926,         /* search_statement  */
  YYSYMBOL_927_76 = 927,                   /* $@76  */
  YYSYMBOL_search_body = 928,              /* search_body  */
  YYSYMBOL_929_77 = 929,                   /* $@77  */
  YYSYMBOL_search_varying = 930,           /* search_varying  */
  YYSYMBOL_search_at_end = 931,            /* search_at_end  */
  YYSYMBOL_932_78 = 932,                   /* $@78  */
  YYSYMBOL_search_whens = 933,             /* search_whens  */
  YYSYMBOL_search_when = 934,              /* search_when  */
  YYSYMBOL_935_79 = 935,                   /* $@79  */
  YYSYMBOL_end_search = 936,               /* end_search  */
  YYSYMBOL_set_statement = 937,            /* set_statement  */
  YYSYMBOL_938_80 = 938,                   /* $@80  */
  YYSYMBOL_set_body = 939,                 /* set_body  */
  YYSYMBOL_set_environment = 940,          /* set_environment  */
  YYSYMBOL_set_to = 941,                   /* set_to  */
  YYSYMBOL_set_up_down = 942,              /* set_up_down  */
  YYSYMBOL_up_or_down = 943,               /* up_or_down  */
  YYSYMBOL_set_to_on_off_sequence = 944,   /* set_to_on_off_sequence  */
  YYSYMBOL_set_to_on_off = 945,            /* set_to_on_off  */
  YYSYMBOL_set_to_true_false_sequence = 946, /* set_to_true_false_sequence  */
  YYSYMBOL_set_to_true_false = 947,        /* set_to_true_false  */
  YYSYMBOL_sort_statement = 948,           /* sort_statement  */
  YYSYMBOL_949_81 = 949,                   /* $@81  */
  YYSYMBOL_sort_body = 950,                /* sort_body  */
  YYSYMBOL_951_82 = 951,                   /* $@82  */
  YYSYMBOL_sort_key_list = 952,            /* sort_key_list  */
  YYSYMBOL_opt_key_list = 953,             /* opt_key_list  */
  YYSYMBOL_sort_duplicates = 954,          /* sort_duplicates  */
  YYSYMBOL_sort_collating = 955,           /* sort_collating  */
  YYSYMBOL_sort_input = 956,               /* sort_input  */
  YYSYMBOL_sort_output = 957,              /* sort_output  */
  YYSYMBOL_start_statement = 958,          /* start_statement  */
  YYSYMBOL_959_83 = 959,                   /* $@83  */
  YYSYMBOL_960_84 = 960,                   /* @84  */
  YYSYMBOL_start_key = 961,                /* start_key  */
  YYSYMBOL_start_op = 962,                 /* start_op  */
  YYSYMBOL_end_start = 963,                /* end_start  */
  YYSYMBOL_stop_statement = 964,           /* stop_statement  */
  YYSYMBOL_965_85 = 965,                   /* $@85  */
  YYSYMBOL_966_86 = 966,                   /* $@86  */
  YYSYMBOL_stop_returning = 967,           /* stop_returning  */
  YYSYMBOL_string_statement = 968,         /* string_statement  */
  YYSYMBOL_969_87 = 969,                   /* $@87  */
  YYSYMBOL_string_item_list = 970,         /* string_item_list  */
  YYSYMBOL_string_item = 971,              /* string_item  */
  YYSYMBOL_opt_with_pointer = 972,         /* opt_with_pointer  */
  YYSYMBOL_end_string = 973,               /* end_string  */
  YYSYMBOL_subtract_statement = 974,       /* subtract_statement  */
  YYSYMBOL_975_88 = 975,                   /* $@88  */
  YYSYMBOL_subtract_body = 976,            /* subtract_body  */
  YYSYMBOL_end_subtract = 977,             /* end_subtract  */
  YYSYMBOL_suppress_statement = 978,       /* suppress_statement  */
  YYSYMBOL__printing = 979,                /* _printing  */
  YYSYMBOL_terminate_statement = 980,      /* terminate_statement  */
  YYSYMBOL_981_89 = 981,                   /* $@89  */
  YYSYMBOL_transform_statement = 982,      /* transform_statement  */
  YYSYMBOL_983_90 = 983,                   /* $@90  */
  YYSYMBOL_unlock_statement = 984,         /* unlock_statement  */
  YYSYMBOL_985_91 = 985,                   /* $@91  */
  YYSYMBOL_opt_record = 986,               /* opt_record  */
  YYSYMBOL_unstring_statement = 987,       /* unstring_statement  */
  YYSYMBOL_988_92 = 988,                   /* $@92  */
  YYSYMBOL_unstring_delimited = 989,       /* unstring_delimited  */
  YYSYMBOL_unstring_delimited_list = 990,  /* unstring_delimited_list  */
  YYSYMBOL_unstring_delimited_item = 991,  /* unstring_delimited_item  */
  YYSYMBOL_unstring_into = 992,            /* unstring_into  */
  YYSYMBOL_unstring_into_item = 993,       /* unstring_into_item  */
  YYSYMBOL_unstring_into_delimiter = 994,  /* unstring_into_delimiter  */
  YYSYMBOL_unstring_into_count = 995,      /* unstring_into_count  */
  YYSYMBOL_unstring_tallying = 996,        /* unstring_tallying  */
  YYSYMBOL_end_unstring = 997,             /* end_unstring  */
  YYSYMBOL_use_statement = 998,            /* use_statement  */
  YYSYMBOL_use_exception = 999,            /* use_exception  */
  YYSYMBOL_use_global = 1000,              /* use_global  */
  YYSYMBOL_use_exception_target = 1001,    /* use_exception_target  */
  YYSYMBOL__after = 1002,                  /* _after  */
  YYSYMBOL__standard = 1003,               /* _standard  */
  YYSYMBOL_exception_or_error = 1004,      /* exception_or_error  */
  YYSYMBOL_exception_or_overflow = 1005,   /* exception_or_overflow  */
  YYSYMBOL_not_exception_or_overflow = 1006, /* not_exception_or_overflow  */
  YYSYMBOL__procedure = 1007,              /* _procedure  */
  YYSYMBOL_use_debugging = 1008,           /* use_debugging  */
  YYSYMBOL_use_debugging_target = 1009,    /* use_debugging_target  */
  YYSYMBOL_use_reporting = 1010,           /* use_reporting  */
  YYSYMBOL_write_statement = 1011,         /* write_statement  */
  YYSYMBOL_1012_93 = 1012,                 /* $@93  */
  YYSYMBOL_write_from = 1013,              /* write_from  */
  YYSYMBOL_write_option = 1014,            /* write_option  */
  YYSYMBOL_before_or_after = 1015,         /* before_or_after  */
  YYSYMBOL_write_handler = 1016,           /* write_handler  */
  YYSYMBOL_end_write = 1017,               /* end_write  */
  YYSYMBOL_on_accp_exception = 1018,       /* on_accp_exception  */
  YYSYMBOL_on_disp_exception = 1019,       /* on_disp_exception  */
  YYSYMBOL_opt_on_exception = 1020,        /* opt_on_exception  */
  YYSYMBOL_1021_94 = 1021,                 /* $@94  */
  YYSYMBOL_opt_not_on_exception = 1022,    /* opt_not_on_exception  */
  YYSYMBOL_1023_95 = 1023,                 /* $@95  */
  YYSYMBOL_on_size_error = 1024,           /* on_size_error  */
  YYSYMBOL_opt_on_size_error = 1025,       /* opt_on_size_error  */
  YYSYMBOL_1026_96 = 1026,                 /* $@96  */
  YYSYMBOL_opt_not_on_size_error = 1027,   /* opt_not_on_size_error  */
  YYSYMBOL_1028_97 = 1028,                 /* $@97  */
  YYSYMBOL_on_overflow = 1029,             /* on_overflow  */
  YYSYMBOL_opt_on_overflow = 1030,         /* opt_on_overflow  */
  YYSYMBOL_1031_98 = 1031,                 /* $@98  */
  YYSYMBOL_opt_not_on_overflow = 1032,     /* opt_not_on_overflow  */
  YYSYMBOL_1033_99 = 1033,                 /* $@99  */
  YYSYMBOL_at_end = 1034,                  /* at_end  */
  YYSYMBOL_at_end_sentence = 1035,         /* at_end_sentence  */
  YYSYMBOL_1036_100 = 1036,                /* $@100  */
  YYSYMBOL_not_at_end_sentence = 1037,     /* not_at_end_sentence  */
  YYSYMBOL_1038_101 = 1038,                /* $@101  */
  YYSYMBOL_at_eop = 1039,                  /* at_eop  */
  YYSYMBOL_at_eop_sentence = 1040,         /* at_eop_sentence  */
  YYSYMBOL_1041_102 = 1041,                /* $@102  */
  YYSYMBOL_not_at_eop_sentence = 1042,     /* not_at_eop_sentence  */
  YYSYMBOL_1043_103 = 1043,                /* $@103  */
  YYSYMBOL_opt_invalid_key = 1044,         /* opt_invalid_key  */
  YYSYMBOL_invalid_key = 1045,             /* invalid_key  */
  YYSYMBOL_invalid_key_sentence = 1046,    /* invalid_key_sentence  */
  YYSYMBOL_1047_104 = 1047,                /* $@104  */
  YYSYMBOL_not_invalid_key_sentence = 1048, /* not_invalid_key_sentence  */
  YYSYMBOL_1049_105 = 1049,                /* $@105  */
  YYSYMBOL__opt_scroll_lines = 1050,       /* _opt_scroll_lines  */
  YYSYMBOL_condition = 1051,               /* condition  */
  YYSYMBOL_expr = 1052,                    /* expr  */
  YYSYMBOL_partial_expr = 1053,            /* partial_expr  */
  YYSYMBOL_1054_106 = 1054,                /* $@106  */
  YYSYMBOL_expr_tokens = 1055,             /* expr_tokens  */
  YYSYMBOL_expr_token = 1056,              /* expr_token  */
  YYSYMBOL_eq = 1057,                      /* eq  */
  YYSYMBOL_gt = 1058,                      /* gt  */
  YYSYMBOL_lt = 1059,                      /* lt  */
  YYSYMBOL_ge = 1060,                      /* ge  */
  YYSYMBOL_le = 1061,                      /* le  */
  YYSYMBOL_exp_list = 1062,                /* exp_list  */
  YYSYMBOL_e_sep = 1063,                   /* e_sep  */
  YYSYMBOL_exp = 1064,                     /* exp  */
  YYSYMBOL_linage_counter = 1065,          /* linage_counter  */
  YYSYMBOL_arithmetic_x_list = 1066,       /* arithmetic_x_list  */
  YYSYMBOL_arithmetic_x = 1067,            /* arithmetic_x  */
  YYSYMBOL_record_name = 1068,             /* record_name  */
  YYSYMBOL_table_name = 1069,              /* table_name  */
  YYSYMBOL_file_name_list = 1070,          /* file_name_list  */
  YYSYMBOL_file_name = 1071,               /* file_name  */
  YYSYMBOL_mnemonic_name_list = 1072,      /* mnemonic_name_list  */
  YYSYMBOL_mnemonic_name = 1073,           /* mnemonic_name  */
  YYSYMBOL_procedure_name_list = 1074,     /* procedure_name_list  */
  YYSYMBOL_procedure_name = 1075,          /* procedure_name  */
  YYSYMBOL_label = 1076,                   /* label  */
  YYSYMBOL_integer_label = 1077,           /* integer_label  */
  YYSYMBOL_reference_list = 1078,          /* reference_list  */
  YYSYMBOL_reference = 1079,               /* reference  */
  YYSYMBOL_no_reference_list = 1080,       /* no_reference_list  */
  YYSYMBOL_opt_reference = 1081,           /* opt_reference  */
  YYSYMBOL_reference_or_literal = 1082,    /* reference_or_literal  */
  YYSYMBOL_undefined_word = 1083,          /* undefined_word  */
  YYSYMBOL_target_x_list = 1084,           /* target_x_list  */
  YYSYMBOL_target_x = 1085,                /* target_x  */
  YYSYMBOL_x_list = 1086,                  /* x_list  */
  YYSYMBOL_x = 1087,                       /* x  */
  YYSYMBOL_arith_x = 1088,                 /* arith_x  */
  YYSYMBOL_prog_or_entry = 1089,           /* prog_or_entry  */
  YYSYMBOL_alnum_or_id = 1090,             /* alnum_or_id  */
  YYSYMBOL_simple_value = 1091,            /* simple_value  */
  YYSYMBOL_simple_all_value = 1092,        /* simple_all_value  */
  YYSYMBOL_id_or_lit = 1093,               /* id_or_lit  */
  YYSYMBOL_id_or_lit_or_func = 1094,       /* id_or_lit_or_func  */
  YYSYMBOL_num_id_or_lit = 1095,           /* num_id_or_lit  */
  YYSYMBOL_identifier = 1096,              /* identifier  */
  YYSYMBOL_identifier_1 = 1097,            /* identifier_1  */
  YYSYMBOL_qualified_word = 1098,          /* qualified_word  */
  YYSYMBOL_subref = 1099,                  /* subref  */
  YYSYMBOL_refmod = 1100,                  /* refmod  */
  YYSYMBOL_integer = 1101,                 /* integer  */
  YYSYMBOL_literal = 1102,                 /* literal  */
  YYSYMBOL_basic_literal = 1103,           /* basic_literal  */
  YYSYMBOL_basic_value = 1104,             /* basic_value  */
  YYSYMBOL_function = 1105,                /* function  */
  YYSYMBOL_func_refmod = 1106,             /* func_refmod  */
  YYSYMBOL_func_args = 1107,               /* func_args  */
  YYSYMBOL_list_func_args = 1108,          /* list_func_args  */
  YYSYMBOL_trim_args = 1109,               /* trim_args  */
  YYSYMBOL_numvalc_args = 1110,            /* numvalc_args  */
  YYSYMBOL_locale_dt_args = 1111,          /* locale_dt_args  */
  YYSYMBOL_not_const_word = 1112,          /* not_const_word  */
  YYSYMBOL_flag_all = 1113,                /* flag_all  */
  YYSYMBOL_flag_duplicates = 1114,         /* flag_duplicates  */
  YYSYMBOL_flag_initialized = 1115,        /* flag_initialized  */
  YYSYMBOL_flag_next = 1116,               /* flag_next  */
  YYSYMBOL_flag_not = 1117,                /* flag_not  */
  YYSYMBOL_flag_optional = 1118,           /* flag_optional  */
  YYSYMBOL_flag_rounded = 1119,            /* flag_rounded  */
  YYSYMBOL_flag_separate = 1120,           /* flag_separate  */
  YYSYMBOL_in_of = 1121,                   /* in_of  */
  YYSYMBOL_records = 1122,                 /* records  */
  YYSYMBOL_with_dups = 1123,               /* with_dups  */
  YYSYMBOL_coll_sequence = 1124,           /* coll_sequence  */
  YYSYMBOL__advancing = 1125,              /* _advancing  */
  YYSYMBOL__also = 1126,                   /* _also  */
  YYSYMBOL__are = 1127,                    /* _are  */
  YYSYMBOL__area = 1128,                   /* _area  */
  YYSYMBOL__as = 1129,                     /* _as  */
  YYSYMBOL__at = 1130,                     /* _at  */
  YYSYMBOL__binary = 1131,                 /* _binary  */
  YYSYMBOL__by = 1132,                     /* _by  */
  YYSYMBOL__character = 1133,              /* _character  */
  YYSYMBOL__characters = 1134,             /* _characters  */
  YYSYMBOL__contains = 1135,               /* _contains  */
  YYSYMBOL__data = 1136,                   /* _data  */
  YYSYMBOL__file = 1137,                   /* _file  */
  YYSYMBOL__for = 1138,                    /* _for  */
  YYSYMBOL__from = 1139,                   /* _from  */
  YYSYMBOL__in = 1140,                     /* _in  */
  YYSYMBOL__is = 1141,                     /* _is  */
  YYSYMBOL__is_are = 1142,                 /* _is_are  */
  YYSYMBOL__key = 1143,                    /* _key  */
  YYSYMBOL__line_or_lines = 1144,          /* _line_or_lines  */
  YYSYMBOL__lines = 1145,                  /* _lines  */
  YYSYMBOL__literal = 1146,                /* _literal  */
  YYSYMBOL__mode = 1147,                   /* _mode  */
  YYSYMBOL__number = 1148,                 /* _number  */
  YYSYMBOL__of = 1149,                     /* _of  */
  YYSYMBOL__on = 1150,                     /* _on  */
  YYSYMBOL__in_order = 1151,               /* _in_order  */
  YYSYMBOL__other = 1152,                  /* _other  */
  YYSYMBOL__program = 1153,                /* _program  */
  YYSYMBOL__record = 1154,                 /* _record  */
  YYSYMBOL__right = 1155,                  /* _right  */
  YYSYMBOL__set = 1156,                    /* _set  */
  YYSYMBOL__sign = 1157,                   /* _sign  */
  YYSYMBOL__sign_is = 1158,                /* _sign_is  */
  YYSYMBOL__size = 1159,                   /* _size  */
  YYSYMBOL__status = 1160,                 /* _status  */
  YYSYMBOL__tape = 1161,                   /* _tape  */
  YYSYMBOL__than = 1162,                   /* _than  */
  YYSYMBOL__then = 1163,                   /* _then  */
  YYSYMBOL__times = 1164,                  /* _times  */
  YYSYMBOL__to = 1165,                     /* _to  */
  YYSYMBOL__when = 1166,                   /* _when  */
  YYSYMBOL__with = 1167,                   /* _with  */
  YYSYMBOL_exec_sql_statement = 1168,      /* exec_sql_statement  */
  YYSYMBOL_exec_java_statement = 1169      /* exec_java_statement  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   5516

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  465
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  705
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1556
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  2293

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   706


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   464,     2,
     460,   459,   453,   451,     2,   452,   457,   454,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   463,     2,
     462,   458,   461,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   456,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   455
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   776,   776,   776,   820,   821,   825,   826,   831,   832,
     830,   845,   846,   844,   859,   860,   858,   865,   866,   867,
     870,   871,   899,   925,   957,   956,  1002,  1046,  1047,  1051,
    1052,  1055,  1056,  1060,  1067,  1074,  1078,  1082,  1094,  1095,
    1105,  1106,  1115,  1116,  1120,  1121,  1122,  1123,  1132,  1135,
    1136,  1137,  1138,  1142,  1149,  1158,  1161,  1162,  1163,  1164,
    1168,  1169,  1173,  1174,  1175,  1179,  1186,  1187,  1191,  1198,
    1210,  1213,  1214,  1218,  1219,  1223,  1227,  1234,  1235,  1245,
    1248,  1249,  1250,  1254,  1255,  1259,  1260,  1261,  1262,  1263,
    1264,  1265,  1266,  1267,  1268,  1269,  1276,  1287,  1286,  1298,
    1297,  1306,  1320,  1334,  1348,  1364,  1365,  1369,  1370,  1374,
    1385,  1386,  1394,  1393,  1405,  1406,  1407,  1408,  1409,  1417,
    1418,  1423,  1424,  1426,  1425,  1437,  1438,  1442,  1443,  1444,
    1445,  1446,  1447,  1451,  1452,  1453,  1454,  1455,  1456,  1463,
    1474,  1486,  1487,  1491,  1492,  1499,  1508,  1509,  1513,  1514,
    1528,  1543,  1610,  1621,  1628,  1635,  1641,  1648,  1649,  1653,
    1652,  1662,  1661,  1677,  1678,  1681,  1682,  1687,  1686,  1707,
    1708,  1712,  1713,  1714,  1715,  1716,  1717,  1718,  1719,  1720,
    1721,  1722,  1723,  1724,  1725,  1726,  1733,  1737,  1742,  1749,
    1750,  1751,  1754,  1755,  1759,  1766,  1767,  1774,  1794,  1795,
    1801,  1805,  1806,  1807,  1814,  1834,  1877,  1877,  1881,  1885,
    1901,  1902,  1903,  1909,  1919,  1928,  1930,  1931,  1937,  1941,
    1942,  1943,  1946,  1947,  1948,  1952,  1956,  1957,  1963,  1964,
    1968,  1977,  1986,  1995,  2004,  2019,  2029,  2036,  2045,  2083,
    2090,  2091,  2098,  2102,  2103,  2104,  2110,  2117,  2118,  2121,
    2122,  2123,  2124,  2128,  2129,  2133,  2134,  2140,  2165,  2166,
    2167,  2168,  2174,  2181,  2182,  2186,  2189,  2190,  2196,  2197,
    2198,  2202,  2206,  2210,  2214,  2218,  2228,  2229,  2243,  2244,
    2244,  2247,  2246,  2259,  2260,  2264,  2276,  2285,  2289,  2290,
    2300,  2299,  2317,  2318,  2322,  2329,  2336,  2337,  2338,  2339,
    2340,  2341,  2342,  2343,  2344,  2345,  2352,  2356,  2356,  2356,
    2362,  2374,  2399,  2423,  2424,  2431,  2432,  2436,  2437,  2444,
    2451,  2452,  2459,  2463,  2472,  2473,  2479,  2489,  2507,  2508,
    2512,  2513,  2514,  2518,  2525,  2532,  2542,  2549,  2567,  2571,
    2582,  2583,  2583,  2594,  2595,  2599,  2599,  2616,  2617,  2619,
    2620,  2621,  2625,  2632,  2639,  2641,  2640,  2666,  2665,  2691,
    2695,  2702,  2704,  2726,  2731,  2737,  2746,  2754,  2755,  2763,
    2764,  2765,  2769,  2789,  2793,  2802,  2803,  2804,  2805,  2806,
    2807,  2808,  2809,  2810,  2811,  2812,  2813,  2814,  2815,  2816,
    2817,  2824,  2846,  2868,  2869,  2881,  2901,  2908,  2909,  2913,
    2914,  2915,  2920,  2925,  2926,  2927,  2928,  2929,  2930,  2931,
    2932,  2937,  2942,  2943,  2944,  2945,  2946,  2947,  2948,  2949,
    2950,  2951,  2952,  2953,  2954,  2955,  2956,  2957,  2958,  2959,
    2960,  2968,  2976,  2984,  2991,  2996,  3006,  3007,  3008,  3012,
    3029,  3030,  3033,  3034,  3040,  3040,  3043,  3067,  3083,  3084,
    3088,  3089,  3092,  3092,  3095,  3102,  3103,  3108,  3118,  3125,
    3128,  3129,  3130,  3137,  3144,  3169,  3173,  3173,  3178,  3179,
    3183,  3184,  3187,  3188,  3201,  3213,  3233,  3246,  3256,  3258,
    3257,  3278,  3279,  3279,  3292,  3294,  3293,  3305,  3306,  3310,
    3311,  3320,  3327,  3330,  3334,  3338,  3339,  3340,  3347,  3348,
    3352,  3355,  3355,  3358,  3359,  3365,  3370,  3371,  3374,  3375,
    3378,  3379,  3382,  3383,  3386,  3387,  3391,  3392,  3393,  3397,
    3398,  3401,  3402,  3406,  3410,  3411,  3415,  3416,  3417,  3418,
    3419,  3420,  3421,  3422,  3423,  3424,  3425,  3426,  3427,  3428,
    3429,  3430,  3434,  3438,  3439,  3440,  3441,  3442,  3443,  3444,
    3448,  3452,  3453,  3454,  3458,  3459,  3463,  3467,  3472,  3476,
    3480,  3484,  3485,  3489,  3490,  3494,  3495,  3496,  3499,  3499,
    3499,  3502,  3506,  3509,  3509,  3512,  3519,  3520,  3520,  3530,
    3532,  3546,  3531,  3573,  3575,  3574,  3581,  3580,  3589,  3590,
    3595,  3602,  3604,  3608,  3618,  3620,  3628,  3636,  3665,  3696,
    3698,  3708,  3713,  3724,  3725,  3725,  3752,  3753,  3757,  3758,
    3759,  3760,  3776,  3788,  3819,  3856,  3868,  3871,  3872,  3881,
    3885,  3881,  3898,  3919,  3923,  3924,  3925,  3926,  3927,  3928,
    3929,  3930,  3931,  3932,  3933,  3934,  3935,  3936,  3937,  3938,
    3939,  3940,  3941,  3942,  3943,  3944,  3945,  3946,  3947,  3948,
    3949,  3950,  3951,  3952,  3953,  3954,  3955,  3956,  3957,  3958,
    3959,  3960,  3961,  3962,  3963,  3964,  3965,  3966,  3967,  3968,
    3969,  3970,  3971,  3972,  3973,  3974,  3997,  3996,  4009,  4013,
    4017,  4021,  4025,  4029,  4033,  4037,  4041,  4045,  4049,  4053,
    4057,  4061,  4065,  4069,  4073,  4080,  4081,  4082,  4083,  4084,
    4085,  4089,  4093,  4094,  4097,  4098,  4102,  4103,  4107,  4108,
    4109,  4110,  4111,  4112,  4113,  4114,  4118,  4122,  4126,  4131,
    4132,  4133,  4134,  4135,  4136,  4140,  4141,  4150,  4150,  4156,
    4160,  4164,  4170,  4171,  4175,  4176,  4185,  4185,  4190,  4194,
    4201,  4202,  4211,  4217,  4218,  4222,  4222,  4230,  4230,  4240,
    4242,  4241,  4250,  4251,  4256,  4263,  4270,  4272,  4276,  4284,
    4295,  4296,  4297,  4302,  4306,  4305,  4317,  4321,  4320,  4331,
    4332,  4341,  4341,  4345,  4346,  4350,  4362,  4362,  4366,  4367,
    4378,  4379,  4380,  4381,  4382,  4385,  4385,  4393,  4393,  4399,
    4406,  4407,  4410,  4410,  4417,  4430,  4443,  4443,  4454,  4455,
    4464,  4464,  4484,  4483,  4496,  4500,  4504,  4508,  4512,  4516,
    4520,  4525,  4529,  4536,  4537,  4538,  4542,  4543,  4548,  4549,
    4550,  4551,  4552,  4553,  4554,  4555,  4556,  4557,  4561,  4565,
    4569,  4574,  4575,  4579,  4580,  4589,  4589,  4595,  4599,  4603,
    4607,  4611,  4618,  4619,  4628,  4628,  4650,  4649,  4668,  4669,
    4680,  4689,  4694,  4702,  4731,  4732,  4738,  4737,  4753,  4757,
    4756,  4771,  4772,  4777,  4778,  4789,  4818,  4819,  4820,  4823,
    4824,  4828,  4829,  4838,  4838,  4843,  4844,  4852,  4860,  4868,
    4886,  4911,  4911,  4924,  4924,  4937,  4937,  4946,  4950,  4963,
    4963,  4976,  4978,  4976,  4989,  4994,  4998,  4997,  5011,  5012,
    5021,  5021,  5029,  5030,  5034,  5035,  5036,  5040,  5041,  5046,
    5047,  5052,  5056,  5057,  5058,  5059,  5060,  5061,  5062,  5066,
    5067,  5076,  5076,  5089,  5088,  5098,  5099,  5100,  5104,  5105,
    5109,  5110,  5111,  5117,  5117,  5122,  5123,  5127,  5128,  5129,
    5130,  5131,  5132,  5138,  5142,  5143,  5147,  5152,  5156,  5157,
    5158,  5159,  5160,  5164,  5190,  5203,  5204,  5208,  5208,  5216,
    5216,  5226,  5226,  5231,  5235,  5247,  5247,  5253,  5257,  5264,
    5265,  5274,  5274,  5278,  5279,  5293,  5294,  5295,  5296,  5300,
    5301,  5305,  5306,  5307,  5319,  5319,  5324,  5329,  5328,  5338,
    5345,  5346,  5350,  5355,  5364,  5367,  5371,  5376,  5383,  5390,
    5391,  5395,  5396,  5401,  5413,  5413,  5440,  5441,  5445,  5446,
    5450,  5454,  5458,  5462,  5469,  5470,  5476,  5477,  5478,  5482,
    5483,  5492,  5492,  5507,  5507,  5518,  5519,  5528,  5528,  5545,
    5546,  5550,  5557,  5558,  5567,  5580,  5580,  5586,  5591,  5590,
    5601,  5602,  5606,  5608,  5607,  5618,  5619,  5624,  5623,  5634,
    5635,  5644,  5644,  5649,  5650,  5651,  5652,  5653,  5659,  5668,
    5672,  5681,  5688,  5689,  5695,  5696,  5700,  5709,  5710,  5714,
    5718,  5730,  5730,  5736,  5735,  5752,  5755,  5776,  5777,  5780,
    5781,  5785,  5786,  5791,  5796,  5804,  5816,  5821,  5829,  5845,
    5846,  5845,  5866,  5867,  5875,  5876,  5877,  5878,  5879,  5883,
    5884,  5893,  5893,  5898,  5898,  5905,  5906,  5907,  5916,  5916,
    5925,  5926,  5930,  5931,  5932,  5936,  5937,  5941,  5942,  5951,
    5951,  5957,  5961,  5965,  5972,  5973,  5982,  5989,  5990,  5998,
    5998,  6011,  6011,  6027,  6027,  6036,  6038,  6039,  6048,  6048,
    6058,  6059,  6064,  6065,  6070,  6077,  6078,  6083,  6090,  6091,
    6095,  6096,  6100,  6101,  6105,  6106,  6115,  6116,  6117,  6121,
    6145,  6148,  6156,  6166,  6171,  6176,  6181,  6188,  6189,  6192,
    6193,  6197,  6197,  6201,  6201,  6205,  6205,  6208,  6209,  6213,
    6220,  6221,  6225,  6237,  6237,  6254,  6255,  6260,  6263,  6267,
    6271,  6278,  6279,  6282,  6283,  6284,  6288,  6289,  6302,  6310,
    6317,  6319,  6318,  6328,  6330,  6329,  6344,  6348,  6350,  6349,
    6360,  6362,  6361,  6378,  6384,  6386,  6385,  6395,  6397,  6396,
    6412,  6417,  6422,  6432,  6431,  6443,  6442,  6458,  6463,  6468,
    6478,  6477,  6489,  6488,  6503,  6504,  6508,  6513,  6518,  6528,
    6527,  6539,  6538,  6555,  6558,  6570,  6577,  6584,  6584,  6594,
    6595,  6597,  6598,  6599,  6600,  6601,  6602,  6604,  6605,  6606,
    6607,  6608,  6609,  6611,  6612,  6614,  6615,  6616,  6619,  6621,
    6622,  6623,  6625,  6626,  6627,  6629,  6630,  6632,  6633,  6634,
    6635,  6636,  6638,  6639,  6640,  6641,  6642,  6643,  6645,  6646,
    6647,  6648,  6649,  6650,  6652,  6653,  6656,  6656,  6656,  6657,
    6657,  6658,  6658,  6659,  6659,  6659,  6660,  6660,  6660,  6665,
    6666,  6669,  6670,  6671,  6675,  6676,  6677,  6678,  6679,  6680,
    6681,  6682,  6683,  6694,  6706,  6721,  6722,  6727,  6733,  6755,
    6775,  6779,  6795,  6809,  6810,  6815,  6821,  6822,  6827,  6836,
    6837,  6838,  6842,  6853,  6854,  6858,  6868,  6869,  6873,  6874,
    6878,  6879,  6885,  6905,  6906,  6910,  6911,  6915,  6916,  6920,
    6921,  6922,  6923,  6924,  6925,  6926,  6927,  6928,  6932,  6933,
    6934,  6935,  6936,  6937,  6938,  6942,  6943,  6947,  6948,  6952,
    6953,  6957,  6958,  6969,  6970,  6974,  6975,  6976,  6980,  6981,
    6982,  6990,  6994,  6995,  6996,  6997,  7001,  7002,  7006,  7016,
    7030,  7053,  7065,  7066,  7076,  7077,  7081,  7082,  7083,  7084,
    7085,  7086,  7087,  7095,  7099,  7103,  7107,  7111,  7115,  7119,
    7123,  7127,  7131,  7135,  7139,  7146,  7147,  7148,  7152,  7153,
    7157,  7158,  7163,  7170,  7177,  7187,  7194,  7204,  7211,  7225,
    7235,  7236,  7240,  7241,  7245,  7246,  7250,  7251,  7252,  7256,
    7257,  7261,  7262,  7266,  7267,  7271,  7272,  7279,  7279,  7280,
    7280,  7281,  7281,  7282,  7282,  7284,  7284,  7285,  7285,  7286,
    7286,  7287,  7287,  7288,  7288,  7289,  7289,  7290,  7290,  7291,
    7291,  7292,  7292,  7293,  7293,  7294,  7294,  7295,  7295,  7296,
    7296,  7297,  7297,  7298,  7298,  7299,  7299,  7300,  7300,  7301,
    7301,  7301,  7302,  7302,  7303,  7303,  7303,  7304,  7304,  7305,
    7305,  7306,  7306,  7307,  7307,  7308,  7308,  7309,  7309,  7310,
    7310,  7310,  7311,  7311,  7312,  7312,  7313,  7313,  7314,  7314,
    7315,  7315,  7316,  7316,  7317,  7317,  7317,  7318,  7318,  7319,
    7319,  7320,  7320,  7321,  7321,  7322,  7322,  7323,  7323,  7324,
    7324,  7326,  7326,  7327,  7327,  7334,  7369
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ACCEPT", "ACCESS",
  "ADD", "ADDRESS", "ADVANCING", "AFTER", "ALL", "ALLOCATE", "ALPHABET",
  "ALPHABETIC", "\"ALPHABETIC-LOWER\"", "\"ALPHABETIC-UPPER\"",
  "ALPHANUMERIC", "\"ALPHANUMERIC-EDITED\"", "ALSO", "ALTER", "ALTERNATE",
  "AND", "ANY", "APPLY", "ARE", "AREA", "\"ARGUMENT-NUMBER\"",
  "\"ARGUMENT-VALUE\"", "AS", "ASCENDING", "ASSIGN", "AT", "AUTO",
  "AUTOMATIC", "\"BACKGROUND-COLOR\"", "BASED", "BEFORE", "BELL", "BINARY",
  "\"BINARY-C-LONG\"", "\"BINARY-CHAR\"", "\"BINARY-DOUBLE\"",
  "\"BINARY-LONG\"", "\"BINARY-SHORT\"", "BLANK", "\"BLANK-LINE\"",
  "\"BLANK-SCREEN\"", "BLINK", "BLOCK", "BOTTOM", "BY", "\"BYTE-LENGTH\"",
  "CALL", "CANCEL", "CH", "CHAINING", "CHARACTER", "CHARACTERS", "CLASS",
  "CLASS_NAME", "CLOSE", "\"CLOSE-NOFEED\"", "CODE", "\"CODE-SET\"",
  "COLLATING", "COL", "COLS", "COLUMN", "COLUMNS", "COMMA",
  "\"COMMAND-LINE\"", "\"comma delimiter\"", "COMMIT",
  "\"COMMITMENT-CONTROL\"", "COMMON", "COMP", "COMPUTE", "\"COMP-1\"",
  "\"COMP-2\"", "\"COMP-3\"", "\"COMP-4\"", "\"COMP-5\"", "\"COMP-X\"",
  "\"FUNCTION CONCATENATE\"", "CONFIGURATION", "CONSTANT", "CONTAINS",
  "CONTENT", "CONTINUE", "CONTROL", "CONTROLS", "\"CONTROL FOOTING\"",
  "\"CONTROL HEADING\"", "CONVERTING", "\"CORE-INDEX\"", "CORRESPONDING",
  "COUNT", "CRT", "CURRENCY", "\"FUNCTION CURRENT-DATE\"", "CURSOR",
  "CYCLE", "\"CYL-OVERFLOW\"", "DATA", "DATE", "DAY", "\"DAY-OF-WEEK\"",
  "DE", "DEBUGGING", "\"DECIMAL-POINT\"", "DECLARATIVES", "DEFAULT",
  "DELETE", "DELIMITED", "DELIMITER", "DEPENDING", "DESCENDING", "DETAIL",
  "DISK", "DISPLAY", "DIVIDE", "DIVISION", "DOWN", "DUPLICATES", "DYNAMIC",
  "EBCDIC", "ELSE", "END", "\"END-ACCEPT\"", "\"END-ADD\"", "\"END-CALL\"",
  "\"END-COMPUTE\"", "\"END-DELETE\"", "\"END-DISPLAY\"", "\"END-DIVIDE\"",
  "\"END-EVALUATE\"", "\"END FUNCTION\"", "\"END-IF\"", "\"END-MULTIPLY\"",
  "\"END-PERFORM\"", "\"END PROGRAM\"", "\"END-READ\"", "\"END-RETURN\"",
  "\"END-REWRITE\"", "\"END-SEARCH\"", "\"END-START\"", "\"END-STRING\"",
  "\"END-SUBTRACT\"", "\"END-UNSTRING\"", "\"END-WRITE\"", "ENTRY",
  "ENVIRONMENT", "\"ENVIRONMENT-NAME\"", "\"ENVIRONMENT-VALUE\"", "EOL",
  "EOP", "EOS", "EQUAL", "EQUALS", "ERASE", "ERROR", "ESCAPE", "EVALUATE",
  "\"EVENT-STATUS\"", "EXCEPTION", "EXCLUSIVE", "EXIT", "EXTEND",
  "EXTERNAL", "FD", "\"FILE-CONTROL\"", "\"FILE-ID\"", "FILLER", "FINAL",
  "FIRST", "FOOTING", "FOR", "\"FOREGROUND-COLOR\"", "FOREVER",
  "\"FORMS-OVERLAY\"", "FREE", "FROM", "FULL", "FUNCTION",
  "\"FUNCTION-ID\"", "\"FUNCTION\"", "GE", "GENERATE", "GIVING", "GLOBAL",
  "GO", "GOBACK", "GREATER", "GROUP", "HEADING", "HIGHLIGHT",
  "\"HIGH-VALUE\"", "IDENTIFICATION", "IF", "IGNORE", "IGNORING", "IN",
  "INDEX", "INDEXED", "INDICATE", "INITIALIZE", "INITIALIZED", "INITIATE",
  "INPUT", "\"INPUT-OUTPUT\"", "INSPECT", "INTO", "INTRINSIC", "INVALID",
  "\"INVALID KEY\"", "IS", "\"I-O\"", "\"I-O-CONTROL\"", "JUSTIFIED",
  "KEY", "LABEL", "LAST", "\"LAST DETAIL\"", "LE", "LEADING", "LEFT",
  "LENGTH", "LESS", "LEVEL_NUMBER_WORD", "LEVEL88_NUMBER_WORD", "LIMIT",
  "LIMITS", "LINAGE", "\"LINAGE-COUNTER\"", "LINE", "LINES", "LINKAGE",
  "\"Literal\"", "LOCALE", "\"FUNCTION LOCALE\"", "\"LOCAL-STORAGE\"",
  "LOCK", "\"FUNCTION LOWER-CASE\"", "LOWLIGHT", "\"LOW-VALUE\"", "MANUAL",
  "MEMORY", "MERGE", "MINUS", "\"MNEMONIC NAME\"", "MODE", "MOVE",
  "MULTIPLE", "MULTIPLY", "NATIONAL", "\"NATIONAL-EDITED\"", "NATIVE",
  "NE", "NEGATIVE", "NEXT", "\"NEXT SENTENCE\"", "NO", "NOMINAL", "NOT",
  "\"NOT END\"", "\"NOT EOP\"", "\"NOT EXCEPTION\"", "\"NOT INVALID KEY\"",
  "\"NOT OVERFLOW\"", "\"NOT SIZE ERROR\"", "\"NO ADVANCING\"", "NUMBER",
  "NUMBERS", "NUMERIC", "\"NUMERIC-EDITED\"", "\"FUNCTION NUMVALC\"",
  "\"OBJECT-COMPUTER\"", "OCCURS", "OF", "OFF", "OMITTED", "ON", "ONLY",
  "OPEN", "OPTIONAL", "OR", "ORDER", "ORGANIZATION", "OTHER", "OUTPUT",
  "OVERFLOW", "OVERLINE", "\"PACKED-DECIMAL\"", "PADDING", "PAGE",
  "\"PAGE FOOTING\"", "\"PAGE HEADING\"", "PARAGRAPH", "PERFORM",
  "PICTURE", "PLUS", "POINTER", "POSITION", "POSITIVE", "PRESENT",
  "PREVIOUS", "PRINTER", "PRINTING", "PROCEDURE", "PROCEDURES", "PROCEED",
  "PROGRAM", "\"PROGRAM-ID\"", "\"Program name\"", "\"PROGRAM-POINTER\"",
  "PROMPT", "QUOTE", "RANDOM", "RD", "READ", "RECORD", "RECORDING",
  "RECORDS", "RECURSIVE", "REDEFINES", "REEL", "REFERENCE", "RELATIVE",
  "RELEASE", "REMAINDER", "REMOVAL", "RENAMES", "REPLACING", "REPORT",
  "REPORTING", "REPORTS", "\"REPORT FOOTING\"", "\"REPORT HEADING\"",
  "REPOSITORY", "REQUIRED", "RESERVE", "RETURN", "RETURNING",
  "\"FUNCTION REVERSE\"", "\"REVERSE-VIDEO\"", "REWIND", "REWRITE",
  "RIGHT", "ROLLBACK", "ROUNDED", "RUN", "SAME", "SCREEN",
  "\"SCREEN-CONTROL\"", "SCROLL", "SD", "SEARCH", "SECTION", "SECURE",
  "\"SEGMENT-LIMIT\"", "SELECT", "\"semi-colon\"", "SENTENCE", "SEPARATE",
  "SEQUENCE", "SEQUENTIAL", "SET", "SHARING", "SIGN", "SIGNED",
  "\"SIGNED-INT\"", "\"SIGNED-LONG\"", "\"SIGNED-SHORT\"", "SIZE",
  "\"SIZE ERROR\"", "SORT", "\"SORT-MERGE\"", "SOURCE",
  "\"SOURCE-COMPUTER\"", "SPACE", "\"SPECIAL-NAMES\"", "STANDARD",
  "\"STANDARD-1\"", "\"STANDARD-2\"", "START", "STATUS", "STOP", "STRING",
  "\"FUNCTION SUBSTITUTE\"", "\"FUNCTION SUBSTITUTE-CASE\"", "SUBTRACT",
  "SUM", "SUPPRESS", "SYMBOLIC", "SYNCHRONIZED", "TALLYING", "TAPE",
  "TERMINATE", "TEST", "THAN", "THEN", "THRU", "TIME", "TIMES", "TO",
  "\"FALSE\"", "\"FILE\"", "\"INITIAL\"", "\"NULL\"", "\"TRUE\"", "TOP",
  "TRACKS", "TRAILING", "TRANSFORM", "\"FUNCTION TRIM\"", "TYPE",
  "UNDERLINE", "UNIT", "UNLOCK", "UNSIGNED", "\"UNSIGNED-INT\"",
  "\"UNSIGNED-LONG\"", "\"UNSIGNED-SHORT\"", "UNSTRING", "UNTIL", "UP",
  "UPDATE", "UPON", "\"UPON ARGUMENT-NUMBER\"", "\"UPON COMMAND-LINE\"",
  "\"UPON ENVIRONMENT-NAME\"", "\"UPON ENVIRONMENT-VALUE\"",
  "\"FUNCTION UPPER-CASE\"", "USAGE", "USE", "USING", "VALUE", "VARYING",
  "WAIT", "WHEN", "\"FUNCTION WHEN-COMPILED\"", "\"WHEN OTHER\"", "WITH",
  "\"Identifier\"", "WORDS", "\"WORKING-STORAGE\"", "WRITE", "YYYYDDD",
  "YYYYMMDD", "ZERO", "\"EXEC SQL statement\"", "\"EXEC JAVA statement\"",
  "'+'", "'-'", "'*'", "'/'", "UNARY_SIGN", "'^'", "'.'", "'='", "')'",
  "'('", "'>'", "'<'", "':'", "'&'", "$accept", "start", "$@1",
  "nested_list", "source_element", "program_definition", "$@2", "$@3",
  "program_mandatory", "$@4", "$@5", "function_definition", "$@6", "$@7",
  "nested_prog", "end_program", "end_mandatory", "end_function",
  "identification_division", "$@8", "function_division", "program_name",
  "as_literal", "program_type", "program_type_clause", "_init_or_recurs",
  "environment_division", "configuration_section", "configuration_list",
  "configuration_paragraph", "source_computer_paragraph",
  "source_computer_entry", "with_debugging_mode", "computer_name",
  "object_computer_paragraph", "object_computer_entry",
  "object_clauses_list", "object_clauses", "object_computer_memory",
  "object_char_or_word", "object_computer_sequence",
  "object_computer_segment", "repository_paragraph", "opt_repository",
  "repository_list", "repository_name", "repository_literal_list",
  "special_names_paragraph", "opt_special_names", "special_name_list",
  "special_name", "mnemonic_name_clause", "$@9", "$@10",
  "special_name_mnemonic_on_off_list",
  "special_name_mnemonic_on_off_list_mandatory",
  "special_name_mnemonic_on_off", "on_or_off", "alphabet_name_clause",
  "$@11", "alphabet_definition", "alphabet_literal_list",
  "alphabet_literal", "@12", "alphabet_also_sequence", "alphabet_lits",
  "alphabet_also_literal", "symbolic_characters_clause",
  "symbolic_characters_list", "char_list", "integer_list",
  "class_name_clause", "class_item_list", "class_item", "locale_clause",
  "currency_sign_clause", "decimal_point_clause", "cursor_clause",
  "crt_status_clause", "screen_control", "event_status",
  "input_output_section", "$@13", "$@14", "file_control_paragraph",
  "file_control_sequence", "file_control_entry", "$@15",
  "select_clause_sequence", "select_clause", "assign_clause", "_device",
  "_ext_clause", "assignment_name", "assignment_device_name_list",
  "access_mode_clause", "access_mode", "alternative_record_key_clause",
  "split_key_list", "$@16", "split_key", "key_is_eq",
  "collating_sequence_clause", "file_status_clause", "file_or_sort",
  "lock_mode_clause", "lock_mode", "lock_with", "lock_records",
  "organization_clause", "organization", "padding_character_clause",
  "record_delimiter_clause", "record_key_clause", "relative_key_clause",
  "reserve_clause", "sharing_clause", "sharing_option",
  "nominal_key_clause", "i_o_control_paragraph", "opt_i_o_control",
  "i_o_control_list", "i_o_control_clause", "same_clause", "same_option",
  "multiple_file_tape_clause", "multiple_file_list", "multiple_file",
  "multiple_file_position", "apply_clause_list", "apply_clause",
  "data_division", "file_section", "$@17", "$@18",
  "file_description_sequence", "file_description",
  "file_description_sequence_without_type", "file_type",
  "file_description_entry", "@19", "file_description_clause_sequence",
  "file_description_clause", "block_contains_clause",
  "_records_or_characters", "record_clause", "record_depending",
  "opt_from_integer", "opt_to_integer", "label_records_clause",
  "label_option", "value_of_clause", "valueof_name", "data_records_clause",
  "linage_clause", "linage_sequence", "linage_lines", "linage_footing",
  "linage_top", "linage_bottom", "recording_mode_clause",
  "code_set_clause", "report_clause", "working_storage_section", "$@20",
  "record_description_list", "record_description_list_1", "$@21",
  "record_description_list_2", "exec_sql_data_statement",
  "data_description", "$@22", "$@23", "level_number", "level_number_88",
  "_maybe_next_level_number", "entry_name", "const_name", "const_global",
  "lit_or_length", "constant_entry", "data_description_clause_sequence",
  "data_description_clause", "redefines_clause", "external_clause",
  "as_extname", "global_clause", "picture_clause", "usage_clause", "usage",
  "sign_clause", "occurs_key_spec", "occurs_clause", "occurs_to_integer",
  "occurs_depending", "_occurs_keys", "occurs_keys", "occurs_key",
  "occurs_key_list", "ascending_or_descending", "_occurs_indexed",
  "occurs_indexed", "occurs_index_list", "occurs_index",
  "justified_clause", "synchronized_clause", "left_or_right",
  "blank_clause", "based_clause", "value_clause", "value_cond_clause",
  "$@24", "value_item_list", "value_item", "false_is", "renames_clause",
  "any_length_clause", "varying_clause", "local_storage_section", "$@25",
  "linkage_section", "$@26", "report_section", "$@27",
  "opt_report_description_list", "report_description_list",
  "report_description_entry", "report_description_options",
  "report_description_option", "control_clause", "control_field_list",
  "_final", "identifier_list", "page_limit_clause", "heading_clause",
  "first_detail", "last_heading", "last_detail", "footing_clause",
  "page_line_column", "line_or_lines", "report_group_description_list",
  "report_group_description_entry", "report_group_options",
  "report_group_option", "type_clause", "type_option", "next_group_clause",
  "column_clause", "sum_clause_list", "sum_clause", "ref_id_exp",
  "present_when_condition", "line_clause", "line_keyword_clause",
  "report_line_integer_list", "line_or_plus", "_numbers", "source_clause",
  "group_indicate_clause", "_indicate", "report_name", "screen_section",
  "$@28", "procedure_division", "$@29", "$@30", "procedure_using_chaining",
  "$@31", "$@32", "procedure_param_list", "procedure_param",
  "procedure_type", "size_optional", "procedure_optional",
  "procedure_returning", "procedure_declaratives", "$@33",
  "procedure_list", "procedure", "section_header", "paragraph_header",
  "invalid_statement", "section_name", "opt_segment", "statement_list",
  "@34", "@35", "statements", "statement", "accept_statement", "$@36",
  "accept_body", "opt_at_line_column", "line_number", "column_number",
  "opt_accp_attr", "accp_attrs", "accp_attr", "end_accept",
  "add_statement", "$@37", "add_body", "add_to", "end_add",
  "allocate_statement", "$@38", "allocate_body", "allocate_returning",
  "alter_statement", "alter_options", "_proceed_to", "call_statement",
  "$@39", "call_using", "$@40", "call_param_list", "call_param",
  "call_type", "call_returning", "call_on_exception", "$@41",
  "call_not_on_exception", "$@42", "end_call", "cancel_statement", "$@43",
  "cancel_list", "close_statement", "$@44", "close_list", "close_option",
  "reel_or_unit", "compute_statement", "$@45", "compute_body",
  "end_compute", "comp_equal", "commit_statement", "continue_statement",
  "delete_statement", "$@46", "end_delete", "delete_file_statement",
  "$@47", "display_statement", "$@48", "display_body", "with_clause",
  "disp_attrs", "disp_attr", "end_display", "divide_statement", "$@49",
  "divide_body", "end_divide", "entry_statement", "$@50",
  "evaluate_statement", "$@51", "evaluate_subject_list",
  "evaluate_subject", "evaluate_condition_list", "evaluate_case_list",
  "evaluate_case", "$@52", "evaluate_other", "$@53", "evaluate_when_list",
  "evaluate_object_list", "evaluate_object", "opt_evaluate_thru_expr",
  "end_evaluate", "exit_statement", "$@54", "exit_body", "free_statement",
  "$@55", "generate_statement", "$@56", "goto_statement", "$@57",
  "goto_depending", "goback_statement", "$@58", "if_statement", "$@59",
  "$@60", "if_else_sentence", "$@61", "end_if", "initialize_statement",
  "$@62", "initialize_filler", "initialize_value", "initialize_replacing",
  "initialize_replacing_list", "initialize_replacing_item",
  "initialize_category", "initialize_default", "initiate_statement",
  "$@63", "inspect_statement", "$@64", "send_identifier", "inspect_list",
  "inspect_item", "inspect_tallying", "$@65", "tallying_list",
  "tallying_item", "inspect_replacing", "replacing_list", "replacing_item",
  "rep_keyword", "replacing_region", "inspect_converting",
  "inspect_region", "_initial", "merge_statement", "$@66",
  "move_statement", "$@67", "move_body", "multiply_statement", "$@68",
  "multiply_body", "end_multiply", "open_statement", "$@69", "open_list",
  "open_mode", "open_sharing", "open_option", "perform_statement", "$@70",
  "perform_body", "$@71", "end_perform", "perform_procedure",
  "perform_option", "perform_test", "perform_varying_list",
  "perform_varying", "read_statement", "$@72", "read_into", "with_lock",
  "read_key", "read_handler", "end_read", "release_statement", "$@73",
  "return_statement", "$@74", "end_return", "rewrite_statement", "$@75",
  "write_lock", "end_rewrite", "rollback_statement", "search_statement",
  "$@76", "search_body", "$@77", "search_varying", "search_at_end", "$@78",
  "search_whens", "search_when", "$@79", "end_search", "set_statement",
  "$@80", "set_body", "set_environment", "set_to", "set_up_down",
  "up_or_down", "set_to_on_off_sequence", "set_to_on_off",
  "set_to_true_false_sequence", "set_to_true_false", "sort_statement",
  "$@81", "sort_body", "$@82", "sort_key_list", "opt_key_list",
  "sort_duplicates", "sort_collating", "sort_input", "sort_output",
  "start_statement", "$@83", "@84", "start_key", "start_op", "end_start",
  "stop_statement", "$@85", "$@86", "stop_returning", "string_statement",
  "$@87", "string_item_list", "string_item", "opt_with_pointer",
  "end_string", "subtract_statement", "$@88", "subtract_body",
  "end_subtract", "suppress_statement", "_printing", "terminate_statement",
  "$@89", "transform_statement", "$@90", "unlock_statement", "$@91",
  "opt_record", "unstring_statement", "$@92", "unstring_delimited",
  "unstring_delimited_list", "unstring_delimited_item", "unstring_into",
  "unstring_into_item", "unstring_into_delimiter", "unstring_into_count",
  "unstring_tallying", "end_unstring", "use_statement", "use_exception",
  "use_global", "use_exception_target", "_after", "_standard",
  "exception_or_error", "exception_or_overflow",
  "not_exception_or_overflow", "_procedure", "use_debugging",
  "use_debugging_target", "use_reporting", "write_statement", "$@93",
  "write_from", "write_option", "before_or_after", "write_handler",
  "end_write", "on_accp_exception", "on_disp_exception",
  "opt_on_exception", "$@94", "opt_not_on_exception", "$@95",
  "on_size_error", "opt_on_size_error", "$@96", "opt_not_on_size_error",
  "$@97", "on_overflow", "opt_on_overflow", "$@98", "opt_not_on_overflow",
  "$@99", "at_end", "at_end_sentence", "$@100", "not_at_end_sentence",
  "$@101", "at_eop", "at_eop_sentence", "$@102", "not_at_eop_sentence",
  "$@103", "opt_invalid_key", "invalid_key", "invalid_key_sentence",
  "$@104", "not_invalid_key_sentence", "$@105", "_opt_scroll_lines",
  "condition", "expr", "partial_expr", "$@106", "expr_tokens",
  "expr_token", "eq", "gt", "lt", "ge", "le", "exp_list", "e_sep", "exp",
  "linage_counter", "arithmetic_x_list", "arithmetic_x", "record_name",
  "table_name", "file_name_list", "file_name", "mnemonic_name_list",
  "mnemonic_name", "procedure_name_list", "procedure_name", "label",
  "integer_label", "reference_list", "reference", "no_reference_list",
  "opt_reference", "reference_or_literal", "undefined_word",
  "target_x_list", "target_x", "x_list", "x", "arith_x", "prog_or_entry",
  "alnum_or_id", "simple_value", "simple_all_value", "id_or_lit",
  "id_or_lit_or_func", "num_id_or_lit", "identifier", "identifier_1",
  "qualified_word", "subref", "refmod", "integer", "literal",
  "basic_literal", "basic_value", "function", "func_refmod", "func_args",
  "list_func_args", "trim_args", "numvalc_args", "locale_dt_args",
  "not_const_word", "flag_all", "flag_duplicates", "flag_initialized",
  "flag_next", "flag_not", "flag_optional", "flag_rounded",
  "flag_separate", "in_of", "records", "with_dups", "coll_sequence",
  "_advancing", "_also", "_are", "_area", "_as", "_at", "_binary", "_by",
  "_character", "_characters", "_contains", "_data", "_file", "_for",
  "_from", "_in", "_is", "_is_are", "_key", "_line_or_lines", "_lines",
  "_literal", "_mode", "_number", "_of", "_on", "_in_order", "_other",
  "_program", "_record", "_right", "_set", "_sign", "_sign_is", "_size",
  "_status", "_tape", "_than", "_then", "_times", "_to", "_when", "_with",
  "exec_sql_statement", "exec_java_statement", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1747)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1554)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1747,   192,   409, -1747,  -231,  -203,   110, -1747, -1747, -1747,
     450,   450,   503,   503, -1747, -1747,   509, -1747, -1747, -1747,
   -1747,   689,   689,   288,   709,   709,   633,   441, -1747,   820,
     913, -1747, -1747, -1747, -1747,   -80,   726,   922,   588,   794,
     794, -1747,   651,   224,   695,   713,   806,   729, -1747,    39,
    1087,   844,  1064, -1747,   -89, -1747, -1747,   910, -1747, -1747,
   -1747,   759, -1747, -1747, -1747,   881,   808, -1747,    29, -1747,
     485,   450,   503, -1747, -1747, -1747, -1747,   513, -1747,  1093,
     305,   841,   909,  1048,   833, -1747, -1747,   964,   503, -1747,
   -1747, -1747,   866,   873,   883,   890,   892, -1747, -1747, -1747,
   -1747, -1747,   947,   895,  1134,  1242,   949,   699, -1747,   332,
   -1747, -1747, -1747,    32, -1747, -1747,   898,  1002,  1126, -1747,
     382,   -44, -1747,    45,    45,   923,   912,   914,   709, -1747,
      11,  1184,    98,  1072,  1095, -1747, -1747,   927, -1747,  1088,
    1106,   990,  1103,   992, -1747,   997, -1747, -1747, -1747,  1363,
   -1747, -1747, -1747, -1747, -1747, -1747,   939,  1042,  1067, -1747,
     833, -1747, -1747,   953, -1747, -1747,    42, -1747,  -155,  -156,
      94, -1747, -1747, -1747, -1747,  1033,  1193, -1747,   394, -1747,
     414, -1747, -1747, -1747, -1747,    37,   206, -1747,   -79, -1747,
   -1747, -1747,   951,   682,  1303,   970,  1193,  1193,   970,  1029,
    1051,  1193,  1193,  1193,  1193,  1193,   970,  1193,  1358,  1193,
   -1747,  1320, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747,   970,   947,   305,   979, -1747,
     979,   979, -1747,  1186,   979, -1747,  1338, -1747,  1252,   382,
     -44, -1747,   972,  1075,  1081,   -44,   978,   404, -1747,   900,
     944, -1747,  1193,  1065,  1155, -1747, -1747,  1332,   794,  1193,
    1203, -1747,   642, -1747, -1747,  1080, -1747,  1193,  1231, -1747,
     431, -1747, -1747, -1747, -1747,   991,  1197, -1747, -1747,   970,
     970,  1193,  1193, -1747,  1193,   979,  1379,   970,   970,   979,
    1193,   979, -1747,   970,     5, -1747, -1747, -1747, -1747,   737,
     979, -1747, -1747,   979,  1170,  1055,  1175, -1747,   833, -1747,
     833, -1747, -1747,   -44, -1747,  1005,  1111, -1747, -1747, -1747,
   -1747, -1747,   900, -1747, -1747, -1747,   -41,   -38, -1747, -1747,
    1338,  1193,   906,   906,  1193,    81,  1219,  1193,  1451,  1198,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747,   976,   571,  1193, -1747,  1034,  1021, -1747,   844,  1203,
   -1747, -1747, -1747, -1747,   979, -1747, -1747, -1747, -1747, -1747,
    1193, -1747, -1747,  1047,   979,  1243, -1747, -1747, -1747, -1747,
   -1747,   979, -1747, -1747,    91, -1747, -1747,   955, -1747, -1747,
   -1747, -1747,   979, -1747,   979,  1204,   979,   833, -1747,  1187,
     833, -1747, -1747,   -44, -1747,  1028, -1747, -1747,  1403, -1747,
    1412, -1747, -1747,  1203,  1056,  1193,  1451,   979,   -92,   -97,
    1203,  1061, -1747,  1193,  1059, -1747,  1059,   -52, -1747, -1747,
   -1747, -1747, -1747,  1203, -1747, -1747, -1747,   572,    16, -1747,
    1052, -1747, -1747, -1747, -1747, -1747, -1747, -1747,  1047, -1747,
    1104, -1747, -1747, -1747, -1747, -1747, -1747,  1203, -1747, -1747,
     955, -1747,  1119, -1747,   958, -1747,   979,   979,   979, -1747,
    1203, -1747, -1747, -1747,  1189, -1747, -1747,    44,  1073,  1107,
   -1747, -1747, -1747,   979, -1747, -1747, -1747, -1747, -1747, -1747,
    1273,    15,  1311,  1071, -1747, -1747, -1747,  1193,  1193, -1747,
   -1747,  2671,   503, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,   698, -1747,
      61, -1747,  1047,  1203, -1747, -1747, -1747,  1193,   955, -1747,
    1219,  1196,  1113, -1747,  1153,  1219,  1300,  1193,  1466,   164,
     -43,   849, -1747,  1083, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747,  1143, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747,  1193,   979, -1747,  1059, -1747,  1189,
   -1747, -1747,  2343,  1501,  1357,   115, -1747,  1203,   118, -1747,
   -1747, -1747,  1203, -1747, -1747,  1176, -1747,    54,    54,  2813,
   -1747,  1092, -1747, -1747, -1747, -1747,  1195,  3479,  1096, -1747,
   -1747,   698, -1747, -1747,   970, -1747,  1193,  1300, -1747,   933,
   -1747,  1193, -1747,  1193,   902, -1747,  1193, -1747,  1193,  1188,
    1193,  1193, -1747,  1363,   278,  1193,  1114, -1747, -1747, -1747,
    1334, -1747, -1747,  -159,   597,   626,   652,   727,  1122, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,  1224, -1747,
   -1747,  1203, -1747, -1747, -1747, -1747,   979,   979,  1351, -1747,
   -1747, -1747,   -76, -1747, -1747, -1747,  1193,  1193,   979, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747,   989,   -71, -1747,
     645, -1747,   677, -1747, -1747, -1747, -1747,    40,  1358, -1747,
     767, -1747, -1747, -1747, -1747,  1462, -1747,  1337, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,  1169, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747,  1113, -1747,  1868, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747,   -63, -1747, -1747,
    1274, -1747, -1747, -1747, -1747,   823, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747,  1074, -1747, -1747,   301,
    1193, -1747, -1747,   122,   103,   979,  1545, -1747, -1747,   -97,
    1206, -1747,   979,   979, -1747,  1298,  1298,  1308, -1747,   979,
   -1747,    58, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747,  1144, -1747, -1747,  1191, -1747,  1131,
    1199, -1747, -1747, -1747, -1747,  4987,   677,  1413, -1747,  1571,
   -1747,  1239,  1239,  1047,  1103,  1103, -1747, -1747,  1138, -1747,
     677, -1747,  1205, -1747, -1747, -1747, -1747, -1747,    64,  1430,
   -1747, -1747,  1107,  1203,  1149, -1747,  1150,   979,  3960,  1166,
     112,  2099,  1602, -1747,  4503,   833,  1208,  4559,  4503,  1378,
    -147,   893,    59,   979, -1747, -1747,  1479, -1747,    59,   979,
    4644,   979,  4041,  4503, -1747,  2792,   833,   979,   833,   979,
      53,    80,   979,   833, -1747, -1747,  4160,  4241, -1747, -1747,
     979,   979,   833,   979, -1747,   208,  1510,   979, -1747, -1747,
   -1747, -1747, -1747, -1747,  1603, -1747, -1747, -1747, -1747, -1747,
     979,    90,   117,   666,  1180, -1747,  1180, -1747, -1747, -1747,
   -1747,   350, -1747, -1747, -1747, -1747, -1747,   979,  1193,  1450,
    1450,   115, -1747, -1747, -1747, -1747,  1436, -1747,  1203,  1226,
    5006,  1165, -1747,   979, -1747, -1747,  4503,  1390, -1747,  1466,
   -1747, -1747, -1747,   979,   979,  1047,  1122, -1747,   677,   -97,
     -97,  1601,  1358, -1747, -1747, -1747,  1506,   739,  1103,  1177,
     979,  1178,  1181,  1103,   737,  1194,  1200,  1207,  1211,  1213,
    1221,  1228,  1229,  1178,  1508, -1747,  2507, -1747, -1747, -1747,
   -1747,  1435, -1747,  1588, -1747, -1747, -1747,  1244, -1747,   737,
   -1747, -1747,  1212, -1747, -1747, -1747,   119,   833,  1519,  2059,
   -1747,  1302,  1336,   833,   827,  1525,  3769,   852,   899,  1526,
     101,  1212, -1747, -1747,    47, -1747, -1747, -1747,  1563, -1747,
   -1747, -1747,  1103,    59, -1747, -1747, -1747, -1747, -1747,  1265,
   -1747,    84,   979, -1747,   233, -1747, -1747, -1747, -1747, -1747,
    4503, -1747,  1263,  1531,  1635,   815, -1747,  1284, -1747,  3529,
    1552,   738,  1301,  1306,  -179,  1307,   -39,  1520, -1747,  1336,
    1520,   979,  1559,  1267, -1747,   911, -1747, -1747, -1747, -1747,
   -1747,  1459, -1747,    59, -1747,  -107, -1747,    55, -1747, -1747,
   -1747,   511,  1660,  2162, -1747, -1747,   979,  1565,  4281,   979,
    1533,   924,  1607, -1747,  1382,  1342,  1106,  1520,  1074,   350,
   -1747,  1282, -1747,   979,   366, -1747, -1747, -1747,  1193,  1605,
   -1747, -1747, -1747, -1747, -1747, -1747,   119, -1747, -1747,   979,
   -1747,  1203,  1498, -1747, -1747, -1747,  1615,  1103,  5006,  5006,
    5006,    46,  1007, -1747, -1747, -1747,  1138, -1747,  5006, -1747,
   -1747,  1682, -1747, -1747, -1747, -1747, -1747,  1367, -1747, -1747,
    1273,   -97,  1624, -1747, -1747,   911,   984,  1299,   319,   -32,
    5006,  1340,  5006, -1747,  5006, -1747,  5068,  1310,  5006,  5006,
    5006,  5006,  5006,  5006,  5006,  5006, -1747, -1747, -1747,  4503,
    1554, -1747, -1747,  1409,  1435,  3425,  3161,  1444,  1522, -1747,
     545, -1747, -1747, -1747,   622, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747,   799,   833, -1747, -1747,   437,  1581,
    1581,  1581,  1581, -1747, -1747,  4503,  4503, -1747, -1747,    48,
    1621,   830, -1747,  1318,  -147, -1747,   979, -1747,    28, -1747,
   -1747,   582,  1589, -1747,   911,    71, -1747,   233, -1747, -1747,
   -1747, -1747,    96,  1361,    59, -1747, -1747,  4503, -1747, -1747,
   -1747, -1747,  1395, -1747, -1747, -1747, -1747,   979,   112, -1747,
     860, -1747, -1747,  1336,   119, -1747,  1555,   494,   693, -1747,
   -1747,   979,   693,  1365, -1747,  1138, -1747, -1747,    52,   955,
   -1747, -1747,  3857, -1747,  1717,  1553,  4503,  4503, -1747,  4362,
     979, -1747,  1590, -1747, -1747,  4503,   911, -1747, -1747, -1747,
    1660,  1562,   979, -1747,   845,    78,   494, -1747, -1747, -1747,
     979, -1747,  1493, -1747, -1747, -1747,   107,   979, -1747,   979,
    1582,   977,   -38, -1747,  1106,   529,  5068,  1329,  1329,  1110,
   -1747, -1747, -1747,  5006,  5006,  5006,  5006,  5006,  5006,  1485,
    1007,  4503, -1747,  1113, -1747,  1273,  1106, -1747, -1747, -1747,
    1581, -1747, -1747,  1327,  1344, -1747,   911,  1581,  1558, -1747,
   -1747, -1747, -1747,  1305,  1581,  1518,  1518,  1518,    74,  1564,
   -1747, -1747,   390, -1747,    88,  1085,   979,  1079,   109,  1333,
   -1747,  1138, -1747, -1747,   573,  1343,  1125,   583,  1346,  1223,
     126,   128,   608,  1347,  1283,  4481,   -50,  4503,    59, -1747,
    1454, -1747, -1747, -1747, -1747, -1747,  1113, -1747, -1747,  1393,
   -1747, -1747,  1393, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747,  1396,   112, -1747,    95,
     979,   979,   423, -1747, -1747, -1747,   380,   745,  1423, -1747,
   -1747,  1672, -1747,  1541, -1747,    89,  1215,  1581,  1539, -1747,
   -1747,  1546, -1747, -1747, -1747,  1623,  4481,   498, -1747, -1747,
   -1747,  3065, -1747,  1414, -1747, -1747, -1747, -1747, -1747,    48,
   -1747, -1747, -1747,  1106, -1747, -1747, -1747,  1113, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747,  1484,  1113, -1747,  1415, -1747,
    1767, -1747, -1747, -1747,    76, -1747,   911,   280, -1747,    37,
     704,   535,    59,    59,  4481,   563,  1083,   833,  1679, -1747,
   -1747,  1812, -1747,  1641, -1747, -1747, -1747, -1747,  1555, -1747,
     979,   154,   799,   757,  1385,  1698, -1747,  1394,   911,   532,
   -1747,   390, -1747, -1747, -1747,  4503,  1193,   799, -1747, -1747,
   -1747, -1747,   352,   979,  4481,   615,  1425,  1820,   979,   473,
   -1747, -1747, -1747,  1524,  1527, -1747, -1747,   860,   107, -1747,
     371, -1747, -1747, -1747, -1747,  1193,  1661, -1747, -1747,  1203,
   -1747,   979, -1747, -1747,  1660, -1747,  1634, -1747,   535,  1300,
     535, -1747,  1138, -1747, -1747,  1085,   818,   818,  1329,  1329,
    1329, -1747,  1295, -1747,  1433, -1747,   979, -1747,  1546, -1747,
   -1747,  1581, -1747, -1747, -1747,  1193, -1747, -1747,  1193, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747,     1, -1747, -1747, -1747,
    1305, -1747, -1747, -1747,   119,   119,   119, -1747, -1747, -1747,
   -1747, -1747,  1178,  1302,  4901, -1747,   979,  1178,  1178,  5006,
   -1747,  1178,  1178,  1178,    -3,  1178,  1178, -1747, -1747,  1570,
    4481, -1747,    59, -1747, -1747,    24,   610, -1747, -1747,  1391,
   -1747,   566,    -5, -1747, -1747, -1747, -1747,  1036, -1747,  1511,
   -1747,  1497, -1747, -1747, -1747, -1747, -1747, -1747,   -83,   -83,
     -83,   -83,  1193, -1747, -1747, -1747, -1747,  1158,  1193, -1747,
   -1747, -1747, -1747,     2, -1747,  1215, -1747, -1747, -1747, -1747,
   -1747, -1747,  4503, -1747,  4503,    48, -1747, -1747, -1747,  3065,
   -1747,   979,  1718,  1410,   941,  1734,  1411,   136,   911, -1747,
   -1747,  1798, -1747, -1747, -1747, -1747,   280, -1747,  1675, -1747,
    1193,  1567, -1747, -1747,  1300,    59, -1747,  4503,   278,   465,
   -1747, -1747, -1747,   979,  4503,   640, -1747, -1747, -1747,  1712,
    1591, -1747,  1713, -1747,  1626, -1747, -1747, -1747, -1747,  1394,
   -1747, -1747, -1747,  1606,  1720,  1578,  1575,  1302, -1747,  4503,
     136, -1747,  1586, -1747,   911, -1747,  1758,  1489, -1747, -1747,
    1106, -1747,   777,  1877, -1747,  1018, -1747, -1747, -1747,  1203,
    1770,  1667,  1821,  4915, -1747,  1447, -1747, -1747, -1747,  1193,
   -1747, -1747, -1747,  1193, -1747, -1747, -1747, -1747,   368,   368,
      79,    79, -1747, -1747, -1747, -1747, -1747,  1423, -1747,  1330,
   -1747, -1747, -1747,  1085, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747,  1113,  1735,  1113,  1737,
   -1747, -1747,  4503, -1747, -1747, -1747, -1747, -1747,  1761, -1747,
   -1747, -1747, -1747, -1747, -1747,  1581,  1581,  1581,  1581,   368,
   -1747, -1747,   368,    79,    79, -1747, -1747, -1747,  4481,  1566,
    4481,  1568, -1747, -1747, -1747, -1747, -1747,  1759, -1747,   941,
   -1747,  1795, -1747, -1747, -1747, -1747, -1747, -1747, -1747,   136,
     860, -1747, -1747,   860,   131,   979, -1747,  1193,  4481, -1747,
   -1747,   832,  3741, -1747,  1851,  1662,  1683,   162, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747,   979,  1082, -1747, -1747, -1747,  1760,  1636,   979,  1423,
    4481, -1747,  1820, -1747,  1311,  1809,  1311,  1578,   442, -1747,
   -1747,  1762, -1747,  1642, -1747, -1747, -1747,   534, -1747, -1747,
    1193,  1816,  1687, -1747,  1045, -1747,  1706,  1049,  1451,  1719,
    1474,  1193,  1103,  1193, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747,  1523, -1747, -1747,
   -1747,   854, -1747, -1747, -1747,  1447, -1747,   979,   677, -1747,
   -1747, -1747, -1747, -1747, -1747,   368, -1747, -1747, -1747, -1747,
   -1747, -1747,  1113, -1747,  1113, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
    4503, -1747,  4503, -1747, -1747, -1747, -1747, -1747,  1865,   860,
     860, -1747,  1509,  1610,   833,     4, -1747, -1747, -1747, -1747,
    1574,  4503, -1747,  1193,   765,  1680, -1747,  1685, -1747, -1747,
   -1747, -1747, -1747, -1747,   979, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,   979,
    1311, -1747,   979,  1772, -1747, -1747, -1747, -1747, -1747,   833,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747,  1100,  1203,  1193,
    1193,  1748, -1747,  1193, -1747, -1747, -1747, -1747,   115, -1747,
    1193, -1747,   979,   979,   785, -1747,  1630,  1203,   854, -1747,
   -1747, -1747,   979, -1747,  1100, -1747, -1747, -1747, -1747,  1423,
    1423, -1747,  4503,   860, -1747,  4503,  1193,   833,   833,  1622,
   -1747,   979, -1747,  1505,   979,  1790, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747,   979, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747,  1203,  1203,  1193, -1747,  1203,
   -1747,  1203, -1747,  1302, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747,   112,   833,  1193, -1747, -1747,   979, -1747, -1747,
   -1747, -1747, -1747, -1747,  1203, -1747, -1747, -1747, -1747,   112,
   -1747, -1747, -1747
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     0,     1,     0,     0,     0,     4,     6,     7,
      38,    38,     0,     0,     3,     5,     0,     8,    14,    28,
      27,    29,    29,     0,   276,   276,     0,     0,    24,    40,
       0,     9,    15,    30,    26,  1497,     0,   157,     0,   579,
     579,  1498,     0,     0,     0,     0,     0,     0,    39,   278,
       0,    17,     0,    25,    33,    37,    36,  1524,    35,    42,
     159,     0,   161,   288,   289,     0,   340,   281,   583,    18,
      20,    38,     0,    16,    34,  1525,    32,    41,   165,   163,
     249,     0,     0,   478,     0,   586,   584,   601,     0,    19,
      10,    11,     0,     0,     0,     0,     0,    43,    44,    45,
      47,    46,   160,     0,   247,     0,  1489,   258,   162,   251,
     253,   255,   256,   252,   269,   279,     0,     0,   481,  1332,
     282,   343,   290,   591,   591,     0,     0,     0,   276,    23,
      56,    71,    49,    80,  1451,   166,   165,     0,   158,     0,
    1517,     0,  1515,     0,  1490,  1541,   259,   260,   261,  1471,
     250,   254,   268,   270,   283,   341,     0,     0,   484,   287,
       0,   286,   344,  1439,   292,  1480,   591,   588,   594,     0,
     591,   602,   580,    21,    12,     0,  1497,    54,  1524,    55,
    1524,    60,    62,    63,    64,     0,     0,    70,     0,    73,
    1554,    48,     0,  1553,     0,     0,  1497,  1497,     0,     0,
    1532,  1497,  1497,  1497,  1497,  1497,     0,  1497,  1483,  1497,
      79,    81,    83,    85,    86,    87,    89,    88,    90,    91,
      92,    93,    94,    95,  1452,     0,   164,   249,     0,  1518,
       0,     0,  1516,     0,     0,  1542,  1485,  1472,  1491,   280,
     343,   479,     0,     0,   576,   343,   352,   346,   351,     0,
       0,   589,  1497,     0,   599,   592,   593,   603,   579,  1497,
       0,    57,  1524,    59,    61,     0,  1464,  1497,     0,    77,
       0,    72,    74,    52,    50,     0,     0,  1352,   112,     0,
       0,  1497,  1497,  1533,  1497,     0,     0,     0,     0,     0,
    1497,     0,  1484,     0,    99,    82,    84,   167,   248,  1396,
     275,  1343,  1345,   271,     0,     0,     0,  1486,     0,  1492,
       0,   284,   342,   343,   482,     0,     0,   277,   285,   353,
     349,   350,     0,   359,   360,   347,   363,   363,   354,   305,
    1485,  1497,     0,     0,  1497,  1485,  1511,  1497,  1469,     0,
     291,   293,   296,   297,   298,   299,   300,   301,   302,   303,
     304,     0,     0,  1497,   600,     0,     0,   581,    17,     0,
    1401,    69,    58,  1463,     0,    76,    75,    78,    51,    53,
    1497,   101,   102,     0,     0,     0,   153,   152,   103,   104,
     156,     0,   155,   139,  1499,   141,    96,     0,    97,   169,
    1457,  1458,     0,  1344,     0,     0,     0,   262,   263,   266,
     257,  1330,   480,   343,   485,     0,   348,   364,   365,   355,
       0,   365,   357,     0,     0,  1497,  1469,     0,     0,     0,
       0,     0,  1512,  1497,     0,  1470,     0,     0,   294,   295,
     595,   596,   598,     0,   590,   604,   606,     0,     0,    68,
       0,  1410,  1406,  1411,  1409,  1407,  1412,  1408,   145,   146,
     148,   154,   151,   150,  1501,  1500,   142,     0,   111,   110,
     100,   107,  1539,   105,     0,  1397,   273,     0,   274,   264,
       0,   265,  1331,   483,   487,   577,   373,   367,     0,   317,
     337,  1459,  1460,   326,  1346,   321,   320,   319,  1351,  1350,
    1507,  1483,  1495,     0,   575,   338,   339,  1497,  1497,   597,
     606,     0,     0,    13,    66,    67,    65,   117,   131,   127,
     132,   114,   130,   128,   115,   116,   129,   113,   118,   119,
     121,   147,     0,   140,   143,   108,  1540,  1497,    98,   184,
    1511,     0,  1549,   230,     0,  1511,  1502,  1497,  1481,  1502,
     233,     0,   232,  1553,   217,   216,   168,   170,   171,   172,
     173,   174,   175,     0,   176,   177,   229,   178,   179,   180,
     181,   182,   183,   185,  1497,   272,   267,     0,   486,   488,
     489,   578,     0,  1473,     0,  1499,   358,     0,   307,  1347,
    1508,   328,     0,   310,  1496,  1537,   336,     0,     0,     0,
     612,   616,   607,   608,   609,   610,   615,     0,     0,   120,
     123,     0,   149,   144,     0,   106,  1497,  1502,  1550,   192,
     234,  1497,  1503,  1497,     0,  1482,  1497,  1478,  1497,     0,
    1497,  1497,   241,  1471,     0,  1497,     0,   492,   490,   390,
       0,   464,   399,   432,   420,   429,   426,   423,  1551,   400,
     401,   402,   403,   404,   405,   406,   407,   408,  1528,   362,
     433,     0,   409,   396,   410,   411,     0,     0,  1535,   413,
     414,   412,   460,   416,   417,   415,  1497,  1497,   477,   356,
     374,   375,   376,   377,   378,   379,   397,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,     0,     0,  1474,
       0,   368,     0,   318,   309,   308,   306,   327,  1483,  1538,
     315,   324,   323,   325,   322,     0,   614,   617,   676,   727,
     736,   743,   747,   771,   776,   794,   787,   795,   796,   802,
     835,   844,   846,   873,   881,   883,  1549,   889,     0,   900,
     921,   923,   959,   961,   965,   675,   971,   984,  1004,  1021,
    1023,  1027,  1034,  1035,  1051,  1071,  1089,     0,  1108,  1119,
    1127,  1129,  1131,  1133,  1138,  1160,  1183,  1555,  1556,   611,
     623,   624,   625,   626,   627,   628,   629,   630,   632,   631,
     633,   634,   635,   636,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,   669,   670,   671,  1156,
    1157,  1158,   672,   673,   674,    22,     0,   122,   109,     0,
    1497,   194,   193,   189,     0,     0,  1477,   233,   228,     0,
       0,   231,     0,     0,   240,  1522,  1522,     0,   242,     0,
     213,  1497,   476,   430,   431,   418,   419,   427,   428,   424,
     425,   421,   422,  1552,     0,  1529,   458,   440,   391,  1392,
     474,  1536,   461,   462,   459,     0,     0,     0,  1391,   393,
     395,  1455,  1455,     0,  1515,  1515,   372,   369,  1402,  1404,
     466,   468,   470,  1476,   329,   330,   331,   332,     0,     0,
     311,  1494,   317,     0,     0,   618,     0,     0,     0,  1247,
     742,     0,   773,   778,     0,     0,     0,     0,     0,     0,
    1247,   875,     0,     0,   885,   890,     0,  1247,     0,     0,
       0,     0,     0,     0,   973,   994,     0,     0,     0,     0,
       0,     0,     0,     0,  1103,  1101,     0,     0,  1128,  1126,
       0,     0,     0,     0,  1161,  1167,     0,     0,   137,   133,
     138,   136,   134,   135,   124,   125,   202,   203,   201,   200,
       0,   187,   188,  1509,   222,   221,   222,   218,   246,   235,
     236,  1442,   239,  1523,   243,   244,   245,  1348,  1497,   501,
     501,  1499,   521,   493,   496,   497,     0,   463,     0,  1547,
       0,  1393,  1394,     0,   398,   465,     0,     0,   392,  1481,
     434,   435,  1403,     0,     0,     0,  1551,   469,     0,     0,
       0,  1475,  1483,   316,   605,   613,   725,   695,  1515,     0,
       0,  1425,  1428,  1515,  1323,     0,     0,     0,     0,     0,
       0,     0,     0,  1425,   734,  1367,   732,  1357,  1359,  1365,
    1366,  1444,   737,     0,  1246,  1268,  1342,     0,  1338,  1340,
    1339,  1386,   749,  1385,  1387,   775,   772,   777,   790,     0,
    1325,  1453,  1526,     0,  1406,   833,   695,     0,  1359,   842,
       0,   749,   852,   851,  1467,   848,   850,   880,   877,   876,
     879,   874,  1515,   882,  1353,  1355,   884,  1336,   894,  1545,
    1245,   902,   922,   503,     0,   925,   926,   927,   960,  1075,
       0,   962,     0,   969,     0,   972,   995,  1342,   985,   994,
     987,     0,   992,     0,  1339,     0,  1446,  1185,  1328,  1526,
    1185,     0,  1049,  1040,  1329,     0,  1335,  1052,  1053,  1054,
    1055,  1056,  1064,  1057,  1067,     0,  1333,     0,  1072,  1090,
    1104,  1105,  1479,     0,  1110,  1112,     0,  1124,     0,  1130,
       0,  1135,  1140,  1168,     0,  1169,  1517,  1185,     0,  1442,
     196,   195,   186,     0,     0,   220,   219,  1462,  1497,     0,
     212,   206,   237,  1443,  1349,   214,     0,   502,   498,     0,
     499,     0,   491,   494,   441,  1548,   442,  1515,     0,     0,
       0,  1311,  1309,  1374,  1314,  1368,  1372,  1373,     0,  1395,
     475,     0,   394,  1456,   371,   370,  1405,  1530,   471,   335,
    1507,     0,   313,   726,   677,  1476,     0,   704,     0,     0,
       0,     0,     0,  1413,  1430,  1424,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1414,   735,   728,     0,
       0,  1358,  1445,   740,  1444,  1248,     0,   745,     0,   750,
     760,  1384,   774,  1383,   780,   791,   788,   793,   792,  1247,
    1326,  1454,  1327,  1527,  1234,   801,   834,   803,   813,  1200,
    1200,  1200,  1200,   843,   836,     0,     0,   845,  1468,  1247,
     871,   858,   854,   856,  1247,   878,     0,  1354,   887,  1546,
     892,   904,     0,   504,     0,   948,   933,   924,   928,   930,
     931,   932,  1079,     0,     0,   970,   966,     0,   978,   975,
     977,   976,   979,   986,   989,   619,  1247,     0,     0,   996,
       0,  1447,  1448,  1526,     0,  1022,  1006,  1029,  1042,  1050,
    1036,     0,  1042,     0,  1379,  1380,  1065,  1068,     0,     0,
    1334,  1063,     0,  1062,     0,  1092,     0,     0,  1102,     0,
       0,  1111,     0,  1125,  1120,     0,     0,  1136,  1137,  1134,
    1479,     0,     0,  1170,     0,     0,  1029,   126,   206,   204,
     197,   198,     0,   225,   211,  1461,  1442,     0,   495,   500,
     506,   516,   363,   522,  1517,   436,     0,  1319,  1320,     0,
    1312,  1313,  1398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1531,  1549,   334,  1507,  1517,   312,   700,   691,
    1200,   681,   688,   682,   684,   686,     0,  1200,     0,   680,
     687,   694,   693,     0,  1200,  1513,  1513,  1513,   698,   699,
    1376,  1375,     0,  1364,  1311,  1309,     0,     0,  1311,     0,
    1360,  1361,  1362,  1324,  1311,     0,     0,  1311,     0,     0,
    1311,  1311,  1311,     0,     0,  1207,  1453,     0,     0,   738,
       0,  1259,  1260,  1261,  1294,  1262,  1549,  1298,  1303,  1543,
    1269,  1306,  1543,  1287,  1266,  1276,  1258,  1257,  1295,  1265,
    1267,  1277,  1278,  1279,  1280,  1281,  1296,  1250,  1299,  1301,
    1282,  1283,  1284,  1285,  1286,  1253,  1254,  1255,  1256,  1270,
    1293,  1264,  1275,  1252,  1251,  1263,  1272,  1273,  1274,  1271,
    1288,  1289,  1290,  1291,  1292,  1249,     0,     0,  1341,   756,
       0,     0,   763,   785,   786,   779,   781,     0,  1207,  1239,
    1241,   798,  1235,  1236,  1237,     0,  1554,  1200,     0,  1201,
     806,  1203,   807,   804,   805,     0,  1207,  1453,   866,   868,
     867,   861,   863,   869,   872,   847,   859,   855,   853,  1247,
     619,   849,  1356,  1517,   886,  1337,   619,  1549,   912,   913,
     915,   917,   918,   914,   916,   907,  1549,   903,     0,   949,
       0,   951,   950,   952,   943,   944,     0,     0,   929,  1081,
    1519,     0,     0,   963,  1207,  1453,  1553,     0,   990,   620,
     997,   998,  1001,     0,   993,  1192,  1191,  1000,  1006,  1186,
       0,     0,  1234,     0,     0,     0,  1041,     0,     0,     0,
    1066,     0,  1070,  1069,  1060,     0,  1497,  1234,  1107,  1106,
    1113,  1114,  1115,     0,  1207,  1453,     0,  1440,     0,  1115,
    1182,  1172,  1171,  1177,     0,  1179,  1180,  1187,  1442,   199,
       0,   208,   209,   238,   207,  1497,   508,   519,   520,   518,
     524,     0,   450,   451,  1479,   439,   452,   448,   446,  1502,
     444,  1369,  1370,  1371,  1322,  1310,  1315,  1316,  1317,  1318,
    1321,  1399,     0,   559,   472,   333,     0,   692,  1203,   683,
     685,  1200,   689,   679,   719,  1497,   708,   709,  1497,   720,
     710,   711,   714,   724,   721,   712,     0,   722,   713,   723,
     705,   706,   678,  1514,     0,     0,     0,   696,   697,  1378,
    1363,  1377,  1425,  1453,     0,  1429,     0,  1425,  1425,     0,
    1422,  1425,  1425,  1425,     0,  1425,  1425,  1208,   729,  1210,
    1207,   741,     0,  1297,  1544,  1300,  1302,   746,   744,   751,
     752,   594,     0,   762,   761,  1173,  1174,   766,   764,     0,
     784,     0,   789,   619,   619,   799,   797,  1238,   813,   813,
     813,   813,  1497,   818,   831,   832,   819,     0,  1497,   822,
     823,   826,   824,     0,   825,   815,   816,   808,   814,   619,
    1204,  1199,     0,   837,     0,  1247,  1247,   865,   619,   862,
     857,     0,   895,     0,     0,   919,     0,     0,     0,   945,
     947,     0,   939,   955,   940,   941,   934,   935,   955,  1073,
    1497,     0,  1520,  1080,  1502,   964,   967,     0,     0,   981,
     991,   988,   622,     0,     0,  1008,  1007,  1223,  1225,  1025,
    1220,  1221,  1032,  1030,     0,  1247,  1043,  1247,  1037,  1045,
    1058,  1059,  1061,  1449,  1099,  1214,     0,  1453,  1121,     0,
       0,  1441,  1141,  1142,     0,  1145,  1148,  1152,  1146,  1178,
    1517,  1181,  1193,  1465,   205,     0,   226,   227,   223,     0,
       0,   510,     0,  1534,   443,     0,   437,   453,   449,  1497,
     438,   445,  1400,  1497,   467,   314,  1198,   690,     0,     0,
    1243,  1243,   707,   702,   701,   703,  1418,  1207,  1426,     0,
    1438,  1423,  1416,  1436,  1417,  1419,  1420,  1433,  1434,  1421,
    1415,   619,  1211,  1206,   730,   739,  1549,     0,  1549,     0,
     753,   754,     0,   758,   757,   759,  1175,  1176,   769,   767,
     619,   782,   783,  1240,  1242,  1200,  1200,  1200,  1200,     0,
     820,   821,     0,  1243,  1243,   817,  1202,   619,  1207,  1325,
    1207,  1325,   864,   870,   860,   888,   896,   898,   905,   908,
     909,  1487,   920,   901,   906,   955,  1381,  1382,   955,     0,
     938,   936,   937,   942,  1083,     0,  1521,  1497,  1207,   980,
     974,     0,   621,  1002,     0,     0,  1014,     0,   619,   619,
    1026,  1024,  1222,  1033,  1028,  1031,  1038,   619,  1047,  1046,
    1450,     0,     0,  1100,  1091,  1215,  1117,  1217,     0,  1207,
    1207,  1132,  1440,  1144,  1495,  1150,  1495,  1214,     0,  1230,
    1232,  1196,  1194,  1227,  1228,  1195,  1466,     0,   224,   507,
    1497,     0,   512,   517,  1513,   553,   573,   568,  1469,     0,
       0,  1497,  1515,  1497,   523,   529,   530,   531,   540,   532,
     534,   537,   541,   525,   526,   527,   533,   536,   554,   538,
     528,     0,   535,   539,   457,   454,   455,     0,     0,  1390,
     716,  1388,  1389,   715,   718,     0,   717,   731,  1427,  1209,
     619,  1305,  1549,  1308,  1549,   755,   770,   748,   619,   765,
     812,   811,   810,   809,   828,   827,   830,   829,  1205,   839,
       0,   838,     0,   619,   899,   893,   910,  1488,     0,   954,
     946,   955,   957,     0,     0,  1086,  1082,  1077,   968,   983,
       0,     0,  1009,  1497,  1016,     0,  1010,     0,  1013,  1224,
    1226,   619,  1044,   619,  1093,  1094,  1095,  1096,  1097,  1098,
     619,  1118,  1109,  1218,  1213,  1116,  1123,  1122,  1143,     0,
    1495,  1147,     0,  1154,  1166,  1163,  1165,  1164,  1159,  1162,
     619,   619,  1197,  1184,  1229,  1190,  1189,  1504,     0,  1497,
    1497,   514,   552,  1497,   574,   572,   569,   570,  1499,   562,
    1497,  1247,     0,     0,     0,   555,     0,     0,   560,   563,
     566,   456,   447,   473,  1504,  1212,  1304,  1307,   768,  1207,
    1207,   897,     0,   953,   958,     0,  1497,  1084,     0,     0,
    1074,  1076,   982,     0,     0,  1019,  1017,  1018,  1012,  1011,
    1039,  1048,  1216,   619,  1149,     0,  1153,  1155,  1139,  1231,
    1233,  1505,  1506,  1188,   509,     0,     0,  1497,   505,     0,
     561,     0,   558,  1453,   556,   557,   547,   545,   546,   548,
     544,   549,   543,   542,   567,   565,   564,  1244,   841,   840,
     911,   956,     0,  1087,  1497,  1078,  1247,  1015,  1020,  1005,
    1219,  1151,   511,   513,     0,   551,   550,   571,  1085,     0,
    1003,   515,  1088
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1747, -1747, -1747, -1747,  1925, -1747, -1747, -1747,    34, -1747,
   -1747, -1747, -1747, -1747,  1576, -1747, -1747, -1747,  1190, -1747,
   -1747,    33,  1910, -1747, -1747,  1881,   195, -1747, -1747, -1747,
   -1747, -1747,  1745,  1807, -1747, -1747,  1763,   705, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747,  1752, -1747, -1747, -1747, -1747,
    1731, -1747, -1747, -1747, -1747, -1747,   377,   604, -1747, -1747,
   -1747, -1747,  1428, -1747, -1747,  1348,   789, -1747, -1747, -1747,
   -1747, -1747, -1747,  1500, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747,  1815, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,   584, -1747,
     576,   795, -1747, -1747, -1747, -1747, -1747,   993,    82, -1747,
    1349, -1747, -1747, -1747, -1747, -1747, -1747,   132, -1747, -1747,
    1729, -1747,  1852, -1747, -1747, -1747, -1747,  1572, -1747, -1747,
    1849,   551, -1747, -1747, -1747, -1747,  1725, -1747,  1916,  1808,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,  1078, -1747,
   -1747, -1747,  1383, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747,   756, -1747, -1747, -1747,  1726,
    1645, -1747, -1747,   790, -1747, -1747,  -314, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747,    93,    97,  1112,
      99, -1747,   102, -1747, -1747, -1747,   308,   311, -1747,   395,
   -1747,   315, -1747,   -88,   105, -1747, -1747,   106, -1747,   108,
   -1747, -1747, -1747,  1115, -1747, -1747, -1747,   111, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747,  1421, -1747, -1747, -1747,
    1012, -1747,  -933, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
     -74, -1747, -1747, -1747, -1747, -1747,  -202, -1747, -1747, -1747,
   -1747,   276, -1747, -1747,    85, -1747, -1747, -1747, -1747, -1747,
    1873,  1035, -1747,   247, -1747, -1747, -1747, -1747,  1499, -1747,
   -1747, -1747, -1747, -1747, -1747,  -941, -1747, -1747,   168, -1747,
   -1747, -1747, -1747,   936,   574,   578, -1747, -1747,   297, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747,   938, -1747, -1747,   262, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,  -278, -1747,
     227, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747,   730, -1747, -1747,   732, -1747, -1747, -1747, -1747,   457,
     222, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747,    49,   728, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747,   723, -1747, -1747, -1747,
     205, -1747, -1747,   443, -1747, -1747, -1747, -1340, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1122,
     921, -1747, -1747,   198, -1747, -1747,   424, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,   667, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747,   702, -1747,   186, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
     905, -1747,   904, -1747, -1747,  1108, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747,   896,   399, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747,    20, -1747,   405, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747, -1747,  -469, -1747,
   -1231, -1747, -1747, -1304, -1204, -1068, -1747,   355, -1747, -1160,
   -1747, -1747, -1747, -1747,    18, -1747, -1747, -1747, -1747,   -87,
   -1747, -1747,   211, -1747, -1747, -1747, -1747,    13, -1747,  -461,
   -1746, -1747, -1747,   515, -1747,  -906, -1302,  -875, -1220, -1747,
   -1747, -1747, -1229, -1226, -1215, -1213, -1206,   124,  -265,  -280,
    -471,  -915,  -895,  -301,   937, -1053,   -84, -1747, -1120, -1747,
    -881, -1747,   812,  -226,  -136, -1747, -1747,  -675,  1006,  -871,
   -1035,   200,  -686, -1747, -1747,   433, -1052, -1695, -1015,  1161,
    -983,   463,  -638,  -192, -1747,  1076,  -223,  -667,  -722,  -291,
    -619,  -902, -1747, -1747, -1747, -1747, -1747,  1817, -1747, -1107,
     825, -1747, -1747, -1747, -1680,  1202,  -431,  1738,   768,  -456,
   -1747,  1001,  -413,  1453, -1747,  -648, -1747, -1087,  1084,  -438,
     474, -1747, -1747,  -739, -1747, -1038,  -175,  -570,  -528,  -127,
   -1059, -1747,   653, -1376,  -830, -1099, -1747,  1248,  2021,  -925,
   -1747, -1747, -1747, -1747, -1747, -1747, -1747,   607, -1747, -1747,
    -720,  1086,  -130, -1747, -1747
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,     6,     7,     8,    24,    39,    69,   128,
     258,     9,    25,    40,    70,    90,   503,    73,    71,    35,
      11,    21,    27,    42,    57,    58,    17,    37,    77,    97,
      98,   191,   192,   178,    99,   179,   180,   181,   182,   506,
     183,   184,   100,   187,   188,   189,   270,   101,   210,   211,
     212,   213,   463,   387,   528,   460,   461,   462,   214,   370,
     517,   518,   519,   816,   954,   520,   955,   215,   383,   384,
     523,   216,   448,   449,   217,   218,   219,   220,   221,   222,
     223,    48,    78,    80,   104,   102,   135,   389,   464,   547,
     548,   963,   823,  1162,  1370,   549,   959,   550,  1376,  1377,
    1651,  1171,   551,   552,   553,   554,   967,  1165,  1878,   555,
     556,   557,   558,   559,   560,   561,   562,   838,   563,   138,
     108,   109,   110,   111,   149,   112,   397,   398,   471,   113,
     114,    31,    66,   154,    84,   239,   159,   120,   160,   121,
     164,   250,   341,   342,   696,   343,  1407,   892,   578,   344,
     487,   345,   702,   346,   347,   697,   884,   885,   886,   887,
     348,   349,   350,    83,   240,   161,   162,   163,   247,   248,
     325,   476,   478,   326,   327,   669,   409,   410,   573,   876,
     328,   572,   670,   671,   672,   998,   673,   674,   675,   676,
     677,  1665,   678,   989,  1385,  1890,  1666,  1667,  1668,  1669,
    1886,  1670,  2075,  2076,   679,   680,   864,   681,   682,   683,
     576,  1006,   880,   881,  1894,   684,   685,   686,   118,   313,
     158,   403,   244,   474,   568,   569,   570,   841,   983,   984,
    1178,  1179,  1092,   985,  1656,  1881,  2042,  2181,  2248,  1380,
    1659,  1182,  1383,  1883,  2063,  2064,  2263,  2065,  2066,  2067,
    2068,  2254,  2069,  2070,  2071,  2198,  2199,  2188,  2072,  2073,
    2185,   495,   317,   571,    51,   257,   436,    87,   124,   123,
     166,   167,   168,   254,   355,   126,   357,   500,   501,   592,
     593,   594,   595,   596,   896,  1598,  1599,  1832,   597,   760,
     761,   897,  1016,  1217,  1428,  1429,  1424,  1710,  1711,  1214,
     762,   898,  1034,  1240,  1238,   763,   899,  1042,  1459,   764,
     900,  1517,   765,   901,  1250,  1519,  1749,  1750,  1751,  1522,
    1757,  1940,  1938,  2098,  2097,   766,   902,  1056,   767,   903,
    1057,  1525,  1526,   768,   904,  1058,  1256,  1259,   769,   770,
     771,   905,  1766,   772,   906,   773,   907,  1065,  1537,  1785,
    1786,  1267,   774,   908,  1069,  1274,   775,   909,   776,   910,
    1074,  1075,  1280,  1281,  1282,  1560,  1558,  1798,  1283,  1551,
    1552,  1797,  1555,   777,   911,  1081,   778,   912,   779,   913,
     780,  1087,  1564,   781,   915,   782,   917,  1566,  1967,  2113,
    2115,   783,   918,  1291,  1575,  1805,  1969,  1970,  1971,  1973,
     784,   919,   785,   920,  1094,  1297,  1298,  1299,  1587,  1816,
    1817,  1300,  1584,  1585,  1586,  1810,  1301,  1980,  2215,   786,
     921,   787,   922,  1101,   788,   923,  1103,  1306,   789,   924,
    1105,  1312,  1597,  1990,   790,   925,  1108,  1315,  1831,  1109,
    1110,  1111,  1601,  1602,   791,   926,  1611,  1996,  2134,  2225,
    2279,   792,   927,   793,   928,  2001,   794,   929,  1612,  2004,
     795,   796,   930,  1122,  2141,  1332,  1614,  2007,  1848,  1849,
    2143,  1330,   797,   931,  1127,  1128,  1129,  1130,  1344,  1131,
    1132,  1133,  1134,   798,   932,  1098,  1984,  1302,  2221,  1589,
    1819,  2125,  2220,   799,   933,  1345,  1627,  2011,  2014,   800,
    1141,  1140,  1348,   801,   936,  1143,  1144,  1855,  2152,   802,
     937,  1147,  1354,   803,   939,   804,   940,   805,   941,   806,
     942,  1359,   807,   943,  1361,  1862,  1863,  1639,  1865,  2025,
    2161,  2027,  2238,   808,   809,   945,  2168,  1155,  1364,  1643,
    1758,  1939,  1870,   810,  1645,   811,   812,   947,  1325,  1872,
    2122,  2031,  2173,  1687,  1540,  1541,  1789,  1791,  1957,  1738,
    1739,  1921,  1923,  2090,  2016,  2017,  2150,  2154,  2233,  1839,
    1840,  1998,  1841,  1999,  2032,  2033,  2170,  2034,  2171,  1531,
    1532,  1533,  1763,  1534,  1764,  2084,  1089,  1090,  1044,  1045,
    1245,  1246,  1490,  1491,  1492,  1493,  1494,  1191,  1393,  1435,
    1035,  1059,  1260,  1117,  1123,   400,   401,  1135,  1136,  1288,
    1112,  1048,  1049,   300,   301,   483,  1175,   490,   278,  1083,
    1084,  1036,  1061,  1194,  1432,  1720,  1818,  1975,  1067,  1113,
    2080,  1038,   868,   859,   991,   992,  2082,  1039,   878,   879,
    1040,  1223,  1225,  1439,  1453,  1448,  1445,   249,  1864,  1172,
    1243,  1323,  2012,   225,  1262,  1000,   392,   417,  1173,   267,
    2037,  1795,   426,   238,   690,  1218,   619,   169,   616,   293,
     308,  2118,   145,   310,   893,   585,    43,   457,   613,  2243,
     581,  1163,   423,  1714,   233,   230,  1823,   974,   185,  1264,
     856,  1403,   284,   688,   700,   527,   236,  1745,  1290,  1186,
     609,   854,  1538,   813,   814
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     122,   260,   194,   482,   303,   692,   914,  1149,   564,  1060,
    1265,   620,   621,   412,  1600,  1340,   946,  1510,   858,  1047,
    1511,   279,   280,   877,  1043,   882,   285,   286,   287,   288,
     289,  1512,   291,  1513,   294,  1076,   302,   361,   302,   302,
    1514,  1252,   302,  1907,  1003,  1004,    22,  1091,  1287,   888,
    1715,  1716,  1369,   583,   105,  1349,  1287,  1365,  1082,  1553,
    1137,  1082,  1121,   194,  1278,  1082,  1542,  1543,  1544,  1548,
     883,   292,   504,  1333, -1524,   351,   122,   352,   600,   820,
    1579,  1933,   450,    85,   359,  1579,  1082,  1644, -1475,  1607,
    1082,   165,   364,   302,   165,   304,  1422,   302,   306,   302,
     265,   386,  1287,   186,    89,    92,   373,   374,   302,   375,
      14,   302,  1009,  1692,   454,   381,  1390,  1430,   497,   978,
    1712,   127,  1900,  1953, -1517,    52,  2035,  1580,   165,   432,
     407,  1236,  1580,   407,    41,   964,   438,  -733,   454,   488,
    1425,  1126,  1563,   165,   165,   873,   979,   980,   862,   376,
    1275,  1404,   871,   380,   969,   382,   414,   450,  1390,   419,
     421,  1378,   424,  1408,   393,  2021,   307,   393,   466,   255,
     468, -1502,   302,   934,   694,   612,  1341,  2019,   433,  1390,
    1926, -1479,   302,  -345,  -345,  1768, -1553,   485,  1219,   302,
     479,  2218,     3,  1226,  1326,   440,  1390,   491,  1390,   995,
     465,   617,   302,  1591,   302, -1553,    18,    63,  -190,   843,
     499, -1517,  1037,   882, -1553,   268,  1153,   252,  1167,  1321,
    1917,  1037,  1070, -1392,   399,   484,    12,   302,   439,  1167,
    1125,   602, -1497,    55,   524,  -191,  1102,  1104,   451,   961,
     481,   565,  1578,  1154,  1581,   453,  1379,   566,   493,  1581,
    1145,  1037,  1286,  1096,    13, -1553,   175,  1072,    41,   844,
     890,  1073,  1338,   253,  1046,  1322,    91,   965,  1196,  1653,
     863,  -948,    41,  1637,   302,   302,   302,   618,  1431,   256,
    1837,   990,  1054,   489,  2121,  1661,   935,   835,   486,  1812,
     701,   579,  2219,     4,  1582,    41,  1339,    54,  1261,  1582,
     603,  1097,   574,  1287,  1636,   455,  1054,  1686,  1927,  1609,
    1201,  1276,  -948,   399,  1046, -1479,   472,    56,   623,  -948,
    1934,    75,   587,   588,  1455,  1294,  -190,   105,  1126,   455,
     393,   441,   393,  1787,  1209,  1210,  1813,  1126,  2123,  1553,
     872,  1208,  1688,   358,  1060,   299,  1685,   966,  1046,  1688,
    1241,   981,   604,  -191,   693,  1251,  1688,  1386,  1536,   698,
    2135,  1546,   614,  1211,  1691,  1204,  1205, -1479,  1762,   176,
   -1479,  1197,   442,   302, -1524,  1427,   229,   -31,   271,   443,
    1241,  1060,   612,  -587,  1528,  1425,  1793,  1897,  2227,   626,
     498,  -948,  1594,    64,  1769,   703,   703,   687,  1608, -1502,
     266,   408,  2136,  1335,   411,  -345,  1391,  1565,  1918,  1076,
     299,  1181,  1060,   624,  1303,  1771,  1873,  1838,   582, -1479,
   -1479,     5,  2137,  1741,   956,  1901,  1954,   962,   857,   393,
    1935,   819,  1752,  1593,  1826,  -585,   824,  1604,   825,   695,
    1634,   829,   269,   830,    65,   832,   833,   277,  1391, -1475,
     839,   444,  1549,   177,  -948,  1619,  1550,  1145,  1342,   505,
    1060,   601,  1241,    86,  1801,   860,  1196,  1196,  1196,  1391,
     299,    89,  1167,  1010,  1858,   441,  1196, -1479,  1983,  1343,
   -1479,   190,  1583,  -948,  1646,  1279,  1391,  1583,  1391,   152,
    1054,   865,   866,  1335,   299,   299,   299,   299,  1196,  -587,
    1196,   299,  1196,  1814,  1441,  1392,  1196,  1196,  1196,  1196,
    1196,  1196,  1196,  1196,   445,   982,   442, -1497,  -948,  1193,
     299, -1479,   299,   443,  -948,   190,   299, -1479, -1477, -1479,
   -1479,  1770,  -190,   277,  1811,   598,  1405,  1169,   836,   190,
     177,  1874,  1740,   446,  1829,  2008,    55,  1722,  1169,   299,
      63,  -585,  1426,  1456,   299,   309,   106,  1662,  1287,  -191,
    1515,   299,  1060,  1662,  1295,  2124,  1850,   889, -1431,  1197,
    1197,  1197,  1335,  2287,  1212,  1553,    32,  1885,   299,  1197,
    1924,  1433,  1002,   106,   447,  1732,  1755,  1733,  1440,  1545,
    1547,  1567,     4,  1227,  1568,   444,   837,  1569,  1570,  2138,
      16,  1197,   430,  1197,   360,  1197,  1372,  1442,  2164,  1197,
    1197,  1197,  1197,  1197,  1197,  1197,  1197,   957,  1248,  1800,
    1427,  1595,  1875,  1688,    88,  1802,  1719,  1296,  1120,    23,
      56, -1439, -1439,   302,  1335,  2119,  1748,   302,  2120,   175,
     302,   302,   366,  1390,  1663,   960,  1157,   302,  1562,  2165,
    1663,  1327, -1553,  1390,   873,   107,  1624,  2166,   445,   175,
    1628,  1629,  1752,  1631,  1672,   958,   986,   367,  2183,  1635,
    1013,  1196,  1196,  1196,  1196,  1196,  1196,  1196,  1390,   174,
    1615,   431,   107,  1684,  1615,  1794,   873,   446,  1366,   968,
    1876,  1815,  1877,   489,  1335,   874,   971,   972,  1346,  1903,
    1904,  1905,   496,   977,    75, -1553, -1553,  1925,  1050, -1491,
    1192,   502,  1756,  1373,  1206,  1683,    26,  1193,  1193,  1193,
       5,  1825,   299,   883,    75, -1553,  1168,  1193,   447,  1099,
    2167,  1664,  1520,  1114, -1553,  1118,    64,  1118,  1124,    19,
    1099,  2100,  2101,  2102,  2103,    29,  1743,  2087,  1671,  1193,
    1827,  1193,   176,  1193, -1553,  1118,  1978,  1193,  1193,  1193,
    1193,  1193,  1193,  1193,  1193,  1184,  1928,  1673,   302,  1215,
     360,  2028,   176, -1553,  1197,  1197,  1197,  1197,  1197,  1197,
    1197,  2213,  1126,  2145,  1160,   302,  2146,  1759,    93,   150,
    1287,  1169,  -210,   190,  1721,  1115,     5,  2147,  2109,  2148,
    2111,  1200,  1859,  1176,   413, -1475,  2149,   360,  1170,   420,
     299,    30,  2023,  2085,  2085,    20,  2079,   302,   302, -1475,
    1906,  1062,  1943,  1944,  1159,  1911,  1912,  2175,  2128,  1914,
    1915,  1916,   299,  1919,  1920,  1571,  1572,   525, -1553,  1995,
     441,  1174,  1116,   627,  1119,  1931,  1261,  1803,  1956,  1139,
      94,   261,  1347,   246,  1573,  1574,  1806,  1964,  1151,  2156,
    2157,   320, -1553,  1535,  1335,  1335,  2085,  2085,  1605,    33,
     875,   263,   441,   489,   489,  2256,  2257,  1958,  1536,  1960,
   -1553,   442, -1553,     5,   119,   264,  1521,   175,   443,  2252,
      95,  1837,    96,   508,  1929,  1606,  1335,  1959,    34,  1961,
   -1553,  2258,  1161,    36,  1821,   605,   190,   119,  1387,  1388,
    1389,  1261,  1988,   442,   190,   299,  2083,  2176,  1400,  1216,
     443,  1963,  1193,  1193,  1193,  1193,  1193,  1193,  1193,  1124,
   -1491,  2029,  1060,  1391,   509,   190,  1622,   390,   252,  1852,
    1623,   510,  1437,  1391,  2020,   329,  1523,   891,  1444,  1446,
    1447,  1449,    75,  1568,  1452,  1454,  1569,  1570,  1381,   529,
     444,  1292,   530,  1261,  1060,   845,  2104,   264,  1391,  2105,
    2006,  1371, -1475,  1254,  2290,  2169,   299,   531,  1529,  1115,
    2089,  1308,  2079,  1721,   253,  1760,  2159,   532,  2162,  1822,
    1529,   330,   444,  1374,   847,  2086,   312,  1843,   309,  2099,
     176,   318,  1196, -1493,  1641,  1761,   331,  1196,  1642,  1409,
    1410,   944,  1529,   512,   391,   846,  2108,  1844,   146,   302,
     849,   265,  1309,   445,  1394,  1395,  1396,  1397,  1838,  1398,
    1310,  1530, -1437,    38,  1394,  1395,  1396,  1397,  1524,  1398,
   -1475,  2030, -1435,  1530,   848,    49,   332,  2106,  2107,  2268,
    2269,  1411,   446,  1412,  2177,   445,   821,  2139,  2140,  1394,
    1395,  1396,  1397,   190,  1398,  1530,  2142, -1432,  1660,   402,
     850,  2217,  2129,   147,   148,   489,   513, -1553,  2144,  2259,
    2260,   190,    44,   195,   446,   360,  1335,  1413,  1414,  1415,
     360,    45,  2130,   447,  1335,   851,  1050,   196,   197,   362,
     822,    50,  2204,  1311,   533,  1197,   441,  1066,    53,   622,
    1197, -1497,  2196,  1675,  1676,  1677,  1678,  1679,  1680,  1682,
    2261,  2262,  2235,   190,  1527,   447,  1050,   323,   324,   198,
      46,   867, -1497,  1820,  1416,   534,  1417,  1148,    47,   274,
    1977,  1889,  1335,   428,  1418,   852,   516,   442,  1994,  2205,
    2288,  1842,    59,  2197,   443,     5,   869,  2208,    41,   473,
     533,  1316,    61,   333,   429,  2273,  1854,  2292,   199,   200,
      60,   201,  2211,  1050,  1317,   334,   507,   870,  1649,  1726,
     202,   472,  1729,   606,   302,   302,    62,  1734,   611,  1077,
    1078,   534,    10,  1977,  1571,  1572,    10,  1613,   535,    72,
    2230,   251,  2231,  1079,   281,   251,  2091,    68,  2093,  2232,
    1657,  1658,   290,  1573,  1574,  2209,    79,  2210,  1419,   536,
      75,   826,  2193,   203,   204,   415,   444,   416,   827,  2239,
    2240,   297,  1126,   458,   205,   459,  1613,    81,  1466,  1467,
    1652,  1652,   441,  1357,   537,  1358,  2095,   508,  1772,  1080,
     538,  1773,    82,  1193, -1384, -1384, -1384, -1384,  1193,  1774,
    1775,  1776,   103,   335,   336,   116,   542,  1468,  1279,   948,
    1556,  1396,  1397,  1469,  1398,   119,   337,   539,   338,  1269,
    1270,  1271,  1272,   442,   540,   371,   372,   117,   509,   445,
     443,  2277,  2280,   378,   379,   510,  1987,   541,   115,   385,
     388,  1936,   139,  1937,  1471,   125,   134,   511,  1472,   206,
     949,  1950,  1977,  1951,   140,  1713,  2182,   950,   446,  2186,
    2187,   266,   542,   129,   543,  1050, -1383, -1383, -1383, -1383,
     130,   195,   544,  2241,  2242,   141,  1694,  1876,  1695,  1877,
     131,  1696,  -215,   142,  1434,   196,   197,   132,  1438,   133,
     137,  1697,   136,   299,   144,   155,  1450,  1451,   156,   447,
    1017,   157,   444,   545,  1053,   171,   186,   512,   228,   172,
    1068,   173,  2206,  1777,  2207,  1085,  1086,   198,   224,   339,
     232,  1085,  1093,  1095,   227,  1420,   229,   237,  1053,   951,
     456,  1778,   235,   231,  1085,   234,   241,  -756,   242,   243,
    -756,   340,   246,  1093,  1150,   259,  1152,    41,   273,  1779,
     276,  2203,   277,   282,   292,   546,   199,   200,   283,   201,
     143,   299,   305,   307,   207,   445,  1421,   309,   202,   314,
     513,   315,   316,   514,   515,   319,  1882,   353,   354,   360,
     165,   356,   365,   363,  1909,  2223,   369,   377,   368,  1913,
     394,  1853,   952,  1195,   446,   396,   302,  1780,  1394,  1395,
    1396,  1397,   404,  1398,   208,   395,  1828,   405,   422,   302,
    1399,   203,   204,  -756,   425,   427,   434, -1479,   435,   452,
    1879,  1698,   205,  1221,   467,   475,  1699,  -366,   470,  -756,
    1945,  1946,  1947,  1948,   302,   447,   477,   492,   480,  1700,
     516,   494,  1856,   526,   522,  1781,   567,   580,   575,  1856,
     577,   584,  1652,   586,   209,   607,   608,   610,   612,  1253,
    1898,   615,   953,  1899,   190,  1884,  2270,   625,   689,  2271,
    1394,  1395,  1396,  1397,   302,  1398,  1394,  1395,  1396,  1397,
    1486,  1398,  1724,  1488,  1489,   691,  1085,  1701,   699,   706,
    1895,   707,   831,   815,  1085,  1293,   840,   206,  1782,   842,
     853,  1394,  1395,  1396,  1397,   861,  1398,  1019,  1783,  1674,
     855,   894,  1053,   895,  -800,  -756,  1394,  1395,  1396,  1397,
     938,  1398,   617,  1021,  1728,   973,  -756,   970,  1334,   976,
    1910,   990,   987,   996,   988,  1702,  1085,  1949,   997,   993,
    1085,   999,  1005,  1952,  1011,  1008,  1014,  1015,  1041,  1352,
     818,  1055,  1293,  1063,  1071,  1088,  -756,  1156,  2250,  1703,
    1158,  1164,  1177,  -756,  1183,  1198,  1202,  -756,  1185,  -756,
    1784,   883,  -756,  1213,  -756,  2189,  1237,  1220,  1222,  1253,
    1242,  1224,  1093,  1704,  1244,  1985,  1249,  1247,  1705,  1255,
    1261,  1195,  1195,  1195,  1228,  1263,  2039,  1266,  1706,  1273,
    1229,  1195,  1707,  1285,  1289,  -756,  1304,  1230,  1305,  1022,
    -756,  1231,   207,  1232,  1394,  1395,  1396,  1397,  1334,  1398,
     441,  1233,  1731,  1195,  1307,  1195, -1386,  1195,  1234,  1235,
    1314,  1195,  1195,  1195,  1195,  1195,  1195,  1195,  1195,  1991,
    1324,  1318,  1329,  1331,  1320,  1997,  -756,  1126,  1319,   165,
    1187,  1353,   208,  1356,  2077,  1362, -1479,  1024,  2078,  1360,
    1708,   442,  1363,  1025, -1510,   323,  1026,  1375,   443,  1384,
    1709,  1401,  1402,  -756,  1394,  1395,  1396,  1397,  1406,  1398,
    1423,  1457,  1736,  1436,  1539,   472,  1394,  1395,  1396,  1397,
    1458,  1398,  1443,  1516,  1892,  1554,  1559,  1334,  1046,  1027,
    1577,  1596,   209,  -756,  1592,  1610,  1625,  1085,  1618,  -756,
    1633,  1626,  1638,  1650,  1689,  1655,  1693,   295,  -756,  -756,
    1603,  1394,  1395,  1396,  1397,  1398,  1398,  1253,  1713,  2088,
    1690,  1744,  1725,   302,  1616,  1742,  1737,  1426,  -756,  1747,
     444,  1085,  1727,  1765,  -756,  1730,  1735,  1530,  1788,  -756,
    1792,  1790,  2127,  1632,  1796,  1804,  1808,  1830,  1807,  1334,
    1833,  1834,  -756,  1845,  1846,  1640, -1479,  1028,  1860,  1861,
    -756,  1869,  1847,  -756,  1880,  1871,  1664,  1893,  1922,  -756,
    1941,  1942,  1293,  1966,  1972,  1968,  1974,  1979,  2200,  2126,
    1982,  2202,  1986,  2000,  1838,  2003,  1195,  1195,  1195,  1195,
    1195,  1195,  1195,   445,  2013,  2178,  2005,  2015,  2010,   916,
    2022,  2024,  1029,  1030,  -891,  2018,  2192,  -891,  2194,  1334,
    -891,  -891,  -891,  2026,  2036,   302,  2040,  2041,  2043,  2074,
    2096,  2092,   446,  2094,  2110,  2114,  2112,  2117,  1031,  1723,
    2131,  2133,  2132,  2153,  2160,  2151,  2030,  2179,  2180,  2184,
    2172,  2190,  2191,  2052,  2212,  2214,  1032,  2216,  2222,  2237,
    2228,  1085,  2247,  2264,  1033,  2229,  -891,   299,  2276,  2274,
    2278,    15,    28,   447,   437,    74,  1188,  1189,   275,   193,
     272,   262,   296,  1620,  1681,  1190,   599,  1367,   521,   817,
    -891,   226,  1648,  1654,  1368,  2244,   298,  2038,  2224,  1166,
    1989,   151,   153,   828,   311,    67,  -891,   406,   245,   469,
    1012,   704,  1382,   321,  2265,  2200,  2055,   994,  1891,  1888,
    2056,  1887,  2057,  1753,  1754,  2058,  1824,  2201,  2059,  2060,
     628,  2061,  1180,  2195,  2062,  1007,  2266,   170,  1932,   589,
    1992,   302,  1268,  1718,  2245,  2246,  1717,  1902,  2249,  1277,
     302,  1930,  1955,  1557,  1561,  2251,  1799,  1962,  2116,  1576,
    1588,  1981,  2282,  2283,  -891,  -891,  2285,  1809,  2286,  2275,
    1313,  1993,  1835,  1647,  1617,  2009,  1336,  1337,  1867,  1351,
    1138,  2272,  2158,  1896,  1868,  2163,  2174,  2226,  1767,  1334,
    1334,  2002,  -891,  -891,  1851,  1085,  1085,  2255,  1328,  -891,
    1518,  2291,  1052,  -891,   322,  1018,   393,  1199,   873,  1460,
    1590,   418,  2284,  1836,  1001,  1284,   834,  2267,    76,  1746,
    1050,  1334,  -891,  1203,   975,   472,     0,     0,     0,     0,
    -891,     0,  1207,  -891,  -891,     0,  1857,  1050,     0,  2289,
    -891,  1866,  1866,     0,  -891,     0,  -891,     0,     0,  -891,
       0,  -891,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -891,  -891,     0,     0,     0,     0,
    -891,     0,     0,   472,     0,     0,     0,     0,     0,     0,
    -891,  1019,  -891,     0,     0,     0,     0,  -891,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1021,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1018,     0,
    -891,   873,     0,     0,     0,     0,     0,  1253,  1253,  1253,
       0,  1019,     0,  -891,     0,     0,     0,  1195,     0,   472,
       0,     0,  1195,     0,     0,     0,     0,  1021,     0,     0,
       0,     0,     0,     0,     0,  1085,     0,     0,     0,     0,
    -891,     0,     0,     0,     0,  1257,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1022,  1019,     0,  -891,     0,     0,     0,
       0,     0,     0,     0,   441,  -891,  -891,     0,     0,     0,
    1021,     0,     0,     0,  1965,     0,     0,     0,     0,     0,
    1976,  1334,     0,     0,  1142,  -891,     0,     0,     0,  1334,
       0,  -891,     0,  1022,  1023,     0,     0,     0,  1085,     0,
       0,  1024,     0,     0,     0,   442,  1603,  1025,     0,  -891,
    1026,     0,   443,     0,     0,     0,     0,  -891,     0,     0,
    -891,     0,     0,     0,     0,     0,  -891,     0,     0,  -891,
    -891,     0,     0,  1976,  -891,     0,  -891,  1334,  -891,  -891,
    -891,     0,     0,  1027,     0,  1051,     0,  1025,     0,     0,
    1026,     0,     0,  -361,   629,     0,  1022,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,     0,     0,
       0,  2081,  2081,     0,   630,     0,     0,     0,     0,     0,
       0,     0,  1350,  1027,   444,     0,     0,   631,     0,     0,
     632,   633,   634,   635,   636,   637,   638,  1023,     0,     0,
       0,     0,     0,     0,  1024,     0,     0,     0,   442,     0,
    1025,  1028,     0,  1026,     0,   443,     0,     0,     0,     0,
       0,     0,  2081,     0,     0,  2081,     0,   639,     0,   640,
     641,   642,   643,   644,   645,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1027,   445,     0,     0,
       0,  1028,  1976,     0,     0,     0,  1029,  1030,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   646,     0,     0,     0,     0,   446,     0,     0,     0,
       0,     0,  1031,     0,  1093,     0,     0,   444,  -361,     0,
       0,  2155,  -361,     0,     0,     0,  1029,  1030,     0,     0,
    1032,     0,     0,     0,     0,     0,     0,     0,  1033,     0,
    2081,   299,     0,     0,  1028,     0,     0,   447,     0,     0,
   -1497,  -361,  1031,  1018,     0,     0,   873,  1258,     0,     0,
       0,     0,     0,     0,     0,     0,  -361,     0,     0,     0,
    1032, -1497,     0,     0,     0,     0,     0,     0,  1033,     0,
     445,   299,     0,     0,   647,     0,     0,     0,  2081,  1029,
    1030,     0,     0,     0,     0,     0,     0,    41,     0,     0,
     648,     0,     0,     0,     0,     0, -1534,     0,     0,   446,
    -361,  -361,     0,     0,     0,  1031,     0,     0,  -361,   649,
       0,     0,  -361,     0,     0,     0,     0,     0,     0,  1019,
       0,     0,     0,  1032,     0,     0,   650,     0,     0,     0,
       0,  1033,     0,     0,   299,  1021,     0,  1293,     0,     0,
     447,     0,     0,     0,     0,     0,     0,     0,     0,   651,
       0,     0,  2234,     0,     0,  2236,     0,     0,     0,     0,
       0,     0,     0,     0,   652,     0,     0,     0,     0,     0,
       0,   653,     0,   654,     0,     0,     0,     0,     0,     0,
    -361,     0,     0,     0,  -361,  2253,   655,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   656,     0,     0,     0,
       0,  -582,   590,   657,  -622,  -361,  -622,     0,     0,     0,
       0,  -622,     0,     0,     0,     0,     0,  1093,     0,  -622,
       0,  1022,     0,     0,  -361,     0,     0,  -361,  2281,     0,
       0,     0,   441,     0,     0,     0,     0,     0,     0,     0,
     658,     0,   659,   660,   661,     0,     0,     0,     0,     0,
       0,     0,  -622,  -622,     0,     0,     0,     0,     0,     0,
    -622,     0,  1023,     0,     0,     0,   662,     0,     0,  1024,
    1293,     0,  -622,   442,     0,  1025,  -622,     0,  1026,     0,
     443,     0,     0,     0, -1534,     0,     0,     0,  -622,     0,
       0,     0,   663,   664,   665,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   666,     0,     0,   667,   668,
       0,  1027,  -622,     0,     0,     0,     0,  -361,     0,  -622,
    -622,     0,  -361,     0,     0,     0,     0,     0,     0,     0,
    -361,     0,     0,     0,     0,     0,  -582,     0,     0,     0,
    -582,     0,     0,     0,   590,     0,  -622,     0,  -622,     0,
    -622,     0,   444,  -622,     0,     0,     0,     0,     0,     0,
       0,  -622,  -622,     0,     0,     0,  -622,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1028,
    -622,     0,     0,     0,  -582,     0,     0,  -622,     0,     0,
    -622,  -622,     0,     0,  -622,  -622,     0,     0,  -622,     0,
       0,     0,  -622,     0,  1019,  -622,     0,  -622,     0,     0,
    -622,     0,     0,     0,  -622,   445,     0,     0,  -622,     0,
    1021,     0,     0,     0,  1029,  1030,     0,     0,     0,     0,
    -622,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1239,     0,     0,     0,   446,     0,     0,  -622,     0,     0,
    1031,  -622,     0,  -622,  -622,     0,     0,     0,     0,     0,
    -622,  -622,  -622,     0,     0,     0,     0,     0,  1032,   705,
       0,     0,     0,     0,     0,     0,  1033,     0,     0,   299,
       0,     0,     0,  -622,     0,   447,     0,     0,     0,     0,
       0,     0,  -622,     0,     0,     0,     0,     0,  -622,  1106,
       0,     0,     0,     0,  -622,     0,  1022,     0,  -622,     0,
       0,     0,  -582,     0,     0,     0,     0,     0,     0,  -622,
       0,     0,  -622,     0,     0,     0,     0,     0,  -622,  -622,
       0,     0,  -622,  -622,     0,     0,     0,     0,     0,     0,
    -622,  -622,     0,     0,     0,     0,  -622,  -622,  -622,  -622,
       0,     0,  -622,     0,     0,     0,  -622,     0,  1107,     0,
    1025,     0,     0,  1026,     0,     0,  -622,     0,     0,     0,
       0,     0,     0,     0,     0,  -622,     0,     0,     0,     0,
       0,     0,     0,     0,  -622,     0,  -622,  -622,     0,  -622,
    -622,     0,  -622,  -622,     0,  -622,  1027,  -622,     0,     0,
       0, -1467,  -622,     0, -1467,     0,     0, -1467, -1467, -1467,
       0,     0,  1278,  -622,     0,     0, -1467,     0,  -622,     0,
       0,     0,     0,  -622,     0,  -622,     0,     0,     0,     0,
       0,     0,     0,     0,  -622,     0,     0,     0,     0,     0,
    -622,     0,     0,   591,     0,     0,  -622,     0,     0,     0,
    -622,  -622,     0, -1467,     0,     0,     0,     0,  -622,     0,
       0,  -622,     0,     0,  1028,     0,     0,     0,     0,     0,
    -622,     0,     0,     0,     0,     0,     0, -1467,     0,     0,
       0,     0,     0,  -622,     0,     0,     0,     0,  -622,     0,
    -622,     0,     0, -1467,     0,     0,     0,  1018,  -622,     0,
     873,     0,     0,  1495,  1496,  1497,     0,     0,  -622,  1029,
    1030,     0,     0,     0,     0,     0,     0,  -622,     0, -1553,
       0,     0,     0,     0,     0,     0,  -622,     0,  -622,  -622,
       0,     0,  -622,     0,  -622,  1031,     0,     0,     0,  -622,
       0,     0,     0,     0,     0,  -999,     0,     0,     0,  1498,
       0, -1467, -1467,  1032,     0,  -622,     0,     0,  -999,     0,
    -622,  1033,     0,   190,   299,  -622,     0,     0,     0,     0,
       0,     0,     0,  1019,     0,     0,  -622,     0,     0, -1467,
   -1467,     0,     0,     0,     0,   591, -1467,     0,  -622,  1021,
   -1467,     0,  -622,  -622,     0,     0,     0,     0,     0,     0,
    -622,     0,     0,     0,     0,     0,     0,     0,     0, -1467,
       0,     0,     0,     0,     0,     0,     0, -1467,     0,     0,
   -1467, -1467,     0,     0,     0,     0,     0, -1467,     0,     0,
       0, -1467,     0, -1467,     0,     0, -1467,     0, -1467,     0,
       0,     0,     0,     0,     0,     0,     0,  1466,  1467,     0,
       0, -1467, -1467,     0,     0,     0,     0, -1467,     0,     0,
       0,     0,     0,     0,     0,     0,     0, -1467,     0, -1467,
       0,     0,     0,     0, -1467,  1022,  1468,     0,     0,     0,
       0,     0,  1469,     0,     0,     0,   441,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0, -1467,     0,     0,
       0,     0,     0,     0,     0,  1499,     0,     0,     0,     0,
   -1467,     0,     0,  1471,     0,     0,  1023,  1472,     0,     0,
       0,     0,     0,  1024,     0,     0,     0,   442,     0,  1025,
       0,     0,  1026,     0,   443,     0,     0, -1467,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1500,  1501,     0,
       0,     0,     0,  1502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1503,     0,  1027,     0,  1461,  1462,  1463,
    1504,     0,     0, -1467,     0,  1464,     0,     0,     0,     0,
       0,     0, -1467, -1467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1505,     0,     0,     0,     0,     0, -1467,
       0,     0, -1467, -1467,     0,     0,   444,     0, -1467,     0,
       0,     0,   708,  1465,   709,     0,     0,     0,     0,   710,
       0,     0,     0,     0,     0,     0, -1467,   711,     0,     0,
       0,     0,     0,  1028, -1467,     0,     0, -1467,     0,     0,
       0,     0,     0, -1467,     0,     0, -1467, -1467,     0,     0,
       0, -1467,     0, -1467,     0, -1467, -1467, -1467,     0,     0,
     712,   713,     0,     0,     0,     0,     0,     0,   714,   445,
       0,     0,     0,     0,     0,     0,     0,     0,  1029,  1030,
     715,     0,     0,     0,   716,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   717,     0,   446,     0,
       0,     0,     0,     0,  1031,     0,     0,     0,     0,     0,
       0,  1466,  1467,     0,     0,     0,     0,     0,     0,     0,
     718,     0,  1032,     0,     0,     0,     0,   719,   720,     0,
    1033,     0,     0,   299,     0,     0,     0,     0,     0,   447,
    1468,  1019,  1506,  1507,     0,     0,  1469,  1508,     0,  1486,
       0,  1509,  1488,  1489,     0,     0,     0,  1021,   721,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1470,
     722,     0,     0,     0,   723,     0,     0,  1471,     0,     0,
       0,  1472,     0,     0,     0,     0,     0,     0,   724,     0,
       0,     0,     0,     0,     0,   725,     0,     0,   726,   727,
       0,     0,     0,     0,     0,     0,   728,     0,     0,     0,
       0,  1473,  1474,   729,     0,   730,     0,  1475,   731,     0,
       0,     0,     0,     0,     0,     0,     0,  1476,     0,     0,
       0,     0,     0,     0,  1477,     0,  1106,     0,     0,  1478,
       0,     0,     0,  1022,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   732,     0,  1479,     0,   733,
       0,   734,     0,     0,     0,     0,     0,     0,   735,     0,
       0,     0,     0,     0,   708,     0,   709,     0,     0,     0,
       0,   710,     0,     0,     0,     0,     0,     0,     0,   711,
       0,   736,     0,     0,     0,  1051,     0,  1025,     0,     0,
    1026,     0,     0,     0,     0,  1018,   737,     0,   873,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   712,   713,     0,     0,     0,   738,     0,  1215,
     714,     0,     0,  1027,     0,     0,   739,     0,     0,     0,
       0,     0,   715,     0,     0,     0,   716,     0,     0,   740,
       0,     0,     0,     0,   741,     0,   742,     0,   717,     0,
       0,     0,     0,     0,   743, -1475,     0,     0,     0,     0,
       0,     0,     0,     0,   744,     0,     0,     0,     0,     0,
       0,  1019,   718,   745,     0,     0,     0,     0,     0,   719,
     720,     0,   746,  1018,   747,   748,   873,  1021,   749,     0,
     750,  1028,     0,  1480,     0,   751,  1481,  1482,  1483,  1484,
       0,  1485,     0,  1486,  1487,     0,  1488,  1489,     0,     0,
     721,   752,     0,     0,     0,     0,   753,     0,     0,     0,
       0,   754,   722,     0,     0,     0,   723,     0,     0,     0,
       0,     0,   755,     0,     0,     0,  1029,  1030,     0,     0,
     724,     0,     0,     0,   756,     0, -1553,   725,   757,   758,
     726,   727,     0,     0,     0,     0,   759,     0,   728,  1019,
       0,     0,  1031,     0,     0,   729,     0,   730,     0,     0,
     731,     0,  -999,  1022,     0,  1021,     0,     0,     0,     0,
    1032,     0,     0,     0,   441,  -999,  1018,     0,  1033,   873,
     190,   299,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   732,     0,     0,
       0,   733,     0,   734,  1023,     0,     0,     0,     0,     0,
     735,  1024, -1475,     0,     0,   442,  1621,  1025,     0,     0,
    1026,     0,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   736,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   737,     0,
       0,  1022,  1019,  1027,     0,     0,     0,  1018,     0,     0,
     873,     0,   441,     0,  1020,     0,     0,     0,  1021,   738,
       0,     0,     0,     0,     0,     0,     0,     0,   739,     0,
   -1475,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   740,  1023,     0,   444,     0,   741,     0,   742,  1024,
       0,     0,     0,   442,     0,  1025,   743,     0,  1026,     0,
     443,     0,     0,     0,     0,     0,   744,     0,     0,     0,
       0,  1028,     0,     0,     0,   745,     0,     0,     0,     0,
       0,     0,     0,  1019,   746,     0,   747,   748,     0,     0,
     749,  1027,   750,     0,     0,  1100,     0,   751,     0,  1021,
       0,     0,     0,     0,  1022,     0,     0,   445,     0,     0,
       0,     0,     0,   752,     0,   441,  1029,  1030,   753,     0,
       0,     0,     0,   754,     0,     0,  1018,     0,     0,   873,
       0,     0,   444,     0,   755,     0,   446,     0,     0,     0,
       0,     0,  1031,     0,     0,  1023,   756,     0,     0,     0,
     757,   758,  1024,     0,     0,     0,   442,     0,  1025,  1028,
    1032,  1026,     0,   443,     0,     0,     0,     0,  1033,     0,
       0,   299,     0,     0,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,  1022,     0,     0,     0,     0,
       0,     0,     0,     0,  1027,   445,   441,     0,     0,     0,
       0,     0,  1019,     0,  1029,  1030,     0,  1018,     0,     0,
     873,     0,     0,     0,     0,     0,     0,     0,  1021,     0,
       0,  1622,     0,     0,   446,  1623,  1023,     0,     0,     0,
    1031,     0,  1142,  1024,     0,   444,     0,   442,     0,  1025,
       0,     0,  1026,     0,   443,     0,     0,  1018,  1032,     0,
     873,     0,     0,     0,     0,     0,  1033,     0,     0,   299,
       0,     0,  1028,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1027,     0,     0,     0,     0,
       0,     0,     0,  1019,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1146,     0,     0,   445,  1021,
       0,     0,     0,     0,  1022,     0,     0,  1029,  1030,     0,
       0,     0,     0,     0,     0,   441,   444,     0,     0,     0,
       0,     0,     0,  1019,     0,     0,     0,   446,  1018,     0,
       0,   873,     0,  1031,     0,     0,     0,     0,     0,  1021,
       0,     0,     0,  1028,     0,  1023,     0,     0,     0,     0,
       0,  1032,  1024,     0,     0,     0,   442,     0,  1025,  1033,
       0,  1026,   299,   443,     0,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   445,
       0,     0,     0,     0,     0,  1022,     0,     0,  1029,  1030,
       0,     0,     0,     0,  1027,     0,   441,     0,     0,     0,
       0,     0,     0,     0,  1019,     0,     0,     0,   446,     0,
       0,     0,     0,     0,  1031,     0,     0,     0,     0,     0,
    1021,  1355,     0,     0,     0,  1022,  1023,     0,     0,     0,
       0,     0,  1032,  1024,     0,   444,   441,   442,     0,  1025,
    1033,     0,  1026,   299,   443,     0,     0,  1018,     0,   447,
     873,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1028,     0,     0,     0,  1023,     0,     0,  1018,
       0,     0,   873,  1024,     0,  1027,     0,   442,     0,  1025,
       0,     0,  1026,     0,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   445,     0,
       0,     0,     0,     0,     0,     0,  1022,  1029,  1030,     0,
       0,     0,     0,     0,     0,  1027,   444,   441,     0,     0,
       0,     0,     0,  1019,     0,  1018,     0,   446,   873,     0,
       0,     0,     0,  1031,     0,     0,     0,     0,     0,  1021,
       0,     0,     0,  1028,     0,  1019,     0,  1023,     0,     0,
       0,  1032,     0,     0,  1024,     0,   444,     0,   442,  1033,
    1025,  1021,   299,  1026,     0,   443,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   445,
       0,     0,     0,  1028,     0,     0,     0,     0,  1029,  1030,
       0,     0,     0,     0,     0,     0,  1027,     0,     0,     0,
       0,  1019,     0,     0,     0,     0,     0,     0,   446,     0,
       0,     0,     0,   873,  1031,     0,     0,  1021,     0,   445,
       0,     0,     0,     0,     0,  1022,     0,     0,  1029,  1030,
       0,     0,  1032,     0,     0,     0,   441,   444,     0,     0,
    1033,     0,     0,   299,     0,     0,     0,  1022,   446,   447,
       0,     0,     0,     0,  1031,     0,     0,     0,   441,     0,
       0,     0,     0,     0,  1028,     0,  1023,     0,     0,     0,
       0,     0,  1032,  1024,     0,     0,     0,   442,     0,  1025,
    1033,     0,  1026,   299,   443,     0,  1019,     0,  1023,   447,
       0,     0,     0,     0,  1630,  1024,     0,     0,     0,   442,
     445,  1025,  1021,  1022,  1026,     0,   443,     0,     0,  1029,
    1030,     0,     0,     0,   441,  1027,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   446,
       0,     0,     0,     0,     0,  1031,     0,  1027,     0,     0,
       0,     0,     0,     0,  1023,     0,     0,     0,     0,     0,
       0,  1024,     0,  1032,     0,  1064,   444,  1025,     0,     0,
    1026,  1033,   443,     0,   299,     0,     0,     0,     0,     0,
     447,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,  1028,     0,     0,     0,     0,  1022,     0,
       0,     0,     0,  1027,     0,     0,     0,     0,     0,   441,
       0,     0,     0,     0,     0,  1028,     0,     0,     0,     0,
       0,     0,     0,     0,  1737,     0,     0,     0,     0,   445,
       0,     0,     0,     0,     0,     0,     0,     0,  1029,  1030,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
     442,   445,  1025,     0,     0,  1026,     0,   443,   446,     0,
    1029,  1030,     0,     0,  1031,     0,     0,     0,     0,     0,
       0,  1028,     0,     0,     0,     0,     0,     0,     0,     0,
     446,     0,  1032,     0,     0,     0,  1031,     0,  1027,     0,
    1033,     0,     0,   299,     0,     0,     0,     0,     0,   447,
       0,     0,     0,     0,  1032,     0,     0,   445,     0,     0,
       0,     0,  1033,     0,     0,   299,  1029,  1030,     0,     0,
       0,   447,   632,   633,   634,   635,   636,   637,   638,   444,
       0,     0,     0,     0,     0,     0,   446,     0,     0,     0,
       0,     0,  1031,     0,     0,     0,     0,     0,     0,     0,
       0,  2044,  2045,  1019,     0,     0,  1028,     0,     0,   639,
    1032,   640,   641,   642,   643,   644,   645,     0,  1033,  1021,
       0,   299,     0,     0,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   445,     0,   632,   633,   634,   635,   636,   637,
       0,  1029,  1030,   646,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   446,     0,     0,     0,     0,     0,  1031,     0,     0,
       0,   639,     0,   640,   641,   642,   643,   644,   645,     0,
       0,     0,     0,     0,     0,  1032,     0,     0,     0,     0,
       0,     0,     0,  1033,     0,  1022,   299,     0,  1019,     0,
       0,     0,   447,     0,     0,     0,   441,     0,     0,     0,
       0,     0,     0,     0,  1021,   646,     0,  2046,     0,     0,
       0,     0,     0,     0,     0,     0,   647,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1187,     0,     0,     0,
       0,     0,   648,  1024,     0,     0,     0,   442,     0,  1025,
       0,     0,  1026,     0,   443,     0,     0,     0,  2047,  2048,
    1019,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1021,     0,   650,     0,
       0,     0,     0,  2049,     0,  1027,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   647,     0,
    1022,   651,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,     0,     0,     0,     0,   652,     0,     0,     0,
       0,     0,     0,   653,     0,   654,   444,     0,  2050,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   655,     0,
       0,  1187,     0,     0,     0,     0,     0,     0,  1024,     0,
     650,     0,   442,  1028,  1025,     0,     0,  1026,     0,   443,
       0,     0,  1022,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   652,   445,
    1027,     0,   658,     0,   659,   660,   661,   654,  1029,  1030,
       0,  2051,     0,     0,     0,     0,     0,     0,     0,     0,
     655,     0,     0,     0,   442,  2052,  1025,     0,   446,  1026,
       0,   443,     0,     0,  1031,     0,     0,     0,     0,     0,
       0,   444,     0,     0,     0,     0,     0,     0,     0,  2053,
       0,     0,  1032,     0,   663,   664,   665,     0,     0,     0,
    1033,     0,  1027,   299,     0,     0,     0,   666,  1028,   447,
     667,   668,  1188,  1189,     0,     0,   659,   660,   661,     0,
    1908,  1190,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  2054,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   444,   445,     0,     0,     0,     0,     0,
       0,     0,     0,  1029,  1030,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   663,   664,   665,     0,
    1028,     0,     0,   446,     0,     0,     0,     0,     0,  1031,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1032,     0,     0,
       0,     0,     0,     0,     0,  1033,   445,     0,   299,     0,
       0,     0,     0,     0,   447,  1029,  1030,  1188,  1189,     0,
       0,     0,     0,     0,     0,     0,  1190,     0,     0,     0,
       0,     0,     0,     0,     0,   446,     0,     0,     0,     0,
       0,  1031,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1032,
       0,     0,     0,     0,     0,     0,     0,  1033,     0,     0,
     299,     0,     0,     0,     0,     0,   447
};

static const yytype_int16 yycheck[] =
{
      84,   176,   132,   416,   230,   575,   726,   940,   464,   904,
    1063,   539,   540,   327,  1316,  1135,   755,  1246,   656,   900,
    1246,   196,   197,   690,   899,   692,   201,   202,   203,   204,
     205,  1246,   207,  1246,   209,   910,   228,   260,   230,   231,
    1246,  1056,   234,  1723,   874,   875,    13,   918,  1083,   697,
    1426,  1427,  1159,   491,    22,  1142,  1091,  1156,     6,  1279,
     931,     6,     9,   193,    17,     6,  1270,  1271,  1272,    21,
      30,    56,    56,  1125,    63,   250,   160,   252,    17,   607,
       9,    86,   373,    54,   259,     9,     6,     9,    48,  1320,
       6,    49,   267,   285,    49,   231,  1216,   289,   234,   291,
      63,    96,  1137,   182,    70,    72,   281,   282,   300,   284,
       0,   303,    48,  1417,    23,   290,    70,   149,   170,    61,
    1424,    88,   121,   121,    28,    40,  1872,    56,    49,   352,
     171,  1033,    56,   171,   214,    32,   359,   187,    23,   236,
      66,   248,   114,    49,    49,     9,    88,    89,   224,   285,
      49,  1210,   223,   289,   829,   291,   331,   448,    70,   334,
     335,  1176,   337,  1215,   300,  1860,    85,   303,   394,   325,
     396,   214,   364,   236,    56,   218,   121,  1857,   353,    70,
     156,    86,   374,   227,   228,    96,   269,   279,  1018,   381,
     413,   187,     0,  1023,  1119,   370,    70,   420,    70,   866,
     392,    37,   394,  1302,   396,   107,    11,   168,   118,   368,
     433,   115,   898,   880,   174,     9,     8,   372,   122,   258,
     223,   907,   908,   402,   308,   417,   457,   419,   364,   122,
     150,   522,   188,   322,   457,   118,   922,   923,   374,   117,
     415,   467,  1294,    35,   173,   381,  1179,   470,   423,   173,
     936,   937,  1082,   920,   457,   171,   245,   404,   214,   418,
     698,   408,  1133,   418,   236,   304,    71,   164,   990,  1376,
     346,   195,   214,  1360,   466,   467,   468,   113,   310,   435,
     126,   460,   901,   419,  1979,  1384,   349,     9,   380,     9,
     236,   483,   288,   183,   223,   214,   403,    73,   348,   223,
     523,   920,   477,  1338,  1356,   214,   925,  1406,   284,  1324,
     996,   210,   236,   397,   236,   236,   400,   406,   541,   243,
     325,   310,   497,   498,  1239,    92,   236,    22,   248,   214,
     466,   195,   468,  1537,  1009,  1010,    56,   248,   207,  1559,
     411,  1008,  1410,   258,  1239,   442,  1405,   244,   236,  1417,
    1036,   293,   527,   236,   577,   236,  1424,  1187,   441,   582,
     198,  1276,   537,  1011,  1416,  1003,  1004,   325,  1528,   358,
     325,   990,   236,   565,   363,   301,   280,   457,   457,   243,
    1066,  1276,   218,   341,  1259,    66,  1546,  1691,  2134,   564,
     442,   315,  1307,   354,   305,   587,   588,   572,  1323,   442,
     363,   442,   240,  1125,   442,   449,   360,  1288,   411,  1284,
     442,   981,  1307,   543,  1100,  1535,  1647,   263,   403,   325,
     325,   311,   260,  1458,   123,   424,   424,   305,   651,   565,
     435,   606,  1519,  1304,  1594,   341,   611,  1318,   613,   321,
    1355,   616,   236,   618,   405,   620,   621,   442,   360,   409,
     625,   315,   404,   442,   378,   403,   408,  1143,   403,   443,
    1355,   400,  1148,   434,  1563,   657,  1188,  1189,  1190,   360,
     442,   437,   122,   409,  1634,   195,  1198,   435,  1818,   424,
     435,   441,   411,   407,  1365,   438,   360,   411,   360,   457,
    1109,   666,   667,  1215,   442,   442,   442,   442,  1220,   457,
    1222,   442,  1224,   223,  1226,   459,  1228,  1229,  1230,  1231,
    1232,  1233,  1234,  1235,   378,   457,   236,   436,   442,   990,
     442,   442,   442,   243,   448,   441,   442,   448,   364,   435,
     435,   442,   442,   442,  1586,   502,  1211,   441,   260,   441,
     442,  1648,  1457,   407,  1597,  1847,   322,   459,   441,   442,
     168,   457,   233,  1239,   442,   175,   251,    28,  1593,   442,
    1246,   442,  1457,    28,   331,   434,  1618,   697,   459,  1188,
    1189,  1190,  1294,  2253,  1012,  1795,    25,  1664,   442,  1198,
    1740,  1219,   873,   251,   448,   459,   163,   459,  1226,  1275,
    1276,     9,   183,  1024,    12,   315,   318,    15,    16,   437,
     150,  1220,    31,  1222,   236,  1224,   240,  1226,   166,  1228,
    1229,  1230,  1231,  1232,  1233,  1234,  1235,   316,  1049,  1560,
     301,  1307,   251,  1691,   139,  1566,   236,   394,   929,   120,
     406,   227,   228,   825,  1356,  1975,  1517,   829,  1978,   245,
     832,   833,   211,    70,   115,   820,   947,   839,  1286,   207,
     115,  1120,   300,    70,     9,   350,  1342,   215,   378,   245,
    1346,  1347,  1749,  1349,  1386,   364,   841,   236,  2044,  1355,
     893,  1393,  1394,  1395,  1396,  1397,  1398,  1399,    70,   128,
    1328,   110,   350,  1403,  1332,   187,     9,   407,  1157,   825,
     319,   411,   321,   829,  1416,    50,   832,   833,   187,  1714,
    1715,  1716,   426,   839,   310,   240,   269,  1742,   900,   329,
     990,   139,   289,   347,  1005,  1401,    27,  1188,  1189,  1190,
     311,  1592,   442,    30,   310,   260,   376,  1198,   448,   921,
     288,   202,   187,   925,   240,   927,   354,   929,   930,   236,
     932,  1945,  1946,  1947,  1948,   457,  1466,  1907,  1386,  1220,
     187,  1222,   358,  1224,   260,   947,  1808,  1228,  1229,  1230,
    1231,  1232,  1233,  1234,  1235,   988,   156,  1386,   960,    30,
     236,  1870,   358,   300,  1393,  1394,  1395,  1396,  1397,  1398,
    1399,  2121,   248,  2012,   118,   977,  2012,  1526,   275,   457,
    1825,   441,   442,   441,  1432,   925,   311,  2012,  1958,  2012,
    1960,   993,   187,   978,   330,    66,  2012,   236,   458,   335,
     442,   102,  1864,  1900,  1901,   312,   448,  1009,  1010,   126,
    1722,   905,  1763,  1764,   960,  1727,  1728,   293,  1988,  1731,
    1732,  1733,   442,  1735,  1736,   253,   254,   460,   198,   199,
     195,   977,   926,   567,   928,   279,   348,  1567,  1789,   933,
     337,   457,   341,   449,   272,   273,  1576,  1798,   942,  2019,
    2020,   457,   240,   426,  1586,  1587,  1953,  1954,     8,   236,
     225,   457,   195,  1009,  1010,    90,    91,  1792,   441,  1794,
     240,   236,   260,   311,   442,   180,   341,   245,   243,  2191,
     377,   126,   379,   195,   284,    35,  1618,  1792,   457,  1794,
     260,   116,   236,    83,   200,   528,   441,   442,  1188,  1189,
    1190,   348,  1827,   236,   441,   442,  1899,  2037,  1198,   180,
     243,  1796,  1393,  1394,  1395,  1396,  1397,  1398,  1399,  1121,
     107,   154,  1827,   360,   236,   441,   404,   200,   372,  1625,
     408,   243,  1222,   360,  1859,     1,   324,   180,  1228,  1229,
    1230,  1231,   310,    12,  1234,  1235,    15,    16,  1181,     1,
     315,  1091,     4,   348,  1859,   368,  1949,   262,   360,  1952,
    1845,  1163,   233,  1057,  2276,  2028,   442,    19,   213,  1109,
    1921,   166,   448,  1621,   418,   240,  2024,    29,  2026,   285,
     213,    47,   315,  1168,   368,  1901,   240,   240,   175,  1940,
     358,   245,  1724,   236,   159,   260,    62,  1729,   163,    25,
      26,   188,   213,   315,   277,   418,  1957,   260,   319,  1211,
     368,    63,   207,   378,   451,   452,   453,   454,   263,   456,
     215,   266,   459,   120,   451,   452,   453,   454,   416,   456,
     301,   264,   459,   266,   418,   457,   102,  1953,  1954,  2209,
    2210,    67,   407,    69,  2037,   378,   123,  1998,  1999,   451,
     452,   453,   454,   441,   456,   266,  2007,   459,  1382,   313,
     418,  2124,   240,   374,   375,  1211,   378,   437,  2011,   294,
     295,   441,   356,    11,   407,   236,  1808,   103,   104,   105,
     236,   169,   260,   448,  1816,   368,  1288,    25,    26,   457,
     167,   307,  2085,   288,   202,  1724,   195,   907,   457,   260,
    1729,   167,   258,  1393,  1394,  1395,  1396,  1397,  1398,  1399,
     335,   336,  2160,   441,  1254,   448,  1318,   227,   228,    57,
     208,   668,   188,  1589,   150,   233,   152,   937,   216,   457,
    1807,  1669,  1864,   167,   160,   418,   448,   236,  1834,  2090,
    2272,  1612,   457,   299,   243,   311,   167,  2098,   214,   403,
     202,   423,   356,   219,   188,  2218,  1627,  2289,    96,    97,
     457,    99,  2113,  1365,   436,   231,   124,   188,  1370,  1444,
     108,  1265,  1447,   530,  1376,  1377,   457,  1452,   535,   296,
     297,   233,     2,  1860,   253,   254,     6,  1327,   240,   135,
    2141,   166,  2143,   310,   198,   170,  1926,   120,  1928,  2150,
     233,   234,   206,   272,   273,  2110,   457,  2112,   234,   261,
     310,   319,  2052,   151,   152,   319,   315,   321,   326,  2170,
    2171,   225,   248,   278,   162,   280,  1366,   356,   156,   157,
    1376,  1377,   195,   319,   286,   321,  1932,   195,    33,   356,
     292,    36,   444,  1724,   427,   428,   429,   430,  1729,    44,
      45,    46,   169,   319,   320,   356,   364,   185,   438,   195,
     440,   453,   454,   191,   456,   442,   332,   319,   334,   427,
     428,   429,   430,   236,   326,   279,   280,   239,   236,   378,
     243,  2224,  2233,   287,   288,   243,  1824,   339,   457,   293,
     294,   265,    60,   267,   222,   341,   359,   255,   226,   237,
     236,   153,  1979,   155,    72,   270,   271,   243,   407,   270,
     271,   363,   364,   457,   366,  1517,   427,   428,   429,   430,
     457,    11,   374,   233,   234,    93,    31,   319,    33,   321,
     457,    36,   384,   101,  1220,    25,    26,   457,  1224,   457,
     216,    46,   457,   442,   405,   457,  1232,  1233,   356,   448,
     897,   235,   315,   405,   901,   442,   182,   315,   280,   457,
     907,   457,  2092,   158,  2094,   912,   913,    57,   283,   435,
     277,   918,   919,   920,   457,   401,   280,    24,   925,   315,
     384,   176,   395,   403,   931,   403,   457,     6,   356,   332,
       9,   457,   449,   940,   941,   372,   943,   214,   457,   194,
     107,  2078,   442,   384,    56,   457,    96,    97,   367,    99,
     178,   442,   236,    85,   352,   378,   442,   175,   108,   457,
     378,   356,   351,   381,   382,   457,  1659,   372,   283,   236,
      49,   109,   211,   363,  1724,  2131,   249,    68,   457,  1729,
     280,  1626,   378,   990,   407,   280,  1648,   242,   451,   452,
     453,   454,   457,   456,   392,   410,  1596,   356,   249,  1661,
     463,   151,   152,    82,    23,   277,   442,    86,   457,   236,
    1655,   176,   162,  1020,   280,   457,   181,    84,   301,    98,
    1768,  1769,  1770,  1771,  1686,   448,    84,   436,   442,   194,
     448,   442,  1632,   384,   400,   290,   317,   234,   435,  1639,
     403,   200,  1648,   442,   442,   319,   403,   364,   218,  1056,
    1695,    55,   448,  1698,   441,  1661,  2212,   384,    27,  2215,
     451,   452,   453,   454,  1726,   456,   451,   452,   453,   454,
     458,   456,   463,   461,   462,   188,  1083,   242,   372,   457,
    1686,   356,   364,   457,  1091,  1092,   442,   237,   343,   225,
     438,   451,   452,   453,   454,   214,   456,    82,   353,   459,
     346,   109,  1109,   236,   405,   184,   451,   452,   453,   454,
     306,   456,    37,    98,   459,   287,   195,   381,  1125,   281,
    1726,   460,   448,   180,   403,   290,  1133,  1772,    27,   400,
    1137,   362,   464,  1778,   174,   400,   457,   457,   442,  1146,
     604,     9,  1149,   405,   236,   136,   225,   107,  2188,   314,
      17,   441,   172,   232,   188,   460,   236,   236,   402,   238,
     415,    30,   241,   127,   243,  2048,   128,   460,   460,  1176,
     205,   460,  1179,   338,    56,  1820,   434,   403,   343,   130,
     348,  1188,  1189,  1190,   460,   319,  1879,   132,   353,   133,
     460,  1198,   357,   100,   399,   274,   403,   460,   137,   184,
     279,   460,   352,   460,   451,   452,   453,   454,  1215,   456,
     195,   460,   459,  1220,    49,  1222,   402,  1224,   460,   460,
     138,  1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1829,
     180,   400,   143,   436,   397,  1835,   315,   248,   402,    49,
     225,   146,   392,   180,  1889,   333,   325,   232,  1893,   112,
     415,   236,   380,   238,   442,   227,   241,   122,   243,   114,
     425,    49,   365,   342,   451,   452,   453,   454,   114,   456,
     441,   187,   459,   403,   163,  1829,   451,   452,   453,   454,
     341,   456,   442,   309,   459,   134,   438,  1294,   236,   274,
     171,   366,   442,   372,   403,   210,    49,  1304,   403,   378,
     180,   218,   210,   280,   447,   193,   218,   457,   387,   388,
    1317,   451,   452,   453,   454,   456,   456,  1324,   270,   459,
     446,   398,   459,  1985,  1331,   341,   373,   233,   407,   403,
     315,  1338,   459,   131,   413,   459,   459,   266,   269,   418,
     187,   265,  1987,  1350,   400,   331,    49,   138,   403,  1356,
       8,   180,   431,   438,   126,  1362,   435,   342,   403,     9,
     439,   307,   438,   442,   173,   308,   202,   404,   268,   448,
     329,   344,  1379,   125,   110,   435,   435,    49,  2071,  1985,
     175,  2077,   285,   141,   263,   142,  1393,  1394,  1395,  1396,
    1397,  1398,  1399,   378,   144,  2040,   240,   289,   262,     1,
     284,   113,   387,   388,     6,   300,  2051,     9,  2053,  1416,
      12,    13,    14,   394,     7,  2077,   116,   220,    67,   442,
     129,   156,   407,   156,   328,   136,   328,   102,   413,  1436,
      49,   218,   240,   267,    95,   145,   264,    91,   221,   203,
     148,   192,   438,   390,    49,   406,   431,   307,   344,   147,
     240,  1458,   174,   293,   439,   240,    58,   442,   423,   307,
     140,     6,    22,   448,   358,    54,   451,   452,   193,   132,
     188,   178,   211,  1339,   459,   460,   518,  1158,   448,   601,
      82,   136,  1368,  1377,  1159,  2178,   227,  1875,  2133,   966,
    1828,   109,   113,   614,   239,    49,    98,   322,   160,   397,
     892,   588,  1182,   247,  2197,  2198,  1883,   865,  1670,  1668,
    1883,  1666,  1883,  1520,  1521,  1883,  1591,  2075,  1883,  1883,
     569,  1883,   980,  2067,  1883,   880,  2198,   124,  1751,   500,
    1832,  2193,  1066,  1429,  2179,  2180,  1428,  1710,  2183,  1071,
    2202,  1749,  1785,  1281,  1284,  2190,  1559,  1795,  1969,  1291,
    1297,  1816,  2245,  2246,   156,   157,  2249,  1584,  2251,  2221,
    1109,  1833,  1608,  1366,  1332,  1849,  1131,  1133,  1639,  1143,
     932,  2216,  2022,  1688,  1639,  2027,  2033,  2134,  1533,  1586,
    1587,  1840,   184,   185,  1621,  1592,  1593,  2193,  1121,   191,
    1248,  2284,   901,   195,   247,     6,  2202,   991,     9,  1244,
    1302,   333,  2247,  1610,   872,  1074,   623,  2204,    57,  1472,
    2272,  1618,   214,   999,   836,  2169,    -1,    -1,    -1,    -1,
     222,    -1,  1006,   225,   226,    -1,  1633,  2289,    -1,  2274,
     232,  1638,  1639,    -1,   236,    -1,   238,    -1,    -1,   241,
      -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   256,   257,    -1,    -1,    -1,    -1,
     262,    -1,    -1,  2217,    -1,    -1,    -1,    -1,    -1,    -1,
     272,    82,   274,    -1,    -1,    -1,    -1,   279,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     6,    -1,
     302,     9,    -1,    -1,    -1,    -1,    -1,  1714,  1715,  1716,
      -1,    82,    -1,   315,    -1,    -1,    -1,  1724,    -1,  2273,
      -1,    -1,  1729,    -1,    -1,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1742,    -1,    -1,    -1,    -1,
     342,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   184,    82,    -1,   378,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   387,   388,    -1,    -1,    -1,
      98,    -1,    -1,    -1,  1801,    -1,    -1,    -1,    -1,    -1,
    1807,  1808,    -1,    -1,   112,   407,    -1,    -1,    -1,  1816,
      -1,   413,    -1,   184,   225,    -1,    -1,    -1,  1825,    -1,
      -1,   232,    -1,    -1,    -1,   236,  1833,   238,    -1,   431,
     241,    -1,   243,    -1,    -1,    -1,    -1,   439,    -1,    -1,
     442,    -1,    -1,    -1,    -1,    -1,   448,    -1,    -1,   451,
     452,    -1,    -1,  1860,   456,    -1,   458,  1864,   460,   461,
     462,    -1,    -1,   274,    -1,   236,    -1,   238,    -1,    -1,
     241,    -1,    -1,     0,     1,    -1,   184,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,
      -1,  1898,  1899,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   274,   315,    -1,    -1,    34,    -1,    -1,
      37,    38,    39,    40,    41,    42,    43,   225,    -1,    -1,
      -1,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,    -1,
     238,   342,    -1,   241,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,  1949,    -1,    -1,  1952,    -1,    74,    -1,    76,
      77,    78,    79,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   274,   378,    -1,    -1,
      -1,   342,  1979,    -1,    -1,    -1,   387,   388,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,   407,    -1,    -1,    -1,
      -1,    -1,   413,    -1,  2011,    -1,    -1,   315,   135,    -1,
      -1,  2018,   139,    -1,    -1,    -1,   387,   388,    -1,    -1,
     431,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,
    2037,   442,    -1,    -1,   342,    -1,    -1,   448,    -1,    -1,
     167,   168,   413,     6,    -1,    -1,     9,   458,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
     431,   188,    -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,
     378,   442,    -1,    -1,   201,    -1,    -1,    -1,  2085,   387,
     388,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,   407,
     227,   228,    -1,    -1,    -1,   413,    -1,    -1,   235,   236,
      -1,    -1,   239,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    -1,   431,    -1,    -1,   253,    -1,    -1,    -1,
      -1,   439,    -1,    -1,   442,    98,    -1,  2144,    -1,    -1,
     448,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   276,
      -1,    -1,  2159,    -1,    -1,  2162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,    -1,
      -1,   298,    -1,   300,    -1,    -1,    -1,    -1,    -1,    -1,
     307,    -1,    -1,    -1,   311,  2192,   313,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   323,    -1,    -1,    -1,
      -1,     0,     1,   330,     3,   332,     5,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,  2224,    -1,    18,
      -1,   184,    -1,    -1,   351,    -1,    -1,   354,  2235,    -1,
      -1,    -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     367,    -1,   369,   370,   371,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    -1,   225,    -1,    -1,    -1,   393,    -1,    -1,   232,
    2277,    -1,    71,   236,    -1,   238,    75,    -1,   241,    -1,
     243,    -1,    -1,    -1,   411,    -1,    -1,    -1,    87,    -1,
      -1,    -1,   419,   420,   421,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   432,    -1,    -1,   435,   436,
      -1,   274,   111,    -1,    -1,    -1,    -1,   444,    -1,   118,
     119,    -1,   449,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     457,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,     1,    -1,     3,    -1,     5,    -1,
     149,    -1,   315,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,   161,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   342,
     179,    -1,    -1,    -1,   183,    -1,    -1,   186,    -1,    -1,
     189,   190,    -1,    -1,    51,    52,    -1,    -1,   197,    -1,
      -1,    -1,    59,    -1,    82,   204,    -1,   206,    -1,    -1,
     209,    -1,    -1,    -1,    71,   378,    -1,    -1,    75,    -1,
      98,    -1,    -1,    -1,   387,   388,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   246,    -1,    -1,
     413,   250,    -1,   252,   111,    -1,    -1,    -1,    -1,    -1,
     259,   118,   119,    -1,    -1,    -1,    -1,    -1,   431,   126,
      -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,    -1,   442,
      -1,    -1,    -1,   282,    -1,   448,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,   297,   177,
      -1,    -1,    -1,    -1,   161,    -1,   184,    -1,   165,    -1,
      -1,    -1,   311,    -1,    -1,    -1,    -1,    -1,    -1,   318,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,   327,   186,
      -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   340,    -1,    -1,    -1,    -1,   345,   204,   347,   206,
      -1,    -1,   209,    -1,    -1,    -1,   355,    -1,   236,    -1,
     238,    -1,    -1,   241,    -1,    -1,   365,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   374,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   383,    -1,   385,   386,    -1,   246,
     389,    -1,   391,   250,    -1,   252,   274,   396,    -1,    -1,
      -1,     6,   259,    -1,     9,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,   412,    -1,    -1,    21,    -1,   417,    -1,
      -1,    -1,    -1,   422,    -1,   282,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   433,    -1,    -1,    -1,    -1,    -1,
     297,    -1,    -1,   442,    -1,    -1,   445,    -1,    -1,    -1,
     449,   450,    -1,    58,    -1,    -1,    -1,    -1,   457,    -1,
      -1,   318,    -1,    -1,   342,    -1,    -1,    -1,    -1,    -1,
     327,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    -1,    -1,   340,    -1,    -1,    -1,    -1,   345,    -1,
     347,    -1,    -1,    98,    -1,    -1,    -1,     6,   355,    -1,
       9,    -1,    -1,    12,    13,    14,    -1,    -1,   365,   387,
     388,    -1,    -1,    -1,    -1,    -1,    -1,   374,    -1,   397,
      -1,    -1,    -1,    -1,    -1,    -1,   383,    -1,   385,   386,
      -1,    -1,   389,    -1,   391,   413,    -1,    -1,    -1,   396,
      -1,    -1,    -1,    -1,    -1,   423,    -1,    -1,    -1,    58,
      -1,   156,   157,   431,    -1,   412,    -1,    -1,   436,    -1,
     417,   439,    -1,   441,   442,   422,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,   433,    -1,    -1,   184,
     185,    -1,    -1,    -1,    -1,   442,   191,    -1,   445,    98,
     195,    -1,   449,   450,    -1,    -1,    -1,    -1,    -1,    -1,
     457,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   222,    -1,    -1,
     225,   226,    -1,    -1,    -1,    -1,    -1,   232,    -1,    -1,
      -1,   236,    -1,   238,    -1,    -1,   241,    -1,   243,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   157,    -1,
      -1,   256,   257,    -1,    -1,    -1,    -1,   262,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   272,    -1,   274,
      -1,    -1,    -1,    -1,   279,   184,   185,    -1,    -1,    -1,
      -1,    -1,   191,    -1,    -1,    -1,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   302,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,    -1,    -1,
     315,    -1,    -1,   222,    -1,    -1,   225,   226,    -1,    -1,
      -1,    -1,    -1,   232,    -1,    -1,    -1,   236,    -1,   238,
      -1,    -1,   241,    -1,   243,    -1,    -1,   342,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   256,   257,    -1,
      -1,    -1,    -1,   262,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   272,    -1,   274,    -1,    12,    13,    14,
     279,    -1,    -1,   378,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,   387,   388,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   302,    -1,    -1,    -1,    -1,    -1,   404,
      -1,    -1,   407,   408,    -1,    -1,   315,    -1,   413,    -1,
      -1,    -1,     3,    58,     5,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,   431,    18,    -1,    -1,
      -1,    -1,    -1,   342,   439,    -1,    -1,   442,    -1,    -1,
      -1,    -1,    -1,   448,    -1,    -1,   451,   452,    -1,    -1,
      -1,   456,    -1,   458,    -1,   460,   461,   462,    -1,    -1,
      51,    52,    -1,    -1,    -1,    -1,    -1,    -1,    59,   378,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,   388,
      71,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,   407,    -1,
      -1,    -1,    -1,    -1,   413,    -1,    -1,    -1,    -1,    -1,
      -1,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,   431,    -1,    -1,    -1,    -1,   118,   119,    -1,
     439,    -1,    -1,   442,    -1,    -1,    -1,    -1,    -1,   448,
     185,    82,   451,   452,    -1,    -1,   191,   456,    -1,   458,
      -1,   460,   461,   462,    -1,    -1,    -1,    98,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     161,    -1,    -1,    -1,   165,    -1,    -1,   222,    -1,    -1,
      -1,   226,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,
      -1,   256,   257,   204,    -1,   206,    -1,   262,   209,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   272,    -1,    -1,
      -1,    -1,    -1,    -1,   279,    -1,   177,    -1,    -1,   284,
      -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   246,    -1,   302,    -1,   250,
      -1,   252,    -1,    -1,    -1,    -1,    -1,    -1,   259,    -1,
      -1,    -1,    -1,    -1,     3,    -1,     5,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,   282,    -1,    -1,    -1,   236,    -1,   238,    -1,    -1,
     241,    -1,    -1,    -1,    -1,     6,   297,    -1,     9,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    52,    -1,    -1,    -1,   318,    -1,    30,
      59,    -1,    -1,   274,    -1,    -1,   327,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    75,    -1,    -1,   340,
      -1,    -1,    -1,    -1,   345,    -1,   347,    -1,    87,    -1,
      -1,    -1,    -1,    -1,   355,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   365,    -1,    -1,    -1,    -1,    -1,
      -1,    82,   111,   374,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,   383,     6,   385,   386,     9,    98,   389,    -1,
     391,   342,    -1,   448,    -1,   396,   451,   452,   453,   454,
      -1,   456,    -1,   458,   459,    -1,   461,   462,    -1,    -1,
     149,   412,    -1,    -1,    -1,    -1,   417,    -1,    -1,    -1,
      -1,   422,   161,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   433,    -1,    -1,    -1,   387,   388,    -1,    -1,
     179,    -1,    -1,    -1,   445,    -1,   397,   186,   449,   450,
     189,   190,    -1,    -1,    -1,    -1,   457,    -1,   197,    82,
      -1,    -1,   413,    -1,    -1,   204,    -1,   206,    -1,    -1,
     209,    -1,   423,   184,    -1,    98,    -1,    -1,    -1,    -1,
     431,    -1,    -1,    -1,   195,   436,     6,    -1,   439,     9,
     441,   442,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   246,    -1,    -1,
      -1,   250,    -1,   252,   225,    -1,    -1,    -1,    -1,    -1,
     259,   232,   233,    -1,    -1,   236,   149,   238,    -1,    -1,
     241,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   282,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   297,    -1,
      -1,   184,    82,   274,    -1,    -1,    -1,     6,    -1,    -1,
       9,    -1,   195,    -1,    94,    -1,    -1,    -1,    98,   318,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   327,    -1,
     301,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   340,   225,    -1,   315,    -1,   345,    -1,   347,   232,
      -1,    -1,    -1,   236,    -1,   238,   355,    -1,   241,    -1,
     243,    -1,    -1,    -1,    -1,    -1,   365,    -1,    -1,    -1,
      -1,   342,    -1,    -1,    -1,   374,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,   383,    -1,   385,   386,    -1,    -1,
     389,   274,   391,    -1,    -1,    94,    -1,   396,    -1,    98,
      -1,    -1,    -1,    -1,   184,    -1,    -1,   378,    -1,    -1,
      -1,    -1,    -1,   412,    -1,   195,   387,   388,   417,    -1,
      -1,    -1,    -1,   422,    -1,    -1,     6,    -1,    -1,     9,
      -1,    -1,   315,    -1,   433,    -1,   407,    -1,    -1,    -1,
      -1,    -1,   413,    -1,    -1,   225,   445,    -1,    -1,    -1,
     449,   450,   232,    -1,    -1,    -1,   236,    -1,   238,   342,
     431,   241,    -1,   243,    -1,    -1,    -1,    -1,   439,    -1,
      -1,   442,    -1,    -1,    -1,    -1,    -1,   448,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   274,   378,   195,    -1,    -1,    -1,
      -1,    -1,    82,    -1,   387,   388,    -1,     6,    -1,    -1,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,   404,    -1,    -1,   407,   408,   225,    -1,    -1,    -1,
     413,    -1,   112,   232,    -1,   315,    -1,   236,    -1,   238,
      -1,    -1,   241,    -1,   243,    -1,    -1,     6,   431,    -1,
       9,    -1,    -1,    -1,    -1,    -1,   439,    -1,    -1,   442,
      -1,    -1,   342,    -1,    -1,   448,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,   378,    98,
      -1,    -1,    -1,    -1,   184,    -1,    -1,   387,   388,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   315,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    -1,   407,     6,    -1,
      -1,     9,    -1,   413,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,    -1,   342,    -1,   225,    -1,    -1,    -1,    -1,
      -1,   431,   232,    -1,    -1,    -1,   236,    -1,   238,   439,
      -1,   241,   442,   243,    -1,    -1,    -1,    -1,   448,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   378,
      -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,   387,   388,
      -1,    -1,    -1,    -1,   274,    -1,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,   407,    -1,
      -1,    -1,    -1,    -1,   413,    -1,    -1,    -1,    -1,    -1,
      98,   180,    -1,    -1,    -1,   184,   225,    -1,    -1,    -1,
      -1,    -1,   431,   232,    -1,   315,   195,   236,    -1,   238,
     439,    -1,   241,   442,   243,    -1,    -1,     6,    -1,   448,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   342,    -1,    -1,    -1,   225,    -1,    -1,     6,
      -1,    -1,     9,   232,    -1,   274,    -1,   236,    -1,   238,
      -1,    -1,   241,    -1,   243,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   378,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,   387,   388,    -1,
      -1,    -1,    -1,    -1,    -1,   274,   315,   195,    -1,    -1,
      -1,    -1,    -1,    82,    -1,     6,    -1,   407,     9,    -1,
      -1,    -1,    -1,   413,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,    -1,   342,    -1,    82,    -1,   225,    -1,    -1,
      -1,   431,    -1,    -1,   232,    -1,   315,    -1,   236,   439,
     238,    98,   442,   241,    -1,   243,    -1,    -1,   448,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   378,
      -1,    -1,    -1,   342,    -1,    -1,    -1,    -1,   387,   388,
      -1,    -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,   407,    -1,
      -1,    -1,    -1,     9,   413,    -1,    -1,    98,    -1,   378,
      -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,   387,   388,
      -1,    -1,   431,    -1,    -1,    -1,   195,   315,    -1,    -1,
     439,    -1,    -1,   442,    -1,    -1,    -1,   184,   407,   448,
      -1,    -1,    -1,    -1,   413,    -1,    -1,    -1,   195,    -1,
      -1,    -1,    -1,    -1,   342,    -1,   225,    -1,    -1,    -1,
      -1,    -1,   431,   232,    -1,    -1,    -1,   236,    -1,   238,
     439,    -1,   241,   442,   243,    -1,    82,    -1,   225,   448,
      -1,    -1,    -1,    -1,   372,   232,    -1,    -1,    -1,   236,
     378,   238,    98,   184,   241,    -1,   243,    -1,    -1,   387,
     388,    -1,    -1,    -1,   195,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   407,
      -1,    -1,    -1,    -1,    -1,   413,    -1,   274,    -1,    -1,
      -1,    -1,    -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,
      -1,   232,    -1,   431,    -1,   236,   315,   238,    -1,    -1,
     241,   439,   243,    -1,   442,    -1,    -1,    -1,    -1,    -1,
     448,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,    -1,
      -1,    -1,    -1,   342,    -1,    -1,    -1,    -1,   184,    -1,
      -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,    -1,   195,
      -1,    -1,    -1,    -1,    -1,   342,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   373,    -1,    -1,    -1,    -1,   378,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,   388,
      -1,    -1,    -1,    -1,   315,    -1,    -1,    -1,    -1,    -1,
     236,   378,   238,    -1,    -1,   241,    -1,   243,   407,    -1,
     387,   388,    -1,    -1,   413,    -1,    -1,    -1,    -1,    -1,
      -1,   342,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     407,    -1,   431,    -1,    -1,    -1,   413,    -1,   274,    -1,
     439,    -1,    -1,   442,    -1,    -1,    -1,    -1,    -1,   448,
      -1,    -1,    -1,    -1,   431,    -1,    -1,   378,    -1,    -1,
      -1,    -1,   439,    -1,    -1,   442,   387,   388,    -1,    -1,
      -1,   448,    37,    38,    39,    40,    41,    42,    43,   315,
      -1,    -1,    -1,    -1,    -1,    -1,   407,    -1,    -1,    -1,
      -1,    -1,   413,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    82,    -1,    -1,   342,    -1,    -1,    74,
     431,    76,    77,    78,    79,    80,    81,    -1,   439,    98,
      -1,   442,    -1,    -1,    -1,    -1,    -1,   448,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   378,    -1,    37,    38,    39,    40,    41,    42,
      -1,   387,   388,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   407,    -1,    -1,    -1,    -1,    -1,   413,    -1,    -1,
      -1,    74,    -1,    76,    77,    78,    79,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,   431,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   439,    -1,   184,   442,    -1,    82,    -1,
      -1,    -1,   448,    -1,    -1,    -1,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,   118,    -1,   192,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   225,    -1,    -1,    -1,
      -1,    -1,   217,   232,    -1,    -1,    -1,   236,    -1,   238,
      -1,    -1,   241,    -1,   243,    -1,    -1,    -1,   233,   234,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   253,    -1,
      -1,    -1,    -1,   258,    -1,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,    -1,
     184,   276,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,
      -1,    -1,    -1,   298,    -1,   300,   315,    -1,   303,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   313,    -1,
      -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,
     253,    -1,   236,   342,   238,    -1,    -1,   241,    -1,   243,
      -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,   378,
     274,    -1,   367,    -1,   369,   370,   371,   300,   387,   388,
      -1,   376,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     313,    -1,    -1,    -1,   236,   390,   238,    -1,   407,   241,
      -1,   243,    -1,    -1,   413,    -1,    -1,    -1,    -1,    -1,
      -1,   315,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   414,
      -1,    -1,   431,    -1,   419,   420,   421,    -1,    -1,    -1,
     439,    -1,   274,   442,    -1,    -1,    -1,   432,   342,   448,
     435,   436,   451,   452,    -1,    -1,   369,   370,   371,    -1,
     459,   460,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   457,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   315,   378,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   387,   388,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   419,   420,   421,    -1,
     342,    -1,    -1,   407,    -1,    -1,    -1,    -1,    -1,   413,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   431,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   439,   378,    -1,   442,    -1,
      -1,    -1,    -1,    -1,   448,   387,   388,   451,   452,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   460,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   407,    -1,    -1,    -1,    -1,
      -1,   413,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   431,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,    -1,
     442,    -1,    -1,    -1,    -1,    -1,   448
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   466,   467,     0,   183,   311,   468,   469,   470,   476,
     483,   485,   457,   457,     0,   469,   150,   491,   491,   236,
     312,   486,   486,   120,   471,   477,    27,   487,   487,   457,
     102,   596,   596,   236,   457,   484,    83,   492,   120,   472,
     478,   214,   488,  1141,   356,   169,   208,   216,   546,   457,
     307,   729,   729,   457,    73,   322,   406,   489,   490,   457,
     457,   356,   457,   168,   354,   405,   597,   603,   120,   473,
     479,   483,   135,   482,   490,   310,  1153,   493,   547,   457,
     548,   356,   444,   628,   599,    54,   434,   732,   139,   473,
     480,   491,   486,   275,   337,   377,   379,   494,   495,   499,
     507,   512,   550,   169,   549,    22,   251,   350,   585,   586,
     587,   588,   590,   594,   595,   457,   356,   239,   683,   442,
     602,   604,  1071,   734,   733,   341,   740,   486,   474,   457,
     457,   457,   457,   457,   359,   551,   457,   216,   584,    60,
      72,    93,   101,   178,   405,  1137,   319,   374,   375,   589,
     457,   587,   457,   595,   598,   457,   356,   235,   685,   601,
     603,   630,   631,   632,   605,    49,   735,   736,   737,  1132,
     735,   442,   457,   457,   596,   245,   358,   442,   498,   500,
     501,   502,   503,   505,   506,  1153,   182,   508,   509,   510,
     441,   496,   497,   498,  1167,    11,    25,    26,    57,    96,
      97,    99,   108,   151,   152,   162,   237,   352,   392,   442,
     513,   514,   515,   516,   523,   532,   536,   539,   540,   541,
     542,   543,   544,   545,   283,  1118,   550,   457,   280,   280,
    1150,   403,   277,  1149,   403,   395,  1161,    24,  1128,   600,
     629,   457,   356,   332,   687,   604,   449,   633,   634,  1112,
     606,   736,   372,   418,   738,   325,   435,   730,   475,   372,
    1141,   457,   501,   457,   502,    63,   363,  1124,     9,   236,
     511,   457,   510,   457,   457,   497,   107,   442,  1083,  1141,
    1141,  1083,   384,   367,  1157,  1141,  1141,  1141,  1141,  1141,
    1083,  1141,    56,  1134,  1141,   457,   515,  1083,   585,   442,
    1078,  1079,  1098,  1078,  1079,   236,  1079,    85,  1135,   175,
    1138,   601,   630,   684,   457,   356,   351,   727,   630,   457,
     457,   634,  1112,   227,   228,   635,   638,   639,   645,     1,
      47,    62,   102,   219,   231,   319,   320,   332,   334,   435,
     457,   607,   608,   610,   614,   616,   618,   619,   625,   626,
     627,  1141,  1141,   372,   283,   739,   109,   741,   729,  1141,
     236,  1101,   457,   363,  1141,   211,   211,   236,   457,   249,
     524,  1083,  1083,  1141,  1141,  1141,  1079,    68,  1083,  1083,
    1079,  1141,  1079,   533,   534,  1083,    96,   518,  1083,   552,
     200,   277,  1121,  1079,   280,   410,   280,   591,   592,  1071,
    1070,  1071,   630,   686,   457,   356,   635,   171,   442,   641,
     642,   442,   641,  1135,  1141,   319,   321,  1122,  1122,  1141,
    1135,  1141,   249,  1147,  1141,    23,  1127,   277,   167,   188,
      31,   110,  1101,  1141,   442,   457,   731,   479,  1101,  1079,
    1141,   195,   236,   243,   315,   378,   407,   448,   537,   538,
    1104,  1079,   236,  1079,    23,   214,  1083,  1142,   278,   280,
     520,   521,   522,   517,   553,  1098,  1078,   280,  1078,   592,
     301,   593,  1071,   630,   688,   457,   636,    84,   637,  1101,
     442,  1141,  1127,  1080,  1098,   279,   380,   615,   236,  1079,
    1082,  1101,   436,  1141,   442,   726,   726,   170,   442,  1101,
     742,   743,   139,   481,    56,   443,   504,   124,   195,   236,
     243,   255,   315,   378,   381,   382,   448,   525,   526,   527,
     530,   538,   400,   535,  1101,   521,   384,  1160,   519,     1,
       4,    19,    29,   202,   233,   240,   261,   286,   292,   319,
     326,   339,   364,   366,   374,   405,   457,   554,   555,   560,
     562,   567,   568,   569,   570,   574,   575,   576,   577,   578,
     579,   580,   581,   583,  1124,  1078,  1101,   317,   689,   690,
     691,   728,   646,   643,  1141,   435,   675,   403,   613,  1098,
     234,  1145,   403,  1134,   200,  1140,   442,  1141,  1141,   743,
       1,   442,   744,   745,   746,   747,   748,   753,   486,   527,
      17,   400,  1104,  1101,  1141,   521,  1147,   319,   403,  1165,
     364,  1147,   218,  1143,  1141,    55,  1133,    37,   113,  1131,
    1143,  1143,   260,  1101,  1167,   384,  1141,   726,   691,     1,
      21,    34,    37,    38,    39,    40,    41,    42,    43,    74,
      76,    77,    78,    79,    80,    81,   118,   201,   217,   236,
     253,   276,   291,   298,   300,   313,   323,   330,   367,   369,
     370,   371,   393,   419,   420,   421,   432,   435,   436,   640,
     647,   648,   649,   651,   652,   653,   654,   655,   657,   669,
     670,   672,   673,   674,   680,   681,   682,  1141,  1158,    27,
    1129,   188,  1142,  1101,    56,   321,   609,   620,  1101,   372,
    1159,   236,   617,  1098,   617,   126,   457,   356,     3,     5,
      10,    18,    51,    52,    59,    71,    75,    87,   111,   118,
     119,   149,   161,   165,   179,   186,   189,   190,   197,   204,
     206,   209,   246,   250,   252,   259,   282,   297,   318,   327,
     340,   345,   347,   355,   365,   374,   383,   385,   386,   389,
     391,   396,   412,   417,   422,   433,   445,   449,   450,   457,
     754,   755,   765,   770,   774,   777,   790,   793,   798,   803,
     804,   805,   808,   810,   817,   821,   823,   838,   841,   843,
     845,   848,   850,   856,   865,   867,   884,   886,   889,   893,
     899,   909,   916,   918,   921,   925,   926,   937,   948,   958,
     964,   968,   974,   978,   980,   982,   984,   987,   998,   999,
    1008,  1010,  1011,  1168,  1169,   457,   528,   530,  1083,  1141,
    1143,   123,   167,   557,  1141,  1141,   319,   326,   575,  1141,
    1141,   364,  1141,  1141,  1128,     9,   260,   318,   582,  1141,
     442,   692,   225,   368,   418,   368,   418,   368,   418,   368,
     418,   368,   418,   438,  1166,   346,  1155,  1101,  1097,  1098,
    1098,   214,   224,   346,   671,  1141,  1141,  1096,  1097,   167,
     188,   223,   411,     9,    50,   225,   644,  1102,  1103,  1104,
     677,   678,  1102,    30,   621,   622,   623,   624,  1130,  1167,
    1134,   180,   612,  1139,   109,   236,   749,   756,   766,   771,
     775,   778,   791,   794,   799,   806,   809,   811,   818,   822,
     824,   839,   842,   844,  1165,   849,     1,   851,   857,   866,
     868,   885,   887,   890,   894,   900,   910,   917,   919,   922,
     927,   938,   949,   959,   236,   349,   969,   975,   306,   979,
     981,   983,   985,   988,   188,  1000,  1138,  1012,   195,   236,
     243,   315,   378,   448,   529,   531,   123,   316,   364,   561,
    1141,   117,   305,   556,    32,   164,   244,   571,  1079,  1082,
     381,  1079,  1079,   287,  1152,  1152,   281,  1079,    61,    88,
      89,   293,   457,   693,   694,   698,  1141,   448,   403,   658,
     460,  1099,  1100,   400,   654,  1102,   180,    27,   650,   362,
    1120,  1120,  1104,  1149,  1149,   464,   676,   678,   400,    48,
     409,   174,   613,  1101,   457,   457,   757,  1096,     6,    82,
      94,    98,   184,   225,   232,   238,   241,   274,   342,   387,
     388,   413,   431,   439,   767,  1065,  1086,  1087,  1096,  1102,
    1105,   442,   772,  1052,  1053,  1054,   236,  1075,  1076,  1077,
    1098,   236,  1094,  1096,  1105,     9,   792,   795,   800,  1066,
    1067,  1087,  1071,   405,   236,   812,  1086,  1093,  1096,   819,
    1087,   236,   404,   408,   825,   826,  1052,   296,   297,   310,
     356,   840,     6,  1084,  1085,  1096,  1096,   846,   136,  1051,
    1052,  1084,   697,  1096,   869,  1096,  1102,  1105,   950,  1098,
      94,   888,  1087,   891,  1087,   895,   177,   236,   901,   904,
     905,   906,  1075,  1094,  1098,  1167,  1071,  1068,  1098,  1071,
    1068,     9,   928,  1069,  1098,   150,   248,   939,   940,   941,
     942,   944,   945,   946,   947,  1072,  1073,  1084,   950,  1071,
     966,   965,   112,   970,   971,  1087,    94,   976,  1086,   697,
    1096,  1071,  1096,     8,    35,  1002,   107,  1068,    17,  1079,
     118,   236,   558,  1146,   441,   572,   572,   122,   376,   441,
     458,   566,  1114,  1123,  1079,  1081,  1141,   172,   695,   696,
     695,  1142,   706,   188,  1101,   402,  1164,   225,   451,   452,
     460,  1062,  1064,  1065,  1088,  1096,  1103,  1105,   460,  1100,
    1098,  1087,   236,  1133,  1097,  1097,  1104,  1166,  1102,  1082,
    1082,  1130,  1134,   127,   764,    30,   180,   758,  1130,  1149,
     460,  1096,   460,  1106,   460,  1107,  1149,  1121,   460,   460,
     460,   460,   460,   460,   460,   460,  1106,   128,   769,   403,
     768,  1087,   205,  1115,    56,  1055,  1056,   403,  1121,   434,
     779,   236,  1093,  1096,  1071,   130,   801,   156,   458,   802,
    1067,   348,  1119,   319,  1154,  1070,   132,   816,   758,   427,
     428,   429,   430,   133,   820,    49,   210,   779,    17,   438,
     827,   828,   829,   833,  1126,   100,  1149,  1085,  1074,   399,
    1163,   858,  1167,  1096,    92,   331,   394,   870,   871,   872,
     876,   881,   952,  1087,   403,   137,   892,    49,   166,   207,
     215,   288,   896,   905,   138,   902,   423,   436,   400,   402,
     397,   258,   304,  1116,   180,  1013,  1154,  1013,  1069,   143,
     936,   436,   930,  1091,  1096,  1103,   945,   947,  1084,   403,
    1073,   121,   403,   424,   943,   960,   187,   341,   967,  1132,
     210,   971,  1096,   146,   977,   180,   180,   319,   321,   986,
     112,   989,   333,   380,  1003,  1150,  1013,   531,   566,  1114,
     559,  1098,   240,   347,  1141,   122,   563,   564,  1093,   697,
     704,  1101,   638,   707,   114,   659,  1149,  1064,  1064,  1064,
      70,   360,   459,  1063,   451,   452,   453,   454,   456,   463,
    1064,    49,   365,  1156,  1145,  1082,   114,   611,  1091,    25,
      26,    67,    69,   103,   104,   105,   150,   152,   160,   234,
     401,   442,  1073,   441,   761,    66,   233,   301,   759,   760,
     149,   310,  1089,  1097,  1062,  1064,   403,  1064,  1062,  1108,
    1097,  1103,  1105,   442,  1064,  1111,  1064,  1064,  1110,  1064,
    1062,  1062,  1064,  1109,  1064,  1066,  1087,   187,   341,   773,
    1115,    12,    13,    14,    20,    58,   156,   157,   185,   191,
     214,   222,   226,   256,   257,   262,   272,   279,   284,   302,
     448,   451,   452,   453,   454,   456,   458,   459,   461,   462,
    1057,  1058,  1059,  1060,  1061,    12,    13,    14,    58,   214,
     256,   257,   262,   272,   279,   302,   451,   452,   456,   460,
    1057,  1058,  1059,  1060,  1061,  1087,   309,   776,  1077,   780,
     187,   341,   784,   324,   416,   796,   797,  1167,  1052,   213,
     266,  1044,  1045,  1046,  1048,   426,   441,   813,  1167,   163,
    1019,  1020,  1019,  1019,  1019,  1087,  1066,  1087,    21,   404,
     408,   834,   835,  1053,   134,   837,   440,   829,   831,   438,
     830,   826,  1097,   114,   847,  1075,   852,     9,    12,    15,
      16,   253,   254,   272,   273,   859,   863,   171,  1091,     9,
      56,   173,   223,   411,   877,   878,   879,   873,   871,   954,
    1123,  1150,   403,  1084,  1066,  1087,   366,   897,   750,   751,
    1051,   907,   908,  1096,  1075,     8,    35,  1015,  1154,  1093,
     210,   911,   923,  1167,   931,  1130,  1096,   931,   403,   403,
     522,   149,   404,   408,  1087,    49,   218,   961,  1087,  1087,
     372,  1087,  1096,   180,  1066,  1087,  1091,  1132,   210,   992,
    1096,   159,   163,  1004,     9,  1009,  1075,   923,   563,  1098,
     280,   565,  1079,  1114,   565,   193,   699,   233,   234,   705,
     641,  1150,    28,   115,   202,   656,   661,   662,   663,   664,
     666,  1097,  1103,  1105,   459,  1064,  1064,  1064,  1064,  1064,
    1064,   459,  1064,  1087,  1165,  1145,  1150,  1018,  1020,   447,
     446,  1091,  1018,   218,    31,    33,    36,    46,   176,   181,
     194,   242,   290,   314,   338,   343,   353,   357,   415,   425,
     762,   763,  1018,   270,  1148,  1148,  1148,   760,   759,   236,
    1090,  1097,   459,  1096,   463,   459,  1063,   459,   459,  1063,
     459,   459,   459,   459,  1063,   459,   459,   373,  1024,  1025,
    1066,  1085,   341,  1165,   398,  1162,  1162,   403,  1075,   781,
     782,   783,  1132,  1096,  1096,   163,   289,   785,  1005,  1138,
     240,   260,  1024,  1047,  1049,   131,   807,  1048,    96,   305,
     442,  1073,    33,    36,    44,    45,    46,   158,   176,   194,
     242,   290,   343,   353,   415,   814,   815,  1019,   269,  1021,
     265,  1022,   187,  1024,   187,  1126,   400,   836,   832,   834,
     750,  1150,   750,  1165,   331,   860,  1165,   403,    49,   878,
     880,  1091,     9,    56,   223,   411,   874,   875,  1091,   955,
    1124,   200,   285,  1151,   664,  1084,  1024,   187,  1167,  1070,
     138,   903,   752,     8,   180,   911,  1096,   126,   263,  1034,
    1035,  1037,  1044,   240,   260,   438,   126,   438,   933,   934,
    1091,  1090,  1087,  1141,  1044,   972,  1167,  1096,  1024,   187,
     403,     9,   990,   991,  1113,   993,  1096,   972,   993,   307,
    1007,   308,  1014,  1015,  1114,   251,   319,   321,   573,  1141,
     173,   700,  1101,   708,  1079,  1132,   665,   666,   662,  1143,
     660,   661,   459,   404,   679,  1079,  1022,  1018,  1141,  1141,
     121,   424,   763,  1093,  1093,  1093,  1106,  1119,   459,  1064,
    1079,  1106,  1106,  1064,  1106,  1106,  1106,   223,   411,  1106,
    1106,  1026,   268,  1027,  1024,  1085,   156,   284,   156,   284,
     782,   279,   738,    86,   325,   435,   265,   267,   787,  1006,
     786,   329,   344,   750,   750,   813,   813,   813,   813,  1141,
     153,   155,  1141,   121,   424,   815,   750,  1023,  1066,  1067,
    1066,  1067,   835,  1052,   750,  1096,   125,   853,   435,   861,
     862,   863,   110,   864,   435,  1092,  1096,  1102,  1091,    49,
     882,   875,   175,   882,   951,  1141,   285,  1143,  1066,   582,
     898,  1167,   753,   908,  1087,   199,   912,  1167,  1036,  1038,
     141,   920,  1037,   142,   924,   240,  1052,   932,  1051,   933,
     262,   962,  1117,   144,   963,   289,  1029,  1030,   300,  1119,
    1066,  1092,   284,  1091,   113,   994,   394,   996,  1150,   154,
     264,  1016,  1039,  1040,  1042,  1045,     7,  1125,   573,  1101,
     116,   220,   701,    67,    66,    67,   192,   233,   234,   258,
     303,   376,   390,   414,   457,   652,   653,   655,   657,   669,
     672,   674,   682,   709,   710,   712,   713,   714,   715,   717,
     718,   719,   723,   724,   442,   667,   668,  1141,  1141,   448,
    1095,  1096,  1101,  1095,  1050,  1132,  1050,  1024,   459,   750,
    1028,  1165,   156,  1165,   156,  1087,   129,   789,   788,   750,
    1019,  1019,  1019,  1019,  1095,  1095,  1050,  1050,   750,  1024,
     328,  1024,   328,   854,   136,   855,   862,   102,  1136,   882,
     882,  1092,  1015,   207,   434,   956,  1079,  1141,  1024,   240,
     260,    49,   240,   218,   913,   198,   240,   260,   437,   750,
     750,   929,   750,   935,   697,  1057,  1058,  1059,  1060,  1061,
    1031,   145,   973,   267,  1032,  1096,  1024,  1024,   991,  1140,
      95,   995,  1140,  1029,   166,   207,   215,   288,  1001,  1070,
    1041,  1043,   148,  1017,  1042,   293,  1073,  1095,  1141,    91,
     221,   702,   271,  1148,   203,   725,   270,   271,   722,  1127,
     192,   438,  1141,  1149,  1141,   715,   258,   299,   720,   721,
    1101,   668,  1078,  1102,  1095,   750,  1165,  1165,   750,  1067,
    1067,   750,    49,   882,   406,   883,   307,  1070,   187,   288,
     957,   953,   344,  1087,  1141,   914,  1034,  1045,   240,   240,
     750,   750,   750,  1033,  1096,  1140,  1096,   147,   997,   750,
     750,   233,   234,  1144,  1101,  1141,  1141,   174,   703,  1141,
    1142,  1141,  1051,  1096,   716,  1079,    90,    91,   116,   294,
     295,   335,   336,   711,   293,  1101,   721,  1144,  1024,  1024,
    1087,  1087,  1141,  1070,   307,  1098,   423,   697,   140,   915,
     750,  1096,  1101,  1101,  1141,  1101,  1101,  1119,   904,  1141,
    1051,  1101,   904
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   465,   467,   466,   468,   468,   469,   469,   471,   472,
     470,   474,   475,   473,   477,   478,   476,   479,   479,   479,
     480,   480,   481,   482,   484,   483,   485,   486,   486,   487,
     487,   488,   488,   489,   489,   489,   490,   490,   491,   491,
     492,   492,   493,   493,   494,   494,   494,   494,   495,   496,
     496,   496,   496,   497,   498,   499,   500,   500,   500,   500,
     501,   501,   502,   502,   502,   503,   504,   504,   505,   506,
     507,   508,   508,   509,   509,   510,   510,   511,   511,   512,
     513,   513,   513,   514,   514,   515,   515,   515,   515,   515,
     515,   515,   515,   515,   515,   515,   516,   517,   516,   518,
     516,   516,   516,   516,   516,   519,   519,   520,   520,   521,
     522,   522,   524,   523,   525,   525,   525,   525,   525,   526,
     526,   527,   527,   528,   527,   529,   529,   530,   530,   530,
     530,   530,   530,   531,   531,   531,   531,   531,   531,   532,
     533,   534,   534,   535,   535,   536,   537,   537,   538,   538,
     539,   540,   541,   542,   543,   544,   545,   546,   546,   547,
     546,   548,   546,   549,   549,   550,   550,   552,   551,   553,
     553,   554,   554,   554,   554,   554,   554,   554,   554,   554,
     554,   554,   554,   554,   554,   554,   555,   555,   555,   556,
     556,   556,   557,   557,   557,   558,   558,   558,   559,   559,
     560,   561,   561,   561,   562,   562,   564,   563,   563,   565,
     566,   566,   566,   567,   568,   569,   569,   569,   570,   571,
     571,   571,   572,   572,   572,   572,   573,   573,   574,   574,
     575,   575,   575,   575,   575,   576,   577,   578,   578,   579,
     580,   580,   581,   582,   582,   582,   583,   584,   584,   585,
     585,   585,   585,   586,   586,   587,   587,   588,   589,   589,
     589,   589,   590,   591,   591,   592,   593,   593,   594,   594,
     594,   595,   595,   595,   595,   595,   596,   596,   597,   598,
     597,   599,   597,   600,   600,   601,   602,   602,   603,   603,
     605,   604,   606,   606,   607,   607,   607,   607,   607,   607,
     607,   607,   607,   607,   607,   607,   608,   609,   609,   609,
     610,   610,   610,   611,   611,   612,   612,   613,   613,   614,
     615,   615,   616,   616,   617,   617,   618,   619,   620,   620,
     621,   621,   621,   622,   623,   624,   625,   626,   627,   627,
     628,   629,   628,   630,   630,   632,   631,   633,   633,   633,
     633,   633,   634,   634,   635,   636,   635,   637,   635,   638,
     639,   640,   640,   641,   641,   641,   642,   643,   643,   644,
     644,   644,   645,   646,   646,   647,   647,   647,   647,   647,
     647,   647,   647,   647,   647,   647,   647,   647,   647,   647,
     647,   648,   649,   650,   650,   651,   652,   653,   653,   654,
     654,   654,   654,   654,   654,   654,   654,   654,   654,   654,
     654,   654,   654,   654,   654,   654,   654,   654,   654,   654,
     654,   654,   654,   654,   654,   654,   654,   654,   654,   654,
     654,   654,   654,   654,   655,   655,   656,   656,   656,   657,
     658,   658,   659,   659,   660,   660,   661,   662,   663,   663,
     664,   664,   665,   665,   666,   667,   667,   668,   669,   670,
     671,   671,   671,   672,   673,   674,   676,   675,   677,   677,
     678,   678,   679,   679,   680,   680,   681,   682,   683,   684,
     683,   685,   686,   685,   687,   688,   687,   689,   689,   690,
     690,   691,   692,   692,   693,   693,   693,   693,   694,   694,
     695,   696,   696,   697,   697,   698,   699,   699,   700,   700,
     701,   701,   702,   702,   703,   703,   704,   704,   704,   705,
     705,   706,   706,   707,   708,   708,   709,   709,   709,   709,
     709,   709,   709,   709,   709,   709,   709,   709,   709,   709,
     709,   709,   710,   711,   711,   711,   711,   711,   711,   711,
     712,   713,   713,   713,   714,   714,   715,   716,   717,   682,
     718,   719,   719,   720,   720,   721,   721,   721,   722,   722,
     722,   723,   724,   725,   725,   726,   727,   728,   727,   729,
     730,   731,   729,   732,   733,   732,   734,   732,   735,   735,
     736,   737,   737,   737,   738,   738,   738,   738,   738,   739,
     739,   740,   740,   741,   742,   741,   743,   743,   744,   744,
     744,   744,   744,   745,   746,   747,   748,   749,   749,   751,
     752,   750,   753,   753,   754,   754,   754,   754,   754,   754,
     754,   754,   754,   754,   754,   754,   754,   754,   754,   754,
     754,   754,   754,   754,   754,   754,   754,   754,   754,   754,
     754,   754,   754,   754,   754,   754,   754,   754,   754,   754,
     754,   754,   754,   754,   754,   754,   754,   754,   754,   754,
     754,   754,   754,   754,   754,   754,   756,   755,   757,   757,
     757,   757,   757,   757,   757,   757,   757,   757,   757,   757,
     757,   757,   757,   757,   757,   758,   758,   758,   758,   758,
     758,   759,   760,   760,   761,   761,   762,   762,   763,   763,
     763,   763,   763,   763,   763,   763,   763,   763,   763,   763,
     763,   763,   763,   763,   763,   764,   764,   766,   765,   767,
     767,   767,   768,   768,   769,   769,   771,   770,   772,   772,
     773,   773,   774,   775,   775,   776,   776,   778,   777,   779,
     780,   779,   781,   781,   782,   782,   783,   783,   783,   783,
     784,   784,   784,   785,   786,   785,   787,   788,   787,   789,
     789,   791,   790,   792,   792,   792,   794,   793,   795,   795,
     796,   796,   796,   796,   796,   797,   797,   799,   798,   800,
     801,   801,   802,   802,   803,   804,   806,   805,   807,   807,
     809,   808,   811,   810,   812,   812,   812,   812,   812,   812,
     812,   812,   812,   813,   813,   813,   814,   814,   815,   815,
     815,   815,   815,   815,   815,   815,   815,   815,   815,   815,
     815,   815,   815,   816,   816,   818,   817,   819,   819,   819,
     819,   819,   820,   820,   822,   821,   824,   823,   825,   825,
     826,   826,   826,   827,   828,   828,   830,   829,   831,   832,
     831,   833,   833,   834,   834,   835,   835,   835,   835,   836,
     836,   837,   837,   839,   838,   840,   840,   840,   840,   840,
     840,   842,   841,   844,   843,   846,   845,   847,   847,   849,
     848,   851,   852,   850,   850,   853,   854,   853,   855,   855,
     857,   856,   858,   858,   859,   859,   859,   860,   860,   861,
     861,   862,   863,   863,   863,   863,   863,   863,   863,   864,
     864,   866,   865,   868,   867,   869,   869,   869,   870,   870,
     871,   871,   871,   873,   872,   874,   874,   875,   875,   875,
     875,   875,   875,   876,   877,   877,   878,   878,   879,   879,
     879,   879,   879,   880,   881,   882,   882,   883,   883,   885,
     884,   887,   886,   888,   888,   890,   889,   891,   891,   892,
     892,   894,   893,   895,   895,   896,   896,   896,   896,   897,
     897,   898,   898,   898,   900,   899,   901,   902,   901,   901,
     903,   903,   904,   904,   905,   905,   905,   905,   905,   906,
     906,   907,   907,   908,   910,   909,   911,   911,   912,   912,
     912,   912,   912,   912,   913,   913,   914,   914,   914,   915,
     915,   917,   916,   919,   918,   920,   920,   922,   921,   923,
     923,   923,   924,   924,   925,   927,   926,   928,   929,   928,
     930,   930,   931,   932,   931,   933,   933,   935,   934,   936,
     936,   938,   937,   939,   939,   939,   939,   939,   940,   941,
     941,   942,   943,   943,   944,   944,   945,   946,   946,   947,
     947,   949,   948,   951,   950,   952,   952,   953,   953,   954,
     954,   955,   955,   956,   956,   956,   957,   957,   957,   959,
     960,   958,   961,   961,   962,   962,   962,   962,   962,   963,
     963,   965,   964,   966,   964,   967,   967,   967,   969,   968,
     970,   970,   971,   971,   971,   972,   972,   973,   973,   975,
     974,   976,   976,   976,   977,   977,   978,   979,   979,   981,
     980,   983,   982,   985,   984,   986,   986,   986,   988,   987,
     989,   989,   990,   990,   991,   992,   992,   993,   994,   994,
     995,   995,   996,   996,   997,   997,   998,   998,   998,   999,
    1000,  1000,  1001,  1001,  1001,  1001,  1001,  1002,  1002,  1003,
    1003,  1004,  1004,  1005,  1005,  1006,  1006,  1007,  1007,  1008,
    1009,  1009,  1010,  1012,  1011,  1013,  1013,  1014,  1014,  1014,
    1014,  1015,  1015,  1016,  1016,  1016,  1017,  1017,  1018,  1019,
    1020,  1021,  1020,  1022,  1023,  1022,  1024,  1025,  1026,  1025,
    1027,  1028,  1027,  1029,  1030,  1031,  1030,  1032,  1033,  1032,
    1034,  1034,  1034,  1036,  1035,  1038,  1037,  1039,  1039,  1039,
    1041,  1040,  1043,  1042,  1044,  1044,  1045,  1045,  1045,  1047,
    1046,  1049,  1048,  1050,  1050,  1051,  1052,  1054,  1053,  1055,
    1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,
    1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,  1056,  1056,
    1056,  1056,  1056,  1056,  1056,  1056,  1056,  1056,  1056,  1056,
    1056,  1056,  1056,  1056,  1056,  1056,  1056,  1056,  1056,  1056,
    1056,  1056,  1056,  1056,  1056,  1056,  1057,  1057,  1057,  1058,
    1058,  1059,  1059,  1060,  1060,  1060,  1061,  1061,  1061,  1062,
    1062,  1063,  1063,  1063,  1064,  1064,  1064,  1064,  1064,  1064,
    1064,  1064,  1064,  1065,  1065,  1066,  1066,  1067,  1068,  1069,
    1070,  1070,  1071,  1072,  1072,  1073,  1074,  1074,  1075,  1076,
    1076,  1076,  1077,  1078,  1078,  1079,  1080,  1080,  1081,  1081,
    1082,  1082,  1083,  1084,  1084,  1085,  1085,  1086,  1086,  1087,
    1087,  1087,  1087,  1087,  1087,  1087,  1087,  1087,  1088,  1088,
    1088,  1088,  1088,  1088,  1088,  1089,  1089,  1090,  1090,  1091,
    1091,  1092,  1092,  1093,  1093,  1094,  1094,  1094,  1095,  1095,
    1095,  1096,  1097,  1097,  1097,  1097,  1098,  1098,  1099,  1100,
    1100,  1101,  1102,  1102,  1103,  1103,  1104,  1104,  1104,  1104,
    1104,  1104,  1104,  1105,  1105,  1105,  1105,  1105,  1105,  1105,
    1105,  1105,  1105,  1105,  1105,  1106,  1106,  1106,  1107,  1107,
    1108,  1108,  1109,  1109,  1109,  1110,  1110,  1111,  1111,  1112,
    1113,  1113,  1114,  1114,  1115,  1115,  1116,  1116,  1116,  1117,
    1117,  1118,  1118,  1119,  1119,  1120,  1120,  1121,  1121,  1122,
    1122,  1123,  1123,  1124,  1124,  1125,  1125,  1126,  1126,  1127,
    1127,  1128,  1128,  1129,  1129,  1130,  1130,  1131,  1131,  1132,
    1132,  1133,  1133,  1134,  1134,  1135,  1135,  1136,  1136,  1137,
    1137,  1138,  1138,  1139,  1139,  1140,  1140,  1141,  1141,  1142,
    1142,  1142,  1143,  1143,  1144,  1144,  1144,  1145,  1145,  1146,
    1146,  1147,  1147,  1148,  1148,  1149,  1149,  1150,  1150,  1151,
    1151,  1151,  1152,  1152,  1153,  1153,  1154,  1154,  1155,  1155,
    1156,  1156,  1157,  1157,  1158,  1158,  1158,  1159,  1159,  1160,
    1160,  1161,  1161,  1162,  1162,  1163,  1163,  1164,  1164,  1165,
    1165,  1166,  1166,  1167,  1167,  1168,  1169
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     1,     2,     1,     1,     0,     0,
       8,     0,     0,     8,     0,     0,     7,     0,     1,     2,
       0,     3,     3,     3,     0,     7,     5,     1,     1,     0,
       2,     0,     3,     1,     2,     1,     1,     1,     0,     5,
       0,     4,     0,     2,     1,     1,     1,     1,     3,     0,
       2,     3,     2,     3,     1,     3,     0,     2,     3,     2,
       1,     2,     1,     1,     1,     5,     1,     1,     4,     3,
       3,     0,     2,     1,     2,     3,     3,     1,     2,     3,
       0,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     5,     0,
       4,     3,     3,     3,     3,     0,     2,     1,     2,     4,
       1,     1,     0,     5,     1,     1,     1,     1,     1,     1,
       2,     1,     3,     0,     4,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     1,     2,     1,     2,     4,     1,     2,     1,     3,
       4,     4,     3,     3,     4,     3,     3,     0,     5,     0,
       4,     0,     4,     0,     3,     0,     2,     0,     6,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     5,     4,     4,     0,
       1,     1,     0,     1,     1,     1,     1,     2,     1,     2,
       4,     1,     1,     1,     6,     8,     0,     2,     2,     1,
       0,     2,     1,     3,     5,     0,     1,     1,     4,     2,
       2,     1,     0,     4,     5,     2,     1,     1,     3,     1,
       1,     3,     1,     1,     2,     4,     4,     5,     7,     4,
       3,     2,     3,     2,     2,     2,     4,     0,     3,     0,
       2,     1,     1,     1,     2,     1,     1,     5,     0,     1,
       1,     1,     5,     1,     2,     2,     0,     2,     2,     1,
       2,     4,     7,     6,     6,     4,     0,     9,     0,     0,
       5,     0,     3,     0,     2,     3,     2,     2,     1,     1,
       0,     4,     0,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     5,     0,     1,     1,
       4,     6,     9,     0,     3,     0,     2,     0,     2,     3,
       1,     1,     5,     5,     1,     1,     3,     5,     0,     2,
       1,     1,     1,     5,     4,     3,     4,     3,     3,     3,
       0,     0,     5,     0,     1,     0,     2,     2,     3,     2,
       2,     1,     1,     2,     1,     0,     5,     0,     4,     1,
       1,     0,     1,     0,     1,     1,     1,     0,     2,     1,
       3,     3,     6,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     3,     0,     2,     2,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     1,     3,     3,     0,     2,     2,     6,
       0,     2,     0,     3,     0,     1,     1,     4,     1,     2,
       1,     1,     0,     1,     3,     1,     2,     1,     2,     2,
       0,     1,     1,     3,     1,     3,     0,     8,     1,     2,
       1,     3,     0,     3,     2,     4,     2,     1,     0,     0,
       5,     0,     0,     5,     0,     0,     5,     0,     1,     1,
       2,     5,     0,     2,     2,     3,     1,     1,     2,     2,
       2,     0,     1,     1,     2,     8,     0,     3,     0,     4,
       0,     4,     0,     3,     0,     3,     1,     4,     2,     1,
       1,     0,     2,     4,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     2,     1,     1,     2,     3,     1,     3,     6,
       2,     3,     2,     1,     2,     2,     1,     2,     0,     1,
       1,     4,     2,     0,     1,     1,     0,     0,     4,     0,
       0,     0,     9,     0,     0,     3,     0,     3,     1,     2,
       4,     0,     2,     2,     0,     3,     3,     4,     3,     0,
       1,     0,     2,     0,     0,     7,     0,     2,     1,     1,
       1,     2,     1,     4,     2,     1,     1,     0,     1,     0,
       0,     3,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     4,     4,     4,
       3,     3,     3,     4,     3,     4,     3,     3,     3,     4,
       5,     3,     4,     3,     3,     0,     3,     3,     2,     2,
       2,     3,     3,     3,     0,     2,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     0,     1,     0,     4,     4,
       5,     6,     0,     2,     0,     1,     0,     3,     3,     5,
       0,     2,     2,     0,     5,     0,     2,     0,     8,     0,
       0,     3,     1,     2,     2,     3,     0,     2,     2,     2,
       0,     2,     2,     0,     0,     3,     0,     0,     3,     0,
       1,     0,     3,     0,     2,     1,     0,     3,     0,     3,
       0,     1,     3,     3,     2,     1,     1,     0,     4,     4,
       0,     1,     1,     1,     1,     1,     0,     6,     0,     1,
       0,     4,     0,     4,     3,     3,     3,     3,     4,     6,
       6,     6,     6,     0,     2,     2,     1,     2,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     1,     1,     0,     1,     0,     4,     4,     6,     6,
       8,     8,     0,     1,     0,     4,     0,     5,     1,     3,
       1,     1,     1,     2,     1,     2,     0,     3,     0,     0,
       3,     2,     3,     1,     3,     2,     1,     1,     1,     0,
       2,     0,     1,     0,     3,     0,     1,     1,     2,     1,
       1,     0,     3,     0,     3,     0,     5,     0,     3,     0,
       2,     0,     0,     8,     3,     0,     0,     3,     0,     1,
       0,     7,     0,     2,     0,     3,     3,     0,     2,     1,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     0,     3,     0,     4,     1,     1,     1,     1,     2,
       1,     1,     1,     0,     3,     1,     2,     2,     2,     1,
       1,     1,     2,     2,     1,     2,     4,     2,     0,     1,
       1,     1,     1,     4,     5,     0,     4,     0,     1,     0,
       3,     0,     3,     3,     4,     0,     4,     4,     6,     0,
       1,     0,     3,     0,     5,     1,     1,     1,     1,     0,
       3,     0,     3,     2,     0,     3,     2,     0,     4,     2,
       0,     1,     1,     3,     0,     1,     2,     3,     3,     0,
       3,     1,     3,     7,     0,    10,     0,     2,     0,     2,
       2,     3,     3,     2,     0,     3,     0,     1,     1,     0,
       1,     0,     4,     0,     7,     0,     1,     0,     7,     0,
       2,     3,     0,     1,     1,     0,     4,     4,     0,     7,
       0,     2,     0,     0,     4,     1,     2,     0,     4,     0,
       1,     0,     3,     1,     1,     1,     1,     1,     4,     4,
       3,     4,     1,     1,     1,     2,     3,     1,     2,     3,
       3,     0,     3,     0,     7,     0,     6,     0,     2,     0,
       2,     0,     3,     0,     2,     4,     0,     2,     4,     0,
       0,     7,     0,     4,     2,     2,     2,     2,     2,     0,
       1,     0,     4,     0,     3,     0,     2,     2,     0,     8,
       1,     2,     1,     3,     3,     0,     3,     0,     1,     0,
       4,     4,     6,     6,     0,     1,     2,     0,     1,     0,
       3,     0,     7,     0,     4,     0,     1,     1,     0,     9,
       0,     3,     1,     3,     2,     2,     2,     3,     0,     3,
       0,     3,     0,     3,     0,     1,     1,     1,     1,     8,
       0,     1,     1,     1,     1,     1,     1,     0,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     5,
       1,     2,     5,     0,     8,     0,     2,     0,     4,     3,
       3,     1,     1,     0,     1,     1,     0,     1,     2,     2,
       0,     0,     3,     0,     0,     3,     2,     0,     0,     3,
       0,     0,     3,     2,     0,     0,     3,     0,     0,     3,
       1,     1,     2,     0,     3,     0,     3,     1,     1,     2,
       0,     3,     0,     3,     0,     1,     1,     1,     2,     0,
       3,     0,     3,     0,     3,     1,     1,     0,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     1,     1,
       2,     1,     2,     1,     5,     4,     1,     5,     4,     1,
       3,     0,     1,     1,     1,     3,     3,     3,     3,     2,
       2,     3,     3,     1,     3,     1,     2,     2,     1,     1,
       1,     2,     1,     1,     2,     1,     0,     2,     1,     1,
       1,     3,     1,     1,     2,     1,     1,     2,     0,     1,
       1,     1,     1,     1,     2,     1,     3,     1,     2,     1,
       3,     3,     3,     4,     3,     1,     1,     1,     1,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     1,     3,     3,     4,
       5,     1,     1,     2,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     5,     5,     5,     5,     5,
       5,     5,     4,     5,     2,     0,     4,     5,     0,     3,
       0,     1,     1,     3,     3,     1,     3,     1,     3,     0,
       0,     1,     0,     1,     0,     1,     0,     1,     1,     0,
       1,     0,     1,     0,     1,     0,     2,     1,     1,     2,
       2,     2,     1,     2,     1,     0,     1,     0,     1,     0,
       1,     0,     1,     0,     1,     0,     1,     0,     1,     0,
       1,     0,     1,     0,     1,     0,     1,     0,     1,     0,
       1,     0,     1,     0,     1,     0,     1,     0,     1,     0,
       1,     1,     0,     1,     0,     1,     1,     0,     1,     0,
       1,     0,     1,     0,     1,     0,     1,     0,     1,     0,
       1,     2,     0,     1,     0,     1,     0,     1,     0,     1,
       0,     1,     0,     1,     0,     1,     2,     0,     1,     0,
       1,     0,     1,     0,     1,     0,     1,     0,     1,     0,
       1,     0,     1,     0,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= TOKEN_EOF)
    {
      yychar = TOKEN_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 776 "parser.y"
  {
	perform_stack = NULL;
	current_statement = NULL;
	next_label_id = 0;
	current_linage = 0;
	current_storage = 0;
	eval_level = 0;
	eval_inc = 0;
	eval_inc2 = 0;
	prog_end = 0;
	depth = 0;
	inspect_keyword = 0;
	check_unreached = 0;
	samearea = 1;
	memset ((char *)eval_check, 0, sizeof(eval_check));
	memset ((char *)term_array, 0, sizeof(term_array));
	linage_file = NULL;
	next_label_list = NULL;
	current_program = cb_build_program (NULL, 0);
	cb_build_registers ();
	current_program->flag_main = cb_flag_main;
  }
#line 5224 "parser.c"
    break;

  case 3: /* start: $@1 nested_list "end of file"  */
#line 800 "parser.y"
  {
	current_section = NULL;
	current_paragraph = NULL;
	if (!current_program->flag_validated) {
		current_program->flag_validated = 1;
		cb_validate_program_body (current_program);
	}
	if (depth > 1) {
		cb_error (_("Multiple PROGRAM-ID's without matching END PROGRAM"));
	}
	if (errorcount > 0) {
		YYABORT;
	}
	if (!current_program->entry_list) {
		emit_entry (current_program->program_id, 0, NULL);
	}
  }
#line 5246 "parser.c"
    break;

  case 8: /* $@2: %empty  */
#line 831 "parser.y"
                        { cb_validate_program_environment (current_program); }
#line 5252 "parser.c"
    break;

  case 9: /* $@3: %empty  */
#line 832 "parser.y"
                        { cb_validate_program_data (current_program); }
#line 5258 "parser.c"
    break;

  case 10: /* program_definition: identification_division environment_division $@2 data_division $@3 procedure_division nested_prog end_program  */
#line 835 "parser.y"
              {
	cb_tree file = current_program->file_list;
	for(; file; file = CB_CHAIN(file)) {
		cb_validate_indexed_file_key(CB_FILE(CB_VALUE(file)));
	}
  }
#line 5269 "parser.c"
    break;

  case 11: /* $@4: %empty  */
#line 845 "parser.y"
                        { cb_validate_program_environment (current_program); }
#line 5275 "parser.c"
    break;

  case 12: /* $@5: %empty  */
#line 846 "parser.y"
                        { cb_validate_program_data (current_program); }
#line 5281 "parser.c"
    break;

  case 13: /* program_mandatory: identification_division environment_division $@4 data_division $@5 procedure_division nested_prog end_mandatory  */
#line 849 "parser.y"
                {
	cb_tree file = current_program->file_list;
	for(; file; file = CB_CHAIN(file)) {
		cb_validate_indexed_file_key(CB_FILE(CB_VALUE(file)));
	}
  }
#line 5292 "parser.c"
    break;

  case 14: /* $@6: %empty  */
#line 859 "parser.y"
                        { cb_validate_program_environment (current_program); }
#line 5298 "parser.c"
    break;

  case 15: /* $@7: %empty  */
#line 860 "parser.y"
                        { cb_validate_program_data (current_program); }
#line 5304 "parser.c"
    break;

  case 21: /* end_program: "END PROGRAM" program_name '.'  */
#line 872 "parser.y"
  {
	char			*s;

	current_section = NULL;
	current_paragraph = NULL;
	if (CB_LITERAL_P (yyvsp[-1])) {
		s = (char *)(CB_LITERAL (yyvsp[-1])->data);
	} else {
		s = (char *)(CB_NAME (yyvsp[-1]));
	}
	if (depth) {
		depth--;
	}
	if (cb_allow_end_program_with_wrong_name) {
		/* ignore wrong program-id. */
	} else if (strcmp (stack_progid[depth], s)) {
		cb_error (_("END PROGRAM '%s' is different to PROGRAM-ID '%s'"),
			s, stack_progid[depth]);
	}
	if (!current_program->flag_validated) {
		current_program->flag_validated = 1;
		cb_validate_program_body (current_program);
	}
  }
#line 5333 "parser.c"
    break;

  case 22: /* end_mandatory: "END PROGRAM" program_name '.'  */
#line 900 "parser.y"
  {
	char			*s;

	current_section = NULL;
	current_paragraph = NULL;
	if (CB_LITERAL_P (yyvsp[-1])) {
		s = (char *)(CB_LITERAL (yyvsp[-1])->data);
	} else {
		s = (char *)(CB_NAME (yyvsp[-1]));
	}
	if (depth) {
		depth--;
	}
	if (strcmp (stack_progid[depth], s)) {
		cb_error (_("END PROGRAM '%s' is different to PROGRAM-ID '%s'"),
			s, stack_progid[depth]);
	}
	if (!current_program->flag_validated) {
		current_program->flag_validated = 1;
		cb_validate_program_body (current_program);
	}
  }
#line 5360 "parser.c"
    break;

  case 23: /* end_function: "END FUNCTION" program_name '.'  */
#line 926 "parser.y"
  {
	char			*s;

	current_section = NULL;
	current_paragraph = NULL;
	if (CB_LITERAL_P (yyvsp[-1])) {
		s = (char *)(CB_LITERAL (yyvsp[-1])->data);
	} else {
		s = (char *)(CB_NAME (yyvsp[-1]));
	}
	if (depth) {
		depth--;
	}
	if (strcmp (stack_progid[depth], s)) {
		cb_error (_("END FUNCTION '%s' is different to FUNCTION-ID '%s'"),
			s, stack_progid[depth]);
	}
	if (!current_program->flag_validated) {
		current_program->flag_validated = 1;
		cb_validate_program_body (current_program);
	}
  }
#line 5387 "parser.c"
    break;

  case 24: /* $@8: %empty  */
#line 957 "parser.y"
  {
	current_section = NULL;
	current_paragraph = NULL;
	/* SQLCA 注入フラグはプログラム単位でリセットする。 */
	esql_sqlca_injected = 0;
	if (CB_LITERAL_P (yyvsp[-1])) {
		stack_progid[depth] = (char *)(CB_LITERAL (yyvsp[-1])->data);
	} else {
		stack_progid[depth] = (char *)(CB_NAME (yyvsp[-1]));
	}
	if(strcmp("MAIN", stack_progid[depth]) == 0) {
		cb_error (_("PROGRAM-ID should not be MAIN"));
	}
	if (prog_end) {
		if (!current_program->flag_validated) {
			current_program->flag_validated = 1;
			cb_validate_program_body (current_program);
		}
		perform_stack = NULL;
		current_statement = NULL;
		next_label_id = 0;
		current_linage = 0;
		current_storage = 0;
		eval_level = 0;
		inspect_keyword = 0;
		check_unreached = 0;
		eval_inc = 0;
		eval_inc2 = 0;
		samearea = 1;
		memset ((char *)eval_check, 0, sizeof(eval_check));
		memset ((char *)term_array, 0, sizeof(term_array));
		linage_file = NULL;
		next_label_list = NULL;
		current_program = cb_build_program (current_program, depth);
		cb_build_registers ();
	} else {
		prog_end = 1;
	}
	depth++;
	current_program->program_id = cb_build_program_id (yyvsp[-1], yyvsp[0]);
  }
#line 5433 "parser.c"
    break;

  case 26: /* function_division: "FUNCTION-ID" '.' program_name as_literal '.'  */
#line 1003 "parser.y"
  {
	cb_error (_("FUNCTION-ID is not yet implemented"));
	current_section = NULL;
	current_paragraph = NULL;
	if (CB_LITERAL_P (yyvsp[-2])) {
		stack_progid[depth] = (char *)(CB_LITERAL (yyvsp[-2])->data);
	} else {
		stack_progid[depth] = (char *)(CB_NAME (yyvsp[-2]));
	}
	if (prog_end) {
		if (!current_program->flag_validated) {
			current_program->flag_validated = 1;
			cb_validate_program_body (current_program);
		}
		perform_stack = NULL;
		current_statement = NULL;
		next_label_id = 0;
		current_linage = 0;
		current_storage = 0;
		eval_level = 0;
		inspect_keyword = 0;
		check_unreached = 0;
		eval_inc = 0;
		eval_inc2 = 0;
		samearea = 1;
		memset ((char *)eval_check, 0, sizeof(eval_check));
		memset ((char *)term_array, 0, sizeof(term_array));
		linage_file = NULL;
		next_label_list = NULL;
		current_program = cb_build_program (current_program, depth);
		cb_build_registers ();
	} else {
		prog_end = 1;
	}
	depth++;
	current_program->program_id = cb_build_program_id (yyvsp[-2], yyvsp[-1]);
	current_program->prog_type = CB_FUNCTION_TYPE;
	current_program->flag_recursive = 1;
	current_program->flag_initial = 1;
  }
#line 5478 "parser.c"
    break;

  case 29: /* as_literal: %empty  */
#line 1051 "parser.y"
                                { yyval = NULL; }
#line 5484 "parser.c"
    break;

  case 30: /* as_literal: AS "Literal"  */
#line 1052 "parser.y"
                                { yyval = yyvsp[0]; }
#line 5490 "parser.c"
    break;

  case 33: /* program_type_clause: COMMON  */
#line 1061 "parser.y"
  {
	if (!current_program->nested_level) {
		cb_error (_("COMMON may only be used in a nested program"));
	}
	current_program->flag_common = 1;
  }
#line 5501 "parser.c"
    break;

  case 34: /* program_type_clause: COMMON _init_or_recurs  */
#line 1068 "parser.y"
  {
	if (!current_program->nested_level) {
		cb_error (_("COMMON may only be used in a nested program"));
	}
	current_program->flag_common = 1;
  }
#line 5512 "parser.c"
    break;

  case 36: /* _init_or_recurs: "INITIAL"  */
#line 1079 "parser.y"
  {
	current_program->flag_initial = 1;
  }
#line 5520 "parser.c"
    break;

  case 37: /* _init_or_recurs: RECURSIVE  */
#line 1083 "parser.y"
  {
	current_program->flag_recursive = 1;
	current_program->flag_initial = 1;
  }
#line 5529 "parser.c"
    break;

  case 41: /* configuration_section: CONFIGURATION SECTION '.' configuration_list  */
#line 1108 "parser.y"
  {
	if (current_program->nested_level) {
		cb_error (_("CONFIGURATION SECTION not allowed in nested programs"));
	}
  }
#line 5539 "parser.c"
    break;

  case 53: /* with_debugging_mode: _with DEBUGGING MODE  */
#line 1143 "parser.y"
  {
	cb_verify (cb_debugging_line, "DEBUGGING MODE");
  }
#line 5547 "parser.c"
    break;

  case 54: /* computer_name: "Identifier"  */
#line 1149 "parser.y"
       { }
#line 5553 "parser.c"
    break;

  case 65: /* object_computer_memory: MEMORY SIZE _is integer object_char_or_word  */
#line 1180 "parser.y"
  {
	cb_verify (cb_memory_size_clause, "MEMORY SIZE");
  }
#line 5561 "parser.c"
    break;

  case 68: /* object_computer_sequence: _program coll_sequence _is reference  */
#line 1192 "parser.y"
  {
	current_program->collating_sequence = yyvsp[0];
  }
#line 5569 "parser.c"
    break;

  case 69: /* object_computer_segment: "SEGMENT-LIMIT" _is integer  */
#line 1199 "parser.y"
  {
	/* Ignore */
  }
#line 5577 "parser.c"
    break;

  case 75: /* repository_name: FUNCTION repository_literal_list INTRINSIC  */
#line 1224 "parser.y"
  {
	current_program->function_spec_list = yyvsp[-1];
  }
#line 5585 "parser.c"
    break;

  case 76: /* repository_name: FUNCTION ALL INTRINSIC  */
#line 1228 "parser.y"
  {
	functions_are_all = 1;
  }
#line 5593 "parser.c"
    break;

  case 77: /* repository_literal_list: "Literal"  */
#line 1234 "parser.y"
                        { yyval = cb_list_init (yyvsp[0]); }
#line 5599 "parser.c"
    break;

  case 78: /* repository_literal_list: repository_literal_list "Literal"  */
#line 1236 "parser.y"
                        { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5605 "parser.c"
    break;

  case 96: /* mnemonic_name_clause: "Identifier" _is CRT  */
#line 1277 "parser.y"
  {
	save_tree_1 = lookup_system_name (CB_NAME (yyvsp[-2]));
	if (save_tree_1 == cb_error_node) {
		cb_error_x (yyvsp[-2], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-2]));
	} else if (CB_SYSTEM_NAME(save_tree_1)->token != CB_DEVICE_CONSOLE) {
		cb_error_x (save_tree_1, _("Invalid CRT clause"));
	}
	/* current_program->flag_screen = 1; */
  }
#line 5619 "parser.c"
    break;

  case 97: /* $@9: %empty  */
#line 1287 "parser.y"
  {
	save_tree_1 = lookup_system_name (CB_NAME (yyvsp[-2]));
	if (save_tree_1 == cb_error_node) {
		cb_error_x (yyvsp[-2], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-2]));
	} else {
		cb_define (yyvsp[0], save_tree_1);
	}
	save_tree_2 = yyvsp[0];
  }
#line 5633 "parser.c"
    break;

  case 99: /* $@10: %empty  */
#line 1298 "parser.y"
  {
	save_tree_1 = lookup_system_name (CB_NAME (yyvsp[-1]));
	if (save_tree_1 == cb_error_node) {
		cb_error_x (yyvsp[-1], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-1]));
	}
	save_tree_2 = NULL;
  }
#line 5645 "parser.c"
    break;

  case 101: /* mnemonic_name_clause: "ARGUMENT-NUMBER" _is undefined_word  */
#line 1307 "parser.y"
  {
	if (cb_enable_special_names_argument_clause) {
		save_tree_1 = lookup_system_name ("ARGUMENT-NUMBER");
		if (save_tree_1 == cb_error_node) {
			cb_error_x (yyvsp[-2], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-2]));
		} else {
			cb_define (yyvsp[0], save_tree_1);
		}
		save_tree_2 = yyvsp[0];
	} else {
		cb_error (_("SPECIAL-NAMES with ARGUMENT-NUMBER clause is not yet supported"));
	}
  }
#line 5663 "parser.c"
    break;

  case 102: /* mnemonic_name_clause: "ARGUMENT-VALUE" _is undefined_word  */
#line 1321 "parser.y"
  {
	if (cb_enable_special_names_argument_clause) {
		save_tree_1 = lookup_system_name ("ARGUMENT-VALUE");
		if (save_tree_1 == cb_error_node) {
			cb_error_x (yyvsp[-2], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-2]));
		} else {
			cb_define (yyvsp[0], save_tree_1);
		}
		save_tree_2 = yyvsp[0];
	} else {
		cb_error (_("SPECIAL-NAMES with ARGUMENT-VALUE clause is not yet supported"));
	}
  }
#line 5681 "parser.c"
    break;

  case 103: /* mnemonic_name_clause: "ENVIRONMENT-NAME" _is undefined_word  */
#line 1335 "parser.y"
  {
	if (cb_enable_special_names_environment_clause) {
		save_tree_1 = lookup_system_name ("ENVIRONMENT-NAME");
		if (save_tree_1 == cb_error_node) {
			cb_error_x (yyvsp[-2], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-2]));
		} else {
			cb_define (yyvsp[0], save_tree_1);
		}
		save_tree_2 = yyvsp[0];
	} else {
		cb_error (_("SPECIAL-NAMES with ENVIRONMENT-NAME clause is not yet supported"));
	}
  }
#line 5699 "parser.c"
    break;

  case 104: /* mnemonic_name_clause: "ENVIRONMENT-VALUE" _is undefined_word  */
#line 1349 "parser.y"
  {
	if (cb_enable_special_names_environment_clause) {
		save_tree_1 = lookup_system_name ("ENVIRONMENT-VALUE");
		if (save_tree_1 == cb_error_node) {
			cb_error_x (yyvsp[-2], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-2]));
		} else {
			cb_define (yyvsp[0], save_tree_1);
		}
		save_tree_2 = yyvsp[0];
	} else {
		cb_error (_("SPECIAL-NAMES with ENVIRONMENT-VALUE clause is not yet supported"));
	}
  }
#line 5717 "parser.c"
    break;

  case 109: /* special_name_mnemonic_on_off: on_or_off _status _is undefined_word  */
#line 1375 "parser.y"
  {
	if (!save_tree_2 && !cb_switch_no_mnemonic) {
		cb_error_x (yyvsp[0], _("'%s' with no mnemonic name"), CB_NAME (yyvsp[0]));
	} else {
		cb_define_switch_name (yyvsp[0], save_tree_1, yyvsp[-3], save_tree_2);
	}
  }
#line 5729 "parser.c"
    break;

  case 110: /* on_or_off: ON  */
#line 1385 "parser.y"
                                { yyval = cb_int1; }
#line 5735 "parser.c"
    break;

  case 111: /* on_or_off: OFF  */
#line 1386 "parser.y"
                                { yyval = cb_int0; }
#line 5741 "parser.c"
    break;

  case 112: /* $@11: %empty  */
#line 1394 "parser.y"
  {
	save_tree_1 = yyvsp[0];
  }
#line 5749 "parser.c"
    break;

  case 113: /* alphabet_name_clause: ALPHABET undefined_word $@11 _is alphabet_definition  */
#line 1398 "parser.y"
  {
	current_program->alphabet_name_list =
		cb_list_add (current_program->alphabet_name_list, yyvsp[0]);
  }
#line 5758 "parser.c"
    break;

  case 114: /* alphabet_definition: NATIVE  */
#line 1405 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_NATIVE); }
#line 5764 "parser.c"
    break;

  case 115: /* alphabet_definition: "STANDARD-1"  */
#line 1406 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_STANDARD_1); }
#line 5770 "parser.c"
    break;

  case 116: /* alphabet_definition: "STANDARD-2"  */
#line 1407 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_STANDARD_2); }
#line 5776 "parser.c"
    break;

  case 117: /* alphabet_definition: EBCDIC  */
#line 1408 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_EBCDIC); }
#line 5782 "parser.c"
    break;

  case 118: /* alphabet_definition: alphabet_literal_list  */
#line 1410 "parser.y"
  {
	yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_CUSTOM);
	CB_ALPHABET_NAME (yyval)->custom_list = yyvsp[0];
  }
#line 5791 "parser.c"
    break;

  case 119: /* alphabet_literal_list: alphabet_literal  */
#line 1417 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5797 "parser.c"
    break;

  case 120: /* alphabet_literal_list: alphabet_literal_list alphabet_literal  */
#line 1419 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5803 "parser.c"
    break;

  case 121: /* alphabet_literal: alphabet_lits  */
#line 1423 "parser.y"
                                        { yyval = yyvsp[0]; }
#line 5809 "parser.c"
    break;

  case 122: /* alphabet_literal: alphabet_lits THRU alphabet_lits  */
#line 1424 "parser.y"
                                        { yyval = cb_build_pair (yyvsp[-2], yyvsp[0]); }
#line 5815 "parser.c"
    break;

  case 123: /* @12: %empty  */
#line 1426 "parser.y"
  {
	yyval = cb_list_init (yyvsp[-1]);
	save_tree_2 = yyval;
  }
#line 5824 "parser.c"
    break;

  case 124: /* alphabet_literal: alphabet_lits ALSO @12 alphabet_also_sequence  */
#line 1431 "parser.y"
  {
	yyval = yyvsp[-1];
  }
#line 5832 "parser.c"
    break;

  case 127: /* alphabet_lits: "Literal"  */
#line 1442 "parser.y"
                                { yyval = yyvsp[0]; }
#line 5838 "parser.c"
    break;

  case 128: /* alphabet_lits: SPACE  */
#line 1443 "parser.y"
                                { yyval = cb_space; }
#line 5844 "parser.c"
    break;

  case 129: /* alphabet_lits: ZERO  */
#line 1444 "parser.y"
                                { yyval = cb_zero; }
#line 5850 "parser.c"
    break;

  case 130: /* alphabet_lits: QUOTE  */
#line 1445 "parser.y"
                                { yyval = cb_quote; }
#line 5856 "parser.c"
    break;

  case 131: /* alphabet_lits: "HIGH-VALUE"  */
#line 1446 "parser.y"
                                { yyval = cb_norm_high; }
#line 5862 "parser.c"
    break;

  case 132: /* alphabet_lits: "LOW-VALUE"  */
#line 1447 "parser.y"
                                { yyval = cb_norm_low; }
#line 5868 "parser.c"
    break;

  case 133: /* alphabet_also_literal: "Literal"  */
#line 1451 "parser.y"
                                { cb_list_add (save_tree_2, yyvsp[0]); }
#line 5874 "parser.c"
    break;

  case 134: /* alphabet_also_literal: SPACE  */
#line 1452 "parser.y"
                                { cb_list_add (save_tree_2, cb_space); }
#line 5880 "parser.c"
    break;

  case 135: /* alphabet_also_literal: ZERO  */
#line 1453 "parser.y"
                                { cb_list_add (save_tree_2, cb_zero); }
#line 5886 "parser.c"
    break;

  case 136: /* alphabet_also_literal: QUOTE  */
#line 1454 "parser.y"
                                { cb_list_add (save_tree_2, cb_quote); }
#line 5892 "parser.c"
    break;

  case 137: /* alphabet_also_literal: "HIGH-VALUE"  */
#line 1455 "parser.y"
                                { cb_list_add (save_tree_2, cb_norm_high); }
#line 5898 "parser.c"
    break;

  case 138: /* alphabet_also_literal: "LOW-VALUE"  */
#line 1456 "parser.y"
                                { cb_list_add (save_tree_2, cb_norm_low); }
#line 5904 "parser.c"
    break;

  case 139: /* symbolic_characters_clause: SYMBOLIC _characters symbolic_characters_list  */
#line 1464 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->symbolic_list =
			cb_list_add (current_program->symbolic_list, yyvsp[0]);
	}
	PENDING ("SYMBOLIC CHARACTERS");
  }
#line 5916 "parser.c"
    break;

  case 140: /* symbolic_characters_list: char_list _is_are integer_list  */
#line 1475 "parser.y"
  {
	if (cb_list_length (yyvsp[-2]) != cb_list_length (yyvsp[0])) {
		cb_error (_("Invalid SYMBOLIC clause"));
		yyval = NULL;
	} else {
		yyval = NULL;
	}
  }
#line 5929 "parser.c"
    break;

  case 141: /* char_list: undefined_word  */
#line 1486 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5935 "parser.c"
    break;

  case 142: /* char_list: char_list undefined_word  */
#line 1487 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5941 "parser.c"
    break;

  case 143: /* integer_list: integer  */
#line 1491 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5947 "parser.c"
    break;

  case 144: /* integer_list: integer_list integer  */
#line 1492 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5953 "parser.c"
    break;

  case 145: /* class_name_clause: CLASS undefined_word _is class_item_list  */
#line 1500 "parser.y"
  {
	current_program->class_name_list =
			cb_list_add (current_program->class_name_list,
			cb_build_class_name (yyvsp[-2], yyvsp[0]));
  }
#line 5963 "parser.c"
    break;

  case 146: /* class_item_list: class_item  */
#line 1508 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5969 "parser.c"
    break;

  case 147: /* class_item_list: class_item_list class_item  */
#line 1509 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5975 "parser.c"
    break;

  case 148: /* class_item: basic_value  */
#line 1513 "parser.y"
                                { yyval = yyvsp[0]; }
#line 5981 "parser.c"
    break;

  case 149: /* class_item: basic_value THRU basic_value  */
#line 1515 "parser.y"
  {
	/* if (CB_LITERAL ($1)->data[0] < CB_LITERAL ($3)->data[0]) */
	if (literal_value (yyvsp[-2]) < literal_value (yyvsp[0])) {
		yyval = cb_build_pair (yyvsp[-2], yyvsp[0]);
	} else {
		yyval = cb_build_pair (yyvsp[0], yyvsp[-2]);
	}
  }
#line 5994 "parser.c"
    break;

  case 150: /* locale_clause: LOCALE undefined_word _is reference  */
#line 1529 "parser.y"
  {
	cb_tree	l;

	l = cb_build_locale_name (yyvsp[-2], yyvsp[0]);
	if (l != cb_error_node) {
		current_program->locale_list =
			cb_list_add (current_program->locale_list, l);
	}
  }
#line 6008 "parser.c"
    break;

  case 151: /* currency_sign_clause: CURRENCY _sign _is "Literal"  */
#line 1544 "parser.y"
  {
	unsigned char *s = CB_LITERAL (yyvsp[0])->data;

	if (CB_LITERAL (yyvsp[0])->size != 1) {
		cb_error_x (yyvsp[0], _("Invalid currency sign '%s'"), s);
	}
	switch (*s) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'a':
	case 'A':
	case 'b':
	case 'B':
	case 'c':
	case 'C':
	case 'd':
	case 'D':
	case 'e':
	case 'E':
	case 'n':
	case 'N':
	case 'p':
	case 'P':
	case 'r':
	case 'R':
	case 's':
	case 'S':
	case 'v':
	case 'V':
	case 'x':
	case 'X':
	case 'z':
	case 'Z':
	case '+':
	case '-':
	case ',':
	case '.':
	case '*':
	case '/':
	case ';':
	case '(':
	case ')':
	case '=':
	case '"':
	case ' ':
		cb_error_x (yyvsp[0], _("Invalid currency sign '%s'"), s);
		break;
	default:
		break;
	}
	current_program->currency_symbol = s[0];
  }
#line 6073 "parser.c"
    break;

  case 152: /* decimal_point_clause: "DECIMAL-POINT" _is COMMA  */
#line 1611 "parser.y"
  {
	current_program->decimal_point = ',';
	current_program->numeric_separator = '.';
  }
#line 6082 "parser.c"
    break;

  case 153: /* cursor_clause: CURSOR _is reference  */
#line 1621 "parser.y"
                                { current_program->cursor_pos = yyvsp[0]; }
#line 6088 "parser.c"
    break;

  case 154: /* crt_status_clause: CRT STATUS _is reference  */
#line 1628 "parser.y"
                                { current_program->crt_status = yyvsp[0]; }
#line 6094 "parser.c"
    break;

  case 155: /* screen_control: "SCREEN-CONTROL" _is reference  */
#line 1635 "parser.y"
                                {  PENDING ("SCREEN CONTROL"); }
#line 6100 "parser.c"
    break;

  case 156: /* event_status: "EVENT-STATUS" _is reference  */
#line 1641 "parser.y"
                                {  PENDING ("EVENT STATUS"); }
#line 6106 "parser.c"
    break;

  case 159: /* $@13: %empty  */
#line 1653 "parser.y"
  {
	/* hack for MF compatibility */
	if (cb_relaxed_syntax_check) {
		cb_warning (_("INPUT-OUTPUT SECTION header missing - assumed"));
	} else {
		cb_error (_("INPUT-OUTPUT SECTION header missing"));
	}
  }
#line 6119 "parser.c"
    break;

  case 161: /* $@14: %empty  */
#line 1662 "parser.y"
  {
	/* hack for MF compatibility */
	if (cb_relaxed_syntax_check) {
		cb_warning (_("INPUT-OUTPUT SECTION header missing - assumed"));
	} else {
		cb_error (_("INPUT-OUTPUT SECTION header missing"));
	}
  }
#line 6132 "parser.c"
    break;

  case 167: /* $@15: %empty  */
#line 1687 "parser.y"
  {
	organized_seen = 0;
	if (yyvsp[0] == cb_error_node) {
		YYERROR;
	}

	/* build new file */
	current_file = build_file (yyvsp[0]);
	current_file->optional = CB_INTEGER (yyvsp[-1])->val;

	/* register the file */
	current_program->file_list =
		cb_cons (CB_TREE (current_file), current_program->file_list);
  }
#line 6151 "parser.c"
    break;

  case 168: /* file_control_entry: SELECT flag_optional undefined_word $@15 select_clause_sequence '.'  */
#line 1702 "parser.y"
  {
	validate_file (current_file, yyvsp[-3]);
  }
#line 6159 "parser.c"
    break;

  case 186: /* assign_clause: ASSIGN _to _ext_clause _device assignment_name  */
#line 1734 "parser.y"
  {
	current_file->assign = cb_build_assignment_name (current_file, yyvsp[0]);
  }
#line 6167 "parser.c"
    break;

  case 187: /* assign_clause: ASSIGN _to _ext_clause DISK  */
#line 1738 "parser.y"
  {
	current_file->fileid_assign = 1;
	current_file->assign = cb_build_assignment_name (current_file, cb_build_reference ("DISK"));
  }
#line 6176 "parser.c"
    break;

  case 188: /* assign_clause: ASSIGN _to _ext_clause PRINTER  */
#line 1743 "parser.y"
  {
	current_file->fileid_assign = 1;
	current_file->assign = cb_build_assignment_name (current_file, cb_build_reference ("PRINTER"));
  }
#line 6185 "parser.c"
    break;

  case 191: /* _device: PRINTER  */
#line 1751 "parser.y"
                { current_file->organization = COB_ORG_LINE_SEQUENTIAL; }
#line 6191 "parser.c"
    break;

  case 193: /* _ext_clause: EXTERNAL  */
#line 1756 "parser.y"
  {
	current_file->external_assign = 1;
  }
#line 6199 "parser.c"
    break;

  case 194: /* _ext_clause: DYNAMIC  */
#line 1760 "parser.y"
  {
	current_file->external_assign = 0;
  }
#line 6207 "parser.c"
    break;

  case 196: /* assignment_name: DISPLAY  */
#line 1768 "parser.y"
  {
	const char	*s;

	s = "$#@DUMMY@#$";
	yyval = cb_build_alphanumeric_literal ((unsigned char *)s, strlen (s));
  }
#line 6218 "parser.c"
    break;

  case 197: /* assignment_name: _literal assignment_device_name_list  */
#line 1775 "parser.y"
  {

	if (!yyvsp[-1] || (yyvsp[-1] && CB_TREE_CLASS (yyvsp[-1]) == CB_CLASS_NUMERIC)) {
		if (yyvsp[0]) {
			if (CB_CHAIN (yyvsp[0])) {
				PENDING (_("ASSIGN TO multiple external device names"));
			}
			yyval = CB_VALUE (yyvsp[0]);
		}
	} else {
		if(yyvsp[0]) {
			PENDING (_("ASSIGN TO multiple external device names"));
		}
		yyval = yyvsp[-1];
	}
  }
#line 6239 "parser.c"
    break;

  case 198: /* assignment_device_name_list: qualified_word  */
#line 1794 "parser.y"
                                                { yyval = cb_list_init (yyvsp[0]); }
#line 6245 "parser.c"
    break;

  case 199: /* assignment_device_name_list: assignment_device_name_list qualified_word  */
#line 1795 "parser.y"
                                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 6251 "parser.c"
    break;

  case 201: /* access_mode: SEQUENTIAL  */
#line 1805 "parser.y"
                        { current_file->access_mode = COB_ACCESS_SEQUENTIAL; }
#line 6257 "parser.c"
    break;

  case 202: /* access_mode: DYNAMIC  */
#line 1806 "parser.y"
                        { current_file->access_mode = COB_ACCESS_DYNAMIC; }
#line 6263 "parser.c"
    break;

  case 203: /* access_mode: RANDOM  */
#line 1807 "parser.y"
                        { current_file->access_mode = COB_ACCESS_RANDOM; }
#line 6269 "parser.c"
    break;

  case 204: /* alternative_record_key_clause: ALTERNATE RECORD _key _is reference flag_duplicates  */
#line 1815 "parser.y"
  {
	struct cb_alt_key *p;
	struct cb_alt_key *l;

	p = cobc_malloc (sizeof (struct cb_alt_key));
	p->key = yyvsp[-1];
	p->duplicates = CB_INTEGER (yyvsp[0])->val;
	p->component_list = NULL;
	p->next = NULL;

	/* add to the end of list */
	if (current_file->alt_key_list == NULL) {
		current_file->alt_key_list = p;
	} else {
		l = current_file->alt_key_list;
		for (; l->next; l = l->next);
		l->next = p;
	}
  }
#line 6293 "parser.c"
    break;

  case 205: /* alternative_record_key_clause: ALTERNATE RECORD _key _is reference key_is_eq split_key_list flag_duplicates  */
#line 1835 "parser.y"
  {
	struct cb_alt_key *p;
	struct cb_alt_key *l;
	cb_tree composite_key;
	struct cb_key_component *comp;

	p = cobc_malloc (sizeof (struct cb_alt_key));
	/* generate field (in w-s) for composite-key */
	if (!yyvsp[-2]) {
		/* dialect */
		composite_key = cb_build_field (cb_build_anonymous ());
		comp = cobc_malloc (sizeof (struct cb_key_component));
		comp->next = key_component_list;
		comp->component = yyvsp[-3];
		key_component_list = comp;
	} else {
		/* standard or mf syntax */
		composite_key = cb_build_field (yyvsp[-3]);
	}
	if (composite_key == cb_error_node) {
		YYERROR;
	} else {
		composite_key->category = CB_CATEGORY_ALPHANUMERIC;
		((struct cb_field *)composite_key)->count = 1;
		p->key = cb_build_field_reference ((struct cb_field *)composite_key, NULL);
		p->component_list = key_component_list;
		p->duplicates = CB_INTEGER (yyvsp[0])->val;
		p->next = NULL;

		/* add to the end of list */
		if (current_file->alt_key_list == NULL) {
			current_file->alt_key_list = p;
		} else {
			l = current_file->alt_key_list;
			for (; l->next; l = l->next);
			l->next = p;
		}
	}
  }
#line 6337 "parser.c"
    break;

  case 206: /* $@16: %empty  */
#line 1877 "parser.y"
  {
	key_component_list = NULL;
  }
#line 6345 "parser.c"
    break;

  case 209: /* split_key: reference  */
#line 1886 "parser.y"
  {
	struct cb_key_component *c;
	struct cb_key_component *comp = cobc_malloc (sizeof (struct cb_key_component));
	comp->next = NULL;
	comp->component = yyvsp[0];
	if (key_component_list == NULL) {
		key_component_list = comp;
	} else {
		for (c = key_component_list; c->next != NULL; c = c->next);
		c->next = comp;
	}
  }
#line 6362 "parser.c"
    break;

  case 210: /* key_is_eq: %empty  */
#line 1901 "parser.y"
                { yyval = NULL; }
#line 6368 "parser.c"
    break;

  case 211: /* key_is_eq: SOURCE _is  */
#line 1902 "parser.y"
                { yyval = cb_int1; }
#line 6374 "parser.c"
    break;

  case 212: /* key_is_eq: '='  */
#line 1903 "parser.y"
                { yyval = cb_int('='); }
#line 6380 "parser.c"
    break;

  case 213: /* collating_sequence_clause: coll_sequence _is "Identifier"  */
#line 1910 "parser.y"
  {
	PENDING ("COLLATING SEQUENCE");
  }
#line 6388 "parser.c"
    break;

  case 214: /* file_status_clause: file_or_sort STATUS _is reference opt_reference  */
#line 1920 "parser.y"
  {
	current_file->file_status = yyvsp[-1];
	if (yyvsp[0]) {
		PENDING ("2nd FILE STATUS");
	}
  }
#line 6399 "parser.c"
    break;

  case 219: /* lock_mode: MANUAL lock_with  */
#line 1941 "parser.y"
                        { current_file->lock_mode = COB_LOCK_MANUAL; }
#line 6405 "parser.c"
    break;

  case 220: /* lock_mode: AUTOMATIC lock_with  */
#line 1942 "parser.y"
                        { current_file->lock_mode = COB_LOCK_AUTOMATIC; }
#line 6411 "parser.c"
    break;

  case 221: /* lock_mode: EXCLUSIVE  */
#line 1943 "parser.y"
                        { current_file->lock_mode = COB_LOCK_EXCLUSIVE; }
#line 6417 "parser.c"
    break;

  case 224: /* lock_with: WITH LOCK ON MULTIPLE lock_records  */
#line 1949 "parser.y"
  {
	current_file->lock_mode |= COB_LOCK_MULTIPLE;
  }
#line 6425 "parser.c"
    break;

  case 225: /* lock_with: WITH ROLLBACK  */
#line 1952 "parser.y"
                                { PENDING ("WITH ROLLBACK"); }
#line 6431 "parser.c"
    break;

  case 230: /* organization: INDEXED  */
#line 1969 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_INDEXED;
		organized_seen = 1;
	}
  }
#line 6444 "parser.c"
    break;

  case 231: /* organization: RECORD _binary SEQUENTIAL  */
#line 1978 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_SEQUENTIAL;
		organized_seen = 1;
	}
  }
#line 6457 "parser.c"
    break;

  case 232: /* organization: SEQUENTIAL  */
#line 1987 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = cb_default_organization;
		organized_seen = 1;
	}
  }
#line 6470 "parser.c"
    break;

  case 233: /* organization: RELATIVE  */
#line 1996 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_RELATIVE;
		organized_seen = 1;
	}
  }
#line 6483 "parser.c"
    break;

  case 234: /* organization: LINE SEQUENTIAL  */
#line 2005 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_LINE_SEQUENTIAL;
		organized_seen = 1;
	}
  }
#line 6496 "parser.c"
    break;

  case 235: /* padding_character_clause: PADDING _character _is reference_or_literal  */
#line 2020 "parser.y"
  {
	cb_verify (cb_padding_character_clause, "PADDING CHARACTER");
  }
#line 6504 "parser.c"
    break;

  case 236: /* record_delimiter_clause: RECORD DELIMITER _is "STANDARD-1"  */
#line 2029 "parser.y"
                                        { /* ignored */ }
#line 6510 "parser.c"
    break;

  case 237: /* record_key_clause: RECORD _key _is reference flag_duplicates  */
#line 2037 "parser.y"
  {

	if(yyvsp[0] == cb_int1) {
		cb_error (_("Record keys with duplicates are not yet supported"));
	}

	current_file->key = yyvsp[-1];
  }
#line 6523 "parser.c"
    break;

  case 238: /* record_key_clause: RECORD _key _is reference key_is_eq split_key_list flag_duplicates  */
#line 2046 "parser.y"
  {
	/* SPLIT KEY use */

	cb_tree composite_key;	
	struct cb_key_component *comp;

	if(yyvsp[0] == cb_int1) {
		cb_error (_("Record keys with duplicates are not yet supported"));
	}

	/* generate field (in w-s) for composite-key */
	if (!yyvsp[-2]) {
		/* dialect */
		composite_key = cb_build_field (cb_build_anonymous ());
		comp = cobc_malloc (sizeof (struct cb_key_component));
		comp->next = key_component_list;
		comp->component = yyvsp[-3];
		key_component_list = comp;
	} else {
		/* standard or mf syntax */
		composite_key = cb_build_field (yyvsp[-3]);
	}
	if (composite_key == cb_error_node) {
		YYERROR;
	} else {
		composite_key->category = CB_CATEGORY_ALPHANUMERIC;
		((struct cb_field *)composite_key)->count = 1;
		current_file->key = cb_build_field_reference ((struct cb_field *)composite_key, NULL);
		current_file->component_list = key_component_list;
	}
  }
#line 6559 "parser.c"
    break;

  case 239: /* relative_key_clause: RELATIVE _key _is reference  */
#line 2083 "parser.y"
                                { current_file->key = yyvsp[0]; }
#line 6565 "parser.c"
    break;

  case 240: /* reserve_clause: RESERVE integer _area  */
#line 2090 "parser.y"
                                { /* ignored */ }
#line 6571 "parser.c"
    break;

  case 241: /* reserve_clause: RESERVE NO  */
#line 2091 "parser.y"
                                { /* ignored */ }
#line 6577 "parser.c"
    break;

  case 242: /* sharing_clause: SHARING _with sharing_option  */
#line 2098 "parser.y"
                                { current_file->sharing = yyvsp[0]; }
#line 6583 "parser.c"
    break;

  case 243: /* sharing_option: ALL _other  */
#line 2102 "parser.y"
                                { yyval = NULL; PENDING ("SHARING ALL OTHER"); }
#line 6589 "parser.c"
    break;

  case 244: /* sharing_option: NO _other  */
#line 2103 "parser.y"
                                { yyval = cb_int1; }
#line 6595 "parser.c"
    break;

  case 245: /* sharing_option: READ ONLY  */
#line 2104 "parser.y"
                                { yyval = cb_int0; }
#line 6601 "parser.c"
    break;

  case 246: /* nominal_key_clause: NOMINAL _key _is reference  */
#line 2110 "parser.y"
                                { PENDING ("NOMINAL KEY"); }
#line 6607 "parser.c"
    break;

  case 257: /* same_clause: SAME same_option _area _for file_name_list  */
#line 2141 "parser.y"
  {
	cb_tree l;

	switch (CB_INTEGER (yyvsp[-3])->val) {
	case 0:
		/* SAME AREA */
		break;
	case 1:
		/* SAME RECORD */
		for (l = yyvsp[0]; l; l = CB_CHAIN (l)) {
			if (CB_VALUE (l) != cb_error_node) {
				CB_FILE (cb_ref (CB_VALUE (l)))->same_clause = samearea;
			}
		}
		samearea++;
		break;
	case 2:
		/* SAME SORT-MERGE */
		break;
	}
  }
#line 6633 "parser.c"
    break;

  case 258: /* same_option: %empty  */
#line 2165 "parser.y"
                                { yyval = cb_int0; }
#line 6639 "parser.c"
    break;

  case 259: /* same_option: RECORD  */
#line 2166 "parser.y"
                                { yyval = cb_int1; }
#line 6645 "parser.c"
    break;

  case 260: /* same_option: SORT  */
#line 2167 "parser.y"
                                { yyval = cb_int2; }
#line 6651 "parser.c"
    break;

  case 261: /* same_option: "SORT-MERGE"  */
#line 2168 "parser.y"
                                { yyval = cb_int2; }
#line 6657 "parser.c"
    break;

  case 262: /* multiple_file_tape_clause: MULTIPLE _file _tape _contains multiple_file_list  */
#line 2175 "parser.y"
  {
	cb_verify (cb_multiple_file_tape_clause, "MULTIPLE FILE TAPE");
  }
#line 6665 "parser.c"
    break;

  case 265: /* multiple_file: file_name multiple_file_position  */
#line 2186 "parser.y"
                                   { }
#line 6671 "parser.c"
    break;

  case 271: /* apply_clause: APPLY "COMMITMENT-CONTROL" _on reference_list  */
#line 2203 "parser.y"
  {
	PENDING ("APPLY COMMITMENT-CONTROL");
  }
#line 6679 "parser.c"
    break;

  case 272: /* apply_clause: APPLY "CYL-OVERFLOW" _of "Literal" TRACKS ON reference_list  */
#line 2207 "parser.y"
  {
	PENDING ("APPLY CYL-OVERFLOW");
  }
#line 6687 "parser.c"
    break;

  case 273: /* apply_clause: APPLY "CORE-INDEX" TO reference ON reference_list  */
#line 2211 "parser.y"
  {
	PENDING ("APPLY CORE-INDEX");
  }
#line 6695 "parser.c"
    break;

  case 274: /* apply_clause: APPLY "FORMS-OVERLAY" TO reference ON reference_list  */
#line 2215 "parser.y"
  {
	PENDING ("APPLY FORMS-OVERLAY");
  }
#line 6703 "parser.c"
    break;

  case 275: /* apply_clause: APPLY "CLOSE-NOFEED" ON reference_list  */
#line 2219 "parser.y"
  {
	PENDING ("APPLY CLOSE-NOFEED");
  }
#line 6711 "parser.c"
    break;

  case 279: /* $@17: %empty  */
#line 2244 "parser.y"
                                { current_storage = CB_STORAGE_FILE; }
#line 6717 "parser.c"
    break;

  case 281: /* $@18: %empty  */
#line 2247 "parser.y"
  {
	/* hack for MF compatibility */
	if (cb_relaxed_syntax_check) {
		cb_warning (_("FILE SECTION header missing - assumed"));
	} else {
		cb_error (_("FILE SECTION header missing"));
	}
	current_storage = CB_STORAGE_FILE;
  }
#line 6731 "parser.c"
    break;

  case 285: /* file_description: file_type file_description_entry record_description_list  */
#line 2266 "parser.y"
  {
	if (yyvsp[0] && yyvsp[0] != cb_error_node) {
		finalize_file (current_file, CB_FIELD (yyvsp[0]));
	} else {
		cb_error (_("RECORD description missing or invalid"));
	}
  }
#line 6743 "parser.c"
    break;

  case 286: /* file_description_sequence_without_type: file_description_entry record_description_list  */
#line 2278 "parser.y"
  {
	if (yyvsp[0] && yyvsp[0] != cb_error_node) {
		finalize_file (current_file, CB_FIELD (yyvsp[0]));
	} else {
		cb_error (_("RECORD description missing or invalid"));
	}
  }
#line 6755 "parser.c"
    break;

  case 288: /* file_type: FD  */
#line 2289 "parser.y"
                               { yyval = cb_int0; }
#line 6761 "parser.c"
    break;

  case 289: /* file_type: SD  */
#line 2290 "parser.y"
                               { yyval = cb_int1; }
#line 6767 "parser.c"
    break;

  case 290: /* @19: %empty  */
#line 2300 "parser.y"
  {
	if (yyvsp[0] == cb_error_node) {
		YYERROR;
	}

	current_file = CB_FILE (cb_ref (yyvsp[0]));
	if (yyvsp[-1] == cb_int1) {
		current_file->organization = COB_ORG_SORT;
	}
  }
#line 6782 "parser.c"
    break;

  case 291: /* file_description_entry: file_name @19 file_description_clause_sequence '.'  */
#line 2311 "parser.y"
  {
	/* Shut up bison */
	dummy_tree = yyvsp[-2];
  }
#line 6791 "parser.c"
    break;

  case 294: /* file_description_clause: _is EXTERNAL  */
#line 2323 "parser.y"
  {
	if (current_file->global) {
		cb_error (_("File cannot have both EXTERNAL and GLOBAL clauses"));
	}
	current_file->external = 1;
  }
#line 6802 "parser.c"
    break;

  case 295: /* file_description_clause: _is GLOBAL  */
#line 2330 "parser.y"
  {
	if (current_file->external) {
		cb_error (_("File cannot have both EXTERNAL and GLOBAL clauses"));
	}
	current_file->global = 1;
  }
#line 6813 "parser.c"
    break;

  case 306: /* block_contains_clause: BLOCK _contains integer opt_to_integer _records_or_characters  */
#line 2353 "parser.y"
  { /* ignored */ }
#line 6819 "parser.c"
    break;

  case 310: /* record_clause: RECORD _contains integer _characters  */
#line 2363 "parser.y"
  {
	if (current_file->organization == COB_ORG_LINE_SEQUENTIAL) {
		cb_warning (_("RECORD clause ignored for LINE SEQUENTIAL"));
	} else {
		current_file->record_max = cb_get_int (yyvsp[-1]);
		if (current_file->record_max < 1)  {
			current_file->record_max = 1;
			cb_error (_("RECORD clause invalid"));
		}
	}
  }
#line 6835 "parser.c"
    break;

  case 311: /* record_clause: RECORD _contains integer TO integer _characters  */
#line 2375 "parser.y"
  {
	int	error_ind = 0;

	if (current_file->organization == COB_ORG_LINE_SEQUENTIAL) {
		cb_warning (_("RECORD clause ignored for LINE SEQUENTIAL"));
	} else {
		current_file->record_min = cb_get_int (yyvsp[-3]);
		current_file->record_max = cb_get_int (yyvsp[-1]);
		if (current_file->record_min < 0)  {
			current_file->record_min = 0;
			error_ind = 1;
		}
		if (current_file->record_max < 1)  {
			current_file->record_max = 1;
			error_ind = 1;
		}
		if (current_file->record_max <= current_file->record_min)  {
			error_ind = 1;
		}
		if (error_ind) {
			cb_error (_("RECORD clause invalid"));
		}
	}
  }
#line 6864 "parser.c"
    break;

  case 312: /* record_clause: RECORD _is VARYING _in _size opt_from_integer opt_to_integer _characters record_depending  */
#line 2401 "parser.y"
  {
	int	error_ind = 0;

	current_file->record_min = yyvsp[-3] ? cb_get_int (yyvsp[-3]) : 0;
	current_file->record_max = yyvsp[-2] ? cb_get_int (yyvsp[-2]) : 0;
	if (yyvsp[-3] && current_file->record_min < 0)  {
		current_file->record_min = 0;
		error_ind = 1;
	}
	if (yyvsp[-2] && current_file->record_max < 1)  {
		current_file->record_max = 1;
		error_ind = 1;
	}
	if ((yyvsp[-3] || yyvsp[-2]) && current_file->record_max <= current_file->record_min)  {
		error_ind = 1;
	}
	if (error_ind) {
		cb_error (_("RECORD clause invalid"));
	}
  }
#line 6889 "parser.c"
    break;

  case 314: /* record_depending: DEPENDING _on reference  */
#line 2425 "parser.y"
  {
	current_file->record_depending = yyvsp[0];
  }
#line 6897 "parser.c"
    break;

  case 315: /* opt_from_integer: %empty  */
#line 2431 "parser.y"
                                { yyval = NULL; }
#line 6903 "parser.c"
    break;

  case 316: /* opt_from_integer: _from integer  */
#line 2432 "parser.y"
                                { yyval = yyvsp[0]; }
#line 6909 "parser.c"
    break;

  case 317: /* opt_to_integer: %empty  */
#line 2436 "parser.y"
                                { yyval = NULL; }
#line 6915 "parser.c"
    break;

  case 318: /* opt_to_integer: TO integer  */
#line 2437 "parser.y"
                                { yyval = yyvsp[0]; }
#line 6921 "parser.c"
    break;

  case 319: /* label_records_clause: LABEL records label_option  */
#line 2445 "parser.y"
  {
	cb_verify (cb_label_records_clause, "LABEL RECORDS");
  }
#line 6929 "parser.c"
    break;

  case 322: /* value_of_clause: VALUE OF "Identifier" _is valueof_name  */
#line 2460 "parser.y"
  {
	cb_verify (cb_value_of_clause, "VALUE OF");
  }
#line 6937 "parser.c"
    break;

  case 323: /* value_of_clause: VALUE OF "FILE-ID" _is valueof_name  */
#line 2464 "parser.y"
  {
	if (!current_file->assign) {
		current_file->assign = cb_build_assignment_name (current_file, yyvsp[0]);
	}
  }
#line 6947 "parser.c"
    break;

  case 326: /* data_records_clause: DATA records no_reference_list  */
#line 2480 "parser.y"
  {
	cb_verify (cb_data_records_clause, "DATA RECORDS");
  }
#line 6955 "parser.c"
    break;

  case 327: /* linage_clause: LINAGE _is reference_or_literal _lines linage_sequence  */
#line 2491 "parser.y"
  {
	if (current_file->organization != COB_ORG_LINE_SEQUENTIAL
	    && current_file->organization != COB_ORG_SEQUENTIAL) {
		cb_error (_("LINAGE clause with wrong file type"));
		yyval = cb_error_node;
	} else {
		current_file->linage = yyvsp[-2];
		current_file->organization = COB_ORG_LINE_SEQUENTIAL;
		if (current_linage == 0) {
			linage_file = current_file;
		}
		current_linage++;
	}
  }
#line 6974 "parser.c"
    break;

  case 333: /* linage_footing: _with FOOTING _at reference_or_literal _lines  */
#line 2519 "parser.y"
  {
	current_file->latfoot = yyvsp[-1];
  }
#line 6982 "parser.c"
    break;

  case 334: /* linage_top: _at TOP reference_or_literal _lines  */
#line 2526 "parser.y"
  {
	current_file->lattop = yyvsp[-1];
  }
#line 6990 "parser.c"
    break;

  case 335: /* linage_bottom: _at BOTTOM reference_or_literal  */
#line 2533 "parser.y"
  {
	current_file->latbot = yyvsp[0];
  }
#line 6998 "parser.c"
    break;

  case 336: /* recording_mode_clause: RECORDING _mode _is "Identifier"  */
#line 2542 "parser.y"
                                { /* ignore */ }
#line 7004 "parser.c"
    break;

  case 337: /* code_set_clause: "CODE-SET" _is "Identifier"  */
#line 2550 "parser.y"
  {
	if (yyvsp[0] != cb_error_node) {
		cb_tree x;

		x = cb_ref (yyvsp[0]);
		if (!CB_ALPHABET_NAME_P (x)) {
			cb_error_x (yyvsp[0], _("Alphabet-name is expected '%s'"), cb_name (yyvsp[0]));
		} else if (CB_ALPHABET_NAME (x)->custom_list) {
			PENDING ("CODE-SET");
		}
	}
  }
#line 7021 "parser.c"
    break;

  case 338: /* report_clause: REPORT _is report_name  */
#line 2568 "parser.y"
  {
	cb_warning (_("file descriptor REPORT IS"));
  }
#line 7029 "parser.c"
    break;

  case 339: /* report_clause: REPORTS _are report_name  */
#line 2572 "parser.y"
  {
	cb_warning (_("file descriptor REPORTS ARE"));
  }
#line 7037 "parser.c"
    break;

  case 341: /* $@20: %empty  */
#line 2583 "parser.y"
                                { current_storage = CB_STORAGE_WORKING; }
#line 7043 "parser.c"
    break;

  case 342: /* working_storage_section: "WORKING-STORAGE" SECTION '.' $@20 record_description_list  */
#line 2585 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->working_storage =
			cb_field_add (current_program->working_storage, CB_FIELD (yyvsp[0]));
	}
  }
#line 7054 "parser.c"
    break;

  case 343: /* record_description_list: %empty  */
#line 2594 "parser.y"
                                { yyval = NULL; }
#line 7060 "parser.c"
    break;

  case 344: /* record_description_list: record_description_list_1  */
#line 2595 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7066 "parser.c"
    break;

  case 345: /* $@21: %empty  */
#line 2599 "parser.y"
  {
	current_field = NULL;
	description_field = NULL;
	cb_clear_real_field ();
  }
#line 7076 "parser.c"
    break;

  case 346: /* record_description_list_1: $@21 record_description_list_2  */
#line 2605 "parser.y"
  {
	struct cb_field *p;

	for (p = description_field; p; p = p->sister) {
		cb_validate_field (p);
	}
	yyval = CB_TREE (description_field);
  }
#line 7089 "parser.c"
    break;

  case 352: /* exec_sql_data_statement: "EXEC SQL statement"  */
#line 2626 "parser.y"
  {
	/* DATA DIVISION に渡ってきた EXEC SQL。INCLUDE SQLCA や DECLARE SECTION は
	   前処理 (pplex) 側で消費されるため通常ここには来ないが、その他の EXEC SQL
	   が来た場合に備えて受け取り (実行コードは生成しない)。 */
	(void) CB_LITERAL (yyvsp[0])->data;
  }
#line 7100 "parser.c"
    break;

  case 353: /* exec_sql_data_statement: "EXEC SQL statement" '.'  */
#line 2633 "parser.y"
  {
	(void) CB_LITERAL (yyvsp[-1])->data;
  }
#line 7108 "parser.c"
    break;

  case 355: /* $@22: %empty  */
#line 2641 "parser.y"
  {
	cb_tree x;

	x = cb_build_field_tree (yyvsp[-1], yyvsp[0], current_field, current_storage, current_file, cb_source_file, cb_source_line, prev_field_line_number);
	prev_field_line_number = cb_source_line;
	if (x == cb_error_node) {
		YYERROR;
	} else {
		current_field = CB_FIELD (x);
	}
  }
#line 7124 "parser.c"
    break;

  case 356: /* data_description: level_number entry_name $@22 data_description_clause_sequence _maybe_next_level_number  */
#line 2653 "parser.y"
  {
	if (!qualifier && (current_field->level == 66 || current_field->flag_item_78)) {
		cb_error (_("Item requires a data name"));
	}
	if (current_field->flag_item_78) {
		/* Reset to last non-78 item */
		current_field = cb_validate_78_item (current_field);
	}
	if (!description_field) {
		description_field = current_field;
	}
  }
#line 7141 "parser.c"
    break;

  case 357: /* $@23: %empty  */
#line 2666 "parser.y"
  {
	cb_tree x;

	x = cb_build_field_tree (yyvsp[-1], yyvsp[0], current_field, current_storage, current_file, cb_source_file, cb_source_line, prev_field_line_number);
	prev_field_line_number = cb_source_line;
	if (x == cb_error_node) {
		YYERROR;
	} else {
		current_field = CB_FIELD (x);
	}
  }
#line 7157 "parser.c"
    break;

  case 358: /* data_description: level_number_88 entry_name $@23 value_cond_clause  */
#line 2678 "parser.y"
  {
	if (!qualifier) {
		cb_error (_("Item requires a data name"));
	}
	cb_validate_88_item (current_field);
	if (!description_field) {
		description_field = current_field;
	}
	
  }
#line 7172 "parser.c"
    break;

  case 362: /* _maybe_next_level_number: "Literal"  */
#line 2705 "parser.y"
  {
	if (CB_TREE_CLASS (yyvsp[0]) == CB_CLASS_NUMERIC) {
		cb_tree x = cb_build_reference ((char *)CB_LITERAL(yyvsp[0])->data);
		int lev = cb_get_level (x);
		if (!lev) {
			/* do nothing expecting cb_get_level() had
			 * already given some error message. */
		} else if (lev == 88) {
			cb_unget_token (LEVEL88_NUMBER_WORD, x);
		} else {
			cb_unget_token (LEVEL_NUMBER_WORD, x);		  
		}
	} else {
		/* cause syntax error */
		cb_unget_token (LITERAL, yyvsp[0]);
	}
  }
#line 7194 "parser.c"
    break;

  case 363: /* entry_name: %empty  */
#line 2726 "parser.y"
  {
	yyval = cb_build_filler ();
	qualifier = NULL;
	non_const_word = 0;
  }
#line 7204 "parser.c"
    break;

  case 364: /* entry_name: FILLER  */
#line 2732 "parser.y"
  {
	yyval = cb_build_filler ();
	qualifier = NULL;
	non_const_word = 0;
  }
#line 7214 "parser.c"
    break;

  case 365: /* entry_name: "Identifier"  */
#line 2738 "parser.y"
  {
	yyval = yyvsp[0];
	qualifier = yyvsp[0];
	non_const_word = 0;
  }
#line 7224 "parser.c"
    break;

  case 366: /* const_name: "Identifier"  */
#line 2747 "parser.y"
  {
	yyval = yyvsp[0];
	qualifier = yyvsp[0];
	non_const_word = 0;
  }
#line 7234 "parser.c"
    break;

  case 368: /* const_global: _is GLOBAL  */
#line 2756 "parser.y"
  {
	current_field->flag_is_global = 1;
	cb_error (_("CONSTANT with GLOBAL clause is not yet supported"));
  }
#line 7243 "parser.c"
    break;

  case 369: /* lit_or_length: literal  */
#line 2763 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7249 "parser.c"
    break;

  case 370: /* lit_or_length: LENGTH _of identifier_1  */
#line 2764 "parser.y"
                                { yyval = cb_build_const_length (yyvsp[0]); }
#line 7255 "parser.c"
    break;

  case 371: /* lit_or_length: "BYTE-LENGTH" _of identifier_1  */
#line 2765 "parser.y"
                                { yyval = cb_build_const_length (yyvsp[0]); }
#line 7261 "parser.c"
    break;

  case 372: /* constant_entry: level_number const_name CONSTANT const_global _as lit_or_length  */
#line 2770 "parser.y"
  {
	cb_tree x;
	int	level;

	level = cb_get_level (yyvsp[-5]);
	if (level && level != 1) {
		cb_error (_("CONSTANT item not at 01 level"));
	}
	x = cb_build_constant (yyvsp[-4], yyvsp[0]);
	CB_FIELD (x)->flag_item_78 = 1;
	CB_FIELD (x)->level = 1;
	cb_needs_01 = 1;
	/* Ignore return value */
	cb_validate_78_item (CB_FIELD (x));
  }
#line 7281 "parser.c"
    break;

  case 373: /* data_description_clause_sequence: %empty  */
#line 2789 "parser.y"
  {
	/* required to check redefines */
	yyval = NULL;
  }
#line 7290 "parser.c"
    break;

  case 374: /* data_description_clause_sequence: data_description_clause_sequence data_description_clause  */
#line 2795 "parser.y"
  {
	/* required to check redefines */
	yyval = cb_true;
  }
#line 7299 "parser.c"
    break;

  case 391: /* redefines_clause: REDEFINES identifier_1  */
#line 2825 "parser.y"
  {
	if (yyvsp[-2] != NULL) {
		/* hack for MF compatibility */
		if (cb_relaxed_syntax_check) {
			cb_warning_x (yyvsp[0], _("REDEFINES clause should follow entry-name"));
		} else {
			cb_error_x (yyvsp[0], _("REDEFINES clause must follow entry-name"));
		}
	}

	current_field->redefines = cb_resolve_redefines (current_field, yyvsp[0]);
	if (current_field->redefines == NULL) {
		YYERROR;
	}
  }
#line 7319 "parser.c"
    break;

  case 392: /* external_clause: _is EXTERNAL as_extname  */
#line 2847 "parser.y"
  {
	if (current_storage != CB_STORAGE_WORKING) {
		cb_error (_("EXTERNAL not allowed here"));
	} else if (current_field->level != 1 && current_field->level != 77) {
		cb_error (_("EXTERNAL only allowed at 01/77 level"));
	} else if (!qualifier) {
		cb_error (_("EXTERNAL requires a data name"));
	} else if (current_field->flag_is_global) {
		cb_error (_("GLOBAL and EXTERNAL are mutually exclusive"));
	} else if (current_field->flag_item_based) {
		cb_error (_("BASED and EXTERNAL are mutually exclusive"));
	} else if (current_field->redefines) {
		cb_error (_("EXTERNAL and REDEFINES are mutually exclusive"));
	} else {
		current_field->flag_external = 1;
		has_external = 1;
	}
  }
#line 7342 "parser.c"
    break;

  case 393: /* as_extname: %empty  */
#line 2868 "parser.y"
                                { current_field->ename = NULL; }
#line 7348 "parser.c"
    break;

  case 394: /* as_extname: AS "Literal"  */
#line 2870 "parser.y"
 {
	struct cb_field *x;

	x = CB_FIELD(cb_build_field (cb_build_reference ((char *)(CB_LITERAL (yyvsp[0])->data))));
	current_field->ename = x->name;
 }
#line 7359 "parser.c"
    break;

  case 395: /* global_clause: _is GLOBAL  */
#line 2882 "parser.y"
  {
	if (current_field->level != 1 && current_field->level != 77) {
		cb_error (_("GLOBAL only allowed at 01/77 level"));
	} else if (!qualifier) {
		cb_error (_("GLOBAL requires a data name"));
	} else if (current_field->flag_external) {
		cb_error (_("GLOBAL and EXTERNAL are mutually exclusive"));
	} else if (current_storage == CB_STORAGE_LOCAL) {
		cb_error (_("GLOBAL not allowed here"));
	} else {
		current_field->flag_is_global = 1;
	}
  }
#line 7377 "parser.c"
    break;

  case 396: /* picture_clause: PICTURE  */
#line 2901 "parser.y"
                                { current_field->pic = CB_PICTURE (yyvsp[0]); }
#line 7383 "parser.c"
    break;

  case 399: /* usage: BINARY  */
#line 2913 "parser.y"
                                { current_field->usage = CB_USAGE_BINARY; }
#line 7389 "parser.c"
    break;

  case 400: /* usage: COMP  */
#line 2914 "parser.y"
                                { current_field->usage = CB_USAGE_BINARY; }
#line 7395 "parser.c"
    break;

  case 401: /* usage: "COMP-1"  */
#line 2916 "parser.y"
{
	current_field->usage = CB_USAGE_FLOAT; 
	cb_error (_("COMP-1 not implemented"));
}
#line 7404 "parser.c"
    break;

  case 402: /* usage: "COMP-2"  */
#line 2921 "parser.y"
{ 
	current_field->usage = CB_USAGE_DOUBLE; 
	cb_error (_("COMP-2 not implemented"));
}
#line 7413 "parser.c"
    break;

  case 403: /* usage: "COMP-3"  */
#line 2925 "parser.y"
                                { current_field->usage = CB_USAGE_PACKED; }
#line 7419 "parser.c"
    break;

  case 404: /* usage: "COMP-4"  */
#line 2926 "parser.y"
                                { current_field->usage = CB_USAGE_BINARY; }
#line 7425 "parser.c"
    break;

  case 405: /* usage: "COMP-5"  */
#line 2927 "parser.y"
                                { current_field->usage = CB_USAGE_COMP_5; }
#line 7431 "parser.c"
    break;

  case 406: /* usage: "COMP-X"  */
#line 2928 "parser.y"
                                { current_field->usage = CB_USAGE_COMP_X; }
#line 7437 "parser.c"
    break;

  case 407: /* usage: DISPLAY  */
#line 2929 "parser.y"
                                { current_field->usage = CB_USAGE_DISPLAY; }
#line 7443 "parser.c"
    break;

  case 408: /* usage: INDEX  */
#line 2930 "parser.y"
                                { current_field->usage = CB_USAGE_INDEX; }
#line 7449 "parser.c"
    break;

  case 409: /* usage: "PACKED-DECIMAL"  */
#line 2931 "parser.y"
                                { current_field->usage = CB_USAGE_PACKED; }
#line 7455 "parser.c"
    break;

  case 410: /* usage: POINTER  */
#line 2933 "parser.y"
  {
	current_field->usage = CB_USAGE_POINTER;
	current_field->flag_is_pointer = 1;
  }
#line 7464 "parser.c"
    break;

  case 411: /* usage: "PROGRAM-POINTER"  */
#line 2938 "parser.y"
  {
	current_field->usage = CB_USAGE_PROGRAM_POINTER;
	current_field->flag_is_pointer = 1;
  }
#line 7473 "parser.c"
    break;

  case 412: /* usage: "SIGNED-SHORT"  */
#line 2942 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_SHORT; }
#line 7479 "parser.c"
    break;

  case 413: /* usage: "SIGNED-INT"  */
#line 2943 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_INT; }
#line 7485 "parser.c"
    break;

  case 414: /* usage: "SIGNED-LONG"  */
#line 2944 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_LONG; }
#line 7491 "parser.c"
    break;

  case 415: /* usage: "UNSIGNED-SHORT"  */
#line 2945 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_SHORT; }
#line 7497 "parser.c"
    break;

  case 416: /* usage: "UNSIGNED-INT"  */
#line 2946 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_INT; }
#line 7503 "parser.c"
    break;

  case 417: /* usage: "UNSIGNED-LONG"  */
#line 2947 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_LONG; }
#line 7509 "parser.c"
    break;

  case 418: /* usage: "BINARY-CHAR" SIGNED  */
#line 2948 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_CHAR; }
#line 7515 "parser.c"
    break;

  case 419: /* usage: "BINARY-CHAR" UNSIGNED  */
#line 2949 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_CHAR; }
#line 7521 "parser.c"
    break;

  case 420: /* usage: "BINARY-CHAR"  */
#line 2950 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_CHAR; }
#line 7527 "parser.c"
    break;

  case 421: /* usage: "BINARY-SHORT" SIGNED  */
#line 2951 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_SHORT; }
#line 7533 "parser.c"
    break;

  case 422: /* usage: "BINARY-SHORT" UNSIGNED  */
#line 2952 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_SHORT; }
#line 7539 "parser.c"
    break;

  case 423: /* usage: "BINARY-SHORT"  */
#line 2953 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_SHORT; }
#line 7545 "parser.c"
    break;

  case 424: /* usage: "BINARY-LONG" SIGNED  */
#line 2954 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_INT; }
#line 7551 "parser.c"
    break;

  case 425: /* usage: "BINARY-LONG" UNSIGNED  */
#line 2955 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_INT; }
#line 7557 "parser.c"
    break;

  case 426: /* usage: "BINARY-LONG"  */
#line 2956 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_INT; }
#line 7563 "parser.c"
    break;

  case 427: /* usage: "BINARY-DOUBLE" SIGNED  */
#line 2957 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_LONG; }
#line 7569 "parser.c"
    break;

  case 428: /* usage: "BINARY-DOUBLE" UNSIGNED  */
#line 2958 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_LONG; }
#line 7575 "parser.c"
    break;

  case 429: /* usage: "BINARY-DOUBLE"  */
#line 2959 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_LONG; }
#line 7581 "parser.c"
    break;

  case 430: /* usage: "BINARY-C-LONG" SIGNED  */
#line 2961 "parser.y"
  {
	if (sizeof(long) == 4) {
		current_field->usage = CB_USAGE_SIGNED_INT;
	} else {
		current_field->usage = CB_USAGE_SIGNED_LONG;
	}
  }
#line 7593 "parser.c"
    break;

  case 431: /* usage: "BINARY-C-LONG" UNSIGNED  */
#line 2969 "parser.y"
  {
	if (sizeof(long) == 4) {
		current_field->usage = CB_USAGE_UNSIGNED_INT;
	} else {
		current_field->usage = CB_USAGE_UNSIGNED_LONG;
	}
  }
#line 7605 "parser.c"
    break;

  case 432: /* usage: "BINARY-C-LONG"  */
#line 2977 "parser.y"
  {
	if (sizeof(long) == 4) {
		current_field->usage = CB_USAGE_SIGNED_INT;
	} else {
		current_field->usage = CB_USAGE_SIGNED_LONG;
	}
  }
#line 7617 "parser.c"
    break;

  case 433: /* usage: NATIONAL  */
#line 2984 "parser.y"
                                { PENDING ("USAGE NATIONAL");}
#line 7623 "parser.c"
    break;

  case 434: /* sign_clause: _sign_is LEADING flag_separate  */
#line 2992 "parser.y"
  {
	current_field->flag_sign_separate = CB_INTEGER (yyvsp[0])->val;
	current_field->flag_sign_leading  = 1;
  }
#line 7632 "parser.c"
    break;

  case 435: /* sign_clause: _sign_is TRAILING flag_separate  */
#line 2997 "parser.y"
  {
	current_field->flag_sign_separate = CB_INTEGER (yyvsp[0])->val;
	current_field->flag_sign_leading  = 0;
  }
#line 7641 "parser.c"
    break;

  case 439: /* occurs_clause: OCCURS integer occurs_to_integer _times occurs_depending occurs_key_spec  */
#line 3014 "parser.y"
  {
	if (current_field->occurs_depending && !(yyvsp[-3])) {
		cb_verify (cb_odo_without_to, "ODO without TO clause");
	}
	current_field->occurs_min = yyvsp[-3] ? cb_get_int (yyvsp[-4]) : 1;
	current_field->occurs_max = yyvsp[-3] ? cb_get_int (yyvsp[-3]) : cb_get_int (yyvsp[-4]);
	current_field->indexes++;
	if (current_field->indexes > COB_MAX_SUBSCRIPTS) {
		cb_error (_("Maximum OCCURS depth exceeded"));
	}
	current_field->flag_occurs = 1;
  }
#line 7658 "parser.c"
    break;

  case 440: /* occurs_to_integer: %empty  */
#line 3029 "parser.y"
                                { yyval = NULL; }
#line 7664 "parser.c"
    break;

  case 441: /* occurs_to_integer: TO integer  */
#line 3030 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7670 "parser.c"
    break;

  case 443: /* occurs_depending: DEPENDING _on reference  */
#line 3035 "parser.y"
  {
	current_field->occurs_depending = yyvsp[0];
  }
#line 7678 "parser.c"
    break;

  case 446: /* occurs_keys: occurs_key_list  */
#line 3044 "parser.y"
  {
	if (yyvsp[0]) {
		cb_tree		l;
		struct cb_key	*keys;
		int		i;
		int		nkeys;

		l = yyvsp[0];
		nkeys = cb_list_length (yyvsp[0]);
		keys = cobc_malloc (sizeof (struct cb_key) * nkeys);

		for (i = 0; i < nkeys; i++) {
			keys[i].dir = CB_PURPOSE_INT (l);
			keys[i].key = CB_VALUE (l);
			l = CB_CHAIN (l);
		}
		current_field->keys = keys;
		current_field->nkeys = nkeys;
	}
  }
#line 7703 "parser.c"
    break;

  case 447: /* occurs_key: ascending_or_descending _key _is reference_list  */
#line 3068 "parser.y"
  {
	cb_tree l;

	for (l = yyvsp[0]; l; l = CB_CHAIN (l)) {
		CB_PURPOSE (l) = yyvsp[-3];
		if (qualifier && !CB_REFERENCE(CB_VALUE(l))->chain &&
		    strcasecmp (CB_NAME(CB_VALUE(l)), CB_NAME(qualifier))) {
			CB_REFERENCE(CB_VALUE(l))->chain = qualifier;
		}
	}
	yyval = yyvsp[0];
  }
#line 7720 "parser.c"
    break;

  case 448: /* occurs_key_list: occurs_key  */
#line 3083 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7726 "parser.c"
    break;

  case 449: /* occurs_key_list: occurs_key_list occurs_key  */
#line 3084 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 7732 "parser.c"
    break;

  case 450: /* ascending_or_descending: ASCENDING  */
#line 3088 "parser.y"
                                { yyval = cb_int (COB_ASCENDING); }
#line 7738 "parser.c"
    break;

  case 451: /* ascending_or_descending: DESCENDING  */
#line 3089 "parser.y"
                                { yyval = cb_int (COB_DESCENDING); }
#line 7744 "parser.c"
    break;

  case 454: /* occurs_indexed: INDEXED _by occurs_index_list  */
#line 3096 "parser.y"
  {
	current_field->index_list = yyvsp[0];
  }
#line 7752 "parser.c"
    break;

  case 455: /* occurs_index_list: occurs_index  */
#line 3102 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 7758 "parser.c"
    break;

  case 456: /* occurs_index_list: occurs_index_list occurs_index  */
#line 3104 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 7764 "parser.c"
    break;

  case 457: /* occurs_index: "Identifier"  */
#line 3109 "parser.y"
  {
	yyval = cb_build_index (yyvsp[0], cb_int1, 1, current_field);
  }
#line 7772 "parser.c"
    break;

  case 458: /* justified_clause: JUSTIFIED _right  */
#line 3118 "parser.y"
                                { current_field->flag_justified = 1; }
#line 7778 "parser.c"
    break;

  case 459: /* synchronized_clause: SYNCHRONIZED left_or_right  */
#line 3125 "parser.y"
                                { current_field->flag_synchronized = 1; }
#line 7784 "parser.c"
    break;

  case 463: /* blank_clause: BLANK _when ZERO  */
#line 3137 "parser.y"
                                { current_field->flag_blank_zero = 1; }
#line 7790 "parser.c"
    break;

  case 464: /* based_clause: BASED  */
#line 3145 "parser.y"
  {
	if (current_storage != CB_STORAGE_WORKING &&
	    current_storage != CB_STORAGE_LINKAGE &&
	    current_storage != CB_STORAGE_LOCAL) {
		cb_error (_("BASED not allowed here"));
	} else if (current_field->level != 1 && current_field->level != 77) {
		cb_error (_("BASED only allowed at 01/77 level"));
	} else if (!qualifier) {
		cb_error (_("BASED requires a data name"));
	} else if (current_field->flag_external) {
		cb_error (_("BASED and EXTERNAL are mutually exclusive"));
	} else if (current_field->redefines) {
		cb_error (_("BASED and REDEFINES are mutually exclusive"));
	} else if (current_field->flag_any_length) {
		cb_error (_("BASED and ANY LENGTH are mutually exclusive"));
	} else {
		current_field->flag_item_based = 1;
	}
  }
#line 7814 "parser.c"
    break;

  case 465: /* value_clause: VALUE _is literal  */
#line 3169 "parser.y"
                                { current_field->values = cb_list_init (yyvsp[0]); }
#line 7820 "parser.c"
    break;

  case 466: /* $@24: %empty  */
#line 3173 "parser.y"
                                { current_field->values = yyvsp[0]; }
#line 7826 "parser.c"
    break;

  case 468: /* value_item_list: value_item  */
#line 3178 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 7832 "parser.c"
    break;

  case 469: /* value_item_list: value_item_list value_item  */
#line 3179 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 7838 "parser.c"
    break;

  case 470: /* value_item: literal  */
#line 3183 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7844 "parser.c"
    break;

  case 471: /* value_item: literal THRU literal  */
#line 3184 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-2], yyvsp[0]); }
#line 7850 "parser.c"
    break;

  case 473: /* false_is: "FALSE" _is literal  */
#line 3189 "parser.y"
  {
	if (current_field->level != 88) {
		cb_error (_("FALSE clause only allowed for 88 level"));
	}
	current_field->false_88 = cb_list_init (yyvsp[0]);
  }
#line 7861 "parser.c"
    break;

  case 474: /* renames_clause: RENAMES qualified_word  */
#line 3202 "parser.y"
  {
	if (cb_ref (yyvsp[0]) != cb_error_node) {
		if (CB_FIELD (cb_ref (yyvsp[0]))->level == 01 ||
		    CB_FIELD (cb_ref (yyvsp[0]))->level > 50) {
			cb_error (_("RENAMES may not reference a level 01 or > 50"));
		} else {
			current_field->redefines = CB_FIELD (cb_ref (yyvsp[0]));
			current_field->pic = current_field->redefines->pic;
		}
	}
  }
#line 7877 "parser.c"
    break;

  case 475: /* renames_clause: RENAMES qualified_word THRU qualified_word  */
#line 3214 "parser.y"
  {
	if (cb_ref (yyvsp[-2]) != cb_error_node && cb_ref (yyvsp[0]) != cb_error_node) {
		if (CB_FIELD (cb_ref (yyvsp[-2]))->level == 01 ||
		    CB_FIELD (cb_ref (yyvsp[-2]))->level > 50) {
			cb_error (_("RENAMES may not reference a level 01 or > 50"));
		} else if (CB_FIELD (cb_ref (yyvsp[0]))->level == 01 ||
		    CB_FIELD (cb_ref (yyvsp[0]))->level > 50) {
			cb_error (_("RENAMES may not reference a level 01 or > 50"));
		} else {
			current_field->redefines = CB_FIELD (cb_ref (yyvsp[-2]));
			current_field->rename_thru = CB_FIELD (cb_ref (yyvsp[0]));
		}
	}
  }
#line 7896 "parser.c"
    break;

  case 476: /* any_length_clause: ANY LENGTH  */
#line 3234 "parser.y"
  {
	if (current_field->flag_item_based) {
		cb_error (_("BASED and ANY LENGTH are mutually exclusive"));
	} else {
		current_field->flag_any_length = 1;
	}
  }
#line 7908 "parser.c"
    break;

  case 477: /* varying_clause: VARYING  */
#line 3247 "parser.y"
  {
	current_field->flag_varying = 1;
  }
#line 7916 "parser.c"
    break;

  case 479: /* $@25: %empty  */
#line 3258 "parser.y"
  {
	cb_error (_("LOCAL-STORAGE SECTION is not supported"));
	current_storage = CB_STORAGE_LOCAL;
	if (current_program->nested_level) {
		cb_error (_("LOCAL-STORAGE not allowed in nested programs"));
	}
  }
#line 7928 "parser.c"
    break;

  case 480: /* local_storage_section: "LOCAL-STORAGE" SECTION '.' $@25 record_description_list  */
#line 3266 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->local_storage = CB_FIELD (yyvsp[0]);
	}
  }
#line 7938 "parser.c"
    break;

  case 482: /* $@26: %empty  */
#line 3279 "parser.y"
                                { current_storage = CB_STORAGE_LINKAGE; }
#line 7944 "parser.c"
    break;

  case 483: /* linkage_section: LINKAGE SECTION '.' $@26 record_description_list  */
#line 3281 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->linkage_storage = CB_FIELD (yyvsp[0]);
	}
  }
#line 7954 "parser.c"
    break;

  case 485: /* $@27: %empty  */
#line 3294 "parser.y"
  {
	cb_error (_("REPORT SECTION not supported"));
	current_storage = CB_STORAGE_REPORT;
  }
#line 7963 "parser.c"
    break;

  case 492: /* report_description_options: %empty  */
#line 3327 "parser.y"
  {
	cb_warning (_("Report description using defaults"));
  }
#line 7971 "parser.c"
    break;

  case 494: /* report_description_option: _is GLOBAL  */
#line 3335 "parser.y"
  {
	cb_error (_("GLOBAL is not allowed with RD"));
  }
#line 7979 "parser.c"
    break;

  case 503: /* identifier_list: identifier  */
#line 3358 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 7985 "parser.c"
    break;

  case 504: /* identifier_list: identifier_list identifier  */
#line 3359 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 7991 "parser.c"
    break;

  case 526: /* report_group_option: type_clause  */
#line 3415 "parser.y"
              { cb_warning (_("looking for Report line TYPE")); }
#line 7997 "parser.c"
    break;

  case 577: /* $@28: %empty  */
#line 3520 "parser.y"
                                { current_storage = CB_STORAGE_SCREEN; }
#line 8003 "parser.c"
    break;

  case 578: /* screen_section: SCREEN SECTION '.' $@28  */
#line 3521 "parser.y"
  {
	cb_error (_("SCREEN SECTION is not supported"));
  }
#line 8011 "parser.c"
    break;

  case 580: /* $@29: %empty  */
#line 3532 "parser.y"
  {
	/* SQLCA は実際の埋め込み SQL を最初に検出した時点で注入する
	   (esql_inject_sqlca。下記 exec_sql_statement を参照)。
	   ここでは注入しない: INCLUDE SQLCA や DECLARE SECTION だけで
	   実 SQL の無いプログラムには SQLCA を入れないため。 */
	current_section = NULL;
	current_paragraph = NULL;
	cb_define_system_name ("CONSOLE");
	cb_define_system_name ("SYSIN");
	cb_define_system_name ("SYSOUT");
	cb_define_system_name ("SYSERR");
	cb_set_in_procedure ();
  }
#line 8029 "parser.c"
    break;

  case 581: /* $@30: %empty  */
#line 3546 "parser.y"
  {
	if (current_program->flag_main && !current_program->flag_chained && yyvsp[-4]) {
		cb_error (_("Executable program requested but PROCEDURE/ENTRY has USING clause"));
	}
	emit_entry (current_program->program_id, 0, yyvsp[-4]); /* main entry point */
	if (current_program->source_name) {
		emit_entry (current_program->source_name, 1, yyvsp[-4]);
	}
  }
#line 8043 "parser.c"
    break;

  case 582: /* procedure_division: PROCEDURE DIVISION procedure_using_chaining procedure_returning '.' $@29 procedure_declaratives $@30 procedure_list  */
#line 3556 "parser.y"
  {
	if (current_paragraph) {
		if (current_paragraph->exit_label) {
			emit_statement (current_paragraph->exit_label);
		}
		emit_statement (cb_build_perform_exit (current_paragraph));
	}
	if (current_section) {
		if (current_section->exit_label) {
			emit_statement (current_section->exit_label);
		}
		emit_statement (cb_build_perform_exit (current_section));
	}
  }
#line 8062 "parser.c"
    break;

  case 583: /* procedure_using_chaining: %empty  */
#line 3573 "parser.y"
                                { yyval = NULL; }
#line 8068 "parser.c"
    break;

  case 584: /* $@31: %empty  */
#line 3575 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
	size_mode = CB_SIZE_4;
  }
#line 8077 "parser.c"
    break;

  case 585: /* procedure_using_chaining: USING $@31 procedure_param_list  */
#line 3579 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8083 "parser.c"
    break;

  case 586: /* $@32: %empty  */
#line 3581 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
	current_program->flag_chained = 1;
  }
#line 8092 "parser.c"
    break;

  case 587: /* procedure_using_chaining: CHAINING $@32 procedure_param_list  */
#line 3585 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8098 "parser.c"
    break;

  case 588: /* procedure_param_list: procedure_param  */
#line 3589 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8104 "parser.c"
    break;

  case 589: /* procedure_param_list: procedure_param_list procedure_param  */
#line 3591 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 8110 "parser.c"
    break;

  case 590: /* procedure_param: procedure_type size_optional procedure_optional "Identifier"  */
#line 3596 "parser.y"
  {
	yyval = cb_build_pair (cb_int (call_mode), cb_build_identifier (yyvsp[0]));
	CB_SIZES (yyval) = size_mode;
  }
#line 8119 "parser.c"
    break;

  case 592: /* procedure_type: _by REFERENCE  */
#line 3605 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
  }
#line 8127 "parser.c"
    break;

  case 593: /* procedure_type: _by VALUE  */
#line 3609 "parser.y"
  {
	if (current_program->flag_chained) {
		cb_error (_("BY VALUE not allowed in CHAINED program"));
	} else {
		call_mode = CB_CALL_BY_VALUE;
	}
  }
#line 8139 "parser.c"
    break;

  case 595: /* size_optional: SIZE _is AUTO  */
#line 3621 "parser.y"
  {
	if (call_mode != CB_CALL_BY_VALUE) {
		cb_error (_("SIZE only allowed for BY VALUE items"));
	} else {
		size_mode = CB_SIZE_AUTO;
	}
  }
#line 8151 "parser.c"
    break;

  case 596: /* size_optional: SIZE _is DEFAULT  */
#line 3629 "parser.y"
  {
	if (call_mode != CB_CALL_BY_VALUE) {
		cb_error (_("SIZE only allowed for BY VALUE items"));
	} else {
		size_mode = CB_SIZE_4;
	}
  }
#line 8163 "parser.c"
    break;

  case 597: /* size_optional: UNSIGNED SIZE _is integer  */
#line 3637 "parser.y"
  {
	unsigned char *s = CB_LITERAL (yyvsp[0])->data;

	if (call_mode != CB_CALL_BY_VALUE) {
		cb_error (_("SIZE only allowed for BY VALUE items"));
	} else if (CB_LITERAL (yyvsp[0])->size != 1) {
		cb_error_x (yyvsp[0], _("Invalid value for SIZE"));
	} else {
		size_mode = CB_SIZE_UNSIGNED;
		switch (*s) {
		case '1':
			size_mode |= CB_SIZE_1;
			break;
		case '2':
			size_mode |= CB_SIZE_2;
			break;
		case '4':
			size_mode |= CB_SIZE_4;
			break;
		case '8':
			size_mode |= CB_SIZE_8;
			break;
		default:
			cb_error_x (yyvsp[0], _("Invalid value for SIZE"));
			break;
		}
	}
  }
#line 8196 "parser.c"
    break;

  case 598: /* size_optional: SIZE _is integer  */
#line 3666 "parser.y"
  {
	unsigned char *s = CB_LITERAL (yyvsp[0])->data;

	if (call_mode != CB_CALL_BY_VALUE) {
		cb_error (_("SIZE only allowed for BY VALUE items"));
	} else if (CB_LITERAL (yyvsp[0])->size != 1) {
		cb_error_x (yyvsp[0], _("Invalid value for SIZE"));
	} else {
		size_mode = 0;
		switch (*s) {
		case '1':
			size_mode = CB_SIZE_1;
			break;
		case '2':
			size_mode = CB_SIZE_2;
			break;
		case '4':
			size_mode = CB_SIZE_4;
			break;
		case '8':
			size_mode = CB_SIZE_8;
			break;
		default:
			cb_error_x (yyvsp[0], _("Invalid value for SIZE"));
			break;
		}
	}
  }
#line 8229 "parser.c"
    break;

  case 600: /* procedure_optional: OPTIONAL  */
#line 3699 "parser.y"
  {
	if (call_mode != CB_CALL_BY_REFERENCE) {
		cb_error (_("OPTIONAL only allowed for BY REFERENCE items"));
	}
  }
#line 8239 "parser.c"
    break;

  case 601: /* procedure_returning: %empty  */
#line 3708 "parser.y"
  {
	if (current_program->prog_type == CB_FUNCTION_TYPE) {
		cb_error (_("RETURNING clause is required for a FUNCTION"));
	}
  }
#line 8249 "parser.c"
    break;

  case 602: /* procedure_returning: RETURNING "Identifier"  */
#line 3714 "parser.y"
  {
	if (cb_ref (yyvsp[0]) != cb_error_node) {
		current_program->returning = yyvsp[0];
		if (cb_field (yyvsp[0])->storage != CB_STORAGE_LINKAGE) {
			cb_error (_("RETURNING item is not defined in LINKAGE SECTION"));
		}
	}
  }
#line 8262 "parser.c"
    break;

  case 604: /* $@33: %empty  */
#line 3725 "parser.y"
                        { in_declaratives = 1; }
#line 8268 "parser.c"
    break;

  case 605: /* procedure_declaratives: DECLARATIVES '.' $@33 procedure_list END DECLARATIVES '.'  */
#line 3728 "parser.y"
  {
	in_declaratives = 0;
	if (current_paragraph) {
		if (current_paragraph->exit_label) {
			emit_statement (current_paragraph->exit_label);
		}
		emit_statement (cb_build_perform_exit (current_paragraph));
		current_paragraph = NULL;
	}
	if (current_section) {
		if (current_section->exit_label) {
			emit_statement (current_section->exit_label);
		}
		emit_statement (cb_build_perform_exit (current_section));
		current_section = NULL;
	}
  }
#line 8290 "parser.c"
    break;

  case 611: /* procedure: statements '.'  */
#line 3761 "parser.y"
  {
	if (next_label_list) {
		cb_tree label;
		char name[16];

		sprintf (name, "L$%d", next_label_id);
		label = cb_build_reference (name);
		emit_statement (cb_build_label (label, current_section));
		current_program->label_list =
			cb_list_append (current_program->label_list, next_label_list);
		next_label_list = NULL;
		next_label_id++;
	}
	/* check_unreached = 0; */
  }
#line 8310 "parser.c"
    break;

  case 612: /* procedure: error  */
#line 3777 "parser.y"
  {
	check_unreached = 0;
  }
#line 8318 "parser.c"
    break;

  case 613: /* section_header: section_name SECTION opt_segment '.'  */
#line 3789 "parser.y"
  {
	non_const_word = 0;
	check_unreached = 0;
	if (yyvsp[-3] == cb_error_node) {
		YYERROR;
	}

	/* Exit the last section */
	if (current_paragraph) {
		if (current_paragraph->exit_label) {
			emit_statement (current_paragraph->exit_label);
		}
		emit_statement (cb_build_perform_exit (current_paragraph));
	}
	if (current_section) {
		if (current_section->exit_label) {
			emit_statement (current_section->exit_label);
		}
		emit_statement (cb_build_perform_exit (current_section));
	}

	/* Begin a new section */
	current_section = CB_LABEL (cb_build_label (yyvsp[-3], NULL));
	current_section->is_section = 1;
	current_paragraph = NULL;
	emit_statement (CB_TREE (current_section));
  }
#line 8350 "parser.c"
    break;

  case 614: /* paragraph_header: "Identifier" '.'  */
#line 3820 "parser.y"
  {
	cb_tree label;

	non_const_word = 0;
	check_unreached = 0;
	yyval = cb_build_section_name (yyvsp[-1], 1);
	/* if ($1 == cb_error_node) */
	if (yyval == cb_error_node) {
		YYERROR;
	}

	/* Exit the last paragraph */
	if (current_paragraph) {
		if (current_paragraph->exit_label) {
			emit_statement (current_paragraph->exit_label);
		}
		emit_statement (cb_build_perform_exit (current_paragraph));
	}

	/* Begin a new paragraph */
	if (!current_section) {
		label = cb_build_reference ("MAIN SECTION");
		current_section = CB_LABEL (cb_build_label (label, NULL));
		current_section->is_section = 1;
		emit_statement (CB_TREE (current_section));
	}
	current_paragraph = CB_LABEL (cb_build_label (yyval, current_section));
	if (current_section) {
		current_section->children =
			cb_cons (CB_TREE (current_paragraph), current_section->children);
	}
	emit_statement (CB_TREE (current_paragraph));
  }
#line 8388 "parser.c"
    break;

  case 615: /* invalid_statement: section_name  */
#line 3857 "parser.y"
  {
	non_const_word = 0;
	check_unreached = 0;
	if (yyvsp[0] != cb_error_node) {
		cb_error_x (yyvsp[0], _("Unknown statement '%s'"), CB_NAME (yyvsp[0]));
	}
	YYERROR;
  }
#line 8401 "parser.c"
    break;

  case 616: /* section_name: "Identifier"  */
#line 3868 "parser.y"
                                { yyval = cb_build_section_name (yyvsp[0], 0); }
#line 8407 "parser.c"
    break;

  case 618: /* opt_segment: "Literal"  */
#line 3872 "parser.y"
                                { /* ignore */ }
#line 8413 "parser.c"
    break;

  case 619: /* @34: %empty  */
#line 3881 "parser.y"
  {
	yyval = current_program->exec_list;
	current_program->exec_list = NULL;
  }
#line 8422 "parser.c"
    break;

  case 620: /* @35: %empty  */
#line 3885 "parser.y"
  {
	yyval = CB_TREE (current_statement);
	current_statement = NULL;
  }
#line 8431 "parser.c"
    break;

  case 621: /* statement_list: @34 @35 statements  */
#line 3890 "parser.y"
  {
	yyval = cb_list_reverse (current_program->exec_list);
	current_program->exec_list = yyvsp[-2];
	current_statement = CB_STATEMENT (yyvsp[-1]);
  }
#line 8441 "parser.c"
    break;

  case 622: /* statements: %empty  */
#line 3898 "parser.y"
  {
	cb_tree label;

	if (!current_section) {
		label = cb_build_reference ("MAIN");
		current_section = CB_LABEL (cb_build_label (label, NULL));
		current_section->is_section = 1;
		emit_statement (CB_TREE (current_section));
	}
	if (!current_paragraph) {
		const char* suffix = "_SECTION__DEFAULT_PARAGRAPH";
		char *label_name = malloc(strlen((char*)current_section->name) + strlen(suffix) + 1);
		sprintf(label_name, "%s%s", current_section->name, suffix);
		label = cb_build_reference (label_name);
		current_paragraph = CB_LABEL (cb_build_label (label, NULL));
		emit_statement (CB_TREE (current_paragraph));
		current_section->children =
			cb_cons (CB_TREE (current_paragraph), current_section->children);
	}
  }
#line 8466 "parser.c"
    break;

  case 675: /* statement: "NEXT SENTENCE"  */
#line 3975 "parser.y"
  {
	if (cb_verify (cb_next_sentence_phrase, "NEXT SENTENCE")) {
		cb_tree label;
		char	name[16];

		BEGIN_STATEMENT ("NEXT SENTENCE", 0);
		sprintf (name, "L$%d", next_label_id);
		label = cb_build_reference (name);
		next_label_list = cb_list_add (next_label_list, label);
		emit_statement (cb_build_goto (label, NULL));
	}
	check_unreached = 0;
  }
#line 8484 "parser.c"
    break;

  case 676: /* $@36: %empty  */
#line 3997 "parser.y"
  {
	BEGIN_STATEMENT ("ACCEPT", TERM_ACCEPT);
	dispattrs = 0;
	fgc = NULL;
	bgc = NULL;
	scroll = NULL;
  }
#line 8496 "parser.c"
    break;

  case 678: /* accept_body: identifier opt_at_line_column opt_accp_attr on_accp_exception  */
#line 4010 "parser.y"
  {
	cb_emit_accept (yyvsp[-3], yyvsp[-2], fgc, bgc, scroll, dispattrs);
  }
#line 8504 "parser.c"
    break;

  case 679: /* accept_body: identifier FROM ESCAPE KEY  */
#line 4014 "parser.y"
  {
	PENDING ("ACCEPT .. FROM ESCAPE KEY");
  }
#line 8512 "parser.c"
    break;

  case 680: /* accept_body: identifier FROM LINES  */
#line 4018 "parser.y"
  {
	cb_emit_accept_line_or_col (yyvsp[-2], 0);
  }
#line 8520 "parser.c"
    break;

  case 681: /* accept_body: identifier FROM COLUMNS  */
#line 4022 "parser.y"
  {
	cb_emit_accept_line_or_col (yyvsp[-2], 1);
  }
#line 8528 "parser.c"
    break;

  case 682: /* accept_body: identifier FROM DATE  */
#line 4026 "parser.y"
  {
	cb_emit_accept_date (yyvsp[-2]);
  }
#line 8536 "parser.c"
    break;

  case 683: /* accept_body: identifier FROM DATE YYYYMMDD  */
#line 4030 "parser.y"
  {
	cb_emit_accept_date_yyyymmdd (yyvsp[-3]);
  }
#line 8544 "parser.c"
    break;

  case 684: /* accept_body: identifier FROM DAY  */
#line 4034 "parser.y"
  {
	cb_emit_accept_day (yyvsp[-2]);
  }
#line 8552 "parser.c"
    break;

  case 685: /* accept_body: identifier FROM DAY YYYYDDD  */
#line 4038 "parser.y"
  {
	cb_emit_accept_day_yyyyddd (yyvsp[-3]);
  }
#line 8560 "parser.c"
    break;

  case 686: /* accept_body: identifier FROM "DAY-OF-WEEK"  */
#line 4042 "parser.y"
  {
	cb_emit_accept_day_of_week (yyvsp[-2]);
  }
#line 8568 "parser.c"
    break;

  case 687: /* accept_body: identifier FROM TIME  */
#line 4046 "parser.y"
  {
	cb_emit_accept_time (yyvsp[-2]);
  }
#line 8576 "parser.c"
    break;

  case 688: /* accept_body: identifier FROM "COMMAND-LINE"  */
#line 4050 "parser.y"
  {
	cb_emit_accept_command_line (yyvsp[-2]);
  }
#line 8584 "parser.c"
    break;

  case 689: /* accept_body: identifier FROM "ENVIRONMENT-VALUE" on_accp_exception  */
#line 4054 "parser.y"
  {
	cb_emit_accept_environment (yyvsp[-3]);
  }
#line 8592 "parser.c"
    break;

  case 690: /* accept_body: identifier FROM ENVIRONMENT simple_value on_accp_exception  */
#line 4058 "parser.y"
  { 
	cb_emit_get_environment (yyvsp[-1], yyvsp[-4]);
  }
#line 8600 "parser.c"
    break;

  case 691: /* accept_body: identifier FROM "ARGUMENT-NUMBER"  */
#line 4062 "parser.y"
  {
	cb_emit_accept_arg_number (yyvsp[-2]);
  }
#line 8608 "parser.c"
    break;

  case 692: /* accept_body: identifier FROM "ARGUMENT-VALUE" on_accp_exception  */
#line 4066 "parser.y"
  {
	cb_emit_accept_arg_value (yyvsp[-3]);
  }
#line 8616 "parser.c"
    break;

  case 693: /* accept_body: identifier FROM mnemonic_name  */
#line 4070 "parser.y"
  {
	cb_emit_accept_mnemonic (yyvsp[-2], yyvsp[0]);
  }
#line 8624 "parser.c"
    break;

  case 694: /* accept_body: identifier FROM "Identifier"  */
#line 4074 "parser.y"
  {
	cb_emit_accept_name (yyvsp[-2], yyvsp[0]);
  }
#line 8632 "parser.c"
    break;

  case 695: /* opt_at_line_column: %empty  */
#line 4080 "parser.y"
                                { yyval = NULL; }
#line 8638 "parser.c"
    break;

  case 696: /* opt_at_line_column: _at line_number column_number  */
#line 4081 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-1], yyvsp[0]); }
#line 8644 "parser.c"
    break;

  case 697: /* opt_at_line_column: _at column_number line_number  */
#line 4082 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], yyvsp[-1]); }
#line 8650 "parser.c"
    break;

  case 698: /* opt_at_line_column: _at line_number  */
#line 4083 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], NULL); }
#line 8656 "parser.c"
    break;

  case 699: /* opt_at_line_column: _at column_number  */
#line 4084 "parser.y"
                                { yyval = cb_build_pair (NULL, yyvsp[0]); }
#line 8662 "parser.c"
    break;

  case 700: /* opt_at_line_column: AT simple_value  */
#line 4085 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8668 "parser.c"
    break;

  case 701: /* line_number: LINE _number id_or_lit  */
#line 4089 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8674 "parser.c"
    break;

  case 702: /* column_number: COLUMN _number id_or_lit  */
#line 4093 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8680 "parser.c"
    break;

  case 703: /* column_number: POSITION _number id_or_lit  */
#line 4094 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8686 "parser.c"
    break;

  case 708: /* accp_attr: BELL  */
#line 4107 "parser.y"
                { dispattrs |= COB_SCREEN_BELL; }
#line 8692 "parser.c"
    break;

  case 709: /* accp_attr: BLINK  */
#line 4108 "parser.y"
                { dispattrs |= COB_SCREEN_BLINK; }
#line 8698 "parser.c"
    break;

  case 710: /* accp_attr: HIGHLIGHT  */
#line 4109 "parser.y"
                { dispattrs |= COB_SCREEN_HIGHLIGHT; }
#line 8704 "parser.c"
    break;

  case 711: /* accp_attr: LOWLIGHT  */
#line 4110 "parser.y"
                { dispattrs |= COB_SCREEN_LOWLIGHT; }
#line 8710 "parser.c"
    break;

  case 712: /* accp_attr: "REVERSE-VIDEO"  */
#line 4111 "parser.y"
                { dispattrs |= COB_SCREEN_REVERSE; }
#line 8716 "parser.c"
    break;

  case 713: /* accp_attr: UNDERLINE  */
#line 4112 "parser.y"
                { dispattrs |= COB_SCREEN_UNDERLINE; }
#line 8722 "parser.c"
    break;

  case 714: /* accp_attr: OVERLINE  */
#line 4113 "parser.y"
                { dispattrs |= COB_SCREEN_OVERLINE; }
#line 8728 "parser.c"
    break;

  case 715: /* accp_attr: "FOREGROUND-COLOR" _is num_id_or_lit  */
#line 4115 "parser.y"
  {
	fgc = yyvsp[0];
  }
#line 8736 "parser.c"
    break;

  case 716: /* accp_attr: "BACKGROUND-COLOR" _is num_id_or_lit  */
#line 4119 "parser.y"
  {
	bgc = yyvsp[0];
  }
#line 8744 "parser.c"
    break;

  case 717: /* accp_attr: SCROLL UP _opt_scroll_lines  */
#line 4123 "parser.y"
  {
	scroll = yyvsp[0];
  }
#line 8752 "parser.c"
    break;

  case 718: /* accp_attr: SCROLL DOWN _opt_scroll_lines  */
#line 4127 "parser.y"
  {
	dispattrs |= COB_SCREEN_SCROLL_DOWN;
	scroll = yyvsp[0];
  }
#line 8761 "parser.c"
    break;

  case 719: /* accp_attr: AUTO  */
#line 4131 "parser.y"
                { dispattrs |= COB_SCREEN_AUTO; }
#line 8767 "parser.c"
    break;

  case 720: /* accp_attr: FULL  */
#line 4132 "parser.y"
                { dispattrs |= COB_SCREEN_FULL; }
#line 8773 "parser.c"
    break;

  case 721: /* accp_attr: REQUIRED  */
#line 4133 "parser.y"
                { dispattrs |= COB_SCREEN_REQUIRED; }
#line 8779 "parser.c"
    break;

  case 722: /* accp_attr: SECURE  */
#line 4134 "parser.y"
                { dispattrs |= COB_SCREEN_SECURE; }
#line 8785 "parser.c"
    break;

  case 723: /* accp_attr: UPDATE  */
#line 4135 "parser.y"
                { dispattrs |= COB_SCREEN_UPDATE; }
#line 8791 "parser.c"
    break;

  case 724: /* accp_attr: PROMPT  */
#line 4136 "parser.y"
                { dispattrs |= COB_SCREEN_PROMPT; }
#line 8797 "parser.c"
    break;

  case 725: /* end_accept: %empty  */
#line 4140 "parser.y"
                                { terminator_warning (TERM_ACCEPT); }
#line 8803 "parser.c"
    break;

  case 726: /* end_accept: "END-ACCEPT"  */
#line 4141 "parser.y"
                                { terminator_clear (TERM_ACCEPT); }
#line 8809 "parser.c"
    break;

  case 727: /* $@37: %empty  */
#line 4150 "parser.y"
                                { BEGIN_STATEMENT ("ADD", TERM_ADD); }
#line 8815 "parser.c"
    break;

  case 729: /* add_body: x_list TO arithmetic_x_list on_size_error  */
#line 4157 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '+', cb_build_binary_list (yyvsp[-3], '+'));
  }
#line 8823 "parser.c"
    break;

  case 730: /* add_body: x_list add_to GIVING arithmetic_x_list on_size_error  */
#line 4161 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_list (yyvsp[-4], '+'));
  }
#line 8831 "parser.c"
    break;

  case 731: /* add_body: CORRESPONDING identifier TO identifier flag_rounded on_size_error  */
#line 4165 "parser.y"
  {
	cb_emit_corresponding (cb_build_add, yyvsp[-2], yyvsp[-4], yyvsp[-1]);
  }
#line 8839 "parser.c"
    break;

  case 733: /* add_to: TO x  */
#line 4171 "parser.y"
                                { cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 8845 "parser.c"
    break;

  case 734: /* end_add: %empty  */
#line 4175 "parser.y"
                                { terminator_warning (TERM_ADD); }
#line 8851 "parser.c"
    break;

  case 735: /* end_add: "END-ADD"  */
#line 4176 "parser.y"
                                { terminator_clear (TERM_ADD); }
#line 8857 "parser.c"
    break;

  case 736: /* $@38: %empty  */
#line 4185 "parser.y"
                                { BEGIN_STATEMENT ("ALLOCATE", 0); }
#line 8863 "parser.c"
    break;

  case 738: /* allocate_body: "Identifier" flag_initialized allocate_returning  */
#line 4191 "parser.y"
  {
	cb_emit_allocate (yyvsp[-2], yyvsp[0], NULL, yyvsp[-1]);
  }
#line 8871 "parser.c"
    break;

  case 739: /* allocate_body: expr CHARACTERS flag_initialized RETURNING target_x  */
#line 4195 "parser.y"
  {
	cb_emit_allocate (NULL, yyvsp[0], yyvsp[-4], yyvsp[-2]);
  }
#line 8879 "parser.c"
    break;

  case 740: /* allocate_returning: %empty  */
#line 4201 "parser.y"
                                { yyval = NULL; }
#line 8885 "parser.c"
    break;

  case 741: /* allocate_returning: RETURNING target_x  */
#line 4202 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8891 "parser.c"
    break;

  case 742: /* alter_statement: ALTER alter_options  */
#line 4212 "parser.y"
  {
	cb_error (_("ALTER statement is obsolete and unsupported"));
  }
#line 8899 "parser.c"
    break;

  case 747: /* $@39: %empty  */
#line 4230 "parser.y"
                                { BEGIN_STATEMENT ("CALL", TERM_CALL); }
#line 8905 "parser.c"
    break;

  case 748: /* call_statement: CALL $@39 id_or_lit_or_func call_using call_returning call_on_exception call_not_on_exception end_call  */
#line 4234 "parser.y"
  {
	cb_emit_call (yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1]);
  }
#line 8913 "parser.c"
    break;

  case 749: /* call_using: %empty  */
#line 4240 "parser.y"
                                { yyval = NULL; }
#line 8919 "parser.c"
    break;

  case 750: /* $@40: %empty  */
#line 4242 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
	size_mode = CB_SIZE_4;
  }
#line 8928 "parser.c"
    break;

  case 751: /* call_using: USING $@40 call_param_list  */
#line 4246 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8934 "parser.c"
    break;

  case 752: /* call_param_list: call_param  */
#line 4250 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8940 "parser.c"
    break;

  case 753: /* call_param_list: call_param_list call_param  */
#line 4252 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 8946 "parser.c"
    break;

  case 754: /* call_param: call_type OMITTED  */
#line 4257 "parser.y"
  {
	if (call_mode != CB_CALL_BY_REFERENCE) {
		cb_error (_("OMITTED only allowed with BY REFERENCE"));
	}
	yyval = cb_build_pair (cb_int (call_mode), cb_null);
  }
#line 8957 "parser.c"
    break;

  case 755: /* call_param: call_type size_optional x  */
#line 4264 "parser.y"
  {
	yyval = cb_build_pair (cb_int (call_mode), yyvsp[0]);
	CB_SIZES (yyval) = size_mode;
  }
#line 8966 "parser.c"
    break;

  case 757: /* call_type: _by REFERENCE  */
#line 4273 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
  }
#line 8974 "parser.c"
    break;

  case 758: /* call_type: _by CONTENT  */
#line 4277 "parser.y"
  {
	if (current_program->flag_chained) {
		cb_error (_("BY CONTENT not allowed in CHAINED program"));
	} else {
		call_mode = CB_CALL_BY_CONTENT;
	}
  }
#line 8986 "parser.c"
    break;

  case 759: /* call_type: _by VALUE  */
#line 4285 "parser.y"
  {
	if (current_program->flag_chained) {
		cb_error (_("BY VALUE not allowed in CHAINED program"));
	} else {
		call_mode = CB_CALL_BY_VALUE;
	}
  }
#line 8998 "parser.c"
    break;

  case 760: /* call_returning: %empty  */
#line 4295 "parser.y"
                                { yyval = NULL; }
#line 9004 "parser.c"
    break;

  case 761: /* call_returning: RETURNING identifier  */
#line 4296 "parser.y"
                                { yyval = yyvsp[0]; }
#line 9010 "parser.c"
    break;

  case 762: /* call_returning: GIVING identifier  */
#line 4297 "parser.y"
                                { yyval = yyvsp[0]; }
#line 9016 "parser.c"
    break;

  case 763: /* call_on_exception: %empty  */
#line 4302 "parser.y"
  {
	yyval = NULL;
  }
#line 9024 "parser.c"
    break;

  case 764: /* $@41: %empty  */
#line 4306 "parser.y"
  {
	check_unreached = 0;
  }
#line 9032 "parser.c"
    break;

  case 765: /* call_on_exception: exception_or_overflow $@41 statement_list  */
#line 4310 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 9040 "parser.c"
    break;

  case 766: /* call_not_on_exception: %empty  */
#line 4317 "parser.y"
  {
	yyval = NULL;
  }
#line 9048 "parser.c"
    break;

  case 767: /* $@42: %empty  */
#line 4321 "parser.y"
  {
	check_unreached = 0;
  }
#line 9056 "parser.c"
    break;

  case 768: /* call_not_on_exception: not_exception_or_overflow $@42 statement_list  */
#line 4325 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 9064 "parser.c"
    break;

  case 769: /* end_call: %empty  */
#line 4331 "parser.y"
                                { terminator_warning (TERM_CALL); }
#line 9070 "parser.c"
    break;

  case 770: /* end_call: "END-CALL"  */
#line 4332 "parser.y"
                                { terminator_clear (TERM_CALL); }
#line 9076 "parser.c"
    break;

  case 771: /* $@43: %empty  */
#line 4341 "parser.y"
                                { BEGIN_STATEMENT ("CANCEL", 0); }
#line 9082 "parser.c"
    break;

  case 774: /* cancel_list: cancel_list id_or_lit  */
#line 4347 "parser.y"
  {
	cb_emit_cancel (yyvsp[0]);
  }
#line 9090 "parser.c"
    break;

  case 775: /* cancel_list: ALL  */
#line 4351 "parser.y"
  {
	cb_emit_cancel_all ();
  }
#line 9098 "parser.c"
    break;

  case 776: /* $@44: %empty  */
#line 4362 "parser.y"
                                { BEGIN_STATEMENT ("CLOSE", 0); }
#line 9104 "parser.c"
    break;

  case 779: /* close_list: close_list file_name close_option  */
#line 4369 "parser.y"
  {
	BEGIN_IMPLICIT_STATEMENT (yyvsp[-1]);
	if (yyvsp[-1] != cb_error_node) {
		cb_emit_close (yyvsp[-1], yyvsp[0]);
	}
  }
#line 9115 "parser.c"
    break;

  case 780: /* close_option: %empty  */
#line 4378 "parser.y"
                                { yyval = cb_int (COB_CLOSE_NORMAL); }
#line 9121 "parser.c"
    break;

  case 781: /* close_option: reel_or_unit  */
#line 4379 "parser.y"
                                { yyval = cb_int (COB_CLOSE_UNIT); }
#line 9127 "parser.c"
    break;

  case 782: /* close_option: reel_or_unit _for REMOVAL  */
#line 4380 "parser.y"
                                { yyval = cb_int (COB_CLOSE_UNIT_REMOVAL); }
#line 9133 "parser.c"
    break;

  case 783: /* close_option: _with NO REWIND  */
#line 4381 "parser.y"
                                { yyval = cb_int (COB_CLOSE_NO_REWIND); }
#line 9139 "parser.c"
    break;

  case 784: /* close_option: _with LOCK  */
#line 4382 "parser.y"
                                { yyval = cb_int (COB_CLOSE_LOCK); }
#line 9145 "parser.c"
    break;

  case 787: /* $@45: %empty  */
#line 4393 "parser.y"
                                { BEGIN_STATEMENT ("COMPUTE", TERM_COMPUTE); }
#line 9151 "parser.c"
    break;

  case 789: /* compute_body: arithmetic_x_list comp_equal expr on_size_error  */
#line 4400 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-3], 0, yyvsp[-1]);
  }
#line 9159 "parser.c"
    break;

  case 790: /* end_compute: %empty  */
#line 4406 "parser.y"
                                { terminator_warning (TERM_COMPUTE); }
#line 9165 "parser.c"
    break;

  case 791: /* end_compute: "END-COMPUTE"  */
#line 4407 "parser.y"
                                { terminator_clear (TERM_COMPUTE); }
#line 9171 "parser.c"
    break;

  case 794: /* commit_statement: COMMIT  */
#line 4418 "parser.y"
  {
	BEGIN_STATEMENT ("COMMIT", 0);
	cb_emit_commit ();
  }
#line 9180 "parser.c"
    break;

  case 795: /* continue_statement: CONTINUE  */
#line 4431 "parser.y"
  {
	BEGIN_STATEMENT ("CONTINUE", 0);
	cb_emit_continue ();
  }
#line 9189 "parser.c"
    break;

  case 796: /* $@46: %empty  */
#line 4443 "parser.y"
                                { BEGIN_STATEMENT ("DELETE", TERM_DELETE); }
#line 9195 "parser.c"
    break;

  case 797: /* delete_statement: DELETE $@46 file_name _record opt_invalid_key end_delete  */
#line 4446 "parser.y"
  {
	if (yyvsp[-3] != cb_error_node) {
		cb_emit_delete (yyvsp[-3]);
	}
  }
#line 9205 "parser.c"
    break;

  case 798: /* end_delete: %empty  */
#line 4454 "parser.y"
                                { terminator_warning (TERM_DELETE); }
#line 9211 "parser.c"
    break;

  case 799: /* end_delete: "END-DELETE"  */
#line 4455 "parser.y"
                                { terminator_clear (TERM_DELETE); }
#line 9217 "parser.c"
    break;

  case 800: /* $@47: %empty  */
#line 4464 "parser.y"
                                  { BEGIN_STATEMENT ("DELETE-FILE", 0); }
#line 9223 "parser.c"
    break;

  case 801: /* delete_file_statement: DELETE $@47 "FILE" file_name_list  */
#line 4466 "parser.y"
  {
	cb_tree l;
	for (l = yyvsp[0]; l; l = CB_CHAIN (l)) {
		if (CB_VALUE (l) != cb_error_node) {
			BEGIN_IMPLICIT_STATEMENT (l);
			cb_emit_delete_file (CB_VALUE (l));
		}
	}
  }
#line 9237 "parser.c"
    break;

  case 802: /* $@48: %empty  */
#line 4484 "parser.y"
  {
	BEGIN_STATEMENT ("DISPLAY", TERM_DISPLAY);
	dispattrs = 0;
	fgc = NULL;
	bgc = NULL;
	scroll = NULL;
  }
#line 9249 "parser.c"
    break;

  case 804: /* display_body: id_or_lit "UPON ENVIRONMENT-NAME" on_disp_exception  */
#line 4497 "parser.y"
  {
	cb_emit_env_name (yyvsp[-2]);
  }
#line 9257 "parser.c"
    break;

  case 805: /* display_body: id_or_lit "UPON ENVIRONMENT-VALUE" on_disp_exception  */
#line 4501 "parser.y"
  {
	cb_emit_env_value (yyvsp[-2]);
  }
#line 9265 "parser.c"
    break;

  case 806: /* display_body: id_or_lit "UPON ARGUMENT-NUMBER" on_disp_exception  */
#line 4505 "parser.y"
  {
	cb_emit_arg_number (yyvsp[-2]);
  }
#line 9273 "parser.c"
    break;

  case 807: /* display_body: id_or_lit "UPON COMMAND-LINE" on_disp_exception  */
#line 4509 "parser.y"
  {
	cb_emit_command_line (yyvsp[-2]);
  }
#line 9281 "parser.c"
    break;

  case 808: /* display_body: x_list opt_at_line_column with_clause on_disp_exception  */
#line 4513 "parser.y"
  {
	cb_emit_display (yyvsp[-3], cb_int0, yyvsp[-1], yyvsp[-2], fgc, bgc, scroll, dispattrs);
  }
#line 9289 "parser.c"
    break;

  case 809: /* display_body: x_list opt_at_line_column UPON mnemonic_name with_clause on_disp_exception  */
#line 4517 "parser.y"
  {
	cb_emit_display_mnemonic (yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9297 "parser.c"
    break;

  case 810: /* display_body: x_list opt_at_line_column UPON "Identifier" with_clause on_disp_exception  */
#line 4521 "parser.y"
  {
	cb_tree word = cb_build_display_upon_direct (yyvsp[-2]);
	cb_emit_display (yyvsp[-5], word, yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9306 "parser.c"
    break;

  case 811: /* display_body: x_list opt_at_line_column UPON PRINTER with_clause on_disp_exception  */
#line 4526 "parser.y"
  {
	cb_emit_display (yyvsp[-5], cb_int0, yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9314 "parser.c"
    break;

  case 812: /* display_body: x_list opt_at_line_column UPON CRT with_clause on_disp_exception  */
#line 4530 "parser.y"
  {
	cb_emit_display (yyvsp[-5], cb_int0, yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9322 "parser.c"
    break;

  case 813: /* with_clause: %empty  */
#line 4536 "parser.y"
                                { yyval = cb_int1; }
#line 9328 "parser.c"
    break;

  case 814: /* with_clause: _with "NO ADVANCING"  */
#line 4537 "parser.y"
                                { yyval = cb_int0; }
#line 9334 "parser.c"
    break;

  case 815: /* with_clause: WITH disp_attrs  */
#line 4538 "parser.y"
                                { yyval = cb_int1; }
#line 9340 "parser.c"
    break;

  case 818: /* disp_attr: BELL  */
#line 4548 "parser.y"
                { dispattrs |= COB_SCREEN_BELL; }
#line 9346 "parser.c"
    break;

  case 819: /* disp_attr: BLINK  */
#line 4549 "parser.y"
                { dispattrs |= COB_SCREEN_BLINK; }
#line 9352 "parser.c"
    break;

  case 820: /* disp_attr: ERASE EOL  */
#line 4550 "parser.y"
                { dispattrs |= COB_SCREEN_ERASE_EOL; }
#line 9358 "parser.c"
    break;

  case 821: /* disp_attr: ERASE EOS  */
#line 4551 "parser.y"
                { dispattrs |= COB_SCREEN_ERASE_EOS; }
#line 9364 "parser.c"
    break;

  case 822: /* disp_attr: HIGHLIGHT  */
#line 4552 "parser.y"
                { dispattrs |= COB_SCREEN_HIGHLIGHT; }
#line 9370 "parser.c"
    break;

  case 823: /* disp_attr: LOWLIGHT  */
#line 4553 "parser.y"
                { dispattrs |= COB_SCREEN_LOWLIGHT; }
#line 9376 "parser.c"
    break;

  case 824: /* disp_attr: "REVERSE-VIDEO"  */
#line 4554 "parser.y"
                { dispattrs |= COB_SCREEN_REVERSE; }
#line 9382 "parser.c"
    break;

  case 825: /* disp_attr: UNDERLINE  */
#line 4555 "parser.y"
                { dispattrs |= COB_SCREEN_UNDERLINE; }
#line 9388 "parser.c"
    break;

  case 826: /* disp_attr: OVERLINE  */
#line 4556 "parser.y"
                { dispattrs |= COB_SCREEN_OVERLINE; }
#line 9394 "parser.c"
    break;

  case 827: /* disp_attr: "FOREGROUND-COLOR" _is num_id_or_lit  */
#line 4558 "parser.y"
  {
	fgc = yyvsp[0];
  }
#line 9402 "parser.c"
    break;

  case 828: /* disp_attr: "BACKGROUND-COLOR" _is num_id_or_lit  */
#line 4562 "parser.y"
  {
	bgc = yyvsp[0];
  }
#line 9410 "parser.c"
    break;

  case 829: /* disp_attr: SCROLL UP _opt_scroll_lines  */
#line 4566 "parser.y"
  {
	scroll = yyvsp[0];
  }
#line 9418 "parser.c"
    break;

  case 830: /* disp_attr: SCROLL DOWN _opt_scroll_lines  */
#line 4570 "parser.y"
  {
	dispattrs |= COB_SCREEN_SCROLL_DOWN;
	scroll = yyvsp[0];
  }
#line 9427 "parser.c"
    break;

  case 831: /* disp_attr: "BLANK-LINE"  */
#line 4574 "parser.y"
                { dispattrs |= COB_SCREEN_BLANK_LINE; }
#line 9433 "parser.c"
    break;

  case 832: /* disp_attr: "BLANK-SCREEN"  */
#line 4575 "parser.y"
                { dispattrs |= COB_SCREEN_BLANK_SCREEN; }
#line 9439 "parser.c"
    break;

  case 833: /* end_display: %empty  */
#line 4579 "parser.y"
                                { terminator_warning (TERM_DISPLAY); }
#line 9445 "parser.c"
    break;

  case 834: /* end_display: "END-DISPLAY"  */
#line 4580 "parser.y"
                                { terminator_clear (TERM_DISPLAY); }
#line 9451 "parser.c"
    break;

  case 835: /* $@49: %empty  */
#line 4589 "parser.y"
                                { BEGIN_STATEMENT ("DIVIDE", TERM_DIVIDE); }
#line 9457 "parser.c"
    break;

  case 837: /* divide_body: x INTO arithmetic_x_list on_size_error  */
#line 4596 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '/', yyvsp[-3]);
  }
#line 9465 "parser.c"
    break;

  case 838: /* divide_body: x INTO x GIVING arithmetic_x_list on_size_error  */
#line 4600 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_op (yyvsp[-3], '/', yyvsp[-5]));
  }
#line 9473 "parser.c"
    break;

  case 839: /* divide_body: x BY x GIVING arithmetic_x_list on_size_error  */
#line 4604 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_op (yyvsp[-5], '/', yyvsp[-3]));
  }
#line 9481 "parser.c"
    break;

  case 840: /* divide_body: x INTO x GIVING arithmetic_x REMAINDER arithmetic_x on_size_error  */
#line 4608 "parser.y"
  {
	cb_emit_divide (yyvsp[-5], yyvsp[-7], yyvsp[-3], yyvsp[-1]);
  }
#line 9489 "parser.c"
    break;

  case 841: /* divide_body: x BY x GIVING arithmetic_x REMAINDER arithmetic_x on_size_error  */
#line 4612 "parser.y"
  {
	cb_emit_divide (yyvsp[-7], yyvsp[-5], yyvsp[-3], yyvsp[-1]);
  }
#line 9497 "parser.c"
    break;

  case 842: /* end_divide: %empty  */
#line 4618 "parser.y"
                                { terminator_warning (TERM_DIVIDE); }
#line 9503 "parser.c"
    break;

  case 843: /* end_divide: "END-DIVIDE"  */
#line 4619 "parser.y"
                                { terminator_clear (TERM_DIVIDE); }
#line 9509 "parser.c"
    break;

  case 844: /* $@50: %empty  */
#line 4628 "parser.y"
                                { BEGIN_STATEMENT ("ENTRY", 0); }
#line 9515 "parser.c"
    break;

  case 845: /* entry_statement: ENTRY $@50 "Literal" call_using  */
#line 4630 "parser.y"
  {
	if (current_program->nested_level) {
		cb_error (_("ENTRY is invalid in nested program"));
	} else if (cb_verify (cb_entry_statement, "ENTRY")) {
		if (cobc_check_valid_name ((char *)(CB_LITERAL (yyvsp[-1])->data))) {
			cb_error (_("ENTRY '%s' invalid"), (char *)(CB_LITERAL (yyvsp[-1])->data));
		}
		emit_entry ((char *)(CB_LITERAL (yyvsp[-1])->data), 1, yyvsp[0]);
	}
	check_unreached = 0;
  }
#line 9531 "parser.c"
    break;

  case 846: /* $@51: %empty  */
#line 4650 "parser.y"
  {
	BEGIN_STATEMENT ("EVALUATE", TERM_EVALUATE);
	eval_level++;
	for (eval_inc = 0; eval_inc < 64; eval_inc++) {
		eval_check[eval_level][eval_inc] = 0;
	}
	eval_inc = 0;
	eval_inc2 = 0;
  }
#line 9545 "parser.c"
    break;

  case 847: /* evaluate_statement: EVALUATE $@51 evaluate_subject_list evaluate_condition_list end_evaluate  */
#line 4661 "parser.y"
  {
	cb_emit_evaluate (yyvsp[-2], yyvsp[-1]);
	eval_level--;
  }
#line 9554 "parser.c"
    break;

  case 848: /* evaluate_subject_list: evaluate_subject  */
#line 4668 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9560 "parser.c"
    break;

  case 849: /* evaluate_subject_list: evaluate_subject_list _also evaluate_subject  */
#line 4671 "parser.y"
  {
 	if (!cb_allow_missing_also_clause_in_evaluate && yyvsp[-1] != cb_int1) {
 		cb_error  (_("Invalid expression"));
 	}
 	yyval = cb_list_add (yyvsp[-2], yyvsp[0]);
  }
#line 9571 "parser.c"
    break;

  case 850: /* evaluate_subject: expr  */
#line 4681 "parser.y"
  {
	yyval = yyvsp[0];
	if (CB_REFERENCE_P (yyvsp[0])) {
		eval_check[eval_level][eval_inc++] = 0;
	} else {
		eval_check[eval_level][eval_inc++] = 1;
	}
  }
#line 9584 "parser.c"
    break;

  case 851: /* evaluate_subject: "TRUE"  */
#line 4690 "parser.y"
  {
	yyval = cb_true;
	eval_check[eval_level][eval_inc++] = 2;
  }
#line 9593 "parser.c"
    break;

  case 852: /* evaluate_subject: "FALSE"  */
#line 4695 "parser.y"
  {
	yyval = cb_false;
	eval_check[eval_level][eval_inc++] = 3;
  }
#line 9602 "parser.c"
    break;

  case 853: /* evaluate_condition_list: evaluate_case_list evaluate_other  */
#line 4703 "parser.y"
  {
	yyval = yyvsp[-1];
	if (yyvsp[0]) {
		if (cb_allow_empty_imperative_statement) {
			/*
			 * some compiler implementation allow empty
			 * imperative statements in WHEN phrases, and
			 * treats WHEN OTHER phrase following that
			 * asif the rest part of when_list belonging
			 * to that.
			 */
			cb_tree l, case_item;
			l = yyval;
			while (CB_CHAIN (l)) {
				l = CB_CHAIN (l);
			}
			case_item = CB_VALUE (l);
			if (!CB_VALUE (case_item)) {
				 /* warning: duplecates ptr. here */
				CB_VALUE (case_item) = CB_VALUE (yyvsp[0]);
			}
		}
		yyval = cb_list_add (yyval, yyvsp[0]);
	}
  }
#line 9632 "parser.c"
    break;

  case 854: /* evaluate_case_list: evaluate_case  */
#line 4731 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9638 "parser.c"
    break;

  case 855: /* evaluate_case_list: evaluate_case_list evaluate_case  */
#line 4733 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 9644 "parser.c"
    break;

  case 856: /* $@52: %empty  */
#line 4738 "parser.y"
  {
	check_unreached = 0;
  }
#line 9652 "parser.c"
    break;

  case 857: /* evaluate_case: evaluate_when_list $@52 statement_list  */
#line 4742 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[0] == NULL) {
		cb_error (_("syntax error"));
	}
	yyval = cb_cons (yyvsp[0], yyvsp[-2]);
	eval_inc2 = 0;
  }
#line 9664 "parser.c"
    break;

  case 858: /* evaluate_other: %empty  */
#line 4753 "parser.y"
  {
	yyval = NULL;
  }
#line 9672 "parser.c"
    break;

  case 859: /* $@53: %empty  */
#line 4757 "parser.y"
  {
	check_unreached = 0;
  }
#line 9680 "parser.c"
    break;

  case 860: /* evaluate_other: "WHEN OTHER" $@53 statement_list  */
#line 4761 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[0] == NULL) {
		cb_error (_("syntax error"));
	}
	yyval = cb_cons (yyvsp[0], NULL);
	eval_inc2 = 0;
  }
#line 9692 "parser.c"
    break;

  case 861: /* evaluate_when_list: WHEN evaluate_object_list  */
#line 4771 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9698 "parser.c"
    break;

  case 862: /* evaluate_when_list: evaluate_when_list WHEN evaluate_object_list  */
#line 4773 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 9704 "parser.c"
    break;

  case 863: /* evaluate_object_list: evaluate_object  */
#line 4777 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9710 "parser.c"
    break;

  case 864: /* evaluate_object_list: evaluate_object_list _also evaluate_object  */
#line 4780 "parser.y"
  {
 	if (!cb_allow_missing_also_clause_in_evaluate && yyvsp[-1] != cb_int1) {
 		cb_error  (_("Invalid expression"));
 	}
 	yyval = cb_list_add (yyvsp[-2], yyvsp[0]);
  }
#line 9721 "parser.c"
    break;

  case 865: /* evaluate_object: partial_expr opt_evaluate_thru_expr  */
#line 4790 "parser.y"
  {
	cb_tree not;
	cb_tree e1;
	cb_tree e2;

	not = cb_int0;
	e2 = yyvsp[0];
	/* in case the first token is NOT */
	if (CB_PURPOSE_INT (yyvsp[-1]) == '!') {
		if (eval_check[eval_level][eval_inc2] < 2) {
			not = cb_int1;
			yyvsp[-1] = CB_CHAIN (yyvsp[-1]);
		}
	}

	/* build expr now */
	e1 = cb_build_expr (yyvsp[-1]);

	if (e2 == NULL) {
		/* WHEN expr */
		eval_inc2++;
		yyval = cb_build_pair (not, cb_build_pair (e1, NULL));
	} else {
		/* WHEN expr THRU expr */
		yyval = cb_build_pair (not, cb_build_pair (e1, e2));
		eval_inc2++;
	}
  }
#line 9754 "parser.c"
    break;

  case 866: /* evaluate_object: ANY  */
#line 4818 "parser.y"
                                { yyval = cb_any; eval_inc2++; }
#line 9760 "parser.c"
    break;

  case 867: /* evaluate_object: "TRUE"  */
#line 4819 "parser.y"
                                { yyval = cb_true; eval_inc2++; }
#line 9766 "parser.c"
    break;

  case 868: /* evaluate_object: "FALSE"  */
#line 4820 "parser.y"
                                { yyval = cb_false; eval_inc2++; }
#line 9772 "parser.c"
    break;

  case 869: /* opt_evaluate_thru_expr: %empty  */
#line 4823 "parser.y"
                                { yyval = NULL; }
#line 9778 "parser.c"
    break;

  case 870: /* opt_evaluate_thru_expr: THRU expr  */
#line 4824 "parser.y"
                                { yyval = yyvsp[0]; }
#line 9784 "parser.c"
    break;

  case 871: /* end_evaluate: %empty  */
#line 4828 "parser.y"
                                { terminator_warning (TERM_EVALUATE); }
#line 9790 "parser.c"
    break;

  case 872: /* end_evaluate: "END-EVALUATE"  */
#line 4829 "parser.y"
                                { terminator_clear (TERM_EVALUATE); }
#line 9796 "parser.c"
    break;

  case 873: /* $@54: %empty  */
#line 4838 "parser.y"
                                { BEGIN_STATEMENT ("EXIT", 0); }
#line 9802 "parser.c"
    break;

  case 875: /* exit_body: %empty  */
#line 4843 "parser.y"
                                { /* nothing */ }
#line 9808 "parser.c"
    break;

  case 876: /* exit_body: PROGRAM  */
#line 4845 "parser.y"
  {
	if (in_declaratives && use_global_ind) {
		cb_error (_("EXIT PROGRAM is not allowed within a USE GLOBAL procedure"));
	}
	check_unreached = 1;
	cb_emit_exit (0);
  }
#line 9820 "parser.c"
    break;

  case 877: /* exit_body: PERFORM  */
#line 4853 "parser.y"
  {
	if (!perform_stack) {
		cb_error (_("EXIT PERFORM is only valid with inline PERFORM"));
	} else {
		cb_emit_java_break ();
	}
  }
#line 9832 "parser.c"
    break;

  case 878: /* exit_body: PERFORM CYCLE  */
#line 4861 "parser.y"
  {
	if (!perform_stack) {
		cb_error (_("EXIT PERFORM is only valid with inline PERFORM"));
	} else {
		cb_emit_java_continue ();
	}
  }
#line 9844 "parser.c"
    break;

  case 879: /* exit_body: SECTION  */
#line 4869 "parser.y"
  {
	cb_tree	plabel;
	char	name[64];

	if (!current_section) {
		cb_error (_("EXIT SECTION is only valid with an active SECTION"));
	} else {
		if (!current_section->exit_label) {
			sprintf (name, "EXIT SECTION %d", cb_id);
			plabel = cb_build_reference (name);
			current_section->exit_label = cb_build_label (plabel, current_section);
			current_section->exit_label_ref = plabel;
			CB_LABEL (current_section->exit_label)->need_begin = 1;
		}
		cb_emit_goto (cb_list_init (current_section->exit_label_ref), NULL);
	}
  }
#line 9866 "parser.c"
    break;

  case 880: /* exit_body: PARAGRAPH  */
#line 4887 "parser.y"
  {
	cb_tree	plabel;
	char	name[64];

	if (!current_paragraph) {
		cb_error (_("EXIT PARAGRAPH is only valid with an active PARAGRAPH"));
	} else {
		if (!current_paragraph->exit_label) {
			sprintf (name, "EXIT PARAGRAPH %d", cb_id);
			plabel = cb_build_reference (name);
			current_paragraph->exit_label = cb_build_label (plabel, current_section);
			current_paragraph->exit_label_ref = plabel;
			CB_LABEL (current_paragraph->exit_label)->need_begin = 1;
		}
		cb_emit_goto (cb_list_init (current_paragraph->exit_label_ref), NULL);
	}
  }
#line 9888 "parser.c"
    break;

  case 881: /* $@55: %empty  */
#line 4911 "parser.y"
                                { BEGIN_STATEMENT ("FREE", 0); }
#line 9894 "parser.c"
    break;

  case 882: /* free_statement: FREE $@55 target_x_list  */
#line 4913 "parser.y"
  {
	cb_emit_free (yyvsp[0]);
  }
#line 9902 "parser.c"
    break;

  case 883: /* $@56: %empty  */
#line 4924 "parser.y"
                                { BEGIN_STATEMENT ("GENERATE", 0); }
#line 9908 "parser.c"
    break;

  case 884: /* generate_statement: GENERATE $@56 identifier  */
#line 4926 "parser.y"
  {
	PENDING("GENERATE");
  }
#line 9916 "parser.c"
    break;

  case 885: /* $@57: %empty  */
#line 4937 "parser.y"
                                { BEGIN_STATEMENT ("GO TO", 0); }
#line 9922 "parser.c"
    break;

  case 886: /* goto_statement: GO _to $@57 procedure_name_list goto_depending  */
#line 4939 "parser.y"
  {
	cb_emit_goto (yyvsp[-1], yyvsp[0]);
  }
#line 9930 "parser.c"
    break;

  case 887: /* goto_depending: %empty  */
#line 4946 "parser.y"
  {
	check_unreached = 1;
	yyval = NULL;
  }
#line 9939 "parser.c"
    break;

  case 888: /* goto_depending: DEPENDING _on identifier  */
#line 4951 "parser.y"
  {
	check_unreached = 0;
	yyval = yyvsp[0];
  }
#line 9948 "parser.c"
    break;

  case 889: /* $@58: %empty  */
#line 4963 "parser.y"
                                { BEGIN_STATEMENT ("GOBACK", 0); }
#line 9954 "parser.c"
    break;

  case 890: /* goback_statement: GOBACK $@58  */
#line 4964 "parser.y"
  {
	check_unreached = 1;
	cb_emit_exit (1);
  }
#line 9963 "parser.c"
    break;

  case 891: /* $@59: %empty  */
#line 4976 "parser.y"
                                { BEGIN_STATEMENT ("IF", TERM_IF); }
#line 9969 "parser.c"
    break;

  case 892: /* $@60: %empty  */
#line 4978 "parser.y"
  {
	check_unreached = 0;
  }
#line 9977 "parser.c"
    break;

  case 893: /* if_statement: IF $@59 condition _then $@60 statement_list if_else_sentence end_if  */
#line 4983 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[-2] == NULL) {
		cb_error (_("syntax error"));
	}
	cb_emit_if (yyvsp[-5], yyvsp[-2], yyvsp[-1]);
  }
#line 9988 "parser.c"
    break;

  case 895: /* if_else_sentence: %empty  */
#line 4994 "parser.y"
  {
	yyval = NULL;
  }
#line 9996 "parser.c"
    break;

  case 896: /* $@61: %empty  */
#line 4998 "parser.y"
  {
	check_unreached = 0;
  }
#line 10004 "parser.c"
    break;

  case 897: /* if_else_sentence: ELSE $@61 statement_list  */
#line 5002 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[0] == NULL) {
		cb_error (_("syntax error"));
	}
	yyval = yyvsp[0];
  }
#line 10015 "parser.c"
    break;

  case 898: /* end_if: %empty  */
#line 5011 "parser.y"
                                { terminator_warning (TERM_IF); }
#line 10021 "parser.c"
    break;

  case 899: /* end_if: "END-IF"  */
#line 5012 "parser.y"
                                { terminator_clear (TERM_IF); }
#line 10027 "parser.c"
    break;

  case 900: /* $@62: %empty  */
#line 5021 "parser.y"
                                { BEGIN_STATEMENT ("INITIALIZE", 0); }
#line 10033 "parser.c"
    break;

  case 901: /* initialize_statement: INITIALIZE $@62 target_x_list initialize_filler initialize_value initialize_replacing initialize_default  */
#line 5023 "parser.y"
  {
	cb_emit_initialize (yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]);
  }
#line 10041 "parser.c"
    break;

  case 902: /* initialize_filler: %empty  */
#line 5029 "parser.y"
                                { yyval = NULL; }
#line 10047 "parser.c"
    break;

  case 903: /* initialize_filler: _with FILLER  */
#line 5030 "parser.y"
                                { yyval = cb_true; }
#line 10053 "parser.c"
    break;

  case 904: /* initialize_value: %empty  */
#line 5034 "parser.y"
                                { yyval = NULL; }
#line 10059 "parser.c"
    break;

  case 905: /* initialize_value: ALL _to VALUE  */
#line 5035 "parser.y"
                                { yyval = cb_true; }
#line 10065 "parser.c"
    break;

  case 906: /* initialize_value: initialize_category _to VALUE  */
#line 5036 "parser.y"
                                { yyval = yyvsp[-2]; }
#line 10071 "parser.c"
    break;

  case 907: /* initialize_replacing: %empty  */
#line 5040 "parser.y"
                                { yyval = NULL; }
#line 10077 "parser.c"
    break;

  case 908: /* initialize_replacing: REPLACING initialize_replacing_list  */
#line 5042 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10083 "parser.c"
    break;

  case 909: /* initialize_replacing_list: initialize_replacing_item  */
#line 5046 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10089 "parser.c"
    break;

  case 910: /* initialize_replacing_list: initialize_replacing_list initialize_replacing_item  */
#line 5048 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 10095 "parser.c"
    break;

  case 911: /* initialize_replacing_item: initialize_category _data BY x  */
#line 5052 "parser.y"
                                 { yyval = cb_build_pair (yyvsp[-3], yyvsp[0]); }
#line 10101 "parser.c"
    break;

  case 912: /* initialize_category: ALPHABETIC  */
#line 5056 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_ALPHABETIC); }
#line 10107 "parser.c"
    break;

  case 913: /* initialize_category: ALPHANUMERIC  */
#line 5057 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_ALPHANUMERIC); }
#line 10113 "parser.c"
    break;

  case 914: /* initialize_category: NUMERIC  */
#line 5058 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NUMERIC); }
#line 10119 "parser.c"
    break;

  case 915: /* initialize_category: "ALPHANUMERIC-EDITED"  */
#line 5059 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_ALPHANUMERIC_EDITED); }
#line 10125 "parser.c"
    break;

  case 916: /* initialize_category: "NUMERIC-EDITED"  */
#line 5060 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NUMERIC_EDITED); }
#line 10131 "parser.c"
    break;

  case 917: /* initialize_category: NATIONAL  */
#line 5061 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NATIONAL); }
#line 10137 "parser.c"
    break;

  case 918: /* initialize_category: "NATIONAL-EDITED"  */
#line 5062 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NATIONAL_EDITED); }
#line 10143 "parser.c"
    break;

  case 919: /* initialize_default: %empty  */
#line 5066 "parser.y"
                                { yyval = NULL; }
#line 10149 "parser.c"
    break;

  case 920: /* initialize_default: DEFAULT  */
#line 5067 "parser.y"
                                { yyval = cb_true; }
#line 10155 "parser.c"
    break;

  case 921: /* $@63: %empty  */
#line 5076 "parser.y"
                                { BEGIN_STATEMENT ("INITIATE", 0); }
#line 10161 "parser.c"
    break;

  case 922: /* initiate_statement: INITIATE $@63 identifier_list  */
#line 5078 "parser.y"
  {
	PENDING("INITIATE");
  }
#line 10169 "parser.c"
    break;

  case 923: /* $@64: %empty  */
#line 5089 "parser.y"
  {
	BEGIN_STATEMENT ("INSPECT", 0);
	sending_id = 0;
	inspect_keyword = 0;
  }
#line 10179 "parser.c"
    break;

  case 925: /* send_identifier: identifier  */
#line 5098 "parser.y"
                                { save_tree_1 = yyvsp[0]; sending_id = 0; }
#line 10185 "parser.c"
    break;

  case 926: /* send_identifier: literal  */
#line 5099 "parser.y"
                                { save_tree_1 = yyvsp[0]; sending_id = 1; }
#line 10191 "parser.c"
    break;

  case 927: /* send_identifier: function  */
#line 5100 "parser.y"
                                { save_tree_1 = yyvsp[0]; sending_id = 1; }
#line 10197 "parser.c"
    break;

  case 930: /* inspect_item: inspect_tallying  */
#line 5109 "parser.y"
                                { cb_emit_inspect (save_tree_1, yyvsp[0], cb_int0, 0); }
#line 10203 "parser.c"
    break;

  case 931: /* inspect_item: inspect_replacing  */
#line 5110 "parser.y"
                                { cb_emit_inspect (save_tree_1, yyvsp[0], cb_int1, 1); }
#line 10209 "parser.c"
    break;

  case 932: /* inspect_item: inspect_converting  */
#line 5111 "parser.y"
                                { cb_emit_inspect (save_tree_1, yyvsp[0], cb_int0, 2); }
#line 10215 "parser.c"
    break;

  case 933: /* $@65: %empty  */
#line 5117 "parser.y"
                                { cb_init_tarrying (); }
#line 10221 "parser.c"
    break;

  case 934: /* inspect_tallying: TALLYING $@65 tallying_list  */
#line 5118 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10227 "parser.c"
    break;

  case 935: /* tallying_list: tallying_item  */
#line 5122 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10233 "parser.c"
    break;

  case 936: /* tallying_list: tallying_list tallying_item  */
#line 5123 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 10239 "parser.c"
    break;

  case 937: /* tallying_item: simple_value FOR  */
#line 5127 "parser.y"
                                { yyval = cb_build_tarrying_data (yyvsp[-1]); }
#line 10245 "parser.c"
    break;

  case 938: /* tallying_item: CHARACTERS inspect_region  */
#line 5128 "parser.y"
                                { yyval = cb_build_tarrying_characters (yyvsp[0]); }
#line 10251 "parser.c"
    break;

  case 939: /* tallying_item: ALL  */
#line 5129 "parser.y"
                                { yyval = cb_build_tarrying_all (); }
#line 10257 "parser.c"
    break;

  case 940: /* tallying_item: LEADING  */
#line 5130 "parser.y"
                                { yyval = cb_build_tarrying_leading (); }
#line 10263 "parser.c"
    break;

  case 941: /* tallying_item: TRAILING  */
#line 5131 "parser.y"
                                { yyval = cb_build_tarrying_trailing (); }
#line 10269 "parser.c"
    break;

  case 942: /* tallying_item: simple_value inspect_region  */
#line 5132 "parser.y"
                                { yyval = cb_build_tarrying_value (yyvsp[-1], yyvsp[0]); }
#line 10275 "parser.c"
    break;

  case 943: /* inspect_replacing: REPLACING replacing_list  */
#line 5138 "parser.y"
                                { yyval = yyvsp[0]; inspect_keyword = 0; }
#line 10281 "parser.c"
    break;

  case 944: /* replacing_list: replacing_item  */
#line 5142 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10287 "parser.c"
    break;

  case 945: /* replacing_list: replacing_list replacing_item  */
#line 5143 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 10293 "parser.c"
    break;

  case 946: /* replacing_item: CHARACTERS BY simple_value inspect_region  */
#line 5148 "parser.y"
  {
	yyval = cb_build_replacing_characters (yyvsp[-1], yyvsp[0], save_tree_1);
	inspect_keyword = 0;
  }
#line 10302 "parser.c"
    break;

  case 947: /* replacing_item: rep_keyword replacing_region  */
#line 5152 "parser.y"
                                        { yyval = yyvsp[0]; }
#line 10308 "parser.c"
    break;

  case 948: /* rep_keyword: %empty  */
#line 5156 "parser.y"
                                { /* Nothing */ }
#line 10314 "parser.c"
    break;

  case 949: /* rep_keyword: ALL  */
#line 5157 "parser.y"
                                { inspect_keyword = 1; }
#line 10320 "parser.c"
    break;

  case 950: /* rep_keyword: LEADING  */
#line 5158 "parser.y"
                                { inspect_keyword = 2; }
#line 10326 "parser.c"
    break;

  case 951: /* rep_keyword: FIRST  */
#line 5159 "parser.y"
                                { inspect_keyword = 3; }
#line 10332 "parser.c"
    break;

  case 952: /* rep_keyword: TRAILING  */
#line 5160 "parser.y"
                                { inspect_keyword = 4; }
#line 10338 "parser.c"
    break;

  case 953: /* replacing_region: simple_value BY simple_all_value inspect_region  */
#line 5165 "parser.y"
  {
	switch (inspect_keyword) {
		case 1:
			yyval = cb_build_replacing_all (yyvsp[-3], yyvsp[-1], yyvsp[0], save_tree_1);
			break;
		case 2:
			yyval = cb_build_replacing_leading (yyvsp[-3], yyvsp[-1], yyvsp[0]);
			break;
		case 3:
			yyval = cb_build_replacing_first (yyvsp[-3], yyvsp[-1], yyvsp[0]);
			break;
		case 4:
			yyval = cb_build_replacing_trailing (yyvsp[-3], yyvsp[-1], yyvsp[0]);
			break;
		default:
			cb_error (_("INSPECT missing a keyword"));
			yyval = cb_error_node;
			break;
	}
  }
#line 10363 "parser.c"
    break;

  case 954: /* inspect_converting: CONVERTING simple_value TO simple_all_value inspect_region  */
#line 5191 "parser.y"
  {
	if (cb_validate_inspect (save_tree_1, yyvsp[-3], yyvsp[-1]) < 0 ) {
		yyval = cb_error_node;
	} else {
		yyval = cb_build_converting (yyvsp[-3], yyvsp[-1], yyvsp[0]);
	}
  }
#line 10375 "parser.c"
    break;

  case 955: /* inspect_region: %empty  */
#line 5203 "parser.y"
                                { yyval = cb_build_inspect_region_start (); }
#line 10381 "parser.c"
    break;

  case 956: /* inspect_region: inspect_region before_or_after _initial x  */
#line 5205 "parser.y"
                                { yyval = cb_build_inspect_region (yyvsp[-3], yyvsp[-2], yyvsp[0]); }
#line 10387 "parser.c"
    break;

  case 959: /* $@66: %empty  */
#line 5216 "parser.y"
                                { BEGIN_STATEMENT ("MERGE", 0); }
#line 10393 "parser.c"
    break;

  case 961: /* $@67: %empty  */
#line 5226 "parser.y"
                                { BEGIN_STATEMENT ("MOVE", 0); }
#line 10399 "parser.c"
    break;

  case 963: /* move_body: x TO target_x_list  */
#line 5232 "parser.y"
  {
	cb_emit_move (yyvsp[-2], yyvsp[0]);
  }
#line 10407 "parser.c"
    break;

  case 964: /* move_body: CORRESPONDING x TO target_x_list  */
#line 5236 "parser.y"
  {
	cb_emit_move_corresponding (yyvsp[-2], yyvsp[0]);
  }
#line 10415 "parser.c"
    break;

  case 965: /* $@68: %empty  */
#line 5247 "parser.y"
                                { BEGIN_STATEMENT ("MULTIPLY", TERM_MULTIPLY); }
#line 10421 "parser.c"
    break;

  case 967: /* multiply_body: x BY arithmetic_x_list on_size_error  */
#line 5254 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '*', yyvsp[-3]);
  }
#line 10429 "parser.c"
    break;

  case 968: /* multiply_body: x BY x GIVING arithmetic_x_list on_size_error  */
#line 5258 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_op (yyvsp[-5], '*', yyvsp[-3]));
  }
#line 10437 "parser.c"
    break;

  case 969: /* end_multiply: %empty  */
#line 5264 "parser.y"
                                { terminator_warning (TERM_MULTIPLY); }
#line 10443 "parser.c"
    break;

  case 970: /* end_multiply: "END-MULTIPLY"  */
#line 5265 "parser.y"
                                { terminator_clear (TERM_MULTIPLY); }
#line 10449 "parser.c"
    break;

  case 971: /* $@69: %empty  */
#line 5274 "parser.y"
                                { BEGIN_STATEMENT ("OPEN", 0); }
#line 10455 "parser.c"
    break;

  case 974: /* open_list: open_list open_mode open_sharing file_name_list open_option  */
#line 5281 "parser.y"
  {
	cb_tree l;
	for (l = yyvsp[-1]; l; l = CB_CHAIN (l)) {
		if (CB_VALUE (l) != cb_error_node) {
			BEGIN_IMPLICIT_STATEMENT (l);
			cb_emit_open (CB_VALUE (l), yyvsp[-3], yyvsp[-2]);
		}
	}
  }
#line 10469 "parser.c"
    break;

  case 975: /* open_mode: INPUT  */
#line 5293 "parser.y"
                                { yyval = cb_int (COB_OPEN_INPUT); }
#line 10475 "parser.c"
    break;

  case 976: /* open_mode: OUTPUT  */
#line 5294 "parser.y"
                                { yyval = cb_int (COB_OPEN_OUTPUT); }
#line 10481 "parser.c"
    break;

  case 977: /* open_mode: "I-O"  */
#line 5295 "parser.y"
                                { yyval = cb_int (COB_OPEN_I_O); }
#line 10487 "parser.c"
    break;

  case 978: /* open_mode: EXTEND  */
#line 5296 "parser.y"
                                { yyval = cb_int (COB_OPEN_EXTEND); }
#line 10493 "parser.c"
    break;

  case 979: /* open_sharing: %empty  */
#line 5300 "parser.y"
                                { yyval = NULL; }
#line 10499 "parser.c"
    break;

  case 980: /* open_sharing: SHARING _with sharing_option  */
#line 5301 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10505 "parser.c"
    break;

  case 981: /* open_option: %empty  */
#line 5305 "parser.y"
                                { yyval = NULL; }
#line 10511 "parser.c"
    break;

  case 982: /* open_option: _with NO REWIND  */
#line 5306 "parser.y"
                                { yyval = NULL; }
#line 10517 "parser.c"
    break;

  case 983: /* open_option: _with LOCK  */
#line 5307 "parser.y"
                                { PENDING ("OPEN ... WITH LOCK"); }
#line 10523 "parser.c"
    break;

  case 984: /* $@70: %empty  */
#line 5319 "parser.y"
                                { BEGIN_STATEMENT ("PERFORM", TERM_PERFORM); }
#line 10529 "parser.c"
    break;

  case 986: /* perform_body: perform_procedure perform_option  */
#line 5325 "parser.y"
  {
	cb_emit_perform (yyvsp[0], yyvsp[-1]);
  }
#line 10537 "parser.c"
    break;

  case 987: /* $@71: %empty  */
#line 5329 "parser.y"
  {
	perform_stack = cb_cons (yyvsp[0], perform_stack);
	check_unreached = 0;
  }
#line 10546 "parser.c"
    break;

  case 988: /* perform_body: perform_option $@71 statement_list end_perform  */
#line 5334 "parser.y"
  {
	perform_stack = CB_CHAIN (perform_stack);
	cb_emit_perform (yyvsp[-3], yyvsp[-1]);
  }
#line 10555 "parser.c"
    break;

  case 989: /* perform_body: perform_option "END-PERFORM"  */
#line 5339 "parser.y"
  {
	cb_emit_perform (yyvsp[-1], NULL);
  }
#line 10563 "parser.c"
    break;

  case 990: /* end_perform: %empty  */
#line 5345 "parser.y"
                                { terminator_error (); }
#line 10569 "parser.c"
    break;

  case 991: /* end_perform: "END-PERFORM"  */
#line 5346 "parser.y"
                                { terminator_clear (TERM_PERFORM); }
#line 10575 "parser.c"
    break;

  case 992: /* perform_procedure: procedure_name  */
#line 5351 "parser.y"
  {
	CB_REFERENCE (yyvsp[0])->length = cb_true; /* return from $1 */
	yyval = cb_build_pair (yyvsp[0], yyvsp[0]);
  }
#line 10584 "parser.c"
    break;

  case 993: /* perform_procedure: procedure_name THRU procedure_name  */
#line 5356 "parser.y"
  {
	CB_REFERENCE (yyvsp[0])->length = cb_true; /* return from $3 */
	yyval = cb_build_pair (yyvsp[-2], yyvsp[0]);
  }
#line 10593 "parser.c"
    break;

  case 994: /* perform_option: %empty  */
#line 5364 "parser.y"
  {
	yyval = cb_build_perform_once (NULL);
  }
#line 10601 "parser.c"
    break;

  case 995: /* perform_option: FOREVER  */
#line 5368 "parser.y"
  {
	yyval = cb_build_perform_forever (NULL);
  }
#line 10609 "parser.c"
    break;

  case 996: /* perform_option: id_or_lit_or_func TIMES  */
#line 5372 "parser.y"
  {
	yyval = cb_build_perform_times (yyvsp[-1]);
	current_program->loop_counter++;
  }
#line 10618 "parser.c"
    break;

  case 997: /* perform_option: perform_test UNTIL condition  */
#line 5377 "parser.y"
  {
	cb_tree varying;

	varying = cb_list_init (cb_build_perform_varying (NULL, NULL, NULL, yyvsp[0]));
	yyval = cb_build_perform_until (yyvsp[-2], varying);
  }
#line 10629 "parser.c"
    break;

  case 998: /* perform_option: perform_test VARYING perform_varying_list  */
#line 5384 "parser.y"
  {
	yyval = cb_build_perform_until (yyvsp[-2], yyvsp[0]);
  }
#line 10637 "parser.c"
    break;

  case 999: /* perform_test: %empty  */
#line 5390 "parser.y"
                                { yyval = CB_BEFORE; }
#line 10643 "parser.c"
    break;

  case 1000: /* perform_test: _with TEST before_or_after  */
#line 5391 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10649 "parser.c"
    break;

  case 1001: /* perform_varying_list: perform_varying  */
#line 5395 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 10655 "parser.c"
    break;

  case 1002: /* perform_varying_list: perform_varying_list AFTER perform_varying  */
#line 5397 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 10661 "parser.c"
    break;

  case 1003: /* perform_varying: identifier FROM x BY x UNTIL condition  */
#line 5402 "parser.y"
  {
	yyval = cb_build_perform_varying (yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 10669 "parser.c"
    break;

  case 1004: /* $@72: %empty  */
#line 5413 "parser.y"
                                { BEGIN_STATEMENT ("READ", TERM_READ); }
#line 10675 "parser.c"
    break;

  case 1005: /* read_statement: READ $@72 file_name flag_next _record read_into with_lock read_key read_handler end_read  */
#line 5416 "parser.y"
  {
	if (yyvsp[-7] != cb_error_node) {
		if (cb_use_invalidkey_handler_on_status34 &&
		    current_statement->handler_id == COB_EC_I_O_INVALID_KEY &&
		    (CB_FILE(cb_ref (yyvsp[-7]))->organization != COB_ORG_RELATIVE &&
		     CB_FILE(cb_ref (yyvsp[-7]))->organization != COB_ORG_INDEXED)) {
			current_statement->handler_id = COB_EC_I_O_PERMANENT_ERROR;
		}
		if (yyvsp[-2] &&
		      (CB_FILE(cb_ref (yyvsp[-7]))->organization != COB_ORG_RELATIVE &&
		       CB_FILE(cb_ref (yyvsp[-7]))->organization != COB_ORG_INDEXED)) {
			cb_error (_("KEY clause invalid with this file type"));
		} else if (current_statement->handler_id == COB_EC_I_O_INVALID_KEY &&
		      (CB_FILE(cb_ref (yyvsp[-7]))->organization != COB_ORG_RELATIVE &&
		       CB_FILE(cb_ref (yyvsp[-7]))->organization != COB_ORG_INDEXED)) {
			cb_error (_("INVALID KEY clause invalid with this file type"));
		} else {
			cb_emit_read (yyvsp[-7], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[-3]);
		}
	}
  }
#line 10701 "parser.c"
    break;

  case 1006: /* read_into: %empty  */
#line 5440 "parser.y"
                                { yyval = NULL; }
#line 10707 "parser.c"
    break;

  case 1007: /* read_into: INTO identifier  */
#line 5441 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10713 "parser.c"
    break;

  case 1008: /* with_lock: %empty  */
#line 5445 "parser.y"
                                { yyval = NULL; }
#line 10719 "parser.c"
    break;

  case 1009: /* with_lock: IGNORING LOCK  */
#line 5447 "parser.y"
  {
	yyval = cb_int3;
  }
#line 10727 "parser.c"
    break;

  case 1010: /* with_lock: _with LOCK  */
#line 5451 "parser.y"
  {
	yyval = cb_int1;
  }
#line 10735 "parser.c"
    break;

  case 1011: /* with_lock: _with NO LOCK  */
#line 5455 "parser.y"
  {
	yyval = cb_int2;
  }
#line 10743 "parser.c"
    break;

  case 1012: /* with_lock: _with IGNORE LOCK  */
#line 5459 "parser.y"
  {
	yyval = cb_int3;
  }
#line 10751 "parser.c"
    break;

  case 1013: /* with_lock: _with WAIT  */
#line 5463 "parser.y"
  {
	yyval = cb_int4;
  }
#line 10759 "parser.c"
    break;

  case 1014: /* read_key: %empty  */
#line 5469 "parser.y"
                                { yyval = NULL; }
#line 10765 "parser.c"
    break;

  case 1015: /* read_key: KEY _is identifier_list  */
#line 5471 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 10773 "parser.c"
    break;

  case 1019: /* end_read: %empty  */
#line 5482 "parser.y"
                                { terminator_warning (TERM_READ); }
#line 10779 "parser.c"
    break;

  case 1020: /* end_read: "END-READ"  */
#line 5483 "parser.y"
                                { terminator_clear (TERM_READ); }
#line 10785 "parser.c"
    break;

  case 1021: /* $@73: %empty  */
#line 5492 "parser.y"
                                { BEGIN_STATEMENT ("RELEASE", 0); }
#line 10791 "parser.c"
    break;

  case 1022: /* release_statement: RELEASE $@73 record_name write_from  */
#line 5494 "parser.y"
  {
	if (yyvsp[-1] != cb_error_node) {
		cb_emit_release (yyvsp[-1], yyvsp[0]);
	}
  }
#line 10801 "parser.c"
    break;

  case 1023: /* $@74: %empty  */
#line 5507 "parser.y"
                                { BEGIN_STATEMENT ("RETURN", TERM_RETURN); }
#line 10807 "parser.c"
    break;

  case 1024: /* return_statement: RETURN $@74 file_name _record read_into at_end end_return  */
#line 5510 "parser.y"
  {
	if (yyvsp[-4] != cb_error_node) {
		cb_emit_return (yyvsp[-4], yyvsp[-2]);
	}
  }
#line 10817 "parser.c"
    break;

  case 1025: /* end_return: %empty  */
#line 5518 "parser.y"
                                { terminator_warning (TERM_RETURN); }
#line 10823 "parser.c"
    break;

  case 1026: /* end_return: "END-RETURN"  */
#line 5519 "parser.y"
                                { terminator_clear (TERM_RETURN); }
#line 10829 "parser.c"
    break;

  case 1027: /* $@75: %empty  */
#line 5528 "parser.y"
                                { BEGIN_STATEMENT ("REWRITE", TERM_REWRITE); }
#line 10835 "parser.c"
    break;

  case 1028: /* rewrite_statement: REWRITE $@75 record_name write_from write_lock opt_invalid_key end_rewrite  */
#line 5531 "parser.y"
  {
	if (yyvsp[-4] != cb_error_node) {
		if (cb_use_invalidkey_handler_on_status34 &&
		    current_statement->handler_id == COB_EC_I_O_INVALID_KEY &&
		    (CB_FIELD(cb_ref (yyvsp[-4]))->file->organization != COB_ORG_RELATIVE &&
		     CB_FIELD(cb_ref (yyvsp[-4]))->file->organization != COB_ORG_INDEXED)) {
			current_statement->handler_id = COB_EC_I_O_PERMANENT_ERROR;
		}
		cb_emit_rewrite (yyvsp[-4], yyvsp[-3], yyvsp[-2]);
	}
  }
#line 10851 "parser.c"
    break;

  case 1029: /* write_lock: %empty  */
#line 5545 "parser.y"
                                { yyval = NULL; }
#line 10857 "parser.c"
    break;

  case 1030: /* write_lock: _with LOCK  */
#line 5547 "parser.y"
  {
	yyval = cb_int1;
  }
#line 10865 "parser.c"
    break;

  case 1031: /* write_lock: _with NO LOCK  */
#line 5551 "parser.y"
  {
	yyval = cb_int2;
  }
#line 10873 "parser.c"
    break;

  case 1032: /* end_rewrite: %empty  */
#line 5557 "parser.y"
                                { terminator_warning (TERM_REWRITE); }
#line 10879 "parser.c"
    break;

  case 1033: /* end_rewrite: "END-REWRITE"  */
#line 5558 "parser.y"
                                { terminator_clear (TERM_REWRITE); }
#line 10885 "parser.c"
    break;

  case 1034: /* rollback_statement: ROLLBACK  */
#line 5568 "parser.y"
  {
	BEGIN_STATEMENT ("ROLLBACK", 0);
	cb_emit_rollback ();
  }
#line 10894 "parser.c"
    break;

  case 1035: /* $@76: %empty  */
#line 5580 "parser.y"
                                { BEGIN_STATEMENT ("SEARCH", TERM_SEARCH); }
#line 10900 "parser.c"
    break;

  case 1037: /* search_body: table_name search_varying search_at_end search_whens  */
#line 5587 "parser.y"
  {
	cb_emit_search (yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]);
  }
#line 10908 "parser.c"
    break;

  case 1038: /* $@77: %empty  */
#line 5591 "parser.y"
  {
	check_unreached = 0;
  }
#line 10916 "parser.c"
    break;

  case 1039: /* search_body: ALL table_name search_at_end WHEN expr $@77 statement_list  */
#line 5595 "parser.y"
  {
	cb_emit_search_all (yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 10924 "parser.c"
    break;

  case 1040: /* search_varying: %empty  */
#line 5601 "parser.y"
                                { yyval = NULL; }
#line 10930 "parser.c"
    break;

  case 1041: /* search_varying: VARYING identifier  */
#line 5602 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10936 "parser.c"
    break;

  case 1042: /* search_at_end: %empty  */
#line 5606 "parser.y"
                                { yyval = NULL; }
#line 10942 "parser.c"
    break;

  case 1043: /* $@78: %empty  */
#line 5608 "parser.y"
  {
	check_unreached = 0;
  }
#line 10950 "parser.c"
    break;

  case 1044: /* search_at_end: _at END $@78 statement_list  */
#line 5612 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 10958 "parser.c"
    break;

  case 1045: /* search_whens: search_when  */
#line 5618 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10964 "parser.c"
    break;

  case 1046: /* search_whens: search_when search_whens  */
#line 5619 "parser.y"
                                { yyval = yyvsp[-1]; CB_IF (yyvsp[-1])->stmt2 = yyvsp[0]; }
#line 10970 "parser.c"
    break;

  case 1047: /* $@79: %empty  */
#line 5624 "parser.y"
  {
	check_unreached = 0;
  }
#line 10978 "parser.c"
    break;

  case 1048: /* search_when: WHEN condition $@79 statement_list  */
#line 5628 "parser.y"
  {
	yyval = cb_build_if (yyvsp[-2], yyvsp[0], NULL);
  }
#line 10986 "parser.c"
    break;

  case 1049: /* end_search: %empty  */
#line 5634 "parser.y"
                                { terminator_warning (TERM_SEARCH); }
#line 10992 "parser.c"
    break;

  case 1050: /* end_search: "END-SEARCH"  */
#line 5635 "parser.y"
                                { terminator_clear (TERM_SEARCH); }
#line 10998 "parser.c"
    break;

  case 1051: /* $@80: %empty  */
#line 5644 "parser.y"
                                { BEGIN_STATEMENT ("SET", 0); }
#line 11004 "parser.c"
    break;

  case 1058: /* set_environment: ENVIRONMENT simple_value TO simple_value  */
#line 5660 "parser.y"
  {
	cb_emit_setenv (yyvsp[-2], yyvsp[0]);
  }
#line 11012 "parser.c"
    break;

  case 1059: /* set_to: target_x_list TO ENTRY alnum_or_id  */
#line 5669 "parser.y"
  {
	cb_emit_set_to (yyvsp[-3], cb_build_ppointer (yyvsp[0]));
  }
#line 11020 "parser.c"
    break;

  case 1060: /* set_to: target_x_list TO x  */
#line 5673 "parser.y"
  {
	cb_emit_set_to (yyvsp[-2], yyvsp[0]);
  }
#line 11028 "parser.c"
    break;

  case 1061: /* set_up_down: target_x_list up_or_down BY x  */
#line 5682 "parser.y"
  {
	cb_emit_set_up_down (yyvsp[-3], yyvsp[-2], yyvsp[0]);
  }
#line 11036 "parser.c"
    break;

  case 1062: /* up_or_down: UP  */
#line 5688 "parser.y"
                                { yyval = cb_int0; }
#line 11042 "parser.c"
    break;

  case 1063: /* up_or_down: DOWN  */
#line 5689 "parser.y"
                                { yyval = cb_int1; }
#line 11048 "parser.c"
    break;

  case 1066: /* set_to_on_off: mnemonic_name_list TO on_or_off  */
#line 5701 "parser.y"
  {
	cb_emit_set_on_off (yyvsp[-2], yyvsp[0]);
  }
#line 11056 "parser.c"
    break;

  case 1069: /* set_to_true_false: target_x_list TO "TRUE"  */
#line 5715 "parser.y"
  {
	cb_emit_set_true (yyvsp[-2]);
  }
#line 11064 "parser.c"
    break;

  case 1070: /* set_to_true_false: target_x_list TO "FALSE"  */
#line 5719 "parser.y"
  {
	cb_emit_set_false (yyvsp[-2]);
  }
#line 11072 "parser.c"
    break;

  case 1071: /* $@81: %empty  */
#line 5730 "parser.y"
                                { BEGIN_STATEMENT ("SORT", 0); }
#line 11078 "parser.c"
    break;

  case 1073: /* $@82: %empty  */
#line 5736 "parser.y"
  {
	cb_emit_sort_init (yyvsp[-3], yyvsp[-2], yyvsp[0]);
	if (CB_FILE_P (cb_ref (yyvsp[-3])) && yyvsp[-2] == NULL) {
		cb_error (_("File sort requires KEY phrase"));
	}
	/* used in sort_input/sort_output */
	save_tree_1 = yyvsp[-3];
  }
#line 11091 "parser.c"
    break;

  case 1074: /* sort_body: qualified_word sort_key_list sort_duplicates sort_collating $@82 sort_input sort_output  */
#line 5745 "parser.y"
  {
	cb_emit_sort_finish (yyvsp[-6]);
  }
#line 11099 "parser.c"
    break;

  case 1075: /* sort_key_list: %empty  */
#line 5752 "parser.y"
  {
	yyval = NULL;
  }
#line 11107 "parser.c"
    break;

  case 1076: /* sort_key_list: sort_key_list _on ascending_or_descending _key _is opt_key_list  */
#line 5757 "parser.y"
  {
	cb_tree l;

	if (!cb_allow_is_in_sort_key_spec && yyvsp[-1] != NULL) {
		cb_error (_("syntax error"));
		yyval = cb_error_node;
	} else {
		if (yyvsp[0] == NULL) {
			yyvsp[0] = cb_list_init (NULL);
		}
		for (l = yyvsp[0]; l; l = CB_CHAIN (l)) {
			CB_PURPOSE (l) = yyvsp[-3];
		}
		yyval = cb_list_append (yyvsp[-5], yyvsp[0]);
	}
  }
#line 11128 "parser.c"
    break;

  case 1077: /* opt_key_list: %empty  */
#line 5776 "parser.y"
                                { yyval = NULL; }
#line 11134 "parser.c"
    break;

  case 1078: /* opt_key_list: opt_key_list qualified_word  */
#line 5777 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 11140 "parser.c"
    break;

  case 1080: /* sort_duplicates: with_dups _in_order  */
#line 5781 "parser.y"
                                { /* nothing */ }
#line 11146 "parser.c"
    break;

  case 1081: /* sort_collating: %empty  */
#line 5785 "parser.y"
                                        { yyval = cb_null; }
#line 11152 "parser.c"
    break;

  case 1082: /* sort_collating: coll_sequence _is reference  */
#line 5786 "parser.y"
                                        { yyval = cb_ref (yyvsp[0]); }
#line 11158 "parser.c"
    break;

  case 1083: /* sort_input: %empty  */
#line 5791 "parser.y"
  {
	if (CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("File sort requires USING or INPUT PROCEDURE"));
	}
  }
#line 11168 "parser.c"
    break;

  case 1084: /* sort_input: USING file_name_list  */
#line 5797 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("USING invalid with table SORT"));
	} else {
		cb_emit_sort_using (save_tree_1, yyvsp[0]);
	}
  }
#line 11180 "parser.c"
    break;

  case 1085: /* sort_input: INPUT PROCEDURE _is perform_procedure  */
#line 5805 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("INPUT PROCEDURE invalid with table SORT"));
	} else {
		cb_emit_sort_input (yyvsp[0], save_tree_1);
	}
  }
#line 11192 "parser.c"
    break;

  case 1086: /* sort_output: %empty  */
#line 5816 "parser.y"
  {
	if (CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("File sort requires GIVING or OUTPUT PROCEDURE"));
	}
  }
#line 11202 "parser.c"
    break;

  case 1087: /* sort_output: GIVING file_name_list  */
#line 5822 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("GIVING invalid with table SORT"));
	} else {
		cb_emit_sort_giving (save_tree_1, yyvsp[0]);
	}
  }
#line 11214 "parser.c"
    break;

  case 1088: /* sort_output: OUTPUT PROCEDURE _is perform_procedure  */
#line 5830 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("OUTPUT PROCEDURE invalid with table SORT"));
	} else {
		cb_emit_sort_output (yyvsp[0], save_tree_1);
	}
  }
#line 11226 "parser.c"
    break;

  case 1089: /* $@83: %empty  */
#line 5845 "parser.y"
                                { BEGIN_STATEMENT ("START", TERM_START); }
#line 11232 "parser.c"
    break;

  case 1090: /* @84: %empty  */
#line 5846 "parser.y"
                                { yyval = cb_int (COB_EQ); }
#line 11238 "parser.c"
    break;

  case 1091: /* start_statement: START $@83 file_name @84 start_key opt_invalid_key end_start  */
#line 5849 "parser.y"
  {
	if (CB_FILE_P (cb_ref (yyvsp[-4]))) {
		if (CB_FILE (cb_ref (yyvsp[-4]))->organization != COB_ORG_INDEXED &&
		     CB_FILE (cb_ref (yyvsp[-4]))->organization != COB_ORG_RELATIVE) {
			cb_error (_("START not allowed on SEQUENTIAL files"));
			yyval = cb_error_node;
		} else {
			cb_emit_start (yyvsp[-4], yyvsp[-3], yyvsp[-2]);
		}
	} else {
		cb_error_x (yyvsp[-4], _("'%s' is not a file name"), CB_NAME (yyvsp[-4]));
		yyval = cb_error_node;
	}
  }
#line 11257 "parser.c"
    break;

  case 1092: /* start_key: %empty  */
#line 5866 "parser.y"
                                { yyval = NULL; }
#line 11263 "parser.c"
    break;

  case 1093: /* start_key: KEY _is start_op identifier_list  */
#line 5868 "parser.y"
  {
	yyvsp[-4] = yyvsp[-1];
	yyval = yyvsp[0];
  }
#line 11272 "parser.c"
    break;

  case 1094: /* start_op: flag_not eq  */
#line 5875 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_NE : COB_EQ); }
#line 11278 "parser.c"
    break;

  case 1095: /* start_op: flag_not gt  */
#line 5876 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_LE : COB_GT); }
#line 11284 "parser.c"
    break;

  case 1096: /* start_op: flag_not lt  */
#line 5877 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_GE : COB_LT); }
#line 11290 "parser.c"
    break;

  case 1097: /* start_op: flag_not ge  */
#line 5878 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_LT : COB_GE); }
#line 11296 "parser.c"
    break;

  case 1098: /* start_op: flag_not le  */
#line 5879 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_GT : COB_LE); }
#line 11302 "parser.c"
    break;

  case 1099: /* end_start: %empty  */
#line 5883 "parser.y"
                                { terminator_warning (TERM_START); }
#line 11308 "parser.c"
    break;

  case 1100: /* end_start: "END-START"  */
#line 5884 "parser.y"
                                { terminator_clear (TERM_START); }
#line 11314 "parser.c"
    break;

  case 1101: /* $@85: %empty  */
#line 5893 "parser.y"
                                { BEGIN_STATEMENT ("STOP", 0); }
#line 11320 "parser.c"
    break;

  case 1102: /* stop_statement: STOP RUN $@85 stop_returning  */
#line 5895 "parser.y"
  {
	cb_emit_stop_run (yyvsp[0]);
  }
#line 11328 "parser.c"
    break;

  case 1103: /* $@86: %empty  */
#line 5898 "parser.y"
                                { BEGIN_STATEMENT ("STOP", 0); }
#line 11334 "parser.c"
    break;

  case 1104: /* stop_statement: STOP "Literal" $@86  */
#line 5899 "parser.y"
  {
	cb_verify (cb_stop_literal_statement, "STOP literal");
  }
#line 11342 "parser.c"
    break;

  case 1105: /* stop_returning: %empty  */
#line 5905 "parser.y"
                        { yyval = current_program->cb_return_code; }
#line 11348 "parser.c"
    break;

  case 1106: /* stop_returning: RETURNING x  */
#line 5906 "parser.y"
                        { yyval = yyvsp[0]; }
#line 11354 "parser.c"
    break;

  case 1107: /* stop_returning: GIVING x  */
#line 5907 "parser.y"
                        { yyval = yyvsp[0]; }
#line 11360 "parser.c"
    break;

  case 1108: /* $@87: %empty  */
#line 5916 "parser.y"
                                { BEGIN_STATEMENT ("STRING", TERM_STRING); }
#line 11366 "parser.c"
    break;

  case 1109: /* string_statement: STRING $@87 string_item_list INTO identifier opt_with_pointer on_overflow end_string  */
#line 5919 "parser.y"
  {
	cb_emit_string (yyvsp[-5], yyvsp[-3], yyvsp[-2]);
  }
#line 11374 "parser.c"
    break;

  case 1110: /* string_item_list: string_item  */
#line 5925 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 11380 "parser.c"
    break;

  case 1111: /* string_item_list: string_item_list string_item  */
#line 5926 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 11386 "parser.c"
    break;

  case 1112: /* string_item: x  */
#line 5930 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11392 "parser.c"
    break;

  case 1113: /* string_item: DELIMITED _by SIZE  */
#line 5931 "parser.y"
                                { yyval = cb_build_pair (cb_int0, NULL); }
#line 11398 "parser.c"
    break;

  case 1114: /* string_item: DELIMITED _by x  */
#line 5932 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], NULL); }
#line 11404 "parser.c"
    break;

  case 1115: /* opt_with_pointer: %empty  */
#line 5936 "parser.y"
                                { yyval = cb_int0; }
#line 11410 "parser.c"
    break;

  case 1116: /* opt_with_pointer: _with POINTER identifier  */
#line 5937 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11416 "parser.c"
    break;

  case 1117: /* end_string: %empty  */
#line 5941 "parser.y"
                                { terminator_warning (TERM_STRING); }
#line 11422 "parser.c"
    break;

  case 1118: /* end_string: "END-STRING"  */
#line 5942 "parser.y"
                                { terminator_clear (TERM_STRING); }
#line 11428 "parser.c"
    break;

  case 1119: /* $@88: %empty  */
#line 5951 "parser.y"
                                { BEGIN_STATEMENT ("SUBTRACT", TERM_SUBTRACT); }
#line 11434 "parser.c"
    break;

  case 1121: /* subtract_body: x_list FROM arithmetic_x_list on_size_error  */
#line 5958 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '-', cb_build_binary_list (yyvsp[-3], '+'));
  }
#line 11442 "parser.c"
    break;

  case 1122: /* subtract_body: x_list FROM x GIVING arithmetic_x_list on_size_error  */
#line 5962 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_list (cb_cons (yyvsp[-3], yyvsp[-5]), '-'));
  }
#line 11450 "parser.c"
    break;

  case 1123: /* subtract_body: CORRESPONDING identifier FROM identifier flag_rounded on_size_error  */
#line 5966 "parser.y"
  {
	cb_emit_corresponding (cb_build_sub, yyvsp[-2], yyvsp[-4], yyvsp[-1]);
  }
#line 11458 "parser.c"
    break;

  case 1124: /* end_subtract: %empty  */
#line 5972 "parser.y"
                                { terminator_warning (TERM_SUBTRACT); }
#line 11464 "parser.c"
    break;

  case 1125: /* end_subtract: "END-SUBTRACT"  */
#line 5973 "parser.y"
                                { terminator_clear (TERM_SUBTRACT); }
#line 11470 "parser.c"
    break;

  case 1126: /* suppress_statement: SUPPRESS _printing  */
#line 5983 "parser.y"
  {
	BEGIN_STATEMENT ("SUPPRESS", 0);
	PENDING("SUPPRESS");
  }
#line 11479 "parser.c"
    break;

  case 1129: /* $@89: %empty  */
#line 5998 "parser.y"
                                { BEGIN_STATEMENT ("TERMINATE", 0); }
#line 11485 "parser.c"
    break;

  case 1130: /* terminate_statement: TERMINATE $@89 identifier_list  */
#line 6000 "parser.y"
  {
	PENDING("TERMINATE");
  }
#line 11493 "parser.c"
    break;

  case 1131: /* $@90: %empty  */
#line 6011 "parser.y"
                                { BEGIN_STATEMENT ("TRANSFORM", 0); }
#line 11499 "parser.c"
    break;

  case 1132: /* transform_statement: TRANSFORM $@90 identifier FROM simple_value TO simple_all_value  */
#line 6013 "parser.y"
  {
	cb_tree		x;

	x = cb_build_converting (yyvsp[-2], yyvsp[0], cb_build_inspect_region_start ());
	cb_emit_inspect (yyvsp[-4], x, cb_int0, 2);
  }
#line 11510 "parser.c"
    break;

  case 1133: /* $@91: %empty  */
#line 6027 "parser.y"
                                { BEGIN_STATEMENT ("UNLOCK", 0); }
#line 11516 "parser.c"
    break;

  case 1134: /* unlock_statement: UNLOCK $@91 file_name opt_record  */
#line 6029 "parser.y"
  {
	if (yyvsp[-1] != cb_error_node) {
		cb_emit_unlock (yyvsp[-1]);
	}
  }
#line 11526 "parser.c"
    break;

  case 1138: /* $@92: %empty  */
#line 6048 "parser.y"
                                { BEGIN_STATEMENT ("UNSTRING", TERM_UNSTRING); }
#line 11532 "parser.c"
    break;

  case 1139: /* unstring_statement: UNSTRING $@92 identifier unstring_delimited unstring_into opt_with_pointer unstring_tallying on_overflow end_unstring  */
#line 6052 "parser.y"
  {
	cb_emit_unstring (yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2]);
  }
#line 11540 "parser.c"
    break;

  case 1140: /* unstring_delimited: %empty  */
#line 6058 "parser.y"
                                { yyval = NULL; }
#line 11546 "parser.c"
    break;

  case 1141: /* unstring_delimited: DELIMITED _by unstring_delimited_list  */
#line 6060 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11552 "parser.c"
    break;

  case 1142: /* unstring_delimited_list: unstring_delimited_item  */
#line 6064 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 11558 "parser.c"
    break;

  case 1143: /* unstring_delimited_list: unstring_delimited_list OR unstring_delimited_item  */
#line 6066 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 11564 "parser.c"
    break;

  case 1144: /* unstring_delimited_item: flag_all simple_value  */
#line 6071 "parser.y"
  {
	yyval = cb_build_unstring_delimited (yyvsp[-1], yyvsp[0]);
  }
#line 11572 "parser.c"
    break;

  case 1145: /* unstring_into: INTO unstring_into_item  */
#line 6077 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 11578 "parser.c"
    break;

  case 1146: /* unstring_into: unstring_into unstring_into_item  */
#line 6079 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 11584 "parser.c"
    break;

  case 1147: /* unstring_into_item: identifier unstring_into_delimiter unstring_into_count  */
#line 6084 "parser.y"
  {
	yyval = cb_build_unstring_into (yyvsp[-2], yyvsp[-1], yyvsp[0]);
  }
#line 11592 "parser.c"
    break;

  case 1148: /* unstring_into_delimiter: %empty  */
#line 6090 "parser.y"
                                { yyval = NULL; }
#line 11598 "parser.c"
    break;

  case 1149: /* unstring_into_delimiter: DELIMITER _in identifier  */
#line 6091 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11604 "parser.c"
    break;

  case 1150: /* unstring_into_count: %empty  */
#line 6095 "parser.y"
                                { yyval = NULL; }
#line 11610 "parser.c"
    break;

  case 1151: /* unstring_into_count: COUNT _in identifier  */
#line 6096 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11616 "parser.c"
    break;

  case 1152: /* unstring_tallying: %empty  */
#line 6100 "parser.y"
                                { yyval = NULL; }
#line 11622 "parser.c"
    break;

  case 1153: /* unstring_tallying: TALLYING _in identifier  */
#line 6101 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11628 "parser.c"
    break;

  case 1154: /* end_unstring: %empty  */
#line 6105 "parser.y"
                                { terminator_warning (TERM_UNSTRING); }
#line 11634 "parser.c"
    break;

  case 1155: /* end_unstring: "END-UNSTRING"  */
#line 6106 "parser.y"
                                { terminator_clear (TERM_UNSTRING); }
#line 11640 "parser.c"
    break;

  case 1159: /* use_exception: USE use_global _after _standard exception_or_error _procedure _on use_exception_target  */
#line 6124 "parser.y"
  {
	if (!in_declaratives) {
		cb_error (_("USE statement must be within DECLARATIVES"));
	} else if (!current_section) {
		cb_error (_("SECTION header missing before USE statement"));
	} else {
		current_section->need_begin = 1;
		current_section->need_return = 1;
		CB_EXCEPTION_ENABLE (COB_EC_I_O) = 1;
		if (use_global_ind) {
			current_section->is_global = 1;
			current_program->global_list =
				cb_list_add (current_program->global_list,
					     CB_TREE (current_section));
		}
	}
  }
#line 11662 "parser.c"
    break;

  case 1160: /* use_global: %empty  */
#line 6145 "parser.y"
  {
	use_global_ind = 0;
  }
#line 11670 "parser.c"
    break;

  case 1161: /* use_global: GLOBAL  */
#line 6149 "parser.y"
  {
	use_global_ind = 1;
	current_program->flag_global_use = 1;
  }
#line 11679 "parser.c"
    break;

  case 1162: /* use_exception_target: file_name_list  */
#line 6157 "parser.y"
  {
	cb_tree		l;

	for (l = yyvsp[0]; l; l = CB_CHAIN (l)) {
		if (CB_VALUE (l) != cb_error_node) {
			setup_use_file (CB_FILE (cb_ref (CB_VALUE (l))));
		}
	}
  }
#line 11693 "parser.c"
    break;

  case 1163: /* use_exception_target: INPUT  */
#line 6167 "parser.y"
  {
	current_program->global_handler[COB_OPEN_INPUT].handler_label = current_section;
	current_program->global_handler[COB_OPEN_INPUT].handler_prog = current_program;
  }
#line 11702 "parser.c"
    break;

  case 1164: /* use_exception_target: OUTPUT  */
#line 6172 "parser.y"
  {
	current_program->global_handler[COB_OPEN_OUTPUT].handler_label = current_section;
	current_program->global_handler[COB_OPEN_OUTPUT].handler_prog = current_program;
  }
#line 11711 "parser.c"
    break;

  case 1165: /* use_exception_target: "I-O"  */
#line 6177 "parser.y"
  {
	current_program->global_handler[COB_OPEN_I_O].handler_label = current_section;
	current_program->global_handler[COB_OPEN_I_O].handler_prog = current_program;
  }
#line 11720 "parser.c"
    break;

  case 1166: /* use_exception_target: EXTEND  */
#line 6182 "parser.y"
  {
	current_program->global_handler[COB_OPEN_EXTEND].handler_label = current_section;
	current_program->global_handler[COB_OPEN_EXTEND].handler_prog = current_program;
  }
#line 11729 "parser.c"
    break;

  case 1179: /* use_debugging: USE _for DEBUGGING _on use_debugging_target  */
#line 6214 "parser.y"
  {
	PENDING ("USE FOR DEBUGGING");
  }
#line 11737 "parser.c"
    break;

  case 1182: /* use_reporting: USE use_global BEFORE REPORTING identifier  */
#line 6226 "parser.y"
  {
	PENDING ("USE BEFORE REPORTING");
  }
#line 11745 "parser.c"
    break;

  case 1183: /* $@93: %empty  */
#line 6237 "parser.y"
                                { BEGIN_STATEMENT ("WRITE", TERM_WRITE); }
#line 11751 "parser.c"
    break;

  case 1184: /* write_statement: WRITE $@93 record_name write_from write_lock write_option write_handler end_write  */
#line 6240 "parser.y"
  {
	if (yyvsp[-5] != cb_error_node) {
		if (cb_use_invalidkey_handler_on_status34 &&
		    current_statement->handler_id == COB_EC_I_O_INVALID_KEY &&
		    (CB_FIELD(cb_ref (yyvsp[-5]))->file->organization != COB_ORG_RELATIVE &&
		     CB_FIELD(cb_ref (yyvsp[-5]))->file->organization != COB_ORG_INDEXED)) {
			current_statement->handler_id = COB_EC_I_O_PERMANENT_ERROR;
		}
		cb_emit_write (yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[-3]);
	}
  }
#line 11767 "parser.c"
    break;

  case 1185: /* write_from: %empty  */
#line 6254 "parser.y"
                                { yyval = NULL; }
#line 11773 "parser.c"
    break;

  case 1186: /* write_from: FROM id_or_lit  */
#line 6255 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11779 "parser.c"
    break;

  case 1187: /* write_option: %empty  */
#line 6260 "parser.y"
  {
	yyval = cb_int0;
  }
#line 11787 "parser.c"
    break;

  case 1188: /* write_option: before_or_after _advancing num_id_or_lit _line_or_lines  */
#line 6264 "parser.y"
  {
	yyval = cb_build_write_advancing_lines (yyvsp[-3], yyvsp[-1]);
  }
#line 11795 "parser.c"
    break;

  case 1189: /* write_option: before_or_after _advancing mnemonic_name  */
#line 6268 "parser.y"
  {
	yyval = cb_build_write_advancing_mnemonic (yyvsp[-2], yyvsp[0]);
  }
#line 11803 "parser.c"
    break;

  case 1190: /* write_option: before_or_after _advancing PAGE  */
#line 6272 "parser.y"
  {
	yyval = cb_build_write_advancing_page (yyvsp[-2]);
  }
#line 11811 "parser.c"
    break;

  case 1191: /* before_or_after: BEFORE  */
#line 6278 "parser.y"
                                { yyval = CB_BEFORE; }
#line 11817 "parser.c"
    break;

  case 1192: /* before_or_after: AFTER  */
#line 6279 "parser.y"
                                { yyval = CB_AFTER; }
#line 11823 "parser.c"
    break;

  case 1196: /* end_write: %empty  */
#line 6288 "parser.y"
                                { terminator_warning (TERM_WRITE); }
#line 11829 "parser.c"
    break;

  case 1197: /* end_write: "END-WRITE"  */
#line 6289 "parser.y"
                                { terminator_clear (TERM_WRITE); }
#line 11835 "parser.c"
    break;

  case 1198: /* on_accp_exception: opt_on_exception opt_not_on_exception  */
#line 6304 "parser.y"
  {
	current_statement->handler_id = COB_EC_IMP_ACCEPT;
  }
#line 11843 "parser.c"
    break;

  case 1199: /* on_disp_exception: opt_on_exception opt_not_on_exception  */
#line 6312 "parser.y"
  {
	current_statement->handler_id = COB_EC_IMP_DISPLAY;
  }
#line 11851 "parser.c"
    break;

  case 1201: /* $@94: %empty  */
#line 6319 "parser.y"
  {
	check_unreached = 0;
  }
#line 11859 "parser.c"
    break;

  case 1202: /* opt_on_exception: EXCEPTION $@94 statement_list  */
#line 6323 "parser.y"
  {
	current_statement->handler1 = yyvsp[0];
  }
#line 11867 "parser.c"
    break;

  case 1204: /* $@95: %empty  */
#line 6330 "parser.y"
  {
	check_unreached = 0;
  }
#line 11875 "parser.c"
    break;

  case 1205: /* opt_not_on_exception: "NOT EXCEPTION" $@95 statement_list  */
#line 6334 "parser.y"
  {
	current_statement->handler2 = yyvsp[0];
  }
#line 11883 "parser.c"
    break;

  case 1208: /* $@96: %empty  */
#line 6350 "parser.y"
  {
	check_unreached = 0;
	current_statement->handler_id = COB_EC_SIZE;
  }
#line 11892 "parser.c"
    break;

  case 1209: /* opt_on_size_error: "SIZE ERROR" $@96 statement_list  */
#line 6355 "parser.y"
  {
	current_statement->handler1 = yyvsp[0];
  }
#line 11900 "parser.c"
    break;

  case 1211: /* $@97: %empty  */
#line 6362 "parser.y"
  {
	check_unreached = 0;
	current_statement->handler_id = COB_EC_SIZE;
  }
#line 11909 "parser.c"
    break;

  case 1212: /* opt_not_on_size_error: "NOT SIZE ERROR" $@97 statement_list  */
#line 6367 "parser.y"
  {
	current_statement->handler2 = yyvsp[0];
  }
#line 11917 "parser.c"
    break;

  case 1213: /* on_overflow: opt_on_overflow opt_not_on_overflow  */
#line 6379 "parser.y"
  {
	current_statement->handler_id = COB_EC_OVERFLOW;
  }
#line 11925 "parser.c"
    break;

  case 1215: /* $@98: %empty  */
#line 6386 "parser.y"
  {
	check_unreached = 0;
  }
#line 11933 "parser.c"
    break;

  case 1216: /* opt_on_overflow: OVERFLOW $@98 statement_list  */
#line 6390 "parser.y"
  {
	current_statement->handler1 = yyvsp[0];
  }
#line 11941 "parser.c"
    break;

  case 1218: /* $@99: %empty  */
#line 6397 "parser.y"
  {
	check_unreached = 0;
  }
#line 11949 "parser.c"
    break;

  case 1219: /* opt_not_on_overflow: "NOT OVERFLOW" $@99 statement_list  */
#line 6401 "parser.y"
  {
	current_statement->handler2 = yyvsp[0];
  }
#line 11957 "parser.c"
    break;

  case 1220: /* at_end: at_end_sentence  */
#line 6413 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_AT_END;
	current_statement->handler1 = yyvsp[0];
  }
#line 11966 "parser.c"
    break;

  case 1221: /* at_end: not_at_end_sentence  */
#line 6418 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_AT_END;
	current_statement->handler2 = yyvsp[0];
  }
#line 11975 "parser.c"
    break;

  case 1222: /* at_end: at_end_sentence not_at_end_sentence  */
#line 6423 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_AT_END;
	current_statement->handler1 = yyvsp[-1];
	current_statement->handler2 = yyvsp[0];
  }
#line 11985 "parser.c"
    break;

  case 1223: /* $@100: %empty  */
#line 6432 "parser.y"
  {
	check_unreached = 0;
  }
#line 11993 "parser.c"
    break;

  case 1224: /* at_end_sentence: END $@100 statement_list  */
#line 6436 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12001 "parser.c"
    break;

  case 1225: /* $@101: %empty  */
#line 6443 "parser.y"
  {
	check_unreached = 0;
  }
#line 12009 "parser.c"
    break;

  case 1226: /* not_at_end_sentence: "NOT END" $@101 statement_list  */
#line 6447 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12017 "parser.c"
    break;

  case 1227: /* at_eop: at_eop_sentence  */
#line 6459 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_EOP;
	current_statement->handler1 = yyvsp[0];
  }
#line 12026 "parser.c"
    break;

  case 1228: /* at_eop: not_at_eop_sentence  */
#line 6464 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_EOP;
	current_statement->handler2 = yyvsp[0];
  }
#line 12035 "parser.c"
    break;

  case 1229: /* at_eop: at_eop_sentence not_at_eop_sentence  */
#line 6469 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_EOP;
	current_statement->handler1 = yyvsp[-1];
	current_statement->handler2 = yyvsp[0];
  }
#line 12045 "parser.c"
    break;

  case 1230: /* $@102: %empty  */
#line 6478 "parser.y"
  {
	check_unreached = 0;
  }
#line 12053 "parser.c"
    break;

  case 1231: /* at_eop_sentence: EOP $@102 statement_list  */
#line 6482 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12061 "parser.c"
    break;

  case 1232: /* $@103: %empty  */
#line 6489 "parser.y"
  {
	check_unreached = 0;
  }
#line 12069 "parser.c"
    break;

  case 1233: /* not_at_eop_sentence: "NOT EOP" $@103 statement_list  */
#line 6493 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12077 "parser.c"
    break;

  case 1236: /* invalid_key: invalid_key_sentence  */
#line 6509 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_INVALID_KEY;
	current_statement->handler1 = yyvsp[0];
  }
#line 12086 "parser.c"
    break;

  case 1237: /* invalid_key: not_invalid_key_sentence  */
#line 6514 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_INVALID_KEY;
	current_statement->handler2 = yyvsp[0];
  }
#line 12095 "parser.c"
    break;

  case 1238: /* invalid_key: invalid_key_sentence not_invalid_key_sentence  */
#line 6519 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_INVALID_KEY;
	current_statement->handler1 = yyvsp[-1];
	current_statement->handler2 = yyvsp[0];
  }
#line 12105 "parser.c"
    break;

  case 1239: /* $@104: %empty  */
#line 6528 "parser.y"
  {
	check_unreached = 0;
  }
#line 12113 "parser.c"
    break;

  case 1240: /* invalid_key_sentence: "INVALID KEY" $@104 statement_list  */
#line 6532 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12121 "parser.c"
    break;

  case 1241: /* $@105: %empty  */
#line 6539 "parser.y"
  {
	check_unreached = 0;
  }
#line 12129 "parser.c"
    break;

  case 1242: /* not_invalid_key_sentence: "NOT INVALID KEY" $@105 statement_list  */
#line 6543 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12137 "parser.c"
    break;

  case 1243: /* _opt_scroll_lines: %empty  */
#line 6555 "parser.y"
  {
	yyval = cb_one;
  }
#line 12145 "parser.c"
    break;

  case 1244: /* _opt_scroll_lines: _by num_id_or_lit _line_or_lines  */
#line 6559 "parser.y"
  {
	yyval = yyvsp[-1];
  }
#line 12153 "parser.c"
    break;

  case 1245: /* condition: expr  */
#line 6571 "parser.y"
  {
	yyval = cb_build_cond (yyvsp[0]);
  }
#line 12161 "parser.c"
    break;

  case 1246: /* expr: partial_expr  */
#line 6578 "parser.y"
  {
	yyval = cb_build_expr (yyvsp[0]);
  }
#line 12169 "parser.c"
    break;

  case 1247: /* $@106: %empty  */
#line 6584 "parser.y"
  {
	current_expr = NULL;
  }
#line 12177 "parser.c"
    break;

  case 1248: /* partial_expr: $@106 expr_tokens  */
#line 6588 "parser.y"
  {
	yyval = cb_list_reverse (current_expr);
  }
#line 12185 "parser.c"
    break;

  case 1249: /* expr_tokens: expr_token x  */
#line 6594 "parser.y"
                        { push_expr ('x', yyvsp[0]); }
#line 12191 "parser.c"
    break;

  case 1250: /* expr_tokens: expr_tokens ')'  */
#line 6595 "parser.y"
                        { push_expr (')', NULL); }
#line 12197 "parser.c"
    break;

  case 1251: /* expr_tokens: expr_token OMITTED  */
#line 6597 "parser.y"
                                { push_expr ('O', NULL); }
#line 12203 "parser.c"
    break;

  case 1252: /* expr_tokens: expr_token NUMERIC  */
#line 6598 "parser.y"
                                { push_expr ('9', NULL); }
#line 12209 "parser.c"
    break;

  case 1253: /* expr_tokens: expr_token ALPHABETIC  */
#line 6599 "parser.y"
                                { push_expr ('A', NULL); }
#line 12215 "parser.c"
    break;

  case 1254: /* expr_tokens: expr_token "ALPHABETIC-LOWER"  */
#line 6600 "parser.y"
                                { push_expr ('L', NULL); }
#line 12221 "parser.c"
    break;

  case 1255: /* expr_tokens: expr_token "ALPHABETIC-UPPER"  */
#line 6601 "parser.y"
                                { push_expr ('U', NULL); }
#line 12227 "parser.c"
    break;

  case 1256: /* expr_tokens: expr_token CLASS_NAME  */
#line 6602 "parser.y"
                                { push_expr ('x', yyvsp[0]); }
#line 12233 "parser.c"
    break;

  case 1257: /* expr_tokens: expr_tokens OMITTED  */
#line 6604 "parser.y"
                                        { push_expr ('O', NULL); }
#line 12239 "parser.c"
    break;

  case 1258: /* expr_tokens: expr_tokens NUMERIC  */
#line 6605 "parser.y"
                                        { push_expr ('9', NULL); }
#line 12245 "parser.c"
    break;

  case 1259: /* expr_tokens: expr_tokens ALPHABETIC  */
#line 6606 "parser.y"
                                        { push_expr ('A', NULL); }
#line 12251 "parser.c"
    break;

  case 1260: /* expr_tokens: expr_tokens "ALPHABETIC-LOWER"  */
#line 6607 "parser.y"
                                        { push_expr ('L', NULL); }
#line 12257 "parser.c"
    break;

  case 1261: /* expr_tokens: expr_tokens "ALPHABETIC-UPPER"  */
#line 6608 "parser.y"
                                        { push_expr ('U', NULL); }
#line 12263 "parser.c"
    break;

  case 1262: /* expr_tokens: expr_tokens CLASS_NAME  */
#line 6609 "parser.y"
                                        { push_expr ('x', yyvsp[0]); }
#line 12269 "parser.c"
    break;

  case 1263: /* expr_tokens: expr_token POSITIVE  */
#line 6611 "parser.y"
                        { push_expr ('P', NULL); }
#line 12275 "parser.c"
    break;

  case 1264: /* expr_tokens: expr_token NEGATIVE  */
#line 6612 "parser.y"
                        { push_expr ('N', NULL); }
#line 12281 "parser.c"
    break;

  case 1265: /* expr_tokens: expr_tokens POSITIVE  */
#line 6614 "parser.y"
                        { push_expr ('P', NULL); }
#line 12287 "parser.c"
    break;

  case 1266: /* expr_tokens: expr_tokens NEGATIVE  */
#line 6615 "parser.y"
                        { push_expr ('N', NULL); }
#line 12293 "parser.c"
    break;

  case 1267: /* expr_tokens: expr_tokens ZERO  */
#line 6616 "parser.y"
                        { push_expr ('x', cb_zero); }
#line 12299 "parser.c"
    break;

  case 1271: /* expr_token: expr_token '('  */
#line 6623 "parser.y"
                        { push_expr ('(', NULL); }
#line 12305 "parser.c"
    break;

  case 1272: /* expr_token: expr_token '+'  */
#line 6625 "parser.y"
                        { push_expr ('+', NULL); }
#line 12311 "parser.c"
    break;

  case 1273: /* expr_token: expr_token '-'  */
#line 6626 "parser.y"
                        { push_expr ('-', NULL); }
#line 12317 "parser.c"
    break;

  case 1274: /* expr_token: expr_token '^'  */
#line 6627 "parser.y"
                        { push_expr ('^', NULL); }
#line 12323 "parser.c"
    break;

  case 1275: /* expr_token: expr_token NOT  */
#line 6629 "parser.y"
                        { push_expr ('!', NULL); }
#line 12329 "parser.c"
    break;

  case 1276: /* expr_token: expr_tokens NOT  */
#line 6630 "parser.y"
                        { push_expr ('!', NULL); }
#line 12335 "parser.c"
    break;

  case 1277: /* expr_token: expr_tokens '+'  */
#line 6632 "parser.y"
                        { push_expr ('+', NULL); }
#line 12341 "parser.c"
    break;

  case 1278: /* expr_token: expr_tokens '-'  */
#line 6633 "parser.y"
                        { push_expr ('-', NULL); }
#line 12347 "parser.c"
    break;

  case 1279: /* expr_token: expr_tokens '*'  */
#line 6634 "parser.y"
                        { push_expr ('*', NULL); }
#line 12353 "parser.c"
    break;

  case 1280: /* expr_token: expr_tokens '/'  */
#line 6635 "parser.y"
                        { push_expr ('/', NULL); }
#line 12359 "parser.c"
    break;

  case 1281: /* expr_token: expr_tokens '^'  */
#line 6636 "parser.y"
                        { push_expr ('^', NULL); }
#line 12365 "parser.c"
    break;

  case 1282: /* expr_token: expr_tokens eq  */
#line 6638 "parser.y"
                        { push_expr ('=', NULL); }
#line 12371 "parser.c"
    break;

  case 1283: /* expr_token: expr_tokens gt  */
#line 6639 "parser.y"
                        { push_expr ('>', NULL); }
#line 12377 "parser.c"
    break;

  case 1284: /* expr_token: expr_tokens lt  */
#line 6640 "parser.y"
                        { push_expr ('<', NULL); }
#line 12383 "parser.c"
    break;

  case 1285: /* expr_token: expr_tokens ge  */
#line 6641 "parser.y"
                        { push_expr (']', NULL); }
#line 12389 "parser.c"
    break;

  case 1286: /* expr_token: expr_tokens le  */
#line 6642 "parser.y"
                        { push_expr ('[', NULL); }
#line 12395 "parser.c"
    break;

  case 1287: /* expr_token: expr_tokens NE  */
#line 6643 "parser.y"
                        { push_expr ('~', NULL); }
#line 12401 "parser.c"
    break;

  case 1288: /* expr_token: expr_token eq  */
#line 6645 "parser.y"
                        { push_expr ('=', NULL); }
#line 12407 "parser.c"
    break;

  case 1289: /* expr_token: expr_token gt  */
#line 6646 "parser.y"
                        { push_expr ('>', NULL); }
#line 12413 "parser.c"
    break;

  case 1290: /* expr_token: expr_token lt  */
#line 6647 "parser.y"
                        { push_expr ('<', NULL); }
#line 12419 "parser.c"
    break;

  case 1291: /* expr_token: expr_token ge  */
#line 6648 "parser.y"
                        { push_expr (']', NULL); }
#line 12425 "parser.c"
    break;

  case 1292: /* expr_token: expr_token le  */
#line 6649 "parser.y"
                        { push_expr ('[', NULL); }
#line 12431 "parser.c"
    break;

  case 1293: /* expr_token: expr_token NE  */
#line 6650 "parser.y"
                        { push_expr ('~', NULL); }
#line 12437 "parser.c"
    break;

  case 1294: /* expr_token: expr_tokens AND  */
#line 6652 "parser.y"
                        { push_expr ('&', NULL); }
#line 12443 "parser.c"
    break;

  case 1295: /* expr_token: expr_tokens OR  */
#line 6653 "parser.y"
                        { push_expr ('|', NULL); }
#line 12449 "parser.c"
    break;

  case 1309: /* exp_list: exp  */
#line 6665 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12455 "parser.c"
    break;

  case 1310: /* exp_list: exp_list e_sep exp  */
#line 6666 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 12461 "parser.c"
    break;

  case 1314: /* exp: arith_x  */
#line 6675 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12467 "parser.c"
    break;

  case 1315: /* exp: exp '+' exp  */
#line 6676 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '+', yyvsp[0]); }
#line 12473 "parser.c"
    break;

  case 1316: /* exp: exp '-' exp  */
#line 6677 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '-', yyvsp[0]); }
#line 12479 "parser.c"
    break;

  case 1317: /* exp: exp '*' exp  */
#line 6678 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '*', yyvsp[0]); }
#line 12485 "parser.c"
    break;

  case 1318: /* exp: exp '/' exp  */
#line 6679 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '/', yyvsp[0]); }
#line 12491 "parser.c"
    break;

  case 1319: /* exp: '+' exp  */
#line 6680 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12497 "parser.c"
    break;

  case 1320: /* exp: '-' exp  */
#line 6681 "parser.y"
                                { yyval = cb_build_binary_op (cb_zero, '-', yyvsp[0]); }
#line 12503 "parser.c"
    break;

  case 1321: /* exp: exp '^' exp  */
#line 6682 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '^', yyvsp[0]); }
#line 12509 "parser.c"
    break;

  case 1322: /* exp: '(' exp ')'  */
#line 6683 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 12515 "parser.c"
    break;

  case 1323: /* linage_counter: "LINAGE-COUNTER"  */
#line 6695 "parser.y"
  {
	if (current_linage > 1) {
		cb_error (_("LINAGE-COUNTER must be qualified here"));
		yyval = cb_error_node;
	} else if (current_linage == 0) {
		cb_error (_("Invalid LINAGE-COUNTER usage"));
		yyval = cb_error_node;
	} else {
		yyval = linage_file->linage_ctr;
	}
  }
#line 12531 "parser.c"
    break;

  case 1324: /* linage_counter: "LINAGE-COUNTER" in_of "Identifier"  */
#line 6707 "parser.y"
  {
	if (CB_FILE_P (cb_ref (yyvsp[0]))) {
		yyval = CB_FILE (cb_ref (yyvsp[0]))->linage_ctr;
	} else {
		cb_error_x (yyvsp[0], _("'%s' is not a file name"), CB_NAME (yyvsp[0]));
		yyval = cb_error_node;
	}
  }
#line 12544 "parser.c"
    break;

  case 1325: /* arithmetic_x_list: arithmetic_x  */
#line 6721 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12550 "parser.c"
    break;

  case 1326: /* arithmetic_x_list: arithmetic_x_list arithmetic_x  */
#line 6723 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 12556 "parser.c"
    break;

  case 1327: /* arithmetic_x: x flag_rounded  */
#line 6727 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], yyvsp[-1]); }
#line 12562 "parser.c"
    break;

  case 1328: /* record_name: qualified_word  */
#line 6734 "parser.y"
  {
	cb_tree x;
	cb_tree r;

	if ((x = cb_build_identifier (yyvsp[0])) != cb_error_node &&
	    (r = cb_ref (x)) != cb_error_node) {
		if (!CB_FIELD_P(r)) {
			cb_error_x (x, _("Field name expected."));
			x = cb_error_node;
		} else if (!CB_FIELD(r)->file) {
			cb_error_x (x, _("Record name expected."));
			x = cb_error_node;
		}
	}
	yyval = x;
  }
#line 12583 "parser.c"
    break;

  case 1329: /* table_name: qualified_word  */
#line 6756 "parser.y"
  {
	cb_tree x;

	x = cb_ref (yyvsp[0]);
	if (!CB_FIELD_P (x)) {
		yyval = cb_error_node;
	} else if (!CB_FIELD (x)->index_list) {
		cb_error_x (yyvsp[0], _("'%s' not indexed"), cb_name (yyvsp[0]));
		cb_error_x (x, _("'%s' defined here"), cb_name (x));
		yyval = cb_error_node;
	} else {
		yyval = yyvsp[0];
	}
  }
#line 12602 "parser.c"
    break;

  case 1330: /* file_name_list: file_name  */
#line 6776 "parser.y"
  {
	yyval = cb_list_init (yyvsp[0]);
  }
#line 12610 "parser.c"
    break;

  case 1331: /* file_name_list: file_name_list file_name  */
#line 6780 "parser.y"
  {
	cb_tree		l;

	if (yyvsp[0] != cb_error_node) {
		for (l = yyvsp[-1]; l; l = CB_CHAIN (l)) {
			if (!strcasecmp (CB_NAME (yyvsp[0]), CB_NAME (CB_VALUE (l)))) {
				cb_error_x (yyvsp[0], _("Multiple reference to '%s' "), CB_NAME (yyvsp[0]));
			}
		}
		yyval = cb_list_add (yyvsp[-1], yyvsp[0]);
	}
  }
#line 12627 "parser.c"
    break;

  case 1332: /* file_name: "Identifier"  */
#line 6796 "parser.y"
  {
	if (CB_FILE_P (cb_ref (yyvsp[0]))) {
		yyval = yyvsp[0];
	} else {
		cb_error_x (yyvsp[0], _("'%s' is not a file name"), CB_NAME (yyvsp[0]));
		yyval = cb_error_node;
	}
  }
#line 12640 "parser.c"
    break;

  case 1333: /* mnemonic_name_list: mnemonic_name  */
#line 6809 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12646 "parser.c"
    break;

  case 1334: /* mnemonic_name_list: mnemonic_name_list mnemonic_name  */
#line 6811 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12652 "parser.c"
    break;

  case 1335: /* mnemonic_name: "MNEMONIC NAME"  */
#line 6815 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12658 "parser.c"
    break;

  case 1336: /* procedure_name_list: %empty  */
#line 6821 "parser.y"
                                { yyval = NULL; }
#line 12664 "parser.c"
    break;

  case 1337: /* procedure_name_list: procedure_name_list procedure_name  */
#line 6823 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12670 "parser.c"
    break;

  case 1338: /* procedure_name: label  */
#line 6828 "parser.y"
  {
	yyval = yyvsp[0];
	CB_REFERENCE (yyval)->offset = CB_TREE (current_section);
	current_program->label_list = cb_cons (yyval, current_program->label_list);
  }
#line 12680 "parser.c"
    break;

  case 1342: /* integer_label: "Literal"  */
#line 6843 "parser.y"
  {
	yyval = cb_build_reference ((char *)(CB_LITERAL (yyvsp[0])->data));
	yyval->source_file = yyvsp[0]->source_file;
	yyval->source_line = yyvsp[0]->source_line;
  }
#line 12690 "parser.c"
    break;

  case 1343: /* reference_list: reference  */
#line 6853 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12696 "parser.c"
    break;

  case 1344: /* reference_list: reference_list reference  */
#line 6854 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12702 "parser.c"
    break;

  case 1345: /* reference: qualified_word  */
#line 6859 "parser.y"
  {
	yyval = yyvsp[0];
	current_program->reference_list = cb_cons (yyval, current_program->reference_list);
  }
#line 12711 "parser.c"
    break;

  case 1346: /* no_reference_list: qualified_word  */
#line 6868 "parser.y"
                                        { yyval = cb_list_init (yyvsp[0]); }
#line 12717 "parser.c"
    break;

  case 1347: /* no_reference_list: no_reference_list qualified_word  */
#line 6869 "parser.y"
                                        { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12723 "parser.c"
    break;

  case 1348: /* opt_reference: %empty  */
#line 6873 "parser.y"
                                { yyval = NULL; }
#line 12729 "parser.c"
    break;

  case 1349: /* opt_reference: reference  */
#line 6874 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12735 "parser.c"
    break;

  case 1352: /* undefined_word: "Identifier"  */
#line 6886 "parser.y"
  {
	yyval = yyvsp[0];
	if (CB_REFERENCE (yyval)->word->count > 0) {
		redefinition_error (yyval);
		yyval = cb_error_node;
	}
  }
#line 12747 "parser.c"
    break;

  case 1353: /* target_x_list: target_x  */
#line 6905 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12753 "parser.c"
    break;

  case 1354: /* target_x_list: target_x_list target_x  */
#line 6906 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12759 "parser.c"
    break;

  case 1356: /* target_x: ADDRESS _of identifier_1  */
#line 6911 "parser.y"
                                { yyval = cb_build_address (yyvsp[0]); }
#line 12765 "parser.c"
    break;

  case 1357: /* x_list: x  */
#line 6915 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12771 "parser.c"
    break;

  case 1358: /* x_list: x_list x  */
#line 6916 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12777 "parser.c"
    break;

  case 1360: /* x: LENGTH _of identifier_1  */
#line 6921 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12783 "parser.c"
    break;

  case 1361: /* x: LENGTH _of basic_literal  */
#line 6922 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12789 "parser.c"
    break;

  case 1362: /* x: LENGTH _of function  */
#line 6923 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12795 "parser.c"
    break;

  case 1363: /* x: ADDRESS _of prog_or_entry alnum_or_id  */
#line 6924 "parser.y"
                                                { yyval = cb_build_ppointer (yyvsp[0]); }
#line 12801 "parser.c"
    break;

  case 1364: /* x: ADDRESS _of identifier_1  */
#line 6925 "parser.y"
                                                { yyval = cb_build_address (yyvsp[0]); }
#line 12807 "parser.c"
    break;

  case 1369: /* arith_x: LENGTH _of identifier_1  */
#line 6933 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12813 "parser.c"
    break;

  case 1370: /* arith_x: LENGTH _of basic_literal  */
#line 6934 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12819 "parser.c"
    break;

  case 1371: /* arith_x: LENGTH _of function  */
#line 6935 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12825 "parser.c"
    break;

  case 1377: /* alnum_or_id: identifier_1  */
#line 6947 "parser.y"
                        { yyval = yyvsp[0]; }
#line 12831 "parser.c"
    break;

  case 1378: /* alnum_or_id: "Literal"  */
#line 6948 "parser.y"
                        { yyval = yyvsp[0]; }
#line 12837 "parser.c"
    break;

  case 1390: /* num_id_or_lit: ZERO  */
#line 6982 "parser.y"
                                { yyval = cb_zero; }
#line 12843 "parser.c"
    break;

  case 1391: /* identifier: identifier_1  */
#line 6990 "parser.y"
                                { yyval = cb_build_identifier (yyvsp[0]); }
#line 12849 "parser.c"
    break;

  case 1392: /* identifier_1: qualified_word  */
#line 6994 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12855 "parser.c"
    break;

  case 1393: /* identifier_1: qualified_word subref  */
#line 6995 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 12861 "parser.c"
    break;

  case 1394: /* identifier_1: qualified_word refmod  */
#line 6996 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 12867 "parser.c"
    break;

  case 1395: /* identifier_1: qualified_word subref refmod  */
#line 6997 "parser.y"
                                { yyval = yyvsp[-2]; }
#line 12873 "parser.c"
    break;

  case 1396: /* qualified_word: "Identifier"  */
#line 7001 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12879 "parser.c"
    break;

  case 1397: /* qualified_word: "Identifier" in_of qualified_word  */
#line 7002 "parser.y"
                                { yyval = yyvsp[-2]; CB_REFERENCE (yyvsp[-2])->chain = yyvsp[0]; }
#line 12885 "parser.c"
    break;

  case 1398: /* subref: '(' exp_list ')'  */
#line 7007 "parser.y"
  {
	if (cb_ref (yyvsp[-3]) != cb_error_node) {
		yyval = yyvsp[-3];
		CB_REFERENCE (yyvsp[-3])->subs = cb_list_reverse (yyvsp[-1]);
	}
  }
#line 12896 "parser.c"
    break;

  case 1399: /* refmod: '(' exp ':' ')'  */
#line 7017 "parser.y"
  {
	if (cb_ref (yyvsp[-4]) != cb_error_node) {
		CB_REFERENCE (yyvsp[-4])->value = CB_TREE (cb_field (yyvsp[-4]));
		if (cb_tree_category (yyvsp[-4]) == CB_CATEGORY_NATIONAL ||
		    cb_tree_category (yyvsp[-4]) == CB_CATEGORY_NATIONAL_EDITED) {
			yyvsp[-2] = cb_build_binary_op (yyvsp[-2], '*', cb_int2);
			yyvsp[-2] = cb_build_binary_op (yyvsp[-2], '-', cb_int1);
		} else {
			CB_TREE (yyvsp[-4])->category = CB_CATEGORY_ALPHANUMERIC;
		}
		CB_REFERENCE (yyvsp[-4])->offset = yyvsp[-2];
	}
  }
#line 12914 "parser.c"
    break;

  case 1400: /* refmod: '(' exp ':' exp ')'  */
#line 7031 "parser.y"
  {
	if (cb_ref (yyvsp[-5]) != cb_error_node) {
		CB_REFERENCE (yyvsp[-5])->value = CB_TREE (cb_field (yyvsp[-5]));
		if (cb_tree_category (yyvsp[-5]) == CB_CATEGORY_NATIONAL ||
		    cb_tree_category (yyvsp[-5]) == CB_CATEGORY_NATIONAL_EDITED) {
			yyvsp[-3] = cb_build_binary_op (yyvsp[-3], '*', cb_int2);
			yyvsp[-3] = cb_build_binary_op (yyvsp[-3], '-', cb_int1);
			yyvsp[-1] = cb_build_binary_op (yyvsp[-1], '*', cb_int2);
		} else {
			CB_TREE (yyvsp[-5])->category = CB_CATEGORY_ALPHANUMERIC;
		}
		CB_REFERENCE (yyvsp[-5])->offset = yyvsp[-3];
		CB_REFERENCE (yyvsp[-5])->length = yyvsp[-1];
	}
  }
#line 12934 "parser.c"
    break;

  case 1401: /* integer: "Literal"  */
#line 7054 "parser.y"
  {
	if (cb_tree_category (yyvsp[0]) != CB_CATEGORY_NUMERIC) {
		cb_error (_("Integer value expected"));
	} else if (CB_LITERAL (yyvsp[0])->sign < 0 || CB_LITERAL (yyvsp[0])->scale) {
		cb_error (_("Integer value expected"));
	}
	yyval = yyvsp[0];
  }
#line 12947 "parser.c"
    break;

  case 1402: /* literal: basic_literal  */
#line 7065 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12953 "parser.c"
    break;

  case 1403: /* literal: ALL basic_value  */
#line 7067 "parser.y"
  {
	yyval = yyvsp[0];
	if (CB_LITERAL_P (yyvsp[0])) {
		CB_LITERAL (yyvsp[0])->all = 1;
	}
  }
#line 12964 "parser.c"
    break;

  case 1404: /* basic_literal: basic_value  */
#line 7076 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12970 "parser.c"
    break;

  case 1405: /* basic_literal: basic_literal '&' basic_value  */
#line 7077 "parser.y"
                                { yyval = cb_concat_literals (yyvsp[-2], yyvsp[0]); }
#line 12976 "parser.c"
    break;

  case 1406: /* basic_value: "Literal"  */
#line 7081 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12982 "parser.c"
    break;

  case 1407: /* basic_value: SPACE  */
#line 7082 "parser.y"
                                { yyval = cb_space; }
#line 12988 "parser.c"
    break;

  case 1408: /* basic_value: ZERO  */
#line 7083 "parser.y"
                                { yyval = cb_zero; }
#line 12994 "parser.c"
    break;

  case 1409: /* basic_value: QUOTE  */
#line 7084 "parser.y"
                                { yyval = cb_quote; }
#line 13000 "parser.c"
    break;

  case 1410: /* basic_value: "HIGH-VALUE"  */
#line 7085 "parser.y"
                                { yyval = cb_high; }
#line 13006 "parser.c"
    break;

  case 1411: /* basic_value: "LOW-VALUE"  */
#line 7086 "parser.y"
                                { yyval = cb_low; }
#line 13012 "parser.c"
    break;

  case 1412: /* basic_value: "NULL"  */
#line 7087 "parser.y"
                                { yyval = cb_null; }
#line 13018 "parser.c"
    break;

  case 1413: /* function: "FUNCTION CURRENT-DATE" func_refmod  */
#line 7096 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-1], NULL, yyvsp[0]);
  }
#line 13026 "parser.c"
    break;

  case 1414: /* function: "FUNCTION WHEN-COMPILED" func_refmod  */
#line 7100 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-1], NULL, yyvsp[0]);
  }
#line 13034 "parser.c"
    break;

  case 1415: /* function: "FUNCTION UPPER-CASE" '(' exp ')' func_refmod  */
#line 7104 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], cb_list_init (yyvsp[-2]), yyvsp[0]);
  }
#line 13042 "parser.c"
    break;

  case 1416: /* function: "FUNCTION LOWER-CASE" '(' exp ')' func_refmod  */
#line 7108 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], cb_list_init (yyvsp[-2]), yyvsp[0]);
  }
#line 13050 "parser.c"
    break;

  case 1417: /* function: "FUNCTION REVERSE" '(' exp ')' func_refmod  */
#line 7112 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], cb_list_init (yyvsp[-2]), yyvsp[0]);
  }
#line 13058 "parser.c"
    break;

  case 1418: /* function: "FUNCTION CONCATENATE" '(' exp_list ')' func_refmod  */
#line 7116 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13066 "parser.c"
    break;

  case 1419: /* function: "FUNCTION SUBSTITUTE" '(' exp_list ')' func_refmod  */
#line 7120 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13074 "parser.c"
    break;

  case 1420: /* function: "FUNCTION SUBSTITUTE-CASE" '(' exp_list ')' func_refmod  */
#line 7124 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13082 "parser.c"
    break;

  case 1421: /* function: "FUNCTION TRIM" '(' trim_args ')' func_refmod  */
#line 7128 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13090 "parser.c"
    break;

  case 1422: /* function: "FUNCTION NUMVALC" '(' numvalc_args ')'  */
#line 7132 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-3], yyvsp[-1], NULL);
  }
#line 13098 "parser.c"
    break;

  case 1423: /* function: "FUNCTION LOCALE" '(' locale_dt_args ')' func_refmod  */
#line 7136 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13106 "parser.c"
    break;

  case 1424: /* function: "FUNCTION" func_args  */
#line 7140 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-1], yyvsp[0], NULL);
  }
#line 13114 "parser.c"
    break;

  case 1425: /* func_refmod: %empty  */
#line 7146 "parser.y"
                                { yyval = NULL; }
#line 13120 "parser.c"
    break;

  case 1426: /* func_refmod: '(' exp ':' ')'  */
#line 7147 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-2], NULL); }
#line 13126 "parser.c"
    break;

  case 1427: /* func_refmod: '(' exp ':' exp ')'  */
#line 7148 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-3], yyvsp[-1]); }
#line 13132 "parser.c"
    break;

  case 1428: /* func_args: %empty  */
#line 7152 "parser.y"
                                { yyval = NULL; }
#line 13138 "parser.c"
    break;

  case 1429: /* func_args: '(' list_func_args ')'  */
#line 7153 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 13144 "parser.c"
    break;

  case 1430: /* list_func_args: %empty  */
#line 7157 "parser.y"
                                { yyval = NULL; }
#line 13150 "parser.c"
    break;

  case 1431: /* list_func_args: exp_list  */
#line 7158 "parser.y"
                                { yyval = yyvsp[0]; }
#line 13156 "parser.c"
    break;

  case 1432: /* trim_args: exp  */
#line 7164 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[0]);
	yyval = cb_list_add (x, cb_int0);
  }
#line 13167 "parser.c"
    break;

  case 1433: /* trim_args: exp e_sep LEADING  */
#line 7171 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, cb_int1);
  }
#line 13178 "parser.c"
    break;

  case 1434: /* trim_args: exp e_sep TRAILING  */
#line 7178 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, cb_int2);
  }
#line 13189 "parser.c"
    break;

  case 1435: /* numvalc_args: exp  */
#line 7188 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[0]);
	yyval = cb_list_add (x, cb_null);
  }
#line 13200 "parser.c"
    break;

  case 1436: /* numvalc_args: exp e_sep exp  */
#line 7195 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, yyvsp[0]);
  }
#line 13211 "parser.c"
    break;

  case 1437: /* locale_dt_args: exp  */
#line 7205 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[0]);
	yyval = cb_list_add (x, cb_null);
  }
#line 13222 "parser.c"
    break;

  case 1438: /* locale_dt_args: exp e_sep reference  */
#line 7212 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, cb_ref (yyvsp[0]));
  }
#line 13233 "parser.c"
    break;

  case 1439: /* not_const_word: %empty  */
#line 7225 "parser.y"
  {
	non_const_word = 1;
  }
#line 13241 "parser.c"
    break;

  case 1440: /* flag_all: %empty  */
#line 7235 "parser.y"
                                { yyval = cb_int0; }
#line 13247 "parser.c"
    break;

  case 1441: /* flag_all: ALL  */
#line 7236 "parser.y"
                                { yyval = cb_int1; }
#line 13253 "parser.c"
    break;

  case 1442: /* flag_duplicates: %empty  */
#line 7240 "parser.y"
                                { yyval = cb_int0; }
#line 13259 "parser.c"
    break;

  case 1443: /* flag_duplicates: with_dups  */
#line 7241 "parser.y"
                                { yyval = cb_int1; }
#line 13265 "parser.c"
    break;

  case 1444: /* flag_initialized: %empty  */
#line 7245 "parser.y"
                                { yyval = NULL; }
#line 13271 "parser.c"
    break;

  case 1445: /* flag_initialized: INITIALIZED  */
#line 7246 "parser.y"
                                { yyval = cb_int1; }
#line 13277 "parser.c"
    break;

  case 1446: /* flag_next: %empty  */
#line 7250 "parser.y"
                                { yyval = cb_int0; }
#line 13283 "parser.c"
    break;

  case 1447: /* flag_next: NEXT  */
#line 7251 "parser.y"
                                { yyval = cb_int1; }
#line 13289 "parser.c"
    break;

  case 1448: /* flag_next: PREVIOUS  */
#line 7252 "parser.y"
                                { yyval = cb_int2; }
#line 13295 "parser.c"
    break;

  case 1449: /* flag_not: %empty  */
#line 7256 "parser.y"
                                { yyval = cb_int0; }
#line 13301 "parser.c"
    break;

  case 1450: /* flag_not: NOT  */
#line 7257 "parser.y"
                                { yyval = cb_int1; }
#line 13307 "parser.c"
    break;

  case 1451: /* flag_optional: %empty  */
#line 7261 "parser.y"
                                { yyval = cb_int0; }
#line 13313 "parser.c"
    break;

  case 1452: /* flag_optional: OPTIONAL  */
#line 7262 "parser.y"
                                { yyval = cb_int1; }
#line 13319 "parser.c"
    break;

  case 1453: /* flag_rounded: %empty  */
#line 7266 "parser.y"
                                { yyval = cb_int0; }
#line 13325 "parser.c"
    break;

  case 1454: /* flag_rounded: ROUNDED  */
#line 7267 "parser.y"
                                { yyval = cb_int1; }
#line 13331 "parser.c"
    break;

  case 1455: /* flag_separate: %empty  */
#line 7271 "parser.y"
                                { yyval = cb_int0; }
#line 13337 "parser.c"
    break;

  case 1456: /* flag_separate: SEPARATE _character  */
#line 7272 "parser.y"
                                { yyval = cb_int1; }
#line 13343 "parser.c"
    break;

  case 1468: /* _also: ALSO  */
#line 7285 "parser.y"
                       { yyval = cb_int1; }
#line 13349 "parser.c"
    break;

  case 1497: /* _is: %empty  */
#line 7300 "parser.y"
                { yyval = NULL; }
#line 13355 "parser.c"
    break;

  case 1498: /* _is: IS  */
#line 7300 "parser.y"
                                    { yyval = cb_int1; }
#line 13361 "parser.c"
    break;

  case 1509: /* _literal: %empty  */
#line 7305 "parser.y"
                { yyval = NULL; }
#line 13367 "parser.c"
    break;

  case 1510: /* _literal: "Literal"  */
#line 7305 "parser.y"
                                         { yyval = yyvsp[0]; }
#line 13373 "parser.c"
    break;

  case 1555: /* exec_sql_statement: "EXEC SQL statement"  */
#line 7335 "parser.y"
  {
	cb_tree sql_node;
	/* 実際の埋め込み SQL (PROCEDURE DIVISION の EXEC SQL) を初めて検出した
	   時点で SQLCA を一度だけ暗黙に注入する。EXEC SQL INCLUDE SQLCA END-EXEC が
	   書かれていない場合はコンパイル時に警告する (INCLUDE SQLCA の有無は
	   前処理段で cb_sqlca_include_seen に記録される)。 */
	if (!esql_sqlca_injected) {
		esql_sqlca_injected = 1;
		esql_inject_sqlca ();
		if (!cb_sqlca_include_seen) {
			cb_warning_x (yyvsp[0],
				_("embedded SQL is used without 'EXEC SQL INCLUDE SQLCA END-EXEC'; SQLCA is declared implicitly"));
		}
	}
	BEGIN_STATEMENT ("EXEC SQL", 0);
	/* BEGIN_STATEMENT は cb_source_line (= END-EXEC 行) を入れる。
	   $1 のリテラルには scanner が EXEC SQL 開始行を入れているので、
	   そちらで上書きして「コメントは EXEC SQL の行を指す」ようにする。 */
	if (yyvsp[0]->source_line) {
		CB_TREE (current_statement)->source_line = yyvsp[0]->source_line;
	}
	sql_node = cb_parse_exec_sql ((char *)CB_LITERAL (yyvsp[0])->data);
	if (sql_node != cb_error_node) {
		current_statement->body =
			cb_list_add (current_statement->body, sql_node);
	}
  }
#line 13405 "parser.c"
    break;

  case 1556: /* exec_java_statement: "EXEC JAVA statement"  */
#line 7370 "parser.y"
  {
	cb_tree java_node;
	BEGIN_STATEMENT ("EXEC JAVA", 0);
	/* BEGIN_STATEMENT は cb_source_line (= END-EXEC 行) を入れる。
	   $1 のリテラルには scanner が EXEC JAVA 開始行を入れているので、
	   そちらで上書きして「コメントは EXEC JAVA の行を指す」ようにする。 */
	if (yyvsp[0]->source_line) {
		CB_TREE (current_statement)->source_line = yyvsp[0]->source_line;
	}
	java_node = cb_build_exec_java (yyvsp[0]);
	if (java_node != cb_error_node) {
		current_statement->body =
			cb_list_add (current_statement->body, java_node);
	}
  }
#line 13425 "parser.c"
    break;


#line 13429 "parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= TOKEN_EOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == TOKEN_EOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 7388 "parser.y"


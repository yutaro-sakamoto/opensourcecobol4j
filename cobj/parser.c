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


#line 355 "parser.c"

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
  YYSYMBOL_450_ = 450,                     /* '+'  */
  YYSYMBOL_451_ = 451,                     /* '-'  */
  YYSYMBOL_452_ = 452,                     /* '*'  */
  YYSYMBOL_453_ = 453,                     /* '/'  */
  YYSYMBOL_UNARY_SIGN = 454,               /* UNARY_SIGN  */
  YYSYMBOL_455_ = 455,                     /* '^'  */
  YYSYMBOL_456_ = 456,                     /* '.'  */
  YYSYMBOL_457_ = 457,                     /* '='  */
  YYSYMBOL_458_ = 458,                     /* ')'  */
  YYSYMBOL_459_ = 459,                     /* '('  */
  YYSYMBOL_460_ = 460,                     /* '>'  */
  YYSYMBOL_461_ = 461,                     /* '<'  */
  YYSYMBOL_462_ = 462,                     /* ':'  */
  YYSYMBOL_463_ = 463,                     /* '&'  */
  YYSYMBOL_YYACCEPT = 464,                 /* $accept  */
  YYSYMBOL_start = 465,                    /* start  */
  YYSYMBOL_466_1 = 466,                    /* $@1  */
  YYSYMBOL_nested_list = 467,              /* nested_list  */
  YYSYMBOL_source_element = 468,           /* source_element  */
  YYSYMBOL_program_definition = 469,       /* program_definition  */
  YYSYMBOL_470_2 = 470,                    /* $@2  */
  YYSYMBOL_471_3 = 471,                    /* $@3  */
  YYSYMBOL_program_mandatory = 472,        /* program_mandatory  */
  YYSYMBOL_473_4 = 473,                    /* $@4  */
  YYSYMBOL_474_5 = 474,                    /* $@5  */
  YYSYMBOL_function_definition = 475,      /* function_definition  */
  YYSYMBOL_476_6 = 476,                    /* $@6  */
  YYSYMBOL_477_7 = 477,                    /* $@7  */
  YYSYMBOL_nested_prog = 478,              /* nested_prog  */
  YYSYMBOL_end_program = 479,              /* end_program  */
  YYSYMBOL_end_mandatory = 480,            /* end_mandatory  */
  YYSYMBOL_end_function = 481,             /* end_function  */
  YYSYMBOL_identification_division = 482,  /* identification_division  */
  YYSYMBOL_483_8 = 483,                    /* $@8  */
  YYSYMBOL_function_division = 484,        /* function_division  */
  YYSYMBOL_program_name = 485,             /* program_name  */
  YYSYMBOL_as_literal = 486,               /* as_literal  */
  YYSYMBOL_program_type = 487,             /* program_type  */
  YYSYMBOL_program_type_clause = 488,      /* program_type_clause  */
  YYSYMBOL__init_or_recurs = 489,          /* _init_or_recurs  */
  YYSYMBOL_environment_division = 490,     /* environment_division  */
  YYSYMBOL_configuration_section = 491,    /* configuration_section  */
  YYSYMBOL_configuration_list = 492,       /* configuration_list  */
  YYSYMBOL_configuration_paragraph = 493,  /* configuration_paragraph  */
  YYSYMBOL_source_computer_paragraph = 494, /* source_computer_paragraph  */
  YYSYMBOL_source_computer_entry = 495,    /* source_computer_entry  */
  YYSYMBOL_with_debugging_mode = 496,      /* with_debugging_mode  */
  YYSYMBOL_computer_name = 497,            /* computer_name  */
  YYSYMBOL_object_computer_paragraph = 498, /* object_computer_paragraph  */
  YYSYMBOL_object_computer_entry = 499,    /* object_computer_entry  */
  YYSYMBOL_object_clauses_list = 500,      /* object_clauses_list  */
  YYSYMBOL_object_clauses = 501,           /* object_clauses  */
  YYSYMBOL_object_computer_memory = 502,   /* object_computer_memory  */
  YYSYMBOL_object_char_or_word = 503,      /* object_char_or_word  */
  YYSYMBOL_object_computer_sequence = 504, /* object_computer_sequence  */
  YYSYMBOL_object_computer_segment = 505,  /* object_computer_segment  */
  YYSYMBOL_repository_paragraph = 506,     /* repository_paragraph  */
  YYSYMBOL_opt_repository = 507,           /* opt_repository  */
  YYSYMBOL_repository_list = 508,          /* repository_list  */
  YYSYMBOL_repository_name = 509,          /* repository_name  */
  YYSYMBOL_repository_literal_list = 510,  /* repository_literal_list  */
  YYSYMBOL_special_names_paragraph = 511,  /* special_names_paragraph  */
  YYSYMBOL_opt_special_names = 512,        /* opt_special_names  */
  YYSYMBOL_special_name_list = 513,        /* special_name_list  */
  YYSYMBOL_special_name = 514,             /* special_name  */
  YYSYMBOL_mnemonic_name_clause = 515,     /* mnemonic_name_clause  */
  YYSYMBOL_516_9 = 516,                    /* $@9  */
  YYSYMBOL_517_10 = 517,                   /* $@10  */
  YYSYMBOL_special_name_mnemonic_on_off_list = 518, /* special_name_mnemonic_on_off_list  */
  YYSYMBOL_special_name_mnemonic_on_off_list_mandatory = 519, /* special_name_mnemonic_on_off_list_mandatory  */
  YYSYMBOL_special_name_mnemonic_on_off = 520, /* special_name_mnemonic_on_off  */
  YYSYMBOL_on_or_off = 521,                /* on_or_off  */
  YYSYMBOL_alphabet_name_clause = 522,     /* alphabet_name_clause  */
  YYSYMBOL_523_11 = 523,                   /* $@11  */
  YYSYMBOL_alphabet_definition = 524,      /* alphabet_definition  */
  YYSYMBOL_alphabet_literal_list = 525,    /* alphabet_literal_list  */
  YYSYMBOL_alphabet_literal = 526,         /* alphabet_literal  */
  YYSYMBOL_527_12 = 527,                   /* @12  */
  YYSYMBOL_alphabet_also_sequence = 528,   /* alphabet_also_sequence  */
  YYSYMBOL_alphabet_lits = 529,            /* alphabet_lits  */
  YYSYMBOL_alphabet_also_literal = 530,    /* alphabet_also_literal  */
  YYSYMBOL_symbolic_characters_clause = 531, /* symbolic_characters_clause  */
  YYSYMBOL_symbolic_characters_list = 532, /* symbolic_characters_list  */
  YYSYMBOL_char_list = 533,                /* char_list  */
  YYSYMBOL_integer_list = 534,             /* integer_list  */
  YYSYMBOL_class_name_clause = 535,        /* class_name_clause  */
  YYSYMBOL_class_item_list = 536,          /* class_item_list  */
  YYSYMBOL_class_item = 537,               /* class_item  */
  YYSYMBOL_locale_clause = 538,            /* locale_clause  */
  YYSYMBOL_currency_sign_clause = 539,     /* currency_sign_clause  */
  YYSYMBOL_decimal_point_clause = 540,     /* decimal_point_clause  */
  YYSYMBOL_cursor_clause = 541,            /* cursor_clause  */
  YYSYMBOL_crt_status_clause = 542,        /* crt_status_clause  */
  YYSYMBOL_screen_control = 543,           /* screen_control  */
  YYSYMBOL_event_status = 544,             /* event_status  */
  YYSYMBOL_input_output_section = 545,     /* input_output_section  */
  YYSYMBOL_546_13 = 546,                   /* $@13  */
  YYSYMBOL_547_14 = 547,                   /* $@14  */
  YYSYMBOL_file_control_paragraph = 548,   /* file_control_paragraph  */
  YYSYMBOL_file_control_sequence = 549,    /* file_control_sequence  */
  YYSYMBOL_file_control_entry = 550,       /* file_control_entry  */
  YYSYMBOL_551_15 = 551,                   /* $@15  */
  YYSYMBOL_select_clause_sequence = 552,   /* select_clause_sequence  */
  YYSYMBOL_select_clause = 553,            /* select_clause  */
  YYSYMBOL_assign_clause = 554,            /* assign_clause  */
  YYSYMBOL__device = 555,                  /* _device  */
  YYSYMBOL__ext_clause = 556,              /* _ext_clause  */
  YYSYMBOL_assignment_name = 557,          /* assignment_name  */
  YYSYMBOL_assignment_device_name_list = 558, /* assignment_device_name_list  */
  YYSYMBOL_access_mode_clause = 559,       /* access_mode_clause  */
  YYSYMBOL_access_mode = 560,              /* access_mode  */
  YYSYMBOL_alternative_record_key_clause = 561, /* alternative_record_key_clause  */
  YYSYMBOL_split_key_list = 562,           /* split_key_list  */
  YYSYMBOL_563_16 = 563,                   /* $@16  */
  YYSYMBOL_split_key = 564,                /* split_key  */
  YYSYMBOL_key_is_eq = 565,                /* key_is_eq  */
  YYSYMBOL_collating_sequence_clause = 566, /* collating_sequence_clause  */
  YYSYMBOL_file_status_clause = 567,       /* file_status_clause  */
  YYSYMBOL_file_or_sort = 568,             /* file_or_sort  */
  YYSYMBOL_lock_mode_clause = 569,         /* lock_mode_clause  */
  YYSYMBOL_lock_mode = 570,                /* lock_mode  */
  YYSYMBOL_lock_with = 571,                /* lock_with  */
  YYSYMBOL_lock_records = 572,             /* lock_records  */
  YYSYMBOL_organization_clause = 573,      /* organization_clause  */
  YYSYMBOL_organization = 574,             /* organization  */
  YYSYMBOL_padding_character_clause = 575, /* padding_character_clause  */
  YYSYMBOL_record_delimiter_clause = 576,  /* record_delimiter_clause  */
  YYSYMBOL_record_key_clause = 577,        /* record_key_clause  */
  YYSYMBOL_relative_key_clause = 578,      /* relative_key_clause  */
  YYSYMBOL_reserve_clause = 579,           /* reserve_clause  */
  YYSYMBOL_sharing_clause = 580,           /* sharing_clause  */
  YYSYMBOL_sharing_option = 581,           /* sharing_option  */
  YYSYMBOL_nominal_key_clause = 582,       /* nominal_key_clause  */
  YYSYMBOL_i_o_control_paragraph = 583,    /* i_o_control_paragraph  */
  YYSYMBOL_opt_i_o_control = 584,          /* opt_i_o_control  */
  YYSYMBOL_i_o_control_list = 585,         /* i_o_control_list  */
  YYSYMBOL_i_o_control_clause = 586,       /* i_o_control_clause  */
  YYSYMBOL_same_clause = 587,              /* same_clause  */
  YYSYMBOL_same_option = 588,              /* same_option  */
  YYSYMBOL_multiple_file_tape_clause = 589, /* multiple_file_tape_clause  */
  YYSYMBOL_multiple_file_list = 590,       /* multiple_file_list  */
  YYSYMBOL_multiple_file = 591,            /* multiple_file  */
  YYSYMBOL_multiple_file_position = 592,   /* multiple_file_position  */
  YYSYMBOL_apply_clause_list = 593,        /* apply_clause_list  */
  YYSYMBOL_apply_clause = 594,             /* apply_clause  */
  YYSYMBOL_data_division = 595,            /* data_division  */
  YYSYMBOL_file_section = 596,             /* file_section  */
  YYSYMBOL_597_17 = 597,                   /* $@17  */
  YYSYMBOL_598_18 = 598,                   /* $@18  */
  YYSYMBOL_file_description_sequence = 599, /* file_description_sequence  */
  YYSYMBOL_file_description = 600,         /* file_description  */
  YYSYMBOL_file_description_sequence_without_type = 601, /* file_description_sequence_without_type  */
  YYSYMBOL_file_type = 602,                /* file_type  */
  YYSYMBOL_file_description_entry = 603,   /* file_description_entry  */
  YYSYMBOL_604_19 = 604,                   /* @19  */
  YYSYMBOL_file_description_clause_sequence = 605, /* file_description_clause_sequence  */
  YYSYMBOL_file_description_clause = 606,  /* file_description_clause  */
  YYSYMBOL_block_contains_clause = 607,    /* block_contains_clause  */
  YYSYMBOL__records_or_characters = 608,   /* _records_or_characters  */
  YYSYMBOL_record_clause = 609,            /* record_clause  */
  YYSYMBOL_record_depending = 610,         /* record_depending  */
  YYSYMBOL_opt_from_integer = 611,         /* opt_from_integer  */
  YYSYMBOL_opt_to_integer = 612,           /* opt_to_integer  */
  YYSYMBOL_label_records_clause = 613,     /* label_records_clause  */
  YYSYMBOL_label_option = 614,             /* label_option  */
  YYSYMBOL_value_of_clause = 615,          /* value_of_clause  */
  YYSYMBOL_valueof_name = 616,             /* valueof_name  */
  YYSYMBOL_data_records_clause = 617,      /* data_records_clause  */
  YYSYMBOL_linage_clause = 618,            /* linage_clause  */
  YYSYMBOL_linage_sequence = 619,          /* linage_sequence  */
  YYSYMBOL_linage_lines = 620,             /* linage_lines  */
  YYSYMBOL_linage_footing = 621,           /* linage_footing  */
  YYSYMBOL_linage_top = 622,               /* linage_top  */
  YYSYMBOL_linage_bottom = 623,            /* linage_bottom  */
  YYSYMBOL_recording_mode_clause = 624,    /* recording_mode_clause  */
  YYSYMBOL_code_set_clause = 625,          /* code_set_clause  */
  YYSYMBOL_report_clause = 626,            /* report_clause  */
  YYSYMBOL_working_storage_section = 627,  /* working_storage_section  */
  YYSYMBOL_628_20 = 628,                   /* $@20  */
  YYSYMBOL_record_description_list = 629,  /* record_description_list  */
  YYSYMBOL_record_description_list_1 = 630, /* record_description_list_1  */
  YYSYMBOL_631_21 = 631,                   /* $@21  */
  YYSYMBOL_record_description_list_2 = 632, /* record_description_list_2  */
  YYSYMBOL_exec_sql_data_statement = 633,  /* exec_sql_data_statement  */
  YYSYMBOL_data_description = 634,         /* data_description  */
  YYSYMBOL_635_22 = 635,                   /* $@22  */
  YYSYMBOL_636_23 = 636,                   /* $@23  */
  YYSYMBOL_level_number = 637,             /* level_number  */
  YYSYMBOL_level_number_88 = 638,          /* level_number_88  */
  YYSYMBOL__maybe_next_level_number = 639, /* _maybe_next_level_number  */
  YYSYMBOL_entry_name = 640,               /* entry_name  */
  YYSYMBOL_const_name = 641,               /* const_name  */
  YYSYMBOL_const_global = 642,             /* const_global  */
  YYSYMBOL_lit_or_length = 643,            /* lit_or_length  */
  YYSYMBOL_constant_entry = 644,           /* constant_entry  */
  YYSYMBOL_data_description_clause_sequence = 645, /* data_description_clause_sequence  */
  YYSYMBOL_data_description_clause = 646,  /* data_description_clause  */
  YYSYMBOL_redefines_clause = 647,         /* redefines_clause  */
  YYSYMBOL_external_clause = 648,          /* external_clause  */
  YYSYMBOL_as_extname = 649,               /* as_extname  */
  YYSYMBOL_global_clause = 650,            /* global_clause  */
  YYSYMBOL_picture_clause = 651,           /* picture_clause  */
  YYSYMBOL_usage_clause = 652,             /* usage_clause  */
  YYSYMBOL_usage = 653,                    /* usage  */
  YYSYMBOL_sign_clause = 654,              /* sign_clause  */
  YYSYMBOL_occurs_key_spec = 655,          /* occurs_key_spec  */
  YYSYMBOL_occurs_clause = 656,            /* occurs_clause  */
  YYSYMBOL_occurs_to_integer = 657,        /* occurs_to_integer  */
  YYSYMBOL_occurs_depending = 658,         /* occurs_depending  */
  YYSYMBOL__occurs_keys = 659,             /* _occurs_keys  */
  YYSYMBOL_occurs_keys = 660,              /* occurs_keys  */
  YYSYMBOL_occurs_key = 661,               /* occurs_key  */
  YYSYMBOL_occurs_key_list = 662,          /* occurs_key_list  */
  YYSYMBOL_ascending_or_descending = 663,  /* ascending_or_descending  */
  YYSYMBOL__occurs_indexed = 664,          /* _occurs_indexed  */
  YYSYMBOL_occurs_indexed = 665,           /* occurs_indexed  */
  YYSYMBOL_occurs_index_list = 666,        /* occurs_index_list  */
  YYSYMBOL_occurs_index = 667,             /* occurs_index  */
  YYSYMBOL_justified_clause = 668,         /* justified_clause  */
  YYSYMBOL_synchronized_clause = 669,      /* synchronized_clause  */
  YYSYMBOL_left_or_right = 670,            /* left_or_right  */
  YYSYMBOL_blank_clause = 671,             /* blank_clause  */
  YYSYMBOL_based_clause = 672,             /* based_clause  */
  YYSYMBOL_value_clause = 673,             /* value_clause  */
  YYSYMBOL_value_cond_clause = 674,        /* value_cond_clause  */
  YYSYMBOL_675_24 = 675,                   /* $@24  */
  YYSYMBOL_value_item_list = 676,          /* value_item_list  */
  YYSYMBOL_value_item = 677,               /* value_item  */
  YYSYMBOL_false_is = 678,                 /* false_is  */
  YYSYMBOL_renames_clause = 679,           /* renames_clause  */
  YYSYMBOL_any_length_clause = 680,        /* any_length_clause  */
  YYSYMBOL_local_storage_section = 681,    /* local_storage_section  */
  YYSYMBOL_682_25 = 682,                   /* $@25  */
  YYSYMBOL_linkage_section = 683,          /* linkage_section  */
  YYSYMBOL_684_26 = 684,                   /* $@26  */
  YYSYMBOL_report_section = 685,           /* report_section  */
  YYSYMBOL_686_27 = 686,                   /* $@27  */
  YYSYMBOL_opt_report_description_list = 687, /* opt_report_description_list  */
  YYSYMBOL_report_description_list = 688,  /* report_description_list  */
  YYSYMBOL_report_description_entry = 689, /* report_description_entry  */
  YYSYMBOL_report_description_options = 690, /* report_description_options  */
  YYSYMBOL_report_description_option = 691, /* report_description_option  */
  YYSYMBOL_control_clause = 692,           /* control_clause  */
  YYSYMBOL_control_field_list = 693,       /* control_field_list  */
  YYSYMBOL__final = 694,                   /* _final  */
  YYSYMBOL_identifier_list = 695,          /* identifier_list  */
  YYSYMBOL_page_limit_clause = 696,        /* page_limit_clause  */
  YYSYMBOL_heading_clause = 697,           /* heading_clause  */
  YYSYMBOL_first_detail = 698,             /* first_detail  */
  YYSYMBOL_last_heading = 699,             /* last_heading  */
  YYSYMBOL_last_detail = 700,              /* last_detail  */
  YYSYMBOL_footing_clause = 701,           /* footing_clause  */
  YYSYMBOL_page_line_column = 702,         /* page_line_column  */
  YYSYMBOL_line_or_lines = 703,            /* line_or_lines  */
  YYSYMBOL_report_group_description_list = 704, /* report_group_description_list  */
  YYSYMBOL_report_group_description_entry = 705, /* report_group_description_entry  */
  YYSYMBOL_report_group_options = 706,     /* report_group_options  */
  YYSYMBOL_report_group_option = 707,      /* report_group_option  */
  YYSYMBOL_type_clause = 708,              /* type_clause  */
  YYSYMBOL_type_option = 709,              /* type_option  */
  YYSYMBOL_next_group_clause = 710,        /* next_group_clause  */
  YYSYMBOL_column_clause = 711,            /* column_clause  */
  YYSYMBOL_sum_clause_list = 712,          /* sum_clause_list  */
  YYSYMBOL_sum_clause = 713,               /* sum_clause  */
  YYSYMBOL_ref_id_exp = 714,               /* ref_id_exp  */
  YYSYMBOL_present_when_condition = 715,   /* present_when_condition  */
  YYSYMBOL_varying_clause = 716,           /* varying_clause  */
  YYSYMBOL_line_clause = 717,              /* line_clause  */
  YYSYMBOL_line_keyword_clause = 718,      /* line_keyword_clause  */
  YYSYMBOL_report_line_integer_list = 719, /* report_line_integer_list  */
  YYSYMBOL_line_or_plus = 720,             /* line_or_plus  */
  YYSYMBOL__numbers = 721,                 /* _numbers  */
  YYSYMBOL_source_clause = 722,            /* source_clause  */
  YYSYMBOL_group_indicate_clause = 723,    /* group_indicate_clause  */
  YYSYMBOL__indicate = 724,                /* _indicate  */
  YYSYMBOL_report_name = 725,              /* report_name  */
  YYSYMBOL_screen_section = 726,           /* screen_section  */
  YYSYMBOL_727_28 = 727,                   /* $@28  */
  YYSYMBOL_procedure_division = 728,       /* procedure_division  */
  YYSYMBOL_729_29 = 729,                   /* $@29  */
  YYSYMBOL_730_30 = 730,                   /* $@30  */
  YYSYMBOL_procedure_using_chaining = 731, /* procedure_using_chaining  */
  YYSYMBOL_732_31 = 732,                   /* $@31  */
  YYSYMBOL_733_32 = 733,                   /* $@32  */
  YYSYMBOL_procedure_param_list = 734,     /* procedure_param_list  */
  YYSYMBOL_procedure_param = 735,          /* procedure_param  */
  YYSYMBOL_procedure_type = 736,           /* procedure_type  */
  YYSYMBOL_size_optional = 737,            /* size_optional  */
  YYSYMBOL_procedure_optional = 738,       /* procedure_optional  */
  YYSYMBOL_procedure_returning = 739,      /* procedure_returning  */
  YYSYMBOL_procedure_declaratives = 740,   /* procedure_declaratives  */
  YYSYMBOL_741_33 = 741,                   /* $@33  */
  YYSYMBOL_procedure_list = 742,           /* procedure_list  */
  YYSYMBOL_procedure = 743,                /* procedure  */
  YYSYMBOL_section_header = 744,           /* section_header  */
  YYSYMBOL_paragraph_header = 745,         /* paragraph_header  */
  YYSYMBOL_invalid_statement = 746,        /* invalid_statement  */
  YYSYMBOL_section_name = 747,             /* section_name  */
  YYSYMBOL_opt_segment = 748,              /* opt_segment  */
  YYSYMBOL_statement_list = 749,           /* statement_list  */
  YYSYMBOL_750_34 = 750,                   /* @34  */
  YYSYMBOL_751_35 = 751,                   /* @35  */
  YYSYMBOL_statements = 752,               /* statements  */
  YYSYMBOL_statement = 753,                /* statement  */
  YYSYMBOL_accept_statement = 754,         /* accept_statement  */
  YYSYMBOL_755_36 = 755,                   /* $@36  */
  YYSYMBOL_accept_body = 756,              /* accept_body  */
  YYSYMBOL_opt_at_line_column = 757,       /* opt_at_line_column  */
  YYSYMBOL_line_number = 758,              /* line_number  */
  YYSYMBOL_column_number = 759,            /* column_number  */
  YYSYMBOL_opt_accp_attr = 760,            /* opt_accp_attr  */
  YYSYMBOL_accp_attrs = 761,               /* accp_attrs  */
  YYSYMBOL_accp_attr = 762,                /* accp_attr  */
  YYSYMBOL_end_accept = 763,               /* end_accept  */
  YYSYMBOL_add_statement = 764,            /* add_statement  */
  YYSYMBOL_765_37 = 765,                   /* $@37  */
  YYSYMBOL_add_body = 766,                 /* add_body  */
  YYSYMBOL_add_to = 767,                   /* add_to  */
  YYSYMBOL_end_add = 768,                  /* end_add  */
  YYSYMBOL_allocate_statement = 769,       /* allocate_statement  */
  YYSYMBOL_770_38 = 770,                   /* $@38  */
  YYSYMBOL_allocate_body = 771,            /* allocate_body  */
  YYSYMBOL_allocate_returning = 772,       /* allocate_returning  */
  YYSYMBOL_alter_statement = 773,          /* alter_statement  */
  YYSYMBOL_alter_options = 774,            /* alter_options  */
  YYSYMBOL__proceed_to = 775,              /* _proceed_to  */
  YYSYMBOL_call_statement = 776,           /* call_statement  */
  YYSYMBOL_777_39 = 777,                   /* $@39  */
  YYSYMBOL_call_using = 778,               /* call_using  */
  YYSYMBOL_779_40 = 779,                   /* $@40  */
  YYSYMBOL_call_param_list = 780,          /* call_param_list  */
  YYSYMBOL_call_param = 781,               /* call_param  */
  YYSYMBOL_call_type = 782,                /* call_type  */
  YYSYMBOL_call_returning = 783,           /* call_returning  */
  YYSYMBOL_call_on_exception = 784,        /* call_on_exception  */
  YYSYMBOL_785_41 = 785,                   /* $@41  */
  YYSYMBOL_call_not_on_exception = 786,    /* call_not_on_exception  */
  YYSYMBOL_787_42 = 787,                   /* $@42  */
  YYSYMBOL_end_call = 788,                 /* end_call  */
  YYSYMBOL_cancel_statement = 789,         /* cancel_statement  */
  YYSYMBOL_790_43 = 790,                   /* $@43  */
  YYSYMBOL_cancel_list = 791,              /* cancel_list  */
  YYSYMBOL_close_statement = 792,          /* close_statement  */
  YYSYMBOL_793_44 = 793,                   /* $@44  */
  YYSYMBOL_close_list = 794,               /* close_list  */
  YYSYMBOL_close_option = 795,             /* close_option  */
  YYSYMBOL_reel_or_unit = 796,             /* reel_or_unit  */
  YYSYMBOL_compute_statement = 797,        /* compute_statement  */
  YYSYMBOL_798_45 = 798,                   /* $@45  */
  YYSYMBOL_compute_body = 799,             /* compute_body  */
  YYSYMBOL_end_compute = 800,              /* end_compute  */
  YYSYMBOL_comp_equal = 801,               /* comp_equal  */
  YYSYMBOL_commit_statement = 802,         /* commit_statement  */
  YYSYMBOL_continue_statement = 803,       /* continue_statement  */
  YYSYMBOL_delete_statement = 804,         /* delete_statement  */
  YYSYMBOL_805_46 = 805,                   /* $@46  */
  YYSYMBOL_end_delete = 806,               /* end_delete  */
  YYSYMBOL_delete_file_statement = 807,    /* delete_file_statement  */
  YYSYMBOL_808_47 = 808,                   /* $@47  */
  YYSYMBOL_display_statement = 809,        /* display_statement  */
  YYSYMBOL_810_48 = 810,                   /* $@48  */
  YYSYMBOL_display_body = 811,             /* display_body  */
  YYSYMBOL_with_clause = 812,              /* with_clause  */
  YYSYMBOL_disp_attrs = 813,               /* disp_attrs  */
  YYSYMBOL_disp_attr = 814,                /* disp_attr  */
  YYSYMBOL_end_display = 815,              /* end_display  */
  YYSYMBOL_divide_statement = 816,         /* divide_statement  */
  YYSYMBOL_817_49 = 817,                   /* $@49  */
  YYSYMBOL_divide_body = 818,              /* divide_body  */
  YYSYMBOL_end_divide = 819,               /* end_divide  */
  YYSYMBOL_entry_statement = 820,          /* entry_statement  */
  YYSYMBOL_821_50 = 821,                   /* $@50  */
  YYSYMBOL_evaluate_statement = 822,       /* evaluate_statement  */
  YYSYMBOL_823_51 = 823,                   /* $@51  */
  YYSYMBOL_evaluate_subject_list = 824,    /* evaluate_subject_list  */
  YYSYMBOL_evaluate_subject = 825,         /* evaluate_subject  */
  YYSYMBOL_evaluate_condition_list = 826,  /* evaluate_condition_list  */
  YYSYMBOL_evaluate_case_list = 827,       /* evaluate_case_list  */
  YYSYMBOL_evaluate_case = 828,            /* evaluate_case  */
  YYSYMBOL_829_52 = 829,                   /* $@52  */
  YYSYMBOL_evaluate_other = 830,           /* evaluate_other  */
  YYSYMBOL_831_53 = 831,                   /* $@53  */
  YYSYMBOL_evaluate_when_list = 832,       /* evaluate_when_list  */
  YYSYMBOL_evaluate_object_list = 833,     /* evaluate_object_list  */
  YYSYMBOL_evaluate_object = 834,          /* evaluate_object  */
  YYSYMBOL_opt_evaluate_thru_expr = 835,   /* opt_evaluate_thru_expr  */
  YYSYMBOL_end_evaluate = 836,             /* end_evaluate  */
  YYSYMBOL_exit_statement = 837,           /* exit_statement  */
  YYSYMBOL_838_54 = 838,                   /* $@54  */
  YYSYMBOL_exit_body = 839,                /* exit_body  */
  YYSYMBOL_free_statement = 840,           /* free_statement  */
  YYSYMBOL_841_55 = 841,                   /* $@55  */
  YYSYMBOL_generate_statement = 842,       /* generate_statement  */
  YYSYMBOL_843_56 = 843,                   /* $@56  */
  YYSYMBOL_goto_statement = 844,           /* goto_statement  */
  YYSYMBOL_845_57 = 845,                   /* $@57  */
  YYSYMBOL_goto_depending = 846,           /* goto_depending  */
  YYSYMBOL_goback_statement = 847,         /* goback_statement  */
  YYSYMBOL_848_58 = 848,                   /* $@58  */
  YYSYMBOL_if_statement = 849,             /* if_statement  */
  YYSYMBOL_850_59 = 850,                   /* $@59  */
  YYSYMBOL_851_60 = 851,                   /* $@60  */
  YYSYMBOL_if_else_sentence = 852,         /* if_else_sentence  */
  YYSYMBOL_853_61 = 853,                   /* $@61  */
  YYSYMBOL_end_if = 854,                   /* end_if  */
  YYSYMBOL_initialize_statement = 855,     /* initialize_statement  */
  YYSYMBOL_856_62 = 856,                   /* $@62  */
  YYSYMBOL_initialize_filler = 857,        /* initialize_filler  */
  YYSYMBOL_initialize_value = 858,         /* initialize_value  */
  YYSYMBOL_initialize_replacing = 859,     /* initialize_replacing  */
  YYSYMBOL_initialize_replacing_list = 860, /* initialize_replacing_list  */
  YYSYMBOL_initialize_replacing_item = 861, /* initialize_replacing_item  */
  YYSYMBOL_initialize_category = 862,      /* initialize_category  */
  YYSYMBOL_initialize_default = 863,       /* initialize_default  */
  YYSYMBOL_initiate_statement = 864,       /* initiate_statement  */
  YYSYMBOL_865_63 = 865,                   /* $@63  */
  YYSYMBOL_inspect_statement = 866,        /* inspect_statement  */
  YYSYMBOL_867_64 = 867,                   /* $@64  */
  YYSYMBOL_send_identifier = 868,          /* send_identifier  */
  YYSYMBOL_inspect_list = 869,             /* inspect_list  */
  YYSYMBOL_inspect_item = 870,             /* inspect_item  */
  YYSYMBOL_inspect_tallying = 871,         /* inspect_tallying  */
  YYSYMBOL_872_65 = 872,                   /* $@65  */
  YYSYMBOL_tallying_list = 873,            /* tallying_list  */
  YYSYMBOL_tallying_item = 874,            /* tallying_item  */
  YYSYMBOL_inspect_replacing = 875,        /* inspect_replacing  */
  YYSYMBOL_replacing_list = 876,           /* replacing_list  */
  YYSYMBOL_replacing_item = 877,           /* replacing_item  */
  YYSYMBOL_rep_keyword = 878,              /* rep_keyword  */
  YYSYMBOL_replacing_region = 879,         /* replacing_region  */
  YYSYMBOL_inspect_converting = 880,       /* inspect_converting  */
  YYSYMBOL_inspect_region = 881,           /* inspect_region  */
  YYSYMBOL__initial = 882,                 /* _initial  */
  YYSYMBOL_merge_statement = 883,          /* merge_statement  */
  YYSYMBOL_884_66 = 884,                   /* $@66  */
  YYSYMBOL_move_statement = 885,           /* move_statement  */
  YYSYMBOL_886_67 = 886,                   /* $@67  */
  YYSYMBOL_move_body = 887,                /* move_body  */
  YYSYMBOL_multiply_statement = 888,       /* multiply_statement  */
  YYSYMBOL_889_68 = 889,                   /* $@68  */
  YYSYMBOL_multiply_body = 890,            /* multiply_body  */
  YYSYMBOL_end_multiply = 891,             /* end_multiply  */
  YYSYMBOL_open_statement = 892,           /* open_statement  */
  YYSYMBOL_893_69 = 893,                   /* $@69  */
  YYSYMBOL_open_list = 894,                /* open_list  */
  YYSYMBOL_open_mode = 895,                /* open_mode  */
  YYSYMBOL_open_sharing = 896,             /* open_sharing  */
  YYSYMBOL_open_option = 897,              /* open_option  */
  YYSYMBOL_perform_statement = 898,        /* perform_statement  */
  YYSYMBOL_899_70 = 899,                   /* $@70  */
  YYSYMBOL_perform_body = 900,             /* perform_body  */
  YYSYMBOL_901_71 = 901,                   /* $@71  */
  YYSYMBOL_end_perform = 902,              /* end_perform  */
  YYSYMBOL_perform_procedure = 903,        /* perform_procedure  */
  YYSYMBOL_perform_option = 904,           /* perform_option  */
  YYSYMBOL_perform_test = 905,             /* perform_test  */
  YYSYMBOL_perform_varying_list = 906,     /* perform_varying_list  */
  YYSYMBOL_perform_varying = 907,          /* perform_varying  */
  YYSYMBOL_read_statement = 908,           /* read_statement  */
  YYSYMBOL_909_72 = 909,                   /* $@72  */
  YYSYMBOL_read_into = 910,                /* read_into  */
  YYSYMBOL_with_lock = 911,                /* with_lock  */
  YYSYMBOL_read_key = 912,                 /* read_key  */
  YYSYMBOL_read_handler = 913,             /* read_handler  */
  YYSYMBOL_end_read = 914,                 /* end_read  */
  YYSYMBOL_release_statement = 915,        /* release_statement  */
  YYSYMBOL_916_73 = 916,                   /* $@73  */
  YYSYMBOL_return_statement = 917,         /* return_statement  */
  YYSYMBOL_918_74 = 918,                   /* $@74  */
  YYSYMBOL_end_return = 919,               /* end_return  */
  YYSYMBOL_rewrite_statement = 920,        /* rewrite_statement  */
  YYSYMBOL_921_75 = 921,                   /* $@75  */
  YYSYMBOL_write_lock = 922,               /* write_lock  */
  YYSYMBOL_end_rewrite = 923,              /* end_rewrite  */
  YYSYMBOL_rollback_statement = 924,       /* rollback_statement  */
  YYSYMBOL_search_statement = 925,         /* search_statement  */
  YYSYMBOL_926_76 = 926,                   /* $@76  */
  YYSYMBOL_search_body = 927,              /* search_body  */
  YYSYMBOL_928_77 = 928,                   /* $@77  */
  YYSYMBOL_search_varying = 929,           /* search_varying  */
  YYSYMBOL_search_at_end = 930,            /* search_at_end  */
  YYSYMBOL_931_78 = 931,                   /* $@78  */
  YYSYMBOL_search_whens = 932,             /* search_whens  */
  YYSYMBOL_search_when = 933,              /* search_when  */
  YYSYMBOL_934_79 = 934,                   /* $@79  */
  YYSYMBOL_end_search = 935,               /* end_search  */
  YYSYMBOL_set_statement = 936,            /* set_statement  */
  YYSYMBOL_937_80 = 937,                   /* $@80  */
  YYSYMBOL_set_body = 938,                 /* set_body  */
  YYSYMBOL_set_environment = 939,          /* set_environment  */
  YYSYMBOL_set_to = 940,                   /* set_to  */
  YYSYMBOL_set_up_down = 941,              /* set_up_down  */
  YYSYMBOL_up_or_down = 942,               /* up_or_down  */
  YYSYMBOL_set_to_on_off_sequence = 943,   /* set_to_on_off_sequence  */
  YYSYMBOL_set_to_on_off = 944,            /* set_to_on_off  */
  YYSYMBOL_set_to_true_false_sequence = 945, /* set_to_true_false_sequence  */
  YYSYMBOL_set_to_true_false = 946,        /* set_to_true_false  */
  YYSYMBOL_sort_statement = 947,           /* sort_statement  */
  YYSYMBOL_948_81 = 948,                   /* $@81  */
  YYSYMBOL_sort_body = 949,                /* sort_body  */
  YYSYMBOL_950_82 = 950,                   /* $@82  */
  YYSYMBOL_sort_key_list = 951,            /* sort_key_list  */
  YYSYMBOL_opt_key_list = 952,             /* opt_key_list  */
  YYSYMBOL_sort_duplicates = 953,          /* sort_duplicates  */
  YYSYMBOL_sort_collating = 954,           /* sort_collating  */
  YYSYMBOL_sort_input = 955,               /* sort_input  */
  YYSYMBOL_sort_output = 956,              /* sort_output  */
  YYSYMBOL_start_statement = 957,          /* start_statement  */
  YYSYMBOL_958_83 = 958,                   /* $@83  */
  YYSYMBOL_959_84 = 959,                   /* @84  */
  YYSYMBOL_start_key = 960,                /* start_key  */
  YYSYMBOL_start_op = 961,                 /* start_op  */
  YYSYMBOL_end_start = 962,                /* end_start  */
  YYSYMBOL_stop_statement = 963,           /* stop_statement  */
  YYSYMBOL_964_85 = 964,                   /* $@85  */
  YYSYMBOL_965_86 = 965,                   /* $@86  */
  YYSYMBOL_stop_returning = 966,           /* stop_returning  */
  YYSYMBOL_string_statement = 967,         /* string_statement  */
  YYSYMBOL_968_87 = 968,                   /* $@87  */
  YYSYMBOL_string_item_list = 969,         /* string_item_list  */
  YYSYMBOL_string_item = 970,              /* string_item  */
  YYSYMBOL_opt_with_pointer = 971,         /* opt_with_pointer  */
  YYSYMBOL_end_string = 972,               /* end_string  */
  YYSYMBOL_subtract_statement = 973,       /* subtract_statement  */
  YYSYMBOL_974_88 = 974,                   /* $@88  */
  YYSYMBOL_subtract_body = 975,            /* subtract_body  */
  YYSYMBOL_end_subtract = 976,             /* end_subtract  */
  YYSYMBOL_suppress_statement = 977,       /* suppress_statement  */
  YYSYMBOL__printing = 978,                /* _printing  */
  YYSYMBOL_terminate_statement = 979,      /* terminate_statement  */
  YYSYMBOL_980_89 = 980,                   /* $@89  */
  YYSYMBOL_transform_statement = 981,      /* transform_statement  */
  YYSYMBOL_982_90 = 982,                   /* $@90  */
  YYSYMBOL_unlock_statement = 983,         /* unlock_statement  */
  YYSYMBOL_984_91 = 984,                   /* $@91  */
  YYSYMBOL_opt_record = 985,               /* opt_record  */
  YYSYMBOL_unstring_statement = 986,       /* unstring_statement  */
  YYSYMBOL_987_92 = 987,                   /* $@92  */
  YYSYMBOL_unstring_delimited = 988,       /* unstring_delimited  */
  YYSYMBOL_unstring_delimited_list = 989,  /* unstring_delimited_list  */
  YYSYMBOL_unstring_delimited_item = 990,  /* unstring_delimited_item  */
  YYSYMBOL_unstring_into = 991,            /* unstring_into  */
  YYSYMBOL_unstring_into_item = 992,       /* unstring_into_item  */
  YYSYMBOL_unstring_into_delimiter = 993,  /* unstring_into_delimiter  */
  YYSYMBOL_unstring_into_count = 994,      /* unstring_into_count  */
  YYSYMBOL_unstring_tallying = 995,        /* unstring_tallying  */
  YYSYMBOL_end_unstring = 996,             /* end_unstring  */
  YYSYMBOL_use_statement = 997,            /* use_statement  */
  YYSYMBOL_use_exception = 998,            /* use_exception  */
  YYSYMBOL_use_global = 999,               /* use_global  */
  YYSYMBOL_use_exception_target = 1000,    /* use_exception_target  */
  YYSYMBOL__after = 1001,                  /* _after  */
  YYSYMBOL__standard = 1002,               /* _standard  */
  YYSYMBOL_exception_or_error = 1003,      /* exception_or_error  */
  YYSYMBOL_exception_or_overflow = 1004,   /* exception_or_overflow  */
  YYSYMBOL_not_exception_or_overflow = 1005, /* not_exception_or_overflow  */
  YYSYMBOL__procedure = 1006,              /* _procedure  */
  YYSYMBOL_use_debugging = 1007,           /* use_debugging  */
  YYSYMBOL_use_debugging_target = 1008,    /* use_debugging_target  */
  YYSYMBOL_use_reporting = 1009,           /* use_reporting  */
  YYSYMBOL_write_statement = 1010,         /* write_statement  */
  YYSYMBOL_1011_93 = 1011,                 /* $@93  */
  YYSYMBOL_write_from = 1012,              /* write_from  */
  YYSYMBOL_write_option = 1013,            /* write_option  */
  YYSYMBOL_before_or_after = 1014,         /* before_or_after  */
  YYSYMBOL_write_handler = 1015,           /* write_handler  */
  YYSYMBOL_end_write = 1016,               /* end_write  */
  YYSYMBOL_on_accp_exception = 1017,       /* on_accp_exception  */
  YYSYMBOL_on_disp_exception = 1018,       /* on_disp_exception  */
  YYSYMBOL_opt_on_exception = 1019,        /* opt_on_exception  */
  YYSYMBOL_1020_94 = 1020,                 /* $@94  */
  YYSYMBOL_opt_not_on_exception = 1021,    /* opt_not_on_exception  */
  YYSYMBOL_1022_95 = 1022,                 /* $@95  */
  YYSYMBOL_on_size_error = 1023,           /* on_size_error  */
  YYSYMBOL_opt_on_size_error = 1024,       /* opt_on_size_error  */
  YYSYMBOL_1025_96 = 1025,                 /* $@96  */
  YYSYMBOL_opt_not_on_size_error = 1026,   /* opt_not_on_size_error  */
  YYSYMBOL_1027_97 = 1027,                 /* $@97  */
  YYSYMBOL_on_overflow = 1028,             /* on_overflow  */
  YYSYMBOL_opt_on_overflow = 1029,         /* opt_on_overflow  */
  YYSYMBOL_1030_98 = 1030,                 /* $@98  */
  YYSYMBOL_opt_not_on_overflow = 1031,     /* opt_not_on_overflow  */
  YYSYMBOL_1032_99 = 1032,                 /* $@99  */
  YYSYMBOL_at_end = 1033,                  /* at_end  */
  YYSYMBOL_at_end_sentence = 1034,         /* at_end_sentence  */
  YYSYMBOL_1035_100 = 1035,                /* $@100  */
  YYSYMBOL_not_at_end_sentence = 1036,     /* not_at_end_sentence  */
  YYSYMBOL_1037_101 = 1037,                /* $@101  */
  YYSYMBOL_at_eop = 1038,                  /* at_eop  */
  YYSYMBOL_at_eop_sentence = 1039,         /* at_eop_sentence  */
  YYSYMBOL_1040_102 = 1040,                /* $@102  */
  YYSYMBOL_not_at_eop_sentence = 1041,     /* not_at_eop_sentence  */
  YYSYMBOL_1042_103 = 1042,                /* $@103  */
  YYSYMBOL_opt_invalid_key = 1043,         /* opt_invalid_key  */
  YYSYMBOL_invalid_key = 1044,             /* invalid_key  */
  YYSYMBOL_invalid_key_sentence = 1045,    /* invalid_key_sentence  */
  YYSYMBOL_1046_104 = 1046,                /* $@104  */
  YYSYMBOL_not_invalid_key_sentence = 1047, /* not_invalid_key_sentence  */
  YYSYMBOL_1048_105 = 1048,                /* $@105  */
  YYSYMBOL__opt_scroll_lines = 1049,       /* _opt_scroll_lines  */
  YYSYMBOL_condition = 1050,               /* condition  */
  YYSYMBOL_expr = 1051,                    /* expr  */
  YYSYMBOL_partial_expr = 1052,            /* partial_expr  */
  YYSYMBOL_1053_106 = 1053,                /* $@106  */
  YYSYMBOL_expr_tokens = 1054,             /* expr_tokens  */
  YYSYMBOL_expr_token = 1055,              /* expr_token  */
  YYSYMBOL_eq = 1056,                      /* eq  */
  YYSYMBOL_gt = 1057,                      /* gt  */
  YYSYMBOL_lt = 1058,                      /* lt  */
  YYSYMBOL_ge = 1059,                      /* ge  */
  YYSYMBOL_le = 1060,                      /* le  */
  YYSYMBOL_exp_list = 1061,                /* exp_list  */
  YYSYMBOL_e_sep = 1062,                   /* e_sep  */
  YYSYMBOL_exp = 1063,                     /* exp  */
  YYSYMBOL_linage_counter = 1064,          /* linage_counter  */
  YYSYMBOL_arithmetic_x_list = 1065,       /* arithmetic_x_list  */
  YYSYMBOL_arithmetic_x = 1066,            /* arithmetic_x  */
  YYSYMBOL_record_name = 1067,             /* record_name  */
  YYSYMBOL_table_name = 1068,              /* table_name  */
  YYSYMBOL_file_name_list = 1069,          /* file_name_list  */
  YYSYMBOL_file_name = 1070,               /* file_name  */
  YYSYMBOL_mnemonic_name_list = 1071,      /* mnemonic_name_list  */
  YYSYMBOL_mnemonic_name = 1072,           /* mnemonic_name  */
  YYSYMBOL_procedure_name_list = 1073,     /* procedure_name_list  */
  YYSYMBOL_procedure_name = 1074,          /* procedure_name  */
  YYSYMBOL_label = 1075,                   /* label  */
  YYSYMBOL_integer_label = 1076,           /* integer_label  */
  YYSYMBOL_reference_list = 1077,          /* reference_list  */
  YYSYMBOL_reference = 1078,               /* reference  */
  YYSYMBOL_no_reference_list = 1079,       /* no_reference_list  */
  YYSYMBOL_opt_reference = 1080,           /* opt_reference  */
  YYSYMBOL_reference_or_literal = 1081,    /* reference_or_literal  */
  YYSYMBOL_undefined_word = 1082,          /* undefined_word  */
  YYSYMBOL_target_x_list = 1083,           /* target_x_list  */
  YYSYMBOL_target_x = 1084,                /* target_x  */
  YYSYMBOL_x_list = 1085,                  /* x_list  */
  YYSYMBOL_x = 1086,                       /* x  */
  YYSYMBOL_arith_x = 1087,                 /* arith_x  */
  YYSYMBOL_prog_or_entry = 1088,           /* prog_or_entry  */
  YYSYMBOL_alnum_or_id = 1089,             /* alnum_or_id  */
  YYSYMBOL_simple_value = 1090,            /* simple_value  */
  YYSYMBOL_simple_all_value = 1091,        /* simple_all_value  */
  YYSYMBOL_id_or_lit = 1092,               /* id_or_lit  */
  YYSYMBOL_id_or_lit_or_func = 1093,       /* id_or_lit_or_func  */
  YYSYMBOL_num_id_or_lit = 1094,           /* num_id_or_lit  */
  YYSYMBOL_identifier = 1095,              /* identifier  */
  YYSYMBOL_identifier_1 = 1096,            /* identifier_1  */
  YYSYMBOL_qualified_word = 1097,          /* qualified_word  */
  YYSYMBOL_subref = 1098,                  /* subref  */
  YYSYMBOL_refmod = 1099,                  /* refmod  */
  YYSYMBOL_integer = 1100,                 /* integer  */
  YYSYMBOL_literal = 1101,                 /* literal  */
  YYSYMBOL_basic_literal = 1102,           /* basic_literal  */
  YYSYMBOL_basic_value = 1103,             /* basic_value  */
  YYSYMBOL_function = 1104,                /* function  */
  YYSYMBOL_func_refmod = 1105,             /* func_refmod  */
  YYSYMBOL_func_args = 1106,               /* func_args  */
  YYSYMBOL_list_func_args = 1107,          /* list_func_args  */
  YYSYMBOL_trim_args = 1108,               /* trim_args  */
  YYSYMBOL_numvalc_args = 1109,            /* numvalc_args  */
  YYSYMBOL_locale_dt_args = 1110,          /* locale_dt_args  */
  YYSYMBOL_not_const_word = 1111,          /* not_const_word  */
  YYSYMBOL_flag_all = 1112,                /* flag_all  */
  YYSYMBOL_flag_duplicates = 1113,         /* flag_duplicates  */
  YYSYMBOL_flag_initialized = 1114,        /* flag_initialized  */
  YYSYMBOL_flag_next = 1115,               /* flag_next  */
  YYSYMBOL_flag_not = 1116,                /* flag_not  */
  YYSYMBOL_flag_optional = 1117,           /* flag_optional  */
  YYSYMBOL_flag_rounded = 1118,            /* flag_rounded  */
  YYSYMBOL_flag_separate = 1119,           /* flag_separate  */
  YYSYMBOL_in_of = 1120,                   /* in_of  */
  YYSYMBOL_records = 1121,                 /* records  */
  YYSYMBOL_with_dups = 1122,               /* with_dups  */
  YYSYMBOL_coll_sequence = 1123,           /* coll_sequence  */
  YYSYMBOL__advancing = 1124,              /* _advancing  */
  YYSYMBOL__also = 1125,                   /* _also  */
  YYSYMBOL__are = 1126,                    /* _are  */
  YYSYMBOL__area = 1127,                   /* _area  */
  YYSYMBOL__as = 1128,                     /* _as  */
  YYSYMBOL__at = 1129,                     /* _at  */
  YYSYMBOL__binary = 1130,                 /* _binary  */
  YYSYMBOL__by = 1131,                     /* _by  */
  YYSYMBOL__character = 1132,              /* _character  */
  YYSYMBOL__characters = 1133,             /* _characters  */
  YYSYMBOL__contains = 1134,               /* _contains  */
  YYSYMBOL__data = 1135,                   /* _data  */
  YYSYMBOL__file = 1136,                   /* _file  */
  YYSYMBOL__for = 1137,                    /* _for  */
  YYSYMBOL__from = 1138,                   /* _from  */
  YYSYMBOL__in = 1139,                     /* _in  */
  YYSYMBOL__is = 1140,                     /* _is  */
  YYSYMBOL__is_are = 1141,                 /* _is_are  */
  YYSYMBOL__key = 1142,                    /* _key  */
  YYSYMBOL__line_or_lines = 1143,          /* _line_or_lines  */
  YYSYMBOL__lines = 1144,                  /* _lines  */
  YYSYMBOL__literal = 1145,                /* _literal  */
  YYSYMBOL__mode = 1146,                   /* _mode  */
  YYSYMBOL__number = 1147,                 /* _number  */
  YYSYMBOL__of = 1148,                     /* _of  */
  YYSYMBOL__on = 1149,                     /* _on  */
  YYSYMBOL__in_order = 1150,               /* _in_order  */
  YYSYMBOL__other = 1151,                  /* _other  */
  YYSYMBOL__program = 1152,                /* _program  */
  YYSYMBOL__record = 1153,                 /* _record  */
  YYSYMBOL__right = 1154,                  /* _right  */
  YYSYMBOL__set = 1155,                    /* _set  */
  YYSYMBOL__sign = 1156,                   /* _sign  */
  YYSYMBOL__sign_is = 1157,                /* _sign_is  */
  YYSYMBOL__size = 1158,                   /* _size  */
  YYSYMBOL__status = 1159,                 /* _status  */
  YYSYMBOL__tape = 1160,                   /* _tape  */
  YYSYMBOL__than = 1161,                   /* _than  */
  YYSYMBOL__then = 1162,                   /* _then  */
  YYSYMBOL__times = 1163,                  /* _times  */
  YYSYMBOL__to = 1164,                     /* _to  */
  YYSYMBOL__when = 1165,                   /* _when  */
  YYSYMBOL__with = 1166,                   /* _with  */
  YYSYMBOL_exec_sql_statement = 1167       /* exec_sql_statement  */
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
#define YYLAST   5297

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  464
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  704
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1551
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  2289

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   705


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
       2,     2,     2,     2,     2,     2,     2,     2,   463,     2,
     459,   458,   452,   450,     2,   451,   456,   453,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   462,     2,
     461,   457,   460,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   455,     2,     2,     2,     2,     2,
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
     445,   446,   447,   448,   449,   454
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   774,   774,   774,   818,   819,   823,   824,   829,   830,
     828,   843,   844,   842,   857,   858,   856,   863,   864,   865,
     868,   869,   897,   923,   955,   954,   998,  1042,  1043,  1047,
    1048,  1051,  1052,  1056,  1063,  1070,  1074,  1078,  1090,  1091,
    1101,  1102,  1111,  1112,  1116,  1117,  1118,  1119,  1128,  1131,
    1132,  1133,  1134,  1138,  1145,  1154,  1157,  1158,  1159,  1160,
    1164,  1165,  1169,  1170,  1171,  1175,  1182,  1183,  1187,  1194,
    1206,  1209,  1210,  1214,  1215,  1219,  1223,  1230,  1231,  1241,
    1244,  1245,  1246,  1250,  1251,  1255,  1256,  1257,  1258,  1259,
    1260,  1261,  1262,  1263,  1264,  1265,  1272,  1283,  1282,  1294,
    1293,  1302,  1316,  1330,  1344,  1360,  1361,  1365,  1366,  1370,
    1381,  1382,  1390,  1389,  1401,  1402,  1403,  1404,  1405,  1413,
    1414,  1419,  1420,  1422,  1421,  1433,  1434,  1438,  1439,  1440,
    1441,  1442,  1443,  1447,  1448,  1449,  1450,  1451,  1452,  1459,
    1470,  1482,  1483,  1487,  1488,  1495,  1504,  1505,  1509,  1510,
    1524,  1539,  1606,  1617,  1624,  1631,  1637,  1644,  1645,  1649,
    1648,  1658,  1657,  1673,  1674,  1677,  1678,  1683,  1682,  1703,
    1704,  1708,  1709,  1710,  1711,  1712,  1713,  1714,  1715,  1716,
    1717,  1718,  1719,  1720,  1721,  1722,  1729,  1733,  1738,  1745,
    1746,  1747,  1750,  1751,  1755,  1762,  1763,  1770,  1790,  1791,
    1797,  1801,  1802,  1803,  1810,  1830,  1873,  1873,  1877,  1881,
    1897,  1898,  1899,  1905,  1915,  1924,  1926,  1927,  1933,  1937,
    1938,  1939,  1942,  1943,  1944,  1948,  1952,  1953,  1959,  1960,
    1964,  1973,  1982,  1991,  2000,  2015,  2025,  2032,  2041,  2079,
    2086,  2087,  2094,  2098,  2099,  2100,  2106,  2113,  2114,  2117,
    2118,  2119,  2120,  2124,  2125,  2129,  2130,  2136,  2161,  2162,
    2163,  2164,  2170,  2177,  2178,  2182,  2185,  2186,  2192,  2193,
    2194,  2198,  2202,  2206,  2210,  2214,  2224,  2225,  2239,  2240,
    2240,  2243,  2242,  2255,  2256,  2260,  2272,  2281,  2285,  2286,
    2296,  2295,  2313,  2314,  2318,  2325,  2332,  2333,  2334,  2335,
    2336,  2337,  2338,  2339,  2340,  2341,  2348,  2352,  2352,  2352,
    2358,  2370,  2395,  2419,  2420,  2427,  2428,  2432,  2433,  2440,
    2447,  2448,  2455,  2459,  2468,  2469,  2475,  2485,  2503,  2504,
    2508,  2509,  2510,  2514,  2521,  2528,  2538,  2545,  2563,  2567,
    2578,  2579,  2579,  2590,  2591,  2595,  2595,  2612,  2613,  2615,
    2616,  2617,  2621,  2633,  2635,  2634,  2660,  2659,  2685,  2689,
    2696,  2698,  2720,  2725,  2731,  2740,  2748,  2749,  2757,  2758,
    2759,  2763,  2783,  2787,  2796,  2797,  2798,  2799,  2800,  2801,
    2802,  2803,  2804,  2805,  2806,  2807,  2808,  2809,  2810,  2817,
    2839,  2861,  2862,  2874,  2894,  2901,  2902,  2906,  2907,  2908,
    2913,  2918,  2919,  2920,  2921,  2922,  2923,  2924,  2925,  2930,
    2935,  2936,  2937,  2938,  2939,  2940,  2941,  2942,  2943,  2944,
    2945,  2946,  2947,  2948,  2949,  2950,  2951,  2952,  2953,  2961,
    2969,  2977,  2984,  2989,  2999,  3000,  3001,  3005,  3022,  3023,
    3026,  3027,  3033,  3033,  3036,  3060,  3076,  3077,  3081,  3082,
    3085,  3085,  3088,  3095,  3096,  3101,  3111,  3118,  3121,  3122,
    3123,  3130,  3137,  3162,  3166,  3166,  3171,  3172,  3176,  3177,
    3180,  3181,  3194,  3206,  3226,  3240,  3242,  3241,  3262,  3263,
    3263,  3276,  3278,  3277,  3289,  3290,  3294,  3295,  3304,  3311,
    3314,  3318,  3322,  3323,  3324,  3331,  3332,  3336,  3339,  3339,
    3342,  3343,  3349,  3354,  3355,  3358,  3359,  3362,  3363,  3366,
    3367,  3370,  3371,  3375,  3376,  3377,  3381,  3382,  3385,  3386,
    3390,  3394,  3395,  3399,  3400,  3401,  3402,  3403,  3404,  3405,
    3406,  3407,  3408,  3409,  3410,  3411,  3412,  3413,  3414,  3418,
    3422,  3423,  3424,  3425,  3426,  3427,  3428,  3432,  3436,  3437,
    3438,  3442,  3443,  3447,  3451,  3456,  3460,  3464,  3468,  3469,
    3473,  3474,  3478,  3479,  3480,  3483,  3483,  3483,  3486,  3490,
    3493,  3493,  3496,  3503,  3504,  3504,  3514,  3516,  3526,  3515,
    3553,  3555,  3554,  3561,  3560,  3569,  3570,  3575,  3582,  3584,
    3588,  3598,  3600,  3608,  3616,  3645,  3676,  3678,  3688,  3693,
    3704,  3705,  3705,  3732,  3733,  3737,  3738,  3739,  3740,  3756,
    3768,  3799,  3836,  3848,  3851,  3852,  3861,  3865,  3861,  3878,
    3899,  3903,  3904,  3905,  3906,  3907,  3908,  3909,  3910,  3911,
    3912,  3913,  3914,  3915,  3916,  3917,  3918,  3919,  3920,  3921,
    3922,  3923,  3924,  3925,  3926,  3927,  3928,  3929,  3930,  3931,
    3932,  3933,  3934,  3935,  3936,  3937,  3938,  3939,  3940,  3941,
    3942,  3943,  3944,  3945,  3946,  3947,  3948,  3949,  3950,  3951,
    3952,  3953,  3976,  3975,  3988,  3992,  3996,  4000,  4004,  4008,
    4012,  4016,  4020,  4024,  4028,  4032,  4036,  4040,  4044,  4048,
    4052,  4059,  4060,  4061,  4062,  4063,  4064,  4068,  4072,  4073,
    4076,  4077,  4081,  4082,  4086,  4087,  4088,  4089,  4090,  4091,
    4092,  4093,  4097,  4101,  4105,  4110,  4111,  4112,  4113,  4114,
    4115,  4119,  4120,  4129,  4129,  4135,  4139,  4143,  4149,  4150,
    4154,  4155,  4164,  4164,  4169,  4173,  4180,  4181,  4190,  4196,
    4197,  4201,  4201,  4209,  4209,  4219,  4221,  4220,  4229,  4230,
    4235,  4242,  4249,  4251,  4255,  4263,  4274,  4275,  4276,  4281,
    4285,  4284,  4296,  4300,  4299,  4310,  4311,  4320,  4320,  4324,
    4325,  4329,  4341,  4341,  4345,  4346,  4357,  4358,  4359,  4360,
    4361,  4364,  4364,  4372,  4372,  4378,  4385,  4386,  4389,  4389,
    4396,  4409,  4422,  4422,  4433,  4434,  4443,  4443,  4463,  4462,
    4475,  4479,  4483,  4487,  4491,  4495,  4499,  4504,  4508,  4515,
    4516,  4517,  4521,  4522,  4527,  4528,  4529,  4530,  4531,  4532,
    4533,  4534,  4535,  4536,  4540,  4544,  4548,  4553,  4554,  4558,
    4559,  4568,  4568,  4574,  4578,  4582,  4586,  4590,  4597,  4598,
    4607,  4607,  4629,  4628,  4647,  4648,  4659,  4668,  4673,  4681,
    4710,  4711,  4717,  4716,  4732,  4736,  4735,  4750,  4751,  4756,
    4757,  4768,  4797,  4798,  4799,  4802,  4803,  4807,  4808,  4817,
    4817,  4822,  4823,  4831,  4839,  4847,  4865,  4890,  4890,  4903,
    4903,  4916,  4916,  4925,  4929,  4942,  4942,  4955,  4957,  4955,
    4968,  4973,  4977,  4976,  4990,  4991,  5000,  5000,  5008,  5009,
    5013,  5014,  5015,  5019,  5020,  5025,  5026,  5031,  5035,  5036,
    5037,  5038,  5039,  5040,  5041,  5045,  5046,  5055,  5055,  5068,
    5067,  5077,  5078,  5079,  5083,  5084,  5088,  5089,  5090,  5096,
    5096,  5101,  5102,  5106,  5107,  5108,  5109,  5110,  5111,  5117,
    5121,  5122,  5126,  5131,  5135,  5136,  5137,  5138,  5139,  5143,
    5169,  5182,  5183,  5187,  5187,  5195,  5195,  5205,  5205,  5210,
    5214,  5226,  5226,  5232,  5236,  5243,  5244,  5253,  5253,  5257,
    5258,  5272,  5273,  5274,  5275,  5279,  5280,  5284,  5285,  5286,
    5298,  5298,  5303,  5308,  5307,  5317,  5324,  5325,  5329,  5334,
    5343,  5346,  5350,  5355,  5362,  5369,  5370,  5374,  5375,  5380,
    5392,  5392,  5419,  5420,  5424,  5425,  5429,  5433,  5437,  5441,
    5448,  5449,  5455,  5456,  5457,  5461,  5462,  5471,  5471,  5486,
    5486,  5497,  5498,  5507,  5507,  5524,  5525,  5529,  5536,  5537,
    5546,  5559,  5559,  5565,  5570,  5569,  5580,  5581,  5585,  5587,
    5586,  5597,  5598,  5603,  5602,  5613,  5614,  5623,  5623,  5628,
    5629,  5630,  5631,  5632,  5638,  5647,  5651,  5660,  5667,  5668,
    5674,  5675,  5679,  5688,  5689,  5693,  5697,  5709,  5709,  5715,
    5714,  5731,  5734,  5755,  5756,  5759,  5760,  5764,  5765,  5770,
    5775,  5783,  5795,  5800,  5808,  5824,  5825,  5824,  5845,  5846,
    5854,  5855,  5856,  5857,  5858,  5862,  5863,  5872,  5872,  5877,
    5877,  5884,  5885,  5886,  5895,  5895,  5904,  5905,  5909,  5910,
    5911,  5915,  5916,  5920,  5921,  5930,  5930,  5936,  5940,  5944,
    5951,  5952,  5961,  5968,  5969,  5977,  5977,  5990,  5990,  6006,
    6006,  6015,  6017,  6018,  6027,  6027,  6037,  6038,  6043,  6044,
    6049,  6056,  6057,  6062,  6069,  6070,  6074,  6075,  6079,  6080,
    6084,  6085,  6094,  6095,  6096,  6100,  6124,  6127,  6135,  6145,
    6150,  6155,  6160,  6167,  6168,  6171,  6172,  6176,  6176,  6180,
    6180,  6184,  6184,  6187,  6188,  6192,  6199,  6200,  6204,  6216,
    6216,  6233,  6234,  6239,  6242,  6246,  6250,  6257,  6258,  6261,
    6262,  6263,  6267,  6268,  6281,  6289,  6296,  6298,  6297,  6307,
    6309,  6308,  6323,  6327,  6329,  6328,  6339,  6341,  6340,  6357,
    6363,  6365,  6364,  6374,  6376,  6375,  6391,  6396,  6401,  6411,
    6410,  6422,  6421,  6437,  6442,  6447,  6457,  6456,  6468,  6467,
    6482,  6483,  6487,  6492,  6497,  6507,  6506,  6518,  6517,  6534,
    6537,  6549,  6556,  6563,  6563,  6573,  6574,  6576,  6577,  6578,
    6579,  6580,  6581,  6583,  6584,  6585,  6586,  6587,  6588,  6590,
    6591,  6593,  6594,  6595,  6598,  6600,  6601,  6602,  6604,  6605,
    6606,  6608,  6609,  6611,  6612,  6613,  6614,  6615,  6617,  6618,
    6619,  6620,  6621,  6622,  6624,  6625,  6626,  6627,  6628,  6629,
    6631,  6632,  6635,  6635,  6635,  6636,  6636,  6637,  6637,  6638,
    6638,  6638,  6639,  6639,  6639,  6644,  6645,  6648,  6649,  6650,
    6654,  6655,  6656,  6657,  6658,  6659,  6660,  6661,  6662,  6673,
    6685,  6700,  6701,  6706,  6712,  6734,  6754,  6758,  6774,  6788,
    6789,  6794,  6800,  6801,  6806,  6815,  6816,  6817,  6821,  6832,
    6833,  6837,  6847,  6848,  6852,  6853,  6857,  6858,  6864,  6884,
    6885,  6889,  6890,  6894,  6895,  6899,  6900,  6901,  6902,  6903,
    6904,  6905,  6906,  6907,  6911,  6912,  6913,  6914,  6915,  6916,
    6917,  6921,  6922,  6926,  6927,  6931,  6932,  6936,  6937,  6948,
    6949,  6953,  6954,  6955,  6959,  6960,  6961,  6969,  6973,  6974,
    6975,  6976,  6980,  6981,  6985,  6995,  7009,  7032,  7044,  7045,
    7055,  7056,  7060,  7061,  7062,  7063,  7064,  7065,  7066,  7074,
    7078,  7082,  7086,  7090,  7094,  7098,  7102,  7106,  7110,  7114,
    7118,  7125,  7126,  7127,  7131,  7132,  7136,  7137,  7142,  7149,
    7156,  7166,  7173,  7183,  7190,  7204,  7214,  7215,  7219,  7220,
    7224,  7225,  7229,  7230,  7231,  7235,  7236,  7240,  7241,  7245,
    7246,  7250,  7251,  7258,  7258,  7259,  7259,  7260,  7260,  7261,
    7261,  7263,  7263,  7264,  7264,  7265,  7265,  7266,  7266,  7267,
    7267,  7268,  7268,  7269,  7269,  7270,  7270,  7271,  7271,  7272,
    7272,  7273,  7273,  7274,  7274,  7275,  7275,  7276,  7276,  7277,
    7277,  7278,  7278,  7279,  7279,  7280,  7280,  7280,  7281,  7281,
    7282,  7282,  7282,  7283,  7283,  7284,  7284,  7285,  7285,  7286,
    7286,  7287,  7287,  7288,  7288,  7289,  7289,  7289,  7290,  7290,
    7291,  7291,  7292,  7292,  7293,  7293,  7294,  7294,  7295,  7295,
    7296,  7296,  7296,  7297,  7297,  7298,  7298,  7299,  7299,  7300,
    7300,  7301,  7301,  7302,  7302,  7303,  7303,  7305,  7305,  7306,
    7306,  7313
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
  "YYYYMMDD", "ZERO", "\"EXEC SQL statement\"", "'+'", "'-'", "'*'", "'/'",
  "UNARY_SIGN", "'^'", "'.'", "'='", "')'", "'('", "'>'", "'<'", "':'",
  "'&'", "$accept", "start", "$@1", "nested_list", "source_element",
  "program_definition", "$@2", "$@3", "program_mandatory", "$@4", "$@5",
  "function_definition", "$@6", "$@7", "nested_prog", "end_program",
  "end_mandatory", "end_function", "identification_division", "$@8",
  "function_division", "program_name", "as_literal", "program_type",
  "program_type_clause", "_init_or_recurs", "environment_division",
  "configuration_section", "configuration_list", "configuration_paragraph",
  "source_computer_paragraph", "source_computer_entry",
  "with_debugging_mode", "computer_name", "object_computer_paragraph",
  "object_computer_entry", "object_clauses_list", "object_clauses",
  "object_computer_memory", "object_char_or_word",
  "object_computer_sequence", "object_computer_segment",
  "repository_paragraph", "opt_repository", "repository_list",
  "repository_name", "repository_literal_list", "special_names_paragraph",
  "opt_special_names", "special_name_list", "special_name",
  "mnemonic_name_clause", "$@9", "$@10",
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
  "any_length_clause", "local_storage_section", "$@25", "linkage_section",
  "$@26", "report_section", "$@27", "opt_report_description_list",
  "report_description_list", "report_description_entry",
  "report_description_options", "report_description_option",
  "control_clause", "control_field_list", "_final", "identifier_list",
  "page_limit_clause", "heading_clause", "first_detail", "last_heading",
  "last_detail", "footing_clause", "page_line_column", "line_or_lines",
  "report_group_description_list", "report_group_description_entry",
  "report_group_options", "report_group_option", "type_clause",
  "type_option", "next_group_clause", "column_clause", "sum_clause_list",
  "sum_clause", "ref_id_exp", "present_when_condition", "varying_clause",
  "line_clause", "line_keyword_clause", "report_line_integer_list",
  "line_or_plus", "_numbers", "source_clause", "group_indicate_clause",
  "_indicate", "report_name", "screen_section", "$@28",
  "procedure_division", "$@29", "$@30", "procedure_using_chaining", "$@31",
  "$@32", "procedure_param_list", "procedure_param", "procedure_type",
  "size_optional", "procedure_optional", "procedure_returning",
  "procedure_declaratives", "$@33", "procedure_list", "procedure",
  "section_header", "paragraph_header", "invalid_statement",
  "section_name", "opt_segment", "statement_list", "@34", "@35",
  "statements", "statement", "accept_statement", "$@36", "accept_body",
  "opt_at_line_column", "line_number", "column_number", "opt_accp_attr",
  "accp_attrs", "accp_attr", "end_accept", "add_statement", "$@37",
  "add_body", "add_to", "end_add", "allocate_statement", "$@38",
  "allocate_body", "allocate_returning", "alter_statement",
  "alter_options", "_proceed_to", "call_statement", "$@39", "call_using",
  "$@40", "call_param_list", "call_param", "call_type", "call_returning",
  "call_on_exception", "$@41", "call_not_on_exception", "$@42", "end_call",
  "cancel_statement", "$@43", "cancel_list", "close_statement", "$@44",
  "close_list", "close_option", "reel_or_unit", "compute_statement",
  "$@45", "compute_body", "end_compute", "comp_equal", "commit_statement",
  "continue_statement", "delete_statement", "$@46", "end_delete",
  "delete_file_statement", "$@47", "display_statement", "$@48",
  "display_body", "with_clause", "disp_attrs", "disp_attr", "end_display",
  "divide_statement", "$@49", "divide_body", "end_divide",
  "entry_statement", "$@50", "evaluate_statement", "$@51",
  "evaluate_subject_list", "evaluate_subject", "evaluate_condition_list",
  "evaluate_case_list", "evaluate_case", "$@52", "evaluate_other", "$@53",
  "evaluate_when_list", "evaluate_object_list", "evaluate_object",
  "opt_evaluate_thru_expr", "end_evaluate", "exit_statement", "$@54",
  "exit_body", "free_statement", "$@55", "generate_statement", "$@56",
  "goto_statement", "$@57", "goto_depending", "goback_statement", "$@58",
  "if_statement", "$@59", "$@60", "if_else_sentence", "$@61", "end_if",
  "initialize_statement", "$@62", "initialize_filler", "initialize_value",
  "initialize_replacing", "initialize_replacing_list",
  "initialize_replacing_item", "initialize_category", "initialize_default",
  "initiate_statement", "$@63", "inspect_statement", "$@64",
  "send_identifier", "inspect_list", "inspect_item", "inspect_tallying",
  "$@65", "tallying_list", "tallying_item", "inspect_replacing",
  "replacing_list", "replacing_item", "rep_keyword", "replacing_region",
  "inspect_converting", "inspect_region", "_initial", "merge_statement",
  "$@66", "move_statement", "$@67", "move_body", "multiply_statement",
  "$@68", "multiply_body", "end_multiply", "open_statement", "$@69",
  "open_list", "open_mode", "open_sharing", "open_option",
  "perform_statement", "$@70", "perform_body", "$@71", "end_perform",
  "perform_procedure", "perform_option", "perform_test",
  "perform_varying_list", "perform_varying", "read_statement", "$@72",
  "read_into", "with_lock", "read_key", "read_handler", "end_read",
  "release_statement", "$@73", "return_statement", "$@74", "end_return",
  "rewrite_statement", "$@75", "write_lock", "end_rewrite",
  "rollback_statement", "search_statement", "$@76", "search_body", "$@77",
  "search_varying", "search_at_end", "$@78", "search_whens", "search_when",
  "$@79", "end_search", "set_statement", "$@80", "set_body",
  "set_environment", "set_to", "set_up_down", "up_or_down",
  "set_to_on_off_sequence", "set_to_on_off", "set_to_true_false_sequence",
  "set_to_true_false", "sort_statement", "$@81", "sort_body", "$@82",
  "sort_key_list", "opt_key_list", "sort_duplicates", "sort_collating",
  "sort_input", "sort_output", "start_statement", "$@83", "@84",
  "start_key", "start_op", "end_start", "stop_statement", "$@85", "$@86",
  "stop_returning", "string_statement", "$@87", "string_item_list",
  "string_item", "opt_with_pointer", "end_string", "subtract_statement",
  "$@88", "subtract_body", "end_subtract", "suppress_statement",
  "_printing", "terminate_statement", "$@89", "transform_statement",
  "$@90", "unlock_statement", "$@91", "opt_record", "unstring_statement",
  "$@92", "unstring_delimited", "unstring_delimited_list",
  "unstring_delimited_item", "unstring_into", "unstring_into_item",
  "unstring_into_delimiter", "unstring_into_count", "unstring_tallying",
  "end_unstring", "use_statement", "use_exception", "use_global",
  "use_exception_target", "_after", "_standard", "exception_or_error",
  "exception_or_overflow", "not_exception_or_overflow", "_procedure",
  "use_debugging", "use_debugging_target", "use_reporting",
  "write_statement", "$@93", "write_from", "write_option",
  "before_or_after", "write_handler", "end_write", "on_accp_exception",
  "on_disp_exception", "opt_on_exception", "$@94", "opt_not_on_exception",
  "$@95", "on_size_error", "opt_on_size_error", "$@96",
  "opt_not_on_size_error", "$@97", "on_overflow", "opt_on_overflow",
  "$@98", "opt_not_on_overflow", "$@99", "at_end", "at_end_sentence",
  "$@100", "not_at_end_sentence", "$@101", "at_eop", "at_eop_sentence",
  "$@102", "not_at_eop_sentence", "$@103", "opt_invalid_key",
  "invalid_key", "invalid_key_sentence", "$@104",
  "not_invalid_key_sentence", "$@105", "_opt_scroll_lines", "condition",
  "expr", "partial_expr", "$@106", "expr_tokens", "expr_token", "eq", "gt",
  "lt", "ge", "le", "exp_list", "e_sep", "exp", "linage_counter",
  "arithmetic_x_list", "arithmetic_x", "record_name", "table_name",
  "file_name_list", "file_name", "mnemonic_name_list", "mnemonic_name",
  "procedure_name_list", "procedure_name", "label", "integer_label",
  "reference_list", "reference", "no_reference_list", "opt_reference",
  "reference_or_literal", "undefined_word", "target_x_list", "target_x",
  "x_list", "x", "arith_x", "prog_or_entry", "alnum_or_id", "simple_value",
  "simple_all_value", "id_or_lit", "id_or_lit_or_func", "num_id_or_lit",
  "identifier", "identifier_1", "qualified_word", "subref", "refmod",
  "integer", "literal", "basic_literal", "basic_value", "function",
  "func_refmod", "func_args", "list_func_args", "trim_args",
  "numvalc_args", "locale_dt_args", "not_const_word", "flag_all",
  "flag_duplicates", "flag_initialized", "flag_next", "flag_not",
  "flag_optional", "flag_rounded", "flag_separate", "in_of", "records",
  "with_dups", "coll_sequence", "_advancing", "_also", "_are", "_area",
  "_as", "_at", "_binary", "_by", "_character", "_characters", "_contains",
  "_data", "_file", "_for", "_from", "_in", "_is", "_is_are", "_key",
  "_line_or_lines", "_lines", "_literal", "_mode", "_number", "_of", "_on",
  "_in_order", "_other", "_program", "_record", "_right", "_set", "_sign",
  "_sign_is", "_size", "_status", "_tape", "_than", "_then", "_times",
  "_to", "_when", "_with", "exec_sql_statement", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1716)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1550)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1716,   267,   560, -1716,  -123,   -24,   142, -1716, -1716, -1716,
     120,   120,   657,   657, -1716, -1716,   509, -1716, -1716, -1716,
   -1716,   809,   809,   439,   859,   859,   666,   558, -1716,   938,
     924, -1716, -1716, -1716, -1716,   -33,   674,   692,   636,   792,
     792, -1716,   653,   117,   716,   725,   880,   794, -1716,   225,
    1138,   971,  1143, -1716,   574, -1716, -1716,   984, -1716, -1716,
   -1716,   829, -1716, -1716, -1716,   945,   879, -1716,    27, -1716,
      33,   120,   657, -1716, -1716, -1716, -1716,   626, -1716,  1166,
     176,   902,  1010,  1130,   933, -1716, -1716,  1040,   657, -1716,
   -1716, -1716,   927,   929,   931,   940,   947, -1716, -1716, -1716,
   -1716, -1716,  1041,   950,  1185,  1021,  1003,   836, -1716,   373,
   -1716, -1716, -1716,    43, -1716, -1716,   956,  1057,  1189, -1716,
     477,   -74, -1716,    49,    49,   985,   972,   973,   859, -1716,
     290,  1249,   151,  1519,  1149, -1716, -1716,   977, -1716,  1157,
    1159,  1038,  1172,  1047, -1716,  1060, -1716, -1716, -1716,  1428,
   -1716, -1716, -1716, -1716, -1716, -1716,  1001,  1103,  1128, -1716,
     933, -1716, -1716,  1019, -1716, -1716,   154, -1716,   569,   472,
     167, -1716, -1716, -1716, -1716,  1098,  1259, -1716,   -85, -1716,
     312, -1716, -1716, -1716, -1716,    48,   161, -1716,   -52, -1716,
   -1716, -1716,  1022,   596,  1372,  1044,  1259,  1259,  1044,  1096,
    1122,  1259,  1259,  1259,  1259,  1259,  1044,  1259,  1436,  1259,
   -1716,   932, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716,  1044,  1041,   176,  1051, -1716,
    1051,  1051, -1716,  1263,  1051, -1716,  1415, -1716,  1327,   477,
     -74, -1716,  1056,  1147,  1164,   -74, -1716,   427, -1716,  1099,
     949, -1716,  1259,  1145,  1236, -1716, -1716,  1411,   792,  1259,
    1287, -1716,   580, -1716, -1716,  1161, -1716,  1259,  1315, -1716,
     731, -1716, -1716, -1716, -1716,  1072,  1280, -1716, -1716,  1044,
    1044,  1259,  1259, -1716,  1259,  1051,  1468,  1044,  1044,  1051,
    1259,  1051, -1716,  1044,    61, -1716, -1716, -1716, -1716,   670,
    1051, -1716, -1716,  1051,  1258,  1129,  1260, -1716,   933, -1716,
     933, -1716, -1716,   -74, -1716,  1085,  1186, -1716, -1716, -1716,
   -1716,  1099, -1716, -1716, -1716,   -39,   -34, -1716, -1716,  1415,
    1259,   396,   396,  1259,    16,  1298,  1259,  1527,  1274, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
     995,   717,  1259, -1716,  1110,  1104, -1716,   971,  1287, -1716,
   -1716, -1716, -1716,  1051, -1716, -1716, -1716, -1716, -1716,  1259,
   -1716, -1716,   708,  1051,  1319, -1716, -1716, -1716, -1716, -1716,
    1051, -1716, -1716,    70, -1716, -1716,   948, -1716, -1716, -1716,
   -1716,  1051, -1716,  1051,  1276,  1051,   933, -1716,  1257,   933,
   -1716, -1716,   -74, -1716,  1105, -1716, -1716,  1475, -1716,  1478,
   -1716, -1716,  1287,  1125,  1259,  1527,  1051,   -62,   -70,  1287,
    1133, -1716,  1259,  1132, -1716,  1132,   -48, -1716, -1716, -1716,
   -1716, -1716,  1287, -1716, -1716, -1716,   446,    68, -1716,  1017,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716,   708, -1716,  1171,
   -1716, -1716, -1716, -1716, -1716, -1716,  1287, -1716, -1716,   948,
   -1716,  1188, -1716,   786, -1716,  1051,  1051,  1051, -1716,  1287,
   -1716, -1716, -1716,  1261, -1716, -1716,   771,  1142,  1176, -1716,
   -1716, -1716,  1051, -1716, -1716, -1716, -1716, -1716, -1716,  1354,
      62,  1389,  1148, -1716, -1716, -1716,  1259,  1259, -1716, -1716,
    2439,   657, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716,   655, -1716,    83,
   -1716,   708,  1287, -1716, -1716, -1716,  1259,   948, -1716,  1298,
    1272,  1191, -1716,  1228,  1298,  1377,  1259,  1541,   149,   442,
     746, -1716,  1156, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716,  1214, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716,  1259,  1051, -1716,  1132, -1716,  1261, -1716,
   -1716,  2171,  1576,  1417,   314, -1716,  1287,    80, -1716, -1716,
   -1716,  1287, -1716, -1716,  1232, -1716,   -65,   -65,  2701, -1716,
    1151, -1716, -1716, -1716, -1716,  1252,  3326,  1154, -1716, -1716,
     655, -1716, -1716,  1044, -1716,  1259,  1377, -1716,   514, -1716,
    1259, -1716,  1259,   727, -1716,  1259, -1716,  1259,  1250,  1259,
    1259, -1716,  1428,   307,  1259,  1175, -1716, -1716, -1716,  1388,
   -1716, -1716,   656,   743,   770,   775,   799,  1182, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716,  1277, -1716, -1716,
    1287, -1716, -1716, -1716, -1716,  1051,  1051,  1412, -1716, -1716,
   -1716,   -19, -1716, -1716, -1716,  1259,  1259, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716,  1008,    20, -1716,   279, -1716,   107,
   -1716, -1716, -1716, -1716,    60,  1436, -1716,   798, -1716, -1716,
   -1716, -1716,  1516, -1716,  1392, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716,  1225, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716,  1191, -1716,  1728, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716,   437, -1716, -1716,  1325, -1716, -1716,
   -1716, -1716,   883, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716,   827, -1716, -1716,    25,  1259, -1716, -1716,   240,   374,
    1051,  1595, -1716, -1716,   -70,  1255, -1716,  1051,  1051, -1716,
    1346,  1346,  1356, -1716,  1051, -1716,   291, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,  1192,
   -1716, -1716,  1238, -1716,  1184,  1245, -1716, -1716, -1716, -1716,
    2929,   107,  1622, -1716,  1288,  1288,   708,  1172,  1172, -1716,
   -1716,  1190, -1716,   107, -1716,  1251, -1716, -1716, -1716, -1716,
   -1716,    66,  1480, -1716, -1716,  1176,  1287,  1199, -1716,  1202,
    1051,  3600,  1218,   -13,  1669,  1652, -1716,  4510,   933,  1262,
    4592,  4510,  1426,  -140,   917,    44,  1051, -1716, -1716,  1528,
   -1716,    44,  1051,  1989,  1051,  3947,  4510, -1716,  4109,   933,
    1051,   933,  1051,    58,    82,  1051,   933, -1716, -1716,  3961,
    4056, -1716, -1716,  1051,  1051,   933,  1051, -1716,   274,  1556,
    1051, -1716, -1716, -1716, -1716, -1716, -1716,  1648, -1716, -1716,
   -1716, -1716, -1716,  1051,    -8,    78,   127,  1227, -1716,  1227,
   -1716, -1716, -1716, -1716,   256, -1716, -1716, -1716, -1716, -1716,
    1051,  1259,  1494,  1494,   314, -1716, -1716, -1716, -1716,  1481,
   -1716,  1287,  1270,  2827,  1216, -1716,  1051, -1716, -1716,  1440,
   -1716,  1541, -1716, -1716, -1716,  1051,  1051,   708,  1182, -1716,
     107,   -70,   -70,  1647,  1436, -1716, -1716, -1716,  1551,   586,
   -1716,  1172,  1221,  1051,  1223,  1224,  1172,   670,  1226,  1229,
    1230,  1233,  1234,  1235,  1239,  1240,  1223,  1558, -1716,  4070,
   -1716, -1716, -1716, -1716,  1479, -1716,  1631, -1716, -1716, -1716,
    1292, -1716,   670, -1716, -1716,  1266, -1716, -1716, -1716,    98,
     933,  1561,  1332, -1716,  1349,  1382,   933,  1055,  1570,  2429,
    1068,  1079,  1572,   160,  1266, -1716, -1716,    72, -1716, -1716,
   -1716,  1603, -1716, -1716, -1716,  1172,    44, -1716, -1716, -1716,
   -1716, -1716,  1310, -1716,    55,  1051, -1716,   308, -1716, -1716,
   -1716, -1716, -1716,  4510, -1716,  1308,  1575,  1664,   944, -1716,
    1312, -1716,  4855,  1577,   766,  1316,  1320,  -190,  1321,   691,
    1537, -1716,  1382,  1537,  1051,  1578,  1289, -1716,   570, -1716,
   -1716, -1716, -1716, -1716,  1476, -1716,    44, -1716,   -64, -1716,
      85, -1716, -1716, -1716,   440,  1674,  3825, -1716, -1716,  1051,
    1580,  4192,  1051,  1547,   967,  1616, -1716,  1399,  1353,  1159,
    1537,   827,   256, -1716,  1294, -1716,  1051,   -21, -1716, -1716,
   -1716,  1259,  1621, -1716, -1716, -1716, -1716, -1716, -1716,    98,
   -1716, -1716,  1051, -1716,  1287,  1511, -1716, -1716, -1716,  1633,
    1172,  2827,  2827,  2827,   -15,   812, -1716, -1716, -1716,  1190,
   -1716,  2827, -1716, -1716, -1716, -1716, -1716, -1716, -1716,  1383,
   -1716, -1716,  1354,   -70,  1635, -1716, -1716,   570,  1496,  1309,
     511,   -30,  2827,  1350,  2827, -1716,  2827, -1716,  3359,  1313,
    2827,  2827,  2827,  2827,  2827,  2827,  2827,  2827, -1716, -1716,
   -1716,  4510,  1565, -1716, -1716,  1413,  1479,  1867,  3011,  1448,
    1522, -1716,   519, -1716, -1716, -1716,   686, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716,     8,   933, -1716, -1716,
     447,  1596,  1596,  1596,  1596, -1716, -1716,  4510,  4510, -1716,
   -1716,   178,  1626,   870, -1716,  1323,  -140, -1716,  1051, -1716,
      -1, -1716, -1716,  1023,  1591, -1716,   570,   126, -1716,   308,
   -1716, -1716, -1716, -1716,    93,  1362,    44, -1716, -1716,  4510,
   -1716, -1716, -1716, -1716,  1400, -1716, -1716, -1716, -1716,  1051,
     -13, -1716,  1112, -1716, -1716,  1382,    98, -1716,  1560,   445,
     583, -1716, -1716,  1051,   583,  1365, -1716,  1190, -1716, -1716,
      64,   948, -1716, -1716,  3722, -1716,  1726,  1559,  4510,  4510,
   -1716,  4295,  1051, -1716,  1601, -1716, -1716,  4510,   570, -1716,
   -1716, -1716,  1674,  1566,  1051, -1716,   548,    77,   445, -1716,
   -1716, -1716,  1051, -1716,  1503, -1716, -1716, -1716,   201,  1051,
   -1716,  1051,  1592,  1106,   -34, -1716,  1159,   185,  3359,  1329,
    1329,   878, -1716, -1716, -1716,  2827,  2827,  2827,  2827,  2827,
    2827,  4631,   812, -1716,  1191, -1716,  1354,  1159, -1716, -1716,
   -1716,  1596, -1716, -1716,  1340,  1342, -1716,   570,  1596,  1573,
   -1716, -1716, -1716, -1716,  1588,  1596,  1520,  1520,  1520,    59,
    1562, -1716, -1716,   365, -1716,    35,   910,  1051,   854,   103,
    1334, -1716,  1190, -1716, -1716,   183,  1335,   892,   462,  1339,
     901,   118,   122,   549,  1341,   939,  4407,   -12,  4510,    44,
   -1716,  1457, -1716, -1716, -1716, -1716, -1716,  1191, -1716, -1716,
    1402, -1716, -1716,  1402, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716,  1398,   -13, -1716,
     173,  1051,  1051,   648, -1716, -1716, -1716,    39,   884,  1430,
   -1716, -1716,  1675, -1716,  1539, -1716,   114,   833,  1596,  1538,
   -1716, -1716,  1543, -1716, -1716, -1716,  1624,  4407,   320, -1716,
   -1716, -1716,  2915, -1716,  1409, -1716, -1716, -1716, -1716, -1716,
     178, -1716, -1716, -1716,  1159, -1716, -1716, -1716,  1191, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716,  1483,  1191, -1716,  1414,
   -1716,  1763, -1716, -1716, -1716,  1231, -1716,   570,  1416, -1716,
      48,    63,   861,    44,    44,  4407,   444,  1156,   933,  1677,
   -1716, -1716,  1808, -1716,  1638, -1716, -1716, -1716, -1716,  1560,
   -1716,  1051,   340,     8,   975,  1384,  1694, -1716,  1390,   570,
     837, -1716,   365, -1716, -1716, -1716,  4510,  1259,     8, -1716,
   -1716, -1716, -1716,   377,  1051,  4407,   482,  1418,  1820,  1051,
     422, -1716, -1716, -1716,  1524,  1525, -1716, -1716,  1112,   201,
   -1716,   769, -1716, -1716, -1716, -1716,  1259,  1659, -1716, -1716,
    1287, -1716,  1051, -1716, -1716,  1674, -1716,  1632, -1716,   861,
    1377,   861, -1716,  1190, -1716, -1716,   910,   862,   862,  1329,
    1329,  1329, -1716,   993,  1431, -1716,  1051, -1716,  1543, -1716,
   -1716,  1596, -1716, -1716, -1716,  1259, -1716, -1716,  1259, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716,    12, -1716, -1716, -1716,
    1588, -1716, -1716, -1716,    98,    98,    98, -1716, -1716, -1716,
   -1716, -1716,  1223,  1349,  4779, -1716,  1051,  1223,  1223,  2827,
   -1716,  1223,  1223,  1223,   389,  1223,  1223, -1716, -1716,  1568,
    4407, -1716,    44, -1716, -1716,   672,   724, -1716, -1716,  3491,
   -1716,   625,    13, -1716, -1716, -1716, -1716,  1035, -1716,  1508,
   -1716,  1495, -1716, -1716, -1716, -1716, -1716, -1716,   406,   406,
     406,   406,  1259, -1716, -1716, -1716, -1716,  1167,  1259, -1716,
   -1716, -1716, -1716,    22, -1716,   833, -1716, -1716, -1716, -1716,
   -1716, -1716,  4510, -1716,  4510,   178, -1716, -1716, -1716,  2915,
   -1716,  1051,  1713,  1405,   912,  1731,  1408,   111,   570, -1716,
   -1716,  1795, -1716, -1716, -1716, -1716,  1416, -1716,  1670, -1716,
    1259,  1563, -1716, -1716,  1377,    44, -1716,  4510,   307,   608,
   -1716, -1716, -1716,  1051,  4510,   999, -1716, -1716, -1716,  1705,
    1584, -1716,  1707, -1716,  1611, -1716, -1716, -1716, -1716,  1390,
   -1716, -1716, -1716,  1590,  1710,  1567,  1557,  1349, -1716,  4510,
     111, -1716,  1579, -1716,   570, -1716,  1746,  1466, -1716, -1716,
    1159, -1716,   800,  1854, -1716,  1036, -1716, -1716, -1716,  1287,
    1749,  1642,  1800,  4678, -1716,  1427, -1716, -1716, -1716,  1259,
   -1716, -1716, -1716,  1259, -1716, -1716, -1716, -1716,   -56,   -56,
      29,    29, -1716, -1716, -1716, -1716, -1716,  1430, -1716,  1011,
   -1716, -1716, -1716,   910, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716,  1191,  1714,  1191,  1716,
   -1716, -1716,  4510, -1716, -1716, -1716, -1716, -1716,  1744, -1716,
   -1716, -1716, -1716, -1716, -1716,  1596,  1596,  1596,  1596,   -56,
   -1716, -1716,   -56,    29,    29, -1716, -1716, -1716,  4407,  1546,
    4407,  1548, -1716, -1716, -1716, -1716, -1716,  1739, -1716,   912,
   -1716,  1775, -1716, -1716, -1716, -1716, -1716, -1716, -1716,   111,
    1112, -1716, -1716,  1112,   -76,  1051, -1716,  1259,  4407, -1716,
   -1716,   994,  3499, -1716,  1833,  1643,  1668,   622, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716,  1051,   619, -1716, -1716, -1716,  1743,  1623,  1051,  1430,
    4407, -1716,  1820, -1716,  1389,  1794,  1389,  1567,   497, -1716,
   -1716,  1745, -1716,  1627, -1716, -1716, -1716,   523, -1716, -1716,
    1259,  1801,  1673, -1716,  1078, -1716,  1692,  1102,  1527,  1704,
    1460,  1259,  1172,  1259,  1051, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716,  1513, -1716, -1716,
   -1716, -1716,   682, -1716, -1716, -1716,  1427, -1716,  1051,   107,
   -1716, -1716, -1716, -1716, -1716, -1716,   -56, -1716, -1716, -1716,
   -1716, -1716, -1716,  1191, -1716,  1191, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716,  4510, -1716,  4510, -1716, -1716, -1716, -1716, -1716,  1855,
    1112,  1112, -1716,  1502,  1608,   933,   647, -1716, -1716, -1716,
   -1716,  1574,  4510, -1716,  1259,   668,  1676, -1716,  1680, -1716,
   -1716, -1716, -1716, -1716, -1716,  1051, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
    1051,  1389, -1716,  1051,  1770, -1716, -1716, -1716, -1716, -1716,
     933, -1716, -1716, -1716, -1716, -1716, -1716, -1716,  1146,  1287,
    1259,  1259,  1747, -1716,  1259, -1716, -1716, -1716, -1716,   314,
   -1716,  1259, -1716,  1051,  1051,  1140,  1742, -1716,  1636,  1287,
     682, -1716, -1716, -1716,  1051, -1716,  1146, -1716, -1716, -1716,
   -1716,  1430,  1430, -1716,  4510,  1112, -1716,  4510,  1259,   933,
     933,  1617, -1716,  1051, -1716,  1505,  1051,  1790, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716,  1051, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716,  1287,  1287,  1259,
   -1716,  1287, -1716,  1287, -1716,  1349, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716,  4510, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716,   -13,   933,  1259, -1716, -1716,
    1051, -1716, -1716, -1716, -1716, -1716, -1716,  1287, -1716, -1716,
   -1716,  1883, -1716,   -13, -1716, -1716,  4510, -1716, -1716
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     0,     1,     0,     0,     0,     4,     6,     7,
      38,    38,     0,     0,     3,     5,     0,     8,    14,    28,
      27,    29,    29,     0,   276,   276,     0,     0,    24,    40,
       0,     9,    15,    30,    26,  1493,     0,   157,     0,   576,
     576,  1494,     0,     0,     0,     0,     0,     0,    39,   278,
       0,    17,     0,    25,    33,    37,    36,  1520,    35,    42,
     159,     0,   161,   288,   289,     0,   340,   281,   580,    18,
      20,    38,     0,    16,    34,  1521,    32,    41,   165,   163,
     249,     0,     0,   475,     0,   583,   581,   598,     0,    19,
      10,    11,     0,     0,     0,     0,     0,    43,    44,    45,
      47,    46,   160,     0,   247,     0,  1485,   258,   162,   251,
     253,   255,   256,   252,   269,   279,     0,     0,   478,  1328,
     282,   343,   290,   588,   588,     0,     0,     0,   276,    23,
      56,    71,    49,    80,  1447,   166,   165,     0,   158,     0,
    1513,     0,  1511,     0,  1486,  1537,   259,   260,   261,  1467,
     250,   254,   268,   270,   283,   341,     0,     0,   481,   287,
       0,   286,   344,  1435,   292,  1476,   588,   585,   591,     0,
     588,   599,   577,    21,    12,     0,  1493,    54,  1520,    55,
    1520,    60,    62,    63,    64,     0,     0,    70,     0,    73,
    1550,    48,     0,  1549,     0,     0,  1493,  1493,     0,     0,
    1528,  1493,  1493,  1493,  1493,  1493,     0,  1493,  1479,  1493,
      79,    81,    83,    85,    86,    87,    89,    88,    90,    91,
      92,    93,    94,    95,  1448,     0,   164,   249,     0,  1514,
       0,     0,  1512,     0,     0,  1538,  1481,  1468,  1487,   280,
     343,   476,     0,     0,   573,   343,   352,   346,   351,     0,
       0,   586,  1493,     0,   596,   589,   590,   600,   576,  1493,
       0,    57,  1520,    59,    61,     0,  1460,  1493,     0,    77,
       0,    72,    74,    52,    50,     0,     0,  1348,   112,     0,
       0,  1493,  1493,  1529,  1493,     0,     0,     0,     0,     0,
    1493,     0,  1480,     0,    99,    82,    84,   167,   248,  1392,
     275,  1339,  1341,   271,     0,     0,     0,  1482,     0,  1488,
       0,   284,   342,   343,   479,     0,     0,   277,   285,   349,
     350,     0,   358,   359,   347,   362,   362,   353,   305,  1481,
    1493,     0,     0,  1493,  1481,  1507,  1493,  1465,     0,   291,
     293,   296,   297,   298,   299,   300,   301,   302,   303,   304,
       0,     0,  1493,   597,     0,     0,   578,    17,     0,  1397,
      69,    58,  1459,     0,    76,    75,    78,    51,    53,  1493,
     101,   102,     0,     0,     0,   153,   152,   103,   104,   156,
       0,   155,   139,  1495,   141,    96,     0,    97,   169,  1453,
    1454,     0,  1340,     0,     0,     0,   262,   263,   266,   257,
    1326,   477,   343,   482,     0,   348,   363,   364,   354,     0,
     364,   356,     0,     0,  1493,  1465,     0,     0,     0,     0,
       0,  1508,  1493,     0,  1466,     0,     0,   294,   295,   592,
     593,   595,     0,   587,   601,   603,     0,     0,    68,     0,
    1406,  1402,  1407,  1405,  1403,  1408,  1404,   145,   146,   148,
     154,   151,   150,  1497,  1496,   142,     0,   111,   110,   100,
     107,  1535,   105,     0,  1393,   273,     0,   274,   264,     0,
     265,  1327,   480,   484,   574,   372,   366,     0,   317,   337,
    1455,  1456,   326,  1342,   321,   320,   319,  1347,  1346,  1503,
    1479,  1491,     0,   572,   338,   339,  1493,  1493,   594,   603,
       0,     0,    13,    66,    67,    65,   117,   131,   127,   132,
     114,   130,   128,   115,   116,   129,   113,   118,   119,   121,
     147,     0,   140,   143,   108,  1536,  1493,    98,   184,  1507,
       0,  1545,   230,     0,  1507,  1498,  1493,  1477,  1498,   233,
       0,   232,  1549,   217,   216,   168,   170,   171,   172,   173,
     174,   175,     0,   176,   177,   229,   178,   179,   180,   181,
     182,   183,   185,  1493,   272,   267,     0,   483,   485,   486,
     575,     0,  1469,     0,  1495,   357,     0,   307,  1343,  1504,
     328,     0,   310,  1492,  1533,   336,     0,     0,     0,   609,
     613,   604,   605,   606,   607,   612,     0,     0,   120,   123,
       0,   149,   144,     0,   106,  1493,  1498,  1546,   192,   234,
    1493,  1499,  1493,     0,  1478,  1493,  1474,  1493,     0,  1493,
    1493,   241,  1467,     0,  1493,     0,   489,   487,   388,     0,
     462,   397,   430,   418,   427,   424,   421,  1547,   398,   399,
     400,   401,   402,   403,   404,   405,   406,  1524,   361,   431,
       0,   407,   394,   408,   409,     0,     0,  1531,   411,   412,
     410,   458,   414,   415,   413,  1493,  1493,   355,   373,   374,
     375,   376,   377,   378,   395,   379,   380,   381,   382,   383,
     384,   385,   386,   387,     0,     0,  1470,     0,   367,     0,
     318,   309,   308,   306,   327,  1479,  1534,   315,   324,   323,
     325,   322,     0,   611,   614,   672,   723,   732,   739,   743,
     767,   772,   790,   783,   791,   792,   798,   831,   840,   842,
     869,   877,   879,  1545,   885,     0,   896,   917,   919,   955,
     957,   961,   671,   967,   980,  1000,  1017,  1019,  1023,  1030,
    1031,  1047,  1067,  1085,     0,  1104,  1115,  1123,  1125,  1127,
    1129,  1134,  1156,  1179,  1551,   608,   620,   621,   622,   623,
     624,   625,   626,   627,   629,   628,   630,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
     644,   645,   646,   647,   648,   649,   650,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,  1152,  1153,  1154,   669,   670,
      22,     0,   122,   109,     0,  1493,   194,   193,   189,     0,
       0,  1473,   233,   228,     0,     0,   231,     0,     0,   240,
    1518,  1518,     0,   242,     0,   213,  1493,   474,   428,   429,
     416,   417,   425,   426,   422,   423,   419,   420,  1548,     0,
    1525,   456,   438,   389,  1388,   472,  1532,   459,   460,   457,
       0,     0,   391,   393,  1451,  1451,     0,  1511,  1511,   371,
     368,  1398,  1400,   464,   466,   468,  1472,   329,   330,   331,
     332,     0,     0,   311,  1490,   317,     0,     0,   615,     0,
       0,     0,  1243,   738,     0,   769,   774,     0,     0,     0,
       0,     0,     0,  1243,   871,     0,     0,   881,   886,     0,
    1243,     0,     0,     0,     0,     0,     0,   969,   990,     0,
       0,     0,     0,     0,     0,     0,     0,  1099,  1097,     0,
       0,  1124,  1122,     0,     0,     0,     0,  1157,  1163,     0,
       0,   137,   133,   138,   136,   134,   135,   124,   125,   202,
     203,   201,   200,     0,   187,   188,  1505,   222,   221,   222,
     218,   246,   235,   236,  1438,   239,  1519,   243,   244,   245,
    1344,  1493,   498,   498,  1495,   518,   490,   493,   494,     0,
     461,     0,  1543,     0,  1389,  1390,     0,   396,   463,     0,
     390,  1477,   432,   433,  1399,     0,     0,     0,  1547,   467,
       0,     0,     0,  1471,  1479,   316,   602,   610,   721,   691,
    1387,  1511,     0,     0,  1421,  1424,  1511,  1319,     0,     0,
       0,     0,     0,     0,     0,     0,  1421,   730,  1363,   728,
    1353,  1355,  1361,  1362,  1440,   733,     0,  1242,  1264,  1338,
       0,  1334,  1336,  1335,  1382,   745,  1381,  1383,   771,   768,
     773,   786,     0,  1321,  1449,  1522,     0,  1402,   829,   691,
       0,  1355,   838,     0,   745,   848,   847,  1463,   844,   846,
     876,   873,   872,   875,   870,  1511,   878,  1349,  1351,   880,
    1332,   890,  1541,  1241,   898,   918,   500,     0,   921,   922,
     923,   956,  1071,     0,   958,     0,   965,     0,   968,   991,
    1338,   981,   990,   983,     0,   988,     0,  1335,     0,  1442,
    1181,  1324,  1522,  1181,     0,  1045,  1036,  1325,     0,  1331,
    1048,  1049,  1050,  1051,  1052,  1060,  1053,  1063,     0,  1329,
       0,  1068,  1086,  1100,  1101,  1475,     0,  1106,  1108,     0,
    1120,     0,  1126,     0,  1131,  1136,  1164,     0,  1165,  1513,
    1181,     0,  1438,   196,   195,   186,     0,     0,   220,   219,
    1458,  1493,     0,   212,   206,   237,  1439,  1345,   214,     0,
     499,   495,     0,   496,     0,   488,   491,   439,  1544,   440,
    1511,     0,     0,     0,  1307,  1305,  1370,  1310,  1364,  1368,
    1369,     0,  1391,   473,   392,  1452,   370,   369,  1401,  1526,
     469,   335,  1503,     0,   313,   722,   673,  1472,     0,   700,
       0,     0,     0,     0,     0,  1409,  1426,  1420,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1410,   731,
     724,     0,     0,  1354,  1441,   736,  1440,  1244,     0,   741,
       0,   746,   756,  1380,   770,  1379,   776,   787,   784,   789,
     788,  1243,  1322,  1450,  1323,  1523,  1230,   797,   830,   799,
     809,  1196,  1196,  1196,  1196,   839,   832,     0,     0,   841,
    1464,  1243,   867,   854,   850,   852,  1243,   874,     0,  1350,
     883,  1542,   888,   900,     0,   501,     0,   944,   929,   920,
     924,   926,   927,   928,  1075,     0,     0,   966,   962,     0,
     974,   971,   973,   972,   975,   982,   985,   616,  1243,     0,
       0,   992,     0,  1443,  1444,  1522,     0,  1018,  1002,  1025,
    1038,  1046,  1032,     0,  1038,     0,  1375,  1376,  1061,  1064,
       0,     0,  1330,  1059,     0,  1058,     0,  1088,     0,     0,
    1098,     0,     0,  1107,     0,  1121,  1116,     0,     0,  1132,
    1133,  1130,  1475,     0,     0,  1166,     0,     0,  1025,   126,
     206,   204,   197,   198,     0,   225,   211,  1457,  1438,     0,
     492,   497,   503,   513,   362,   519,  1513,   434,     0,  1315,
    1316,     0,  1308,  1309,  1394,     0,     0,     0,     0,     0,
       0,     0,     0,  1527,  1545,   334,  1503,  1513,   312,   696,
     687,  1196,   677,   684,   678,   680,   682,     0,  1196,     0,
     676,   683,   690,   689,     0,  1196,  1509,  1509,  1509,   694,
     695,  1372,  1371,     0,  1360,  1307,  1305,     0,     0,  1307,
       0,  1356,  1357,  1358,  1320,  1307,     0,     0,  1307,     0,
       0,  1307,  1307,  1307,     0,     0,  1203,  1449,     0,     0,
     734,     0,  1255,  1256,  1257,  1290,  1258,  1545,  1294,  1299,
    1539,  1265,  1302,  1539,  1283,  1262,  1272,  1254,  1253,  1291,
    1261,  1263,  1273,  1274,  1275,  1276,  1277,  1292,  1246,  1295,
    1297,  1278,  1279,  1280,  1281,  1282,  1249,  1250,  1251,  1252,
    1266,  1289,  1260,  1271,  1248,  1247,  1259,  1268,  1269,  1270,
    1267,  1284,  1285,  1286,  1287,  1288,  1245,     0,     0,  1337,
     752,     0,     0,   759,   781,   782,   775,   777,     0,  1203,
    1235,  1237,   794,  1231,  1232,  1233,     0,  1550,  1196,     0,
    1197,   802,  1199,   803,   800,   801,     0,  1203,  1449,   862,
     864,   863,   857,   859,   865,   868,   843,   855,   851,   849,
    1243,   616,   845,  1352,  1513,   882,  1333,   616,  1545,   908,
     909,   911,   913,   914,   910,   912,   903,  1545,   899,     0,
     945,     0,   947,   946,   948,   939,   940,     0,     0,   925,
    1077,  1515,     0,     0,   959,  1203,  1449,  1549,     0,   986,
     617,   993,   994,   997,     0,   989,  1188,  1187,   996,  1002,
    1182,     0,     0,  1230,     0,     0,     0,  1037,     0,     0,
       0,  1062,     0,  1066,  1065,  1056,     0,  1493,  1230,  1103,
    1102,  1109,  1110,  1111,     0,  1203,  1449,     0,  1436,     0,
    1111,  1178,  1168,  1167,  1173,     0,  1175,  1176,  1183,  1438,
     199,     0,   208,   209,   238,   207,  1493,   505,   516,   517,
     515,   521,     0,   448,   449,  1475,   437,   450,   446,   444,
    1498,   442,  1365,  1366,  1367,  1318,  1306,  1311,  1312,  1313,
    1314,  1317,  1395,     0,   470,   333,     0,   688,  1199,   679,
     681,  1196,   685,   675,   715,  1493,   704,   705,  1493,   716,
     706,   707,   710,   720,   717,   708,     0,   718,   709,   719,
     701,   702,   674,  1510,     0,     0,     0,   692,   693,  1374,
    1359,  1373,  1421,  1449,     0,  1425,     0,  1421,  1421,     0,
    1418,  1421,  1421,  1421,     0,  1421,  1421,  1204,   725,  1206,
    1203,   737,     0,  1293,  1540,  1296,  1298,   742,   740,   747,
     748,   591,     0,   758,   757,  1169,  1170,   762,   760,     0,
     780,     0,   785,   616,   616,   795,   793,  1234,   809,   809,
     809,   809,  1493,   814,   827,   828,   815,     0,  1493,   818,
     819,   822,   820,     0,   821,   811,   812,   804,   810,   616,
    1200,  1195,     0,   833,     0,  1243,  1243,   861,   616,   858,
     853,     0,   891,     0,     0,   915,     0,     0,     0,   941,
     943,     0,   935,   951,   936,   937,   930,   931,   951,  1069,
    1493,     0,  1516,  1076,  1498,   960,   963,     0,     0,   977,
     987,   984,   619,     0,     0,  1004,  1003,  1219,  1221,  1021,
    1216,  1217,  1028,  1026,     0,  1243,  1039,  1243,  1033,  1041,
    1054,  1055,  1057,  1445,  1095,  1210,     0,  1449,  1117,     0,
       0,  1437,  1137,  1138,     0,  1141,  1144,  1148,  1142,  1174,
    1513,  1177,  1189,  1461,   205,     0,   226,   227,   223,     0,
       0,   507,     0,  1530,   441,     0,   435,   451,   447,  1493,
     436,   443,  1396,  1493,   465,   314,  1194,   686,     0,     0,
    1239,  1239,   703,   698,   697,   699,  1414,  1203,  1422,     0,
    1434,  1419,  1412,  1432,  1413,  1415,  1416,  1429,  1430,  1417,
    1411,   616,  1207,  1202,   726,   735,  1545,     0,  1545,     0,
     749,   750,     0,   754,   753,   755,  1171,  1172,   765,   763,
     616,   778,   779,  1236,  1238,  1196,  1196,  1196,  1196,     0,
     816,   817,     0,  1239,  1239,   813,  1198,   616,  1203,  1321,
    1203,  1321,   860,   866,   856,   884,   892,   894,   901,   904,
     905,  1483,   916,   897,   902,   951,  1377,  1378,   951,     0,
     934,   932,   933,   938,  1079,     0,  1517,  1493,  1203,   976,
     970,     0,   618,   998,     0,     0,  1010,     0,   616,   616,
    1022,  1020,  1218,  1029,  1024,  1027,  1034,   616,  1043,  1042,
    1446,     0,     0,  1096,  1087,  1211,  1113,  1213,     0,  1203,
    1203,  1128,  1436,  1140,  1491,  1146,  1491,  1210,     0,  1226,
    1228,  1192,  1190,  1223,  1224,  1191,  1462,     0,   224,   504,
    1493,     0,   509,   514,  1509,   550,   570,   565,  1465,     0,
       0,  1493,  1511,  1493,     0,   520,   526,   527,   528,   537,
     529,   531,   534,   522,   523,   524,   530,   533,   551,   535,
     538,   525,     0,   532,   536,   455,   452,   453,     0,     0,
    1386,   712,  1384,  1385,   711,   714,     0,   713,   727,  1423,
    1205,   616,  1301,  1545,  1304,  1545,   751,   766,   744,   616,
     761,   808,   807,   806,   805,   824,   823,   826,   825,  1201,
     835,     0,   834,     0,   616,   895,   889,   906,  1484,     0,
     950,   942,   951,   953,     0,     0,  1082,  1078,  1073,   964,
     979,     0,     0,  1005,  1493,  1012,     0,  1006,     0,  1009,
    1220,  1222,   616,  1040,   616,  1089,  1090,  1091,  1092,  1093,
    1094,   616,  1114,  1105,  1214,  1209,  1112,  1119,  1118,  1139,
       0,  1491,  1143,     0,  1150,  1162,  1159,  1161,  1160,  1155,
    1158,   616,   616,  1193,  1180,  1225,  1186,  1185,  1500,     0,
    1493,  1493,   511,   549,  1493,   571,   569,   566,   567,  1495,
     559,  1493,  1243,     0,     0,     0,     0,   552,     0,     0,
     557,   560,   563,   454,   445,   471,  1500,  1208,  1300,  1303,
     764,  1203,  1203,   893,     0,   949,   954,     0,  1493,  1080,
       0,     0,  1070,  1072,   978,     0,     0,  1015,  1013,  1014,
    1008,  1007,  1035,  1044,  1212,   616,  1145,     0,  1149,  1151,
    1135,  1227,  1229,  1501,  1502,  1184,   506,     0,     0,  1493,
     502,     0,   558,     0,   555,  1449,   553,   554,   544,   542,
     543,   545,   541,   546,   540,   539,     0,   564,   562,   561,
    1240,   837,   836,   907,   952,     0,  1083,  1493,  1074,  1243,
    1011,  1016,  1001,  1215,  1147,   508,   510,     0,   548,   547,
     568,     0,  1081,     0,   999,   512,     0,  1084,   556
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1716, -1716, -1716, -1716,  1927, -1716, -1716, -1716,    34, -1716,
   -1716, -1716, -1716, -1716,  1582, -1716, -1716, -1716,  1241, -1716,
   -1716,    40,  1912, -1716, -1716,  1881,   922, -1716, -1716, -1716,
   -1716, -1716,  1751,  1804, -1716, -1716,  1759,    15, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716,  1752, -1716, -1716, -1716, -1716,
    1735, -1716, -1716, -1716, -1716, -1716,   457,   616, -1716, -1716,
   -1716, -1716,  1434, -1716, -1716,  1348,   801, -1716, -1716, -1716,
   -1716, -1716, -1716,  1509, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716,  1822, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,   599, -1716,
     593,   811, -1716, -1716, -1716, -1716, -1716,  1009,   102, -1716,
    1357, -1716, -1716, -1716, -1716, -1716, -1716,   155, -1716, -1716,
    1748, -1716,  1863, -1716, -1716, -1716, -1716,  1583, -1716, -1716,
    1861,   847, -1716, -1716, -1716, -1716,  1737, -1716,  1931,  1821,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,  1097, -1716,
   -1716, -1716,  1396, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716,   581, -1716, -1716, -1716,  1740,
    1667, -1716, -1716,   814, -1716, -1716,  -309, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716,   121,   124,  1131,
     131, -1716,   132, -1716, -1716, -1716,   347,   350, -1716,   430,
   -1716,   357, -1716,   -51,   143, -1716, -1716,   144, -1716,   146,
   -1716, -1716, -1716,  1155, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716,  1452, -1716, -1716, -1716,  1048,
   -1716,  -924, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,   -23,
   -1716, -1716, -1716, -1716, -1716, -1716,  -163, -1716, -1716, -1716,
   -1716,   359, -1716, -1716,   162, -1716, -1716, -1716, -1716, -1716,
    1911,  1100, -1716,   295, -1716, -1716, -1716, -1716,  1540, -1716,
   -1716, -1716, -1716, -1716, -1716,  -843, -1716, -1716,   215, -1716,
   -1716, -1716, -1716,   979,   620,   623, -1716, -1716,   341, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716,   980, -1716, -1716,   309, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,  -227, -1716,
     271, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716,   774, -1716, -1716,   780, -1716, -1716, -1716, -1716,   504,
     262, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716,    96,   776, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716,   772, -1716, -1716, -1716,
     254, -1716, -1716,   487, -1716, -1716, -1716, -1191, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1032,
     961, -1716, -1716,   243, -1716, -1716,   468, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,   710, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716,   745, -1716,   233, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
     951, -1716,   952, -1716, -1716,  1152, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716,   937,   449, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716,    73, -1716,   450, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716, -1716,  -388, -1716,
   -1210, -1716, -1716, -1263, -1217, -1010, -1716,   408, -1716, -1104,
   -1716, -1716, -1716, -1716,    57, -1716, -1716, -1716, -1716,   -37,
   -1716, -1716,   260, -1716, -1716, -1716, -1716,    71, -1716,  -426,
   -1715, -1716, -1716,   567, -1716,  -809, -1290,  -867, -1231, -1716,
   -1716, -1716, -1207, -1203, -1201, -1200, -1199,    87,  -186,  -166,
    -532, -1173,  -885,   315,   981, -1046,   -84, -1716, -1105, -1716,
    -808, -1716,   856,  -226,  -229, -1716, -1716,  -730,   327,  -828,
   -1033,   196,  -751, -1716, -1716,   485, -1031, -1604,  -976,  1204,
    -897,   195,  -614,  -162, -1716,  1115,  -212,  -676,  -931,  -330,
    -652,  -972, -1716, -1716, -1716, -1716, -1716,  1857, -1716, -1089,
     865, -1716, -1716, -1716, -1664,  1237,   115,  1773,   813,  -447,
   -1716,  1042,  -409,  1488, -1716,  -637, -1716, -1111,  1121,  -446,
     824, -1716, -1716,  -719, -1716, -1313,  -175,  -567,  -524,   -83,
   -1040, -1716,   675, -1338,  -848, -1090, -1716,  1283,  2060,  -956,
   -1716, -1716, -1716, -1716, -1716, -1716, -1716,   659, -1716, -1716,
    -715,  1120,  -129, -1716
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,     6,     7,     8,    24,    39,    69,   128,
     258,     9,    25,    40,    70,    90,   502,    73,    71,    35,
      11,    21,    27,    42,    57,    58,    17,    37,    77,    97,
      98,   191,   192,   178,    99,   179,   180,   181,   182,   505,
     183,   184,   100,   187,   188,   189,   270,   101,   210,   211,
     212,   213,   462,   386,   527,   459,   460,   461,   214,   369,
     516,   517,   518,   811,   947,   519,   948,   215,   382,   383,
     522,   216,   447,   448,   217,   218,   219,   220,   221,   222,
     223,    48,    78,    80,   104,   102,   135,   388,   463,   546,
     547,   956,   818,  1155,  1362,   548,   952,   549,  1368,  1369,
    1642,  1164,   550,   551,   552,   553,   960,  1158,  1868,   554,
     555,   556,   557,   558,   559,   560,   561,   833,   562,   138,
     108,   109,   110,   111,   149,   112,   396,   397,   470,   113,
     114,    31,    66,   154,    84,   239,   159,   120,   160,   121,
     164,   250,   340,   341,   693,   342,  1398,   885,   577,   343,
     486,   344,   699,   345,   346,   694,   877,   878,   879,   880,
     347,   348,   349,    83,   240,   161,   162,   163,   247,   248,
     324,   475,   477,   325,   326,   667,   408,   409,   572,   869,
     327,   571,   668,   669,   670,   990,   671,   672,   673,   674,
     675,  1656,   676,   982,  1377,  1880,  1657,  1658,  1659,  1660,
    1876,  1661,  2066,  2067,   677,   678,   859,   679,   680,   681,
     575,   998,   873,   874,  1884,   682,   683,   118,   313,   158,
     402,   244,   473,   567,   568,   569,   836,   976,   977,  1171,
    1172,  1085,   978,  1647,  1871,  2032,  2172,  2240,  1372,  1650,
    1175,  1375,  1873,  2053,  2054,  2255,  2055,  2056,  2057,  2058,
    2246,  2059,  2060,  2061,  2062,  2190,  2191,  2179,  2063,  2064,
    2176,   494,   317,   570,    51,   257,   435,    87,   124,   123,
     166,   167,   168,   254,   354,   126,   356,   499,   500,   591,
     592,   593,   594,   595,   889,  1589,  1590,  1822,   596,   756,
     757,   890,  1008,  1209,  1419,  1420,  1415,  1700,  1701,  1206,
     758,   891,  1027,  1232,  1230,   759,   892,  1035,  1450,   760,
     893,  1508,   761,   894,  1242,  1510,  1739,  1740,  1741,  1513,
    1747,  1930,  1928,  2089,  2088,   762,   895,  1049,   763,   896,
    1050,  1516,  1517,   764,   897,  1051,  1248,  1251,   765,   766,
     767,   898,  1756,   768,   899,   769,   900,  1058,  1528,  1775,
    1776,  1259,   770,   901,  1062,  1266,   771,   902,   772,   903,
    1067,  1068,  1272,  1273,  1274,  1551,  1549,  1788,  1275,  1542,
    1543,  1787,  1546,   773,   904,  1074,   774,   905,   775,   906,
     776,  1080,  1555,   777,   908,   778,   910,  1557,  1957,  2104,
    2106,   779,   911,  1283,  1566,  1795,  1959,  1960,  1961,  1963,
     780,   912,   781,   913,  1087,  1289,  1290,  1291,  1578,  1806,
    1807,  1292,  1575,  1576,  1577,  1800,  1293,  1970,  2207,   782,
     914,   783,   915,  1094,   784,   916,  1096,  1298,   785,   917,
    1098,  1304,  1588,  1980,   786,   918,  1101,  1307,  1821,  1102,
    1103,  1104,  1592,  1593,   787,   919,  1602,  1986,  2125,  2217,
    2272,   788,   920,   789,   921,  1991,   790,   922,  1603,  1994,
     791,   792,   923,  1115,  2132,  1324,  1605,  1997,  1838,  1839,
    2134,  1322,   793,   924,  1120,  1121,  1122,  1123,  1336,  1124,
    1125,  1126,  1127,   794,   925,  1091,  1974,  1294,  2213,  1580,
    1809,  2116,  2212,   795,   926,  1337,  1618,  2001,  2004,   796,
    1134,  1133,  1340,   797,   929,  1136,  1137,  1845,  2143,   798,
     930,  1140,  1346,   799,   932,   800,   933,   801,   934,   802,
     935,  1351,   803,   936,  1353,  1852,  1853,  1630,  1855,  2015,
    2152,  2017,  2230,   804,   805,   938,  2159,  1148,  1356,  1634,
    1748,  1929,  1860,   806,  1636,   807,   808,   940,  1317,  1862,
    2113,  2021,  2164,  1677,  1531,  1532,  1779,  1781,  1947,  1728,
    1729,  1911,  1913,  2081,  2006,  2007,  2141,  2145,  2225,  1829,
    1830,  1988,  1831,  1989,  2022,  2023,  2161,  2024,  2162,  1522,
    1523,  1524,  1753,  1525,  1754,  2075,  1082,  1083,  1037,  1038,
    1237,  1238,  1481,  1482,  1483,  1484,  1485,  1184,  1385,  1426,
    1028,  1052,  1252,  1110,  1116,   399,   400,  1128,  1129,  1280,
    1105,  1041,  1042,   300,   301,   482,  1168,   489,   278,  1076,
    1077,  1029,  1054,  1187,  1423,  1710,  1808,  1965,  1060,  1106,
    2071,  1031,  1010,   854,   984,   985,  2073,  1032,   871,   872,
    1033,  1215,  1217,  1430,  1444,  1439,  1436,   249,  1854,  1165,
    1235,  1315,  2002,   225,  1254,   992,   391,   416,  1166,   267,
    2027,  1785,   425,   238,   687,  1210,   618,   169,   615,   293,
     308,  2109,   145,   310,   886,   584,    43,   456,   612,  2235,
     580,  1156,   422,  1704,   233,   230,  1813,   967,   185,  1256,
     851,  1394,   284,   685,   697,   526,   236,  1735,  1282,  1179,
     608,   849,  1529,   809
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     122,   260,   304,   194,   303,   306,   481,   689,   907,  1142,
    1257,   870,  1053,   875,   619,   620,   563,   411,  1591,   995,
     996,   279,   280,  1332,  1341,  1036,   285,   286,   287,   288,
     289,  1501,   291,   939,   294,  1502,  1069,  1503,  1504,  1505,
    1544,   853,   449,  1279,   582,  1533,  1534,  1535,   360,  1897,
    1075,  1279,  1189,    22,  1228,  1382,   375,   881,  1446,  1357,
     379,  1075,   381,  1361,   194,   105,   302,  1114,   302,   302,
    1075,   392,   302,  1244,   392,   350,   122,   351,   165,  1705,
    1706,    85,   815,  1084,   358,  1040,  1635,  1325,  1075,  1270,
     876,  1075,   363,   453,   962,  1537,  1130,  1279,   165,  1923,
     599,   307,  1598,  1413,    89,  1382,   372,   373, -1471,   374,
    -190,   265,    92,  1554,  1001,   380,   866,   449,   292,  1421,
     866, -1513,   496,   302,   503,  1416,  1585,   302,   127,   302,
     186,  2114,   406,  1890,   438,  1570,   691,   406,   302,   431,
    1030,   302,    14,  1943,   450,  1682,   437,  2025,   949,  1030,
    1063,   452,  1702,  -345,  -345,   413,  1318,   385,   418,   420,
     175,   423,  1395,  1211,  1095,  1097,   487,   465,  1218,   467,
     268,   698,    88,  1382,  1625,  -729,  1399,   432,  1138,  1030,
     359,    41,  1571,  2009,  1119,   988,   616,  1327,  1382,   488,
      54,   601,  1382,  1370,   439,   264,  -191,   875,   105,  1539,
     478,   302,    52,   165,  1582,   857,  1333,   490, -1513,  1267,
    1758,   302, -1388,  1653,   309,  1160,   165,   484,   302,  1364,
     498,  1520,   165,  1039,   398,    75, -1549,  1278,  -190,   464,
      41,   302,  1118,   302, -1549,  1039,   392,  1089,   392,   480,
     564,  1628,  1047,   864,   523,  1153,  2011,   492,  1371,   883,
    1189,  1189,  1189,  1382,   483,  1569,   302,   565, -1549, -1475,
    1189,  1090,   617,  1811,  1065, -1475,  1047,     3,  1066,   983,
      16,  1201,  1202,   176,  1521,  1730,  1327,   264,  1233,  1644,
    1422,  1189,  1146,  1189,   454,  1189,  1652,  1432,   866,  1189,
    1189,  1189,  1189,  1189,  1189,  1189,  1189,  1279,  1330,  1572,
    1654,   573,   440,   302,   302,   302,   440,  1676,  1233,  1147,
     602,  1777,   398,  1039,  -191,   471,   830,  1627,   485,  1544,
     578,   586,   587,  1160,  1200,     4,  1365,   858,   622,   867,
    1119,  1190,  1378,    12,  1243,   392,  1253,   453,  1924,  1331,
    1600,   950,  1295,   441,     5,  1383,  1053,   441,  1812,  1573,
     442,   603,   971, -1520,   442,  1327,  1675,   954,  2115,  1599,
    1418,   613,  1119,  1154,   690,  2112,  1203,   611, -1487,   695,
    1268,   261,   299,   229, -1475,  -345,  1681,   299,  1160,   972,
     973,  1196,  1197,  1053,  1519,  1138,   299,  1655,   625,   951,
    1233,  1678,  2070,    63,   497,  1383,   684,   269,  1678,  1742,
    1286,   692,   302,   407,   271,  1678,   957,  1174,   410,  1069,
    2219,   266,   299,   623,  1053,  1752,  1731,  1327,  1887,  1759,
     357,  1761,   443,   -31,   700,   700,   443,   106,  1863,   299,
     814,   865,    13,  1783,  -190,   819,  1891,   820,   852,    55,
     824,   299,   825,  1384,   827,   828,  1944,  1663,  1925,   834,
    1047,  1186, -1493,     5,  1189,  1189,  1189,  1189,  1189,  1189,
    1189,    86,  1053,  1383,  1791,   581,  1827,  1610,  1584, -1471,
      89, -1475,  1556,  1396,   440,  1002,  1327, -1475,  1383, -1475,
    1447,  1816,  1383,   600, -1475,   444,   299,  1506,  1334,   444,
     860,   861, -1475,  1712,   855,  -584,   190,   299, -1475,   152,
     299,   190,  1595,   277,   868,    41,   299,  1784,  -582,  1335,
    1271,   504,   277, -1473,   445,   441,  1536,  1538,   445,   299,
    -191,  1848,   442,    56,   299,   281,   107,   299,   454,  1190,
    1190,  1190,  1382,   290,  1162,   175,   994,  1574,   958,  1190,
     299,   597,  1819,  1383,  1875,   955,  1801,  1998,  1586,  1637,
    1864,  1279,   297,   299,  1544,   446,  1760,   175,  1204,   446,
    1190, -1427,  1190,  1053,  1190,   882,  1433,   831,  1190,  1190,
    1190,  1190,  1190,  1190,  1190,  1190,  1722,  1416,  1840,    64,
    1723,  2280,  1540,  1615,   974,   501,  1541,  1619,  1620, -1475,
    1622,   961,   190,   177,   443,   488,  1626,  1424,   964,   965,
      75,  1709, -1475,  1828,  1431,   970,   370,   371, -1475,  1948,
    -584,  1950,  1907,   876,   377,   378,  1207,  1973,   959,  1382,
     384,   387,    75,  -582,   106,   832,  1914,  1338,  1742,    23,
      65,  1817,  1161,  1386,  1387,  1388,  1389,   816,  1390,  1287,
     953, -1433,  1162,   299,  1978,    63,  1327,  1327,   176,  1186,
    1186,  1186, -1471, -1520, -1435, -1435, -1498,   444,   302,  1186,
     611,   979,   302,  2155,  1553,   302,   302,  1198,  1253,  1849,
     176,  1678,   302,   927,  1005, -1549,  2010, -1549,  1327,  1674,
    1186,   817,  1186,  1606,  1186, -1549,   445,  1606,  1186,  1186,
    1186,  1186,  1186,  1186,  1186,  1186,  2174,  1162,  -210,  1915,
    1738,  2150,  1288,  2153,  2156, -1549,  1511,  1632,  1790, -1471,
     455,  1633,  2157,  1163,  1792,   414, -1549,   415,  2091,  2092,
    2093,  2094, -1549,   107,  1152,  1319,  1664,   446,  1893,  1894,
    1895,  1043,   177,  1190,  1190,  1190,  1190,  1190,  1190,  1190,
    1896,  1167,  1733,     4,  1417,  1901,  1902,   975,   429,  1904,
    1905,  1906,  1092,  1909,  1910,  1815,  1107,     5,  1111,   359,
    1111,  1117,  1358,  1092,  1662,   440,  1208,  1968,   263,  1177,
    2018,  1119,   488,   488,  2110,  1457,  1458,  2111,  1111,  2076,
    2076,  1339,  1279,  1189,   495,  2158,   928,   528,  1189,  1108,
     529,   302,  1253,  2078,  1827,  2136,  1169,   255,  1749,  2137,
    1908,  2138,  2139,  2140,  1459,   530,   441,   299,   302,  1711,
    1460,  1745,  1418,   442,  1055,   531,  2166,  1185,   190, -1471,
    2126,   312,  1383,  2013,  1193,   175,   318,   430,  1916,   150,
    1253,    64,  2076,  2076,  2210,  1109,    26,  1112,  2227,   302,
     302,  1462,  1132,  1793,  2100,  1463,  2102,  1527, -1549,   265,
     507,  1144,  1796,  1186,  1186,  1186,  1186,  1186,  1186,  1186,
    1512,    45,  2127,   190,   299,  1842,  1762,  1327, -1549,  1763,
     389,     5,    32,  1526,  2119,  1327,   246,  1764,  1765,  1766,
    1918,  1520,  2128,   319, -1498,   443,   190, -1471,  1527,  1653,
      75,   508,  2244,    19,   401,    29,    55,  1949,   509,  1951,
      46,    93,    33,   440,  1921,  2147,  2148,   256,    47,  1383,
    1933,  1934,  1386,  1387,  1388,  1389,   524,  1390,   359,  1953,
   -1431,  2205,  2167,  1327,  1559,   626, -1549,  1560,  1561,   532,
     813,  1828,  1053,    18,  1521,  2211,  1946,  1746,   176,   119,
    2188,   252,   365,   195,   441,  1954, -1549,   390,   444,  1313,
     328,   442,  1117,   359,  2019,  1284,  1917,   196,   197, -1493,
     533,    30,  1373,    94,  1053,   299,  1246,   366,  1996,    20,
     511,  2070,  2160,  1108,   488,   174,  1654,   445,   884,  2284,
      56,  2189,   359,   472,   604,    41,  1366,   253,   532,   198,
   -1487,  1767,  2074,    91,  1363,  1314,   329,   252,  1711,  1386,
    1387,  1388,  1389,    95,  1390,    96,   621, -1428,  1919,  1768,
    1514,   330,   299,  1520,    34,  1379,  1380,  1381,   446,   533,
    1865,    36,   941,   443,   838,  1392,   534,  1769,   199,   200,
      44,   201,  1558,   512, -1489,  1559,   361,   190,  1560,  1561,
     202,   302,  2095,   253,    38,  2096,   821,   535,  1428,   190,
     119,   331,   274,   822,  1435,  1437,  1438,  1440,   309,  2129,
    1443,  1445,  1190,   942,  2020,  1651,  1521,  1190,  2080,  2209,
     943,   937,   536,  1984,   839,  1770,  1477,  2135,   537,  1479,
    1480,   139,  2077,   203,   204,  1009,   444,  2090,  1866,  1046,
    1867,   541,    49,   140,   205,  1061,  1059,  2261,  2262,    50,
    1078,  1079,  1515,   515,  2099,   538,  1078,  1086,  1088,    53,
    1300,   840,   539,  1046,   141,   445, -1493,  1518,  1043,  1078,
    1596,  1967,   142,  1771,  1750,   540,  1141,   190,  1086,  1143,
    2168,  1145,  1219,  1810,  2097,  2098,  1879, -1493,   842,  1643,
    1643,   506,   944,   844,  1751,  2130,  2131,  1597,  1043,   266,
     541,  1301,   542,   412,  2133,   146,   446,  1240,   419,  1302,
     543,   841,   427,    41,  2266,  1562,  1563,   846,   332,   206,
    -215,  2086,    59,   471,  1967,   862,  1772,  1832,  1188,  2196,
     333,    60,  1186,   428,  1564,  1565,  1773,  1186,   843,  1308,
    1604,   544,  1844,   845,  2184,  1043,   863, -1549,  1985,   143,
    1640,  2082,  1309,  2084,   605,   945,   302,   302,  1213,   610,
     147,   148,   507,  1070,  1071,  1833,  2201,   847,  2202,  1666,
    1667,  1668,  1669,  1670,  1671,  1673,   457,  1072,   458,  1604,
    2248,  2249,  1303,  2282,  2120,  1834,    61,  1113,  2197, -1549,
    1570,  1613,   545,    10,  1245,  1614,  2200,    10,  1774,  1716,
      62,  2287,  1719,   508,  2121,  1150,  2250,  1724,    68, -1549,
     509,  2203,  1386,  1387,  1388,  1389,   251,  1390,   334,   335,
     251,  1078,   510,  1073,  1391,   946,  1562,  1563,    72,  1078,
    1285,   336,     5,   337,   207,    79,  1349,  1571,  1350,  2222,
    1977,  2223,  2270,  1967,    75,  1564,  1565,  1046,  2224,  1425,
    1926,    81,  1927,  1429,  1386,  1387,  1388,  1389,  1271,  1390,
    1547,  1441,  1442,  1326,  1388,  1389,  1714,  1390,  2231,  2232,
    1940,  1078,  1941,    82,   208,  1078,   322,   323,  1386,  1387,
    1388,  1389,   511,  1390,  1344,   103,  1665,  1285,  1011,  1648,
    1649,   866,  1386,  1387,  1388,  1389,  1043,  1390,  1703,  2173,
    1718,  1386,  1387,  1388,  1389,  1866,  1390,  1867,   115,  1721,
    1386,  1387,  1388,  1389,  1245,  1390,   116,  1086,  2198,   117,
    2199,  2215,  2177,  2178,   209,   119,  1188,  1188,  1188,  2233,
    2234,   125,  2273,   129,   338,   130,  1188,   131,   295,  1386,
    1387,  1388,  1389,  2195,  1390,   512,   132,  1726,   513,   514,
     134,   137,  1326,   133,  1572,   339,   136,  1188,   144,  1188,
    1643,  1188,   155,   156,  1012,  1188,  1188,  1188,  1188,  1188,
    1188,  1188,  1188,  1874,   157,  1802,  -944,   171,   172,   173,
    1014,   186,   224,   227,  2251,  2252, -1549,   228,  1872,   229,
     190,   231,  1843,  1386,  1387,  1388,  1389,  1885,  1390,   232,
     234,  1882,   237,  2263,  1573,   235,  2264,   241,  1818,   242,
     243,  1386,  1387,  1388,  1389,   515,  1390,  -944,   246,  2079,
     259,  1869,  1803,    41,  -944,  2253,  2254,   302,   273,   276,
     282,  1326, -1380, -1380, -1380, -1380,   277,  1900,  1249,   283,
     302,  1078,   292,   299,  1846,  1261,  1262,  1263,  1264,   305,
     307,  1846,   309,   315,  1594,  2281, -1379, -1379, -1379, -1379,
    1888,  1245,   314,  1889,   302,   316,  1015,   352,  1607,   353,
     355,  1400,  1401,   359,   362,  1078,   364,   440,   367,   368,
     195,  1935,  1936,  1937,  1938,  2288,   376,  1623,   393,   394,
     395,   403,   404,  1326,   196,   197,  -944,   421,  1899,  1631,
     424,   426,   433,  1903,   302,   451,   466,  1016,   469,  -365,
     434,   474,   476,  1402,  1017,  1403,  1285,   479,   441,   491,
    1018,   521,   525,  1019,   493,   442,   198,   574,   566,   576,
    1188,  1188,  1188,  1188,  1188,  1188,  1188,  1939,   579,   583,
     585,   606,   609,  1942,   607,   611,   614,   190,   624,  1404,
    1405,  1406,  1326,   686,   696,   688,  1020,   703,   704,  -944,
     810,   440,  2242,   837,   826,   199,   200,   835,   201,  1684,
     848,  1685,  1713,   850,  1686,   887,   856,   202,   888,  2180,
    -796,   931,   616,   966,  1687,  1975,   963,   969,  -944,  1804,
     980,   981,  1574,   983,  1078,   986,  1407,   443,  1408,   989,
     991,  1000,   441,   997,  1003,  1006,  1409,  2029,  1007,   442,
    1034,  1048,  1064,  1149,  1081,  1151,  1170,  1056,  1157,  1176,
     203,   204,  1178,  -944,  1021,  1191,  1194,   876,  1205,  -944,
    1212,   205,  1214,  1216,  1234,  1220,  1229,  1236,  1221,  1222,
    1981,  1247,  1223,  1224,  1225,  1239,  1987,  1253,  1226,  1227,
    1241,  1255,  1258,  1277,  2068,  1265,  1743,  1744,  2069,  1281,
     444,  1296,  1297,  1299, -1382,  1306,  1310,  1316,  1312,  1022,
    1023,  1321,  1311,   165,  1119,  1323,  1345,  1348,  1352,   909,
    1410,   443,  1354,  1355,  -887,   471, -1506,  -887,   322,   445,
    -887,  -887,  -887,  1367,  1119,  1024,  2117,  1376,  1393,  1397,
    1414,  1012,  1448,  1427,  1449,  1434,   206,  1507,  1039,  1530,
    1545,  1550,  1568,  1025,  1688,  1583,  1587,  1014,  1609,  1689,
    1601,  1026,  1326,  1326,   299,  1616,  1629,  1617,  1078,  1078,
     446,  1624,  1690,  1641,  1390,  1646,  -887,  1679,  1680,  1250,
    1703,  1683,  1715,  1717,   444,  1417,  1826,  1720,  1732,  1725,
    1734,  1737,  2118,  1727,  1326,  1521,  1755,  1778,  1780,  1786,
    -887,  1782,  1798,   302,  1794,  1820,  1823,  1797,  1824,  1847,
    1836,  1850,  1835,   445,  1856,  1856,  -887,  1805,  1837,  1851,
    1691,  1859,  1870,  1861,  1655,  1883,  1912,  1931,  1956,  1932,
    1958,  1962,  2194,  1964,  1969,  1972,  1990,  1828,  1976,  1993,
    2192,  1995,  2000,  1015,  2003,  2169,  2005,  2008,   299,  2014,
    2016,  2026,  2031,  2012,   446,  2030,  2183,  2033,  2185,  2065,
    2083,   207,  2085,  2087,  2101,  2105,  2103,  2108,  1692,  1452,
    1453,  1454,  2122,  2123,  -887,  -887,  2124,  1455,  2142,  2151,
    2144,  2020,  2170,  2163,  2171,  2175,  2181,  1411,  2182,  1245,
    1245,  1245,  1693,  2042,  2204,  1044,   302,  1018,  2206,  1188,
    1019,   208,  -887,  -887,  1188,  2208,  2220,  2229,  2214,  -887,
    2221,  2239,  2256,  -887,  2267,  1456,  1694,  1078,  2269,  2257,
    2271,  1695,  2286,    15,    28,    74,   193,   262,  1412,   436,
     272,  1696,  -887,  1020,   275,  1697,   296,  1611,   812,  2216,
    -887,   598,  1359,  -887,  -887,  2247,   520,  2236,   226,  1639,
    -887,   209,  1645,  1360,  -887,   392,  -887,  2028,  1159,  -887,
     823,  -887,   151,  1979,   153,   298,   311,  2258,  2192,   468,
      67,   245,  1004,   701,  -887,  -887,  1955,   320,   405,  1374,
    -887,   987,  1966,  1326,  2046,  2237,  2238,  2047,   866,  2241,
    -887,  1326,  -887,  1698,  2048,  2049,  2243,  -887,  1881,  1878,
    1078,  1021,  1814,  1699,  1877,  2193,  2050,  2051,  1594,  2052,
     627,  1173,   302,  1457,  1458,  2275,  2276,  2259,   999,  2278,
    -887,  2279,   302,  2265,  2187,   170,  1922,  1982,  1260,   588,
    1708,  1892,  1707,  -887,  1269,  1966,  1945,  1952,  1920,  1326,
    1552,  2268,  1459,  1548,  1789,  2107,  1022,  1023,  1460,  1567,
    1971,  1579,  1799,  1305,  2277,  2285,  1983,  1825,  1638,  1608,
    -887,  1012,  1999,  1343,  2154,  1328,   471,  1131,  1329,  1857,
    1858,  1461,  1024,  2072,  2072,  2149,  1886,  1014,  2218,  1462,
    1992,  1757,  2283,  1463,  2165,  1320,  1509,  1841,  1045,  1192,
    1025,  1451,   993,  1043,   321,   417,  -887,  1581,  1026,  1276,
     829,   299,  1195,  2260,   968,  -887,  -887,    76,  1199,     0,
       0,  1043,  1736,  1464,  1465,   471,     0,     0,     0,  1466,
       0,     0,     0,     0,  2072,  -887,     0,  2072,     0,  1467,
       0,  -887,     0,     0,     0,     0,  1468,     0,     0,     0,
       0,  1469,     0,     0,     0,     0,     0,     0,     0,  -887,
       0,     0,     0,     0,  1966,     0,     0,  -887,     0,  1470,
    -887,  -360,   628,  1015,     0,     0,  -887,     0,  -887,  -887,
       0,     0,   471,  -887,   440,  -887,     0,  -887,  -887,  -887,
       0,     0,   629,     0,     0,     0,  1086,     0,     0,     0,
       0,     0,     0,  2146,     0,   630,     0,     0,   631,   632,
     633,   634,   635,   636,   637,     0,     0,     0,     0,     0,
       0,     0,  2072,     0,     0,   441,     0,  1018,     0,     0,
    1019,     0,   442,     0,     0,     0,     0,     0,     0,  2186,
       0,     0,     0,     0,     0,   638,     0,   639,   640,   641,
     642,   643,   644,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1020,     0,     0,     0,     0,     0,     0,
       0,  2072,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   645,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   443,     0,  -360,     0,     0,     0,
    -360,     0,     0,     0,     0,  1471,     0,  1472,  1473,  1474,
    1475,     0,  1476,     0,  1477,  1478,     0,  1479,  1480,     0,
    1285,  1021,     0,     0,     0,     0,     0,     0, -1493,  -360,
       0,     0,     0,     0,     0,  2226,     0,     0,  2228,     0,
       0,     0,     0,     0,  -360,     0,     0,     0,     0, -1493,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,   646,     0,     0,     0,  1022,  1023,  2245,     0,
       0,     0,     0,     0,     0,    41,     0,     0,   647,     0,
       0,     0,     0,     0, -1530,     0,   445,     0,  -360,  -360,
       0,     0,  1024,     0,     0,     0,  -360,   648,     0,     0,
    -360,  1086,     0,     0,     0,     0,     0,     0,     0,     0,
    1025,     0,  2274,     0,   649,     0,     0,     0,  1026,     0,
       0,   299,     0,     0,     0,  1011,     0,   446,   866,  -579,
     589,     0,  -619,     0,  -619,     0,     0,   650,     0,  -619,
       0,     0,     0,     0,     0,     0,     0,  -619,     0,  1207,
       0,     0,   651,     0,     0,  1285,     0,     0,     0,   652,
       0,   653,     0,     0,     0,     0,     0,     0,  -360,     0,
       0,     0,  -360,     0,   654,     0,     0,     0,     0,     0,
    -619,  -619,     0,     0,   655, -1471,     0,     0,  -619,     0,
       0,   656,     0,  -360,     0,     0,     0,     0,     0,     0,
    -619,  1012,     0,     0,  -619,     0,     0,     0,     0,     0,
       0,     0,  -360,     0,     0,  -360,  -619,  1014,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   657,     0,
     658,   659,   660,     0,     0,     0,     0,     0,     0,     0,
    -619,     0,     0,     0,     0,     0,     0,  -619,  -619,     0,
       0,     0,     0,     0,   661,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -579,     0,     0,     0,  -579,     0,
       0,     0, -1530,     0,     0,     0,     0,     0,  -619,     0,
     662,   663,   664,     0,     0,     0,     0,     0,     0,     0,
    -619,     0,     0,   665,  -619,     0,   666,     0,     0,     0,
       0,     0,     0,  1015,     0,  -360,     0,     0,  -619,     0,
    -360,     0,  -579,     0,   440,  -619,     0,  -360,  -619,  -619,
       0,     0,     0,     0,     0,     0,  -619,     0,     0,     0,
       0,     0,     0,  -619,     0,  -619,     0,     0,  -619,     0,
       0,     0,     0,     0,  1016,     0,     0,     0,     0,     0,
       0,  1017, -1471,     0,     0,   441,     0,  1018,     0,     0,
    1019,     0,   442,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -619,     0,     0,     0,  -619,
       0,  -619,     0,     0,     0,     0,     0,     0,  -619,     0,
       0,     0,   589,  1020,  -619,     0,  -619,     0,     0,     0,
       0,  -619,     0,     0,     0,     0,     0,     0,     0,  -619,
       0,  -619,     0,     0,     0,     0,     0,     0,     0,     0,
   -1471,     0,     0,     0,     0,     0,  -619,     0,     0,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
    -579,     0,  -619,  -619,     0,     0,     0,  -619,     0,     0,
    -619,     0,     0,     0,     0,     0,  -619,     0,     0,     0,
       0,  1021,  -619,     0,     0,     0,  -619,     0,     0,  -619,
       0,     0,     0,     0,  -619,     0,  -619,     0,  -619,     0,
       0,     0,     0,     0,  -619,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -619,     0,     0,   444,     0,     0,
       0,     0,  -619,  -619,     0,     0,  1022,  1023,     0,  -619,
    -619,     0,  -619,     0,  -619,  -619,     0,   702,  -619,     0,
    -619,     0,     0,     0,     0,  -619,   445,     0,     0,     0,
       0,     0,  1024,     0,     0,     0,     0,     0,     0,     0,
    -619,  -619,     0,     0,     0,     0,  -619,     0,     0,     0,
    1025,  -619,  -619,     0,     0,     0,  -619,     0,  1026,     0,
       0,   299,  -619,     0,     0,     0,     0,   446,     0,     0,
    -619,   590,     0,     0,  -619,     0,     0,  -619,  -619,     0,
    -619,  -619,     0,     0,     0,  -619,     0,     0,  -619,     0,
       0,     0,     0,     0,     0,  -619,     0,  -619,     0,  1012,
    -619,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0, -1463,     0,     0, -1463,  1014,     0, -1463, -1463, -1463,
       0,     0,  1270,     0,     0,     0, -1463,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -619,     0,     0,
       0,  -619,     0,  -619,     0,     0,     0,     0,     0,     0,
    -619,     0,     0,     0,     0,     0,   631,   632,   633,   634,
     635,   636,     0, -1463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -619,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0, -1463,  -619,     0,
       0,     0,     0,   638,     0,   639,   640,   641,   642,   643,
     644,  1015,     0, -1463,     0,     0,     0,  1011,     0,  -619,
     866,     0,   440,  1486,  1487,  1488,     0,     0,  -619,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -619,     0,     0,     0,     0,  -619,   645,  -619,     0,
       0,     0,  1180,     0,     0,     0,  -619,     0,     0,  1017,
       0,     0,     0,   441,     0,  1018,  -619,     0,  1019,  1489,
     442, -1463, -1463,     0,     0,  -619,     0,     0,     0,     0,
       0,     0,     0,     0,  -619,     0,  -619,  -619,     0,     0,
    -619,     0,  -619,  1012,     0,     0,     0,  -619,     0, -1463,
   -1463,  1020,     0,     0,     0,     0, -1463,     0,     0,  1014,
   -1463,     0,     0,  -619,     0,     0,     0,     0,  -619,     0,
       0,     0,     0,  -619,     0,     0,     0,     0,     0, -1463,
     646,     0,     0,     0,  -619,     0,     0, -1463,     0,     0,
   -1463, -1463,   443,   590,     0,     0,  -619, -1463,     0,     0,
    -619, -1463,     0, -1463,     0,     0, -1463,  -619, -1463,     0,
       0,     0,     0,     0,     0,     0,     0,  1457,  1458,  1021,
       0, -1463, -1463,     0,     0,     0,     0, -1463,     0,     0,
       0,     0,   649,     0,     0,     0,     0, -1463,     0, -1463,
       0,     0,     0,     0, -1463,  1015,  1459,     0,     0,     0,
       0,     0,  1460,     0,     0,   444,   440,     0,     0,     0,
       0,     0,     0,     0,  1022,  1023,     0, -1463,     0,     0,
     651,     0,     0,     0,     0,  1490,     0,     0,     0,   653,
   -1463,     0,     0,  1462,   445,     0,  1016,  1463,     0,     0,
    1024,     0,   654,  1017,     0,     0,     0,   441,     0,  1018,
       0,     0,  1019,     0,   442,     0,     0, -1463,  1025,     0,
       0,     0,     0,     0,     0,     0,  1026,  1491,  1492,   299,
       0,     0,     0,  1493,     0,   446,     0,  1181,  1182,     0,
       0,     0,     0,  1494,     0,  1020,  1183,     0,     0,     0,
    1495,     0,     0, -1463,     0,     0,     0,     0,   658,   659,
     660,     0, -1463, -1463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1496,     0,     0,     0,     0,     0, -1463,
       0,     0, -1463, -1463,     0,     0,   443,     0, -1463,   705,
       0,   706,     0,     0,     0,     0,   707,     0,     0,     0,
       0,     0,     0,     0,   708,     0, -1463,     0,   662,   663,
     664,     0,     0,  1021, -1463,     0,     0, -1463,     0,     0,
       0,     0,     0, -1463,     0, -1463, -1463,     0,     0,     0,
   -1463,     0, -1463,     0, -1463, -1463, -1463,   709,   710,     0,
       0,     0,     0,     0,     0,   711,     0,     0,     0,   444,
       0,     0,     0,     0,     0,     0,     0,   712,  1022,  1023,
       0,   713,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   714,     0,     0,     0,     0,   445,     0,
       0,     0,     0,     0,  1024,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   715,     0,     0,
       0,  1012,  1025,     0,   716,   717,     0,     0,     0,     0,
    1026,     0,     0,   299,     0,     0,     0,  1014,     0,   446,
       0,  1497,  1498,     0,     0,     0,  1499,     0,  1477,     0,
    1500,  1479,  1480,     0,     0,   718,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   719,     0,     0,
       0,   720,     0,     0,     0,     0,     0,  -752,     0,     0,
    -752,     0,   705,     0,   706,   721,     0,     0,     0,   707,
       0,     0,   722,     0,     0,   723,   724,   708,     0,     0,
       0,     0,     0,   725,     0,     0,     0,     0,     0,     0,
     726,     0,   727,     0,     0,   728,     0,     0,     0,     0,
     165,     0,     0,  1015,     0,     0,     0,     0,     0,     0,
     709,   710,     0,     0,   440,     0,     0,     0,   711,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     712,     0,   729,  -752,   713,     0,   730, -1475,   731,     0,
       0,     0,     0,     0,     0,   732,   714,     0,     0,  -752,
       0,     0,     0,     0,     0,   441,     0,  1018,     0,     0,
    1019,     0,   442,     0,     0,     0,  1011,     0,   733,   866,
     715,     0,     0,     0,     0,     0,     0,   716,   717,     0,
       0,     0,     0,   734,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1020,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   735,     0,     0,     0,   718,     0,
       0,     0,     0,   736,     0,     0,     0,     0,     0,     0,
     719,     0,     0,     0,   720,     0,   737,     0,     0,     0,
       0,   738,     0,   739,   443,  -752,     0,     0,   721,     0,
       0,   740,  1012,     0,     0,   722,  -752,     0,   723,   724,
       0,   741,     0,     0,  1013,     0,   725,     0,  1014,     0,
     742,  1021,     0,   726,     0,   727,     0,     0,   728,   743,
       0,   744,   745,     0,     0,   746,  -752,   747,     0,     0,
       0,     0,   748,  -752,     0,     0,     0,  -752,  1011,  -752,
       0,   866,  -752,     0,  -752,     0,     0,   444,   749,     0,
       0,     0,     0,   750,     0,   729,  1022,  1023,   751,   730,
       0,   731,     0,     0,     0,     0,     0,     0,   732,   752,
       0,     0,     0,     0,     0,  -752,   445,     0,     0,     0,
    -752,   753,  1024,     0,     0,   754,     0,     0,     0,     0,
       0,   733,   755,     0,  1015,     0,     0,     0,     0,     0,
    1025,     0,     0,     0,     0,   440,   734,     0,  1026,     0,
       0,   299,     0,     0,  1012,     0,  -752,   446,     0,     0,
       0,     0,     0,     0,     0,     0, -1475,   735,     0,     0,
    1014,     0,     0,     0,     0,  1016,   736,     0,     0,     0,
       0,  1011,  1017,  -752,   866,     0,   441,     0,  1018,   737,
       0,  1019,     0,   442,   738,     0,   739,     0,     0,     0,
       0,     0,     0,     0,   740,     0,     0,     0,     0,     0,
       0,     0,     0,  -752,   741,     0,     0,     0,     0,  -752,
       0,  1612,     0,   742,  1020,     0,     0,     0,  -752,  -752,
       0,     0,   743,     0,   744,   745,     0,     0,   746,     0,
     747,     0,     0,     0,     0,   748,     0,     0,  -752,     0,
       0,     0,     0,     0,  -752,     0,  1015,  1012,     0,  -752,
       0,   749,     0,     0,     0,   443,   750,   440,     0,     0,
       0,   751,  -752,  1014,     0,     0, -1475,     0,     0,     0,
    -752,     0,   752,  -752,     0,     0,     0,  1135,     0,  -752,
       0,     0,  1021,     0,   753,     0,     0,  1016,   754,     0,
       0,     0,     0,  1011,  1017,     0,   866,     0,   441,     0,
    1018,     0,     0,  1019,     0,   442,     0,  1011,     0,     0,
     866,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,     0,     0,  1022,  1023,     0,
       0,     0,     0,     0,     0,     0,  1020,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   445,     0,  1015,
       0,     0,     0,  1024,     0,     0,     0,     0,     0,     0,
     440,     0,     0,     0,     0,     0,     0,     0,     0,  1012,
       0,  1025,     0,     0,     0,  1342,     0,   443,     0,  1026,
       0,  1093,   299,  1012,     0,  1014,     0,     0,   446,     0,
    1016,     0,     0,     0,     0,     0,     0,  1017,     0,  1014,
       0,   441,  1011,  1018,  1021,   866,  1019,     0,   442,     0,
       0,     0,     0,  1135,     0,     0,  1011,     0,     0,   866,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1020,
     444,     0,     0,     0,     0,     0,     0,     0,     0,  1022,
    1023,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1613,     0,     0,   445,
    1614,  1015,     0,     0,     0,  1024,     0,     0,  1012,     0,
     443,     0,   440,     0,     0,  1015,     0,     0,     0,     0,
    1139,     0,  1012,  1025,  1014,     0,   440,     0,     0,     0,
       0,  1026,     0,     0,   299,     0,     0,  1021,  1014,     0,
     446,     0,  1016,     0,     0,     0,     0,     0,     0,  1017,
       0,     0,     0,   441,     0,  1018,  1016,     0,  1019,     0,
     442,  1012,     0,  1017,     0,     0,     0,   441,  1011,  1018,
       0,   866,  1019,   444,   442,     0,     0,  1014,     0,     0,
       0,     0,  1022,  1023,     0,     0,     0,     0,     0,     0,
       0,  1020,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   445,     0,     0,  1020,     0,     0,  1024,     0,
    1015,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,     0,     0,  1015,     0,  1025,     0,     0,     0,
       0,     0,   443,     0,  1026,   440,     0,   299,     0,     0,
       0,     0,     0,   446,  1012,     0,   443,     0,     0,     0,
       0,  1016,     0,     0,     0,     0,  1099,     0,  1017,  1021,
    1014,     0,   441,  1015,  1018,  1016,     0,  1019,     0,   442,
       0,  1011,  1017,  1021,   866,     0,   441,     0,  1018,     0,
       0,  1019,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   444,     0,     0,     0,     0,
    1020,     0,     0,     0,  1022,  1023,     0,     0,     0,   444,
       0,     0,     0,     0,  1020,  1100,     0,  1018,  1022,  1023,
    1019,     0,     0,     0,   445,     0,     0,     0,     0,     0,
    1024,     0,     0,     0,     0,     0,     0,     0,   445,     0,
       0,   443,  1347,     0,  1024,     0,  1015,  1012,  1025,     0,
       0,     0,     0,  1020,     0,   443,  1026,   440,     0,   299,
       0,     0,  1025,  1014,     0,   446,     0,     0,  1021,     0,
    1026,     0,     0,   299,     0,     0,     0,     0,     0,   446,
       0,     0,  1021,  1011,     0,     0,   866,  1016,     0,     0,
       0,     0,     0,     0,  1017,     0,     0,     0,   441,     0,
    1018,     0,     0,  1019,   444,   442,     0,     0,     0,     0,
       0,     0,     0,  1022,  1023,     0,     0,     0,   444,     0,
       0,  1021,     0,     0,     0,     0,     0,  1022,  1023,     0,
       0,     0,     0,   445,     0,     0,  1020,     0,     0,  1024,
       0,     0,     0,  1231,     0,     0,     0,   445,     0,  1015,
       0,     0,     0,  1024,     0,     0,     0,  1025,     0,  1012,
     440,     0,     0,     0,     0,  1026,  1022,  1023,   299,     0,
       0,  1025,     0,     0,   446,  1014, -1549,   443,     0,  1026,
       0,     0,   299,     0,     0,     0,  1011,     0,   446,   866,
    1016,     0,  1024,     0,     0,     0,     0,  1017,     0,     0,
       0,   441,  -995,  1018,  1021,     0,  1019,     0,   442,     0,
    1025,     0,     0,     0,     0,  -995,     0,     0,  1026,     0,
     190,   299,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1020,
     444,     0,     0,     0,     0,     0,     0,     0,     0,  1022,
    1023,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1015,  1012,     0,     0,     0,     0,     0,  1011,   445,
       0,   866,   440,     0,     0,  1024,     0,     0,  1014,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1025,     0,     0,     0,     0,     0,     0,
       0,  1026,  1016,     0,   299,     0,     0,  1021,     0,  1017,
     446,     0,     0,   441,     0,  1018,     0,     0,  1019,     0,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1621,     0,     0,
       0,     0,     0,   444,  1012,     0,     0,     0,     0,     0,
       0,  1020,  1022,  1023,     0,     0,     0,     0,     0,     0,
    1014,     0,     0,     0,  1015,     0,     0,     0,     0,     0,
       0,     0,   445,     0,     0,   440,     0,     0,  1024,     0,
       0,     0,     0,  1012,     0,   631,   632,   633,   634,   635,
     636,   637,   443,     0,     0,     0,  1025,     0,     0,  1014,
       0,     0,     0,     0,  1026,  1016,     0,   299,     0,     0,
       0,     0,  1017,   446,  2034,  2035,   441,     0,  1018,  1021,
       0,  1019,   638,   442,   639,   640,   641,   642,   643,   644,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1015,     0,     0,     0,
    1727,     0,     0,     0,  1020,   444,     0,   440,     0,     0,
       0,     0,     0,     0,  1022,  1023,   645,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,  1015,     0,  1016,     0,     0,
    1024,     0,     0,     0,  1017,   443,   440,     0,  1057,     0,
    1018,     0,     0,  1019,     0,   442,     0,     0,  1025,     0,
       0,     0,     0,     0,     0,     0,  1026,     0,     0,   299,
       0,     0,  1021,     0,     0,   446,  1180,     0,     0,     0,
       0,  1012,     0,  1017,     0,     0,  1020,   441,     0,  1018,
    2036,     0,  1019,     0,   442,     0,     0,  1014,     0,   646,
       0,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,   647,     0,  1022,  1023,     0,
       0,     0,     0,     0,     0,  1020,     0,   443,     0,     0,
       0,  2037,  2038,     0,     0,     0,     0,   445,     0,     0,
       0,     0,     0,  1024,     0,     0,     0,     0,     0,     0,
       0,   649,     0,     0,  1021,     0,  2039,  1012,     0,     0,
       0,  1025,     0,     0,     0,     0,   443,     0,     0,  1026,
       0,     0,   299,  1014,   650,     0,     0,     0,   446,     0,
       0,     0,     0,  1015,     0,     0,     0,     0,     0,   651,
     444,     0,     0,  1021,   440,     0,   652,     0,   653,  1022,
    1023,  2040,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   654,     0,     0,     0,     0,     0,     0,     0,   445,
       0,     0,     0,     0,  1180,  1024,     0,     0,     0,   444,
       0,  1017,     0,     0,     0,   441,     0,  1018,  1022,  1023,
    1019,     0,   442,  1025,     0,     0,     0,     0,     0,     0,
       0,  1026,  1099,     0,   299,     0,     0,     0,   445,  1015,
     446,     0,     0,     0,  1024,   657,     0,   658,   659,   660,
       0,     0,     0,  1020,  2041,     0,     0,     0,     0,     0,
       0,     0,  1025,     0,     0,     0,     0,     0,  2042,     0,
    1026,     0,     0,   299,     0,     0,     0,     0,     0,   446,
       0,  1181,  1182,     0,     0,     0,     0,     0,     0,  1672,
    1183,  1044,  2043,  1018,   443,     0,  1019,   662,   663,   664,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     665,     0,     0,   666,  2044,     0,     0,     0,     0,     0,
       0,  1021,     0,     0,     0,     0,     0,     0,     0,  1020,
       0,     0,     0,     0,  2045,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,     0,     0,     0,  1022,  1023,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,     0,     0,     0,
       0,     0,  1024,     0,     0,     0,     0,  1021,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1025,     0,     0,     0,     0,     0,     0,     0,  1026,     0,
       0,   299,     0,     0,     0,     0,     0,   446,     0,  1181,
    1182,     0,     0,     0,     0,     0,     0,  1898,  1183,     0,
       0,     0,  1022,  1023,     0,     0,     0,     0,     0,     0,
       0,     0, -1549,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1024,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -995,     0,
       0,     0,     0,     0,     0,     0,  1025,     0,     0,     0,
       0,  -995,     0,     0,  1026,     0,   190,   299
};

static const yytype_int16 yycheck[] =
{
      84,   176,   231,   132,   230,   234,   415,   574,   723,   933,
    1056,   687,   897,   689,   538,   539,   463,   326,  1308,   867,
     868,   196,   197,  1128,  1135,   892,   201,   202,   203,   204,
     205,  1238,   207,   752,   209,  1238,   903,  1238,  1238,  1238,
    1271,   655,   372,  1076,   490,  1262,  1263,  1264,   260,  1713,
       6,  1084,   983,    13,  1026,    70,   285,   694,  1231,  1149,
     289,     6,   291,  1152,   193,    22,   228,     9,   230,   231,
       6,   300,   234,  1049,   303,   250,   160,   252,    49,  1417,
    1418,    54,   606,   911,   259,   893,     9,  1118,     6,    17,
      30,     6,   267,    23,   824,  1268,   924,  1130,    49,    86,
      17,    85,  1312,  1208,    70,    70,   281,   282,    48,   284,
     118,    63,    72,   114,    48,   290,     9,   447,    56,   149,
       9,    28,   170,   285,    56,    66,  1299,   289,    88,   291,
     182,   207,   171,   121,   363,     9,    56,   171,   300,   351,
     891,   303,     0,   121,   373,  1408,   358,  1862,   123,   900,
     901,   380,  1415,   227,   228,   330,  1112,    96,   333,   334,
     245,   336,  1202,  1011,   915,   916,   236,   393,  1016,   395,
       9,   236,   139,    70,  1347,   187,  1207,   352,   929,   930,
     236,   214,    56,  1847,   248,   861,    37,  1118,    70,   418,
      73,   521,    70,  1169,   369,   180,   118,   873,    22,    21,
     412,   363,    40,    49,  1294,   224,   121,   419,   115,    49,
      96,   373,   402,    28,   175,   122,    49,   279,   380,   240,
     432,   213,    49,   236,   308,   310,   171,  1075,   236,   391,
     214,   393,   150,   395,   174,   236,   465,   913,   467,   414,
     466,  1352,   894,   223,   456,   118,  1850,   422,  1172,   695,
    1181,  1182,  1183,    70,   416,  1286,   418,   469,   107,    86,
    1191,   913,   113,   200,   404,   236,   918,     0,   408,   459,
     150,  1001,  1002,   358,   266,  1448,  1207,   262,  1029,  1368,
     310,  1212,     8,  1214,   214,  1216,  1376,  1218,     9,  1220,
    1221,  1222,  1223,  1224,  1225,  1226,  1227,  1330,  1126,   173,
     115,   476,   195,   465,   466,   467,   195,  1397,  1059,    35,
     522,  1528,   396,   236,   236,   399,     9,  1348,   380,  1550,
     482,   496,   497,   122,  1000,   183,   347,   346,   540,    50,
     248,   983,  1180,   456,   236,   564,   348,    23,   325,   403,
    1316,   316,  1093,   236,   311,   360,  1231,   236,   285,   223,
     243,   526,    61,    63,   243,  1286,  1396,   117,   434,  1315,
     301,   536,   248,   236,   576,  1969,  1003,   218,   329,   581,
     210,   456,   442,   280,   325,   449,  1407,   442,   122,    88,
      89,   995,   996,  1268,  1251,  1136,   442,   202,   563,   364,
    1141,  1401,   448,   168,   442,   360,   571,   236,  1408,  1510,
      92,   321,   564,   442,   456,  1415,    32,   974,   442,  1276,
    2125,   363,   442,   542,  1299,  1519,  1449,  1348,  1681,   305,
     258,  1526,   315,   456,   586,   587,   315,   251,  1638,   442,
     605,   411,   456,  1537,   442,   610,   424,   612,   650,   322,
     615,   442,   617,   458,   619,   620,   424,  1378,   435,   624,
    1102,   983,   436,   311,  1385,  1386,  1387,  1388,  1389,  1390,
    1391,   434,  1347,   360,  1554,   403,   126,   403,  1296,   409,
     436,   442,  1280,  1203,   195,   409,  1407,   448,   360,   325,
    1231,  1585,   360,   400,   435,   378,   442,  1238,   403,   378,
     665,   666,   325,   458,   656,   341,   441,   442,   325,   456,
     442,   441,  1310,   442,   225,   214,   442,   187,   341,   424,
     438,   443,   442,   364,   407,   236,  1267,  1268,   407,   442,
     442,  1625,   243,   406,   442,   198,   350,   442,   214,  1181,
    1182,  1183,    70,   206,   441,   245,   866,   411,   164,  1191,
     442,   501,  1588,   360,  1655,   305,  1577,  1837,  1299,  1357,
    1639,  1584,   225,   442,  1785,   448,   442,   245,  1004,   448,
    1212,   458,  1214,  1448,  1216,   694,  1218,   260,  1220,  1221,
    1222,  1223,  1224,  1225,  1226,  1227,   458,    66,  1609,   354,
     458,  2245,   404,  1334,   293,   139,   408,  1338,  1339,   435,
    1341,   820,   441,   442,   315,   824,  1347,  1211,   827,   828,
     310,   236,   435,   263,  1218,   834,   279,   280,   435,  1782,
     456,  1784,   223,    30,   287,   288,    30,  1808,   244,    70,
     293,   294,   310,   456,   251,   318,  1730,   187,  1739,   120,
     405,   187,   376,   450,   451,   452,   453,   123,   455,   331,
     815,   458,   441,   442,  1817,   168,  1577,  1578,   358,  1181,
    1182,  1183,    66,   363,   227,   228,   214,   378,   820,  1191,
     218,   836,   824,   166,  1278,   827,   828,   997,   348,   187,
     358,  1681,   834,   236,   886,   269,  1849,   300,  1609,  1394,
    1212,   167,  1214,  1320,  1216,   240,   407,  1324,  1220,  1221,
    1222,  1223,  1224,  1225,  1226,  1227,  2034,   441,   442,  1732,
    1508,  2014,   394,  2016,   207,   260,   187,   159,  1551,   126,
     383,   163,   215,   457,  1557,   319,   269,   321,  1935,  1936,
    1937,  1938,   300,   350,   953,  1113,  1378,   448,  1704,  1705,
    1706,   893,   442,  1385,  1386,  1387,  1388,  1389,  1390,  1391,
    1712,   970,  1457,   183,   233,  1717,  1718,   456,    31,  1721,
    1722,  1723,   914,  1725,  1726,  1583,   918,   311,   920,   236,
     922,   923,  1150,   925,  1378,   195,   180,  1798,   456,   981,
    1860,   248,  1001,  1002,  1965,   156,   157,  1968,   940,  1890,
    1891,   341,  1815,  1714,   425,   288,   349,     1,  1719,   918,
       4,   953,   348,  1897,   126,  2002,   971,   325,  1517,  2002,
     411,  2002,  2002,  2002,   185,    19,   236,   442,   970,  1423,
     191,   163,   301,   243,   898,    29,   293,   983,   441,   233,
     198,   240,   360,  1854,   986,   245,   245,   110,   156,   456,
     348,   354,  1943,  1944,   187,   919,    27,   921,  2151,  1001,
    1002,   222,   926,  1558,  1948,   226,  1950,   441,   240,    63,
     195,   935,  1567,  1385,  1386,  1387,  1388,  1389,  1390,  1391,
     341,   169,   240,   441,   442,  1616,    33,  1798,   260,    36,
     200,   311,    25,   426,  1978,  1806,   449,    44,    45,    46,
     156,   213,   260,   456,   442,   315,   441,   301,   441,    28,
     310,   236,  2182,   236,   313,   456,   322,  1782,   243,  1784,
     208,   275,   236,   195,   279,  2009,  2010,   435,   216,   360,
    1753,  1754,   450,   451,   452,   453,   459,   455,   236,  1786,
     458,  2112,  2027,  1854,    12,   566,   240,    15,    16,   202,
     603,   263,  1817,    11,   266,   288,  1779,   289,   358,   442,
     258,   372,   211,    11,   236,  1788,   260,   277,   378,   258,
       1,   243,  1114,   236,   154,  1084,   284,    25,    26,   188,
     233,   102,  1174,   337,  1849,   442,  1050,   236,  1835,   312,
     315,   448,  2018,  1102,  1203,   128,   115,   407,   180,  2269,
     406,   299,   236,   402,   527,   214,  1161,   418,   202,    57,
     107,   158,  1889,    71,  1156,   304,    47,   372,  1612,   450,
     451,   452,   453,   377,   455,   379,   260,   458,   284,   176,
     324,    62,   442,   213,   456,  1181,  1182,  1183,   448,   233,
     251,    83,   195,   315,   368,  1191,   240,   194,    96,    97,
     356,    99,     9,   378,   236,    12,   456,   441,    15,    16,
     108,  1203,  1939,   418,   120,  1942,   319,   261,  1214,   441,
     442,   102,   456,   326,  1220,  1221,  1222,  1223,   175,   437,
    1226,  1227,  1714,   236,   264,  1374,   266,  1719,  1911,  2115,
     243,   188,   286,  1824,   418,   242,   457,  2001,   292,   460,
     461,    60,  1891,   151,   152,   890,   378,  1930,   319,   894,
     321,   364,   456,    72,   162,   900,   900,  2201,  2202,   307,
     905,   906,   416,   448,  1947,   319,   911,   912,   913,   456,
     166,   368,   326,   918,    93,   407,   167,  1246,  1280,   924,
       8,  1797,   101,   290,   240,   339,   930,   441,   933,   934,
    2027,   936,  1017,  1580,  1943,  1944,  1660,   188,   368,  1368,
    1369,   124,   315,   368,   260,  1988,  1989,    35,  1310,   363,
     364,   207,   366,   329,  1997,   319,   448,  1042,   334,   215,
     374,   418,   167,   214,  2210,   253,   254,   368,   219,   237,
     384,  1922,   456,  1257,  1850,   167,   343,  1603,   983,  2076,
     231,   456,  1714,   188,   272,   273,   353,  1719,   418,   423,
    1319,   405,  1618,   418,  2042,  1357,   188,   198,   199,   178,
    1362,  1916,   436,  1918,   529,   378,  1368,  1369,  1013,   534,
     374,   375,   195,   296,   297,   240,  2101,   418,  2103,  1385,
    1386,  1387,  1388,  1389,  1390,  1391,   278,   310,   280,  1358,
      90,    91,   288,  2265,   240,   260,   356,   922,  2081,   240,
       9,   404,   456,     2,  1049,   408,  2089,     6,   415,  1435,
     456,  2283,  1438,   236,   260,   940,   116,  1443,   120,   260,
     243,  2104,   450,   451,   452,   453,   166,   455,   319,   320,
     170,  1076,   255,   356,   462,   448,   253,   254,   135,  1084,
    1085,   332,   311,   334,   352,   456,   319,    56,   321,  2132,
    1814,  2134,  2216,  1969,   310,   272,   273,  1102,  2141,  1212,
     265,   356,   267,  1216,   450,   451,   452,   453,   438,   455,
     440,  1224,  1225,  1118,   452,   453,   462,   455,  2161,  2162,
     153,  1126,   155,   444,   392,  1130,   227,   228,   450,   451,
     452,   453,   315,   455,  1139,   169,   458,  1142,     6,   233,
     234,     9,   450,   451,   452,   453,  1508,   455,   270,   271,
     458,   450,   451,   452,   453,   319,   455,   321,   456,   458,
     450,   451,   452,   453,  1169,   455,   356,  1172,  2083,   239,
    2085,  2122,   270,   271,   442,   442,  1181,  1182,  1183,   233,
     234,   341,  2225,   456,   435,   456,  1191,   456,   456,   450,
     451,   452,   453,  2069,   455,   378,   456,   458,   381,   382,
     359,   216,  1207,   456,   173,   456,   456,  1212,   405,  1214,
    1639,  1216,   456,   356,    82,  1220,  1221,  1222,  1223,  1224,
    1225,  1226,  1227,  1652,   235,     9,   195,   442,   456,   456,
      98,   182,   283,   456,   294,   295,   437,   280,  1650,   280,
     441,   403,  1617,   450,   451,   452,   453,  1676,   455,   277,
     403,   458,    24,  2204,   223,   395,  2207,   456,  1587,   356,
     332,   450,   451,   452,   453,   448,   455,   236,   449,   458,
     372,  1646,    56,   214,   243,   335,   336,  1639,   456,   107,
     384,  1286,   427,   428,   429,   430,   442,  1716,   156,   367,
    1652,  1296,    56,   442,  1623,   427,   428,   429,   430,   236,
      85,  1630,   175,   356,  1309,  2256,   427,   428,   429,   430,
    1685,  1316,   456,  1688,  1676,   351,   184,   372,  1323,   283,
     109,    25,    26,   236,   363,  1330,   211,   195,   456,   249,
      11,  1758,  1759,  1760,  1761,  2286,    68,  1342,   280,   410,
     280,   456,   356,  1348,    25,    26,   315,   249,  1714,  1354,
      23,   277,   442,  1719,  1716,   236,   280,   225,   301,    84,
     456,   456,    84,    67,   232,    69,  1371,   442,   236,   436,
     238,   400,   384,   241,   442,   243,    57,   435,   317,   403,
    1385,  1386,  1387,  1388,  1389,  1390,  1391,  1762,   234,   200,
     442,   319,   364,  1768,   403,   218,    55,   441,   384,   103,
     104,   105,  1407,    27,   372,   188,   274,   456,   356,   378,
     456,   195,  2179,   225,   364,    96,    97,   442,    99,    31,
     438,    33,  1427,   346,    36,   109,   214,   108,   236,  2038,
     405,   306,    37,   287,    46,  1810,   381,   281,   407,   223,
     448,   403,   411,   459,  1449,   400,   150,   315,   152,    27,
     362,   400,   236,   463,   174,   456,   160,  1869,   456,   243,
     442,     9,   236,   107,   136,    17,   172,   405,   441,   188,
     151,   152,   402,   442,   342,   459,   236,    30,   127,   448,
     459,   162,   459,   459,   205,   459,   128,    56,   459,   459,
    1819,   130,   459,   459,   459,   403,  1825,   348,   459,   459,
     434,   319,   132,   100,  1879,   133,  1511,  1512,  1883,   399,
     378,   403,   137,    49,   402,   138,   400,   180,   397,   387,
     388,   143,   402,    49,   248,   436,   146,   180,   112,     1,
     234,   315,   333,   380,     6,  1819,   442,     9,   227,   407,
      12,    13,    14,   122,   248,   413,  1975,   114,   365,   114,
     441,    82,   187,   403,   341,   442,   237,   309,   236,   163,
     134,   438,   171,   431,   176,   403,   366,    98,   403,   181,
     210,   439,  1577,  1578,   442,    49,   210,   218,  1583,  1584,
     448,   180,   194,   280,   455,   193,    58,   447,   446,   457,
     270,   218,   458,   458,   378,   233,  1601,   458,   341,   458,
     398,   403,  1977,   373,  1609,   266,   131,   269,   265,   400,
      82,   187,    49,  1975,   331,   138,     8,   403,   180,  1624,
     126,   403,   438,   407,  1629,  1630,    98,   411,   438,     9,
     242,   307,   173,   308,   202,   404,   268,   329,   125,   344,
     435,   110,  2068,   435,    49,   175,   141,   263,   285,   142,
    2062,   240,   262,   184,   144,  2030,   289,   300,   442,   113,
     394,     7,   220,   284,   448,   116,  2041,    67,  2043,   442,
     156,   352,   156,   129,   328,   136,   328,   102,   290,    12,
      13,    14,    49,   240,   156,   157,   218,    20,   145,    95,
     267,   264,    91,   148,   221,   203,   192,   401,   438,  1704,
    1705,  1706,   314,   390,    49,   236,  2068,   238,   406,  1714,
     241,   392,   184,   185,  1719,   307,   240,   147,   344,   191,
     240,   174,   180,   195,   307,    58,   338,  1732,   423,   293,
     140,   343,    49,     6,    22,    54,   132,   178,   442,   357,
     188,   353,   214,   274,   193,   357,   211,  1331,   600,  2124,
     222,   517,  1151,   225,   226,  2184,   447,  2169,   136,  1360,
     232,   442,  1369,  1152,   236,  2194,   238,  1865,   959,   241,
     613,   243,   109,  1818,   113,   227,   239,  2189,  2190,   396,
      49,   160,   885,   587,   256,   257,  1791,   247,   321,  1175,
     262,   860,  1797,  1798,  1873,  2170,  2171,  1873,     9,  2174,
     272,  1806,   274,   415,  1873,  1873,  2181,   279,  1661,  1659,
    1815,   342,  1582,   425,  1657,  2066,  1873,  1873,  1823,  1873,
     568,   973,  2184,   156,   157,  2237,  2238,  2190,   873,  2241,
     302,  2243,  2194,  2208,  2057,   124,  1741,  1822,  1059,   499,
    1420,  1700,  1419,   315,  1064,  1850,  1775,  1785,  1739,  1854,
    1276,  2213,   185,  1273,  1550,  1959,   387,   388,   191,  1283,
    1806,  1289,  1575,  1102,  2239,  2277,  1823,  1599,  1358,  1324,
     342,    82,  1839,  1136,  2017,  1124,  2160,   925,  1126,  1630,
    1630,   214,   413,  1888,  1889,  2012,  1678,    98,  2125,   222,
    1830,  1524,  2267,   226,  2023,  1114,  1240,  1612,   894,   984,
     431,  1236,   865,  2265,   247,   332,   378,  1294,   439,  1067,
     622,   442,   991,  2196,   831,   387,   388,    57,   998,    -1,
      -1,  2283,  1463,   256,   257,  2209,    -1,    -1,    -1,   262,
      -1,    -1,    -1,    -1,  1939,   407,    -1,  1942,    -1,   272,
      -1,   413,    -1,    -1,    -1,    -1,   279,    -1,    -1,    -1,
      -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   431,
      -1,    -1,    -1,    -1,  1969,    -1,    -1,   439,    -1,   302,
     442,     0,     1,   184,    -1,    -1,   448,    -1,   450,   451,
      -1,    -1,  2266,   455,   195,   457,    -1,   459,   460,   461,
      -1,    -1,    21,    -1,    -1,    -1,  2001,    -1,    -1,    -1,
      -1,    -1,    -1,  2008,    -1,    34,    -1,    -1,    37,    38,
      39,    40,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  2027,    -1,    -1,   236,    -1,   238,    -1,    -1,
     241,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,  2044,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    76,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2076,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   315,    -1,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,   448,    -1,   450,   451,   452,
     453,    -1,   455,    -1,   457,   458,    -1,   460,   461,    -1,
    2135,   342,    -1,    -1,    -1,    -1,    -1,    -1,   167,   168,
      -1,    -1,    -1,    -1,    -1,  2150,    -1,    -1,  2153,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,   188,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   378,    -1,    -1,
      -1,    -1,   201,    -1,    -1,    -1,   387,   388,  2183,    -1,
      -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,   223,    -1,   407,    -1,   227,   228,
      -1,    -1,   413,    -1,    -1,    -1,   235,   236,    -1,    -1,
     239,  2216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     431,    -1,  2227,    -1,   253,    -1,    -1,    -1,   439,    -1,
      -1,   442,    -1,    -1,    -1,     6,    -1,   448,     9,     0,
       1,    -1,     3,    -1,     5,    -1,    -1,   276,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    30,
      -1,    -1,   291,    -1,    -1,  2270,    -1,    -1,    -1,   298,
      -1,   300,    -1,    -1,    -1,    -1,    -1,    -1,   307,    -1,
      -1,    -1,   311,    -1,   313,    -1,    -1,    -1,    -1,    -1,
      51,    52,    -1,    -1,   323,    66,    -1,    -1,    59,    -1,
      -1,   330,    -1,   332,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    82,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   351,    -1,    -1,   354,    87,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   367,    -1,
     369,   370,   371,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,   393,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,   411,    -1,    -1,    -1,    -1,    -1,   149,    -1,
     419,   420,   421,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,   432,   165,    -1,   435,    -1,    -1,    -1,
      -1,    -1,    -1,   184,    -1,   444,    -1,    -1,   179,    -1,
     449,    -1,   183,    -1,   195,   186,    -1,   456,   189,   190,
      -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,
      -1,    -1,    -1,   204,    -1,   206,    -1,    -1,   209,    -1,
      -1,    -1,    -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,
      -1,   232,   233,    -1,    -1,   236,    -1,   238,    -1,    -1,
     241,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   246,    -1,    -1,    -1,   250,
      -1,   252,    -1,    -1,    -1,    -1,    -1,    -1,   259,    -1,
      -1,    -1,     1,   274,     3,    -1,     5,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,   282,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     301,    -1,    -1,    -1,    -1,    -1,   297,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   315,    -1,    -1,    -1,    -1,    -1,
     311,    -1,    51,    52,    -1,    -1,    -1,   318,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,   327,    -1,    -1,    -1,
      -1,   342,    71,    -1,    -1,    -1,    75,    -1,    -1,   340,
      -1,    -1,    -1,    -1,   345,    -1,   347,    -1,    87,    -1,
      -1,    -1,    -1,    -1,   355,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   365,    -1,    -1,   378,    -1,    -1,
      -1,    -1,   111,   374,    -1,    -1,   387,   388,    -1,   118,
     119,    -1,   383,    -1,   385,   386,    -1,   126,   389,    -1,
     391,    -1,    -1,    -1,    -1,   396,   407,    -1,    -1,    -1,
      -1,    -1,   413,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   412,    -1,    -1,    -1,    -1,   417,    -1,    -1,    -1,
     431,   422,   161,    -1,    -1,    -1,   165,    -1,   439,    -1,
      -1,   442,   433,    -1,    -1,    -1,    -1,   448,    -1,    -1,
     179,   442,    -1,    -1,   445,    -1,    -1,   186,   449,    -1,
     189,   190,    -1,    -1,    -1,   456,    -1,    -1,   197,    -1,
      -1,    -1,    -1,    -1,    -1,   204,    -1,   206,    -1,    82,
     209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     6,    -1,    -1,     9,    98,    -1,    12,    13,    14,
      -1,    -1,    17,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   246,    -1,    -1,
      -1,   250,    -1,   252,    -1,    -1,    -1,    -1,    -1,    -1,
     259,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   282,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,   297,    -1,
      -1,    -1,    -1,    74,    -1,    76,    77,    78,    79,    80,
      81,   184,    -1,    98,    -1,    -1,    -1,     6,    -1,   318,
       9,    -1,   195,    12,    13,    14,    -1,    -1,   327,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   340,    -1,    -1,    -1,    -1,   345,   118,   347,    -1,
      -1,    -1,   225,    -1,    -1,    -1,   355,    -1,    -1,   232,
      -1,    -1,    -1,   236,    -1,   238,   365,    -1,   241,    58,
     243,   156,   157,    -1,    -1,   374,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   383,    -1,   385,   386,    -1,    -1,
     389,    -1,   391,    82,    -1,    -1,    -1,   396,    -1,   184,
     185,   274,    -1,    -1,    -1,    -1,   191,    -1,    -1,    98,
     195,    -1,    -1,   412,    -1,    -1,    -1,    -1,   417,    -1,
      -1,    -1,    -1,   422,    -1,    -1,    -1,    -1,    -1,   214,
     201,    -1,    -1,    -1,   433,    -1,    -1,   222,    -1,    -1,
     225,   226,   315,   442,    -1,    -1,   445,   232,    -1,    -1,
     449,   236,    -1,   238,    -1,    -1,   241,   456,   243,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   157,   342,
      -1,   256,   257,    -1,    -1,    -1,    -1,   262,    -1,    -1,
      -1,    -1,   253,    -1,    -1,    -1,    -1,   272,    -1,   274,
      -1,    -1,    -1,    -1,   279,   184,   185,    -1,    -1,    -1,
      -1,    -1,   191,    -1,    -1,   378,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   387,   388,    -1,   302,    -1,    -1,
     291,    -1,    -1,    -1,    -1,   214,    -1,    -1,    -1,   300,
     315,    -1,    -1,   222,   407,    -1,   225,   226,    -1,    -1,
     413,    -1,   313,   232,    -1,    -1,    -1,   236,    -1,   238,
      -1,    -1,   241,    -1,   243,    -1,    -1,   342,   431,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   439,   256,   257,   442,
      -1,    -1,    -1,   262,    -1,   448,    -1,   450,   451,    -1,
      -1,    -1,    -1,   272,    -1,   274,   459,    -1,    -1,    -1,
     279,    -1,    -1,   378,    -1,    -1,    -1,    -1,   369,   370,
     371,    -1,   387,   388,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   302,    -1,    -1,    -1,    -1,    -1,   404,
      -1,    -1,   407,   408,    -1,    -1,   315,    -1,   413,     3,
      -1,     5,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,   431,    -1,   419,   420,
     421,    -1,    -1,   342,   439,    -1,    -1,   442,    -1,    -1,
      -1,    -1,    -1,   448,    -1,   450,   451,    -1,    -1,    -1,
     455,    -1,   457,    -1,   459,   460,   461,    51,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,   378,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,   387,   388,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,   407,    -1,
      -1,    -1,    -1,    -1,   413,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    82,   431,    -1,   118,   119,    -1,    -1,    -1,    -1,
     439,    -1,    -1,   442,    -1,    -1,    -1,    98,    -1,   448,
      -1,   450,   451,    -1,    -1,    -1,   455,    -1,   457,    -1,
     459,   460,   461,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,     6,    -1,    -1,
       9,    -1,     3,    -1,     5,   179,    -1,    -1,    -1,    10,
      -1,    -1,   186,    -1,    -1,   189,   190,    18,    -1,    -1,
      -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,    -1,
     204,    -1,   206,    -1,    -1,   209,    -1,    -1,    -1,    -1,
      49,    -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    52,    -1,    -1,   195,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    -1,   246,    82,    75,    -1,   250,    86,   252,    -1,
      -1,    -1,    -1,    -1,    -1,   259,    87,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,   236,    -1,   238,    -1,    -1,
     241,    -1,   243,    -1,    -1,    -1,     6,    -1,   282,     9,
     111,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,   297,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,    -1,   165,    -1,   340,    -1,    -1,    -1,
      -1,   345,    -1,   347,   315,   184,    -1,    -1,   179,    -1,
      -1,   355,    82,    -1,    -1,   186,   195,    -1,   189,   190,
      -1,   365,    -1,    -1,    94,    -1,   197,    -1,    98,    -1,
     374,   342,    -1,   204,    -1,   206,    -1,    -1,   209,   383,
      -1,   385,   386,    -1,    -1,   389,   225,   391,    -1,    -1,
      -1,    -1,   396,   232,    -1,    -1,    -1,   236,     6,   238,
      -1,     9,   241,    -1,   243,    -1,    -1,   378,   412,    -1,
      -1,    -1,    -1,   417,    -1,   246,   387,   388,   422,   250,
      -1,   252,    -1,    -1,    -1,    -1,    -1,    -1,   259,   433,
      -1,    -1,    -1,    -1,    -1,   274,   407,    -1,    -1,    -1,
     279,   445,   413,    -1,    -1,   449,    -1,    -1,    -1,    -1,
      -1,   282,   456,    -1,   184,    -1,    -1,    -1,    -1,    -1,
     431,    -1,    -1,    -1,    -1,   195,   297,    -1,   439,    -1,
      -1,   442,    -1,    -1,    82,    -1,   315,   448,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   325,   318,    -1,    -1,
      98,    -1,    -1,    -1,    -1,   225,   327,    -1,    -1,    -1,
      -1,     6,   232,   342,     9,    -1,   236,    -1,   238,   340,
      -1,   241,    -1,   243,   345,    -1,   347,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   355,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   372,   365,    -1,    -1,    -1,    -1,   378,
      -1,   149,    -1,   374,   274,    -1,    -1,    -1,   387,   388,
      -1,    -1,   383,    -1,   385,   386,    -1,    -1,   389,    -1,
     391,    -1,    -1,    -1,    -1,   396,    -1,    -1,   407,    -1,
      -1,    -1,    -1,    -1,   413,    -1,   184,    82,    -1,   418,
      -1,   412,    -1,    -1,    -1,   315,   417,   195,    -1,    -1,
      -1,   422,   431,    98,    -1,    -1,   435,    -1,    -1,    -1,
     439,    -1,   433,   442,    -1,    -1,    -1,   112,    -1,   448,
      -1,    -1,   342,    -1,   445,    -1,    -1,   225,   449,    -1,
      -1,    -1,    -1,     6,   232,    -1,     9,    -1,   236,    -1,
     238,    -1,    -1,   241,    -1,   243,    -1,     6,    -1,    -1,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   378,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,   388,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   407,    -1,   184,
      -1,    -1,    -1,   413,    -1,    -1,    -1,    -1,    -1,    -1,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,   431,    -1,    -1,    -1,   210,    -1,   315,    -1,   439,
      -1,    94,   442,    82,    -1,    98,    -1,    -1,   448,    -1,
     225,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,    98,
      -1,   236,     6,   238,   342,     9,   241,    -1,   243,    -1,
      -1,    -1,    -1,   112,    -1,    -1,     6,    -1,    -1,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   274,
     378,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,
     388,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   404,    -1,    -1,   407,
     408,   184,    -1,    -1,    -1,   413,    -1,    -1,    82,    -1,
     315,    -1,   195,    -1,    -1,   184,    -1,    -1,    -1,    -1,
      94,    -1,    82,   431,    98,    -1,   195,    -1,    -1,    -1,
      -1,   439,    -1,    -1,   442,    -1,    -1,   342,    98,    -1,
     448,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,   232,
      -1,    -1,    -1,   236,    -1,   238,   225,    -1,   241,    -1,
     243,    82,    -1,   232,    -1,    -1,    -1,   236,     6,   238,
      -1,     9,   241,   378,   243,    -1,    -1,    98,    -1,    -1,
      -1,    -1,   387,   388,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   407,    -1,    -1,   274,    -1,    -1,   413,    -1,
     184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,    -1,   184,    -1,   431,    -1,    -1,    -1,
      -1,    -1,   315,    -1,   439,   195,    -1,   442,    -1,    -1,
      -1,    -1,    -1,   448,    82,    -1,   315,    -1,    -1,    -1,
      -1,   225,    -1,    -1,    -1,    -1,   177,    -1,   232,   342,
      98,    -1,   236,   184,   238,   225,    -1,   241,    -1,   243,
      -1,     6,   232,   342,     9,    -1,   236,    -1,   238,    -1,
      -1,   241,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   378,    -1,    -1,    -1,    -1,
     274,    -1,    -1,    -1,   387,   388,    -1,    -1,    -1,   378,
      -1,    -1,    -1,    -1,   274,   236,    -1,   238,   387,   388,
     241,    -1,    -1,    -1,   407,    -1,    -1,    -1,    -1,    -1,
     413,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   407,    -1,
      -1,   315,   180,    -1,   413,    -1,   184,    82,   431,    -1,
      -1,    -1,    -1,   274,    -1,   315,   439,   195,    -1,   442,
      -1,    -1,   431,    98,    -1,   448,    -1,    -1,   342,    -1,
     439,    -1,    -1,   442,    -1,    -1,    -1,    -1,    -1,   448,
      -1,    -1,   342,     6,    -1,    -1,     9,   225,    -1,    -1,
      -1,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,    -1,
     238,    -1,    -1,   241,   378,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   387,   388,    -1,    -1,    -1,   378,    -1,
      -1,   342,    -1,    -1,    -1,    -1,    -1,   387,   388,    -1,
      -1,    -1,    -1,   407,    -1,    -1,   274,    -1,    -1,   413,
      -1,    -1,    -1,   403,    -1,    -1,    -1,   407,    -1,   184,
      -1,    -1,    -1,   413,    -1,    -1,    -1,   431,    -1,    82,
     195,    -1,    -1,    -1,    -1,   439,   387,   388,   442,    -1,
      -1,   431,    -1,    -1,   448,    98,   397,   315,    -1,   439,
      -1,    -1,   442,    -1,    -1,    -1,     6,    -1,   448,     9,
     225,    -1,   413,    -1,    -1,    -1,    -1,   232,    -1,    -1,
      -1,   236,   423,   238,   342,    -1,   241,    -1,   243,    -1,
     431,    -1,    -1,    -1,    -1,   436,    -1,    -1,   439,    -1,
     441,   442,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   274,
     378,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,
     388,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    82,    -1,    -1,    -1,    -1,    -1,     6,   407,
      -1,     9,   195,    -1,    -1,   413,    -1,    -1,    98,    -1,
     315,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   431,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   439,   225,    -1,   442,    -1,    -1,   342,    -1,   232,
     448,    -1,    -1,   236,    -1,   238,    -1,    -1,   241,    -1,
     243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   372,    -1,    -1,
      -1,    -1,    -1,   378,    82,    -1,    -1,    -1,    -1,    -1,
      -1,   274,   387,   388,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   407,    -1,    -1,   195,    -1,    -1,   413,    -1,
      -1,    -1,    -1,    82,    -1,    37,    38,    39,    40,    41,
      42,    43,   315,    -1,    -1,    -1,   431,    -1,    -1,    98,
      -1,    -1,    -1,    -1,   439,   225,    -1,   442,    -1,    -1,
      -1,    -1,   232,   448,    66,    67,   236,    -1,   238,   342,
      -1,   241,    74,   243,    76,    77,    78,    79,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,
     373,    -1,    -1,    -1,   274,   378,    -1,   195,    -1,    -1,
      -1,    -1,    -1,    -1,   387,   388,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   407,   184,    -1,   225,    -1,    -1,
     413,    -1,    -1,    -1,   232,   315,   195,    -1,   236,    -1,
     238,    -1,    -1,   241,    -1,   243,    -1,    -1,   431,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,    -1,   442,
      -1,    -1,   342,    -1,    -1,   448,   225,    -1,    -1,    -1,
      -1,    82,    -1,   232,    -1,    -1,   274,   236,    -1,   238,
     192,    -1,   241,    -1,   243,    -1,    -1,    98,    -1,   201,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   378,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,   387,   388,    -1,
      -1,    -1,    -1,    -1,    -1,   274,    -1,   315,    -1,    -1,
      -1,   233,   234,    -1,    -1,    -1,    -1,   407,    -1,    -1,
      -1,    -1,    -1,   413,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   253,    -1,    -1,   342,    -1,   258,    82,    -1,    -1,
      -1,   431,    -1,    -1,    -1,    -1,   315,    -1,    -1,   439,
      -1,    -1,   442,    98,   276,    -1,    -1,    -1,   448,    -1,
      -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,   291,
     378,    -1,    -1,   342,   195,    -1,   298,    -1,   300,   387,
     388,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   313,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   407,
      -1,    -1,    -1,    -1,   225,   413,    -1,    -1,    -1,   378,
      -1,   232,    -1,    -1,    -1,   236,    -1,   238,   387,   388,
     241,    -1,   243,   431,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   439,   177,    -1,   442,    -1,    -1,    -1,   407,   184,
     448,    -1,    -1,    -1,   413,   367,    -1,   369,   370,   371,
      -1,    -1,    -1,   274,   376,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   431,    -1,    -1,    -1,    -1,    -1,   390,    -1,
     439,    -1,    -1,   442,    -1,    -1,    -1,    -1,    -1,   448,
      -1,   450,   451,    -1,    -1,    -1,    -1,    -1,    -1,   458,
     459,   236,   414,   238,   315,    -1,   241,   419,   420,   421,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     432,    -1,    -1,   435,   436,    -1,    -1,    -1,    -1,    -1,
      -1,   342,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   274,
      -1,    -1,    -1,    -1,   456,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   378,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   387,   388,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   407,    -1,    -1,    -1,
      -1,    -1,   413,    -1,    -1,    -1,    -1,   342,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     431,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,
      -1,   442,    -1,    -1,    -1,    -1,    -1,   448,    -1,   450,
     451,    -1,    -1,    -1,    -1,    -1,    -1,   458,   459,    -1,
      -1,    -1,   387,   388,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   397,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   413,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   423,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   431,    -1,    -1,    -1,
      -1,   436,    -1,    -1,   439,    -1,   441,   442
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   465,   466,     0,   183,   311,   467,   468,   469,   475,
     482,   484,   456,   456,     0,   468,   150,   490,   490,   236,
     312,   485,   485,   120,   470,   476,    27,   486,   486,   456,
     102,   595,   595,   236,   456,   483,    83,   491,   120,   471,
     477,   214,   487,  1140,   356,   169,   208,   216,   545,   456,
     307,   728,   728,   456,    73,   322,   406,   488,   489,   456,
     456,   356,   456,   168,   354,   405,   596,   602,   120,   472,
     478,   482,   135,   481,   489,   310,  1152,   492,   546,   456,
     547,   356,   444,   627,   598,    54,   434,   731,   139,   472,
     479,   490,   485,   275,   337,   377,   379,   493,   494,   498,
     506,   511,   549,   169,   548,    22,   251,   350,   584,   585,
     586,   587,   589,   593,   594,   456,   356,   239,   681,   442,
     601,   603,  1070,   733,   732,   341,   739,   485,   473,   456,
     456,   456,   456,   456,   359,   550,   456,   216,   583,    60,
      72,    93,   101,   178,   405,  1136,   319,   374,   375,   588,
     456,   586,   456,   594,   597,   456,   356,   235,   683,   600,
     602,   629,   630,   631,   604,    49,   734,   735,   736,  1131,
     734,   442,   456,   456,   595,   245,   358,   442,   497,   499,
     500,   501,   502,   504,   505,  1152,   182,   507,   508,   509,
     441,   495,   496,   497,  1166,    11,    25,    26,    57,    96,
      97,    99,   108,   151,   152,   162,   237,   352,   392,   442,
     512,   513,   514,   515,   522,   531,   535,   538,   539,   540,
     541,   542,   543,   544,   283,  1117,   549,   456,   280,   280,
    1149,   403,   277,  1148,   403,   395,  1160,    24,  1127,   599,
     628,   456,   356,   332,   685,   603,   449,   632,   633,  1111,
     605,   735,   372,   418,   737,   325,   435,   729,   474,   372,
    1140,   456,   500,   456,   501,    63,   363,  1123,     9,   236,
     510,   456,   509,   456,   456,   496,   107,   442,  1082,  1140,
    1140,  1082,   384,   367,  1156,  1140,  1140,  1140,  1140,  1140,
    1082,  1140,    56,  1133,  1140,   456,   514,  1082,   584,   442,
    1077,  1078,  1097,  1077,  1078,   236,  1078,    85,  1134,   175,
    1137,   600,   629,   682,   456,   356,   351,   726,   629,   456,
     633,  1111,   227,   228,   634,   637,   638,   644,     1,    47,
      62,   102,   219,   231,   319,   320,   332,   334,   435,   456,
     606,   607,   609,   613,   615,   617,   618,   624,   625,   626,
    1140,  1140,   372,   283,   738,   109,   740,   728,  1140,   236,
    1100,   456,   363,  1140,   211,   211,   236,   456,   249,   523,
    1082,  1082,  1140,  1140,  1140,  1078,    68,  1082,  1082,  1078,
    1140,  1078,   532,   533,  1082,    96,   517,  1082,   551,   200,
     277,  1120,  1078,   280,   410,   280,   590,   591,  1070,  1069,
    1070,   629,   684,   456,   356,   634,   171,   442,   640,   641,
     442,   640,  1134,  1140,   319,   321,  1121,  1121,  1140,  1134,
    1140,   249,  1146,  1140,    23,  1126,   277,   167,   188,    31,
     110,  1100,  1140,   442,   456,   730,   478,  1100,  1078,  1140,
     195,   236,   243,   315,   378,   407,   448,   536,   537,  1103,
    1078,   236,  1078,    23,   214,  1082,  1141,   278,   280,   519,
     520,   521,   516,   552,  1097,  1077,   280,  1077,   591,   301,
     592,  1070,   629,   686,   456,   635,    84,   636,  1100,   442,
    1140,  1126,  1079,  1097,   279,   380,   614,   236,  1078,  1081,
    1100,   436,  1140,   442,   725,   725,   170,   442,  1100,   741,
     742,   139,   480,    56,   443,   503,   124,   195,   236,   243,
     255,   315,   378,   381,   382,   448,   524,   525,   526,   529,
     537,   400,   534,  1100,   520,   384,  1159,   518,     1,     4,
      19,    29,   202,   233,   240,   261,   286,   292,   319,   326,
     339,   364,   366,   374,   405,   456,   553,   554,   559,   561,
     566,   567,   568,   569,   573,   574,   575,   576,   577,   578,
     579,   580,   582,  1123,  1077,  1100,   317,   687,   688,   689,
     727,   645,   642,  1140,   435,   674,   403,   612,  1097,   234,
    1144,   403,  1133,   200,  1139,   442,  1140,  1140,   742,     1,
     442,   743,   744,   745,   746,   747,   752,   485,   526,    17,
     400,  1103,  1100,  1140,   520,  1146,   319,   403,  1164,   364,
    1146,   218,  1142,  1140,    55,  1132,    37,   113,  1130,  1142,
    1142,   260,  1100,  1166,   384,  1140,   725,   689,     1,    21,
      34,    37,    38,    39,    40,    41,    42,    43,    74,    76,
      77,    78,    79,    80,    81,   118,   201,   217,   236,   253,
     276,   291,   298,   300,   313,   323,   330,   367,   369,   370,
     371,   393,   419,   420,   421,   432,   435,   639,   646,   647,
     648,   650,   651,   652,   653,   654,   656,   668,   669,   671,
     672,   673,   679,   680,  1140,  1157,    27,  1128,   188,  1141,
    1100,    56,   321,   608,   619,  1100,   372,  1158,   236,   616,
    1097,   616,   126,   456,   356,     3,     5,    10,    18,    51,
      52,    59,    71,    75,    87,   111,   118,   119,   149,   161,
     165,   179,   186,   189,   190,   197,   204,   206,   209,   246,
     250,   252,   259,   282,   297,   318,   327,   340,   345,   347,
     355,   365,   374,   383,   385,   386,   389,   391,   396,   412,
     417,   422,   433,   445,   449,   456,   753,   754,   764,   769,
     773,   776,   789,   792,   797,   802,   803,   804,   807,   809,
     816,   820,   822,   837,   840,   842,   844,   847,   849,   855,
     864,   866,   883,   885,   888,   892,   898,   908,   915,   917,
     920,   924,   925,   936,   947,   957,   963,   967,   973,   977,
     979,   981,   983,   986,   997,   998,  1007,  1009,  1010,  1167,
     456,   527,   529,  1082,  1140,  1142,   123,   167,   556,  1140,
    1140,   319,   326,   574,  1140,  1140,   364,  1140,  1140,  1127,
       9,   260,   318,   581,  1140,   442,   690,   225,   368,   418,
     368,   418,   368,   418,   368,   418,   368,   418,   438,  1165,
     346,  1154,  1100,  1096,  1097,  1097,   214,   224,   346,   670,
    1140,  1140,   167,   188,   223,   411,     9,    50,   225,   643,
    1101,  1102,  1103,   676,   677,  1101,    30,   620,   621,   622,
     623,  1129,  1166,  1133,   180,   611,  1138,   109,   236,   748,
     755,   765,   770,   774,   777,   790,   793,   798,   805,   808,
     810,   817,   821,   823,   838,   841,   843,  1164,   848,     1,
     850,   856,   865,   867,   884,   886,   889,   893,   899,   909,
     916,   918,   921,   926,   937,   948,   958,   236,   349,   968,
     974,   306,   978,   980,   982,   984,   987,   188,   999,  1137,
    1011,   195,   236,   243,   315,   378,   448,   528,   530,   123,
     316,   364,   560,  1140,   117,   305,   555,    32,   164,   244,
     570,  1078,  1081,   381,  1078,  1078,   287,  1151,  1151,   281,
    1078,    61,    88,    89,   293,   456,   691,   692,   696,  1140,
     448,   403,   657,   459,  1098,  1099,   400,   653,  1101,    27,
     649,   362,  1119,  1119,  1103,  1148,  1148,   463,   675,   677,
     400,    48,   409,   174,   612,  1100,   456,   456,   756,  1095,
    1096,     6,    82,    94,    98,   184,   225,   232,   238,   241,
     274,   342,   387,   388,   413,   431,   439,   766,  1064,  1085,
    1086,  1095,  1101,  1104,   442,   771,  1051,  1052,  1053,   236,
    1074,  1075,  1076,  1097,   236,  1093,  1095,  1104,     9,   791,
     794,   799,  1065,  1066,  1086,  1070,   405,   236,   811,  1085,
    1092,  1095,   818,  1086,   236,   404,   408,   824,   825,  1051,
     296,   297,   310,   356,   839,     6,  1083,  1084,  1095,  1095,
     845,   136,  1050,  1051,  1083,   695,  1095,   868,  1095,  1101,
    1104,   949,  1097,    94,   887,  1086,   890,  1086,   894,   177,
     236,   900,   903,   904,   905,  1074,  1093,  1097,  1166,  1070,
    1067,  1097,  1070,  1067,     9,   927,  1068,  1097,   150,   248,
     938,   939,   940,   941,   943,   944,   945,   946,  1071,  1072,
    1083,   949,  1070,   965,   964,   112,   969,   970,  1086,    94,
     975,  1085,   695,  1095,  1070,  1095,     8,    35,  1001,   107,
    1067,    17,  1078,   118,   236,   557,  1145,   441,   571,   571,
     122,   376,   441,   457,   565,  1113,  1122,  1078,  1080,  1140,
     172,   693,   694,   693,  1141,   704,   188,  1100,   402,  1163,
     225,   450,   451,   459,  1061,  1063,  1064,  1087,  1095,  1102,
    1104,   459,  1099,  1097,   236,  1132,  1096,  1096,  1103,  1165,
    1101,  1081,  1081,  1129,  1133,   127,   763,    30,   180,   757,
    1129,  1148,   459,  1095,   459,  1105,   459,  1106,  1148,  1120,
     459,   459,   459,   459,   459,   459,   459,   459,  1105,   128,
     768,   403,   767,  1086,   205,  1114,    56,  1054,  1055,   403,
    1120,   434,   778,   236,  1092,  1095,  1070,   130,   800,   156,
     457,   801,  1066,   348,  1118,   319,  1153,  1069,   132,   815,
     757,   427,   428,   429,   430,   133,   819,    49,   210,   778,
      17,   438,   826,   827,   828,   832,  1125,   100,  1148,  1084,
    1073,   399,  1162,   857,  1166,  1095,    92,   331,   394,   869,
     870,   871,   875,   880,   951,  1086,   403,   137,   891,    49,
     166,   207,   215,   288,   895,   904,   138,   901,   423,   436,
     400,   402,   397,   258,   304,  1115,   180,  1012,  1153,  1012,
    1068,   143,   935,   436,   929,  1090,  1095,  1102,   944,   946,
    1083,   403,  1072,   121,   403,   424,   942,   959,   187,   341,
     966,  1131,   210,   970,  1095,   146,   976,   180,   180,   319,
     321,   985,   112,   988,   333,   380,  1002,  1149,  1012,   530,
     565,  1113,   558,  1097,   240,   347,  1140,   122,   562,   563,
    1092,   695,   702,  1100,   637,   705,   114,   658,  1148,  1063,
    1063,  1063,    70,   360,   458,  1062,   450,   451,   452,   453,
     455,   462,  1063,   365,  1155,  1144,  1081,   114,   610,  1090,
      25,    26,    67,    69,   103,   104,   105,   150,   152,   160,
     234,   401,   442,  1072,   441,   760,    66,   233,   301,   758,
     759,   149,   310,  1088,  1096,  1061,  1063,   403,  1063,  1061,
    1107,  1096,  1102,  1104,   442,  1063,  1110,  1063,  1063,  1109,
    1063,  1061,  1061,  1063,  1108,  1063,  1065,  1086,   187,   341,
     772,  1114,    12,    13,    14,    20,    58,   156,   157,   185,
     191,   214,   222,   226,   256,   257,   262,   272,   279,   284,
     302,   448,   450,   451,   452,   453,   455,   457,   458,   460,
     461,  1056,  1057,  1058,  1059,  1060,    12,    13,    14,    58,
     214,   256,   257,   262,   272,   279,   302,   450,   451,   455,
     459,  1056,  1057,  1058,  1059,  1060,  1086,   309,   775,  1076,
     779,   187,   341,   783,   324,   416,   795,   796,  1166,  1051,
     213,   266,  1043,  1044,  1045,  1047,   426,   441,   812,  1166,
     163,  1018,  1019,  1018,  1018,  1018,  1086,  1065,  1086,    21,
     404,   408,   833,   834,  1052,   134,   836,   440,   828,   830,
     438,   829,   825,  1096,   114,   846,  1074,   851,     9,    12,
      15,    16,   253,   254,   272,   273,   858,   862,   171,  1090,
       9,    56,   173,   223,   411,   876,   877,   878,   872,   870,
     953,  1122,  1149,   403,  1083,  1065,  1086,   366,   896,   749,
     750,  1050,   906,   907,  1095,  1074,     8,    35,  1014,  1153,
    1092,   210,   910,   922,  1166,   930,  1129,  1095,   930,   403,
     403,   521,   149,   404,   408,  1086,    49,   218,   960,  1086,
    1086,   372,  1086,  1095,   180,  1065,  1086,  1090,  1131,   210,
     991,  1095,   159,   163,  1003,     9,  1008,  1074,   922,   562,
    1097,   280,   564,  1078,  1113,   564,   193,   697,   233,   234,
     703,   640,  1149,    28,   115,   202,   655,   660,   661,   662,
     663,   665,  1096,  1102,  1104,   458,  1063,  1063,  1063,  1063,
    1063,  1063,   458,  1063,  1164,  1144,  1149,  1017,  1019,   447,
     446,  1090,  1017,   218,    31,    33,    36,    46,   176,   181,
     194,   242,   290,   314,   338,   343,   353,   357,   415,   425,
     761,   762,  1017,   270,  1147,  1147,  1147,   759,   758,   236,
    1089,  1096,   458,  1095,   462,   458,  1062,   458,   458,  1062,
     458,   458,   458,   458,  1062,   458,   458,   373,  1023,  1024,
    1065,  1084,   341,  1164,   398,  1161,  1161,   403,  1074,   780,
     781,   782,  1131,  1095,  1095,   163,   289,   784,  1004,  1137,
     240,   260,  1023,  1046,  1048,   131,   806,  1047,    96,   305,
     442,  1072,    33,    36,    44,    45,    46,   158,   176,   194,
     242,   290,   343,   353,   415,   813,   814,  1018,   269,  1020,
     265,  1021,   187,  1023,   187,  1125,   400,   835,   831,   833,
     749,  1149,   749,  1164,   331,   859,  1164,   403,    49,   877,
     879,  1090,     9,    56,   223,   411,   873,   874,  1090,   954,
    1123,   200,   285,  1150,   663,  1083,  1023,   187,  1166,  1069,
     138,   902,   751,     8,   180,   910,  1095,   126,   263,  1033,
    1034,  1036,  1043,   240,   260,   438,   126,   438,   932,   933,
    1090,  1089,  1086,  1140,  1043,   971,  1166,  1095,  1023,   187,
     403,     9,   989,   990,  1112,   992,  1095,   971,   992,   307,
    1006,   308,  1013,  1014,  1113,   251,   319,   321,   572,  1140,
     173,   698,  1100,   706,  1078,  1131,   664,   665,   661,  1142,
     659,   660,   458,   404,   678,  1078,  1021,  1017,  1140,  1140,
     121,   424,   762,  1092,  1092,  1092,  1105,  1118,   458,  1063,
    1078,  1105,  1105,  1063,  1105,  1105,  1105,   223,   411,  1105,
    1105,  1025,   268,  1026,  1023,  1084,   156,   284,   156,   284,
     781,   279,   737,    86,   325,   435,   265,   267,   786,  1005,
     785,   329,   344,   749,   749,   812,   812,   812,   812,  1140,
     153,   155,  1140,   121,   424,   814,   749,  1022,  1065,  1066,
    1065,  1066,   834,  1051,   749,  1095,   125,   852,   435,   860,
     861,   862,   110,   863,   435,  1091,  1095,  1101,  1090,    49,
     881,   874,   175,   881,   950,  1140,   285,  1142,  1065,   581,
     897,  1166,   752,   907,  1086,   199,   911,  1166,  1035,  1037,
     141,   919,  1036,   142,   923,   240,  1051,   931,  1050,   932,
     262,   961,  1116,   144,   962,   289,  1028,  1029,   300,  1118,
    1065,  1091,   284,  1090,   113,   993,   394,   995,  1149,   154,
     264,  1015,  1038,  1039,  1041,  1044,     7,  1124,   572,  1100,
     116,   220,   699,    67,    66,    67,   192,   233,   234,   258,
     303,   376,   390,   414,   436,   456,   651,   652,   654,   656,
     668,   671,   673,   707,   708,   710,   711,   712,   713,   715,
     716,   717,   718,   722,   723,   442,   666,   667,  1140,  1140,
     448,  1094,  1095,  1100,  1094,  1049,  1131,  1049,  1023,   458,
     749,  1027,  1164,   156,  1164,   156,  1086,   129,   788,   787,
     749,  1018,  1018,  1018,  1018,  1094,  1094,  1049,  1049,   749,
    1023,   328,  1023,   328,   853,   136,   854,   861,   102,  1135,
     881,   881,  1091,  1014,   207,   434,   955,  1078,  1140,  1023,
     240,   260,    49,   240,   218,   912,   198,   240,   260,   437,
     749,   749,   928,   749,   934,   695,  1056,  1057,  1058,  1059,
    1060,  1030,   145,   972,   267,  1031,  1095,  1023,  1023,   990,
    1139,    95,   994,  1139,  1028,   166,   207,   215,   288,  1000,
    1069,  1040,  1042,   148,  1016,  1041,   293,  1072,  1094,  1140,
      91,   221,   700,   271,  1147,   203,   724,   270,   271,   721,
    1126,   192,   438,  1140,  1148,  1140,  1095,   713,   258,   299,
     719,   720,  1100,   667,  1077,  1101,  1094,   749,  1164,  1164,
     749,  1066,  1066,   749,    49,   881,   406,   882,   307,  1069,
     187,   288,   956,   952,   344,  1086,  1140,   913,  1033,  1044,
     240,   240,   749,   749,   749,  1032,  1095,  1139,  1095,   147,
     996,   749,   749,   233,   234,  1143,  1100,  1140,  1140,   174,
     701,  1140,  1141,  1140,  1050,  1095,   714,  1078,    90,    91,
     116,   294,   295,   335,   336,   709,   180,   293,  1100,   720,
    1143,  1023,  1023,  1086,  1086,  1140,  1069,   307,  1097,   423,
     695,   140,   914,   749,  1095,  1100,  1100,  1140,  1100,  1100,
    1118,  1086,   903,  1140,  1050,  1100,    49,   903,  1086
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   464,   466,   465,   467,   467,   468,   468,   470,   471,
     469,   473,   474,   472,   476,   477,   475,   478,   478,   478,
     479,   479,   480,   481,   483,   482,   484,   485,   485,   486,
     486,   487,   487,   488,   488,   488,   489,   489,   490,   490,
     491,   491,   492,   492,   493,   493,   493,   493,   494,   495,
     495,   495,   495,   496,   497,   498,   499,   499,   499,   499,
     500,   500,   501,   501,   501,   502,   503,   503,   504,   505,
     506,   507,   507,   508,   508,   509,   509,   510,   510,   511,
     512,   512,   512,   513,   513,   514,   514,   514,   514,   514,
     514,   514,   514,   514,   514,   514,   515,   516,   515,   517,
     515,   515,   515,   515,   515,   518,   518,   519,   519,   520,
     521,   521,   523,   522,   524,   524,   524,   524,   524,   525,
     525,   526,   526,   527,   526,   528,   528,   529,   529,   529,
     529,   529,   529,   530,   530,   530,   530,   530,   530,   531,
     532,   533,   533,   534,   534,   535,   536,   536,   537,   537,
     538,   539,   540,   541,   542,   543,   544,   545,   545,   546,
     545,   547,   545,   548,   548,   549,   549,   551,   550,   552,
     552,   553,   553,   553,   553,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   553,   554,   554,   554,   555,
     555,   555,   556,   556,   556,   557,   557,   557,   558,   558,
     559,   560,   560,   560,   561,   561,   563,   562,   562,   564,
     565,   565,   565,   566,   567,   568,   568,   568,   569,   570,
     570,   570,   571,   571,   571,   571,   572,   572,   573,   573,
     574,   574,   574,   574,   574,   575,   576,   577,   577,   578,
     579,   579,   580,   581,   581,   581,   582,   583,   583,   584,
     584,   584,   584,   585,   585,   586,   586,   587,   588,   588,
     588,   588,   589,   590,   590,   591,   592,   592,   593,   593,
     593,   594,   594,   594,   594,   594,   595,   595,   596,   597,
     596,   598,   596,   599,   599,   600,   601,   601,   602,   602,
     604,   603,   605,   605,   606,   606,   606,   606,   606,   606,
     606,   606,   606,   606,   606,   606,   607,   608,   608,   608,
     609,   609,   609,   610,   610,   611,   611,   612,   612,   613,
     614,   614,   615,   615,   616,   616,   617,   618,   619,   619,
     620,   620,   620,   621,   622,   623,   624,   625,   626,   626,
     627,   628,   627,   629,   629,   631,   630,   632,   632,   632,
     632,   632,   633,   634,   635,   634,   636,   634,   637,   638,
     639,   639,   640,   640,   640,   641,   642,   642,   643,   643,
     643,   644,   645,   645,   646,   646,   646,   646,   646,   646,
     646,   646,   646,   646,   646,   646,   646,   646,   646,   647,
     648,   649,   649,   650,   651,   652,   652,   653,   653,   653,
     653,   653,   653,   653,   653,   653,   653,   653,   653,   653,
     653,   653,   653,   653,   653,   653,   653,   653,   653,   653,
     653,   653,   653,   653,   653,   653,   653,   653,   653,   653,
     653,   653,   654,   654,   655,   655,   655,   656,   657,   657,
     658,   658,   659,   659,   660,   661,   662,   662,   663,   663,
     664,   664,   665,   666,   666,   667,   668,   669,   670,   670,
     670,   671,   672,   673,   675,   674,   676,   676,   677,   677,
     678,   678,   679,   679,   680,   681,   682,   681,   683,   684,
     683,   685,   686,   685,   687,   687,   688,   688,   689,   690,
     690,   691,   691,   691,   691,   692,   692,   693,   694,   694,
     695,   695,   696,   697,   697,   698,   698,   699,   699,   700,
     700,   701,   701,   702,   702,   702,   703,   703,   704,   704,
     705,   706,   706,   707,   707,   707,   707,   707,   707,   707,
     707,   707,   707,   707,   707,   707,   707,   707,   707,   708,
     709,   709,   709,   709,   709,   709,   709,   710,   711,   711,
     711,   712,   712,   713,   714,   715,   716,   717,   718,   718,
     719,   719,   720,   720,   720,   721,   721,   721,   722,   723,
     724,   724,   725,   726,   727,   726,   728,   729,   730,   728,
     731,   732,   731,   733,   731,   734,   734,   735,   736,   736,
     736,   737,   737,   737,   737,   737,   738,   738,   739,   739,
     740,   741,   740,   742,   742,   743,   743,   743,   743,   743,
     744,   745,   746,   747,   748,   748,   750,   751,   749,   752,
     752,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   755,   754,   756,   756,   756,   756,   756,   756,
     756,   756,   756,   756,   756,   756,   756,   756,   756,   756,
     756,   757,   757,   757,   757,   757,   757,   758,   759,   759,
     760,   760,   761,   761,   762,   762,   762,   762,   762,   762,
     762,   762,   762,   762,   762,   762,   762,   762,   762,   762,
     762,   763,   763,   765,   764,   766,   766,   766,   767,   767,
     768,   768,   770,   769,   771,   771,   772,   772,   773,   774,
     774,   775,   775,   777,   776,   778,   779,   778,   780,   780,
     781,   781,   782,   782,   782,   782,   783,   783,   783,   784,
     785,   784,   786,   787,   786,   788,   788,   790,   789,   791,
     791,   791,   793,   792,   794,   794,   795,   795,   795,   795,
     795,   796,   796,   798,   797,   799,   800,   800,   801,   801,
     802,   803,   805,   804,   806,   806,   808,   807,   810,   809,
     811,   811,   811,   811,   811,   811,   811,   811,   811,   812,
     812,   812,   813,   813,   814,   814,   814,   814,   814,   814,
     814,   814,   814,   814,   814,   814,   814,   814,   814,   815,
     815,   817,   816,   818,   818,   818,   818,   818,   819,   819,
     821,   820,   823,   822,   824,   824,   825,   825,   825,   826,
     827,   827,   829,   828,   830,   831,   830,   832,   832,   833,
     833,   834,   834,   834,   834,   835,   835,   836,   836,   838,
     837,   839,   839,   839,   839,   839,   839,   841,   840,   843,
     842,   845,   844,   846,   846,   848,   847,   850,   851,   849,
     849,   852,   853,   852,   854,   854,   856,   855,   857,   857,
     858,   858,   858,   859,   859,   860,   860,   861,   862,   862,
     862,   862,   862,   862,   862,   863,   863,   865,   864,   867,
     866,   868,   868,   868,   869,   869,   870,   870,   870,   872,
     871,   873,   873,   874,   874,   874,   874,   874,   874,   875,
     876,   876,   877,   877,   878,   878,   878,   878,   878,   879,
     880,   881,   881,   882,   882,   884,   883,   886,   885,   887,
     887,   889,   888,   890,   890,   891,   891,   893,   892,   894,
     894,   895,   895,   895,   895,   896,   896,   897,   897,   897,
     899,   898,   900,   901,   900,   900,   902,   902,   903,   903,
     904,   904,   904,   904,   904,   905,   905,   906,   906,   907,
     909,   908,   910,   910,   911,   911,   911,   911,   911,   911,
     912,   912,   913,   913,   913,   914,   914,   916,   915,   918,
     917,   919,   919,   921,   920,   922,   922,   922,   923,   923,
     924,   926,   925,   927,   928,   927,   929,   929,   930,   931,
     930,   932,   932,   934,   933,   935,   935,   937,   936,   938,
     938,   938,   938,   938,   939,   940,   940,   941,   942,   942,
     943,   943,   944,   945,   945,   946,   946,   948,   947,   950,
     949,   951,   951,   952,   952,   953,   953,   954,   954,   955,
     955,   955,   956,   956,   956,   958,   959,   957,   960,   960,
     961,   961,   961,   961,   961,   962,   962,   964,   963,   965,
     963,   966,   966,   966,   968,   967,   969,   969,   970,   970,
     970,   971,   971,   972,   972,   974,   973,   975,   975,   975,
     976,   976,   977,   978,   978,   980,   979,   982,   981,   984,
     983,   985,   985,   985,   987,   986,   988,   988,   989,   989,
     990,   991,   991,   992,   993,   993,   994,   994,   995,   995,
     996,   996,   997,   997,   997,   998,   999,   999,  1000,  1000,
    1000,  1000,  1000,  1001,  1001,  1002,  1002,  1003,  1003,  1004,
    1004,  1005,  1005,  1006,  1006,  1007,  1008,  1008,  1009,  1011,
    1010,  1012,  1012,  1013,  1013,  1013,  1013,  1014,  1014,  1015,
    1015,  1015,  1016,  1016,  1017,  1018,  1019,  1020,  1019,  1021,
    1022,  1021,  1023,  1024,  1025,  1024,  1026,  1027,  1026,  1028,
    1029,  1030,  1029,  1031,  1032,  1031,  1033,  1033,  1033,  1035,
    1034,  1037,  1036,  1038,  1038,  1038,  1040,  1039,  1042,  1041,
    1043,  1043,  1044,  1044,  1044,  1046,  1045,  1048,  1047,  1049,
    1049,  1050,  1051,  1053,  1052,  1054,  1054,  1054,  1054,  1054,
    1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,
    1054,  1054,  1054,  1054,  1055,  1055,  1055,  1055,  1055,  1055,
    1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,
    1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,
    1055,  1055,  1056,  1056,  1056,  1057,  1057,  1058,  1058,  1059,
    1059,  1059,  1060,  1060,  1060,  1061,  1061,  1062,  1062,  1062,
    1063,  1063,  1063,  1063,  1063,  1063,  1063,  1063,  1063,  1064,
    1064,  1065,  1065,  1066,  1067,  1068,  1069,  1069,  1070,  1071,
    1071,  1072,  1073,  1073,  1074,  1075,  1075,  1075,  1076,  1077,
    1077,  1078,  1079,  1079,  1080,  1080,  1081,  1081,  1082,  1083,
    1083,  1084,  1084,  1085,  1085,  1086,  1086,  1086,  1086,  1086,
    1086,  1086,  1086,  1086,  1087,  1087,  1087,  1087,  1087,  1087,
    1087,  1088,  1088,  1089,  1089,  1090,  1090,  1091,  1091,  1092,
    1092,  1093,  1093,  1093,  1094,  1094,  1094,  1095,  1096,  1096,
    1096,  1096,  1097,  1097,  1098,  1099,  1099,  1100,  1101,  1101,
    1102,  1102,  1103,  1103,  1103,  1103,  1103,  1103,  1103,  1104,
    1104,  1104,  1104,  1104,  1104,  1104,  1104,  1104,  1104,  1104,
    1104,  1105,  1105,  1105,  1106,  1106,  1107,  1107,  1108,  1108,
    1108,  1109,  1109,  1110,  1110,  1111,  1112,  1112,  1113,  1113,
    1114,  1114,  1115,  1115,  1115,  1116,  1116,  1117,  1117,  1118,
    1118,  1119,  1119,  1120,  1120,  1121,  1121,  1122,  1122,  1123,
    1123,  1124,  1124,  1125,  1125,  1126,  1126,  1127,  1127,  1128,
    1128,  1129,  1129,  1130,  1130,  1131,  1131,  1132,  1132,  1133,
    1133,  1134,  1134,  1135,  1135,  1136,  1136,  1137,  1137,  1138,
    1138,  1139,  1139,  1140,  1140,  1141,  1141,  1141,  1142,  1142,
    1143,  1143,  1143,  1144,  1144,  1145,  1145,  1146,  1146,  1147,
    1147,  1148,  1148,  1149,  1149,  1150,  1150,  1150,  1151,  1151,
    1152,  1152,  1153,  1153,  1154,  1154,  1155,  1155,  1156,  1156,
    1157,  1157,  1157,  1158,  1158,  1159,  1159,  1160,  1160,  1161,
    1161,  1162,  1162,  1163,  1163,  1164,  1164,  1165,  1165,  1166,
    1166,  1167
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
       2,     1,     1,     1,     0,     5,     0,     4,     1,     1,
       0,     1,     0,     1,     1,     1,     0,     2,     1,     3,
       3,     6,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       3,     0,     2,     2,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     1,     3,     3,     0,     2,     2,     6,     0,     2,
       0,     3,     0,     1,     1,     4,     1,     2,     1,     1,
       0,     1,     3,     1,     2,     1,     2,     2,     0,     1,
       1,     3,     1,     3,     0,     8,     1,     2,     1,     3,
       0,     3,     2,     4,     2,     0,     0,     5,     0,     0,
       5,     0,     0,     5,     0,     1,     1,     2,     5,     0,
       2,     2,     3,     1,     1,     2,     2,     2,     0,     1,
       1,     2,     8,     0,     3,     0,     4,     0,     4,     0,
       3,     0,     3,     1,     4,     2,     1,     1,     0,     2,
       4,     0,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     2,
       1,     1,     2,     3,     1,     3,     6,     2,     3,     2,
       1,     2,     2,     1,     2,     0,     1,     1,     4,     2,
       0,     1,     1,     0,     0,     4,     0,     0,     0,     9,
       0,     0,     3,     0,     3,     1,     2,     4,     0,     2,
       2,     0,     3,     3,     4,     3,     0,     1,     0,     2,
       0,     0,     7,     0,     2,     1,     1,     1,     2,     1,
       4,     2,     1,     1,     0,     1,     0,     0,     3,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     4,     4,     4,     3,     3,     3,     4,
       3,     4,     3,     3,     3,     4,     5,     3,     4,     3,
       3,     0,     3,     3,     2,     2,     2,     3,     3,     3,
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     0,     1,     0,     4,     4,     5,     6,     0,     2,
       0,     1,     0,     3,     3,     5,     0,     2,     2,     0,
       5,     0,     2,     0,     8,     0,     0,     3,     1,     2,
       2,     3,     0,     2,     2,     2,     0,     2,     2,     0,
       0,     3,     0,     0,     3,     0,     1,     0,     3,     0,
       2,     1,     0,     3,     0,     3,     0,     1,     3,     3,
       2,     1,     1,     0,     4,     4,     0,     1,     1,     1,
       1,     1,     0,     6,     0,     1,     0,     4,     0,     4,
       3,     3,     3,     3,     4,     6,     6,     6,     6,     0,
       2,     2,     1,     2,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     1,     1,     0,
       1,     0,     4,     4,     6,     6,     8,     8,     0,     1,
       0,     4,     0,     5,     1,     3,     1,     1,     1,     2,
       1,     2,     0,     3,     0,     0,     3,     2,     3,     1,
       3,     2,     1,     1,     1,     0,     2,     0,     1,     0,
       3,     0,     1,     1,     2,     1,     1,     0,     3,     0,
       3,     0,     5,     0,     3,     0,     2,     0,     0,     8,
       3,     0,     0,     3,     0,     1,     0,     7,     0,     2,
       0,     3,     3,     0,     2,     1,     2,     4,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     0,     3,     0,
       4,     1,     1,     1,     1,     2,     1,     1,     1,     0,
       3,     1,     2,     2,     2,     1,     1,     1,     2,     2,
       1,     2,     4,     2,     0,     1,     1,     1,     1,     4,
       5,     0,     4,     0,     1,     0,     3,     0,     3,     3,
       4,     0,     4,     4,     6,     0,     1,     0,     3,     0,
       5,     1,     1,     1,     1,     0,     3,     0,     3,     2,
       0,     3,     2,     0,     4,     2,     0,     1,     1,     3,
       0,     1,     2,     3,     3,     0,     3,     1,     3,     7,
       0,    10,     0,     2,     0,     2,     2,     3,     3,     2,
       0,     3,     0,     1,     1,     0,     1,     0,     4,     0,
       7,     0,     1,     0,     7,     0,     2,     3,     0,     1,
       1,     0,     4,     4,     0,     7,     0,     2,     0,     0,
       4,     1,     2,     0,     4,     0,     1,     0,     3,     1,
       1,     1,     1,     1,     4,     4,     3,     4,     1,     1,
       1,     2,     3,     1,     2,     3,     3,     0,     3,     0,
       7,     0,     6,     0,     2,     0,     2,     0,     3,     0,
       2,     4,     0,     2,     4,     0,     0,     7,     0,     4,
       2,     2,     2,     2,     2,     0,     1,     0,     4,     0,
       3,     0,     2,     2,     0,     8,     1,     2,     1,     3,
       3,     0,     3,     0,     1,     0,     4,     4,     6,     6,
       0,     1,     2,     0,     1,     0,     3,     0,     7,     0,
       4,     0,     1,     1,     0,     9,     0,     3,     1,     3,
       2,     2,     2,     3,     0,     3,     0,     3,     0,     3,
       0,     1,     1,     1,     1,     8,     0,     1,     1,     1,
       1,     1,     1,     0,     1,     0,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     5,     1,     2,     5,     0,
       8,     0,     2,     0,     4,     3,     3,     1,     1,     0,
       1,     1,     0,     1,     2,     2,     0,     0,     3,     0,
       0,     3,     2,     0,     0,     3,     0,     0,     3,     2,
       0,     0,     3,     0,     0,     3,     1,     1,     2,     0,
       3,     0,     3,     1,     1,     2,     0,     3,     0,     3,
       0,     1,     1,     1,     2,     0,     3,     0,     3,     0,
       3,     1,     1,     0,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     0,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     1,     2,     1,     1,     2,     1,     2,     1,
       5,     4,     1,     5,     4,     1,     3,     0,     1,     1,
       1,     3,     3,     3,     3,     2,     2,     3,     3,     1,
       3,     1,     2,     2,     1,     1,     1,     2,     1,     1,
       2,     1,     0,     2,     1,     1,     1,     3,     1,     1,
       2,     1,     1,     2,     0,     1,     1,     1,     1,     1,
       2,     1,     3,     1,     2,     1,     3,     3,     3,     4,
       3,     1,     1,     1,     1,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     3,     1,     3,     3,     4,     5,     1,     1,     2,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     5,     5,     5,     5,     5,     5,     5,     4,     5,
       2,     0,     4,     5,     0,     3,     0,     1,     1,     3,
       3,     1,     3,     1,     3,     0,     0,     1,     0,     1,
       0,     1,     0,     1,     1,     0,     1,     0,     1,     0,
       1,     0,     2,     1,     1,     2,     2,     2,     1,     2,
       1,     0,     1,     0,     1,     0,     1,     0,     1,     0,
       1,     0,     1,     0,     1,     0,     1,     0,     1,     0,
       1,     0,     1,     0,     1,     0,     1,     0,     1,     0,
       1,     0,     1,     0,     1,     0,     1,     1,     0,     1,
       0,     1,     1,     0,     1,     0,     1,     0,     1,     0,
       1,     0,     1,     0,     1,     0,     1,     2,     0,     1,
       0,     1,     0,     1,     0,     1,     0,     1,     0,     1,
       0,     1,     2,     0,     1,     0,     1,     0,     1,     0,
       1,     0,     1,     0,     1,     0,     1,     0,     1,     0,
       1,     1
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
#line 774 "parser.y"
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
#line 5173 "parser.c"
    break;

  case 3: /* start: $@1 nested_list "end of file"  */
#line 798 "parser.y"
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
#line 5195 "parser.c"
    break;

  case 8: /* $@2: %empty  */
#line 829 "parser.y"
                        { cb_validate_program_environment (current_program); }
#line 5201 "parser.c"
    break;

  case 9: /* $@3: %empty  */
#line 830 "parser.y"
                        { cb_validate_program_data (current_program); }
#line 5207 "parser.c"
    break;

  case 10: /* program_definition: identification_division environment_division $@2 data_division $@3 procedure_division nested_prog end_program  */
#line 833 "parser.y"
              {
	cb_tree file = current_program->file_list;
	for(; file; file = CB_CHAIN(file)) {
		cb_validate_indexed_file_key(CB_FILE(CB_VALUE(file)));
	}
  }
#line 5218 "parser.c"
    break;

  case 11: /* $@4: %empty  */
#line 843 "parser.y"
                        { cb_validate_program_environment (current_program); }
#line 5224 "parser.c"
    break;

  case 12: /* $@5: %empty  */
#line 844 "parser.y"
                        { cb_validate_program_data (current_program); }
#line 5230 "parser.c"
    break;

  case 13: /* program_mandatory: identification_division environment_division $@4 data_division $@5 procedure_division nested_prog end_mandatory  */
#line 847 "parser.y"
                {
	cb_tree file = current_program->file_list;
	for(; file; file = CB_CHAIN(file)) {
		cb_validate_indexed_file_key(CB_FILE(CB_VALUE(file)));
	}
  }
#line 5241 "parser.c"
    break;

  case 14: /* $@6: %empty  */
#line 857 "parser.y"
                        { cb_validate_program_environment (current_program); }
#line 5247 "parser.c"
    break;

  case 15: /* $@7: %empty  */
#line 858 "parser.y"
                        { cb_validate_program_data (current_program); }
#line 5253 "parser.c"
    break;

  case 21: /* end_program: "END PROGRAM" program_name '.'  */
#line 870 "parser.y"
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
#line 5282 "parser.c"
    break;

  case 22: /* end_mandatory: "END PROGRAM" program_name '.'  */
#line 898 "parser.y"
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
#line 5309 "parser.c"
    break;

  case 23: /* end_function: "END FUNCTION" program_name '.'  */
#line 924 "parser.y"
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
#line 5336 "parser.c"
    break;

  case 24: /* $@8: %empty  */
#line 955 "parser.y"
  {
	current_section = NULL;
	current_paragraph = NULL;
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
#line 5380 "parser.c"
    break;

  case 26: /* function_division: "FUNCTION-ID" '.' program_name as_literal '.'  */
#line 999 "parser.y"
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
#line 5425 "parser.c"
    break;

  case 29: /* as_literal: %empty  */
#line 1047 "parser.y"
                                { yyval = NULL; }
#line 5431 "parser.c"
    break;

  case 30: /* as_literal: AS "Literal"  */
#line 1048 "parser.y"
                                { yyval = yyvsp[0]; }
#line 5437 "parser.c"
    break;

  case 33: /* program_type_clause: COMMON  */
#line 1057 "parser.y"
  {
	if (!current_program->nested_level) {
		cb_error (_("COMMON may only be used in a nested program"));
	}
	current_program->flag_common = 1;
  }
#line 5448 "parser.c"
    break;

  case 34: /* program_type_clause: COMMON _init_or_recurs  */
#line 1064 "parser.y"
  {
	if (!current_program->nested_level) {
		cb_error (_("COMMON may only be used in a nested program"));
	}
	current_program->flag_common = 1;
  }
#line 5459 "parser.c"
    break;

  case 36: /* _init_or_recurs: "INITIAL"  */
#line 1075 "parser.y"
  {
	current_program->flag_initial = 1;
  }
#line 5467 "parser.c"
    break;

  case 37: /* _init_or_recurs: RECURSIVE  */
#line 1079 "parser.y"
  {
	current_program->flag_recursive = 1;
	current_program->flag_initial = 1;
  }
#line 5476 "parser.c"
    break;

  case 41: /* configuration_section: CONFIGURATION SECTION '.' configuration_list  */
#line 1104 "parser.y"
  {
	if (current_program->nested_level) {
		cb_error (_("CONFIGURATION SECTION not allowed in nested programs"));
	}
  }
#line 5486 "parser.c"
    break;

  case 53: /* with_debugging_mode: _with DEBUGGING MODE  */
#line 1139 "parser.y"
  {
	cb_verify (cb_debugging_line, "DEBUGGING MODE");
  }
#line 5494 "parser.c"
    break;

  case 54: /* computer_name: "Identifier"  */
#line 1145 "parser.y"
       { }
#line 5500 "parser.c"
    break;

  case 65: /* object_computer_memory: MEMORY SIZE _is integer object_char_or_word  */
#line 1176 "parser.y"
  {
	cb_verify (cb_memory_size_clause, "MEMORY SIZE");
  }
#line 5508 "parser.c"
    break;

  case 68: /* object_computer_sequence: _program coll_sequence _is reference  */
#line 1188 "parser.y"
  {
	current_program->collating_sequence = yyvsp[0];
  }
#line 5516 "parser.c"
    break;

  case 69: /* object_computer_segment: "SEGMENT-LIMIT" _is integer  */
#line 1195 "parser.y"
  {
	/* Ignore */
  }
#line 5524 "parser.c"
    break;

  case 75: /* repository_name: FUNCTION repository_literal_list INTRINSIC  */
#line 1220 "parser.y"
  {
	current_program->function_spec_list = yyvsp[-1];
  }
#line 5532 "parser.c"
    break;

  case 76: /* repository_name: FUNCTION ALL INTRINSIC  */
#line 1224 "parser.y"
  {
	functions_are_all = 1;
  }
#line 5540 "parser.c"
    break;

  case 77: /* repository_literal_list: "Literal"  */
#line 1230 "parser.y"
                        { yyval = cb_list_init (yyvsp[0]); }
#line 5546 "parser.c"
    break;

  case 78: /* repository_literal_list: repository_literal_list "Literal"  */
#line 1232 "parser.y"
                        { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5552 "parser.c"
    break;

  case 96: /* mnemonic_name_clause: "Identifier" _is CRT  */
#line 1273 "parser.y"
  {
	save_tree_1 = lookup_system_name (CB_NAME (yyvsp[-2]));
	if (save_tree_1 == cb_error_node) {
		cb_error_x (yyvsp[-2], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-2]));
	} else if (CB_SYSTEM_NAME(save_tree_1)->token != CB_DEVICE_CONSOLE) {
		cb_error_x (save_tree_1, _("Invalid CRT clause"));
	}
	/* current_program->flag_screen = 1; */
  }
#line 5566 "parser.c"
    break;

  case 97: /* $@9: %empty  */
#line 1283 "parser.y"
  {
	save_tree_1 = lookup_system_name (CB_NAME (yyvsp[-2]));
	if (save_tree_1 == cb_error_node) {
		cb_error_x (yyvsp[-2], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-2]));
	} else {
		cb_define (yyvsp[0], save_tree_1);
	}
	save_tree_2 = yyvsp[0];
  }
#line 5580 "parser.c"
    break;

  case 99: /* $@10: %empty  */
#line 1294 "parser.y"
  {
	save_tree_1 = lookup_system_name (CB_NAME (yyvsp[-1]));
	if (save_tree_1 == cb_error_node) {
		cb_error_x (yyvsp[-1], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-1]));
	}
	save_tree_2 = NULL;
  }
#line 5592 "parser.c"
    break;

  case 101: /* mnemonic_name_clause: "ARGUMENT-NUMBER" _is undefined_word  */
#line 1303 "parser.y"
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
#line 5610 "parser.c"
    break;

  case 102: /* mnemonic_name_clause: "ARGUMENT-VALUE" _is undefined_word  */
#line 1317 "parser.y"
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
#line 5628 "parser.c"
    break;

  case 103: /* mnemonic_name_clause: "ENVIRONMENT-NAME" _is undefined_word  */
#line 1331 "parser.y"
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
#line 5646 "parser.c"
    break;

  case 104: /* mnemonic_name_clause: "ENVIRONMENT-VALUE" _is undefined_word  */
#line 1345 "parser.y"
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
#line 5664 "parser.c"
    break;

  case 109: /* special_name_mnemonic_on_off: on_or_off _status _is undefined_word  */
#line 1371 "parser.y"
  {
	if (!save_tree_2 && !cb_switch_no_mnemonic) {
		cb_error_x (yyvsp[0], _("'%s' with no mnemonic name"), CB_NAME (yyvsp[0]));
	} else {
		cb_define_switch_name (yyvsp[0], save_tree_1, yyvsp[-3], save_tree_2);
	}
  }
#line 5676 "parser.c"
    break;

  case 110: /* on_or_off: ON  */
#line 1381 "parser.y"
                                { yyval = cb_int1; }
#line 5682 "parser.c"
    break;

  case 111: /* on_or_off: OFF  */
#line 1382 "parser.y"
                                { yyval = cb_int0; }
#line 5688 "parser.c"
    break;

  case 112: /* $@11: %empty  */
#line 1390 "parser.y"
  {
	save_tree_1 = yyvsp[0];
  }
#line 5696 "parser.c"
    break;

  case 113: /* alphabet_name_clause: ALPHABET undefined_word $@11 _is alphabet_definition  */
#line 1394 "parser.y"
  {
	current_program->alphabet_name_list =
		cb_list_add (current_program->alphabet_name_list, yyvsp[0]);
  }
#line 5705 "parser.c"
    break;

  case 114: /* alphabet_definition: NATIVE  */
#line 1401 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_NATIVE); }
#line 5711 "parser.c"
    break;

  case 115: /* alphabet_definition: "STANDARD-1"  */
#line 1402 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_STANDARD_1); }
#line 5717 "parser.c"
    break;

  case 116: /* alphabet_definition: "STANDARD-2"  */
#line 1403 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_STANDARD_2); }
#line 5723 "parser.c"
    break;

  case 117: /* alphabet_definition: EBCDIC  */
#line 1404 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_EBCDIC); }
#line 5729 "parser.c"
    break;

  case 118: /* alphabet_definition: alphabet_literal_list  */
#line 1406 "parser.y"
  {
	yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_CUSTOM);
	CB_ALPHABET_NAME (yyval)->custom_list = yyvsp[0];
  }
#line 5738 "parser.c"
    break;

  case 119: /* alphabet_literal_list: alphabet_literal  */
#line 1413 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5744 "parser.c"
    break;

  case 120: /* alphabet_literal_list: alphabet_literal_list alphabet_literal  */
#line 1415 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5750 "parser.c"
    break;

  case 121: /* alphabet_literal: alphabet_lits  */
#line 1419 "parser.y"
                                        { yyval = yyvsp[0]; }
#line 5756 "parser.c"
    break;

  case 122: /* alphabet_literal: alphabet_lits THRU alphabet_lits  */
#line 1420 "parser.y"
                                        { yyval = cb_build_pair (yyvsp[-2], yyvsp[0]); }
#line 5762 "parser.c"
    break;

  case 123: /* @12: %empty  */
#line 1422 "parser.y"
  {
	yyval = cb_list_init (yyvsp[-1]);
	save_tree_2 = yyval;
  }
#line 5771 "parser.c"
    break;

  case 124: /* alphabet_literal: alphabet_lits ALSO @12 alphabet_also_sequence  */
#line 1427 "parser.y"
  {
	yyval = yyvsp[-1];
  }
#line 5779 "parser.c"
    break;

  case 127: /* alphabet_lits: "Literal"  */
#line 1438 "parser.y"
                                { yyval = yyvsp[0]; }
#line 5785 "parser.c"
    break;

  case 128: /* alphabet_lits: SPACE  */
#line 1439 "parser.y"
                                { yyval = cb_space; }
#line 5791 "parser.c"
    break;

  case 129: /* alphabet_lits: ZERO  */
#line 1440 "parser.y"
                                { yyval = cb_zero; }
#line 5797 "parser.c"
    break;

  case 130: /* alphabet_lits: QUOTE  */
#line 1441 "parser.y"
                                { yyval = cb_quote; }
#line 5803 "parser.c"
    break;

  case 131: /* alphabet_lits: "HIGH-VALUE"  */
#line 1442 "parser.y"
                                { yyval = cb_norm_high; }
#line 5809 "parser.c"
    break;

  case 132: /* alphabet_lits: "LOW-VALUE"  */
#line 1443 "parser.y"
                                { yyval = cb_norm_low; }
#line 5815 "parser.c"
    break;

  case 133: /* alphabet_also_literal: "Literal"  */
#line 1447 "parser.y"
                                { cb_list_add (save_tree_2, yyvsp[0]); }
#line 5821 "parser.c"
    break;

  case 134: /* alphabet_also_literal: SPACE  */
#line 1448 "parser.y"
                                { cb_list_add (save_tree_2, cb_space); }
#line 5827 "parser.c"
    break;

  case 135: /* alphabet_also_literal: ZERO  */
#line 1449 "parser.y"
                                { cb_list_add (save_tree_2, cb_zero); }
#line 5833 "parser.c"
    break;

  case 136: /* alphabet_also_literal: QUOTE  */
#line 1450 "parser.y"
                                { cb_list_add (save_tree_2, cb_quote); }
#line 5839 "parser.c"
    break;

  case 137: /* alphabet_also_literal: "HIGH-VALUE"  */
#line 1451 "parser.y"
                                { cb_list_add (save_tree_2, cb_norm_high); }
#line 5845 "parser.c"
    break;

  case 138: /* alphabet_also_literal: "LOW-VALUE"  */
#line 1452 "parser.y"
                                { cb_list_add (save_tree_2, cb_norm_low); }
#line 5851 "parser.c"
    break;

  case 139: /* symbolic_characters_clause: SYMBOLIC _characters symbolic_characters_list  */
#line 1460 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->symbolic_list =
			cb_list_add (current_program->symbolic_list, yyvsp[0]);
	}
	PENDING ("SYMBOLIC CHARACTERS");
  }
#line 5863 "parser.c"
    break;

  case 140: /* symbolic_characters_list: char_list _is_are integer_list  */
#line 1471 "parser.y"
  {
	if (cb_list_length (yyvsp[-2]) != cb_list_length (yyvsp[0])) {
		cb_error (_("Invalid SYMBOLIC clause"));
		yyval = NULL;
	} else {
		yyval = NULL;
	}
  }
#line 5876 "parser.c"
    break;

  case 141: /* char_list: undefined_word  */
#line 1482 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5882 "parser.c"
    break;

  case 142: /* char_list: char_list undefined_word  */
#line 1483 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5888 "parser.c"
    break;

  case 143: /* integer_list: integer  */
#line 1487 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5894 "parser.c"
    break;

  case 144: /* integer_list: integer_list integer  */
#line 1488 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5900 "parser.c"
    break;

  case 145: /* class_name_clause: CLASS undefined_word _is class_item_list  */
#line 1496 "parser.y"
  {
	current_program->class_name_list =
			cb_list_add (current_program->class_name_list,
			cb_build_class_name (yyvsp[-2], yyvsp[0]));
  }
#line 5910 "parser.c"
    break;

  case 146: /* class_item_list: class_item  */
#line 1504 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5916 "parser.c"
    break;

  case 147: /* class_item_list: class_item_list class_item  */
#line 1505 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5922 "parser.c"
    break;

  case 148: /* class_item: basic_value  */
#line 1509 "parser.y"
                                { yyval = yyvsp[0]; }
#line 5928 "parser.c"
    break;

  case 149: /* class_item: basic_value THRU basic_value  */
#line 1511 "parser.y"
  {
	/* if (CB_LITERAL ($1)->data[0] < CB_LITERAL ($3)->data[0]) */
	if (literal_value (yyvsp[-2]) < literal_value (yyvsp[0])) {
		yyval = cb_build_pair (yyvsp[-2], yyvsp[0]);
	} else {
		yyval = cb_build_pair (yyvsp[0], yyvsp[-2]);
	}
  }
#line 5941 "parser.c"
    break;

  case 150: /* locale_clause: LOCALE undefined_word _is reference  */
#line 1525 "parser.y"
  {
	cb_tree	l;

	l = cb_build_locale_name (yyvsp[-2], yyvsp[0]);
	if (l != cb_error_node) {
		current_program->locale_list =
			cb_list_add (current_program->locale_list, l);
	}
  }
#line 5955 "parser.c"
    break;

  case 151: /* currency_sign_clause: CURRENCY _sign _is "Literal"  */
#line 1540 "parser.y"
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
#line 6020 "parser.c"
    break;

  case 152: /* decimal_point_clause: "DECIMAL-POINT" _is COMMA  */
#line 1607 "parser.y"
  {
	current_program->decimal_point = ',';
	current_program->numeric_separator = '.';
  }
#line 6029 "parser.c"
    break;

  case 153: /* cursor_clause: CURSOR _is reference  */
#line 1617 "parser.y"
                                { current_program->cursor_pos = yyvsp[0]; }
#line 6035 "parser.c"
    break;

  case 154: /* crt_status_clause: CRT STATUS _is reference  */
#line 1624 "parser.y"
                                { current_program->crt_status = yyvsp[0]; }
#line 6041 "parser.c"
    break;

  case 155: /* screen_control: "SCREEN-CONTROL" _is reference  */
#line 1631 "parser.y"
                                {  PENDING ("SCREEN CONTROL"); }
#line 6047 "parser.c"
    break;

  case 156: /* event_status: "EVENT-STATUS" _is reference  */
#line 1637 "parser.y"
                                {  PENDING ("EVENT STATUS"); }
#line 6053 "parser.c"
    break;

  case 159: /* $@13: %empty  */
#line 1649 "parser.y"
  {
	/* hack for MF compatibility */
	if (cb_relaxed_syntax_check) {
		cb_warning (_("INPUT-OUTPUT SECTION header missing - assumed"));
	} else {
		cb_error (_("INPUT-OUTPUT SECTION header missing"));
	}
  }
#line 6066 "parser.c"
    break;

  case 161: /* $@14: %empty  */
#line 1658 "parser.y"
  {
	/* hack for MF compatibility */
	if (cb_relaxed_syntax_check) {
		cb_warning (_("INPUT-OUTPUT SECTION header missing - assumed"));
	} else {
		cb_error (_("INPUT-OUTPUT SECTION header missing"));
	}
  }
#line 6079 "parser.c"
    break;

  case 167: /* $@15: %empty  */
#line 1683 "parser.y"
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
#line 6098 "parser.c"
    break;

  case 168: /* file_control_entry: SELECT flag_optional undefined_word $@15 select_clause_sequence '.'  */
#line 1698 "parser.y"
  {
	validate_file (current_file, yyvsp[-3]);
  }
#line 6106 "parser.c"
    break;

  case 186: /* assign_clause: ASSIGN _to _ext_clause _device assignment_name  */
#line 1730 "parser.y"
  {
	current_file->assign = cb_build_assignment_name (current_file, yyvsp[0]);
  }
#line 6114 "parser.c"
    break;

  case 187: /* assign_clause: ASSIGN _to _ext_clause DISK  */
#line 1734 "parser.y"
  {
	current_file->fileid_assign = 1;
	current_file->assign = cb_build_assignment_name (current_file, cb_build_reference ("DISK"));
  }
#line 6123 "parser.c"
    break;

  case 188: /* assign_clause: ASSIGN _to _ext_clause PRINTER  */
#line 1739 "parser.y"
  {
	current_file->fileid_assign = 1;
	current_file->assign = cb_build_assignment_name (current_file, cb_build_reference ("PRINTER"));
  }
#line 6132 "parser.c"
    break;

  case 191: /* _device: PRINTER  */
#line 1747 "parser.y"
                { current_file->organization = COB_ORG_LINE_SEQUENTIAL; }
#line 6138 "parser.c"
    break;

  case 193: /* _ext_clause: EXTERNAL  */
#line 1752 "parser.y"
  {
	current_file->external_assign = 1;
  }
#line 6146 "parser.c"
    break;

  case 194: /* _ext_clause: DYNAMIC  */
#line 1756 "parser.y"
  {
	current_file->external_assign = 0;
  }
#line 6154 "parser.c"
    break;

  case 196: /* assignment_name: DISPLAY  */
#line 1764 "parser.y"
  {
	const char	*s;

	s = "$#@DUMMY@#$";
	yyval = cb_build_alphanumeric_literal ((unsigned char *)s, strlen (s));
  }
#line 6165 "parser.c"
    break;

  case 197: /* assignment_name: _literal assignment_device_name_list  */
#line 1771 "parser.y"
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
#line 6186 "parser.c"
    break;

  case 198: /* assignment_device_name_list: qualified_word  */
#line 1790 "parser.y"
                                                { yyval = cb_list_init (yyvsp[0]); }
#line 6192 "parser.c"
    break;

  case 199: /* assignment_device_name_list: assignment_device_name_list qualified_word  */
#line 1791 "parser.y"
                                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 6198 "parser.c"
    break;

  case 201: /* access_mode: SEQUENTIAL  */
#line 1801 "parser.y"
                        { current_file->access_mode = COB_ACCESS_SEQUENTIAL; }
#line 6204 "parser.c"
    break;

  case 202: /* access_mode: DYNAMIC  */
#line 1802 "parser.y"
                        { current_file->access_mode = COB_ACCESS_DYNAMIC; }
#line 6210 "parser.c"
    break;

  case 203: /* access_mode: RANDOM  */
#line 1803 "parser.y"
                        { current_file->access_mode = COB_ACCESS_RANDOM; }
#line 6216 "parser.c"
    break;

  case 204: /* alternative_record_key_clause: ALTERNATE RECORD _key _is reference flag_duplicates  */
#line 1811 "parser.y"
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
#line 6240 "parser.c"
    break;

  case 205: /* alternative_record_key_clause: ALTERNATE RECORD _key _is reference key_is_eq split_key_list flag_duplicates  */
#line 1831 "parser.y"
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
#line 6284 "parser.c"
    break;

  case 206: /* $@16: %empty  */
#line 1873 "parser.y"
  {
	key_component_list = NULL;
  }
#line 6292 "parser.c"
    break;

  case 209: /* split_key: reference  */
#line 1882 "parser.y"
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
#line 6309 "parser.c"
    break;

  case 210: /* key_is_eq: %empty  */
#line 1897 "parser.y"
                { yyval = NULL; }
#line 6315 "parser.c"
    break;

  case 211: /* key_is_eq: SOURCE _is  */
#line 1898 "parser.y"
                { yyval = cb_int1; }
#line 6321 "parser.c"
    break;

  case 212: /* key_is_eq: '='  */
#line 1899 "parser.y"
                { yyval = cb_int('='); }
#line 6327 "parser.c"
    break;

  case 213: /* collating_sequence_clause: coll_sequence _is "Identifier"  */
#line 1906 "parser.y"
  {
	PENDING ("COLLATING SEQUENCE");
  }
#line 6335 "parser.c"
    break;

  case 214: /* file_status_clause: file_or_sort STATUS _is reference opt_reference  */
#line 1916 "parser.y"
  {
	current_file->file_status = yyvsp[-1];
	if (yyvsp[0]) {
		PENDING ("2nd FILE STATUS");
	}
  }
#line 6346 "parser.c"
    break;

  case 219: /* lock_mode: MANUAL lock_with  */
#line 1937 "parser.y"
                        { current_file->lock_mode = COB_LOCK_MANUAL; }
#line 6352 "parser.c"
    break;

  case 220: /* lock_mode: AUTOMATIC lock_with  */
#line 1938 "parser.y"
                        { current_file->lock_mode = COB_LOCK_AUTOMATIC; }
#line 6358 "parser.c"
    break;

  case 221: /* lock_mode: EXCLUSIVE  */
#line 1939 "parser.y"
                        { current_file->lock_mode = COB_LOCK_EXCLUSIVE; }
#line 6364 "parser.c"
    break;

  case 224: /* lock_with: WITH LOCK ON MULTIPLE lock_records  */
#line 1945 "parser.y"
  {
	current_file->lock_mode |= COB_LOCK_MULTIPLE;
  }
#line 6372 "parser.c"
    break;

  case 225: /* lock_with: WITH ROLLBACK  */
#line 1948 "parser.y"
                                { PENDING ("WITH ROLLBACK"); }
#line 6378 "parser.c"
    break;

  case 230: /* organization: INDEXED  */
#line 1965 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_INDEXED;
		organized_seen = 1;
	}
  }
#line 6391 "parser.c"
    break;

  case 231: /* organization: RECORD _binary SEQUENTIAL  */
#line 1974 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_SEQUENTIAL;
		organized_seen = 1;
	}
  }
#line 6404 "parser.c"
    break;

  case 232: /* organization: SEQUENTIAL  */
#line 1983 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = cb_default_organization;
		organized_seen = 1;
	}
  }
#line 6417 "parser.c"
    break;

  case 233: /* organization: RELATIVE  */
#line 1992 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_RELATIVE;
		organized_seen = 1;
	}
  }
#line 6430 "parser.c"
    break;

  case 234: /* organization: LINE SEQUENTIAL  */
#line 2001 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_LINE_SEQUENTIAL;
		organized_seen = 1;
	}
  }
#line 6443 "parser.c"
    break;

  case 235: /* padding_character_clause: PADDING _character _is reference_or_literal  */
#line 2016 "parser.y"
  {
	cb_verify (cb_padding_character_clause, "PADDING CHARACTER");
  }
#line 6451 "parser.c"
    break;

  case 236: /* record_delimiter_clause: RECORD DELIMITER _is "STANDARD-1"  */
#line 2025 "parser.y"
                                        { /* ignored */ }
#line 6457 "parser.c"
    break;

  case 237: /* record_key_clause: RECORD _key _is reference flag_duplicates  */
#line 2033 "parser.y"
  {

	if(yyvsp[0] == cb_int1) {
		cb_error (_("Record keys with duplicates are not yet supported"));
	}

	current_file->key = yyvsp[-1];
  }
#line 6470 "parser.c"
    break;

  case 238: /* record_key_clause: RECORD _key _is reference key_is_eq split_key_list flag_duplicates  */
#line 2042 "parser.y"
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
#line 6506 "parser.c"
    break;

  case 239: /* relative_key_clause: RELATIVE _key _is reference  */
#line 2079 "parser.y"
                                { current_file->key = yyvsp[0]; }
#line 6512 "parser.c"
    break;

  case 240: /* reserve_clause: RESERVE integer _area  */
#line 2086 "parser.y"
                                { /* ignored */ }
#line 6518 "parser.c"
    break;

  case 241: /* reserve_clause: RESERVE NO  */
#line 2087 "parser.y"
                                { /* ignored */ }
#line 6524 "parser.c"
    break;

  case 242: /* sharing_clause: SHARING _with sharing_option  */
#line 2094 "parser.y"
                                { current_file->sharing = yyvsp[0]; }
#line 6530 "parser.c"
    break;

  case 243: /* sharing_option: ALL _other  */
#line 2098 "parser.y"
                                { yyval = NULL; PENDING ("SHARING ALL OTHER"); }
#line 6536 "parser.c"
    break;

  case 244: /* sharing_option: NO _other  */
#line 2099 "parser.y"
                                { yyval = cb_int1; }
#line 6542 "parser.c"
    break;

  case 245: /* sharing_option: READ ONLY  */
#line 2100 "parser.y"
                                { yyval = cb_int0; }
#line 6548 "parser.c"
    break;

  case 246: /* nominal_key_clause: NOMINAL _key _is reference  */
#line 2106 "parser.y"
                                { PENDING ("NOMINAL KEY"); }
#line 6554 "parser.c"
    break;

  case 257: /* same_clause: SAME same_option _area _for file_name_list  */
#line 2137 "parser.y"
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
#line 6580 "parser.c"
    break;

  case 258: /* same_option: %empty  */
#line 2161 "parser.y"
                                { yyval = cb_int0; }
#line 6586 "parser.c"
    break;

  case 259: /* same_option: RECORD  */
#line 2162 "parser.y"
                                { yyval = cb_int1; }
#line 6592 "parser.c"
    break;

  case 260: /* same_option: SORT  */
#line 2163 "parser.y"
                                { yyval = cb_int2; }
#line 6598 "parser.c"
    break;

  case 261: /* same_option: "SORT-MERGE"  */
#line 2164 "parser.y"
                                { yyval = cb_int2; }
#line 6604 "parser.c"
    break;

  case 262: /* multiple_file_tape_clause: MULTIPLE _file _tape _contains multiple_file_list  */
#line 2171 "parser.y"
  {
	cb_verify (cb_multiple_file_tape_clause, "MULTIPLE FILE TAPE");
  }
#line 6612 "parser.c"
    break;

  case 265: /* multiple_file: file_name multiple_file_position  */
#line 2182 "parser.y"
                                   { }
#line 6618 "parser.c"
    break;

  case 271: /* apply_clause: APPLY "COMMITMENT-CONTROL" _on reference_list  */
#line 2199 "parser.y"
  {
	PENDING ("APPLY COMMITMENT-CONTROL");
  }
#line 6626 "parser.c"
    break;

  case 272: /* apply_clause: APPLY "CYL-OVERFLOW" _of "Literal" TRACKS ON reference_list  */
#line 2203 "parser.y"
  {
	PENDING ("APPLY CYL-OVERFLOW");
  }
#line 6634 "parser.c"
    break;

  case 273: /* apply_clause: APPLY "CORE-INDEX" TO reference ON reference_list  */
#line 2207 "parser.y"
  {
	PENDING ("APPLY CORE-INDEX");
  }
#line 6642 "parser.c"
    break;

  case 274: /* apply_clause: APPLY "FORMS-OVERLAY" TO reference ON reference_list  */
#line 2211 "parser.y"
  {
	PENDING ("APPLY FORMS-OVERLAY");
  }
#line 6650 "parser.c"
    break;

  case 275: /* apply_clause: APPLY "CLOSE-NOFEED" ON reference_list  */
#line 2215 "parser.y"
  {
	PENDING ("APPLY CLOSE-NOFEED");
  }
#line 6658 "parser.c"
    break;

  case 279: /* $@17: %empty  */
#line 2240 "parser.y"
                                { current_storage = CB_STORAGE_FILE; }
#line 6664 "parser.c"
    break;

  case 281: /* $@18: %empty  */
#line 2243 "parser.y"
  {
	/* hack for MF compatibility */
	if (cb_relaxed_syntax_check) {
		cb_warning (_("FILE SECTION header missing - assumed"));
	} else {
		cb_error (_("FILE SECTION header missing"));
	}
	current_storage = CB_STORAGE_FILE;
  }
#line 6678 "parser.c"
    break;

  case 285: /* file_description: file_type file_description_entry record_description_list  */
#line 2262 "parser.y"
  {
	if (yyvsp[0] && yyvsp[0] != cb_error_node) {
		finalize_file (current_file, CB_FIELD (yyvsp[0]));
	} else {
		cb_error (_("RECORD description missing or invalid"));
	}
  }
#line 6690 "parser.c"
    break;

  case 286: /* file_description_sequence_without_type: file_description_entry record_description_list  */
#line 2274 "parser.y"
  {
	if (yyvsp[0] && yyvsp[0] != cb_error_node) {
		finalize_file (current_file, CB_FIELD (yyvsp[0]));
	} else {
		cb_error (_("RECORD description missing or invalid"));
	}
  }
#line 6702 "parser.c"
    break;

  case 288: /* file_type: FD  */
#line 2285 "parser.y"
                               { yyval = cb_int0; }
#line 6708 "parser.c"
    break;

  case 289: /* file_type: SD  */
#line 2286 "parser.y"
                               { yyval = cb_int1; }
#line 6714 "parser.c"
    break;

  case 290: /* @19: %empty  */
#line 2296 "parser.y"
  {
	if (yyvsp[0] == cb_error_node) {
		YYERROR;
	}

	current_file = CB_FILE (cb_ref (yyvsp[0]));
	if (yyvsp[-1] == cb_int1) {
		current_file->organization = COB_ORG_SORT;
	}
  }
#line 6729 "parser.c"
    break;

  case 291: /* file_description_entry: file_name @19 file_description_clause_sequence '.'  */
#line 2307 "parser.y"
  {
	/* Shut up bison */
	dummy_tree = yyvsp[-2];
  }
#line 6738 "parser.c"
    break;

  case 294: /* file_description_clause: _is EXTERNAL  */
#line 2319 "parser.y"
  {
	if (current_file->global) {
		cb_error (_("File cannot have both EXTERNAL and GLOBAL clauses"));
	}
	current_file->external = 1;
  }
#line 6749 "parser.c"
    break;

  case 295: /* file_description_clause: _is GLOBAL  */
#line 2326 "parser.y"
  {
	if (current_file->external) {
		cb_error (_("File cannot have both EXTERNAL and GLOBAL clauses"));
	}
	current_file->global = 1;
  }
#line 6760 "parser.c"
    break;

  case 306: /* block_contains_clause: BLOCK _contains integer opt_to_integer _records_or_characters  */
#line 2349 "parser.y"
  { /* ignored */ }
#line 6766 "parser.c"
    break;

  case 310: /* record_clause: RECORD _contains integer _characters  */
#line 2359 "parser.y"
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
#line 6782 "parser.c"
    break;

  case 311: /* record_clause: RECORD _contains integer TO integer _characters  */
#line 2371 "parser.y"
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
#line 6811 "parser.c"
    break;

  case 312: /* record_clause: RECORD _is VARYING _in _size opt_from_integer opt_to_integer _characters record_depending  */
#line 2397 "parser.y"
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
#line 6836 "parser.c"
    break;

  case 314: /* record_depending: DEPENDING _on reference  */
#line 2421 "parser.y"
  {
	current_file->record_depending = yyvsp[0];
  }
#line 6844 "parser.c"
    break;

  case 315: /* opt_from_integer: %empty  */
#line 2427 "parser.y"
                                { yyval = NULL; }
#line 6850 "parser.c"
    break;

  case 316: /* opt_from_integer: _from integer  */
#line 2428 "parser.y"
                                { yyval = yyvsp[0]; }
#line 6856 "parser.c"
    break;

  case 317: /* opt_to_integer: %empty  */
#line 2432 "parser.y"
                                { yyval = NULL; }
#line 6862 "parser.c"
    break;

  case 318: /* opt_to_integer: TO integer  */
#line 2433 "parser.y"
                                { yyval = yyvsp[0]; }
#line 6868 "parser.c"
    break;

  case 319: /* label_records_clause: LABEL records label_option  */
#line 2441 "parser.y"
  {
	cb_verify (cb_label_records_clause, "LABEL RECORDS");
  }
#line 6876 "parser.c"
    break;

  case 322: /* value_of_clause: VALUE OF "Identifier" _is valueof_name  */
#line 2456 "parser.y"
  {
	cb_verify (cb_value_of_clause, "VALUE OF");
  }
#line 6884 "parser.c"
    break;

  case 323: /* value_of_clause: VALUE OF "FILE-ID" _is valueof_name  */
#line 2460 "parser.y"
  {
	if (!current_file->assign) {
		current_file->assign = cb_build_assignment_name (current_file, yyvsp[0]);
	}
  }
#line 6894 "parser.c"
    break;

  case 326: /* data_records_clause: DATA records no_reference_list  */
#line 2476 "parser.y"
  {
	cb_verify (cb_data_records_clause, "DATA RECORDS");
  }
#line 6902 "parser.c"
    break;

  case 327: /* linage_clause: LINAGE _is reference_or_literal _lines linage_sequence  */
#line 2487 "parser.y"
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
#line 6921 "parser.c"
    break;

  case 333: /* linage_footing: _with FOOTING _at reference_or_literal _lines  */
#line 2515 "parser.y"
  {
	current_file->latfoot = yyvsp[-1];
  }
#line 6929 "parser.c"
    break;

  case 334: /* linage_top: _at TOP reference_or_literal _lines  */
#line 2522 "parser.y"
  {
	current_file->lattop = yyvsp[-1];
  }
#line 6937 "parser.c"
    break;

  case 335: /* linage_bottom: _at BOTTOM reference_or_literal  */
#line 2529 "parser.y"
  {
	current_file->latbot = yyvsp[0];
  }
#line 6945 "parser.c"
    break;

  case 336: /* recording_mode_clause: RECORDING _mode _is "Identifier"  */
#line 2538 "parser.y"
                                { /* ignore */ }
#line 6951 "parser.c"
    break;

  case 337: /* code_set_clause: "CODE-SET" _is "Identifier"  */
#line 2546 "parser.y"
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
#line 6968 "parser.c"
    break;

  case 338: /* report_clause: REPORT _is report_name  */
#line 2564 "parser.y"
  {
	cb_warning (_("file descriptor REPORT IS"));
  }
#line 6976 "parser.c"
    break;

  case 339: /* report_clause: REPORTS _are report_name  */
#line 2568 "parser.y"
  {
	cb_warning (_("file descriptor REPORTS ARE"));
  }
#line 6984 "parser.c"
    break;

  case 341: /* $@20: %empty  */
#line 2579 "parser.y"
                                { current_storage = CB_STORAGE_WORKING; }
#line 6990 "parser.c"
    break;

  case 342: /* working_storage_section: "WORKING-STORAGE" SECTION '.' $@20 record_description_list  */
#line 2581 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->working_storage =
			cb_field_add (current_program->working_storage, CB_FIELD (yyvsp[0]));
	}
  }
#line 7001 "parser.c"
    break;

  case 343: /* record_description_list: %empty  */
#line 2590 "parser.y"
                                { yyval = NULL; }
#line 7007 "parser.c"
    break;

  case 344: /* record_description_list: record_description_list_1  */
#line 2591 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7013 "parser.c"
    break;

  case 345: /* $@21: %empty  */
#line 2595 "parser.y"
  {
	current_field = NULL;
	description_field = NULL;
	cb_clear_real_field ();
  }
#line 7023 "parser.c"
    break;

  case 346: /* record_description_list_1: $@21 record_description_list_2  */
#line 2601 "parser.y"
  {
	struct cb_field *p;

	for (p = description_field; p; p = p->sister) {
		cb_validate_field (p);
	}
	yyval = CB_TREE (description_field);
  }
#line 7036 "parser.c"
    break;

  case 352: /* exec_sql_data_statement: "EXEC SQL statement"  */
#line 2622 "parser.y"
  {
	/* Handle EXEC SQL in DATA DIVISION (BEGIN/END DECLARE SECTION, INCLUDE SQLCA) */
	const char *sql_text = (const char *)CB_LITERAL (yyvsp[0])->data;
	/* BEGIN DECLARE SECTION and END DECLARE SECTION are no-ops */
	/* INCLUDE SQLCA is handled by pplex via COPY */
	/* Silently ignore these in data division */
	(void)sql_text;
  }
#line 7049 "parser.c"
    break;

  case 354: /* $@22: %empty  */
#line 2635 "parser.y"
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
#line 7065 "parser.c"
    break;

  case 355: /* data_description: level_number entry_name $@22 data_description_clause_sequence _maybe_next_level_number  */
#line 2647 "parser.y"
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
#line 7082 "parser.c"
    break;

  case 356: /* $@23: %empty  */
#line 2660 "parser.y"
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
#line 7098 "parser.c"
    break;

  case 357: /* data_description: level_number_88 entry_name $@23 value_cond_clause  */
#line 2672 "parser.y"
  {
	if (!qualifier) {
		cb_error (_("Item requires a data name"));
	}
	cb_validate_88_item (current_field);
	if (!description_field) {
		description_field = current_field;
	}
	
  }
#line 7113 "parser.c"
    break;

  case 361: /* _maybe_next_level_number: "Literal"  */
#line 2699 "parser.y"
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
#line 7135 "parser.c"
    break;

  case 362: /* entry_name: %empty  */
#line 2720 "parser.y"
  {
	yyval = cb_build_filler ();
	qualifier = NULL;
	non_const_word = 0;
  }
#line 7145 "parser.c"
    break;

  case 363: /* entry_name: FILLER  */
#line 2726 "parser.y"
  {
	yyval = cb_build_filler ();
	qualifier = NULL;
	non_const_word = 0;
  }
#line 7155 "parser.c"
    break;

  case 364: /* entry_name: "Identifier"  */
#line 2732 "parser.y"
  {
	yyval = yyvsp[0];
	qualifier = yyvsp[0];
	non_const_word = 0;
  }
#line 7165 "parser.c"
    break;

  case 365: /* const_name: "Identifier"  */
#line 2741 "parser.y"
  {
	yyval = yyvsp[0];
	qualifier = yyvsp[0];
	non_const_word = 0;
  }
#line 7175 "parser.c"
    break;

  case 367: /* const_global: _is GLOBAL  */
#line 2750 "parser.y"
  {
	current_field->flag_is_global = 1;
	cb_error (_("CONSTANT with GLOBAL clause is not yet supported"));
  }
#line 7184 "parser.c"
    break;

  case 368: /* lit_or_length: literal  */
#line 2757 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7190 "parser.c"
    break;

  case 369: /* lit_or_length: LENGTH _of identifier_1  */
#line 2758 "parser.y"
                                { yyval = cb_build_const_length (yyvsp[0]); }
#line 7196 "parser.c"
    break;

  case 370: /* lit_or_length: "BYTE-LENGTH" _of identifier_1  */
#line 2759 "parser.y"
                                { yyval = cb_build_const_length (yyvsp[0]); }
#line 7202 "parser.c"
    break;

  case 371: /* constant_entry: level_number const_name CONSTANT const_global _as lit_or_length  */
#line 2764 "parser.y"
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
#line 7222 "parser.c"
    break;

  case 372: /* data_description_clause_sequence: %empty  */
#line 2783 "parser.y"
  {
	/* required to check redefines */
	yyval = NULL;
  }
#line 7231 "parser.c"
    break;

  case 373: /* data_description_clause_sequence: data_description_clause_sequence data_description_clause  */
#line 2789 "parser.y"
  {
	/* required to check redefines */
	yyval = cb_true;
  }
#line 7240 "parser.c"
    break;

  case 389: /* redefines_clause: REDEFINES identifier_1  */
#line 2818 "parser.y"
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
#line 7260 "parser.c"
    break;

  case 390: /* external_clause: _is EXTERNAL as_extname  */
#line 2840 "parser.y"
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
#line 7283 "parser.c"
    break;

  case 391: /* as_extname: %empty  */
#line 2861 "parser.y"
                                { current_field->ename = NULL; }
#line 7289 "parser.c"
    break;

  case 392: /* as_extname: AS "Literal"  */
#line 2863 "parser.y"
 {
	struct cb_field *x;

	x = CB_FIELD(cb_build_field (cb_build_reference ((char *)(CB_LITERAL (yyvsp[0])->data))));
	current_field->ename = x->name;
 }
#line 7300 "parser.c"
    break;

  case 393: /* global_clause: _is GLOBAL  */
#line 2875 "parser.y"
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
#line 7318 "parser.c"
    break;

  case 394: /* picture_clause: PICTURE  */
#line 2894 "parser.y"
                                { current_field->pic = CB_PICTURE (yyvsp[0]); }
#line 7324 "parser.c"
    break;

  case 397: /* usage: BINARY  */
#line 2906 "parser.y"
                                { current_field->usage = CB_USAGE_BINARY; }
#line 7330 "parser.c"
    break;

  case 398: /* usage: COMP  */
#line 2907 "parser.y"
                                { current_field->usage = CB_USAGE_BINARY; }
#line 7336 "parser.c"
    break;

  case 399: /* usage: "COMP-1"  */
#line 2909 "parser.y"
{
	current_field->usage = CB_USAGE_FLOAT; 
	cb_error (_("COMP-1 not implemented"));
}
#line 7345 "parser.c"
    break;

  case 400: /* usage: "COMP-2"  */
#line 2914 "parser.y"
{ 
	current_field->usage = CB_USAGE_DOUBLE; 
	cb_error (_("COMP-2 not implemented"));
}
#line 7354 "parser.c"
    break;

  case 401: /* usage: "COMP-3"  */
#line 2918 "parser.y"
                                { current_field->usage = CB_USAGE_PACKED; }
#line 7360 "parser.c"
    break;

  case 402: /* usage: "COMP-4"  */
#line 2919 "parser.y"
                                { current_field->usage = CB_USAGE_BINARY; }
#line 7366 "parser.c"
    break;

  case 403: /* usage: "COMP-5"  */
#line 2920 "parser.y"
                                { current_field->usage = CB_USAGE_COMP_5; }
#line 7372 "parser.c"
    break;

  case 404: /* usage: "COMP-X"  */
#line 2921 "parser.y"
                                { current_field->usage = CB_USAGE_COMP_X; }
#line 7378 "parser.c"
    break;

  case 405: /* usage: DISPLAY  */
#line 2922 "parser.y"
                                { current_field->usage = CB_USAGE_DISPLAY; }
#line 7384 "parser.c"
    break;

  case 406: /* usage: INDEX  */
#line 2923 "parser.y"
                                { current_field->usage = CB_USAGE_INDEX; }
#line 7390 "parser.c"
    break;

  case 407: /* usage: "PACKED-DECIMAL"  */
#line 2924 "parser.y"
                                { current_field->usage = CB_USAGE_PACKED; }
#line 7396 "parser.c"
    break;

  case 408: /* usage: POINTER  */
#line 2926 "parser.y"
  {
	current_field->usage = CB_USAGE_POINTER;
	current_field->flag_is_pointer = 1;
  }
#line 7405 "parser.c"
    break;

  case 409: /* usage: "PROGRAM-POINTER"  */
#line 2931 "parser.y"
  {
	current_field->usage = CB_USAGE_PROGRAM_POINTER;
	current_field->flag_is_pointer = 1;
  }
#line 7414 "parser.c"
    break;

  case 410: /* usage: "SIGNED-SHORT"  */
#line 2935 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_SHORT; }
#line 7420 "parser.c"
    break;

  case 411: /* usage: "SIGNED-INT"  */
#line 2936 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_INT; }
#line 7426 "parser.c"
    break;

  case 412: /* usage: "SIGNED-LONG"  */
#line 2937 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_LONG; }
#line 7432 "parser.c"
    break;

  case 413: /* usage: "UNSIGNED-SHORT"  */
#line 2938 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_SHORT; }
#line 7438 "parser.c"
    break;

  case 414: /* usage: "UNSIGNED-INT"  */
#line 2939 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_INT; }
#line 7444 "parser.c"
    break;

  case 415: /* usage: "UNSIGNED-LONG"  */
#line 2940 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_LONG; }
#line 7450 "parser.c"
    break;

  case 416: /* usage: "BINARY-CHAR" SIGNED  */
#line 2941 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_CHAR; }
#line 7456 "parser.c"
    break;

  case 417: /* usage: "BINARY-CHAR" UNSIGNED  */
#line 2942 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_CHAR; }
#line 7462 "parser.c"
    break;

  case 418: /* usage: "BINARY-CHAR"  */
#line 2943 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_CHAR; }
#line 7468 "parser.c"
    break;

  case 419: /* usage: "BINARY-SHORT" SIGNED  */
#line 2944 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_SHORT; }
#line 7474 "parser.c"
    break;

  case 420: /* usage: "BINARY-SHORT" UNSIGNED  */
#line 2945 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_SHORT; }
#line 7480 "parser.c"
    break;

  case 421: /* usage: "BINARY-SHORT"  */
#line 2946 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_SHORT; }
#line 7486 "parser.c"
    break;

  case 422: /* usage: "BINARY-LONG" SIGNED  */
#line 2947 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_INT; }
#line 7492 "parser.c"
    break;

  case 423: /* usage: "BINARY-LONG" UNSIGNED  */
#line 2948 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_INT; }
#line 7498 "parser.c"
    break;

  case 424: /* usage: "BINARY-LONG"  */
#line 2949 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_INT; }
#line 7504 "parser.c"
    break;

  case 425: /* usage: "BINARY-DOUBLE" SIGNED  */
#line 2950 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_LONG; }
#line 7510 "parser.c"
    break;

  case 426: /* usage: "BINARY-DOUBLE" UNSIGNED  */
#line 2951 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_LONG; }
#line 7516 "parser.c"
    break;

  case 427: /* usage: "BINARY-DOUBLE"  */
#line 2952 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_LONG; }
#line 7522 "parser.c"
    break;

  case 428: /* usage: "BINARY-C-LONG" SIGNED  */
#line 2954 "parser.y"
  {
	if (sizeof(long) == 4) {
		current_field->usage = CB_USAGE_SIGNED_INT;
	} else {
		current_field->usage = CB_USAGE_SIGNED_LONG;
	}
  }
#line 7534 "parser.c"
    break;

  case 429: /* usage: "BINARY-C-LONG" UNSIGNED  */
#line 2962 "parser.y"
  {
	if (sizeof(long) == 4) {
		current_field->usage = CB_USAGE_UNSIGNED_INT;
	} else {
		current_field->usage = CB_USAGE_UNSIGNED_LONG;
	}
  }
#line 7546 "parser.c"
    break;

  case 430: /* usage: "BINARY-C-LONG"  */
#line 2970 "parser.y"
  {
	if (sizeof(long) == 4) {
		current_field->usage = CB_USAGE_SIGNED_INT;
	} else {
		current_field->usage = CB_USAGE_SIGNED_LONG;
	}
  }
#line 7558 "parser.c"
    break;

  case 431: /* usage: NATIONAL  */
#line 2977 "parser.y"
                                { PENDING ("USAGE NATIONAL");}
#line 7564 "parser.c"
    break;

  case 432: /* sign_clause: _sign_is LEADING flag_separate  */
#line 2985 "parser.y"
  {
	current_field->flag_sign_separate = CB_INTEGER (yyvsp[0])->val;
	current_field->flag_sign_leading  = 1;
  }
#line 7573 "parser.c"
    break;

  case 433: /* sign_clause: _sign_is TRAILING flag_separate  */
#line 2990 "parser.y"
  {
	current_field->flag_sign_separate = CB_INTEGER (yyvsp[0])->val;
	current_field->flag_sign_leading  = 0;
  }
#line 7582 "parser.c"
    break;

  case 437: /* occurs_clause: OCCURS integer occurs_to_integer _times occurs_depending occurs_key_spec  */
#line 3007 "parser.y"
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
#line 7599 "parser.c"
    break;

  case 438: /* occurs_to_integer: %empty  */
#line 3022 "parser.y"
                                { yyval = NULL; }
#line 7605 "parser.c"
    break;

  case 439: /* occurs_to_integer: TO integer  */
#line 3023 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7611 "parser.c"
    break;

  case 441: /* occurs_depending: DEPENDING _on reference  */
#line 3028 "parser.y"
  {
	current_field->occurs_depending = yyvsp[0];
  }
#line 7619 "parser.c"
    break;

  case 444: /* occurs_keys: occurs_key_list  */
#line 3037 "parser.y"
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
#line 7644 "parser.c"
    break;

  case 445: /* occurs_key: ascending_or_descending _key _is reference_list  */
#line 3061 "parser.y"
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
#line 7661 "parser.c"
    break;

  case 446: /* occurs_key_list: occurs_key  */
#line 3076 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7667 "parser.c"
    break;

  case 447: /* occurs_key_list: occurs_key_list occurs_key  */
#line 3077 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 7673 "parser.c"
    break;

  case 448: /* ascending_or_descending: ASCENDING  */
#line 3081 "parser.y"
                                { yyval = cb_int (COB_ASCENDING); }
#line 7679 "parser.c"
    break;

  case 449: /* ascending_or_descending: DESCENDING  */
#line 3082 "parser.y"
                                { yyval = cb_int (COB_DESCENDING); }
#line 7685 "parser.c"
    break;

  case 452: /* occurs_indexed: INDEXED _by occurs_index_list  */
#line 3089 "parser.y"
  {
	current_field->index_list = yyvsp[0];
  }
#line 7693 "parser.c"
    break;

  case 453: /* occurs_index_list: occurs_index  */
#line 3095 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 7699 "parser.c"
    break;

  case 454: /* occurs_index_list: occurs_index_list occurs_index  */
#line 3097 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 7705 "parser.c"
    break;

  case 455: /* occurs_index: "Identifier"  */
#line 3102 "parser.y"
  {
	yyval = cb_build_index (yyvsp[0], cb_int1, 1, current_field);
  }
#line 7713 "parser.c"
    break;

  case 456: /* justified_clause: JUSTIFIED _right  */
#line 3111 "parser.y"
                                { current_field->flag_justified = 1; }
#line 7719 "parser.c"
    break;

  case 457: /* synchronized_clause: SYNCHRONIZED left_or_right  */
#line 3118 "parser.y"
                                { current_field->flag_synchronized = 1; }
#line 7725 "parser.c"
    break;

  case 461: /* blank_clause: BLANK _when ZERO  */
#line 3130 "parser.y"
                                { current_field->flag_blank_zero = 1; }
#line 7731 "parser.c"
    break;

  case 462: /* based_clause: BASED  */
#line 3138 "parser.y"
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
#line 7755 "parser.c"
    break;

  case 463: /* value_clause: VALUE _is literal  */
#line 3162 "parser.y"
                                { current_field->values = cb_list_init (yyvsp[0]); }
#line 7761 "parser.c"
    break;

  case 464: /* $@24: %empty  */
#line 3166 "parser.y"
                                { current_field->values = yyvsp[0]; }
#line 7767 "parser.c"
    break;

  case 466: /* value_item_list: value_item  */
#line 3171 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 7773 "parser.c"
    break;

  case 467: /* value_item_list: value_item_list value_item  */
#line 3172 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 7779 "parser.c"
    break;

  case 468: /* value_item: literal  */
#line 3176 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7785 "parser.c"
    break;

  case 469: /* value_item: literal THRU literal  */
#line 3177 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-2], yyvsp[0]); }
#line 7791 "parser.c"
    break;

  case 471: /* false_is: "FALSE" _is literal  */
#line 3182 "parser.y"
  {
	if (current_field->level != 88) {
		cb_error (_("FALSE clause only allowed for 88 level"));
	}
	current_field->false_88 = cb_list_init (yyvsp[0]);
  }
#line 7802 "parser.c"
    break;

  case 472: /* renames_clause: RENAMES qualified_word  */
#line 3195 "parser.y"
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
#line 7818 "parser.c"
    break;

  case 473: /* renames_clause: RENAMES qualified_word THRU qualified_word  */
#line 3207 "parser.y"
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
#line 7837 "parser.c"
    break;

  case 474: /* any_length_clause: ANY LENGTH  */
#line 3227 "parser.y"
  {
	if (current_field->flag_item_based) {
		cb_error (_("BASED and ANY LENGTH are mutually exclusive"));
	} else {
		current_field->flag_any_length = 1;
	}
  }
#line 7849 "parser.c"
    break;

  case 476: /* $@25: %empty  */
#line 3242 "parser.y"
  {
	cb_error (_("LOCAL-STORAGE SECTION is not supported"));
	current_storage = CB_STORAGE_LOCAL;
	if (current_program->nested_level) {
		cb_error (_("LOCAL-STORAGE not allowed in nested programs"));
	}
  }
#line 7861 "parser.c"
    break;

  case 477: /* local_storage_section: "LOCAL-STORAGE" SECTION '.' $@25 record_description_list  */
#line 3250 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->local_storage = CB_FIELD (yyvsp[0]);
	}
  }
#line 7871 "parser.c"
    break;

  case 479: /* $@26: %empty  */
#line 3263 "parser.y"
                                { current_storage = CB_STORAGE_LINKAGE; }
#line 7877 "parser.c"
    break;

  case 480: /* linkage_section: LINKAGE SECTION '.' $@26 record_description_list  */
#line 3265 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->linkage_storage = CB_FIELD (yyvsp[0]);
	}
  }
#line 7887 "parser.c"
    break;

  case 482: /* $@27: %empty  */
#line 3278 "parser.y"
  {
	cb_error (_("REPORT SECTION not supported"));
	current_storage = CB_STORAGE_REPORT;
  }
#line 7896 "parser.c"
    break;

  case 489: /* report_description_options: %empty  */
#line 3311 "parser.y"
  {
	cb_warning (_("Report description using defaults"));
  }
#line 7904 "parser.c"
    break;

  case 491: /* report_description_option: _is GLOBAL  */
#line 3319 "parser.y"
  {
	cb_error (_("GLOBAL is not allowed with RD"));
  }
#line 7912 "parser.c"
    break;

  case 500: /* identifier_list: identifier  */
#line 3342 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 7918 "parser.c"
    break;

  case 501: /* identifier_list: identifier_list identifier  */
#line 3343 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 7924 "parser.c"
    break;

  case 523: /* report_group_option: type_clause  */
#line 3399 "parser.y"
              { cb_warning (_("looking for Report line TYPE")); }
#line 7930 "parser.c"
    break;

  case 574: /* $@28: %empty  */
#line 3504 "parser.y"
                                { current_storage = CB_STORAGE_SCREEN; }
#line 7936 "parser.c"
    break;

  case 575: /* screen_section: SCREEN SECTION '.' $@28  */
#line 3505 "parser.y"
  {
	cb_error (_("SCREEN SECTION is not supported"));
  }
#line 7944 "parser.c"
    break;

  case 577: /* $@29: %empty  */
#line 3516 "parser.y"
  {
	current_section = NULL;
	current_paragraph = NULL;
	cb_define_system_name ("CONSOLE");
	cb_define_system_name ("SYSIN");
	cb_define_system_name ("SYSOUT");
	cb_define_system_name ("SYSERR");
	cb_set_in_procedure ();
  }
#line 7958 "parser.c"
    break;

  case 578: /* $@30: %empty  */
#line 3526 "parser.y"
  {
	if (current_program->flag_main && !current_program->flag_chained && yyvsp[-4]) {
		cb_error (_("Executable program requested but PROCEDURE/ENTRY has USING clause"));
	}
	emit_entry (current_program->program_id, 0, yyvsp[-4]); /* main entry point */
	if (current_program->source_name) {
		emit_entry (current_program->source_name, 1, yyvsp[-4]);
	}
  }
#line 7972 "parser.c"
    break;

  case 579: /* procedure_division: PROCEDURE DIVISION procedure_using_chaining procedure_returning '.' $@29 procedure_declaratives $@30 procedure_list  */
#line 3536 "parser.y"
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
#line 7991 "parser.c"
    break;

  case 580: /* procedure_using_chaining: %empty  */
#line 3553 "parser.y"
                                { yyval = NULL; }
#line 7997 "parser.c"
    break;

  case 581: /* $@31: %empty  */
#line 3555 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
	size_mode = CB_SIZE_4;
  }
#line 8006 "parser.c"
    break;

  case 582: /* procedure_using_chaining: USING $@31 procedure_param_list  */
#line 3559 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8012 "parser.c"
    break;

  case 583: /* $@32: %empty  */
#line 3561 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
	current_program->flag_chained = 1;
  }
#line 8021 "parser.c"
    break;

  case 584: /* procedure_using_chaining: CHAINING $@32 procedure_param_list  */
#line 3565 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8027 "parser.c"
    break;

  case 585: /* procedure_param_list: procedure_param  */
#line 3569 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8033 "parser.c"
    break;

  case 586: /* procedure_param_list: procedure_param_list procedure_param  */
#line 3571 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 8039 "parser.c"
    break;

  case 587: /* procedure_param: procedure_type size_optional procedure_optional "Identifier"  */
#line 3576 "parser.y"
  {
	yyval = cb_build_pair (cb_int (call_mode), cb_build_identifier (yyvsp[0]));
	CB_SIZES (yyval) = size_mode;
  }
#line 8048 "parser.c"
    break;

  case 589: /* procedure_type: _by REFERENCE  */
#line 3585 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
  }
#line 8056 "parser.c"
    break;

  case 590: /* procedure_type: _by VALUE  */
#line 3589 "parser.y"
  {
	if (current_program->flag_chained) {
		cb_error (_("BY VALUE not allowed in CHAINED program"));
	} else {
		call_mode = CB_CALL_BY_VALUE;
	}
  }
#line 8068 "parser.c"
    break;

  case 592: /* size_optional: SIZE _is AUTO  */
#line 3601 "parser.y"
  {
	if (call_mode != CB_CALL_BY_VALUE) {
		cb_error (_("SIZE only allowed for BY VALUE items"));
	} else {
		size_mode = CB_SIZE_AUTO;
	}
  }
#line 8080 "parser.c"
    break;

  case 593: /* size_optional: SIZE _is DEFAULT  */
#line 3609 "parser.y"
  {
	if (call_mode != CB_CALL_BY_VALUE) {
		cb_error (_("SIZE only allowed for BY VALUE items"));
	} else {
		size_mode = CB_SIZE_4;
	}
  }
#line 8092 "parser.c"
    break;

  case 594: /* size_optional: UNSIGNED SIZE _is integer  */
#line 3617 "parser.y"
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
#line 8125 "parser.c"
    break;

  case 595: /* size_optional: SIZE _is integer  */
#line 3646 "parser.y"
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
#line 8158 "parser.c"
    break;

  case 597: /* procedure_optional: OPTIONAL  */
#line 3679 "parser.y"
  {
	if (call_mode != CB_CALL_BY_REFERENCE) {
		cb_error (_("OPTIONAL only allowed for BY REFERENCE items"));
	}
  }
#line 8168 "parser.c"
    break;

  case 598: /* procedure_returning: %empty  */
#line 3688 "parser.y"
  {
	if (current_program->prog_type == CB_FUNCTION_TYPE) {
		cb_error (_("RETURNING clause is required for a FUNCTION"));
	}
  }
#line 8178 "parser.c"
    break;

  case 599: /* procedure_returning: RETURNING "Identifier"  */
#line 3694 "parser.y"
  {
	if (cb_ref (yyvsp[0]) != cb_error_node) {
		current_program->returning = yyvsp[0];
		if (cb_field (yyvsp[0])->storage != CB_STORAGE_LINKAGE) {
			cb_error (_("RETURNING item is not defined in LINKAGE SECTION"));
		}
	}
  }
#line 8191 "parser.c"
    break;

  case 601: /* $@33: %empty  */
#line 3705 "parser.y"
                        { in_declaratives = 1; }
#line 8197 "parser.c"
    break;

  case 602: /* procedure_declaratives: DECLARATIVES '.' $@33 procedure_list END DECLARATIVES '.'  */
#line 3708 "parser.y"
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
#line 8219 "parser.c"
    break;

  case 608: /* procedure: statements '.'  */
#line 3741 "parser.y"
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
#line 8239 "parser.c"
    break;

  case 609: /* procedure: error  */
#line 3757 "parser.y"
  {
	check_unreached = 0;
  }
#line 8247 "parser.c"
    break;

  case 610: /* section_header: section_name SECTION opt_segment '.'  */
#line 3769 "parser.y"
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
#line 8279 "parser.c"
    break;

  case 611: /* paragraph_header: "Identifier" '.'  */
#line 3800 "parser.y"
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
#line 8317 "parser.c"
    break;

  case 612: /* invalid_statement: section_name  */
#line 3837 "parser.y"
  {
	non_const_word = 0;
	check_unreached = 0;
	if (yyvsp[0] != cb_error_node) {
		cb_error_x (yyvsp[0], _("Unknown statement '%s'"), CB_NAME (yyvsp[0]));
	}
	YYERROR;
  }
#line 8330 "parser.c"
    break;

  case 613: /* section_name: "Identifier"  */
#line 3848 "parser.y"
                                { yyval = cb_build_section_name (yyvsp[0], 0); }
#line 8336 "parser.c"
    break;

  case 615: /* opt_segment: "Literal"  */
#line 3852 "parser.y"
                                { /* ignore */ }
#line 8342 "parser.c"
    break;

  case 616: /* @34: %empty  */
#line 3861 "parser.y"
  {
	yyval = current_program->exec_list;
	current_program->exec_list = NULL;
  }
#line 8351 "parser.c"
    break;

  case 617: /* @35: %empty  */
#line 3865 "parser.y"
  {
	yyval = CB_TREE (current_statement);
	current_statement = NULL;
  }
#line 8360 "parser.c"
    break;

  case 618: /* statement_list: @34 @35 statements  */
#line 3870 "parser.y"
  {
	yyval = cb_list_reverse (current_program->exec_list);
	current_program->exec_list = yyvsp[-2];
	current_statement = CB_STATEMENT (yyvsp[-1]);
  }
#line 8370 "parser.c"
    break;

  case 619: /* statements: %empty  */
#line 3878 "parser.y"
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
#line 8395 "parser.c"
    break;

  case 671: /* statement: "NEXT SENTENCE"  */
#line 3954 "parser.y"
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
#line 8413 "parser.c"
    break;

  case 672: /* $@36: %empty  */
#line 3976 "parser.y"
  {
	BEGIN_STATEMENT ("ACCEPT", TERM_ACCEPT);
	dispattrs = 0;
	fgc = NULL;
	bgc = NULL;
	scroll = NULL;
  }
#line 8425 "parser.c"
    break;

  case 674: /* accept_body: identifier opt_at_line_column opt_accp_attr on_accp_exception  */
#line 3989 "parser.y"
  {
	cb_emit_accept (yyvsp[-3], yyvsp[-2], fgc, bgc, scroll, dispattrs);
  }
#line 8433 "parser.c"
    break;

  case 675: /* accept_body: identifier FROM ESCAPE KEY  */
#line 3993 "parser.y"
  {
	PENDING ("ACCEPT .. FROM ESCAPE KEY");
  }
#line 8441 "parser.c"
    break;

  case 676: /* accept_body: identifier FROM LINES  */
#line 3997 "parser.y"
  {
	cb_emit_accept_line_or_col (yyvsp[-2], 0);
  }
#line 8449 "parser.c"
    break;

  case 677: /* accept_body: identifier FROM COLUMNS  */
#line 4001 "parser.y"
  {
	cb_emit_accept_line_or_col (yyvsp[-2], 1);
  }
#line 8457 "parser.c"
    break;

  case 678: /* accept_body: identifier FROM DATE  */
#line 4005 "parser.y"
  {
	cb_emit_accept_date (yyvsp[-2]);
  }
#line 8465 "parser.c"
    break;

  case 679: /* accept_body: identifier FROM DATE YYYYMMDD  */
#line 4009 "parser.y"
  {
	cb_emit_accept_date_yyyymmdd (yyvsp[-3]);
  }
#line 8473 "parser.c"
    break;

  case 680: /* accept_body: identifier FROM DAY  */
#line 4013 "parser.y"
  {
	cb_emit_accept_day (yyvsp[-2]);
  }
#line 8481 "parser.c"
    break;

  case 681: /* accept_body: identifier FROM DAY YYYYDDD  */
#line 4017 "parser.y"
  {
	cb_emit_accept_day_yyyyddd (yyvsp[-3]);
  }
#line 8489 "parser.c"
    break;

  case 682: /* accept_body: identifier FROM "DAY-OF-WEEK"  */
#line 4021 "parser.y"
  {
	cb_emit_accept_day_of_week (yyvsp[-2]);
  }
#line 8497 "parser.c"
    break;

  case 683: /* accept_body: identifier FROM TIME  */
#line 4025 "parser.y"
  {
	cb_emit_accept_time (yyvsp[-2]);
  }
#line 8505 "parser.c"
    break;

  case 684: /* accept_body: identifier FROM "COMMAND-LINE"  */
#line 4029 "parser.y"
  {
	cb_emit_accept_command_line (yyvsp[-2]);
  }
#line 8513 "parser.c"
    break;

  case 685: /* accept_body: identifier FROM "ENVIRONMENT-VALUE" on_accp_exception  */
#line 4033 "parser.y"
  {
	cb_emit_accept_environment (yyvsp[-3]);
  }
#line 8521 "parser.c"
    break;

  case 686: /* accept_body: identifier FROM ENVIRONMENT simple_value on_accp_exception  */
#line 4037 "parser.y"
  { 
	cb_emit_get_environment (yyvsp[-1], yyvsp[-4]);
  }
#line 8529 "parser.c"
    break;

  case 687: /* accept_body: identifier FROM "ARGUMENT-NUMBER"  */
#line 4041 "parser.y"
  {
	cb_emit_accept_arg_number (yyvsp[-2]);
  }
#line 8537 "parser.c"
    break;

  case 688: /* accept_body: identifier FROM "ARGUMENT-VALUE" on_accp_exception  */
#line 4045 "parser.y"
  {
	cb_emit_accept_arg_value (yyvsp[-3]);
  }
#line 8545 "parser.c"
    break;

  case 689: /* accept_body: identifier FROM mnemonic_name  */
#line 4049 "parser.y"
  {
	cb_emit_accept_mnemonic (yyvsp[-2], yyvsp[0]);
  }
#line 8553 "parser.c"
    break;

  case 690: /* accept_body: identifier FROM "Identifier"  */
#line 4053 "parser.y"
  {
	cb_emit_accept_name (yyvsp[-2], yyvsp[0]);
  }
#line 8561 "parser.c"
    break;

  case 691: /* opt_at_line_column: %empty  */
#line 4059 "parser.y"
                                { yyval = NULL; }
#line 8567 "parser.c"
    break;

  case 692: /* opt_at_line_column: _at line_number column_number  */
#line 4060 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-1], yyvsp[0]); }
#line 8573 "parser.c"
    break;

  case 693: /* opt_at_line_column: _at column_number line_number  */
#line 4061 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], yyvsp[-1]); }
#line 8579 "parser.c"
    break;

  case 694: /* opt_at_line_column: _at line_number  */
#line 4062 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], NULL); }
#line 8585 "parser.c"
    break;

  case 695: /* opt_at_line_column: _at column_number  */
#line 4063 "parser.y"
                                { yyval = cb_build_pair (NULL, yyvsp[0]); }
#line 8591 "parser.c"
    break;

  case 696: /* opt_at_line_column: AT simple_value  */
#line 4064 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8597 "parser.c"
    break;

  case 697: /* line_number: LINE _number id_or_lit  */
#line 4068 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8603 "parser.c"
    break;

  case 698: /* column_number: COLUMN _number id_or_lit  */
#line 4072 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8609 "parser.c"
    break;

  case 699: /* column_number: POSITION _number id_or_lit  */
#line 4073 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8615 "parser.c"
    break;

  case 704: /* accp_attr: BELL  */
#line 4086 "parser.y"
                { dispattrs |= COB_SCREEN_BELL; }
#line 8621 "parser.c"
    break;

  case 705: /* accp_attr: BLINK  */
#line 4087 "parser.y"
                { dispattrs |= COB_SCREEN_BLINK; }
#line 8627 "parser.c"
    break;

  case 706: /* accp_attr: HIGHLIGHT  */
#line 4088 "parser.y"
                { dispattrs |= COB_SCREEN_HIGHLIGHT; }
#line 8633 "parser.c"
    break;

  case 707: /* accp_attr: LOWLIGHT  */
#line 4089 "parser.y"
                { dispattrs |= COB_SCREEN_LOWLIGHT; }
#line 8639 "parser.c"
    break;

  case 708: /* accp_attr: "REVERSE-VIDEO"  */
#line 4090 "parser.y"
                { dispattrs |= COB_SCREEN_REVERSE; }
#line 8645 "parser.c"
    break;

  case 709: /* accp_attr: UNDERLINE  */
#line 4091 "parser.y"
                { dispattrs |= COB_SCREEN_UNDERLINE; }
#line 8651 "parser.c"
    break;

  case 710: /* accp_attr: OVERLINE  */
#line 4092 "parser.y"
                { dispattrs |= COB_SCREEN_OVERLINE; }
#line 8657 "parser.c"
    break;

  case 711: /* accp_attr: "FOREGROUND-COLOR" _is num_id_or_lit  */
#line 4094 "parser.y"
  {
	fgc = yyvsp[0];
  }
#line 8665 "parser.c"
    break;

  case 712: /* accp_attr: "BACKGROUND-COLOR" _is num_id_or_lit  */
#line 4098 "parser.y"
  {
	bgc = yyvsp[0];
  }
#line 8673 "parser.c"
    break;

  case 713: /* accp_attr: SCROLL UP _opt_scroll_lines  */
#line 4102 "parser.y"
  {
	scroll = yyvsp[0];
  }
#line 8681 "parser.c"
    break;

  case 714: /* accp_attr: SCROLL DOWN _opt_scroll_lines  */
#line 4106 "parser.y"
  {
	dispattrs |= COB_SCREEN_SCROLL_DOWN;
	scroll = yyvsp[0];
  }
#line 8690 "parser.c"
    break;

  case 715: /* accp_attr: AUTO  */
#line 4110 "parser.y"
                { dispattrs |= COB_SCREEN_AUTO; }
#line 8696 "parser.c"
    break;

  case 716: /* accp_attr: FULL  */
#line 4111 "parser.y"
                { dispattrs |= COB_SCREEN_FULL; }
#line 8702 "parser.c"
    break;

  case 717: /* accp_attr: REQUIRED  */
#line 4112 "parser.y"
                { dispattrs |= COB_SCREEN_REQUIRED; }
#line 8708 "parser.c"
    break;

  case 718: /* accp_attr: SECURE  */
#line 4113 "parser.y"
                { dispattrs |= COB_SCREEN_SECURE; }
#line 8714 "parser.c"
    break;

  case 719: /* accp_attr: UPDATE  */
#line 4114 "parser.y"
                { dispattrs |= COB_SCREEN_UPDATE; }
#line 8720 "parser.c"
    break;

  case 720: /* accp_attr: PROMPT  */
#line 4115 "parser.y"
                { dispattrs |= COB_SCREEN_PROMPT; }
#line 8726 "parser.c"
    break;

  case 721: /* end_accept: %empty  */
#line 4119 "parser.y"
                                { terminator_warning (TERM_ACCEPT); }
#line 8732 "parser.c"
    break;

  case 722: /* end_accept: "END-ACCEPT"  */
#line 4120 "parser.y"
                                { terminator_clear (TERM_ACCEPT); }
#line 8738 "parser.c"
    break;

  case 723: /* $@37: %empty  */
#line 4129 "parser.y"
                                { BEGIN_STATEMENT ("ADD", TERM_ADD); }
#line 8744 "parser.c"
    break;

  case 725: /* add_body: x_list TO arithmetic_x_list on_size_error  */
#line 4136 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '+', cb_build_binary_list (yyvsp[-3], '+'));
  }
#line 8752 "parser.c"
    break;

  case 726: /* add_body: x_list add_to GIVING arithmetic_x_list on_size_error  */
#line 4140 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_list (yyvsp[-4], '+'));
  }
#line 8760 "parser.c"
    break;

  case 727: /* add_body: CORRESPONDING identifier TO identifier flag_rounded on_size_error  */
#line 4144 "parser.y"
  {
	cb_emit_corresponding (cb_build_add, yyvsp[-2], yyvsp[-4], yyvsp[-1]);
  }
#line 8768 "parser.c"
    break;

  case 729: /* add_to: TO x  */
#line 4150 "parser.y"
                                { cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 8774 "parser.c"
    break;

  case 730: /* end_add: %empty  */
#line 4154 "parser.y"
                                { terminator_warning (TERM_ADD); }
#line 8780 "parser.c"
    break;

  case 731: /* end_add: "END-ADD"  */
#line 4155 "parser.y"
                                { terminator_clear (TERM_ADD); }
#line 8786 "parser.c"
    break;

  case 732: /* $@38: %empty  */
#line 4164 "parser.y"
                                { BEGIN_STATEMENT ("ALLOCATE", 0); }
#line 8792 "parser.c"
    break;

  case 734: /* allocate_body: "Identifier" flag_initialized allocate_returning  */
#line 4170 "parser.y"
  {
	cb_emit_allocate (yyvsp[-2], yyvsp[0], NULL, yyvsp[-1]);
  }
#line 8800 "parser.c"
    break;

  case 735: /* allocate_body: expr CHARACTERS flag_initialized RETURNING target_x  */
#line 4174 "parser.y"
  {
	cb_emit_allocate (NULL, yyvsp[0], yyvsp[-4], yyvsp[-2]);
  }
#line 8808 "parser.c"
    break;

  case 736: /* allocate_returning: %empty  */
#line 4180 "parser.y"
                                { yyval = NULL; }
#line 8814 "parser.c"
    break;

  case 737: /* allocate_returning: RETURNING target_x  */
#line 4181 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8820 "parser.c"
    break;

  case 738: /* alter_statement: ALTER alter_options  */
#line 4191 "parser.y"
  {
	cb_error (_("ALTER statement is obsolete and unsupported"));
  }
#line 8828 "parser.c"
    break;

  case 743: /* $@39: %empty  */
#line 4209 "parser.y"
                                { BEGIN_STATEMENT ("CALL", TERM_CALL); }
#line 8834 "parser.c"
    break;

  case 744: /* call_statement: CALL $@39 id_or_lit_or_func call_using call_returning call_on_exception call_not_on_exception end_call  */
#line 4213 "parser.y"
  {
	cb_emit_call (yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1]);
  }
#line 8842 "parser.c"
    break;

  case 745: /* call_using: %empty  */
#line 4219 "parser.y"
                                { yyval = NULL; }
#line 8848 "parser.c"
    break;

  case 746: /* $@40: %empty  */
#line 4221 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
	size_mode = CB_SIZE_4;
  }
#line 8857 "parser.c"
    break;

  case 747: /* call_using: USING $@40 call_param_list  */
#line 4225 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8863 "parser.c"
    break;

  case 748: /* call_param_list: call_param  */
#line 4229 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8869 "parser.c"
    break;

  case 749: /* call_param_list: call_param_list call_param  */
#line 4231 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 8875 "parser.c"
    break;

  case 750: /* call_param: call_type OMITTED  */
#line 4236 "parser.y"
  {
	if (call_mode != CB_CALL_BY_REFERENCE) {
		cb_error (_("OMITTED only allowed with BY REFERENCE"));
	}
	yyval = cb_build_pair (cb_int (call_mode), cb_null);
  }
#line 8886 "parser.c"
    break;

  case 751: /* call_param: call_type size_optional x  */
#line 4243 "parser.y"
  {
	yyval = cb_build_pair (cb_int (call_mode), yyvsp[0]);
	CB_SIZES (yyval) = size_mode;
  }
#line 8895 "parser.c"
    break;

  case 753: /* call_type: _by REFERENCE  */
#line 4252 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
  }
#line 8903 "parser.c"
    break;

  case 754: /* call_type: _by CONTENT  */
#line 4256 "parser.y"
  {
	if (current_program->flag_chained) {
		cb_error (_("BY CONTENT not allowed in CHAINED program"));
	} else {
		call_mode = CB_CALL_BY_CONTENT;
	}
  }
#line 8915 "parser.c"
    break;

  case 755: /* call_type: _by VALUE  */
#line 4264 "parser.y"
  {
	if (current_program->flag_chained) {
		cb_error (_("BY VALUE not allowed in CHAINED program"));
	} else {
		call_mode = CB_CALL_BY_VALUE;
	}
  }
#line 8927 "parser.c"
    break;

  case 756: /* call_returning: %empty  */
#line 4274 "parser.y"
                                { yyval = NULL; }
#line 8933 "parser.c"
    break;

  case 757: /* call_returning: RETURNING identifier  */
#line 4275 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8939 "parser.c"
    break;

  case 758: /* call_returning: GIVING identifier  */
#line 4276 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8945 "parser.c"
    break;

  case 759: /* call_on_exception: %empty  */
#line 4281 "parser.y"
  {
	yyval = NULL;
  }
#line 8953 "parser.c"
    break;

  case 760: /* $@41: %empty  */
#line 4285 "parser.y"
  {
	check_unreached = 0;
  }
#line 8961 "parser.c"
    break;

  case 761: /* call_on_exception: exception_or_overflow $@41 statement_list  */
#line 4289 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 8969 "parser.c"
    break;

  case 762: /* call_not_on_exception: %empty  */
#line 4296 "parser.y"
  {
	yyval = NULL;
  }
#line 8977 "parser.c"
    break;

  case 763: /* $@42: %empty  */
#line 4300 "parser.y"
  {
	check_unreached = 0;
  }
#line 8985 "parser.c"
    break;

  case 764: /* call_not_on_exception: not_exception_or_overflow $@42 statement_list  */
#line 4304 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 8993 "parser.c"
    break;

  case 765: /* end_call: %empty  */
#line 4310 "parser.y"
                                { terminator_warning (TERM_CALL); }
#line 8999 "parser.c"
    break;

  case 766: /* end_call: "END-CALL"  */
#line 4311 "parser.y"
                                { terminator_clear (TERM_CALL); }
#line 9005 "parser.c"
    break;

  case 767: /* $@43: %empty  */
#line 4320 "parser.y"
                                { BEGIN_STATEMENT ("CANCEL", 0); }
#line 9011 "parser.c"
    break;

  case 770: /* cancel_list: cancel_list id_or_lit  */
#line 4326 "parser.y"
  {
	cb_emit_cancel (yyvsp[0]);
  }
#line 9019 "parser.c"
    break;

  case 771: /* cancel_list: ALL  */
#line 4330 "parser.y"
  {
	cb_emit_cancel_all ();
  }
#line 9027 "parser.c"
    break;

  case 772: /* $@44: %empty  */
#line 4341 "parser.y"
                                { BEGIN_STATEMENT ("CLOSE", 0); }
#line 9033 "parser.c"
    break;

  case 775: /* close_list: close_list file_name close_option  */
#line 4348 "parser.y"
  {
	BEGIN_IMPLICIT_STATEMENT (yyvsp[-1]);
	if (yyvsp[-1] != cb_error_node) {
		cb_emit_close (yyvsp[-1], yyvsp[0]);
	}
  }
#line 9044 "parser.c"
    break;

  case 776: /* close_option: %empty  */
#line 4357 "parser.y"
                                { yyval = cb_int (COB_CLOSE_NORMAL); }
#line 9050 "parser.c"
    break;

  case 777: /* close_option: reel_or_unit  */
#line 4358 "parser.y"
                                { yyval = cb_int (COB_CLOSE_UNIT); }
#line 9056 "parser.c"
    break;

  case 778: /* close_option: reel_or_unit _for REMOVAL  */
#line 4359 "parser.y"
                                { yyval = cb_int (COB_CLOSE_UNIT_REMOVAL); }
#line 9062 "parser.c"
    break;

  case 779: /* close_option: _with NO REWIND  */
#line 4360 "parser.y"
                                { yyval = cb_int (COB_CLOSE_NO_REWIND); }
#line 9068 "parser.c"
    break;

  case 780: /* close_option: _with LOCK  */
#line 4361 "parser.y"
                                { yyval = cb_int (COB_CLOSE_LOCK); }
#line 9074 "parser.c"
    break;

  case 783: /* $@45: %empty  */
#line 4372 "parser.y"
                                { BEGIN_STATEMENT ("COMPUTE", TERM_COMPUTE); }
#line 9080 "parser.c"
    break;

  case 785: /* compute_body: arithmetic_x_list comp_equal expr on_size_error  */
#line 4379 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-3], 0, yyvsp[-1]);
  }
#line 9088 "parser.c"
    break;

  case 786: /* end_compute: %empty  */
#line 4385 "parser.y"
                                { terminator_warning (TERM_COMPUTE); }
#line 9094 "parser.c"
    break;

  case 787: /* end_compute: "END-COMPUTE"  */
#line 4386 "parser.y"
                                { terminator_clear (TERM_COMPUTE); }
#line 9100 "parser.c"
    break;

  case 790: /* commit_statement: COMMIT  */
#line 4397 "parser.y"
  {
	BEGIN_STATEMENT ("COMMIT", 0);
	cb_emit_commit ();
  }
#line 9109 "parser.c"
    break;

  case 791: /* continue_statement: CONTINUE  */
#line 4410 "parser.y"
  {
	BEGIN_STATEMENT ("CONTINUE", 0);
	cb_emit_continue ();
  }
#line 9118 "parser.c"
    break;

  case 792: /* $@46: %empty  */
#line 4422 "parser.y"
                                { BEGIN_STATEMENT ("DELETE", TERM_DELETE); }
#line 9124 "parser.c"
    break;

  case 793: /* delete_statement: DELETE $@46 file_name _record opt_invalid_key end_delete  */
#line 4425 "parser.y"
  {
	if (yyvsp[-3] != cb_error_node) {
		cb_emit_delete (yyvsp[-3]);
	}
  }
#line 9134 "parser.c"
    break;

  case 794: /* end_delete: %empty  */
#line 4433 "parser.y"
                                { terminator_warning (TERM_DELETE); }
#line 9140 "parser.c"
    break;

  case 795: /* end_delete: "END-DELETE"  */
#line 4434 "parser.y"
                                { terminator_clear (TERM_DELETE); }
#line 9146 "parser.c"
    break;

  case 796: /* $@47: %empty  */
#line 4443 "parser.y"
                                  { BEGIN_STATEMENT ("DELETE-FILE", 0); }
#line 9152 "parser.c"
    break;

  case 797: /* delete_file_statement: DELETE $@47 "FILE" file_name_list  */
#line 4445 "parser.y"
  {
	cb_tree l;
	for (l = yyvsp[0]; l; l = CB_CHAIN (l)) {
		if (CB_VALUE (l) != cb_error_node) {
			BEGIN_IMPLICIT_STATEMENT (l);
			cb_emit_delete_file (CB_VALUE (l));
		}
	}
  }
#line 9166 "parser.c"
    break;

  case 798: /* $@48: %empty  */
#line 4463 "parser.y"
  {
	BEGIN_STATEMENT ("DISPLAY", TERM_DISPLAY);
	dispattrs = 0;
	fgc = NULL;
	bgc = NULL;
	scroll = NULL;
  }
#line 9178 "parser.c"
    break;

  case 800: /* display_body: id_or_lit "UPON ENVIRONMENT-NAME" on_disp_exception  */
#line 4476 "parser.y"
  {
	cb_emit_env_name (yyvsp[-2]);
  }
#line 9186 "parser.c"
    break;

  case 801: /* display_body: id_or_lit "UPON ENVIRONMENT-VALUE" on_disp_exception  */
#line 4480 "parser.y"
  {
	cb_emit_env_value (yyvsp[-2]);
  }
#line 9194 "parser.c"
    break;

  case 802: /* display_body: id_or_lit "UPON ARGUMENT-NUMBER" on_disp_exception  */
#line 4484 "parser.y"
  {
	cb_emit_arg_number (yyvsp[-2]);
  }
#line 9202 "parser.c"
    break;

  case 803: /* display_body: id_or_lit "UPON COMMAND-LINE" on_disp_exception  */
#line 4488 "parser.y"
  {
	cb_emit_command_line (yyvsp[-2]);
  }
#line 9210 "parser.c"
    break;

  case 804: /* display_body: x_list opt_at_line_column with_clause on_disp_exception  */
#line 4492 "parser.y"
  {
	cb_emit_display (yyvsp[-3], cb_int0, yyvsp[-1], yyvsp[-2], fgc, bgc, scroll, dispattrs);
  }
#line 9218 "parser.c"
    break;

  case 805: /* display_body: x_list opt_at_line_column UPON mnemonic_name with_clause on_disp_exception  */
#line 4496 "parser.y"
  {
	cb_emit_display_mnemonic (yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9226 "parser.c"
    break;

  case 806: /* display_body: x_list opt_at_line_column UPON "Identifier" with_clause on_disp_exception  */
#line 4500 "parser.y"
  {
	cb_tree word = cb_build_display_upon_direct (yyvsp[-2]);
	cb_emit_display (yyvsp[-5], word, yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9235 "parser.c"
    break;

  case 807: /* display_body: x_list opt_at_line_column UPON PRINTER with_clause on_disp_exception  */
#line 4505 "parser.y"
  {
	cb_emit_display (yyvsp[-5], cb_int0, yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9243 "parser.c"
    break;

  case 808: /* display_body: x_list opt_at_line_column UPON CRT with_clause on_disp_exception  */
#line 4509 "parser.y"
  {
	cb_emit_display (yyvsp[-5], cb_int0, yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9251 "parser.c"
    break;

  case 809: /* with_clause: %empty  */
#line 4515 "parser.y"
                                { yyval = cb_int1; }
#line 9257 "parser.c"
    break;

  case 810: /* with_clause: _with "NO ADVANCING"  */
#line 4516 "parser.y"
                                { yyval = cb_int0; }
#line 9263 "parser.c"
    break;

  case 811: /* with_clause: WITH disp_attrs  */
#line 4517 "parser.y"
                                { yyval = cb_int1; }
#line 9269 "parser.c"
    break;

  case 814: /* disp_attr: BELL  */
#line 4527 "parser.y"
                { dispattrs |= COB_SCREEN_BELL; }
#line 9275 "parser.c"
    break;

  case 815: /* disp_attr: BLINK  */
#line 4528 "parser.y"
                { dispattrs |= COB_SCREEN_BLINK; }
#line 9281 "parser.c"
    break;

  case 816: /* disp_attr: ERASE EOL  */
#line 4529 "parser.y"
                { dispattrs |= COB_SCREEN_ERASE_EOL; }
#line 9287 "parser.c"
    break;

  case 817: /* disp_attr: ERASE EOS  */
#line 4530 "parser.y"
                { dispattrs |= COB_SCREEN_ERASE_EOS; }
#line 9293 "parser.c"
    break;

  case 818: /* disp_attr: HIGHLIGHT  */
#line 4531 "parser.y"
                { dispattrs |= COB_SCREEN_HIGHLIGHT; }
#line 9299 "parser.c"
    break;

  case 819: /* disp_attr: LOWLIGHT  */
#line 4532 "parser.y"
                { dispattrs |= COB_SCREEN_LOWLIGHT; }
#line 9305 "parser.c"
    break;

  case 820: /* disp_attr: "REVERSE-VIDEO"  */
#line 4533 "parser.y"
                { dispattrs |= COB_SCREEN_REVERSE; }
#line 9311 "parser.c"
    break;

  case 821: /* disp_attr: UNDERLINE  */
#line 4534 "parser.y"
                { dispattrs |= COB_SCREEN_UNDERLINE; }
#line 9317 "parser.c"
    break;

  case 822: /* disp_attr: OVERLINE  */
#line 4535 "parser.y"
                { dispattrs |= COB_SCREEN_OVERLINE; }
#line 9323 "parser.c"
    break;

  case 823: /* disp_attr: "FOREGROUND-COLOR" _is num_id_or_lit  */
#line 4537 "parser.y"
  {
	fgc = yyvsp[0];
  }
#line 9331 "parser.c"
    break;

  case 824: /* disp_attr: "BACKGROUND-COLOR" _is num_id_or_lit  */
#line 4541 "parser.y"
  {
	bgc = yyvsp[0];
  }
#line 9339 "parser.c"
    break;

  case 825: /* disp_attr: SCROLL UP _opt_scroll_lines  */
#line 4545 "parser.y"
  {
	scroll = yyvsp[0];
  }
#line 9347 "parser.c"
    break;

  case 826: /* disp_attr: SCROLL DOWN _opt_scroll_lines  */
#line 4549 "parser.y"
  {
	dispattrs |= COB_SCREEN_SCROLL_DOWN;
	scroll = yyvsp[0];
  }
#line 9356 "parser.c"
    break;

  case 827: /* disp_attr: "BLANK-LINE"  */
#line 4553 "parser.y"
                { dispattrs |= COB_SCREEN_BLANK_LINE; }
#line 9362 "parser.c"
    break;

  case 828: /* disp_attr: "BLANK-SCREEN"  */
#line 4554 "parser.y"
                { dispattrs |= COB_SCREEN_BLANK_SCREEN; }
#line 9368 "parser.c"
    break;

  case 829: /* end_display: %empty  */
#line 4558 "parser.y"
                                { terminator_warning (TERM_DISPLAY); }
#line 9374 "parser.c"
    break;

  case 830: /* end_display: "END-DISPLAY"  */
#line 4559 "parser.y"
                                { terminator_clear (TERM_DISPLAY); }
#line 9380 "parser.c"
    break;

  case 831: /* $@49: %empty  */
#line 4568 "parser.y"
                                { BEGIN_STATEMENT ("DIVIDE", TERM_DIVIDE); }
#line 9386 "parser.c"
    break;

  case 833: /* divide_body: x INTO arithmetic_x_list on_size_error  */
#line 4575 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '/', yyvsp[-3]);
  }
#line 9394 "parser.c"
    break;

  case 834: /* divide_body: x INTO x GIVING arithmetic_x_list on_size_error  */
#line 4579 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_op (yyvsp[-3], '/', yyvsp[-5]));
  }
#line 9402 "parser.c"
    break;

  case 835: /* divide_body: x BY x GIVING arithmetic_x_list on_size_error  */
#line 4583 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_op (yyvsp[-5], '/', yyvsp[-3]));
  }
#line 9410 "parser.c"
    break;

  case 836: /* divide_body: x INTO x GIVING arithmetic_x REMAINDER arithmetic_x on_size_error  */
#line 4587 "parser.y"
  {
	cb_emit_divide (yyvsp[-5], yyvsp[-7], yyvsp[-3], yyvsp[-1]);
  }
#line 9418 "parser.c"
    break;

  case 837: /* divide_body: x BY x GIVING arithmetic_x REMAINDER arithmetic_x on_size_error  */
#line 4591 "parser.y"
  {
	cb_emit_divide (yyvsp[-7], yyvsp[-5], yyvsp[-3], yyvsp[-1]);
  }
#line 9426 "parser.c"
    break;

  case 838: /* end_divide: %empty  */
#line 4597 "parser.y"
                                { terminator_warning (TERM_DIVIDE); }
#line 9432 "parser.c"
    break;

  case 839: /* end_divide: "END-DIVIDE"  */
#line 4598 "parser.y"
                                { terminator_clear (TERM_DIVIDE); }
#line 9438 "parser.c"
    break;

  case 840: /* $@50: %empty  */
#line 4607 "parser.y"
                                { BEGIN_STATEMENT ("ENTRY", 0); }
#line 9444 "parser.c"
    break;

  case 841: /* entry_statement: ENTRY $@50 "Literal" call_using  */
#line 4609 "parser.y"
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
#line 9460 "parser.c"
    break;

  case 842: /* $@51: %empty  */
#line 4629 "parser.y"
  {
	BEGIN_STATEMENT ("EVALUATE", TERM_EVALUATE);
	eval_level++;
	for (eval_inc = 0; eval_inc < 64; eval_inc++) {
		eval_check[eval_level][eval_inc] = 0;
	}
	eval_inc = 0;
	eval_inc2 = 0;
  }
#line 9474 "parser.c"
    break;

  case 843: /* evaluate_statement: EVALUATE $@51 evaluate_subject_list evaluate_condition_list end_evaluate  */
#line 4640 "parser.y"
  {
	cb_emit_evaluate (yyvsp[-2], yyvsp[-1]);
	eval_level--;
  }
#line 9483 "parser.c"
    break;

  case 844: /* evaluate_subject_list: evaluate_subject  */
#line 4647 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9489 "parser.c"
    break;

  case 845: /* evaluate_subject_list: evaluate_subject_list _also evaluate_subject  */
#line 4650 "parser.y"
  {
 	if (!cb_allow_missing_also_clause_in_evaluate && yyvsp[-1] != cb_int1) {
 		cb_error  (_("Invalid expression"));
 	}
 	yyval = cb_list_add (yyvsp[-2], yyvsp[0]);
  }
#line 9500 "parser.c"
    break;

  case 846: /* evaluate_subject: expr  */
#line 4660 "parser.y"
  {
	yyval = yyvsp[0];
	if (CB_REFERENCE_P (yyvsp[0])) {
		eval_check[eval_level][eval_inc++] = 0;
	} else {
		eval_check[eval_level][eval_inc++] = 1;
	}
  }
#line 9513 "parser.c"
    break;

  case 847: /* evaluate_subject: "TRUE"  */
#line 4669 "parser.y"
  {
	yyval = cb_true;
	eval_check[eval_level][eval_inc++] = 2;
  }
#line 9522 "parser.c"
    break;

  case 848: /* evaluate_subject: "FALSE"  */
#line 4674 "parser.y"
  {
	yyval = cb_false;
	eval_check[eval_level][eval_inc++] = 3;
  }
#line 9531 "parser.c"
    break;

  case 849: /* evaluate_condition_list: evaluate_case_list evaluate_other  */
#line 4682 "parser.y"
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
#line 9561 "parser.c"
    break;

  case 850: /* evaluate_case_list: evaluate_case  */
#line 4710 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9567 "parser.c"
    break;

  case 851: /* evaluate_case_list: evaluate_case_list evaluate_case  */
#line 4712 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 9573 "parser.c"
    break;

  case 852: /* $@52: %empty  */
#line 4717 "parser.y"
  {
	check_unreached = 0;
  }
#line 9581 "parser.c"
    break;

  case 853: /* evaluate_case: evaluate_when_list $@52 statement_list  */
#line 4721 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[0] == NULL) {
		cb_error (_("syntax error"));
	}
	yyval = cb_cons (yyvsp[0], yyvsp[-2]);
	eval_inc2 = 0;
  }
#line 9593 "parser.c"
    break;

  case 854: /* evaluate_other: %empty  */
#line 4732 "parser.y"
  {
	yyval = NULL;
  }
#line 9601 "parser.c"
    break;

  case 855: /* $@53: %empty  */
#line 4736 "parser.y"
  {
	check_unreached = 0;
  }
#line 9609 "parser.c"
    break;

  case 856: /* evaluate_other: "WHEN OTHER" $@53 statement_list  */
#line 4740 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[0] == NULL) {
		cb_error (_("syntax error"));
	}
	yyval = cb_cons (yyvsp[0], NULL);
	eval_inc2 = 0;
  }
#line 9621 "parser.c"
    break;

  case 857: /* evaluate_when_list: WHEN evaluate_object_list  */
#line 4750 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9627 "parser.c"
    break;

  case 858: /* evaluate_when_list: evaluate_when_list WHEN evaluate_object_list  */
#line 4752 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 9633 "parser.c"
    break;

  case 859: /* evaluate_object_list: evaluate_object  */
#line 4756 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9639 "parser.c"
    break;

  case 860: /* evaluate_object_list: evaluate_object_list _also evaluate_object  */
#line 4759 "parser.y"
  {
 	if (!cb_allow_missing_also_clause_in_evaluate && yyvsp[-1] != cb_int1) {
 		cb_error  (_("Invalid expression"));
 	}
 	yyval = cb_list_add (yyvsp[-2], yyvsp[0]);
  }
#line 9650 "parser.c"
    break;

  case 861: /* evaluate_object: partial_expr opt_evaluate_thru_expr  */
#line 4769 "parser.y"
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
#line 9683 "parser.c"
    break;

  case 862: /* evaluate_object: ANY  */
#line 4797 "parser.y"
                                { yyval = cb_any; eval_inc2++; }
#line 9689 "parser.c"
    break;

  case 863: /* evaluate_object: "TRUE"  */
#line 4798 "parser.y"
                                { yyval = cb_true; eval_inc2++; }
#line 9695 "parser.c"
    break;

  case 864: /* evaluate_object: "FALSE"  */
#line 4799 "parser.y"
                                { yyval = cb_false; eval_inc2++; }
#line 9701 "parser.c"
    break;

  case 865: /* opt_evaluate_thru_expr: %empty  */
#line 4802 "parser.y"
                                { yyval = NULL; }
#line 9707 "parser.c"
    break;

  case 866: /* opt_evaluate_thru_expr: THRU expr  */
#line 4803 "parser.y"
                                { yyval = yyvsp[0]; }
#line 9713 "parser.c"
    break;

  case 867: /* end_evaluate: %empty  */
#line 4807 "parser.y"
                                { terminator_warning (TERM_EVALUATE); }
#line 9719 "parser.c"
    break;

  case 868: /* end_evaluate: "END-EVALUATE"  */
#line 4808 "parser.y"
                                { terminator_clear (TERM_EVALUATE); }
#line 9725 "parser.c"
    break;

  case 869: /* $@54: %empty  */
#line 4817 "parser.y"
                                { BEGIN_STATEMENT ("EXIT", 0); }
#line 9731 "parser.c"
    break;

  case 871: /* exit_body: %empty  */
#line 4822 "parser.y"
                                { /* nothing */ }
#line 9737 "parser.c"
    break;

  case 872: /* exit_body: PROGRAM  */
#line 4824 "parser.y"
  {
	if (in_declaratives && use_global_ind) {
		cb_error (_("EXIT PROGRAM is not allowed within a USE GLOBAL procedure"));
	}
	check_unreached = 1;
	cb_emit_exit (0);
  }
#line 9749 "parser.c"
    break;

  case 873: /* exit_body: PERFORM  */
#line 4832 "parser.y"
  {
	if (!perform_stack) {
		cb_error (_("EXIT PERFORM is only valid with inline PERFORM"));
	} else {
		cb_emit_java_break ();
	}
  }
#line 9761 "parser.c"
    break;

  case 874: /* exit_body: PERFORM CYCLE  */
#line 4840 "parser.y"
  {
	if (!perform_stack) {
		cb_error (_("EXIT PERFORM is only valid with inline PERFORM"));
	} else {
		cb_emit_java_continue ();
	}
  }
#line 9773 "parser.c"
    break;

  case 875: /* exit_body: SECTION  */
#line 4848 "parser.y"
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
#line 9795 "parser.c"
    break;

  case 876: /* exit_body: PARAGRAPH  */
#line 4866 "parser.y"
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
#line 9817 "parser.c"
    break;

  case 877: /* $@55: %empty  */
#line 4890 "parser.y"
                                { BEGIN_STATEMENT ("FREE", 0); }
#line 9823 "parser.c"
    break;

  case 878: /* free_statement: FREE $@55 target_x_list  */
#line 4892 "parser.y"
  {
	cb_emit_free (yyvsp[0]);
  }
#line 9831 "parser.c"
    break;

  case 879: /* $@56: %empty  */
#line 4903 "parser.y"
                                { BEGIN_STATEMENT ("GENERATE", 0); }
#line 9837 "parser.c"
    break;

  case 880: /* generate_statement: GENERATE $@56 identifier  */
#line 4905 "parser.y"
  {
	PENDING("GENERATE");
  }
#line 9845 "parser.c"
    break;

  case 881: /* $@57: %empty  */
#line 4916 "parser.y"
                                { BEGIN_STATEMENT ("GO TO", 0); }
#line 9851 "parser.c"
    break;

  case 882: /* goto_statement: GO _to $@57 procedure_name_list goto_depending  */
#line 4918 "parser.y"
  {
	cb_emit_goto (yyvsp[-1], yyvsp[0]);
  }
#line 9859 "parser.c"
    break;

  case 883: /* goto_depending: %empty  */
#line 4925 "parser.y"
  {
	check_unreached = 1;
	yyval = NULL;
  }
#line 9868 "parser.c"
    break;

  case 884: /* goto_depending: DEPENDING _on identifier  */
#line 4930 "parser.y"
  {
	check_unreached = 0;
	yyval = yyvsp[0];
  }
#line 9877 "parser.c"
    break;

  case 885: /* $@58: %empty  */
#line 4942 "parser.y"
                                { BEGIN_STATEMENT ("GOBACK", 0); }
#line 9883 "parser.c"
    break;

  case 886: /* goback_statement: GOBACK $@58  */
#line 4943 "parser.y"
  {
	check_unreached = 1;
	cb_emit_exit (1);
  }
#line 9892 "parser.c"
    break;

  case 887: /* $@59: %empty  */
#line 4955 "parser.y"
                                { BEGIN_STATEMENT ("IF", TERM_IF); }
#line 9898 "parser.c"
    break;

  case 888: /* $@60: %empty  */
#line 4957 "parser.y"
  {
	check_unreached = 0;
  }
#line 9906 "parser.c"
    break;

  case 889: /* if_statement: IF $@59 condition _then $@60 statement_list if_else_sentence end_if  */
#line 4962 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[-2] == NULL) {
		cb_error (_("syntax error"));
	}
	cb_emit_if (yyvsp[-5], yyvsp[-2], yyvsp[-1]);
  }
#line 9917 "parser.c"
    break;

  case 891: /* if_else_sentence: %empty  */
#line 4973 "parser.y"
  {
	yyval = NULL;
  }
#line 9925 "parser.c"
    break;

  case 892: /* $@61: %empty  */
#line 4977 "parser.y"
  {
	check_unreached = 0;
  }
#line 9933 "parser.c"
    break;

  case 893: /* if_else_sentence: ELSE $@61 statement_list  */
#line 4981 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[0] == NULL) {
		cb_error (_("syntax error"));
	}
	yyval = yyvsp[0];
  }
#line 9944 "parser.c"
    break;

  case 894: /* end_if: %empty  */
#line 4990 "parser.y"
                                { terminator_warning (TERM_IF); }
#line 9950 "parser.c"
    break;

  case 895: /* end_if: "END-IF"  */
#line 4991 "parser.y"
                                { terminator_clear (TERM_IF); }
#line 9956 "parser.c"
    break;

  case 896: /* $@62: %empty  */
#line 5000 "parser.y"
                                { BEGIN_STATEMENT ("INITIALIZE", 0); }
#line 9962 "parser.c"
    break;

  case 897: /* initialize_statement: INITIALIZE $@62 target_x_list initialize_filler initialize_value initialize_replacing initialize_default  */
#line 5002 "parser.y"
  {
	cb_emit_initialize (yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]);
  }
#line 9970 "parser.c"
    break;

  case 898: /* initialize_filler: %empty  */
#line 5008 "parser.y"
                                { yyval = NULL; }
#line 9976 "parser.c"
    break;

  case 899: /* initialize_filler: _with FILLER  */
#line 5009 "parser.y"
                                { yyval = cb_true; }
#line 9982 "parser.c"
    break;

  case 900: /* initialize_value: %empty  */
#line 5013 "parser.y"
                                { yyval = NULL; }
#line 9988 "parser.c"
    break;

  case 901: /* initialize_value: ALL _to VALUE  */
#line 5014 "parser.y"
                                { yyval = cb_true; }
#line 9994 "parser.c"
    break;

  case 902: /* initialize_value: initialize_category _to VALUE  */
#line 5015 "parser.y"
                                { yyval = yyvsp[-2]; }
#line 10000 "parser.c"
    break;

  case 903: /* initialize_replacing: %empty  */
#line 5019 "parser.y"
                                { yyval = NULL; }
#line 10006 "parser.c"
    break;

  case 904: /* initialize_replacing: REPLACING initialize_replacing_list  */
#line 5021 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10012 "parser.c"
    break;

  case 905: /* initialize_replacing_list: initialize_replacing_item  */
#line 5025 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10018 "parser.c"
    break;

  case 906: /* initialize_replacing_list: initialize_replacing_list initialize_replacing_item  */
#line 5027 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 10024 "parser.c"
    break;

  case 907: /* initialize_replacing_item: initialize_category _data BY x  */
#line 5031 "parser.y"
                                 { yyval = cb_build_pair (yyvsp[-3], yyvsp[0]); }
#line 10030 "parser.c"
    break;

  case 908: /* initialize_category: ALPHABETIC  */
#line 5035 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_ALPHABETIC); }
#line 10036 "parser.c"
    break;

  case 909: /* initialize_category: ALPHANUMERIC  */
#line 5036 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_ALPHANUMERIC); }
#line 10042 "parser.c"
    break;

  case 910: /* initialize_category: NUMERIC  */
#line 5037 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NUMERIC); }
#line 10048 "parser.c"
    break;

  case 911: /* initialize_category: "ALPHANUMERIC-EDITED"  */
#line 5038 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_ALPHANUMERIC_EDITED); }
#line 10054 "parser.c"
    break;

  case 912: /* initialize_category: "NUMERIC-EDITED"  */
#line 5039 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NUMERIC_EDITED); }
#line 10060 "parser.c"
    break;

  case 913: /* initialize_category: NATIONAL  */
#line 5040 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NATIONAL); }
#line 10066 "parser.c"
    break;

  case 914: /* initialize_category: "NATIONAL-EDITED"  */
#line 5041 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NATIONAL_EDITED); }
#line 10072 "parser.c"
    break;

  case 915: /* initialize_default: %empty  */
#line 5045 "parser.y"
                                { yyval = NULL; }
#line 10078 "parser.c"
    break;

  case 916: /* initialize_default: DEFAULT  */
#line 5046 "parser.y"
                                { yyval = cb_true; }
#line 10084 "parser.c"
    break;

  case 917: /* $@63: %empty  */
#line 5055 "parser.y"
                                { BEGIN_STATEMENT ("INITIATE", 0); }
#line 10090 "parser.c"
    break;

  case 918: /* initiate_statement: INITIATE $@63 identifier_list  */
#line 5057 "parser.y"
  {
	PENDING("INITIATE");
  }
#line 10098 "parser.c"
    break;

  case 919: /* $@64: %empty  */
#line 5068 "parser.y"
  {
	BEGIN_STATEMENT ("INSPECT", 0);
	sending_id = 0;
	inspect_keyword = 0;
  }
#line 10108 "parser.c"
    break;

  case 921: /* send_identifier: identifier  */
#line 5077 "parser.y"
                                { save_tree_1 = yyvsp[0]; sending_id = 0; }
#line 10114 "parser.c"
    break;

  case 922: /* send_identifier: literal  */
#line 5078 "parser.y"
                                { save_tree_1 = yyvsp[0]; sending_id = 1; }
#line 10120 "parser.c"
    break;

  case 923: /* send_identifier: function  */
#line 5079 "parser.y"
                                { save_tree_1 = yyvsp[0]; sending_id = 1; }
#line 10126 "parser.c"
    break;

  case 926: /* inspect_item: inspect_tallying  */
#line 5088 "parser.y"
                                { cb_emit_inspect (save_tree_1, yyvsp[0], cb_int0, 0); }
#line 10132 "parser.c"
    break;

  case 927: /* inspect_item: inspect_replacing  */
#line 5089 "parser.y"
                                { cb_emit_inspect (save_tree_1, yyvsp[0], cb_int1, 1); }
#line 10138 "parser.c"
    break;

  case 928: /* inspect_item: inspect_converting  */
#line 5090 "parser.y"
                                { cb_emit_inspect (save_tree_1, yyvsp[0], cb_int0, 2); }
#line 10144 "parser.c"
    break;

  case 929: /* $@65: %empty  */
#line 5096 "parser.y"
                                { cb_init_tarrying (); }
#line 10150 "parser.c"
    break;

  case 930: /* inspect_tallying: TALLYING $@65 tallying_list  */
#line 5097 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10156 "parser.c"
    break;

  case 931: /* tallying_list: tallying_item  */
#line 5101 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10162 "parser.c"
    break;

  case 932: /* tallying_list: tallying_list tallying_item  */
#line 5102 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 10168 "parser.c"
    break;

  case 933: /* tallying_item: simple_value FOR  */
#line 5106 "parser.y"
                                { yyval = cb_build_tarrying_data (yyvsp[-1]); }
#line 10174 "parser.c"
    break;

  case 934: /* tallying_item: CHARACTERS inspect_region  */
#line 5107 "parser.y"
                                { yyval = cb_build_tarrying_characters (yyvsp[0]); }
#line 10180 "parser.c"
    break;

  case 935: /* tallying_item: ALL  */
#line 5108 "parser.y"
                                { yyval = cb_build_tarrying_all (); }
#line 10186 "parser.c"
    break;

  case 936: /* tallying_item: LEADING  */
#line 5109 "parser.y"
                                { yyval = cb_build_tarrying_leading (); }
#line 10192 "parser.c"
    break;

  case 937: /* tallying_item: TRAILING  */
#line 5110 "parser.y"
                                { yyval = cb_build_tarrying_trailing (); }
#line 10198 "parser.c"
    break;

  case 938: /* tallying_item: simple_value inspect_region  */
#line 5111 "parser.y"
                                { yyval = cb_build_tarrying_value (yyvsp[-1], yyvsp[0]); }
#line 10204 "parser.c"
    break;

  case 939: /* inspect_replacing: REPLACING replacing_list  */
#line 5117 "parser.y"
                                { yyval = yyvsp[0]; inspect_keyword = 0; }
#line 10210 "parser.c"
    break;

  case 940: /* replacing_list: replacing_item  */
#line 5121 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10216 "parser.c"
    break;

  case 941: /* replacing_list: replacing_list replacing_item  */
#line 5122 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 10222 "parser.c"
    break;

  case 942: /* replacing_item: CHARACTERS BY simple_value inspect_region  */
#line 5127 "parser.y"
  {
	yyval = cb_build_replacing_characters (yyvsp[-1], yyvsp[0], save_tree_1);
	inspect_keyword = 0;
  }
#line 10231 "parser.c"
    break;

  case 943: /* replacing_item: rep_keyword replacing_region  */
#line 5131 "parser.y"
                                        { yyval = yyvsp[0]; }
#line 10237 "parser.c"
    break;

  case 944: /* rep_keyword: %empty  */
#line 5135 "parser.y"
                                { /* Nothing */ }
#line 10243 "parser.c"
    break;

  case 945: /* rep_keyword: ALL  */
#line 5136 "parser.y"
                                { inspect_keyword = 1; }
#line 10249 "parser.c"
    break;

  case 946: /* rep_keyword: LEADING  */
#line 5137 "parser.y"
                                { inspect_keyword = 2; }
#line 10255 "parser.c"
    break;

  case 947: /* rep_keyword: FIRST  */
#line 5138 "parser.y"
                                { inspect_keyword = 3; }
#line 10261 "parser.c"
    break;

  case 948: /* rep_keyword: TRAILING  */
#line 5139 "parser.y"
                                { inspect_keyword = 4; }
#line 10267 "parser.c"
    break;

  case 949: /* replacing_region: simple_value BY simple_all_value inspect_region  */
#line 5144 "parser.y"
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
#line 10292 "parser.c"
    break;

  case 950: /* inspect_converting: CONVERTING simple_value TO simple_all_value inspect_region  */
#line 5170 "parser.y"
  {
	if (cb_validate_inspect (save_tree_1, yyvsp[-3], yyvsp[-1]) < 0 ) {
		yyval = cb_error_node;
	} else {
		yyval = cb_build_converting (yyvsp[-3], yyvsp[-1], yyvsp[0]);
	}
  }
#line 10304 "parser.c"
    break;

  case 951: /* inspect_region: %empty  */
#line 5182 "parser.y"
                                { yyval = cb_build_inspect_region_start (); }
#line 10310 "parser.c"
    break;

  case 952: /* inspect_region: inspect_region before_or_after _initial x  */
#line 5184 "parser.y"
                                { yyval = cb_build_inspect_region (yyvsp[-3], yyvsp[-2], yyvsp[0]); }
#line 10316 "parser.c"
    break;

  case 955: /* $@66: %empty  */
#line 5195 "parser.y"
                                { BEGIN_STATEMENT ("MERGE", 0); }
#line 10322 "parser.c"
    break;

  case 957: /* $@67: %empty  */
#line 5205 "parser.y"
                                { BEGIN_STATEMENT ("MOVE", 0); }
#line 10328 "parser.c"
    break;

  case 959: /* move_body: x TO target_x_list  */
#line 5211 "parser.y"
  {
	cb_emit_move (yyvsp[-2], yyvsp[0]);
  }
#line 10336 "parser.c"
    break;

  case 960: /* move_body: CORRESPONDING x TO target_x_list  */
#line 5215 "parser.y"
  {
	cb_emit_move_corresponding (yyvsp[-2], yyvsp[0]);
  }
#line 10344 "parser.c"
    break;

  case 961: /* $@68: %empty  */
#line 5226 "parser.y"
                                { BEGIN_STATEMENT ("MULTIPLY", TERM_MULTIPLY); }
#line 10350 "parser.c"
    break;

  case 963: /* multiply_body: x BY arithmetic_x_list on_size_error  */
#line 5233 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '*', yyvsp[-3]);
  }
#line 10358 "parser.c"
    break;

  case 964: /* multiply_body: x BY x GIVING arithmetic_x_list on_size_error  */
#line 5237 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_op (yyvsp[-5], '*', yyvsp[-3]));
  }
#line 10366 "parser.c"
    break;

  case 965: /* end_multiply: %empty  */
#line 5243 "parser.y"
                                { terminator_warning (TERM_MULTIPLY); }
#line 10372 "parser.c"
    break;

  case 966: /* end_multiply: "END-MULTIPLY"  */
#line 5244 "parser.y"
                                { terminator_clear (TERM_MULTIPLY); }
#line 10378 "parser.c"
    break;

  case 967: /* $@69: %empty  */
#line 5253 "parser.y"
                                { BEGIN_STATEMENT ("OPEN", 0); }
#line 10384 "parser.c"
    break;

  case 970: /* open_list: open_list open_mode open_sharing file_name_list open_option  */
#line 5260 "parser.y"
  {
	cb_tree l;
	for (l = yyvsp[-1]; l; l = CB_CHAIN (l)) {
		if (CB_VALUE (l) != cb_error_node) {
			BEGIN_IMPLICIT_STATEMENT (l);
			cb_emit_open (CB_VALUE (l), yyvsp[-3], yyvsp[-2]);
		}
	}
  }
#line 10398 "parser.c"
    break;

  case 971: /* open_mode: INPUT  */
#line 5272 "parser.y"
                                { yyval = cb_int (COB_OPEN_INPUT); }
#line 10404 "parser.c"
    break;

  case 972: /* open_mode: OUTPUT  */
#line 5273 "parser.y"
                                { yyval = cb_int (COB_OPEN_OUTPUT); }
#line 10410 "parser.c"
    break;

  case 973: /* open_mode: "I-O"  */
#line 5274 "parser.y"
                                { yyval = cb_int (COB_OPEN_I_O); }
#line 10416 "parser.c"
    break;

  case 974: /* open_mode: EXTEND  */
#line 5275 "parser.y"
                                { yyval = cb_int (COB_OPEN_EXTEND); }
#line 10422 "parser.c"
    break;

  case 975: /* open_sharing: %empty  */
#line 5279 "parser.y"
                                { yyval = NULL; }
#line 10428 "parser.c"
    break;

  case 976: /* open_sharing: SHARING _with sharing_option  */
#line 5280 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10434 "parser.c"
    break;

  case 977: /* open_option: %empty  */
#line 5284 "parser.y"
                                { yyval = NULL; }
#line 10440 "parser.c"
    break;

  case 978: /* open_option: _with NO REWIND  */
#line 5285 "parser.y"
                                { yyval = NULL; }
#line 10446 "parser.c"
    break;

  case 979: /* open_option: _with LOCK  */
#line 5286 "parser.y"
                                { PENDING ("OPEN ... WITH LOCK"); }
#line 10452 "parser.c"
    break;

  case 980: /* $@70: %empty  */
#line 5298 "parser.y"
                                { BEGIN_STATEMENT ("PERFORM", TERM_PERFORM); }
#line 10458 "parser.c"
    break;

  case 982: /* perform_body: perform_procedure perform_option  */
#line 5304 "parser.y"
  {
	cb_emit_perform (yyvsp[0], yyvsp[-1]);
  }
#line 10466 "parser.c"
    break;

  case 983: /* $@71: %empty  */
#line 5308 "parser.y"
  {
	perform_stack = cb_cons (yyvsp[0], perform_stack);
	check_unreached = 0;
  }
#line 10475 "parser.c"
    break;

  case 984: /* perform_body: perform_option $@71 statement_list end_perform  */
#line 5313 "parser.y"
  {
	perform_stack = CB_CHAIN (perform_stack);
	cb_emit_perform (yyvsp[-3], yyvsp[-1]);
  }
#line 10484 "parser.c"
    break;

  case 985: /* perform_body: perform_option "END-PERFORM"  */
#line 5318 "parser.y"
  {
	cb_emit_perform (yyvsp[-1], NULL);
  }
#line 10492 "parser.c"
    break;

  case 986: /* end_perform: %empty  */
#line 5324 "parser.y"
                                { terminator_error (); }
#line 10498 "parser.c"
    break;

  case 987: /* end_perform: "END-PERFORM"  */
#line 5325 "parser.y"
                                { terminator_clear (TERM_PERFORM); }
#line 10504 "parser.c"
    break;

  case 988: /* perform_procedure: procedure_name  */
#line 5330 "parser.y"
  {
	CB_REFERENCE (yyvsp[0])->length = cb_true; /* return from $1 */
	yyval = cb_build_pair (yyvsp[0], yyvsp[0]);
  }
#line 10513 "parser.c"
    break;

  case 989: /* perform_procedure: procedure_name THRU procedure_name  */
#line 5335 "parser.y"
  {
	CB_REFERENCE (yyvsp[0])->length = cb_true; /* return from $3 */
	yyval = cb_build_pair (yyvsp[-2], yyvsp[0]);
  }
#line 10522 "parser.c"
    break;

  case 990: /* perform_option: %empty  */
#line 5343 "parser.y"
  {
	yyval = cb_build_perform_once (NULL);
  }
#line 10530 "parser.c"
    break;

  case 991: /* perform_option: FOREVER  */
#line 5347 "parser.y"
  {
	yyval = cb_build_perform_forever (NULL);
  }
#line 10538 "parser.c"
    break;

  case 992: /* perform_option: id_or_lit_or_func TIMES  */
#line 5351 "parser.y"
  {
	yyval = cb_build_perform_times (yyvsp[-1]);
	current_program->loop_counter++;
  }
#line 10547 "parser.c"
    break;

  case 993: /* perform_option: perform_test UNTIL condition  */
#line 5356 "parser.y"
  {
	cb_tree varying;

	varying = cb_list_init (cb_build_perform_varying (NULL, NULL, NULL, yyvsp[0]));
	yyval = cb_build_perform_until (yyvsp[-2], varying);
  }
#line 10558 "parser.c"
    break;

  case 994: /* perform_option: perform_test VARYING perform_varying_list  */
#line 5363 "parser.y"
  {
	yyval = cb_build_perform_until (yyvsp[-2], yyvsp[0]);
  }
#line 10566 "parser.c"
    break;

  case 995: /* perform_test: %empty  */
#line 5369 "parser.y"
                                { yyval = CB_BEFORE; }
#line 10572 "parser.c"
    break;

  case 996: /* perform_test: _with TEST before_or_after  */
#line 5370 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10578 "parser.c"
    break;

  case 997: /* perform_varying_list: perform_varying  */
#line 5374 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 10584 "parser.c"
    break;

  case 998: /* perform_varying_list: perform_varying_list AFTER perform_varying  */
#line 5376 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 10590 "parser.c"
    break;

  case 999: /* perform_varying: identifier FROM x BY x UNTIL condition  */
#line 5381 "parser.y"
  {
	yyval = cb_build_perform_varying (yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 10598 "parser.c"
    break;

  case 1000: /* $@72: %empty  */
#line 5392 "parser.y"
                                { BEGIN_STATEMENT ("READ", TERM_READ); }
#line 10604 "parser.c"
    break;

  case 1001: /* read_statement: READ $@72 file_name flag_next _record read_into with_lock read_key read_handler end_read  */
#line 5395 "parser.y"
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
#line 10630 "parser.c"
    break;

  case 1002: /* read_into: %empty  */
#line 5419 "parser.y"
                                { yyval = NULL; }
#line 10636 "parser.c"
    break;

  case 1003: /* read_into: INTO identifier  */
#line 5420 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10642 "parser.c"
    break;

  case 1004: /* with_lock: %empty  */
#line 5424 "parser.y"
                                { yyval = NULL; }
#line 10648 "parser.c"
    break;

  case 1005: /* with_lock: IGNORING LOCK  */
#line 5426 "parser.y"
  {
	yyval = cb_int3;
  }
#line 10656 "parser.c"
    break;

  case 1006: /* with_lock: _with LOCK  */
#line 5430 "parser.y"
  {
	yyval = cb_int1;
  }
#line 10664 "parser.c"
    break;

  case 1007: /* with_lock: _with NO LOCK  */
#line 5434 "parser.y"
  {
	yyval = cb_int2;
  }
#line 10672 "parser.c"
    break;

  case 1008: /* with_lock: _with IGNORE LOCK  */
#line 5438 "parser.y"
  {
	yyval = cb_int3;
  }
#line 10680 "parser.c"
    break;

  case 1009: /* with_lock: _with WAIT  */
#line 5442 "parser.y"
  {
	yyval = cb_int4;
  }
#line 10688 "parser.c"
    break;

  case 1010: /* read_key: %empty  */
#line 5448 "parser.y"
                                { yyval = NULL; }
#line 10694 "parser.c"
    break;

  case 1011: /* read_key: KEY _is identifier_list  */
#line 5450 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 10702 "parser.c"
    break;

  case 1015: /* end_read: %empty  */
#line 5461 "parser.y"
                                { terminator_warning (TERM_READ); }
#line 10708 "parser.c"
    break;

  case 1016: /* end_read: "END-READ"  */
#line 5462 "parser.y"
                                { terminator_clear (TERM_READ); }
#line 10714 "parser.c"
    break;

  case 1017: /* $@73: %empty  */
#line 5471 "parser.y"
                                { BEGIN_STATEMENT ("RELEASE", 0); }
#line 10720 "parser.c"
    break;

  case 1018: /* release_statement: RELEASE $@73 record_name write_from  */
#line 5473 "parser.y"
  {
	if (yyvsp[-1] != cb_error_node) {
		cb_emit_release (yyvsp[-1], yyvsp[0]);
	}
  }
#line 10730 "parser.c"
    break;

  case 1019: /* $@74: %empty  */
#line 5486 "parser.y"
                                { BEGIN_STATEMENT ("RETURN", TERM_RETURN); }
#line 10736 "parser.c"
    break;

  case 1020: /* return_statement: RETURN $@74 file_name _record read_into at_end end_return  */
#line 5489 "parser.y"
  {
	if (yyvsp[-4] != cb_error_node) {
		cb_emit_return (yyvsp[-4], yyvsp[-2]);
	}
  }
#line 10746 "parser.c"
    break;

  case 1021: /* end_return: %empty  */
#line 5497 "parser.y"
                                { terminator_warning (TERM_RETURN); }
#line 10752 "parser.c"
    break;

  case 1022: /* end_return: "END-RETURN"  */
#line 5498 "parser.y"
                                { terminator_clear (TERM_RETURN); }
#line 10758 "parser.c"
    break;

  case 1023: /* $@75: %empty  */
#line 5507 "parser.y"
                                { BEGIN_STATEMENT ("REWRITE", TERM_REWRITE); }
#line 10764 "parser.c"
    break;

  case 1024: /* rewrite_statement: REWRITE $@75 record_name write_from write_lock opt_invalid_key end_rewrite  */
#line 5510 "parser.y"
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
#line 10780 "parser.c"
    break;

  case 1025: /* write_lock: %empty  */
#line 5524 "parser.y"
                                { yyval = NULL; }
#line 10786 "parser.c"
    break;

  case 1026: /* write_lock: _with LOCK  */
#line 5526 "parser.y"
  {
	yyval = cb_int1;
  }
#line 10794 "parser.c"
    break;

  case 1027: /* write_lock: _with NO LOCK  */
#line 5530 "parser.y"
  {
	yyval = cb_int2;
  }
#line 10802 "parser.c"
    break;

  case 1028: /* end_rewrite: %empty  */
#line 5536 "parser.y"
                                { terminator_warning (TERM_REWRITE); }
#line 10808 "parser.c"
    break;

  case 1029: /* end_rewrite: "END-REWRITE"  */
#line 5537 "parser.y"
                                { terminator_clear (TERM_REWRITE); }
#line 10814 "parser.c"
    break;

  case 1030: /* rollback_statement: ROLLBACK  */
#line 5547 "parser.y"
  {
	BEGIN_STATEMENT ("ROLLBACK", 0);
	cb_emit_rollback ();
  }
#line 10823 "parser.c"
    break;

  case 1031: /* $@76: %empty  */
#line 5559 "parser.y"
                                { BEGIN_STATEMENT ("SEARCH", TERM_SEARCH); }
#line 10829 "parser.c"
    break;

  case 1033: /* search_body: table_name search_varying search_at_end search_whens  */
#line 5566 "parser.y"
  {
	cb_emit_search (yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]);
  }
#line 10837 "parser.c"
    break;

  case 1034: /* $@77: %empty  */
#line 5570 "parser.y"
  {
	check_unreached = 0;
  }
#line 10845 "parser.c"
    break;

  case 1035: /* search_body: ALL table_name search_at_end WHEN expr $@77 statement_list  */
#line 5574 "parser.y"
  {
	cb_emit_search_all (yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 10853 "parser.c"
    break;

  case 1036: /* search_varying: %empty  */
#line 5580 "parser.y"
                                { yyval = NULL; }
#line 10859 "parser.c"
    break;

  case 1037: /* search_varying: VARYING identifier  */
#line 5581 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10865 "parser.c"
    break;

  case 1038: /* search_at_end: %empty  */
#line 5585 "parser.y"
                                { yyval = NULL; }
#line 10871 "parser.c"
    break;

  case 1039: /* $@78: %empty  */
#line 5587 "parser.y"
  {
	check_unreached = 0;
  }
#line 10879 "parser.c"
    break;

  case 1040: /* search_at_end: _at END $@78 statement_list  */
#line 5591 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 10887 "parser.c"
    break;

  case 1041: /* search_whens: search_when  */
#line 5597 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10893 "parser.c"
    break;

  case 1042: /* search_whens: search_when search_whens  */
#line 5598 "parser.y"
                                { yyval = yyvsp[-1]; CB_IF (yyvsp[-1])->stmt2 = yyvsp[0]; }
#line 10899 "parser.c"
    break;

  case 1043: /* $@79: %empty  */
#line 5603 "parser.y"
  {
	check_unreached = 0;
  }
#line 10907 "parser.c"
    break;

  case 1044: /* search_when: WHEN condition $@79 statement_list  */
#line 5607 "parser.y"
  {
	yyval = cb_build_if (yyvsp[-2], yyvsp[0], NULL);
  }
#line 10915 "parser.c"
    break;

  case 1045: /* end_search: %empty  */
#line 5613 "parser.y"
                                { terminator_warning (TERM_SEARCH); }
#line 10921 "parser.c"
    break;

  case 1046: /* end_search: "END-SEARCH"  */
#line 5614 "parser.y"
                                { terminator_clear (TERM_SEARCH); }
#line 10927 "parser.c"
    break;

  case 1047: /* $@80: %empty  */
#line 5623 "parser.y"
                                { BEGIN_STATEMENT ("SET", 0); }
#line 10933 "parser.c"
    break;

  case 1054: /* set_environment: ENVIRONMENT simple_value TO simple_value  */
#line 5639 "parser.y"
  {
	cb_emit_setenv (yyvsp[-2], yyvsp[0]);
  }
#line 10941 "parser.c"
    break;

  case 1055: /* set_to: target_x_list TO ENTRY alnum_or_id  */
#line 5648 "parser.y"
  {
	cb_emit_set_to (yyvsp[-3], cb_build_ppointer (yyvsp[0]));
  }
#line 10949 "parser.c"
    break;

  case 1056: /* set_to: target_x_list TO x  */
#line 5652 "parser.y"
  {
	cb_emit_set_to (yyvsp[-2], yyvsp[0]);
  }
#line 10957 "parser.c"
    break;

  case 1057: /* set_up_down: target_x_list up_or_down BY x  */
#line 5661 "parser.y"
  {
	cb_emit_set_up_down (yyvsp[-3], yyvsp[-2], yyvsp[0]);
  }
#line 10965 "parser.c"
    break;

  case 1058: /* up_or_down: UP  */
#line 5667 "parser.y"
                                { yyval = cb_int0; }
#line 10971 "parser.c"
    break;

  case 1059: /* up_or_down: DOWN  */
#line 5668 "parser.y"
                                { yyval = cb_int1; }
#line 10977 "parser.c"
    break;

  case 1062: /* set_to_on_off: mnemonic_name_list TO on_or_off  */
#line 5680 "parser.y"
  {
	cb_emit_set_on_off (yyvsp[-2], yyvsp[0]);
  }
#line 10985 "parser.c"
    break;

  case 1065: /* set_to_true_false: target_x_list TO "TRUE"  */
#line 5694 "parser.y"
  {
	cb_emit_set_true (yyvsp[-2]);
  }
#line 10993 "parser.c"
    break;

  case 1066: /* set_to_true_false: target_x_list TO "FALSE"  */
#line 5698 "parser.y"
  {
	cb_emit_set_false (yyvsp[-2]);
  }
#line 11001 "parser.c"
    break;

  case 1067: /* $@81: %empty  */
#line 5709 "parser.y"
                                { BEGIN_STATEMENT ("SORT", 0); }
#line 11007 "parser.c"
    break;

  case 1069: /* $@82: %empty  */
#line 5715 "parser.y"
  {
	cb_emit_sort_init (yyvsp[-3], yyvsp[-2], yyvsp[0]);
	if (CB_FILE_P (cb_ref (yyvsp[-3])) && yyvsp[-2] == NULL) {
		cb_error (_("File sort requires KEY phrase"));
	}
	/* used in sort_input/sort_output */
	save_tree_1 = yyvsp[-3];
  }
#line 11020 "parser.c"
    break;

  case 1070: /* sort_body: qualified_word sort_key_list sort_duplicates sort_collating $@82 sort_input sort_output  */
#line 5724 "parser.y"
  {
	cb_emit_sort_finish (yyvsp[-6]);
  }
#line 11028 "parser.c"
    break;

  case 1071: /* sort_key_list: %empty  */
#line 5731 "parser.y"
  {
	yyval = NULL;
  }
#line 11036 "parser.c"
    break;

  case 1072: /* sort_key_list: sort_key_list _on ascending_or_descending _key _is opt_key_list  */
#line 5736 "parser.y"
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
#line 11057 "parser.c"
    break;

  case 1073: /* opt_key_list: %empty  */
#line 5755 "parser.y"
                                { yyval = NULL; }
#line 11063 "parser.c"
    break;

  case 1074: /* opt_key_list: opt_key_list qualified_word  */
#line 5756 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 11069 "parser.c"
    break;

  case 1076: /* sort_duplicates: with_dups _in_order  */
#line 5760 "parser.y"
                                { /* nothing */ }
#line 11075 "parser.c"
    break;

  case 1077: /* sort_collating: %empty  */
#line 5764 "parser.y"
                                        { yyval = cb_null; }
#line 11081 "parser.c"
    break;

  case 1078: /* sort_collating: coll_sequence _is reference  */
#line 5765 "parser.y"
                                        { yyval = cb_ref (yyvsp[0]); }
#line 11087 "parser.c"
    break;

  case 1079: /* sort_input: %empty  */
#line 5770 "parser.y"
  {
	if (CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("File sort requires USING or INPUT PROCEDURE"));
	}
  }
#line 11097 "parser.c"
    break;

  case 1080: /* sort_input: USING file_name_list  */
#line 5776 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("USING invalid with table SORT"));
	} else {
		cb_emit_sort_using (save_tree_1, yyvsp[0]);
	}
  }
#line 11109 "parser.c"
    break;

  case 1081: /* sort_input: INPUT PROCEDURE _is perform_procedure  */
#line 5784 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("INPUT PROCEDURE invalid with table SORT"));
	} else {
		cb_emit_sort_input (yyvsp[0], save_tree_1);
	}
  }
#line 11121 "parser.c"
    break;

  case 1082: /* sort_output: %empty  */
#line 5795 "parser.y"
  {
	if (CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("File sort requires GIVING or OUTPUT PROCEDURE"));
	}
  }
#line 11131 "parser.c"
    break;

  case 1083: /* sort_output: GIVING file_name_list  */
#line 5801 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("GIVING invalid with table SORT"));
	} else {
		cb_emit_sort_giving (save_tree_1, yyvsp[0]);
	}
  }
#line 11143 "parser.c"
    break;

  case 1084: /* sort_output: OUTPUT PROCEDURE _is perform_procedure  */
#line 5809 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("OUTPUT PROCEDURE invalid with table SORT"));
	} else {
		cb_emit_sort_output (yyvsp[0], save_tree_1);
	}
  }
#line 11155 "parser.c"
    break;

  case 1085: /* $@83: %empty  */
#line 5824 "parser.y"
                                { BEGIN_STATEMENT ("START", TERM_START); }
#line 11161 "parser.c"
    break;

  case 1086: /* @84: %empty  */
#line 5825 "parser.y"
                                { yyval = cb_int (COB_EQ); }
#line 11167 "parser.c"
    break;

  case 1087: /* start_statement: START $@83 file_name @84 start_key opt_invalid_key end_start  */
#line 5828 "parser.y"
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
#line 11186 "parser.c"
    break;

  case 1088: /* start_key: %empty  */
#line 5845 "parser.y"
                                { yyval = NULL; }
#line 11192 "parser.c"
    break;

  case 1089: /* start_key: KEY _is start_op identifier_list  */
#line 5847 "parser.y"
  {
	yyvsp[-4] = yyvsp[-1];
	yyval = yyvsp[0];
  }
#line 11201 "parser.c"
    break;

  case 1090: /* start_op: flag_not eq  */
#line 5854 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_NE : COB_EQ); }
#line 11207 "parser.c"
    break;

  case 1091: /* start_op: flag_not gt  */
#line 5855 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_LE : COB_GT); }
#line 11213 "parser.c"
    break;

  case 1092: /* start_op: flag_not lt  */
#line 5856 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_GE : COB_LT); }
#line 11219 "parser.c"
    break;

  case 1093: /* start_op: flag_not ge  */
#line 5857 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_LT : COB_GE); }
#line 11225 "parser.c"
    break;

  case 1094: /* start_op: flag_not le  */
#line 5858 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_GT : COB_LE); }
#line 11231 "parser.c"
    break;

  case 1095: /* end_start: %empty  */
#line 5862 "parser.y"
                                { terminator_warning (TERM_START); }
#line 11237 "parser.c"
    break;

  case 1096: /* end_start: "END-START"  */
#line 5863 "parser.y"
                                { terminator_clear (TERM_START); }
#line 11243 "parser.c"
    break;

  case 1097: /* $@85: %empty  */
#line 5872 "parser.y"
                                { BEGIN_STATEMENT ("STOP", 0); }
#line 11249 "parser.c"
    break;

  case 1098: /* stop_statement: STOP RUN $@85 stop_returning  */
#line 5874 "parser.y"
  {
	cb_emit_stop_run (yyvsp[0]);
  }
#line 11257 "parser.c"
    break;

  case 1099: /* $@86: %empty  */
#line 5877 "parser.y"
                                { BEGIN_STATEMENT ("STOP", 0); }
#line 11263 "parser.c"
    break;

  case 1100: /* stop_statement: STOP "Literal" $@86  */
#line 5878 "parser.y"
  {
	cb_verify (cb_stop_literal_statement, "STOP literal");
  }
#line 11271 "parser.c"
    break;

  case 1101: /* stop_returning: %empty  */
#line 5884 "parser.y"
                        { yyval = current_program->cb_return_code; }
#line 11277 "parser.c"
    break;

  case 1102: /* stop_returning: RETURNING x  */
#line 5885 "parser.y"
                        { yyval = yyvsp[0]; }
#line 11283 "parser.c"
    break;

  case 1103: /* stop_returning: GIVING x  */
#line 5886 "parser.y"
                        { yyval = yyvsp[0]; }
#line 11289 "parser.c"
    break;

  case 1104: /* $@87: %empty  */
#line 5895 "parser.y"
                                { BEGIN_STATEMENT ("STRING", TERM_STRING); }
#line 11295 "parser.c"
    break;

  case 1105: /* string_statement: STRING $@87 string_item_list INTO identifier opt_with_pointer on_overflow end_string  */
#line 5898 "parser.y"
  {
	cb_emit_string (yyvsp[-5], yyvsp[-3], yyvsp[-2]);
  }
#line 11303 "parser.c"
    break;

  case 1106: /* string_item_list: string_item  */
#line 5904 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 11309 "parser.c"
    break;

  case 1107: /* string_item_list: string_item_list string_item  */
#line 5905 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 11315 "parser.c"
    break;

  case 1108: /* string_item: x  */
#line 5909 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11321 "parser.c"
    break;

  case 1109: /* string_item: DELIMITED _by SIZE  */
#line 5910 "parser.y"
                                { yyval = cb_build_pair (cb_int0, NULL); }
#line 11327 "parser.c"
    break;

  case 1110: /* string_item: DELIMITED _by x  */
#line 5911 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], NULL); }
#line 11333 "parser.c"
    break;

  case 1111: /* opt_with_pointer: %empty  */
#line 5915 "parser.y"
                                { yyval = cb_int0; }
#line 11339 "parser.c"
    break;

  case 1112: /* opt_with_pointer: _with POINTER identifier  */
#line 5916 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11345 "parser.c"
    break;

  case 1113: /* end_string: %empty  */
#line 5920 "parser.y"
                                { terminator_warning (TERM_STRING); }
#line 11351 "parser.c"
    break;

  case 1114: /* end_string: "END-STRING"  */
#line 5921 "parser.y"
                                { terminator_clear (TERM_STRING); }
#line 11357 "parser.c"
    break;

  case 1115: /* $@88: %empty  */
#line 5930 "parser.y"
                                { BEGIN_STATEMENT ("SUBTRACT", TERM_SUBTRACT); }
#line 11363 "parser.c"
    break;

  case 1117: /* subtract_body: x_list FROM arithmetic_x_list on_size_error  */
#line 5937 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '-', cb_build_binary_list (yyvsp[-3], '+'));
  }
#line 11371 "parser.c"
    break;

  case 1118: /* subtract_body: x_list FROM x GIVING arithmetic_x_list on_size_error  */
#line 5941 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_list (cb_cons (yyvsp[-3], yyvsp[-5]), '-'));
  }
#line 11379 "parser.c"
    break;

  case 1119: /* subtract_body: CORRESPONDING identifier FROM identifier flag_rounded on_size_error  */
#line 5945 "parser.y"
  {
	cb_emit_corresponding (cb_build_sub, yyvsp[-2], yyvsp[-4], yyvsp[-1]);
  }
#line 11387 "parser.c"
    break;

  case 1120: /* end_subtract: %empty  */
#line 5951 "parser.y"
                                { terminator_warning (TERM_SUBTRACT); }
#line 11393 "parser.c"
    break;

  case 1121: /* end_subtract: "END-SUBTRACT"  */
#line 5952 "parser.y"
                                { terminator_clear (TERM_SUBTRACT); }
#line 11399 "parser.c"
    break;

  case 1122: /* suppress_statement: SUPPRESS _printing  */
#line 5962 "parser.y"
  {
	BEGIN_STATEMENT ("SUPPRESS", 0);
	PENDING("SUPPRESS");
  }
#line 11408 "parser.c"
    break;

  case 1125: /* $@89: %empty  */
#line 5977 "parser.y"
                                { BEGIN_STATEMENT ("TERMINATE", 0); }
#line 11414 "parser.c"
    break;

  case 1126: /* terminate_statement: TERMINATE $@89 identifier_list  */
#line 5979 "parser.y"
  {
	PENDING("TERMINATE");
  }
#line 11422 "parser.c"
    break;

  case 1127: /* $@90: %empty  */
#line 5990 "parser.y"
                                { BEGIN_STATEMENT ("TRANSFORM", 0); }
#line 11428 "parser.c"
    break;

  case 1128: /* transform_statement: TRANSFORM $@90 identifier FROM simple_value TO simple_all_value  */
#line 5992 "parser.y"
  {
	cb_tree		x;

	x = cb_build_converting (yyvsp[-2], yyvsp[0], cb_build_inspect_region_start ());
	cb_emit_inspect (yyvsp[-4], x, cb_int0, 2);
  }
#line 11439 "parser.c"
    break;

  case 1129: /* $@91: %empty  */
#line 6006 "parser.y"
                                { BEGIN_STATEMENT ("UNLOCK", 0); }
#line 11445 "parser.c"
    break;

  case 1130: /* unlock_statement: UNLOCK $@91 file_name opt_record  */
#line 6008 "parser.y"
  {
	if (yyvsp[-1] != cb_error_node) {
		cb_emit_unlock (yyvsp[-1]);
	}
  }
#line 11455 "parser.c"
    break;

  case 1134: /* $@92: %empty  */
#line 6027 "parser.y"
                                { BEGIN_STATEMENT ("UNSTRING", TERM_UNSTRING); }
#line 11461 "parser.c"
    break;

  case 1135: /* unstring_statement: UNSTRING $@92 identifier unstring_delimited unstring_into opt_with_pointer unstring_tallying on_overflow end_unstring  */
#line 6031 "parser.y"
  {
	cb_emit_unstring (yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2]);
  }
#line 11469 "parser.c"
    break;

  case 1136: /* unstring_delimited: %empty  */
#line 6037 "parser.y"
                                { yyval = NULL; }
#line 11475 "parser.c"
    break;

  case 1137: /* unstring_delimited: DELIMITED _by unstring_delimited_list  */
#line 6039 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11481 "parser.c"
    break;

  case 1138: /* unstring_delimited_list: unstring_delimited_item  */
#line 6043 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 11487 "parser.c"
    break;

  case 1139: /* unstring_delimited_list: unstring_delimited_list OR unstring_delimited_item  */
#line 6045 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 11493 "parser.c"
    break;

  case 1140: /* unstring_delimited_item: flag_all simple_value  */
#line 6050 "parser.y"
  {
	yyval = cb_build_unstring_delimited (yyvsp[-1], yyvsp[0]);
  }
#line 11501 "parser.c"
    break;

  case 1141: /* unstring_into: INTO unstring_into_item  */
#line 6056 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 11507 "parser.c"
    break;

  case 1142: /* unstring_into: unstring_into unstring_into_item  */
#line 6058 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 11513 "parser.c"
    break;

  case 1143: /* unstring_into_item: identifier unstring_into_delimiter unstring_into_count  */
#line 6063 "parser.y"
  {
	yyval = cb_build_unstring_into (yyvsp[-2], yyvsp[-1], yyvsp[0]);
  }
#line 11521 "parser.c"
    break;

  case 1144: /* unstring_into_delimiter: %empty  */
#line 6069 "parser.y"
                                { yyval = NULL; }
#line 11527 "parser.c"
    break;

  case 1145: /* unstring_into_delimiter: DELIMITER _in identifier  */
#line 6070 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11533 "parser.c"
    break;

  case 1146: /* unstring_into_count: %empty  */
#line 6074 "parser.y"
                                { yyval = NULL; }
#line 11539 "parser.c"
    break;

  case 1147: /* unstring_into_count: COUNT _in identifier  */
#line 6075 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11545 "parser.c"
    break;

  case 1148: /* unstring_tallying: %empty  */
#line 6079 "parser.y"
                                { yyval = NULL; }
#line 11551 "parser.c"
    break;

  case 1149: /* unstring_tallying: TALLYING _in identifier  */
#line 6080 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11557 "parser.c"
    break;

  case 1150: /* end_unstring: %empty  */
#line 6084 "parser.y"
                                { terminator_warning (TERM_UNSTRING); }
#line 11563 "parser.c"
    break;

  case 1151: /* end_unstring: "END-UNSTRING"  */
#line 6085 "parser.y"
                                { terminator_clear (TERM_UNSTRING); }
#line 11569 "parser.c"
    break;

  case 1155: /* use_exception: USE use_global _after _standard exception_or_error _procedure _on use_exception_target  */
#line 6103 "parser.y"
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
#line 11591 "parser.c"
    break;

  case 1156: /* use_global: %empty  */
#line 6124 "parser.y"
  {
	use_global_ind = 0;
  }
#line 11599 "parser.c"
    break;

  case 1157: /* use_global: GLOBAL  */
#line 6128 "parser.y"
  {
	use_global_ind = 1;
	current_program->flag_global_use = 1;
  }
#line 11608 "parser.c"
    break;

  case 1158: /* use_exception_target: file_name_list  */
#line 6136 "parser.y"
  {
	cb_tree		l;

	for (l = yyvsp[0]; l; l = CB_CHAIN (l)) {
		if (CB_VALUE (l) != cb_error_node) {
			setup_use_file (CB_FILE (cb_ref (CB_VALUE (l))));
		}
	}
  }
#line 11622 "parser.c"
    break;

  case 1159: /* use_exception_target: INPUT  */
#line 6146 "parser.y"
  {
	current_program->global_handler[COB_OPEN_INPUT].handler_label = current_section;
	current_program->global_handler[COB_OPEN_INPUT].handler_prog = current_program;
  }
#line 11631 "parser.c"
    break;

  case 1160: /* use_exception_target: OUTPUT  */
#line 6151 "parser.y"
  {
	current_program->global_handler[COB_OPEN_OUTPUT].handler_label = current_section;
	current_program->global_handler[COB_OPEN_OUTPUT].handler_prog = current_program;
  }
#line 11640 "parser.c"
    break;

  case 1161: /* use_exception_target: "I-O"  */
#line 6156 "parser.y"
  {
	current_program->global_handler[COB_OPEN_I_O].handler_label = current_section;
	current_program->global_handler[COB_OPEN_I_O].handler_prog = current_program;
  }
#line 11649 "parser.c"
    break;

  case 1162: /* use_exception_target: EXTEND  */
#line 6161 "parser.y"
  {
	current_program->global_handler[COB_OPEN_EXTEND].handler_label = current_section;
	current_program->global_handler[COB_OPEN_EXTEND].handler_prog = current_program;
  }
#line 11658 "parser.c"
    break;

  case 1175: /* use_debugging: USE _for DEBUGGING _on use_debugging_target  */
#line 6193 "parser.y"
  {
	PENDING ("USE FOR DEBUGGING");
  }
#line 11666 "parser.c"
    break;

  case 1178: /* use_reporting: USE use_global BEFORE REPORTING identifier  */
#line 6205 "parser.y"
  {
	PENDING ("USE BEFORE REPORTING");
  }
#line 11674 "parser.c"
    break;

  case 1179: /* $@93: %empty  */
#line 6216 "parser.y"
                                { BEGIN_STATEMENT ("WRITE", TERM_WRITE); }
#line 11680 "parser.c"
    break;

  case 1180: /* write_statement: WRITE $@93 record_name write_from write_lock write_option write_handler end_write  */
#line 6219 "parser.y"
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
#line 11696 "parser.c"
    break;

  case 1181: /* write_from: %empty  */
#line 6233 "parser.y"
                                { yyval = NULL; }
#line 11702 "parser.c"
    break;

  case 1182: /* write_from: FROM id_or_lit  */
#line 6234 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11708 "parser.c"
    break;

  case 1183: /* write_option: %empty  */
#line 6239 "parser.y"
  {
	yyval = cb_int0;
  }
#line 11716 "parser.c"
    break;

  case 1184: /* write_option: before_or_after _advancing num_id_or_lit _line_or_lines  */
#line 6243 "parser.y"
  {
	yyval = cb_build_write_advancing_lines (yyvsp[-3], yyvsp[-1]);
  }
#line 11724 "parser.c"
    break;

  case 1185: /* write_option: before_or_after _advancing mnemonic_name  */
#line 6247 "parser.y"
  {
	yyval = cb_build_write_advancing_mnemonic (yyvsp[-2], yyvsp[0]);
  }
#line 11732 "parser.c"
    break;

  case 1186: /* write_option: before_or_after _advancing PAGE  */
#line 6251 "parser.y"
  {
	yyval = cb_build_write_advancing_page (yyvsp[-2]);
  }
#line 11740 "parser.c"
    break;

  case 1187: /* before_or_after: BEFORE  */
#line 6257 "parser.y"
                                { yyval = CB_BEFORE; }
#line 11746 "parser.c"
    break;

  case 1188: /* before_or_after: AFTER  */
#line 6258 "parser.y"
                                { yyval = CB_AFTER; }
#line 11752 "parser.c"
    break;

  case 1192: /* end_write: %empty  */
#line 6267 "parser.y"
                                { terminator_warning (TERM_WRITE); }
#line 11758 "parser.c"
    break;

  case 1193: /* end_write: "END-WRITE"  */
#line 6268 "parser.y"
                                { terminator_clear (TERM_WRITE); }
#line 11764 "parser.c"
    break;

  case 1194: /* on_accp_exception: opt_on_exception opt_not_on_exception  */
#line 6283 "parser.y"
  {
	current_statement->handler_id = COB_EC_IMP_ACCEPT;
  }
#line 11772 "parser.c"
    break;

  case 1195: /* on_disp_exception: opt_on_exception opt_not_on_exception  */
#line 6291 "parser.y"
  {
	current_statement->handler_id = COB_EC_IMP_DISPLAY;
  }
#line 11780 "parser.c"
    break;

  case 1197: /* $@94: %empty  */
#line 6298 "parser.y"
  {
	check_unreached = 0;
  }
#line 11788 "parser.c"
    break;

  case 1198: /* opt_on_exception: EXCEPTION $@94 statement_list  */
#line 6302 "parser.y"
  {
	current_statement->handler1 = yyvsp[0];
  }
#line 11796 "parser.c"
    break;

  case 1200: /* $@95: %empty  */
#line 6309 "parser.y"
  {
	check_unreached = 0;
  }
#line 11804 "parser.c"
    break;

  case 1201: /* opt_not_on_exception: "NOT EXCEPTION" $@95 statement_list  */
#line 6313 "parser.y"
  {
	current_statement->handler2 = yyvsp[0];
  }
#line 11812 "parser.c"
    break;

  case 1204: /* $@96: %empty  */
#line 6329 "parser.y"
  {
	check_unreached = 0;
	current_statement->handler_id = COB_EC_SIZE;
  }
#line 11821 "parser.c"
    break;

  case 1205: /* opt_on_size_error: "SIZE ERROR" $@96 statement_list  */
#line 6334 "parser.y"
  {
	current_statement->handler1 = yyvsp[0];
  }
#line 11829 "parser.c"
    break;

  case 1207: /* $@97: %empty  */
#line 6341 "parser.y"
  {
	check_unreached = 0;
	current_statement->handler_id = COB_EC_SIZE;
  }
#line 11838 "parser.c"
    break;

  case 1208: /* opt_not_on_size_error: "NOT SIZE ERROR" $@97 statement_list  */
#line 6346 "parser.y"
  {
	current_statement->handler2 = yyvsp[0];
  }
#line 11846 "parser.c"
    break;

  case 1209: /* on_overflow: opt_on_overflow opt_not_on_overflow  */
#line 6358 "parser.y"
  {
	current_statement->handler_id = COB_EC_OVERFLOW;
  }
#line 11854 "parser.c"
    break;

  case 1211: /* $@98: %empty  */
#line 6365 "parser.y"
  {
	check_unreached = 0;
  }
#line 11862 "parser.c"
    break;

  case 1212: /* opt_on_overflow: OVERFLOW $@98 statement_list  */
#line 6369 "parser.y"
  {
	current_statement->handler1 = yyvsp[0];
  }
#line 11870 "parser.c"
    break;

  case 1214: /* $@99: %empty  */
#line 6376 "parser.y"
  {
	check_unreached = 0;
  }
#line 11878 "parser.c"
    break;

  case 1215: /* opt_not_on_overflow: "NOT OVERFLOW" $@99 statement_list  */
#line 6380 "parser.y"
  {
	current_statement->handler2 = yyvsp[0];
  }
#line 11886 "parser.c"
    break;

  case 1216: /* at_end: at_end_sentence  */
#line 6392 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_AT_END;
	current_statement->handler1 = yyvsp[0];
  }
#line 11895 "parser.c"
    break;

  case 1217: /* at_end: not_at_end_sentence  */
#line 6397 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_AT_END;
	current_statement->handler2 = yyvsp[0];
  }
#line 11904 "parser.c"
    break;

  case 1218: /* at_end: at_end_sentence not_at_end_sentence  */
#line 6402 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_AT_END;
	current_statement->handler1 = yyvsp[-1];
	current_statement->handler2 = yyvsp[0];
  }
#line 11914 "parser.c"
    break;

  case 1219: /* $@100: %empty  */
#line 6411 "parser.y"
  {
	check_unreached = 0;
  }
#line 11922 "parser.c"
    break;

  case 1220: /* at_end_sentence: END $@100 statement_list  */
#line 6415 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 11930 "parser.c"
    break;

  case 1221: /* $@101: %empty  */
#line 6422 "parser.y"
  {
	check_unreached = 0;
  }
#line 11938 "parser.c"
    break;

  case 1222: /* not_at_end_sentence: "NOT END" $@101 statement_list  */
#line 6426 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 11946 "parser.c"
    break;

  case 1223: /* at_eop: at_eop_sentence  */
#line 6438 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_EOP;
	current_statement->handler1 = yyvsp[0];
  }
#line 11955 "parser.c"
    break;

  case 1224: /* at_eop: not_at_eop_sentence  */
#line 6443 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_EOP;
	current_statement->handler2 = yyvsp[0];
  }
#line 11964 "parser.c"
    break;

  case 1225: /* at_eop: at_eop_sentence not_at_eop_sentence  */
#line 6448 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_EOP;
	current_statement->handler1 = yyvsp[-1];
	current_statement->handler2 = yyvsp[0];
  }
#line 11974 "parser.c"
    break;

  case 1226: /* $@102: %empty  */
#line 6457 "parser.y"
  {
	check_unreached = 0;
  }
#line 11982 "parser.c"
    break;

  case 1227: /* at_eop_sentence: EOP $@102 statement_list  */
#line 6461 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 11990 "parser.c"
    break;

  case 1228: /* $@103: %empty  */
#line 6468 "parser.y"
  {
	check_unreached = 0;
  }
#line 11998 "parser.c"
    break;

  case 1229: /* not_at_eop_sentence: "NOT EOP" $@103 statement_list  */
#line 6472 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12006 "parser.c"
    break;

  case 1232: /* invalid_key: invalid_key_sentence  */
#line 6488 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_INVALID_KEY;
	current_statement->handler1 = yyvsp[0];
  }
#line 12015 "parser.c"
    break;

  case 1233: /* invalid_key: not_invalid_key_sentence  */
#line 6493 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_INVALID_KEY;
	current_statement->handler2 = yyvsp[0];
  }
#line 12024 "parser.c"
    break;

  case 1234: /* invalid_key: invalid_key_sentence not_invalid_key_sentence  */
#line 6498 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_INVALID_KEY;
	current_statement->handler1 = yyvsp[-1];
	current_statement->handler2 = yyvsp[0];
  }
#line 12034 "parser.c"
    break;

  case 1235: /* $@104: %empty  */
#line 6507 "parser.y"
  {
	check_unreached = 0;
  }
#line 12042 "parser.c"
    break;

  case 1236: /* invalid_key_sentence: "INVALID KEY" $@104 statement_list  */
#line 6511 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12050 "parser.c"
    break;

  case 1237: /* $@105: %empty  */
#line 6518 "parser.y"
  {
	check_unreached = 0;
  }
#line 12058 "parser.c"
    break;

  case 1238: /* not_invalid_key_sentence: "NOT INVALID KEY" $@105 statement_list  */
#line 6522 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12066 "parser.c"
    break;

  case 1239: /* _opt_scroll_lines: %empty  */
#line 6534 "parser.y"
  {
	yyval = cb_one;
  }
#line 12074 "parser.c"
    break;

  case 1240: /* _opt_scroll_lines: _by num_id_or_lit _line_or_lines  */
#line 6538 "parser.y"
  {
	yyval = yyvsp[-1];
  }
#line 12082 "parser.c"
    break;

  case 1241: /* condition: expr  */
#line 6550 "parser.y"
  {
	yyval = cb_build_cond (yyvsp[0]);
  }
#line 12090 "parser.c"
    break;

  case 1242: /* expr: partial_expr  */
#line 6557 "parser.y"
  {
	yyval = cb_build_expr (yyvsp[0]);
  }
#line 12098 "parser.c"
    break;

  case 1243: /* $@106: %empty  */
#line 6563 "parser.y"
  {
	current_expr = NULL;
  }
#line 12106 "parser.c"
    break;

  case 1244: /* partial_expr: $@106 expr_tokens  */
#line 6567 "parser.y"
  {
	yyval = cb_list_reverse (current_expr);
  }
#line 12114 "parser.c"
    break;

  case 1245: /* expr_tokens: expr_token x  */
#line 6573 "parser.y"
                        { push_expr ('x', yyvsp[0]); }
#line 12120 "parser.c"
    break;

  case 1246: /* expr_tokens: expr_tokens ')'  */
#line 6574 "parser.y"
                        { push_expr (')', NULL); }
#line 12126 "parser.c"
    break;

  case 1247: /* expr_tokens: expr_token OMITTED  */
#line 6576 "parser.y"
                                { push_expr ('O', NULL); }
#line 12132 "parser.c"
    break;

  case 1248: /* expr_tokens: expr_token NUMERIC  */
#line 6577 "parser.y"
                                { push_expr ('9', NULL); }
#line 12138 "parser.c"
    break;

  case 1249: /* expr_tokens: expr_token ALPHABETIC  */
#line 6578 "parser.y"
                                { push_expr ('A', NULL); }
#line 12144 "parser.c"
    break;

  case 1250: /* expr_tokens: expr_token "ALPHABETIC-LOWER"  */
#line 6579 "parser.y"
                                { push_expr ('L', NULL); }
#line 12150 "parser.c"
    break;

  case 1251: /* expr_tokens: expr_token "ALPHABETIC-UPPER"  */
#line 6580 "parser.y"
                                { push_expr ('U', NULL); }
#line 12156 "parser.c"
    break;

  case 1252: /* expr_tokens: expr_token CLASS_NAME  */
#line 6581 "parser.y"
                                { push_expr ('x', yyvsp[0]); }
#line 12162 "parser.c"
    break;

  case 1253: /* expr_tokens: expr_tokens OMITTED  */
#line 6583 "parser.y"
                                        { push_expr ('O', NULL); }
#line 12168 "parser.c"
    break;

  case 1254: /* expr_tokens: expr_tokens NUMERIC  */
#line 6584 "parser.y"
                                        { push_expr ('9', NULL); }
#line 12174 "parser.c"
    break;

  case 1255: /* expr_tokens: expr_tokens ALPHABETIC  */
#line 6585 "parser.y"
                                        { push_expr ('A', NULL); }
#line 12180 "parser.c"
    break;

  case 1256: /* expr_tokens: expr_tokens "ALPHABETIC-LOWER"  */
#line 6586 "parser.y"
                                        { push_expr ('L', NULL); }
#line 12186 "parser.c"
    break;

  case 1257: /* expr_tokens: expr_tokens "ALPHABETIC-UPPER"  */
#line 6587 "parser.y"
                                        { push_expr ('U', NULL); }
#line 12192 "parser.c"
    break;

  case 1258: /* expr_tokens: expr_tokens CLASS_NAME  */
#line 6588 "parser.y"
                                        { push_expr ('x', yyvsp[0]); }
#line 12198 "parser.c"
    break;

  case 1259: /* expr_tokens: expr_token POSITIVE  */
#line 6590 "parser.y"
                        { push_expr ('P', NULL); }
#line 12204 "parser.c"
    break;

  case 1260: /* expr_tokens: expr_token NEGATIVE  */
#line 6591 "parser.y"
                        { push_expr ('N', NULL); }
#line 12210 "parser.c"
    break;

  case 1261: /* expr_tokens: expr_tokens POSITIVE  */
#line 6593 "parser.y"
                        { push_expr ('P', NULL); }
#line 12216 "parser.c"
    break;

  case 1262: /* expr_tokens: expr_tokens NEGATIVE  */
#line 6594 "parser.y"
                        { push_expr ('N', NULL); }
#line 12222 "parser.c"
    break;

  case 1263: /* expr_tokens: expr_tokens ZERO  */
#line 6595 "parser.y"
                        { push_expr ('x', cb_zero); }
#line 12228 "parser.c"
    break;

  case 1267: /* expr_token: expr_token '('  */
#line 6602 "parser.y"
                        { push_expr ('(', NULL); }
#line 12234 "parser.c"
    break;

  case 1268: /* expr_token: expr_token '+'  */
#line 6604 "parser.y"
                        { push_expr ('+', NULL); }
#line 12240 "parser.c"
    break;

  case 1269: /* expr_token: expr_token '-'  */
#line 6605 "parser.y"
                        { push_expr ('-', NULL); }
#line 12246 "parser.c"
    break;

  case 1270: /* expr_token: expr_token '^'  */
#line 6606 "parser.y"
                        { push_expr ('^', NULL); }
#line 12252 "parser.c"
    break;

  case 1271: /* expr_token: expr_token NOT  */
#line 6608 "parser.y"
                        { push_expr ('!', NULL); }
#line 12258 "parser.c"
    break;

  case 1272: /* expr_token: expr_tokens NOT  */
#line 6609 "parser.y"
                        { push_expr ('!', NULL); }
#line 12264 "parser.c"
    break;

  case 1273: /* expr_token: expr_tokens '+'  */
#line 6611 "parser.y"
                        { push_expr ('+', NULL); }
#line 12270 "parser.c"
    break;

  case 1274: /* expr_token: expr_tokens '-'  */
#line 6612 "parser.y"
                        { push_expr ('-', NULL); }
#line 12276 "parser.c"
    break;

  case 1275: /* expr_token: expr_tokens '*'  */
#line 6613 "parser.y"
                        { push_expr ('*', NULL); }
#line 12282 "parser.c"
    break;

  case 1276: /* expr_token: expr_tokens '/'  */
#line 6614 "parser.y"
                        { push_expr ('/', NULL); }
#line 12288 "parser.c"
    break;

  case 1277: /* expr_token: expr_tokens '^'  */
#line 6615 "parser.y"
                        { push_expr ('^', NULL); }
#line 12294 "parser.c"
    break;

  case 1278: /* expr_token: expr_tokens eq  */
#line 6617 "parser.y"
                        { push_expr ('=', NULL); }
#line 12300 "parser.c"
    break;

  case 1279: /* expr_token: expr_tokens gt  */
#line 6618 "parser.y"
                        { push_expr ('>', NULL); }
#line 12306 "parser.c"
    break;

  case 1280: /* expr_token: expr_tokens lt  */
#line 6619 "parser.y"
                        { push_expr ('<', NULL); }
#line 12312 "parser.c"
    break;

  case 1281: /* expr_token: expr_tokens ge  */
#line 6620 "parser.y"
                        { push_expr (']', NULL); }
#line 12318 "parser.c"
    break;

  case 1282: /* expr_token: expr_tokens le  */
#line 6621 "parser.y"
                        { push_expr ('[', NULL); }
#line 12324 "parser.c"
    break;

  case 1283: /* expr_token: expr_tokens NE  */
#line 6622 "parser.y"
                        { push_expr ('~', NULL); }
#line 12330 "parser.c"
    break;

  case 1284: /* expr_token: expr_token eq  */
#line 6624 "parser.y"
                        { push_expr ('=', NULL); }
#line 12336 "parser.c"
    break;

  case 1285: /* expr_token: expr_token gt  */
#line 6625 "parser.y"
                        { push_expr ('>', NULL); }
#line 12342 "parser.c"
    break;

  case 1286: /* expr_token: expr_token lt  */
#line 6626 "parser.y"
                        { push_expr ('<', NULL); }
#line 12348 "parser.c"
    break;

  case 1287: /* expr_token: expr_token ge  */
#line 6627 "parser.y"
                        { push_expr (']', NULL); }
#line 12354 "parser.c"
    break;

  case 1288: /* expr_token: expr_token le  */
#line 6628 "parser.y"
                        { push_expr ('[', NULL); }
#line 12360 "parser.c"
    break;

  case 1289: /* expr_token: expr_token NE  */
#line 6629 "parser.y"
                        { push_expr ('~', NULL); }
#line 12366 "parser.c"
    break;

  case 1290: /* expr_token: expr_tokens AND  */
#line 6631 "parser.y"
                        { push_expr ('&', NULL); }
#line 12372 "parser.c"
    break;

  case 1291: /* expr_token: expr_tokens OR  */
#line 6632 "parser.y"
                        { push_expr ('|', NULL); }
#line 12378 "parser.c"
    break;

  case 1305: /* exp_list: exp  */
#line 6644 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12384 "parser.c"
    break;

  case 1306: /* exp_list: exp_list e_sep exp  */
#line 6645 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 12390 "parser.c"
    break;

  case 1310: /* exp: arith_x  */
#line 6654 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12396 "parser.c"
    break;

  case 1311: /* exp: exp '+' exp  */
#line 6655 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '+', yyvsp[0]); }
#line 12402 "parser.c"
    break;

  case 1312: /* exp: exp '-' exp  */
#line 6656 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '-', yyvsp[0]); }
#line 12408 "parser.c"
    break;

  case 1313: /* exp: exp '*' exp  */
#line 6657 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '*', yyvsp[0]); }
#line 12414 "parser.c"
    break;

  case 1314: /* exp: exp '/' exp  */
#line 6658 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '/', yyvsp[0]); }
#line 12420 "parser.c"
    break;

  case 1315: /* exp: '+' exp  */
#line 6659 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12426 "parser.c"
    break;

  case 1316: /* exp: '-' exp  */
#line 6660 "parser.y"
                                { yyval = cb_build_binary_op (cb_zero, '-', yyvsp[0]); }
#line 12432 "parser.c"
    break;

  case 1317: /* exp: exp '^' exp  */
#line 6661 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '^', yyvsp[0]); }
#line 12438 "parser.c"
    break;

  case 1318: /* exp: '(' exp ')'  */
#line 6662 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 12444 "parser.c"
    break;

  case 1319: /* linage_counter: "LINAGE-COUNTER"  */
#line 6674 "parser.y"
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
#line 12460 "parser.c"
    break;

  case 1320: /* linage_counter: "LINAGE-COUNTER" in_of "Identifier"  */
#line 6686 "parser.y"
  {
	if (CB_FILE_P (cb_ref (yyvsp[0]))) {
		yyval = CB_FILE (cb_ref (yyvsp[0]))->linage_ctr;
	} else {
		cb_error_x (yyvsp[0], _("'%s' is not a file name"), CB_NAME (yyvsp[0]));
		yyval = cb_error_node;
	}
  }
#line 12473 "parser.c"
    break;

  case 1321: /* arithmetic_x_list: arithmetic_x  */
#line 6700 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12479 "parser.c"
    break;

  case 1322: /* arithmetic_x_list: arithmetic_x_list arithmetic_x  */
#line 6702 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 12485 "parser.c"
    break;

  case 1323: /* arithmetic_x: x flag_rounded  */
#line 6706 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], yyvsp[-1]); }
#line 12491 "parser.c"
    break;

  case 1324: /* record_name: qualified_word  */
#line 6713 "parser.y"
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
#line 12512 "parser.c"
    break;

  case 1325: /* table_name: qualified_word  */
#line 6735 "parser.y"
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
#line 12531 "parser.c"
    break;

  case 1326: /* file_name_list: file_name  */
#line 6755 "parser.y"
  {
	yyval = cb_list_init (yyvsp[0]);
  }
#line 12539 "parser.c"
    break;

  case 1327: /* file_name_list: file_name_list file_name  */
#line 6759 "parser.y"
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
#line 12556 "parser.c"
    break;

  case 1328: /* file_name: "Identifier"  */
#line 6775 "parser.y"
  {
	if (CB_FILE_P (cb_ref (yyvsp[0]))) {
		yyval = yyvsp[0];
	} else {
		cb_error_x (yyvsp[0], _("'%s' is not a file name"), CB_NAME (yyvsp[0]));
		yyval = cb_error_node;
	}
  }
#line 12569 "parser.c"
    break;

  case 1329: /* mnemonic_name_list: mnemonic_name  */
#line 6788 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12575 "parser.c"
    break;

  case 1330: /* mnemonic_name_list: mnemonic_name_list mnemonic_name  */
#line 6790 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12581 "parser.c"
    break;

  case 1331: /* mnemonic_name: "MNEMONIC NAME"  */
#line 6794 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12587 "parser.c"
    break;

  case 1332: /* procedure_name_list: %empty  */
#line 6800 "parser.y"
                                { yyval = NULL; }
#line 12593 "parser.c"
    break;

  case 1333: /* procedure_name_list: procedure_name_list procedure_name  */
#line 6802 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12599 "parser.c"
    break;

  case 1334: /* procedure_name: label  */
#line 6807 "parser.y"
  {
	yyval = yyvsp[0];
	CB_REFERENCE (yyval)->offset = CB_TREE (current_section);
	current_program->label_list = cb_cons (yyval, current_program->label_list);
  }
#line 12609 "parser.c"
    break;

  case 1338: /* integer_label: "Literal"  */
#line 6822 "parser.y"
  {
	yyval = cb_build_reference ((char *)(CB_LITERAL (yyvsp[0])->data));
	yyval->source_file = yyvsp[0]->source_file;
	yyval->source_line = yyvsp[0]->source_line;
  }
#line 12619 "parser.c"
    break;

  case 1339: /* reference_list: reference  */
#line 6832 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12625 "parser.c"
    break;

  case 1340: /* reference_list: reference_list reference  */
#line 6833 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12631 "parser.c"
    break;

  case 1341: /* reference: qualified_word  */
#line 6838 "parser.y"
  {
	yyval = yyvsp[0];
	current_program->reference_list = cb_cons (yyval, current_program->reference_list);
  }
#line 12640 "parser.c"
    break;

  case 1342: /* no_reference_list: qualified_word  */
#line 6847 "parser.y"
                                        { yyval = cb_list_init (yyvsp[0]); }
#line 12646 "parser.c"
    break;

  case 1343: /* no_reference_list: no_reference_list qualified_word  */
#line 6848 "parser.y"
                                        { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12652 "parser.c"
    break;

  case 1344: /* opt_reference: %empty  */
#line 6852 "parser.y"
                                { yyval = NULL; }
#line 12658 "parser.c"
    break;

  case 1345: /* opt_reference: reference  */
#line 6853 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12664 "parser.c"
    break;

  case 1348: /* undefined_word: "Identifier"  */
#line 6865 "parser.y"
  {
	yyval = yyvsp[0];
	if (CB_REFERENCE (yyval)->word->count > 0) {
		redefinition_error (yyval);
		yyval = cb_error_node;
	}
  }
#line 12676 "parser.c"
    break;

  case 1349: /* target_x_list: target_x  */
#line 6884 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12682 "parser.c"
    break;

  case 1350: /* target_x_list: target_x_list target_x  */
#line 6885 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12688 "parser.c"
    break;

  case 1352: /* target_x: ADDRESS _of identifier_1  */
#line 6890 "parser.y"
                                { yyval = cb_build_address (yyvsp[0]); }
#line 12694 "parser.c"
    break;

  case 1353: /* x_list: x  */
#line 6894 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12700 "parser.c"
    break;

  case 1354: /* x_list: x_list x  */
#line 6895 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12706 "parser.c"
    break;

  case 1356: /* x: LENGTH _of identifier_1  */
#line 6900 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12712 "parser.c"
    break;

  case 1357: /* x: LENGTH _of basic_literal  */
#line 6901 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12718 "parser.c"
    break;

  case 1358: /* x: LENGTH _of function  */
#line 6902 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12724 "parser.c"
    break;

  case 1359: /* x: ADDRESS _of prog_or_entry alnum_or_id  */
#line 6903 "parser.y"
                                                { yyval = cb_build_ppointer (yyvsp[0]); }
#line 12730 "parser.c"
    break;

  case 1360: /* x: ADDRESS _of identifier_1  */
#line 6904 "parser.y"
                                                { yyval = cb_build_address (yyvsp[0]); }
#line 12736 "parser.c"
    break;

  case 1365: /* arith_x: LENGTH _of identifier_1  */
#line 6912 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12742 "parser.c"
    break;

  case 1366: /* arith_x: LENGTH _of basic_literal  */
#line 6913 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12748 "parser.c"
    break;

  case 1367: /* arith_x: LENGTH _of function  */
#line 6914 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12754 "parser.c"
    break;

  case 1373: /* alnum_or_id: identifier_1  */
#line 6926 "parser.y"
                        { yyval = yyvsp[0]; }
#line 12760 "parser.c"
    break;

  case 1374: /* alnum_or_id: "Literal"  */
#line 6927 "parser.y"
                        { yyval = yyvsp[0]; }
#line 12766 "parser.c"
    break;

  case 1386: /* num_id_or_lit: ZERO  */
#line 6961 "parser.y"
                                { yyval = cb_zero; }
#line 12772 "parser.c"
    break;

  case 1387: /* identifier: identifier_1  */
#line 6969 "parser.y"
                                { yyval = cb_build_identifier (yyvsp[0]); }
#line 12778 "parser.c"
    break;

  case 1388: /* identifier_1: qualified_word  */
#line 6973 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12784 "parser.c"
    break;

  case 1389: /* identifier_1: qualified_word subref  */
#line 6974 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 12790 "parser.c"
    break;

  case 1390: /* identifier_1: qualified_word refmod  */
#line 6975 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 12796 "parser.c"
    break;

  case 1391: /* identifier_1: qualified_word subref refmod  */
#line 6976 "parser.y"
                                { yyval = yyvsp[-2]; }
#line 12802 "parser.c"
    break;

  case 1392: /* qualified_word: "Identifier"  */
#line 6980 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12808 "parser.c"
    break;

  case 1393: /* qualified_word: "Identifier" in_of qualified_word  */
#line 6981 "parser.y"
                                { yyval = yyvsp[-2]; CB_REFERENCE (yyvsp[-2])->chain = yyvsp[0]; }
#line 12814 "parser.c"
    break;

  case 1394: /* subref: '(' exp_list ')'  */
#line 6986 "parser.y"
  {
	if (cb_ref (yyvsp[-3]) != cb_error_node) {
		yyval = yyvsp[-3];
		CB_REFERENCE (yyvsp[-3])->subs = cb_list_reverse (yyvsp[-1]);
	}
  }
#line 12825 "parser.c"
    break;

  case 1395: /* refmod: '(' exp ':' ')'  */
#line 6996 "parser.y"
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
#line 12843 "parser.c"
    break;

  case 1396: /* refmod: '(' exp ':' exp ')'  */
#line 7010 "parser.y"
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
#line 12863 "parser.c"
    break;

  case 1397: /* integer: "Literal"  */
#line 7033 "parser.y"
  {
	if (cb_tree_category (yyvsp[0]) != CB_CATEGORY_NUMERIC) {
		cb_error (_("Integer value expected"));
	} else if (CB_LITERAL (yyvsp[0])->sign < 0 || CB_LITERAL (yyvsp[0])->scale) {
		cb_error (_("Integer value expected"));
	}
	yyval = yyvsp[0];
  }
#line 12876 "parser.c"
    break;

  case 1398: /* literal: basic_literal  */
#line 7044 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12882 "parser.c"
    break;

  case 1399: /* literal: ALL basic_value  */
#line 7046 "parser.y"
  {
	yyval = yyvsp[0];
	if (CB_LITERAL_P (yyvsp[0])) {
		CB_LITERAL (yyvsp[0])->all = 1;
	}
  }
#line 12893 "parser.c"
    break;

  case 1400: /* basic_literal: basic_value  */
#line 7055 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12899 "parser.c"
    break;

  case 1401: /* basic_literal: basic_literal '&' basic_value  */
#line 7056 "parser.y"
                                { yyval = cb_concat_literals (yyvsp[-2], yyvsp[0]); }
#line 12905 "parser.c"
    break;

  case 1402: /* basic_value: "Literal"  */
#line 7060 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12911 "parser.c"
    break;

  case 1403: /* basic_value: SPACE  */
#line 7061 "parser.y"
                                { yyval = cb_space; }
#line 12917 "parser.c"
    break;

  case 1404: /* basic_value: ZERO  */
#line 7062 "parser.y"
                                { yyval = cb_zero; }
#line 12923 "parser.c"
    break;

  case 1405: /* basic_value: QUOTE  */
#line 7063 "parser.y"
                                { yyval = cb_quote; }
#line 12929 "parser.c"
    break;

  case 1406: /* basic_value: "HIGH-VALUE"  */
#line 7064 "parser.y"
                                { yyval = cb_high; }
#line 12935 "parser.c"
    break;

  case 1407: /* basic_value: "LOW-VALUE"  */
#line 7065 "parser.y"
                                { yyval = cb_low; }
#line 12941 "parser.c"
    break;

  case 1408: /* basic_value: "NULL"  */
#line 7066 "parser.y"
                                { yyval = cb_null; }
#line 12947 "parser.c"
    break;

  case 1409: /* function: "FUNCTION CURRENT-DATE" func_refmod  */
#line 7075 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-1], NULL, yyvsp[0]);
  }
#line 12955 "parser.c"
    break;

  case 1410: /* function: "FUNCTION WHEN-COMPILED" func_refmod  */
#line 7079 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-1], NULL, yyvsp[0]);
  }
#line 12963 "parser.c"
    break;

  case 1411: /* function: "FUNCTION UPPER-CASE" '(' exp ')' func_refmod  */
#line 7083 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], cb_list_init (yyvsp[-2]), yyvsp[0]);
  }
#line 12971 "parser.c"
    break;

  case 1412: /* function: "FUNCTION LOWER-CASE" '(' exp ')' func_refmod  */
#line 7087 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], cb_list_init (yyvsp[-2]), yyvsp[0]);
  }
#line 12979 "parser.c"
    break;

  case 1413: /* function: "FUNCTION REVERSE" '(' exp ')' func_refmod  */
#line 7091 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], cb_list_init (yyvsp[-2]), yyvsp[0]);
  }
#line 12987 "parser.c"
    break;

  case 1414: /* function: "FUNCTION CONCATENATE" '(' exp_list ')' func_refmod  */
#line 7095 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 12995 "parser.c"
    break;

  case 1415: /* function: "FUNCTION SUBSTITUTE" '(' exp_list ')' func_refmod  */
#line 7099 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13003 "parser.c"
    break;

  case 1416: /* function: "FUNCTION SUBSTITUTE-CASE" '(' exp_list ')' func_refmod  */
#line 7103 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13011 "parser.c"
    break;

  case 1417: /* function: "FUNCTION TRIM" '(' trim_args ')' func_refmod  */
#line 7107 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13019 "parser.c"
    break;

  case 1418: /* function: "FUNCTION NUMVALC" '(' numvalc_args ')'  */
#line 7111 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-3], yyvsp[-1], NULL);
  }
#line 13027 "parser.c"
    break;

  case 1419: /* function: "FUNCTION LOCALE" '(' locale_dt_args ')' func_refmod  */
#line 7115 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13035 "parser.c"
    break;

  case 1420: /* function: "FUNCTION" func_args  */
#line 7119 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-1], yyvsp[0], NULL);
  }
#line 13043 "parser.c"
    break;

  case 1421: /* func_refmod: %empty  */
#line 7125 "parser.y"
                                { yyval = NULL; }
#line 13049 "parser.c"
    break;

  case 1422: /* func_refmod: '(' exp ':' ')'  */
#line 7126 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-2], NULL); }
#line 13055 "parser.c"
    break;

  case 1423: /* func_refmod: '(' exp ':' exp ')'  */
#line 7127 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-3], yyvsp[-1]); }
#line 13061 "parser.c"
    break;

  case 1424: /* func_args: %empty  */
#line 7131 "parser.y"
                                { yyval = NULL; }
#line 13067 "parser.c"
    break;

  case 1425: /* func_args: '(' list_func_args ')'  */
#line 7132 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 13073 "parser.c"
    break;

  case 1426: /* list_func_args: %empty  */
#line 7136 "parser.y"
                                { yyval = NULL; }
#line 13079 "parser.c"
    break;

  case 1427: /* list_func_args: exp_list  */
#line 7137 "parser.y"
                                { yyval = yyvsp[0]; }
#line 13085 "parser.c"
    break;

  case 1428: /* trim_args: exp  */
#line 7143 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[0]);
	yyval = cb_list_add (x, cb_int0);
  }
#line 13096 "parser.c"
    break;

  case 1429: /* trim_args: exp e_sep LEADING  */
#line 7150 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, cb_int1);
  }
#line 13107 "parser.c"
    break;

  case 1430: /* trim_args: exp e_sep TRAILING  */
#line 7157 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, cb_int2);
  }
#line 13118 "parser.c"
    break;

  case 1431: /* numvalc_args: exp  */
#line 7167 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[0]);
	yyval = cb_list_add (x, cb_null);
  }
#line 13129 "parser.c"
    break;

  case 1432: /* numvalc_args: exp e_sep exp  */
#line 7174 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, yyvsp[0]);
  }
#line 13140 "parser.c"
    break;

  case 1433: /* locale_dt_args: exp  */
#line 7184 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[0]);
	yyval = cb_list_add (x, cb_null);
  }
#line 13151 "parser.c"
    break;

  case 1434: /* locale_dt_args: exp e_sep reference  */
#line 7191 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, cb_ref (yyvsp[0]));
  }
#line 13162 "parser.c"
    break;

  case 1435: /* not_const_word: %empty  */
#line 7204 "parser.y"
  {
	non_const_word = 1;
  }
#line 13170 "parser.c"
    break;

  case 1436: /* flag_all: %empty  */
#line 7214 "parser.y"
                                { yyval = cb_int0; }
#line 13176 "parser.c"
    break;

  case 1437: /* flag_all: ALL  */
#line 7215 "parser.y"
                                { yyval = cb_int1; }
#line 13182 "parser.c"
    break;

  case 1438: /* flag_duplicates: %empty  */
#line 7219 "parser.y"
                                { yyval = cb_int0; }
#line 13188 "parser.c"
    break;

  case 1439: /* flag_duplicates: with_dups  */
#line 7220 "parser.y"
                                { yyval = cb_int1; }
#line 13194 "parser.c"
    break;

  case 1440: /* flag_initialized: %empty  */
#line 7224 "parser.y"
                                { yyval = NULL; }
#line 13200 "parser.c"
    break;

  case 1441: /* flag_initialized: INITIALIZED  */
#line 7225 "parser.y"
                                { yyval = cb_int1; }
#line 13206 "parser.c"
    break;

  case 1442: /* flag_next: %empty  */
#line 7229 "parser.y"
                                { yyval = cb_int0; }
#line 13212 "parser.c"
    break;

  case 1443: /* flag_next: NEXT  */
#line 7230 "parser.y"
                                { yyval = cb_int1; }
#line 13218 "parser.c"
    break;

  case 1444: /* flag_next: PREVIOUS  */
#line 7231 "parser.y"
                                { yyval = cb_int2; }
#line 13224 "parser.c"
    break;

  case 1445: /* flag_not: %empty  */
#line 7235 "parser.y"
                                { yyval = cb_int0; }
#line 13230 "parser.c"
    break;

  case 1446: /* flag_not: NOT  */
#line 7236 "parser.y"
                                { yyval = cb_int1; }
#line 13236 "parser.c"
    break;

  case 1447: /* flag_optional: %empty  */
#line 7240 "parser.y"
                                { yyval = cb_int0; }
#line 13242 "parser.c"
    break;

  case 1448: /* flag_optional: OPTIONAL  */
#line 7241 "parser.y"
                                { yyval = cb_int1; }
#line 13248 "parser.c"
    break;

  case 1449: /* flag_rounded: %empty  */
#line 7245 "parser.y"
                                { yyval = cb_int0; }
#line 13254 "parser.c"
    break;

  case 1450: /* flag_rounded: ROUNDED  */
#line 7246 "parser.y"
                                { yyval = cb_int1; }
#line 13260 "parser.c"
    break;

  case 1451: /* flag_separate: %empty  */
#line 7250 "parser.y"
                                { yyval = cb_int0; }
#line 13266 "parser.c"
    break;

  case 1452: /* flag_separate: SEPARATE _character  */
#line 7251 "parser.y"
                                { yyval = cb_int1; }
#line 13272 "parser.c"
    break;

  case 1464: /* _also: ALSO  */
#line 7264 "parser.y"
                       { yyval = cb_int1; }
#line 13278 "parser.c"
    break;

  case 1493: /* _is: %empty  */
#line 7279 "parser.y"
                { yyval = NULL; }
#line 13284 "parser.c"
    break;

  case 1494: /* _is: IS  */
#line 7279 "parser.y"
                                    { yyval = cb_int1; }
#line 13290 "parser.c"
    break;

  case 1505: /* _literal: %empty  */
#line 7284 "parser.y"
                { yyval = NULL; }
#line 13296 "parser.c"
    break;

  case 1506: /* _literal: "Literal"  */
#line 7284 "parser.y"
                                         { yyval = yyvsp[0]; }
#line 13302 "parser.c"
    break;

  case 1551: /* exec_sql_statement: "EXEC SQL statement"  */
#line 7314 "parser.y"
  {
	cb_tree sql_node;
	BEGIN_STATEMENT ("EXEC SQL", 0);
	sql_node = cb_parse_exec_sql ((char *)CB_LITERAL (yyvsp[0])->data);
	if (sql_node != cb_error_node) {
		current_statement->body =
			cb_list_add (current_statement->body, sql_node);
	}
  }
#line 13316 "parser.c"
    break;


#line 13320 "parser.c"

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

#line 7326 "parser.y"


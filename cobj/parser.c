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
  YYSYMBOL_EXEC_JAVA_IMPORT_STATEMENT = 451, /* "EXEC JAVA IMPORT statement"  */
  YYSYMBOL_EXEC_JAVA_MEMBER_STATEMENT = 452, /* "EXEC JAVA CLASS-MEMBER statement"  */
  YYSYMBOL_453_ = 453,                     /* '+'  */
  YYSYMBOL_454_ = 454,                     /* '-'  */
  YYSYMBOL_455_ = 455,                     /* '*'  */
  YYSYMBOL_456_ = 456,                     /* '/'  */
  YYSYMBOL_UNARY_SIGN = 457,               /* UNARY_SIGN  */
  YYSYMBOL_458_ = 458,                     /* '^'  */
  YYSYMBOL_459_ = 459,                     /* '.'  */
  YYSYMBOL_460_ = 460,                     /* '='  */
  YYSYMBOL_461_ = 461,                     /* ')'  */
  YYSYMBOL_462_ = 462,                     /* '('  */
  YYSYMBOL_463_ = 463,                     /* '>'  */
  YYSYMBOL_464_ = 464,                     /* '<'  */
  YYSYMBOL_465_ = 465,                     /* ':'  */
  YYSYMBOL_466_ = 466,                     /* '&'  */
  YYSYMBOL_YYACCEPT = 467,                 /* $accept  */
  YYSYMBOL_start = 468,                    /* start  */
  YYSYMBOL_469_1 = 469,                    /* $@1  */
  YYSYMBOL_nested_list = 470,              /* nested_list  */
  YYSYMBOL_source_element = 471,           /* source_element  */
  YYSYMBOL_program_definition = 472,       /* program_definition  */
  YYSYMBOL_473_2 = 473,                    /* $@2  */
  YYSYMBOL_474_3 = 474,                    /* $@3  */
  YYSYMBOL_program_mandatory = 475,        /* program_mandatory  */
  YYSYMBOL_476_4 = 476,                    /* $@4  */
  YYSYMBOL_477_5 = 477,                    /* $@5  */
  YYSYMBOL_function_definition = 478,      /* function_definition  */
  YYSYMBOL_479_6 = 479,                    /* $@6  */
  YYSYMBOL_480_7 = 480,                    /* $@7  */
  YYSYMBOL_nested_prog = 481,              /* nested_prog  */
  YYSYMBOL_end_program = 482,              /* end_program  */
  YYSYMBOL_end_mandatory = 483,            /* end_mandatory  */
  YYSYMBOL_end_function = 484,             /* end_function  */
  YYSYMBOL_identification_division = 485,  /* identification_division  */
  YYSYMBOL_486_8 = 486,                    /* $@8  */
  YYSYMBOL_function_division = 487,        /* function_division  */
  YYSYMBOL_program_name = 488,             /* program_name  */
  YYSYMBOL_as_literal = 489,               /* as_literal  */
  YYSYMBOL_program_type = 490,             /* program_type  */
  YYSYMBOL_program_type_clause = 491,      /* program_type_clause  */
  YYSYMBOL__init_or_recurs = 492,          /* _init_or_recurs  */
  YYSYMBOL_environment_division = 493,     /* environment_division  */
  YYSYMBOL_configuration_section = 494,    /* configuration_section  */
  YYSYMBOL_configuration_list = 495,       /* configuration_list  */
  YYSYMBOL_configuration_paragraph = 496,  /* configuration_paragraph  */
  YYSYMBOL_source_computer_paragraph = 497, /* source_computer_paragraph  */
  YYSYMBOL_source_computer_entry = 498,    /* source_computer_entry  */
  YYSYMBOL_with_debugging_mode = 499,      /* with_debugging_mode  */
  YYSYMBOL_computer_name = 500,            /* computer_name  */
  YYSYMBOL_object_computer_paragraph = 501, /* object_computer_paragraph  */
  YYSYMBOL_object_computer_entry = 502,    /* object_computer_entry  */
  YYSYMBOL_object_clauses_list = 503,      /* object_clauses_list  */
  YYSYMBOL_object_clauses = 504,           /* object_clauses  */
  YYSYMBOL_object_computer_memory = 505,   /* object_computer_memory  */
  YYSYMBOL_object_char_or_word = 506,      /* object_char_or_word  */
  YYSYMBOL_object_computer_sequence = 507, /* object_computer_sequence  */
  YYSYMBOL_object_computer_segment = 508,  /* object_computer_segment  */
  YYSYMBOL_repository_paragraph = 509,     /* repository_paragraph  */
  YYSYMBOL_opt_repository = 510,           /* opt_repository  */
  YYSYMBOL_repository_list = 511,          /* repository_list  */
  YYSYMBOL_repository_name = 512,          /* repository_name  */
  YYSYMBOL_repository_literal_list = 513,  /* repository_literal_list  */
  YYSYMBOL_special_names_paragraph = 514,  /* special_names_paragraph  */
  YYSYMBOL_opt_special_names = 515,        /* opt_special_names  */
  YYSYMBOL_special_name_list = 516,        /* special_name_list  */
  YYSYMBOL_special_name = 517,             /* special_name  */
  YYSYMBOL_mnemonic_name_clause = 518,     /* mnemonic_name_clause  */
  YYSYMBOL_519_9 = 519,                    /* $@9  */
  YYSYMBOL_520_10 = 520,                   /* $@10  */
  YYSYMBOL_special_name_mnemonic_on_off_list = 521, /* special_name_mnemonic_on_off_list  */
  YYSYMBOL_special_name_mnemonic_on_off_list_mandatory = 522, /* special_name_mnemonic_on_off_list_mandatory  */
  YYSYMBOL_special_name_mnemonic_on_off = 523, /* special_name_mnemonic_on_off  */
  YYSYMBOL_on_or_off = 524,                /* on_or_off  */
  YYSYMBOL_alphabet_name_clause = 525,     /* alphabet_name_clause  */
  YYSYMBOL_526_11 = 526,                   /* $@11  */
  YYSYMBOL_alphabet_definition = 527,      /* alphabet_definition  */
  YYSYMBOL_alphabet_literal_list = 528,    /* alphabet_literal_list  */
  YYSYMBOL_alphabet_literal = 529,         /* alphabet_literal  */
  YYSYMBOL_530_12 = 530,                   /* @12  */
  YYSYMBOL_alphabet_also_sequence = 531,   /* alphabet_also_sequence  */
  YYSYMBOL_alphabet_lits = 532,            /* alphabet_lits  */
  YYSYMBOL_alphabet_also_literal = 533,    /* alphabet_also_literal  */
  YYSYMBOL_symbolic_characters_clause = 534, /* symbolic_characters_clause  */
  YYSYMBOL_symbolic_characters_list = 535, /* symbolic_characters_list  */
  YYSYMBOL_char_list = 536,                /* char_list  */
  YYSYMBOL_integer_list = 537,             /* integer_list  */
  YYSYMBOL_class_name_clause = 538,        /* class_name_clause  */
  YYSYMBOL_class_item_list = 539,          /* class_item_list  */
  YYSYMBOL_class_item = 540,               /* class_item  */
  YYSYMBOL_locale_clause = 541,            /* locale_clause  */
  YYSYMBOL_currency_sign_clause = 542,     /* currency_sign_clause  */
  YYSYMBOL_decimal_point_clause = 543,     /* decimal_point_clause  */
  YYSYMBOL_cursor_clause = 544,            /* cursor_clause  */
  YYSYMBOL_crt_status_clause = 545,        /* crt_status_clause  */
  YYSYMBOL_screen_control = 546,           /* screen_control  */
  YYSYMBOL_event_status = 547,             /* event_status  */
  YYSYMBOL_input_output_section = 548,     /* input_output_section  */
  YYSYMBOL_549_13 = 549,                   /* $@13  */
  YYSYMBOL_550_14 = 550,                   /* $@14  */
  YYSYMBOL_file_control_paragraph = 551,   /* file_control_paragraph  */
  YYSYMBOL_file_control_sequence = 552,    /* file_control_sequence  */
  YYSYMBOL_file_control_entry = 553,       /* file_control_entry  */
  YYSYMBOL_554_15 = 554,                   /* $@15  */
  YYSYMBOL_select_clause_sequence = 555,   /* select_clause_sequence  */
  YYSYMBOL_select_clause = 556,            /* select_clause  */
  YYSYMBOL_assign_clause = 557,            /* assign_clause  */
  YYSYMBOL__device = 558,                  /* _device  */
  YYSYMBOL__ext_clause = 559,              /* _ext_clause  */
  YYSYMBOL_assignment_name = 560,          /* assignment_name  */
  YYSYMBOL_assignment_device_name_list = 561, /* assignment_device_name_list  */
  YYSYMBOL_access_mode_clause = 562,       /* access_mode_clause  */
  YYSYMBOL_access_mode = 563,              /* access_mode  */
  YYSYMBOL_alternative_record_key_clause = 564, /* alternative_record_key_clause  */
  YYSYMBOL_split_key_list = 565,           /* split_key_list  */
  YYSYMBOL_566_16 = 566,                   /* $@16  */
  YYSYMBOL_split_key = 567,                /* split_key  */
  YYSYMBOL_key_is_eq = 568,                /* key_is_eq  */
  YYSYMBOL_collating_sequence_clause = 569, /* collating_sequence_clause  */
  YYSYMBOL_file_status_clause = 570,       /* file_status_clause  */
  YYSYMBOL_file_or_sort = 571,             /* file_or_sort  */
  YYSYMBOL_lock_mode_clause = 572,         /* lock_mode_clause  */
  YYSYMBOL_lock_mode = 573,                /* lock_mode  */
  YYSYMBOL_lock_with = 574,                /* lock_with  */
  YYSYMBOL_lock_records = 575,             /* lock_records  */
  YYSYMBOL_organization_clause = 576,      /* organization_clause  */
  YYSYMBOL_organization = 577,             /* organization  */
  YYSYMBOL_padding_character_clause = 578, /* padding_character_clause  */
  YYSYMBOL_record_delimiter_clause = 579,  /* record_delimiter_clause  */
  YYSYMBOL_record_key_clause = 580,        /* record_key_clause  */
  YYSYMBOL_relative_key_clause = 581,      /* relative_key_clause  */
  YYSYMBOL_reserve_clause = 582,           /* reserve_clause  */
  YYSYMBOL_sharing_clause = 583,           /* sharing_clause  */
  YYSYMBOL_sharing_option = 584,           /* sharing_option  */
  YYSYMBOL_nominal_key_clause = 585,       /* nominal_key_clause  */
  YYSYMBOL_i_o_control_paragraph = 586,    /* i_o_control_paragraph  */
  YYSYMBOL_opt_i_o_control = 587,          /* opt_i_o_control  */
  YYSYMBOL_i_o_control_list = 588,         /* i_o_control_list  */
  YYSYMBOL_i_o_control_clause = 589,       /* i_o_control_clause  */
  YYSYMBOL_same_clause = 590,              /* same_clause  */
  YYSYMBOL_same_option = 591,              /* same_option  */
  YYSYMBOL_multiple_file_tape_clause = 592, /* multiple_file_tape_clause  */
  YYSYMBOL_multiple_file_list = 593,       /* multiple_file_list  */
  YYSYMBOL_multiple_file = 594,            /* multiple_file  */
  YYSYMBOL_multiple_file_position = 595,   /* multiple_file_position  */
  YYSYMBOL_apply_clause_list = 596,        /* apply_clause_list  */
  YYSYMBOL_apply_clause = 597,             /* apply_clause  */
  YYSYMBOL_data_division = 598,            /* data_division  */
  YYSYMBOL_file_section = 599,             /* file_section  */
  YYSYMBOL_600_17 = 600,                   /* $@17  */
  YYSYMBOL_601_18 = 601,                   /* $@18  */
  YYSYMBOL_file_description_sequence = 602, /* file_description_sequence  */
  YYSYMBOL_file_description = 603,         /* file_description  */
  YYSYMBOL_file_description_sequence_without_type = 604, /* file_description_sequence_without_type  */
  YYSYMBOL_file_type = 605,                /* file_type  */
  YYSYMBOL_file_description_entry = 606,   /* file_description_entry  */
  YYSYMBOL_607_19 = 607,                   /* @19  */
  YYSYMBOL_file_description_clause_sequence = 608, /* file_description_clause_sequence  */
  YYSYMBOL_file_description_clause = 609,  /* file_description_clause  */
  YYSYMBOL_block_contains_clause = 610,    /* block_contains_clause  */
  YYSYMBOL__records_or_characters = 611,   /* _records_or_characters  */
  YYSYMBOL_record_clause = 612,            /* record_clause  */
  YYSYMBOL_record_depending = 613,         /* record_depending  */
  YYSYMBOL_opt_from_integer = 614,         /* opt_from_integer  */
  YYSYMBOL_opt_to_integer = 615,           /* opt_to_integer  */
  YYSYMBOL_label_records_clause = 616,     /* label_records_clause  */
  YYSYMBOL_label_option = 617,             /* label_option  */
  YYSYMBOL_value_of_clause = 618,          /* value_of_clause  */
  YYSYMBOL_valueof_name = 619,             /* valueof_name  */
  YYSYMBOL_data_records_clause = 620,      /* data_records_clause  */
  YYSYMBOL_linage_clause = 621,            /* linage_clause  */
  YYSYMBOL_linage_sequence = 622,          /* linage_sequence  */
  YYSYMBOL_linage_lines = 623,             /* linage_lines  */
  YYSYMBOL_linage_footing = 624,           /* linage_footing  */
  YYSYMBOL_linage_top = 625,               /* linage_top  */
  YYSYMBOL_linage_bottom = 626,            /* linage_bottom  */
  YYSYMBOL_recording_mode_clause = 627,    /* recording_mode_clause  */
  YYSYMBOL_code_set_clause = 628,          /* code_set_clause  */
  YYSYMBOL_report_clause = 629,            /* report_clause  */
  YYSYMBOL_working_storage_section = 630,  /* working_storage_section  */
  YYSYMBOL_631_20 = 631,                   /* $@20  */
  YYSYMBOL_record_description_list = 632,  /* record_description_list  */
  YYSYMBOL_record_description_list_1 = 633, /* record_description_list_1  */
  YYSYMBOL_634_21 = 634,                   /* $@21  */
  YYSYMBOL_record_description_list_2 = 635, /* record_description_list_2  */
  YYSYMBOL_exec_sql_data_statement = 636,  /* exec_sql_data_statement  */
  YYSYMBOL_exec_java_import_data_statement = 637, /* exec_java_import_data_statement  */
  YYSYMBOL_exec_java_member_data_statement = 638, /* exec_java_member_data_statement  */
  YYSYMBOL_data_description = 639,         /* data_description  */
  YYSYMBOL_640_22 = 640,                   /* $@22  */
  YYSYMBOL_641_23 = 641,                   /* $@23  */
  YYSYMBOL_level_number = 642,             /* level_number  */
  YYSYMBOL_level_number_88 = 643,          /* level_number_88  */
  YYSYMBOL__maybe_next_level_number = 644, /* _maybe_next_level_number  */
  YYSYMBOL_entry_name = 645,               /* entry_name  */
  YYSYMBOL_const_name = 646,               /* const_name  */
  YYSYMBOL_const_global = 647,             /* const_global  */
  YYSYMBOL_lit_or_length = 648,            /* lit_or_length  */
  YYSYMBOL_constant_entry = 649,           /* constant_entry  */
  YYSYMBOL_data_description_clause_sequence = 650, /* data_description_clause_sequence  */
  YYSYMBOL_data_description_clause = 651,  /* data_description_clause  */
  YYSYMBOL_redefines_clause = 652,         /* redefines_clause  */
  YYSYMBOL_external_clause = 653,          /* external_clause  */
  YYSYMBOL_as_extname = 654,               /* as_extname  */
  YYSYMBOL_global_clause = 655,            /* global_clause  */
  YYSYMBOL_picture_clause = 656,           /* picture_clause  */
  YYSYMBOL_usage_clause = 657,             /* usage_clause  */
  YYSYMBOL_usage = 658,                    /* usage  */
  YYSYMBOL_sign_clause = 659,              /* sign_clause  */
  YYSYMBOL_occurs_key_spec = 660,          /* occurs_key_spec  */
  YYSYMBOL_occurs_clause = 661,            /* occurs_clause  */
  YYSYMBOL_occurs_to_integer = 662,        /* occurs_to_integer  */
  YYSYMBOL_occurs_depending = 663,         /* occurs_depending  */
  YYSYMBOL__occurs_keys = 664,             /* _occurs_keys  */
  YYSYMBOL_occurs_keys = 665,              /* occurs_keys  */
  YYSYMBOL_occurs_key = 666,               /* occurs_key  */
  YYSYMBOL_occurs_key_list = 667,          /* occurs_key_list  */
  YYSYMBOL_ascending_or_descending = 668,  /* ascending_or_descending  */
  YYSYMBOL__occurs_indexed = 669,          /* _occurs_indexed  */
  YYSYMBOL_occurs_indexed = 670,           /* occurs_indexed  */
  YYSYMBOL_occurs_index_list = 671,        /* occurs_index_list  */
  YYSYMBOL_occurs_index = 672,             /* occurs_index  */
  YYSYMBOL_justified_clause = 673,         /* justified_clause  */
  YYSYMBOL_synchronized_clause = 674,      /* synchronized_clause  */
  YYSYMBOL_left_or_right = 675,            /* left_or_right  */
  YYSYMBOL_blank_clause = 676,             /* blank_clause  */
  YYSYMBOL_based_clause = 677,             /* based_clause  */
  YYSYMBOL_value_clause = 678,             /* value_clause  */
  YYSYMBOL_value_cond_clause = 679,        /* value_cond_clause  */
  YYSYMBOL_680_24 = 680,                   /* $@24  */
  YYSYMBOL_value_item_list = 681,          /* value_item_list  */
  YYSYMBOL_value_item = 682,               /* value_item  */
  YYSYMBOL_false_is = 683,                 /* false_is  */
  YYSYMBOL_renames_clause = 684,           /* renames_clause  */
  YYSYMBOL_any_length_clause = 685,        /* any_length_clause  */
  YYSYMBOL_varying_clause = 686,           /* varying_clause  */
  YYSYMBOL_local_storage_section = 687,    /* local_storage_section  */
  YYSYMBOL_688_25 = 688,                   /* $@25  */
  YYSYMBOL_linkage_section = 689,          /* linkage_section  */
  YYSYMBOL_690_26 = 690,                   /* $@26  */
  YYSYMBOL_report_section = 691,           /* report_section  */
  YYSYMBOL_692_27 = 692,                   /* $@27  */
  YYSYMBOL_opt_report_description_list = 693, /* opt_report_description_list  */
  YYSYMBOL_report_description_list = 694,  /* report_description_list  */
  YYSYMBOL_report_description_entry = 695, /* report_description_entry  */
  YYSYMBOL_report_description_options = 696, /* report_description_options  */
  YYSYMBOL_report_description_option = 697, /* report_description_option  */
  YYSYMBOL_control_clause = 698,           /* control_clause  */
  YYSYMBOL_control_field_list = 699,       /* control_field_list  */
  YYSYMBOL__final = 700,                   /* _final  */
  YYSYMBOL_identifier_list = 701,          /* identifier_list  */
  YYSYMBOL_page_limit_clause = 702,        /* page_limit_clause  */
  YYSYMBOL_heading_clause = 703,           /* heading_clause  */
  YYSYMBOL_first_detail = 704,             /* first_detail  */
  YYSYMBOL_last_heading = 705,             /* last_heading  */
  YYSYMBOL_last_detail = 706,              /* last_detail  */
  YYSYMBOL_footing_clause = 707,           /* footing_clause  */
  YYSYMBOL_page_line_column = 708,         /* page_line_column  */
  YYSYMBOL_line_or_lines = 709,            /* line_or_lines  */
  YYSYMBOL_report_group_description_list = 710, /* report_group_description_list  */
  YYSYMBOL_report_group_description_entry = 711, /* report_group_description_entry  */
  YYSYMBOL_report_group_options = 712,     /* report_group_options  */
  YYSYMBOL_report_group_option = 713,      /* report_group_option  */
  YYSYMBOL_type_clause = 714,              /* type_clause  */
  YYSYMBOL_type_option = 715,              /* type_option  */
  YYSYMBOL_next_group_clause = 716,        /* next_group_clause  */
  YYSYMBOL_column_clause = 717,            /* column_clause  */
  YYSYMBOL_sum_clause_list = 718,          /* sum_clause_list  */
  YYSYMBOL_sum_clause = 719,               /* sum_clause  */
  YYSYMBOL_ref_id_exp = 720,               /* ref_id_exp  */
  YYSYMBOL_present_when_condition = 721,   /* present_when_condition  */
  YYSYMBOL_line_clause = 722,              /* line_clause  */
  YYSYMBOL_line_keyword_clause = 723,      /* line_keyword_clause  */
  YYSYMBOL_report_line_integer_list = 724, /* report_line_integer_list  */
  YYSYMBOL_line_or_plus = 725,             /* line_or_plus  */
  YYSYMBOL__numbers = 726,                 /* _numbers  */
  YYSYMBOL_source_clause = 727,            /* source_clause  */
  YYSYMBOL_group_indicate_clause = 728,    /* group_indicate_clause  */
  YYSYMBOL__indicate = 729,                /* _indicate  */
  YYSYMBOL_report_name = 730,              /* report_name  */
  YYSYMBOL_screen_section = 731,           /* screen_section  */
  YYSYMBOL_732_28 = 732,                   /* $@28  */
  YYSYMBOL_procedure_division = 733,       /* procedure_division  */
  YYSYMBOL_734_29 = 734,                   /* $@29  */
  YYSYMBOL_735_30 = 735,                   /* $@30  */
  YYSYMBOL_procedure_using_chaining = 736, /* procedure_using_chaining  */
  YYSYMBOL_737_31 = 737,                   /* $@31  */
  YYSYMBOL_738_32 = 738,                   /* $@32  */
  YYSYMBOL_procedure_param_list = 739,     /* procedure_param_list  */
  YYSYMBOL_procedure_param = 740,          /* procedure_param  */
  YYSYMBOL_procedure_type = 741,           /* procedure_type  */
  YYSYMBOL_size_optional = 742,            /* size_optional  */
  YYSYMBOL_procedure_optional = 743,       /* procedure_optional  */
  YYSYMBOL_procedure_returning = 744,      /* procedure_returning  */
  YYSYMBOL_procedure_declaratives = 745,   /* procedure_declaratives  */
  YYSYMBOL_746_33 = 746,                   /* $@33  */
  YYSYMBOL_procedure_list = 747,           /* procedure_list  */
  YYSYMBOL_procedure = 748,                /* procedure  */
  YYSYMBOL_section_header = 749,           /* section_header  */
  YYSYMBOL_paragraph_header = 750,         /* paragraph_header  */
  YYSYMBOL_invalid_statement = 751,        /* invalid_statement  */
  YYSYMBOL_section_name = 752,             /* section_name  */
  YYSYMBOL_opt_segment = 753,              /* opt_segment  */
  YYSYMBOL_statement_list = 754,           /* statement_list  */
  YYSYMBOL_755_34 = 755,                   /* @34  */
  YYSYMBOL_756_35 = 756,                   /* @35  */
  YYSYMBOL_statements = 757,               /* statements  */
  YYSYMBOL_statement = 758,                /* statement  */
  YYSYMBOL_accept_statement = 759,         /* accept_statement  */
  YYSYMBOL_760_36 = 760,                   /* $@36  */
  YYSYMBOL_accept_body = 761,              /* accept_body  */
  YYSYMBOL_opt_at_line_column = 762,       /* opt_at_line_column  */
  YYSYMBOL_line_number = 763,              /* line_number  */
  YYSYMBOL_column_number = 764,            /* column_number  */
  YYSYMBOL_opt_accp_attr = 765,            /* opt_accp_attr  */
  YYSYMBOL_accp_attrs = 766,               /* accp_attrs  */
  YYSYMBOL_accp_attr = 767,                /* accp_attr  */
  YYSYMBOL_end_accept = 768,               /* end_accept  */
  YYSYMBOL_add_statement = 769,            /* add_statement  */
  YYSYMBOL_770_37 = 770,                   /* $@37  */
  YYSYMBOL_add_body = 771,                 /* add_body  */
  YYSYMBOL_add_to = 772,                   /* add_to  */
  YYSYMBOL_end_add = 773,                  /* end_add  */
  YYSYMBOL_allocate_statement = 774,       /* allocate_statement  */
  YYSYMBOL_775_38 = 775,                   /* $@38  */
  YYSYMBOL_allocate_body = 776,            /* allocate_body  */
  YYSYMBOL_allocate_returning = 777,       /* allocate_returning  */
  YYSYMBOL_alter_statement = 778,          /* alter_statement  */
  YYSYMBOL_alter_options = 779,            /* alter_options  */
  YYSYMBOL__proceed_to = 780,              /* _proceed_to  */
  YYSYMBOL_call_statement = 781,           /* call_statement  */
  YYSYMBOL_782_39 = 782,                   /* $@39  */
  YYSYMBOL_call_using = 783,               /* call_using  */
  YYSYMBOL_784_40 = 784,                   /* $@40  */
  YYSYMBOL_call_param_list = 785,          /* call_param_list  */
  YYSYMBOL_call_param = 786,               /* call_param  */
  YYSYMBOL_call_type = 787,                /* call_type  */
  YYSYMBOL_call_returning = 788,           /* call_returning  */
  YYSYMBOL_call_on_exception = 789,        /* call_on_exception  */
  YYSYMBOL_790_41 = 790,                   /* $@41  */
  YYSYMBOL_call_not_on_exception = 791,    /* call_not_on_exception  */
  YYSYMBOL_792_42 = 792,                   /* $@42  */
  YYSYMBOL_end_call = 793,                 /* end_call  */
  YYSYMBOL_cancel_statement = 794,         /* cancel_statement  */
  YYSYMBOL_795_43 = 795,                   /* $@43  */
  YYSYMBOL_cancel_list = 796,              /* cancel_list  */
  YYSYMBOL_close_statement = 797,          /* close_statement  */
  YYSYMBOL_798_44 = 798,                   /* $@44  */
  YYSYMBOL_close_list = 799,               /* close_list  */
  YYSYMBOL_close_option = 800,             /* close_option  */
  YYSYMBOL_reel_or_unit = 801,             /* reel_or_unit  */
  YYSYMBOL_compute_statement = 802,        /* compute_statement  */
  YYSYMBOL_803_45 = 803,                   /* $@45  */
  YYSYMBOL_compute_body = 804,             /* compute_body  */
  YYSYMBOL_end_compute = 805,              /* end_compute  */
  YYSYMBOL_comp_equal = 806,               /* comp_equal  */
  YYSYMBOL_commit_statement = 807,         /* commit_statement  */
  YYSYMBOL_continue_statement = 808,       /* continue_statement  */
  YYSYMBOL_delete_statement = 809,         /* delete_statement  */
  YYSYMBOL_810_46 = 810,                   /* $@46  */
  YYSYMBOL_end_delete = 811,               /* end_delete  */
  YYSYMBOL_delete_file_statement = 812,    /* delete_file_statement  */
  YYSYMBOL_813_47 = 813,                   /* $@47  */
  YYSYMBOL_display_statement = 814,        /* display_statement  */
  YYSYMBOL_815_48 = 815,                   /* $@48  */
  YYSYMBOL_display_body = 816,             /* display_body  */
  YYSYMBOL_with_clause = 817,              /* with_clause  */
  YYSYMBOL_disp_attrs = 818,               /* disp_attrs  */
  YYSYMBOL_disp_attr = 819,                /* disp_attr  */
  YYSYMBOL_end_display = 820,              /* end_display  */
  YYSYMBOL_divide_statement = 821,         /* divide_statement  */
  YYSYMBOL_822_49 = 822,                   /* $@49  */
  YYSYMBOL_divide_body = 823,              /* divide_body  */
  YYSYMBOL_end_divide = 824,               /* end_divide  */
  YYSYMBOL_entry_statement = 825,          /* entry_statement  */
  YYSYMBOL_826_50 = 826,                   /* $@50  */
  YYSYMBOL_evaluate_statement = 827,       /* evaluate_statement  */
  YYSYMBOL_828_51 = 828,                   /* $@51  */
  YYSYMBOL_evaluate_subject_list = 829,    /* evaluate_subject_list  */
  YYSYMBOL_evaluate_subject = 830,         /* evaluate_subject  */
  YYSYMBOL_evaluate_condition_list = 831,  /* evaluate_condition_list  */
  YYSYMBOL_evaluate_case_list = 832,       /* evaluate_case_list  */
  YYSYMBOL_evaluate_case = 833,            /* evaluate_case  */
  YYSYMBOL_834_52 = 834,                   /* $@52  */
  YYSYMBOL_evaluate_other = 835,           /* evaluate_other  */
  YYSYMBOL_836_53 = 836,                   /* $@53  */
  YYSYMBOL_evaluate_when_list = 837,       /* evaluate_when_list  */
  YYSYMBOL_evaluate_object_list = 838,     /* evaluate_object_list  */
  YYSYMBOL_evaluate_object = 839,          /* evaluate_object  */
  YYSYMBOL_opt_evaluate_thru_expr = 840,   /* opt_evaluate_thru_expr  */
  YYSYMBOL_end_evaluate = 841,             /* end_evaluate  */
  YYSYMBOL_exit_statement = 842,           /* exit_statement  */
  YYSYMBOL_843_54 = 843,                   /* $@54  */
  YYSYMBOL_exit_body = 844,                /* exit_body  */
  YYSYMBOL_free_statement = 845,           /* free_statement  */
  YYSYMBOL_846_55 = 846,                   /* $@55  */
  YYSYMBOL_generate_statement = 847,       /* generate_statement  */
  YYSYMBOL_848_56 = 848,                   /* $@56  */
  YYSYMBOL_goto_statement = 849,           /* goto_statement  */
  YYSYMBOL_850_57 = 850,                   /* $@57  */
  YYSYMBOL_goto_depending = 851,           /* goto_depending  */
  YYSYMBOL_goback_statement = 852,         /* goback_statement  */
  YYSYMBOL_853_58 = 853,                   /* $@58  */
  YYSYMBOL_if_statement = 854,             /* if_statement  */
  YYSYMBOL_855_59 = 855,                   /* $@59  */
  YYSYMBOL_856_60 = 856,                   /* $@60  */
  YYSYMBOL_if_else_sentence = 857,         /* if_else_sentence  */
  YYSYMBOL_858_61 = 858,                   /* $@61  */
  YYSYMBOL_end_if = 859,                   /* end_if  */
  YYSYMBOL_initialize_statement = 860,     /* initialize_statement  */
  YYSYMBOL_861_62 = 861,                   /* $@62  */
  YYSYMBOL_initialize_filler = 862,        /* initialize_filler  */
  YYSYMBOL_initialize_value = 863,         /* initialize_value  */
  YYSYMBOL_initialize_replacing = 864,     /* initialize_replacing  */
  YYSYMBOL_initialize_replacing_list = 865, /* initialize_replacing_list  */
  YYSYMBOL_initialize_replacing_item = 866, /* initialize_replacing_item  */
  YYSYMBOL_initialize_category = 867,      /* initialize_category  */
  YYSYMBOL_initialize_default = 868,       /* initialize_default  */
  YYSYMBOL_initiate_statement = 869,       /* initiate_statement  */
  YYSYMBOL_870_63 = 870,                   /* $@63  */
  YYSYMBOL_inspect_statement = 871,        /* inspect_statement  */
  YYSYMBOL_872_64 = 872,                   /* $@64  */
  YYSYMBOL_send_identifier = 873,          /* send_identifier  */
  YYSYMBOL_inspect_list = 874,             /* inspect_list  */
  YYSYMBOL_inspect_item = 875,             /* inspect_item  */
  YYSYMBOL_inspect_tallying = 876,         /* inspect_tallying  */
  YYSYMBOL_877_65 = 877,                   /* $@65  */
  YYSYMBOL_tallying_list = 878,            /* tallying_list  */
  YYSYMBOL_tallying_item = 879,            /* tallying_item  */
  YYSYMBOL_inspect_replacing = 880,        /* inspect_replacing  */
  YYSYMBOL_replacing_list = 881,           /* replacing_list  */
  YYSYMBOL_replacing_item = 882,           /* replacing_item  */
  YYSYMBOL_rep_keyword = 883,              /* rep_keyword  */
  YYSYMBOL_replacing_region = 884,         /* replacing_region  */
  YYSYMBOL_inspect_converting = 885,       /* inspect_converting  */
  YYSYMBOL_inspect_region = 886,           /* inspect_region  */
  YYSYMBOL__initial = 887,                 /* _initial  */
  YYSYMBOL_merge_statement = 888,          /* merge_statement  */
  YYSYMBOL_889_66 = 889,                   /* $@66  */
  YYSYMBOL_move_statement = 890,           /* move_statement  */
  YYSYMBOL_891_67 = 891,                   /* $@67  */
  YYSYMBOL_move_body = 892,                /* move_body  */
  YYSYMBOL_multiply_statement = 893,       /* multiply_statement  */
  YYSYMBOL_894_68 = 894,                   /* $@68  */
  YYSYMBOL_multiply_body = 895,            /* multiply_body  */
  YYSYMBOL_end_multiply = 896,             /* end_multiply  */
  YYSYMBOL_open_statement = 897,           /* open_statement  */
  YYSYMBOL_898_69 = 898,                   /* $@69  */
  YYSYMBOL_open_list = 899,                /* open_list  */
  YYSYMBOL_open_mode = 900,                /* open_mode  */
  YYSYMBOL_open_sharing = 901,             /* open_sharing  */
  YYSYMBOL_open_option = 902,              /* open_option  */
  YYSYMBOL_perform_statement = 903,        /* perform_statement  */
  YYSYMBOL_904_70 = 904,                   /* $@70  */
  YYSYMBOL_perform_body = 905,             /* perform_body  */
  YYSYMBOL_906_71 = 906,                   /* $@71  */
  YYSYMBOL_end_perform = 907,              /* end_perform  */
  YYSYMBOL_perform_procedure = 908,        /* perform_procedure  */
  YYSYMBOL_perform_option = 909,           /* perform_option  */
  YYSYMBOL_perform_test = 910,             /* perform_test  */
  YYSYMBOL_perform_varying_list = 911,     /* perform_varying_list  */
  YYSYMBOL_perform_varying = 912,          /* perform_varying  */
  YYSYMBOL_read_statement = 913,           /* read_statement  */
  YYSYMBOL_914_72 = 914,                   /* $@72  */
  YYSYMBOL_read_into = 915,                /* read_into  */
  YYSYMBOL_with_lock = 916,                /* with_lock  */
  YYSYMBOL_read_key = 917,                 /* read_key  */
  YYSYMBOL_read_handler = 918,             /* read_handler  */
  YYSYMBOL_end_read = 919,                 /* end_read  */
  YYSYMBOL_release_statement = 920,        /* release_statement  */
  YYSYMBOL_921_73 = 921,                   /* $@73  */
  YYSYMBOL_return_statement = 922,         /* return_statement  */
  YYSYMBOL_923_74 = 923,                   /* $@74  */
  YYSYMBOL_end_return = 924,               /* end_return  */
  YYSYMBOL_rewrite_statement = 925,        /* rewrite_statement  */
  YYSYMBOL_926_75 = 926,                   /* $@75  */
  YYSYMBOL_write_lock = 927,               /* write_lock  */
  YYSYMBOL_end_rewrite = 928,              /* end_rewrite  */
  YYSYMBOL_rollback_statement = 929,       /* rollback_statement  */
  YYSYMBOL_search_statement = 930,         /* search_statement  */
  YYSYMBOL_931_76 = 931,                   /* $@76  */
  YYSYMBOL_search_body = 932,              /* search_body  */
  YYSYMBOL_933_77 = 933,                   /* $@77  */
  YYSYMBOL_search_varying = 934,           /* search_varying  */
  YYSYMBOL_search_at_end = 935,            /* search_at_end  */
  YYSYMBOL_936_78 = 936,                   /* $@78  */
  YYSYMBOL_search_whens = 937,             /* search_whens  */
  YYSYMBOL_search_when = 938,              /* search_when  */
  YYSYMBOL_939_79 = 939,                   /* $@79  */
  YYSYMBOL_end_search = 940,               /* end_search  */
  YYSYMBOL_set_statement = 941,            /* set_statement  */
  YYSYMBOL_942_80 = 942,                   /* $@80  */
  YYSYMBOL_set_body = 943,                 /* set_body  */
  YYSYMBOL_set_environment = 944,          /* set_environment  */
  YYSYMBOL_set_to = 945,                   /* set_to  */
  YYSYMBOL_set_up_down = 946,              /* set_up_down  */
  YYSYMBOL_up_or_down = 947,               /* up_or_down  */
  YYSYMBOL_set_to_on_off_sequence = 948,   /* set_to_on_off_sequence  */
  YYSYMBOL_set_to_on_off = 949,            /* set_to_on_off  */
  YYSYMBOL_set_to_true_false_sequence = 950, /* set_to_true_false_sequence  */
  YYSYMBOL_set_to_true_false = 951,        /* set_to_true_false  */
  YYSYMBOL_sort_statement = 952,           /* sort_statement  */
  YYSYMBOL_953_81 = 953,                   /* $@81  */
  YYSYMBOL_sort_body = 954,                /* sort_body  */
  YYSYMBOL_955_82 = 955,                   /* $@82  */
  YYSYMBOL_sort_key_list = 956,            /* sort_key_list  */
  YYSYMBOL_opt_key_list = 957,             /* opt_key_list  */
  YYSYMBOL_sort_duplicates = 958,          /* sort_duplicates  */
  YYSYMBOL_sort_collating = 959,           /* sort_collating  */
  YYSYMBOL_sort_input = 960,               /* sort_input  */
  YYSYMBOL_sort_output = 961,              /* sort_output  */
  YYSYMBOL_start_statement = 962,          /* start_statement  */
  YYSYMBOL_963_83 = 963,                   /* $@83  */
  YYSYMBOL_964_84 = 964,                   /* @84  */
  YYSYMBOL_start_key = 965,                /* start_key  */
  YYSYMBOL_start_op = 966,                 /* start_op  */
  YYSYMBOL_end_start = 967,                /* end_start  */
  YYSYMBOL_stop_statement = 968,           /* stop_statement  */
  YYSYMBOL_969_85 = 969,                   /* $@85  */
  YYSYMBOL_970_86 = 970,                   /* $@86  */
  YYSYMBOL_stop_returning = 971,           /* stop_returning  */
  YYSYMBOL_string_statement = 972,         /* string_statement  */
  YYSYMBOL_973_87 = 973,                   /* $@87  */
  YYSYMBOL_string_item_list = 974,         /* string_item_list  */
  YYSYMBOL_string_item = 975,              /* string_item  */
  YYSYMBOL_opt_with_pointer = 976,         /* opt_with_pointer  */
  YYSYMBOL_end_string = 977,               /* end_string  */
  YYSYMBOL_subtract_statement = 978,       /* subtract_statement  */
  YYSYMBOL_979_88 = 979,                   /* $@88  */
  YYSYMBOL_subtract_body = 980,            /* subtract_body  */
  YYSYMBOL_end_subtract = 981,             /* end_subtract  */
  YYSYMBOL_suppress_statement = 982,       /* suppress_statement  */
  YYSYMBOL__printing = 983,                /* _printing  */
  YYSYMBOL_terminate_statement = 984,      /* terminate_statement  */
  YYSYMBOL_985_89 = 985,                   /* $@89  */
  YYSYMBOL_transform_statement = 986,      /* transform_statement  */
  YYSYMBOL_987_90 = 987,                   /* $@90  */
  YYSYMBOL_unlock_statement = 988,         /* unlock_statement  */
  YYSYMBOL_989_91 = 989,                   /* $@91  */
  YYSYMBOL_opt_record = 990,               /* opt_record  */
  YYSYMBOL_unstring_statement = 991,       /* unstring_statement  */
  YYSYMBOL_992_92 = 992,                   /* $@92  */
  YYSYMBOL_unstring_delimited = 993,       /* unstring_delimited  */
  YYSYMBOL_unstring_delimited_list = 994,  /* unstring_delimited_list  */
  YYSYMBOL_unstring_delimited_item = 995,  /* unstring_delimited_item  */
  YYSYMBOL_unstring_into = 996,            /* unstring_into  */
  YYSYMBOL_unstring_into_item = 997,       /* unstring_into_item  */
  YYSYMBOL_unstring_into_delimiter = 998,  /* unstring_into_delimiter  */
  YYSYMBOL_unstring_into_count = 999,      /* unstring_into_count  */
  YYSYMBOL_unstring_tallying = 1000,       /* unstring_tallying  */
  YYSYMBOL_end_unstring = 1001,            /* end_unstring  */
  YYSYMBOL_use_statement = 1002,           /* use_statement  */
  YYSYMBOL_use_exception = 1003,           /* use_exception  */
  YYSYMBOL_use_global = 1004,              /* use_global  */
  YYSYMBOL_use_exception_target = 1005,    /* use_exception_target  */
  YYSYMBOL__after = 1006,                  /* _after  */
  YYSYMBOL__standard = 1007,               /* _standard  */
  YYSYMBOL_exception_or_error = 1008,      /* exception_or_error  */
  YYSYMBOL_exception_or_overflow = 1009,   /* exception_or_overflow  */
  YYSYMBOL_not_exception_or_overflow = 1010, /* not_exception_or_overflow  */
  YYSYMBOL__procedure = 1011,              /* _procedure  */
  YYSYMBOL_use_debugging = 1012,           /* use_debugging  */
  YYSYMBOL_use_debugging_target = 1013,    /* use_debugging_target  */
  YYSYMBOL_use_reporting = 1014,           /* use_reporting  */
  YYSYMBOL_write_statement = 1015,         /* write_statement  */
  YYSYMBOL_1016_93 = 1016,                 /* $@93  */
  YYSYMBOL_write_from = 1017,              /* write_from  */
  YYSYMBOL_write_option = 1018,            /* write_option  */
  YYSYMBOL_before_or_after = 1019,         /* before_or_after  */
  YYSYMBOL_write_handler = 1020,           /* write_handler  */
  YYSYMBOL_end_write = 1021,               /* end_write  */
  YYSYMBOL_on_accp_exception = 1022,       /* on_accp_exception  */
  YYSYMBOL_on_disp_exception = 1023,       /* on_disp_exception  */
  YYSYMBOL_opt_on_exception = 1024,        /* opt_on_exception  */
  YYSYMBOL_1025_94 = 1025,                 /* $@94  */
  YYSYMBOL_opt_not_on_exception = 1026,    /* opt_not_on_exception  */
  YYSYMBOL_1027_95 = 1027,                 /* $@95  */
  YYSYMBOL_on_size_error = 1028,           /* on_size_error  */
  YYSYMBOL_opt_on_size_error = 1029,       /* opt_on_size_error  */
  YYSYMBOL_1030_96 = 1030,                 /* $@96  */
  YYSYMBOL_opt_not_on_size_error = 1031,   /* opt_not_on_size_error  */
  YYSYMBOL_1032_97 = 1032,                 /* $@97  */
  YYSYMBOL_on_overflow = 1033,             /* on_overflow  */
  YYSYMBOL_opt_on_overflow = 1034,         /* opt_on_overflow  */
  YYSYMBOL_1035_98 = 1035,                 /* $@98  */
  YYSYMBOL_opt_not_on_overflow = 1036,     /* opt_not_on_overflow  */
  YYSYMBOL_1037_99 = 1037,                 /* $@99  */
  YYSYMBOL_at_end = 1038,                  /* at_end  */
  YYSYMBOL_at_end_sentence = 1039,         /* at_end_sentence  */
  YYSYMBOL_1040_100 = 1040,                /* $@100  */
  YYSYMBOL_not_at_end_sentence = 1041,     /* not_at_end_sentence  */
  YYSYMBOL_1042_101 = 1042,                /* $@101  */
  YYSYMBOL_at_eop = 1043,                  /* at_eop  */
  YYSYMBOL_at_eop_sentence = 1044,         /* at_eop_sentence  */
  YYSYMBOL_1045_102 = 1045,                /* $@102  */
  YYSYMBOL_not_at_eop_sentence = 1046,     /* not_at_eop_sentence  */
  YYSYMBOL_1047_103 = 1047,                /* $@103  */
  YYSYMBOL_opt_invalid_key = 1048,         /* opt_invalid_key  */
  YYSYMBOL_invalid_key = 1049,             /* invalid_key  */
  YYSYMBOL_invalid_key_sentence = 1050,    /* invalid_key_sentence  */
  YYSYMBOL_1051_104 = 1051,                /* $@104  */
  YYSYMBOL_not_invalid_key_sentence = 1052, /* not_invalid_key_sentence  */
  YYSYMBOL_1053_105 = 1053,                /* $@105  */
  YYSYMBOL__opt_scroll_lines = 1054,       /* _opt_scroll_lines  */
  YYSYMBOL_condition = 1055,               /* condition  */
  YYSYMBOL_expr = 1056,                    /* expr  */
  YYSYMBOL_partial_expr = 1057,            /* partial_expr  */
  YYSYMBOL_1058_106 = 1058,                /* $@106  */
  YYSYMBOL_expr_tokens = 1059,             /* expr_tokens  */
  YYSYMBOL_expr_token = 1060,              /* expr_token  */
  YYSYMBOL_eq = 1061,                      /* eq  */
  YYSYMBOL_gt = 1062,                      /* gt  */
  YYSYMBOL_lt = 1063,                      /* lt  */
  YYSYMBOL_ge = 1064,                      /* ge  */
  YYSYMBOL_le = 1065,                      /* le  */
  YYSYMBOL_exp_list = 1066,                /* exp_list  */
  YYSYMBOL_e_sep = 1067,                   /* e_sep  */
  YYSYMBOL_exp = 1068,                     /* exp  */
  YYSYMBOL_linage_counter = 1069,          /* linage_counter  */
  YYSYMBOL_arithmetic_x_list = 1070,       /* arithmetic_x_list  */
  YYSYMBOL_arithmetic_x = 1071,            /* arithmetic_x  */
  YYSYMBOL_record_name = 1072,             /* record_name  */
  YYSYMBOL_table_name = 1073,              /* table_name  */
  YYSYMBOL_file_name_list = 1074,          /* file_name_list  */
  YYSYMBOL_file_name = 1075,               /* file_name  */
  YYSYMBOL_mnemonic_name_list = 1076,      /* mnemonic_name_list  */
  YYSYMBOL_mnemonic_name = 1077,           /* mnemonic_name  */
  YYSYMBOL_procedure_name_list = 1078,     /* procedure_name_list  */
  YYSYMBOL_procedure_name = 1079,          /* procedure_name  */
  YYSYMBOL_label = 1080,                   /* label  */
  YYSYMBOL_integer_label = 1081,           /* integer_label  */
  YYSYMBOL_reference_list = 1082,          /* reference_list  */
  YYSYMBOL_reference = 1083,               /* reference  */
  YYSYMBOL_no_reference_list = 1084,       /* no_reference_list  */
  YYSYMBOL_opt_reference = 1085,           /* opt_reference  */
  YYSYMBOL_reference_or_literal = 1086,    /* reference_or_literal  */
  YYSYMBOL_undefined_word = 1087,          /* undefined_word  */
  YYSYMBOL_target_x_list = 1088,           /* target_x_list  */
  YYSYMBOL_target_x = 1089,                /* target_x  */
  YYSYMBOL_x_list = 1090,                  /* x_list  */
  YYSYMBOL_x = 1091,                       /* x  */
  YYSYMBOL_arith_x = 1092,                 /* arith_x  */
  YYSYMBOL_prog_or_entry = 1093,           /* prog_or_entry  */
  YYSYMBOL_alnum_or_id = 1094,             /* alnum_or_id  */
  YYSYMBOL_simple_value = 1095,            /* simple_value  */
  YYSYMBOL_simple_all_value = 1096,        /* simple_all_value  */
  YYSYMBOL_id_or_lit = 1097,               /* id_or_lit  */
  YYSYMBOL_id_or_lit_or_func = 1098,       /* id_or_lit_or_func  */
  YYSYMBOL_num_id_or_lit = 1099,           /* num_id_or_lit  */
  YYSYMBOL_identifier = 1100,              /* identifier  */
  YYSYMBOL_identifier_1 = 1101,            /* identifier_1  */
  YYSYMBOL_qualified_word = 1102,          /* qualified_word  */
  YYSYMBOL_subref = 1103,                  /* subref  */
  YYSYMBOL_refmod = 1104,                  /* refmod  */
  YYSYMBOL_integer = 1105,                 /* integer  */
  YYSYMBOL_literal = 1106,                 /* literal  */
  YYSYMBOL_basic_literal = 1107,           /* basic_literal  */
  YYSYMBOL_basic_value = 1108,             /* basic_value  */
  YYSYMBOL_function = 1109,                /* function  */
  YYSYMBOL_func_refmod = 1110,             /* func_refmod  */
  YYSYMBOL_func_args = 1111,               /* func_args  */
  YYSYMBOL_list_func_args = 1112,          /* list_func_args  */
  YYSYMBOL_trim_args = 1113,               /* trim_args  */
  YYSYMBOL_numvalc_args = 1114,            /* numvalc_args  */
  YYSYMBOL_locale_dt_args = 1115,          /* locale_dt_args  */
  YYSYMBOL_not_const_word = 1116,          /* not_const_word  */
  YYSYMBOL_flag_all = 1117,                /* flag_all  */
  YYSYMBOL_flag_duplicates = 1118,         /* flag_duplicates  */
  YYSYMBOL_flag_initialized = 1119,        /* flag_initialized  */
  YYSYMBOL_flag_next = 1120,               /* flag_next  */
  YYSYMBOL_flag_not = 1121,                /* flag_not  */
  YYSYMBOL_flag_optional = 1122,           /* flag_optional  */
  YYSYMBOL_flag_rounded = 1123,            /* flag_rounded  */
  YYSYMBOL_flag_separate = 1124,           /* flag_separate  */
  YYSYMBOL_in_of = 1125,                   /* in_of  */
  YYSYMBOL_records = 1126,                 /* records  */
  YYSYMBOL_with_dups = 1127,               /* with_dups  */
  YYSYMBOL_coll_sequence = 1128,           /* coll_sequence  */
  YYSYMBOL__advancing = 1129,              /* _advancing  */
  YYSYMBOL__also = 1130,                   /* _also  */
  YYSYMBOL__are = 1131,                    /* _are  */
  YYSYMBOL__area = 1132,                   /* _area  */
  YYSYMBOL__as = 1133,                     /* _as  */
  YYSYMBOL__at = 1134,                     /* _at  */
  YYSYMBOL__binary = 1135,                 /* _binary  */
  YYSYMBOL__by = 1136,                     /* _by  */
  YYSYMBOL__character = 1137,              /* _character  */
  YYSYMBOL__characters = 1138,             /* _characters  */
  YYSYMBOL__contains = 1139,               /* _contains  */
  YYSYMBOL__data = 1140,                   /* _data  */
  YYSYMBOL__file = 1141,                   /* _file  */
  YYSYMBOL__for = 1142,                    /* _for  */
  YYSYMBOL__from = 1143,                   /* _from  */
  YYSYMBOL__in = 1144,                     /* _in  */
  YYSYMBOL__is = 1145,                     /* _is  */
  YYSYMBOL__is_are = 1146,                 /* _is_are  */
  YYSYMBOL__key = 1147,                    /* _key  */
  YYSYMBOL__line_or_lines = 1148,          /* _line_or_lines  */
  YYSYMBOL__lines = 1149,                  /* _lines  */
  YYSYMBOL__literal = 1150,                /* _literal  */
  YYSYMBOL__mode = 1151,                   /* _mode  */
  YYSYMBOL__number = 1152,                 /* _number  */
  YYSYMBOL__of = 1153,                     /* _of  */
  YYSYMBOL__on = 1154,                     /* _on  */
  YYSYMBOL__in_order = 1155,               /* _in_order  */
  YYSYMBOL__other = 1156,                  /* _other  */
  YYSYMBOL__program = 1157,                /* _program  */
  YYSYMBOL__record = 1158,                 /* _record  */
  YYSYMBOL__right = 1159,                  /* _right  */
  YYSYMBOL__set = 1160,                    /* _set  */
  YYSYMBOL__sign = 1161,                   /* _sign  */
  YYSYMBOL__sign_is = 1162,                /* _sign_is  */
  YYSYMBOL__size = 1163,                   /* _size  */
  YYSYMBOL__status = 1164,                 /* _status  */
  YYSYMBOL__tape = 1165,                   /* _tape  */
  YYSYMBOL__than = 1166,                   /* _than  */
  YYSYMBOL__then = 1167,                   /* _then  */
  YYSYMBOL__times = 1168,                  /* _times  */
  YYSYMBOL__to = 1169,                     /* _to  */
  YYSYMBOL__when = 1170,                   /* _when  */
  YYSYMBOL__with = 1171,                   /* _with  */
  YYSYMBOL_exec_sql_statement = 1172,      /* exec_sql_statement  */
  YYSYMBOL_exec_java_statement = 1173,     /* exec_java_statement  */
  YYSYMBOL_exec_java_import_statement = 1174, /* exec_java_import_statement  */
  YYSYMBOL_exec_java_member_statement = 1175 /* exec_java_member_statement  */
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
#define YYLAST   5493

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  467
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  709
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1568
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  2305

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   708


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
       2,     2,     2,     2,     2,     2,     2,     2,   466,     2,
     462,   461,   455,   453,     2,   454,   459,   456,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   465,     2,
     464,   460,   463,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   458,     2,     2,     2,     2,     2,
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
     445,   446,   447,   448,   449,   450,   451,   452,   457
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   778,   778,   778,   822,   823,   827,   828,   833,   834,
     832,   847,   848,   846,   861,   862,   860,   867,   868,   869,
     872,   873,   901,   927,   959,   958,  1004,  1048,  1049,  1053,
    1054,  1057,  1058,  1062,  1069,  1076,  1080,  1084,  1096,  1097,
    1107,  1108,  1117,  1118,  1122,  1123,  1124,  1125,  1134,  1137,
    1138,  1139,  1140,  1144,  1151,  1160,  1163,  1164,  1165,  1166,
    1170,  1171,  1175,  1176,  1177,  1181,  1188,  1189,  1193,  1200,
    1212,  1215,  1216,  1220,  1221,  1225,  1229,  1236,  1237,  1247,
    1250,  1251,  1252,  1256,  1257,  1261,  1262,  1263,  1264,  1265,
    1266,  1267,  1268,  1269,  1270,  1271,  1278,  1289,  1288,  1300,
    1299,  1308,  1322,  1336,  1350,  1366,  1367,  1371,  1372,  1376,
    1387,  1388,  1396,  1395,  1407,  1408,  1409,  1410,  1411,  1419,
    1420,  1425,  1426,  1428,  1427,  1439,  1440,  1444,  1445,  1446,
    1447,  1448,  1449,  1453,  1454,  1455,  1456,  1457,  1458,  1465,
    1476,  1488,  1489,  1493,  1494,  1501,  1510,  1511,  1515,  1516,
    1530,  1545,  1612,  1623,  1630,  1637,  1643,  1650,  1651,  1655,
    1654,  1664,  1663,  1679,  1680,  1683,  1684,  1689,  1688,  1709,
    1710,  1714,  1715,  1716,  1717,  1718,  1719,  1720,  1721,  1722,
    1723,  1724,  1725,  1726,  1727,  1728,  1735,  1739,  1744,  1751,
    1752,  1753,  1756,  1757,  1761,  1768,  1769,  1776,  1796,  1797,
    1803,  1807,  1808,  1809,  1816,  1836,  1879,  1879,  1883,  1887,
    1903,  1904,  1905,  1911,  1921,  1930,  1932,  1933,  1939,  1943,
    1944,  1945,  1948,  1949,  1950,  1954,  1958,  1959,  1965,  1966,
    1970,  1979,  1988,  1997,  2006,  2021,  2031,  2038,  2047,  2085,
    2092,  2093,  2100,  2104,  2105,  2106,  2112,  2119,  2120,  2123,
    2124,  2125,  2126,  2130,  2131,  2135,  2136,  2142,  2167,  2168,
    2169,  2170,  2176,  2183,  2184,  2188,  2191,  2192,  2198,  2199,
    2200,  2204,  2208,  2212,  2216,  2220,  2230,  2231,  2245,  2246,
    2246,  2249,  2248,  2261,  2262,  2266,  2278,  2287,  2291,  2292,
    2302,  2301,  2319,  2320,  2324,  2331,  2338,  2339,  2340,  2341,
    2342,  2343,  2344,  2345,  2346,  2347,  2354,  2358,  2358,  2358,
    2364,  2376,  2401,  2425,  2426,  2433,  2434,  2438,  2439,  2446,
    2453,  2454,  2461,  2465,  2474,  2475,  2481,  2491,  2509,  2510,
    2514,  2515,  2516,  2520,  2527,  2534,  2544,  2551,  2569,  2573,
    2584,  2585,  2585,  2596,  2597,  2601,  2601,  2618,  2619,  2621,
    2622,  2623,  2624,  2625,  2626,  2627,  2631,  2638,  2645,  2649,
    2656,  2660,  2667,  2669,  2668,  2694,  2693,  2719,  2723,  2730,
    2732,  2754,  2759,  2765,  2774,  2782,  2783,  2791,  2792,  2793,
    2797,  2817,  2821,  2830,  2831,  2832,  2833,  2834,  2835,  2836,
    2837,  2838,  2839,  2840,  2841,  2842,  2843,  2844,  2845,  2852,
    2874,  2896,  2897,  2909,  2929,  2936,  2937,  2941,  2942,  2943,
    2948,  2953,  2954,  2955,  2956,  2957,  2958,  2959,  2960,  2965,
    2970,  2971,  2972,  2973,  2974,  2975,  2976,  2977,  2978,  2979,
    2980,  2981,  2982,  2983,  2984,  2985,  2986,  2987,  2988,  2996,
    3004,  3012,  3019,  3024,  3034,  3035,  3036,  3040,  3057,  3058,
    3061,  3062,  3068,  3068,  3071,  3095,  3111,  3112,  3116,  3117,
    3120,  3120,  3123,  3130,  3131,  3136,  3146,  3153,  3156,  3157,
    3158,  3165,  3172,  3197,  3201,  3201,  3206,  3207,  3211,  3212,
    3215,  3216,  3229,  3241,  3261,  3274,  3284,  3286,  3285,  3306,
    3307,  3307,  3320,  3322,  3321,  3333,  3334,  3338,  3339,  3348,
    3355,  3358,  3362,  3366,  3367,  3368,  3375,  3376,  3380,  3383,
    3383,  3386,  3387,  3393,  3398,  3399,  3402,  3403,  3406,  3407,
    3410,  3411,  3414,  3415,  3419,  3420,  3421,  3425,  3426,  3429,
    3430,  3434,  3438,  3439,  3443,  3444,  3445,  3446,  3447,  3448,
    3449,  3450,  3451,  3452,  3453,  3454,  3455,  3456,  3457,  3458,
    3462,  3466,  3467,  3468,  3469,  3470,  3471,  3472,  3476,  3480,
    3481,  3482,  3486,  3487,  3491,  3495,  3500,  3504,  3508,  3512,
    3513,  3517,  3518,  3522,  3523,  3524,  3527,  3527,  3527,  3530,
    3534,  3537,  3537,  3540,  3547,  3548,  3548,  3558,  3560,  3574,
    3559,  3601,  3603,  3602,  3609,  3608,  3617,  3618,  3623,  3630,
    3632,  3636,  3646,  3648,  3656,  3664,  3693,  3724,  3726,  3736,
    3741,  3752,  3753,  3753,  3780,  3781,  3785,  3786,  3787,  3788,
    3804,  3816,  3847,  3884,  3896,  3899,  3900,  3909,  3913,  3909,
    3926,  3947,  3951,  3952,  3953,  3954,  3955,  3956,  3957,  3958,
    3959,  3960,  3961,  3962,  3963,  3964,  3965,  3966,  3967,  3968,
    3969,  3970,  3971,  3972,  3973,  3974,  3975,  3976,  3977,  3978,
    3979,  3980,  3981,  3982,  3983,  3984,  3985,  3986,  3987,  3988,
    3989,  3990,  3991,  3992,  3993,  3994,  3995,  3996,  3997,  3998,
    3999,  4000,  4001,  4002,  4003,  4004,  4027,  4026,  4039,  4043,
    4047,  4051,  4055,  4059,  4063,  4067,  4071,  4075,  4079,  4083,
    4087,  4091,  4095,  4099,  4103,  4110,  4111,  4112,  4113,  4114,
    4115,  4119,  4123,  4124,  4127,  4128,  4132,  4133,  4137,  4138,
    4139,  4140,  4141,  4142,  4143,  4144,  4148,  4152,  4156,  4161,
    4162,  4163,  4164,  4165,  4166,  4170,  4171,  4180,  4180,  4186,
    4190,  4194,  4200,  4201,  4205,  4206,  4215,  4215,  4220,  4224,
    4231,  4232,  4241,  4247,  4248,  4252,  4252,  4260,  4260,  4270,
    4272,  4271,  4280,  4281,  4286,  4293,  4300,  4302,  4306,  4314,
    4325,  4326,  4327,  4332,  4336,  4335,  4347,  4351,  4350,  4361,
    4362,  4371,  4371,  4375,  4376,  4380,  4392,  4392,  4396,  4397,
    4408,  4409,  4410,  4411,  4412,  4415,  4415,  4423,  4423,  4429,
    4436,  4437,  4440,  4440,  4447,  4460,  4473,  4473,  4484,  4485,
    4494,  4494,  4514,  4513,  4526,  4530,  4534,  4538,  4542,  4546,
    4550,  4555,  4559,  4566,  4567,  4568,  4572,  4573,  4578,  4579,
    4580,  4581,  4582,  4583,  4584,  4585,  4586,  4587,  4591,  4595,
    4599,  4604,  4605,  4609,  4610,  4619,  4619,  4625,  4629,  4633,
    4637,  4641,  4648,  4649,  4658,  4658,  4680,  4679,  4698,  4699,
    4710,  4719,  4724,  4732,  4761,  4762,  4768,  4767,  4783,  4787,
    4786,  4801,  4802,  4807,  4808,  4819,  4848,  4849,  4850,  4853,
    4854,  4858,  4859,  4868,  4868,  4873,  4874,  4882,  4890,  4898,
    4916,  4941,  4941,  4954,  4954,  4967,  4967,  4976,  4980,  4993,
    4993,  5006,  5008,  5006,  5019,  5024,  5028,  5027,  5041,  5042,
    5051,  5051,  5059,  5060,  5064,  5065,  5066,  5070,  5071,  5076,
    5077,  5082,  5086,  5087,  5088,  5089,  5090,  5091,  5092,  5096,
    5097,  5106,  5106,  5119,  5118,  5128,  5129,  5130,  5134,  5135,
    5139,  5140,  5141,  5147,  5147,  5152,  5153,  5157,  5158,  5159,
    5160,  5161,  5162,  5168,  5172,  5173,  5177,  5182,  5186,  5187,
    5188,  5189,  5190,  5194,  5220,  5233,  5234,  5238,  5238,  5246,
    5246,  5256,  5256,  5261,  5265,  5277,  5277,  5283,  5287,  5294,
    5295,  5304,  5304,  5308,  5309,  5323,  5324,  5325,  5326,  5330,
    5331,  5335,  5336,  5337,  5349,  5349,  5354,  5359,  5358,  5368,
    5375,  5376,  5380,  5385,  5394,  5397,  5401,  5406,  5413,  5420,
    5421,  5425,  5426,  5431,  5443,  5443,  5470,  5471,  5475,  5476,
    5480,  5484,  5488,  5492,  5499,  5500,  5506,  5507,  5508,  5512,
    5513,  5522,  5522,  5537,  5537,  5548,  5549,  5558,  5558,  5575,
    5576,  5580,  5587,  5588,  5597,  5610,  5610,  5616,  5621,  5620,
    5631,  5632,  5636,  5638,  5637,  5648,  5649,  5654,  5653,  5664,
    5665,  5674,  5674,  5679,  5680,  5681,  5682,  5683,  5689,  5698,
    5702,  5711,  5718,  5719,  5725,  5726,  5730,  5739,  5740,  5744,
    5748,  5760,  5760,  5766,  5765,  5782,  5785,  5806,  5807,  5810,
    5811,  5815,  5816,  5821,  5826,  5834,  5846,  5851,  5859,  5875,
    5876,  5875,  5896,  5897,  5905,  5906,  5907,  5908,  5909,  5913,
    5914,  5923,  5923,  5928,  5928,  5935,  5936,  5937,  5946,  5946,
    5955,  5956,  5960,  5961,  5962,  5966,  5967,  5971,  5972,  5981,
    5981,  5987,  5991,  5995,  6002,  6003,  6012,  6019,  6020,  6028,
    6028,  6041,  6041,  6057,  6057,  6066,  6068,  6069,  6078,  6078,
    6088,  6089,  6094,  6095,  6100,  6107,  6108,  6113,  6120,  6121,
    6125,  6126,  6130,  6131,  6135,  6136,  6145,  6146,  6147,  6151,
    6175,  6178,  6186,  6196,  6201,  6206,  6211,  6218,  6219,  6222,
    6223,  6227,  6227,  6231,  6231,  6235,  6235,  6238,  6239,  6243,
    6250,  6251,  6255,  6267,  6267,  6284,  6285,  6290,  6293,  6297,
    6301,  6308,  6309,  6312,  6313,  6314,  6318,  6319,  6332,  6340,
    6347,  6349,  6348,  6358,  6360,  6359,  6374,  6378,  6380,  6379,
    6390,  6392,  6391,  6408,  6414,  6416,  6415,  6425,  6427,  6426,
    6442,  6447,  6452,  6462,  6461,  6473,  6472,  6488,  6493,  6498,
    6508,  6507,  6519,  6518,  6533,  6534,  6538,  6543,  6548,  6558,
    6557,  6569,  6568,  6585,  6588,  6600,  6607,  6614,  6614,  6624,
    6625,  6627,  6628,  6629,  6630,  6631,  6632,  6634,  6635,  6636,
    6637,  6638,  6639,  6641,  6642,  6644,  6645,  6646,  6649,  6651,
    6652,  6653,  6655,  6656,  6657,  6659,  6660,  6662,  6663,  6664,
    6665,  6666,  6668,  6669,  6670,  6671,  6672,  6673,  6675,  6676,
    6677,  6678,  6679,  6680,  6682,  6683,  6686,  6686,  6686,  6687,
    6687,  6688,  6688,  6689,  6689,  6689,  6690,  6690,  6690,  6695,
    6696,  6699,  6700,  6701,  6705,  6706,  6707,  6708,  6709,  6710,
    6711,  6712,  6713,  6724,  6736,  6751,  6752,  6757,  6763,  6785,
    6805,  6809,  6825,  6839,  6840,  6845,  6851,  6852,  6857,  6866,
    6867,  6868,  6872,  6883,  6884,  6888,  6898,  6899,  6903,  6904,
    6908,  6909,  6915,  6935,  6936,  6940,  6941,  6945,  6946,  6950,
    6951,  6952,  6953,  6954,  6955,  6956,  6957,  6958,  6962,  6963,
    6964,  6965,  6966,  6967,  6968,  6972,  6973,  6977,  6978,  6982,
    6983,  6987,  6988,  6999,  7000,  7004,  7005,  7006,  7010,  7011,
    7012,  7020,  7024,  7025,  7026,  7027,  7031,  7032,  7036,  7046,
    7060,  7083,  7095,  7096,  7106,  7107,  7111,  7112,  7113,  7114,
    7115,  7116,  7117,  7125,  7129,  7133,  7137,  7141,  7145,  7149,
    7153,  7157,  7161,  7165,  7169,  7176,  7177,  7178,  7182,  7183,
    7187,  7188,  7193,  7200,  7207,  7217,  7224,  7234,  7241,  7255,
    7265,  7266,  7270,  7271,  7275,  7276,  7280,  7281,  7282,  7286,
    7287,  7291,  7292,  7296,  7297,  7301,  7302,  7309,  7309,  7310,
    7310,  7311,  7311,  7312,  7312,  7314,  7314,  7315,  7315,  7316,
    7316,  7317,  7317,  7318,  7318,  7319,  7319,  7320,  7320,  7321,
    7321,  7322,  7322,  7323,  7323,  7324,  7324,  7325,  7325,  7326,
    7326,  7327,  7327,  7328,  7328,  7329,  7329,  7330,  7330,  7331,
    7331,  7331,  7332,  7332,  7333,  7333,  7333,  7334,  7334,  7335,
    7335,  7336,  7336,  7337,  7337,  7338,  7338,  7339,  7339,  7340,
    7340,  7340,  7341,  7341,  7342,  7342,  7343,  7343,  7344,  7344,
    7345,  7345,  7346,  7346,  7347,  7347,  7347,  7348,  7348,  7349,
    7349,  7350,  7350,  7351,  7351,  7352,  7352,  7353,  7353,  7354,
    7354,  7356,  7356,  7357,  7357,  7364,  7399,  7424,  7441
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
  "\"EXEC JAVA IMPORT statement\"", "\"EXEC JAVA CLASS-MEMBER statement\"",
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
  "exec_java_import_data_statement", "exec_java_member_data_statement",
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
  "exec_sql_statement", "exec_java_statement",
  "exec_java_import_statement", "exec_java_member_statement", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1762)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1564)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1762,   223,   447, -1762,  -198,  -188,    90, -1762, -1762, -1762,
     137,   137,   576,   576, -1762, -1762,   514, -1762, -1762, -1762,
   -1762,   627,   627,   355,   784,   784,   653,   479, -1762,   813,
     830, -1762, -1762, -1762, -1762,  -109,   616,   832,   554,   734,
     734, -1762,   588,    31,   623,   626,   739,   644, -1762,   -39,
     994,   809,   992, -1762,   277, -1762, -1762,   849, -1762, -1762,
   -1762,   689, -1762, -1762, -1762,   828,   758, -1762,    21, -1762,
      22,   137,   576, -1762, -1762, -1762, -1762,   677, -1762,  1042,
      84,   762,   881,  1006,   808, -1762, -1762,   912,   576, -1762,
   -1762, -1762,   805,   826,   855,   868,   870, -1762, -1762, -1762,
   -1762, -1762,   973,   879,  1137,  1229,   949,   663, -1762,   144,
   -1762, -1762, -1762,    50, -1762, -1762,   898,  1005,  1124, -1762,
     -20,   649, -1762,   142,   142,   920,   905,   906,   784, -1762,
     279,  1188,   113,  1671,  1097, -1762, -1762,   922, -1762,  1102,
    1104,   983,  1110,   986, -1762,   995, -1762, -1762, -1762,  1370,
   -1762, -1762, -1762, -1762, -1762, -1762,   936,  1040,  1066, -1762,
     808, -1762, -1762,   789, -1762, -1762,   116, -1762,   605,   -91,
     189, -1762, -1762, -1762, -1762,  1028,  1187, -1762,   333, -1762,
     346, -1762, -1762, -1762, -1762,    68,   174, -1762,   -88, -1762,
   -1762, -1762,   946,  -189,  1301,   970,  1187,  1187,   970,  1033,
    1058,  1187,  1187,  1187,  1187,  1187,   970,  1187,  1373,  1187,
   -1762,   910, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762,   970,   973,    84,   984, -1762,
     984,   984, -1762,  1202,   984, -1762,  1355, -1762,  1267,   -20,
     649, -1762,   987,  1091,  1094,   649,   990,   993,   996,   705,
   -1762, -1762, -1762,  1089,  1072, -1762,  1187,  1079,  1171, -1762,
   -1762,  1356,   734,  1187,  1230, -1762,   406, -1762, -1762,  1106,
   -1762,  1187,  1256, -1762,   670, -1762, -1762, -1762, -1762,  1015,
    1227, -1762, -1762,   970,   970,  1187,  1187, -1762,  1187,   984,
    1418,   970,   970,   984,  1187,   984, -1762,   970,     6, -1762,
   -1762, -1762, -1762,   422,   984, -1762, -1762,   984,  1208,  1081,
    1209, -1762,   808, -1762,   808, -1762, -1762,   649, -1762,  1034,
    1138, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
    1089, -1762, -1762, -1762,   -49,   -31, -1762, -1762,  1355,  1187,
     711,   711,  1187,     7,  1246,  1187,  1474,  1223, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,   586,
     540,  1187, -1762,  1059,  1045, -1762,   809,  1230, -1762, -1762,
   -1762, -1762,   984, -1762, -1762, -1762, -1762, -1762,  1187, -1762,
   -1762,   873,   984,  1269, -1762, -1762, -1762, -1762, -1762,   984,
   -1762, -1762,    40, -1762, -1762,   862, -1762, -1762, -1762, -1762,
     984, -1762,   984,  1228,   984,   808, -1762,  1211,   808, -1762,
   -1762,   649, -1762,  1050, -1762, -1762,  1426, -1762,  1429, -1762,
   -1762,  1230,  1075,  1187,  1474,   984,   -52,  -102,  1230,  1078,
   -1762,  1187,  1087, -1762,  1087,   -53, -1762, -1762, -1762, -1762,
   -1762,  1230, -1762, -1762, -1762,   449,    35, -1762,  1184, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762,   873, -1762,  1121, -1762,
   -1762, -1762, -1762, -1762, -1762,  1230, -1762, -1762,   862, -1762,
    1146, -1762,  1359, -1762,   984,   984,   984, -1762,  1230, -1762,
   -1762, -1762,  1216, -1762, -1762,    58,  1099,  1141, -1762, -1762,
   -1762,   984, -1762, -1762, -1762, -1762, -1762, -1762,  1306,    18,
    1345,  1111, -1762, -1762, -1762,  1187,  1187, -1762, -1762,  2455,
     576, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762,   651, -1762,    60, -1762,
     873,  1230, -1762, -1762, -1762,  1187,   862, -1762,  1246,  1233,
    1151, -1762,  1192,  1246,  1339,  1187,  1503,   162,   -55,   -30,
   -1762,  1119, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762,  1179, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762,  1187,   984, -1762,  1087, -1762,  1216, -1762, -1762,
    2178,  1540,  1380,   349, -1762,  1230,    53, -1762, -1762, -1762,
    1230, -1762, -1762,  1197, -1762,   -63,   -63,  2737, -1762,  1113,
   -1762, -1762, -1762, -1762,  1214,  3403,  1115, -1762, -1762,   651,
   -1762, -1762,   970, -1762,  1187,  1339, -1762,   749, -1762,  1187,
   -1762,  1187,   806, -1762,  1187, -1762,  1187,  1207,  1187,  1187,
   -1762,  1370,   267,  1187,  1134, -1762, -1762, -1762,  1364, -1762,
   -1762,   261,   427,   541,   585,   652,  1152, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762,  1234, -1762, -1762,  1230,
   -1762, -1762, -1762, -1762,   984,   984,  1387, -1762, -1762, -1762,
     -43, -1762, -1762, -1762,  1187,  1187,   984, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762,   690,  -105, -1762,   286, -1762,
      62, -1762, -1762, -1762, -1762,   103,  1373, -1762,   448, -1762,
   -1762, -1762, -1762,  1493, -1762,  1368, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762,  1200, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762,  1151, -1762,  1911, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762,   -57, -1762, -1762,  1300, -1762,
   -1762, -1762, -1762,   790, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,   656, -1762,
   -1762,   268,  1187, -1762, -1762,   318,   124,   984,  1570, -1762,
   -1762,  -102,  1232, -1762,   984,   984, -1762,  1321,  1321,  1330,
   -1762,   984, -1762,   127, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762,  1166, -1762, -1762,  1218,
   -1762,  1155,  1219, -1762, -1762, -1762, -1762,  4801,    62,  1445,
   -1762,  1599, -1762,  1265,  1265,   873,  1110,  1110, -1762, -1762,
    1162, -1762,    62, -1762,  1231, -1762, -1762, -1762, -1762, -1762,
      19,  1455, -1762, -1762,  1141,  1230,  1175, -1762,  1176,   984,
    3677,  1194,   111,  1921,  1621, -1762,  4587,   808,  1236,  4669,
    4587,  1402,   774,   756,    78,   984, -1762, -1762,  1506, -1762,
      78,   984,  3436,   984,  4024,  4587, -1762,  2245,   808,   984,
     808,   984,    73,    46,   984,   808, -1762, -1762,  4038,  4133,
   -1762, -1762,   984,   984,   808,   984, -1762,   250,  1532,   984,
   -1762, -1762, -1762, -1762, -1762, -1762,  1626, -1762, -1762, -1762,
   -1762, -1762,   984,    -4,    28,   557,  1205, -1762,  1205, -1762,
   -1762, -1762, -1762,   221, -1762, -1762, -1762, -1762, -1762,   984,
    1187,  1476,  1476,   349, -1762, -1762, -1762, -1762,  1461, -1762,
    1230,  1248,  5031,  1190, -1762,   984, -1762, -1762,  4587,  1417,
   -1762,  1503, -1762, -1762, -1762,   984,   984,   873,  1152, -1762,
      62,  -102,  -102,  1624,  1373, -1762, -1762, -1762,  1531,   630,
    1110,  1198,   984,  1204,  1210,  1110,   422,  1215,  1217,  1221,
    1222,  1224,  1225,  1226,  1237,  1204,  1534, -1762,  4147, -1762,
   -1762, -1762, -1762,  1454, -1762,  1608, -1762, -1762, -1762,  1264,
   -1762,   422, -1762, -1762,  1235, -1762, -1762, -1762,   256,   808,
    1541,  2040, -1762,  1325,  1357,   808,   921,  1548,  2637,  1096,
    1109,  1556,   128,  1235, -1762, -1762,    49, -1762, -1762, -1762,
    1581, -1762, -1762, -1762,  1110,    78, -1762, -1762, -1762, -1762,
   -1762,  1293, -1762,    94,   984, -1762,    33, -1762, -1762, -1762,
   -1762, -1762,  4587, -1762,  1290,  1557,  1646,   958, -1762,  1298,
   -1762,  2286,  1563,   575,  1303,  1302,   177,  1308,   746,  1528,
   -1762,  1357,  1528,   984,  1566,  1274, -1762,   639, -1762, -1762,
   -1762, -1762, -1762,  1463, -1762,    78, -1762,   289, -1762,   110,
   -1762, -1762, -1762,   -27,  1665,  3902, -1762, -1762,   984,  1569,
    4269,   984,  1536,   850,  1605, -1762,  1386,  1340,  1104,  1528,
     656,   221, -1762,  1279, -1762,   984,   372, -1762, -1762, -1762,
    1187,  1602, -1762, -1762, -1762, -1762, -1762, -1762,   256, -1762,
   -1762,   984, -1762,  1230,  1500, -1762, -1762, -1762,  1615,  1110,
    5031,  5031,  5031,    -5,   778, -1762, -1762, -1762,  1162, -1762,
    5031, -1762, -1762,  1682, -1762, -1762, -1762, -1762, -1762,  1369,
   -1762, -1762,  1306,  -102,  1622, -1762, -1762,   639,   955,  1297,
      64,    17,  5031,  1337,  5031, -1762,  5031, -1762,  5008,  1299,
    5031,  5031,  5031,  5031,  5031,  5031,  5031,  5031, -1762, -1762,
   -1762,  4587,  1555, -1762, -1762,  1403,  1454,  1786,  3085,  1436,
    1518, -1762,   106, -1762, -1762, -1762,   457, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762,   434,   808, -1762, -1762,
     347,  1592,  1592,  1592,  1592, -1762, -1762,  4587,  4587, -1762,
   -1762,   161,  1625,   754, -1762,  1318,   774, -1762,   984, -1762,
      44, -1762, -1762,   975,  1587, -1762,   639,   129, -1762,    33,
   -1762, -1762, -1762, -1762,   104,  1360,    78, -1762, -1762,  4587,
   -1762, -1762, -1762, -1762,  1394, -1762, -1762, -1762, -1762,   984,
     111, -1762,  1008, -1762, -1762,  1357,   256, -1762,  1552,     9,
     658, -1762, -1762,   984,   658,  1362, -1762,  1162, -1762, -1762,
      81,   862, -1762, -1762,  3799, -1762,  1720,  1553,  4587,  4587,
   -1762,  4372,   984, -1762,  1593, -1762, -1762,  4587,   639, -1762,
   -1762, -1762,  1665,  1562,   984, -1762,   943,    86,     9, -1762,
   -1762, -1762,   984, -1762,  1494, -1762, -1762, -1762,   -42,   984,
   -1762,   984,  1582,  1086,   -31, -1762,  1104,   546,  5008,  1319,
    1319,   886, -1762, -1762, -1762,  5031,  5031,  5031,  5031,  5031,
    5031,  4853,   778,  4587, -1762,  1151, -1762,  1306,  1104, -1762,
   -1762, -1762,  1592, -1762, -1762,  1329,  1332, -1762,   639,  1592,
    1564, -1762, -1762, -1762, -1762,  1755,  1592,  1511,  1511,  1511,
      87,  1550, -1762, -1762,   337, -1762,    13,   821,   984,   839,
      15,  1323, -1762,  1162, -1762, -1762,   382,  1324,   960,   394,
    1326,  1003,    16,    56,   502,  1328,  1017,  4484,   -25,  4587,
      78, -1762,  1449, -1762, -1762, -1762, -1762, -1762,  1151, -1762,
   -1762,  1395, -1762, -1762,  1395, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,  1389,   111,
   -1762,    98,   984,   984,   441, -1762, -1762, -1762,    61,   815,
    1421, -1762, -1762,  1664, -1762,  1530, -1762,    43,  1322,  1592,
    1533, -1762, -1762,  1538, -1762, -1762, -1762,  1610,  4484,   292,
   -1762, -1762, -1762,  2989, -1762,  1404, -1762, -1762, -1762, -1762,
   -1762,   161, -1762, -1762, -1762,  1104, -1762, -1762, -1762,  1151,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762,  1477,  1151, -1762,
    1406, -1762,  1758, -1762, -1762, -1762,   183, -1762,   639,   295,
   -1762,    68,     4,   599,    78,    78,  4484,   396,  1119,   808,
    1672, -1762, -1762,  1803, -1762,  1632, -1762, -1762, -1762, -1762,
    1552, -1762,   984,    74,   434,   836,  1375,  1688, -1762,  1377,
     639,   787, -1762,   337, -1762, -1762, -1762,  4587,  1187,   434,
   -1762, -1762, -1762, -1762,  -122,   984,  4484,   408,  1413,  1808,
     984,   282, -1762, -1762, -1762,  1512,  1513, -1762, -1762,  1008,
     -42, -1762,   667, -1762, -1762, -1762, -1762,  1187,  1647, -1762,
   -1762,  1230, -1762,   984, -1762, -1762,  1665, -1762,  1623, -1762,
     599,  1339,   599, -1762,  1162, -1762, -1762,   821,   800,   800,
    1319,  1319,  1319, -1762,  1029, -1762,  1422, -1762,   984, -1762,
    1538, -1762, -1762,  1592, -1762, -1762, -1762,  1187, -1762, -1762,
    1187, -1762, -1762, -1762, -1762, -1762, -1762, -1762,   108, -1762,
   -1762, -1762,  1755, -1762, -1762, -1762,   256,   256,   256, -1762,
   -1762, -1762, -1762, -1762,  1204,  1325,  4903, -1762,   984,  1204,
    1204,  5031, -1762,  1204,  1204,  1204,   364,  1204,  1204, -1762,
   -1762,  1559,  4484, -1762,    78, -1762, -1762,   477,   518, -1762,
   -1762,  3568, -1762,   450,   158, -1762, -1762, -1762, -1762,   977,
   -1762,  1499, -1762,  1485, -1762, -1762, -1762, -1762, -1762, -1762,
     331,   331,   331,   331,  1187, -1762, -1762, -1762, -1762,  1143,
    1187, -1762, -1762, -1762, -1762,   119, -1762,  1322, -1762, -1762,
   -1762, -1762, -1762, -1762,  4587, -1762,  4587,   161, -1762, -1762,
   -1762,  2989, -1762,   984,  1705,  1396,   811,  1722,  1399,   118,
     639, -1762, -1762,  1787, -1762, -1762, -1762, -1762,   295, -1762,
    1660, -1762,  1187,  1558, -1762, -1762,  1339,    78, -1762,  4587,
     267,   506, -1762, -1762, -1762,   984,  4587,   702, -1762, -1762,
   -1762,  1696,  1575, -1762,  1698, -1762,  1601, -1762, -1762, -1762,
   -1762,  1377, -1762, -1762, -1762,  1580,  1702,  1560,  1547,  1325,
   -1762,  4587,   118, -1762,  1567, -1762,   639, -1762,  1735,  1456,
   -1762, -1762,  1104, -1762,   913,  1845, -1762,   991, -1762, -1762,
   -1762,  1230,  1739,  1636,  1791,  4755, -1762,  1420, -1762, -1762,
   -1762,  1187, -1762, -1762, -1762,  1187, -1762, -1762, -1762, -1762,
     383,   383,    24,    24, -1762, -1762, -1762, -1762, -1762,  1421,
   -1762,  1142, -1762, -1762, -1762,   821, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,  1151,  1703,
    1151,  1707, -1762, -1762,  4587, -1762, -1762, -1762, -1762, -1762,
    1736, -1762, -1762, -1762, -1762, -1762, -1762,  1592,  1592,  1592,
    1592,   383, -1762, -1762,   383,    24,    24, -1762, -1762, -1762,
    4484,  1539,  4484,  1542, -1762, -1762, -1762, -1762, -1762,  1728,
   -1762,   811, -1762,  1764, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762,   118,  1008, -1762, -1762,  1008,   -36,   984, -1762,  1187,
    4484, -1762, -1762,   857,  3576, -1762,  1819,  1629,  1653,   -23,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762,   984,   723, -1762, -1762, -1762,  1727,  1606,
     984,  1421,  4484, -1762,  1808, -1762,  1345,  1779,  1345,  1560,
     480, -1762, -1762,  1730, -1762,  1611, -1762, -1762, -1762,   573,
   -1762, -1762,  1187,  1785,  1658, -1762,  1055, -1762,  1677,  1065,
    1474,  1689,  1444,  1187,  1110,  1187, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,  1495,
   -1762, -1762, -1762,   606, -1762, -1762, -1762,  1420, -1762,   984,
      62, -1762, -1762, -1762, -1762, -1762, -1762,   383, -1762, -1762,
   -1762, -1762, -1762, -1762,  1151, -1762,  1151, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762,  4587, -1762,  4587, -1762, -1762, -1762, -1762, -1762,
    1834,  1008,  1008, -1762,  1478,  1579,   808,    75, -1762, -1762,
   -1762, -1762,  1545,  4587, -1762,  1187,   698,  1651, -1762,  1652,
   -1762, -1762, -1762, -1762, -1762, -1762,   984, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762,   984,  1345, -1762,   984,  1746, -1762, -1762, -1762, -1762,
   -1762,   808, -1762, -1762, -1762, -1762, -1762, -1762, -1762,  1112,
    1230,  1187,  1187,  1721, -1762,  1187, -1762, -1762, -1762, -1762,
     349, -1762,  1187, -1762,   984,   984,  1108, -1762,  1603,  1230,
     606, -1762, -1762, -1762,   984, -1762,  1112, -1762, -1762, -1762,
   -1762,  1421,  1421, -1762,  4587,  1008, -1762,  4587,  1187,   808,
     808,  1591, -1762,   984, -1762,  1471,   984,  1759, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762,   984, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762,  1230,  1230,  1187,
   -1762,  1230, -1762,  1230, -1762,  1325, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762,   111,   808,  1187, -1762, -1762,   984,
   -1762, -1762, -1762, -1762, -1762, -1762,  1230, -1762, -1762, -1762,
   -1762,   111, -1762, -1762, -1762
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     0,     1,     0,     0,     0,     4,     6,     7,
      38,    38,     0,     0,     3,     5,     0,     8,    14,    28,
      27,    29,    29,     0,   276,   276,     0,     0,    24,    40,
       0,     9,    15,    30,    26,  1507,     0,   157,     0,   587,
     587,  1508,     0,     0,     0,     0,     0,     0,    39,   278,
       0,    17,     0,    25,    33,    37,    36,  1534,    35,    42,
     159,     0,   161,   288,   289,     0,   340,   281,   591,    18,
      20,    38,     0,    16,    34,  1535,    32,    41,   165,   163,
     249,     0,     0,   486,     0,   594,   592,   609,     0,    19,
      10,    11,     0,     0,     0,     0,     0,    43,    44,    45,
      47,    46,   160,     0,   247,     0,  1499,   258,   162,   251,
     253,   255,   256,   252,   269,   279,     0,     0,   489,  1342,
     282,   343,   290,   599,   599,     0,     0,     0,   276,    23,
      56,    71,    49,    80,  1461,   166,   165,     0,   158,     0,
    1527,     0,  1525,     0,  1500,  1551,   259,   260,   261,  1481,
     250,   254,   268,   270,   283,   341,     0,     0,   492,   287,
       0,   286,   344,  1449,   292,  1490,   599,   596,   602,     0,
     599,   610,   588,    21,    12,     0,  1507,    54,  1534,    55,
    1534,    60,    62,    63,    64,     0,     0,    70,     0,    73,
    1564,    48,     0,  1563,     0,     0,  1507,  1507,     0,     0,
    1542,  1507,  1507,  1507,  1507,  1507,     0,  1507,  1493,  1507,
      79,    81,    83,    85,    86,    87,    89,    88,    90,    91,
      92,    93,    94,    95,  1462,     0,   164,   249,     0,  1528,
       0,     0,  1526,     0,     0,  1552,  1495,  1482,  1501,   280,
     343,   487,     0,     0,   584,   343,   356,   358,   360,   346,
     351,   353,   355,     0,     0,   597,  1507,     0,   607,   600,
     601,   611,   587,  1507,     0,    57,  1534,    59,    61,     0,
    1474,  1507,     0,    77,     0,    72,    74,    52,    50,     0,
       0,  1362,   112,     0,     0,  1507,  1507,  1543,  1507,     0,
       0,     0,     0,     0,  1507,     0,  1494,     0,    99,    82,
      84,   167,   248,  1406,   275,  1353,  1355,   271,     0,     0,
       0,  1496,     0,  1502,     0,   284,   342,   343,   490,     0,
       0,   277,   285,   357,   359,   361,   349,   350,   352,   354,
       0,   367,   368,   347,   371,   371,   362,   305,  1495,  1507,
       0,     0,  1507,  1495,  1521,  1507,  1479,     0,   291,   293,
     296,   297,   298,   299,   300,   301,   302,   303,   304,     0,
       0,  1507,   608,     0,     0,   589,    17,     0,  1411,    69,
      58,  1473,     0,    76,    75,    78,    51,    53,  1507,   101,
     102,     0,     0,     0,   153,   152,   103,   104,   156,     0,
     155,   139,  1509,   141,    96,     0,    97,   169,  1467,  1468,
       0,  1354,     0,     0,     0,   262,   263,   266,   257,  1340,
     488,   343,   493,     0,   348,   372,   373,   363,     0,   373,
     365,     0,     0,  1507,  1479,     0,     0,     0,     0,     0,
    1522,  1507,     0,  1480,     0,     0,   294,   295,   603,   604,
     606,     0,   598,   612,   614,     0,     0,    68,     0,  1420,
    1416,  1421,  1419,  1417,  1422,  1418,   145,   146,   148,   154,
     151,   150,  1511,  1510,   142,     0,   111,   110,   100,   107,
    1549,   105,     0,  1407,   273,     0,   274,   264,     0,   265,
    1341,   491,   495,   585,   381,   375,     0,   317,   337,  1469,
    1470,   326,  1356,   321,   320,   319,  1361,  1360,  1517,  1493,
    1505,     0,   583,   338,   339,  1507,  1507,   605,   614,     0,
       0,    13,    66,    67,    65,   117,   131,   127,   132,   114,
     130,   128,   115,   116,   129,   113,   118,   119,   121,   147,
       0,   140,   143,   108,  1550,  1507,    98,   184,  1521,     0,
    1559,   230,     0,  1521,  1512,  1507,  1491,  1512,   233,     0,
     232,  1563,   217,   216,   168,   170,   171,   172,   173,   174,
     175,     0,   176,   177,   229,   178,   179,   180,   181,   182,
     183,   185,  1507,   272,   267,     0,   494,   496,   497,   586,
       0,  1483,     0,  1509,   366,     0,   307,  1357,  1518,   328,
       0,   310,  1506,  1547,   336,     0,     0,     0,   620,   624,
     615,   616,   617,   618,   623,     0,     0,   120,   123,     0,
     149,   144,     0,   106,  1507,  1512,  1560,   192,   234,  1507,
    1513,  1507,     0,  1492,  1507,  1488,  1507,     0,  1507,  1507,
     241,  1481,     0,  1507,     0,   500,   498,   398,     0,   472,
     407,   440,   428,   437,   434,   431,  1561,   408,   409,   410,
     411,   412,   413,   414,   415,   416,  1538,   370,   441,     0,
     417,   404,   418,   419,     0,     0,  1545,   421,   422,   420,
     468,   424,   425,   423,  1507,  1507,   485,   364,   382,   383,
     384,   385,   386,   387,   405,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   397,     0,     0,  1484,     0,   376,
       0,   318,   309,   308,   306,   327,  1493,  1548,   315,   324,
     323,   325,   322,     0,   622,   625,   686,   737,   746,   753,
     757,   781,   786,   804,   797,   805,   806,   812,   845,   854,
     856,   883,   891,   893,  1559,   899,     0,   910,   931,   933,
     969,   971,   975,   685,   981,   994,  1014,  1031,  1033,  1037,
    1044,  1045,  1061,  1081,  1099,     0,  1118,  1129,  1137,  1139,
    1141,  1143,  1148,  1170,  1193,  1565,  1566,  1567,  1568,   619,
     631,   632,   633,   634,   635,   636,   637,   638,   640,   639,
     641,   642,   643,   644,   645,   646,   647,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,   659,   660,
     661,   662,   663,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   679,  1166,
    1167,  1168,   680,   681,   682,   683,   684,    22,     0,   122,
     109,     0,  1507,   194,   193,   189,     0,     0,  1487,   233,
     228,     0,     0,   231,     0,     0,   240,  1532,  1532,     0,
     242,     0,   213,  1507,   484,   438,   439,   426,   427,   435,
     436,   432,   433,   429,   430,  1562,     0,  1539,   466,   448,
     399,  1402,   482,  1546,   469,   470,   467,     0,     0,     0,
    1401,   401,   403,  1465,  1465,     0,  1525,  1525,   380,   377,
    1412,  1414,   474,   476,   478,  1486,   329,   330,   331,   332,
       0,     0,   311,  1504,   317,     0,     0,   626,     0,     0,
       0,  1257,   752,     0,   783,   788,     0,     0,     0,     0,
       0,     0,  1257,   885,     0,     0,   895,   900,     0,  1257,
       0,     0,     0,     0,     0,     0,   983,  1004,     0,     0,
       0,     0,     0,     0,     0,     0,  1113,  1111,     0,     0,
    1138,  1136,     0,     0,     0,     0,  1171,  1177,     0,     0,
     137,   133,   138,   136,   134,   135,   124,   125,   202,   203,
     201,   200,     0,   187,   188,  1519,   222,   221,   222,   218,
     246,   235,   236,  1452,   239,  1533,   243,   244,   245,  1358,
    1507,   509,   509,  1509,   529,   501,   504,   505,     0,   471,
       0,  1557,     0,  1403,  1404,     0,   406,   473,     0,     0,
     400,  1491,   442,   443,  1413,     0,     0,     0,  1561,   477,
       0,     0,     0,  1485,  1493,   316,   613,   621,   735,   705,
    1525,     0,     0,  1435,  1438,  1525,  1333,     0,     0,     0,
       0,     0,     0,     0,     0,  1435,   744,  1377,   742,  1367,
    1369,  1375,  1376,  1454,   747,     0,  1256,  1278,  1352,     0,
    1348,  1350,  1349,  1396,   759,  1395,  1397,   785,   782,   787,
     800,     0,  1335,  1463,  1536,     0,  1416,   843,   705,     0,
    1369,   852,     0,   759,   862,   861,  1477,   858,   860,   890,
     887,   886,   889,   884,  1525,   892,  1363,  1365,   894,  1346,
     904,  1555,  1255,   912,   932,   511,     0,   935,   936,   937,
     970,  1085,     0,   972,     0,   979,     0,   982,  1005,  1352,
     995,  1004,   997,     0,  1002,     0,  1349,     0,  1456,  1195,
    1338,  1536,  1195,     0,  1059,  1050,  1339,     0,  1345,  1062,
    1063,  1064,  1065,  1066,  1074,  1067,  1077,     0,  1343,     0,
    1082,  1100,  1114,  1115,  1489,     0,  1120,  1122,     0,  1134,
       0,  1140,     0,  1145,  1150,  1178,     0,  1179,  1527,  1195,
       0,  1452,   196,   195,   186,     0,     0,   220,   219,  1472,
    1507,     0,   212,   206,   237,  1453,  1359,   214,     0,   510,
     506,     0,   507,     0,   499,   502,   449,  1558,   450,  1525,
       0,     0,     0,  1321,  1319,  1384,  1324,  1378,  1382,  1383,
       0,  1405,   483,     0,   402,  1466,   379,   378,  1415,  1540,
     479,   335,  1517,     0,   313,   736,   687,  1486,     0,   714,
       0,     0,     0,     0,     0,  1423,  1440,  1434,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1424,   745,
     738,     0,     0,  1368,  1455,   750,  1454,  1258,     0,   755,
       0,   760,   770,  1394,   784,  1393,   790,   801,   798,   803,
     802,  1257,  1336,  1464,  1337,  1537,  1244,   811,   844,   813,
     823,  1210,  1210,  1210,  1210,   853,   846,     0,     0,   855,
    1478,  1257,   881,   868,   864,   866,  1257,   888,     0,  1364,
     897,  1556,   902,   914,     0,   512,     0,   958,   943,   934,
     938,   940,   941,   942,  1089,     0,     0,   980,   976,     0,
     988,   985,   987,   986,   989,   996,   999,   627,  1257,     0,
       0,  1006,     0,  1457,  1458,  1536,     0,  1032,  1016,  1039,
    1052,  1060,  1046,     0,  1052,     0,  1389,  1390,  1075,  1078,
       0,     0,  1344,  1073,     0,  1072,     0,  1102,     0,     0,
    1112,     0,     0,  1121,     0,  1135,  1130,     0,     0,  1146,
    1147,  1144,  1489,     0,     0,  1180,     0,     0,  1039,   126,
     206,   204,   197,   198,     0,   225,   211,  1471,  1452,     0,
     503,   508,   514,   524,   371,   530,  1527,   444,     0,  1329,
    1330,     0,  1322,  1323,  1408,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1541,  1559,   334,  1517,  1527,   312,
     710,   701,  1210,   691,   698,   692,   694,   696,     0,  1210,
       0,   690,   697,   704,   703,     0,  1210,  1523,  1523,  1523,
     708,   709,  1386,  1385,     0,  1374,  1321,  1319,     0,     0,
    1321,     0,  1370,  1371,  1372,  1334,  1321,     0,     0,  1321,
       0,     0,  1321,  1321,  1321,     0,     0,  1217,  1463,     0,
       0,   748,     0,  1269,  1270,  1271,  1304,  1272,  1559,  1308,
    1313,  1553,  1279,  1316,  1553,  1297,  1276,  1286,  1268,  1267,
    1305,  1275,  1277,  1287,  1288,  1289,  1290,  1291,  1306,  1260,
    1309,  1311,  1292,  1293,  1294,  1295,  1296,  1263,  1264,  1265,
    1266,  1280,  1303,  1274,  1285,  1262,  1261,  1273,  1282,  1283,
    1284,  1281,  1298,  1299,  1300,  1301,  1302,  1259,     0,     0,
    1351,   766,     0,     0,   773,   795,   796,   789,   791,     0,
    1217,  1249,  1251,   808,  1245,  1246,  1247,     0,  1564,  1210,
       0,  1211,   816,  1213,   817,   814,   815,     0,  1217,  1463,
     876,   878,   877,   871,   873,   879,   882,   857,   869,   865,
     863,  1257,   627,   859,  1366,  1527,   896,  1347,   627,  1559,
     922,   923,   925,   927,   928,   924,   926,   917,  1559,   913,
       0,   959,     0,   961,   960,   962,   953,   954,     0,     0,
     939,  1091,  1529,     0,     0,   973,  1217,  1463,  1563,     0,
    1000,   628,  1007,  1008,  1011,     0,  1003,  1202,  1201,  1010,
    1016,  1196,     0,     0,  1244,     0,     0,     0,  1051,     0,
       0,     0,  1076,     0,  1080,  1079,  1070,     0,  1507,  1244,
    1117,  1116,  1123,  1124,  1125,     0,  1217,  1463,     0,  1450,
       0,  1125,  1192,  1182,  1181,  1187,     0,  1189,  1190,  1197,
    1452,   199,     0,   208,   209,   238,   207,  1507,   516,   527,
     528,   526,   532,     0,   458,   459,  1489,   447,   460,   456,
     454,  1512,   452,  1379,  1380,  1381,  1332,  1320,  1325,  1326,
    1327,  1328,  1331,  1409,     0,   567,   480,   333,     0,   702,
    1213,   693,   695,  1210,   699,   689,   729,  1507,   718,   719,
    1507,   730,   720,   721,   724,   734,   731,   722,     0,   732,
     723,   733,   715,   716,   688,  1524,     0,     0,     0,   706,
     707,  1388,  1373,  1387,  1435,  1463,     0,  1439,     0,  1435,
    1435,     0,  1432,  1435,  1435,  1435,     0,  1435,  1435,  1218,
     739,  1220,  1217,   751,     0,  1307,  1554,  1310,  1312,   756,
     754,   761,   762,   602,     0,   772,   771,  1183,  1184,   776,
     774,     0,   794,     0,   799,   627,   627,   809,   807,  1248,
     823,   823,   823,   823,  1507,   828,   841,   842,   829,     0,
    1507,   832,   833,   836,   834,     0,   835,   825,   826,   818,
     824,   627,  1214,  1209,     0,   847,     0,  1257,  1257,   875,
     627,   872,   867,     0,   905,     0,     0,   929,     0,     0,
       0,   955,   957,     0,   949,   965,   950,   951,   944,   945,
     965,  1083,  1507,     0,  1530,  1090,  1512,   974,   977,     0,
       0,   991,  1001,   998,   630,     0,     0,  1018,  1017,  1233,
    1235,  1035,  1230,  1231,  1042,  1040,     0,  1257,  1053,  1257,
    1047,  1055,  1068,  1069,  1071,  1459,  1109,  1224,     0,  1463,
    1131,     0,     0,  1451,  1151,  1152,     0,  1155,  1158,  1162,
    1156,  1188,  1527,  1191,  1203,  1475,   205,     0,   226,   227,
     223,     0,     0,   518,     0,  1544,   451,     0,   445,   461,
     457,  1507,   446,   453,  1410,  1507,   475,   314,  1208,   700,
       0,     0,  1253,  1253,   717,   712,   711,   713,  1428,  1217,
    1436,     0,  1448,  1433,  1426,  1446,  1427,  1429,  1430,  1443,
    1444,  1431,  1425,   627,  1221,  1216,   740,   749,  1559,     0,
    1559,     0,   763,   764,     0,   768,   767,   769,  1185,  1186,
     779,   777,   627,   792,   793,  1250,  1252,  1210,  1210,  1210,
    1210,     0,   830,   831,     0,  1253,  1253,   827,  1212,   627,
    1217,  1335,  1217,  1335,   874,   880,   870,   898,   906,   908,
     915,   918,   919,  1497,   930,   911,   916,   965,  1391,  1392,
     965,     0,   948,   946,   947,   952,  1093,     0,  1531,  1507,
    1217,   990,   984,     0,   629,  1012,     0,     0,  1024,     0,
     627,   627,  1036,  1034,  1232,  1043,  1038,  1041,  1048,   627,
    1057,  1056,  1460,     0,     0,  1110,  1101,  1225,  1127,  1227,
       0,  1217,  1217,  1142,  1450,  1154,  1505,  1160,  1505,  1224,
       0,  1240,  1242,  1206,  1204,  1237,  1238,  1205,  1476,     0,
     224,   515,  1507,     0,   520,   525,  1523,   561,   581,   576,
    1479,     0,     0,  1507,  1525,  1507,   531,   537,   538,   539,
     548,   540,   542,   545,   549,   533,   534,   535,   541,   544,
     562,   546,   536,     0,   543,   547,   465,   462,   463,     0,
       0,  1400,   726,  1398,  1399,   725,   728,     0,   727,   741,
    1437,  1219,   627,  1315,  1559,  1318,  1559,   765,   780,   758,
     627,   775,   822,   821,   820,   819,   838,   837,   840,   839,
    1215,   849,     0,   848,     0,   627,   909,   903,   920,  1498,
       0,   964,   956,   965,   967,     0,     0,  1096,  1092,  1087,
     978,   993,     0,     0,  1019,  1507,  1026,     0,  1020,     0,
    1023,  1234,  1236,   627,  1054,   627,  1103,  1104,  1105,  1106,
    1107,  1108,   627,  1128,  1119,  1228,  1223,  1126,  1133,  1132,
    1153,     0,  1505,  1157,     0,  1164,  1176,  1173,  1175,  1174,
    1169,  1172,   627,   627,  1207,  1194,  1239,  1200,  1199,  1514,
       0,  1507,  1507,   522,   560,  1507,   582,   580,   577,   578,
    1509,   570,  1507,  1257,     0,     0,     0,   563,     0,     0,
     568,   571,   574,   464,   455,   481,  1514,  1222,  1314,  1317,
     778,  1217,  1217,   907,     0,   963,   968,     0,  1507,  1094,
       0,     0,  1084,  1086,   992,     0,     0,  1029,  1027,  1028,
    1022,  1021,  1049,  1058,  1226,   627,  1159,     0,  1163,  1165,
    1149,  1241,  1243,  1515,  1516,  1198,   517,     0,     0,  1507,
     513,     0,   569,     0,   566,  1463,   564,   565,   555,   553,
     554,   556,   552,   557,   551,   550,   575,   573,   572,  1254,
     851,   850,   921,   966,     0,  1097,  1507,  1088,  1257,  1025,
    1030,  1015,  1229,  1161,   519,   521,     0,   559,   558,   579,
    1095,     0,  1013,   523,  1098
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1762, -1762, -1762, -1762,  1894, -1762, -1762, -1762,     0, -1762,
   -1762, -1762, -1762, -1762,  1537, -1762, -1762, -1762,  1199, -1762,
   -1762,    48,  1880, -1762, -1762,  1850,   842, -1762, -1762, -1762,
   -1762, -1762,  1712,  1774, -1762, -1762,  1729,   437, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762,  1723, -1762, -1762, -1762, -1762,
    1699, -1762, -1762, -1762, -1762, -1762,  -225,   558, -1762, -1762,
   -1762, -1762,  1388, -1762, -1762,  1304,   748, -1762, -1762, -1762,
   -1762, -1762, -1762,  1460, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762,  1790, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,   542, -1762,
     532,   757, -1762, -1762, -1762, -1762, -1762,   951,    45, -1762,
    1305, -1762, -1762, -1762, -1762, -1762, -1762,    93, -1762, -1762,
    1708, -1762,  1828, -1762, -1762, -1762, -1762,  1535, -1762, -1762,
    1825,   655, -1762, -1762, -1762, -1762,  1700, -1762,  1892,  1784,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,  1041, -1762,
   -1762, -1762,  1350, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762,   301, -1762, -1762, -1762,  1701,
    1704,  1706,  1617, -1762, -1762,   760, -1762, -1762,  -323, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,    57,
      63,  1071,    65, -1762,    66, -1762, -1762, -1762,   269,   276,
   -1762,   354, -1762,   281, -1762,  -125,    69, -1762, -1762,    70,
   -1762,    71, -1762, -1762, -1762,  1076, -1762, -1762, -1762,    80,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,  1397, -1762,
   -1762, -1762,   971, -1762,  -946, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762,  -106, -1762, -1762, -1762, -1762, -1762,  -234, -1762,
   -1762, -1762, -1762,  -166, -1762, -1762,    95, -1762, -1762, -1762,
   -1762, -1762,  1854,  1048, -1762,   216, -1762, -1762, -1762, -1762,
    1472, -1762, -1762, -1762, -1762, -1762, -1762,  -492, -1762, -1762,
     138, -1762, -1762, -1762, -1762,   903,   543,   545, -1762, -1762,
     266, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762,   900, -1762, -1762,   228,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
    -233, -1762,   193, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762,   695, -1762, -1762,   699, -1762, -1762, -1762,
   -1762,   423,   188, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,    20,
     693, -1762, -1762, -1762, -1762, -1762, -1762, -1762,   696, -1762,
   -1762, -1762,   170, -1762, -1762,   403, -1762, -1762, -1762, -1428,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1389,   883, -1762, -1762,   157, -1762, -1762,   386, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
     629, -1762, -1762, -1762, -1762, -1762, -1762, -1762,   666, -1762,
     150, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762,   871, -1762,   876, -1762, -1762,  1080, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,   874,   374,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,    -8, -1762,
     377, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
   -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762, -1762,
    -511, -1762, -1263, -1762, -1762, -1279, -1141, -1097, -1762,   330,
   -1762, -1451, -1762, -1762, -1762, -1762,    -7, -1762, -1762, -1762,
   -1762,  -115, -1762, -1762,   181, -1762, -1762, -1762, -1762,    -9,
   -1762,  -645, -1761, -1762, -1762,   492, -1762,  -970, -1312,  -864,
   -1251, -1762, -1762, -1762, -1239, -1235, -1233, -1223, -1221,    79,
    -284,   -34,  -739, -1195,  -902,   192,   911, -1067,   -84, -1762,
   -1129, -1762,  -888, -1762,   779,  -228,   -82, -1762, -1762,  -743,
     838,  -886, -1041,  -668,  -209, -1762, -1762,   414, -1082, -1679,
   -1019,  1139,  -936,  -317,  -633,  -192, -1762,  1047,  -223,  -691,
      25,  -328,   341, -1012, -1762, -1762, -1762, -1762, -1762,  1802,
   -1762, -1121,   798, -1762, -1762, -1762, -1715,  1172,   -26,  1714,
     743,  -455, -1762,   974,  -420,  1428, -1762,  -654, -1762, -1106,
    1051,  -456,   780, -1762, -1762,  -748, -1762, -1238,  -175,  -578,
    -537,  -155, -1101, -1762,   617, -1379,  -841, -1100, -1762,  1238,
    2007,  -922, -1762, -1762, -1762, -1762, -1762, -1762, -1762,   582,
   -1762, -1762,  -721,  1053,  -129, -1762, -1762, -1762, -1762
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,     6,     7,     8,    24,    39,    69,   128,
     262,     9,    25,    40,    70,    90,   511,    73,    71,    35,
      11,    21,    27,    42,    57,    58,    17,    37,    77,    97,
      98,   191,   192,   178,    99,   179,   180,   181,   182,   514,
     183,   184,   100,   187,   188,   189,   274,   101,   210,   211,
     212,   213,   471,   395,   536,   468,   469,   470,   214,   378,
     525,   526,   527,   828,   966,   528,   967,   215,   391,   392,
     531,   216,   456,   457,   217,   218,   219,   220,   221,   222,
     223,    48,    78,    80,   104,   102,   135,   397,   472,   555,
     556,   975,   835,  1174,  1382,   557,   971,   558,  1388,  1389,
    1663,  1183,   559,   560,   561,   562,   979,  1177,  1890,   563,
     564,   565,   566,   567,   568,   569,   570,   850,   571,   138,
     108,   109,   110,   111,   149,   112,   405,   406,   479,   113,
     114,    31,    66,   154,    84,   239,   159,   120,   160,   121,
     164,   254,   349,   350,   704,   351,  1419,   904,   586,   352,
     495,   353,   710,   354,   355,   705,   896,   897,   898,   899,
     356,   357,   358,    83,   240,   161,   162,   163,   249,   250,
     251,   252,   333,   484,   486,   334,   335,   677,   417,   418,
     581,   888,   336,   580,   678,   679,   680,  1010,   681,   682,
     683,   684,   685,  1677,   686,  1001,  1397,  1902,  1678,  1679,
    1680,  1681,  1898,  1682,  2087,  2088,   687,   688,   876,   689,
     690,   691,   584,  1018,   892,   893,  1906,   692,   693,   694,
     118,   317,   158,   411,   244,   482,   576,   577,   578,   853,
     995,   996,  1190,  1191,  1104,   997,  1668,  1893,  2054,  2193,
    2260,  1392,  1671,  1194,  1395,  1895,  2075,  2076,  2275,  2077,
    2078,  2079,  2080,  2266,  2081,  2082,  2083,  2210,  2211,  2200,
    2084,  2085,  2197,   503,   321,   579,    51,   261,   444,    87,
     124,   123,   166,   167,   168,   258,   363,   126,   365,   508,
     509,   600,   601,   602,   603,   604,   908,  1610,  1611,  1844,
     605,   770,   771,   909,  1028,  1229,  1440,  1441,  1436,  1722,
    1723,  1226,   772,   910,  1046,  1252,  1250,   773,   911,  1054,
    1471,   774,   912,  1529,   775,   913,  1262,  1531,  1761,  1762,
    1763,  1534,  1769,  1952,  1950,  2110,  2109,   776,   914,  1068,
     777,   915,  1069,  1537,  1538,   778,   916,  1070,  1268,  1271,
     779,   780,   781,   917,  1778,   782,   918,   783,   919,  1077,
    1549,  1797,  1798,  1279,   784,   920,  1081,  1286,   785,   921,
     786,   922,  1086,  1087,  1292,  1293,  1294,  1572,  1570,  1810,
    1295,  1563,  1564,  1809,  1567,   787,   923,  1093,   788,   924,
     789,   925,   790,  1099,  1576,   791,   927,   792,   929,  1578,
    1979,  2125,  2127,   793,   930,  1303,  1587,  1817,  1981,  1982,
    1983,  1985,   794,   931,   795,   932,  1106,  1309,  1310,  1311,
    1599,  1828,  1829,  1312,  1596,  1597,  1598,  1822,  1313,  1992,
    2227,   796,   933,   797,   934,  1113,   798,   935,  1115,  1318,
     799,   936,  1117,  1324,  1609,  2002,   800,   937,  1120,  1327,
    1843,  1121,  1122,  1123,  1613,  1614,   801,   938,  1623,  2008,
    2146,  2237,  2291,   802,   939,   803,   940,  2013,   804,   941,
    1624,  2016,   805,   806,   942,  1134,  2153,  1344,  1626,  2019,
    1860,  1861,  2155,  1342,   807,   943,  1139,  1140,  1141,  1142,
    1356,  1143,  1144,  1145,  1146,   808,   944,  1110,  1996,  1314,
    2233,  1601,  1831,  2137,  2232,   809,   945,  1357,  1639,  2023,
    2026,   810,  1153,  1152,  1360,   811,   948,  1155,  1156,  1867,
    2164,   812,   949,  1159,  1366,   813,   951,   814,   952,   815,
     953,   816,   954,  1371,   817,   955,  1373,  1874,  1875,  1651,
    1877,  2037,  2173,  2039,  2250,   818,   819,   957,  2180,  1167,
    1376,  1655,  1770,  1951,  1882,   820,  1657,   821,   822,   959,
    1337,  1884,  2134,  2043,  2185,  1699,  1552,  1553,  1801,  1803,
    1969,  1750,  1751,  1933,  1935,  2102,  2028,  2029,  2162,  2166,
    2245,  1851,  1852,  2010,  1853,  2011,  2044,  2045,  2182,  2046,
    2183,  1543,  1544,  1545,  1775,  1546,  1776,  2096,  1101,  1102,
    1056,  1057,  1257,  1258,  1502,  1503,  1504,  1505,  1506,  1203,
    1405,  1447,  1047,  1071,  1272,  1129,  1135,   408,   409,  1147,
    1148,  1300,  1124,  1060,  1061,   304,   305,   491,  1187,   498,
     282,  1095,  1096,  1048,  1073,  1206,  1444,  1732,  1830,  1987,
    1079,  1125,  2092,  1050,   880,   871,  1003,  1004,  2094,  1051,
     890,   891,  1052,  1235,  1237,  1451,  1465,  1460,  1457,   253,
    1876,  1184,  1255,  1335,  2024,   225,  1274,  1012,   400,   425,
    1185,   271,  2049,  1807,   434,   238,   698,  1230,   627,   169,
     624,   297,   312,  2130,   145,   314,   905,   593,    43,   465,
     621,  2255,   589,  1175,   431,  1726,   233,   230,  1835,   986,
     185,  1276,   868,  1415,   288,   696,   708,   535,   236,  1757,
    1302,  1198,   617,   866,  1550,   823,   824,   825,   826
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     122,   264,   307,   194,   490,   700,  1161,   889,  1277,   894,
     628,   629,   420,   926,  1072,   958,  1612,   572,  1352,  1522,
    1919,   283,   284,  1523,  1059,  1524,   289,   290,   291,   292,
     293,   870,   295,  1248,   298,  1525,   306,  1526,   306,   306,
    1565,   369,   306,   591,  1103,  1015,  1016,  1055,  1361,  1264,
    1381,   900,  1094,   458,  1299,  1345,  1467,  1149,  1088,  1727,
    1728,    22,  1299,   462,   194,  1402,  1290,  1021,  1377,  1619,
      89,   885,   105,   165,   296,    85,   122,   608,   832,   359,
    1179,   360,  1133,  1402,  1094,  1402,  1402,  1094,   367,  1774,
      14,   512,   311,  1558,   186,  1656,   372,   306,   981,  1434,
    1094,   306,   394,   306,    54,    41,   105,  1805,  1299,   702,
     381,   382,   306,   383,  -190,   306,  1094,   505,   883,   389,
      92,  1416,   415,  2047,  1606,  1306,  1402,   885,   458,    63,
    1437,   269, -1527,   895,   496,    52,   127,   440,  1591,  1780,
     415,  1554,  1555,  1556,   446,  1420,  -191,   165,    63,   308,
    1704, -1485,   310,  1437,  2031,  1838,   976,  1724,  1575, -1512,
    1358,    88,  -743,   620,   422,   165,  1442,   427,   429,  1390,
     432,  2135,  1646,   709,   474,  2147,   476,  1287, -1563,   946,
     306,   874,  1560,   272, -1489,  1592,   441,  1007,   990,  1231,
     306,   165,  1591,  2033,  1238,  1870,  1137,   306,   487,   625,
    1849,   894,   610,   448,  1833,   499,   368,   384,   473,  1338,
     306,   388,   306,   390,  1603,   991,   992,  2148,   507, -1527,
   -1563,    41,   401,     3,  1590,   401,  1179,   493,   407,  1912,
     630,  1353,  -190,   492,   259,   306,   313,  2149,   165,  1592,
    1965,  1108,   532,   533,  1945,  1391, -1507,   573,   489, -1563,
     902,  1078,   190,  1298,   463,   574,   501,   449,  1165,  1350,
   -1489,    12,  2230,  1205,  -191, -1563,  1649,  1665,   504, -1563,
     278,    13,    41,     4,  1752,   626,   847, -1563,  1221,  1222,
    1058,  1160,   306,   306,   306,  1166,  1648,    16,   977,  1834,
     447,  1138,   947,  1532,  1138,   885,  1673,  1438,   450,   587,
     459,  1936,  1593,   875,  1824,   451,   884,   461,   611,  1299,
     582,   613,  2133,   449,  1359,    64,  1697,  1621,  1698,   190,
    1565,   407,  1058,  1273,   480,  1700,   631,  1443,   494,  1220,
     595,   596,  1700,     5,    64,   106,   886,  1850,  1288,  1700,
     303,    41, -1534,  1179,   260,   497,  1703,  1058,  1781,  1072,
     -31,  1825,  1594,    55,   450,  1403,  1593,   366,  1398,   879,
     612,   451,   701,  2231,  1307,  1439,    65,   706,   978,  1223,
     622,   275,   462,  1403,   703,  1403,  1403,   452,  -958,   303,
     620,   306,  1216,  1217,   229,  2239,  1072, -1512,  1439,   506,
   -1501,   968,   401,   416,   401,   106,  1885,   634,  2136,  1181,
     303,     5,  1995,   711,   711,   695,  1594,  1540,  1799,   635,
     273,   419,  1577,  1620,  2150,  1193,  1403,  1072,  1783,  -958,
     993,   590,   632, -1489,  1909,  1764,  -958,  1308,  1022,  1753,
    1605,   270,  1088,   452,   107,   973,   869,    56,  -190,   831,
     453, -1489,  1616, -1507,   836,    89,   837,  1533,   281,   841,
     190,   842,  1402,   844,   845,    86,  1404,  -595,   851,   303,
     609,  1205,  1205,  1205,  1402,  1072, -1489, -1489,  2099,   454,
    -191,  1205, -1489,   872,  1734,  1813, -1441,  1744,   513,  1806,
    1417,   449,   281,  1946,  1631,  1782,   303,  1291,   303,  1658,
     449,   401,  1263,  1205,   107,  1205,   453,  1205,  -958,   877,
     878,  1205,  1205,  1205,  1205,  1205,  1205,  1205,  1205,   152,
     455,   887, -1485,  1354, -1489,   303,  1823,  1745,  1826,  2121,
     303,  2123,   450,   303,   175,   454, -1487,   848,   303,   451,
    -593,   450,  1913, -1489,  1355,   190,   303,  1138,   451,  1886,
    1595,   316,  1841,  1966,   190,  1181,   322,  2020,  1862,  2140,
    2299, -1489,   303,   303,   190,   177,  1565,  1014,   606,  2131,
     303,  -958,  2132,   463,  1299,  1561,   455,  1072,  1224,  1562,
    1897,   438,  1402,  1731,  1674,  -595,   901, -1489,   175, -1402,
    2168,  2169, -1563,  1839,   969,   849,   994,  1929,   510,    75,
    -958,   175,  1029,  1947,  1595,  1871,  1065,  1180,  1445,    55,
   -1563,   452,  1080,   150,  1767,  1452,  1700,  1097,  1098,  1970,
     452,  1972,  1384,  1097,  1105,  1107, -1563,   268,   410,   368,
    1065,  1339,   398,   974, -1489,  -958,  1097,  1674,   903,   855,
       4,  -958,   970,  1938,    23,  1105,  1162,   176,  1164,  1002,
    1273,  1760, -1534,    75,  2000,   306,  2176,  1541,  -593,   306,
     439,   175,   306,   306,    26,  1764,    75,   972,  1378,   306,
    1227,  1675,  1181,  -210,   453,  1574,  1205,  1205,  1205,  1205,
    1205,  1205,  1205,   453,  1940,  1172,  2032,  2195,   998,   856,
      32,  1182,  1025,    56, -1503,  1207,  1627,  2177,   895,  1218,
    1627,   176,  1351,   454,  1696,  2178, -1485, -1563,   303,   399,
    1542,  1049,   454,   268,   176,  2225,  1827,  1915,  1916,  1917,
    1049,  1082,   481,  1937,  1675,  1233,    75, -1563,  1837,  1385,
    1062,   177,  1918,   190,   303,  1114,  1116,  1923,  1924,  1943,
    1768,  1926,  1927,  1928,   455,  1931,  1932,   303,  1990,  1157,
    1049,  1111,  1403,   455,  1273,  1126, -1563,  1130,  1676,  1130,
    1136,  1265,  1111,   436,  1403,   980,  1273,  1755,     5,   497,
       5,  1939,   983,   984,   176,  1683, -1563,  1130,  2179,   989,
    2280,  2281,  1548,  1547,   437,  1930,   368,  1196,  1097,   303,
     306,  1535,  2040,   174, -1485,  2157,  1097,  1305,  1548,  2158,
    1771,  2159,   265,  1173,  2035,   857,  1299,   306,  2171,  1213,
    2174,  2160,  1941,  2161,  1065,   267,  2097,  2097,  1127,   368,
    1228,  1733,    19,  1212,    29,  1188,  2112,  2113,  2114,  2115,
    1346,  1138,   256,  1580,  1849,   303,  1581,  1582,  1097,   306,
     306,  2091,  1097,  1074,   449,  1406,  1407,  1408,  1409,  1253,
    1410,  1364,   368, -1447,  1305,   858,   516,  1406,  1407,  1408,
    1409,   960,  1410,    18,  1128, -1445,  1131,   881,  1815,  2097,
    2097,  1151,  1403, -1485,  2208,   370,  2187,  1818,   257,  1253,
    1163,  1265,   833,  1536,  1105,   450,  -345,  -345,   882,  1478,
    1479,   374,   451,  1207,  1207,  1207,    30,   517,    20,    33,
    1171,  2264,   961,  1207,   518,  2300,    36, -1501,   190,   962,
   -1563,  2007,  1971,  1315,  1973,  2209,   375,  1186,  1480,   859,
    1346,  1541,  2304,    91,  1481,  1207,   834,  1207,  1887,  1207,
    2188,   195,   119,  1207,  1207,  1207,  1207,  1207,  1207,  1207,
    1207, -1485, -1449, -1449,  2247,   196,   197,  1072,    34,   497,
     497,  1136, -1563,  2098,  1975,  1483,  1157,   190,   119,  1484,
      38,  1253,    93,   861,   452,  1406,  1407,  1408,  1409,   860,
    1410,  1850, -1563, -1442,  1542,   313,   520,   198,  1204,  1072,
    1393,   963,    44,  2181,  1304,  2095,  2302,   256,   956,  1854,
    1421,  1422,   146,  1383,  1579,  1266,  1888,  1580,  1889,  1346,
    1581,  1582,  1127,  2018,  1866,  2118,  2119,  1205,  1328,  1097,
    1733,    45,  1205,   862,  1333,  1386,   199,   200,   541,   201,
    1239,  1329,  1615,    49,    94,   303,  1617,   453,   202,  1265,
     863,  2091,  1423,   257,  1424,  2116,  1628,  1208,  2117,   521,
     423,   306,   424,  1097,   964,  1260,   285,   147,   148,   542,
      46,    50,  1468,  1618,   294,  1644,   454,    53,    47,  1527,
    1334,  1346,  1089,  1090,    95,  1772,    96,  1652,  1425,  1426,
    1427,   203,   204,   301,  1583,  1584,  1091,  2041,   449,  2229,
     864,  1672,   205,   337,  1305,  1773,  1855,  2156,  1557,  1559,
    1812,   303,    59,  1585,  1586,    60,  1814,   455,  1207,  1207,
    1207,  1207,  1207,  1207,  1207,    61,  1856,  2141,  -345,   524,
    -345,  -345,  1653,    62,   965,  1428,  1654,  1429,  1062,   450,
    1607,  1346,  1092,  2189,    68,  1430,   451,  2142,   421,   338,
       5,   379,   380,   428,  1320,   838,  1541,    72,  1989,   386,
     387,  1735,   839,  1132,   339,   393,   396,  1539,  1062, -1563,
     466,   497,   467,   190,  1901,  1636,  1832,   206,    79,  1640,
    1641,  1169,  1643,  1097,   246,   614,   247,   248,  1647,    75,
     619,  2216,  1347,  2285,   326,  1321,  1399,  1400,  1401,  1369,
     550,  1370,  1738,  1322,   340,  1741,  1412,  2042,  1084,  1542,
    1746,  1989,  1085,  1498,    81,  1062,  1500,  1501,   452,  1431,
    1661,  1634,  1291,   480,  1568,  1635,   306,   306,  2268,  2269,
    1449,    10,    82,  1138,  1695,    10,  1456,  1458,  1459,  1461,
    1625,   103,  1464,  1466,   255,  1765,  1766,  2103,   255,  2105,
    2221,   115,  2222,  2205,  2270,  1208,  1208,  1208,  1583,  1584,
     464,  1406,  1407,  1408,  1409,  1208,  1410,   116,   246, -1507,
     247,   248,  1948,  1411,  1949,   117,  1323,  1585,  1586,  1625,
     119,   453,  1347,   125,  1066,  1408,  1409,  1208,  1410,  1208,
   -1507,  1208,   207,  1453,   129,  1208,  1208,  1208,  1208,  1208,
    1208,  1208,  1208,  1109,  1406,  1407,  1408,  1409,  1066,  1410,
     454,  1346,  1346,  1955,  1956,   130,    41,  1097,  1097,   139,
    2289,   341,  1406,  1407,  1408,  1409,  1962,  1410,  1963,  1999,
    1989,   140,   208,   342,  1736,  1848,  1664,  1664,   515,  1968,
    1888,  1446,  1889,  1346,   131,  1450,   331,   332,  1976,  1669,
    1670,   455,   141,  1462,  1463,  1725,  2194,   132,  1869,   133,
     142,  1347,   134,  1878,  1878,  2198,  2199,  1062,   136,  1406,
    1407,  1408,  1409,  1209,  1410,  2253,  2254,  1686, -1394, -1394,
   -1394, -1394,   209,   137,   144,  1784,  1432,   155,  1785,   157,
     537,   156,   171,   538,   172,   173,  1786,  1787,  1788,   299,
     186,  1687,  1688,  1689,  1690,  1691,  1692,  1694,   539,   516,
     224,   227,   228,  2218,   229,  2219,   231,   232,   540,   234,
     235,   343,   344,  1347,   237,   241,   242,  1433,   243,  2215,
     263,    41,  2271,  2272,   345,   277,   346,   143,   280,  1265,
    1265,  1265,   281,  1406,  1407,  1408,  1409,   286,  1410,  1207,
     517,  1740,   269,  1684,  1207,   287,   303,   518,  1864,   296,
    1208,  1208,  1208,  1208,  1208,  1208,  1208,  1097,   309,   519,
     311,  2101,   313,  2273,  2274,   320,   318,   319,  1894,   323,
     830,   361,   324,  1347,   362,   325,  1406,  1407,  1408,  1409,
    2111,  1410,  1066,  1865,  1743,   364,   368,   373,   306,   371,
    1406,  1407,  1408,  1409,   376,  1410,   377,  2120,  1748,  1840,
    1789,   306,  1406,  1407,  1408,  1409,   385,  1410,   402,   404,
    1904,   403,  1891,   412,   413,   430,  1977,   433,  1790,   520,
     435,   442,  1988,  1346,   443,   460,   306,   347,   475,   483,
    -374,  1346,   478,   485,   500,  1868,  1791,   488,  2151,  2152,
    1097,   530,  1868,  1281,  1282,  1283,  1284,  2154,  1615,   502,
     534,   348,  1910,   575,   583,  1911, -1393, -1393, -1393, -1393,
     588,  1209,  1209,  1209,   585,   592,   306,  1957,  1958,  1959,
    1960,  1209,   615,   594,   616,  1988,   618,   620,   623,  1346,
     190,   541,   521,   633,  1792,   522,   523,   697,   699,   707,
     715,   843,   714,  1209,   827,  1209,   852,  1209,  1664,  1454,
     867,  1209,  1209,  1209,  1209,  1209,  1209,  1209,  1209,   854,
     865,  1896,   542,  2093,  2093,  1406,  1407,  1408,  1409,   543,
    1410,   873,   906,  2100,   907,  -810,   950,   625,   985,  1961,
    2217,   988,  1793,   982,   999,  1964,  1907,  1002,  2220,  1005,
     544,  1000,  2262,  1347,  1347,  1008,  1009,  1011,  1017,  1023,
    1067,  1020,   524,  2223,  1026,  1027,  1053,  2006,  1083,  1168,
    2201,  1075,  1100,  1170,  2093,   545,  1176,  2093,  1189,  1195,
    1197,   546,  1210,  1214,   895,  1347,  1922,  1997,  1225,  1254,
    1232,  2242,  1249,  2243,  1256,  1794,  1234,  1259,  2051,  1261,
    2244,  1267,  1236,  1273,  1988,  1795,  1275,  1240,   547,  1241,
    1278,  1297,   195,  1242,  1243,   548,  1244,  1245,  1246,  1285,
    2251,  2252,  1301,  1316,  1317,  1319,   196,   197,   549,  1247,
   -1396,  1326,  1921,  1330,  1331,  1332,  1105,  1925,  1336,  1341,
    1343,  1138,  2003,  2167,   165,  1365,  1368,  1372,  2009,  1374,
    1375, -1520,   270,   550,  1387,   551,  2089,   331,   198,  1396,
    2090,  1413,  2093,   552,  1414,  2107,  1418,  1796,  1435,  1685,
    1448,  1455,  1469,  -215,  1470,  1528,  1209,  1209,  1209,  1209,
    1209,  1209,  1209,  2292,  1058,  1551,  1571,   480,  1589,  1566,
    1608,  1208,  1622,  1604,   553,  1630,  1208,   199,   200,  1637,
     201,  1638,  1650,  1645,  1662,  1667,  1701,  1410,  1702,   202,
    2093,  1725,  1705,  1438,  1737,  1739,  1706,  1742,  1707,  1747,
    1754,  1708,  1759,  1756,  1749,  1777,  1542,  1804,  1473,  1474,
    1475,  1709,  1800,  1802,  1808,   306,  1476,  1820,  1816,  1819,
    1842,  1845,  1846,  1857,  1858,  1859,  1872,  1873,   554,  1881,
    1892,  1883,   203,   204,  2139,  1676,  1905,  1934,  1953,  1954,
    1978,  1980,  1984,   205,  1986,  1994,  1991,  2012,  1850,  1305,
    2015,  2017,  2022,  1998,  1477,  1347,  2025,  2030,  2036,  2027,
    2038,  2034,  2048,  1347,  2246,  2052,  2053,  2248,  2055,  2104,
    2212,  2214,  2086,  2106,  2126,  2108,  2129,  2122,  2143,  2144,
    2124,  2145,  2163,  2165,  2172,  2042,  2191,  2190,  2184,  2192,
    2196,  2202,  2203,  2224,  2226,  2064,  2228,  2265,  2204,  2234,
    2206,  2240,  2241,  2249,  2288,  2259,  2276,   306,  2286,  2290,
      15,  1347,    28,   445,    74,   279,   193,   266,   206,  1632,
     300,   276,   928,   829,   607,  2138,   529,  -901,  1379,  1105,
    -901,  1666,  1660,  -901,  -901,  -901,   226,   840,  1380,  1178,
    2293,  1710,  2050,  2001,  2235,   302,  1711,   151,   153,   315,
     477,    67,  1478,  1479,   245,  1024,   712,   414,  1006,  1712,
     327,  1903,  2067,   328,  1394,   329,  1900,  1836,  2068,  1899,
    2069,  2070,  2213,  1192,  2071,  2072,  2073,  2256,  1019,  -901,
    2236,  1480,  1305,  2207,   636,  2074,  2278,  1481,   170,  1944,
     597,  1280,  2004,  1289,  1730,  1729,  2277,  2212,  1914,  1942,
    1967,  1573,  1569,  -901,  1811,  1974,  1588,  1713,  1993,  1821,
    1482,  2128,  2005,  1031,  1325,  1600,  1847,  1659,  1483,  -901,
    1629,  2021,  1484,   306,  1348,  2282,  2257,  2258,  2283,  1033,
    2261,  1349,   306,   207,  1150,  1879,  2170,  2263,  1880,  1363,
    1908,  2238,  2175,  2014,  2294,  2295,  2186,  1779,  2297,  1530,
    2298,  2287,  1485,  1486,  1340,  1714,  1030,  1863,  1487,   885,
    1211,   330,  1064,  2284,  1472,   426,  1013,  1602,  1488,   846,
    1296,  2279,  1215,   208,    76,  1489,  1758,  -901,  -901,  1715,
    1490,  1219,     0,  2303,     0,     0,     0,  1209,     0,     0,
       0,     0,  1209,     0,  2296,     0,   987,     0,  1491,     0,
       0,     0,  1062,  1716,     0,  -901,  -901,   480,  1717,     0,
       0,     0,  -901,     0,     0,  1034,  -901,     0,  1718,  1062,
       0,  2301,  1719,   209,     0,     0,     0,     0,     0,     0,
       0,     0,  1031,  2267,     0,  -901,     0,     0,     0,     0,
       0,     0,   401,  -901,     0,     0,  -901,  -901,  1033,     0,
       0,     0,     0,  -901,     0,   480,     0,  -901,     0,  -901,
       0,     0,  -901,     0,  -901,     0,     0,  1063,     0,  1037,
       0,     0,  1038,     0,     0,     0,     0,  -901,  -901,     0,
    1720,     0,     0,  -901,     0,     0,     0,     0,  -369,   637,
    1721,     0,     0,  -901,     0,  -901,     0,     0,     0,     0,
    -901,     0,     0,     0,     0,  1039,  1269,     0,     0,   638,
       0,   480,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   639,  -901,     0,   640,   641,   642,   643,   644,
     645,   646,     0,     0,  1034,     0,  -901,     0,     0,     0,
       0,     0,     0,     0,  1492,   449,     0,     0,     0,  1493,
    1494,  1495,  1496,     0,  1497,     0,  1498,  1499,     0,  1500,
    1501,     0,   647,  -901,   648,   649,   650,   651,   652,   653,
       0,     0,     0,  1040,     0,  1035,     0,     0,     0,     0,
       0,     0,  1036,     0,     0,     0,   450,     0,  1037,     0,
       0,  1038,     0,   451,     0,     0,     0,     0,     0,  -901,
       0,     0,     0,     0,     0,     0,   654,     0,  -901,  -901,
       0,     0,     0,     0,     0,     0,     0,     0,  1041,  1042,
       0,     0,     0,  -369,  1039,     0,     0,  -369,  -901,     0,
       0,     0,     0,     0,  -901,     0,     0,  1031,     0,     0,
       0,     0,     0,     0,  1043,     0,     0,     0,     0,     0,
       0,     0,  -901,  1033,     0, -1507,  -369,     0,     0,     0,
    -901,     0,  1044,  -901,     0,   452,     0,     0,     0,  -901,
    1045,  -369,     0,   303,  -901,  -901, -1507,     0,  1031,  -901,
       0,  -901,     0,  -901,  -901,  -901,     0,     0,     0,   655,
       0,     0,  1040,     0,  1033,     0,     0,     0,     0,     0,
       0,     0,    41,     0,     0,   656,     0,     0,     0,     0,
       0, -1544,     0,     0,     0,  -369,  -369,     0,     0,     0,
       0,     0,     0,  -369,   657,     0,     0,  -369,   453,     0,
       0,     0,  1118,     0,     0,     0,     0,  1041,  1042,  1034,
       0,   658,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   454,     0,     0,
       0,     0,     0,  1043,   659,  -590,   598,     0,  -630,     0,
    -630,     0,     0,  1118,     0,  -630,     0,     0,     0,   660,
    1034,  1044,     0,  -630,     0,     0,   661,     0,   662,  1045,
       0,  1119,   303,  1037,     0,  -369,  1038,     0,   455,  -369,
       0,   663,     0,     0,     0,     0,     0,     0,     0,     0,
    1270,   664,     0,     0,     0,     0,  -630,  -630,   665,     0,
    -369,     0,     0,     0,  -630,     0,     0,     0,     0,  1039,
       0,     0,  1063,     0,  1037,     0,  -630,  1038,     0,  -369,
    -630,     0,  -369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -630,     0,     0,   666,     0,   667,   668,   669,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1039,     0,     0,     0,     0,     0,  -630,     0,     0,     0,
       0,   670,     0,  -630,  -630,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1040,     0, -1544,
    -590,     0,     0,     0,  -590,     0,     0,   671,   672,   673,
       0,     0,     0,     0,  -630,     0,     0,     0,     0,     0,
     674,     0,     0,   675,   676,     0,  -630,     0,     0,     0,
    -630,     0,  -369,     0,     0,     0,     0,  -369,  1040,  -369,
    -369,     0,  1041,  1042,  -630,     0,     0,  -369,  -590,     0,
       0,  -630, -1563,  1030,  -630,  -630,   885,     0,     0,     0,
       0,     0,  -630,     0,     0,     0,     0,     0,  1043,  -630,
       0,  -630,     0,     0,  -630,     0,     0,  1227, -1009,     0,
       0,     0,     0,  1041,  1042,     0,  1044,     0,     0,     0,
       0, -1009,     0, -1563,  1045,     0,   190,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1043,
       0,  -630,     0, -1485,     0,  -630,     0,  -630,     0, -1009,
       0,     0,     0,     0,  -630,     0,     0,  1044,     0,  1031,
       0,     0, -1009,     0,     0,  1045,     0,   190,   303,     0,
       0,     0,     0,     0,     0,  1033,     0,  -630,   598,     0,
    -630,     0,  -630,     0,     0,     0,     0,  -630,     0,     0,
       0,     0,  -630,     0,     0,  -630,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -590,     0,     0,     0,
       0,     0,     0,  -630,     0,     0,     0,     0,     0,     0,
       0,     0,  -630,     0,     0,     0,     0,     0,  -630,  -630,
       0,     0,     0,     0,     0,  -630,  -630,     0,     0,     0,
    -630,     0,  -630,     0,     0,     0,     0,     0,  -630,     0,
    -630,     0,  -630,     0,     0,     0,     0,     0,     0,     0,
    -630,  1034,     0,     0,  -630,     0,     0,     0,     0,  -630,
       0,     0,   449,     0,     0,     0,     0,     0,  -630,     0,
    -630,  -630,     0,     0,  -630,     0,  -630,     0,  -630,     0,
       0,  -630,     0,     0,     0,  -630,  -630,     0,     0,     0,
       0,     0,  1035,   713,     0,     0,     0,  -630,     0,  1036,
   -1485,     0,  -630,   450,     0,  1037,     0,  -630,  1038,     0,
     451,     0,     0,     0,     0,     0,  -630,     0,  -630,     0,
       0,     0,     0,     0,     0,     0,     0,   599,  -630,     0,
    -630,     0,  -630,     0,  -630,  -630,  -630,  -630,     0,     0,
       0,  1039,     0,     0,  -630,     0,  -630,     0,     0,     0,
       0,     0,     0,  -630,     0,     0,  -630,  -630,     0,     0,
       0,     0,     0,     0,  -630,     0,     0,     0, -1485,     0,
       0,  -630,     0,  -630,     0,     0,  -630,     0,     0,     0,
       0,     0,   452,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1040,
       0,     0,     0,  -630,     0,     0,     0,  -630,     0,  -630,
       0,     0,     0,     0,     0, -1477,  -630,     0, -1477,     0,
       0, -1477, -1477, -1477,     0,     0,  1290,     0,     0,     0,
   -1477,     0,     0,     0,     0,   453,     0,     0,     0,  -630,
       0,     0,     0,     0,  1041,  1042,     0,     0,     0,     0,
       0,     0,     0,     0,  -630,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   454,     0,     0, -1477,     0,     0,
    1043,     0,     0,     0,     0,  -630,     0,     0,     0,     0,
       0,     0,     0,     0,  -630,     0,     0,     0,  1044,     0,
       0, -1477,     0,     0,     0,     0,  1045,  -630,     0,   303,
       0,     0,  -630,     0,  -630,   455,     0, -1477,     0,     0,
       0,  1030,  -630,     0,   885,     0,     0,  1507,  1508,  1509,
       0,     0,  -630,     0,     0,     0,     0,     0,     0,     0,
       0,  -630,     0,     0,     0,     0,     0,     0,     0,     0,
    -630,     0,  -630,  -630,     0,     0,  -630,     0,  -630,     0,
       0,     0,     0,  -630,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1510,     0, -1477, -1477,     0,     0,  -630,
       0,     0,     0,     0,  -630,     0,     0,     0,     0,  -630,
       0,     0,     0,     0,     0,     0,     0,  1031,     0,     0,
    -630,     0,     0, -1477, -1477,     0,     0,     0,     0,   599,
   -1477,     0,  -630,  1033, -1477,     0,  -630,  -630,  -630,  -630,
       0,     0,     0,     0,     0,     0,  -630,     0,     0,     0,
       0,     0,     0, -1477,     0,     0,     0,     0,     0,     0,
       0, -1477,     0,     0, -1477, -1477,     0,     0,     0,     0,
       0, -1477,     0,     0,     0, -1477,     0, -1477,     0,     0,
   -1477,     0, -1477,     0,     0,     0,     0,     0,     0,     0,
       0,  1478,  1479,     0,     0, -1477, -1477,     0,     0,     0,
       0, -1477,     0,     0,     0,     0,     0,     0,     0,     0,
       0, -1477,     0, -1477,     0,     0,     0,     0, -1477,  1034,
    1480,     0,     0,     0,     0,     0,  1481,     0,     0,     0,
     449,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0, -1477,     0,     0,     0,     0,     0,     0,     0,  1511,
       0,     0,     0,     0, -1477,     0,     0,  1483,     0,     0,
    1035,  1484,     0,     0,     0,     0,     0,  1036,     0,     0,
       0,   450,     0,  1037,     0,     0,  1038,     0,   451,     0,
       0, -1477,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1512,  1513,     0,     0,     0,     0,  1514,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1515,     0,  1039,
       0,     0,     0,     0,  1516,     0,     0, -1477,     0,     0,
       0,     0,     0,     0,     0,     0, -1477, -1477,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1517,     0,     0,
       0,     0,     0, -1477,     0,     0, -1477, -1477,     0,     0,
     452,     0, -1477,     0,     0,     0,   716,     0,   717,     0,
       0,     0,     0,   718,     0,     0,     0,     0,     0,     0,
   -1477,   719,     0,     0,     0,     0,     0,  1040, -1477,     0,
       0, -1477,     0,     0,     0,     0,     0, -1477,     0,     0,
       0,     0, -1477, -1477,     0,   885,     0, -1477,     0, -1477,
       0, -1477, -1477, -1477,   720,   721,     0,     0,     0,     0,
       0,     0,   722,   453,     0,     0,     0,     0,     0,     0,
       0,     0,  1041,  1042,   723,     0,     0,     0,   724,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     725,     0,   454,     0,     0,     0,     0,     0,  1043,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   726,     0,  1044,     0,  1031,     0,
       0,   727,   728,     0,  1045,     0,     0,   303,     0,     0,
       0,     0,     0,   455,  1033,     0,     0,     0,  1518,  1519,
       0,     0,     0,  1520,     0,  1498,     0,  1521,  1500,  1501,
       0,     0,   729,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   730,     0,     0,     0,   731,     0,
       0,     0,     0,     0,  -766,     0,     0,  -766,     0,   716,
       0,   717,   732,     0,     0,     0,   718,     0,     0,   733,
       0,     0,   734,   735,   719,     0,     0,     0,     0,     0,
     736,     0,     0,     0,     0,     0,     0,   737,     0,   738,
       0,     0,   739,     0,     0,     0,     0,   165,     0,     0,
    1034,     0,     0,     0,     0,     0,     0,   720,   721,     0,
       0,   449,     0,     0,     0,   722,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   723,     0,   740,
    -766,   724,     0,   741, -1489,   742,     0,     0,     0,     0,
       0,     0,   743,   725,     0,     0,  -766,     0,     0,     0,
       0,     0,   450,     0,  1037,     0,     0,  1038,     0,   451,
       0,     0,     0,  1030,     0,   744,   885,   726,     0,     0,
       0,     0,     0,     0,   727,   728,     0,     0,     0,     0,
     745,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1039,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   746,     0,     0,     0,   729,     0,     0,     0,     0,
     747,     0,     0,     0,     0,     0,     0,   730,     0,     0,
       0,   731,     0,   748,     0,     0,     0,     0,   749,     0,
     750,   452,  -766,     0,     0,   732,     0,     0,   751,  1031,
       0,     0,   733,  -766,     0,   734,   735,     0,   752,     0,
       0,  1032,     0,   736,     0,  1033,     0,   753,  1040,     0,
     737,     0,   738,     0,     0,   739,   754,     0,   755,   756,
       0,     0,   757,  -766,   758,     0,     0,     0,     0,   759,
    -766,     0,     0,     0,  -766,  1030,  -766,     0,   885,  -766,
       0,  -766,     0,     0,   453,   760,     0,     0,     0,     0,
     761,     0,   740,  1041,  1042,   762,   741,     0,   742,     0,
       0,     0,     0,     0,     0,   743,   763,     0,     0,     0,
       0,     0,  -766,   454,     0,     0,     0,  -766,   764,  1043,
       0,     0,   765,   766,   767,   768,     0,     0,   744,     0,
       0,  1034,   769,     0,     0,     0,     0,  1044,     0,     0,
       0,     0,   449,   745,     0,  1045,     0,     0,   303,     0,
       0,  1031,     0,  -766,   455,     0,     0,     0,     0,     0,
       0,     0,     0, -1489,   746,     0,     0,  1033,     0,     0,
       0,     0,  1035,   747,     0,     0,     0,     0,  1030,  1036,
    -766,   885,     0,   450,     0,  1037,   748,     0,  1038,     0,
     451,   749,     0,   750,     0,     0,     0,     0,     0,     0,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
    -766,   752,     0,     0,     0,     0,  -766,     0,  1633,     0,
     753,  1039,     0,     0,     0,  -766,  -766,     0,     0,   754,
       0,   755,   756,     0,     0,   757,     0,   758,     0,     0,
       0,     0,   759,     0,     0,  -766,     0,     0,     0,     0,
       0,  -766,     0,  1034,  1031,     0,  -766,     0,   760,     0,
       0,     0,   452,   761,   449,     0,     0,     0,   762,  -766,
    1033,     0,     0, -1489,     0,     0,     0,  -766,     0,   763,
    -766,     0,     0,     0,  1154,     0,  -766,     0,     0,  1040,
       0,   764,     0,     0,  1035,   765,   766,   767,   768,     0,
    1030,  1036,     0,   885,     0,   450,     0,  1037,     0,     0,
    1038,     0,   451,     0,  1030,     0,     0,   885,     0,     0,
       0,     0,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,     0,  1041,  1042,     0,     0,     0,     0,
       0,     0,     0,  1039,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   454,     0,  1034,     0,     0,     0,
    1043,     0,     0,     0,     0,     0,     0,   449,     0,     0,
       0,     0,     0,     0,     0,     0,  1031,     0,  1044,     0,
       0,     0,  1362,     0,   452,     0,  1045,     0,  1112,   303,
    1031,     0,  1033,     0,     0,   455,     0,  1035,     0,     0,
       0,     0,     0,     0,  1036,     0,  1033,     0,   450,  1030,
    1037,  1040,   885,  1038,     0,   451,     0,     0,     0,     0,
    1154,     0,     0,  1030,     0,     0,   885,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1039,   453,     0,     0,
       0,     0,     0,     0,     0,     0,  1041,  1042,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1634,     0,     0,   454,  1635,  1034,     0,
       0,     0,  1043,     0,     0,  1031,     0,   452,     0,   449,
       0,     0,  1034,     0,     0,     0,     0,  1158,     0,  1031,
    1044,  1033,     0,   449,     0,     0,     0,     0,  1045,     0,
       0,   303,     0,     0,  1040,  1033,     0,   455,     0,  1035,
       0,     0,     0,     0,     0,     0,  1036,     0,     0,     0,
     450,     0,  1037,  1035,     0,  1038,     0,   451,     0,     0,
    1036,     0,     0,     0,   450,  1030,  1037,     0,   885,  1038,
     453,   451,     0,     0,     0,     0,     0,     0,     0,  1041,
    1042,     0,     0,     0,     0,     0,     0,     0,  1039,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   454,
       0,     0,  1039,     0,     0,  1043,     0,  1034,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   449,     0,
       0,  1034,     0,  1044,     0,     0,     0,     0,     0,   452,
       0,  1045,   449,     0,   303,     0,     0,     0,     0,     0,
     455,  1031,     0,   452,     0,     0,     0,     0,  1035,     0,
       0,     0,     0,     0,     0,  1036,  1040,  1033,     0,   450,
       0,  1037,  1035,     0,  1038,     0,   451,     0,  1030,  1036,
    1040,   885,     0,   450,     0,  1037,     0,     0,  1038,     0,
     451,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   453,     0,     0,     0,     0,  1039,     0,     0,
       0,  1041,  1042,     0,     0,     0,   453,     0,     0,     0,
       0,  1039,     0,     0,     0,  1041,  1042,     0,     0,     0,
       0,   454,     0,     0,     0,     0,     0,  1043,     0,     0,
       0,     0,     0,     0,     0,   454,     0,     0,   452,  1367,
       0,  1043,     0,  1034,  1031,  1044,     0,     0,     0,     0,
       0,     0,   452,  1045,   449,     0,   303,     0,     0,  1044,
    1033,     0,   455,     0,     0,  1040,     0,  1045,     0,     0,
     303,     0,     0,     0,     0,     0,   455,     0,     0,  1040,
    1030,     0,     0,   885,  1035,     0,     0,     0,     0,     0,
       0,  1036,     0,     0,     0,   450,     0,  1037,     0,     0,
    1038,   453,   451,     0,     0,     0,     0,     0,     0,     0,
    1041,  1042,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,     0,  1041,  1042,     0,     0,     0,     0,
     454,     0,     0,  1039,     0,     0,  1043,     0,     0,     0,
    1251,     0,     0,     0,   454,     0,  1034,     0,     0,     0,
    1043,     0,     0,     0,  1044,     0,  1031,   449,     0,     0,
       0,     0,  1045,     0,     0,   303,     0,     0,  1044,     0,
       0,   455,  1033,     0,   452,     0,  1045,     0,     0,   303,
       0,     0,     0,  1030,     0,   455,   885,  1035,     0,     0,
       0,     0,     0,     0,  1036,     0,     0,     0,   450,     0,
    1037,  1040,     0,  1038,     0,   451,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1039,   453,     0,     0,
       0,     0,     0,     0,     0,     0,  1041,  1042,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1034,  1031,
       0,     0,     0,     0,     0,  1030,   454,     0,   885,   449,
       0,     0,  1043,     0,     0,  1033,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1044,     0,     0,     0,     0,     0,     0,     0,  1045,  1035,
       0,   303,     0,     0,  1040,     0,  1036,   455,     0,     0,
     450,     0,  1037,     0,     0,  1038,     0,   451,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1642,     0,     0,     0,     0,     0,
     453,  1031,     0,     0,     0,     0,     0,     0,  1039,  1041,
    1042,     0,     0,     0,     0,     0,     0,  1033,     0,     0,
       0,  1034,     0,     0,     0,     0,     0,     0,     0,   454,
       0,     0,   449,     0,     0,  1043,     0,     0,     0,     0,
       0,     0,   640,   641,   642,   643,   644,   645,   646,   452,
       0,     0,     0,  1044,     0,     0,     0,     0,     0,     0,
       0,  1045,  1035,     0,   303,     0,     0,     0,     0,  1036,
     455,  2056,  2057,   450,     0,  1037,  1040,     0,  1038,   647,
     451,   648,   649,   650,   651,   652,   653,     0,   640,   641,
     642,   643,   644,   645,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1034,     0,     0,     0,  1749,     0,     0,
       0,  1039,   453,     0,   449,     0,     0,     0,     0,     0,
       0,  1041,  1042,   654,     0,   647,     0,   648,   649,   650,
     651,   652,   653,     0,     0,     0,     0,     0,     0,     0,
       0,   454,     0,     0,  1035,     0,     0,  1043,     0,     0,
       0,  1036,   452,     0,     0,  1076,     0,  1037,     0,     0,
    1038,     0,   451,     0,     0,  1044,     0,     0,     0,   654,
       0,     0,     0,  1045,     0,     0,   303,     0,     0,  1040,
       0,     0,   455,     0,     0,  1031,     0,     0,     0,     0,
       0,     0,     0,  1039,     0,     0,     0,  2058,     0,     0,
       0,  1033,     0,     0,     0,     0,   655,     0,     0,     0,
       0,     0,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,   656,     0,  1041,  1042,     0,     0,     0,     0,
       0,     0,     0,     0,   452,  1031,     0,     0,  2059,  2060,
       0,     0,     0,     0,   454,     0,     0,     0,     0,     0,
    1043,  1033,   655,     0,     0,     0,     0,     0,   658,     0,
       0,  1040,     0,  2061,     0,     0,     0,     0,  1044,     0,
       0,     0,     0,     0,     0,     0,  1045,     0,     0,   303,
       0,   659,     0,     0,     0,   455,     0,  1034,     0,     0,
       0,     0,     0,     0,     0,     0,   660,   453,   449,     0,
       0,     0,     0,   661,   658,   662,  1041,  1042,  2062,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   663,     0,
       0,     0,     0,     0,     0,     0,   454,     0,  1199,     0,
       0,     0,  1043,     0,     0,  1036,     0,  1034,     0,   450,
    1031,  1037,   660,     0,  1038,     0,   451,     0,   449,     0,
    1044,   662,     0,     0,     0,     0,  1033,     0,  1045,     0,
       0,   303,     0,  1031,   663,     0,     0,   455,     0,     0,
       0,     0,   666,     0,   667,   668,   669,  1039,  1199,  1033,
       0,  2063,     0,     0,     0,  1036,     0,     0,     0,   450,
       0,  1037,     0,     0,  1038,  2064,   451,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   452,  2065,
     667,   668,   669,     0,   671,   672,   673,  1039,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   674,     0,     0,
     675,   676,  1034,     0,     0,  1040,     0,     0,     0,     0,
       0,     0,     0,   449,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  2066,  1034,     0,     0,   452,     0,
     671,   672,   673,     0,     0,     0,   449,     0,     0,     0,
       0,   453,     0,     0,     0,     0,     0,     0,     0,     0,
    1041,  1042,     0,     0,   450,  1040,  1037,     0,     0,  1038,
       0,   451,     0,     0,     0,     0,  1199,     0,     0,     0,
     454,     0,     0,  1036,     0,     0,  1043,   450,     0,  1037,
       0,     0,  1038,     0,   451,     0,     0,     0,     0,     0,
       0,   453,  1039,     0,  1044,     0,     0,     0,     0,     0,
    1041,  1042,  1045,     0,     0,   303,     0,     0,     0,     0,
       0,   455,     0,     0,     0,  1039,  1200,  1201,     0,     0,
     454,     0,     0,     0,  1693,  1202,  1043,     0,     0,     0,
       0,     0,     0,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1044,     0,     0,     0,     0,     0,
       0,     0,  1045,     0,     0,   303,   452,     0,     0,     0,
    1040,   455,     0,     0,     0,     0,  1200,  1201,     0,     0,
       0,     0,     0,     0,  1920,  1202,     0,     0,     0,     0,
       0,     0,     0,  1040,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   453,     0,     0,     0,
       0,     0,     0,     0,     0,  1041,  1042,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   453,
       0,     0,     0,     0,     0,   454,     0,     0,  1041,  1042,
       0,  1043,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   454,  1044,
       0,     0,     0,     0,  1043,     0,     0,  1045,     0,     0,
     303,     0,     0,     0,     0,     0,   455,     0,     0,     0,
       0,     0,  1044,     0,     0,     0,     0,     0,     0,     0,
    1045,     0,     0,   303,     0,     0,     0,     0,     0,   455,
       0,     0,     0,     0,  1200,  1201,     0,     0,     0,     0,
       0,     0,     0,  1202
};

static const yytype_int16 yycheck[] =
{
      84,   176,   230,   132,   424,   583,   952,   698,  1075,   700,
     547,   548,   335,   734,   916,   763,  1328,   472,  1147,  1258,
    1735,   196,   197,  1258,   912,  1258,   201,   202,   203,   204,
     205,   664,   207,  1045,   209,  1258,   228,  1258,   230,   231,
    1291,   264,   234,   499,   930,   886,   887,   911,  1154,  1068,
    1171,   705,     6,   381,  1095,  1137,  1251,   943,   922,  1438,
    1439,    13,  1103,    23,   193,    70,    17,    48,  1168,  1332,
      70,     9,    22,    49,    56,    54,   160,    17,   615,   254,
     122,   256,     9,    70,     6,    70,    70,     6,   263,  1540,
       0,    56,    85,  1288,   182,     9,   271,   289,   841,  1228,
       6,   293,    96,   295,    73,   214,    22,  1558,  1149,    56,
     285,   286,   304,   288,   118,   307,     6,   170,   223,   294,
      72,  1222,   171,  1884,  1319,    92,    70,     9,   456,   168,
      66,    63,    28,    30,   236,    40,    88,   360,     9,    96,
     171,  1282,  1283,  1284,   367,  1227,   118,    49,   168,   231,
    1429,    48,   234,    66,  1869,  1606,    32,  1436,   114,   214,
     187,   139,   187,   218,   339,    49,   149,   342,   343,  1188,
     345,   207,  1367,   236,   402,   198,   404,    49,   300,   236,
     372,   224,    21,     9,    86,    56,   361,   878,    61,  1030,
     382,    49,     9,  1872,  1035,  1646,   150,   389,   421,    37,
     126,   892,   530,   378,   200,   428,   236,   289,   400,  1131,
     402,   293,   404,   295,  1314,    88,    89,   240,   441,   115,
     107,   214,   304,     0,  1306,   307,   122,   279,   312,   121,
     260,   121,   236,   425,   325,   427,   175,   260,    49,    56,
     121,   932,   465,   468,    86,  1191,   188,   475,   423,   240,
     706,   919,   441,  1094,   214,   478,   431,   195,     8,  1145,
     236,   459,   187,  1002,   236,   171,  1372,  1388,   434,   260,
     459,   459,   214,   183,  1469,   113,     9,   174,  1021,  1022,
     236,   949,   474,   475,   476,    35,  1368,   150,   164,   285,
     372,   248,   349,   187,   248,     9,  1396,   233,   236,   491,
     382,  1752,   173,   346,     9,   243,   411,   389,   531,  1350,
     485,   536,  1991,   195,   341,   354,  1417,  1336,  1418,   441,
    1571,   405,   236,   348,   408,  1422,   549,   310,   380,  1020,
     505,   506,  1429,   311,   354,   251,    50,   263,   210,  1436,
     442,   214,    63,   122,   435,   427,  1428,   236,   305,  1251,
     459,    56,   223,   322,   236,   360,   173,   262,  1199,   676,
     535,   243,   585,   288,   331,   301,   405,   590,   244,  1023,
     545,   459,    23,   360,   321,   360,   360,   315,   195,   442,
     218,   573,  1015,  1016,   280,  2146,  1288,   442,   301,   442,
     329,   123,   474,   442,   476,   251,  1659,   572,   434,   441,
     442,   311,  1830,   595,   596,   580,   223,  1271,  1549,   575,
     236,   442,  1300,  1335,   437,   993,   360,  1319,  1547,   236,
     293,   403,   551,   325,  1703,  1531,   243,   394,   409,  1470,
    1316,   363,  1296,   315,   350,   117,   659,   406,   442,   614,
     378,   325,  1330,   436,   619,   445,   621,   341,   442,   624,
     441,   626,    70,   628,   629,   434,   461,   341,   633,   442,
     400,  1200,  1201,  1202,    70,  1367,   442,   325,  1919,   407,
     442,  1210,   448,   665,   461,  1575,   461,   461,   443,   187,
    1223,   195,   442,   325,   403,   442,   442,   438,   442,  1377,
     195,   573,   236,  1232,   350,  1234,   378,  1236,   315,   674,
     675,  1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,   459,
     448,   225,   409,   403,   325,   442,  1598,   461,   223,  1970,
     442,  1972,   236,   442,   245,   407,   364,   260,   442,   243,
     341,   236,   424,   435,   424,   441,   442,   248,   243,  1660,
     411,   240,  1609,   424,   441,   441,   245,  1859,  1630,  2000,
    2265,   435,   442,   442,   441,   442,  1807,   885,   510,  1987,
     442,   378,  1990,   214,  1605,   404,   448,  1469,  1024,   408,
    1676,    31,    70,   236,    28,   459,   705,   435,   245,   402,
    2031,  2032,   300,   187,   316,   318,   459,   223,   139,   310,
     407,   245,   909,   435,   411,   187,   913,   376,  1231,   322,
     269,   315,   919,   459,   163,  1238,  1703,   924,   925,  1804,
     315,  1806,   240,   930,   931,   932,   269,   180,   317,   236,
     937,  1132,   200,   305,   435,   442,   943,    28,   180,   368,
     183,   448,   364,   156,   120,   952,   953,   358,   955,   462,
     348,  1529,   363,   310,  1839,   837,   166,   213,   459,   841,
     110,   245,   844,   845,    27,  1761,   310,   832,  1169,   851,
      30,   115,   441,   442,   378,  1298,  1405,  1406,  1407,  1408,
    1409,  1410,  1411,   378,   156,   118,  1871,  2056,   853,   418,
      25,   460,   905,   406,   236,  1002,  1340,   207,    30,  1017,
    1344,   358,   403,   407,  1415,   215,    66,   240,   442,   277,
     266,   910,   407,   266,   358,  2133,   411,  1726,  1727,  1728,
     919,   920,   411,  1754,   115,  1032,   310,   260,  1604,   347,
     912,   442,  1734,   441,   442,   934,   935,  1739,  1740,   279,
     289,  1743,  1744,  1745,   448,  1747,  1748,   442,  1820,   948,
     949,   933,   360,   448,   348,   937,   240,   939,   202,   941,
     942,  1068,   944,   167,   360,   837,   348,  1478,   311,   841,
     311,   284,   844,   845,   358,  1398,   260,   959,   288,   851,
    2221,  2222,   441,   426,   188,   411,   236,  1000,  1095,   442,
     972,   324,  1882,   128,   126,  2024,  1103,  1104,   441,  2024,
    1538,  2024,   459,   236,  1876,   368,  1837,   989,  2036,  1008,
    2038,  2024,   284,  2024,  1121,   459,  1912,  1913,   937,   236,
     180,  1444,   236,  1005,   459,   990,  1957,  1958,  1959,  1960,
    1137,   248,   372,    12,   126,   442,    15,    16,  1145,  1021,
    1022,   448,  1149,   917,   195,   453,   454,   455,   456,  1048,
     458,  1158,   236,   461,  1161,   418,   195,   453,   454,   455,
     456,   195,   458,    11,   938,   461,   940,   167,  1579,  1965,
    1966,   945,   360,   233,   258,   459,   293,  1588,   418,  1078,
     954,  1188,   123,   416,  1191,   236,   227,   228,   188,   156,
     157,   211,   243,  1200,  1201,  1202,   102,   236,   312,   236,
     972,  2203,   236,  1210,   243,  2284,    83,   107,   441,   243,
     198,   199,  1804,  1112,  1806,   299,   236,   989,   185,   368,
    1227,   213,  2301,    71,   191,  1232,   167,  1234,   251,  1236,
    2049,    11,   442,  1240,  1241,  1242,  1243,  1244,  1245,  1246,
    1247,   301,   227,   228,  2172,    25,    26,  1839,   459,  1021,
    1022,  1133,   240,  1913,  1808,   222,  1155,   441,   442,   226,
     120,  1160,   275,   368,   315,   453,   454,   455,   456,   418,
     458,   263,   260,   461,   266,   175,   315,    57,  1002,  1871,
    1193,   315,   356,  2040,  1103,  1911,  2288,   372,   188,  1624,
      25,    26,   319,  1175,     9,  1069,   319,    12,   321,  1306,
      15,    16,  1121,  1857,  1639,  1965,  1966,  1736,   423,  1316,
    1633,   169,  1741,   418,   258,  1180,    96,    97,   202,    99,
    1036,   436,  1329,   459,   337,   442,     8,   378,   108,  1336,
     368,   448,    67,   418,    69,  1961,  1343,  1002,  1964,   378,
     319,  1223,   321,  1350,   378,  1061,   198,   374,   375,   233,
     208,   307,  1251,    35,   206,  1362,   407,   459,   216,  1258,
     304,  1368,   296,   297,   377,   240,   379,  1374,   103,   104,
     105,   151,   152,   225,   253,   254,   310,   154,   195,  2136,
     418,  1394,   162,     1,  1391,   260,   240,  2023,  1287,  1288,
    1572,   442,   459,   272,   273,   459,  1578,   448,  1405,  1406,
    1407,  1408,  1409,  1410,  1411,   356,   260,   240,   449,   448,
     451,   452,   159,   459,   448,   150,   163,   152,  1300,   236,
    1319,  1428,   356,  2049,   120,   160,   243,   260,   338,    47,
     311,   283,   284,   343,   166,   319,   213,   135,  1819,   291,
     292,  1448,   326,   941,    62,   297,   298,  1266,  1330,   437,
     278,  1223,   280,   441,  1681,  1354,  1601,   237,   459,  1358,
    1359,   959,  1361,  1470,   449,   538,   451,   452,  1367,   310,
     543,  2097,  1137,  2230,   459,   207,  1200,  1201,  1202,   319,
     364,   321,  1456,   215,   102,  1459,  1210,   264,   404,   266,
    1464,  1872,   408,   460,   356,  1377,   463,   464,   315,   234,
    1382,   404,   438,  1277,   440,   408,  1388,  1389,    90,    91,
    1234,     2,   444,   248,  1413,     6,  1240,  1241,  1242,  1243,
    1339,   169,  1246,  1247,   166,  1532,  1533,  1938,   170,  1940,
    2122,   459,  2124,  2064,   116,  1200,  1201,  1202,   253,   254,
     392,   453,   454,   455,   456,  1210,   458,   356,   449,   167,
     451,   452,   265,   465,   267,   239,   288,   272,   273,  1378,
     442,   378,  1227,   341,   913,   455,   456,  1232,   458,  1234,
     188,  1236,   352,  1238,   459,  1240,  1241,  1242,  1243,  1244,
    1245,  1246,  1247,   932,   453,   454,   455,   456,   937,   458,
     407,  1598,  1599,  1775,  1776,   459,   214,  1604,  1605,    60,
    2236,   219,   453,   454,   455,   456,   153,   458,   155,  1836,
    1991,    72,   392,   231,   465,  1622,  1388,  1389,   124,  1801,
     319,  1232,   321,  1630,   459,  1236,   227,   228,  1810,   233,
     234,   448,    93,  1244,  1245,   270,   271,   459,  1645,   459,
     101,  1306,   359,  1650,  1651,   270,   271,  1529,   459,   453,
     454,   455,   456,  1002,   458,   233,   234,   461,   427,   428,
     429,   430,   442,   216,   405,    33,   401,   459,    36,   235,
       1,   356,   442,     4,   459,   459,    44,    45,    46,   459,
     182,  1405,  1406,  1407,  1408,  1409,  1410,  1411,    19,   195,
     283,   459,   280,  2104,   280,  2106,   403,   277,    29,   403,
     395,   319,   320,  1368,    24,   459,   356,   442,   332,  2090,
     372,   214,   294,   295,   332,   459,   334,   178,   107,  1726,
    1727,  1728,   442,   453,   454,   455,   456,   384,   458,  1736,
     236,   461,    63,  1398,  1741,   367,   442,   243,  1637,    56,
    1405,  1406,  1407,  1408,  1409,  1410,  1411,  1754,   236,   255,
      85,  1933,   175,   335,   336,   351,   459,   356,  1671,   459,
     612,   372,   459,  1428,   283,   459,   453,   454,   455,   456,
    1952,   458,  1121,  1638,   461,   109,   236,   211,  1660,   363,
     453,   454,   455,   456,   459,   458,   249,  1969,   461,  1608,
     158,  1673,   453,   454,   455,   456,    68,   458,   280,   280,
     461,   410,  1667,   459,   356,   249,  1813,    23,   176,   315,
     277,   442,  1819,  1820,   459,   236,  1698,   435,   280,   459,
      84,  1828,   301,    84,   436,  1644,   194,   442,  2010,  2011,
    1837,   400,  1651,   427,   428,   429,   430,  2019,  1845,   442,
     384,   459,  1707,   317,   435,  1710,   427,   428,   429,   430,
     234,  1200,  1201,  1202,   403,   200,  1738,  1780,  1781,  1782,
    1783,  1210,   319,   442,   403,  1872,   364,   218,    55,  1876,
     441,   202,   378,   384,   242,   381,   382,    27,   188,   372,
     356,   364,   459,  1232,   459,  1234,   442,  1236,  1660,  1238,
     346,  1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,   225,
     438,  1673,   233,  1910,  1911,   453,   454,   455,   456,   240,
     458,   214,   109,   461,   236,   405,   306,    37,   287,  1784,
    2102,   281,   290,   381,   448,  1790,  1698,   462,  2110,   400,
     261,   403,  2200,  1598,  1599,   180,    27,   362,   466,   174,
       9,   400,   448,  2125,   459,   459,   442,  1846,   236,   107,
    2060,   405,   136,    17,  1961,   286,   441,  1964,   172,   188,
     402,   292,   462,   236,    30,  1630,  1738,  1832,   127,   205,
     462,  2153,   128,  2155,    56,   343,   462,   403,  1891,   434,
    2162,   130,   462,   348,  1991,   353,   319,   462,   319,   462,
     132,   100,    11,   462,   462,   326,   462,   462,   462,   133,
    2182,  2183,   399,   403,   137,    49,    25,    26,   339,   462,
     402,   138,  1736,   400,   402,   397,  2023,  1741,   180,   143,
     436,   248,  1841,  2030,    49,   146,   180,   112,  1847,   333,
     380,   442,   363,   364,   122,   366,  1901,   227,    57,   114,
    1905,    49,  2049,   374,   365,  1944,   114,   415,   441,  1398,
     403,   442,   187,   384,   341,   309,  1405,  1406,  1407,  1408,
    1409,  1410,  1411,  2245,   236,   163,   438,  1841,   171,   134,
     366,  1736,   210,   403,   405,   403,  1741,    96,    97,    49,
      99,   218,   210,   180,   280,   193,   447,   458,   446,   108,
    2097,   270,   218,   233,   461,   461,    31,   461,    33,   461,
     341,    36,   403,   398,   373,   131,   266,   187,    12,    13,
      14,    46,   269,   265,   400,  1997,    20,    49,   331,   403,
     138,     8,   180,   438,   126,   438,   403,     9,   459,   307,
     173,   308,   151,   152,  1999,   202,   404,   268,   329,   344,
     125,   435,   110,   162,   435,   175,    49,   141,   263,  2156,
     142,   240,   262,   285,    58,  1820,   144,   300,   113,   289,
     394,   284,     7,  1828,  2171,   116,   220,  2174,    67,   156,
    2083,  2089,   442,   156,   136,   129,   102,   328,    49,   240,
     328,   218,   145,   267,    95,   264,    91,  2052,   148,   221,
     203,   192,   438,    49,   406,   390,   307,  2204,  2063,   344,
    2065,   240,   240,   147,   423,   174,   293,  2089,   307,   140,
       6,  1876,    22,   366,    54,   193,   132,   178,   237,  1351,
     211,   188,     1,   609,   526,  1997,   456,     6,  1170,  2236,
       9,  1389,  1380,    12,    13,    14,   136,   622,  1171,   978,
    2247,   176,  1887,  1840,  2143,   227,   181,   109,   113,   239,
     405,    49,   156,   157,   160,   904,   596,   330,   877,   194,
     249,  1682,  1895,   249,  1194,   249,  1680,  1603,  1895,  1678,
    1895,  1895,  2087,   992,  1895,  1895,  1895,  2190,   892,    58,
    2145,   185,  2289,  2079,   577,  1895,  2210,   191,   124,  1763,
     508,  1078,  1844,  1083,  1441,  1440,  2209,  2210,  1722,  1761,
    1797,  1296,  1293,    82,  1571,  1807,  1303,   242,  1828,  1596,
     214,  1981,  1845,    82,  1121,  1309,  1620,  1378,   222,    98,
    1344,  1861,   226,  2205,  1143,  2224,  2191,  2192,  2227,    98,
    2195,  1145,  2214,   352,   944,  1651,  2034,  2202,  1651,  1155,
    1700,  2146,  2039,  1852,  2257,  2258,  2045,  1545,  2261,  1260,
    2263,  2233,   256,   257,  1133,   290,     6,  1633,   262,     9,
    1003,   249,   913,  2228,  1256,   341,   884,  1314,   272,   631,
    1086,  2216,  1011,   392,    57,   279,  1484,   156,   157,   314,
     284,  1018,    -1,  2296,    -1,    -1,    -1,  1736,    -1,    -1,
      -1,    -1,  1741,    -1,  2259,    -1,   848,    -1,   302,    -1,
      -1,    -1,  2284,   338,    -1,   184,   185,  2181,   343,    -1,
      -1,    -1,   191,    -1,    -1,   184,   195,    -1,   353,  2301,
      -1,  2286,   357,   442,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,  2205,    -1,   214,    -1,    -1,    -1,    -1,
      -1,    -1,  2214,   222,    -1,    -1,   225,   226,    98,    -1,
      -1,    -1,    -1,   232,    -1,  2229,    -1,   236,    -1,   238,
      -1,    -1,   241,    -1,   243,    -1,    -1,   236,    -1,   238,
      -1,    -1,   241,    -1,    -1,    -1,    -1,   256,   257,    -1,
     415,    -1,    -1,   262,    -1,    -1,    -1,    -1,     0,     1,
     425,    -1,    -1,   272,    -1,   274,    -1,    -1,    -1,    -1,
     279,    -1,    -1,    -1,    -1,   274,   156,    -1,    -1,    21,
      -1,  2285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,   302,    -1,    37,    38,    39,    40,    41,
      42,    43,    -1,    -1,   184,    -1,   315,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   448,   195,    -1,    -1,    -1,   453,
     454,   455,   456,    -1,   458,    -1,   460,   461,    -1,   463,
     464,    -1,    74,   342,    76,    77,    78,    79,    80,    81,
      -1,    -1,    -1,   342,    -1,   225,    -1,    -1,    -1,    -1,
      -1,    -1,   232,    -1,    -1,    -1,   236,    -1,   238,    -1,
      -1,   241,    -1,   243,    -1,    -1,    -1,    -1,    -1,   378,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,   387,   388,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,   388,
      -1,    -1,    -1,   135,   274,    -1,    -1,   139,   407,    -1,
      -1,    -1,    -1,    -1,   413,    -1,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,   413,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   431,    98,    -1,   167,   168,    -1,    -1,    -1,
     439,    -1,   431,   442,    -1,   315,    -1,    -1,    -1,   448,
     439,   183,    -1,   442,   453,   454,   188,    -1,    82,   458,
      -1,   460,    -1,   462,   463,   464,    -1,    -1,    -1,   201,
      -1,    -1,   342,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,   223,    -1,    -1,    -1,   227,   228,    -1,    -1,    -1,
      -1,    -1,    -1,   235,   236,    -1,    -1,   239,   378,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,   387,   388,   184,
      -1,   253,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   407,    -1,    -1,
      -1,    -1,    -1,   413,   276,     0,     1,    -1,     3,    -1,
       5,    -1,    -1,   177,    -1,    10,    -1,    -1,    -1,   291,
     184,   431,    -1,    18,    -1,    -1,   298,    -1,   300,   439,
      -1,   236,   442,   238,    -1,   307,   241,    -1,   448,   311,
      -1,   313,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     460,   323,    -1,    -1,    -1,    -1,    51,    52,   330,    -1,
     332,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,   274,
      -1,    -1,   236,    -1,   238,    -1,    71,   241,    -1,   351,
      75,    -1,   354,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,   367,    -1,   369,   370,   371,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     274,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,   393,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   342,    -1,   411,
     135,    -1,    -1,    -1,   139,    -1,    -1,   419,   420,   421,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
     432,    -1,    -1,   435,   436,    -1,   161,    -1,    -1,    -1,
     165,    -1,   444,    -1,    -1,    -1,    -1,   449,   342,   451,
     452,    -1,   387,   388,   179,    -1,    -1,   459,   183,    -1,
      -1,   186,   397,     6,   189,   190,     9,    -1,    -1,    -1,
      -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,   413,   204,
      -1,   206,    -1,    -1,   209,    -1,    -1,    30,   423,    -1,
      -1,    -1,    -1,   387,   388,    -1,   431,    -1,    -1,    -1,
      -1,   436,    -1,   397,   439,    -1,   441,   442,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   413,
      -1,   246,    -1,    66,    -1,   250,    -1,   252,    -1,   423,
      -1,    -1,    -1,    -1,   259,    -1,    -1,   431,    -1,    82,
      -1,    -1,   436,    -1,    -1,   439,    -1,   441,   442,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   282,     1,    -1,
       3,    -1,     5,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,   297,    -1,    -1,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   311,    -1,    -1,    -1,
      -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   327,    -1,    -1,    -1,    -1,    -1,    51,    52,
      -1,    -1,    -1,    -1,    -1,   340,    59,    -1,    -1,    -1,
     345,    -1,   347,    -1,    -1,    -1,    -1,    -1,    71,    -1,
     355,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     365,   184,    -1,    -1,    87,    -1,    -1,    -1,    -1,   374,
      -1,    -1,   195,    -1,    -1,    -1,    -1,    -1,   383,    -1,
     385,   386,    -1,    -1,   389,    -1,   391,    -1,   111,    -1,
      -1,   396,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,   225,   126,    -1,    -1,    -1,   412,    -1,   232,
     233,    -1,   417,   236,    -1,   238,    -1,   422,   241,    -1,
     243,    -1,    -1,    -1,    -1,    -1,   149,    -1,   433,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   442,   161,    -1,
     445,    -1,   165,    -1,   449,   450,   451,   452,    -1,    -1,
      -1,   274,    -1,    -1,   459,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,   189,   190,    -1,    -1,
      -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,   301,    -1,
      -1,   204,    -1,   206,    -1,    -1,   209,    -1,    -1,    -1,
      -1,    -1,   315,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   342,
      -1,    -1,    -1,   246,    -1,    -1,    -1,   250,    -1,   252,
      -1,    -1,    -1,    -1,    -1,     6,   259,    -1,     9,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,   378,    -1,    -1,    -1,   282,
      -1,    -1,    -1,    -1,   387,   388,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   297,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   407,    -1,    -1,    58,    -1,    -1,
     413,    -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   327,    -1,    -1,    -1,   431,    -1,
      -1,    82,    -1,    -1,    -1,    -1,   439,   340,    -1,   442,
      -1,    -1,   345,    -1,   347,   448,    -1,    98,    -1,    -1,
      -1,     6,   355,    -1,     9,    -1,    -1,    12,    13,    14,
      -1,    -1,   365,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   374,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     383,    -1,   385,   386,    -1,    -1,   389,    -1,   391,    -1,
      -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    -1,   156,   157,    -1,    -1,   412,
      -1,    -1,    -1,    -1,   417,    -1,    -1,    -1,    -1,   422,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,
     433,    -1,    -1,   184,   185,    -1,    -1,    -1,    -1,   442,
     191,    -1,   445,    98,   195,    -1,   449,   450,   451,   452,
      -1,    -1,    -1,    -1,    -1,    -1,   459,    -1,    -1,    -1,
      -1,    -1,    -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   222,    -1,    -1,   225,   226,    -1,    -1,    -1,    -1,
      -1,   232,    -1,    -1,    -1,   236,    -1,   238,    -1,    -1,
     241,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,   157,    -1,    -1,   256,   257,    -1,    -1,    -1,
      -1,   262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   272,    -1,   274,    -1,    -1,    -1,    -1,   279,   184,
     185,    -1,    -1,    -1,    -1,    -1,   191,    -1,    -1,    -1,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   302,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
      -1,    -1,    -1,    -1,   315,    -1,    -1,   222,    -1,    -1,
     225,   226,    -1,    -1,    -1,    -1,    -1,   232,    -1,    -1,
      -1,   236,    -1,   238,    -1,    -1,   241,    -1,   243,    -1,
      -1,   342,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   256,   257,    -1,    -1,    -1,    -1,   262,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   272,    -1,   274,
      -1,    -1,    -1,    -1,   279,    -1,    -1,   378,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   387,   388,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   302,    -1,    -1,
      -1,    -1,    -1,   404,    -1,    -1,   407,   408,    -1,    -1,
     315,    -1,   413,    -1,    -1,    -1,     3,    -1,     5,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
     431,    18,    -1,    -1,    -1,    -1,    -1,   342,   439,    -1,
      -1,   442,    -1,    -1,    -1,    -1,    -1,   448,    -1,    -1,
      -1,    -1,   453,   454,    -1,     9,    -1,   458,    -1,   460,
      -1,   462,   463,   464,    51,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    59,   378,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   387,   388,    71,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,   407,    -1,    -1,    -1,    -1,    -1,   413,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,   431,    -1,    82,    -1,
      -1,   118,   119,    -1,   439,    -1,    -1,   442,    -1,    -1,
      -1,    -1,    -1,   448,    98,    -1,    -1,    -1,   453,   454,
      -1,    -1,    -1,   458,    -1,   460,    -1,   462,   463,   464,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,     6,    -1,    -1,     9,    -1,     3,
      -1,     5,   179,    -1,    -1,    -1,    10,    -1,    -1,   186,
      -1,    -1,   189,   190,    18,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,   204,    -1,   206,
      -1,    -1,   209,    -1,    -1,    -1,    -1,    49,    -1,    -1,
     184,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    -1,
      -1,   195,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,   246,
      82,    75,    -1,   250,    86,   252,    -1,    -1,    -1,    -1,
      -1,    -1,   259,    87,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,   236,    -1,   238,    -1,    -1,   241,    -1,   243,
      -1,    -1,    -1,     6,    -1,   282,     9,   111,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
     297,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   318,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
     327,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,
      -1,   165,    -1,   340,    -1,    -1,    -1,    -1,   345,    -1,
     347,   315,   184,    -1,    -1,   179,    -1,    -1,   355,    82,
      -1,    -1,   186,   195,    -1,   189,   190,    -1,   365,    -1,
      -1,    94,    -1,   197,    -1,    98,    -1,   374,   342,    -1,
     204,    -1,   206,    -1,    -1,   209,   383,    -1,   385,   386,
      -1,    -1,   389,   225,   391,    -1,    -1,    -1,    -1,   396,
     232,    -1,    -1,    -1,   236,     6,   238,    -1,     9,   241,
      -1,   243,    -1,    -1,   378,   412,    -1,    -1,    -1,    -1,
     417,    -1,   246,   387,   388,   422,   250,    -1,   252,    -1,
      -1,    -1,    -1,    -1,    -1,   259,   433,    -1,    -1,    -1,
      -1,    -1,   274,   407,    -1,    -1,    -1,   279,   445,   413,
      -1,    -1,   449,   450,   451,   452,    -1,    -1,   282,    -1,
      -1,   184,   459,    -1,    -1,    -1,    -1,   431,    -1,    -1,
      -1,    -1,   195,   297,    -1,   439,    -1,    -1,   442,    -1,
      -1,    82,    -1,   315,   448,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   325,   318,    -1,    -1,    98,    -1,    -1,
      -1,    -1,   225,   327,    -1,    -1,    -1,    -1,     6,   232,
     342,     9,    -1,   236,    -1,   238,   340,    -1,   241,    -1,
     243,   345,    -1,   347,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   355,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     372,   365,    -1,    -1,    -1,    -1,   378,    -1,   149,    -1,
     374,   274,    -1,    -1,    -1,   387,   388,    -1,    -1,   383,
      -1,   385,   386,    -1,    -1,   389,    -1,   391,    -1,    -1,
      -1,    -1,   396,    -1,    -1,   407,    -1,    -1,    -1,    -1,
      -1,   413,    -1,   184,    82,    -1,   418,    -1,   412,    -1,
      -1,    -1,   315,   417,   195,    -1,    -1,    -1,   422,   431,
      98,    -1,    -1,   435,    -1,    -1,    -1,   439,    -1,   433,
     442,    -1,    -1,    -1,   112,    -1,   448,    -1,    -1,   342,
      -1,   445,    -1,    -1,   225,   449,   450,   451,   452,    -1,
       6,   232,    -1,     9,    -1,   236,    -1,   238,    -1,    -1,
     241,    -1,   243,    -1,     6,    -1,    -1,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   378,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   387,   388,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   407,    -1,   184,    -1,    -1,    -1,
     413,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,   431,    -1,
      -1,    -1,   210,    -1,   315,    -1,   439,    -1,    94,   442,
      82,    -1,    98,    -1,    -1,   448,    -1,   225,    -1,    -1,
      -1,    -1,    -1,    -1,   232,    -1,    98,    -1,   236,     6,
     238,   342,     9,   241,    -1,   243,    -1,    -1,    -1,    -1,
     112,    -1,    -1,     6,    -1,    -1,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   274,   378,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   387,   388,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   404,    -1,    -1,   407,   408,   184,    -1,
      -1,    -1,   413,    -1,    -1,    82,    -1,   315,    -1,   195,
      -1,    -1,   184,    -1,    -1,    -1,    -1,    94,    -1,    82,
     431,    98,    -1,   195,    -1,    -1,    -1,    -1,   439,    -1,
      -1,   442,    -1,    -1,   342,    98,    -1,   448,    -1,   225,
      -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,    -1,    -1,
     236,    -1,   238,   225,    -1,   241,    -1,   243,    -1,    -1,
     232,    -1,    -1,    -1,   236,     6,   238,    -1,     9,   241,
     378,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,
     388,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   274,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   407,
      -1,    -1,   274,    -1,    -1,   413,    -1,   184,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,
      -1,   184,    -1,   431,    -1,    -1,    -1,    -1,    -1,   315,
      -1,   439,   195,    -1,   442,    -1,    -1,    -1,    -1,    -1,
     448,    82,    -1,   315,    -1,    -1,    -1,    -1,   225,    -1,
      -1,    -1,    -1,    -1,    -1,   232,   342,    98,    -1,   236,
      -1,   238,   225,    -1,   241,    -1,   243,    -1,     6,   232,
     342,     9,    -1,   236,    -1,   238,    -1,    -1,   241,    -1,
     243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   378,    -1,    -1,    -1,    -1,   274,    -1,    -1,
      -1,   387,   388,    -1,    -1,    -1,   378,    -1,    -1,    -1,
      -1,   274,    -1,    -1,    -1,   387,   388,    -1,    -1,    -1,
      -1,   407,    -1,    -1,    -1,    -1,    -1,   413,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   407,    -1,    -1,   315,   180,
      -1,   413,    -1,   184,    82,   431,    -1,    -1,    -1,    -1,
      -1,    -1,   315,   439,   195,    -1,   442,    -1,    -1,   431,
      98,    -1,   448,    -1,    -1,   342,    -1,   439,    -1,    -1,
     442,    -1,    -1,    -1,    -1,    -1,   448,    -1,    -1,   342,
       6,    -1,    -1,     9,   225,    -1,    -1,    -1,    -1,    -1,
      -1,   232,    -1,    -1,    -1,   236,    -1,   238,    -1,    -1,
     241,   378,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     387,   388,    -1,    -1,    -1,   378,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   387,   388,    -1,    -1,    -1,    -1,
     407,    -1,    -1,   274,    -1,    -1,   413,    -1,    -1,    -1,
     403,    -1,    -1,    -1,   407,    -1,   184,    -1,    -1,    -1,
     413,    -1,    -1,    -1,   431,    -1,    82,   195,    -1,    -1,
      -1,    -1,   439,    -1,    -1,   442,    -1,    -1,   431,    -1,
      -1,   448,    98,    -1,   315,    -1,   439,    -1,    -1,   442,
      -1,    -1,    -1,     6,    -1,   448,     9,   225,    -1,    -1,
      -1,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,    -1,
     238,   342,    -1,   241,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   274,   378,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   387,   388,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    82,
      -1,    -1,    -1,    -1,    -1,     6,   407,    -1,     9,   195,
      -1,    -1,   413,    -1,    -1,    98,    -1,   315,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     431,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   439,   225,
      -1,   442,    -1,    -1,   342,    -1,   232,   448,    -1,    -1,
     236,    -1,   238,    -1,    -1,   241,    -1,   243,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   372,    -1,    -1,    -1,    -1,    -1,
     378,    82,    -1,    -1,    -1,    -1,    -1,    -1,   274,   387,
     388,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,
      -1,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   407,
      -1,    -1,   195,    -1,    -1,   413,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,   315,
      -1,    -1,    -1,   431,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   439,   225,    -1,   442,    -1,    -1,    -1,    -1,   232,
     448,    66,    67,   236,    -1,   238,   342,    -1,   241,    74,
     243,    76,    77,    78,    79,    80,    81,    -1,    37,    38,
      39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   184,    -1,    -1,    -1,   373,    -1,    -1,
      -1,   274,   378,    -1,   195,    -1,    -1,    -1,    -1,    -1,
      -1,   387,   388,   118,    -1,    74,    -1,    76,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   407,    -1,    -1,   225,    -1,    -1,   413,    -1,    -1,
      -1,   232,   315,    -1,    -1,   236,    -1,   238,    -1,    -1,
     241,    -1,   243,    -1,    -1,   431,    -1,    -1,    -1,   118,
      -1,    -1,    -1,   439,    -1,    -1,   442,    -1,    -1,   342,
      -1,    -1,   448,    -1,    -1,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   274,    -1,    -1,    -1,   192,    -1,    -1,
      -1,    98,    -1,    -1,    -1,    -1,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   378,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,   387,   388,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   315,    82,    -1,    -1,   233,   234,
      -1,    -1,    -1,    -1,   407,    -1,    -1,    -1,    -1,    -1,
     413,    98,   201,    -1,    -1,    -1,    -1,    -1,   253,    -1,
      -1,   342,    -1,   258,    -1,    -1,    -1,    -1,   431,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,    -1,   442,
      -1,   276,    -1,    -1,    -1,   448,    -1,   184,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   291,   378,   195,    -1,
      -1,    -1,    -1,   298,   253,   300,   387,   388,   303,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   313,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   407,    -1,   225,    -1,
      -1,    -1,   413,    -1,    -1,   232,    -1,   184,    -1,   236,
      82,   238,   291,    -1,   241,    -1,   243,    -1,   195,    -1,
     431,   300,    -1,    -1,    -1,    -1,    98,    -1,   439,    -1,
      -1,   442,    -1,    82,   313,    -1,    -1,   448,    -1,    -1,
      -1,    -1,   367,    -1,   369,   370,   371,   274,   225,    98,
      -1,   376,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,
      -1,   238,    -1,    -1,   241,   390,   243,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   414,
     369,   370,   371,    -1,   419,   420,   421,   274,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   432,    -1,    -1,
     435,   436,   184,    -1,    -1,   342,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   459,   184,    -1,    -1,   315,    -1,
     419,   420,   421,    -1,    -1,    -1,   195,    -1,    -1,    -1,
      -1,   378,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     387,   388,    -1,    -1,   236,   342,   238,    -1,    -1,   241,
      -1,   243,    -1,    -1,    -1,    -1,   225,    -1,    -1,    -1,
     407,    -1,    -1,   232,    -1,    -1,   413,   236,    -1,   238,
      -1,    -1,   241,    -1,   243,    -1,    -1,    -1,    -1,    -1,
      -1,   378,   274,    -1,   431,    -1,    -1,    -1,    -1,    -1,
     387,   388,   439,    -1,    -1,   442,    -1,    -1,    -1,    -1,
      -1,   448,    -1,    -1,    -1,   274,   453,   454,    -1,    -1,
     407,    -1,    -1,    -1,   461,   462,   413,    -1,    -1,    -1,
      -1,    -1,    -1,   315,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   431,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   439,    -1,    -1,   442,   315,    -1,    -1,    -1,
     342,   448,    -1,    -1,    -1,    -1,   453,   454,    -1,    -1,
      -1,    -1,    -1,    -1,   461,   462,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   342,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   378,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   387,   388,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   378,
      -1,    -1,    -1,    -1,    -1,   407,    -1,    -1,   387,   388,
      -1,   413,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   407,   431,
      -1,    -1,    -1,    -1,   413,    -1,    -1,   439,    -1,    -1,
     442,    -1,    -1,    -1,    -1,    -1,   448,    -1,    -1,    -1,
      -1,    -1,   431,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     439,    -1,    -1,   442,    -1,    -1,    -1,    -1,    -1,   448,
      -1,    -1,    -1,    -1,   453,   454,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   462
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   468,   469,     0,   183,   311,   470,   471,   472,   478,
     485,   487,   459,   459,     0,   471,   150,   493,   493,   236,
     312,   488,   488,   120,   473,   479,    27,   489,   489,   459,
     102,   598,   598,   236,   459,   486,    83,   494,   120,   474,
     480,   214,   490,  1145,   356,   169,   208,   216,   548,   459,
     307,   733,   733,   459,    73,   322,   406,   491,   492,   459,
     459,   356,   459,   168,   354,   405,   599,   605,   120,   475,
     481,   485,   135,   484,   492,   310,  1157,   495,   549,   459,
     550,   356,   444,   630,   601,    54,   434,   736,   139,   475,
     482,   493,   488,   275,   337,   377,   379,   496,   497,   501,
     509,   514,   552,   169,   551,    22,   251,   350,   587,   588,
     589,   590,   592,   596,   597,   459,   356,   239,   687,   442,
     604,   606,  1075,   738,   737,   341,   744,   488,   476,   459,
     459,   459,   459,   459,   359,   553,   459,   216,   586,    60,
      72,    93,   101,   178,   405,  1141,   319,   374,   375,   591,
     459,   589,   459,   597,   600,   459,   356,   235,   689,   603,
     605,   632,   633,   634,   607,    49,   739,   740,   741,  1136,
     739,   442,   459,   459,   598,   245,   358,   442,   500,   502,
     503,   504,   505,   507,   508,  1157,   182,   510,   511,   512,
     441,   498,   499,   500,  1171,    11,    25,    26,    57,    96,
      97,    99,   108,   151,   152,   162,   237,   352,   392,   442,
     515,   516,   517,   518,   525,   534,   538,   541,   542,   543,
     544,   545,   546,   547,   283,  1122,   552,   459,   280,   280,
    1154,   403,   277,  1153,   403,   395,  1165,    24,  1132,   602,
     631,   459,   356,   332,   691,   606,   449,   451,   452,   635,
     636,   637,   638,  1116,   608,   740,   372,   418,   742,   325,
     435,   734,   477,   372,  1145,   459,   503,   459,   504,    63,
     363,  1128,     9,   236,   513,   459,   512,   459,   459,   499,
     107,   442,  1087,  1145,  1145,  1087,   384,   367,  1161,  1145,
    1145,  1145,  1145,  1145,  1087,  1145,    56,  1138,  1145,   459,
     517,  1087,   587,   442,  1082,  1083,  1102,  1082,  1083,   236,
    1083,    85,  1139,   175,  1142,   603,   632,   688,   459,   356,
     351,   731,   632,   459,   459,   459,   459,   636,   637,   638,
    1116,   227,   228,   639,   642,   643,   649,     1,    47,    62,
     102,   219,   231,   319,   320,   332,   334,   435,   459,   609,
     610,   612,   616,   618,   620,   621,   627,   628,   629,  1145,
    1145,   372,   283,   743,   109,   745,   733,  1145,   236,  1105,
     459,   363,  1145,   211,   211,   236,   459,   249,   526,  1087,
    1087,  1145,  1145,  1145,  1083,    68,  1087,  1087,  1083,  1145,
    1083,   535,   536,  1087,    96,   520,  1087,   554,   200,   277,
    1125,  1083,   280,   410,   280,   593,   594,  1075,  1074,  1075,
     632,   690,   459,   356,   639,   171,   442,   645,   646,   442,
     645,  1139,  1145,   319,   321,  1126,  1126,  1145,  1139,  1145,
     249,  1151,  1145,    23,  1131,   277,   167,   188,    31,   110,
    1105,  1145,   442,   459,   735,   481,  1105,  1083,  1145,   195,
     236,   243,   315,   378,   407,   448,   539,   540,  1108,  1083,
     236,  1083,    23,   214,  1087,  1146,   278,   280,   522,   523,
     524,   519,   555,  1102,  1082,   280,  1082,   594,   301,   595,
    1075,   632,   692,   459,   640,    84,   641,  1105,   442,  1145,
    1131,  1084,  1102,   279,   380,   617,   236,  1083,  1086,  1105,
     436,  1145,   442,   730,   730,   170,   442,  1105,   746,   747,
     139,   483,    56,   443,   506,   124,   195,   236,   243,   255,
     315,   378,   381,   382,   448,   527,   528,   529,   532,   540,
     400,   537,  1105,   523,   384,  1164,   521,     1,     4,    19,
      29,   202,   233,   240,   261,   286,   292,   319,   326,   339,
     364,   366,   374,   405,   459,   556,   557,   562,   564,   569,
     570,   571,   572,   576,   577,   578,   579,   580,   581,   582,
     583,   585,  1128,  1082,  1105,   317,   693,   694,   695,   732,
     650,   647,  1145,   435,   679,   403,   615,  1102,   234,  1149,
     403,  1138,   200,  1144,   442,  1145,  1145,   747,     1,   442,
     748,   749,   750,   751,   752,   757,   488,   529,    17,   400,
    1108,  1105,  1145,   523,  1151,   319,   403,  1169,   364,  1151,
     218,  1147,  1145,    55,  1137,    37,   113,  1135,  1147,  1147,
     260,  1105,  1171,   384,  1145,   730,   695,     1,    21,    34,
      37,    38,    39,    40,    41,    42,    43,    74,    76,    77,
      78,    79,    80,    81,   118,   201,   217,   236,   253,   276,
     291,   298,   300,   313,   323,   330,   367,   369,   370,   371,
     393,   419,   420,   421,   432,   435,   436,   644,   651,   652,
     653,   655,   656,   657,   658,   659,   661,   673,   674,   676,
     677,   678,   684,   685,   686,  1145,  1162,    27,  1133,   188,
    1146,  1105,    56,   321,   611,   622,  1105,   372,  1163,   236,
     619,  1102,   619,   126,   459,   356,     3,     5,    10,    18,
      51,    52,    59,    71,    75,    87,   111,   118,   119,   149,
     161,   165,   179,   186,   189,   190,   197,   204,   206,   209,
     246,   250,   252,   259,   282,   297,   318,   327,   340,   345,
     347,   355,   365,   374,   383,   385,   386,   389,   391,   396,
     412,   417,   422,   433,   445,   449,   450,   451,   452,   459,
     758,   759,   769,   774,   778,   781,   794,   797,   802,   807,
     808,   809,   812,   814,   821,   825,   827,   842,   845,   847,
     849,   852,   854,   860,   869,   871,   888,   890,   893,   897,
     903,   913,   920,   922,   925,   929,   930,   941,   952,   962,
     968,   972,   978,   982,   984,   986,   988,   991,  1002,  1003,
    1012,  1014,  1015,  1172,  1173,  1174,  1175,   459,   530,   532,
    1087,  1145,  1147,   123,   167,   559,  1145,  1145,   319,   326,
     577,  1145,  1145,   364,  1145,  1145,  1132,     9,   260,   318,
     584,  1145,   442,   696,   225,   368,   418,   368,   418,   368,
     418,   368,   418,   368,   418,   438,  1170,   346,  1159,  1105,
    1101,  1102,  1102,   214,   224,   346,   675,  1145,  1145,  1100,
    1101,   167,   188,   223,   411,     9,    50,   225,   648,  1106,
    1107,  1108,   681,   682,  1106,    30,   623,   624,   625,   626,
    1134,  1171,  1138,   180,   614,  1143,   109,   236,   753,   760,
     770,   775,   779,   782,   795,   798,   803,   810,   813,   815,
     822,   826,   828,   843,   846,   848,  1169,   853,     1,   855,
     861,   870,   872,   889,   891,   894,   898,   904,   914,   921,
     923,   926,   931,   942,   953,   963,   236,   349,   973,   979,
     306,   983,   985,   987,   989,   992,   188,  1004,  1142,  1016,
     195,   236,   243,   315,   378,   448,   531,   533,   123,   316,
     364,   563,  1145,   117,   305,   558,    32,   164,   244,   573,
    1083,  1086,   381,  1083,  1083,   287,  1156,  1156,   281,  1083,
      61,    88,    89,   293,   459,   697,   698,   702,  1145,   448,
     403,   662,   462,  1103,  1104,   400,   658,  1106,   180,    27,
     654,   362,  1124,  1124,  1108,  1153,  1153,   466,   680,   682,
     400,    48,   409,   174,   615,  1105,   459,   459,   761,  1100,
       6,    82,    94,    98,   184,   225,   232,   238,   241,   274,
     342,   387,   388,   413,   431,   439,   771,  1069,  1090,  1091,
    1100,  1106,  1109,   442,   776,  1056,  1057,  1058,   236,  1079,
    1080,  1081,  1102,   236,  1098,  1100,  1109,     9,   796,   799,
     804,  1070,  1071,  1091,  1075,   405,   236,   816,  1090,  1097,
    1100,   823,  1091,   236,   404,   408,   829,   830,  1056,   296,
     297,   310,   356,   844,     6,  1088,  1089,  1100,  1100,   850,
     136,  1055,  1056,  1088,   701,  1100,   873,  1100,  1106,  1109,
     954,  1102,    94,   892,  1091,   895,  1091,   899,   177,   236,
     905,   908,   909,   910,  1079,  1098,  1102,  1171,  1075,  1072,
    1102,  1075,  1072,     9,   932,  1073,  1102,   150,   248,   943,
     944,   945,   946,   948,   949,   950,   951,  1076,  1077,  1088,
     954,  1075,   970,   969,   112,   974,   975,  1091,    94,   980,
    1090,   701,  1100,  1075,  1100,     8,    35,  1006,   107,  1072,
      17,  1083,   118,   236,   560,  1150,   441,   574,   574,   122,
     376,   441,   460,   568,  1118,  1127,  1083,  1085,  1145,   172,
     699,   700,   699,  1146,   710,   188,  1105,   402,  1168,   225,
     453,   454,   462,  1066,  1068,  1069,  1092,  1100,  1107,  1109,
     462,  1104,  1102,  1091,   236,  1137,  1101,  1101,  1108,  1170,
    1106,  1086,  1086,  1134,  1138,   127,   768,    30,   180,   762,
    1134,  1153,   462,  1100,   462,  1110,   462,  1111,  1153,  1125,
     462,   462,   462,   462,   462,   462,   462,   462,  1110,   128,
     773,   403,   772,  1091,   205,  1119,    56,  1059,  1060,   403,
    1125,   434,   783,   236,  1097,  1100,  1075,   130,   805,   156,
     460,   806,  1071,   348,  1123,   319,  1158,  1074,   132,   820,
     762,   427,   428,   429,   430,   133,   824,    49,   210,   783,
      17,   438,   831,   832,   833,   837,  1130,   100,  1153,  1089,
    1078,   399,  1167,   862,  1171,  1100,    92,   331,   394,   874,
     875,   876,   880,   885,   956,  1091,   403,   137,   896,    49,
     166,   207,   215,   288,   900,   909,   138,   906,   423,   436,
     400,   402,   397,   258,   304,  1120,   180,  1017,  1158,  1017,
    1073,   143,   940,   436,   934,  1095,  1100,  1107,   949,   951,
    1088,   403,  1077,   121,   403,   424,   947,   964,   187,   341,
     971,  1136,   210,   975,  1100,   146,   981,   180,   180,   319,
     321,   990,   112,   993,   333,   380,  1007,  1154,  1017,   533,
     568,  1118,   561,  1102,   240,   347,  1145,   122,   565,   566,
    1097,   701,   708,  1105,   642,   711,   114,   663,  1153,  1068,
    1068,  1068,    70,   360,   461,  1067,   453,   454,   455,   456,
     458,   465,  1068,    49,   365,  1160,  1149,  1086,   114,   613,
    1095,    25,    26,    67,    69,   103,   104,   105,   150,   152,
     160,   234,   401,   442,  1077,   441,   765,    66,   233,   301,
     763,   764,   149,   310,  1093,  1101,  1066,  1068,   403,  1068,
    1066,  1112,  1101,  1107,  1109,   442,  1068,  1115,  1068,  1068,
    1114,  1068,  1066,  1066,  1068,  1113,  1068,  1070,  1091,   187,
     341,   777,  1119,    12,    13,    14,    20,    58,   156,   157,
     185,   191,   214,   222,   226,   256,   257,   262,   272,   279,
     284,   302,   448,   453,   454,   455,   456,   458,   460,   461,
     463,   464,  1061,  1062,  1063,  1064,  1065,    12,    13,    14,
      58,   214,   256,   257,   262,   272,   279,   302,   453,   454,
     458,   462,  1061,  1062,  1063,  1064,  1065,  1091,   309,   780,
    1081,   784,   187,   341,   788,   324,   416,   800,   801,  1171,
    1056,   213,   266,  1048,  1049,  1050,  1052,   426,   441,   817,
    1171,   163,  1023,  1024,  1023,  1023,  1023,  1091,  1070,  1091,
      21,   404,   408,   838,   839,  1057,   134,   841,   440,   833,
     835,   438,   834,   830,  1101,   114,   851,  1079,   856,     9,
      12,    15,    16,   253,   254,   272,   273,   863,   867,   171,
    1095,     9,    56,   173,   223,   411,   881,   882,   883,   877,
     875,   958,  1127,  1154,   403,  1088,  1070,  1091,   366,   901,
     754,   755,  1055,   911,   912,  1100,  1079,     8,    35,  1019,
    1158,  1097,   210,   915,   927,  1171,   935,  1134,  1100,   935,
     403,   403,   524,   149,   404,   408,  1091,    49,   218,   965,
    1091,  1091,   372,  1091,  1100,   180,  1070,  1091,  1095,  1136,
     210,   996,  1100,   159,   163,  1008,     9,  1013,  1079,   927,
     565,  1102,   280,   567,  1083,  1118,   567,   193,   703,   233,
     234,   709,   645,  1154,    28,   115,   202,   660,   665,   666,
     667,   668,   670,  1101,  1107,  1109,   461,  1068,  1068,  1068,
    1068,  1068,  1068,   461,  1068,  1091,  1169,  1149,  1154,  1022,
    1024,   447,   446,  1095,  1022,   218,    31,    33,    36,    46,
     176,   181,   194,   242,   290,   314,   338,   343,   353,   357,
     415,   425,   766,   767,  1022,   270,  1152,  1152,  1152,   764,
     763,   236,  1094,  1101,   461,  1100,   465,   461,  1067,   461,
     461,  1067,   461,   461,   461,   461,  1067,   461,   461,   373,
    1028,  1029,  1070,  1089,   341,  1169,   398,  1166,  1166,   403,
    1079,   785,   786,   787,  1136,  1100,  1100,   163,   289,   789,
    1009,  1142,   240,   260,  1028,  1051,  1053,   131,   811,  1052,
      96,   305,   442,  1077,    33,    36,    44,    45,    46,   158,
     176,   194,   242,   290,   343,   353,   415,   818,   819,  1023,
     269,  1025,   265,  1026,   187,  1028,   187,  1130,   400,   840,
     836,   838,   754,  1154,   754,  1169,   331,   864,  1169,   403,
      49,   882,   884,  1095,     9,    56,   223,   411,   878,   879,
    1095,   959,  1128,   200,   285,  1155,   668,  1088,  1028,   187,
    1171,  1074,   138,   907,   756,     8,   180,   915,  1100,   126,
     263,  1038,  1039,  1041,  1048,   240,   260,   438,   126,   438,
     937,   938,  1095,  1094,  1091,  1145,  1048,   976,  1171,  1100,
    1028,   187,   403,     9,   994,   995,  1117,   997,  1100,   976,
     997,   307,  1011,   308,  1018,  1019,  1118,   251,   319,   321,
     575,  1145,   173,   704,  1105,   712,  1083,  1136,   669,   670,
     666,  1147,   664,   665,   461,   404,   683,  1083,  1026,  1022,
    1145,  1145,   121,   424,   767,  1097,  1097,  1097,  1110,  1123,
     461,  1068,  1083,  1110,  1110,  1068,  1110,  1110,  1110,   223,
     411,  1110,  1110,  1030,   268,  1031,  1028,  1089,   156,   284,
     156,   284,   786,   279,   742,    86,   325,   435,   265,   267,
     791,  1010,   790,   329,   344,   754,   754,   817,   817,   817,
     817,  1145,   153,   155,  1145,   121,   424,   819,   754,  1027,
    1070,  1071,  1070,  1071,   839,  1056,   754,  1100,   125,   857,
     435,   865,   866,   867,   110,   868,   435,  1096,  1100,  1106,
    1095,    49,   886,   879,   175,   886,   955,  1145,   285,  1147,
    1070,   584,   902,  1171,   757,   912,  1091,   199,   916,  1171,
    1040,  1042,   141,   924,  1041,   142,   928,   240,  1056,   936,
    1055,   937,   262,   966,  1121,   144,   967,   289,  1033,  1034,
     300,  1123,  1070,  1096,   284,  1095,   113,   998,   394,  1000,
    1154,   154,   264,  1020,  1043,  1044,  1046,  1049,     7,  1129,
     575,  1105,   116,   220,   705,    67,    66,    67,   192,   233,
     234,   258,   303,   376,   390,   414,   459,   656,   657,   659,
     661,   673,   676,   678,   686,   713,   714,   716,   717,   718,
     719,   721,   722,   723,   727,   728,   442,   671,   672,  1145,
    1145,   448,  1099,  1100,  1105,  1099,  1054,  1136,  1054,  1028,
     461,   754,  1032,  1169,   156,  1169,   156,  1091,   129,   793,
     792,   754,  1023,  1023,  1023,  1023,  1099,  1099,  1054,  1054,
     754,  1028,   328,  1028,   328,   858,   136,   859,   866,   102,
    1140,   886,   886,  1096,  1019,   207,   434,   960,  1083,  1145,
    1028,   240,   260,    49,   240,   218,   917,   198,   240,   260,
     437,   754,   754,   933,   754,   939,   701,  1061,  1062,  1063,
    1064,  1065,  1035,   145,   977,   267,  1036,  1100,  1028,  1028,
     995,  1144,    95,   999,  1144,  1033,   166,   207,   215,   288,
    1005,  1074,  1045,  1047,   148,  1021,  1046,   293,  1077,  1099,
    1145,    91,   221,   706,   271,  1152,   203,   729,   270,   271,
     726,  1131,   192,   438,  1145,  1153,  1145,   719,   258,   299,
     724,   725,  1105,   672,  1082,  1106,  1099,   754,  1169,  1169,
     754,  1071,  1071,   754,    49,   886,   406,   887,   307,  1074,
     187,   288,   961,   957,   344,  1091,  1145,   918,  1038,  1049,
     240,   240,   754,   754,   754,  1037,  1100,  1144,  1100,   147,
    1001,   754,   754,   233,   234,  1148,  1105,  1145,  1145,   174,
     707,  1145,  1146,  1145,  1055,  1100,   720,  1083,    90,    91,
     116,   294,   295,   335,   336,   715,   293,  1105,   725,  1148,
    1028,  1028,  1091,  1091,  1145,  1074,   307,  1102,   423,   701,
     140,   919,   754,  1100,  1105,  1105,  1145,  1105,  1105,  1123,
     908,  1145,  1055,  1105,   908
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   467,   469,   468,   470,   470,   471,   471,   473,   474,
     472,   476,   477,   475,   479,   480,   478,   481,   481,   481,
     482,   482,   483,   484,   486,   485,   487,   488,   488,   489,
     489,   490,   490,   491,   491,   491,   492,   492,   493,   493,
     494,   494,   495,   495,   496,   496,   496,   496,   497,   498,
     498,   498,   498,   499,   500,   501,   502,   502,   502,   502,
     503,   503,   504,   504,   504,   505,   506,   506,   507,   508,
     509,   510,   510,   511,   511,   512,   512,   513,   513,   514,
     515,   515,   515,   516,   516,   517,   517,   517,   517,   517,
     517,   517,   517,   517,   517,   517,   518,   519,   518,   520,
     518,   518,   518,   518,   518,   521,   521,   522,   522,   523,
     524,   524,   526,   525,   527,   527,   527,   527,   527,   528,
     528,   529,   529,   530,   529,   531,   531,   532,   532,   532,
     532,   532,   532,   533,   533,   533,   533,   533,   533,   534,
     535,   536,   536,   537,   537,   538,   539,   539,   540,   540,
     541,   542,   543,   544,   545,   546,   547,   548,   548,   549,
     548,   550,   548,   551,   551,   552,   552,   554,   553,   555,
     555,   556,   556,   556,   556,   556,   556,   556,   556,   556,
     556,   556,   556,   556,   556,   556,   557,   557,   557,   558,
     558,   558,   559,   559,   559,   560,   560,   560,   561,   561,
     562,   563,   563,   563,   564,   564,   566,   565,   565,   567,
     568,   568,   568,   569,   570,   571,   571,   571,   572,   573,
     573,   573,   574,   574,   574,   574,   575,   575,   576,   576,
     577,   577,   577,   577,   577,   578,   579,   580,   580,   581,
     582,   582,   583,   584,   584,   584,   585,   586,   586,   587,
     587,   587,   587,   588,   588,   589,   589,   590,   591,   591,
     591,   591,   592,   593,   593,   594,   595,   595,   596,   596,
     596,   597,   597,   597,   597,   597,   598,   598,   599,   600,
     599,   601,   599,   602,   602,   603,   604,   604,   605,   605,
     607,   606,   608,   608,   609,   609,   609,   609,   609,   609,
     609,   609,   609,   609,   609,   609,   610,   611,   611,   611,
     612,   612,   612,   613,   613,   614,   614,   615,   615,   616,
     617,   617,   618,   618,   619,   619,   620,   621,   622,   622,
     623,   623,   623,   624,   625,   626,   627,   628,   629,   629,
     630,   631,   630,   632,   632,   634,   633,   635,   635,   635,
     635,   635,   635,   635,   635,   635,   636,   636,   637,   637,
     638,   638,   639,   640,   639,   641,   639,   642,   643,   644,
     644,   645,   645,   645,   646,   647,   647,   648,   648,   648,
     649,   650,   650,   651,   651,   651,   651,   651,   651,   651,
     651,   651,   651,   651,   651,   651,   651,   651,   651,   652,
     653,   654,   654,   655,   656,   657,   657,   658,   658,   658,
     658,   658,   658,   658,   658,   658,   658,   658,   658,   658,
     658,   658,   658,   658,   658,   658,   658,   658,   658,   658,
     658,   658,   658,   658,   658,   658,   658,   658,   658,   658,
     658,   658,   659,   659,   660,   660,   660,   661,   662,   662,
     663,   663,   664,   664,   665,   666,   667,   667,   668,   668,
     669,   669,   670,   671,   671,   672,   673,   674,   675,   675,
     675,   676,   677,   678,   680,   679,   681,   681,   682,   682,
     683,   683,   684,   684,   685,   686,   687,   688,   687,   689,
     690,   689,   691,   692,   691,   693,   693,   694,   694,   695,
     696,   696,   697,   697,   697,   697,   698,   698,   699,   700,
     700,   701,   701,   702,   703,   703,   704,   704,   705,   705,
     706,   706,   707,   707,   708,   708,   708,   709,   709,   710,
     710,   711,   712,   712,   713,   713,   713,   713,   713,   713,
     713,   713,   713,   713,   713,   713,   713,   713,   713,   713,
     714,   715,   715,   715,   715,   715,   715,   715,   716,   717,
     717,   717,   718,   718,   719,   720,   721,   686,   722,   723,
     723,   724,   724,   725,   725,   725,   726,   726,   726,   727,
     728,   729,   729,   730,   731,   732,   731,   733,   734,   735,
     733,   736,   737,   736,   738,   736,   739,   739,   740,   741,
     741,   741,   742,   742,   742,   742,   742,   743,   743,   744,
     744,   745,   746,   745,   747,   747,   748,   748,   748,   748,
     748,   749,   750,   751,   752,   753,   753,   755,   756,   754,
     757,   757,   758,   758,   758,   758,   758,   758,   758,   758,
     758,   758,   758,   758,   758,   758,   758,   758,   758,   758,
     758,   758,   758,   758,   758,   758,   758,   758,   758,   758,
     758,   758,   758,   758,   758,   758,   758,   758,   758,   758,
     758,   758,   758,   758,   758,   758,   758,   758,   758,   758,
     758,   758,   758,   758,   758,   758,   760,   759,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   762,   762,   762,   762,   762,
     762,   763,   764,   764,   765,   765,   766,   766,   767,   767,
     767,   767,   767,   767,   767,   767,   767,   767,   767,   767,
     767,   767,   767,   767,   767,   768,   768,   770,   769,   771,
     771,   771,   772,   772,   773,   773,   775,   774,   776,   776,
     777,   777,   778,   779,   779,   780,   780,   782,   781,   783,
     784,   783,   785,   785,   786,   786,   787,   787,   787,   787,
     788,   788,   788,   789,   790,   789,   791,   792,   791,   793,
     793,   795,   794,   796,   796,   796,   798,   797,   799,   799,
     800,   800,   800,   800,   800,   801,   801,   803,   802,   804,
     805,   805,   806,   806,   807,   808,   810,   809,   811,   811,
     813,   812,   815,   814,   816,   816,   816,   816,   816,   816,
     816,   816,   816,   817,   817,   817,   818,   818,   819,   819,
     819,   819,   819,   819,   819,   819,   819,   819,   819,   819,
     819,   819,   819,   820,   820,   822,   821,   823,   823,   823,
     823,   823,   824,   824,   826,   825,   828,   827,   829,   829,
     830,   830,   830,   831,   832,   832,   834,   833,   835,   836,
     835,   837,   837,   838,   838,   839,   839,   839,   839,   840,
     840,   841,   841,   843,   842,   844,   844,   844,   844,   844,
     844,   846,   845,   848,   847,   850,   849,   851,   851,   853,
     852,   855,   856,   854,   854,   857,   858,   857,   859,   859,
     861,   860,   862,   862,   863,   863,   863,   864,   864,   865,
     865,   866,   867,   867,   867,   867,   867,   867,   867,   868,
     868,   870,   869,   872,   871,   873,   873,   873,   874,   874,
     875,   875,   875,   877,   876,   878,   878,   879,   879,   879,
     879,   879,   879,   880,   881,   881,   882,   882,   883,   883,
     883,   883,   883,   884,   885,   886,   886,   887,   887,   889,
     888,   891,   890,   892,   892,   894,   893,   895,   895,   896,
     896,   898,   897,   899,   899,   900,   900,   900,   900,   901,
     901,   902,   902,   902,   904,   903,   905,   906,   905,   905,
     907,   907,   908,   908,   909,   909,   909,   909,   909,   910,
     910,   911,   911,   912,   914,   913,   915,   915,   916,   916,
     916,   916,   916,   916,   917,   917,   918,   918,   918,   919,
     919,   921,   920,   923,   922,   924,   924,   926,   925,   927,
     927,   927,   928,   928,   929,   931,   930,   932,   933,   932,
     934,   934,   935,   936,   935,   937,   937,   939,   938,   940,
     940,   942,   941,   943,   943,   943,   943,   943,   944,   945,
     945,   946,   947,   947,   948,   948,   949,   950,   950,   951,
     951,   953,   952,   955,   954,   956,   956,   957,   957,   958,
     958,   959,   959,   960,   960,   960,   961,   961,   961,   963,
     964,   962,   965,   965,   966,   966,   966,   966,   966,   967,
     967,   969,   968,   970,   968,   971,   971,   971,   973,   972,
     974,   974,   975,   975,   975,   976,   976,   977,   977,   979,
     978,   980,   980,   980,   981,   981,   982,   983,   983,   985,
     984,   987,   986,   989,   988,   990,   990,   990,   992,   991,
     993,   993,   994,   994,   995,   996,   996,   997,   998,   998,
     999,   999,  1000,  1000,  1001,  1001,  1002,  1002,  1002,  1003,
    1004,  1004,  1005,  1005,  1005,  1005,  1005,  1006,  1006,  1007,
    1007,  1008,  1008,  1009,  1009,  1010,  1010,  1011,  1011,  1012,
    1013,  1013,  1014,  1016,  1015,  1017,  1017,  1018,  1018,  1018,
    1018,  1019,  1019,  1020,  1020,  1020,  1021,  1021,  1022,  1023,
    1024,  1025,  1024,  1026,  1027,  1026,  1028,  1029,  1030,  1029,
    1031,  1032,  1031,  1033,  1034,  1035,  1034,  1036,  1037,  1036,
    1038,  1038,  1038,  1040,  1039,  1042,  1041,  1043,  1043,  1043,
    1045,  1044,  1047,  1046,  1048,  1048,  1049,  1049,  1049,  1051,
    1050,  1053,  1052,  1054,  1054,  1055,  1056,  1058,  1057,  1059,
    1059,  1059,  1059,  1059,  1059,  1059,  1059,  1059,  1059,  1059,
    1059,  1059,  1059,  1059,  1059,  1059,  1059,  1059,  1060,  1060,
    1060,  1060,  1060,  1060,  1060,  1060,  1060,  1060,  1060,  1060,
    1060,  1060,  1060,  1060,  1060,  1060,  1060,  1060,  1060,  1060,
    1060,  1060,  1060,  1060,  1060,  1060,  1061,  1061,  1061,  1062,
    1062,  1063,  1063,  1064,  1064,  1064,  1065,  1065,  1065,  1066,
    1066,  1067,  1067,  1067,  1068,  1068,  1068,  1068,  1068,  1068,
    1068,  1068,  1068,  1069,  1069,  1070,  1070,  1071,  1072,  1073,
    1074,  1074,  1075,  1076,  1076,  1077,  1078,  1078,  1079,  1080,
    1080,  1080,  1081,  1082,  1082,  1083,  1084,  1084,  1085,  1085,
    1086,  1086,  1087,  1088,  1088,  1089,  1089,  1090,  1090,  1091,
    1091,  1091,  1091,  1091,  1091,  1091,  1091,  1091,  1092,  1092,
    1092,  1092,  1092,  1092,  1092,  1093,  1093,  1094,  1094,  1095,
    1095,  1096,  1096,  1097,  1097,  1098,  1098,  1098,  1099,  1099,
    1099,  1100,  1101,  1101,  1101,  1101,  1102,  1102,  1103,  1104,
    1104,  1105,  1106,  1106,  1107,  1107,  1108,  1108,  1108,  1108,
    1108,  1108,  1108,  1109,  1109,  1109,  1109,  1109,  1109,  1109,
    1109,  1109,  1109,  1109,  1109,  1110,  1110,  1110,  1111,  1111,
    1112,  1112,  1113,  1113,  1113,  1114,  1114,  1115,  1115,  1116,
    1117,  1117,  1118,  1118,  1119,  1119,  1120,  1120,  1120,  1121,
    1121,  1122,  1122,  1123,  1123,  1124,  1124,  1125,  1125,  1126,
    1126,  1127,  1127,  1128,  1128,  1129,  1129,  1130,  1130,  1131,
    1131,  1132,  1132,  1133,  1133,  1134,  1134,  1135,  1135,  1136,
    1136,  1137,  1137,  1138,  1138,  1139,  1139,  1140,  1140,  1141,
    1141,  1142,  1142,  1143,  1143,  1144,  1144,  1145,  1145,  1146,
    1146,  1146,  1147,  1147,  1148,  1148,  1148,  1149,  1149,  1150,
    1150,  1151,  1151,  1152,  1152,  1153,  1153,  1154,  1154,  1155,
    1155,  1155,  1156,  1156,  1157,  1157,  1158,  1158,  1159,  1159,
    1160,  1160,  1161,  1161,  1162,  1162,  1162,  1163,  1163,  1164,
    1164,  1165,  1165,  1166,  1166,  1167,  1167,  1168,  1168,  1169,
    1169,  1170,  1170,  1171,  1171,  1172,  1173,  1174,  1175
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
       2,     1,     2,     1,     2,     1,     1,     2,     1,     2,
       1,     2,     1,     0,     5,     0,     4,     1,     1,     0,
       1,     0,     1,     1,     1,     0,     2,     1,     3,     3,
       6,     0,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       3,     0,     2,     2,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     1,     3,     3,     0,     2,     2,     6,     0,     2,
       0,     3,     0,     1,     1,     4,     1,     2,     1,     1,
       0,     1,     3,     1,     2,     1,     2,     2,     0,     1,
       1,     3,     1,     3,     0,     8,     1,     2,     1,     3,
       0,     3,     2,     4,     2,     1,     0,     0,     5,     0,
       0,     5,     0,     0,     5,     0,     1,     1,     2,     5,
       0,     2,     2,     3,     1,     1,     2,     2,     2,     0,
       1,     1,     2,     8,     0,     3,     0,     4,     0,     4,
       0,     3,     0,     3,     1,     4,     2,     1,     1,     0,
       2,     4,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       2,     1,     1,     2,     3,     1,     3,     6,     2,     3,
       2,     1,     2,     2,     1,     2,     0,     1,     1,     4,
       2,     0,     1,     1,     0,     0,     4,     0,     0,     0,
       9,     0,     0,     3,     0,     3,     1,     2,     4,     0,
       2,     2,     0,     3,     3,     4,     3,     0,     1,     0,
       2,     0,     0,     7,     0,     2,     1,     1,     1,     2,
       1,     4,     2,     1,     1,     0,     1,     0,     0,     3,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
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
       1,     0,     1,     0,     1,     1,     1,     1,     1
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
#line 778 "parser.y"
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
#line 5235 "parser.c"
    break;

  case 3: /* start: $@1 nested_list "end of file"  */
#line 802 "parser.y"
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
#line 5257 "parser.c"
    break;

  case 8: /* $@2: %empty  */
#line 833 "parser.y"
                        { cb_validate_program_environment (current_program); }
#line 5263 "parser.c"
    break;

  case 9: /* $@3: %empty  */
#line 834 "parser.y"
                        { cb_validate_program_data (current_program); }
#line 5269 "parser.c"
    break;

  case 10: /* program_definition: identification_division environment_division $@2 data_division $@3 procedure_division nested_prog end_program  */
#line 837 "parser.y"
              {
	cb_tree file = current_program->file_list;
	for(; file; file = CB_CHAIN(file)) {
		cb_validate_indexed_file_key(CB_FILE(CB_VALUE(file)));
	}
  }
#line 5280 "parser.c"
    break;

  case 11: /* $@4: %empty  */
#line 847 "parser.y"
                        { cb_validate_program_environment (current_program); }
#line 5286 "parser.c"
    break;

  case 12: /* $@5: %empty  */
#line 848 "parser.y"
                        { cb_validate_program_data (current_program); }
#line 5292 "parser.c"
    break;

  case 13: /* program_mandatory: identification_division environment_division $@4 data_division $@5 procedure_division nested_prog end_mandatory  */
#line 851 "parser.y"
                {
	cb_tree file = current_program->file_list;
	for(; file; file = CB_CHAIN(file)) {
		cb_validate_indexed_file_key(CB_FILE(CB_VALUE(file)));
	}
  }
#line 5303 "parser.c"
    break;

  case 14: /* $@6: %empty  */
#line 861 "parser.y"
                        { cb_validate_program_environment (current_program); }
#line 5309 "parser.c"
    break;

  case 15: /* $@7: %empty  */
#line 862 "parser.y"
                        { cb_validate_program_data (current_program); }
#line 5315 "parser.c"
    break;

  case 21: /* end_program: "END PROGRAM" program_name '.'  */
#line 874 "parser.y"
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
#line 5344 "parser.c"
    break;

  case 22: /* end_mandatory: "END PROGRAM" program_name '.'  */
#line 902 "parser.y"
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
#line 5371 "parser.c"
    break;

  case 23: /* end_function: "END FUNCTION" program_name '.'  */
#line 928 "parser.y"
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
#line 5398 "parser.c"
    break;

  case 24: /* $@8: %empty  */
#line 959 "parser.y"
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
#line 5444 "parser.c"
    break;

  case 26: /* function_division: "FUNCTION-ID" '.' program_name as_literal '.'  */
#line 1005 "parser.y"
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
#line 5489 "parser.c"
    break;

  case 29: /* as_literal: %empty  */
#line 1053 "parser.y"
                                { yyval = NULL; }
#line 5495 "parser.c"
    break;

  case 30: /* as_literal: AS "Literal"  */
#line 1054 "parser.y"
                                { yyval = yyvsp[0]; }
#line 5501 "parser.c"
    break;

  case 33: /* program_type_clause: COMMON  */
#line 1063 "parser.y"
  {
	if (!current_program->nested_level) {
		cb_error (_("COMMON may only be used in a nested program"));
	}
	current_program->flag_common = 1;
  }
#line 5512 "parser.c"
    break;

  case 34: /* program_type_clause: COMMON _init_or_recurs  */
#line 1070 "parser.y"
  {
	if (!current_program->nested_level) {
		cb_error (_("COMMON may only be used in a nested program"));
	}
	current_program->flag_common = 1;
  }
#line 5523 "parser.c"
    break;

  case 36: /* _init_or_recurs: "INITIAL"  */
#line 1081 "parser.y"
  {
	current_program->flag_initial = 1;
  }
#line 5531 "parser.c"
    break;

  case 37: /* _init_or_recurs: RECURSIVE  */
#line 1085 "parser.y"
  {
	current_program->flag_recursive = 1;
	current_program->flag_initial = 1;
  }
#line 5540 "parser.c"
    break;

  case 41: /* configuration_section: CONFIGURATION SECTION '.' configuration_list  */
#line 1110 "parser.y"
  {
	if (current_program->nested_level) {
		cb_error (_("CONFIGURATION SECTION not allowed in nested programs"));
	}
  }
#line 5550 "parser.c"
    break;

  case 53: /* with_debugging_mode: _with DEBUGGING MODE  */
#line 1145 "parser.y"
  {
	cb_verify (cb_debugging_line, "DEBUGGING MODE");
  }
#line 5558 "parser.c"
    break;

  case 54: /* computer_name: "Identifier"  */
#line 1151 "parser.y"
       { }
#line 5564 "parser.c"
    break;

  case 65: /* object_computer_memory: MEMORY SIZE _is integer object_char_or_word  */
#line 1182 "parser.y"
  {
	cb_verify (cb_memory_size_clause, "MEMORY SIZE");
  }
#line 5572 "parser.c"
    break;

  case 68: /* object_computer_sequence: _program coll_sequence _is reference  */
#line 1194 "parser.y"
  {
	current_program->collating_sequence = yyvsp[0];
  }
#line 5580 "parser.c"
    break;

  case 69: /* object_computer_segment: "SEGMENT-LIMIT" _is integer  */
#line 1201 "parser.y"
  {
	/* Ignore */
  }
#line 5588 "parser.c"
    break;

  case 75: /* repository_name: FUNCTION repository_literal_list INTRINSIC  */
#line 1226 "parser.y"
  {
	current_program->function_spec_list = yyvsp[-1];
  }
#line 5596 "parser.c"
    break;

  case 76: /* repository_name: FUNCTION ALL INTRINSIC  */
#line 1230 "parser.y"
  {
	functions_are_all = 1;
  }
#line 5604 "parser.c"
    break;

  case 77: /* repository_literal_list: "Literal"  */
#line 1236 "parser.y"
                        { yyval = cb_list_init (yyvsp[0]); }
#line 5610 "parser.c"
    break;

  case 78: /* repository_literal_list: repository_literal_list "Literal"  */
#line 1238 "parser.y"
                        { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5616 "parser.c"
    break;

  case 96: /* mnemonic_name_clause: "Identifier" _is CRT  */
#line 1279 "parser.y"
  {
	save_tree_1 = lookup_system_name (CB_NAME (yyvsp[-2]));
	if (save_tree_1 == cb_error_node) {
		cb_error_x (yyvsp[-2], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-2]));
	} else if (CB_SYSTEM_NAME(save_tree_1)->token != CB_DEVICE_CONSOLE) {
		cb_error_x (save_tree_1, _("Invalid CRT clause"));
	}
	/* current_program->flag_screen = 1; */
  }
#line 5630 "parser.c"
    break;

  case 97: /* $@9: %empty  */
#line 1289 "parser.y"
  {
	save_tree_1 = lookup_system_name (CB_NAME (yyvsp[-2]));
	if (save_tree_1 == cb_error_node) {
		cb_error_x (yyvsp[-2], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-2]));
	} else {
		cb_define (yyvsp[0], save_tree_1);
	}
	save_tree_2 = yyvsp[0];
  }
#line 5644 "parser.c"
    break;

  case 99: /* $@10: %empty  */
#line 1300 "parser.y"
  {
	save_tree_1 = lookup_system_name (CB_NAME (yyvsp[-1]));
	if (save_tree_1 == cb_error_node) {
		cb_error_x (yyvsp[-1], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-1]));
	}
	save_tree_2 = NULL;
  }
#line 5656 "parser.c"
    break;

  case 101: /* mnemonic_name_clause: "ARGUMENT-NUMBER" _is undefined_word  */
#line 1309 "parser.y"
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
#line 5674 "parser.c"
    break;

  case 102: /* mnemonic_name_clause: "ARGUMENT-VALUE" _is undefined_word  */
#line 1323 "parser.y"
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
#line 5692 "parser.c"
    break;

  case 103: /* mnemonic_name_clause: "ENVIRONMENT-NAME" _is undefined_word  */
#line 1337 "parser.y"
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
#line 5710 "parser.c"
    break;

  case 104: /* mnemonic_name_clause: "ENVIRONMENT-VALUE" _is undefined_word  */
#line 1351 "parser.y"
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
#line 5728 "parser.c"
    break;

  case 109: /* special_name_mnemonic_on_off: on_or_off _status _is undefined_word  */
#line 1377 "parser.y"
  {
	if (!save_tree_2 && !cb_switch_no_mnemonic) {
		cb_error_x (yyvsp[0], _("'%s' with no mnemonic name"), CB_NAME (yyvsp[0]));
	} else {
		cb_define_switch_name (yyvsp[0], save_tree_1, yyvsp[-3], save_tree_2);
	}
  }
#line 5740 "parser.c"
    break;

  case 110: /* on_or_off: ON  */
#line 1387 "parser.y"
                                { yyval = cb_int1; }
#line 5746 "parser.c"
    break;

  case 111: /* on_or_off: OFF  */
#line 1388 "parser.y"
                                { yyval = cb_int0; }
#line 5752 "parser.c"
    break;

  case 112: /* $@11: %empty  */
#line 1396 "parser.y"
  {
	save_tree_1 = yyvsp[0];
  }
#line 5760 "parser.c"
    break;

  case 113: /* alphabet_name_clause: ALPHABET undefined_word $@11 _is alphabet_definition  */
#line 1400 "parser.y"
  {
	current_program->alphabet_name_list =
		cb_list_add (current_program->alphabet_name_list, yyvsp[0]);
  }
#line 5769 "parser.c"
    break;

  case 114: /* alphabet_definition: NATIVE  */
#line 1407 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_NATIVE); }
#line 5775 "parser.c"
    break;

  case 115: /* alphabet_definition: "STANDARD-1"  */
#line 1408 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_STANDARD_1); }
#line 5781 "parser.c"
    break;

  case 116: /* alphabet_definition: "STANDARD-2"  */
#line 1409 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_STANDARD_2); }
#line 5787 "parser.c"
    break;

  case 117: /* alphabet_definition: EBCDIC  */
#line 1410 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_EBCDIC); }
#line 5793 "parser.c"
    break;

  case 118: /* alphabet_definition: alphabet_literal_list  */
#line 1412 "parser.y"
  {
	yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_CUSTOM);
	CB_ALPHABET_NAME (yyval)->custom_list = yyvsp[0];
  }
#line 5802 "parser.c"
    break;

  case 119: /* alphabet_literal_list: alphabet_literal  */
#line 1419 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5808 "parser.c"
    break;

  case 120: /* alphabet_literal_list: alphabet_literal_list alphabet_literal  */
#line 1421 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5814 "parser.c"
    break;

  case 121: /* alphabet_literal: alphabet_lits  */
#line 1425 "parser.y"
                                        { yyval = yyvsp[0]; }
#line 5820 "parser.c"
    break;

  case 122: /* alphabet_literal: alphabet_lits THRU alphabet_lits  */
#line 1426 "parser.y"
                                        { yyval = cb_build_pair (yyvsp[-2], yyvsp[0]); }
#line 5826 "parser.c"
    break;

  case 123: /* @12: %empty  */
#line 1428 "parser.y"
  {
	yyval = cb_list_init (yyvsp[-1]);
	save_tree_2 = yyval;
  }
#line 5835 "parser.c"
    break;

  case 124: /* alphabet_literal: alphabet_lits ALSO @12 alphabet_also_sequence  */
#line 1433 "parser.y"
  {
	yyval = yyvsp[-1];
  }
#line 5843 "parser.c"
    break;

  case 127: /* alphabet_lits: "Literal"  */
#line 1444 "parser.y"
                                { yyval = yyvsp[0]; }
#line 5849 "parser.c"
    break;

  case 128: /* alphabet_lits: SPACE  */
#line 1445 "parser.y"
                                { yyval = cb_space; }
#line 5855 "parser.c"
    break;

  case 129: /* alphabet_lits: ZERO  */
#line 1446 "parser.y"
                                { yyval = cb_zero; }
#line 5861 "parser.c"
    break;

  case 130: /* alphabet_lits: QUOTE  */
#line 1447 "parser.y"
                                { yyval = cb_quote; }
#line 5867 "parser.c"
    break;

  case 131: /* alphabet_lits: "HIGH-VALUE"  */
#line 1448 "parser.y"
                                { yyval = cb_norm_high; }
#line 5873 "parser.c"
    break;

  case 132: /* alphabet_lits: "LOW-VALUE"  */
#line 1449 "parser.y"
                                { yyval = cb_norm_low; }
#line 5879 "parser.c"
    break;

  case 133: /* alphabet_also_literal: "Literal"  */
#line 1453 "parser.y"
                                { cb_list_add (save_tree_2, yyvsp[0]); }
#line 5885 "parser.c"
    break;

  case 134: /* alphabet_also_literal: SPACE  */
#line 1454 "parser.y"
                                { cb_list_add (save_tree_2, cb_space); }
#line 5891 "parser.c"
    break;

  case 135: /* alphabet_also_literal: ZERO  */
#line 1455 "parser.y"
                                { cb_list_add (save_tree_2, cb_zero); }
#line 5897 "parser.c"
    break;

  case 136: /* alphabet_also_literal: QUOTE  */
#line 1456 "parser.y"
                                { cb_list_add (save_tree_2, cb_quote); }
#line 5903 "parser.c"
    break;

  case 137: /* alphabet_also_literal: "HIGH-VALUE"  */
#line 1457 "parser.y"
                                { cb_list_add (save_tree_2, cb_norm_high); }
#line 5909 "parser.c"
    break;

  case 138: /* alphabet_also_literal: "LOW-VALUE"  */
#line 1458 "parser.y"
                                { cb_list_add (save_tree_2, cb_norm_low); }
#line 5915 "parser.c"
    break;

  case 139: /* symbolic_characters_clause: SYMBOLIC _characters symbolic_characters_list  */
#line 1466 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->symbolic_list =
			cb_list_add (current_program->symbolic_list, yyvsp[0]);
	}
	PENDING ("SYMBOLIC CHARACTERS");
  }
#line 5927 "parser.c"
    break;

  case 140: /* symbolic_characters_list: char_list _is_are integer_list  */
#line 1477 "parser.y"
  {
	if (cb_list_length (yyvsp[-2]) != cb_list_length (yyvsp[0])) {
		cb_error (_("Invalid SYMBOLIC clause"));
		yyval = NULL;
	} else {
		yyval = NULL;
	}
  }
#line 5940 "parser.c"
    break;

  case 141: /* char_list: undefined_word  */
#line 1488 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5946 "parser.c"
    break;

  case 142: /* char_list: char_list undefined_word  */
#line 1489 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5952 "parser.c"
    break;

  case 143: /* integer_list: integer  */
#line 1493 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5958 "parser.c"
    break;

  case 144: /* integer_list: integer_list integer  */
#line 1494 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5964 "parser.c"
    break;

  case 145: /* class_name_clause: CLASS undefined_word _is class_item_list  */
#line 1502 "parser.y"
  {
	current_program->class_name_list =
			cb_list_add (current_program->class_name_list,
			cb_build_class_name (yyvsp[-2], yyvsp[0]));
  }
#line 5974 "parser.c"
    break;

  case 146: /* class_item_list: class_item  */
#line 1510 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5980 "parser.c"
    break;

  case 147: /* class_item_list: class_item_list class_item  */
#line 1511 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5986 "parser.c"
    break;

  case 148: /* class_item: basic_value  */
#line 1515 "parser.y"
                                { yyval = yyvsp[0]; }
#line 5992 "parser.c"
    break;

  case 149: /* class_item: basic_value THRU basic_value  */
#line 1517 "parser.y"
  {
	/* if (CB_LITERAL ($1)->data[0] < CB_LITERAL ($3)->data[0]) */
	if (literal_value (yyvsp[-2]) < literal_value (yyvsp[0])) {
		yyval = cb_build_pair (yyvsp[-2], yyvsp[0]);
	} else {
		yyval = cb_build_pair (yyvsp[0], yyvsp[-2]);
	}
  }
#line 6005 "parser.c"
    break;

  case 150: /* locale_clause: LOCALE undefined_word _is reference  */
#line 1531 "parser.y"
  {
	cb_tree	l;

	l = cb_build_locale_name (yyvsp[-2], yyvsp[0]);
	if (l != cb_error_node) {
		current_program->locale_list =
			cb_list_add (current_program->locale_list, l);
	}
  }
#line 6019 "parser.c"
    break;

  case 151: /* currency_sign_clause: CURRENCY _sign _is "Literal"  */
#line 1546 "parser.y"
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
#line 6084 "parser.c"
    break;

  case 152: /* decimal_point_clause: "DECIMAL-POINT" _is COMMA  */
#line 1613 "parser.y"
  {
	current_program->decimal_point = ',';
	current_program->numeric_separator = '.';
  }
#line 6093 "parser.c"
    break;

  case 153: /* cursor_clause: CURSOR _is reference  */
#line 1623 "parser.y"
                                { current_program->cursor_pos = yyvsp[0]; }
#line 6099 "parser.c"
    break;

  case 154: /* crt_status_clause: CRT STATUS _is reference  */
#line 1630 "parser.y"
                                { current_program->crt_status = yyvsp[0]; }
#line 6105 "parser.c"
    break;

  case 155: /* screen_control: "SCREEN-CONTROL" _is reference  */
#line 1637 "parser.y"
                                {  PENDING ("SCREEN CONTROL"); }
#line 6111 "parser.c"
    break;

  case 156: /* event_status: "EVENT-STATUS" _is reference  */
#line 1643 "parser.y"
                                {  PENDING ("EVENT STATUS"); }
#line 6117 "parser.c"
    break;

  case 159: /* $@13: %empty  */
#line 1655 "parser.y"
  {
	/* hack for MF compatibility */
	if (cb_relaxed_syntax_check) {
		cb_warning (_("INPUT-OUTPUT SECTION header missing - assumed"));
	} else {
		cb_error (_("INPUT-OUTPUT SECTION header missing"));
	}
  }
#line 6130 "parser.c"
    break;

  case 161: /* $@14: %empty  */
#line 1664 "parser.y"
  {
	/* hack for MF compatibility */
	if (cb_relaxed_syntax_check) {
		cb_warning (_("INPUT-OUTPUT SECTION header missing - assumed"));
	} else {
		cb_error (_("INPUT-OUTPUT SECTION header missing"));
	}
  }
#line 6143 "parser.c"
    break;

  case 167: /* $@15: %empty  */
#line 1689 "parser.y"
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
#line 6162 "parser.c"
    break;

  case 168: /* file_control_entry: SELECT flag_optional undefined_word $@15 select_clause_sequence '.'  */
#line 1704 "parser.y"
  {
	validate_file (current_file, yyvsp[-3]);
  }
#line 6170 "parser.c"
    break;

  case 186: /* assign_clause: ASSIGN _to _ext_clause _device assignment_name  */
#line 1736 "parser.y"
  {
	current_file->assign = cb_build_assignment_name (current_file, yyvsp[0]);
  }
#line 6178 "parser.c"
    break;

  case 187: /* assign_clause: ASSIGN _to _ext_clause DISK  */
#line 1740 "parser.y"
  {
	current_file->fileid_assign = 1;
	current_file->assign = cb_build_assignment_name (current_file, cb_build_reference ("DISK"));
  }
#line 6187 "parser.c"
    break;

  case 188: /* assign_clause: ASSIGN _to _ext_clause PRINTER  */
#line 1745 "parser.y"
  {
	current_file->fileid_assign = 1;
	current_file->assign = cb_build_assignment_name (current_file, cb_build_reference ("PRINTER"));
  }
#line 6196 "parser.c"
    break;

  case 191: /* _device: PRINTER  */
#line 1753 "parser.y"
                { current_file->organization = COB_ORG_LINE_SEQUENTIAL; }
#line 6202 "parser.c"
    break;

  case 193: /* _ext_clause: EXTERNAL  */
#line 1758 "parser.y"
  {
	current_file->external_assign = 1;
  }
#line 6210 "parser.c"
    break;

  case 194: /* _ext_clause: DYNAMIC  */
#line 1762 "parser.y"
  {
	current_file->external_assign = 0;
  }
#line 6218 "parser.c"
    break;

  case 196: /* assignment_name: DISPLAY  */
#line 1770 "parser.y"
  {
	const char	*s;

	s = "$#@DUMMY@#$";
	yyval = cb_build_alphanumeric_literal ((unsigned char *)s, strlen (s));
  }
#line 6229 "parser.c"
    break;

  case 197: /* assignment_name: _literal assignment_device_name_list  */
#line 1777 "parser.y"
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
#line 6250 "parser.c"
    break;

  case 198: /* assignment_device_name_list: qualified_word  */
#line 1796 "parser.y"
                                                { yyval = cb_list_init (yyvsp[0]); }
#line 6256 "parser.c"
    break;

  case 199: /* assignment_device_name_list: assignment_device_name_list qualified_word  */
#line 1797 "parser.y"
                                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 6262 "parser.c"
    break;

  case 201: /* access_mode: SEQUENTIAL  */
#line 1807 "parser.y"
                        { current_file->access_mode = COB_ACCESS_SEQUENTIAL; }
#line 6268 "parser.c"
    break;

  case 202: /* access_mode: DYNAMIC  */
#line 1808 "parser.y"
                        { current_file->access_mode = COB_ACCESS_DYNAMIC; }
#line 6274 "parser.c"
    break;

  case 203: /* access_mode: RANDOM  */
#line 1809 "parser.y"
                        { current_file->access_mode = COB_ACCESS_RANDOM; }
#line 6280 "parser.c"
    break;

  case 204: /* alternative_record_key_clause: ALTERNATE RECORD _key _is reference flag_duplicates  */
#line 1817 "parser.y"
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
#line 6304 "parser.c"
    break;

  case 205: /* alternative_record_key_clause: ALTERNATE RECORD _key _is reference key_is_eq split_key_list flag_duplicates  */
#line 1837 "parser.y"
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
#line 6348 "parser.c"
    break;

  case 206: /* $@16: %empty  */
#line 1879 "parser.y"
  {
	key_component_list = NULL;
  }
#line 6356 "parser.c"
    break;

  case 209: /* split_key: reference  */
#line 1888 "parser.y"
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
#line 6373 "parser.c"
    break;

  case 210: /* key_is_eq: %empty  */
#line 1903 "parser.y"
                { yyval = NULL; }
#line 6379 "parser.c"
    break;

  case 211: /* key_is_eq: SOURCE _is  */
#line 1904 "parser.y"
                { yyval = cb_int1; }
#line 6385 "parser.c"
    break;

  case 212: /* key_is_eq: '='  */
#line 1905 "parser.y"
                { yyval = cb_int('='); }
#line 6391 "parser.c"
    break;

  case 213: /* collating_sequence_clause: coll_sequence _is "Identifier"  */
#line 1912 "parser.y"
  {
	PENDING ("COLLATING SEQUENCE");
  }
#line 6399 "parser.c"
    break;

  case 214: /* file_status_clause: file_or_sort STATUS _is reference opt_reference  */
#line 1922 "parser.y"
  {
	current_file->file_status = yyvsp[-1];
	if (yyvsp[0]) {
		PENDING ("2nd FILE STATUS");
	}
  }
#line 6410 "parser.c"
    break;

  case 219: /* lock_mode: MANUAL lock_with  */
#line 1943 "parser.y"
                        { current_file->lock_mode = COB_LOCK_MANUAL; }
#line 6416 "parser.c"
    break;

  case 220: /* lock_mode: AUTOMATIC lock_with  */
#line 1944 "parser.y"
                        { current_file->lock_mode = COB_LOCK_AUTOMATIC; }
#line 6422 "parser.c"
    break;

  case 221: /* lock_mode: EXCLUSIVE  */
#line 1945 "parser.y"
                        { current_file->lock_mode = COB_LOCK_EXCLUSIVE; }
#line 6428 "parser.c"
    break;

  case 224: /* lock_with: WITH LOCK ON MULTIPLE lock_records  */
#line 1951 "parser.y"
  {
	current_file->lock_mode |= COB_LOCK_MULTIPLE;
  }
#line 6436 "parser.c"
    break;

  case 225: /* lock_with: WITH ROLLBACK  */
#line 1954 "parser.y"
                                { PENDING ("WITH ROLLBACK"); }
#line 6442 "parser.c"
    break;

  case 230: /* organization: INDEXED  */
#line 1971 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_INDEXED;
		organized_seen = 1;
	}
  }
#line 6455 "parser.c"
    break;

  case 231: /* organization: RECORD _binary SEQUENTIAL  */
#line 1980 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_SEQUENTIAL;
		organized_seen = 1;
	}
  }
#line 6468 "parser.c"
    break;

  case 232: /* organization: SEQUENTIAL  */
#line 1989 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = cb_default_organization;
		organized_seen = 1;
	}
  }
#line 6481 "parser.c"
    break;

  case 233: /* organization: RELATIVE  */
#line 1998 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_RELATIVE;
		organized_seen = 1;
	}
  }
#line 6494 "parser.c"
    break;

  case 234: /* organization: LINE SEQUENTIAL  */
#line 2007 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_LINE_SEQUENTIAL;
		organized_seen = 1;
	}
  }
#line 6507 "parser.c"
    break;

  case 235: /* padding_character_clause: PADDING _character _is reference_or_literal  */
#line 2022 "parser.y"
  {
	cb_verify (cb_padding_character_clause, "PADDING CHARACTER");
  }
#line 6515 "parser.c"
    break;

  case 236: /* record_delimiter_clause: RECORD DELIMITER _is "STANDARD-1"  */
#line 2031 "parser.y"
                                        { /* ignored */ }
#line 6521 "parser.c"
    break;

  case 237: /* record_key_clause: RECORD _key _is reference flag_duplicates  */
#line 2039 "parser.y"
  {

	if(yyvsp[0] == cb_int1) {
		cb_error (_("Record keys with duplicates are not yet supported"));
	}

	current_file->key = yyvsp[-1];
  }
#line 6534 "parser.c"
    break;

  case 238: /* record_key_clause: RECORD _key _is reference key_is_eq split_key_list flag_duplicates  */
#line 2048 "parser.y"
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
#line 6570 "parser.c"
    break;

  case 239: /* relative_key_clause: RELATIVE _key _is reference  */
#line 2085 "parser.y"
                                { current_file->key = yyvsp[0]; }
#line 6576 "parser.c"
    break;

  case 240: /* reserve_clause: RESERVE integer _area  */
#line 2092 "parser.y"
                                { /* ignored */ }
#line 6582 "parser.c"
    break;

  case 241: /* reserve_clause: RESERVE NO  */
#line 2093 "parser.y"
                                { /* ignored */ }
#line 6588 "parser.c"
    break;

  case 242: /* sharing_clause: SHARING _with sharing_option  */
#line 2100 "parser.y"
                                { current_file->sharing = yyvsp[0]; }
#line 6594 "parser.c"
    break;

  case 243: /* sharing_option: ALL _other  */
#line 2104 "parser.y"
                                { yyval = NULL; PENDING ("SHARING ALL OTHER"); }
#line 6600 "parser.c"
    break;

  case 244: /* sharing_option: NO _other  */
#line 2105 "parser.y"
                                { yyval = cb_int1; }
#line 6606 "parser.c"
    break;

  case 245: /* sharing_option: READ ONLY  */
#line 2106 "parser.y"
                                { yyval = cb_int0; }
#line 6612 "parser.c"
    break;

  case 246: /* nominal_key_clause: NOMINAL _key _is reference  */
#line 2112 "parser.y"
                                { PENDING ("NOMINAL KEY"); }
#line 6618 "parser.c"
    break;

  case 257: /* same_clause: SAME same_option _area _for file_name_list  */
#line 2143 "parser.y"
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
#line 6644 "parser.c"
    break;

  case 258: /* same_option: %empty  */
#line 2167 "parser.y"
                                { yyval = cb_int0; }
#line 6650 "parser.c"
    break;

  case 259: /* same_option: RECORD  */
#line 2168 "parser.y"
                                { yyval = cb_int1; }
#line 6656 "parser.c"
    break;

  case 260: /* same_option: SORT  */
#line 2169 "parser.y"
                                { yyval = cb_int2; }
#line 6662 "parser.c"
    break;

  case 261: /* same_option: "SORT-MERGE"  */
#line 2170 "parser.y"
                                { yyval = cb_int2; }
#line 6668 "parser.c"
    break;

  case 262: /* multiple_file_tape_clause: MULTIPLE _file _tape _contains multiple_file_list  */
#line 2177 "parser.y"
  {
	cb_verify (cb_multiple_file_tape_clause, "MULTIPLE FILE TAPE");
  }
#line 6676 "parser.c"
    break;

  case 265: /* multiple_file: file_name multiple_file_position  */
#line 2188 "parser.y"
                                   { }
#line 6682 "parser.c"
    break;

  case 271: /* apply_clause: APPLY "COMMITMENT-CONTROL" _on reference_list  */
#line 2205 "parser.y"
  {
	PENDING ("APPLY COMMITMENT-CONTROL");
  }
#line 6690 "parser.c"
    break;

  case 272: /* apply_clause: APPLY "CYL-OVERFLOW" _of "Literal" TRACKS ON reference_list  */
#line 2209 "parser.y"
  {
	PENDING ("APPLY CYL-OVERFLOW");
  }
#line 6698 "parser.c"
    break;

  case 273: /* apply_clause: APPLY "CORE-INDEX" TO reference ON reference_list  */
#line 2213 "parser.y"
  {
	PENDING ("APPLY CORE-INDEX");
  }
#line 6706 "parser.c"
    break;

  case 274: /* apply_clause: APPLY "FORMS-OVERLAY" TO reference ON reference_list  */
#line 2217 "parser.y"
  {
	PENDING ("APPLY FORMS-OVERLAY");
  }
#line 6714 "parser.c"
    break;

  case 275: /* apply_clause: APPLY "CLOSE-NOFEED" ON reference_list  */
#line 2221 "parser.y"
  {
	PENDING ("APPLY CLOSE-NOFEED");
  }
#line 6722 "parser.c"
    break;

  case 279: /* $@17: %empty  */
#line 2246 "parser.y"
                                { current_storage = CB_STORAGE_FILE; }
#line 6728 "parser.c"
    break;

  case 281: /* $@18: %empty  */
#line 2249 "parser.y"
  {
	/* hack for MF compatibility */
	if (cb_relaxed_syntax_check) {
		cb_warning (_("FILE SECTION header missing - assumed"));
	} else {
		cb_error (_("FILE SECTION header missing"));
	}
	current_storage = CB_STORAGE_FILE;
  }
#line 6742 "parser.c"
    break;

  case 285: /* file_description: file_type file_description_entry record_description_list  */
#line 2268 "parser.y"
  {
	if (yyvsp[0] && yyvsp[0] != cb_error_node) {
		finalize_file (current_file, CB_FIELD (yyvsp[0]));
	} else {
		cb_error (_("RECORD description missing or invalid"));
	}
  }
#line 6754 "parser.c"
    break;

  case 286: /* file_description_sequence_without_type: file_description_entry record_description_list  */
#line 2280 "parser.y"
  {
	if (yyvsp[0] && yyvsp[0] != cb_error_node) {
		finalize_file (current_file, CB_FIELD (yyvsp[0]));
	} else {
		cb_error (_("RECORD description missing or invalid"));
	}
  }
#line 6766 "parser.c"
    break;

  case 288: /* file_type: FD  */
#line 2291 "parser.y"
                               { yyval = cb_int0; }
#line 6772 "parser.c"
    break;

  case 289: /* file_type: SD  */
#line 2292 "parser.y"
                               { yyval = cb_int1; }
#line 6778 "parser.c"
    break;

  case 290: /* @19: %empty  */
#line 2302 "parser.y"
  {
	if (yyvsp[0] == cb_error_node) {
		YYERROR;
	}

	current_file = CB_FILE (cb_ref (yyvsp[0]));
	if (yyvsp[-1] == cb_int1) {
		current_file->organization = COB_ORG_SORT;
	}
  }
#line 6793 "parser.c"
    break;

  case 291: /* file_description_entry: file_name @19 file_description_clause_sequence '.'  */
#line 2313 "parser.y"
  {
	/* Shut up bison */
	dummy_tree = yyvsp[-2];
  }
#line 6802 "parser.c"
    break;

  case 294: /* file_description_clause: _is EXTERNAL  */
#line 2325 "parser.y"
  {
	if (current_file->global) {
		cb_error (_("File cannot have both EXTERNAL and GLOBAL clauses"));
	}
	current_file->external = 1;
  }
#line 6813 "parser.c"
    break;

  case 295: /* file_description_clause: _is GLOBAL  */
#line 2332 "parser.y"
  {
	if (current_file->external) {
		cb_error (_("File cannot have both EXTERNAL and GLOBAL clauses"));
	}
	current_file->global = 1;
  }
#line 6824 "parser.c"
    break;

  case 306: /* block_contains_clause: BLOCK _contains integer opt_to_integer _records_or_characters  */
#line 2355 "parser.y"
  { /* ignored */ }
#line 6830 "parser.c"
    break;

  case 310: /* record_clause: RECORD _contains integer _characters  */
#line 2365 "parser.y"
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
#line 6846 "parser.c"
    break;

  case 311: /* record_clause: RECORD _contains integer TO integer _characters  */
#line 2377 "parser.y"
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
#line 6875 "parser.c"
    break;

  case 312: /* record_clause: RECORD _is VARYING _in _size opt_from_integer opt_to_integer _characters record_depending  */
#line 2403 "parser.y"
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
#line 6900 "parser.c"
    break;

  case 314: /* record_depending: DEPENDING _on reference  */
#line 2427 "parser.y"
  {
	current_file->record_depending = yyvsp[0];
  }
#line 6908 "parser.c"
    break;

  case 315: /* opt_from_integer: %empty  */
#line 2433 "parser.y"
                                { yyval = NULL; }
#line 6914 "parser.c"
    break;

  case 316: /* opt_from_integer: _from integer  */
#line 2434 "parser.y"
                                { yyval = yyvsp[0]; }
#line 6920 "parser.c"
    break;

  case 317: /* opt_to_integer: %empty  */
#line 2438 "parser.y"
                                { yyval = NULL; }
#line 6926 "parser.c"
    break;

  case 318: /* opt_to_integer: TO integer  */
#line 2439 "parser.y"
                                { yyval = yyvsp[0]; }
#line 6932 "parser.c"
    break;

  case 319: /* label_records_clause: LABEL records label_option  */
#line 2447 "parser.y"
  {
	cb_verify (cb_label_records_clause, "LABEL RECORDS");
  }
#line 6940 "parser.c"
    break;

  case 322: /* value_of_clause: VALUE OF "Identifier" _is valueof_name  */
#line 2462 "parser.y"
  {
	cb_verify (cb_value_of_clause, "VALUE OF");
  }
#line 6948 "parser.c"
    break;

  case 323: /* value_of_clause: VALUE OF "FILE-ID" _is valueof_name  */
#line 2466 "parser.y"
  {
	if (!current_file->assign) {
		current_file->assign = cb_build_assignment_name (current_file, yyvsp[0]);
	}
  }
#line 6958 "parser.c"
    break;

  case 326: /* data_records_clause: DATA records no_reference_list  */
#line 2482 "parser.y"
  {
	cb_verify (cb_data_records_clause, "DATA RECORDS");
  }
#line 6966 "parser.c"
    break;

  case 327: /* linage_clause: LINAGE _is reference_or_literal _lines linage_sequence  */
#line 2493 "parser.y"
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
#line 6985 "parser.c"
    break;

  case 333: /* linage_footing: _with FOOTING _at reference_or_literal _lines  */
#line 2521 "parser.y"
  {
	current_file->latfoot = yyvsp[-1];
  }
#line 6993 "parser.c"
    break;

  case 334: /* linage_top: _at TOP reference_or_literal _lines  */
#line 2528 "parser.y"
  {
	current_file->lattop = yyvsp[-1];
  }
#line 7001 "parser.c"
    break;

  case 335: /* linage_bottom: _at BOTTOM reference_or_literal  */
#line 2535 "parser.y"
  {
	current_file->latbot = yyvsp[0];
  }
#line 7009 "parser.c"
    break;

  case 336: /* recording_mode_clause: RECORDING _mode _is "Identifier"  */
#line 2544 "parser.y"
                                { /* ignore */ }
#line 7015 "parser.c"
    break;

  case 337: /* code_set_clause: "CODE-SET" _is "Identifier"  */
#line 2552 "parser.y"
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
#line 7032 "parser.c"
    break;

  case 338: /* report_clause: REPORT _is report_name  */
#line 2570 "parser.y"
  {
	cb_warning (_("file descriptor REPORT IS"));
  }
#line 7040 "parser.c"
    break;

  case 339: /* report_clause: REPORTS _are report_name  */
#line 2574 "parser.y"
  {
	cb_warning (_("file descriptor REPORTS ARE"));
  }
#line 7048 "parser.c"
    break;

  case 341: /* $@20: %empty  */
#line 2585 "parser.y"
                                { current_storage = CB_STORAGE_WORKING; }
#line 7054 "parser.c"
    break;

  case 342: /* working_storage_section: "WORKING-STORAGE" SECTION '.' $@20 record_description_list  */
#line 2587 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->working_storage =
			cb_field_add (current_program->working_storage, CB_FIELD (yyvsp[0]));
	}
  }
#line 7065 "parser.c"
    break;

  case 343: /* record_description_list: %empty  */
#line 2596 "parser.y"
                                { yyval = NULL; }
#line 7071 "parser.c"
    break;

  case 344: /* record_description_list: record_description_list_1  */
#line 2597 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7077 "parser.c"
    break;

  case 345: /* $@21: %empty  */
#line 2601 "parser.y"
  {
	current_field = NULL;
	description_field = NULL;
	cb_clear_real_field ();
  }
#line 7087 "parser.c"
    break;

  case 346: /* record_description_list_1: $@21 record_description_list_2  */
#line 2607 "parser.y"
  {
	struct cb_field *p;

	for (p = description_field; p; p = p->sister) {
		cb_validate_field (p);
	}
	yyval = CB_TREE (description_field);
  }
#line 7100 "parser.c"
    break;

  case 356: /* exec_sql_data_statement: "EXEC SQL statement"  */
#line 2632 "parser.y"
  {
	/* DATA DIVISION に渡ってきた EXEC SQL。INCLUDE SQLCA や DECLARE SECTION は
	   前処理 (pplex) 側で消費されるため通常ここには来ないが、その他の EXEC SQL
	   が来た場合に備えて受け取り (実行コードは生成しない)。 */
	(void) CB_LITERAL (yyvsp[0])->data;
  }
#line 7111 "parser.c"
    break;

  case 357: /* exec_sql_data_statement: "EXEC SQL statement" '.'  */
#line 2639 "parser.y"
  {
	(void) CB_LITERAL (yyvsp[-1])->data;
  }
#line 7119 "parser.c"
    break;

  case 358: /* exec_java_import_data_statement: "EXEC JAVA IMPORT statement"  */
#line 2646 "parser.y"
  {
	cb_add_exec_java_import (yyvsp[0]);
  }
#line 7127 "parser.c"
    break;

  case 359: /* exec_java_import_data_statement: "EXEC JAVA IMPORT statement" '.'  */
#line 2650 "parser.y"
  {
	cb_add_exec_java_import (yyvsp[-1]);
  }
#line 7135 "parser.c"
    break;

  case 360: /* exec_java_member_data_statement: "EXEC JAVA CLASS-MEMBER statement"  */
#line 2657 "parser.y"
  {
	cb_add_exec_java_member (yyvsp[0]);
  }
#line 7143 "parser.c"
    break;

  case 361: /* exec_java_member_data_statement: "EXEC JAVA CLASS-MEMBER statement" '.'  */
#line 2661 "parser.y"
  {
	cb_add_exec_java_member (yyvsp[-1]);
  }
#line 7151 "parser.c"
    break;

  case 363: /* $@22: %empty  */
#line 2669 "parser.y"
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
#line 7167 "parser.c"
    break;

  case 364: /* data_description: level_number entry_name $@22 data_description_clause_sequence _maybe_next_level_number  */
#line 2681 "parser.y"
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
#line 7184 "parser.c"
    break;

  case 365: /* $@23: %empty  */
#line 2694 "parser.y"
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
#line 7200 "parser.c"
    break;

  case 366: /* data_description: level_number_88 entry_name $@23 value_cond_clause  */
#line 2706 "parser.y"
  {
	if (!qualifier) {
		cb_error (_("Item requires a data name"));
	}
	cb_validate_88_item (current_field);
	if (!description_field) {
		description_field = current_field;
	}
	
  }
#line 7215 "parser.c"
    break;

  case 370: /* _maybe_next_level_number: "Literal"  */
#line 2733 "parser.y"
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
#line 7237 "parser.c"
    break;

  case 371: /* entry_name: %empty  */
#line 2754 "parser.y"
  {
	yyval = cb_build_filler ();
	qualifier = NULL;
	non_const_word = 0;
  }
#line 7247 "parser.c"
    break;

  case 372: /* entry_name: FILLER  */
#line 2760 "parser.y"
  {
	yyval = cb_build_filler ();
	qualifier = NULL;
	non_const_word = 0;
  }
#line 7257 "parser.c"
    break;

  case 373: /* entry_name: "Identifier"  */
#line 2766 "parser.y"
  {
	yyval = yyvsp[0];
	qualifier = yyvsp[0];
	non_const_word = 0;
  }
#line 7267 "parser.c"
    break;

  case 374: /* const_name: "Identifier"  */
#line 2775 "parser.y"
  {
	yyval = yyvsp[0];
	qualifier = yyvsp[0];
	non_const_word = 0;
  }
#line 7277 "parser.c"
    break;

  case 376: /* const_global: _is GLOBAL  */
#line 2784 "parser.y"
  {
	current_field->flag_is_global = 1;
	cb_error (_("CONSTANT with GLOBAL clause is not yet supported"));
  }
#line 7286 "parser.c"
    break;

  case 377: /* lit_or_length: literal  */
#line 2791 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7292 "parser.c"
    break;

  case 378: /* lit_or_length: LENGTH _of identifier_1  */
#line 2792 "parser.y"
                                { yyval = cb_build_const_length (yyvsp[0]); }
#line 7298 "parser.c"
    break;

  case 379: /* lit_or_length: "BYTE-LENGTH" _of identifier_1  */
#line 2793 "parser.y"
                                { yyval = cb_build_const_length (yyvsp[0]); }
#line 7304 "parser.c"
    break;

  case 380: /* constant_entry: level_number const_name CONSTANT const_global _as lit_or_length  */
#line 2798 "parser.y"
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
#line 7324 "parser.c"
    break;

  case 381: /* data_description_clause_sequence: %empty  */
#line 2817 "parser.y"
  {
	/* required to check redefines */
	yyval = NULL;
  }
#line 7333 "parser.c"
    break;

  case 382: /* data_description_clause_sequence: data_description_clause_sequence data_description_clause  */
#line 2823 "parser.y"
  {
	/* required to check redefines */
	yyval = cb_true;
  }
#line 7342 "parser.c"
    break;

  case 399: /* redefines_clause: REDEFINES identifier_1  */
#line 2853 "parser.y"
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
#line 7362 "parser.c"
    break;

  case 400: /* external_clause: _is EXTERNAL as_extname  */
#line 2875 "parser.y"
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
#line 7385 "parser.c"
    break;

  case 401: /* as_extname: %empty  */
#line 2896 "parser.y"
                                { current_field->ename = NULL; }
#line 7391 "parser.c"
    break;

  case 402: /* as_extname: AS "Literal"  */
#line 2898 "parser.y"
 {
	struct cb_field *x;

	x = CB_FIELD(cb_build_field (cb_build_reference ((char *)(CB_LITERAL (yyvsp[0])->data))));
	current_field->ename = x->name;
 }
#line 7402 "parser.c"
    break;

  case 403: /* global_clause: _is GLOBAL  */
#line 2910 "parser.y"
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
#line 7420 "parser.c"
    break;

  case 404: /* picture_clause: PICTURE  */
#line 2929 "parser.y"
                                { current_field->pic = CB_PICTURE (yyvsp[0]); }
#line 7426 "parser.c"
    break;

  case 407: /* usage: BINARY  */
#line 2941 "parser.y"
                                { current_field->usage = CB_USAGE_BINARY; }
#line 7432 "parser.c"
    break;

  case 408: /* usage: COMP  */
#line 2942 "parser.y"
                                { current_field->usage = CB_USAGE_BINARY; }
#line 7438 "parser.c"
    break;

  case 409: /* usage: "COMP-1"  */
#line 2944 "parser.y"
{
	current_field->usage = CB_USAGE_FLOAT; 
	cb_error (_("COMP-1 not implemented"));
}
#line 7447 "parser.c"
    break;

  case 410: /* usage: "COMP-2"  */
#line 2949 "parser.y"
{ 
	current_field->usage = CB_USAGE_DOUBLE; 
	cb_error (_("COMP-2 not implemented"));
}
#line 7456 "parser.c"
    break;

  case 411: /* usage: "COMP-3"  */
#line 2953 "parser.y"
                                { current_field->usage = CB_USAGE_PACKED; }
#line 7462 "parser.c"
    break;

  case 412: /* usage: "COMP-4"  */
#line 2954 "parser.y"
                                { current_field->usage = CB_USAGE_BINARY; }
#line 7468 "parser.c"
    break;

  case 413: /* usage: "COMP-5"  */
#line 2955 "parser.y"
                                { current_field->usage = CB_USAGE_COMP_5; }
#line 7474 "parser.c"
    break;

  case 414: /* usage: "COMP-X"  */
#line 2956 "parser.y"
                                { current_field->usage = CB_USAGE_COMP_X; }
#line 7480 "parser.c"
    break;

  case 415: /* usage: DISPLAY  */
#line 2957 "parser.y"
                                { current_field->usage = CB_USAGE_DISPLAY; }
#line 7486 "parser.c"
    break;

  case 416: /* usage: INDEX  */
#line 2958 "parser.y"
                                { current_field->usage = CB_USAGE_INDEX; }
#line 7492 "parser.c"
    break;

  case 417: /* usage: "PACKED-DECIMAL"  */
#line 2959 "parser.y"
                                { current_field->usage = CB_USAGE_PACKED; }
#line 7498 "parser.c"
    break;

  case 418: /* usage: POINTER  */
#line 2961 "parser.y"
  {
	current_field->usage = CB_USAGE_POINTER;
	current_field->flag_is_pointer = 1;
  }
#line 7507 "parser.c"
    break;

  case 419: /* usage: "PROGRAM-POINTER"  */
#line 2966 "parser.y"
  {
	current_field->usage = CB_USAGE_PROGRAM_POINTER;
	current_field->flag_is_pointer = 1;
  }
#line 7516 "parser.c"
    break;

  case 420: /* usage: "SIGNED-SHORT"  */
#line 2970 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_SHORT; }
#line 7522 "parser.c"
    break;

  case 421: /* usage: "SIGNED-INT"  */
#line 2971 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_INT; }
#line 7528 "parser.c"
    break;

  case 422: /* usage: "SIGNED-LONG"  */
#line 2972 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_LONG; }
#line 7534 "parser.c"
    break;

  case 423: /* usage: "UNSIGNED-SHORT"  */
#line 2973 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_SHORT; }
#line 7540 "parser.c"
    break;

  case 424: /* usage: "UNSIGNED-INT"  */
#line 2974 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_INT; }
#line 7546 "parser.c"
    break;

  case 425: /* usage: "UNSIGNED-LONG"  */
#line 2975 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_LONG; }
#line 7552 "parser.c"
    break;

  case 426: /* usage: "BINARY-CHAR" SIGNED  */
#line 2976 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_CHAR; }
#line 7558 "parser.c"
    break;

  case 427: /* usage: "BINARY-CHAR" UNSIGNED  */
#line 2977 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_CHAR; }
#line 7564 "parser.c"
    break;

  case 428: /* usage: "BINARY-CHAR"  */
#line 2978 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_CHAR; }
#line 7570 "parser.c"
    break;

  case 429: /* usage: "BINARY-SHORT" SIGNED  */
#line 2979 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_SHORT; }
#line 7576 "parser.c"
    break;

  case 430: /* usage: "BINARY-SHORT" UNSIGNED  */
#line 2980 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_SHORT; }
#line 7582 "parser.c"
    break;

  case 431: /* usage: "BINARY-SHORT"  */
#line 2981 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_SHORT; }
#line 7588 "parser.c"
    break;

  case 432: /* usage: "BINARY-LONG" SIGNED  */
#line 2982 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_INT; }
#line 7594 "parser.c"
    break;

  case 433: /* usage: "BINARY-LONG" UNSIGNED  */
#line 2983 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_INT; }
#line 7600 "parser.c"
    break;

  case 434: /* usage: "BINARY-LONG"  */
#line 2984 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_INT; }
#line 7606 "parser.c"
    break;

  case 435: /* usage: "BINARY-DOUBLE" SIGNED  */
#line 2985 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_LONG; }
#line 7612 "parser.c"
    break;

  case 436: /* usage: "BINARY-DOUBLE" UNSIGNED  */
#line 2986 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_LONG; }
#line 7618 "parser.c"
    break;

  case 437: /* usage: "BINARY-DOUBLE"  */
#line 2987 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_LONG; }
#line 7624 "parser.c"
    break;

  case 438: /* usage: "BINARY-C-LONG" SIGNED  */
#line 2989 "parser.y"
  {
	if (sizeof(long) == 4) {
		current_field->usage = CB_USAGE_SIGNED_INT;
	} else {
		current_field->usage = CB_USAGE_SIGNED_LONG;
	}
  }
#line 7636 "parser.c"
    break;

  case 439: /* usage: "BINARY-C-LONG" UNSIGNED  */
#line 2997 "parser.y"
  {
	if (sizeof(long) == 4) {
		current_field->usage = CB_USAGE_UNSIGNED_INT;
	} else {
		current_field->usage = CB_USAGE_UNSIGNED_LONG;
	}
  }
#line 7648 "parser.c"
    break;

  case 440: /* usage: "BINARY-C-LONG"  */
#line 3005 "parser.y"
  {
	if (sizeof(long) == 4) {
		current_field->usage = CB_USAGE_SIGNED_INT;
	} else {
		current_field->usage = CB_USAGE_SIGNED_LONG;
	}
  }
#line 7660 "parser.c"
    break;

  case 441: /* usage: NATIONAL  */
#line 3012 "parser.y"
                                { PENDING ("USAGE NATIONAL");}
#line 7666 "parser.c"
    break;

  case 442: /* sign_clause: _sign_is LEADING flag_separate  */
#line 3020 "parser.y"
  {
	current_field->flag_sign_separate = CB_INTEGER (yyvsp[0])->val;
	current_field->flag_sign_leading  = 1;
  }
#line 7675 "parser.c"
    break;

  case 443: /* sign_clause: _sign_is TRAILING flag_separate  */
#line 3025 "parser.y"
  {
	current_field->flag_sign_separate = CB_INTEGER (yyvsp[0])->val;
	current_field->flag_sign_leading  = 0;
  }
#line 7684 "parser.c"
    break;

  case 447: /* occurs_clause: OCCURS integer occurs_to_integer _times occurs_depending occurs_key_spec  */
#line 3042 "parser.y"
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
#line 7701 "parser.c"
    break;

  case 448: /* occurs_to_integer: %empty  */
#line 3057 "parser.y"
                                { yyval = NULL; }
#line 7707 "parser.c"
    break;

  case 449: /* occurs_to_integer: TO integer  */
#line 3058 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7713 "parser.c"
    break;

  case 451: /* occurs_depending: DEPENDING _on reference  */
#line 3063 "parser.y"
  {
	current_field->occurs_depending = yyvsp[0];
  }
#line 7721 "parser.c"
    break;

  case 454: /* occurs_keys: occurs_key_list  */
#line 3072 "parser.y"
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
#line 7746 "parser.c"
    break;

  case 455: /* occurs_key: ascending_or_descending _key _is reference_list  */
#line 3096 "parser.y"
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
#line 7763 "parser.c"
    break;

  case 456: /* occurs_key_list: occurs_key  */
#line 3111 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7769 "parser.c"
    break;

  case 457: /* occurs_key_list: occurs_key_list occurs_key  */
#line 3112 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 7775 "parser.c"
    break;

  case 458: /* ascending_or_descending: ASCENDING  */
#line 3116 "parser.y"
                                { yyval = cb_int (COB_ASCENDING); }
#line 7781 "parser.c"
    break;

  case 459: /* ascending_or_descending: DESCENDING  */
#line 3117 "parser.y"
                                { yyval = cb_int (COB_DESCENDING); }
#line 7787 "parser.c"
    break;

  case 462: /* occurs_indexed: INDEXED _by occurs_index_list  */
#line 3124 "parser.y"
  {
	current_field->index_list = yyvsp[0];
  }
#line 7795 "parser.c"
    break;

  case 463: /* occurs_index_list: occurs_index  */
#line 3130 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 7801 "parser.c"
    break;

  case 464: /* occurs_index_list: occurs_index_list occurs_index  */
#line 3132 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 7807 "parser.c"
    break;

  case 465: /* occurs_index: "Identifier"  */
#line 3137 "parser.y"
  {
	yyval = cb_build_index (yyvsp[0], cb_int1, 1, current_field);
  }
#line 7815 "parser.c"
    break;

  case 466: /* justified_clause: JUSTIFIED _right  */
#line 3146 "parser.y"
                                { current_field->flag_justified = 1; }
#line 7821 "parser.c"
    break;

  case 467: /* synchronized_clause: SYNCHRONIZED left_or_right  */
#line 3153 "parser.y"
                                { current_field->flag_synchronized = 1; }
#line 7827 "parser.c"
    break;

  case 471: /* blank_clause: BLANK _when ZERO  */
#line 3165 "parser.y"
                                { current_field->flag_blank_zero = 1; }
#line 7833 "parser.c"
    break;

  case 472: /* based_clause: BASED  */
#line 3173 "parser.y"
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
#line 7857 "parser.c"
    break;

  case 473: /* value_clause: VALUE _is literal  */
#line 3197 "parser.y"
                                { current_field->values = cb_list_init (yyvsp[0]); }
#line 7863 "parser.c"
    break;

  case 474: /* $@24: %empty  */
#line 3201 "parser.y"
                                { current_field->values = yyvsp[0]; }
#line 7869 "parser.c"
    break;

  case 476: /* value_item_list: value_item  */
#line 3206 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 7875 "parser.c"
    break;

  case 477: /* value_item_list: value_item_list value_item  */
#line 3207 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 7881 "parser.c"
    break;

  case 478: /* value_item: literal  */
#line 3211 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7887 "parser.c"
    break;

  case 479: /* value_item: literal THRU literal  */
#line 3212 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-2], yyvsp[0]); }
#line 7893 "parser.c"
    break;

  case 481: /* false_is: "FALSE" _is literal  */
#line 3217 "parser.y"
  {
	if (current_field->level != 88) {
		cb_error (_("FALSE clause only allowed for 88 level"));
	}
	current_field->false_88 = cb_list_init (yyvsp[0]);
  }
#line 7904 "parser.c"
    break;

  case 482: /* renames_clause: RENAMES qualified_word  */
#line 3230 "parser.y"
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
#line 7920 "parser.c"
    break;

  case 483: /* renames_clause: RENAMES qualified_word THRU qualified_word  */
#line 3242 "parser.y"
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
#line 7939 "parser.c"
    break;

  case 484: /* any_length_clause: ANY LENGTH  */
#line 3262 "parser.y"
  {
	if (current_field->flag_item_based) {
		cb_error (_("BASED and ANY LENGTH are mutually exclusive"));
	} else {
		current_field->flag_any_length = 1;
	}
  }
#line 7951 "parser.c"
    break;

  case 485: /* varying_clause: VARYING  */
#line 3275 "parser.y"
  {
	current_field->flag_varying = 1;
  }
#line 7959 "parser.c"
    break;

  case 487: /* $@25: %empty  */
#line 3286 "parser.y"
  {
	cb_error (_("LOCAL-STORAGE SECTION is not supported"));
	current_storage = CB_STORAGE_LOCAL;
	if (current_program->nested_level) {
		cb_error (_("LOCAL-STORAGE not allowed in nested programs"));
	}
  }
#line 7971 "parser.c"
    break;

  case 488: /* local_storage_section: "LOCAL-STORAGE" SECTION '.' $@25 record_description_list  */
#line 3294 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->local_storage = CB_FIELD (yyvsp[0]);
	}
  }
#line 7981 "parser.c"
    break;

  case 490: /* $@26: %empty  */
#line 3307 "parser.y"
                                { current_storage = CB_STORAGE_LINKAGE; }
#line 7987 "parser.c"
    break;

  case 491: /* linkage_section: LINKAGE SECTION '.' $@26 record_description_list  */
#line 3309 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->linkage_storage = CB_FIELD (yyvsp[0]);
	}
  }
#line 7997 "parser.c"
    break;

  case 493: /* $@27: %empty  */
#line 3322 "parser.y"
  {
	cb_error (_("REPORT SECTION not supported"));
	current_storage = CB_STORAGE_REPORT;
  }
#line 8006 "parser.c"
    break;

  case 500: /* report_description_options: %empty  */
#line 3355 "parser.y"
  {
	cb_warning (_("Report description using defaults"));
  }
#line 8014 "parser.c"
    break;

  case 502: /* report_description_option: _is GLOBAL  */
#line 3363 "parser.y"
  {
	cb_error (_("GLOBAL is not allowed with RD"));
  }
#line 8022 "parser.c"
    break;

  case 511: /* identifier_list: identifier  */
#line 3386 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 8028 "parser.c"
    break;

  case 512: /* identifier_list: identifier_list identifier  */
#line 3387 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 8034 "parser.c"
    break;

  case 534: /* report_group_option: type_clause  */
#line 3443 "parser.y"
              { cb_warning (_("looking for Report line TYPE")); }
#line 8040 "parser.c"
    break;

  case 585: /* $@28: %empty  */
#line 3548 "parser.y"
                                { current_storage = CB_STORAGE_SCREEN; }
#line 8046 "parser.c"
    break;

  case 586: /* screen_section: SCREEN SECTION '.' $@28  */
#line 3549 "parser.y"
  {
	cb_error (_("SCREEN SECTION is not supported"));
  }
#line 8054 "parser.c"
    break;

  case 588: /* $@29: %empty  */
#line 3560 "parser.y"
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
#line 8072 "parser.c"
    break;

  case 589: /* $@30: %empty  */
#line 3574 "parser.y"
  {
	if (current_program->flag_main && !current_program->flag_chained && yyvsp[-4]) {
		cb_error (_("Executable program requested but PROCEDURE/ENTRY has USING clause"));
	}
	emit_entry (current_program->program_id, 0, yyvsp[-4]); /* main entry point */
	if (current_program->source_name) {
		emit_entry (current_program->source_name, 1, yyvsp[-4]);
	}
  }
#line 8086 "parser.c"
    break;

  case 590: /* procedure_division: PROCEDURE DIVISION procedure_using_chaining procedure_returning '.' $@29 procedure_declaratives $@30 procedure_list  */
#line 3584 "parser.y"
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
#line 8105 "parser.c"
    break;

  case 591: /* procedure_using_chaining: %empty  */
#line 3601 "parser.y"
                                { yyval = NULL; }
#line 8111 "parser.c"
    break;

  case 592: /* $@31: %empty  */
#line 3603 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
	size_mode = CB_SIZE_4;
  }
#line 8120 "parser.c"
    break;

  case 593: /* procedure_using_chaining: USING $@31 procedure_param_list  */
#line 3607 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8126 "parser.c"
    break;

  case 594: /* $@32: %empty  */
#line 3609 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
	current_program->flag_chained = 1;
  }
#line 8135 "parser.c"
    break;

  case 595: /* procedure_using_chaining: CHAINING $@32 procedure_param_list  */
#line 3613 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8141 "parser.c"
    break;

  case 596: /* procedure_param_list: procedure_param  */
#line 3617 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8147 "parser.c"
    break;

  case 597: /* procedure_param_list: procedure_param_list procedure_param  */
#line 3619 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 8153 "parser.c"
    break;

  case 598: /* procedure_param: procedure_type size_optional procedure_optional "Identifier"  */
#line 3624 "parser.y"
  {
	yyval = cb_build_pair (cb_int (call_mode), cb_build_identifier (yyvsp[0]));
	CB_SIZES (yyval) = size_mode;
  }
#line 8162 "parser.c"
    break;

  case 600: /* procedure_type: _by REFERENCE  */
#line 3633 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
  }
#line 8170 "parser.c"
    break;

  case 601: /* procedure_type: _by VALUE  */
#line 3637 "parser.y"
  {
	if (current_program->flag_chained) {
		cb_error (_("BY VALUE not allowed in CHAINED program"));
	} else {
		call_mode = CB_CALL_BY_VALUE;
	}
  }
#line 8182 "parser.c"
    break;

  case 603: /* size_optional: SIZE _is AUTO  */
#line 3649 "parser.y"
  {
	if (call_mode != CB_CALL_BY_VALUE) {
		cb_error (_("SIZE only allowed for BY VALUE items"));
	} else {
		size_mode = CB_SIZE_AUTO;
	}
  }
#line 8194 "parser.c"
    break;

  case 604: /* size_optional: SIZE _is DEFAULT  */
#line 3657 "parser.y"
  {
	if (call_mode != CB_CALL_BY_VALUE) {
		cb_error (_("SIZE only allowed for BY VALUE items"));
	} else {
		size_mode = CB_SIZE_4;
	}
  }
#line 8206 "parser.c"
    break;

  case 605: /* size_optional: UNSIGNED SIZE _is integer  */
#line 3665 "parser.y"
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
#line 8239 "parser.c"
    break;

  case 606: /* size_optional: SIZE _is integer  */
#line 3694 "parser.y"
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
#line 8272 "parser.c"
    break;

  case 608: /* procedure_optional: OPTIONAL  */
#line 3727 "parser.y"
  {
	if (call_mode != CB_CALL_BY_REFERENCE) {
		cb_error (_("OPTIONAL only allowed for BY REFERENCE items"));
	}
  }
#line 8282 "parser.c"
    break;

  case 609: /* procedure_returning: %empty  */
#line 3736 "parser.y"
  {
	if (current_program->prog_type == CB_FUNCTION_TYPE) {
		cb_error (_("RETURNING clause is required for a FUNCTION"));
	}
  }
#line 8292 "parser.c"
    break;

  case 610: /* procedure_returning: RETURNING "Identifier"  */
#line 3742 "parser.y"
  {
	if (cb_ref (yyvsp[0]) != cb_error_node) {
		current_program->returning = yyvsp[0];
		if (cb_field (yyvsp[0])->storage != CB_STORAGE_LINKAGE) {
			cb_error (_("RETURNING item is not defined in LINKAGE SECTION"));
		}
	}
  }
#line 8305 "parser.c"
    break;

  case 612: /* $@33: %empty  */
#line 3753 "parser.y"
                        { in_declaratives = 1; }
#line 8311 "parser.c"
    break;

  case 613: /* procedure_declaratives: DECLARATIVES '.' $@33 procedure_list END DECLARATIVES '.'  */
#line 3756 "parser.y"
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
#line 8333 "parser.c"
    break;

  case 619: /* procedure: statements '.'  */
#line 3789 "parser.y"
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
#line 8353 "parser.c"
    break;

  case 620: /* procedure: error  */
#line 3805 "parser.y"
  {
	check_unreached = 0;
  }
#line 8361 "parser.c"
    break;

  case 621: /* section_header: section_name SECTION opt_segment '.'  */
#line 3817 "parser.y"
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
#line 8393 "parser.c"
    break;

  case 622: /* paragraph_header: "Identifier" '.'  */
#line 3848 "parser.y"
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
#line 8431 "parser.c"
    break;

  case 623: /* invalid_statement: section_name  */
#line 3885 "parser.y"
  {
	non_const_word = 0;
	check_unreached = 0;
	if (yyvsp[0] != cb_error_node) {
		cb_error_x (yyvsp[0], _("Unknown statement '%s'"), CB_NAME (yyvsp[0]));
	}
	YYERROR;
  }
#line 8444 "parser.c"
    break;

  case 624: /* section_name: "Identifier"  */
#line 3896 "parser.y"
                                { yyval = cb_build_section_name (yyvsp[0], 0); }
#line 8450 "parser.c"
    break;

  case 626: /* opt_segment: "Literal"  */
#line 3900 "parser.y"
                                { /* ignore */ }
#line 8456 "parser.c"
    break;

  case 627: /* @34: %empty  */
#line 3909 "parser.y"
  {
	yyval = current_program->exec_list;
	current_program->exec_list = NULL;
  }
#line 8465 "parser.c"
    break;

  case 628: /* @35: %empty  */
#line 3913 "parser.y"
  {
	yyval = CB_TREE (current_statement);
	current_statement = NULL;
  }
#line 8474 "parser.c"
    break;

  case 629: /* statement_list: @34 @35 statements  */
#line 3918 "parser.y"
  {
	yyval = cb_list_reverse (current_program->exec_list);
	current_program->exec_list = yyvsp[-2];
	current_statement = CB_STATEMENT (yyvsp[-1]);
  }
#line 8484 "parser.c"
    break;

  case 630: /* statements: %empty  */
#line 3926 "parser.y"
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
#line 8509 "parser.c"
    break;

  case 685: /* statement: "NEXT SENTENCE"  */
#line 4005 "parser.y"
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
#line 8527 "parser.c"
    break;

  case 686: /* $@36: %empty  */
#line 4027 "parser.y"
  {
	BEGIN_STATEMENT ("ACCEPT", TERM_ACCEPT);
	dispattrs = 0;
	fgc = NULL;
	bgc = NULL;
	scroll = NULL;
  }
#line 8539 "parser.c"
    break;

  case 688: /* accept_body: identifier opt_at_line_column opt_accp_attr on_accp_exception  */
#line 4040 "parser.y"
  {
	cb_emit_accept (yyvsp[-3], yyvsp[-2], fgc, bgc, scroll, dispattrs);
  }
#line 8547 "parser.c"
    break;

  case 689: /* accept_body: identifier FROM ESCAPE KEY  */
#line 4044 "parser.y"
  {
	PENDING ("ACCEPT .. FROM ESCAPE KEY");
  }
#line 8555 "parser.c"
    break;

  case 690: /* accept_body: identifier FROM LINES  */
#line 4048 "parser.y"
  {
	cb_emit_accept_line_or_col (yyvsp[-2], 0);
  }
#line 8563 "parser.c"
    break;

  case 691: /* accept_body: identifier FROM COLUMNS  */
#line 4052 "parser.y"
  {
	cb_emit_accept_line_or_col (yyvsp[-2], 1);
  }
#line 8571 "parser.c"
    break;

  case 692: /* accept_body: identifier FROM DATE  */
#line 4056 "parser.y"
  {
	cb_emit_accept_date (yyvsp[-2]);
  }
#line 8579 "parser.c"
    break;

  case 693: /* accept_body: identifier FROM DATE YYYYMMDD  */
#line 4060 "parser.y"
  {
	cb_emit_accept_date_yyyymmdd (yyvsp[-3]);
  }
#line 8587 "parser.c"
    break;

  case 694: /* accept_body: identifier FROM DAY  */
#line 4064 "parser.y"
  {
	cb_emit_accept_day (yyvsp[-2]);
  }
#line 8595 "parser.c"
    break;

  case 695: /* accept_body: identifier FROM DAY YYYYDDD  */
#line 4068 "parser.y"
  {
	cb_emit_accept_day_yyyyddd (yyvsp[-3]);
  }
#line 8603 "parser.c"
    break;

  case 696: /* accept_body: identifier FROM "DAY-OF-WEEK"  */
#line 4072 "parser.y"
  {
	cb_emit_accept_day_of_week (yyvsp[-2]);
  }
#line 8611 "parser.c"
    break;

  case 697: /* accept_body: identifier FROM TIME  */
#line 4076 "parser.y"
  {
	cb_emit_accept_time (yyvsp[-2]);
  }
#line 8619 "parser.c"
    break;

  case 698: /* accept_body: identifier FROM "COMMAND-LINE"  */
#line 4080 "parser.y"
  {
	cb_emit_accept_command_line (yyvsp[-2]);
  }
#line 8627 "parser.c"
    break;

  case 699: /* accept_body: identifier FROM "ENVIRONMENT-VALUE" on_accp_exception  */
#line 4084 "parser.y"
  {
	cb_emit_accept_environment (yyvsp[-3]);
  }
#line 8635 "parser.c"
    break;

  case 700: /* accept_body: identifier FROM ENVIRONMENT simple_value on_accp_exception  */
#line 4088 "parser.y"
  { 
	cb_emit_get_environment (yyvsp[-1], yyvsp[-4]);
  }
#line 8643 "parser.c"
    break;

  case 701: /* accept_body: identifier FROM "ARGUMENT-NUMBER"  */
#line 4092 "parser.y"
  {
	cb_emit_accept_arg_number (yyvsp[-2]);
  }
#line 8651 "parser.c"
    break;

  case 702: /* accept_body: identifier FROM "ARGUMENT-VALUE" on_accp_exception  */
#line 4096 "parser.y"
  {
	cb_emit_accept_arg_value (yyvsp[-3]);
  }
#line 8659 "parser.c"
    break;

  case 703: /* accept_body: identifier FROM mnemonic_name  */
#line 4100 "parser.y"
  {
	cb_emit_accept_mnemonic (yyvsp[-2], yyvsp[0]);
  }
#line 8667 "parser.c"
    break;

  case 704: /* accept_body: identifier FROM "Identifier"  */
#line 4104 "parser.y"
  {
	cb_emit_accept_name (yyvsp[-2], yyvsp[0]);
  }
#line 8675 "parser.c"
    break;

  case 705: /* opt_at_line_column: %empty  */
#line 4110 "parser.y"
                                { yyval = NULL; }
#line 8681 "parser.c"
    break;

  case 706: /* opt_at_line_column: _at line_number column_number  */
#line 4111 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-1], yyvsp[0]); }
#line 8687 "parser.c"
    break;

  case 707: /* opt_at_line_column: _at column_number line_number  */
#line 4112 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], yyvsp[-1]); }
#line 8693 "parser.c"
    break;

  case 708: /* opt_at_line_column: _at line_number  */
#line 4113 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], NULL); }
#line 8699 "parser.c"
    break;

  case 709: /* opt_at_line_column: _at column_number  */
#line 4114 "parser.y"
                                { yyval = cb_build_pair (NULL, yyvsp[0]); }
#line 8705 "parser.c"
    break;

  case 710: /* opt_at_line_column: AT simple_value  */
#line 4115 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8711 "parser.c"
    break;

  case 711: /* line_number: LINE _number id_or_lit  */
#line 4119 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8717 "parser.c"
    break;

  case 712: /* column_number: COLUMN _number id_or_lit  */
#line 4123 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8723 "parser.c"
    break;

  case 713: /* column_number: POSITION _number id_or_lit  */
#line 4124 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8729 "parser.c"
    break;

  case 718: /* accp_attr: BELL  */
#line 4137 "parser.y"
                { dispattrs |= COB_SCREEN_BELL; }
#line 8735 "parser.c"
    break;

  case 719: /* accp_attr: BLINK  */
#line 4138 "parser.y"
                { dispattrs |= COB_SCREEN_BLINK; }
#line 8741 "parser.c"
    break;

  case 720: /* accp_attr: HIGHLIGHT  */
#line 4139 "parser.y"
                { dispattrs |= COB_SCREEN_HIGHLIGHT; }
#line 8747 "parser.c"
    break;

  case 721: /* accp_attr: LOWLIGHT  */
#line 4140 "parser.y"
                { dispattrs |= COB_SCREEN_LOWLIGHT; }
#line 8753 "parser.c"
    break;

  case 722: /* accp_attr: "REVERSE-VIDEO"  */
#line 4141 "parser.y"
                { dispattrs |= COB_SCREEN_REVERSE; }
#line 8759 "parser.c"
    break;

  case 723: /* accp_attr: UNDERLINE  */
#line 4142 "parser.y"
                { dispattrs |= COB_SCREEN_UNDERLINE; }
#line 8765 "parser.c"
    break;

  case 724: /* accp_attr: OVERLINE  */
#line 4143 "parser.y"
                { dispattrs |= COB_SCREEN_OVERLINE; }
#line 8771 "parser.c"
    break;

  case 725: /* accp_attr: "FOREGROUND-COLOR" _is num_id_or_lit  */
#line 4145 "parser.y"
  {
	fgc = yyvsp[0];
  }
#line 8779 "parser.c"
    break;

  case 726: /* accp_attr: "BACKGROUND-COLOR" _is num_id_or_lit  */
#line 4149 "parser.y"
  {
	bgc = yyvsp[0];
  }
#line 8787 "parser.c"
    break;

  case 727: /* accp_attr: SCROLL UP _opt_scroll_lines  */
#line 4153 "parser.y"
  {
	scroll = yyvsp[0];
  }
#line 8795 "parser.c"
    break;

  case 728: /* accp_attr: SCROLL DOWN _opt_scroll_lines  */
#line 4157 "parser.y"
  {
	dispattrs |= COB_SCREEN_SCROLL_DOWN;
	scroll = yyvsp[0];
  }
#line 8804 "parser.c"
    break;

  case 729: /* accp_attr: AUTO  */
#line 4161 "parser.y"
                { dispattrs |= COB_SCREEN_AUTO; }
#line 8810 "parser.c"
    break;

  case 730: /* accp_attr: FULL  */
#line 4162 "parser.y"
                { dispattrs |= COB_SCREEN_FULL; }
#line 8816 "parser.c"
    break;

  case 731: /* accp_attr: REQUIRED  */
#line 4163 "parser.y"
                { dispattrs |= COB_SCREEN_REQUIRED; }
#line 8822 "parser.c"
    break;

  case 732: /* accp_attr: SECURE  */
#line 4164 "parser.y"
                { dispattrs |= COB_SCREEN_SECURE; }
#line 8828 "parser.c"
    break;

  case 733: /* accp_attr: UPDATE  */
#line 4165 "parser.y"
                { dispattrs |= COB_SCREEN_UPDATE; }
#line 8834 "parser.c"
    break;

  case 734: /* accp_attr: PROMPT  */
#line 4166 "parser.y"
                { dispattrs |= COB_SCREEN_PROMPT; }
#line 8840 "parser.c"
    break;

  case 735: /* end_accept: %empty  */
#line 4170 "parser.y"
                                { terminator_warning (TERM_ACCEPT); }
#line 8846 "parser.c"
    break;

  case 736: /* end_accept: "END-ACCEPT"  */
#line 4171 "parser.y"
                                { terminator_clear (TERM_ACCEPT); }
#line 8852 "parser.c"
    break;

  case 737: /* $@37: %empty  */
#line 4180 "parser.y"
                                { BEGIN_STATEMENT ("ADD", TERM_ADD); }
#line 8858 "parser.c"
    break;

  case 739: /* add_body: x_list TO arithmetic_x_list on_size_error  */
#line 4187 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '+', cb_build_binary_list (yyvsp[-3], '+'));
  }
#line 8866 "parser.c"
    break;

  case 740: /* add_body: x_list add_to GIVING arithmetic_x_list on_size_error  */
#line 4191 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_list (yyvsp[-4], '+'));
  }
#line 8874 "parser.c"
    break;

  case 741: /* add_body: CORRESPONDING identifier TO identifier flag_rounded on_size_error  */
#line 4195 "parser.y"
  {
	cb_emit_corresponding (cb_build_add, yyvsp[-2], yyvsp[-4], yyvsp[-1]);
  }
#line 8882 "parser.c"
    break;

  case 743: /* add_to: TO x  */
#line 4201 "parser.y"
                                { cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 8888 "parser.c"
    break;

  case 744: /* end_add: %empty  */
#line 4205 "parser.y"
                                { terminator_warning (TERM_ADD); }
#line 8894 "parser.c"
    break;

  case 745: /* end_add: "END-ADD"  */
#line 4206 "parser.y"
                                { terminator_clear (TERM_ADD); }
#line 8900 "parser.c"
    break;

  case 746: /* $@38: %empty  */
#line 4215 "parser.y"
                                { BEGIN_STATEMENT ("ALLOCATE", 0); }
#line 8906 "parser.c"
    break;

  case 748: /* allocate_body: "Identifier" flag_initialized allocate_returning  */
#line 4221 "parser.y"
  {
	cb_emit_allocate (yyvsp[-2], yyvsp[0], NULL, yyvsp[-1]);
  }
#line 8914 "parser.c"
    break;

  case 749: /* allocate_body: expr CHARACTERS flag_initialized RETURNING target_x  */
#line 4225 "parser.y"
  {
	cb_emit_allocate (NULL, yyvsp[0], yyvsp[-4], yyvsp[-2]);
  }
#line 8922 "parser.c"
    break;

  case 750: /* allocate_returning: %empty  */
#line 4231 "parser.y"
                                { yyval = NULL; }
#line 8928 "parser.c"
    break;

  case 751: /* allocate_returning: RETURNING target_x  */
#line 4232 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8934 "parser.c"
    break;

  case 752: /* alter_statement: ALTER alter_options  */
#line 4242 "parser.y"
  {
	cb_error (_("ALTER statement is obsolete and unsupported"));
  }
#line 8942 "parser.c"
    break;

  case 757: /* $@39: %empty  */
#line 4260 "parser.y"
                                { BEGIN_STATEMENT ("CALL", TERM_CALL); }
#line 8948 "parser.c"
    break;

  case 758: /* call_statement: CALL $@39 id_or_lit_or_func call_using call_returning call_on_exception call_not_on_exception end_call  */
#line 4264 "parser.y"
  {
	cb_emit_call (yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1]);
  }
#line 8956 "parser.c"
    break;

  case 759: /* call_using: %empty  */
#line 4270 "parser.y"
                                { yyval = NULL; }
#line 8962 "parser.c"
    break;

  case 760: /* $@40: %empty  */
#line 4272 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
	size_mode = CB_SIZE_4;
  }
#line 8971 "parser.c"
    break;

  case 761: /* call_using: USING $@40 call_param_list  */
#line 4276 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8977 "parser.c"
    break;

  case 762: /* call_param_list: call_param  */
#line 4280 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8983 "parser.c"
    break;

  case 763: /* call_param_list: call_param_list call_param  */
#line 4282 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 8989 "parser.c"
    break;

  case 764: /* call_param: call_type OMITTED  */
#line 4287 "parser.y"
  {
	if (call_mode != CB_CALL_BY_REFERENCE) {
		cb_error (_("OMITTED only allowed with BY REFERENCE"));
	}
	yyval = cb_build_pair (cb_int (call_mode), cb_null);
  }
#line 9000 "parser.c"
    break;

  case 765: /* call_param: call_type size_optional x  */
#line 4294 "parser.y"
  {
	yyval = cb_build_pair (cb_int (call_mode), yyvsp[0]);
	CB_SIZES (yyval) = size_mode;
  }
#line 9009 "parser.c"
    break;

  case 767: /* call_type: _by REFERENCE  */
#line 4303 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
  }
#line 9017 "parser.c"
    break;

  case 768: /* call_type: _by CONTENT  */
#line 4307 "parser.y"
  {
	if (current_program->flag_chained) {
		cb_error (_("BY CONTENT not allowed in CHAINED program"));
	} else {
		call_mode = CB_CALL_BY_CONTENT;
	}
  }
#line 9029 "parser.c"
    break;

  case 769: /* call_type: _by VALUE  */
#line 4315 "parser.y"
  {
	if (current_program->flag_chained) {
		cb_error (_("BY VALUE not allowed in CHAINED program"));
	} else {
		call_mode = CB_CALL_BY_VALUE;
	}
  }
#line 9041 "parser.c"
    break;

  case 770: /* call_returning: %empty  */
#line 4325 "parser.y"
                                { yyval = NULL; }
#line 9047 "parser.c"
    break;

  case 771: /* call_returning: RETURNING identifier  */
#line 4326 "parser.y"
                                { yyval = yyvsp[0]; }
#line 9053 "parser.c"
    break;

  case 772: /* call_returning: GIVING identifier  */
#line 4327 "parser.y"
                                { yyval = yyvsp[0]; }
#line 9059 "parser.c"
    break;

  case 773: /* call_on_exception: %empty  */
#line 4332 "parser.y"
  {
	yyval = NULL;
  }
#line 9067 "parser.c"
    break;

  case 774: /* $@41: %empty  */
#line 4336 "parser.y"
  {
	check_unreached = 0;
  }
#line 9075 "parser.c"
    break;

  case 775: /* call_on_exception: exception_or_overflow $@41 statement_list  */
#line 4340 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 9083 "parser.c"
    break;

  case 776: /* call_not_on_exception: %empty  */
#line 4347 "parser.y"
  {
	yyval = NULL;
  }
#line 9091 "parser.c"
    break;

  case 777: /* $@42: %empty  */
#line 4351 "parser.y"
  {
	check_unreached = 0;
  }
#line 9099 "parser.c"
    break;

  case 778: /* call_not_on_exception: not_exception_or_overflow $@42 statement_list  */
#line 4355 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 9107 "parser.c"
    break;

  case 779: /* end_call: %empty  */
#line 4361 "parser.y"
                                { terminator_warning (TERM_CALL); }
#line 9113 "parser.c"
    break;

  case 780: /* end_call: "END-CALL"  */
#line 4362 "parser.y"
                                { terminator_clear (TERM_CALL); }
#line 9119 "parser.c"
    break;

  case 781: /* $@43: %empty  */
#line 4371 "parser.y"
                                { BEGIN_STATEMENT ("CANCEL", 0); }
#line 9125 "parser.c"
    break;

  case 784: /* cancel_list: cancel_list id_or_lit  */
#line 4377 "parser.y"
  {
	cb_emit_cancel (yyvsp[0]);
  }
#line 9133 "parser.c"
    break;

  case 785: /* cancel_list: ALL  */
#line 4381 "parser.y"
  {
	cb_emit_cancel_all ();
  }
#line 9141 "parser.c"
    break;

  case 786: /* $@44: %empty  */
#line 4392 "parser.y"
                                { BEGIN_STATEMENT ("CLOSE", 0); }
#line 9147 "parser.c"
    break;

  case 789: /* close_list: close_list file_name close_option  */
#line 4399 "parser.y"
  {
	BEGIN_IMPLICIT_STATEMENT (yyvsp[-1]);
	if (yyvsp[-1] != cb_error_node) {
		cb_emit_close (yyvsp[-1], yyvsp[0]);
	}
  }
#line 9158 "parser.c"
    break;

  case 790: /* close_option: %empty  */
#line 4408 "parser.y"
                                { yyval = cb_int (COB_CLOSE_NORMAL); }
#line 9164 "parser.c"
    break;

  case 791: /* close_option: reel_or_unit  */
#line 4409 "parser.y"
                                { yyval = cb_int (COB_CLOSE_UNIT); }
#line 9170 "parser.c"
    break;

  case 792: /* close_option: reel_or_unit _for REMOVAL  */
#line 4410 "parser.y"
                                { yyval = cb_int (COB_CLOSE_UNIT_REMOVAL); }
#line 9176 "parser.c"
    break;

  case 793: /* close_option: _with NO REWIND  */
#line 4411 "parser.y"
                                { yyval = cb_int (COB_CLOSE_NO_REWIND); }
#line 9182 "parser.c"
    break;

  case 794: /* close_option: _with LOCK  */
#line 4412 "parser.y"
                                { yyval = cb_int (COB_CLOSE_LOCK); }
#line 9188 "parser.c"
    break;

  case 797: /* $@45: %empty  */
#line 4423 "parser.y"
                                { BEGIN_STATEMENT ("COMPUTE", TERM_COMPUTE); }
#line 9194 "parser.c"
    break;

  case 799: /* compute_body: arithmetic_x_list comp_equal expr on_size_error  */
#line 4430 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-3], 0, yyvsp[-1]);
  }
#line 9202 "parser.c"
    break;

  case 800: /* end_compute: %empty  */
#line 4436 "parser.y"
                                { terminator_warning (TERM_COMPUTE); }
#line 9208 "parser.c"
    break;

  case 801: /* end_compute: "END-COMPUTE"  */
#line 4437 "parser.y"
                                { terminator_clear (TERM_COMPUTE); }
#line 9214 "parser.c"
    break;

  case 804: /* commit_statement: COMMIT  */
#line 4448 "parser.y"
  {
	BEGIN_STATEMENT ("COMMIT", 0);
	cb_emit_commit ();
  }
#line 9223 "parser.c"
    break;

  case 805: /* continue_statement: CONTINUE  */
#line 4461 "parser.y"
  {
	BEGIN_STATEMENT ("CONTINUE", 0);
	cb_emit_continue ();
  }
#line 9232 "parser.c"
    break;

  case 806: /* $@46: %empty  */
#line 4473 "parser.y"
                                { BEGIN_STATEMENT ("DELETE", TERM_DELETE); }
#line 9238 "parser.c"
    break;

  case 807: /* delete_statement: DELETE $@46 file_name _record opt_invalid_key end_delete  */
#line 4476 "parser.y"
  {
	if (yyvsp[-3] != cb_error_node) {
		cb_emit_delete (yyvsp[-3]);
	}
  }
#line 9248 "parser.c"
    break;

  case 808: /* end_delete: %empty  */
#line 4484 "parser.y"
                                { terminator_warning (TERM_DELETE); }
#line 9254 "parser.c"
    break;

  case 809: /* end_delete: "END-DELETE"  */
#line 4485 "parser.y"
                                { terminator_clear (TERM_DELETE); }
#line 9260 "parser.c"
    break;

  case 810: /* $@47: %empty  */
#line 4494 "parser.y"
                                  { BEGIN_STATEMENT ("DELETE-FILE", 0); }
#line 9266 "parser.c"
    break;

  case 811: /* delete_file_statement: DELETE $@47 "FILE" file_name_list  */
#line 4496 "parser.y"
  {
	cb_tree l;
	for (l = yyvsp[0]; l; l = CB_CHAIN (l)) {
		if (CB_VALUE (l) != cb_error_node) {
			BEGIN_IMPLICIT_STATEMENT (l);
			cb_emit_delete_file (CB_VALUE (l));
		}
	}
  }
#line 9280 "parser.c"
    break;

  case 812: /* $@48: %empty  */
#line 4514 "parser.y"
  {
	BEGIN_STATEMENT ("DISPLAY", TERM_DISPLAY);
	dispattrs = 0;
	fgc = NULL;
	bgc = NULL;
	scroll = NULL;
  }
#line 9292 "parser.c"
    break;

  case 814: /* display_body: id_or_lit "UPON ENVIRONMENT-NAME" on_disp_exception  */
#line 4527 "parser.y"
  {
	cb_emit_env_name (yyvsp[-2]);
  }
#line 9300 "parser.c"
    break;

  case 815: /* display_body: id_or_lit "UPON ENVIRONMENT-VALUE" on_disp_exception  */
#line 4531 "parser.y"
  {
	cb_emit_env_value (yyvsp[-2]);
  }
#line 9308 "parser.c"
    break;

  case 816: /* display_body: id_or_lit "UPON ARGUMENT-NUMBER" on_disp_exception  */
#line 4535 "parser.y"
  {
	cb_emit_arg_number (yyvsp[-2]);
  }
#line 9316 "parser.c"
    break;

  case 817: /* display_body: id_or_lit "UPON COMMAND-LINE" on_disp_exception  */
#line 4539 "parser.y"
  {
	cb_emit_command_line (yyvsp[-2]);
  }
#line 9324 "parser.c"
    break;

  case 818: /* display_body: x_list opt_at_line_column with_clause on_disp_exception  */
#line 4543 "parser.y"
  {
	cb_emit_display (yyvsp[-3], cb_int0, yyvsp[-1], yyvsp[-2], fgc, bgc, scroll, dispattrs);
  }
#line 9332 "parser.c"
    break;

  case 819: /* display_body: x_list opt_at_line_column UPON mnemonic_name with_clause on_disp_exception  */
#line 4547 "parser.y"
  {
	cb_emit_display_mnemonic (yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9340 "parser.c"
    break;

  case 820: /* display_body: x_list opt_at_line_column UPON "Identifier" with_clause on_disp_exception  */
#line 4551 "parser.y"
  {
	cb_tree word = cb_build_display_upon_direct (yyvsp[-2]);
	cb_emit_display (yyvsp[-5], word, yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9349 "parser.c"
    break;

  case 821: /* display_body: x_list opt_at_line_column UPON PRINTER with_clause on_disp_exception  */
#line 4556 "parser.y"
  {
	cb_emit_display (yyvsp[-5], cb_int0, yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9357 "parser.c"
    break;

  case 822: /* display_body: x_list opt_at_line_column UPON CRT with_clause on_disp_exception  */
#line 4560 "parser.y"
  {
	cb_emit_display (yyvsp[-5], cb_int0, yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9365 "parser.c"
    break;

  case 823: /* with_clause: %empty  */
#line 4566 "parser.y"
                                { yyval = cb_int1; }
#line 9371 "parser.c"
    break;

  case 824: /* with_clause: _with "NO ADVANCING"  */
#line 4567 "parser.y"
                                { yyval = cb_int0; }
#line 9377 "parser.c"
    break;

  case 825: /* with_clause: WITH disp_attrs  */
#line 4568 "parser.y"
                                { yyval = cb_int1; }
#line 9383 "parser.c"
    break;

  case 828: /* disp_attr: BELL  */
#line 4578 "parser.y"
                { dispattrs |= COB_SCREEN_BELL; }
#line 9389 "parser.c"
    break;

  case 829: /* disp_attr: BLINK  */
#line 4579 "parser.y"
                { dispattrs |= COB_SCREEN_BLINK; }
#line 9395 "parser.c"
    break;

  case 830: /* disp_attr: ERASE EOL  */
#line 4580 "parser.y"
                { dispattrs |= COB_SCREEN_ERASE_EOL; }
#line 9401 "parser.c"
    break;

  case 831: /* disp_attr: ERASE EOS  */
#line 4581 "parser.y"
                { dispattrs |= COB_SCREEN_ERASE_EOS; }
#line 9407 "parser.c"
    break;

  case 832: /* disp_attr: HIGHLIGHT  */
#line 4582 "parser.y"
                { dispattrs |= COB_SCREEN_HIGHLIGHT; }
#line 9413 "parser.c"
    break;

  case 833: /* disp_attr: LOWLIGHT  */
#line 4583 "parser.y"
                { dispattrs |= COB_SCREEN_LOWLIGHT; }
#line 9419 "parser.c"
    break;

  case 834: /* disp_attr: "REVERSE-VIDEO"  */
#line 4584 "parser.y"
                { dispattrs |= COB_SCREEN_REVERSE; }
#line 9425 "parser.c"
    break;

  case 835: /* disp_attr: UNDERLINE  */
#line 4585 "parser.y"
                { dispattrs |= COB_SCREEN_UNDERLINE; }
#line 9431 "parser.c"
    break;

  case 836: /* disp_attr: OVERLINE  */
#line 4586 "parser.y"
                { dispattrs |= COB_SCREEN_OVERLINE; }
#line 9437 "parser.c"
    break;

  case 837: /* disp_attr: "FOREGROUND-COLOR" _is num_id_or_lit  */
#line 4588 "parser.y"
  {
	fgc = yyvsp[0];
  }
#line 9445 "parser.c"
    break;

  case 838: /* disp_attr: "BACKGROUND-COLOR" _is num_id_or_lit  */
#line 4592 "parser.y"
  {
	bgc = yyvsp[0];
  }
#line 9453 "parser.c"
    break;

  case 839: /* disp_attr: SCROLL UP _opt_scroll_lines  */
#line 4596 "parser.y"
  {
	scroll = yyvsp[0];
  }
#line 9461 "parser.c"
    break;

  case 840: /* disp_attr: SCROLL DOWN _opt_scroll_lines  */
#line 4600 "parser.y"
  {
	dispattrs |= COB_SCREEN_SCROLL_DOWN;
	scroll = yyvsp[0];
  }
#line 9470 "parser.c"
    break;

  case 841: /* disp_attr: "BLANK-LINE"  */
#line 4604 "parser.y"
                { dispattrs |= COB_SCREEN_BLANK_LINE; }
#line 9476 "parser.c"
    break;

  case 842: /* disp_attr: "BLANK-SCREEN"  */
#line 4605 "parser.y"
                { dispattrs |= COB_SCREEN_BLANK_SCREEN; }
#line 9482 "parser.c"
    break;

  case 843: /* end_display: %empty  */
#line 4609 "parser.y"
                                { terminator_warning (TERM_DISPLAY); }
#line 9488 "parser.c"
    break;

  case 844: /* end_display: "END-DISPLAY"  */
#line 4610 "parser.y"
                                { terminator_clear (TERM_DISPLAY); }
#line 9494 "parser.c"
    break;

  case 845: /* $@49: %empty  */
#line 4619 "parser.y"
                                { BEGIN_STATEMENT ("DIVIDE", TERM_DIVIDE); }
#line 9500 "parser.c"
    break;

  case 847: /* divide_body: x INTO arithmetic_x_list on_size_error  */
#line 4626 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '/', yyvsp[-3]);
  }
#line 9508 "parser.c"
    break;

  case 848: /* divide_body: x INTO x GIVING arithmetic_x_list on_size_error  */
#line 4630 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_op (yyvsp[-3], '/', yyvsp[-5]));
  }
#line 9516 "parser.c"
    break;

  case 849: /* divide_body: x BY x GIVING arithmetic_x_list on_size_error  */
#line 4634 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_op (yyvsp[-5], '/', yyvsp[-3]));
  }
#line 9524 "parser.c"
    break;

  case 850: /* divide_body: x INTO x GIVING arithmetic_x REMAINDER arithmetic_x on_size_error  */
#line 4638 "parser.y"
  {
	cb_emit_divide (yyvsp[-5], yyvsp[-7], yyvsp[-3], yyvsp[-1]);
  }
#line 9532 "parser.c"
    break;

  case 851: /* divide_body: x BY x GIVING arithmetic_x REMAINDER arithmetic_x on_size_error  */
#line 4642 "parser.y"
  {
	cb_emit_divide (yyvsp[-7], yyvsp[-5], yyvsp[-3], yyvsp[-1]);
  }
#line 9540 "parser.c"
    break;

  case 852: /* end_divide: %empty  */
#line 4648 "parser.y"
                                { terminator_warning (TERM_DIVIDE); }
#line 9546 "parser.c"
    break;

  case 853: /* end_divide: "END-DIVIDE"  */
#line 4649 "parser.y"
                                { terminator_clear (TERM_DIVIDE); }
#line 9552 "parser.c"
    break;

  case 854: /* $@50: %empty  */
#line 4658 "parser.y"
                                { BEGIN_STATEMENT ("ENTRY", 0); }
#line 9558 "parser.c"
    break;

  case 855: /* entry_statement: ENTRY $@50 "Literal" call_using  */
#line 4660 "parser.y"
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
#line 9574 "parser.c"
    break;

  case 856: /* $@51: %empty  */
#line 4680 "parser.y"
  {
	BEGIN_STATEMENT ("EVALUATE", TERM_EVALUATE);
	eval_level++;
	for (eval_inc = 0; eval_inc < 64; eval_inc++) {
		eval_check[eval_level][eval_inc] = 0;
	}
	eval_inc = 0;
	eval_inc2 = 0;
  }
#line 9588 "parser.c"
    break;

  case 857: /* evaluate_statement: EVALUATE $@51 evaluate_subject_list evaluate_condition_list end_evaluate  */
#line 4691 "parser.y"
  {
	cb_emit_evaluate (yyvsp[-2], yyvsp[-1]);
	eval_level--;
  }
#line 9597 "parser.c"
    break;

  case 858: /* evaluate_subject_list: evaluate_subject  */
#line 4698 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9603 "parser.c"
    break;

  case 859: /* evaluate_subject_list: evaluate_subject_list _also evaluate_subject  */
#line 4701 "parser.y"
  {
 	if (!cb_allow_missing_also_clause_in_evaluate && yyvsp[-1] != cb_int1) {
 		cb_error  (_("Invalid expression"));
 	}
 	yyval = cb_list_add (yyvsp[-2], yyvsp[0]);
  }
#line 9614 "parser.c"
    break;

  case 860: /* evaluate_subject: expr  */
#line 4711 "parser.y"
  {
	yyval = yyvsp[0];
	if (CB_REFERENCE_P (yyvsp[0])) {
		eval_check[eval_level][eval_inc++] = 0;
	} else {
		eval_check[eval_level][eval_inc++] = 1;
	}
  }
#line 9627 "parser.c"
    break;

  case 861: /* evaluate_subject: "TRUE"  */
#line 4720 "parser.y"
  {
	yyval = cb_true;
	eval_check[eval_level][eval_inc++] = 2;
  }
#line 9636 "parser.c"
    break;

  case 862: /* evaluate_subject: "FALSE"  */
#line 4725 "parser.y"
  {
	yyval = cb_false;
	eval_check[eval_level][eval_inc++] = 3;
  }
#line 9645 "parser.c"
    break;

  case 863: /* evaluate_condition_list: evaluate_case_list evaluate_other  */
#line 4733 "parser.y"
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
#line 9675 "parser.c"
    break;

  case 864: /* evaluate_case_list: evaluate_case  */
#line 4761 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9681 "parser.c"
    break;

  case 865: /* evaluate_case_list: evaluate_case_list evaluate_case  */
#line 4763 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 9687 "parser.c"
    break;

  case 866: /* $@52: %empty  */
#line 4768 "parser.y"
  {
	check_unreached = 0;
  }
#line 9695 "parser.c"
    break;

  case 867: /* evaluate_case: evaluate_when_list $@52 statement_list  */
#line 4772 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[0] == NULL) {
		cb_error (_("syntax error"));
	}
	yyval = cb_cons (yyvsp[0], yyvsp[-2]);
	eval_inc2 = 0;
  }
#line 9707 "parser.c"
    break;

  case 868: /* evaluate_other: %empty  */
#line 4783 "parser.y"
  {
	yyval = NULL;
  }
#line 9715 "parser.c"
    break;

  case 869: /* $@53: %empty  */
#line 4787 "parser.y"
  {
	check_unreached = 0;
  }
#line 9723 "parser.c"
    break;

  case 870: /* evaluate_other: "WHEN OTHER" $@53 statement_list  */
#line 4791 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[0] == NULL) {
		cb_error (_("syntax error"));
	}
	yyval = cb_cons (yyvsp[0], NULL);
	eval_inc2 = 0;
  }
#line 9735 "parser.c"
    break;

  case 871: /* evaluate_when_list: WHEN evaluate_object_list  */
#line 4801 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9741 "parser.c"
    break;

  case 872: /* evaluate_when_list: evaluate_when_list WHEN evaluate_object_list  */
#line 4803 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 9747 "parser.c"
    break;

  case 873: /* evaluate_object_list: evaluate_object  */
#line 4807 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9753 "parser.c"
    break;

  case 874: /* evaluate_object_list: evaluate_object_list _also evaluate_object  */
#line 4810 "parser.y"
  {
 	if (!cb_allow_missing_also_clause_in_evaluate && yyvsp[-1] != cb_int1) {
 		cb_error  (_("Invalid expression"));
 	}
 	yyval = cb_list_add (yyvsp[-2], yyvsp[0]);
  }
#line 9764 "parser.c"
    break;

  case 875: /* evaluate_object: partial_expr opt_evaluate_thru_expr  */
#line 4820 "parser.y"
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
#line 9797 "parser.c"
    break;

  case 876: /* evaluate_object: ANY  */
#line 4848 "parser.y"
                                { yyval = cb_any; eval_inc2++; }
#line 9803 "parser.c"
    break;

  case 877: /* evaluate_object: "TRUE"  */
#line 4849 "parser.y"
                                { yyval = cb_true; eval_inc2++; }
#line 9809 "parser.c"
    break;

  case 878: /* evaluate_object: "FALSE"  */
#line 4850 "parser.y"
                                { yyval = cb_false; eval_inc2++; }
#line 9815 "parser.c"
    break;

  case 879: /* opt_evaluate_thru_expr: %empty  */
#line 4853 "parser.y"
                                { yyval = NULL; }
#line 9821 "parser.c"
    break;

  case 880: /* opt_evaluate_thru_expr: THRU expr  */
#line 4854 "parser.y"
                                { yyval = yyvsp[0]; }
#line 9827 "parser.c"
    break;

  case 881: /* end_evaluate: %empty  */
#line 4858 "parser.y"
                                { terminator_warning (TERM_EVALUATE); }
#line 9833 "parser.c"
    break;

  case 882: /* end_evaluate: "END-EVALUATE"  */
#line 4859 "parser.y"
                                { terminator_clear (TERM_EVALUATE); }
#line 9839 "parser.c"
    break;

  case 883: /* $@54: %empty  */
#line 4868 "parser.y"
                                { BEGIN_STATEMENT ("EXIT", 0); }
#line 9845 "parser.c"
    break;

  case 885: /* exit_body: %empty  */
#line 4873 "parser.y"
                                { /* nothing */ }
#line 9851 "parser.c"
    break;

  case 886: /* exit_body: PROGRAM  */
#line 4875 "parser.y"
  {
	if (in_declaratives && use_global_ind) {
		cb_error (_("EXIT PROGRAM is not allowed within a USE GLOBAL procedure"));
	}
	check_unreached = 1;
	cb_emit_exit (0);
  }
#line 9863 "parser.c"
    break;

  case 887: /* exit_body: PERFORM  */
#line 4883 "parser.y"
  {
	if (!perform_stack) {
		cb_error (_("EXIT PERFORM is only valid with inline PERFORM"));
	} else {
		cb_emit_java_break ();
	}
  }
#line 9875 "parser.c"
    break;

  case 888: /* exit_body: PERFORM CYCLE  */
#line 4891 "parser.y"
  {
	if (!perform_stack) {
		cb_error (_("EXIT PERFORM is only valid with inline PERFORM"));
	} else {
		cb_emit_java_continue ();
	}
  }
#line 9887 "parser.c"
    break;

  case 889: /* exit_body: SECTION  */
#line 4899 "parser.y"
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
#line 9909 "parser.c"
    break;

  case 890: /* exit_body: PARAGRAPH  */
#line 4917 "parser.y"
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
#line 9931 "parser.c"
    break;

  case 891: /* $@55: %empty  */
#line 4941 "parser.y"
                                { BEGIN_STATEMENT ("FREE", 0); }
#line 9937 "parser.c"
    break;

  case 892: /* free_statement: FREE $@55 target_x_list  */
#line 4943 "parser.y"
  {
	cb_emit_free (yyvsp[0]);
  }
#line 9945 "parser.c"
    break;

  case 893: /* $@56: %empty  */
#line 4954 "parser.y"
                                { BEGIN_STATEMENT ("GENERATE", 0); }
#line 9951 "parser.c"
    break;

  case 894: /* generate_statement: GENERATE $@56 identifier  */
#line 4956 "parser.y"
  {
	PENDING("GENERATE");
  }
#line 9959 "parser.c"
    break;

  case 895: /* $@57: %empty  */
#line 4967 "parser.y"
                                { BEGIN_STATEMENT ("GO TO", 0); }
#line 9965 "parser.c"
    break;

  case 896: /* goto_statement: GO _to $@57 procedure_name_list goto_depending  */
#line 4969 "parser.y"
  {
	cb_emit_goto (yyvsp[-1], yyvsp[0]);
  }
#line 9973 "parser.c"
    break;

  case 897: /* goto_depending: %empty  */
#line 4976 "parser.y"
  {
	check_unreached = 1;
	yyval = NULL;
  }
#line 9982 "parser.c"
    break;

  case 898: /* goto_depending: DEPENDING _on identifier  */
#line 4981 "parser.y"
  {
	check_unreached = 0;
	yyval = yyvsp[0];
  }
#line 9991 "parser.c"
    break;

  case 899: /* $@58: %empty  */
#line 4993 "parser.y"
                                { BEGIN_STATEMENT ("GOBACK", 0); }
#line 9997 "parser.c"
    break;

  case 900: /* goback_statement: GOBACK $@58  */
#line 4994 "parser.y"
  {
	check_unreached = 1;
	cb_emit_exit (1);
  }
#line 10006 "parser.c"
    break;

  case 901: /* $@59: %empty  */
#line 5006 "parser.y"
                                { BEGIN_STATEMENT ("IF", TERM_IF); }
#line 10012 "parser.c"
    break;

  case 902: /* $@60: %empty  */
#line 5008 "parser.y"
  {
	check_unreached = 0;
  }
#line 10020 "parser.c"
    break;

  case 903: /* if_statement: IF $@59 condition _then $@60 statement_list if_else_sentence end_if  */
#line 5013 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[-2] == NULL) {
		cb_error (_("syntax error"));
	}
	cb_emit_if (yyvsp[-5], yyvsp[-2], yyvsp[-1]);
  }
#line 10031 "parser.c"
    break;

  case 905: /* if_else_sentence: %empty  */
#line 5024 "parser.y"
  {
	yyval = NULL;
  }
#line 10039 "parser.c"
    break;

  case 906: /* $@61: %empty  */
#line 5028 "parser.y"
  {
	check_unreached = 0;
  }
#line 10047 "parser.c"
    break;

  case 907: /* if_else_sentence: ELSE $@61 statement_list  */
#line 5032 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[0] == NULL) {
		cb_error (_("syntax error"));
	}
	yyval = yyvsp[0];
  }
#line 10058 "parser.c"
    break;

  case 908: /* end_if: %empty  */
#line 5041 "parser.y"
                                { terminator_warning (TERM_IF); }
#line 10064 "parser.c"
    break;

  case 909: /* end_if: "END-IF"  */
#line 5042 "parser.y"
                                { terminator_clear (TERM_IF); }
#line 10070 "parser.c"
    break;

  case 910: /* $@62: %empty  */
#line 5051 "parser.y"
                                { BEGIN_STATEMENT ("INITIALIZE", 0); }
#line 10076 "parser.c"
    break;

  case 911: /* initialize_statement: INITIALIZE $@62 target_x_list initialize_filler initialize_value initialize_replacing initialize_default  */
#line 5053 "parser.y"
  {
	cb_emit_initialize (yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]);
  }
#line 10084 "parser.c"
    break;

  case 912: /* initialize_filler: %empty  */
#line 5059 "parser.y"
                                { yyval = NULL; }
#line 10090 "parser.c"
    break;

  case 913: /* initialize_filler: _with FILLER  */
#line 5060 "parser.y"
                                { yyval = cb_true; }
#line 10096 "parser.c"
    break;

  case 914: /* initialize_value: %empty  */
#line 5064 "parser.y"
                                { yyval = NULL; }
#line 10102 "parser.c"
    break;

  case 915: /* initialize_value: ALL _to VALUE  */
#line 5065 "parser.y"
                                { yyval = cb_true; }
#line 10108 "parser.c"
    break;

  case 916: /* initialize_value: initialize_category _to VALUE  */
#line 5066 "parser.y"
                                { yyval = yyvsp[-2]; }
#line 10114 "parser.c"
    break;

  case 917: /* initialize_replacing: %empty  */
#line 5070 "parser.y"
                                { yyval = NULL; }
#line 10120 "parser.c"
    break;

  case 918: /* initialize_replacing: REPLACING initialize_replacing_list  */
#line 5072 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10126 "parser.c"
    break;

  case 919: /* initialize_replacing_list: initialize_replacing_item  */
#line 5076 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10132 "parser.c"
    break;

  case 920: /* initialize_replacing_list: initialize_replacing_list initialize_replacing_item  */
#line 5078 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 10138 "parser.c"
    break;

  case 921: /* initialize_replacing_item: initialize_category _data BY x  */
#line 5082 "parser.y"
                                 { yyval = cb_build_pair (yyvsp[-3], yyvsp[0]); }
#line 10144 "parser.c"
    break;

  case 922: /* initialize_category: ALPHABETIC  */
#line 5086 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_ALPHABETIC); }
#line 10150 "parser.c"
    break;

  case 923: /* initialize_category: ALPHANUMERIC  */
#line 5087 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_ALPHANUMERIC); }
#line 10156 "parser.c"
    break;

  case 924: /* initialize_category: NUMERIC  */
#line 5088 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NUMERIC); }
#line 10162 "parser.c"
    break;

  case 925: /* initialize_category: "ALPHANUMERIC-EDITED"  */
#line 5089 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_ALPHANUMERIC_EDITED); }
#line 10168 "parser.c"
    break;

  case 926: /* initialize_category: "NUMERIC-EDITED"  */
#line 5090 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NUMERIC_EDITED); }
#line 10174 "parser.c"
    break;

  case 927: /* initialize_category: NATIONAL  */
#line 5091 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NATIONAL); }
#line 10180 "parser.c"
    break;

  case 928: /* initialize_category: "NATIONAL-EDITED"  */
#line 5092 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NATIONAL_EDITED); }
#line 10186 "parser.c"
    break;

  case 929: /* initialize_default: %empty  */
#line 5096 "parser.y"
                                { yyval = NULL; }
#line 10192 "parser.c"
    break;

  case 930: /* initialize_default: DEFAULT  */
#line 5097 "parser.y"
                                { yyval = cb_true; }
#line 10198 "parser.c"
    break;

  case 931: /* $@63: %empty  */
#line 5106 "parser.y"
                                { BEGIN_STATEMENT ("INITIATE", 0); }
#line 10204 "parser.c"
    break;

  case 932: /* initiate_statement: INITIATE $@63 identifier_list  */
#line 5108 "parser.y"
  {
	PENDING("INITIATE");
  }
#line 10212 "parser.c"
    break;

  case 933: /* $@64: %empty  */
#line 5119 "parser.y"
  {
	BEGIN_STATEMENT ("INSPECT", 0);
	sending_id = 0;
	inspect_keyword = 0;
  }
#line 10222 "parser.c"
    break;

  case 935: /* send_identifier: identifier  */
#line 5128 "parser.y"
                                { save_tree_1 = yyvsp[0]; sending_id = 0; }
#line 10228 "parser.c"
    break;

  case 936: /* send_identifier: literal  */
#line 5129 "parser.y"
                                { save_tree_1 = yyvsp[0]; sending_id = 1; }
#line 10234 "parser.c"
    break;

  case 937: /* send_identifier: function  */
#line 5130 "parser.y"
                                { save_tree_1 = yyvsp[0]; sending_id = 1; }
#line 10240 "parser.c"
    break;

  case 940: /* inspect_item: inspect_tallying  */
#line 5139 "parser.y"
                                { cb_emit_inspect (save_tree_1, yyvsp[0], cb_int0, 0); }
#line 10246 "parser.c"
    break;

  case 941: /* inspect_item: inspect_replacing  */
#line 5140 "parser.y"
                                { cb_emit_inspect (save_tree_1, yyvsp[0], cb_int1, 1); }
#line 10252 "parser.c"
    break;

  case 942: /* inspect_item: inspect_converting  */
#line 5141 "parser.y"
                                { cb_emit_inspect (save_tree_1, yyvsp[0], cb_int0, 2); }
#line 10258 "parser.c"
    break;

  case 943: /* $@65: %empty  */
#line 5147 "parser.y"
                                { cb_init_tarrying (); }
#line 10264 "parser.c"
    break;

  case 944: /* inspect_tallying: TALLYING $@65 tallying_list  */
#line 5148 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10270 "parser.c"
    break;

  case 945: /* tallying_list: tallying_item  */
#line 5152 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10276 "parser.c"
    break;

  case 946: /* tallying_list: tallying_list tallying_item  */
#line 5153 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 10282 "parser.c"
    break;

  case 947: /* tallying_item: simple_value FOR  */
#line 5157 "parser.y"
                                { yyval = cb_build_tarrying_data (yyvsp[-1]); }
#line 10288 "parser.c"
    break;

  case 948: /* tallying_item: CHARACTERS inspect_region  */
#line 5158 "parser.y"
                                { yyval = cb_build_tarrying_characters (yyvsp[0]); }
#line 10294 "parser.c"
    break;

  case 949: /* tallying_item: ALL  */
#line 5159 "parser.y"
                                { yyval = cb_build_tarrying_all (); }
#line 10300 "parser.c"
    break;

  case 950: /* tallying_item: LEADING  */
#line 5160 "parser.y"
                                { yyval = cb_build_tarrying_leading (); }
#line 10306 "parser.c"
    break;

  case 951: /* tallying_item: TRAILING  */
#line 5161 "parser.y"
                                { yyval = cb_build_tarrying_trailing (); }
#line 10312 "parser.c"
    break;

  case 952: /* tallying_item: simple_value inspect_region  */
#line 5162 "parser.y"
                                { yyval = cb_build_tarrying_value (yyvsp[-1], yyvsp[0]); }
#line 10318 "parser.c"
    break;

  case 953: /* inspect_replacing: REPLACING replacing_list  */
#line 5168 "parser.y"
                                { yyval = yyvsp[0]; inspect_keyword = 0; }
#line 10324 "parser.c"
    break;

  case 954: /* replacing_list: replacing_item  */
#line 5172 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10330 "parser.c"
    break;

  case 955: /* replacing_list: replacing_list replacing_item  */
#line 5173 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 10336 "parser.c"
    break;

  case 956: /* replacing_item: CHARACTERS BY simple_value inspect_region  */
#line 5178 "parser.y"
  {
	yyval = cb_build_replacing_characters (yyvsp[-1], yyvsp[0], save_tree_1);
	inspect_keyword = 0;
  }
#line 10345 "parser.c"
    break;

  case 957: /* replacing_item: rep_keyword replacing_region  */
#line 5182 "parser.y"
                                        { yyval = yyvsp[0]; }
#line 10351 "parser.c"
    break;

  case 958: /* rep_keyword: %empty  */
#line 5186 "parser.y"
                                { /* Nothing */ }
#line 10357 "parser.c"
    break;

  case 959: /* rep_keyword: ALL  */
#line 5187 "parser.y"
                                { inspect_keyword = 1; }
#line 10363 "parser.c"
    break;

  case 960: /* rep_keyword: LEADING  */
#line 5188 "parser.y"
                                { inspect_keyword = 2; }
#line 10369 "parser.c"
    break;

  case 961: /* rep_keyword: FIRST  */
#line 5189 "parser.y"
                                { inspect_keyword = 3; }
#line 10375 "parser.c"
    break;

  case 962: /* rep_keyword: TRAILING  */
#line 5190 "parser.y"
                                { inspect_keyword = 4; }
#line 10381 "parser.c"
    break;

  case 963: /* replacing_region: simple_value BY simple_all_value inspect_region  */
#line 5195 "parser.y"
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
#line 10406 "parser.c"
    break;

  case 964: /* inspect_converting: CONVERTING simple_value TO simple_all_value inspect_region  */
#line 5221 "parser.y"
  {
	if (cb_validate_inspect (save_tree_1, yyvsp[-3], yyvsp[-1]) < 0 ) {
		yyval = cb_error_node;
	} else {
		yyval = cb_build_converting (yyvsp[-3], yyvsp[-1], yyvsp[0]);
	}
  }
#line 10418 "parser.c"
    break;

  case 965: /* inspect_region: %empty  */
#line 5233 "parser.y"
                                { yyval = cb_build_inspect_region_start (); }
#line 10424 "parser.c"
    break;

  case 966: /* inspect_region: inspect_region before_or_after _initial x  */
#line 5235 "parser.y"
                                { yyval = cb_build_inspect_region (yyvsp[-3], yyvsp[-2], yyvsp[0]); }
#line 10430 "parser.c"
    break;

  case 969: /* $@66: %empty  */
#line 5246 "parser.y"
                                { BEGIN_STATEMENT ("MERGE", 0); }
#line 10436 "parser.c"
    break;

  case 971: /* $@67: %empty  */
#line 5256 "parser.y"
                                { BEGIN_STATEMENT ("MOVE", 0); }
#line 10442 "parser.c"
    break;

  case 973: /* move_body: x TO target_x_list  */
#line 5262 "parser.y"
  {
	cb_emit_move (yyvsp[-2], yyvsp[0]);
  }
#line 10450 "parser.c"
    break;

  case 974: /* move_body: CORRESPONDING x TO target_x_list  */
#line 5266 "parser.y"
  {
	cb_emit_move_corresponding (yyvsp[-2], yyvsp[0]);
  }
#line 10458 "parser.c"
    break;

  case 975: /* $@68: %empty  */
#line 5277 "parser.y"
                                { BEGIN_STATEMENT ("MULTIPLY", TERM_MULTIPLY); }
#line 10464 "parser.c"
    break;

  case 977: /* multiply_body: x BY arithmetic_x_list on_size_error  */
#line 5284 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '*', yyvsp[-3]);
  }
#line 10472 "parser.c"
    break;

  case 978: /* multiply_body: x BY x GIVING arithmetic_x_list on_size_error  */
#line 5288 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_op (yyvsp[-5], '*', yyvsp[-3]));
  }
#line 10480 "parser.c"
    break;

  case 979: /* end_multiply: %empty  */
#line 5294 "parser.y"
                                { terminator_warning (TERM_MULTIPLY); }
#line 10486 "parser.c"
    break;

  case 980: /* end_multiply: "END-MULTIPLY"  */
#line 5295 "parser.y"
                                { terminator_clear (TERM_MULTIPLY); }
#line 10492 "parser.c"
    break;

  case 981: /* $@69: %empty  */
#line 5304 "parser.y"
                                { BEGIN_STATEMENT ("OPEN", 0); }
#line 10498 "parser.c"
    break;

  case 984: /* open_list: open_list open_mode open_sharing file_name_list open_option  */
#line 5311 "parser.y"
  {
	cb_tree l;
	for (l = yyvsp[-1]; l; l = CB_CHAIN (l)) {
		if (CB_VALUE (l) != cb_error_node) {
			BEGIN_IMPLICIT_STATEMENT (l);
			cb_emit_open (CB_VALUE (l), yyvsp[-3], yyvsp[-2]);
		}
	}
  }
#line 10512 "parser.c"
    break;

  case 985: /* open_mode: INPUT  */
#line 5323 "parser.y"
                                { yyval = cb_int (COB_OPEN_INPUT); }
#line 10518 "parser.c"
    break;

  case 986: /* open_mode: OUTPUT  */
#line 5324 "parser.y"
                                { yyval = cb_int (COB_OPEN_OUTPUT); }
#line 10524 "parser.c"
    break;

  case 987: /* open_mode: "I-O"  */
#line 5325 "parser.y"
                                { yyval = cb_int (COB_OPEN_I_O); }
#line 10530 "parser.c"
    break;

  case 988: /* open_mode: EXTEND  */
#line 5326 "parser.y"
                                { yyval = cb_int (COB_OPEN_EXTEND); }
#line 10536 "parser.c"
    break;

  case 989: /* open_sharing: %empty  */
#line 5330 "parser.y"
                                { yyval = NULL; }
#line 10542 "parser.c"
    break;

  case 990: /* open_sharing: SHARING _with sharing_option  */
#line 5331 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10548 "parser.c"
    break;

  case 991: /* open_option: %empty  */
#line 5335 "parser.y"
                                { yyval = NULL; }
#line 10554 "parser.c"
    break;

  case 992: /* open_option: _with NO REWIND  */
#line 5336 "parser.y"
                                { yyval = NULL; }
#line 10560 "parser.c"
    break;

  case 993: /* open_option: _with LOCK  */
#line 5337 "parser.y"
                                { PENDING ("OPEN ... WITH LOCK"); }
#line 10566 "parser.c"
    break;

  case 994: /* $@70: %empty  */
#line 5349 "parser.y"
                                { BEGIN_STATEMENT ("PERFORM", TERM_PERFORM); }
#line 10572 "parser.c"
    break;

  case 996: /* perform_body: perform_procedure perform_option  */
#line 5355 "parser.y"
  {
	cb_emit_perform (yyvsp[0], yyvsp[-1]);
  }
#line 10580 "parser.c"
    break;

  case 997: /* $@71: %empty  */
#line 5359 "parser.y"
  {
	perform_stack = cb_cons (yyvsp[0], perform_stack);
	check_unreached = 0;
  }
#line 10589 "parser.c"
    break;

  case 998: /* perform_body: perform_option $@71 statement_list end_perform  */
#line 5364 "parser.y"
  {
	perform_stack = CB_CHAIN (perform_stack);
	cb_emit_perform (yyvsp[-3], yyvsp[-1]);
  }
#line 10598 "parser.c"
    break;

  case 999: /* perform_body: perform_option "END-PERFORM"  */
#line 5369 "parser.y"
  {
	cb_emit_perform (yyvsp[-1], NULL);
  }
#line 10606 "parser.c"
    break;

  case 1000: /* end_perform: %empty  */
#line 5375 "parser.y"
                                { terminator_error (); }
#line 10612 "parser.c"
    break;

  case 1001: /* end_perform: "END-PERFORM"  */
#line 5376 "parser.y"
                                { terminator_clear (TERM_PERFORM); }
#line 10618 "parser.c"
    break;

  case 1002: /* perform_procedure: procedure_name  */
#line 5381 "parser.y"
  {
	CB_REFERENCE (yyvsp[0])->length = cb_true; /* return from $1 */
	yyval = cb_build_pair (yyvsp[0], yyvsp[0]);
  }
#line 10627 "parser.c"
    break;

  case 1003: /* perform_procedure: procedure_name THRU procedure_name  */
#line 5386 "parser.y"
  {
	CB_REFERENCE (yyvsp[0])->length = cb_true; /* return from $3 */
	yyval = cb_build_pair (yyvsp[-2], yyvsp[0]);
  }
#line 10636 "parser.c"
    break;

  case 1004: /* perform_option: %empty  */
#line 5394 "parser.y"
  {
	yyval = cb_build_perform_once (NULL);
  }
#line 10644 "parser.c"
    break;

  case 1005: /* perform_option: FOREVER  */
#line 5398 "parser.y"
  {
	yyval = cb_build_perform_forever (NULL);
  }
#line 10652 "parser.c"
    break;

  case 1006: /* perform_option: id_or_lit_or_func TIMES  */
#line 5402 "parser.y"
  {
	yyval = cb_build_perform_times (yyvsp[-1]);
	current_program->loop_counter++;
  }
#line 10661 "parser.c"
    break;

  case 1007: /* perform_option: perform_test UNTIL condition  */
#line 5407 "parser.y"
  {
	cb_tree varying;

	varying = cb_list_init (cb_build_perform_varying (NULL, NULL, NULL, yyvsp[0]));
	yyval = cb_build_perform_until (yyvsp[-2], varying);
  }
#line 10672 "parser.c"
    break;

  case 1008: /* perform_option: perform_test VARYING perform_varying_list  */
#line 5414 "parser.y"
  {
	yyval = cb_build_perform_until (yyvsp[-2], yyvsp[0]);
  }
#line 10680 "parser.c"
    break;

  case 1009: /* perform_test: %empty  */
#line 5420 "parser.y"
                                { yyval = CB_BEFORE; }
#line 10686 "parser.c"
    break;

  case 1010: /* perform_test: _with TEST before_or_after  */
#line 5421 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10692 "parser.c"
    break;

  case 1011: /* perform_varying_list: perform_varying  */
#line 5425 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 10698 "parser.c"
    break;

  case 1012: /* perform_varying_list: perform_varying_list AFTER perform_varying  */
#line 5427 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 10704 "parser.c"
    break;

  case 1013: /* perform_varying: identifier FROM x BY x UNTIL condition  */
#line 5432 "parser.y"
  {
	yyval = cb_build_perform_varying (yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 10712 "parser.c"
    break;

  case 1014: /* $@72: %empty  */
#line 5443 "parser.y"
                                { BEGIN_STATEMENT ("READ", TERM_READ); }
#line 10718 "parser.c"
    break;

  case 1015: /* read_statement: READ $@72 file_name flag_next _record read_into with_lock read_key read_handler end_read  */
#line 5446 "parser.y"
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
#line 10744 "parser.c"
    break;

  case 1016: /* read_into: %empty  */
#line 5470 "parser.y"
                                { yyval = NULL; }
#line 10750 "parser.c"
    break;

  case 1017: /* read_into: INTO identifier  */
#line 5471 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10756 "parser.c"
    break;

  case 1018: /* with_lock: %empty  */
#line 5475 "parser.y"
                                { yyval = NULL; }
#line 10762 "parser.c"
    break;

  case 1019: /* with_lock: IGNORING LOCK  */
#line 5477 "parser.y"
  {
	yyval = cb_int3;
  }
#line 10770 "parser.c"
    break;

  case 1020: /* with_lock: _with LOCK  */
#line 5481 "parser.y"
  {
	yyval = cb_int1;
  }
#line 10778 "parser.c"
    break;

  case 1021: /* with_lock: _with NO LOCK  */
#line 5485 "parser.y"
  {
	yyval = cb_int2;
  }
#line 10786 "parser.c"
    break;

  case 1022: /* with_lock: _with IGNORE LOCK  */
#line 5489 "parser.y"
  {
	yyval = cb_int3;
  }
#line 10794 "parser.c"
    break;

  case 1023: /* with_lock: _with WAIT  */
#line 5493 "parser.y"
  {
	yyval = cb_int4;
  }
#line 10802 "parser.c"
    break;

  case 1024: /* read_key: %empty  */
#line 5499 "parser.y"
                                { yyval = NULL; }
#line 10808 "parser.c"
    break;

  case 1025: /* read_key: KEY _is identifier_list  */
#line 5501 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 10816 "parser.c"
    break;

  case 1029: /* end_read: %empty  */
#line 5512 "parser.y"
                                { terminator_warning (TERM_READ); }
#line 10822 "parser.c"
    break;

  case 1030: /* end_read: "END-READ"  */
#line 5513 "parser.y"
                                { terminator_clear (TERM_READ); }
#line 10828 "parser.c"
    break;

  case 1031: /* $@73: %empty  */
#line 5522 "parser.y"
                                { BEGIN_STATEMENT ("RELEASE", 0); }
#line 10834 "parser.c"
    break;

  case 1032: /* release_statement: RELEASE $@73 record_name write_from  */
#line 5524 "parser.y"
  {
	if (yyvsp[-1] != cb_error_node) {
		cb_emit_release (yyvsp[-1], yyvsp[0]);
	}
  }
#line 10844 "parser.c"
    break;

  case 1033: /* $@74: %empty  */
#line 5537 "parser.y"
                                { BEGIN_STATEMENT ("RETURN", TERM_RETURN); }
#line 10850 "parser.c"
    break;

  case 1034: /* return_statement: RETURN $@74 file_name _record read_into at_end end_return  */
#line 5540 "parser.y"
  {
	if (yyvsp[-4] != cb_error_node) {
		cb_emit_return (yyvsp[-4], yyvsp[-2]);
	}
  }
#line 10860 "parser.c"
    break;

  case 1035: /* end_return: %empty  */
#line 5548 "parser.y"
                                { terminator_warning (TERM_RETURN); }
#line 10866 "parser.c"
    break;

  case 1036: /* end_return: "END-RETURN"  */
#line 5549 "parser.y"
                                { terminator_clear (TERM_RETURN); }
#line 10872 "parser.c"
    break;

  case 1037: /* $@75: %empty  */
#line 5558 "parser.y"
                                { BEGIN_STATEMENT ("REWRITE", TERM_REWRITE); }
#line 10878 "parser.c"
    break;

  case 1038: /* rewrite_statement: REWRITE $@75 record_name write_from write_lock opt_invalid_key end_rewrite  */
#line 5561 "parser.y"
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
#line 10894 "parser.c"
    break;

  case 1039: /* write_lock: %empty  */
#line 5575 "parser.y"
                                { yyval = NULL; }
#line 10900 "parser.c"
    break;

  case 1040: /* write_lock: _with LOCK  */
#line 5577 "parser.y"
  {
	yyval = cb_int1;
  }
#line 10908 "parser.c"
    break;

  case 1041: /* write_lock: _with NO LOCK  */
#line 5581 "parser.y"
  {
	yyval = cb_int2;
  }
#line 10916 "parser.c"
    break;

  case 1042: /* end_rewrite: %empty  */
#line 5587 "parser.y"
                                { terminator_warning (TERM_REWRITE); }
#line 10922 "parser.c"
    break;

  case 1043: /* end_rewrite: "END-REWRITE"  */
#line 5588 "parser.y"
                                { terminator_clear (TERM_REWRITE); }
#line 10928 "parser.c"
    break;

  case 1044: /* rollback_statement: ROLLBACK  */
#line 5598 "parser.y"
  {
	BEGIN_STATEMENT ("ROLLBACK", 0);
	cb_emit_rollback ();
  }
#line 10937 "parser.c"
    break;

  case 1045: /* $@76: %empty  */
#line 5610 "parser.y"
                                { BEGIN_STATEMENT ("SEARCH", TERM_SEARCH); }
#line 10943 "parser.c"
    break;

  case 1047: /* search_body: table_name search_varying search_at_end search_whens  */
#line 5617 "parser.y"
  {
	cb_emit_search (yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]);
  }
#line 10951 "parser.c"
    break;

  case 1048: /* $@77: %empty  */
#line 5621 "parser.y"
  {
	check_unreached = 0;
  }
#line 10959 "parser.c"
    break;

  case 1049: /* search_body: ALL table_name search_at_end WHEN expr $@77 statement_list  */
#line 5625 "parser.y"
  {
	cb_emit_search_all (yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 10967 "parser.c"
    break;

  case 1050: /* search_varying: %empty  */
#line 5631 "parser.y"
                                { yyval = NULL; }
#line 10973 "parser.c"
    break;

  case 1051: /* search_varying: VARYING identifier  */
#line 5632 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10979 "parser.c"
    break;

  case 1052: /* search_at_end: %empty  */
#line 5636 "parser.y"
                                { yyval = NULL; }
#line 10985 "parser.c"
    break;

  case 1053: /* $@78: %empty  */
#line 5638 "parser.y"
  {
	check_unreached = 0;
  }
#line 10993 "parser.c"
    break;

  case 1054: /* search_at_end: _at END $@78 statement_list  */
#line 5642 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 11001 "parser.c"
    break;

  case 1055: /* search_whens: search_when  */
#line 5648 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11007 "parser.c"
    break;

  case 1056: /* search_whens: search_when search_whens  */
#line 5649 "parser.y"
                                { yyval = yyvsp[-1]; CB_IF (yyvsp[-1])->stmt2 = yyvsp[0]; }
#line 11013 "parser.c"
    break;

  case 1057: /* $@79: %empty  */
#line 5654 "parser.y"
  {
	check_unreached = 0;
  }
#line 11021 "parser.c"
    break;

  case 1058: /* search_when: WHEN condition $@79 statement_list  */
#line 5658 "parser.y"
  {
	yyval = cb_build_if (yyvsp[-2], yyvsp[0], NULL);
  }
#line 11029 "parser.c"
    break;

  case 1059: /* end_search: %empty  */
#line 5664 "parser.y"
                                { terminator_warning (TERM_SEARCH); }
#line 11035 "parser.c"
    break;

  case 1060: /* end_search: "END-SEARCH"  */
#line 5665 "parser.y"
                                { terminator_clear (TERM_SEARCH); }
#line 11041 "parser.c"
    break;

  case 1061: /* $@80: %empty  */
#line 5674 "parser.y"
                                { BEGIN_STATEMENT ("SET", 0); }
#line 11047 "parser.c"
    break;

  case 1068: /* set_environment: ENVIRONMENT simple_value TO simple_value  */
#line 5690 "parser.y"
  {
	cb_emit_setenv (yyvsp[-2], yyvsp[0]);
  }
#line 11055 "parser.c"
    break;

  case 1069: /* set_to: target_x_list TO ENTRY alnum_or_id  */
#line 5699 "parser.y"
  {
	cb_emit_set_to (yyvsp[-3], cb_build_ppointer (yyvsp[0]));
  }
#line 11063 "parser.c"
    break;

  case 1070: /* set_to: target_x_list TO x  */
#line 5703 "parser.y"
  {
	cb_emit_set_to (yyvsp[-2], yyvsp[0]);
  }
#line 11071 "parser.c"
    break;

  case 1071: /* set_up_down: target_x_list up_or_down BY x  */
#line 5712 "parser.y"
  {
	cb_emit_set_up_down (yyvsp[-3], yyvsp[-2], yyvsp[0]);
  }
#line 11079 "parser.c"
    break;

  case 1072: /* up_or_down: UP  */
#line 5718 "parser.y"
                                { yyval = cb_int0; }
#line 11085 "parser.c"
    break;

  case 1073: /* up_or_down: DOWN  */
#line 5719 "parser.y"
                                { yyval = cb_int1; }
#line 11091 "parser.c"
    break;

  case 1076: /* set_to_on_off: mnemonic_name_list TO on_or_off  */
#line 5731 "parser.y"
  {
	cb_emit_set_on_off (yyvsp[-2], yyvsp[0]);
  }
#line 11099 "parser.c"
    break;

  case 1079: /* set_to_true_false: target_x_list TO "TRUE"  */
#line 5745 "parser.y"
  {
	cb_emit_set_true (yyvsp[-2]);
  }
#line 11107 "parser.c"
    break;

  case 1080: /* set_to_true_false: target_x_list TO "FALSE"  */
#line 5749 "parser.y"
  {
	cb_emit_set_false (yyvsp[-2]);
  }
#line 11115 "parser.c"
    break;

  case 1081: /* $@81: %empty  */
#line 5760 "parser.y"
                                { BEGIN_STATEMENT ("SORT", 0); }
#line 11121 "parser.c"
    break;

  case 1083: /* $@82: %empty  */
#line 5766 "parser.y"
  {
	cb_emit_sort_init (yyvsp[-3], yyvsp[-2], yyvsp[0]);
	if (CB_FILE_P (cb_ref (yyvsp[-3])) && yyvsp[-2] == NULL) {
		cb_error (_("File sort requires KEY phrase"));
	}
	/* used in sort_input/sort_output */
	save_tree_1 = yyvsp[-3];
  }
#line 11134 "parser.c"
    break;

  case 1084: /* sort_body: qualified_word sort_key_list sort_duplicates sort_collating $@82 sort_input sort_output  */
#line 5775 "parser.y"
  {
	cb_emit_sort_finish (yyvsp[-6]);
  }
#line 11142 "parser.c"
    break;

  case 1085: /* sort_key_list: %empty  */
#line 5782 "parser.y"
  {
	yyval = NULL;
  }
#line 11150 "parser.c"
    break;

  case 1086: /* sort_key_list: sort_key_list _on ascending_or_descending _key _is opt_key_list  */
#line 5787 "parser.y"
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
#line 11171 "parser.c"
    break;

  case 1087: /* opt_key_list: %empty  */
#line 5806 "parser.y"
                                { yyval = NULL; }
#line 11177 "parser.c"
    break;

  case 1088: /* opt_key_list: opt_key_list qualified_word  */
#line 5807 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 11183 "parser.c"
    break;

  case 1090: /* sort_duplicates: with_dups _in_order  */
#line 5811 "parser.y"
                                { /* nothing */ }
#line 11189 "parser.c"
    break;

  case 1091: /* sort_collating: %empty  */
#line 5815 "parser.y"
                                        { yyval = cb_null; }
#line 11195 "parser.c"
    break;

  case 1092: /* sort_collating: coll_sequence _is reference  */
#line 5816 "parser.y"
                                        { yyval = cb_ref (yyvsp[0]); }
#line 11201 "parser.c"
    break;

  case 1093: /* sort_input: %empty  */
#line 5821 "parser.y"
  {
	if (CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("File sort requires USING or INPUT PROCEDURE"));
	}
  }
#line 11211 "parser.c"
    break;

  case 1094: /* sort_input: USING file_name_list  */
#line 5827 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("USING invalid with table SORT"));
	} else {
		cb_emit_sort_using (save_tree_1, yyvsp[0]);
	}
  }
#line 11223 "parser.c"
    break;

  case 1095: /* sort_input: INPUT PROCEDURE _is perform_procedure  */
#line 5835 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("INPUT PROCEDURE invalid with table SORT"));
	} else {
		cb_emit_sort_input (yyvsp[0], save_tree_1);
	}
  }
#line 11235 "parser.c"
    break;

  case 1096: /* sort_output: %empty  */
#line 5846 "parser.y"
  {
	if (CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("File sort requires GIVING or OUTPUT PROCEDURE"));
	}
  }
#line 11245 "parser.c"
    break;

  case 1097: /* sort_output: GIVING file_name_list  */
#line 5852 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("GIVING invalid with table SORT"));
	} else {
		cb_emit_sort_giving (save_tree_1, yyvsp[0]);
	}
  }
#line 11257 "parser.c"
    break;

  case 1098: /* sort_output: OUTPUT PROCEDURE _is perform_procedure  */
#line 5860 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("OUTPUT PROCEDURE invalid with table SORT"));
	} else {
		cb_emit_sort_output (yyvsp[0], save_tree_1);
	}
  }
#line 11269 "parser.c"
    break;

  case 1099: /* $@83: %empty  */
#line 5875 "parser.y"
                                { BEGIN_STATEMENT ("START", TERM_START); }
#line 11275 "parser.c"
    break;

  case 1100: /* @84: %empty  */
#line 5876 "parser.y"
                                { yyval = cb_int (COB_EQ); }
#line 11281 "parser.c"
    break;

  case 1101: /* start_statement: START $@83 file_name @84 start_key opt_invalid_key end_start  */
#line 5879 "parser.y"
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
#line 11300 "parser.c"
    break;

  case 1102: /* start_key: %empty  */
#line 5896 "parser.y"
                                { yyval = NULL; }
#line 11306 "parser.c"
    break;

  case 1103: /* start_key: KEY _is start_op identifier_list  */
#line 5898 "parser.y"
  {
	yyvsp[-4] = yyvsp[-1];
	yyval = yyvsp[0];
  }
#line 11315 "parser.c"
    break;

  case 1104: /* start_op: flag_not eq  */
#line 5905 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_NE : COB_EQ); }
#line 11321 "parser.c"
    break;

  case 1105: /* start_op: flag_not gt  */
#line 5906 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_LE : COB_GT); }
#line 11327 "parser.c"
    break;

  case 1106: /* start_op: flag_not lt  */
#line 5907 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_GE : COB_LT); }
#line 11333 "parser.c"
    break;

  case 1107: /* start_op: flag_not ge  */
#line 5908 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_LT : COB_GE); }
#line 11339 "parser.c"
    break;

  case 1108: /* start_op: flag_not le  */
#line 5909 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_GT : COB_LE); }
#line 11345 "parser.c"
    break;

  case 1109: /* end_start: %empty  */
#line 5913 "parser.y"
                                { terminator_warning (TERM_START); }
#line 11351 "parser.c"
    break;

  case 1110: /* end_start: "END-START"  */
#line 5914 "parser.y"
                                { terminator_clear (TERM_START); }
#line 11357 "parser.c"
    break;

  case 1111: /* $@85: %empty  */
#line 5923 "parser.y"
                                { BEGIN_STATEMENT ("STOP", 0); }
#line 11363 "parser.c"
    break;

  case 1112: /* stop_statement: STOP RUN $@85 stop_returning  */
#line 5925 "parser.y"
  {
	cb_emit_stop_run (yyvsp[0]);
  }
#line 11371 "parser.c"
    break;

  case 1113: /* $@86: %empty  */
#line 5928 "parser.y"
                                { BEGIN_STATEMENT ("STOP", 0); }
#line 11377 "parser.c"
    break;

  case 1114: /* stop_statement: STOP "Literal" $@86  */
#line 5929 "parser.y"
  {
	cb_verify (cb_stop_literal_statement, "STOP literal");
  }
#line 11385 "parser.c"
    break;

  case 1115: /* stop_returning: %empty  */
#line 5935 "parser.y"
                        { yyval = current_program->cb_return_code; }
#line 11391 "parser.c"
    break;

  case 1116: /* stop_returning: RETURNING x  */
#line 5936 "parser.y"
                        { yyval = yyvsp[0]; }
#line 11397 "parser.c"
    break;

  case 1117: /* stop_returning: GIVING x  */
#line 5937 "parser.y"
                        { yyval = yyvsp[0]; }
#line 11403 "parser.c"
    break;

  case 1118: /* $@87: %empty  */
#line 5946 "parser.y"
                                { BEGIN_STATEMENT ("STRING", TERM_STRING); }
#line 11409 "parser.c"
    break;

  case 1119: /* string_statement: STRING $@87 string_item_list INTO identifier opt_with_pointer on_overflow end_string  */
#line 5949 "parser.y"
  {
	cb_emit_string (yyvsp[-5], yyvsp[-3], yyvsp[-2]);
  }
#line 11417 "parser.c"
    break;

  case 1120: /* string_item_list: string_item  */
#line 5955 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 11423 "parser.c"
    break;

  case 1121: /* string_item_list: string_item_list string_item  */
#line 5956 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 11429 "parser.c"
    break;

  case 1122: /* string_item: x  */
#line 5960 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11435 "parser.c"
    break;

  case 1123: /* string_item: DELIMITED _by SIZE  */
#line 5961 "parser.y"
                                { yyval = cb_build_pair (cb_int0, NULL); }
#line 11441 "parser.c"
    break;

  case 1124: /* string_item: DELIMITED _by x  */
#line 5962 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], NULL); }
#line 11447 "parser.c"
    break;

  case 1125: /* opt_with_pointer: %empty  */
#line 5966 "parser.y"
                                { yyval = cb_int0; }
#line 11453 "parser.c"
    break;

  case 1126: /* opt_with_pointer: _with POINTER identifier  */
#line 5967 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11459 "parser.c"
    break;

  case 1127: /* end_string: %empty  */
#line 5971 "parser.y"
                                { terminator_warning (TERM_STRING); }
#line 11465 "parser.c"
    break;

  case 1128: /* end_string: "END-STRING"  */
#line 5972 "parser.y"
                                { terminator_clear (TERM_STRING); }
#line 11471 "parser.c"
    break;

  case 1129: /* $@88: %empty  */
#line 5981 "parser.y"
                                { BEGIN_STATEMENT ("SUBTRACT", TERM_SUBTRACT); }
#line 11477 "parser.c"
    break;

  case 1131: /* subtract_body: x_list FROM arithmetic_x_list on_size_error  */
#line 5988 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '-', cb_build_binary_list (yyvsp[-3], '+'));
  }
#line 11485 "parser.c"
    break;

  case 1132: /* subtract_body: x_list FROM x GIVING arithmetic_x_list on_size_error  */
#line 5992 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_list (cb_cons (yyvsp[-3], yyvsp[-5]), '-'));
  }
#line 11493 "parser.c"
    break;

  case 1133: /* subtract_body: CORRESPONDING identifier FROM identifier flag_rounded on_size_error  */
#line 5996 "parser.y"
  {
	cb_emit_corresponding (cb_build_sub, yyvsp[-2], yyvsp[-4], yyvsp[-1]);
  }
#line 11501 "parser.c"
    break;

  case 1134: /* end_subtract: %empty  */
#line 6002 "parser.y"
                                { terminator_warning (TERM_SUBTRACT); }
#line 11507 "parser.c"
    break;

  case 1135: /* end_subtract: "END-SUBTRACT"  */
#line 6003 "parser.y"
                                { terminator_clear (TERM_SUBTRACT); }
#line 11513 "parser.c"
    break;

  case 1136: /* suppress_statement: SUPPRESS _printing  */
#line 6013 "parser.y"
  {
	BEGIN_STATEMENT ("SUPPRESS", 0);
	PENDING("SUPPRESS");
  }
#line 11522 "parser.c"
    break;

  case 1139: /* $@89: %empty  */
#line 6028 "parser.y"
                                { BEGIN_STATEMENT ("TERMINATE", 0); }
#line 11528 "parser.c"
    break;

  case 1140: /* terminate_statement: TERMINATE $@89 identifier_list  */
#line 6030 "parser.y"
  {
	PENDING("TERMINATE");
  }
#line 11536 "parser.c"
    break;

  case 1141: /* $@90: %empty  */
#line 6041 "parser.y"
                                { BEGIN_STATEMENT ("TRANSFORM", 0); }
#line 11542 "parser.c"
    break;

  case 1142: /* transform_statement: TRANSFORM $@90 identifier FROM simple_value TO simple_all_value  */
#line 6043 "parser.y"
  {
	cb_tree		x;

	x = cb_build_converting (yyvsp[-2], yyvsp[0], cb_build_inspect_region_start ());
	cb_emit_inspect (yyvsp[-4], x, cb_int0, 2);
  }
#line 11553 "parser.c"
    break;

  case 1143: /* $@91: %empty  */
#line 6057 "parser.y"
                                { BEGIN_STATEMENT ("UNLOCK", 0); }
#line 11559 "parser.c"
    break;

  case 1144: /* unlock_statement: UNLOCK $@91 file_name opt_record  */
#line 6059 "parser.y"
  {
	if (yyvsp[-1] != cb_error_node) {
		cb_emit_unlock (yyvsp[-1]);
	}
  }
#line 11569 "parser.c"
    break;

  case 1148: /* $@92: %empty  */
#line 6078 "parser.y"
                                { BEGIN_STATEMENT ("UNSTRING", TERM_UNSTRING); }
#line 11575 "parser.c"
    break;

  case 1149: /* unstring_statement: UNSTRING $@92 identifier unstring_delimited unstring_into opt_with_pointer unstring_tallying on_overflow end_unstring  */
#line 6082 "parser.y"
  {
	cb_emit_unstring (yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2]);
  }
#line 11583 "parser.c"
    break;

  case 1150: /* unstring_delimited: %empty  */
#line 6088 "parser.y"
                                { yyval = NULL; }
#line 11589 "parser.c"
    break;

  case 1151: /* unstring_delimited: DELIMITED _by unstring_delimited_list  */
#line 6090 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11595 "parser.c"
    break;

  case 1152: /* unstring_delimited_list: unstring_delimited_item  */
#line 6094 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 11601 "parser.c"
    break;

  case 1153: /* unstring_delimited_list: unstring_delimited_list OR unstring_delimited_item  */
#line 6096 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 11607 "parser.c"
    break;

  case 1154: /* unstring_delimited_item: flag_all simple_value  */
#line 6101 "parser.y"
  {
	yyval = cb_build_unstring_delimited (yyvsp[-1], yyvsp[0]);
  }
#line 11615 "parser.c"
    break;

  case 1155: /* unstring_into: INTO unstring_into_item  */
#line 6107 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 11621 "parser.c"
    break;

  case 1156: /* unstring_into: unstring_into unstring_into_item  */
#line 6109 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 11627 "parser.c"
    break;

  case 1157: /* unstring_into_item: identifier unstring_into_delimiter unstring_into_count  */
#line 6114 "parser.y"
  {
	yyval = cb_build_unstring_into (yyvsp[-2], yyvsp[-1], yyvsp[0]);
  }
#line 11635 "parser.c"
    break;

  case 1158: /* unstring_into_delimiter: %empty  */
#line 6120 "parser.y"
                                { yyval = NULL; }
#line 11641 "parser.c"
    break;

  case 1159: /* unstring_into_delimiter: DELIMITER _in identifier  */
#line 6121 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11647 "parser.c"
    break;

  case 1160: /* unstring_into_count: %empty  */
#line 6125 "parser.y"
                                { yyval = NULL; }
#line 11653 "parser.c"
    break;

  case 1161: /* unstring_into_count: COUNT _in identifier  */
#line 6126 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11659 "parser.c"
    break;

  case 1162: /* unstring_tallying: %empty  */
#line 6130 "parser.y"
                                { yyval = NULL; }
#line 11665 "parser.c"
    break;

  case 1163: /* unstring_tallying: TALLYING _in identifier  */
#line 6131 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11671 "parser.c"
    break;

  case 1164: /* end_unstring: %empty  */
#line 6135 "parser.y"
                                { terminator_warning (TERM_UNSTRING); }
#line 11677 "parser.c"
    break;

  case 1165: /* end_unstring: "END-UNSTRING"  */
#line 6136 "parser.y"
                                { terminator_clear (TERM_UNSTRING); }
#line 11683 "parser.c"
    break;

  case 1169: /* use_exception: USE use_global _after _standard exception_or_error _procedure _on use_exception_target  */
#line 6154 "parser.y"
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
#line 11705 "parser.c"
    break;

  case 1170: /* use_global: %empty  */
#line 6175 "parser.y"
  {
	use_global_ind = 0;
  }
#line 11713 "parser.c"
    break;

  case 1171: /* use_global: GLOBAL  */
#line 6179 "parser.y"
  {
	use_global_ind = 1;
	current_program->flag_global_use = 1;
  }
#line 11722 "parser.c"
    break;

  case 1172: /* use_exception_target: file_name_list  */
#line 6187 "parser.y"
  {
	cb_tree		l;

	for (l = yyvsp[0]; l; l = CB_CHAIN (l)) {
		if (CB_VALUE (l) != cb_error_node) {
			setup_use_file (CB_FILE (cb_ref (CB_VALUE (l))));
		}
	}
  }
#line 11736 "parser.c"
    break;

  case 1173: /* use_exception_target: INPUT  */
#line 6197 "parser.y"
  {
	current_program->global_handler[COB_OPEN_INPUT].handler_label = current_section;
	current_program->global_handler[COB_OPEN_INPUT].handler_prog = current_program;
  }
#line 11745 "parser.c"
    break;

  case 1174: /* use_exception_target: OUTPUT  */
#line 6202 "parser.y"
  {
	current_program->global_handler[COB_OPEN_OUTPUT].handler_label = current_section;
	current_program->global_handler[COB_OPEN_OUTPUT].handler_prog = current_program;
  }
#line 11754 "parser.c"
    break;

  case 1175: /* use_exception_target: "I-O"  */
#line 6207 "parser.y"
  {
	current_program->global_handler[COB_OPEN_I_O].handler_label = current_section;
	current_program->global_handler[COB_OPEN_I_O].handler_prog = current_program;
  }
#line 11763 "parser.c"
    break;

  case 1176: /* use_exception_target: EXTEND  */
#line 6212 "parser.y"
  {
	current_program->global_handler[COB_OPEN_EXTEND].handler_label = current_section;
	current_program->global_handler[COB_OPEN_EXTEND].handler_prog = current_program;
  }
#line 11772 "parser.c"
    break;

  case 1189: /* use_debugging: USE _for DEBUGGING _on use_debugging_target  */
#line 6244 "parser.y"
  {
	PENDING ("USE FOR DEBUGGING");
  }
#line 11780 "parser.c"
    break;

  case 1192: /* use_reporting: USE use_global BEFORE REPORTING identifier  */
#line 6256 "parser.y"
  {
	PENDING ("USE BEFORE REPORTING");
  }
#line 11788 "parser.c"
    break;

  case 1193: /* $@93: %empty  */
#line 6267 "parser.y"
                                { BEGIN_STATEMENT ("WRITE", TERM_WRITE); }
#line 11794 "parser.c"
    break;

  case 1194: /* write_statement: WRITE $@93 record_name write_from write_lock write_option write_handler end_write  */
#line 6270 "parser.y"
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
#line 11810 "parser.c"
    break;

  case 1195: /* write_from: %empty  */
#line 6284 "parser.y"
                                { yyval = NULL; }
#line 11816 "parser.c"
    break;

  case 1196: /* write_from: FROM id_or_lit  */
#line 6285 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11822 "parser.c"
    break;

  case 1197: /* write_option: %empty  */
#line 6290 "parser.y"
  {
	yyval = cb_int0;
  }
#line 11830 "parser.c"
    break;

  case 1198: /* write_option: before_or_after _advancing num_id_or_lit _line_or_lines  */
#line 6294 "parser.y"
  {
	yyval = cb_build_write_advancing_lines (yyvsp[-3], yyvsp[-1]);
  }
#line 11838 "parser.c"
    break;

  case 1199: /* write_option: before_or_after _advancing mnemonic_name  */
#line 6298 "parser.y"
  {
	yyval = cb_build_write_advancing_mnemonic (yyvsp[-2], yyvsp[0]);
  }
#line 11846 "parser.c"
    break;

  case 1200: /* write_option: before_or_after _advancing PAGE  */
#line 6302 "parser.y"
  {
	yyval = cb_build_write_advancing_page (yyvsp[-2]);
  }
#line 11854 "parser.c"
    break;

  case 1201: /* before_or_after: BEFORE  */
#line 6308 "parser.y"
                                { yyval = CB_BEFORE; }
#line 11860 "parser.c"
    break;

  case 1202: /* before_or_after: AFTER  */
#line 6309 "parser.y"
                                { yyval = CB_AFTER; }
#line 11866 "parser.c"
    break;

  case 1206: /* end_write: %empty  */
#line 6318 "parser.y"
                                { terminator_warning (TERM_WRITE); }
#line 11872 "parser.c"
    break;

  case 1207: /* end_write: "END-WRITE"  */
#line 6319 "parser.y"
                                { terminator_clear (TERM_WRITE); }
#line 11878 "parser.c"
    break;

  case 1208: /* on_accp_exception: opt_on_exception opt_not_on_exception  */
#line 6334 "parser.y"
  {
	current_statement->handler_id = COB_EC_IMP_ACCEPT;
  }
#line 11886 "parser.c"
    break;

  case 1209: /* on_disp_exception: opt_on_exception opt_not_on_exception  */
#line 6342 "parser.y"
  {
	current_statement->handler_id = COB_EC_IMP_DISPLAY;
  }
#line 11894 "parser.c"
    break;

  case 1211: /* $@94: %empty  */
#line 6349 "parser.y"
  {
	check_unreached = 0;
  }
#line 11902 "parser.c"
    break;

  case 1212: /* opt_on_exception: EXCEPTION $@94 statement_list  */
#line 6353 "parser.y"
  {
	current_statement->handler1 = yyvsp[0];
  }
#line 11910 "parser.c"
    break;

  case 1214: /* $@95: %empty  */
#line 6360 "parser.y"
  {
	check_unreached = 0;
  }
#line 11918 "parser.c"
    break;

  case 1215: /* opt_not_on_exception: "NOT EXCEPTION" $@95 statement_list  */
#line 6364 "parser.y"
  {
	current_statement->handler2 = yyvsp[0];
  }
#line 11926 "parser.c"
    break;

  case 1218: /* $@96: %empty  */
#line 6380 "parser.y"
  {
	check_unreached = 0;
	current_statement->handler_id = COB_EC_SIZE;
  }
#line 11935 "parser.c"
    break;

  case 1219: /* opt_on_size_error: "SIZE ERROR" $@96 statement_list  */
#line 6385 "parser.y"
  {
	current_statement->handler1 = yyvsp[0];
  }
#line 11943 "parser.c"
    break;

  case 1221: /* $@97: %empty  */
#line 6392 "parser.y"
  {
	check_unreached = 0;
	current_statement->handler_id = COB_EC_SIZE;
  }
#line 11952 "parser.c"
    break;

  case 1222: /* opt_not_on_size_error: "NOT SIZE ERROR" $@97 statement_list  */
#line 6397 "parser.y"
  {
	current_statement->handler2 = yyvsp[0];
  }
#line 11960 "parser.c"
    break;

  case 1223: /* on_overflow: opt_on_overflow opt_not_on_overflow  */
#line 6409 "parser.y"
  {
	current_statement->handler_id = COB_EC_OVERFLOW;
  }
#line 11968 "parser.c"
    break;

  case 1225: /* $@98: %empty  */
#line 6416 "parser.y"
  {
	check_unreached = 0;
  }
#line 11976 "parser.c"
    break;

  case 1226: /* opt_on_overflow: OVERFLOW $@98 statement_list  */
#line 6420 "parser.y"
  {
	current_statement->handler1 = yyvsp[0];
  }
#line 11984 "parser.c"
    break;

  case 1228: /* $@99: %empty  */
#line 6427 "parser.y"
  {
	check_unreached = 0;
  }
#line 11992 "parser.c"
    break;

  case 1229: /* opt_not_on_overflow: "NOT OVERFLOW" $@99 statement_list  */
#line 6431 "parser.y"
  {
	current_statement->handler2 = yyvsp[0];
  }
#line 12000 "parser.c"
    break;

  case 1230: /* at_end: at_end_sentence  */
#line 6443 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_AT_END;
	current_statement->handler1 = yyvsp[0];
  }
#line 12009 "parser.c"
    break;

  case 1231: /* at_end: not_at_end_sentence  */
#line 6448 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_AT_END;
	current_statement->handler2 = yyvsp[0];
  }
#line 12018 "parser.c"
    break;

  case 1232: /* at_end: at_end_sentence not_at_end_sentence  */
#line 6453 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_AT_END;
	current_statement->handler1 = yyvsp[-1];
	current_statement->handler2 = yyvsp[0];
  }
#line 12028 "parser.c"
    break;

  case 1233: /* $@100: %empty  */
#line 6462 "parser.y"
  {
	check_unreached = 0;
  }
#line 12036 "parser.c"
    break;

  case 1234: /* at_end_sentence: END $@100 statement_list  */
#line 6466 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12044 "parser.c"
    break;

  case 1235: /* $@101: %empty  */
#line 6473 "parser.y"
  {
	check_unreached = 0;
  }
#line 12052 "parser.c"
    break;

  case 1236: /* not_at_end_sentence: "NOT END" $@101 statement_list  */
#line 6477 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12060 "parser.c"
    break;

  case 1237: /* at_eop: at_eop_sentence  */
#line 6489 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_EOP;
	current_statement->handler1 = yyvsp[0];
  }
#line 12069 "parser.c"
    break;

  case 1238: /* at_eop: not_at_eop_sentence  */
#line 6494 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_EOP;
	current_statement->handler2 = yyvsp[0];
  }
#line 12078 "parser.c"
    break;

  case 1239: /* at_eop: at_eop_sentence not_at_eop_sentence  */
#line 6499 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_EOP;
	current_statement->handler1 = yyvsp[-1];
	current_statement->handler2 = yyvsp[0];
  }
#line 12088 "parser.c"
    break;

  case 1240: /* $@102: %empty  */
#line 6508 "parser.y"
  {
	check_unreached = 0;
  }
#line 12096 "parser.c"
    break;

  case 1241: /* at_eop_sentence: EOP $@102 statement_list  */
#line 6512 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12104 "parser.c"
    break;

  case 1242: /* $@103: %empty  */
#line 6519 "parser.y"
  {
	check_unreached = 0;
  }
#line 12112 "parser.c"
    break;

  case 1243: /* not_at_eop_sentence: "NOT EOP" $@103 statement_list  */
#line 6523 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12120 "parser.c"
    break;

  case 1246: /* invalid_key: invalid_key_sentence  */
#line 6539 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_INVALID_KEY;
	current_statement->handler1 = yyvsp[0];
  }
#line 12129 "parser.c"
    break;

  case 1247: /* invalid_key: not_invalid_key_sentence  */
#line 6544 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_INVALID_KEY;
	current_statement->handler2 = yyvsp[0];
  }
#line 12138 "parser.c"
    break;

  case 1248: /* invalid_key: invalid_key_sentence not_invalid_key_sentence  */
#line 6549 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_INVALID_KEY;
	current_statement->handler1 = yyvsp[-1];
	current_statement->handler2 = yyvsp[0];
  }
#line 12148 "parser.c"
    break;

  case 1249: /* $@104: %empty  */
#line 6558 "parser.y"
  {
	check_unreached = 0;
  }
#line 12156 "parser.c"
    break;

  case 1250: /* invalid_key_sentence: "INVALID KEY" $@104 statement_list  */
#line 6562 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12164 "parser.c"
    break;

  case 1251: /* $@105: %empty  */
#line 6569 "parser.y"
  {
	check_unreached = 0;
  }
#line 12172 "parser.c"
    break;

  case 1252: /* not_invalid_key_sentence: "NOT INVALID KEY" $@105 statement_list  */
#line 6573 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12180 "parser.c"
    break;

  case 1253: /* _opt_scroll_lines: %empty  */
#line 6585 "parser.y"
  {
	yyval = cb_one;
  }
#line 12188 "parser.c"
    break;

  case 1254: /* _opt_scroll_lines: _by num_id_or_lit _line_or_lines  */
#line 6589 "parser.y"
  {
	yyval = yyvsp[-1];
  }
#line 12196 "parser.c"
    break;

  case 1255: /* condition: expr  */
#line 6601 "parser.y"
  {
	yyval = cb_build_cond (yyvsp[0]);
  }
#line 12204 "parser.c"
    break;

  case 1256: /* expr: partial_expr  */
#line 6608 "parser.y"
  {
	yyval = cb_build_expr (yyvsp[0]);
  }
#line 12212 "parser.c"
    break;

  case 1257: /* $@106: %empty  */
#line 6614 "parser.y"
  {
	current_expr = NULL;
  }
#line 12220 "parser.c"
    break;

  case 1258: /* partial_expr: $@106 expr_tokens  */
#line 6618 "parser.y"
  {
	yyval = cb_list_reverse (current_expr);
  }
#line 12228 "parser.c"
    break;

  case 1259: /* expr_tokens: expr_token x  */
#line 6624 "parser.y"
                        { push_expr ('x', yyvsp[0]); }
#line 12234 "parser.c"
    break;

  case 1260: /* expr_tokens: expr_tokens ')'  */
#line 6625 "parser.y"
                        { push_expr (')', NULL); }
#line 12240 "parser.c"
    break;

  case 1261: /* expr_tokens: expr_token OMITTED  */
#line 6627 "parser.y"
                                { push_expr ('O', NULL); }
#line 12246 "parser.c"
    break;

  case 1262: /* expr_tokens: expr_token NUMERIC  */
#line 6628 "parser.y"
                                { push_expr ('9', NULL); }
#line 12252 "parser.c"
    break;

  case 1263: /* expr_tokens: expr_token ALPHABETIC  */
#line 6629 "parser.y"
                                { push_expr ('A', NULL); }
#line 12258 "parser.c"
    break;

  case 1264: /* expr_tokens: expr_token "ALPHABETIC-LOWER"  */
#line 6630 "parser.y"
                                { push_expr ('L', NULL); }
#line 12264 "parser.c"
    break;

  case 1265: /* expr_tokens: expr_token "ALPHABETIC-UPPER"  */
#line 6631 "parser.y"
                                { push_expr ('U', NULL); }
#line 12270 "parser.c"
    break;

  case 1266: /* expr_tokens: expr_token CLASS_NAME  */
#line 6632 "parser.y"
                                { push_expr ('x', yyvsp[0]); }
#line 12276 "parser.c"
    break;

  case 1267: /* expr_tokens: expr_tokens OMITTED  */
#line 6634 "parser.y"
                                        { push_expr ('O', NULL); }
#line 12282 "parser.c"
    break;

  case 1268: /* expr_tokens: expr_tokens NUMERIC  */
#line 6635 "parser.y"
                                        { push_expr ('9', NULL); }
#line 12288 "parser.c"
    break;

  case 1269: /* expr_tokens: expr_tokens ALPHABETIC  */
#line 6636 "parser.y"
                                        { push_expr ('A', NULL); }
#line 12294 "parser.c"
    break;

  case 1270: /* expr_tokens: expr_tokens "ALPHABETIC-LOWER"  */
#line 6637 "parser.y"
                                        { push_expr ('L', NULL); }
#line 12300 "parser.c"
    break;

  case 1271: /* expr_tokens: expr_tokens "ALPHABETIC-UPPER"  */
#line 6638 "parser.y"
                                        { push_expr ('U', NULL); }
#line 12306 "parser.c"
    break;

  case 1272: /* expr_tokens: expr_tokens CLASS_NAME  */
#line 6639 "parser.y"
                                        { push_expr ('x', yyvsp[0]); }
#line 12312 "parser.c"
    break;

  case 1273: /* expr_tokens: expr_token POSITIVE  */
#line 6641 "parser.y"
                        { push_expr ('P', NULL); }
#line 12318 "parser.c"
    break;

  case 1274: /* expr_tokens: expr_token NEGATIVE  */
#line 6642 "parser.y"
                        { push_expr ('N', NULL); }
#line 12324 "parser.c"
    break;

  case 1275: /* expr_tokens: expr_tokens POSITIVE  */
#line 6644 "parser.y"
                        { push_expr ('P', NULL); }
#line 12330 "parser.c"
    break;

  case 1276: /* expr_tokens: expr_tokens NEGATIVE  */
#line 6645 "parser.y"
                        { push_expr ('N', NULL); }
#line 12336 "parser.c"
    break;

  case 1277: /* expr_tokens: expr_tokens ZERO  */
#line 6646 "parser.y"
                        { push_expr ('x', cb_zero); }
#line 12342 "parser.c"
    break;

  case 1281: /* expr_token: expr_token '('  */
#line 6653 "parser.y"
                        { push_expr ('(', NULL); }
#line 12348 "parser.c"
    break;

  case 1282: /* expr_token: expr_token '+'  */
#line 6655 "parser.y"
                        { push_expr ('+', NULL); }
#line 12354 "parser.c"
    break;

  case 1283: /* expr_token: expr_token '-'  */
#line 6656 "parser.y"
                        { push_expr ('-', NULL); }
#line 12360 "parser.c"
    break;

  case 1284: /* expr_token: expr_token '^'  */
#line 6657 "parser.y"
                        { push_expr ('^', NULL); }
#line 12366 "parser.c"
    break;

  case 1285: /* expr_token: expr_token NOT  */
#line 6659 "parser.y"
                        { push_expr ('!', NULL); }
#line 12372 "parser.c"
    break;

  case 1286: /* expr_token: expr_tokens NOT  */
#line 6660 "parser.y"
                        { push_expr ('!', NULL); }
#line 12378 "parser.c"
    break;

  case 1287: /* expr_token: expr_tokens '+'  */
#line 6662 "parser.y"
                        { push_expr ('+', NULL); }
#line 12384 "parser.c"
    break;

  case 1288: /* expr_token: expr_tokens '-'  */
#line 6663 "parser.y"
                        { push_expr ('-', NULL); }
#line 12390 "parser.c"
    break;

  case 1289: /* expr_token: expr_tokens '*'  */
#line 6664 "parser.y"
                        { push_expr ('*', NULL); }
#line 12396 "parser.c"
    break;

  case 1290: /* expr_token: expr_tokens '/'  */
#line 6665 "parser.y"
                        { push_expr ('/', NULL); }
#line 12402 "parser.c"
    break;

  case 1291: /* expr_token: expr_tokens '^'  */
#line 6666 "parser.y"
                        { push_expr ('^', NULL); }
#line 12408 "parser.c"
    break;

  case 1292: /* expr_token: expr_tokens eq  */
#line 6668 "parser.y"
                        { push_expr ('=', NULL); }
#line 12414 "parser.c"
    break;

  case 1293: /* expr_token: expr_tokens gt  */
#line 6669 "parser.y"
                        { push_expr ('>', NULL); }
#line 12420 "parser.c"
    break;

  case 1294: /* expr_token: expr_tokens lt  */
#line 6670 "parser.y"
                        { push_expr ('<', NULL); }
#line 12426 "parser.c"
    break;

  case 1295: /* expr_token: expr_tokens ge  */
#line 6671 "parser.y"
                        { push_expr (']', NULL); }
#line 12432 "parser.c"
    break;

  case 1296: /* expr_token: expr_tokens le  */
#line 6672 "parser.y"
                        { push_expr ('[', NULL); }
#line 12438 "parser.c"
    break;

  case 1297: /* expr_token: expr_tokens NE  */
#line 6673 "parser.y"
                        { push_expr ('~', NULL); }
#line 12444 "parser.c"
    break;

  case 1298: /* expr_token: expr_token eq  */
#line 6675 "parser.y"
                        { push_expr ('=', NULL); }
#line 12450 "parser.c"
    break;

  case 1299: /* expr_token: expr_token gt  */
#line 6676 "parser.y"
                        { push_expr ('>', NULL); }
#line 12456 "parser.c"
    break;

  case 1300: /* expr_token: expr_token lt  */
#line 6677 "parser.y"
                        { push_expr ('<', NULL); }
#line 12462 "parser.c"
    break;

  case 1301: /* expr_token: expr_token ge  */
#line 6678 "parser.y"
                        { push_expr (']', NULL); }
#line 12468 "parser.c"
    break;

  case 1302: /* expr_token: expr_token le  */
#line 6679 "parser.y"
                        { push_expr ('[', NULL); }
#line 12474 "parser.c"
    break;

  case 1303: /* expr_token: expr_token NE  */
#line 6680 "parser.y"
                        { push_expr ('~', NULL); }
#line 12480 "parser.c"
    break;

  case 1304: /* expr_token: expr_tokens AND  */
#line 6682 "parser.y"
                        { push_expr ('&', NULL); }
#line 12486 "parser.c"
    break;

  case 1305: /* expr_token: expr_tokens OR  */
#line 6683 "parser.y"
                        { push_expr ('|', NULL); }
#line 12492 "parser.c"
    break;

  case 1319: /* exp_list: exp  */
#line 6695 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12498 "parser.c"
    break;

  case 1320: /* exp_list: exp_list e_sep exp  */
#line 6696 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 12504 "parser.c"
    break;

  case 1324: /* exp: arith_x  */
#line 6705 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12510 "parser.c"
    break;

  case 1325: /* exp: exp '+' exp  */
#line 6706 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '+', yyvsp[0]); }
#line 12516 "parser.c"
    break;

  case 1326: /* exp: exp '-' exp  */
#line 6707 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '-', yyvsp[0]); }
#line 12522 "parser.c"
    break;

  case 1327: /* exp: exp '*' exp  */
#line 6708 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '*', yyvsp[0]); }
#line 12528 "parser.c"
    break;

  case 1328: /* exp: exp '/' exp  */
#line 6709 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '/', yyvsp[0]); }
#line 12534 "parser.c"
    break;

  case 1329: /* exp: '+' exp  */
#line 6710 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12540 "parser.c"
    break;

  case 1330: /* exp: '-' exp  */
#line 6711 "parser.y"
                                { yyval = cb_build_binary_op (cb_zero, '-', yyvsp[0]); }
#line 12546 "parser.c"
    break;

  case 1331: /* exp: exp '^' exp  */
#line 6712 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '^', yyvsp[0]); }
#line 12552 "parser.c"
    break;

  case 1332: /* exp: '(' exp ')'  */
#line 6713 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 12558 "parser.c"
    break;

  case 1333: /* linage_counter: "LINAGE-COUNTER"  */
#line 6725 "parser.y"
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
#line 12574 "parser.c"
    break;

  case 1334: /* linage_counter: "LINAGE-COUNTER" in_of "Identifier"  */
#line 6737 "parser.y"
  {
	if (CB_FILE_P (cb_ref (yyvsp[0]))) {
		yyval = CB_FILE (cb_ref (yyvsp[0]))->linage_ctr;
	} else {
		cb_error_x (yyvsp[0], _("'%s' is not a file name"), CB_NAME (yyvsp[0]));
		yyval = cb_error_node;
	}
  }
#line 12587 "parser.c"
    break;

  case 1335: /* arithmetic_x_list: arithmetic_x  */
#line 6751 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12593 "parser.c"
    break;

  case 1336: /* arithmetic_x_list: arithmetic_x_list arithmetic_x  */
#line 6753 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 12599 "parser.c"
    break;

  case 1337: /* arithmetic_x: x flag_rounded  */
#line 6757 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], yyvsp[-1]); }
#line 12605 "parser.c"
    break;

  case 1338: /* record_name: qualified_word  */
#line 6764 "parser.y"
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
#line 12626 "parser.c"
    break;

  case 1339: /* table_name: qualified_word  */
#line 6786 "parser.y"
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
#line 12645 "parser.c"
    break;

  case 1340: /* file_name_list: file_name  */
#line 6806 "parser.y"
  {
	yyval = cb_list_init (yyvsp[0]);
  }
#line 12653 "parser.c"
    break;

  case 1341: /* file_name_list: file_name_list file_name  */
#line 6810 "parser.y"
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
#line 12670 "parser.c"
    break;

  case 1342: /* file_name: "Identifier"  */
#line 6826 "parser.y"
  {
	if (CB_FILE_P (cb_ref (yyvsp[0]))) {
		yyval = yyvsp[0];
	} else {
		cb_error_x (yyvsp[0], _("'%s' is not a file name"), CB_NAME (yyvsp[0]));
		yyval = cb_error_node;
	}
  }
#line 12683 "parser.c"
    break;

  case 1343: /* mnemonic_name_list: mnemonic_name  */
#line 6839 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12689 "parser.c"
    break;

  case 1344: /* mnemonic_name_list: mnemonic_name_list mnemonic_name  */
#line 6841 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12695 "parser.c"
    break;

  case 1345: /* mnemonic_name: "MNEMONIC NAME"  */
#line 6845 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12701 "parser.c"
    break;

  case 1346: /* procedure_name_list: %empty  */
#line 6851 "parser.y"
                                { yyval = NULL; }
#line 12707 "parser.c"
    break;

  case 1347: /* procedure_name_list: procedure_name_list procedure_name  */
#line 6853 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12713 "parser.c"
    break;

  case 1348: /* procedure_name: label  */
#line 6858 "parser.y"
  {
	yyval = yyvsp[0];
	CB_REFERENCE (yyval)->offset = CB_TREE (current_section);
	current_program->label_list = cb_cons (yyval, current_program->label_list);
  }
#line 12723 "parser.c"
    break;

  case 1352: /* integer_label: "Literal"  */
#line 6873 "parser.y"
  {
	yyval = cb_build_reference ((char *)(CB_LITERAL (yyvsp[0])->data));
	yyval->source_file = yyvsp[0]->source_file;
	yyval->source_line = yyvsp[0]->source_line;
  }
#line 12733 "parser.c"
    break;

  case 1353: /* reference_list: reference  */
#line 6883 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12739 "parser.c"
    break;

  case 1354: /* reference_list: reference_list reference  */
#line 6884 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12745 "parser.c"
    break;

  case 1355: /* reference: qualified_word  */
#line 6889 "parser.y"
  {
	yyval = yyvsp[0];
	current_program->reference_list = cb_cons (yyval, current_program->reference_list);
  }
#line 12754 "parser.c"
    break;

  case 1356: /* no_reference_list: qualified_word  */
#line 6898 "parser.y"
                                        { yyval = cb_list_init (yyvsp[0]); }
#line 12760 "parser.c"
    break;

  case 1357: /* no_reference_list: no_reference_list qualified_word  */
#line 6899 "parser.y"
                                        { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12766 "parser.c"
    break;

  case 1358: /* opt_reference: %empty  */
#line 6903 "parser.y"
                                { yyval = NULL; }
#line 12772 "parser.c"
    break;

  case 1359: /* opt_reference: reference  */
#line 6904 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12778 "parser.c"
    break;

  case 1362: /* undefined_word: "Identifier"  */
#line 6916 "parser.y"
  {
	yyval = yyvsp[0];
	if (CB_REFERENCE (yyval)->word->count > 0) {
		redefinition_error (yyval);
		yyval = cb_error_node;
	}
  }
#line 12790 "parser.c"
    break;

  case 1363: /* target_x_list: target_x  */
#line 6935 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12796 "parser.c"
    break;

  case 1364: /* target_x_list: target_x_list target_x  */
#line 6936 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12802 "parser.c"
    break;

  case 1366: /* target_x: ADDRESS _of identifier_1  */
#line 6941 "parser.y"
                                { yyval = cb_build_address (yyvsp[0]); }
#line 12808 "parser.c"
    break;

  case 1367: /* x_list: x  */
#line 6945 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12814 "parser.c"
    break;

  case 1368: /* x_list: x_list x  */
#line 6946 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12820 "parser.c"
    break;

  case 1370: /* x: LENGTH _of identifier_1  */
#line 6951 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12826 "parser.c"
    break;

  case 1371: /* x: LENGTH _of basic_literal  */
#line 6952 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12832 "parser.c"
    break;

  case 1372: /* x: LENGTH _of function  */
#line 6953 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12838 "parser.c"
    break;

  case 1373: /* x: ADDRESS _of prog_or_entry alnum_or_id  */
#line 6954 "parser.y"
                                                { yyval = cb_build_ppointer (yyvsp[0]); }
#line 12844 "parser.c"
    break;

  case 1374: /* x: ADDRESS _of identifier_1  */
#line 6955 "parser.y"
                                                { yyval = cb_build_address (yyvsp[0]); }
#line 12850 "parser.c"
    break;

  case 1379: /* arith_x: LENGTH _of identifier_1  */
#line 6963 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12856 "parser.c"
    break;

  case 1380: /* arith_x: LENGTH _of basic_literal  */
#line 6964 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12862 "parser.c"
    break;

  case 1381: /* arith_x: LENGTH _of function  */
#line 6965 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12868 "parser.c"
    break;

  case 1387: /* alnum_or_id: identifier_1  */
#line 6977 "parser.y"
                        { yyval = yyvsp[0]; }
#line 12874 "parser.c"
    break;

  case 1388: /* alnum_or_id: "Literal"  */
#line 6978 "parser.y"
                        { yyval = yyvsp[0]; }
#line 12880 "parser.c"
    break;

  case 1400: /* num_id_or_lit: ZERO  */
#line 7012 "parser.y"
                                { yyval = cb_zero; }
#line 12886 "parser.c"
    break;

  case 1401: /* identifier: identifier_1  */
#line 7020 "parser.y"
                                { yyval = cb_build_identifier (yyvsp[0]); }
#line 12892 "parser.c"
    break;

  case 1402: /* identifier_1: qualified_word  */
#line 7024 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12898 "parser.c"
    break;

  case 1403: /* identifier_1: qualified_word subref  */
#line 7025 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 12904 "parser.c"
    break;

  case 1404: /* identifier_1: qualified_word refmod  */
#line 7026 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 12910 "parser.c"
    break;

  case 1405: /* identifier_1: qualified_word subref refmod  */
#line 7027 "parser.y"
                                { yyval = yyvsp[-2]; }
#line 12916 "parser.c"
    break;

  case 1406: /* qualified_word: "Identifier"  */
#line 7031 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12922 "parser.c"
    break;

  case 1407: /* qualified_word: "Identifier" in_of qualified_word  */
#line 7032 "parser.y"
                                { yyval = yyvsp[-2]; CB_REFERENCE (yyvsp[-2])->chain = yyvsp[0]; }
#line 12928 "parser.c"
    break;

  case 1408: /* subref: '(' exp_list ')'  */
#line 7037 "parser.y"
  {
	if (cb_ref (yyvsp[-3]) != cb_error_node) {
		yyval = yyvsp[-3];
		CB_REFERENCE (yyvsp[-3])->subs = cb_list_reverse (yyvsp[-1]);
	}
  }
#line 12939 "parser.c"
    break;

  case 1409: /* refmod: '(' exp ':' ')'  */
#line 7047 "parser.y"
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
#line 12957 "parser.c"
    break;

  case 1410: /* refmod: '(' exp ':' exp ')'  */
#line 7061 "parser.y"
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
#line 12977 "parser.c"
    break;

  case 1411: /* integer: "Literal"  */
#line 7084 "parser.y"
  {
	if (cb_tree_category (yyvsp[0]) != CB_CATEGORY_NUMERIC) {
		cb_error (_("Integer value expected"));
	} else if (CB_LITERAL (yyvsp[0])->sign < 0 || CB_LITERAL (yyvsp[0])->scale) {
		cb_error (_("Integer value expected"));
	}
	yyval = yyvsp[0];
  }
#line 12990 "parser.c"
    break;

  case 1412: /* literal: basic_literal  */
#line 7095 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12996 "parser.c"
    break;

  case 1413: /* literal: ALL basic_value  */
#line 7097 "parser.y"
  {
	yyval = yyvsp[0];
	if (CB_LITERAL_P (yyvsp[0])) {
		CB_LITERAL (yyvsp[0])->all = 1;
	}
  }
#line 13007 "parser.c"
    break;

  case 1414: /* basic_literal: basic_value  */
#line 7106 "parser.y"
                                { yyval = yyvsp[0]; }
#line 13013 "parser.c"
    break;

  case 1415: /* basic_literal: basic_literal '&' basic_value  */
#line 7107 "parser.y"
                                { yyval = cb_concat_literals (yyvsp[-2], yyvsp[0]); }
#line 13019 "parser.c"
    break;

  case 1416: /* basic_value: "Literal"  */
#line 7111 "parser.y"
                                { yyval = yyvsp[0]; }
#line 13025 "parser.c"
    break;

  case 1417: /* basic_value: SPACE  */
#line 7112 "parser.y"
                                { yyval = cb_space; }
#line 13031 "parser.c"
    break;

  case 1418: /* basic_value: ZERO  */
#line 7113 "parser.y"
                                { yyval = cb_zero; }
#line 13037 "parser.c"
    break;

  case 1419: /* basic_value: QUOTE  */
#line 7114 "parser.y"
                                { yyval = cb_quote; }
#line 13043 "parser.c"
    break;

  case 1420: /* basic_value: "HIGH-VALUE"  */
#line 7115 "parser.y"
                                { yyval = cb_high; }
#line 13049 "parser.c"
    break;

  case 1421: /* basic_value: "LOW-VALUE"  */
#line 7116 "parser.y"
                                { yyval = cb_low; }
#line 13055 "parser.c"
    break;

  case 1422: /* basic_value: "NULL"  */
#line 7117 "parser.y"
                                { yyval = cb_null; }
#line 13061 "parser.c"
    break;

  case 1423: /* function: "FUNCTION CURRENT-DATE" func_refmod  */
#line 7126 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-1], NULL, yyvsp[0]);
  }
#line 13069 "parser.c"
    break;

  case 1424: /* function: "FUNCTION WHEN-COMPILED" func_refmod  */
#line 7130 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-1], NULL, yyvsp[0]);
  }
#line 13077 "parser.c"
    break;

  case 1425: /* function: "FUNCTION UPPER-CASE" '(' exp ')' func_refmod  */
#line 7134 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], cb_list_init (yyvsp[-2]), yyvsp[0]);
  }
#line 13085 "parser.c"
    break;

  case 1426: /* function: "FUNCTION LOWER-CASE" '(' exp ')' func_refmod  */
#line 7138 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], cb_list_init (yyvsp[-2]), yyvsp[0]);
  }
#line 13093 "parser.c"
    break;

  case 1427: /* function: "FUNCTION REVERSE" '(' exp ')' func_refmod  */
#line 7142 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], cb_list_init (yyvsp[-2]), yyvsp[0]);
  }
#line 13101 "parser.c"
    break;

  case 1428: /* function: "FUNCTION CONCATENATE" '(' exp_list ')' func_refmod  */
#line 7146 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13109 "parser.c"
    break;

  case 1429: /* function: "FUNCTION SUBSTITUTE" '(' exp_list ')' func_refmod  */
#line 7150 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13117 "parser.c"
    break;

  case 1430: /* function: "FUNCTION SUBSTITUTE-CASE" '(' exp_list ')' func_refmod  */
#line 7154 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13125 "parser.c"
    break;

  case 1431: /* function: "FUNCTION TRIM" '(' trim_args ')' func_refmod  */
#line 7158 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13133 "parser.c"
    break;

  case 1432: /* function: "FUNCTION NUMVALC" '(' numvalc_args ')'  */
#line 7162 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-3], yyvsp[-1], NULL);
  }
#line 13141 "parser.c"
    break;

  case 1433: /* function: "FUNCTION LOCALE" '(' locale_dt_args ')' func_refmod  */
#line 7166 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13149 "parser.c"
    break;

  case 1434: /* function: "FUNCTION" func_args  */
#line 7170 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-1], yyvsp[0], NULL);
  }
#line 13157 "parser.c"
    break;

  case 1435: /* func_refmod: %empty  */
#line 7176 "parser.y"
                                { yyval = NULL; }
#line 13163 "parser.c"
    break;

  case 1436: /* func_refmod: '(' exp ':' ')'  */
#line 7177 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-2], NULL); }
#line 13169 "parser.c"
    break;

  case 1437: /* func_refmod: '(' exp ':' exp ')'  */
#line 7178 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-3], yyvsp[-1]); }
#line 13175 "parser.c"
    break;

  case 1438: /* func_args: %empty  */
#line 7182 "parser.y"
                                { yyval = NULL; }
#line 13181 "parser.c"
    break;

  case 1439: /* func_args: '(' list_func_args ')'  */
#line 7183 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 13187 "parser.c"
    break;

  case 1440: /* list_func_args: %empty  */
#line 7187 "parser.y"
                                { yyval = NULL; }
#line 13193 "parser.c"
    break;

  case 1441: /* list_func_args: exp_list  */
#line 7188 "parser.y"
                                { yyval = yyvsp[0]; }
#line 13199 "parser.c"
    break;

  case 1442: /* trim_args: exp  */
#line 7194 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[0]);
	yyval = cb_list_add (x, cb_int0);
  }
#line 13210 "parser.c"
    break;

  case 1443: /* trim_args: exp e_sep LEADING  */
#line 7201 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, cb_int1);
  }
#line 13221 "parser.c"
    break;

  case 1444: /* trim_args: exp e_sep TRAILING  */
#line 7208 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, cb_int2);
  }
#line 13232 "parser.c"
    break;

  case 1445: /* numvalc_args: exp  */
#line 7218 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[0]);
	yyval = cb_list_add (x, cb_null);
  }
#line 13243 "parser.c"
    break;

  case 1446: /* numvalc_args: exp e_sep exp  */
#line 7225 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, yyvsp[0]);
  }
#line 13254 "parser.c"
    break;

  case 1447: /* locale_dt_args: exp  */
#line 7235 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[0]);
	yyval = cb_list_add (x, cb_null);
  }
#line 13265 "parser.c"
    break;

  case 1448: /* locale_dt_args: exp e_sep reference  */
#line 7242 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, cb_ref (yyvsp[0]));
  }
#line 13276 "parser.c"
    break;

  case 1449: /* not_const_word: %empty  */
#line 7255 "parser.y"
  {
	non_const_word = 1;
  }
#line 13284 "parser.c"
    break;

  case 1450: /* flag_all: %empty  */
#line 7265 "parser.y"
                                { yyval = cb_int0; }
#line 13290 "parser.c"
    break;

  case 1451: /* flag_all: ALL  */
#line 7266 "parser.y"
                                { yyval = cb_int1; }
#line 13296 "parser.c"
    break;

  case 1452: /* flag_duplicates: %empty  */
#line 7270 "parser.y"
                                { yyval = cb_int0; }
#line 13302 "parser.c"
    break;

  case 1453: /* flag_duplicates: with_dups  */
#line 7271 "parser.y"
                                { yyval = cb_int1; }
#line 13308 "parser.c"
    break;

  case 1454: /* flag_initialized: %empty  */
#line 7275 "parser.y"
                                { yyval = NULL; }
#line 13314 "parser.c"
    break;

  case 1455: /* flag_initialized: INITIALIZED  */
#line 7276 "parser.y"
                                { yyval = cb_int1; }
#line 13320 "parser.c"
    break;

  case 1456: /* flag_next: %empty  */
#line 7280 "parser.y"
                                { yyval = cb_int0; }
#line 13326 "parser.c"
    break;

  case 1457: /* flag_next: NEXT  */
#line 7281 "parser.y"
                                { yyval = cb_int1; }
#line 13332 "parser.c"
    break;

  case 1458: /* flag_next: PREVIOUS  */
#line 7282 "parser.y"
                                { yyval = cb_int2; }
#line 13338 "parser.c"
    break;

  case 1459: /* flag_not: %empty  */
#line 7286 "parser.y"
                                { yyval = cb_int0; }
#line 13344 "parser.c"
    break;

  case 1460: /* flag_not: NOT  */
#line 7287 "parser.y"
                                { yyval = cb_int1; }
#line 13350 "parser.c"
    break;

  case 1461: /* flag_optional: %empty  */
#line 7291 "parser.y"
                                { yyval = cb_int0; }
#line 13356 "parser.c"
    break;

  case 1462: /* flag_optional: OPTIONAL  */
#line 7292 "parser.y"
                                { yyval = cb_int1; }
#line 13362 "parser.c"
    break;

  case 1463: /* flag_rounded: %empty  */
#line 7296 "parser.y"
                                { yyval = cb_int0; }
#line 13368 "parser.c"
    break;

  case 1464: /* flag_rounded: ROUNDED  */
#line 7297 "parser.y"
                                { yyval = cb_int1; }
#line 13374 "parser.c"
    break;

  case 1465: /* flag_separate: %empty  */
#line 7301 "parser.y"
                                { yyval = cb_int0; }
#line 13380 "parser.c"
    break;

  case 1466: /* flag_separate: SEPARATE _character  */
#line 7302 "parser.y"
                                { yyval = cb_int1; }
#line 13386 "parser.c"
    break;

  case 1478: /* _also: ALSO  */
#line 7315 "parser.y"
                       { yyval = cb_int1; }
#line 13392 "parser.c"
    break;

  case 1507: /* _is: %empty  */
#line 7330 "parser.y"
                { yyval = NULL; }
#line 13398 "parser.c"
    break;

  case 1508: /* _is: IS  */
#line 7330 "parser.y"
                                    { yyval = cb_int1; }
#line 13404 "parser.c"
    break;

  case 1519: /* _literal: %empty  */
#line 7335 "parser.y"
                { yyval = NULL; }
#line 13410 "parser.c"
    break;

  case 1520: /* _literal: "Literal"  */
#line 7335 "parser.y"
                                         { yyval = yyvsp[0]; }
#line 13416 "parser.c"
    break;

  case 1565: /* exec_sql_statement: "EXEC SQL statement"  */
#line 7365 "parser.y"
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
#line 13448 "parser.c"
    break;

  case 1566: /* exec_java_statement: "EXEC JAVA statement"  */
#line 7400 "parser.y"
  {
	cb_tree java_node;
	BEGIN_STATEMENT ("EXEC JAVA", 0);
	/* BEGIN_STATEMENT は cb_source_line (= END-EXEC 行) を入れる。
	   $1 のリテラルには scanner が EXEC JAVA 開始行を入れているので、
	   そちらで上書きして「コメントは EXEC JAVA の行を指す」ようにする。 */
	if (yyvsp[0]->source_line) {
		CB_TREE (current_statement)->source_line = yyvsp[0]->source_line;
	}
	java_node = cb_build_exec_java (yyvsp[0], 1);
	if (java_node != cb_error_node) {
		current_statement->body =
			cb_list_add (current_statement->body, java_node);
	}
  }
#line 13468 "parser.c"
    break;

  case 1567: /* exec_java_import_statement: "EXEC JAVA IMPORT statement"  */
#line 7425 "parser.y"
  {
	BEGIN_STATEMENT ("EXEC JAVA IMPORT", 0);
	if (yyvsp[0]->source_line) {
		CB_TREE (current_statement)->source_line = yyvsp[0]->source_line;
	}
	cb_add_exec_java_import (yyvsp[0]);
  }
#line 13480 "parser.c"
    break;

  case 1568: /* exec_java_member_statement: "EXEC JAVA CLASS-MEMBER statement"  */
#line 7442 "parser.y"
  {
	BEGIN_STATEMENT ("EXEC JAVA CLASS-MEMBER", 0);
	if (yyvsp[0]->source_line) {
		CB_TREE (current_statement)->source_line = yyvsp[0]->source_line;
	}
	cb_add_exec_java_member (yyvsp[0]);
  }
#line 13492 "parser.c"
    break;


#line 13496 "parser.c"

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

#line 7452 "parser.y"


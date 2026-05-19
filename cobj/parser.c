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
static int			esql_program_seen = 0;
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
  YYSYMBOL_varying_clause = 681,           /* varying_clause  */
  YYSYMBOL_local_storage_section = 682,    /* local_storage_section  */
  YYSYMBOL_683_25 = 683,                   /* $@25  */
  YYSYMBOL_linkage_section = 684,          /* linkage_section  */
  YYSYMBOL_685_26 = 685,                   /* $@26  */
  YYSYMBOL_report_section = 686,           /* report_section  */
  YYSYMBOL_687_27 = 687,                   /* $@27  */
  YYSYMBOL_opt_report_description_list = 688, /* opt_report_description_list  */
  YYSYMBOL_report_description_list = 689,  /* report_description_list  */
  YYSYMBOL_report_description_entry = 690, /* report_description_entry  */
  YYSYMBOL_report_description_options = 691, /* report_description_options  */
  YYSYMBOL_report_description_option = 692, /* report_description_option  */
  YYSYMBOL_control_clause = 693,           /* control_clause  */
  YYSYMBOL_control_field_list = 694,       /* control_field_list  */
  YYSYMBOL__final = 695,                   /* _final  */
  YYSYMBOL_identifier_list = 696,          /* identifier_list  */
  YYSYMBOL_page_limit_clause = 697,        /* page_limit_clause  */
  YYSYMBOL_heading_clause = 698,           /* heading_clause  */
  YYSYMBOL_first_detail = 699,             /* first_detail  */
  YYSYMBOL_last_heading = 700,             /* last_heading  */
  YYSYMBOL_last_detail = 701,              /* last_detail  */
  YYSYMBOL_footing_clause = 702,           /* footing_clause  */
  YYSYMBOL_page_line_column = 703,         /* page_line_column  */
  YYSYMBOL_line_or_lines = 704,            /* line_or_lines  */
  YYSYMBOL_report_group_description_list = 705, /* report_group_description_list  */
  YYSYMBOL_report_group_description_entry = 706, /* report_group_description_entry  */
  YYSYMBOL_report_group_options = 707,     /* report_group_options  */
  YYSYMBOL_report_group_option = 708,      /* report_group_option  */
  YYSYMBOL_type_clause = 709,              /* type_clause  */
  YYSYMBOL_type_option = 710,              /* type_option  */
  YYSYMBOL_next_group_clause = 711,        /* next_group_clause  */
  YYSYMBOL_column_clause = 712,            /* column_clause  */
  YYSYMBOL_sum_clause_list = 713,          /* sum_clause_list  */
  YYSYMBOL_sum_clause = 714,               /* sum_clause  */
  YYSYMBOL_ref_id_exp = 715,               /* ref_id_exp  */
  YYSYMBOL_present_when_condition = 716,   /* present_when_condition  */
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
#define YYLAST   5381

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  464
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  704
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1554
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  2291

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
       0,   775,   775,   775,   819,   820,   824,   825,   830,   831,
     829,   844,   845,   843,   858,   859,   857,   864,   865,   866,
     869,   870,   898,   924,   956,   955,   999,  1043,  1044,  1048,
    1049,  1052,  1053,  1057,  1064,  1071,  1075,  1079,  1091,  1092,
    1102,  1103,  1112,  1113,  1117,  1118,  1119,  1120,  1129,  1132,
    1133,  1134,  1135,  1139,  1146,  1155,  1158,  1159,  1160,  1161,
    1165,  1166,  1170,  1171,  1172,  1176,  1183,  1184,  1188,  1195,
    1207,  1210,  1211,  1215,  1216,  1220,  1224,  1231,  1232,  1242,
    1245,  1246,  1247,  1251,  1252,  1256,  1257,  1258,  1259,  1260,
    1261,  1262,  1263,  1264,  1265,  1266,  1273,  1284,  1283,  1295,
    1294,  1303,  1317,  1331,  1345,  1361,  1362,  1366,  1367,  1371,
    1382,  1383,  1391,  1390,  1402,  1403,  1404,  1405,  1406,  1414,
    1415,  1420,  1421,  1423,  1422,  1434,  1435,  1439,  1440,  1441,
    1442,  1443,  1444,  1448,  1449,  1450,  1451,  1452,  1453,  1460,
    1471,  1483,  1484,  1488,  1489,  1496,  1505,  1506,  1510,  1511,
    1525,  1540,  1607,  1618,  1625,  1632,  1638,  1645,  1646,  1650,
    1649,  1659,  1658,  1674,  1675,  1678,  1679,  1684,  1683,  1704,
    1705,  1709,  1710,  1711,  1712,  1713,  1714,  1715,  1716,  1717,
    1718,  1719,  1720,  1721,  1722,  1723,  1730,  1734,  1739,  1746,
    1747,  1748,  1751,  1752,  1756,  1763,  1764,  1771,  1791,  1792,
    1798,  1802,  1803,  1804,  1811,  1831,  1874,  1874,  1878,  1882,
    1898,  1899,  1900,  1906,  1916,  1925,  1927,  1928,  1934,  1938,
    1939,  1940,  1943,  1944,  1945,  1949,  1953,  1954,  1960,  1961,
    1965,  1974,  1983,  1992,  2001,  2016,  2026,  2033,  2042,  2080,
    2087,  2088,  2095,  2099,  2100,  2101,  2107,  2114,  2115,  2118,
    2119,  2120,  2121,  2125,  2126,  2130,  2131,  2137,  2162,  2163,
    2164,  2165,  2171,  2178,  2179,  2183,  2186,  2187,  2193,  2194,
    2195,  2199,  2203,  2207,  2211,  2215,  2225,  2226,  2240,  2241,
    2241,  2244,  2243,  2256,  2257,  2261,  2273,  2282,  2286,  2287,
    2297,  2296,  2314,  2315,  2319,  2326,  2333,  2334,  2335,  2336,
    2337,  2338,  2339,  2340,  2341,  2342,  2349,  2353,  2353,  2353,
    2359,  2371,  2396,  2420,  2421,  2428,  2429,  2433,  2434,  2441,
    2448,  2449,  2456,  2460,  2469,  2470,  2476,  2486,  2504,  2505,
    2509,  2510,  2511,  2515,  2522,  2529,  2539,  2546,  2564,  2568,
    2579,  2580,  2580,  2591,  2592,  2596,  2596,  2613,  2614,  2616,
    2617,  2618,  2622,  2629,  2637,  2639,  2638,  2664,  2663,  2689,
    2693,  2700,  2702,  2724,  2729,  2735,  2744,  2752,  2753,  2761,
    2762,  2763,  2767,  2787,  2791,  2800,  2801,  2802,  2803,  2804,
    2805,  2806,  2807,  2808,  2809,  2810,  2811,  2812,  2813,  2814,
    2815,  2822,  2844,  2866,  2867,  2879,  2899,  2906,  2907,  2911,
    2912,  2913,  2918,  2923,  2924,  2925,  2926,  2927,  2928,  2929,
    2930,  2935,  2940,  2941,  2942,  2943,  2944,  2945,  2946,  2947,
    2948,  2949,  2950,  2951,  2952,  2953,  2954,  2955,  2956,  2957,
    2958,  2966,  2974,  2982,  2989,  2994,  3004,  3005,  3006,  3010,
    3027,  3028,  3031,  3032,  3038,  3038,  3041,  3065,  3081,  3082,
    3086,  3087,  3090,  3090,  3093,  3100,  3101,  3106,  3116,  3123,
    3126,  3127,  3128,  3135,  3142,  3167,  3171,  3171,  3176,  3177,
    3181,  3182,  3185,  3186,  3199,  3211,  3231,  3244,  3254,  3256,
    3255,  3276,  3277,  3277,  3290,  3292,  3291,  3303,  3304,  3308,
    3309,  3318,  3325,  3328,  3332,  3336,  3337,  3338,  3345,  3346,
    3350,  3353,  3353,  3356,  3357,  3363,  3368,  3369,  3372,  3373,
    3376,  3377,  3380,  3381,  3384,  3385,  3389,  3390,  3391,  3395,
    3396,  3399,  3400,  3404,  3408,  3409,  3413,  3414,  3415,  3416,
    3417,  3418,  3419,  3420,  3421,  3422,  3423,  3424,  3425,  3426,
    3427,  3428,  3432,  3436,  3437,  3438,  3439,  3440,  3441,  3442,
    3446,  3450,  3451,  3452,  3456,  3457,  3461,  3465,  3470,  3474,
    3478,  3482,  3483,  3487,  3488,  3492,  3493,  3494,  3497,  3497,
    3497,  3500,  3504,  3507,  3507,  3510,  3517,  3518,  3518,  3528,
    3530,  3543,  3529,  3570,  3572,  3571,  3578,  3577,  3586,  3587,
    3592,  3599,  3601,  3605,  3615,  3617,  3625,  3633,  3662,  3693,
    3695,  3705,  3710,  3721,  3722,  3722,  3749,  3750,  3754,  3755,
    3756,  3757,  3773,  3785,  3816,  3853,  3865,  3868,  3869,  3878,
    3882,  3878,  3895,  3916,  3920,  3921,  3922,  3923,  3924,  3925,
    3926,  3927,  3928,  3929,  3930,  3931,  3932,  3933,  3934,  3935,
    3936,  3937,  3938,  3939,  3940,  3941,  3942,  3943,  3944,  3945,
    3946,  3947,  3948,  3949,  3950,  3951,  3952,  3953,  3954,  3955,
    3956,  3957,  3958,  3959,  3960,  3961,  3962,  3963,  3964,  3965,
    3966,  3967,  3968,  3969,  3970,  3993,  3992,  4005,  4009,  4013,
    4017,  4021,  4025,  4029,  4033,  4037,  4041,  4045,  4049,  4053,
    4057,  4061,  4065,  4069,  4076,  4077,  4078,  4079,  4080,  4081,
    4085,  4089,  4090,  4093,  4094,  4098,  4099,  4103,  4104,  4105,
    4106,  4107,  4108,  4109,  4110,  4114,  4118,  4122,  4127,  4128,
    4129,  4130,  4131,  4132,  4136,  4137,  4146,  4146,  4152,  4156,
    4160,  4166,  4167,  4171,  4172,  4181,  4181,  4186,  4190,  4197,
    4198,  4207,  4213,  4214,  4218,  4218,  4226,  4226,  4236,  4238,
    4237,  4246,  4247,  4252,  4259,  4266,  4268,  4272,  4280,  4291,
    4292,  4293,  4298,  4302,  4301,  4313,  4317,  4316,  4327,  4328,
    4337,  4337,  4341,  4342,  4346,  4358,  4358,  4362,  4363,  4374,
    4375,  4376,  4377,  4378,  4381,  4381,  4389,  4389,  4395,  4402,
    4403,  4406,  4406,  4413,  4426,  4439,  4439,  4450,  4451,  4460,
    4460,  4480,  4479,  4492,  4496,  4500,  4504,  4508,  4512,  4516,
    4521,  4525,  4532,  4533,  4534,  4538,  4539,  4544,  4545,  4546,
    4547,  4548,  4549,  4550,  4551,  4552,  4553,  4557,  4561,  4565,
    4570,  4571,  4575,  4576,  4585,  4585,  4591,  4595,  4599,  4603,
    4607,  4614,  4615,  4624,  4624,  4646,  4645,  4664,  4665,  4676,
    4685,  4690,  4698,  4727,  4728,  4734,  4733,  4749,  4753,  4752,
    4767,  4768,  4773,  4774,  4785,  4814,  4815,  4816,  4819,  4820,
    4824,  4825,  4834,  4834,  4839,  4840,  4848,  4856,  4864,  4882,
    4907,  4907,  4920,  4920,  4933,  4933,  4942,  4946,  4959,  4959,
    4972,  4974,  4972,  4985,  4990,  4994,  4993,  5007,  5008,  5017,
    5017,  5025,  5026,  5030,  5031,  5032,  5036,  5037,  5042,  5043,
    5048,  5052,  5053,  5054,  5055,  5056,  5057,  5058,  5062,  5063,
    5072,  5072,  5085,  5084,  5094,  5095,  5096,  5100,  5101,  5105,
    5106,  5107,  5113,  5113,  5118,  5119,  5123,  5124,  5125,  5126,
    5127,  5128,  5134,  5138,  5139,  5143,  5148,  5152,  5153,  5154,
    5155,  5156,  5160,  5186,  5199,  5200,  5204,  5204,  5212,  5212,
    5222,  5222,  5227,  5231,  5243,  5243,  5249,  5253,  5260,  5261,
    5270,  5270,  5274,  5275,  5289,  5290,  5291,  5292,  5296,  5297,
    5301,  5302,  5303,  5315,  5315,  5320,  5325,  5324,  5334,  5341,
    5342,  5346,  5351,  5360,  5363,  5367,  5372,  5379,  5386,  5387,
    5391,  5392,  5397,  5409,  5409,  5436,  5437,  5441,  5442,  5446,
    5450,  5454,  5458,  5465,  5466,  5472,  5473,  5474,  5478,  5479,
    5488,  5488,  5503,  5503,  5514,  5515,  5524,  5524,  5541,  5542,
    5546,  5553,  5554,  5563,  5576,  5576,  5582,  5587,  5586,  5597,
    5598,  5602,  5604,  5603,  5614,  5615,  5620,  5619,  5630,  5631,
    5640,  5640,  5645,  5646,  5647,  5648,  5649,  5655,  5664,  5668,
    5677,  5684,  5685,  5691,  5692,  5696,  5705,  5706,  5710,  5714,
    5726,  5726,  5732,  5731,  5748,  5751,  5772,  5773,  5776,  5777,
    5781,  5782,  5787,  5792,  5800,  5812,  5817,  5825,  5841,  5842,
    5841,  5862,  5863,  5871,  5872,  5873,  5874,  5875,  5879,  5880,
    5889,  5889,  5894,  5894,  5901,  5902,  5903,  5912,  5912,  5921,
    5922,  5926,  5927,  5928,  5932,  5933,  5937,  5938,  5947,  5947,
    5953,  5957,  5961,  5968,  5969,  5978,  5985,  5986,  5994,  5994,
    6007,  6007,  6023,  6023,  6032,  6034,  6035,  6044,  6044,  6054,
    6055,  6060,  6061,  6066,  6073,  6074,  6079,  6086,  6087,  6091,
    6092,  6096,  6097,  6101,  6102,  6111,  6112,  6113,  6117,  6141,
    6144,  6152,  6162,  6167,  6172,  6177,  6184,  6185,  6188,  6189,
    6193,  6193,  6197,  6197,  6201,  6201,  6204,  6205,  6209,  6216,
    6217,  6221,  6233,  6233,  6250,  6251,  6256,  6259,  6263,  6267,
    6274,  6275,  6278,  6279,  6280,  6284,  6285,  6298,  6306,  6313,
    6315,  6314,  6324,  6326,  6325,  6340,  6344,  6346,  6345,  6356,
    6358,  6357,  6374,  6380,  6382,  6381,  6391,  6393,  6392,  6408,
    6413,  6418,  6428,  6427,  6439,  6438,  6454,  6459,  6464,  6474,
    6473,  6485,  6484,  6499,  6500,  6504,  6509,  6514,  6524,  6523,
    6535,  6534,  6551,  6554,  6566,  6573,  6580,  6580,  6590,  6591,
    6593,  6594,  6595,  6596,  6597,  6598,  6600,  6601,  6602,  6603,
    6604,  6605,  6607,  6608,  6610,  6611,  6612,  6615,  6617,  6618,
    6619,  6621,  6622,  6623,  6625,  6626,  6628,  6629,  6630,  6631,
    6632,  6634,  6635,  6636,  6637,  6638,  6639,  6641,  6642,  6643,
    6644,  6645,  6646,  6648,  6649,  6652,  6652,  6652,  6653,  6653,
    6654,  6654,  6655,  6655,  6655,  6656,  6656,  6656,  6661,  6662,
    6665,  6666,  6667,  6671,  6672,  6673,  6674,  6675,  6676,  6677,
    6678,  6679,  6690,  6702,  6717,  6718,  6723,  6729,  6751,  6771,
    6775,  6791,  6805,  6806,  6811,  6817,  6818,  6823,  6832,  6833,
    6834,  6838,  6849,  6850,  6854,  6864,  6865,  6869,  6870,  6874,
    6875,  6881,  6901,  6902,  6906,  6907,  6911,  6912,  6916,  6917,
    6918,  6919,  6920,  6921,  6922,  6923,  6924,  6928,  6929,  6930,
    6931,  6932,  6933,  6934,  6938,  6939,  6943,  6944,  6948,  6949,
    6953,  6954,  6965,  6966,  6970,  6971,  6972,  6976,  6977,  6978,
    6986,  6990,  6991,  6992,  6993,  6997,  6998,  7002,  7012,  7026,
    7049,  7061,  7062,  7072,  7073,  7077,  7078,  7079,  7080,  7081,
    7082,  7083,  7091,  7095,  7099,  7103,  7107,  7111,  7115,  7119,
    7123,  7127,  7131,  7135,  7142,  7143,  7144,  7148,  7149,  7153,
    7154,  7159,  7166,  7173,  7183,  7190,  7200,  7207,  7221,  7231,
    7232,  7236,  7237,  7241,  7242,  7246,  7247,  7248,  7252,  7253,
    7257,  7258,  7262,  7263,  7267,  7268,  7275,  7275,  7276,  7276,
    7277,  7277,  7278,  7278,  7280,  7280,  7281,  7281,  7282,  7282,
    7283,  7283,  7284,  7284,  7285,  7285,  7286,  7286,  7287,  7287,
    7288,  7288,  7289,  7289,  7290,  7290,  7291,  7291,  7292,  7292,
    7293,  7293,  7294,  7294,  7295,  7295,  7296,  7296,  7297,  7297,
    7297,  7298,  7298,  7299,  7299,  7299,  7300,  7300,  7301,  7301,
    7302,  7302,  7303,  7303,  7304,  7304,  7305,  7305,  7306,  7306,
    7306,  7307,  7307,  7308,  7308,  7309,  7309,  7310,  7310,  7311,
    7311,  7312,  7312,  7313,  7313,  7313,  7314,  7314,  7315,  7315,
    7316,  7316,  7317,  7317,  7318,  7318,  7319,  7319,  7320,  7320,
    7322,  7322,  7323,  7323,  7330
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
  "exec_sql_statement", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1729)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1553)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1729,   282,   515, -1729,   -96,   178,   191, -1729, -1729, -1729,
     555,   555,   -23,   -23, -1729, -1729,   592, -1729, -1729, -1729,
   -1729,   753,   753,   395,   726,   726,   655,   467, -1729,   872,
     848, -1729, -1729, -1729, -1729,   -49,   638,   820,   531,   696,
     696, -1729,   560,    68,   596,   614,   732,   698, -1729,   235,
     991,   831,  1022, -1729,   -87, -1729, -1729,   853, -1729, -1729,
   -1729,   749, -1729, -1729, -1729,   852,   785, -1729,    19, -1729,
     448,   555,   -23, -1729, -1729, -1729, -1729,   782, -1729,  1048,
      83,   770,   897,  1020,   826, -1729, -1729,   934,   -23, -1729,
   -1729, -1729,   824,   832,   855,   859,   880, -1729, -1729, -1729,
   -1729, -1729,   974,   882,  1109,  1076,   938,   689, -1729,   298,
   -1729, -1729, -1729,    33, -1729, -1729,   900,   989,  1113, -1729,
     425,    80, -1729,    91,    91,   919,   909,   911,   726, -1729,
     292,  1193,    89,  1472,  1101, -1729, -1729,   933, -1729,  1110,
    1111,   994,  1123,  1000, -1729,  1011, -1729, -1729, -1729,  1383,
   -1729, -1729, -1729, -1729, -1729, -1729,   953,  1060,  1092, -1729,
     826, -1729, -1729,   976, -1729, -1729,    76, -1729,  -123,   -82,
      85, -1729, -1729, -1729, -1729,  1056,  1218, -1729,   590, -1729,
     728, -1729, -1729, -1729, -1729,   118,   285, -1729,   -51, -1729,
   -1729, -1729,   977,  -178,  1328,   995,  1218,  1218,   995,  1052,
    1072,  1218,  1218,  1218,  1218,  1218,   995,  1218,  1390,  1218,
   -1729,   915, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729,   995,   974,    83,  1003, -1729,
    1003,  1003, -1729,  1212,  1003, -1729,  1365, -1729,  1277,   425,
      80, -1729,   999,  1104,  1106,    80,  1009,   433, -1729,  1008,
     830, -1729,  1218,  1091,  1183, -1729, -1729,  1359,   696,  1218,
    1234, -1729,   765, -1729, -1729,  1114, -1729,  1218,  1261, -1729,
     851, -1729, -1729, -1729, -1729,  1017,  1226, -1729, -1729,   995,
     995,  1218,  1218, -1729,  1218,  1003,  1408,   995,   995,  1003,
    1218,  1003, -1729,   995,    -5, -1729, -1729, -1729, -1729,   403,
    1003, -1729, -1729,  1003,  1206,  1074,  1215, -1729,   826, -1729,
     826, -1729, -1729,    80, -1729,  1047,  1148, -1729, -1729, -1729,
   -1729, -1729,  1008, -1729, -1729, -1729,   -43,   -28, -1729, -1729,
    1365,  1218,   -50,   -50,  1218,    28,  1256,  1218,  1483,  1232,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729,   866,   631,  1218, -1729,  1069,  1057, -1729,   831,  1234,
   -1729, -1729, -1729, -1729,  1003, -1729, -1729, -1729, -1729, -1729,
    1218, -1729, -1729,   740,  1003,  1276, -1729, -1729, -1729, -1729,
   -1729,  1003, -1729, -1729,    67, -1729, -1729,   965, -1729, -1729,
   -1729, -1729,  1003, -1729,  1003,  1236,  1003,   826, -1729,  1214,
     826, -1729, -1729,    80, -1729,  1064, -1729, -1729,  1438, -1729,
    1447, -1729, -1729,  1234,  1094,  1218,  1483,  1003,   586,   -91,
    1234,  1098, -1729,  1218,  1095, -1729,  1095,   -44, -1729, -1729,
   -1729, -1729, -1729,  1234, -1729, -1729, -1729,   456,    61, -1729,
     623, -1729, -1729, -1729, -1729, -1729, -1729, -1729,   740, -1729,
    1138, -1729, -1729, -1729, -1729, -1729, -1729,  1234, -1729, -1729,
     965, -1729,  1156, -1729,   932, -1729,  1003,  1003,  1003, -1729,
    1234, -1729, -1729, -1729,  1222, -1729, -1729,   440,  1107,  1143,
   -1729, -1729, -1729,  1003, -1729, -1729, -1729, -1729, -1729, -1729,
    1307,     3,  1347,  1120, -1729, -1729, -1729,  1218,  1218, -1729,
   -1729,  2712,   -23, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,   509, -1729,
      54, -1729,   740,  1234, -1729, -1729, -1729,  1218,   965, -1729,
    1256,  1237,  1152, -1729,  1187,  1256,  1349,  1218,  1519,   268,
     419,   649, -1729,  1134, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729,  1195, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729,  1218,  1003, -1729,  1095, -1729,  1222,
   -1729, -1729,  2444,  1550,  1397,   153, -1729,  1234,    71, -1729,
   -1729, -1729,  1234, -1729, -1729,  1217, -1729,   -59,   -59,  2974,
   -1729,  1131, -1729, -1729, -1729, -1729,  1240,  3599,  1135, -1729,
   -1729,   509, -1729, -1729,   995, -1729,  1218,  1349, -1729,   892,
   -1729,  1218, -1729,  1218,   773, -1729,  1218, -1729,  1218,  1228,
    1218,  1218, -1729,  1383,   101,  1218,  1151, -1729, -1729, -1729,
    1372, -1729, -1729,  -112,   481,   651,   675,   727,  1161, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,  1244, -1729,
   -1729,  1234, -1729, -1729, -1729, -1729,  1003,  1003,  1386, -1729,
   -1729, -1729,    -4, -1729, -1729, -1729,  1218,  1218,  1003, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729,   963,   -35, -1729,
     643, -1729,   263, -1729, -1729, -1729, -1729,    38,  1390, -1729,
      75, -1729, -1729, -1729, -1729,  1493, -1729,  1367, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,  1199, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729,  1152, -1729,  2016, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729,   633, -1729, -1729,
    1299, -1729, -1729, -1729, -1729,   951, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729,   512, -1729, -1729,   299,  1218, -1729,
   -1729,    63,   564,  1003,  1569, -1729, -1729,   -91,  1227, -1729,
    1003,  1003, -1729,  1322,  1322,  1331, -1729,  1003, -1729,   378,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729,  1165, -1729, -1729,  1216, -1729,  1158,  1221, -1729,
   -1729, -1729, -1729,  3202,   263,  1440, -1729,  1591, -1729,  1260,
    1260,   740,  1123,  1123, -1729, -1729,  1166, -1729,   263, -1729,
    1231, -1729, -1729, -1729, -1729, -1729,    40,  1454, -1729, -1729,
    1143,  1234,  1179, -1729,  1180,  1003,  3820,  1196,   372,  1043,
    1630, -1729,  4415,   826,  1239,  4582,  4415,  1405,   790,   877,
      52,  1003, -1729, -1729,  1506, -1729,    52,  1003,  1389,  1003,
    4020,  4415, -1729,  3578,   826,  1003,   826,  1003,    56,    51,
    1003,   826, -1729, -1729,  4041,  4132, -1729, -1729,  1003,  1003,
     826,  1003, -1729,   245,  1538,  1003, -1729, -1729, -1729, -1729,
   -1729, -1729,  1636, -1729, -1729, -1729, -1729, -1729,  1003,    93,
     152,    97,  1207, -1729,  1207, -1729, -1729, -1729, -1729,   143,
   -1729, -1729, -1729, -1729, -1729,  1003,  1218,  1475,  1475,   153,
   -1729, -1729, -1729, -1729,  1468, -1729,  1234,  1255,  4922,  1201,
   -1729,  1003, -1729, -1729,  4415,  1422, -1729,  1519, -1729, -1729,
   -1729,  1003,  1003,   740,  1161, -1729,   263,   -91,   -91,  1635,
    1390, -1729, -1729, -1729,  1534,   107,  1123,  1209,  1003,  1210,
    1225,  1123,   403,  1229,  1243,  1246,  1247,  1249,  1252,  1254,
    1258,  1210,  1546, -1729,  4228, -1729, -1729, -1729, -1729,  1461,
   -1729,  1623, -1729, -1729, -1729,  1283, -1729,   403, -1729, -1729,
    1253, -1729, -1729, -1729,   411,   826,  1568,  1340, -1729,  1332,
    1395,   826,   923,  1583,  1925,   966,  1014,  1586,   136,  1253,
   -1729, -1729,    39, -1729, -1729, -1729,  1620, -1729, -1729, -1729,
    1123,    52, -1729, -1729, -1729, -1729, -1729,  1326, -1729,    55,
    1003, -1729,   194, -1729, -1729, -1729, -1729, -1729,  4415, -1729,
    1327,  1595,  1684,   747, -1729,  1333, -1729,  4905,  1596,  -157,
    1336,  1335,  -171,  1341,   579,  1559, -1729,  1395,  1559,  1003,
    1597,  1305, -1729,   589, -1729, -1729, -1729, -1729, -1729,  1494,
   -1729,    52, -1729,   -77, -1729,    58, -1729, -1729, -1729,   502,
    1695,  3100, -1729, -1729,  1003,  1600,  4312,  1003,  1570,   929,
    1637, -1729,  1415,  1374,  1111,  1559,   512,   143, -1729,  1310,
   -1729,  1003,   -32, -1729, -1729, -1729,  1218,  1634, -1729, -1729,
   -1729, -1729, -1729, -1729,   411, -1729, -1729,  1003, -1729,  1234,
    1530, -1729, -1729, -1729,  1644,  1123,  4922,  4922,  4922,   -10,
     850, -1729, -1729, -1729,  1166, -1729,  4922, -1729, -1729,  1710,
   -1729, -1729, -1729, -1729, -1729,  1396, -1729, -1729,  1307,   -91,
    1646, -1729, -1729,   589,  1309,  1321,   151,   200,  4922,  1360,
    4922, -1729,  4922, -1729,  2414,  1324,  4922,  4922,  4922,  4922,
    4922,  4922,  4922,  4922, -1729, -1729, -1729,  4415,  1577, -1729,
   -1729,  1427,  1461,  1687,  3284,  1460,  1536, -1729,   505, -1729,
   -1729, -1729,   836, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729,   444,   826, -1729, -1729,   -85,  1607,  1607,  1607,
    1607, -1729, -1729,  4415,  4415, -1729, -1729,    79,  1639,   845,
   -1729,  1337,   790, -1729,  1003, -1729,    92, -1729, -1729,   937,
    1603, -1729,   589,   198, -1729,   194, -1729, -1729, -1729, -1729,
     122,  1378,    52, -1729, -1729,  4415, -1729, -1729, -1729, -1729,
    1420, -1729, -1729, -1729, -1729,  1003,   372, -1729,  1005, -1729,
   -1729,  1395,   411, -1729,  1579,   359,   599, -1729, -1729,  1003,
     599,  1384, -1729,  1166, -1729, -1729,    73,   965, -1729, -1729,
    2200, -1729,  1741,  1575,  4415,  4415, -1729,  4340,  1003, -1729,
    1614, -1729, -1729,  4415,   589, -1729, -1729, -1729,  1695,  1585,
    1003, -1729,  1044,    74,   359, -1729, -1729, -1729,  1003, -1729,
    1518, -1729, -1729, -1729,   -48,  1003, -1729,  1003,  1608,  1096,
     -28, -1729,  1111,   230,  2414,  1345,  1345,   889, -1729, -1729,
   -1729,  4922,  4922,  4922,  4922,  4922,  4922,  4817,   850,  4415,
   -1729,  1152, -1729,  1307,  1111, -1729, -1729, -1729,  1607, -1729,
   -1729,  1356,  1358, -1729,   589,  1607,  1587, -1729, -1729, -1729,
   -1729,  1631,  1607,  1539,  1539,  1539,   184,  1573, -1729, -1729,
     413, -1729,    10,   811,  1003,   869,    15,  1350, -1729,  1166,
   -1729, -1729,   492,  1355,   968,   574,  1357,  1075,    17,    26,
     662,  1361,  1108,  4520,   464,  4415,    52, -1729,  1473, -1729,
   -1729, -1729, -1729, -1729,  1152, -1729, -1729,  1418, -1729, -1729,
    1418, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729,  1419,   372, -1729,    53,  1003,  1003,
      12, -1729, -1729, -1729,   521,   437,  1445, -1729, -1729,  1690,
   -1729,  1560, -1729,    20,  1211,  1607,  1558, -1729, -1729,  1564,
   -1729, -1729, -1729,  1643,  4520,   469, -1729, -1729, -1729,  3188,
   -1729,  1432, -1729, -1729, -1729, -1729, -1729,    79, -1729, -1729,
   -1729,  1111, -1729, -1729, -1729,  1152, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729,  1502,  1152, -1729,  1431, -1729,  1786, -1729,
   -1729, -1729,  1204, -1729,   589,  1051, -1729,   118,   620,   205,
      52,    52,  4520,   485,  1134,   826,  1698, -1729, -1729,  1830,
   -1729,  1659, -1729, -1729, -1729, -1729,  1579, -1729,  1003,   450,
     444,   941,  1403,  1716, -1729,  1407,   589,   807, -1729,   413,
   -1729, -1729, -1729,  4415,  1218,   444, -1729, -1729, -1729, -1729,
     301,  1003,  4520,   488,  1448,  1839,  1003,   388, -1729, -1729,
   -1729,  1543,  1544, -1729, -1729,  1005,   -48, -1729,   688, -1729,
   -1729, -1729, -1729,  1218,  1680, -1729, -1729,  1234, -1729,  1003,
   -1729, -1729,  1695, -1729,  1660, -1729,   205,  1349,   205, -1729,
    1166, -1729, -1729,   811,   837,   837,  1345,  1345,  1345, -1729,
    1220, -1729,  1457, -1729,  1003, -1729,  1564, -1729, -1729,  1607,
   -1729, -1729, -1729,  1218, -1729, -1729,  1218, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729,    -9, -1729, -1729, -1729,  1631, -1729,
   -1729, -1729,   411,   411,   411, -1729, -1729, -1729, -1729, -1729,
    1210,  1332,  4875, -1729,  1003,  1210,  1210,  4922, -1729,  1210,
    1210,  1210,   112,  1210,  1210, -1729, -1729,  1598,  4520, -1729,
      52, -1729, -1729,    47,   134, -1729, -1729,  3764, -1729,   616,
      60, -1729, -1729, -1729, -1729,  1032, -1729,  1540, -1729,  1521,
   -1729, -1729, -1729, -1729, -1729, -1729,  -103,  -103,  -103,  -103,
    1218, -1729, -1729, -1729, -1729,  1155,  1218, -1729, -1729, -1729,
   -1729,    27, -1729,  1211, -1729, -1729, -1729, -1729, -1729, -1729,
    4415, -1729,  4415,    79, -1729, -1729, -1729,  3188, -1729,  1003,
    1742,  1433,   913,  1760,  1436,   129,   589, -1729, -1729,  1826,
   -1729, -1729, -1729, -1729,  1051, -1729,  1704, -1729,  1218,  1599,
   -1729, -1729,  1349,    52, -1729,  4415,   101,   439, -1729, -1729,
   -1729,  1003,  4415,   604, -1729, -1729, -1729,  1739,  1618, -1729,
    1740, -1729,  1645, -1729, -1729, -1729, -1729,  1407, -1729, -1729,
   -1729,  1624,  1743,  1601,  1588,  1332, -1729,  4415,   129, -1729,
    1605, -1729,   589, -1729,  1778,  1498, -1729, -1729,  1111, -1729,
     914,  1886, -1729,  1007, -1729, -1729, -1729,  1234,  1779,  1674,
    1829,  4799, -1729,  1456, -1729, -1729, -1729,  1218, -1729, -1729,
   -1729,  1218, -1729, -1729, -1729, -1729,   188,   188,    21,    21,
   -1729, -1729, -1729, -1729, -1729,  1445, -1729,  1271, -1729, -1729,
   -1729,   811, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729,  1152,  1744,  1152,  1746, -1729, -1729,
    4415, -1729, -1729, -1729, -1729, -1729,  1770, -1729, -1729, -1729,
   -1729, -1729, -1729,  1607,  1607,  1607,  1607,   188, -1729, -1729,
     188,    21,    21, -1729, -1729, -1729,  4520,  1576,  4520,  1578,
   -1729, -1729, -1729, -1729, -1729,  1767, -1729,   913, -1729,  1803,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729,   129,  1005, -1729,
   -1729,  1005,   141,  1003, -1729,  1218,  4520, -1729, -1729,   960,
    3772, -1729,  1858,  1668,  1692,   -46, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,  1003,
    1323, -1729, -1729, -1729,  1766,  1648,  1003,  1445,  4520, -1729,
    1839, -1729,  1347,  1817,  1347,  1601,   451, -1729, -1729,  1768,
   -1729,  1653, -1729, -1729, -1729,   754, -1729, -1729,  1218,  1827,
    1699, -1729,  1089, -1729,  1719,  1093,  1483,  1731,  1486,  1218,
    1123,  1218, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729,  1535, -1729, -1729, -1729,   666,
   -1729, -1729, -1729,  1456, -1729,  1003,   263, -1729, -1729, -1729,
   -1729, -1729, -1729,   188, -1729, -1729, -1729, -1729, -1729, -1729,
    1152, -1729,  1152, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,  4415, -1729,
    4415, -1729, -1729, -1729, -1729, -1729,  1877,  1005,  1005, -1729,
    1522,  1622,   826,   686, -1729, -1729, -1729, -1729,  1589,  4415,
   -1729,  1218,   838,  1696, -1729,  1697, -1729, -1729, -1729, -1729,
   -1729, -1729,  1003, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729,  1003,  1347, -1729,
    1003,  1780, -1729, -1729, -1729, -1729, -1729,   826, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729,  1149,  1234,  1218,  1218,  1756,
   -1729,  1218, -1729, -1729, -1729, -1729,   153, -1729,  1218, -1729,
    1003,  1003,  1019, -1729,  1642,  1234,   666, -1729, -1729, -1729,
    1003, -1729,  1149, -1729, -1729, -1729, -1729,  1445,  1445, -1729,
    4415,  1005, -1729,  4415,  1218,   826,   826,  1625, -1729,  1003,
   -1729,  1515,  1003,  1799, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729,  1003, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729,  1234,  1234,  1218, -1729,  1234, -1729,  1234,
   -1729,  1332, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
     372,   826,  1218, -1729, -1729,  1003, -1729, -1729, -1729, -1729,
   -1729, -1729,  1234, -1729, -1729, -1729, -1729,   372, -1729, -1729,
   -1729
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     0,     1,     0,     0,     0,     4,     6,     7,
      38,    38,     0,     0,     3,     5,     0,     8,    14,    28,
      27,    29,    29,     0,   276,   276,     0,     0,    24,    40,
       0,     9,    15,    30,    26,  1496,     0,   157,     0,   579,
     579,  1497,     0,     0,     0,     0,     0,     0,    39,   278,
       0,    17,     0,    25,    33,    37,    36,  1523,    35,    42,
     159,     0,   161,   288,   289,     0,   340,   281,   583,    18,
      20,    38,     0,    16,    34,  1524,    32,    41,   165,   163,
     249,     0,     0,   478,     0,   586,   584,   601,     0,    19,
      10,    11,     0,     0,     0,     0,     0,    43,    44,    45,
      47,    46,   160,     0,   247,     0,  1488,   258,   162,   251,
     253,   255,   256,   252,   269,   279,     0,     0,   481,  1331,
     282,   343,   290,   591,   591,     0,     0,     0,   276,    23,
      56,    71,    49,    80,  1450,   166,   165,     0,   158,     0,
    1516,     0,  1514,     0,  1489,  1540,   259,   260,   261,  1470,
     250,   254,   268,   270,   283,   341,     0,     0,   484,   287,
       0,   286,   344,  1438,   292,  1479,   591,   588,   594,     0,
     591,   602,   580,    21,    12,     0,  1496,    54,  1523,    55,
    1523,    60,    62,    63,    64,     0,     0,    70,     0,    73,
    1553,    48,     0,  1552,     0,     0,  1496,  1496,     0,     0,
    1531,  1496,  1496,  1496,  1496,  1496,     0,  1496,  1482,  1496,
      79,    81,    83,    85,    86,    87,    89,    88,    90,    91,
      92,    93,    94,    95,  1451,     0,   164,   249,     0,  1517,
       0,     0,  1515,     0,     0,  1541,  1484,  1471,  1490,   280,
     343,   479,     0,     0,   576,   343,   352,   346,   351,     0,
       0,   589,  1496,     0,   599,   592,   593,   603,   579,  1496,
       0,    57,  1523,    59,    61,     0,  1463,  1496,     0,    77,
       0,    72,    74,    52,    50,     0,     0,  1351,   112,     0,
       0,  1496,  1496,  1532,  1496,     0,     0,     0,     0,     0,
    1496,     0,  1483,     0,    99,    82,    84,   167,   248,  1395,
     275,  1342,  1344,   271,     0,     0,     0,  1485,     0,  1491,
       0,   284,   342,   343,   482,     0,     0,   277,   285,   353,
     349,   350,     0,   359,   360,   347,   363,   363,   354,   305,
    1484,  1496,     0,     0,  1496,  1484,  1510,  1496,  1468,     0,
     291,   293,   296,   297,   298,   299,   300,   301,   302,   303,
     304,     0,     0,  1496,   600,     0,     0,   581,    17,     0,
    1400,    69,    58,  1462,     0,    76,    75,    78,    51,    53,
    1496,   101,   102,     0,     0,     0,   153,   152,   103,   104,
     156,     0,   155,   139,  1498,   141,    96,     0,    97,   169,
    1456,  1457,     0,  1343,     0,     0,     0,   262,   263,   266,
     257,  1329,   480,   343,   485,     0,   348,   364,   365,   355,
       0,   365,   357,     0,     0,  1496,  1468,     0,     0,     0,
       0,     0,  1511,  1496,     0,  1469,     0,     0,   294,   295,
     595,   596,   598,     0,   590,   604,   606,     0,     0,    68,
       0,  1409,  1405,  1410,  1408,  1406,  1411,  1407,   145,   146,
     148,   154,   151,   150,  1500,  1499,   142,     0,   111,   110,
     100,   107,  1538,   105,     0,  1396,   273,     0,   274,   264,
       0,   265,  1330,   483,   487,   577,   373,   367,     0,   317,
     337,  1458,  1459,   326,  1345,   321,   320,   319,  1350,  1349,
    1506,  1482,  1494,     0,   575,   338,   339,  1496,  1496,   597,
     606,     0,     0,    13,    66,    67,    65,   117,   131,   127,
     132,   114,   130,   128,   115,   116,   129,   113,   118,   119,
     121,   147,     0,   140,   143,   108,  1539,  1496,    98,   184,
    1510,     0,  1548,   230,     0,  1510,  1501,  1496,  1480,  1501,
     233,     0,   232,  1552,   217,   216,   168,   170,   171,   172,
     173,   174,   175,     0,   176,   177,   229,   178,   179,   180,
     181,   182,   183,   185,  1496,   272,   267,     0,   486,   488,
     489,   578,     0,  1472,     0,  1498,   358,     0,   307,  1346,
    1507,   328,     0,   310,  1495,  1536,   336,     0,     0,     0,
     612,   616,   607,   608,   609,   610,   615,     0,     0,   120,
     123,     0,   149,   144,     0,   106,  1496,  1501,  1549,   192,
     234,  1496,  1502,  1496,     0,  1481,  1496,  1477,  1496,     0,
    1496,  1496,   241,  1470,     0,  1496,     0,   492,   490,   390,
       0,   464,   399,   432,   420,   429,   426,   423,  1550,   400,
     401,   402,   403,   404,   405,   406,   407,   408,  1527,   362,
     433,     0,   409,   396,   410,   411,     0,     0,  1534,   413,
     414,   412,   460,   416,   417,   415,  1496,  1496,   477,   356,
     374,   375,   376,   377,   378,   379,   397,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,     0,     0,  1473,
       0,   368,     0,   318,   309,   308,   306,   327,  1482,  1537,
     315,   324,   323,   325,   322,     0,   614,   617,   675,   726,
     735,   742,   746,   770,   775,   793,   786,   794,   795,   801,
     834,   843,   845,   872,   880,   882,  1548,   888,     0,   899,
     920,   922,   958,   960,   964,   674,   970,   983,  1003,  1020,
    1022,  1026,  1033,  1034,  1050,  1070,  1088,     0,  1107,  1118,
    1126,  1128,  1130,  1132,  1137,  1159,  1182,  1554,   611,   623,
     624,   625,   626,   627,   628,   629,   630,   632,   631,   633,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
     644,   645,   646,   647,   648,   649,   650,   651,   652,   653,
     654,   655,   656,   657,   658,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,   669,   670,   671,  1155,  1156,
    1157,   672,   673,    22,     0,   122,   109,     0,  1496,   194,
     193,   189,     0,     0,  1476,   233,   228,     0,     0,   231,
       0,     0,   240,  1521,  1521,     0,   242,     0,   213,  1496,
     476,   430,   431,   418,   419,   427,   428,   424,   425,   421,
     422,  1551,     0,  1528,   458,   440,   391,  1391,   474,  1535,
     461,   462,   459,     0,     0,     0,  1390,   393,   395,  1454,
    1454,     0,  1514,  1514,   372,   369,  1401,  1403,   466,   468,
     470,  1475,   329,   330,   331,   332,     0,     0,   311,  1493,
     317,     0,     0,   618,     0,     0,     0,  1246,   741,     0,
     772,   777,     0,     0,     0,     0,     0,     0,  1246,   874,
       0,     0,   884,   889,     0,  1246,     0,     0,     0,     0,
       0,     0,   972,   993,     0,     0,     0,     0,     0,     0,
       0,     0,  1102,  1100,     0,     0,  1127,  1125,     0,     0,
       0,     0,  1160,  1166,     0,     0,   137,   133,   138,   136,
     134,   135,   124,   125,   202,   203,   201,   200,     0,   187,
     188,  1508,   222,   221,   222,   218,   246,   235,   236,  1441,
     239,  1522,   243,   244,   245,  1347,  1496,   501,   501,  1498,
     521,   493,   496,   497,     0,   463,     0,  1546,     0,  1392,
    1393,     0,   398,   465,     0,     0,   392,  1480,   434,   435,
    1402,     0,     0,     0,  1550,   469,     0,     0,     0,  1474,
    1482,   316,   605,   613,   724,   694,  1514,     0,     0,  1424,
    1427,  1514,  1322,     0,     0,     0,     0,     0,     0,     0,
       0,  1424,   733,  1366,   731,  1356,  1358,  1364,  1365,  1443,
     736,     0,  1245,  1267,  1341,     0,  1337,  1339,  1338,  1385,
     748,  1384,  1386,   774,   771,   776,   789,     0,  1324,  1452,
    1525,     0,  1405,   832,   694,     0,  1358,   841,     0,   748,
     851,   850,  1466,   847,   849,   879,   876,   875,   878,   873,
    1514,   881,  1352,  1354,   883,  1335,   893,  1544,  1244,   901,
     921,   503,     0,   924,   925,   926,   959,  1074,     0,   961,
       0,   968,     0,   971,   994,  1341,   984,   993,   986,     0,
     991,     0,  1338,     0,  1445,  1184,  1327,  1525,  1184,     0,
    1048,  1039,  1328,     0,  1334,  1051,  1052,  1053,  1054,  1055,
    1063,  1056,  1066,     0,  1332,     0,  1071,  1089,  1103,  1104,
    1478,     0,  1109,  1111,     0,  1123,     0,  1129,     0,  1134,
    1139,  1167,     0,  1168,  1516,  1184,     0,  1441,   196,   195,
     186,     0,     0,   220,   219,  1461,  1496,     0,   212,   206,
     237,  1442,  1348,   214,     0,   502,   498,     0,   499,     0,
     491,   494,   441,  1547,   442,  1514,     0,     0,     0,  1310,
    1308,  1373,  1313,  1367,  1371,  1372,     0,  1394,   475,     0,
     394,  1455,   371,   370,  1404,  1529,   471,   335,  1506,     0,
     313,   725,   676,  1475,     0,   703,     0,     0,     0,     0,
       0,  1412,  1429,  1423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1413,   734,   727,     0,     0,  1357,
    1444,   739,  1443,  1247,     0,   744,     0,   749,   759,  1383,
     773,  1382,   779,   790,   787,   792,   791,  1246,  1325,  1453,
    1326,  1526,  1233,   800,   833,   802,   812,  1199,  1199,  1199,
    1199,   842,   835,     0,     0,   844,  1467,  1246,   870,   857,
     853,   855,  1246,   877,     0,  1353,   886,  1545,   891,   903,
       0,   504,     0,   947,   932,   923,   927,   929,   930,   931,
    1078,     0,     0,   969,   965,     0,   977,   974,   976,   975,
     978,   985,   988,   619,  1246,     0,     0,   995,     0,  1446,
    1447,  1525,     0,  1021,  1005,  1028,  1041,  1049,  1035,     0,
    1041,     0,  1378,  1379,  1064,  1067,     0,     0,  1333,  1062,
       0,  1061,     0,  1091,     0,     0,  1101,     0,     0,  1110,
       0,  1124,  1119,     0,     0,  1135,  1136,  1133,  1478,     0,
       0,  1169,     0,     0,  1028,   126,   206,   204,   197,   198,
       0,   225,   211,  1460,  1441,     0,   495,   500,   506,   516,
     363,   522,  1516,   436,     0,  1318,  1319,     0,  1311,  1312,
    1397,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1530,  1548,   334,  1506,  1516,   312,   699,   690,  1199,   680,
     687,   681,   683,   685,     0,  1199,     0,   679,   686,   693,
     692,     0,  1199,  1512,  1512,  1512,   697,   698,  1375,  1374,
       0,  1363,  1310,  1308,     0,     0,  1310,     0,  1359,  1360,
    1361,  1323,  1310,     0,     0,  1310,     0,     0,  1310,  1310,
    1310,     0,     0,  1206,  1452,     0,     0,   737,     0,  1258,
    1259,  1260,  1293,  1261,  1548,  1297,  1302,  1542,  1268,  1305,
    1542,  1286,  1265,  1275,  1257,  1256,  1294,  1264,  1266,  1276,
    1277,  1278,  1279,  1280,  1295,  1249,  1298,  1300,  1281,  1282,
    1283,  1284,  1285,  1252,  1253,  1254,  1255,  1269,  1292,  1263,
    1274,  1251,  1250,  1262,  1271,  1272,  1273,  1270,  1287,  1288,
    1289,  1290,  1291,  1248,     0,     0,  1340,   755,     0,     0,
     762,   784,   785,   778,   780,     0,  1206,  1238,  1240,   797,
    1234,  1235,  1236,     0,  1553,  1199,     0,  1200,   805,  1202,
     806,   803,   804,     0,  1206,  1452,   865,   867,   866,   860,
     862,   868,   871,   846,   858,   854,   852,  1246,   619,   848,
    1355,  1516,   885,  1336,   619,  1548,   911,   912,   914,   916,
     917,   913,   915,   906,  1548,   902,     0,   948,     0,   950,
     949,   951,   942,   943,     0,     0,   928,  1080,  1518,     0,
       0,   962,  1206,  1452,  1552,     0,   989,   620,   996,   997,
    1000,     0,   992,  1191,  1190,   999,  1005,  1185,     0,     0,
    1233,     0,     0,     0,  1040,     0,     0,     0,  1065,     0,
    1069,  1068,  1059,     0,  1496,  1233,  1106,  1105,  1112,  1113,
    1114,     0,  1206,  1452,     0,  1439,     0,  1114,  1181,  1171,
    1170,  1176,     0,  1178,  1179,  1186,  1441,   199,     0,   208,
     209,   238,   207,  1496,   508,   519,   520,   518,   524,     0,
     450,   451,  1478,   439,   452,   448,   446,  1501,   444,  1368,
    1369,  1370,  1321,  1309,  1314,  1315,  1316,  1317,  1320,  1398,
       0,   559,   472,   333,     0,   691,  1202,   682,   684,  1199,
     688,   678,   718,  1496,   707,   708,  1496,   719,   709,   710,
     713,   723,   720,   711,     0,   721,   712,   722,   704,   705,
     677,  1513,     0,     0,     0,   695,   696,  1377,  1362,  1376,
    1424,  1452,     0,  1428,     0,  1424,  1424,     0,  1421,  1424,
    1424,  1424,     0,  1424,  1424,  1207,   728,  1209,  1206,   740,
       0,  1296,  1543,  1299,  1301,   745,   743,   750,   751,   594,
       0,   761,   760,  1172,  1173,   765,   763,     0,   783,     0,
     788,   619,   619,   798,   796,  1237,   812,   812,   812,   812,
    1496,   817,   830,   831,   818,     0,  1496,   821,   822,   825,
     823,     0,   824,   814,   815,   807,   813,   619,  1203,  1198,
       0,   836,     0,  1246,  1246,   864,   619,   861,   856,     0,
     894,     0,     0,   918,     0,     0,     0,   944,   946,     0,
     938,   954,   939,   940,   933,   934,   954,  1072,  1496,     0,
    1519,  1079,  1501,   963,   966,     0,     0,   980,   990,   987,
     622,     0,     0,  1007,  1006,  1222,  1224,  1024,  1219,  1220,
    1031,  1029,     0,  1246,  1042,  1246,  1036,  1044,  1057,  1058,
    1060,  1448,  1098,  1213,     0,  1452,  1120,     0,     0,  1440,
    1140,  1141,     0,  1144,  1147,  1151,  1145,  1177,  1516,  1180,
    1192,  1464,   205,     0,   226,   227,   223,     0,     0,   510,
       0,  1533,   443,     0,   437,   453,   449,  1496,   438,   445,
    1399,  1496,   467,   314,  1197,   689,     0,     0,  1242,  1242,
     706,   701,   700,   702,  1417,  1206,  1425,     0,  1437,  1422,
    1415,  1435,  1416,  1418,  1419,  1432,  1433,  1420,  1414,   619,
    1210,  1205,   729,   738,  1548,     0,  1548,     0,   752,   753,
       0,   757,   756,   758,  1174,  1175,   768,   766,   619,   781,
     782,  1239,  1241,  1199,  1199,  1199,  1199,     0,   819,   820,
       0,  1242,  1242,   816,  1201,   619,  1206,  1324,  1206,  1324,
     863,   869,   859,   887,   895,   897,   904,   907,   908,  1486,
     919,   900,   905,   954,  1380,  1381,   954,     0,   937,   935,
     936,   941,  1082,     0,  1520,  1496,  1206,   979,   973,     0,
     621,  1001,     0,     0,  1013,     0,   619,   619,  1025,  1023,
    1221,  1032,  1027,  1030,  1037,   619,  1046,  1045,  1449,     0,
       0,  1099,  1090,  1214,  1116,  1216,     0,  1206,  1206,  1131,
    1439,  1143,  1494,  1149,  1494,  1213,     0,  1229,  1231,  1195,
    1193,  1226,  1227,  1194,  1465,     0,   224,   507,  1496,     0,
     512,   517,  1512,   553,   573,   568,  1468,     0,     0,  1496,
    1514,  1496,   523,   529,   530,   531,   540,   532,   534,   537,
     541,   525,   526,   527,   533,   536,   554,   538,   528,     0,
     535,   539,   457,   454,   455,     0,     0,  1389,   715,  1387,
    1388,   714,   717,     0,   716,   730,  1426,  1208,   619,  1304,
    1548,  1307,  1548,   754,   769,   747,   619,   764,   811,   810,
     809,   808,   827,   826,   829,   828,  1204,   838,     0,   837,
       0,   619,   898,   892,   909,  1487,     0,   953,   945,   954,
     956,     0,     0,  1085,  1081,  1076,   967,   982,     0,     0,
    1008,  1496,  1015,     0,  1009,     0,  1012,  1223,  1225,   619,
    1043,   619,  1092,  1093,  1094,  1095,  1096,  1097,   619,  1117,
    1108,  1217,  1212,  1115,  1122,  1121,  1142,     0,  1494,  1146,
       0,  1153,  1165,  1162,  1164,  1163,  1158,  1161,   619,   619,
    1196,  1183,  1228,  1189,  1188,  1503,     0,  1496,  1496,   514,
     552,  1496,   574,   572,   569,   570,  1498,   562,  1496,  1246,
       0,     0,     0,   555,     0,     0,   560,   563,   566,   456,
     447,   473,  1503,  1211,  1303,  1306,   767,  1206,  1206,   896,
       0,   952,   957,     0,  1496,  1083,     0,     0,  1073,  1075,
     981,     0,     0,  1018,  1016,  1017,  1011,  1010,  1038,  1047,
    1215,   619,  1148,     0,  1152,  1154,  1138,  1230,  1232,  1504,
    1505,  1187,   509,     0,     0,  1496,   505,     0,   561,     0,
     558,  1452,   556,   557,   547,   545,   546,   548,   544,   549,
     543,   542,   567,   565,   564,  1243,   840,   839,   910,   955,
       0,  1086,  1496,  1077,  1246,  1014,  1019,  1004,  1218,  1150,
     511,   513,     0,   551,   550,   571,  1084,     0,  1002,   515,
    1087
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1729, -1729, -1729, -1729,  1934, -1729, -1729, -1729,    81, -1729,
   -1729, -1729, -1729, -1729,  1584, -1729, -1729, -1729,  1235, -1729,
   -1729,   111,  1919, -1729, -1729,  1892,   697, -1729, -1729, -1729,
   -1729, -1729,  1754,  1816, -1729, -1729,  1772,    59, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729,  1763, -1729, -1729, -1729, -1729,
    1747, -1729, -1729, -1729, -1729, -1729,   249,   615, -1729, -1729,
   -1729, -1729,  1439, -1729, -1729,  1353,   804, -1729, -1729, -1729,
   -1729, -1729, -1729,  1513, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729,  1828, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,   597, -1729,
     587,   808, -1729, -1729, -1729, -1729, -1729,  1004,    94, -1729,
    1362, -1729, -1729, -1729, -1729, -1729, -1729,   144, -1729, -1729,
    1748, -1729,  1868, -1729, -1729, -1729, -1729,  1581, -1729, -1729,
    1866,   648, -1729, -1729, -1729, -1729,  1751, -1729,  1931,  1823,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,  1097, -1729,
   -1729, -1729,  1398, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729,   445, -1729, -1729, -1729,  1738,
    1670, -1729, -1729,   813, -1729, -1729,  -315, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729,   113,   114,  1133,
     116, -1729,   117, -1729, -1729, -1729,   332,   335, -1729,   415,
   -1729,   341, -1729,   -64,   130, -1729, -1729,   131, -1729,   133,
   -1729, -1729, -1729,  1132, -1729, -1729, -1729,   135, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729,  1446, -1729, -1729, -1729,
    1040, -1729,  -931, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
     -34, -1729, -1729, -1729, -1729, -1729,  -177, -1729, -1729, -1729,
   -1729,   274, -1729, -1729,   269, -1729, -1729, -1729, -1729, -1729,
    1908,  1112, -1729,   284, -1729, -1729, -1729, -1729,  1537, -1729,
   -1729, -1729, -1729, -1729, -1729,  -899, -1729, -1729,   204, -1729,
   -1729, -1729, -1729,   971,   609,   612, -1729, -1729,   333, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729,   973, -1729, -1729,   293, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,  -278, -1729,
     260, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729,   762, -1729, -1729,   766, -1729, -1729, -1729, -1729,   493,
     256, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729,    84,   763, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729,   767, -1729, -1729, -1729,
     243, -1729, -1729,   476, -1729, -1729, -1729, -1404, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1584,
     954, -1729, -1729,   229, -1729, -1729,   457, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,   701, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729,   736, -1729,   220, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
     939, -1729,   942, -1729, -1729,  1139, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729,   931,   438, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729,    57, -1729,   443, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729, -1729,  -248, -1729,
   -1188, -1729, -1729, -1229, -1148, -1199, -1729,   390, -1729, -1478,
   -1729, -1729, -1729, -1729,    62, -1729, -1729, -1729, -1729,   -47,
   -1729, -1729,   244, -1729, -1729, -1729, -1729,    50, -1729,  -481,
   -1728, -1729, -1729,   553, -1729,  -796, -1295,  -857, -1260, -1729,
   -1729, -1729, -1228, -1203, -1201, -1200, -1197,     1,  -220,  -622,
    -674, -1076,  -894,  -660,   967, -1048,   -84, -1729, -1113, -1729,
    -846, -1729,   842,  -226,  -136, -1729, -1729,  -704,   705,  -847,
   -1000,    65,  -802, -1729, -1729,   470, -1056, -1640, -1030,  1191,
    -807,   463,  -638,  -192, -1729,  1102,  -223,  -659,  -312,  -301,
    -606,  -969, -1729, -1729, -1729, -1729, -1729,  1845, -1729, -1122,
     854, -1729, -1729, -1729, -1668,  1223,    86,  1769,   801,  -455,
   -1729,  1031,  -413,  1481, -1729,  -652, -1729, -1094,  1115,  -437,
     823, -1729, -1729,  -730, -1729, -1298,  -175,  -570,  -529,   -97,
   -1034, -1729,   640, -1375,  -858, -1131, -1729,  1272,  2050,  -887,
   -1729, -1729, -1729, -1729, -1729, -1729, -1729,   641, -1729, -1729,
    -720,  1117,  -130, -1729
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
     517,   518,   519,   814,   952,   520,   953,   215,   383,   384,
     523,   216,   448,   449,   217,   218,   219,   220,   221,   222,
     223,    48,    78,    80,   104,   102,   135,   389,   464,   547,
     548,   961,   821,  1160,  1368,   549,   957,   550,  1374,  1375,
    1649,  1169,   551,   552,   553,   554,   965,  1163,  1876,   555,
     556,   557,   558,   559,   560,   561,   562,   836,   563,   138,
     108,   109,   110,   111,   149,   112,   397,   398,   471,   113,
     114,    31,    66,   154,    84,   239,   159,   120,   160,   121,
     164,   250,   341,   342,   696,   343,  1405,   890,   578,   344,
     487,   345,   702,   346,   347,   697,   882,   883,   884,   885,
     348,   349,   350,    83,   240,   161,   162,   163,   247,   248,
     325,   476,   478,   326,   327,   669,   409,   410,   573,   874,
     328,   572,   670,   671,   672,   996,   673,   674,   675,   676,
     677,  1663,   678,   987,  1383,  1888,  1664,  1665,  1666,  1667,
    1884,  1668,  2073,  2074,   679,   680,   862,   681,   682,   683,
     576,  1004,   878,   879,  1892,   684,   685,   686,   118,   313,
     158,   403,   244,   474,   568,   569,   570,   839,   981,   982,
    1176,  1177,  1090,   983,  1654,  1879,  2040,  2179,  2246,  1378,
    1657,  1180,  1381,  1881,  2061,  2062,  2261,  2063,  2064,  2065,
    2066,  2252,  2067,  2068,  2069,  2196,  2197,  2186,  2070,  2071,
    2183,   495,   317,   571,    51,   257,   436,    87,   124,   123,
     166,   167,   168,   254,   355,   126,   357,   500,   501,   592,
     593,   594,   595,   596,   894,  1596,  1597,  1830,   597,   759,
     760,   895,  1014,  1215,  1426,  1427,  1422,  1708,  1709,  1212,
     761,   896,  1032,  1238,  1236,   762,   897,  1040,  1457,   763,
     898,  1515,   764,   899,  1248,  1517,  1747,  1748,  1749,  1520,
    1755,  1938,  1936,  2096,  2095,   765,   900,  1054,   766,   901,
    1055,  1523,  1524,   767,   902,  1056,  1254,  1257,   768,   769,
     770,   903,  1764,   771,   904,   772,   905,  1063,  1535,  1783,
    1784,  1265,   773,   906,  1067,  1272,   774,   907,   775,   908,
    1072,  1073,  1278,  1279,  1280,  1558,  1556,  1796,  1281,  1549,
    1550,  1795,  1553,   776,   909,  1079,   777,   910,   778,   911,
     779,  1085,  1562,   780,   913,   781,   915,  1564,  1965,  2111,
    2113,   782,   916,  1289,  1573,  1803,  1967,  1968,  1969,  1971,
     783,   917,   784,   918,  1092,  1295,  1296,  1297,  1585,  1814,
    1815,  1298,  1582,  1583,  1584,  1808,  1299,  1978,  2213,   785,
     919,   786,   920,  1099,   787,   921,  1101,  1304,   788,   922,
    1103,  1310,  1595,  1988,   789,   923,  1106,  1313,  1829,  1107,
    1108,  1109,  1599,  1600,   790,   924,  1609,  1994,  2132,  2223,
    2277,   791,   925,   792,   926,  1999,   793,   927,  1610,  2002,
     794,   795,   928,  1120,  2139,  1330,  1612,  2005,  1846,  1847,
    2141,  1328,   796,   929,  1125,  1126,  1127,  1128,  1342,  1129,
    1130,  1131,  1132,   797,   930,  1096,  1982,  1300,  2219,  1587,
    1817,  2123,  2218,   798,   931,  1343,  1625,  2009,  2012,   799,
    1139,  1138,  1346,   800,   934,  1141,  1142,  1853,  2150,   801,
     935,  1145,  1352,   802,   937,   803,   938,   804,   939,   805,
     940,  1357,   806,   941,  1359,  1860,  1861,  1637,  1863,  2023,
    2159,  2025,  2236,   807,   808,   943,  2166,  1153,  1362,  1641,
    1756,  1937,  1868,   809,  1643,   810,   811,   945,  1323,  1870,
    2120,  2029,  2171,  1685,  1538,  1539,  1787,  1789,  1955,  1736,
    1737,  1919,  1921,  2088,  2014,  2015,  2148,  2152,  2231,  1837,
    1838,  1996,  1839,  1997,  2030,  2031,  2168,  2032,  2169,  1529,
    1530,  1531,  1761,  1532,  1762,  2082,  1087,  1088,  1042,  1043,
    1243,  1244,  1488,  1489,  1490,  1491,  1492,  1189,  1391,  1433,
    1033,  1057,  1258,  1115,  1121,   400,   401,  1133,  1134,  1286,
    1110,  1046,  1047,   300,   301,   483,  1173,   490,   278,  1081,
    1082,  1034,  1059,  1192,  1430,  1718,  1816,  1973,  1065,  1111,
    2078,  1036,   866,   857,   989,   990,  2080,  1037,   876,   877,
    1038,  1221,  1223,  1437,  1451,  1446,  1443,   249,  1862,  1170,
    1241,  1321,  2010,   225,  1260,   998,   392,   417,  1171,   267,
    2035,  1793,   426,   238,   690,  1216,   619,   169,   616,   293,
     308,  2116,   145,   310,   891,   585,    43,   457,   613,  2241,
     581,  1161,   423,  1712,   233,   230,  1821,   972,   185,  1262,
     854,  1401,   284,   688,   700,   527,   236,  1743,  1288,  1184,
     609,   852,  1536,   812
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     122,   260,   194,   482,   303,   692,   912,  1147,  1058,   564,
     620,   621,   412,  1263,  1001,  1002,  1508,  1551,   856,  1598,
    1338,   279,   280,  1363,  1250,   944,   285,   286,   287,   288,
     289,   875,   291,   880,   294,  1367,   302,   361,   302,   302,
    1041,  1509,   302,  1510,  1511,   886,  1347,  1512,  1760,  1713,
    1714,  1074,  1045,  1905,   583,   105,  1276,  1080,  1080,   292,
    1388,  1080,  1234,   194,  1080,  1119,  1791,  1331,   881,  1089,
     165,   600,   450,    85,  1165,   351,   122,   352,   818,  1080,
    1388,  1285,  1135,  1642,   359,  1388, -1474,  1388,  1007,  1285,
     454,   386,   364,   302,  1035,   304,  1388,   302,   306,   302,
    1546,  1420,   165,  1035,  1068,   105,   373,   374,   302,   375,
     833,   302,  1898,   307,  1824,   381,  1766,   504,  1100,  1102,
    1540,  1541,  1542,   967,    22,   165,   497,   694,   407,   432,
    1605,   186,  1143,  1035,   165,  1285,   438,  1213,   871, -1478,
     165,    54,  2033,   407,  1376,   488,  1931,   450,  1951,   376,
   -1516,    89,  2133,   380,  1856,   382,   414,  1406,  1217,   419,
     421,  1453,   424,  1224,   393,    41, -1552,   393,   466,  1589,
     468,  1124,   302, -1474,  1402,  1753,   454,   701,   433,  1339,
     959,   265,   302,    92, -1552,  1273,  1690,  2017,   869,   302,
     479,    14,  1199,  1710,  2134,   440, -1552,   491,  1544,   127,
     465,  1123,   302,  1924,   302,   993,  1561,  1577,  1370,  1686,
     499,  -190, -1552,    19,  2135,  1158,  1686,  1423,  2019,   880,
     860,   602,  1284,  1686,   399,   484, -1552,   302,   439,  1592,
    1324, -1391,  1239,  1660,   524,    55,  1576, -1516,   451,   264,
     481,   565,    41,   255,  1165,   453,  1377,   566,   493,   252,
    1423,  1659,  1651,  1151,  1578,   889,   841, -1478,  1660,  1094,
    1922,   888,  1239,   190,  1635,  1165,  1314,  1118,  1124,   415,
    -191,   416,   871,  1684,   302,   302,   302,  1632,   274,  1315,
    1152,   455,     3,   489,  1336,  1155,  1292,  1214,   988,    20,
    1926,   579,  1607,  1052,   268,   253,  1301,  1551,  1634,  1124,
     603,  1754,   574,  1207,  1208,   617,   842,  -345,  -345,    52,
    1044, -1492,  1095,   399,  1191,  1371,   472,  1052,   623,    56,
    1661,   264,   587,   588,   441,  1767,  1337,  1384,  1044,  -190,
     393,  1925,   393,  1159,   106,  1915,  1285,  2119,  1534,  1143,
   -1474,  1533,   861,  1058,  1239,  1661,  1274,  1206,  2121,  1428,
    1389,   299,   604,   256,   693, -1523,  1534,  1209,  1689,   698,
      12,   834,   614,  1202,  1203,   442,  1190,   455,   960,  1683,
    1389,  1579,   443,   302,     4,  1389,   870,  1389, -1478,  1738,
    1058,   618,  1195,   299,  1424,  1932,  1389,  1785,  -191,   626,
      55,  2136,   695,  1167,   299,   703,   703,   687,   498,   408,
    1526, -1478,   229,    63,  2225,   271,   582,   -31, -1474,  1179,
   -1478,  1058,  1981,   624,   411,  1899, -1478,  -587,  1927,   835,
    1769,  1580,   954,  1750,   360,  1074,  -585,  2085,   855,   393,
    1799,   817,  1662,   107,  1606,  1454,   822,   277,   823,   976,
    1563,   827,  1513,   828,   444,   830,   831, -1474,  1390,  1008,
     837,  1952,  1425,    86,   601,  1591,  1739,  1871,   441,  1058,
    1895,  1340,  1768, -1478, -1496,   858,   977,   978,  1720, -1478,
    1602,  1543,  1545, -1430,    56,  1730,  1617,  1277,  2107,   190,
    2109,   266,  1341,  1547,  1731,  1425,   612,  1548, -1478,   152,
    1686,   863,   864,   299,   299,  1933,   190,   299,   299,   442,
     299,  1052,     5,  1593,   505,  1403,   443,   445,  2126,   277,
    1429, -1478,  1191,  1191,  1191,   299,   299,  1644,    89,  1166,
   -1478,   269,  1191,  1916,  1872,  1293, -1478,   358,  1809,  -345,
     190,   177,  -587,  1551,   299,  -190,   446,   175,  1622,  2154,
    2155,  -585,  1626,  1627,  1191,  1629,  1191,  1827,  1191,   106,
    2006,  1633,  1191,  1191,  1191,  1191,  1191,  1191,  1191,  1191,
    1848,  1058,  1388,  1167,  1385,  1386,  1387,   887,  1883,  2117,
    1000,   299,  2118,  1210,  1398,  2122,  1835,   447,   444,  1431,
    1195,  1195,  1195,  2285,  1167,  -210,  1438,    88,  1294,    64,
    1195,  1285,    41,    63,  -191,   502,   962,  1681,  1435, -1552,
    1168, -1552,    75,   390,  1442,  1444,  1445,  1447,  1044,  1581,
    1450,  1452,  1195,   598,  1195,   955,  1195,  2162,  1440, -1552,
    1195,  1195,  1195,  1195,  1195,  1195,  1195,  1195, -1496,   881,
     299,   302, -1476, -1501,    13,   302,  2077,   612,   302,   302,
      65,   445,   299,   958,  1388,   302,  1560,  1249,   107,  1717,
     176,  -732,   871,  1750,    41, -1523,  1792,  1527,  2163,  1798,
   -1438, -1438,   430,   956,   984,  1800,  2164,  2181,  1011,  1746,
     446,   979,  1825,    32,  1613,  1857,  1194,  1758,  1613, -1552,
     391,  1682,  1901,  1902,  1903,   312,  2286,   966, -1552,  1344,
     318,   489,  1518,   872,   969,   970,   309,  1759,     4, -1552,
     496,   975,  1204,  2290,   508,    16,  1048,   946,    18,   525,
    1528,   447,    23,  1836,  1956,  2211,  1958,  1191,  1191,  1191,
    1191,  1191,  1191,  1191,  2157, -1474,  2160,  1097,   963,  2266,
    2267,  1112,  1388,  1116,   177,  1116,  1122,  2026,  1097,  2165,
    1923,   431,   190,  1823,  1741,   509,  1669,   507,   947,  1986,
    1976,  1904,   510,  1116,   150,   948,  1909,  1910,   402,     5,
    1912,  1913,  1914,  1182,  1917,  1918,   302,     5,    91,  1673,
    1674,  1675,  1676,  1677,  1678,  1680,   174,   605,  1671,    64,
      26,  2018,  2143,   302,   441,  1195,  1195,  1195,  1195,  1195,
    1195,  1195,  1719,  1113,  1757,  2098,  2099,  2100,  2101,  1198,
     190,  1174, -1552,  1993,  2083,  2083,  2021,  2144,   964,  2145,
    2146,  1333,  1259,  2147,   299,   302,   302,  1259,   508,  1060,
    1819,  1850,  1157,  1285,   512,   442,     5,   949,    30,   190,
     299,   329,   443,  1259,   980,   175,  1259,  1319,   441,  1172,
    1114,   627,  1117,  1345, -1552,  1801,  1519,  1137,   473,   843,
   -1490,    29,  1389,   299,  1804,   299,  1149,  2083,  2083,   509,
    2233, -1501,  1941,  1942, -1552,   485,   510,   360,   873,   932,
    1325,   489,   489,  2216,  1194,  1194,  1194,   330,   511,   442,
     190,   119,   246,  1320,  1194,   360,   443,   513,  1954,   320,
     950,    33,   331,   119,  2250,  1929,  1957,  1962,  1959,   844,
      75,  1333,   360,   281,   444,  1820,  1194,  1364,  1194,   622,
    1194,   290,  1439,  1306,  1194,  1194,  1194,  1194,  1194,  1194,
    1194,  1194,  2174,    34,  2194,  1566,   195,  1122,  1567,  1568,
     297,  1058,   332,   529,  1389,   441,   530,  1961,   512,  1873,
     196,   197,  1392,  1393,  1394,  1395,  1565,  1396,   176,  1566,
   -1436,   531,  1567,  1568,  1307,    36,  1379,   516,   444,  1290,
     951,   532,  1308,  1058,  1835,  2195,   486,   445,    38,  1369,
    1064,  1252,   198,   175,  2217,   533,   442,  1113,  2167,  2288,
    1333,  1719,   933,   443,   371,   372,  2004,    49,   252,    45,
     360,  1372,   378,   379,    44,   265,   446, -1496,   385,   388,
    1146,   513,  1124,    50,   514,   515,   534,  1874,   146,  1875,
     175,   199,   200,  1603,   201,   819,    53,   302, -1496,   845,
    2087,   445,  1389,   202,  1392,  1393,  1394,  1395,    46,  1396,
    1992,   299, -1434,   428,   253,  1309,    47,   447,    75,  2097,
    1604, -1552,  1333,   847,    41,   190,   261,  2173,  1191,   333,
     446,  1527,    59,  1191,   429,   444,  2106,    93, -1490,   820,
    1810,   334,   366,   147,   148,  1658,   203,   204,  2027,   846,
      60,   516,  1670,   489,  2215,    75, -1552,   205,  2142,  1194,
    1194,  1194,  1194,  1194,  1194,  1194,   176,   367,    61,   456,
    2081,   447,   824,   848,  1048,   849, -1552,  2137,  2138,   825,
    1907,  1836,  1333,  2084,  1528,  1911,  2140,  1811,  1225,  2254,
    2255,    68,  1392,  1393,  1394,  1395,  1195,  1396,   445,    94,
   -1431,  1195,  1525,   176,  1048,  1017,   309,  1527,  2093,  1840,
     867,   865,  1818,  1246,   533,  2256,   139,   542,  1887,   942,
    2102,  1019,     5,  2103,  1852,   850,  1975,   446,   140,   335,
     336,   868,   206,   413,    62,  2104,  2105,    72,   420,    95,
    1521,    96,   337,    75,   338,   534,  1569,  1570,  2271,   141,
     606,  1048,   535,  1075,  1076,   611,  1647,   142,  2028,   472,
    1528,  1841,   302,   302,   263,  1571,  1572,  1077,   447,  2203,
    1569,  1570,  2191,   536,  1070,  1611,   299,  2206,  1071,  1975,
    2127,  1842,  2077,  1639,  2089,    79,  2091,  1640,    81,  1571,
    1572,  1620,  2209,  1577,  2207,  1621,  2208,   103,   537,  1432,
    2128,   362,  1724,  1436,   538,  1727,   115,  1020,  2175,    82,
    1732,  1448,  1449,  1078,  1611,   323,   324,    10,  1650,  1650,
    2228,    10,  2229,   458,  1770,   459,   441,  1771,  1355,  2230,
    1356,   539,  1522,   116,   143,  1772,  1773,  1774,   540,   117,
    1578,  1392,  1393,  1394,  1395,   339,  1396,   207,   119,  2237,
    2238,   541,  1333,  1333,  1812,   125,  2202,   190,   251,  1049,
     129,  1023,   251,  1277,  1024,  1554,   340,   442,   130,  1394,
    1395,  2275,  1396,  1985,   443,   266,   542,  1934,   543,  1935,
    1392,  1393,  1394,  1395,  1333,  1396,   544,   208,  1948,   816,
    1949,   131,  1397,  2257,  2258,   132,  -215,  1025,  1975,  1392,
    1393,  1394,  1395,  1048,  1396,   137,  1874,  2221,  1875,  1655,
    1656,  1722,  2278,   134,  1407,  1408,   133,   545,   136,  1392,
    1393,  1394,  1395,   144,  1396,   156,  1016,  1672,   157,   871,
   -1383, -1383, -1383, -1383,  2259,  2260,   155,   209,  1015,  1711,
    2180,   171,  1051,  2184,  2185,   172,   444,   173,  1066,  1775,
    2204,   295,  2205,  1083,  1084,   186,  1409,  1579,  1410,  1083,
    1091,  1093,  2239,  2240,   224,  1026,  1051,  1776,   546,   227,
     228,   229,  1083,  1267,  1268,  1269,  1270,   231,   871,  -947,
     232,  1091,  1148,   234,  1150,  1777,   235,   237,  2268,   241,
    1194,  2269,  1411,  1412,  1413,  1194,   242,  2201,  1392,  1393,
    1394,  1395,  1017,  1396,   243,   246,  1726,  1580,   259,   445,
    1027,  1028,    41,   273,  1880,   276,   282,   277,  1019,   283,
    -947, -1382, -1382, -1382, -1382,   299,   292,  -947,   305,  1851,
     307,  1193,   309,  1778,   302,   314,  1029,   316,   446,  1414,
     315,  1415,  1813,   353,  1826,   319,   354,   302,   356,  1416,
     360,  1017,   365,   368,  1030,   369,   377,   363,  1877,  1464,
    1465,  1219,  1031,   195,   395,   299,   394,  1019,  1943,  1944,
    1945,  1946,   302,   299,  1333,   396,  1255,   196,   197,   447,
    1854,  1779,  1333,   404,   405,   422,   425,  1854,  1466,   427,
    1650,   434,   452,   435,  1467,   470,   467,  1251,  1896,  -947,
     475,  1897,  -366,  1882,  1020,  1392,  1393,  1394,  1395,   198,
    1396,   477,   302,  1729,   492,   441,   480,   494,   522,   567,
     526,   580,   575,  1417,  1083,  1469,   577,   584,  1893,  1470,
    1333,   610,  1083,  1291,  1780,   608,   607,  1124,  1392,  1393,
    1394,  1395,   586,  1396,  1781,  1021,  1734,   612,   199,   200,
    1051,   201,  1022,  1020,   615,   190,   442,   689,  1023,   625,
     202,  1024,  -947,   443,   441,   691,  1332,   706,  1908,   699,
     853,   813,   829,   838,  1083,  1947,   707,   840,  1083,   851,
     859,  1950,   892,   893,  -799,   936,   617,  1350,   968,   971,
    1291,  -947,   974,   985,  1025,  1581,  2248,   988,   995,   986,
     994,   991,   997,   203,   204,   442,  1782,  1023,  1009,  1003,
    1024,  1006,   443,  2187,   205,  1012,  1013,  1251,  1039,  1053,
    1091,  1069,  1086,  1983,  1061,  1154,  -947,  1175,  1162,  1193,
    1193,  1193,  -947,  1156,  2037,   444,  1181,  1183,  1200,  1193,
    1196,  1211,  1692,  1025,  1693,   881,  1240,  1694,  1218,  1220,
    1392,  1393,  1394,  1395,  1235,  1396,  1332,  1695,  1890,  1242,
    1259,  1193,  1026,  1193,  1222,  1193,  1245,  1247,  1226,  1193,
    1193,  1193,  1193,  1193,  1193,  1193,  1193,  1989,  1253,  1459,
    1460,  1461,  1227,  1995,   444,  1228,  1229,  1462,  1230,   206,
    1418,  1231,  2075,  1232,  1261,  1264,  2076,  1233,   445,  1271,
    1283,  1392,  1393,  1394,  1395,  1287,  1396,  1027,  1028,  2086,
    1302,  1026,  1303,  1305,  1312, -1385,  1316,  1317,  1318,  1322,
    1327,  1329,  1124,   472,   165,  1463,  1351,   446,  1360,  1358,
    1354,  1419, -1509,  1029,  1361,  1332,  1373,   323,  1382,  1399,
    1404,  1400,  1421,  1434,  1455,  1083,  1441,   445,  1456,  1514,
    1537,  1030,  1044,  1552,  1575,  1557,  1027,  1028,  1601,  1031,
    1484,  1590,   299,  1486,  1487,  1251,  1594,  1616,   447,  1608,
    1623,   302,  1614,  1624,  1631,  1636,   446,  1256,  1648,  1083,
    1396,  1653,  1029,  1687,  1688,  1691,  1424,  1696,  1723,  1711,
    2125,  1630,  1697,  1725,  1740,  1728,  1742,  1332,  1735,  1733,
    1030,  1763,  1745,  1638,   207,  1698,  1528,  1786,  1031,  1788,
    1790,   299,  1794,  1802,  1805,  1806,  1828,   447,  1831,  1832,
    1291,  1843,  1844,  1464,  1465,  1845,  2198,  2124,  1859,  2200,
    1867,  1858,  1869,  1878,  1193,  1193,  1193,  1193,  1193,  1193,
    1193,  1891,  1662,  2176,   208,  1940,  1920,  1964,  1966,  1939,
    1970,  1972,  1466,  1699,  2190,  1977,  2192,  1332,  1467,  1980,
    1998,  1836,  2001,   302,  1984,  2003,  2008,  2011,  2016,  2020,
    2013,  2022,  2024,  2034,  2039,  2038,  2041,  1721,  2072,  2094,
    2090,  1468,  2092,  2112,  2108,  2115,  2110,  2129,  2130,  1469,
    2131,  2149,  2158,  1470,   209,  2151,  2170,  2028,  2177,  1083,
    2178,  1700,  2182,  2188,  2189,  2050,  2210,  2235,  2212,  2214,
    2245,  1016,  2272,  2220,   871,  2262,  2226,  2227,  2274,  2276,
      15,    28,   437,  1471,  1472,  1701,    74,   275,   193,  1473,
     262,   272,  1618,  2242,   815,  1213,  2222,   599,   296,  1474,
    1365,   521,  1652,  1646,   226,  1366,  1475,  2036,  1164,  1702,
    1987,  1476,  2263,  2198,  1703,   298,   826,   151,   469,   153,
      67,  1751,  1752,   245,  1704,   321,   704,  1010,  1705,  1477,
     311, -1474,   406,  1380,  2053,  2054,   992,  2055,  2056,   302,
    1889,  1886,  2243,  2244,  1822,  1885,  2247,  1017,   302,  2199,
    1005,  2057,  2058,  2249,  2059,   628,  2060,   914,  1178,  2264,
    2280,  2281,  -890,  1019,  2283,  -890,  2284,  2273,  -890,  -890,
    -890,  2193,   170,  1930,  1990,  1266,  1716,   589,  1715,  2270,
    1928,  1900,  1275,  1953,  1559,  1555,  1706,  1332,  1332,  1960,
    1797,  2114,  1574,  1083,  1083,  2253,  1707,  1979,  1807,  2289,
    1991,  1311,  1586,  1833,   393,  1645,  1615,  2007,  1334,  1136,
    2282,  1834,  1349,  1335,  -890,  1865,  1894,  2156,  1048,  1332,
    1866,  2172,  2000,   472,  1765,  2224,  1326,  2161,  1516,  1849,
    1050,  1197,   322,   999,  1855,  1048,  1458,  2287,  -890,  1864,
    1864,  1588,   418,  1282,   832,  2265,   973,    76,     0,  1020,
       0,  1744,  1201,     0,  -890,     0,     0,     0,     0,     0,
     441,  1205,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   472,     0,     0,     0,  1478,     0,  1479,  1480,  1481,
    1482,     0,  1483,     0,  1484,  1485,     0,  1486,  1487,     0,
    1021,     0,     0,     0,     0,     0,     0,  1022, -1474,     0,
       0,   442,     0,  1023,     0,     0,  1024,     0,   443,     0,
       0,     0,  -890,  -890,     0,  1251,  1251,  1251,     0,     0,
       0,     0,     0,     0,     0,  1193,     0,   472,     0,     0,
    1193,     0,     0,     0,     0,     0,     0,     0,     0,  1025,
    -890,  -890,     0,  1083,     0,     0,  1016,  -890,     0,   871,
       0,  -890,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0, -1474,     0,     0,     0,
    -890,     0,     0,     0,     0,     0,     0,     0,  -890,     0,
     444,  -890,  -890,     0,     0,     0,     0,     0,  -890,     0,
       0,     0,  -890,     0,  -890,     0,     0,  -890,     0,  -890,
       0,     0,  1963,     0,     0,     0,     0,  1026,  1974,  1332,
       0,     0,  -890,  -890,     0,     0,     0,  1332,  -890,     0,
       0,     0,  1017,     0,     0,     0,  1083,     0,  -890,     0,
    -890,     0,     0,     0,  1601,  -890,     0,     0,  1019,     0,
       0,     0,     0,   445,     0,     0,     0,     0,     0,     0,
       0,     0,  1027,  1028,     0,     0,     0,     0,  -890,     0,
       0,  1974,     0,     0,     0,  1332,     0,     0,     0,     0,
       0,  -890,   446,     0,     0,     0,     0,     0,  1029,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1619,
       0,     0,     0,     0,     0,     0,  1030,     0,  -890,  2079,
    2079,     0,     0,     0,  1031,     0,     0,   299,     0,     0,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1020,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -890,   441,     0,     0,     0,     0,
       0,     0,     0,  -890,  -890,     0,     0,     0,     0,     0,
    2079,     0,     0,  2079,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -890,     0,  1021,     0,     0,     0,  -890,
       0,     0,  1022,     0,     0,     0,   442,     0,  1023,     0,
    1974,  1024,     0,   443,  -361,   629,     0,  -890,     0,     0,
       0,     0,     0,     0,     0,  -890,     0,     0,  -890,     0,
       0,     0,     0,     0,  -890,   630,  -890,  -890,     0,     0,
       0,  -890,  1091,  -890,  1025,  -890,  -890,  -890,   631,  2153,
       0,   632,   633,   634,   635,   636,   637,   638,     0,     0,
       0,     0,     0,     0,     0,     0,  1017,     0,  2079,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1019,     0,     0,   444,     0,     0,   639,     0,
     640,   641,   642,   643,   644,   645,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1026,     0,     0,     0,  2079,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   646,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   445,  -361,
       0,     0,     0,  -361,     0,     0,     0,  1027,  1028,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1020,     0,
       0,     0,     0,     0,  1620,  1291,     0,   446,  1621,   441,
       0, -1496,  -361,  1029,     0,     0,     0,     0,     0,     0,
    2232,     0,     0,  2234,     0,     0,     0,  -361,     0,     0,
       0,  1030, -1496,     0,     0,     0,     0,     0,     0,  1031,
       0,     0,   299,     0,     0,   647,     0,     0,   447,     0,
     442,     0,  1023,  2251,     0,  1024,     0,   443,    41,     0,
       0,   648,     0,     0,     0,     0,     0, -1533,     0,     0,
       0,  -361,  -361,     0,     0,     0,     0,     0,     0,  -361,
     649,     0,     0,  -361,     0,  1091,     0,     0,  1025,     0,
       0,     0,     0,     0,     0,     0,  2279,   650,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -582,   590,     0,  -622,     0,  -622,     0,     0,
     651,     0,  -622,     0,     0,     0,     0,     0,     0,   444,
    -622,     0,     0,     0,     0,   652,     0,     0,  1291,     0,
       0,     0,   653,     0,   654,     0,     0,     0,     0,     0,
       0,  -361,     0,     0,     0,  -361,  1026,   655,     0,     0,
       0,     0,     0,  -622,  -622,     0,     0,   656,     0,     0,
       0,  -622,     0,     0,   657,     0,  -361,     0,     0,     0,
       0,     0,     0,  -622,     0,     0,     0,  -622,     0,     0,
       0,     0,   445,     0,     0,  -361,     0,     0,  -361,  -622,
       0,  1027,  1028,     0,     0,     0,     0,     0,     0,     0,
       0,   658,     0,   659,   660,   661,     0,     0,     0,     0,
       0,   446,     0,  -622,     0,     0,     0,  1029,     0,     0,
    -622,  -622,     0,     0,     0,     0,     0,   662,     0,     0,
       0,     0,     0,     0,     0,  1030,     0,  -582,     0,     0,
       0,  -582,     0,  1031,     0, -1533,   299,     0,     0,     0,
       0,  -622,   447,   663,   664,   665,     0,     0,     0,     0,
       0,     0,     0,  -622,     0,     0,   666,  -622,     0,   667,
     668,     0,     0,     0,     0,     0,     0,     0,  -361,     0,
       0,  -622,     0,  -361,     0,  -582,     0,     0,  -622,     0,
    -361,  -622,  -622,     0,     0,     0,     0,     0,     0,  -622,
       0,     0,     0,     0,     0,     0,  -622,     0,  -622,     0,
       0,  -622,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -622,     0,
       0,     0,  -622,     0,  -622,     0,     0,     0,     0,     0,
       0,  -622,     0,     0,     0,   590,     0,  -622,     0,  -622,
       0,     0,     0,     0,  -622,     0,     0,     0,     0,     0,
       0,     0,  -622,     0,  -622,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -622,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -582,     0,  -622,  -622,     0,     0,     0,
    -622,     0,     0,  -622,     0,     0,     0,     0,     0,  -622,
       0,     0,     0,     0,     0,  -622,     0,     0,     0,  -622,
       0,     0,  -622,     0,     0,     0,     0,  -622,     0,  -622,
       0,  -622,     0,     0,     0,     0,     0,  -622,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -622,     0,     0,
       0,     0,     0,     0,     0,  -622,  -622,     0,     0,     0,
       0,     0,  -622,  -622,     0,  -622,     0,  -622,  -622,     0,
     705,  -622,     0,  -622,     0,     0,  1016,     0,  -622,   871,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -622,  -622,     0,     0,     0,     0,  -622,
       0,     0,     0,     0,  -622,  -622,     0,     0,     0,  -622,
       0,     0,     0,     0,     0,  -622,     0,     0,     0,     0,
       0,     0,     0,  -622,   591,     0,     0,  -622,     0,     0,
    -622,  -622,     0,  -622,  -622,     0,     0,     0,  -622,     0,
       0,  -622,     0,     0,     0,     0,     0,     0,  -622,     0,
    -622,     0,  1017,  -622,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0, -1466,     0,     0, -1466,  1019,     0,
   -1466, -1466, -1466,     0,     0,  1276,     0,     0,     0, -1466,
       0,     0,  1140,     0,     0,     0,     0,     0,     0,     0,
    -622,     0,     0,     0,  -622,     0,  -622,     0,     0,     0,
       0,     0,     0,  -622,     0,     0,     0,     0,     0,   632,
     633,   634,   635,   636,   637,     0, -1466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -622,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   -1466,  -622,     0,     0,     0,     0,   639,     0,   640,   641,
     642,   643,   644,   645,  1020,     0, -1466,     0,     0,     0,
    1016,     0,  -622,   871,     0,   441,  1493,  1494,  1495,     0,
       0,  -622,     0,     0,     0,     0,     0,     0,     0,     0,
    1348,     0,     0,     0,  -622,     0,     0,     0,     0,  -622,
     646,  -622,     0,     0,     0,  1021,     0,     0,     0,  -622,
       0,     0,  1022,     0,     0,     0,   442,     0,  1023,  -622,
       0,  1024,  1496,   443, -1466, -1466,     0,     0,  -622,     0,
       0,     0,     0,     0,     0,     0,     0,  -622,     0,  -622,
    -622,     0,     0,  -622,     0,  -622,  1017,     0,     0,     0,
    -622,     0, -1466, -1466,  1025,     0,     0,     0,     0, -1466,
       0,     0,  1019, -1466,     0,     0,  -622,     0,     0,     0,
       0,  -622,     0,     0,     0,     0,  -622,     0,     0,     0,
       0,     0, -1466,   647,     0,     0,     0,  -622,     0,     0,
   -1466,     0,     0, -1466, -1466,   444,   591,     0,     0,  -622,
   -1466,     0,     0,  -622, -1466,     0, -1466,     0,     0, -1466,
    -622, -1466,     0,     0,     0,     0,     0,     0,     0,     0,
    1464,  1465,  1026,     0, -1466, -1466,     0,     0,     0,     0,
   -1466,     0,     0,     0,     0,   650,     0,     0,     0,     0,
   -1466,     0, -1466,     0,     0,     0,     0, -1466,  1020,  1466,
       0,     0,     0,     0,     0,  1467,     0,     0,   445,   441,
       0,     0,     0,     0,     0,     0,     0,  1027,  1028,     0,
   -1466,     0,     0,   652,     0,     0,     0,     0,  1497,     0,
       0,     0,   654, -1466,     0,     0,  1469,   446,     0,  1021,
    1470,     0,     0,  1029,     0,   655,  1022,     0,     0,     0,
     442,     0,  1023,     0,     0,  1024,     0,   443,     0,     0,
   -1466,  1030,     0,     0,     0,     0,     0,     0,     0,  1031,
    1498,  1499,   299,     0,     0,     0,  1500,     0,   447,     0,
       0,     0,     0,     0,     0,     0,  1501,     0,  1025,     0,
       0,     0,     0,  1502,     0,     0, -1466,     0,     0,     0,
       0,   659,   660,   661,     0, -1466, -1466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1503,     0,     0,     0,
       0,     0, -1466,     0,     0, -1466, -1466,     0,     0,   444,
       0, -1466,   708,     0,   709,     0,     0,     0,     0,   710,
       0,     0,     0,     0,     0,     0,     0,   711,     0, -1466,
       0,   663,   664,   665,     0,     0,  1026, -1466,     0,     0,
   -1466,     0,     0,     0,     0,     0, -1466,     0, -1466, -1466,
       0,     0,     0, -1466,     0, -1466,     0, -1466, -1466, -1466,
     712,   713,     0,     0,     0,     0,     0,     0,   714,     0,
    1017,     0,   445,     0,     0,     0,     0,     0,     0,     0,
     715,  1027,  1028,     0,   716,     0,  1019,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   717,     0,     0,     0,
       0,   446,     0,     0,     0,     0,     0,  1029,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     718,     0,     0,     0,     0,  1030,     0,   719,   720,     0,
       0,     0,     0,  1031,     0,     0,   299,     0,     0,     0,
       0,     0,   447,     0,  1504,  1505,     0,     0,     0,  1506,
       0,  1484,     0,  1507,  1486,  1487,     0,     0,   721,     0,
       0,     0,     0,     0,     0,  1104,     0,     0,     0,     0,
     722,     0,  1020,     0,   723,     0,     0,     0,     0,     0,
    -755,     0,     0,  -755,     0,   708,     0,   709,   724,     0,
       0,     0,   710,     0,     0,   725,     0,     0,   726,   727,
     711,     0,     0,     0,     0,     0,   728,     0,     0,     0,
       0,     0,     0,   729,     0,   730,     0,     0,   731,     0,
       0,     0,     0,   165,  1105,     0,  1023,     0,     0,  1024,
       0,     0,     0,   712,   713,     0,  1016,     0,     0,   871,
       0,   714,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   715,     0,   732,  -755,   716,     0,   733,
   -1478,   734,  1025,     0,     0,     0,     0,     0,   735,   717,
       0,     0,  -755,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   736,     0,   718,     0,     0,     0,     0,     0,     0,
     719,   720,     0,     0,     0,     0,   737,     0,     0,     0,
       0,     0,  1017,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1018,     0,     0,   738,  1019,     0,
    1026,   721,     0,     0,     0,     0,   739,     0,     0,     0,
       0,     0,     0,   722,     0,     0,     0,   723,     0,   740,
       0,     0,     0,     0,   741,     0,   742,     0,  -755,     0,
       0,   724,     0,     0,   743,     0,     0,     0,   725,  -755,
       0,   726,   727,     0,   744,  1027,  1028,     0,     0,   728,
       0,     0,     0,   745,     0, -1552,   729,     0,   730,     0,
       0,   731,   746,     0,   747,   748,     0,     0,   749,  -755,
     750,  1029,     0,     0,     0,   751,  -755,     0,     0,     0,
    -755,  -998,  -755,     0,  1020,  -755,     0,  -755,     0,  1030,
       0,   752,     0,     0,  -998,   441,   753,  1031,   732,   190,
     299,   754,   733,     0,   734,     0,  1016,     0,     0,   871,
       0,   735,   755,     0,     0,     0,     0,     0,  -755,     0,
       0,     0,     0,  -755,   756,  1021,     0,  1016,   757,     0,
     871,     0,  1022,     0,   736,   758,   442,     0,  1023,     0,
       0,  1024,     0,   443,     0,     0,     0,     0,     0,   737,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -755,
       0,     0,     0,     0,     0,     0,     0,     0,     0, -1478,
     738,     0,     0,     0,  1025,     0,     0,     0,     0,   739,
       0,     0,  1017,     0,     0,     0,  -755,     0,     0,     0,
       0,     0,   740,     0,  1098,     0,     0,   741,  1019,   742,
       0,     0,     0,  1017,     0,     0,     0,   743,     0,     0,
       0,     0,     0,     0,     0,   444,  -755,   744,  1016,  1019,
       0,   871,  -755,     0,     0,     0,   745,     0,     0,     0,
       0,  -755,  -755,  1140,     0,   746,     0,   747,   748,     0,
       0,   749,  1026,   750,     0,     0,     0,     0,   751,     0,
       0,  -755,     0,     0,     0,     0,     0,  -755,     0,     0,
       0,     0,  -755,     0,   752,     0,     0,     0,     0,   753,
       0,     0,     0,     0,   754,  -755,     0,     0,   445, -1478,
       0,     0,     0,  -755,  1020,   755,  -755,  1027,  1028,     0,
       0,     0,  -755,     0,  1017,   441,     0,   756,     0,     0,
       0,   757,     0,     0,     0,  1020,  1144,   446,     0,     0,
    1019,     0,     0,  1029,  1016,     0,   441,   871,     0,     0,
       0,     0,     0,     0,     0,  1021,     0,     0,     0,     0,
       0,  1030,  1022,     0,     0,     0,   442,     0,  1023,  1031,
       0,  1024,   299,   443,     0,     0,  1021,     0,   447,     0,
       0,     0,     0,  1022,     0,     0,     0,   442,     0,  1023,
       0,     0,  1024,     0,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1025,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1017,     0,     0,     0,     0,  1025,  1020,     0,  1016,     0,
       0,   871,     0,     0,     0,     0,  1019,   441,     0,     0,
       0,     0,     0,     0,     0,   444,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1016,     0,     0,   871,
       0,     0,     0,     0,     0,     0,   444,  1021,     0,     0,
       0,     0,  1026,     0,  1022,     0,     0,     0,   442,     0,
    1023,     0,     0,  1024,     0,   443,     0,     0,     0,     0,
       0,     0,     0,  1026,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1017,     0,     0,     0,   445,     0,
       0,     0,     0,     0,     0,     0,  1025,  1027,  1028,     0,
    1019,     0,  1020,     0,     0,     0,     0,     0,     0,   445,
       0,  1016,  1017,   441,   871,     0,     0,   446,  1027,  1028,
       0,     0,     0,  1029,     0,     0,     0,     0,  1019,     0,
       0,     0,     0,     0,     0,     0,     0,   444,   446,     0,
       0,  1030,     0,  1021,  1029,     0,     0,     0,     0,  1031,
    1022,     0,   299,     0,   442,     0,  1023,     0,   447,  1024,
       0,   443,  1030,     0,  1026,     0,     0,     0,     0,     0,
    1031,     0,     0,   299,     0,     0,     0,     0,     0,   447,
       0,     0,  1353,     0,     0,     0,  1020,  1017,     0,     0,
       0,     0,  1025,     0,     0,     0,     0,   441,     0,     0,
     445,     0,     0,  1019,     0,     0,     0,     0,     0,  1027,
    1028,     0,     0,     0,  1020,     0,  1016,     0,     0,   871,
       0,     0,     0,     0,     0,   441,     0,  1021,     0,   446,
       0,     0,     0,   444,  1022,  1029,     0,     0,   442,     0,
    1023,     0,     0,  1024,     0,   443,     0,     0,     0,     0,
       0,     0,     0,  1030,     0,  1021,     0,     0,     0,     0,
    1026,  1031,  1022,     0,   299,     0,   442,     0,  1023,     0,
     447,  1024,     0,   443,     0,     0,  1025,     0,  1016,     0,
       0,   871,     0,     0,     0,     0,     0,     0,     0,  1020,
       0,     0,  1017,     0,     0,     0,   445,     0,     0,     0,
     441,     0,     0,     0,  1025,  1027,  1028,     0,  1019,     0,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,  1237,     0,     0,     0,   446,     0,     0,     0,     0,
    1021,  1029,     0,     0,     0,     0,     0,  1022,     0,     0,
       0,   442,     0,  1023,  1026,   444,  1024,     0,   443,  1030,
       0,     0,     0,     0,  1017,     0,     0,  1031,     0,     0,
     299,     0,     0,     0,     0,     0,   447,     0,     0,     0,
    1019,     0,  1026,     0,     0,     0,     0,     0,     0,  1025,
     445,     0,     0,     0,     0,     0,     0,     0,     0,  1027,
    1028,     0,     0,     0,  1020,     0,     0,     0,     0,     0,
       0,     0,  1628,     0,     0,   441,     0,     0,   445,   446,
       0,     0,     0,     0,     0,  1029,     0,  1027,  1028,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1030,     0,  1021,     0,   446,     0,     0,
       0,  1031,  1022,  1029,   299,     0,   442,  1026,  1023,     0,
     447,  1024,     0,   443,     0,     0,  1020,     0,     0,     0,
       0,  1030,     0,     0,     0,     0,     0,   441,     0,  1031,
       0,     0,   299,     0,     0,     0,     0,     0,   447,     0,
       0,     0,     0,   445,  1025,     0,     0,     0,     0,     0,
       0,     0,  1027,  1028,     0,     0,     0,  1021,     0,     0,
       0,     0,     0,     0,  1022,     0,     0,     0,  1062,     0,
    1023,     0,   446,  1024,     0,   443,     0,     0,  1029,     0,
       0,     0,     0,     0,     0,   444,   632,   633,   634,   635,
     636,   637,   638,     0,     0,     0,  1030,     0,     0,     0,
       0,     0,     0,     0,  1031,     0,  1025,   299,     0,     0,
       0,     0,  1026,   447,     0,  2042,  2043,     0,     0,     0,
       0,     0,     0,   639,     0,   640,   641,   642,   643,   644,
     645,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1735,     0,     0,     0,   444,   445,  1017,
       0,     0,     0,     0,     0,     0,     0,  1027,  1028,     0,
       0,     0,     0,     0,     0,  1019,     0,   646,     0,     0,
       0,     0,     0,     0,  1026,     0,     0,   446,     0,     0,
       0,     0,     0,  1029,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1030,     0,     0,     0,     0,     0,  1017,     0,  1031,
     445,     0,   299,     0,     0,     0,     0,     0,   447,  1027,
    1028,     0,     0,  1019,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1017,     0,   446,
       0,  2044,     0,     0,     0,  1029,     0,     0,     0,     0,
     647,  1020,     0,  1019,  1017,     0,     0,     0,     0,     0,
       0,     0,   441,  1030,     0,     0,   648,     0,     0,     0,
    1019,  1031,     0,     0,   299,     0,     0,     0,     0,     0,
     447,     0,  2045,  2046,     0,     0,     0,     0,     0,     0,
       0,     0,  1185,     0,     0,     0,     0,     0,     0,  1022,
       0,     0,   650,   442,     0,  1023,     0,  2047,  1024,  1020,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     441,     0,     0,     0,     0,   651,     0,     0,     0,     0,
       0,     0,  1104,     0,     0,     0,     0,     0,     0,  1020,
     652,  1025,     0,     0,     0,     0,     0,   653,     0,   654,
    1185,     0,  2048,     0,     0,     0,  1020,  1022,     0,     0,
       0,   442,   655,  1023,     0,     0,  1024,   441,   443,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   444,     0,     0,     0,     0,     0,     0,     0,
       0,  1049,     0,  1023,     0,     0,  1024,  1185,     0,  1025,
       0,     0,     0,     0,  1022,     0,     0,     0,   442,  1026,
    1023,     0,     0,  1024,     0,   443,   658,     0,   659,   660,
     661,     0,     0,     0,     0,  2049,     0,     0,     0,  1025,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  2050,
     444,     0,     0,     0,     0,   445,  1025,     0,     0,     0,
       0,     0,     0,     0,  1027,  1028,     0,     0,     0,     0,
       0,     0,     0,  2051,     0,     0,     0,  1026,   663,   664,
     665,     0,     0,     0,   446,     0,     0,     0,     0,     0,
    1029,   666,     0,     0,   667,   668,     0,   444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1026,  1030,     0,
       0,     0,     0,   445,     0,  2052,  1031,     0,     0,   299,
       0,     0,  1027,  1028,  1026,   447,     0,  1186,  1187,     0,
       0,     0,     0,     0,     0,  1679,  1188,     0,     0,     0,
       0,     0,   446,     0,     0,     0,     0,     0,  1029,     0,
       0,     0,  1027,  1028,     0,     0,     0,     0,     0,     0,
     445,     0, -1552,     0,     0,     0,  1030,     0,     0,  1027,
    1028,     0,     0,     0,  1031,     0,     0,   299,  1029,     0,
       0,     0,     0,   447,     0,  1186,  1187,     0,  -998,   446,
       0,     0,     0,  1906,  1188,  1029,  1030,     0,     0,     0,
       0,  -998,     0,     0,  1031,     0,   190,   299,     0,     0,
       0,     0,     0,  1030,     0,     0,     0,     0,     0,     0,
       0,  1031,     0,     0,   299,     0,     0,     0,     0,     0,
     447,     0,  1186,  1187,     0,     0,     0,     0,     0,     0,
       0,  1188
};

static const yytype_int16 yycheck[] =
{
      84,   176,   132,   416,   230,   575,   726,   938,   902,   464,
     539,   540,   327,  1061,   872,   873,  1244,  1277,   656,  1314,
    1133,   196,   197,  1154,  1054,   755,   201,   202,   203,   204,
     205,   690,   207,   692,   209,  1157,   228,   260,   230,   231,
     897,  1244,   234,  1244,  1244,   697,  1140,  1244,  1526,  1424,
    1425,   908,   898,  1721,   491,    22,    17,     6,     6,    56,
      70,     6,  1031,   193,     6,     9,  1544,  1123,    30,   916,
      49,    17,   373,    54,   122,   250,   160,   252,   607,     6,
      70,  1081,   929,     9,   259,    70,    48,    70,    48,  1089,
      23,    96,   267,   285,   896,   231,    70,   289,   234,   291,
      21,  1214,    49,   905,   906,    22,   281,   282,   300,   284,
       9,   303,   121,    85,  1592,   290,    96,    56,   920,   921,
    1268,  1269,  1270,   827,    13,    49,   170,    56,   171,   352,
    1318,   182,   934,   935,    49,  1135,   359,    30,     9,    86,
      49,    73,  1870,   171,  1174,   236,    86,   448,   121,   285,
      28,    70,   198,   289,  1632,   291,   331,  1213,  1016,   334,
     335,  1237,   337,  1021,   300,   214,   269,   303,   394,  1300,
     396,   248,   364,    66,  1208,   163,    23,   236,   353,   121,
     117,    63,   374,    72,   269,    49,  1415,  1855,   223,   381,
     413,     0,   994,  1422,   240,   370,   107,   420,  1274,    88,
     392,   150,   394,   156,   396,   864,   114,     9,   240,  1408,
     433,   118,   174,   236,   260,   118,  1415,    66,  1858,   878,
     224,   522,  1080,  1422,   308,   417,   171,   419,   364,  1305,
    1117,   402,  1034,    28,   457,   322,  1292,   115,   374,   180,
     415,   467,   214,   325,   122,   381,  1177,   470,   423,   372,
      66,  1382,  1374,     8,    56,   180,   368,   236,    28,   918,
    1738,   698,  1064,   441,  1358,   122,   423,   927,   248,   319,
     118,   321,     9,  1404,   466,   467,   468,  1353,   456,   436,
      35,   214,     0,   419,  1131,   945,    92,   180,   459,   312,
     156,   483,  1322,   899,     9,   418,  1098,  1557,  1354,   248,
     523,   289,   477,  1007,  1008,    37,   418,   227,   228,    40,
     236,   236,   918,   397,   988,   347,   400,   923,   541,   406,
     115,   262,   497,   498,   195,   305,   403,  1185,   236,   236,
     466,   284,   468,   236,   251,   223,  1336,  1977,   441,  1141,
     233,   426,   346,  1237,  1146,   115,   210,  1006,   207,   149,
     360,   442,   527,   435,   577,    63,   441,  1009,  1414,   582,
     456,   260,   537,  1001,  1002,   236,   988,   214,   305,  1403,
     360,   173,   243,   565,   183,   360,   411,   360,   325,  1455,
    1274,   113,   988,   442,   233,   325,   360,  1535,   236,   564,
     322,   437,   321,   441,   442,   587,   588,   572,   442,   442,
    1257,   325,   280,   168,  2132,   456,   403,   456,   301,   979,
     325,  1305,  1816,   543,   442,   424,   325,   341,   284,   318,
    1533,   223,   123,  1517,   236,  1282,   341,  1905,   651,   565,
    1561,   606,   202,   350,  1321,  1237,   611,   442,   613,    61,
    1286,   616,  1244,   618,   315,   620,   621,   409,   458,   409,
     625,   424,   301,   434,   400,  1302,  1456,  1645,   195,  1353,
    1689,   403,   442,   442,   436,   657,    88,    89,   458,   448,
    1316,  1273,  1274,   458,   406,   458,   403,   438,  1956,   441,
    1958,   363,   424,   404,   458,   301,   218,   408,   435,   456,
    1689,   666,   667,   442,   442,   435,   441,   442,   442,   236,
     442,  1107,   311,  1305,   443,  1209,   243,   378,  1986,   442,
     310,   435,  1186,  1187,  1188,   442,   442,  1363,   437,   376,
     435,   236,  1196,   411,  1646,   331,   435,   258,  1584,   449,
     441,   442,   456,  1793,   442,   442,   407,   245,  1340,  2017,
    2018,   456,  1344,  1345,  1218,  1347,  1220,  1595,  1222,   251,
    1845,  1353,  1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,
    1616,  1455,    70,   441,  1186,  1187,  1188,   697,  1662,  1973,
     871,   442,  1976,  1010,  1196,   434,   126,   448,   315,  1217,
    1186,  1187,  1188,  2251,   441,   442,  1224,   139,   394,   354,
    1196,  1591,   214,   168,   442,   139,    32,  1399,  1220,   240,
     457,   300,   310,   200,  1226,  1227,  1228,  1229,   236,   411,
    1232,  1233,  1218,   502,  1220,   316,  1222,   166,  1224,   260,
    1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,   188,    30,
     442,   823,   364,   214,   456,   827,   448,   218,   830,   831,
     405,   378,   442,   818,    70,   837,  1284,   236,   350,   236,
     358,   187,     9,  1747,   214,   363,   187,   213,   207,  1558,
     227,   228,    31,   364,   839,  1564,   215,  2042,   891,  1515,
     407,   293,   187,    25,  1326,   187,   988,   240,  1330,   240,
     277,  1401,  1712,  1713,  1714,   240,  2270,   823,   300,   187,
     245,   827,   187,    50,   830,   831,   175,   260,   183,   260,
     426,   837,  1003,  2287,   195,   150,   898,   195,    11,   460,
     266,   448,   120,   263,  1790,  2119,  1792,  1391,  1392,  1393,
    1394,  1395,  1396,  1397,  2022,   126,  2024,   919,   164,  2207,
    2208,   923,    70,   925,   442,   927,   928,  1868,   930,   288,
    1740,   110,   441,  1590,  1464,   236,  1384,   124,   236,  1825,
    1806,  1720,   243,   945,   456,   243,  1725,  1726,   313,   311,
    1729,  1730,  1731,   986,  1733,  1734,   958,   311,    71,  1391,
    1392,  1393,  1394,  1395,  1396,  1397,   128,   528,  1384,   354,
      27,  1857,  2010,   975,   195,  1391,  1392,  1393,  1394,  1395,
    1396,  1397,  1430,   923,  1524,  1943,  1944,  1945,  1946,   991,
     441,   976,   198,   199,  1898,  1899,  1862,  2010,   244,  2010,
    2010,  1123,   348,  2010,   442,  1007,  1008,   348,   195,   903,
     200,  1623,   958,  1823,   315,   236,   311,   315,   102,   441,
     442,     1,   243,   348,   456,   245,   348,   258,   195,   975,
     924,   567,   926,   341,   240,  1565,   341,   931,   403,   368,
     329,   456,   360,   442,  1574,   442,   940,  1951,  1952,   236,
    2158,   442,  1761,  1762,   260,   279,   243,   236,   225,   236,
    1118,  1007,  1008,   187,  1186,  1187,  1188,    47,   255,   236,
     441,   442,   449,   304,  1196,   236,   243,   378,  1787,   456,
     378,   236,    62,   442,  2189,   279,  1790,  1796,  1792,   418,
     310,  1213,   236,   198,   315,   285,  1218,  1155,  1220,   260,
    1222,   206,  1224,   166,  1226,  1227,  1228,  1229,  1230,  1231,
    1232,  1233,  2035,   456,   258,    12,    11,  1119,    15,    16,
     225,  1825,   102,     1,   360,   195,     4,  1794,   315,   251,
      25,    26,   450,   451,   452,   453,     9,   455,   358,    12,
     458,    19,    15,    16,   207,    83,  1179,   448,   315,  1089,
     448,    29,   215,  1857,   126,   299,   380,   378,   120,  1161,
     905,  1055,    57,   245,   288,   202,   236,  1107,  2026,  2274,
    1292,  1619,   349,   243,   279,   280,  1843,   456,   372,   169,
     236,  1166,   287,   288,   356,    63,   407,   167,   293,   294,
     935,   378,   248,   307,   381,   382,   233,   319,   319,   321,
     245,    96,    97,     8,    99,   123,   456,  1209,   188,   368,
    1919,   378,   360,   108,   450,   451,   452,   453,   208,   455,
    1832,   442,   458,   167,   418,   288,   216,   448,   310,  1938,
      35,   437,  1354,   368,   214,   441,   456,   293,  1722,   219,
     407,   213,   456,  1727,   188,   315,  1955,   275,   107,   167,
       9,   231,   211,   374,   375,  1380,   151,   152,   154,   418,
     456,   448,  1384,  1209,  2122,   310,   240,   162,  2009,  1391,
    1392,  1393,  1394,  1395,  1396,  1397,   358,   236,   356,   384,
    1897,   448,   319,   418,  1286,   368,   260,  1996,  1997,   326,
    1722,   263,  1414,  1899,   266,  1727,  2005,    56,  1022,    90,
      91,   120,   450,   451,   452,   453,  1722,   455,   378,   337,
     458,  1727,  1252,   358,  1316,    82,   175,   213,  1930,  1610,
     167,   668,  1587,  1047,   202,   116,    60,   364,  1667,   188,
    1947,    98,   311,  1950,  1625,   418,  1805,   407,    72,   319,
     320,   188,   237,   330,   456,  1951,  1952,   135,   335,   377,
     324,   379,   332,   310,   334,   233,   253,   254,  2216,    93,
     530,  1363,   240,   296,   297,   535,  1368,   101,   264,  1263,
     266,   240,  1374,  1375,   456,   272,   273,   310,   448,  2088,
     253,   254,  2050,   261,   404,  1325,   442,  2096,   408,  1858,
     240,   260,   448,   159,  1924,   456,  1926,   163,   356,   272,
     273,   404,  2111,     9,  2108,   408,  2110,   169,   286,  1218,
     260,   456,  1442,  1222,   292,  1445,   456,   184,  2035,   444,
    1450,  1230,  1231,   356,  1364,   227,   228,     2,  1374,  1375,
    2139,     6,  2141,   278,    33,   280,   195,    36,   319,  2148,
     321,   319,   416,   356,   178,    44,    45,    46,   326,   239,
      56,   450,   451,   452,   453,   435,   455,   352,   442,  2168,
    2169,   339,  1584,  1585,   223,   341,  2083,   441,   166,   236,
     456,   238,   170,   438,   241,   440,   456,   236,   456,   452,
     453,  2222,   455,  1822,   243,   363,   364,   265,   366,   267,
     450,   451,   452,   453,  1616,   455,   374,   392,   153,   604,
     155,   456,   462,   294,   295,   456,   384,   274,  1977,   450,
     451,   452,   453,  1515,   455,   216,   319,  2129,   321,   233,
     234,   462,  2231,   359,    25,    26,   456,   405,   456,   450,
     451,   452,   453,   405,   455,   356,     6,   458,   235,     9,
     427,   428,   429,   430,   335,   336,   456,   442,   895,   270,
     271,   442,   899,   270,   271,   456,   315,   456,   905,   158,
    2090,   456,  2092,   910,   911,   182,    67,   173,    69,   916,
     917,   918,   233,   234,   283,   342,   923,   176,   456,   456,
     280,   280,   929,   427,   428,   429,   430,   403,     9,   195,
     277,   938,   939,   403,   941,   194,   395,    24,  2210,   456,
    1722,  2213,   103,   104,   105,  1727,   356,  2076,   450,   451,
     452,   453,    82,   455,   332,   449,   458,   223,   372,   378,
     387,   388,   214,   456,  1657,   107,   384,   442,    98,   367,
     236,   427,   428,   429,   430,   442,    56,   243,   236,  1624,
      85,   988,   175,   242,  1646,   456,   413,   351,   407,   150,
     356,   152,   411,   372,  1594,   456,   283,  1659,   109,   160,
     236,    82,   211,   456,   431,   249,    68,   363,  1653,   156,
     157,  1018,   439,    11,   410,   442,   280,    98,  1766,  1767,
    1768,  1769,  1684,   442,  1806,   280,   156,    25,    26,   448,
    1630,   290,  1814,   456,   356,   249,    23,  1637,   185,   277,
    1646,   442,   236,   456,   191,   301,   280,  1054,  1693,   315,
     456,  1696,    84,  1659,   184,   450,   451,   452,   453,    57,
     455,    84,  1724,   458,   436,   195,   442,   442,   400,   317,
     384,   234,   435,   234,  1081,   222,   403,   200,  1684,   226,
    1862,   364,  1089,  1090,   343,   403,   319,   248,   450,   451,
     452,   453,   442,   455,   353,   225,   458,   218,    96,    97,
    1107,    99,   232,   184,    55,   441,   236,    27,   238,   384,
     108,   241,   378,   243,   195,   188,  1123,   456,  1724,   372,
     346,   456,   364,   442,  1131,  1770,   356,   225,  1135,   438,
     214,  1776,   109,   236,   405,   306,    37,  1144,   381,   287,
    1147,   407,   281,   448,   274,   411,  2186,   459,    27,   403,
     180,   400,   362,   151,   152,   236,   415,   238,   174,   463,
     241,   400,   243,  2046,   162,   456,   456,  1174,   442,     9,
    1177,   236,   136,  1818,   405,   107,   442,   172,   441,  1186,
    1187,  1188,   448,    17,  1877,   315,   188,   402,   236,  1196,
     459,   127,    31,   274,    33,    30,   205,    36,   459,   459,
     450,   451,   452,   453,   128,   455,  1213,    46,   458,    56,
     348,  1218,   342,  1220,   459,  1222,   403,   434,   459,  1226,
    1227,  1228,  1229,  1230,  1231,  1232,  1233,  1827,   130,    12,
      13,    14,   459,  1833,   315,   459,   459,    20,   459,   237,
     401,   459,  1887,   459,   319,   132,  1891,   459,   378,   133,
     100,   450,   451,   452,   453,   399,   455,   387,   388,   458,
     403,   342,   137,    49,   138,   402,   400,   402,   397,   180,
     143,   436,   248,  1827,    49,    58,   146,   407,   333,   112,
     180,   442,   442,   413,   380,  1292,   122,   227,   114,    49,
     114,   365,   441,   403,   187,  1302,   442,   378,   341,   309,
     163,   431,   236,   134,   171,   438,   387,   388,  1315,   439,
     457,   403,   442,   460,   461,  1322,   366,   403,   448,   210,
      49,  1983,  1329,   218,   180,   210,   407,   457,   280,  1336,
     455,   193,   413,   447,   446,   218,   233,   176,   458,   270,
    1985,  1348,   181,   458,   341,   458,   398,  1354,   373,   458,
     431,   131,   403,  1360,   352,   194,   266,   269,   439,   265,
     187,   442,   400,   331,   403,    49,   138,   448,     8,   180,
    1377,   438,   126,   156,   157,   438,  2069,  1983,     9,  2075,
     307,   403,   308,   173,  1391,  1392,  1393,  1394,  1395,  1396,
    1397,   404,   202,  2038,   392,   344,   268,   125,   435,   329,
     110,   435,   185,   242,  2049,    49,  2051,  1414,   191,   175,
     141,   263,   142,  2075,   285,   240,   262,   144,   300,   284,
     289,   113,   394,     7,   220,   116,    67,  1434,   442,   129,
     156,   214,   156,   136,   328,   102,   328,    49,   240,   222,
     218,   145,    95,   226,   442,   267,   148,   264,    91,  1456,
     221,   290,   203,   192,   438,   390,    49,   147,   406,   307,
     174,     6,   307,   344,     9,   293,   240,   240,   423,   140,
       6,    22,   358,   256,   257,   314,    54,   193,   132,   262,
     178,   188,  1337,  2176,   601,    30,  2131,   518,   211,   272,
    1156,   448,  1375,  1366,   136,  1157,   279,  1873,   964,   338,
    1826,   284,  2195,  2196,   343,   227,   614,   109,   397,   113,
      49,  1518,  1519,   160,   353,   247,   588,   890,   357,   302,
     239,    66,   322,  1180,  1881,  1881,   863,  1881,  1881,  2191,
    1668,  1666,  2177,  2178,  1589,  1664,  2181,    82,  2200,  2073,
     878,  1881,  1881,  2188,  1881,   569,  1881,     1,   978,  2196,
    2243,  2244,     6,    98,  2247,     9,  2249,  2219,    12,    13,
      14,  2065,   124,  1749,  1830,  1064,  1427,   500,  1426,  2214,
    1747,  1708,  1069,  1783,  1282,  1279,   415,  1584,  1585,  1793,
    1557,  1967,  1289,  1590,  1591,  2191,   425,  1814,  1582,  2282,
    1831,  1107,  1295,  1606,  2200,  1364,  1330,  1847,  1129,   930,
    2245,  1608,  1141,  1131,    58,  1637,  1686,  2020,  2270,  1616,
    1637,  2031,  1838,  2167,  1531,  2132,  1119,  2025,  1246,  1619,
     899,   989,   247,   870,  1631,  2287,  1242,  2272,    82,  1636,
    1637,  1300,   333,  1072,   623,  2202,   834,    57,    -1,   184,
      -1,  1470,   997,    -1,    98,    -1,    -1,    -1,    -1,    -1,
     195,  1004,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2215,    -1,    -1,    -1,   448,    -1,   450,   451,   452,
     453,    -1,   455,    -1,   457,   458,    -1,   460,   461,    -1,
     225,    -1,    -1,    -1,    -1,    -1,    -1,   232,   233,    -1,
      -1,   236,    -1,   238,    -1,    -1,   241,    -1,   243,    -1,
      -1,    -1,   156,   157,    -1,  1712,  1713,  1714,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1722,    -1,  2271,    -1,    -1,
    1727,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   274,
     184,   185,    -1,  1740,    -1,    -1,     6,   191,    -1,     9,
      -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   301,    -1,    -1,    -1,
     214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   222,    -1,
     315,   225,   226,    -1,    -1,    -1,    -1,    -1,   232,    -1,
      -1,    -1,   236,    -1,   238,    -1,    -1,   241,    -1,   243,
      -1,    -1,  1799,    -1,    -1,    -1,    -1,   342,  1805,  1806,
      -1,    -1,   256,   257,    -1,    -1,    -1,  1814,   262,    -1,
      -1,    -1,    82,    -1,    -1,    -1,  1823,    -1,   272,    -1,
     274,    -1,    -1,    -1,  1831,   279,    -1,    -1,    98,    -1,
      -1,    -1,    -1,   378,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   387,   388,    -1,    -1,    -1,    -1,   302,    -1,
      -1,  1858,    -1,    -1,    -1,  1862,    -1,    -1,    -1,    -1,
      -1,   315,   407,    -1,    -1,    -1,    -1,    -1,   413,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,   431,    -1,   342,  1896,
    1897,    -1,    -1,    -1,   439,    -1,    -1,   442,    -1,    -1,
      -1,    -1,    -1,   448,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   378,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   387,   388,    -1,    -1,    -1,    -1,    -1,
    1947,    -1,    -1,  1950,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   407,    -1,   225,    -1,    -1,    -1,   413,
      -1,    -1,   232,    -1,    -1,    -1,   236,    -1,   238,    -1,
    1977,   241,    -1,   243,     0,     1,    -1,   431,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   439,    -1,    -1,   442,    -1,
      -1,    -1,    -1,    -1,   448,    21,   450,   451,    -1,    -1,
      -1,   455,  2009,   457,   274,   459,   460,   461,    34,  2016,
      -1,    37,    38,    39,    40,    41,    42,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,  2035,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,    -1,   315,    -1,    -1,    74,    -1,
      76,    77,    78,    79,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   342,    -1,    -1,    -1,  2083,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   378,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,   387,   388,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
      -1,    -1,    -1,    -1,   404,  2142,    -1,   407,   408,   195,
      -1,   167,   168,   413,    -1,    -1,    -1,    -1,    -1,    -1,
    2157,    -1,    -1,  2160,    -1,    -1,    -1,   183,    -1,    -1,
      -1,   431,   188,    -1,    -1,    -1,    -1,    -1,    -1,   439,
      -1,    -1,   442,    -1,    -1,   201,    -1,    -1,   448,    -1,
     236,    -1,   238,  2190,    -1,   241,    -1,   243,   214,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,
      -1,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,    -1,    -1,   239,    -1,  2222,    -1,    -1,   274,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2233,   253,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     0,     1,    -1,     3,    -1,     5,    -1,    -1,
     276,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,   315,
      18,    -1,    -1,    -1,    -1,   291,    -1,    -1,  2275,    -1,
      -1,    -1,   298,    -1,   300,    -1,    -1,    -1,    -1,    -1,
      -1,   307,    -1,    -1,    -1,   311,   342,   313,    -1,    -1,
      -1,    -1,    -1,    51,    52,    -1,    -1,   323,    -1,    -1,
      -1,    59,    -1,    -1,   330,    -1,   332,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    -1,   378,    -1,    -1,   351,    -1,    -1,   354,    87,
      -1,   387,   388,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   367,    -1,   369,   370,   371,    -1,    -1,    -1,    -1,
      -1,   407,    -1,   111,    -1,    -1,    -1,   413,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,   393,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   431,    -1,   135,    -1,    -1,
      -1,   139,    -1,   439,    -1,   411,   442,    -1,    -1,    -1,
      -1,   149,   448,   419,   420,   421,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,   432,   165,    -1,   435,
     436,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   444,    -1,
      -1,   179,    -1,   449,    -1,   183,    -1,    -1,   186,    -1,
     456,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,   197,
      -1,    -1,    -1,    -1,    -1,    -1,   204,    -1,   206,    -1,
      -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   246,    -1,
      -1,    -1,   250,    -1,   252,    -1,    -1,    -1,    -1,    -1,
      -1,   259,    -1,    -1,    -1,     1,    -1,     3,    -1,     5,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,   282,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   297,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   311,    -1,    51,    52,    -1,    -1,    -1,
     318,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,   327,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    75,
      -1,    -1,   340,    -1,    -1,    -1,    -1,   345,    -1,   347,
      -1,    87,    -1,    -1,    -1,    -1,    -1,   355,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   365,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   374,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,   383,    -1,   385,   386,    -1,
     126,   389,    -1,   391,    -1,    -1,     6,    -1,   396,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   412,    -1,    -1,    -1,    -1,   417,
      -1,    -1,    -1,    -1,   422,   161,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   433,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   179,   442,    -1,    -1,   445,    -1,    -1,
     186,   449,    -1,   189,   190,    -1,    -1,    -1,   456,    -1,
      -1,   197,    -1,    -1,    -1,    -1,    -1,    -1,   204,    -1,
     206,    -1,    82,   209,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     6,    -1,    -1,     9,    98,    -1,
      12,    13,    14,    -1,    -1,    17,    -1,    -1,    -1,    21,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     246,    -1,    -1,    -1,   250,    -1,   252,    -1,    -1,    -1,
      -1,    -1,    -1,   259,    -1,    -1,    -1,    -1,    -1,    37,
      38,    39,    40,    41,    42,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   282,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,   297,    -1,    -1,    -1,    -1,    74,    -1,    76,    77,
      78,    79,    80,    81,   184,    -1,    98,    -1,    -1,    -1,
       6,    -1,   318,     9,    -1,   195,    12,    13,    14,    -1,
      -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     210,    -1,    -1,    -1,   340,    -1,    -1,    -1,    -1,   345,
     118,   347,    -1,    -1,    -1,   225,    -1,    -1,    -1,   355,
      -1,    -1,   232,    -1,    -1,    -1,   236,    -1,   238,   365,
      -1,   241,    58,   243,   156,   157,    -1,    -1,   374,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   383,    -1,   385,
     386,    -1,    -1,   389,    -1,   391,    82,    -1,    -1,    -1,
     396,    -1,   184,   185,   274,    -1,    -1,    -1,    -1,   191,
      -1,    -1,    98,   195,    -1,    -1,   412,    -1,    -1,    -1,
      -1,   417,    -1,    -1,    -1,    -1,   422,    -1,    -1,    -1,
      -1,    -1,   214,   201,    -1,    -1,    -1,   433,    -1,    -1,
     222,    -1,    -1,   225,   226,   315,   442,    -1,    -1,   445,
     232,    -1,    -1,   449,   236,    -1,   238,    -1,    -1,   241,
     456,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,   157,   342,    -1,   256,   257,    -1,    -1,    -1,    -1,
     262,    -1,    -1,    -1,    -1,   253,    -1,    -1,    -1,    -1,
     272,    -1,   274,    -1,    -1,    -1,    -1,   279,   184,   185,
      -1,    -1,    -1,    -1,    -1,   191,    -1,    -1,   378,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,   388,    -1,
     302,    -1,    -1,   291,    -1,    -1,    -1,    -1,   214,    -1,
      -1,    -1,   300,   315,    -1,    -1,   222,   407,    -1,   225,
     226,    -1,    -1,   413,    -1,   313,   232,    -1,    -1,    -1,
     236,    -1,   238,    -1,    -1,   241,    -1,   243,    -1,    -1,
     342,   431,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   439,
     256,   257,   442,    -1,    -1,    -1,   262,    -1,   448,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   272,    -1,   274,    -1,
      -1,    -1,    -1,   279,    -1,    -1,   378,    -1,    -1,    -1,
      -1,   369,   370,   371,    -1,   387,   388,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   302,    -1,    -1,    -1,
      -1,    -1,   404,    -1,    -1,   407,   408,    -1,    -1,   315,
      -1,   413,     3,    -1,     5,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,   431,
      -1,   419,   420,   421,    -1,    -1,   342,   439,    -1,    -1,
     442,    -1,    -1,    -1,    -1,    -1,   448,    -1,   450,   451,
      -1,    -1,    -1,   455,    -1,   457,    -1,   459,   460,   461,
      51,    52,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      82,    -1,   378,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,   387,   388,    -1,    75,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,   407,    -1,    -1,    -1,    -1,    -1,   413,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,   431,    -1,   118,   119,    -1,
      -1,    -1,    -1,   439,    -1,    -1,   442,    -1,    -1,    -1,
      -1,    -1,   448,    -1,   450,   451,    -1,    -1,    -1,   455,
      -1,   457,    -1,   459,   460,   461,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
     161,    -1,   184,    -1,   165,    -1,    -1,    -1,    -1,    -1,
       6,    -1,    -1,     9,    -1,     3,    -1,     5,   179,    -1,
      -1,    -1,    10,    -1,    -1,   186,    -1,    -1,   189,   190,
      18,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,
      -1,    -1,    -1,   204,    -1,   206,    -1,    -1,   209,    -1,
      -1,    -1,    -1,    49,   236,    -1,   238,    -1,    -1,   241,
      -1,    -1,    -1,    51,    52,    -1,     6,    -1,    -1,     9,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    -1,   246,    82,    75,    -1,   250,
      86,   252,   274,    -1,    -1,    -1,    -1,    -1,   259,    87,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   282,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,   297,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,   318,    98,    -1,
     342,   149,    -1,    -1,    -1,    -1,   327,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,    -1,   165,    -1,   340,
      -1,    -1,    -1,    -1,   345,    -1,   347,    -1,   184,    -1,
      -1,   179,    -1,    -1,   355,    -1,    -1,    -1,   186,   195,
      -1,   189,   190,    -1,   365,   387,   388,    -1,    -1,   197,
      -1,    -1,    -1,   374,    -1,   397,   204,    -1,   206,    -1,
      -1,   209,   383,    -1,   385,   386,    -1,    -1,   389,   225,
     391,   413,    -1,    -1,    -1,   396,   232,    -1,    -1,    -1,
     236,   423,   238,    -1,   184,   241,    -1,   243,    -1,   431,
      -1,   412,    -1,    -1,   436,   195,   417,   439,   246,   441,
     442,   422,   250,    -1,   252,    -1,     6,    -1,    -1,     9,
      -1,   259,   433,    -1,    -1,    -1,    -1,    -1,   274,    -1,
      -1,    -1,    -1,   279,   445,   225,    -1,     6,   449,    -1,
       9,    -1,   232,    -1,   282,   456,   236,    -1,   238,    -1,
      -1,   241,    -1,   243,    -1,    -1,    -1,    -1,    -1,   297,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   325,
     318,    -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,   327,
      -1,    -1,    82,    -1,    -1,    -1,   342,    -1,    -1,    -1,
      -1,    -1,   340,    -1,    94,    -1,    -1,   345,    98,   347,
      -1,    -1,    -1,    82,    -1,    -1,    -1,   355,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   315,   372,   365,     6,    98,
      -1,     9,   378,    -1,    -1,    -1,   374,    -1,    -1,    -1,
      -1,   387,   388,   112,    -1,   383,    -1,   385,   386,    -1,
      -1,   389,   342,   391,    -1,    -1,    -1,    -1,   396,    -1,
      -1,   407,    -1,    -1,    -1,    -1,    -1,   413,    -1,    -1,
      -1,    -1,   418,    -1,   412,    -1,    -1,    -1,    -1,   417,
      -1,    -1,    -1,    -1,   422,   431,    -1,    -1,   378,   435,
      -1,    -1,    -1,   439,   184,   433,   442,   387,   388,    -1,
      -1,    -1,   448,    -1,    82,   195,    -1,   445,    -1,    -1,
      -1,   449,    -1,    -1,    -1,   184,    94,   407,    -1,    -1,
      98,    -1,    -1,   413,     6,    -1,   195,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   225,    -1,    -1,    -1,    -1,
      -1,   431,   232,    -1,    -1,    -1,   236,    -1,   238,   439,
      -1,   241,   442,   243,    -1,    -1,   225,    -1,   448,    -1,
      -1,    -1,    -1,   232,    -1,    -1,    -1,   236,    -1,   238,
      -1,    -1,   241,    -1,   243,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    -1,    -1,   274,   184,    -1,     6,    -1,
      -1,     9,    -1,    -1,    -1,    -1,    98,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   315,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     6,    -1,    -1,     9,
      -1,    -1,    -1,    -1,    -1,    -1,   315,   225,    -1,    -1,
      -1,    -1,   342,    -1,   232,    -1,    -1,    -1,   236,    -1,
     238,    -1,    -1,   241,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   342,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,   378,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   274,   387,   388,    -1,
      98,    -1,   184,    -1,    -1,    -1,    -1,    -1,    -1,   378,
      -1,     6,    82,   195,     9,    -1,    -1,   407,   387,   388,
      -1,    -1,    -1,   413,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   407,    -1,
      -1,   431,    -1,   225,   413,    -1,    -1,    -1,    -1,   439,
     232,    -1,   442,    -1,   236,    -1,   238,    -1,   448,   241,
      -1,   243,   431,    -1,   342,    -1,    -1,    -1,    -1,    -1,
     439,    -1,    -1,   442,    -1,    -1,    -1,    -1,    -1,   448,
      -1,    -1,   180,    -1,    -1,    -1,   184,    82,    -1,    -1,
      -1,    -1,   274,    -1,    -1,    -1,    -1,   195,    -1,    -1,
     378,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,   387,
     388,    -1,    -1,    -1,   184,    -1,     6,    -1,    -1,     9,
      -1,    -1,    -1,    -1,    -1,   195,    -1,   225,    -1,   407,
      -1,    -1,    -1,   315,   232,   413,    -1,    -1,   236,    -1,
     238,    -1,    -1,   241,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   431,    -1,   225,    -1,    -1,    -1,    -1,
     342,   439,   232,    -1,   442,    -1,   236,    -1,   238,    -1,
     448,   241,    -1,   243,    -1,    -1,   274,    -1,     6,    -1,
      -1,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,    -1,    82,    -1,    -1,    -1,   378,    -1,    -1,    -1,
     195,    -1,    -1,    -1,   274,   387,   388,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,    -1,    -1,
      -1,   403,    -1,    -1,    -1,   407,    -1,    -1,    -1,    -1,
     225,   413,    -1,    -1,    -1,    -1,    -1,   232,    -1,    -1,
      -1,   236,    -1,   238,   342,   315,   241,    -1,   243,   431,
      -1,    -1,    -1,    -1,    82,    -1,    -1,   439,    -1,    -1,
     442,    -1,    -1,    -1,    -1,    -1,   448,    -1,    -1,    -1,
      98,    -1,   342,    -1,    -1,    -1,    -1,    -1,    -1,   274,
     378,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,
     388,    -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   372,    -1,    -1,   195,    -1,    -1,   378,   407,
      -1,    -1,    -1,    -1,    -1,   413,    -1,   387,   388,    -1,
     315,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   431,    -1,   225,    -1,   407,    -1,    -1,
      -1,   439,   232,   413,   442,    -1,   236,   342,   238,    -1,
     448,   241,    -1,   243,    -1,    -1,   184,    -1,    -1,    -1,
      -1,   431,    -1,    -1,    -1,    -1,    -1,   195,    -1,   439,
      -1,    -1,   442,    -1,    -1,    -1,    -1,    -1,   448,    -1,
      -1,    -1,    -1,   378,   274,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   387,   388,    -1,    -1,    -1,   225,    -1,    -1,
      -1,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,    -1,
     238,    -1,   407,   241,    -1,   243,    -1,    -1,   413,    -1,
      -1,    -1,    -1,    -1,    -1,   315,    37,    38,    39,    40,
      41,    42,    43,    -1,    -1,    -1,   431,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   439,    -1,   274,   442,    -1,    -1,
      -1,    -1,   342,   448,    -1,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    76,    77,    78,    79,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   373,    -1,    -1,    -1,   315,   378,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,   388,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,   342,    -1,    -1,   407,    -1,    -1,
      -1,    -1,    -1,   413,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   431,    -1,    -1,    -1,    -1,    -1,    82,    -1,   439,
     378,    -1,   442,    -1,    -1,    -1,    -1,    -1,   448,   387,
     388,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,   407,
      -1,   192,    -1,    -1,    -1,   413,    -1,    -1,    -1,    -1,
     201,   184,    -1,    98,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   431,    -1,    -1,   217,    -1,    -1,    -1,
      98,   439,    -1,    -1,   442,    -1,    -1,    -1,    -1,    -1,
     448,    -1,   233,   234,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,   232,
      -1,    -1,   253,   236,    -1,   238,    -1,   258,   241,   184,
     243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,    -1,    -1,    -1,    -1,   276,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,   184,
     291,   274,    -1,    -1,    -1,    -1,    -1,   298,    -1,   300,
     225,    -1,   303,    -1,    -1,    -1,   184,   232,    -1,    -1,
      -1,   236,   313,   238,    -1,    -1,   241,   195,   243,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   315,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   236,    -1,   238,    -1,    -1,   241,   225,    -1,   274,
      -1,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   342,
     238,    -1,    -1,   241,    -1,   243,   367,    -1,   369,   370,
     371,    -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,   274,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   390,
     315,    -1,    -1,    -1,    -1,   378,   274,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   387,   388,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   414,    -1,    -1,    -1,   342,   419,   420,
     421,    -1,    -1,    -1,   407,    -1,    -1,    -1,    -1,    -1,
     413,   432,    -1,    -1,   435,   436,    -1,   315,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   342,   431,    -1,
      -1,    -1,    -1,   378,    -1,   456,   439,    -1,    -1,   442,
      -1,    -1,   387,   388,   342,   448,    -1,   450,   451,    -1,
      -1,    -1,    -1,    -1,    -1,   458,   459,    -1,    -1,    -1,
      -1,    -1,   407,    -1,    -1,    -1,    -1,    -1,   413,    -1,
      -1,    -1,   387,   388,    -1,    -1,    -1,    -1,    -1,    -1,
     378,    -1,   397,    -1,    -1,    -1,   431,    -1,    -1,   387,
     388,    -1,    -1,    -1,   439,    -1,    -1,   442,   413,    -1,
      -1,    -1,    -1,   448,    -1,   450,   451,    -1,   423,   407,
      -1,    -1,    -1,   458,   459,   413,   431,    -1,    -1,    -1,
      -1,   436,    -1,    -1,   439,    -1,   441,   442,    -1,    -1,
      -1,    -1,    -1,   431,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   439,    -1,    -1,   442,    -1,    -1,    -1,    -1,    -1,
     448,    -1,   450,   451,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   459
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
     586,   587,   589,   593,   594,   456,   356,   239,   682,   442,
     601,   603,  1070,   733,   732,   341,   739,   485,   473,   456,
     456,   456,   456,   456,   359,   550,   456,   216,   583,    60,
      72,    93,   101,   178,   405,  1136,   319,   374,   375,   588,
     456,   586,   456,   594,   597,   456,   356,   235,   684,   600,
     602,   629,   630,   631,   604,    49,   734,   735,   736,  1131,
     734,   442,   456,   456,   595,   245,   358,   442,   497,   499,
     500,   501,   502,   504,   505,  1152,   182,   507,   508,   509,
     441,   495,   496,   497,  1166,    11,    25,    26,    57,    96,
      97,    99,   108,   151,   152,   162,   237,   352,   392,   442,
     512,   513,   514,   515,   522,   531,   535,   538,   539,   540,
     541,   542,   543,   544,   283,  1117,   549,   456,   280,   280,
    1149,   403,   277,  1148,   403,   395,  1160,    24,  1127,   599,
     628,   456,   356,   332,   686,   603,   449,   632,   633,  1111,
     605,   735,   372,   418,   737,   325,   435,   729,   474,   372,
    1140,   456,   500,   456,   501,    63,   363,  1123,     9,   236,
     510,   456,   509,   456,   456,   496,   107,   442,  1082,  1140,
    1140,  1082,   384,   367,  1156,  1140,  1140,  1140,  1140,  1140,
    1082,  1140,    56,  1133,  1140,   456,   514,  1082,   584,   442,
    1077,  1078,  1097,  1077,  1078,   236,  1078,    85,  1134,   175,
    1137,   600,   629,   683,   456,   356,   351,   726,   629,   456,
     456,   633,  1111,   227,   228,   634,   637,   638,   644,     1,
      47,    62,   102,   219,   231,   319,   320,   332,   334,   435,
     456,   606,   607,   609,   613,   615,   617,   618,   624,   625,
     626,  1140,  1140,   372,   283,   738,   109,   740,   728,  1140,
     236,  1100,   456,   363,  1140,   211,   211,   236,   456,   249,
     523,  1082,  1082,  1140,  1140,  1140,  1078,    68,  1082,  1082,
    1078,  1140,  1078,   532,   533,  1082,    96,   517,  1082,   551,
     200,   277,  1120,  1078,   280,   410,   280,   590,   591,  1070,
    1069,  1070,   629,   685,   456,   356,   634,   171,   442,   640,
     641,   442,   640,  1134,  1140,   319,   321,  1121,  1121,  1140,
    1134,  1140,   249,  1146,  1140,    23,  1126,   277,   167,   188,
      31,   110,  1100,  1140,   442,   456,   730,   478,  1100,  1078,
    1140,   195,   236,   243,   315,   378,   407,   448,   536,   537,
    1103,  1078,   236,  1078,    23,   214,  1082,  1141,   278,   280,
     519,   520,   521,   516,   552,  1097,  1077,   280,  1077,   591,
     301,   592,  1070,   629,   687,   456,   635,    84,   636,  1100,
     442,  1140,  1126,  1079,  1097,   279,   380,   614,   236,  1078,
    1081,  1100,   436,  1140,   442,   725,   725,   170,   442,  1100,
     741,   742,   139,   480,    56,   443,   503,   124,   195,   236,
     243,   255,   315,   378,   381,   382,   448,   524,   525,   526,
     529,   537,   400,   534,  1100,   520,   384,  1159,   518,     1,
       4,    19,    29,   202,   233,   240,   261,   286,   292,   319,
     326,   339,   364,   366,   374,   405,   456,   553,   554,   559,
     561,   566,   567,   568,   569,   573,   574,   575,   576,   577,
     578,   579,   580,   582,  1123,  1077,  1100,   317,   688,   689,
     690,   727,   645,   642,  1140,   435,   674,   403,   612,  1097,
     234,  1144,   403,  1133,   200,  1139,   442,  1140,  1140,   742,
       1,   442,   743,   744,   745,   746,   747,   752,   485,   526,
      17,   400,  1103,  1100,  1140,   520,  1146,   319,   403,  1164,
     364,  1146,   218,  1142,  1140,    55,  1132,    37,   113,  1130,
    1142,  1142,   260,  1100,  1166,   384,  1140,   725,   690,     1,
      21,    34,    37,    38,    39,    40,    41,    42,    43,    74,
      76,    77,    78,    79,    80,    81,   118,   201,   217,   236,
     253,   276,   291,   298,   300,   313,   323,   330,   367,   369,
     370,   371,   393,   419,   420,   421,   432,   435,   436,   639,
     646,   647,   648,   650,   651,   652,   653,   654,   656,   668,
     669,   671,   672,   673,   679,   680,   681,  1140,  1157,    27,
    1128,   188,  1141,  1100,    56,   321,   608,   619,  1100,   372,
    1158,   236,   616,  1097,   616,   126,   456,   356,     3,     5,
      10,    18,    51,    52,    59,    71,    75,    87,   111,   118,
     119,   149,   161,   165,   179,   186,   189,   190,   197,   204,
     206,   209,   246,   250,   252,   259,   282,   297,   318,   327,
     340,   345,   347,   355,   365,   374,   383,   385,   386,   389,
     391,   396,   412,   417,   422,   433,   445,   449,   456,   753,
     754,   764,   769,   773,   776,   789,   792,   797,   802,   803,
     804,   807,   809,   816,   820,   822,   837,   840,   842,   844,
     847,   849,   855,   864,   866,   883,   885,   888,   892,   898,
     908,   915,   917,   920,   924,   925,   936,   947,   957,   963,
     967,   973,   977,   979,   981,   983,   986,   997,   998,  1007,
    1009,  1010,  1167,   456,   527,   529,  1082,  1140,  1142,   123,
     167,   556,  1140,  1140,   319,   326,   574,  1140,  1140,   364,
    1140,  1140,  1127,     9,   260,   318,   581,  1140,   442,   691,
     225,   368,   418,   368,   418,   368,   418,   368,   418,   368,
     418,   438,  1165,   346,  1154,  1100,  1096,  1097,  1097,   214,
     224,   346,   670,  1140,  1140,  1095,  1096,   167,   188,   223,
     411,     9,    50,   225,   643,  1101,  1102,  1103,   676,   677,
    1101,    30,   620,   621,   622,   623,  1129,  1166,  1133,   180,
     611,  1138,   109,   236,   748,   755,   765,   770,   774,   777,
     790,   793,   798,   805,   808,   810,   817,   821,   823,   838,
     841,   843,  1164,   848,     1,   850,   856,   865,   867,   884,
     886,   889,   893,   899,   909,   916,   918,   921,   926,   937,
     948,   958,   236,   349,   968,   974,   306,   978,   980,   982,
     984,   987,   188,   999,  1137,  1011,   195,   236,   243,   315,
     378,   448,   528,   530,   123,   316,   364,   560,  1140,   117,
     305,   555,    32,   164,   244,   570,  1078,  1081,   381,  1078,
    1078,   287,  1151,  1151,   281,  1078,    61,    88,    89,   293,
     456,   692,   693,   697,  1140,   448,   403,   657,   459,  1098,
    1099,   400,   653,  1101,   180,    27,   649,   362,  1119,  1119,
    1103,  1148,  1148,   463,   675,   677,   400,    48,   409,   174,
     612,  1100,   456,   456,   756,  1095,     6,    82,    94,    98,
     184,   225,   232,   238,   241,   274,   342,   387,   388,   413,
     431,   439,   766,  1064,  1085,  1086,  1095,  1101,  1104,   442,
     771,  1051,  1052,  1053,   236,  1074,  1075,  1076,  1097,   236,
    1093,  1095,  1104,     9,   791,   794,   799,  1065,  1066,  1086,
    1070,   405,   236,   811,  1085,  1092,  1095,   818,  1086,   236,
     404,   408,   824,   825,  1051,   296,   297,   310,   356,   839,
       6,  1083,  1084,  1095,  1095,   845,   136,  1050,  1051,  1083,
     696,  1095,   868,  1095,  1101,  1104,   949,  1097,    94,   887,
    1086,   890,  1086,   894,   177,   236,   900,   903,   904,   905,
    1074,  1093,  1097,  1166,  1070,  1067,  1097,  1070,  1067,     9,
     927,  1068,  1097,   150,   248,   938,   939,   940,   941,   943,
     944,   945,   946,  1071,  1072,  1083,   949,  1070,   965,   964,
     112,   969,   970,  1086,    94,   975,  1085,   696,  1095,  1070,
    1095,     8,    35,  1001,   107,  1067,    17,  1078,   118,   236,
     557,  1145,   441,   571,   571,   122,   376,   441,   457,   565,
    1113,  1122,  1078,  1080,  1140,   172,   694,   695,   694,  1141,
     705,   188,  1100,   402,  1163,   225,   450,   451,   459,  1061,
    1063,  1064,  1087,  1095,  1102,  1104,   459,  1099,  1097,  1086,
     236,  1132,  1096,  1096,  1103,  1165,  1101,  1081,  1081,  1129,
    1133,   127,   763,    30,   180,   757,  1129,  1148,   459,  1095,
     459,  1105,   459,  1106,  1148,  1120,   459,   459,   459,   459,
     459,   459,   459,   459,  1105,   128,   768,   403,   767,  1086,
     205,  1114,    56,  1054,  1055,   403,  1120,   434,   778,   236,
    1092,  1095,  1070,   130,   800,   156,   457,   801,  1066,   348,
    1118,   319,  1153,  1069,   132,   815,   757,   427,   428,   429,
     430,   133,   819,    49,   210,   778,    17,   438,   826,   827,
     828,   832,  1125,   100,  1148,  1084,  1073,   399,  1162,   857,
    1166,  1095,    92,   331,   394,   869,   870,   871,   875,   880,
     951,  1086,   403,   137,   891,    49,   166,   207,   215,   288,
     895,   904,   138,   901,   423,   436,   400,   402,   397,   258,
     304,  1115,   180,  1012,  1153,  1012,  1068,   143,   935,   436,
     929,  1090,  1095,  1102,   944,   946,  1083,   403,  1072,   121,
     403,   424,   942,   959,   187,   341,   966,  1131,   210,   970,
    1095,   146,   976,   180,   180,   319,   321,   985,   112,   988,
     333,   380,  1002,  1149,  1012,   530,   565,  1113,   558,  1097,
     240,   347,  1140,   122,   562,   563,  1092,   696,   703,  1100,
     637,   706,   114,   658,  1148,  1063,  1063,  1063,    70,   360,
     458,  1062,   450,   451,   452,   453,   455,   462,  1063,    49,
     365,  1155,  1144,  1081,   114,   610,  1090,    25,    26,    67,
      69,   103,   104,   105,   150,   152,   160,   234,   401,   442,
    1072,   441,   760,    66,   233,   301,   758,   759,   149,   310,
    1088,  1096,  1061,  1063,   403,  1063,  1061,  1107,  1096,  1102,
    1104,   442,  1063,  1110,  1063,  1063,  1109,  1063,  1061,  1061,
    1063,  1108,  1063,  1065,  1086,   187,   341,   772,  1114,    12,
      13,    14,    20,    58,   156,   157,   185,   191,   214,   222,
     226,   256,   257,   262,   272,   279,   284,   302,   448,   450,
     451,   452,   453,   455,   457,   458,   460,   461,  1056,  1057,
    1058,  1059,  1060,    12,    13,    14,    58,   214,   256,   257,
     262,   272,   279,   302,   450,   451,   455,   459,  1056,  1057,
    1058,  1059,  1060,  1086,   309,   775,  1076,   779,   187,   341,
     783,   324,   416,   795,   796,  1166,  1051,   213,   266,  1043,
    1044,  1045,  1047,   426,   441,   812,  1166,   163,  1018,  1019,
    1018,  1018,  1018,  1086,  1065,  1086,    21,   404,   408,   833,
     834,  1052,   134,   836,   440,   828,   830,   438,   829,   825,
    1096,   114,   846,  1074,   851,     9,    12,    15,    16,   253,
     254,   272,   273,   858,   862,   171,  1090,     9,    56,   173,
     223,   411,   876,   877,   878,   872,   870,   953,  1122,  1149,
     403,  1083,  1065,  1086,   366,   896,   749,   750,  1050,   906,
     907,  1095,  1074,     8,    35,  1014,  1153,  1092,   210,   910,
     922,  1166,   930,  1129,  1095,   930,   403,   403,   521,   149,
     404,   408,  1086,    49,   218,   960,  1086,  1086,   372,  1086,
    1095,   180,  1065,  1086,  1090,  1131,   210,   991,  1095,   159,
     163,  1003,     9,  1008,  1074,   922,   562,  1097,   280,   564,
    1078,  1113,   564,   193,   698,   233,   234,   704,   640,  1149,
      28,   115,   202,   655,   660,   661,   662,   663,   665,  1096,
    1102,  1104,   458,  1063,  1063,  1063,  1063,  1063,  1063,   458,
    1063,  1086,  1164,  1144,  1149,  1017,  1019,   447,   446,  1090,
    1017,   218,    31,    33,    36,    46,   176,   181,   194,   242,
     290,   314,   338,   343,   353,   357,   415,   425,   761,   762,
    1017,   270,  1147,  1147,  1147,   759,   758,   236,  1089,  1096,
     458,  1095,   462,   458,  1062,   458,   458,  1062,   458,   458,
     458,   458,  1062,   458,   458,   373,  1023,  1024,  1065,  1084,
     341,  1164,   398,  1161,  1161,   403,  1074,   780,   781,   782,
    1131,  1095,  1095,   163,   289,   784,  1004,  1137,   240,   260,
    1023,  1046,  1048,   131,   806,  1047,    96,   305,   442,  1072,
      33,    36,    44,    45,    46,   158,   176,   194,   242,   290,
     343,   353,   415,   813,   814,  1018,   269,  1020,   265,  1021,
     187,  1023,   187,  1125,   400,   835,   831,   833,   749,  1149,
     749,  1164,   331,   859,  1164,   403,    49,   877,   879,  1090,
       9,    56,   223,   411,   873,   874,  1090,   954,  1123,   200,
     285,  1150,   663,  1083,  1023,   187,  1166,  1069,   138,   902,
     751,     8,   180,   910,  1095,   126,   263,  1033,  1034,  1036,
    1043,   240,   260,   438,   126,   438,   932,   933,  1090,  1089,
    1086,  1140,  1043,   971,  1166,  1095,  1023,   187,   403,     9,
     989,   990,  1112,   992,  1095,   971,   992,   307,  1006,   308,
    1013,  1014,  1113,   251,   319,   321,   572,  1140,   173,   699,
    1100,   707,  1078,  1131,   664,   665,   661,  1142,   659,   660,
     458,   404,   678,  1078,  1021,  1017,  1140,  1140,   121,   424,
     762,  1092,  1092,  1092,  1105,  1118,   458,  1063,  1078,  1105,
    1105,  1063,  1105,  1105,  1105,   223,   411,  1105,  1105,  1025,
     268,  1026,  1023,  1084,   156,   284,   156,   284,   781,   279,
     737,    86,   325,   435,   265,   267,   786,  1005,   785,   329,
     344,   749,   749,   812,   812,   812,   812,  1140,   153,   155,
    1140,   121,   424,   814,   749,  1022,  1065,  1066,  1065,  1066,
     834,  1051,   749,  1095,   125,   852,   435,   860,   861,   862,
     110,   863,   435,  1091,  1095,  1101,  1090,    49,   881,   874,
     175,   881,   950,  1140,   285,  1142,  1065,   581,   897,  1166,
     752,   907,  1086,   199,   911,  1166,  1035,  1037,   141,   919,
    1036,   142,   923,   240,  1051,   931,  1050,   932,   262,   961,
    1116,   144,   962,   289,  1028,  1029,   300,  1118,  1065,  1091,
     284,  1090,   113,   993,   394,   995,  1149,   154,   264,  1015,
    1038,  1039,  1041,  1044,     7,  1124,   572,  1100,   116,   220,
     700,    67,    66,    67,   192,   233,   234,   258,   303,   376,
     390,   414,   456,   651,   652,   654,   656,   668,   671,   673,
     681,   708,   709,   711,   712,   713,   714,   716,   717,   718,
     722,   723,   442,   666,   667,  1140,  1140,   448,  1094,  1095,
    1100,  1094,  1049,  1131,  1049,  1023,   458,   749,  1027,  1164,
     156,  1164,   156,  1086,   129,   788,   787,   749,  1018,  1018,
    1018,  1018,  1094,  1094,  1049,  1049,   749,  1023,   328,  1023,
     328,   853,   136,   854,   861,   102,  1135,   881,   881,  1091,
    1014,   207,   434,   955,  1078,  1140,  1023,   240,   260,    49,
     240,   218,   912,   198,   240,   260,   437,   749,   749,   928,
     749,   934,   696,  1056,  1057,  1058,  1059,  1060,  1030,   145,
     972,   267,  1031,  1095,  1023,  1023,   990,  1139,    95,   994,
    1139,  1028,   166,   207,   215,   288,  1000,  1069,  1040,  1042,
     148,  1016,  1041,   293,  1072,  1094,  1140,    91,   221,   701,
     271,  1147,   203,   724,   270,   271,   721,  1126,   192,   438,
    1140,  1148,  1140,   714,   258,   299,   719,   720,  1100,   667,
    1077,  1101,  1094,   749,  1164,  1164,   749,  1066,  1066,   749,
      49,   881,   406,   882,   307,  1069,   187,   288,   956,   952,
     344,  1086,  1140,   913,  1033,  1044,   240,   240,   749,   749,
     749,  1032,  1095,  1139,  1095,   147,   996,   749,   749,   233,
     234,  1143,  1100,  1140,  1140,   174,   702,  1140,  1141,  1140,
    1050,  1095,   715,  1078,    90,    91,   116,   294,   295,   335,
     336,   710,   293,  1100,   720,  1143,  1023,  1023,  1086,  1086,
    1140,  1069,   307,  1097,   423,   696,   140,   914,   749,  1095,
    1100,  1100,  1140,  1100,  1100,  1118,   903,  1140,  1050,  1100,
     903
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
     632,   632,   633,   633,   634,   635,   634,   636,   634,   637,
     638,   639,   639,   640,   640,   640,   641,   642,   642,   643,
     643,   643,   644,   645,   645,   646,   646,   646,   646,   646,
     646,   646,   646,   646,   646,   646,   646,   646,   646,   646,
     646,   647,   648,   649,   649,   650,   651,   652,   652,   653,
     653,   653,   653,   653,   653,   653,   653,   653,   653,   653,
     653,   653,   653,   653,   653,   653,   653,   653,   653,   653,
     653,   653,   653,   653,   653,   653,   653,   653,   653,   653,
     653,   653,   653,   653,   654,   654,   655,   655,   655,   656,
     657,   657,   658,   658,   659,   659,   660,   661,   662,   662,
     663,   663,   664,   664,   665,   666,   666,   667,   668,   669,
     670,   670,   670,   671,   672,   673,   675,   674,   676,   676,
     677,   677,   678,   678,   679,   679,   680,   681,   682,   683,
     682,   684,   685,   684,   686,   687,   686,   688,   688,   689,
     689,   690,   691,   691,   692,   692,   692,   692,   693,   693,
     694,   695,   695,   696,   696,   697,   698,   698,   699,   699,
     700,   700,   701,   701,   702,   702,   703,   703,   703,   704,
     704,   705,   705,   706,   707,   707,   708,   708,   708,   708,
     708,   708,   708,   708,   708,   708,   708,   708,   708,   708,
     708,   708,   709,   710,   710,   710,   710,   710,   710,   710,
     711,   712,   712,   712,   713,   713,   714,   715,   716,   681,
     717,   718,   718,   719,   719,   720,   720,   720,   721,   721,
     721,   722,   723,   724,   724,   725,   726,   727,   726,   728,
     729,   730,   728,   731,   732,   731,   733,   731,   734,   734,
     735,   736,   736,   736,   737,   737,   737,   737,   737,   738,
     738,   739,   739,   740,   741,   740,   742,   742,   743,   743,
     743,   743,   743,   744,   745,   746,   747,   748,   748,   750,
     751,   749,   752,   752,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   753,   753,   753,   753,   753,
     753,   753,   753,   753,   753,   755,   754,   756,   756,   756,
     756,   756,   756,   756,   756,   756,   756,   756,   756,   756,
     756,   756,   756,   756,   757,   757,   757,   757,   757,   757,
     758,   759,   759,   760,   760,   761,   761,   762,   762,   762,
     762,   762,   762,   762,   762,   762,   762,   762,   762,   762,
     762,   762,   762,   762,   763,   763,   765,   764,   766,   766,
     766,   767,   767,   768,   768,   770,   769,   771,   771,   772,
     772,   773,   774,   774,   775,   775,   777,   776,   778,   779,
     778,   780,   780,   781,   781,   782,   782,   782,   782,   783,
     783,   783,   784,   785,   784,   786,   787,   786,   788,   788,
     790,   789,   791,   791,   791,   793,   792,   794,   794,   795,
     795,   795,   795,   795,   796,   796,   798,   797,   799,   800,
     800,   801,   801,   802,   803,   805,   804,   806,   806,   808,
     807,   810,   809,   811,   811,   811,   811,   811,   811,   811,
     811,   811,   812,   812,   812,   813,   813,   814,   814,   814,
     814,   814,   814,   814,   814,   814,   814,   814,   814,   814,
     814,   814,   815,   815,   817,   816,   818,   818,   818,   818,
     818,   819,   819,   821,   820,   823,   822,   824,   824,   825,
     825,   825,   826,   827,   827,   829,   828,   830,   831,   830,
     832,   832,   833,   833,   834,   834,   834,   834,   835,   835,
     836,   836,   838,   837,   839,   839,   839,   839,   839,   839,
     841,   840,   843,   842,   845,   844,   846,   846,   848,   847,
     850,   851,   849,   849,   852,   853,   852,   854,   854,   856,
     855,   857,   857,   858,   858,   858,   859,   859,   860,   860,
     861,   862,   862,   862,   862,   862,   862,   862,   863,   863,
     865,   864,   867,   866,   868,   868,   868,   869,   869,   870,
     870,   870,   872,   871,   873,   873,   874,   874,   874,   874,
     874,   874,   875,   876,   876,   877,   877,   878,   878,   878,
     878,   878,   879,   880,   881,   881,   882,   882,   884,   883,
     886,   885,   887,   887,   889,   888,   890,   890,   891,   891,
     893,   892,   894,   894,   895,   895,   895,   895,   896,   896,
     897,   897,   897,   899,   898,   900,   901,   900,   900,   902,
     902,   903,   903,   904,   904,   904,   904,   904,   905,   905,
     906,   906,   907,   909,   908,   910,   910,   911,   911,   911,
     911,   911,   911,   912,   912,   913,   913,   913,   914,   914,
     916,   915,   918,   917,   919,   919,   921,   920,   922,   922,
     922,   923,   923,   924,   926,   925,   927,   928,   927,   929,
     929,   930,   931,   930,   932,   932,   934,   933,   935,   935,
     937,   936,   938,   938,   938,   938,   938,   939,   940,   940,
     941,   942,   942,   943,   943,   944,   945,   945,   946,   946,
     948,   947,   950,   949,   951,   951,   952,   952,   953,   953,
     954,   954,   955,   955,   955,   956,   956,   956,   958,   959,
     957,   960,   960,   961,   961,   961,   961,   961,   962,   962,
     964,   963,   965,   963,   966,   966,   966,   968,   967,   969,
     969,   970,   970,   970,   971,   971,   972,   972,   974,   973,
     975,   975,   975,   976,   976,   977,   978,   978,   980,   979,
     982,   981,   984,   983,   985,   985,   985,   987,   986,   988,
     988,   989,   989,   990,   991,   991,   992,   993,   993,   994,
     994,   995,   995,   996,   996,   997,   997,   997,   998,   999,
     999,  1000,  1000,  1000,  1000,  1000,  1001,  1001,  1002,  1002,
    1003,  1003,  1004,  1004,  1005,  1005,  1006,  1006,  1007,  1008,
    1008,  1009,  1011,  1010,  1012,  1012,  1013,  1013,  1013,  1013,
    1014,  1014,  1015,  1015,  1015,  1016,  1016,  1017,  1018,  1019,
    1020,  1019,  1021,  1022,  1021,  1023,  1024,  1025,  1024,  1026,
    1027,  1026,  1028,  1029,  1030,  1029,  1031,  1032,  1031,  1033,
    1033,  1033,  1035,  1034,  1037,  1036,  1038,  1038,  1038,  1040,
    1039,  1042,  1041,  1043,  1043,  1044,  1044,  1044,  1046,  1045,
    1048,  1047,  1049,  1049,  1050,  1051,  1053,  1052,  1054,  1054,
    1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,
    1054,  1054,  1054,  1054,  1054,  1054,  1054,  1055,  1055,  1055,
    1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,
    1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,  1055,
    1055,  1055,  1055,  1055,  1055,  1056,  1056,  1056,  1057,  1057,
    1058,  1058,  1059,  1059,  1059,  1060,  1060,  1060,  1061,  1061,
    1062,  1062,  1062,  1063,  1063,  1063,  1063,  1063,  1063,  1063,
    1063,  1063,  1064,  1064,  1065,  1065,  1066,  1067,  1068,  1069,
    1069,  1070,  1071,  1071,  1072,  1073,  1073,  1074,  1075,  1075,
    1075,  1076,  1077,  1077,  1078,  1079,  1079,  1080,  1080,  1081,
    1081,  1082,  1083,  1083,  1084,  1084,  1085,  1085,  1086,  1086,
    1086,  1086,  1086,  1086,  1086,  1086,  1086,  1087,  1087,  1087,
    1087,  1087,  1087,  1087,  1088,  1088,  1089,  1089,  1090,  1090,
    1091,  1091,  1092,  1092,  1093,  1093,  1093,  1094,  1094,  1094,
    1095,  1096,  1096,  1096,  1096,  1097,  1097,  1098,  1099,  1099,
    1100,  1101,  1101,  1102,  1102,  1103,  1103,  1103,  1103,  1103,
    1103,  1103,  1104,  1104,  1104,  1104,  1104,  1104,  1104,  1104,
    1104,  1104,  1104,  1104,  1105,  1105,  1105,  1106,  1106,  1107,
    1107,  1108,  1108,  1108,  1109,  1109,  1110,  1110,  1111,  1112,
    1112,  1113,  1113,  1114,  1114,  1115,  1115,  1115,  1116,  1116,
    1117,  1117,  1118,  1118,  1119,  1119,  1120,  1120,  1121,  1121,
    1122,  1122,  1123,  1123,  1124,  1124,  1125,  1125,  1126,  1126,
    1127,  1127,  1128,  1128,  1129,  1129,  1130,  1130,  1131,  1131,
    1132,  1132,  1133,  1133,  1134,  1134,  1135,  1135,  1136,  1136,
    1137,  1137,  1138,  1138,  1139,  1139,  1140,  1140,  1141,  1141,
    1141,  1142,  1142,  1143,  1143,  1143,  1144,  1144,  1145,  1145,
    1146,  1146,  1147,  1147,  1148,  1148,  1149,  1149,  1150,  1150,
    1150,  1151,  1151,  1152,  1152,  1153,  1153,  1154,  1154,  1155,
    1155,  1156,  1156,  1157,  1157,  1157,  1158,  1158,  1159,  1159,
    1160,  1160,  1161,  1161,  1162,  1162,  1163,  1163,  1164,  1164,
    1165,  1165,  1166,  1166,  1167
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
       1,     1,     1,     1,     1,     0,     4,     4,     4,     3,
       3,     3,     4,     3,     4,     3,     3,     3,     4,     5,
       3,     4,     3,     3,     0,     3,     3,     2,     2,     2,
       3,     3,     3,     0,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     0,     1,     0,     4,     4,     5,
       6,     0,     2,     0,     1,     0,     3,     3,     5,     0,
       2,     2,     0,     5,     0,     2,     0,     8,     0,     0,
       3,     1,     2,     2,     3,     0,     2,     2,     2,     0,
       2,     2,     0,     0,     3,     0,     0,     3,     0,     1,
       0,     3,     0,     2,     1,     0,     3,     0,     3,     0,
       1,     3,     3,     2,     1,     1,     0,     4,     4,     0,
       1,     1,     1,     1,     1,     0,     6,     0,     1,     0,
       4,     0,     4,     3,     3,     3,     3,     4,     6,     6,
       6,     6,     0,     2,     2,     1,     2,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       1,     1,     0,     1,     0,     4,     4,     6,     6,     8,
       8,     0,     1,     0,     4,     0,     5,     1,     3,     1,
       1,     1,     2,     1,     2,     0,     3,     0,     0,     3,
       2,     3,     1,     3,     2,     1,     1,     1,     0,     2,
       0,     1,     0,     3,     0,     1,     1,     2,     1,     1,
       0,     3,     0,     3,     0,     5,     0,     3,     0,     2,
       0,     0,     8,     3,     0,     0,     3,     0,     1,     0,
       7,     0,     2,     0,     3,     3,     0,     2,     1,     2,
       4,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       0,     3,     0,     4,     1,     1,     1,     1,     2,     1,
       1,     1,     0,     3,     1,     2,     2,     2,     1,     1,
       1,     2,     2,     1,     2,     4,     2,     0,     1,     1,
       1,     1,     4,     5,     0,     4,     0,     1,     0,     3,
       0,     3,     3,     4,     0,     4,     4,     6,     0,     1,
       0,     3,     0,     5,     1,     1,     1,     1,     0,     3,
       0,     3,     2,     0,     3,     2,     0,     4,     2,     0,
       1,     1,     3,     0,     1,     2,     3,     3,     0,     3,
       1,     3,     7,     0,    10,     0,     2,     0,     2,     2,
       3,     3,     2,     0,     3,     0,     1,     1,     0,     1,
       0,     4,     0,     7,     0,     1,     0,     7,     0,     2,
       3,     0,     1,     1,     0,     4,     4,     0,     7,     0,
       2,     0,     0,     4,     1,     2,     0,     4,     0,     1,
       0,     3,     1,     1,     1,     1,     1,     4,     4,     3,
       4,     1,     1,     1,     2,     3,     1,     2,     3,     3,
       0,     3,     0,     7,     0,     6,     0,     2,     0,     2,
       0,     3,     0,     2,     4,     0,     2,     4,     0,     0,
       7,     0,     4,     2,     2,     2,     2,     2,     0,     1,
       0,     4,     0,     3,     0,     2,     2,     0,     8,     1,
       2,     1,     3,     3,     0,     3,     0,     1,     0,     4,
       4,     6,     6,     0,     1,     2,     0,     1,     0,     3,
       0,     7,     0,     4,     0,     1,     1,     0,     9,     0,
       3,     1,     3,     2,     2,     2,     3,     0,     3,     0,
       3,     0,     3,     0,     1,     1,     1,     1,     8,     0,
       1,     1,     1,     1,     1,     1,     0,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     5,     1,
       2,     5,     0,     8,     0,     2,     0,     4,     3,     3,
       1,     1,     0,     1,     1,     0,     1,     2,     2,     0,
       0,     3,     0,     0,     3,     2,     0,     0,     3,     0,
       0,     3,     2,     0,     0,     3,     0,     0,     3,     1,
       1,     2,     0,     3,     0,     3,     1,     1,     2,     0,
       3,     0,     3,     0,     1,     1,     1,     2,     0,     3,
       0,     3,     0,     3,     1,     1,     0,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     0,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     2,     1,     1,     2,
       1,     2,     1,     5,     4,     1,     5,     4,     1,     3,
       0,     1,     1,     1,     3,     3,     3,     3,     2,     2,
       3,     3,     1,     3,     1,     2,     2,     1,     1,     1,
       2,     1,     1,     2,     1,     0,     2,     1,     1,     1,
       3,     1,     1,     2,     1,     1,     2,     0,     1,     1,
       1,     1,     1,     2,     1,     3,     1,     2,     1,     3,
       3,     3,     4,     3,     1,     1,     1,     1,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     1,     3,     3,     4,     5,
       1,     1,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     5,     5,     5,     5,     5,     5,
       5,     4,     5,     2,     0,     4,     5,     0,     3,     0,
       1,     1,     3,     3,     1,     3,     1,     3,     0,     0,
       1,     0,     1,     0,     1,     0,     1,     1,     0,     1,
       0,     1,     0,     1,     0,     2,     1,     1,     2,     2,
       2,     1,     2,     1,     0,     1,     0,     1,     0,     1,
       0,     1,     0,     1,     0,     1,     0,     1,     0,     1,
       0,     1,     0,     1,     0,     1,     0,     1,     0,     1,
       0,     1,     0,     1,     0,     1,     0,     1,     0,     1,
       1,     0,     1,     0,     1,     1,     0,     1,     0,     1,
       0,     1,     0,     1,     0,     1,     0,     1,     0,     1,
       2,     0,     1,     0,     1,     0,     1,     0,     1,     0,
       1,     0,     1,     0,     1,     2,     0,     1,     0,     1,
       0,     1,     0,     1,     0,     1,     0,     1,     0,     1,
       0,     1,     0,     1,     1
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
#line 775 "parser.y"
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
#line 5196 "parser.c"
    break;

  case 3: /* start: $@1 nested_list "end of file"  */
#line 799 "parser.y"
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
#line 5218 "parser.c"
    break;

  case 8: /* $@2: %empty  */
#line 830 "parser.y"
                        { cb_validate_program_environment (current_program); }
#line 5224 "parser.c"
    break;

  case 9: /* $@3: %empty  */
#line 831 "parser.y"
                        { cb_validate_program_data (current_program); }
#line 5230 "parser.c"
    break;

  case 10: /* program_definition: identification_division environment_division $@2 data_division $@3 procedure_division nested_prog end_program  */
#line 834 "parser.y"
              {
	cb_tree file = current_program->file_list;
	for(; file; file = CB_CHAIN(file)) {
		cb_validate_indexed_file_key(CB_FILE(CB_VALUE(file)));
	}
  }
#line 5241 "parser.c"
    break;

  case 11: /* $@4: %empty  */
#line 844 "parser.y"
                        { cb_validate_program_environment (current_program); }
#line 5247 "parser.c"
    break;

  case 12: /* $@5: %empty  */
#line 845 "parser.y"
                        { cb_validate_program_data (current_program); }
#line 5253 "parser.c"
    break;

  case 13: /* program_mandatory: identification_division environment_division $@4 data_division $@5 procedure_division nested_prog end_mandatory  */
#line 848 "parser.y"
                {
	cb_tree file = current_program->file_list;
	for(; file; file = CB_CHAIN(file)) {
		cb_validate_indexed_file_key(CB_FILE(CB_VALUE(file)));
	}
  }
#line 5264 "parser.c"
    break;

  case 14: /* $@6: %empty  */
#line 858 "parser.y"
                        { cb_validate_program_environment (current_program); }
#line 5270 "parser.c"
    break;

  case 15: /* $@7: %empty  */
#line 859 "parser.y"
                        { cb_validate_program_data (current_program); }
#line 5276 "parser.c"
    break;

  case 21: /* end_program: "END PROGRAM" program_name '.'  */
#line 871 "parser.y"
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
#line 5305 "parser.c"
    break;

  case 22: /* end_mandatory: "END PROGRAM" program_name '.'  */
#line 899 "parser.y"
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
#line 5332 "parser.c"
    break;

  case 23: /* end_function: "END FUNCTION" program_name '.'  */
#line 925 "parser.y"
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
#line 5359 "parser.c"
    break;

  case 24: /* $@8: %empty  */
#line 956 "parser.y"
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
#line 5403 "parser.c"
    break;

  case 26: /* function_division: "FUNCTION-ID" '.' program_name as_literal '.'  */
#line 1000 "parser.y"
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
#line 5448 "parser.c"
    break;

  case 29: /* as_literal: %empty  */
#line 1048 "parser.y"
                                { yyval = NULL; }
#line 5454 "parser.c"
    break;

  case 30: /* as_literal: AS "Literal"  */
#line 1049 "parser.y"
                                { yyval = yyvsp[0]; }
#line 5460 "parser.c"
    break;

  case 33: /* program_type_clause: COMMON  */
#line 1058 "parser.y"
  {
	if (!current_program->nested_level) {
		cb_error (_("COMMON may only be used in a nested program"));
	}
	current_program->flag_common = 1;
  }
#line 5471 "parser.c"
    break;

  case 34: /* program_type_clause: COMMON _init_or_recurs  */
#line 1065 "parser.y"
  {
	if (!current_program->nested_level) {
		cb_error (_("COMMON may only be used in a nested program"));
	}
	current_program->flag_common = 1;
  }
#line 5482 "parser.c"
    break;

  case 36: /* _init_or_recurs: "INITIAL"  */
#line 1076 "parser.y"
  {
	current_program->flag_initial = 1;
  }
#line 5490 "parser.c"
    break;

  case 37: /* _init_or_recurs: RECURSIVE  */
#line 1080 "parser.y"
  {
	current_program->flag_recursive = 1;
	current_program->flag_initial = 1;
  }
#line 5499 "parser.c"
    break;

  case 41: /* configuration_section: CONFIGURATION SECTION '.' configuration_list  */
#line 1105 "parser.y"
  {
	if (current_program->nested_level) {
		cb_error (_("CONFIGURATION SECTION not allowed in nested programs"));
	}
  }
#line 5509 "parser.c"
    break;

  case 53: /* with_debugging_mode: _with DEBUGGING MODE  */
#line 1140 "parser.y"
  {
	cb_verify (cb_debugging_line, "DEBUGGING MODE");
  }
#line 5517 "parser.c"
    break;

  case 54: /* computer_name: "Identifier"  */
#line 1146 "parser.y"
       { }
#line 5523 "parser.c"
    break;

  case 65: /* object_computer_memory: MEMORY SIZE _is integer object_char_or_word  */
#line 1177 "parser.y"
  {
	cb_verify (cb_memory_size_clause, "MEMORY SIZE");
  }
#line 5531 "parser.c"
    break;

  case 68: /* object_computer_sequence: _program coll_sequence _is reference  */
#line 1189 "parser.y"
  {
	current_program->collating_sequence = yyvsp[0];
  }
#line 5539 "parser.c"
    break;

  case 69: /* object_computer_segment: "SEGMENT-LIMIT" _is integer  */
#line 1196 "parser.y"
  {
	/* Ignore */
  }
#line 5547 "parser.c"
    break;

  case 75: /* repository_name: FUNCTION repository_literal_list INTRINSIC  */
#line 1221 "parser.y"
  {
	current_program->function_spec_list = yyvsp[-1];
  }
#line 5555 "parser.c"
    break;

  case 76: /* repository_name: FUNCTION ALL INTRINSIC  */
#line 1225 "parser.y"
  {
	functions_are_all = 1;
  }
#line 5563 "parser.c"
    break;

  case 77: /* repository_literal_list: "Literal"  */
#line 1231 "parser.y"
                        { yyval = cb_list_init (yyvsp[0]); }
#line 5569 "parser.c"
    break;

  case 78: /* repository_literal_list: repository_literal_list "Literal"  */
#line 1233 "parser.y"
                        { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5575 "parser.c"
    break;

  case 96: /* mnemonic_name_clause: "Identifier" _is CRT  */
#line 1274 "parser.y"
  {
	save_tree_1 = lookup_system_name (CB_NAME (yyvsp[-2]));
	if (save_tree_1 == cb_error_node) {
		cb_error_x (yyvsp[-2], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-2]));
	} else if (CB_SYSTEM_NAME(save_tree_1)->token != CB_DEVICE_CONSOLE) {
		cb_error_x (save_tree_1, _("Invalid CRT clause"));
	}
	/* current_program->flag_screen = 1; */
  }
#line 5589 "parser.c"
    break;

  case 97: /* $@9: %empty  */
#line 1284 "parser.y"
  {
	save_tree_1 = lookup_system_name (CB_NAME (yyvsp[-2]));
	if (save_tree_1 == cb_error_node) {
		cb_error_x (yyvsp[-2], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-2]));
	} else {
		cb_define (yyvsp[0], save_tree_1);
	}
	save_tree_2 = yyvsp[0];
  }
#line 5603 "parser.c"
    break;

  case 99: /* $@10: %empty  */
#line 1295 "parser.y"
  {
	save_tree_1 = lookup_system_name (CB_NAME (yyvsp[-1]));
	if (save_tree_1 == cb_error_node) {
		cb_error_x (yyvsp[-1], _("Unknown system-name '%s'"), CB_NAME (yyvsp[-1]));
	}
	save_tree_2 = NULL;
  }
#line 5615 "parser.c"
    break;

  case 101: /* mnemonic_name_clause: "ARGUMENT-NUMBER" _is undefined_word  */
#line 1304 "parser.y"
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
#line 5633 "parser.c"
    break;

  case 102: /* mnemonic_name_clause: "ARGUMENT-VALUE" _is undefined_word  */
#line 1318 "parser.y"
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
#line 5651 "parser.c"
    break;

  case 103: /* mnemonic_name_clause: "ENVIRONMENT-NAME" _is undefined_word  */
#line 1332 "parser.y"
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
#line 5669 "parser.c"
    break;

  case 104: /* mnemonic_name_clause: "ENVIRONMENT-VALUE" _is undefined_word  */
#line 1346 "parser.y"
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
#line 5687 "parser.c"
    break;

  case 109: /* special_name_mnemonic_on_off: on_or_off _status _is undefined_word  */
#line 1372 "parser.y"
  {
	if (!save_tree_2 && !cb_switch_no_mnemonic) {
		cb_error_x (yyvsp[0], _("'%s' with no mnemonic name"), CB_NAME (yyvsp[0]));
	} else {
		cb_define_switch_name (yyvsp[0], save_tree_1, yyvsp[-3], save_tree_2);
	}
  }
#line 5699 "parser.c"
    break;

  case 110: /* on_or_off: ON  */
#line 1382 "parser.y"
                                { yyval = cb_int1; }
#line 5705 "parser.c"
    break;

  case 111: /* on_or_off: OFF  */
#line 1383 "parser.y"
                                { yyval = cb_int0; }
#line 5711 "parser.c"
    break;

  case 112: /* $@11: %empty  */
#line 1391 "parser.y"
  {
	save_tree_1 = yyvsp[0];
  }
#line 5719 "parser.c"
    break;

  case 113: /* alphabet_name_clause: ALPHABET undefined_word $@11 _is alphabet_definition  */
#line 1395 "parser.y"
  {
	current_program->alphabet_name_list =
		cb_list_add (current_program->alphabet_name_list, yyvsp[0]);
  }
#line 5728 "parser.c"
    break;

  case 114: /* alphabet_definition: NATIVE  */
#line 1402 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_NATIVE); }
#line 5734 "parser.c"
    break;

  case 115: /* alphabet_definition: "STANDARD-1"  */
#line 1403 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_STANDARD_1); }
#line 5740 "parser.c"
    break;

  case 116: /* alphabet_definition: "STANDARD-2"  */
#line 1404 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_STANDARD_2); }
#line 5746 "parser.c"
    break;

  case 117: /* alphabet_definition: EBCDIC  */
#line 1405 "parser.y"
                { yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_EBCDIC); }
#line 5752 "parser.c"
    break;

  case 118: /* alphabet_definition: alphabet_literal_list  */
#line 1407 "parser.y"
  {
	yyval = cb_build_alphabet_name (save_tree_1, CB_ALPHABET_CUSTOM);
	CB_ALPHABET_NAME (yyval)->custom_list = yyvsp[0];
  }
#line 5761 "parser.c"
    break;

  case 119: /* alphabet_literal_list: alphabet_literal  */
#line 1414 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5767 "parser.c"
    break;

  case 120: /* alphabet_literal_list: alphabet_literal_list alphabet_literal  */
#line 1416 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5773 "parser.c"
    break;

  case 121: /* alphabet_literal: alphabet_lits  */
#line 1420 "parser.y"
                                        { yyval = yyvsp[0]; }
#line 5779 "parser.c"
    break;

  case 122: /* alphabet_literal: alphabet_lits THRU alphabet_lits  */
#line 1421 "parser.y"
                                        { yyval = cb_build_pair (yyvsp[-2], yyvsp[0]); }
#line 5785 "parser.c"
    break;

  case 123: /* @12: %empty  */
#line 1423 "parser.y"
  {
	yyval = cb_list_init (yyvsp[-1]);
	save_tree_2 = yyval;
  }
#line 5794 "parser.c"
    break;

  case 124: /* alphabet_literal: alphabet_lits ALSO @12 alphabet_also_sequence  */
#line 1428 "parser.y"
  {
	yyval = yyvsp[-1];
  }
#line 5802 "parser.c"
    break;

  case 127: /* alphabet_lits: "Literal"  */
#line 1439 "parser.y"
                                { yyval = yyvsp[0]; }
#line 5808 "parser.c"
    break;

  case 128: /* alphabet_lits: SPACE  */
#line 1440 "parser.y"
                                { yyval = cb_space; }
#line 5814 "parser.c"
    break;

  case 129: /* alphabet_lits: ZERO  */
#line 1441 "parser.y"
                                { yyval = cb_zero; }
#line 5820 "parser.c"
    break;

  case 130: /* alphabet_lits: QUOTE  */
#line 1442 "parser.y"
                                { yyval = cb_quote; }
#line 5826 "parser.c"
    break;

  case 131: /* alphabet_lits: "HIGH-VALUE"  */
#line 1443 "parser.y"
                                { yyval = cb_norm_high; }
#line 5832 "parser.c"
    break;

  case 132: /* alphabet_lits: "LOW-VALUE"  */
#line 1444 "parser.y"
                                { yyval = cb_norm_low; }
#line 5838 "parser.c"
    break;

  case 133: /* alphabet_also_literal: "Literal"  */
#line 1448 "parser.y"
                                { cb_list_add (save_tree_2, yyvsp[0]); }
#line 5844 "parser.c"
    break;

  case 134: /* alphabet_also_literal: SPACE  */
#line 1449 "parser.y"
                                { cb_list_add (save_tree_2, cb_space); }
#line 5850 "parser.c"
    break;

  case 135: /* alphabet_also_literal: ZERO  */
#line 1450 "parser.y"
                                { cb_list_add (save_tree_2, cb_zero); }
#line 5856 "parser.c"
    break;

  case 136: /* alphabet_also_literal: QUOTE  */
#line 1451 "parser.y"
                                { cb_list_add (save_tree_2, cb_quote); }
#line 5862 "parser.c"
    break;

  case 137: /* alphabet_also_literal: "HIGH-VALUE"  */
#line 1452 "parser.y"
                                { cb_list_add (save_tree_2, cb_norm_high); }
#line 5868 "parser.c"
    break;

  case 138: /* alphabet_also_literal: "LOW-VALUE"  */
#line 1453 "parser.y"
                                { cb_list_add (save_tree_2, cb_norm_low); }
#line 5874 "parser.c"
    break;

  case 139: /* symbolic_characters_clause: SYMBOLIC _characters symbolic_characters_list  */
#line 1461 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->symbolic_list =
			cb_list_add (current_program->symbolic_list, yyvsp[0]);
	}
	PENDING ("SYMBOLIC CHARACTERS");
  }
#line 5886 "parser.c"
    break;

  case 140: /* symbolic_characters_list: char_list _is_are integer_list  */
#line 1472 "parser.y"
  {
	if (cb_list_length (yyvsp[-2]) != cb_list_length (yyvsp[0])) {
		cb_error (_("Invalid SYMBOLIC clause"));
		yyval = NULL;
	} else {
		yyval = NULL;
	}
  }
#line 5899 "parser.c"
    break;

  case 141: /* char_list: undefined_word  */
#line 1483 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5905 "parser.c"
    break;

  case 142: /* char_list: char_list undefined_word  */
#line 1484 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5911 "parser.c"
    break;

  case 143: /* integer_list: integer  */
#line 1488 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5917 "parser.c"
    break;

  case 144: /* integer_list: integer_list integer  */
#line 1489 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5923 "parser.c"
    break;

  case 145: /* class_name_clause: CLASS undefined_word _is class_item_list  */
#line 1497 "parser.y"
  {
	current_program->class_name_list =
			cb_list_add (current_program->class_name_list,
			cb_build_class_name (yyvsp[-2], yyvsp[0]));
  }
#line 5933 "parser.c"
    break;

  case 146: /* class_item_list: class_item  */
#line 1505 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 5939 "parser.c"
    break;

  case 147: /* class_item_list: class_item_list class_item  */
#line 1506 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 5945 "parser.c"
    break;

  case 148: /* class_item: basic_value  */
#line 1510 "parser.y"
                                { yyval = yyvsp[0]; }
#line 5951 "parser.c"
    break;

  case 149: /* class_item: basic_value THRU basic_value  */
#line 1512 "parser.y"
  {
	/* if (CB_LITERAL ($1)->data[0] < CB_LITERAL ($3)->data[0]) */
	if (literal_value (yyvsp[-2]) < literal_value (yyvsp[0])) {
		yyval = cb_build_pair (yyvsp[-2], yyvsp[0]);
	} else {
		yyval = cb_build_pair (yyvsp[0], yyvsp[-2]);
	}
  }
#line 5964 "parser.c"
    break;

  case 150: /* locale_clause: LOCALE undefined_word _is reference  */
#line 1526 "parser.y"
  {
	cb_tree	l;

	l = cb_build_locale_name (yyvsp[-2], yyvsp[0]);
	if (l != cb_error_node) {
		current_program->locale_list =
			cb_list_add (current_program->locale_list, l);
	}
  }
#line 5978 "parser.c"
    break;

  case 151: /* currency_sign_clause: CURRENCY _sign _is "Literal"  */
#line 1541 "parser.y"
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
#line 6043 "parser.c"
    break;

  case 152: /* decimal_point_clause: "DECIMAL-POINT" _is COMMA  */
#line 1608 "parser.y"
  {
	current_program->decimal_point = ',';
	current_program->numeric_separator = '.';
  }
#line 6052 "parser.c"
    break;

  case 153: /* cursor_clause: CURSOR _is reference  */
#line 1618 "parser.y"
                                { current_program->cursor_pos = yyvsp[0]; }
#line 6058 "parser.c"
    break;

  case 154: /* crt_status_clause: CRT STATUS _is reference  */
#line 1625 "parser.y"
                                { current_program->crt_status = yyvsp[0]; }
#line 6064 "parser.c"
    break;

  case 155: /* screen_control: "SCREEN-CONTROL" _is reference  */
#line 1632 "parser.y"
                                {  PENDING ("SCREEN CONTROL"); }
#line 6070 "parser.c"
    break;

  case 156: /* event_status: "EVENT-STATUS" _is reference  */
#line 1638 "parser.y"
                                {  PENDING ("EVENT STATUS"); }
#line 6076 "parser.c"
    break;

  case 159: /* $@13: %empty  */
#line 1650 "parser.y"
  {
	/* hack for MF compatibility */
	if (cb_relaxed_syntax_check) {
		cb_warning (_("INPUT-OUTPUT SECTION header missing - assumed"));
	} else {
		cb_error (_("INPUT-OUTPUT SECTION header missing"));
	}
  }
#line 6089 "parser.c"
    break;

  case 161: /* $@14: %empty  */
#line 1659 "parser.y"
  {
	/* hack for MF compatibility */
	if (cb_relaxed_syntax_check) {
		cb_warning (_("INPUT-OUTPUT SECTION header missing - assumed"));
	} else {
		cb_error (_("INPUT-OUTPUT SECTION header missing"));
	}
  }
#line 6102 "parser.c"
    break;

  case 167: /* $@15: %empty  */
#line 1684 "parser.y"
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
#line 6121 "parser.c"
    break;

  case 168: /* file_control_entry: SELECT flag_optional undefined_word $@15 select_clause_sequence '.'  */
#line 1699 "parser.y"
  {
	validate_file (current_file, yyvsp[-3]);
  }
#line 6129 "parser.c"
    break;

  case 186: /* assign_clause: ASSIGN _to _ext_clause _device assignment_name  */
#line 1731 "parser.y"
  {
	current_file->assign = cb_build_assignment_name (current_file, yyvsp[0]);
  }
#line 6137 "parser.c"
    break;

  case 187: /* assign_clause: ASSIGN _to _ext_clause DISK  */
#line 1735 "parser.y"
  {
	current_file->fileid_assign = 1;
	current_file->assign = cb_build_assignment_name (current_file, cb_build_reference ("DISK"));
  }
#line 6146 "parser.c"
    break;

  case 188: /* assign_clause: ASSIGN _to _ext_clause PRINTER  */
#line 1740 "parser.y"
  {
	current_file->fileid_assign = 1;
	current_file->assign = cb_build_assignment_name (current_file, cb_build_reference ("PRINTER"));
  }
#line 6155 "parser.c"
    break;

  case 191: /* _device: PRINTER  */
#line 1748 "parser.y"
                { current_file->organization = COB_ORG_LINE_SEQUENTIAL; }
#line 6161 "parser.c"
    break;

  case 193: /* _ext_clause: EXTERNAL  */
#line 1753 "parser.y"
  {
	current_file->external_assign = 1;
  }
#line 6169 "parser.c"
    break;

  case 194: /* _ext_clause: DYNAMIC  */
#line 1757 "parser.y"
  {
	current_file->external_assign = 0;
  }
#line 6177 "parser.c"
    break;

  case 196: /* assignment_name: DISPLAY  */
#line 1765 "parser.y"
  {
	const char	*s;

	s = "$#@DUMMY@#$";
	yyval = cb_build_alphanumeric_literal ((unsigned char *)s, strlen (s));
  }
#line 6188 "parser.c"
    break;

  case 197: /* assignment_name: _literal assignment_device_name_list  */
#line 1772 "parser.y"
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
#line 6209 "parser.c"
    break;

  case 198: /* assignment_device_name_list: qualified_word  */
#line 1791 "parser.y"
                                                { yyval = cb_list_init (yyvsp[0]); }
#line 6215 "parser.c"
    break;

  case 199: /* assignment_device_name_list: assignment_device_name_list qualified_word  */
#line 1792 "parser.y"
                                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 6221 "parser.c"
    break;

  case 201: /* access_mode: SEQUENTIAL  */
#line 1802 "parser.y"
                        { current_file->access_mode = COB_ACCESS_SEQUENTIAL; }
#line 6227 "parser.c"
    break;

  case 202: /* access_mode: DYNAMIC  */
#line 1803 "parser.y"
                        { current_file->access_mode = COB_ACCESS_DYNAMIC; }
#line 6233 "parser.c"
    break;

  case 203: /* access_mode: RANDOM  */
#line 1804 "parser.y"
                        { current_file->access_mode = COB_ACCESS_RANDOM; }
#line 6239 "parser.c"
    break;

  case 204: /* alternative_record_key_clause: ALTERNATE RECORD _key _is reference flag_duplicates  */
#line 1812 "parser.y"
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
#line 6263 "parser.c"
    break;

  case 205: /* alternative_record_key_clause: ALTERNATE RECORD _key _is reference key_is_eq split_key_list flag_duplicates  */
#line 1832 "parser.y"
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
#line 6307 "parser.c"
    break;

  case 206: /* $@16: %empty  */
#line 1874 "parser.y"
  {
	key_component_list = NULL;
  }
#line 6315 "parser.c"
    break;

  case 209: /* split_key: reference  */
#line 1883 "parser.y"
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
#line 6332 "parser.c"
    break;

  case 210: /* key_is_eq: %empty  */
#line 1898 "parser.y"
                { yyval = NULL; }
#line 6338 "parser.c"
    break;

  case 211: /* key_is_eq: SOURCE _is  */
#line 1899 "parser.y"
                { yyval = cb_int1; }
#line 6344 "parser.c"
    break;

  case 212: /* key_is_eq: '='  */
#line 1900 "parser.y"
                { yyval = cb_int('='); }
#line 6350 "parser.c"
    break;

  case 213: /* collating_sequence_clause: coll_sequence _is "Identifier"  */
#line 1907 "parser.y"
  {
	PENDING ("COLLATING SEQUENCE");
  }
#line 6358 "parser.c"
    break;

  case 214: /* file_status_clause: file_or_sort STATUS _is reference opt_reference  */
#line 1917 "parser.y"
  {
	current_file->file_status = yyvsp[-1];
	if (yyvsp[0]) {
		PENDING ("2nd FILE STATUS");
	}
  }
#line 6369 "parser.c"
    break;

  case 219: /* lock_mode: MANUAL lock_with  */
#line 1938 "parser.y"
                        { current_file->lock_mode = COB_LOCK_MANUAL; }
#line 6375 "parser.c"
    break;

  case 220: /* lock_mode: AUTOMATIC lock_with  */
#line 1939 "parser.y"
                        { current_file->lock_mode = COB_LOCK_AUTOMATIC; }
#line 6381 "parser.c"
    break;

  case 221: /* lock_mode: EXCLUSIVE  */
#line 1940 "parser.y"
                        { current_file->lock_mode = COB_LOCK_EXCLUSIVE; }
#line 6387 "parser.c"
    break;

  case 224: /* lock_with: WITH LOCK ON MULTIPLE lock_records  */
#line 1946 "parser.y"
  {
	current_file->lock_mode |= COB_LOCK_MULTIPLE;
  }
#line 6395 "parser.c"
    break;

  case 225: /* lock_with: WITH ROLLBACK  */
#line 1949 "parser.y"
                                { PENDING ("WITH ROLLBACK"); }
#line 6401 "parser.c"
    break;

  case 230: /* organization: INDEXED  */
#line 1966 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_INDEXED;
		organized_seen = 1;
	}
  }
#line 6414 "parser.c"
    break;

  case 231: /* organization: RECORD _binary SEQUENTIAL  */
#line 1975 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_SEQUENTIAL;
		organized_seen = 1;
	}
  }
#line 6427 "parser.c"
    break;

  case 232: /* organization: SEQUENTIAL  */
#line 1984 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = cb_default_organization;
		organized_seen = 1;
	}
  }
#line 6440 "parser.c"
    break;

  case 233: /* organization: RELATIVE  */
#line 1993 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_RELATIVE;
		organized_seen = 1;
	}
  }
#line 6453 "parser.c"
    break;

  case 234: /* organization: LINE SEQUENTIAL  */
#line 2002 "parser.y"
  {
	if (organized_seen) {
		cb_error (_("Invalid or duplicate ORGANIZED clause"));
	} else {
		current_file->organization = COB_ORG_LINE_SEQUENTIAL;
		organized_seen = 1;
	}
  }
#line 6466 "parser.c"
    break;

  case 235: /* padding_character_clause: PADDING _character _is reference_or_literal  */
#line 2017 "parser.y"
  {
	cb_verify (cb_padding_character_clause, "PADDING CHARACTER");
  }
#line 6474 "parser.c"
    break;

  case 236: /* record_delimiter_clause: RECORD DELIMITER _is "STANDARD-1"  */
#line 2026 "parser.y"
                                        { /* ignored */ }
#line 6480 "parser.c"
    break;

  case 237: /* record_key_clause: RECORD _key _is reference flag_duplicates  */
#line 2034 "parser.y"
  {

	if(yyvsp[0] == cb_int1) {
		cb_error (_("Record keys with duplicates are not yet supported"));
	}

	current_file->key = yyvsp[-1];
  }
#line 6493 "parser.c"
    break;

  case 238: /* record_key_clause: RECORD _key _is reference key_is_eq split_key_list flag_duplicates  */
#line 2043 "parser.y"
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
#line 6529 "parser.c"
    break;

  case 239: /* relative_key_clause: RELATIVE _key _is reference  */
#line 2080 "parser.y"
                                { current_file->key = yyvsp[0]; }
#line 6535 "parser.c"
    break;

  case 240: /* reserve_clause: RESERVE integer _area  */
#line 2087 "parser.y"
                                { /* ignored */ }
#line 6541 "parser.c"
    break;

  case 241: /* reserve_clause: RESERVE NO  */
#line 2088 "parser.y"
                                { /* ignored */ }
#line 6547 "parser.c"
    break;

  case 242: /* sharing_clause: SHARING _with sharing_option  */
#line 2095 "parser.y"
                                { current_file->sharing = yyvsp[0]; }
#line 6553 "parser.c"
    break;

  case 243: /* sharing_option: ALL _other  */
#line 2099 "parser.y"
                                { yyval = NULL; PENDING ("SHARING ALL OTHER"); }
#line 6559 "parser.c"
    break;

  case 244: /* sharing_option: NO _other  */
#line 2100 "parser.y"
                                { yyval = cb_int1; }
#line 6565 "parser.c"
    break;

  case 245: /* sharing_option: READ ONLY  */
#line 2101 "parser.y"
                                { yyval = cb_int0; }
#line 6571 "parser.c"
    break;

  case 246: /* nominal_key_clause: NOMINAL _key _is reference  */
#line 2107 "parser.y"
                                { PENDING ("NOMINAL KEY"); }
#line 6577 "parser.c"
    break;

  case 257: /* same_clause: SAME same_option _area _for file_name_list  */
#line 2138 "parser.y"
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
#line 6603 "parser.c"
    break;

  case 258: /* same_option: %empty  */
#line 2162 "parser.y"
                                { yyval = cb_int0; }
#line 6609 "parser.c"
    break;

  case 259: /* same_option: RECORD  */
#line 2163 "parser.y"
                                { yyval = cb_int1; }
#line 6615 "parser.c"
    break;

  case 260: /* same_option: SORT  */
#line 2164 "parser.y"
                                { yyval = cb_int2; }
#line 6621 "parser.c"
    break;

  case 261: /* same_option: "SORT-MERGE"  */
#line 2165 "parser.y"
                                { yyval = cb_int2; }
#line 6627 "parser.c"
    break;

  case 262: /* multiple_file_tape_clause: MULTIPLE _file _tape _contains multiple_file_list  */
#line 2172 "parser.y"
  {
	cb_verify (cb_multiple_file_tape_clause, "MULTIPLE FILE TAPE");
  }
#line 6635 "parser.c"
    break;

  case 265: /* multiple_file: file_name multiple_file_position  */
#line 2183 "parser.y"
                                   { }
#line 6641 "parser.c"
    break;

  case 271: /* apply_clause: APPLY "COMMITMENT-CONTROL" _on reference_list  */
#line 2200 "parser.y"
  {
	PENDING ("APPLY COMMITMENT-CONTROL");
  }
#line 6649 "parser.c"
    break;

  case 272: /* apply_clause: APPLY "CYL-OVERFLOW" _of "Literal" TRACKS ON reference_list  */
#line 2204 "parser.y"
  {
	PENDING ("APPLY CYL-OVERFLOW");
  }
#line 6657 "parser.c"
    break;

  case 273: /* apply_clause: APPLY "CORE-INDEX" TO reference ON reference_list  */
#line 2208 "parser.y"
  {
	PENDING ("APPLY CORE-INDEX");
  }
#line 6665 "parser.c"
    break;

  case 274: /* apply_clause: APPLY "FORMS-OVERLAY" TO reference ON reference_list  */
#line 2212 "parser.y"
  {
	PENDING ("APPLY FORMS-OVERLAY");
  }
#line 6673 "parser.c"
    break;

  case 275: /* apply_clause: APPLY "CLOSE-NOFEED" ON reference_list  */
#line 2216 "parser.y"
  {
	PENDING ("APPLY CLOSE-NOFEED");
  }
#line 6681 "parser.c"
    break;

  case 279: /* $@17: %empty  */
#line 2241 "parser.y"
                                { current_storage = CB_STORAGE_FILE; }
#line 6687 "parser.c"
    break;

  case 281: /* $@18: %empty  */
#line 2244 "parser.y"
  {
	/* hack for MF compatibility */
	if (cb_relaxed_syntax_check) {
		cb_warning (_("FILE SECTION header missing - assumed"));
	} else {
		cb_error (_("FILE SECTION header missing"));
	}
	current_storage = CB_STORAGE_FILE;
  }
#line 6701 "parser.c"
    break;

  case 285: /* file_description: file_type file_description_entry record_description_list  */
#line 2263 "parser.y"
  {
	if (yyvsp[0] && yyvsp[0] != cb_error_node) {
		finalize_file (current_file, CB_FIELD (yyvsp[0]));
	} else {
		cb_error (_("RECORD description missing or invalid"));
	}
  }
#line 6713 "parser.c"
    break;

  case 286: /* file_description_sequence_without_type: file_description_entry record_description_list  */
#line 2275 "parser.y"
  {
	if (yyvsp[0] && yyvsp[0] != cb_error_node) {
		finalize_file (current_file, CB_FIELD (yyvsp[0]));
	} else {
		cb_error (_("RECORD description missing or invalid"));
	}
  }
#line 6725 "parser.c"
    break;

  case 288: /* file_type: FD  */
#line 2286 "parser.y"
                               { yyval = cb_int0; }
#line 6731 "parser.c"
    break;

  case 289: /* file_type: SD  */
#line 2287 "parser.y"
                               { yyval = cb_int1; }
#line 6737 "parser.c"
    break;

  case 290: /* @19: %empty  */
#line 2297 "parser.y"
  {
	if (yyvsp[0] == cb_error_node) {
		YYERROR;
	}

	current_file = CB_FILE (cb_ref (yyvsp[0]));
	if (yyvsp[-1] == cb_int1) {
		current_file->organization = COB_ORG_SORT;
	}
  }
#line 6752 "parser.c"
    break;

  case 291: /* file_description_entry: file_name @19 file_description_clause_sequence '.'  */
#line 2308 "parser.y"
  {
	/* Shut up bison */
	dummy_tree = yyvsp[-2];
  }
#line 6761 "parser.c"
    break;

  case 294: /* file_description_clause: _is EXTERNAL  */
#line 2320 "parser.y"
  {
	if (current_file->global) {
		cb_error (_("File cannot have both EXTERNAL and GLOBAL clauses"));
	}
	current_file->external = 1;
  }
#line 6772 "parser.c"
    break;

  case 295: /* file_description_clause: _is GLOBAL  */
#line 2327 "parser.y"
  {
	if (current_file->external) {
		cb_error (_("File cannot have both EXTERNAL and GLOBAL clauses"));
	}
	current_file->global = 1;
  }
#line 6783 "parser.c"
    break;

  case 306: /* block_contains_clause: BLOCK _contains integer opt_to_integer _records_or_characters  */
#line 2350 "parser.y"
  { /* ignored */ }
#line 6789 "parser.c"
    break;

  case 310: /* record_clause: RECORD _contains integer _characters  */
#line 2360 "parser.y"
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
#line 6805 "parser.c"
    break;

  case 311: /* record_clause: RECORD _contains integer TO integer _characters  */
#line 2372 "parser.y"
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
#line 6834 "parser.c"
    break;

  case 312: /* record_clause: RECORD _is VARYING _in _size opt_from_integer opt_to_integer _characters record_depending  */
#line 2398 "parser.y"
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
#line 6859 "parser.c"
    break;

  case 314: /* record_depending: DEPENDING _on reference  */
#line 2422 "parser.y"
  {
	current_file->record_depending = yyvsp[0];
  }
#line 6867 "parser.c"
    break;

  case 315: /* opt_from_integer: %empty  */
#line 2428 "parser.y"
                                { yyval = NULL; }
#line 6873 "parser.c"
    break;

  case 316: /* opt_from_integer: _from integer  */
#line 2429 "parser.y"
                                { yyval = yyvsp[0]; }
#line 6879 "parser.c"
    break;

  case 317: /* opt_to_integer: %empty  */
#line 2433 "parser.y"
                                { yyval = NULL; }
#line 6885 "parser.c"
    break;

  case 318: /* opt_to_integer: TO integer  */
#line 2434 "parser.y"
                                { yyval = yyvsp[0]; }
#line 6891 "parser.c"
    break;

  case 319: /* label_records_clause: LABEL records label_option  */
#line 2442 "parser.y"
  {
	cb_verify (cb_label_records_clause, "LABEL RECORDS");
  }
#line 6899 "parser.c"
    break;

  case 322: /* value_of_clause: VALUE OF "Identifier" _is valueof_name  */
#line 2457 "parser.y"
  {
	cb_verify (cb_value_of_clause, "VALUE OF");
  }
#line 6907 "parser.c"
    break;

  case 323: /* value_of_clause: VALUE OF "FILE-ID" _is valueof_name  */
#line 2461 "parser.y"
  {
	if (!current_file->assign) {
		current_file->assign = cb_build_assignment_name (current_file, yyvsp[0]);
	}
  }
#line 6917 "parser.c"
    break;

  case 326: /* data_records_clause: DATA records no_reference_list  */
#line 2477 "parser.y"
  {
	cb_verify (cb_data_records_clause, "DATA RECORDS");
  }
#line 6925 "parser.c"
    break;

  case 327: /* linage_clause: LINAGE _is reference_or_literal _lines linage_sequence  */
#line 2488 "parser.y"
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
#line 6944 "parser.c"
    break;

  case 333: /* linage_footing: _with FOOTING _at reference_or_literal _lines  */
#line 2516 "parser.y"
  {
	current_file->latfoot = yyvsp[-1];
  }
#line 6952 "parser.c"
    break;

  case 334: /* linage_top: _at TOP reference_or_literal _lines  */
#line 2523 "parser.y"
  {
	current_file->lattop = yyvsp[-1];
  }
#line 6960 "parser.c"
    break;

  case 335: /* linage_bottom: _at BOTTOM reference_or_literal  */
#line 2530 "parser.y"
  {
	current_file->latbot = yyvsp[0];
  }
#line 6968 "parser.c"
    break;

  case 336: /* recording_mode_clause: RECORDING _mode _is "Identifier"  */
#line 2539 "parser.y"
                                { /* ignore */ }
#line 6974 "parser.c"
    break;

  case 337: /* code_set_clause: "CODE-SET" _is "Identifier"  */
#line 2547 "parser.y"
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
#line 6991 "parser.c"
    break;

  case 338: /* report_clause: REPORT _is report_name  */
#line 2565 "parser.y"
  {
	cb_warning (_("file descriptor REPORT IS"));
  }
#line 6999 "parser.c"
    break;

  case 339: /* report_clause: REPORTS _are report_name  */
#line 2569 "parser.y"
  {
	cb_warning (_("file descriptor REPORTS ARE"));
  }
#line 7007 "parser.c"
    break;

  case 341: /* $@20: %empty  */
#line 2580 "parser.y"
                                { current_storage = CB_STORAGE_WORKING; }
#line 7013 "parser.c"
    break;

  case 342: /* working_storage_section: "WORKING-STORAGE" SECTION '.' $@20 record_description_list  */
#line 2582 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->working_storage =
			cb_field_add (current_program->working_storage, CB_FIELD (yyvsp[0]));
	}
  }
#line 7024 "parser.c"
    break;

  case 343: /* record_description_list: %empty  */
#line 2591 "parser.y"
                                { yyval = NULL; }
#line 7030 "parser.c"
    break;

  case 344: /* record_description_list: record_description_list_1  */
#line 2592 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7036 "parser.c"
    break;

  case 345: /* $@21: %empty  */
#line 2596 "parser.y"
  {
	current_field = NULL;
	description_field = NULL;
	cb_clear_real_field ();
  }
#line 7046 "parser.c"
    break;

  case 346: /* record_description_list_1: $@21 record_description_list_2  */
#line 2602 "parser.y"
  {
	struct cb_field *p;

	for (p = description_field; p; p = p->sister) {
		cb_validate_field (p);
	}
	yyval = CB_TREE (description_field);
  }
#line 7059 "parser.c"
    break;

  case 352: /* exec_sql_data_statement: "EXEC SQL statement"  */
#line 2623 "parser.y"
  {
	/* Handle EXEC SQL in DATA DIVISION (BEGIN/END DECLARE SECTION, INCLUDE SQLCA) */
	/* Mark this as an ESQL program so SQLCA will be injected */
	esql_program_seen = 1;
	(void)CB_LITERAL (yyvsp[0])->data;
  }
#line 7070 "parser.c"
    break;

  case 353: /* exec_sql_data_statement: "EXEC SQL statement" '.'  */
#line 2630 "parser.y"
  {
	esql_program_seen = 1;
	(void)CB_LITERAL (yyvsp[-1])->data;
  }
#line 7079 "parser.c"
    break;

  case 355: /* $@22: %empty  */
#line 2639 "parser.y"
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
#line 7095 "parser.c"
    break;

  case 356: /* data_description: level_number entry_name $@22 data_description_clause_sequence _maybe_next_level_number  */
#line 2651 "parser.y"
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
#line 7112 "parser.c"
    break;

  case 357: /* $@23: %empty  */
#line 2664 "parser.y"
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
#line 7128 "parser.c"
    break;

  case 358: /* data_description: level_number_88 entry_name $@23 value_cond_clause  */
#line 2676 "parser.y"
  {
	if (!qualifier) {
		cb_error (_("Item requires a data name"));
	}
	cb_validate_88_item (current_field);
	if (!description_field) {
		description_field = current_field;
	}
	
  }
#line 7143 "parser.c"
    break;

  case 362: /* _maybe_next_level_number: "Literal"  */
#line 2703 "parser.y"
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
#line 7165 "parser.c"
    break;

  case 363: /* entry_name: %empty  */
#line 2724 "parser.y"
  {
	yyval = cb_build_filler ();
	qualifier = NULL;
	non_const_word = 0;
  }
#line 7175 "parser.c"
    break;

  case 364: /* entry_name: FILLER  */
#line 2730 "parser.y"
  {
	yyval = cb_build_filler ();
	qualifier = NULL;
	non_const_word = 0;
  }
#line 7185 "parser.c"
    break;

  case 365: /* entry_name: "Identifier"  */
#line 2736 "parser.y"
  {
	yyval = yyvsp[0];
	qualifier = yyvsp[0];
	non_const_word = 0;
  }
#line 7195 "parser.c"
    break;

  case 366: /* const_name: "Identifier"  */
#line 2745 "parser.y"
  {
	yyval = yyvsp[0];
	qualifier = yyvsp[0];
	non_const_word = 0;
  }
#line 7205 "parser.c"
    break;

  case 368: /* const_global: _is GLOBAL  */
#line 2754 "parser.y"
  {
	current_field->flag_is_global = 1;
	cb_error (_("CONSTANT with GLOBAL clause is not yet supported"));
  }
#line 7214 "parser.c"
    break;

  case 369: /* lit_or_length: literal  */
#line 2761 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7220 "parser.c"
    break;

  case 370: /* lit_or_length: LENGTH _of identifier_1  */
#line 2762 "parser.y"
                                { yyval = cb_build_const_length (yyvsp[0]); }
#line 7226 "parser.c"
    break;

  case 371: /* lit_or_length: "BYTE-LENGTH" _of identifier_1  */
#line 2763 "parser.y"
                                { yyval = cb_build_const_length (yyvsp[0]); }
#line 7232 "parser.c"
    break;

  case 372: /* constant_entry: level_number const_name CONSTANT const_global _as lit_or_length  */
#line 2768 "parser.y"
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
#line 7252 "parser.c"
    break;

  case 373: /* data_description_clause_sequence: %empty  */
#line 2787 "parser.y"
  {
	/* required to check redefines */
	yyval = NULL;
  }
#line 7261 "parser.c"
    break;

  case 374: /* data_description_clause_sequence: data_description_clause_sequence data_description_clause  */
#line 2793 "parser.y"
  {
	/* required to check redefines */
	yyval = cb_true;
  }
#line 7270 "parser.c"
    break;

  case 391: /* redefines_clause: REDEFINES identifier_1  */
#line 2823 "parser.y"
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
#line 7290 "parser.c"
    break;

  case 392: /* external_clause: _is EXTERNAL as_extname  */
#line 2845 "parser.y"
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
#line 7313 "parser.c"
    break;

  case 393: /* as_extname: %empty  */
#line 2866 "parser.y"
                                { current_field->ename = NULL; }
#line 7319 "parser.c"
    break;

  case 394: /* as_extname: AS "Literal"  */
#line 2868 "parser.y"
 {
	struct cb_field *x;

	x = CB_FIELD(cb_build_field (cb_build_reference ((char *)(CB_LITERAL (yyvsp[0])->data))));
	current_field->ename = x->name;
 }
#line 7330 "parser.c"
    break;

  case 395: /* global_clause: _is GLOBAL  */
#line 2880 "parser.y"
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
#line 7348 "parser.c"
    break;

  case 396: /* picture_clause: PICTURE  */
#line 2899 "parser.y"
                                { current_field->pic = CB_PICTURE (yyvsp[0]); }
#line 7354 "parser.c"
    break;

  case 399: /* usage: BINARY  */
#line 2911 "parser.y"
                                { current_field->usage = CB_USAGE_BINARY; }
#line 7360 "parser.c"
    break;

  case 400: /* usage: COMP  */
#line 2912 "parser.y"
                                { current_field->usage = CB_USAGE_BINARY; }
#line 7366 "parser.c"
    break;

  case 401: /* usage: "COMP-1"  */
#line 2914 "parser.y"
{
	current_field->usage = CB_USAGE_FLOAT; 
	cb_error (_("COMP-1 not implemented"));
}
#line 7375 "parser.c"
    break;

  case 402: /* usage: "COMP-2"  */
#line 2919 "parser.y"
{ 
	current_field->usage = CB_USAGE_DOUBLE; 
	cb_error (_("COMP-2 not implemented"));
}
#line 7384 "parser.c"
    break;

  case 403: /* usage: "COMP-3"  */
#line 2923 "parser.y"
                                { current_field->usage = CB_USAGE_PACKED; }
#line 7390 "parser.c"
    break;

  case 404: /* usage: "COMP-4"  */
#line 2924 "parser.y"
                                { current_field->usage = CB_USAGE_BINARY; }
#line 7396 "parser.c"
    break;

  case 405: /* usage: "COMP-5"  */
#line 2925 "parser.y"
                                { current_field->usage = CB_USAGE_COMP_5; }
#line 7402 "parser.c"
    break;

  case 406: /* usage: "COMP-X"  */
#line 2926 "parser.y"
                                { current_field->usage = CB_USAGE_COMP_X; }
#line 7408 "parser.c"
    break;

  case 407: /* usage: DISPLAY  */
#line 2927 "parser.y"
                                { current_field->usage = CB_USAGE_DISPLAY; }
#line 7414 "parser.c"
    break;

  case 408: /* usage: INDEX  */
#line 2928 "parser.y"
                                { current_field->usage = CB_USAGE_INDEX; }
#line 7420 "parser.c"
    break;

  case 409: /* usage: "PACKED-DECIMAL"  */
#line 2929 "parser.y"
                                { current_field->usage = CB_USAGE_PACKED; }
#line 7426 "parser.c"
    break;

  case 410: /* usage: POINTER  */
#line 2931 "parser.y"
  {
	current_field->usage = CB_USAGE_POINTER;
	current_field->flag_is_pointer = 1;
  }
#line 7435 "parser.c"
    break;

  case 411: /* usage: "PROGRAM-POINTER"  */
#line 2936 "parser.y"
  {
	current_field->usage = CB_USAGE_PROGRAM_POINTER;
	current_field->flag_is_pointer = 1;
  }
#line 7444 "parser.c"
    break;

  case 412: /* usage: "SIGNED-SHORT"  */
#line 2940 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_SHORT; }
#line 7450 "parser.c"
    break;

  case 413: /* usage: "SIGNED-INT"  */
#line 2941 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_INT; }
#line 7456 "parser.c"
    break;

  case 414: /* usage: "SIGNED-LONG"  */
#line 2942 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_LONG; }
#line 7462 "parser.c"
    break;

  case 415: /* usage: "UNSIGNED-SHORT"  */
#line 2943 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_SHORT; }
#line 7468 "parser.c"
    break;

  case 416: /* usage: "UNSIGNED-INT"  */
#line 2944 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_INT; }
#line 7474 "parser.c"
    break;

  case 417: /* usage: "UNSIGNED-LONG"  */
#line 2945 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_LONG; }
#line 7480 "parser.c"
    break;

  case 418: /* usage: "BINARY-CHAR" SIGNED  */
#line 2946 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_CHAR; }
#line 7486 "parser.c"
    break;

  case 419: /* usage: "BINARY-CHAR" UNSIGNED  */
#line 2947 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_CHAR; }
#line 7492 "parser.c"
    break;

  case 420: /* usage: "BINARY-CHAR"  */
#line 2948 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_CHAR; }
#line 7498 "parser.c"
    break;

  case 421: /* usage: "BINARY-SHORT" SIGNED  */
#line 2949 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_SHORT; }
#line 7504 "parser.c"
    break;

  case 422: /* usage: "BINARY-SHORT" UNSIGNED  */
#line 2950 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_SHORT; }
#line 7510 "parser.c"
    break;

  case 423: /* usage: "BINARY-SHORT"  */
#line 2951 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_SHORT; }
#line 7516 "parser.c"
    break;

  case 424: /* usage: "BINARY-LONG" SIGNED  */
#line 2952 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_INT; }
#line 7522 "parser.c"
    break;

  case 425: /* usage: "BINARY-LONG" UNSIGNED  */
#line 2953 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_INT; }
#line 7528 "parser.c"
    break;

  case 426: /* usage: "BINARY-LONG"  */
#line 2954 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_INT; }
#line 7534 "parser.c"
    break;

  case 427: /* usage: "BINARY-DOUBLE" SIGNED  */
#line 2955 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_LONG; }
#line 7540 "parser.c"
    break;

  case 428: /* usage: "BINARY-DOUBLE" UNSIGNED  */
#line 2956 "parser.y"
                                { current_field->usage = CB_USAGE_UNSIGNED_LONG; }
#line 7546 "parser.c"
    break;

  case 429: /* usage: "BINARY-DOUBLE"  */
#line 2957 "parser.y"
                                { current_field->usage = CB_USAGE_SIGNED_LONG; }
#line 7552 "parser.c"
    break;

  case 430: /* usage: "BINARY-C-LONG" SIGNED  */
#line 2959 "parser.y"
  {
	if (sizeof(long) == 4) {
		current_field->usage = CB_USAGE_SIGNED_INT;
	} else {
		current_field->usage = CB_USAGE_SIGNED_LONG;
	}
  }
#line 7564 "parser.c"
    break;

  case 431: /* usage: "BINARY-C-LONG" UNSIGNED  */
#line 2967 "parser.y"
  {
	if (sizeof(long) == 4) {
		current_field->usage = CB_USAGE_UNSIGNED_INT;
	} else {
		current_field->usage = CB_USAGE_UNSIGNED_LONG;
	}
  }
#line 7576 "parser.c"
    break;

  case 432: /* usage: "BINARY-C-LONG"  */
#line 2975 "parser.y"
  {
	if (sizeof(long) == 4) {
		current_field->usage = CB_USAGE_SIGNED_INT;
	} else {
		current_field->usage = CB_USAGE_SIGNED_LONG;
	}
  }
#line 7588 "parser.c"
    break;

  case 433: /* usage: NATIONAL  */
#line 2982 "parser.y"
                                { PENDING ("USAGE NATIONAL");}
#line 7594 "parser.c"
    break;

  case 434: /* sign_clause: _sign_is LEADING flag_separate  */
#line 2990 "parser.y"
  {
	current_field->flag_sign_separate = CB_INTEGER (yyvsp[0])->val;
	current_field->flag_sign_leading  = 1;
  }
#line 7603 "parser.c"
    break;

  case 435: /* sign_clause: _sign_is TRAILING flag_separate  */
#line 2995 "parser.y"
  {
	current_field->flag_sign_separate = CB_INTEGER (yyvsp[0])->val;
	current_field->flag_sign_leading  = 0;
  }
#line 7612 "parser.c"
    break;

  case 439: /* occurs_clause: OCCURS integer occurs_to_integer _times occurs_depending occurs_key_spec  */
#line 3012 "parser.y"
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
#line 7629 "parser.c"
    break;

  case 440: /* occurs_to_integer: %empty  */
#line 3027 "parser.y"
                                { yyval = NULL; }
#line 7635 "parser.c"
    break;

  case 441: /* occurs_to_integer: TO integer  */
#line 3028 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7641 "parser.c"
    break;

  case 443: /* occurs_depending: DEPENDING _on reference  */
#line 3033 "parser.y"
  {
	current_field->occurs_depending = yyvsp[0];
  }
#line 7649 "parser.c"
    break;

  case 446: /* occurs_keys: occurs_key_list  */
#line 3042 "parser.y"
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
#line 7674 "parser.c"
    break;

  case 447: /* occurs_key: ascending_or_descending _key _is reference_list  */
#line 3066 "parser.y"
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
#line 7691 "parser.c"
    break;

  case 448: /* occurs_key_list: occurs_key  */
#line 3081 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7697 "parser.c"
    break;

  case 449: /* occurs_key_list: occurs_key_list occurs_key  */
#line 3082 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 7703 "parser.c"
    break;

  case 450: /* ascending_or_descending: ASCENDING  */
#line 3086 "parser.y"
                                { yyval = cb_int (COB_ASCENDING); }
#line 7709 "parser.c"
    break;

  case 451: /* ascending_or_descending: DESCENDING  */
#line 3087 "parser.y"
                                { yyval = cb_int (COB_DESCENDING); }
#line 7715 "parser.c"
    break;

  case 454: /* occurs_indexed: INDEXED _by occurs_index_list  */
#line 3094 "parser.y"
  {
	current_field->index_list = yyvsp[0];
  }
#line 7723 "parser.c"
    break;

  case 455: /* occurs_index_list: occurs_index  */
#line 3100 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 7729 "parser.c"
    break;

  case 456: /* occurs_index_list: occurs_index_list occurs_index  */
#line 3102 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 7735 "parser.c"
    break;

  case 457: /* occurs_index: "Identifier"  */
#line 3107 "parser.y"
  {
	yyval = cb_build_index (yyvsp[0], cb_int1, 1, current_field);
  }
#line 7743 "parser.c"
    break;

  case 458: /* justified_clause: JUSTIFIED _right  */
#line 3116 "parser.y"
                                { current_field->flag_justified = 1; }
#line 7749 "parser.c"
    break;

  case 459: /* synchronized_clause: SYNCHRONIZED left_or_right  */
#line 3123 "parser.y"
                                { current_field->flag_synchronized = 1; }
#line 7755 "parser.c"
    break;

  case 463: /* blank_clause: BLANK _when ZERO  */
#line 3135 "parser.y"
                                { current_field->flag_blank_zero = 1; }
#line 7761 "parser.c"
    break;

  case 464: /* based_clause: BASED  */
#line 3143 "parser.y"
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
#line 7785 "parser.c"
    break;

  case 465: /* value_clause: VALUE _is literal  */
#line 3167 "parser.y"
                                { current_field->values = cb_list_init (yyvsp[0]); }
#line 7791 "parser.c"
    break;

  case 466: /* $@24: %empty  */
#line 3171 "parser.y"
                                { current_field->values = yyvsp[0]; }
#line 7797 "parser.c"
    break;

  case 468: /* value_item_list: value_item  */
#line 3176 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 7803 "parser.c"
    break;

  case 469: /* value_item_list: value_item_list value_item  */
#line 3177 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 7809 "parser.c"
    break;

  case 470: /* value_item: literal  */
#line 3181 "parser.y"
                                { yyval = yyvsp[0]; }
#line 7815 "parser.c"
    break;

  case 471: /* value_item: literal THRU literal  */
#line 3182 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-2], yyvsp[0]); }
#line 7821 "parser.c"
    break;

  case 473: /* false_is: "FALSE" _is literal  */
#line 3187 "parser.y"
  {
	if (current_field->level != 88) {
		cb_error (_("FALSE clause only allowed for 88 level"));
	}
	current_field->false_88 = cb_list_init (yyvsp[0]);
  }
#line 7832 "parser.c"
    break;

  case 474: /* renames_clause: RENAMES qualified_word  */
#line 3200 "parser.y"
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
#line 7848 "parser.c"
    break;

  case 475: /* renames_clause: RENAMES qualified_word THRU qualified_word  */
#line 3212 "parser.y"
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
#line 7867 "parser.c"
    break;

  case 476: /* any_length_clause: ANY LENGTH  */
#line 3232 "parser.y"
  {
	if (current_field->flag_item_based) {
		cb_error (_("BASED and ANY LENGTH are mutually exclusive"));
	} else {
		current_field->flag_any_length = 1;
	}
  }
#line 7879 "parser.c"
    break;

  case 477: /* varying_clause: VARYING  */
#line 3245 "parser.y"
  {
	current_field->flag_varying = 1;
  }
#line 7887 "parser.c"
    break;

  case 479: /* $@25: %empty  */
#line 3256 "parser.y"
  {
	cb_error (_("LOCAL-STORAGE SECTION is not supported"));
	current_storage = CB_STORAGE_LOCAL;
	if (current_program->nested_level) {
		cb_error (_("LOCAL-STORAGE not allowed in nested programs"));
	}
  }
#line 7899 "parser.c"
    break;

  case 480: /* local_storage_section: "LOCAL-STORAGE" SECTION '.' $@25 record_description_list  */
#line 3264 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->local_storage = CB_FIELD (yyvsp[0]);
	}
  }
#line 7909 "parser.c"
    break;

  case 482: /* $@26: %empty  */
#line 3277 "parser.y"
                                { current_storage = CB_STORAGE_LINKAGE; }
#line 7915 "parser.c"
    break;

  case 483: /* linkage_section: LINKAGE SECTION '.' $@26 record_description_list  */
#line 3279 "parser.y"
  {
	if (yyvsp[0]) {
		current_program->linkage_storage = CB_FIELD (yyvsp[0]);
	}
  }
#line 7925 "parser.c"
    break;

  case 485: /* $@27: %empty  */
#line 3292 "parser.y"
  {
	cb_error (_("REPORT SECTION not supported"));
	current_storage = CB_STORAGE_REPORT;
  }
#line 7934 "parser.c"
    break;

  case 492: /* report_description_options: %empty  */
#line 3325 "parser.y"
  {
	cb_warning (_("Report description using defaults"));
  }
#line 7942 "parser.c"
    break;

  case 494: /* report_description_option: _is GLOBAL  */
#line 3333 "parser.y"
  {
	cb_error (_("GLOBAL is not allowed with RD"));
  }
#line 7950 "parser.c"
    break;

  case 503: /* identifier_list: identifier  */
#line 3356 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 7956 "parser.c"
    break;

  case 504: /* identifier_list: identifier_list identifier  */
#line 3357 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 7962 "parser.c"
    break;

  case 526: /* report_group_option: type_clause  */
#line 3413 "parser.y"
              { cb_warning (_("looking for Report line TYPE")); }
#line 7968 "parser.c"
    break;

  case 577: /* $@28: %empty  */
#line 3518 "parser.y"
                                { current_storage = CB_STORAGE_SCREEN; }
#line 7974 "parser.c"
    break;

  case 578: /* screen_section: SCREEN SECTION '.' $@28  */
#line 3519 "parser.y"
  {
	cb_error (_("SCREEN SECTION is not supported"));
  }
#line 7982 "parser.c"
    break;

  case 580: /* $@29: %empty  */
#line 3530 "parser.y"
  {
	if (esql_program_seen) {
		esql_inject_sqlca ();
	}
	current_section = NULL;
	current_paragraph = NULL;
	cb_define_system_name ("CONSOLE");
	cb_define_system_name ("SYSIN");
	cb_define_system_name ("SYSOUT");
	cb_define_system_name ("SYSERR");
	cb_set_in_procedure ();
  }
#line 7999 "parser.c"
    break;

  case 581: /* $@30: %empty  */
#line 3543 "parser.y"
  {
	if (current_program->flag_main && !current_program->flag_chained && yyvsp[-4]) {
		cb_error (_("Executable program requested but PROCEDURE/ENTRY has USING clause"));
	}
	emit_entry (current_program->program_id, 0, yyvsp[-4]); /* main entry point */
	if (current_program->source_name) {
		emit_entry (current_program->source_name, 1, yyvsp[-4]);
	}
  }
#line 8013 "parser.c"
    break;

  case 582: /* procedure_division: PROCEDURE DIVISION procedure_using_chaining procedure_returning '.' $@29 procedure_declaratives $@30 procedure_list  */
#line 3553 "parser.y"
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
#line 8032 "parser.c"
    break;

  case 583: /* procedure_using_chaining: %empty  */
#line 3570 "parser.y"
                                { yyval = NULL; }
#line 8038 "parser.c"
    break;

  case 584: /* $@31: %empty  */
#line 3572 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
	size_mode = CB_SIZE_4;
  }
#line 8047 "parser.c"
    break;

  case 585: /* procedure_using_chaining: USING $@31 procedure_param_list  */
#line 3576 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8053 "parser.c"
    break;

  case 586: /* $@32: %empty  */
#line 3578 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
	current_program->flag_chained = 1;
  }
#line 8062 "parser.c"
    break;

  case 587: /* procedure_using_chaining: CHAINING $@32 procedure_param_list  */
#line 3582 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8068 "parser.c"
    break;

  case 588: /* procedure_param_list: procedure_param  */
#line 3586 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8074 "parser.c"
    break;

  case 589: /* procedure_param_list: procedure_param_list procedure_param  */
#line 3588 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 8080 "parser.c"
    break;

  case 590: /* procedure_param: procedure_type size_optional procedure_optional "Identifier"  */
#line 3593 "parser.y"
  {
	yyval = cb_build_pair (cb_int (call_mode), cb_build_identifier (yyvsp[0]));
	CB_SIZES (yyval) = size_mode;
  }
#line 8089 "parser.c"
    break;

  case 592: /* procedure_type: _by REFERENCE  */
#line 3602 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
  }
#line 8097 "parser.c"
    break;

  case 593: /* procedure_type: _by VALUE  */
#line 3606 "parser.y"
  {
	if (current_program->flag_chained) {
		cb_error (_("BY VALUE not allowed in CHAINED program"));
	} else {
		call_mode = CB_CALL_BY_VALUE;
	}
  }
#line 8109 "parser.c"
    break;

  case 595: /* size_optional: SIZE _is AUTO  */
#line 3618 "parser.y"
  {
	if (call_mode != CB_CALL_BY_VALUE) {
		cb_error (_("SIZE only allowed for BY VALUE items"));
	} else {
		size_mode = CB_SIZE_AUTO;
	}
  }
#line 8121 "parser.c"
    break;

  case 596: /* size_optional: SIZE _is DEFAULT  */
#line 3626 "parser.y"
  {
	if (call_mode != CB_CALL_BY_VALUE) {
		cb_error (_("SIZE only allowed for BY VALUE items"));
	} else {
		size_mode = CB_SIZE_4;
	}
  }
#line 8133 "parser.c"
    break;

  case 597: /* size_optional: UNSIGNED SIZE _is integer  */
#line 3634 "parser.y"
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
#line 8166 "parser.c"
    break;

  case 598: /* size_optional: SIZE _is integer  */
#line 3663 "parser.y"
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
#line 8199 "parser.c"
    break;

  case 600: /* procedure_optional: OPTIONAL  */
#line 3696 "parser.y"
  {
	if (call_mode != CB_CALL_BY_REFERENCE) {
		cb_error (_("OPTIONAL only allowed for BY REFERENCE items"));
	}
  }
#line 8209 "parser.c"
    break;

  case 601: /* procedure_returning: %empty  */
#line 3705 "parser.y"
  {
	if (current_program->prog_type == CB_FUNCTION_TYPE) {
		cb_error (_("RETURNING clause is required for a FUNCTION"));
	}
  }
#line 8219 "parser.c"
    break;

  case 602: /* procedure_returning: RETURNING "Identifier"  */
#line 3711 "parser.y"
  {
	if (cb_ref (yyvsp[0]) != cb_error_node) {
		current_program->returning = yyvsp[0];
		if (cb_field (yyvsp[0])->storage != CB_STORAGE_LINKAGE) {
			cb_error (_("RETURNING item is not defined in LINKAGE SECTION"));
		}
	}
  }
#line 8232 "parser.c"
    break;

  case 604: /* $@33: %empty  */
#line 3722 "parser.y"
                        { in_declaratives = 1; }
#line 8238 "parser.c"
    break;

  case 605: /* procedure_declaratives: DECLARATIVES '.' $@33 procedure_list END DECLARATIVES '.'  */
#line 3725 "parser.y"
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
#line 8260 "parser.c"
    break;

  case 611: /* procedure: statements '.'  */
#line 3758 "parser.y"
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
#line 8280 "parser.c"
    break;

  case 612: /* procedure: error  */
#line 3774 "parser.y"
  {
	check_unreached = 0;
  }
#line 8288 "parser.c"
    break;

  case 613: /* section_header: section_name SECTION opt_segment '.'  */
#line 3786 "parser.y"
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
#line 8320 "parser.c"
    break;

  case 614: /* paragraph_header: "Identifier" '.'  */
#line 3817 "parser.y"
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
#line 8358 "parser.c"
    break;

  case 615: /* invalid_statement: section_name  */
#line 3854 "parser.y"
  {
	non_const_word = 0;
	check_unreached = 0;
	if (yyvsp[0] != cb_error_node) {
		cb_error_x (yyvsp[0], _("Unknown statement '%s'"), CB_NAME (yyvsp[0]));
	}
	YYERROR;
  }
#line 8371 "parser.c"
    break;

  case 616: /* section_name: "Identifier"  */
#line 3865 "parser.y"
                                { yyval = cb_build_section_name (yyvsp[0], 0); }
#line 8377 "parser.c"
    break;

  case 618: /* opt_segment: "Literal"  */
#line 3869 "parser.y"
                                { /* ignore */ }
#line 8383 "parser.c"
    break;

  case 619: /* @34: %empty  */
#line 3878 "parser.y"
  {
	yyval = current_program->exec_list;
	current_program->exec_list = NULL;
  }
#line 8392 "parser.c"
    break;

  case 620: /* @35: %empty  */
#line 3882 "parser.y"
  {
	yyval = CB_TREE (current_statement);
	current_statement = NULL;
  }
#line 8401 "parser.c"
    break;

  case 621: /* statement_list: @34 @35 statements  */
#line 3887 "parser.y"
  {
	yyval = cb_list_reverse (current_program->exec_list);
	current_program->exec_list = yyvsp[-2];
	current_statement = CB_STATEMENT (yyvsp[-1]);
  }
#line 8411 "parser.c"
    break;

  case 622: /* statements: %empty  */
#line 3895 "parser.y"
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
#line 8436 "parser.c"
    break;

  case 674: /* statement: "NEXT SENTENCE"  */
#line 3971 "parser.y"
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
#line 8454 "parser.c"
    break;

  case 675: /* $@36: %empty  */
#line 3993 "parser.y"
  {
	BEGIN_STATEMENT ("ACCEPT", TERM_ACCEPT);
	dispattrs = 0;
	fgc = NULL;
	bgc = NULL;
	scroll = NULL;
  }
#line 8466 "parser.c"
    break;

  case 677: /* accept_body: identifier opt_at_line_column opt_accp_attr on_accp_exception  */
#line 4006 "parser.y"
  {
	cb_emit_accept (yyvsp[-3], yyvsp[-2], fgc, bgc, scroll, dispattrs);
  }
#line 8474 "parser.c"
    break;

  case 678: /* accept_body: identifier FROM ESCAPE KEY  */
#line 4010 "parser.y"
  {
	PENDING ("ACCEPT .. FROM ESCAPE KEY");
  }
#line 8482 "parser.c"
    break;

  case 679: /* accept_body: identifier FROM LINES  */
#line 4014 "parser.y"
  {
	cb_emit_accept_line_or_col (yyvsp[-2], 0);
  }
#line 8490 "parser.c"
    break;

  case 680: /* accept_body: identifier FROM COLUMNS  */
#line 4018 "parser.y"
  {
	cb_emit_accept_line_or_col (yyvsp[-2], 1);
  }
#line 8498 "parser.c"
    break;

  case 681: /* accept_body: identifier FROM DATE  */
#line 4022 "parser.y"
  {
	cb_emit_accept_date (yyvsp[-2]);
  }
#line 8506 "parser.c"
    break;

  case 682: /* accept_body: identifier FROM DATE YYYYMMDD  */
#line 4026 "parser.y"
  {
	cb_emit_accept_date_yyyymmdd (yyvsp[-3]);
  }
#line 8514 "parser.c"
    break;

  case 683: /* accept_body: identifier FROM DAY  */
#line 4030 "parser.y"
  {
	cb_emit_accept_day (yyvsp[-2]);
  }
#line 8522 "parser.c"
    break;

  case 684: /* accept_body: identifier FROM DAY YYYYDDD  */
#line 4034 "parser.y"
  {
	cb_emit_accept_day_yyyyddd (yyvsp[-3]);
  }
#line 8530 "parser.c"
    break;

  case 685: /* accept_body: identifier FROM "DAY-OF-WEEK"  */
#line 4038 "parser.y"
  {
	cb_emit_accept_day_of_week (yyvsp[-2]);
  }
#line 8538 "parser.c"
    break;

  case 686: /* accept_body: identifier FROM TIME  */
#line 4042 "parser.y"
  {
	cb_emit_accept_time (yyvsp[-2]);
  }
#line 8546 "parser.c"
    break;

  case 687: /* accept_body: identifier FROM "COMMAND-LINE"  */
#line 4046 "parser.y"
  {
	cb_emit_accept_command_line (yyvsp[-2]);
  }
#line 8554 "parser.c"
    break;

  case 688: /* accept_body: identifier FROM "ENVIRONMENT-VALUE" on_accp_exception  */
#line 4050 "parser.y"
  {
	cb_emit_accept_environment (yyvsp[-3]);
  }
#line 8562 "parser.c"
    break;

  case 689: /* accept_body: identifier FROM ENVIRONMENT simple_value on_accp_exception  */
#line 4054 "parser.y"
  { 
	cb_emit_get_environment (yyvsp[-1], yyvsp[-4]);
  }
#line 8570 "parser.c"
    break;

  case 690: /* accept_body: identifier FROM "ARGUMENT-NUMBER"  */
#line 4058 "parser.y"
  {
	cb_emit_accept_arg_number (yyvsp[-2]);
  }
#line 8578 "parser.c"
    break;

  case 691: /* accept_body: identifier FROM "ARGUMENT-VALUE" on_accp_exception  */
#line 4062 "parser.y"
  {
	cb_emit_accept_arg_value (yyvsp[-3]);
  }
#line 8586 "parser.c"
    break;

  case 692: /* accept_body: identifier FROM mnemonic_name  */
#line 4066 "parser.y"
  {
	cb_emit_accept_mnemonic (yyvsp[-2], yyvsp[0]);
  }
#line 8594 "parser.c"
    break;

  case 693: /* accept_body: identifier FROM "Identifier"  */
#line 4070 "parser.y"
  {
	cb_emit_accept_name (yyvsp[-2], yyvsp[0]);
  }
#line 8602 "parser.c"
    break;

  case 694: /* opt_at_line_column: %empty  */
#line 4076 "parser.y"
                                { yyval = NULL; }
#line 8608 "parser.c"
    break;

  case 695: /* opt_at_line_column: _at line_number column_number  */
#line 4077 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-1], yyvsp[0]); }
#line 8614 "parser.c"
    break;

  case 696: /* opt_at_line_column: _at column_number line_number  */
#line 4078 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], yyvsp[-1]); }
#line 8620 "parser.c"
    break;

  case 697: /* opt_at_line_column: _at line_number  */
#line 4079 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], NULL); }
#line 8626 "parser.c"
    break;

  case 698: /* opt_at_line_column: _at column_number  */
#line 4080 "parser.y"
                                { yyval = cb_build_pair (NULL, yyvsp[0]); }
#line 8632 "parser.c"
    break;

  case 699: /* opt_at_line_column: AT simple_value  */
#line 4081 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8638 "parser.c"
    break;

  case 700: /* line_number: LINE _number id_or_lit  */
#line 4085 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8644 "parser.c"
    break;

  case 701: /* column_number: COLUMN _number id_or_lit  */
#line 4089 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8650 "parser.c"
    break;

  case 702: /* column_number: POSITION _number id_or_lit  */
#line 4090 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8656 "parser.c"
    break;

  case 707: /* accp_attr: BELL  */
#line 4103 "parser.y"
                { dispattrs |= COB_SCREEN_BELL; }
#line 8662 "parser.c"
    break;

  case 708: /* accp_attr: BLINK  */
#line 4104 "parser.y"
                { dispattrs |= COB_SCREEN_BLINK; }
#line 8668 "parser.c"
    break;

  case 709: /* accp_attr: HIGHLIGHT  */
#line 4105 "parser.y"
                { dispattrs |= COB_SCREEN_HIGHLIGHT; }
#line 8674 "parser.c"
    break;

  case 710: /* accp_attr: LOWLIGHT  */
#line 4106 "parser.y"
                { dispattrs |= COB_SCREEN_LOWLIGHT; }
#line 8680 "parser.c"
    break;

  case 711: /* accp_attr: "REVERSE-VIDEO"  */
#line 4107 "parser.y"
                { dispattrs |= COB_SCREEN_REVERSE; }
#line 8686 "parser.c"
    break;

  case 712: /* accp_attr: UNDERLINE  */
#line 4108 "parser.y"
                { dispattrs |= COB_SCREEN_UNDERLINE; }
#line 8692 "parser.c"
    break;

  case 713: /* accp_attr: OVERLINE  */
#line 4109 "parser.y"
                { dispattrs |= COB_SCREEN_OVERLINE; }
#line 8698 "parser.c"
    break;

  case 714: /* accp_attr: "FOREGROUND-COLOR" _is num_id_or_lit  */
#line 4111 "parser.y"
  {
	fgc = yyvsp[0];
  }
#line 8706 "parser.c"
    break;

  case 715: /* accp_attr: "BACKGROUND-COLOR" _is num_id_or_lit  */
#line 4115 "parser.y"
  {
	bgc = yyvsp[0];
  }
#line 8714 "parser.c"
    break;

  case 716: /* accp_attr: SCROLL UP _opt_scroll_lines  */
#line 4119 "parser.y"
  {
	scroll = yyvsp[0];
  }
#line 8722 "parser.c"
    break;

  case 717: /* accp_attr: SCROLL DOWN _opt_scroll_lines  */
#line 4123 "parser.y"
  {
	dispattrs |= COB_SCREEN_SCROLL_DOWN;
	scroll = yyvsp[0];
  }
#line 8731 "parser.c"
    break;

  case 718: /* accp_attr: AUTO  */
#line 4127 "parser.y"
                { dispattrs |= COB_SCREEN_AUTO; }
#line 8737 "parser.c"
    break;

  case 719: /* accp_attr: FULL  */
#line 4128 "parser.y"
                { dispattrs |= COB_SCREEN_FULL; }
#line 8743 "parser.c"
    break;

  case 720: /* accp_attr: REQUIRED  */
#line 4129 "parser.y"
                { dispattrs |= COB_SCREEN_REQUIRED; }
#line 8749 "parser.c"
    break;

  case 721: /* accp_attr: SECURE  */
#line 4130 "parser.y"
                { dispattrs |= COB_SCREEN_SECURE; }
#line 8755 "parser.c"
    break;

  case 722: /* accp_attr: UPDATE  */
#line 4131 "parser.y"
                { dispattrs |= COB_SCREEN_UPDATE; }
#line 8761 "parser.c"
    break;

  case 723: /* accp_attr: PROMPT  */
#line 4132 "parser.y"
                { dispattrs |= COB_SCREEN_PROMPT; }
#line 8767 "parser.c"
    break;

  case 724: /* end_accept: %empty  */
#line 4136 "parser.y"
                                { terminator_warning (TERM_ACCEPT); }
#line 8773 "parser.c"
    break;

  case 725: /* end_accept: "END-ACCEPT"  */
#line 4137 "parser.y"
                                { terminator_clear (TERM_ACCEPT); }
#line 8779 "parser.c"
    break;

  case 726: /* $@37: %empty  */
#line 4146 "parser.y"
                                { BEGIN_STATEMENT ("ADD", TERM_ADD); }
#line 8785 "parser.c"
    break;

  case 728: /* add_body: x_list TO arithmetic_x_list on_size_error  */
#line 4153 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '+', cb_build_binary_list (yyvsp[-3], '+'));
  }
#line 8793 "parser.c"
    break;

  case 729: /* add_body: x_list add_to GIVING arithmetic_x_list on_size_error  */
#line 4157 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_list (yyvsp[-4], '+'));
  }
#line 8801 "parser.c"
    break;

  case 730: /* add_body: CORRESPONDING identifier TO identifier flag_rounded on_size_error  */
#line 4161 "parser.y"
  {
	cb_emit_corresponding (cb_build_add, yyvsp[-2], yyvsp[-4], yyvsp[-1]);
  }
#line 8809 "parser.c"
    break;

  case 732: /* add_to: TO x  */
#line 4167 "parser.y"
                                { cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 8815 "parser.c"
    break;

  case 733: /* end_add: %empty  */
#line 4171 "parser.y"
                                { terminator_warning (TERM_ADD); }
#line 8821 "parser.c"
    break;

  case 734: /* end_add: "END-ADD"  */
#line 4172 "parser.y"
                                { terminator_clear (TERM_ADD); }
#line 8827 "parser.c"
    break;

  case 735: /* $@38: %empty  */
#line 4181 "parser.y"
                                { BEGIN_STATEMENT ("ALLOCATE", 0); }
#line 8833 "parser.c"
    break;

  case 737: /* allocate_body: "Identifier" flag_initialized allocate_returning  */
#line 4187 "parser.y"
  {
	cb_emit_allocate (yyvsp[-2], yyvsp[0], NULL, yyvsp[-1]);
  }
#line 8841 "parser.c"
    break;

  case 738: /* allocate_body: expr CHARACTERS flag_initialized RETURNING target_x  */
#line 4191 "parser.y"
  {
	cb_emit_allocate (NULL, yyvsp[0], yyvsp[-4], yyvsp[-2]);
  }
#line 8849 "parser.c"
    break;

  case 739: /* allocate_returning: %empty  */
#line 4197 "parser.y"
                                { yyval = NULL; }
#line 8855 "parser.c"
    break;

  case 740: /* allocate_returning: RETURNING target_x  */
#line 4198 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8861 "parser.c"
    break;

  case 741: /* alter_statement: ALTER alter_options  */
#line 4208 "parser.y"
  {
	cb_error (_("ALTER statement is obsolete and unsupported"));
  }
#line 8869 "parser.c"
    break;

  case 746: /* $@39: %empty  */
#line 4226 "parser.y"
                                { BEGIN_STATEMENT ("CALL", TERM_CALL); }
#line 8875 "parser.c"
    break;

  case 747: /* call_statement: CALL $@39 id_or_lit_or_func call_using call_returning call_on_exception call_not_on_exception end_call  */
#line 4230 "parser.y"
  {
	cb_emit_call (yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1]);
  }
#line 8883 "parser.c"
    break;

  case 748: /* call_using: %empty  */
#line 4236 "parser.y"
                                { yyval = NULL; }
#line 8889 "parser.c"
    break;

  case 749: /* $@40: %empty  */
#line 4238 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
	size_mode = CB_SIZE_4;
  }
#line 8898 "parser.c"
    break;

  case 750: /* call_using: USING $@40 call_param_list  */
#line 4242 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8904 "parser.c"
    break;

  case 751: /* call_param_list: call_param  */
#line 4246 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8910 "parser.c"
    break;

  case 752: /* call_param_list: call_param_list call_param  */
#line 4248 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 8916 "parser.c"
    break;

  case 753: /* call_param: call_type OMITTED  */
#line 4253 "parser.y"
  {
	if (call_mode != CB_CALL_BY_REFERENCE) {
		cb_error (_("OMITTED only allowed with BY REFERENCE"));
	}
	yyval = cb_build_pair (cb_int (call_mode), cb_null);
  }
#line 8927 "parser.c"
    break;

  case 754: /* call_param: call_type size_optional x  */
#line 4260 "parser.y"
  {
	yyval = cb_build_pair (cb_int (call_mode), yyvsp[0]);
	CB_SIZES (yyval) = size_mode;
  }
#line 8936 "parser.c"
    break;

  case 756: /* call_type: _by REFERENCE  */
#line 4269 "parser.y"
  {
	call_mode = CB_CALL_BY_REFERENCE;
  }
#line 8944 "parser.c"
    break;

  case 757: /* call_type: _by CONTENT  */
#line 4273 "parser.y"
  {
	if (current_program->flag_chained) {
		cb_error (_("BY CONTENT not allowed in CHAINED program"));
	} else {
		call_mode = CB_CALL_BY_CONTENT;
	}
  }
#line 8956 "parser.c"
    break;

  case 758: /* call_type: _by VALUE  */
#line 4281 "parser.y"
  {
	if (current_program->flag_chained) {
		cb_error (_("BY VALUE not allowed in CHAINED program"));
	} else {
		call_mode = CB_CALL_BY_VALUE;
	}
  }
#line 8968 "parser.c"
    break;

  case 759: /* call_returning: %empty  */
#line 4291 "parser.y"
                                { yyval = NULL; }
#line 8974 "parser.c"
    break;

  case 760: /* call_returning: RETURNING identifier  */
#line 4292 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8980 "parser.c"
    break;

  case 761: /* call_returning: GIVING identifier  */
#line 4293 "parser.y"
                                { yyval = yyvsp[0]; }
#line 8986 "parser.c"
    break;

  case 762: /* call_on_exception: %empty  */
#line 4298 "parser.y"
  {
	yyval = NULL;
  }
#line 8994 "parser.c"
    break;

  case 763: /* $@41: %empty  */
#line 4302 "parser.y"
  {
	check_unreached = 0;
  }
#line 9002 "parser.c"
    break;

  case 764: /* call_on_exception: exception_or_overflow $@41 statement_list  */
#line 4306 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 9010 "parser.c"
    break;

  case 765: /* call_not_on_exception: %empty  */
#line 4313 "parser.y"
  {
	yyval = NULL;
  }
#line 9018 "parser.c"
    break;

  case 766: /* $@42: %empty  */
#line 4317 "parser.y"
  {
	check_unreached = 0;
  }
#line 9026 "parser.c"
    break;

  case 767: /* call_not_on_exception: not_exception_or_overflow $@42 statement_list  */
#line 4321 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 9034 "parser.c"
    break;

  case 768: /* end_call: %empty  */
#line 4327 "parser.y"
                                { terminator_warning (TERM_CALL); }
#line 9040 "parser.c"
    break;

  case 769: /* end_call: "END-CALL"  */
#line 4328 "parser.y"
                                { terminator_clear (TERM_CALL); }
#line 9046 "parser.c"
    break;

  case 770: /* $@43: %empty  */
#line 4337 "parser.y"
                                { BEGIN_STATEMENT ("CANCEL", 0); }
#line 9052 "parser.c"
    break;

  case 773: /* cancel_list: cancel_list id_or_lit  */
#line 4343 "parser.y"
  {
	cb_emit_cancel (yyvsp[0]);
  }
#line 9060 "parser.c"
    break;

  case 774: /* cancel_list: ALL  */
#line 4347 "parser.y"
  {
	cb_emit_cancel_all ();
  }
#line 9068 "parser.c"
    break;

  case 775: /* $@44: %empty  */
#line 4358 "parser.y"
                                { BEGIN_STATEMENT ("CLOSE", 0); }
#line 9074 "parser.c"
    break;

  case 778: /* close_list: close_list file_name close_option  */
#line 4365 "parser.y"
  {
	BEGIN_IMPLICIT_STATEMENT (yyvsp[-1]);
	if (yyvsp[-1] != cb_error_node) {
		cb_emit_close (yyvsp[-1], yyvsp[0]);
	}
  }
#line 9085 "parser.c"
    break;

  case 779: /* close_option: %empty  */
#line 4374 "parser.y"
                                { yyval = cb_int (COB_CLOSE_NORMAL); }
#line 9091 "parser.c"
    break;

  case 780: /* close_option: reel_or_unit  */
#line 4375 "parser.y"
                                { yyval = cb_int (COB_CLOSE_UNIT); }
#line 9097 "parser.c"
    break;

  case 781: /* close_option: reel_or_unit _for REMOVAL  */
#line 4376 "parser.y"
                                { yyval = cb_int (COB_CLOSE_UNIT_REMOVAL); }
#line 9103 "parser.c"
    break;

  case 782: /* close_option: _with NO REWIND  */
#line 4377 "parser.y"
                                { yyval = cb_int (COB_CLOSE_NO_REWIND); }
#line 9109 "parser.c"
    break;

  case 783: /* close_option: _with LOCK  */
#line 4378 "parser.y"
                                { yyval = cb_int (COB_CLOSE_LOCK); }
#line 9115 "parser.c"
    break;

  case 786: /* $@45: %empty  */
#line 4389 "parser.y"
                                { BEGIN_STATEMENT ("COMPUTE", TERM_COMPUTE); }
#line 9121 "parser.c"
    break;

  case 788: /* compute_body: arithmetic_x_list comp_equal expr on_size_error  */
#line 4396 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-3], 0, yyvsp[-1]);
  }
#line 9129 "parser.c"
    break;

  case 789: /* end_compute: %empty  */
#line 4402 "parser.y"
                                { terminator_warning (TERM_COMPUTE); }
#line 9135 "parser.c"
    break;

  case 790: /* end_compute: "END-COMPUTE"  */
#line 4403 "parser.y"
                                { terminator_clear (TERM_COMPUTE); }
#line 9141 "parser.c"
    break;

  case 793: /* commit_statement: COMMIT  */
#line 4414 "parser.y"
  {
	BEGIN_STATEMENT ("COMMIT", 0);
	cb_emit_commit ();
  }
#line 9150 "parser.c"
    break;

  case 794: /* continue_statement: CONTINUE  */
#line 4427 "parser.y"
  {
	BEGIN_STATEMENT ("CONTINUE", 0);
	cb_emit_continue ();
  }
#line 9159 "parser.c"
    break;

  case 795: /* $@46: %empty  */
#line 4439 "parser.y"
                                { BEGIN_STATEMENT ("DELETE", TERM_DELETE); }
#line 9165 "parser.c"
    break;

  case 796: /* delete_statement: DELETE $@46 file_name _record opt_invalid_key end_delete  */
#line 4442 "parser.y"
  {
	if (yyvsp[-3] != cb_error_node) {
		cb_emit_delete (yyvsp[-3]);
	}
  }
#line 9175 "parser.c"
    break;

  case 797: /* end_delete: %empty  */
#line 4450 "parser.y"
                                { terminator_warning (TERM_DELETE); }
#line 9181 "parser.c"
    break;

  case 798: /* end_delete: "END-DELETE"  */
#line 4451 "parser.y"
                                { terminator_clear (TERM_DELETE); }
#line 9187 "parser.c"
    break;

  case 799: /* $@47: %empty  */
#line 4460 "parser.y"
                                  { BEGIN_STATEMENT ("DELETE-FILE", 0); }
#line 9193 "parser.c"
    break;

  case 800: /* delete_file_statement: DELETE $@47 "FILE" file_name_list  */
#line 4462 "parser.y"
  {
	cb_tree l;
	for (l = yyvsp[0]; l; l = CB_CHAIN (l)) {
		if (CB_VALUE (l) != cb_error_node) {
			BEGIN_IMPLICIT_STATEMENT (l);
			cb_emit_delete_file (CB_VALUE (l));
		}
	}
  }
#line 9207 "parser.c"
    break;

  case 801: /* $@48: %empty  */
#line 4480 "parser.y"
  {
	BEGIN_STATEMENT ("DISPLAY", TERM_DISPLAY);
	dispattrs = 0;
	fgc = NULL;
	bgc = NULL;
	scroll = NULL;
  }
#line 9219 "parser.c"
    break;

  case 803: /* display_body: id_or_lit "UPON ENVIRONMENT-NAME" on_disp_exception  */
#line 4493 "parser.y"
  {
	cb_emit_env_name (yyvsp[-2]);
  }
#line 9227 "parser.c"
    break;

  case 804: /* display_body: id_or_lit "UPON ENVIRONMENT-VALUE" on_disp_exception  */
#line 4497 "parser.y"
  {
	cb_emit_env_value (yyvsp[-2]);
  }
#line 9235 "parser.c"
    break;

  case 805: /* display_body: id_or_lit "UPON ARGUMENT-NUMBER" on_disp_exception  */
#line 4501 "parser.y"
  {
	cb_emit_arg_number (yyvsp[-2]);
  }
#line 9243 "parser.c"
    break;

  case 806: /* display_body: id_or_lit "UPON COMMAND-LINE" on_disp_exception  */
#line 4505 "parser.y"
  {
	cb_emit_command_line (yyvsp[-2]);
  }
#line 9251 "parser.c"
    break;

  case 807: /* display_body: x_list opt_at_line_column with_clause on_disp_exception  */
#line 4509 "parser.y"
  {
	cb_emit_display (yyvsp[-3], cb_int0, yyvsp[-1], yyvsp[-2], fgc, bgc, scroll, dispattrs);
  }
#line 9259 "parser.c"
    break;

  case 808: /* display_body: x_list opt_at_line_column UPON mnemonic_name with_clause on_disp_exception  */
#line 4513 "parser.y"
  {
	cb_emit_display_mnemonic (yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9267 "parser.c"
    break;

  case 809: /* display_body: x_list opt_at_line_column UPON "Identifier" with_clause on_disp_exception  */
#line 4517 "parser.y"
  {
	cb_tree word = cb_build_display_upon_direct (yyvsp[-2]);
	cb_emit_display (yyvsp[-5], word, yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9276 "parser.c"
    break;

  case 810: /* display_body: x_list opt_at_line_column UPON PRINTER with_clause on_disp_exception  */
#line 4522 "parser.y"
  {
	cb_emit_display (yyvsp[-5], cb_int0, yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9284 "parser.c"
    break;

  case 811: /* display_body: x_list opt_at_line_column UPON CRT with_clause on_disp_exception  */
#line 4526 "parser.y"
  {
	cb_emit_display (yyvsp[-5], cb_int0, yyvsp[-1], yyvsp[-4], fgc, bgc, scroll, dispattrs);
  }
#line 9292 "parser.c"
    break;

  case 812: /* with_clause: %empty  */
#line 4532 "parser.y"
                                { yyval = cb_int1; }
#line 9298 "parser.c"
    break;

  case 813: /* with_clause: _with "NO ADVANCING"  */
#line 4533 "parser.y"
                                { yyval = cb_int0; }
#line 9304 "parser.c"
    break;

  case 814: /* with_clause: WITH disp_attrs  */
#line 4534 "parser.y"
                                { yyval = cb_int1; }
#line 9310 "parser.c"
    break;

  case 817: /* disp_attr: BELL  */
#line 4544 "parser.y"
                { dispattrs |= COB_SCREEN_BELL; }
#line 9316 "parser.c"
    break;

  case 818: /* disp_attr: BLINK  */
#line 4545 "parser.y"
                { dispattrs |= COB_SCREEN_BLINK; }
#line 9322 "parser.c"
    break;

  case 819: /* disp_attr: ERASE EOL  */
#line 4546 "parser.y"
                { dispattrs |= COB_SCREEN_ERASE_EOL; }
#line 9328 "parser.c"
    break;

  case 820: /* disp_attr: ERASE EOS  */
#line 4547 "parser.y"
                { dispattrs |= COB_SCREEN_ERASE_EOS; }
#line 9334 "parser.c"
    break;

  case 821: /* disp_attr: HIGHLIGHT  */
#line 4548 "parser.y"
                { dispattrs |= COB_SCREEN_HIGHLIGHT; }
#line 9340 "parser.c"
    break;

  case 822: /* disp_attr: LOWLIGHT  */
#line 4549 "parser.y"
                { dispattrs |= COB_SCREEN_LOWLIGHT; }
#line 9346 "parser.c"
    break;

  case 823: /* disp_attr: "REVERSE-VIDEO"  */
#line 4550 "parser.y"
                { dispattrs |= COB_SCREEN_REVERSE; }
#line 9352 "parser.c"
    break;

  case 824: /* disp_attr: UNDERLINE  */
#line 4551 "parser.y"
                { dispattrs |= COB_SCREEN_UNDERLINE; }
#line 9358 "parser.c"
    break;

  case 825: /* disp_attr: OVERLINE  */
#line 4552 "parser.y"
                { dispattrs |= COB_SCREEN_OVERLINE; }
#line 9364 "parser.c"
    break;

  case 826: /* disp_attr: "FOREGROUND-COLOR" _is num_id_or_lit  */
#line 4554 "parser.y"
  {
	fgc = yyvsp[0];
  }
#line 9372 "parser.c"
    break;

  case 827: /* disp_attr: "BACKGROUND-COLOR" _is num_id_or_lit  */
#line 4558 "parser.y"
  {
	bgc = yyvsp[0];
  }
#line 9380 "parser.c"
    break;

  case 828: /* disp_attr: SCROLL UP _opt_scroll_lines  */
#line 4562 "parser.y"
  {
	scroll = yyvsp[0];
  }
#line 9388 "parser.c"
    break;

  case 829: /* disp_attr: SCROLL DOWN _opt_scroll_lines  */
#line 4566 "parser.y"
  {
	dispattrs |= COB_SCREEN_SCROLL_DOWN;
	scroll = yyvsp[0];
  }
#line 9397 "parser.c"
    break;

  case 830: /* disp_attr: "BLANK-LINE"  */
#line 4570 "parser.y"
                { dispattrs |= COB_SCREEN_BLANK_LINE; }
#line 9403 "parser.c"
    break;

  case 831: /* disp_attr: "BLANK-SCREEN"  */
#line 4571 "parser.y"
                { dispattrs |= COB_SCREEN_BLANK_SCREEN; }
#line 9409 "parser.c"
    break;

  case 832: /* end_display: %empty  */
#line 4575 "parser.y"
                                { terminator_warning (TERM_DISPLAY); }
#line 9415 "parser.c"
    break;

  case 833: /* end_display: "END-DISPLAY"  */
#line 4576 "parser.y"
                                { terminator_clear (TERM_DISPLAY); }
#line 9421 "parser.c"
    break;

  case 834: /* $@49: %empty  */
#line 4585 "parser.y"
                                { BEGIN_STATEMENT ("DIVIDE", TERM_DIVIDE); }
#line 9427 "parser.c"
    break;

  case 836: /* divide_body: x INTO arithmetic_x_list on_size_error  */
#line 4592 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '/', yyvsp[-3]);
  }
#line 9435 "parser.c"
    break;

  case 837: /* divide_body: x INTO x GIVING arithmetic_x_list on_size_error  */
#line 4596 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_op (yyvsp[-3], '/', yyvsp[-5]));
  }
#line 9443 "parser.c"
    break;

  case 838: /* divide_body: x BY x GIVING arithmetic_x_list on_size_error  */
#line 4600 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_op (yyvsp[-5], '/', yyvsp[-3]));
  }
#line 9451 "parser.c"
    break;

  case 839: /* divide_body: x INTO x GIVING arithmetic_x REMAINDER arithmetic_x on_size_error  */
#line 4604 "parser.y"
  {
	cb_emit_divide (yyvsp[-5], yyvsp[-7], yyvsp[-3], yyvsp[-1]);
  }
#line 9459 "parser.c"
    break;

  case 840: /* divide_body: x BY x GIVING arithmetic_x REMAINDER arithmetic_x on_size_error  */
#line 4608 "parser.y"
  {
	cb_emit_divide (yyvsp[-7], yyvsp[-5], yyvsp[-3], yyvsp[-1]);
  }
#line 9467 "parser.c"
    break;

  case 841: /* end_divide: %empty  */
#line 4614 "parser.y"
                                { terminator_warning (TERM_DIVIDE); }
#line 9473 "parser.c"
    break;

  case 842: /* end_divide: "END-DIVIDE"  */
#line 4615 "parser.y"
                                { terminator_clear (TERM_DIVIDE); }
#line 9479 "parser.c"
    break;

  case 843: /* $@50: %empty  */
#line 4624 "parser.y"
                                { BEGIN_STATEMENT ("ENTRY", 0); }
#line 9485 "parser.c"
    break;

  case 844: /* entry_statement: ENTRY $@50 "Literal" call_using  */
#line 4626 "parser.y"
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
#line 9501 "parser.c"
    break;

  case 845: /* $@51: %empty  */
#line 4646 "parser.y"
  {
	BEGIN_STATEMENT ("EVALUATE", TERM_EVALUATE);
	eval_level++;
	for (eval_inc = 0; eval_inc < 64; eval_inc++) {
		eval_check[eval_level][eval_inc] = 0;
	}
	eval_inc = 0;
	eval_inc2 = 0;
  }
#line 9515 "parser.c"
    break;

  case 846: /* evaluate_statement: EVALUATE $@51 evaluate_subject_list evaluate_condition_list end_evaluate  */
#line 4657 "parser.y"
  {
	cb_emit_evaluate (yyvsp[-2], yyvsp[-1]);
	eval_level--;
  }
#line 9524 "parser.c"
    break;

  case 847: /* evaluate_subject_list: evaluate_subject  */
#line 4664 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9530 "parser.c"
    break;

  case 848: /* evaluate_subject_list: evaluate_subject_list _also evaluate_subject  */
#line 4667 "parser.y"
  {
 	if (!cb_allow_missing_also_clause_in_evaluate && yyvsp[-1] != cb_int1) {
 		cb_error  (_("Invalid expression"));
 	}
 	yyval = cb_list_add (yyvsp[-2], yyvsp[0]);
  }
#line 9541 "parser.c"
    break;

  case 849: /* evaluate_subject: expr  */
#line 4677 "parser.y"
  {
	yyval = yyvsp[0];
	if (CB_REFERENCE_P (yyvsp[0])) {
		eval_check[eval_level][eval_inc++] = 0;
	} else {
		eval_check[eval_level][eval_inc++] = 1;
	}
  }
#line 9554 "parser.c"
    break;

  case 850: /* evaluate_subject: "TRUE"  */
#line 4686 "parser.y"
  {
	yyval = cb_true;
	eval_check[eval_level][eval_inc++] = 2;
  }
#line 9563 "parser.c"
    break;

  case 851: /* evaluate_subject: "FALSE"  */
#line 4691 "parser.y"
  {
	yyval = cb_false;
	eval_check[eval_level][eval_inc++] = 3;
  }
#line 9572 "parser.c"
    break;

  case 852: /* evaluate_condition_list: evaluate_case_list evaluate_other  */
#line 4699 "parser.y"
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
#line 9602 "parser.c"
    break;

  case 853: /* evaluate_case_list: evaluate_case  */
#line 4727 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9608 "parser.c"
    break;

  case 854: /* evaluate_case_list: evaluate_case_list evaluate_case  */
#line 4729 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 9614 "parser.c"
    break;

  case 855: /* $@52: %empty  */
#line 4734 "parser.y"
  {
	check_unreached = 0;
  }
#line 9622 "parser.c"
    break;

  case 856: /* evaluate_case: evaluate_when_list $@52 statement_list  */
#line 4738 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[0] == NULL) {
		cb_error (_("syntax error"));
	}
	yyval = cb_cons (yyvsp[0], yyvsp[-2]);
	eval_inc2 = 0;
  }
#line 9634 "parser.c"
    break;

  case 857: /* evaluate_other: %empty  */
#line 4749 "parser.y"
  {
	yyval = NULL;
  }
#line 9642 "parser.c"
    break;

  case 858: /* $@53: %empty  */
#line 4753 "parser.y"
  {
	check_unreached = 0;
  }
#line 9650 "parser.c"
    break;

  case 859: /* evaluate_other: "WHEN OTHER" $@53 statement_list  */
#line 4757 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[0] == NULL) {
		cb_error (_("syntax error"));
	}
	yyval = cb_cons (yyvsp[0], NULL);
	eval_inc2 = 0;
  }
#line 9662 "parser.c"
    break;

  case 860: /* evaluate_when_list: WHEN evaluate_object_list  */
#line 4767 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9668 "parser.c"
    break;

  case 861: /* evaluate_when_list: evaluate_when_list WHEN evaluate_object_list  */
#line 4769 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 9674 "parser.c"
    break;

  case 862: /* evaluate_object_list: evaluate_object  */
#line 4773 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 9680 "parser.c"
    break;

  case 863: /* evaluate_object_list: evaluate_object_list _also evaluate_object  */
#line 4776 "parser.y"
  {
 	if (!cb_allow_missing_also_clause_in_evaluate && yyvsp[-1] != cb_int1) {
 		cb_error  (_("Invalid expression"));
 	}
 	yyval = cb_list_add (yyvsp[-2], yyvsp[0]);
  }
#line 9691 "parser.c"
    break;

  case 864: /* evaluate_object: partial_expr opt_evaluate_thru_expr  */
#line 4786 "parser.y"
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
#line 9724 "parser.c"
    break;

  case 865: /* evaluate_object: ANY  */
#line 4814 "parser.y"
                                { yyval = cb_any; eval_inc2++; }
#line 9730 "parser.c"
    break;

  case 866: /* evaluate_object: "TRUE"  */
#line 4815 "parser.y"
                                { yyval = cb_true; eval_inc2++; }
#line 9736 "parser.c"
    break;

  case 867: /* evaluate_object: "FALSE"  */
#line 4816 "parser.y"
                                { yyval = cb_false; eval_inc2++; }
#line 9742 "parser.c"
    break;

  case 868: /* opt_evaluate_thru_expr: %empty  */
#line 4819 "parser.y"
                                { yyval = NULL; }
#line 9748 "parser.c"
    break;

  case 869: /* opt_evaluate_thru_expr: THRU expr  */
#line 4820 "parser.y"
                                { yyval = yyvsp[0]; }
#line 9754 "parser.c"
    break;

  case 870: /* end_evaluate: %empty  */
#line 4824 "parser.y"
                                { terminator_warning (TERM_EVALUATE); }
#line 9760 "parser.c"
    break;

  case 871: /* end_evaluate: "END-EVALUATE"  */
#line 4825 "parser.y"
                                { terminator_clear (TERM_EVALUATE); }
#line 9766 "parser.c"
    break;

  case 872: /* $@54: %empty  */
#line 4834 "parser.y"
                                { BEGIN_STATEMENT ("EXIT", 0); }
#line 9772 "parser.c"
    break;

  case 874: /* exit_body: %empty  */
#line 4839 "parser.y"
                                { /* nothing */ }
#line 9778 "parser.c"
    break;

  case 875: /* exit_body: PROGRAM  */
#line 4841 "parser.y"
  {
	if (in_declaratives && use_global_ind) {
		cb_error (_("EXIT PROGRAM is not allowed within a USE GLOBAL procedure"));
	}
	check_unreached = 1;
	cb_emit_exit (0);
  }
#line 9790 "parser.c"
    break;

  case 876: /* exit_body: PERFORM  */
#line 4849 "parser.y"
  {
	if (!perform_stack) {
		cb_error (_("EXIT PERFORM is only valid with inline PERFORM"));
	} else {
		cb_emit_java_break ();
	}
  }
#line 9802 "parser.c"
    break;

  case 877: /* exit_body: PERFORM CYCLE  */
#line 4857 "parser.y"
  {
	if (!perform_stack) {
		cb_error (_("EXIT PERFORM is only valid with inline PERFORM"));
	} else {
		cb_emit_java_continue ();
	}
  }
#line 9814 "parser.c"
    break;

  case 878: /* exit_body: SECTION  */
#line 4865 "parser.y"
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
#line 9836 "parser.c"
    break;

  case 879: /* exit_body: PARAGRAPH  */
#line 4883 "parser.y"
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
#line 9858 "parser.c"
    break;

  case 880: /* $@55: %empty  */
#line 4907 "parser.y"
                                { BEGIN_STATEMENT ("FREE", 0); }
#line 9864 "parser.c"
    break;

  case 881: /* free_statement: FREE $@55 target_x_list  */
#line 4909 "parser.y"
  {
	cb_emit_free (yyvsp[0]);
  }
#line 9872 "parser.c"
    break;

  case 882: /* $@56: %empty  */
#line 4920 "parser.y"
                                { BEGIN_STATEMENT ("GENERATE", 0); }
#line 9878 "parser.c"
    break;

  case 883: /* generate_statement: GENERATE $@56 identifier  */
#line 4922 "parser.y"
  {
	PENDING("GENERATE");
  }
#line 9886 "parser.c"
    break;

  case 884: /* $@57: %empty  */
#line 4933 "parser.y"
                                { BEGIN_STATEMENT ("GO TO", 0); }
#line 9892 "parser.c"
    break;

  case 885: /* goto_statement: GO _to $@57 procedure_name_list goto_depending  */
#line 4935 "parser.y"
  {
	cb_emit_goto (yyvsp[-1], yyvsp[0]);
  }
#line 9900 "parser.c"
    break;

  case 886: /* goto_depending: %empty  */
#line 4942 "parser.y"
  {
	check_unreached = 1;
	yyval = NULL;
  }
#line 9909 "parser.c"
    break;

  case 887: /* goto_depending: DEPENDING _on identifier  */
#line 4947 "parser.y"
  {
	check_unreached = 0;
	yyval = yyvsp[0];
  }
#line 9918 "parser.c"
    break;

  case 888: /* $@58: %empty  */
#line 4959 "parser.y"
                                { BEGIN_STATEMENT ("GOBACK", 0); }
#line 9924 "parser.c"
    break;

  case 889: /* goback_statement: GOBACK $@58  */
#line 4960 "parser.y"
  {
	check_unreached = 1;
	cb_emit_exit (1);
  }
#line 9933 "parser.c"
    break;

  case 890: /* $@59: %empty  */
#line 4972 "parser.y"
                                { BEGIN_STATEMENT ("IF", TERM_IF); }
#line 9939 "parser.c"
    break;

  case 891: /* $@60: %empty  */
#line 4974 "parser.y"
  {
	check_unreached = 0;
  }
#line 9947 "parser.c"
    break;

  case 892: /* if_statement: IF $@59 condition _then $@60 statement_list if_else_sentence end_if  */
#line 4979 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[-2] == NULL) {
		cb_error (_("syntax error"));
	}
	cb_emit_if (yyvsp[-5], yyvsp[-2], yyvsp[-1]);
  }
#line 9958 "parser.c"
    break;

  case 894: /* if_else_sentence: %empty  */
#line 4990 "parser.y"
  {
	yyval = NULL;
  }
#line 9966 "parser.c"
    break;

  case 895: /* $@61: %empty  */
#line 4994 "parser.y"
  {
	check_unreached = 0;
  }
#line 9974 "parser.c"
    break;

  case 896: /* if_else_sentence: ELSE $@61 statement_list  */
#line 4998 "parser.y"
  {
	if (!cb_allow_empty_imperative_statement && yyvsp[0] == NULL) {
		cb_error (_("syntax error"));
	}
	yyval = yyvsp[0];
  }
#line 9985 "parser.c"
    break;

  case 897: /* end_if: %empty  */
#line 5007 "parser.y"
                                { terminator_warning (TERM_IF); }
#line 9991 "parser.c"
    break;

  case 898: /* end_if: "END-IF"  */
#line 5008 "parser.y"
                                { terminator_clear (TERM_IF); }
#line 9997 "parser.c"
    break;

  case 899: /* $@62: %empty  */
#line 5017 "parser.y"
                                { BEGIN_STATEMENT ("INITIALIZE", 0); }
#line 10003 "parser.c"
    break;

  case 900: /* initialize_statement: INITIALIZE $@62 target_x_list initialize_filler initialize_value initialize_replacing initialize_default  */
#line 5019 "parser.y"
  {
	cb_emit_initialize (yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]);
  }
#line 10011 "parser.c"
    break;

  case 901: /* initialize_filler: %empty  */
#line 5025 "parser.y"
                                { yyval = NULL; }
#line 10017 "parser.c"
    break;

  case 902: /* initialize_filler: _with FILLER  */
#line 5026 "parser.y"
                                { yyval = cb_true; }
#line 10023 "parser.c"
    break;

  case 903: /* initialize_value: %empty  */
#line 5030 "parser.y"
                                { yyval = NULL; }
#line 10029 "parser.c"
    break;

  case 904: /* initialize_value: ALL _to VALUE  */
#line 5031 "parser.y"
                                { yyval = cb_true; }
#line 10035 "parser.c"
    break;

  case 905: /* initialize_value: initialize_category _to VALUE  */
#line 5032 "parser.y"
                                { yyval = yyvsp[-2]; }
#line 10041 "parser.c"
    break;

  case 906: /* initialize_replacing: %empty  */
#line 5036 "parser.y"
                                { yyval = NULL; }
#line 10047 "parser.c"
    break;

  case 907: /* initialize_replacing: REPLACING initialize_replacing_list  */
#line 5038 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10053 "parser.c"
    break;

  case 908: /* initialize_replacing_list: initialize_replacing_item  */
#line 5042 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10059 "parser.c"
    break;

  case 909: /* initialize_replacing_list: initialize_replacing_list initialize_replacing_item  */
#line 5044 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 10065 "parser.c"
    break;

  case 910: /* initialize_replacing_item: initialize_category _data BY x  */
#line 5048 "parser.y"
                                 { yyval = cb_build_pair (yyvsp[-3], yyvsp[0]); }
#line 10071 "parser.c"
    break;

  case 911: /* initialize_category: ALPHABETIC  */
#line 5052 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_ALPHABETIC); }
#line 10077 "parser.c"
    break;

  case 912: /* initialize_category: ALPHANUMERIC  */
#line 5053 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_ALPHANUMERIC); }
#line 10083 "parser.c"
    break;

  case 913: /* initialize_category: NUMERIC  */
#line 5054 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NUMERIC); }
#line 10089 "parser.c"
    break;

  case 914: /* initialize_category: "ALPHANUMERIC-EDITED"  */
#line 5055 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_ALPHANUMERIC_EDITED); }
#line 10095 "parser.c"
    break;

  case 915: /* initialize_category: "NUMERIC-EDITED"  */
#line 5056 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NUMERIC_EDITED); }
#line 10101 "parser.c"
    break;

  case 916: /* initialize_category: NATIONAL  */
#line 5057 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NATIONAL); }
#line 10107 "parser.c"
    break;

  case 917: /* initialize_category: "NATIONAL-EDITED"  */
#line 5058 "parser.y"
                        { yyval = cb_int (CB_CATEGORY_NATIONAL_EDITED); }
#line 10113 "parser.c"
    break;

  case 918: /* initialize_default: %empty  */
#line 5062 "parser.y"
                                { yyval = NULL; }
#line 10119 "parser.c"
    break;

  case 919: /* initialize_default: DEFAULT  */
#line 5063 "parser.y"
                                { yyval = cb_true; }
#line 10125 "parser.c"
    break;

  case 920: /* $@63: %empty  */
#line 5072 "parser.y"
                                { BEGIN_STATEMENT ("INITIATE", 0); }
#line 10131 "parser.c"
    break;

  case 921: /* initiate_statement: INITIATE $@63 identifier_list  */
#line 5074 "parser.y"
  {
	PENDING("INITIATE");
  }
#line 10139 "parser.c"
    break;

  case 922: /* $@64: %empty  */
#line 5085 "parser.y"
  {
	BEGIN_STATEMENT ("INSPECT", 0);
	sending_id = 0;
	inspect_keyword = 0;
  }
#line 10149 "parser.c"
    break;

  case 924: /* send_identifier: identifier  */
#line 5094 "parser.y"
                                { save_tree_1 = yyvsp[0]; sending_id = 0; }
#line 10155 "parser.c"
    break;

  case 925: /* send_identifier: literal  */
#line 5095 "parser.y"
                                { save_tree_1 = yyvsp[0]; sending_id = 1; }
#line 10161 "parser.c"
    break;

  case 926: /* send_identifier: function  */
#line 5096 "parser.y"
                                { save_tree_1 = yyvsp[0]; sending_id = 1; }
#line 10167 "parser.c"
    break;

  case 929: /* inspect_item: inspect_tallying  */
#line 5105 "parser.y"
                                { cb_emit_inspect (save_tree_1, yyvsp[0], cb_int0, 0); }
#line 10173 "parser.c"
    break;

  case 930: /* inspect_item: inspect_replacing  */
#line 5106 "parser.y"
                                { cb_emit_inspect (save_tree_1, yyvsp[0], cb_int1, 1); }
#line 10179 "parser.c"
    break;

  case 931: /* inspect_item: inspect_converting  */
#line 5107 "parser.y"
                                { cb_emit_inspect (save_tree_1, yyvsp[0], cb_int0, 2); }
#line 10185 "parser.c"
    break;

  case 932: /* $@65: %empty  */
#line 5113 "parser.y"
                                { cb_init_tarrying (); }
#line 10191 "parser.c"
    break;

  case 933: /* inspect_tallying: TALLYING $@65 tallying_list  */
#line 5114 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10197 "parser.c"
    break;

  case 934: /* tallying_list: tallying_item  */
#line 5118 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10203 "parser.c"
    break;

  case 935: /* tallying_list: tallying_list tallying_item  */
#line 5119 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 10209 "parser.c"
    break;

  case 936: /* tallying_item: simple_value FOR  */
#line 5123 "parser.y"
                                { yyval = cb_build_tarrying_data (yyvsp[-1]); }
#line 10215 "parser.c"
    break;

  case 937: /* tallying_item: CHARACTERS inspect_region  */
#line 5124 "parser.y"
                                { yyval = cb_build_tarrying_characters (yyvsp[0]); }
#line 10221 "parser.c"
    break;

  case 938: /* tallying_item: ALL  */
#line 5125 "parser.y"
                                { yyval = cb_build_tarrying_all (); }
#line 10227 "parser.c"
    break;

  case 939: /* tallying_item: LEADING  */
#line 5126 "parser.y"
                                { yyval = cb_build_tarrying_leading (); }
#line 10233 "parser.c"
    break;

  case 940: /* tallying_item: TRAILING  */
#line 5127 "parser.y"
                                { yyval = cb_build_tarrying_trailing (); }
#line 10239 "parser.c"
    break;

  case 941: /* tallying_item: simple_value inspect_region  */
#line 5128 "parser.y"
                                { yyval = cb_build_tarrying_value (yyvsp[-1], yyvsp[0]); }
#line 10245 "parser.c"
    break;

  case 942: /* inspect_replacing: REPLACING replacing_list  */
#line 5134 "parser.y"
                                { yyval = yyvsp[0]; inspect_keyword = 0; }
#line 10251 "parser.c"
    break;

  case 943: /* replacing_list: replacing_item  */
#line 5138 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10257 "parser.c"
    break;

  case 944: /* replacing_list: replacing_list replacing_item  */
#line 5139 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 10263 "parser.c"
    break;

  case 945: /* replacing_item: CHARACTERS BY simple_value inspect_region  */
#line 5144 "parser.y"
  {
	yyval = cb_build_replacing_characters (yyvsp[-1], yyvsp[0], save_tree_1);
	inspect_keyword = 0;
  }
#line 10272 "parser.c"
    break;

  case 946: /* replacing_item: rep_keyword replacing_region  */
#line 5148 "parser.y"
                                        { yyval = yyvsp[0]; }
#line 10278 "parser.c"
    break;

  case 947: /* rep_keyword: %empty  */
#line 5152 "parser.y"
                                { /* Nothing */ }
#line 10284 "parser.c"
    break;

  case 948: /* rep_keyword: ALL  */
#line 5153 "parser.y"
                                { inspect_keyword = 1; }
#line 10290 "parser.c"
    break;

  case 949: /* rep_keyword: LEADING  */
#line 5154 "parser.y"
                                { inspect_keyword = 2; }
#line 10296 "parser.c"
    break;

  case 950: /* rep_keyword: FIRST  */
#line 5155 "parser.y"
                                { inspect_keyword = 3; }
#line 10302 "parser.c"
    break;

  case 951: /* rep_keyword: TRAILING  */
#line 5156 "parser.y"
                                { inspect_keyword = 4; }
#line 10308 "parser.c"
    break;

  case 952: /* replacing_region: simple_value BY simple_all_value inspect_region  */
#line 5161 "parser.y"
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
#line 10333 "parser.c"
    break;

  case 953: /* inspect_converting: CONVERTING simple_value TO simple_all_value inspect_region  */
#line 5187 "parser.y"
  {
	if (cb_validate_inspect (save_tree_1, yyvsp[-3], yyvsp[-1]) < 0 ) {
		yyval = cb_error_node;
	} else {
		yyval = cb_build_converting (yyvsp[-3], yyvsp[-1], yyvsp[0]);
	}
  }
#line 10345 "parser.c"
    break;

  case 954: /* inspect_region: %empty  */
#line 5199 "parser.y"
                                { yyval = cb_build_inspect_region_start (); }
#line 10351 "parser.c"
    break;

  case 955: /* inspect_region: inspect_region before_or_after _initial x  */
#line 5201 "parser.y"
                                { yyval = cb_build_inspect_region (yyvsp[-3], yyvsp[-2], yyvsp[0]); }
#line 10357 "parser.c"
    break;

  case 958: /* $@66: %empty  */
#line 5212 "parser.y"
                                { BEGIN_STATEMENT ("MERGE", 0); }
#line 10363 "parser.c"
    break;

  case 960: /* $@67: %empty  */
#line 5222 "parser.y"
                                { BEGIN_STATEMENT ("MOVE", 0); }
#line 10369 "parser.c"
    break;

  case 962: /* move_body: x TO target_x_list  */
#line 5228 "parser.y"
  {
	cb_emit_move (yyvsp[-2], yyvsp[0]);
  }
#line 10377 "parser.c"
    break;

  case 963: /* move_body: CORRESPONDING x TO target_x_list  */
#line 5232 "parser.y"
  {
	cb_emit_move_corresponding (yyvsp[-2], yyvsp[0]);
  }
#line 10385 "parser.c"
    break;

  case 964: /* $@68: %empty  */
#line 5243 "parser.y"
                                { BEGIN_STATEMENT ("MULTIPLY", TERM_MULTIPLY); }
#line 10391 "parser.c"
    break;

  case 966: /* multiply_body: x BY arithmetic_x_list on_size_error  */
#line 5250 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '*', yyvsp[-3]);
  }
#line 10399 "parser.c"
    break;

  case 967: /* multiply_body: x BY x GIVING arithmetic_x_list on_size_error  */
#line 5254 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_op (yyvsp[-5], '*', yyvsp[-3]));
  }
#line 10407 "parser.c"
    break;

  case 968: /* end_multiply: %empty  */
#line 5260 "parser.y"
                                { terminator_warning (TERM_MULTIPLY); }
#line 10413 "parser.c"
    break;

  case 969: /* end_multiply: "END-MULTIPLY"  */
#line 5261 "parser.y"
                                { terminator_clear (TERM_MULTIPLY); }
#line 10419 "parser.c"
    break;

  case 970: /* $@69: %empty  */
#line 5270 "parser.y"
                                { BEGIN_STATEMENT ("OPEN", 0); }
#line 10425 "parser.c"
    break;

  case 973: /* open_list: open_list open_mode open_sharing file_name_list open_option  */
#line 5277 "parser.y"
  {
	cb_tree l;
	for (l = yyvsp[-1]; l; l = CB_CHAIN (l)) {
		if (CB_VALUE (l) != cb_error_node) {
			BEGIN_IMPLICIT_STATEMENT (l);
			cb_emit_open (CB_VALUE (l), yyvsp[-3], yyvsp[-2]);
		}
	}
  }
#line 10439 "parser.c"
    break;

  case 974: /* open_mode: INPUT  */
#line 5289 "parser.y"
                                { yyval = cb_int (COB_OPEN_INPUT); }
#line 10445 "parser.c"
    break;

  case 975: /* open_mode: OUTPUT  */
#line 5290 "parser.y"
                                { yyval = cb_int (COB_OPEN_OUTPUT); }
#line 10451 "parser.c"
    break;

  case 976: /* open_mode: "I-O"  */
#line 5291 "parser.y"
                                { yyval = cb_int (COB_OPEN_I_O); }
#line 10457 "parser.c"
    break;

  case 977: /* open_mode: EXTEND  */
#line 5292 "parser.y"
                                { yyval = cb_int (COB_OPEN_EXTEND); }
#line 10463 "parser.c"
    break;

  case 978: /* open_sharing: %empty  */
#line 5296 "parser.y"
                                { yyval = NULL; }
#line 10469 "parser.c"
    break;

  case 979: /* open_sharing: SHARING _with sharing_option  */
#line 5297 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10475 "parser.c"
    break;

  case 980: /* open_option: %empty  */
#line 5301 "parser.y"
                                { yyval = NULL; }
#line 10481 "parser.c"
    break;

  case 981: /* open_option: _with NO REWIND  */
#line 5302 "parser.y"
                                { yyval = NULL; }
#line 10487 "parser.c"
    break;

  case 982: /* open_option: _with LOCK  */
#line 5303 "parser.y"
                                { PENDING ("OPEN ... WITH LOCK"); }
#line 10493 "parser.c"
    break;

  case 983: /* $@70: %empty  */
#line 5315 "parser.y"
                                { BEGIN_STATEMENT ("PERFORM", TERM_PERFORM); }
#line 10499 "parser.c"
    break;

  case 985: /* perform_body: perform_procedure perform_option  */
#line 5321 "parser.y"
  {
	cb_emit_perform (yyvsp[0], yyvsp[-1]);
  }
#line 10507 "parser.c"
    break;

  case 986: /* $@71: %empty  */
#line 5325 "parser.y"
  {
	perform_stack = cb_cons (yyvsp[0], perform_stack);
	check_unreached = 0;
  }
#line 10516 "parser.c"
    break;

  case 987: /* perform_body: perform_option $@71 statement_list end_perform  */
#line 5330 "parser.y"
  {
	perform_stack = CB_CHAIN (perform_stack);
	cb_emit_perform (yyvsp[-3], yyvsp[-1]);
  }
#line 10525 "parser.c"
    break;

  case 988: /* perform_body: perform_option "END-PERFORM"  */
#line 5335 "parser.y"
  {
	cb_emit_perform (yyvsp[-1], NULL);
  }
#line 10533 "parser.c"
    break;

  case 989: /* end_perform: %empty  */
#line 5341 "parser.y"
                                { terminator_error (); }
#line 10539 "parser.c"
    break;

  case 990: /* end_perform: "END-PERFORM"  */
#line 5342 "parser.y"
                                { terminator_clear (TERM_PERFORM); }
#line 10545 "parser.c"
    break;

  case 991: /* perform_procedure: procedure_name  */
#line 5347 "parser.y"
  {
	CB_REFERENCE (yyvsp[0])->length = cb_true; /* return from $1 */
	yyval = cb_build_pair (yyvsp[0], yyvsp[0]);
  }
#line 10554 "parser.c"
    break;

  case 992: /* perform_procedure: procedure_name THRU procedure_name  */
#line 5352 "parser.y"
  {
	CB_REFERENCE (yyvsp[0])->length = cb_true; /* return from $3 */
	yyval = cb_build_pair (yyvsp[-2], yyvsp[0]);
  }
#line 10563 "parser.c"
    break;

  case 993: /* perform_option: %empty  */
#line 5360 "parser.y"
  {
	yyval = cb_build_perform_once (NULL);
  }
#line 10571 "parser.c"
    break;

  case 994: /* perform_option: FOREVER  */
#line 5364 "parser.y"
  {
	yyval = cb_build_perform_forever (NULL);
  }
#line 10579 "parser.c"
    break;

  case 995: /* perform_option: id_or_lit_or_func TIMES  */
#line 5368 "parser.y"
  {
	yyval = cb_build_perform_times (yyvsp[-1]);
	current_program->loop_counter++;
  }
#line 10588 "parser.c"
    break;

  case 996: /* perform_option: perform_test UNTIL condition  */
#line 5373 "parser.y"
  {
	cb_tree varying;

	varying = cb_list_init (cb_build_perform_varying (NULL, NULL, NULL, yyvsp[0]));
	yyval = cb_build_perform_until (yyvsp[-2], varying);
  }
#line 10599 "parser.c"
    break;

  case 997: /* perform_option: perform_test VARYING perform_varying_list  */
#line 5380 "parser.y"
  {
	yyval = cb_build_perform_until (yyvsp[-2], yyvsp[0]);
  }
#line 10607 "parser.c"
    break;

  case 998: /* perform_test: %empty  */
#line 5386 "parser.y"
                                { yyval = CB_BEFORE; }
#line 10613 "parser.c"
    break;

  case 999: /* perform_test: _with TEST before_or_after  */
#line 5387 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10619 "parser.c"
    break;

  case 1000: /* perform_varying_list: perform_varying  */
#line 5391 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 10625 "parser.c"
    break;

  case 1001: /* perform_varying_list: perform_varying_list AFTER perform_varying  */
#line 5393 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 10631 "parser.c"
    break;

  case 1002: /* perform_varying: identifier FROM x BY x UNTIL condition  */
#line 5398 "parser.y"
  {
	yyval = cb_build_perform_varying (yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 10639 "parser.c"
    break;

  case 1003: /* $@72: %empty  */
#line 5409 "parser.y"
                                { BEGIN_STATEMENT ("READ", TERM_READ); }
#line 10645 "parser.c"
    break;

  case 1004: /* read_statement: READ $@72 file_name flag_next _record read_into with_lock read_key read_handler end_read  */
#line 5412 "parser.y"
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
#line 10671 "parser.c"
    break;

  case 1005: /* read_into: %empty  */
#line 5436 "parser.y"
                                { yyval = NULL; }
#line 10677 "parser.c"
    break;

  case 1006: /* read_into: INTO identifier  */
#line 5437 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10683 "parser.c"
    break;

  case 1007: /* with_lock: %empty  */
#line 5441 "parser.y"
                                { yyval = NULL; }
#line 10689 "parser.c"
    break;

  case 1008: /* with_lock: IGNORING LOCK  */
#line 5443 "parser.y"
  {
	yyval = cb_int3;
  }
#line 10697 "parser.c"
    break;

  case 1009: /* with_lock: _with LOCK  */
#line 5447 "parser.y"
  {
	yyval = cb_int1;
  }
#line 10705 "parser.c"
    break;

  case 1010: /* with_lock: _with NO LOCK  */
#line 5451 "parser.y"
  {
	yyval = cb_int2;
  }
#line 10713 "parser.c"
    break;

  case 1011: /* with_lock: _with IGNORE LOCK  */
#line 5455 "parser.y"
  {
	yyval = cb_int3;
  }
#line 10721 "parser.c"
    break;

  case 1012: /* with_lock: _with WAIT  */
#line 5459 "parser.y"
  {
	yyval = cb_int4;
  }
#line 10729 "parser.c"
    break;

  case 1013: /* read_key: %empty  */
#line 5465 "parser.y"
                                { yyval = NULL; }
#line 10735 "parser.c"
    break;

  case 1014: /* read_key: KEY _is identifier_list  */
#line 5467 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 10743 "parser.c"
    break;

  case 1018: /* end_read: %empty  */
#line 5478 "parser.y"
                                { terminator_warning (TERM_READ); }
#line 10749 "parser.c"
    break;

  case 1019: /* end_read: "END-READ"  */
#line 5479 "parser.y"
                                { terminator_clear (TERM_READ); }
#line 10755 "parser.c"
    break;

  case 1020: /* $@73: %empty  */
#line 5488 "parser.y"
                                { BEGIN_STATEMENT ("RELEASE", 0); }
#line 10761 "parser.c"
    break;

  case 1021: /* release_statement: RELEASE $@73 record_name write_from  */
#line 5490 "parser.y"
  {
	if (yyvsp[-1] != cb_error_node) {
		cb_emit_release (yyvsp[-1], yyvsp[0]);
	}
  }
#line 10771 "parser.c"
    break;

  case 1022: /* $@74: %empty  */
#line 5503 "parser.y"
                                { BEGIN_STATEMENT ("RETURN", TERM_RETURN); }
#line 10777 "parser.c"
    break;

  case 1023: /* return_statement: RETURN $@74 file_name _record read_into at_end end_return  */
#line 5506 "parser.y"
  {
	if (yyvsp[-4] != cb_error_node) {
		cb_emit_return (yyvsp[-4], yyvsp[-2]);
	}
  }
#line 10787 "parser.c"
    break;

  case 1024: /* end_return: %empty  */
#line 5514 "parser.y"
                                { terminator_warning (TERM_RETURN); }
#line 10793 "parser.c"
    break;

  case 1025: /* end_return: "END-RETURN"  */
#line 5515 "parser.y"
                                { terminator_clear (TERM_RETURN); }
#line 10799 "parser.c"
    break;

  case 1026: /* $@75: %empty  */
#line 5524 "parser.y"
                                { BEGIN_STATEMENT ("REWRITE", TERM_REWRITE); }
#line 10805 "parser.c"
    break;

  case 1027: /* rewrite_statement: REWRITE $@75 record_name write_from write_lock opt_invalid_key end_rewrite  */
#line 5527 "parser.y"
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
#line 10821 "parser.c"
    break;

  case 1028: /* write_lock: %empty  */
#line 5541 "parser.y"
                                { yyval = NULL; }
#line 10827 "parser.c"
    break;

  case 1029: /* write_lock: _with LOCK  */
#line 5543 "parser.y"
  {
	yyval = cb_int1;
  }
#line 10835 "parser.c"
    break;

  case 1030: /* write_lock: _with NO LOCK  */
#line 5547 "parser.y"
  {
	yyval = cb_int2;
  }
#line 10843 "parser.c"
    break;

  case 1031: /* end_rewrite: %empty  */
#line 5553 "parser.y"
                                { terminator_warning (TERM_REWRITE); }
#line 10849 "parser.c"
    break;

  case 1032: /* end_rewrite: "END-REWRITE"  */
#line 5554 "parser.y"
                                { terminator_clear (TERM_REWRITE); }
#line 10855 "parser.c"
    break;

  case 1033: /* rollback_statement: ROLLBACK  */
#line 5564 "parser.y"
  {
	BEGIN_STATEMENT ("ROLLBACK", 0);
	cb_emit_rollback ();
  }
#line 10864 "parser.c"
    break;

  case 1034: /* $@76: %empty  */
#line 5576 "parser.y"
                                { BEGIN_STATEMENT ("SEARCH", TERM_SEARCH); }
#line 10870 "parser.c"
    break;

  case 1036: /* search_body: table_name search_varying search_at_end search_whens  */
#line 5583 "parser.y"
  {
	cb_emit_search (yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]);
  }
#line 10878 "parser.c"
    break;

  case 1037: /* $@77: %empty  */
#line 5587 "parser.y"
  {
	check_unreached = 0;
  }
#line 10886 "parser.c"
    break;

  case 1038: /* search_body: ALL table_name search_at_end WHEN expr $@77 statement_list  */
#line 5591 "parser.y"
  {
	cb_emit_search_all (yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 10894 "parser.c"
    break;

  case 1039: /* search_varying: %empty  */
#line 5597 "parser.y"
                                { yyval = NULL; }
#line 10900 "parser.c"
    break;

  case 1040: /* search_varying: VARYING identifier  */
#line 5598 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10906 "parser.c"
    break;

  case 1041: /* search_at_end: %empty  */
#line 5602 "parser.y"
                                { yyval = NULL; }
#line 10912 "parser.c"
    break;

  case 1042: /* $@78: %empty  */
#line 5604 "parser.y"
  {
	check_unreached = 0;
  }
#line 10920 "parser.c"
    break;

  case 1043: /* search_at_end: _at END $@78 statement_list  */
#line 5608 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 10928 "parser.c"
    break;

  case 1044: /* search_whens: search_when  */
#line 5614 "parser.y"
                                { yyval = yyvsp[0]; }
#line 10934 "parser.c"
    break;

  case 1045: /* search_whens: search_when search_whens  */
#line 5615 "parser.y"
                                { yyval = yyvsp[-1]; CB_IF (yyvsp[-1])->stmt2 = yyvsp[0]; }
#line 10940 "parser.c"
    break;

  case 1046: /* $@79: %empty  */
#line 5620 "parser.y"
  {
	check_unreached = 0;
  }
#line 10948 "parser.c"
    break;

  case 1047: /* search_when: WHEN condition $@79 statement_list  */
#line 5624 "parser.y"
  {
	yyval = cb_build_if (yyvsp[-2], yyvsp[0], NULL);
  }
#line 10956 "parser.c"
    break;

  case 1048: /* end_search: %empty  */
#line 5630 "parser.y"
                                { terminator_warning (TERM_SEARCH); }
#line 10962 "parser.c"
    break;

  case 1049: /* end_search: "END-SEARCH"  */
#line 5631 "parser.y"
                                { terminator_clear (TERM_SEARCH); }
#line 10968 "parser.c"
    break;

  case 1050: /* $@80: %empty  */
#line 5640 "parser.y"
                                { BEGIN_STATEMENT ("SET", 0); }
#line 10974 "parser.c"
    break;

  case 1057: /* set_environment: ENVIRONMENT simple_value TO simple_value  */
#line 5656 "parser.y"
  {
	cb_emit_setenv (yyvsp[-2], yyvsp[0]);
  }
#line 10982 "parser.c"
    break;

  case 1058: /* set_to: target_x_list TO ENTRY alnum_or_id  */
#line 5665 "parser.y"
  {
	cb_emit_set_to (yyvsp[-3], cb_build_ppointer (yyvsp[0]));
  }
#line 10990 "parser.c"
    break;

  case 1059: /* set_to: target_x_list TO x  */
#line 5669 "parser.y"
  {
	cb_emit_set_to (yyvsp[-2], yyvsp[0]);
  }
#line 10998 "parser.c"
    break;

  case 1060: /* set_up_down: target_x_list up_or_down BY x  */
#line 5678 "parser.y"
  {
	cb_emit_set_up_down (yyvsp[-3], yyvsp[-2], yyvsp[0]);
  }
#line 11006 "parser.c"
    break;

  case 1061: /* up_or_down: UP  */
#line 5684 "parser.y"
                                { yyval = cb_int0; }
#line 11012 "parser.c"
    break;

  case 1062: /* up_or_down: DOWN  */
#line 5685 "parser.y"
                                { yyval = cb_int1; }
#line 11018 "parser.c"
    break;

  case 1065: /* set_to_on_off: mnemonic_name_list TO on_or_off  */
#line 5697 "parser.y"
  {
	cb_emit_set_on_off (yyvsp[-2], yyvsp[0]);
  }
#line 11026 "parser.c"
    break;

  case 1068: /* set_to_true_false: target_x_list TO "TRUE"  */
#line 5711 "parser.y"
  {
	cb_emit_set_true (yyvsp[-2]);
  }
#line 11034 "parser.c"
    break;

  case 1069: /* set_to_true_false: target_x_list TO "FALSE"  */
#line 5715 "parser.y"
  {
	cb_emit_set_false (yyvsp[-2]);
  }
#line 11042 "parser.c"
    break;

  case 1070: /* $@81: %empty  */
#line 5726 "parser.y"
                                { BEGIN_STATEMENT ("SORT", 0); }
#line 11048 "parser.c"
    break;

  case 1072: /* $@82: %empty  */
#line 5732 "parser.y"
  {
	cb_emit_sort_init (yyvsp[-3], yyvsp[-2], yyvsp[0]);
	if (CB_FILE_P (cb_ref (yyvsp[-3])) && yyvsp[-2] == NULL) {
		cb_error (_("File sort requires KEY phrase"));
	}
	/* used in sort_input/sort_output */
	save_tree_1 = yyvsp[-3];
  }
#line 11061 "parser.c"
    break;

  case 1073: /* sort_body: qualified_word sort_key_list sort_duplicates sort_collating $@82 sort_input sort_output  */
#line 5741 "parser.y"
  {
	cb_emit_sort_finish (yyvsp[-6]);
  }
#line 11069 "parser.c"
    break;

  case 1074: /* sort_key_list: %empty  */
#line 5748 "parser.y"
  {
	yyval = NULL;
  }
#line 11077 "parser.c"
    break;

  case 1075: /* sort_key_list: sort_key_list _on ascending_or_descending _key _is opt_key_list  */
#line 5753 "parser.y"
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
#line 11098 "parser.c"
    break;

  case 1076: /* opt_key_list: %empty  */
#line 5772 "parser.y"
                                { yyval = NULL; }
#line 11104 "parser.c"
    break;

  case 1077: /* opt_key_list: opt_key_list qualified_word  */
#line 5773 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 11110 "parser.c"
    break;

  case 1079: /* sort_duplicates: with_dups _in_order  */
#line 5777 "parser.y"
                                { /* nothing */ }
#line 11116 "parser.c"
    break;

  case 1080: /* sort_collating: %empty  */
#line 5781 "parser.y"
                                        { yyval = cb_null; }
#line 11122 "parser.c"
    break;

  case 1081: /* sort_collating: coll_sequence _is reference  */
#line 5782 "parser.y"
                                        { yyval = cb_ref (yyvsp[0]); }
#line 11128 "parser.c"
    break;

  case 1082: /* sort_input: %empty  */
#line 5787 "parser.y"
  {
	if (CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("File sort requires USING or INPUT PROCEDURE"));
	}
  }
#line 11138 "parser.c"
    break;

  case 1083: /* sort_input: USING file_name_list  */
#line 5793 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("USING invalid with table SORT"));
	} else {
		cb_emit_sort_using (save_tree_1, yyvsp[0]);
	}
  }
#line 11150 "parser.c"
    break;

  case 1084: /* sort_input: INPUT PROCEDURE _is perform_procedure  */
#line 5801 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("INPUT PROCEDURE invalid with table SORT"));
	} else {
		cb_emit_sort_input (yyvsp[0], save_tree_1);
	}
  }
#line 11162 "parser.c"
    break;

  case 1085: /* sort_output: %empty  */
#line 5812 "parser.y"
  {
	if (CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("File sort requires GIVING or OUTPUT PROCEDURE"));
	}
  }
#line 11172 "parser.c"
    break;

  case 1086: /* sort_output: GIVING file_name_list  */
#line 5818 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("GIVING invalid with table SORT"));
	} else {
		cb_emit_sort_giving (save_tree_1, yyvsp[0]);
	}
  }
#line 11184 "parser.c"
    break;

  case 1087: /* sort_output: OUTPUT PROCEDURE _is perform_procedure  */
#line 5826 "parser.y"
  {
	if (!CB_FILE_P (cb_ref (save_tree_1))) {
		cb_error (_("OUTPUT PROCEDURE invalid with table SORT"));
	} else {
		cb_emit_sort_output (yyvsp[0], save_tree_1);
	}
  }
#line 11196 "parser.c"
    break;

  case 1088: /* $@83: %empty  */
#line 5841 "parser.y"
                                { BEGIN_STATEMENT ("START", TERM_START); }
#line 11202 "parser.c"
    break;

  case 1089: /* @84: %empty  */
#line 5842 "parser.y"
                                { yyval = cb_int (COB_EQ); }
#line 11208 "parser.c"
    break;

  case 1090: /* start_statement: START $@83 file_name @84 start_key opt_invalid_key end_start  */
#line 5845 "parser.y"
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
#line 11227 "parser.c"
    break;

  case 1091: /* start_key: %empty  */
#line 5862 "parser.y"
                                { yyval = NULL; }
#line 11233 "parser.c"
    break;

  case 1092: /* start_key: KEY _is start_op identifier_list  */
#line 5864 "parser.y"
  {
	yyvsp[-4] = yyvsp[-1];
	yyval = yyvsp[0];
  }
#line 11242 "parser.c"
    break;

  case 1093: /* start_op: flag_not eq  */
#line 5871 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_NE : COB_EQ); }
#line 11248 "parser.c"
    break;

  case 1094: /* start_op: flag_not gt  */
#line 5872 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_LE : COB_GT); }
#line 11254 "parser.c"
    break;

  case 1095: /* start_op: flag_not lt  */
#line 5873 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_GE : COB_LT); }
#line 11260 "parser.c"
    break;

  case 1096: /* start_op: flag_not ge  */
#line 5874 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_LT : COB_GE); }
#line 11266 "parser.c"
    break;

  case 1097: /* start_op: flag_not le  */
#line 5875 "parser.y"
                        { yyval = cb_int ((yyvsp[-1] == cb_int1) ? COB_GT : COB_LE); }
#line 11272 "parser.c"
    break;

  case 1098: /* end_start: %empty  */
#line 5879 "parser.y"
                                { terminator_warning (TERM_START); }
#line 11278 "parser.c"
    break;

  case 1099: /* end_start: "END-START"  */
#line 5880 "parser.y"
                                { terminator_clear (TERM_START); }
#line 11284 "parser.c"
    break;

  case 1100: /* $@85: %empty  */
#line 5889 "parser.y"
                                { BEGIN_STATEMENT ("STOP", 0); }
#line 11290 "parser.c"
    break;

  case 1101: /* stop_statement: STOP RUN $@85 stop_returning  */
#line 5891 "parser.y"
  {
	cb_emit_stop_run (yyvsp[0]);
  }
#line 11298 "parser.c"
    break;

  case 1102: /* $@86: %empty  */
#line 5894 "parser.y"
                                { BEGIN_STATEMENT ("STOP", 0); }
#line 11304 "parser.c"
    break;

  case 1103: /* stop_statement: STOP "Literal" $@86  */
#line 5895 "parser.y"
  {
	cb_verify (cb_stop_literal_statement, "STOP literal");
  }
#line 11312 "parser.c"
    break;

  case 1104: /* stop_returning: %empty  */
#line 5901 "parser.y"
                        { yyval = current_program->cb_return_code; }
#line 11318 "parser.c"
    break;

  case 1105: /* stop_returning: RETURNING x  */
#line 5902 "parser.y"
                        { yyval = yyvsp[0]; }
#line 11324 "parser.c"
    break;

  case 1106: /* stop_returning: GIVING x  */
#line 5903 "parser.y"
                        { yyval = yyvsp[0]; }
#line 11330 "parser.c"
    break;

  case 1107: /* $@87: %empty  */
#line 5912 "parser.y"
                                { BEGIN_STATEMENT ("STRING", TERM_STRING); }
#line 11336 "parser.c"
    break;

  case 1108: /* string_statement: STRING $@87 string_item_list INTO identifier opt_with_pointer on_overflow end_string  */
#line 5915 "parser.y"
  {
	cb_emit_string (yyvsp[-5], yyvsp[-3], yyvsp[-2]);
  }
#line 11344 "parser.c"
    break;

  case 1109: /* string_item_list: string_item  */
#line 5921 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 11350 "parser.c"
    break;

  case 1110: /* string_item_list: string_item_list string_item  */
#line 5922 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 11356 "parser.c"
    break;

  case 1111: /* string_item: x  */
#line 5926 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11362 "parser.c"
    break;

  case 1112: /* string_item: DELIMITED _by SIZE  */
#line 5927 "parser.y"
                                { yyval = cb_build_pair (cb_int0, NULL); }
#line 11368 "parser.c"
    break;

  case 1113: /* string_item: DELIMITED _by x  */
#line 5928 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], NULL); }
#line 11374 "parser.c"
    break;

  case 1114: /* opt_with_pointer: %empty  */
#line 5932 "parser.y"
                                { yyval = cb_int0; }
#line 11380 "parser.c"
    break;

  case 1115: /* opt_with_pointer: _with POINTER identifier  */
#line 5933 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11386 "parser.c"
    break;

  case 1116: /* end_string: %empty  */
#line 5937 "parser.y"
                                { terminator_warning (TERM_STRING); }
#line 11392 "parser.c"
    break;

  case 1117: /* end_string: "END-STRING"  */
#line 5938 "parser.y"
                                { terminator_clear (TERM_STRING); }
#line 11398 "parser.c"
    break;

  case 1118: /* $@88: %empty  */
#line 5947 "parser.y"
                                { BEGIN_STATEMENT ("SUBTRACT", TERM_SUBTRACT); }
#line 11404 "parser.c"
    break;

  case 1120: /* subtract_body: x_list FROM arithmetic_x_list on_size_error  */
#line 5954 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], '-', cb_build_binary_list (yyvsp[-3], '+'));
  }
#line 11412 "parser.c"
    break;

  case 1121: /* subtract_body: x_list FROM x GIVING arithmetic_x_list on_size_error  */
#line 5958 "parser.y"
  {
	cb_emit_arithmetic (yyvsp[-1], 0, cb_build_binary_list (cb_cons (yyvsp[-3], yyvsp[-5]), '-'));
  }
#line 11420 "parser.c"
    break;

  case 1122: /* subtract_body: CORRESPONDING identifier FROM identifier flag_rounded on_size_error  */
#line 5962 "parser.y"
  {
	cb_emit_corresponding (cb_build_sub, yyvsp[-2], yyvsp[-4], yyvsp[-1]);
  }
#line 11428 "parser.c"
    break;

  case 1123: /* end_subtract: %empty  */
#line 5968 "parser.y"
                                { terminator_warning (TERM_SUBTRACT); }
#line 11434 "parser.c"
    break;

  case 1124: /* end_subtract: "END-SUBTRACT"  */
#line 5969 "parser.y"
                                { terminator_clear (TERM_SUBTRACT); }
#line 11440 "parser.c"
    break;

  case 1125: /* suppress_statement: SUPPRESS _printing  */
#line 5979 "parser.y"
  {
	BEGIN_STATEMENT ("SUPPRESS", 0);
	PENDING("SUPPRESS");
  }
#line 11449 "parser.c"
    break;

  case 1128: /* $@89: %empty  */
#line 5994 "parser.y"
                                { BEGIN_STATEMENT ("TERMINATE", 0); }
#line 11455 "parser.c"
    break;

  case 1129: /* terminate_statement: TERMINATE $@89 identifier_list  */
#line 5996 "parser.y"
  {
	PENDING("TERMINATE");
  }
#line 11463 "parser.c"
    break;

  case 1130: /* $@90: %empty  */
#line 6007 "parser.y"
                                { BEGIN_STATEMENT ("TRANSFORM", 0); }
#line 11469 "parser.c"
    break;

  case 1131: /* transform_statement: TRANSFORM $@90 identifier FROM simple_value TO simple_all_value  */
#line 6009 "parser.y"
  {
	cb_tree		x;

	x = cb_build_converting (yyvsp[-2], yyvsp[0], cb_build_inspect_region_start ());
	cb_emit_inspect (yyvsp[-4], x, cb_int0, 2);
  }
#line 11480 "parser.c"
    break;

  case 1132: /* $@91: %empty  */
#line 6023 "parser.y"
                                { BEGIN_STATEMENT ("UNLOCK", 0); }
#line 11486 "parser.c"
    break;

  case 1133: /* unlock_statement: UNLOCK $@91 file_name opt_record  */
#line 6025 "parser.y"
  {
	if (yyvsp[-1] != cb_error_node) {
		cb_emit_unlock (yyvsp[-1]);
	}
  }
#line 11496 "parser.c"
    break;

  case 1137: /* $@92: %empty  */
#line 6044 "parser.y"
                                { BEGIN_STATEMENT ("UNSTRING", TERM_UNSTRING); }
#line 11502 "parser.c"
    break;

  case 1138: /* unstring_statement: UNSTRING $@92 identifier unstring_delimited unstring_into opt_with_pointer unstring_tallying on_overflow end_unstring  */
#line 6048 "parser.y"
  {
	cb_emit_unstring (yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2]);
  }
#line 11510 "parser.c"
    break;

  case 1139: /* unstring_delimited: %empty  */
#line 6054 "parser.y"
                                { yyval = NULL; }
#line 11516 "parser.c"
    break;

  case 1140: /* unstring_delimited: DELIMITED _by unstring_delimited_list  */
#line 6056 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11522 "parser.c"
    break;

  case 1141: /* unstring_delimited_list: unstring_delimited_item  */
#line 6060 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 11528 "parser.c"
    break;

  case 1142: /* unstring_delimited_list: unstring_delimited_list OR unstring_delimited_item  */
#line 6062 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 11534 "parser.c"
    break;

  case 1143: /* unstring_delimited_item: flag_all simple_value  */
#line 6067 "parser.y"
  {
	yyval = cb_build_unstring_delimited (yyvsp[-1], yyvsp[0]);
  }
#line 11542 "parser.c"
    break;

  case 1144: /* unstring_into: INTO unstring_into_item  */
#line 6073 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 11548 "parser.c"
    break;

  case 1145: /* unstring_into: unstring_into unstring_into_item  */
#line 6075 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 11554 "parser.c"
    break;

  case 1146: /* unstring_into_item: identifier unstring_into_delimiter unstring_into_count  */
#line 6080 "parser.y"
  {
	yyval = cb_build_unstring_into (yyvsp[-2], yyvsp[-1], yyvsp[0]);
  }
#line 11562 "parser.c"
    break;

  case 1147: /* unstring_into_delimiter: %empty  */
#line 6086 "parser.y"
                                { yyval = NULL; }
#line 11568 "parser.c"
    break;

  case 1148: /* unstring_into_delimiter: DELIMITER _in identifier  */
#line 6087 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11574 "parser.c"
    break;

  case 1149: /* unstring_into_count: %empty  */
#line 6091 "parser.y"
                                { yyval = NULL; }
#line 11580 "parser.c"
    break;

  case 1150: /* unstring_into_count: COUNT _in identifier  */
#line 6092 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11586 "parser.c"
    break;

  case 1151: /* unstring_tallying: %empty  */
#line 6096 "parser.y"
                                { yyval = NULL; }
#line 11592 "parser.c"
    break;

  case 1152: /* unstring_tallying: TALLYING _in identifier  */
#line 6097 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11598 "parser.c"
    break;

  case 1153: /* end_unstring: %empty  */
#line 6101 "parser.y"
                                { terminator_warning (TERM_UNSTRING); }
#line 11604 "parser.c"
    break;

  case 1154: /* end_unstring: "END-UNSTRING"  */
#line 6102 "parser.y"
                                { terminator_clear (TERM_UNSTRING); }
#line 11610 "parser.c"
    break;

  case 1158: /* use_exception: USE use_global _after _standard exception_or_error _procedure _on use_exception_target  */
#line 6120 "parser.y"
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
#line 11632 "parser.c"
    break;

  case 1159: /* use_global: %empty  */
#line 6141 "parser.y"
  {
	use_global_ind = 0;
  }
#line 11640 "parser.c"
    break;

  case 1160: /* use_global: GLOBAL  */
#line 6145 "parser.y"
  {
	use_global_ind = 1;
	current_program->flag_global_use = 1;
  }
#line 11649 "parser.c"
    break;

  case 1161: /* use_exception_target: file_name_list  */
#line 6153 "parser.y"
  {
	cb_tree		l;

	for (l = yyvsp[0]; l; l = CB_CHAIN (l)) {
		if (CB_VALUE (l) != cb_error_node) {
			setup_use_file (CB_FILE (cb_ref (CB_VALUE (l))));
		}
	}
  }
#line 11663 "parser.c"
    break;

  case 1162: /* use_exception_target: INPUT  */
#line 6163 "parser.y"
  {
	current_program->global_handler[COB_OPEN_INPUT].handler_label = current_section;
	current_program->global_handler[COB_OPEN_INPUT].handler_prog = current_program;
  }
#line 11672 "parser.c"
    break;

  case 1163: /* use_exception_target: OUTPUT  */
#line 6168 "parser.y"
  {
	current_program->global_handler[COB_OPEN_OUTPUT].handler_label = current_section;
	current_program->global_handler[COB_OPEN_OUTPUT].handler_prog = current_program;
  }
#line 11681 "parser.c"
    break;

  case 1164: /* use_exception_target: "I-O"  */
#line 6173 "parser.y"
  {
	current_program->global_handler[COB_OPEN_I_O].handler_label = current_section;
	current_program->global_handler[COB_OPEN_I_O].handler_prog = current_program;
  }
#line 11690 "parser.c"
    break;

  case 1165: /* use_exception_target: EXTEND  */
#line 6178 "parser.y"
  {
	current_program->global_handler[COB_OPEN_EXTEND].handler_label = current_section;
	current_program->global_handler[COB_OPEN_EXTEND].handler_prog = current_program;
  }
#line 11699 "parser.c"
    break;

  case 1178: /* use_debugging: USE _for DEBUGGING _on use_debugging_target  */
#line 6210 "parser.y"
  {
	PENDING ("USE FOR DEBUGGING");
  }
#line 11707 "parser.c"
    break;

  case 1181: /* use_reporting: USE use_global BEFORE REPORTING identifier  */
#line 6222 "parser.y"
  {
	PENDING ("USE BEFORE REPORTING");
  }
#line 11715 "parser.c"
    break;

  case 1182: /* $@93: %empty  */
#line 6233 "parser.y"
                                { BEGIN_STATEMENT ("WRITE", TERM_WRITE); }
#line 11721 "parser.c"
    break;

  case 1183: /* write_statement: WRITE $@93 record_name write_from write_lock write_option write_handler end_write  */
#line 6236 "parser.y"
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
#line 11737 "parser.c"
    break;

  case 1184: /* write_from: %empty  */
#line 6250 "parser.y"
                                { yyval = NULL; }
#line 11743 "parser.c"
    break;

  case 1185: /* write_from: FROM id_or_lit  */
#line 6251 "parser.y"
                                { yyval = yyvsp[0]; }
#line 11749 "parser.c"
    break;

  case 1186: /* write_option: %empty  */
#line 6256 "parser.y"
  {
	yyval = cb_int0;
  }
#line 11757 "parser.c"
    break;

  case 1187: /* write_option: before_or_after _advancing num_id_or_lit _line_or_lines  */
#line 6260 "parser.y"
  {
	yyval = cb_build_write_advancing_lines (yyvsp[-3], yyvsp[-1]);
  }
#line 11765 "parser.c"
    break;

  case 1188: /* write_option: before_or_after _advancing mnemonic_name  */
#line 6264 "parser.y"
  {
	yyval = cb_build_write_advancing_mnemonic (yyvsp[-2], yyvsp[0]);
  }
#line 11773 "parser.c"
    break;

  case 1189: /* write_option: before_or_after _advancing PAGE  */
#line 6268 "parser.y"
  {
	yyval = cb_build_write_advancing_page (yyvsp[-2]);
  }
#line 11781 "parser.c"
    break;

  case 1190: /* before_or_after: BEFORE  */
#line 6274 "parser.y"
                                { yyval = CB_BEFORE; }
#line 11787 "parser.c"
    break;

  case 1191: /* before_or_after: AFTER  */
#line 6275 "parser.y"
                                { yyval = CB_AFTER; }
#line 11793 "parser.c"
    break;

  case 1195: /* end_write: %empty  */
#line 6284 "parser.y"
                                { terminator_warning (TERM_WRITE); }
#line 11799 "parser.c"
    break;

  case 1196: /* end_write: "END-WRITE"  */
#line 6285 "parser.y"
                                { terminator_clear (TERM_WRITE); }
#line 11805 "parser.c"
    break;

  case 1197: /* on_accp_exception: opt_on_exception opt_not_on_exception  */
#line 6300 "parser.y"
  {
	current_statement->handler_id = COB_EC_IMP_ACCEPT;
  }
#line 11813 "parser.c"
    break;

  case 1198: /* on_disp_exception: opt_on_exception opt_not_on_exception  */
#line 6308 "parser.y"
  {
	current_statement->handler_id = COB_EC_IMP_DISPLAY;
  }
#line 11821 "parser.c"
    break;

  case 1200: /* $@94: %empty  */
#line 6315 "parser.y"
  {
	check_unreached = 0;
  }
#line 11829 "parser.c"
    break;

  case 1201: /* opt_on_exception: EXCEPTION $@94 statement_list  */
#line 6319 "parser.y"
  {
	current_statement->handler1 = yyvsp[0];
  }
#line 11837 "parser.c"
    break;

  case 1203: /* $@95: %empty  */
#line 6326 "parser.y"
  {
	check_unreached = 0;
  }
#line 11845 "parser.c"
    break;

  case 1204: /* opt_not_on_exception: "NOT EXCEPTION" $@95 statement_list  */
#line 6330 "parser.y"
  {
	current_statement->handler2 = yyvsp[0];
  }
#line 11853 "parser.c"
    break;

  case 1207: /* $@96: %empty  */
#line 6346 "parser.y"
  {
	check_unreached = 0;
	current_statement->handler_id = COB_EC_SIZE;
  }
#line 11862 "parser.c"
    break;

  case 1208: /* opt_on_size_error: "SIZE ERROR" $@96 statement_list  */
#line 6351 "parser.y"
  {
	current_statement->handler1 = yyvsp[0];
  }
#line 11870 "parser.c"
    break;

  case 1210: /* $@97: %empty  */
#line 6358 "parser.y"
  {
	check_unreached = 0;
	current_statement->handler_id = COB_EC_SIZE;
  }
#line 11879 "parser.c"
    break;

  case 1211: /* opt_not_on_size_error: "NOT SIZE ERROR" $@97 statement_list  */
#line 6363 "parser.y"
  {
	current_statement->handler2 = yyvsp[0];
  }
#line 11887 "parser.c"
    break;

  case 1212: /* on_overflow: opt_on_overflow opt_not_on_overflow  */
#line 6375 "parser.y"
  {
	current_statement->handler_id = COB_EC_OVERFLOW;
  }
#line 11895 "parser.c"
    break;

  case 1214: /* $@98: %empty  */
#line 6382 "parser.y"
  {
	check_unreached = 0;
  }
#line 11903 "parser.c"
    break;

  case 1215: /* opt_on_overflow: OVERFLOW $@98 statement_list  */
#line 6386 "parser.y"
  {
	current_statement->handler1 = yyvsp[0];
  }
#line 11911 "parser.c"
    break;

  case 1217: /* $@99: %empty  */
#line 6393 "parser.y"
  {
	check_unreached = 0;
  }
#line 11919 "parser.c"
    break;

  case 1218: /* opt_not_on_overflow: "NOT OVERFLOW" $@99 statement_list  */
#line 6397 "parser.y"
  {
	current_statement->handler2 = yyvsp[0];
  }
#line 11927 "parser.c"
    break;

  case 1219: /* at_end: at_end_sentence  */
#line 6409 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_AT_END;
	current_statement->handler1 = yyvsp[0];
  }
#line 11936 "parser.c"
    break;

  case 1220: /* at_end: not_at_end_sentence  */
#line 6414 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_AT_END;
	current_statement->handler2 = yyvsp[0];
  }
#line 11945 "parser.c"
    break;

  case 1221: /* at_end: at_end_sentence not_at_end_sentence  */
#line 6419 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_AT_END;
	current_statement->handler1 = yyvsp[-1];
	current_statement->handler2 = yyvsp[0];
  }
#line 11955 "parser.c"
    break;

  case 1222: /* $@100: %empty  */
#line 6428 "parser.y"
  {
	check_unreached = 0;
  }
#line 11963 "parser.c"
    break;

  case 1223: /* at_end_sentence: END $@100 statement_list  */
#line 6432 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 11971 "parser.c"
    break;

  case 1224: /* $@101: %empty  */
#line 6439 "parser.y"
  {
	check_unreached = 0;
  }
#line 11979 "parser.c"
    break;

  case 1225: /* not_at_end_sentence: "NOT END" $@101 statement_list  */
#line 6443 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 11987 "parser.c"
    break;

  case 1226: /* at_eop: at_eop_sentence  */
#line 6455 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_EOP;
	current_statement->handler1 = yyvsp[0];
  }
#line 11996 "parser.c"
    break;

  case 1227: /* at_eop: not_at_eop_sentence  */
#line 6460 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_EOP;
	current_statement->handler2 = yyvsp[0];
  }
#line 12005 "parser.c"
    break;

  case 1228: /* at_eop: at_eop_sentence not_at_eop_sentence  */
#line 6465 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_EOP;
	current_statement->handler1 = yyvsp[-1];
	current_statement->handler2 = yyvsp[0];
  }
#line 12015 "parser.c"
    break;

  case 1229: /* $@102: %empty  */
#line 6474 "parser.y"
  {
	check_unreached = 0;
  }
#line 12023 "parser.c"
    break;

  case 1230: /* at_eop_sentence: EOP $@102 statement_list  */
#line 6478 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12031 "parser.c"
    break;

  case 1231: /* $@103: %empty  */
#line 6485 "parser.y"
  {
	check_unreached = 0;
  }
#line 12039 "parser.c"
    break;

  case 1232: /* not_at_eop_sentence: "NOT EOP" $@103 statement_list  */
#line 6489 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12047 "parser.c"
    break;

  case 1235: /* invalid_key: invalid_key_sentence  */
#line 6505 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_INVALID_KEY;
	current_statement->handler1 = yyvsp[0];
  }
#line 12056 "parser.c"
    break;

  case 1236: /* invalid_key: not_invalid_key_sentence  */
#line 6510 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_INVALID_KEY;
	current_statement->handler2 = yyvsp[0];
  }
#line 12065 "parser.c"
    break;

  case 1237: /* invalid_key: invalid_key_sentence not_invalid_key_sentence  */
#line 6515 "parser.y"
  {
	current_statement->handler_id = COB_EC_I_O_INVALID_KEY;
	current_statement->handler1 = yyvsp[-1];
	current_statement->handler2 = yyvsp[0];
  }
#line 12075 "parser.c"
    break;

  case 1238: /* $@104: %empty  */
#line 6524 "parser.y"
  {
	check_unreached = 0;
  }
#line 12083 "parser.c"
    break;

  case 1239: /* invalid_key_sentence: "INVALID KEY" $@104 statement_list  */
#line 6528 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12091 "parser.c"
    break;

  case 1240: /* $@105: %empty  */
#line 6535 "parser.y"
  {
	check_unreached = 0;
  }
#line 12099 "parser.c"
    break;

  case 1241: /* not_invalid_key_sentence: "NOT INVALID KEY" $@105 statement_list  */
#line 6539 "parser.y"
  {
	yyval = yyvsp[0];
  }
#line 12107 "parser.c"
    break;

  case 1242: /* _opt_scroll_lines: %empty  */
#line 6551 "parser.y"
  {
	yyval = cb_one;
  }
#line 12115 "parser.c"
    break;

  case 1243: /* _opt_scroll_lines: _by num_id_or_lit _line_or_lines  */
#line 6555 "parser.y"
  {
	yyval = yyvsp[-1];
  }
#line 12123 "parser.c"
    break;

  case 1244: /* condition: expr  */
#line 6567 "parser.y"
  {
	yyval = cb_build_cond (yyvsp[0]);
  }
#line 12131 "parser.c"
    break;

  case 1245: /* expr: partial_expr  */
#line 6574 "parser.y"
  {
	yyval = cb_build_expr (yyvsp[0]);
  }
#line 12139 "parser.c"
    break;

  case 1246: /* $@106: %empty  */
#line 6580 "parser.y"
  {
	current_expr = NULL;
  }
#line 12147 "parser.c"
    break;

  case 1247: /* partial_expr: $@106 expr_tokens  */
#line 6584 "parser.y"
  {
	yyval = cb_list_reverse (current_expr);
  }
#line 12155 "parser.c"
    break;

  case 1248: /* expr_tokens: expr_token x  */
#line 6590 "parser.y"
                        { push_expr ('x', yyvsp[0]); }
#line 12161 "parser.c"
    break;

  case 1249: /* expr_tokens: expr_tokens ')'  */
#line 6591 "parser.y"
                        { push_expr (')', NULL); }
#line 12167 "parser.c"
    break;

  case 1250: /* expr_tokens: expr_token OMITTED  */
#line 6593 "parser.y"
                                { push_expr ('O', NULL); }
#line 12173 "parser.c"
    break;

  case 1251: /* expr_tokens: expr_token NUMERIC  */
#line 6594 "parser.y"
                                { push_expr ('9', NULL); }
#line 12179 "parser.c"
    break;

  case 1252: /* expr_tokens: expr_token ALPHABETIC  */
#line 6595 "parser.y"
                                { push_expr ('A', NULL); }
#line 12185 "parser.c"
    break;

  case 1253: /* expr_tokens: expr_token "ALPHABETIC-LOWER"  */
#line 6596 "parser.y"
                                { push_expr ('L', NULL); }
#line 12191 "parser.c"
    break;

  case 1254: /* expr_tokens: expr_token "ALPHABETIC-UPPER"  */
#line 6597 "parser.y"
                                { push_expr ('U', NULL); }
#line 12197 "parser.c"
    break;

  case 1255: /* expr_tokens: expr_token CLASS_NAME  */
#line 6598 "parser.y"
                                { push_expr ('x', yyvsp[0]); }
#line 12203 "parser.c"
    break;

  case 1256: /* expr_tokens: expr_tokens OMITTED  */
#line 6600 "parser.y"
                                        { push_expr ('O', NULL); }
#line 12209 "parser.c"
    break;

  case 1257: /* expr_tokens: expr_tokens NUMERIC  */
#line 6601 "parser.y"
                                        { push_expr ('9', NULL); }
#line 12215 "parser.c"
    break;

  case 1258: /* expr_tokens: expr_tokens ALPHABETIC  */
#line 6602 "parser.y"
                                        { push_expr ('A', NULL); }
#line 12221 "parser.c"
    break;

  case 1259: /* expr_tokens: expr_tokens "ALPHABETIC-LOWER"  */
#line 6603 "parser.y"
                                        { push_expr ('L', NULL); }
#line 12227 "parser.c"
    break;

  case 1260: /* expr_tokens: expr_tokens "ALPHABETIC-UPPER"  */
#line 6604 "parser.y"
                                        { push_expr ('U', NULL); }
#line 12233 "parser.c"
    break;

  case 1261: /* expr_tokens: expr_tokens CLASS_NAME  */
#line 6605 "parser.y"
                                        { push_expr ('x', yyvsp[0]); }
#line 12239 "parser.c"
    break;

  case 1262: /* expr_tokens: expr_token POSITIVE  */
#line 6607 "parser.y"
                        { push_expr ('P', NULL); }
#line 12245 "parser.c"
    break;

  case 1263: /* expr_tokens: expr_token NEGATIVE  */
#line 6608 "parser.y"
                        { push_expr ('N', NULL); }
#line 12251 "parser.c"
    break;

  case 1264: /* expr_tokens: expr_tokens POSITIVE  */
#line 6610 "parser.y"
                        { push_expr ('P', NULL); }
#line 12257 "parser.c"
    break;

  case 1265: /* expr_tokens: expr_tokens NEGATIVE  */
#line 6611 "parser.y"
                        { push_expr ('N', NULL); }
#line 12263 "parser.c"
    break;

  case 1266: /* expr_tokens: expr_tokens ZERO  */
#line 6612 "parser.y"
                        { push_expr ('x', cb_zero); }
#line 12269 "parser.c"
    break;

  case 1270: /* expr_token: expr_token '('  */
#line 6619 "parser.y"
                        { push_expr ('(', NULL); }
#line 12275 "parser.c"
    break;

  case 1271: /* expr_token: expr_token '+'  */
#line 6621 "parser.y"
                        { push_expr ('+', NULL); }
#line 12281 "parser.c"
    break;

  case 1272: /* expr_token: expr_token '-'  */
#line 6622 "parser.y"
                        { push_expr ('-', NULL); }
#line 12287 "parser.c"
    break;

  case 1273: /* expr_token: expr_token '^'  */
#line 6623 "parser.y"
                        { push_expr ('^', NULL); }
#line 12293 "parser.c"
    break;

  case 1274: /* expr_token: expr_token NOT  */
#line 6625 "parser.y"
                        { push_expr ('!', NULL); }
#line 12299 "parser.c"
    break;

  case 1275: /* expr_token: expr_tokens NOT  */
#line 6626 "parser.y"
                        { push_expr ('!', NULL); }
#line 12305 "parser.c"
    break;

  case 1276: /* expr_token: expr_tokens '+'  */
#line 6628 "parser.y"
                        { push_expr ('+', NULL); }
#line 12311 "parser.c"
    break;

  case 1277: /* expr_token: expr_tokens '-'  */
#line 6629 "parser.y"
                        { push_expr ('-', NULL); }
#line 12317 "parser.c"
    break;

  case 1278: /* expr_token: expr_tokens '*'  */
#line 6630 "parser.y"
                        { push_expr ('*', NULL); }
#line 12323 "parser.c"
    break;

  case 1279: /* expr_token: expr_tokens '/'  */
#line 6631 "parser.y"
                        { push_expr ('/', NULL); }
#line 12329 "parser.c"
    break;

  case 1280: /* expr_token: expr_tokens '^'  */
#line 6632 "parser.y"
                        { push_expr ('^', NULL); }
#line 12335 "parser.c"
    break;

  case 1281: /* expr_token: expr_tokens eq  */
#line 6634 "parser.y"
                        { push_expr ('=', NULL); }
#line 12341 "parser.c"
    break;

  case 1282: /* expr_token: expr_tokens gt  */
#line 6635 "parser.y"
                        { push_expr ('>', NULL); }
#line 12347 "parser.c"
    break;

  case 1283: /* expr_token: expr_tokens lt  */
#line 6636 "parser.y"
                        { push_expr ('<', NULL); }
#line 12353 "parser.c"
    break;

  case 1284: /* expr_token: expr_tokens ge  */
#line 6637 "parser.y"
                        { push_expr (']', NULL); }
#line 12359 "parser.c"
    break;

  case 1285: /* expr_token: expr_tokens le  */
#line 6638 "parser.y"
                        { push_expr ('[', NULL); }
#line 12365 "parser.c"
    break;

  case 1286: /* expr_token: expr_tokens NE  */
#line 6639 "parser.y"
                        { push_expr ('~', NULL); }
#line 12371 "parser.c"
    break;

  case 1287: /* expr_token: expr_token eq  */
#line 6641 "parser.y"
                        { push_expr ('=', NULL); }
#line 12377 "parser.c"
    break;

  case 1288: /* expr_token: expr_token gt  */
#line 6642 "parser.y"
                        { push_expr ('>', NULL); }
#line 12383 "parser.c"
    break;

  case 1289: /* expr_token: expr_token lt  */
#line 6643 "parser.y"
                        { push_expr ('<', NULL); }
#line 12389 "parser.c"
    break;

  case 1290: /* expr_token: expr_token ge  */
#line 6644 "parser.y"
                        { push_expr (']', NULL); }
#line 12395 "parser.c"
    break;

  case 1291: /* expr_token: expr_token le  */
#line 6645 "parser.y"
                        { push_expr ('[', NULL); }
#line 12401 "parser.c"
    break;

  case 1292: /* expr_token: expr_token NE  */
#line 6646 "parser.y"
                        { push_expr ('~', NULL); }
#line 12407 "parser.c"
    break;

  case 1293: /* expr_token: expr_tokens AND  */
#line 6648 "parser.y"
                        { push_expr ('&', NULL); }
#line 12413 "parser.c"
    break;

  case 1294: /* expr_token: expr_tokens OR  */
#line 6649 "parser.y"
                        { push_expr ('|', NULL); }
#line 12419 "parser.c"
    break;

  case 1308: /* exp_list: exp  */
#line 6661 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12425 "parser.c"
    break;

  case 1309: /* exp_list: exp_list e_sep exp  */
#line 6662 "parser.y"
                                { yyval = cb_list_add (yyvsp[-2], yyvsp[0]); }
#line 12431 "parser.c"
    break;

  case 1313: /* exp: arith_x  */
#line 6671 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12437 "parser.c"
    break;

  case 1314: /* exp: exp '+' exp  */
#line 6672 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '+', yyvsp[0]); }
#line 12443 "parser.c"
    break;

  case 1315: /* exp: exp '-' exp  */
#line 6673 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '-', yyvsp[0]); }
#line 12449 "parser.c"
    break;

  case 1316: /* exp: exp '*' exp  */
#line 6674 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '*', yyvsp[0]); }
#line 12455 "parser.c"
    break;

  case 1317: /* exp: exp '/' exp  */
#line 6675 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '/', yyvsp[0]); }
#line 12461 "parser.c"
    break;

  case 1318: /* exp: '+' exp  */
#line 6676 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12467 "parser.c"
    break;

  case 1319: /* exp: '-' exp  */
#line 6677 "parser.y"
                                { yyval = cb_build_binary_op (cb_zero, '-', yyvsp[0]); }
#line 12473 "parser.c"
    break;

  case 1320: /* exp: exp '^' exp  */
#line 6678 "parser.y"
                                { yyval = cb_build_binary_op (yyvsp[-2], '^', yyvsp[0]); }
#line 12479 "parser.c"
    break;

  case 1321: /* exp: '(' exp ')'  */
#line 6679 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 12485 "parser.c"
    break;

  case 1322: /* linage_counter: "LINAGE-COUNTER"  */
#line 6691 "parser.y"
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
#line 12501 "parser.c"
    break;

  case 1323: /* linage_counter: "LINAGE-COUNTER" in_of "Identifier"  */
#line 6703 "parser.y"
  {
	if (CB_FILE_P (cb_ref (yyvsp[0]))) {
		yyval = CB_FILE (cb_ref (yyvsp[0]))->linage_ctr;
	} else {
		cb_error_x (yyvsp[0], _("'%s' is not a file name"), CB_NAME (yyvsp[0]));
		yyval = cb_error_node;
	}
  }
#line 12514 "parser.c"
    break;

  case 1324: /* arithmetic_x_list: arithmetic_x  */
#line 6717 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12520 "parser.c"
    break;

  case 1325: /* arithmetic_x_list: arithmetic_x_list arithmetic_x  */
#line 6719 "parser.y"
                                { yyval = cb_list_append (yyvsp[-1], yyvsp[0]); }
#line 12526 "parser.c"
    break;

  case 1326: /* arithmetic_x: x flag_rounded  */
#line 6723 "parser.y"
                                { yyval = cb_build_pair (yyvsp[0], yyvsp[-1]); }
#line 12532 "parser.c"
    break;

  case 1327: /* record_name: qualified_word  */
#line 6730 "parser.y"
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
#line 12553 "parser.c"
    break;

  case 1328: /* table_name: qualified_word  */
#line 6752 "parser.y"
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
#line 12572 "parser.c"
    break;

  case 1329: /* file_name_list: file_name  */
#line 6772 "parser.y"
  {
	yyval = cb_list_init (yyvsp[0]);
  }
#line 12580 "parser.c"
    break;

  case 1330: /* file_name_list: file_name_list file_name  */
#line 6776 "parser.y"
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
#line 12597 "parser.c"
    break;

  case 1331: /* file_name: "Identifier"  */
#line 6792 "parser.y"
  {
	if (CB_FILE_P (cb_ref (yyvsp[0]))) {
		yyval = yyvsp[0];
	} else {
		cb_error_x (yyvsp[0], _("'%s' is not a file name"), CB_NAME (yyvsp[0]));
		yyval = cb_error_node;
	}
  }
#line 12610 "parser.c"
    break;

  case 1332: /* mnemonic_name_list: mnemonic_name  */
#line 6805 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12616 "parser.c"
    break;

  case 1333: /* mnemonic_name_list: mnemonic_name_list mnemonic_name  */
#line 6807 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12622 "parser.c"
    break;

  case 1334: /* mnemonic_name: "MNEMONIC NAME"  */
#line 6811 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12628 "parser.c"
    break;

  case 1335: /* procedure_name_list: %empty  */
#line 6817 "parser.y"
                                { yyval = NULL; }
#line 12634 "parser.c"
    break;

  case 1336: /* procedure_name_list: procedure_name_list procedure_name  */
#line 6819 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12640 "parser.c"
    break;

  case 1337: /* procedure_name: label  */
#line 6824 "parser.y"
  {
	yyval = yyvsp[0];
	CB_REFERENCE (yyval)->offset = CB_TREE (current_section);
	current_program->label_list = cb_cons (yyval, current_program->label_list);
  }
#line 12650 "parser.c"
    break;

  case 1341: /* integer_label: "Literal"  */
#line 6839 "parser.y"
  {
	yyval = cb_build_reference ((char *)(CB_LITERAL (yyvsp[0])->data));
	yyval->source_file = yyvsp[0]->source_file;
	yyval->source_line = yyvsp[0]->source_line;
  }
#line 12660 "parser.c"
    break;

  case 1342: /* reference_list: reference  */
#line 6849 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12666 "parser.c"
    break;

  case 1343: /* reference_list: reference_list reference  */
#line 6850 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12672 "parser.c"
    break;

  case 1344: /* reference: qualified_word  */
#line 6855 "parser.y"
  {
	yyval = yyvsp[0];
	current_program->reference_list = cb_cons (yyval, current_program->reference_list);
  }
#line 12681 "parser.c"
    break;

  case 1345: /* no_reference_list: qualified_word  */
#line 6864 "parser.y"
                                        { yyval = cb_list_init (yyvsp[0]); }
#line 12687 "parser.c"
    break;

  case 1346: /* no_reference_list: no_reference_list qualified_word  */
#line 6865 "parser.y"
                                        { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12693 "parser.c"
    break;

  case 1347: /* opt_reference: %empty  */
#line 6869 "parser.y"
                                { yyval = NULL; }
#line 12699 "parser.c"
    break;

  case 1348: /* opt_reference: reference  */
#line 6870 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12705 "parser.c"
    break;

  case 1351: /* undefined_word: "Identifier"  */
#line 6882 "parser.y"
  {
	yyval = yyvsp[0];
	if (CB_REFERENCE (yyval)->word->count > 0) {
		redefinition_error (yyval);
		yyval = cb_error_node;
	}
  }
#line 12717 "parser.c"
    break;

  case 1352: /* target_x_list: target_x  */
#line 6901 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12723 "parser.c"
    break;

  case 1353: /* target_x_list: target_x_list target_x  */
#line 6902 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12729 "parser.c"
    break;

  case 1355: /* target_x: ADDRESS _of identifier_1  */
#line 6907 "parser.y"
                                { yyval = cb_build_address (yyvsp[0]); }
#line 12735 "parser.c"
    break;

  case 1356: /* x_list: x  */
#line 6911 "parser.y"
                                { yyval = cb_list_init (yyvsp[0]); }
#line 12741 "parser.c"
    break;

  case 1357: /* x_list: x_list x  */
#line 6912 "parser.y"
                                { yyval = cb_list_add (yyvsp[-1], yyvsp[0]); }
#line 12747 "parser.c"
    break;

  case 1359: /* x: LENGTH _of identifier_1  */
#line 6917 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12753 "parser.c"
    break;

  case 1360: /* x: LENGTH _of basic_literal  */
#line 6918 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12759 "parser.c"
    break;

  case 1361: /* x: LENGTH _of function  */
#line 6919 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12765 "parser.c"
    break;

  case 1362: /* x: ADDRESS _of prog_or_entry alnum_or_id  */
#line 6920 "parser.y"
                                                { yyval = cb_build_ppointer (yyvsp[0]); }
#line 12771 "parser.c"
    break;

  case 1363: /* x: ADDRESS _of identifier_1  */
#line 6921 "parser.y"
                                                { yyval = cb_build_address (yyvsp[0]); }
#line 12777 "parser.c"
    break;

  case 1368: /* arith_x: LENGTH _of identifier_1  */
#line 6929 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12783 "parser.c"
    break;

  case 1369: /* arith_x: LENGTH _of basic_literal  */
#line 6930 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12789 "parser.c"
    break;

  case 1370: /* arith_x: LENGTH _of function  */
#line 6931 "parser.y"
                                                { yyval = cb_build_length (yyvsp[0]); }
#line 12795 "parser.c"
    break;

  case 1376: /* alnum_or_id: identifier_1  */
#line 6943 "parser.y"
                        { yyval = yyvsp[0]; }
#line 12801 "parser.c"
    break;

  case 1377: /* alnum_or_id: "Literal"  */
#line 6944 "parser.y"
                        { yyval = yyvsp[0]; }
#line 12807 "parser.c"
    break;

  case 1389: /* num_id_or_lit: ZERO  */
#line 6978 "parser.y"
                                { yyval = cb_zero; }
#line 12813 "parser.c"
    break;

  case 1390: /* identifier: identifier_1  */
#line 6986 "parser.y"
                                { yyval = cb_build_identifier (yyvsp[0]); }
#line 12819 "parser.c"
    break;

  case 1391: /* identifier_1: qualified_word  */
#line 6990 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12825 "parser.c"
    break;

  case 1392: /* identifier_1: qualified_word subref  */
#line 6991 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 12831 "parser.c"
    break;

  case 1393: /* identifier_1: qualified_word refmod  */
#line 6992 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 12837 "parser.c"
    break;

  case 1394: /* identifier_1: qualified_word subref refmod  */
#line 6993 "parser.y"
                                { yyval = yyvsp[-2]; }
#line 12843 "parser.c"
    break;

  case 1395: /* qualified_word: "Identifier"  */
#line 6997 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12849 "parser.c"
    break;

  case 1396: /* qualified_word: "Identifier" in_of qualified_word  */
#line 6998 "parser.y"
                                { yyval = yyvsp[-2]; CB_REFERENCE (yyvsp[-2])->chain = yyvsp[0]; }
#line 12855 "parser.c"
    break;

  case 1397: /* subref: '(' exp_list ')'  */
#line 7003 "parser.y"
  {
	if (cb_ref (yyvsp[-3]) != cb_error_node) {
		yyval = yyvsp[-3];
		CB_REFERENCE (yyvsp[-3])->subs = cb_list_reverse (yyvsp[-1]);
	}
  }
#line 12866 "parser.c"
    break;

  case 1398: /* refmod: '(' exp ':' ')'  */
#line 7013 "parser.y"
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
#line 12884 "parser.c"
    break;

  case 1399: /* refmod: '(' exp ':' exp ')'  */
#line 7027 "parser.y"
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
#line 12904 "parser.c"
    break;

  case 1400: /* integer: "Literal"  */
#line 7050 "parser.y"
  {
	if (cb_tree_category (yyvsp[0]) != CB_CATEGORY_NUMERIC) {
		cb_error (_("Integer value expected"));
	} else if (CB_LITERAL (yyvsp[0])->sign < 0 || CB_LITERAL (yyvsp[0])->scale) {
		cb_error (_("Integer value expected"));
	}
	yyval = yyvsp[0];
  }
#line 12917 "parser.c"
    break;

  case 1401: /* literal: basic_literal  */
#line 7061 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12923 "parser.c"
    break;

  case 1402: /* literal: ALL basic_value  */
#line 7063 "parser.y"
  {
	yyval = yyvsp[0];
	if (CB_LITERAL_P (yyvsp[0])) {
		CB_LITERAL (yyvsp[0])->all = 1;
	}
  }
#line 12934 "parser.c"
    break;

  case 1403: /* basic_literal: basic_value  */
#line 7072 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12940 "parser.c"
    break;

  case 1404: /* basic_literal: basic_literal '&' basic_value  */
#line 7073 "parser.y"
                                { yyval = cb_concat_literals (yyvsp[-2], yyvsp[0]); }
#line 12946 "parser.c"
    break;

  case 1405: /* basic_value: "Literal"  */
#line 7077 "parser.y"
                                { yyval = yyvsp[0]; }
#line 12952 "parser.c"
    break;

  case 1406: /* basic_value: SPACE  */
#line 7078 "parser.y"
                                { yyval = cb_space; }
#line 12958 "parser.c"
    break;

  case 1407: /* basic_value: ZERO  */
#line 7079 "parser.y"
                                { yyval = cb_zero; }
#line 12964 "parser.c"
    break;

  case 1408: /* basic_value: QUOTE  */
#line 7080 "parser.y"
                                { yyval = cb_quote; }
#line 12970 "parser.c"
    break;

  case 1409: /* basic_value: "HIGH-VALUE"  */
#line 7081 "parser.y"
                                { yyval = cb_high; }
#line 12976 "parser.c"
    break;

  case 1410: /* basic_value: "LOW-VALUE"  */
#line 7082 "parser.y"
                                { yyval = cb_low; }
#line 12982 "parser.c"
    break;

  case 1411: /* basic_value: "NULL"  */
#line 7083 "parser.y"
                                { yyval = cb_null; }
#line 12988 "parser.c"
    break;

  case 1412: /* function: "FUNCTION CURRENT-DATE" func_refmod  */
#line 7092 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-1], NULL, yyvsp[0]);
  }
#line 12996 "parser.c"
    break;

  case 1413: /* function: "FUNCTION WHEN-COMPILED" func_refmod  */
#line 7096 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-1], NULL, yyvsp[0]);
  }
#line 13004 "parser.c"
    break;

  case 1414: /* function: "FUNCTION UPPER-CASE" '(' exp ')' func_refmod  */
#line 7100 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], cb_list_init (yyvsp[-2]), yyvsp[0]);
  }
#line 13012 "parser.c"
    break;

  case 1415: /* function: "FUNCTION LOWER-CASE" '(' exp ')' func_refmod  */
#line 7104 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], cb_list_init (yyvsp[-2]), yyvsp[0]);
  }
#line 13020 "parser.c"
    break;

  case 1416: /* function: "FUNCTION REVERSE" '(' exp ')' func_refmod  */
#line 7108 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], cb_list_init (yyvsp[-2]), yyvsp[0]);
  }
#line 13028 "parser.c"
    break;

  case 1417: /* function: "FUNCTION CONCATENATE" '(' exp_list ')' func_refmod  */
#line 7112 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13036 "parser.c"
    break;

  case 1418: /* function: "FUNCTION SUBSTITUTE" '(' exp_list ')' func_refmod  */
#line 7116 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13044 "parser.c"
    break;

  case 1419: /* function: "FUNCTION SUBSTITUTE-CASE" '(' exp_list ')' func_refmod  */
#line 7120 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13052 "parser.c"
    break;

  case 1420: /* function: "FUNCTION TRIM" '(' trim_args ')' func_refmod  */
#line 7124 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13060 "parser.c"
    break;

  case 1421: /* function: "FUNCTION NUMVALC" '(' numvalc_args ')'  */
#line 7128 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-3], yyvsp[-1], NULL);
  }
#line 13068 "parser.c"
    break;

  case 1422: /* function: "FUNCTION LOCALE" '(' locale_dt_args ')' func_refmod  */
#line 7132 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-4], yyvsp[-2], yyvsp[0]);
  }
#line 13076 "parser.c"
    break;

  case 1423: /* function: "FUNCTION" func_args  */
#line 7136 "parser.y"
  {
	yyval = cb_build_intrinsic (yyvsp[-1], yyvsp[0], NULL);
  }
#line 13084 "parser.c"
    break;

  case 1424: /* func_refmod: %empty  */
#line 7142 "parser.y"
                                { yyval = NULL; }
#line 13090 "parser.c"
    break;

  case 1425: /* func_refmod: '(' exp ':' ')'  */
#line 7143 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-2], NULL); }
#line 13096 "parser.c"
    break;

  case 1426: /* func_refmod: '(' exp ':' exp ')'  */
#line 7144 "parser.y"
                                { yyval = cb_build_pair (yyvsp[-3], yyvsp[-1]); }
#line 13102 "parser.c"
    break;

  case 1427: /* func_args: %empty  */
#line 7148 "parser.y"
                                { yyval = NULL; }
#line 13108 "parser.c"
    break;

  case 1428: /* func_args: '(' list_func_args ')'  */
#line 7149 "parser.y"
                                { yyval = yyvsp[-1]; }
#line 13114 "parser.c"
    break;

  case 1429: /* list_func_args: %empty  */
#line 7153 "parser.y"
                                { yyval = NULL; }
#line 13120 "parser.c"
    break;

  case 1430: /* list_func_args: exp_list  */
#line 7154 "parser.y"
                                { yyval = yyvsp[0]; }
#line 13126 "parser.c"
    break;

  case 1431: /* trim_args: exp  */
#line 7160 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[0]);
	yyval = cb_list_add (x, cb_int0);
  }
#line 13137 "parser.c"
    break;

  case 1432: /* trim_args: exp e_sep LEADING  */
#line 7167 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, cb_int1);
  }
#line 13148 "parser.c"
    break;

  case 1433: /* trim_args: exp e_sep TRAILING  */
#line 7174 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, cb_int2);
  }
#line 13159 "parser.c"
    break;

  case 1434: /* numvalc_args: exp  */
#line 7184 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[0]);
	yyval = cb_list_add (x, cb_null);
  }
#line 13170 "parser.c"
    break;

  case 1435: /* numvalc_args: exp e_sep exp  */
#line 7191 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, yyvsp[0]);
  }
#line 13181 "parser.c"
    break;

  case 1436: /* locale_dt_args: exp  */
#line 7201 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[0]);
	yyval = cb_list_add (x, cb_null);
  }
#line 13192 "parser.c"
    break;

  case 1437: /* locale_dt_args: exp e_sep reference  */
#line 7208 "parser.y"
  {
	cb_tree	x;

	x = cb_list_init (yyvsp[-2]);
	yyval = cb_list_add (x, cb_ref (yyvsp[0]));
  }
#line 13203 "parser.c"
    break;

  case 1438: /* not_const_word: %empty  */
#line 7221 "parser.y"
  {
	non_const_word = 1;
  }
#line 13211 "parser.c"
    break;

  case 1439: /* flag_all: %empty  */
#line 7231 "parser.y"
                                { yyval = cb_int0; }
#line 13217 "parser.c"
    break;

  case 1440: /* flag_all: ALL  */
#line 7232 "parser.y"
                                { yyval = cb_int1; }
#line 13223 "parser.c"
    break;

  case 1441: /* flag_duplicates: %empty  */
#line 7236 "parser.y"
                                { yyval = cb_int0; }
#line 13229 "parser.c"
    break;

  case 1442: /* flag_duplicates: with_dups  */
#line 7237 "parser.y"
                                { yyval = cb_int1; }
#line 13235 "parser.c"
    break;

  case 1443: /* flag_initialized: %empty  */
#line 7241 "parser.y"
                                { yyval = NULL; }
#line 13241 "parser.c"
    break;

  case 1444: /* flag_initialized: INITIALIZED  */
#line 7242 "parser.y"
                                { yyval = cb_int1; }
#line 13247 "parser.c"
    break;

  case 1445: /* flag_next: %empty  */
#line 7246 "parser.y"
                                { yyval = cb_int0; }
#line 13253 "parser.c"
    break;

  case 1446: /* flag_next: NEXT  */
#line 7247 "parser.y"
                                { yyval = cb_int1; }
#line 13259 "parser.c"
    break;

  case 1447: /* flag_next: PREVIOUS  */
#line 7248 "parser.y"
                                { yyval = cb_int2; }
#line 13265 "parser.c"
    break;

  case 1448: /* flag_not: %empty  */
#line 7252 "parser.y"
                                { yyval = cb_int0; }
#line 13271 "parser.c"
    break;

  case 1449: /* flag_not: NOT  */
#line 7253 "parser.y"
                                { yyval = cb_int1; }
#line 13277 "parser.c"
    break;

  case 1450: /* flag_optional: %empty  */
#line 7257 "parser.y"
                                { yyval = cb_int0; }
#line 13283 "parser.c"
    break;

  case 1451: /* flag_optional: OPTIONAL  */
#line 7258 "parser.y"
                                { yyval = cb_int1; }
#line 13289 "parser.c"
    break;

  case 1452: /* flag_rounded: %empty  */
#line 7262 "parser.y"
                                { yyval = cb_int0; }
#line 13295 "parser.c"
    break;

  case 1453: /* flag_rounded: ROUNDED  */
#line 7263 "parser.y"
                                { yyval = cb_int1; }
#line 13301 "parser.c"
    break;

  case 1454: /* flag_separate: %empty  */
#line 7267 "parser.y"
                                { yyval = cb_int0; }
#line 13307 "parser.c"
    break;

  case 1455: /* flag_separate: SEPARATE _character  */
#line 7268 "parser.y"
                                { yyval = cb_int1; }
#line 13313 "parser.c"
    break;

  case 1467: /* _also: ALSO  */
#line 7281 "parser.y"
                       { yyval = cb_int1; }
#line 13319 "parser.c"
    break;

  case 1496: /* _is: %empty  */
#line 7296 "parser.y"
                { yyval = NULL; }
#line 13325 "parser.c"
    break;

  case 1497: /* _is: IS  */
#line 7296 "parser.y"
                                    { yyval = cb_int1; }
#line 13331 "parser.c"
    break;

  case 1508: /* _literal: %empty  */
#line 7301 "parser.y"
                { yyval = NULL; }
#line 13337 "parser.c"
    break;

  case 1509: /* _literal: "Literal"  */
#line 7301 "parser.y"
                                         { yyval = yyvsp[0]; }
#line 13343 "parser.c"
    break;

  case 1554: /* exec_sql_statement: "EXEC SQL statement"  */
#line 7331 "parser.y"
  {
	cb_tree sql_node;
	/* If no DECLARE SECTION was seen in DATA DIVISION, the first
	   EXEC SQL in PROCEDURE DIVISION also marks the program as ESQL
	   so SQLCA gets injected. */
	if (!esql_program_seen) {
		esql_program_seen = 1;
		esql_inject_sqlca ();
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
#line 13370 "parser.c"
    break;


#line 13374 "parser.c"

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

#line 7356 "parser.y"


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

/* Substitute the type names.  */
#define YYSTYPE         ESQL_STYPE
/* Substitute the variable and function names.  */
#define yyparse         esql_parse
#define yylex           esql_lex
#define yyerror         esql_error
#define yydebug         esql_debug
#define yynerrs         esql_nerrs
#define yylval          esql_lval
#define yychar          esql_char

/* First part of user prologue.  */
#line 8 "esql-parser.y"

#include <stdlib.h>
#include <string.h>
#include "esql-common.h"

int esql_lex(void);
void esql_error(const char *msg);
void esql_parser_init(void);

void esql_error(const char *msg) {
  cb_error("ESQL parse error: %s", msg);
}

/* Parse result: set by grammar actions, read by cb_parse_exec_sql */
cb_tree esql_parsed_result;

/* Parser-local state */
static struct cb_sql_host_var *esql_host_list;
static struct cb_sql_host_var *esql_res_host_list;
static int esql_host_count;
static int esql_res_host_count;
static char esql_cursor_name[256];
static char esql_prepare_name[256];
static char esql_db_name[256];
static int esql_conn_use_other_db;

void esql_parser_init(void) {
  esql_host_list = NULL;
  esql_res_host_list = NULL;
  esql_host_count = 0;
  esql_res_host_count = 0;
  memset(esql_cursor_name, 0, sizeof(esql_cursor_name));
  memset(esql_prepare_name, 0, sizeof(esql_prepare_name));
  memset(esql_db_name, 0, sizeof(esql_db_name));
  esql_conn_use_other_db = 0;
}

static void esql_add_host_var(const char *name) {
  cb_tree ref = cb_build_reference((char *)name);
  struct cb_sql_host_var *hv = cb_build_sql_host_var(strdup(name), ref);
  esql_host_list = cb_sql_host_var_list_add(esql_host_list, hv);
  esql_host_count++;
}

static void esql_add_res_host_var(const char *name) {
  cb_tree ref = cb_build_reference((char *)name);
  struct cb_sql_host_var *hv = cb_build_sql_host_var(strdup(name), ref);
  esql_res_host_list = cb_sql_host_var_list_add(esql_res_host_list, hv);
  esql_res_host_count++;
}

static void esql_set_cursor(const char *name) {
  strncpy(esql_cursor_name, name, sizeof(esql_cursor_name) - 1);
}

static void esql_set_prepare(const char *name) {
  strncpy(esql_prepare_name, name, sizeof(esql_prepare_name) - 1);
}

static void esql_set_dbname(const char *name) {
  strncpy(esql_db_name, name, sizeof(esql_db_name) - 1);
  esql_conn_use_other_db = 1;
}

static cb_tree esql_build_node(enum cb_sql_command cmd) {
  char *sql = strdup(esql_get_sqlbody());
  char *cur = esql_cursor_name[0] ? strdup(esql_cursor_name) : NULL;
  char *prep = esql_prepare_name[0] ? strdup(esql_prepare_name) : NULL;
  char *db = esql_conn_use_other_db ? strdup(esql_db_name) : NULL;
  return esql_build_and_resolve(cmd, sql, cur, prep, db,
                                esql_host_list, esql_host_count,
                                esql_res_host_list, esql_res_host_count,
                                esql_conn_use_other_db);
}


#line 156 "esql-parser.c"

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

#include "esql-parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ESQL_SELECT = 3,                /* ESQL_SELECT  */
  YYSYMBOL_ESQL_DISCONNECT = 4,            /* ESQL_DISCONNECT  */
  YYSYMBOL_ESQL_TOKEN = 5,                 /* ESQL_TOKEN  */
  YYSYMBOL_ESQL_HOSTTOKEN = 6,             /* ESQL_HOSTTOKEN  */
  YYSYMBOL_ESQL_CURNAME = 7,               /* ESQL_CURNAME  */
  YYSYMBOL_ESQL_OTHERFUNC = 8,             /* ESQL_OTHERFUNC  */
  YYSYMBOL_ESQL_INTO = 9,                  /* ESQL_INTO  */
  YYSYMBOL_ESQL_SELECTFROM = 10,           /* ESQL_SELECTFROM  */
  YYSYMBOL_ESQL_FOR = 11,                  /* ESQL_FOR  */
  YYSYMBOL_ESQL_CONNECT = 12,              /* ESQL_CONNECT  */
  YYSYMBOL_ESQL_DECLARE = 13,              /* ESQL_DECLARE  */
  YYSYMBOL_ESQL_CURSOR = 14,               /* ESQL_CURSOR  */
  YYSYMBOL_ESQL_OPEN = 15,                 /* ESQL_OPEN  */
  YYSYMBOL_ESQL_CLOSE = 16,                /* ESQL_CLOSE  */
  YYSYMBOL_ESQL_FETCH = 17,                /* ESQL_FETCH  */
  YYSYMBOL_ESQL_PREPARE = 18,              /* ESQL_PREPARE  */
  YYSYMBOL_ESQL_EXECUTE = 19,              /* ESQL_EXECUTE  */
  YYSYMBOL_ESQL_FROM = 20,                 /* ESQL_FROM  */
  YYSYMBOL_ESQL_COMMIT_WORK = 21,          /* ESQL_COMMIT_WORK  */
  YYSYMBOL_ESQL_ROLLBACK_WORK = 22,        /* ESQL_ROLLBACK_WORK  */
  YYSYMBOL_ESQL_IDENTIFIED_BY = 23,        /* ESQL_IDENTIFIED_BY  */
  YYSYMBOL_ESQL_USING = 24,                /* ESQL_USING  */
  YYSYMBOL_ESQL_AT = 25,                   /* ESQL_AT  */
  YYSYMBOL_ESQL_BEGIN_DECLARE = 26,        /* ESQL_BEGIN_DECLARE  */
  YYSYMBOL_ESQL_END_DECLARE = 27,          /* ESQL_END_DECLARE  */
  YYSYMBOL_ESQL_INCLUDE_SQLCA = 28,        /* ESQL_INCLUDE_SQLCA  */
  YYSYMBOL_ESQL_INCLUDE = 29,              /* ESQL_INCLUDE  */
  YYSYMBOL_ESQL_WHERECURRENTOF = 30,       /* ESQL_WHERECURRENTOF  */
  YYSYMBOL_YYACCEPT = 31,                  /* $accept  */
  YYSYMBOL_input = 32,                     /* input  */
  YYSYMBOL_esql_statement = 33,            /* esql_statement  */
  YYSYMBOL_connectsql = 34,                /* connectsql  */
  YYSYMBOL_connect_user = 35,              /* connect_user  */
  YYSYMBOL_identified = 36,                /* identified  */
  YYSYMBOL_using = 37,                     /* using  */
  YYSYMBOL_disconnectsql = 38,             /* disconnectsql  */
  YYSYMBOL_commitsql = 39,                 /* commitsql  */
  YYSYMBOL_rollbacksql = 40,               /* rollbacksql  */
  YYSYMBOL_declaresql = 41,                /* declaresql  */
  YYSYMBOL_opensql = 42,                   /* opensql  */
  YYSYMBOL_closesql = 43,                  /* closesql  */
  YYSYMBOL_fetchsql = 44,                  /* fetchsql  */
  YYSYMBOL_preparesql = 45,                /* preparesql  */
  YYSYMBOL_executesql = 46,                /* executesql  */
  YYSYMBOL_selectintosql = 47,             /* selectintosql  */
  YYSYMBOL_othersql = 48,                  /* othersql  */
  YYSYMBOL_declaresection = 49,            /* declaresection  */
  YYSYMBOL_includesql = 50,                /* includesql  */
  YYSYMBOL_otherdb = 51,                   /* otherdb  */
  YYSYMBOL_host_references = 52,           /* host_references  */
  YYSYMBOL_res_host_references = 53,       /* res_host_references  */
  YYSYMBOL_host_reference = 54,            /* host_reference  */
  YYSYMBOL_prepared_stname = 55,           /* prepared_stname  */
  YYSYMBOL_token_list = 56,                /* token_list  */
  YYSYMBOL_token_list_opt = 57,            /* token_list_opt  */
  YYSYMBOL_expr = 58                       /* expr  */
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
typedef yytype_int8 yy_state_t;

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
         || (defined ESQL_STYPE_IS_TRIVIAL && ESQL_STYPE_IS_TRIVIAL)))

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
#define YYFINAL  60
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   126

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  62
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  105

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   285


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30
};

#if ESQL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   107,   107,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   129,   133,   136,
     142,   148,   154,   161,   168,   174,   181,   188,   196,   200,
     208,   216,   224,   232,   235,   242,   248,   257,   265,   268,
     274,   277,   285,   286,   292,   295,   296,   302,   305,   306,
     312,   316,   323,   324,   325,   328,   333,   335,   339,   340,
     341,   342,   343
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if ESQL_DEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ESQL_SELECT",
  "ESQL_DISCONNECT", "ESQL_TOKEN", "ESQL_HOSTTOKEN", "ESQL_CURNAME",
  "ESQL_OTHERFUNC", "ESQL_INTO", "ESQL_SELECTFROM", "ESQL_FOR",
  "ESQL_CONNECT", "ESQL_DECLARE", "ESQL_CURSOR", "ESQL_OPEN", "ESQL_CLOSE",
  "ESQL_FETCH", "ESQL_PREPARE", "ESQL_EXECUTE", "ESQL_FROM",
  "ESQL_COMMIT_WORK", "ESQL_ROLLBACK_WORK", "ESQL_IDENTIFIED_BY",
  "ESQL_USING", "ESQL_AT", "ESQL_BEGIN_DECLARE", "ESQL_END_DECLARE",
  "ESQL_INCLUDE_SQLCA", "ESQL_INCLUDE", "ESQL_WHERECURRENTOF", "$accept",
  "input", "esql_statement", "connectsql", "connect_user", "identified",
  "using", "disconnectsql", "commitsql", "rollbacksql", "declaresql",
  "opensql", "closesql", "fetchsql", "preparesql", "executesql",
  "selectintosql", "othersql", "declaresection", "includesql", "otherdb",
  "host_references", "res_host_references", "host_reference",
  "prepared_stname", "token_list", "token_list_opt", "expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-24)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      97,   -20,   -20,   -20,     1,    42,    42,    42,    42,     3,
       3,   -20,   -20,    42,    42,   -24,    12,    32,   -24,   -24,
      -2,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,
     -24,   -24,   -24,   -24,    31,    42,    42,    42,   -24,   -24,
     -24,   -24,   -24,   -24,   -24,   -24,    20,   -20,   -20,   -20,
     -24,   -20,   -20,   -24,   -24,    19,   -24,   -24,   -24,   -24,
     -24,    40,    24,   -24,   -24,    33,   -24,    19,    46,    43,
     -24,    57,    49,    47,    63,   -24,   -24,   -24,    40,   -20,
      40,    28,    40,    40,    40,    40,   -24,   -24,   -24,    10,
     -24,    42,   -24,    35,   -24,    59,   -24,    35,   -24,    42,
     -24,    19,   -24,   -24,    19
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    42,    42,    42,    42,     0,     0,     0,     0,     0,
       0,    42,    42,    56,    56,    40,     0,     0,     2,     3,
      42,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,     0,     0,    56,     0,    50,    17,
      20,    59,    58,    62,    61,    60,     0,    42,    42,    42,
      51,    42,    42,    24,    25,    57,    38,    52,    39,    41,
       1,     0,     0,    19,    43,     0,    23,    37,     0,    28,
      30,     0,     0,    34,     0,    54,    53,    21,     0,    42,
      62,     0,     0,     0,     0,     0,    55,    22,    18,    36,
      47,     0,    27,    29,    44,    31,    32,    33,    48,     0,
      49,    26,    45,    46,    35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,
     -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,
       7,   -13,   -10,     0,    -4,   -23,    -1,    -5
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    17,    18,    19,    20,    62,    79,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      35,    93,    89,    75,    51,    55,    56,    57
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      46,    47,    48,    49,    40,    34,    52,    38,    50,    36,
      37,    39,    65,    58,    67,    98,    38,    59,    53,    54,
      99,    61,    41,    34,    42,    38,    34,    63,    43,    44,
      45,    91,    60,    50,    68,    66,    41,    64,    42,    38,
     102,    38,    80,    44,    45,    41,    38,    42,    78,    74,
      76,    43,    44,    45,    69,    70,    71,    81,    72,    73,
      76,    77,    76,    74,    98,    38,    83,    82,   101,    84,
      86,    85,    97,    95,     0,     0,   104,    92,    87,     0,
      90,     0,    94,    90,    96,    94,    88,     0,     0,   100,
       0,     0,     0,   103,     0,   100,    76,   103,     0,    76,
       1,     2,     0,     0,     0,     3,     0,     0,     0,     4,
       5,     0,     6,     7,     8,     9,    10,     0,    11,    12,
       0,     0,     0,    13,    14,    15,    16
};

static const yytype_int8 yycheck[] =
{
       5,     6,     7,     8,     4,    25,    10,     6,     5,     2,
       3,     4,    35,    14,    37,     5,     6,     5,    11,    12,
      10,    23,     3,    25,     5,     6,    25,    20,     9,    10,
      11,     3,     0,     5,    14,    36,     3,     6,     5,     6,
       5,     6,     9,    10,    11,     3,     6,     5,    24,    30,
      55,     9,    10,    11,    47,    48,    49,    11,    51,    52,
      65,    61,    67,    30,     5,     6,     9,    24,    91,    20,
       7,    24,    85,    83,    -1,    -1,    99,    81,    78,    -1,
      80,    -1,    82,    83,    84,    85,    79,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    95,   101,    97,    -1,   104,
       3,     4,    -1,    -1,    -1,     8,    -1,    -1,    -1,    12,
      13,    -1,    15,    16,    17,    18,    19,    -1,    21,    22,
      -1,    -1,    -1,    26,    27,    28,    29
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     8,    12,    13,    15,    16,    17,    18,
      19,    21,    22,    26,    27,    28,    29,    32,    33,    34,
      35,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    25,    51,    51,    51,     6,    51,
      54,     3,     5,     9,    10,    11,    58,    58,    58,    58,
       5,    55,    55,    51,    51,    56,    57,    58,    57,     5,
       0,    23,    36,    51,     6,    56,    57,    56,    14,    51,
      51,    51,    51,    51,    30,    54,    58,    54,    24,    37,
       9,    11,    24,     9,    20,    24,     7,    54,    51,    53,
      54,     3,    55,    52,    54,    53,    54,    52,     5,    10,
      54,    56,     5,    54,    56
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    31,    32,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    34,    34,    34,
      35,    36,    37,    38,    39,    40,    41,    41,    42,    42,
      43,    44,    45,    46,    46,    47,    47,    48,    49,    49,
      50,    50,    51,    51,    52,    52,    52,    53,    53,    53,
      54,    55,    56,    56,    56,    56,    57,    57,    58,    58,
      58,    58,    58
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     4,     2,
       2,     2,     2,     3,     2,     2,     6,     5,     3,     5,
       3,     5,     5,     5,     3,     7,     5,     3,     2,     2,
       1,     2,     0,     2,     1,     2,     2,     1,     2,     2,
       1,     1,     1,     2,     2,     3,     0,     1,     1,     1,
       1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = ESQL_EMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == ESQL_EMPTY)                                        \
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
   Use ESQL_error or ESQL_UNDEF. */
#define YYERRCODE ESQL_UNDEF


/* Enable debugging if requested.  */
#if ESQL_DEBUG

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
#else /* !ESQL_DEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !ESQL_DEBUG */


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

  yychar = ESQL_EMPTY; /* Cause a token to be read.  */

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
  if (yychar == ESQL_EMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= ESQL_EOF)
    {
      yychar = ESQL_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == ESQL_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = ESQL_UNDEF;
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
  yychar = ESQL_EMPTY;
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
  case 17: /* connectsql: ESQL_CONNECT otherdb  */
#line 129 "esql-parser.y"
                         {
      /* CONNECT (short form) */
      esql_parsed_result = esql_build_node(CB_SQL_CONNECT_SHORT);
    }
#line 1281 "esql-parser.c"
    break;

  case 18: /* connectsql: connect_user identified using otherdb  */
#line 133 "esql-parser.y"
                                          {
      esql_parsed_result = esql_build_node(CB_SQL_CONNECT);
    }
#line 1289 "esql-parser.c"
    break;

  case 19: /* connectsql: connect_user otherdb  */
#line 136 "esql-parser.y"
                         {
      esql_parsed_result = esql_build_node(CB_SQL_CONNECT_INFORMAL);
    }
#line 1297 "esql-parser.c"
    break;

  case 20: /* connect_user: ESQL_CONNECT host_reference  */
#line 142 "esql-parser.y"
                                {
      esql_add_host_var((yyvsp[0].s));
    }
#line 1305 "esql-parser.c"
    break;

  case 21: /* identified: ESQL_IDENTIFIED_BY host_reference  */
#line 148 "esql-parser.y"
                                      {
      esql_add_host_var((yyvsp[0].s));
    }
#line 1313 "esql-parser.c"
    break;

  case 22: /* using: ESQL_USING host_reference  */
#line 154 "esql-parser.y"
                              {
      esql_add_host_var((yyvsp[0].s));
    }
#line 1321 "esql-parser.c"
    break;

  case 23: /* disconnectsql: ESQL_DISCONNECT otherdb token_list_opt  */
#line 161 "esql-parser.y"
                                           {
      esql_parsed_result = esql_build_node(CB_SQL_DISCONNECT);
    }
#line 1329 "esql-parser.c"
    break;

  case 24: /* commitsql: ESQL_COMMIT_WORK otherdb  */
#line 168 "esql-parser.y"
                             {
      esql_parsed_result = esql_build_node(CB_SQL_COMMIT);
    }
#line 1337 "esql-parser.c"
    break;

  case 25: /* rollbacksql: ESQL_ROLLBACK_WORK otherdb  */
#line 174 "esql-parser.y"
                               {
      esql_parsed_result = esql_build_node(CB_SQL_ROLLBACK);
    }
#line 1345 "esql-parser.c"
    break;

  case 26: /* declaresql: ESQL_DECLARE expr ESQL_CURSOR ESQL_FOR ESQL_SELECT token_list  */
#line 182 "esql-parser.y"
                             {
      esql_set_cursor((yyvsp[-4].s));
      esql_parsed_result = esql_build_node(
        esql_host_count > 0 ? CB_SQL_DECLARE_CURSOR_PARAMS
                            : CB_SQL_DECLARE_CURSOR);
    }
#line 1356 "esql-parser.c"
    break;

  case 27: /* declaresql: ESQL_DECLARE expr ESQL_CURSOR ESQL_FOR prepared_stname  */
#line 188 "esql-parser.y"
                                                           {
      esql_set_cursor((yyvsp[-3].s));
      esql_parsed_result = esql_build_node(CB_SQL_DECLARE_CURSOR);
    }
#line 1365 "esql-parser.c"
    break;

  case 28: /* opensql: ESQL_OPEN expr otherdb  */
#line 196 "esql-parser.y"
                           {
      esql_set_cursor((yyvsp[-1].s));
      esql_parsed_result = esql_build_node(CB_SQL_OPEN_CURSOR);
    }
#line 1374 "esql-parser.c"
    break;

  case 29: /* opensql: ESQL_OPEN expr otherdb ESQL_USING host_references  */
#line 200 "esql-parser.y"
                                                      {
      esql_set_cursor((yyvsp[-3].s));
      esql_parsed_result = esql_build_node(CB_SQL_OPEN_CURSOR_PARAMS);
    }
#line 1383 "esql-parser.c"
    break;

  case 30: /* closesql: ESQL_CLOSE expr otherdb  */
#line 208 "esql-parser.y"
                            {
      esql_set_cursor((yyvsp[-1].s));
      esql_parsed_result = esql_build_node(CB_SQL_CLOSE_CURSOR);
    }
#line 1392 "esql-parser.c"
    break;

  case 31: /* fetchsql: ESQL_FETCH expr otherdb ESQL_INTO res_host_references  */
#line 216 "esql-parser.y"
                                                          {
      esql_set_cursor((yyvsp[-3].s));
      esql_parsed_result = esql_build_node(CB_SQL_FETCH_ONE);
    }
#line 1401 "esql-parser.c"
    break;

  case 32: /* preparesql: ESQL_PREPARE prepared_stname otherdb ESQL_FROM host_reference  */
#line 224 "esql-parser.y"
                                                                  {
      esql_add_host_var((yyvsp[0].s));
      esql_parsed_result = esql_build_node(CB_SQL_PREPARE);
    }
#line 1410 "esql-parser.c"
    break;

  case 33: /* executesql: ESQL_EXECUTE prepared_stname otherdb ESQL_USING host_references  */
#line 232 "esql-parser.y"
                                                                    {
      esql_parsed_result = esql_build_node(CB_SQL_EXECUTE_PREPARED);
    }
#line 1418 "esql-parser.c"
    break;

  case 34: /* executesql: ESQL_EXECUTE prepared_stname otherdb  */
#line 235 "esql-parser.y"
                                         {
      esql_parsed_result = esql_build_node(CB_SQL_EXECUTE_PREPARED);
    }
#line 1426 "esql-parser.c"
    break;

  case 35: /* selectintosql: ESQL_SELECT otherdb token_list ESQL_INTO res_host_references ESQL_SELECTFROM token_list  */
#line 243 "esql-parser.y"
                                 {
      esql_parsed_result = esql_build_node(
        (esql_host_count > 0 || esql_res_host_count > 0)
          ? CB_SQL_SELECT_INTO_ONE : CB_SQL_EXEC);
    }
#line 1436 "esql-parser.c"
    break;

  case 36: /* selectintosql: ESQL_SELECT otherdb token_list ESQL_INTO res_host_references  */
#line 248 "esql-parser.y"
                                                                 {
      esql_parsed_result = esql_build_node(
        (esql_host_count > 0 || esql_res_host_count > 0)
          ? CB_SQL_SELECT_INTO_ONE : CB_SQL_EXEC);
    }
#line 1446 "esql-parser.c"
    break;

  case 37: /* othersql: ESQL_OTHERFUNC otherdb token_list  */
#line 257 "esql-parser.y"
                                      {
      esql_parsed_result = esql_build_node(
        esql_host_count > 0 ? CB_SQL_EXEC_PARAMS : CB_SQL_EXEC);
    }
#line 1455 "esql-parser.c"
    break;

  case 38: /* declaresection: ESQL_BEGIN_DECLARE token_list_opt  */
#line 265 "esql-parser.y"
                                      {
      esql_parsed_result = cb_error_node;
    }
#line 1463 "esql-parser.c"
    break;

  case 39: /* declaresection: ESQL_END_DECLARE token_list_opt  */
#line 268 "esql-parser.y"
                                    {
      esql_parsed_result = cb_error_node;
    }
#line 1471 "esql-parser.c"
    break;

  case 40: /* includesql: ESQL_INCLUDE_SQLCA  */
#line 274 "esql-parser.y"
                       {
      esql_parsed_result = cb_error_node;
    }
#line 1479 "esql-parser.c"
    break;

  case 41: /* includesql: ESQL_INCLUDE ESQL_TOKEN  */
#line 277 "esql-parser.y"
                            {
      /* INCLUDE filename - handled at preprocessor level */
      esql_parsed_result = cb_error_node;
    }
#line 1488 "esql-parser.c"
    break;

  case 42: /* otherdb: %empty  */
#line 285 "esql-parser.y"
                { }
#line 1494 "esql-parser.c"
    break;

  case 43: /* otherdb: ESQL_AT ESQL_HOSTTOKEN  */
#line 286 "esql-parser.y"
                           {
      esql_set_dbname((yyvsp[0].s));
    }
#line 1502 "esql-parser.c"
    break;

  case 44: /* host_references: host_reference  */
#line 292 "esql-parser.y"
                   {
      esql_add_host_var((yyvsp[0].s));
    }
#line 1510 "esql-parser.c"
    break;

  case 46: /* host_references: host_references host_reference  */
#line 296 "esql-parser.y"
                                   {
      esql_add_host_var((yyvsp[0].s));
    }
#line 1518 "esql-parser.c"
    break;

  case 47: /* res_host_references: host_reference  */
#line 302 "esql-parser.y"
                   {
      esql_add_res_host_var((yyvsp[0].s));
    }
#line 1526 "esql-parser.c"
    break;

  case 49: /* res_host_references: res_host_references host_reference  */
#line 306 "esql-parser.y"
                                       {
      esql_add_res_host_var((yyvsp[0].s));
    }
#line 1534 "esql-parser.c"
    break;

  case 50: /* host_reference: ESQL_HOSTTOKEN  */
#line 312 "esql-parser.y"
                   { (yyval.s) = (yyvsp[0].s); }
#line 1540 "esql-parser.c"
    break;

  case 51: /* prepared_stname: ESQL_TOKEN  */
#line 316 "esql-parser.y"
               {
      esql_set_prepare((yyvsp[0].s));
      (yyval.s) = (yyvsp[0].s);
    }
#line 1549 "esql-parser.c"
    break;

  case 54: /* token_list: token_list host_reference  */
#line 325 "esql-parser.y"
                              {
      esql_add_host_var((yyvsp[0].s));
    }
#line 1557 "esql-parser.c"
    break;

  case 55: /* token_list: token_list ESQL_WHERECURRENTOF ESQL_CURNAME  */
#line 328 "esql-parser.y"
                                                {
      esql_set_cursor((yyvsp[0].s));
    }
#line 1565 "esql-parser.c"
    break;

  case 58: /* expr: ESQL_TOKEN  */
#line 339 "esql-parser.y"
               { (yyval.s) = (yyvsp[0].s); }
#line 1571 "esql-parser.c"
    break;

  case 59: /* expr: ESQL_SELECT  */
#line 340 "esql-parser.y"
                { (yyval.s) = (yyvsp[0].s); }
#line 1577 "esql-parser.c"
    break;

  case 60: /* expr: ESQL_FOR  */
#line 341 "esql-parser.y"
             { (yyval.s) = (yyvsp[0].s); }
#line 1583 "esql-parser.c"
    break;

  case 61: /* expr: ESQL_SELECTFROM  */
#line 342 "esql-parser.y"
                    { (yyval.s) = (yyvsp[0].s); }
#line 1589 "esql-parser.c"
    break;

  case 62: /* expr: ESQL_INTO  */
#line 343 "esql-parser.y"
              { (yyval.s) = (yyvsp[0].s); }
#line 1595 "esql-parser.c"
    break;


#line 1599 "esql-parser.c"

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
  yytoken = yychar == ESQL_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
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

      if (yychar <= ESQL_EOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == ESQL_EOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = ESQL_EMPTY;
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
  if (yychar != ESQL_EMPTY)
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

#line 346 "esql-parser.y"


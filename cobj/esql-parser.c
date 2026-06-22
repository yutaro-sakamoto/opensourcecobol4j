/* A Bison parser, made by GNU Bison 3.7.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#define YYBISON 30704

/* Bison version string.  */
#define YYBISON_VERSION "3.7.4"

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
/* host_reference の :VAR(添字) で構築した subs を、
   親規則の esql_add_host_var が消費するためのサイドチャネル。
   各 host_reference の還元直後に親規則が必ず消費するので、
   1 つの静的変数で十分。 */
static cb_tree esql_pending_subs;

void esql_parser_init(void) {
  esql_host_list = NULL;
  esql_res_host_list = NULL;
  esql_host_count = 0;
  esql_res_host_count = 0;
  memset(esql_cursor_name, 0, sizeof(esql_cursor_name));
  memset(esql_prepare_name, 0, sizeof(esql_prepare_name));
  memset(esql_db_name, 0, sizeof(esql_db_name));
  esql_conn_use_other_db = 0;
  esql_pending_subs = NULL;
}

static void esql_add_host_var(const char *name) {
  cb_tree ref = esql_build_qualified_ref(name);
  const char *leaf = esql_qualified_leaf_name(name);
  if (esql_pending_subs) {
    esql_set_ref_subs(ref, esql_pending_subs);
    esql_pending_subs = NULL;
  }
  struct cb_sql_host_var *hv = cb_build_sql_host_var(strdup(leaf), ref);
  esql_host_list = cb_sql_host_var_list_add(esql_host_list, hv);
  esql_host_count++;
}

static void esql_add_res_host_var(const char *name) {
  cb_tree ref = esql_build_qualified_ref(name);
  const char *leaf = esql_qualified_leaf_name(name);
  if (esql_pending_subs) {
    esql_set_ref_subs(ref, esql_pending_subs);
    esql_pending_subs = NULL;
  }
  struct cb_sql_host_var *hv = cb_build_sql_host_var(strdup(leaf), ref);
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


#line 172 "esql-parser.c"

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
  YYSYMBOL_ESQL_HOSTSUB_NUMBER = 12,       /* ESQL_HOSTSUB_NUMBER  */
  YYSYMBOL_ESQL_HOSTSUB_IDENT = 13,        /* ESQL_HOSTSUB_IDENT  */
  YYSYMBOL_ESQL_HOSTSUB_LPAREN = 14,       /* ESQL_HOSTSUB_LPAREN  */
  YYSYMBOL_ESQL_HOSTSUB_RPAREN = 15,       /* ESQL_HOSTSUB_RPAREN  */
  YYSYMBOL_ESQL_HOSTSUB_COMMA = 16,        /* ESQL_HOSTSUB_COMMA  */
  YYSYMBOL_ESQL_CONNECT = 17,              /* ESQL_CONNECT  */
  YYSYMBOL_ESQL_DECLARE = 18,              /* ESQL_DECLARE  */
  YYSYMBOL_ESQL_CURSOR = 19,               /* ESQL_CURSOR  */
  YYSYMBOL_ESQL_OPEN = 20,                 /* ESQL_OPEN  */
  YYSYMBOL_ESQL_CLOSE = 21,                /* ESQL_CLOSE  */
  YYSYMBOL_ESQL_FETCH = 22,                /* ESQL_FETCH  */
  YYSYMBOL_ESQL_PREPARE = 23,              /* ESQL_PREPARE  */
  YYSYMBOL_ESQL_EXECUTE = 24,              /* ESQL_EXECUTE  */
  YYSYMBOL_ESQL_FROM = 25,                 /* ESQL_FROM  */
  YYSYMBOL_ESQL_COMMIT_WORK = 26,          /* ESQL_COMMIT_WORK  */
  YYSYMBOL_ESQL_ROLLBACK_WORK = 27,        /* ESQL_ROLLBACK_WORK  */
  YYSYMBOL_ESQL_IDENTIFIED_BY = 28,        /* ESQL_IDENTIFIED_BY  */
  YYSYMBOL_ESQL_USING = 29,                /* ESQL_USING  */
  YYSYMBOL_ESQL_AT = 30,                   /* ESQL_AT  */
  YYSYMBOL_ESQL_WHERECURRENTOF = 31,       /* ESQL_WHERECURRENTOF  */
  YYSYMBOL_YYACCEPT = 32,                  /* $accept  */
  YYSYMBOL_input = 33,                     /* input  */
  YYSYMBOL_esql_statement = 34,            /* esql_statement  */
  YYSYMBOL_connectsql = 35,                /* connectsql  */
  YYSYMBOL_connect_user = 36,              /* connect_user  */
  YYSYMBOL_identified = 37,                /* identified  */
  YYSYMBOL_using = 38,                     /* using  */
  YYSYMBOL_disconnectsql = 39,             /* disconnectsql  */
  YYSYMBOL_commitsql = 40,                 /* commitsql  */
  YYSYMBOL_rollbacksql = 41,               /* rollbacksql  */
  YYSYMBOL_declaresql = 42,                /* declaresql  */
  YYSYMBOL_opensql = 43,                   /* opensql  */
  YYSYMBOL_closesql = 44,                  /* closesql  */
  YYSYMBOL_fetchsql = 45,                  /* fetchsql  */
  YYSYMBOL_preparesql = 46,                /* preparesql  */
  YYSYMBOL_executesql = 47,                /* executesql  */
  YYSYMBOL_selectintosql = 48,             /* selectintosql  */
  YYSYMBOL_othersql = 49,                  /* othersql  */
  YYSYMBOL_otherdb = 50,                   /* otherdb  */
  YYSYMBOL_host_references = 51,           /* host_references  */
  YYSYMBOL_res_host_references = 52,       /* res_host_references  */
  YYSYMBOL_host_reference = 53,            /* host_reference  */
  YYSYMBOL_subscript_list = 54,            /* subscript_list  */
  YYSYMBOL_subscript = 55,                 /* subscript  */
  YYSYMBOL_prepared_stname = 56,           /* prepared_stname  */
  YYSYMBOL_token_list = 57,                /* token_list  */
  YYSYMBOL_token_list_opt = 58,            /* token_list_opt  */
  YYSYMBOL_expr = 59                       /* expr  */
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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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
#define YYFINAL  49
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   112

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  104

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   286


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
      25,    26,    27,    28,    29,    30,    31
};

#if ESQL_DEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   125,   125,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   145,   149,   152,   158,   164,
     170,   177,   184,   190,   197,   204,   212,   216,   224,   232,
     240,   248,   251,   258,   264,   273,   281,   282,   288,   291,
     292,   298,   301,   302,   308,   312,   321,   324,   330,   333,
     342,   349,   350,   351,   354,   359,   361,   365,   366,   367,
     368,   369
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
  "ESQL_HOSTSUB_NUMBER", "ESQL_HOSTSUB_IDENT", "ESQL_HOSTSUB_LPAREN",
  "ESQL_HOSTSUB_RPAREN", "ESQL_HOSTSUB_COMMA", "ESQL_CONNECT",
  "ESQL_DECLARE", "ESQL_CURSOR", "ESQL_OPEN", "ESQL_CLOSE", "ESQL_FETCH",
  "ESQL_PREPARE", "ESQL_EXECUTE", "ESQL_FROM", "ESQL_COMMIT_WORK",
  "ESQL_ROLLBACK_WORK", "ESQL_IDENTIFIED_BY", "ESQL_USING", "ESQL_AT",
  "ESQL_WHERECURRENTOF", "$accept", "input", "esql_statement",
  "connectsql", "connect_user", "identified", "using", "disconnectsql",
  "commitsql", "rollbacksql", "declaresql", "opensql", "closesql",
  "fetchsql", "preparesql", "executesql", "selectintosql", "othersql",
  "otherdb", "host_references", "res_host_references", "host_reference",
  "subscript_list", "subscript", "prepared_stname", "token_list",
  "token_list_opt", "expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286
};
#endif

#define YYPACT_NINF (-27)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      85,   -24,   -24,   -24,    11,    67,    67,    67,    67,    26,
      26,   -24,   -24,    50,   -27,   -27,    -7,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,     5,    67,
      67,    67,     2,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
      33,   -24,   -24,   -24,   -27,   -24,   -24,   -27,   -27,   -27,
      36,    25,   -27,   -27,     9,   -27,    27,   -27,    27,    16,
      44,    30,   -27,    48,    35,    32,   -27,    36,   -24,    36,
      55,   -27,   -27,   -27,   -27,    28,   -27,    19,    36,    36,
      36,    36,   -27,   -27,     3,   -27,   -27,   -27,    16,    67,
     -27,    40,   -27,    42,   -27,    40,   -27,    67,   -27,   -27,
      27,   -27,   -27,    27
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    36,    36,    36,    36,     0,     0,     0,     0,     0,
       0,    36,    36,     0,     2,     3,    36,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,     0,     0,
      55,     0,    44,    15,    18,    58,    57,    61,    60,    59,
       0,    36,    36,    36,    50,    36,    36,    22,    23,     1,
       0,     0,    17,    37,     0,    51,    56,    21,    35,     0,
       0,    26,    28,     0,     0,    32,    19,     0,    36,    61,
       0,    53,    52,    48,    49,     0,    46,     0,     0,     0,
       0,     0,    20,    16,    34,    41,    54,    45,     0,     0,
      25,    27,    38,    29,    30,    31,    42,     0,    43,    47,
      24,    39,    40,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,    23,   -14,
       0,     6,   -27,    -8,    -3,   -26,   -27,    -5
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    51,    68,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    29,    91,
      84,    71,    75,    76,    45,    54,    57,    55
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      40,    41,    42,    43,    56,    58,    28,    46,    96,    32,
      34,    53,    35,    97,    36,    32,    59,    32,    69,    38,
      39,    50,    89,    28,    44,    30,    31,    33,    73,    74,
      35,    44,    36,    32,    47,    48,    37,    38,    39,    52,
      70,    28,    32,    87,    88,   101,    32,    96,    32,    72,
      49,    72,    60,    72,    67,    77,    66,    79,    70,    78,
      80,    81,    86,   100,    61,    62,    63,    95,    64,    65,
      35,   103,    36,    82,    90,    85,    37,    38,    39,    93,
      99,     0,     0,     0,    92,    85,    94,    92,     1,     2,
      98,    83,     0,     3,     0,    72,     0,   102,    72,    98,
       0,   102,     4,     5,     0,     6,     7,     8,     9,    10,
       0,    11,    12
};

static const yytype_int8 yycheck[] =
{
       5,     6,     7,     8,    30,    31,    30,    10,     5,     6,
       4,     6,     3,    10,     5,     6,    14,     6,     9,    10,
      11,    28,     3,    30,     5,     2,     3,     4,    12,    13,
       3,     5,     5,     6,    11,    12,     9,    10,    11,    16,
      31,    30,     6,    15,    16,     5,     6,     5,     6,    54,
       0,    56,    19,    58,    29,    11,    50,     9,    31,    29,
      25,    29,     7,    89,    41,    42,    43,    81,    45,    46,
       3,    97,     5,    67,    77,    69,     9,    10,    11,    79,
      88,    -1,    -1,    -1,    78,    79,    80,    81,     3,     4,
      84,    68,    -1,     8,    -1,   100,    -1,    91,   103,    93,
      -1,    95,    17,    18,    -1,    20,    21,    22,    23,    24,
      -1,    26,    27
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     8,    17,    18,    20,    21,    22,    23,
      24,    26,    27,    33,    34,    35,    36,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    30,    50,
      50,    50,     6,    50,    53,     3,     5,     9,    10,    11,
      59,    59,    59,    59,     5,    56,    56,    50,    50,     0,
      28,    37,    50,     6,    57,    59,    57,    58,    57,    14,
      19,    50,    50,    50,    50,    50,    53,    29,    38,     9,
      31,    53,    59,    12,    13,    54,    55,    11,    29,     9,
      25,    29,    53,    50,    52,    53,     7,    15,    16,     3,
      56,    51,    53,    52,    53,    51,     5,    10,    53,    55,
      57,     5,    53,    57
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    32,    33,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    35,    35,    35,    36,    37,
      38,    39,    40,    41,    42,    42,    43,    43,    44,    45,
      46,    47,    47,    48,    48,    49,    50,    50,    51,    51,
      51,    52,    52,    52,    53,    53,    54,    54,    55,    55,
      56,    57,    57,    57,    57,    58,    58,    59,    59,    59,
      59,    59
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     4,     2,     2,     2,
       2,     3,     2,     2,     6,     5,     3,     5,     3,     5,
       5,     5,     3,     7,     5,     3,     0,     2,     1,     2,
       2,     1,     2,     2,     1,     4,     1,     3,     1,     1,
       1,     1,     2,     2,     3,     0,     1,     1,     1,     1,
       1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = ESQL_EMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


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
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
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
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
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
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
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
  case 15: /* connectsql: ESQL_CONNECT otherdb  */
#line 145 "esql-parser.y"
                         {
      /* CONNECT (short form) */
      esql_parsed_result = esql_build_node(CB_SQL_CONNECT_SHORT);
    }
#line 1295 "esql-parser.c"
    break;

  case 16: /* connectsql: connect_user identified using otherdb  */
#line 149 "esql-parser.y"
                                          {
      esql_parsed_result = esql_build_node(CB_SQL_CONNECT);
    }
#line 1303 "esql-parser.c"
    break;

  case 17: /* connectsql: connect_user otherdb  */
#line 152 "esql-parser.y"
                         {
      esql_parsed_result = esql_build_node(CB_SQL_CONNECT_INFORMAL);
    }
#line 1311 "esql-parser.c"
    break;

  case 18: /* connect_user: ESQL_CONNECT host_reference  */
#line 158 "esql-parser.y"
                                {
      esql_add_host_var((yyvsp[0].s));
    }
#line 1319 "esql-parser.c"
    break;

  case 19: /* identified: ESQL_IDENTIFIED_BY host_reference  */
#line 164 "esql-parser.y"
                                      {
      esql_add_host_var((yyvsp[0].s));
    }
#line 1327 "esql-parser.c"
    break;

  case 20: /* using: ESQL_USING host_reference  */
#line 170 "esql-parser.y"
                              {
      esql_add_host_var((yyvsp[0].s));
    }
#line 1335 "esql-parser.c"
    break;

  case 21: /* disconnectsql: ESQL_DISCONNECT otherdb token_list_opt  */
#line 177 "esql-parser.y"
                                           {
      esql_parsed_result = esql_build_node(CB_SQL_DISCONNECT);
    }
#line 1343 "esql-parser.c"
    break;

  case 22: /* commitsql: ESQL_COMMIT_WORK otherdb  */
#line 184 "esql-parser.y"
                             {
      esql_parsed_result = esql_build_node(CB_SQL_COMMIT);
    }
#line 1351 "esql-parser.c"
    break;

  case 23: /* rollbacksql: ESQL_ROLLBACK_WORK otherdb  */
#line 190 "esql-parser.y"
                               {
      esql_parsed_result = esql_build_node(CB_SQL_ROLLBACK);
    }
#line 1359 "esql-parser.c"
    break;

  case 24: /* declaresql: ESQL_DECLARE expr ESQL_CURSOR ESQL_FOR ESQL_SELECT token_list  */
#line 198 "esql-parser.y"
                             {
      esql_set_cursor((yyvsp[-4].s));
      esql_parsed_result = esql_build_node(
        esql_host_count > 0 ? CB_SQL_DECLARE_CURSOR_PARAMS
                            : CB_SQL_DECLARE_CURSOR);
    }
#line 1370 "esql-parser.c"
    break;

  case 25: /* declaresql: ESQL_DECLARE expr ESQL_CURSOR ESQL_FOR prepared_stname  */
#line 204 "esql-parser.y"
                                                           {
      esql_set_cursor((yyvsp[-3].s));
      esql_parsed_result = esql_build_node(CB_SQL_DECLARE_CURSOR);
    }
#line 1379 "esql-parser.c"
    break;

  case 26: /* opensql: ESQL_OPEN expr otherdb  */
#line 212 "esql-parser.y"
                           {
      esql_set_cursor((yyvsp[-1].s));
      esql_parsed_result = esql_build_node(CB_SQL_OPEN_CURSOR);
    }
#line 1388 "esql-parser.c"
    break;

  case 27: /* opensql: ESQL_OPEN expr otherdb ESQL_USING host_references  */
#line 216 "esql-parser.y"
                                                      {
      esql_set_cursor((yyvsp[-3].s));
      esql_parsed_result = esql_build_node(CB_SQL_OPEN_CURSOR_PARAMS);
    }
#line 1397 "esql-parser.c"
    break;

  case 28: /* closesql: ESQL_CLOSE expr otherdb  */
#line 224 "esql-parser.y"
                            {
      esql_set_cursor((yyvsp[-1].s));
      esql_parsed_result = esql_build_node(CB_SQL_CLOSE_CURSOR);
    }
#line 1406 "esql-parser.c"
    break;

  case 29: /* fetchsql: ESQL_FETCH expr otherdb ESQL_INTO res_host_references  */
#line 232 "esql-parser.y"
                                                          {
      esql_set_cursor((yyvsp[-3].s));
      esql_parsed_result = esql_build_node(CB_SQL_FETCH_ONE);
    }
#line 1415 "esql-parser.c"
    break;

  case 30: /* preparesql: ESQL_PREPARE prepared_stname otherdb ESQL_FROM host_reference  */
#line 240 "esql-parser.y"
                                                                  {
      esql_add_host_var((yyvsp[0].s));
      esql_parsed_result = esql_build_node(CB_SQL_PREPARE);
    }
#line 1424 "esql-parser.c"
    break;

  case 31: /* executesql: ESQL_EXECUTE prepared_stname otherdb ESQL_USING host_references  */
#line 248 "esql-parser.y"
                                                                    {
      esql_parsed_result = esql_build_node(CB_SQL_EXECUTE_PREPARED);
    }
#line 1432 "esql-parser.c"
    break;

  case 32: /* executesql: ESQL_EXECUTE prepared_stname otherdb  */
#line 251 "esql-parser.y"
                                         {
      esql_parsed_result = esql_build_node(CB_SQL_EXECUTE_PREPARED);
    }
#line 1440 "esql-parser.c"
    break;

  case 33: /* selectintosql: ESQL_SELECT otherdb token_list ESQL_INTO res_host_references ESQL_SELECTFROM token_list  */
#line 259 "esql-parser.y"
                                 {
      esql_parsed_result = esql_build_node(
        (esql_host_count > 0 || esql_res_host_count > 0)
          ? CB_SQL_SELECT_INTO_ONE : CB_SQL_EXEC);
    }
#line 1450 "esql-parser.c"
    break;

  case 34: /* selectintosql: ESQL_SELECT otherdb token_list ESQL_INTO res_host_references  */
#line 264 "esql-parser.y"
                                                                 {
      esql_parsed_result = esql_build_node(
        (esql_host_count > 0 || esql_res_host_count > 0)
          ? CB_SQL_SELECT_INTO_ONE : CB_SQL_EXEC);
    }
#line 1460 "esql-parser.c"
    break;

  case 35: /* othersql: ESQL_OTHERFUNC otherdb token_list  */
#line 273 "esql-parser.y"
                                      {
      esql_parsed_result = esql_build_node(
        esql_host_count > 0 ? CB_SQL_EXEC_PARAMS : CB_SQL_EXEC);
    }
#line 1469 "esql-parser.c"
    break;

  case 36: /* otherdb: %empty  */
#line 281 "esql-parser.y"
                { }
#line 1475 "esql-parser.c"
    break;

  case 37: /* otherdb: ESQL_AT ESQL_HOSTTOKEN  */
#line 282 "esql-parser.y"
                           {
      esql_set_dbname((yyvsp[0].s));
    }
#line 1483 "esql-parser.c"
    break;

  case 38: /* host_references: host_reference  */
#line 288 "esql-parser.y"
                   {
      esql_add_host_var((yyvsp[0].s));
    }
#line 1491 "esql-parser.c"
    break;

  case 40: /* host_references: host_references host_reference  */
#line 292 "esql-parser.y"
                                   {
      esql_add_host_var((yyvsp[0].s));
    }
#line 1499 "esql-parser.c"
    break;

  case 41: /* res_host_references: host_reference  */
#line 298 "esql-parser.y"
                   {
      esql_add_res_host_var((yyvsp[0].s));
    }
#line 1507 "esql-parser.c"
    break;

  case 43: /* res_host_references: res_host_references host_reference  */
#line 302 "esql-parser.y"
                                       {
      esql_add_res_host_var((yyvsp[0].s));
    }
#line 1515 "esql-parser.c"
    break;

  case 44: /* host_reference: ESQL_HOSTTOKEN  */
#line 308 "esql-parser.y"
                   {
      (yyval.s) = (yyvsp[0].s);
      esql_pending_subs = NULL;
    }
#line 1524 "esql-parser.c"
    break;

  case 45: /* host_reference: ESQL_HOSTTOKEN ESQL_HOSTSUB_LPAREN subscript_list ESQL_HOSTSUB_RPAREN  */
#line 312 "esql-parser.y"
                                                                          {
      (yyval.s) = (yyvsp[-3].s);
      /* parser.y の subref と同じく cb_reference->subs は leaf→root の
         並びで構築する必要があるため、ここで反転する。 */
      esql_pending_subs = esql_subs_reverse((yyvsp[-1].t));
    }
#line 1535 "esql-parser.c"
    break;

  case 46: /* subscript_list: subscript  */
#line 321 "esql-parser.y"
              {
      (yyval.t) = esql_subs_list_init((yyvsp[0].t));
    }
#line 1543 "esql-parser.c"
    break;

  case 47: /* subscript_list: subscript_list ESQL_HOSTSUB_COMMA subscript  */
#line 324 "esql-parser.y"
                                                {
      (yyval.t) = esql_subs_list_add((yyvsp[-2].t), (yyvsp[0].t));
    }
#line 1551 "esql-parser.c"
    break;

  case 48: /* subscript: ESQL_HOSTSUB_NUMBER  */
#line 330 "esql-parser.y"
                        {
      (yyval.t) = esql_build_subs_number((yyvsp[0].s));
    }
#line 1559 "esql-parser.c"
    break;

  case 49: /* subscript: ESQL_HOSTSUB_IDENT  */
#line 333 "esql-parser.y"
                       {
      /* 添字の値となる変数も dotted 修飾を許す
         (ただし入れ子添字は不可)。esql_build_qualified_ref は
         dotted でなければ単純な cb_build_reference にフォールバックする。 */
      (yyval.t) = esql_build_qualified_ref((yyvsp[0].s));
    }
#line 1570 "esql-parser.c"
    break;

  case 50: /* prepared_stname: ESQL_TOKEN  */
#line 342 "esql-parser.y"
               {
      esql_set_prepare((yyvsp[0].s));
      (yyval.s) = (yyvsp[0].s);
    }
#line 1579 "esql-parser.c"
    break;

  case 53: /* token_list: token_list host_reference  */
#line 351 "esql-parser.y"
                              {
      esql_add_host_var((yyvsp[0].s));
    }
#line 1587 "esql-parser.c"
    break;

  case 54: /* token_list: token_list ESQL_WHERECURRENTOF ESQL_CURNAME  */
#line 354 "esql-parser.y"
                                                {
      esql_set_cursor((yyvsp[0].s));
    }
#line 1595 "esql-parser.c"
    break;

  case 57: /* expr: ESQL_TOKEN  */
#line 365 "esql-parser.y"
               { (yyval.s) = (yyvsp[0].s); }
#line 1601 "esql-parser.c"
    break;

  case 58: /* expr: ESQL_SELECT  */
#line 366 "esql-parser.y"
                { (yyval.s) = (yyvsp[0].s); }
#line 1607 "esql-parser.c"
    break;

  case 59: /* expr: ESQL_FOR  */
#line 367 "esql-parser.y"
             { (yyval.s) = (yyvsp[0].s); }
#line 1613 "esql-parser.c"
    break;

  case 60: /* expr: ESQL_SELECTFROM  */
#line 368 "esql-parser.y"
                    { (yyval.s) = (yyvsp[0].s); }
#line 1619 "esql-parser.c"
    break;

  case 61: /* expr: ESQL_INTO  */
#line 369 "esql-parser.y"
              { (yyval.s) = (yyvsp[0].s); }
#line 1625 "esql-parser.c"
    break;


#line 1629 "esql-parser.c"

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
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
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

#line 372 "esql-parser.y"


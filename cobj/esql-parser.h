/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_ESQL_ESQL_PARSER_H_INCLUDED
# define YY_ESQL_ESQL_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef ESQL_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define ESQL_DEBUG 1
#  else
#   define ESQL_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define ESQL_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined ESQL_DEBUG */
#if ESQL_DEBUG
extern int esql_debug;
#endif

/* Token kinds.  */
#ifndef ESQL_TOKENTYPE
# define ESQL_TOKENTYPE
  enum esql_tokentype
  {
    ESQL_EMPTY = -2,
    ESQL_EOF = 0,                  /* "end of file"  */
    ESQL_error = 256,              /* error  */
    ESQL_UNDEF = 257,              /* "invalid token"  */
    ESQL_SELECT = 258,             /* ESQL_SELECT  */
    ESQL_INSERT = 259,             /* ESQL_INSERT  */
    ESQL_UPDATE = 260,             /* ESQL_UPDATE  */
    ESQL_DELETE = 261,             /* ESQL_DELETE  */
    ESQL_DISCONNECT = 262,         /* ESQL_DISCONNECT  */
    ESQL_TOKEN = 263,              /* ESQL_TOKEN  */
    ESQL_HOSTTOKEN = 264,          /* ESQL_HOSTTOKEN  */
    ESQL_CURNAME = 265,            /* ESQL_CURNAME  */
    ESQL_OTHERFUNC = 266,          /* ESQL_OTHERFUNC  */
    ESQL_INTO = 267,               /* ESQL_INTO  */
    ESQL_SELECTFROM = 268,         /* ESQL_SELECTFROM  */
    ESQL_FOR = 269,                /* ESQL_FOR  */
    ESQL_CONNECT = 270,            /* ESQL_CONNECT  */
    ESQL_DECLARE = 271,            /* ESQL_DECLARE  */
    ESQL_CURSOR = 272,             /* ESQL_CURSOR  */
    ESQL_OPEN = 273,               /* ESQL_OPEN  */
    ESQL_CLOSE = 274,              /* ESQL_CLOSE  */
    ESQL_FETCH = 275,              /* ESQL_FETCH  */
    ESQL_PREPARE = 276,            /* ESQL_PREPARE  */
    ESQL_EXECUTE = 277,            /* ESQL_EXECUTE  */
    ESQL_FROM = 278,               /* ESQL_FROM  */
    ESQL_COMMIT_WORK = 279,        /* ESQL_COMMIT_WORK  */
    ESQL_ROLLBACK_WORK = 280,      /* ESQL_ROLLBACK_WORK  */
    ESQL_IDENTIFIED_BY = 281,      /* ESQL_IDENTIFIED_BY  */
    ESQL_USING = 282,              /* ESQL_USING  */
    ESQL_AT = 283,                 /* ESQL_AT  */
    ESQL_BEGIN_DECLARE = 284,      /* ESQL_BEGIN_DECLARE  */
    ESQL_END_DECLARE = 285,        /* ESQL_END_DECLARE  */
    ESQL_INCLUDE_SQLCA = 286,      /* ESQL_INCLUDE_SQLCA  */
    ESQL_INCLUDE = 287,            /* ESQL_INCLUDE  */
    ESQL_WHERECURRENTOF = 288      /* ESQL_WHERECURRENTOF  */
  };
  typedef enum esql_tokentype esql_token_kind_t;
#endif

/* Value type.  */
#if ! defined ESQL_STYPE && ! defined ESQL_STYPE_IS_DECLARED
union ESQL_STYPE
{
#line 85 "esql-parser.y"

  char *s;

#line 109 "esql-parser.h"

};
typedef union ESQL_STYPE ESQL_STYPE;
# define ESQL_STYPE_IS_TRIVIAL 1
# define ESQL_STYPE_IS_DECLARED 1
#endif


extern ESQL_STYPE esql_lval;


int esql_parse (void);


#endif /* !YY_ESQL_ESQL_PARSER_H_INCLUDED  */

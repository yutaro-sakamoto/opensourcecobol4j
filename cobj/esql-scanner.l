/*
 * ESQL lexer: tokenizes SQL text extracted from EXEC SQL...END-EXEC blocks.
 * Used by esql-parser.y via re-entrant scanner API.
 */

%option noyywrap
%option never-interactive
%option caseless
%option prefix="esql_"
%option stack
%option noyy_top_state

%{
#include <string.h>
#include "esql-parser.h"

#define YY_NO_INPUT
#define YY_NO_UNPUT

/* SQL body buffer (accumulated SQL text with host vars replaced by ?) */
static char esql_sqlbody[8192];
static int esql_flag_insqlstring;
static int esql_flag_selectcommand;
static int esql_flag_select_into;
static int esql_flag_declare_cursor;
static char esql_commandname[256];

static void esql_sqlbody_append(const char *s) {
  strncat(esql_sqlbody, s, sizeof(esql_sqlbody) - strlen(esql_sqlbody) - 1);
}

static int esql_in_quote = 0;

static void esql_reset_flags(void) {
  memset(esql_sqlbody, 0, sizeof(esql_sqlbody));
  memset(esql_commandname, 0, sizeof(esql_commandname));
  esql_flag_insqlstring = 0;
  esql_flag_selectcommand = 0;
  esql_flag_select_into = 0;
  esql_flag_declare_cursor = 0;
  esql_in_quote = 0;
}

const char *esql_get_sqlbody(void);
void esql_scanner_reset(void);

const char *esql_get_sqlbody(void) {
  return esql_sqlbody;
}

%}

%x ESQL_STATE ESQL_DBNAME_STATE WHERE_CURRENT_OF

ZENSPC [\x81][\x40]
JPNWORD [\xA0-\xDF]|([\x81-\x9F\xE0-\xFC][\x40-\x7E\x80-\xFC])
MBWORD ([\xC0-\xDF][\x80-\xBF])|([\xE0-\xEF][\x80-\xBF]{2})|([\xF0-\xF7][\x80-\xBF]{3})
HOSTWORD ":"([A-Za-z\-0-9_]|{JPNWORD}|{MBWORD})+
IDENT ([A-Za-z\+\-0-9_]|[(]|[)]|{JPNWORD}|{MBWORD})
STRVALUE ("N"?"\""[^\"]+"\""|"N"?"\'"[^\']+"\'")
SELF [,()\[\].;\:\+\-\*\/\%\^\<\>\=]
OP_CHARS [\~\!\@\#\^\&\|\`\?\+\-\*\/\%\<\>\=]
OPERATOR {OP_CHARS}+
COMMA ","

%%

 /* === Initial state: match first SQL keyword === */

[ \t\r\n]+ { /* skip leading whitespace */ }

"SELECT" {
  BEGIN ESQL_STATE;
  esql_flag_insqlstring = 1;
  esql_flag_selectcommand = 1;
  strncpy(esql_commandname, yytext, sizeof(esql_commandname) - 1);
  esql_sqlbody_append(yytext);
  esql_lval.s = strdup(yytext);
  return ESQL_SELECT;
}

"INSERT" {
  BEGIN ESQL_STATE;
  esql_flag_insqlstring = 1;
  strncpy(esql_commandname, yytext, sizeof(esql_commandname) - 1);
  esql_sqlbody_append(yytext);
  esql_lval.s = strdup(yytext);
  return ESQL_INSERT;
}

"DELETE" {
  BEGIN ESQL_STATE;
  esql_flag_insqlstring = 1;
  strncpy(esql_commandname, yytext, sizeof(esql_commandname) - 1);
  esql_sqlbody_append(yytext);
  esql_lval.s = strdup(yytext);
  return ESQL_DELETE;
}

"UPDATE" {
  BEGIN ESQL_STATE;
  esql_flag_insqlstring = 1;
  strncpy(esql_commandname, yytext, sizeof(esql_commandname) - 1);
  esql_sqlbody_append(yytext);
  esql_lval.s = strdup(yytext);
  return ESQL_UPDATE;
}

"CONNECT" {
  BEGIN ESQL_STATE;
  strncpy(esql_commandname, "CONNECT", sizeof(esql_commandname) - 1);
  return ESQL_CONNECT;
}

"DISCONNECT" {
  BEGIN ESQL_STATE;
  esql_flag_insqlstring = 1;
  strncpy(esql_commandname, yytext, sizeof(esql_commandname) - 1);
  esql_sqlbody_append(yytext);
  esql_lval.s = strdup(yytext);
  return ESQL_DISCONNECT;
}

"DECLARE" {
  BEGIN ESQL_STATE;
  strncpy(esql_commandname, "DECLARE", sizeof(esql_commandname) - 1);
  return ESQL_DECLARE;
}

"OPEN" {
  BEGIN ESQL_STATE;
  strncpy(esql_commandname, "OPEN", sizeof(esql_commandname) - 1);
  esql_sqlbody_append(yytext);
  return ESQL_OPEN;
}

"CLOSE" {
  BEGIN ESQL_STATE;
  strncpy(esql_commandname, "CLOSE", sizeof(esql_commandname) - 1);
  esql_sqlbody_append(yytext);
  return ESQL_CLOSE;
}

"FETCH" {
  BEGIN ESQL_STATE;
  strncpy(esql_commandname, "FETCH", sizeof(esql_commandname) - 1);
  return ESQL_FETCH;
}

"PREPARE" {
  BEGIN ESQL_STATE;
  strncpy(esql_commandname, "PREPARE", sizeof(esql_commandname) - 1);
  esql_sqlbody_append(yytext);
  return ESQL_PREPARE;
}

"EXECUTE" {
  BEGIN ESQL_STATE;
  strncpy(esql_commandname, "EXECUTE", sizeof(esql_commandname) - 1);
  esql_sqlbody_append(yytext);
  return ESQL_EXECUTE;
}

"COMMIT"(({ZENSPC}|[ ])+"WORK"(({ZENSPC}|[ ])+("WITH"({ZENSPC}|[ ])+)?"RELEASE")?)? {
  BEGIN ESQL_STATE;
  return ESQL_COMMIT_WORK;
}

"ROLLBACK"(({ZENSPC}|[ ])+"WORK"(({ZENSPC}|[ ])+("WITH"({ZENSPC}|[ ])+)?"RELEASE")?)? {
  BEGIN ESQL_STATE;
  return ESQL_ROLLBACK_WORK;
}

"BEGIN"[ \t]+"DECLARE" {
  BEGIN ESQL_STATE;
  return ESQL_BEGIN_DECLARE;
}

"END"[ \t]+"DECLARE" {
  BEGIN ESQL_STATE;
  return ESQL_END_DECLARE;
}

"INCLUDE"[ \t]+"SQLCA" {
  return ESQL_INCLUDE_SQLCA;
}

"INCLUDE"[ \t]+ {
  return ESQL_INCLUDE;
}

"AT" {
  esql_sqlbody_append(yytext);
  yy_push_state(ESQL_DBNAME_STATE);
  return ESQL_AT;
}

{IDENT}+ {
  BEGIN ESQL_STATE;
  esql_flag_insqlstring = 1;
  strncpy(esql_commandname, yytext, sizeof(esql_commandname) - 1);
  esql_sqlbody_append(yytext);
  esql_lval.s = strdup(yytext);
  return ESQL_OTHERFUNC;
}

 /* === ESQL_DBNAME_STATE: after AT keyword === */
<ESQL_DBNAME_STATE>{
  [ \t]+ { }
  {HOSTWORD} {
    esql_lval.s = strdup(yytext + 1);
    yy_pop_state();
    return ESQL_HOSTTOKEN;
  }
}

 /* === ESQL_STATE: inside SQL statement === */
<ESQL_STATE>{
  {COMMA} {
    esql_sqlbody_append(yytext);
    esql_lval.s = strdup(yytext);
    return ESQL_TOKEN;
  }

  ({ZENSPC}|[ \t\r\n])+ {
    if (esql_in_quote) {
      esql_sqlbody_append(yytext);
    } else {
      esql_sqlbody_append(" ");
    }
  }

  "AT" {
    esql_sqlbody_append(yytext);
    if (esql_flag_insqlstring) {
      esql_lval.s = strdup(yytext);
      return ESQL_TOKEN;
    }
    yy_push_state(ESQL_DBNAME_STATE);
    return ESQL_AT;
  }

  "SELECT" {
    esql_sqlbody_append(yytext);
    if (esql_flag_insqlstring) {
      esql_lval.s = strdup(yytext);
      return ESQL_TOKEN;
    }
    esql_flag_insqlstring = 1;
    esql_flag_selectcommand = 1;
    strncpy(esql_commandname, yytext, sizeof(esql_commandname) - 1);
    esql_lval.s = strdup(yytext);
    return ESQL_SELECT;
  }

  "FROM" {
    esql_sqlbody_append(yytext);
    if (esql_flag_insqlstring) {
      esql_lval.s = strdup(yytext);
      if (esql_flag_selectcommand) {
        esql_flag_selectcommand = 0;
        return ESQL_SELECTFROM;
      }
      return ESQL_TOKEN;
    }
    return ESQL_FROM;
  }

  "CURSOR" {
    if (esql_flag_insqlstring) {
      esql_lval.s = strdup(yytext);
      return ESQL_TOKEN;
    }
    if (strcmp(esql_commandname, "DECLARE") == 0) {
      esql_flag_declare_cursor = 1;
    }
    return ESQL_CURSOR;
  }

  "FOR" {
    if (esql_flag_insqlstring) {
      esql_lval.s = strdup(yytext);
      return ESQL_TOKEN;
    }
    if (esql_flag_declare_cursor) {
      esql_flag_declare_cursor = 0;
    } else {
      esql_sqlbody_append(yytext);
    }
    return ESQL_FOR;
  }

  "IDENTIFIED"({ZENSPC}|[ ])+"BY" {
    if (esql_flag_insqlstring) {
      esql_lval.s = strdup(yytext);
      return ESQL_TOKEN;
    }
    return ESQL_IDENTIFIED_BY;
  }

  "USING" {
    esql_sqlbody_append(yytext);
    if (esql_flag_insqlstring) {
      esql_lval.s = strdup(yytext);
      return ESQL_TOKEN;
    }
    return ESQL_USING;
  }

  ({ZENSPC}|[ \t])+"INTO"({ZENSPC}|[ \t])+ {
    esql_flag_select_into = 0;
    if (strcmp(esql_commandname, "SELECT") != 0) {
      esql_sqlbody_append(yytext);
    } else {
      esql_flag_select_into = 1;
    }
    if (esql_flag_insqlstring && !esql_flag_selectcommand) {
      esql_lval.s = strdup(yytext);
      return ESQL_TOKEN;
    }
    esql_lval.s = strdup(yytext);
    return ESQL_INTO;
  }

  "WHERE"[ \r\n]+"CURRENT"[ \r\n]+"OF" {
    esql_sqlbody_append(yytext);
    yy_push_state(WHERE_CURRENT_OF);
    return ESQL_WHERECURRENTOF;
  }

  {OPERATOR} {
    esql_sqlbody_append(yytext);
    esql_lval.s = strdup(yytext);
    return ESQL_TOKEN;
  }

  {HOSTWORD} {
    if (!esql_flag_select_into) {
      esql_sqlbody_append(" ?");
    }
    esql_lval.s = strdup(yytext + 1); /* strip ':' */
    return ESQL_HOSTTOKEN;
  }

  "'" {
    esql_sqlbody_append(yytext);
    if (esql_in_quote && strlen(esql_sqlbody) >= 2) {
      /* Check for escaped quote ('') - previous char was also quote */
      size_t len = strlen(esql_sqlbody);
      if (len >= 2 && esql_sqlbody[len - 2] == '\'') {
        /* Escaped quote: stay in quote */
      } else {
        esql_in_quote = 0;
      }
    } else {
      esql_in_quote = 1;
    }
    esql_lval.s = strdup(yytext);
    return ESQL_TOKEN;
  }

  {IDENT}+("."{IDENT}+)? {
    if (strcmp(esql_commandname, "DECLARE") != 0) {
      esql_sqlbody_append(yytext);
    }
    esql_lval.s = strdup(yytext);
    return ESQL_TOKEN;
  }

  {SELF} {
    esql_sqlbody_append(yytext);
    esql_lval.s = strdup(yytext);
    return ESQL_TOKEN;
  }
}

 /* === WHERE_CURRENT_OF state === */
<WHERE_CURRENT_OF>{
  [ \t\r\n]+ { }
  {IDENT}+ {
    esql_lval.s = strdup(yytext);
    yy_pop_state();
    return ESQL_CURNAME;
  }
}

 /* Fallback: skip anything unrecognized */
. { }

%%

void esql_scanner_reset(void) {
  esql_reset_flags();
  BEGIN(INITIAL);
}

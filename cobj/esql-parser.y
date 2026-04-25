/*
 * ESQL parser: parses SQL text extracted from EXEC SQL...END-EXEC blocks.
 * Builds cb_exec_sql AST nodes for code generation.
 */

%define api.prefix {esql_}

%{
#include <stdlib.h>
#include <string.h>

/* Forward declarations from tree.h (avoid including tree.h directly
   because it defines YYSTYPE as cb_tree, conflicting with ESQL_STYPE) */
struct cb_tree_common;
typedef struct cb_tree_common *cb_tree;
extern cb_tree cb_error_node;
extern cb_tree cb_build_reference(const char *name);

struct cb_sql_host_var;
extern struct cb_sql_host_var *cb_build_sql_host_var(char *name, cb_tree ref);
extern struct cb_sql_host_var *
cb_sql_host_var_list_add(struct cb_sql_host_var *list,
                         struct cb_sql_host_var *item);

enum cb_sql_command {
  CB_SQL_CONNECT,
  CB_SQL_CONNECT_INFORMAL,
  CB_SQL_CONNECT_SHORT,
  CB_SQL_DISCONNECT,
  CB_SQL_EXEC,
  CB_SQL_EXEC_PARAMS,
  CB_SQL_SELECT_INTO_ONE,
  CB_SQL_SELECT_INTO_OCCURS,
  CB_SQL_DECLARE_CURSOR,
  CB_SQL_DECLARE_CURSOR_PARAMS,
  CB_SQL_OPEN_CURSOR,
  CB_SQL_OPEN_CURSOR_PARAMS,
  CB_SQL_CLOSE_CURSOR,
  CB_SQL_FETCH_ONE,
  CB_SQL_FETCH_OCCURS,
  CB_SQL_PREPARE,
  CB_SQL_EXECUTE_PREPARED,
  CB_SQL_COMMIT,
  CB_SQL_ROLLBACK,
};

extern void cb_error(const char *msg, ...);

/* Forward declarations */
extern const char *esql_get_sqlbody(void);
extern cb_tree esql_build_and_resolve(enum cb_sql_command command,
                                      char *sql_text, char *cursor_name,
                                      char *prepare_name, char *db_name,
                                      struct cb_sql_host_var *host_list,
                                      int host_count,
                                      struct cb_sql_host_var *res_host_list,
                                      int res_host_count,
                                      int conn_use_other_db);

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

%}

%union {
  char *s;
}

%token<s> ESQL_SELECT ESQL_INSERT ESQL_UPDATE ESQL_DELETE ESQL_DISCONNECT
%token<s> ESQL_TOKEN ESQL_HOSTTOKEN ESQL_CURNAME ESQL_OTHERFUNC
%token<s> ESQL_INTO ESQL_SELECTFROM
%token<s> ESQL_FOR
%token ESQL_CONNECT ESQL_DECLARE ESQL_CURSOR
%token ESQL_OPEN ESQL_CLOSE ESQL_FETCH
%token ESQL_PREPARE ESQL_EXECUTE ESQL_FROM
%token ESQL_COMMIT_WORK ESQL_ROLLBACK_WORK
%token ESQL_IDENTIFIED_BY ESQL_USING ESQL_AT
%token ESQL_BEGIN_DECLARE ESQL_END_DECLARE
%token ESQL_INCLUDE_SQLCA ESQL_INCLUDE
%token ESQL_WHERECURRENTOF

%type<s> host_reference expr prepared_stname

%%

input:
    esql_statement
  ;

esql_statement:
    connectsql
  | disconnectsql
  | commitsql
  | rollbacksql
  | declaresql
  | opensql
  | closesql
  | fetchsql
  | preparesql
  | executesql
  | selectintosql
  | insertsql
  | updatesql
  | deletesql
  | othersql
  | declaresection
  | includesql
  ;

/* --- CONNECT --- */
connectsql:
    ESQL_CONNECT otherdb {
      /* CONNECT (short form) */
      esql_parsed_result =esql_build_node(CB_SQL_CONNECT_SHORT);
    }
  | connect_user identified using otherdb {
      esql_parsed_result =esql_build_node(CB_SQL_CONNECT);
    }
  | connect_user otherdb {
      esql_parsed_result =esql_build_node(CB_SQL_CONNECT_INFORMAL);
    }
  ;

connect_user:
    ESQL_CONNECT host_reference {
      esql_add_host_var($2);
    }
  ;

identified:
    ESQL_IDENTIFIED_BY host_reference {
      esql_add_host_var($2);
    }
  ;

using:
    ESQL_USING host_reference {
      esql_add_host_var($2);
    }
  ;

/* --- DISCONNECT --- */
disconnectsql:
    ESQL_DISCONNECT otherdb token_list_opt {
      esql_parsed_result = esql_build_node(CB_SQL_DISCONNECT);
    }
  ;

/* --- COMMIT / ROLLBACK --- */
commitsql:
    ESQL_COMMIT_WORK otherdb {
      esql_parsed_result =esql_build_node(CB_SQL_COMMIT);
    }
  ;

rollbacksql:
    ESQL_ROLLBACK_WORK otherdb {
      esql_parsed_result =esql_build_node(CB_SQL_ROLLBACK);
    }
  ;

/* --- DECLARE CURSOR --- */
declaresql:
    ESQL_DECLARE expr ESQL_CURSOR ESQL_FOR
      ESQL_SELECT token_list {
      esql_set_cursor($2);
      esql_parsed_result =esql_build_node(
        esql_host_count > 0 ? CB_SQL_DECLARE_CURSOR_PARAMS
                            : CB_SQL_DECLARE_CURSOR);
    }
  | ESQL_DECLARE expr ESQL_CURSOR ESQL_FOR prepared_stname {
      esql_set_cursor($2);
      esql_parsed_result =esql_build_node(CB_SQL_DECLARE_CURSOR);
    }
  ;

/* --- OPEN CURSOR --- */
opensql:
    ESQL_OPEN expr otherdb {
      esql_set_cursor($2);
      esql_parsed_result =esql_build_node(CB_SQL_OPEN_CURSOR);
    }
  | ESQL_OPEN expr otherdb ESQL_USING host_references {
      esql_set_cursor($2);
      esql_parsed_result =esql_build_node(CB_SQL_OPEN_CURSOR_PARAMS);
    }
  ;

/* --- CLOSE CURSOR --- */
closesql:
    ESQL_CLOSE expr otherdb {
      esql_set_cursor($2);
      esql_parsed_result =esql_build_node(CB_SQL_CLOSE_CURSOR);
    }
  ;

/* --- FETCH --- */
fetchsql:
    ESQL_FETCH expr otherdb ESQL_INTO res_host_references {
      esql_set_cursor($2);
      esql_parsed_result =esql_build_node(CB_SQL_FETCH_ONE);
    }
  ;

/* --- PREPARE --- */
preparesql:
    ESQL_PREPARE prepared_stname otherdb ESQL_FROM host_reference {
      esql_add_host_var($5);
      esql_parsed_result =esql_build_node(CB_SQL_PREPARE);
    }
  ;

/* --- EXECUTE --- */
executesql:
    ESQL_EXECUTE prepared_stname otherdb ESQL_USING host_references {
      esql_parsed_result =esql_build_node(CB_SQL_EXECUTE_PREPARED);
    }
  | ESQL_EXECUTE prepared_stname otherdb {
      esql_parsed_result =esql_build_node(CB_SQL_EXECUTE_PREPARED);
    }
  ;

/* --- SELECT INTO --- */
selectintosql:
    ESQL_SELECT otherdb token_list ESQL_INTO res_host_references
      ESQL_SELECTFROM token_list {
      esql_parsed_result =esql_build_node(
        (esql_host_count > 0 || esql_res_host_count > 0)
          ? CB_SQL_SELECT_INTO_ONE : CB_SQL_EXEC);
    }
  | ESQL_SELECT otherdb token_list ESQL_INTO res_host_references {
      esql_parsed_result =esql_build_node(
        (esql_host_count > 0 || esql_res_host_count > 0)
          ? CB_SQL_SELECT_INTO_ONE : CB_SQL_EXEC);
    }
  ;

/* --- INSERT --- */
insertsql:
    ESQL_INSERT otherdb insert_rest token_list {
      esql_parsed_result =esql_build_node(
        esql_host_count > 0 ? CB_SQL_EXEC_PARAMS : CB_SQL_EXEC);
    }
  ;

insert_rest:
    ESQL_INTO { }
  | /* empty */ { }
  ;

/* --- UPDATE --- */
updatesql:
    ESQL_UPDATE otherdb token_list {
      esql_parsed_result =esql_build_node(
        esql_host_count > 0 ? CB_SQL_EXEC_PARAMS : CB_SQL_EXEC);
    }
  ;

/* --- DELETE --- */
deletesql:
    ESQL_DELETE otherdb token_list {
      esql_parsed_result =esql_build_node(
        esql_host_count > 0 ? CB_SQL_EXEC_PARAMS : CB_SQL_EXEC);
    }
  ;

/* --- Other SQL (DROP, CREATE, ALTER, etc.) --- */
othersql:
    ESQL_OTHERFUNC otherdb token_list {
      esql_parsed_result =esql_build_node(
        esql_host_count > 0 ? CB_SQL_EXEC_PARAMS : CB_SQL_EXEC);
    }
  ;

/* --- DECLARE SECTION / INCLUDE (no-ops) --- */
declaresection:
    ESQL_BEGIN_DECLARE token_list_opt {
      esql_parsed_result =cb_error_node;
    }
  | ESQL_END_DECLARE token_list_opt {
      esql_parsed_result =cb_error_node;
    }
  ;

includesql:
    ESQL_INCLUDE_SQLCA {
      esql_parsed_result =cb_error_node;
    }
  | ESQL_INCLUDE ESQL_TOKEN {
      /* INCLUDE filename - handled at preprocessor level */
      esql_parsed_result =cb_error_node;
    }
  ;

/* --- Common rules --- */
otherdb:
    /* empty */ { }
  | ESQL_AT ESQL_HOSTTOKEN {
      esql_set_dbname($2);
    }
  ;

host_references:
    host_reference {
      esql_add_host_var($1);
    }
  | host_references ESQL_TOKEN
  | host_references host_reference {
      esql_add_host_var($2);
    }
  ;

res_host_references:
    host_reference {
      esql_add_res_host_var($1);
    }
  | res_host_references ESQL_TOKEN
  | res_host_references host_reference {
      esql_add_res_host_var($2);
    }
  ;

host_reference:
    ESQL_HOSTTOKEN { $$ = $1; }
  ;

prepared_stname:
    ESQL_TOKEN {
      esql_set_prepare($1);
      $$ = $1;
    }
  ;

token_list:
    expr
  | token_list expr
  | token_list host_reference {
      esql_add_host_var($2);
    }
  | token_list ESQL_WHERECURRENTOF ESQL_CURNAME {
      esql_set_cursor($3);
    }
  ;

token_list_opt:
    /* empty */
  | token_list
  ;

expr:
    ESQL_TOKEN { $$ = $1; }
  | ESQL_SELECT { $$ = $1; }
  | ESQL_FOR { $$ = $1; }
  | ESQL_UPDATE { $$ = $1; }
  | ESQL_SELECTFROM { $$ = $1; }
  | ESQL_INTO { $$ = $1; }
  | ESQL_DELETE { $$ = $1; }
  | ESQL_INSERT { $$ = $1; }
  ;

%%

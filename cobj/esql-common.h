/*
 * Shared declarations for the ESQL parser/scanner.
 * Avoids including tree.h (which defines YYSTYPE as cb_tree,
 * conflicting with the ESQL parser's own YYSTYPE union).
 */
#ifndef ESQL_COMMON_H
#define ESQL_COMMON_H

/* Forward-declare tree types */
struct cb_tree_common;
typedef struct cb_tree_common *cb_tree;
extern cb_tree cb_error_node;
extern cb_tree cb_build_reference(const char *name);

/* Helpers used from esql-parser.y to build :VAR(添字) subs.
   esql-parser.y は tree.h を include しない (YYSTYPE が衝突する) ため、
   subs リスト操作と数値リテラル生成は esql.c のラッパ経由で行う。 */
extern cb_tree esql_subs_list_init(cb_tree x);
extern cb_tree esql_subs_list_add(cb_tree l, cb_tree x);
extern cb_tree esql_subs_reverse(cb_tree l);
extern cb_tree esql_build_subs_number(const char *text);
extern void esql_set_ref_subs(cb_tree ref, cb_tree subs);

/* Host variable list */
struct cb_sql_host_var;
extern struct cb_sql_host_var *cb_build_sql_host_var(char *name, cb_tree ref);
extern struct cb_sql_host_var *
cb_sql_host_var_list_add(struct cb_sql_host_var *list,
                         struct cb_sql_host_var *item);

#ifndef CB_SQL_COMMAND_DEFINED
#define CB_SQL_COMMAND_DEFINED
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
#endif

extern void cb_error(const char *msg, ...);

/* ESQL build/resolve function */
extern cb_tree esql_build_and_resolve(enum cb_sql_command command,
                                      char *sql_text, char *cursor_name,
                                      char *prepare_name, char *db_name,
                                      struct cb_sql_host_var *host_list,
                                      int host_count,
                                      struct cb_sql_host_var *res_host_list,
                                      int res_host_count,
                                      int conn_use_other_db);

/* Scanner API */
extern const char *esql_get_sqlbody(void);
extern void esql_scanner_reset(void);
extern void esql_parser_init(void);

#endif /* ESQL_COMMON_H */

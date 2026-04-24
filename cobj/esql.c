/*
 * Copyright (C) 2024 TOKYO SYSTEM HOUSE Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, 51 Franklin Street, Fifth Floor
 * Boston, MA 02110-1301 USA
 */

#include "config.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cobj.h"
#include "tree.h"

/* Helper: case-insensitive prefix match, returns pointer past match or NULL */
static const char *match_keyword(const char *str, const char *keyword) {
  size_t len = strlen(keyword);
  if (strncasecmp(str, keyword, len) == 0) {
    char c = str[len];
    if (c == '\0' || c == ' ' || c == '\t' || c == '\n') {
      return str + len;
    }
  }
  return NULL;
}

/* Helper: skip whitespace */
static const char *skip_ws(const char *p) {
  while (*p == ' ' || *p == '\t' || *p == '\n') {
    p++;
  }
  return p;
}

/* Helper: extract identifier (alphanumeric, hyphen, underscore) */
static const char *extract_ident(const char *p, char *buf, size_t bufsize) {
  size_t i = 0;
  while (i < bufsize - 1 &&
         (isalnum((unsigned char)*p) || *p == '-' || *p == '_')) {
    buf[i++] = *p++;
  }
  buf[i] = '\0';
  return p;
}

/*
 * Resolve host variable type from cb_field properties.
 * Maps COBOL field attributes to HVARTYPE_* constants.
 */
static void resolve_host_var_type(struct cb_sql_host_var *hv) {
  cb_tree resolved;
  struct cb_field *f;

  if (!hv->ref) {
    return;
  }

  resolved = cb_ref(hv->ref);
  if (!resolved || resolved == cb_error_node || !CB_FIELD_P(resolved)) {
    return;
  }

  f = CB_FIELD(resolved);
  hv->length = f->size;
  hv->scale = f->pic ? f->pic->scale : 0;

  /* VARYING field: detect from flag */
  if (f->flag_varying) {
    struct cb_field *arr = f->children ? f->children->sister : NULL;
    if (arr && arr->pic && arr->pic->category == CB_CATEGORY_NATIONAL) {
      hv->hvar_type = HVARTYPE_JAPANESE_VARYING;
    } else {
      hv->hvar_type = HVARTYPE_ALPHANUMERIC_VARYING;
    }
    return;
  }

  /* Determine HVARTYPE based on usage and picture */
  switch (f->usage) {
  case CB_USAGE_PACKED:
    if (f->pic && f->pic->have_sign) {
      hv->hvar_type = HVARTYPE_SIGNED_PACKED;
    } else {
      hv->hvar_type = HVARTYPE_UNSIGNED_PACKED;
    }
    break;

  case CB_USAGE_BINARY:
  case CB_USAGE_COMP_5:
  case CB_USAGE_COMP_X:
    if (f->pic && f->pic->have_sign) {
      hv->hvar_type = HVARTYPE_SIGNED_BINARY_NATIVE;
    } else {
      hv->hvar_type = HVARTYPE_UNSIGNED_BINARY_NATIVE;
    }
    break;

  case CB_USAGE_FLOAT:
  case CB_USAGE_DOUBLE:
    hv->hvar_type = HVARTYPE_FLOAT;
    break;

  case CB_USAGE_NATIONAL:
    hv->hvar_type = HVARTYPE_NATIONAL;
    break;

  case CB_USAGE_DISPLAY:
  default:
    if (f->children) {
      /* Check for VARYING pattern: GROUP with LEN (COMP-5) + ARR (X/N) */
      struct cb_field *child1 = f->children;
      struct cb_field *child2 = child1 ? child1->sister : NULL;
      if (child1 && child2 && !child2->sister &&
          (child1->usage == CB_USAGE_BINARY ||
           child1->usage == CB_USAGE_COMP_5) &&
          child2->pic) {
        /* Mark as VARYING so CobolDataConverter can detect it */
        f->flag_varying = 1;
        if (child2->pic->category == CB_CATEGORY_ALPHANUMERIC ||
            child2->pic->category == CB_CATEGORY_ALPHABETIC) {
          hv->hvar_type = HVARTYPE_ALPHANUMERIC_VARYING;
        } else if (child2->pic->category == CB_CATEGORY_NATIONAL) {
          hv->hvar_type = HVARTYPE_JAPANESE_VARYING;
        } else {
          hv->hvar_type = HVARTYPE_GROUP;
        }
      } else {
        hv->hvar_type = HVARTYPE_GROUP;
      }
    } else if (f->pic) {
      if (f->pic->category == CB_CATEGORY_NUMERIC ||
          f->pic->category == CB_CATEGORY_NUMERIC_EDITED) {
        if (f->pic->have_sign) {
          if (f->flag_sign_leading) {
            hv->hvar_type = f->flag_sign_separate
                                ? HVARTYPE_SIGNED_LEADING_SEPARATE
                                : HVARTYPE_SIGNED_LEADING_COMBINED;
          } else {
            hv->hvar_type = f->flag_sign_separate
                                ? HVARTYPE_SIGNED_TRAILING_SEPARATE
                                : HVARTYPE_SIGNED_TRAILING_COMBINED;
          }
        } else {
          hv->hvar_type = HVARTYPE_UNSIGNED_NUMERIC;
        }
      } else {
        /* ALPHANUMERIC */
        hv->hvar_type = HVARTYPE_ALPHABETIC;
      }
    } else {
      hv->hvar_type = HVARTYPE_ALPHABETIC;
    }
    break;
  }
}

/*
 * Resolve all host variable types in a list.
 */
/*
 * Expand GROUP host variables into their children.
 * For SELECT INTO with OCCURS GROUP, we need individual child fields.
 * Returns a new list with GROUP items replaced by their children.
 */
static struct cb_sql_host_var *
expand_group_host_vars(struct cb_sql_host_var *list, int *count) {
  struct cb_sql_host_var *result = NULL;
  struct cb_sql_host_var *hv;
  int new_count = 0;

  for (hv = list; hv; hv = hv->next) {
    cb_tree resolved = hv->ref ? cb_ref(hv->ref) : NULL;
    if (resolved && resolved != cb_error_node && CB_FIELD_P(resolved)) {
      struct cb_field *f = CB_FIELD(resolved);
      if (f->children) {
        /* GROUP field: expand into children */
        struct cb_field *child;
        for (child = f->children; child; child = child->sister) {
          cb_tree child_ref = cb_build_field_reference(child, NULL);
          struct cb_sql_host_var *new_hv =
              cb_build_sql_host_var(strdup(child->name), child_ref);
          resolve_host_var_type(new_hv);
          result = cb_sql_host_var_list_add(result, new_hv);
          new_count++;
        }
        continue;
      }
    }
    /* Non-GROUP: keep as-is */
    resolve_host_var_type(hv);
    struct cb_sql_host_var *copy = cb_build_sql_host_var(hv->name, hv->ref);
    copy->hvar_type = hv->hvar_type;
    copy->length = hv->length;
    copy->scale = hv->scale;
    result = cb_sql_host_var_list_add(result, copy);
    new_count++;
  }

  *count = new_count;
  return result;
}

static void resolve_host_var_list(struct cb_sql_host_var *list) {
  struct cb_sql_host_var *hv;
  for (hv = list; hv; hv = hv->next) {
    resolve_host_var_type(hv);
  }
}

/*
 * Parse host variable references (:varname) in SQL text.
 * Replaces :varname with ? in the output SQL body.
 * Returns the host variable list and sets *host_count.
 */
static struct cb_sql_host_var *parse_host_vars(const char *sql_text,
                                               char *sql_body, size_t body_size,
                                               int *host_count) {
  struct cb_sql_host_var *list = NULL;
  const char *p = sql_text;
  size_t out = 0;
  int count = 0;
  int in_string = 0;

  while (*p && out < body_size - 2) {
    if (*p == '\'') {
      in_string = !in_string;
      sql_body[out++] = *p++;
      continue;
    }
    if (in_string) {
      sql_body[out++] = *p++;
      continue;
    }
    if (*p == ':' && (isalpha((unsigned char)p[1]) || p[1] == '_')) {
      /* Host variable reference */
      char varname[256];
      p++; /* skip ':' */
      p = extract_ident(p, varname, sizeof(varname));

      /* Replace with ? */
      sql_body[out++] = '?';
      count++;

      /* Build host var and add to list */
      cb_tree ref = cb_build_reference(varname);
      struct cb_sql_host_var *hv = cb_build_sql_host_var(strdup(varname), ref);
      list = cb_sql_host_var_list_add(list, hv);
    } else {
      sql_body[out++] = *p++;
    }
  }
  sql_body[out] = '\0';
  *host_count = count;
  return list;
}

static cb_tree
build_and_resolve_exec_sql(enum cb_sql_command command, char *sql_text,
                           char *cursor_name, char *prepare_name, char *db_name,
                           struct cb_sql_host_var *host_list, int host_count,
                           struct cb_sql_host_var *res_host_list,
                           int res_host_count, int conn_use_other_db) {
  resolve_host_var_list(host_list);
  resolve_host_var_list(host_list);
  int occurs_size = 0;
  int occurs_max = 0;
  /* Detect OCCURS for FETCH_ONE → promote to FETCH_OCCURS */
  if (command == CB_SQL_FETCH_ONE && res_host_list && res_host_list->ref) {
    cb_tree resolved = cb_ref(res_host_list->ref);
    if (resolved && resolved != cb_error_node && CB_FIELD_P(resolved)) {
      struct cb_field *f = CB_FIELD(resolved);
      if (f->flag_occurs && f->occurs_max > 1) {
        command = CB_SQL_FETCH_OCCURS;
      } else if (f->children) {
        struct cb_field *child;
        for (child = f->children; child; child = child->sister) {
          if (child->flag_occurs && child->occurs_max > 1) {
            command = CB_SQL_FETCH_OCCURS;
            break;
          }
        }
      }
    }
  }
  /* Expand GROUP result host vars into children for SELECT INTO / FETCH OCCURS
   */
  if (command == CB_SQL_SELECT_INTO_ONE ||
      command == CB_SQL_SELECT_INTO_OCCURS || command == CB_SQL_FETCH_OCCURS) {
    /* Detect OCCURS info from the original GROUP field before expansion */
    if (res_host_list && res_host_list->ref) {
      cb_tree resolved = cb_ref(res_host_list->ref);
      if (resolved && resolved != cb_error_node && CB_FIELD_P(resolved)) {
        struct cb_field *f = CB_FIELD(resolved);
        if (f->flag_occurs && f->occurs_max > 1) {
          occurs_size = f->size;
          occurs_max = f->occurs_max;
          if (command != CB_SQL_FETCH_OCCURS) {
            command = CB_SQL_SELECT_INTO_OCCURS;
          }
        } else if (f->children) {
          struct cb_field *child;
          for (child = f->children; child; child = child->sister) {
            if (child->flag_occurs && child->occurs_max > 1) {
              occurs_size = child->size;
              occurs_max = child->occurs_max;
              if (command != CB_SQL_FETCH_OCCURS) {
                command = CB_SQL_SELECT_INTO_OCCURS;
              }
              f = child;
              break;
            }
          }
        }
      }
    }
    int new_res_count = 0;
    res_host_list = expand_group_host_vars(res_host_list, &new_res_count);
    res_host_count = new_res_count;
  } else {
    resolve_host_var_list(res_host_list);
  }
  {
    cb_tree node = cb_build_exec_sql(
        command, sql_text, cursor_name, prepare_name, db_name, host_list,
        host_count, res_host_list, res_host_count, conn_use_other_db);
    CB_EXEC_SQL(node)->occurs_size = occurs_size;
    CB_EXEC_SQL(node)->occurs_max = occurs_max;
    return node;
  }
}

/*
 * Parse the SQL text collected between EXEC SQL and END-EXEC.
 * Returns a cb_exec_sql AST node.
 */
cb_tree cb_parse_exec_sql(const char *sql_text) {
  const char *p;
  const char *rest;
  char sql_body[8192];
  char db_name[256] = "";
  struct cb_sql_host_var *host_list = NULL;
  struct cb_sql_host_var *res_host_list = NULL;
  int host_count = 0;
  int res_host_count = 0;
  int conn_use_other_db = 0;
  enum cb_sql_command command;

  p = skip_ws(sql_text);

  /* Check for AT clause */
  rest = match_keyword(p, "AT");
  if (rest) {
    rest = skip_ws(rest);
    if (*rest == ':') {
      rest++;
      rest = extract_ident(rest, db_name, sizeof(db_name));
      conn_use_other_db = 1;
      p = skip_ws(rest);
    }
  }

  /* BEGIN DECLARE SECTION / END DECLARE SECTION - no-ops */
  rest = match_keyword(p, "BEGIN");
  if (rest) {
    return cb_error_node; /* silently skip */
  }
  rest = match_keyword(p, "END");
  if (rest) {
    rest = skip_ws(rest);
    if (match_keyword(rest, "DECLARE")) {
      return cb_error_node; /* silently skip */
    }
  }

  /* INCLUDE SQLCA - no-op (handled at preprocessor level) */
  rest = match_keyword(p, "INCLUDE");
  if (rest) {
    return cb_error_node; /* silently skip */
  }

  /* CONNECT */
  rest = match_keyword(p, "CONNECT");
  if (rest) {
    rest = skip_ws(rest);

    /* Check for CONNECT with no args (short form) */
    if (*rest == '\0') {
      command = CB_SQL_CONNECT_SHORT;
      return build_and_resolve_exec_sql(command, strdup(""), NULL, NULL,
                                        conn_use_other_db ? strdup(db_name)
                                                          : NULL,
                                        NULL, 0, NULL, 0, conn_use_other_db);
    }

    /* CONNECT :user IDENTIFIED BY :passwd USING :dbname */
    /* or CONNECT :user (informal) */
    host_list = NULL;
    host_count = 0;

    if (*rest == ':') {
      rest++;
      char varname[256];
      rest = extract_ident(rest, varname, sizeof(varname));
      cb_tree ref = cb_build_reference(varname);
      struct cb_sql_host_var *hv = cb_build_sql_host_var(strdup(varname), ref);
      host_list = cb_sql_host_var_list_add(host_list, hv);
      host_count++;

      rest = skip_ws(rest);
      const char *ident_rest = match_keyword(rest, "IDENTIFIED");
      if (ident_rest) {
        ident_rest = skip_ws(ident_rest);
        ident_rest = match_keyword(ident_rest, "BY");
        if (ident_rest) {
          ident_rest = skip_ws(ident_rest);
          if (*ident_rest == ':') {
            ident_rest++;
            ident_rest = extract_ident(ident_rest, varname, sizeof(varname));
            ref = cb_build_reference(varname);
            hv = cb_build_sql_host_var(strdup(varname), ref);
            host_list = cb_sql_host_var_list_add(host_list, hv);
            host_count++;
          }
        }
        rest = skip_ws(ident_rest);
        const char *using_rest = match_keyword(rest, "USING");
        if (using_rest) {
          using_rest = skip_ws(using_rest);
          if (*using_rest == ':') {
            using_rest++;
            using_rest = extract_ident(using_rest, varname, sizeof(varname));
            ref = cb_build_reference(varname);
            hv = cb_build_sql_host_var(strdup(varname), ref);
            host_list = cb_sql_host_var_list_add(host_list, hv);
            host_count++;
          }
        }
        command = CB_SQL_CONNECT;
      } else {
        /* Informal connect - just :user */
        command = CB_SQL_CONNECT_INFORMAL;
      }
    } else {
      command = CB_SQL_CONNECT_SHORT;
    }

    return build_and_resolve_exec_sql(
        command, strdup(""), NULL, NULL,
        conn_use_other_db ? strdup(db_name) : NULL, host_list, host_count, NULL,
        0, conn_use_other_db);
  }

  /* DISCONNECT */
  rest = match_keyword(p, "DISCONNECT");
  if (rest) {
    command = CB_SQL_DISCONNECT;
    return build_and_resolve_exec_sql(command, strdup(""), NULL, NULL,
                                      conn_use_other_db ? strdup(db_name)
                                                        : NULL,
                                      NULL, 0, NULL, 0, conn_use_other_db);
  }

  /* COMMIT */
  rest = match_keyword(p, "COMMIT");
  if (rest) {
    command = CB_SQL_COMMIT;
    return build_and_resolve_exec_sql(command, strdup(""), NULL, NULL,
                                      conn_use_other_db ? strdup(db_name)
                                                        : NULL,
                                      NULL, 0, NULL, 0, conn_use_other_db);
  }

  /* ROLLBACK */
  rest = match_keyword(p, "ROLLBACK");
  if (rest) {
    command = CB_SQL_ROLLBACK;
    return build_and_resolve_exec_sql(command, strdup(""), NULL, NULL,
                                      conn_use_other_db ? strdup(db_name)
                                                        : NULL,
                                      NULL, 0, NULL, 0, conn_use_other_db);
  }

  /* DECLARE cursor CURSOR FOR ... */
  rest = match_keyword(p, "DECLARE");
  if (rest) {
    rest = skip_ws(rest);
    char cname[256];
    rest = extract_ident(rest, cname, sizeof(cname));
    rest = skip_ws(rest);
    const char *cur_rest = match_keyword(rest, "CURSOR");
    if (cur_rest) {
      cur_rest = skip_ws(cur_rest);
      cur_rest = match_keyword(cur_rest, "FOR");
      if (cur_rest) {
        cur_rest = skip_ws(cur_rest);

        /* Check if it's a prepared statement name or a SELECT */
        const char *sel_rest = match_keyword(cur_rest, "SELECT");
        if (sel_rest) {
          /* DECLARE cursor CURSOR FOR SELECT ... */
          /* Parse the SELECT statement for host variables */
          host_list = parse_host_vars(cur_rest, sql_body, sizeof(sql_body),
                                      &host_count);
          command = host_count > 0 ? CB_SQL_DECLARE_CURSOR_PARAMS
                                   : CB_SQL_DECLARE_CURSOR;
          return build_and_resolve_exec_sql(
              command, strdup(sql_body), strdup(cname), NULL,
              conn_use_other_db ? strdup(db_name) : NULL, host_list, host_count,
              NULL, 0, conn_use_other_db);
        } else {
          /* DECLARE cursor CURSOR FOR prepared_name */
          char pname[256];
          extract_ident(cur_rest, pname, sizeof(pname));
          command = CB_SQL_DECLARE_CURSOR;
          return build_and_resolve_exec_sql(
              command, strdup(""), strdup(cname), strdup(pname),
              conn_use_other_db ? strdup(db_name) : NULL, NULL, 0, NULL, 0,
              conn_use_other_db);
        }
      }
    }
  }

  /* OPEN cursor [USING host_references] */
  rest = match_keyword(p, "OPEN");
  if (rest) {
    rest = skip_ws(rest);
    char cname[256];
    rest = extract_ident(rest, cname, sizeof(cname));
    rest = skip_ws(rest);

    const char *using_rest = match_keyword(rest, "USING");
    if (using_rest) {
      /* Parse host variables from USING clause */
      using_rest = skip_ws(using_rest);
      host_list = NULL;
      host_count = 0;
      while (*using_rest == ':') {
        using_rest++;
        char varname[256];
        using_rest = extract_ident(using_rest, varname, sizeof(varname));
        cb_tree ref = cb_build_reference(varname);
        struct cb_sql_host_var *hv =
            cb_build_sql_host_var(strdup(varname), ref);
        host_list = cb_sql_host_var_list_add(host_list, hv);
        host_count++;
        using_rest = skip_ws(using_rest);
        if (*using_rest == ',') {
          using_rest++;
          using_rest = skip_ws(using_rest);
        }
      }
      command = CB_SQL_OPEN_CURSOR_PARAMS;
    } else {
      command = CB_SQL_OPEN_CURSOR;
    }
    return build_and_resolve_exec_sql(
        command, strdup(""), strdup(cname), NULL,
        conn_use_other_db ? strdup(db_name) : NULL, host_list, host_count, NULL,
        0, conn_use_other_db);
  }

  /* CLOSE cursor */
  rest = match_keyword(p, "CLOSE");
  if (rest) {
    rest = skip_ws(rest);
    char cname[256];
    extract_ident(rest, cname, sizeof(cname));
    command = CB_SQL_CLOSE_CURSOR;
    return build_and_resolve_exec_sql(command, strdup(""), strdup(cname), NULL,
                                      conn_use_other_db ? strdup(db_name)
                                                        : NULL,
                                      NULL, 0, NULL, 0, conn_use_other_db);
  }

  /* FETCH cursor INTO :var1, :var2, ... */
  rest = match_keyword(p, "FETCH");
  if (rest) {
    rest = skip_ws(rest);
    char cname[256];
    rest = extract_ident(rest, cname, sizeof(cname));
    rest = skip_ws(rest);

    const char *into_rest = match_keyword(rest, "INTO");
    if (into_rest) {
      into_rest = skip_ws(into_rest);
      res_host_list = NULL;
      res_host_count = 0;
      while (*into_rest == ':') {
        into_rest++;
        char varname[256];
        into_rest = extract_ident(into_rest, varname, sizeof(varname));
        cb_tree ref = cb_build_reference(varname);
        struct cb_sql_host_var *hv =
            cb_build_sql_host_var(strdup(varname), ref);
        res_host_list = cb_sql_host_var_list_add(res_host_list, hv);
        res_host_count++;
        into_rest = skip_ws(into_rest);
        if (*into_rest == ',') {
          into_rest++;
          into_rest = skip_ws(into_rest);
        }
      }
    }

    command = CB_SQL_FETCH_ONE;
    return build_and_resolve_exec_sql(
        command, strdup(""), strdup(cname), NULL,
        conn_use_other_db ? strdup(db_name) : NULL, NULL, 0, res_host_list,
        res_host_count, conn_use_other_db);
  }

  /* PREPARE stname FROM :query */
  rest = match_keyword(p, "PREPARE");
  if (rest) {
    rest = skip_ws(rest);
    char pname[256];
    rest = extract_ident(rest, pname, sizeof(pname));
    rest = skip_ws(rest);
    rest = match_keyword(rest, "FROM");
    if (rest) {
      rest = skip_ws(rest);
      if (*rest == ':') {
        rest++;
        char varname[256];
        extract_ident(rest, varname, sizeof(varname));
        cb_tree ref = cb_build_reference(varname);
        struct cb_sql_host_var *hv =
            cb_build_sql_host_var(strdup(varname), ref);
        host_list = cb_sql_host_var_list_add(NULL, hv);
        host_count = 1;
      }
    }
    command = CB_SQL_PREPARE;
    return build_and_resolve_exec_sql(
        command, strdup(""), NULL, strdup(pname),
        conn_use_other_db ? strdup(db_name) : NULL, host_list, host_count, NULL,
        0, conn_use_other_db);
  }

  /* EXECUTE stname [USING :var1, :var2, ...] */
  rest = match_keyword(p, "EXECUTE");
  if (rest) {
    rest = skip_ws(rest);
    char pname[256];
    rest = extract_ident(rest, pname, sizeof(pname));
    rest = skip_ws(rest);

    const char *using_rest = match_keyword(rest, "USING");
    if (using_rest) {
      using_rest = skip_ws(using_rest);
      host_list = NULL;
      host_count = 0;
      while (*using_rest == ':') {
        using_rest++;
        char varname[256];
        using_rest = extract_ident(using_rest, varname, sizeof(varname));
        cb_tree ref = cb_build_reference(varname);
        struct cb_sql_host_var *hv =
            cb_build_sql_host_var(strdup(varname), ref);
        host_list = cb_sql_host_var_list_add(host_list, hv);
        host_count++;
        using_rest = skip_ws(using_rest);
        if (*using_rest == ',') {
          using_rest++;
          using_rest = skip_ws(using_rest);
        }
      }
    }

    command = CB_SQL_EXECUTE_PREPARED;
    return build_and_resolve_exec_sql(
        command, strdup(""), NULL, strdup(pname),
        conn_use_other_db ? strdup(db_name) : NULL, host_list, host_count, NULL,
        0, conn_use_other_db);
  }

  /* SELECT ... INTO ... FROM ... */
  rest = match_keyword(p, "SELECT");
  if (rest) {
    /* Find INTO clause to split into result vars and SQL body */
    /* SQL text: SELECT cols INTO :vars FROM table WHERE ... */
    /* Need to extract: SELECT cols FROM table WHERE ... (with host vars as ?)
     */
    /* and result host vars from INTO clause */

    const char *into_pos = NULL;
    const char *from_pos = NULL;
    const char *scan = rest;
    int paren_depth = 0;

    /* Find INTO and FROM positions (case insensitive) */
    while (*scan) {
      if (*scan == '(') {
        paren_depth++;
      } else if (*scan == ')') {
        paren_depth--;
      } else if (paren_depth == 0) {
        if (match_keyword(scan, "INTO") &&
            (scan == rest || scan[-1] == ' ' || scan[-1] == '\t')) {
          into_pos = scan;
        }
        if (match_keyword(scan, "FROM") && !from_pos &&
            (scan == rest || scan[-1] == ' ' || scan[-1] == '\t')) {
          /* Only set from_pos if it's after INTO */
          if (into_pos) {
            from_pos = scan;
            break;
          }
        }
      }
      scan++;
    }

    if (into_pos) {
      /* SELECT ... INTO :vars FROM ... */
      /* Build SQL body: SELECT <cols> FROM <rest> (with host vars replaced) */
      char temp_sql[8192];
      size_t temp_len = 0;

      /* Copy "SELECT " */
      memcpy(temp_sql, "SELECT ", 7);
      temp_len = 7;

      /* Copy columns (between SELECT and INTO) */
      const char *cols_start = skip_ws(rest);
      size_t cols_len = into_pos - cols_start;
      /* Trim trailing whitespace */
      while (cols_len > 0 && (cols_start[cols_len - 1] == ' ' ||
                              cols_start[cols_len - 1] == '\t')) {
        cols_len--;
      }
      memcpy(temp_sql + temp_len, cols_start, cols_len);
      temp_len += cols_len;

      /* Parse result host vars from INTO clause */
      const char *into_content = skip_ws(into_pos + 4); /* skip "INTO" */
      res_host_list = NULL;
      res_host_count = 0;

      while (*into_content && into_content < from_pos) {
        into_content = skip_ws(into_content);
        if (*into_content == ':') {
          into_content++;
          char varname[256];
          into_content = extract_ident(into_content, varname, sizeof(varname));
          cb_tree ref = cb_build_reference(varname);
          struct cb_sql_host_var *hv =
              cb_build_sql_host_var(strdup(varname), ref);
          res_host_list = cb_sql_host_var_list_add(res_host_list, hv);
          res_host_count++;
          into_content = skip_ws(into_content);
          if (*into_content == ',') {
            into_content++;
          }
        } else {
          into_content++;
        }
      }

      /* Append FROM clause and rest */
      if (from_pos) {
        temp_sql[temp_len++] = ' ';
        size_t rest_len = strlen(from_pos);
        memcpy(temp_sql + temp_len, from_pos, rest_len);
        temp_len += rest_len;
      }
      temp_sql[temp_len] = '\0';

      /* Parse host variables in the full SQL (WHERE clause params etc.) */
      host_list =
          parse_host_vars(temp_sql, sql_body, sizeof(sql_body), &host_count);

      command = host_count > 0 || res_host_count > 0 ? CB_SQL_SELECT_INTO_ONE
                                                     : CB_SQL_EXEC;

      return build_and_resolve_exec_sql(
          command, strdup(sql_body), NULL, NULL,
          conn_use_other_db ? strdup(db_name) : NULL, host_list, host_count,
          res_host_list, res_host_count, conn_use_other_db);
    } else {
      /* SELECT without INTO - this is a cursor declaration or general query */
      /* Reconstruct: SELECT <rest> */
      char temp_sql[8192];
      snprintf(temp_sql, sizeof(temp_sql), "SELECT %s", rest);
      host_list =
          parse_host_vars(temp_sql, sql_body, sizeof(sql_body), &host_count);

      command = host_count > 0 ? CB_SQL_EXEC_PARAMS : CB_SQL_EXEC;
      return build_and_resolve_exec_sql(
          command, strdup(sql_body), NULL, NULL,
          conn_use_other_db ? strdup(db_name) : NULL, host_list, host_count,
          NULL, 0, conn_use_other_db);
    }
  }

  /* INSERT, UPDATE, DELETE, and other SQL (DDL, etc.) */
  /* These all follow the same pattern: parse host vars and generate exec */
  host_list = parse_host_vars(p, sql_body, sizeof(sql_body), &host_count);

  command = host_count > 0 ? CB_SQL_EXEC_PARAMS : CB_SQL_EXEC;
  return build_and_resolve_exec_sql(command, strdup(sql_body), NULL, NULL,
                                    conn_use_other_db ? strdup(db_name) : NULL,
                                    host_list, host_count, NULL, 0,
                                    conn_use_other_db);
}

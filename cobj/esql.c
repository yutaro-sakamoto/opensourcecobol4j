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

/* ESQL bison/flex parser declarations */
struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE esql__scan_string(const char *str);
extern void esql__delete_buffer(YY_BUFFER_STATE buf);
extern int esql_parse(void);
extern void esql_scanner_reset(void);
extern void esql_parser_init(void);
extern cb_tree esql_parsed_result;

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

cb_tree esql_build_and_resolve(enum cb_sql_command command, char *sql_text,
                               char *cursor_name, char *prepare_name,
                               char *db_name, struct cb_sql_host_var *host_list,
                               int host_count,
                               struct cb_sql_host_var *res_host_list,
                               int res_host_count, int conn_use_other_db) {
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
 * Uses the ESQL bison parser (esql-parser.y) and lexer (esql-scanner.l).
 * Returns a cb_exec_sql AST node.
 */
cb_tree cb_parse_exec_sql(const char *sql_text) {
  esql_parser_init();
  esql_parsed_result = cb_error_node;

  YY_BUFFER_STATE buf = esql__scan_string(sql_text);
  esql_scanner_reset();
  if (esql_parse() != 0) {
    esql_parsed_result = cb_error_node;
  }
  esql__delete_buffer(buf);

  return esql_parsed_result;
}

/*
 * Helper: create a cb_field with given properties.
 */
static struct cb_field *make_sqlca_field(int level, const char *name,
                                         const char *pic_str, int usage) {
  struct cb_field *f =
      CB_FIELD(cb_build_field(cb_build_reference((char *)name)));
  f->level = level;
  f->storage = CB_STORAGE_WORKING;
  if (pic_str) {
    f->pic = CB_PICTURE(cb_build_picture(pic_str));
  }
  f->usage = usage;
  return f;
}

/*
 * Inject SQLCA structure into the compiler's field tree.
 * Called from parser.y when the first EXEC SQL statement is encountered.
 * This avoids preprocessor text insertion that would shift line numbers.
 */
void esql_inject_sqlca(void) {
  struct cb_field *sqlca, *child, *prev, *grp, *gprev;

  /* 01 SQLCA GLOBAL */
  sqlca = make_sqlca_field(1, "SQLCA", NULL, CB_USAGE_DISPLAY);
  sqlca->flag_is_global = 1;

  /* 05 SQLCAID PIC X(8) */
  child = make_sqlca_field(5, "SQLCAID", "X(8)", CB_USAGE_DISPLAY);
  child->parent = sqlca;
  sqlca->children = child;
  prev = child;

  /* 05 SQLCABC PIC S9(9) COMP-5 */
  child = make_sqlca_field(5, "SQLCABC", "S9(9)", CB_USAGE_COMP_5);
  child->parent = sqlca;
  prev->sister = child;
  prev = child;

  /* 05 SQLCODE PIC S9(9) COMP-5 */
  child = make_sqlca_field(5, "SQLCODE", "S9(9)", CB_USAGE_COMP_5);
  child->parent = sqlca;
  prev->sister = child;
  prev = child;

  /* 05 SQLERRM (group) */
  grp = make_sqlca_field(5, "SQLERRM", NULL, CB_USAGE_DISPLAY);
  grp->parent = sqlca;
  prev->sister = grp;
  prev = grp;

  /* 49 SQLERRML PIC S9(4) COMP-5 */
  child = make_sqlca_field(49, "SQLERRML", "S9(4)", CB_USAGE_COMP_5);
  child->parent = grp;
  grp->children = child;
  gprev = child;

  /* 49 SQLERRMC PIC X(70) */
  child = make_sqlca_field(49, "SQLERRMC", "X(70)", CB_USAGE_DISPLAY);
  child->parent = grp;
  gprev->sister = child;

  /* 05 SQLERRP PIC X(8) */
  child = make_sqlca_field(5, "SQLERRP", "X(8)", CB_USAGE_DISPLAY);
  child->parent = sqlca;
  prev->sister = child;
  prev = child;

  /* 05 SQLERRD PIC S9(9) COMP-5 OCCURS 6 */
  child = make_sqlca_field(5, "SQLERRD", "S9(9)", CB_USAGE_COMP_5);
  child->parent = sqlca;
  child->flag_occurs = 1;
  child->occurs_min = 6;
  child->occurs_max = 6;
  child->indexes = 1;
  prev->sister = child;
  prev = child;

  /* 05 SQLWARN (group) */
  grp = make_sqlca_field(5, "SQLWARN", NULL, CB_USAGE_DISPLAY);
  grp->parent = sqlca;
  prev->sister = grp;
  prev = grp;

  /* 10 SQLWARN0-7 PIC X(1) */
  {
    int i;
    struct cb_field *wprev = NULL;
    for (i = 0; i < 8; i++) {
      char wname[16];
      snprintf(wname, sizeof(wname), "SQLWARN%d", i);
      child = make_sqlca_field(10, wname, "X(1)", CB_USAGE_DISPLAY);
      child->parent = grp;
      if (i == 0) {
        grp->children = child;
      } else {
        wprev->sister = child;
      }
      wprev = child;
    }
  }

  /* 05 SQLSTATE PIC X(5) */
  child = make_sqlca_field(5, "SQLSTATE", "X(5)", CB_USAGE_DISPLAY);
  child->parent = sqlca;
  prev->sister = child;

  /* Validate each field individually, then the root */
  {
    struct cb_field *f;
    for (f = sqlca->children; f; f = f->sister) {
      if (f->children) {
        struct cb_field *c;
        for (c = f->children; c; c = c->sister) {
          cb_validate_field(c);
        }
      }
      cb_validate_field(f);
    }
  }
  cb_validate_field(sqlca);

  /* Set count=1 so codegen generates storage declarations */
  {
    struct cb_field *f;
    sqlca->count = 1;
    for (f = sqlca->children; f; f = f->sister) {
      f->count = 1;
      if (f->children) {
        struct cb_field *c;
        for (c = f->children; c; c = c->sister) {
          c->count = 1;
        }
      }
    }
  }

  current_program->working_storage =
      cb_field_add(current_program->working_storage, sqlca);
}

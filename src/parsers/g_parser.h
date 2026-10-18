#ifndef G_PARSER_H
#define G_PARSER_H
#include "ct_vec.h"
struct stmt_inc;
struct pl_op_entry;
typedef struct { char ** src_lines; int src_nlines; char * src_path; struct stmt_inc * incs; int n_incs; int cap_incs; } g_parser_stmt_t;
typedef struct { struct pl_op_entry * uinfix; int uinfix_n; int uinfix_cap; int dcg_var_counter; int pb_fresh_ctr; cv_t consulted_v; int consulted_n; int consult_depth; } g_parser_pl_t;
typedef struct { char pp_src[1024]; } g_parser_icn_t;
typedef struct { g_parser_stmt_t stmt; g_parser_pl_t pl; g_parser_icn_t icn; } g_parser_t;
extern g_parser_t g_parser;
#endif

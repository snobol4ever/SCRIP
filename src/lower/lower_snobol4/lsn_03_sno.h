#ifndef LOWER_LOWER_SNOBOL4_LSN_03_SNO_H
#define LOWER_LOWER_SNOBOL4_LSN_03_SNO_H
#include "lower/lower_snobol4/lsn_head.h"
__attribute__((visibility("hidden"))) void sno_fatal(const char * what, const char * detail);
__attribute__((visibility("hidden"))) const tree_t * sfind(const tree_t * s, const char * tag);
__attribute__((visibility("hidden"))) const char * sfind_str(const tree_t * s, const char * tag);
__attribute__((visibility("hidden"))) tree_t * sfind_expr(const tree_t * s, const char * tag);
__attribute__((visibility("hidden"))) void sno_reg_var(const char * nm);
__attribute__((visibility("hidden"))) IR_t * sco_branch(scx_t * cx, const tree_t * pg, IR_t * γ, IR_t * ω);
__attribute__((visibility("hidden"))) const char * sno_qlit_fold(const tree_t * t);
__attribute__((visibility("hidden"))) const tree_t * sno_stmt_define(const tree_t * s, int * out_argbase);
__attribute__((visibility("hidden"))) IR_graph_t * sno_build_graph(const tree_t ** st, int nst, int entry_idx, const int * is_def, const char * result_name, long stno_base, long end_line, long code_line);
#endif

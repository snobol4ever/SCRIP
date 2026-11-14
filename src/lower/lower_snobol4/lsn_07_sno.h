#ifndef LOWER_LOWER_SNOBOL4_LSN_07_SNO_H
#define LOWER_LOWER_SNOBOL4_LSN_07_SNO_H
#include "lower/lower_snobol4/lsn_head.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
__attribute__((visibility("hidden"))) const tree_t * sgoto_expr(const tree_t * s, tree_e kind);
__attribute__((visibility("hidden"))) const tree_t * sgoto_direct(const tree_t * s, tree_e kind);
__attribute__((visibility("hidden"))) int sno_goto_specials_impossible(const tree_t * expr);
__attribute__((visibility("hidden"))) const char * sno_cset_fold(const tree_t * a);
__attribute__((visibility("hidden"))) const char * sno_pat_defer_arg_name(const tree_t * inner, int lead_star);
__attribute__((visibility("hidden"))) void sno_pre_req(scx_t * cx, const tree_t * t, IR_t * prim);
extern __attribute__((visibility("hidden"))) const char sno_elem_snapg[6];
__attribute__((visibility("hidden"))) int sno_pat_inline_ok(const tree_t * t);
__attribute__((visibility("hidden"))) int sno_rsv_walk(tree_t * t, const tree_t * s, int quiet);
__attribute__((visibility("hidden"))) char * sno_list_build_listing(const tree_t * prog);
__attribute__((visibility("hidden"))) tree_t * sno_noexec_program(const tree_t * prog);
#endif

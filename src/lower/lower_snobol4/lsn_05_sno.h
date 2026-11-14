#ifndef LOWER_LOWER_SNOBOL4_LSN_05_SNO_H
#define LOWER_LOWER_SNOBOL4_LSN_05_SNO_H
#include "lower/lower_snobol4/lsn_head.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
__attribute__((visibility("hidden"))) int sno_mkpat_here(const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * sno_lower_match(scx_t * cx, const tree_t * subj, const tree_t * repl_t, int has_repl, IR_t * sJ, IR_t * fJ, IR_t ** out_land);
__attribute__((visibility("hidden"))) int sno_pat_supported(const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * sx_scan(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * sx_assign(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) int sno_predef_registered(const char * fname);
__attribute__((visibility("hidden"))) const char * sno_t4_target(const char * op, int nops);
__attribute__((visibility("hidden"))) const tree_t * sno_const_val(const char * ck);
__attribute__((visibility("hidden"))) tree_e sno_pat_eff_kind(const tree_t * t);
__attribute__((visibility("hidden"))) int sno_is_fence(const tree_t * t);
__attribute__((visibility("hidden"))) int sno_is_flush(const tree_t * t);
__attribute__((visibility("hidden"))) int sno_is_fence1(const tree_t * t);
__attribute__((visibility("hidden"))) int sno_is_fence0(const tree_t * t);
__attribute__((visibility("hidden"))) void sno_seq_flatten_pat(const tree_t * t, sno_tvec_t * e);
__attribute__((visibility("hidden"))) const tree_t * sno_const_pat(const char * ck);
__attribute__((visibility("hidden"))) int sno_is_pattern_rhs(const tree_t * t);
__attribute__((visibility("hidden"))) int sno_pat_supported(const tree_t * t);
__attribute__((visibility("hidden"))) int sno_const_static_on(void);
int sno_name_prologue_bound(const char * nm);
__attribute__((visibility("hidden"))) void sno_prog_scan(const tree_t ** st, int nst);
__attribute__((visibility("hidden"))) int sno_cap_name_strict(void);
__attribute__((visibility("hidden"))) int sno_seq_tail(void);
__attribute__((visibility("hidden"))) int sno_alt_tail(void);
__attribute__((visibility("hidden"))) IR_t * sno_seq_nary(scx_t * cx, const tree_t ** elems, const char * rc, int ne, IR_t * succ, IR_t * fail, IR_t ** out_rtail);
__attribute__((visibility("hidden"))) IR_t * sno_capt_body(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail, IR_t ** out_itail);
__attribute__((visibility("hidden"))) const tree_t * sno_capt_elem_rewrite(const tree_t * t, sno_tvec_t * pro);
__attribute__((visibility("hidden"))) IR_t * sno_capt_prologue(scx_t * cx, sno_tvec_t * pro, IR_t * entry, IR_t * fail);
int sno_expr_mark(void);
void sno_pat_thunks_build(int p0);
#endif

#ifndef LOWER_LOWER_SNOBOL4_LSN_04_SNO_H
#define LOWER_LOWER_SNOBOL4_LSN_04_SNO_H
#include "lower/lower_snobol4/lsn_head.h"
__attribute__((visibility("hidden"))) IR_t * sx_idx_container(scx_t * cx, const tree_t * t, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) void sx_sub_container_only(IR_t * sub);
__attribute__((visibility("hidden"))) char * sno_scan_tmp(const char * pfx);
__attribute__((visibility("hidden"))) const tree_t * sno_pat_valued(scx_t * cx, const tree_t ** subj, int has_repl, const tree_t * pat, IR_t * ω, IR_t ** pent, IR_t ** pasn);
__attribute__((visibility("hidden"))) IR_t * sx_binop(scx_t * cx, const tree_t * t, int code, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) void sno_predef_note(const char * fname);
__attribute__((visibility("hidden"))) void sno_tdef_note(const char * fname);
__attribute__((visibility("hidden"))) int sno_tdef_registered(const char * fname);
__attribute__((visibility("hidden"))) int sno_pred_relop(const char * n, int * lex, long * c1, long * c2);
__attribute__((visibility("hidden"))) IR_t * sx_pred_cmp(scx_t * cx, const tree_t * t, int argbase, int lex, int rk, long c1, long c2, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * sx_ident_differ(scx_t * cx, const tree_t * t, int argbase, int is_differ, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * sno_defer_seal(scx_t * cx, IR_t * nd);
__attribute__((visibility("hidden"))) IR_t * sno_thunk_abort_exit(IR_graph_t * g, IR_t * no);
__attribute__((visibility("hidden"))) IR_t * sno_arm_result(IR_t * rv);
__attribute__((visibility("hidden"))) int sno_ident_inline_on(void);
__attribute__((visibility("hidden"))) int sno_vl_quiet(const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * sno_vlist_arms(scx_t * cx, const tree_t * t, int j, char * tn, IR_t * jn, IR_t * ω);
__attribute__((visibility("hidden"))) IR_t * sno_xname_lv(scx_t * cx, const tree_t * nexp, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * sx_scan_body(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
#endif

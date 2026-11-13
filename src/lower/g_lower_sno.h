#ifndef G_LOWER_SNO_H
#define G_LOWER_SNO_H
#include "ir/ct_vec.h"
typedef struct tree_t tree_t;
typedef struct {
    int sno_sub_val_on_v;
    int sno_scan_tmp_scv_n;
    int sx_binop_scl_n;
    int sx_call_named_c2bb;
    int sno_ident_inline_on_on;
    int sno_goto_special_chain_sp;
    int sno_goto_computed_target_igt_n;
    int sno_goto_computed_target_bn;
    int sno_goto_direct_target_dgt_n;
    int sno_const_feature_cs;
    int sno_const_t1_on_t;
    int sno_t4_on_v;
    int fc_tail_walk_dtl;
    int sno_cap_name_strict_v;
    int sno_rtseq_resume_v;
    int sno_defer_resume_v;
    int sno_seq_tail_v;
    int sno_kw_nest_ok_nn;
    int sno_pat_inline_ok_ia;
    int sno_pat_node_cn;
    int sno_pat_node_ci;
    int sno_pat_node_ck;
    int sno_pat_node_ec;
    int sno_is_pattern_rhs_kdepth;
    int sno_mkpat_here_on;
    int sno_patsalt_on_v;
    int sno_patname_salt_on_v;
    int sno_lower_match_preord;
    int sno_lower_match_snapctr;
    int sno_expr_thunks_build_xd;
    cv_t exprs;
    int expr_salt;
    cv_t pats;
    int uses_stmtkw;
    int traces_a_label;
    int uses_code;
    int uses_xname;
    int calls_code;
    cv_t predef;
    cv_t bare_pat;
    cv_t tdef;
    cv_t multiproto;
    cv_t proto_fn;
    cv_t proto_enc;
    int in_patproc;
    int pat_match_ctx;
    int seal_enabled;
    cv_t seal;
    cv_t pro;
    cv_t t4;
    int t4_unsafe;
    int in_arbno;
    cv_t scd;
    const tree_t * prescan_top;
    int expr_define_seen;
} g_lower_sno_t;
#define G_LOWER_SNO_INIT { .sno_sub_val_on_v = -1, .sx_call_named_c2bb = -1, .sno_ident_inline_on_on = -1, .sno_goto_special_chain_sp = -1, .sno_goto_computed_target_bn = -1, . sno_const_feature_cs \
    = -1, .sno_const_t1_on_t = -1, .sno_t4_on_v = -1, .fc_tail_walk_dtl = -1, .sno_cap_name_strict_v = -1, .sno_rtseq_resume_v = -1, .sno_defer_resume_v = -1, . sno_seq_tail_v = -1, . \
    sno_kw_nest_ok_nn = -1, .sno_pat_inline_ok_ia = -1, .sno_pat_node_cn = -1, .sno_pat_node_ci = -1, .sno_pat_node_ck = -1, .sno_pat_node_ec = -1, .sno_mkpat_here_on = -1, . sno_patsalt_on_v = -1, \
    .sno_patname_salt_on_v = -1, .sno_lower_match_preord = -1, .sno_expr_thunks_build_xd = -1 }
#ifdef __cplusplus
extern "C" {
#endif
extern g_lower_sno_t g_lower_sno;
#ifdef __cplusplus
}
#endif
#endif

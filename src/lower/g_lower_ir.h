#ifndef G_LOWER_IR_H
#define G_LOWER_IR_H
#include <stdint.h>
#include <stddef.h>
#include "ir/ct_vec.h"
#include "ir/ct_arena_ct_align.h"
typedef struct IR_t IR_t;
typedef struct IR_graph_t IR_graph_t;
typedef struct { const IR_graph_t * g; int i1; } zgh_t;
typedef struct {
    const IR_t * head;
    const IR_t * arbno;
    int i0;
    int ia;
    int b0;
    int b1;
    int r1;
    int fpl;
    int fpb;
    int fpr;
    int fpr_rsp;
    int span;
    int rspan;
    int opsb;
    int fin;
    int dfr;
    cv_t wsv;
    cv_t wcd;
    int nw;
} zls_fct_t;
typedef struct {
    int zls_slot_census_on;
    long zls_slot_census_tg;
    long zls_slot_census_tl;
    long zls_slot_census_tn;
    int zls_build_dbg;
    int zls_build_dbg2;
    int zls_build_dbg3;
    int zls_build_subj_on;
    int zls_build_dyn_on;
    int zls_build_dbg4;
    int zls_build_dbg5;
    int zls_build_eon;
    int zc_nofc_v;
    int fc_geom_ac;
    const char * fc_reg_hw_e;
    int zw_carve_k_ba;
    int zw_carve_k_all;
    const char * zw_carve_k_bo;
    const char * zw_carve_k_bs;
    int bb_print_v_xd2;
    int zdp_mode_m;
    cv_t nid;
    int nid_count;
    IR_t ** nidx_nodes;
    int nidx_n;
    IR_t ** nidx_keys;
    int * nidx_vals;
    size_t nidx_cap;
    int x86_zdp_on_c_v;
    int zzone_on_v;
    int zdp_cap_seamtier_v;
    int zdp_atp_seamtier_v;
    int zdp_bomb_census_bm;
    int ast_print_width;
    cv_t ze_v;
    int ze_n;
    cv_t zf_v;
    int zf_n;
    cv_t znb;
    uint32_t znb_gen;
    uint32_t znb_n;
    cv_t zs_v;
    int zs_n;
    cv_t zg_v;
    int zg_n;
    cv_t zv_v;
    int zv_n;
    cv_t zm_v;
    int zm_n;
    cv_t zx_v;
    int zx_n;
    int zx_sorted;
    cv_t zx_tail;
    zgh_t * zgh;
    uint32_t zgh_cap;
    cv_t za_v;
    int za_n;
    int fct_n;
    int fct_pricing;
    int fcc_gfence;
    cv_t fcn;
    uint32_t fcn_gen;
    uint32_t fcn_n;
    cv_t fca;
    cv_t zlq_v;
    const int * seq_of_node;
    int seq_of_node_n;
    cv_t zdp_ent;
    cv_t zdp_idx;
    cv_t zdp_wl;
    int zdp_valid;
    int zzone_valid;
    zls_fct_t fct[64];
    struct ct_head * ct_bin[CT_BINS];
    struct ct_head * ct_jhead;
    int ct_poison;
    uint8_t * ct_cur;
    uint8_t * ct_end;
    size_t ct_taken;
    size_t ct_mapped;
    uint8_t * bb_pool_base;
    uint8_t * bb_pool_top;
    uint8_t * bb_pool_limit;
    long bb_pool_page_size;
    cv_t bb_holes;
    uint8_t * bb_last_lo;
    uint8_t * bb_last_hi;
} g_lower_ir_t;
#define G_LOWER_IR_INIT { .zls_slot_census_on = -1, .zls_build_dbg = -1, . zls_build_dbg2 = -1, .zls_build_dbg3 = -1, .zls_build_subj_on = -1, .zls_build_dyn_on = -1, .zls_build_dbg4 = -1, . \
    zls_build_dbg5 = -1, .zls_build_eon = -1, .zc_nofc_v = -1, .fc_geom_ac = -1, . fc_reg_hw_e = (const char *) 1, . zw_carve_k_ba = -1, .zw_carve_k_all = -1, .bb_print_v_xd2 = -1, .zdp_mode_m = -1, \
    .x86_zdp_on_c_v = -1, .zzone_on_v = -1, .zdp_cap_seamtier_v = -1, . zdp_atp_seamtier_v = -1, . zdp_bomb_census_bm = -1, .ast_print_width = 140, .znb_gen = 1, .zgh = (zgh_t *)0, .fcn_gen = 1, . \
    seq_of_node = (const int *)0, .ct_poison = -1 }
#ifdef __cplusplus
extern "C" {
#endif
extern g_lower_ir_t g_lower_ir;
#ifdef __cplusplus
}
#endif
#endif

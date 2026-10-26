#ifndef G_EMITTER_H
#define G_EMITTER_H
#ifndef __cplusplus
#error g_emitter.h is C++ only (CEO-1575)
#endif
#include <unordered_map>
#include "ct_arena.h"
#include "ct_vec.h"
#include <string>
#include "emit.h"
#include "gc_frame_map.h"
#include "dtp.h"
#include "ir_index.h"
#include "templates/x86/x86_asm.h"
#include "templates/bb/bb_templates.h"
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "driver/scrip_sm.h"
#include "stage2.h"
#include "templates/xa/xa_templates.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "planner/g_planner.h"
struct bb_ab_thunk_stems_t { std::unordered_map<std::string, std::string> stem; std::unordered_set<std::string> stems; int np; int nx; std::string fn; };
typedef struct { int32_t line; int32_t _r; const char * file; } sno_stno_src_t;
typedef struct { IR_t * key; int val; } nid_slot_t;
typedef struct { const char *s; int idx; } strtab_ent_t;
typedef struct { const char *s; int idx; unsigned char *tbl; unsigned char *bits; } csettab_ent_t;
typedef struct { IR_t *key; int off; } bb_slotmap_ent_t;
typedef struct { IR_t *key; int idx; uint32_t gen; } bb_slotix_t;
typedef struct { const char * name; int depth; } zv_home_t;
typedef struct { IR_t *key; bb_box_fn fn; const char *text_lbl; } child_cache_ent_t;
typedef struct { const void *k; uint32_t gen; } bnr_slot_t;
typedef struct { const char * name; int slot; } ab_ent_t;
struct emit_dir_ref { cv_t * chunks; cv_t * names; cv_t * hix; int * n; size_t esz; };
typedef struct { unsigned char set[256]; int nempty, impure, unknown; } blob_fg_t;
typedef struct { cv_t var; cv_t thunk; } blob_fg_deps_t;
typedef struct { const char * var; const char * thunk; const char * from; int conflict; } blob_fg_bind_t;
typedef struct { IR_t * t; int d0; int nd; int multi; } zdw_ent_t;
typedef struct { uint64_t pc; int32_t depth; uint16_t kind; uint16_t rule; int32_t zdepth, fcb, fcbase, demit; const char * bk; int mn; const IR_t * nd; } gc_site_t;
typedef struct { int enc_kind, enc_depth; const IR_t * nd; } gc_match_lvl_t;
struct zeta_plan_t;
struct g_emitter_t {
    int dump_homes = 0;
    int dump_plan = 0;
    int g_zd_abs = 0;
    int g_zd_def = 1;
    int g_zd_cutd = -1;
    const unsigned char * g_zd_on_cur = NULL;
    int g_zd_n_cur = 0;
    std::unordered_set<std::string> g_port_exit_promoting_labels;
    bb_label_t ** g_label_pool = NULL;
    std::unordered_map<std::string, bb_label_t *> g_label_map;
    int g_label_pool_n = 0;
    int g_label_pool_max = 0;
    bb_label_t * g_flt_lbl[4] = {0, 0, 0, 0};
    const char * g_flt_fam = (const char *)0;
    char flat_label_kind_b[48];
    bb_label_t *g_lblfold_from[4096];
    bb_label_t *g_lblfold_to[4096];
    int g_lblfold_n = 0;
    cv_t bb_patch_hix, bb_patch_hix2;
    uint32_t bb_patch_hused = 0, bb_patch_gen = 1;
    int bb_patch_n = 0;
    int seq_fold_on_on = -1;
    int bb_emit_overflow = 0;
    int bb_label_define_bm = -1;
    cv_t g_nid;
    int g_nid_count = 0;
    cv_t g_strtab;
    int g_strtab_n = 0;
    cv_t xa_emit_strtab_rodata_chars, xa_emit_strtab_rodata_labels, xa_emit_strtab_rodata_escaped;
    int sn4_cset32__c = -1;
    cv_t g_csettab;
    int g_csettab_n = 0;
    cv_t xa_emit_csettab_rodata_chars, xa_emit_csettab_rodata_rows, xa_emit_csettab_rodata_labels;
    long flat_buf_max_v_v = -1;
    bb_slotmap_ent_t *g_bb_slotmap = NULL;
    int g_bb_slotmap_n = 0;
    bb_slotix_t *g_bb_slotix = NULL;
    uint32_t g_bb_slotix_cap = 0, g_bb_slotix_gen = 1;
    int g_bb_slotmap_max = 0;
    int fbdiv_n = 0;
    int fbdiv_mode = -1;
    int fbdiv_hooked = 0;
    bb_label_t *g_limit_gen_beta = NULL;
    bb_label_t *g_scan_body_beta = NULL;
    int g_scan_t0_pop = 0;
    int g_limit_t0_pop = 0;
    bb_label_t *g_suspend_dobody_beta = NULL;
    int g_suspend_resume_slot = -1;
    int g_flat_data_active = 0;
    int g_flat_data_just_closed = 0;
    const char * g_flat_data_pending_lbl = "";
    bb_label_t g_α_ring[8];
    int g_α_ring_i = 0;
    int g_bb_alpha_seq = 0;
    int fence_kk_gamma_on_v = -1;
    int fence0_dyn_on_v = -1;
    int frame_need_of__cn = -1;
    int bb_classify_node__ed = -1;
    int bb_classify_node__cd = -1;
    cv_t g_bnr_vnode, g_bnr_vname;
    uint32_t g_bnr_gen = 0, g_bnr_nn = 0, g_bnr_nm = 0;
    IR_t **g_nidx_nodes = 0;
    int g_nidx_n = 0;
    IR_t **g_nidx_keys = 0;
    int *g_nidx_vals = 0;
    size_t g_nidx_cap = 0;
    int g_zprobe_depth = 0;
    int walk_bb_node__zp = -1;
    int emit_sep_on__sep = -1;
    int g_zd_djok;
    int g_zd_stage, g_zd_arm, g_zd_k, g_zd_gpop, g_zd_bgrow, g_zd_wpop, g_zd_wsteal, g_zd_cpread, g_zd_rdread;
    int g_arbk16_stmt = 0;
    long g_zd_suspend_uclaim = 0;
    int sr3_gamma_label__off = -1;
    int walk_bb_node_inner__ba = -1;
    const char * walk_bb_node_inner__bo;
    const char * walk_bb_node_inner__bs;
    int walk_bb_node_inner__all = -1;
    int walk_bb_node_inner_bm = -1;
    int walk_bb_node_inner__sc = -1;
    int walk_bb_node_inner__dl = -1;
    int walk_bb_node_inner__dd = -1;
    int walk_bb_node_inner__k16e = -1;
    int walk_bb_node_inner__ad=-1;
    int walk_bb_node_inner__dtl = -1;
    int walk_bb_node_inner__pk = -1;
    int walk_bb_node_inner__mgt = -1;
    int walk_bb_node_inner__zdpa = -1;
    cv_t g_ab_dir, g_ab_names, g_ab_hix;
    int g_ab_fn_cell_n = 0;
    int sn4_arbno_reentry_v = -1;
    int cap_fail_retreat__cf = -1;
    uint32_t g_xci_epoch = 1;
    int sn4_defer_resume__dres = -1;
    int sn4_arbno_altsib_v = -1;
    int sn4_arbno_tailbeta__atb = -1;
    int sn4_seam_walk__sw = -1;
    int sn4_alt_carrier__ac = -1;
    const IR_graph_t * g_bcs_g;
    IR_t ** g_bcs_all;
    int g_bcs_n = -1, g_bcs_nc, g_bcs_lf, g_bcs_fn;
    uint32_t g_bcs_ep;
    int blob_carve_pad_v = -1;
    cv_t g_blob_lay;
    cv_t g_alt_pads;
    int g_zd_fence_exit = 0;
    cv_t g_zls_lay_v;
    int g_zls_lay_gaps = 0, g_zls_lay_conf = 0;
    int blob_cont_copy_v = -1;
    int blob_zf_gc_only_v = -1;
    int blob_zf_rep_v = -1;
    int blob_omega_ret_v = -1;
    int blob_fg_on_v = -1;
    int blob_alt_guard_on_v = -1;
    int blob_fg_bind_on_v = -1;
    int blob_first_guard_seq = 0;
    int x86_diag_regs_on_c_v = -1;
    int zop_audit_reg = 0;
    long zop_g_total = 0, zop_g_mixed = 0;
    long zop_hist[16] = {0};
    std::unordered_set<std::string> icn_trace_intern_pool;
    bb_label_t g_gc_map_lbl;
    int g_gc_map_pending = 0, g_gc_value_bytes = -1, g_gc_map_fb = 0, g_gc_map_hdr = 0, g_gc_map_last_off = -1, g_gc_map_names_n = 0;
    unsigned g_gc_map_flags = 0;
    uint64_t g_gc_map_link = 0;
    cv_t g_gc_map_names_v;
    int gc_sites_report_on_v = -1;
    cv_t g_gc_sites_v;
    int g_gc_sites_armed = 0, g_gc_sites_last_off = -1, g_gc_sites_names_n = 0;
    cv_t g_gc_sites_names_v;
    gc_match_lvl_t g_gc_match_out;
    int g_gc_match_n = 0;
    cv_t g_gc_sites_offs_v;
    int emit_gc_sites_data_seq = 0;
    uint64_t g_gc_site_play_seq = 0;
    int gc_maps_report_on_v = -1;
    bb_label_t **codegen_flat_chain_body_alt_tr = 0;
    bb_label_t **codegen_flat_chain_body_ret_tr = 0;
    int codegen_flat_chain_body_alt_tr_cap = 0;
    int codegen_flat_chain_body__zk0_on = -1;
    int codegen_flat_chain_body__plzk0_on = -1;
    int codegen_flat_chain_body__zw5_on = -1;
    int codegen_flat_chain_body__lpd = -1;
    int codegen_flat_chain_body__tp = -1;
    int codegen_flat_chain_body__en = -1;
    int codegen_flat_chain_body__iws = -1;
    int codegen_flat_chain_body__en_0 = -1;
    int codegen_flat_chain_body__hd = -1;
    int codegen_flat_chain_body__gluo = -1;
    int codegen_flat_chain_body__gluod = -1;
    int codegen_flat_chain_body__diag_chain_seq = 0;
    int codegen_flat_chain_body__uwl = -1;
    int codegen_flat_chain_body__sz = -1;
    int codegen_flat_chain_body__beo = -1;
    int codegen_flat_chain_body__dd = -1;
    int codegen_flat_chain_body__dd_0 = -1;
    int codegen_flat_chain_body__zpf = -1;
    int codegen_flat_chain_body__dd_1 = -1;
    int codegen_flat_chain_body__uw = -1;
    int codegen_flat_chain_body__zpd = -1;
    int codegen_flat_chain_body__sz_0 = -1;
    const char ** g_rtxe_tab = (const char **)0;
    uint32_t g_rtxe_cap = 0;
    int emit_jmp_entry_arm_region_v = -1;
    ir_index_t g_fbm_nd;
    cv_t g_fbm_bit;
    int g_fbm_n = 0;
    int emit_fb_stmt_scan_fbdbg = -1;
    int emit_stmt_frame_scan__sd = -1;
    int emit_stmt_frame_scan__c2c = -1;
    int emit_chain_fbdbg2 = -1;
    std::string g_text_acc;
    FILE * g_emit_sink = NULL;
    long g_emit_text_count = 0;
    int sn4_defer_beta_guard__bg = -1;
    int sn4_define_fold__df = -1;
    int sn4_descr_stamp_on = -1;
    int sn4_u2_fence_v = -1;
    int sn4_nret_cap_v = -1;
    int sn4_cap_gva_v = -1;
    int sn4_opt_binimm_off_v = -1;
    std::unordered_set<std::string> xa_icn_trace_intern_pool;
    int bcps_sig_tally__sd = -1;
    int defer_prepare_g_defer_site_n;
};
extern g_emitter_t g_emitter;
#endif

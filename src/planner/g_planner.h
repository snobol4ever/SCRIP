#ifndef G_PLANNER_H
#define G_PLANNER_H
#ifndef __cplusplus
#error g_planner.h is C++ only (CEO-1575)
#endif
#include <unordered_map>
#include "ct_arena.h"
#include "ct_vec.h"
#include <string>
#include "gc_frame_map.h"
#include "dtp.h"
#include "ir_index.h"
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "stage2.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
struct zeta_plan_t;
typedef struct { const IR_t * nd; uint32_t gen; } ocr_slot_t;
struct g_planner_t {
    struct zsm_frame_rec_t { const IR_graph_t * g; std::string name; int region; int resident; int frames; int addrs; int cls[4]; int rsp_form; int total; };
    const int * g_zd_cut_p = 0;
    const int * g_zd_claim_p = 0;
    const int * g_zd_xb_p = 0;
    const int * g_zd_fam_p = 0;
    const int * g_zd_fbase_p = 0;
    const int * g_zd_gp_p = 0;
    const int * g_zd_out_p = 0;
    const int * g_zd_wp_p = 0;
    std::vector<zsm_frame_rec_t> zsm_frames;
    int zsm_frames_last = -1;
    int zsm_frame_on = -1;
    int zsm_frame_diag = -1;
    IR_t ** g_zd_nodes_p = 0;
    int g_zd_n = 0;
    int g_zd_cur = -1;
    IR_t ** zst_nodes = NULL;
    int zst_n = 0;
    std::vector<int> zst_s, zst_root, zst_vroot, zst_c, zst_j, zst_L, zst_vuse, zst_vcov;
    std::vector<unsigned char> pdef; const IR_graph_t * pdef_g; int pdef_n; uint64_t pdef_h; const IR_graph_t * pdef_lock = 0; int pdef_depth = 0;
    std::unordered_map<const IR_t *, int> pdef_ix; std::unordered_map<const IR_t *, unsigned char> pdef_memo; std::unordered_map<const IR_t *, const IR_t *> pdef_gb; int pdef_gb_ok = 0;
    std::vector<const char *> zst_name;
    std::unordered_map<const IR_t *, int> zst_sm;
    int g_zd_cps = -1;
    int g_zd_cpd = -1;
    const unsigned char * g_zd_plan_on = NULL;
    IR_t ** g_zd_plan_nodes = NULL;
    const int * g_zd_zarm_p = 0;
    IR_t ** g_zd_zarm_nodes = NULL;
    int g_zd_zarm_n = 0;
    std::vector<int> g_zd_djx;
    std::vector<unsigned char> g_zd_admit;
    zeta_plan_t * g_zd_rec_p = (zeta_plan_t *)0;
    const IR_graph_t * g_pcon_g = (const IR_graph_t *)0;
    IR_t ** g_pcon_all = (IR_t **)0;
    int g_pcon_n = 0, g_pcon_ret = 0;
    std::vector<int> g_pcon_off, g_pcon_con;
    std::unordered_map<const IR_t *, int> g_pcon_idx;
    std::vector<int> g_zd_upost;
    IR_t ** g_zd_djx_nodes = NULL;
    std::vector<int> g_zd_grec, g_zd_wrec, g_zd_grec_pop, g_zd_wrec_pop;
    int g_zd_grec_off = -1, g_zd_wrec_off = -1, g_zd_grec_res = 0, g_zd_wrec_res = 0, g_zd_cpsx = -1, g_zd_cpdx = -1;
    int g_zd_plan_n = 0;
    const IR_graph_t * g_act_g = (const IR_graph_t *)0;
    IR_t ** g_act_all = (IR_t **)0;
    int g_act_n = -1, g_act_blob = -1, g_act_head = 0, g_act_blob_units = 0;
    std::vector<int> g_act_cls, g_act_cause, g_act_slot, g_act_units, g_act_extra, g_act_owner;
    int zd_map_on__m = -1;
    int fence0_whack_on_v = -1;
    int fence0_dyn_on_v = -1;
    cv_t g_zdo;
    uint32_t g_zdo_gen = 0, g_zdo_cnt = 0;
    IR_t **g_zdo_nodes = 0;
    int g_zdo_n = -1;
    cv_t g_zrm_map;
    uint32_t g_zrm_gen = 1;
    int zd_plan__dg = -1;
    int zd_plan__lp=-1;
    int zd_plan__gp = -1;
    int zd_plan__cd = -1;
    int zd_depth_census__on = -1;
    int zd_check__on = -1;
    IR_t **codegen_flat_chain_body_nodes = 0;
    int codegen_flat_chain_body_nodes_cap = 0;
    IR_t **codegen_flat_chain_body_queue = 0;
    int codegen_flat_chain_body_queue_cap = 0;
    IR_t **codegen_flat_chain_body_postv = 0;
    int codegen_flat_chain_body_postv_cap = 0;
    std::unordered_map<const IR_t *, struct zeta_plan_t *> planner_table;
    int dump_plan_cols = 0;
    std::vector<signed char> dh_rt;
    std::unordered_map<std::string, int> dh_gidx;
    int planner_hits = 0, planner_fallbacks = 0, planner_planned = 0, planner_rt = 0, planner_report_armed = 0;
    int sn4_blob_casmark_v = -1;
    cv_t g_ocr_map;
    uint32_t g_ocr_gen = 1, g_ocr_cnt = 0;
    cv_t g_ocr_chain, g_ocr_stkv, g_ocr_stk;
};
extern g_planner_t g_planner;
#endif

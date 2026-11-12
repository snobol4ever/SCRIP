#ifndef PLANNER_ZETA_PLAN_ZP_02_SH_H
#define PLANNER_ZETA_PLAN_ZP_02_SH_H
#include "planner/zeta_plan/zp_head.h"
extern "C" { IR_t * zd_chase(IR_t * t); }
extern "C" { int zd_yield_end(IR_t ** nodes, int n, int sp, const unsigned char * zon, const int * claim); }
extern "C" { int zd_nops_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { int zd_nops_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { int zd_pl_dj_arm_holds(const IR_t * d, int a); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pl_dj_holds(const IR_t * d); }
extern "C" { __attribute__((visibility("hidden"))) IR_t * zd_suspend_run_next(IR_t ** nodes, int n, IR_t * sp, const int * claim); }
extern "C" { __attribute__((visibility("hidden"))) int zd_icn_chain_step_ok(const IR_graph_t * cg, IR_t ** nodes, int n, int p, int q); }
extern "C" { __attribute__((visibility("hidden"))) int zd_map_on(void); }
extern "C" { __attribute__((visibility("hidden"))) void zdo_build(IR_t **nodes, int n); }
extern "C" { int bb_proc_target_retains(const char *fname); }
extern "C" { int planner_fence0_release(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) int zd_omega_head(IR_t **nodes, int n, IR_t *t); }
extern "C" { __attribute__((visibility("hidden"))) int zd_omega_seed(IR_t **nodes, int n, IR_t *t, unsigned char *zon, int *zout, const int *claim); }
extern "C" { __attribute__((visibility("hidden"))) int zd_omega_test_idx(IR_t **nodes, int n, IR_t *t); }
extern "C" { __attribute__((visibility("hidden"))) int zd_vl_join(const IR_t *last, const IR_t *cur); }
extern "C" { __attribute__((visibility("hidden"))) int zd_vl_sibling(IR_t **nodes, int n, const int *claim, int c, IR_t *cur); }
extern "C" { int zd_arrival_depth_g(const IR_graph_t * cg, IR_t **nodes, int n, int i, const unsigned char *zon, const int *zout, const int *zgp, const int *zwp); }
extern "C" { __attribute__((visibility("hidden"))) int zd_exit_pop_s(IR_e op, int mark, int full, int stmt, int mafter); }
extern "C" { __attribute__((visibility("hidden"))) const char * zd_edge_sz(IR_t * x, int v); }
extern "C" { __attribute__((visibility("hidden"))) int zd_sz_sigma(const char * sz); }
extern "C" { __attribute__((visibility("hidden"))) int zd_sz_phi(const char * sz); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pl_resume_depth(const IR_graph_t * cg, IR_t ** nodes, int n, const unsigned char * zon, const int * zout, const int * claim, const int * base, const unsigned char * known, int r); }
extern "C" { __attribute__((visibility("hidden"))) int zd_unmark_acts(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) IR_t * zd_lm_chase(IR_t * x, int n, IR_t ** last); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pred_bt(const IR_graph_t * cg, IR_t ** nodes, int n, const int * claim, int hi, int k); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pl_land(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, int i, int e, int *q, int *beta); }
extern "C" { __attribute__((visibility("hidden"))) int zd_g_into_glue_beta(IR_t * x, IR_t * t); }
extern "C" { void zd_depth_census_g(const IR_graph_t * cg, IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, const char *prefix); }
extern "C" { void zd_check_g(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, const int *xb, const int *zchn, const int *zcut, const int *zarm, const char *prefix); }
extern "C" { zeta_plan_t * planner_chain_plan(const IR_graph_t * cg, IR_t * entry, const char * prefix); }
#endif

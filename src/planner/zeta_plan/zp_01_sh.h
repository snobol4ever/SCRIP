#ifndef PLANNER_ZETA_PLAN_ZP_01_SH_H
#define PLANNER_ZETA_PLAN_ZP_01_SH_H
#include "planner/zeta_plan/zp_head.h"
extern "C" { extern g_planner_t g_planner; }
extern "C" { int zst_s_of(const IR_t * nd); }
extern "C" { int zd_edge_retains_g(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) int zd_arm_resumes(const IR_t * a, const IR_t * g); }
extern "C" { __attribute__((visibility("hidden"))) int zd_beta_via_act(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { int zd_djx_of(const IR_t * nd); }
extern "C" { int zd_upost_of(const IR_t * nd); }
extern "C" { int zd_k_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { int zd_kf_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { int zd_k(IR_t * nd); }
extern "C" { int zd_k_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { int zd_k(IR_t * nd); }
extern "C" { int zd_k_own_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { int zd_icn_suspend_g(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) int zd_icn_stmt_head(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { int zd_icn_local_var_g(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { int zd_k_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { int zd_k(IR_t * nd); }
extern "C" { int zd_icn_suspend_g(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { int zd_k_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { int zd_k(IR_t * nd); }
extern "C" { int zd_k_own_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { int zst_s_of(const IR_t * nd); }
extern "C" { int zd_k_own_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { int zd_dj_admitted(int n, int ei); }
extern "C" { int zd_dj_cellless(IR_t ** nodes, int n, const int * zarm, int k); }
extern "C" { IR_t * zd_value_producer_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) int zd_icn_stmt_sibling(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { int zd_rel_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { int zd_mframe_g(const IR_graph_t * cg, IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pl_home(const IR_graph_t * cg); }
extern "C" { __attribute__((visibility("hidden"))) int zd_bound_pop(const IR_graph_t * cg, IR_t * b); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pl_cut(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pl_entry(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, int i, int e, int *l, const unsigned char *dead); }
extern "C" { __attribute__((visibility("hidden"))) int zst_names(const IR_t * nd, const char ** out); }
extern "C" { __attribute__((visibility("hidden"))) void zd_dj_discards(const IR_graph_t * cg, IR_t ** nodes, int n, const unsigned char * zon, const int * zout, const int * zgpop, const int * zwpop, const int * zarm, const int * claim, const int * base, const unsigned char * known, int on); }
extern "C" { __attribute__((visibility("hidden"))) int zd_check_discard(const IR_t * t); }
extern "C" { __attribute__((visibility("hidden"))) int zd_check_arm_env(int o); }
extern "C" { __attribute__((visibility("hidden"))) void zd_place(const IR_graph_t * cg, zeta_plan_t * rec, IR_t ** nodes, int n, const unsigned char * zon, const int * zout, const int * zarm, const int * zcps, const int * zcpd); }
extern "C" { void planner_capture(zeta_plan_t * rec); }
extern "C" { int planner_entry_planned(const IR_t * entry); }
#endif

#ifndef PLANNER_ZETA_PLAN_ZP_03_SH_H
#define PLANNER_ZETA_PLAN_ZP_03_SH_H
#include "planner/zeta_plan/zp_head.h"
extern "C" { int zd_dj_arm_end(IR_t ** nodes, int n, int dj, int a); }
extern "C" { IR_t * zd_scan_subject(IR_t * nd); }
extern "C" { int planner_bound_spine(const IR_graph_t * g, const IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pl_bspine(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pl_rest_ival(const IR_graph_t * cg, const IR_t * u); }
extern "C" { __attribute__((visibility("hidden"))) int zd_mark_restore(const IR_t * u); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pl_unmark_bound(const IR_graph_t * cg, IR_t ** nodes, int n, int i); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pl_unmark_exit(const IR_graph_t * cg, IR_t ** nodes, int n, const unsigned char * zon, const int * zout, const int * claim, const unsigned char * known, const int * base, const int * fam, const int * fbase, int i, int ai); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pl_dj_join(IR_t **nodes, int n, int i, int q); }
extern "C" { __attribute__((visibility("hidden"))) int zd_pl_dj_gate_end(IR_t **nodes, int n, int i, int q); }
extern "C" { void zd_pl_bases(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, int *base, unsigned char *known, int *aof, int *zcut, int *fam, int *fbase, const unsigned char *adead); }
extern "C" { __attribute__((visibility("hidden"))) void zst_plan(const IR_graph_t * cg, IR_t ** nodes, int n); }
#endif

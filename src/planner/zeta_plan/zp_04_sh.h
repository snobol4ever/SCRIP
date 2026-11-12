#ifndef PLANNER_ZETA_PLAN_ZP_04_SH_H
#define PLANNER_ZETA_PLAN_ZP_04_SH_H
#include "planner/zeta_plan/zp_head.h"
extern "C" { void zd_rec_arm(const IR_graph_t * cg, IR_t ** nodes, int i); }
extern "C" { void zd_cp_arm(const IR_graph_t * cg, IR_t ** nodes, int i, const int * zcps, const int * zcpd, const int * zout); }
extern "C" { __attribute__((visibility("hidden"))) int zd_dj_anc(const int * zarm, int i, int q); }
extern "C" { __attribute__((visibility("hidden"))) int zd_match_env_record(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { void zd_plan(const IR_graph_t * cg, IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, int *zarm, const unsigned char *walls, int *zcps, int *zcpd, int *zchn, int *zclaim); }
#endif

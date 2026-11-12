#ifndef PLANNER_ZETA_PLAN_ZP_HEAD_H
#define PLANNER_ZETA_PLAN_ZP_HEAD_H
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
extern "C" {
#include "ir/ct_arena.h"
#include "ir/ct_vec.h"
#include "ir/ir_node.h"
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "optimizer/ir_index.h"
#include "ir/ir_node_index.h"
#include "optimizer/ir_query.h"
#include "ir/call_forms.h"
#include "planner/zeta_depth.h"
}
extern "C" {
#include "planner/g_planner.h"
#include "planner/zeta_plan.h"
#include "planner/act_words.h"
#include "planner/plan_facts.h"
#include "lower/lower_common.h"
enum { BCS_ALWAYS, BCS_CALL_CURMOV, BCS_ICN_LOCAL, BCS_ICN_SUSPEND, BCS_HAS_OPERANDS, BCS_NO_OPERANDS, BCS_PATV, BCS_HOME_FRAME, BCS_HOME_STATE, BCS_PL_HOME, BCS_PL_HOME_DYN, BCS_PL_HOME_RANGE_HDR,
    BCS_PL_HOME_HAS_OPERANDS, BCS_ICN_HOME, BCS_PL_BOUND_SPINE, BCS_PL_SPINE_RECORD, BCS_PL_GATE_ARM, BCS_PL_TO_TRAILED, BCS_PL_TO_DB, BCS_CALL_ARGV, BCS_CALL_ARGV_CUR, BCS_CALL_ARGV_REC,
        BCS_CALL_ARGV_CUR_REC, BCS_CALL_ARGV_RSP, BCS_RSP_FLAT, BCS_FLUSH_RSP_FLAT };
extern "C" int zls_g_home_frame(const IR_graph_t * g);
extern "C" int zls_home_why(const IR_t * nd);
extern "C" int planner_home_dyn(const IR_graph_t * g, const IR_t * nd);
extern "C" int rt_proc_is_registered(const char *); extern "C" int rt_proc_is_generator(const char *); extern "C" int rt_proc_dyn_scope(const char *); extern "C" int rt_proc_index_of(const char *);
extern "C" int rt_proc_pinned(const char *);
extern "C" int planner_gate_cells(const IR_graph_t * g, const IR_t * gate);
extern "C" int zls_off(const IR_t * nd);
extern "C" int zls_g_hf_resident(const IR_graph_t * g, const IR_t * nd);
typedef struct { IR_t * t; int d0; int nd; int multi; } zdw_ent_t;
typedef struct { const IR_t *k; int v; uint32_t gen; } zdo_slot_t;
typedef struct { const IR_t * nd; uint32_t gen; int maxk; } zrm_slot_t;
extern "C" void planner_mark_pass(const IR_graph_t * cg, unsigned char * ind);
extern "C" { void planner_pdef_lock(const IR_graph_t * g); }
extern "C" { void planner_pdef_unlock(void); }
struct zd_pdef_scope { int on; zd_pdef_scope(const IR_graph_t * g) : on(g != 0) { if (on) planner_pdef_lock(g); } ~zd_pdef_scope() { if (on) planner_pdef_unlock(); } };
extern "C" int zls_key_unbacked(const IR_graph_t * g, int off);
struct zst_dtree_t { std::vector<int> idom, depth, tin, tout, top; std::vector<unsigned char> univ; };
extern "C" int zls_key_unbacked(const IR_graph_t * g, int off);
struct zwk_blk_t { std::vector<uint64_t> bits; int top = 0; int first = INT_MAX; };
struct zwk_st_t { int b = -1; int len = 0; };
extern "C" int zls_retains_why(const IR_graph_t * g, const IR_t * nd);
extern "C" int zls_g_frame_region(const IR_graph_t * g);
extern "C" int zls_g_resident_bytes(const IR_graph_t * g);
extern "C" const unsigned char * zls_g_key_map(const IR_graph_t * g, int * n);
}
#endif

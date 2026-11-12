#include "planner/zeta_plan/zp_head.h"
extern "C" {
#include "ir/ct_arena.h"
}
extern "C" {
#include "ir/ct_vec.h"
}
extern "C" {
#include "ir/ir_graph.h"
}
extern "C" {
#include "ir/stage2.h"
}
extern "C" {
#include "ir/ir_node_index.h"
}
extern "C" {
#include "ir/call_forms.h"
}
extern "C" {
#include "planner/zeta_depth.h"
}
extern "C" {
#include "planner/g_planner.h"
}
extern "C" {
#include "planner/act_words.h"
}
extern "C" {
#include "lower/lower_common.h"
}
#include "planner/zeta_plan/zp_01_sh.h"
#include "planner/zeta_plan/zp_02_sh.h"
#include "planner/zeta_plan/zp_03_sh.h"
#include "planner/zeta_plan/zp_04_sh.h"
#include "planner/box_cell_shapes.inc"
extern "C" { int zst_coord(const char * name); }
extern "C" { int zd_edge_retains(const IR_t * nd); }
extern "C" { static int zd_gate_holds(const IR_t * g); }
extern "C" { int zd_nops(IR_t * nd); }
extern "C" { int zd_k_own(IR_t * nd); }
extern "C" { int zd_icn_suspend(const IR_t * nd); }
extern "C" { int zd_icn_local_var(const IR_t * nd); }
extern "C" { int zd_icn_local_assign_g(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { int zd_icn_local_assign(const IR_t * nd); }
extern "C" { const IR_t * zd_local_home_node(const IR_graph_t * cg, const IR_t * nd); }
extern "C" { int zd_icn_suspend(const IR_t * nd); }
extern "C" { int zd_nops(IR_t * nd); }
extern "C" { int zd_k_own(IR_t * nd); }
extern "C" { int zd_k_own(IR_t * nd); }
extern "C" { int fc_head_fp(const IR_t * nd); }
extern "C" {
g_planner_t g_planner;
int zd_dj_admitted(int n, int ei) { return (ei >= 0 && ei < n && (size_t)ei < g_planner.g_zd_admit.size() && g_planner.g_zd_admit[(size_t)ei]) ? 1 : 0; }
int zd_dj_cellless(IR_t ** nodes, int n, const int * zarm, int k) {
    return k >= 0 && nodes[k]->op == IR_DISJUNCTION && zarm && !zd_dj_admitted(n, k);
}
IR_t * zd_value_producer_g(const IR_graph_t * cg, IR_t * nd) {
    for (int g = 0; nd && g < 16 && zd_k_own_g(cg, nd) == 0 && (nd->op == IR_ASSIGN || nd->op == IR_CONJUNCTION) && nd->n_operands > 0; g++) nd = nd->operands[0];
    return nd;
}
IR_t * zd_conj_value_node_g(const IR_graph_t * cg, IR_t * nd) { int g = 0; while (nd && nd->op == IR_CONJUNCTION && nd->n_operands > 0 && zd_k_own_g(cg, nd) == 0 && g++ < 16) nd = nd->operands[0];
    return nd; }
int zd_plan_on_of(const IR_t * nd) { if (!nd || !g_planner.g_zd_plan_on || g_planner.g_zd_plan_nodes != g_planner.codegen_flat_chain_body_nodes) return 0;
    for (int k = 0; k < g_planner.g_zd_plan_n; k++) if (g_planner.g_zd_plan_nodes[k] == nd) return g_planner.g_zd_plan_on[k] ? 1 : 0; return 0; }
int zd_sr_role(const IR_t * nd) { return ir_define_ch8_role(nd); }
int zd_cpos(int i, const int *xb, const int *zout, long *d) {
    const int * cl = g_planner.g_zd_claim_p, * fm = g_planner.g_zd_fam_p, * fb = g_planner.g_zd_fbase_p;
    if (xb[i] != INT_MIN) { *d = xb[i] + zout[i]; return -2; }
    if (cl[i] >= 0 && fm[cl[i]] >= 0) { *d = fb[cl[i]] + zout[i]; return fm[cl[i]]; }
    *d = zout[i];
    return cl[i] >= 0 ? -3 - cl[i] : INT_MIN;
}
int zd_fam_delta(int i, int k, int * ok) {
    const int * cl = g_planner.g_zd_claim_p, * fm = g_planner.g_zd_fam_p, * fb = g_planner.g_zd_fbase_p;
    *ok = (cl && fm && fb && cl[i] >= 0 && cl[k] >= 0 && cl[i] != cl[k] && fm[cl[i]] >= 0 && fm[cl[i]] == fm[cl[k]]) ? 1 : 0;
    return *ok ? fb[cl[i]] - fb[cl[k]] : 0;
}
int zd_gate_pair_g(const IR_graph_t * cg, IR_t **nodes, int n, int a, int k, int wantb, const unsigned char *zon, const int *zout, const int *xb, long *rsp) {
    long da = 0, dk = 0;
    int ca = zd_cpos(a, xb, zout, &da), ck = INT_MIN;
    da -= g_planner.g_zd_gp_p[a];
    if (wantb && zd_edge_retains_g(cg, nodes[k])) {
        IR_t * c = zd_chase(nodes[k]->γ.node);
        int kc = c ? nidx(nodes, n, c) : -1;
        if (kc >= 0 && zon[kc]) { ck = zd_cpos(kc, xb, zout, &dk); dk -= zd_k_g(cg, nodes[kc]); }
    }
    else { ck = zd_cpos(k, xb, zout, &dk); if (!wantb) dk -= zd_k_g(cg, nodes[k]); }
    if (ca == INT_MIN || ca != ck) return 0;
    *rsp = da - dk;
    return 1;
}
int zd_gate_pair_at_g(const IR_graph_t * cg, IR_t **nodes, int n, int g, int a, int k, int wantb, const unsigned char *zon, const int *zout, const int *xb, long *rsp) {
    if (!zd_gate_pair_g(cg, nodes, n, a, k, wantb, zon, zout, xb, rsp)) return 0;
    IR_t * c = (g >= 0 && wantb && (nodes[k]->op == IR_DISJUNCTION || (int)ir_kind0(nodes[k]) == IR_CALL_PROC_STAGED)) ? zd_chase(nodes[g]->γ.node) : (IR_t *)0;
    int ci = c ? nidx(nodes, n, c) : -1;
    const int * cl = g_planner.g_zd_claim_p;
    if (ci >= 0 && ci != g && zon[ci] && zon[g] && cl && cl[ci] >= 0 && cl[ci] == cl[g] && cl[g] != cl[a]) *rsp += zout[g] - (zout[ci] - zd_k_g(cg, nodes[ci]));
    return 1;
}
int zd_cl_read_g(const IR_graph_t * cg, IR_t **nodes, int n, int i, const unsigned char *on, const int *out, const int *xb) {
    IR_t *nd = nodes[i];
    if ((int) IR_LIT(nd).ival != CLEANUP_OPEN || nd->n_operands < 2 || !nd->operands[1] || !on[i]) return -1;
    IR_t *p = nd->operands[1];
    IR_t *pv = zd_conj_value_node_g(cg, p);
    if (pv != p && zd_k_own_g(cg, pv) >= 16 && nidx(nodes, n, pv) >= 0 && on[nidx(nodes, n, pv)]) p = pv;
    int k = nidx(nodes, n, p);
    if (k < 0 || !on[k] || zd_k_own_g(cg, p) < 16) return -1;
    return out[i] - out[k] + ((xb && xb[i] != INT_MIN && xb[k] != INT_MIN) ? xb[i] - xb[k] : 0);
}
int zd_icn_local_var_g(const IR_graph_t * cg, const IR_t * nd) { return nd && nd->op == IR_VAR && cg && cg->spine_cells && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '&' &&
    (!is_global(IR_LIT(nd).sval) || graph_has_local(cg, IR_LIT(nd).sval)); }
int zd_icn_local_assign_g(const IR_graph_t * cg, const IR_t * nd) { return nd && nd->op == IR_ASSIGN && cg && cg->spine_cells && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '&' &&
    (!is_global(IR_LIT(nd).sval) || graph_has_local(cg, IR_LIT(nd).sval)); }
const IR_t * zd_local_home_node(const IR_graph_t * cg, const IR_t * nd) { int g = 0;
    while (nd && (nd->op == IR_CONJUNCTION || (nd->op == IR_ASSIGN && !zd_icn_local_assign_g(cg, nd))) && nd->n_operands > 0 && g++ < 16) nd = nd->operands[0];
    return (zd_icn_local_var_g(cg, nd) || zd_icn_local_assign_g(cg, nd)) ? nd : (const IR_t *)0; }
int zd_icn_suspend_g(const IR_graph_t * cg, const IR_t * nd) { return nd && nd->op == IR_SUSPEND && cg && cg->spine_cells; }
int zd_icn_stmt_head(const IR_graph_t * cg, const IR_t * nd) { extern const char * bb_src_of(const IR_t *); const char * s = (nd && cg && cg->spine_cells) ? bb_src_of(nd) : (const char *)0;
    return (s && (!strcmp(s, "stmt") || !strcmp(s, "nstmt"))) ? 1 : 0; }
int zd_icn_stmt_sibling(const IR_graph_t * cg, const IR_t * nd) { extern const char * bb_src_of(const IR_t *); const char * s = (nd && cg && cg->spine_cells) ? bb_src_of(nd) : (const char *)0;
    return (s && !strcmp(s, "stmt")) ? 1 : 0; }
int zd_djx_of(const IR_t * nd) { if (!nd || nd->op != IR_DISJUNCTION || g_planner.g_zd_djx_nodes != g_planner.codegen_flat_chain_body_nodes) return 0;
    int i = nidx(g_planner.g_zd_djx_nodes, (int)g_planner.g_zd_djx.size(), nd); return i >= 0 ? g_planner.g_zd_djx[(size_t)i] : 0; }
int zd_upost_of(const IR_t * nd) { if (!nd || nd->op != IR_UNMARK || g_planner.g_zd_djx_nodes != g_planner.codegen_flat_chain_body_nodes) return 0;
    int i = nidx(g_planner.g_zd_djx_nodes, (int)g_planner.g_zd_upost.size(), nd); return i >= 0 ? g_planner.g_zd_upost[(size_t)i] : 0; }
int zd_k_g(const IR_graph_t * cg, IR_t * nd) { return zd_k_own_g(cg, nd) + zst_s_of(nd) + zd_djx_of(nd) + zd_rel_g(cg, nd); }
int zd_kf_g(const IR_graph_t * cg, IR_t * nd) { return zd_k_g(cg, nd) - zd_rel_g(cg, nd); }
int zd_mframe_g(const IR_graph_t * cg, IR_t * nd) { return nd && nd->op == IR_MATCH_BEGIN ? zd_k_own_g(cg, nd) : 0; }
static int zd_dj_spine(const IR_t * nd) { if (!g_planner.g_zd_zarm_p || g_planner.g_zd_zarm_nodes != g_planner.codegen_flat_chain_body_nodes) return 0;
    int i = nidx(g_planner.g_zd_zarm_nodes, g_planner.g_zd_zarm_n, nd); return (i >= 0 && zd_dj_admitted(g_planner.g_zd_zarm_n, i) && !zls_dj_retains(nd)) ? 32 : 0; }
int zd_call_curmov(const IR_t * nd) { return lower_call_node_is_curmov(nd); }
extern "C" int zd_dj_on_spine(const IR_t * nd) { return zd_dj_spine(nd) ? 1 : 0; }
static int zd_call_argv_kind(const IR_t * nd) {
    switch (nd->op) {
    case IR_CALL_DIRECT: case IR_CALL_PL_LEAF: case IR_CALL_PAS_BI: case IR_CALL_CALLEE_REC: case IR_CALL_FIELD_REC: case IR_CALL_BYNAME: case IR_CALL_TRY: case IR_CALL_METHOD: case IR_CALL_REDISP:
    case IR_CALL_APPLY: case IR_CALL_EVAL: return 1;
    default: return 0; } }
int zd_pl_home(const IR_graph_t * cg) { return (zls_g_home_frame(cg) && zls_g_resumable_frame(cg)) ? 1 : 0; }
extern "C" int planner_call_spine_record(const IR_graph_t * cg, const IR_t * nd) {
    if (cg && nd && nd->op == IR_CALL_VALUE) return (zd_pl_home(cg) && IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "goal") && !zls_home_why(nd)) ? 1 : 0;
    if (!cg || !nd || ir_kind0(nd) != IR_CALL_PROC_STAGED || !zd_pl_home(cg) || !zd_edge_retains_g(cg, nd) || zls_home_why(nd) || getenv("SCRIP_PL_SPREC_OFF")) return 0;
    const char * fn = IR_LIT(nd).sval;
    if (!fn || !fn[0] || !rt_proc_is_registered(fn) || !rt_proc_is_generator(fn) || rt_proc_dyn_scope(fn) || rt_proc_index_of(fn) < 0) return 0;
    int i = bb_proc_find(fn), bi = i >= 0 ? g_stage2.proc_table[i].bb_idx : -1;
    const IR_graph_t * tg = (bi >= 0 && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (const IR_graph_t *)0;
    return tg ? (zls_g_resumable_frame(tg) ? 1 : 0) : (rt_proc_pinned(fn) ? 1 : 0);
}
static int zd_pl_home_dyn(const IR_graph_t * cg, IR_t * nd) { return zd_pl_home(cg) && (zls_home_why(nd) || zd_edge_retains_g(cg, nd) || planner_home_dyn(cg, nd)) ? 1 : 0; }
static int zd_bcs_when(const IR_graph_t * cg, IR_t * nd, int w) {
    if (w == BCS_ALWAYS) return 1;
    if (w == BCS_HOME_FRAME) return (zls_g_home_frame(cg) && !cg->resumable_callable) ? 1 : 0;
    if (w == BCS_ICN_HOME) return (zls_g_home_frame(cg) && !cg->resumable_callable) ? 1 : 0;
    if (w == BCS_HOME_STATE) return (zls_g_home_frame(cg) && zls_home_why(nd)) ? 1 : 0;
    if (w == BCS_CALL_CURMOV) return zd_call_curmov(nd);
    if (w == BCS_ICN_LOCAL) return zd_icn_local_var_g(cg, nd);
    if (w == BCS_ICN_SUSPEND) return zd_icn_suspend_g(cg, nd);
    if (w == BCS_HAS_OPERANDS) return nd->n_operands > 0;
    if (w == BCS_NO_OPERANDS) return nd->n_operands == 0;
    if (w == BCS_PL_HOME) return zd_pl_home(cg);
    if (w == BCS_PL_HOME_DYN) return zd_pl_home_dyn(cg, nd);
    if (w == BCS_PL_HOME_RANGE_HDR) return (zd_pl_home(cg) && nd->op == IR_TO && (ir_range_kind_is(nd, "db") || ir_range_trailed(nd))) ? 1 : 0;
    if (w == BCS_PL_HOME_HAS_OPERANDS) return (zd_pl_home(cg) && nd->n_operands > 0) ? 1 : 0;
    if (w == BCS_PL_BOUND_SPINE) return (zd_pl_home(cg) && planner_bound_spine(cg, nd)) ? 1 : 0;
    if (w == BCS_PL_SPINE_RECORD) return planner_call_spine_record(cg, nd);
    if (w == BCS_PL_TO_TRAILED) return (zd_pl_home(cg) && nd->op == IR_TO && !ir_range_kind_is(nd, "db") && ir_range_trailed(nd)) ? 1 : 0;
    if (w == BCS_PL_TO_DB) return (zd_pl_home(cg) && nd->op == IR_TO && ir_range_kind_is(nd, "db")) ? 1 : 0;
    if (w == BCS_PL_GATE_ARM) return (nd->n_operands > 1 && planner_gate_cells(cg, nd->operands[1])) ? 1 : 0;
    if (w == BCS_CALL_ARGV) return zd_call_argv_kind(nd);
    if (w == BCS_RSP_FLAT) return cg && !zls_g_home_frame(cg) && !zls_g_resumable_frame(cg) && !cg->spine_cells;
    if (w == BCS_FLUSH_RSP_FLAT) return cg && IR_LIT(nd).ival == SNO_FENCE_LIT_FLUSH && !zls_g_home_frame(cg) && !zls_g_resumable_frame(cg) && !cg->spine_cells;
    if (w == BCS_CALL_ARGV_RSP) return zd_call_argv_kind(nd) && cg && !zls_g_home_frame(cg) && !zls_g_resumable_frame(cg) && !cg->spine_cells;
    if (w == BCS_CALL_ARGV_CUR) return zd_call_argv_kind(nd) && zd_call_curmov(nd);
    if (w == BCS_CALL_ARGV_REC) return zd_call_argv_kind(nd) && lower_call_node_has_rec(nd);
    if (w == BCS_CALL_ARGV_CUR_REC) return zd_call_argv_kind(nd) && zd_call_curmov(nd) && lower_call_node_has_rec(nd);
    return (w == BCS_PATV && nd->pat_static && IR_LIT(nd).sval && !strncmp(IR_LIT(nd).sval, "PATV$", 5)) ? 1 : 0;
}
static int zd_bcs_words(const IR_graph_t * cg, const IR_t * nd, int w) {
    if (w == -1) return zd_dj_spine(nd);
    if (w == -2) return zd_dj_spine(nd) ? zd_dj_spine(nd) + 32 : 0;
    if (w == -3) return 16 * (2 + nd->n_operands);
    if (w == -4) return planner_fence_k(cg, nd);
    if (w == -5) return planner_alt_k(cg, nd);
    if (w == -6) return 16 * (1 + nd->n_operands);
    if (w == -7) return 16 * (2 + nd->n_operands);
    if (w == -8) return 16 * (1 + nd->n_operands) + 112;
    if (w == -9) return 16 * (2 + nd->n_operands) + 112;
    if (w == -10) return 56 + planner_act_extra(nd);
    return w;
}
static int zd_bcs_pick(const IR_graph_t * cg, IR_t * nd) {
    int op = (int)nd->op;
    for (size_t r = 0; r < sizeof box_cell_shapes / sizeof box_cell_shapes[0]; r++) {
        if ((box_cell_shapes[r].op >= 0 && box_cell_shapes[r].op != op) || !zd_bcs_when(cg, nd, box_cell_shapes[r].when)) continue;
        return (int)r;
    }
    return -1;
}
int zd_k_own_g(const IR_graph_t * cg, IR_t * nd) {
    int r = zd_bcs_pick(cg, nd);
    return r < 0 ? 16 : zd_bcs_words(cg, nd, box_cell_shapes[r].words);
}
int zd_rel_g(const IR_graph_t * cg, IR_t * nd) {
    for (size_t r = 0; r < sizeof box_low_regions / sizeof box_low_regions[0]; r++) {
        if ((box_low_regions[r].op >= 0 && box_low_regions[r].op != (int)nd->op) || !zd_bcs_when(cg, nd, box_low_regions[r].when)) continue;
        return box_low_regions[r].bytes;
    }
    return 0;
}
void zd_wk_own(const IR_graph_t * cg, IR_t * nd, std::vector<char> & w) {
    int r = zd_bcs_pick(cg, nd), nw = (r < 0 ? 16 : zd_bcs_words(cg, nd, box_cell_shapes[r].words)) / 8;
    w.assign((size_t)(nw > 0 ? nw : 0), 0);
    if (r < 0) { if (nw > 0) w[0] = 1; }
    else {
        const auto & b = box_cell_shapes[r];
        int fill_n = nw - b.wk_pre_n - b.wk_suf_n;
        for (int i = 0; i < b.wk_pre_n && i < nw; i++) if ((b.wk_pre >> i) & 1u) w[(size_t)i] = 1;
        if (b.wk_fill == 1) for (int i = 0; i < fill_n; i += 2) w[(size_t)(b.wk_pre_n + i)] = 1;
        if (b.wk_fill && fill_n >= 0) for (int i = 0; i < b.wk_suf_n; i++) if ((b.wk_suf >> i) & 1u) w[(size_t)(b.wk_pre_n + fill_n + i)] = 1;
    }
    int rw = zd_rel_g(cg, nd) / 8, ow = (int)w.size(), st = zst_s_of(nd) / 16, kw = zd_k_g(cg, nd) / 8;
    w.insert(w.begin(), (size_t)rw, 0);
    ow += rw;
    w.resize((size_t)(kw > 0 ? kw : 0), 0);
    for (int j = 0; j < st && ow + 2 * j < kw; j++) w[(size_t)(ow + 2 * j)] = 1;
}
int zd_arm_resumes(const IR_t * a, const IR_t * g) {
    if (!a || a->op != IR_GATE_ARM || !(int) a->seal || a->n_operands < 1) return 0;
    IR_t * t = zd_chase(a->operands[0]);
    return t && t != g && t->op != IR_SUCCEED && t->op != IR_FAIL && t->op != IR_UNMARK && t->op != IR_CUT;
}
static int zd_gate_holds(const IR_t * g) {
    if (!g || g->op != IR_GATE) return 0;
    for (int j = 0; j < g->n_operands; j++) if (zd_arm_resumes(g->operands[j], g)) return 1;
    return 0;
}
int planner_retains_g(const IR_graph_t * cg, const IR_t * nd) {
    if (!nd) return 0;
    int k0 = (int)ir_kind0(nd), op = (int)nd->op;
    if (k0 == IR_SUSPEND) return PR_SUSPEND;
    if (k0 == IR_CREATE || k0 == IR_ACTIVATE || k0 == IR_CORET || k0 == IR_COFAIL) return PR_COEXPR;
    if (k0 == IR_CALL_VALUE) return PR_UNKNOWN_CALLEE;
    if (k0 == IR_CALL_GEN || k0 == IR_PROC_GEN) return PR_GENERATOR_CALLEE;
    if (k0 == IR_MATCH_DEFER) return PR_DEFERRED_PATTERN;
    if (op == IR_GATE && zd_gate_holds(nd)) return PR_GATE_HOLDS;
    if (op == IR_DISJUNCTION && zd_pl_dj_holds(nd)) return PR_DISJUNCTION_HOLDS;
    if (op == IR_TO && ir_range_kind_is(nd, "db")) return PR_DB_GENERATOR;
    if (k0 == IR_CALL_PROC_STAGED && IR_LIT(nd).sval && bb_proc_target_retains(IR_LIT(nd).sval)) return PR_RETAINING_TARGET;
    if (zls_fcn_hret(nd)) return PR_RETAINING_CALLEE;
    if (zls_fcn_hunres(nd) && k0 == IR_CALL_PROC_STAGED && zls_g_resumable_frame(cg)) return PR_UNRESOLVED_CALLEE;
    return 0;
}
int zd_edge_retains_g(const IR_graph_t * cg, const IR_t * nd) {
    if (!nd || !planner_resumable_frame(cg)) return 0;
    int r = planner_retains_g(cg, nd);
    return (r == PR_UNKNOWN_CALLEE || r == PR_GATE_HOLDS || r == PR_DISJUNCTION_HOLDS || r == PR_DB_GENERATOR || r == PR_RETAINING_TARGET) ? 1 : 0;
}
extern "C" int planner_edge_retains(const IR_graph_t * g, const IR_t * nd) { int r = zd_edge_retains_g(g, nd); return r; }
int zd_bound_pop(const IR_graph_t * cg, IR_t * b) { return zd_k_own_g(cg, b); }
int planner_retains_emitter_view(int op) { return op == IR_PROC_GEN || op == IR_CALL_VALUE || op == IR_CALL_GEN || op == IR_ACTIVATE; }
extern "C" int planner_home_dyn(const IR_graph_t * g, const IR_t * nd) {
        int r = planner_retains_g(g, nd);
    if (r == PR_GATE_HOLDS || r == PR_DISJUNCTION_HOLDS || r == PR_DB_GENERATOR || r == PR_RETAINING_TARGET) {
        int u = zls_fcn_hunres(nd) && (int)ir_kind0(nd) == IR_CALL_PROC_STAGED && zls_g_resumable_frame(g);
        r = zls_fcn_hret(nd) ? PR_RETAINING_CALLEE : u ? PR_UNRESOLVED_CALLEE : 0;
    }
    return (r == PR_SUSPEND || r == PR_COEXPR || r == PR_UNKNOWN_CALLEE || r == PR_GENERATOR_CALLEE || r == PR_DEFERRED_PATTERN || r == PR_RETAINING_CALLEE || r == PR_UNRESOLVED_CALLEE) ? r : 0;
}
int zd_beta_via_act(const IR_graph_t * cg, const IR_t * nd) { return zd_edge_retains_g(cg, nd) && nd->op != IR_TO; }
int zd_pl_cut(const IR_graph_t * cg, const IR_t * nd) { return nd->op == IR_CUT && !IR_LIT(nd).ival && planner_resumable_frame(cg); }
int zd_pl_entry(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, int i, int e, int *l,
    const unsigned char *dead){
    int q, b;
    if (e == 0 && zd_edge_retains_g(cg, nodes[i])) return -1;
    if (e == 0 && zd_pl_dj_join(nodes, n, i, nidx(nodes, n, zd_chase(nodes[i]->γ.node)))) return -1;
    if (e >= 2 && nodes[i]->op == IR_DISJUNCTION && cg && cg->spine_cells && (((e - 2) & 1) || e - 2 >= 2 * (int)IR_LIT(nodes[i]).ival)) return -1;
    (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, i, e, &q, &b);
    if (q < 0 || claim[q] < 0 || claim[q] == claim[i] || b || (claim[q] != q && !(dead && dead[claim[q]]))) return -1;
    int v = (e == 1) ? ((nodes[i]->op == IR_BOUND && !zd_pl_bspine(cg, nodes[i]) && (planner_resumable_frame(cg) || (cg && cg->spine_cells))) ? zout[i] : zout[i] - zd_k_g(cg, nodes[i])) :
        (e >= 2 && nodes[i]->op == IR_DISJUNCTION) ? zout[i] - zd_djx_of(nodes[i]) :
        zout[i];
    if (e == 0 && nodes[i]->op == IR_UNMARK && nodes[i]->n_operands > 0 && zd_pl_rest_ival(cg, nodes[i]) && zd_pl_bspine(cg, nodes[i]->operands[0])) {
        int ub = nidx(nodes, n, nodes[i]->operands[0]);
        if (ub >= 0) v = (claim[ub] == claim[i]) ? zout[ub] - zd_bound_pop(cg, nodes[ub]) : zout[i] - zd_bound_pop(cg, nodes[ub]);
    }
    IR_t * r = (e == 0 && nodes[i]->op == IR_GATE_ARM && nodes[i]->n_operands > 0 && (int) nodes[i]->seal) ? nodes[i]->operands[0] : (IR_t *)0;
    int rq = r ? nidx(nodes, n, r) : -1;
    if (rq >= 0 && zon[rq] && claim[rq] == claim[i]) v = zout[rq];
    int hq = claim[q];
    *l = v - (zout[q] - zd_k_g(cg, nodes[q])) + (zout[hq] - zd_k_g(cg, nodes[hq]));
    return hq;
}
int zst_s_of(const IR_t * nd) { if (!nd || g_planner.zst_sm.empty()) return 0; auto it = g_planner.zst_sm.find(nd); return it == g_planner.zst_sm.end() ? 0 : it->second; }
int zst_names(const IR_t * nd, const char ** out) {
    int k = 0, o = (int)nd->op;
    if (o == IR_VAR || o == IR_VAR_REF || o == IR_ASSIGN || o == IR_ASSIGN_VAR || o == IR_REV_ASSIGN_VAR || o == IR_NULLTEST_VAR || o == IR_FIELD_VAR || o == IR_UNIFY_FIRST || o == IR_UNIFY_VALUE ||
        o == IR_REV_SWAP) out[k++] = IR_LIT(nd).sval;
    if (o == IR_REV_ASSIGN && nd->n_operands > 1 && nd->operands[1]) out[k++] = IR_LIT(nd->operands[1]).sval;
    if (o == IR_REV_SWAP && nd->n_operands > 0 && nd->operands[0]) out[k++] = IR_LIT(nd->operands[0]).sval;
    if ((o == IR_UNIFY_CONST || o == IR_UNIFY_STRUCT) && nd->n_operands > 0 && nd->operands[0] && nd->operands[0]->op == IR_VAR_REF) out[k++] = IR_LIT(nd->operands[0]).sval;
    if (o == IR_SCAN && nd->n_operands > 1 && nd->operands[1] && nd->operands[1]->op == IR_VAR && IR_LIT(nd->operands[1]).sval) out[k++] = IR_LIT(nd->operands[1]).sval;
    return k;
}
int zst_coord(const char * name) {
    if (!name || g_planner.g_zd_cur < 0 || g_planner.g_zd_cur >= g_planner.zst_n || g_planner.zst_nodes != g_planner.codegen_flat_chain_body_nodes) return -1;
    int r = g_planner.zst_root[(size_t)g_planner.g_zd_cur]; if (r < 0) return -1;
    if (!g_planner.zst_nmap_ok) { g_planner.zst_nmap.clear(); for (size_t t = 0; t < g_planner.zst_name.size(); t++) g_planner.zst_nmap[g_planner.zst_name[t]].push_back((int)t);
        g_planner.zst_nmap_ok = 1; }
    auto it = g_planner.zst_nmap.find(name);
    if (it == g_planner.zst_nmap.end()) return -1;
    for (size_t q = 0; q < it->second.size(); q++) { int t = it->second[q];
        if (g_planner.zst_vroot[(size_t)t] == r && g_planner.zst_L[(size_t)t] != INT_MIN) return ZST_BIAS - g_planner.zst_L[(size_t)t]; }
    return -1;
}
void zd_dj_discards(const IR_graph_t * cg, IR_t ** nodes, int n, const unsigned char * zon, const int * zout, const int * zgpop, const int * zwpop, const int * zarm, const int * claim,
    const int * base, const unsigned char * known, int on){
    g_planner.g_zd_grec.assign((size_t)(n > 0 ? n : 0), -1);
    g_planner.g_zd_wrec.assign((size_t)(n > 0 ? n : 0), -1);
    g_planner.g_zd_grec_pop.assign((size_t)(n > 0 ? n : 0), 0);
    g_planner.g_zd_wrec_pop.assign((size_t)(n > 0 ? n : 0), 0);
    if (!on || n <= 0 || !zarm) return;
    std::vector<int> recd((size_t)n * 2, INT_MAX);
    for (int d = 0; d < n; d++) {
        if (!zon[d] || !zls_dj_retains(nodes[d]) || zd_djx_of(nodes[d]) || !zd_dj_admitted(n, d) || claim[d] < 0 || !known[claim[d]]) continue;
        int zdab = base[claim[d]] + zout[d];
        std::vector<unsigned char> r0((size_t)n, 0), r1((size_t)n, 0);
        std::vector<int> wl;
        auto push = [&](std::vector<unsigned char> & rr, IR_t * x) { int k = x ? nidx(nodes, n, x) : -1; if (k >= 0 && k != d && !rr[(size_t)k]) { rr[(size_t)k] = 1; wl.push_back(k); } };
        auto walk = [&](std::vector<unsigned char> & rr) {
            while (!wl.empty()) {
                int k = wl.back();
                wl.pop_back();
                push(rr, zd_chase(nodes[k]->γ.node));
                push(rr, zd_chase(nodes[k]->ω.node));
                for (int a = 0; a < nodes[k]->n_operands; a++) push(rr, nodes[k]->operands[a]);
            }
        };
        push(r0, nodes[0]);
        for (int a = 0; cg && cg->alt_entry && a < cg->n_alts; a++) push(r0, zd_chase(cg->alt_entry[a]));
        walk(r0);
        push(r1, zd_chase(nodes[d]->γ.node));
        walk(r1);
        for (int k = 0; k < n; k++) {
            if (!r1[(size_t)k] || r0[(size_t)k] || !zon[k] || claim[k] < 0 || !known[claim[k]]) continue;
            for (int v = 0; v < 2; v++) {
                IR_t * t = zd_chase(v ? nodes[k]->ω.node : nodes[k]->γ.node);
                int q = t ? nidx(nodes, n, t) : -1;
                if (q == d || (q >= 0 && r1[(size_t)q] && !r0[(size_t)q])) continue;
                int ai = base[claim[k]] + zout[k];
                int land = v ? ai - zd_k_g(cg, nodes[k]) - zwpop[k] : ai - zgpop[k];
                if (land >= zdab) {
                    if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-DJREC] i=%d edge %d leaves the continuation of dj %d at %d, not below its %d\n", k, v, d, land, zdab);
                    continue;
                }
                if (zdab >= recd[(size_t)(2 * k + v)]) continue;
                recd[(size_t)(2 * k + v)] = zdab;
                (v ? g_planner.g_zd_wrec : g_planner.g_zd_grec)[(size_t)k] = d;
                (v ? g_planner.g_zd_wrec_pop : g_planner.g_zd_grec_pop)[(size_t)k] = zdab - land;
                if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-DJREC] i=%d %s edge leaves the continuation of dj %d: rsp from its record, then pop %d\n", k, v ? "omega" : "gamma", d, zdab - land);
            }
        }
    }
}
int zd_check_discard(const IR_t * t) { int o = t ? (int)t->op : -1;
    return o == IR_UNMARK || o == IR_BOUND || o == IR_MATCH_FENCE0 || o == IR_MATCH_FENCE1 || o == IR_CUT || o == IR_GATE || o == IR_GATE_ARM || o == IR_STATEMENT_BEGIN || o == IR_SUCCEED ||
        o == IR_FAIL; }
int zd_check_arm_env(int o) { return o == IR_GATE || o == IR_GATE_ARM || o == IR_DISJUNCTION || o == IR_MATCH_ARBNO || o == IR_MATCH_ALTERNATE; }
int zd_run_alpha_dead(const IR_graph_t * cg, IR_t ** nodes, int n, const int * claim, int h) {
    if (h <= 0 || h >= n || claim[h] != h || (cg && cg->n_alts > 0)) return 0;
    for (int i = 0; i < n; i++) {
        if (claim[i] == h) continue;
        for (int v = 0; v < 2; v++) {
            IR_t * t = zd_chase(v ? nodes[i]->ω.node : nodes[i]->γ.node);
            int q = t ? nidx(nodes, n, t) : -1;
            if (q >= 0 && claim[q] == h && !port_sz_beta(zd_edge_sz(nodes[i], v))) return 0;
        }
        int na = (nodes[i]->op == IR_DISJUNCTION) ? (int)IR_LIT(nodes[i]).ival : -1;
        for (int k = 0; k < nodes[i]->n_operands; k++) {
            int q = nodes[i]->operands[k] ? nidx(nodes, n, nodes[i]->operands[k]) : -1;
            if (q >= 0 && claim[q] == h && !(na >= 0 && (k >= 2 * na || (k & 1)))) return 0;
        }
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int planner_varslot(const IR_graph_t * cg, const char * name) { int c = zst_coord(name); if (c >= 0) return c; return ir_varslot_of(cg, name); }
static int * zd_place_ints(int n, int fill) { int * p = (int *)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(int)); for (int i = 0; i < n; i++) p[i] = fill; return p; }
static int zd_place_key(IR_t ** nodes, int n, int i, IR_t * nd, int own) {
    if (!nd) return -1;
    if (nd->op == IR_CONJUNCTION && nd->n_operands > 0 && nd->operands[0] && zls_off(nd->operands[0]) >= 0) { int k = nidx(nodes, n, nd);
        if (k >= 0 && (own ? k <= i : k < i)) return zls_off(nd->operands[0]); }
    return zls_off(nd);
}
static int zd_cross_edge(const IR_graph_t * cg, const IR_t * nd) {
    int c = planner_retains_g(cg, nd);
    if (c == PR_DEFERRED_PATTERN || c == PR_GENERATOR_CALLEE || c == PR_RETAINING_CALLEE || c == PR_UNRESOLVED_CALLEE) return c;
    return (c >= 3 && zd_edge_retains_g(cg, nd)) ? c : 0;
}
static int zd_cross_mark(IR_t ** nodes, int n, int x) {
    if (nodes[x]->op == IR_UNMARK) return nodes[x]->n_operands > 0 ? nidx(nodes, n, nodes[x]->operands[0]) : -1;
    int xc = 0;
    for (int z = x - 1; z >= 0; z--) {
        if (nodes[z]->op == IR_MATCH_END) xc++;
        else if (nodes[z]->op == IR_MATCH_BEGIN) { if (xc == 0) return z; xc--; }
    }
    return -1;
}
static int zd_cross_join(int o) { return o == IR_MATCH_ALTERNATE || o == IR_DISJUNCTION || o == IR_GATE || o == IR_GATE_ARM; }
static int zd_cross_code(const IR_graph_t * cg, IR_t ** nodes, int n, int k, int i, std::vector<unsigned> & mk) {
    const size_t jo = (size_t)n + 1;
    for (int x = k; x <= i; x++) { mk[(size_t)x] = 0u; mk[jo + (size_t)x] = 0u; }
    mk[(size_t)k] = 1u;
    for (int pass = 0, ch = 1; ch && pass < 8; pass++) {
        ch = 0;
        for (int x = k; x < i; x++)
            for (int w = 0; w < 2; w++) {
                unsigned out = mk[(size_t)x + (w ? jo : 0)];
                if (!out) continue;
                if (x > k) {
                    int o = (int)nodes[x]->op, c = zd_cross_edge(cg, nodes[x]);
                    if ((o == IR_UNMARK && (zd_pl_rest_ival(cg, nodes[x]) || zd_mark_restore(nodes[x]))) || o == IR_MATCH_END) { int m = zd_cross_mark(nodes, n, x);
                        out = (m >= k && m < x) ? mk[(size_t)m] : 1u; }
                    else if (o == IR_STATEMENT_BEGIN || zd_pl_cut(cg, nodes[x])) out = 1u;
                    if (c) out |= 1u << c;
                }
                IR_t * sc[1 + nodes[x]->n_operands]; int ns = 0, o2 = (int)nodes[x]->op;
                sc[ns++] = nodes[x]->γ.node;
                if (!w && zd_check_arm_env(o2)) {
                    int na = (o2 == IR_DISJUNCTION) ? (int)IR_LIT(nodes[x]).ival : -1;
                    for (int q = 0; q < nodes[x]->n_operands; q++) if (na < 0 || (q < 2 * na && !(q & 1))) sc[ns++] = nodes[x]->operands[q];
                }
                for (int s = 0; s < ns; s++) {
                    IR_t * t = sc[s] ? zd_chase(sc[s]) : (IR_t *)0;
                    int y = t ? nidx(nodes, n, t) : -1;
                    if (y <= k || y > i) continue;
                    size_t at = (size_t)y + ((y <= x && zd_cross_join((int)nodes[y]->op)) ? jo : 0);
                    if ((mk[at] | out) != mk[at]) { mk[at] |= out; ch = 1; }
                }
            }
    }
    unsigned fin = mk[(size_t)i] | mk[jo + (size_t)i];
    for (int c = 3; c <= 11; c++) if (fin & (1u << c)) return c;
    return 0;
}
static uint64_t zwk_word(const zwk_blk_t & k, size_t q) { return q < k.bits.size() ? k.bits[q] : 0ull; }
static uint64_t zwk_view_word(const std::vector<zwk_blk_t> & bl, const zwk_st_t & s, size_t q) {
    if (s.b < 0 || (int)(q * 64) >= s.len) return 0ull;
    uint64_t v = zwk_word(bl[(size_t)s.b], q);
    return (int)(q * 64 + 64) > s.len ? v & ((1ull << (s.len - (int)(q * 64))) - 1ull) : v;
}
static int zwk_has_bits(const std::vector<zwk_blk_t> & bl, int b, int from, int to) {
    if (b < 0 || from >= to || bl[(size_t)b].first >= to) return 0;
    for (int q = from / 64; q * 64 < to; q++) {
        uint64_t v = zwk_word(bl[(size_t)b], (size_t)q);
        if (q * 64 < from) v &= ~((1ull << (from - q * 64)) - 1ull);
        if (q * 64 + 64 > to) v &= (1ull << (to - q * 64)) - 1ull;
        if (v) return 1;
    }
    return 0;
}
static int zwk_subset(const std::vector<zwk_blk_t> & bl, const zwk_st_t & a, const zwk_st_t & c) {
    for (size_t q = 0; (int)(q * 64) < a.len; q++) if (zwk_view_word(bl, a, q) & ~zwk_view_word(bl, c, q)) return 0;
    return 1;
}
static zwk_st_t zwk_norm(const std::vector<zwk_blk_t> & bl, zwk_st_t s) {
    if (s.b < 0 || bl[(size_t)s.b].first >= s.len) return zwk_st_t();
    if (s.len > bl[(size_t)s.b].top) s.len = bl[(size_t)s.b].top;
    return s;
}
static int zwk_fork(std::vector<zwk_blk_t> & bl, const zwk_st_t & s) {
    zwk_blk_t k;
    if (s.b >= 0) {
        k.bits.assign((size_t)((s.len + 63) / 64), 0ull);
        for (size_t q = 0; q < k.bits.size(); q++) k.bits[q] = zwk_view_word(bl, s, q);
        k.top = s.len;
        k.first = bl[(size_t)s.b].first < s.len ? bl[(size_t)s.b].first : INT_MAX;
    }
    bl.push_back(k);
    return (int)bl.size() - 1;
}
static void zwk_set(zwk_blk_t & k, int w) {
    if ((size_t)(w / 64) >= k.bits.size()) k.bits.resize((size_t)(w / 64) + 1, 0ull);
    k.bits[(size_t)w / 64] |= 1ull << (w % 64);
    if (w + 1 > k.top) k.top = w + 1;
    if (w < k.first) k.first = w;
}
static zwk_st_t zwk_extend(std::vector<zwk_blk_t> & bl, zwk_st_t s, const std::vector<int> & ws) {
    if (ws.empty()) return s;
    int mn = ws[0];
    for (size_t q = 1; q < ws.size(); q++) if (ws[q] < mn) mn = ws[q];
    int b = s.b;
    if (b < 0 || s.len != bl[(size_t)b].top || mn < s.len) b = zwk_fork(bl, s);
    for (size_t q = 0; q < ws.size(); q++) zwk_set(bl[(size_t)b], ws[q]);
    zwk_st_t r; r.b = b; r.len = bl[(size_t)b].top;
    return r;
}
static int zwk_join(std::vector<zwk_blk_t> & bl, zwk_st_t & into, const zwk_st_t & add) {
    if (add.b < 0 || add.len <= 0) return 0;
    if (into.b < 0) { into = add; return 1; }
    if (into.b == add.b) {
        if (add.len <= into.len || !zwk_has_bits(bl, add.b, into.len, add.len)) return 0;
        into.len = add.len;
        return 1;
    }
    if (zwk_subset(bl, add, into)) return 0;
    if (zwk_subset(bl, into, add)) { into = add; return 1; }
    zwk_blk_t k;
    int len = into.len > add.len ? into.len : add.len;
    k.bits.assign((size_t)((len + 63) / 64), 0ull);
    for (size_t q = 0; q < k.bits.size(); q++) k.bits[q] = zwk_view_word(bl, into, q) | zwk_view_word(bl, add, q);
    k.top = len;
    int f1 = bl[(size_t)into.b].first < into.len ? bl[(size_t)into.b].first : INT_MAX, f2 = bl[(size_t)add.b].first < add.len ? bl[(size_t)add.b].first : INT_MAX;
    k.first = f1 < f2 ? f1 : f2;
    bl.push_back(k);
    into.b = (int)bl.size() - 1;
    into.len = len;
    return 1;
}
void zd_wk_fold(zeta_plan_t * rec, IR_t ** nodes, int n, const IR_graph_t * g) {
    rec->v_wk.clear();
    std::vector<int> E((size_t)n, -1), X((size_t)n, -1);
    std::vector<zwk_blk_t> bl;
    std::vector<zwk_st_t> in((size_t)n), ext((size_t)n), extin((size_t)n);
    std::vector<unsigned char> extok((size_t)n, 0);
    for (int i = 0; i < n; i++) {
        znode_t * z = &rec->zn[i];
        z->wkoff = -1; z->wkn = 0; z->wke = 0;
        if (!rec->on[i] || !rec->xb || rec->xb[i] == INT_MIN || rec->out[i] < 0) continue;
        int e = z->zrunb + z->xbp, x = e + z->k;
        if (z->k < 0 || (e & 7) || (x & 7) || e < 0) continue;
        E[(size_t)i] = e; X[(size_t)i] = x;
    }
    std::vector<char> own;
    std::vector<int> ws;
    for (int pass = 0, ch = 1; ch && pass < 4; pass++) {
        ch = 0;
        for (int j = 0; j < n; j++) {
            if (E[(size_t)j] < 0) continue;
            if (!extok[(size_t)j] || in[(size_t)j].b != extin[(size_t)j].b || in[(size_t)j].len != extin[(size_t)j].len) {
                zd_wk_own(g, nodes[j], own);
                ws.clear();
                for (size_t c = 0; c < own.size(); c++) if (own[c]) { int w = X[(size_t)j] / 8 - 1 - (int)c; if (w >= 0) ws.push_back(w); }
                ext[(size_t)j] = zwk_extend(bl, in[(size_t)j], ws);
                extin[(size_t)j] = in[(size_t)j];
                extok[(size_t)j] = 1;
            }
            const zwk_st_t st = ext[(size_t)j];
            IR_t * sc[1 + nodes[j]->n_operands]; int ns = 0, o2 = (int)nodes[j]->op;
            sc[ns++] = nodes[j]->γ.node;
            if (zd_check_arm_env(o2)) {
                int na = (o2 == IR_DISJUNCTION) ? (int)IR_LIT(nodes[j]).ival : -1;
                for (int q = 0; q < nodes[j]->n_operands; q++) if (na < 0 || (q < 2 * na && !(q & 1))) sc[ns++] = nodes[j]->operands[q];
            }
            for (int s = 0; s < ns; s++) {
                IR_t * t = sc[s] ? zd_chase(sc[s]) : (IR_t *)0;
                int y = t ? nidx(nodes, n, t) : -1;
                if (y < 0 || E[(size_t)y] < 0) continue;
                int lim = E[(size_t)y] / 8 < X[(size_t)j] / 8 ? E[(size_t)y] / 8 : X[(size_t)j] / 8;
                zwk_st_t add = st;
                if (add.len > lim) add.len = lim;
                add = zwk_norm(bl, add);
                if (zwk_join(bl, in[(size_t)y], add)) ch = 1;
            }
            IR_t * wt = nodes[j]->ω.node ? zd_chase(nodes[j]->ω.node) : (IR_t *)0;
            int wy = wt ? nidx(nodes, n, wt) : -1;
            if (wy >= 0 && E[(size_t)wy] >= 0) {
                int lim = E[(size_t)wy] / 8 < E[(size_t)j] / 8 ? E[(size_t)wy] / 8 : E[(size_t)j] / 8;
                zwk_st_t add = in[(size_t)j];
                if (add.len > lim) add.len = lim;
                add = zwk_norm(bl, add);
                if (zwk_join(bl, in[(size_t)wy], add)) ch = 1;
            }
        }
    }
    std::vector<int> bofs(bl.size(), -1);
    for (int i = 0; i < n; i++) {
        if (E[(size_t)i] < 0) continue;
        const zwk_st_t s = zwk_norm(bl, in[(size_t)i]);
        if (s.b < 0) continue;
        znode_t * z = &rec->zn[i];
        if (bofs[(size_t)s.b] < 0) {
            const zwk_blk_t & k = bl[(size_t)s.b];
            rec->v_wk.push_back((uint64_t)k.bits.size());
            bofs[(size_t)s.b] = (int)rec->v_wk.size();
            rec->v_wk.insert(rec->v_wk.end(), k.bits.begin(), k.bits.end());
        }
        z->wkoff = bofs[(size_t)s.b]; z->wkn = s.len; z->wke = E[(size_t)i];
    }
}
static int zd_callee_frame_retained(const IR_graph_t * cg, const IR_t * nd) { int r = planner_retains_g(cg, nd);
    return r == PR_RETAINING_TARGET || r == PR_GENERATOR_CALLEE || r == PR_RETAINING_CALLEE; }
void zd_place(const IR_graph_t * cg, zeta_plan_t * rec, IR_t ** nodes, int n, const unsigned char * zon, const int * zout, const int * zarm, const int * zcps, const int * zcpd) {
    const IR_graph_t * g = cg;
    rec->home = zd_place_ints(n, -1); rec->zeta = (unsigned char *)ct_zalloc((size_t)n, 1); rec->lkey = zd_place_ints(n, -1); rec->alias = zd_place_ints(n, -1); rec->under = zd_place_ints(n, 0);
        rec->slid = zd_place_ints(n, 0);
    rec->pat = zd_place_ints(n, 0); rec->fc = zd_place_ints(n, 0); rec->tail = zd_place_ints(n, 0); rec->ret = zd_place_ints(n, 0); rec->grec_off = zd_place_ints(n, -1);
        rec->wrec_off = zd_place_ints(n, -1);
    rec->grec_res = zd_place_ints(n, 0); rec->wrec_res = zd_place_ints(n, 0); rec->cpa_s = zd_place_ints(n, -1); rec->cpa_d = zd_place_ints(n, -1); rec->cpa_sx = zd_place_ints(n, -1);
    rec->cpa_dx = zd_place_ints(n, -1); rec->rdo = zd_place_ints(n + 1, 0);
    int total = 0;
    for (int i = 0; i < n; i++) { rec->rdo[i] = total; total += nodes[i]->n_operands + 1; }
    rec->rdo[n] = total; rec->rdn = total; rec->rd = (zrd_t *)ct_zalloc((size_t)(total > 0 ? total : 1), sizeof(zrd_t));
    for (int q = 0; q < total; q++) { rec->rd[q].dist = 0; rec->rd[q].xf = -1; rec->rd[q].fr = -1; rec->rd[q].key = -1; rec->rd[q].kind = -1; rec->rd[q].anchor = -1; rec->rd[q].kown = -1;
        rec->rd[q].fres = 1; }
    planner_consumers_build(g);
    int cur_saved = g_planner.g_zd_cur, slid_seen = 0;
    int cr_r = 0, cr_f = 0, cr_s = 0, cr_sc[16] = {0}; const char * cr_e = getenv("SCRIP_ZD_CHECK"); const int cr_on = (cr_e && *cr_e == '1') ? 1 : 0;
    std::vector<unsigned> cr_mk(cr_on ? 2 * ((size_t)n + 1) : 0, 0u);
    for (int i = 0; i < n; i++) {
        IR_t * nd = nodes[i]; zrd_t * rd = rec->rd + rec->rdo[i]; int no = nd->n_operands;
        g_planner.g_zd_cur = i;
        rec->home[i] = planner_frame_slot(g, nd);
        { int K = zd_k_g(cg, nd), off = zd_place_key(nodes, n, i, nd, 1); if (zls_key_unbacked(g, off)) off = -1; rec->lkey[i] = off;
            int esc = (nd->op == IR_TABLE_HDR && off >= 0) ? 1 : 0, xf = rec->home[i];
            rec->zeta[i] =
                (unsigned char)(esc ? ZETA_ACT : zon[i] ? (K > 0 ? (xf != -1 ? ZETA_SPINE_ACT : ZETA_SPINE) : (xf != -1 ? ZETA_ACT : ZETA_NONE)) : off < 0 ? ZETA_NONE : xf != -1 ? ZETA_ACT : ZETA_PRE)
                ; }
        rec->alias[i] = (zd_k_own_g(cg, nd) == 16 && !zd_icn_suspend_g(cg, nd)) ? zls_off(nd) : -1;
        rec->ret[i] = planner_retains_g(cg, nd);
        rec->slid[i] = slid_seen; if (cg->spine_cells && zd_callee_frame_retained(cg, nd)) slid_seen = 1;
        if (ir_is_matcher_element((int)nd->op))
            for (int j = 0; j < no; j++) {
                IR_t * pa = nd->operands[j];
                if (!pa || ir_is_matcher((int)pa->op)) continue;
                for (int ka = 0; ka < n; ka++) {
                    if (nodes[ka] != pa) continue;
                    if (ka >= i) break;
                    int mb1 = -1, nmb = 0;
                    for (int z2 = i - 1; z2 > ka; z2--) if (nodes[z2]->op == IR_MATCH_BEGIN) { nmb++; mb1 = z2; }
                    if (nmb == 1 && mb1 > ka) { int xk = 0; for (int z3 = mb1 - 1; z3 > ka; z3--) if (nodes[z3]->op == IR_MATCH_DEFER) xk += 16;
                        rd[j].anchor = 8 + (zout[mb1] - zd_mframe_g(cg, nodes[mb1]) - zout[ka]) + xk; }
                    break;
                }
            }
        { int xn = zd_nops_g(cg, nd);
            for (int j = 0; j < xn && j < no; j++) { rd[j].xf = planner_frame_slot(g, nd->operands[j]);
                if (rd[j].xf == -1 && zd_value_producer_g(cg, nd->operands[j]) != nd->operands[j]) rd[j].xf = planner_frame_slot(g, zd_value_producer_g(cg, nd->operands[j])); }
            if (zd_scan_subject(nd) && xn == no) rd[xn].xf = planner_frame_slot(g, zd_scan_subject(nd));
            for (int j = 0; j <= no; j++) if (rd[j].xf == -1 && rd[j].anchor != -1) rd[j].xf = rd[j].anchor; }
        zd_rec_arm(cg, nodes, i);
        rec->grec_off[i] = g_planner.g_zd_grec_off; rec->grec_res[i] = g_planner.g_zd_grec_off != -1 ? g_planner.g_zd_grec_res : 0; rec->wrec_off[i] = g_planner.g_zd_wrec_off;
        rec->wrec_res[i] = g_planner.g_zd_wrec_off != -1 ? g_planner.g_zd_wrec_res : 0;
        g_planner.g_zd_cps = zon[i] ? zcps[i] : -1; g_planner.g_zd_cpd = zon[i] ? zcpd[i] : -1; g_planner.g_zd_cpsx = -1; g_planner.g_zd_cpdx = -1;
        if (zon[i]) zd_cp_arm(cg, nodes, i, zcps, zcpd, zout);
        rec->cpa_s[i] = g_planner.g_zd_cps; rec->cpa_d[i] = g_planner.g_zd_cpd; rec->cpa_sx[i] = g_planner.g_zd_cpsx; rec->cpa_dx[i] = g_planner.g_zd_cpdx;
        if (zon[i] && rec->cpa_s[i] == 0 && rec->cpa_d[i] >= 0 && rec->cpa_sx[i] == -1) { IR_t * vp = zd_value_producer_g(cg, nd);
            if (vp && planner_frame_slot(g, vp) != -1) rec->cpa_sx[i] = planner_frame_slot(g, vp); }
        if (!zon[i]) continue;
        if (nd->op == IR_MATCH_REPLACE) {
            int zu = 0;
            for (int zj = i - 1; zj >= 0; zj--) { if (nodes[zj]->op == IR_MATCH_END) break; if (zon[zj]) zu += zd_kf_g(cg, nodes[zj]); }
            rec->under[i] = zu;
            int zp = 0, inpat = 0;
            for (int zj = i - 1; zj >= 0; zj--) {
                if (nodes[zj]->op == IR_MATCH_BEGIN) break;
                if (nodes[zj]->op == IR_MATCH_END) { inpat = 1; continue; }
                if (inpat && zon[zj] && nodes[zj]->op >= IR_MATCH && nodes[zj]->op <= IR_MATCH_VALUE) zp += zd_kf_g(cg, nodes[zj]) - planner_fence0_release(cg, nodes[zj]);
            }
            rec->pat[i] = zp;
            IR_t * mb = (IR_t *)0;
            for (int zj = i - 1; zj >= 0; zj--) if (nodes[zj]->op == IR_MATCH_BEGIN) { mb = nodes[zj]; break; }
            int fp = mb ? fc_head_fp(mb) : -1;
            rec->fc[i] = fp >= 0 ? fp : 0;
        }
        if (nd->op == IR_TO || nd->op == IR_TO_BY) { int zt = 0; for (int q = i + 1; q < n; q++) if (zon[q]) zt += zd_kf_g(cg, nodes[q]); rec->tail[i] = zt; }
        { int nq = zd_nops_g(cg, nd), op = (int)nd->op, cap = (op == IR_MATCH_ASSIGN_COND || op == IR_MATCH_ASSIGN_IMM) ? 1 : 0;
            IR_t * xs = zd_scan_subject(nd); int nx = (xs && nq == no) ? 1 : 0;
            for (int j = 0; j < nq + nx && j < no + nx; j++) {
                if (cap && j == 0) continue;
                IR_t * p = (j < no) ? nd->operands[j] : xs;
                { IR_t * pv = zd_conj_value_node_g(cg, p); int kv = pv != p ? nidx(nodes, n, pv) : -1; if (pv != p && zd_k_own_g(cg, pv) >= 16 && kv >= 0 && zon[kv]) p = pv; }
                int k = nidx(nodes, n, p);
                if (k < 0) continue;
                int kd = k;
                if (p->op == IR_ASSIGN && zd_k_own_g(cg, p) == 0 && !zd_icn_local_assign_g(cg, p)) { IR_t * pa = zd_value_producer_g(cg, p); int ka = (pa && pa != p) ? nidx(nodes, n, pa) : -1; if (ka >= 0 && ka < k && zon[ka] && zd_k_own_g(cg, pa) >= 16) kd = ka; }
                rd[j].dist = planner_read_distance(cg, nodes, n, zarm, i, kd, zout, rec->xb);
                if (nd->op == IR_MATCH_BEGIN) rd[j].dist -= zd_mframe_g(cg, nd);
                if (rd[j].xf == -1 && zd_pl_home(cg) && rec->xb && zon[k] && rec->xb[k] != INT_MIN && nd->op == IR_UNMARK && p->op == IR_BOUND && planner_bound_spine(cg, p))
                    rd[j].xf = -(rec->xb[k] + zout[k]);
                if (rd[j].xf == -1 && zd_pl_home(cg) && rec->xb && rec->claim && zon[k] && rec->claim[i] != rec->claim[k] && rec->xb[k] != INT_MIN && rec->xb[i] == INT_MIN) { int fok = 0;
                    zd_fam_delta(i, k, &fok); if (!fok) rd[j].xf = -(rec->xb[k] + zout[k]); }
                if (rd[j].xf == -1 && zd_pl_home(cg) && rec->xb && rec->claim && zon[k] && rec->claim[i] != rec->claim[k] && (rec->xb[k] == INT_MIN || rec->xb[i] == INT_MIN) &&
                    zd_k_own_g(cg, p) >= 16 && !zd_local_home_node(cg, p) && getenv("SCRIP_PL_ALIAS_DIAG")) { int fok = 0; zd_fam_delta(i, k, &fok);
                    if (!fok) fprintf(stderr, "[PL-ALIAS-MISS] graph=%s i=%d %s reads k=%d %s: no rsp distance, no alias, no frame home\n", zls_graph_name_get(cg), i, bb_op_name(nd->op), k,
                        bb_op_name(p->op)); }
                if (g_planner.zd_check__on > 0 && rec->xb && rec->claim && rec->claim[i] != rec->claim[k] && (rec->xb[i] == INT_MIN || rec->xb[k] == INT_MIN) && planner_frame_slot(g, p) == -1 &&
                    (zd_k_own_g(cg, p) >= 16 || zd_local_home_node(cg, p)) && !zd_run_alpha_dead(cg, nodes, n, rec->claim, rec->claim[i])) { int fok = 0; zd_fam_delta(i, k, &fok);
                    if (!fok) fprintf(stderr, "[ZD-CHECK]   READ i=%d %s reads p=%d %s across runs with an unknown base\n", i, bb_op_name(nd->op), k, bb_op_name(p->op)); }
                { const IR_t * lh = zd_local_home_node(cg, p); if (lh) rd[j].fr = planner_varslot(cg, IR_LIT(lh).sval);
                    else if (zd_dj_cellless(nodes, n, zarm, k) && zls_off(p) >= 0) rd[j].fr = zls_off(p); }
                rd[j].kind = (int)ir_norm_call_kind(p->op);
                if (cr_on) { int rc = 0;
                    if (k < i) rc = zd_cross_code(cg, nodes, n, k, i, cr_mk);
                    else for (int x = i + 1; x < k && !rc; x++) rc = zd_cross_edge(cg, nodes[x]);
                    if (rc) { cr_r++; if (rd[j].xf != -1 || rd[j].fr >= 0 || !zon[k] || planner_frame_slot(g, p) != -1 || planner_act_slot(p) != -1) cr_f++;
                        else { cr_s++; cr_sc[rc & 15]++; fprintf(stderr, "[ZD-CHECK]   CROSS-RSP i=%d %s reads k=%d %s code=%d\n", i, bb_op_name(nd->op), k, bb_op_name(p->op), rc); } } }
            }
            if (nd->op == IR_MATCH_REPLACE) { rd[2].dist = -1; rd[2].kind = -1; IR_t * mb2 = (IR_t *)0; int jh2 = -1;
                for (int zj = i - 1; zj >= 0; zj--) if (nodes[zj]->op == IR_MATCH_BEGIN) { mb2 = nodes[zj]; jh2 = zj; break; }
                IR_t * sp = (mb2 && mb2->n_operands > 0) ? mb2->operands[0] : (IR_t *)0;
                if (sp && jh2 >= 0) for (int js = jh2 - 1; js >= 0; js--) if (nodes[js] == sp) { if (zon[js]) { rd[2].dist = rec->under[i] + (zout[jh2] - zd_mframe_g(cg, nodes[jh2]) - zout[js]);
                    rd[2].kind = (int)sp->op; } break;
                    } }
            int ns = nq < no ? nq : no;
            for (int j = 0; j < ns; j++) { IR_t * po = nd->operands[j], * pv = zd_value_producer_g(cg, po); int ko = po ? nidx(nodes, n, po) : -1, kp = pv ? nidx(nodes, n, pv) : -1;
                int ph = (po && pv != po && planner_frame_slot(g, pv) != -1) ? 1 : 0, onp = (kp >= 0 && zon[kp]) ? 1 : 0, ono = (ko >= 0 && zon[ko]) ? 1 : 0;
                rd[j].key = (po && (zd_k_own_g(cg, po) >= 16 || zd_local_home_node(cg, po) || (pv != po && zd_k_own_g(cg, pv) >= 16 && onp) || ph) && ono) ? zd_place_key(nodes, n, i, po, 0) : -1; }
            if (xs && ns == no) { int kx = nidx(nodes, n, xs); rd[ns].key = ((zd_k_own_g(cg, xs) >= 16 || zd_local_home_node(cg, xs)) && kx >= 0 && zon[kx]) ? zls_off(xs) : -1; } }
    }
    g_planner.g_zd_grec_off = -1; g_planner.g_zd_wrec_off = -1; g_planner.g_zd_grec_res = 0; g_planner.g_zd_wrec_res = 0; g_planner.g_zd_cpsx = -1; g_planner.g_zd_cpdx = -1; g_planner.g_zd_cps = -1;
    g_planner.g_zd_cpd = -1; g_planner.g_zd_cur = cur_saved;
    if (cr_on) { fprintf(stderr, "[ZD-CHECK] CROSS graph=%s reads=%d via_frame=%d via_rsp=%d rsp_codes=", zls_graph_name_get(cg), cr_r, cr_f, cr_s);
        for (int c = 1; c < 16; c++) if (cr_sc[c]) fprintf(stderr, "%d:%d,", c, cr_sc[c]);
        fprintf(stderr, "\n"); }
}
void planner_capture(zeta_plan_t * rec) {
    int n = rec->n;
    rec->nodes_own = (IR_t **)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(IR_t *)); memcpy(rec->nodes_own, g_planner.codegen_flat_chain_body_nodes, sizeof(IR_t *) * (size_t)n);
    rec->v_zst_s = g_planner.zst_s; rec->v_zst_root = g_planner.zst_root; rec->v_zst_vroot = g_planner.zst_vroot; rec->v_zst_c = g_planner.zst_c; rec->v_zst_j = g_planner.zst_j;
        rec->v_zst_L = g_planner.zst_L;
    rec->v_zst_vuse = g_planner.zst_vuse; rec->v_zst_vcov = g_planner.zst_vcov; rec->v_zst_name = g_planner.zst_name; rec->v_zst_sm = g_planner.zst_sm; rec->zst_n_saved = g_planner.zst_n;
    rec->v_djx = g_planner.g_zd_djx; rec->v_upost = g_planner.g_zd_upost; rec->v_grec = g_planner.g_zd_grec; rec->v_wrec = g_planner.g_zd_wrec; rec->v_grec_pop = g_planner.g_zd_grec_pop;
    rec->v_wrec_pop = g_planner.g_zd_wrec_pop; rec->v_admit = g_planner.g_zd_admit;
    rec->zarm_published = (g_planner.g_zd_zarm_p == rec->arm) ? 1 : 0; rec->djx_nodes_published = (g_planner.g_zd_djx_nodes == g_planner.codegen_flat_chain_body_nodes) ? 1 : 0;
    rec->zrm_len = g_planner.g_zrm_map.len; rec->zrm_cap = g_planner.g_zrm_map.cap; rec->zrm_esz = g_planner.g_zrm_map.esz; rec->zrm_gen = g_planner.g_zrm_gen;
    rec->zrm_bytes.assign((const unsigned char *)g_planner.g_zrm_map.p, (const unsigned char *)g_planner.g_zrm_map.p + (size_t)g_planner.g_zrm_map.len * (size_t)g_planner.g_zrm_map.esz);
}
void planner_restore(zeta_plan_t * rec) {
    planner_act_build(rec->cfg, rec->scope, rec->head);
    int n = rec->n; int CH_MAX = 65536; if (rec->cfg && rec->cfg->n + 1024 > CH_MAX) CH_MAX = rec->cfg->n + 1024;
    if (g_planner.codegen_flat_chain_body_nodes_cap < CH_MAX) { g_planner.codegen_flat_chain_body_nodes = (IR_t **)ct_grow(g_planner.codegen_flat_chain_body_nodes, (size_t)CH_MAX * sizeof(IR_t *));
        g_planner.codegen_flat_chain_body_nodes_cap = CH_MAX; }
    IR_t ** nodes = g_planner.codegen_flat_chain_body_nodes; memcpy(nodes, rec->nodes_own, sizeof(IR_t *) * (size_t)n);
    nidx_build(nodes, n);
    g_planner.zst_s = rec->v_zst_s; g_planner.zst_root = rec->v_zst_root; g_planner.zst_vroot = rec->v_zst_vroot; g_planner.zst_c = rec->v_zst_c; g_planner.zst_j = rec->v_zst_j;
        g_planner.zst_L = rec->v_zst_L;
    g_planner.zst_vuse = rec->v_zst_vuse; g_planner.zst_vcov = rec->v_zst_vcov; g_planner.zst_name = rec->v_zst_name; g_planner.zst_nmap_ok = 0; g_planner.zst_sm = rec->v_zst_sm;
        g_planner.zst_nodes = nodes;
        g_planner.zst_n = rec->zst_n_saved;
    g_planner.g_zd_djx = rec->v_djx; g_planner.g_zd_upost = rec->v_upost; g_planner.g_zd_grec = rec->v_grec; g_planner.g_zd_wrec = rec->v_wrec; g_planner.g_zd_grec_pop = rec->v_grec_pop;
    g_planner.g_zd_wrec_pop = rec->v_wrec_pop; g_planner.g_zd_admit = rec->v_admit;
    if (rec->djx_nodes_published) g_planner.g_zd_djx_nodes = nodes;
    zdo_build(nodes, n);
    if (g_planner.g_zrm_map.cap < rec->zrm_cap || g_planner.g_zrm_map.esz != rec->zrm_esz) { g_planner.g_zrm_map.p = ct_zalloc(rec->zrm_cap, rec->zrm_esz); g_planner.g_zrm_map.cap = rec->zrm_cap;
        g_planner.g_zrm_map.esz = rec->zrm_esz; }
    g_planner.g_zrm_map.len = rec->zrm_len; if (rec->zrm_len) memcpy(g_planner.g_zrm_map.p, rec->zrm_bytes.data(), (size_t)rec->zrm_len * (size_t)rec->zrm_esz); g_planner.g_zrm_gen = rec->zrm_gen;
    if (n > 0) { g_planner.g_zd_claim_p = rec->claim; g_planner.g_zd_xb_p = rec->xb; g_planner.g_zd_nodes_p = nodes; g_planner.g_zd_n = n; g_planner.g_zd_cut_p = rec->cut;
        g_planner.g_zd_fam_p = rec->fam;
        g_planner.g_zd_fbase_p = rec->fbase; g_planner.g_zd_gp_p = rec->gp; g_planner.g_zd_out_p = rec->out; g_planner.g_zd_wp_p = rec->wp; }
    g_planner.g_zd_plan_on = rec->on; g_planner.g_zd_plan_nodes = nodes; g_planner.g_zd_plan_n = n;
    if (rec->zarm_published) { g_planner.g_zd_zarm_p = rec->arm; g_planner.g_zd_zarm_nodes = nodes; g_planner.g_zd_zarm_n = n; }
    g_planner.g_zd_rec_p = rec;
}
static int zsm_frame_on(void) {
    if (g_planner.zsm_frame_on < 0) { const char * e = getenv("SCRIP_ZSM"); g_planner.zsm_frame_on = (e && *e == '1') ? 1 : 0; }
    return g_planner.zsm_frame_on;
}
extern "C" int planner_zsm_on(void) { return zsm_frame_on(); }
extern "C" long planner_zsm_row_word(const IR_graph_t * g, const zeta_plan_t * rec, int i, int * own0) {
    *own0 = 0;
    if (!rec || i < 0 || i >= rec->n || !rec->on[i]) return 0L;
    const IR_t * nd = rec->nodes[i];
    int rel = rec->gp && rec->wp;
    long k = zd_k_g(g, (IR_t *)nd), gp = rel ? rec->gp[i] : 0, wp = rel ? rec->wp[i] : 0;
    int o = (int)nd->op, en = o == IR_MATCH_END || o == IR_MATCH_REPLACE, ex = zd_check_arm_env(o) || o == IR_UNMARK || o == IR_CUT || en;
    if (o == IR_MATCH_REPLACE) gp += zd_rel_g(g, (IR_t *)nd);
    if (o == IR_MATCH_END) {
        const IR_t * ob = (nd->n_operands > 0 && nd->operands[0] && nd->operands[0]->op == IR_MATCH_BEGIN) ? nd->operands[0] : (const IR_t *)0;
        int j = ob ? nidx(rec->nodes, rec->n, ob) : -1;
        gp = k;
        if (rel && rec->gp[i] > 0 && j >= 0 && rec->on[j]) gp += rec->out[i] - rec->gp[i] - (rec->out[j] - zd_k_g(g, (IR_t *)ob));
    }
    IR_t * tg = zd_chase(nd->γ.node), * tw = zd_chase(nd->ω.node);
    int lg = !ex && tg && !zd_check_discard(tg) && !zd_check_arm_env((int)tg->op);
    int lw = !ex && tw && !zd_check_discard(tw) && !zd_check_arm_env((int)tw->op) && !(tw->op == IR_MATCH_BEGIN && zd_sz_phi(zd_edge_sz((IR_t *)nd, 1)));
    int lb = !zd_check_discard(nd) && !zd_check_arm_env(o);
    int ret = zls_retains_why(g, nd) != 0;
    if (o == IR_MATCH_BEGIN) *own0 = 8;
    return (k & 0xFFFFL) | ((gp & 0xFFFFL) << 16) | ((wp & 0xFFFFL) << 32) | (1L << 48) | ((long)rel << 49) | ((long)lg << 50) | ((long)lw << 51) | ((long)lb << 54) | ((long)ret << 57);
}
static void planner_zsm_graph_facts(IR_graph_t * g) {
    int n = 0;
    g->pf.kcls = zls_g_key_map(g, &n); g->pf.kcls_n = n; g->pf.frame_region = zls_g_frame_region(g); g->pf.resident = zls_g_resident_bytes(g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void planner_report_at_exit(void) {
    fprintf(stderr, "[PLANNER-PASS] planned=%d planned_rt=%d hits=%d fallbacks=%d\n", g_planner.planner_planned, g_planner.planner_rt, g_planner.planner_hits, g_planner.planner_fallbacks);
}
static void planner_report_arm(void) { if (!g_planner.planner_report_armed) { g_planner.planner_report_armed = 1; if (getenv("SCRIP_PLANNER_REPORT")) atexit(planner_report_at_exit); } }
void planner_pass_plan(IR_graph_t * g, IR_t * entry, const char * prefix) {
    planner_report_arm();
    zeta_plan_t * rec = planner_chain_plan(g, entry, prefix); rec->cfg = g; rec->key = entry; g_planner.planner_table[entry] = rec; g_planner.planner_planned++;
    rec->zdp_on = x86_zdp_on_c();
    for (int i = 0; i < rec->n; i++) rec->zn[i].f0r = rec->nodes_own[i]->op == IR_MATCH_FENCE0 ? planner_fence0_release(g, rec->nodes_own[i]) : 0;
    for (int i = 0; i < rec->n; i++) rec->zn[i].rel = rec->on[i] ? zd_rel_g(g, rec->nodes_own[i]) : 0;
}
int planner_entry_planned(const IR_t * entry) { return (entry && g_planner.planner_table.find(entry) != g_planner.planner_table.end()) ? 1 : 0; }
void planner_table_forget(const IR_t * entry) { if (entry) g_planner.planner_table.erase(entry); }
void planner_table_clear(void) { g_planner.planner_table.clear(); }
zeta_plan_t * planner_chain_record(IR_graph_t * g, IR_t * entry, const char * prefix) {
    planner_report_arm();
    std::unordered_map<const IR_t *, zeta_plan_t *>::iterator it = g_planner.planner_table.find(entry);
    if (it == g_planner.planner_table.end() || it->second->cfg != g) {
        fprintf(stderr, "[PLANNER] chain not staged before emission: prefix=%s graph=%p n=%d %s -- every road stages its chain through the planner stage first\n", prefix ? prefix : "?",
            (const void *) g, g ? g->n : -1,
            it == g_planner.planner_table.end() ? "(no record for this entry)" : "(the record is for another graph)");
        abort();
    }
    g_planner.planner_hits++;
    if (g && zsm_frame_on()) planner_zsm_graph_facts(g);
    planner_restore(it->second);
    return it->second;
}
int planner_retains(const IR_t * nd) { return planner_retains_g(planner_published_graph(), nd); }
int zd_arrival_depth(IR_t **nodes, int n, int i, const unsigned char *zon, const int *zout, const int *zgp, const int *zwp)
    { return zd_arrival_depth_g(planner_published_graph(), nodes, n, i, zon, zout, zgp, zwp); }
void zd_check(IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, const int *xb, const int *zchn, const int *zcut, const int *zarm,
    const char *prefix){ zd_check_g(planner_published_graph(), nodes, n, zon, zout, zgpop, zwpop, claim, xb, zchn, zcut, zarm, prefix); }
int zd_cl_read(IR_t **nodes, int n, int i, const unsigned char *on, const int *out, const int *xb) { return zd_cl_read_g(planner_published_graph(), nodes, n, i, on, out, xb); }
IR_t * zd_conj_value_node(IR_t * nd) { return zd_conj_value_node_g(planner_published_graph(), nd); }
void zd_depth_census(IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, const char *prefix)
    { zd_depth_census_g(planner_published_graph(), nodes, n, zon, zout, zgpop, zwpop, prefix); }
int zd_edge_retains(const IR_t * nd) { return zd_edge_retains_g(planner_published_graph(), nd); }
int zd_gate_pair(IR_t **nodes, int n, int a, int k, int wantb, const unsigned char *zon, const int *zout, const int *xb, long *rsp)
    { return zd_gate_pair_g(planner_published_graph(), nodes, n, a, k, wantb, zon, zout, xb, rsp); }
int zd_gate_pair_at(IR_t **nodes, int n, int g, int a, int k, int wantb, const unsigned char *zon, const int *zout, const int *xb, long *rsp)
    { return zd_gate_pair_at_g(planner_published_graph(), nodes, n, g, a, k, wantb, zon, zout, xb, rsp); }
int zd_icn_local_assign(const IR_t * nd) { return zd_icn_local_assign_g(planner_published_graph(), nd); }
int zd_icn_local_var(const IR_t * nd) { return zd_icn_local_var_g(planner_published_graph(), nd); }
int zd_icn_suspend(const IR_t * nd) { return zd_icn_suspend_g(planner_published_graph(), nd); }
int zd_k(IR_t * nd) { return zd_k_g(planner_published_graph(), nd); }
int zd_k_own(IR_t * nd) { return zd_k_own_g(planner_published_graph(), nd); }
int zd_nops(IR_t * nd) { return zd_nops_g(planner_published_graph(), nd); }
IR_t * zd_value_producer(IR_t * nd) { return zd_value_producer_g(planner_published_graph(), nd); }
IR_graph_t * planner_published_graph(void) { return g_planner.g_zd_rec_p ? g_planner.g_zd_rec_p->cfg : (IR_graph_t *)0; }
}

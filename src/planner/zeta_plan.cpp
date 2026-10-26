#include "../emitter/emit_head.h"
#include "../emitter/emit_decls.h"
#include "planner.h"
g_planner_t g_planner;
void nidx_build(IR_t **nodes, int n);
int fence0_release_bytes(const IR_t * nd);
int nidx(IR_t **nodes, int n, const IR_t *e);
int zst_coord(const char * name);
int zst_s_of(const IR_t * nd);
IR_t * zd_chase(IR_t * t);
int zd_yield_end(IR_t ** nodes, int n, int sp, const unsigned char * zon, const int * claim);
int zd_edge_retains_g(const IR_graph_t * cg, const IR_t * nd);
int zd_edge_retains(const IR_t * nd);
static int zd_arm_resumes(const IR_t * a, const IR_t * g);
static int zd_gate_holds(const IR_t * g);
static int zd_beta_via_act(const IR_graph_t * cg, const IR_t * nd);
int zd_djx_of(const IR_t * nd);
int zd_upost_of(const IR_t * nd);
int zd_nops_g(const IR_graph_t * cg, IR_t * nd);
int zd_nops(IR_t * nd);
int zd_k_g(const IR_graph_t * cg, IR_t * nd);
int zd_k(IR_t * nd);
int zd_k_g(const IR_graph_t * cg, IR_t * nd);
int zd_k(IR_t * nd);
int zd_k_own_g(const IR_graph_t * cg, IR_t * nd);
int zd_k_own(IR_t * nd);
int zd_icn_suspend_g(const IR_graph_t * cg, const IR_t * nd);
int zd_icn_suspend(const IR_t * nd);
static int zd_icn_stmt_head(const IR_graph_t * cg, const IR_t * nd);
int zd_icn_local_var_g(const IR_graph_t * cg, const IR_t * nd);
int zd_icn_local_var(const IR_t * nd);
int zd_icn_local_assign_g(const IR_graph_t * cg, const IR_t * nd);
int zd_icn_local_assign(const IR_t * nd);
const IR_t * zd_local_home_node(const IR_graph_t * cg, const IR_t * nd);
int zd_k_g(const IR_graph_t * cg, IR_t * nd);
int zd_k(IR_t * nd);
int zd_icn_suspend_g(const IR_graph_t * cg, const IR_t * nd);
int zd_icn_suspend(const IR_t * nd);
int zd_k_g(const IR_graph_t * cg, IR_t * nd);
int zd_k(IR_t * nd);
int zd_nops_g(const IR_graph_t * cg, IR_t * nd);
int zd_nops(IR_t * nd);
int zd_k_own_g(const IR_graph_t * cg, IR_t * nd);
int zd_k_own(IR_t * nd);
int zst_s_of(const IR_t * nd);
int zd_k_own_g(const IR_graph_t * cg, IR_t * nd);
int zd_k_own(IR_t * nd);
int zd_dj_admitted(int n, int ei) { return (ei >= 0 && ei < n && (size_t)ei < g_planner.g_zd_admit.size() && g_planner.g_zd_admit[(size_t)ei]) ? 1 : 0; }
int zd_dj_cellless(IR_t ** nodes, int n, const int * zarm, int k) {
    return k >= 0 && nodes[k]->op == IR_DISJUNCTION && zarm && !zd_dj_admitted(n, k);
}
int zd_pl_dj_arm_holds(const IR_t * d, int a) {
    IR_t * t = zd_chase(d->operands[2 * a + 1]);
    return t && t != d && t->op != IR_FAIL && t->op != IR_SUCCEED && t->op != IR_UNMARK && t->op != IR_CUT;
}
static int zd_pl_dj_holds(const IR_t * d) {
    int na = (d && d->op == IR_DISJUNCTION) ? (int)IR_LIT(d).ival : 0;
    if (na <= 0 || d->n_operands < 2 * na) return 0;
    for (int a = 0; a < na; a++) if (zd_pl_dj_arm_holds(d, a)) return 1;
    return 0;
}
int zd_dj_arm_end(IR_t ** nodes, int n, int dj, int a) {
    IR_t * d = nodes[dj], * x = zd_chase(d->operands[2 * a]);
    for (int g = 0; x && x != d && g <= n; g++) {
        IR_t * nx = (x->op == IR_GATE_ARM && !x->γ.node && x->n_operands > 1) ? zd_chase(x->operands[1]->γ.node) : zd_chase(x->γ.node);
        if (nx == d) return nidx(nodes, n, x);
        x = nx;
    }
    return -1;
}
IR_t * zd_scan_subject(IR_t * nd) {
    if (!nd || nd->op != IR_SCAN || nd->n_operands < 1 || !nd->operands[0] || nd->operands[0]->n_operands < 1) return (IR_t *)0;
    return (IR_LIT(nd).dval == 4.0 || (nd->n_operands > 2 && !nd->operands[1])) ? nd->operands[0]->operands[0] : (IR_t *)0;
}
IR_t * zd_value_producer_g(const IR_graph_t * cg, IR_t * nd) {
    for (int g = 0; nd && g < 16 && zd_k_own_g(cg, nd) == 0 && (nd->op == IR_ASSIGN || nd->op == IR_CONJUNCTION) && nd->n_operands > 0; g++) nd = nd->operands[0];
    return nd;
}
IR_t * zd_conj_value_node_g(const IR_graph_t * cg, IR_t * nd) { int g = 0; while (nd && nd->op == IR_CONJUNCTION && nd->n_operands > 0 && zd_k_own_g(cg, nd) == 0 && g++ < 16) nd = nd->operands[0];
    return nd; }
int zd_plan_on_of(const IR_t * nd) { if (!nd || !g_planner.g_zd_plan_on || g_planner.g_zd_plan_nodes != g_planner.codegen_flat_chain_body_nodes) return 0;
    for (int k = 0; k < g_planner.g_zd_plan_n; k++) if (g_planner.g_zd_plan_nodes[k] == nd) return g_planner.g_zd_plan_on[k] ? 1 : 0; return 0; }
int zd_sr_role(const IR_t * nd) { int64_t v = IR_LIT(nd).ival; return (v == 1 || v == 2 || v == 3 || v == 4) ? (int)v : 0; }
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
IR_t * zd_chase(IR_t * t) { int g = 0; while (t && (t->op == IR_GOTO || (t->op == IR_SUCCEED && t->γ.node)) && g++ < 4096) t = t->γ.node; return t; }
int zd_yield_end(IR_t ** nodes, int n, int sp, const unsigned char * zon, const int * claim) {
    IR_t * t = zd_chase(nodes[sp]->γ.node);
    for (int g = 0; t && g <= n; g++) {
        int k = nidx(nodes, n, t);
        if (k < 0 || !zon[k]) return -1;
        IR_t * nx = zd_chase(t->γ.node);
        int nk = nx ? nidx(nodes, n, nx) : -1;
        if (!nx || nx->op == IR_SUCCEED || nk < 0 || !zon[nk]) return claim[k] != claim[sp] ? -1 : k;
        t = nx;
    }
    return -1;
}
static IR_t * zd_suspend_run_next(IR_t ** nodes, int n, IR_t * sp, const int * claim) {
    IR_t * r = zd_chase(sp->n_operands > 1 ? sp->operands[1] : (IR_t *)0), * t = zd_chase(sp->γ.node), * u = t;
    for (int g = 0; r && u && u != r && g <= n; g++) { int k = nidx(nodes, n, u); if (k < 0 || claim[k] >= 0) return r; u = zd_chase(u->γ.node); }
    return (r && u == r) ? t : r;
}
void zd_vdj_live(IR_t ** nodes, int n, int * live) {
    int q[n > 0 ? n : 1];
    unsigned char seen[n > 0 ? n : 1];
    for (int i = 0; i < n; i++) live[i] = 0;
    for (int d = 0; d < n; d++) {
        IR_t * dj = nodes[d];
        long k = 0;
        int na = dj ? (int)IR_LIT(dj).ival : 0, qn = 0;
        if (!dj || dj->op != IR_DISJUNCTION || na <= 0 || dj->n_operands < 2 * na || !fc_geom(dj, &k) || k <= 0) continue;
        memset(seen, 0, (size_t)(n > 0 ? n : 1));
        for (int a = 0; a < na; a++) { int x = nidx(nodes, n, zd_chase(dj->operands[2 * a])); if (x >= 0 && x != d && !seen[x]) { seen[x] = 1; q[qn++] = x; } }
        for (int h = 0; h < qn; h++) {
            IR_t * x = nodes[q[h]];
            int m = 2 + ((x->op == IR_DISJUNCTION && (int)IR_LIT(x).ival > 0 && x->n_operands >= 2 * (int)IR_LIT(x).ival) ? (int)IR_LIT(x).ival : 0);
            live[q[h]] += (int)k;
            for (int e = 0; e < m; e++) {
                IR_t * t = zd_chase(e == 0 ? x->γ.node : e == 1 ? x->ω.node : x->operands[2 * (e - 2)]);
                int y = nidx(nodes, n, t);
                if (y < 0 || y == d || seen[y] || t->op == IR_STATEMENT_BEGIN || t->op == IR_STATEMENT_END || t->op == IR_STATEMENT) continue;
                seen[y] = 1;
                q[qn++] = y;
            }
        }
    }
}
int zd_nops_g(const IR_graph_t * cg, IR_t * nd) { int op = (int)ir_kind0(nd);
    if (cg && cg->spine_cells && !zd_icn_suspend_g(cg, nd) && op != IR_DISJUNCTION && op != IR_GATE && op != IR_GATE_ARM && op != IR_BOUND && op != IR_UNMARK && op != IR_CREATE)
        return (int)nd->n_operands;
    if (planner_resumable_frame(cg) && (op == IR_CALL_PL_LEAF || op == IR_CALL_DIRECT)) return (int)nd->n_operands;
    if (planner_resumable_frame(cg) && (op == IR_UNIFY_CONST || op == IR_UNIFY_STRUCT || op == IR_UNIFY_FIRST || op == IR_UNIFY_VALUE))
        return (nd->n_operands > 0 && nd->operands[0] && nd->operands[0]->op == IR_UNIFY_STRUCT) ? 1 : 0;
    if (op == IR_KW_ASSIGN_SNOBOL4) return 1;
    if (op == IR_ITERATE) return 1;
    if (zd_icn_suspend_g(cg, nd)) return 1;
    return (op == IR_UNOP || op == IR_ASSIGN || op == IR_DEREF || op == IR_COERCE_STRING || op == IR_COERCE_INTEGER || op == IR_FIELD_VAR || op == IR_FIELD_GET || op == IR_MATCH_BEGIN ||
        op == IR_RETURN) ? 1 :
        (op == IR_BINOP || op == IR_ASSIGN_FRAME || op == IR_COERCE_NUMERIC || op == IR_CMP_TEST || op == IR_IDENT || op == IR_DIFFER || op == IR_BINOP_TEST || op == IR_BINOP_RELOP_VAL) ? 2 :
            (op == IR_MATCH_REPLACE) ? 2 :
        (op == IR_SUBSCRIPT || op == IR_ASSIGN_VAR || op == IR_MAKE_LIST || ir_norm_call_kind((IR_e)op) == IR_CALL || op == IR_PROC_GEN || op == IR_SUSPEND || op == IR_TO ||
            op == IR_TO_BY || op == IR_MATCH_LEN ||
        op == IR_MATCH_ANY || op == IR_MATCH_NOTANY || op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_SPAN || op == IR_MATCH_BREAK ||
        op == IR_MATCH_BREAKX || op == IR_MATCH_ASSIGN_COND || op == IR_MATCH_ASSIGN_IMM || op == IR_MATCH_VALUE) ? (int)nd->n_operands : 0; }
void zd_rec_arm(const IR_graph_t * cg, IR_t ** nodes, int i) {
    g_planner.g_zd_grec_off = -1;
    g_planner.g_zd_wrec_off = -1;
    if (i < 0 || i >= (int)g_planner.g_zd_grec.size()) return;
    int gd = g_planner.g_zd_grec[(size_t)i], wd = g_planner.g_zd_wrec[(size_t)i];
    int gs = gd >= 0 ? planner_frame_slot(cg, nodes[gd]) : -1, ws = wd >= 0 ? planner_frame_slot(cg, nodes[wd]) : -1;
    if (gs != -1) { g_planner.g_zd_grec_off = gs + 24; g_planner.g_zd_grec_res = g_planner.g_zd_grec_pop[(size_t)i]; }
    if (ws != -1) { g_planner.g_zd_wrec_off = ws + 24; g_planner.g_zd_wrec_res = g_planner.g_zd_wrec_pop[(size_t)i]; }
}
void zd_cp_arm(const IR_graph_t * cg, IR_t ** nodes, int i, const int * zcps, const int * zcpd, const int * zout) {
    g_planner.g_zd_cpsx = -1;
    g_planner.g_zd_cpdx = -1;
    if (zcpd[i] > -2) {
        if (zcps[i] > -2) return;
        int k = -zcps[i] - 2, sx = planner_frame_slot(cg, nodes[k]);
        if (sx == -1 && zd_value_producer_g(cg, nodes[k]) != nodes[k]) sx = planner_frame_slot(cg, zd_value_producer_g(cg, nodes[k]));
        if (sx != -1) { g_planner.g_zd_cpsx = sx; g_planner.g_zd_cps = 0; }
        else g_planner.g_zd_cps = zout[i] - zout[k];
        return;
    }
    int d = -zcpd[i] - 2, k = zcps[i] <= -2 ? -zcps[i] - 2 : -1;
    int dx = planner_frame_slot(cg, nodes[d]), sx = k >= 0 ? planner_frame_slot(cg, nodes[k]) : -1;
    g_planner.g_zd_cpd = -1;
    g_planner.g_zd_cps = -1;
    if (dx == -1 || k < 0) return;
    g_planner.g_zd_cpdx = dx;
    if (sx != -1) g_planner.g_zd_cpsx = sx;
    else g_planner.g_zd_cps = zout[i] - zout[k];
}
int zd_icn_local_var_g(const IR_graph_t * cg, const IR_t * nd) { return nd && nd->op == IR_VAR && cg && cg->spine_cells && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '&' &&
    (!is_global(IR_LIT(nd).sval) || graph_has_local(cg, IR_LIT(nd).sval)); }
int zd_icn_local_assign_g(const IR_graph_t * cg, const IR_t * nd) { return nd && nd->op == IR_ASSIGN && cg && cg->spine_cells && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '&' &&
    (!is_global(IR_LIT(nd).sval) || graph_has_local(cg, IR_LIT(nd).sval)); }
const IR_t * zd_local_home_node(const IR_graph_t * cg, const IR_t * nd) { int g = 0;
    while (nd && (nd->op == IR_CONJUNCTION || (nd->op == IR_ASSIGN && !zd_icn_local_assign_g(cg, nd))) && nd->n_operands > 0 && g++ < 16) nd = nd->operands[0];
    return (zd_icn_local_var_g(cg, nd) || zd_icn_local_assign_g(cg, nd)) ? nd : (const IR_t *)0; }
static int zd_icn_chain_step_ok(const IR_graph_t * cg, IR_t ** nodes, int n, int p, int q) {
    if (!(cg && cg->spine_cells)) return 1;
    if (p < 0 || q < 0 || zd_chase(nodes[p]->ω.node) != nodes[q]) return 0;
    switch (nodes[p]->op) {
        case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_REAL: case IR_VAR: case IR_VAR_REF: case IR_DEREF: case IR_COERCE_NUMERIC: case IR_BINOP: case IR_BINOP_TEST: case IR_UNOP: case IR_ASSIGN:
        case IR_CALL_BYNAME: case IR_CALL_DIRECT: case IR_LINE_MARK: return 1;
        default: return 0;
    }
}
int zd_icn_suspend_g(const IR_graph_t * cg, const IR_t * nd) { return nd && nd->op == IR_SUSPEND && cg && cg->spine_cells; }
static int zd_icn_stmt_head(const IR_graph_t * cg, const IR_t * nd) { extern const char * bb_src_of(const IR_t *); const char * s = (nd && cg && cg->spine_cells) ? bb_src_of(nd) : (const char *)0;
    return (s && (!strcmp(s, "stmt") || !strcmp(s, "nstmt"))) ? 1 : 0; }
static int zd_icn_stmt_sibling(const IR_graph_t * cg, const IR_t * nd) { extern const char * bb_src_of(const IR_t *); const char * s = (nd && cg && cg->spine_cells) ? bb_src_of(nd) : (const char *)0;
    return (s && !strcmp(s, "stmt")) ? 1 : 0; }
int zd_djx_of(const IR_t * nd) { if (!nd || nd->op != IR_DISJUNCTION || g_planner.g_zd_djx_nodes != g_planner.codegen_flat_chain_body_nodes) return 0;
    int i = nidx(g_planner.g_zd_djx_nodes, (int)g_planner.g_zd_djx.size(), nd); return i >= 0 ? g_planner.g_zd_djx[(size_t)i] : 0; }
int zd_upost_of(const IR_t * nd) { if (!nd || nd->op != IR_UNMARK || g_planner.g_zd_djx_nodes != g_planner.codegen_flat_chain_body_nodes) return 0;
    int i = nidx(g_planner.g_zd_djx_nodes, (int)g_planner.g_zd_upost.size(), nd); return i >= 0 ? g_planner.g_zd_upost[(size_t)i] : 0; }
int zd_k_g(const IR_graph_t * cg, IR_t * nd) { return zd_k_own_g(cg, nd) + zst_s_of(nd) + zd_djx_of(nd); }
static int zd_dj_spine(const IR_t * nd) { if (!g_planner.g_zd_zarm_p || g_planner.g_zd_zarm_nodes != g_planner.codegen_flat_chain_body_nodes) return 0;
    int i = nidx(g_planner.g_zd_zarm_nodes, g_planner.g_zd_zarm_n, nd); return (i >= 0 && zd_dj_admitted(g_planner.g_zd_zarm_n, i) && !zls_dj_retains(nd)) ? 32 : 0; }
int zd_call_curmov(const IR_t * nd) { return lower_call_node_is_curmov(nd); }
extern "C" int zd_dj_on_spine(const IR_t * nd) { return zd_dj_spine(nd) ? 1 : 0; }
enum { BCS_ALWAYS, BCS_CALL_CURMOV, BCS_ICN_LOCAL, BCS_ICN_SUSPEND, BCS_HAS_OPERANDS, BCS_NO_OPERANDS, BCS_PATV, BCS_HOME_FRAME, BCS_HOME_STATE, BCS_PL_HOME, BCS_PL_HOME_DYN, BCS_PL_HOME_RANGE_HDR,
    BCS_PL_HOME_HAS_OPERANDS, BCS_ICN_HOME, BCS_PL_BOUND_SPINE };
extern "C" int zls_g_home_frame(const IR_graph_t * g);
extern "C" int zls_home_why(const IR_t * nd);
extern "C" int planner_home_dyn(const IR_graph_t * g, const IR_t * nd);
static int zd_pl_home(const IR_graph_t * cg) { return (zls_g_home_frame(cg) && zls_g_resumable_frame(cg)) ? 1 : 0; }
static int zd_pl_home_dyn(const IR_graph_t * cg, IR_t * nd) { return zd_pl_home(cg) && (zls_home_why(nd) || zd_edge_retains_g(cg, nd) || planner_home_dyn(cg, nd)) ? 1 : 0; }
#include "box_cell_shapes.inc"
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
    return (w == BCS_PATV && nd->pat_static && IR_LIT(nd).sval && !strncmp(IR_LIT(nd).sval, "PATV$", 5)) ? 1 : 0;
}
extern "C" int zls_off(const IR_t * nd);
extern "C" int zls_g_hf_resident(const IR_graph_t * g, const IR_t * nd);
int zd_cross_cell_g(const IR_graph_t * cg, int off) {
    int n = g_planner.g_zd_n, i = g_planner.g_zd_cur;
    IR_t ** nodes = g_planner.g_zd_nodes_p;
    const int * out = g_planner.g_zd_out_p, * xb = g_planner.g_zd_xb_p, * cl = g_planner.g_zd_claim_p, * fam = g_planner.g_zd_fam_p, * fb = g_planner.g_zd_fbase_p;
    const unsigned char * on = g_planner.g_zd_plan_on;
    if (!nodes || !out || !on || g_planner.g_zd_plan_nodes != nodes || i < 0 || i >= n || !on[i]) return -1;
    int b = -1, key = -1;
    for (int c = i - 1; c >= 0 && b < 0; c--) { int k = on[c] ? zls_off(nodes[c]) : -1; if (k >= 0 && off >= k && off < k + zd_k_own_g(cg, nodes[c])) { b = c; key = k; } }
    for (int c = i + 1; c < n && b < 0; c++) { int k = on[c] ? zls_off(nodes[c]) : -1; if (k >= 0 && off >= k && off < k + zd_k_own_g(cg, nodes[c])) { b = c; key = k; } }
    for (int c = b + 1; b > i && c < n; c++) { int k = on[c] ? zls_off(nodes[c]) : -1; if (k >= 0 && off >= k && off < k + zd_k_own_g(cg, nodes[c])) return -1; }
    if (b < 0) return -1;
    long xi, xo;
    if (xb && xb[i] != INT_MIN && xb[b] != INT_MIN) { xi = (long)xb[i] + out[i]; xo = (long)xb[b] + out[b]; }
    else if (cl && cl[i] >= 0 && cl[i] == cl[b]) { xi = out[i]; xo = out[b]; }
    else if (fam && fb && cl && cl[i] >= 0 && cl[b] >= 0 && fam[cl[i]] >= 0 && fam[cl[i]] == fam[cl[b]]) { xi = (long)fb[cl[i]] + out[i]; xo = (long)fb[cl[b]] + out[b]; }
    else return -1;
    return xi < xo ? -1 : (int)(xi - xo) + (off - key);
}
static int zd_bcs_words(const IR_graph_t * cg, const IR_t * nd, int w) {
    if (w == -1) return zd_dj_spine(nd);
    if (w == -2) return zd_dj_spine(nd) ? zd_dj_spine(nd) + 32 : 0;
    if (w == -3) return 16 * (2 + nd->n_operands);
    if (w == -4) return planner_fence_k(cg, nd);
    if (w == -5) return planner_alt_k(cg, nd);
    return w;
}
int zd_k_own_g(const IR_graph_t * cg, IR_t * nd) {
    int op = (int)nd->op;
    for (size_t r = 0; r < sizeof box_cell_shapes / sizeof box_cell_shapes[0]; r++) {
        if ((box_cell_shapes[r].op >= 0 && box_cell_shapes[r].op != op) || !zd_bcs_when(cg, nd, box_cell_shapes[r].when)) continue;
        return zd_bcs_words(cg, nd, box_cell_shapes[r].words);
    }
    return 16;
}
static int zd_map_on(void) { if (g_planner.zd_map_on__m < 0) { const char * e = getenv("SCRIP_ZD_MAP"); g_planner.zd_map_on__m = (e && *e == '1') ? 1 : 0; } return g_planner.zd_map_on__m; }
typedef struct { const IR_t *k; int v; uint32_t gen; } zdo_slot_t;
typedef struct { const IR_t * nd; uint32_t gen; int maxk; } zrm_slot_t;
static zdo_slot_t *zdo_probe(const IR_t *k) { uint64_t h = (((uint64_t)(uintptr_t)k >> 4) * 11400714819323198485ull) & (g_planner.g_zdo.len - 1);
    for (;;) { zdo_slot_t *q = &CV_AT(g_planner.g_zdo, zdo_slot_t, h); if (q->gen != g_planner.g_zdo_gen || q->k == k) return q; h = (h + 1) & (g_planner.g_zdo.len - 1); } }
static void zdo_build(IR_t **nodes, int n) {
    g_planner.g_zdo_gen++; g_planner.g_zdo_cnt = 0; g_planner.g_zdo_nodes = nodes; g_planner.g_zdo_n = n;
    uint32_t need = 64; while (need < (uint32_t)n * 2 + 2) need <<= 1;
    if (g_planner.g_zdo.len < need) { if (g_planner.g_zdo.p) ct_drop(g_planner.g_zdo.p); g_planner.g_zdo.p = ct_zalloc(need, sizeof(zdo_slot_t)); g_planner.g_zdo.len = need;
        g_planner.g_zdo.cap = need; g_planner.g_zdo.esz = (uint32_t)sizeof(zdo_slot_t); }
    for (int k = 0; k < n; k++) if (zd_omega_test_kind(nodes[k])) { IR_t *t = zd_chase(nodes[k]->ω.node); if (!t) continue; zdo_slot_t *q = zdo_probe(t);
        if (q->gen != g_planner.g_zdo_gen) { q->k = t;
        q->v = k; q->gen = g_planner.g_zdo_gen; g_planner.g_zdo_cnt++; } }
}
static int zdo_first(IR_t **nodes, int n, IR_t *t) {
    if (t && nodes == g_planner.g_zdo_nodes && n == g_planner.g_zdo_n && g_planner.g_zdo.len) { zdo_slot_t *q = zdo_probe(t); return q->gen == g_planner.g_zdo_gen ? q->v : -1; }
    for (int k = 0; k < n; k++) if (zd_omega_test_kind(nodes[k]) && zd_chase(nodes[k]->ω.node) == t) return k; return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static zrm_slot_t * zrm_probe(const IR_t * nd) {
    uint32_t m = g_planner.g_zrm_map.len - 1; uint64_t h = (((uint64_t)(uintptr_t)nd >> 4) * 0xff51afd7ed558ccdull) & m;
    for (;;) { zrm_slot_t * t = &CV_AT(g_planner.g_zrm_map, zrm_slot_t, h); if (t->gen != g_planner.g_zrm_gen || t->nd == nd) return t; h = (h + 1) & m; }
}
static void zrm_build(IR_t ** nodes, const int * run, int rl) {
    g_planner.g_zrm_gen++;
    if ((uint64_t)g_planner.g_zrm_map.len < (uint64_t)rl * 2 + 2) { uint32_t c = 256; while ((uint64_t)c < (uint64_t)rl * 2 + 2) c *= 2; cv_t v = { 0, 0, 0, 0 };
        v.p = ct_zalloc(c, sizeof(zrm_slot_t));
        v.len = c; v.cap = c; v.esz = (uint32_t)sizeof(zrm_slot_t); g_planner.g_zrm_map = v; }
    for (int k = 0; k < rl; k++) { zrm_slot_t * t = zrm_probe(nodes[run[k]]); t->nd = nodes[run[k]]; t->gen = g_planner.g_zrm_gen; t->maxk = k; }
}
static int zrm_maxk(const IR_t * nd) { zrm_slot_t * t = zrm_probe(nd); return t->gen == g_planner.g_zrm_gen ? t->maxk : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zd_omega_test_kind(const IR_t * nd) { return (nd && nd->ω.node && ir_decides_own_failure_kind(nd->op)) ? 1 : 0; }
static int zd_omega_head(IR_t **nodes, int n, IR_t *t) { return zdo_first(nodes, n, t) >= 0; }
static int zd_omega_seed(IR_t **nodes, int n, IR_t *t, unsigned char *zon, int *zout, const int *claim) { int k = zdo_first(nodes, n, t); if (k < 0) return 0; int s = zon[k] ? zout[k] : 0;
    for (int j = k + 1; j < n; j++) if (zon[j] && claim[j] == claim[k] && zout[j] > s && zd_omega_test_kind(nodes[j]) && zd_chase(nodes[j]->ω.node) == t) s = zout[j]; return s; }
static int zd_omega_test_idx(IR_t **nodes, int n, IR_t *t) { return zdo_first(nodes, n, t); }
static int zd_vl_join(const IR_t *last, const IR_t *cur) { return last && cur && last->op == IR_ASSIGN && cur->op == IR_VAR && IR_LIT(last).sval && IR_LIT(cur).sval &&
    !strcmp(IR_LIT(last).sval, IR_LIT(cur).sval); }
static int zd_vl_sibling(IR_t **nodes, int n, const int *claim, int c, IR_t *cur) {
    for (int k = 0; k < n; k++) if (claim[k] == c && zd_chase(nodes[k]->γ.node) == cur && zd_vl_join(nodes[k], cur)) return 1;
    return 0;
}
int zd_arrival_depth_g(const IR_graph_t * cg, IR_t **nodes, int n, int i, const unsigned char *zon, const int *zout, const int *zgp, const int *zwp) {
    int d = -1;
    for (int j = 0; j < n; j++) {
        int e = -1;
        if (zd_chase(nodes[j]->ω.node) == nodes[i]) e = zon[j] ? zout[j] - zd_k_g(cg, nodes[j]) - zwp[j] : -2;
        if (zd_chase(nodes[j]->γ.node) == nodes[i]) { int g = zon[j] ? zout[j] - zgp[j] : -2; e = (e == -1 || e == g) ? g : -2; }
        if (e == -1) continue;
        if (e < 0 || (d >= 0 && d != e)) return -2;
        d = e;
    }
    return d;
}
static int zd_stmt_exit_kind(IR_e op) { return (op == IR_STATEMENT_END || op == IR_STATEMENT || op == IR_GOTO_DEFERRED) ? 1 : 0; }
static int zd_exit_pop_s(IR_e op, int mark, int full, int stmt, int mafter) { if (!(mark >= 0 && zd_stmt_exit_kind(op))) return full; int wm = mark;
    if (op == IR_GOTO_DEFERRED && stmt >= 0) return full - (stmt - wm);
    return (mafter >= 0 && full > mafter) ? (wm + (full - mafter)) : wm; }
static const char * zd_edge_sz(IR_t * x, int v) {
    const char * sz = v ? x->ω.sz : x->γ.sz;
    IR_t * t = v ? x->ω.node : x->γ.node;
    int g = 0;
    while (t && t->op == IR_GOTO && g++ < 128) { sz = t->γ.sz; t = t->γ.node; }
    return sz;
}
static int zd_sz_sigma(const char * sz) { return (unsigned char)sz[0] == 0xcf && (unsigned char)sz[1] == 0x83 && sz[2] == 0; }
static int zd_sz_phi(const char * sz) { return (unsigned char)sz[0] == 0xcf && (unsigned char)sz[1] == 0x86 && sz[2] == 0; }
static int zd_sz_alpha(const char * sz) { return (unsigned char)sz[0] == 0xce && (unsigned char)sz[1] == 0xb1 && sz[2] == 0; }
static int zd_dj_anc(const int * zarm, int i, int q) { int g = 0; for (int p = zarm[i]; p >= 0 && g++ < 4096; p = zarm[p]) if (p == q) return 1; return 0; }
extern "C" int planner_env_encloses(const int * zarm, int i, int q) { return (zarm && i >= 0 && q >= 0) ? zd_dj_anc(zarm, i, q) : 0; }
static IR_t * zd_dj_res(IR_t * dj, int j) {
    int na = (int)IR_LIT(dj).ival;
    IR_t * rj = (2 * na + j < dj->n_operands) ? dj->operands[2 * na + j] : (IR_t *)0;
    int g = 0;
    while (rj && rj->op == IR_CONJUNCTION && rj->n_operands > 0 && g++ < 16) rj = rj->operands[0];
    return rj;
}
static int zd_unmark_acts(const IR_graph_t * cg, const IR_t * nd) {
    const IR_t * sc = nd;
    if (sc->n_operands > 0 && sc->operands[0] && sc->operands[0]->op == IR_BOUND) sc = sc->operands[0];
    return cg ? zls_g_release_act_offs(cg, sc, 5, (int *)0, 0) : 0;
}
static IR_t * zd_lm_chase(IR_t * x, int n, IR_t ** last) { IR_t * s = x; IR_t * t = zd_chase(x->γ.node); int g = 0;
    while (t && t->op == IR_LINE_MARK && g++ <= n) { s = t; t = zd_chase(t->γ.node); } *last = s; return t; }
static int zd_loop_back_unmark(const IR_graph_t * cg, IR_t ** nodes, const int * run, int rb, const IR_t * head) {
    if (!head || head->op != IR_BOUND) return -1;
    int k = rb;
    while (k > 0 && nodes[run[k]]->op == IR_LINE_MARK && zd_chase(nodes[run[k - 1]]->γ.node) == nodes[run[k]]) k--;
    IR_t * u = nodes[run[k]];
    if (u->op != IR_UNMARK || u->n_operands < 1 || u->operands[0] != head || zd_unmark_acts(cg, u) > 0) return -1;
    return k;
}
static int zd_fails_to(const IR_graph_t * cg, IR_t * x, IR_t * t, int depth) {
    if (!x || depth > 16) return 0;
    if (x == t) return 0;
    int o = (int)x->op;
    if (!(o == IR_LINE_MARK || o == IR_BOUND || (o == IR_UNMARK && zd_unmark_acts(cg, x) <= 0))) return 0;
    IR_t * g = zd_chase(x->γ.node);
    IR_t * w = zd_chase(x->ω.node);
    if (g != w) return 0;
    if (g == t) { const char * sz = zd_edge_sz(x, 0); return port_sz_beta(sz) || zd_sz_phi(sz); }
    return zd_fails_to(cg, g, t, depth + 1);
}
static int zd_chain_link(const IR_graph_t * cg, IR_t ** nodes, int q, int t, const unsigned char * djok) {
    IR_t * qo = zd_chase(nodes[q]->ω.node);
    if (qo && qo != nodes[t] && zd_fails_to(cg, qo, nodes[t], 0)) return 1;
    const char * qs = zd_edge_sz(nodes[q], 1);
    if (nodes[q]->op == IR_DISJUNCTION) return djok[q] && (qo == nodes[t] || (qo && qo == zd_chase(nodes[q]->γ.node)));
    if (ir_is_generator_kind(nodes[q]->op) || nodes[q]->op == IR_TO) return 0;
    if (qo == nodes[t] && (port_sz_beta(qs) || zd_sz_phi(qs))) return 1;
    int o = (int)nodes[q]->op;
    int never_fails = o == IR_ASSIGN || o == IR_VAR || o == IR_VAR_REF || o == IR_LINE_MARK || o == IR_BOUND || o == IR_LIT_INTEGER || o == IR_LIT_STRING || o == IR_LIT_REAL ||
        o == IR_LIT_CHARSET;
    return !qo || qo == zd_chase(nodes[q]->γ.node) || never_fails;
}
static int zd_pred(const IR_graph_t * cg, IR_t ** nodes, int n, const int * claim, int hi, int k) {
    int p = -1;
    for (int j = 0; j < n; j++) {
        if (claim[j] != hi || j == k || zd_chase(nodes[j]->γ.node) != nodes[k]) continue;
        if (zd_icn_suspend_g(cg, nodes[j])) continue;
        const char * sz = zd_edge_sz(nodes[j], 0);
        if (port_sz_beta(sz) || zd_sz_sigma(sz) || zd_sz_phi(sz)) continue;
        if (p >= 0) return -2;
        p = j;
    }
    return p;
}
static int zd_pred_bt(const IR_graph_t * cg, IR_t ** nodes, int n, const int * claim, int hi, int k) {
    int p = zd_pred(cg, nodes, n, claim, hi, k);
    for (int g = 0; p >= 0 && nodes[p]->op == IR_LINE_MARK && g++ <= n; ) p = zd_pred(cg, nodes, n, claim, hi, p);
    return p;
}
static int zd_arm_resumes(const IR_t * a, const IR_t * g) {
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
extern "C" int planner_span_retains(const IR_graph_t * g, int from_ai, int to_ai) {
    if (!g || !g->all || g->n <= 0) return 0;
    if (from_ai > to_ai) { int t = from_ai; from_ai = to_ai; to_ai = t; }
    if (from_ai < 0) from_ai = 0;
    if (to_ai >= g->n) to_ai = g->n - 1;
    int r = 0;
    for (int k = from_ai; k <= to_ai && !r; k++) {
        const IR_t * nd = g->all[k];
        if (nd && (zd_edge_retains_g(g, nd) || nd->op == IR_SUSPEND || (nd->op == IR_DISJUNCTION && zls_dj_retains(nd)))) r = 1;
    }
    return r;
}
extern "C" int planner_bound_spine(const IR_graph_t * g, const IR_t * nd) {
    if (!g || !nd || nd->op != IR_BOUND || !zls_g_home_frame(g) || !zls_g_resumable_frame(g)) return 0;
    const IR_t * ig = zd_chase((IR_t *)nd->ω.node);
    if (!ig || ig->op != IR_GATE) return 0;
    int fence = zls_fcn_bs_get(nd, 0);
    if (!fence) {
        for (int k = 0; k < g->n && fence != 2; k++) { const IR_t * u = g->all[k]; if (u && u->op == IR_UNMARK && u->n_operands > 0 && u->operands[0] == nd && IR_LIT(u).ival == 2) fence = 2; }
        if (fence != 2) fence = 1;
        zls_fcn_bs_set(nd, 0, fence);
    }
    if (fence == 1) return 0;
        std::vector<const IR_t *> seen(1, nd), stk(1, nd);
    int r = 0;
    while (!stk.empty() && !r) {
        const IR_t * x = stk.back();
        stk.pop_back();
        if (x != nd && (zd_edge_retains_g(g, x) || x->op == IR_SUSPEND || x->op == IR_DISJUNCTION || x->op == IR_BOUND || (x->op == IR_CUT && !IR_LIT(x).ival))) r = 1;
        if (r || (x != nd && x->op == IR_UNMARK && x->n_operands > 0 && x->operands[0] == nd)) continue;
        for (int e = -2; e < x->n_operands; e++) {
            const IR_t * y = e == -2 ? x->γ.node : e == -1 ? x->ω.node : x->operands[e];
            if (!y || y == ig || y == ig->γ.node || y == ig->ω.node || std::find(seen.begin(), seen.end(), y) != seen.end()) continue;
            seen.push_back(y);
            stk.push_back(y);
        }
    }
    if (!r) {
        int o = zls_fcn_bs_get(nd, 1);
        if (!o) {
            for (int k = 0; k < g->n && o != 2; k++) {
                const IR_t * x = g->all[k];
                if (!x || x == nd || std::find(seen.begin(), seen.end(), x) != seen.end()) continue;
                for (int e = -2; e < x->n_operands && o != 2; e++) {
                    const IR_t * y = e == -2 ? x->γ.node : e == -1 ? x->ω.node : x->operands[e];
                    if (y && y->op == IR_UNMARK && y->n_operands > 0 && y->operands[0] == nd && !(IR_LIT(y).ival & 32)) o = 2;
                }
            }
            if (o != 2) o = 1;
            zls_fcn_bs_set(nd, 1, o);
        }
        r = o == 2;
    }
    return r ? 0 : 1;
}
int planner_retains_emitter_view(int op) { return op == IR_PROC_GEN || op == IR_CALL_VALUE || op == IR_CALL_GEN || op == IR_ACTIVATE; }
extern "C" int planner_home_dyn(const IR_graph_t * g, const IR_t * nd) {
        int r = planner_retains_g(g, nd);
    if (r == PR_GATE_HOLDS || r == PR_DISJUNCTION_HOLDS || r == PR_DB_GENERATOR || r == PR_RETAINING_TARGET) {
        int u = zls_fcn_hunres(nd) && (int)ir_kind0(nd) == IR_CALL_PROC_STAGED && zls_g_resumable_frame(g);
        r = zls_fcn_hret(nd) ? PR_RETAINING_CALLEE : u ? PR_UNRESOLVED_CALLEE : 0;
    }
    return (r == PR_SUSPEND || r == PR_COEXPR || r == PR_UNKNOWN_CALLEE || r == PR_GENERATOR_CALLEE || r == PR_DEFERRED_PATTERN || r == PR_RETAINING_CALLEE || r == PR_UNRESOLVED_CALLEE) ? r : 0;
}
static int zd_beta_via_act(const IR_graph_t * cg, const IR_t * nd) { return zd_edge_retains_g(cg, nd) && nd->op != IR_TO; }
static int zd_pl_cut(const IR_graph_t * cg, const IR_t * nd) { return nd->op == IR_CUT && !IR_LIT(nd).ival && planner_resumable_frame(cg); }
static int zd_pl_bspine(const IR_graph_t * cg, const IR_t * nd) { return nd && nd->op == IR_BOUND && planner_bound_spine(cg, nd); }
static int zd_pl_rest_ival(const IR_graph_t * cg, const IR_t * u) {
    if (!(IR_LIT(u).ival & 6)) return 0;
    return !((IR_LIT(u).ival & 32) && u->n_operands > 0 && zd_pl_bspine(cg, u->operands[0]));
}
static int zd_pl_unmark_bound(const IR_graph_t * cg, IR_t ** nodes, int n, int i) {
    IR_t * nd = nodes[i];
    if (nd->op != IR_UNMARK || nd->n_operands < 1 || !planner_resumable_frame(cg) || !zd_pl_rest_ival(cg, nd)) return -1;
    return nidx(nodes, n, nd->operands[0]);
}
static int zd_pl_restores_head(const IR_graph_t * cg, IR_t ** nodes, int n, const int * claim, int h) { int ub = zd_pl_unmark_bound(cg, nodes, n, h);
    return ub >= 0 && claim[ub] >= 0 && claim[ub] != h;
    }
static int zd_pl_edges(IR_t * nd) { return 2 + ((nd->op == IR_DISJUNCTION || nd->op == IR_GATE) ? nd->n_operands : 0); }
static int zd_pl_land(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, int i, int e, int *q, int *beta) {
    IR_t * nd = nodes[i], * t = (IR_t *)0;
    int l = 0;
    *beta = 0;
    if (e == 0) {
        IR_t * ig = (nd->op == IR_GATE_ARM && !nd->γ.node && nd->n_operands > 1) ? nd->operands[1] : (IR_t *)0;
        t = zd_chase(ig ? ig->γ.node : nd->γ.node);
        *beta = ig ? port_sz_beta(ig->γ.sz) : port_sz_beta(zd_edge_sz(nd, 0));
        l = zout[i] - zgpop[i];
    }
    else if (e == 1) { t = zd_chase(nd->ω.node); *beta = port_sz_beta(zd_edge_sz(nd, 1)); l = zout[i] - zd_k_g(cg, nd) - zwpop[i]; }
    else if (nd->op == IR_GATE) {
        IR_t * arm = nd->operands[e - 2];
        t = (arm && arm->n_operands > 0) ? arm->operands[0] : (IR_t *)0;
        *beta = arm ? (int) arm->seal : 0;
        if (t && (t->op == IR_SUCCEED || t->op == IR_FAIL)) t = (IR_t *)0;
        l = zout[i];
    }
    else { t = nd->operands[e - 2]; l = zout[i]; }
    *q = t ? nidx(nodes, n, t) : -1;
    if (*q < 0 || !zon[*q]) { *q = -1; return 0; }
    return l;
}
static int zd_pl_dj_join(IR_t **nodes, int n, int i, int q) {
    if (q < 0 || nodes[q]->op != IR_DISJUNCTION) return 0;
    for (int a = 0; a < (int)IR_LIT(nodes[q]).ival; a++) if (zd_dj_arm_end(nodes, n, q, a) == i) return 1;
    return 0;
}
static int zd_pl_entry(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, int i, int e, int *l,
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
        if (ub >= 0) v = (claim[ub] == claim[i]) ? zout[ub] - zd_k_own_g(cg, nodes[ub]) : zout[i] - zd_k_own_g(cg, nodes[ub]);
    }
    IR_t * r = (e == 0 && nodes[i]->op == IR_GATE_ARM && nodes[i]->n_operands > 0 && (int) nodes[i]->seal) ? nodes[i]->operands[0] : (IR_t *)0;
    int rq = r ? nidx(nodes, n, r) : -1;
    if (rq >= 0 && zon[rq] && claim[rq] == claim[i]) v = zout[rq];
    int hq = claim[q];
    *l = v - (zout[q] - zd_k_g(cg, nodes[q])) + (zout[hq] - zd_k_g(cg, nodes[hq]));
    return hq;
}
void zd_pl_bases(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, int *base, unsigned char *known, int *aof,
    int *zcut, int *fam, int *fbase){
    int * rpo = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    int * ord = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    int * stk = (int *)alloca(sizeof(int) * (size_t)(2 * n + 2));
    unsigned char * st = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
    for (int i = 0; i < n; i++) { base[i] = 0; known[i] = 0; rpo[i] = -1; st[i] = 0; if (aof) aof[i] = -1; if (fam) { fam[i] = -1; fbase[i] = 0; } }
    if (n <= 0 || !zon[0] || claim[0] < 0) return;
    int pl = planner_resumable_frame(cg);
    int no = 0, sp = 0, nalt = cg ? cg->n_alts : 0, ew = 8;
    for (int i = 0; i < n; i++) if (zd_pl_edges(nodes[i]) > ew) ew = zd_pl_edges(nodes[i]);
    unsigned char * dead = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
    for (int i = 0; i < n; i++) dead[i] = (unsigned char)(zon[i] && claim[i] == i && i != 0);
    for (int i = 0; i < n; i++) for (int e = 0; zon[i] && e < zd_pl_edges(nodes[i]); e++) { int q, b; (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, i, e, &q, &b);
        if (q >= 0 && q != i) dead[q] = 0; }
    int stmtg = 0;
    for (int i = 0; i < n; i++) if (nodes[i]->op == IR_STATEMENT_BEGIN || nodes[i]->op == IR_SETEXIT_TEST || nodes[i]->op == IR_STMT_MARK) stmtg = 1;
    int * bst = (int *)alloca(sizeof(int) * (size_t)(n + 2));
    int * bfl = (int *)alloca(sizeof(int) * (size_t)(n + 2));
    int * bit = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    for (int i = 0; i < n + 2; i++) bst[i] = 0;
    for (int i = 0; i < n; i++) if (zon[i] && claim[i] >= 0 && claim[i] < n) bst[claim[i] + 1]++;
    for (int i = 0; i <= n; i++) bst[i + 1] += bst[i];
    for (int i = 0; i <= n; i++) bfl[i] = bst[i];
    for (int i = 0; i < n; i++) if (zon[i] && claim[i] >= 0 && claim[i] < n) bit[bfl[claim[i]]++] = i;
    auto absolute_pass = [&](const unsigned char * xi) {
    no = 0; sp = 0;
    for (int i = 0; i < n; i++) { base[i] = 0; known[i] = 0; rpo[i] = -1; st[i] = 0; if (aof) aof[i] = -1; }
    for (int a0 = 0; a0 <= nalt + n; a0++) {
        int a = (a0 < nalt) ? a0 : -1;
        int r = (a0 == nalt) ? claim[0] : -1;
        if (a >= 0 && cg->alt_entry && cg->alt_entry[a]) { int q = nidx(nodes, n, zd_chase(cg->alt_entry[a])); if (q >= 0 && zon[q] && claim[q] >= 0) r = claim[q]; }
        if (a0 > nalt && nodes[a0 - nalt - 1]->op == IR_CREATE && nodes[a0 - nalt - 1]->n_operands > 0) {
            int q = nidx(nodes, n, zd_chase(nodes[a0 - nalt - 1]->operands[0]));
            if (q >= 0 && zon[q] && claim[q] >= 0) r = claim[q];
        }
        if (r < 0 || st[r] || (xi && xi[r])) continue;
        known[r] = 1;
        if (aof) aof[r] = a;
        stk[sp++] = r; stk[sp++] = 0; st[r] = 1;
    while (sp > 0) {
        int h = stk[sp - 2], pos = stk[sp - 1], pushed = 0, hb = (h >= 0 && h < n) ? bst[h] : 0, hc = (h >= 0 && h < n) ? bst[h + 1] - bst[h] : 0;
        for (; pos < hc * ew && !pushed; pos++) {
            int i = bit[hb + pos / ew], e = pos % ew, l;
            if (e >= zd_pl_edges(nodes[i])) continue;
            int q = zd_pl_entry(cg, nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
            if (q < 0 || st[q] || (xi && xi[q])) continue;
            stk[sp - 1] = pos + 1; st[q] = 1; if (aof) aof[q] = aof[h]; stk[sp++] = q; stk[sp++] = 0; pushed = 1;
        }
        if (pushed) continue;
        st[h] = 2; ord[no++] = h; sp -= 2;
    }
    }
    for (int k = 0; k < no; k++) rpo[ord[no - 1 - k]] = k;
    for (int k = 0; k < no; k++) {
        int h = ord[no - 1 - k];
        int ub = (nodes[h]->op == IR_UNMARK && nodes[h]->n_operands > 0) ? nidx(nodes, n, nodes[h]->operands[0]) : -1;
        if (ub >= 0 && zon[ub] && claim[ub] >= 0 && claim[ub] != h && known[claim[ub]] && !(xi && xi[claim[ub]])) { base[h] = base[claim[ub]] + zout[ub] - zout[h]; known[h] = 1; }
        if (!known[h] || (xi && xi[h])) continue;
        for (int i = 0; i < n; i++) {
            if (claim[i] != h || !zon[i]) continue;
            for (int e = 0; e < zd_pl_edges(nodes[i]); e++) {
                int l, q = zd_pl_entry(cg, nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                if (q < 0 || (xi && xi[q]) || rpo[q] <= rpo[h]) continue;
                if (stmtg) { base[q] = 0; known[q] = 1; continue; }
                int c = base[h] + l - (zout[q] - zd_k_g(cg, nodes[q]));
                if (zd_icn_stmt_sibling(cg, nodes[q]) && zd_icn_stmt_head(cg, nodes[h])) { c = base[h]; for (int s = 0; s < n; s++) if (claim[s] == h && zon[s]) c += zst_s_of(nodes[s]); }
                if (cg && cg->home_frame) for (int s = 0; s < n; s++)
                    if (claim[s] == h && zon[s] && zst_s_of(nodes[s]) > 0 && base[h] + zout[s] - zd_k_g(cg, nodes[s]) + zst_s_of(nodes[s]) > c)
                    c = base[h] + zout[s] - zd_k_g(cg, nodes[s]) + zst_s_of(nodes[s]);
                if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-BASES] h=%d i=%d e=%d q=%d l=%d c=%d known=%d base=%d\n", h, i, e, q, l, c, known[q], base[q]);
                if (known[q] && ((cg && cg->spine_cells) ? c >= base[q] : c <= base[q])) continue;
                base[q] = c;
                known[q] = 1;
            }
        }
    }
    };
    absolute_pass((const unsigned char *)0);
    {
        unsigned char * indef = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
        int * wl = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
        int wn = 0;
        for (int i = 0; i < n; i++) indef[i] = 0;
        for (int i = 0; i < n; i++) {
            if (!zon[i] || claim[i] < 0 || !zd_edge_retains_g(cg, nodes[i])) continue;
            IR_t * t = zd_chase(nodes[i]->γ.node);
            int q = t ? nidx(nodes, n, t) : -1;
            if (q == i) continue;
            for (int cur = i, hop = 0; q >= 0 && hop <= n && zd_pl_dj_join(nodes, n, cur, q); hop++) { IR_t * t2 = zd_chase(nodes[q]->γ.node); cur = q; q = t2 ? nidx(nodes, n, t2) : -1; }
            if (q < 0 || !zon[q] || claim[q] < 0 || indef[claim[q]]) continue;
            int ub = (pl && claim[q] == q && nodes[q]->op == IR_UNMARK && zd_pl_rest_ival(cg, nodes[q]) && nodes[q]->n_operands > 0) ? nidx(nodes, n, nodes[q]->operands[0]) : -1;
            if (ub >= 0 && zon[ub] && claim[ub] >= 0 && known[claim[ub]]) continue;
            indef[claim[q]] = 1; wl[wn++] = claim[q];
        }
        while (wn > 0) {
            int h = wl[--wn];
            for (int i = 0; i < n; i++) {
                if (claim[i] != h || !zon[i]) continue;
                for (int e = 0; e < zd_pl_edges(nodes[i]); e++) {
                    int q, b;
                    if (e == 1 || (e >= 2 && nodes[i]->op != IR_DISJUNCTION)) continue;
                    (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, i, e, &q, &b);
                    if (q < 0 || claim[q] < 0 || indef[claim[q]] || (b && zd_beta_via_act(cg, nodes[q])) || (e == 0 && zd_pl_dj_join(nodes, n, i, q))) continue;
                    int r = claim[q];
                    if (r != q && !(pl && nodes[q]->op != IR_DISJUNCTION)) continue;
                    int ub = (r == q && nodes[r]->op == IR_UNMARK && zd_pl_rest_ival(cg, nodes[r]) && nodes[r]->n_operands > 0) ? nidx(nodes, n, nodes[r]->operands[0]) : -1;
                    if (ub >= 0 && zon[ub] && claim[ub] >= 0 && !indef[claim[ub]] && known[claim[ub]]) continue;
                    indef[r] = 1; wl[wn++] = r;
                }
            }
        }
        for (int r = 0; r < n; r++) if (indef[r]) known[r] = 0;
        if (pl) absolute_pass(indef);
        for (int h = 0; h < n; h++) {
            if (!zon[h] || claim[h] != h || known[h] || indef[h] || nodes[h]->op != IR_SCAN || nodes[h]->n_operands < 1 || !nodes[h]->operands[0]) continue;
            int q = nidx(nodes, n, nodes[h]->operands[0]);
            if (q >= 0 && zon[q] && claim[q] >= 0 && known[claim[q]] && !indef[claim[q]]) { base[h] = base[claim[q]] + zout[q] - (zout[h] - zd_k_g(cg, nodes[h])); known[h] = 1; }
        }
        if (pl) {
            for (int pass = 0, moved = 1; moved && pass < n + 2; pass++) {
                moved = 0;
                for (int h = 0; h < n; h++) {
                    if (!zon[h] || claim[h] != h || known[h] || indef[h]) continue;
                    int ub = (nodes[h]->op == IR_UNMARK && nodes[h]->n_operands > 0) ? nidx(nodes, n, nodes[h]->operands[0]) : -1;
                    if (ub >= 0 && zon[ub] && claim[ub] >= 0 && claim[ub] != h && known[claim[ub]] && !indef[claim[ub]]) {
                        base[h] = base[claim[ub]] + zout[ub] - zout[h]; known[h] = 1; moved = 1; continue;
                    }
                    for (int i = 0; i < n && !known[h]; i++) {
                        if (!zon[i] || claim[i] < 0 || claim[i] == h || !known[claim[i]] || indef[claim[i]]) continue;
                        for (int e = 0; e < zd_pl_edges(nodes[i]); e++) {
                            int l, q = zd_pl_entry(cg, nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                            if (q != h) continue;
                            base[h] = base[claim[i]] + l - (zout[h] - zd_k_g(cg, nodes[h])); known[h] = 1; moved = 1; break;
                        }
                    }
                }
            }
            for (int h = 0; h < n; h++) if (zon[h] && claim[h] == h && !known[h] && !indef[h]) indef[h] = 1;
        }
        {
            unsigned char * open = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
            for (int pass = 0, moved = 1; moved && pass < n + 2; pass++) {
                moved = 0;
                for (int r = 0; r < n; r++) open[r] = (unsigned char)(zon[r] && claim[r] == r && !known[r] && !indef[r]);
                for (int i = 0; i < n; i++) {
                    if (!zon[i] || claim[i] < 0 || !known[claim[i]] || indef[claim[i]]) continue;
                    for (int e = 0; e < zd_pl_edges(nodes[i]); e++) {
                        int l, q = zd_pl_entry(cg, nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, open);
                        if (q < 0 || !open[q] || known[q]) continue;
                        base[q] = base[claim[i]] + l - (zout[q] - zd_k_g(cg, nodes[q])); known[q] = 1; moved = 1;
                    }
                }
            }
        }
        if (pl && fam) for (int ph = 0; ph < 2; ph++) {
            for (int i = 0; i < n; i++) {
                if (!zon[i] || claim[i] < 0 || !zd_edge_retains_g(cg, nodes[i])) continue;
                int q, b;
                (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, i, 0, &q, &b);
                if (q < 0 || !zon[q] || claim[q] < 0 || known[claim[q]] || fam[claim[q]] >= 0 || (claim[q] == claim[i]) != (ph == 1)) continue;
                fam[claim[q]] = claim[q]; fbase[claim[q]] = 0 - (zout[q] - (b ? 0 : zd_k_g(cg, nodes[q])));
            }
            for (int pass = 0, moved = 1; moved && pass < n + 2; pass++) {
                moved = 0;
                for (int h = 0; h < n; h++) {
                    if (!zon[h] || claim[h] != h || fam[h] < 0) continue;
                    for (int i = 0; i < n; i++) {
                        if (!zon[i] || claim[i] != h) continue;
                        for (int e = 0; e < zd_pl_edges(nodes[i]); e++) {
                            int l, q = zd_pl_entry(cg, nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                            if (q >= 0 && !known[q] && fam[q] < 0 && !zd_pl_restores_head(cg, nodes, n, claim, q)) { fam[q] = fam[h]; fbase[q] = fbase[h] + l - (zout[q] - zd_k_g(cg, nodes[q]));
                                moved = 1; }
                            if (e == 1 || (e >= 2 && nodes[i]->op != IR_DISJUNCTION) || zd_edge_retains_g(cg, nodes[i])) continue;
                            int b2, q2;
                            (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, i, e, &q2, &b2);
                            if (q2 < 0 || !zon[q2] || claim[q2] < 0 || claim[q2] == h || known[claim[q2]] || fam[claim[q2]] >= 0 || (b2 && zd_beta_via_act(cg, nodes[q2])) ||
                                zd_pl_restores_head(cg, nodes, n, claim, claim[q2])) continue;
                            int ld = (nodes[i]->op == IR_GATE_ARM) ? 0 : fbase[h] + zout[i] - zgpop[i];
                            fam[claim[q2]] = fam[h]; fbase[claim[q2]] = ld - (zout[q2] - (b2 ? 0 : zd_k_g(cg, nodes[q2]))); moved = 1;
                        }
                    }
                }
                for (int h = 0; h < n; h++) {
                    if (!zon[h] || claim[h] != h || known[h] || (fam[h] >= 0 && (fam[h] != h || (nodes[h]->op == IR_UNMARK && (IR_LIT(nodes[h]).ival & 8))))) continue;
                    int ub = (nodes[h]->op == IR_UNMARK && nodes[h]->n_operands > 0) ? nidx(nodes, n, nodes[h]->operands[0]) : -1;
                    if (ub < 0 || !zon[ub] || claim[ub] < 0 || claim[ub] == h || fam[claim[ub]] < 0 || fam[claim[ub]] == h) continue;
                    int f = fam[claim[ub]], nb = fbase[claim[ub]] + zout[ub] - zout[h], sh = (fam[h] == h) ? nb - fbase[h] : 0;
                    if (fam[h] == h) for (int r = 0; r < n; r++) if (fam[r] == h) { fam[r] = f; fbase[r] += sh; }
                    fam[h] = f; fbase[h] = nb; moved = 1;
                }
            }
        }
        if (pl && fam && zd_map_on()) for (int r = 0; r < n; r++) if (zon[r] && claim[r] == r && fam[r] >= 0) fprintf(stderr, "[ZD-MAP] FAM run=%d head=%d fbase=%d\n", r, fam[r], fbase[r]);
        int * anchor = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
        for (int r = 0; r < n; r++) anchor[r] = -1;
        for (int pass = 0, moved = 1; moved && pass < n + 2; pass++) {
            moved = 0;
            for (int i = 0; i < n; i++) {
                if (!zon[i] || claim[i] < 0) continue;
                int h = claim[i], a = -1, q = -1;
                if (zd_edge_retains_g(cg, nodes[i])) { IR_t * t = zd_chase(nodes[i]->γ.node); q = t ? nidx(nodes, n, t) : -1; a = known[h] ? base[h] + zout[i] : anchor[h];
                    if (pl && q >= 0 && zon[q] && claim[q] >= 0) q = claim[q]; }
                else if (indef[h] && anchor[h] >= 0) {
                    for (int e = 0; e < zd_pl_edges(nodes[i]); e++) { int b2, q2; if (e == 1 || (e >= 2 && nodes[i]->op != IR_DISJUNCTION)) continue;
                        (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, i, e, &q2, &b2);
                        if (pl && q2 >= 0 && zon[q2] && claim[q2] >= 0) q2 = claim[q2];
                        if (q2 >= 0 && claim[q2] == q2 && indef[q2] && anchor[q2] < 0) { anchor[q2] = anchor[h]; moved = 1; } }
                    continue; }
                if (q < 0 || !zon[q] || claim[q] != q || a < 0 || !indef[q] || anchor[q] >= 0) continue;
                anchor[q] = a; moved = 1;
            }
        }
        if (zcut) for (int i = 0; i < n; i++) zcut[i] = -1;
        unsigned char * inp = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
        for (int c = 0; c < n; c++) {
            if (!zon[c] || claim[c] < 0 || !zd_pl_cut(cg, nodes[c])) continue;
            int h = claim[c], D = known[h] ? base[h] + zout[c] - zd_k_g(cg, nodes[c]) : anchor[h];
            {
                IR_t * jt = zd_chase(nodes[c]->γ.node);
                int j = jt ? nidx(nodes, n, jt) : -1;
                if (j >= 0 && zon[j] && claim[j] >= 0 && known[claim[j]] && !indef[claim[j]] && zd_pl_dj_join(nodes, n, c, j)) D = base[claim[j]] + zout[j] + zgpop[c];
            }
            if (zcut) zcut[c] = D;
            if (known[h] || D < 0) continue;
            IR_t * t = zd_chase(nodes[c]->γ.node);
            int q = t ? nidx(nodes, n, t) : -1;
            if (q < 0 || !zon[q] || claim[q] < 0 || claim[q] == h || known[claim[q]]) continue;
            int r0 = claim[q], wn2 = 0;
            for (int r = 0; r < n; r++) inp[r] = 0;
            inp[r0] = 1; wl[wn2++] = r0;
            while (wn2 > 0) {
                int r = wl[--wn2];
                for (int i = 0; i < n; i++) for (int e = 0; claim[i] == r && zon[i] && e < zd_pl_edges(nodes[i]); e++) {
                    int l, q2 = zd_pl_entry(cg, nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                    if (q2 >= 0 && !known[q2] && !inp[q2]) { inp[q2] = 1; wl[wn2++] = q2; }
                }
            }
            auto rest = [&](int r) { int ub = (nodes[r]->op == IR_UNMARK && zd_pl_rest_ival(cg, nodes[r]) && nodes[r]->n_operands > 0) ? nidx(nodes, n, nodes[r]->operands[0]) : -1;
                return ub >= 0 && zon[ub] && claim[ub] >= 0 && (inp[claim[ub]] || (known[claim[ub]] && !indef[claim[ub]])); };
            for (int moved = 1; moved; ) {
                moved = 0;
                for (int i = 0; i < n; i++) {
                    IR_t * rt = (zon[i] && claim[i] >= 0 && zd_edge_retains_g(cg, nodes[i])) ? zd_chase(nodes[i]->γ.node) : (IR_t *)0;
                    int rq = rt ? nidx(nodes, n, rt) : -1;
                    if (rq >= 0 && zon[rq] && claim[rq] >= 0 && inp[claim[rq]] && !rest(claim[rq])) { inp[claim[rq]] = 0; moved = 1; }
                }
                for (int i = 0; i < n; i++) for (int e = 0; zon[i] && claim[i] >= 0 && !inp[claim[i]] && e < zd_pl_edges(nodes[i]); e++) {
                    int l, q2 = zd_pl_entry(cg, nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                    if (q2 < 0 || q2 == r0 || !inp[q2] || (i == c && e == 0) || (known[claim[i]] && !indef[claim[i]] && !zd_edge_retains_g(cg, nodes[i])) || rest(q2)) continue;
                    inp[q2] = 0; moved = 1;
                }
            }
            if (!inp[r0]) continue;
            base[r0] = D - (zout[q] - zd_k_g(cg, nodes[q])); known[r0] = 1; indef[r0] = 0;
            wl[wn2++] = r0;
            while (wn2 > 0) {
                int r = wl[--wn2];
                for (int i = 0; i < n; i++) {
                    if (claim[i] != r || !zon[i]) continue;
                    for (int e = 0; e < zd_pl_edges(nodes[i]); e++) {
                        int l, q2 = zd_pl_entry(cg, nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                        if (q2 < 0 || known[q2] || !inp[q2]) continue;
                        int ub = (nodes[q2]->op == IR_UNMARK && nodes[q2]->n_operands > 0) ? nidx(nodes, n, nodes[q2]->operands[0]) : -1;
                        base[q2] = (ub >= 0 && zon[ub] && claim[ub] >= 0 && claim[ub] != q2 && known[claim[ub]] && !indef[claim[ub]]) ? base[claim[ub]] + zout[ub] - zout[q2]
                                 : base[r] + l - (zout[q2] - zd_k_g(cg, nodes[q2]));
                        known[q2] = 1; indef[q2] = 0; wl[wn2++] = q2;
                    }
                }
            }
        }
        for (int r = 0; r < n; r++) if (zon[r] && claim[r] == r && !known[r] && nodes[r]->op == IR_MATCH_ABORT && claim[0] >= 0 && known[claim[0]]) { base[r] = base[claim[0]]; known[r] = 1; }
        if (zd_map_on()) for (int r = 0; r < n; r++) if (indef[r])
            fprintf(stderr, "[ZD-MAP] INDEF run=%d (%s) anchor=%d: entered after a call that may return with its frame live -- no base, its cells rsp-relative within the run only\n", r,
            bb_op_name(nodes[r]->op), anchor[r]);
        if (zd_map_on()) for (int c = 0; c < n; c++) if (zon[c] && claim[c] >= 0 && zd_pl_cut(cg, nodes[c]))
            fprintf(stderr, "[ZD-MAP] CUT i=%d restores rsp to depth %d\n", c, zcut ? zcut[c] : known[claim[c]] ? base[claim[c]] + zout[c] - zd_k_g(cg, nodes[c]) : anchor[claim[c]]);
        if (aof) for (int pass = 0, moved = 1; moved && pass < n + 2; pass++) {
            moved = 0;
            for (int i = 0; i < n; i++) {
                if (!zon[i] || claim[i] < 0 || aof[claim[i]] < 0) continue;
                for (int e = 0; e < zd_pl_edges(nodes[i]); e++) {
                    int lb, lq; (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, i, e, &lq, &lb);
                    if (lq >= 0 && claim[lq] >= 0 && aof[claim[lq]] < 0) { aof[claim[lq]] = aof[claim[i]]; moved = 1; }
                }
            }
        }
    }
}
int zst_s_of(const IR_t * nd) { if (!nd || g_planner.zst_sm.empty()) return 0; auto it = g_planner.zst_sm.find(nd); return it == g_planner.zst_sm.end() ? 0 : it->second; }
static int zst_edge(IR_t ** nodes, int n, int i, int e) {
    IR_t * nd = nodes[i], * t = (IR_t *)0;
    if (e == 0) { IR_t * ig = (nd->op == IR_GATE_ARM && !nd->γ.node && nd->n_operands > 1) ? nd->operands[1] : (IR_t *)0; t = zd_chase(ig ? ig->γ.node : nd->γ.node); }
    else if (e == 1) t = zd_chase(nd->ω.node);
    else if (nd->op == IR_GATE) { IR_t * arm = nd->operands[e - 2]; if (!arm || (int) arm->seal || arm->n_operands < 1) return -1; t = arm->operands[0]; }
    else t = nd->operands[e - 2];
    return t ? nidx(nodes, n, t) : -1;
}
static int zst_names(const IR_t * nd, const char ** out) {
    int k = 0, o = (int)nd->op;
    if (o == IR_VAR || o == IR_VAR_REF || o == IR_ASSIGN || o == IR_ASSIGN_VAR || o == IR_REV_ASSIGN_VAR || o == IR_NULLTEST_VAR || o == IR_FIELD_VAR || o == IR_UNIFY_FIRST || o == IR_UNIFY_VALUE ||
        o == IR_REV_SWAP) out[k++] = IR_LIT(nd).sval;
    if (o == IR_REV_ASSIGN && nd->n_operands > 1 && nd->operands[1]) out[k++] = IR_LIT(nd->operands[1]).sval;
    if (o == IR_REV_SWAP && nd->n_operands > 0 && nd->operands[0]) out[k++] = IR_LIT(nd->operands[0]).sval;
    if ((o == IR_UNIFY_CONST || o == IR_UNIFY_STRUCT) && nd->n_operands > 0 && nd->operands[0] && nd->operands[0]->op == IR_VAR_REF) out[k++] = IR_LIT(nd->operands[0]).sval;
    if (o == IR_SCAN && nd->n_operands > 1 && nd->operands[1] && nd->operands[1]->op == IR_VAR && IR_LIT(nd->operands[1]).sval) out[k++] = IR_LIT(nd->operands[1]).sval;
    return k;
}
static int zst_dj_reads(const IR_t * nd, const char * vn) {
    if (!nd || nd->op != IR_DISJUNCTION) return 0;
    for (int j = 0, N = (int)IR_LIT(nd).ival; j < N && 2 * N + j < nd->n_operands; j++) {
        const IR_t * rj = nd->operands[2 * N + j];
        for (int g = 0; rj && rj->op == IR_CONJUNCTION && rj->n_operands > 0 && g < 16; g++) rj = rj->operands[0];
        if (rj && rj->op == IR_VAR && IR_LIT(rj).sval && !strcmp(IR_LIT(rj).sval, vn)) return 1;
    }
    return 0;
}
static int zst_isparam(const IR_graph_t * g, const char * vn) { for (int i = 0; g && g->pnames && i < g->nparams; i++) if (g->pnames[i] && !strcmp(g->pnames[i], vn)) return 1; return 0; }
static void zst_plan(const IR_graph_t * cg, IR_t ** nodes, int n) {
    g_planner.zst_nodes = nodes; g_planner.zst_n = n; g_planner.g_zd_cur = -1; g_planner.zst_s.assign((size_t)(n > 0 ? n : 0), 0); g_planner.zst_root.assign((size_t)(n > 0 ? n : 0), -1);
    g_planner.zst_name.clear(); g_planner.zst_vroot.clear(); g_planner.zst_c.clear(); g_planner.zst_j.clear(); g_planner.zst_L.clear(); g_planner.zst_vuse.clear(); g_planner.zst_vcov.clear();
        g_planner.zst_sm.clear();
    const IR_graph_t * g = cg;
    if (!g || n <= 0 || !(planner_resumable_frame(cg) || g->spine_cells || g->home_frame)) return;
    if (g->spine_cells && !g->home_frame) return;
    int nv = zls_g_vslot_count(g); if (nv <= 0) return;
    g_planner.zst_vuse.assign((size_t)nv, 0); g_planner.zst_vcov.assign((size_t)nv, 0);
    std::vector<int> es, ed, pcnt((size_t)n + 1, 0);
    for (int i = 0; i < n; i++) for (int e = 0; e < zd_pl_edges(nodes[i]); e++) { int q = zst_edge(nodes, n, i, e); if (q >= 0 && q != i) { es.push_back(i); ed.push_back(q); pcnt[(size_t)q + 1]++; } }
    for (int i = 0; i < n; i++) pcnt[(size_t)i + 1] += pcnt[(size_t)i];
    std::vector<int> preds(es.size() + 1), fill(pcnt);
    for (size_t k = 0; k < es.size(); k++) preds[(size_t)fill[(size_t)ed[k]]++] = es[k];
    std::vector<unsigned char> isroot((size_t)n, 0); isroot[0] = 1;
    for (int a = 0; g->alt_entry && a < g->n_alts; a++) if (g->alt_entry[a]) { int q = nidx(nodes, n, zd_chase(g->alt_entry[a])); if (q >= 0) isroot[(size_t)q] = 1; }
    size_t W = ((size_t)n + 63) / 64;
    std::vector<uint64_t> dom((size_t)n * W, ~0ull), tmp(W);
    for (int v = 0; v < n; v++) if (isroot[(size_t)v]) { for (size_t w = 0; w < W; w++) dom[(size_t)v * W + w] = 0; dom[(size_t)v * W + (size_t)v / 64] |= 1ull << (v % 64); }
    for (int pass = 0, changed = 1; changed && pass < 200; pass++) { changed = 0;
        for (int v = 0; v < n; v++) { if (isroot[(size_t)v]) continue; int np = pcnt[(size_t)v + 1] - pcnt[(size_t)v]; if (np == 0) continue;
            for (size_t w = 0; w < W; w++) tmp[w] = ~0ull;
            for (int k = pcnt[(size_t)v]; k < pcnt[(size_t)v + 1]; k++) { int p = preds[(size_t)k]; for (size_t w = 0; w < W; w++) tmp[w] &= dom[(size_t)p * W + w]; }
            tmp[(size_t)v / 64] |= 1ull << (v % 64);
            for (size_t w = 0; w < W; w++) if (dom[(size_t)v * W + w] != tmp[w]) { dom[(size_t)v * W + w] = tmp[w]; changed = 1; } } }
    std::vector<unsigned char> reach((size_t)n, 0);
    for (int v = 0; v < n; v++) { int np = pcnt[(size_t)v + 1] - pcnt[(size_t)v]; reach[(size_t)v] = (unsigned char)(isroot[(size_t)v] || np > 0); }
    for (int v = 0; v < n; v++) { if (!reach[(size_t)v]) continue; int r = -1, nr = 0;
        for (int u = 0; u < n; u++) if (isroot[(size_t)u] && (dom[(size_t)v * W + (size_t)u / 64] >> (u % 64) & 1)) { r = u; nr++; }
        g_planner.zst_root[(size_t)v] = nr == 1 ? r : -1; }
    std::vector<int> idx((size_t)n, -1), low((size_t)n, 0), st; std::vector<unsigned char> on((size_t)n, 0), loop((size_t)n, 0); int ctr = 0;
    std::vector<int> scnt((size_t)n + 1, 0), ss, sd; for (size_t k = 0; k < es.size(); k++) scnt[(size_t)es[k] + 1]++;
    for (int i = 0; i < n; i++) scnt[(size_t)i + 1] += scnt[(size_t)i]; { std::vector<int> f2(scnt); sd.assign(es.size() + 1, -1);
        for (size_t k = 0; k < es.size(); k++) sd[(size_t)f2[(size_t)es[k]]++] = ed[k]; }
    for (int s0 = 0; s0 < n; s0++) { if (idx[(size_t)s0] >= 0) continue; std::vector<std::pair<int,int>> cs; cs.push_back(std::make_pair(s0, scnt[(size_t)s0]));
        idx[(size_t)s0] = low[(size_t)s0] = ctr++; st.push_back(s0); on[(size_t)s0] = 1;
        while (!cs.empty()) { int v = cs.back().first; int & e = cs.back().second;
            if (e < scnt[(size_t)v + 1]) { int w = sd[(size_t)e]; e++; if (w == v) loop[(size_t)v] = 1; else if (idx[(size_t)w] < 0) { idx[(size_t)w] = low[(size_t)w] = ctr++; st.push_back(w);
                on[(size_t)w] = 1; cs.push_back(std::make_pair(w, scnt[(size_t)w])); }
                else if (on[(size_t)w] && low[(size_t)w] < low[(size_t)v]) low[(size_t)v] = low[(size_t)w]; continue; }
            cs.pop_back(); if (!cs.empty() && low[(size_t)v] < low[(size_t)cs.back().first]) low[(size_t)cs.back().first] = low[(size_t)v];
            if (low[(size_t)v] == idx[(size_t)v]) { std::vector<int> mem; int w; do { w = st.back(); st.pop_back(); on[(size_t)w] = 0; mem.push_back(w); } while (w != v);
                if (mem.size() > 1) for (size_t m = 0; m < mem.size(); m++) loop[(size_t)mem[m]] = 1; } } }
    std::vector<int> dsz((size_t)n, 0); for (int v = 0; v < n; v++) { int c = 0; for (size_t w = 0; w < W; w++) c += __builtin_popcountll(dom[(size_t)v * W + w]); dsz[(size_t)v] = c; }
    std::vector<unsigned char> indef0((size_t)n, 0);
    { std::vector<int> wl;
        for (int i = 0; i < n; i++) if (zd_edge_retains_g(cg, nodes[i])) { int q = zst_edge(nodes, n, i, 0);
            for (int cur = i, hop = 0; q >= 0 && hop <= n && zd_pl_dj_join(nodes, n, cur, q); hop++) { cur = q; q = zst_edge(nodes, n, q, 0); }
            if (q >= 0 && q != i && !indef0[(size_t)q]) { indef0[(size_t)q] = 1; if (nodes[q]->op != IR_CUT) wl.push_back(q); } }
        while (!wl.empty()) { int v = wl.back(); wl.pop_back();
            for (int e = 0; e < zd_pl_edges(nodes[v]); e++) { if (e == 1 || (e >= 2 && nodes[v]->op != IR_DISJUNCTION)) continue;
                int w = zst_edge(nodes, n, v, e); if (w < 0 || w == v || indef0[(size_t)w] || (e == 0 && zd_pl_dj_join(nodes, n, v, w))) continue;
                int ub = (nodes[w]->op == IR_UNMARK && zd_pl_rest_ival(cg, nodes[w]) && nodes[w]->n_operands > 0) ? nidx(nodes, n, nodes[w]->operands[0]) : -1;
                if (ub >= 0 && !indef0[(size_t)ub]) continue;
                indef0[(size_t)w] = 1; if (nodes[w]->op != IR_CUT) wl.push_back(w); } } }
    std::vector<int> bnds; std::vector<std::vector<unsigned char>> esc; std::vector<unsigned char> wland((size_t)n, 0);
    for (int i = 0; i < n; i++) { int q = zst_edge(nodes, n, i, 1); if (q >= 0) wland[(size_t)q] = 1; }
    for (int b = 0; b < n; b++) { if (nodes[b]->op != IR_BOUND) continue; std::vector<unsigned char> r((size_t)n, 0); std::vector<int> wl;
        for (int u = 0; u < n; u++) if (nodes[u]->op == IR_UNMARK && nodes[u]->n_operands > 0 && nodes[u]->operands[0] == nodes[b] && (zd_pl_rest_ival(cg, nodes[u]) || wland[(size_t)u])) {
            int q = zst_edge(nodes, n, u, 0);
            if (q >= 0 && !r[(size_t)q]) { r[(size_t)q] = 1; wl.push_back(q); } }
        while (!wl.empty()) { int v = wl.back(); wl.pop_back(); for (int k = scnt[(size_t)v]; k < scnt[(size_t)v + 1]; k++) { int w = sd[(size_t)k]; if (w >= 0 && !r[(size_t)w]) { r[(size_t)w] = 1;
            wl.push_back(w); } } }
        bnds.push_back(b); esc.push_back(r); }
    const char * nm[2];
    for (int k = 0; k < nv; k++) { int off = -1; const char * vn = zls_g_vslot_get(g, k, &off); if (!vn || vn[0] == '&' || zst_isparam(g, vn)) continue;
        std::vector<int> uses; for (int i = 0; i < n; i++) { int m = zst_names(nodes[i], nm); int hit = g->home_frame ? zst_dj_reads(nodes[i], vn) : 0;
            for (int q = 0; q < m && !hit; q++) if (nm[q] && !strcmp(nm[q], vn)) hit = 1;
            if (hit) uses.push_back(i); }
        { int past = 0; for (size_t u = 0; (g->spine_cells || g->home_frame) && u < uses.size() && !past; u++) if (zls_home_local_why(nodes[uses[u]])) past = 1; if (past) continue; }
        if (zls_home_local_test(g, vn) == 9) continue;
        std::vector<int> roots; for (size_t u = 0; u < uses.size(); u++) { int r = g_planner.zst_root[(size_t)uses[u]]; if (r < 0) continue; int seen = 0;
            for (size_t t = 0; t < roots.size(); t++) if (roots[t] == r) seen = 1; if (!seen) roots.push_back(r); }
        g_planner.zst_vuse[(size_t)k] = (int)roots.size();
        for (size_t t = 0; t < roots.size(); t++) { int r = roots[t]; for (size_t w = 0; w < W; w++) tmp[w] = ~0ull;
            for (size_t u = 0; u < uses.size(); u++) if (g_planner.zst_root[(size_t)uses[u]] == r) for (size_t w = 0; w < W; w++) tmp[w] &= dom[(size_t)uses[u] * W + w];
            int best = -1;
            for (int pass = 0; pass < (g->home_frame ? 2 : 1) && best < 0; pass++) for (int d = 0; d < n; d++) {
                if (!(tmp[(size_t)d / 64] >> (d % 64) & 1) || (loop[(size_t)d] && !(pass && zd_pl_home(cg))) || indef0[(size_t)d] || nodes[d]->op == IR_DISJUNCTION ||
                    (zd_k_own_g(cg, nodes[d]) <= 0 && !pass))
                    continue;
                int inside = 0;
                for (size_t bi = 0; bi < bnds.size() && !inside; bi++) {
                    int b = bnds[bi];
                    if (b == d || !(dom[(size_t)d * W + (size_t)b / 64] >> (b % 64) & 1) || esc[bi][(size_t)d]) continue;
                    for (size_t u = 0; u < uses.size() && !inside; u++) if (g_planner.zst_root[(size_t)uses[u]] == r && esc[bi][(size_t)uses[u]]) inside = 1;
                }
                if (inside) continue;
                if (best < 0 || dsz[(size_t)d] > dsz[(size_t)best]) best = d;
            }
            if (best < 0) continue;
            g_planner.zst_name.push_back(vn); g_planner.zst_vroot.push_back(r); g_planner.zst_c.push_back(best); g_planner.zst_j.push_back(g_planner.zst_s[(size_t)best] / 16);
                g_planner.zst_L.push_back(0);
            g_planner.zst_s[(size_t)best] += 16; g_planner.zst_vcov[(size_t)k]++; } }
    for (int i = 0; i < n; i++) if (g_planner.zst_s[(size_t)i] > 0) g_planner.zst_sm[nodes[i]] = g_planner.zst_s[(size_t)i];
}
void zst_finish(const IR_graph_t * cg, IR_t ** nodes, int n, const int * zout, const int * claim, const int * base, const unsigned char * known) {
    if (g_planner.zst_nodes != nodes || g_planner.zst_n != n) return;
    for (size_t t = 0; t < g_planner.zst_c.size(); t++) { int c = g_planner.zst_c[t]; int rk = claim && claim[c] >= 0 && known && known[claim[c]] && base;
        g_planner.zst_L[t] = rk ? base[claim[c]] + zout[c] - zd_k_g(cg, nodes[c]) + g_planner.zst_s[(size_t)c] - 16 * g_planner.zst_j[t] : INT_MIN; }
    if (zd_map_on()) for (size_t t = 0; t < g_planner.zst_c.size(); t++) { int c = g_planner.zst_c[t];
        fprintf(stderr, "[ZD-MAP] STICKY name=%s root=%d c=%d (%s zout=%d k=%d own=%d s=%d) j=%d L=%d\n", g_planner.zst_name[t], g_planner.zst_vroot[t], c, bb_op_name(nodes[c]->op), zout[c],
            zd_k_g(cg, nodes[c]), zd_k_own_g(cg, nodes[c]), g_planner.zst_s[(size_t)c], g_planner.zst_j[t], g_planner.zst_L[t]); }
}
int zst_coord(const char * name) {
    if (!name || g_planner.g_zd_cur < 0 || g_planner.g_zd_cur >= g_planner.zst_n || g_planner.zst_nodes != g_planner.codegen_flat_chain_body_nodes) return -1;
    int r = g_planner.zst_root[(size_t)g_planner.g_zd_cur]; if (r < 0) return -1;
    for (size_t t = 0; t < g_planner.zst_name.size(); t++) if (g_planner.zst_vroot[t] == r && g_planner.zst_L[t] != INT_MIN && !strcmp(g_planner.zst_name[t], name)) return ZST_BIAS -
        g_planner.zst_L[t];
    return -1;
}
static void zd_dj_discards(const IR_graph_t * cg, IR_t ** nodes, int n, const unsigned char * zon, const int * zout, const int * zgpop, const int * zwpop, const int * zarm, const int * claim,
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
static int zd_match_env_record(const IR_graph_t * cg, const IR_t * nd) { return nd->op == IR_MATCH_ALTERNATE ? 32 : nd->op == IR_MATCH_FENCE1 ? planner_fence_k(cg, nd) : 0; }
void zd_plan(const IR_graph_t * cg, IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, int *zarm, const unsigned char *walls, int *zcps, int *zcpd, int *zchn, int *zclaim) {
    extern const char * bb_src_of(const IR_t *);
    if (g_planner.zd_plan__dg < 0) { const char * d = getenv("SCRIP_ZD_DIAG"); g_planner.zd_plan__dg = (d && *d == '1') ? 1 : 0; }
    g_planner.g_zd_djx.assign((size_t)(n > 0 ? n : 0), 0);
    g_planner.g_zd_admit.assign((size_t)(n > 0 ? n : 1), 0);
    g_planner.g_zd_upost.assign((size_t)(n > 0 ? n : 0), 0);
    g_planner.g_zd_djx_nodes = nodes;
    zdo_build(nodes, n);
    zst_plan(cg, nodes, n);
    if (zclaim) for (int i = 0; i < n; i++) zclaim[i] = -1;
    IR_t ** zgt = (IR_t **)alloca(sizeof(IR_t *) * (size_t)n); IR_t ** zot = (IR_t **)alloca(sizeof(IR_t *) * (size_t)n);
    unsigned char * zgin = (unsigned char *)alloca((size_t)n); unsigned char * zoin = (unsigned char *)alloca((size_t)n); int * zmatch = (int *)alloca(sizeof(int) * (size_t)n);
    int * zstmt = (int *)alloca(sizeof(int) * (size_t)n); for (int _q = 0; _q < n; _q++) zstmt[_q] = -1;
    int * zmafter = (int *)alloca(sizeof(int) * (size_t)n); for (int _q = 0; _q < n; _q++) zmafter[_q] = -1;
    int * zwms = (int *)alloca(sizeof(int) * (size_t)n); for (int _q = 0; _q < n; _q++) zwms[_q] = 0;
    int * zurel = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1)); for (int _q = 0; _q < n; _q++) zurel[_q] = 0;
    for (int i = 0; i < n; i++) { zon[i] = 0; zout[i] = -1; zgpop[i] = 0; zwpop[i] = 0; if (zarm) zarm[i] = -1; zgt[i] = (IR_t *)0; zot[i] = (IR_t *)0; zgin[i] = 0; zoin[i] = 0; zmatch[i] = -1; }
    int * zarmj = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    int * djn = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    unsigned char * djok = g_planner.g_zd_admit.data();
    for (int i = 0; i < n; i++) { zarmj[i] = -1; djn[i] = 0; djok[i] = 0; if (zcps) zcps[i] = -1; if (zcpd) zcpd[i] = -1; if (zchn) { zchn[2 * i] = -1; zchn[2 * i + 1] = -1; } }
    const int zdj_on = zarm && zcps && cg && cg->spine_cells;
    const int zdj_arms = zarm && zcps && cg;
    { if(g_planner.zd_plan__lp<0){const char*e=getenv("SCRIP_ZDLOCAL");g_planner.zd_plan__lp=(e&&*e=='1')?1:0;} if(g_planner.zd_plan__lp){ extern int is_global(const char *);
        for (int i=0;i<n;i++){ int o=(int)nodes[i]->op;
        if(o==IR_VAR||o==IR_ASSIGN){ const char*vn=IR_LIT(nodes[i]).sval; int gl = (vn && is_global(vn) && !graph_has_local(cg, vn));
        if(!gl) fprintf(stderr,"[ZDLOCAL] %s name=%s\n", bb_op_name(nodes[i]->op), vn?vn:"<null>"); } } } }
    {
      if (n <= 0) return; }
    int * claim = (int *)alloca(sizeof(int) * (size_t)n); int * run = (int *)alloca(sizeof(int) * (size_t)n); int * rpos = (int *)alloca(sizeof(int) * (size_t)n);
        unsigned char * aent = (unsigned char *)alloca((size_t)n);
    unsigned char * cm = (unsigned char *)alloca((size_t)n); int * wl = (int *)alloca(sizeof(int) * (size_t)n);
    for (int i = 0; i < n; i++) { claim[i] = -1; rpos[i] = -1; }
    { if (zd_map_on()) { extern const char * zls_graph_name_get(const IR_graph_t *);
        fprintf(stderr, "[ZD-MAP] GRAPH n=%d name=%s -- i= is this nodes[] array, NOT a --dump-ir slot; g/o/ops resolve into the SAME array\n", n, cg ? zls_graph_name_get(cg) : "?");
        for (int i = 0; i < n; i++) { IR_t * gt = zd_chase(nodes[i]->γ.node); IR_t * ot = zd_chase(nodes[i]->ω.node); int gi = -1, oi = -1;
            for (int k = 0; k < n; k++) { if (nodes[k] == gt) gi = k; if (nodes[k] == ot) oi = k; }
            fprintf(stderr, "[ZD-MAP] i=%-3d %-22s nops=%d ops=[", i, bb_op_name(nodes[i]->op), nodes[i]->n_operands);
            for (int a = 0; a < nodes[i]->n_operands; a++) { int pi = -1;
                { int k = nidx(nodes, n, nodes[i]->operands[a]); if (k >= 0) { pi = k; } }
                fprintf(stderr, "%s%d", a ? "," : "", pi); }
            fprintf(stderr, "] g=%d o=%d lit/nameptr=%lld src=%s\n", gi, oi, (long long)IR_LIT(nodes[i]).ival, bb_src_of(nodes[i]) ? bb_src_of(nodes[i]) : "-"); } } }
    unsigned char * rjoin = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
    for (int k = 0; k < n; k++) rjoin[k] = 0;
    for (int k = 0; k < n; k++) if (zd_edge_retains_g(cg, nodes[k])) { int q = nidx(nodes, n, zd_chase(nodes[k]->γ.node)); if (q >= 0 && q != k && nodes[q]->op != IR_DISJUNCTION) rjoin[q] = 1; }
    for (int pass = 0; pass < 2; pass++) {
    for (int hi = 0; hi < n; hi++) {
        if (pass == 0) { }
        else { if (claim[hi] >= 0 || !zd_omega_head(nodes, n, nodes[hi])) continue; }
        if (claim[hi] >= 0) continue;
        int rl = 0; IR_t * cur = nodes[hi]; int guard = 0;
        while (cur && guard++ <= n) {
            int ci = -1; { int k = nidx(nodes, n, cur); if (k >= 0) { ci = k; } }
            if (ci < 0 || claim[ci] >= 0) break;
            if (rl > 0 && bb_src_of(nodes[ci]) && !zd_icn_suspend_g(cg, nodes[ci])) break; if (rl > 0 && walls && walls[ci]) break;
            if (rl > 0 && rjoin[ci]) break;
            { int _ub = zd_pl_unmark_bound(cg, nodes, n, ci); if (rl > 0 && _ub >= 0 && claim[_ub] != hi) break; }
            if (rl > 0 && ir_floater_kind(nodes[ci])) break;
            { int back = 0;
              for (int u = 0; nodes[ci]->op == IR_BOUND && u < rl && !back; u++) back = nodes[run[u]]->op == IR_UNMARK && nodes[run[u]]->n_operands > 0 && nodes[run[u]]->operands[0] == nodes[ci];
              if (back) break; }
            run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; rl++;
            if (zd_pl_cut(cg, nodes[ci])) break;
            if (zd_edge_retains_g(cg, nodes[ci])) break;
            cur =zd_icn_suspend_g(cg, cur) ? zd_suspend_run_next(nodes, n, cur, claim) : zd_chase(cur->γ.node);
        }
        { for (int k = 0; k < n; k++) aent[k] = 0;
          if (zarm) {
            for (int r0 = 0; r0 < rl; r0++) { IR_t * env = nodes[run[r0]]; if ((int)env->op != IR_MATCH_ALTERNATE && (int)env->op != IR_MATCH_ARBNO && (int)env->op != IR_MATCH_FENCE1) continue;
                int ei = run[r0];
                for (int a = 0; a + 1 < env->n_operands; a += 2) { IR_t * c2 = env->operands[a]; int rl0 = rl; int bad = 0; int g2 = 0; int first = 1;
                    while (c2 && g2++ <= n) {
                        int ci = -1; { int k = nidx(nodes, n, c2); if (k >= 0) { ci = k; } }
                        if (ci < 0 || claim[ci] >= 0) break;
                        if (bb_src_of(nodes[ci]) || ir_floater_kind(nodes[ci])) { bad = 1; break; }
                        run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; zarm[ci] = ei; if (first) { aent[ci] = 1; first = 0; } rl++;
                        c2 = zd_chase(c2->γ.node);
                    }
                    if (bad) { for (int u = rl0; u < rl; u++) { int ci = run[u]; claim[ci] = -1; rpos[ci] = -1; zarm[ci] = -1; aent[ci] = 0; } rl = rl0; }
                } } } }
        if (zdj_arms) {
            for (int r0 = 0; r0 < rl; r0++) {
                int ei = run[r0];
                IR_t * env = nodes[ei];
                int na = (int)IR_LIT(env).ival;
                if ((int)env->op != IR_DISJUNCTION || na <= 0 || env->n_operands < 3 * na) continue;
                int rl0 = rl;
                int bad = 0;
                for (int a = 0; a < na && !bad; a++) {
                    IR_t * rs = env->operands[2 * a + 1];
                    if (!rs || (zdj_on ? 0 : rs != env)) { bad = 1; break; }
                    IR_t * c2 = env->operands[2 * a];
                    if (c2 && c2->op == IR_FAIL && (rs == env || (zdj_on && rs->op == IR_FAIL)) && nidx(nodes, n, c2) < 0) {
                        if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d is a bare fail: no box, its entry is the chain's omega\n", ei, a);
                        continue;
                    }
                    int first = 1, g2 = 0, phi = 0;
                    while (c2 && c2 != env && g2++ <= n) {
                        int ci = nidx(nodes, n, c2);
                        if (first && ci >= 0 && claim[ci] < 0 && zd_icn_stmt_head(cg, nodes[ci])) {
                            if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d opens with a statement: no box claimed, its runs reach the box by sigma and phi\n", ei, a);
                            first = 0;
                            c2 = (IR_t *)0;
                            break;
                        }
                        if (!first && ci >= 0 && claim[ci] == hi && zarm[ci] == ei && zarmj[ci] == a) {
                            if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d loops back to ci=%d: the arm's own loop, it leaves by phi\n", ei, a, ci);
                            c2 = (IR_t *)0;
                            phi = 1;
                            break;
                        }
                        if (ci < 0 && !first && (c2->op == IR_SUCCEED || c2->op == IR_FAIL)) { c2 = (IR_t *)0; break; }
                        if (zdj_on && ci >= 0 && !first && claim[ci] < 0 && (bb_src_of(nodes[ci]) || (zd_omega_head(nodes, n, nodes[ci]) && !(cg && cg->spine_cells)))) { c2 =
                            (IR_t *)0; break; }
                        if (zdj_on && !first && ci >= 0 && claim[ci] >= 0 && (claim[ci] != hi || ((zarm[ci] != ei || zarmj[ci] != a) && (nodes[ci] == zd_chase(env->γ.node) || (cg &&
                            (cg->spine_cells || (cg->home_frame && nodes[ci]->op == IR_UNMARK))))))) {
                            if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d leaves the box into ci=%d of run %d\n", ei, a, ci, claim[ci]);
                            c2 = (IR_t *)0;
                            break;
                        }
                        if (ci < 0 || claim[ci] >= 0 || bb_src_of(nodes[ci]) || ir_floater_kind(nodes[ci]) || (walls && walls[ci])) { bad = 1;
                            if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d stops at ci=%d (claim=%d src=%d floater=%d wall=%d)\n", ei, a, ci, ci >= 0 ? claim[ci] : -2,
                                ci >= 0 ? (bb_src_of(nodes[ci]) != 0) : -1, ci >= 0 ? ir_floater_kind(nodes[ci]) : -1, (ci >= 0 && walls) ? walls[ci] : -1); break; }
                        if (planner_resumable_frame(cg) && zd_edge_retains_g(cg, nodes[ci]) && zd_chase(c2->γ.node) != env) {
                            if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d holds a retaining ci=%d before its end: the disjunction is not admitted\n", ei, a, ci);
                            bad = 1;
                            break;
                        }
                        run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; zarm[ci] = ei; zarmj[ci] = a; aent[ci] = (unsigned char)first; first = 0; rl++;
                        if (g_planner.zd_plan__dg)
                            fprintf(stderr, "[ZD-DJ] i=%d arm %d claims ci=%d %s rawgamma=%s\n", ei, a, ci, bb_op_name(nodes[ci]->op), c2->γ.node ? bb_op_name(c2->γ.node->op) : "-");
                        if (zdj_on && c2->γ.node && c2->γ.node->op == IR_GOTO && zd_chase(c2->γ.node) != env) { c2 = (IR_t *)0; break; }
                        if (zdj_on && c2->op == IR_GOTO) {
                            int leaves = 1;
                            IR_t * t = zd_chase(c2->γ.node);
                            for (int g4 = 0; t && g4 <= n; g4++) {
                                int tk = nidx(nodes, n, t);
                                if (t == env || (tk >= 0 && claim[tk] < 0 && bb_src_of(nodes[tk]))) { leaves = 0; break; }
                                if (tk < 0 || claim[tk] >= 0) break;
                                t = zd_chase(t->γ.node);
                            }
                            if (leaves) {
                                if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d leaves the box at its GOTO ci=%d (next, break)\n", ei, a, ci);
                                c2 = (IR_t *)0;
                                break;
                            }
                        }
                        c2 = zd_chase(c2->γ.node);
                    }
                    if (zdj_on && !bad && !first && (c2 == env || phi) && cg && cg->spine_cells && rl > rl0) {
                        IR_t * c3 = zd_chase(nodes[run[rl - 1]]->ω.node);
                        for (int u = rl - 1; phi && u >= rl0; u--) {
                            IR_t * o3 = zd_chase(nodes[run[u]]->ω.node);
                            int k3 = o3 ? nidx(nodes, n, o3) : -1;
                            if (k3 >= 0 && claim[k3] < 0) { c3 = o3; break; }
                        }
                        int rl1 = rl, g3 = 0;
                        while (c3 && c3 != env && g3++ <= n) {
                            int ci = nidx(nodes, n, c3);
                            if (ci < 0 || claim[ci] >= 0 || bb_src_of(nodes[ci]) || ir_floater_kind(nodes[ci]) || (walls && walls[ci])) break;
                            run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; zarm[ci] = ei; zarmj[ci] = a; aent[ci] = 0; rl++;
                            if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d claims ci=%d %s by omega\n", ei, a, ci, bb_op_name(nodes[ci]->op));
                            c3 = zd_chase(c3->γ.node);
                        }
                        if (!(c3 == env && rl > rl1)) { for (int u = rl1; u < rl; u++) { int ci = run[u]; claim[ci] = -1; rpos[ci] = -1; zarm[ci] = -1; zarmj[ci] = -1; aent[ci] = 0; } rl = rl1; }
                    }
                    if (first || (c2 && c2 != env)) bad = 1;
                }
                if (g_planner.zd_plan__dg && bad) fprintf(stderr, "[ZD-DJ] i=%d not admitted (na=%d claimed=%d)\n", ei, na, rl - rl0);
                if (bad && getenv("SCRIP_ZD_CHECK") && zd_pl_home(cg) && !zls_g_hf_resident(cg, nodes[ei]))
                    { fprintf(stderr, "[ZD-CHECK] FATAL DISJUNCTION i=%d is not admitted to the spine, does not retain and holds no frame record (zls_hf_resident)\n", ei); abort(); }
                if (bad) { for (int u = rl0; u < rl; u++) { int ci = run[u]; claim[ci] = -1; rpos[ci] = -1; zarm[ci] = -1; zarmj[ci] = -1; aent[ci] = 0; } rl = rl0; continue; }
                djok[ei] = 1;
                djn[ei] = rl - rl0;
            }
        }
        if (zdj_arms) {
            for (int r0 = rl - 1; r0 >= 0; r0--) {
                int d = run[r0], x = 0, cst = 1, dep = 0;
                if (nodes[d]->op != IR_DISJUNCTION || !djok[d] || !zls_dj_retains(nodes[d])) continue;
                for (int r = 0; r < rl && cst; r++) {
                    int i = run[r];
                    if (zarm[i] != d) continue;
                    if (aent[i]) dep = 0;
                    if (zd_edge_retains_g(cg, nodes[i]) || nodes[i]->op == IR_SUSPEND || (nodes[i]->op == IR_DISJUNCTION && zls_dj_retains(nodes[i]) && !zd_djx_of(nodes[i]))) cst = 0;
                    dep += zd_k_g(cg, nodes[i]);
                    if (zd_chase(nodes[i]->γ.node) == nodes[d] && zd_sz_sigma(zd_edge_sz(nodes[i], 0)) && zls_dj_arm_resumes(nodes[d], zarmj[i]) && dep > x) x = dep;
                }
                if (cst && x > 0) g_planner.g_zd_djx[(size_t)d] = x;
                if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-DJX] i=%d exit depth %d%s\n", d, x, cst ? "" : " (an arm's depth is not a constant: the record road)");
            }
        }
        for (int k = 0; k < n; k++) cm[k] = 0;
        g_planner.g_zd_zarm_p = zarm; g_planner.g_zd_zarm_nodes = nodes; g_planner.g_zd_zarm_n = n;
        int nblob = 0;
        { int wn = 0;
          for (int r = 0; r < rl; r++) { cm[run[r]] = 1; wl[wn++] = run[r]; }
          while (wn > 0) { int i = wl[--wn];
              for (int a = 0; a < nodes[i]->n_operands; a++) { IR_t * p = nodes[i]->operands[a]; if (!p) continue;
                  { int k = nidx(nodes, n, p); if (k >= 0 && (!cm[k])) { cm[k] = 1; wl[wn++] = k; nblob++; } } } } }
        int ok = (rl > 0); const char * why = ""; int badi = -1; int rgood = rl;
        (void)rgood;
        int vd_tidx = -1;
        if (ok && (pass == 1 || zd_vl_join(nodes[run[rl - 1]], cur))) { vd_tidx = zd_omega_test_idx(nodes, n, nodes[hi]);
            if (vd_tidx >= 0 && claim[vd_tidx] >= 0 && cur) { int _curi = -1; { int k = nidx(nodes, n, cur); if (k >= 0) { _curi = k; } }
                if (!(_curi >= 0 && (claim[_curi] == claim[vd_tidx] || (zon[vd_tidx] && zd_vl_sibling(nodes, n, claim, claim[vd_tidx], cur))))) vd_tidx = -1; }
            else vd_tidx = -1; }
        if (ok) { int zd = (vd_tidx >= 0) ? zd_omega_seed(nodes, n, nodes[hi], zon, zout, claim) : 0; int arm_zd = 0; int zdh_match = -1; int zdh_stmt = -1; int zdh_mafter = -1; int zwt = 0;
            if (nblob <= 0) zrm_build(nodes, run, rl);
            for (int r = 0; r < rl; r++) { int i = run[r];
                int REL = fence0_release_bytes(nodes[i]);
                int K = zd_k_g(cg, nodes[i]);
                int UREL = 0;
                { int _ub = zd_pl_unmark_bound(cg, nodes, n, i);
                    if (_ub >= 0 && claim[_ub] == hi && rpos[_ub] >= 0 && rpos[_ub] < r) UREL = (int)zd + K - (zout[_ub] - (zd_pl_bspine(cg, nodes[_ub]) ? zd_k_own_g(cg, nodes[_ub]) : 0));
                    else if (_ub >= 0 && claim[_ub] != hi && zd_pl_bspine(cg, nodes[_ub])) UREL = zd_k_own_g(cg, nodes[_ub]); }
                if (zd_pl_cut(cg, nodes[i])) UREL = (int)zd + K;
                zurel[i] = UREL;
                { if (nodes[i]->op == IR_MATCH_BEGIN && zdh_match < 0) zdh_match = (int)zd + K; } { if (nodes[i]->op == IR_STATEMENT_END) zdh_stmt = (int)zd + K; }
                { if (nodes[i]->op == IR_STATEMENT_BEGIN && r > 0 && zd > 0 && !(zarm && zarm[i] >= 0) ) { int rel =
                    zd_exit_pop_s(IR_STATEMENT_END, zdh_match, (int)zd, zdh_stmt, zdh_mafter); if (rel > 0) zgpop[run[r - 1]] += rel; zd = 0; } }
                { if (nodes[i]->op == IR_STATEMENT_BEGIN && r > 0) { zdh_match = -1; zdh_stmt = -1; zdh_mafter = -1; zwt = 0; } }
                { if (nodes[i]->op == IR_MATCH_BEGIN && zwt < n) zwms[zwt++] = (int)zd + K; }
                { if (nodes[i]->op == IR_MATCH_REPLACE || nodes[i]->op == IR_MATCH_END) zdh_mafter = (int)zd + K; }
                if (zarm && zarm[i] >= 0) { if (aent[i]) arm_zd = zout[zarm[i]] - (nodes[zarm[i]]->op == IR_DISJUNCTION ? zd_djx_of(nodes[zarm[i]]) : zd_k_g(cg, nodes[zarm[i]])); zon[i] = 1;
                    if (nodes[i]->op == IR_UNMARK && (IR_LIT(nodes[i]).ival & 16) && nodes[i]->n_operands > 0 && nodes[i]->operands[0] && nodes[i]->operands[0]->op == IR_BOUND) {
                        int _bk = nidx(nodes, n, nodes[i]->operands[0]);
                        if (_bk >= 0 && claim[_bk] == hi && zarm[_bk] == zarm[i] && rpos[_bk] >= 0 && rpos[_bk] < r) arm_zd = zout[_bk] - zd_k_g(cg, nodes[_bk]);
                    }
                    zout[i] = arm_zd + K;
                    arm_zd = arm_zd + K; }
                else { zon[i] = 1; zout[i] = zd + K - REL; zd = zd + K - REL - UREL; }
                { if (zwt == 0 && nodes[i]->op == IR_MATCH_REPLACE) { int _zu = 0, _me = 0; for (int _rr = r - 1; _rr >= 0; _rr--) { int _j = run[_rr];
                    if (nodes[_j]->op == IR_MATCH_END) { _me = 1; break; } _zu += zd_k_g(cg, nodes[_j]); } if (_me) { zd -= _zu; zdh_mafter = (int)zd; } } }
                { if (zwt > 0 && (nodes[i]->op == IR_MATCH_REPLACE || nodes[i]->op == IR_MATCH_END)) { zd = zwms[--zwt]; zdh_match = (int)zd; zdh_mafter = (int)zd; } }
                IR_t * gt = zd_chase(nodes[i]->γ.node); IR_t * ot = zd_chase(nodes[i]->ω.node);
                int gin = 0; int oin = 0; int gback = -1; int oback = -1;
                int gib = port_sz_beta(nodes[i]->γ.sz); { int _gg = 0; IR_t * _g = nodes[i]->γ.node; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!gib) gib = port_sz_beta(_g->γ.sz);
                    _g = _g->γ.node; } } gib = gib && !beta_is_stmt_land(gt);
                if (nblob > 0) { for (int k = 0; k < n; k++) if (cm[k]) { int _bk = claim[k] == hi && rpos[k] >= 0 && rpos[k] <= r; if (nodes[k] == gt && (!_bk || gib)) gin = 1;
                    if (nodes[k] == ot && (!_bk || port_sz_beta(nodes[i]->ω.sz)) && !(port_sz_beta(nodes[i]->ω.sz) && beta_is_stmt_land(ot))) oin = 1; } }
                else { int _mg = zrm_maxk(gt), _mo = zrm_maxk(ot);
                    if (_mg >= 0 && (_mg > r || gib)) gin = 1;
                    if (_mo >= 0 && (_mo > r || port_sz_beta(nodes[i]->ω.sz)) && !(port_sz_beta(nodes[i]->ω.sz) && beta_is_stmt_land(ot))) oin = 1; }
                if (gt) { { int tk = nidx(nodes, n, gt); if (tk >= 0 && (zon[tk])) { gback = tk; } } }
                if (ot) { { int tk = nidx(nodes, n, ot); if (tk >= 0 && (zon[tk])) { oback = tk; } } }
                { if (!gin && gt && gt->op == IR_SUCCEED && cg && cg->spine_cells && port_sz_beta(nodes[i]->ω.sz)) gin = 1; }
                { if (!gin && zd_icn_suspend_g(cg, nodes[i])) gin = 1; }
                { (void)0; }
                { int oib = port_sz_beta(nodes[i]->ω.sz); if (!oin && oib && K == 0 && !beta_is_stmt_land(ot)) oin = 1; }
                { if (!oin && K == 0) { int _io = !ot;
                  if (!_io && !(port_sz_beta(nodes[i]->ω.sz) && beta_is_stmt_land(ot))) { for (int _ik = 0; _ik <= r; _ik++) { if (nodes[run[_ik]] == ot) { _io = 1; break;
                      } } } if (_io) oin = 1; } }
                { zgt[i] = gt; zot[i] = ot; zgin[i] = (unsigned char)gin; zoin[i] = (unsigned char)oin; zmatch[i] = zdh_match; zstmt[i] = zdh_stmt; zmafter[i] = zdh_mafter;
                    }
                                { long kc = 0;
                int _wzdepth = (zarm && zarm[i] >= 0) ? zout[i] : (int)zd;
                int _gbpre = (gback >= 0) ? (zout[gback] - zd_k_g(cg, nodes[gback])) : 0;
                int _obpre = (oback >= 0) ? (zout[oback] - zd_k_g(cg, nodes[oback])) : 0;
                {
                if (!gin) zgpop[i] = (planner_resumable_frame(cg) && !(gback >= 0 && claim[gback] == claim[i])) ? 0 : (gback >= 0) ? (_wzdepth - _gbpre) :
                    (zd_exit_pop_s(nodes[i]->op, zdh_match, _wzdepth, zdh_stmt, zdh_mafter) + (int)kc);
                if (nodes[i]->op == IR_BOUND && !zd_pl_bspine(cg, nodes[i]) && (planner_resumable_frame(cg) || (cg && cg->spine_cells))) zwpop[i] = -K;
                else if (!oin) zwpop[i] = (oback >= 0) ? ((_wzdepth - K) - _obpre) :
                    (((nodes[i]->op == IR_GOTO_DEFERRED) ? zd_exit_pop_s(nodes[i]->op, zdh_match, _wzdepth, zdh_stmt, zdh_mafter) : (_wzdepth - K)) + (int)kc);
                else if (oback >= 0 && claim[oback] == claim[i] && rpos[oback] < rpos[i] && port_sz_beta(nodes[i]->ω.sz)) zwpop[i] = (_wzdepth - K) - zout[oback]; } }
                if (g_planner.zd_plan__dg)
                    fprintf(stderr, "[ZD] h=%d r=%d i=%d %s K=%d zout=%d gpop=%d wpop=%d gin=%d oin=%d gback=%d oback=%d\n", hi, r, i, bb_op_name(nodes[i]->op), K, zout[i], zgpop[i], zwpop[i], gin,
                    oin, gback, oback);
            }
        }
        else {
            for (int r = 0; r < rl; r++) rpos[run[r]] = -1;
               if (g_planner.zd_plan__dg && rl > 0) { extern const char * zls_graph_name_get(const IR_graph_t *);
                   fprintf(stderr, "[ZD] run h=%d len=%d REFUSED at i=%d (%s op=%d) graph=%s\n", hi, rl, badi, why, badi >= 0 ? (int)nodes[badi]->op : -1,
                   cg ? zls_graph_name_get(cg) : "?"); }
               { if (g_planner.zd_plan__gp < 0) { const char * _e = getenv("SCRIP_ZD_GAP"); g_planner.zd_plan__gp = (_e && _e[0] == '1') ? 1 : 0;
                   } if (g_planner.zd_plan__gp && rl > 0) { for (int _r = 0; _r < rl; _r++) { IR_t * _n = nodes[run[_r]]; const char * _nm = bb_op_name(_n->op); { int _o = (int)_n->op;
                   int _ic = (_o == (int)IR_CALL || (_o != (int)IR_CALL_VALUE && ir_is_call_kind((IR_e)_o))); const char * _cs = (_ic && IR_LIT(_n).sval) ? IR_LIT(_n).sval : "-";
                   fprintf(stderr, "[ZD-GAP] r=%d op=%s(%d) nops=%d callee=%s\n", _r, _nm ? _nm : "<unnamed>", _o, (int)_n->n_operands, _cs); } } } }
               { if (g_planner.zd_plan__cd < 0) { const char *_e = getenv("SCRIP_CALL_DIAG"); g_planner.zd_plan__cd = (_e && _e[0] == '1') ? 1 : 0; }
                 if (g_planner.zd_plan__cd && rl > 0 && badi >= 0) { IR_t * _bn = nodes[badi]; int _bop = (int)_bn->op;
                     int _is_call = (_bop == (int)IR_CALL || (_bop != (int)IR_CALL_VALUE && ir_is_call_kind((IR_e)_bop)));
                     const char * _callee = (_is_call && IR_LIT(_bn).sval) ? IR_LIT(_bn).sval : (const char *)0;
                     fprintf(stderr, "[CALL-DIAG] REFUSE blocker op=%s(%d) callee=%s rl=%d badi=%d narg=%d\n", bb_op_name(_bn->op), _bop, _callee ? _callee : "-", rl, badi,
                     _is_call ? (int)_bn->n_operands : -1); } } }
    }
    }
    { for (int i = 0; i < n; i++) { if (!zon[i]) continue; int K = zd_k_g(cg, nodes[i]); int gback = -1, oback = -1;
        if (zgt[i]) { { int tk = nidx(nodes, n, zgt[i]); if (tk >= 0 && (zon[tk])) { gback = tk; } } }
        if (zot[i]) { { int tk = nidx(nodes, n, zot[i]); if (tk >= 0 && (zon[tk])) { oback = tk; } } }
        int _gbpre = (gback >= 0) ? (zout[gback] - zd_k_g(cg, nodes[gback])) : 0;
        int _obpre = (oback >= 0) ? (zout[oback] - zd_k_g(cg, nodes[oback])) : 0;
        if (!zgin[i]) zgpop[i] = (planner_resumable_frame(cg) && !(gback >= 0 && claim[gback] == claim[i])) ? 0 : (gback >= 0 && _gbpre > 0) ? (zout[i] - zurel[i] - _gbpre) :
            zd_exit_pop_s(nodes[i]->op, zmatch[i], zout[i] - zurel[i], zstmt[i], zmafter[i]);
        int _obeta = oback >= 0 && port_sz_beta(zd_edge_sz(nodes[i], 1)) && !beta_is_stmt_land(zot[i]);
        if (nodes[i]->op == IR_BOUND && !zd_pl_bspine(cg, nodes[i]) && (planner_resumable_frame(cg) || (cg && cg->spine_cells))) zwpop[i] = -K;
        else if (!zoin[i]) zwpop[i] = (oback >= 0) ? ((zout[i] - K) - (_obeta ? zout[oback] : _obpre)) :
            ((nodes[i]->op == IR_GOTO_DEFERRED) ? zd_exit_pop_s(nodes[i]->op, zmatch[i], zout[i], zstmt[i], zmafter[i]) : (zout[i] - K));
        else if (oback >= 0 && claim[oback] == claim[i] && rpos[oback] > rpos[i] && !port_sz_beta(nodes[i]->ω.sz)) zwpop[i] = (zout[i] - K) - _obpre;
        else if (oback >= 0 && claim[oback] == claim[i] && rpos[oback] < rpos[i] && port_sz_beta(nodes[i]->ω.sz)) zwpop[i] = (zout[i] - K) - zout[oback];
        else if (oback >= 0 && claim[oback] == claim[i] && rpos[oback] < rpos[i] && zd_sz_alpha(zd_edge_sz(nodes[i], 1))) zwpop[i] = (zout[i] - K) - _obpre;
        if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-FINAL] i=%d %s K=%d zout=%d gpop=%d wpop=%d gback=%d oback=%d\n", i, bb_op_name(nodes[i]->op), K, zout[i], zgpop[i], zwpop[i], gback, oback);
            } }
    for (int i = 0; i < n; i++) {
        if (!zon[i]) continue;
        IR_t * t = zd_chase(nodes[i]->γ.node);
        int q = t ? nidx(nodes, n, t) : -1;
        if (q >= 0 && zon[q] && claim[q] == claim[i] && q != i && port_sz_beta(zd_edge_sz(nodes[i], 0))) zgpop[i] = zout[i] - zout[q];
        if (!planner_resumable_frame(cg)) continue;
        IR_t * r = (nodes[i]->op == IR_GATE_ARM && nodes[i]->n_operands > 0 && (int) nodes[i]->seal) ? nodes[i]->operands[0] : (IR_t *)0;
        int rq = r ? nidx(nodes, n, r) : -1;
        if (rq >= 0 && zon[rq] && claim[rq] == claim[i] && rpos[rq] < rpos[i]) zgpop[i] = zout[i] - zout[rq];
        IR_t * o = zd_chase(nodes[i]->ω.node);
        int qo = o ? nidx(nodes, n, o) : -1;
        if (qo >= 0 && zon[qo] && claim[qo] != claim[i]) zwpop[i] = zout[i] - zd_k_g(cg, nodes[i]);
        if (qo >= 0 && zon[qo] && claim[qo] == claim[i] && qo != i && port_sz_beta(zd_edge_sz(nodes[i], 1))) zwpop[i] = zout[i] - zd_k_g(cg, nodes[i]) - zout[qo];
        if (q >= 0 && zon[q] && claim[q] != claim[i] && nodes[q]->op == IR_DISJUNCTION && !zd_edge_retains_g(cg, nodes[i])) zgpop[i] = zout[i];
    }
    int * base = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    unsigned char * known = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
    {
        int * aof = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
        int * fam = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
        int * fbase = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
        zd_pl_bases(cg, nodes, n, zon, zout, zgpop, zwpop, claim, base, known, aof, (int *)0, fam, fbase);
        for (int i = 0; i < n; i++) {
            if (!zon[i] || claim[i] < 0) continue;
            int ci = claim[i], ki = known[ci] ? 1 : (fam[ci] >= 0 ? 2 : 0);
            if (!ki) continue;
            int ai = (ki == 1 ? base[ci] : fbase[ci]) + zout[i];
            for (int v = 0; v < 2; v++) {
                int q, b;
                (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, i, v, &q, &b);
                if (g_planner.zd_plan__dg)
                    fprintf(stderr, "[ZD-XC] i=%d v=%d q=%d b=%d cq=%d known=%d ai=%d\n", i, v, q, b, q >= 0 ? claim[q] : -2, (q >= 0 && claim[q] >= 0) ? known[claim[q]] : -1, ai);
                if (q < 0 || claim[q] < 0 || claim[q] == claim[i]) continue;
                int cq = claim[q], kq = known[cq] ? 1 : (fam[cq] >= 0 ? 2 : 0);
                if (ki == 2 && b && zd_beta_via_act(cg, nodes[q])) {
                    IR_t * lt = zd_chase(nodes[q]->γ.node);
                    int lq = lt ? nidx(nodes, n, lt) : -1;
                    int ub = (nodes[ci]->op == IR_UNMARK && zd_pl_rest_ival(cg, nodes[ci]) && nodes[ci]->n_operands > 0) ? nidx(nodes, n, nodes[ci]->operands[0]) : -1;
                    int fl = (lq >= 0 && zon[lq] && claim[lq] >= 0) ? fam[claim[lq]] : -1, cb = (ub >= 0 && zon[ub] && claim[ub] >= 0) ? claim[ub] : -1;
                    int af = fl < 0 ? INT_MIN : fam[ci] == fl ? ai : (cb >= 0 && !known[cb] && fam[cb] == fl) ? fbase[cb] + zout[ub] - zout[ci] + zout[i] : INT_MIN;
                    int ax = (lq >= 0 && claim[lq] >= 0) ? fbase[claim[lq]] + zout[lq] - zd_k_g(cg, nodes[lq]) : 0;
                    if (af != INT_MIN) { if (v) zwpop[i] = af - zd_k_g(cg, nodes[i]) - ax; else zgpop[i] = af - ax; continue; }
                }
                if (ki == 2 && v && fam[ci] != ci && zd_edge_retains_g(cg, nodes[q])) {
                    int ql, qb;
                    (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, q, 0, &ql, &qb);
                    if (ql >= 0 && zon[ql] && claim[ql] >= 0 && claim[ql] == fam[ci]) { zwpop[i] = ai - zd_k_g(cg, nodes[i]) - (fbase[fam[ci]] + zout[ql] - (qb ? 0 : zd_k_g(cg, nodes[ql]))); continue;
                        }
                }
                if (kq != ki || (ki == 2 && fam[ci] != fam[cq])) continue;
                int aq = (kq == 1 ? base[cq] : fbase[cq]) + zout[q] - (b ? 0 : zd_k_g(cg, nodes[q]));
                if (!b && zarm && nodes[q]->op == IR_DISJUNCTION && zd_dj_admitted(n, q)) {
                    const char * sz = zd_edge_sz(nodes[i], v);
                    if (zd_sz_sigma(sz)) aq = (kq == 1 ? base[cq] : fbase[cq]) + zout[q];
                    else if (zd_sz_phi(sz)) aq = (kq == 1 ? base[cq] : fbase[cq]) + zout[q] - zd_djx_of(nodes[q]);
                    if (zd_sz_sigma(sz) && !v && zcps && zcpd && zarm[i] < 0 && zcpd[i] == -1)
                        for (int a = 0; a < (int)IR_LIT(nodes[q]).ival; a++) {
                            IR_t * rj = zd_dj_res(nodes[q], a);
                            int kr = rj ? nidx(nodes, n, rj) : -1, hit = 0;
                            if (kr < 0 || !zon[kr] || claim[kr] != claim[i] || zd_icn_local_var_g(cg, nodes[kr]) || zd_dj_cellless(nodes, n, zarm, kr)) continue;
                            for (IR_t * t = nodes[kr]; t && hit < n; t = zd_chase(t->γ.node)) { if (t == nodes[i]) { hit = -1; break; } hit++; }
                            if (hit != -1) continue;
                            if (zls_dj_retains(nodes[q]) || (cg && cg->spine_cells && planner_frame_slot(cg, nodes[q]) != -1)) { zcps[i] = -(kr + 2); zcpd[i] = -(q + 2); }
                            else { zcps[i] = -(kr + 2); zcpd[i] = ai - aq; }
                            break;
                        }
                }
                if (v) zwpop[i] = ai - zd_k_g(cg, nodes[i]) - aq; else zgpop[i] = ai - aq;
            }
            if (ki != 1) continue;
            IR_t * r = (nodes[i]->op == IR_GATE_ARM && nodes[i]->n_operands > 0 && (int) nodes[i]->seal) ? nodes[i]->operands[0] : (IR_t *)0;
            int rq = r ? nidx(nodes, n, r) : -1;
            if (rq >= 0 && zon[rq] && claim[rq] >= 0 && claim[rq] != claim[i] && known[claim[rq]]) zgpop[i] = ai - (base[claim[rq]] + zout[rq]);
            IR_t * ig = (nodes[i]->op == IR_GATE_ARM && !nodes[i]->γ.node && nodes[i]->n_operands > 1) ? nodes[i]->operands[1] : (IR_t *)0;
            int gq = ig ? nidx(nodes, n, ig) : -1, cq = -1, cb = 0;
            (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, i, 0, &cq, &cb);
            if (gq >= 0 && zon[gq] && claim[gq] >= 0 && known[claim[gq]] && cq < 0) zgpop[i] = ai - (base[claim[gq]] + zout[gq]);
        }
        for (int i = 0; i < n; i++) {
            if (!zon[i] || claim[i] < 0 || nodes[i]->op != IR_GATE || known[claim[i]] || fam[claim[i]] < 0) continue;
            int p = -1, ub = -1;
            for (int j = 0; j < n && p < 0; j++) if (zon[j] && claim[j] >= 0 && nodes[j]->op == IR_UNMARK && nodes[j]->n_operands > 0 && zd_chase(nodes[j]->γ.node) == nodes[i]) p = j;
            if (p >= 0) ub = nidx(nodes, n, nodes[p]->operands[0]);
            if (ub < 0 || !zon[ub] || claim[ub] < 0 || ((IR_LIT(nodes[p]).ival & 32) && zd_pl_bspine(cg, nodes[ub]))) continue;
            int cu = claim[ub], ku = known[cu] ? 1 : (fam[cu] >= 0 ? 2 : 0);
            if (!ku) continue;
            int q, b;
            (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, i, 1, &q, &b);
            if (q < 0 || claim[q] < 0 || claim[q] == claim[i]) continue;
            int cq = claim[q], kq = known[cq] ? 1 : (fam[cq] >= 0 ? 2 : 0);
            if (kq != ku || (ku == 2 && fam[cu] != fam[cq])) continue;
            zwpop[i] = ((ku == 1 ? base[cu] : fbase[cu]) + zout[ub]) - ((kq == 1 ? base[cq] : fbase[cq]) + zout[q] - (b ? 0 : zd_k_g(cg, nodes[q])));
        }
        int nalt = cg ? cg->n_alts : 0, gx = -1, ngx = 0;
        unsigned char * redo_priced = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
        memset(redo_priced, 0, (size_t)(n > 0 ? n : 1));
        for (int i = 0; i < n; i++) if (zon[i] && nodes[i]->op == IR_GATE && nodes[i]->γ.node && nodes[i]->γ.node->op == IR_SUCCEED && claim[i] >= 0 && known[claim[i]]) { gx = i; ngx++; }
        for (int i = 0; i < n; i++) {
            if (!zon[i] || claim[i] < 0 || (!known[claim[i]] && fam[claim[i]] < 0)) continue;
            IR_t * ig = (nodes[i]->op == IR_GATE_ARM && !nodes[i]->γ.node && nodes[i]->n_operands > 1) ? nodes[i]->operands[1] : (IR_t *)0;
            IR_t * c = zd_chase(ig ? ig->γ.node : nodes[i]->γ.node);
            if (!c || c->op != IR_SUCCEED || (!ig && nodes[i]->op == IR_GATE)) continue;
            int ci = claim[i], a = aof[ci];
            for (int f = known[ci] ? -1 : fam[ci], hop = 0; f >= 0 && a < 0 && hop <= n; hop++) {
                int nf = -1;
                for (int a2 = 0; a2 < nalt && a < 0 && cg->alt_entry; a2++) {
                    int q2 = cg->alt_entry[a2] ? nidx(nodes, n, zd_chase(cg->alt_entry[a2])) : -1;
                    if (q2 >= 0 && zon[q2] && claim[q2] >= 0 && fam[claim[q2]] == f) a = a2;
                }
                for (int e = 0; e < n && a < 0 && nf < 0; e++) {
                    IR_t * t = (zon[e] && claim[e] >= 0 && zd_edge_retains_g(cg, nodes[e])) ? zd_chase(nodes[e]->γ.node) : (IR_t *)0;
                    int q = t ? nidx(nodes, n, t) : -1;
                    if (q < 0 || !zon[q] || claim[q] < 0 || fam[claim[q]] != f || claim[e] == claim[q]) continue;
                    if (known[claim[e]]) a = aof[claim[e]];
                    else nf = fam[claim[e]];
                }
                f = nf;
            }
            IR_t * ad = (nalt > 0 && a >= 0 && a < nalt && cg->alt_redo) ? cg->alt_redo[a] : (IR_t *)0;
            int dq = ad ? nidx(nodes, n, zd_chase(ad)) : (nalt == 0 && ngx == 1) ? gx : -1;
            if (dq < 0 || !zon[dq] || claim[dq] < 0) continue;
            int cd = claim[dq];
            IR_t * t = zd_edge_retains_g(cg, nodes[dq]) ? zd_chase(nodes[dq]->γ.node) : (IR_t *)0;
            int q = t ? nidx(nodes, n, t) : -1;
            if (known[ci] && known[cd]) zgpop[i] = base[ci] + zout[i] - (base[cd] + zout[dq]);
            else if (!known[ci] && q >= 0 && zon[q] && claim[q] >= 0 && !known[claim[q]] && fam[claim[q]] == fam[ci])
                zgpop[i] = fbase[ci] + zout[i] - (fbase[claim[q]] + zout[q] - zd_k_g(cg, nodes[q]));
            else if (!known[ci] && !known[cd] && fam[cd] == fam[ci]) zgpop[i] = fbase[ci] + zout[i] - (fbase[cd] + zout[dq]);
            else continue;
            redo_priced[i] = 1;
        }
        if (planner_resumable_frame(cg)) for (int i = 0; i < n; i++) {
            if (!zon[i] || claim[i] < 0 || !known[claim[i]]) continue;
            IR_t * ig = (nodes[i]->op == IR_GATE_ARM && !nodes[i]->γ.node && nodes[i]->n_operands > 1) ? nodes[i]->operands[1] : (IR_t *)0;
            IR_t * c = zd_chase(ig ? ig->γ.node : nodes[i]->γ.node);
            int cq = c ? nidx(nodes, n, c) : -1;
            if (c && (c->op == IR_SUCCEED || cq < 0 || !zon[cq]) && !redo_priced[i]) zgpop[i] = zd_edge_retains_g(cg, nodes[i]) ? 0 : base[claim[i]] + zout[i];
            IR_t * o = zd_chase(nodes[i]->ω.node);
            int oq = o ? nidx(nodes, n, o) : -1;
            if (!o || o->op == IR_FAIL || oq < 0 || !zon[oq]) zwpop[i] = base[claim[i]] + zout[i] - zd_k_g(cg, nodes[i]);
        }
        else if (!(cg && cg->spine_cells)) for (int i = 0; i < n; i++) {
            if (!zon[i] || claim[i] < 0 || !known[claim[i]] || !base[claim[i]]) continue;
            IR_t * c = zd_chase(nodes[i]->γ.node);
            int cq = c ? nidx(nodes, n, c) : -1;
            if (!c || c->op == IR_SUCCEED || cq < 0 || !zon[cq]) zgpop[i] += base[claim[i]];
            IR_t * o = zd_chase(nodes[i]->ω.node);
            int oq = o ? nidx(nodes, n, o) : -1;
            if (!o || o->op == IR_FAIL || oq < 0 || !zon[oq]) zwpop[i] += base[claim[i]];
        }
    }
    if (zdj_on) {
        for (int i = 0; i < n; i++) {
            if (!zon[i]) continue;
            for (int v = 0; v < 2; v++) {
                IR_t * t = zd_chase(v ? nodes[i]->ω.node : nodes[i]->γ.node);
                int q = t ? nidx(nodes, n, t) : -1;
                const char * sz = zd_edge_sz(nodes[i], v);
                if (q < 0 || !zon[q] || !(port_sz_beta(sz) || zd_sz_phi(sz))) continue;
                if (zd_icn_stmt_head(cg, nodes[q])) continue;
                if (!v && zd_sz_phi(sz) && nodes[q]->op != IR_DISJUNCTION && zd_chase(nodes[i]->ω.node) == t && !port_sz_beta(zd_edge_sz(nodes[i], 1))) continue;
                int p = (v == 0 && nodes[i]->op == IR_UNMARK) ? -1 : zd_pred_bt(cg, nodes, n, claim, claim[i], i);
                if (p >= 0 && p != q && !zd_icn_chain_step_ok(cg, nodes, n, p, q)) p = -1;
                int to = q;
                if (p >= 0 && p != q) { to = p; if (zchn) zchn[2 * i + v] = p; }
                int xb = (claim[i] != claim[to] && claim[i] >= 0 && claim[to] >= 0 && known[claim[i]] && known[claim[to]]) ? base[claim[i]] - base[claim[to]] : 0;
                int xq = (claim[i] != claim[q] && claim[i] >= 0 && claim[q] >= 0 && known[claim[i]] && known[claim[q]]) ? base[claim[i]] - base[claim[q]] : 0;
                int zq = zout[q] - (zd_sz_phi(sz) ? zd_djx_of(nodes[q]) : 0);
                if (v) zwpop[i] = (zout[i] - zd_k_g(cg, nodes[i])) - (to == q ? zq : zout[to]) + xb;
                else zgpop[i] = zout[i] - (to == q ? zq : zout[to]) + xb;
                for (int c = p, g3 = 0; c >= 0 && c != q && g3++ <= n; c = zd_pred_bt(cg, nodes, n, claim, claim[i], c)) {
                    int pc = zd_pred_bt(cg, nodes, n, claim, claim[i], c);
                    int stop = (pc >= 0 && pc != q && !zd_icn_chain_step_ok(cg, nodes, n, pc, q));
                    if (stop) pc = -1;
                    int tc = (pc >= 0) ? pc : q;
                    if (zchn && pc >= 0 && pc != q) zchn[2 * c + 1] = pc;
                    zwpop[c] = (zout[c] - zd_k_g(cg, nodes[c])) - (tc == q ? zq - xq : zout[tc]);
                    if (zchn && pc == q && zd_chase(nodes[c]->ω.node) != nodes[q]) zchn[2 * c + 1] = q;
                    if (stop) break;
                }
            }
        }
    }
    if (zdj_on) {
        for (int i = 0; i < n; i++) {
            if (!zon[i] || nodes[i]->op != IR_UNMARK || nodes[i]->n_operands < 1) continue;
            int b = nidx(nodes, n, nodes[i]->operands[0]);
            if (b < 0 || !zon[b]) continue;
            int xc = claim[b] != claim[i];
            if (xc && !(zd_unmark_acts(cg, nodes[i]) > 0 && claim[b] >= 0 && claim[i] >= 0 && known[claim[b]] && known[claim[i]])) continue;
            IR_t * _gs = nodes[i];
            IR_t * _gt = zd_lm_chase(nodes[i], n, &_gs);
            int _gq = _gt ? nidx(nodes, n, _gt) : -1;
            int _rb = _gq >= 0 && zon[_gq] && port_sz_beta(zd_edge_sz(_gs, 0));
            if (xc && _rb && (claim[_gq] < 0 || !known[claim[_gq]])) continue;
            if (zd_unmark_acts(cg, nodes[i]) > 0) {
                int ab = base[claim[b]] + zout[b];
                if (_gt == nodes[b] || _rb) for (IR_t * u = zd_chase(nodes[i]->γ.node); u && u != _gt; u = zd_chase(u->γ.node)) { int k = nidx(nodes, n, u);
                    if (k < 0 || !zon[k] || (xc && (claim[k] < 0 || !known[claim[k]]))) continue;
                    zout[k] = xc ? ab - base[claim[k]] : zout[b];
                    zgpop[k] = (u == _gs && _rb) ? (xc ? ab - (base[claim[_gq]] + zout[_gq]) : zout[b] - zout[_gq]) : (u == _gs && _gt == nodes[b]) ? zd_k_g(cg, nodes[b]) : 0;
                    zwpop[k] = 0; }
                if (_rb && _gs == nodes[i]) g_planner.g_zd_upost[(size_t)i] = xc ? ab - (base[claim[_gq]] + zout[_gq]) : zout[b] - zout[_gq];
                else if (_gt == nodes[b] && _gs == nodes[i]) g_planner.g_zd_upost[(size_t)i] = zd_k_g(cg, nodes[b]);
                continue;
            }
            if (!xc && !_rb && _gt && _gt->op == IR_DISJUNCTION && zd_sz_phi(zd_edge_sz(_gs, 0))) continue;
            if (zchn[2 * i] < 0) {
                int _xr = !_rb && _gq >= 0 && zon[_gq] && claim[_gq] >= 0 && claim[i] >= 0 && claim[_gq] != claim[i] && known[claim[_gq]] && known[claim[i]];
                zgpop[i] = _xr ? base[claim[i]] + zout[i] - (base[claim[_gq]] + zout[_gq] - zd_k_g(cg, nodes[_gq]))
                               : zout[i] - (_rb ? zout[_gq] : _gt == nodes[b] ? zout[b] - zd_k_g(cg, nodes[b]) : zout[b]);
                zchn[2 * i + 1] = b;
                zwpop[i] = 0;
                if (_gt == nodes[b] || _rb) for (IR_t * u = zd_chase(nodes[i]->γ.node); u && u != _gt; u = zd_chase(u->γ.node)) { int k = nidx(nodes, n, u);
                    if (k >= 0 && zon[k]) { zout[k] = zout[i] - zgpop[i]; zgpop[k] = 0; zwpop[k] = 0; } }
            }
            else if (zchn[2 * i + 1] < 0) zwpop[i] = zout[i] - zout[b];
        }
    }
    if (zdj_arms) {
        for (int i = 0; i < n; i++) {
            if (!zon[i] || zarm[i] < 0 || (!zdj_on && zarmj[i] < 0)) continue;
            for (int v = 0; v < 2; v++) {
                IR_t * t = zd_chase(v ? nodes[i]->ω.node : nodes[i]->γ.node);
                int q = t ? nidx(nodes, n, t) : -1;
                if (q < 0 || !zon[q]) continue;
                if (nodes[q]->op != IR_DISJUNCTION || !zd_dj_anc(zarm, i, q)) {
                    if (zarm[q] == zarm[i] && zarmj[q] == zarmj[i]) continue;
                    if (zchn && zchn[2 * i + v] >= 0 && zchn[2 * i + v] != q) continue;
                    int res = 0;
                    for (int j = 0, na = (int)IR_LIT(nodes[q]).ival; !v && nodes[q]->op == IR_DISJUNCTION && j < na && !res; j++)
                        res = zd_dj_res(nodes[q], j) == nodes[i] || (2 * na + j < nodes[q]->n_operands && nodes[q]->operands[2 * na + j] == nodes[i]);
                    int ent = (port_sz_beta(zd_edge_sz(nodes[i], v)) || res) ? zout[q] : zout[q] - zd_k_g(cg, nodes[q]);
                    int xb = (claim[i] != claim[q] && claim[i] >= 0 && claim[q] >= 0 && known[claim[i]] && known[claim[q]]) ? base[claim[i]] - base[claim[q]] : 0;
                    if (v) zwpop[i] = (zout[i] - zd_k_g(cg, nodes[i])) - ent + xb;
                    else zgpop[i] = zout[i] - ent + xb;
                    continue;
                }
                if (v) { if (!zdj_on) zwpop[i] = (zout[i] - zd_k_g(cg, nodes[i])) - zout[q]; continue; }
                if (zdj_on && (port_sz_beta(zd_edge_sz(nodes[i], 0)) || zd_sz_phi(zd_edge_sz(nodes[i], 0)))) continue;
                zgpop[i] = zout[i] - zout[q];
                if (zls_dj_retains(nodes[q]) && !zd_djx_of(nodes[q])) { int p0 = i; while (zarm[p0] != q) p0 = zarm[p0]; if (zls_dj_arm_resumes(nodes[q], zarmj[p0])) zgpop[i] = 0; }
                if (!zd_sz_sigma(zd_edge_sz(nodes[i], 0))) continue;
                int p = i;
                while (zarm[p] != q) p = zarm[p];
                IR_t * rj = zd_dj_res(nodes[q], zarmj[p]);
                int kr = rj ? nidx(nodes, n, rj) : -1;
                if (kr >= 0 && zon[kr] && !zd_icn_local_var_g(cg, nodes[kr]) && !zd_dj_cellless(nodes, n, zarm, kr) &&
                    (zls_dj_retains(nodes[q]) || (cg && cg->spine_cells && planner_frame_slot(cg, nodes[q]) != -1))) { zcps[i] = -(kr + 2);
                    zcpd[i] = -(q + 2); }
                else if (kr >= 0 && zon[kr] && !zd_icn_local_var_g(cg, nodes[kr]) && !zd_dj_cellless(nodes, n, zarm, kr)) { zcps[i] = -(kr + 2); zcpd[i] = zout[i] - zout[q]; }
            }
        }
    }
    if (zarm)
        for (int i = 0; i < n; i++) {
            if (!zon[i] || zarm[i] < 0) continue;
            IR_t * t = zd_chase(nodes[i]->γ.node);
            int q = t ? nidx(nodes, n, t) : -1;
            if (q >= 0 && (nodes[q]->op == IR_MATCH_ARBNO || nodes[q]->op == IR_MATCH_ALTERNATE || nodes[q]->op == IR_MATCH_FENCE1) && zd_dj_anc(zarm, i, q)) zgpop[i] = 0;
        }
    for (int sp = 0; sp < n; sp++) {
        if (!zon[sp] || nodes[sp]->op != IR_SUSPEND || claim[sp] < 0) continue;
        int k = zd_yield_end(nodes, n, sp, zon, claim);
        if (k < 0) continue;
        zgpop[k] = (cg && cg->home_frame && cg->spine_cells) ? 0 : zout[k] - zout[sp];
        if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-YIELD] i=%d ends the yield of suspend %d: it pops %d\n", k, sp, zgpop[k]);
    }
    for (int i = 0; i < n; i++) if (zon[i] && nodes[i]->op == IR_GATE_ARM && nodes[i]->n_operands > 1 && zd_edge_retains_g(cg, nodes[i]->operands[1]) &&
        zd_arm_resumes(nodes[i], nodes[i]->operands[1])) zgpop[i] = 0;
    for (int dj = 0; dj < n; dj++) if (zon[dj] && nodes[dj]->op == IR_DISJUNCTION && zd_edge_retains_g(cg, nodes[dj]))
        for (int a = 0; a < (int)IR_LIT(nodes[dj]).ival; a++) { int e = zd_pl_dj_arm_holds(nodes[dj], a) ? zd_dj_arm_end(nodes, n, dj, a) : -1; if (e >= 0 && zon[e]) zgpop[e] = 0; }
    zd_dj_discards(cg, nodes, n, zon, zout, zgpop, zwpop, zarm, claim, base, known, zdj_on);
    if (zarm)
        for (int i = 0; i < n; i++) {
            if (!zon[i] || zarm[i] < 0) continue;
            IR_t * t = zd_chase(nodes[i]->ω.node);
            int q = t ? nidx(nodes, n, t) : -1;
            if (q < 0 || !zon[q]) continue;
            for (int p = zarm[i], g = 0; p >= 0 && g++ < 4096; p = zarm[p]) if (q != p && !zd_dj_anc(zarm, q, p)) zwpop[i] += zd_match_env_record(cg, nodes[p]);
        }
    { if (zd_map_on()) { fprintf(stderr, "[ZD-MAP] PLAN -- same i= indices as GRAPH above\n");
        for (int i = 0; i < n; i++) fprintf(stderr, "[ZD-MAP] i=%-3d %-22s claim=%-3d rpos=%-3d zon=%d zout=%-4d gpop=%-5d wpop=%-5d arm=%d slot=%d\n",
            i, bb_op_name(nodes[i]->op), claim[i], rpos[i], (int)zon[i], zout[i], zgpop[i], zwpop[i], zarm ? zarm[i] : -1, zls_off(nodes[i])); } }
    if (zclaim) for (int i = 0; i < n; i++) zclaim[i] = zon[i] ? claim[i] : -1;
}
void zd_depth_census_g(const IR_graph_t * cg, IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, const char *prefix) {
     if (g_planner.zd_depth_census__on < 0) { const char * e = getenv("SCRIP_ZD_DEPTH"); g_planner.zd_depth_census__on = (e && *e == '1') ? 1 : 0; }
    if (!g_planner.zd_depth_census__on || n <= 0) return;
    zdw_ent_t * w = (zdw_ent_t *)ct_zalloc((size_t)(2 * n + 1), sizeof(zdw_ent_t)); if (!w) return;
    int wn = 0, armed = 0, edges = 0, walls = 0;
    for (int i = 0; i < n; i++) { if (!zon[i]) continue; armed++;
        int K = zd_k_g(cg, nodes[i]);
        for (int p = 0; p < 2; p++) { IR_t * t = zd_chase(p == 0 ? nodes[i]->γ.node : nodes[i]->ω.node); if (!t) continue;
            int d = (p == 0) ? (zout[i] - zgpop[i]) : (zout[i] - K - zwpop[i]); int ti = -1;
            { int k = nidx(nodes, n, t); if (k >= 0) { ti = k; } }
            if (ti < 0 || !zon[ti]) continue; edges++;
            int f = -1; for (int k = 0; k < wn; k++) if (w[k].t == t) { f = k; break; }
            if (f < 0) { w[wn].t = t; w[wn].d0 = d; w[wn].nd = 1; w[wn].multi = 0; wn++; }
            else { w[f].nd++; if (d != w[f].d0 && !w[f].multi) { w[f].multi = 1; walls++; } } } }
    for (int k = 0; k < wn; k++) { if (!w[k].multi) continue;
        int _wi = -1; { int q = nidx(nodes, n, w[k].t); if (q >= 0) { _wi = q; } }
        fprintf(stderr, "[ZD-DEPTH] WALL %s i=%d %s preds=%d first_depth=%d gpop=%d zout=%d\n", prefix ? prefix : "?", _wi, bb_op_name(w[k].t->op) ? bb_op_name(w[k].t->op) : "<unnamed>", w[k].nd,
            w[k].d0, _wi >= 0 ? zgpop[_wi] : -9999, _wi >= 0 ? zout[_wi] : -9999);
        for (int i = 0; i < n; i++) { if (!zon[i]) continue; int Kp = zd_k_g(cg, nodes[i]);
            for (int p = 0; p < 2; p++) { IR_t * t = zd_chase(p == 0 ? nodes[i]->γ.node : nodes[i]->ω.node); if (t != w[k].t) continue;
                int d = (p == 0) ? (zout[i] - zgpop[i]) : (zout[i] - Kp - zwpop[i]);
                fprintf(stderr, "[ZD-DEPTH]   pred %s i=%d %-22s depth=%d gpop=%d%s\n", p == 0 ? "\xce\xb3" : "\xcf\x89", i, bb_op_name(nodes[i]->op) ? bb_op_name(nodes[i]->op) : "<unnamed>", d,
                    zgpop[i], d == w[k].d0 ? "" : "   <-- DISAGREES"); } } }
    fprintf(stderr, "[ZD-DEPTH] %s n=%d armed=%d in_edges=%d joins=%d walls=%d\n", prefix ? prefix : "?", n, armed, edges, wn, walls);
    ct_drop(w);
}
static int zd_check_discard(const IR_t * t) { int o = t ? (int)t->op : -1;
    return o == IR_UNMARK || o == IR_BOUND || o == IR_MATCH_FENCE0 || o == IR_MATCH_FENCE1 || o == IR_CUT || o == IR_GATE || o == IR_GATE_ARM || o == IR_STATEMENT_BEGIN || o == IR_SUCCEED ||
        o == IR_FAIL; }
static int zd_check_arm_env(int o) { return o == IR_GATE || o == IR_GATE_ARM || o == IR_DISJUNCTION || o == IR_MATCH_ARBNO || o == IR_MATCH_ALTERNATE; }
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
void zd_check_g(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, const int *xb, const int *zchn,
    const int *zcut, const int *zarm, const char *prefix){
    if (g_planner.zd_check__on < 0) { const char * e = getenv("SCRIP_ZD_CHECK"); g_planner.zd_check__on = (e && *e == '1') ? 1 : 0; }
    if (!g_planner.zd_check__on || n <= 0) return;
    int armed = 0, def = 0, edges = 0, viol = 0;
    for (int i = 0; i < n; i++) { if (!zon[i]) continue; armed++; if (xb && xb[i] != INT_MIN) def++; }
    for (int i = 0; i < n; i++) {
        if (!zon[i] || (xb && xb[i] == INT_MIN)) continue;
        int o = (int)nodes[i]->op;
        if (zd_check_arm_env(o) || o == IR_UNMARK || o == IR_CUT || o == IR_MATCH_END || o == IR_MATCH_REPLACE) continue;
        int bi = xb ? xb[i] : 0, Xi = bi + zout[i], Ki = zd_k_g(cg, nodes[i]);
        for (int v = 0; v < 2; v++) {
            if (v && o == IR_LINE_MARK) continue;
            IR_t * t = zd_chase(v ? nodes[i]->ω.node : nodes[i]->γ.node);
            if (!t || zd_check_discard(t) || zd_check_arm_env((int)t->op) || (v && t->op == IR_MATCH_BEGIN && zd_sz_phi(zd_edge_sz(nodes[i], 1)))) continue;
            int j = nidx(nodes, n, t), via = (zchn && zchn[2 * i + v] >= 0) ? zchn[2 * i + v] : -1;
            if (via >= 0) j = via;
            if (j < 0 || j == i || !zon[j]) continue;
            if (xb) { if (xb[j] == INT_MIN) continue; } else if (!claim || claim[j] != claim[i]) continue;
            const char * sz = zd_edge_sz(nodes[i], v);
            int bj = xb ? xb[j] : 0, beta = via >= 0 || port_sz_beta(sz) || (zd_sz_phi(sz) && ir_glue_box(nodes[j], 1)) || (!v && zd_sz_sigma(sz) && ir_glue_box(nodes[j], 0));
            int arrive = v ? (Xi - Ki - zwpop[i]) : (Xi - zgpop[i]);
            if (v && zarm) for (int p = zarm[i], g = 0; p >= 0 && g++ < 4096; p = zarm[p]) if (j != p && !zd_dj_anc(zarm, j, p)) arrive += zd_match_env_record(cg, nodes[p]);
            int Xj = bj + zout[j], expect = beta ? Xj : Xj - zd_k_g(cg, nodes[j]);
            edges++;
            if (arrive == expect) continue;
            viol++;
            fprintf(stderr, "[ZD-CHECK]   %s i=%d %s -> j=%d %s arrives=%d expects=%d\n", v ? (beta ? "OMEGA-BETA " : "OMEGA-ALPHA") : (beta ? "GAMMA-BETA " : "GAMMA-ALPHA"), i,
                bb_op_name(nodes[i]->op), j,
                bb_op_name(nodes[j]->op), arrive, expect);
        }
    }
    for (int i = 0; zcut && xb && i < n; i++) {
        if (!zon[i] || nodes[i]->op != IR_CUT || zcut[i] < 0) continue;
        IR_t * t = zd_chase(nodes[i]->γ.node);
        int j = t ? nidx(nodes, n, t) : -1;
        if (j < 0 || j == i || !zon[j] || xb[j] == INT_MIN) continue;
        int join = zd_pl_dj_join(nodes, n, i, j), arrive = zcut[i] - zgpop[i], expect = xb[j] + zout[j] - (join ? 0 : zd_k_g(cg, nodes[j]));
        edges++;
        if (arrive == expect) continue;
        viol++;
        fprintf(stderr, "[ZD-CHECK]   DISCARD     i=%d %s -> j=%d %s restores=%d expects=%d\n", i, bb_op_name(nodes[i]->op), j, bb_op_name(nodes[j]->op), arrive, expect);
    }
    for (int i = 0; zcut && xb && i < n; i++) {
        if (!zon[i] || nodes[i]->op != IR_UNMARK || zcut[i] < 0 || nodes[i]->n_operands < 1) continue;
        int b = nidx(nodes, n, nodes[i]->operands[0]), up = zd_upost_of(nodes[i]);
        int chn = (xb[i] != INT_MIN) ? xb[i] + zout[i] - zgpop[i] : INT_MIN;
        int rec = (b >= 0 && zon[b] && xb[b] != INT_MIN) ? xb[b] + zout[b] - zd_k_g(cg, nodes[b]) - up : INT_MIN;
        if (chn != zcut[i] && rec != zcut[i])
            fprintf(stderr, "[ZD-CHECK]   UNMARK      i=%d -> E_cont=%d chain_arrives=%d record_arrives=%d (reported, not counted)\n", i, zcut[i], chn, rec);
    }
    int sb_cut = 0;
    for (int i = 0; i < n; i++) if (nodes[i]->op == IR_CUT && !IR_LIT(nodes[i]).ival) sb_cut = 1;
    for (int i = 0; !sb_cut && xb && cg && cg->n_alts > 0 && cg->alt_redo && cg->alt_ret && i < n; i++) {
        if (!zon[i] || xb[i] == INT_MIN) continue;
        IR_t * ig = (nodes[i]->op == IR_GATE_ARM && !nodes[i]->γ.node && nodes[i]->n_operands > 1) ? nodes[i]->operands[1] : (IR_t *)0;
        IR_t * c = zd_chase(ig ? ig->γ.node : nodes[i]->γ.node);
        if (!c || c->op != IR_SUCCEED || (!ig && nodes[i]->op == IR_GATE)) continue;
        for (int k = 0; k < cg->n_alts; k++) {
            IR_t * rd = cg->alt_redo[k];
            int dq = (rd && cg->alt_ret[k] && zd_chase(cg->alt_ret[k]) == c) ? nidx(nodes, n, zd_chase(rd)) : -1;
            int expect;
            if (dq < 0 || !zon[dq] || xb[dq] == INT_MIN) continue;
            expect = xb[i] + zout[i] - (xb[dq] + zout[dq]);
            edges++;
            if (zgpop[i] != expect) {
                viol++;
                fprintf(stderr, "[ZD-CHECK]   SAVED-BETA i=%d %s -> redo j=%d %s gpop=%d expects=%d\n", i, bb_op_name(nodes[i]->op), dq, bb_op_name(nodes[dq]->op), zgpop[i], expect);
            }
            break;
        }
    }
    fprintf(stderr, "[ZD-CHECK] graph=%s boxes=%d armed=%d definite=%d coordinate=%s edges=%d violations=%d\n", prefix ? prefix : "?", n, armed, def, xb ? "absolute" : "relative", edges, viol);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int planner_varslot(const IR_graph_t * cg, const char * name) { int c = zst_coord(name); if (c >= 0) return c; return ir_varslot_of(cg, name); }
int fc_head_fp(const IR_t * nd);
extern "C" int zls_key_unbacked(const IR_graph_t * g, int off);
static int * zd_place_ints(int n, int fill) { int * p = (int *)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(int)); for (int i = 0; i < n; i++) p[i] = fill; return p; }
static int zd_place_key(IR_t ** nodes, int n, int i, IR_t * nd, int own) {
    if (!nd) return -1;
    if (nd->op == IR_CONJUNCTION && nd->n_operands > 0 && nd->operands[0] && zls_off(nd->operands[0]) >= 0) { int k = nidx(nodes, n, nd);
        if (k >= 0 && (own ? k <= i : k < i)) return zls_off(nd->operands[0]); }
    return zls_off(nd);
}
static void zd_place(const IR_graph_t * cg, zeta_plan_t * rec, IR_t ** nodes, int n, const unsigned char * zon, const int * zout, const int * zarm, const int * zcps, const int * zcpd) {
    const IR_graph_t * g = cg;
    rec->home = zd_place_ints(n, -1); rec->zeta = (unsigned char *)ct_zalloc((size_t)n, 1); rec->lkey = zd_place_ints(n, -1); rec->alias = zd_place_ints(n, -1); rec->under = zd_place_ints(n, 0);
    rec->pat = zd_place_ints(n, 0); rec->fc = zd_place_ints(n, 0); rec->tail = zd_place_ints(n, 0); rec->ret = zd_place_ints(n, 0); rec->grec_off = zd_place_ints(n, -1);
        rec->wrec_off = zd_place_ints(n, -1);
    rec->grec_res = zd_place_ints(n, 0); rec->wrec_res = zd_place_ints(n, 0); rec->cpa_s = zd_place_ints(n, -1); rec->cpa_d = zd_place_ints(n, -1); rec->cpa_sx = zd_place_ints(n, -1);
    rec->cpa_dx = zd_place_ints(n, -1); rec->rdo = zd_place_ints(n + 1, 0);
    int total = 0;
    for (int i = 0; i < n; i++) { rec->rdo[i] = total; total += nodes[i]->n_operands + 1; }
    rec->rdo[n] = total; rec->rdn = total; rec->rd = (zrd_t *)ct_zalloc((size_t)(total > 0 ? total : 1), sizeof(zrd_t));
    for (int q = 0; q < total; q++) { rec->rd[q].dist = 0; rec->rd[q].xf = -1; rec->rd[q].fr = -1; rec->rd[q].key = -1; rec->rd[q].kind = -1; rec->rd[q].anchor = -1; }
    planner_consumers_build(g);
    int cur_saved = g_planner.g_zd_cur;
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
        if (ir_is_matcher_element((int)nd->op))
            for (int j = 0; j < no; j++) {
                IR_t * pa = nd->operands[j];
                if (!pa || ir_is_matcher((int)pa->op)) continue;
                for (int ka = 0; ka < n; ka++) {
                    if (nodes[ka] != pa) continue;
                    if (ka >= i) break;
                    int mb1 = -1, nmb = 0;
                    for (int z2 = i - 1; z2 > ka; z2--) if (nodes[z2]->op == IR_MATCH_BEGIN) { nmb++; mb1 = z2; }
                    if (nmb == 1 && mb1 > ka) { int xk = 0; for (int z3 = mb1 - 1; z3 > ka; z3--) if (nodes[z3]->op == IR_MATCH_DEFER) xk += 16; rd[j].anchor = 8 + (zout[mb1] - zout[ka]) + xk; }
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
            for (int zj = i - 1; zj >= 0; zj--) { if (nodes[zj]->op == IR_MATCH_END) break; if (zon[zj]) zu += zd_k_g(cg, nodes[zj]); }
            rec->under[i] = zu;
            int zp = 0, inpat = 0;
            for (int zj = i - 1; zj >= 0; zj--) {
                if (nodes[zj]->op == IR_MATCH_BEGIN) break;
                if (nodes[zj]->op == IR_MATCH_END) { inpat = 1; continue; }
                if (inpat && zon[zj] && nodes[zj]->op >= IR_MATCH && nodes[zj]->op <= IR_MATCH_VALUE) zp += zd_k_g(cg, nodes[zj]) - fence0_release_bytes(nodes[zj]);
            }
            rec->pat[i] = zp;
            IR_t * mb = (IR_t *)0;
            for (int zj = i - 1; zj >= 0; zj--) if (nodes[zj]->op == IR_MATCH_BEGIN) { mb = nodes[zj]; break; }
            int fp = mb ? fc_head_fp(mb) : -1;
            rec->fc[i] = fp >= 0 ? fp : 0;
        }
        if (nd->op == IR_TO || nd->op == IR_TO_BY) { int zt = 0; for (int q = i + 1; q < n; q++) if (zon[q]) zt += zd_k_g(cg, nodes[q]); rec->tail[i] = zt; }
        { int nq = zd_nops_g(cg, nd), op = (int)nd->op, cap = (op == IR_MATCH_ASSIGN_COND || op == IR_MATCH_ASSIGN_IMM) ? 1 : 0;
            IR_t * xs = zd_scan_subject(nd); int nx = (xs && nq == no) ? 1 : 0;
            for (int j = 0; j < nq + nx && j < no + nx; j++) {
                if (cap && j == 0) continue;
                IR_t * p = (j < no) ? nd->operands[j] : xs;
                { IR_t * pv = zd_conj_value_node_g(cg, p); int kv = pv != p ? nidx(nodes, n, pv) : -1; if (pv != p && zd_k_own_g(cg, pv) >= 16 && kv >= 0 && zon[kv]) p = pv; }
                int k = nidx(nodes, n, p);
                if (k < 0) continue;
                rd[j].dist = planner_read_distance(cg, nodes, n, zarm, i, k, zout, rec->xb);
                if (g_planner.zd_check__on > 0 && rec->xb && rec->claim && rec->claim[i] != rec->claim[k] && (rec->xb[i] == INT_MIN || rec->xb[k] == INT_MIN) && planner_frame_slot(g, p) == -1 &&
                    (zd_k_own_g(cg, p) >= 16 || zd_local_home_node(cg, p)) && !zd_run_alpha_dead(cg, nodes, n, rec->claim, rec->claim[i])) { int fok = 0; zd_fam_delta(i, k, &fok);
                    if (!fok) fprintf(stderr, "[ZD-CHECK]   READ i=%d %s reads p=%d %s across runs with an unknown base\n", i, bb_op_name(nd->op), k, bb_op_name(p->op)); }
                { const IR_t * lh = zd_local_home_node(cg, p); if (lh) rd[j].fr = planner_varslot(cg, IR_LIT(lh).sval);
                    else if (zd_dj_cellless(nodes, n, zarm, k) && zls_off(p) >= 0) rd[j].fr = zls_off(p); }
                rd[j].kind = (int)ir_norm_call_kind(p->op);
            }
            if (nd->op == IR_MATCH_REPLACE) { rd[2].dist = -1; rd[2].kind = -1; IR_t * mb2 = (IR_t *)0; int jh2 = -1;
                for (int zj = i - 1; zj >= 0; zj--) if (nodes[zj]->op == IR_MATCH_BEGIN) { mb2 = nodes[zj]; jh2 = zj; break; }
                IR_t * sp = (mb2 && mb2->n_operands > 0) ? mb2->operands[0] : (IR_t *)0;
                if (sp && jh2 >= 0) for (int js = jh2 - 1; js >= 0; js--) if (nodes[js] == sp) { if (zon[js]) { rd[2].dist = rec->under[i] + (zout[jh2] - zout[js]); rd[2].kind = (int)sp->op; } break;
                    } }
            int ns = nq < no ? nq : no;
            for (int j = 0; j < ns; j++) { IR_t * po = nd->operands[j], * pv = zd_value_producer_g(cg, po); int ko = po ? nidx(nodes, n, po) : -1, kp = pv ? nidx(nodes, n, pv) : -1;
                int ph = (po && pv != po && planner_frame_slot(g, pv) != -1) ? 1 : 0, onp = (kp >= 0 && zon[kp]) ? 1 : 0, ono = (ko >= 0 && zon[ko]) ? 1 : 0;
                rd[j].key = (po && (zd_k_own_g(cg, po) >= 16 || zd_local_home_node(cg, po) || (pv != po && zd_k_own_g(cg, pv) >= 16 && onp) || ph) && ono) ? zd_place_key(nodes, n, i, po, 0) : -1; }
            if (xs && ns == no) { int kx = nidx(nodes, n, xs); rd[ns].key = ((zd_k_own_g(cg, xs) >= 16 || zd_local_home_node(cg, xs)) && kx >= 0 && zon[kx]) ? zls_off(xs) : -1; } }
    }
    g_planner.g_zd_grec_off = -1; g_planner.g_zd_wrec_off = -1; g_planner.g_zd_grec_res = 0; g_planner.g_zd_wrec_res = 0; g_planner.g_zd_cpsx = -1; g_planner.g_zd_cpdx = -1; g_planner.g_zd_cps = -1;
    g_planner.g_zd_cpd = -1; g_planner.g_zd_cur = cur_saved;
}
zeta_plan_t * planner_chain_plan(const IR_graph_t * cg, IR_t * entry, const char * prefix) {
    int CH_MAX = 65536; if (cg && cg->n + 1024 > CH_MAX) CH_MAX = cg->n + 1024; int Q_MAX = CH_MAX * 16;
      if (g_planner.codegen_flat_chain_body_nodes_cap < CH_MAX) { g_planner.codegen_flat_chain_body_nodes = (IR_t **)ct_grow(g_planner.codegen_flat_chain_body_nodes, (size_t)CH_MAX * sizeof(IR_t *));
          g_planner.codegen_flat_chain_body_nodes_cap = CH_MAX; } int n = 0;
      if (g_planner.codegen_flat_chain_body_queue_cap < Q_MAX) { g_planner.codegen_flat_chain_body_queue = (IR_t **)ct_grow(g_planner.codegen_flat_chain_body_queue, (size_t)Q_MAX * sizeof(IR_t *));
          g_planner.codegen_flat_chain_body_queue_cap = Q_MAX; } int qh = 0, qt = 0;
    int entry_is_own_graph_root = (cg && (entry == cg->entry)) ? 1 : 0;
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL || entry->op == IR_GOTO) && entry->γ.node && guard++ < CH_MAX) entry = entry->γ.node; }
    if (cg && entry == cg->entry) entry_is_own_graph_root = 1;
    int flat_empty_body_fail = (entry && entry->op == IR_FAIL) ? 1 : 0;
    int flat_empty_body_succ = (entry && entry->op == IR_SUCCEED) ? 1 : 0;
    entry = entry;
      if (g_planner.codegen_flat_chain_body_postv_cap < CH_MAX) { g_planner.codegen_flat_chain_body_postv = (IR_t **)ct_grow(g_planner.codegen_flat_chain_body_postv, (size_t)CH_MAX * sizeof(IR_t *));
          g_planner.codegen_flat_chain_body_postv_cap = CH_MAX; } int pn = 0;
    ir_pset_t seenset; ir_pset_init(&seenset);
#define RPO_VISITED(p) (ir_pset_has(&seenset, (const IR_t *)(p)))
#define RPO_MARK(p) ir_pset_add(&seenset, (const IR_t *)(p))
#define RPO_TAG_EMIT ((IR_t *)1)
#define RPO_PUSH(p) do { IR_t *_rpp = (IR_t *)(p); int _rppg = 0; while (_rpp && _rpp->op == IR_SUCCEED && _rpp->γ.node && _rppg++ < 4096) _rpp = _rpp->γ.node; if (_rpp && _rpp->op != IR_SUCCEED \
&& _rpp->op != IR_FAIL && !ir_floater_member(_rpp) && !(_stmt_seed && _rpp->op == IR_STATEMENT_BEGIN) && !RPO_VISITED(_rpp) && qt < Q_MAX) g_planner.codegen_flat_chain_body_queue[qt++] = _rpp; } \
    while (0)
#define RPO_PUSH_SUCCS(c) \
         \
        RPO_PUSH((c)->γ.node); \
if (((c)->op == IR_MATCH_ALTERNATE || (c)->op == IR_MATCH_ARBNO || (c)->op == IR_MATCH_FENCE1 || (c)->op == IR_SCAN_ALTERNATE || (c)->op == IR_DISJUNCTION) && (c)-> \
    n_operands > 0) for (int _oi = (c)->n_operands - 1; _oi >= 0; _oi--) RPO_PUSH((c)->operands[_oi]); \
        if ((ir_is_matcher_element((c)->op)) || (c)->op == IR_MATCH_DEFER || (c)->op == IR_MATCH_VALUE) RPO_PUSH((c)->ω.node); \
        if ((c)->op == IR_MATCH_DEFER) for (int _di = (c)->n_operands - 1; _di >= 0; _di--) if ((c)->operands[_di] && (c)->operands[_di]->op == IR_MATCH_ABORT) RPO_PUSH((c)->operands[_di]); \
        if ((c)->op == IR_REPALT && (c)->n_operands > 1) RPO_PUSH((c)->operands[1]); \
        if ((c)->op == IR_REPALT && (c)->n_operands > 0) RPO_PUSH((c)->operands[0]); \
        if ((c)->op == IR_GATE_ARM && (c)->n_operands > 1) RPO_PUSH((c)->operands[1]); \
        if ((c)->op == IR_GATE_ARM && (c)->n_operands > 0) RPO_PUSH((c)->operands[0]); \
        if ((c)->op == IR_CREATE && (c)->n_operands > 0) RPO_PUSH((c)->operands[0]); \
        if ((c)->op == IR_CLEANUP) { if ((c)->ω.node) RPO_PUSH((c)->ω.node); for (int _ci = (c)->n_operands - 1; _ci >= 0; _ci--) RPO_PUSH((c)->operands[_ci]); } \
        if ((c)->op == IR_SUSPEND && (c)->n_operands > 1) RPO_PUSH((c)->operands[1]); \
if (((c)->op == IR_SUBSCRIPT || (c)->op == IR_RANDOM || (c)->op == IR_DEREF || (c)->op == IR_UNIFY_CONST || (c)->op == IR_UNIFY_STRUCT || (c)->op == IR_UNIFY_FIRST || (c)->op == IR_UNIFY_VALUE || (c \
    )->op == IR_ASSIGN_VAR || (c)->op == IR_REV_ASSIGN_VAR || (c)->op == IR_KW_ASSIGN || (c)->op == IR_SCAN_TAB || (c)->op == IR_SCAN_MOVE || (c)->op == IR_SCAN_POS || (c)->op == IR_SCAN_MATCH || (c \
    )->op == IR_SCAN_ANY || (c)->op == IR_SWAP_VAR || (c)->op == IR_CALL_VALUE || (c)->op == IR_VAR || (c)->op == IR_KW_ICON) && (c)->ω.node) RPO_PUSH((c)->ω.node); \
if (((c)->op == IR_CALL || ir_is_call_kind((c)->op) || (c)->op == IR_PROC_GEN || (c)->op == IR_ACTIVATE || (c)->op == IR_TO || (c)->op == IR_TO_BY || (c)->op == IR_GATE || (c)->op == \
    IR_GOTO_DEFERRED) && (c)->ω.node) RPO_PUSH((c)->ω.node); \
        if ((c)->op == IR_STATEMENT_BEGIN && (c)->ω.node) RPO_PUSH((c)->ω.node); \
if (((c)->op == IR_BINOP || (c)->op == IR_BINOP_TEST || (c)->op == IR_BINOP_RELOP_VAL || (c)->op == IR_UNOP || (c)->op == IR_UNOP_TEST || (c)->op == IR_NULLTEST_VAR || (c)->op == IR_COERCE_STRING || \
    (c)->op == IR_COERCE_INTEGER || (c)->op == IR_COERCE_NUMERIC || (c)->op == IR_COERCE_REAL || (c)->op == IR_CMP_TEST || (c)->op == IR_IDENT || (c)->op == IR_DIFFER) && (c)->ω.node) RPO_PUSH((c) \
    ->ω.node); \
        (void)0;
#define RPO_DRAIN() \
    while (qt > qh) { \
        IR_t *c = g_planner.codegen_flat_chain_body_queue[--qt]; \
        if (c == RPO_TAG_EMIT) { IR_t *e = g_planner.codegen_flat_chain_body_queue[--qt]; if (pn < CH_MAX) g_planner.codegen_flat_chain_body_postv[pn++] = e; continue; } \
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue; \
        if (RPO_VISITED(c)) continue; \
        RPO_MARK(c); \
        if (pn >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); } \
        if (qt + 2 > Q_MAX) { fprintf(stderr, "[GZ-7] FATAL: chain traversal queue saturated at EMIT tag\n"); abort(); } \
        g_planner.codegen_flat_chain_body_queue[qt++] = c; g_planner.codegen_flat_chain_body_queue[qt++] = RPO_TAG_EMIT; \
        RPO_PUSH_SUCCS(c); \
    }
#define RPO_FLUSH() do { for (int _i = pn - 1; _i >= 0; _i--) { if (n >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); } g_planner.codegen_flat_chain_body_nodes[n++] = \
    g_planner.codegen_flat_chain_body_postv[_i]; } pn = 0; } while (0)
    int _stmt_seed = 0; if (cg && entry_is_own_graph_root) for (int _ss = 0; _ss < cg->n; _ss++) { IR_t * _sn = cg->all[_ss];
        if (_sn && _sn->op == IR_STATEMENT_BEGIN) { _stmt_seed = 1; break; } }
    if (_stmt_seed) { if (qt < Q_MAX) g_planner.codegen_flat_chain_body_queue[qt++] = entry; RPO_DRAIN(); RPO_FLUSH();
        for (int _ss = 0; _ss < cg->n; _ss++) { IR_t * _sb = cg->all[_ss];
        if (!_sb || _sb->op != IR_STATEMENT_BEGIN || RPO_VISITED(_sb)) continue; if (qt < Q_MAX) { g_planner.codegen_flat_chain_body_queue[qt++] = _sb; RPO_DRAIN(); RPO_FLUSH(); } } }
    else { RPO_PUSH(entry); RPO_DRAIN(); RPO_FLUSH(); }
    if (cg && cg->n_alts > 1 && cg->alt_entry)
        for (int _ak = 1; _ak < cg->n_alts; _ak++) { IR_t * _ae = cg->alt_entry[_ak]; if (_ae && !RPO_VISITED(_ae)) { RPO_PUSH(_ae); RPO_DRAIN(); RPO_FLUSH(); } }
    { extern int zls_g_group_count(const IR_graph_t *); extern const IR_t * zls_g_group_anchor(const IR_graph_t *, int);
      if (cg && entry_is_own_graph_root) { int _gc = zls_g_group_count(cg); for (int _k = 0; _k < _gc; _k++) { const IR_t * _a = zls_g_group_anchor(cg, _k);
          if (_a && !planner_entry_planned(_a)) { RPO_PUSH(_a); RPO_DRAIN(); RPO_FLUSH(); } } } }
    { if (cg && entry_is_own_graph_root) { int _refd[4] = {0, 0, 0, 0};
          for (int _fi = 0; _fi < cg->n; _fi++) { IR_t * _fn = cg->all[_fi]; int _fk = ir_floater_kind(_fn); if (!_fk || _refd[_fk]) continue;
              for (int _rj = 0; _rj < cg->n && !_refd[_fk]; _rj++) { IR_t * _r = cg->all[_rj];
                  if (_r && _r != _fn && !ir_floater_member(_r) && (_r->γ.node == _fn || _r->ω.node == _fn)) _refd[_fk] = 1; } }
          if (_refd[3]) _refd[1] = _refd[2] = 1;
          for (int _fk = 1; _fk <= 3; _fk++) { if (!_refd[_fk]) continue; for (int _fi = 0; _fi < cg->n; _fi++) { IR_t * _fn = cg->all[_fi];
              if (!_fn || ir_floater_kind(_fn) != _fk || RPO_VISITED(_fn)) continue;
              if (qt < Q_MAX) { g_planner.codegen_flat_chain_body_queue[qt++] = _fn; RPO_DRAIN(); RPO_FLUSH(); } } } } }
    { int _p2_base = 0; do { int _p2_added = 0;
        for (int i = _p2_base; i < n; i++)
            if (ir_is_generator_kind(g_planner.codegen_flat_chain_body_nodes[i]->op) && g_planner.codegen_flat_chain_body_nodes[i]->ω.node &&
            !RPO_VISITED(g_planner.codegen_flat_chain_body_nodes[i]->ω.node)) { RPO_PUSH(g_planner.codegen_flat_chain_body_nodes[i]->ω.node); _p2_added = 1; } _p2_base = n;
        RPO_DRAIN(); RPO_FLUSH(); if (!_p2_added) break; } while (1); }
#undef RPO_FLUSH
#undef RPO_DRAIN
#undef RPO_PUSH_SUCCS
#undef RPO_PUSH
#undef RPO_TAG_EMIT
#undef RPO_MARK
#undef RPO_VISITED
    ir_pset_free(&seenset);
    if (qt >= Q_MAX)
        { fprintf(stderr, "[GZ-7] FATAL: chain traversal queue saturated (qt=%d >= Q_MAX=%d) for prefix=%s -- control-flow edges were silently dropped; raise CH_MAX\n", qt, (int)Q_MAX, prefix);
        abort(); }
    unsigned char *zd_on = (unsigned char *)alloca((size_t)(n > 0 ? n : 1)); int *zd_out = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
        int *zd_gp = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1)); int *zd_wp = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
        int *zd_arm = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    int *zd_cps = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    int *zd_cpd = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    int *zd_chn = (int *)alloca(sizeof(int) * (size_t)(2 * n + 2));
    nidx_build(g_planner.codegen_flat_chain_body_nodes, n);
    int *zd_claim = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    zd_plan(cg, g_planner.codegen_flat_chain_body_nodes, n, zd_on, zd_out, zd_gp, zd_wp, zd_arm, (const unsigned char *)0, zd_cps, zd_cpd, zd_chn, zd_claim);
    int * zd_vl = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    zd_vdj_live(g_planner.codegen_flat_chain_body_nodes, n, zd_vl);
    for (int i = 0; i < n; i++) for (int p = zd_on[i] ? zd_arm[i] : -1, g = 0; p >= 0 && g++ < n; p = zd_arm[p]) if (g_planner.codegen_flat_chain_body_nodes[p]->op == IR_DISJUNCTION) { zd_vl[i] = 0;
        break; }
    int * zd_xb = (int *)0;
    int * zd_cut = (int *)0;
        { unsigned char * _zw = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
            unsigned char * _zarm = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
        int * _zst = (int *)alloca(sizeof(int) * (size_t)(3 * n + 3)); for (int _q = 0; _q < n; _q++) _zw[_q] = 0; auto _in_arms = [&](int t, int k) -> int { int sp = 0;
        for (int q = 0; q < n; q++) _zarm[q] = 0;
            for (int a = 0; a < g_planner.codegen_flat_chain_body_nodes[t]->n_operands; a++) { int q =
            nidx(g_planner.codegen_flat_chain_body_nodes, n, g_planner.codegen_flat_chain_body_nodes[t]->operands[a]); if (q >= 0 && !_zarm[q]) { _zarm[q] = 1;
        _zst[sp++] = q; } } while (sp > 0) { IR_t * x = g_planner.codegen_flat_chain_body_nodes[_zst[--sp]];
        for (int e = 0; e < 2 + x->n_operands; e++) { IR_t * y = e == 0 ? zd_chase(x->γ.node) : e == 1 ? zd_chase(x->ω.node) : x->operands[e - 2];
        int q = (y && y != g_planner.codegen_flat_chain_body_nodes[t]) ? nidx(g_planner.codegen_flat_chain_body_nodes, n, y) : -1; if (q >= 0 && !_zarm[q] && sp < 3 * n + 3) { _zarm[q] = 1;
            _zst[sp++] = q; } } } return _zarm[k]; };
        for (int _it = 0; _it < n; _it++) { int _add = 0;
            for (int k = 0; k < n; k++) { if (g_planner.codegen_flat_chain_body_nodes[k]->op == IR_GOTO) continue;
        for (int v = 0; v < 2; v++) { IR_t * tt = zd_chase(v == 0 ? g_planner.codegen_flat_chain_body_nodes[k]->γ.node : g_planner.codegen_flat_chain_body_nodes[k]->ω.node);
            if (!tt || (v == 1 && port_sz_beta(g_planner.codegen_flat_chain_body_nodes[k]->ω.sz))) continue; int t = nidx(g_planner.codegen_flat_chain_body_nodes, n, tt);
        if (t < 0 || t == k || _zw[t] || !zd_on[t] || zd_arm[t] >= 0) continue;
            int ad = zd_out[t] - zd_k_g(cg, g_planner.codegen_flat_chain_body_nodes[t]) + fence0_release_bytes(g_planner.codegen_flat_chain_body_nodes[t]); int d = 0;
        if (zd_on[k] || ad <= 0 || !strncmp(bb_op_name(g_planner.codegen_flat_chain_body_nodes[k]->op), "IR_MATCH_", 9) ||
            !strncmp(bb_op_name(g_planner.codegen_flat_chain_body_nodes[t]->op), "IR_MATCH_", 9) || _in_arms(t, k)) continue; _zw[t] = 1; _add++;
        if (getenv("SCRIP_ZD_DIAG"))
            fprintf(stderr, "[ZD] JOIN-WALL i=%d (%s) alpha depth %d entered at depth %d by i=%d (%s)\n", t, bb_op_name(g_planner.codegen_flat_chain_body_nodes[t]->op), ad, d, k,
            bb_op_name(g_planner.codegen_flat_chain_body_nodes[k]->op));
        } } if (!_add) break; zd_plan(cg, g_planner.codegen_flat_chain_body_nodes, n, zd_on, zd_out, zd_gp, zd_wp, zd_arm, _zw, zd_cps, zd_cpd, zd_chn, zd_claim); }
        }
    zeta_plan_t * rec = new zeta_plan_t();
    rec->entry = entry; rec->n = n; rec->own_root = entry_is_own_graph_root; rec->empty_fail = flat_empty_body_fail; rec->empty_succ = flat_empty_body_succ;
    rec->nodes = g_planner.codegen_flat_chain_body_nodes;
    rec->on = (unsigned char *)ct_zalloc((size_t)(n > 0 ? n : 1), 1); memcpy(rec->on, zd_on, (size_t)n);
    rec->out = (int *)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(int)); memcpy(rec->out, zd_out, sizeof(int) * (size_t)n);
    rec->gp = (int *)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(int)); memcpy(rec->gp, zd_gp, sizeof(int) * (size_t)n);
    rec->wp = (int *)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(int)); memcpy(rec->wp, zd_wp, sizeof(int) * (size_t)n);
    rec->arm = (int *)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(int)); memcpy(rec->arm, zd_arm, sizeof(int) * (size_t)n);
    rec->cps = (int *)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(int)); memcpy(rec->cps, zd_cps, sizeof(int) * (size_t)n);
    rec->cpd = (int *)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(int)); memcpy(rec->cpd, zd_cpd, sizeof(int) * (size_t)n);
    rec->claim = (int *)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(int)); memcpy(rec->claim, zd_claim, sizeof(int) * (size_t)n);
    rec->vl = (int *)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(int)); memcpy(rec->vl, zd_vl, sizeof(int) * (size_t)n);
    rec->chn = (int *)ct_zalloc((size_t)(2 * n + 2), sizeof(int)); memcpy(rec->chn, zd_chn, sizeof(int) * (size_t)(2 * n + 2));
    if (g_planner.g_zd_zarm_p == zd_arm) g_planner.g_zd_zarm_p = rec->arm;
    if (n > 0) {
        int * _b = (int *)alloca(sizeof(int) * (size_t)n);
        unsigned char * _kn = (unsigned char *)alloca((size_t)n);
        zd_xb = (int *)alloca(sizeof(int) * (size_t)n);
        zd_cut = (int *)alloca(sizeof(int) * (size_t)n);
        int * zd_fam = (int *)alloca(sizeof(int) * (size_t)n);
        int * zd_fbase = (int *)alloca(sizeof(int) * (size_t)n);
        zd_pl_bases(cg, g_planner.codegen_flat_chain_body_nodes, n, zd_on, zd_out, zd_gp, zd_wp, zd_claim, _b, _kn, (int *)0, zd_cut, zd_fam, zd_fbase);
        for (int _i = 0; _i < n; _i++) zd_xb[_i] = (zd_on[_i] && zd_claim[_i] >= 0 && _kn[zd_claim[_i]]) ? _b[zd_claim[_i]] : INT_MIN;
        for (int _i = 0; _i < n; _i++) {
            IR_t * _u = g_planner.codegen_flat_chain_body_nodes[_i], * _gs = _u;
            if (!zd_on[_i] || _u->op != IR_UNMARK || _u->n_operands < 1 || zd_cut[_i] >= 0) continue;
            IR_t * _gt = zd_lm_chase(_u, n, &_gs);
            int _q = _gt ? nidx(g_planner.codegen_flat_chain_body_nodes, n, _gt) : -1;
            if (_q < 0 || !zd_on[_q] || zd_xb[_q] == INT_MIN) continue;
            zd_cut[_i] = zd_xb[_q] + zd_out[_q] - (port_sz_beta(zd_edge_sz(_gs, 0)) ? 0 : zd_k_g(cg, g_planner.codegen_flat_chain_body_nodes[_q]));
        }
        zst_finish(cg, g_planner.codegen_flat_chain_body_nodes, n, zd_out, zd_claim, _b, _kn);
        rec->xb = (int *)ct_zalloc((size_t)n, sizeof(int)); memcpy(rec->xb, zd_xb, sizeof(int) * (size_t)n);
        rec->cut = (int *)ct_zalloc((size_t)n, sizeof(int)); memcpy(rec->cut, zd_cut, sizeof(int) * (size_t)n);
        rec->fam = (int *)ct_zalloc((size_t)n, sizeof(int)); memcpy(rec->fam, zd_fam, sizeof(int) * (size_t)n);
        rec->fbase = (int *)ct_zalloc((size_t)n, sizeof(int)); memcpy(rec->fbase, zd_fbase, sizeof(int) * (size_t)n);
        rec->b = (int *)ct_zalloc((size_t)n, sizeof(int)); memcpy(rec->b, _b, sizeof(int) * (size_t)n);
        rec->kn = (unsigned char *)ct_zalloc((size_t)n, 1); memcpy(rec->kn, _kn, (size_t)n);
        g_planner.g_zd_claim_p = rec->claim; g_planner.g_zd_xb_p = rec->xb; g_planner.g_zd_nodes_p = g_planner.codegen_flat_chain_body_nodes; g_planner.g_zd_n = n; g_planner.g_zd_cut_p = rec->cut;
        g_planner.g_zd_fam_p = rec->fam; g_planner.g_zd_fbase_p = rec->fbase; g_planner.g_zd_gp_p = rec->gp; g_planner.g_zd_out_p = rec->out;
        g_planner.g_zd_wp_p = rec->wp;
    }
    g_planner.g_zd_plan_on = rec->on; g_planner.g_zd_plan_nodes = g_planner.codegen_flat_chain_body_nodes; g_planner.g_zd_plan_n = n;
    if (n > 0) zd_place(cg, rec, g_planner.codegen_flat_chain_body_nodes, n, zd_on, zd_out, zd_arm, zd_cps, zd_cpd);
    planner_facts(rec, g_planner.codegen_flat_chain_body_nodes, n, cg);
    zd_depth_census_g(cg, g_planner.codegen_flat_chain_body_nodes, n, rec->on, rec->out, rec->gp, rec->wp, prefix);
    g_planner.g_zd_rec_p = rec;
    planner_capture(rec);
    return rec;
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
    g_planner.zst_vuse = rec->v_zst_vuse; g_planner.zst_vcov = rec->v_zst_vcov; g_planner.zst_name = rec->v_zst_name; g_planner.zst_sm = rec->v_zst_sm; g_planner.zst_nodes = nodes;
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
extern "C" int zls_retains_why(const IR_graph_t * g, const IR_t * nd);
extern "C" long zsm_declared_word(const IR_graph_t * g, const IR_t * nd, int op_zres, int op_subj_cell, long owner) {
    int n = g_planner.g_zd_plan_n, i = (nd && g_planner.g_zd_plan_nodes && g_planner.g_zd_plan_on) ? nidx(g_planner.g_zd_plan_nodes, n, nd) : -1;
    if (i < 0 || !g_planner.g_zd_plan_on[i]) return 0L;
    int rel = g_planner.g_zd_nodes_p == g_planner.g_zd_plan_nodes && g_planner.g_zd_n == n && g_planner.g_zd_gp_p && g_planner.g_zd_wp_p;
    long k = zd_k_g(g, (IR_t *)nd), gp = rel ? g_planner.g_zd_gp_p[i] : 0, wp = rel ? g_planner.g_zd_wp_p[i] : 0;
    int o = (int)nd->op, ex = zd_check_arm_env(o) || o == IR_UNMARK || o == IR_CUT || o == IR_MATCH_END || o == IR_MATCH_REPLACE;
    IR_t * tg = zd_chase(nd->γ.node), * tw = zd_chase(nd->ω.node);
    int lg = !ex && tg && !zd_check_discard(tg) && !zd_check_arm_env((int)tg->op);
    int lw = !ex && tw && !zd_check_discard(tw) && !zd_check_arm_env((int)tw->op) && !(tw->op == IR_MATCH_BEGIN && zd_sz_phi(zd_edge_sz((IR_t *)nd, 1)));
    int lb = !zd_check_discard(nd) && !zd_check_arm_env(o);
    long own = o == IR_MATCH_BEGIN ? 64L + planner_act_extra(nd) - ((!op_zres && op_subj_cell) ? 16L : 0L) : 0L;
    int end = (o == IR_MATCH_END || o == IR_MATCH_REPLACE) && owner > 0L;
    int ret = zls_retains_why(g, nd) != 0;
    return ((k + own) & 0xFFFFL) | ((gp & 0xFFFFL) << 16) | ((wp & 0xFFFFL) << 32) | (1L << 48) | ((long)rel << 49) | ((long)lg << 50) | ((long)lw << 51) | ((long)lb << 54) |
        ((long)(own != 0L) << 55) | ((long)end << 56) | ((long)ret << 57);
}
extern "C" const char * zls_graph_name_get(const IR_graph_t * g);
extern "C" int zls_g_frame_region(const IR_graph_t * g);
extern "C" int zls_g_resident_bytes(const IR_graph_t * g);
extern "C" int zls_g_key_class(const IR_graph_t * g, int off);
static int zsm_frame_on(void) {
    if (g_planner.zsm_frame_on < 0) { const char * e = getenv("SCRIP_ZSM"); g_planner.zsm_frame_on = (e && *e == '1') ? 1 : 0; }
    return g_planner.zsm_frame_on;
}
static void zsm_frame_report(void) {
    for (const auto & r : g_planner.zsm_frames)
        fprintf(stderr, "[ZSM-FRAME] graph=%s total=%d region=%d resident=%d frames=%d addrs=%d word=%d resident_addrs=%d outside=%d unbacked=%d rsp_form=%d\n", r.name.c_str(), r.total,
            r.region, r.resident, r.frames, r.addrs, r.cls[1], r.cls[2], r.cls[3], r.cls[0], r.rsp_form);
}
static g_planner_t::zsm_frame_rec_t * zsm_frame_rec(const IR_graph_t * g) {
    if (!g) return (g_planner_t::zsm_frame_rec_t *)0;
    int L = g_planner.zsm_frames_last;
    if (L >= 0 && L < (int)g_planner.zsm_frames.size() && g_planner.zsm_frames[(size_t)L].g == g) return &g_planner.zsm_frames[(size_t)L];
    for (size_t k = 0; k < g_planner.zsm_frames.size(); k++) if (g_planner.zsm_frames[k].g == g) { g_planner.zsm_frames_last = (int)k; return &g_planner.zsm_frames[k]; }
    if (g_planner.zsm_frames.empty()) atexit(zsm_frame_report);
    const char * nm = zls_graph_name_get(g);
    g_planner_t::zsm_frame_rec_t r = { g, std::string(nm ? nm : "?"), zls_g_frame_region(g), zls_g_resident_bytes(g), 0, 0, { 0, 0, 0, 0 }, 0, 0 };
    g_planner.zsm_frames.push_back(r);
    g_planner.zsm_frames_last = (int)g_planner.zsm_frames.size() - 1;
    return &g_planner.zsm_frames.back();
}
extern "C" void zsm_frame_addr(const IR_graph_t * g, const IR_t * nd, int off, int cls, int rsp_form) {
    if (!zsm_frame_on()) return;
    g_planner_t::zsm_frame_rec_t * r = zsm_frame_rec(g);
    if (!r) return;
    int c = cls >= 0 ? cls : zls_g_key_class(g, off);
    if (c == 0 && off >= r->region + 16 && off < r->total) c = 1;
    if (g_planner.zsm_frame_diag < 0) { const char * e = getenv("SCRIP_ZSM_FRAME_DIAG"); g_planner.zsm_frame_diag = (e && *e == '1') ? 1 : 0; }
    if (g_planner.zsm_frame_diag && (c == 0 || c == 3))
        fprintf(stderr, "[ZSM-FRAME-ADDR] graph=%s class=%s op=%s off=%d region=%d form=%s\n", r->name.c_str(), c ? "outside" : "unbacked", nd ? bb_op_name(nd->op) : "-", off,
            r->region, rsp_form ? "rsp" : "frame");
    r->addrs++;
    r->cls[c & 3]++;
    r->rsp_form += rsp_form ? 1 : 0;
}
extern "C" void zsm_frame_carve(const IR_graph_t * g, int frame_bytes) {
    if (!zsm_frame_on()) return;
    g_planner_t::zsm_frame_rec_t * r = zsm_frame_rec(g);
    if (!r) return;
    r->frames++;
    if (frame_bytes > r->total) r->total = frame_bytes;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void planner_report_at_exit(void) {
    fprintf(stderr, "[PLANNER-PASS] planned=%d planned_rt=%d hits=%d fallbacks=%d\n", g_planner.planner_planned, g_planner.planner_rt, g_planner.planner_hits, g_planner.planner_fallbacks);
}
static void planner_report_arm(void) { if (!g_planner.planner_report_armed) { g_planner.planner_report_armed = 1; if (getenv("SCRIP_PLANNER_REPORT")) atexit(planner_report_at_exit); } }
void planner_pass_plan(IR_graph_t * g, IR_t * entry, const char * prefix) {
    planner_report_arm();
    zeta_plan_t * rec = planner_chain_plan(g, entry, prefix); rec->cfg = g; rec->key = entry; g_planner.planner_table[entry] = rec; g_planner.planner_planned++;
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
    planner_restore(it->second);
    return it->second;
}
}
int planner_retains(const IR_t * nd) { return planner_retains_g(planner_published_graph(), nd); }
int zd_arrival_depth(IR_t **nodes, int n, int i, const unsigned char *zon, const int *zout, const int *zgp, const int *zwp)
    { return zd_arrival_depth_g(planner_published_graph(), nodes, n, i, zon, zout, zgp, zwp); }
void zd_check(IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, const int *xb, const int *zchn, const int *zcut, const int *zarm,
    const char *prefix){ zd_check_g(planner_published_graph(), nodes, n, zon, zout, zgpop, zwpop, claim, xb, zchn, zcut, zarm, prefix); }
int zd_cl_read(IR_t **nodes, int n, int i, const unsigned char *on, const int *out, const int *xb) { return zd_cl_read_g(planner_published_graph(), nodes, n, i, on, out, xb); }
IR_t * zd_conj_value_node(IR_t * nd) { return zd_conj_value_node_g(planner_published_graph(), nd); }
extern "C" int zd_cross_cell(int off){ return zd_cross_cell_g(planner_published_graph(), off); }
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

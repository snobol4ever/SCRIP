#include "../emitter/emit_head.h"
#include "../emitter/emit_decls.h"
#include "planner.h"
int emit_floater_member(const IR_t * n);
void nidx_build(IR_t **nodes, int n);
int emit_floater_kind(const IR_t * n);
int fence0_release_bytes(const IR_t * nd);
int nidx(IR_t **nodes, int n, const IR_t *e);
int port_sz_beta(const char * sz);
int beta_is_stmt_land(const IR_t * tgt);
int emit_glue_box(const IR_t * t, int phi);
void zdo_build(IR_t **nodes, int n);
int zdo_first(IR_t **nodes, int n, IR_t *t);
void zrm_build(IR_t ** nodes, const int * run, int rl);
int zrm_maxk(const IR_t * nd);
int zst_coord(const char * name);
int zst_s_of(const IR_t * nd);
IR_t * zd_chase(IR_t * t);
int zd_edge_retains(const IR_t * nd);
static int zd_arm_resumes(const IR_t * a, const IR_t * g);
static int zd_gate_holds(const IR_t * g);
static int zd_beta_via_act(const IR_t * nd);
int zd_djx_of(const IR_t * nd);
int zd_upost_of(const IR_t * nd);
int zd_nops(IR_t * nd);
int zd_k(IR_t * nd);
int zd_k(IR_t * nd);
int zd_k_own(IR_t * nd);
int zd_icn_suspend(const IR_t * nd);
static int zd_icn_stmt_head(const IR_t * nd);
int zd_icn_local_var(const IR_t * nd);
int zd_icn_local_assign(const IR_t * nd);
const IR_t * zd_local_home_node(const IR_t * nd);
int zd_k(IR_t * nd);
int zd_icn_suspend(const IR_t * nd);
int zd_k(IR_t * nd);
int zd_nops(IR_t * nd);
int zd_k_own(IR_t * nd);
int zst_s_of(const IR_t * nd);
int zd_k_own(IR_t * nd);
int zd_dj_admitted(const int * zarm, int n, int ei) { for (int j = 0; j < n; j++) if (zarm[j] == ei) return 1; return 0; }
int zd_dj_cellless(IR_t ** nodes, int n, const int * zarm, int k) {
    return k >= 0 && nodes[k]->op == IR_DISJUNCTION && zarm && !zd_dj_admitted(zarm, n, k);
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
IR_t * zd_value_producer(IR_t * nd) {
    for (int g = 0; nd && g < 16 && zd_k_own(nd) == 0 && (nd->op == IR_ASSIGN || nd->op == IR_CONJUNCTION) && nd->n_operands > 0; g++) nd = nd->operands[0];
    return nd;
}
IR_t * zd_conj_value_node(IR_t * nd) { int g = 0; while (nd && nd->op == IR_CONJUNCTION && nd->n_operands > 0 && zd_k_own(nd) == 0 && g++ < 16) nd = nd->operands[0]; return nd; }
int zd_plan_on_of(const IR_t * nd) { if (!nd || !g_emitter.g_zd_plan_on || g_emitter.g_zd_plan_nodes != g_emitter.codegen_flat_chain_body_nodes) return 0;
    for (int k = 0; k < g_emitter.g_zd_plan_n; k++) if (g_emitter.g_zd_plan_nodes[k] == nd) return g_emitter.g_zd_plan_on[k] ? 1 : 0; return 0; }
static int * zd_grow_one(int * p, int o, int cap, int fill) { int * q = (int *)ct_alloc((size_t)cap * sizeof(int)); if (o) memcpy(q, p, (size_t)o * sizeof(int));
    for (int k = o; k < cap; k++) q[k] = fill; return q; }
void zd_ops_reserve(int n) {
    if (n < ZD_NOPS_MIN) n = ZD_NOPS_MIN;
    if (n <= g_emit.op_zcap) return;
    int o = g_emit.op_zcap, cap = o > 0 ? o : ZD_NOPS_MIN; while (cap < n) cap *= 2;
    g_emit.op_zread = zd_grow_one(g_emit.op_zread, o, cap, 0); g_emit.op_zkind = zd_grow_one(g_emit.op_zkind, o, cap, -1); g_emit.op_zread_xf = zd_grow_one(g_emit.op_zread_xf, o, cap, -1);
    g_emit.op_zread_fr = zd_grow_one(g_emit.op_zread_fr, o, cap, -1); g_emitter.g_zd_fread = zd_grow_one(g_emitter.g_zd_fread, o, cap, -1); g_emit.op_zslot = zd_grow_one(g_emit.op_zslot, o, cap, -1);
    g_emitter.g_zd_read = zd_grow_one(g_emitter.g_zd_read, o, cap, 0); g_emitter.g_zd_kind = zd_grow_one(g_emitter.g_zd_kind, o, cap, -1);
        g_emitter.g_zd_anchor = zd_grow_one(g_emitter.g_zd_anchor, o, cap, -1);
    g_emit.op_zcap = cap;
}
int zd_sr_role(const IR_t * nd) { int64_t v = IR_LIT(nd).ival; return (v == 1 || v == 2 || v == 3 || v == 4) ? (int)v : 0; }
int zd_cpos(int i, const int *xb, const int *zout, long *d) {
    const int * cl = g_emitter.g_zd_claim_p, * fm = g_emitter.g_zd_fam_p, * fb = g_emitter.g_zd_fbase_p;
    if (xb[i] != INT_MIN) { *d = xb[i] + zout[i]; return -2; }
    if (cl[i] >= 0 && fm[cl[i]] >= 0) { *d = fb[cl[i]] + zout[i]; return fm[cl[i]]; }
    *d = zout[i];
    return cl[i] >= 0 ? -3 - cl[i] : INT_MIN;
}
int zd_gate_pair(IR_t **nodes, int n, int a, int k, int wantb, const unsigned char *zon, const int *zout, const int *xb, long *rsp) {
    long da = 0, dk = 0;
    int ca = zd_cpos(a, xb, zout, &da), ck = INT_MIN;
    da -= g_emitter.g_zd_gp_p[a];
    if (wantb && zd_edge_retains(nodes[k])) {
        IR_t * c = zd_chase(nodes[k]->γ.node);
        int kc = c ? nidx(nodes, n, c) : -1;
        if (kc >= 0 && zon[kc]) { ck = zd_cpos(kc, xb, zout, &dk); dk -= zd_k(nodes[kc]); }
    }
    else { ck = zd_cpos(k, xb, zout, &dk); if (!wantb) dk -= zd_k(nodes[k]); }
    if (ca == INT_MIN || ca != ck) return 0;
    *rsp = da - dk;
    return 1;
}
int zd_gate_pair_at(IR_t **nodes, int n, int g, int a, int k, int wantb, const unsigned char *zon, const int *zout, const int *xb, long *rsp) {
    if (!zd_gate_pair(nodes, n, a, k, wantb, zon, zout, xb, rsp)) return 0;
    IR_t * c = (g >= 0 && wantb && (nodes[k]->op == IR_DISJUNCTION || (int)ir_kind0(nodes[k]) == IR_CALL_PROC_STAGED)) ? zd_chase(nodes[g]->γ.node) : (IR_t *)0;
    int ci = c ? nidx(nodes, n, c) : -1;
    const int * cl = g_emitter.g_zd_claim_p;
    if (ci >= 0 && ci != g && zon[ci] && zon[g] && cl && cl[ci] >= 0 && cl[ci] == cl[g] && cl[g] != cl[a]) *rsp += zout[g] - (zout[ci] - zd_k(nodes[ci]));
    return 1;
}
int zd_cl_read(IR_t **nodes, int n, int i, const unsigned char *on, const int *out, const int *xb) {
    IR_t *nd = nodes[i];
    if ((int) IR_LIT(nd).ival != CLEANUP_OPEN || nd->n_operands < 2 || !nd->operands[1] || !on[i]) return -1;
    IR_t *p = nd->operands[1];
    IR_t *pv = zd_conj_value_node(p);
    if (pv != p && zd_k_own(pv) >= 16 && nidx(nodes, n, pv) >= 0 && on[nidx(nodes, n, pv)]) p = pv;
    int k = nidx(nodes, n, p);
    if (k < 0 || !on[k] || zd_k_own(p) < 16) return -1;
    return out[i] - out[k] + ((xb && xb[i] != INT_MIN && xb[k] != INT_MIN) ? xb[i] - xb[k] : 0);
}
IR_t * zd_chase(IR_t * t) { int g = 0; while (t && (t->op == IR_GOTO || (t->op == IR_SUCCEED && t->γ.node)) && g++ < 4096) t = t->γ.node; return t; }
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
int zd_nops(IR_t * nd) { int op = (int)ir_kind0(nd);
    if (g_emit_cfg && g_emit_cfg->spine_cells && !zd_icn_suspend(nd) && op != IR_DISJUNCTION && op != IR_GATE && op != IR_GATE_ARM && op != IR_BOUND && op != IR_UNMARK && op != IR_CREATE)
        return (int)nd->n_operands;
    if (emit_resumable_frame() && (op == IR_CALL_PL_LEAF || op == IR_CALL_DIRECT)) return (int)nd->n_operands;
    if (emit_resumable_frame() && (op == IR_UNIFY_CONST || op == IR_UNIFY_STRUCT || op == IR_UNIFY_FIRST || op == IR_UNIFY_VALUE))
        return (nd->n_operands > 0 && nd->operands[0] && nd->operands[0]->op == IR_UNIFY_STRUCT) ? 1 : 0;
    if (op == IR_KW_ASSIGN_SNOBOL4) return 1;
    if (op == IR_ITERATE) return 1;
    if (zd_icn_suspend(nd)) return 1;
    return (op == IR_UNOP || op == IR_ASSIGN || op == IR_DEREF || op == IR_COERCE_STRING || op == IR_COERCE_INTEGER || op == IR_FIELD_VAR || op == IR_FIELD_GET || op == IR_MATCH_BEGIN ||
        op == IR_RETURN) ? 1 :
        (op == IR_BINOP || op == IR_ASSIGN_FRAME || op == IR_COERCE_NUMERIC || op == IR_CMP_TEST || op == IR_IDENT || op == IR_DIFFER || op == IR_BINOP_TEST || op == IR_BINOP_RELOP_VAL) ? 2 :
            (op == IR_MATCH_REPLACE) ? 2 :
        (op == IR_SUBSCRIPT || op == IR_ASSIGN_VAR || op == IR_MAKE_LIST || ir_norm_call_kind((IR_e)op) == IR_CALL || op == IR_PROC_GEN || op == IR_SUSPEND || op == IR_TO ||
            op == IR_TO_BY || op == IR_MATCH_LEN ||
        op == IR_MATCH_ANY || op == IR_MATCH_NOTANY || op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_SPAN || op == IR_MATCH_BREAK ||
        op == IR_MATCH_BREAKX || op == IR_MATCH_ASSIGN_COND || op == IR_MATCH_ASSIGN_IMM || op == IR_MATCH_VALUE) ? (int)nd->n_operands : 0; }
void zd_rec_arm(IR_t ** nodes, int i) {
    g_emitter.g_zd_grec_off = -1;
    g_emitter.g_zd_wrec_off = -1;
    if (i < 0 || i >= (int)g_emitter.g_zd_grec.size()) return;
    int gd = g_emitter.g_zd_grec[(size_t)i], wd = g_emitter.g_zd_wrec[(size_t)i];
    int gs = gd >= 0 ? xop_frame_slot(nodes[gd]) : -1, ws = wd >= 0 ? xop_frame_slot(nodes[wd]) : -1;
    if (gs != -1) { g_emitter.g_zd_grec_off = gs + 24; g_emitter.g_zd_grec_res = g_emitter.g_zd_grec_pop[(size_t)i]; }
    if (ws != -1) { g_emitter.g_zd_wrec_off = ws + 24; g_emitter.g_zd_wrec_res = g_emitter.g_zd_wrec_pop[(size_t)i]; }
}
void zd_cp_arm(IR_t ** nodes, int i, const int * zcps, const int * zcpd, const int * zout) {
    g_emitter.g_zd_cpsx = -1;
    g_emitter.g_zd_cpdx = -1;
    if (zcpd[i] > -2) {
        if (zcps[i] > -2) return;
        int k = -zcps[i] - 2, sx = xop_frame_slot(nodes[k]);
        if (sx == -1 && zd_value_producer(nodes[k]) != nodes[k]) sx = xop_frame_slot(zd_value_producer(nodes[k]));
        if (sx != -1) { g_emitter.g_zd_cpsx = sx; g_emitter.g_zd_cps = 0; }
        else g_emitter.g_zd_cps = zout[i] - zout[k];
        return;
    }
    int d = -zcpd[i] - 2, k = zcps[i] <= -2 ? -zcps[i] - 2 : -1;
    int dx = xop_frame_slot(nodes[d]), sx = k >= 0 ? xop_frame_slot(nodes[k]) : -1;
    g_emitter.g_zd_cpd = -1;
    g_emitter.g_zd_cps = -1;
    if (dx == -1 || k < 0) return;
    g_emitter.g_zd_cpdx = dx;
    if (sx != -1) g_emitter.g_zd_cpsx = sx;
    else g_emitter.g_zd_cps = zout[i] - zout[k];
}
int zd_icn_local_var(const IR_t * nd) { return nd && nd->op == IR_VAR && g_emit_cfg && g_emit_cfg->spine_cells && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '&' &&
    (!is_global(IR_LIT(nd).sval) || graph_has_local(g_emit_cfg, IR_LIT(nd).sval)); }
int zd_icn_local_assign(const IR_t * nd) { return nd && nd->op == IR_ASSIGN && g_emit_cfg && g_emit_cfg->spine_cells && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '&' &&
    (!is_global(IR_LIT(nd).sval) || graph_has_local(g_emit_cfg, IR_LIT(nd).sval)); }
const IR_t * zd_local_home_node(const IR_t * nd) { int g = 0;
    while (nd && (nd->op == IR_CONJUNCTION || (nd->op == IR_ASSIGN && !zd_icn_local_assign(nd))) && nd->n_operands > 0 && g++ < 16) nd = nd->operands[0];
    return (zd_icn_local_var(nd) || zd_icn_local_assign(nd)) ? nd : (const IR_t *)0; }
static int zd_icn_chain_step_ok(IR_t ** nodes, int n, int p, int q) {
    if (!(g_emit_cfg && g_emit_cfg->spine_cells)) return 1;
    if (p < 0 || q < 0 || zd_chase(nodes[p]->ω.node) != nodes[q]) return 0;
    switch (nodes[p]->op) {
        case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_REAL: case IR_VAR: case IR_VAR_REF: case IR_DEREF: case IR_COERCE_NUMERIC: case IR_BINOP: case IR_BINOP_TEST: case IR_UNOP: case IR_ASSIGN:
        case IR_CALL_BYNAME: case IR_CALL_DIRECT: case IR_LINE_MARK: return 1;
        default: return 0;
    }
}
int zd_icn_suspend(const IR_t * nd) { return nd && nd->op == IR_SUSPEND && g_emit_cfg && g_emit_cfg->spine_cells; }
static int zd_icn_stmt_head(const IR_t * nd) { extern const char * bb_src_of(const IR_t *); const char * s = (nd && g_emit_cfg && g_emit_cfg->spine_cells) ? bb_src_of(nd) : (const char *)0;
    return (s && (!strcmp(s, "stmt") || !strcmp(s, "nstmt"))) ? 1 : 0; }
static int zd_icn_stmt_sibling(const IR_t * nd) { extern const char * bb_src_of(const IR_t *); const char * s = (nd && g_emit_cfg && g_emit_cfg->spine_cells) ? bb_src_of(nd) : (const char *)0;
    return (s && !strcmp(s, "stmt")) ? 1 : 0; }
int zd_djx_of(const IR_t * nd) { if (!nd || nd->op != IR_DISJUNCTION || g_emitter.g_zd_djx_nodes != g_emitter.codegen_flat_chain_body_nodes) return 0;
    int i = nidx(g_emitter.g_zd_djx_nodes, (int)g_emitter.g_zd_djx.size(), nd); return i >= 0 ? g_emitter.g_zd_djx[(size_t)i] : 0; }
int zd_upost_of(const IR_t * nd) { if (!nd || nd->op != IR_UNMARK || g_emitter.g_zd_djx_nodes != g_emitter.codegen_flat_chain_body_nodes) return 0;
    int i = nidx(g_emitter.g_zd_djx_nodes, (int)g_emitter.g_zd_upost.size(), nd); return i >= 0 ? g_emitter.g_zd_upost[(size_t)i] : 0; }
int zd_k(IR_t * nd) { return zd_k_own(nd) + zst_s_of(nd) + zd_djx_of(nd); }
static int zd_dj_spine(const IR_t * nd) { if (!g_emitter.g_zd_zarm_p || g_emitter.g_zd_zarm_nodes != g_emitter.codegen_flat_chain_body_nodes) return 0;
    int i = nidx(g_emitter.g_zd_zarm_nodes, g_emitter.g_zd_zarm_n, nd); return (i >= 0 && zd_dj_admitted(g_emitter.g_zd_zarm_p, g_emitter.g_zd_zarm_n, i) && !zls_dj_retains(nd)) ? 32 : 0; }
int zd_call_curmov(const IR_t * nd) { return lower_call_node_is_curmov(nd); }
enum { BCS_ALWAYS, BCS_CALL_CURMOV, BCS_ICN_LOCAL, BCS_ICN_SUSPEND, BCS_HAS_OPERANDS, BCS_NO_OPERANDS, BCS_PATV, BCS_HOME_FRAME, BCS_HOME_STATE, BCS_PL_HOME, BCS_PL_HOME_DYN, BCS_PL_HOME_RANGE_HDR,
    BCS_PL_HOME_HAS_OPERANDS };
extern "C" int zls_g_home_frame(const IR_graph_t * g);
extern "C" int zls_home_why(const IR_t * nd);
extern "C" int planner_home_dyn(const IR_graph_t * g, const IR_t * nd);
static int zd_pl_home(void) { return (zls_g_home_frame(g_emit_cfg) && zls_g_resumable_frame(g_emit_cfg)) ? 1 : 0; }
static int zd_pl_home_dyn(IR_t * nd) { return zd_pl_home() && (zls_home_why(nd) || zd_edge_retains(nd) || planner_home_dyn(g_emit_cfg, nd)) ? 1 : 0; }
#include "box_cell_shapes.inc"
static int zd_bcs_when(IR_t * nd, int w) {
    if (w == BCS_ALWAYS) return 1;
    if (w == BCS_CALL_CURMOV) return zd_call_curmov(nd);
    if (w == BCS_ICN_LOCAL) return zd_icn_local_var(nd);
    if (w == BCS_ICN_SUSPEND) return zd_icn_suspend(nd);
    if (w == BCS_HAS_OPERANDS) return nd->n_operands > 0;
    if (w == BCS_NO_OPERANDS) return nd->n_operands == 0;
    if (w == BCS_HOME_FRAME) return zls_g_home_frame(g_emit_cfg) ? 1 : 0;
    if (w == BCS_HOME_STATE) return (zls_g_home_frame(g_emit_cfg) && zls_home_why(nd)) ? 1 : 0;
    if (w == BCS_PL_HOME) return zd_pl_home();
    if (w == BCS_PL_HOME_DYN) return zd_pl_home_dyn(nd);
    if (w == BCS_PL_HOME_RANGE_HDR) return (zd_pl_home() && nd->op == IR_TO && (ir_range_kind_is(nd, "db") || ir_range_trailed(nd))) ? 1 : 0;
    if (w == BCS_PL_HOME_HAS_OPERANDS) return (zd_pl_home() && nd->n_operands > 0) ? 1 : 0;
    return (w == BCS_PATV && nd->pat_static && IR_LIT(nd).sval && !strncmp(IR_LIT(nd).sval, "PATV$", 5)) ? 1 : 0;
}
extern "C" int zls_off(const IR_t * nd);
extern "C" int zd_cross_cell(int off) {
    int n = g_emitter.g_zd_n, i = g_emitter.g_zd_cur;
    IR_t ** nodes = g_emitter.g_zd_nodes_p;
    const int * out = g_emitter.g_zd_out_p, * xb = g_emitter.g_zd_xb_p, * cl = g_emitter.g_zd_claim_p;
    const unsigned char * on = g_emitter.g_zd_plan_on;
    if (!nodes || !out || !on || g_emitter.g_zd_plan_nodes != nodes || i < 0 || i >= n || !on[i]) return -1;
    for (int b = 0; b < i; b++) {
        if (!on[b]) continue;
        int key = zls_off(nodes[b]), kw = zd_k_own(nodes[b]);
        if (key < 0 || off < key || off >= key + kw) continue;
        long xi, xo;
        if (xb && xb[i] != INT_MIN && xb[b] != INT_MIN) { xi = (long)xb[i] + out[i]; xo = (long)xb[b] + out[b]; }
        else if (cl && cl[i] >= 0 && cl[i] == cl[b]) { xi = out[i]; xo = out[b]; }
        else return -1;
        return xi < xo ? -1 : (int)(xi - xo) + (off - key);
    }
    return -1;
}
static int zd_bcs_words(const IR_t * nd, int w) {
    if (w == -1) return zd_dj_spine(nd);
    if (w == -2) return zd_dj_spine(nd) ? zd_dj_spine(nd) + 32 : 0;
    return w;
}
int zd_k_own(IR_t * nd) {
    int op = (int)nd->op;
    for (size_t r = 0; r < sizeof box_cell_shapes / sizeof box_cell_shapes[0]; r++) {
        if ((box_cell_shapes[r].op >= 0 && box_cell_shapes[r].op != op) || !zd_bcs_when(nd, box_cell_shapes[r].when)) continue;
        return zd_bcs_words(nd, box_cell_shapes[r].words);
    }
    return 16;
}
static int zd_map_on(void) { if (g_emitter.zd_map_on__m < 0) { const char * e = getenv("SCRIP_ZD_MAP"); g_emitter.zd_map_on__m = (e && *e == '1') ? 1 : 0; } return g_emitter.zd_map_on__m; }
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
int zd_arrival_depth(IR_t **nodes, int n, int i, const unsigned char *zon, const int *zout, const int *zgp, const int *zwp) {
    int d = -1;
    for (int j = 0; j < n; j++) {
        int e = -1;
        if (zd_chase(nodes[j]->ω.node) == nodes[i]) e = zon[j] ? zout[j] - zd_k(nodes[j]) - zwp[j] : -2;
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
static IR_t * zd_dj_res(IR_t * dj, int j) {
    int na = (int)IR_LIT(dj).ival;
    IR_t * rj = (2 * na + j < dj->n_operands) ? dj->operands[2 * na + j] : (IR_t *)0;
    int g = 0;
    while (rj && rj->op == IR_CONJUNCTION && rj->n_operands > 0 && g++ < 16) rj = rj->operands[0];
    return rj;
}
static int zd_unmark_acts(const IR_t * nd) {
    const IR_t * sc = nd;
    if (sc->n_operands > 0 && sc->operands[0] && sc->operands[0]->op == IR_BOUND) sc = sc->operands[0];
    return g_emit_cfg ? zls_g_release_act_offs(g_emit_cfg, sc, 5, (int *)0, 0) : 0;
}
static IR_t * zd_lm_chase(IR_t * x, int n, IR_t ** last) { IR_t * s = x; IR_t * t = zd_chase(x->γ.node); int g = 0;
    while (t && t->op == IR_LINE_MARK && g++ <= n) { s = t; t = zd_chase(t->γ.node); } *last = s; return t; }
static int zd_loop_back_unmark(IR_t ** nodes, const int * run, int rb, const IR_t * head) {
    if (!head || head->op != IR_BOUND) return -1;
    int k = rb;
    while (k > 0 && nodes[run[k]]->op == IR_LINE_MARK && zd_chase(nodes[run[k - 1]]->γ.node) == nodes[run[k]]) k--;
    IR_t * u = nodes[run[k]];
    if (u->op != IR_UNMARK || u->n_operands < 1 || u->operands[0] != head || zd_unmark_acts(u) > 0) return -1;
    return k;
}
static int zd_fails_to(IR_t * x, IR_t * t, int depth) {
    if (!x || depth > 16) return 0;
    if (x == t) return 0;
    int o = (int)x->op;
    if (!(o == IR_LINE_MARK || o == IR_BOUND || (o == IR_UNMARK && zd_unmark_acts(x) <= 0))) return 0;
    IR_t * g = zd_chase(x->γ.node);
    IR_t * w = zd_chase(x->ω.node);
    if (g != w) return 0;
    if (g == t) { const char * sz = zd_edge_sz(x, 0); return port_sz_beta(sz) || zd_sz_phi(sz); }
    return zd_fails_to(g, t, depth + 1);
}
static int zd_chain_link(IR_t ** nodes, int q, int t, const unsigned char * djok) {
    IR_t * qo = zd_chase(nodes[q]->ω.node);
    if (qo && qo != nodes[t] && zd_fails_to(qo, nodes[t], 0)) return 1;
    const char * qs = zd_edge_sz(nodes[q], 1);
    if (nodes[q]->op == IR_DISJUNCTION) return djok[q] && (qo == nodes[t] || (qo && qo == zd_chase(nodes[q]->γ.node)));
    if (ir_is_generator_kind(nodes[q]->op) || nodes[q]->op == IR_TO) return 0;
    if (qo == nodes[t] && (port_sz_beta(qs) || zd_sz_phi(qs))) return 1;
    int o = (int)nodes[q]->op;
    int never_fails = o == IR_ASSIGN || o == IR_VAR || o == IR_VAR_REF || o == IR_LINE_MARK || o == IR_BOUND || o == IR_LIT_INTEGER || o == IR_LIT_STRING || o == IR_LIT_REAL ||
        o == IR_LIT_CHARSET;
    return !qo || qo == zd_chase(nodes[q]->γ.node) || never_fails;
}
static int zd_pred(IR_t ** nodes, int n, const int * claim, int hi, int k) {
    int p = -1;
    for (int j = 0; j < n; j++) {
        if (claim[j] != hi || j == k || zd_chase(nodes[j]->γ.node) != nodes[k]) continue;
        if (zd_icn_suspend(nodes[j])) continue;
        const char * sz = zd_edge_sz(nodes[j], 0);
        if (port_sz_beta(sz) || zd_sz_sigma(sz) || zd_sz_phi(sz)) continue;
        if (p >= 0) return -2;
        p = j;
    }
    return p;
}
static int zd_pred_bt(IR_t ** nodes, int n, const int * claim, int hi, int k) {
    int p = zd_pred(nodes, n, claim, hi, k);
    for (int g = 0; p >= 0 && nodes[p]->op == IR_LINE_MARK && g++ <= n; ) p = zd_pred(nodes, n, claim, hi, p);
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
int planner_retains(const IR_t * nd) {
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
    if (zls_fcn_hunres(nd) && k0 == IR_CALL_PROC_STAGED && zls_g_resumable_frame(g_emit_cfg)) return PR_UNRESOLVED_CALLEE;
    return 0;
}
int zd_edge_retains(const IR_t * nd) {
    if (!nd || !emit_resumable_frame()) return 0;
    int r = planner_retains(nd);
    return (r == PR_UNKNOWN_CALLEE || r == PR_GATE_HOLDS || r == PR_DISJUNCTION_HOLDS || r == PR_DB_GENERATOR || r == PR_RETAINING_TARGET) ? 1 : 0;
}
extern "C" int planner_edge_retains(const IR_graph_t * g, const IR_t * nd) { IR_graph_t * sv = g_emit_cfg; g_emit_cfg = (IR_graph_t *)g; int r = zd_edge_retains(nd); g_emit_cfg = sv; return r; }
extern "C" int planner_span_retains(const IR_graph_t * g, int from_ai, int to_ai) {
    if (!g || !g->all || g->n <= 0) return 0;
    if (from_ai > to_ai) { int t = from_ai; from_ai = to_ai; to_ai = t; }
    if (from_ai < 0) from_ai = 0;
    if (to_ai >= g->n) to_ai = g->n - 1;
    IR_graph_t * sv = g_emit_cfg; g_emit_cfg = (IR_graph_t *)g; int r = 0;
    for (int k = from_ai; k <= to_ai && !r; k++) {
        const IR_t * nd = g->all[k];
        if (nd && (zd_edge_retains(nd) || nd->op == IR_SUSPEND || (nd->op == IR_DISJUNCTION && zls_dj_retains(nd)))) r = 1;
    }
    g_emit_cfg = sv; return r;
}
int planner_retains_emitter_view(int op) { return op == IR_PROC_GEN || op == IR_CALL_VALUE || op == IR_CALL_GEN || op == IR_ACTIVATE; }
extern "C" int planner_home_dyn(const IR_graph_t * g, const IR_t * nd) {
    IR_graph_t * sv = g_emit_cfg; g_emit_cfg = (IR_graph_t *)g;
    int r = planner_retains(nd);
    if (r == PR_GATE_HOLDS || r == PR_DISJUNCTION_HOLDS || r == PR_DB_GENERATOR || r == PR_RETAINING_TARGET) {
        int u = zls_fcn_hunres(nd) && (int)ir_kind0(nd) == IR_CALL_PROC_STAGED && zls_g_resumable_frame(g);
        r = zls_fcn_hret(nd) ? PR_RETAINING_CALLEE : u ? PR_UNRESOLVED_CALLEE : 0;
    }
    g_emit_cfg = sv;
    return (r == PR_SUSPEND || r == PR_COEXPR || r == PR_UNKNOWN_CALLEE || r == PR_GENERATOR_CALLEE || r == PR_DEFERRED_PATTERN || r == PR_RETAINING_CALLEE || r == PR_UNRESOLVED_CALLEE) ? r : 0;
}
static int zd_beta_via_act(const IR_t * nd) { return zd_edge_retains(nd) && nd->op != IR_TO; }
static int zd_pl_cut(const IR_t * nd) { return nd->op == IR_CUT && !IR_LIT(nd).ival && emit_resumable_frame(); }
static int zd_pl_unmark_bound(IR_t ** nodes, int n, int i) {
    IR_t * nd = nodes[i];
    if (nd->op != IR_UNMARK || nd->n_operands < 1 || !emit_resumable_frame() || !(IR_LIT(nd).ival & 6)) return -1;
    return nidx(nodes, n, nd->operands[0]);
}
static int zd_pl_restores_head(IR_t ** nodes, int n, const int * claim, int h) { int ub = zd_pl_unmark_bound(nodes, n, h); return ub >= 0 && claim[ub] >= 0 && claim[ub] != h; }
static int zd_pl_edges(IR_t * nd) { return 2 + ((nd->op == IR_DISJUNCTION || nd->op == IR_GATE) ? nd->n_operands : 0); }
static int zd_pl_land(IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, int i, int e, int *q, int *beta) {
    IR_t * nd = nodes[i], * t = (IR_t *)0;
    int l = 0;
    *beta = 0;
    if (e == 0) {
        IR_t * ig = (nd->op == IR_GATE_ARM && !nd->γ.node && nd->n_operands > 1) ? nd->operands[1] : (IR_t *)0;
        t = zd_chase(ig ? ig->γ.node : nd->γ.node);
        *beta = ig ? port_sz_beta(ig->γ.sz) : port_sz_beta(zd_edge_sz(nd, 0));
        l = zout[i] - zgpop[i];
    }
    else if (e == 1) { t = zd_chase(nd->ω.node); *beta = port_sz_beta(zd_edge_sz(nd, 1)); l = zout[i] - zd_k(nd) - zwpop[i]; }
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
static int zd_pl_entry(IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, int i, int e, int *l, const unsigned char *dead) {
    int q, b;
    if (e == 0 && zd_edge_retains(nodes[i])) return -1;
    if (e == 0 && zd_pl_dj_join(nodes, n, i, nidx(nodes, n, zd_chase(nodes[i]->γ.node)))) return -1;
    if (e >= 2 && nodes[i]->op == IR_DISJUNCTION && g_emit_cfg && g_emit_cfg->spine_cells && (((e - 2) & 1) || e - 2 >= 2 * (int)IR_LIT(nodes[i]).ival)) return -1;
    (void) zd_pl_land(nodes, n, zon, zout, zgpop, zwpop, i, e, &q, &b);
    if (q < 0 || claim[q] < 0 || claim[q] == claim[i] || b || (claim[q] != q && !(dead && dead[claim[q]]))) return -1;
    int v = (e == 1) ? ((nodes[i]->op == IR_BOUND && emit_resumable_frame()) ? zout[i] : zout[i] - zd_k(nodes[i])) : (e >= 2 && nodes[i]->op == IR_DISJUNCTION) ? zout[i] - zd_djx_of(nodes[i]) :
        zout[i];
    IR_t * r = (e == 0 && nodes[i]->op == IR_GATE_ARM && nodes[i]->n_operands > 0 && (int) nodes[i]->seal) ? nodes[i]->operands[0] : (IR_t *)0;
    int rq = r ? nidx(nodes, n, r) : -1;
    if (rq >= 0 && zon[rq] && claim[rq] == claim[i]) v = zout[rq];
    int hq = claim[q];
    *l = v - (zout[q] - zd_k(nodes[q])) + (zout[hq] - zd_k(nodes[hq]));
    return hq;
}
void zd_pl_bases(IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, int *base, unsigned char *known, int *aof, int *zcut,
    int *fam, int *fbase) {
    int * rpo = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    int * ord = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    int * stk = (int *)alloca(sizeof(int) * (size_t)(2 * n + 2));
    unsigned char * st = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
    for (int i = 0; i < n; i++) { base[i] = 0; known[i] = 0; rpo[i] = -1; st[i] = 0; if (aof) aof[i] = -1; if (fam) { fam[i] = -1; fbase[i] = 0; } }
    if (n <= 0 || !zon[0] || claim[0] < 0) return;
    int pl = emit_resumable_frame();
    int no = 0, sp = 0, nalt = g_emit_cfg ? g_emit_cfg->n_alts : 0, ew = 8;
    for (int i = 0; i < n; i++) if (zd_pl_edges(nodes[i]) > ew) ew = zd_pl_edges(nodes[i]);
    unsigned char * dead = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
    for (int i = 0; i < n; i++) dead[i] = (unsigned char)(zon[i] && claim[i] == i && i != 0);
    for (int i = 0; i < n; i++) for (int e = 0; zon[i] && e < zd_pl_edges(nodes[i]); e++) { int q, b; (void) zd_pl_land(nodes, n, zon, zout, zgpop, zwpop, i, e, &q, &b);
        if (q >= 0 && q != i) dead[q] = 0; }
    int stmtg = 0;
    for (int i = 0; i < n; i++) if (nodes[i]->op == IR_STATEMENT_BEGIN || nodes[i]->op == IR_SETEXIT_TEST || nodes[i]->op == IR_STMT_MARK) stmtg = 1;
    auto absolute_pass = [&](const unsigned char * xi) {
    no = 0; sp = 0;
    for (int i = 0; i < n; i++) { base[i] = 0; known[i] = 0; rpo[i] = -1; st[i] = 0; if (aof) aof[i] = -1; }
    for (int a0 = 0; a0 <= nalt; a0++) {
        int a = (a0 < nalt) ? a0 : -1;
        int r = (a < 0) ? claim[0] : -1;
        if (a >= 0 && g_emit_cfg->alt_entry && g_emit_cfg->alt_entry[a]) { int q = nidx(nodes, n, zd_chase(g_emit_cfg->alt_entry[a])); if (q >= 0 && zon[q] && claim[q] >= 0) r = claim[q]; }
        if (r < 0 || st[r] || (xi && xi[r])) continue;
        known[r] = 1;
        if (aof) aof[r] = a;
        stk[sp++] = r; stk[sp++] = 0; st[r] = 1;
    while (sp > 0) {
        int h = stk[sp - 2], pos = stk[sp - 1], pushed = 0;
        for (; pos < n * ew && !pushed; pos++) {
            int i = pos / ew, e = pos % ew, l;
            if (i >= n || claim[i] != h || !zon[i] || e >= zd_pl_edges(nodes[i])) continue;
            int q = zd_pl_entry(nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
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
                int l, q = zd_pl_entry(nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                if (q < 0 || (xi && xi[q]) || rpo[q] <= rpo[h]) continue;
                if (stmtg) { base[q] = 0; known[q] = 1; continue; }
                int c = base[h] + l - (zout[q] - zd_k(nodes[q]));
                if (zd_icn_stmt_sibling(nodes[q]) && zd_icn_stmt_head(nodes[h])) c = base[h];
                if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-BASES] h=%d i=%d e=%d q=%d l=%d c=%d known=%d base=%d\n", h, i, e, q, l, c, known[q], base[q]);
                if (known[q] && ((g_emit_cfg && g_emit_cfg->spine_cells) ? c >= base[q] : c <= base[q])) continue;
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
            if (!zon[i] || claim[i] < 0 || !zd_edge_retains(nodes[i])) continue;
            IR_t * t = zd_chase(nodes[i]->γ.node);
            int q = t ? nidx(nodes, n, t) : -1;
            if (q < 0 || !zon[q] || claim[q] < 0 || indef[claim[q]]) continue;
            int ub = (pl && claim[q] == q && nodes[q]->op == IR_UNMARK && (IR_LIT(nodes[q]).ival & 6) && nodes[q]->n_operands > 0) ? nidx(nodes, n, nodes[q]->operands[0]) : -1;
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
                    (void) zd_pl_land(nodes, n, zon, zout, zgpop, zwpop, i, e, &q, &b);
                    if (q < 0 || claim[q] < 0 || indef[claim[q]] || (b && zd_beta_via_act(nodes[q])) || (e == 0 && zd_pl_dj_join(nodes, n, i, q))) continue;
                    int r = claim[q];
                    if (r != q && !(pl && nodes[q]->op != IR_DISJUNCTION)) continue;
                    int ub = (r == q && nodes[r]->op == IR_UNMARK && (IR_LIT(nodes[r]).ival & 6) && nodes[r]->n_operands > 0) ? nidx(nodes, n, nodes[r]->operands[0]) : -1;
                    if (ub >= 0 && zon[ub] && claim[ub] >= 0 && !indef[claim[ub]] && known[claim[ub]]) continue;
                    indef[r] = 1; wl[wn++] = r;
                }
            }
        }
        for (int r = 0; r < n; r++) if (indef[r]) known[r] = 0;
        if (pl) absolute_pass(indef);
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
                            int l, q = zd_pl_entry(nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                            if (q != h) continue;
                            base[h] = base[claim[i]] + l - (zout[h] - zd_k(nodes[h])); known[h] = 1; moved = 1; break;
                        }
                    }
                }
            }
            for (int h = 0; h < n; h++) if (zon[h] && claim[h] == h && !known[h] && !indef[h]) indef[h] = 1;
        }
        if (pl && fam) for (int ph = 0; ph < 2; ph++) {
            for (int i = 0; i < n; i++) {
                if (!zon[i] || claim[i] < 0 || !zd_edge_retains(nodes[i])) continue;
                int q, b;
                (void) zd_pl_land(nodes, n, zon, zout, zgpop, zwpop, i, 0, &q, &b);
                if (q < 0 || !zon[q] || claim[q] < 0 || known[claim[q]] || fam[claim[q]] >= 0 || (claim[q] == claim[i]) != (ph == 1)) continue;
                fam[claim[q]] = claim[q]; fbase[claim[q]] = 0 - (zout[q] - (b ? 0 : zd_k(nodes[q])));
            }
            for (int pass = 0, moved = 1; moved && pass < n + 2; pass++) {
                moved = 0;
                for (int h = 0; h < n; h++) {
                    if (!zon[h] || claim[h] != h || fam[h] < 0) continue;
                    for (int i = 0; i < n; i++) {
                        if (!zon[i] || claim[i] != h) continue;
                        for (int e = 0; e < zd_pl_edges(nodes[i]); e++) {
                            int l, q = zd_pl_entry(nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                            if (q >= 0 && !known[q] && fam[q] < 0 && !zd_pl_restores_head(nodes, n, claim, q)) { fam[q] = fam[h]; fbase[q] = fbase[h] + l - (zout[q] - zd_k(nodes[q])); moved = 1; }
                            if (e == 1 || (e >= 2 && nodes[i]->op != IR_DISJUNCTION) || zd_edge_retains(nodes[i])) continue;
                            int b2, q2;
                            (void) zd_pl_land(nodes, n, zon, zout, zgpop, zwpop, i, e, &q2, &b2);
                            if (q2 < 0 || !zon[q2] || claim[q2] < 0 || claim[q2] == h || known[claim[q2]] || fam[claim[q2]] >= 0 || (b2 && zd_beta_via_act(nodes[q2])) ||
                                zd_pl_restores_head(nodes, n, claim, claim[q2])) continue;
                            int ld = (nodes[i]->op == IR_GATE_ARM) ? 0 : fbase[h] + zout[i] - zgpop[i];
                            fam[claim[q2]] = fam[h]; fbase[claim[q2]] = ld - (zout[q2] - (b2 ? 0 : zd_k(nodes[q2]))); moved = 1;
                        }
                    }
                }
                for (int h = 0; h < n; h++) {
                    if (!zon[h] || claim[h] != h || known[h] || fam[h] >= 0) continue;
                    int ub = (nodes[h]->op == IR_UNMARK && nodes[h]->n_operands > 0) ? nidx(nodes, n, nodes[h]->operands[0]) : -1;
                    if (ub < 0 || !zon[ub] || claim[ub] < 0 || claim[ub] == h || fam[claim[ub]] < 0) continue;
                    fam[h] = fam[claim[ub]]; fbase[h] = fbase[claim[ub]] + zout[ub] - zout[h]; moved = 1;
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
                if (zd_edge_retains(nodes[i])) { IR_t * t = zd_chase(nodes[i]->γ.node); q = t ? nidx(nodes, n, t) : -1; a = known[h] ? base[h] + zout[i] : anchor[h];
                    if (pl && q >= 0 && zon[q] && claim[q] >= 0) q = claim[q]; }
                else if (indef[h] && anchor[h] >= 0) {
                    for (int e = 0; e < zd_pl_edges(nodes[i]); e++) { int b2, q2; if (e == 1 || (e >= 2 && nodes[i]->op != IR_DISJUNCTION)) continue;
                        (void) zd_pl_land(nodes, n, zon, zout, zgpop, zwpop, i, e, &q2, &b2);
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
            if (!zon[c] || claim[c] < 0 || !zd_pl_cut(nodes[c])) continue;
            int h = claim[c], D = known[h] ? base[h] + zout[c] - zd_k(nodes[c]) : anchor[h];
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
                    int l, q2 = zd_pl_entry(nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                    if (q2 >= 0 && !known[q2] && !inp[q2]) { inp[q2] = 1; wl[wn2++] = q2; }
                }
            }
            auto rest = [&](int r) { int ub = (nodes[r]->op == IR_UNMARK && (IR_LIT(nodes[r]).ival & 6) && nodes[r]->n_operands > 0) ? nidx(nodes, n, nodes[r]->operands[0]) : -1;
                return ub >= 0 && zon[ub] && claim[ub] >= 0 && (inp[claim[ub]] || (known[claim[ub]] && !indef[claim[ub]])); };
            for (int moved = 1; moved; ) {
                moved = 0;
                for (int i = 0; i < n; i++) {
                    IR_t * rt = (zon[i] && claim[i] >= 0 && zd_edge_retains(nodes[i])) ? zd_chase(nodes[i]->γ.node) : (IR_t *)0;
                    int rq = rt ? nidx(nodes, n, rt) : -1;
                    if (rq >= 0 && zon[rq] && claim[rq] >= 0 && inp[claim[rq]] && !rest(claim[rq])) { inp[claim[rq]] = 0; moved = 1; }
                }
                for (int i = 0; i < n; i++) for (int e = 0; zon[i] && claim[i] >= 0 && !inp[claim[i]] && e < zd_pl_edges(nodes[i]); e++) {
                    int l, q2 = zd_pl_entry(nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                    if (q2 < 0 || q2 == r0 || !inp[q2] || (i == c && e == 0) || (known[claim[i]] && !indef[claim[i]] && !zd_edge_retains(nodes[i])) || rest(q2)) continue;
                    inp[q2] = 0; moved = 1;
                }
            }
            if (!inp[r0]) continue;
            base[r0] = D - (zout[q] - zd_k(nodes[q])); known[r0] = 1; indef[r0] = 0;
            wl[wn2++] = r0;
            while (wn2 > 0) {
                int r = wl[--wn2];
                for (int i = 0; i < n; i++) {
                    if (claim[i] != r || !zon[i]) continue;
                    for (int e = 0; e < zd_pl_edges(nodes[i]); e++) {
                        int l, q2 = zd_pl_entry(nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                        if (q2 < 0 || known[q2] || !inp[q2]) continue;
                        int ub = (nodes[q2]->op == IR_UNMARK && nodes[q2]->n_operands > 0) ? nidx(nodes, n, nodes[q2]->operands[0]) : -1;
                        base[q2] = (ub >= 0 && zon[ub] && claim[ub] >= 0 && claim[ub] != q2 && known[claim[ub]] && !indef[claim[ub]]) ? base[claim[ub]] + zout[ub] - zout[q2]
                                 : base[r] + l - (zout[q2] - zd_k(nodes[q2]));
                        known[q2] = 1; indef[q2] = 0; wl[wn2++] = q2;
                    }
                }
            }
        }
        for (int r = 0; r < n; r++) if (zon[r] && claim[r] == r && !known[r] && nodes[r]->op == IR_MATCH_ABORT && claim[0] >= 0 && known[claim[0]]) { base[r] = base[claim[0]]; known[r] = 1; }
        if (zd_map_on()) for (int r = 0; r < n; r++) if (indef[r])
            fprintf(stderr, "[ZD-MAP] INDEF run=%d (%s) anchor=%d: entered after a call that may return with its frame live -- no base, its cells rsp-relative within the run only\n", r,
            bb_op_name(nodes[r]->op), anchor[r]);
        if (zd_map_on()) for (int c = 0; c < n; c++) if (zon[c] && claim[c] >= 0 && zd_pl_cut(nodes[c]))
            fprintf(stderr, "[ZD-MAP] CUT i=%d restores rsp to depth %d\n", c, zcut ? zcut[c] : known[claim[c]] ? base[claim[c]] + zout[c] - zd_k(nodes[c]) : anchor[claim[c]]);
        if (aof) for (int pass = 0, moved = 1; moved && pass < n + 2; pass++) {
            moved = 0;
            for (int i = 0; i < n; i++) {
                if (!zon[i] || claim[i] < 0 || aof[claim[i]] < 0) continue;
                for (int e = 0; e < zd_pl_edges(nodes[i]); e++) {
                    int l, q = zd_pl_entry(nodes, n, zon, zout, zgpop, zwpop, claim, i, e, &l, dead);
                    if (q >= 0 && claim[q] == q && aof[q] < 0) { aof[q] = aof[claim[i]]; moved = 1; }
                }
            }
        }
    }
}
int zst_s_of(const IR_t * nd) { if (!nd || g_emitter.zst_sm.empty()) return 0; auto it = g_emitter.zst_sm.find(nd); return it == g_emitter.zst_sm.end() ? 0 : it->second; }
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
    return k;
}
static int zst_isparam(const IR_graph_t * g, const char * vn) { for (int i = 0; g && g->pnames && i < g->nparams; i++) if (g->pnames[i] && !strcmp(g->pnames[i], vn)) return 1; return 0; }
static void zst_plan(IR_t ** nodes, int n) {
    g_emitter.zst_nodes = nodes; g_emitter.zst_n = n; g_emitter.g_zd_cur = -1; g_emitter.zst_s.assign((size_t)(n > 0 ? n : 0), 0); g_emitter.zst_root.assign((size_t)(n > 0 ? n : 0), -1);
    g_emitter.zst_name.clear(); g_emitter.zst_vroot.clear(); g_emitter.zst_c.clear(); g_emitter.zst_j.clear(); g_emitter.zst_L.clear(); g_emitter.zst_vuse.clear(); g_emitter.zst_vcov.clear();
        g_emitter.zst_sm.clear();
    const IR_graph_t * g = g_emit_cfg;
    if (!g || n <= 0 || !(emit_resumable_frame() || g->spine_cells || g->home_frame)) return;
    if (g->spine_cells && !g->home_frame) return;
    int nv = zls_g_vslot_count(g); if (nv <= 0) return;
    g_emitter.zst_vuse.assign((size_t)nv, 0); g_emitter.zst_vcov.assign((size_t)nv, 0);
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
        g_emitter.zst_root[(size_t)v] = nr == 1 ? r : -1; }
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
        for (int i = 0; i < n; i++) if (zd_edge_retains(nodes[i])) { int q = zst_edge(nodes, n, i, 0); if (q >= 0 && !indef0[(size_t)q]) { indef0[(size_t)q] = 1; wl.push_back(q); } }
        while (!wl.empty()) { int v = wl.back(); wl.pop_back();
            for (int k = scnt[(size_t)v]; k < scnt[(size_t)v + 1]; k++) { int w = sd[(size_t)k]; if (w < 0 || indef0[(size_t)w]) continue;
                int ub = (nodes[w]->op == IR_UNMARK && (IR_LIT(nodes[w]).ival & 6) && nodes[w]->n_operands > 0) ? nidx(nodes, n, nodes[w]->operands[0]) : -1;
                if (ub >= 0 && !indef0[(size_t)ub]) continue;
                indef0[(size_t)w] = 1; wl.push_back(w); } } }
    std::vector<int> bnds; std::vector<std::vector<unsigned char>> esc; std::vector<unsigned char> wland((size_t)n, 0);
    for (int i = 0; i < n; i++) { int q = zst_edge(nodes, n, i, 1); if (q >= 0) wland[(size_t)q] = 1; }
    for (int b = 0; b < n; b++) { if (nodes[b]->op != IR_BOUND) continue; std::vector<unsigned char> r((size_t)n, 0); std::vector<int> wl;
        for (int u = 0; u < n; u++) if (nodes[u]->op == IR_UNMARK && ((IR_LIT(nodes[u]).ival & 6) || wland[(size_t)u]) && nodes[u]->n_operands > 0 && nodes[u]->operands[0] == nodes[b]) {
            int q = zst_edge(nodes, n, u, 0);
            if (q >= 0 && !r[(size_t)q]) { r[(size_t)q] = 1; wl.push_back(q); } }
        while (!wl.empty()) { int v = wl.back(); wl.pop_back(); for (int k = scnt[(size_t)v]; k < scnt[(size_t)v + 1]; k++) { int w = sd[(size_t)k]; if (w >= 0 && !r[(size_t)w]) { r[(size_t)w] = 1;
            wl.push_back(w); } } }
        bnds.push_back(b); esc.push_back(r); }
    const char * nm[2];
    for (int k = 0; k < nv; k++) { int off = -1; const char * vn = zls_g_vslot_get(g, k, &off); if (!vn || vn[0] == '&' || zst_isparam(g, vn)) continue;
        std::vector<int> uses; for (int i = 0; i < n; i++) { int m = zst_names(nodes[i], nm); for (int q = 0; q < m; q++) if (nm[q] && !strcmp(nm[q], vn)) { uses.push_back(i); break; } }
        { int past = 0; for (size_t u = 0; (g->spine_cells || g->home_frame) && u < uses.size() && !past; u++) if (zls_home_local_why(nodes[uses[u]])) past = 1; if (past) continue; }
        if (zls_home_local_test(g, vn) == 9) continue;
        std::vector<int> roots; for (size_t u = 0; u < uses.size(); u++) { int r = g_emitter.zst_root[(size_t)uses[u]]; if (r < 0) continue; int seen = 0;
            for (size_t t = 0; t < roots.size(); t++) if (roots[t] == r) seen = 1; if (!seen) roots.push_back(r); }
        g_emitter.zst_vuse[(size_t)k] = (int)roots.size();
        for (size_t t = 0; t < roots.size(); t++) { int r = roots[t]; for (size_t w = 0; w < W; w++) tmp[w] = ~0ull;
            for (size_t u = 0; u < uses.size(); u++) if (g_emitter.zst_root[(size_t)uses[u]] == r) for (size_t w = 0; w < W; w++) tmp[w] &= dom[(size_t)uses[u] * W + w];
            int best = -1;
            for (int pass = 0; pass < (g->home_frame ? 2 : 1) && best < 0; pass++) for (int d = 0; d < n; d++) {
                if (!(tmp[(size_t)d / 64] >> (d % 64) & 1) || loop[(size_t)d] || indef0[(size_t)d] || nodes[d]->op == IR_DISJUNCTION || (zd_k_own(nodes[d]) <= 0 && !pass)) continue;
                int inside = 0;
                for (size_t bi = 0; bi < bnds.size() && !inside; bi++) {
                    int b = bnds[bi];
                    if (b == d || !(dom[(size_t)d * W + (size_t)b / 64] >> (b % 64) & 1) || esc[bi][(size_t)d]) continue;
                    for (size_t u = 0; u < uses.size() && !inside; u++) if (g_emitter.zst_root[(size_t)uses[u]] == r && esc[bi][(size_t)uses[u]]) inside = 1;
                }
                if (inside) continue;
                if (best < 0 || dsz[(size_t)d] > dsz[(size_t)best]) best = d;
            }
            if (best < 0) continue;
            g_emitter.zst_name.push_back(vn); g_emitter.zst_vroot.push_back(r); g_emitter.zst_c.push_back(best); g_emitter.zst_j.push_back(g_emitter.zst_s[(size_t)best] / 16);
                g_emitter.zst_L.push_back(0);
            g_emitter.zst_s[(size_t)best] += 16; g_emitter.zst_vcov[(size_t)k]++; } }
    for (int i = 0; i < n; i++) if (g_emitter.zst_s[(size_t)i] > 0) g_emitter.zst_sm[nodes[i]] = g_emitter.zst_s[(size_t)i];
}
void zst_finish(IR_t ** nodes, int n, const int * zout, const int * claim, const int * base, const unsigned char * known) {
    if (g_emitter.zst_nodes != nodes || g_emitter.zst_n != n) return;
    for (size_t t = 0; t < g_emitter.zst_c.size(); t++) { int c = g_emitter.zst_c[t]; int rk = claim && claim[c] >= 0 && known && known[claim[c]] && base;
        g_emitter.zst_L[t] = rk ? base[claim[c]] + zout[c] - zd_k(nodes[c]) + g_emitter.zst_s[(size_t)c] - 16 * g_emitter.zst_j[t] : INT_MIN; }
    if (zd_map_on()) for (size_t t = 0; t < g_emitter.zst_c.size(); t++) { int c = g_emitter.zst_c[t];
        fprintf(stderr, "[ZD-MAP] STICKY name=%s root=%d c=%d (%s zout=%d k=%d own=%d s=%d) j=%d L=%d\n", g_emitter.zst_name[t], g_emitter.zst_vroot[t], c, bb_op_name(nodes[c]->op), zout[c],
            zd_k(nodes[c]), zd_k_own(nodes[c]), g_emitter.zst_s[(size_t)c], g_emitter.zst_j[t], g_emitter.zst_L[t]); }
}
int zst_coord(const char * name) {
    if (!name || g_emitter.g_zd_cur < 0 || g_emitter.g_zd_cur >= g_emitter.zst_n || g_emitter.zst_nodes != g_emitter.codegen_flat_chain_body_nodes) return -1;
    int r = g_emitter.zst_root[(size_t)g_emitter.g_zd_cur]; if (r < 0) return -1;
    for (size_t t = 0; t < g_emitter.zst_name.size(); t++) if (g_emitter.zst_vroot[t] == r && g_emitter.zst_L[t] != INT_MIN && !strcmp(g_emitter.zst_name[t], name)) return ZST_BIAS -
        g_emitter.zst_L[t];
    return -1;
}
static void zd_dj_discards(IR_t ** nodes, int n, const unsigned char * zon, const int * zout, const int * zgpop, const int * zwpop, const int * zarm, const int * claim, const int * base,
    const unsigned char * known, int on) {
    g_emitter.g_zd_grec.assign((size_t)(n > 0 ? n : 0), -1);
    g_emitter.g_zd_wrec.assign((size_t)(n > 0 ? n : 0), -1);
    g_emitter.g_zd_grec_pop.assign((size_t)(n > 0 ? n : 0), 0);
    g_emitter.g_zd_wrec_pop.assign((size_t)(n > 0 ? n : 0), 0);
    if (!on || n <= 0 || !zarm) return;
    std::vector<int> recd((size_t)n * 2, INT_MAX);
    for (int d = 0; d < n; d++) {
        if (!zon[d] || !zls_dj_retains(nodes[d]) || zd_djx_of(nodes[d]) || !zd_dj_admitted(zarm, n, d) || claim[d] < 0 || !known[claim[d]]) continue;
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
        for (int a = 0; g_emit_cfg && g_emit_cfg->alt_entry && a < g_emit_cfg->n_alts; a++) push(r0, zd_chase(g_emit_cfg->alt_entry[a]));
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
                int land = v ? ai - zd_k(nodes[k]) - zwpop[k] : ai - zgpop[k];
                if (land >= zdab) {
                    if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-DJREC] i=%d edge %d leaves the continuation of dj %d at %d, not below its %d\n", k, v, d, land, zdab);
                    continue;
                }
                if (zdab >= recd[(size_t)(2 * k + v)]) continue;
                recd[(size_t)(2 * k + v)] = zdab;
                (v ? g_emitter.g_zd_wrec : g_emitter.g_zd_grec)[(size_t)k] = d;
                (v ? g_emitter.g_zd_wrec_pop : g_emitter.g_zd_grec_pop)[(size_t)k] = zdab - land;
                if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-DJREC] i=%d %s edge leaves the continuation of dj %d: rsp from its record, then pop %d\n", k, v ? "omega" : "gamma", d, zdab - land);
            }
        }
    }
}
void zd_plan(IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, int *zarm, const unsigned char *walls, int *zcps, int *zcpd, int *zchn, int *zclaim) {
    extern const char * bb_src_of(const IR_t *);
    if (g_emitter.zd_plan__dg < 0) { const char * d = getenv("SCRIP_ZD_DIAG"); g_emitter.zd_plan__dg = (d && *d == '1') ? 1 : 0; }
    g_emitter.g_zd_djx.assign((size_t)(n > 0 ? n : 0), 0);
    g_emitter.g_zd_upost.assign((size_t)(n > 0 ? n : 0), 0);
    g_emitter.g_zd_djx_nodes = nodes;
    zdo_build(nodes, n);
    zst_plan(nodes, n);
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
    unsigned char * djok = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
    for (int i = 0; i < n; i++) { zarmj[i] = -1; djn[i] = 0; djok[i] = 0; if (zcps) zcps[i] = -1; if (zcpd) zcpd[i] = -1; if (zchn) { zchn[2 * i] = -1; zchn[2 * i + 1] = -1; } }
    const int zdj_on = zarm && zcps && g_emit_cfg && g_emit_cfg->spine_cells;
    const int zdj_arms = zarm && zcps && g_emit_cfg;
    { if(g_emitter.zd_plan__lp<0){const char*e=getenv("SCRIP_ZDLOCAL");g_emitter.zd_plan__lp=(e&&*e=='1')?1:0;} if(g_emitter.zd_plan__lp){ extern int is_global(const char *);
        for (int i=0;i<n;i++){ int o=(int)nodes[i]->op;
        if(o==IR_VAR||o==IR_ASSIGN){ const char*vn=IR_LIT(nodes[i]).sval; int gl = (vn && is_global(vn) && !graph_has_local(g_emit_cfg, vn));
        if(!gl) fprintf(stderr,"[ZDLOCAL] %s name=%s fbdata=%d\n", bb_op_name(nodes[i]->op), vn?vn:"<null>", x86_fb_data()); } } } }
    {
      if (n <= 0) return; }
    int * claim = (int *)alloca(sizeof(int) * (size_t)n); int * run = (int *)alloca(sizeof(int) * (size_t)n); int * rpos = (int *)alloca(sizeof(int) * (size_t)n);
        unsigned char * aent = (unsigned char *)alloca((size_t)n);
    unsigned char * cm = (unsigned char *)alloca((size_t)n); int * wl = (int *)alloca(sizeof(int) * (size_t)n);
    for (int i = 0; i < n; i++) { claim[i] = -1; rpos[i] = -1; }
    { if (zd_map_on()) { extern const char * zls_graph_name_get(const IR_graph_t *);
        fprintf(stderr, "[ZD-MAP] GRAPH n=%d name=%s -- i= is this nodes[] array, NOT a --dump-ir slot; g/o/ops resolve into the SAME array\n", n, g_emit_cfg ? zls_graph_name_get(g_emit_cfg) : "?");
        for (int i = 0; i < n; i++) { IR_t * gt = zd_chase(nodes[i]->γ.node); IR_t * ot = zd_chase(nodes[i]->ω.node); int gi = -1, oi = -1;
            for (int k = 0; k < n; k++) { if (nodes[k] == gt) gi = k; if (nodes[k] == ot) oi = k; }
            fprintf(stderr, "[ZD-MAP] i=%-3d %-22s nops=%d ops=[", i, bb_op_name(nodes[i]->op), nodes[i]->n_operands);
            for (int a = 0; a < nodes[i]->n_operands; a++) { int pi = -1;
                { int k = nidx(nodes, n, nodes[i]->operands[a]); if (k >= 0) { pi = k; } }
                fprintf(stderr, "%s%d", a ? "," : "", pi); }
            fprintf(stderr, "] g=%d o=%d lit/nameptr=%lld src=%s\n", gi, oi, (long long)IR_LIT(nodes[i]).ival, bb_src_of(nodes[i]) ? bb_src_of(nodes[i]) : "-"); } } }
    unsigned char * rjoin = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
    for (int k = 0; k < n; k++) rjoin[k] = 0;
    for (int k = 0; k < n; k++) if (zd_edge_retains(nodes[k])) { int q = nidx(nodes, n, zd_chase(nodes[k]->γ.node)); if (q >= 0 && nodes[q]->op != IR_DISJUNCTION) rjoin[q] = 1; }
    for (int pass = 0; pass < 2; pass++) {
    for (int hi = 0; hi < n; hi++) {
        if (pass == 0) { }
        else { if (claim[hi] >= 0 || !zd_omega_head(nodes, n, nodes[hi])) continue; }
        if (claim[hi] >= 0) continue;
        int rl = 0; IR_t * cur = nodes[hi]; int guard = 0;
        while (cur && guard++ <= n) {
            int ci = -1; { int k = nidx(nodes, n, cur); if (k >= 0) { ci = k; } }
            if (ci < 0 || claim[ci] >= 0) break;
            if (rl > 0 && bb_src_of(nodes[ci]) && !zd_icn_suspend(nodes[ci])) break; if (rl > 0 && walls && walls[ci]) break;
            if (rl > 0 && rjoin[ci]) break;
            { int _ub = zd_pl_unmark_bound(nodes, n, ci); if (rl > 0 && _ub >= 0 && claim[_ub] != hi) break; }
            if (rl > 0 && emit_floater_kind(nodes[ci])) break;
            run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; rl++;
            if (zd_pl_cut(nodes[ci])) break;
            if (zd_edge_retains(nodes[ci])) break;
            cur = zd_icn_suspend(cur) ? zd_chase(cur->n_operands > 1 ? cur->operands[1] : (IR_t *)0) : zd_chase(cur->γ.node);
        }
        { for (int k = 0; k < n; k++) aent[k] = 0;
          if (zarm) {
            for (int r0 = 0; r0 < rl; r0++) { IR_t * env = nodes[run[r0]]; if ((int)env->op != IR_MATCH_ALTERNATE && (int)env->op != IR_MATCH_ARBNO && (int)env->op != IR_MATCH_FENCE1) continue;
                int ei = run[r0];
                for (int a = 0; a + 1 < env->n_operands; a += 2) { IR_t * c2 = env->operands[a]; int rl0 = rl; int bad = 0; int g2 = 0; int first = 1;
                    while (c2 && g2++ <= n) {
                        int ci = -1; { int k = nidx(nodes, n, c2); if (k >= 0) { ci = k; } }
                        if (ci < 0 || claim[ci] >= 0) break;
                        if (bb_src_of(nodes[ci]) || emit_floater_kind(nodes[ci])) { bad = 1; break; }
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
                        if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d is a bare fail: no box, its entry is the chain's omega\n", ei, a);
                        continue;
                    }
                    int first = 1, g2 = 0;
                    while (c2 && c2 != env && g2++ <= n) {
                        int ci = nidx(nodes, n, c2);
                        if (first && ci >= 0 && claim[ci] < 0 && zd_icn_stmt_head(nodes[ci])) {
                            if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d opens with a statement: no box claimed, its runs reach the box by sigma and phi\n", ei, a);
                            first = 0;
                            c2 = (IR_t *)0;
                            break;
                        }
                        if (!first && ci >= 0 && claim[ci] == hi && zarm[ci] == ei && zarmj[ci] == a) {
                            if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d loops back to ci=%d: the arm's own loop, it leaves by phi\n", ei, a, ci);
                            c2 = (IR_t *)0;
                            break;
                        }
                        if (ci < 0 && !first && (c2->op == IR_SUCCEED || c2->op == IR_FAIL)) { c2 = (IR_t *)0; break; }
                        if (zdj_on && ci >= 0 && !first && claim[ci] < 0 && (bb_src_of(nodes[ci]) || (zd_omega_head(nodes, n, nodes[ci]) && !(g_emit_cfg && g_emit_cfg->spine_cells)))) { c2 =
                            (IR_t *)0; break; }
                        if (zdj_on && !first && ci >= 0 && claim[ci] >= 0 && (claim[ci] != hi || (zarm[ci] != ei && nodes[ci] == zd_chase(env->γ.node)))) {
                            if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d leaves the box into ci=%d of run %d\n", ei, a, ci, claim[ci]);
                            c2 = (IR_t *)0;
                            break;
                        }
                        if (ci < 0 || claim[ci] >= 0 || bb_src_of(nodes[ci]) || emit_floater_kind(nodes[ci]) || (walls && walls[ci])) { bad = 1;
                            if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d stops at ci=%d (claim=%d src=%d floater=%d wall=%d)\n", ei, a, ci, ci >= 0 ? claim[ci] : -2,
                                ci >= 0 ? (bb_src_of(nodes[ci]) != 0) : -1, ci >= 0 ? emit_floater_kind(nodes[ci]) : -1, (ci >= 0 && walls) ? walls[ci] : -1); break; }
                        if (emit_resumable_frame() && zd_edge_retains(nodes[ci]) && zd_chase(c2->γ.node) != env) {
                            if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d holds a retaining ci=%d before its end: the disjunction is not admitted\n", ei, a, ci);
                            bad = 1;
                            break;
                        }
                        run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; zarm[ci] = ei; zarmj[ci] = a; aent[ci] = (unsigned char)first; first = 0; rl++;
                        if (g_emitter.zd_plan__dg)
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
                                if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d leaves the box at its GOTO ci=%d (next, break)\n", ei, a, ci);
                                c2 = (IR_t *)0;
                                break;
                            }
                        }
                        c2 = zd_chase(c2->γ.node);
                    }
                    if (zdj_on && !bad && !first && c2 == env && g_emit_cfg && g_emit_cfg->spine_cells && rl > rl0) {
                        IR_t * c3 = zd_chase(nodes[run[rl - 1]]->ω.node);
                        int rl1 = rl, g3 = 0;
                        while (c3 && c3 != env && g3++ <= n) {
                            int ci = nidx(nodes, n, c3);
                            if (ci < 0 || claim[ci] >= 0 || bb_src_of(nodes[ci]) || emit_floater_kind(nodes[ci]) || (walls && walls[ci])) break;
                            run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; zarm[ci] = ei; zarmj[ci] = a; aent[ci] = 0; rl++;
                            if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-DJ] i=%d arm %d claims ci=%d %s by omega\n", ei, a, ci, bb_op_name(nodes[ci]->op));
                            c3 = zd_chase(c3->γ.node);
                        }
                        if (!(c3 == env && rl > rl1)) { for (int u = rl1; u < rl; u++) { int ci = run[u]; claim[ci] = -1; rpos[ci] = -1; zarm[ci] = -1; zarmj[ci] = -1; aent[ci] = 0; } rl = rl1; }
                    }
                    if (first || (c2 && c2 != env)) bad = 1;
                }
                if (g_emitter.zd_plan__dg && bad) fprintf(stderr, "[ZD-DJ] i=%d not admitted (na=%d claimed=%d)\n", ei, na, rl - rl0);
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
                    if (zd_edge_retains(nodes[i]) || nodes[i]->op == IR_SUSPEND || (nodes[i]->op == IR_DISJUNCTION && zls_dj_retains(nodes[i]) && !zd_djx_of(nodes[i]))) cst = 0;
                    dep += zd_k(nodes[i]);
                    if (zd_chase(nodes[i]->γ.node) == nodes[d] && zd_sz_sigma(zd_edge_sz(nodes[i], 0)) && zls_dj_arm_resumes(nodes[d], zarmj[i]) && dep > x) x = dep;
                }
                if (cst && x > 0) g_emitter.g_zd_djx[(size_t)d] = x;
                if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-DJX] i=%d exit depth %d%s\n", d, x, cst ? "" : " (an arm's depth is not a constant: the record road)");
            }
        }
        for (int k = 0; k < n; k++) cm[k] = 0;
        g_emitter.g_zd_zarm_p = zarm; g_emitter.g_zd_zarm_nodes = nodes; g_emitter.g_zd_zarm_n = n;
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
                int K = zd_k(nodes[i]);
                int UREL = 0;
                { int _ub = zd_pl_unmark_bound(nodes, n, i); if (_ub >= 0 && claim[_ub] == hi && rpos[_ub] >= 0 && rpos[_ub] < r) UREL = (int)zd + K - zout[_ub]; }
                if (zd_pl_cut(nodes[i])) UREL = (int)zd + K;
                zurel[i] = UREL;
                { if (nodes[i]->op == IR_MATCH_BEGIN && zdh_match < 0) zdh_match = (int)zd + K; } { if (nodes[i]->op == IR_STATEMENT_END) zdh_stmt = (int)zd + K; }
                { if (nodes[i]->op == IR_STATEMENT_BEGIN && r > 0 && zd > 0 && !(zarm && zarm[i] >= 0) ) { int rel =
                    zd_exit_pop_s(IR_STATEMENT_END, zdh_match, (int)zd, zdh_stmt, zdh_mafter); if (rel > 0) zgpop[run[r - 1]] += rel; zd = 0; } }
                { if (nodes[i]->op == IR_STATEMENT_BEGIN && r > 0) { zdh_match = -1; zdh_stmt = -1; zdh_mafter = -1; zwt = 0; } }
                { if (nodes[i]->op == IR_MATCH_BEGIN && zwt < n) zwms[zwt++] = (int)zd + K; }
                { if (nodes[i]->op == IR_MATCH_REPLACE || nodes[i]->op == IR_MATCH_END) zdh_mafter = (int)zd + K; }
                if (zarm && zarm[i] >= 0) { if (aent[i]) arm_zd = zout[zarm[i]] - (nodes[zarm[i]]->op == IR_DISJUNCTION ? zd_djx_of(nodes[zarm[i]]) : zd_k(nodes[zarm[i]])); zon[i] = 1;
                    if (nodes[i]->op == IR_UNMARK && (IR_LIT(nodes[i]).ival & 16) && nodes[i]->n_operands > 0 && nodes[i]->operands[0] && nodes[i]->operands[0]->op == IR_BOUND) {
                        int _bk = nidx(nodes, n, nodes[i]->operands[0]);
                        if (_bk >= 0 && claim[_bk] == hi && zarm[_bk] == zarm[i] && rpos[_bk] >= 0 && rpos[_bk] < r) arm_zd = zout[_bk] - zd_k(nodes[_bk]);
                    }
                    zout[i] = arm_zd + K;
                    arm_zd = arm_zd + K; }
                else { zon[i] = 1; zout[i] = zd + K - REL; zd = zd + K - REL - UREL; }
                { if (zwt == 0 && nodes[i]->op == IR_MATCH_REPLACE) { int _zu = 0, _me = 0; for (int _rr = r - 1; _rr >= 0; _rr--) { int _j = run[_rr];
                    if (nodes[_j]->op == IR_MATCH_END) { _me = 1; break; } _zu += zd_k(nodes[_j]); } if (_me) { zd -= _zu; zdh_mafter = (int)zd; } } }
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
                { if (!gin && gt && gt->op == IR_SUCCEED && g_emit_cfg && g_emit_cfg->spine_cells && port_sz_beta(nodes[i]->ω.sz)) gin = 1; }
                { if (!gin && zd_icn_suspend(nodes[i])) gin = 1; }
                { (void)0; }
                { int oib = port_sz_beta(nodes[i]->ω.sz); if (!oin && oib && K == 0 && !beta_is_stmt_land(ot)) oin = 1; }
                { if (!oin && K == 0) { int _io = !ot;
                  if (!_io && !(port_sz_beta(nodes[i]->ω.sz) && beta_is_stmt_land(ot))) { for (int _ik = 0; _ik <= r; _ik++) { if (nodes[run[_ik]] == ot) { _io = 1; break;
                      } } } if (_io) oin = 1; } }
                { zgt[i] = gt; zot[i] = ot; zgin[i] = (unsigned char)gin; zoin[i] = (unsigned char)oin; zmatch[i] = zdh_match; zstmt[i] = zdh_stmt; zmafter[i] = zdh_mafter;
                    }
                                { long kc = 0;
                int _wzdepth = (zarm && zarm[i] >= 0) ? zout[i] : (int)zd;
                int _gbpre = (gback >= 0) ? (zout[gback] - zd_k(nodes[gback])) : 0;
                int _obpre = (oback >= 0) ? (zout[oback] - zd_k(nodes[oback])) : 0;
                {
                if (!gin) zgpop[i] = (emit_resumable_frame() && !(gback >= 0 && claim[gback] == claim[i])) ? 0 : (gback >= 0) ? (_wzdepth - _gbpre) :
                    (zd_exit_pop_s(nodes[i]->op, zdh_match, _wzdepth, zdh_stmt, zdh_mafter) + (int)kc);
                if (nodes[i]->op == IR_BOUND && emit_resumable_frame()) zwpop[i] = -K;
                else if (!oin) zwpop[i] = (oback >= 0) ? ((_wzdepth - K) - _obpre) :
                    (((nodes[i]->op == IR_GOTO_DEFERRED) ? zd_exit_pop_s(nodes[i]->op, zdh_match, _wzdepth, zdh_stmt, zdh_mafter) : (_wzdepth - K)) + (int)kc);
                else if (oback >= 0 && claim[oback] == claim[i] && rpos[oback] < rpos[i] && port_sz_beta(nodes[i]->ω.sz)) zwpop[i] = (_wzdepth - K) - zout[oback]; } }
                if (g_emitter.zd_plan__dg)
                    fprintf(stderr, "[ZD] h=%d r=%d i=%d %s K=%d zout=%d gpop=%d wpop=%d gin=%d oin=%d gback=%d oback=%d\n", hi, r, i, bb_op_name(nodes[i]->op), K, zout[i], zgpop[i], zwpop[i], gin,
                    oin, gback, oback);
            }
        }
        else {
            for (int r = 0; r < rl; r++) rpos[run[r]] = -1;
               if (g_emitter.zd_plan__dg && rl > 0) { extern const char * zls_graph_name_get(const IR_graph_t *);
                   fprintf(stderr, "[ZD] run h=%d len=%d REFUSED at i=%d (%s op=%d) graph=%s\n", hi, rl, badi, why, badi >= 0 ? (int)nodes[badi]->op : -1,
                   g_emit_cfg ? zls_graph_name_get(g_emit_cfg) : "?"); }
               { if (g_emitter.zd_plan__gp < 0) { const char * _e = getenv("SCRIP_ZD_GAP"); g_emitter.zd_plan__gp = (_e && _e[0] == '1') ? 1 : 0;
                   } if (g_emitter.zd_plan__gp && rl > 0) { for (int _r = 0; _r < rl; _r++) { IR_t * _n = nodes[run[_r]]; const char * _nm = bb_op_name(_n->op); { int _o = (int)_n->op;
                   int _ic = (_o == (int)IR_CALL || (_o != (int)IR_CALL_VALUE && ir_is_call_kind((IR_e)_o))); const char * _cs = (_ic && IR_LIT(_n).sval) ? IR_LIT(_n).sval : "-";
                   fprintf(stderr, "[ZD-GAP] r=%d op=%s(%d) nops=%d callee=%s\n", _r, _nm ? _nm : "<unnamed>", _o, (int)_n->n_operands, _cs); } } } }
               { if (g_emitter.zd_plan__cd < 0) { const char *_e = getenv("SCRIP_CALL_DIAG"); g_emitter.zd_plan__cd = (_e && _e[0] == '1') ? 1 : 0; }
                 if (g_emitter.zd_plan__cd && rl > 0 && badi >= 0) { IR_t * _bn = nodes[badi]; int _bop = (int)_bn->op;
                     int _is_call = (_bop == (int)IR_CALL || (_bop != (int)IR_CALL_VALUE && ir_is_call_kind((IR_e)_bop)));
                     const char * _callee = (_is_call && IR_LIT(_bn).sval) ? IR_LIT(_bn).sval : (const char *)0;
                     fprintf(stderr, "[CALL-DIAG] REFUSE blocker op=%s(%d) callee=%s rl=%d badi=%d narg=%d\n", bb_op_name(_bn->op), _bop, _callee ? _callee : "-", rl, badi,
                     _is_call ? (int)_bn->n_operands : -1); } } }
    }
    }
    { for (int i = 0; i < n; i++) { if (!zon[i]) continue; int K = zd_k(nodes[i]); int gback = -1, oback = -1;
        if (zgt[i]) { { int tk = nidx(nodes, n, zgt[i]); if (tk >= 0 && (zon[tk])) { gback = tk; } } }
        if (zot[i]) { { int tk = nidx(nodes, n, zot[i]); if (tk >= 0 && (zon[tk])) { oback = tk; } } }
        int _gbpre = (gback >= 0) ? (zout[gback] - zd_k(nodes[gback])) : 0;
        int _obpre = (oback >= 0) ? (zout[oback] - zd_k(nodes[oback])) : 0;
        if (!zgin[i]) zgpop[i] = (emit_resumable_frame() && !(gback >= 0 && claim[gback] == claim[i])) ? 0 : (gback >= 0 && _gbpre > 0) ? (zout[i] - zurel[i] - _gbpre) :
            zd_exit_pop_s(nodes[i]->op, zmatch[i], zout[i] - zurel[i], zstmt[i], zmafter[i]);
        int _obeta = oback >= 0 && port_sz_beta(zd_edge_sz(nodes[i], 1)) && !beta_is_stmt_land(zot[i]);
        if (nodes[i]->op == IR_BOUND && emit_resumable_frame()) zwpop[i] = -K;
        else if (!zoin[i]) zwpop[i] = (oback >= 0) ? ((zout[i] - K) - (_obeta ? zout[oback] : _obpre)) :
            ((nodes[i]->op == IR_GOTO_DEFERRED) ? zd_exit_pop_s(nodes[i]->op, zmatch[i], zout[i], zstmt[i], zmafter[i]) : (zout[i] - K));
        else if (oback >= 0 && claim[oback] == claim[i] && rpos[oback] > rpos[i] && !port_sz_beta(nodes[i]->ω.sz)) zwpop[i] = (zout[i] - K) - _obpre;
        else if (oback >= 0 && claim[oback] == claim[i] && rpos[oback] < rpos[i] && port_sz_beta(nodes[i]->ω.sz)) zwpop[i] = (zout[i] - K) - zout[oback];
        else if (oback >= 0 && claim[oback] == claim[i] && rpos[oback] < rpos[i] && zd_sz_alpha(zd_edge_sz(nodes[i], 1))) zwpop[i] = (zout[i] - K) - _obpre;
        if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-FINAL] i=%d %s K=%d zout=%d gpop=%d wpop=%d gback=%d oback=%d\n", i, bb_op_name(nodes[i]->op), K, zout[i], zgpop[i], zwpop[i], gback, oback);
            } }
    for (int i = 0; i < n; i++) {
        if (!zon[i]) continue;
        IR_t * t = zd_chase(nodes[i]->γ.node);
        int q = t ? nidx(nodes, n, t) : -1;
        if (q >= 0 && zon[q] && claim[q] == claim[i] && q != i && port_sz_beta(zd_edge_sz(nodes[i], 0))) zgpop[i] = zout[i] - zout[q];
        if (!emit_resumable_frame()) continue;
        IR_t * r = (nodes[i]->op == IR_GATE_ARM && nodes[i]->n_operands > 0 && (int) nodes[i]->seal) ? nodes[i]->operands[0] : (IR_t *)0;
        int rq = r ? nidx(nodes, n, r) : -1;
        if (rq >= 0 && zon[rq] && claim[rq] == claim[i] && rpos[rq] < rpos[i]) zgpop[i] = zout[i] - zout[rq];
        IR_t * o = zd_chase(nodes[i]->ω.node);
        int qo = o ? nidx(nodes, n, o) : -1;
        if (qo >= 0 && zon[qo] && claim[qo] != claim[i]) zwpop[i] = zout[i] - zd_k(nodes[i]);
        if (qo >= 0 && zon[qo] && claim[qo] == claim[i] && qo != i && port_sz_beta(zd_edge_sz(nodes[i], 1))) zwpop[i] = zout[i] - zd_k(nodes[i]) - zout[qo];
        if (q >= 0 && zon[q] && claim[q] != claim[i] && nodes[q]->op == IR_DISJUNCTION && !zd_edge_retains(nodes[i])) zgpop[i] = zout[i];
    }
    int * base = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    unsigned char * known = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
    {
        int * aof = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
        int * fam = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
        int * fbase = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
        zd_pl_bases(nodes, n, zon, zout, zgpop, zwpop, claim, base, known, aof, (int *)0, fam, fbase);
        for (int i = 0; i < n; i++) {
            if (!zon[i] || claim[i] < 0) continue;
            int ci = claim[i], ki = known[ci] ? 1 : (fam[ci] >= 0 ? 2 : 0);
            if (!ki) continue;
            int ai = (ki == 1 ? base[ci] : fbase[ci]) + zout[i];
            for (int v = 0; v < 2; v++) {
                int q, b;
                (void) zd_pl_land(nodes, n, zon, zout, zgpop, zwpop, i, v, &q, &b);
                if (g_emitter.zd_plan__dg)
                    fprintf(stderr, "[ZD-XC] i=%d v=%d q=%d b=%d cq=%d known=%d ai=%d\n", i, v, q, b, q >= 0 ? claim[q] : -2, (q >= 0 && claim[q] >= 0) ? known[claim[q]] : -1, ai);
                if (q < 0 || claim[q] < 0 || claim[q] == claim[i]) continue;
                int cq = claim[q], kq = known[cq] ? 1 : (fam[cq] >= 0 ? 2 : 0);
                if (ki == 2 && b && zd_beta_via_act(nodes[q])) {
                    IR_t * lt = zd_chase(nodes[q]->γ.node);
                    int lq = lt ? nidx(nodes, n, lt) : -1;
                    int ub = (nodes[ci]->op == IR_UNMARK && (IR_LIT(nodes[ci]).ival & 6) && nodes[ci]->n_operands > 0) ? nidx(nodes, n, nodes[ci]->operands[0]) : -1;
                    int fl = (lq >= 0 && zon[lq] && claim[lq] >= 0) ? fam[claim[lq]] : -1, cb = (ub >= 0 && zon[ub] && claim[ub] >= 0) ? claim[ub] : -1;
                    int af = fl < 0 ? INT_MIN : fam[ci] == fl ? ai : (cb >= 0 && !known[cb] && fam[cb] == fl) ? fbase[cb] + zout[ub] - zout[ci] + zout[i] : INT_MIN;
                    int ax = (lq >= 0 && claim[lq] >= 0) ? fbase[claim[lq]] + zout[lq] - zd_k(nodes[lq]) : 0;
                    if (af != INT_MIN) { if (v) zwpop[i] = af - zd_k(nodes[i]) - ax; else zgpop[i] = af - ax; continue; }
                }
                if (ki == 2 && v && fam[ci] != ci && zd_edge_retains(nodes[q])) {
                    int ql, qb;
                    (void) zd_pl_land(nodes, n, zon, zout, zgpop, zwpop, q, 0, &ql, &qb);
                    if (ql >= 0 && zon[ql] && claim[ql] >= 0 && claim[ql] == fam[ci]) { zwpop[i] = ai - zd_k(nodes[i]) - (fbase[fam[ci]] + zout[ql] - (qb ? 0 : zd_k(nodes[ql]))); continue; }
                }
                if (kq != ki || (ki == 2 && fam[ci] != fam[cq])) continue;
                int aq = (kq == 1 ? base[cq] : fbase[cq]) + zout[q] - (b ? 0 : zd_k(nodes[q]));
                if (!b && zarm && nodes[q]->op == IR_DISJUNCTION && zd_dj_admitted(zarm, n, q)) {
                    const char * sz = zd_edge_sz(nodes[i], v);
                    if (zd_sz_sigma(sz)) aq = (kq == 1 ? base[cq] : fbase[cq]) + zout[q];
                    else if (zd_sz_phi(sz)) aq = (kq == 1 ? base[cq] : fbase[cq]) + zout[q] - zd_djx_of(nodes[q]);
                    if (zd_sz_sigma(sz) && !v && zcps && zcpd && zarm[i] < 0 && zcpd[i] == -1)
                        for (int a = 0; a < (int)IR_LIT(nodes[q]).ival; a++) {
                            IR_t * rj = zd_dj_res(nodes[q], a);
                            int kr = rj ? nidx(nodes, n, rj) : -1, hit = 0;
                            if (kr < 0 || !zon[kr] || claim[kr] != claim[i] || zd_icn_local_var(nodes[kr]) || zd_dj_cellless(nodes, n, zarm, kr)) continue;
                            for (IR_t * t = nodes[kr]; t && hit < n; t = zd_chase(t->γ.node)) { if (t == nodes[i]) { hit = -1; break; } hit++; }
                            if (hit != -1) continue;
                            if (zls_dj_retains(nodes[q])) { zcps[i] = -(kr + 2); zcpd[i] = -(q + 2); }
                            else { zcps[i] = -(kr + 2); zcpd[i] = ai - aq; }
                            break;
                        }
                }
                if (v) zwpop[i] = ai - zd_k(nodes[i]) - aq; else zgpop[i] = ai - aq;
            }
            if (ki != 1) continue;
            IR_t * r = (nodes[i]->op == IR_GATE_ARM && nodes[i]->n_operands > 0 && (int) nodes[i]->seal) ? nodes[i]->operands[0] : (IR_t *)0;
            int rq = r ? nidx(nodes, n, r) : -1;
            if (rq >= 0 && zon[rq] && claim[rq] >= 0 && claim[rq] != claim[i] && known[claim[rq]]) zgpop[i] = ai - (base[claim[rq]] + zout[rq]);
            IR_t * ig = (nodes[i]->op == IR_GATE_ARM && !nodes[i]->γ.node && nodes[i]->n_operands > 1) ? nodes[i]->operands[1] : (IR_t *)0;
            int gq = ig ? nidx(nodes, n, ig) : -1, cq = -1, cb = 0;
            (void) zd_pl_land(nodes, n, zon, zout, zgpop, zwpop, i, 0, &cq, &cb);
            if (gq >= 0 && zon[gq] && claim[gq] >= 0 && known[claim[gq]] && cq < 0) zgpop[i] = ai - (base[claim[gq]] + zout[gq]);
        }
        for (int i = 0; i < n; i++) {
            if (!zon[i] || claim[i] < 0 || nodes[i]->op != IR_GATE || known[claim[i]] || fam[claim[i]] < 0) continue;
            int p = -1, ub = -1;
            for (int j = 0; j < n && p < 0; j++) if (zon[j] && claim[j] >= 0 && nodes[j]->op == IR_UNMARK && nodes[j]->n_operands > 0 && zd_chase(nodes[j]->γ.node) == nodes[i]) p = j;
            if (p >= 0) ub = nidx(nodes, n, nodes[p]->operands[0]);
            if (ub < 0 || !zon[ub] || claim[ub] < 0) continue;
            int cu = claim[ub], ku = known[cu] ? 1 : (fam[cu] >= 0 ? 2 : 0);
            if (!ku) continue;
            int q, b;
            (void) zd_pl_land(nodes, n, zon, zout, zgpop, zwpop, i, 1, &q, &b);
            if (q < 0 || claim[q] < 0 || claim[q] == claim[i]) continue;
            int cq = claim[q], kq = known[cq] ? 1 : (fam[cq] >= 0 ? 2 : 0);
            if (kq != ku || (ku == 2 && fam[cu] != fam[cq])) continue;
            zwpop[i] = ((ku == 1 ? base[cu] : fbase[cu]) + zout[ub]) - ((kq == 1 ? base[cq] : fbase[cq]) + zout[q] - (b ? 0 : zd_k(nodes[q])));
        }
        int nalt = g_emit_cfg ? g_emit_cfg->n_alts : 0, gx = -1, ngx = 0;
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
                for (int a2 = 0; a2 < nalt && a < 0 && g_emit_cfg->alt_entry; a2++) {
                    int q2 = g_emit_cfg->alt_entry[a2] ? nidx(nodes, n, zd_chase(g_emit_cfg->alt_entry[a2])) : -1;
                    if (q2 >= 0 && zon[q2] && claim[q2] >= 0 && fam[claim[q2]] == f) a = a2;
                }
                for (int e = 0; e < n && a < 0 && nf < 0; e++) {
                    IR_t * t = (zon[e] && claim[e] >= 0 && zd_edge_retains(nodes[e])) ? zd_chase(nodes[e]->γ.node) : (IR_t *)0;
                    int q = t ? nidx(nodes, n, t) : -1;
                    if (q < 0 || !zon[q] || claim[q] < 0 || fam[claim[q]] != f || claim[e] == claim[q]) continue;
                    if (known[claim[e]]) a = aof[claim[e]];
                    else nf = fam[claim[e]];
                }
                f = nf;
            }
            IR_t * ad = (nalt > 0 && a >= 0 && a < nalt && g_emit_cfg->alt_redo) ? g_emit_cfg->alt_redo[a] : (IR_t *)0;
            int dq = ad ? nidx(nodes, n, zd_chase(ad)) : (nalt == 0 && ngx == 1) ? gx : -1;
            if (dq < 0 || !zon[dq] || claim[dq] < 0) continue;
            int cd = claim[dq];
            IR_t * t = zd_edge_retains(nodes[dq]) ? zd_chase(nodes[dq]->γ.node) : (IR_t *)0;
            int q = t ? nidx(nodes, n, t) : -1;
            if (known[ci] && known[cd]) zgpop[i] = base[ci] + zout[i] - (base[cd] + zout[dq]);
            else if (!known[ci] && q >= 0 && zon[q] && claim[q] >= 0 && !known[claim[q]] && fam[claim[q]] == fam[ci])
                zgpop[i] = fbase[ci] + zout[i] - (fbase[claim[q]] + zout[q] - zd_k(nodes[q]));
            else if (!known[ci] && !known[cd] && fam[cd] == fam[ci]) zgpop[i] = fbase[ci] + zout[i] - (fbase[cd] + zout[dq]);
            else continue;
            redo_priced[i] = 1;
        }
        if (emit_resumable_frame()) for (int i = 0; i < n; i++) {
            if (!zon[i] || claim[i] < 0 || !known[claim[i]]) continue;
            IR_t * ig = (nodes[i]->op == IR_GATE_ARM && !nodes[i]->γ.node && nodes[i]->n_operands > 1) ? nodes[i]->operands[1] : (IR_t *)0;
            IR_t * c = zd_chase(ig ? ig->γ.node : nodes[i]->γ.node);
            int cq = c ? nidx(nodes, n, c) : -1;
            if (c && (c->op == IR_SUCCEED || cq < 0 || !zon[cq]) && !redo_priced[i]) zgpop[i] = zd_edge_retains(nodes[i]) ? 0 : base[claim[i]] + zout[i];
            IR_t * o = zd_chase(nodes[i]->ω.node);
            int oq = o ? nidx(nodes, n, o) : -1;
            if (!o || o->op == IR_FAIL || oq < 0 || !zon[oq]) zwpop[i] = base[claim[i]] + zout[i] - zd_k(nodes[i]);
        }
        else if (!(g_emit_cfg && g_emit_cfg->spine_cells)) for (int i = 0; i < n; i++) {
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
                if (zd_icn_stmt_head(nodes[q])) continue;
                if (!v && zd_sz_phi(sz) && nodes[q]->op != IR_DISJUNCTION && zd_chase(nodes[i]->ω.node) == t && !port_sz_beta(zd_edge_sz(nodes[i], 1))) continue;
                int p = (v == 0 && nodes[i]->op == IR_UNMARK) ? -1 : zd_pred_bt(nodes, n, claim, claim[i], i);
                if (p >= 0 && p != q && !zd_icn_chain_step_ok(nodes, n, p, q)) p = -1;
                int to = q;
                if (p >= 0 && p != q) { to = p; if (zchn) zchn[2 * i + v] = p; }
                int xb = (claim[i] != claim[to] && claim[i] >= 0 && claim[to] >= 0 && known[claim[i]] && known[claim[to]]) ? base[claim[i]] - base[claim[to]] : 0;
                int xq = (claim[i] != claim[q] && claim[i] >= 0 && claim[q] >= 0 && known[claim[i]] && known[claim[q]]) ? base[claim[i]] - base[claim[q]] : 0;
                int zq = zout[q] - (zd_sz_phi(sz) ? zd_djx_of(nodes[q]) : 0);
                if (v) zwpop[i] = (zout[i] - zd_k(nodes[i])) - (to == q ? zq : zout[to]) + xb;
                else zgpop[i] = zout[i] - (to == q ? zq : zout[to]) + xb;
                for (int c = p, g3 = 0; c >= 0 && c != q && g3++ <= n; c = zd_pred_bt(nodes, n, claim, claim[i], c)) {
                    int pc = zd_pred_bt(nodes, n, claim, claim[i], c);
                    int stop = (pc >= 0 && pc != q && !zd_icn_chain_step_ok(nodes, n, pc, q));
                    if (stop) pc = -1;
                    int tc = (pc >= 0) ? pc : q;
                    if (zchn && pc >= 0 && pc != q) zchn[2 * c + 1] = pc;
                    zwpop[c] = (zout[c] - zd_k(nodes[c])) - (tc == q ? zq - xq : zout[tc]);
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
            if (xc && !(zd_unmark_acts(nodes[i]) > 0 && claim[b] >= 0 && claim[i] >= 0 && known[claim[b]] && known[claim[i]])) continue;
            IR_t * _gs = nodes[i];
            IR_t * _gt = zd_lm_chase(nodes[i], n, &_gs);
            int _gq = _gt ? nidx(nodes, n, _gt) : -1;
            int _rb = _gq >= 0 && zon[_gq] && port_sz_beta(zd_edge_sz(_gs, 0));
            if (xc && _rb && (claim[_gq] < 0 || !known[claim[_gq]])) continue;
            if (zd_unmark_acts(nodes[i]) > 0) {
                int ab = base[claim[b]] + zout[b];
                if (_gt == nodes[b] || _rb) for (IR_t * u = zd_chase(nodes[i]->γ.node); u && u != _gt; u = zd_chase(u->γ.node)) { int k = nidx(nodes, n, u);
                    if (k < 0 || !zon[k] || (xc && (claim[k] < 0 || !known[claim[k]]))) continue;
                    zout[k] = xc ? ab - base[claim[k]] : zout[b];
                    zgpop[k] = (u == _gs && _rb) ? (xc ? ab - (base[claim[_gq]] + zout[_gq]) : zout[b] - zout[_gq]) : 0;
                    zwpop[k] = 0; }
                if (_rb && _gs == nodes[i]) g_emitter.g_zd_upost[(size_t)i] = xc ? ab - (base[claim[_gq]] + zout[_gq]) : zout[b] - zout[_gq];
                continue;
            }
            if (!xc && !_rb && _gt && _gt->op == IR_DISJUNCTION && zd_sz_phi(zd_edge_sz(_gs, 0))) continue;
            if (zchn[2 * i] < 0) {
                zgpop[i] = zout[i] - (_rb ? zout[_gq] : zout[b]);
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
                    int ent = port_sz_beta(zd_edge_sz(nodes[i], v)) ? zout[q] : zout[q] - zd_k(nodes[q]);
                    int xb = (claim[i] != claim[q] && claim[i] >= 0 && claim[q] >= 0 && known[claim[i]] && known[claim[q]]) ? base[claim[i]] - base[claim[q]] : 0;
                    if (v) zwpop[i] = (zout[i] - zd_k(nodes[i])) - ent + xb;
                    else zgpop[i] = zout[i] - ent + xb;
                    continue;
                }
                if (v) { if (!zdj_on) zwpop[i] = (zout[i] - zd_k(nodes[i])) - zout[q]; continue; }
                if (zdj_on && (port_sz_beta(zd_edge_sz(nodes[i], 0)) || zd_sz_phi(zd_edge_sz(nodes[i], 0)))) continue;
                zgpop[i] = zout[i] - zout[q];
                if (zls_dj_retains(nodes[q]) && !zd_djx_of(nodes[q])) { int p0 = i; while (zarm[p0] != q) p0 = zarm[p0]; if (zls_dj_arm_resumes(nodes[q], zarmj[p0])) zgpop[i] = 0; }
                if (!zd_sz_sigma(zd_edge_sz(nodes[i], 0))) continue;
                int p = i;
                while (zarm[p] != q) p = zarm[p];
                IR_t * rj = zd_dj_res(nodes[q], zarmj[p]);
                int kr = rj ? nidx(nodes, n, rj) : -1;
                if (kr >= 0 && zon[kr] && !zd_icn_local_var(nodes[kr]) && !zd_dj_cellless(nodes, n, zarm, kr) && zls_dj_retains(nodes[q])) { zcps[i] = -(kr + 2); zcpd[i] = -(q + 2); }
                else if (kr >= 0 && zon[kr] && !zd_icn_local_var(nodes[kr]) && !zd_dj_cellless(nodes, n, zarm, kr)) { zcps[i] = -(kr + 2); zcpd[i] = zout[i] - zout[q]; }
            }
        }
    }
    if (zarm)
        for (int i = 0; i < n; i++) {
            if (!zon[i] || zarm[i] < 0) continue;
            IR_t * t = zd_chase(nodes[i]->γ.node);
            int q = t ? nidx(nodes, n, t) : -1;
            if (q >= 0 && nodes[q]->op == IR_MATCH_ARBNO && zd_dj_anc(zarm, i, q)) zgpop[i] = 0;
        }
    for (int sp = 0; sp < n; sp++) {
        if (!zon[sp] || nodes[sp]->op != IR_SUSPEND || claim[sp] < 0) continue;
        IR_t * t = zd_chase(nodes[sp]->γ.node);
        for (int g = 0; t && g <= n; g++) {
            int k = nidx(nodes, n, t);
            if (k < 0 || !zon[k]) break;
            IR_t * nx = zd_chase(t->γ.node);
            int nk = nx ? nidx(nodes, n, nx) : -1;
            if (!nx || nx->op == IR_SUCCEED || nk < 0 || !zon[nk]) {
                if (claim[k] != claim[sp]) break;
                zgpop[k] = zout[k] - zout[sp];
                if (g_emitter.zd_plan__dg) fprintf(stderr, "[ZD-YIELD] i=%d ends the yield of suspend %d: it pops to the suspend's depth (%d)\n", k, sp, zgpop[k]);
                break;
            }
            t = nx;
        }
    }
    for (int i = 0; i < n; i++) if (zon[i] && nodes[i]->op == IR_GATE_ARM && nodes[i]->n_operands > 1 && zd_edge_retains(nodes[i]->operands[1]) &&
        zd_arm_resumes(nodes[i], nodes[i]->operands[1])) zgpop[i] = 0;
    for (int dj = 0; dj < n; dj++) if (zon[dj] && nodes[dj]->op == IR_DISJUNCTION && zd_edge_retains(nodes[dj]))
        for (int a = 0; a < (int)IR_LIT(nodes[dj]).ival; a++) { int e = zd_pl_dj_arm_holds(nodes[dj], a) ? zd_dj_arm_end(nodes, n, dj, a) : -1; if (e >= 0 && zon[e]) zgpop[e] = 0; }
    zd_dj_discards(nodes, n, zon, zout, zgpop, zwpop, zarm, claim, base, known, zdj_on);
    { if (zd_map_on()) { fprintf(stderr, "[ZD-MAP] PLAN -- same i= indices as GRAPH above\n");
        for (int i = 0; i < n; i++) fprintf(stderr, "[ZD-MAP] i=%-3d %-22s claim=%-3d rpos=%-3d zon=%d zout=%-4d gpop=%-5d wpop=%-5d arm=%d slot=%d\n",
            i, bb_op_name(nodes[i]->op), claim[i], rpos[i], (int)zon[i], zout[i], zgpop[i], zwpop[i], zarm ? zarm[i] : -1, zls_off(nodes[i])); } }
    if (zclaim) for (int i = 0; i < n; i++) zclaim[i] = zon[i] ? claim[i] : -1;
}
void zd_depth_census(IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, const char *prefix) {
     if (g_emitter.zd_depth_census__on < 0) { const char * e = getenv("SCRIP_ZD_DEPTH"); g_emitter.zd_depth_census__on = (e && *e == '1') ? 1 : 0; }
    if (!g_emitter.zd_depth_census__on || n <= 0) return;
    zdw_ent_t * w = (zdw_ent_t *)ct_zalloc((size_t)(2 * n + 1), sizeof(zdw_ent_t)); if (!w) return;
    int wn = 0, armed = 0, edges = 0, walls = 0;
    for (int i = 0; i < n; i++) { if (!zon[i]) continue; armed++;
        int K = zd_k(nodes[i]);
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
        for (int i = 0; i < n; i++) { if (!zon[i]) continue; int Kp = zd_k(nodes[i]);
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
int zd_run_alpha_dead(IR_t ** nodes, int n, const int * claim, int h) {
    if (h <= 0 || h >= n || claim[h] != h || (g_emit_cfg && g_emit_cfg->n_alts > 0)) return 0;
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
void zd_check(IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, const int *xb, const int *zchn, const int *zcut, const char *prefix)
    {
    if (g_emitter.zd_check__on < 0) { const char * e = getenv("SCRIP_ZD_CHECK"); g_emitter.zd_check__on = (e && *e == '1') ? 1 : 0; }
    if (!g_emitter.zd_check__on || n <= 0) return;
    int armed = 0, def = 0, edges = 0, viol = 0;
    for (int i = 0; i < n; i++) { if (!zon[i]) continue; armed++; if (xb && xb[i] != INT_MIN) def++; }
    for (int i = 0; i < n; i++) {
        if (!zon[i] || (xb && xb[i] == INT_MIN)) continue;
        int o = (int)nodes[i]->op;
        if (zd_check_arm_env(o) || o == IR_UNMARK || o == IR_CUT || o == IR_MATCH_END || o == IR_MATCH_REPLACE) continue;
        int bi = xb ? xb[i] : 0, Xi = bi + zout[i], Ki = zd_k(nodes[i]);
        for (int v = 0; v < 2; v++) {
            if (v && o == IR_LINE_MARK) continue;
            IR_t * t = zd_chase(v ? nodes[i]->ω.node : nodes[i]->γ.node);
            if (!t || zd_check_discard(t) || zd_check_arm_env((int)t->op) || (v && t->op == IR_MATCH_BEGIN && zd_sz_phi(zd_edge_sz(nodes[i], 1)))) continue;
            int j = nidx(nodes, n, t), via = (zchn && zchn[2 * i + v] >= 0) ? zchn[2 * i + v] : -1;
            if (via >= 0) j = via;
            if (j < 0 || j == i || !zon[j]) continue;
            if (xb) { if (xb[j] == INT_MIN) continue; } else if (!claim || claim[j] != claim[i]) continue;
            const char * sz = zd_edge_sz(nodes[i], v);
            int bj = xb ? xb[j] : 0, beta = via >= 0 || port_sz_beta(sz) || (zd_sz_phi(sz) && emit_glue_box(nodes[j], 1)) || (!v && zd_sz_sigma(sz) && emit_glue_box(nodes[j], 0));
            int arrive = v ? (Xi - Ki - zwpop[i]) : (Xi - zgpop[i]);
            int Xj = bj + zout[j], expect = beta ? Xj : Xj - zd_k(nodes[j]);
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
        int join = zd_pl_dj_join(nodes, n, i, j), arrive = zcut[i] - zgpop[i], expect = xb[j] + zout[j] - (join ? 0 : zd_k(nodes[j]));
        edges++;
        if (arrive == expect) continue;
        viol++;
        fprintf(stderr, "[ZD-CHECK]   DISCARD     i=%d %s -> j=%d %s restores=%d expects=%d\n", i, bb_op_name(nodes[i]->op), j, bb_op_name(nodes[j]->op), arrive, expect);
    }
    for (int i = 0; zcut && xb && i < n; i++) {
        if (!zon[i] || nodes[i]->op != IR_UNMARK || zcut[i] < 0 || nodes[i]->n_operands < 1) continue;
        int b = nidx(nodes, n, nodes[i]->operands[0]), up = zd_upost_of(nodes[i]);
        int chn = (xb[i] != INT_MIN) ? xb[i] + zout[i] - zgpop[i] : INT_MIN;
        int rec = (b >= 0 && zon[b] && xb[b] != INT_MIN) ? xb[b] + zout[b] - zd_k(nodes[b]) - up : INT_MIN;
        if (chn != zcut[i] && rec != zcut[i])
            fprintf(stderr, "[ZD-CHECK]   UNMARK      i=%d -> E_cont=%d chain_arrives=%d record_arrives=%d (reported, not counted)\n", i, zcut[i], chn, rec);
    }
    fprintf(stderr, "[ZD-CHECK] graph=%s boxes=%d armed=%d definite=%d coordinate=%s edges=%d violations=%d\n", prefix ? prefix : "?", n, armed, def, xb ? "absolute" : "relative", edges, viol);
}
zeta_plan_t * planner_chain_plan(IR_t * entry, const char * prefix) {
    int CH_MAX = 65536; if (g_emit_cfg && g_emit_cfg->n + 1024 > CH_MAX) CH_MAX = g_emit_cfg->n + 1024; int Q_MAX = CH_MAX * 16;
      if (g_emitter.codegen_flat_chain_body_nodes_cap < CH_MAX) { g_emitter.codegen_flat_chain_body_nodes = (IR_t **)ct_grow(g_emitter.codegen_flat_chain_body_nodes, (size_t)CH_MAX * sizeof(IR_t *));
          g_emitter.codegen_flat_chain_body_nodes_cap = CH_MAX; } int n = 0;
      if (g_emitter.codegen_flat_chain_body_queue_cap < Q_MAX) { g_emitter.codegen_flat_chain_body_queue = (IR_t **)ct_grow(g_emitter.codegen_flat_chain_body_queue, (size_t)Q_MAX * sizeof(IR_t *));
          g_emitter.codegen_flat_chain_body_queue_cap = Q_MAX; } int qh = 0, qt = 0;
    int entry_is_own_graph_root = (g_emit_cfg && (entry == g_emit_cfg->entry)) ? 1 : 0;
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL || entry->op == IR_GOTO) && entry->γ.node && guard++ < CH_MAX) entry = entry->γ.node; }
    if (g_emit_cfg && entry == g_emit_cfg->entry) entry_is_own_graph_root = 1;
    int flat_empty_body_fail = (entry && entry->op == IR_FAIL) ? 1 : 0;
    int flat_empty_body_succ = (entry && entry->op == IR_SUCCEED) ? 1 : 0;
    entry = entry;
      if (g_emitter.codegen_flat_chain_body_postv_cap < CH_MAX) { g_emitter.codegen_flat_chain_body_postv = (IR_t **)ct_grow(g_emitter.codegen_flat_chain_body_postv, (size_t)CH_MAX * sizeof(IR_t *));
          g_emitter.codegen_flat_chain_body_postv_cap = CH_MAX; } int pn = 0;
    ir_pset_t seenset; ir_pset_init(&seenset);
#define RPO_VISITED(p) (ir_pset_has(&seenset, (const IR_t *)(p)))
#define RPO_MARK(p) ir_pset_add(&seenset, (const IR_t *)(p))
#define RPO_TAG_EMIT ((IR_t *)1)
#define RPO_PUSH(p) do { IR_t *_rpp = (IR_t *)(p); int _rppg = 0; while (_rpp && _rpp->op == IR_SUCCEED && _rpp->γ.node && _rppg++ < 4096) _rpp = _rpp->γ.node; if (_rpp && _rpp->op != IR_SUCCEED \
&& _rpp->op != IR_FAIL && !emit_floater_member(_rpp) && !(_stmt_seed && _rpp->op == IR_STATEMENT_BEGIN) && !RPO_VISITED(_rpp) && qt < Q_MAX) g_emitter.codegen_flat_chain_body_queue[qt++] = _rpp; } \
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
        IR_t *c = g_emitter.codegen_flat_chain_body_queue[--qt]; \
        if (c == RPO_TAG_EMIT) { IR_t *e = g_emitter.codegen_flat_chain_body_queue[--qt]; if (pn < CH_MAX) g_emitter.codegen_flat_chain_body_postv[pn++] = e; continue; } \
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue; \
        if (RPO_VISITED(c)) continue; \
        RPO_MARK(c); \
        if (pn >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); } \
        if (qt + 2 > Q_MAX) { fprintf(stderr, "[GZ-7] FATAL: chain traversal queue saturated at EMIT tag\n"); abort(); } \
        g_emitter.codegen_flat_chain_body_queue[qt++] = c; g_emitter.codegen_flat_chain_body_queue[qt++] = RPO_TAG_EMIT; \
        RPO_PUSH_SUCCS(c); \
    }
#define RPO_FLUSH() do { for (int _i = pn - 1; _i >= 0; _i--) { if (n >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); } g_emitter.codegen_flat_chain_body_nodes[n++] = \
    g_emitter.codegen_flat_chain_body_postv[_i]; } pn = 0; } while (0)
    int _stmt_seed = 0; if (g_emit_cfg && entry_is_own_graph_root) for (int _ss = 0; _ss < g_emit_cfg->n; _ss++) { IR_t * _sn = g_emit_cfg->all[_ss];
        if (_sn && _sn->op == IR_STATEMENT_BEGIN) { _stmt_seed = 1; break; } }
    if (_stmt_seed) { if (qt < Q_MAX) g_emitter.codegen_flat_chain_body_queue[qt++] = entry; RPO_DRAIN(); RPO_FLUSH();
        for (int _ss = 0; _ss < g_emit_cfg->n; _ss++) { IR_t * _sb = g_emit_cfg->all[_ss];
        if (!_sb || _sb->op != IR_STATEMENT_BEGIN || RPO_VISITED(_sb)) continue; if (qt < Q_MAX) { g_emitter.codegen_flat_chain_body_queue[qt++] = _sb; RPO_DRAIN(); RPO_FLUSH(); } } }
    else { RPO_PUSH(entry); RPO_DRAIN(); RPO_FLUSH(); }
    if (g_emit_cfg && g_emit_cfg->n_alts > 1 && g_emit_cfg->alt_entry)
        for (int _ak = 1; _ak < g_emit_cfg->n_alts; _ak++) { IR_t * _ae = g_emit_cfg->alt_entry[_ak]; if (_ae && !RPO_VISITED(_ae)) { RPO_PUSH(_ae); RPO_DRAIN(); RPO_FLUSH(); } }
    { extern int zls_g_group_count(const IR_graph_t *); extern const IR_t * zls_g_group_anchor(const IR_graph_t *, int);
      if (g_emit_cfg && entry_is_own_graph_root) { int _gc = zls_g_group_count(g_emit_cfg); for (int _k = 0; _k < _gc; _k++) { const IR_t * _a = zls_g_group_anchor(g_emit_cfg, _k);
          if (_a && !planner_entry_planned(_a)) { RPO_PUSH(_a); RPO_DRAIN(); RPO_FLUSH(); } } } }
    { if (g_emit_cfg && entry_is_own_graph_root) { int _refd[4] = {0, 0, 0, 0};
          for (int _fi = 0; _fi < g_emit_cfg->n; _fi++) { IR_t * _fn = g_emit_cfg->all[_fi]; int _fk = emit_floater_kind(_fn); if (!_fk || _refd[_fk]) continue;
              for (int _rj = 0; _rj < g_emit_cfg->n && !_refd[_fk]; _rj++) { IR_t * _r = g_emit_cfg->all[_rj];
                  if (_r && _r != _fn && !emit_floater_member(_r) && (_r->γ.node == _fn || _r->ω.node == _fn)) _refd[_fk] = 1; } }
          if (_refd[3]) _refd[1] = _refd[2] = 1;
          for (int _fk = 1; _fk <= 3; _fk++) { if (!_refd[_fk]) continue; for (int _fi = 0; _fi < g_emit_cfg->n; _fi++) { IR_t * _fn = g_emit_cfg->all[_fi];
              if (!_fn || emit_floater_kind(_fn) != _fk || RPO_VISITED(_fn)) continue;
              if (qt < Q_MAX) { g_emitter.codegen_flat_chain_body_queue[qt++] = _fn; RPO_DRAIN(); RPO_FLUSH(); } } } } }
    { int _p2_base = 0; do { int _p2_added = 0;
        for (int i = _p2_base; i < n; i++)
            if (ir_is_generator_kind(g_emitter.codegen_flat_chain_body_nodes[i]->op) && g_emitter.codegen_flat_chain_body_nodes[i]->ω.node &&
            !RPO_VISITED(g_emitter.codegen_flat_chain_body_nodes[i]->ω.node)) { RPO_PUSH(g_emitter.codegen_flat_chain_body_nodes[i]->ω.node); _p2_added = 1; } _p2_base = n;
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
    if (!g_emit.flat_jmp_entry) g_emit.flat_pat = 0;
    nidx_build(g_emitter.codegen_flat_chain_body_nodes, n);
    int *zd_claim = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    zd_plan(g_emitter.codegen_flat_chain_body_nodes, n, zd_on, zd_out, zd_gp, zd_wp, zd_arm, (const unsigned char *)0, zd_cps, zd_cpd, zd_chn, zd_claim);
    int * zd_vl = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    zd_vdj_live(g_emitter.codegen_flat_chain_body_nodes, n, zd_vl);
    for (int i = 0; i < n; i++) for (int p = zd_on[i] ? zd_arm[i] : -1, g = 0; p >= 0 && g++ < n; p = zd_arm[p]) if (g_emitter.codegen_flat_chain_body_nodes[p]->op == IR_DISJUNCTION) { zd_vl[i] = 0;
        break; }
    int * zd_xb = (int *)0;
    int * zd_cut = (int *)0;
        { unsigned char * _zw = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
            unsigned char * _zarm = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
        int * _zst = (int *)alloca(sizeof(int) * (size_t)(3 * n + 3)); for (int _q = 0; _q < n; _q++) _zw[_q] = 0; auto _in_arms = [&](int t, int k) -> int { int sp = 0;
        for (int q = 0; q < n; q++) _zarm[q] = 0;
            for (int a = 0; a < g_emitter.codegen_flat_chain_body_nodes[t]->n_operands; a++) { int q =
            nidx(g_emitter.codegen_flat_chain_body_nodes, n, g_emitter.codegen_flat_chain_body_nodes[t]->operands[a]); if (q >= 0 && !_zarm[q]) { _zarm[q] = 1;
        _zst[sp++] = q; } } while (sp > 0) { IR_t * x = g_emitter.codegen_flat_chain_body_nodes[_zst[--sp]];
        for (int e = 0; e < 2 + x->n_operands; e++) { IR_t * y = e == 0 ? zd_chase(x->γ.node) : e == 1 ? zd_chase(x->ω.node) : x->operands[e - 2];
        int q = (y && y != g_emitter.codegen_flat_chain_body_nodes[t]) ? nidx(g_emitter.codegen_flat_chain_body_nodes, n, y) : -1; if (q >= 0 && !_zarm[q] && sp < 3 * n + 3) { _zarm[q] = 1;
            _zst[sp++] = q; } } } return _zarm[k]; };
        for (int _it = 0; _it < n; _it++) { int _add = 0;
            for (int k = 0; k < n; k++) { if (g_emitter.codegen_flat_chain_body_nodes[k]->op == IR_GOTO) continue;
        for (int v = 0; v < 2; v++) { IR_t * tt = zd_chase(v == 0 ? g_emitter.codegen_flat_chain_body_nodes[k]->γ.node : g_emitter.codegen_flat_chain_body_nodes[k]->ω.node);
            if (!tt || (v == 1 && port_sz_beta(g_emitter.codegen_flat_chain_body_nodes[k]->ω.sz))) continue; int t = nidx(g_emitter.codegen_flat_chain_body_nodes, n, tt);
        if (t < 0 || t == k || _zw[t] || !zd_on[t] || zd_arm[t] >= 0) continue;
            int ad = zd_out[t] - zd_k(g_emitter.codegen_flat_chain_body_nodes[t]) + fence0_release_bytes(g_emitter.codegen_flat_chain_body_nodes[t]); int d = 0;
        if (zd_on[k] || ad <= 0 || !strncmp(bb_op_name(g_emitter.codegen_flat_chain_body_nodes[k]->op), "IR_MATCH_", 9) ||
            !strncmp(bb_op_name(g_emitter.codegen_flat_chain_body_nodes[t]->op), "IR_MATCH_", 9) || _in_arms(t, k)) continue; _zw[t] = 1; _add++;
        if (getenv("SCRIP_ZD_DIAG"))
            fprintf(stderr, "[ZD] JOIN-WALL i=%d (%s) alpha depth %d entered at depth %d by i=%d (%s)\n", t, bb_op_name(g_emitter.codegen_flat_chain_body_nodes[t]->op), ad, d, k,
            bb_op_name(g_emitter.codegen_flat_chain_body_nodes[k]->op));
        } } if (!_add) break; zd_plan(g_emitter.codegen_flat_chain_body_nodes, n, zd_on, zd_out, zd_gp, zd_wp, zd_arm, _zw, zd_cps, zd_cpd, zd_chn, zd_claim); }
        }
    zeta_plan_t * rec = new zeta_plan_t();
    rec->entry = entry; rec->n = n; rec->own_root = entry_is_own_graph_root; rec->empty_fail = flat_empty_body_fail; rec->empty_succ = flat_empty_body_succ;
    rec->nodes = g_emitter.codegen_flat_chain_body_nodes;
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
    if (g_emitter.g_zd_zarm_p == zd_arm) g_emitter.g_zd_zarm_p = rec->arm;
    if (n > 0) {
        int * _b = (int *)alloca(sizeof(int) * (size_t)n);
        unsigned char * _kn = (unsigned char *)alloca((size_t)n);
        zd_xb = (int *)alloca(sizeof(int) * (size_t)n);
        zd_cut = (int *)alloca(sizeof(int) * (size_t)n);
        int * zd_fam = (int *)alloca(sizeof(int) * (size_t)n);
        int * zd_fbase = (int *)alloca(sizeof(int) * (size_t)n);
        zd_pl_bases(g_emitter.codegen_flat_chain_body_nodes, n, zd_on, zd_out, zd_gp, zd_wp, zd_claim, _b, _kn, (int *)0, zd_cut, zd_fam, zd_fbase);
        for (int _i = 0; _i < n; _i++) zd_xb[_i] = (zd_on[_i] && zd_claim[_i] >= 0 && _kn[zd_claim[_i]]) ? _b[zd_claim[_i]] : INT_MIN;
        for (int _i = 0; _i < n; _i++) {
            IR_t * _u = g_emitter.codegen_flat_chain_body_nodes[_i], * _gs = _u;
            if (!zd_on[_i] || _u->op != IR_UNMARK || _u->n_operands < 1 || zd_cut[_i] >= 0) continue;
            IR_t * _gt = zd_lm_chase(_u, n, &_gs);
            int _q = _gt ? nidx(g_emitter.codegen_flat_chain_body_nodes, n, _gt) : -1;
            if (_q < 0 || !zd_on[_q] || zd_xb[_q] == INT_MIN) continue;
            zd_cut[_i] = zd_xb[_q] + zd_out[_q] - (port_sz_beta(zd_edge_sz(_gs, 0)) ? 0 : zd_k(g_emitter.codegen_flat_chain_body_nodes[_q]));
        }
        zst_finish(g_emitter.codegen_flat_chain_body_nodes, n, zd_out, zd_claim, _b, _kn);
        rec->xb = (int *)ct_zalloc((size_t)n, sizeof(int)); memcpy(rec->xb, zd_xb, sizeof(int) * (size_t)n);
        rec->cut = (int *)ct_zalloc((size_t)n, sizeof(int)); memcpy(rec->cut, zd_cut, sizeof(int) * (size_t)n);
        rec->fam = (int *)ct_zalloc((size_t)n, sizeof(int)); memcpy(rec->fam, zd_fam, sizeof(int) * (size_t)n);
        rec->fbase = (int *)ct_zalloc((size_t)n, sizeof(int)); memcpy(rec->fbase, zd_fbase, sizeof(int) * (size_t)n);
        rec->b = (int *)ct_zalloc((size_t)n, sizeof(int)); memcpy(rec->b, _b, sizeof(int) * (size_t)n);
        rec->kn = (unsigned char *)ct_zalloc((size_t)n, 1); memcpy(rec->kn, _kn, (size_t)n);
        g_emitter.g_zd_claim_p = rec->claim; g_emitter.g_zd_xb_p = rec->xb; g_emitter.g_zd_nodes_p = g_emitter.codegen_flat_chain_body_nodes; g_emitter.g_zd_n = n; g_emitter.g_zd_cut_p = rec->cut;
        g_emitter.g_zd_fam_p = rec->fam; g_emitter.g_zd_fbase_p = rec->fbase; g_emitter.g_zd_gp_p = rec->gp; g_emitter.g_zd_out_p = rec->out;
        g_emitter.g_zd_wp_p = rec->wp;
    }
    g_emitter.g_zd_plan_on = rec->on; g_emitter.g_zd_plan_nodes = g_emitter.codegen_flat_chain_body_nodes; g_emitter.g_zd_plan_n = n;
    planner_capture(rec);
    return rec;
}
void planner_capture(zeta_plan_t * rec) {
    int n = rec->n;
    rec->nodes_own = (IR_t **)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(IR_t *)); memcpy(rec->nodes_own, g_emitter.codegen_flat_chain_body_nodes, sizeof(IR_t *) * (size_t)n);
    rec->v_zst_s = g_emitter.zst_s; rec->v_zst_root = g_emitter.zst_root; rec->v_zst_vroot = g_emitter.zst_vroot; rec->v_zst_c = g_emitter.zst_c; rec->v_zst_j = g_emitter.zst_j;
        rec->v_zst_L = g_emitter.zst_L;
    rec->v_zst_vuse = g_emitter.zst_vuse; rec->v_zst_vcov = g_emitter.zst_vcov; rec->v_zst_name = g_emitter.zst_name; rec->v_zst_sm = g_emitter.zst_sm; rec->zst_n_saved = g_emitter.zst_n;
    rec->v_djx = g_emitter.g_zd_djx; rec->v_upost = g_emitter.g_zd_upost; rec->v_grec = g_emitter.g_zd_grec; rec->v_wrec = g_emitter.g_zd_wrec; rec->v_grec_pop = g_emitter.g_zd_grec_pop;
    rec->v_wrec_pop = g_emitter.g_zd_wrec_pop;
    rec->zarm_published = (g_emitter.g_zd_zarm_p == rec->arm) ? 1 : 0; rec->djx_nodes_published = (g_emitter.g_zd_djx_nodes == g_emitter.codegen_flat_chain_body_nodes) ? 1 : 0;
    rec->zrm_len = g_emitter.g_zrm_map.len; rec->zrm_cap = g_emitter.g_zrm_map.cap; rec->zrm_esz = g_emitter.g_zrm_map.esz; rec->zrm_gen = g_emitter.g_zrm_gen;
    rec->zrm_bytes.assign((const unsigned char *)g_emitter.g_zrm_map.p, (const unsigned char *)g_emitter.g_zrm_map.p + (size_t)g_emitter.g_zrm_map.len * (size_t)g_emitter.g_zrm_map.esz);
}
void planner_restore(zeta_plan_t * rec) {
    int n = rec->n; int CH_MAX = 65536; if (g_emit_cfg && g_emit_cfg->n + 1024 > CH_MAX) CH_MAX = g_emit_cfg->n + 1024;
    if (g_emitter.codegen_flat_chain_body_nodes_cap < CH_MAX) { g_emitter.codegen_flat_chain_body_nodes = (IR_t **)ct_grow(g_emitter.codegen_flat_chain_body_nodes, (size_t)CH_MAX * sizeof(IR_t *));
        g_emitter.codegen_flat_chain_body_nodes_cap = CH_MAX; }
    IR_t ** nodes = g_emitter.codegen_flat_chain_body_nodes; memcpy(nodes, rec->nodes_own, sizeof(IR_t *) * (size_t)n);
    nidx_build(nodes, n);
    g_emitter.zst_s = rec->v_zst_s; g_emitter.zst_root = rec->v_zst_root; g_emitter.zst_vroot = rec->v_zst_vroot; g_emitter.zst_c = rec->v_zst_c; g_emitter.zst_j = rec->v_zst_j;
        g_emitter.zst_L = rec->v_zst_L;
    g_emitter.zst_vuse = rec->v_zst_vuse; g_emitter.zst_vcov = rec->v_zst_vcov; g_emitter.zst_name = rec->v_zst_name; g_emitter.zst_sm = rec->v_zst_sm; g_emitter.zst_nodes = nodes;
        g_emitter.zst_n = rec->zst_n_saved;
    g_emitter.g_zd_djx = rec->v_djx; g_emitter.g_zd_upost = rec->v_upost; g_emitter.g_zd_grec = rec->v_grec; g_emitter.g_zd_wrec = rec->v_wrec; g_emitter.g_zd_grec_pop = rec->v_grec_pop;
    g_emitter.g_zd_wrec_pop = rec->v_wrec_pop;
    if (rec->djx_nodes_published) g_emitter.g_zd_djx_nodes = nodes;
    zdo_build(nodes, n);
    if (g_emitter.g_zrm_map.cap < rec->zrm_cap || g_emitter.g_zrm_map.esz != rec->zrm_esz) { g_emitter.g_zrm_map.p = ct_zalloc(rec->zrm_cap, rec->zrm_esz); g_emitter.g_zrm_map.cap = rec->zrm_cap;
        g_emitter.g_zrm_map.esz = rec->zrm_esz; }
    g_emitter.g_zrm_map.len = rec->zrm_len; if (rec->zrm_len) memcpy(g_emitter.g_zrm_map.p, rec->zrm_bytes.data(), (size_t)rec->zrm_len * (size_t)rec->zrm_esz); g_emitter.g_zrm_gen = rec->zrm_gen;
    if (n > 0) { g_emitter.g_zd_claim_p = rec->claim; g_emitter.g_zd_xb_p = rec->xb; g_emitter.g_zd_nodes_p = nodes; g_emitter.g_zd_n = n; g_emitter.g_zd_cut_p = rec->cut;
        g_emitter.g_zd_fam_p = rec->fam;
        g_emitter.g_zd_fbase_p = rec->fbase; g_emitter.g_zd_gp_p = rec->gp; g_emitter.g_zd_out_p = rec->out; g_emitter.g_zd_wp_p = rec->wp; }
    g_emitter.g_zd_plan_on = rec->on; g_emitter.g_zd_plan_nodes = nodes; g_emitter.g_zd_plan_n = n;
    if (rec->zarm_published) { g_emitter.g_zd_zarm_p = rec->arm; g_emitter.g_zd_zarm_nodes = nodes; g_emitter.g_zd_zarm_n = n; }
}
extern "C" long zsm_declared_owner(void);
extern "C" int zls_retains_why(const IR_graph_t * g, const IR_t * nd);
extern "C" long zsm_declared_word(void) {
    const IR_t * nd = g_emit.node;
    int n = g_emitter.g_zd_plan_n, i = (nd && g_emitter.g_zd_plan_nodes && g_emitter.g_zd_plan_on) ? nidx(g_emitter.g_zd_plan_nodes, n, nd) : -1;
    if (i < 0 || !g_emitter.g_zd_plan_on[i]) return 0L;
    int rel = g_emitter.g_zd_nodes_p == g_emitter.g_zd_plan_nodes && g_emitter.g_zd_n == n && g_emitter.g_zd_gp_p && g_emitter.g_zd_wp_p;
    long k = zd_k((IR_t *)nd), gp = rel ? g_emitter.g_zd_gp_p[i] : 0, wp = rel ? g_emitter.g_zd_wp_p[i] : 0;
    int o = (int)nd->op, ex = zd_check_arm_env(o) || o == IR_UNMARK || o == IR_CUT || o == IR_MATCH_END || o == IR_MATCH_REPLACE;
    IR_t * tg = zd_chase(nd->γ.node), * tw = zd_chase(nd->ω.node);
    int lg = !ex && tg && !zd_check_discard(tg) && !zd_check_arm_env((int)tg->op);
    int lw = !ex && tw && !zd_check_discard(tw) && !zd_check_arm_env((int)tw->op) && !(tw->op == IR_MATCH_BEGIN && zd_sz_phi(zd_edge_sz((IR_t *)nd, 1)));
    int lb = !zd_check_discard(nd) && !zd_check_arm_env(o);
    long own = o == IR_MATCH_BEGIN ? 64L + emit_match_begin_frame_extra(nd) - ((!g_emit.op_zres && g_emit.op_subj_cell) ? 16L : 0L) : 0L;
    int end = (o == IR_MATCH_END || o == IR_MATCH_REPLACE) && zsm_declared_owner() > 0L;
    int ret = zls_retains_why(g_emit_cfg, nd) != 0;
    return ((k + own) & 0xFFFFL) | ((gp & 0xFFFFL) << 16) | ((wp & 0xFFFFL) << 32) | (1L << 48) | ((long)rel << 49) | ((long)lg << 50) | ((long)lw << 51) | ((long)lb << 54) |
        ((long)(own != 0L) << 55) | ((long)end << 56) | ((long)ret << 57);
}
extern "C" long zsm_declared_owner(void) {
    const IR_t * nd = g_emit.node;
    if (!nd || (nd->op != IR_MATCH_END && nd->op != IR_MATCH_REPLACE) || nd->n_operands < 1 || !nd->operands[0] || nd->operands[0]->op != IR_MATCH_BEGIN) return 0L;
    return (long)bb_node_id(nd->operands[0]);
}
extern "C" const char * zls_graph_name_get(const IR_graph_t * g);
extern "C" int zls_g_frame_region(const IR_graph_t * g);
extern "C" int zls_g_resident_bytes(const IR_graph_t * g);
extern "C" int zls_g_key_class(const IR_graph_t * g, int off);
static int zsm_frame_on(void) {
    if (g_emitter.zsm_frame_on < 0) { const char * e = getenv("SCRIP_ZSM"); g_emitter.zsm_frame_on = (e && *e == '1') ? 1 : 0; }
    return g_emitter.zsm_frame_on;
}
static void zsm_frame_report(void) {
    for (const auto & r : g_emitter.zsm_frames)
        fprintf(stderr, "[ZSM-FRAME] graph=%s total=%d region=%d resident=%d frames=%d addrs=%d word=%d resident_addrs=%d outside=%d unbacked=%d rsp_form=%d\n", r.name.c_str(), r.total,
            r.region, r.resident, r.frames, r.addrs, r.cls[1], r.cls[2], r.cls[3], r.cls[0], r.rsp_form);
}
static g_emitter_t::zsm_frame_rec_t * zsm_frame_rec(void) {
    const IR_graph_t * g = g_emit_cfg;
    if (!g) return (g_emitter_t::zsm_frame_rec_t *)0;
    int L = g_emitter.zsm_frames_last;
    if (L >= 0 && L < (int)g_emitter.zsm_frames.size() && g_emitter.zsm_frames[(size_t)L].g == g) return &g_emitter.zsm_frames[(size_t)L];
    for (size_t k = 0; k < g_emitter.zsm_frames.size(); k++) if (g_emitter.zsm_frames[k].g == g) { g_emitter.zsm_frames_last = (int)k; return &g_emitter.zsm_frames[k]; }
    if (g_emitter.zsm_frames.empty()) atexit(zsm_frame_report);
    const char * nm = zls_graph_name_get(g);
    g_emitter_t::zsm_frame_rec_t r = { g, std::string(nm ? nm : "?"), zls_g_frame_region(g), zls_g_resident_bytes(g), 0, 0, { 0, 0, 0, 0 }, 0, 0 };
    g_emitter.zsm_frames.push_back(r);
    g_emitter.zsm_frames_last = (int)g_emitter.zsm_frames.size() - 1;
    return &g_emitter.zsm_frames.back();
}
extern "C" void zsm_frame_addr(int off, int cls, int rsp_form) {
    if (!zsm_frame_on()) return;
    g_emitter_t::zsm_frame_rec_t * r = zsm_frame_rec();
    if (!r) return;
    int c = cls >= 0 ? cls : zls_g_key_class(g_emit_cfg, off);
    if (c == 0 && off >= r->region + 16 && off < r->total) c = 1;
    if (g_emitter.zsm_frame_diag < 0) { const char * e = getenv("SCRIP_ZSM_FRAME_DIAG"); g_emitter.zsm_frame_diag = (e && *e == '1') ? 1 : 0; }
    if (g_emitter.zsm_frame_diag && (c == 0 || c == 3))
        fprintf(stderr, "[ZSM-FRAME-ADDR] graph=%s class=%s op=%s off=%d region=%d form=%s\n", r->name.c_str(), c ? "outside" : "unbacked", g_emit.node ? bb_op_name(g_emit.node->op) : "-", off,
            r->region, rsp_form ? "rsp" : "frame");
    r->addrs++;
    r->cls[c & 3]++;
    r->rsp_form += rsp_form ? 1 : 0;
}
extern "C" void zsm_frame_carve(int frame_bytes) {
    if (!zsm_frame_on()) return;
    g_emitter_t::zsm_frame_rec_t * r = zsm_frame_rec();
    if (!r) return;
    r->frames++;
    if (frame_bytes > r->total) r->total = frame_bytes;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void planner_report_at_exit(void) {
    fprintf(stderr, "[PLANNER-PASS] planned=%d planned_rt=%d hits=%d fallbacks=%d\n", g_emitter.planner_planned, g_emitter.planner_rt, g_emitter.planner_hits, g_emitter.planner_fallbacks);
}
static void planner_report_arm(void) { if (!g_emitter.planner_report_armed) { g_emitter.planner_report_armed = 1; if (getenv("SCRIP_PLANNER_REPORT")) atexit(planner_report_at_exit); } }
void planner_pass_plan(IR_t * entry, const char * prefix) {
    planner_report_arm();
    zeta_plan_t * rec = planner_chain_plan(entry, prefix); rec->cfg = g_emit_cfg; rec->key = entry; g_emitter.planner_table[entry] = rec; g_emitter.planner_planned++;
}
int planner_entry_planned(const IR_t * entry) { return (entry && g_emitter.planner_table.find(entry) != g_emitter.planner_table.end()) ? 1 : 0; }
int planner_rt_plan(IR_t * entry, const char * prefix) { if (planner_entry_planned(entry)) return 0; planner_pass_plan(entry, prefix); g_emitter.planner_rt++; return 1; }
void planner_table_forget(const IR_t * entry) { if (entry) g_emitter.planner_table.erase(entry); }
void planner_table_clear(void) { g_emitter.planner_table.clear(); }
zeta_plan_t * planner_lookup_or_plan(IR_t * entry, const char * prefix) {
    planner_report_arm();
    std::unordered_map<const IR_t *, zeta_plan_t *>::iterator it = g_emitter.planner_table.find(entry);
    if (it != g_emitter.planner_table.end() && it->second->cfg == g_emit_cfg) { g_emitter.planner_hits++; planner_restore(it->second); return it->second; }
    g_emitter.planner_fallbacks++;
    if (getenv("SCRIP_PLANNER_REPORT")) fprintf(stderr, "[PLANNER-FALLBACK] prefix=%s graph=%p n=%d\n", prefix ? prefix : "?", (const void *) g_emit_cfg, g_emit_cfg ? g_emit_cfg->n : -1);
    zeta_plan_t * rec = planner_chain_plan(entry, prefix); rec->cfg = g_emit_cfg; rec->key = entry; planner_restore(rec); return rec;
}
}

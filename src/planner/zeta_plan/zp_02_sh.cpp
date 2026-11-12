#include "planner/zeta_plan/zp_head.h"
#include "planner/zeta_plan/zp_01_sh.h"
#include "planner/zeta_plan/zp_02_sh.h"
#include "planner/zeta_plan/zp_03_sh.h"
#include "planner/zeta_plan/zp_04_sh.h"
extern "C" {
int zd_pl_dj_arm_holds(const IR_t * d, int a) {
    IR_t * t = zd_chase(d->operands[2 * a + 1]);
    return t && t != d && t->op != IR_FAIL && t->op != IR_SUCCEED && t->op != IR_UNMARK && t->op != IR_CUT;
}
int zd_pl_dj_holds(const IR_t * d) {
    int na = (d && d->op == IR_DISJUNCTION) ? (int)IR_LIT(d).ival : 0;
    if (na <= 0 || d->n_operands < 2 * na) return 0;
    for (int a = 0; a < na; a++) if (zd_pl_dj_arm_holds(d, a)) return 1;
    return 0;
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
IR_t * zd_suspend_run_next(IR_t ** nodes, int n, IR_t * sp, const int * claim) {
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
int zd_icn_chain_step_ok(const IR_graph_t * cg, IR_t ** nodes, int n, int p, int q) {
    if (!(cg && cg->spine_cells)) return 1;
    if (p < 0 || q < 0 || zd_chase(nodes[p]->ω.node) != nodes[q]) return 0;
    switch (nodes[p]->op) {
        case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_DSTAR: case IR_LIT_THUNK: case IR_LIT_REAL: case IR_VAR: case IR_VAR_REF: case IR_DEREF: case IR_COERCE_NUMERIC: case IR_BINOP:
            case IR_BINOP_TEST: case IR_UNOP: case IR_ASSIGN:
        case IR_CALL_BYNAME: case IR_CALL_DIRECT: case IR_LINE_MARK: return 1;
        default: return 0;
    }
}
int zd_map_on(void) { if (g_planner.zd_map_on__m < 0) { const char * e = getenv("SCRIP_ZD_MAP"); g_planner.zd_map_on__m = (e && *e == '1') ? 1 : 0; } return g_planner.zd_map_on__m; }
static zdo_slot_t *zdo_probe(const IR_t *k) { uint64_t h = (((uint64_t)(uintptr_t)k >> 4) * 11400714819323198485ull) & (g_planner.g_zdo.len - 1);
    for (;;) { zdo_slot_t *q = &CV_AT(g_planner.g_zdo, zdo_slot_t, h); if (q->gen != g_planner.g_zdo_gen || q->k == k) return q; h = (h + 1) & (g_planner.g_zdo.len - 1); } }
void zdo_build(IR_t **nodes, int n) {
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
extern "C" int bb_proc_target_retains(const char *fname) {
    if (!fname) return 0;
    {
        int i = bb_proc_find(fname);
        if (i >= 0) {
            int bi = g_stage2.proc_table[i].bb_idx;
            IR_graph_t * cg = (bi >= 0 && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0;
            return (!cg || zls_g_resumable_frame(cg)) ? 1 : 0;
        }
    }
    return 1;
}
extern "C" int planner_callee_uniform_ft(const char * fname, int * ft) {
    int i = fname ? bb_proc_find(fname) : -1;
    int bi = i >= 0 ? g_stage2.proc_table[i].bb_idx : -1;
    IR_graph_t * cg = (bi >= 0 && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0;
    if (!cg) return 0;
    if (ft) *ft = cg->pf.gen_ft;
    return cg->pf.uniform;
}
extern "C" int planner_callee_gen_ft(const char * fname) {
    int i = fname ? bb_proc_find(fname) : -1;
    if (i < 0 || g_stage2.proc_table[i].dyn_scope) return -1;
    int bi = g_stage2.proc_table[i].bb_idx;
    IR_graph_t * cg = (bi >= 0 && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0;
    return cg ? cg->pf.gen_ft : -1;
}
static int planner_fence0_whack_on() { if (g_planner.fence0_whack_on_v < 0) { const char * e = getenv("SCRIP_FENCE0_WHACK"); g_planner.fence0_whack_on_v = (e && *e == '0') ? 0 : 1;
    } return g_planner.fence0_whack_on_v; }
int planner_fence0_release(const IR_graph_t * cg, const IR_t * nd) {
    if (!planner_fence0_whack_on()) return 0;
    if (!nd || !cg || nd->op != IR_MATCH_FENCE0) return 0;
    int n = cg->n; if (n <= 0) return 0;
    int _fzd = getenv("SCRIP_FZ_DIAG") ? 1 : 0;
    IR_t ** seq = (IR_t **)alloca(sizeof(IR_t *) * (size_t)n); int ns = -1;
    for (int b = 0; b < n && ns < 0; b++) { IR_t * mb = cg->all[b]; if (!mb || mb->op != IR_MATCH_BEGIN) continue;
        int cnt = 0, guard = 0; IR_t * cur = zd_chase(mb->γ.node);
        while (cur && guard++ <= n) { if (cur == nd) { ns = cnt; break; } if (cnt >= n) break; seq[cnt++] = cur; cur = zd_chase(cur->γ.node); } }
    { int has_mb = 0; for (int b = 0; b < n; b++) if (cg->all[b] && cg->all[b]->op == IR_MATCH_BEGIN) { has_mb = 1; break; }
      if (ns < 0 && !has_mb && cg->entry) { int cnt = 0, guard = 0; IR_t * cur = zd_chase(cg->entry);
        while (cur && guard++ <= n) { if (cur == nd) { ns = cnt; break; } if (cnt >= n) break; seq[cnt++] = cur; cur = zd_chase(cur->γ.node); } } }
    if (ns < 0) { if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p not on any MATCH_BEGIN or thunk-entry forward chain -- bill 0\n", (const void *)nd); return 0; }
    if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p executed-prefix len=%d (cfg_n=%d)\n", (const void *)nd, ns, n);
    int total = 0;
    for (int j = ns - 1; j >= 0; j--) { IR_t * m = seq[j]; int mo = (int)ir_kind0(m);
        if (_fzd) fprintf(stderr, "[FZ-DIAG]   step j=%-3d op=%-22s zd_k=%-3d total=%d\n", j, bb_op_name((IR_e)mo), zd_k(m), total);
        if (mo == IR_MATCH_BEGIN || mo == IR_STATEMENT_BEGIN) break;
        if (mo == IR_MATCH_ALTERNATE || mo == IR_MATCH_ARBNO || mo == IR_MATCH_FENCE1 || mo == IR_MATCH_FENCE0 || mo == IR_MATCH_DEFER || mo == IR_MATCH_VALUE || mo == IR_CALL ||
            mo == IR_CALL_VALUE || mo == IR_DISJUNCTION || mo == IR_MATCH_ABORT) break;
        if (mo == IR_MATCH_SPAN || mo == IR_MATCH_BREAK || mo == IR_MATCH_BREAKX || mo == IR_MATCH_TAB || mo == IR_MATCH_RTAB || mo == IR_MATCH_REM || mo == IR_MATCH_BAL) { total += zd_k(m); continue;
            }
        if (zd_k(m) == 0) continue;
        break; }
    if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p RELEASE=%d\n", (const void *)nd, total);
    return total;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zd_omega_test_kind(const IR_t * nd) { return (nd && nd->ω.node && ir_decides_own_failure_kind(nd->op)) ? 1 : 0; }
int zd_omega_head(IR_t **nodes, int n, IR_t *t) { return zdo_first(nodes, n, t) >= 0; }
int zd_omega_seed(IR_t **nodes, int n, IR_t *t, unsigned char *zon, int *zout, const int *claim) { int k = zdo_first(nodes, n, t); if (k < 0) return 0; int s = zon[k] ? zout[k] : 0;
    for (int j = k + 1; j < n; j++) if (zon[j] && claim[j] == claim[k] && zout[j] > s && zd_omega_test_kind(nodes[j]) && zd_chase(nodes[j]->ω.node) == t) s = zout[j]; return s; }
int zd_omega_test_idx(IR_t **nodes, int n, IR_t *t) { return zdo_first(nodes, n, t); }
int zd_vl_join(const IR_t *last, const IR_t *cur) { return last && cur && last->op == IR_ASSIGN && cur->op == IR_VAR && IR_LIT(last).sval && IR_LIT(cur).sval &&
    !strcmp(IR_LIT(last).sval, IR_LIT(cur).sval); }
int zd_vl_sibling(IR_t **nodes, int n, const int *claim, int c, IR_t *cur) {
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
int zd_exit_pop_s(IR_e op, int mark, int full, int stmt, int mafter) { if (!(mark >= 0 && zd_stmt_exit_kind(op))) return full; int wm = mark;
    if (op == IR_GOTO_DEFERRED && stmt >= 0) return full - (stmt - wm);
    return (mafter >= 0 && full > mafter) ? (wm + (full - mafter)) : wm; }
const char * zd_edge_sz(IR_t * x, int v) {
    const char * sz = v ? x->ω.sz : x->γ.sz;
    IR_t * t = v ? x->ω.node : x->γ.node;
    int g = 0;
    while (t && t->op == IR_GOTO && g++ < 128) { sz = t->γ.sz; t = t->γ.node; }
    return sz;
}
int zd_sz_sigma(const char * sz) { return (unsigned char)sz[0] == 0xcf && (unsigned char)sz[1] == 0x83 && sz[2] == 0; }
int zd_sz_phi(const char * sz) { return (unsigned char)sz[0] == 0xcf && (unsigned char)sz[1] == 0x86 && sz[2] == 0; }
int zd_pl_resume_depth(const IR_graph_t * cg, IR_t ** nodes, int n, const unsigned char * zon, const int * zout, const int * claim, const int * base, const unsigned char * known, int r) {
    if (r < 0 || !zon[r] || nodes[r]->op != IR_CLEANUP || IR_LIT(nodes[r]).ival != CLEANUP_RESUME) return -1;
    IR_t * t = zd_chase(nodes[r]->ω.node);
    int j = t ? nidx(nodes, n, t) : -1;
    if (j < 0 || j == r || !zon[j] || claim[j] < 0 || !known[claim[j]]) return -1;
    const char * sz = zd_edge_sz(nodes[r], 1);
    int beta = port_sz_beta(sz) || (zd_sz_phi(sz) && ir_glue_box(nodes[j], 1));
    return base[claim[j]] + zout[j] - (beta ? 0 : zd_k_g(cg, nodes[j]));
}
int zd_unmark_acts(const IR_graph_t * cg, const IR_t * nd) {
    const IR_t * sc = nd;
    if (sc->n_operands > 0 && sc->operands[0] && sc->operands[0]->op == IR_BOUND) sc = sc->operands[0];
    return cg ? zls_g_release_act_offs(cg, sc, 5, (int *)0, 0) : 0;
}
IR_t * zd_lm_chase(IR_t * x, int n, IR_t ** last) { IR_t * s = x; IR_t * t = zd_chase(x->γ.node); int g = 0;
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
    int never_fails = o == IR_ASSIGN || o == IR_VAR || o == IR_VAR_REF || o == IR_LINE_MARK || o == IR_BOUND || o == IR_LIT_INTEGER || ir_is_lit_text_kind((IR_e)o) || o == IR_LIT_REAL ||
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
int zd_pred_bt(const IR_graph_t * cg, IR_t ** nodes, int n, const int * claim, int hi, int k) {
    int p = zd_pred(cg, nodes, n, claim, hi, k);
    for (int g = 0; p >= 0 && nodes[p]->op == IR_LINE_MARK && g++ <= n; ) p = zd_pred(cg, nodes, n, claim, hi, p);
    return p;
}
int zd_pl_land(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, int i, int e, int *q, int *beta) {
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
void zst_finish(const IR_graph_t * cg, IR_t ** nodes, int n, const int * zout, const int * claim, const int * base, const unsigned char * known) {
    if (g_planner.zst_nodes != nodes || g_planner.zst_n != n) return;
    for (size_t t = 0; t < g_planner.zst_c.size(); t++) { int c = g_planner.zst_c[t]; int rk = claim && claim[c] >= 0 && known && known[claim[c]] && base;
        g_planner.zst_L[t] = rk ? base[claim[c]] + zout[c] - zd_k_g(cg, nodes[c]) + g_planner.zst_s[(size_t)c] - 16 * g_planner.zst_j[t] : INT_MIN; }
    if (zd_map_on()) for (size_t t = 0; t < g_planner.zst_c.size(); t++) { int c = g_planner.zst_c[t];
        fprintf(stderr, "[ZD-MAP] STICKY name=%s root=%d c=%d (%s zout=%d k=%d own=%d s=%d) j=%d L=%d\n", g_planner.zst_name[t], g_planner.zst_vroot[t], c, bb_op_name(nodes[c]->op), zout[c],
            zd_k_g(cg, nodes[c]), zd_k_own_g(cg, nodes[c]), g_planner.zst_s[(size_t)c], g_planner.zst_j[t], g_planner.zst_L[t]); }
}
int zd_g_into_glue_beta(IR_t * x, IR_t * t) { const char * sz = zd_edge_sz(x, 0);
    return t->op == IR_MATCH_BEGIN && ((zd_sz_sigma(sz) && ir_glue_box(t, 0)) || (zd_sz_phi(sz) && ir_glue_box(t, 1))); }
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
            int rdd = (dq >= 0 && zon[dq]) ? ((xb[dq] != INT_MIN) ? xb[dq] + zout[dq] : (zcut && zcut[dq] >= 0) ? zcut[dq] : INT_MIN) : INT_MIN;
            if (rdd == INT_MIN) continue;
            expect = xb[i] + zout[i] - rdd;
            if (ig && g_planner.g_zd_rec_p && nodes[i]->n_operands > 0) {
                const int jg = nidx(nodes, n, ig);
                IR_t * rt = nodes[i]->operands[0];
                const int kt = (rt && rt->op != IR_SUCCEED && rt->op != IR_FAIL) ? nidx(nodes, n, rt) : -1;
                int aj = -1;
                for (int j = 0; jg >= 0 && j < ig->n_operands; j++) if (ig->operands[j] == nodes[i]) { aj = j; break; }
                const int gto = (jg >= 0 && jg < g_planner.g_zd_rec_p->n) ? g_planner.g_zd_rec_p->zn[jg].gto : -1;
                const int vg = (aj >= 0 && gto >= 0 && (size_t)(gto + aj) < g_planner.g_zd_rec_p->v_gate.size()) ? g_planner.g_zd_rec_p->v_gate[(size_t)(gto + aj)] : INT_MIN;
                if (kt >= 0 && zon[kt] && xb[kt] != INT_MIN && vg != INT_MIN) expect = xb[i] + zout[i] - (xb[kt] + zout[kt] - (nodes[i]->seal ? 0 : zd_k_g(cg, nodes[kt]))) - vg;
            }
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
static int zd_has_match(IR_t ** nodes, int n) {
    for (int i = 0; i < n; i++) if (!strncmp(bb_op_name(nodes[i]->op), "IR_MATCH", 8)) return 1;
    return 0;
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
            int ad = zd_out[t] - zd_k_g(cg, g_planner.codegen_flat_chain_body_nodes[t]) + planner_fence0_release(cg, g_planner.codegen_flat_chain_body_nodes[t]); int d = 0;
        if (zd_on[k] || ad <= 0 || !strncmp(bb_op_name(g_planner.codegen_flat_chain_body_nodes[k]->op), "IR_MATCH_", 9) ||
            !strncmp(bb_op_name(g_planner.codegen_flat_chain_body_nodes[t]->op), "IR_MATCH_", 9) || _in_arms(t, k)) continue; _zw[t] = 1; _add++;
        if (getenv("SCRIP_ZD_DIAG"))
            fprintf(stderr, "[ZD] JOIN-WALL i=%d (%s) alpha depth %d entered at depth %d by i=%d (%s)\n", t, bb_op_name(g_planner.codegen_flat_chain_body_nodes[t]->op), ad, d, k,
            bb_op_name(g_planner.codegen_flat_chain_body_nodes[k]->op));
        } } if (!_add) break; zd_plan(cg, g_planner.codegen_flat_chain_body_nodes, n, zd_on, zd_out, zd_gp, zd_wp, zd_arm, _zw, zd_cps, zd_cpd, zd_chn, zd_claim); }
        }
    unsigned char * zd_unr = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
    unsigned char * zd_adead = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
    for (int i = 0; i < n; i++) { zd_unr[i] = 0; zd_adead[i] = 0; }
    if (!(cg && cg->n_alts > 0) && !_stmt_seed && !zd_has_match(g_planner.codegen_flat_chain_body_nodes, n)) {
        auto arm_yields = [&](int dj, int k) -> int { IR_t * d = g_planner.codegen_flat_chain_body_nodes[dj]; std::vector<IR_t *> st, seen; st.push_back(zd_chase(d->operands[k - 1]));
            while (!st.empty()) { IR_t * x = st.back(); st.pop_back(); if (!x || x == d) continue; int dup = 0; for (IR_t * y : seen) if (y == x) { dup = 1; break;
                } if (dup || seen.size() > (size_t)(2 * n + 2)) continue;
                seen.push_back(x); if (x->op == IR_SUSPEND) return 1; st.push_back(zd_chase(x->γ.node)); st.push_back(zd_chase(x->ω.node)); } return 0; };
        auto landed = [&](int h, int beta) -> int {
            for (int i = 0; i < n; i++) {
                if (zd_claim[i] < 0 || zd_claim[i] == h || zd_unr[i]) continue;
                for (int v = 0; v < 2; v++) { IR_t * t = zd_chase(v ? g_planner.codegen_flat_chain_body_nodes[i]->ω.node : g_planner.codegen_flat_chain_body_nodes[i]->γ.node);
                    int q = t ? nidx(g_planner.codegen_flat_chain_body_nodes, n, t) : -1;
                    IR_t * t0 = v ? g_planner.codegen_flat_chain_body_nodes[i]->ω.node : g_planner.codegen_flat_chain_body_nodes[i]->γ.node;
                        int q0 = t0 ? nidx(g_planner.codegen_flat_chain_body_nodes, n, t0) : -1;
                    int bs = port_sz_beta(zd_edge_sz(g_planner.codegen_flat_chain_body_nodes[i], v)) ? 1 : 0;
                    if ((q >= 0 && zd_claim[q] == h && bs == beta) || (!beta && q0 >= 0 && q0 != q && zd_claim[q0] == h)) return 1; }
                IR_t * nd = g_planner.codegen_flat_chain_body_nodes[i]; int na = (nd->op == IR_DISJUNCTION) ? (int)IR_LIT(nd).ival : -1;
                for (int k = 0; k < nd->n_operands; k++) { int q = nd->operands[k] ? nidx(g_planner.codegen_flat_chain_body_nodes, n, nd->operands[k]) : -1;
                    if (q < 0 || zd_claim[q] != h || q == i) continue;
                    if (!beta && !(na >= 0 && (k >= 2 * na || (k & 1)))) return 1;
                    if (beta && na >= 0 && (k & 1) && arm_yields(i, k)) return 1; } }
            return 0; };
        for (int pass = 0, moved = 1; moved && pass < n + 2; pass++) { moved = 0;
            for (int h = 1; h < n; h++) { if (zd_claim[h] != h || zd_unr[h] || zd_adead[h] || h == zd_claim[0] || ir_floater_kind(g_planner.codegen_flat_chain_body_nodes[h]) || landed(h, 0)) continue;
                if (landed(h, 1)) { for (int i = 0; i < n; i++) if (zd_claim[i] == h) zd_adead[i] = 1;
                    if (zd_map_on()) fprintf(stderr, "[ZD-MAP] ADEAD run=%d (%s): entered only at beta; its alpha emits a trap, its base is the beta landing\n", h,
                        bb_op_name(g_planner.codegen_flat_chain_body_nodes[h]->op)); continue; }
                for (int i = 0; i < n; i++) if (zd_claim[i] == h) zd_unr[i] = 1; moved = 1;
                if (zd_map_on()) fprintf(stderr, "[ZD-MAP] UNREACHED run=%d (%s): no alpha landing in the plan; its boxes emit a trap, no cell access\n", h,
                    bb_op_name(g_planner.codegen_flat_chain_body_nodes[h]->op)); } } }
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
    rec->unr = (unsigned char *)ct_zalloc((size_t)(n > 0 ? n : 1), 1); memcpy(rec->unr, zd_unr, (size_t)n);
    rec->adead = (unsigned char *)ct_zalloc((size_t)(n > 0 ? n : 1), 1); memcpy(rec->adead, zd_adead, (size_t)n);
    rec->chn = (int *)ct_zalloc((size_t)(2 * n + 2), sizeof(int)); memcpy(rec->chn, zd_chn, sizeof(int) * (size_t)(2 * n + 2));
    if (g_planner.g_zd_zarm_p == zd_arm) g_planner.g_zd_zarm_p = rec->arm;
    if (n > 0) {
        zd_pdef_scope pds(cg);
        int * _b = (int *)alloca(sizeof(int) * (size_t)n);
        unsigned char * _kn = (unsigned char *)alloca((size_t)n);
        zd_xb = (int *)alloca(sizeof(int) * (size_t)n);
        zd_cut = (int *)alloca(sizeof(int) * (size_t)n);
        int * zd_fam = (int *)alloca(sizeof(int) * (size_t)n);
        int * zd_fbase = (int *)alloca(sizeof(int) * (size_t)n);
        zd_pl_bases(cg, g_planner.codegen_flat_chain_body_nodes, n, zd_on, zd_out, zd_gp, zd_wp, zd_claim, _b, _kn, (int *)0, zd_cut, zd_fam, zd_fbase, zd_adead);
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
}

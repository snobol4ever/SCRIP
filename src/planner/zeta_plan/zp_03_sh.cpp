#include "planner/zeta_plan/zp_head.h"
#include "planner/zeta_plan/zp_01_sh.h"
#include "planner/zeta_plan/zp_02_sh.h"
#include "planner/zeta_plan/zp_03_sh.h"
extern "C" { static int zd_bound_spine0(const IR_graph_t * g, const IR_t * nd); }
extern "C" {
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
static uint64_t zd_pdef_hash(const IR_graph_t * g) {
    uint64_t h = (uint64_t)(uintptr_t)g * 0x9E3779B97F4A7C15ull ^ (uint64_t)g->n;
    for (int i = 0; i < g->n; i++) { const IR_t * x = g->all[i]; if (!x) continue;
        h = (h ^ (uint64_t)(uintptr_t)x->γ.node ^ ((uint64_t)(uintptr_t)x->ω.node << 1) ^ ((uint64_t)x->n_operands << 7) ^ ((uint64_t)x->seal << 3)) * 0x100000001B3ull;
        for (int k = 0; k < x->n_operands; k++) h = (h ^ (uint64_t)(uintptr_t)x->operands[k]) * 0x100000001B3ull; }
    return h;
}
static void zd_pdef_fill(const IR_graph_t * g) {
    uint64_t h = zd_pdef_hash(g);
    if (g_planner.pdef_g == g && g_planner.pdef_n == g->n && g_planner.pdef_h == h) return;
    g_planner.pdef.assign((size_t)g->n, 0); planner_mark_pass(g, g_planner.pdef.data()); g_planner.pdef_g = g; g_planner.pdef_n = g->n; g_planner.pdef_h = h;
    g_planner.pdef_ix.clear(); for (int i = 0; i < g->n; i++) if (g->all[i]) g_planner.pdef_ix[g->all[i]] = i;
}
extern "C" const unsigned char * planner_pass_marks(const IR_graph_t * g) { if (!g || g->n <= 0) return (const unsigned char *)0; zd_pdef_fill(g); return g_planner.pdef.data(); }
extern "C" void planner_pdef_lock(const IR_graph_t * g) { if (g_planner.pdef_depth++ == 0 && g) { g_planner.pdef_lock = g; g_planner.pdef_memo.clear(); if (g->n > 0) zd_pdef_fill(g); } }
extern "C" void planner_pdef_unlock(void) { if (g_planner.pdef_depth > 0 && --g_planner.pdef_depth == 0) { g_planner.pdef_lock = (const IR_graph_t *)0; g_planner.pdef_memo.clear(); } }
static int zd_pdef_locked(const IR_graph_t * g) { return g && g_planner.pdef_lock == g && g_planner.pdef_g == g && g_planner.pdef_n == g->n; }
extern "C" int planner_node_definite(const IR_graph_t * g, const IR_t * nd) {
    if (!g || !nd || g->n <= 0) return 0;
    if (zd_pdef_locked(g)) { auto it = g_planner.pdef_ix.find(nd); return (it != g_planner.pdef_ix.end() && !g_planner.pdef[(size_t)it->second]) ? 1 : 0; }
    zd_pdef_fill(g);
    for (int i = 0; i < g->n; i++) if (g->all[i] == nd) return g_planner.pdef[(size_t)i] ? 0 : 1;
    return 0;
}
static int zd_bound_arm_resumes(const IR_graph_t * g, const IR_t * nd, const IR_t * ig) {
    std::vector<const IR_t *> seen(1, nd), stk(1, nd);
    while (!stk.empty()) {
        const IR_t * x = stk.back();
        stk.pop_back();
        if (x != nd && (x->op == IR_SUSPEND || x->op == IR_DISJUNCTION || zd_edge_retains_g(g, x))) return 1;
        for (int e = -2; e < x->n_operands; e++) {
            const IR_t * y = e == -2 ? x->γ.node : e == -1 ? x->ω.node : x->operands[e];
            if (!y || y == ig || y == ig->γ.node || y == ig->ω.node || std::find(seen.begin(), seen.end(), y) != seen.end()) continue;
            seen.push_back(y);
            stk.push_back(y);
        }
    }
    return 0;
}
static int zd_pl_freezes(const IR_graph_t * g) { return g && zls_g_resumable_frame(g) && zls_g_home_frame(g); }
extern "C" int planner_bound_spine(const IR_graph_t * g, const IR_t * nd) {
    if (!nd || nd->op != IR_BOUND) return 0;
    { int v = zls_fcn_bs_get(nd, 2); if (v) return v == 2; }
    int r = zd_bound_spine0(g, nd);
    if (zd_pl_freezes(g)) zls_fcn_bs_set(nd, 2, r ? 2 : 1);
    return r;
}
static int zd_bound_spine0(const IR_graph_t * g, const IR_t * nd) {
    if (!g || !nd || nd->op != IR_BOUND || !(zls_g_home_frame(g) || (g->home_frame && planner_node_definite(g, nd))) || !zls_g_resumable_frame(g)) return 0;
    const IR_t * ig = zd_chase((IR_t *)nd->ω.node);
    if (!ig || ig->op != IR_GATE) return 0;
    int fence = zls_fcn_bs_get(nd, 0);
    if (!fence) {
        for (int k = 0; k < g->n && fence != 2; k++) { const IR_t * u = g->all[k]; if (u && u->op == IR_UNMARK && u->n_operands > 0 && u->operands[0] == nd && IR_LIT(u).ival == 2) fence = 2; }
        if (fence != 2) fence = 1;
        zls_fcn_bs_set(nd, 0, fence);
    }
    if (fence == 1) return 0;
    for (int k = 0; k < g->n; k++) { const IR_t * u = g->all[k]; if (u && u->op == IR_CLEANUP && u->n_operands > 0 && u->operands[0] == nd) return 0; }
    const int bdef = planner_node_definite(g, nd);
        std::vector<const IR_t *> seen(1, nd), stk(1, nd);
    int r = 0;
    while (!stk.empty() && !r) {
        const IR_t * x = stk.back();
        stk.pop_back();
        if (x != nd && (x->op == IR_SUSPEND || (!bdef && (zd_edge_retains_g(g, x) || x->op == IR_DISJUNCTION || x->op == IR_BOUND || (x->op == IR_CUT && !IR_LIT(x).ival))))) r = 1;
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
    if (!r && zd_bound_arm_resumes(g, nd, ig)) r = 1;
    return r ? 0 : 1;
}
static int zd_gate_cond_retains(const IR_graph_t * g, const IR_t * b, const IR_t * gate) {
    std::vector<const IR_t *> seen, stk;
    const IR_t * s = b ? zd_chase((IR_t *)b->γ.node) : (const IR_t *)0;
    if (s && s->op == IR_GATE_ARM && s->n_operands > 1 && s->operands[1] == gate) s = zd_chase((IR_t *)s->γ.node);
    if (s) stk.push_back(s);
    while (!stk.empty()) {
        const IR_t * y = stk.back();
        stk.pop_back();
        if (!y || y == gate || std::find(seen.begin(), seen.end(), y) != seen.end()) continue;
        seen.push_back(y);
        if (y->op == IR_GATE_ARM && y->n_operands > 1 && y->operands[1] == gate) continue;
        if (y->op == IR_SUSPEND || y->op == IR_DISJUNCTION || zd_edge_retains_g(g, y)) return 1;
        stk.push_back(zd_chase((IR_t *)y->γ.node));
        for (int e = 0; e < y->n_operands; e++) stk.push_back(y->operands[e]);
    }
    return 0;
}
extern "C" int planner_gate_cells(const IR_graph_t * g, const IR_t * gate) {
    if (!g || !gate || gate->op != IR_GATE || !zd_pl_home(g) || !getenv("SCRIP_PL_GATE_CELLS")) return 0;
    int v = zls_fcn_bs_get(gate, 3);
    if (v) return v == 2;
    const IR_t * t = zd_chase((IR_t *)gate->γ.node);
    int ok = !(t && (t->op == IR_GATE_ARM || t->op == IR_DISJUNCTION));
    for (int i = 0; i < g->n && ok; i++) {
        const IR_t * x = g->all[i];
        if (x && x != gate && zd_chase((IR_t *)x->γ.node) == gate && port_sz_beta(x->γ.sz) && !(x->op == IR_UNMARK && x->n_operands > 0 && !zd_gate_cond_retains(g, x->operands[0], gate)))
            ok = 0;
    }
    zls_fcn_bs_set(gate, 3, ok ? 2 : 1);
    return ok;
}
int zd_pl_bspine(const IR_graph_t * cg, const IR_t * nd) { return nd && nd->op == IR_BOUND && planner_bound_spine(cg, nd); }
int zd_pl_rest_ival(const IR_graph_t * cg, const IR_t * u) {
    if (!(IR_LIT(u).ival & 6)) return 0;
    return !((IR_LIT(u).ival & 32) && u->n_operands > 0 && zd_pl_bspine(cg, u->operands[0]));
}
static int zd_pl_unmark_takes_bound(const IR_graph_t * cg, const IR_t * u) {
    if (IR_LIT(u).ival & 8) return 0;
    return !((IR_LIT(u).ival & 32) && u->n_operands > 0 && zd_pl_bspine(cg, u->operands[0]));
}
int zd_mark_restore(const IR_t * u) { return u && u->op == IR_UNMARK && u->n_operands > 0 && (IR_LIT(u).ival & 6) && !(IR_LIT(u).ival & 32); }
int zd_pl_unmark_bound(const IR_graph_t * cg, IR_t ** nodes, int n, int i) {
    IR_t * nd = nodes[i];
    if (nd->op != IR_UNMARK || nd->n_operands < 1 || !planner_resumable_frame(cg) || !zd_pl_rest_ival(cg, nd)) return -1;
    return nidx(nodes, n, nd->operands[0]);
}
int zd_pl_unmark_exit(const IR_graph_t * cg, IR_t ** nodes, int n, const unsigned char * zon, const int * zout, const int * claim, const unsigned char * known, const int * base,
    const int * fam,
                             const int * fbase, int i, int ai) {
    int ub = (IR_LIT(nodes[i]).ival & 8) ? -1 : zd_pl_unmark_bound(cg, nodes, n, i), cu = ub >= 0 && zon[ub] ? claim[ub] : -1;
    if (cu < 0 || !zd_pl_bspine(cg, nodes[ub]) || !(known[cu] || fam[cu] >= 0) || !known[claim[i]] != !known[cu]) return ai;
    return (known[cu] ? base[cu] : fbase[cu]) + zout[ub] - zd_k_own_g(cg, nodes[ub]);
}
static int zd_pl_restores_head(const IR_graph_t * cg, IR_t ** nodes, int n, const int * claim, int h) { int ub = zd_pl_unmark_bound(cg, nodes, n, h);
    return ub >= 0 && claim[ub] >= 0 && claim[ub] != h;
    }
static int zd_pl_edges(IR_t * nd) { return 2 + ((nd->op == IR_DISJUNCTION || nd->op == IR_GATE) ? nd->n_operands : 0); }
int zd_pl_dj_join(IR_t **nodes, int n, int i, int q) {
    if (q < 0 || nodes[q]->op != IR_DISJUNCTION) return 0;
    for (int a = 0; a < (int)IR_LIT(nodes[q]).ival; a++) if (zd_dj_arm_end(nodes, n, q, a) == i) return 1;
    return 0;
}
int zd_pl_dj_gate_end(IR_t **nodes, int n, int i, int q) {
    if (q < 0 || nodes[q]->op != IR_DISJUNCTION) return 0;
    for (int a = 0; a < (int)IR_LIT(nodes[q]).ival; a++) { int e = zd_dj_arm_end(nodes, n, q, a);
        if (e >= 0 && nodes[e]->op == IR_GATE_ARM && !nodes[e]->γ.node && nodes[e]->n_operands > 1 && nidx(nodes, n, nodes[e]->operands[1]) == i) return 1; }
    return 0;
}
void zd_pl_bases(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, int *base, unsigned char *known, int *aof,
    int *zcut, int *fam, int *fbase, const unsigned char *adead){
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
    if (adead) for (int pass = 0, moved = 1; moved && pass < n + 2; pass++) { moved = 0;
        for (int i = 0; i < n; i++) { if (!zon[i] || claim[i] < 0 || !known[claim[i]]) continue;
            IR_t * nd = nodes[i];
            for (int v = 0; v < 2; v++) { int q, b; int l = zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, i, v, &q, &b);
                if (q < 0 || !b || claim[q] < 0 || !adead[claim[q]] || known[claim[q]] || claim[q] == claim[i]) continue;
                base[claim[q]] = base[claim[i]] + l - zout[q]; known[claim[q]] = 1; moved = 1;
                if (zd_map_on()) fprintf(stderr, "[ZD-MAP] BETA-BASE run=%d (%s) from i=%d edge=%d l=%d base=%d\n", claim[q], bb_op_name(nodes[q]->op), i, v, l, base[claim[q]]); }
            if (nd->op == IR_DISJUNCTION) for (int k = 1; k < nd->n_operands; k += 2) { int e = nd->operands[k] ? nidx(nodes, n, nd->operands[k]) : -1;
                if (e < 0 || e == i || !zon[e] || claim[e] < 0 || !adead[claim[e]] || known[claim[e]] || claim[e] == claim[i]) continue;
                base[claim[e]] = base[claim[i]] + zout[i] - zd_k_g(cg, nd) + zgpop[e] - zout[e]; known[claim[e]] = 1; moved = 1;
                if (zd_map_on()) fprintf(stderr, "[ZD-MAP] BETA-BASE run=%d (%s) arm end of disjunction i=%d base=%d\n", claim[e], bb_op_name(nodes[e]->op), i, base[claim[e]]); } } }
    {
        unsigned char * indef = (unsigned char *)alloca((size_t)(n > 0 ? n : 1));
        int * wl = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
        for (int i = 0; i < n; i++) indef[i] = 0;
        if (pl) { zd_pdef_fill(cg); for (int i = 0; i < n; i++) { if (!zon[i] || claim[i] < 0 || nodes[i]->op == IR_GOTO || (nodes[i]->op == IR_SUCCEED && nodes[i]->γ.node)) continue;
            auto it = g_planner.pdef_ix.find(nodes[i]); if (it != g_planner.pdef_ix.end() && g_planner.pdef[(size_t)it->second]) indef[claim[i]] = 1; } }
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
                    if (!zon[h] || claim[h] != h || known[h]) continue;
                    int ub = (nodes[h]->op == IR_UNMARK && nodes[h]->n_operands > 0) ? nidx(nodes, n, nodes[h]->operands[0]) : -1;
                    int bk = ub >= 0 && zon[ub] && claim[ub] >= 0 && claim[ub] != h && known[claim[ub]] && !indef[claim[ub]];
                    if (indef[h] && !(bk && zd_pl_unmark_takes_bound(cg, nodes[h]))) continue;
                    if (bk) {
                        base[h] = base[claim[ub]] + zout[ub] - zout[h]; known[h] = 1; indef[h] = 0; moved = 1; continue;
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
            for (int r = 0; r < n; r++) {
                if (!zon[r] || nodes[r]->op != IR_CLEANUP || IR_LIT(nodes[r]).ival != CLEANUP_RESUME || nodes[r]->n_operands < 2 || claim[r] != r || known[r] || fam[r] >= 0) continue;
                int g = nidx(nodes, n, nodes[r]->operands[1]), q, b;
                if (g < 0 || !zon[g] || !zd_edge_retains_g(cg, nodes[g])) continue;
                (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, g, 0, &q, &b);
                if (q < 0 || !zon[q] || claim[q] < 0 || fam[claim[q]] < 0) continue;
                fam[r] = fam[claim[q]]; fbase[r] = fbase[claim[q]] + (zout[q] - (b ? 0 : zd_k_g(cg, nodes[q]))) - zout[r];
            }
            for (int pass = 0, moved = 1; moved && pass < n + 2; pass++) {
                moved = 0;
                for (int h = 0; h < n; h++) {
                    if (!zon[h] || claim[h] != h || fam[h] < 0) continue;
                    for (int mi = bst[h]; mi < bst[h + 1]; mi++) {
                        int i = bit[mi];
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
        for (int r = 0; zcut && pl && r < n; r++) { int d = zd_pl_resume_depth(cg, nodes, n, zon, zout, claim, base, known, r); if (d >= 0) zcut[r] = d; }
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
static int zst_edge(IR_t ** nodes, int n, int i, int e) {
    IR_t * nd = nodes[i], * t = (IR_t *)0;
    if (e == 0) { IR_t * ig = (nd->op == IR_GATE_ARM && !nd->γ.node && nd->n_operands > 1) ? nd->operands[1] : (IR_t *)0; t = zd_chase(ig ? ig->γ.node : nd->γ.node); }
    else if (e == 1) t = zd_chase(nd->ω.node);
    else if (nd->op == IR_GATE) { IR_t * arm = nd->operands[e - 2]; if (!arm || (int) arm->seal || arm->n_operands < 1) return -1; t = arm->operands[0]; }
    else t = nd->operands[e - 2];
    return t ? nidx(nodes, n, t) : -1;
}
static void zst_dj_names(const IR_t * nd, std::vector<const char *> & out) {
    if (!nd || nd->op != IR_DISJUNCTION) return;
    for (int j = 0, N = (int)IR_LIT(nd).ival; j < N && 2 * N + j < nd->n_operands; j++) {
        const IR_t * rj = nd->operands[2 * N + j];
        for (int g = 0; rj && rj->op == IR_CONJUNCTION && rj->n_operands > 0 && g < 16; g++) rj = rj->operands[0];
        if (rj && rj->op == IR_VAR && IR_LIT(rj).sval) out.push_back(IR_LIT(rj).sval);
    }
}
static int zst_isparam(const IR_graph_t * g, const char * vn) { for (int i = 0; g && g->pnames && i < g->nparams; i++) if (g->pnames[i] && !strcmp(g->pnames[i], vn)) return 1; return 0; }
extern "C" void planner_mark_pass(const IR_graph_t * cg, unsigned char * ind) {
    int n = cg ? cg->n : 0; IR_t ** nodes = cg ? cg->all : (IR_t **)0;
    if (n > 0) memset(ind, 0, (size_t)n);
    if (!cg || n <= 0 || !planner_resumable_frame(cg)) return;
    nidx_scope_t nix; nidx_scope_open(&nix, nodes, n);
    std::vector<int> wl, bl; std::vector<unsigned char> bseen((size_t)(n > 0 ? n : 1), 0); std::vector<int> gp1((size_t)(n > 0 ? n : 1), -1), gpn((size_t)(n > 0 ? n : 1), 0);
    for (int x = 0; x < n; x++) if (nodes[x] && !port_sz_beta(zd_edge_sz(nodes[x], 0))) { int t = zst_edge(nodes, n, x, 0); if (t >= 0 && t != x) { gpn[(size_t)t]++; gp1[(size_t)t] = x; } }
    for (int t = 0; t < n; t++) if (gpn[(size_t)t] != 1) gp1[(size_t)t] = -1;
    for (int i = 0; i < n; i++) if (nodes[i] && zd_edge_retains_g(cg, nodes[i])) { int q = zst_edge(nodes, n, i, 0);
        for (int cur = i, hop = 0; q >= 0 && hop <= n && (zd_pl_dj_join(nodes, n, cur, q) || zd_pl_dj_gate_end(nodes, n, cur, q)); hop++) { cur = q; q = zst_edge(nodes, n, q, 0); }
        if (q >= 0 && q != i && !ind[q] && !zd_mark_restore(nodes[q])) { ind[q] = 1; bl.push_back(q); if (nodes[q]->op != IR_CUT) wl.push_back(q); } }
    std::vector<int> pend;
    auto drain = [&]() {
    while (!wl.empty()) { int v = wl.back(); wl.pop_back();
        for (int e = 0; e < zd_pl_edges(nodes[v]); e++) { if (e == 1 || (e >= 2 && nodes[v]->op != IR_DISJUNCTION)) continue;
            int w = zst_edge(nodes, n, v, e); if (w < 0 || w == v || !nodes[w] || (e == 0 && zd_pl_dj_join(nodes, n, v, w))) continue;
            if (e == 0) { IR_t * ig = (nodes[v]->op == IR_GATE_ARM && !nodes[v]->γ.node && nodes[v]->n_operands > 1) ? nodes[v]->operands[1] : (IR_t *)0;
                if (ig && zd_pl_dj_join(nodes, n, nidx(nodes, n, ig), w)) continue;
                if ((ig ? port_sz_beta(ig->γ.sz) : port_sz_beta(zd_edge_sz(nodes[v], 0))) && zd_beta_via_act(cg, nodes[w])) continue;
                if ((ig ? port_sz_beta(ig->γ.sz) : port_sz_beta(zd_edge_sz(nodes[v], 0))) && !bseen[(size_t)w]) { bseen[(size_t)w] = 1; bl.push_back(w); } }
            if (ind[w]) continue;
            if (zd_mark_restore(nodes[w])) { int ub = nidx(nodes, n, nodes[w]->operands[0]); if (ub >= 0 && !ind[ub]) { pend.push_back(w); continue; } }
            ind[w] = 1; bl.push_back(w); if (nodes[w]->op != IR_CUT) wl.push_back(w); }
        while (wl.empty() && !bl.empty()) { int b = bl.back(); bl.pop_back(); int x = gp1[(size_t)b];
            if (x >= 0 && !ind[x] && !zd_edge_retains_g(cg, nodes[x])) { ind[x] = 1; bl.push_back(x); if (nodes[x]->op != IR_CUT) wl.push_back(x); } } }
    };
    drain();
    for (int moved = 1; moved; ) { moved = 0;
        for (size_t k = 0; k < pend.size(); k++) { int w = pend[k]; if (ind[w]) continue; int ub = nidx(nodes, n, nodes[w]->operands[0]); if (ub >= 0 && ind[ub]) { ind[w] = 1; bl.push_back(w);
            wl.push_back(w); moved = 1; } }
        drain(); }
    std::vector<unsigned char> ia(ind, ind + n), rch((size_t)n, 0); std::vector<int> rl;
    auto reach = [&](IR_t * t) { int q = t ? nidx(nodes, n, zd_chase(t)) : -1; if (q >= 0 && nodes[q] && !ind[q] && !rch[(size_t)q]) { rch[(size_t)q] = 1; rl.push_back(q); } };
    reach(cg->entry);
    for (int a = 0; cg->alt_entry && a < cg->n_alts; a++) reach(cg->alt_entry[a]);
    for (int moved = 1; moved; ) { moved = 0;
        while (!rl.empty()) { int v = rl.back(); rl.pop_back(); IR_t * x = nodes[v];
            for (int e = 0; e < zd_pl_edges(x); e++) { if (e == 0 && zd_edge_retains_g(cg, x)) continue;
                if (e <= 1) { IR_t * ig = (e == 0 && x->op == IR_GATE_ARM && !x->γ.node && x->n_operands > 1) ? x->operands[1] : (IR_t *)0;
                    if (ig ? port_sz_beta(ig->γ.sz) : port_sz_beta(zd_edge_sz(x, e))) { int w = zst_edge(nodes, n, v, e);
                        if (w >= 0 && w != v && nodes[w]->op == IR_GATE) reach(nodes[w]);
                        continue; } }
                int w = zst_edge(nodes, n, v, e); if (w < 0 || w == v || (e == 0 && zd_pl_dj_join(nodes, n, v, w))) continue;
                reach(nodes[w]); } }
        for (int u = 0; u < n; u++) { IR_t * x = nodes[u]; int ub = zd_mark_restore(x) ? nidx(nodes, n, x->operands[0]) : -1;
            if (ub >= 0 && rch[(size_t)ub] && !rch[(size_t)u] && !ind[u]) { reach(x); moved = 1; } } }
    for (int i = 0; i < n; i++) if (nodes[i] && !rch[(size_t)i]) ind[i] = 1;
    if (getenv("SCRIP_PL_INDEF_DIAG")) { extern const char * zls_graph_name_get(const IR_graph_t *); const char * gn = zls_graph_name_get(cg);
        if (gn && !strcmp(gn, getenv("SCRIP_PL_INDEF_DIAG"))) for (int i = 0; i < n; i++) if (nodes[i])
            { fprintf(stderr, "[PL-INDEF] all=%d %s ind=%d a=%d r=%d ret=%d g=%d w=%d ops", i, bb_op_name(nodes[i]->op), ind[i], ia[(size_t)i], rch[(size_t)i],
            zd_edge_retains_g(cg, nodes[i]), nidx(nodes, n, zd_chase(nodes[i]->γ.node)), nidx(nodes, n, zd_chase(nodes[i]->ω.node)));
              for (int k = 0; k < nodes[i]->n_operands; k++) fprintf(stderr, " %d", nidx(nodes, n, zd_chase(nodes[i]->operands[k]))); fprintf(stderr, "\n"); } }
    nidx_scope_close(&nix);
}
static int zst_excursion_why(int lw) { return lw == PR_UNKNOWN_CALLEE || lw == PR_RETAINING_CALLEE || lw == PR_UNRESOLVED_CALLEE || lw == PR_GATE_HOLDS || lw == PR_RETAINING_TARGET; }
static int zst_dt_meet(const std::vector<int> & idom, const std::vector<int> & po, int a, int b) {
    while (a != b) { while (po[(size_t)a] < po[(size_t)b]) a = idom[(size_t)a]; while (po[(size_t)b] < po[(size_t)a]) b = idom[(size_t)b]; }
    return a;
}
static int zst_dominates(const zst_dtree_t & t, int a, int v) {
    return t.univ[(size_t)v] || (!t.univ[(size_t)a] && t.tin[(size_t)a] <= t.tin[(size_t)v] && t.tout[(size_t)v] <= t.tout[(size_t)a]);
}
static void zst_dtree_build(zst_dtree_t & t, int n, const std::vector<int> & pcnt, const std::vector<int> & preds, const std::vector<int> & scnt, const std::vector<int> & sd,
    const std::vector<unsigned char> & isroot) {
    const int S = n;
    std::vector<int> po((size_t)n + 1, -1), order; std::vector<unsigned char> seen((size_t)n, 0); std::vector<std::pair<int,int>> cs;
    order.reserve((size_t)n);
    for (int r = 0; r < n; r++) { if (!isroot[(size_t)r]) continue; seen[(size_t)r] = 1; cs.push_back(std::make_pair(r, scnt[(size_t)r]));
        while (!cs.empty()) { int v = cs.back().first; int & e = cs.back().second;
            if (e < scnt[(size_t)v + 1]) { int w = sd[(size_t)e]; e++;
                if (w >= 0 && !seen[(size_t)w] && !isroot[(size_t)w]) { seen[(size_t)w] = 1; cs.push_back(std::make_pair(w, scnt[(size_t)w])); } continue; }
            po[(size_t)v] = (int)order.size(); order.push_back(v); cs.pop_back(); } }
    po[(size_t)S] = (int)order.size();
    t.idom.assign((size_t)n + 1, -1); t.idom[(size_t)S] = S;
    for (int r = 0; r < n; r++) if (isroot[(size_t)r]) t.idom[(size_t)r] = S;
    for (int ch = 1; ch; ) { ch = 0;
        for (size_t k = order.size(); k-- > 0; ) { int v = order[k]; if (isroot[(size_t)v]) continue; int ni = -1;
            for (int e = pcnt[(size_t)v]; e < pcnt[(size_t)v + 1]; e++) { int p = preds[(size_t)e]; if (!seen[(size_t)p] || t.idom[(size_t)p] < 0) continue;
                ni = ni < 0 ? p : zst_dt_meet(t.idom, po, p, ni); }
            if (ni >= 0 && ni != t.idom[(size_t)v]) { t.idom[(size_t)v] = ni; ch = 1; } } }
    t.univ.assign((size_t)n, 1); for (size_t k = 0; k < order.size(); k++) t.univ[(size_t)order[k]] = 0;
    std::vector<int> ccnt((size_t)n + 2, 0), kid(order.size() + 1, -1);
    for (size_t k = 0; k < order.size(); k++) ccnt[(size_t)t.idom[(size_t)order[k]] + 1]++;
    for (int i = 0; i <= n; i++) ccnt[(size_t)i + 1] += ccnt[(size_t)i];
    { std::vector<int> f(ccnt); for (size_t k = 0; k < order.size(); k++) kid[(size_t)f[(size_t)t.idom[(size_t)order[k]]]++] = order[k]; }
    t.depth.assign((size_t)n + 1, ((n + 63) / 64) * 64); t.tin.assign((size_t)n + 1, 0); t.tout.assign((size_t)n + 1, 0); t.top.assign((size_t)n + 1, -1);
    int clk = 0; std::vector<std::pair<int,int>> ds; t.depth[(size_t)S] = 0; t.tin[(size_t)S] = clk++; ds.push_back(std::make_pair(S, ccnt[(size_t)S]));
    while (!ds.empty()) { int v = ds.back().first; int & e = ds.back().second;
        if (e < ccnt[(size_t)v + 1]) { int w = kid[(size_t)e]; e++; t.depth[(size_t)w] = t.depth[(size_t)v] + 1; t.top[(size_t)w] = v == S ? w : t.top[(size_t)v];
            t.tin[(size_t)w] = clk++; ds.push_back(std::make_pair(w, ccnt[(size_t)w])); continue; }
        t.tout[(size_t)v] = clk++; ds.pop_back(); }
}
void zst_plan(const IR_graph_t * cg, IR_t ** nodes, int n) {
    g_planner.zst_nodes = nodes; g_planner.zst_n = n; g_planner.g_zd_cur = -1; g_planner.zst_s.assign((size_t)(n > 0 ? n : 0), 0); g_planner.zst_root.assign((size_t)(n > 0 ? n : 0), -1);
    g_planner.zst_name.clear(); g_planner.zst_nmap_ok = 0; g_planner.zst_vroot.clear(); g_planner.zst_c.clear(); g_planner.zst_j.clear(); g_planner.zst_L.clear(); g_planner.zst_vuse.clear();
        g_planner.zst_vcov.clear();
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
    std::vector<int> scnt((size_t)n + 1, 0), ss, sd; for (size_t k = 0; k < es.size(); k++) scnt[(size_t)es[k] + 1]++;
    for (int i = 0; i < n; i++) scnt[(size_t)i + 1] += scnt[(size_t)i]; { std::vector<int> f2(scnt); sd.assign(es.size() + 1, -1);
        for (size_t k = 0; k < es.size(); k++) sd[(size_t)f2[(size_t)es[k]]++] = ed[k]; }
    zst_dtree_t dt; zst_dtree_build(dt, n, pcnt, preds, scnt, sd, isroot);
    int nroots = 0, lastroot = -1; for (int u = 0; u < n; u++) if (isroot[(size_t)u]) { nroots++; lastroot = u; }
    for (int v = 0; v < n; v++) { if (!(isroot[(size_t)v] || pcnt[(size_t)v + 1] > pcnt[(size_t)v])) continue;
        if (dt.univ[(size_t)v]) g_planner.zst_root[(size_t)v] = nroots == 1 ? lastroot : -1;
        else g_planner.zst_root[(size_t)v] = isroot[(size_t)dt.top[(size_t)v]] ? dt.top[(size_t)v] : -1; }
    std::vector<int> idx((size_t)n, -1), low((size_t)n, 0), st; std::vector<unsigned char> on((size_t)n, 0), loop((size_t)n, 0); int ctr = 0;
    for (int s0 = 0; s0 < n; s0++) { if (idx[(size_t)s0] >= 0) continue; std::vector<std::pair<int,int>> cs; cs.push_back(std::make_pair(s0, scnt[(size_t)s0]));
        idx[(size_t)s0] = low[(size_t)s0] = ctr++; st.push_back(s0); on[(size_t)s0] = 1;
        while (!cs.empty()) { int v = cs.back().first; int & e = cs.back().second;
            if (e < scnt[(size_t)v + 1]) { int w = sd[(size_t)e]; e++; if (w == v) loop[(size_t)v] = 1; else if (idx[(size_t)w] < 0) { idx[(size_t)w] = low[(size_t)w] = ctr++; st.push_back(w);
                on[(size_t)w] = 1; cs.push_back(std::make_pair(w, scnt[(size_t)w])); }
                else if (on[(size_t)w] && low[(size_t)w] < low[(size_t)v]) low[(size_t)v] = low[(size_t)w]; continue; }
            cs.pop_back(); if (!cs.empty() && low[(size_t)v] < low[(size_t)cs.back().first]) low[(size_t)cs.back().first] = low[(size_t)v];
            if (low[(size_t)v] == idx[(size_t)v]) { std::vector<int> mem; int w; do { w = st.back(); st.pop_back(); on[(size_t)w] = 0; mem.push_back(w); } while (w != v);
                if (mem.size() > 1) for (size_t m = 0; m < mem.size(); m++) loop[(size_t)mem[m]] = 1; } } }
    std::vector<unsigned char> indef0((size_t)n, 0);
    { std::vector<int> wl;
        for (int i = 0; i < n; i++) if (zd_edge_retains_g(cg, nodes[i])) { int q = zst_edge(nodes, n, i, 0);
            for (int cur = i, hop = 0; q >= 0 && hop <= n && (zd_pl_dj_join(nodes, n, cur, q) || zd_pl_dj_gate_end(nodes, n, cur, q)); hop++) { cur = q; q = zst_edge(nodes, n, q, 0); }
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
    std::vector<int> kown((size_t)n, INT_MIN);
    auto kown_of = [&](int d) { if (kown[(size_t)d] == INT_MIN) kown[(size_t)d] = zd_k_own_g(cg, nodes[d]); return kown[(size_t)d]; };
    std::unordered_map<std::string, std::vector<int>> nmuse, hlk;
    for (int i = 0; i < n; i++) {
        const char * nn[2]; std::vector<const char *> dn; int m = zst_names(nodes[i], nn);
        if (g->home_frame) zst_dj_names(nodes[i], dn);
        for (int q = 0; q < m + (int)dn.size(); q++) { const char * s1 = q < m ? nn[q] : dn[(size_t)(q - m)]; if (!s1) continue; std::vector<int> & v = nmuse[s1];
            if (v.empty() || v.back() != i) v.push_back(i); }
    }
    for (int i = 0; i < g->n; i++) { const IR_t * x = g->all[i]; if (x && zls_home_local_kind((int)x->op) && IR_LIT(x).sval) hlk[IR_LIT(x).sval].push_back(i); }
    for (int k = 0; k < nv; k++) { int off = -1; const char * vn = zls_g_vslot_get(g, k, &off); if (!vn || vn[0] == '&' || zst_isparam(g, vn)) continue;
        std::vector<int> uses; { auto ui = nmuse.find(vn); if (ui != nmuse.end()) uses = ui->second; }
        { int past = 0; for (size_t u = 0; (g->spine_cells || g->home_frame) && u < uses.size() && !past; u++) { int lw = zls_home_local_why(nodes[uses[u]]);
              if (lw && !(zd_pl_home(cg) && zst_excursion_why(lw) && off < 0)) past = 1; } if (past) continue; }
        { int hw = 0; auto hi = hlk.find(vn); if (hi != hlk.end()) for (size_t q = 0; q < hi->second.size() && !hw; q++) hw = zls_home_local_why(g->all[hi->second[q]]); if (hw == 9) continue; }
        std::vector<int> roots; for (size_t u = 0; u < uses.size(); u++) { int r = g_planner.zst_root[(size_t)uses[u]]; if (r < 0) continue; int seen = 0;
            for (size_t t = 0; t < roots.size(); t++) if (roots[t] == r) seen = 1; if (!seen) roots.push_back(r); }
        g_planner.zst_vuse[(size_t)k] = (int)roots.size();
        for (size_t t = 0; t < roots.size(); t++) { int r = roots[t]; int nca = -1;
            for (size_t u = 0; u < uses.size(); u++) { int a = uses[u]; if (g_planner.zst_root[(size_t)a] != r || dt.univ[(size_t)a]) continue;
                if (nca < 0) nca = a; else while (nca < n && !zst_dominates(dt, nca, a)) nca = dt.idom[(size_t)nca]; }
            int best = -1, dfirst = 0;
            for (size_t u = 0; u < uses.size() && !dfirst && zls_key_unbacked(g, off); u++) { int a = uses[u];
                if (g_planner.zst_root[(size_t)a] != r || nodes[a]->op != IR_ASSIGN) continue;
                dfirst = 1;
                for (size_t v = 0; v < uses.size() && dfirst; v++) { int x = uses[v];
                    if (x != a && g_planner.zst_root[(size_t)x] == r && !zst_dominates(dt, a, x)) dfirst = 0; } }
            const int bp = g->home_frame ? 2 : 1;
            for (int pass = 0; pass < bp + dfirst && best < 0; pass++) {
                const int fb = pass >= bp;
                auto cand = [&](int d) {
                    if ((loop[(size_t)d] && !(fb || (pass && zd_pl_home(cg)))) || indef0[(size_t)d] || nodes[d]->op == IR_DISJUNCTION || (kown_of(d) <= 0 && !(pass && !fb))) return 0;
                    for (size_t bi = 0; bi < bnds.size(); bi++) {
                        int b = bnds[bi];
                        if (b == d || !zst_dominates(dt, b, d) || esc[bi][(size_t)d]) continue;
                        for (size_t u = 0; u < uses.size(); u++) if (g_planner.zst_root[(size_t)uses[u]] == r && esc[bi][(size_t)uses[u]]) return 0;
                    }
                    return 1;
                };
                if (nca >= 0) { for (int d = nca; d < n && best < 0; d = dt.idom[(size_t)d]) if (cand(d)) best = d; }
                else for (int d = 0; d < n; d++) if (cand(d) && (best < 0 || dt.depth[(size_t)d] > dt.depth[(size_t)best])) best = d;
            }
            if (best < 0) continue;
            g_planner.zst_name.push_back(vn); g_planner.zst_nmap_ok = 0; g_planner.zst_vroot.push_back(r); g_planner.zst_c.push_back(best);
                g_planner.zst_j.push_back(g_planner.zst_s[(size_t)best] / 16);
                g_planner.zst_L.push_back(0);
            g_planner.zst_s[(size_t)best] += 16; g_planner.zst_vcov[(size_t)k]++; } }
    for (int i = 0; i < n; i++) if (g_planner.zst_s[(size_t)i] > 0) g_planner.zst_sm[nodes[i]] = g_planner.zst_s[(size_t)i];
}
}

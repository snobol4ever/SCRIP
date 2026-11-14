#include "planner/zeta_plan/zp_head.h"
extern "C" {
#include "ir/ct_arena.h"
}
extern "C" {
#include "ir/ct_vec.h"
}
extern "C" {
#include "ir/ir_graph.h"
#include "ir/ir_graph_scrip_ir_api.h"
}
extern "C" {
#include "ir/ir_node_index.h"
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
extern "C" {
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
        else g_planner.g_zd_cps = zout[i] - zout[k] + zd_rel_g(cg, nodes[k]);
        return;
    }
    int d = -zcpd[i] - 2, k = zcps[i] <= -2 ? -zcps[i] - 2 : -1;
    int dx = planner_frame_slot(cg, nodes[d]), sx = k >= 0 ? planner_frame_slot(cg, nodes[k]) : -1;
    g_planner.g_zd_cpd = -1;
    g_planner.g_zd_cps = -1;
    if (dx == -1 || k < 0) return;
    g_planner.g_zd_cpdx = dx;
    if (sx != -1) g_planner.g_zd_cpsx = sx;
    else g_planner.g_zd_cps = zout[i] - zout[k] + zd_rel_g(cg, nodes[k]);
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
static int zd_sz_alpha(const char * sz) { return (unsigned char)sz[0] == 0xce && (unsigned char)sz[1] == 0xb1 && sz[2] == 0; }
int zd_dj_anc(const int * zarm, int i, int q) { int g = 0; for (int p = zarm[i]; p >= 0 && g++ < 4096; p = zarm[p]) if (p == q) return 1; return 0; }
extern "C" int planner_env_encloses(const int * zarm, int i, int q) { return (zarm && i >= 0 && q >= 0) ? zd_dj_anc(zarm, i, q) : 0; }
static IR_t * zd_dj_res(IR_t * dj, int j) {
    int na = (int)IR_LIT(dj).ival;
    IR_t * rj = (2 * na + j < dj->n_operands) ? dj->operands[2 * na + j] : (IR_t *)0;
    int g = 0;
    while (rj && rj->op == IR_CONJUNCTION && rj->n_operands > 0 && g++ < 16) rj = rj->operands[0];
    return rj;
}
int zd_match_env_record(const IR_graph_t * cg, const IR_t * nd) { return nd->op == IR_MATCH_ALTERNATE ? 32 : nd->op == IR_MATCH_FENCE1 ? planner_fence_k(cg, nd) : 0; }
void zd_plan(const IR_graph_t * cg, IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, int *zarm, const unsigned char *walls, int *zcps, int *zcpd, int *zchn, int *zclaim) {
    extern const char * bb_src_of(const IR_t *);
    if (g_planner.zd_plan__dg < 0) { const char * d = getenv("SCRIP_ZD_DIAG"); g_planner.zd_plan__dg = (d && *d == '1') ? 1 : 0; }
    g_planner.g_zd_djx.assign((size_t)(n > 0 ? n : 0), 0);
    g_planner.g_zd_admit.assign((size_t)(n > 0 ? n : 1), 0);
    g_planner.g_zd_upost.assign((size_t)(n > 0 ? n : 0), 0);
    g_planner.g_zd_djx_nodes = nodes;
    zd_pdef_scope pds(cg);
    zdo_build(nodes, n);
    zst_plan(cg, nodes, n);
    if (zclaim) for (int i = 0; i < n; i++) zclaim[i] = -1;
    IR_t ** zgt = (IR_t **)alloca(sizeof(IR_t *) * (size_t)n); IR_t ** zot = (IR_t **)alloca(sizeof(IR_t *) * (size_t)n);
    int * zegp = (int *)alloca(sizeof(int) * (size_t)n); unsigned char * zgin = (unsigned char *)alloca((size_t)n); unsigned char * zoin = (unsigned char *)alloca((size_t)n);
        int * zmatch = (int *)alloca(sizeof(int) * (size_t)n);
    int * zstmt = (int *)alloca(sizeof(int) * (size_t)n); for (int _q = 0; _q < n; _q++) zstmt[_q] = -1;
    int * zmafter = (int *)alloca(sizeof(int) * (size_t)n); for (int _q = 0; _q < n; _q++) zmafter[_q] = -1;
    int * zwms = (int *)alloca(sizeof(int) * (size_t)n); for (int _q = 0; _q < n; _q++) zwms[_q] = 0;
    int * zurel = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1)); for (int _q = 0; _q < n; _q++) zurel[_q] = 0;
    for (int i = 0; i < n; i++) { zon[i] = 0; zout[i] = -1; zgpop[i] = 0; zwpop[i] = 0; if (zarm) zarm[i] = -1; zgt[i] = (IR_t *)0; zot[i] = (IR_t *)0; zgin[i] = 0; zoin[i] = 0; zmatch[i] = -1;
        zegp[i] = -1; }
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
    for (int k = 0; k < n; k++) { aent[k] = 0; cm[k] = 0; }
    std::vector<int> aent_t, cm_t;
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
    for (int k = 0; k < n; k++) if (zd_edge_retains_g(cg, nodes[k])) { int q = nidx(nodes, n, zd_chase(nodes[k]->γ.node));
        if (q >= 0 && q != k && nodes[q]->op != IR_DISJUNCTION) rjoin[q] = 1;
        else if (q >= 0 && q != k && cg && cg->home_frame && planner_resumable_frame(cg) && !zd_pl_dj_join(nodes, n, k, q) && zd_pl_dj_gate_end(nodes, n, k, q)) { int q2 =
            nidx(nodes, n, zd_chase(nodes[q]->γ.node)); if (q2 >= 0 && q2 != k && nodes[q2]->op != IR_DISJUNCTION) rjoin[q2] = 1; } }
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
            { IR_t * tw = nodes[ci]->ω.node; int ti = (rl > 0 && tw && tw->op == IR_MATCH_TRAP) ? nidx(nodes, n, tw) : -1;
              if (ti >= 0 && claim[ti] < 0) { run[rl] = ti; rpos[ti] = rl; claim[ti] = hi; rl++; } }
            run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; rl++;
            if (zd_pl_cut(cg, nodes[ci])) break;
            if (zd_edge_retains_g(cg, nodes[ci])) break;
            cur =zd_icn_suspend_g(cg, cur) ? zd_suspend_run_next(nodes, n, cur, claim) : zd_chase(cur->γ.node);
        }
        { for (int k : aent_t) aent[k] = 0;
          aent_t.clear();
          if (zarm) {
            for (int pass = 0; pass < 2; pass++) for (int r0 = 0; r0 < rl; r0++) { IR_t * env = nodes[run[r0]];
                if ((int)env->op != IR_MATCH_ALTERNATE && (int)env->op != IR_MATCH_ARBNO && (int)env->op != IR_MATCH_FENCE1) continue;
                int ei = run[r0];
                for (int a = 0; a + 1 < env->n_operands; a += 2) { if (pass == 0 && a >= 2 && (int)env->op == IR_MATCH_ARBNO) continue; IR_t * c2 = env->operands[a]; int rl0 = rl; int bad = 0;
                    int g2 = 0; int first = 1;
                    while (c2 && g2++ <= n) {
                        int ci = -1; { int k = nidx(nodes, n, c2); if (k >= 0) { ci = k; } }
                        if (ci < 0 || claim[ci] >= 0) break;
                        if (bb_src_of(nodes[ci]) || ir_floater_kind(nodes[ci])) { bad = 1; break; }
                        { IR_t * tw = nodes[ci]->ω.node; int ti = (tw && tw->op == IR_MATCH_TRAP) ? nidx(nodes, n, tw) : -1;
                            if (ti >= 0 && claim[ti] < 0) { run[rl] = ti; rpos[ti] = rl; claim[ti] = hi; zarm[ti] = ei; if (first) { aent[ti] = 1; aent_t.push_back(ti); first = 0; } rl++; } }
                        run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; zarm[ci] = ei; if (first) { aent[ci] = 1; aent_t.push_back(ci); first = 0; } rl++;
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
                        run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; zarm[ci] = ei; zarmj[ci] = a; aent[ci] = (unsigned char)first; if (first) aent_t.push_back(ci); first = 0; rl++;
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
                    dep += zd_kf_g(cg, nodes[i]);
                    if (zd_chase(nodes[i]->γ.node) == nodes[d] && zd_sz_sigma(zd_edge_sz(nodes[i], 0)) && zls_dj_arm_resumes(nodes[d], zarmj[i]) && dep > x) x = dep;
                }
                if (cst && x > 0) g_planner.g_zd_djx[(size_t)d] = x;
                if (g_planner.zd_plan__dg) fprintf(stderr, "[ZD-DJX] i=%d exit depth %d%s\n", d, x, cst ? "" : " (an arm's depth is not a constant: the record road)");
            }
        }
        for (int k : cm_t) cm[k] = 0;
        cm_t.clear();
        g_planner.g_zd_zarm_p = zarm; g_planner.g_zd_zarm_nodes = nodes; g_planner.g_zd_zarm_n = n;
        int nblob = 0;
        { int wn = 0;
          for (int r = 0; r < rl; r++) { cm[run[r]] = 1; cm_t.push_back(run[r]); wl[wn++] = run[r]; }
          while (wn > 0) { int i = wl[--wn];
              for (int a = 0; a < nodes[i]->n_operands; a++) { IR_t * p = nodes[i]->operands[a]; if (!p) continue;
                  { int k = nidx(nodes, n, p); if (k >= 0 && (!cm[k])) { cm[k] = 1; cm_t.push_back(k); wl[wn++] = k; nblob++; } } } } }
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
                int REL = planner_fence0_release(cg, nodes[i]);
                int K = zd_k_g(cg, nodes[i]);
                int RG = zd_rel_g(cg, nodes[i]);
                int UREL = 0;
                { int _ub = zd_pl_unmark_bound(cg, nodes, n, i);
                    if (_ub >= 0 && claim[_ub] == hi && rpos[_ub] >= 0 && rpos[_ub] < r) UREL = (int)zd + K - (zout[_ub] - (zd_pl_bspine(cg, nodes[_ub]) ? zd_bound_pop(cg, nodes[_ub]) : 0));
                    else if (_ub >= 0 && claim[_ub] != hi && zd_pl_bspine(cg, nodes[_ub])) UREL = zd_bound_pop(cg, nodes[_ub]); }
                if (zd_pl_cut(cg, nodes[i])) UREL = (int)zd + K;
                zurel[i] = UREL;
                { if (nodes[i]->op == IR_MATCH_BEGIN && zdh_match < 0) zdh_match = (int)zd + K - RG - zd_mframe_g(cg, nodes[i]); } { if (nodes[i]->op == IR_STATEMENT_END) zdh_stmt = (int)zd + K - RG;
                    }
                { if (nodes[i]->op == IR_STATEMENT_BEGIN && r > 0 && zd > 0 && !(zarm && zarm[i] >= 0) ) { int rel =
                    zd_exit_pop_s(IR_STATEMENT_END, zdh_match, (int)zd, zdh_stmt, zdh_mafter); if (rel > 0) zgpop[run[r - 1]] += rel; zd = 0; } }
                { if (nodes[i]->op == IR_STATEMENT_BEGIN && r > 0) { zdh_match = -1; zdh_stmt = -1; zdh_mafter = -1; zwt = 0; } }
                { if (nodes[i]->op == IR_MATCH_BEGIN && zwt < n) zwms[zwt++] = (int)zd + K - RG - zd_mframe_g(cg, nodes[i]); }
                { if (nodes[i]->op == IR_MATCH_REPLACE || nodes[i]->op == IR_MATCH_END) zdh_mafter = (int)zd + K - RG; }
                if (zarm && zarm[i] >= 0) { if (aent[i]) arm_zd = zout[zarm[i]] - (nodes[zarm[i]]->op == IR_DISJUNCTION ? zd_djx_of(nodes[zarm[i]]) : zd_k_g(cg, nodes[zarm[i]])); zon[i] = 1;
                    if (nodes[i]->op == IR_UNMARK && (IR_LIT(nodes[i]).ival & 16) && nodes[i]->n_operands > 0 && nodes[i]->operands[0] && nodes[i]->operands[0]->op == IR_BOUND) {
                        int _bk = nidx(nodes, n, nodes[i]->operands[0]);
                        if (_bk >= 0 && claim[_bk] == hi && zarm[_bk] == zarm[i] && rpos[_bk] >= 0 && rpos[_bk] < r) arm_zd = zout[_bk] - zd_k_g(cg, nodes[_bk]);
                    }
                    zout[i] = arm_zd + K;
                    arm_zd = arm_zd + K - RG; }
                else { zon[i] = 1; zout[i] = zd + K - REL; zd = zd + K - REL - UREL - RG; }
                { if (zwt == 0 && nodes[i]->op == IR_MATCH_REPLACE) { int _zu = 0, _me = 0; for (int _rr = r - 1; _rr >= 0; _rr--) { int _j = run[_rr];
                    if (nodes[_j]->op == IR_MATCH_END) { _me = 1; break; } _zu += zd_kf_g(cg, nodes[_j]); } if (_me) { zd -= _zu; zdh_mafter = (int)zd; } } }
                { if (zwt > 0 && (nodes[i]->op == IR_MATCH_REPLACE || nodes[i]->op == IR_MATCH_END)) { int _zx = zout[i]; zd = zwms[--zwt]; zdh_match = (int)zd; zdh_mafter = (int)zd;
                    if (nodes[i]->op == IR_MATCH_END) zegp[i] = _zx - (int)zd; } }
                IR_t * gt = zd_chase(nodes[i]->γ.node); IR_t * ot = zd_chase(nodes[i]->ω.node);
                int gin = 0; int oin = 0; int gback = -1; int oback = -1;
                int gib = port_sz_beta(nodes[i]->γ.sz); { int _gg = 0; IR_t * _g = nodes[i]->γ.node; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!gib) gib = port_sz_beta(_g->γ.sz);
                    _g = _g->γ.node; } } gib = gib && !beta_is_stmt_land(gt);
                if (nblob > 0) { for (int k : cm_t) if (cm[k]) { int _bk = claim[k] == hi && rpos[k] >= 0 && rpos[k] <= r; if (nodes[k] == gt && (!_bk || gib)) gin = 1;
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
                int _wzdepth = (zarm && zarm[i] >= 0) ? zout[i] : (int)zd + RG;
                int _gbpre = (gback >= 0) ? (zout[gback] - (zd_g_into_glue_beta(nodes[i], nodes[gback]) ? 0 : zd_k_g(cg, nodes[gback]))) : 0;
                int _obpre = (oback >= 0) ? (zout[oback] - zd_k_g(cg, nodes[oback])) : 0;
                {
                if (!gin) zgpop[i] = (planner_resumable_frame(cg) && !(gback >= 0 && claim[gback] == claim[i])) ? 0 : (gback >= 0) ? (_wzdepth - _gbpre) :
                    (zd_exit_pop_s(nodes[i]->op, zdh_match, _wzdepth, zdh_stmt, zdh_mafter) + (int)kc);
                if (gin && nodes[i]->op != IR_MATCH_END && nodes[i]->op != IR_MATCH_REPLACE) zgpop[i] += RG;
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
        int _gbpre = (gback >= 0) ? (zout[gback] - (zd_g_into_glue_beta(nodes[i], nodes[gback]) ? 0 : zd_k_g(cg, nodes[gback]))) : 0;
        int _obpre = (oback >= 0) ? (zout[oback] - zd_k_g(cg, nodes[oback])) : 0;
        if (!zgin[i]) zgpop[i] = (planner_resumable_frame(cg) && !(gback >= 0 && claim[gback] == claim[i])) ? 0 : (gback >= 0 && _gbpre > 0) ? (zout[i] - zurel[i] - _gbpre) :
            zd_exit_pop_s(nodes[i]->op, zmatch[i], zout[i] - zurel[i], zstmt[i], zmafter[i]);
        if (zegp[i] >= 0) zgpop[i] = zegp[i];
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
        zd_pl_bases(cg, nodes, n, zon, zout, zgpop, zwpop, claim, base, known, aof, (int *)0, fam, fbase, (const unsigned char *)0);
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
                if (ki == 2 && !v && b && zd_edge_retains_g(cg, nodes[q])) {
                    int ql, qb;
                    (void) zd_pl_land(cg, nodes, n, zon, zout, zgpop, zwpop, q, 0, &ql, &qb);
                    if (ql >= 0 && zon[ql] && claim[ql] >= 0 && claim[ql] == fam[ci]) {
                        zgpop[i] = ai - (fbase[fam[ci]] + zout[ql] - (qb ? 0 : zd_k_g(cg, nodes[ql]))) - (nodes[q]->op == IR_TO ? zgpop[q] : 0);
                        continue;
                    }
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
                if (v) zwpop[i] = ai - zd_k_g(cg, nodes[i]) - aq; else zgpop[i] = zd_pl_unmark_exit(cg, nodes, n, zon, zout, claim, known, base, fam, fbase, i, ai) - aq;
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
            int rd = known[cd] ? -1 : zd_pl_resume_depth(cg, nodes, n, zon, zout, claim, base, known, dq);
            if (known[ci] && rd >= 0) zgpop[i] = base[ci] + zout[i] - rd;
            else if (known[ci] && known[cd]) zgpop[i] = base[ci] + zout[i] - (base[cd] + zout[dq]);
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
}

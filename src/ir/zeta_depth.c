#include "zeta_depth.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern long zw_node_k(const IR_t * nd);
extern long zw_carve_k(const IR_t * nd);
extern int  zls_result_live(const IR_t * nd);
#define ZDP_CAP 8192
static const IR_t * zdp_key[ZDP_CAP]; static int zdp_ina[ZDP_CAP]; static int zdp_inb[ZDP_CAP]; static int zdp_ret[ZDP_CAP]; static int zdp_wy[ZDP_CAP]; static int zdp_src[ZDP_CAP]; static int zdp_used = 0; static int zdp_valid = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zdp_mode(void) { static int m = -1; if (m < 0) { const char * e = getenv("SCRIP_ZDP"); m = (e && *e && *e != '0') ? atoi(e) : 0; } return m; }
static unsigned zdp_h(const IR_t * p) { unsigned long long v = (unsigned long long)(size_t)p; v ^= v >> 33; v *= 0xff51afd7ed558ccdULL; v ^= v >> 33; return (unsigned)(v & (ZDP_CAP - 1)); }
static int zdp_slot(const IR_t * nd, int create) { if (!nd) return -1; unsigned i = zdp_h(nd); for (int probe = 0; probe < ZDP_CAP; probe++) { unsigned j = (i + (unsigned)probe) & (ZDP_CAP - 1); if (zdp_key[j] == nd) return (int)j; if (!zdp_key[j]) { if (!create) return -1; zdp_key[j] = nd; zdp_ina[j] = ZDP_BOT; zdp_inb[j] = ZDP_BOT; zdp_ret[j] = ZDP_BOT; zdp_wy[j] = ZDP_UNREACHED; zdp_src[j] = 0; zdp_used++; return (int)j; } } return -1; }
static int zdp_meet(int a, int b) { if (a == ZDP_BOT) return b; if (b == ZDP_BOT) return a; if (a == ZDP_TOP || b == ZDP_TOP) return ZDP_TOP; return (a == b) ? a : ZDP_TOP; }
static int zdp_carve_dynamic(const IR_t * nd) { if (!nd) return 0; switch (nd->op) { case IR_STATEMENT: case IR_BOUND: case IR_UNMARK: return 1; default: return 0; } }
int zdp_scratch_cell(const IR_t * nd) { if (!nd) return 0; switch (nd->op) { case IR_MATCH_SPAN: case IR_MATCH_BREAK: case IR_MATCH_BREAKX: case IR_MATCH_TAB: case IR_MATCH_RTAB: case IR_MATCH_REM: case IR_MATCH_ARB: case IR_MATCH_BAL: return 1; default: return 0; } }
int zdp_scan_pure(const IR_t * nd) { if (!nd) return 0; switch (nd->op) { case IR_MATCH_LIT: case IR_MATCH_LEN: case IR_MATCH_ANY: case IR_MATCH_NOTANY: case IR_MATCH_POS: case IR_MATCH_RPOS: case IR_MATCH_TAB: case IR_MATCH_RTAB: case IR_MATCH_SPAN: case IR_MATCH_BREAK: case IR_MATCH_REM: return 1; default: return 0; } }
static int zdp_cap_alttier(void) { const char * e = getenv("SCRIP_ALT_SEAM_TIER"); return (e && *e == '0') ? 0 : 1; }
static int zdp_cap_seamtier(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_CAP_SEAMTIER"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int zdp_atp_seamtier(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ATP_SEAMTIER"); v = (e && *e == '0') ? 0 : 1; } return v; }
int zdp_seam_tier(const IR_t * nd) { if (!nd) return 0; switch (nd->op) { case IR_MATCH_ARBNO: case IR_MATCH_ARB: case IR_MATCH_BAL: return 1; case IR_MATCH_DEFER: case IR_MATCH_LEN: case IR_MATCH_POS: case IR_MATCH_RPOS: case IR_MATCH_TAB: case IR_MATCH_RTAB: case IR_MATCH_LIT: case IR_MATCH_ANY: case IR_MATCH_NOTANY: case IR_MATCH_SPAN: case IR_MATCH_SPAN_VAR: case IR_MATCH_BREAK: case IR_MATCH_REM: return 2; case IR_MATCH_ALTERNATE: return zdp_cap_alttier() ? 3 : 0; case IR_MATCH_ASSIGN_COND: case IR_MATCH_ASSIGN_IMM: case IR_MATCH_ASSIGN_SAVE: return zdp_cap_seamtier() ? 2 : 0; case IR_MATCH_ATP: return zdp_atp_seamtier() ? 2 : 0; default: return 0; } }
static long zdp_carve(const IR_t * nd) { return nd ? zw_carve_k(nd) : 0L; }
static int zdp_out_gamma(const IR_t * nd, int in) { if (in == ZDP_TOP) return ZDP_TOP; if (zdp_carve_dynamic(nd)) return ZDP_TOP; return zls_result_live(nd) ? in + (int)zdp_carve(nd) : in; }
static int zdp_out_omega(const IR_t * nd, int in) { if (in == ZDP_TOP) return ZDP_TOP; if (zdp_carve_dynamic(nd)) return ZDP_TOP; return in; }
static int zdp_disagree(int a, int b) { return (a != ZDP_BOT && b != ZDP_BOT && a != ZDP_TOP && b != ZDP_TOP && a != b); }
static void zdp_push(const IR_t ** wl, int * wn, const IR_t * nd) { if (*wn < ZDP_CAP) wl[(*wn)++] = nd; }
static void zdp_edge(int t, int val, int is_beta, const IR_t ** wl, int * wn, const IR_t * succ) { if (t < 0) return; int * cell = is_beta ? &zdp_inb[t] : &zdp_ina[t]; if (zdp_disagree(*cell, val)) zdp_src[t] = 1; int nv = zdp_meet(*cell, val); if (nv != *cell) { *cell = nv; zdp_push(wl, wn, succ); } }
static int zdp_tag(const char * sz) { if (sz[0] == (char)0xce && (unsigned char)sz[1] == 0xb2) return 1; if (sz[0] == (char)0xcf && (unsigned char)sz[1] == 0x83) return 2; if (sz[0] == (char)0xcf && (unsigned char)sz[1] == 0x86) return 3; return 0; }
static const IR_t * zdp_chase(const IR_t * t, int * tag) { int g = 0; while (t && t->op == IR_GOTO && g++ < 128) { int nt = zdp_tag(t->γ.sz); if (nt && !*tag) *tag = nt; t = t->γ.node; } return t; }
static int zdp_beta_is_stmt_land(const IR_t * tgt) { return tgt && tgt->op == IR_STATEMENT_BEGIN; }
static int zdp_land_value(const IR_t * tgt, int v) { (void)tgt; return v; }
static int zdp_has_pairs(const IR_t * nd) { if (!nd || nd->n_operands <= 0) return 0; switch (nd->op) { case IR_MATCH_ALTERNATE: case IR_MATCH_ARBNO: case IR_MATCH_FENCE1: case IR_SCAN_SEQUENCE: case IR_SCAN_ALTERNATE: case IR_DISJUNCTION: return 1; default: return 0; } }
static int zdp_pairs_n(const IR_t * nd) { int N = (nd->op == IR_MATCH_ARBNO) ? 1 : (nd->op == IR_DISJUNCTION) ? (int)IR_LIT(nd).ival : nd->n_operands / 2; if (N < 0) N = 0; if (2 * N > nd->n_operands) N = nd->n_operands / 2; return N; }
static int zdp_inside_of(const IR_t * nd, int in) { if (in == ZDP_TOP) return ZDP_TOP; if (in == ZDP_BOT) return ZDP_BOT; if (zdp_carve_dynamic(nd)) return ZDP_TOP; return in + (int)zdp_carve(nd); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zdp_analyze(IR_graph_t * g) {
    if (!g || !g->entry || g->n <= 0) return 0;
    memset(zdp_key, 0, sizeof(zdp_key)); zdp_used = 0; zdp_valid = 0;
    for (int i = 0; i < g->n; i++) (void)zdp_slot(g->all[i], 1);
    int es = zdp_slot(g->entry, 1); if (es < 0) return 0;
    zdp_ina[es] = 0;
    const IR_t * wl[ZDP_CAP]; int wn = 0; zdp_push(wl, &wn, g->entry); int guard = 0;
    while (wn > 0 && guard++ < ZDP_CAP * 16) {
        const IR_t * nd = wl[--wn]; int s = zdp_slot(nd, 0); if (s < 0) continue;
        int gtag = zdp_tag(nd->γ.sz); const IR_t * gtgt = zdp_chase(nd->γ.node, &gtag);
        int otag = zdp_tag(nd->ω.sz); const IR_t * otgt = zdp_chase(nd->ω.node, &otag);
        int omega_is_retry = (otag == 1) && !zdp_beta_is_stmt_land(otgt);
        int ports[2]; ports[0] = zdp_ina[s]; ports[1] = zdp_inb[s];
        for (int pi = 0; pi < 2; pi++) {
            int in = ports[pi]; if (in == ZDP_BOT) continue;
            if (gtgt) {
                int gv = zdp_out_gamma(nd, in);
                if (gtag == 2 || gtag == 3) { int t = zdp_slot(gtgt, 1); if (t >= 0) { if (zdp_disagree(zdp_ret[t], gv)) zdp_src[t] = 1; int nv = zdp_meet(zdp_ret[t], gv); if (nv != zdp_ret[t]) { zdp_ret[t] = nv; zdp_push(wl, &wn, gtgt); } } }
                else zdp_edge(zdp_slot(gtgt, 1), zdp_land_value(gtgt, gv), 0, wl, &wn, gtgt);
            }
            if (otgt) zdp_edge(zdp_slot(otgt, 1), zdp_land_value(otgt, zdp_out_omega(nd, in)), omega_is_retry, wl, &wn, otgt);
        }
        if (zdp_has_pairs(nd)) {
            int in = (zdp_ina[s] != ZDP_BOT) ? zdp_ina[s] : zdp_inb[s];
            if (in != ZDP_BOT) {
                int inside = zdp_inside_of(nd, in); int N = zdp_pairs_n(nd);
                for (int j = 0; j < N; j++) {
                    const IR_t * e = nd->operands[2 * j]; const IR_t * r = nd->operands[2 * j + 1];
                    if (e && e->op != IR_FAIL && e != nd) zdp_edge(zdp_slot(e, 1), inside, 0, wl, &wn, e);
                    if (r && r->op != IR_FAIL && r != nd) zdp_edge(zdp_slot(r, 1), inside, 1, wl, &wn, r);
                }
            }
        }
    }
    for (int i = 0; i < g->n; i++) {
        int s = zdp_slot(g->all[i], 0); if (s < 0) continue; const IR_t * nd = g->all[i];
        int inside = zdp_inside_of(nd, zdp_ina[s]);
        if (zdp_ina[s] == ZDP_BOT && zdp_inb[s] == ZDP_BOT) zdp_wy[s] = ZDP_UNREACHED;
        else if (zdp_carve_dynamic(nd)) zdp_wy[s] = ZDP_DYN;
        else if (zdp_ret[s] != ZDP_BOT && inside != ZDP_BOT && inside != ZDP_TOP && zdp_ret[s] != inside) zdp_wy[s] = ZDP_ALTRET;
        else if (zdp_inb[s] != ZDP_BOT && zdp_ina[s] != ZDP_BOT && zdp_inb[s] != zdp_ina[s]) zdp_wy[s] = ZDP_RESUME;
        else if (zdp_src[s]) zdp_wy[s] = ZDP_JOIN;
        else zdp_wy[s] = ZDP_OK;
    }
    zdp_valid = 1; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zdp_alpha(const IR_t * nd) { int s = zdp_valid ? zdp_slot(nd, 0) : -1; return (s < 0) ? ZDP_TOP : zdp_ina[s]; }
int zdp_beta(const IR_t * nd) { int s = zdp_valid ? zdp_slot(nd, 0) : -1; return (s < 0) ? ZDP_TOP : zdp_inb[s]; }
int zdp_inside(const IR_t * nd) { int a = zdp_alpha(nd); return zdp_inside_of(nd, a); }
int zdp_known_alpha(const IR_t * nd) { int d = zdp_alpha(nd); return (d != ZDP_TOP && d != ZDP_BOT); }
int zdp_known_beta(const IR_t * nd) { int d = zdp_beta(nd); return (d != ZDP_TOP && d != ZDP_BOT); }
int zdp_resume_sound(const IR_t * nd) { int a = zdp_alpha(nd); int b = zdp_beta(nd); if (b == ZDP_BOT) return 1; if (a == ZDP_TOP || b == ZDP_TOP) return 0; return a == b; }
int zdp_why(const IR_t * nd) { int s = zdp_valid ? zdp_slot(nd, 0) : -1; return (s < 0) ? ZDP_UNREACHED : zdp_wy[s]; }
static int zdp_match_lifetime(const IR_t * nd) { if (!nd) return 0; switch (nd->op) { case IR_MATCH_ASSIGN_SAVE: case IR_MATCH_ASSIGN_COND: case IR_MATCH_ASSIGN_IMM: case IR_MATCH_BEGIN: return 1; default: return 0; } }
int zdp_tier(const IR_t * nd) { if (zdp_match_lifetime(nd)) return ZDP_TIER_STANDING; if (!zdp_known_alpha(nd)) return ZDP_TIER_ACTIVATION; if (!zdp_resume_sound(nd)) return ZDP_TIER_ACTIVATION; return ZDP_TIER_SPINE; }
static const char * zdp_wname(int w) { switch (w) { case ZDP_OK: return "OK"; case ZDP_UNREACHED: return "UNREACHED"; case ZDP_JOIN: return "TOP-JOIN"; case ZDP_DYN: return "TOP-DYN"; case ZDP_RESUME: return "RESUME-SKEW"; case ZDP_ALTRET: return "ALT-RETURN"; default: return "?"; } }
static int zdp_teardown_owner(const IR_t * nd) { if (!nd) return 0; switch (nd->op) { case IR_MATCH_BEGIN: case IR_MATCH_DEFER: case IR_MATCH_ARBNO: return 1; default: return 0; } }
static int zdp_guaranteed_meet(const IR_t * nd) { if (!nd) return 0; switch (nd->op) { case IR_MATCH_BEGIN: case IR_STATEMENT_BEGIN: case IR_STATEMENT_END: return 1; default: return 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zdp_bomb_census(IR_graph_t * g, const char * tag) {
    if (!g) return; static int bm = -1; if (bm < 0) { const char * e = getenv("SCRIP_ZDP_BOMB"); bm = (e && *e && *e != '0') ? 1 : 0; } if (!bm) return;
    for (int i = 0; i < g->n; i++) { const IR_t * nd = g->all[i]; int td = zdp_teardown_owner(nd); if (!zdp_guaranteed_meet(nd) && !td) continue; int s = zdp_slot(nd, 0); if (s < 0) continue;
        if (zdp_ina[s] == ZDP_BOT && zdp_inb[s] == ZDP_BOT) continue;
        if (td && zdp_inb[s] != ZDP_BOT) { int a = zdp_ina[s], b = zdp_inb[s]; if (a == ZDP_TOP || b == ZDP_TOP || a != b) { fprintf(stderr, "[ZDPBOMB] %s op=%s TEARDOWN-SKEW\n", tag ? tag : "graph", bb_op_name(nd->op)); continue; } }
        const char * why = zdp_src[s] ? "SOURCE-DISAGREE" : (zdp_inb[s] != ZDP_BOT && zdp_ina[s] != ZDP_BOT && zdp_ina[s] != ZDP_TOP && zdp_inb[s] != ZDP_TOP && zdp_ina[s] != zdp_inb[s]) ? "ALPHA-NE-BETA" : (const char *)0;
        if (!why) continue;
        fprintf(stderr, "[ZDPBOMB] %s op=%s %s\n", tag ? tag : "graph", bb_op_name(nd->op), why);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zdp_unwind_census(IR_graph_t * g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) { const IR_t * b = g->all[i]; if (!b || b->op != IR_MATCH_BEGIN) continue; int bs = zdp_slot(b, 0); if (bs < 0) continue;
        int base = zdp_ina[bs]; int inside = zdp_inside_of(b, base);
        for (int j = 0; j < g->n; j++) { const IR_t * nd = g->all[j]; if (!nd) continue; int s = zdp_slot(nd, 0); if (s < 0) continue;
            int otag = zdp_tag(nd->ω.sz); const IR_t * otgt = zdp_chase(nd->ω.node, &otag); if (otgt != b) continue; if (otag != 1 || zdp_beta_is_stmt_land(otgt)) continue;
            int in = (zdp_ina[s] != ZDP_BOT) ? zdp_ina[s] : zdp_inb[s]; if (in == ZDP_BOT) continue; int v = zdp_out_omega(nd, in);
            if (v == base || v == inside) continue;
            fprintf(stderr, "[ZDPUNW] src=%s arrives=%s base=%s inside=%s carve=%ld\n", bb_op_name(nd->op), (v == ZDP_TOP) ? "TOP" : "off-base", (base == ZDP_TOP) ? "TOP" : "int", (inside == ZDP_TOP) ? "TOP" : "int", zdp_carve(nd));
        } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zdp_report(IR_graph_t * g, const char * tag) {
    if (!zdp_mode() || !g) return; if (!zdp_analyze(g)) return;
    int ok = 0, un = 0, tj = 0, td = 0, rs = 0, ar = 0, sp = 0, ac = 0, st = 0;
    for (int i = 0; i < g->n; i++) { int w = zdp_why(g->all[i]);
        switch (w) { case ZDP_OK: ok++; break; case ZDP_UNREACHED: un++; break; case ZDP_JOIN: tj++; break; case ZDP_DYN: td++; break; case ZDP_RESUME: rs++; break; case ZDP_ALTRET: ar++; break; default: break; }
        if (w == ZDP_UNREACHED) continue;
        switch (zdp_tier(g->all[i])) { case ZDP_TIER_SPINE: sp++; break; case ZDP_TIER_ACTIVATION: ac++; break; case ZDP_TIER_STANDING: st++; break; default: break; } }
    fprintf(stderr, "[ZDP] %s n=%d ok=%d top_join=%d top_dyn=%d resume_skew=%d alt_return=%d unreached=%d | TIER spine=%d activation=%d standing=%d\n", tag ? tag : "graph", g->n, ok, tj, td, rs, ar, un, sp, ac, st);
    zdp_bomb_census(g, tag);
    if (zdp_mode() < 2) return;
    zdp_unwind_census(g);
    { static const char * tn[3] = { "SPINE", "ACTIVATION", "STANDING" };
      for (int i = 0; i < g->n; i++) { int w = zdp_why(g->all[i]); if (w == ZDP_OK || w == ZDP_UNREACHED) continue; int a = zdp_alpha(g->all[i]); int b = zdp_beta(g->all[i]);
        fprintf(stderr, "[ZDP]   node#%d op=%s %s tier=%s alpha=%s beta=%s\n", i, bb_op_name(g->all[i]->op), zdp_wname(w), tn[zdp_tier(g->all[i])], (a == ZDP_TOP) ? "TOP" : (a == ZDP_BOT) ? "BOT" : "int", (b == ZDP_TOP) ? "TOP" : (b == ZDP_BOT) ? "BOT" : "int"); } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void zdp_port_census(IR_graph_t * g) { if (zdp_mode() < 3 || !g) return; for (int i = 0; i < g->n; i++) { const IR_t * nd = g->all[i]; if (!nd) continue; int gt = zdp_tag(nd->γ.sz); const IR_t * gtgt = zdp_chase(nd->γ.node, &gt); int ot = zdp_tag(nd->ω.sz); const IR_t * otgt = zdp_chase(nd->ω.node, &ot);
        if (gtgt) fprintf(stderr, "[ZDPORT] src=%s port=gamma lands=%s tgt=%s self=%d\n", bb_op_name(nd->op), (gt == 1) ? "beta" : (gt == 2 || gt == 3) ? "returns-into" : "alpha", bb_op_name(gtgt->op), (nd == gtgt));
        if (otgt) fprintf(stderr, "[ZDPORT] src=%s port=omega lands=%s tgt=%s\n", bb_op_name(nd->op), ((ot == 1) && !zdp_beta_is_stmt_land(otgt)) ? "beta" : "alpha", bb_op_name(otgt->op));
        if (zdp_has_pairs(nd)) { int N = zdp_pairs_n(nd); for (int j = 0; j < N; j++) { const IR_t * e = nd->operands[2 * j]; const IR_t * r = nd->operands[2 * j + 1];
            if (e && e->op != IR_FAIL && e != nd) fprintf(stderr, "[ZDPORT] src=%s port=operand lands=alpha tgt=%s\n", bb_op_name(nd->op), bb_op_name(e->op));
            if (r && r->op != IR_FAIL && r != nd) fprintf(stderr, "[ZDPORT] src=%s port=operand lands=beta tgt=%s\n", bb_op_name(nd->op), bb_op_name(r->op)); } } } }
static int  zzone_tier[ZDP_CAP]; static int zzone_off[ZDP_CAP]; static int zzone_valid = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zzone_plan(IR_graph_t * g) {
    if (!g || g->n <= 0) return 0;
    zzone_valid = 0;
    for (int i = 0; i < g->n; i++) {
        const IR_t * nd = g->all[i]; int s = zdp_slot(nd, 0); if (s < 0) continue;
        zzone_off[s] = -1;
        zzone_tier[s] = zdp_tier(nd);
    }
    zzone_valid = 1; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zzone_tier_of(const IR_t * nd) { int s = zzone_valid ? zdp_slot(nd, 0) : -1; return (s < 0) ? ZDP_TIER_SPINE : zzone_tier[s]; }
int zzone_off_of(const IR_t * nd) { int s = zzone_valid ? zdp_slot(nd, 0) : -1; return (s < 0) ? -1 : zzone_off[s]; }

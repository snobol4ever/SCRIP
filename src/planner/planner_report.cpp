#include <climits>
#include "planner_head.h"
#include "planner.h"
}
extern "C" int zls_g_resumable_frame(const IR_graph_t * g);
extern "C" int zls_g_keys_only(const IR_graph_t * g);
extern "C" int zls_home_why(const IR_t * nd);
extern "C" int zls_home_local_test(const IR_graph_t * g, const char * vn);
extern "C" int zls_g_hf_resident(const IR_graph_t * g, const IR_t * nd);
extern "C" int zd_dj_on_spine(const IR_t * nd);
extern "C" const char * zls_home_why_name(int w);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dh_callee_graph(const IR_t * nd) {
    if (!ir_is_call_kind(ir_kind0(nd)) || ir_kind0(nd) == IR_CALL_VALUE || !IR_LIT(nd).sval) return -1;
    auto it = g_planner.dh_gidx.find(IR_LIT(nd).sval);
    return it == g_planner.dh_gidx.end() ? -1 : it->second;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * dh_box_dyn(const IR_graph_t * g, const IR_t * nd) {
    switch ((int)ir_kind0(nd)) {
        case IR_SUSPEND:
        return "suspend";
        case IR_CREATE:
        case IR_ACTIVATE:
        case IR_CORET:
        case IR_COFAIL:
        return "coexpression";
        case IR_CALL_VALUE:
        return "unknown-callee";
        case IR_CALL_GEN:
        case IR_PROC_GEN:
        return "generator-callee";
        default:
        break;
    }
    int h = dh_callee_graph(nd);
    if (h < 0 && ir_kind0(nd) == IR_CALL_PROC_STAGED && (g && zls_g_resumable_frame(g))) return "unresolved-callee";
    return (h >= 0 && g_planner.dh_rt[h]) ? "retaining-callee" : (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void dh_retains_build(void) {
    extern const char * zls_graph_name_get(const IR_graph_t *);
    int G = g_stage2.bbp.count;
    if ((int)g_planner.dh_rt.size() == G) return;
    g_planner.dh_gidx.clear();
    for (int k = 0; k < G; k++) { const char * nm = g_stage2.bbp.table[k] ? zls_graph_name_get(g_stage2.bbp.table[k]) : (const char *)0; if (nm) g_planner.dh_gidx.emplace(nm, k); }
    g_planner.dh_rt.assign((size_t)G, 0);
    for (int k = 0; k < G; k++) {
        const IR_graph_t * g = g_stage2.bbp.table[k];
        int nsucc = 0;
        for (int i = 0; g && i < g->n; i++) {
            const IR_t * nd = g->all[i];
            if (!nd) continue;
            if (nd->op == IR_SUCCEED) nsucc++;
            int o = (int)ir_kind0(nd);
            if (o == IR_SUSPEND || o == IR_CREATE || o == IR_ACTIVATE || o == IR_CORET || o == IR_COFAIL || o == IR_CALL_VALUE || o == IR_CALL_GEN || o == IR_PROC_GEN) g_planner.dh_rt[k] = 1;
        }
        if (nsucc > 1) g_planner.dh_rt[k] = 1;
    }
    for (int changed = 1; changed;) {
        changed = 0;
        for (int k = 0; k < G; k++) {
            const IR_graph_t * g = g_stage2.bbp.table[k];
            for (int i = 0; g && !g_planner.dh_rt[k] && i < g->n; i++) {
                int h = g->all[i] ? dh_callee_graph(g->all[i]) : -1;
                if (h >= 0 && g_planner.dh_rt[h]) { g_planner.dh_rt[k] = 1; changed = 1; }
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * xop_frame_why(const IR_t * nd) {
    return zls_dj_retains(nd) ? "retaining-disjunction" : nd->op == IR_TO ? "generator-state" : "read-across-retaining-edge";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_zrow_gix(const IR_t * nd);
extern "C" int emit_zrow_gixr(const IR_t * nd);
void planner_dump_homes(const IR_graph_t * g, IR_t ** nodes, int n, const unsigned char * zon, const int * eslot) {
    extern const char * zls_graph_name_get(const IR_graph_t *); extern int zls_g_vslot_count(const IR_graph_t *); extern const char * zls_g_vslot_get(const IR_graph_t *, int, int *);
    const char * gn = g ? zls_graph_name_get(g) : (const char *)0;
    int gi = -1;
    for (int k = 0; g && k < g_stage2.bbp.count; k++) if (g_stage2.bbp.table[k] == g) { gi = k; break; }
    for (int i = 0; i < n; i++) {
        IR_t * nd = nodes[i];
        if (!nd) continue;
        int ai = -1;
        for (int k = 0; g && k < g->n; k++) if (g->all[k] == nd) { ai = k; break; }
        int off = eslot[i]; if (off < 0) off = zls_off(nd); if (zls_key_unbacked(g, off)) off = -1;
        int xf = planner_rec_home(nd), rbp = xf != -1, esc = nd->op == IR_TABLE_HDR && off >= 0;
        int hs = !esc && !rbp && off >= 0 && zls_home_why(nd) && !ir_node_produces_value(nd->op);
        int hr = !esc && !rbp && !hs && off >= 0 && g && zls_g_hf_resident(g, nd) && (nd->op != IR_DISJUNCTION || zls_dj_retains(nd) || !zd_dj_on_spine(nd)) &&
            !(nd->op == IR_GATE && (emit_zrow_gix(nd) != INT_MIN || emit_zrow_gixr(nd) != INT_MIN));
        const char * home = (esc || hs || hr) ? "RBP" : zon[i] ? (zd_k(nd) > 0 ? "SPINE" : "NONE") : off < 0 ? "NONE" : rbp ? "RBP" : "RSP_CARVED";
        if (esc || hs || hr) xf = off;
        fprintf(stderr, "[HOMES] g=%d graph=%s all=%d kind=%s home=%s off=%d xf=%d xfwhy=%s\n", gi, gn ? gn : "?", ai, bb_op_name(nd->op), home, off, xf,
            esc ? "address-escapes" : hs ? zls_home_why_name(zls_home_why(nd)) : hr ? (ir_node_produces_value(nd->op) ? "resident-result" : "construct-record") : rbp ? xop_frame_why(nd) : "-");
        if (esc) fprintf(stderr, "[HOMETEST] g=%d graph=%s all=%d kind=%s test=state:address-escapes\n", gi, gn ? gn : "?", ai, bb_op_name(nd->op));
        else fprintf(stderr, "[HOMETEST] g=%d graph=%s all=%d kind=%s test=%s%s\n", gi, gn ? gn : "?", ai, bb_op_name(nd->op), zls_home_why(nd) ? "operand:" : "", zls_home_why_name(zls_home_why(nd)));
    }
    int nv = g ? zls_g_vslot_count(g) : 0;
    for (int k = 0; k < nv; k++) {
        int off = -1;
        const char * vn = zls_g_vslot_get(g, k, &off);
        if (zls_g_keys_only(g)) off = -1;
        int blk = g && g->block_args ? zls_g_block_base(g) : -1; int cov = (k < (int)g_planner.zst_vcov.size()) ? g_planner.zst_vcov[(size_t)k] : 0,
            use = (k < (int)g_planner.zst_vuse.size()) ? g_planner.zst_vuse[(size_t)k] : -1;
        int soff = off; const char * home = off < 0 ? "NONE" : (blk >= 0 && off >= blk) ? "SPINE" : use == 0 ? "NONE" : (cov > 0 && cov == use) ? "SPINE" : "RSP_CARVED";
        if (cov > 0 && cov == use) for (size_t t = 0; t < g_planner.zst_name.size(); t++) if (vn && g_planner.zst_L[t] != INT_MIN && !strcmp(g_planner.zst_name[t], vn)) { soff = -g_planner.zst_L[t];
            break; }
        fprintf(stderr, "[HOMES] g=%d graph=%s local=%s home=%s off=%d\n", gi, gn ? gn : "?", vn ? vn : "?", home, soff);
        fprintf(stderr, "[HOMETEST] g=%d graph=%s local=%s test=%s%s\n", gi, gn ? gn : "?", vn ? vn : "?", zls_home_local_test(g, vn) ? "local:" : "", zls_home_why_name(zls_home_local_test(g, vn)));
    }
    dh_retains_build();
    for (int k = 0; g && k < g->n; k++) {
        const IR_t * nd = g->all[k];
        if (!nd) continue;
        int lo = (int)nd->op;
        int names_local = lo == IR_VAR || lo == IR_VAR_REF || lo == IR_ASSIGN || lo == IR_ASSIGN_VAR || lo == IR_REV_ASSIGN_VAR || lo == IR_NULLTEST_VAR || lo == IR_FIELD_VAR ||
            lo == IR_UNIFY_FIRST || lo == IR_UNIFY_VALUE;
        if (names_local && IR_LIT(nd).sval) {
            for (int v = 0; v < nv; v++) {
                int voff = -1;
                const char * vn = zls_g_vslot_get(g, v, &voff);
                if (vn && !strcmp(vn, IR_LIT(nd).sval)) { fprintf(stderr, "[LOCALUSE] g=%d graph=%s all=%d kind=%s local=%s\n", gi, gn ? gn : "?", k, bb_op_name(nd->op), vn); break; }
            }
        }
        const char * why = dh_box_dyn(g, nd);
        if (why) fprintf(stderr, "[RSPFX] g=%d graph=%s all=%d kind=%s rsp=dynamic why=%s callee=%s\n", gi, gn ? gn : "?", k, bb_op_name(nd->op), why,
            (ir_is_call_kind(ir_kind0(nd)) && IR_LIT(nd).sval) ? IR_LIT(nd).sval : "-");
        else if (nd->op == IR_CUT) fprintf(stderr, "[RSPFX] g=%d graph=%s all=%d kind=%s rsp=renormal why=cut-restores-the-frame-base callee=-\n", gi, gn ? gn : "?", k, bb_op_name(nd->op));
    }
    if (g && g->root_graph && g->standing_cells > 0)
        fprintf(stderr, "[HOMES] g=%d graph=%s standing=%d home=STANDING why=dynamic-predicate-root\n", gi, gn ? gn : "?", g->standing_cells);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void dp_col(long v, long none) { if (v == none) printf(" -"); else printf(" %ld", v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void planner_dump_plan(const IR_graph_t * g, const zeta_plan_t * pl, const char * prefix) {
    extern const char * zls_graph_name_get(const IR_graph_t *);
    const char * gn = g ? zls_graph_name_get(g) : (const char *)0;
    IR_t ** nodes = g_planner.codegen_flat_chain_body_nodes;
    int n = pl->n;
    if (!g_planner.dump_plan_cols) { printf("PLAN_COLUMNS E X K definite zeta off depth retains_code owner fenced gpop wpop restore act\n"); g_planner.dump_plan_cols = 1; }
    printf("PLAN graph=%s serial=%d chain=%s n=%d constructs=%d resumable=%d\n", gn ? gn : "?", g ? g->serial : -1, prefix ? prefix : "-", n, (g && pl->own_root) ? g->n_constructs : 0,
        g ? zls_g_resumable_frame(g) : 0);
    for (int c = 0; g && pl->own_root && c < g->n_constructs; c++) {
        const IR_construct_t * k = &g->constructs[c];
        int bi = -1, ei = -1;
        for (int q = 0; q < g->n; q++) { if (g->all[q] == k->begin) bi = q; if (k->end && g->all[q] == k->end) ei = q; }
        printf("PLAN_CONSTRUCT graph=%s serial=%d k=%d kind=%s begin=%d end=%d lo=%d hi=%d\n", gn ? gn : "?", g->serial, c, ir_construct_name(k->kind), bi, ei, k->lo, k->hi);
    }
    for (int i = 0; i < n; i++) {
        IR_t * nd = nodes[i];
        int ai = -1;
        for (int k = 0; g && k < g->n; k++) if (g->all[k] == nd) { ai = k; break; }
        int on = pl->on[i], K = zd_k(nd), X = pl->out[i], gp = pl->gp[i], wp = pl->wp[i];
        int off = pl->lkey ? pl->lkey[i] : -1, xf = pl->home ? pl->home[i] : -1, esc = nd->op == IR_TABLE_HDR && off >= 0;
        const char * zeta = zeta_name(pl->zeta ? (int)pl->zeta[i] : ZETA_NONE);
        long zoff = esc ? off : xf != -1 ? xf : (!on && off >= 0) ? off : LONG_MIN;
        int known = pl->xb && pl->xb[i] != INT_MIN;
        printf("PLAN_ROW i=%d all=%d kind=%s", i, ai, bb_op_name(nd->op));
        dp_col(on ? X - K : LONG_MIN, LONG_MIN);
        dp_col(on ? X : LONG_MIN, LONG_MIN);
        dp_col(K, LONG_MIN);
        dp_col(on ? known : LONG_MIN, LONG_MIN);
        printf(" %s", zeta);
        dp_col(zoff, LONG_MIN);
        dp_col((on && known) ? (long)pl->xb[i] + X : LONG_MIN, LONG_MIN);
        dp_col(pl->ret ? pl->ret[i] : planner_retains(nd), LONG_MIN);
        dp_col(pl->claim[i], -1);
        dp_col(on ? gp > 0 : LONG_MIN, LONG_MIN);
        dp_col(on ? gp : LONG_MIN, LONG_MIN);
        dp_col(on ? wp : LONG_MIN, LONG_MIN);
        dp_col((pl->cut && pl->cut[i] >= 0) ? pl->cut[i] : LONG_MIN, LONG_MIN);
        printf(" %s", planner_act_cause_name(planner_act_cause_of(nd)));
        printf("\n");
    }
}

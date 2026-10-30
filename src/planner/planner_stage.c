#include "IR.h"
#include "stage2.h"
#include "gc_frame_map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void ir_vslot_push(IR_graph_t * g, const char * name, int off);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void planner_graph_facts(IR_graph_t * g) {
    extern int zls_g_resumable_frame(const IR_graph_t *);
    extern int zls_g_block_args(const IR_graph_t *);
    extern int zls_g_det_block(const IR_graph_t *);
    extern int zls_g_home_frame(const IR_graph_t *);
    extern int zls_g_generator_top(const IR_graph_t *);
    extern int zls_g_block_base(const IR_graph_t *);
    extern int zls_g_locals(const IR_graph_t *);
    extern int zls_g_value_region(const IR_graph_t *);
    extern int zdp_seam_tier(const IR_t *);
    extern int zls_g_keys_only(const IR_graph_t *);
    extern int zls_g_kbase(const IR_graph_t *);
    extern int planner_graph_retains(const IR_graph_t *);
    extern int zls_g_frame_bytes(const IR_graph_t *);
    extern int zls_g_fp_total(IR_graph_t *);
    if (!g) return;
    g->pf.resumable_frame = zls_g_resumable_frame(g);
    g->pf.block_args = zls_g_block_args(g);
    g->pf.det_block = zls_g_det_block(g);
    g->pf.home_frame = zls_g_home_frame(g);
    g->pf.generator_top = zls_g_generator_top(g);
    g->pf.block_base = zls_g_block_base(g);
    g->pf.locals = zls_g_locals(g);
    g->pf.value_region = zls_g_value_region(g);
    g->pf.body_seam = g->body_root ? zdp_seam_tier(g->body_root) : 0;
    g->pf.keys_only = zls_g_keys_only(g);
    g->pf.kbase = zls_g_kbase(g);
    g->pf.retains = planner_graph_retains(g);
    g->pf.frame_bytes = zls_g_frame_bytes(g);
    g->pf.fp_total = zls_g_fp_total(g);
    g->pf.flat_gen_rbp = g->pf.frame_bytes + (g->nparams + g->nlocals) * 16;
    g->pf.zframe = (g->zframe_graph && !g->cell_frame) ? 1 : 0;
    {
        extern int zls_g_generator(const IR_graph_t *);
        int zs = 1, ab = 0;
        for (int j = 0; j < g->n; j++) if (g->all[j]) { int o = (int)g->all[j]->op; if (o == IR_MATCH_DEFER || o == IR_MATCH_VALUE) zs = 0; if (o == IR_MATCH_ARBNO) ab = 1; }
        g->pf.zstatic = zs;
        g->pf.uniform = zs && !ab;
        g->pf.callee_fp = g->pf.zframe ? 0 : (zls_g_generator(g) ? (g->nparams + g->nlocals) * 16 : g->pf.fp_total);
        g->pf.gen_ft = ((FLAT_FRAME_ALLOWANCE + g->jcon_value_region + 15) & ~15) + g->pf.callee_fp;
    }
    {
        extern int zls_g_region(const IR_graph_t *);
        extern int zls_g_resume(const IR_graph_t *);
        extern int zls_g_zeta_mark(const IR_graph_t *);
        extern int zls_off(const IR_t *);
        int cm = -1;
        g->pf.region = zls_g_region(g);
        g->pf.resume = zls_g_resume(g);
        g->pf.zeta_mark = zls_g_zeta_mark(g);
        for (int j = 0; j < g->n; j++) if (g->all[j] && g->all[j]->op == IR_MATCH_ASSIGN_SAVE) { int o = zls_off(g->all[j]); if (o >= 0 && (cm < 0 || o < cm)) cm = o; }
        g->pf.cap_min = cm;
    }
    { extern const char * zls_graph_name_get(const IR_graph_t *); g->pf.name = zls_graph_name_get(g); }
    {
        extern int zdp_scan_pure(const IR_t *);
        int nc = 0, lf = 1, fn = 0;
        for (int j = 0; j < g->n; j++) {
            IR_t * m = g->all[j];
            if (!m) continue;
            int o = (int)m->op;
            if (o == IR_MATCH_ALTERNATE || o == IR_DISJUNCTION) { if (m->seal) lf = 0; else nc++; continue; }
            if (o == IR_MATCH_FENCE1 || o == IR_MATCH_FENCE0) { fn = 1; continue; }
            if (o == IR_SUCCEED || o == IR_FAIL) continue;
            if (!zdp_scan_pure(m)) lf = 0;
        }
        g->pf.choice_n = nc;
        g->pf.choice_leaf = lf;
        g->pf.choice_fence = fn;
    }
    {
        extern int zls_g_layout_q(const IR_graph_t *, uint64_t *, int, int *, int *);
        int cap = 64, gaps = 0, conf = 0, n;
        uint64_t * q = (uint64_t *)ct_zalloc((size_t)cap, sizeof(uint64_t));
        while ((n = zls_g_layout_q(g, q, cap, &gaps, &conf)) > cap) { cap *= 2; q = (uint64_t *)ct_zalloc((size_t)cap, sizeof(uint64_t)); }
        g->pf.lay = q;
        g->pf.lay_n = n;
        g->pf.lay_gaps = gaps;
        g->pf.lay_conf = conf;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void planner_stage_graph(IR_graph_t * g) {
    if (!g) return;
    extern void zls_build(IR_graph_t *);
    extern int zls_off(const IR_t *);
    extern int zls_g_nslots(const IR_graph_t *);
    extern int zls_g_region(const IR_graph_t *);
    extern int zls_g_frame_region(const IR_graph_t *);
    extern int zls_g_resume(const IR_graph_t *);
    extern int zls_g_zeta_mark(const IR_graph_t *);
    extern int zls_g_vslot_count(const IR_graph_t *);
    extern const char * zls_g_vslot_get(const IR_graph_t *, int, int *);
    zls_build(g);
    {
        extern int zdp_analyze(IR_graph_t *);
        extern int x86_zdp_on_c(void);
        extern int zdp_mode(void);
        extern int zzone_plan(IR_graph_t *);
        extern int zzone_on_c(void);
        if (zdp_mode() || x86_zdp_on_c() || zzone_on_c()) { zdp_analyze(g); zzone_plan(g); }
    }
    g->n_vslots = 0;
    if (g->vslot_ix_cap) memset(g->vslot_ix, 0, (size_t)g->vslot_ix_cap * sizeof(int));
    for (int v = 0; v < zls_g_vslot_count(g); v++) { int off = -1; const char * vn = zls_g_vslot_get(g, v, &off); if (vn && off >= 0) ir_vslot_push(g, vn, off); }
    g->resume_slot = zls_g_resume(g);
    g->zeta_mark_slot = zls_g_zeta_mark(g);
    g->jcon_value_region = zls_g_frame_region(g);
    g->nvalue_slots = zls_g_nslots(g);
    { extern int zdp_mode(void); extern void zdp_report(IR_graph_t *, const char *); if (zdp_mode()) zdp_report(g, "graph"); { extern void zdp_port_census(IR_graph_t *); zdp_port_census(g); } }
    planner_graph_facts(g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void planner_stage(stage2_t * s2, int main_bb_idx) {
    extern void fl_derive_tier(IR_graph_t * g);
    extern void zls_home_callees_mark(IR_graph_t ** gs, int n, const char ** pn, const int * px, int np);
    extern void zls_home_pass(const IR_graph_t * g);
    extern void zls_g_generator_set(const IR_graph_t * g);
    int _mx = main_bb_idx;
    {
        int np = s2->proc_count;
        const char * pn[np > 0 ? np : 1];
        int px[np > 0 ? np : 1];
        for (int i = 0; i < np; i++) { pn[i] = s2->proc_table[i].name; px[i] = s2->proc_table[i].bb_idx; }
        zls_home_callees_mark(s2->bbp.table, s2->bbp.count, pn, px, np);
    }
    for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi] && s2->bbp.table[_gi]->home_frame) fl_derive_tier(s2->bbp.table[_gi]);
    for (int _gi = 0; _gi < s2->bbp.count; _gi++) if (s2->bbp.table[_gi]) {
        IR_graph_t * _g = s2->bbp.table[_gi];
        if (!_g->home_frame) fl_derive_tier(_g);
        if (_gi == _mx && !_g->cell_frame && !_g->root_graph) _g->zframe_graph = _g->entry_frame ? 1 : 0;
        zls_home_pass(_g);
        planner_stage_graph(_g);
    }
    for (int i = 0; i < s2->proc_count; i++) if (s2->proc_table[i].is_generator && s2->proc_table[i].bb_idx >= 0 && s2->proc_table[i].bb_idx < s2->bbp.count) {
        zls_g_generator_set(s2->bbp.table[s2->proc_table[i].bb_idx]);
        planner_graph_facts(s2->bbp.table[s2->proc_table[i].bb_idx]);
    }
}

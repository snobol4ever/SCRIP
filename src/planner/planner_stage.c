#include "IR.h"
#include "stage2.h"
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
    if (!g) return;
    g->pf.resumable_frame = zls_g_resumable_frame(g);
    g->pf.block_args = zls_g_block_args(g);
    g->pf.det_block = zls_g_det_block(g);
    g->pf.home_frame = zls_g_home_frame(g);
    g->pf.generator_top = zls_g_generator_top(g);
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

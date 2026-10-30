#include <alloca.h>
#include "rt/rt_arena.h"
#include "g_runtime.h"
#include "driver_private.h"
#include "bb_box.h"
void rt_proc_set_generator(const char *, int);
void rt_proc_set_jmpentry(const char *, int);
void rt_proc_set_pinned(const char *, int);
void rt_proc_set_variadic(const char *, int);
void rt_proc_set_rest_kind(const char *, int);
void rt_proc_set_named_rest(const char *, int);
void rt_proc_set_dyn_scope(const char *, int);
void rt_proc_set_result_name(const char *, const char *);
void rt_proc_set_thunk(const char *, int);
void rt_proc_register(const char *, const char **, int);
void rt_proc_set_nformals(const char *, int);
void rt_proc_set_frame_bytes(const char *, int);
void rt_proc_set_fn(const char *, bb_box_fn);
void rt_proc_set_dcfn(const char *, void *);
void rt_proc_set_zstatic(const char *, int);
void rt_proc_set_gen_region_ft(const char *, int);
void bb_ab_seal_entry_cells(const char *, void *, int);
int zls_g_entry_block(const IR_graph_t *);
void emit_thunk_star_bind(const char *);
void planner_proc_road(stage2_t *, int, int, planner_road_t *);
struct zeta_plan_t * planner_chain_record(IR_graph_t *, IR_t *, const char *);
bb_box_fn emit_chain(struct zeta_plan_t *, FILE *, const char *);
void emit_gc_tables_register(const void *);
void emit_proc_unprepare(void);
void bb_thunk_rec_fill(const char *, void *, int, int);
int emit_icn_n2_gen_region_ft(const char *, int, IR_graph_t *);
void planner_road_none(planner_road_t *);
int planner_stage_rt(IR_graph_t *, IR_t *, const char *, const planner_road_t *);
void planner_stage_release(IR_t *);
extern IR_graph_t * g_emit_cfg;
extern int g_last_flat_frame_bytes;
extern int g_last_flat_zstatic;
extern long g_last_dc_off;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void driver_register_proc(stage2_t * s2, int pi) {
    ProcEntry * pe = &s2->proc_table[pi];
    int np = pe->nparams;
    const char ** pn = NULL;
    if (np > 0) { pn = (const char **) ct_zalloc((size_t) np, sizeof(const char *)); for (int k = 0; k < np && k < pe->lower_sc.n; k++) pn[k] = pe->lower_sc.e[k].name; }
    rt_proc_register(pe->name, pn, np);
    rt_proc_set_nformals(pe->name, pe->nformals);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void driver_proc_props(stage2_t * s2, int pi) {
    ProcEntry * pe = &s2->proc_table[pi];
    const char * pname = pe->name;
    if (pe->bb_idx < 0 || pe->bb_idx >= s2->bbp.count) {
        fprintf(stderr, "scrip: driver_proc_props: proc %s has no graph (bb_idx %d of %d): ir_delete_all invalidated it\n", pe->name ? pe->name : "?", pe->bb_idx, s2->bbp.count);
        exit(2);
    }
    IR_graph_t * g = s2->bbp.table[pe->bb_idx];
    rt_proc_set_generator(pname, pe->is_generator);
    rt_proc_set_jmpentry(pname, !g->caller_frame && strncmp(pname, "gram__", 6) != 0);
    rt_proc_set_pinned(pname, zls_g_entry_block(g));
    rt_proc_set_variadic(pname, pe->is_variadic);
    rt_proc_set_rest_kind(pname, pe->rest_kind);
    rt_proc_set_named_rest(pname, pe->named_rest);
    rt_proc_set_dyn_scope(pname, pe->dyn_scope);
    if (pe->result_name) rt_proc_set_result_name(pname, pe->result_name);
    rt_proc_set_thunk(pname, pe->thunk_kind != PROC_THUNK_NONE);
    if (pe->thunk_kind == PROC_THUNK_EXPR) emit_thunk_star_bind(pname);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * driver_install_proc(stage2_t * s2, int pi, const emit_install_hooks_t * hk) {
    ProcEntry * pe = &s2->proc_table[pi];
    const char * pname = pe->name;
    if (pe->bb_idx < 0 || pe->bb_idx >= s2->bbp.count) {
        fprintf(stderr, "scrip: driver_install_proc: proc %s has no graph (bb_idx %d of %d): ir_delete_all invalidated it\n", pe->name ? pe->name : "?", pe->bb_idx, s2->bbp.count);
        exit(2);
    }
    IR_graph_t * g = s2->bbp.table[pe->bb_idx];
    int is_lbl = pname && strncmp(pname, "LBL__", 5) == 0;
    g_emit_cfg = g;
    driver_proc_props(s2, pi);
    planner_road_t rd;
    planner_proc_road(s2, pi, 0, &rd);
    int isp = 0;
    emit_proc_prepare(s2, pi, &rd, &isp);
    char * pfx = (char *) alloca(strlen(pname) + 6);
    snprintf(pfx, strlen(pname) + 6, "proc_%s", pname);
    IR_t * pent = pe->proc_entry_node ? pe->proc_entry_node : g->entry;
    if (!is_lbl && hk && hk->before) hk->before(hk->ctx, pi, pe->bb_idx);
    if (getenv("SCRIP_PL_RTASM") && !is_lbl) {
        fprintf(stderr, "[RTASM] ---- compile-time proc %s ----\n", pname);
        emit_chain(planner_chain_record(g, pent, pfx), stderr, pfx);
        fprintf(stderr, "[RTASM] ---- end %s ----\n", pname);
    }
    bb_box_fn pfn = is_lbl ? NULL : emit_chain(planner_chain_record(g, pent, pfx), NULL, pfx);
    if (pfn && hk && hk->after) hk->after(hk->ctx, pi, pe->bb_idx, (void *) pfn);
    emit_gc_tables_register((const void *) pfn);
    emit_proc_unprepare();
    if (!is_lbl) rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes);
    if (getenv("SCRIP_N2_FB_PREPASS")) fprintf(stderr, "[N2-FB] POSTEMIT mode=3 proc=%s idx=%d fb=%d\n", pname ? pname : "(null)", pi, g_last_flat_frame_bytes);
    if (pfn) rt_proc_set_fn(pname, pfn);
    if (pfn && isp) bb_thunk_rec_fill(pname, (void *) pfn, g_last_flat_frame_bytes, g_last_flat_zstatic);
    if (pfn) bb_ab_seal_entry_cells(pname, (void *) pfn, 1);
    if (pfn) rt_proc_set_zstatic(pname, g_last_flat_zstatic);
    int gft = emit_icn_n2_gen_region_ft(pname, pe->is_generator, g);
    if (gft > 0) rt_proc_set_gen_region_ft(pname, gft);
    if (pfn && g_last_dc_off >= 0) rt_proc_set_dcfn(pname, (void *) ((char *) pfn + g_last_dc_off));
    if (pfn && g_last_flat_frame_bytes > 0) rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes);
    return (void *) pfn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_box_fn compile_chain(IR_graph_t * g, IR_t * entry, FILE * out, const char * prefix) {
    if (!entry) return NULL;
    planner_road_t rd;
    planner_road_none(&rd);
    emit_flat_road(&rd);
    int staged = planner_stage_rt(g, entry, prefix, &rd);
    bb_box_fn fn = emit_chain(planner_chain_record(g, entry, prefix), out, prefix);
    if (staged) planner_stage_release(entry);
    return fn;
}

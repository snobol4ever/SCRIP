#include "planner_head.h"
#include "planner.h"
extern "C" void zls_fct_finalize(IR_graph_t *, int);
extern "C" void zls_graph_name(const IR_graph_t *, const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int planner_blob_casmark(void) { if (g_planner.sn4_blob_casmark_v < 0) { const char * e = getenv("SCRIP_BLOB_CASMARK"); g_planner.sn4_blob_casmark_v = (e && *e == '0') ? 0 : 1;
    } return g_planner.sn4_blob_casmark_v; }
int planner_blob_head_bytes(void) { return planner_blob_casmark() ? 56 : 40; }
int planner_road_blob_scope(const planner_road_t * rd) { return (rd->jmp_entry && rd->pat && !(rd->floor > 0)) ? 1 : 0; }
int proc_role3_kind(const IR_graph_t *g) {
    if (!g || !g->entry) return 0;
    if (g->entry->op == IR_GOTO_DEFERRED) return 1;
    const IR_t *e = (g->entry->op == IR_DEFINE && IR_LIT(g->entry).ival == 3) ? g->entry : (const IR_t *)0;
    return !e ? 0 : (e->γ.node && e->γ.node->op == IR_GOTO_DEFERRED) ? 1 : 2;
}
void planner_stage_chain(IR_graph_t * g, IR_t * entry, const char * prefix, const planner_road_t * rd) {
    if (!entry) return;
    planner_chain_operands(entry);
    if (g) zls_fct_finalize(g, 1);
    int scope = planner_road_blob_scope(rd), head = planner_blob_head_bytes();
    planner_act_build(g, scope, head);
    planner_pass_plan(g, entry, prefix);
    zeta_plan_t * rec = g_planner.planner_table[entry];
    rec->road = *rd; rec->scope = scope; rec->head = head;
    planner_blob_facts(rec);
    if (g) planner_graph_facts(g);
    if (g_planner.dump_homes || g_planner.dump_plan) {
        planner_restore(rec);
        if (g_planner.dump_homes) planner_dump_homes(g, g_planner.codegen_flat_chain_body_nodes, rec->n, rec->on);
        if (g_planner.dump_plan) planner_dump_plan(g, rec, prefix);
    }
}
void planner_stage_proc(stage2_t * s2, int pi, int mode) {
    ProcEntry * pe = &s2->proc_table[pi]; const char * pname = pe->name; int idx = pe->bb_idx;
    pe->port_exit = planner_port_exit_promotes(s2, pi);
    if (!pname || !strcmp(pname, "main") || idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) return;
    IR_graph_t * g = s2->bbp.table[idx]; int is_lbl = strncmp(pname, "LBL__", 5) == 0; int bare = mode == 1 && proc_role3_kind(g) == 1;
    zls_graph_name(g, pname);
    if (is_lbl || bare) return;
    planner_road_t rd; planner_road_proc(s2, pi, &rd, mode == 2);
    char * pfx = (char *) alloca(strlen(pname) + 6); snprintf(pfx, strlen(pname) + 6, "proc_%s", pname);
    planner_stage_chain(g, pe->proc_entry_node ? pe->proc_entry_node : g->entry, pfx, &rd);
}
void planner_stage_procs(stage2_t * s2, int from_pi, int mode) {
    for (int pi = from_pi; pi < s2->proc_count; pi++) planner_stage_proc(s2, pi, mode);
}
void planner_proc_road(stage2_t * s2, int pi, int rt, planner_road_t * rd) {
    ProcEntry * pe = &s2->proc_table[pi]; IR_graph_t * g = stage2_proc_graph(s2, pi, "planner_proc_road");
    const planner_road_t * rr = planner_road_of(pe->proc_entry_node ? pe->proc_entry_node : g->entry);
    if (rr) *rd = *rr; else planner_road_proc(s2, pi, rd, rt);
}
void planner_road_floor_dc(stage2_t * s2, int pi, int rt, int * floor, int * dc_np) { planner_road_t rd; planner_proc_road(s2, pi, rt, &rd); *floor = rd.floor; *dc_np = rd.dc_np; }
const planner_road_t * planner_road_of(const IR_t * entry) {
    if (!entry) return (const planner_road_t *)0;
    auto it = g_planner.planner_table.find(entry);
    return it == g_planner.planner_table.end() ? (const planner_road_t *)0 : &it->second->road;
}
void planner_stage_main(IR_graph_t * bbg, const char * prefix) { planner_road_t rd; planner_road_none(&rd); planner_stage_chain(bbg, bbg->entry, prefix, &rd); }
int planner_stage_rt(IR_graph_t * g, IR_t * entry, const char * prefix, const planner_road_t * rd) { if (!entry || planner_entry_planned(entry)) return 0; planner_stage_chain(g, entry, prefix, rd);
    g_planner.planner_rt++; return 1; }
void planner_stage_release(IR_t * entry) {
    planner_table_forget(entry);
    g_planner.g_act_g = (const IR_graph_t *)0;
    g_planner.pdef_g = (const IR_graph_t *)0;
    g_planner.pdef_ix.clear();
    g_planner.pdef_memo.clear();
    if (!g_planner.pdef_depth) g_planner.pdef_lock = (const IR_graph_t *)0;
    g_planner.zst_sm.clear();
    g_planner.g_pcon_g = (const IR_graph_t *)0;
    g_planner.g_pcon_idx.clear();
    g_planner.g_zdo_nodes = (IR_t **)0;
    if (g_planner.g_zdo.p) memset(g_planner.g_zdo.p, 0, (size_t)g_planner.g_zdo.len * g_planner.g_zdo.esz);
    if (g_planner.g_zrm_map.p) memset(g_planner.g_zrm_map.p, 0, (size_t)g_planner.g_zrm_map.len * g_planner.g_zrm_map.esz);
    if (g_planner.g_ocr_map.p) memset(g_planner.g_ocr_map.p, 0, (size_t)g_planner.g_ocr_map.len * g_planner.g_ocr_map.esz);
}
}

#include "../emitter/emit_head.h"
#include "../emitter/emit_decls.h"
#include "planner.h"
extern "C" int rt_pl_dc_ok(const char *, int);
extern "C" int zls_g_region(const IR_graph_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void planner_road_none(planner_road_t * rd) { rd->jmp_entry = 0; rd->pat = 0; rd->lex = 0; rd->gen = 0; rd->floor = 0; rd->dc_np = -1; rd->gen_proc = 0; rd->caller_dl = -1; }
void planner_road_named(const char * pname, int dyn_scope, int is_generator, const IR_graph_t * g, planner_road_t * rd) {
    planner_road_none(rd);
    if (pname && strncmp(pname, "gram__", 6) == 0) return;
    int is_lbl = pname && strncmp(pname, "LBL__", 5) == 0;
    rd->jmp_entry = 1; rd->lex = (!dyn_scope && !is_lbl) ? 1 : 0; rd->gen = (is_generator && ir_graph_has_suspend(g)) ? 1 : 0;
}
void planner_road_pattern(planner_road_t * rd) { planner_road_none(rd); rd->jmp_entry = 1; rd->pat = 1; }
void planner_road_proc(stage2_t * s2, int pi, planner_road_t * rd, int rt) {
    ProcEntry * pe = &s2->proc_table[pi]; const char * pname = pe->name; IR_graph_t * g = s2->bbp.table[pe->bb_idx];
    int is_lbl = pname && strncmp(pname, "LBL__", 5) == 0;
    if (pe->thunk_kind == PROC_THUNK_PATTERN) planner_road_pattern(rd); else planner_road_named(pname, pe->dyn_scope, pe->is_generator, g, rd);
    rd->gen_proc = pe->is_generator;
    int floor_hit = (is_lbl || (g->entry && ((g->entry->op == IR_DEFINE && IR_LIT(g->entry).ival == 3) || g->entry->op == IR_GOTO_DEFERRED)));
    if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[FLOOR-DIAG] pname=%s is_lbl=%d entry_op=%d hit=%d\n", pname ? pname : "(null)", is_lbl, g->entry ? (int) g->entry->op : -1, floor_hit);
    if (floor_hit) for (int mi = 0; mi < s2->proc_count; mi++) if (s2->proc_table[mi].name && !strcmp(s2->proc_table[mi].name, "main")) {
        int mx = s2->proc_table[mi].bb_idx; if (mx >= 0 && mx < s2->bbp.count && s2->bbp.table[mx]) rd->floor = zls_g_region(s2->bbp.table[mx]); break; }
    if (rt && floor_hit && rd->floor <= 0) rd->floor = zls_g_region(g);
    if (getenv("SCRIP_FLOOR_DIAG") && floor_hit) fprintf(stderr, "[FLOOR-DIAG] -> g_flat_frame_floor=%d\n", rd->floor);
    rd->dc_np = (!rd->pat && rt_pl_dc_ok(pname, pe->nparams)) ? pe->nparams : -1;
    rd->caller_dl = (g->caller_frame && g->nslots > 0) ? pe->decl_level : -1;
}
int planner_port_exit_promotes(const stage2_t * s2, int pi) {
    const ProcEntry * pe = &s2->proc_table[pi];
    if (!pe->name || pe->bb_idx < 0 || pe->bb_idx >= s2->bbp.count) return 0;
    const IR_graph_t * g = s2->bbp.table[pe->bb_idx];
    if (!g) return 0;
    int flat_pat = (pe->thunk_kind == PROC_THUNK_PATTERN) ? 1 : 0;
    int flat_gen = (pe->is_generator && ir_graph_has_suspend(g)) ? 1 : 0;
    int flat_lcl_proc = (!flat_pat && !flat_gen && ((g->nparams > 0 || g->nlocals > 0) || g->cell_frame)) ? 1 : 0;
    int zframe_graph_flag = (g->zframe_graph && !g->cell_frame) ? 1 : 0;
    return (zframe_graph_flag || (g->cell_frame && flat_lcl_proc)) ? 1 : 0;
}
}

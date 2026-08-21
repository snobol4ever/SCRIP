#include "proc_collect.h"
#include <string.h>
#include <stdlib.h>
static const char **g_proc_slot_names = NULL;
static int g_proc_slot_n = 0;
static int g_proc_slot_max = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void proc_collect_reset(void) { g_proc_slot_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int proc_slot_of(const char *name) {
    if (!name) return -1;
    for (int i = 0; i < g_proc_slot_n; i++) if (g_proc_slot_names[i] && strcmp(g_proc_slot_names[i], name) == 0) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int proc_collect_add(const char *name) {
    if (!name || !name[0]) return -1;
    int k = proc_slot_of(name); if (k >= 0) return k;
    if (g_proc_slot_n >= g_proc_slot_max) {
        int nm = g_proc_slot_max ? g_proc_slot_max * 2 : 64; const char **g = (const char **)realloc(g_proc_slot_names, (size_t)nm * sizeof(const char *));
        if (!g) return -1; g_proc_slot_names = g; g_proc_slot_max = nm;
    }
    g_proc_slot_names[g_proc_slot_n] = name; return g_proc_slot_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int proc_slot_count(void) { return g_proc_slot_n; }
const char *proc_slot_name(int k) { return (k >= 0 && k < g_proc_slot_n) ? g_proc_slot_names[k] : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int proc_direct_eligible(const char *name) {
    extern int rt_proc_is_registered(const char *); extern int rt_proc_frame_nslots(const char *);
    return name && name[0] && rt_proc_is_registered(name) && rt_proc_frame_nslots(name) < 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void proc_collect_graph(IR_graph_t *g) {
    if (!g || !g->all) return;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i]; if (!nd || !IR_LIT(nd).sval) continue;
        switch (nd->op) {
        case IR_CALL: case IR_CALL_PROC_STAGED:
            if (proc_direct_eligible(IR_LIT(nd).sval)) (void)proc_collect_add(IR_LIT(nd).sval); break;
        default: break;
        }
    }
}
static int g_scc_taint = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scc_taint_graph(IR_graph_t *g) {
    if (!g || !g->all || g_scc_taint) return;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i]; if (!nd || !IR_LIT(nd).sval) continue;
        switch (nd->op) {
        case IR_CALL: case IR_CALL_PROC_STAGED: { const char *s = IR_LIT(nd).sval;
            if (strcmp(s, "UNLOAD") == 0) { g_scc_taint = 1; return; }
            if (strcmp(s, "OPSYN") == 0) { IR_t *a2 = (IR_LIT(nd).ival >= 3) ? ir_call_arg(nd, 2) : (IR_t *)0;
                if (!(a2 && a2->op == IR_LIT_INTEGER && (IR_LIT(a2).ival == 1 || IR_LIT(a2).ival == 2))) { g_scc_taint = 1; return; } } } break;
        default: break;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int scc_program_ok(void) { return !g_scc_taint; }

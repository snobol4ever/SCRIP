#include "IR.h"
#include "bb_program.h"
#include "stage2.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
stage2_t g_stage2;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* N-2 TRANSITIVE-RESERVE (seat06 2026-08-29, row icon-n2-flat-gen-host-transitive-reserve): THE CROSS-GRAPH ACCESSOR the recursive reserve(g) formula in x86_asm.h walks a callee's own graph
   through. LIVES HERE, NOT IN THE DRIVER (src/driver/scrip.c), FOR THE SAME TWO-LINK-UNITS REASON icn_genframe2()/icn_gen_host_reserve() already document at their own definitions: this file
   (sm_prog.c, which already owns g_stage2 itself) is compiled into libscrip_rt.so, and icn_gen_host_reserve()'s callers include bb_call_proc_staged.cpp -- a TEMPLATE, also compiled into
   libscrip_rt.so, and (unlike the driver's own scrip.c) NOT relinked into a mode-4 standalone program. A first attempt defined this in scrip.c: it linked the `scrip` driver fine (scrip.c's own
   object is part of that final link) but a mode-4 `--compile`d program links ONLY against libscrip_rt.so, leaving `undefined reference to n2_graph_by_proc_name' -- MEASURED, not theoretical,
   the identical failure class icn_gen_host_reserve()'s own comment names ("linked clean for the driver and died... when the RT tried to call it"). Returns NULL on an unknown name, never a
   guessed graph -- the caller already treats "no graph" as "nothing to recurse into". */
IR_graph_t * n2_graph_by_proc_name(const char *name) {
    if (!name) return NULL;
    for (int i = 0; i < g_stage2.proc_count; i++) {
        if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) {
            int gi = g_stage2.proc_table[i].bb_idx;
            return (gi >= 0 && gi < g_stage2.bbp.count) ? g_stage2.bbp.table[gi] : NULL;
        }
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void stage2_reset(void)
{
    bb_program_free(&g_stage2.bbp);
    free(g_stage2.label_table); g_stage2.label_table = NULL;
    free(g_stage2.proc_table);  g_stage2.proc_table  = NULL;
    g_stage2.label_cap   = STAGE2_LABEL_MAX;
    g_stage2.label_count = 0;
    g_stage2.label_table = calloc((size_t)g_stage2.label_cap, sizeof(LabelEntry));
    g_stage2.proc_cap    = STAGE2_PROC_TABLE_MAX;
    g_stage2.proc_count  = 0;
    g_stage2.proc_table  = calloc((size_t)g_stage2.proc_cap,  sizeof(ProcEntry));
    memset(&g_stage2.resolve_pred_table,   0, sizeof g_stage2.resolve_pred_table);
    memset(&g_stage2.module_registry, 0, sizeof g_stage2.module_registry);
    g_stage2.module_registry.main_mod = -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void ir_delete_all(stage2_t *s2)
{
    if (s2) bb_program_free(&s2->bbp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int stage2_label_grow(stage2_t *s2)
{
    if (s2->label_count >= s2->label_cap) {
        s2->label_cap = s2->label_cap ? s2->label_cap * 2 : 16;
        s2->label_table = realloc(s2->label_table, (size_t)s2->label_cap * sizeof(LabelEntry));
    }
    int idx = s2->label_count++;
    memset(&s2->label_table[idx], 0, sizeof(LabelEntry));
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int stage2_proc_grow(stage2_t *s2)
{
    if (s2->proc_count >= s2->proc_cap) {
        s2->proc_cap = s2->proc_cap ? s2->proc_cap * 2 : 16;
        s2->proc_table = realloc(s2->proc_table, (size_t)s2->proc_cap * sizeof(ProcEntry));
    }
    int idx = s2->proc_count++;
    memset(&s2->proc_table[idx], 0, sizeof(ProcEntry));
    s2->proc_table[idx].proc_entry_node = NULL;
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int stage2_owner_varslot(const char * proc, const char * var) {
    extern int ir_varslot_of(const IR_graph_t * g, const char * name);
    if (!proc || !var) return -1;
    for (int i = 0; i < g_stage2.proc_count; i++) {
        if (!g_stage2.proc_table[i].name || strcmp(g_stage2.proc_table[i].name, proc)) continue;
        int b = g_stage2.proc_table[i].bb_idx;
        if (b < 0 || b >= g_stage2.bbp.count || !g_stage2.bbp.table[b]) return -1;
        return ir_varslot_of(g_stage2.bbp.table[b], var);
    }
    return -1;
}

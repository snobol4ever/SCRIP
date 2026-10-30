#include "IR.h"
#include "ct_arena.h"
#include "bb_program.h"
#include "stage2.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
stage2_t g_stage2;
static unsigned bb_proc_hash(const char *s) { unsigned h = 2166136261u; for (; *s; s++) h = (h ^ (unsigned char)*s) * 16777619u; return h; }
static void bb_proc_index_build(stage2_t *s) {
    int cap = 16, nul = 0;
    while (cap < 2 * s->proc_count + 2) cap <<= 1;
    ct_drop(s->proc_ix);
    s->proc_ix = (int *)ct_alloc((size_t)cap * sizeof(int));
    for (int k = 0; k < cap; k++) s->proc_ix[k] = -1;
    s->proc_ix_mask = (unsigned)(cap - 1);
    for (int i = 0; i < s->proc_count; i++) {
        const char *nm = s->proc_table[i].name;
        unsigned h;
        if (!nm) { nul++; continue; }
        h = bb_proc_hash(nm) & s->proc_ix_mask;
        while (s->proc_ix[h] >= 0 && strcmp(s->proc_table[s->proc_ix[h]].name, nm)) h = (h + 1) & s->proc_ix_mask;
        if (s->proc_ix[h] < 0) s->proc_ix[h] = i;
    }
    s->proc_ix_n = s->proc_count;
    s->proc_ix_tbl = s->proc_table;
    s->proc_ix_nul = nul;
}
int bb_proc_find(const char * fname) {
    stage2_t *s = &g_stage2;
    if (!fname || s->proc_count <= 0) return -1;
    for (int pass = 0; pass < 2; pass++) {
        int i = -1;
        unsigned h;
        if (pass || !s->proc_ix || s->proc_ix_n != s->proc_count || s->proc_ix_tbl != s->proc_table) bb_proc_index_build(s);
        h = bb_proc_hash(fname) & s->proc_ix_mask;
        while (s->proc_ix[h] >= 0) { if (!strcmp(s->proc_table[s->proc_ix[h]].name, fname)) { i = s->proc_ix[h]; break; } h = (h + 1) & s->proc_ix_mask; }
        if (i >= 0 ? (s->proc_table[i].name && !strcmp(s->proc_table[i].name, fname)) : !s->proc_ix_nul) return i;
    }
    for (int i = 0; i < s->proc_count; i++) if (s->proc_table[i].name && !strcmp(s->proc_table[i].name, fname)) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
void stage2_reset(void) {
    bb_program_free(&g_stage2.bbp);
    ct_drop(g_stage2.label_table);
    g_stage2.label_table = NULL;
    ct_drop(g_stage2.proc_table);
    g_stage2.proc_table = NULL;
    ct_drop(g_stage2.proc_ix);
    g_stage2.proc_ix = NULL;
    g_stage2.proc_ix_n = -1;
    g_stage2.proc_ix_tbl = NULL;
    g_stage2.label_cap = STAGE2_LABEL_MAX;
    g_stage2.label_count = 0;
    g_stage2.label_table = ct_zalloc((size_t)g_stage2.label_cap, sizeof(LabelEntry));
    g_stage2.proc_cap = STAGE2_PROC_TABLE_MAX;
    g_stage2.proc_count = 0;
    g_stage2.proc_table = ct_zalloc((size_t)g_stage2.proc_cap, sizeof(ProcEntry));
    memset(&g_stage2.resolve_pred_table, 0, sizeof g_stage2.resolve_pred_table);
    memset(&g_stage2.module_registry, 0, sizeof g_stage2.module_registry);
    g_stage2.module_registry.main_mod = -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void ir_delete_all(stage2_t *s2) { if (!s2) return; bb_program_free(&s2->bbp); for (int i = 0; i < s2->proc_count; i++) s2->proc_table[i].bb_idx = -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int stage2_label_grow(stage2_t *s2) {
    if (s2->label_count >= s2->label_cap) { s2->label_cap = s2->label_cap ? s2->label_cap * 2 : 16; s2->label_table = ct_grow(s2->label_table, (size_t)s2->label_cap * sizeof(LabelEntry)); }
    int idx = s2->label_count++;
    memset(&s2->label_table[idx], 0, sizeof(LabelEntry));
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int stage2_proc_grow(stage2_t *s2) {
    if (s2->proc_count >= s2->proc_cap) { s2->proc_cap = s2->proc_cap ? s2->proc_cap * 2 : 16; s2->proc_table = ct_grow(s2->proc_table, (size_t)s2->proc_cap * sizeof(ProcEntry)); }
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * stage2_owner_l3_ancestor(const char * proc) {
    if (!proc) return (const char *)0;
    for (int i = 0; i < g_stage2.proc_count; i++) {
        if (!g_stage2.proc_table[i].name || strcmp(g_stage2.proc_table[i].name, proc)) continue;
        int b = g_stage2.proc_table[i].bb_idx;
        if (b < 0 || b >= g_stage2.bbp.count || !g_stage2.bbp.table[b]) return (const char *)0;
        return g_stage2.bbp.table[b]->l3_ancestor_name;
    }
    return (const char *)0;
}

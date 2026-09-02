#include "resolution.h"
#include <math.h>
#include <limits.h>
#include "ast.h"
#include "../../parsers/snobol4/scrip_cc.h"
#include "../../parsers/prolog/prolog_driver.h"
#include "../../parsers/prolog/prolog_atom.h"
extern void rt_trail_unwind(int mark);
#include "gen_value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int           g_resolve_cut_flag = 0;
#include "../../parsers/prolog/pl_cell.h"
pl_trail_t    g_pl_trail          = { { (char *)0, (char *)0, (char *)0, 0 }, 0 };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_value_trail_mark(void) { return g_pl_trail.top; }
_Static_assert(__builtin_offsetof(pl_trail_t, top) == 32 && sizeof(pl_area_t) == 32, "rtx_plcall.s AND src/templates/bb_define.cpp (RTX-FUNC-1 alpha inline) both bake PL_TRAIL_TOP=32 to inline rt_value_trail_mark; pl_area_t grew or shrank -- the asm would stamp the vtmark from the WRONG WORD, which links fine and makes the epilogue landings tidy the wrong span of the value trail");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_value_trail_tidy_dead_window(int mark, void *lower, void *upper) {
    pl_trail_ent_t *ents = (pl_trail_ent_t *)g_pl_trail.area.base;
    char *lo = (char *)lower;
    char *up = (char *)upper;
    if (!ents || mark < 0 || mark > g_pl_trail.top || !lo || lo >= up) return;
    int w = mark;
    for (int r = mark; r < g_pl_trail.top; r++) {
        char *a = (char *)ents[r].addr;
        if (a >= lo && a < up) continue;
        ents[w++] = ents[r];
    }
    g_pl_trail.top = w;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_value_trail_tidy_dead_below(int mark, void *upper) {
    pl_trail_ent_t *ents = (pl_trail_ent_t *)g_pl_trail.area.base;
    char *floor_ = (char *)&ents;
    char *up = (char *)upper;
    if (!ents || mark < 0 || mark > g_pl_trail.top) return;
    int w = mark;
    for (int r = mark; r < g_pl_trail.top; r++) {
        char *a = (char *)ents[r].addr;
        if (a > floor_ && a < up) continue;
        ents[w++] = ents[r];
    }
    g_pl_trail.top = w;
}
pl_area_t     g_pl_env_area       = { (char *)0, (char *)0, (char *)0, 0 };
int           g_resolve_active   = 0;
typedef struct { IR_graph_t *cfg; int first; } resolve_dcg_state_t;
#define RESOLVE_PRED_TABLE_SIZE RESOLVE_PRED_TABLE_SIZE_FWD
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned resolve_pred_hash(const char *s) {
    unsigned h = 5381;
    while (*s) h = h * 33 ^ (unsigned char)*s++;
    return h % RESOLVE_PRED_TABLE_SIZE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void resolve_pred_table_insert(Resolve_PredTable *pt, const char *key, tree_t *choice) {
    unsigned h = resolve_pred_hash(key);
    Resolve_PredEntry *e = malloc(sizeof(Resolve_PredEntry));
    e->key = key; e->choice = choice; e->entry_pc = -1; e->next = pt->buckets[h]; pt->buckets[h] = e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key) {
    for (Resolve_PredEntry *e = pt->buckets[resolve_pred_hash(key)]; e; e = e->next)
        if (strcmp(e->key, key) == 0) return e->choice;
    return NULL;
}

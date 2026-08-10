#include "resolution.h"
#include <math.h>
#include <limits.h>
#include "ast.h"
#include "../../parser/snobol4/scrip_cc.h"
#include "../../parser/prolog/prolog_driver.h"
#include "../../parser/prolog/term.h"
#include "../../parser/prolog/prolog_runtime.h"
#include "../../parser/prolog/prolog_atom.h"
#include "../../parser/prolog/prolog_builtin.h"
extern void rt_trail_unwind(int mark);
extern tree_t *pl_assert_term(Term *t, int *functor_out, int *arity_out);
#include "gen_value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
Trail         g_resolve_trail;
int           g_resolve_cut_flag = 0;
#include "../../parser/prolog/pl_cell.h"
pl_trail_t    g_pl_trail          = { { (char *)0, (char *)0, (char *)0, 0 }, 0 };
/* RSP-F-2 VALUE-TRAIL DEATH TIDY (2026-07-18): a determinate jmp-entry callee fully unwinds its rsp-carved activation at exit, but the value trail still holds {addr,old} entries pointing INTO that dead
 * stack window (the callee's own frame cells, bound and trailed during its run).  Left in place, a later unwind to an older mark RESTORES through those addresses into whatever now occupies the memory —
 * reused C helper frames (the measured *** stack smashing *** in rt_call_arr) or, worse, a LIVE later activation carved over the same region (silent corruption).  Unwind-time filtering is UNSOUND
 * against that reuse-aliasing, so the drop happens at the only sound moment — FRAME DEATH: the det call's landing walks the segment pushed since call-open and compacts out entries whose address lies in
 * the just-vacated stack window (floor = this leaf's own C frame, the deepest live stack byte — the (floor,upper) span is inside the stack VMA, so no heap cell can alias it; upper = the landing rsp,
 * passed by the epilogue as its frame address + 16).  Entries at or above upper are the caller's live cells (head-unification bindings that backtracking MUST later undo) and entries outside the stack
 * span are heap cells — both kept, order preserved (same-address restore pairs are either both dead or both live, so value-restore commutation is untouched).  Behavioral leaves, no language identity:
 * a program that never pushes the value trail gets mark==top and a zero-iteration walk. */
int rt_value_trail_mark(void) { return g_pl_trail.top; }
_Static_assert(__builtin_offsetof(pl_trail_t, top) == 32 && sizeof(pl_area_t) == 32, "rtx_plcall.S AND src/templates/bb_func_activate.cpp (RTX-FUNC-1 alpha inline) both bake PL_TRAIL_TOP=32 to inline rt_value_trail_mark; pl_area_t grew or shrank -- the asm would stamp the vtmark from the WRONG WORD, which links fine and makes the epilogue landings tidy the wrong span of the value trail");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-DC s108 WINDOW FORM — the (lower,upper) span is the EXACT dead activation [frame base, landing rsp), carried by the caller (pcall c.fb on the wire path, the dc micro-stack pair on the direct
 * path), replacing the floor-heuristic band below whose width was accidentally the tidying C leaves' own frame extent: entries in the suffix zone deeper than that band survived forever (the measured
 * dc 28K saw; latent on the wire path too for any graph whose locals span exceeds the epilogue's frame).  lower==0 (no recorded frame: dyn/slim callers) degrades to the empty window — heap and
 * caller cells were never this walk's business. */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_area_t     g_pl_env_area       = { (char *)0, (char *)0, (char *)0, 0 };
int           g_resolve_active   = 0;
resolve_choice    *g_resolve_bfr      = NULL;
resolve_choice    *g_resolve_cut_barrier = NULL;
int           g_resolve_cp_stamp = 0;
Resolve_PredEntry_BB g_resolve_bb_table[RESOLVE_BB_TABLE_MAX];
int             g_resolve_bb_count = 0;
typedef struct { IR_graph_t *cfg; int first; } resolve_dcg_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
Resolve_PredEntry_BB *resolve_bb_lookup(const char *name, int arity) {
    if (!name) return NULL;
    for (int i = 0; i < g_resolve_bb_count; i++)
        if (g_resolve_bb_table[i].arity == arity && g_resolve_bb_table[i].name && strcmp(g_resolve_bb_table[i].name, name) == 0)
            return &g_resolve_bb_table[i];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int resolve_bb_pred_count(void) { return g_resolve_bb_count; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *resolve_bb_pred_name_at(int idx) {
    if (idx < 0 || idx >= g_resolve_bb_count) return NULL;
    return bb_graph_of_pred(&g_resolve_bb_table[idx]) ? g_resolve_bb_table[idx].name : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int resolve_bb_pred_arity_at(int idx) {
    if (idx < 0 || idx >= g_resolve_bb_count) return 0;
    return g_resolve_bb_table[idx].arity;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
Resolve_PredEntry_BB *resolve_bb_register(const char *name, int arity, int bb_idx) {
    if (!name) return NULL;
    Resolve_PredEntry_BB *existing = resolve_bb_lookup(name, arity);
    if (existing) { existing->bb_idx = bb_idx; return existing; }
    if (g_resolve_bb_count >= RESOLVE_BB_TABLE_MAX) return NULL;
    Resolve_PredEntry_BB *e = &g_resolve_bb_table[g_resolve_bb_count++];
    e->name = strdup(name);
    e->arity = arity;
    e->bb_idx = bb_idx;
    e->lower_sc.n = 0;
    return e;
}
#define RESOLVE_CATCH_STACK_MAX 64
typedef struct {
    jmp_buf  jb;
    Term    *catcher;
    Term   **env;
    int      trail_mark;
    void    *cp_mark;
} Resolve_CatchFrame;
static Resolve_CatchFrame g_resolve_catch_stack[RESOLVE_CATCH_STACK_MAX];
static int           g_resolve_catch_top  = 0;
static Term         *g_resolve_exception  = NULL;
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
#define RESOLVE_CP_STACK_MAX 4096
typedef struct {
    jmp_buf     jb;
    Resolve_PredTable *pt;
    const char *key;
    int         arity;
    Trail      *trail;
    int         trail_mark;
    int         next_clause;
    int         cut;
} Resolve_ChoicePoint;
static Resolve_ChoicePoint resolve_cp_stack[RESOLVE_CP_STACK_MAX];
static int            resolve_cp_top = 0;

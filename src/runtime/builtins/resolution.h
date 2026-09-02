#ifndef RESOLUTION_H
#define RESOLUTION_H
#include "ast.h"
#include "../../parsers/snobol4/scrip_cc.h"
#include "../../parsers/prolog/prolog_driver.h"
#include "../../parsers/prolog/term.h"
#include "../../parsers/prolog/prolog_runtime.h"
#include "bb_box.h"
#include "IR.h"
#include "SM.h"
#include "stage2.h"
#define RESOLVE_PRED_TABLE_SIZE_FWD STAGE2_PL_PRED_TABLE_SIZE
#define RESOLVE_SCOPE_SLOT_MAX       64
#define RESOLVE_BB_TABLE_MAX       256
typedef struct Resolve_PredEntry_t {
    const char *key; tree_t *choice; struct Resolve_PredEntry_t *next;
    int entry_pc;
} Resolve_PredEntry;
typedef struct { const char *name; int slot; } PlScopeEnt;
typedef struct { PlScopeEnt e[RESOLVE_SCOPE_SLOT_MAX]; int n; } PlScope;
typedef struct { const char *name; int arity; int bb_idx; PlScope lower_sc; } Resolve_PredEntry_BB;
extern Resolve_PredEntry_BB g_resolve_bb_table[RESOLVE_BB_TABLE_MAX];
extern int             g_resolve_bb_count;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_graph_t *bb_graph_of_pred(const Resolve_PredEntry_BB *e)
{
    if (!e) return NULL;
    if (e->bb_idx >= 0 && e->bb_idx < g_stage2.bbp.count)
        return g_stage2.bbp.table[e->bb_idx];
    return NULL;
}
extern Trail         g_resolve_trail;
extern int           g_resolve_cut_flag;
extern int           g_resolve_active;
typedef enum { RESOLVE_CP_CLAUSE = 0, RESOLVE_CP_DISJ = 1, RESOLVE_CP_RETRY = 2 } resolve_cp_type;
typedef struct resolve_choice {
    resolve_cp_type        type;
    struct resolve_choice *parent;
    int               trail_mark;
    Term            **env;
    void             *resume;
    Term            **saved_args;
    int               cursor;
    int               stamp;
    int               saved_cut_flag;
    struct resolve_choice *saved_cut_barrier;
} resolve_choice;
extern resolve_choice    *g_resolve_bfr;
extern resolve_choice    *g_resolve_cut_barrier;
extern int           g_resolve_cp_stamp;
resolve_choice *resolve_cp_push(resolve_cp_type type, int trail_mark, Term **env, void *resume, int cursor);
resolve_choice *resolve_cp_current(void);
void       resolve_cp_pop(void);
void       resolve_cp_truncate(resolve_choice *barrier);
tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key);
void    resolve_pred_table_insert(Resolve_PredTable *pt, const char *key, tree_t *choice);
tree_t *resolve_pred_table_lookup_global(const char *key);
Resolve_PredEntry *resolve_pred_entry_lookup(const char *key);
Resolve_PredEntry_BB *resolve_bb_lookup(const char *name, int arity);
int         resolve_bb_pred_count(void);
const char *resolve_bb_pred_name_at(int idx);
int         resolve_bb_pred_arity_at(int idx);
IR_graph_t *resolve_bb_graph_at(int idx);
Resolve_PredEntry_BB *resolve_bb_register(const char *name, int arity, int bb_idx);
void resolve_bb_env_push(int nslots);
Term **resolve_bb_env_save_push(int nslots);
void resolve_bb_bind_arg(int slot, void *caller_term);
void resolve_bb_env_pop(Term **saved);
Term **resolve_bb_env_install(Term **env);
Term **rt_env_current(void);
struct __jmp_buf_tag;
struct __jmp_buf_tag *resolve_catch_push(Term *catcher, Term **env);
void                  resolve_catch_pop_top(void);
int                   resolve_throw_term(Term *ball);
Term                 *resolve_catch_take_exception(void);
int                   resolve_catch_top_trail_mark(void);
Term                **resolve_catch_top_env(void);
void                 *resolve_catch_top_cp_mark(void);
void rt_cp_trail_unwind(void);
void rt_cp_inc_cursor(void);
int  rt_cp_get_cursor(void);
#endif

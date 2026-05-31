#ifndef DRIVER_PL_RUNTIME_H
#define DRIVER_PL_RUNTIME_H
#include "../ast/ast.h"
#include "../../frontend/snobol4/scrip_cc.h"
#include "../../frontend/prolog/prolog_driver.h"
#include "../../frontend/prolog/term.h"
#include "../../frontend/prolog/prolog_runtime.h"
#include "bb_broker.h"
#include "BB.h"
#include "SM.h"
#include "stage2.h"
#define RESOLVE_PRED_TABLE_SIZE_FWD STAGE2_PL_PRED_TABLE_SIZE
#define RESOLVE_SCOPE_SLOT_MAX       64
#define RESOLVE_BB_TABLE_MAX       256
typedef struct Resolve_PredEntry_t {
    const char *key; tree_t *choice; struct Resolve_PredEntry_t *next;
    int entry_pc;
} Resolve_PredEntry;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *name; int slot; } PlScopeEnt;
typedef struct { PlScopeEnt e[RESOLVE_SCOPE_SLOT_MAX]; int n; } PlScope;
typedef struct { const char *name; int arity; int bb_idx; PlScope lower_sc; } Resolve_PredEntry_BB;
extern Resolve_PredEntry_BB g_resolve_bb_table[RESOLVE_BB_TABLE_MAX];
extern int             g_resolve_bb_count;
static inline IR_graph_t *bb_graph_of_pred(const Resolve_PredEntry_BB *e)
{
    if (!e) return NULL;
    if (e->bb_idx >= 0 && e->bb_idx < g_stage2.bbp.count)
        return g_stage2.bbp.table[e->bb_idx];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern Trail         g_resolve_trail;
extern int           g_resolve_cut_flag;
extern Term        **g_resolve_env;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
Term  **resolve_env_new(int n);
Term   *resolve_unified_term_from_expr(tree_t *e, Term **env);
int     is_pl_user_call(tree_t *goal);
int     interp_exec_pl_builtin(tree_t *goal, Term **env);
int     resolve_call_term(Term *gt);
int     resolve_call_term_n(Term *gt, int n_extra, Term **extras);
void    resolve_execute_program_unified(CODE_t *prog);
DESCR_t resolve_bb_dcg(void *zeta, int entry);
Resolve_PredEntry_BB *resolve_bb_lookup(const char *name, int arity);
int         resolve_bb_pred_count(void);
const char *resolve_bb_pred_name_at(int idx);
int         resolve_bb_pred_arity_at(int idx);
Resolve_PredEntry_BB *resolve_bb_register(const char *name, int arity, int bb_idx);
bb_node_t resolve_bb_once_proc_by_name(const char *name, int arity);
void resolve_bb_env_push(int nslots);
Term **resolve_bb_env_save_push(int nslots);
void resolve_bb_bind_arg(int slot, void *caller_term);
void resolve_bb_env_pop(Term **saved);
Term **resolve_bb_env_install(Term **env);
Term **rt_pl_env_current(void);
struct __jmp_buf_tag;
struct __jmp_buf_tag *resolve_catch_push(Term *catcher, Term **env);
void                  resolve_catch_pop_top(void);
int                   resolve_throw_term(Term *ball);
Term                 *resolve_catch_take_exception(void);
int                   resolve_catch_top_trail_mark(void);
Term                **resolve_catch_top_env(void);
#endif

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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void rt_trail_unwind(int mark);
void rt_cp_trail_unwind(void) { resolve_choice *cp = resolve_cp_current(); if (cp) rt_trail_unwind(cp->trail_mark); }
void rt_cp_inc_cursor(void)   { resolve_choice *cp = resolve_cp_current(); if (cp) cp->cursor++; }
int  rt_cp_get_cursor(void)   { resolve_choice *cp = resolve_cp_current(); return cp ? cp->cursor : 0; }
#include "../../interp/IR_interp.h"
extern tree_t *pl_assert_term(Term *t, int *functor_out, int *arity_out);
#include "gen_value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
Trail         g_resolve_trail;
int           g_resolve_cut_flag = 0;
Term        **g_resolve_env      = NULL;
int           g_resolve_active   = 0;
resolve_choice    *g_resolve_bfr      = NULL;
resolve_choice    *g_resolve_cut_barrier = NULL;
int           g_resolve_cp_stamp = 0;
resolve_choice *resolve_cp_push(resolve_cp_type type, int trail_mark, Term **env, void *resume, int cursor)
{
    resolve_choice *cp = (resolve_choice *) calloc(1, sizeof(resolve_choice));
    cp->type = type; cp->parent = g_resolve_bfr; cp->trail_mark = trail_mark; cp->env = env; cp->resume = resume; cp->saved_args = NULL; cp->cursor = cursor; cp->stamp = ++g_resolve_cp_stamp;
    g_resolve_bfr = cp;
    return cp;
}
/*--------------------------------------------------------------------------------------------------------------------*/
resolve_choice *resolve_cp_current(void)
{
    return g_resolve_bfr;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void resolve_cp_pop(void)
{
    resolve_choice *cp = g_resolve_bfr;
    if (!cp) return;
    g_resolve_bfr = cp->parent;
    free(cp);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void resolve_cp_truncate(resolve_choice *barrier)
{
    while (g_resolve_bfr && g_resolve_bfr != barrier) { resolve_choice *cp = g_resolve_bfr; g_resolve_bfr = cp->parent; free(cp); }
}
/*--------------------------------------------------------------------------------------------------------------------*/
Resolve_PredEntry_BB g_resolve_bb_table[RESOLVE_BB_TABLE_MAX];
int             g_resolve_bb_count = 0;
typedef struct { IR_graph_t *cfg; int first; } resolve_dcg_state_t;
/*--------------------------------------------------------------------------------------------------------------------*/
Resolve_PredEntry_BB *resolve_bb_lookup(const char *name, int arity) {
    if (!name) return NULL;
    for (int i = 0; i < g_resolve_bb_count; i++)
        if (g_resolve_bb_table[i].arity == arity && g_resolve_bb_table[i].name && strcmp(g_resolve_bb_table[i].name, name) == 0)
            return &g_resolve_bb_table[i];
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t *resolve_bb_entry_node(const char *name, int arity) {
    Resolve_PredEntry_BB *e = resolve_bb_lookup(name, arity);
    if (!e) return NULL;
    IR_graph_t *cfg = bb_graph_of_pred(e);
    return cfg ? cfg->entry : NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int resolve_bb_pred_count(void) { return g_resolve_bb_count; }
const char *resolve_bb_pred_name_at(int idx) {
    if (idx < 0 || idx >= g_resolve_bb_count) return NULL;
    return bb_graph_of_pred(&g_resolve_bb_table[idx]) ? g_resolve_bb_table[idx].name : NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int resolve_bb_pred_arity_at(int idx) {
    if (idx < 0 || idx >= g_resolve_bb_count) return 0;
    return g_resolve_bb_table[idx].arity;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *resolve_bb_graph_at(int idx) {
    if (idx < 0 || idx >= g_resolve_bb_count) return NULL;
    return bb_graph_of_pred(&g_resolve_bb_table[idx]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
bb_node_t resolve_bb_once_proc_by_name(const char *name, int arity) {
    fprintf(stderr, "[PL] FATAL: resolve_bb_once_proc_by_name: brokered Prolog path removed\n");
    abort();
    (void)name; (void)arity;
    return (bb_node_t){ NULL, NULL, 0 };
}
/*--------------------------------------------------------------------------------------------------------------------*/
void resolve_bb_env_push(int nslots) {
    int n = nslots > 0 ? nslots : 16;
    g_resolve_env = calloc((size_t)n, sizeof(Term *));
}
/*--------------------------------------------------------------------------------------------------------------------*/
Term **resolve_bb_env_save_push(int nslots) {
    Term **saved = g_resolve_env;
    int n = nslots > 0 ? nslots : 16;
    g_resolve_env = calloc((size_t)n, sizeof(Term *));
    return saved;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void resolve_bb_bind_arg(int slot, void *caller_term) {
    extern Trail g_resolve_trail;
    if (!g_resolve_env || slot < 0) return;
    Term *at = term_new_var(slot);
    g_resolve_env[slot] = at;
    if (caller_term) unify(at, (Term *)caller_term, &g_resolve_trail);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void resolve_bb_env_pop(Term **saved) { free(g_resolve_env); g_resolve_env = saved; }
#define RESOLVE_NB_STORE_SIZE 64
typedef struct { char *key; Term *val; } Resolve_NbEntry;
static Resolve_NbEntry g_resolve_nb_store[RESOLVE_NB_STORE_SIZE];
static int        g_resolve_nb_count = 0;
static void resolve_nb_setval(const char *key, Term *val) {
    for (int i = 0; i < g_resolve_nb_count; i++) {
        if (strcmp(g_resolve_nb_store[i].key, key) == 0) {
            g_resolve_nb_store[i].val = val; return;
        }
    }
    if (g_resolve_nb_count < RESOLVE_NB_STORE_SIZE) {
        g_resolve_nb_store[g_resolve_nb_count].key = strdup(key);
        g_resolve_nb_store[g_resolve_nb_count].val = val;
        g_resolve_nb_count++;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static Term *resolve_nb_getval(const char *key) {
    for (int i = 0; i < g_resolve_nb_count; i++)
        if (strcmp(g_resolve_nb_store[i].key, key) == 0)
            return g_resolve_nb_store[i].val;
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define RESOLVE_CATCH_STACK_MAX 64
typedef struct {
    jmp_buf  jb;
    Term    *catcher;
    Term   **env;
    int      trail_mark;
    void    *cp_mark;
} Resolve_CatchFrame;
/*--------------------------------------------------------------------------------------------------------------------*/
static Resolve_CatchFrame g_resolve_catch_stack[RESOLVE_CATCH_STACK_MAX];
static int           g_resolve_catch_top  = 0;
static Term         *g_resolve_exception  = NULL;
static int resolve_throw_iso_error(Term *err_term) {
    Term *args2[2]; args2[0] = err_term; args2[1] = term_new_atom(prolog_atom_intern("context"));
    Term *err = term_new_compound(prolog_atom_intern("error"), 2, args2);
    g_resolve_exception = err;
    for (int i = g_resolve_catch_top - 1; i >= 0; i--) {
        Resolve_CatchFrame *cf = &g_resolve_catch_stack[i];
        Trail tmptrail; trail_init(&tmptrail);
        int tmmark = trail_mark(&tmptrail);
        int matched = unify(cf->catcher, err, &tmptrail);
        trail_unwind(&tmptrail, tmmark);
        if (matched) { g_resolve_catch_top = i + 1; longjmp(cf->jb, 1); }
    }
    fprintf(stderr, "ERROR: Unhandled exception: ");
    pl_write(err); fprintf(stderr, "\n");
    exit(1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int resolve_throw_instantiation_error(void) {
    Term *e = term_new_atom(prolog_atom_intern("instantiation_error"));
    return resolve_throw_iso_error(e);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int resolve_throw_type_error_evaluable(const char *name, int arity) {
    Term *na_args[2]; na_args[0] = term_new_atom(prolog_atom_intern(name)); na_args[1] = term_new_int(arity);
    Term *na = term_new_compound(prolog_atom_intern("/"), 2, na_args);
    Term *te_args[2]; te_args[0] = term_new_atom(prolog_atom_intern("evaluable")); te_args[1] = na;
    Term *te = term_new_compound(prolog_atom_intern("type_error"), 2, te_args);
    return resolve_throw_iso_error(te);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int resolve_throw_existence_error_procedure(const char *name, int arity) {
    Term *na_args[2]; na_args[0] = term_new_atom(prolog_atom_intern(name)); na_args[1] = term_new_int(arity);
    Term *na = term_new_compound(prolog_atom_intern("/"), 2, na_args);
    Term *te_args[2]; te_args[0] = term_new_atom(prolog_atom_intern("procedure")); te_args[1] = na;
    Term *te = term_new_compound(prolog_atom_intern("existence_error"), 2, te_args);
    return resolve_throw_iso_error(te);
}
/*--------------------------------------------------------------------------------------------------------------------*/
struct __jmp_buf_tag *resolve_catch_push(Term *catcher, Term **env)
{
    if (g_resolve_catch_top >= RESOLVE_CATCH_STACK_MAX) return NULL;
    Resolve_CatchFrame *cf = &g_resolve_catch_stack[g_resolve_catch_top];
    cf->catcher    = catcher;
    cf->env        = env;
    cf->trail_mark = trail_mark(&g_resolve_trail);
    cf->cp_mark    = (void *)resolve_cp_current();
    g_resolve_catch_top++;
    return cf->jb;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void resolve_catch_pop_top(void)
{
    if (g_resolve_catch_top > 0) g_resolve_catch_top--;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int resolve_catch_top_trail_mark(void)
{
    if (g_resolve_catch_top <= 0) return 0;
    return g_resolve_catch_stack[g_resolve_catch_top - 1].trail_mark;
}
/*--------------------------------------------------------------------------------------------------------------------*/
Term **resolve_catch_top_env(void)
{
    if (g_resolve_catch_top <= 0) return NULL;
    return g_resolve_catch_stack[g_resolve_catch_top - 1].env;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void *resolve_catch_top_cp_mark(void)
{
    if (g_resolve_catch_top <= 0) return NULL;
    return g_resolve_catch_stack[g_resolve_catch_top - 1].cp_mark;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int resolve_throw_term(Term *ball)
{
    g_resolve_exception = ball;
    while (g_resolve_catch_top > 0) {
        int ci = g_resolve_catch_top - 1;
        Resolve_CatchFrame *cf = &g_resolve_catch_stack[ci];
        Trail tmptrail; trail_init(&tmptrail);
        int tmmark = trail_mark(&tmptrail);
        int matched = unify(cf->catcher, ball, &tmptrail);
        trail_unwind(&tmptrail, tmmark);
        if (matched) longjmp(cf->jb, 1);
        g_resolve_catch_top--;
    }
    fprintf(stderr, "ERROR: Unhandled exception: ");
    pl_write(ball); fprintf(stderr, "\n");
    exit(1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
Term *resolve_catch_take_exception(void)
{
    Term *e = g_resolve_exception;
    g_resolve_exception = NULL;
    return e;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define RESOLVE_PRED_TABLE_SIZE RESOLVE_PRED_TABLE_SIZE_FWD
unsigned resolve_pred_hash(const char *s) {
    unsigned h = 5381;
    while (*s) h = h * 33 ^ (unsigned char)*s++;
    return h % RESOLVE_PRED_TABLE_SIZE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void resolve_pred_table_insert(Resolve_PredTable *pt, const char *key, tree_t *choice) {
    unsigned h = resolve_pred_hash(key);
    Resolve_PredEntry *e = malloc(sizeof(Resolve_PredEntry));
    e->key = key; e->choice = choice; e->entry_pc = -1; e->next = pt->buckets[h]; pt->buckets[h] = e;
}
/*--------------------------------------------------------------------------------------------------------------------*/
tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key) {
    for (Resolve_PredEntry *e = pt->buckets[resolve_pred_hash(key)]; e; e = e->next)
        if (strcmp(e->key, key) == 0) return e->choice;
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
tree_t *resolve_pred_table_lookup_global(const char *key) {
    return resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
}
/*--------------------------------------------------------------------------------------------------------------------*/
Resolve_PredEntry *resolve_pred_entry_lookup(const char *key) {
    for (Resolve_PredEntry *e = g_stage2.resolve_pred_table.buckets[resolve_pred_hash(key)]; e; e = e->next)
        if (e->key && strcmp(e->key, key) == 0) return e;
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *resolve_pred_table_get_or_create_choice(const char *key) {
    tree_t *ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
    if (ch) return ch;
    ch = ast_node_new(TT_CHOICE);
    ch->v.sval = strdup(key);
    resolve_pred_table_insert(&g_stage2.resolve_pred_table, ch->v.sval, ch);
    return ch;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int resolve_assert_clause(Term *t, int end) {
    int functor_id = -1, arity = 0;
    tree_t *ec = pl_assert_term(t, &functor_id, &arity);
    if (!ec) return 0;
    const char *fname = prolog_atom_name(functor_id);
    if (!fname) return 0;
    char key[256];
    snprintf(key, sizeof key, "%s/%d", fname, arity);
    tree_t *ch = resolve_pred_table_get_or_create_choice(key);
    if (end) {
        expr_add_child(ch, ec);
    } else {
        expr_add_child(ch, ec);
        if (ch->n > 1) {
            memmove(&ch->c[1], &ch->c[0], (ch->n - 1) * sizeof(tree_t *));
            ch->c[0] = ec;
        }
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int resolve_retract_clause(Term *t) {
    if (!t) return 0;
    t = term_deref(t);
    Term *head = t;
    if (t->tag == TERM_COMPOUND && t->compound.arity == 2) {
        const char *fn = prolog_atom_name(t->compound.functor);
        if (fn && strcmp(fn, ":-") == 0) head = term_deref(t->compound.args[0]);
    }
    const char *fname = NULL;
    int arity = 0;
    if (head->tag == TERM_ATOM) {
        fname = prolog_atom_name(head->atom_id); arity = 0;
    } else if (head->tag == TERM_COMPOUND) {
        fname = prolog_atom_name(head->compound.functor); arity = head->compound.arity;
    }
    if (!fname) return 0;
    char key[256]; snprintf(key, sizeof key, "%s/%d", fname, arity);
    tree_t *ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
    if (!ch || ch->n == 0) return 0;
    free(ch->c[0]);
    memmove(&ch->c[0], &ch->c[1], (ch->n - 1) * sizeof(tree_t *));
    ch->n--;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int resolve_abolish_pred(Term *t) {
    if (!t) return 0;
    t = term_deref(t);
    const char *fname = NULL; int arity = 0;
    if (t->tag == TERM_COMPOUND && t->compound.arity == 2) {
        const char *fn = prolog_atom_name(t->compound.functor);
        if (fn && strcmp(fn, "/") == 0) {
            Term *na = term_deref(t->compound.args[0]);
            Term *ar = term_deref(t->compound.args[1]);
            if (na && na->tag == TERM_ATOM) fname = prolog_atom_name(na->atom_id);
            if (ar && ar->tag == TERM_INT)  arity = (int)ar->ival;
        }
    } else if (t->tag == TERM_ATOM) {
        fname = prolog_atom_name(t->atom_id); arity = 0;
    }
    if (!fname) return 0;
    char key[256]; snprintf(key, sizeof key, "%s/%d", fname, arity);
    tree_t *ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
    if (!ch) return 1;
    ch->n = 0;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static Resolve_ChoicePoint resolve_cp_stack[RESOLVE_CP_STACK_MAX];
static int            resolve_cp_top = 0;
Term **resolve_env_new(int n) {
    if (n <= 0) return NULL;
    Term **env = malloc(n * sizeof(Term *));
    for (int i = 0; i < n; i++) env[i] = term_new_var(i);
    return env;
}
/*--------------------------------------------------------------------------------------------------------------------*/

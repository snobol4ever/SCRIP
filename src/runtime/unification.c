#include "rt/rt.h"
#include "core.h"
#include "builtins/resolution.h"
#include "bb_pool.h"
#include "../parser/prolog/prolog_atom.h"
#include <stdio.h>
/*====================================================================================================================*/
void *rt_node_to_term(int kind, long ival, const char *sval, double dval)
{
    extern Term **g_resolve_env;
    switch (kind) {
    case IR_LOGICVAR: {
        int slot = (int)ival;
        Term *t = (g_resolve_env && slot >= 0 && g_resolve_env[slot]) ? term_deref(g_resolve_env[slot]) : NULL;
        if (!t) { t = term_new_var(slot); if (g_resolve_env && slot >= 0) g_resolve_env[slot] = t; }
        return t;
    }
    case IR_ATOM:  return term_new_atom(prolog_atom_intern(sval ? sval : "[]"));
    case IR_LIT_F: return term_new_float(dval);
    case IR_LIT_I: return term_new_int(ival);
    default:       return term_new_int(ival);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unify_terms(void *l, void *r)
{
    extern Trail g_resolve_trail;
    Term *lt = (Term *)l, *rt_ = (Term *)r;
    if (!lt || !rt_) return 0;
    int mark = trail_mark(&g_resolve_trail);
    if (!unify(lt, rt_, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unify_const(int slot, int kind, long ival, const char *sval, double dval)
{
    extern Term **g_resolve_env;
    Term *vt = (g_resolve_env && slot >= 0 && g_resolve_env[slot]) ? term_deref(g_resolve_env[slot]) : (Term *)0;
    if (!vt) { vt = term_new_var(slot); if (g_resolve_env && slot >= 0) g_resolve_env[slot] = vt; }
    if (vt->tag == TERM_VAR) return rt_unify_terms(vt, rt_node_to_term(kind, ival, sval, dval));
    switch (kind) {
    case IR_ATOM:  return (vt->tag == TERM_ATOM  && vt->atom_id == prolog_atom_intern(sval ? sval : "[]")) ? 1 : 0;
    case IR_LIT_I: return (vt->tag == TERM_INT   && vt->ival == ival) ? 1 : 0;
    case IR_LIT_F: return (vt->tag == TERM_FLOAT && vt->fval == dval) ? 1 : 0;
    default:       return (vt->tag == TERM_INT   && vt->ival == ival) ? 1 : 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unify_var_var(int lslot, int rslot)
{
    extern Term **g_resolve_env;
    Term *lt = (g_resolve_env && lslot >= 0 && g_resolve_env[lslot]) ? term_deref(g_resolve_env[lslot]) : (Term *)0;
    if (!lt) { lt = term_new_var(lslot); if (g_resolve_env && lslot >= 0) g_resolve_env[lslot] = lt; }
    Term *rt_ = (g_resolve_env && rslot >= 0 && g_resolve_env[rslot]) ? term_deref(g_resolve_env[rslot]) : (Term *)0;
    if (!rt_) { rt_ = term_new_var(rslot); if (g_resolve_env && rslot >= 0) g_resolve_env[rslot] = rt_; }
    return rt_unify_terms(lt, rt_);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_trail_mark(void)
{
    extern Trail g_resolve_trail;
    return trail_mark(&g_resolve_trail);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_trail_unwind(int mark)
{
    extern Trail g_resolve_trail;
    trail_unwind(&g_resolve_trail, mark);
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define RT_MARK_STACK_MAX 32
static int g_resolve_mark_stack[RT_MARK_STACK_MAX];
static int g_resolve_mark_top = 0;
void rt_trail_mark_push(void)
{
    extern Trail g_resolve_trail;
    int m = trail_mark(&g_resolve_trail);
    if (g_resolve_mark_top < RT_MARK_STACK_MAX) g_resolve_mark_stack[g_resolve_mark_top++] = m;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_trail_unwind_top(void)
{
    extern Trail g_resolve_trail;
    if (g_resolve_mark_top <= 0) return;
    int m = g_resolve_mark_stack[g_resolve_mark_top - 1];
    trail_unwind(&g_resolve_trail, m);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_trail_mark_pop(void)
{
    if (g_resolve_mark_top > 0) g_resolve_mark_top--;
}
/*--------------------------------------------------------------------------------------------------------------------*/
Term **rt_env_current(void) { extern Term **g_resolve_env; return g_resolve_env; }
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_env_alloc(int nslots)
{
    extern Term **g_resolve_env;
    extern Trail  g_resolve_trail;
    int n = (nslots > 0 ? nslots : 1) + 8;
    g_resolve_env = (Term **)GC_MALLOC((size_t)n * sizeof(Term *));
    trail_init(&g_resolve_trail);
}
void rt_cp_save_caller_env(void *caller_env) { if (g_resolve_bfr) g_resolve_bfr->saved_args = (Term **)caller_env; }
void rt_choice_cut_enter(void *cp_void)
{
    resolve_choice *cp = (resolve_choice *)cp_void;
    if (!cp) return;
    cp->saved_cut_flag    = g_resolve_cut_flag;
    cp->saved_cut_barrier = g_resolve_cut_barrier;
    g_resolve_cut_flag         = 0;
    g_resolve_cut_barrier      = cp->parent;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_choice_cut_exit(void *cp_void)
{
    resolve_choice *cp = (resolve_choice *)cp_void;
    if (!cp) return;
    g_resolve_cut_flag    = cp->saved_cut_flag;
    g_resolve_cut_barrier = cp->saved_cut_barrier;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_choice_cut_unwind(void *cp_void)
{
    resolve_choice *cp = (resolve_choice *)cp_void;
    if (!cp) return;
    resolve_choice *parent = cp->parent;
    g_resolve_cut_flag    = cp->saved_cut_flag;
    g_resolve_cut_barrier = cp->saved_cut_barrier;
    resolve_cp_truncate(parent);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_get_cut_flag(void)
{
    return g_resolve_cut_flag;
}
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *name; long arity; void *alpha; void *redo; } pl_pred_row_t;
static pl_pred_row_t *g_pl_pred_table = (pl_pred_row_t *)0;
static long           g_pl_pred_n     = 0;
static void          *g_pl_meta_redo  = (void *)0;
void rt_pl_table_install(void *tbl, long n)
{
    g_pl_pred_table = (pl_pred_row_t *)tbl;
    g_pl_pred_n     = n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_pred_lookup(const char *name, int arity, void **redo_out)
{
    if (!g_pl_pred_table || !name) return (void *)0;
    for (long i = 0; i < g_pl_pred_n; i++) {
        pl_pred_row_t *r = &g_pl_pred_table[i];
        if (r->arity == (long)arity && r->name && strcmp(r->name, name) == 0) { if (redo_out) *redo_out = r->redo; return r->alpha; }
    }
    return (void *)0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern const char *prolog_atom_name(int id);
extern int    rt_last_ok(void);
int rt_call_term(void *goal_v)
{
    Term *g = term_deref((Term *)goal_v);
    if (!g) return 0;
    const char *name = (const char *)0; int arity = 0; Term **args = (Term **)0;
    if (g->tag == TERM_ATOM)          { name = prolog_atom_name(g->atom_id); }
    else if (g->tag == TERM_COMPOUND) { name = prolog_atom_name(g->compound.functor); arity = g->compound.arity; args = g->compound.args; }
    else return 0;
    if (!name) return 0;
    if (strcmp(name, "true") == 0) return 1;
    if (strcmp(name, "fail") == 0 || strcmp(name, "false") == 0) return 0;
    void *redo = (void *)0;
    void *alpha = rt_pl_pred_lookup(name, arity, &redo);
    if (!alpha) { fprintf(stderr, "rt_call_term: unknown predicate %s/%d\n", name, arity); return 0; }
    g_pl_meta_redo = redo;
    Term **caller_env = resolve_bb_env_save_push(arity + 16);
    for (int i = 0; i < arity; i++) resolve_bb_bind_arg(i, args ? (void *)args[i] : (void *)0);
    ((void (*)(void))alpha)();
    if (rt_last_ok()) { Term **cur = resolve_bb_env_install(caller_env); rt_cp_save_caller_env((void *)cur); return 1; }
    resolve_bb_env_pop(caller_env);
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_redo_meta(void *entry_cp_v)
{
    resolve_choice *cp = resolve_cp_current();
    if (!cp || cp == (resolve_choice *)entry_cp_v || !g_pl_meta_redo) return 0;
    resolve_bb_env_install(cp->env);
    ((void (*)(void))g_pl_meta_redo)();
    if (rt_last_ok()) { cp = resolve_cp_current(); if (cp && cp != (resolve_choice *)entry_cp_v) resolve_bb_env_install(cp->saved_args); return 1; }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/

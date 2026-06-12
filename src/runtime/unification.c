#include "rt/rt.h"
#include "core.h"
#include "builtins/resolution.h"
#include "bb_pool.h"
#include "../parser/prolog/prolog_atom.h"
#include "../contracts/IR.h"
#include <stdio.h>
#include <unistd.h>
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
void rt_pl_cells_init(void **cells, int n)
{
    for (int i = 0; i < n; i++) cells[i] = term_new_var(i);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void * rt_enter(void **slot, int nslots)
{
    extern void *GC_malloc(size_t);
    if (!*slot) *slot = GC_malloc((size_t)(8 + 8 * (nslots > 0 ? nslots : 1)));
    return *slot;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_unify_cell_const(void *cell_term, int kind, long ival, const char *sval)
{
    return rt_unify_terms(cell_term, rt_node_to_term(kind, ival, sval, 0.0));
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_unify_cell_float(void *cell_term, double dval)
{
    return rt_unify_terms(cell_term, rt_node_to_term(IR_LIT_F, 0, (const char *)0, dval));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static Term *pl_build_term_gz_r(void *frame, const void *ir_node)
{
    extern void *GC_malloc(size_t);
    const IR_t *nd = (const IR_t *)ir_node;
    if (!nd) return (Term *)0;
    switch (nd->op) {
    case IR_LOGICVAR: {
        int slot = (int)IR_LIT(nd).ival;
        if (slot < 0) return (Term *)0;
        Term **cells = (Term **)(((char *)frame) + 8);
        return cells[slot] ? term_deref(cells[slot]) : (Term *)0;
    }
    case IR_ATOM:  return term_new_atom(prolog_atom_intern(IR_LIT(nd).sval ? IR_LIT(nd).sval : "[]"));
    case IR_LIT_I: return term_new_int((long)IR_LIT(nd).ival);
    case IR_LIT_F: return term_new_float(IR_LIT(nd).dval);
    case IR_STRUCT: {
        int arity = (int)IR_LIT(nd).ival;
        if (arity <= 0) return term_new_atom(prolog_atom_intern(IR_LIT(nd).sval ? IR_LIT(nd).sval : "[]"));
        Term **args = (Term **)GC_malloc((size_t)arity * sizeof(Term *));
        if (!args) return (Term *)0;
        for (int i = 0; i < arity; i++) { const IR_t *a = ir_call_arg(nd, i); if (!a) break; args[i] = pl_build_term_gz_r(frame, a); }
        return term_new_compound(prolog_atom_intern(IR_LIT(nd).sval ? IR_LIT(nd).sval : "[]"), arity, args);
    }
    default: return (Term *)0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_unify_struct_gz(void *frame, const void *lnd, const void *rnd)
{
    extern Trail g_resolve_trail;
    Term *lt = pl_build_term_gz_r(frame, lnd);
    Term *rt_ = pl_build_term_gz_r(frame, rnd);
    if (!lt || !rt_) return 0;
    int mark = trail_mark(&g_resolve_trail);
    if (!unify(lt, rt_, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cell(void *cell_term)
{
    extern void pl_write(Term *);
    pl_write(term_deref((Term *)cell_term));
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pl_writeq_cell(void *cell_term)
{
    extern void pl_writeq(Term *);
    pl_writeq(term_deref((Term *)cell_term));
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_canonical_cell(void *cell_term)
{
    extern void pl_write_canonical(Term *);
    pl_write_canonical(term_deref((Term *)cell_term));
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_trail_mark(void)
{
    extern Trail g_resolve_trail;
    extern void trail_init(Trail *t);
    if (!g_resolve_trail.stack || g_resolve_trail.capacity <= 0) trail_init(&g_resolve_trail);
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
extern long   rt_arith(int lk, long li, const char *ls, int rk, long ri, const char *rs, const char *op);
enum { MK_TRUE, MK_FAIL, MK_PRED, MK_BUILTIN, MK_CONJ, MK_DISJ };
typedef struct meta_fr {
    int kind; Term *goal; const char *name; int arity; Term **args;
    void *alpha; void *redo; resolve_choice *mark;
    struct meta_fr **kids; int nkids; int cur;
    resolve_choice *br_cp; int br_trail;
} meta_fr;
typedef struct { meta_fr *fr; Term **E; } meta_root;
/*--------------------------------------------------------------------------------------------------------------------*/
static meta_fr *meta_fr_new(int kind) { meta_fr *f = (meta_fr *)GC_MALLOC(sizeof *f); if (f) f->kind = kind; return f; }
static const char *g_meta_builtins[] = { "is", "=:=", "=\\=", "<", ">", "=<", ">=", "=", "\\=", (const char *)0 };
static meta_fr *meta_compile(Term *g0)
{
    Term *g = term_deref(g0);
    if (!g) return (meta_fr *)0;
    if (g->tag == TERM_VAR) { fprintf(stderr, "rt_call_term: unbound goal\n"); return (meta_fr *)0; }
    const char *name = (const char *)0; int arity = 0; Term **args = (Term **)0;
    if (g->tag == TERM_ATOM)          { name = prolog_atom_name(g->atom_id); }
    else if (g->tag == TERM_COMPOUND) { name = prolog_atom_name(g->compound.functor); arity = g->compound.arity; args = g->compound.args; }
    else { fprintf(stderr, "rt_call_term: non-callable goal\n"); return (meta_fr *)0; }
    if (!name) return (meta_fr *)0;
    if (arity == 0 && strcmp(name, "true") == 0) return meta_fr_new(MK_TRUE);
    if (arity == 0 && (strcmp(name, "fail") == 0 || strcmp(name, "false") == 0)) return meta_fr_new(MK_FAIL);
    if (arity == 0 && strcmp(name, "!") == 0) { fprintf(stderr, "rt_call_term: cut in meta-call not yet supported\n"); return (meta_fr *)0; }
    if (arity == 1 && strcmp(name, "call") == 0) return meta_compile(args[0]);
    if (arity == 2 && (strcmp(name, ",") == 0 || strcmp(name, ";") == 0)) {
        if (name[0] == ';') {
            Term *l = term_deref(args[0]);
            if (l && l->tag == TERM_COMPOUND && l->compound.arity == 2) {
                const char *ln = prolog_atom_name(l->compound.functor);
                if (ln && (strcmp(ln, "->") == 0 || strcmp(ln, "*->") == 0)) { fprintf(stderr, "rt_call_term: if-then-else in meta-call not yet supported\n"); return (meta_fr *)0; }
            }
        }
        meta_fr *f = meta_fr_new(name[0] == ',' ? MK_CONJ : MK_DISJ); if (!f) return (meta_fr *)0;
        f->kids = (meta_fr **)GC_MALLOC(2 * sizeof(meta_fr *)); if (!f->kids) return (meta_fr *)0;
        f->kids[0] = meta_compile(args[0]); f->kids[1] = meta_compile(args[1]);
        if (!f->kids[0] || !f->kids[1]) return (meta_fr *)0;
        f->nkids = 2; return f;
    }
    if ((arity == 2 && strcmp(name, "->") == 0) || (arity == 1 && (strcmp(name, "\\+") == 0 || strcmp(name, "not") == 0))
     || (arity == 3 && strcmp(name, "catch") == 0) || (arity == 1 && strcmp(name, "throw") == 0)) {
        fprintf(stderr, "rt_call_term: %s/%d in meta-call not yet supported\n", name, arity); return (meta_fr *)0;
    }
    for (int k = 0; g_meta_builtins[k]; k++)
        if (arity == 2 && strcmp(name, g_meta_builtins[k]) == 0) {
            meta_fr *f = meta_fr_new(MK_BUILTIN); if (!f) return (meta_fr *)0;
            f->goal = g; f->name = name; f->arity = arity; f->args = args; return f;
        }
    meta_fr *f = meta_fr_new(MK_PRED); if (!f) return (meta_fr *)0;
    f->goal = g; f->name = name; f->arity = arity; f->args = args; return f;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int meta_arith(Term *e0, long *iv, double *dv, int *isf)
{
    Term *e = term_deref(e0);
    if (!e) return 0;
    if (e->tag == TERM_INT)   { *iv = e->ival; *isf = 0; return 1; }
    if (e->tag == TERM_FLOAT) { *dv = e->fval; *isf = 1; return 1; }
    if (e->tag == TERM_VAR)   { fprintf(stderr, "is/2: arguments are not sufficiently instantiated\n"); return 0; }
    const char *op = (const char *)0; int n = 0; Term **a = (Term **)0;
    if (e->tag == TERM_COMPOUND) { op = prolog_atom_name(e->compound.functor); n = e->compound.arity; a = e->compound.args; }
    else if (e->tag == TERM_ATOM) { op = prolog_atom_name(e->atom_id); }
    if (!op) return 0;
    if (n == 0) {
        if (strcmp(op, "pi") == 0) { *dv = 3.14159265358979323846; *isf = 1; return 1; }
        if (strcmp(op, "e")  == 0) { *dv = 2.71828182845904523536; *isf = 1; return 1; }
        fprintf(stderr, "is/2: unknown evaluable %s/0\n", op); return 0;
    }
    long li = 0, ri = 0; double ld = 0, rd = 0; int lf = 0, rf = 0;
    if (!meta_arith(a[0], &li, &ld, &lf)) return 0;
    if (n == 1) {
        if (lf) {
            if (strcmp(op, "-")   == 0) { *dv = -ld; *isf = 1; return 1; }
            if (strcmp(op, "+")   == 0) { *dv =  ld; *isf = 1; return 1; }
            if (strcmp(op, "abs") == 0) { *dv = (ld < 0) ? -ld : ld; *isf = 1; return 1; }
            fprintf(stderr, "is/2: unknown float evaluable %s/1\n", op); return 0;
        }
        if (strcmp(op, "-") == 0) { *iv = -li; *isf = 0; return 1; }
        if (strcmp(op, "+") == 0) { *iv =  li; *isf = 0; return 1; }
        *iv = rt_arith(IR_LIT_I, li, (const char *)0, IR_LIT_I, 0, (const char *)0, op); *isf = 0; return 1;
    }
    if (n != 2 || !meta_arith(a[1], &ri, &rd, &rf)) return 0;
    if (lf || rf) {
        double x = lf ? ld : (double)li, y = rf ? rd : (double)ri;
        if (strcmp(op, "+")   == 0) { *dv = x + y; *isf = 1; return 1; }
        if (strcmp(op, "-")   == 0) { *dv = x - y; *isf = 1; return 1; }
        if (strcmp(op, "*")   == 0) { *dv = x * y; *isf = 1; return 1; }
        if (strcmp(op, "/")   == 0) { *dv = (y != 0.0) ? x / y : 0.0; *isf = 1; return 1; }
        if (strcmp(op, "min") == 0) { *dv = (x < y) ? x : y; *isf = 1; return 1; }
        if (strcmp(op, "max") == 0) { *dv = (x > y) ? x : y; *isf = 1; return 1; }
        fprintf(stderr, "is/2: unknown float evaluable %s/2\n", op); return 0;
    }
    *iv = rt_arith(IR_LIT_I, li, (const char *)0, IR_LIT_I, ri, (const char *)0, op); *isf = 0; return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int meta_builtin_solve(meta_fr *f)
{
    extern Trail g_resolve_trail;
    const char *fn = f->name;
    if (strcmp(fn, "=") == 0) return rt_unify_terms(term_deref(f->args[0]), term_deref(f->args[1]));
    if (strcmp(fn, "\\=") == 0) {
        int mark = trail_mark(&g_resolve_trail);
        int ok = unify(term_deref(f->args[0]), term_deref(f->args[1]), &g_resolve_trail);
        trail_unwind(&g_resolve_trail, mark);
        return !ok;
    }
    long li = 0, ri = 0; double ld = 0, rd = 0; int lf = 0, rf = 0;
    if (strcmp(fn, "is") == 0) {
        if (!meta_arith(f->args[1], &ri, &rd, &rf)) return 0;
        return rt_unify_terms(term_deref(f->args[0]), rf ? term_new_float(rd) : term_new_int(ri));
    }
    if (!meta_arith(f->args[0], &li, &ld, &lf)) return 0;
    if (!meta_arith(f->args[1], &ri, &rd, &rf)) return 0;
    double x = lf ? ld : (double)li, y = rf ? rd : (double)ri;
    int c = (x < y) ? -1 : (x > y) ? 1 : 0;
    if (strcmp(fn, "=:=") == 0)  return c == 0;
    if (strcmp(fn, "=\\=") == 0) return c != 0;
    if (strcmp(fn, "<") == 0)    return c < 0;
    if (strcmp(fn, ">") == 0)    return c > 0;
    if (strcmp(fn, "=<") == 0)   return c <= 0;
    if (strcmp(fn, ">=") == 0)   return c >= 0;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int meta_pred_solve(meta_fr *f, Term **E)
{
    resolve_bb_env_install(E);
    void *redo = (void *)0;
    void *alpha = rt_pl_pred_lookup(f->name, f->arity, &redo);
    if (!alpha) { fprintf(stderr, "rt_call_term: unknown predicate %s/%d\n", f->name, f->arity); return 0; }
    f->alpha = alpha; f->redo = redo;
    f->mark = resolve_cp_current();
    Term **caller_env = resolve_bb_env_save_push(f->arity + 16);
    for (int i = 0; i < f->arity; i++) resolve_bb_bind_arg(i, f->args ? (void *)f->args[i] : (void *)0);
    ((void (*)(void))alpha)();
    if (rt_last_ok()) { Term **cur = resolve_bb_env_install(caller_env); rt_cp_save_caller_env((void *)cur); return 1; }
    resolve_bb_env_pop(caller_env);
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int meta_pred_redo(meta_fr *f, Term **E)
{
    resolve_choice *cp = resolve_cp_current();
    if (!cp || cp == f->mark || !f->redo) { resolve_bb_env_install(E); return 0; }
    resolve_bb_env_install(cp->env);
    ((void (*)(void))f->redo)();
    if (rt_last_ok()) {
        cp = resolve_cp_current();
        if (cp && cp != f->mark) resolve_bb_env_install(cp->saved_args);
        else resolve_bb_env_install(E);
        return 1;
    }
    resolve_bb_env_install(E);
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int meta_solve(meta_fr *f, Term **E);
static int meta_redo(meta_fr *f, Term **E);
static int meta_conj_drive(meta_fr *f, Term **E, int i, int fresh)
{
    while (1) {
        int ok = fresh ? meta_solve(f->kids[i], E) : meta_redo(f->kids[i], E);
        if (ok) { i++; if (i >= f->nkids) return 1; fresh = 1; }
        else    { i--; if (i < 0) return 0; fresh = 0; }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void meta_disj_mark(meta_fr *f)   { extern Trail g_resolve_trail; f->br_cp = resolve_cp_current(); f->br_trail = trail_mark(&g_resolve_trail); }
static void meta_disj_unwind(meta_fr *f) { extern Trail g_resolve_trail; resolve_cp_truncate(f->br_cp); trail_unwind(&g_resolve_trail, f->br_trail); }
static int meta_disj_advance(meta_fr *f, Term **E)
{
    while (f->cur < f->nkids) {
        meta_disj_mark(f);
        if (meta_solve(f->kids[f->cur], E)) return 1;
        meta_disj_unwind(f);
        f->cur++;
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int meta_solve(meta_fr *f, Term **E)
{
    switch (f->kind) {
    case MK_TRUE:    return 1;
    case MK_FAIL:    return 0;
    case MK_BUILTIN: return meta_builtin_solve(f);
    case MK_PRED:    return meta_pred_solve(f, E);
    case MK_CONJ:    return meta_conj_drive(f, E, 0, 1);
    case MK_DISJ:    f->cur = 0; return meta_disj_advance(f, E);
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int meta_redo(meta_fr *f, Term **E)
{
    switch (f->kind) {
    case MK_TRUE: case MK_FAIL: case MK_BUILTIN: return 0;
    case MK_PRED: return meta_pred_redo(f, E);
    case MK_CONJ: return meta_conj_drive(f, E, f->nkids - 1, 0);
    case MK_DISJ:
        if (meta_redo(f->kids[f->cur], E)) return 1;
        meta_disj_unwind(f); f->cur++;
        return meta_disj_advance(f, E);
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_meta_solve(void *goal_v, void **root_out)
{
    extern Term **g_resolve_env;
    if (root_out) *root_out = (void *)0;
    meta_fr *f = meta_compile((Term *)goal_v);
    if (!f) return 0;
    meta_root *r = (meta_root *)GC_MALLOC(sizeof *r); if (!r) return 0;
    r->fr = f; r->E = g_resolve_env;
    if (root_out) *root_out = (void *)r;
    return meta_solve(f, r->E);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_meta_redo(void *root_v)
{
    meta_root *r = (meta_root *)root_v;
    if (!r || !r->fr) return 0;
    return meta_redo(r->fr, r->E);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static meta_root *g_meta_compat = (meta_root *)0;
int rt_call_term(void *goal_v)
{
    void *root = (void *)0;
    int ok = rt_meta_solve(goal_v, &root);
    g_meta_compat = (meta_root *)root;
    return ok;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_redo_meta(void *entry_cp_v)
{
    (void)entry_cp_v;
    return g_meta_compat ? rt_meta_redo((void *)g_meta_compat) : 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_type_test_cell(void *cell_term, const char *fn)
{
    extern int rt_type_test_term(const char *fn, void *t0);
    return rt_type_test_term(fn, term_deref((Term *)cell_term));
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_functor_cell(void *t0_cell, void *name_cell, void *arity_cell)
{
    extern int rt_functor_term(void *t0, int k1, long i1, const char *s1, int k2, long i2, const char *s2);
    extern Trail g_resolve_trail;
    Term *t0 = term_deref((Term *)t0_cell);
    Term *t1 = term_deref((Term *)name_cell);
    Term *t2 = term_deref((Term *)arity_cell);
    int mark = trail_mark(&g_resolve_trail);
    Term *d0 = t0;
    if (d0 && d0->tag != TERM_VAR) {
        Term *nameT, *arityT;
        if (d0->tag == TERM_COMPOUND) { nameT = term_new_atom(d0->compound.functor); arityT = term_new_int(d0->compound.arity); }
        else if (d0->tag == TERM_ATOM) { nameT = term_new_atom(d0->atom_id); arityT = term_new_int(0); }
        else if (d0->tag == TERM_INT) { nameT = term_new_int(d0->ival); arityT = term_new_int(0); }
        else if (d0->tag == TERM_FLOAT) { nameT = term_new_float(d0->fval); arityT = term_new_int(0); }
        else { trail_unwind(&g_resolve_trail, mark); return 0; }
        if (!unify(t1, nameT, &g_resolve_trail) || !unify(t2, arityT, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    if (!t2 || t2->tag != TERM_INT) { trail_unwind(&g_resolve_trail, mark); return 0; }
    long ar = t2->ival;
    Term *built;
    if (ar == 0) { built = t1 ? t1 : term_new_atom(prolog_atom_intern("[]")); }
    else {
        if (!t1 || t1->tag != TERM_ATOM) { trail_unwind(&g_resolve_trail, mark); return 0; }
        Term **args = (Term **)GC_MALLOC((size_t)ar * sizeof(Term *));
        for (long i = 0; i < ar; i++) args[i] = term_new_var(-1);
        built = term_new_compound(t1->atom_id, (int)ar, args);
    }
    if (!unify(t0, built, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_arg_cell(void *n_cell, void *t_cell, void *arg_cell)
{
    extern Trail g_resolve_trail;
    Term *tN = term_deref((Term *)n_cell);
    Term *tT = term_deref((Term *)t_cell);
    Term *tA = (Term *)arg_cell;
    int mark = trail_mark(&g_resolve_trail);
    if (!tN || tN->tag != TERM_INT || !tT || tT->tag != TERM_COMPOUND) { trail_unwind(&g_resolve_trail, mark); return 0; }
    long n = tN->ival;
    if (n < 1 || n > tT->compound.arity) { trail_unwind(&g_resolve_trail, mark); return 0; }
    if (!unify(tA, tT->compound.args[n - 1], &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_univ_cell(void *t0_cell, void *list_cell)
{
    extern int rt_univ_term_term(void *t0, void *t1);
    return rt_univ_term_term(t0_cell, list_cell);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_succ_plus_cell(long arity, void *a_cell, void *b_cell, void *c_cell)
{
    extern Trail g_resolve_trail;
    Term *ta = a_cell ? (Term *)a_cell : (Term *)0, *tb = b_cell ? (Term *)b_cell : (Term *)0, *tc = c_cell ? (Term *)c_cell : (Term *)0;
    Term *da = ta ? term_deref(ta) : (Term *)0, *db = tb ? term_deref(tb) : (Term *)0, *dc = tc ? term_deref(tc) : (Term *)0;
    int mark = trail_mark(&g_resolve_trail);
    if (arity == 2) {
        if (da && da->tag == TERM_INT) {
            if (da->ival < 0) return 0;
            if (!unify(tb, term_new_int(da->ival + 1), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
            return 1;
        }
        if (db && db->tag == TERM_INT) {
            if (db->ival <= 0) return 0;
            if (!unify(ta, term_new_int(db->ival - 1), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
            return 1;
        }
        return 0;
    }
    if (arity == 3) {
        int va = (da && da->tag == TERM_INT), vb = (db && db->tag == TERM_INT), vc = (dc && dc->tag == TERM_INT);
        int ok = 0;
        if (va && vb)      ok = unify(tc, term_new_int(da->ival + db->ival), &g_resolve_trail);
        else if (va && vc) ok = unify(tb, term_new_int(dc->ival - da->ival), &g_resolve_trail);
        else if (vb && vc) ok = unify(ta, term_new_int(dc->ival - db->ival), &g_resolve_trail);
        if (!ok) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *atom_op_text(Term *t, char *buf, size_t bufsz)
{
    t = t ? term_deref(t) : (Term *)0;
    if (!t) return (const char *)0;
    if (t->tag == TERM_ATOM)  return prolog_atom_name(t->atom_id);
    if (t->tag == TERM_INT)   { snprintf(buf, bufsz, "%ld", t->ival);  return buf; }
    if (t->tag == TERM_FLOAT) { snprintf(buf, bufsz, "%g", t->fval);   return buf; }
    return (const char *)0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_atom_op_cell(const char *fn, void *a0_cell, void *a1_cell, void *a2_cell)
{
    extern Trail g_resolve_trail;
    Term *t0 = a0_cell ? term_deref((Term *)a0_cell) : (Term *)0;
    Term *t1 = a1_cell ? term_deref((Term *)a1_cell) : (Term *)0;
    Term *t2 = a2_cell ? term_deref((Term *)a2_cell) : (Term *)0;
    int mark = trail_mark(&g_resolve_trail);
    char buf0[512], buf1[512];
    if (!strcmp(fn, "atom_length")) {
        const char *s = atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { trail_unwind(&g_resolve_trail, mark); return 0; }
        if (!unify((Term *)a1_cell, term_new_int((long)strlen(s)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "atom_concat")) {
        const char *s0 = atom_op_text(t0, buf0, sizeof buf0);
        const char *s1 = atom_op_text(t1, buf1, sizeof buf1);
        if (!s0 || !s1) { trail_unwind(&g_resolve_trail, mark); return 0; }
        size_t l0 = strlen(s0), l1 = strlen(s1);
        char *cat = (char *)GC_MALLOC(l0 + l1 + 1); memcpy(cat, s0, l0); memcpy(cat + l0, s1, l1); cat[l0 + l1] = '\0';
        if (!unify((Term *)a2_cell, term_new_atom(prolog_atom_intern(cat)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "upcase_atom") || !strcmp(fn, "downcase_atom")) {
        const char *s = atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { trail_unwind(&g_resolve_trail, mark); return 0; }
        size_t n = strlen(s); char *out = (char *)GC_MALLOC(n + 1);
        int up = (!strcmp(fn, "upcase_atom"));
        for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        out[n] = '\0';
        if (!unify((Term *)a1_cell, term_new_atom(prolog_atom_intern(out)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    int as_codes = (!strcmp(fn, "atom_codes"));
    if (!strcmp(fn, "atom_chars") || as_codes) {
        if (t0 && t0->tag != TERM_VAR) {
            const char *s = atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { trail_unwind(&g_resolve_trail, mark); return 0; }
            size_t n = strlen(s);
            Term *lst = term_new_atom(prolog_atom_intern("[]"));
            for (size_t i = n; i > 0; i--) {
                unsigned char ch = (unsigned char)s[i - 1];
                Term *el;
                if (as_codes) el = term_new_int((long)ch);
                else { char cs[2] = { (char)ch, '\0' }; el = term_new_atom(prolog_atom_intern(cs)); }
                Term **c = (Term **)GC_MALLOC(2 * sizeof(Term *)); c[0] = el; c[1] = lst;
                lst = term_new_compound(ATOM_DOT, 2, c);
            }
            if (!unify((Term *)a1_cell, lst, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
            return 1;
        }
        Term *cur = t1;
        char out[512]; size_t oi = 0;
        while (cur && cur->tag == TERM_COMPOUND && cur->compound.functor == ATOM_DOT && cur->compound.arity == 2) {
            Term *el = term_deref(cur->compound.args[0]);
            if (oi >= sizeof(out) - 1) break;
            if (as_codes) { if (!el || el->tag != TERM_INT) { trail_unwind(&g_resolve_trail, mark); return 0; } out[oi++] = (char)el->ival; }
            else { if (!el || el->tag != TERM_ATOM) { trail_unwind(&g_resolve_trail, mark); return 0; } const char *cn = prolog_atom_name(el->atom_id); out[oi++] = cn ? cn[0] : '?'; }
            cur = term_deref(cur->compound.args[1]);
        }
        out[oi] = '\0';
        if (!unify((Term *)a0_cell, term_new_atom(prolog_atom_intern(out)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "string_length")) {
        const char *s = atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { trail_unwind(&g_resolve_trail, mark); return 0; }
        if (!unify((Term *)a1_cell, term_new_int((long)strlen(s)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "string_upper") || !strcmp(fn, "string_lower")) {
        const char *s = atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { trail_unwind(&g_resolve_trail, mark); return 0; }
        size_t n = strlen(s); char *out = (char *)GC_MALLOC(n + 1);
        int up = (!strcmp(fn, "string_upper"));
        for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        out[n] = '\0';
        if (!unify((Term *)a1_cell, term_new_atom(prolog_atom_intern(out)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "atom_string") || !strcmp(fn, "string_to_atom")) {
        if (t0 && t0->tag != TERM_VAR) {
            const char *s = atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { trail_unwind(&g_resolve_trail, mark); return 0; }
            if (!unify((Term *)a1_cell, term_new_atom(prolog_atom_intern(s)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
            return 1;
        }
        const char *s = atom_op_text(t1, buf1, sizeof buf1);
        if (!s) { trail_unwind(&g_resolve_trail, mark); return 0; }
        if (!unify((Term *)a0_cell, term_new_atom(prolog_atom_intern(s)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "number_string") || !strcmp(fn, "atom_number")) {
        if (t0 && t0->tag != TERM_VAR) {
            const char *s = atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { trail_unwind(&g_resolve_trail, mark); return 0; }
            if (!unify((Term *)a1_cell, term_new_atom(prolog_atom_intern(s)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
            return 1;
        }
        const char *s = atom_op_text(t1, buf1, sizeof buf1);
        if (!s) { trail_unwind(&g_resolve_trail, mark); return 0; }
        char *end; long iv = strtol(s, &end, 10);
        if (*end == '\0') { if (!unify((Term *)a0_cell, term_new_int(iv), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; } return 1; }
        double dv = strtod(s, &end);
        if (*end == '\0') { if (!unify((Term *)a0_cell, term_new_float(dv), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; } return 1; }
        trail_unwind(&g_resolve_trail, mark); return 0;
    }
    if (!strcmp(fn, "string_concat")) {
        const char *s0 = atom_op_text(t0, buf0, sizeof buf0);
        const char *s1 = atom_op_text(t1, buf1, sizeof buf1);
        if (!s0 || !s1) { trail_unwind(&g_resolve_trail, mark); return 0; }
        size_t l0 = strlen(s0), l1 = strlen(s1);
        char *cat = (char *)GC_MALLOC(l0 + l1 + 1); memcpy(cat, s0, l0); memcpy(cat + l0, s1, l1); cat[l0 + l1] = '\0';
        if (!unify((Term *)a2_cell, term_new_atom(prolog_atom_intern(cat)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "atomic_list_concat") || !strcmp(fn, "concat_atom")) {
        const char *sep = (t1 && t1->tag == TERM_ATOM) ? prolog_atom_name(t1->atom_id) : "";
        void *result_cell = t2 ? a2_cell : a1_cell;
        Term *lst = t0;
        char *out = (char *)GC_MALLOC(4096); size_t oi = 0;
        int first = 1;
        while (lst && lst->tag == TERM_COMPOUND && lst->compound.arity == 2) {
            Term *el = term_deref(lst->compound.args[0]);
            if (!first && sep && sep[0]) { size_t sl = strlen(sep); if (oi + sl < 4095) { memcpy(out + oi, sep, sl); oi += sl; } }
            first = 0;
            const char *es = atom_op_text(el, buf0, sizeof buf0);
            if (!es) { trail_unwind(&g_resolve_trail, mark); return 0; }
            size_t el_len = strlen(es); if (oi + el_len < 4095) { memcpy(out + oi, es, el_len); oi += el_len; }
            lst = term_deref(lst->compound.args[1]);
        }
        out[oi] = '\0';
        if (!unify((Term *)result_cell, term_new_atom(prolog_atom_intern(out)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    (void)t2;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pl_format_cell(const char *fmt, void *list_cell)
{
    extern void pl_write(Term *);
    if (!fmt) return;
    Term *args = list_cell ? term_deref((Term *)list_cell) : (Term *)0;
    for (const char *p = fmt; *p; p++) {
        if (*p == '~') {
            p++;
            if (*p == 'w' || *p == 'a' || *p == 'p') {
                if (args && args->tag == TERM_COMPOUND && args->compound.arity == 2) { pl_write(term_deref(args->compound.args[0])); args = term_deref(args->compound.args[1]); }
            } else if (*p == 'd') {
                if (args && args->tag == TERM_COMPOUND && args->compound.arity == 2) { Term *h = term_deref(args->compound.args[0]); if (h && h->tag == TERM_INT) printf("%ld", h->ival); args = term_deref(args->compound.args[1]); }
            } else if (*p == 'i') {
                if (args && args->tag == TERM_COMPOUND && args->compound.arity == 2) args = term_deref(args->compound.args[1]);
            } else if (*p == 'n' || *p == 'N') { putchar('\n');
            } else if (*p == '~') { putchar('~');
            } else if (*p == 't') { putchar('\t'); }
        } else { putchar(*p); }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_char_type_cell(void *char_cell, void *type_cell, void *val_cell)
{
    extern Trail g_resolve_trail;
    Term *tc = char_cell ? term_deref((Term *)char_cell) : (Term *)0;
    Term *tt = type_cell ? term_deref((Term *)type_cell) : (Term *)0;
    if (!tc || !tt) return 0;
    char b0[256]; const char *cs = atom_op_text(tc, b0, sizeof b0);
    if (!cs || !cs[0]) return 0;
    unsigned char ch = (unsigned char)cs[0];
    int mark = trail_mark(&g_resolve_trail);
    if (tt->tag == TERM_COMPOUND && tt->compound.arity >= 1) {
        const char *ty = prolog_atom_name(tt->compound.functor);
        Term *inner = term_deref(tt->compound.args[0]);
        Term *out = NULL;
        if (!ty) { trail_unwind(&g_resolve_trail, mark); return 0; }
        if (!strcmp(ty, "digit"))    { if (!isdigit(ch)) { trail_unwind(&g_resolve_trail, mark); return 0; } out = term_new_int((long)(ch - '0')); }
        else if (!strcmp(ty, "to_lower")) { char c2[2] = { (char)tolower(ch), 0 }; out = term_new_atom(prolog_atom_intern(c2)); }
        else if (!strcmp(ty, "to_upper")) { char c2[2] = { (char)toupper(ch), 0 }; out = term_new_atom(prolog_atom_intern(c2)); }
        else if (!strcmp(ty, "upper")) { if (!isupper(ch)) { trail_unwind(&g_resolve_trail, mark); return 0; } char c2[2] = { (char)tolower(ch), 0 }; out = term_new_atom(prolog_atom_intern(c2)); }
        else if (!strcmp(ty, "lower")) { if (!islower(ch)) { trail_unwind(&g_resolve_trail, mark); return 0; } char c2[2] = { (char)toupper(ch), 0 }; out = term_new_atom(prolog_atom_intern(c2)); }
        else if (!strcmp(ty, "code"))  { out = term_new_int((long)ch); }
        else { trail_unwind(&g_resolve_trail, mark); return 0; }
        Term *dst = val_cell ? term_deref((Term *)val_cell) : inner;
        if (!unify(dst, out, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    if (tt->tag != TERM_ATOM) { trail_unwind(&g_resolve_trail, mark); return 0; }
    const char *ty = prolog_atom_name(tt->atom_id);
    if (!ty) { trail_unwind(&g_resolve_trail, mark); return 0; }
    int ok = 0;
    if      (!strcmp(ty, "alpha"))        ok = isalpha(ch);
    else if (!strcmp(ty, "alnum"))        ok = isalnum(ch);
    else if (!strcmp(ty, "digit"))        ok = isdigit(ch);
    else if (!strcmp(ty, "space") || !strcmp(ty, "white")) ok = isspace(ch);
    else if (!strcmp(ty, "upper"))        ok = isupper(ch);
    else if (!strcmp(ty, "lower"))        ok = islower(ch);
    else if (!strcmp(ty, "punct"))        ok = ispunct(ch);
    else if (!strcmp(ty, "graph"))        ok = isgraph(ch);
    else if (!strcmp(ty, "csym"))         ok = (isalnum(ch) || ch == '_');
    else if (!strcmp(ty, "csymf"))        ok = (isalpha(ch) || ch == '_');
    else if (!strcmp(ty, "end_of_line"))  ok = (ch == '\n' || ch == '\r');
    else if (!strcmp(ty, "newline"))      ok = (ch == '\n');
    else { trail_unwind(&g_resolve_trail, mark); return 0; }
    if (!ok) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_term_class(Term *t) {
    switch (t->tag) {
    case TERM_VAR:      return 0;
    case TERM_FLOAT:    return 1;
    case TERM_INT:      return 1;
    case TERM_ATOM:     return 2;
    case TERM_COMPOUND: return 3;
    default:            return 4;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_term_compare(Term *a, Term *b) {
    a = a ? term_deref(a) : NULL; b = b ? term_deref(b) : NULL;
    if (!a && !b) return 0; if (!a) return -1; if (!b) return 1;
    int ca = rt_pl_term_class(a), cb = rt_pl_term_class(b);
    if (ca != cb) return ca < cb ? -1 : 1;
    switch (a->tag) {
    case TERM_VAR: return (a == b) ? 0 : (a < b ? -1 : 1);
    case TERM_INT: { double x = (double)a->ival, y = (b->tag == TERM_INT) ? (double)b->ival : b->fval; return x < y ? -1 : (x > y ? 1 : 0); }
    case TERM_FLOAT: { double x = a->fval, y = (b->tag == TERM_INT) ? (double)b->ival : b->fval; return x < y ? -1 : (x > y ? 1 : 0); }
    case TERM_ATOM: { const char *na = prolog_atom_name(a->atom_id), *nb = prolog_atom_name(b->atom_id); int c = strcmp(na ? na : "", nb ? nb : ""); return c < 0 ? -1 : (c > 0 ? 1 : 0); }
    case TERM_COMPOUND: {
        if (a->compound.arity != b->compound.arity) return a->compound.arity < b->compound.arity ? -1 : 1;
        const char *na = prolog_atom_name(a->compound.functor), *nb = prolog_atom_name(b->compound.functor);
        int c = strcmp(na ? na : "", nb ? nb : ""); if (c) return c < 0 ? -1 : 1;
        for (int i = 0; i < a->compound.arity; i++) { int r = rt_pl_term_compare(a->compound.args[i], b->compound.args[i]); if (r) return r; }
        return 0;
    }
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_sort_cell(int do_msort, void *list_cell, void *result_cell)
{
    extern Trail g_resolve_trail;
    Term *lst = term_deref((Term *)list_cell);
    Term *elems[4096]; int n = 0;
    int dot_id = prolog_atom_intern(".");
    Term *cur = lst;
    while (cur && cur->tag == TERM_COMPOUND && cur->compound.functor == dot_id && cur->compound.arity == 2 && n < 4096) {
        elems[n++] = term_deref(cur->compound.args[0]);
        cur = term_deref(cur->compound.args[1]);
    }
    for (int i = 1; i < n; i++) {
        Term *key = elems[i]; int j = i - 1;
        while (j >= 0 && rt_pl_term_compare(elems[j], key) > 0) { elems[j + 1] = elems[j]; j--; }
        elems[j + 1] = key;
    }
    int m = 0; int out_idx[4096];
    for (int i = 0; i < n; i++) {
        if (!do_msort && m > 0 && rt_pl_term_compare(elems[out_idx[m - 1]], elems[i]) == 0) continue;
        out_idx[m++] = i;
    }
    Term *result = term_new_atom(prolog_atom_intern("[]"));
    for (int i = m - 1; i >= 0; i--) {
        Term *pair[2]; pair[0] = elems[out_idx[i]]; pair[1] = result;
        result = term_new_compound(prolog_atom_intern("."), 2, pair);
    }
    int mark = trail_mark(&g_resolve_trail);
    if (!unify((Term *)result_cell, result, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_numbervars_cell(void *term_cell, void *start_cell, void *end_cell) {
    extern Trail g_resolve_trail;
    Term *st = term_deref((Term *)start_cell);
    if (!st || st->tag != TERM_INT) return 0;
    long counter = st->ival;
    int var_id = prolog_atom_intern("$VAR");
    Term *stack[2048]; int top = 0;
    Term *tc = term_deref((Term *)term_cell);
    if (tc) { stack[top++] = tc; }
    while (top > 0) {
        Term *t = term_deref(stack[--top]);
        if (!t) continue;
        if (t->tag == TERM_VAR) {
            Term **a = (Term **)GC_malloc(sizeof(Term *)); a[0] = term_new_int(counter++);
            Term *vt = term_new_compound(var_id, 1, a);
            unify(t, vt, &g_resolve_trail);
        } else if (t->tag == TERM_COMPOUND) {
            for (int i = t->compound.arity - 1; i >= 0; i--) if (top < 2048) stack[top++] = t->compound.args[i];
        }
    }
    int mark = trail_mark(&g_resolve_trail);
    if (!unify((Term *)end_cell, term_new_int(counter), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_term_string_cell(void *term_cell, void *str_cell)
{
    extern Trail g_resolve_trail;
    extern void pl_writeq(Term *);
    int mark = trail_mark(&g_resolve_trail);
    Term *t = term_deref((Term *)term_cell);
    int pipefd[2];
    if (pipe(pipefd) != 0) { trail_unwind(&g_resolve_trail, mark); return 0; }
    int saved_fd = dup(STDOUT_FILENO);
    if (saved_fd < 0) { close(pipefd[0]); close(pipefd[1]); trail_unwind(&g_resolve_trail, mark); return 0; }
    dup2(pipefd[1], STDOUT_FILENO); close(pipefd[1]);
    pl_writeq(t);
    fflush(stdout);
    dup2(saved_fd, STDOUT_FILENO); close(saved_fd);
    char buf[4096]; ssize_t n = read(pipefd[0], buf, sizeof buf - 1); close(pipefd[0]);
    if (n < 0) { trail_unwind(&g_resolve_trail, mark); return 0; }
    buf[n] = '\0';
    int atom_id = prolog_atom_intern(buf);
    if (!unify((Term *)str_cell, term_new_atom(atom_id), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static Term *copy_term_deep(Term *t, Term **var_map, int *var_cap, int *var_n)
{
    if (!t) return NULL;
    t = term_deref(t);
    if (!t) return NULL;
    if (t->tag == TERM_VAR) {
        for (int i = 0; i < *var_n; i += 2) if (var_map[i] == t) return var_map[i+1];
        if (*var_n + 2 > *var_cap) return NULL;
        Term *fresh = term_new_var(-1);
        var_map[(*var_n)++] = t; var_map[(*var_n)++] = fresh;
        return fresh;
    }
    if (t->tag == TERM_ATOM || t->tag == TERM_INT || t->tag == TERM_FLOAT) return t;
    if (t->tag == TERM_COMPOUND) {
        Term **args = (Term **)GC_MALLOC(t->compound.arity * sizeof(Term *));
        if (!args) return NULL;
        for (int i = 0; i < t->compound.arity; i++) { args[i] = copy_term_deep(t->compound.args[i], var_map, var_cap, var_n); if (!args[i] && t->compound.args[i]) return NULL; }
        return term_new_compound(t->compound.functor, t->compound.arity, args);
    }
    return t;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_copy_term_cell(void *term_cell, void *copy_cell)
{
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *t = term_deref((Term *)term_cell);
    Term *var_map[256]; int var_cap = 256, var_n = 0;
    Term *copy = copy_term_deep(t, var_map, &var_cap, &var_n);
    if (!copy && t) { trail_unwind(&g_resolve_trail, mark); return 0; }
    if (!copy) copy = term_new_var(-1);
    if (!unify((Term *)copy_cell, copy, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}

/* ATTIC: dead code excised from src/runtime/unification.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pl_frame_sync_env(void *frame, int nslots)
{
    extern Term **g_resolve_env;
    extern Trail g_resolve_trail;
    extern void trail_init(Trail *t);
    if (!g_resolve_trail.stack || g_resolve_trail.capacity <= 0) trail_init(&g_resolve_trail);
    Term **cells = (Term **)(((char *)frame) + 8);
    for (int i = 0; i < nslots; i++) {
        Term *v = term_new_var(i);
        cells[i] = v;
        if (g_resolve_env) g_resolve_env[i] = v;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_unify_struct_gz(void *frame, const void *lnd, const void *rnd)
{
    (void)frame; (void)lnd; (void)rnd; return *(volatile char *)0;
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
static meta_fr *meta_fr_new(int kind) { meta_fr *f = (meta_fr *)GC_MALLOC(sizeof *f); if (f) f->kind = kind; return f; }
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
void rt_pl_throw_clear(void) { g_pl_throw_ball = (Term *)0; }

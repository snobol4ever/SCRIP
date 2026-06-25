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
        rt_pl_env_ensure(slot);
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
    rt_pl_env_ensure(slot);
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
    rt_pl_env_ensure(lslot > rslot ? lslot : rslot);
    Term *lt = (g_resolve_env && lslot >= 0 && g_resolve_env[lslot]) ? term_deref(g_resolve_env[lslot]) : (Term *)0;
    if (!lt) { lt = term_new_var(lslot); if (g_resolve_env && lslot >= 0) g_resolve_env[lslot] = lt; }
    Term *rt_ = (g_resolve_env && rslot >= 0 && g_resolve_env[rslot]) ? term_deref(g_resolve_env[rslot]) : (Term *)0;
    if (!rt_) { rt_ = term_new_var(rslot); if (g_resolve_env && rslot >= 0) g_resolve_env[rslot] = rt_; }
    return rt_unify_terms(lt, rt_);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static size_t pl_resolve_env_cap = 0;
void rt_pl_env_ensure(int slot)
{
    extern Term **g_resolve_env;
    if (slot < 0) return;
    if (g_resolve_env && (size_t)slot < pl_resolve_env_cap) return;
    size_t need = (size_t)slot + 1, nc = pl_resolve_env_cap ? pl_resolve_env_cap : 64;
    while (nc < need) nc *= 2;
    Term **ne = (Term **)GC_MALLOC(nc * sizeof(Term *));
    for (size_t i = 0; i < pl_resolve_env_cap; i++) ne[i] = g_resolve_env ? g_resolve_env[i] : (Term *)0;
    for (size_t i = pl_resolve_env_cap; i < nc; i++) ne[i] = (Term *)0;
    g_resolve_env = ne; pl_resolve_env_cap = nc;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pl_cells_init(void **cells, int n)
{
    extern Term **g_resolve_env;
    rt_pl_env_ensure(n - 1);
    char *base = (char *)cells;
    for (int i = 0; i < n; i++) {
        Term *v = term_new_var(i);
        *(Term **)(base + (size_t)16 * (size_t)i) = v;
        if (g_resolve_env) g_resolve_env[i] = v;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pl_gz_init(void *frame, int nslots)
{
    extern Term **g_resolve_env;
    prolog_atom_init();
    rt_pl_env_ensure(nslots + 63);
    char *base = (char *)frame;
    for (int i = 0; i < nslots; i++) {
        Term *v = term_new_var(i);
        *(Term **)(base + 8 + (size_t)16 * (size_t)i) = v;
        g_resolve_env[i] = v;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void * rt_enter(void **slot, int nslots)
{
    extern void *GC_malloc(size_t);
    if (!*slot) *slot = GC_malloc((size_t)(8 + 16 * (nslots > 0 ? nslots : 1)));
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
typedef struct { const char *name; long arity; void *alpha; void *redo; } pl_pred_row_t;
static pl_pred_row_t *g_pl_pred_table = (pl_pred_row_t *)0;
static long           g_pl_pred_n     = 0;
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
static const char *g_meta_builtins[] = { "is", "=:=", "=\\=", "<", ">", "=<", ">=", "=", "\\=", (const char *)0 };
/*--------------------------------------------------------------------------------------------------------------------*/
static int meta_solve(meta_fr *f, Term **E);
static int meta_redo(meta_fr *f, Term **E);
/*--------------------------------------------------------------------------------------------------------------------*/
static meta_root *g_meta_compat = (meta_root *)0;
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
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { Term **items; int n; int cap; } pl_findall_acc;
void * rt_pl_findall_begin(void)
{
    pl_findall_acc *a = (pl_findall_acc *)GC_MALLOC(sizeof *a);
    if (!a) return (void *)0;
    a->cap = 16; a->n = 0; a->items = (Term **)GC_MALLOC((size_t)a->cap * sizeof(Term *));
    return (void *)a;
}
void rt_pl_findall_collect(void *acc_v, void *tmpl_term)
{
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    if (!a || !a->items) return;
    if (a->n >= a->cap) { int nc = a->cap * 2; Term **ni = (Term **)GC_MALLOC((size_t)nc * sizeof(Term *)); if (!ni) return; for (int i = 0; i < a->n; i++) ni[i] = a->items[i]; a->items = ni; a->cap = nc; }
    Term *var_map[256]; int var_cap = 256, var_n = 0;
    Term *cp = copy_term_deep(term_deref((Term *)tmpl_term), var_map, &var_cap, &var_n);
    a->items[a->n++] = cp ? cp : term_deref((Term *)tmpl_term);
}
int rt_pl_findall_finish(void *acc_v, void *result_term)
{
    extern Trail g_resolve_trail;
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    int dot = prolog_atom_intern(".");
    Term *lst = term_new_atom(prolog_atom_intern("[]"));
    int n = a ? a->n : 0;
    for (int i = n - 1; i >= 0; i--) {
        Term **c = (Term **)GC_MALLOC(2 * sizeof(Term *)); if (!c) return 0;
        c[0] = a->items[i]; c[1] = lst;
        lst = term_new_compound(dot, 2, c);
    }
    int mark = trail_mark(&g_resolve_trail);
    if (!unify(term_deref((Term *)result_term), lst, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
int rt_pl_agg_count_finish(void *acc_v, void *result_term)
{
    extern Trail g_resolve_trail;
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    int mark = trail_mark(&g_resolve_trail);
    if (!unify(term_deref((Term *)result_term), term_new_int(a ? a->n : 0), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int agg_num(Term *t, long *iv, double *dv, int *isf)
{
    t = t ? term_deref(t) : (Term *)0;
    if (!t) return 0;
    if (t->tag == TERM_INT)   { *iv = t->ival; *isf = 0; return 1; }
    if (t->tag == TERM_FLOAT) { *dv = t->fval; *isf = 1; return 1; }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_sum_finish(void *acc_v, void *result_term)
{
    extern Trail g_resolve_trail;
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    int n = a ? a->n : 0;
    long si = 0; double sd = 0.0; int isf = 0;
    for (int i = 0; i < n; i++) {
        long iv = 0; double dv = 0.0; int ef = 0;
        if (!agg_num(a->items[i], &iv, &dv, &ef)) return 0;
        if (ef) { sd += dv; isf = 1; } else { si += iv; sd += (double)iv; }
    }
    int mark = trail_mark(&g_resolve_trail);
    Term *r = isf ? term_new_float(sd) : term_new_int(si);
    if (!unify(term_deref((Term *)result_term), r, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_agg_minmax_finish(void *acc_v, void *result_term, int want_max)
{
    extern Trail g_resolve_trail;
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    int n = a ? a->n : 0;
    if (n <= 0) return 0;
    long bi = 0; double bd = 0.0; int isf = 0, have = 0;
    for (int i = 0; i < n; i++) {
        long iv = 0; double dv = 0.0; int ef = 0;
        if (!agg_num(a->items[i], &iv, &dv, &ef)) return 0;
        double cur = ef ? dv : (double)iv;
        double best = isf ? bd : (double)bi;
        if (!have || (want_max ? (cur > best) : (cur < best))) { if (ef) { bd = dv; isf = 1; } else { bi = iv; bd = (double)iv; isf = 0; } have = 1; }
    }
    int mark = trail_mark(&g_resolve_trail);
    Term *r = isf ? term_new_float(bd) : term_new_int(bi);
    if (!unify(term_deref((Term *)result_term), r, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_max_finish(void *acc_v, void *result_term) { return rt_pl_agg_minmax_finish(acc_v, result_term, 1); }
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_min_finish(void *acc_v, void *result_term) { return rt_pl_agg_minmax_finish(acc_v, result_term, 0); }
/*--------------------------------------------------------------------------------------------------------------------*/
static Term **g_rt_pl_nb = (Term **)0;
static int g_rt_pl_nb_n = 0;
static int rt_pl_nb_ensure(int id)
{
    if (id < 0) return 0;
    if (id >= g_rt_pl_nb_n) {
        extern void *GC_malloc(size_t); extern void *GC_realloc(void *, size_t);
        int nc = g_rt_pl_nb_n ? g_rt_pl_nb_n : 16; while (nc <= id) nc *= 2;
        Term **nv = (Term **)(g_rt_pl_nb ? GC_realloc(g_rt_pl_nb, (size_t)nc * sizeof(Term *)) : GC_malloc((size_t)nc * sizeof(Term *)));
        if (!nv) return 0;
        for (int i = g_rt_pl_nb_n; i < nc; i++) nv[i] = (Term *)0;
        g_rt_pl_nb = nv; g_rt_pl_nb_n = nc;
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* PL-DESCR-3 incr.2: slot helpers — the nb value-work (SWI deep-copy on store / deep-copy+unify on fetch) lives here */
/* once; both the [rbx+k*16] GVA arm (compile-time slot) and the by-name fallback below reach the global through these. */
void *rt_pl_nb_copy_persist(void *val_cell)
{
    Term *var_map[256]; int var_cap = 256, var_n = 0;
    return (void *)copy_term_deep(term_deref((Term *)val_cell), var_map, &var_cap, &var_n);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_getval_ptr(void *stored_cell, void *val_cell)
{
    extern Trail g_resolve_trail;
    Term *val = (Term *)stored_cell;
    if (!val) return 0;
    int mark = trail_mark(&g_resolve_trail);
    Term *var_map[256]; int var_cap = 256, var_n = 0;
    Term *fresh = copy_term_deep(val, var_map, &var_cap, &var_n);
    if (!fresh) fresh = val;
    if (!unify((Term *)val_cell, fresh, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_setval_cell(void *key_cell, void *val_cell)
{
    Term *k = term_deref((Term *)key_cell);
    if (!k || k->tag != TERM_ATOM) return 0;
    int id = k->atom_id;
    if (!rt_pl_nb_ensure(id)) return 0;
    g_rt_pl_nb[id] = (Term *)rt_pl_nb_copy_persist(val_cell);
    return g_rt_pl_nb[id] ? 1 : 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_getval_cell(void *key_cell, void *val_cell)
{
    Term *k = term_deref((Term *)key_cell);
    if (!k || k->tag != TERM_ATOM) return 0;
    int id = k->atom_id;
    Term *val = (id >= 0 && id < g_rt_pl_nb_n) ? g_rt_pl_nb[id] : (Term *)0;
    return rt_pl_nb_getval_ptr((void *)val, val_cell);
}
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct dyn_clause { Term *head; Term *body; struct dyn_clause *next; } dyn_clause_t;
typedef struct { const char *name; long arity; dyn_clause_t *head; dyn_clause_t *tail; } dyn_pred_row_t;
static dyn_pred_row_t *g_pl_dyn_pred_table = (dyn_pred_row_t *)0;
static long            g_pl_dyn_pred_n     = 0;
static long            g_pl_dyn_pred_cap   = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
static dyn_pred_row_t *dyn_pred_find(const char *name, long arity)
{
    for (long i = 0; i < g_pl_dyn_pred_n; i++) if (g_pl_dyn_pred_table[i].name && !strcmp(g_pl_dyn_pred_table[i].name, name) && g_pl_dyn_pred_table[i].arity == arity) return &g_pl_dyn_pred_table[i];
    return (dyn_pred_row_t *)0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void dyn_term_key(Term *t, const char **name_out, long *arity_out)
{
    Term *d = t ? term_deref(t) : (Term *)0;
    if (d && d->tag == TERM_COMPOUND) { *name_out = prolog_atom_name(d->compound.functor); *arity_out = d->compound.arity; return; }
    if (d && d->tag == TERM_ATOM) { *name_out = prolog_atom_name(d->atom_id); *arity_out = 0; return; }
    *name_out = (const char *)0; *arity_out = 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_dyn_abolish_cell(void *fn_cell, void *ar_cell)
{
    Term *fn = term_deref((Term *)fn_cell);
    Term *ar = term_deref((Term *)ar_cell);
    if (!fn || fn->tag != TERM_ATOM || !ar || ar->tag != TERM_INT) return 1;
    const char *name = prolog_atom_name(fn->atom_id);
    if (!name) return 1;
    dyn_pred_row_t *row = dyn_pred_find(name, ar->ival);
    if (row) { row->head = (dyn_clause_t *)0; row->tail = (dyn_clause_t *)0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_dyn_retract_cell(void *head_cell)
{
    extern Trail g_resolve_trail;
    Term *pat = term_deref((Term *)head_cell);
    const char *name = (const char *)0; long arity = 0;
    dyn_term_key(pat, &name, &arity);
    if (!name) return 0;
    dyn_pred_row_t *row = dyn_pred_find(name, arity);
    if (!row) return 0;
    dyn_clause_t *prev = (dyn_clause_t *)0;
    for (dyn_clause_t *c = row->head; c; prev = c, c = c->next) {
        int mark = trail_mark(&g_resolve_trail);
        Term *var_map[256]; int var_cap = 256, var_n = 0;
        Term *hcopy = copy_term_deep(c->head, var_map, &var_cap, &var_n);
        if (hcopy && unify(pat, hcopy, &g_resolve_trail)) {
            if (prev) prev->next = c->next; else row->head = c->next;
            if (row->tail == c) row->tail = prev;
            return 1;
        }
        trail_unwind(&g_resolve_trail, mark);
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static dyn_pred_row_t *dyn_pred_intern(const char *name, long arity)
{
    dyn_pred_row_t *r = dyn_pred_find(name, arity);
    if (r) return r;
    if (g_pl_dyn_pred_n >= g_pl_dyn_pred_cap) { g_pl_dyn_pred_cap = g_pl_dyn_pred_cap ? g_pl_dyn_pred_cap * 2 : 8; g_pl_dyn_pred_table = (dyn_pred_row_t *)realloc(g_pl_dyn_pred_table, (size_t)g_pl_dyn_pred_cap * sizeof(dyn_pred_row_t)); }
    r = &g_pl_dyn_pred_table[g_pl_dyn_pred_n++];
    r->name = name; r->arity = arity; r->head = (dyn_clause_t *)0; r->tail = (dyn_clause_t *)0;
    return r;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_dyn_assertz_cell(void *clause_cell, int prepend)
{
    Term *cl = term_deref((Term *)clause_cell);
    if (!cl) return 1;
    Term *h = cl, *b = (Term *)0;
    if (cl->tag == TERM_COMPOUND && cl->compound.arity == 2 && prolog_atom_name(cl->compound.functor) && !strcmp(prolog_atom_name(cl->compound.functor), ":-")) { h = term_deref(cl->compound.args[0]); b = term_deref(cl->compound.args[1]); }
    const char *name = (const char *)0; long arity = 0;
    dyn_term_key(h, &name, &arity);
    if (!name) return 1;
    Term *var_map[256]; int var_cap = 256, var_n = 0;
    Term *hcopy = copy_term_deep(h, var_map, &var_cap, &var_n);
    Term *bcopy = b ? copy_term_deep(b, var_map, &var_cap, &var_n) : (Term *)0;
    dyn_pred_row_t *row = dyn_pred_intern(name, arity);
    dyn_clause_t *node = (dyn_clause_t *)GC_MALLOC(sizeof *node);
    node->head = hcopy ? hcopy : h; node->body = bcopy; node->next = (dyn_clause_t *)0;
    if (prepend) { node->next = row->head; row->head = node; if (!row->tail) row->tail = node; }
    else { if (row->tail) row->tail->next = node; else row->head = node; row->tail = node; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { dyn_clause_t *next; } dyn_cursor_t;
/*--------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_dyn_iter_begin(const char *name, long arity)
{
    dyn_pred_row_t *row = name ? dyn_pred_find(name, arity) : (dyn_pred_row_t *)0;
    dyn_cursor_t *cur = (dyn_cursor_t *)GC_MALLOC(sizeof *cur);
    cur->next = row ? row->head : (dyn_clause_t *)0;
    return cur;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_dyn_iter_step(void *cursor, void **arg_cell0, long arity)
{
    extern Trail g_resolve_trail;
    dyn_cursor_t *cur = (dyn_cursor_t *)cursor;
    if (!cur) return 0;
    while (cur->next) {
        dyn_clause_t *c = cur->next; cur->next = c->next;
        int mark = trail_mark(&g_resolve_trail);
        Term *var_map[256]; int var_cap = 256, var_n = 0;
        Term *hcopy = copy_term_deep(c->head, var_map, &var_cap, &var_n);
        int ok = 1;
        if (arity == 0) { ok = (hcopy && hcopy->tag == TERM_ATOM); }
        else if (!hcopy || hcopy->tag != TERM_COMPOUND || hcopy->compound.arity != (int)arity) { ok = 0; }
        else { for (long i = 0; i < arity && ok; i++) if (!unify((Term *)arg_cell0[i], hcopy->compound.args[i], &g_resolve_trail)) ok = 0; }
        if (ok) return 1;
        trail_unwind(&g_resolve_trail, mark);
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* THROW/CATCH — the single in-flight thrown term (a value, like errno; NOT a control/frame stack:
 * the catch FRAMES are box frame cells, only the ball-in-flight needs to cross C-call returns). The
 * GZ model: throw() copies the ball here + the box fails; failure rides the existing ω/return wiring
 * up to the nearest catch box, which checks this ball on its goal's failure edge. */
static Term *g_pl_throw_ball = (Term *)0;
void rt_pl_throw_set(void *ball_cell)
{
    Term *var_map[256]; int var_cap = 256, var_n = 0;
    Term *b = copy_term_deep(term_deref((Term *)ball_cell), var_map, &var_cap, &var_n);
    g_pl_throw_ball = b ? b : term_deref((Term *)ball_cell);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_throw_pending(void) { return g_pl_throw_ball != (Term *)0; }
/*--------------------------------------------------------------------------------------------------------------------*/
/* unify the catcher cell with the in-flight ball; on success the ball is consumed (cleared) and its
 * bindings remain so the recovery can read them; on mismatch the ball stays pending (re-throw). */
int rt_pl_throw_match(void *catcher_cell)
{
    extern Trail g_resolve_trail;
    if (!g_pl_throw_ball) return 0;
    int mark = trail_mark(&g_resolve_trail);
    if (!unify(term_deref((Term *)catcher_cell), g_pl_throw_ball, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    g_pl_throw_ball = (Term *)0;
    return 1;
}

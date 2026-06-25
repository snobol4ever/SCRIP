#ifndef PL_CELL_CONV_H
#define PL_CELL_CONV_H
/*--------------------------------------------------------------------------------------------------------------------*/
/* PL-DESCR-2 — the cell<->Term bridge.
 *
 * The hot Prolog path (unify / is / cmp / type-test / write) goes fully native on the 16-byte inline cell. The deep,
 * low-frequency builtins (copy_term, =.., functor, sort, numbervars, term_string) build and walk Term trees and are
 * NOT the measured bottleneck (PL-DESCR-0: fib's 324K scalar allocs are all on the hot path, zero compound). Rather
 * than transliterate every deep helper at once, they are wrapped: their BOUND inputs are materialised read-only to a
 * Term view (pl_cell_to_term), their result Term is unified back into the destination cell (pl_unify_term_into_cell).
 * Unbound inputs are handled cell-natively at the call site (pl_is_var + pl_unify into the cell) and so never need a
 * Term view — there is therefore NO writeback-through-a-materialised-var, which keeps the bridge landmine-free.
 *
 *   pl_cell_to_term(c)                 : inline cell  ->  Term* view (read-only; recurses compound arg cells)
 *   pl_unify_term_into_cell(dst,t,tr)  : Term*  ->  inline cell, unified into dst via pl_unify (recurses compounds)
 *
 * Both compound paths allocate a small block of inline arg cells; the includer supplies the allocator as PL_CELL_ALLOC
 * (GC_MALLOC in the runtime, malloc in the isolated test) so this header carries no allocator dependency of its own.
 */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "pl_cell.h"
#include "term.h"
#ifndef PL_CELL_ALLOC
#include <stdlib.h>
#define PL_CELL_ALLOC(nbytes) malloc(nbytes)
#endif
/*-- cell -> Term : read-only materialisation of a (typically bound) inline cell into a Term view --------------------*/
static inline Term *pl_cell_to_term(pl_cell_t *c) {
    pl_cell_t *d = pl_deref(c);
    int t = (int)d->v;
    if (t == DT_PLVAR) return term_new_var((int)d->slen);
    if (t == DT_I)     return term_new_int((long)d->i);
    if (t == DT_A)     return term_new_atom((int)d->i);
    if (t == DT_R)     return term_new_float(d->r);
    if (t == DT_PLREF) {
        int fn = (int)(d->slen >> 16), ar = (int)(d->slen & 0xFFFFu);
        pl_cell_t *aa = (pl_cell_t *)d->p;
        Term **args = (Term **)PL_CELL_ALLOC((size_t)(ar > 0 ? ar : 1) * sizeof(Term *));
        for (int i = 0; i < ar; i++) args[i] = pl_cell_to_term(&aa[i]);
        return term_new_compound(fn, ar, args);
    }
    return term_new_var(-1);
}
/*-- Term -> inline cell : build an inline-cell image of a Term and unify it into dst (recurses compounds) -----------*/
static inline int pl_unify_term_into_cell(pl_cell_t *dst, Term *t, pl_trail_t *trail);
/* Term -> inline-cell WORD, preserving variable sharing: a memo (Term var -> its allocated cell) makes every
 * occurrence of one logic variable collapse to a single cell, so e.g. the copy f(Y,Y) round-trips with Y shared
 * (without the memo each occurrence would mint a fresh unbound cell and A==B would wrongly fail). */
static inline pl_cell_t pl_term_to_cell_word_m(Term *t, Term **vk, pl_cell_t **vv, int *vn, int cap) {
    t = term_deref(t);
    if (!t) return pl_make_int(0);
    if (t->tag == TERM_VAR) {
        for (int i = 0; i < *vn; i++) if (vk[i] == t) return pl_make_ref(vv[i], t->var_slot);
        pl_cell_t *v = (pl_cell_t *)PL_CELL_ALLOC(sizeof(pl_cell_t)); pl_init_var(v, t->var_slot);
        if (*vn < cap) { vk[*vn] = t; vv[*vn] = v; (*vn)++; }
        return *v;
    }
    if (t->tag == TERM_INT)   return pl_make_int((int64_t)t->ival);
    if (t->tag == TERM_ATOM)  return pl_make_atom(t->atom_id);
    if (t->tag == TERM_FLOAT) return pl_make_float(t->fval);
    if (t->tag == TERM_COMPOUND) {
        int fn = t->compound.functor, ar = t->compound.arity;
        pl_cell_t *aa = (pl_cell_t *)PL_CELL_ALLOC((size_t)(ar > 0 ? ar : 1) * sizeof(pl_cell_t));
        for (int i = 0; i < ar; i++) aa[i] = pl_term_to_cell_word_m(t->compound.args[i], vk, vv, vn, cap);
        return pl_make_compound(fn, ar, aa);
    }
    return pl_make_int(0);
}
static inline pl_cell_t pl_term_to_cell_word(Term *t) {
    Term *vk[256]; pl_cell_t *vv[256]; int vn = 0;
    return pl_term_to_cell_word_m(t, vk, vv, &vn, 256);
}
static inline int pl_unify_term_into_cell(pl_cell_t *dst, Term *t, pl_trail_t *trail) {
    pl_cell_t w = pl_term_to_cell_word(t);
    pl_cell_t tmp = w;
    return pl_unify(dst, &tmp, trail);
}
#endif

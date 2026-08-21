#ifndef PL_CELL_CONV_H
#define PL_CELL_CONV_H
#include "pl_cell.h"
#include "term.h"
#ifndef PL_CELL_ALLOC
#include <stdlib.h>
#define PL_CELL_ALLOC(nbytes) malloc(nbytes)
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline Term *pl_cell_to_term(pl_cell_t *c) {
    pl_cell_t *d = pl_deref(c);
    int t = (int)d->v;
    if (pl_cell_unbound(d)) return term_new_var((int)((t == DT_PLVAR) ? d->slen : 0));
    if (t == DT_I)     return term_new_int((long)d->i);
    if (t == DT_A)     return term_new_atom((int)d->i);
    if (t == DT_S)     { extern int prolog_atom_intern(const char *); return term_new_atom(prolog_atom_intern(d->s ? d->s : "")); }
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
typedef struct { pl_cell_t *seen[1024]; int n; } pl_v2t_map;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int pl_v2t_index(pl_v2t_map *m, pl_cell_t *d) {
    for (int i = 0; i < m->n; i++) if (m->seen[i] == d) return i;
    if (m->n < 1024) { m->seen[m->n] = d; return m->n++; }
    return m->n - 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline Term *pl_cell_to_term_named_r(pl_cell_t *c, pl_v2t_map *m) {
    pl_cell_t *d = pl_deref(c);
    int t = (int)d->v;
    if (pl_cell_unbound(d)) return term_new_var(pl_v2t_index(m, d));
    if (t == DT_I)     return term_new_int((long)d->i);
    if (t == DT_A)     return term_new_atom((int)d->i);
    if (t == DT_S)     { extern int prolog_atom_intern(const char *); return term_new_atom(prolog_atom_intern(d->s ? d->s : "")); }
    if (t == DT_R)     return term_new_float(d->r);
    if (t == DT_PLREF) {
        int fn = (int)(d->slen >> 16), ar = (int)(d->slen & 0xFFFFu);
        pl_cell_t *aa = (pl_cell_t *)d->p;
        Term **args = (Term **)PL_CELL_ALLOC((size_t)(ar > 0 ? ar : 1) * sizeof(Term *));
        for (int i = 0; i < ar; i++) args[i] = pl_cell_to_term_named_r(&aa[i], m);
        return term_new_compound(fn, ar, args);
    }
    return term_new_var(pl_v2t_index(m, d));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline Term *pl_cell_to_term_named(pl_cell_t *c) {
    pl_v2t_map m; m.n = 0;
    return pl_cell_to_term_named_r(c, &m);
}
static inline int pl_unify_term_into_cell(pl_cell_t *dst, Term *t, pl_trail_t *trail);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    if (t->tag == TERM_ATOM)  { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name(t->atom_id);
                                pl_cell_t c; c.v = DT_S; c.slen = (uint32_t)(nm ? strlen(nm) : 0); c.s = nm ? nm : ""; return c; }
    if (t->tag == TERM_FLOAT) return pl_make_float(t->fval);
    if (t->tag == TERM_COMPOUND) {
        int fn = t->compound.functor, ar = t->compound.arity;
        pl_cell_t *aa = (pl_cell_t *)PL_CELL_ALLOC((size_t)(ar > 0 ? ar : 1) * sizeof(pl_cell_t));
        for (int i = 0; i < ar; i++) aa[i] = pl_term_to_cell_word_m(t->compound.args[i], vk, vv, vn, cap);
        return pl_make_compound(fn, ar, aa);
    }
    return pl_make_int(0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline pl_cell_t pl_term_to_cell_word(Term *t) {
    Term *vk[256]; pl_cell_t *vv[256]; int vn = 0;
    return pl_term_to_cell_word_m(t, vk, vv, &vn, 256);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int pl_unify_term_into_cell(pl_cell_t *dst, Term *t, pl_trail_t *trail) {
    pl_cell_t w = pl_term_to_cell_word(t);
    pl_cell_t tmp = w;
    return pl_unify(dst, &tmp, trail);
}
#endif

#ifndef PL_CELL_H
#define PL_CELL_H
/*--------------------------------------------------------------------------------------------------------------------*/
/* PL-DESCR-1 — the Prolog inline tagged cell.
 *
 * Prolog adopts SCRIP's 16-byte inline DESCR_t cell (the same shape SNOBOL4/Icon carry in r12/rbx) so that
 * ints / atoms / unbound vars live INLINE — no 24-byte heap `Term`, no `term_new_*` GC allocation, no `rt_*`
 * call on the scalar hot path. Only genuine compounds (`DT_PLREF`) hit the heap. The free `slen` field
 * (unavoidable 4-byte alignment padding between the 4-byte tag `v` and the 8-byte-aligned payload union) is
 * repurposed as Prolog's DISCRIMINATOR (`disc`): the var slot for `DT_PLVAR`, the atom id for `DT_A`, and the
 * packed functor-id⊕arity for `DT_PLREF` — so functor/arity, head-functor match and first-arg clause indexing
 * read it straight from the cell word WITHOUT a heap deref.
 *
 * Layout (16 bytes, reusing DESCR_t):   { v:tag(4) , slen:disc(4) , union payload(8) }
 *
 *   unbound var   { DT_PLVAR , slot      , p = &self }   payload points to its own cell (WAM self-ref)
 *   bound var     { DT_PLVAR , slot      , p = &target}  payload points to the cell it is bound to (ref chain)
 *   int           { DT_I     , 0         , i = ival   }   inline, full 64-bit, no heap
 *   atom          { DT_A     , atom_id   , i = atom_id}   inline interned id (in disc AND payload)
 *   float         { DT_R     , 0         , r = fval   }   inline
 *   compound/list { DT_PLREF , fn<<16|ar , p = heap   }   only genuine structures hit the heap
 *
 * Trail entry = (cell_addr, old 16-byte word); unwind restores the word. Cells are mutated in place.
 *
 * NOTE: DT_PLVAR / DT_PLREF are defined here as constants in the free region of the DTYPE_t value space
 * (after DT_FH=12, before DT_FAIL=99). PL-DESCR-2 promotes them into the DTYPE_t enum in descr.h once the
 * cell is integrated onto the live LVA path; until then this header stays isolated and does not perturb the
 * shared contract used by the SNOBOL4/Icon flat-chain path.
 */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "descr.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DT_PLVAR  13   /* Prolog logic variable (unbound self-ref or bound ref); promote into DTYPE_t in DESCR-2 */
#define DT_PLREF  14   /* Prolog compound/list heap reference */

typedef DESCR_t pl_cell_t;

/*-- the GET/SET funnel: the cell layout lives behind these inlines only -----------------------------------------------*/
static inline int      pl_tag(const pl_cell_t *c)     { return (int)c->v; }
static inline uint32_t pl_disc(const pl_cell_t *c)    { return c->slen; }

/* constructors (the SET side of the funnel) */
static inline pl_cell_t pl_make_int(int64_t v)   { pl_cell_t c; c.v = DT_I;  c.slen = 0;        c.i = v;       return c; }
static inline pl_cell_t pl_make_atom(int id)     { pl_cell_t c; c.v = DT_A;  c.slen = (uint32_t)id; c.i = id;  return c; }
static inline pl_cell_t pl_make_float(double d)  { pl_cell_t c; c.v = DT_R;  c.slen = 0;        c.r = d;       return c; }
static inline pl_cell_t pl_make_compound(int functor, int arity, void *heap) {
    pl_cell_t c; c.v = (DTYPE_t)DT_PLREF; c.slen = (((uint32_t)functor) << 16) | ((uint32_t)arity & 0xFFFFu); c.p = heap; return c;
}
/* an unbound var is a self-referential cell: payload points to the cell's own address */
static inline void pl_init_var(pl_cell_t *c, int slot) { c->v = (DTYPE_t)DT_PLVAR; c->slen = (uint32_t)slot; c->p = (void *)c; }
/* a ref word that makes one var point at another cell (used to alias two vars on unify) */
static inline pl_cell_t pl_make_ref(pl_cell_t *target, int slot) {
    pl_cell_t c; c.v = (DTYPE_t)DT_PLVAR; c.slen = (uint32_t)slot; c.p = (void *)target; return c;
}

/*-- deref: follow the var ref-chain to the binding or the unbound self-ref root --------------------------------------*/
static inline pl_cell_t *pl_deref(pl_cell_t *c) {
    while ((int)c->v == DT_PLVAR && c->p != (void *)c) c = (pl_cell_t *)c->p;
    return c;
}

/*-- type tests (the GET side; all deref first) ----------------------------------------------------------------------*/
static inline int pl_is_var(pl_cell_t *c)      { pl_cell_t *d = pl_deref(c); return (int)d->v == DT_PLVAR; }   /* unbound */
static inline int pl_is_int(pl_cell_t *c)      { pl_cell_t *d = pl_deref(c); return (int)d->v == DT_I; }
static inline int pl_is_atom(pl_cell_t *c)     { pl_cell_t *d = pl_deref(c); return (int)d->v == DT_A; }
static inline int pl_is_float(pl_cell_t *c)    { pl_cell_t *d = pl_deref(c); return (int)d->v == DT_R; }
static inline int pl_is_compound(pl_cell_t *c) { pl_cell_t *d = pl_deref(c); return (int)d->v == DT_PLREF; }

/* value extractors (read the deref'd cell) */
static inline int64_t pl_int_val(pl_cell_t *c)  { return pl_deref(c)->i; }
static inline int     pl_atom_id(pl_cell_t *c)  { return (int)pl_deref(c)->i; }
static inline double  pl_float_val(pl_cell_t *c) { return pl_deref(c)->r; }

/* functor/arity straight from the discriminator — NO heap deref (the indexing win) */
static inline int pl_functor(pl_cell_t *c) { return (int)(pl_deref(c)->slen >> 16); }
static inline int pl_arity(pl_cell_t *c)   { return (int)(pl_deref(c)->slen & 0xFFFFu); }
static inline void *pl_compound_heap(pl_cell_t *c) { return pl_deref(c)->p; }

/*-- trail: (addr, old 16-byte word) pairs; unwind restores the word --------------------------------------------------*/
typedef struct { pl_cell_t *addr; pl_cell_t old; } pl_trail_ent_t;
typedef struct { pl_trail_ent_t *ents; int top; int cap; } pl_trail_t;

static inline void pl_trail_init(pl_trail_t *t) { t->ents = NULL; t->top = 0; t->cap = 0; }
static inline int  pl_trail_mark(const pl_trail_t *t) { return t->top; }

/* record a cell's current word before it is mutated (geometric ×2 growth — per THE DIRECTIVE, no fixed cap) */
static inline void pl_trail_push(pl_trail_t *t, pl_cell_t *addr) {
    if (t->top >= t->cap) {
        int nc = t->cap ? t->cap * 2 : 16;
        pl_trail_ent_t *ne = (pl_trail_ent_t *)realloc(t->ents, (size_t)nc * sizeof(pl_trail_ent_t));
        if (!ne) return;
        t->ents = ne; t->cap = nc;
    }
    t->ents[t->top].addr = addr;
    t->ents[t->top].old  = *addr;
    t->top++;
}

/* unwind to a mark: restore each trailed word, newest first */
static inline void pl_trail_unwind(pl_trail_t *t, int mark) {
    while (t->top > mark) {
        t->top--;
        *t->ents[t->top].addr = t->ents[t->top].old;
    }
}

/*-- bind: trail the var cell's old word, then overwrite it in place with `word` ---------------------------------------*/
static inline void pl_bind(pl_cell_t *cell, pl_cell_t word, pl_trail_t *trail) {
    pl_cell_t *v = pl_deref(cell);          /* bind the unbound root, not an interior ref */
    pl_trail_push(trail, v);
    *v = word;
}

#endif

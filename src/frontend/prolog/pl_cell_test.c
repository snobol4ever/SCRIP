#include "pl_cell.h"
#include <stdio.h>
#include <assert.h>
static int fails = 0;
#define CHECK(cond, msg) do { if (!(cond)) { printf("  FAIL: %s\n", msg); fails++; } else { printf("  ok:   %s\n", msg); } } while (0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    printf("=== PL-DESCR-1 inline-cell test ===\n");
    CHECK(sizeof(pl_cell_t) == 16, "sizeof(pl_cell_t) == 16");
    pl_cell_t i42 = pl_make_int(42);
    CHECK(pl_is_int(&i42) && pl_int_val(&i42) == 42, "inline int 42 round-trips");
    pl_cell_t fl  = pl_make_float(3.5);
    CHECK(pl_is_float(&fl) && pl_float_val(&fl) == 3.5, "inline float 3.5 round-trips");
    pl_cell_t at  = pl_make_atom(7);
    CHECK(pl_is_atom(&at) && pl_atom_id(&at) == 7, "inline atom id 7 round-trips");
    CHECK(pl_disc(&at) == 7, "atom id readable from disc without payload");
    pl_cell_t V;  pl_init_var(&V, 0);
    CHECK(pl_is_var(&V), "fresh var is unbound");
    CHECK(pl_deref(&V) == &V, "unbound var derefs to its own self-ref root");
    pl_trail_t tr; pl_trail_init(&tr);
    int m0 = pl_trail_mark(&tr);
    pl_bind(&V, pl_make_int(99), &tr);
    CHECK(pl_is_int(&V) && pl_int_val(&V) == 99, "after bind, var derefs to int 99");
    CHECK(pl_trail_mark(&tr) == m0 + 1, "one trail entry recorded by bind");
    pl_trail_unwind(&tr, m0);
    CHECK(pl_is_var(&V) && pl_deref(&V) == &V, "after unwind, var is unbound again");
    CHECK(pl_trail_mark(&tr) == m0, "trail back to mark after unwind");
    pl_cell_t A, B; pl_init_var(&A, 1); pl_init_var(&B, 2);
    int m1 = pl_trail_mark(&tr);
    pl_bind(&A, pl_make_ref(&B, 1), &tr);
    CHECK(pl_deref(&A) == &B, "A derefs through the ref chain to B");
    CHECK(pl_is_var(&A), "A still unbound (B unbound) via chain");
    pl_bind(&B, pl_make_int(7), &tr);
    CHECK(pl_is_int(&A) && pl_int_val(&A) == 7, "binding B makes A deref to int 7");
    pl_trail_unwind(&tr, m1);
    CHECK(pl_is_var(&A) && pl_deref(&A) == &A, "after unwind, A unbound self-ref again");
    CHECK(pl_is_var(&B) && pl_deref(&B) == &B, "after unwind, B unbound self-ref again");
    int dummy_heap;
    pl_cell_t cmp = pl_make_compound(1234, 3, &dummy_heap);
    CHECK(pl_is_compound(&cmp), "compound cell tagged DT_PLREF");
    CHECK(pl_functor(&cmp) == 1234 && pl_arity(&cmp) == 3, "functor/arity read from disc (no heap deref)");
    CHECK(pl_compound_heap(&cmp) == &dummy_heap, "heap pointer intact in payload");
    pl_trail_t big; pl_trail_init(&big);
    pl_cell_t scratch; pl_init_var(&scratch, 0);
    for (int k = 0; k < 1000; k++) pl_trail_push(&big, &scratch);
    CHECK(pl_trail_mark(&big) == 1000, "trail grew to 1000 entries with no fixed cap");
    pl_trail_t u; pl_trail_init(&u);
    pl_cell_t P, Q; pl_init_var(&P, 0); pl_init_var(&Q, 1);
    int mu0 = pl_trail_mark(&u);
    CHECK(pl_unify(&P, &Q, &u), "unify(var P, var Q) succeeds");
    pl_cell_t seven = pl_make_int(7);
    CHECK(pl_unify(&P, &seven, &u), "unify(P, 7) succeeds");
    CHECK(pl_is_int(&Q) && pl_int_val(&Q) == 7, "Q sees 7 through the P-Q alias");
    pl_trail_unwind(&u, mu0);
    CHECK(pl_is_var(&P) && pl_is_var(&Q) && pl_deref(&P) == &P && pl_deref(&Q) == &Q, "unwind restores P and Q unbound");
    pl_cell_t i3a = pl_make_int(3), i3b = pl_make_int(3), i9 = pl_make_int(9);
    CHECK(pl_unify(&i3a, &i3b, &u), "unify(3, 3) succeeds");
    CHECK(!pl_unify(&i3a, &i9, &u), "unify(3, 9) fails");
    pl_cell_t a5 = pl_make_atom(5), a5b = pl_make_atom(5), a6 = pl_make_atom(6);
    CHECK(pl_unify(&a5, &a5b, &u), "unify(atom5, atom5) succeeds");
    CHECK(!pl_unify(&a5, &a6, &u), "unify(atom5, atom6) fails");
    CHECK(!pl_unify(&i3a, &a5, &u), "unify(int, atom) fails (distinct tags)");
    pl_cell_t argsL[2], argsR[2];
    pl_init_var(&argsL[0], 0); argsL[1] = pl_make_int(2);
    argsR[0] = pl_make_int(1); pl_init_var(&argsR[1], 1);
    pl_cell_t fL = pl_make_compound(100, 2, argsL);
    pl_cell_t fR = pl_make_compound(100, 2, argsR);
    int mu1 = pl_trail_mark(&u);
    CHECK(pl_unify(&fL, &fR, &u), "unify(f(X,2), f(1,Y)) succeeds");
    CHECK(pl_is_int(&argsL[0]) && pl_int_val(&argsL[0]) == 1, "X bound to 1");
    CHECK(pl_is_int(&argsR[1]) && pl_int_val(&argsR[1]) == 2, "Y bound to 2");
    pl_trail_unwind(&u, mu1);
    CHECK(pl_is_var(&argsL[0]) && pl_is_var(&argsR[1]), "unwind restores X and Y unbound");
    pl_cell_t gL = pl_make_compound(100, 2, argsL);
    pl_cell_t gR = pl_make_compound(200, 2, argsR);
    CHECK(!pl_unify(&gL, &gR, &u), "unify(f/2, g/2) fails on functor mismatch");
    printf("=== %s (%d failure%s) ===\n", fails ? "FAILED" : "PASSED", fails, fails == 1 ? "" : "s");
    return fails ? 1 : 0;
}

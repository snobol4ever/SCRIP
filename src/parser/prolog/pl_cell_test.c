/* PL-DESCR-1 isolated test — exercises the inline cell + trail in isolation (no SCRIP build deps).
 * Build: gcc -I src/contracts -I src/parser/prolog -o /tmp/pl_cell_test src/parser/prolog/pl_cell_test.c && /tmp/pl_cell_test
 * Verifies: 16-byte density, scalar inline (no heap), var bind/deref, trail mark/unwind restore, var aliasing
 * (ref chains), and functor/arity read from the discriminator with NO heap deref.
 */
#include "pl_cell.h"
#include <stdio.h>
#include <assert.h>

static int fails = 0;
#define CHECK(cond, msg) do { if (!(cond)) { printf("  FAIL: %s\n", msg); fails++; } else { printf("  ok:   %s\n", msg); } } while (0)

int main(void) {
    printf("=== PL-DESCR-1 inline-cell test ===\n");

    /* 0. density: the cell is exactly 16 bytes (2x denser than 8-ptr + 24-heap Term) */
    CHECK(sizeof(pl_cell_t) == 16, "sizeof(pl_cell_t) == 16");

    /* 1. inline scalars carry their value with no heap pointer */
    pl_cell_t i42 = pl_make_int(42);
    CHECK(pl_is_int(&i42) && pl_int_val(&i42) == 42, "inline int 42 round-trips");
    pl_cell_t fl  = pl_make_float(3.5);
    CHECK(pl_is_float(&fl) && pl_float_val(&fl) == 3.5, "inline float 3.5 round-trips");
    pl_cell_t at  = pl_make_atom(7);
    CHECK(pl_is_atom(&at) && pl_atom_id(&at) == 7, "inline atom id 7 round-trips");
    CHECK(pl_disc(&at) == 7, "atom id readable from disc without payload");

    /* 2. unbound var derefs to itself */
    pl_cell_t V;  pl_init_var(&V, 0);
    CHECK(pl_is_var(&V), "fresh var is unbound");
    CHECK(pl_deref(&V) == &V, "unbound var derefs to its own self-ref root");

    /* 3. bind var -> int, deref sees the int, then unwind restores unbound */
    pl_trail_t tr; pl_trail_init(&tr);
    int m0 = pl_trail_mark(&tr);
    pl_bind(&V, pl_make_int(99), &tr);
    CHECK(pl_is_int(&V) && pl_int_val(&V) == 99, "after bind, var derefs to int 99");
    CHECK(pl_trail_mark(&tr) == m0 + 1, "one trail entry recorded by bind");
    pl_trail_unwind(&tr, m0);
    CHECK(pl_is_var(&V) && pl_deref(&V) == &V, "after unwind, var is unbound again");
    CHECK(pl_trail_mark(&tr) == m0, "trail back to mark after unwind");

    /* 4. var-to-var aliasing: A points at B (ref chain); bind B -> int; A derefs through to it */
    pl_cell_t A, B; pl_init_var(&A, 1); pl_init_var(&B, 2);
    int m1 = pl_trail_mark(&tr);
    pl_bind(&A, pl_make_ref(&B, 1), &tr);            /* A := ref(B) */
    CHECK(pl_deref(&A) == &B, "A derefs through the ref chain to B");
    CHECK(pl_is_var(&A), "A still unbound (B unbound) via chain");
    pl_bind(&B, pl_make_int(7), &tr);                /* binds B (the deref root of A) */
    CHECK(pl_is_int(&A) && pl_int_val(&A) == 7, "binding B makes A deref to int 7");
    pl_trail_unwind(&tr, m1);
    CHECK(pl_is_var(&A) && pl_deref(&A) == &A, "after unwind, A unbound self-ref again");
    CHECK(pl_is_var(&B) && pl_deref(&B) == &B, "after unwind, B unbound self-ref again");

    /* 5. compound: functor/arity packed in disc, read WITHOUT touching the heap pointer */
    int dummy_heap;
    pl_cell_t cmp = pl_make_compound(/*functor=*/1234, /*arity=*/3, &dummy_heap);
    CHECK(pl_is_compound(&cmp), "compound cell tagged DT_PLREF");
    CHECK(pl_functor(&cmp) == 1234 && pl_arity(&cmp) == 3, "functor/arity read from disc (no heap deref)");
    CHECK(pl_compound_heap(&cmp) == &dummy_heap, "heap pointer intact in payload");

    /* 6. trail grows past its initial cap (geometric, per THE DIRECTIVE) */
    pl_trail_t big; pl_trail_init(&big);
    pl_cell_t scratch; pl_init_var(&scratch, 0);
    for (int k = 0; k < 1000; k++) pl_trail_push(&big, &scratch);
    CHECK(pl_trail_mark(&big) == 1000, "trail grew to 1000 entries with no fixed cap");

    printf("=== %s (%d failure%s) ===\n", fails ? "FAILED" : "PASSED", fails, fails == 1 ? "" : "s");
    return fails ? 1 : 0;
}

/* PL-DESCR-2 bridge test — cell<->Term round-trips in isolation.
 * Build: gcc -I src/contracts -I src/parser/prolog -o /tmp/pl_cell_conv_test \
 *        src/parser/prolog/pl_cell_conv_test.c src/parser/prolog/prolog_atom.c && /tmp/pl_cell_conv_test
 * Verifies: scalar cell->Term->cell identity, compound cell->Term preserves functor/arity/args,
 * Term->cell unify binds an unbound dst, and Term->cell into a mismatching bound dst fails.
 */
#include "pl_cell_conv.h"
#include <stdio.h>
static int fails = 0;
#define CHECK(c, m) do { if (!(c)) { printf("  FAIL: %s\n", m); fails++; } else { printf("  ok:   %s\n", m); } } while (0)
int main(void) {
    printf("=== PL-DESCR-2 cell<->Term bridge test ===\n");
    pl_trail_t tr; pl_trail_init(&tr);

    /* scalar cell -> Term view carries the right tag+value */
    pl_cell_t i42 = pl_make_int(42);
    Term *ti = pl_cell_to_term(&i42);
    CHECK(ti && ti->tag == TERM_INT && ti->ival == 42, "int cell -> Term int 42");
    pl_cell_t a9 = pl_make_atom(9);
    Term *ta = pl_cell_to_term(&a9);
    CHECK(ta && ta->tag == TERM_ATOM && ta->atom_id == 9, "atom cell -> Term atom 9");

    /* compound cell -> Term preserves functor/arity and recurses args */
    pl_cell_t args[2]; args[0] = pl_make_int(1); args[1] = pl_make_atom(2);
    pl_cell_t cmp = pl_make_compound(77, 2, args);
    Term *tc = pl_cell_to_term(&cmp);
    CHECK(tc && tc->tag == TERM_COMPOUND && tc->compound.functor == 77 && tc->compound.arity == 2, "compound cell -> Term f/2");
    CHECK(tc->compound.args[0]->tag == TERM_INT && tc->compound.args[0]->ival == 1, "compound arg0 = int 1");
    CHECK(tc->compound.args[1]->tag == TERM_ATOM && tc->compound.args[1]->atom_id == 2, "compound arg1 = atom 2");

    /* Term -> cell unified into an unbound dst binds it */
    Term *src = term_new_compound(77, 2, (Term **)malloc(2 * sizeof(Term *)));
    src->compound.args[0] = term_new_int(5);
    src->compound.args[1] = term_new_atom(6);
    pl_cell_t dst; pl_init_var(&dst, 0);
    int m0 = pl_trail_mark(&tr);
    CHECK(pl_unify_term_into_cell(&dst, src, &tr), "Term f(5,6) unified into unbound dst");
    CHECK(pl_is_compound(&dst) && pl_functor(&dst) == 77 && pl_arity(&dst) == 2, "dst now compound f/2");
    pl_cell_t *d2 = (pl_cell_t *)pl_compound_heap(&dst);
    CHECK(pl_is_int(&d2[0]) && pl_int_val(&d2[0]) == 5, "dst arg0 = 5");
    CHECK(pl_is_atom(&d2[1]) && pl_atom_id(&d2[1]) == 6, "dst arg1 = 6");
    pl_trail_unwind(&tr, m0);
    CHECK(pl_is_var(&dst), "unwind restores dst unbound");

    /* Term -> cell into a mismatching bound dst fails */
    pl_cell_t bound = pl_make_int(100);
    CHECK(!pl_unify_term_into_cell(&bound, term_new_int(200), &tr), "Term 200 into bound int 100 fails");

    printf("=== %s (%d failure%s) ===\n", fails ? "FAILED" : "PASSED", fails, fails == 1 ? "" : "s");
    return fails ? 1 : 0;
}

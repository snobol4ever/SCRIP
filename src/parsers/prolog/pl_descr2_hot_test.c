#include "pl_cell.h"
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(cond, msg) do { if (!(cond)) { printf("  FAIL: %s\n", msg); fails++; } else { printf("  ok:   %s\n", msg); } } while (0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int hot_is_int(pl_cell_t *lhs, long val, pl_trail_t *tr) {
    if (!lhs) return 0;
    pl_cell_t w = pl_make_int((int64_t)val);
    int m = pl_trail_mark(tr);
    if (!pl_unify(lhs, &w, tr)) { pl_trail_unwind(tr, m); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int hot_is_arith(pl_cell_t *lhs, pl_cell_t *rhs, const char *op, long rhs_ival, pl_trail_t *tr) {
    if (!lhs) return 0;
    double rv = (double)rhs_ival;
    if (rhs) {
        pl_cell_t *t = pl_deref(rhs);
        if ((int)t->v == DT_I) rv = (double)t->i; else if ((int)t->v == DT_R) rv = t->r; else return 0;
        if (!op) {}
        else if (!strcmp(op,"+")) rv = rv + (double)rhs_ival;
        else if (!strcmp(op,"-")) rv = rv - (double)rhs_ival;
        else if (!strcmp(op,"*")) rv = rv * (double)rhs_ival;
        else if (!strcmp(op,"//")||!strcmp(op,"div")) { long li=(long)rv; if(!rhs_ival) return 0; rv=(double)(li/rhs_ival); }
        else if (!strcmp(op,"mod")||!strcmp(op,"rem")) { long li=(long)rv; if(!rhs_ival) return 0; rv=(double)(li%rhs_ival); }
    }
    long ival = (long)rv;
    pl_cell_t w = ((double)ival == rv) ? pl_make_int(ival) : pl_make_float(rv);
    int m = pl_trail_mark(tr);
    if (!pl_unify(lhs, &w, tr)) { pl_trail_unwind(tr, m); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int hot_is_bivar(pl_cell_t *lhs, pl_cell_t *c1, pl_cell_t *c2, const char *op, pl_trail_t *tr) {
    if (!lhs || !c1 || !c2) return 0;
    pl_cell_t *t1 = pl_deref(c1), *t2 = pl_deref(c2);
    double a = ((int)t1->v==DT_I) ? (double)t1->i : ((int)t1->v==DT_R) ? t1->r : -1e300;
    double b = ((int)t2->v==DT_I) ? (double)t2->i : ((int)t2->v==DT_R) ? t2->r : -1e300;
    if (a==-1e300 || b==-1e300) return 0;
    double rv;
    if (!op || !strcmp(op,"+")) rv=a+b; else if (!strcmp(op,"-")) rv=a-b; else if (!strcmp(op,"*")) rv=a*b;
    else if (!strcmp(op,"//")||!strcmp(op,"div")) { long la=(long)a,lb=(long)b; if(!lb) return 0; rv=(double)(la/lb); }
    else return 0;
    long ival = (long)rv;
    pl_cell_t w = ((double)ival == rv) ? pl_make_int(ival) : pl_make_float(rv);
    int m = pl_trail_mark(tr);
    if (!pl_unify(lhs, &w, tr)) { pl_trail_unwind(tr, m); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int hot_cmp(const char *op, pl_cell_t *l, long li, pl_cell_t *r, long ri) {
    if (!op) return 0;
    double a=(double)li, b=(double)ri;
    if (l){pl_cell_t*t=pl_deref(l); if((int)t->v==DT_I)a=(double)t->i; else if((int)t->v==DT_R)a=t->r; else return 0;}
    if (r){pl_cell_t*t=pl_deref(r); if((int)t->v==DT_I)b=(double)t->i; else if((int)t->v==DT_R)b=t->r; else return 0;}
    if (!strcmp(op,"=:=")) return (a==b); if (!strcmp(op,"=\\=")) return (a!=b);
    if (!strcmp(op,"<")) return (a< b);   if (!strcmp(op,">")) return (a> b);
    if (!strcmp(op,"=<")||!strcmp(op,"<=")) return (a<=b); if (!strcmp(op,">=")) return (a>=b);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int hot_unify_const(pl_cell_t *c, int is_atom, long ival, int atom_id, pl_trail_t *tr) {
    if (!c) return 0;
    pl_cell_t w = is_atom ? pl_make_atom(atom_id) : pl_make_int(ival);
    int m = pl_trail_mark(tr);
    if (!pl_unify(c, &w, tr)) { pl_trail_unwind(tr, m); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    printf("=== PL-DESCR-2 sub-flip-2 hot-helper test ===\n");
    pl_trail_t tr; pl_trail_init(&tr);
    pl_cell_t V; pl_init_var(&V, 0);
    int m1 = pl_trail_mark(&tr);
    CHECK(hot_is_int(&V, 7, &tr), "X is 7  (X unbound) succeeds");
    CHECK(pl_is_int(&V) && pl_int_val(&V) == 7, "X bound to 7");
    pl_trail_unwind(&tr, m1);
    CHECK(pl_is_var(&V), "unwind restores X unbound");
    pl_cell_t seven = pl_make_int(7);
    CHECK(hot_is_int(&seven, 7, &tr), "7 is 7 succeeds");
    int m2 = pl_trail_mark(&tr);
    CHECK(!hot_is_int(&seven, 8, &tr), "7 is 8 fails");
    CHECK(pl_trail_mark(&tr) == m2, "failed is/2 left the trail clean (partial-fail unwind)");
    pl_cell_t five = pl_make_int(5), Wp; pl_init_var(&Wp, 1);
    CHECK(hot_is_arith(&Wp, &five, "+", 3, &tr) && pl_int_val(&Wp) == 8, "X is 5+3 -> 8");
    pl_cell_t Wt; pl_init_var(&Wt, 2);
    CHECK(hot_is_arith(&Wt, &five, "*", 2, &tr) && pl_int_val(&Wt) == 10, "X is 5*2 -> 10");
    pl_cell_t sv = pl_make_int(17), Wd; pl_init_var(&Wd, 3);
    CHECK(hot_is_arith(&Wd, &sv, "//", 5, &tr) && pl_int_val(&Wd) == 3, "X is 17//5 -> 3");
    pl_cell_t A = pl_make_int(4), B = pl_make_int(9), Wb; pl_init_var(&Wb, 4);
    CHECK(hot_is_bivar(&Wb, &A, &B, "+", &tr) && pl_int_val(&Wb) == 13, "X is A+B (4+9) -> 13");
    pl_cell_t three = pl_make_int(3), threeF = pl_make_float(3.0), fiveF = pl_make_float(5.0);
    CHECK(hot_cmp("<", &three, 0, &five, 0), "3 < 5 true");
    CHECK(!hot_cmp("<", &five, 0, &three, 0), "5 < 3 false");
    CHECK(hot_cmp("=:=", &three, 0, &threeF, 0), "3 =:= 3.0 true (int/float)");
    CHECK(hot_cmp(">=", &fiveF, 0, &five, 0), "5.0 >= 5 true");
    CHECK(hot_cmp("=<", &three, 0, &three, 0), "3 =< 3 true");
    pl_cell_t U; pl_init_var(&U, 5);
    int m6 = pl_trail_mark(&tr);
    CHECK(hot_unify_const(&U, 1, 0, 42, &tr), "X = atom#42 succeeds");
    CHECK(pl_is_atom(&U) && pl_atom_id(&U) == 42, "X bound to atom 42");
    pl_trail_unwind(&tr, m6);
    CHECK(hot_unify_const(&U, 0, 99, 0, &tr) && pl_is_int(&U) && pl_int_val(&U) == 99, "after unwind, X = 99");
    pl_cell_t a1 = pl_make_atom(10);
    int m7 = pl_trail_mark(&tr);
    CHECK(!hot_unify_const(&a1, 1, 0, 11, &tr), "atom#10 = atom#11 fails");
    CHECK(pl_trail_mark(&tr) == m7, "failed unify-const left the trail clean");
    printf("=== %s (%d failure%s) ===\n", fails ? "FAILED" : "PASSED", fails, fails == 1 ? "" : "s");
    return fails ? 1 : 0;
}

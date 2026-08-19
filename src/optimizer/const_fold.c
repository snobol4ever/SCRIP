#include "const_fold.h"
#include "gen.h"
#include <string.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
extern DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
extern DESCR_t str_concat_fracdigit_d(DESCR_t a, DESCR_t b);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cf_lit_descr(const IR_t * nd, DESCR_t * out) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_INTEGER) { *out = INTVAL(IR_LIT(nd).ival); return 1; }
    if (nd->op == IR_LIT_REAL)    { *out = REALVAL(IR_LIT(nd).dval); return 1; }
    if (nd->op == IR_LIT_STRING)  { const char * s = IR_LIT(nd).sval ? IR_LIT(nd).sval : ""; *out = (DESCR_t){ .v = DT_S, .slen = (uint32_t)strlen(s), .s = s }; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐⭐ CN-14 -- THE FOLDED RESULT IS RE-HOMED INTO COMPILE-OWNED STORAGE, AND THAT IS THE WHOLE REASON THIS FUNCTION EXISTS RATHER THAN A DIRECT STORE.  str_concat_d returns its bytes on the GC HEAP
 * (rt_str_alloc / the sxt extend-in-place path); an IR_LIT_STRING's sval is read by the EMITTER, which in mode 3 bakes the pointer into in-process code, so a compile-time descriptor whose payload the
 * collector may later move or reclaim is a dangling literal by construction.  Copying to malloc is not a new discipline -- it is exactly the storage the deleted cf_join used, kept deliberately, so the
 * fold's SEMANTICS move to the one authority while its STORAGE stays where it already was.  The type-preserving arms (DT_I/DT_R) need no copy: they are values, not payloads. */
static int cf_store_descr(IR_t * nd, DESCR_t r) {
    if (r.v == DT_I) { nd->op = IR_LIT_INTEGER; IR_LIT(nd).ival = r.i; nd->n_operands = 0; return 1; }
    if (r.v == DT_R) { nd->op = IR_LIT_REAL;    IR_LIT(nd).dval = r.r; nd->n_operands = 0; return 1; }
    if (r.v == DT_S || r.v == DT_SNUL) { const char * s = r.s ? r.s : ""; size_t n = r.slen ? (size_t)r.slen : strlen(s); char * c = (char *)malloc(n + 1); if (!c) return 0; memcpy(c, s, n); c[n] = '\0'; nd->op = IR_LIT_STRING; IR_LIT(nd).sval = c; nd->n_operands = 0; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐⭐ CN-14 -- CONCATENATION FOLDS THROUGH THE FUNCTION THE EMITTED CODE ITSELF CALLS, NOT A HAND-ROLLED TWIN.  The deleted cf_join spelled concatenation a SECOND time and could only admit
 * IR_LIT_STRING ⊕ IR_LIT_STRING, so `&S = 'ab'` · `&N = 5` · `&S &N` -- the shape a declared constant actually produces -- stayed a runtime str_concat_d call at every site.  Routing to str_concat_d /
 * str_concat_fracdigit_d (the exact pair bb_binop_concat_slot's bcs_rt_addr() emits) buys the whole coercion contract for free AND CANNOT DRIFT FROM IT: the real conventions stay split at their two
 * entry points ("10." for SPITBOL vs "10.0" for Icon, coerce.c's own ruling), and SPITBOL's TYPE-PRESERVING NULL-STRING IDENTITY -- manual Ch.3 p.22, `(20-17) ''` is the INTEGER 3, not the string "3"
 * -- is honoured because str_concat_d returns the OTHER OPERAND UNCHANGED on a null side.  ⛔ THAT IDENTITY IS THE REASON binop_apply WAS REJECTED FOR THIS ARM after being read: its concat case
 * coerces BOTH sides unconditionally and always returns DT_S, so folding through it would have made the killswitch arms disagree on `X = (20-17) ''` -- a silent wrong answer in exactly the class this
 * rung exists to optimize.  Relational codes are NOT admitted here and must never be: a relop's verdict is a CONTROL EDGE (binop_apply reports it through rel_fail, not through the value), so folding one
 * to a value would delete the statement-failure wire the lowerer built.  Admission stays literal-only, so no operand can be DT_DATA, DT_P or a pattern and the list/pattern arms are unreachable. */
static int cf_binop(IR_t * nd, long code, DESCR_t da, DESCR_t db) {
    if (binop_is_concat(code)) return cf_store_descr(nd, code == BINOP_CONCAT_FRACDIGIT ? str_concat_fracdigit_d(da, db) : str_concat_d(da, db));
    if (code == BINOP_ADD || code == BINOP_SUB || code == BINOP_MUL || code == BINOP_DIV || code == BINOP_MOD) return cf_store_descr(nd, rt_num_arith(da, db, (int)code));
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cf_run(IR_graph_t * g) {
    int total = 0;
    if (!g) return 0;
    for (int pass = 0; pass < 8; pass++) {
        int changed = 0;
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd) continue;
            if (nd->op == IR_BINOP && nd->n_operands == 2 && nd->operands[0] && nd->operands[1]) {
                DESCR_t da, db;
                if (cf_lit_descr(nd->operands[0], &da) && cf_lit_descr(nd->operands[1], &db) && cf_binop(nd, (long)IR_LIT(nd).ival, da, db)) { changed++; total++; }
            }
        }
        if (!changed) break;
    }
    return total;
}

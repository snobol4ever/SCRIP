#include "const_fold.h"
#include "gen.h"
#include <string.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cf_lit_descr(const IR_t * nd, DESCR_t * out) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_INTEGER) { *out = INTVAL(IR_LIT(nd).ival); return 1; }
    if (nd->op == IR_LIT_REAL)    { *out = REALVAL(IR_LIT(nd).dval); return 1; }
    if (nd->op == IR_LIT_STRING)  { const char * s = IR_LIT(nd).sval ? IR_LIT(nd).sval : ""; *out = (DESCR_t){ .v = DT_S, .slen = (uint32_t)strlen(s), .s = s }; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * cf_join(const char * as, const char * bs) {
    size_t al = strlen(as), bl = strlen(bs);
    char * j = (char *)malloc(al + bl + 1);
    if (!j) return j;
    memcpy(j, as, al); memcpy(j + al, bs, bl + 1);
    return j;
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
                IR_t * a = nd->operands[0], * b = nd->operands[1];
                long code = (long)IR_LIT(nd).ival;
                if (binop_is_concat(code)) {
                    if (a->op == IR_LIT_STRING && b->op == IR_LIT_STRING) {
                        char * j = cf_join(IR_LIT(a).sval ? IR_LIT(a).sval : "", IR_LIT(b).sval ? IR_LIT(b).sval : "");
                        if (!j) continue;
                        nd->op = IR_LIT_STRING; IR_LIT(nd).sval = j; nd->n_operands = 0; changed++; total++;
                    }
                } else if (code == BINOP_ADD || code == BINOP_SUB || code == BINOP_MUL || code == BINOP_DIV || code == BINOP_MOD) {
                    DESCR_t da, db;
                    if (cf_lit_descr(a, &da) && cf_lit_descr(b, &db)) {
                        DESCR_t r = rt_num_arith(da, db, (int)code);
                        if (r.v == DT_I)      { nd->op = IR_LIT_INTEGER; IR_LIT(nd).ival = r.i; nd->n_operands = 0; changed++; total++; }
                        else if (r.v == DT_R) { nd->op = IR_LIT_REAL;    IR_LIT(nd).dval = r.r; nd->n_operands = 0; changed++; total++; }
                    }
                }
            }
        }
        if (!changed) break;
    }
    return total;
}

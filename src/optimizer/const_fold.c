#include "const_fold.h"
#include "gen.h"
#include <string.h>
#include <stdlib.h>
extern DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
extern DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
extern DESCR_t str_concat_fracdigit_d(DESCR_t a, DESCR_t b);
extern int core_icn_str_ok(DESCR_t d);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cf_lit_descr(const IR_t * nd, DESCR_t * out) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_INTEGER) { *out = INTVAL(IR_LIT(nd).ival); return 1; }
    if (nd->op == IR_LIT_REAL)    { *out = REALVAL(IR_LIT(nd).dval); return 1; }
    if (nd->op == IR_LIT_STRING)  { const char * s = IR_LIT(nd).sval ? IR_LIT(nd).sval : ""; size_t n = (nd->n_operands > 0 && nd->operands[0] && nd->operands[0]->op == IR_LIT_INTEGER && IR_LIT(nd->operands[0]).ival >= 0) ? (size_t)IR_LIT(nd->operands[0]).ival : strlen(s); *out = (DESCR_t){ .v = DT_S, .slen = (uint32_t)n, .s = s }; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cf_store_descr(IR_graph_t * g, IR_t * nd, DESCR_t r) {
    if (r.v == DT_I) { nd->op = IR_LIT_INTEGER; IR_LIT(nd).ival = r.i; nd->n_operands = 0; return 1; }
    if (r.v == DT_R) { nd->op = IR_LIT_REAL;    IR_LIT(nd).dval = r.r; nd->n_operands = 0; return 1; }
    if (r.v == DT_S || r.v == DT_SNUL) { const char * s = r.s ? r.s : ""; size_t n = r.slen ? (size_t)r.slen : strlen(s); char * c = (char *)malloc(n + 1); if (!c) return 0; memcpy(c, s, n); c[n] = '\0'; nd->op = IR_LIT_STRING; IR_LIT(nd).sval = c; nd->n_operands = 0;
        if (g && n != strlen(c)) { IR_t * ln = IR_node_alloc(g, IR_LIT_INTEGER); if (ln) { IR_LIT(ln).ival = (int64_t) n; ir_operand_push(nd, ln); } }
        return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cf_binop(IR_graph_t * g, IR_t * nd, long code, DESCR_t da, DESCR_t db) {
    if (code == BINOP_LCONCAT) return 0;
    if (code == BINOP_CONCAT_FRACDIGIT && !(core_icn_str_ok(da) && core_icn_str_ok(db))) return 0;
    if (binop_is_concat(code)) return cf_store_descr(g, nd, code == BINOP_CONCAT_FRACDIGIT ? str_concat_fracdigit_d(da, db) : str_concat_d(da, db));
    if (code == BINOP_ADD || code == BINOP_SUB || code == BINOP_MUL || code == BINOP_DIV || code == BINOP_MOD) return cf_store_descr(g, nd, rt_num_arith(da, db, (int)code));
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
                if (cf_lit_descr(nd->operands[0], &da) && cf_lit_descr(nd->operands[1], &db) && cf_binop(g, nd, (long)IR_LIT(nd).ival, da, db)) { changed++; total++; }
            }
        }
        if (!changed) break;
    }
    return total;
}

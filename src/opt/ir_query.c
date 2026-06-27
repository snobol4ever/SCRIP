/*--------------------------------------------------------------------------------------------------------------------*/
#include "ir_query.h"
int scan_pat_is_single_lit(IR_graph_t *pg) {
    if (!pg || !pg->entry || pg->entry->op != IR_MATCH_LIT) return 0;
    int nlit = 0;
    for (int i = 0; i < pg->n; i++) {
        IR_e t = pg->all[i]->op;
        if (t == IR_SUCCEED || t == IR_FAIL) continue;
        if (t == IR_MATCH_LIT) { nlit++; continue; }
        return 0;
    }
    return nlit == 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int scan_val_is_single_lit(IR_graph_t *g) {
    if (!g || !g->entry || g->entry->op != IR_LIT_S) return 0;
    int nlit = 0;
    for (int i = 0; i < g->n; i++) {
        IR_e t = g->all[i]->op;
        if (t == IR_SUCCEED || t == IR_FAIL) continue;
        if (t == IR_LIT_S) { nlit++; continue; }
        return 0;
    }
    return nlit == 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int scan_pat_m3_native_safe(IR_graph_t *pg) {
    if (!pg || !pg->all) return 1;
    for (int i = 0; i < pg->n; i++) {
        IR_t *nd = pg->all[i];
        if (!nd) continue;
        if (nd->op == IR_MATCH_ARBNO || (nd->op == IR_MATCH_FENCE && IR_LIT(nd).ival != 1)
            || nd->op == IR_REF_INVARIANT || nd->op == IR_PATTERN_DEFER) return 0;
        if ((nd->op == IR_MATCH_POS || nd->op == IR_MATCH_LEN || nd->op == IR_MATCH_TAB || nd->op == IR_MATCH_RTAB)
            && IR_LIT(nd).dval != 0.0) return 0;
        if ((nd->op == IR_MATCH_ANY || nd->op == IR_MATCH_NOTANY || nd->op == IR_MATCH_BREAK || nd->op == IR_MATCH_BREAKX)
            && IR_LIT(nd).dval != 0.0) return 0;
        if (nd->op == IR_MATCH_SPAN && IR_LIT(nd).ival == 1) return 0;
    }
    return 1;
}

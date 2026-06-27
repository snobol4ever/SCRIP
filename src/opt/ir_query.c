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
/*--------------------------------------------------------------------------------------------------------------------*/
int gz_node_bounded(const IR_t *g) {
    if (!g) return 1;
    switch (g->op) {
    case IR_CELL_CALL: case IR_CELL_CHOICE: case IR_CELL_FINDALL: case IR_CELL_ITE:
    case IR_CELL_CATCH: case IR_CELL_DYNITER:
    case IR_CELL_CUT:  case IR_CUT:
        return 0;   /* generators + cut-barrier + catch: never collapse their β */
    default: return 1;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t *descr_chain_terminal(IR_t *entry) {
    int guard = 0;
    while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL) && entry->γ.node && guard++ < 512) entry = entry->γ.node;
    IR_t *last = entry;
    guard = 0;
    while (last && last->γ.node && last->γ.node->op != IR_SUCCEED && last->γ.node->op != IR_FAIL && guard++ < 512) last = last->γ.node;
    return last;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int ir_is_generator_kind(IR_e t) {
    switch (t) {
        case IR_TO: case IR_TO_BY: case IR_UPTO: case IR_ALT: case IR_REPALT:
        case IR_BINOP_GEN: case IR_ITERATE: case IR_LIMIT: case IR_PROC_GEN:
        case IR_LIST_BANG: case IR_KEY_GEN: case IR_FIND_GEN: case IR_SEQ_GEN:
        case IR_GATHER: case IR_MAP: case IR_GREP:
            return 1;
        default: return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int subchain_node_is_generator(IR_t *nd) {
    if (!nd) return 0;
    if (ir_is_generator_kind(nd->op)) return 1;
    if (nd->op == IR_SCAN_UPTO || nd->op == IR_SCAN_FIND || nd->op == IR_SCAN_BAL) return 1;
    return 0;
}

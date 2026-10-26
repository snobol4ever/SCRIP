#include "../emitter/emit_head.h"
#include "../emitter/emit_decls.h"
#include "planner.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int planner_chain_arity(const IR_t *n) {
    switch (n->op) {
    case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_ATOM: case IR_LIT_REAL:
    case IR_VAR: case IR_KW_ICON: case IR_KW_ICON_GEN: case IR_KW_SNOBOL4: return 0;
    case IR_BINOP: case IR_TO: return 2;
    case IR_TO_BY: return 3;
    case IR_MAKE_LIST: return n->n_operands;
    case IR_CONJUNCTION: case IR_GOTO: return 0;
    case IR_UNOP: case IR_UNOP_TEST: return 1;
    case IR_ASSIGN: return 1;
    case IR_RETURN: return 1;
    case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN:
    case IR_CALL_ICON: case IR_CALL_SNOBOL4: case IR_CALL_DIRECT: case IR_CALL_PL_LEAF: case IR_CALL_PAS_BI: case IR_CALL_CALLEE_REC: case IR_CALL_FIELD_REC: case IR_CALL_BYNAME:
    case IR_CALL_TRY: case IR_CALL_METHOD: case IR_CALL_REDISP: case IR_CALL_APPLY: case IR_CALL_EVAL: case IR_CALL_GEN:
    case IR_CALL: return n->n_operands;
    case IR_PROC_GEN: return 0;
    case IR_SCAN_TAB: case IR_SCAN_MOVE: case IR_SCAN_POS:
    case IR_SCAN_UPTO: case IR_SCAN_ANY: case IR_SCAN_MANY:
    case IR_SCAN_FIND: case IR_SCAN_MATCH: case IR_SCAN_BAL: return 0;
    default: return -1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ocr_slot_t * ocr_probe(const IR_t * nd) {
    uint32_t m = g_planner.g_ocr_map.len - 1; uint64_t h = (((uint64_t)(uintptr_t)nd >> 4) * 0xff51afd7ed558ccdull) & m;
    for (;;) { ocr_slot_t * t = &CV_AT(g_planner.g_ocr_map, ocr_slot_t, h); if (t->gen != g_planner.g_ocr_gen || t->nd == nd) return t; h = (h + 1) & m; }
}
static void ocr_table(uint32_t c) {
    cv_t old = g_planner.g_ocr_map; cv_t v = { 0, 0, 0, 0 }; v.p = ct_zalloc(c, sizeof(ocr_slot_t)); v.len = c; v.cap = c; v.esz = (uint32_t)sizeof(ocr_slot_t); g_planner.g_ocr_map = v;
    for (uint32_t i = 0; i < old.len; i++) { ocr_slot_t * o = &CV_AT(old, ocr_slot_t, i); if (o->gen == g_planner.g_ocr_gen) { ocr_slot_t * t = ocr_probe(o->nd); t->nd = o->nd;
        t->gen = g_planner.g_ocr_gen; } }
}
static void ocr_push(cv_t * v, int * n, IR_t * x) { cv_reserve(v, (uint32_t)sizeof(IR_t *), (uint64_t)*n + 1, "ocr"); CV_AT(*v, IR_t *, *n) = x; (*n)++; }
static void ocr_clear(void) { g_planner.g_ocr_gen++; g_planner.g_ocr_cnt = 0; if (!g_planner.g_ocr_map.len) ocr_table(1024); }
static int ocr_has(const IR_t * nd) { return ocr_probe(nd)->gen == g_planner.g_ocr_gen; }
static void ocr_add(const IR_t * nd) { ocr_slot_t * t = ocr_probe(nd); if (t->gen == g_planner.g_ocr_gen) return; t->nd = nd; t->gen = g_planner.g_ocr_gen;
    if ((uint64_t)++g_planner.g_ocr_cnt * 2 >= (uint64_t)g_planner.g_ocr_map.len) ocr_table(g_planner.g_ocr_map.len * 2); }
void planner_chain_operands(IR_t * entry) {
    int nc = 0;
    ocr_clear();
    int sv = 0;
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL) && entry->γ.node && guard++ < 8192) entry = entry->γ.node; }
    entry = entry;
    ocr_push(&g_planner.g_ocr_stkv, &sv, entry);
    while (sv > 0) {
        IR_t *c = CV_AT(g_planner.g_ocr_stkv, IR_t *, --sv);
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        int dup = ocr_has(c);
        if (dup) continue;
        ocr_add(c); ocr_push(&g_planner.g_ocr_chain, &nc, c);
        if ((c->op == IR_BINOP) && c->ω.node) ocr_push(&g_planner.g_ocr_stkv, &sv, c->ω.node);
        if ((c->op == IR_CALL || ir_is_call_kind(c->op)) && c->ω.node) ocr_push(&g_planner.g_ocr_stkv, &sv, c->ω.node);
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_UNIFY_CONST || c->op == IR_UNIFY_STRUCT || c->op == IR_UNIFY_FIRST || c->op == IR_UNIFY_VALUE ||
            c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_KW_ASSIGN || c->op == IR_SCAN_TAB || c->op == IR_SCAN_MOVE || c->op == IR_SCAN_POS || c->op == IR_SCAN_MATCH ||
            c->op == IR_SCAN_ANY
             || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE || c->op == IR_VAR) && c->ω.node)
            ocr_push(&g_planner.g_ocr_stkv, &sv, c->ω.node);
        if (c->γ.node) ocr_push(&g_planner.g_ocr_stkv, &sv, c->γ.node);
    }
    for (int i = 0; i < nc; i++) if (ir_is_generator_kind(CV_AT(g_planner.g_ocr_chain, IR_t *, i)->op) && CV_AT(g_planner.g_ocr_chain, IR_t *, i)->ω.node) {
        int present = ocr_has(CV_AT(g_planner.g_ocr_chain, IR_t *, i)->ω.node);
        if (!present) ocr_push(&g_planner.g_ocr_stkv, &sv, CV_AT(g_planner.g_ocr_chain, IR_t *, i)->ω.node); }
    while (sv > 0) {
        IR_t *c = CV_AT(g_planner.g_ocr_stkv, IR_t *, --sv);
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        int dup = ocr_has(c);
        if (dup) continue;
        ocr_add(c); ocr_push(&g_planner.g_ocr_chain, &nc, c);
        if ((c->op == IR_BINOP) && c->ω.node) ocr_push(&g_planner.g_ocr_stkv, &sv, c->ω.node);
        if ((c->op == IR_CALL || ir_is_call_kind(c->op)) && c->ω.node) ocr_push(&g_planner.g_ocr_stkv, &sv, c->ω.node);
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_UNIFY_CONST || c->op == IR_UNIFY_STRUCT || c->op == IR_UNIFY_FIRST || c->op == IR_UNIFY_VALUE ||
            c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_KW_ASSIGN || c->op == IR_SCAN_TAB || c->op == IR_SCAN_MOVE || c->op == IR_SCAN_POS || c->op == IR_SCAN_MATCH ||
            c->op == IR_SCAN_ANY
             || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE || c->op == IR_VAR) && c->ω.node)
            ocr_push(&g_planner.g_ocr_stkv, &sv, c->ω.node);
        if (ir_is_generator_kind(c->op) && c->ω.node) ocr_push(&g_planner.g_ocr_stkv, &sv, c->ω.node);
        if (c->γ.node) ocr_push(&g_planner.g_ocr_stkv, &sv, c->γ.node);
    }
    int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = CV_AT(g_planner.g_ocr_chain, IR_t *, i);
        switch (n->op) { case IR_RETURN: case IR_SUSPEND: case IR_CORET: case IR_COFAIL: sp = 0; continue; default: break; }
        int ar = planner_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        else if (ar >= 1 && sp >= ar) { if (n->n_operands < ar) { n->n_operands = 0; for (int k = ar; k >= 1; k--) ir_operand_push(n, CV_AT(g_planner.g_ocr_stk, IR_t *, sp - k)); } sp -= ar; }
        else if (ar >= 1) { sp = 0; }
        ocr_push(&g_planner.g_ocr_stk, &sp, n);
    }
}
}

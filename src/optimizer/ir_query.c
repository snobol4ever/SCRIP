#include "ir_query.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int ir_value_is_null_string(const IR_t * nd) {
    if (!nd) return 0;
    if (nd->op == IR_CMP_TEST) return 1;
    if (nd->op == IR_IDENT || nd->op == IR_DIFFER) return 1;
    if (nd->op == IR_LIT_STRING) { const char * s = IR_LIT(nd).sval; return (!s || !s[0]) ? 1 : 0; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int ir_is_generator_kind(IR_e t) {
    switch (t) {
        case IR_TO: case IR_TO_BY: case IR_PROC_GEN: case IR_CALL_VALUE: case IR_REPALT: case IR_LIMIT: case IR_ITERATE: case IR_REV_ASSIGN: case IR_REV_ASSIGN_VAR: case IR_REV_SWAP:
        case IR_SCAN:
        case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_MANY: case IR_SCAN_BAL:
        case IR_SCAN_SEQUENCE: case IR_SCAN_ALTERNATE: case IR_DISJUNCTION:
        case IR_MATCH_BEGIN:
        case IR_MATCH_ARB:
        case IR_MATCH_BAL:
        case IR_MATCH_FENCE0:
        case IR_MATCH_FENCE1:
        case IR_MATCH_BREAKX:
        case IR_MATCH_ARBNO:
        case IR_MATCH_DEFER:
        case IR_MATCH_VALUE:
        case IR_MATCH_ASSIGN_COND: case IR_MATCH_ASSIGN_SAVE:
        case IR_MATCH_ALTERNATE:
        case IR_KW_ICON_GEN:
        case IR_CALL_BUILTIN_GEN:
            return 1;
        default: return 0;
    }
}

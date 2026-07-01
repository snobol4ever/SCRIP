#include "ir_query.h"
/*--------------------------------------------------------------------------------------------------------------------*/
int ir_is_generator_kind(IR_e t) {
    switch (t) {
        case IR_TO: case IR_PROC_GEN: case IR_REPALT: case IR_LIMIT: case IR_ITERATE:
        case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_MANY: case IR_SCAN_BAL:
            return 1;
        default: return 0;
    }
}

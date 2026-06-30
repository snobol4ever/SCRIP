#include "ir_query.h"
/*--------------------------------------------------------------------------------------------------------------------*/
int ir_is_generator_kind(IR_e t) {
    switch (t) {
        case IR_TO: case IR_PROC_GEN: case IR_REPALT: case IR_LIMIT: case IR_ITERATE:
            return 1;
        default: return 0;
    }
}

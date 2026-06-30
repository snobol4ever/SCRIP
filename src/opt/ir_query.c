#include "ir_query.h"
/*--------------------------------------------------------------------------------------------------------------------*/
int ir_is_generator_kind(IR_e t) {
    switch (t) {
        case IR_TO: case IR_PROC_GEN:
            return 1;
        default: return 0;
    }
}

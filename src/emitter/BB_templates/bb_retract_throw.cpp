#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_retract_throw() {
    if ((strcmp(_.op_sval ? _.op_sval : "", "retract") == 0 || strcmp(_.op_sval ? _.op_sval : "", "retractall") == 0) && _.op_parts_n >= 1) {
        extern int g_sm_native_unsupported;
        g_sm_native_unsupported = 1;
        return x86("label", _.lbl_α)
             + x86("jmp", "ω")
             + x86("def", "β")
             + x86("jmp", "ω");
    }
    return (strcmp(_.op_sval ? _.op_sval : "", "throw") == 0 && _.op_parts_n >= 1)
         ? x86("label", _.lbl_α)
         + emit_build_compound_term((IR_t *)(intptr_t)_.op_parts_ival[8])
         + x86("mov", "rdi", "rax")
         + x86("call", "rt_throw_term", (uint64_t)(uintptr_t)(void*)rt_throw_term)
         + x86("jmp", "ω")
         + x86("def", "β")
         + x86("jmp", "ω")
         : std::string();
}

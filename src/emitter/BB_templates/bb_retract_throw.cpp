#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_retract_throw_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    if ((strcmp(fn, "retract") == 0 || strcmp(fn, "retractall") == 0) && ir_call_arg(pBB, 0)) {
        extern int g_sm_native_unsupported;
        g_sm_native_unsupported = 1;
        return hdr
             + x86("jmp", "ω")
             + x86("def", "β")
             + x86("jmp", "ω");
    }
    return (strcmp(fn, "throw") == 0 && ir_call_arg(pBB, 0))
         ? hdr
         + emit_build_compound_term(ir_call_arg(pBB, 0))
         + x86("mov", "rdi", "rax")
         + x86("call", "rt_throw_term", (uint64_t)(uintptr_t)(void*)rt_throw_term)
         + x86("jmp", "ω")
         + x86("def", "β")
         + x86("jmp", "ω")
         : std::string();
}

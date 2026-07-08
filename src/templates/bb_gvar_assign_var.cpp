#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
void rt_gvar_assign_var(const char * dst, const char * src);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gvar_assign_var() {
    if (PLATFORM_X86)
        return x86("def",     "α")
             + x86("comment", "IR_ASSIGN_VAR")
             + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval   ? _.op_sval   : ""), _.bb_ls)
             + x86("lea",  "rsi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_a_sval ? _.op_a_sval : ""), _.bb_rs)
             + x86("call", "rt_gvar_assign_var", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, const char *))rt_gvar_assign_var)
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    return std::string();
}

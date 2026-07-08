#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
void rt_indirect_assign_str(const char * holder, const char * str);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_indirect_assign_lit_s() {
    if (PLATFORM_X86)
        return x86("def",     "α")
             + x86("comment", "IR_INDIRECT_ASSIGN_LIT_S")
             + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval   ? _.op_sval   : ""), _.bb_ls)
             + x86("lea",  "rsi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_a_sval ? _.op_a_sval : ""), _.bb_rs)
             + x86("call", "rt_indirect_assign_str", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, const char *))rt_indirect_assign_str)
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    return std::string();
}

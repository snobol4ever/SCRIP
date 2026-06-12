#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
void rt_gvar_assign_int(const char * name, int64_t val);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gvar_assign_lit_i() {
    if (PLATFORM_X86)
        return x86("label", _.lbl_α)
             + x86("comment", "IR_ASSIGN_LIT_I")
             + x86("lea",    "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), _.bb_ls)
             + x86("movabs", "rsi", (uint64_t)_.op_a_ival_sg)
             + x86("call",   "rt_gvar_assign_int", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, int64_t))rt_gvar_assign_int)
             + x86("jmp",    "γ")
             + x86("def",    "β")
             + x86("jmp",    "ω");
    return std::string();
}

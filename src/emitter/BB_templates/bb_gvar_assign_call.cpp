#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
void rt_gvar_assign_descr(const char * name, int64_t lo, int64_t hi);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gvar_assign_call() {
    if (PLATFORM_X86)
        return (_.op_a_slot < 0) ? x86_bomb("bb_gvar_assign_call: op_a_slot==-1 (call result slot not promoted)") :
               x86("label", _.lbl_α)
             + x86("comment", "IR_ASSIGN_CALL")
             + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), _.bb_ls)
             + x86("mov",  "rsi", FRQ(_.op_a_slot))
             + x86("mov",  "rdx", FRQ(_.op_a_slot + 8))
             + x86("call", "rt_gvar_assign_descr", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, int64_t, int64_t))rt_gvar_assign_descr)
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    return std::string();
}

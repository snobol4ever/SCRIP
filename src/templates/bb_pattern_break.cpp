#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
void bb_build_break_blob(const char *name, const char *cset);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_pattern_break() {
    if (!PLATFORM_X86) return std::string();
    if (!_.pat_via_dtp)
        return x86("comment", "IR_PATTERN_BREAK passthrough (chain entry; builder emitted via DTP_ASSIGN)")
             + x86("label",   _.lbl_α)
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    static char bn[24];
    static char bc[24];
    strtab_label(bn, sizeof bn, _.bb_ls ? _.bb_ls : "");
    strtab_label(bc, sizeof bc, _.op_sval ? _.op_sval : "");
    return x86("comment", "IR_PATTERN_BREAK builder")
         + x86("label",   _.lbl_α)
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.bb_ls ? _.bb_ls : ""), bn)
         + x86("lea",  "rsi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), bc)
         + x86("push", "rbx")
         + x86("mov",  "rbx", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "bb_build_break_blob", (uint64_t)(uintptr_t)(void *)(void(*)(const char*,const char*))bb_build_break_blob)
         + x86("mov",  "rsp", "rbx")
         + x86("pop",  "rbx")
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_pattern_break_call(void) { bb_emit_x86(bb_pattern_break()); }

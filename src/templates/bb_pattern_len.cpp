#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
void bb_build_len_blob(const char *name, int I);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_pattern_len() {
    if (!PLATFORM_X86) return std::string();
    if (!_.pat_via_dtp)
        return x86("comment", "IR_PATTERN_LEN passthrough (chain entry; builder emitted via DTP_ASSIGN)")
             + x86("def",     "α")
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    static char b[24];
    strtab_label(b, sizeof b, _.bb_ls ? _.bb_ls : "");
    return x86("comment", "IR_PATTERN_LEN builder")
         + x86("def",     "α")
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.bb_ls ? _.bb_ls : ""), b)
         + x86("mov",  "esi", (long)(int)_.op_ival)
         + x86("push", "rbx")
         + x86("mov",  "rbx", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "bb_build_len_blob", (uint64_t)(uintptr_t)(void *)(void(*)(const char*,int))bb_build_len_blob)
         + x86("mov",  "rsp", "rbx")
         + x86("pop",  "rbx")
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_pattern_len_call(void) { bb_emit_x86(bb_pattern_len()); }

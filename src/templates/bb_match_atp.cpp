#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
extern "C" void rt_at_cursor(const char *varname, int cur_delta);
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_atp() {
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    return !(_.op_sval ? _.op_sval : "")[0] ? std::string()
         : (strtab_label(b, sizeof b, _.op_sval ? _.op_sval : ""),
              x86("comment", "IR_MATCH_ATP")
            + x86("label",   _.lbl_α)
            + x86("mov",  "esi", "r14d")
            + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
            + x86("sub",  "rsp", (long)8)
            + x86("call", "rt_at_cursor", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, int))rt_at_cursor)
            + x86("add",  "rsp", (long)8)
            + x86("jmp",  "γ")
            + x86("def",  "β")
            + x86("jmp",  "ω"));
}

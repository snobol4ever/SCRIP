#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
extern "C" int rt_defer_match(const char *varname, int ival_flag, int cur_delta);
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_defer() {
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    return x86("comment", "IR_MATCH_DEFER")
         + x86("label",   _.lbl_α)
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, _.op_sval ? _.op_sval : ""), b))
         + x86("mov",  "esi", (long)(int)(int64_t)_.op_ival)
         + x86("mov",  "edx", "r14d")
         + x86("push", "rbx")
         + x86("mov",  "rbx", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "rt_defer_match", (uint64_t)(uintptr_t)(void *)(int (*)(const char *, int, int))rt_defer_match)
         + x86("mov",  "rsp", "rbx")
         + x86("pop",  "rbx")
         + x86("test", "eax", "eax")
         + x86("js",   "ω")
         + x86("mov",  "r14d", "eax")
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}

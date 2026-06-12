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
    const char *vn = _.op_sval ? _.op_sval : "";
    if (!vn[0]) return std::string();
    const char *lbl = emit_intern_str(vn);
    static char b[24];
    if (!lbl) { strtab_label(b, sizeof b, vn); lbl = b; }
    uint64_t va = (uint64_t)(uintptr_t)(const void *)vn;
    uint64_t fn; { void (*fp)(const char *, int) = rt_at_cursor; fn = (uint64_t)(uintptr_t)(void *)fp; }
    return x86("comment", "IR_MATCH_ATP")
         + x86("label",   _.lbl_α)
         + x86("mov",  "esi", "r14d")
         + x86("lea",  "rdi", "[rip + __]", va, lbl)
         + x86("sub",  "rsp", (long)8)
         + x86("call", "rt_at_cursor", fn)
         + x86("add",  "rsp", (long)8)
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}

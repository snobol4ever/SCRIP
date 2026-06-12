#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
extern "C" void rt_cap_assign_cursor(const char *varname, int saved_delta, int cur_delta, int is_imm);
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_capture() {
    if (!PLATFORM_X86) return std::string();
    int sk = (int)_.op_ival;
    const char *vn = _.op_sval ? _.op_sval : "";
    if (_.op_off < 0) return x86_bomb("IR_PAT_ASSIGN: start slot not promoted (flat_drive_capture)");
    if (!vn[0])       return x86_bomb("IR_PAT_ASSIGN: empty capture variable name");
    const char *lbl = emit_intern_str(vn);
    static char b[24];
    if (!lbl) { strtab_label(b, sizeof b, vn); lbl = b; }
    uint64_t va = (uint64_t)(uintptr_t)(const void *)vn;
    if (sk == 0) {
        return x86("comment", "IR_MATCH_CAPTURE_SAVE")
             + x86("label",   _.lbl_α)
             + x86("mov", FR(_.op_off), "r14d");
    }
    uint64_t fn; { void (*fp)(const char *, int, int, int) = rt_cap_assign_cursor; fn = (uint64_t)(uintptr_t)(void *)fp; }
    return x86("comment", sk == 2 ? "IR_MATCH_CAPTURE_IMM" : "IR_MATCH_CAPTURE_COND")
         + x86("lea",  "rdi", "[rip + __]", va, lbl)
         + x86("mov",  "esi", FR(_.op_off))
         + x86("mov",  "edx", "r14d")
         + x86("mov",  "ecx", (long)(sk == 2 ? 1 : 0))
         + x86("push", "rbx")
         + x86("mov",  "rbx", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "rt_cap_assign_cursor", fn)
         + x86("mov",  "rsp", "rbx")
         + x86("pop",  "rbx")
         + x86("jmp",  "γ");
}

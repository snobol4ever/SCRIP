#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void rt_cap_assign_cursor(const char *varname, int saved_delta, int cur_delta, int is_imm);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_capture() {
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    return _.op_off < 0 ? x86_bomb("IR_MATCH_ASSIGN: start slot not promoted (flat_drive_capture)")
         : !(_.op_sval ? _.op_sval : "")[0] ? x86_bomb("IR_MATCH_ASSIGN: empty capture variable name")
         : (strtab_label(b, sizeof b, _.op_sval ? _.op_sval : ""), (int)_.op_phase == 0
              ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE")
                + x86("label",   _.lbl_α)
                + x86("mov", FR(_.op_off), "r14d")
                + x86("jmp", "γ") )
              : ( x86("comment", (int)_.op_phase == 2 ? "IR_MATCH_CAPTURE_IMM" : "IR_MATCH_CAPTURE_COND")
                + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
                + x86("mov",  "esi", FR(_.op_off))
                + x86("mov",  "edx", "r14d")
                + x86("mov",  "ecx", (long)((int)_.op_phase == 2 ? 1 : 0))
                + x86("push", "rbx")
                + x86("mov",  "rbx", "rsp")
                + x86("and",  "rsp", -16L)
                + x86("call", "rt_cap_assign_cursor", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, int, int, int))rt_cap_assign_cursor)
                + x86("mov",  "rsp", "rbx")
                + x86("pop",  "rbx")
                + x86("jmp",  "γ") ));
}

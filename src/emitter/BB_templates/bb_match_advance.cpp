#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
extern int64_t kw_anchor;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_advance() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_sa < 0 || _.op_off < 0) return x86_bomb("IR_PAT_MATCH: subject/start slot not promoted (flat_drive_match)");
    uint64_t ka = (uint64_t)(uintptr_t)(const void *)&kw_anchor;
    return x86("comment", "IR_MATCH_ADVANCE")
         + x86("add", FR(_.op_off), (long)1)
         + x86("mov", "eax", FR(_.op_off))
         + x86("cmp", "eax", "r15d")
         + x86("jg",  "ω")
         + x86("mov", "rcx", "[rip + __]", ka, "kw_anchor")
         + x86("mov", "rax", "[rcx]")
         + x86("cmp64", "rax", (long)0)
         + x86("jne", "ω")
         + x86("jmp", "γ");
}

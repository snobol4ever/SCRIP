#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
extern int64_t kw_anchor;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_advance() {
    if (!PLATFORM_X86) return std::string();
    return (_.op_sa < 0 || _.op_off < 0)
         ? x86_bomb("IR_MATCH: subject/start slot not promoted (flat_drive_match)")
         : x86("comment", "IR_MATCH_ADVANCE")
         + x86("add", FR(_.op_off), (long)1)
         + x86("mov", "eax", FR(_.op_off))
         + x86("cmp", "eax", "r15d")
         + x86_omega("jg")
         + x86("mov", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&kw_anchor, "kw_anchor")
         + x86("mov", "rax", "[rcx]")
         + x86("cmp64", "rax", (long)0)
         + x86_omega("jne")
         + x86_gamma();
}

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
static inline int      saoff()          { return _.op_sa; }
static inline int      stoff()          { return _.op_off; }
static inline uint64_t kw_anchor_addr() { return (uint64_t)(uintptr_t)(const void *)&kw_anchor; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_advance_str() {
    if (!PLATFORM_X86) return std::string();
    return (saoff() < 0 || stoff() < 0)
         ? x86_bomb("IR_PAT_MATCH: subject/start slot not promoted (flat_drive_match)")
         : IF(MEDIUM_TEXT, x86("comment", "BOX MATCH ADVANCE  [ch.18 step 6 outer start-loop: start++, bound, &ANCHOR]"))
         + x86("add", FR(stoff()), (long)1)
         + x86("mov", "eax", FR(stoff()))
         + x86("cmp", "eax", "r15d")
         + x86("jg",  "ω")
         + x86("mov", "rcx", "[rip + __]", kw_anchor_addr(), "kw_anchor")
         + x86("mov", "rax", "[rcx]")
         + x86("cmp64", "rax", (long)0)
         + x86("jne", "ω")
         + x86("jmp", "γ");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_match_advance(void) {
    x86_begin();
    bb_emit_x86(bb_match_advance_str());
}

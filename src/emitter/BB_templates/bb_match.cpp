#include <string>
#include <cstring>
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
static inline int      sub_kind()       { return (int)_.op_ival; }
static inline int      saoff()          { return _.op_sa; }
static inline int      stoff()          { return _.op_off; }
static inline uint64_t kw_anchor_addr() { return (uint64_t)(uintptr_t)(const void *)&kw_anchor; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_str() {
    if (!PLATFORM_X86) return std::string();
    if (saoff() < 0 || stoff() < 0) return x86_bomb("IR_PAT_MATCH: subject/start slot not promoted (flat_drive_match)");
    if (sub_kind() == 0) {
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment("# BOX MATCH HEAD  [REG-0 Sigma=r13 delta=r14 Delta=r15; ch.18 step 1 start=0]"))
             + x86("mov", "r13",  FRQ(saoff()))
             + x86("mov", "r15d", FR(saoff() + 8))
             + x86("mov", FR(stoff()), (long)0)
             + x86("lea", "r10", FR(stoff() + 8))
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    if (sub_kind() == 1) {
        return IF(MEDIUM_TEXT, s_comment("# BOX MATCH RETRY  [delta=r14 <- start; falls through into the inline element]"))
             + x86("mov", "r14d", FR(stoff()));
    }
    return IF(MEDIUM_TEXT, s_comment("# BOX MATCH ADVANCE  [ch.18 step 6 outer start-loop: start++, bound, &ANCHOR]"))
         + x86("add", FR(stoff()), (long)1)
         + x86("mov", "eax", FR(stoff()))
         + x86("cmp", "eax", "r15d")
         + x86("jg",  PORT_OMEGA)
         + x86("mov", "rcx", "[rip + __]", kw_anchor_addr(), "kw_anchor")
         + x86("mov", "rax", "[rcx]")
         + x86("cmp64", "rax", (long)0)
         + x86("jne", PORT_OMEGA)
         + x86("jmp", PORT_GAMMA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_match(void) {
    x86_begin();
    bb_emit_x86(bb_match_str());
}

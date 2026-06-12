#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int saoff() { return _.op_sa; }
static inline int stoff() { return _.op_off; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_head() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return (saoff() < 0 || stoff() < 0)
         ? x86_bomb("IR_PAT_MATCH: subject/start slot not promoted (flat_drive_match)")
         : IF(MEDIUM_TEXT,
               x86("label", _.lbl_α)
             + x86("comment", "BOX MATCH HEAD  [REG-0 Sigma=r13 delta=r14 Delta=r15; ch.18 step 1 start=0]"))
         + x86("mov", "r13",  FRQ(saoff()))
         + x86("mov", "r15d", FR(saoff() + 8))
         + x86("mov", FR(stoff()), (long)0)
         + x86("lea", "r10", FR(stoff() + 8))
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
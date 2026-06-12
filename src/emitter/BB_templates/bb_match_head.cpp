#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_head() {
    if (!PLATFORM_X86) return std::string();
    return (_.op_sa < 0 || _.op_off < 0)
         ? x86_bomb("IR_PAT_MATCH: subject/start slot not promoted (flat_drive_match)")
         : x86("comment", "IR_MATCH_HEAD")
         + x86("label",   _.lbl_α)
         + x86("mov", "r13",  FRQ(_.op_sa))
         + x86("mov", "r15d", FR(_.op_sa + 8))
         + x86("mov", FR(_.op_off), (long)0)
         + x86("lea", "r10", FR(_.op_off + 8))
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}

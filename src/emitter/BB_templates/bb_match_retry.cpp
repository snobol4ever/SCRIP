#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_retry() {
    if (!PLATFORM_X86) return std::string();
    return (_.op_sa < 0 || _.op_off < 0)
         ? x86_bomb("IR_PAT_MATCH: subject/start slot not promoted (flat_drive_match)")
         : x86("comment", "IR_MATCH_RETRY")
         + x86("mov", "r14d", FR(_.op_off));
}

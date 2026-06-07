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
static std::string bb_match_retry_str() {
    if (!PLATFORM_X86) return std::string();
    return (saoff() < 0 || stoff() < 0)
         ? x86_bomb("IR_PAT_MATCH: subject/start slot not promoted (flat_drive_match)")
         : IF(MEDIUM_TEXT, x86("comment", "BOX MATCH RETRY  [delta=r14 <- start; falls through into the inline element]"))
         + x86("mov", "r14d", FR(stoff()));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_match_retry(void) {
    x86_begin();
    bb_emit_x86(bb_match_retry_str());
}

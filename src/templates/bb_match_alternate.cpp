#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SN4-PAT-3h ALTERNATE (A | B | C).  A phased cursor gate for the live single-HEAD design:
 *   phase 0 (SAVE)    — at the ALT entry: record r14d (the cursor delta) into this node's own
 *                       scratch slot, then proceed to the first alternative.
 *   phase 1 (RESTORE) — before each subsequent alternative: reload r14d from the saved slot so
 *                       the next alternative starts at the ALT-entry cursor, undoing whatever the
 *                       failed alternative consumed.  (A concatenated alternative like 'a''b' can
 *                       advance the cursor before failing; without this, the next alternative would
 *                       resume mid-input.)  The alternatives themselves chain via ω:
 *                       A.fail -> RESTORE_B -> B, ..., last.fail -> outer-fail. */
std::string bb_match_alternate() {
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
        ? x86_bomb("IR_MATCH_ALTERNATE: cursor slot not promoted (ir_drive_slot_assign)")
        : ((int)_.op_phase == 0
             ? ( x86("comment", "IR_MATCH_ALT_SAVE")
               + x86("label",   _.lbl_α)
               + x86("mov", FR(_.op_off), "r14d")
               + x86("jmp", "γ") )
             : ( x86("comment", "IR_MATCH_ALT_RESTORE")
               + x86("label",   _.lbl_α)
               + x86("mov", "r14d", FR(_.op_off))
               + x86("jmp", "γ") ));
}

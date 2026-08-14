#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* s83 CAPTURE GUT (Lon in-chat, verbatim directive: "delete ALL that CAPTURE code and by doing so break CAPTURE
 * completely. CLEAN out all that nasty code first before you begin anew."): every prior mechanism for
 * IR_MATCH_ASSIGN_SAVE/COND/IMM is DELETED, not patched -- six overlapping arms (anchor x3, ZD-cell x3, fc-cell,
 * legacy rt_cap_push/rt_cap_top array) had accumulated across many sessions and were fighting each other; s82's
 * own attempt to add a SEVENTH arm on top produced two real bugs in one turn (a broken ternary chain, then a
 * SAVE-side hazard-detection asymmetry) BECAUSE the existing tangle made it hard to reason about what any one
 * capture actually does.  DELIBERATE, NAMED REGRESSION: every SNOBOL4 program using `.`/`$` capture now bombs at
 * compile time, loudly, honestly (x86_bomb, not a silent wrong answer) -- this is NOT a mistake, it is the stated
 * first step of a clean rebuild on the s81 RSP/RBP ruling: ordinary captures live on ZETA-SPINE, captures crossing
 * unbounded stack growth (a non-static DEFER or MATCH_VALUE between SAVE and its IMM/COND, per the ALREADY-STAGED
 * op_frame_need field, EARN-1) move into a ZETA-ACTIVATION-FRAME -- ONE mechanism, not seven.  NEXT RUNG (not
 * this commit): the zd_plan-adjacent SAVE-propagation pre-pass named in s82 (op_frame_need only fires on the
 * consumer side today; SAVE needs it propagated from its paired IMM/COND via nd->operands[1]), then the actual
 * SAVE/COND/IMM template bodies built clean against that corrected verdict.  The runtime side (rt_cap_open,
 * rt_cap_push, rt_cap_pop, rt_cap_top, rt_cap_finish, g_capx, c_rt_cap_open) is UNTOUCHED by this commit -- it is
 * simply unreferenced now that nothing here calls it; deleting IT is later work, sequenced behind the NRETURN
 * lowering bug (s82) that currently blocks verifying the *VAR/computed-name half is safe to remove. */
std::string bb_match_capture() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86_alpha()
         + x86_bomb("IR_MATCH_ASSIGN: capture rebuilt clean on the s81 ACTIVATION-FRAME model, not yet re-landed (s83 gut) -- see this file's header comment");
}

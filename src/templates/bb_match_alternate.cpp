#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SN4-PAT-3h ALTERNATE (A | B | C) + ALT-RESUME (ZB-5 v2 prerequisite).  Slot (16B): +0 entry-cursor save
 * (4B), +8 resume continuation (8B ZK_PTR_CODE — the code address of a JOIN's reload arm).
 *   phase 0 (SAVE, the construct's first-allocated tail = its resume surface)
 *     α: record r14d into [slot], jmp γ (→ alternative 0; cursor already current, no reload).
 *     β: jmp qword [slot+8] — resume-in from a failing right neighbour REPLAYS "where forward-failure of
 *        the alternative that succeeded would have gone": the next JOIN's reload arm.  Always written
 *        before any possible resume (a resume-in implies the construct previously succeeded through some
 *        JOIN's α, which marked).
 *   phase 1 (JOIN_i, one per alternative i = 1..n INCLUDING a trailing T = JOIN_n; operand[0] = SAVE)
 *     α (MARK — alternative i-1 just SUCCEEDED): store &L(0) (own reload arm) into [slot+8], jmp ω
 *        (= outer succ).  β aliased with L(0) (forward-fail of alternative i-1 lands here via sno_ω_to,
 *        β-wards since ALTERNATE is generator-kind): reload r14d from [slot], jmp γ (→ alternative i's
 *        entry; for the trailing T, γ = the construct's outer FAIL, β-aware — resume exhaust restores the
 *        entry cursor then fails leftward). */
std::string bb_match_alternate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0) return x86_bomb("IR_MATCH_ALTERNATE: cursor slot not granted (zls)");
    if ((int)_.op_phase == 0)
        return x86("comment", "IR_MATCH_ALT_SAVE")
             + x86("def",     "α")
             + x86("mov", FR(_.op_off), "r14d")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", FRQ(_.op_off + 8));
    return x86("comment", "IR_MATCH_ALT_JOIN")
         + x86("def",     "α")
         + x86("lea", "rax", L(0))
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86("jmp", "ω")
         + x86("def", "β")
         + x86("def", L(0))
         + x86("mov", "r14d", FR(_.op_off))
         + x86("jmp", "γ");
}

#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/rt/rt_coexpr.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------
 * bb_coret -- IR_CORET template (RUNG 4, GOAL-IR-IMMUTABLE-EMIT.md). The BODY of a `create EXPR` reaches this
 * node on its OWN success edge (RUNG 1's lowering routes body.success -> coret, NOT the outer create's γ).
 * JCON ir_CoRet(value, resumeLabel): transmit the produced value to the activator + hand control back to it.
 * op_sa = the produced value's DESCR slot (coret.operand[0], set in emit_drive). The 16-byte dual-width DESCR
 * is loaded into {rdi,rsi} and handed to scrip_coret (rt_coexpr.c), which stores it into the running
 * coexpression's xmit slot and switches control back to scrip_co_current->activator.
 *
 * RESUME (pthread model): unlike a stack-switch design, resuming this coexpression does NOT re-jump to a saved
 * PC -- the body runs on its own pthread, so when the activator's next `@` switches back in, scrip_coret simply
 * RETURNS (its sem_wait unblocks) and execution continues at the instruction after the call. The resume_addr
 * argument is therefore informational only in this model (passed NULL here); it is kept in scrip_coctx_t so
 * RUNG 5's `@`-side has it available if a future non-pthread fast-path ever wants it. The post-call `jmp γ`
 * is a PLACEHOLDER continuation: the body is never entered until RUNG 5 wires `@`, so this jump is unreachable
 * dead code today; RUNG 5 refines it into the generator-resume loop (pump the body generator's β for the next
 * value). Flagged explicitly rather than silently baking a wrong resume edge.
 *--------------------------------------------------------------------------------------------------------------------*/
std::string bb_coret() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_sa < 0) return x86_bomb("bb_coret: no produced-value slot (coret.operand[0] has no DESCR slot -- body value node missing from chain)");
    return x86("comment", "IR_CORET yield")
         + x86("label", _.lbl_α)
         + x86("mov",  "rdi", FRQ(_.op_sa))
         + x86("mov",  "rsi", FRQ(_.op_sa + 8))
         + x86("xor",  "edx", "edx")
         + x86("call", "scrip_coret", (uint64_t)(uintptr_t)(void *)scrip_coret)
         + x86("jmp",  "γ");
}

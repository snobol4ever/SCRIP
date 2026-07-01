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
 * bb_cofail -- IR_COFAIL template (RUNG 4, GOAL-IR-IMMUTABLE-EMIT.md). The BODY of a `create EXPR` reaches this
 * node on its OWN failure edge (RUNG 1's lowering routes body.failure -> cofail). JCON ir_CoFail(): the body is
 * exhausted; mark the coexpression permanently dead and hand control back to the activator signaling failure.
 * scrip_cofail (rt_coexpr.c) sets scrip_co_current->dead and switches to scrip_co_current->activator; it never
 * returns (a dead coexpression is never resumed), so the `jmp ω` after the call is unreachable and exists only
 * to leave a defined terminal edge for the assembler. Like bb_coret, this node is never entered until RUNG 5
 * wires `@` (create does not run the body), so it is emit-complete-but-unexercised, matching the RUNG 1/2/3
 * built-not-yet-reachable rhythm.
 *--------------------------------------------------------------------------------------------------------------------*/
std::string bb_cofail() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_COFAIL exhausted")
         + x86("label", _.lbl_α)
         + x86("call", "scrip_cofail", (uint64_t)(uintptr_t)(void *)scrip_cofail)
         + x86("jmp",  "ω");
}

#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ONE BODY, FLAVOR-INVISIBLE (Lon directive 2026-07-08 session 3: "Remove the x86_port_mode function and all
 * its usage in a top-level template function. That is not allowed in a template. The templates stay untouched
 * for this entire operation.") — the former ZC_PORT_ALLOC arm (state-in-ZLS2-block, hand-placed push/pop) is
 * DELETED, the exact sibling of R2's medium-invisibility: a template never branches on a flavor axis.  Under
 * SCRIP_ZETA_PORT=2 the ZLS2 frame protocol is INJECTED at this template's own port emissions by
 * x86_port_hook (x86_asm.h), driven by this node's zls2_geom grant (zeta_storage.c: BUMP|RESTORE|RELEASE,
 * slot = x86_scratch_off+8, K = 16): x86("def","α") bumps a per-activation arena frame (push+prev-chain+
 * save-slot store), x86("def","β") restores the arena cursor to this activation's frame (release_to(block) —
 * backtrack arrival wholesale-frees every failed successor's frame, the fail-direction release), and
 * x86("jmp","ω") releases the frame itself when op_omega_is_death marks it a true chain-exit death (a
 * mid-pattern ARB's exhaust ω lands on its predecessor's β instead, whose OWN restore reclaims this frame —
 * per-frame release at internal-alias ω sites is exactly the six-decoy-ω trap).  The activation STATE stays
 * in the static graph-frame quad below in every mode — the state-in-block re-entrancy design retires with the
 * deleted arm, losing nothing exercisable (same-node nesting needs DP-7, the recorded HONEST SCOPE caveat);
 * the injected block is pure arena-frame discipline, the seam future trace/assert/GC flavors ride. */
std::string bb_match_arb() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_ARB")
         + x86("def",     "α")
         + x86("mov",    FR(_.x86_scratch_off), (long)0)
         + x86("mov",    "eax", "r14d")
         + x86("mov",    FR(_.x86_scratch_off + 4), "eax")
         + x86("jmp",    "γ")
         + x86("def",    "β")
         + x86("add",    FR(_.x86_scratch_off), (long)1)
         + x86("mov",    "eax", FR(_.x86_scratch_off + 4))
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("cmp",    "eax", "r15d")
         + x86("jg",     L(0))
         + x86("mov",    "r14d", "eax")
         + x86("jmp",    "γ")
         + x86("def",    L(0))
         + x86("mov",    "r14d", FR(_.x86_scratch_off + 4))
         + x86("jmp",    "ω");
}

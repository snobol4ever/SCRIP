#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZLS2 SECOND CONSUMER (Claude Sonnet 5, 2026-07-08) — ARB per-activation frame on the ZLS2 down-growing
 * arena, gated on ZC_PORT_ALLOC (SCRIP_ZETA_PORT=2), fully inert otherwise — the "natural sibling" GOAL-
 * SNOBOL4-BB.md names for extending BB-OWNED-ζ past ARBNO, same save-slot-in-frame pattern bb_match_arbno.cpp's
 * role 0/role 2 already proves, adapted to ARB's simpler two-label (α fresh-entry, β retry-extend) shape with
 * ONE internal exhaust branch (L(0), already named that in the PLAIN arm below) instead of ARBNO's six-role
 * dispatch. The activation's two counters (matched-length, saved-start) move from the ONE static per-node
 * quad (x86_scratch_off — the same re-entrancy hazard ARBNO's SESSION STATE documented) into a per-activation
 * 16B ZLS2 block; the CURRENT activation's block pointer lives in the node's own save-slot
 * (FRQ(x86_scratch_off+8), zeta_storage.c — reuses ARB's existing pad, no quad widening needed, confirmed
 * safe since ARB only ever used 8 of its granted 16 bytes); the block's own header (+0) chains the PREVIOUS
 * activation's pointer, so nested/re-entrant activations of the SAME node push/pop correctly, same as ARBNO.
 * r12 is NEVER repointed. Free is the own-constant pop at L(0), ARB's single already-existing exhaust label —
 * ARB has exactly ONE `jmp "ω"` site in this template (unlike ARBNO's six), so there is no op_omega_is_death
 * ambiguity to sidestep and no L(9)-style extra marker is needed here. Block layout (ARB_ZLS2_K = 16): {+0
 * prev block ptr (8B), +8 matched-length (4B), +12 saved-start (4B)} — exactly 16B, no padding. Fields are
 * written before read on every path (α writes both; β reads/writes matched-length, reads saved-start), so the
 * arena's no-zeroing contract holds. Not yet verified against a recursive-pattern exerciser (same HONEST SCOPE
 * caveat ARBNO's v1 carries — DP-7 is not landed, so same-node nested activation is structurally correct but
 * not yet exercisable by any live corpus program). */
enum { ARB_ZLS2_K = 16 };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_arb() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (x86_port_mode() == ZC_PORT_ALLOC)
        return x86("comment", "IR_MATCH_ARB (ZLS2 per-activation)")
             + x86("def",  "α")
             + x86_zls2_push_call(ARB_ZLS2_K)
             + x86("mov",  "rcx", FRQ(_.x86_scratch_off + 8))
             + x86("mov",  RDQ("rax", 0), "rcx")
             + x86("mov",  FRQ(_.x86_scratch_off + 8), "rax")
             + x86("mov",  RDD("rax", 8), (long)0)
             + x86("mov",  RDD("rax", 12), "r14d")
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("mov",  "rax", FRQ(_.x86_scratch_off + 8))
             + x86("mov",  "ecx", RDD("rax", 8))
             + x86("add",  "ecx", 1L)
             + x86("mov",  RDD("rax", 8), "ecx")
             + x86("mov",  "edx", RDD("rax", 12))
             + x86("add",  "edx", "ecx")
             + x86("cmp",  "edx", "r15d")
             + x86("jg",   L(0))
             + x86("mov",  "r14d", "edx")
             + x86("jmp",  "γ")
             + x86("def",  L(0))
             + x86("mov",  "rax", FRQ(_.x86_scratch_off + 8))
             + x86("mov",  "r14d", RDD("rax", 12))
             + x86("mov",  "rcx", RDQ("rax", 0))
             + x86("mov",  FRQ(_.x86_scratch_off + 8), "rcx")
             + x86_zls2_pop_call(ARB_ZLS2_K)
             + x86("jmp",  "ω");
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

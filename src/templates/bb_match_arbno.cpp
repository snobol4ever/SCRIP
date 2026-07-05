#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZB-5 SN4-PAT ARBNO v1 (deterministic body).  Three phases share IR_MATCH_ARBNO; _.op_phase = IR_LIT.ival,
 * _.op_off = the phase-0 node's zls slot (16B ZK_RAW: +0 entry δ, +4 last-yield δ, +8 cur_before, +12 pad).
 *   phase 0 (G, generator) — α: entry=δ, yield=δ, jmp γ (the null yield; SPITBOL shortest-first).
 *                            β: δ=yield (restore — the right neighbour may have consumed cursor before
 *                            failing), cur_before=δ, jmp ω — G's ω is the BODY-ENTRY edge, repurposed as the
 *                            β-continuation; the construct's real fail exit lives on phase 2.
 *   phase 1 (K, ok)        — body-success landing: δ==cur_before → jmp ω (=F, the 4/28 zero-advance rule),
 *                            else yield=δ, jmp γ (yield one more iteration).
 *   phase 2 (F, exhaust)   — δ=entry, jmp ω (outer fail).  Defines a β ALIAS: body leaves stamp their fail
 *                            edges β-wards (sno_ω_to + generator-kind opcode), so both entries land here.
 * Deterministic bodies exhaust totally — no per-iteration COLLECTION (that is the generator-body v2). */
std::string bb_match_arbno() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0) return x86_bomb("IR_MATCH_ARBNO: cursor slot not granted (zls)");
    if ((int)_.op_phase == 0)
        return x86("comment", "IR_MATCH_ARBNO gen")
             + x86("label",   _.lbl_α)
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("mov", "r14d", FR(_.op_off + 4))
             + x86("mov", FR(_.op_off + 8), "r14d")
             + x86("jmp", "ω");
    if ((int)_.op_phase == 1)
        return x86("comment", "IR_MATCH_ARBNO ok")
             + x86("label",   _.lbl_α)
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("cmp", "r14d", "eax")
             + x86("je",  L(0))
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("jmp", "γ")
             + x86("def", L(0))
             + x86("jmp", "ω");
    return x86("comment", "IR_MATCH_ARBNO exhaust")
         + x86("label",   _.lbl_α)
         + x86("def", "β")
         + x86("mov", "r14d", FR(_.op_off))
         + x86("jmp", "ω");
}

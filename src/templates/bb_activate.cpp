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
 * bb_activate -- IR_ACTIVATE template (RUNG 5, the `@`/resume side, GOAL-IR-IMMUTABLE-EMIT.md). Completes the
 * co-expression feature: RUNG 1-4 built create/coret/cofail ("correct by construction but unexercised"); this
 * box is what finally exercises them. JCON routes `@` through its GENERIC 2-arg operator machinery
 * (irgen.icn ir_binary + interfacegen.icn:105 "Activate"); SCRIP's realization is the same shape-2
 * value-op-plus-runtime-call every binop uses -- marshal operand slots into args, call ONE rt helper, route
 * success/failure by its return -- with the coexpression-specific state living entirely in rt_coexpr.c.
 *
 * Ports: α = activate once. success (helper returned 1): the coret'd DESCR is ALREADY in this node's own
 *            [r12+op_off] slot (the helper wrote through the out-pointer -- no post-copy), jmp γ.
 *            failure (helper returned 0: target dead/exhausted): jmp ω.
 *        β = a resumed `@`-expression FAILS (activation is bounded -- one result per activation; LOWER's
 *            cx->beta = ω already routes real consumers there, this def is the defensive mirror of
 *            bb_create's own unreachable β, same rationale).
 *
 * Operands: op_sa = the coexpression operand's DESCR slot; d0 (offset 0) holds the scrip_coctx_t* exactly
 *           as bb_create stored it (`mov [r12+op_off], rax` -- pointer at d0, d1 unused, RUNG 3's shape).
 *           op_sb = the transmitted value's DESCR slot, or -1 for the unary `@e` form -- then &null
 *           {DT_SNUL, 0} is transmitted inline (DT_SNUL == 0, so two xor's suffice), behaviorally identical
 *           to JCON's synthesized a_Key("null") without needing a literal node in the chain.
 *--------------------------------------------------------------------------------------------------------------------*/
std::string bb_activate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0) return x86_bomb("bb_activate: op_off < 0 (no slot assigned -- IR_ACTIVATE missing from ir_node_produces_value?)");
    if (_.op_sa  < 0) return x86_bomb("bb_activate: no coexpression operand slot (operand[0] unregistered -- LOWER/BFS wiring bug)");
    std::string s = x86("comment", "IR_ACTIVATE")
                  + x86("label",   _.lbl_α)
                  + x86("mov",     "rdi", FRQ(_.op_sa));
    if (_.op_sb >= 0)
        s += x86("mov", "rsi", FRQ(_.op_sb))
           + x86("mov", "rdx", FRQ(_.op_sb + 8));
    else
        s += x86("xor", "esi", "esi")
           + x86("xor", "edx", "edx");
    s += x86_frame_lea("rcx", _.op_off)
       + x86("call", "scrip_coexpr_activate", (uint64_t)(uintptr_t)(void *)scrip_coexpr_activate)
       + x86("test", "rax", "rax")
       + x86("je",   "ω")
       + x86("jmp",  "γ")
       + x86("def",  "β")
       + x86("jmp",  "ω");
    return s;
}

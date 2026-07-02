#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/rt/rt_coexpr.h"
}
#include "x86_asm.h"
std::string xa_coexpr_body_lea(const char * dst);
/*--------------------------------------------------------------------------------------------------------------------
 * bb_create -- IR_CREATE template (RUNG 3, Claude Sonnet 2026-07-01, GOAL-IR-IMMUTABLE-EMIT.md).
 *
 * `create EXPR` (JCON ir_a_Create / RUNG 1's lowering): SUCCEEDS IMMEDIATELY, returning a co-expression
 * value. The body (EXPR) does NOT run here -- only its entry address is captured, packaged with this
 * call's live register-contract state, and handed to scrip_coexpr_create (rt_coexpr.c) to build a
 * scrip_coctx_t. Actually running the body happens on the first `@`/resume -- RUNG 5, not yet designed
 * (see GOAL-IR-IMMUTABLE-EMIT.md's own RUNG 5 note: "@ is undesigned").
 *
 * Ports: α = unconditional success (create has no failure path -- matches JCON's ir_a_Create: a bare
 *            `Goto p.ir.success`, no failure edge at all).
 *        β = no resume from CREATE's own result (RUNG 1: "bounded resume... always fails" -- but per
 *            this goal's DIVISION RULE, resumability is ω-wiring at the LOWER level, already handled
 *            by RUNG 1's `cx->beta = ω`; this template's own β is unreachable in practice and mirrors
 *            bb_enter_init.cpp's "β: jmp ω" shape defensively in case it's ever reached).
 *
 * Frame layout:
 *   [r12+op_off .. +7]   : the scrip_coctx_t* this create-site owns (8 of the 16 granted bytes used --
 *                          same stride as every other value-producer, per ir_node_produces_value).
 *   [r12+op_off2 .. +47] : SCRATCH ONLY, live for the duration of this node's own code -- six consecutive
 *                          8-byte slots holding {r12,r13,r14,r15,rbx,rbp} at create-time, built here
 *                          specifically so &scratch can be passed as scrip_coexpr_create's regs[6]
 *                          argument (SysV requires an array POINTER, not six scalars, to stay within
 *                          the 6-integer-argument-register convention -- see rt_coexpr.h's own comment
 *                          on why the signature is shaped this way). NOT preserved after this node --
 *                          the values are copied into the trampoline's own malloc'd package by
 *                          scrip_coexpr_create before this scratch region could be overwritten by
 *                          whatever chain-node runs next.
 *
 * op_sval_lbl: the body-entry label's NAME (a const char*), resolved by emit.cpp's codegen_flat_chain_body
 * BFS pre-pass (the "nodes[i]->op == IR_CREATE" block, same session) BEFORE this template ever runs --
 * two-phase discovery-then-drive, see that function's own structure. This template does not and cannot
 * resolve the label itself; it only LEAs against the name already placed in op_sval_lbl.
 *--------------------------------------------------------------------------------------------------------------------*/
std::string bb_create() {
    x86_begin();
    if (!PLATFORM_X86) return x86_bomb("bb_create: no x86 platform");
    if (_.op_off < 0) return x86_bomb("bb_create: op_off < 0 (no slot assigned -- IR_CREATE missing from ir_node_produces_value?)");
    if (!_.lbl_t0) return x86_bomb("bb_create: body-entry target (t0 port) is NULL -- codegen_flat_chain_body's IR_CREATE resolution did not thread g_create_body_entry (operand[0] not found in this chain's nodes[]? the BFS operand[0] enqueue may be missing)");

    /* op_off2: a second, independent slot region for the six-register scratch array, reserved
       immediately after op_off's 16 bytes. Not obtained via drive_value_slot (that's for THIS node's
       OWN persistent value, already claimed at op_off) -- this is transient scratch, sized+placed by
       hand exactly as IR_LIMIT/IR_ITERATE size their own adjacent counter slots at op_off+16 (see
       those cases in emit.cpp's emit_drive switch, same pattern, same file). */
    int op_off2 = _.op_off + 16;

    std::string s = x86("comment", "IR_CREATE")
                   + x86("label", _.lbl_α);

    /* Fill the regs[6] scratch array FIRST: {r12,r13,r14,r15,rbx,rbp} at create-time, per this file's own
       top-of-file comment on why all six (not just r12) are captured. ORDER MATTERS (RUNG 5 bring-up fix,
       gdb-bracketed 2026-07-01): the original RUNG 3 code staged the body-entry address at op_off2+40
       BEFORE this loop -- whose k=5 iteration stores rbp at exactly op_off2+40, clobbering the address --
       so the trampoline's `jmp *body_entry_addr` landed on the creator's RBP (a stack address, thread-2
       SIGSEGV at 0x7fffffffe950). Latent since RUNG 3: nothing executed a body until RUNG 5's `@`. The
       staging slot is deleted outright -- the LEA goes straight into rdi AFTER the loop. */
    static const char *contract_regs[6] = {"r12", "r13", "r14", "r15", "rbx", "rbp"};
    for (int k = 0; k < 6; k++) {
        s += x86("mov", "qword ptr [r12 + " + std::to_string(op_off2 + k * 8) + "]", contract_regs[k]);
    }
    /* NOTE: writing through [r12+...] to STORE r12's own value is safe -- the store uses r12 as the base
       register for addressing, which is unaffected by the fact that r12's own value is also part of the
       data being written; this is the same non-issue as `mov [r12+8], r12` on any x86 assembler. */

    /* Capture the body-entry address directly into the call's first argument: rdi = &body_entry_label
       (RIP-relative). The LEA is emitted by the XA bridge xa_coexpr_body_lea (xa_coexpr_entry.cpp), which
       closes RUNG-3's LIMITATION 2: it works in BOTH mediums by LEAing the t0 port (the body-entry α-label
       threaded via g_create_body_entry -> lbl_t0_p by codegen_flat_chain_body, exactly as IR_LIMIT threads
       its generator-β). No MEDIUM_* gating and no hand-encoded bytes here -- all encoding lives in
       x86_lea_tgt inside x86_asm.h, per TEMPLATE-ONLY EMISSION. */
    s += xa_coexpr_body_lea("rdi");

    s += x86_frame_lea("rsi", op_off2)                                                   /* &regs[6] */
       + x86("call", "scrip_coexpr_create", (uint64_t)(uintptr_t)(void *)scrip_coexpr_create)
       + x86("mov",  "qword ptr [r12 + " + std::to_string(_.op_off) + "]", "rax")
       + x86("jmp",  "γ")
       + x86("def",  "β")
       + x86("jmp",  "ω");
    return s;
}

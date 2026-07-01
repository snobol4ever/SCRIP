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
    if (!_.op_sval_lbl) return x86_bomb("bb_create: op_sval_lbl is NULL (body-entry label not resolved by the BFS pre-pass -- operand[0] not found in this chain's nodes[]?)");

    /* op_off2: a second, independent slot region for the six-register scratch array, reserved
       immediately after op_off's 16 bytes. Not obtained via drive_value_slot (that's for THIS node's
       OWN persistent value, already claimed at op_off) -- this is transient scratch, sized+placed by
       hand exactly as IR_LIMIT/IR_ITERATE size their own adjacent counter slots at op_off+16 (see
       those cases in emit.cpp's emit_drive switch, same pattern, same file). */
    int op_off2 = _.op_off + 16;
    bb_flat_cursor_reserve(op_off2 + 48);  /* 6 * 8 bytes */

    std::string s = x86("comment", "IR_CREATE")
                   + x86("label", _.lbl_α);

    /* Capture the body-entry address: rax = &body_entry_label (RIP-relative), stored to op_off2+40
       temporarily so the scratch-fill loop below can treat all 6 register writes uniformly; the
       address itself travels to scrip_coexpr_create as a SEPARATE first argument (rdi), not through
       the regs[6] array -- rax here is just a landing spot before the call marshals it into rdi.
       Mirrors the EXACT lea-then-store idiom already used for generator resume (see emit.cpp's
       g_suspend_resume_slot init block: "lea rax, [rip + beta]" then "mov [r12+slot], rax" --
       this is that same idiom, target is op_sval_lbl instead of a suspend's β). */
    if (MEDIUM_TEXT) {
        s += std::string(" lea rax, [rip + ") + _.op_sval_lbl + "]\n";
    }
    /* BINARY mode (--run) is an honest, correctly-diagnosed bomb below, NOT a guess. The blocker is
       specific and was verified this session, not assumed: emitting a binary-mode RIP-relative LEA
       against op_sval_lbl requires turning that label's STRING NAME into a bb_label_t* that
       bb_emit_patch_rel32 can patch. The patch machinery itself IS reachable from templates
       (bb_emit_patch_rel32 is declared in emit.h and x86_asm.h's bb_emit_x86 already invokes it via
       the 'J' tag) -- but x86_label_for, the only bridge from a template's tagged-bytecode to a
       bb_label_t*, resolves ONLY port-label ids (α/β/γ/ω/t0/t1) and template-internal labels; it has
       NO path from an arbitrary externally-resolved label NAME (a const char*, which is exactly what
       op_sval_lbl is). Bridging that gap correctly means either (a) exporting a "patch-rel32 against a
       label looked up by name" helper from emit.cpp, or (b) threading body_entry as a t0/t1-style port
       (the mechanism IR_LIMIT already uses for its generator-β via lbl_t0_p -- likely the cleaner path,
       and a genuine candidate for the follow-up rung). Rather than hand-roll an unverified byte sequence
       from inside this template file -- the precise "silent corruption if it drifts from emit.cpp's own
       patch application" risk this feature's own notes warn about, and untestable this session without a
       live disassemble/run cycle -- RUNG 3 ships TEXT mode (--compile) working and BINARY mode loudly
       bombing. This is a real, intentional, correctly-attributed scope boundary. */
    if (!MEDIUM_TEXT) return x86_bomb("bb_create: BINARY mode (--run) not yet wired -- op_sval_lbl (a label NAME string) has no bridge to bb_label_t* through x86_label_for, which only resolves port/internal label ids. Candidate fix: thread body-entry as a t0/t1-style port like IR_LIMIT's lbl_t0_p. TEXT mode (--compile) works. See this file's own comment.");

    s += x86("mov", "qword ptr [r12 + " + std::to_string(op_off2 + 40) + "]", "rax");

    /* Fill the regs[6] scratch array: {r12,r13,r14,r15,rbx,rbp} at create-time, per this file's own
       top-of-file comment on why all six (not just r12) are captured. */
    static const char *contract_regs[6] = {"r12", "r13", "r14", "r15", "rbx", "rbp"};
    for (int k = 0; k < 6; k++) {
        s += x86("mov", "qword ptr [r12 + " + std::to_string(op_off2 + k * 8) + "]", contract_regs[k]);
    }
    /* NOTE: writing through [r12+...] to STORE r12's own value is safe -- the store uses r12 as the base
       register for addressing, which is unaffected by the fact that r12's own value is also part of the
       data being written; this is the same non-issue as `mov [r12+8], r12` on any x86 assembler. */

    s += x86("mov",  "rdi", "qword ptr [r12 + " + std::to_string(op_off2 + 40) + "]")   /* body_entry_addr */
       + x86_frame_lea("rsi", op_off2)                                                   /* &regs[6] */
       + x86("call", "scrip_coexpr_create@PLT")
       + x86("mov",  "qword ptr [r12 + " + std::to_string(_.op_off) + "]", "rax")
       + x86("jmp",  "γ")
       + x86("def",  "β")
       + x86("jmp",  "ω");
    return s;
}

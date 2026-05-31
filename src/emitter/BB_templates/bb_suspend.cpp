/* bb_suspend.cpp — BB template for IR_SUSPEND (Raku `take(E)` yield-one in a gather block).
   RK-BB-2 step 4, GOAL-RAKU-BB. One file per BB kind per RULES.md. x86 only.

   Four-port yield-one protocol. The consumer (IR_EVERY pump driving SM_BB_SWITCH(SM_BBSW_RK_GEN))
   re-enters this node's β to advance past the yield; the surrounding IR_SEQ's port-follower then
   transfers control to the NEXT statement (which is typically another IR_SUSPEND or the SEQ's ω
   when the gather body is drained):
     α (fresh-pull   ): evaluate operand E, push DESCR_t onto r12 value-stack, jmp γ
     β (resume-after-): this suspend already yielded; jmp ω so the SEQ-follower advances
     γ (yield path   ): consumer takes value, will re-enter via β
     ω (exhaustion   ): unreachable in body context; SEQ's ω-edge carries exhaustion

   LITERAL FAST-PATH: when the operand is a IR_LIT_I leaf (the only shape produced by today's
   `take(<integer-literal>)`), the integer is read off pBB->α->ival at emit time. No operand-box
   value read, no C Byrd box, no rt_* port-logic helper. Dynamic operands (IR_LIT_S, IR_VAR,
   expressions) are a documented follow-on (mirror bb_to_by's H-3 TODO).

   State: none. Unlike bb_to_by/bb_upto/bb_iterate (which carry a counter for multi-yield), a
   single IR_SUSPEND yields exactly once per α; multi-yield comes from chaining several
   IR_SUSPEND nodes inside a IR_SEQ, NOT from looping inside one node.

   FACT RULE: every byte emitted goes through s_* / bytes() — no seg_byte, no SL_B, no
   sl_emit_one, no emit_standard_blob. PEERS RULE: no fields added to IR_t. */
#include <string>
#include <string.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
void rt_push_int(int64_t v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_suspend_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        /* Literal fast-path detection: operand is IR_LIT_I. */
        int      lit_i = (pBB->α && pBB->α->t == IR_LIT_I);
        int64_t  val_i = lit_i ? pBB->α->ival : 0;

        if (MEDIUM_MACRO_DEF)
            return s_comment("# no macro form — SUSPEND");

        if (!lit_i) {
            /* DYNAMIC / non-integer operand — inline-x86 evaluation not yet wired (mirrors          */
            /* bb_to_by's dynamic-bound passthrough). Wire ports α→γ, β→ω so the graph stays          */
            /* connected; generator yields nothing (the SEQ-follower will see γ on α and ω on β,      */
            /* same shape as a no-op SUSPEND).                                                        */
            if (MEDIUM_TEXT) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment("# BOX SUSPEND(<dynamic operand — inline-x86 TODO>)")
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", _.lbl_ω);
            }
            if (MEDIUM_BINARY) {
                bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
            }
        }

        /* ── LITERAL INTEGER YIELD ── */
        if (MEDIUM_TEXT) {
            /* α: push DESCR_t{DT_I, val_i} onto SM value-stack via rt_push_int@PLT (matches         */
            /* bb_to_by TEXT mode — the r12 raw push is only valid in MEDIUM_BINARY where r12 is      */
            /* set up). β: this yield is done, jump to ω so the SEQ-follower picks up the next stmt. */
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX SUSPEND(take %ld)", (long)val_i))
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov rdi,", emit_fmt("%ld", (long)val_i))
                 + s_2asm("call",     "rt_push_int@PLT")
                 + s_2asm("jmp",      _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",      _.lbl_ω);
        }
        if (MEDIUM_BINARY) {
            /* M3-RK-NOINTERP-1d (Opus 4.8, 2026-05-29): yield DT_I(val_i) via rt_push_int — the      */
            /* SM value-stack ABI used by sm_run_native (mode-3 native), the only consumer of this     */
            /* IR_SUSPEND MEDIUM_BINARY arm today (Raku gather bodies driven by bb_seq's gather         */
            /* multi-yield driver). The prior raw `mov [r12], …` push SEGFAULTED here: neither          */
            /* sm_run_native nor the SM_BB_INVOKE wrapper initialises r12 as a value-stack pointer (it  */
            /* holds an arbitrary callee-saved value), so the r12-relative stores wrote to garbage.     */
            /* Same fix bb_to_by.cpp took in 1a / bb_to.cpp in IBB-3. Helper reached via absolute        */
            /* movabs+call (no PLT in mode-3 native). val_i is a compile-time literal → load to rdi.     */
            /*   α: movabs rdi, val_i      (10 bytes: 48 BF + u64le)                                    */
            /*      movabs rax, &rt_push_int (10 bytes: 48 B8 + u64le)                                  */
            /*      call rax               ( 2 bytes: FF D0)                                            */
            /*      jmp lbl_γ              ( 5 bytes: E9 + rel32 RELOC)                                 */
            /*   β: jmp lbl_ω              ( 5 bytes: E9 + rel32 RELOC) — yield done, SEQ advances      */
            uint64_t push_int_fptr; { void (*fp)(int64_t) = rt_push_int; push_int_fptr = (uint64_t)(uintptr_t)(void*)fp; }
            std::string b;
            b += bytes(2, "\x48\xBF") + u64le((uint64_t)(int64_t)val_i);   /* movabs rdi, val_i        */
            b += bytes(2, "\x48\xB8") + u64le(push_int_fptr);             /* movabs rax, &rt_push_int */
            b += bytes(2, "\xFF\xD0");                                    /* call rax                 */
            int succ_off = (int)b.size();
            b += bytes(5, "\xE9\x00\x00\x00\x00");                         /* jmp lbl_γ RELOC          */
            int back_off = (int)b.size();
            b += bytes(5, "\xE9\x00\x00\x00\x00");                         /* lbl_β: jmp lbl_ω RELOC   */
            bin = { {succ_off+1, back_off, back_off+1},
                    {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p},
                    {false,     true,      false} };
            return b;
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_suspend(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_suspend_str(pBB, bin), bin);
}

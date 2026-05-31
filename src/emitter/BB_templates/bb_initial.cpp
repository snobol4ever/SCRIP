/* bb_initial.cpp — BB template for IR_INITIAL (Icon `initial expr` one-shot at procedure entry).
   IBB-9-INITIAL (Opus 4.8, 2026-05-29). One file per BB kind per RULES.md. x86 only.

   JCON ir_a_Initial (irgen.icn:621-639): run the initial-clause expression on the FIRST entry to the
   procedure body, no-op on every subsequent entry; the clause never fails (expr.failure → p.success).
   Mode-2 reference (bb_exec.c IR_INITIAL): a has-run flag in bb->ival gates the body; first entry sets
   the flag and runs bb->α, then returns bb->γ with value NULVCL.

   Mode-3 transcription: the has-run flag is a RUNTIME slot — &pBB->counter (calloc-zeroed at node
   alloc, persists across slab re-entries since the proc slab is built once and rt_call_proc never
   resets it). The guard slab reads the slot; if non-zero it jmps straight to γ (already ran, skip the
   body); otherwise it sets the slot to 1 and jmps to the body entry. The driver (flat_drive_initial)
   walks pBB->α immediately after this guard with γ=success / ω=failure, so the body's own exit edges
   carry the initial-clause result (failure propagates to ω — matching mode-2's IS_FAIL check).

   EMIT_PAIR contract (set by flat_drive_initial):
     pair[0].jmp     = lbl_γ      (skip target — already ran)
     pair[1].jmp     = body_entry (first-entry → run body)
     pair[2].define  = lbl_β,  pair[2].jmp = lbl_ω   (β-stub → ω)

   Bytes (MEDIUM_BINARY, 39 total):
     off  0 : 48 B8 + u64 slot       movabs rax, &counter
     off 10 : 48 8B 08               mov rcx, [rax]
     off 13 : 48 85 C9               test rcx, rcx
     off 16 : 0F 85 + rel32 skip     jne skip          ← skip patch @ 18
     off 22 : 48 C7 00 + u32 1       mov qword ptr [rax], 1
     off 29 : E9 + rel32 body        jmp body          ← body patch @ 30
     off 34 : E9 + rel32 ω           β: jmp ω          ← β-def @ 34, ω patch @ 35
     off 39 : end
*/
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_initial_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_INITIAL");
    int n = g_emit.xa_bb_emit_pair_n;
    if (n < 3 || !pBB) {
        bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
    }
    bb_label_t *skip     = g_emit.xa_bb_emit_pair_jmp[0];
    bb_label_t *body     = g_emit.xa_bb_emit_pair_jmp[1];
    bb_label_t *beta_def = g_emit.xa_bb_emit_pair_define[2];
    bb_label_t *beta_jmp = g_emit.xa_bb_emit_pair_jmp[2];
    uint64_t slot = (uint64_t)(uintptr_t)&pBB->counter;

    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment("# BOX IR_INITIAL (once-guard: skip body if flag set, else set + run)")
             + s_2asm("movabs rax,", emit_fmt("%llu", (unsigned long long)slot))
             + s_2asm("mov rcx,", "[rax]")
             + s_2asm("test", "rcx, rcx")
             + s_2asm("jne", skip->name)
             + s_2asm("mov qword ptr [rax],", "1")
             + s_2asm("jmp", body->name)
             + s_L1asm(emit_fmt("%s:", beta_def->name), "")
             + s_2asm("jmp", beta_jmp->name);
    }
    if (MEDIUM_BINARY) {
        bin = { {18, 30, 34, 35},
                {skip, body, beta_def, beta_jmp},
                {false, false, true, false} };
        return bytes(2, "\x48\xB8") + u64le(slot)
             + bytes(3, "\x48\x8B\x08")
             + bytes(3, "\x48\x85\xC9")
             + bytes(2, "\x0F\x85") + u32le(0)
             + bytes(3, "\x48\xC7\x00") + u32le(1)
             + bytes(1, "\xE9") + u32le(0)
             + bytes(1, "\xE9") + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_initial(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_initial_str(pBB, bin), bin);
}

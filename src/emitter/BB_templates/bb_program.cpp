/* bb_program.cpp — BB template for the program-level preamble (generic, language-ignorant).
   IBB-5 / GOAL-ICON-BB. One file per BB kind per RULES.md.
   LANGUAGE-IGNORANT: in modes 3/4 the program-preamble box loads the root data pointer
   (`lea r10, [rip + Δ_root_data]`) and jmps into the root BB's α label. The shape is the
   same regardless of frontend — Icon, Prolog, Raku, SNOBOL4 all share this entry contract
   per ARCH-x86.md. No g_lang refs in the body.
   TEXT arm filled 2026-05-31 (register-setup prologue per bb_regs.h). BINARY arm + dispatch wiring
   land with the mode-3 BB-native backend rebuild (excised by SMX-4).
   x86 only — IS_JVM/JS/NET/WASM stub. */
#include <string>
#include "emit_str.h"
#include "bb_regs.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Program preamble — SET the registers per bb_regs.h, THEN jump into BB land. Language-ignorant: every frontend shares this entry contract (ARCH-x86.md). ζ (r12) is the BB-local RW frame base for the
   root sequence; r10 is the per-BLOB data pointer. The subject-model trio (Σ=r13, δ=r14, Δ=r15) is loaded only when a pattern/scan graph is entered — a value/call root (Icon write, Prolog write, SNOBOL4
   OUTPUT=) never scans a subject, so it leaves them untouched. TEXT arm filled; BINARY arm (bin) lands with the mode-3 backend rebuild (x86 BB-native emission was excised by SMX-4, not yet reachable). */
static std::string bb_program_str(IR_t * pBB, bb_bin_t & bin) {
    (void) pBB; bin = {};
    if (!PLATFORM_X86) return std::string();
    const char * root_alpha = _.lbl_α ? _.lbl_α : "Δ_root_α";
    return s_comment("; program preamble: SET registers (bb_regs.h) then enter BB land")
         + s_2asm("mov", BBREG_ZETA ", rsp")
         + s_2asm("lea", BBREG_DATA ", [rip + Δ_root_data]")
         + s_2asm("jmp", root_alpha);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_program(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_program_str(pBB, bin), bin); }

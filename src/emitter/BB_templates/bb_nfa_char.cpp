/* bb_nfa_char.cpp — RK-HY-2 de-cram (2026-06-02, Raku): NFA leaf IR_NFA_CHAR (literal char match).
   Split out of bb_nfa.cpp (one-box-one-file). ISOLATED IR_NFA_* family — NOT shared with SNOBOL4
   pattern opcodes. Per raku_re.c (NK_CHAR, s->ch): match one literal char (pBB->ival) at pos; advance
   pos + γ on hit, ω on miss / end-of-input. The NFA walker (sm_bb_invoke.cpp, gated on gen->t in
   IR_NFA_*) owns the subject preamble + leftmost sweep + reg save/restore and holds r13=pos,
   r14=subject base ptr, r15d=slen across the node chain (all callee-saved, saved by the walker); this
   leaf emits only its own four-port body, with γ/ω/β set per-node by the walker. MEDIUM_BINARY (mode-3
   native) deferred to RK-NFA-5 — mode-3 `~~` still runs via the proven C matcher + byname dispatch.
   Dormant (nothing invokes IR_NFA_* yet). FACT RULE: every byte via s_* and bytes(); no seg_byte/SL_B/
   sl_emit_one/emit_standard_blob. PEERS: no IR_t fields added. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_nfa_char_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — NFA CHAR"))
             + IF(MEDIUM_BINARY,    s_comment("# NFA CHAR mode-3 BINARY deferred to RK-NFA-5"))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment(emit_fmt("# BOX NFA_CHAR '%c' (%d)", (pBB->ival >= 32 && pBB->ival < 127) ? (int)pBB->ival : '?', (int)pBB->ival))
               + s_2asm("cmp", "r13d, r15d")
               + s_2asm("jae", _.lbl_ω)
               + s_2asm("movzx", "eax, byte ptr [r14 + r13]")
               + s_2asm("cmp", emit_fmt("eax, %d", (int)(pBB->ival & 0xff)))
               + s_2asm("jne", _.lbl_ω)
               + s_2asm("inc", "r13")
               + s_2asm("jmp", _.lbl_γ)
               + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_char(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_char_str(pBB, bin), bin);
}

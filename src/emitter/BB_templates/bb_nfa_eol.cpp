/* bb_nfa_eol.cpp — RK-HY-2 de-cram (2026-06-02, Raku): NFA leaf IR_NFA_EOL (Raku `$`).
   Split out of bb_nfa.cpp (one-box-one-file). ISOLATED IR_NFA_* family — NOT shared with SNOBOL4
   pattern opcodes. Per raku_re.c (NK_ANCHOR_EOL): zero-width; advance only if pos==slen → γ iff
   pos==slen, else ω. No char consumed. Walker holds r13=pos, r15d=slen (callee-saved); γ/ω/β set
   per-node by the walker. MEDIUM_BINARY (mode-3 native) deferred to RK-NFA-5. Dormant (nothing
   invokes IR_NFA_* yet). FACT RULE: every byte via s_* and bytes(); no seg_byte/SL_B/sl_emit_one/
   emit_standard_blob. PEERS: no IR_t fields added. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_nfa_eol_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    (void)pBB;
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — NFA EOL"))
             + IF(MEDIUM_BINARY,    s_comment("# NFA EOL mode-3 BINARY deferred to RK-NFA-5"))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX NFA_EOL '$'")
               + s_2asm("cmp", "r13d, r15d")
               + s_2asm("jne", _.lbl_ω)
               + s_2asm("jmp", _.lbl_γ)
               + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_eol(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_eol_str(pBB, bin), bin);
}

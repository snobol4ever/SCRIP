/* bb_nfa_accept.cpp — RK-HY-2 de-cram (2026-06-02, Raku): NFA leaf IR_NFA_ACCEPT (terminal success).
   Split out of bb_nfa.cpp (one-box-one-file). ISOLATED IR_NFA_* family — NOT shared with SNOBOL4
   pattern opcodes. Per raku_re.c (NK_ACCEPT): terminal success (the longest-leftmost winner is the
   ACCEPT state reached at the greatest end pos). γ is the walker's matched-epilogue label (it restores
   the saved regs, pushes the verdict, sets last_ok); pure jmp γ; β→ω like the passthrough leaves. The
   bytes coincide with the EPS/CAP passthrough shape, but ACCEPT is a distinct semantic role (terminal
   vs epsilon join) and keeps its own box per the one-box-one-file convention. γ/ω/β set per-node by
   the walker. MEDIUM_BINARY (mode-3 native) deferred to RK-NFA-5. Dormant (nothing invokes IR_NFA_*
   yet). FACT RULE: every byte via s_* and bytes(); no seg_byte/SL_B/sl_emit_one/emit_standard_blob. PEERS:
   no IR_t fields added. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_nfa_accept_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    (void)pBB;
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — NFA ACCEPT"))
             + IF(MEDIUM_BINARY,    s_comment("# NFA ACCEPT mode-3 BINARY deferred to RK-NFA-5"))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX NFA_ACCEPT")
               + s_2asm("jmp", _.lbl_γ)
               + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_accept(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_accept_str(pBB, bin), bin);
}

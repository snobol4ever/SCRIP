/* bb_nfa_passthrough.cpp — RK-HY-2 de-cram (2026-06-02, Raku): the pure-epsilon NFA leaves
   IR_NFA_EPS / IR_NFA_CAP_OPEN / IR_NFA_CAP_CLOSE. Split out of bb_nfa.cpp (one-box-one-file per
   RULES.md; near-identical boxes that SHARE one _str stay together — the bb_pat_pos POS/RPOS idiom).
   ISOLATED IR_NFA_* family — NOT shared with SNOBOL4's pattern opcodes (a Raku regex bug must never
   touch SNOBOL4's hot path). Per the canonical matcher raku_re.c ss_add: EPS follows out1 (pure
   join); CAP_OPEN/CAP_CLOSE record a capture pos as a SIDE EFFECT then follow out1 — for CONTROL
   FLOW all three are pure epsilon joins (the capture-write block is the follow-up capture-block
   writer, not control flow), so they emit the bb_eps shape: γ on out1, β→ω (no retry). Dormant —
   nothing invokes an IR_NFA_* graph yet (the `~~` path stays on the proven C matcher across all
   three modes); reached only once RK-NFA-4 rewires `~~`. FACT RULE: every byte via s_* and bytes(); no
   seg_byte/SL_B/sl_emit_one/emit_standard_blob. PEERS: no IR_t fields added. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_nfa_passthrough_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    (void)pBB;
    if (PLATFORM_X86) {
        bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return IF(MEDIUM_MACRO_DEF,
               s_comment("# no macro form — NFA passthrough"))
             + IF(MEDIUM_BINARY,
               bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX NFA passthrough (EPS/CAP)")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_eps(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_passthrough_str(pBB, bin), bin);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_cap_open(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_passthrough_str(pBB, bin), bin);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_cap_close(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_passthrough_str(pBB, bin), bin);
}

/* bb_nfa_any.cpp — RK-HY-2 de-cram (2026-06-02, Raku): NFA leaf IR_NFA_ANY (Raku `.`).
   Split out of bb_nfa.cpp (one-box-one-file). ISOLATED IR_NFA_* family — NOT shared with SNOBOL4
   pattern opcodes. Per raku_re.c (NK_ANY): match any char except '\n' at pos; advance pos + γ on hit,
   ω on end / newline. Walker holds r13=pos, r14=subject base ptr, r15d=slen (callee-saved); γ/ω/β set
   per-node by the walker. MEDIUM_BINARY (mode-3 native) deferred to RK-NFA-5 — mode-3 `~~` still runs
   via the C matcher. Dormant (nothing invokes IR_NFA_* yet). FACT RULE: every byte via s_* and bytes();
   no seg_byte/SL_B/sl_emit_one/emit_standard_blob. PEERS: no IR_t fields added. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_nfa_any_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    (void)pBB;
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — NFA ANY"))
             + IF(MEDIUM_BINARY,    s_comment("# NFA ANY mode-3 BINARY deferred to RK-NFA-5"))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX NFA_ANY '.'")
               + s_2asm("cmp", "r13d, r15d")
               + s_2asm("jae", _.lbl_ω)
               + s_2asm("movzx", "eax, byte ptr [r14 + r13]")
               + s_2asm("cmp", "eax, 10")               /* '\n' */
               + s_2asm("je", _.lbl_ω)
               + s_2asm("inc", "r13")
               + s_2asm("jmp", _.lbl_γ)
               + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_any(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_any_str(pBB, bin), bin);
}

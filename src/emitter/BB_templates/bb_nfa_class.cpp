/* bb_nfa_class.cpp — RK-HY-2 de-cram (2026-06-02, Raku): NFA leaf IR_NFA_CLASS (cset membership).
   Split out of bb_nfa.cpp (one-box-one-file). ISOLATED IR_NFA_* family — NOT shared with SNOBOL4
   pattern opcodes. Per raku_re.c (NK_CLASS, s->cc — \d \w \s and their inverses, bracket csets):
   match one char in a 32-byte cset bitset (pBB->sval, the blob from raku_nfa_to_bb) at pos; advance +
   γ on hit, ω on miss / end. The bitset is emitted INLINE as 32 .byte rodata (NOT a movabs of
   pBB->sval — that pointer is valid only inside the compiler process; mode-4 TEXT assembles a SEPARATE
   native binary, so the cset must travel as emitted bytes; this is the READ-ONLY-locals-are-IP-relative
   FACT RULE — a compile-time constant in the sealed segment, reached `lea [rip+disp]`). Membership test
   mirrors raku_cc_test: byte = bits[c>>3], bit = c&7 → `bt edx, eax`. Scratch eax/ecx/edx only
   (caller-saved; the walker's r12-r15 + callee-saved rbx untouched). γ/ω/β set per-node by the walker.
   MEDIUM_BINARY (mode-3 native) deferred to RK-NFA-5. Dormant (nothing invokes IR_NFA_* yet). FACT
   RULE: every byte via s_* and bytes(); no seg_byte/SL_B/sl_emit_one/emit_standard_blob. PEERS: no IR_t
   fields added. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_nfa_class_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    int id = bb_node_id(pBB);
    if (PLATFORM_X86) {
        std::string cs = emit_fmt(".Lnfacc%d", id);
        std::string blob;
        const unsigned char * bits = (const unsigned char *)pBB->sval;
        for (int i = 0; i < 32; i++)
            blob += s_directive(emit_fmt(".byte 0x%02x", bits ? (int)bits[i] : 0));
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — NFA CLASS"))
             + IF(MEDIUM_BINARY,    s_comment("# NFA CLASS mode-3 BINARY deferred to RK-NFA-5"))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX NFA_CLASS [cset]")
               + s_directive(".section .rodata")
               + s_directive(cs + ":")
               + blob
               + s_directive(".section .text")
               + s_directive(".intel_syntax noprefix")
               + s_2asm("cmp", "r13d, r15d")
               + s_2asm("jae", _.lbl_ω)
               + s_2asm("movzx", "eax, byte ptr [r14 + r13]")
               + s_2asm("mov", "ecx, eax")
               + s_2asm("shr", "ecx, 3")
               + s_2asm("lea", emit_fmt("rdx, [rip + %s]", cs.c_str()))
               + s_2asm("movzx", "edx, byte ptr [rdx + rcx]")
               + s_2asm("and", "eax, 7")
               + s_2asm("bt", "edx, eax")
               + s_2asm("jnc", _.lbl_ω)
               + s_2asm("inc", "r13")
               + s_2asm("jmp", _.lbl_γ)
               + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_class(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_class_str(pBB, bin), bin);
}

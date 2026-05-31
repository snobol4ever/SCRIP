/* bb_lit_scalar.cpp — grouped BB template for the scalar-literal family.
   Per HQ Invariant #10 (grouped templates): the four scalar-literal ops
   (IR_LIT_I / IR_LIT_S / IR_LIT_F / IR_LIT_NUL) share an identical emit shape.
   IBB GROUND-ZERO RESET (Opus 4.7, 2026-05-28): the previous empty stub broke
   mode-4 because the SEQ template chains children by per-child labels lbl_α
   that each child template MUST define. A scalar-literal leaf in a γ-chain has
   no runtime work to do (the AG ring / sidecar carries the value), but its
   four-port label must exist for cross-BB jmps to land. Emit a minimal pass-
   through: lbl_α: jmp lbl_γ ; lbl_β: jmp lbl_ω.

   IBB-3 (2026-05-28, value-passing-convention = vstack via rt_push_int):
   IR_LIT_I diverges from the pass-through. In MEDIUM_BINARY it pushes its
   ival onto the runtime vstack via rt_push_int (precedent: bb_to_by.cpp TEXT
   arm calls rt_push_int@PLT for yields). Other lit kinds remain pass-through
   until their consumers exist.

   GZ-2 / R-HW-2 (2026-05-30, GROUND ZERO 3, RO-IP-relative FACT RULE): IR_LIT_I
   and IR_LIT_S are READ-ONLY constants — pure four-port pass-throughs here. Their
   consumer box (write/plus/...) seals the literal next to its OWN blob and reads it
   `[rip+disp]`; nothing is pushed onto the (now-excised) value stack. */
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
void rt_push_real_bits(uint64_t bits);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_lit_scalar_str(IR_t * pBB, bb_bin_t & bin) {
    (void)pBB;
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — BB_LIT_scalar");
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment("# BOX BB_LIT_scalar (pass-through; value carried via AG ring/sidecar)")
             + s_2asm("jmp", _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp", _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        if (pBB && pBB->t == IR_LIT_I) {
            /* GZ-2 (ICON READ-ONLY LOCALS ARE IP-RELATIVE, 2026-05-30): a literal int is a READ-ONLY  */
            /* constant. Per the RO-IP-relative FACT RULE it is NOT written to a runtime slot here;     */
            /* the consumer box (write/plus/...) materializes it as sealed RO data adjacent to its own   */
            /* blob and reads it `[rip+disp]`. So IR_LIT_I is a pure four-port pass-through (like the     */
            /* string-literal leaf): α: jmp γ ; β: jmp ω. (10 bytes; mirrors the generic-lit fall-through */
            /* below and bb_fail/bb_seq(n==0).)                                                          */
            bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
        }
        if (pBB && pBB->t == IR_LIT_S && pBB->sval) {
            /* R-HW-2 (GROUND ZERO 3, 2026-05-30): a literal string is a READ-ONLY constant, exactly  */
            /* like the IR_LIT_I case above. Per the RO-IP-relative FACT RULE it is NOT pushed onto a   */
            /* value stack here; the consumer box (the write strlit arm in bb_call.cpp) seals the bytes */
            /* next to its OWN blob and reads them `lea rdi,[rip+disp]`. So IR_LIT_S is a pure four-port */
            /* pass-through (α: jmp γ ; β: jmp ω). The prior form pushed via rt_push_str onto the        */
            /* now-excised value stack. Identical 10-byte shape to IR_LIT_I and the generic fall-through.*/
            bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
        }
        if (pBB && pBB->t == IR_LIT_F) {
            /* IBB-8c (2026-05-29). Push DT_R(dval) onto runtime vstack via rt_push_real_bits:      */
            /* rt_push_real_bits takes uint64_t (IEEE 754 bit pattern of the double).               */
            /*   movabs rdi, bits   (10 bytes: 48 BF + u64le)                                       */
            /*   movabs rax, &rt_push_real_bits (10 bytes: 48 B8 + u64le)                           */
            /*   call rax           ( 2 bytes: FF D0)                                               */
            /*   jmp γ              ( 5 bytes: E9 + u32le rel32)  — patch site 23                   */
            /*   β: jmp ω           ( 5 bytes: E9 + u32le rel32)  — β-def 27, ω patch 28            */
            /* Total: 32 bytes. Identical layout to IR_LIT_I.                                       */
            uint64_t bits; __builtin_memcpy(&bits, &pBB->dval, 8);
            uint64_t fptr; { void (*fp)(uint64_t) = rt_push_real_bits; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            bin = { {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(2, "\x48\xBF") + u64le(bits)
                 + bytes(2, "\x48\xB8") + u64le(fptr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(1, "\xE9")     + u32le(0)
                 + bytes(1, "\xE9")     + u32le(0);
        }
        /* Other scalar literals: pass-through (10 bytes), mirrors bb_fail.cpp / bb_seq.cpp(n==0). */
        bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_lit_scalar(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_lit_scalar_str(pBB, bin), bin); }

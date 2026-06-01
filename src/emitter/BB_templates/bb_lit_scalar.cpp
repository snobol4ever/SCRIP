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
#include "descr.h"
void rt_push_real_bits(uint64_t bits);
int  bb_slot_alloc(IR_t * nd);
int  bb_slot_alloc16(IR_t * nd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_lit_scalar_str(IR_t * pBB, bb_bin_t & bin) {
    (void)pBB;
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — BB_LIT_scalar");
    /* GZ-7 (GROUND ZERO 3) flat-chain slot model: a literal int is a producer box that writes its value */
    /* into its OWN ζ=r12 frame slot as a 16-byte DESCR so a downstream consumer (ASSIGN / write / binop) */
    /* reads it by bb_slot_get(this) — the test_sno_1.c named-slot model (`str_t POS0; ... = POS0`). The  */
    /* DESCR layout (descr.h): eightbyte0 = {v:DT_I@+0, slen:0@+4}, eightbyte1 = the int value@+8. Both    */
    /* eightbytes are sealed RO in-blob and copied to the slot (RO-IP-relative reads + two RW stores, no   */
    /* value stack, no ring). Single-shot leaf: α stores+jmp γ; β jmp ω.                                  */
    if (g_icn_flat_chain && pBB && pBB->t == IR_LIT_I) {
        uint64_t eb0 = (uint64_t)DT_I;          /* v=DT_I (low 4 bytes), slen=0 (high 4 bytes) */
        uint64_t eb1 = (uint64_t)pBB->ival;     /* the int value */
        if (MEDIUM_BINARY) {
            int off = bb_slot_alloc16(pBB);
            /*   off  bytes                          asm                                                    */
            /*   0    48 8B 05 <u32 d0=33>           mov rax,[rip+d0]    (rip-base=7; eb0@40; d0=40-7=33)    */
            /*   7    49 89 84 24 <u32 off>          mov [r12+off],rax   (DESCR eightbyte0 → slot)           */
            /*   15   48 8B 05 <u32 d1=34>           mov rax,[rip+d1]    (rip-base=22; eb1@48; d1=48-22=26)  */
            /*   22   49 89 84 24 <u32 off+8>        mov [r12+off+8],rax (DESCR eightbyte1 → slot)           */
            /*   30   E9 <rel32 → γ>                 jmp γ               ← γ patch at 31                     */
            /*   35   E9 <rel32 → ω>                 β: jmp ω            ← β-def 35, ω patch 36              */
            /*   40   <u64 eb0>                      sealed RO DESCR.lo  (reached only by [rip+33])          */
            /*   48   <u64 eb1>                      sealed RO DESCR.hi  (reached only by [rip+26])          */
            /*   56   end                                                                                   */
            bin = { {31, 35, 36}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(3, "\x48\x8B\x05") + u32le(33u)
                 + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
                 + bytes(3, "\x48\x8B\x05") + u32le(26u)
                 + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)(off + 8))
                 + bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0)
                 + u64le(eb0)
                 + u64le(eb1);
        }
        if (MEDIUM_TEXT) {
            int off = bb_slot_alloc16(pBB);
            int nid = bb_node_id(pBB);
            std::string l0 = emit_fmt(".Llit%d_eb0", nid);
            std::string l1 = emit_fmt(".Llit%d_eb1", nid);
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX IR_LIT_I [GZ-7 flat-chain → ζ slot, 16-byte DESCR]")
                 + s_directive(".section .rodata")
                 + s_directive(l0 + emit_fmt(": .quad %llu", (unsigned long long)eb0))
                 + s_directive(l1 + emit_fmt(": .quad %llu", (unsigned long long)eb1))
                 + s_directive(".section .text")
                 + s_2asm("mov", emit_fmt("rax, [rip+%s]", l0.c_str()))
                 + s_2asm("mov", emit_fmt("[r12+%d], rax", off))
                 + s_2asm("mov", emit_fmt("rax, [rip+%s]", l1.c_str()))
                 + s_2asm("mov", emit_fmt("[r12+%d], rax", off + 8))
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp", _.lbl_ω);
        }
    }
    if (g_icn_flat_chain && pBB && pBB->t == IR_LIT_S) {
        /* RK-EMIT-2 (2026-05-31): a string literal is a producer box too — store its 16-byte DESCR        */
        /* {v=DT_S, slen=0, s=ptr} into its OWN ζ frame slot so a downstream consumer (ASSIGN `$s = "x"`,  */
        /* the dval==2.0 call marshaller, write) reads it via bb_slot_get(this). Mirrors the IR_LIT_I arm. */
        /* slen=0 follows the STRVAL convention (length computed by the runtime via strlen on use). The     */
        /* string bytes are sealed RO (.rodata, IP-relative lea) in TEXT; the JIT uses the host pointer.    */
        const char * sv = pBB->sval ? pBB->sval : "";
        if (MEDIUM_BINARY) {
            int off = bb_slot_alloc16(pBB);
            uint64_t sptr = (uint64_t)(uintptr_t) sv;
            /*   0    49 C7 84 24 <u32 off> <u32 1>   mov qword [r12+off], 1   (DESCR eb0: v=DT_S, slen=0)  */
            /*   12   48 B8 <u64 sptr>                movabs rax, &str                                      */
            /*   22   49 89 84 24 <u32 off+8>         mov [r12+off+8], rax     (DESCR eb1: ptr)             */
            /*   30   E9 <rel32 → γ>                  jmp γ                    ← γ patch at 31              */
            /*   35   E9 <rel32 → ω>                  β: jmp ω                 ← β-def 35, ω patch 36       */
            bin = { {31, 35, 36}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(4, "\x49\xC7\x84\x24") + u32le((uint32_t)off) + u32le(1u)
                 + bytes(2, "\x48\xB8") + u64le(sptr)
                 + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)(off + 8))
                 + bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0);
        }
        if (MEDIUM_TEXT) {
            int off = bb_slot_alloc16(pBB);
            std::string sl = emit_fmt(".Llit%d_str", bb_node_id(pBB));
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX IR_LIT_S [RK-EMIT-2 flat-chain → ζ slot, 16-byte DESCR]")
                 + s_directive(".section .rodata")
                 + s_directive(sl + ": .string \"" + sv + "\"")
                 + s_directive(".section .text") + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov", emit_fmt("qword ptr [r12+%d], 1", off))
                 + s_2asm("lea", emit_fmt("rax, [rip+%s]", sl.c_str()))
                 + s_2asm("mov", emit_fmt("[r12+%d], rax", off + 8))
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp", _.lbl_ω);
        }
    }
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

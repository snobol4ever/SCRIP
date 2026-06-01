/* bb_binop_arith.cpp — BB box: IR_BINOP integer arithmetic, stackless slot→slot DESCR.
   GZ-9 (Icon). Split out of bb_binop.cpp (one-box-one-file). */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/interp/gen.h"
int  bb_slot_get(IR_t * nd);
int  bb_slot_alloc16(IR_t * nd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arith_is_numarith(int64_t op) { return op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_arith_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    int64_t op = pBB ? pBB->ival : -1;
    /* GZ-9 (GROUND ZERO 3, 2026-05-31): stackless integer ARITHMETIC in the flat-chain. The operands are */
    /* producer boxes (VAR / LIT_I / nested binop) that already wrote a 16-byte DESCR into their own ζ=r12 */
    /* frame slots; this box reads each operand's int payload at [r12+slot+8] (bb_slot_get), computes, and */
    /* writes a DESCR result {v:DT_I, payload:result} into its OWN slot [r12+off] (bb_slot_alloc16) so a    */
    /* consumer (IR_ASSIGN / write / nested binop) reads it by bb_slot_get(this). This is the test_icon.c   */
    /* `mult_V = a * b` named-slot model — NO value stack, NO ring. The `i := i + 1` of a while/until loop  */
    /* lowers to exactly this (VAR i + LIT_I 1). Both operands loaded into rax/rcx, operated register-to-   */
    /* register (uniform across ADD/SUB/MUL/DIV/MOD), result re-tagged DT_I. Single-shot (operands const at */
    /* this evaluation) → α computes+stores+jmp γ ; β jmp ω. Grounded in oarith.r integer arithmetic.       */
    if (g_icn_flat_chain && pBB && arith_is_numarith(op) && pBB->α && pBB->β) {
        int sa = bb_slot_get(pBB->α);
        int sb = bb_slot_get(pBB->β);
        if (sa >= 0 && sb >= 0) {
            int off = bb_slot_alloc16(pBB);
            if (MEDIUM_BINARY) {
                /* op block (register-to-register, rax=lhs, rcx=rhs, result in rax):                         */
                std::string opb;
                switch (op) {
                case BINOP_ADD: opb = bytes(3, "\x48\x01\xC8"); break;                       /* add rax,rcx  */
                case BINOP_SUB: opb = bytes(3, "\x48\x29\xC8"); break;                       /* sub rax,rcx  */
                case BINOP_MUL: opb = bytes(4, "\x48\x0F\xAF\xC1"); break;                   /* imul rax,rcx */
                case BINOP_DIV: opb = bytes(2, "\x48\x99") + bytes(3, "\x48\xF7\xF9"); break;/* cqo; idiv rcx */
                case BINOP_MOD: opb = bytes(2, "\x48\x99") + bytes(3, "\x48\xF7\xF9") + bytes(3, "\x48\x89\xD0"); break; /* cqo; idiv rcx; mov rax,rdx */
                default: opb = bytes(3, "\x48\x01\xC8"); break;
                }
                /*   0    49 8B 84 24 <u32 sa+8>      mov rax,[r12+sa+8]   (lhs int payload)                  */
                /*   8    49 8B 8C 24 <u32 sb+8>      mov rcx,[r12+sb+8]   (rhs int payload)                  */
                /*   16   <op block, len L>           result in rax                                            */
                /*   16+L 49 C7 84 24 <u32 off> <i32 6>  mov qword [r12+off],DT_I (DESCR lo: v=DT_I,slen=0) — 12 bytes (4 op/ModRM/SIB + 4 disp32 + 4 imm32) */
                /*   ...  49 89 84 24 <u32 off+8>     mov [r12+off+8],rax  (DESCR hi: payload)               — 8 bytes  */
                /*   ...  E9 <rel32 → γ>              jmp γ                                                  — 5 bytes  */
                /*   ...  E9 <rel32 → ω>              β: jmp ω                                               — 5 bytes  */
                int L      = (int)opb.size();
                int p_tag  = 16 + L;            /* offset of the `mov qword [r12+off],DT_I` (12 bytes) */
                int p_hi   = p_tag + 12;        /* offset of the `mov [r12+off+8],rax`       ( 8 bytes) */
                int p_jmpg = p_hi + 8;          /* offset of `jmp γ` opcode (E9) ; γ patch at +1        */
                int p_jmpb = p_jmpg + 5;        /* offset of `β: jmp ω` (E9) ; β-def here, ω patch +1   */
                bin = { {p_jmpg + 1, p_jmpb, p_jmpb + 1}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)(sa + 8))
                     + bytes(4, "\x49\x8B\x8C\x24") + u32le((uint32_t)(sb + 8))
                     + opb
                     + bytes(4, "\x49\xC7\x84\x24") + u32le((uint32_t)off) + u32le((uint32_t)DT_I)
                     + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)(off + 8))
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0);
            }
            if (MEDIUM_TEXT) {
                const char *opasm = NULL; const char *opextra = NULL;
                switch (op) {
                case BINOP_ADD: opasm = "add rax, rcx"; break;
                case BINOP_SUB: opasm = "sub rax, rcx"; break;
                case BINOP_MUL: opasm = "imul rax, rcx"; break;
                case BINOP_DIV: opasm = "cqo"; opextra = "idiv rcx"; break;
                case BINOP_MOD: opasm = "cqo"; opextra = "idiv rcx"; break;
                default: opasm = "add rax, rcx"; break;
                }
                std::string s =
                       s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment(emit_fmt("# BOX IR_BINOP arith op=%lld [GZ-9 stackless slot→slot DESCR]", (long long)op))
                     + s_2asm("mov", emit_fmt("rax, [r12+%d]", sa + 8))
                     + s_2asm("mov", emit_fmt("rcx, [r12+%d]", sb + 8))
                     + s_1asm(opasm);
                if (opextra) s += s_1asm(opextra);
                if (op == BINOP_MOD) s += s_2asm("mov", "rax, rdx");
                s +=   s_2asm("mov", emit_fmt("qword ptr [r12+%d], %d", off, (int)DT_I))
                     + s_2asm("mov", emit_fmt("[r12+%d], rax", off + 8))
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", _.lbl_ω);
                return s;
            }
        }
    }
    return std::string();
}

/* bb_binop_lit_arith.cpp — BB box: IR_BINOP, both operands IR_LIT_I, op ADD/SUB.
   GZ-3 (GROUND ZERO 3, 2026-05-31). Split out of bb_binop.cpp (one-box-one-file). */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/interp/gen.h"
int  bb_slot_alloc(IR_t * nd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_lit_arith_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    int64_t op = pBB ? pBB->ival : -1;
    /* GZ-3 (GROUND ZERO 3, 2026-05-31): stackless integer binop over two READ-ONLY constant operands.   */
    /* Both operands are IR_LIT_I — compile-time constants. Per the RO-IP-relative + ONE-REGISTER FRAME   */
    /* FACT RULES: the two int64 values are sealed as RO data INSIDE this box's own blob (after the jmps, */
    /* never executed) and read `[rip+disp]` (disp = emit-time constant, data+access share the blob — NO  */
    /* patch, NO absolute address, NO value stack). The result is the box's first READ-WRITE state, so it */
    /* is stored into a per-sequence frame slot `[r12+off]` (ζ=r12), off claimed via bb_slot_alloc; the    */
    /* consumer (write) recovers off via bb_slot_get(pBB). Deterministic single-shot (both operands const) */
    /* → α computes+stores+jmp γ ; β jmp ω (no resume). Grounded in test_icon.c mult (mult_V=a*b). Only    */
    /* ADD/SUB share the `48 0x 05` second-operand encoding; other ops fall through to the rt_arith arm.   */
    if (MEDIUM_BINARY && pBB && pBB->α && pBB->β && pBB->α->t == IR_LIT_I && pBB->β->t == IR_LIT_I
        && (op == BINOP_ADD || op == BINOP_SUB)) {
        int     off = bb_slot_alloc(pBB);
        int64_t v1  = pBB->α->ival;
        int64_t v2  = pBB->β->ival;
        const char *op2 = (op == BINOP_ADD) ? "\x48\x03\x05" : "\x48\x2B\x05";
        /*   off  bytes                       asm                                                          */
        /*   0    48 8B 05 <u32 d1=25>        mov rax,[rip+d1]      (rip-base=7; v1@32; d1=32-7=25)        */
        /*   7    48 0X 05 <u32 d2=26>        add|sub rax,[rip+d2]  (rip-base=14; v2@40; d2=40-14=26)      */
        /*   14   49 89 84 24 <u32 off>       mov [r12+off],rax     (store result into the ζ frame slot)   */
        /*   22   E9 <rel32 → γ>              jmp γ                 ← γ patch at 23                         */
        /*   27   (β defined) E9 <rel32 → ω>  β: jmp ω              ← β-def 27, ω patch 28                  */
        /*   32   <u64 v1>                    sealed RO operand 1   (reached only by [rip+25])              */
        /*   40   <u64 v2>                    sealed RO operand 2   (reached only by [rip+26])              */
        /*   48   end                                                                                       */
        bin = { {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(3, "\x48\x8B\x05") + u32le(25u)
             + bytes(3, op2)            + u32le(26u)
             + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
             + bytes(1, "\xE9")         + u32le(0)
             + bytes(1, "\xE9")         + u32le(0)
             + u64le((uint64_t)v1)
             + u64le((uint64_t)v2);
    }
    if (MEDIUM_TEXT && pBB && pBB->α && pBB->β && pBB->α->t == IR_LIT_I && pBB->β->t == IR_LIT_I
        && (op == BINOP_ADD || op == BINOP_SUB)) {
        /* GZ-3 (GROUND ZERO 3), mode-4 TEXT twin of the BINARY int ADD/SUB arm below: both operands are */
        /* READ-ONLY int constants → emit them to .rodata, read IP-relative, compute, and store the      */
        /* result into the per-sequence ζ frame slot [r12+off] (off claimed via bb_slot_alloc, so the    */
        /* consumer's bb_slot_get hits). No value stack. Single-shot (β→ω).                               */
        int     off = bb_slot_alloc(pBB);
        int     nid = bb_node_id(pBB);
        std::string la = emit_fmt(".Lbinop%d_a", nid);
        std::string lb = emit_fmt(".Lbinop%d_b", nid);
        const char *mn = (op == BINOP_ADD) ? "add rax," : "sub rax,";
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX IR_BINOP %s [GZ-3 stackless RO-int → ζ slot]", op == BINOP_ADD ? "ADD" : "SUB"))
             + s_directive(".section .rodata")
             + s_directive(la + emit_fmt(": .quad %lld", (long long)pBB->α->ival))
             + s_directive(lb + emit_fmt(": .quad %lld", (long long)pBB->β->ival))
             + s_directive(".section .text")
             + s_directive(".intel_syntax noprefix")
             + s_2asm("mov rax,", "[rip + " + la + "]")
             + s_2asm(mn,         "[rip + " + lb + "]")
             + s_2asm("mov",      emit_fmt("[r12 + %d], rax", off))
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    return std::string();
}

/* bb_unop.cpp — grouped BB template for Icon value-producing unary operators.
   Handles IR_UNOP (op in pBB->ival: TT_MNS -E / TT_PLS +E / TT_SIZE *E / TT_NONNULL \E) and the
   legacy split kinds IR_NEG/IR_POS/IR_NONNULL/IR_NULL_TEST/IR_NOT.

   GZ-11+ STACKLESS ARM (this session, the PRIMARY path under g_icn_flat_chain for TT_MNS/TT_PLS) —
   grounded in jcon ir_a_Unop (`-`/`+` are single-result, resume fails) + Icon oarith.r operator{1}
   neg(x)/number(x). The operand producer box already wrote a 16-byte DESCR into its own ζ=r12 slot;
   this box reads the int payload [r12+slot(α)+8], negates (TT_MNS) or passes through (TT_PLS), and
   writes a {DT_I, result} DESCR into its OWN slot [r12+off] for the consumer to read by bb_slot_get.
   NO value stack, NO ring — the test_icon.c named-slot model (mirrors the bb_binop.cpp GZ-9 arith arm).

   LEGACY VSTACK ARM (IBB-9-UNOP, Opus 4.8, 2026-05-29 — still used off-chain + for the not-yet-rebuilt
   ops SIZE/NONNULL/NOT). The flat driver (flat_drive_unop) walks the operand first so its value sits on
   the vstack; each op routes to its rt_unop_* helper which pops the operand, applies the bb_exec.c
   semantics, leaves a balanced result on the vstack, and sets LAST_OK. Those rt_unop_* helpers ABORT
   post-GROUND-ZERO-3 (the vstack is gone), so this arm awaits its own stackless GZ-11+ rung.

   Legacy bytes (BINARY, 22 total):
       0  : 48 B8 + u64le fn_ptr        movabs rax, &rt_unop_<op>
       10 : FF D0                       call rax
       12 : E9 + u32le γ_rel32          jmp γ          (γ patch at 13)
       17 : E9 + u32le ω_rel32          β: jmp ω       (β-def at 17, ω patch at 18)
       22 : end
*/
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "ast.h"
#include "descr.h"
int rt_unop_neg(void);
int rt_unop_pos(void);
int rt_unop_nonnull(void);
int rt_unop_null_test(void);
int rt_unop_not(void);
int rt_unop_size(void);
int  bb_slot_get(IR_t * nd);
int  bb_slot_alloc16(IR_t * nd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *bb_unop_helper_name(IR_t * pBB) {
    if (!pBB) return "rt_unop_not";
    switch (pBB->t) {
    case IR_NEG:        return "rt_unop_neg";
    case IR_POS:        return "rt_unop_pos";
    case IR_NONNULL:    return "rt_unop_nonnull";
    case IR_NULL_TEST:  return "rt_unop_null_test";
    case IR_NOT:        return "rt_unop_not";
    case IR_SIZE:       return "rt_unop_size";
    default:            return "rt_unop_not";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t bb_unop_helper_ptr(IR_t * pBB) {
    int (*fp)(void) = rt_unop_not;
    if (pBB) switch (pBB->t) {
        case IR_NEG:        fp = rt_unop_neg;        break;
        case IR_POS:        fp = rt_unop_pos;        break;
        case IR_NONNULL:    fp = rt_unop_nonnull;    break;
        case IR_NULL_TEST:  fp = rt_unop_null_test;  break;
        case IR_NOT:        fp = rt_unop_not;        break;
        case IR_SIZE:       fp = rt_unop_size;       break;
        default:            fp = rt_unop_not;        break;
    }
    return (uint64_t)(uintptr_t)(void*)fp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_unop_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_UNOP");
    /* GZ-11+ (unary integer arithmetic, this session): stackless `-x` / `+x` over a single producer-box   */
    /* operand. Grounded in jcon ir_a_Unop (`-` is in `funcs` → single-result, resume fails) + Icon        */
    /* oarith.r operator{1} - neg(x) (negate; +x = number coercion, an int stays itself). The operand box   */
    /* (IR_VAR / IR_LIT_I / nested binop) already wrote a 16-byte DESCR into its OWN ζ=r12 slot; this box    */
    /* reads the int payload at [r12+sa+8] (bb_slot_get(pBB->α)), negates (TT_MNS) or passes through        */
    /* (TT_PLS), and writes a {v:DT_I, payload:result} DESCR into its OWN slot [r12+off] (bb_slot_alloc16)   */
    /* so the consumer (write / nested binop / assign) reads it by bb_slot_get(this). The test_icon.c       */
    /* named-slot model — NO value stack, NO ring, the `rt_unop_*` vstack helpers (which abort post-GROUND-  */
    /* ZERO-3) are NOT called. Single-shot (operand const at this evaluation): α computes+stores+jmp γ ;     */
    /* β jmp ω. Non-int-arith ops (SIZE/NONNULL/NOT/…) and the off-chain path fall through to the legacy     */
    /* arm below (still vstack — their own GZ-11+ rung). */
    if (g_icn_flat_chain && pBB && pBB->α && (pBB->ival == (int64_t)TT_MNS || pBB->ival == (int64_t)TT_PLS)) {
        int sa = bb_slot_get(pBB->α);
        if (sa >= 0) {
            int off = bb_slot_alloc16(pBB);
            int is_neg = (pBB->ival == (int64_t)TT_MNS);
            if (MEDIUM_BINARY) {
                /*   0    49 8B 84 24 <u32 sa+8>      mov rax,[r12+sa+8]   (operand int payload)  — 8 bytes  */
                /*   8    (TT_MNS only) 48 F7 D8      neg rax                                     — L bytes  */
                /*  8+L   49 C7 84 24 <u32 off> <i32 DT_I>  mov qword [r12+off], DT_I  (DESCR lo) — 12 bytes  */
                /*  ...   49 89 84 24 <u32 off+8>     mov [r12+off+8], rax (DESCR hi: payload)    —  8 bytes */
                /*  ...   E9 <rel32 → γ>              jmp γ           (opcode @ 28+L; γ patch @ 29+L)        */
                /*  ...   E9 <rel32 → ω>              β: jmp ω        (β-def @ 33+L; ω patch @ 34+L)         */
                std::string opb = is_neg ? bytes(3, "\x48\xF7\xD8") : std::string();
                int L      = (int)opb.size();
                int p_jmpg = 8 + L + 12 + 8;
                int p_jmpb = p_jmpg + 5;
                bin = { {p_jmpg + 1, p_jmpb, p_jmpb + 1}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)(sa + 8))
                     + opb
                     + bytes(4, "\x49\xC7\x84\x24") + u32le((uint32_t)off) + u32le((uint32_t)DT_I)
                     + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)(off + 8))
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0);
            }
            if (MEDIUM_TEXT) {
                std::string s =
                       s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment(emit_fmt("# BOX IR_UNOP %s [GZ-11+ stackless slot→slot DESCR]", is_neg ? "NEG" : "POS"))
                     + s_2asm("mov", emit_fmt("rax, [r12+%d]", sa + 8));
                if (is_neg) s += s_1asm("neg rax");
                s +=   s_2asm("mov", emit_fmt("qword ptr [r12+%d], %d", off, (int)DT_I))
                     + s_2asm("mov", emit_fmt("[r12+%d], rax", off + 8))
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", _.lbl_ω);
                return s;
            }
        }
    }
    const char *hn = bb_unop_helper_name(pBB);
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX IR_UNOP (%s: pop operand, apply, set LAST_OK, jmp γ)", hn))
             + s_2asm("call", emit_fmt("%s@PLT", hn))
             + s_2asm("jmp",  _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",  _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t fptr = bb_unop_helper_ptr(pBB);
        bin = { {13, 17, 18},
                {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p},
                {false, true, false} };
        return bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_unop(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_unop_str(pBB, bin), bin); }

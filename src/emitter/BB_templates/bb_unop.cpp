/* bb_unop.cpp — grouped BB template for Icon value-producing unary operators.
   IBB-9-UNOP (Opus 4.8, 2026-05-29). Handles BB_NEG (-E), BB_POS (+E), BB_NONNULL (\E),
   BB_NULL_TEST (/E), BB_NOT (not E). The flat driver (flat_drive_unop in emit_bb.c) walks
   the operand subgraph (pBB->α) first so the operand's value sits on the vstack; this template
   is the apply node. Each op routes to its rt_unop_* helper which pops the operand, applies the
   Icon semantics from bb_exec.c, leaves a balanced result (value or FAILDESCR) on the vstack, and
   sets the LAST_OK flag (1 = succeeded, 0 = failed).

   Control shape MIRRORS the bb_binop.cpp relop arm exactly: the helper sets LAST_OK + pushes a
   result, and the template then jmps γ UNCONDITIONALLY.  Both ports of a condition-context unop
   (cond of an if/while) point at the BB_IF router (lower_new_If_ag wires cond.γ==cond.ω==BB_IF),
   so the single jmp γ lands at the router regardless of success/failure, and bb_if.cpp reads LAST_OK
   to choose then/else.  In value context (write(\E), x := \E) the consumer takes the pushed value;
   a failed unop leaves FAILDESCR, which the write/assign trailer handles like any failed expr.  The
   return value in eax is ignored (the LAST_OK flag is the channel), matching rt_acomp/rt_lcomp usage.

   Bytes (BINARY, 17 total):
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
int rt_unop_neg(void);
int rt_unop_pos(void);
int rt_unop_nonnull(void);
int rt_unop_null_test(void);
int rt_unop_not(void);
int rt_unop_size(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *bb_unop_helper_name(IR_t * pBB) {
    if (!pBB) return "rt_unop_not";
    switch (pBB->t) {
    case BB_NEG:        return "rt_unop_neg";
    case BB_POS:        return "rt_unop_pos";
    case BB_NONNULL:    return "rt_unop_nonnull";
    case BB_NULL_TEST:  return "rt_unop_null_test";
    case BB_NOT:        return "rt_unop_not";
    case BB_SIZE:       return "rt_unop_size";
    default:            return "rt_unop_not";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t bb_unop_helper_ptr(IR_t * pBB) {
    int (*fp)(void) = rt_unop_not;
    if (pBB) switch (pBB->t) {
        case BB_NEG:        fp = rt_unop_neg;        break;
        case BB_POS:        fp = rt_unop_pos;        break;
        case BB_NONNULL:    fp = rt_unop_nonnull;    break;
        case BB_NULL_TEST:  fp = rt_unop_null_test;  break;
        case BB_NOT:        fp = rt_unop_not;        break;
        case BB_SIZE:       fp = rt_unop_size;       break;
        default:            fp = rt_unop_not;        break;
    }
    return (uint64_t)(uintptr_t)(void*)fp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_unop_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — BB_UNOP");
    const char *hn = bb_unop_helper_name(pBB);
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX BB_UNOP (%s: pop operand, apply, set LAST_OK, jmp γ)", hn))
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

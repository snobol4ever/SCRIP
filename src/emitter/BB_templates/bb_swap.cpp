/* bb_swap.cpp — BB template for BB_SWAP (Icon `x :=: y` swap).
   IBB (2026-05-29, Opus 4.8). Composite: α = lhs (BB_VAR), β = rhs (BB_VAR).
   The flat-driver (flat_drive_swap in emit_bb.c) walks α then β FIRST, pushing both current
   values onto the vstack (top = y's value).  This template emits the swap trailer:
       rt_pop_nv_set(xname)   — pops y's value, stores into x
       rt_pop_nv_set(yname)   — pops x's value, stores into y
       jmp γ ; β: jmp ω
   Net effect: x := old y, y := old x.  Mirrors bb_assign.cpp's rt_pop_nv_set trailer, doubled.

   Bytes (54 total):
       0  : 48 BF + u64le xname_ptr     movabs rdi, xname
       10 : 48 B8 + u64le fn_ptr        movabs rax, &rt_pop_nv_set
       20 : FF D0                       call rax            (pop y_val → x)
       22 : 48 BF + u64le yname_ptr     movabs rdi, yname
       32 : 48 B8 + u64le fn_ptr        movabs rax, &rt_pop_nv_set
       42 : FF D0                       call rax            (pop x_val → y)
       44 : E9 + u32le γ_rel32          jmp γ               (patch at 45 → lbl_γ)
       49 : E9 + u32le ω_rel32          β: jmp ω            (β-def at 49, ω patch at 50)
       54 : end
*/
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
void rt_pop_nv_set(const char *name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_swap_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — BB_SWAP");
    IR_t *lhs = pBB ? pBB->α : NULL;
    IR_t *rhs = pBB ? pBB->β : NULL;
    if (!lhs || lhs->t != BB_VAR || !lhs->sval || !rhs || rhs->t != BB_VAR || !rhs->sval) {
        fprintf(stderr, "[IBB] FATAL bb_swap: x:=:y requires two BB_VAR operands with sval "
                "(lhs kind=%d rhs kind=%d)\n", lhs ? (int)lhs->t : -1, rhs ? (int)rhs->t : -1);
        abort();
    }
    const char *xname = lhs->sval;
    const char *yname = rhs->sval;
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX BB_SWAP %s :=: %s (pop y->x, pop x->y)", xname, yname))
             + s_2asm("call",     "rt_pop_nv_set@PLT")
             + s_2asm("call",     "rt_pop_nv_set@PLT")
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t xptr = (uint64_t)(uintptr_t)xname;
        uint64_t yptr = (uint64_t)(uintptr_t)yname;
        uint64_t fptr; { void (*fp)(const char *) = rt_pop_nv_set; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {45, 49, 50}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xBF") + u64le(xptr)
             + bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(2, "\x48\xBF") + u64le(yptr)
             + bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_swap(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_swap_str(pBB, bin), bin); }

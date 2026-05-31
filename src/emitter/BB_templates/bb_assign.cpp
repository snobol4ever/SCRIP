/* bb_assign.cpp — BB template for BB_ASSIGN (Icon variable assign).
   IBB-7 (2026-05-29). Composite: α = lhs (BB_VAR — sval is the slot name), β = rhs subgraph.
   The flat-driver walks β first (pushing the value onto vstack), then defines lbl_α at this
   template's location; the template emits the trailer `rt_pop_nv_set(name); jmp γ; β: jmp ω`.

   Mirrors bb_call.cpp write(int_expr) trailer (22-byte movabs/call/jmp/β-jmp), but loads the
   name pointer into rdi (SysV arg0) before the call.

   Bytes (32 total):
       0  : 48 BF + u64le name_ptr      movabs rdi, name_ptr
       10 : 48 B8 + u64le fn_ptr        movabs rax, &rt_pop_nv_set
       20 : FF D0                       call rax
       22 : E9 + u32le γ_rel32          jmp γ              (patch at 23 → lbl_γ)
       27 : E9 + u32le ω_rel32          β: jmp ω           (β-def at 27, ω patch at 28)
       32 : end

   The driver (flat_drive_assign in emit_bb.c) is responsible for walking β before the BB_ASSIGN
   template label is defined. The template assumes the value is on the vstack at entry.
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
void rt_pop_nv_set(const char *name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_assign_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — BB_ASSIGN");
    BB_t *lhs = pBB ? pBB->α : NULL;
    if (!lhs || lhs->t != BB_VAR || !lhs->sval) {
        fprintf(stderr, "[IBB] FATAL bb_assign: lhs (pBB->α) must be BB_VAR with sval (got kind=%d sval=%s)\n",
                lhs ? (int)lhs->t : -1, (lhs && lhs->sval) ? lhs->sval : "(null)");
        abort();
    }
    const char *name = lhs->sval;
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX BB_ASSIGN store(\"%s\") [IBB-7 rt_pop_nv_set]", name))
             + s_2asm("call",     "rt_pop_nv_set@PLT")
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t nptr = (uint64_t)(uintptr_t)name;
        uint64_t fptr; { void (*fp)(const char *) = rt_pop_nv_set; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xBF") + u64le(nptr)
             + bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_assign(BB_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_assign_str(pBB, bin), bin); }

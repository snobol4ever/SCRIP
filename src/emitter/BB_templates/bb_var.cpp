/* bb_var.cpp — BB template for BB_VAR (Icon variable read).
   IBB-7 (2026-05-29). Leaf node: pBB->sval = variable name. On α-entry, push the
   variable's value onto the runtime vstack via rt_nv_get(name); on success jmp γ;
   on β re-entry jmp ω (BB_VAR is a non-resumable leaf — single value, no retry).

   Mirrors bb_lit_scalar.cpp BB_LIT_I arm (32-byte push-via-runtime-call), with
   movabs of the name pointer (stable in AST pool) instead of an int64 literal,
   and rt_nv_get instead of rt_push_int.

   Bytes (32 total):
       0  : 48 BF + u64le name_ptr      movabs rdi, name_ptr
       10 : 48 B8 + u64le fn_ptr        movabs rax, &rt_nv_get
       20 : FF D0                       call rax
       22 : E9 + u32le γ_rel32          jmp γ              (patch at 23 → lbl_γ)
       27 : E9 + u32le ω_rel32          β: jmp ω           (β-def at 27, ω patch at 28)
       32 : end
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
void rt_nv_get(const char *name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_var_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — BB_VAR");
    const char *name = (pBB && pBB->sval) ? pBB->sval : "";
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX BB_VAR read(\"%s\") [IBB-7 rt_nv_get push]", name))
             + s_2asm("call",     "rt_nv_get@PLT")
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t nptr = (uint64_t)(uintptr_t)name;
        uint64_t fptr; { void (*fp)(const char *) = rt_nv_get; fptr = (uint64_t)(uintptr_t)(void*)fp; }
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
extern "C" void bb_var(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_var_str(pBB, bin), bin); }

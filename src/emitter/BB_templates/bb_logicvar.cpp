/* bb_logicvar.cpp — BB template for BB_LOGICVAR: Prolog variable slot read.
   PL-T-2 (GOAL-BB-TEMPLATE-LADDER.md). x86 only per Invariant #14 (2026-05-22). */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
extern "C" void rt_pl_var_push(int slot);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_logicvar_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        bin = { {18, 22, 23}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_γ_p}, {false, true, false} };
        return IF(MEDIUM_MACRO_DEF,
               s_comment("# no macro form — RESOLVE_VAR"))
             + IF(MEDIUM_BINARY,
               bytes(1, "\xBF") + u32le((uint32_t)(int)pBB->ival)
                 + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_pl_var_push) + bytes(2, "\xFF\xD0")
                 + bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_1asm(emit_fmt("# BOX RESOLVE_VAR(slot=%d)", (int)pBB->ival))
                 + s_2asm("mov edi,", emit_fmt("%d", (int)pBB->ival))
                 + s_2asm("call", "rt_pl_var_push@PLT")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_γ));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_logicvar(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_logicvar_str(pBB, bin), bin);
}

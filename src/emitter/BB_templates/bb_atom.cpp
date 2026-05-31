/* bb_atom.cpp — BB template for IR_ATOM: Prolog atom literal.
   PL-T-2 (GOAL-BB-TEMPLATE-LADDER.md). x86 only per Invariant #14 (2026-05-22). */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
extern "C" void rt_pl_atom_push(const char *s);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_atom_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        const char *atom = pBB->sval ? pBB->sval : "";
        bin = _.bb_ls
            ? bb_bin_t{ {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_γ_p}, {false, true, false} }
            : bb_bin_t{ {15, 19, 20}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_γ_p}, {false, true, false} };
        std::string load = _.bb_ls
            ? s_2asm("lea rcx,", emit_fmt("[rip + %s]", _.bb_ls)) + s_2asm("mov", "rdi, rcx")
            : s_2asm("xor", "edi, edi");
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — RESOLVE_ATOM"))
             + IF(MEDIUM_BINARY, _.bb_ls
                    ? bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)atom)
                     + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_pl_atom_push) + bytes(2, "\xFF\xD0")
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0)
                    : bytes(2, "\x31\xFF")
                     + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_pl_atom_push) + bytes(2, "\xFF\xD0")
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                   + s_comment(emit_fmt("# BOX RESOLVE_ATOM('%s')", atom))
                 + load
                 + s_2asm("call", "rt_pl_atom_push@PLT")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_γ));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_atom(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_atom_str(pBB, bin), bin);
}

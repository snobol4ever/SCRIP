/* bb_idx.cpp — BB templates for Icon subscript get/set (IBB-IDX, 2026-05-30).
   IR_IDX (base[idx]) and IR_IDX_SET (base[idx] := rhs).

   The flat drivers (flat_drive_idx_get / flat_drive_idx_set in emit_bb.c) walk the operand
   subgraph(s) FIRST so the runtime vstack holds the operand value(s), then define lbl_α at this
   template's location. The template is the apply node: it calls the rt helper (which pops the
   operands and pushes the result), then jmps γ. Re-entry (β) jmps ω — subscript is a single-shot
   non-resumable leaf (one value, no retry), matching canonical Icon (`[]` is in JCON ir_a_Binop's
   `funcs` set: resumption fails immediately) and bb_var / bb_assign / bb_field.

   GET driver leaves [base, idx] on the vstack (idx on top); rt_idx_get pops idx then base,
   dispatches subscript_get, pushes the element. SET driver leaves [base, idx, rhs] (rhs on top);
   rt_idx_set pops rhs, idx, base, dispatches subscript_set, pushes rhs (Icon assign value).
   Both set LAST_OK.

   Control shape MIRRORS bb_unop.cpp exactly (no-arg call). The rt helper sets LAST_OK and pushes a
   result, and the template jmps γ UNCONDITIONALLY: in value context (write(s[i]), x := s[i]) the
   consumer takes the pushed value; a failed access leaves FAILDESCR which the write/assign trailer
   handles like any failed expr. The helper's eax return is ignored (LAST_OK is the channel).

   Bytes (BINARY, 22 total), identical layout for both arms — only the called fn pointer differs:
       0  : 48 B8 + u64le fn_ptr        movabs rax, &rt_idx_<op>
       10 : FF D0                       call rax
       12 : E9 + u32le γ_rel32          jmp γ              (γ patch at 13 → lbl_γ)
       17 : E9 + u32le ω_rel32          β: jmp ω           (β-def at 17, ω patch at 18)
       22 : end
*/
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
int rt_idx_get(void);
int rt_idx_set(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_idx_emit(IR_t * pBB, bb_bin_t & bin, int is_set) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_IDX");
    const char *hn = is_set ? "rt_idx_set" : "rt_idx_get";
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX IR_IDX%s (%s: pop operands, apply, set LAST_OK, jmp γ) [IBB-IDX]", is_set ? "_SET" : "", hn))
             + s_2asm("call",     emit_fmt("%s@PLT", hn))
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t fptr;
        if (is_set) { int (*fp)(void) = rt_idx_set; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        else        { int (*fp)(void) = rt_idx_get; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {13, 17, 18}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_idx(IR_t * pBB)     { bb_bin_t bin; bb_emit_asm_result(bb_idx_emit(pBB, bin, 0), bin); }
extern "C" void bb_idx_set(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_idx_emit(pBB, bin, 1), bin); }

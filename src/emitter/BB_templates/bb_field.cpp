/* bb_field.cpp — BB templates for Icon record field access (IBB-11, 2026-05-29).
   BB_FIELD_GET (obj.field) and BB_FIELD_SET (obj.field := rhs). pBB->sval is the field name.

   The flat drivers (flat_drive_field_get / flat_drive_field_set in emit_bb.c) walk the operand
   subgraph(s) FIRST so the runtime vstack holds the operand value(s), then define lbl_α at this
   template's location. The template is the apply node: it loads the field-name pointer into rdi
   (SysV arg0), calls the rt helper, then jmps γ. Re-entry (β) jmps ω — field access is a
   non-resumable leaf (single value, no retry), like BB_VAR / BB_ASSIGN.

   GET driver leaves [obj] on the vstack; rt_field_get pops obj, pushes the field value.
   SET driver leaves [rhs, obj] on the vstack (rhs deepest); rt_field_set pops obj then rhs,
   writes the cell, pushes rhs (Icon assignment-expression value). Both set LAST_OK.

   Control shape MIRRORS bb_var.cpp / bb_assign.cpp exactly. The rt helper sets LAST_OK and pushes a
   result, and the template jmps γ UNCONDITIONALLY: in value context (write(obj.f), x := obj.f) the
   consumer takes the pushed value; a failed access leaves FAILDESCR which the write/assign trailer
   handles like any failed expr. The helper's eax return is ignored (LAST_OK is the channel).

   Bytes (32 total), identical layout for both arms — only the called fn pointer differs:
       0  : 48 BF + u64le name_ptr      movabs rdi, name_ptr
       10 : 48 B8 + u64le fn_ptr        movabs rax, &rt_field_<op>
       20 : FF D0                       call rax
       22 : E9 + u32le γ_rel32          jmp γ              (γ patch at 23 → lbl_γ)
       27 : E9 + u32le ω_rel32          β: jmp ω           (β-def at 27, ω patch at 28)
       32 : end
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
int rt_field_get(const char *fname);
int rt_field_set(const char *fname);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_field_emit(BB_t * pBB, bb_bin_t & bin, int is_set) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — BB_FIELD");
    if (!pBB || !pBB->sval) {
        fprintf(stderr, "[IBB] FATAL bb_field: BB_FIELD_%s missing field name (sval)\n", is_set ? "SET" : "GET");
        abort();
    }
    const char *fname = pBB->sval;
    const char *hn    = is_set ? "rt_field_set" : "rt_field_get";
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX BB_FIELD_%s field=\"%s\" [IBB-11 %s]", is_set ? "SET" : "GET", fname, hn))
             + s_2asm("call",     emit_fmt("%s@PLT", hn))
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t nptr = (uint64_t)(uintptr_t)fname;
        uint64_t fptr;
        if (is_set) { int (*fp)(const char *) = rt_field_set; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        else        { int (*fp)(const char *) = rt_field_get; fptr = (uint64_t)(uintptr_t)(void*)fp; }
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
extern "C" void bb_field_get(BB_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_field_emit(pBB, bin, 0), bin); }
extern "C" void bb_field_set(BB_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_field_emit(pBB, bin, 1), bin); }

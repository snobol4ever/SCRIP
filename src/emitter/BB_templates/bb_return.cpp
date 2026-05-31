/* bb_return.cpp — BB template for IR_RETURN (Icon `return [E]`).
   IBB-9-6 (Opus 4.8, 2026-05-29, GOAL-ICON-BB). Transcribes JCON ir_a_Return (irgen.icn:867-903):
   `Succeed(t)` on the success path — the return value flows out to the caller. In the SCRIP flat
   slab model a procedure is one self-contained slab; IR_RETURN leaves the value on the value-stack
   and jumps to lbl_γ (the slab's XA_FLAT_EPILOGUE `ret`). rt_call_proc reads that value back off
   the vstack after the slab returns.

   Two shapes, distinguished by pBB->α:
     return E  — the flat driver (flat_drive_return) walks pBB->α first, so E's value is already on
                 the vstack at α-entry; the template just jmps γ (β: jmp ω).  ZERO runtime call here.
     return    — bare return (pBB->α == NULL); push &null via rt_push_null, then jmp γ.

   Bytes (with α — value already on vstack, 10 total):
       0  : E9 + u32le γ_rel32          jmp γ              (patch at 1 → lbl_γ)
       5  : E9 + u32le ω_rel32          β: jmp ω           (β-def at 5, ω patch at 6)
      10  : end
   Bytes (bare return — push null first, 22 total):
       0  : 48 B8 + u64le fn_ptr        movabs rax, &rt_push_null
      10  : FF D0                       call rax
      12  : E9 + u32le γ_rel32          jmp γ              (patch at 13 → lbl_γ)
      17  : E9 + u32le ω_rel32          β: jmp ω           (β-def at 17, ω patch at 18)
      22  : end
*/
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
void rt_push_null(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_return_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_RETURN");
    int has_expr = (pBB && pBB->α) ? 1 : 0;
    if (MEDIUM_TEXT) {
        std::string head = s_1asm(emit_fmt("%s:", _.lbl_α))
                         + s_comment(has_expr ? "# BOX IR_RETURN value-on-vstack [IBB-9-6]"
                                              : "# BOX IR_RETURN bare (push null) [IBB-9-6]");
        if (!has_expr) head = head + s_2asm("call", "rt_push_null@PLT");
        return head + s_2asm("jmp", _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp", _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        if (has_expr) {
            bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0);
        }
        uint64_t fptr; { void (*fp)(void) = rt_push_null; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {13, 17, 18}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9") + u32le(0)
             + bytes(1, "\xE9") + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_return(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_return_str(pBB, bin), bin); }

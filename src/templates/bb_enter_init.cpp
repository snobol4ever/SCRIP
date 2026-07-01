#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------
 * bb_enter_init -- JCON ir_EnterInit analog.
 *
 * Ports: α = procedure entry (every call lands here)
 *        β = no resume (initial blocks don't resume; β → ω = skip)
 *        γ = body entry (taken on FIRST call: flag was 0)
 *        ω = skip-body path (taken on SUBSEQUENT calls: flag already 1)
 *
 * Frame layout at [r12+op_off]:
 *   [r12+op_off+0 .. +7]  : DESCR_t padding (keep 16-byte alignment)
 *   [r12+op_off+8 .. +15] : int64 "done" flag (0=not yet run, 1=already ran)
 *
 * x86 logic:
 *   α:  cmp qword [r12+op_off+8], 0   ; check done-flag
 *       jne  →ω                        ; already done → skip init body
 *       mov  qword [r12+op_off+8], 1   ; mark done
 *       jmp  →γ                        ; enter init body
 *   β:  jmp  →ω                        ; no resume
 *--------------------------------------------------------------------------------------------------------------------*/
std::string bb_enter_init() {
    x86_begin();
    if (!PLATFORM_X86) return x86_bomb("bb_enter_init: no x86 platform");
    if (_.op_off < 0) return x86_bomb("bb_enter_init: op_off < 0 (no slot assigned)");
    return x86("comment", "IR_ENTER_INIT")
         + x86("mov",  "rax", FRQ(_.op_off + 8))
         + x86("cmp",  "rax", (long)0)
         + x86("jne",  "ω")
         + x86("mov",  FRQ(_.op_off + 8), (long)1)
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}

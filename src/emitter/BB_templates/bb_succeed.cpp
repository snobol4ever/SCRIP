/* bb_succeed.cpp — BB template for IR_SUCCEED (Icon/Prolog `true` / pass-through leaf).
   x86() self-encoding (template-revamp). pBB-FREE: reads ONLY g_emit (FACT RULE).
   AGW-9B-3 (2026-05-27); revamp 2026-06-02.

   The driver (emit_bb.c IR_SUCCEED case) always deposits exactly two pair entries:
     pair[0]: {def=NULL,  jmp=lbl_γ}   → jmp PORT_GAMMA
     pair[1]: {def=lbl_β, jmp=lbl_ω}  → def PORT_BETA + jmp PORT_OMEGA
   Fixed-count: no variable-length design needed. x86 arm: ONE return, pure x86() concat,
   NO bb_bin_t, medium invisible. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_succeed_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment("# BOX SUCCEED() [x86() self-encoding — jmp γ; β: jmp ω]"))
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_succeed(void) { bb_emit_x86(bb_succeed_str()); }

/* bb_eps.cpp — BB template for the degenerate EPS (empty-pattern) box. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_eps_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    (void)pBB;
    if (PLATFORM_X86) {
        bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return IF(MEDIUM_MACRO_DEF,
               s_comment("# no macro form — EPS"))
             + IF(MEDIUM_BINARY,
               bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX EPS()")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_eps(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_eps_str(pBB, bin), bin);
}

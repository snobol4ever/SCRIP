/* bb_conj.cpp — BB template for IR_GCONJ: Prolog conjunction (a, b, c).
   Driver flat_drive_pl_seq (emit_bb.c) populates xa_bb_emit_pair_* with the
   conjunction β-chain glue; this template emits those label-defs + jmps inline.
   TEXT: emit_fmt label:, s_1asm jmp.  BINARY: 'E'/'F' pair-table records. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_conj_str() {
    if (!PLATFORM_X86) return std::string();
    std::string r;
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
        if (MEDIUM_BINARY) {
            if (g_emit.xa_bb_emit_pair_define[i]) { r += (char)'E'; r += (char)(unsigned char)i; }
            if (g_emit.xa_bb_emit_pair_jmp[i])    { r += x86_Lrec(x86_b1(0xE9)); r += (char)'F'; r += (char)(unsigned char)i; }
        } else {
            if (g_emit.xa_bb_emit_pair_define[i]) r += emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[i]->name);
            if (g_emit.xa_bb_emit_pair_jmp[i])    r += s_1asm(emit_fmt("jmp %s", g_emit.xa_bb_emit_pair_jmp[i]->name));
        }
    }
    return r;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_conj(void) { bb_emit_x86(bb_conj_str()); }

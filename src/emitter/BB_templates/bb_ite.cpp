/* bb_ite.cpp — BB template for IR_ITE: Prolog (Cond -> Then ; Else).
   Driver flat_drive_pl_ite (emit_bb.c) mints Then/Else labels and deposits
   the β-tombstone glue into xa_bb_emit_pair_*; this template emits it inline.
   TEXT: x86("label")/x86("ins2","jmp").  BINARY: 'E'/'F' pair-table records. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_ite_str() {
    if (!PLATFORM_X86) return std::string();
    std::string r;
    r += IF(MEDIUM_TEXT, x86("comment", "END RESOLVE_ITE (β-tombstone via pair table)"));
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
        if (MEDIUM_BINARY) {
            if (g_emit.xa_bb_emit_pair_define[i]) { r += (char)'E'; r += (char)(unsigned char)i; }
            if (g_emit.xa_bb_emit_pair_jmp[i])    { r += x86_Lrec(x86_b1(0xE9)); r += (char)'F'; r += (char)(unsigned char)i; }
        } else {
            if (g_emit.xa_bb_emit_pair_define[i]) r += emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[i]->name);
            if (g_emit.xa_bb_emit_pair_jmp[i])    r += x86("ins1", emit_fmt("jmp %s", g_emit.xa_bb_emit_pair_jmp[i]->name));
        }
    }
    return r;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ite(void) { bb_emit_x86(bb_ite_str()); }

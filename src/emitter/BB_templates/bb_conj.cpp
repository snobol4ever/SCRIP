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
            if (g_emit.xa_bb_emit_pair_define[i]) r += std::string(g_emit.xa_bb_emit_pair_define[i]->name) + ":\n";
            if (g_emit.xa_bb_emit_pair_jmp[i])    r += x86("ins1", std::string("jmp ") + g_emit.xa_bb_emit_pair_jmp[i]->name);
        }
    }
    return r;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_conj(void) { bb_emit_x86(bb_conj_str()); }

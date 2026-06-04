#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "emit_core.h"
#include "IR.h"
#include "IR_interp_state.h"
}
#include "x86_asm.h"
extern std::string emit_build_compound_term(const IR_t *nd);
extern "C" int pl_catch_block_index(IR_t *nd);
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_catch_build_catcher(IR_t *a) {
    if (!a) return s_2asm("xor", "eax, eax");
    if (a->t == IR_STRUCT) return emit_build_compound_term(a);
    int kind = (int)a->t;
    long ival = (long)a->ival;
    const char *sval = a->sval;
    char slbl[64]; slbl[0] = 0;
    if (sval && *sval) strtab_label(slbl, sizeof slbl, sval);
    return s_2asm("mov", emit_fmt("edi, %d", kind))
         + s_2asm("mov", emit_fmt("rsi, %ld", ival))
         + (slbl[0] ? s_2asm("lea", emit_fmt("rdx, [rip + %s]", slbl))
                    : s_2asm("xor", "edx, edx"))
         + s_2asm("xorps", "xmm0, xmm0")
         + s_2asm("call", "rt_node_to_term@PLT");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_catch_str() {
    if (PLATFORM_X86) {
        if (MEDIUM_TEXT) {
            IR_t *nd = (IR_t *)g_emit.bb_zn;
            bb_catch_state_t *zc = nd ? (bb_catch_state_t *)(intptr_t)nd->ival : (bb_catch_state_t *)0;
            int idx = nd ? pl_catch_block_index(nd) : -1;
            if (nd && zc && zc->goal_g && idx >= 0) {
                return s_1asm(std::string(_.lbl_α) + ":")
                     + s_comment("# BOX RESOLVE_CATCH (PT-3: native goal/rec blocks + rt_catch_native rail)")
                     + bb_catch_build_catcher(zc->catcher)
                     + s_2asm("push", "rax")
                     + s_2asm("sub",  "rsp, 8")
                     + s_2asm("lea",  emit_fmt("rdi, [rip + .Lplcatch_%d_goal]", idx))
                     + (zc->rec_g ? s_2asm("lea", emit_fmt("rsi, [rip + .Lplcatch_%d_rec]", idx)) : s_2asm("xor", "esi, esi"))
                     + s_2asm("mov",  "rdx, [rsp + 8]")
                     + s_2asm("call", "rt_catch_native@PLT")
                     + s_2asm("add",  "rsp, 16")
                     + s_2asm("test", "eax, eax")
                     + s_2asm("je",   _.lbl_ω)
                     + s_2asm("jmp",  _.lbl_γ)
                     + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
        }
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment("# BOX RESOLVE_CATCH (mode-4 STUB — non-admitted shape; fails through)  [x86() self-encoding]"))
             + x86("jmp", PORT_OMEGA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_catch(void) { bb_emit_x86(bb_catch_str()); }

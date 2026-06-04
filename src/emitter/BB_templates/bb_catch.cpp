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
    if (!a) return x86("ins2", "xor", "eax, eax");
    if (a->t == IR_STRUCT) return emit_build_compound_term(a);
    int kind = (int)a->t;
    long ival = (long)a->ival;
    const char *sval = a->sval;
    char slbl[64]; slbl[0] = 0;
    if (sval && *sval) strtab_label(slbl, sizeof slbl, sval);
    return x86("ins2", "mov", emit_fmt("edi, %d", kind))
         + x86("ins2", "mov", emit_fmt("rsi, %ld", ival))
         + (slbl[0] ? x86("ins2", "lea", emit_fmt("rdx, [rip + %s]", slbl))
                    : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "xorps", "xmm0, xmm0")
         + x86("ins2", "call", "rt_node_to_term@PLT");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_catch_str() {
    if (PLATFORM_X86) {
        if (MEDIUM_TEXT) {
            IR_t *nd = (IR_t *)g_emit.bb_zn;
            bb_catch_state_t *zc = nd ? (bb_catch_state_t *)(intptr_t)nd->ival : (bb_catch_state_t *)0;
            int idx = nd ? pl_catch_block_index(nd) : -1;
            if (nd && zc && zc->goal_g && idx >= 0) {
                return x86("label", _.lbl_α)
                     + x86("comment", "BOX RESOLVE_CATCH (PT-3: native goal/rec blocks + rt_catch_native rail)")
                     + bb_catch_build_catcher(zc->catcher)
                     + x86("ins2", "push", "rax")
                     + x86("ins2", "sub",  "rsp, 8")
                     + x86("ins2", "lea",  emit_fmt("rdi, [rip + .Lplcatch_%d_goal]", idx))
                     + (zc->rec_g ? x86("ins2", "lea", emit_fmt("rsi, [rip + .Lplcatch_%d_rec]", idx)) : x86("ins2", "xor", "esi, esi"))
                     + x86("ins2", "mov",  "rdx, [rsp + 8]")
                     + x86("ins2", "call", "rt_catch_native@PLT")
                     + x86("ins2", "add",  "rsp, 16")
                     + x86("ins2", "test", "eax, eax")
                     + x86("ins2", "je",   _.lbl_ω)
                     + x86("ins2", "jmp",  _.lbl_γ)
                     + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
        }
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", "BOX RESOLVE_CATCH (mode-4 STUB — non-admitted shape; fails through)  [x86() self-encoding]"))
             + x86("jmp", PORT_OMEGA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_catch(void) { bb_emit_x86(bb_catch_str()); }

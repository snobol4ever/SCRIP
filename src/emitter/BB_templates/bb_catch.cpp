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
static std::string bcc_sval_lea(const char *sval) { char sl[64]; sl[0] = 0; if (sval && *sval) strtab_label(sl, sizeof sl, sval); return sl[0] ? x86("ins2", "lea", std::string("rdx, [rip + ") + sl + "]") : x86("ins2", "xor", "edx, edx"); }
static std::string bb_catch_build_catcher(IR_t *a) {
    if (!a) return x86("ins2", "xor", "eax, eax");
    if (a->op == IR_STRUCT) return emit_build_compound_term(a);
    return x86("ins2", "mov", std::string("edi, ") + std::to_string((int)a->op))
         + x86("ins2", "mov", std::string("rsi, ") + std::to_string((long)IR_LIT(a).ival))
         + bcc_sval_lea(IR_LIT(a).sval)
         + x86("ins2", "xorps", "xmm0, xmm0")
         + x86("ins2", "call", "rt_node_to_term@PLT");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static bb_catch_state_t *bcc_zc() { IR_t *nd = (IR_t *)g_emit.bb_zn; return nd ? (bb_catch_state_t *)(intptr_t)IR_LIT(nd).ival : (bb_catch_state_t *)0; }
static int bcc_idx() { IR_t *nd = (IR_t *)g_emit.bb_zn; return nd ? pl_catch_block_index(nd) : -1; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bcc_admitted(bb_catch_state_t *zc, int idx) {
    return x86("label", _.lbl_α)
         + x86("comment", "BOX RESOLVE_CATCH (PT-3: native goal/rec blocks + rt_catch_native rail)")
         + bb_catch_build_catcher(zc->catcher)
         + x86("ins2", "push", "rax")
         + x86("ins2", "sub",  "rsp, 8")
         + x86("ins2", "lea",  std::string("rdi, [rip + .Lplcatch_") + std::to_string(idx) + "_goal]")
         + (zc->rec_g ? x86("ins2", "lea", std::string("rsi, [rip + .Lplcatch_") + std::to_string(idx) + "_rec]")
                      : x86("ins2", "xor", "esi, esi"))
         + x86("ins2", "mov",  "rdx, [rsp + 8]")
         + x86("ins2", "call", "rt_catch_native@PLT")
         + x86("ins2", "add",  "rsp, 16")
         + x86("ins2", "test", "eax, eax")
         + x86("ins2", "je",   _.lbl_ω)
         + x86("ins2", "jmp",  _.lbl_γ)
         + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_catch_str() {
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_TEXT && bcc_zc() && bcc_zc()->goal_g && bcc_idx() >= 0)
        return bcc_admitted(bcc_zc(), bcc_idx());
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", "BOX RESOLVE_CATCH (mode-4 STUB — non-admitted shape; fails through)  [x86() self-encoding]"))
         + x86("jmp", "ω")
         + x86("def", "β")
         + x86("jmp", "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_catch(void) { bb_emit_x86(bb_catch_str()); }

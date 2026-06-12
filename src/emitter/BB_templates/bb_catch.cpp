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
static std::string bcc_sval_lea(const char *sval) { char sl[64]; sl[0] = 0; if (sval && *sval) strtab_label(sl, sizeof sl, sval); return sl[0] ? x86("lea", "rdx", std::string("[rip + ") + sl + "]") : x86("xor", "edx", "edx"); }
static std::string bb_catch_build_catcher(IR_t *a) {
    if (!a) return x86("xor", "eax", "eax");
    if (a->op == IR_STRUCT) return emit_build_compound_term(a);
    return x86("mov", "edi", std::to_string((int)a->op))
         + x86("mov", "rsi", std::to_string((long)IR_LIT(a).ival))
         + bcc_sval_lea(IR_LIT(a).sval)
         + x86("xorps", "xmm0", "xmm0")
         + x86("call", "rt_node_to_term@PLT");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static bb_catch_state_t *bcc_zc() { IR_t *nd = (IR_t *)g_emit.bb_zn; return nd ? (bb_catch_state_t *)(intptr_t)IR_LIT(nd).ival : (bb_catch_state_t *)0; }
static int bcc_idx() { IR_t *nd = (IR_t *)g_emit.bb_zn; return nd ? pl_catch_block_index(nd) : -1; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bcc_admitted(bb_catch_state_t *zc, int idx) {
    return x86("label", _.lbl_α)
         + x86("comment", "BOX RESOLVE_CATCH (PT-3: native goal/rec blocks + rt_catch_native rail)")
         + bb_catch_build_catcher(zc->catcher)
         + x86("push", "rax")
         + x86("sub", "rsp", "8")
         + x86("lea", "rdi", std::string("[rip + .Lplcatch_") + std::to_string(idx) + "_goal]")
         + (zc->rec_g ? x86("lea", "rsi", std::string("[rip + .Lplcatch_") + std::to_string(idx) + "_rec]")
                      : x86("xor", "esi", "esi"))
         + x86("mov", "rdx", "[rsp + 8]")
         + x86("call", "rt_catch_native@PLT")
         + x86("add", "rsp", "16")
         + x86("test", "eax", "eax")
         + x86("je",   _.lbl_ω)
         + x86("jmp",  _.lbl_γ)
         + x86("def", "β") + x86("jmp", "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_catch() {
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_TEXT && bcc_zc() && bcc_zc()->goal_g && bcc_idx() >= 0)
        return bcc_admitted(bcc_zc(), bcc_idx());
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", "BOX RESOLVE_CATCH (mode-4 STUB — non-admitted shape; fails through)  [x86() self-encoding]"))
         + x86("jmp", "ω")
         + x86("def", "β")
         + x86("jmp", "ω");
}
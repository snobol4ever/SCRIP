#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "bb_common.h"
extern "C" int   rt_unify_terms(void *l, void *r);
extern "C" int   rt_unify_const(int slot, int kind, long ival, const char *sval, double dval);
extern "C" int   rt_unify_var_var(int lslot, int rslot);
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string u_tail() {
    return x86("test", "eax", "eax")
         + x86("je",   "ω")
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int u_const_kind(int k) { return k == IR_ATOM || k == IR_LIT_I; }
/*--------------------------------------------------------------------------------------------------------------------*/
static int u_compound_kind(int k) { return k == IR_STRUCT || k == IR_ARITH; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string u_build_scalar(int kind, long ival, double dval, const char *lbl) {
    return x86("mov",   "edi", (long)kind)
         + x86("mov",   "rsi", ival)
         + IF(lbl != NULL, x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)lbl, lbl))
         + IF(lbl == NULL, x86("mov", "edx", (long)0))
         + IF(kind == IR_LIT_F, x86("movsd", "xmm0", F64(dval)))
         + IF(kind != IR_LIT_F, x86("xorps", "xmm0", "xmm0"))
         + x86("call",  "rt_node_to_term", (uint64_t)(uintptr_t)(void*)rt_node_to_term);
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_unify() {
    if (PLATFORM_X86)
        return _.bb_lk < 0 ?
               x86("comment", "IR_UNIFY")
             + x86("label",   _.lbl_α)
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + x86("jmp",     "ω") :
               (_.bb_lk == IR_LOGICVAR && _.bb_rk == IR_LOGICVAR && (long)_.bb_li == (long)_.bb_ri) ?
               x86("comment", "IR_UNIFY")
             + x86("label",   _.lbl_α)
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + x86("jmp",     "ω") :
               (_.bb_lk == IR_LOGICVAR && _.bb_rk == IR_LOGICVAR) ?
               x86("comment", "IR_UNIFY")
             + x86("label",   _.lbl_α)
             + x86("mov",     "edi", (long)_.bb_li)
             + x86("mov",     "esi", (long)_.bb_ri)
             + x86("call",    "rt_unify_var_var", (uint64_t)(uintptr_t)(void*)rt_unify_var_var)
             + u_tail() :
               (_.bb_lk == IR_LOGICVAR && u_const_kind(_.bb_rk) && (int)(long)_.bb_li >= 0) ?
               x86("comment", "IR_UNIFY")
             + x86("label",   _.lbl_α)
             + x86("mov",     "edi", (long)_.bb_li)
             + x86("mov",     "esi", (long)_.bb_rk)
             + x86("mov",     "rdx", (long)_.bb_ri)
             + IF(_.bb_rs != NULL, x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)_.bb_rs, _.bb_rs))
             + IF(_.bb_rs == NULL, x86("mov", "ecx", (long)0))
             + x86("xorps",   "xmm0", "xmm0")
             + x86("call",    "rt_unify_const", (uint64_t)(uintptr_t)(void*)rt_unify_const)
             + u_tail() :
               (_.bb_rk == IR_LOGICVAR && u_const_kind(_.bb_lk) && (int)(long)_.bb_ri >= 0) ?
               x86("comment", "IR_UNIFY")
             + x86("label",   _.lbl_α)
             + x86("mov",     "edi", (long)_.bb_ri)
             + x86("mov",     "esi", (long)_.bb_lk)
             + x86("mov",     "rdx", (long)_.bb_li)
             + IF(_.bb_ls != NULL, x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)_.bb_ls, _.bb_ls))
             + IF(_.bb_ls == NULL, x86("mov", "ecx", (long)0))
             + x86("xorps",   "xmm0", "xmm0")
             + x86("call",    "rt_unify_const", (uint64_t)(uintptr_t)(void*)rt_unify_const)
             + u_tail() :
               x86("comment", "IR_UNIFY")
             + x86("label",   _.lbl_α)
             + x86("sub",     "rsp", (long)16)
             + IF(u_compound_kind(_.bb_lk), emit_build_compound_term((const IR_t *)_.bb_ln))
             + IF(!u_compound_kind(_.bb_lk), u_build_scalar(_.bb_lk, (long)_.bb_li, _.bb_ln ? IR_LIT((const IR_t *)_.bb_ln).dval : 0.0, _.bb_ls))
             + x86("mov",     RSP(0), "rax")
             + IF(u_compound_kind(_.bb_rk), emit_build_compound_term((const IR_t *)_.bb_rn))
             + IF(!u_compound_kind(_.bb_rk), u_build_scalar(_.bb_rk, (long)_.bb_ri, _.bb_rn ? IR_LIT((const IR_t *)_.bb_rn).dval : 0.0, _.bb_rs))
             + x86("mov",     "rsi", "rax")
             + x86("mov",     "rdi", RSP(0))
             + x86("add",     "rsp", (long)16)
             + x86("call",    "rt_unify_terms", (uint64_t)(uintptr_t)(void*)rt_unify_terms)
             + u_tail();
    return std::string();
}

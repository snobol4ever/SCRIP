#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "bb_common.h"
extern "C" int   rt_unify_terms(void *l, void *r);
extern "C" int   rt_unify_const(int slot, int kind, long ival, const char *sval, double dval);
extern "C" int   rt_unify_var_var(int lslot, int rslot);
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int  u_present()        { return _.bb_lk >= 0; }
static inline int  u_const_kind(int k){ return k == IR_ATOM || k == IR_LIT_I; }
static inline int  u_compound_kind(int k){ return k == IR_STRUCT || k == IR_ARITH; }
/*--------------------------------------------------------------------------------------------------------------------*/
static inline std::string u_head(const char *msg) {
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", msg));
}
static inline std::string u_vacuous(const char *msg) {
    return u_head(msg) + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω");
}
static inline std::string u_tail() {
    return x86("test", "eax", "eax") + x86("je", "ω") + x86("jmp", "γ")
         + x86("def", "β") + x86("jmp", "ω");
}
static inline std::string u_build_scalar(int kind, long ival, double dval, const char *lbl) {
    return x86("mov", "edi", (long)kind)
         + x86("mov", "rsi", ival)
         + IF(lbl != NULL, x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)lbl, lbl))
         + IF(lbl == NULL, x86("mov", "edx", (long)0))
         + IF(kind == IR_LIT_F, x86("movsd", "xmm0", F64(dval)))
         + IF(kind != IR_LIT_F, x86("xorps", "xmm0", "xmm0"))
         + x86("call", "rt_node_to_term", (uint64_t)(uintptr_t)(void*)rt_node_to_term);
}
static inline std::string u_var_const(int slot, int ck, long ci, const char *clbl) {
    return u_head("# BOX RESOLVE_UNIFY (WAM-CP-7 var-const)  [x86() self-encoding]")
         + x86("mov", "edi", (long)slot)
         + x86("mov", "esi", (long)ck)
         + x86("mov", "rdx", ci)
         + IF(clbl != NULL, x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)clbl, clbl))
         + IF(clbl == NULL, x86("mov", "ecx", (long)0))
         + x86("xorps", "xmm0", "xmm0")
         + x86("call", "rt_unify_const", (uint64_t)(uintptr_t)(void*)rt_unify_const)
         + u_tail();
}
static inline std::string u_general_side(int kind, long ival, const char *lbl, const void *vn) {
    return IF(u_compound_kind(kind), emit_build_compound_term((const IR_t *)vn))
         + IF(!u_compound_kind(kind), u_build_scalar(kind, ival, vn ? IR_LIT((const IR_t *)vn).dval : 0.0, lbl));
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_unify() {
    if (!PLATFORM_X86)
        return std::string();
    if (!u_present())
        return u_vacuous("# BOX RESOLVE_UNIFY: missing children — vacuous success  [x86() self-encoding]");
    if (_.bb_lk == IR_LOGICVAR && _.bb_rk == IR_LOGICVAR && (long)_.bb_li == (long)_.bb_ri)
        return u_vacuous("# BOX RESOLVE_UNIFY (WAM-CP-7 self-unify x=x — vacuous success)  [x86() self-encoding]");
    if (_.bb_lk == IR_LOGICVAR && _.bb_rk == IR_LOGICVAR)
        return u_head("# BOX RESOLVE_UNIFY (WAM-CP-7c var-var — 1 call)  [x86() self-encoding]")
             + x86("mov", "edi", (long)_.bb_li)
             + x86("mov", "esi", (long)_.bb_ri)
             + x86("call", "rt_unify_var_var", (uint64_t)(uintptr_t)(void*)rt_unify_var_var)
             + u_tail();
    if (_.bb_lk == IR_LOGICVAR && u_const_kind(_.bb_rk) && (int)(long)_.bb_li >= 0)
        return u_var_const((int)(long)_.bb_li, _.bb_rk, (long)_.bb_ri, _.bb_rs);
    if (_.bb_rk == IR_LOGICVAR && u_const_kind(_.bb_lk) && (int)(long)_.bb_ri >= 0)
        return u_var_const((int)(long)_.bb_ri, _.bb_lk, (long)_.bb_li, _.bb_ls);
    return u_head("# BOX RESOLVE_UNIFY (general)  [x86() self-encoding]")
         + x86("sub", "rsp", (long)16)
         + FOR(0, 2, [](int i) { return
               IF(i == 0, u_general_side(_.bb_lk, (long)_.bb_li, _.bb_ls, _.bb_ln) + x86("mov", RSP(0), "rax"))
             + IF(i == 1, u_general_side(_.bb_rk, (long)_.bb_ri, _.bb_rs, _.bb_rn) + x86("mov", "rsi", "rax")); })
         + x86("mov", "rdi", RSP(0))
         + x86("add", "rsp", (long)16)
         + x86("call", "rt_unify_terms", (uint64_t)(uintptr_t)(void*)rt_unify_terms)
         + u_tail();
}
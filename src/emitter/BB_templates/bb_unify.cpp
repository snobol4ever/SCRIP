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
    return u_head(msg) + x86("jmp", PORT_GAMMA) + x86("def", PORT_BETA) + x86("jmp", PORT_OMEGA);
}
static inline std::string u_tail() {
    return x86("test", "eax", "eax") + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA) + x86("jmp", PORT_OMEGA);
}
static inline std::string u_build_scalar(int kind, long ival, double dval, const char *lbl) {
    return x86("mov", "edi", (long)kind)
         + x86("mov", "rsi", ival)
         + (lbl ? x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)lbl, lbl) : x86("mov", "edx", (long)0))
         + (kind == IR_LIT_F ? x86("movsd", "xmm0", F64(dval)) : x86("xorps", "xmm0", "xmm0"))
         + x86("call", "rt_node_to_term", (uint64_t)(uintptr_t)(void*)rt_node_to_term);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_unify_str() {
    if (PLATFORM_X86) {
        if (!u_present())
            return u_vacuous("# BOX RESOLVE_UNIFY: missing children — vacuous success  [x86() self-encoding]");
        int  lk = _.bb_lk, rk = _.bb_rk;
        long li = (long)_.bb_li, ri = (long)_.bb_ri;
        const char *ls = _.bb_ls, *rs = _.bb_rs;
        const IR_t *ln = (const IR_t *)_.bb_ln, *rn = (const IR_t *)_.bb_rn;
        if (lk == IR_LOGICVAR && rk == IR_LOGICVAR && li == ri)
            return u_vacuous("# BOX RESOLVE_UNIFY (WAM-CP-7 self-unify x=x — vacuous success)  [x86() self-encoding]");
        if (lk == IR_LOGICVAR && rk == IR_LOGICVAR)
            return u_head("# BOX RESOLVE_UNIFY (WAM-CP-7c var-var — 1 call)  [x86() self-encoding]")
                 + x86("mov", "edi", li)
                 + x86("mov", "esi", ri)
                 + x86("call", "rt_unify_var_var", (uint64_t)(uintptr_t)(void*)rt_unify_var_var)
                 + u_tail();
        {
            int slot = -1, ck = 0; long ci = 0; const char *clbl = (const char *)0;
            if (lk == IR_LOGICVAR && u_const_kind(rk))      { slot = (int)li; ck = rk; ci = ri; clbl = rs; }
            else if (rk == IR_LOGICVAR && u_const_kind(lk)) { slot = (int)ri; ck = lk; ci = li; clbl = ls; }
            if (slot >= 0)
                return u_head("# BOX RESOLVE_UNIFY (WAM-CP-7 var-const)  [x86() self-encoding]")
                     + x86("mov", "edi", (long)slot)
                     + x86("mov", "esi", (long)ck)
                     + x86("mov", "rdx", ci)
                     + (clbl ? x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)clbl, clbl) : x86("mov", "ecx", (long)0))
                     + x86("xorps", "xmm0", "xmm0")
                     + x86("call", "rt_unify_const", (uint64_t)(uintptr_t)(void*)rt_unify_const)
                     + u_tail();
        }
        {
            double lf = ln ? IR_LIT(ln).dval : 0.0, rf = rn ? IR_LIT(rn).dval : 0.0;
            std::string lbuild = u_compound_kind(lk)
                ? emit_build_compound_term(ln)
                : u_build_scalar(lk, li, lf, ls);
            std::string rbuild = u_compound_kind(rk)
                ? emit_build_compound_term(rn)
                : u_build_scalar(rk, ri, rf, rs);
            return u_head("# BOX RESOLVE_UNIFY (general)  [x86() self-encoding]")
                 + x86("sub", "rsp", (long)16)
                 + lbuild
                 + x86("mov", RSP(0), "rax")
                 + rbuild
                 + x86("mov", "rsi", "rax")
                 + x86("mov", "rdi", RSP(0))
                 + x86("add", "rsp", (long)16)
                 + x86("call", "rt_unify_terms", (uint64_t)(uintptr_t)(void*)rt_unify_terms)
                 + u_tail();
        }
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_unify(void) { bb_emit_x86(bb_unify_str()); }

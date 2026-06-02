#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
extern "C" void *rt_node_to_term(int kind, long ival, const char *sval, double dval);
extern "C" int   rt_unify_terms(void *l, void *r);
extern "C" int   rt_unify_const(int slot, int kind, long ival, const char *sval, double dval);
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int  u_present()        { return _.bb_lk >= 0; }
static inline int  u_const_kind(int k){ return k == IR_ATOM || k == IR_LIT_I; }
static inline int  u_deferred()       { int lk = _.bb_lk, rk = _.bb_rk;
                                        return lk == IR_STRUCT || rk == IR_STRUCT || lk == IR_ARITH || rk == IR_ARITH
                                            || lk == IR_LIT_F  || rk == IR_LIT_F; }
/*--------------------------------------------------------------------------------------------------------------------*/
static inline std::string u_head(const char *msg) {
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":") + s_comment(msg));
}
static inline std::string u_vacuous(const char *msg) {
    return u_head(msg) + x86("jmp", PORT_GAMMA) + x86("def", PORT_BETA) + x86("jmp", PORT_OMEGA);
}
static inline std::string u_tail() {
    return x86("test", "eax", "eax") + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA) + x86("jmp", PORT_OMEGA);
}
static inline std::string u_build(int kind, long ival, const char *lbl) {
    return x86("mov", "edi", (long)kind)
         + x86("mov", "rsi", ival)
         + (lbl ? x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)lbl, lbl) : x86("mov", "edx", (long)0))
         + x86("xorps", "xmm0", "xmm0")
         + x86("call", "rt_node_to_term", (uint64_t)(uintptr_t)(void*)rt_node_to_term);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_unify_str() {
    if (PLATFORM_X86) {
        if (!u_present())
            return u_vacuous("# BOX RESOLVE_UNIFY: missing children — vacuous success  [x86() self-encoding]");
        if (u_deferred())
            return x86_bomb("bb_unify: compound/float operand — deferred (PL-HY-1a compound substrate + float-unify)");
        int  lk = _.bb_lk, rk = _.bb_rk;
        long li = (long)_.bb_li, ri = (long)_.bb_ri;
        const char *ls = _.bb_ls, *rs = _.bb_rs;
        if (lk == IR_LOGICVAR && rk == IR_LOGICVAR && li == ri)
            return u_vacuous("# BOX RESOLVE_UNIFY (WAM-CP-7 self-unify x=x — vacuous success)  [x86() self-encoding]");
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
        return u_head("# BOX RESOLVE_UNIFY  [x86() self-encoding]")
             + x86("sub", "rsp", (long)16)
             + u_build(lk, li, ls)
             + x86("mov", RSP(0), "rax")
             + u_build(rk, ri, rs)
             + x86("mov", "rsi", "rax")
             + x86("mov", "rdi", RSP(0))
             + x86("add", "rsp", (long)16)
             + x86("call", "rt_unify_terms", (uint64_t)(uintptr_t)(void*)rt_unify_terms)
             + u_tail();
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_unify(void) { bb_emit_x86(bb_unify_str()); }

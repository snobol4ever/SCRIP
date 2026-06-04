#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
extern long rt_arith(int lk, long li, const char *ls, int rk, long ri, const char *rs, const char *op);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * op_str()  { return _.op_sval ? _.op_sval : "+"; }
static inline int          vacuous() { return _.bb_lk < 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_arith_str() {
    if (PLATFORM_X86) {
        if (vacuous())
            return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                                 + x86("comment", emit_fmt("BOX RESOLVE_ARITH(op='%s')  vacuous [x86() self-encoding]", op_str())))
                 + x86("jmp", PORT_GAMMA)
                 + x86("def", PORT_BETA)
                 + x86("jmp", PORT_OMEGA);
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", emit_fmt("BOX RESOLVE_ARITH(op='%s')  [x86() self-encoding]", op_str())))
             + x86("mov", "edi", (long)_.bb_lk)
             + x86("mov", "esi", (long)_.bb_li)
             + (_.bb_ls ? x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)_.bb_ls, _.bb_ls) : x86("mov", "edx", (long)0))
             + x86("mov", "ecx", (long)_.bb_rk)
             + x86("mov", "r8",  (long)_.bb_ri)
             + (_.bb_rs ? x86("lea", "r9", "[rip + __]", (uint64_t)(uintptr_t)_.bb_rs, _.bb_rs) : x86("mov", "r9d", (long)0))
             + x86("lea", "rax", "[rip + __]", (uint64_t)(uintptr_t)op_str(), _.bb_op_lbl)
             + x86("push", "rax")
             + x86("call", "rt_arith", (uint64_t)(uintptr_t)(void*)rt_arith)
             + x86("add", "rsp", (long)8)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_arith(void) { bb_emit_x86(bb_arith_str()); }

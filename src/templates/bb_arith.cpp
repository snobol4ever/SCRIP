#include <string>
#include <cstring>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
extern long rt_arith(int lk, long li, const char *ls, int rk, long ri, const char *rs, const char *op);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_arith() {
    return x86_alpha()
         + x86("comment", "IR_ARITH")
         + IF(_.bb_lk < 0,
              x86_gamma()
            + x86_beta_trampoline())
         + IF(!(_.bb_lk < 0),
              x86("mov", "edi", (long)_.bb_lk)
            + x86("mov", "esi", (long)_.bb_li)
            + (_.bb_ls ? x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)_.bb_ls, _.bb_ls)
                       : x86("mov", "edx", (long)0))
            + x86("mov", "ecx", (long)_.bb_rk)
            + x86("mov", "r8",  (long)_.bb_ri)
            + (_.bb_rs ? x86("lea", "r9", "[rip + __]", (uint64_t)(uintptr_t)_.bb_rs, _.bb_rs)
                       : x86("mov", "r9d", (long)0))
            + x86("lea", "rax", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : "+"), _.bb_op_lbl)
            + x86("push", "rax")
            + x86("call", "rt_arith", (uint64_t)(uintptr_t)(void*)rt_arith)
            + x86("add", "rsp", (long)8)
            + x86_gamma()
            + x86_beta_trampoline());
}

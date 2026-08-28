#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
long rt_pat_prim_int(const char *varname);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_rpos_body() {
    return x86("comment", "IR_MATCH_RPOS")
         + x86_alpha()
         + IF(_.op_sa >= 0 &&  _.op_zres, x86("mov", "rax", ZOPQ(0, 8)))
         + IF(_.op_sa >= 0 && !_.op_zres, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF(_.op_sa <  0,               x86("mov", "rax", (long)_.op_sb))
         + x86("mov", "ecx", "r15d")
         + x86("sub", "ecx", "eax")
         + x86("cmp", "r14d", "ecx")
         + x86_omega("jne")
         + x86_gamma()
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_rpos() {
    static char b[24];
    if (_.op_sval != NULL) {
        return x86("comment", "IR_MATCH_RPOS defer")
             + x86_alpha()
             + x86("lea",  "rdi", "[rip + __]",
                   (uint64_t)(uintptr_t)(const void *)(_.op_sval + 1), (strtab_label(b, sizeof b, _.op_sval + 1), b))
             + x86("call", "rt_pat_prim_int", (uint64_t)(uintptr_t)(void *)rt_pat_prim_int)
             + x86("test", "rax", "rax")
             + x86_omega("js")
             + x86("mov",  "ecx", "r15d")
             + x86("sub",  "ecx", "eax")
             + x86("cmp",  "r14d", "ecx")
             + x86_omega("jne")
             + x86_gamma()
             + x86_beta_trampoline();
    }
    return bb_match_rpos_body();
}

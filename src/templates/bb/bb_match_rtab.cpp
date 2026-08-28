#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
long rt_pat_prim_int(const char *varname);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_rtab() {
    static char b[24];
    if (_.op_sval != NULL) {
        return x86("comment", "IR_MATCH_RTAB defer")
             + x86_alpha()
             + x86("mov",  LFC(0), "r14d")
             + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval + 1), (strtab_label(b, sizeof b, _.op_sval + 1), b))
             + x86("call", "rt_pat_prim_int", (uint64_t)(uintptr_t)(void *)rt_pat_prim_int)
             + x86("test", "rax", "rax")
             + x86_omega("js")
             + x86("mov",  "ecx", "r15d")
             + x86("sub",  "ecx", "eax")
             + x86("cmp",  "r14d", "ecx")
             + x86_omega("jg")
             + x86("mov",  "r14d", "ecx")
             + x86_gamma()
             + x86_beta()
             + x86("mov",  "r14d", LFC(0))
             + x86_omega();
    }
    if (_.op_zres)
        return x86("comment", "IR_MATCH_RTAB zd")
             + x86_alpha()
             + x86("mov",  LFC(0), "r14d")
             + IF(_.op_sa >= 0, x86("note", ZOPN(0))
                              + x86("mov", "rax", ZOPQ(0, 8)))
             + IF(_.op_sa <  0, x86("mov", "rax", (long)_.op_sb))
             + x86("mov",  "ecx", "r15d")
             + x86("sub",  "ecx", "eax")
             + x86("cmp",  "r14d", "ecx")
             + x86_omega("jg")
             + x86("mov",  "r14d", "ecx")
             + x86_gamma()
             + x86_beta()
             + x86("mov",  "r14d", LFC(0))
             + x86_omega();
    return x86("comment", "IR_MATCH_RTAB")
         + x86_alpha()
         + x86("mov",  LFC(0), "r14d")
         + IF(_.op_sa >= 0, x86("mov", "rax", XSAQ(8)))
         + IF(_.op_sa <  0, x86("mov", "rax", (long)_.op_sb))
         + x86("mov",  "ecx", "r15d")
         + x86("sub",  "ecx", "eax")
         + x86("cmp",  "r14d", "ecx")
         + x86_omega("jg")
         + x86("mov",  "r14d", "ecx")
         + x86_gamma()
         + x86_beta()
         + x86("mov",  "r14d", LFC(0))
         + x86_omega();
}

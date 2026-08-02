#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_rpos_body() {
    return x86("comment", "IR_MATCH_RPOS")   /* ZD arm (A-7/ZD-5b, s25): K=0 — zero-width position predicate from end; scanner-register only (r14d=cursor, r15d=subject-length); static arm reads op_sb, dynamic arm reads ZOPQ(0,8).  x86_beta_trampoline: no cursor movement, no scratch needed. */
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
std::string bb_match_rpos() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres) return bb_match_rpos_body();
    return bb_match_rpos_body();
}

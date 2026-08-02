#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_len_body() {
    return x86("comment", "IR_MATCH_LEN")   /* ZD arm (A-7/ZD-5b, s25/O-7a): K=0 — scanner-register only; static arm uses op_ival (no stack read); dynamic arm reads ZOPQ(0,8) (COERCE_INTEGER predecessor cell). */
         + x86_alpha()
         + IF(_.op_sa >= 0 &&  _.op_zres, x86("note", ZOPN(0)) + x86("mov", "rcx", ZOPQ(0, 8)))
         + IF(_.op_sa >= 0 && !_.op_zres, x86("mov", "rcx", FRQ(_.op_sa + 8)))
         + x86("mov", "eax", "r14d")
         + IF(_.op_sa >= 0, x86("add", "eax", "ecx"))
         + IF(_.op_sa <  0, x86("add", "eax", (long)(int)_.op_ival))
         + x86("cmp", "eax", "r15d")
         + x86_omega("jg")
         + IF(_.op_sa >= 0, x86("add", "r14d", "ecx"))
         + IF(_.op_sa <  0, x86("add", "r14d", (long)(int)_.op_ival))
         + x86_gamma()
         + x86_beta()
         + IF(_.op_sa >= 0 &&  _.op_zres, x86("note", ZOPN(0)) + x86("mov", "rcx", ZOPQ(0, 8)))
         + IF(_.op_sa >= 0 && !_.op_zres, x86("mov", "rcx", FRQ(_.op_sa + 8)))
         + IF(_.op_sa >= 0, x86("sub", "r14d", "ecx"))
         + IF(_.op_sa <  0, x86("sub", "r14d", (long)(int)_.op_ival))
         + x86_omega();
}
std::string bb_match_len() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres) return bb_match_len_body();
    return bb_match_len_body();
}

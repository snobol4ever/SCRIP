#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_pos_body() {
    return x86("comment", "IR_MATCH_POS")
         + x86_alpha()
         + IF(_.op_sa >= 0 &&  _.op_zres, x86("mov", "rax", ZOPQ(0, 8)))
         + IF(_.op_sa >= 0 && !_.op_zres, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF(_.op_sa <  0,               x86("mov", "rax", (long)_.op_sb))
         + x86("cmp", "r14d", "eax")
         + x86_omega("jne")
         + x86_gamma()
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_pos() {
    if (_.op_zres) return bb_match_pos_body();
    return bb_match_pos_body();
}

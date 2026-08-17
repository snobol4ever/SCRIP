#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_rem_body() {
    return x86("comment", "IR_MATCH_REM")   /* ZD arm (A-7/ZD-5b, s25): K=16 — cursor save at ZRESD(0); no arg (always static); moves cursor to end (r15d). */
         + x86_alpha()
         + IF( _.op_zres, x86("mov", ZRESD(0), "r14d"))
         + IF(!_.op_zres, x86("mov", LFC(0), "r14d"))
         + x86("mov", "r14d", "r15d")
         + x86_gamma()
         + x86_beta()
         + IF( _.op_zres, x86("mov", "r14d", ZRESD(0)))
         + IF(!_.op_zres, x86("mov", "r14d", LFC(0)))
         + x86_omega();
}
std::string bb_match_rem() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres) return bb_match_rem_body();
    return bb_match_rem_body();
}

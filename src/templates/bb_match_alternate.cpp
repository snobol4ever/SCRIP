#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
extern "C" int sn4_choice_rbp_off(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string alt_entry_stubs(long N, int cro) {
    std::string r;
    for (long j = 1; j < N; j++)
        r += x86("def", L((int)(20 + j)))
           + x86_lea_rip_id("rax", (j + 1 < N) ? (int)(20 + j + 1) : 19)
           + x86("mov", CROQ(cro, 16), "rax")
           + x86("jmp", PAIR((int)j));
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string alt_sigma_stubs(long N, int cro) {
    std::string r;
    for (long j = 0; j < N; j++)
        r += x86("def", PAIR((int)(2 * N + 2 + j)))
           + x86_lea_rip_id("rax", (int)(40 + j))
           + x86("mov", CROQ(cro, 8), "rax")
           + x86("jmp", PAIR((int)(2 * N)));
    for (long j = 0; j < N; j++)
        r += x86("def", L((int)(40 + j)))
           + x86("jmp", PAIR((int)(N + j)));
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_alternate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    const int cro = sn4_choice_rbp_off();
    return x86("comment", cro ? "IR_MATCH_ALT_NARY (ALT-RBP s128, frame-resident choice record)" : "IR_MATCH_ALT_NARY (ALT-FLAT, s61 own-carved record)")
             + x86_alpha()
             + IF(!cro, x86("sub", "rsp", 32L)) + x86("mov", CROD(cro, 0), "r14d")
             + x86_lea_rip_id("rax", (_.op_ival > 1) ? 21 : 19)
             + x86("mov", CROQ(cro, 16), "rax")
             + x86("jmp", PAIR(0))
             + alt_entry_stubs(_.op_ival, cro)
             + alt_sigma_stubs(_.op_ival, cro)
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86_gamma()
             + x86_beta()
             + x86("mov", "rax", CROQ(cro, 8))
             + x86_jmp_reg("rax")
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86("mov", "r14d", CROD(cro, 0))
             + x86("mov", "rax", CROQ(cro, 16))
             + x86_jmp_reg("rax")
             + x86("def", L(19))
             + (cro ? std::string() : x86("add", "rsp", 32L))
             + x86_omega();
}

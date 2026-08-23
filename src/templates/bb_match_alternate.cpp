#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
extern "C" int sn4_choice_rbp_off(void);
extern "C" int sn4_choice_rbp_off_nd(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string alt_entry_stubs(long N, int cro) {   /* internal labels 21..19+N, plus L(19); alt_sigma_base(N) starts ABOVE this range -- the two series collided at exactly N==21 (porter) */
    std::string r;
    for (long j = 1; j < N; j++)
        r += x86("def", L((int)(20 + j)))
           + x86_lea_rip_id("rax", (j + 1 < N) ? (int)(20 + j + 1) : 19)
           + x86("mov", CROQ(cro, 16), "rax")
           + x86("jmp", PAIR((int)j));
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int alt_sigma_base(long N) { long b = 20 + N; return (int)(b > 40 ? b : 40); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string alt_sigma_stubs(long N, int cro) {
    std::string r; const int sb = alt_sigma_base(N);
    for (long j = 0; j < N; j++)
        r += x86("def", PAIR((int)(2 * N + 2 + j)))
           + x86_lea_rip_id("rax", (int)(sb + j))
           + x86("mov", CROQ(cro, 8), "rax")
           + x86("jmp", PAIR((int)(2 * N)));
    for (long j = 0; j < N; j++)
        r += x86("def", L((int)(sb + j)))
           + x86("jmp", PAIR((int)(N + j)));
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_alternate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    const int cro = sn4_choice_rbp_off_nd();   /* per-node record slot in a multi-choice blob (s266), else the legacy shared slot, else 0 = the FLAT own-carved record */
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

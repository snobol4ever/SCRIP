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
static inline const char * alt_next(int cro) { return CROQ(cro, 16); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string alt_entry_stubs(long N, int cro) {
    return FOR(1, (int)N, [&](int j) { return x86("def", L((int)(20 + j)))
                                            + x86_lea_rip_id("rax", (j + 1 < N) ? (int)(20 + j + 1) : 19)
                                            + x86("mov", alt_next(cro), "rax")
                                            + x86("jmp", PAIR((int)j)); });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int alt_sigma_base(long N) {
    long b = 20 + N;
    return (int)(b > 40 ? b : 40);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string alt_sigma_stubs(long N, int cro) {
    return FOR(0, (int)N, [&](int j) { return x86("def", PAIR((int)(2 * N + 2 + j)))
                                            + x86_lea_rip_id("rax", (int)(alt_sigma_base(N) + j))
                                            + (cro ? x86("mov", CROQ(cro, 8), "rax")
                                               : _.op_alt_cell ? x86("sub", "rsp", 16L)
                                                               + x86("mov", RDQ("rsp", 0), "rax")
                                                               : x86("mov", CROQ(cro, 8), "rax"))
                                            + x86("jmp", PAIR((int)(2 * N))); })
         + FOR(0, (int)N, [&](int j) { return x86("def", L((int)(alt_sigma_base(N) + j)))
                                            + x86("jmp", PAIR((int)(N + j))); });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_alternate() {
    x86_begin();
    return x86("comment",
        _.op_sa ? "IR_MATCH_ALT_NARY (ALT-RBP s128, frame-resident choice record)"
            : "IR_MATCH_ALT_NARY (ALT-FLAT: 32B record carved at alpha -- cursor@0, beta-target@8, next-alternative@16.  "
                "When an ARM leaves live frames at its gamma (a nested alternation, an ARBNO, a DEFER, a FENCE -- op_alt_cell) "
                    "the beta target instead rides in a 16B cell pushed by the sigma stub at that gamma-time stack top and popped "
                        "by beta, because [rsp+8] then addresses the ARM's record and the two beta ports jumped to each other forever; "
                            "a primitive arm keeps the in-place write, so its footprint and every static accounting of it stand)")
             + x86_alpha()
             + IF(_.op_alt_first, x86("cmp", "r14d", "r15d")
             + x86("jge", L(17))
             + x86("movsxd", "rcx", "r14d")
             + x86("movzx", "eax", "[r13+rcx]")
                                  + x86("lea", "rcx", "[rip + __]", x86_csettab_ptr(_.op_alt_first ? _.op_alt_first : ""), x86_csettab_lbl(_.op_alt_first ? _.op_alt_first : "").c_str())
                                  + x86("movzx", "eax", "[rcx+rax]")
                                  + x86("test", "eax", "eax")
                                  + x86("je", L(17)))
             + IF(!_.op_sa, x86("sub", "rsp", 32L))
             + x86("mov", CROD(_.op_sa, 0), "r14d")
             + x86_lea_rip_id("rax", (_.op_ival > 1) ? 21 : 19)
             + x86("mov", alt_next(_.op_sa), "rax")
             + x86("jmp", PAIR(0))
             + alt_entry_stubs(_.op_ival, _.op_sa)
             + alt_sigma_stubs(_.op_ival, _.op_sa)
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86_gamma()
             + x86_beta()
             + (_.op_sa ? x86("mov", "rax", CROQ(_.op_sa, 8))
              : _.op_alt_cell ? x86("mov", "rax", RDQ("rsp", 0))
              + x86("add", "rsp", 16L)
                              : x86("mov", "rax", CROQ(_.op_sa, 8)))
             + x86_jmp_reg("rax")
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86("def", PAIR((int)(3 * _.op_ival + 2)))
             + x86("mov", "r14d", CROD(_.op_sa, 0))
             + x86("mov", "rax", alt_next(_.op_sa))
             + x86_jmp_reg("rax")
             + x86("def", L(19))
             + (_.op_sa ? std::string() : x86("add", "rsp", 32L))
             + IF(_.op_alt_first, x86("def", L(17)))
             + x86_omega();
}

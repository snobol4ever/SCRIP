#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define alt_M() (_.op_alt_M < 0 ? 0 : _.op_alt_M)
#define alt_pad(j) ((_.op_alt_M < 0 || !_.op_alt_pad) ? 0 : _.op_alt_pad[(j)])
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string alt_entry_stubs(long N) {
    return FOR(1, (int)N, [&](int j) { return x86("def", L((int)(20 + j)))
                                            + x86_lea_rip_id("rax", (j + 1 < N) ? (int)(20 + j + 1) : 19)
                                            + x86("mov", RDQ("rsp", 16), "rax")
                                            + x86("jmp", PAIR((int)j)); });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define alt_sigma_base(N) ((int)((20 + (N)) > 40 ? (20 + (N)) : 40))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string alt_sigma_stubs(long N) {
    return FOR(0, (int)N, [&](int j) { return x86("def", PAIR((int)(2 * N + 2 + j)))
                                            + IF(alt_pad(j) > 0, x86("sub", "rsp", (long)alt_pad(j)))
                                            + x86_lea_rip_id("rax", (int)(alt_sigma_base(N) + j))
                                            + (_.op_alt_cell ? x86("sub", "rsp", 16L)
                                                             + x86("mov", RDQ("rsp", 0), "rax")
                                                             : x86("mov", RDQ("rsp", alt_M() + 8), "rax"))
                                            + x86("jmp", PAIR((int)(2 * N))); })
         + FOR(0, (int)N, [&](int j) { return x86("def", L((int)(alt_sigma_base(N) + j)))
                                            + IF(alt_pad(j) > 0, x86("add", "rsp", (long)alt_pad(j)))
                                            + x86("jmp", PAIR((int)(N + j))); });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_alternate() {
    x86_begin();
    return x86("comment",
        "IR_MATCH_ALT_NARY (ALT-FLAT, UNIFORM EXIT: a 32B record carved at alpha -- cursor@0, beta-target@8, next-alternative@16; every arm's sigma stub pads its exit to the "
            "alternation's largest arm and its beta stub unpads before the arm resumes, so the join is one depth and the beta target sits in the record at a fixed distance; "
                "an arm with an unbounded kind leaves the exit indefinite and the beta target then rides a 16B cell pushed at the gamma-time top, op_alt_cell)")
             + x86_alpha()
             + IF(_.op_alt_first, x86("cmp", "r14d", "r15d")
             + x86("jge", L(17))
             + x86("movsxd", "rcx", "r14d")
             + x86("movzx", "eax", "[r13+rcx]")
                                  + x86("lea", "rcx", "[rip + __]", x86_csettab_ptr(_.op_alt_first ? _.op_alt_first : ""), x86_csettab_lbl(_.op_alt_first ? _.op_alt_first : "").c_str())
                                  + x86("movzx", "eax", "[rcx+rax]")
                                  + x86("test", "eax", "eax")
                                  + x86("je", L(17)))
             + x86("sub", "rsp", 32L)
             + x86("mov", RDD("rsp", 0), "r14d")
             + x86_lea_rip_id("rax", (_.op_ival > 1) ? 21 : 19)
             + x86("mov", RDQ("rsp", 16), "rax")
             + x86("jmp", PAIR(0))
             + alt_entry_stubs(_.op_ival)
             + alt_sigma_stubs(_.op_ival)
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86_gamma()
             + x86_beta()
             + (_.op_alt_cell ? x86("mov", "rax", RDQ("rsp", 0))
                              + x86("add", "rsp", 16L)
                              : x86("mov", "rax", RDQ("rsp", alt_M() + 8)))
             + x86_jmp_reg("rax")
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86("def", PAIR((int)(3 * _.op_ival + 2)))
             + x86("mov", "r14d", RDD("rsp", 0))
             + x86("mov", "rax", RDQ("rsp", 16))
             + x86_jmp_reg("rax")
             + x86("def", L(19))
             + x86("add", "rsp", 32L)
             + IF(_.op_alt_first, x86("def", L(17)))
             + x86_omega();
}

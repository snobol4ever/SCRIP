#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
extern "C" long rt_sg_scan_member(void);
extern "C" long rt_sg_scan_nonmember(void);
extern "C" long rt_sg_member(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define CSK() ((long) strlen(_.op_sval ? _.op_sval : ""))
static long an_chainp() { return _.op_sa < 0 && CSK() >= 2 && CSK() <= ZC_CSET_CHAIN_MAX; }
static long an_tablep() { return _.op_sa < 0 && (CSK() == 0 || CSK() > ZC_CSET_CHAIN_MAX); }
static std::string an_memb(long i) { return i >= CSK() ? x86_omega() + x86("def", L(0)) : x86("cmp", "esi", (long)(unsigned char)_.op_sval[i]) + x86("je", L(0)) + an_memb(i + 1); }
std::string bb_match_any() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char c[24];
    const void * ct = an_tablep() ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    return x86("comment", "IR_MATCH_ANY")
         + x86_alpha()
         + x86("mov",    "eax", "r14d")
         + x86("cmp",    "eax", "r15d")
         + x86_omega("jge")
         + x86("movsxd", "rcx", "r14d")
         + x86("movzx",  "esi", "[r13+rcx]")
         + IF(_.op_sa >= 0 && ZC_SPAN_GUTS == ZC_SPAN_GUTS_INLINE,
             x86("mov",  "r8",  FRQ(_.op_sa + 8))
           + x86("mov",  "r9d", FR(_.op_sa + 4))
           + x86("mov",  "edx", (long)0)
           + x86("def",  L(5))
           + x86("cmp",  "edx", "r9d")
           + x86_omega("jge")
           + x86("movzx", "edi", "[r8+rdx]")
           + x86("cmp",  "esi", "edi")
           + x86("je",   L(6))
           + x86("add",  "edx", (long)1)
           + x86("jmp",  L(5))
           + x86("def",  L(6)))
         + IF(_.op_sa >= 0 && ZC_SPAN_GUTS == ZC_SPAN_GUTS_CALL,
             x86("mov",  "edi", "esi")
           + x86("mov",  "rsi", FRQ(_.op_sa + 8))
           + x86("mov",  "edx", FR(_.op_sa + 4))
           + x86("call", "rt_sg_member", (uint64_t)(uintptr_t)(void *)rt_sg_member)
           + x86("test", "eax", "eax")
           + x86_omega("je"))
         + IF(_.op_sa < 0 && CSK() == 1,
             x86("cmp",  "esi", (long)(unsigned char)(_.op_sval ? _.op_sval : " ")[0])
           + x86_omega("jne"))
         + IF(an_chainp(), an_memb(0))
         + IF(an_tablep(),
             x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c)
           + x86("cmpb0", "[rdi+rsi]", "0")
           + x86_omega("je"))
         + x86("add",    "r14d", (long)1)
         + x86_gamma()
         + x86_beta()
         + x86("sub",    "r14d", (long)1)
         + x86_omega();
}

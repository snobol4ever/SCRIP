#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define CSK() ((long) strlen(_.op_sval ? _.op_sval : ""))
static long na_chainp() { return _.op_sa < 0 && CSK() >= 2 && CSK() <= ZC_CSET_CHAIN_MAX; }
static long na_tablep() { return _.op_sa < 0 && (CSK() == 0 || CSK() > ZC_CSET_CHAIN_MAX); }
static std::string na_memb(long i) { return i >= CSK() ? std::string() : x86("cmp", "esi", (long)(unsigned char)_.op_sval[i]) + x86_omega("je") + na_memb(i + 1); }
std::string bb_match_notany() {
    if (!PLATFORM_X86) return std::string();
    static char c[24];
    const void * ct = na_tablep() ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    return x86("comment", "IR_MATCH_NOTANY")
         + x86_alpha()
         + x86("mov",    "eax", "r14d")
         + x86("cmp",    "eax", "r15d")
         + x86_omega("jge")
         + x86("movsxd", "rcx", "r14d")
         + x86("movzx",  "esi", "[r13+rcx]")
         + IF(_.op_sa >= 0,
             x86("mov",  "rdi", FRQ(_.op_sa + 8))
           + x86("call", "strchr", (uint64_t)(uintptr_t)(void *)(const char *(*)(const char *, int))strchr)
           + x86("test", "rax", "rax")
           + x86_omega("jne"))
         + IF(_.op_sa < 0 && CSK() == 1,
             x86("cmp",  "esi", (long)(unsigned char)(_.op_sval ? _.op_sval : " ")[0])
           + x86_omega("je"))
         + IF(na_chainp(), na_memb(0))
         + IF(na_tablep(),
             x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c)
           + x86("cmpb0", "[rdi+rsi]", "0")
           + x86_omega("jne"))
         + x86("add",    "r14d", (long)1)
         + x86_gamma()
         + x86_beta()
         + x86("sub",    "r14d", (long)1)
         + x86_omega();
}

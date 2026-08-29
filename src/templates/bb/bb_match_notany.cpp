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
extern "C" long rt_pat_prim_str(const char *varname, const char **out_ptr, long *out_len);
#define CSK() ((long) strlen(_.op_sval ? _.op_sval : ""))
static char na_nlb[24];
static char na_dlb[24];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define na_gu() (_.op_sa < 0)
#define na_gi() (_.op_sa >= 0)
#define na_ndl_r8() (_.op_sa >= 0 \
         ? x86("mov", "r8",  XSAQ(8)) \
         + x86("mov", "ecx", XSAD(4)) \
         : x86("lea", "r8",  "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), na_nlb) \
         + x86("mov32", "ecx", CSK()))
#define na_chainp() (na_gu() && CSK() >= 2 && CSK() <= ZC_CSET_CHAIN_MAX)
#define na_tablep() (na_gu() && (CSK() == 0 || CSK() > ZC_CSET_CHAIN_MAX))
static std::string na_memb(long i) { return i >= CSK() ? std::string() : x86("cmp", "esi", (long)(unsigned char)_.op_sval[i]) + x86_omega("je") + na_memb(i + 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_notany() {
    if (_.node && _.node->pat_static && _.op_sval)
        return x86("comment", "IR_MATCH_NOTANY defer")
             + x86_alpha()
             + x86("mov",    "eax", "r14d")
             + x86("cmp",    "eax", "r15d")
             + x86_omega("jge")
             + x86("lea",    "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)_.op_sval, (strtab_label(na_dlb, sizeof na_dlb, _.op_sval), na_dlb))
             + x86("lea",    "rsi", LFC(0))
             + x86("lea",    "rdx", LFC(8))
             + x86("call",   "rt_pat_prim_str", (uint64_t)(uintptr_t)(void *)rt_pat_prim_str)
             + x86("test",   "rax", "rax")
             + x86_omega("js")
             + x86("movsxd", "rcx", "r14d")
             + x86("movzx",  "edi", "[r13+rcx]")
             + x86("mov",    "rsi", LFCQ(0))
             + x86("mov",    "edx", LFC(8))
             + x86("call",   "rt_sg_member", (uint64_t)(uintptr_t)(void *)rt_sg_member)
             + x86("test",   "eax", "eax")
             + x86_omega("jne")
             + x86("add",    "r14d", (long)1)
             + x86_gamma()
             + x86_beta()
             + x86("sub",    "r14d", (long)1)
             + x86_omega();
    static char c[24];
    const void * ct = na_tablep() ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    if (_.op_zres && _.op_sa >= 0)
        return x86("comment", "IR_MATCH_NOTANY zd")
             + x86_alpha()
             + x86("mov",    "eax", "r14d")
             + x86("cmp",    "eax", "r15d")
             + x86_omega("jge")
             + x86("movsxd", "rcx", "r14d")
             + x86("movzx",  "esi", "[r13+rcx]")
             + x86("mov",    "edi", "esi")
             + x86("note",   ZOPN(0))
             + x86("mov",    "rsi", ZOPQ(0, 8))
             + x86("note",   ZOPN(0))
             + x86("mov",    "edx", ZOPD(0, 4))
             + x86("call",   "rt_sg_member", (uint64_t)(uintptr_t)(void *)rt_sg_member)
             + x86("test",   "eax", "eax")
             + x86_omega("jne")
             + x86("add",    "r14d", (long)1)
             + x86_gamma()
             + x86_beta()
             + x86("sub",    "r14d", (long)1)
             + x86_omega();
    return x86("comment", "IR_MATCH_NOTANY")
         + x86_alpha()
         + x86("mov",    "eax", "r14d")
         + x86("cmp",    "eax", "r15d")
         + x86_omega("jge")
         + x86("movsxd", "rcx", "r14d")
         + x86("movzx",  "esi", "[r13+rcx]")
         + IF(na_gi(),
             na_ndl_r8()
           + x86("mov",  "edx", (long)0)
           + x86("def",  L(5))
           + x86("cmp",  "edx", "ecx")
           + x86("jge",  L(6))
           + x86("movzx", "edi", "[r8+rdx]")
           + x86("cmp",  "esi", "edi")
           + x86_omega("je")
           + x86("add",  "edx", (long)1)
           + x86("jmp",  L(5))
           + x86("def",  L(6)))
         + IF(na_gu() && CSK() == 1,
             x86("cmp",  "esi", (long)(unsigned char)(_.op_sval ? _.op_sval : " ")[0])
           + x86_omega("je"))
         + IF(na_chainp(), na_memb(0))
         + IF(na_tablep(),
             x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c)
           + (sn4_cset32()
              ? x86("bt", "[rdi]", "esi") + x86_omega("jc")
              : x86("cmpb0", "[rdi+rsi]", "0") + x86_omega("jne")))
         + x86("add",    "r14d", (long)1)
         + x86_gamma()
         + x86_beta()
         + x86("sub",    "r14d", (long)1)
         + x86_omega();
}

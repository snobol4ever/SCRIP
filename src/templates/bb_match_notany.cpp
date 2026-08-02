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
static char na_nlb[24];
static long na_gu() { return _.op_sa < 0 && (ZC_LIT_GUTS == ZC_LIT_GUTS_UNROLL || ZC_LIT_GUTS == ZC_LIT_GUTS_RANGE); }
static long na_rangep() { return _.op_sa < 0 && ZC_LIT_GUTS == ZC_LIT_GUTS_RANGE; }
static unsigned char na_rlo[128], na_rhi[128]; static long na_rn;
static void na_ranges() { unsigned char in[256]; memset(in, 0, 256); for (const unsigned char * p = (const unsigned char *)(_.op_sval ? _.op_sval : ""); *p; p++) in[*p] = 1; na_rn = 0; for (int b = 0; b < 256; b++) if (in[b]) { if (na_rn && na_rhi[na_rn - 1] == b - 1) na_rhi[na_rn - 1] = (unsigned char)b; else { na_rlo[na_rn] = na_rhi[na_rn] = (unsigned char)b; na_rn++; } } }
static std::string na_rtest(long i) { return na_rlo[i] == na_rhi[i] ? x86("cmp", "esi", (long)na_rlo[i]) + x86_omega("je") : x86("mov", "eax", "esi") + x86("sub", "eax", (long)na_rlo[i]) + x86("cmp", "eax", (long)(na_rhi[i] - na_rlo[i])) + x86_omega("jbe"); }
static std::string na_rmemb(long i) { return i >= na_rn ? std::string() : na_rtest(i) + na_rmemb(i + 1); }
static long na_gi() { return _.op_sa >= 0 ? ZC_SPAN_GUTS == ZC_SPAN_GUTS_INLINE : ZC_LIT_GUTS == ZC_LIT_GUTS_INLINE; }
static long na_gc() { return _.op_sa >= 0 ? ZC_SPAN_GUTS == ZC_SPAN_GUTS_CALL   : ZC_LIT_GUTS == ZC_LIT_GUTS_CALL; }
static std::string na_ndl_r8()  { return _.op_sa >= 0 ? x86("mov", "r8",  FRQ(_.op_sa + 8)) + x86("mov", "r9d", FR(_.op_sa + 4)) : x86("lea", "r8",  "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), na_nlb) + x86("mov32", "r9d", CSK()); }
static std::string na_ndl_rsi() { return _.op_sa >= 0 ? x86("mov", "rsi", FRQ(_.op_sa + 8)) + x86("mov", "edx", FR(_.op_sa + 4)) : x86("lea", "rsi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), na_nlb) + x86("mov32", "edx", CSK()); }
static long na_chainp() { return na_gu() && !na_rangep() && CSK() >= 2 && CSK() <= ZC_CSET_CHAIN_MAX; }
static long na_tablep() { return na_gu() && !na_rangep() && (CSK() == 0 || CSK() > ZC_CSET_CHAIN_MAX); }
static std::string na_memb(long i) { return i >= CSK() ? std::string() : x86("cmp", "esi", (long)(unsigned char)_.op_sval[i]) + x86_omega("je") + na_memb(i + 1); }
std::string bb_match_notany() {
    if (!PLATFORM_X86) return std::string();
    static char c[24];
    const void * ct = na_tablep() ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    if (na_rangep()) na_ranges();
    if (_.op_sa < 0 && ZC_LIT_GUTS != ZC_LIT_GUTS_UNROLL) strtab_label(na_nlb, sizeof na_nlb, _.op_sval ? _.op_sval : "");
    if (_.op_zres && _.op_sa >= 0)
        return x86("comment", "IR_MATCH_NOTANY zd")
             + x86_alpha()
             + x86("mov",    "eax", "r14d")
             + x86("cmp",    "eax", "r15d")
             + x86_omega("jge")
             + x86("movsxd", "rcx", "r14d")
             + x86("movzx",  "esi", "[r13+rcx]")
             + x86("mov",    "edi", "esi")
             + x86("note",   ZOPN(0)) + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note",   ZOPN(0)) + x86("mov", "edx", ZOPD(0, 4))
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
           + x86("cmp",  "edx", "r9d")
           + x86("jge",  L(6))
           + x86("movzx", "edi", "[r8+rdx]")
           + x86("cmp",  "esi", "edi")
           + x86_omega("je")
           + x86("add",  "edx", (long)1)
           + x86("jmp",  L(5))
           + x86("def",  L(6)))
         + IF(na_gc(),
             x86("mov",  "edi", "esi")
           + na_ndl_rsi()
           + x86("call", "rt_sg_member", (uint64_t)(uintptr_t)(void *)rt_sg_member)
           + x86("test", "eax", "eax")
           + x86_omega("jne"))
         + IF(na_rangep(), na_rmemb(0))
         + IF(na_gu() && !na_rangep() && CSK() == 1,
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

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
#define CSK() ((long) strlen(_.op_sval ? _.op_sval : ""))
static char an_nlb[24];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long an_gu() { return _.op_sa < 0 && (ZC_LIT_GUTS == ZC_LIT_GUTS_UNROLL || ZC_LIT_GUTS == ZC_LIT_GUTS_RANGE); }
static long an_rangep() { return _.op_sa < 0 && ZC_LIT_GUTS == ZC_LIT_GUTS_RANGE; }
static unsigned char an_rlo[128], an_rhi[128]; static long an_rn;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void an_ranges() { unsigned char in[256]; memset(in, 0, 256); for (const unsigned char * p = (const unsigned char *)(_.op_sval ? _.op_sval : ""); *p; p++) in[*p] = 1; an_rn = 0; for (int b = 0; b < 256; b++) if (in[b]) { if (an_rn && an_rhi[an_rn - 1] == b - 1) an_rhi[an_rn - 1] = (unsigned char)b; else { an_rlo[an_rn] = an_rhi[an_rn] = (unsigned char)b; an_rn++; } } }
static std::string an_rtest(long i) { return an_rlo[i] == an_rhi[i] ? x86("cmp", "esi", (long)an_rlo[i]) + x86("je", L(0)) : x86("mov", "eax", "esi") + x86("sub", "eax", (long)an_rlo[i]) + x86("cmp", "eax", (long)(an_rhi[i] - an_rlo[i])) + x86("jbe", L(0)); }
static std::string an_rmemb(long i) { return i >= an_rn ? x86_omega() + x86("def", L(0)) : an_rtest(i) + an_rmemb(i + 1); }
static long an_gi() { return _.op_sa >= 0 ? ZC_SPAN_GUTS == ZC_SPAN_GUTS_INLINE : ZC_LIT_GUTS == ZC_LIT_GUTS_INLINE; }
static long an_gc() { return _.op_sa >= 0 ? ZC_SPAN_GUTS == ZC_SPAN_GUTS_CALL   : ZC_LIT_GUTS == ZC_LIT_GUTS_CALL; }
static std::string an_ndl_r8()  { return _.op_sa >= 0 ? x86("mov", "r8",  XSAQ(8)) + x86("mov", "ecx", XSAD(4)) : x86("lea", "r8",  "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), an_nlb) + x86("mov32", "ecx", CSK()); }
static std::string an_ndl_rsi() { return _.op_sa >= 0 ? x86("mov", "rsi", XSAQ(8)) + x86("mov", "edx", XSAD(4)) : x86("lea", "rsi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), an_nlb) + x86("mov32", "edx", CSK()); }
static long an_chainp() { return an_gu() && !an_rangep() && CSK() >= 2 && CSK() <= ZC_CSET_CHAIN_MAX; }
static long an_tablep() { return an_gu() && !an_rangep() && (CSK() == 0 || CSK() > ZC_CSET_CHAIN_MAX); }
static std::string an_memb(long i) { return i >= CSK() ? x86_omega() + x86("def", L(0)) : x86("cmp", "esi", (long)(unsigned char)_.op_sval[i]) + x86("je", L(0)) + an_memb(i + 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_any() {
    x86_begin();
    static char c[24];
    const void * ct = an_tablep() ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    if (an_rangep()) an_ranges();
    if (_.op_sa < 0 && ZC_LIT_GUTS != ZC_LIT_GUTS_UNROLL) strtab_label(an_nlb, sizeof an_nlb, _.op_sval ? _.op_sval : "");
    if (_.op_zres && _.op_sa >= 0)
        return x86("comment", "IR_MATCH_ANY zd")
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
             + x86_omega("je")
             + x86("add",    "r14d", (long)1)
             + x86_gamma()
             + x86_beta()
             + x86("sub",    "r14d", (long)1)
             + x86_omega();
    return x86("comment", "IR_MATCH_ANY")
         + x86_alpha()
         + x86("mov",    "eax", "r14d")
         + x86("cmp",    "eax", "r15d")
         + x86_omega("jge")
         + x86("movsxd", "rcx", "r14d")
         + x86("movzx",  "esi", "[r13+rcx]")
         + IF(an_gi(),
             an_ndl_r8()
           + x86("mov",  "edx", (long)0)
           + x86("def",  L(5))
           + x86("cmp",  "edx", "ecx")
           + x86_omega("jge")
           + x86("movzx", "edi", "[r8+rdx]")
           + x86("cmp",  "esi", "edi")
           + x86("je",   L(6))
           + x86("add",  "edx", (long)1)
           + x86("jmp",  L(5))
           + x86("def",  L(6)))
         + IF(an_gc(),
             x86("mov",  "edi", "esi")
           + an_ndl_rsi()
           + x86("call", "rt_sg_member", (uint64_t)(uintptr_t)(void *)rt_sg_member)
           + x86("test", "eax", "eax")
           + x86_omega("je"))
         + IF(an_rangep(), an_rmemb(0))
         + IF(an_gu() && !an_rangep() && CSK() == 1,
             x86("cmp",  "esi", (long)(unsigned char)(_.op_sval ? _.op_sval : " ")[0])
           + x86_omega("jne"))
         + IF(an_chainp(), an_memb(0))
         + IF(an_tablep(),
             x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c)
           + (sn4_cset32() ? x86("bt", "[rdi]", "esi") + x86_omega("jnc") : x86("cmpb0", "[rdi+rsi]", "0") + x86_omega("je")))
         + x86("add",    "r14d", (long)1)
         + x86_gamma()
         + x86_beta()
         + x86("sub",    "r14d", (long)1)
         + x86_omega();
}

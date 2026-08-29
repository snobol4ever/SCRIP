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
static char bk_nlb[24];
static char bk_dlb[24];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bk_gu() (_.op_sa < 0)
#define bk_gi() (_.op_sa >= 0)
#define bk_ndl_r8() ( \
      _.op_sa >= 0 \
    ? x86("mov", "r8", XSAQ(8)) \
    : x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), bk_nlb) \
)
#define bk_len_eax() ( \
      _.op_sa >= 0 \
    ? x86("mov", "eax", XSAD(4)) \
    : x86("mov32", "eax", CSK()) \
)
#define bk_chainp() (bk_gu() && CSK() >= 1 && CSK() <= ZC_CSET_CHAIN_MAX)
#define bk_tablep() (bk_gu() && !bk_chainp())
static std::string bk_memb(long i) {
    return i >= CSK() ? std::string()
         : x86("cmp", "esi", (long)(unsigned char)_.op_sval[i])
         + x86("je",  L(1))
         + bk_memb(i + 1);
}
#define bk_char() ( \
      x86("cmp",   "ecx", "r15d") \
    + x86_omega("jge") \
    + x86("movzx", "esi", "[r13+rcx]") \
    + (bk_chainp() ? bk_memb(0) \
     : (sn4_cset32() \
        ? x86("bt",    "[rdi]", "esi") \
        + x86("jc",    L(1)) \
        : x86("cmpb0", "[rdi+rsi]", "0") \
        + x86("jnz",   L(1)))) \
    + x86("add",   "ecx", (long)1) \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_break() {
    x86_begin();
    if (_.node && _.node->pat_static && _.op_sval)
        return x86("comment", "IR_MATCH_BREAK defer")
             + x86_alpha()
             + x86("lea",   "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)_.op_sval, (strtab_label(bk_dlb, sizeof bk_dlb, _.op_sval), bk_dlb))
             + x86("lea",   "rsi", LFC(4))
             + x86("lea",   "rdx", LFC(12))
             + x86("call",  "rt_pat_prim_str", (uint64_t)(uintptr_t)(void *)rt_pat_prim_str)
             + x86("test",  "rax", "rax")
             + x86_omega("js")
             + x86("mov",   "edi", "r14d")
             + x86("mov",   "rsi", LFCQ(4))
             + x86("mov",   "edx", LFC(12))
             + x86("call",  "rt_sg_scan_member", (uint64_t)(uintptr_t)(void *)rt_sg_scan_member)
             + x86("cmp",   "eax", "r15d")
             + x86_omega("jge")
             + x86("mov",   LFC(0), "r14d")
             + x86("mov",   "r14d", "eax")
             + x86_gamma()
             + x86_beta()
             + x86("mov",   "r14d", LFC(0))
             + x86_omega();
    static char c[24];
    const void * ct = bk_tablep() ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    if (_.op_zres && _.op_sa >= 0)
        return x86("comment", "IR_MATCH_BREAK zd")
             + x86_alpha()
             + x86("mov",    "edi", "r14d")
             + x86("note",   ZOPN(0))
             + x86("mov",    "rsi", ZOPQ(0, 8))
             + x86("note",   ZOPN(0))
             + x86("mov",    "edx", ZOPD(0, 4))
             + x86("call",   "rt_sg_scan_member", (uint64_t)(uintptr_t)(void *)rt_sg_scan_member)
             + x86("cmp",    "eax", "r15d")
             + x86_omega("jge")
             + x86("mov",    LFC(0), "r14d")
             + x86("mov",    "r14d", "eax")
             + x86_gamma()
             + x86_beta()
             + x86("mov",    "r14d", LFC(0))
             + x86_omega();
    return x86("comment", "IR_MATCH_BREAK")
         + x86_alpha()
         + IF(bk_gi(),
              x86("mov",    LFC(0), (long)0)
            + bk_ndl_r8()
            + x86("def",    L(0))
            + x86("mov",    "eax", "r14d")
            + x86("add",    "eax", LFC(0))
            + x86("cmp",    "eax", "r15d")
            + x86_omega("jge")
            + x86("movsxd", "rcx", "eax")
            + x86("movzx",  "esi", "[r13+rcx]")
            + bk_len_eax()
            + x86("mov",    "edx", (long)0)
            + x86("def",    L(2))
            + x86("cmp",    "edx", "eax")
            + x86("jge",    L(3))
            + x86("movzx",  "edi", "[r8+rdx]")
            + x86("cmp",    "esi", "edi")
            + x86("je",     L(1))
            + x86("add",    "edx", (long)1)
            + x86("jmp",    L(2))
            + x86("def",    L(3))
            + x86("add",    LFC(0), (long)1)
            + x86("jmp",    L(0))
            + x86("def",    L(1))
            + x86("mov",    "eax", "r14d")
            + x86("add",    "eax", LFC(0))
            + x86("mov",    LFC(0), "r14d")
            + x86("mov",    "r14d", "eax"))
         + IF(bk_gu(),
              IF(bk_tablep(), x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c))
            + x86("movsxd", "rcx", "r14d")
            + x86("def",    L(0))
            + bk_char()
            + x86("jmp",    L(0))
            + x86("def",    L(1))
            + x86("mov",    LFC(0), "r14d")
            + x86("mov",    "r14d", "ecx"))
         + x86_gamma()
         + x86_beta()
         + x86("mov",    "r14d", LFC(0))
         + x86_omega();
}

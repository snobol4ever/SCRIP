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
static char sp_nlb[24];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define sp_gu() (_.op_sa < 0)
#define sp_gi() (_.op_sa >= 0)
#define sp_ndl_r8() ( \
      _.op_sa >= 0 \
    ? x86("mov", "r8", XSAQ(8)) \
    : x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), sp_nlb) \
)
#define sp_len_eax() ( \
      _.op_sa >= 0 \
    ? x86("mov", "eax", XSAD(4)) \
    : x86("mov32", "eax", CSK()) \
)
#define sp_chainp() (sp_gu() && CSK() >= 1 && CSK() <= ZC_CSET_CHAIN_MAX)
#define sp_tablep() (sp_gu() && !sp_chainp())
static std::string sp_memb(long u, long i) {
    return i >= CSK()
         ? x86("jmp", L(1))
         + x86("def", L(10 + u))
         : x86("cmp", "esi", (long)(unsigned char)_.op_sval[i])
         + x86("je",  L(10 + u))
         + sp_memb(u, i + 1);
}
#define sp_char(u) ( \
      x86("cmp",   "ecx", "r15d") \
    + x86("jge",   L(1)) \
    + x86("movzx", "esi", "[r13+rcx]") \
    + (sp_chainp() ? sp_memb(u, 0) \
     : (sn4_cset32() \
        ? x86("bt",    "[rdi]", "esi") \
        + x86("jnc",   L(1)) \
        : x86("cmpb0", "[rdi+rsi]", "0") \
        + x86("je",    L(1)))) \
    + x86("add",   "ecx", (long)1) \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_span() {
    x86_begin();
    static char c[24];
    const void * ct = sp_tablep() ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    if (_.op_zres && _.op_sa >= 0)
             return x86("comment", "IR_MATCH_SPAN zd")
             + x86_alpha()
             + x86("mov",    LFC(0), "r14d")
             + x86("def",    L(0))
             + x86("mov",    "eax", LFC(0))
             + x86("cmp",    "eax", "r15d")
             + x86("jge",    L(1))
             + x86("movsxd", "rcx", "eax")
             + x86("movzx",  "edi", "[r13+rcx]")
             + x86("note",   ZOPN(0))
             + x86("mov",    "rsi", ZOPQ(0, 8))
             + x86("note",   ZOPN(0))
             + x86("mov",    "edx", ZOPD(0, 4))
             + x86("call",   "rt_sg_member", (uint64_t)(uintptr_t)(void *)rt_sg_member)
             + x86("test",   "eax", "eax")
             + x86("je",     L(1))
             + x86("mov",    "eax", LFC(0))
             + x86("add",    "eax", (long)1)
             + x86("mov",    LFC(0), "eax")
             + x86("jmp",    L(0))
             + x86("def",    L(1))
             + x86("mov",    "eax", LFC(0))
             + x86("cmp",    "eax", "r14d")
             + x86_omega("je")
             + x86("mov",    LFC(0), "r14d")
             + x86("mov",    "r14d", "eax")
             + x86_gamma()
             + x86_beta()
             + x86("mov",    "r14d", LFC(0))
             + x86_omega();
    if (_.node && _.node->pat_static && _.op_sval)
        return x86("comment", "IR_MATCH_SPAN defer")
             + x86_alpha()
             + x86("lea",    "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval), (strtab_label(sp_nlb, sizeof sp_nlb, _.op_sval), sp_nlb))
             + x86("lea",    "rsi", LFC(0))
             + x86("lea",    "rdx", LFC(8))
             + x86("call",   "rt_pat_prim_str", (uint64_t)(uintptr_t)(void *)rt_pat_prim_str)
             + x86("test",   "rax", "rax")
             + x86_omega("js")
             + x86("mov",    "r8",  LFCQ(0))
             + x86("mov",    LFC(0), (long)0)
             + x86("def",    L(0))
             + x86("mov",    "eax", "r14d")
             + x86("add",    "eax", LFC(0))
             + x86("cmp",    "eax", "r15d")
             + x86("jge",    L(1))
             + x86("movsxd", "rcx", "eax")
             + x86("movzx",  "esi", "[r13+rcx]")
             + x86("mov",    "eax", LFC(8))
             + x86("mov",    "edx", (long)0)
             + x86("def",    L(2))
             + x86("cmp",    "edx", "eax")
             + x86("jge",    L(1))
             + x86("movzx",  "edi", "[r8+rdx]")
             + x86("cmp",    "esi", "edi")
             + x86("je",     L(3))
             + x86("add",    "edx", (long)1)
             + x86("jmp",    L(2))
             + x86("def",    L(3))
             + x86("add",    LFC(0), (long)1)
             + x86("jmp",    L(0))
             + x86("def",    L(1))
             + x86("mov",    "eax", LFC(0))
             + x86("test",   "eax", "eax")
             + x86_omega("jle")
             + x86("mov",    "edx", "r14d")
             + x86("mov",    LFC(4), "edx")
             + x86("add",    "edx", "eax")
             + x86("mov",    "r14d", "edx")
             + x86_gamma()
             + x86_beta()
             + x86("mov",    "r14d", LFC(4))
             + x86_omega();
    return x86("comment", "IR_MATCH_SPAN")
         + x86_alpha()
         + IF(sp_gi(),
              x86("mov",    LFC(0), (long)0)
            + sp_ndl_r8()
            + x86("def",    L(0))
            + x86("mov",    "eax", "r14d")
            + x86("add",    "eax", LFC(0))
            + x86("cmp",    "eax", "r15d")
            + x86("jge",    L(1))
            + x86("movsxd", "rcx", "eax")
            + x86("movzx",  "esi", "[r13+rcx]")
            + sp_len_eax()
            + x86("mov",    "edx", (long)0)
            + x86("def",    L(2))
            + x86("cmp",    "edx", "eax")
            + x86("jge",    L(1))
            + x86("movzx",  "edi", "[r8+rdx]")
            + x86("cmp",    "esi", "edi")
            + x86("je",     L(3))
            + x86("add",    "edx", (long)1)
            + x86("jmp",    L(2))
            + x86("def",    L(3))
            + x86("add",    LFC(0), (long)1)
            + x86("jmp",    L(0))
            + x86("def",    L(1))
            + x86("mov",    "eax", LFC(0))
            + x86("test",   "eax", "eax")
            + x86_omega("jle")
            + x86("mov",    "edx", "r14d")
            + x86("mov",    LFC(4), "edx")
            + x86("add",    "edx", "eax")
            + x86("mov",    "r14d", "edx"))
         + IF(sp_gu(),
              IF(sp_tablep(), x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c))
            + x86("movsxd", "rcx", "r14d")
            + x86("def",    L(0))
            + sp_char(0)
            + x86("jmp",    L(0))
            + x86("def",    L(1))
            + x86("cmp",    "ecx", "r14d")
            + x86_omega("jle")
            + x86("mov",    LFC(4), "r14d")
            + x86("mov",    "r14d", "ecx"))
         + x86_gamma()
         + x86_beta()
         + x86("mov",    "r14d", LFC(4))
         + x86_omega();
}

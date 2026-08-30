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
static char bx_dlb[24];
#define CSK() ((long) strlen(_.op_sval ? _.op_sval : ""))
#define BX_CHAINP() (_.op_sa < 0 && CSK() >= 1 && CSK() <= ZC_CSET_CHAIN_MAX)
#define BX_TABLEP() (_.op_sa < 0 && !BX_CHAINP())
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bx_memb(long f, long i) {
    return i >= CSK() ? std::string()
         : x86("cmp", "esi", (long)(unsigned char)_.op_sval[i])
         + x86("je",  L(f))
         + bx_memb(f, i + 1);
}
static std::string bx_char(long f, long e) {
    return x86("cmp",   "ecx", "r15d")
         + (e ? x86("jge", L(4)) : x86_omega("jge"))
         + x86("movzx", "esi", "[r13+rcx]")
         + (BX_CHAINP() ? bx_memb(f, 0)
          : (sn4_cset32()
             ? x86("bt",    "[rdi]", "esi")
             + x86("jc",    L(f))
             : x86("cmpb0", "[rdi+rsi]", "0")
             + x86("jnz",   L(f))))
         + x86("add",   "ecx", (long)1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bx_guts_scan(long t, long f, long e, long inr, long adv) {
    return x86("def",    L(t))
         + x86("mov",    "eax", LFC(4))
         + x86("add",    "eax", LFC(0))
         + x86("cmp",    "eax", "r15d")
         + (e ? x86("jge", L(4)) : x86_omega("jge"))
         + x86("movsxd", "rcx", "eax")
         + x86("movzx",  "esi", "[r13+rcx]")
         + x86("mov",    "eax", XSAD(4))
         + x86("mov",    "edx", (long)0)
         + x86("def",    L(inr))
         + x86("cmp",    "edx", "eax")
         + x86("jge",    L(adv))
         + x86("movzx",  "edi", "[r8+rdx]")
         + x86("cmp",    "esi", "edi")
         + x86("je",     L(f))
         + x86("add",    "edx", (long)1)
         + x86("jmp",    L(inr))
         + x86("def",    L(adv))
         + x86("add",    LFC(0), (long)1)
         + x86("jmp",    L(t));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_breakx() {
    x86_begin();
    if (_.node && _.node->pat_static && _.op_sval)
        return x86("comment", "IR_MATCH_BREAKX defer")
             + x86_alpha()
             + x86("mov",   LFC(4), "r14d")
             + x86("sub",   "rsp", (long)16)
             + x86("lea",   "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)_.op_sval, (strtab_label(bx_dlb, sizeof bx_dlb, _.op_sval), bx_dlb))
             + x86("lea",   "rsi", "qword ptr [rsp + 0]")
             + x86("lea",   "rdx", "qword ptr [rsp + 8]")
             + x86("call",  "rt_pat_prim_str", (uint64_t)(uintptr_t)(void *)rt_pat_prim_str)
             + x86("test",  "rax", "rax")
             + x86("mov",   "r8",  "qword ptr [rsp + 0]")
             + x86("mov",   "r9d", "dword ptr [rsp + 8]")
             + x86("lea",   "rsp", "qword ptr [rsp + 16]")
             + x86_omega("js")
             + x86("mov",   "rsi", "r8")
             + x86("mov",   "edx", "r9d")
             + x86("mov",   "edi", "r14d")
             + x86("call",  "rt_sg_scan_member", (uint64_t)(uintptr_t)(void *)rt_sg_scan_member)
             + x86("cmp",   "eax", "r15d")
             + x86_omega("jge")
             + x86("mov",   "r14d", "eax")
             + x86_gamma()
             + x86_beta()
             + x86("sub",   "rsp", (long)16)
             + x86("lea",   "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)_.op_sval, (strtab_label(bx_dlb, sizeof bx_dlb, _.op_sval), bx_dlb))
             + x86("lea",   "rsi", "qword ptr [rsp + 0]")
             + x86("lea",   "rdx", "qword ptr [rsp + 8]")
             + x86("call",  "rt_pat_prim_str", (uint64_t)(uintptr_t)(void *)rt_pat_prim_str)
             + x86("test",  "rax", "rax")
             + x86("mov",   "r8",  "qword ptr [rsp + 0]")
             + x86("mov",   "r9d", "dword ptr [rsp + 8]")
             + x86("lea",   "rsp", "qword ptr [rsp + 16]")
             + x86("js",    L(4))
             + x86("mov",   "rsi", "r8")
             + x86("mov",   "edx", "r9d")
             + x86("mov",   "edi", "r14d")
             + x86("add",   "edi", (long)1)
             + x86("call",  "rt_sg_scan_member", (uint64_t)(uintptr_t)(void *)rt_sg_scan_member)
             + x86("cmp",   "eax", "r15d")
             + x86("jge",   L(4))
             + x86("mov",   "r14d", "eax")
             + x86_gamma()
             + x86("def",   L(4))
             + x86("mov",   "r14d", LFC(4))
             + x86_omega();
    static char c[24];
    const void * ct = BX_TABLEP() ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    return (_.op_zres && _.op_sa >= 0)
         ? x86("comment", "IR_MATCH_BREAKX zd")
         + x86_alpha()
         + x86("mov",    LFC(4), "r14d")
         + x86("note",   ZOPN(0))
         + x86("mov",    "rsi", ZOPQ(0, 8))
         + x86("note",   ZOPN(0))
         + x86("mov",    "edx", ZOPD(0, 4))
         + x86("mov",    "edi", "r14d")
         + x86("call",   "rt_sg_scan_member", (uint64_t)(uintptr_t)(void *)rt_sg_scan_member)
         + x86("cmp",    "eax", "r15d")
         + x86_omega("jge")
         + x86("mov",    "r14d", "eax")
         + x86_gamma()
         + x86_beta()
         + x86("note",   ZOPN(0))
         + x86("mov",    "rsi", ZOPQ(0, 8))
         + x86("note",   ZOPN(0))
         + x86("mov",    "edx", ZOPD(0, 4))
         + x86("mov",    "edi", "r14d")
         + x86("add",    "edi", (long)1)
         + x86("call",   "rt_sg_scan_member", (uint64_t)(uintptr_t)(void *)rt_sg_scan_member)
         + x86("cmp",    "eax", "r15d")
         + x86("jge",    L(4))
         + x86("mov",    "r14d", "eax")
         + x86_gamma()
         + x86("def",    L(4))
         + x86("mov",    "r14d", LFC(4))
         + x86_omega()
         : x86("comment", "IR_MATCH_BREAKX")
         + x86_alpha()
         + IF(_.op_sa >= 0,
              x86("mov",    LFC(4), "r14d")
            + x86("mov",    LFC(0), (long)0)
            + x86("mov",    "r8",  XSAQ(8))
            + bx_guts_scan(0, 1, 0, 5, 7)
            + x86("def",    L(1))
            + x86("mov",    "eax", LFC(4))
            + x86("add",    "eax", LFC(0))
            + x86("mov",    "r14d", "eax"))
         + IF(_.op_sa < 0,
              IF(BX_TABLEP(), x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c))
            + x86("mov",    LFC(4), "r14d")
            + x86("movsxd", "rcx", "r14d")
            + x86("def",    L(0))
            + bx_char(1, 0)
            + x86("jmp",    L(0))
            + x86("def",    L(1))
            + x86("mov",    "r14d", "ecx"))
         + x86_gamma()
         + x86_beta()
         + IF(_.op_sa >= 0,
              x86("add",    LFC(0), (long)1)
            + x86("mov",    "r8",  XSAQ(8))
            + bx_guts_scan(2, 3, 1, 6, 8)
            + x86("def",    L(3))
            + x86("mov",    "eax", LFC(4))
            + x86("add",    "eax", LFC(0))
            + x86("mov",    "r14d", "eax"))
         + IF(_.op_sa < 0,
              IF(BX_TABLEP(), x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c))
            + x86("movsxd", "rcx", "r14d")
            + x86("add",    "ecx", (long)1)
            + x86("def",    L(2))
            + bx_char(3, 1)
            + x86("jmp",    L(2))
            + x86("def",    L(3))
            + x86("mov",    "r14d", "ecx"))
         + x86_gamma()
         + x86("def",    L(4))
         + x86("mov",    "r14d", LFC(4))
         + x86_omega();
}

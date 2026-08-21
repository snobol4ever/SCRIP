#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
#define LITN() ((long) strlen(_.op_sval ? _.op_sval : ""))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long LITD(long k) { uint32_t w; memcpy(&w, _.op_sval + k, 4); return (long)(int32_t)w; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string lit_chain(long n, long k) {
    return k >= n
             ? std::string()
         : (k + 8 <= n)
             ? x86("mov",    "rdx", LIDX(k))
             + x86("movabs", "rax", LITQ(k))
             + x86("cmp",    "rdx", "rax")
             + x86_omega("jne")
             + lit_chain(n, k + 8)
         : (k + 4 <= n)
             ? x86("mov",    "edx", LIDX(k))
             + x86("cmp",    "edx", LITD(k))
             + x86_omega("jne")
             + lit_chain(n, k + 4)
         : x86("movzx", "eax", LIDX(k))
             + x86("cmp",   "eax", (int)(unsigned char)_.op_sval[k])
             + x86_omega("jne")
             + lit_chain(n, k + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_lit_body() {
    static char b[24];
    return x86("comment", "IR_MATCH_LIT")
         + x86_alpha()
         + IF(LITN() > 0,
              x86("mov",    "eax", "r14d")
            + x86("add",    "eax", LITN())
            + x86("cmp",    "eax", "r15d")
            + x86_omega("jg")
            + x86("movsxd", "rcx", "r14d"))
         + IF(LITN() >= 1 && LITN() <= 64, lit_chain(LITN(), 0))
         + IF(LITN() > 64,
              x86("lea",    "rdi", "[r13+rcx]")
            + x86("lea",    "rsi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
            + x86("mov",    "edx", LITN())
            + x86("call",   "memcmp", (uint64_t)(uintptr_t)(void *)(int (*)(const void *, const void *, size_t)) memcmp)
            + x86("test",   "eax", "eax")
            + x86_omega("jne"))
         + IF(LITN() > 0, x86("add", "r14d", LITN()))
         + x86_gamma()
         + x86_beta()
         + IF(LITN() > 0, x86("sub", "r14d", LITN()))
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_lit() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres) return bb_match_lit_body();
    return bb_match_lit_body();
}

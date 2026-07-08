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
std::string bb_match_lit() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    const char * s = _.op_sval ? _.op_sval : "";
    long n = (long) strlen(s);
    return x86("comment", "IR_MATCH_LIT")
         + x86_alpha()
         + x86("mov",    "eax", "r14d")
         + x86("add",    "eax", n)
         + x86("cmp",    "eax", "r15d")
         + x86_omega("jg")
         + x86("movsxd", "rcx", "r14d")
         + x86("lea",    "rdi", "[r13+rcx]")
         + x86("lea",    "rsi", "[rip + __]", (uint64_t)(uintptr_t)(const void *) s, (strtab_label(b, sizeof b, s), b))
         + x86("mov",    "edx", n)
         + x86("sub",    "rsp", (long)8)
         + x86("call",   "memcmp", (uint64_t)(uintptr_t)(void *)(int (*)(const void *, const void *, size_t)) memcmp)
         + x86("add",    "rsp", (long)8)
         + x86("test",   "eax", "eax")
         + x86_omega("jne")
         + x86("add",    "r14d", n)
         + x86_gamma()
         + x86_beta()
         + x86("sub",    "r14d", n)
         + x86_omega();
}

#include <string>
#include <cstring>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_lit() {
    const char *s = _.op_sval ? _.op_sval : "";
    const char *lbl = emit_intern_str(s);
    static char b[24];
    if (!lbl) { strtab_label(b, sizeof b, s); lbl = b; }
    return x86("comment", "IR_LIT")
         + x86_alpha()
         + x86("mov", "eax", "r14d")
         + x86("add", "eax", (long)strlen(s))
         + x86("cmp", "eax", "r15d")
         + x86_omega("jg")
         + x86("movsxd", "rcx", "r14d")
         + x86("lea", "rdi", "[r13 + rcx]")
         + x86("lea", "rsi", "[rip + __]", (uint64_t)(uintptr_t)s, lbl)
         + x86("mov", "rdx", (long)strlen(s))
         + x86("call", "memcmp", (uint64_t)(uintptr_t)memcmp)
         + x86("test", "eax", "eax")
         + x86_omega("jne")
         + x86("add", "r14d", (long)strlen(s))
         + x86_gamma()
         + x86_beta()
         + x86("sub", "r14d", (long)strlen(s))
         + x86_omega();
}

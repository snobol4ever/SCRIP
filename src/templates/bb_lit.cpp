#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_lit() {
    if (!PLATFORM_X86) return std::string();
    const char *s = _.op_sval ? _.op_sval : "";
    long n = (long)strlen(s);
    const char *lbl = emit_intern_str(s);
    static char b[24];
    if (!lbl) { strtab_label(b, sizeof b, s); lbl = b; }
    uint64_t sa = (uint64_t)(uintptr_t)s;
    uint64_t mc = (uint64_t)(uintptr_t)memcmp;
    return x86("comment", "IR_LIT")
         + x86("label",   _.lbl_α)
         + x86("mov", "eax", "r14d")
         + x86("add", "eax", n)
         + x86("cmp", "eax", "r15d")
         + x86("jg", "ω")
         + x86("movsxd", "rcx", "r14d")
         + x86("lea", "rdi", "[r13 + rcx]")
         + x86("lea", "rsi", "[rip + __]", sa, lbl)
         + x86("mov", "rdx", n)
         + x86("sub", "rsp", (long)8)
         + x86("call", "memcmp", mc)
         + x86("add", "rsp", (long)8)
         + x86("test", "eax", "eax")
         + x86("jne", "ω")
         + x86("add", "r14d", n)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("sub", "r14d", n)
         + x86("jmp", "ω");
}

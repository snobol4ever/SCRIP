#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
void *rt_cs_new(const char *chars);
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_notany() {
    if (!PLATFORM_X86) return std::string();
    const char *cs = _.op_sval ? _.op_sval : "";
    const char *lbl = emit_intern_str(cs);
    static char b[24];
    if (!lbl) { strtab_label(b, sizeof b, cs); lbl = b; }
    uint64_t ca = (uint64_t)(uintptr_t)(const void *)cs;
    uint64_t sf; { const char *(*fp)(const char *, int) = strchr; sf = (uint64_t)(uintptr_t)(void *)fp; }
    std::string test = (strlen(cs) == 1)
        ? ( x86("cmp",  "sil", (long)(unsigned char)cs[0])
          + x86("je",   "ω") )
        : ( x86("lea",  "rdi", "[rip + __]", ca, lbl)
          + x86("sub",  "rsp", (long)8)
          + x86("call", "strchr", sf)
          + x86("add",  "rsp", (long)8)
          + x86("test", "rax", "rax")
          + x86("jne",  "ω") );
    return x86("comment", "IR_MATCH_NOTANY")
         + x86("label",   _.lbl_α)
         + x86("mov",    "eax", "r14d")
         + x86("cmp",    "eax", "r15d")
         + x86("jge",    "ω")
         + x86("movsxd", "rcx", "r14d")
         + x86("movzx",  "esi", "[r13+rcx]")
         + test
         + x86("add",    "r14d", (long)1)
         + x86("jmp",    "γ")
         + x86("def",    "β")
         + x86("sub",    "r14d", (long)1)
         + x86("jmp",    "ω");
}

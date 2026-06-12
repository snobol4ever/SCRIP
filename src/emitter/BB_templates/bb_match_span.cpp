#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
int bb_slot_claim(int bytes);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_span() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    const char *cc = _.op_sval ? _.op_sval : "";
    const char *lbl = emit_intern_str(cc);
    static char b[24];
    if (!lbl) { strtab_label(b, sizeof b, cc); lbl = b; }
    uint64_t ca = (uint64_t)(uintptr_t)(const void *)cc;
    uint64_t sf; { const char *(*fp)(const char *, int) = strchr; sf = (uint64_t)(uintptr_t)(void *)fp; }
    int z = _.x86_scratch_off, zo = _.x86_scratch_off + 4;
    return x86("comment", "IR_MATCH_SPAN")
         + x86("label",   _.lbl_α)
         + x86("mov",    FR(z), (long)0)
         + x86("def",    L(0))
         + x86("mov",    "eax", "r14d")
         + x86("add",    "eax", FR(z))
         + x86("cmp",    "eax", "r15d")
         + x86("jge",    L(1))
         + x86("movsxd", "rcx", "eax")
         + x86("movzx",  "esi", "[r13+rcx]")
         + x86("lea",    "rdi", "[rip + __]", ca, lbl)
         + x86("sub",    "rsp", (long)8)
         + x86("call",   "strchr", sf)
         + x86("add",    "rsp", (long)8)
         + x86("test",   "rax", "rax")
         + x86("je",     L(1))
         + x86("add",    FR(z), (long)1)
         + x86("jmp",    L(0))
         + x86("def",    L(1))
         + x86("mov",    "eax", FR(z))
         + x86("test",   "eax", "eax")
         + x86("jle",    "ω")
         + x86("mov",    "edx", "r14d")
         + x86("mov",    FR(zo), "edx")
         + x86("add",    "edx", "eax")
         + x86("mov",    "r14d", "edx")
         + x86("jmp",    "γ")
         + x86("def",    "β")
         + x86("mov",    "eax", FR(z))
         + x86("sub",    "eax", (long)1)
         + x86("cmp",    "eax", (long)1)
         + x86("jl",     "ω")
         + x86("mov",    FR(z), "eax")
         + x86("mov",    "edx", FR(zo))
         + x86("add",    "edx", "eax")
         + x86("mov",    "r14d", "edx")
         + x86("jmp",    "γ");
}

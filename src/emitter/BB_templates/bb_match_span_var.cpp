#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
const char * rt_nv_cstr(const char * name);
int bb_slot_claim(int bytes);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_span_var() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    const char *vn = _.op_sval ? _.op_sval : "";
    const char *lbl = emit_intern_str(vn);
    static char b[24];
    if (!lbl) { strtab_label(b, sizeof b, vn); lbl = b; }
    uint64_t na = (uint64_t)(uintptr_t)(const void *)vn;
    uint64_t nv; { const char *(*fp)(const char *) = rt_nv_cstr; nv = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t sf; { const char *(*fp)(const char *, int) = strchr; sf = (uint64_t)(uintptr_t)(void *)fp; }
    int cs = _.x86_scratch_off, z = _.x86_scratch_off + 8, zo = _.x86_scratch_off + 12;
    return x86("comment", "IR_MATCH_SPAN_VAR")
         + x86("label",   _.lbl_α)
         + x86("lea",    "rdi", "[rip + __]", na, lbl)
         + x86("sub",    "rsp", (long)8)
         + x86("call",   "rt_nv_cstr", nv)
         + x86("add",    "rsp", (long)8)
         + x86("mov",    FRQ(cs), "rax")
         + x86("mov",    FR(z), (long)0)
         + x86("def",    L(0))
         + x86("mov",    "eax", "r14d")
         + x86("add",    "eax", FR(z))
         + x86("cmp",    "eax", "r15d")
         + x86("jge",    L(1))
         + x86("movsxd", "rcx", "eax")
         + x86("movzx",  "esi", "[r13+rcx]")
         + x86("mov",    "rdi", FRQ(cs))
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

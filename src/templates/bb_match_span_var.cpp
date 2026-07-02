#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
const char * rt_nv_cstr(const char * name);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_span_var() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    return x86("comment", "IR_MATCH_SPAN_VAR")
         + x86("label",   _.lbl_α)
         + x86("lea",    "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
         + x86("sub",    "rsp", (long)8)
         + x86("call",   "rt_nv_cstr", (uint64_t)(uintptr_t)(void *)(const char *(*)(const char *))rt_nv_cstr)
         + x86("add",    "rsp", (long)8)
         + x86("mov",    FRQ(_.x86_scratch_off), "rax")
         + x86("mov",    FR(_.x86_scratch_off + 8), (long)0)
         + x86("def",    L(0))
         + x86("mov",    "eax", "r14d")
         + x86("add",    "eax", FR(_.x86_scratch_off + 8))
         + x86("cmp",    "eax", "r15d")
         + x86("jge",    L(1))
         + x86("movsxd", "rcx", "eax")
         + x86("movzx",  "esi", "[r13+rcx]")
         + x86("mov",    "rdi", FRQ(_.x86_scratch_off))
         + x86("sub",    "rsp", (long)8)
         + x86("call",   "strchr", (uint64_t)(uintptr_t)(void *)(const char *(*)(const char *, int))strchr)
         + x86("add",    "rsp", (long)8)
         + x86("test",   "rax", "rax")
         + x86("je",     L(1))
         + x86("add",    FR(_.x86_scratch_off + 8), (long)1)
         + x86("jmp",    L(0))
         + x86("def",    L(1))
         + x86("mov",    "eax", FR(_.x86_scratch_off + 8))
         + x86("test",   "eax", "eax")
         + x86("jle",    "ω")
         + x86("mov",    "edx", "r14d")
         + x86("mov",    FR(_.x86_scratch_off + 12), "edx")
         + x86("add",    "edx", "eax")
         + x86("mov",    "r14d", "edx")
         + x86("jmp",    "γ")
         + x86("def",    "β")
         + x86("mov",    "r14d", FR(_.x86_scratch_off + 12))
         + x86("jmp",    "ω");
}

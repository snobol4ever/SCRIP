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
std::string bb_match_break() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    return x86("comment", "IR_MATCH_BREAK")
         + x86("label",   _.lbl_α)
         + x86("mov",    FR(_.x86_scratch_off), (long)0)
         + x86("def",    L(0))
         + x86("mov",    "eax", "r14d")
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("cmp",    "eax", "r15d")
         + x86("jge",    "ω")
         + x86("movsxd", "rcx", "eax")
         + x86("movzx",  "esi", "[r13+rcx]")
         + x86("lea",    "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
         + x86("sub",    "rsp", (long)8)
         + x86("call",   "strchr", (uint64_t)(uintptr_t)(void *)(const char *(*)(const char *, int))strchr)
         + x86("add",    "rsp", (long)8)
         + x86("test",   "rax", "rax")
         + x86("jnz",    L(1))
         + x86("add",    FR(_.x86_scratch_off), (long)1)
         + x86("jmp",    L(0))
         + x86("def",    L(1))
         + x86("mov",    "eax", "r14d")
         + x86("add",    "eax", FR(_.x86_scratch_off))
         + x86("mov",    "r14d", "eax")
         + x86("jmp",    "γ")
         + x86("def",    "β")
         + x86("mov",    "eax", "r14d")
         + x86("sub",    "eax", FR(_.x86_scratch_off))
         + x86("mov",    "r14d", "eax")
         + x86("jmp",    "ω");
}

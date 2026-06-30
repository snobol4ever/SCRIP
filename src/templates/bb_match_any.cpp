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
std::string bb_match_any() {
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    return x86("comment", "IR_MATCH_ANY")
         + x86("label",   _.lbl_α)
         + x86("mov",    "eax", "r14d")
         + x86("cmp",    "eax", "r15d")
         + x86("jge",    "ω")
         + x86("movsxd", "rcx", "r14d")
         + x86("movzx",  "esi", "[r13+rcx]")
         + ( strlen(_.op_sval ? _.op_sval : "") == 1
           ? ( x86("cmp",  "sil", (long)(unsigned char)(_.op_sval ? _.op_sval : "")[0])
             + x86("jne",  "ω") )
           : ( x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86("sub",  "rsp", (long)8)
             + x86("call", "strchr", (uint64_t)(uintptr_t)(void *)(const char *(*)(const char *, int))strchr)
             + x86("add",  "rsp", (long)8)
             + x86("test", "rax", "rax")
             + x86("je",   "ω") ) )
         + x86("add",    "r14d", (long)1)
         + x86("jmp",    "γ")
         + x86("def",    "β")
         + x86("sub",    "r14d", (long)1)
         + x86("jmp",    "ω");
}

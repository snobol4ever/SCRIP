#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
const char * rt_nv_cstr(const char * name);
long rt_nv_slen(const char * name);
}
#include "x86_asm.h"
extern "C" long rt_sg_scan_member(void);
extern "C" long rt_sg_scan_nonmember(void);
extern "C" long rt_sg_member(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_span_var() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    return x86("comment", "IR_MATCH_SPAN_VAR")
         + x86_alpha()
         + x86("lea",    "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
         + x86("call",   "rt_nv_cstr", (uint64_t)(uintptr_t)(void *)(const char *(*)(const char *))rt_nv_cstr)
         + x86("mov",    FRQ(_.x86_scratch_off), "rax")
         + x86("lea",    "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
         + x86("call",   "rt_nv_slen", (uint64_t)(uintptr_t)(void *)rt_nv_slen)
         + x86("mov",    FR(_.x86_scratch_off + 16), "eax")
         + x86("mov",    FR(_.x86_scratch_off + 8), (long)0)
         + IF(ZC_SPAN_GUTS == ZC_SPAN_GUTS_INLINE,
              x86("mov",    "r8",  FRQ(_.x86_scratch_off))
            + x86("def",    L(0))
            + x86("mov",    "eax", "r14d")
            + x86("add",    "eax", FR(_.x86_scratch_off + 8))
            + x86("cmp",    "eax", "r15d")
            + x86("jge",    L(1))
            + x86("movsxd", "rcx", "eax")
            + x86("movzx",  "esi", "[r13+rcx]")
            + x86("mov",    "eax", FR(_.x86_scratch_off + 16))
            + x86("mov",    "edx", (long)0)
            + x86("def",    L(2))
            + x86("cmp",    "edx", "eax")
            + x86("jge",    L(1))
            + x86("movzx",  "edi", "[r8+rdx]")
            + x86("cmp",    "esi", "edi")
            + x86("je",     L(3))
            + x86("add",    "edx", (long)1)
            + x86("jmp",    L(2))
            + x86("def",    L(3))
            + x86("add",    FR(_.x86_scratch_off + 8), (long)1)
            + x86("jmp",    L(0)))
         + IF(ZC_SPAN_GUTS == ZC_SPAN_GUTS_CALL,
              x86("mov",    "edi", "r14d")
            + x86("mov",    "rsi", FRQ(_.x86_scratch_off))
            + x86("mov",    "edx", FR(_.x86_scratch_off + 16))
            + x86("call",   "rt_sg_scan_nonmember", (uint64_t)(uintptr_t)(void *)rt_sg_scan_nonmember)
            + x86("mov",    "ecx", "eax")
            + x86("mov",    "eax", "r14d")
            + x86("def",    L(4))
            + x86("cmp",    "eax", "ecx")
            + x86("jge",    L(1))
            + x86("add",    FR(_.x86_scratch_off + 8), (long)1)
            + x86("add",    "eax", (long)1)
            + x86("jmp",    L(4)))
         + x86("def",    L(1))
         + x86("mov",    "eax", FR(_.x86_scratch_off + 8))
         + x86("test",   "eax", "eax")
         + x86_omega("jle")
         + x86("mov",    "edx", "r14d")
         + x86("mov",    FR(_.x86_scratch_off + 12), "edx")
         + x86("add",    "edx", "eax")
         + x86("mov",    "r14d", "edx")
         + x86_gamma()
         + x86_beta()
         + x86("mov",    "r14d", FR(_.x86_scratch_off + 12))
         + x86_omega();
}

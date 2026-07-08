#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" int   rt_defer_match    (const char *varname, int ival_flag, int cur_delta);
extern "C" void *rt_defer_get_pat_fn(const char *varname, int ival_flag);
extern "C" void *rt_zls_alloc      (long bytes);
extern "C" void  rt_zls_release    (void *fb);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_defer() {
    if (!PLATFORM_X86) return std::string();
    bool inl = (_.bb_child_fn != (void *)0) || (_.bb_child_lbl && _.bb_child_lbl[0]);
    static char b[24];
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    std::string head = inl
        ? ( x86("comment", "IR_MATCH_DEFER inlined frozen head (FZ-5b)")
          + x86("def",     "α")
          + x86("lea",  "rax", "[rip + __]", (uint64_t)(uintptr_t)_.bb_child_fn, _.bb_child_lbl ? _.bb_child_lbl : "") )
        : ( x86("comment", "IR_MATCH_DEFER")
          + x86("def",     "α")
          + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
          + x86("xor",  "esi", "esi")
          + x86("push", "rbp")
          + x86("mov",  "rbp", "rsp")
          + x86("and",  "rsp", -16L)
          + x86("call", "rt_defer_get_pat_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(const char *, int))rt_defer_get_pat_fn)
          + x86("mov",  "rsp", "rbp")
          + x86("pop",  "rbp") );
    return head
         + x86("test", "rax", "rax")
         + x86("jz",   "L0")
         + x86("push", "rax")
         + x86("push", "rbp")
         + x86("mov",  "rbp", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("mov",  "rdi", (long)65536)
         + x86("call", "rt_zls_alloc", (uint64_t)(uintptr_t)(void *)(void *(*)(long))rt_zls_alloc)
         + x86("mov",  "rsp", "rbp")
         + x86("pop",  "rbp")
         + x86("pop",  "rcx")
         + x86("push", "rax")
         + x86("mov",  "rdi", "rax")
         + x86("xor",  "esi", "esi")
         + x86("call", "rcx")
         + x86("pop",  "rdi")
         + x86("cmp",  "eax", (long)1)
         + x86("je",   "L1")
         + x86("push", "rbp")
         + x86("mov",  "rbp", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "rt_zls_release", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls_release)
         + x86("mov",  "rsp", "rbp")
         + x86("pop",  "rbp")
         + x86("jmp",  "ω")
         + x86("def",  "L1")
         + x86("jmp",  "γ")
         + x86("def",  "L0")
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
         + x86("xor",  "esi", "esi")
         + x86("mov",  "edx", "r14d")
         + x86("push", "rbp")
         + x86("mov",  "rbp", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "rt_defer_match", (uint64_t)(uintptr_t)(void *)(int (*)(const char *, int, int))rt_defer_match)
         + x86("mov",  "rsp", "rbp")
         + x86("pop",  "rbp")
         + x86("test", "eax", "eax")
         + x86("js",   "ω")
         + x86("mov",  "r14d", "eax")
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}

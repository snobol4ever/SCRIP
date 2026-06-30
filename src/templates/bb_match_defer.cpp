#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
extern "C" int   rt_defer_match    (const char *varname, int ival_flag, int cur_delta);
extern "C" void *rt_defer_get_pat_fn(const char *varname, int ival_flag);
extern "C" void *rt_frame          (void);
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* FZ-5b: when the match site references a once-assigned invariant pattern variable (proven by FZ-5a, staged by the IR_MATCH_DEFER flat_drive into bb_child_fn/bb_child_lbl via child_cache), bake the sealed matcher head directly into rax with the RIPSEAL lea form and SKIP the per-match rt_defer_get_pat_fn fetch (runs 500k× in string_pattern/pattern_bt). The sealed head is always non-NULL so the tail's jz-L0 falls through to the head call; the L0 rt_defer_match fallback stays for the non-inlined fetch path. */
std::string bb_match_defer() {
    if (!PLATFORM_X86) return std::string();
    bool inl = (_.bb_child_fn != (void *)0) || (_.bb_child_lbl && _.bb_child_lbl[0]);
    static char b[24];
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    std::string head = inl
        ? ( x86("comment", "IR_MATCH_DEFER inlined frozen head (FZ-5b)")
          + x86("label", _.lbl_α)
          + x86("lea",  "rax", "[rip + __]", (uint64_t)(uintptr_t)_.bb_child_fn, _.bb_child_lbl ? _.bb_child_lbl : "") )
        : ( x86("comment", "IR_MATCH_DEFER")
          + x86("label", _.lbl_α)
          + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
          + x86("mov",  "esi", (long)(int)(int64_t)_.op_ival)
          + x86("push", "rbx")
          + x86("mov",  "rbx", "rsp")
          + x86("and",  "rsp", -16L)
          + x86("call", "rt_defer_get_pat_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(const char *, int))rt_defer_get_pat_fn)
          + x86("mov",  "rsp", "rbx")
          + x86("pop",  "rbx") );
    return head
         + x86("test", "rax", "rax")
         + x86("jz",   "L0")
         + x86("push", "rax")
         + x86("push", "rbx")
         + x86("mov",  "rbx", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "rt_frame", (uint64_t)(uintptr_t)(void *)(void *(*)())rt_frame)
         + x86("mov",  "rsp", "rbx")
         + x86("pop",  "rbx")
         + x86("pop",  "rcx")
         + x86("mov",  "rdi", "rax")
         + x86("xor",  "esi", "esi")
         + x86("call", "rcx")
         + x86("cmp",  "eax", (long)1)
         + x86("jne",  "ω")
         + x86("jmp",  "γ")
         + x86("def",  "L0")
         + x86("mov",  "edx", "r14d")
         + x86("push", "rbx")
         + x86("mov",  "rbx", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "rt_defer_match", (uint64_t)(uintptr_t)(void *)(int (*)(const char *, int, int))rt_defer_match)
         + x86("mov",  "rsp", "rbx")
         + x86("pop",  "rbx")
         + x86("test", "eax", "eax")
         + x86("js",   "ω")
         + x86("mov",  "r14d", "eax")
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}

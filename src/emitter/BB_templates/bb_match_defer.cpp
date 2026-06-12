#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
extern "C" int rt_defer_match(const char *varname, int ival_flag, int cur_delta);
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * dvar_chars()  { return _.op_sval ? _.op_sval : ""; }
static inline const char * dvar_label()  { const char * l = emit_intern_str(dvar_chars()); if (l) return l;
                                           static char b[24]; strtab_label(b, sizeof b, dvar_chars()); return b; }
static inline uint64_t     dvar_addr()   { return (uint64_t)(uintptr_t)(const void *)dvar_chars(); }
static inline int          defer_ival()  { return (int)(int64_t)_.op_ival; }
static inline uint64_t     defer_fn()    { int (*fp)(const char *, int, int) = rt_defer_match;
                                           return (uint64_t)(uintptr_t)(void *)fp; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_defer() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX DEFER(" + std::string(defer_ival() ? "*" : "") + std::string(dvar_chars()) + ")  [REG-3 δ=r14, x86() self-encoding]"))
             + x86("lea",  "rdi", "[rip + __]", dvar_addr(), dvar_label())
             + x86("mov",  "esi", (long)defer_ival())
             + x86("mov",  "edx", "r14d")
             + x86("push", "rbx")
             + x86("mov", "rbx", "rsp")
             + x86("and", "rsp", -16L)
             + x86("call", "rt_defer_match", defer_fn())
             + x86("mov", "rsp", "rbx")
             + x86("pop",  "rbx")
             + x86("test", "eax", "eax")
             + x86("js",   "ω")
             + x86("mov",  "r14d", "eax")
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    }
    return std::string();
}
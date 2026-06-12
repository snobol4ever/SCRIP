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
static inline const char * var_name()   { return _.op_sval ? _.op_sval : ""; }
static inline const char * name_label() { const char * l = emit_intern_str(var_name()); if (l) return l;
                                          static char b[24]; strtab_label(b, sizeof b, var_name()); return b; }
static inline uint64_t     name_addr()  { return (uint64_t)(uintptr_t)(const void *)var_name(); }
static inline uint64_t     nv_ptr()     { const char *(*fp)(const char *) = rt_nv_cstr; return (uint64_t)(uintptr_t)(void *)fp; }
static inline uint64_t     strchr_ptr() { const char *(*fp)(const char *, int) = strchr; return (uint64_t)(uintptr_t)(void *)fp; }
static inline int          csoff() { return _.x86_scratch_off; }
static inline int          zoff()  { return _.x86_scratch_off + 8; }
static inline int          zooff() { return _.x86_scratch_off + 12; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_span_var() {
    x86_begin();
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX SPAN(var)  [REG-4 Σ=r13 δ=r14 Δ=r15, ζ-frame cs/z/zo, cset via rt_nv_cstr at match time]"))
             + x86("lea",    "rdi", "[rip + __]", name_addr(), name_label())
             + x86("sub",    "rsp", (long)8)
             + x86("call",   "rt_nv_cstr", nv_ptr())
             + x86("add",    "rsp", (long)8)
             + x86_frame_store64(csoff(), "rax")
             + x86("mov",    FR(zoff()), (long)0)
             + x86("def",    L(0))
             + x86("mov",    "eax", "r14d")
             + x86("add",    "eax", FR(zoff()))
             + x86("cmp",    "eax", "r15d")
             + x86("jge",    L(1))
             + x86("movsxd", "rcx", "eax")
             + x86("movzx",  "esi", "[r13+rcx]")
             + x86_frame_load64("rdi", csoff())
             + x86("sub",    "rsp", (long)8)
             + x86("call",   "strchr", strchr_ptr())
             + x86("add",    "rsp", (long)8)
             + x86("test",   "rax", "rax")
             + x86("je",     L(1))
             + x86("add",    FR(zoff()), (long)1)
             + x86("jmp",    L(0))
             + x86("def",    L(1))
             + x86("mov",    "eax", FR(zoff()))
             + x86("test",   "eax", "eax")
             + x86("jle",    "ω")
             + x86("mov",    "edx", "r14d")
             + x86("mov",    FR(zooff()), "edx")
             + x86("add",    "edx", "eax")
             + x86("mov",    "r14d", "edx")
             + x86("jmp",    "γ")
             + x86("def",    "β")
             + x86("mov",    "eax", FR(zoff()))
             + x86("sub",    "eax", (long)1)
             + x86("cmp",    "eax", (long)1)
             + x86("jl",     "ω")
             + x86("mov",    FR(zoff()), "eax")
             + x86("mov",    "edx", FR(zooff()))
             + x86("add",    "edx", "eax")
             + x86("mov",    "r14d", "edx")
             + x86("jmp",    "γ");
    }
    return std::string();
}
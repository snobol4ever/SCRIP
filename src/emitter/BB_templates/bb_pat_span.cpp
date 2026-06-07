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
static inline const char * cset_chars() { return _.op_sval ? _.op_sval : ""; }
static inline const char * cset_label() { const char * l = emit_intern_str(cset_chars()); if (l) return l;
                                          static char b[24]; strtab_label(b, sizeof b, cset_chars()); return b; }
static inline uint64_t     cset_addr()  { return (uint64_t)(uintptr_t)(const void *)cset_chars(); }
static inline uint64_t     strchr_ptr() { const char *(*fp)(const char *, int) = strchr; return (uint64_t)(uintptr_t)(void *)fp; }
static inline int          zoff()  { return _.x86_scratch_off; }
static inline int          zooff() { return _.x86_scratch_off + 4; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_span_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX SPAN()  [REG-4 Σ=r13 δ=r14 Δ=r15, ζ-frame z/zo, x86() self-encoding]"))
             + x86("mov",    FR(zoff()), (long)0)
             + x86("def",    L(0))
             + x86("mov",    "eax", "r14d")
             + x86("add",    "eax", FR(zoff()))
             + x86("cmp",    "eax", "r15d")
             + x86("jge",    L(1))
             + x86("movsxd", "rcx", "eax")
             + x86("movzx",  "esi", "[r13+rcx]")
             + x86("lea",    "rdi", "[rip + __]", cset_addr(), cset_label())
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
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_span(void) {
    x86_begin();
    _.x86_scratch_off = bb_slot_claim(16);
    bb_emit_x86(bb_pat_span_str());
}

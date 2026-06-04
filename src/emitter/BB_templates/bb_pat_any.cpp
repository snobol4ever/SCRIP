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
static inline const char * cset_chars() { return _.op_sval ? _.op_sval : ""; }
static inline const char * cset_label() { const char * l = emit_intern_str(cset_chars()); if (l) return l;
                                          static char b[24]; strtab_label(b, sizeof b, cset_chars()); return b; }
static inline uint64_t     cset_addr()  { return (uint64_t)(uintptr_t)(const void *)cset_chars(); }
static inline uint64_t     strchr_ptr() { const char *(*fp)(const char *, int) = strchr; return (uint64_t)(uintptr_t)(void *)fp; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_any_str() {
    int nid = _.nid; (void)nid;
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX ANY()  [REG-2 Σ=r13 δ=r14 Δ=r15, x86() self-encoding]"))
             + x86("mov",    "eax", "r14d")
             + x86("cmp",    "eax", "r15d")
             + x86("jge",    PORT_OMEGA)
             + x86("movsxd", "rcx", "r14d")
             + x86("movzx",  "esi", "[r13+rcx]")
             + x86("lea",    "rdi", "[rip + __]", cset_addr(), cset_label())
             + x86("push",   "r10")
             + x86("call",   "strchr", strchr_ptr())
             + x86("pop",    "r10")
             + x86("test",   "rax", "rax")
             + x86("je",  PORT_OMEGA)
             + x86("add",    "r14d", (long)1)
             + x86("jmp",    PORT_GAMMA)
             + x86("def",    PORT_BETA)
             + x86("sub",    "r14d", (long)1)
             + x86("jmp",    PORT_OMEGA);
    }
/*--------------------------------------------------------------------------------------------------------------------*/
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_any(void) { bb_emit_x86(bb_pat_any_str()); }

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
static inline const char * cs()       { return _.op_sval ? _.op_sval : ""; }
static inline const char * cs_lbl()   { const char * l = emit_intern_str(cs()); if (l) return l;
                                        static char b[24]; strtab_label(b, sizeof b, cs()); return b; }
static inline uint64_t     cs_addr()  { return (uint64_t)(uintptr_t)(const void *)cs(); }
static inline long         cs_byte()  { return (long)(unsigned char)cs()[0]; }
static inline uint64_t     strchr_fn(){ const char *(*fp)(const char *, int) = strchr; return (uint64_t)(uintptr_t)(void *)fp; }
static inline std::string  cs_test_notany() {
    if (strlen(cs()) == 1)
        return x86("cmp",  "sil", cs_byte())
             + x86("je",   "\xCF\x89");
    return x86("lea",  "rdi", "[rip + __]", cs_addr(), cs_lbl())
         + x86("sub",  "rsp", (long)8)
         + x86("call", "strchr", strchr_fn())
         + x86("add",  "rsp", (long)8)
         + x86("test", "rax", "rax")
         + x86("jne",  "\xCF\x89");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_notany_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", "BOX NOTANY — match one char not in cset [Σ=r13 δ=r14 Δ=r15]"))
             + x86("mov",    "eax", "r14d")
             + x86("cmp",    "eax", "r15d")
             + x86("jge",    "\xCF\x89")
             + x86("movsxd", "rcx", "r14d")
             + x86("movzx",  "esi", "[r13+rcx]")
             + cs_test_notany()
             + x86("add",    "r14d", (long)1)
             + x86("jmp",    "\xCE\xB3")
             + x86("def",    "\xCE\xB2")
             + x86("sub",    "r14d", (long)1)
             + x86("jmp",    "\xCF\x89");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_notany(void) { bb_emit_x86(bb_pat_notany_str()); }

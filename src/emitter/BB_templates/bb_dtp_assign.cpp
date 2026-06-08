#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
}
extern "C" void rt_gvar_assign_pat(const char * name, void * head);
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern uint8_t * g_pat_pool_cur;
static int g_da_seq = 0;
static inline void         da_bump()  { g_da_seq++; }
static inline std::string  dal(const char * sfx) { return std::string(".Lda") + std::to_string(g_da_seq) + sfx; }
static inline std::string  da_sa()    { return std::to_string((long)_.op_sa); }
static inline const char * da_name()  { return _.op_sval ? _.op_sval : ""; }
static inline const char * da_label() { const char * l = emit_intern_str(da_name()); if (l) return l;
                                        static char b[24]; strtab_label(b, sizeof b, da_name()); return b; }
static inline uint64_t     da_addr()  { return (uint64_t)(uintptr_t)(const void *)da_name(); }
static inline uint64_t     da_fn()    { void (*fp)(const char *, void *) = rt_gvar_assign_pat;
                                        return (uint64_t)(uintptr_t)(void *)fp; }
static inline std::string  da_proto() {
    return x86("label", dal("_s"))
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("label", dal("_g"))
         + x86("ins2", "jmp", "qword ptr [rip + " + dal("_s") + " + 8]")
         + x86("label", dal("_w"))
         + x86("ins2", "jmp", "qword ptr [rip + " + dal("_s") + " + 16]")
         + x86("label", dal("_e"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_dtp_assign_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", std::string("BOX DTP_ASSIGN('") + da_name() + "')  [HEAD ζ=r12 <- frag@" + da_sa() + "]"))
             + x86("ins2", "mov", "rax, qword ptr [rip + g_pat_pool_cur]")
             + x86("mov", "rdi", "rax")
             + x86("ins2", "mov", "ecx, " + dal("_e") + " - " + dal("_s"))
             + x86("ins2", "lea", "rsi, [rip + " + dal("_s") + "]")
             + x86("ins1", "rep movsb")
             + x86("ins2", "mov", "qword ptr [rip + g_pat_pool_cur], rdi")
             + x86("ins2", "mov", "rcx, [r12 + " + da_sa() + "]")
             + x86("ins2", "mov", "[rax], rcx")
             + x86("ins2", "lea", "rcx, [rax + " + dal("_g") + " - " + dal("_s") + "]")
             + x86("ins2", "mov", "rdx, [r12 + " + da_sa() + " + 8]")
             + x86("ins2", "mov", "[rdx], rcx")
             + x86("ins2", "lea", "rcx, [rax + " + dal("_w") + " - " + dal("_s") + "]")
             + x86("ins2", "mov", "rdx, [r12 + " + da_sa() + " + 16]")
             + x86("ins2", "mov", "[rdx], rcx")
             + x86("lea",  "rdi", "[rip + __]", da_addr(), da_label())
             + x86("mov",  "rsi", "rax")
             + x86("push", "rbx")
             + x86("mov", "rbx", "rsp")
             + x86("and", "rsp", -16L)
             + x86("call", "rt_gvar_assign_pat", da_fn())
             + x86("mov", "rsp", "rbx")
             + x86("pop",  "rbx")
             + x86("jmp", "γ")
             + da_proto()
             + x86("def", "β")
             + x86("jmp", "ω");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_dtp_assign(void) {
    da_bump();
    bb_emit_x86(bb_dtp_assign_str());
}

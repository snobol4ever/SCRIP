#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern uint8_t * g_pat_pool_cur;
static int g_pa_seq = 0;
static inline void         pa_bump()  { g_pa_seq++; }
static inline std::string  pal(const char * sfx) { return std::string(".Lpa") + std::to_string(g_pa_seq) + sfx; }
static inline std::string  pa_off()   { return std::to_string((long)_.op_off); }
/*--------------------------------------------------------------------------------------------------------------------*/
static inline std::string  pa_proto() {
    return x86("label", pal("_s"))
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("label", pal("_b"))
         + x86("ins2", "mov", "eax, dword ptr [rip + " + pal("_s") + " + 8]")
         + x86("ins2", "add", "eax, dword ptr [rip + " + pal("_s") + " + 12]")
         + x86("ins2", "inc", "dword ptr [rip + " + pal("_s") + " + 12]")
         + x86("ins2", "cmp", "eax, r15d")
         + x86("ins2", "jg", pal("_ov"))
         + x86("ins2", "mov", "r14d, eax")
         + x86("ins2", "jmp", "qword ptr [rip + " + pal("_s") + " + 16]")
         + x86("label", pal("_ov"))
         + x86("ins2", "mov", "r14d, dword ptr [rip + " + pal("_s") + " + 8]")
         + x86("ins2", "jmp", "qword ptr [rip + " + pal("_s") + " + 24]")
         + x86("label", pal("_a"))
         + x86("ins2", "mov", "dword ptr [rip + " + pal("_s") + " + 8], r14d")
         + x86("ins2", "mov", "dword ptr [rip + " + pal("_s") + " + 12], 0")
         + x86("ins2", "jmp", "qword ptr [rip + " + pal("_s") + " + 16]")
         + x86("label", pal("_e"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pattern_arb_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", std::string("BOX PATTERN_ARB  [BUILDER ζ=r12 frag@") + pa_off() + "]"))
             + x86("ins2", "mov", "rax, qword ptr [rip + g_pat_pool_cur]")
             + x86("mov", "rdi", "rax")
             + x86("ins2", "mov", "ecx, " + pal("_e") + " - " + pal("_s"))
             + x86("ins2", "lea", "rsi, [rip + " + pal("_s") + "]")
             + x86("ins1", "rep movsb")
             + x86("ins2", "mov", "qword ptr [rip + g_pat_pool_cur], rdi")
             + x86("ins2", "lea", "rcx, [rax + " + pal("_a") + " - " + pal("_s") + "]")
             + x86("ins2", "mov", "[r12 + " + pa_off() + "], rcx")
             + x86("ins2", "lea", "rcx, [rax + 16]")
             + x86("ins2", "mov", "[r12 + " + pa_off() + " + 8], rcx")
             + x86("ins2", "lea", "rcx, [rax + 24]")
             + x86("ins2", "mov", "[r12 + " + pa_off() + " + 16], rcx")
             + x86("ins2", "mov", "qword ptr [r12 + " + pa_off() + " + 24], 0")
             + x86("jmp", "γ")
             + pa_proto()
             + x86("def", "β")
             + x86("jmp", "ω");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pattern_arb(void) {
    pa_bump();
    bb_emit_x86(bb_pattern_arb_str());
}

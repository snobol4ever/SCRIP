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
static inline std::string  pa_sa()    { return std::to_string((long)_.op_sa); }
static inline std::string  pa_sb()    { return std::to_string((long)_.op_sb); }
static inline std::string  pa_off()   { return std::to_string((long)_.op_off); }
static inline std::string  pa_proto() {
    return x86("label", pal("_s"))
         + x86("raw", ".quad 0")
         + x86("label", pal("_j"))
         + x86("ins2", "jmp", "qword ptr [rip + " + pal("_s") + "]")
         + x86("label", pal("_e"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pattern_alt_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", std::string("BOX PATTERN_ALT  [STITCH ζ=r12 frag@") + pa_off() + " <- @" + pa_sa() + " | @" + pa_sb() + "]"))
             + x86("ins2", "mov", "rax, qword ptr [rip + g_pat_pool_cur]")
             + x86("mov", "rdi", "rax")
             + x86("ins2", "mov", "ecx, " + pal("_e") + " - " + pal("_s"))
             + x86("ins2", "lea", "rsi, [rip + " + pal("_s") + "]")
             + x86("ins1", "rep movsb")
             + x86("ins2", "mov", "qword ptr [rip + g_pat_pool_cur], rdi")
             + x86("ins2", "mov", "rcx, [r12 + " + pa_sb() + "]")
             + x86("ins2", "mov", "rdx, [r12 + " + pa_sa() + " + 16]")
             + x86("ins2", "mov", "[rdx], rcx")
             + x86("ins2", "lea", "rcx, [rax + " + pal("_j") + " - " + pal("_s") + "]")
             + x86("ins2", "mov", "rdx, [r12 + " + pa_sa() + " + 8]")
             + x86("ins2", "mov", "[rdx], rcx")
             + x86("ins2", "mov", "rdx, [r12 + " + pa_sb() + " + 8]")
             + x86("ins2", "mov", "[rdx], rcx")
             + x86("ins2", "mov", "rcx, [r12 + " + pa_sa() + "]")
             + x86("ins2", "mov", "[r12 + " + pa_off() + "], rcx")
             + x86("ins2", "mov", "[r12 + " + pa_off() + " + 8], rax")
             + x86("ins2", "mov", "rcx, [r12 + " + pa_sb() + " + 16]")
             + x86("ins2", "mov", "[r12 + " + pa_off() + " + 16], rcx")
             + x86("jmp", "γ")
             + pa_proto()
             + x86("def", "β")
             + x86("jmp", "ω");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pattern_alt(void) {
    pa_bump();
    bb_emit_x86(bb_pattern_alt_str());
}

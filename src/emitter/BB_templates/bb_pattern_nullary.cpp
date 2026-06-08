#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern uint8_t * g_pat_pool_cur;
static int g_pn_seq = 0;
static inline void         pn_bump()  { g_pn_seq++; }
static inline std::string  pnl(const char * sfx) { return std::string(".Lpn") + std::to_string(g_pn_seq) + sfx; }
static inline const char * pn_kind()  { return _.op_kind ? _.op_kind : "FAIL"; }
static inline std::string  pn_off()   { return std::to_string((long)_.op_off); }
static inline std::string  pn_alpha() {
    if (!strcmp(pn_kind(), "REM"))
        return x86("ins2", "mov", "[rip + " + pnl("_s") + " + 8], r14d")
             + x86("mov", "r14d", "r15d")
             + x86("ins2", "jmp", "qword ptr [rip + " + pnl("_s") + " + 16]");
    if (!strcmp(pn_kind(), "SUCCEED"))
        return x86("ins2", "jmp", "qword ptr [rip + " + pnl("_s") + " + 16]");
    return x86("ins2", "jmp", "qword ptr [rip + " + pnl("_s") + " + 24]");
}
static inline std::string  pn_beta() {
    if (!strcmp(pn_kind(), "REM"))
        return x86("ins2", "mov", "r14d, [rip + " + pnl("_s") + " + 8]")
             + x86("ins2", "jmp", "qword ptr [rip + " + pnl("_s") + " + 24]");
    if (!strcmp(pn_kind(), "SUCCEED"))
        return x86("ins2", "jmp", "qword ptr [rip + " + pnl("_s") + " + 16]");
    return x86("ins2", "jmp", "qword ptr [rip + " + pnl("_s") + " + 24]");
}
static inline std::string  pn_proto() {
    return x86("label", pnl("_s"))
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("label", pnl("_b"))
         + pn_beta()
         + x86("label", pnl("_a"))
         + pn_alpha()
         + x86("label", pnl("_e"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pattern_nullary_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", std::string("BOX PATTERN_") + pn_kind() + "  [BUILDER ζ=r12 frag@" + pn_off() + "]"))
             + x86("ins2", "mov", "rax, qword ptr [rip + g_pat_pool_cur]")
             + x86("mov", "rdi", "rax")
             + x86("ins2", "mov", "ecx, " + pnl("_e") + " - " + pnl("_s"))
             + x86("ins2", "lea", "rsi, [rip + " + pnl("_s") + "]")
             + x86("ins1", "rep movsb")
             + x86("ins2", "mov", "qword ptr [rip + g_pat_pool_cur], rdi")
             + x86("ins2", "lea", "rcx, [rax + " + pnl("_a") + " - " + pnl("_s") + "]")
             + x86("ins2", "mov", "[r12 + " + pn_off() + "], rcx")
             + x86("ins2", "lea", "rcx, [rax + 16]")
             + x86("ins2", "mov", "[r12 + " + pn_off() + " + 8], rcx")
             + x86("ins2", "lea", "rcx, [rax + 24]")
             + x86("ins2", "mov", "[r12 + " + pn_off() + " + 16], rcx")
             + x86("jmp", "γ")
             + pn_proto()
             + x86("def", "β")
             + x86("jmp", "ω");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pattern_nullary(void) {
    pn_bump();
    bb_emit_x86(bb_pattern_nullary_str());
}

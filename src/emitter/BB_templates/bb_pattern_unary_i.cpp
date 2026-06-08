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
static int g_pu_seq = 0;
static inline void         pu_bump()  { g_pu_seq++; }
static inline std::string  pul(const char * sfx) { return std::string(".Lpu") + std::to_string(g_pu_seq) + sfx; }
static inline const char * pu_kind()  { return _.op_kind ? _.op_kind : "LEN"; }
static inline long         pu_n()     { return (long)_.op_ival; }
static inline std::string  pu_off()   { return std::to_string((long)_.op_off); }
static inline std::string  pu_alpha() {
    if (!strcmp(pu_kind(), "POS"))
        return x86("ins2", "mov", "ecx, [rip + " + pul("_s") + "]")
             + x86("cmp", "r14d", "ecx")
             + x86("ins2", "jne", pul("_f"))
             + x86("ins2", "jmp", "qword ptr [rip + " + pul("_s") + " + 16]");
    if (!strcmp(pu_kind(), "RPOS"))
        return x86("ins2", "mov", "ecx, [rip + " + pul("_s") + "]")
             + x86("mov", "eax", "r15d")
             + x86("sub", "eax", "ecx")
             + x86("cmp", "r14d", "eax")
             + x86("ins2", "jne", pul("_f"))
             + x86("ins2", "jmp", "qword ptr [rip + " + pul("_s") + " + 16]");
    if (!strcmp(pu_kind(), "TAB"))
        return x86("ins2", "mov", "ecx, [rip + " + pul("_s") + "]")
             + x86("cmp", "ecx", "r14d")
             + x86("ins2", "jl", pul("_f"))
             + x86("cmp", "ecx", "r15d")
             + x86("ins2", "jg", pul("_f"))
             + x86("ins2", "mov", "[rip + " + pul("_s") + " + 8], r14d")
             + x86("mov", "r14d", "ecx")
             + x86("ins2", "jmp", "qword ptr [rip + " + pul("_s") + " + 16]");
    if (!strcmp(pu_kind(), "RTAB"))
        return x86("ins2", "mov", "ecx, [rip + " + pul("_s") + "]")
             + x86("mov", "eax", "r15d")
             + x86("sub", "eax", "ecx")
             + x86("cmp", "eax", "r14d")
             + x86("ins2", "jl", pul("_f"))
             + x86("ins2", "mov", "[rip + " + pul("_s") + " + 8], r14d")
             + x86("mov", "r14d", "eax")
             + x86("ins2", "jmp", "qword ptr [rip + " + pul("_s") + " + 16]");
    return x86("ins2", "mov", "ecx, [rip + " + pul("_s") + "]")
         + x86("mov", "eax", "r14d")
         + x86("add", "eax", "ecx")
         + x86("cmp", "eax", "r15d")
         + x86("ins2", "jg", pul("_f"))
         + x86("mov", "r14d", "eax")
         + x86("ins2", "jmp", "qword ptr [rip + " + pul("_s") + " + 16]");
}
static inline std::string  pu_beta() {
    if (!strcmp(pu_kind(), "POS") || !strcmp(pu_kind(), "RPOS"))
        return x86("ins2", "jmp", "qword ptr [rip + " + pul("_s") + " + 24]");
    if (!strcmp(pu_kind(), "TAB") || !strcmp(pu_kind(), "RTAB"))
        return x86("ins2", "mov", "r14d, [rip + " + pul("_s") + " + 8]")
             + x86("ins2", "jmp", "qword ptr [rip + " + pul("_s") + " + 24]");
    return x86("ins2", "mov", "ecx, [rip + " + pul("_s") + "]")
         + x86("sub", "r14d", "ecx")
         + x86("ins2", "jmp", "qword ptr [rip + " + pul("_s") + " + 24]");
}
static inline std::string  pu_proto() {
    return x86("label", pul("_s"))
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("label", pul("_b"))
         + pu_beta()
         + x86("label", pul("_a"))
         + pu_alpha()
         + x86("label", pul("_f"))
         + x86("ins2", "jmp", "qword ptr [rip + " + pul("_s") + " + 24]")
         + x86("label", pul("_e"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pattern_unary_i_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", std::string("BOX PATTERN_") + pu_kind() + "(" + std::to_string(pu_n()) + ")  [BUILDER ζ=r12 frag@" + pu_off() + "]"))
             + x86("ins2", "mov", "rax, qword ptr [rip + g_pat_pool_cur]")
             + x86("mov", "rdi", "rax")
             + x86("ins2", "mov", "ecx, " + pul("_e") + " - " + pul("_s"))
             + x86("ins2", "lea", "rsi, [rip + " + pul("_s") + "]")
             + x86("ins1", "rep movsb")
             + x86("ins2", "mov", "qword ptr [rip + g_pat_pool_cur], rdi")
             + x86("ins2", "mov", "ecx, " + std::to_string(pu_n()))
             + x86("ins2", "mov", "[rax], ecx")
             + x86("ins2", "lea", "rcx, [rax + " + pul("_a") + " - " + pul("_s") + "]")
             + x86("ins2", "mov", "[r12 + " + pu_off() + "], rcx")
             + x86("ins2", "lea", "rcx, [rax + 16]")
             + x86("ins2", "mov", "[r12 + " + pu_off() + " + 8], rcx")
             + x86("ins2", "lea", "rcx, [rax + 24]")
             + x86("ins2", "mov", "[r12 + " + pu_off() + " + 16], rcx")
             + x86("jmp", "γ")
             + pu_proto()
             + x86("def", "β")
             + x86("jmp", "ω");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pattern_unary_i(void) {
    pu_bump();
    bb_emit_x86(bb_pattern_unary_i_str());
}

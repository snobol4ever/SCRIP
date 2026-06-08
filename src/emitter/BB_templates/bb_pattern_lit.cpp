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
static int g_pb_seq = 0;
static inline void         pb_bump()     { g_pb_seq++; }
static inline std::string  pbl(const char * sfx) { return std::string(".Lpb") + std::to_string(g_pb_seq) + sfx; }
static inline const char * plit()        { return _.op_sval ? _.op_sval : ""; }
static inline long         plitlen()     { return (long)strlen(plit()); }
static inline const char * plitlabel()   { const char * l = emit_intern_str(plit()); if (l) return l;
                                           static char b[24]; strtab_label(b, sizeof b, plit()); return b; }
static inline uint64_t     plitaddr()    { return (uint64_t)(uintptr_t)plit(); }
static inline uint64_t     pcuraddr()    { return (uint64_t)(uintptr_t)(const void *)&g_pat_pool_cur; }
static inline std::string  pb_off()      { return std::to_string((long)_.op_off); }
static inline std::string  pb_proto() {
    return x86("label", pbl("_s"))
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("label", pbl("_b"))
         + x86("ins2", "mov", "ecx, [rip + " + pbl("_s") + " + 8]")
         + x86("sub", "r14d", "ecx")
         + x86("ins2", "jmp", "qword ptr [rip + " + pbl("_s") + " + 24]")
         + x86("label", pbl("_a"))
         + x86("ins2", "mov", "ecx, [rip + " + pbl("_s") + " + 8]")
         + x86("mov", "eax", "r14d")
         + x86("add", "eax", "ecx")
         + x86("cmp", "eax", "r15d")
         + x86("ins2", "jg", pbl("_f"))
         + x86("ins2", "mov", "rsi, [rip + " + pbl("_s") + "]")
         + x86("movsxd", "rax", "r14d")
         + x86("ins2", "lea", "rdi, [r13 + rax]")
         + x86("label", pbl("_l"))
         + x86("test", "ecx", "ecx")
         + x86("ins2", "je", pbl("_m"))
         + x86("ins2", "mov", "al, [rsi]")
         + x86("ins2", "cmp", "al, [rdi]")
         + x86("ins2", "jne", pbl("_f"))
         + x86("ins1", "inc rsi")
         + x86("ins1", "inc rdi")
         + x86("ins1", "dec ecx")
         + x86("ins2", "jmp", pbl("_l"))
         + x86("label", pbl("_m"))
         + x86("ins2", "mov", "ecx, [rip + " + pbl("_s") + " + 8]")
         + x86("add", "r14d", "ecx")
         + x86("ins2", "jmp", "qword ptr [rip + " + pbl("_s") + " + 16]")
         + x86("label", pbl("_f"))
         + x86("ins2", "jmp", "qword ptr [rip + " + pbl("_s") + " + 24]")
         + x86("label", pbl("_e"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pattern_lit_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", std::string("BOX PATTERN_LIT('") + plit() + "')  [BUILDER ζ=r12 frag@" + pb_off() + "]"))
             + x86("mov", "rax", "[rip + __]", pcuraddr(), "g_pat_pool_cur")
             + x86("mov", "rdi", "rax")
             + x86("ins2", "mov", "ecx, " + pbl("_e") + " - " + pbl("_s"))
             + x86("ins2", "lea", "rsi, [rip + " + pbl("_s") + "]")
             + x86("ins1", "rep movsb")
             + x86("ins2", "mov", "qword ptr [rip + g_pat_pool_cur], rdi")
             + x86("lea", "rcx", "[rip + __]", plitaddr(), plitlabel())
             + x86("ins2", "mov", "[rax], rcx")
             + x86("mov", "rcx", plitlen())
             + x86("ins2", "mov", "[rax + 8], rcx")
             + x86("ins2", "lea", "rcx, [rax + " + pbl("_a") + " - " + pbl("_s") + "]")
             + x86("ins2", "mov", "[r12 + " + pb_off() + "], rcx")
             + x86("ins2", "lea", "rcx, [rax + 16]")
             + x86("ins2", "mov", "[r12 + " + pb_off() + " + 8], rcx")
             + x86("ins2", "lea", "rcx, [rax + 24]")
             + x86("ins2", "mov", "[r12 + " + pb_off() + " + 16], rcx")
             + x86("jmp", "γ")
             + pb_proto()
             + x86("def", "β")
             + x86("jmp", "ω");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pattern_lit(void) {
    pb_bump();
    bb_emit_x86(bb_pattern_lit_str());
}

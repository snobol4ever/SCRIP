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
static int g_us_seq = 0;
static inline void         us_bump()  { g_us_seq++; }
static inline std::string  usl(const std::string & sfx) { return std::string(".Lus") + std::to_string(g_us_seq) + sfx; }
static inline const char * us_kind()  { return _.op_kind ? _.op_kind : "ANY"; }
static inline const char * us_cset()  { return _.op_sval ? _.op_sval : ""; }
static inline std::string  us_off()   { return std::to_string((long)_.op_off); }
static inline std::string  us_label() { const char * l = emit_intern_str(us_cset()); if (l) return l;
                                        static char b[24]; strtab_label(b, sizeof b, us_cset()); return b; }
static inline uint64_t     us_addr()  { return (uint64_t)(uintptr_t)us_cset(); }
/*--------------------------------------------------------------------------------------------------------------------*/
static inline std::string  us_scan_one(const std::string & p, const char * hit, const char * miss) {
    return x86("ins2", "mov", "rsi, qword ptr [rip + " + usl("_s") + "]")
         + x86("label", usl(p + "ml"))
         + x86("ins2", "mov", "cl, byte ptr [rsi]")
         + x86("ins2", "test", "cl, cl")
         + x86("ins2", "je", usl(miss))
         + x86("ins2", "cmp", "cl, dl")
         + x86("ins2", "je", usl(hit))
         + x86("ins1", "inc rsi")
         + x86("ins2", "jmp", usl(p + "ml"));
}
static inline std::string  us_alpha() {
    const char * k = us_kind();
    if (!strcmp(k, "ANY"))
        return x86("ins2", "cmp", "r14d, r15d")
             + x86("ins2", "jge", usl("_f"))
             + x86("ins2", "movsxd", "rax, r14d")
             + x86("ins2", "movzx", "edx, byte ptr [r13 + rax]")
             + us_scan_one("_a", "_ahit", "_f")
             + x86("label", usl("_ahit"))
             + x86("ins1", "inc r14d")
             + x86("ins2", "jmp", "qword ptr [rip + " + usl("_s") + " + 16]");
    if (!strcmp(k, "NOTANY"))
        return x86("ins2", "cmp", "r14d, r15d")
             + x86("ins2", "jge", usl("_f"))
             + x86("ins2", "movsxd", "rax, r14d")
             + x86("ins2", "movzx", "edx, byte ptr [r13 + rax]")
             + us_scan_one("_n", "_f", "_nok")
             + x86("label", usl("_nok"))
             + x86("ins1", "inc r14d")
             + x86("ins2", "jmp", "qword ptr [rip + " + usl("_s") + " + 16]");
    if (!strcmp(k, "SPAN"))
        return x86("ins2", "mov", "eax, r14d")
             + x86("label", usl("_sscan"))
             + x86("ins2", "cmp", "eax, r15d")
             + x86("ins2", "jge", usl("_sdone"))
             + x86("ins2", "movsxd", "r9, eax")
             + x86("ins2", "movzx", "edx, byte ptr [r13 + r9]")
             + us_scan_one("_s", "_smore", "_sdone")
             + x86("label", usl("_smore"))
             + x86("ins1", "inc eax")
             + x86("ins2", "jmp", usl("_sscan"))
             + x86("label", usl("_sdone"))
             + x86("ins2", "cmp", "eax, r14d")
             + x86("ins2", "je", usl("_f"))
             + x86("ins2", "mov", "r14d, eax")
             + x86("ins2", "jmp", "qword ptr [rip + " + usl("_s") + " + 16]");
    if (!strcmp(k, "BREAKX"))
        return x86("ins2", "mov", "eax, r14d")
             + x86("label", usl("_xscan"))
             + x86("ins2", "cmp", "eax, r15d")
             + x86("ins2", "jge", usl("_f"))
             + x86("ins2", "movsxd", "r9, eax")
             + x86("ins2", "movzx", "edx, byte ptr [r13 + r9]")
             + us_scan_one("_x", "_xstop", "_xnext")
             + x86("label", usl("_xnext"))
             + x86("ins1", "inc eax")
             + x86("ins2", "jmp", usl("_xscan"))
             + x86("label", usl("_xstop"))
             + x86("ins2", "mov", "dword ptr [rip + " + usl("_s") + " + 8], eax")
             + x86("ins2", "mov", "r14d, eax")
             + x86("ins2", "jmp", "qword ptr [rip + " + usl("_s") + " + 16]");
    return x86("ins2", "mov", "eax, r14d")
         + x86("label", usl("_bscan"))
         + x86("ins2", "cmp", "eax, r15d")
         + x86("ins2", "jge", usl("_f"))
         + x86("ins2", "movsxd", "r9, eax")
         + x86("ins2", "movzx", "edx, byte ptr [r13 + r9]")
         + us_scan_one("_b", "_bstop", "_bnext")
         + x86("label", usl("_bnext"))
         + x86("ins1", "inc eax")
         + x86("ins2", "jmp", usl("_bscan"))
         + x86("label", usl("_bstop"))
         + x86("ins2", "mov", "r14d, eax")
         + x86("ins2", "jmp", "qword ptr [rip + " + usl("_s") + " + 16]");
}
static inline std::string  us_beta() {
    if (strcmp(us_kind(), "BREAKX"))
        return x86("ins2", "jmp", "qword ptr [rip + " + usl("_s") + " + 24]");
    return x86("ins2", "mov", "eax, dword ptr [rip + " + usl("_s") + " + 8]")
         + x86("ins1", "inc eax")
         + x86("label", usl("_yscan"))
         + x86("ins2", "cmp", "eax, r15d")
         + x86("ins2", "jge", usl("_yfail"))
         + x86("ins2", "movsxd", "r9, eax")
         + x86("ins2", "movzx", "edx, byte ptr [r13 + r9]")
         + us_scan_one("_y", "_ystop", "_ynext")
         + x86("label", usl("_ynext"))
         + x86("ins1", "inc eax")
         + x86("ins2", "jmp", usl("_yscan"))
         + x86("label", usl("_ystop"))
         + x86("ins2", "mov", "dword ptr [rip + " + usl("_s") + " + 8], eax")
         + x86("ins2", "mov", "r14d, eax")
         + x86("ins2", "jmp", "qword ptr [rip + " + usl("_s") + " + 16]")
         + x86("label", usl("_yfail"))
         + x86("ins2", "jmp", "qword ptr [rip + " + usl("_s") + " + 24]");
}
static inline std::string  us_proto() {
    return x86("label", usl("_s"))
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("raw", ".quad 0")
         + x86("label", usl("_b"))
         + us_beta()
         + x86("label", usl("_a"))
         + us_alpha()
         + x86("label", usl("_f"))
         + x86("ins2", "jmp", "qword ptr [rip + " + usl("_s") + " + 24]")
         + x86("label", usl("_e"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pattern_unary_s_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", std::string("BOX PATTERN_") + us_kind() + "('" + us_cset() + "')  [BUILDER ζ=r12 frag@" + us_off() + "]"))
             + x86("ins2", "mov", "rax, qword ptr [rip + g_pat_pool_cur]")
             + x86("mov", "rdi", "rax")
             + x86("ins2", "mov", "ecx, " + usl("_e") + " - " + usl("_s"))
             + x86("ins2", "lea", "rsi, [rip + " + usl("_s") + "]")
             + x86("ins1", "rep movsb")
             + x86("ins2", "mov", "qword ptr [rip + g_pat_pool_cur], rdi")
             + x86("lea", "rcx", "[rip + __]", us_addr(), us_label())
             + x86("ins2", "mov", "[rax], rcx")
             + x86("ins2", "lea", "rcx, [rax + " + usl("_a") + " - " + usl("_s") + "]")
             + x86("ins2", "mov", "[r12 + " + us_off() + "], rcx")
             + x86("ins2", "lea", "rcx, [rax + 16]")
             + x86("ins2", "mov", "[r12 + " + us_off() + " + 8], rcx")
             + x86("ins2", "lea", "rcx, [rax + 24]")
             + x86("ins2", "mov", "[r12 + " + us_off() + " + 16], rcx")
             + x86("jmp", "γ")
             + us_proto()
             + x86("def", "β")
             + x86("jmp", "ω");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pattern_unary_s(void) {
    us_bump();
    bb_emit_x86(bb_pattern_unary_s_str());
}

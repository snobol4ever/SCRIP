#include <string>
#include <cstdio>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bm_line(const char *lbl, const char *act, const char *got)
{
    std::string line = emit_fmt("%-24s%-16s %s", lbl ? lbl : "", act ? act : "", got ? got : "");
    while (!line.empty() && (line.back() == ' ' || line.back() == '\t')) line.pop_back();
    return line + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bm_macro(const char *name, const char *args)
{
    std::string decl = (args && args[0]) ? emit_fmt("%s %s", name, args) : std::string(name);
    return bm_line("", ".macro", decl.c_str());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bm_endm (void)                          { return bm_line("", ".endm", ""); }
static std::string bm_op   (const char *mn, const char *a) { return bm_line("", mn, a ? a : ""); }
static std::string bm_jmp  (const char *cond, const char *tgt) { return bm_line("", cond, emit_fmt("\\%s", tgt).c_str()); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_bb_macro_library_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_BB_MACRO_LIBRARY (it IS the bb macro-def pass)");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            return std::string("# === BEGIN bb macro library (EAO-4/xa_bb_macro_library) ===\n")
                 + "                        .intel_syntax    noprefix\n"
                 + bm_macro("DELTA_LOAD", "")  + bm_op("mov", "eax, [r10]") + bm_endm()
                 + bm_macro("SIGLEN_LOAD", "") + bm_op("lea", "rcx, [rip + \xce\xa3len]") + bm_op("mov", "eax, [rcx]") + bm_endm()
                 + bm_macro("EPS_\xce\xb1", "lbl_γ")  + bm_jmp("jmp", "lbl_γ") + bm_endm()
                 + bm_macro("EPS_\xce\xb2", "lbl_ω")  + bm_jmp("jmp", "lbl_ω") + bm_endm()
                 + bm_macro("FAIL_\xce\xb1", "lbl_ω") + bm_jmp("jmp", "lbl_ω") + bm_endm()
                 + bm_macro("FAIL_\xce\xb2", "lbl_ω") + bm_jmp("jmp", "lbl_ω") + bm_endm()
                 + bm_macro("RPOS_\xce\xb1", "n, lbl_γ, lbl_ω")
                       + bm_op("SIGLEN_LOAD", "") + bm_op("sub", "eax, \\n") + bm_op("mov", "ecx, eax")
                       + bm_op("DELTA_LOAD", "") + bm_op("cmp", "eax, ecx")
                       + bm_jmp("jne", "lbl_ω") + bm_jmp("jmp", "lbl_γ") + bm_endm()
                 + bm_macro("RPOS_\xce\xb2", "lbl_ω") + bm_jmp("jmp", "lbl_ω") + bm_endm()
                 + bm_macro("POS_\xce\xb1", "n, lbl_γ, lbl_ω")
                       + bm_op("DELTA_LOAD", "") + bm_op("cmp", "eax, \\n")
                       + bm_jmp("jne", "lbl_ω") + bm_jmp("jmp", "lbl_γ") + bm_endm()
                 + bm_macro("POS_\xce\xb2", "lbl_ω") + bm_jmp("jmp", "lbl_ω") + bm_endm()
                 + "# === END bb macro library ===\n";
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_bb_macro_library(void) {
    auto s = xa_bb_macro_library_str();
    if (s.empty()) return;
    if (MEDIUM_TEXT) {
        FILE *f = fopen("bb_macros.s", "w");
        if (!f) return;
        fwrite(s.data(), 1, s.size(), f);
        fclose(f);
        return;
    }
    emit_text_n(s.data(), s.size());
}

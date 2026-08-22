#include <string>
#include <cstdio>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_bb_macro_library_text(void) {
    if (PLATFORM_X86) {
        return std::string("# === BEGIN bb macro library (EAO-4/xa_bb_macro_library) ===\n")
             + "                        .intel_syntax    noprefix\n"
             + "                        .macro           DELTA_LOAD\n"
             + "                        mov              eax, [r11]\n"
             + "                        .endm\n"
             + "                        .macro           SIGLEN_LOAD\n"
             + "                        lea              rcx, [rip + Σlen]\n"
             + "                        mov              eax, [rcx]\n"
             + "                        .endm\n"
             + "                        .macro           EPS_α lbl_γ\n"
             + "                        jmp              \\lbl_γ\n"
             + "                        .endm\n"
             + "                        .macro           EPS_β lbl_ω\n"
             + "                        jmp              \\lbl_ω\n"
             + "                        .endm\n"
             + "                        .macro           FAIL_α lbl_ω\n"
             + "                        jmp              \\lbl_ω\n"
             + "                        .endm\n"
             + "                        .macro           FAIL_β lbl_ω\n"
             + "                        jmp              \\lbl_ω\n"
             + "                        .endm\n"
             + "                        .macro           RPOS_α n, lbl_γ, lbl_ω\n"
             + "                        SIGLEN_LOAD\n"
             + "                        sub              eax, \\n\n"
             + "                        mov              ecx, eax\n"
             + "                        DELTA_LOAD\n"
             + "                        cmp              eax, ecx\n"
             + "                        jne              \\lbl_ω\n"
             + "                        jmp              \\lbl_γ\n"
             + "                        .endm\n"
             + "                        .macro           RPOS_β lbl_ω\n"
             + "                        jmp              \\lbl_ω\n"
             + "                        .endm\n"
             + "                        .macro           POS_α n, lbl_γ, lbl_ω\n"
             + "                        DELTA_LOAD\n"
             + "                        cmp              eax, \\n\n"
             + "                        jne              \\lbl_ω\n"
             + "                        jmp              \\lbl_γ\n"
             + "                        .endm\n"
             + "                        .macro           POS_β lbl_ω\n"
             + "                        jmp              \\lbl_ω\n"
             + "                        .endm\n"
             + "# === END bb macro library ===\n";
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_bb_macro_library(void) {
    FILE *f = PLATFORM_X86 ? fopen("bb_macros.s", "w") : (FILE *)0;
    if (f) { fputs(xa_bb_macro_library_text().c_str(), f); fclose(f); }
}

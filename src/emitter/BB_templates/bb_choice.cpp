#include <string>
#include <vector>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "IR.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_choice_str(IR_t * pBB) {
    (void)pBB;
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "no macro form — RESOLVE_CHOICE");
        if (MEDIUM_TEXT) {
            int id = _.resolve_choice_id;
            int n  = _.resolve_choice_n;
            if (n <= 0) {
                return x86("label", _.lbl_α)
                     + x86("ins2", "jmp", _.lbl_ω)
                     + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            char disp[160];      snprintf(disp,      sizeof disp,      ".Lplch%d_dispatch",  id);
            char exit_γ[160];    snprintf(exit_γ,    sizeof exit_γ,    ".Lplch%d_exit_γ",    id);
            char exhausted[160]; snprintf(exhausted,  sizeof exhausted, ".Lplch%d_exhausted", id);
            char cut_unwind_ω[160]; snprintf(cut_unwind_ω, sizeof cut_unwind_ω, ".Lplch%d_cut_ω", id);
            char cut_unwind_γ[160]; snprintf(cut_unwind_γ, sizeof cut_unwind_γ, ".Lplch%d_cut_γ", id);
            char β_nosol[160];      snprintf(β_nosol,      sizeof β_nosol,      ".Lplch%d_β_nosol", id);
            std::string out = x86("label", _.lbl_α)
                            + x86("comment", emit_fmt("BOX RESOLVE_CHOICE n=%d (heap cursor, cut save/restore)", n))
                            + x86("ins2", "call", "rt_env_current@PLT")
                            + x86("ins2", "mov",  "rdx, rax")
                            + x86("ins2", "call", "rt_trail_mark@PLT")
                            + x86("ins2", "mov",  "rsi, rax")
                            + x86("ins2", "xor",  "edi, edi")
                            + x86("ins2", "xor",  "ecx, ecx")
                            + x86("ins2", "xor",  "r8d, r8d")
                            + x86("ins2", "call", "resolve_cp_push@PLT")
                            + x86("ins2", "mov",  "rdi, rax")
                            + x86("ins2", "call", "rt_choice_cut_enter@PLT");
            out += x86("label", disp)
                 + x86("ins2", "call", "resolve_cp_current@PLT")
                 + x86("ins2", "mov",  "edi, [rax + 48]")
                 + x86("ins2", "cmp",  emit_fmt("edi, %d", n))
                 + x86("ins2", "jge",  exhausted);
            for (int i = 0; i < n; i++) {
                char prei[160]; resolve_choice_clause_label(prei, sizeof prei, id, i, "pre");
                out += x86("ins2", "cmp", emit_fmt("edi, %d", i))
                     + x86("ins2", "je",  prei);
            }
            out += x86("ins2", "jmp", exhausted);
            char body0[160]; resolve_choice_clause_label(body0, sizeof body0, id, 0, "body");
            char pre0[160];  resolve_choice_clause_label(pre0,  sizeof pre0,  id, 0, "pre");
            out += x86("label", pre0)
                 + x86("ins2", "call", "resolve_cp_current@PLT")
                 + x86("ins2", "inc",  "dword ptr [rax + 48]")
                 + x86("ins2", "jmp",  body0);
            for (int i = 1; i < n; i++) {
                char prei[160], bodyi[160];
                resolve_choice_clause_label(prei,  sizeof prei,  id, i, "pre");
                resolve_choice_clause_label(bodyi, sizeof bodyi, id, i, "body");
                out += x86("label", prei)
                     + x86("ins2", "call", "resolve_cp_current@PLT")
                     + x86("ins2", "mov",  "edi, [rax + 16]")
                     + x86("ins2", "call", "rt_trail_unwind@PLT")
                     + x86("ins2", "call", "resolve_cp_current@PLT")
                     + x86("ins2", "inc",  "dword ptr [rax + 48]")
                     + x86("ins2", "jmp",  bodyi);
            }
            out += x86("label", exit_γ)
                 + x86("ins2", "call", "rt_get_cut_flag@PLT")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "jnz",  cut_unwind_γ)
                 + x86("ins2", "call", "resolve_cp_current@PLT")
                 + x86("ins2", "mov",  "rdi, rax")
                 + x86("ins2", "call", "rt_choice_cut_exit@PLT")
                 + x86("ins2", "jmp",  _.lbl_γ);
            out += x86("label", cut_unwind_γ)
                 + x86("ins2", "call", "resolve_cp_current@PLT")
                 + x86("ins2", "mov",  "rdi, rax")
                 + x86("ins2", "call", "rt_choice_cut_unwind@PLT")
                 + x86("ins2", "jmp",  _.lbl_γ);
            out += x86("label", cut_unwind_ω)
                 + x86("ins2", "call", "resolve_cp_current@PLT")
                 + x86("ins2", "mov",  "rdi, rax")
                 + x86("ins2", "call", "rt_choice_cut_unwind@PLT")
                 + x86("ins2", "jmp",  _.lbl_ω);
            out += x86("label", exhausted)
                 + x86("ins2", "call", "resolve_cp_current@PLT")
                 + x86("ins2", "mov",  "rdi, rax")
                 + x86("ins2", "call", "rt_choice_cut_exit@PLT")
                 + x86("ins2", "call", "resolve_cp_current@PLT")
                 + x86("ins2", "mov",  "edi, [rax + 16]")
                 + x86("ins2", "call", "rt_trail_unwind@PLT")
                 + x86("ins2", "call", "resolve_cp_pop@PLT")
                 + x86("ins2", "jmp",  _.lbl_ω);
            out += x86("label", _.lbl_β)
                 + x86("ins2", "call", "rt_get_cut_flag@PLT")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "jnz",  cut_unwind_ω)
                 + x86("ins2", "call", "resolve_cp_current@PLT")
                 + x86("ins2", "test", "rax, rax")
                 + x86("ins2", "je",   β_nosol)
                 + x86("ins2", "mov",  "rdi, rax")
                 + x86("ins2", "call", "rt_choice_cut_enter@PLT")
                 + x86("ins2", "jmp",  disp);
            out += x86("Lins2", emit_fmt("%s:", β_nosol), "jmp", _.lbl_ω);
            return out;
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_choice(IR_t * pBB) { bb_emit_x86(bb_choice_str(pBB)); }

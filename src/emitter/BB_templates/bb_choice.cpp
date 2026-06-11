#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string plch(const char *tag) { return ".Lplch" + std::to_string(_.resolve_choice_id) + "_" + tag; }
static std::string plchi(int i, const char *tag) { char b[160]; resolve_choice_clause_label(b, sizeof b, _.resolve_choice_id, i, tag); return std::string(b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_choice_str(void) {
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return x86("comment", "no macro form — RESOLVE_CHOICE");
    return IF(MEDIUM_TEXT,
           IF(_.resolve_choice_n <= 0, x86("label", _.lbl_α)
                                     + x86("ins2", "jmp", _.lbl_ω)
                                     + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω))
         + IF(_.resolve_choice_n >  0, x86("label", _.lbl_α)
                                     + x86("comment", "IR_CHOICE")
                                     + x86("ins2", "call", "rt_env_current@PLT")
                                     + x86("ins2", "mov",  "rdx, rax")
                                     + x86("ins2", "call", "rt_trail_mark@PLT")
                                     + x86("ins2", "mov",  "rsi, rax")
                                     + x86("ins2", "xor",  "edi, edi")
                                     + x86("ins2", "xor",  "ecx, ecx")
                                     + x86("ins2", "xor",  "r8d, r8d")
                                     + x86("ins2", "call", "resolve_cp_push@PLT")
                                     + x86("ins2", "mov",  "rdi, rax")
                                     + x86("ins2", "call", "rt_choice_cut_enter@PLT")
                                     + x86("label", plch("dispatch"))
                                     + x86("ins2", "call", "resolve_cp_current@PLT")
                                     + x86("ins2", "mov",  "edi, [rax + 48]")
                                     + x86("ins2", "cmp",  "edi, " + std::to_string(_.resolve_choice_n))
                                     + x86("ins2", "jge",  plch("exhausted"))
                                     + FOR(0, _.resolve_choice_n, [](int i) { return x86("ins2", "cmp", "edi, " + std::to_string(i))
                                                                                    + x86("ins2", "je",  plchi(i, "pre")); })
                                     + x86("ins2", "jmp", plch("exhausted"))
                                     + FOR(0, _.resolve_choice_n, [](int i) { return x86("label", plchi(i, "pre"))
                                                                                    + x86("ins2", "call", "resolve_cp_current@PLT")
                                                                                    + IF(i > 0, x86("ins2", "mov",  "edi, [rax + 16]")
                                                                                              + x86("ins2", "call", "rt_trail_unwind@PLT")
                                                                                              + x86("ins2", "call", "resolve_cp_current@PLT"))
                                                                                    + x86("ins2", "inc",  "dword ptr [rax + 48]")
                                                                                    + x86("ins2", "jmp",  plchi(i, "body")); })
                                     + x86("label", plch("exit_γ"))
                                     + x86("ins2", "call", "rt_get_cut_flag@PLT")
                                     + x86("ins2", "test", "eax, eax")
                                     + x86("ins2", "jnz",  plch("cut_γ"))
                                     + x86("ins2", "call", "resolve_cp_current@PLT")
                                     + x86("ins2", "mov",  "rdi, rax")
                                     + x86("ins2", "call", "rt_choice_cut_exit@PLT")
                                     + x86("ins2", "jmp",  _.lbl_γ)
                                     + x86("label", plch("cut_γ"))
                                     + x86("ins2", "call", "resolve_cp_current@PLT")
                                     + x86("ins2", "mov",  "rdi, rax")
                                     + x86("ins2", "call", "rt_choice_cut_unwind@PLT")
                                     + x86("ins2", "jmp",  _.lbl_γ)
                                     + x86("label", plch("cut_ω"))
                                     + x86("ins2", "call", "resolve_cp_current@PLT")
                                     + x86("ins2", "mov",  "rdi, rax")
                                     + x86("ins2", "call", "rt_choice_cut_unwind@PLT")
                                     + x86("ins2", "jmp",  _.lbl_ω)
                                     + x86("label", plch("exhausted"))
                                     + x86("ins2", "call", "resolve_cp_current@PLT")
                                     + x86("ins2", "mov",  "rdi, rax")
                                     + x86("ins2", "call", "rt_choice_cut_exit@PLT")
                                     + x86("ins2", "call", "resolve_cp_current@PLT")
                                     + x86("ins2", "mov",  "edi, [rax + 16]")
                                     + x86("ins2", "call", "rt_trail_unwind@PLT")
                                     + x86("ins2", "call", "resolve_cp_pop@PLT")
                                     + x86("ins2", "jmp",  _.lbl_ω)
                                     + x86("label", _.lbl_β)
                                     + x86("ins2", "call", "rt_get_cut_flag@PLT")
                                     + x86("ins2", "test", "eax, eax")
                                     + x86("ins2", "jnz",  plch("cut_ω"))
                                     + x86("ins2", "call", "resolve_cp_current@PLT")
                                     + x86("ins2", "test", "rax, rax")
                                     + x86("ins2", "je",   plch("β_nosol"))
                                     + x86("ins2", "mov",  "rdi, rax")
                                     + x86("ins2", "call", "rt_choice_cut_enter@PLT")
                                     + x86("ins2", "jmp",  plch("dispatch"))
                                     + x86("Lins2", plch("β_nosol") + ":", "jmp", _.lbl_ω)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_choice(void) { bb_emit_x86(bb_choice_str()); }

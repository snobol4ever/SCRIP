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
std::string bb_choice() {
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return x86("comment", "no macro form — RESOLVE_CHOICE");
    return IF(MEDIUM_TEXT,
           IF(_.resolve_choice_n <= 0, x86("label", _.lbl_α)
                                     + x86("jmp", "ω")
                                     + x86("def", "β") + x86("jmp", "ω"))
         + IF(_.resolve_choice_n >  0, x86("label", _.lbl_α)
                                     + x86("comment", "IR_CHOICE")
                                     + x86("call", "rt_env_current@PLT")
                                     + x86("mov", "rdx", "rax")
                                     + x86("call", "rt_trail_mark@PLT")
                                     + x86("mov", "rsi", "rax")
                                     + x86("xor", "edi", "edi")
                                     + x86("xor", "ecx", "ecx")
                                     + x86("xor", "r8d", "r8d")
                                     + x86("call", "resolve_cp_push@PLT")
                                     + x86("mov", "rdi", "rax")
                                     + x86("call", "rt_choice_cut_enter@PLT")
                                     + x86("label", plch("dispatch"))
                                     + x86("call", "resolve_cp_current@PLT")
                                     + x86("mov", "edi", "[rax + 48]")
                                     + x86("cmp", "edi", (long)_.resolve_choice_n)
                                     + x86("jge",  plch("exhausted"))
                                     + FOR(0, _.resolve_choice_n, [](int i) { return x86("cmp", "edi", std::to_string(i))
                                                                                    + x86("je",  plchi(i, "pre")); })
                                     + x86("jmp", plch("exhausted"))
                                     + FOR(0, _.resolve_choice_n, [](int i) { return x86("label", plchi(i, "pre"))
                                                                                    + x86("call", "resolve_cp_current@PLT")
                                                                                    + IF(i > 0, x86("mov", "edi", "[rax + 16]")
                                                                                              + x86("call", "rt_trail_unwind@PLT")
                                                                                              + x86("call", "resolve_cp_current@PLT"))
                                                                                    + x86("inc", "dword ptr [rax + 48]")
                                                                                    + x86("jmp",  plchi(i, "body")); })
                                     + x86("label", plch("exit_γ"))
                                     + x86("call", "rt_get_cut_flag@PLT")
                                     + x86("test", "eax", "eax")
                                     + x86("jnz",  plch("cut_γ"))
                                     + x86("call", "resolve_cp_current@PLT")
                                     + x86("mov", "rdi", "rax")
                                     + x86("call", "rt_choice_cut_exit@PLT")
                                     + x86("jmp",  _.lbl_γ)
                                     + x86("label", plch("cut_γ"))
                                     + x86("call", "resolve_cp_current@PLT")
                                     + x86("mov", "rdi", "rax")
                                     + x86("call", "rt_choice_cut_unwind@PLT")
                                     + x86("jmp",  _.lbl_γ)
                                     + x86("label", plch("cut_ω"))
                                     + x86("call", "resolve_cp_current@PLT")
                                     + x86("mov", "rdi", "rax")
                                     + x86("call", "rt_choice_cut_unwind@PLT")
                                     + x86("jmp",  _.lbl_ω)
                                     + x86("label", plch("exhausted"))
                                     + x86("call", "resolve_cp_current@PLT")
                                     + x86("mov", "rdi", "rax")
                                     + x86("call", "rt_choice_cut_exit@PLT")
                                     + x86("call", "resolve_cp_current@PLT")
                                     + x86("mov", "edi", "[rax + 16]")
                                     + x86("call", "rt_trail_unwind@PLT")
                                     + x86("call", "resolve_cp_pop@PLT")
                                     + x86("jmp",  _.lbl_ω)
                                     + x86("label", _.lbl_β)
                                     + x86("call", "rt_get_cut_flag@PLT")
                                     + x86("test", "eax", "eax")
                                     + x86("jnz",  plch("cut_ω"))
                                     + x86("call", "resolve_cp_current@PLT")
                                     + x86("test", "rax", "rax")
                                     + x86("je",   plch("β_nosol"))
                                     + x86("mov", "rdi", "rax")
                                     + x86("call", "rt_choice_cut_enter@PLT")
                                     + x86("jmp",  plch("dispatch"))
                                     + x86("def", "β") + x86("jmp", "ω")));
}
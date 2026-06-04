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
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — RESOLVE_CHOICE");
        if (MEDIUM_TEXT) {
            int id = _.resolve_choice_id;
            int n  = _.resolve_choice_n;
            if (n <= 0) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_2asm("jmp", _.lbl_ω)
                     + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            char disp[160];      snprintf(disp,      sizeof disp,      ".Lplch%d_dispatch",  id);
            char exit_γ[160];    snprintf(exit_γ,    sizeof exit_γ,    ".Lplch%d_exit_γ",    id);
            char exhausted[160]; snprintf(exhausted,  sizeof exhausted, ".Lplch%d_exhausted", id);
            char cut_unwind_ω[160]; snprintf(cut_unwind_ω, sizeof cut_unwind_ω, ".Lplch%d_cut_ω", id);
            char cut_unwind_γ[160]; snprintf(cut_unwind_γ, sizeof cut_unwind_γ, ".Lplch%d_cut_γ", id);
            char β_nosol[160];      snprintf(β_nosol,      sizeof β_nosol,      ".Lplch%d_β_nosol", id);
            std::string out = s_1asm(emit_fmt("%s:", _.lbl_α))
                            + s_comment(emit_fmt("# BOX RESOLVE_CHOICE n=%d (heap cursor, cut save/restore)", n))
                            + s_2asm("call", "rt_env_current@PLT")
                            + s_2asm("mov",  "rdx, rax")
                            + s_2asm("call", "rt_trail_mark@PLT")
                            + s_2asm("mov",  "rsi, rax")
                            + s_2asm("xor",  "edi, edi")
                            + s_2asm("xor",  "ecx, ecx")
                            + s_2asm("xor",  "r8d, r8d")
                            + s_2asm("call", "resolve_cp_push@PLT")
                            + s_2asm("mov",  "rdi, rax")
                            + s_2asm("call", "rt_choice_cut_enter@PLT");
            /* dispatch: cursor → next clause's pre[]. Flag check moved to β/exit_γ entries (the flag */
            /* is only meaningful BEFORE we re-enter cut scope; here we've just cleared it via _enter). */
            out += s_1asm(emit_fmt("%s:", disp))
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "edi, [rax + 48]")
                 + s_2asm("cmp",  emit_fmt("edi, %d", n))
                 + s_2asm("jge",  exhausted);
            for (int i = 0; i < n; i++) {
                char prei[160]; resolve_choice_clause_label(prei, sizeof prei, id, i, "pre");
                out += s_2asm("cmp", emit_fmt("edi, %d", i))
                     + s_2asm("je",  prei);
            }
            out += s_2asm("jmp", exhausted);
            /* pre[0]: no unwind (already at entry mark), cursor++, jmp body[0] */
            char body0[160]; resolve_choice_clause_label(body0, sizeof body0, id, 0, "body");
            char pre0[160];  resolve_choice_clause_label(pre0,  sizeof pre0,  id, 0, "pre");
            out += s_1asm(emit_fmt("%s:", pre0))
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("inc",  "dword ptr [rax + 48]")
                 + s_2asm("jmp",  body0);
            /* pre[i>0]: unwind trail to cp->trail_mark, cursor++, jmp body[i] */
            for (int i = 1; i < n; i++) {
                char prei[160], bodyi[160];
                resolve_choice_clause_label(prei,  sizeof prei,  id, i, "pre");
                resolve_choice_clause_label(bodyi, sizeof bodyi, id, i, "body");
                out += s_1asm(emit_fmt("%s:", prei))
                     + s_2asm("call", "resolve_cp_current@PLT")
                     + s_2asm("mov",  "edi, [rax + 16]")
                     + s_2asm("call", "rt_trail_unwind@PLT")
                     + s_2asm("call", "resolve_cp_current@PLT")
                     + s_2asm("inc",  "dword ptr [rax + 48]")
                     + s_2asm("jmp",  bodyi);
            }
            /* exit_γ: clause body succeeded. Cut-check: if `!` fired during body, restore outer cut    */
            /* state and truncate cp (no further clauses available). Else restore outer cut state and  */
            /* keep cp alive for caller's β redo. Either way, jmp γ_in.                                 */
            out += s_1asm(emit_fmt("%s:", exit_γ))
                 + s_2asm("call", "rt_get_cut_flag@PLT")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("jnz",  cut_unwind_γ)
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "rdi, rax")
                 + s_2asm("call", "rt_choice_cut_exit@PLT")
                 + s_2asm("jmp",  _.lbl_γ);
            /* cut_unwind_γ: cut fired, body succeeded → restore + truncate to cp->parent → jmp γ_in.  */
            out += s_1asm(emit_fmt("%s:", cut_unwind_γ))
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "rdi, rax")
                 + s_2asm("call", "rt_choice_cut_unwind@PLT")
                 + s_2asm("jmp",  _.lbl_γ);
            /* cut_unwind_ω: cut fired then body failed → restore + truncate to cp->parent → jmp ω_in. */
            out += s_1asm(emit_fmt("%s:", cut_unwind_ω))
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "rdi, rax")
                 + s_2asm("call", "rt_choice_cut_unwind@PLT")
                 + s_2asm("jmp",  _.lbl_ω);
            /* exhausted: all clauses tried, none succeeded. Restore outer cut state, unwind trail to  */
            /* cp's mark, pop cp, jmp ω_in. cut state SHOULD already be 0 here (would have been caught */
            /* at dispatch otherwise), but restoring unconditionally keeps the invariant clean.        */
            out += s_1asm(emit_fmt("%s:", exhausted))
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "rdi, rax")
                 + s_2asm("call", "rt_choice_cut_exit@PLT")
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "edi, [rax + 16]")
                 + s_2asm("call", "rt_trail_unwind@PLT")
                 + s_2asm("call", "resolve_cp_pop@PLT")
                 + s_2asm("jmp",  _.lbl_ω);
            /* β: re-entry. Body[i].ω chains here on failure AND caller's IR_GOAL β chains here on  */
            /* redo. Distinguishing them is the cut flag's job: if the body just executed fired `!`, the */
            /* flag is set (cut_set was called inside; we haven't called _enter since), and we unwind to */
            /* ω_in. Otherwise (flag==0): if cp is NULL the outer cut truncated us → ω_in; else re-save */
            /* the (current outer) cut state into our cp and clear the flag for the next clause attempt. */
            out += s_1asm(emit_fmt("%s:", _.lbl_β))
                 + s_2asm("call", "rt_get_cut_flag@PLT")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("jnz",  cut_unwind_ω)
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("test", "rax, rax")
                 + s_2asm("je",   β_nosol)
                 + s_2asm("mov",  "rdi, rax")
                 + s_2asm("call", "rt_choice_cut_enter@PLT")
                 + s_2asm("jmp",  disp);
            out += s_L2asm(emit_fmt("%s:", β_nosol), "jmp", _.lbl_ω);
            return out;
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_choice(IR_t * pBB) { bb_emit_x86(bb_choice_str(pBB)); }

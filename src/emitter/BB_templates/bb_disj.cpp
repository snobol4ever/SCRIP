#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" {
void rt_pl_trail_mark_push(void);
void rt_pl_trail_unwind_top(void);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string disj_pre (int ci) { char b[160]; resolve_choice_clause_label(b, sizeof b, _.resolve_choice_id, ci, "pre");  return std::string(b); }
static std::string disj_body(int ci) { char b[160]; resolve_choice_clause_label(b, sizeof b, _.resolve_choice_id, ci, "body"); return std::string(b); }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_disj_str() {
    if (PLATFORM_X86) {
        int n = _.resolve_choice_n;
        if (n <= 0)
            return IF(MEDIUM_TEXT,
                      s_1asm(std::string(_.lbl_α) + ":")
                    + s_comment("# BOX RESOLVE_ALT (empty)  [x86() self-encoding]")
                    + s_2asm("jmp", _.lbl_ω)
                    + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
        return IF(MEDIUM_TEXT,
                  s_1asm(std::string(_.lbl_α) + ":")
                + s_comment(emit_fmt("# BOX RESOLVE_ALT n=%d (mode-4 first-solution)  [x86() self-encoding]", n))
                + s_2asm("jmp", disj_pre(0).c_str())
                + s_1asm(disj_pre(0) + ":")
                + x86("call", "rt_pl_trail_mark_push", (uint64_t)(uintptr_t)(void*)rt_pl_trail_mark_push)
                + s_2asm("jmp", disj_body(0).c_str())
                + FOR(1, n, [](int i){
                      return s_1asm(disj_pre(i) + ":")
                           + x86("call", "rt_pl_trail_unwind_top", (uint64_t)(uintptr_t)(void*)rt_pl_trail_unwind_top)
                           + s_2asm("jmp", disj_body(i).c_str()); })
                + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_disj(void) { bb_emit_x86(bb_disj_str()); }

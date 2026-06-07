#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" {
void rt_trail_mark_push(void);
void rt_trail_unwind_top(void);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string disj_pre (int ci) { char b[160]; resolve_choice_clause_label(b, sizeof b, _.resolve_choice_id, ci, "pre");  return std::string(b); }
static std::string disj_body(int ci) { char b[160]; resolve_choice_clause_label(b, sizeof b, _.resolve_choice_id, ci, "body"); return std::string(b); }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_disj_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                  IF(_.resolve_choice_n <= 0,
                      x86("label", _.lbl_α)
                    + x86("comment", "BOX RESOLVE_ALT (empty)  [x86() self-encoding]")
                    + x86("ins2", "jmp", _.lbl_ω)
                    + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω))
                + IF(_.resolve_choice_n > 0,
                      x86("label", _.lbl_α)
                    + x86("comment", "BOX RESOLVE_ALT n=" + std::to_string(_.resolve_choice_n) + " (mode-4 first-solution)  [x86() self-encoding]")
                    + x86("ins2", "jmp", disj_pre(0).c_str())
                    + x86("ins1", disj_pre(0) + ":")
                    + x86("call", "rt_trail_mark_push", (uint64_t)(uintptr_t)(void*)rt_trail_mark_push)
                    + x86("ins2", "jmp", disj_body(0).c_str())
                    + FOR(1, _.resolve_choice_n, [](int i){
                          return x86("ins1", disj_pre(i) + ":")
                               + x86("call", "rt_trail_unwind_top", (uint64_t)(uintptr_t)(void*)rt_trail_unwind_top)
                               + x86("ins2", "jmp", disj_body(i).c_str()); })
                    + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω)));
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_disj(void) { bb_emit_x86(bb_disj_str()); }

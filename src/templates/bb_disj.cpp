#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" {
void rt_trail_mark_push(void);
void rt_trail_unwind_top(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string disj_lbl(int ci, const char *tag) { char b[160]; resolve_choice_clause_label(b, sizeof b, _.resolve_choice_id, ci, tag); return std::string(b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_disj() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                  IF(_.resolve_choice_n <= 0,
                      x86("def",     "α")
                    + x86("jmp", "ω")
                    + x86("def", "β") + x86("jmp", "ω"))
                + IF(_.resolve_choice_n > 0,
                      x86("def",     "α")
                    + x86("comment", "IR_DISJ")
                    + x86("jmp", disj_lbl(0, "pre").c_str())
                    + x86("label", disj_lbl(0, "pre"))
                    + x86("call", "rt_trail_mark_push", (uint64_t)(uintptr_t)(void*)rt_trail_mark_push)
                    + x86("jmp", disj_lbl(0, "body").c_str())
                    + FOR(1, _.resolve_choice_n, [](int i){
                          return x86("label", disj_lbl(i, "pre"))
                               + x86("call", "rt_trail_unwind_top", (uint64_t)(uintptr_t)(void*)rt_trail_unwind_top)
                               + x86("jmp", disj_lbl(i, "body").c_str()); })
                    + x86("def", "β") + x86("jmp", "ω")));
    }
    return std::string();
}

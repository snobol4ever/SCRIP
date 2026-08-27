#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
void rt_gvar_assign_pat(const char *name, void *head);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_ref_invariant() {
    static char bn[24];
    strtab_label(bn, sizeof bn, _.op_sval ? _.op_sval : "");
    return x86("comment", "IR_REF_INVARIANT frozen-pattern store")
         + x86_alpha()
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), bn)
         + x86("lea",  "rsi", "[rip + __]", (uint64_t)(uintptr_t)_.bb_child_fn, _.bb_child_lbl ? _.bb_child_lbl : "")
         + x86_align_enter()
         + x86("call", "rt_gvar_assign_pat", (uint64_t)(uintptr_t)(void *)(void(*)(const char*,void*))rt_gvar_assign_pat)
         + x86_align_leave()
         + x86_gamma()
         + x86_pair_loop();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_ref_invariant_call(void) { bb_emit_x86(bb_ref_invariant()); }

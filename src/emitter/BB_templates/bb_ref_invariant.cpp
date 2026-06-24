#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
void rt_gvar_assign_pat(const char *name, void *head);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* FZ-2: IR_REF_INVARIANT store box. The fully-invariant pattern's matcher graph was sealed by the
   IR_REF_INVARIANT pre-pass (pre_build_children binary / pre_build_children_text text) and its head
   staged by flat_drive_ref_invariant into _.bb_child_fn (binary absolute ptr) / _.bb_child_lbl (text
   label). This box stores that frozen head as PAT's DT_P cell via rt_gvar_assign_pat(name, head),
   replacing the per-shape runtime builder call (bb_build_break_cap_lit_blob). Name = _.op_sval. The
   head load uses the RIPSEAL lea form (binary bakes the absolute bb_box_fn ptr; text emits lea [rip +
   label]) so the template carries no MEDIUM_* branch. β/ω are staged by flat_drive and flushed here. */
std::string bb_ref_invariant() {
    if (!PLATFORM_X86) return std::string();
    static char bn[24];
    strtab_label(bn, sizeof bn, _.op_sval ? _.op_sval : "");
    return x86("comment", "IR_REF_INVARIANT frozen-pattern store")
         + x86("label",   _.lbl_α)
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), bn)
         + x86("lea",  "rsi", "[rip + __]", (uint64_t)(uintptr_t)_.bb_child_fn, _.bb_child_lbl ? _.bb_child_lbl : "")
         + x86("push", "rbx")
         + x86("mov",  "rbx", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "rt_gvar_assign_pat", (uint64_t)(uintptr_t)(void *)(void(*)(const char*,void*))rt_gvar_assign_pat)
         + x86("mov",  "rsp", "rbx")
         + x86("pop",  "rbx")
         + x86("jmp",  "γ")
         + x86_pair_loop();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_ref_invariant_call(void) { bb_emit_x86(bb_ref_invariant()); }

#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
void rt_goto_transfer(const char *name);
void *rt_goto_resolve(const char *name);
extern int g_rt_fragment_emit;
int xa_flat_class_c_pred(void);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_goto_deferred_frame_release() {
    if (!g_rt_fragment_emit || !xa_flat_class_c_pred()) return std::string();
    return x86("comment", "GOTO_DEFERRED leaving a CLASS-C-framed runtime fragment (CODE()/EVAL transferring to a label outside itself): release the alpha-time carve first -- this exit never falls through the fragment's own gamma/omega epilogue to do it, so an unreleased carve sits on the stack and poisons whatever the destination reads off it next (RETURN's {gamma,omega} pop, most visibly)")
         + x86("add", "rsp", (long)_.flat_frame_bytes);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_goto_deferred() {
    x86_begin();
    { static int _df = -1; if (_df < 0)
        { const char * e = getenv("SCRIP_DEFINE_FOLD"); _df = (e && *e == '0') ? 0 : 1; }
    if (_df && _.op_ival == 1 && _.op_sval && _.op_sval[0] && _.op_sval[0] != '$') {
        return x86("comment", "IR_GOTO_DEFERRED (DEFINE-FOLD s55 ONE-SHOT: jmp body alpha, no chain, no reserve)")
             + x86_alpha()
             + bb_goto_deferred_frame_release()
             + x86("jmp", "[rip@cell + __]",
                   (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("body$") + _.op_sval).c_str()),
                   (std::string("LBL__") + _.op_sval).c_str())
             + x86_gamma();
    } }
    { static int _gt = -1; if (_gt < 0)
        { const char * e = getenv("SCRIP_GOTO_TAIL"); _gt = (e && *e == '0') ? 0 : 1; }
    if (_gt) {
        return x86("comment", "IR_GOTO_DEFERRED (TAIL-TRANSFER: resolve then jmp at the SITE's depth -- arm 1's shape with a runtime-resolved target)")
             + x86_alpha()
             + x86_align_enter()
             + x86_ro_load_q("rdi", 0)
             + x86("call", "rt_goto_resolve", (uint64_t)(uintptr_t)(void *)rt_goto_resolve)
             + x86_align_leave()
             + x86("test", "rax", "rax")
             + x86_jcc_id("jz", 1)
             + IF(_.op_zgpop > 0, x86("add", "rsp", (long)_.op_zgpop))
             + bb_goto_deferred_frame_release()
             + x86("jmp", "rax")
             + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "")
             + x86_deflabel_id(1)
             + x86_gamma();
    } }
    return x86("comment", "IR_GOTO_DEFERRED")
         + x86_alpha()
         + x86_align_enter()
         + x86_ro_load_q("rdi", 0)
         + x86("call", "rt_goto_transfer", (uint64_t)(uintptr_t)(void *)rt_goto_transfer)
         + x86_align_leave()
         + x86_jmp_id(1)
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "")
         + x86_deflabel_id(1)
         + x86_gamma();
}

#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
void rt_goto_transfer(const char *name);
void *rt_goto_resolve(const char *name);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_goto_deferred() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    { static int _df = -1; if (_df < 0) { const char * e = getenv("SCRIP_DEFINE_FOLD"); _df = (e && *e == '0') ? 0 : 1; }
    if (_df && _.op_ival == 1 && _.op_sval && _.op_sval[0] && _.op_sval[0] != '$') {
        std::string lbl = std::string("LBL__") + _.op_sval;
        uint64_t body_cell = (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("body$") + _.op_sval).c_str());
        return x86("comment", "IR_GOTO_DEFERRED (DEFINE-FOLD s55 ONE-SHOT: jmp body alpha, no chain, no reserve)")
             + x86_alpha()
             + x86("jmp", "[rip@cell + __]", body_cell, lbl.c_str())
             + x86_gamma();
    } }
    { static int _gt = -1; if (_gt < 0) { const char * e = getenv("SCRIP_GOTO_TAIL"); _gt = (e && *e == '0') ? 0 : 1; }
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

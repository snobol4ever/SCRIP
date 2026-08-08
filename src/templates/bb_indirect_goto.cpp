#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void *rt_pl_retry_pop(void);
void *rt_pl_cp_pop(void);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_indirect_goto() {
    x86_begin();
    if (!PLATFORM_X86 || _.op_off < 0) return x86_alpha() + x86_bomb("bb_indirect_goto: no label-variable slot (op_off<0)");
    /* PL-ZK-4: ZD arm -- pop retry continuation from g_pl_retry (WAM ALTB-read equivalent) and jump.  rt_pl_retry_pop() returns 0 when the stack is exhausted (all clauses tried) -- in that case jmp to omega (clause selection failed).  The popped address is the α of call_proc_staged; re-entering α re-stages args and calls rt_proc_call_open_det which advances the internal clause cursor.  SN4/Icon watermarks: _.op_zres=0 for non-pl_cells graphs -- byte-identical via the legacy FRQ(op_off+16) path. */
    if (_.op_zres) {
        uint64_t pop_fp; { void *(*fp)(void) = rt_pl_retry_pop; pop_fp = (uint64_t)(uintptr_t)(void *)fp; }
        return x86("comment", "IR_INDIRECT_GOTO (ZD/cells: retry stack pop)")
             + x86_alpha()
             + x86("call", "rt_pl_retry_pop", pop_fp)
             + x86("test", "rax", "rax")
             + x86_omega("je")
             + x86("jmp", "rax")
             + x86_beta_trampoline();
    }
    /* PL-FR-4 ZFRAME arm: pop the β-resume address from g_pl_cp_stack and jump.  Mirrors bb_move_label's zframe push arm: the address written there via rt_pl_cp_push is consumed here by rt_pl_cp_pop.  0 = exhausted choice point = all clauses tried = failure → jump to omega.  SEPARATE FROM g_pl_retry (cells track): the cells arm pops the callee α for rt_proc_call_open_det re-entry; this arm pops the β-resume directly.  SN4/Icon watermarks: g_emit.zframe_graph=0 for non-zframe graphs — byte-identical via the legacy FRQ(op_off+16) path below.  Killswitch SCRIP_PL_ZFRAME=0 leaves zframe_graph=0 → legacy path. */
    if (g_emit.zframe_graph) {
        uint64_t cp_fp; { void *(*fp)(void) = rt_pl_cp_pop; cp_fp = (uint64_t)(uintptr_t)(void *)fp; }
        return x86("comment", "IR_INDIRECT_GOTO (zframe: cp_stack pop → jmp)")
             + x86_alpha()
             + x86("call", "rt_pl_cp_pop", cp_fp)
             + x86("test", "rax", "rax")
             + x86_omega("je")
             + x86("jmp", "rax")
             + x86_beta_trampoline();
    }
    return x86("comment", "IR_INDIRECT_GOTO alt-resume: jmp *t")
         + x86_alpha()
         + x86("jmp", FRQ(_.op_off + 16))
         + x86_beta_trampoline();
}

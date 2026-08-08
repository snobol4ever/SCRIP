#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void *rt_pl_retry_pop(void);
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
    return x86("comment", "IR_INDIRECT_GOTO alt-resume: jmp *t")
         + x86_alpha()
         + x86("jmp", FRQ(_.op_off + 16))
         + x86_beta_trampoline();
}

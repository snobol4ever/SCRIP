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
    if (_.op_off < 0) return x86_alpha() + x86_bomb("bb_indirect_goto: no label-variable slot (op_off<0)");
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

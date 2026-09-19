#include <string>
#include <stdint.h>
#include <cstdlib>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/rt/rt_coexpr.h"
}
#include "x86_asm.h"
static inline bool xa_switch_record_planted() { const char * e = getenv("SCRIP_GC_SWITCH_RECORD_PLANT"); return e && *e == '1'; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_activate() {
    x86_begin();
    const char * pn = _.op_activate_proc ? _.op_activate_proc : "main";
    return IF(_.op_off < 0, x86_alpha() + x86_bomb("bb_activate: op_off < 0 (no slot assigned -- IR_ACTIVATE missing from ir_node_produces_value?)"))
         + IF(_.op_off >= 0 && _.op_sa < 0, x86_alpha() + x86_bomb("bb_activate: no coexpression operand slot (operand[0] unregistered -- LOWER/BFS wiring bug)"))
         + IF(_.op_off >= 0 && _.op_sa >= 0,
               x86("comment", "IR_ACTIVATE")
             + x86_alpha()
             + x86("mov",  "rdi", FRQ(_.op_sa + 8))
             + IF(_.op_sb >= 0,
                   x86("mov", "rsi", FRQ(_.op_sb))
                 + x86("mov", "rdx", FRQ(_.op_sb + 8)))
             + IF(_.op_sb < 0,
                   x86("xor", "esi", "esi")
                 + x86("xor", "edx", "edx"))
             + x86("lea",  "rcx", FRQ(_.op_off))
             + x86_load_ro_str("r8", pn)
             + x86("comment", "ARCH-GC 6.2f SWITCH RECORD (Lon 2026-09-19: r13 for SNOBOL4 and Icon points to the subject string): the subject register is spilled as ONE tagged DESCR cell {DT_S, slen=r15d, s=r13} on this thread's own spine before the switch and reloaded after it, because a collection on the other thread cannot rewrite a parked thread's register -- the cell is what the walker relocates; until it existed the register survived only because glibc's sem_wait slow path pushes r13 and the whole-stack sweep rewrote that libc word")
             + IF(!xa_switch_record_planted(), x86("sub", "rsp", (long)16)
             + x86_rsp_store32_imm(0, (long)DT_S)
             + x86_rsp_store32(4, "r15d")
             + x86_rsp_store64(8, "r13"))
             + x86("call", "scrip_coexpr_activate", (uint64_t)(uintptr_t)(void *)scrip_coexpr_activate)
             + IF(!xa_switch_record_planted(), x86_rsp_load64("r13", 8)
             + x86("add", "rsp", (long)16))
             + x86("test", "rax", "rax")
             + x86_omega("je")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline());
}

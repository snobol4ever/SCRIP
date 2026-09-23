#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" void rt_trace_stmt(long line);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_statement() {
    x86_begin();
    return  x86_alpha()
         + IF(x86_diag_regs_on() && !emit_diag_regs_suppress(), x86("comment", "DIAG r10: SNOBOL4 statement number, telemetry only -- see ARCH-SNOBOL4-RTX.md #2")
                                                                + x86("mov", "r10", (long)_.op_stno))
         + IF(_.op_mon_stmt_tap, x86("comment", "MON LABEL tap")
                                + x86("mov", "rdi", (long)_.op_stno)
                                + x86("call", "rt_trace_stmt", (uint64_t)(uintptr_t)(void *)rt_trace_stmt))
         + x86_gamma()
         + x86_beta_trampoline();
}

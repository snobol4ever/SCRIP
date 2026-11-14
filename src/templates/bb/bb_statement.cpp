#include <string>
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state_.h"
}
#include "templates/x86/x86_ops.h"
extern "C" void rt_trace_stmt(long line);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_statement() {
    x86_begin();
    return x86_alpha()
         + IF(_.op_mon_stmt_tap, x86("comment", "MON LABEL tap")
                                + x86("mov", "rdi", (long)_.op_stno)
                                + x86("call", "rt_trace_stmt", (uint64_t)(uintptr_t)(void *)rt_trace_stmt))
         + x86_gamma()
         + x86_beta_trampoline();
}

#include <string>
#include "runtime/core/g_runtime.h"
#include "runtime/core/g_runtime_eval.h"
extern "C" {
#include "emitter/emit_state_.h"
}
#include "templates/x86/x86_ops.h"
extern "C" void rt_trace_stmt(long line);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_succeed() {
        return x86("comment", "IR_SUCCEED")
             + x86_alpha()
             + x86_gamma()
             + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_mon_label_tap(int32_t stno) {
    if (!g_rt_eval.rt_fragment_emit && stno > g_runtime.abi.mon_max_stno) g_runtime.abi.mon_max_stno = stno;
    _.op_stno = stno;
    bb_emit_x86(
          x86("comment", "MON LABEL tap")
        + x86("mov", "rdi", (long)_.op_stno)
        + x86("call", "rt_trace_stmt", (uint64_t)(uintptr_t)(void *)rt_trace_stmt));
}

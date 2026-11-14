#include <string>
#include <cstdint>
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state_.h"
#include "ir/descr.h"
void rt_coerce_real_d(const DESCR_t *in, DESCR_t *out, long codes);
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_gc.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_coerce_real() {
    x86_begin();
    return IF(_.op_sa < 0 || _.op_off < 0, x86_bomb("bb_coerce_real: needs operand slot (op_sa) + own value slot (op_off)"))
         + IF(!(_.op_sa < 0 || _.op_off < 0),
             x86("comment", "IR_COERCE_REAL")
           + x86_alpha()
           + x86("lea", "rdi", FRQ(_.op_sa))
           + x86("lea", "rsi", FRQ(_.op_off))
           + x86("mov", "rdx", (long)_.op_ival)
           + x86("call", "rt_coerce_real_d", (uint64_t)(uintptr_t)(void *)rt_coerce_real_d)
           + x86_rt_gc_poll()
           + x86_gamma()
           + x86_beta_trampoline());
}

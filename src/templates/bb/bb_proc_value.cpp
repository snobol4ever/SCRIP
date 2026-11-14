#include <string>
#include <stdint.h>
extern "C" {
#include "emitter/emit_state_.h"
#include "ir/descr.h"
extern DESCR_t rt_proc_value(const char *name);
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_jump.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_proc_value() {
    x86_begin();
    return (_.op_off < 0 || !_.op_sval) ? x86_alpha() + x86_bomb("bb_proc_value: needs own slot + baked proc name")
         : x86("comment", "IR_PROC_VALUE")
         + x86_alpha()
         + x86("mov", "rdi", ROQ(0))
         + x86("call", "rt_proc_value", (uint64_t)(uintptr_t)(void *)rt_proc_value)
         + x86("mov", FRQ(_.op_off), "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline()
         + x86(".quad_ro", L(0), LS(0), _.op_sval)
         + x86("label", LS(0))
         + x86(".string", _.op_sval);
}

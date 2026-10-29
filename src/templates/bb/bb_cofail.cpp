#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/rt/rt_coexpr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cofail() {
    x86_begin();
    return x86("comment", "IR_COFAIL exhausted")
         + x86_alpha()
         + x86("call", "rt_co_env", (uint64_t)(uintptr_t)(void *)rt_co_env)
         + x86("mov", "[rax + 0]", "r13")
         + x86("mov", "[rax + 8]", "r14")
         + x86("mov", "[rax + 16]", "r15")
         + x86("call", "scrip_cofail", (uint64_t)(uintptr_t)(void *)scrip_cofail)
         + x86_rt_gc_poll()
         + x86_omega();
}

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
         + x86("call", "scrip_cofail", (uint64_t)(uintptr_t)(void *)scrip_cofail)
         + x86_omega();
}

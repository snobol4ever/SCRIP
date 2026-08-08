#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t str_repeat_d(DESCR_t s, DESCR_t n);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int bxr_ok() { return _.op_off >= 0 && _.op_ival == BINOP_XREP && _.op_sa >= 0 && _.op_sb >= 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_xrep_slot() {
    return IF(PLATFORM_X86 && bxr_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_XREP")
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("call_rt", "str_repeat_d", (long)_.op_off, (uint64_t)(uintptr_t)(void*)str_repeat_d)
         + x86_gamma()
         + x86_beta_trampoline());
}

#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int bcs_ok() { return _.op_off >= 0 && _.op_ival == BINOP_CONCAT && _.op_sa >= 0 && _.op_sb >= 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_concat_slot() {
    return IF(PLATFORM_X86 && bcs_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_CONCAT")
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("call", "str_concat_d", (uint64_t)(uintptr_t)(void*)str_concat_d)
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline());
}

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
DESCR_t str_concat_fracdigit_d(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int bcs_ok() { return _.op_off >= 0 && binop_is_concat((long)_.op_ival) && _.op_sa >= 0 && _.op_sb >= 0; }
static inline const char *bcs_rt_name() { return _.op_ival == BINOP_CONCAT_FRACDIGIT ? "str_concat_fracdigit_d" : "str_concat_d"; }
static inline void *bcs_rt_addr() { return _.op_ival == BINOP_CONCAT_FRACDIGIT ? (void*)str_concat_fracdigit_d : (void*)str_concat_d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_concat_slot() {
    if (PLATFORM_X86 && _.op_zres)
        return x86("comment", "IR_BINOP_CONCAT zd")
             + x86_alpha()
             + x86("note", ZOPN(0)) + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0)) + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1)) + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1)) + x86("mov", "rcx", ZOPQ(1, 8))
             + x86("rtcc_wb")
             + x86("call_bare", bcs_rt_name(), (uint64_t)(uintptr_t)bcs_rt_addr())
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86("rtcc_rl")
             + x86_gamma()
             + x86_beta_trampoline();
    return IF(PLATFORM_X86 && bcs_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_CONCAT")
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("call_rt", bcs_rt_name(), (long)_.op_off, (uint64_t)(uintptr_t)bcs_rt_addr())
         + x86_gamma()
         + x86_beta_trampoline());
}

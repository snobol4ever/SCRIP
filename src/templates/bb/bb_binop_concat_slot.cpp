#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t str_concat_d(DESCR_t a, DESCR_t b); DESCR_t sno_concat_d(DESCR_t a, DESCR_t b);
DESCR_t str_concat_fracdigit_d(DESCR_t a, DESCR_t b);
DESCR_t rt_icn_lconcat_d(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
#include <cstdlib>
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int bcs_ok() { return _.op_off >= 0 && binop_is_concat((long)_.op_ival) && _.op_sa >= 0 && _.op_sb >= 0; }
static inline const char *bcs_rt_name() { return _.op_ival == BINOP_LCONCAT ? "rt_icn_lconcat_d" : _.op_ival == BINOP_CONCAT_FRACDIGIT ? "str_concat_fracdigit_d" : _.op_ival == BINOP_CONCAT_SNO ?
    "sno_concat_d" : "str_concat_d"; }
static inline void *bcs_rt_addr() { return _.op_ival == BINOP_LCONCAT ? (void*)rt_icn_lconcat_d : _.op_ival == BINOP_CONCAT_FRACDIGIT ? (void*)str_concat_fracdigit_d : _.op_ival == BINOP_CONCAT_SNO ?
    (void*)sno_concat_d : (void*)str_concat_d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int bcs_null_side() {
    return (getenv("SCRIP_OPT_NULLCAT") && getenv("SCRIP_OPT_NULLCAT")[0] == '0') ? -1
         : (_.op_ival == BINOP_CONCAT_FRACDIGIT || _.op_ival == BINOP_LCONCAT) ? -1 : _.op_snul_a_ok ? 1 : _.op_snul_b_ok ? 0 : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define BCS_NI() (_.op_zres && bcs_null_side() >= 0)
#define BCS_NI2() (!_.op_zres && bcs_ok() && bcs_null_side() >= 0)
std::string bb_binop_concat_slot() {
    return IF(BCS_NI(),
               x86("comment", "IR_BINOP_CONCAT zd null-identity")
             + x86_alpha()
             + x86("note", ZOPN(bcs_null_side()))
             + x86("mov", "rax", ZOPQ(bcs_null_side(), 0))
             + x86("note", ZOPN(bcs_null_side()))
             + x86("mov", "rdx", ZOPQ(bcs_null_side(), 8))
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!BCS_NI() && BCS_NI2(),
               x86_alpha()
             + x86("comment", "IR_BINOP_CONCAT null-identity")
             + x86("mov", "rax", FRQ(bcs_null_side() ? _.op_sb : _.op_sa))
             + x86("mov", "rdx", FRQ((bcs_null_side() ? _.op_sb : _.op_sa) + 8))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!BCS_NI() && !BCS_NI2() && _.op_zres,
               x86("comment", "IR_BINOP_CONCAT zd")
             + x86_alpha()
             + x86("note", ZOPN(0))
             + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0))
             + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1))
             + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1))
             + x86("mov", "rcx", ZOPQ(1, 8))
             + x86("call", bcs_rt_name(), (uint64_t)(uintptr_t)bcs_rt_addr())
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86_rt_gc_poll()
             + IF(_.op_ival == BINOP_LCONCAT || _.op_ival == BINOP_CONCAT_SNO, x86("note", ZRESN())
             + x86("mov", "eax", ZRESD(0))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je"))
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!BCS_NI() && !BCS_NI2() && !_.op_zres && bcs_ok(),
               x86_alpha()
             + x86("comment", "IR_BINOP_CONCAT")
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov", "rdx", FRQ(_.op_sb))
             + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("call_rt", bcs_rt_name(), (long)_.op_off, (uint64_t)(uintptr_t)bcs_rt_addr())
             + x86_rt_gc_poll()
             + IF(_.op_ival == BINOP_LCONCAT || _.op_ival == BINOP_CONCAT_SNO, x86("mov", "eax", FR(_.op_off))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je"))
             + x86_gamma()
             + x86_beta_trampoline());
}

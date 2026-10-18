#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t subscript_get2(DESCR_t arr, DESCR_t i, DESCR_t j);
extern DESCR_t subscript_get2_strict(DESCR_t, DESCR_t, DESCR_t);
extern DESCR_t subscript_get2_ext(DESCR_t arr, DESCR_t i, DESCR_t end);
extern DESCR_t subscript_get2_ext_strict(DESCR_t, DESCR_t, DESCR_t);
extern DESCR_t rt_section_var(DESCR_t base, DESCR_t i1, DESCR_t i2);
extern DESCR_t rt_section_var_strict(DESCR_t, DESCR_t, DESCR_t);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SEC_ARGS() \
    (IF(_.op_zres, \
         x86("note", ZOPN(0)) \
       + x86("mov", "rdi", ZOPQ(0, 0)) \
       + x86("note", ZOPN(0)) \
       + x86("mov", "rsi", ZOPQ(0, 8)) \
       + x86("note", ZOPN(1)) \
       + x86("mov", "rdx", ZOPQ(1, 0)) \
       + x86("note", ZOPN(1)) \
       + x86("mov", "rcx", ZOPQ(1, 8)) \
       + x86("note", ZOPN(2)) \
       + x86("mov", "r8", ZOPQ(2, 0)) \
       + x86("note", ZOPN(2)) \
       + x86("mov", "r9", ZOPQ(2, 8))) \
   + IF(!_.op_zres, \
         x86("mov", "rdi", FRQ(_.op_a_slot)) \
       + x86("mov", "rsi", FRQ(_.op_a_slot + 8)) \
       + x86("mov", "rdx", FRQ(_.op_sa)) \
       + x86("mov", "rcx", FRQ(_.op_sa + 8)) \
       + x86("mov", "r8", FRQ(_.op_sb)) \
       + x86("mov", "r9", FRQ(_.op_sb + 8))))
#define SEC_RESULT() \
    (IF(_.op_zres, \
         x86("note", ZRESN()) \
       + x86("mov", ZRES(0), "rax") \
       + x86("note", ZRESN()) \
       + x86("mov", ZRES(8), "rdx")) \
   + IF(!_.op_zres, \
         x86("mov", FRQ(_.op_off), "rax") \
       + x86("mov", FRQ(_.op_off + 8), "rdx")))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_section() {
        return (!_.op_zres && (_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0 || _.op_sb < 0))
             ? x86_alpha() + x86_bomb("bb_section: needs own slot + base/i1/i2 operand slots")
             : (_.op_sval && _.op_sval[0] == 'l' && _.op_sval[1] == 'v' && !_.op_sval[2])
             ? x86("comment", "IR_SUBSCRIPT section VARIABLE (ASSIGN-LV: rt_section_var tvsubs trap; FAIL routes ω)")
             + x86_alpha()
             + SEC_ARGS()
             + x86("call", (_.op_strict ? "rt_section_var_strict" : "rt_section_var"), (uint64_t)(uintptr_t)(void *)(_.op_strict ? rt_section_var_strict : rt_section_var))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + SEC_RESULT()
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline()
             : (_.op_sval && (_.op_sval[0] == '+' || _.op_sval[0] == '-'))
             ? x86("comment", "IR_SUBSCRIPT section extended (x[i+:n]/x[i-:n]; end pre-computed by IR_BINOP; wraparound→ω)")
             + x86_alpha()
             + SEC_ARGS()
             + x86("call", (_.op_strict ? "subscript_get2_ext_strict" : "subscript_get2_ext"), (uint64_t)(uintptr_t)(void *)(_.op_strict ? subscript_get2_ext_strict : subscript_get2_ext))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + SEC_RESULT()
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline()
             : x86("comment", "IR_SUBSCRIPT section")
             + x86_alpha()
             + SEC_ARGS()
             + x86("call", (_.op_strict ? "subscript_get2_strict" : "subscript_get2"), (uint64_t)(uintptr_t)(void *)(_.op_strict ? subscript_get2_strict : subscript_get2))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + SEC_RESULT()
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline();
}

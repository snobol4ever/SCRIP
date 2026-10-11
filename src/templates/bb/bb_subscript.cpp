#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_subscript_var(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_subscript_var_strict(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_subscript_var_container_only(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_subscript_var_container_only_strict(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_subscript_val(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_subscript_val_strict(DESCR_t base, DESCR_t idx);
}
#include "x86_asm.h"
#define sub_val_on() emit_knob_unless_zero("SCRIP_SUB_VAL")
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define sub_cval() (_.op_sval && !strcmp(_.op_sval, "container-value"))
static int sub_conly(void) { return sub_cval() || (_.op_sval && !strcmp(_.op_sval, "container-only")); }
#define sub_vctx() ((sub_cval() && !_.op_strict && sub_val_on()) ? 1 : 0)
static const char * sub_open_sym(void) { return sub_vctx() ? "rt_subscript_val" : (sub_conly() ? (_.op_strict ? "rt_subscript_var_container_only_strict" : "rt_subscript_var_container_only")
    : (_.op_strict ? "rt_subscript_var_strict" : "rt_subscript_var")); }
#define SUB_OPEN_FN() (sub_vctx() ? (uint64_t)(uintptr_t)(void *)rt_subscript_val \
                      : (uint64_t)(uintptr_t)(void *)(sub_conly() ? (_.op_strict ? rt_subscript_var_container_only_strict : rt_subscript_var_container_only) \
                                                                   : (_.op_strict ? rt_subscript_var_strict : rt_subscript_var)))
#define sub_ival() (_.op_strict && _.op_seal == IR_SEAL_SUBSCRIPT_VALUE && !sub_conly())
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SUB_ARGS() \
    (IF(_.op_zres, \
         x86("note", ZOPN(0)) \
       + x86("mov", "rdi", ZOPQ(0, 0)) \
       + x86("note", ZOPN(0)) \
       + x86("mov", "rsi", ZOPQ(0, 8)) \
       + x86("note", ZOPN(1)) \
       + x86("mov", "rdx", ZOPQ(1, 0)) \
       + x86("note", ZOPN(1)) \
       + x86("mov", "rcx", ZOPQ(1, 8))) \
   + IF(!_.op_zres, \
         x86("mov", "rdi", FRQ(_.op_a_slot)) \
       + x86("mov", "rsi", FRQ(_.op_a_slot + 8)) \
       + x86("mov", "rdx", FRQ(_.op_sa)) \
       + x86("mov", "rcx", FRQ(_.op_sa + 8))))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SUB_CALL() \
    (IF(sub_ival(), \
         x86("xor", "eax", "eax") \
       + x86("def", L(1)) \
       + SUB_ARGS() \
       + x86("test", "eax", "eax") \
       + x86("jne", L(2)) \
       + x86("call", "rt_subscript_val_strict", (uint64_t)(uintptr_t)(void *)rt_subscript_val_strict) \
       + x86("cmp", "al", (long)RTX_NOT_HANDLED) \
       + x86("jne", L(4)) \
       + x86("mov", "eax", 1L) \
       + x86("jmp", L(1)) \
       + x86("def", L(4)) \
       + x86_rt_gc_poll_rec_res() \
       + x86("jmp", L(3)) \
       + x86("def", L(2)) \
       + x86("call", sub_open_sym(), SUB_OPEN_FN()) \
       + x86_rt_gc_poll_rec_res() \
       + x86("def", L(3))) \
   + IF(!sub_ival(), \
         SUB_ARGS() \
       + x86("call", sub_open_sym(), SUB_OPEN_FN())))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_subscript() {
    return IF(_.op_zres,
               x86("comment", "IR_SUBSCRIPT x[i] variable zd")
             + x86_alpha()
             + SUB_CALL()
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + IF(!sub_ival(), x86_rt_gc_poll())
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!_.op_zres && (_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0),
               x86_alpha() + x86_bomb("bb_subscript: needs own slot + base/index operand slots"))
         + IF(!_.op_zres && !(_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0),
               x86("comment", "IR_SUBSCRIPT x[i] variable")
             + x86_alpha()
             + SUB_CALL()
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + IF(!sub_ival(), x86_rt_gc_poll())
             + x86_gamma()
             + x86_beta_trampoline());
}

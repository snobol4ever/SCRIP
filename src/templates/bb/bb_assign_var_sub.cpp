#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t c_rt_table_assign_fast(DESCR_t base, DESCR_t idx, DESCR_t val);
extern DESCR_t rt_table_assign_fast(DESCR_t base, DESCR_t idx, DESCR_t val);
extern DESCR_t rt_subscript_var(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_subscript_var_strict(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_subscript_var_container_only(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_subscript_var_container_only_strict(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
extern DESCR_t rt_assign_var_strict(DESCR_t, DESCR_t);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string avs_base_deref() {
    return x86("comment", "a base that is a variable reference (DT_N, one cell) is looked through for the table fast path; the slow road keeps the reference (CEO-1641)")
         + x86("cmp", "dil", (long)DT_N)
         + x86("jne", L(5))
         + x86("mov", "rax", "rdi")
         + x86("shr", "rax", 32L)
         + x86("cmp", "eax", 1L)
         + x86("jne", L(0))
         + x86("test", "rsi", "rsi")
         + x86("je", L(0))
         + x86("mov", "rax", RDQ("rsi", 0))
         + x86("mov", "r10", RDQ("rsi", 8))
         + x86("cmp", "al", (long)DT_T)
         + x86("jne", L(0))
         + x86("mov", "rdi", "rax")
         + x86("mov", "rsi", "r10")
         + x86("jmp", L(1))
         + x86("def", L(5))
         + x86("cmp", "dil", (long)DT_T)
         + x86("je", L(1))
         + x86("cmp", "dil", (long)DT_A)
         + x86("jne", L(0))
         + x86("def", L(1))
         + x86("test", "rsi", "rsi")
         + x86("je", L(0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define AVS_CONLY() (_.op_sval && !strcmp(_.op_sval, "container-only"))
#define AVS_FN_NAME() (AVS_CONLY() ? (_.op_strict ? "rt_subscript_var_container_only_strict" : "rt_subscript_var_container_only") \
                               : (_.op_strict ? "rt_subscript_var_strict" : "rt_subscript_var"))
#define AVS_FN() ((uint64_t)(uintptr_t)(void *)(AVS_CONLY() ? (_.op_strict ? rt_subscript_var_container_only_strict : rt_subscript_var_container_only) \
                                                          : (_.op_strict ? rt_subscript_var_strict : rt_subscript_var)))
std::string bb_assign_var_sub() {
    x86_begin();
    return IF(_.op_zres,
               x86("comment", "IR_ASSIGN_VAR T[i]=v fused zd")
             + x86_alpha()
             + x86("note", ZOPN(0))
             + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0))
             + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1))
             + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1))
             + x86("mov", "rcx", ZOPQ(1, 8))
             + avs_base_deref()
             + x86("note", ZOPN(2))
             + x86("mov", "r8", ZOPQ(2, 0))
             + x86("note", ZOPN(2))
             + x86("mov", "r9", ZOPQ(2, 8))
             + x86("call", "rt_table_assign_fast", (uint64_t)(uintptr_t)(void *)rt_table_assign_fast)
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86("def", L(0))
             + x86("call", AVS_FN_NAME(), AVS_FN())
             + x86_rt_gc_poll_rec_res()
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + x86("note", ZOPN(2))
             + x86("mov", "rdx", ZOPQ(2, 0))
             + x86("note", ZOPN(2))
             + x86("mov", "rcx", ZOPQ(2, 8))
             + x86("call", (_.op_strict ? "rt_assign_var_strict" : "rt_assign_var"), (uint64_t)(uintptr_t)(void *)(_.op_strict ? rt_assign_var_strict : rt_assign_var))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!(_.op_zres),
               IF(_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0 || _.op_sb < 0, x86_alpha() + x86_bomb("bb_assign_var_sub: needs own slot + base/idx/value operand slots"))
         + IF(_.op_off >= 0 && _.op_a_slot >= 0 && _.op_sa >= 0 && _.op_sb >= 0,
               x86("comment", "IR_ASSIGN_VAR T[i]=v fused")
             + x86_alpha()
             + x86("mov", "rdi", FRQ(_.op_a_slot))
             + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
             + x86("mov", "rdx", FRQ(_.op_sa))
             + x86("mov", "rcx", FRQ(_.op_sa + 8))
             + avs_base_deref()
             + x86("mov", "r8", FRQ(_.op_sb))
             + x86("mov", "r9", FRQ(_.op_sb + 8))
             + x86("call", "rt_table_assign_fast", (uint64_t)(uintptr_t)(void *)rt_table_assign_fast)
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86("def", L(0))
             + x86("call", AVS_FN_NAME(), AVS_FN())
             + x86_rt_gc_poll_res()
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + x86("mov", "rdx", FRQ(_.op_sb))
             + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("call", (_.op_strict ? "rt_assign_var_strict" : "rt_assign_var"), (uint64_t)(uintptr_t)(void *)(_.op_strict ? rt_assign_var_strict : rt_assign_var))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline()));
}

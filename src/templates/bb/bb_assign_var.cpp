#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
extern DESCR_t rt_assign_var_strict(DESCR_t, DESCR_t);
extern DESCR_t rt_table_assign_fast(DESCR_t base, DESCR_t idx, DESCR_t val);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_assign_var_plain();
#define AV_LVTBL() (_.op_strict && _.op_seal == IR_SEAL_ASSIGN_LVTBL)
std::string bb_assign_var() {
    return IF(_.op_zres && AV_LVTBL(), x86_alpha() + x86_bomb("bb_assign_var: a table store in a zero-depth arm is reached by no corpus program; write the arm with its witness"))
         + IF(!_.op_zres && AV_LVTBL() && _.op_off >= 0 && _.op_a_slot >= 0 && _.op_sa >= 0 && _.op_sb >= 0,
               x86("comment", "IR_ASSIGN_VAR t[i]:=v table store")
             + x86_alpha()
             + x86("mov", "rdi", FRQ(_.op_a_slot))
             + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
             + x86("cmp", "dil", (long)DT_T)
             + x86("jne", L(0))
             + x86("test", "rsi", "rsi")
             + x86("je", L(0))
             + x86("mov", "rdx", FRQ(_.op_sb))
             + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("mov", "r8", FRQ(_.op_sa))
             + x86("mov", "r9", FRQ(_.op_sa + 8))
             + x86("call", "rt_table_assign_fast", (uint64_t)(uintptr_t)(void *)rt_table_assign_fast)
             + x86("jmp", L(1))
             + x86("def", L(0))
             + x86("mov", "rdx", FRQ(_.op_sa))
             + x86("mov", "rcx", FRQ(_.op_sa + 8))
             + x86("call", "rt_assign_var_strict", (uint64_t)(uintptr_t)(void *)rt_assign_var_strict)
             + x86("def", L(1))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!_.op_zres && AV_LVTBL() && !(_.op_off >= 0 && _.op_a_slot >= 0 && _.op_sa >= 0 && _.op_sb >= 0),
               x86_alpha() + x86_bomb("bb_assign_var: table store needs own slot + variable/value/index operand slots"))
         + IF(!AV_LVTBL(), bb_assign_var_plain());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_assign_var_plain() {
    return IF(_.op_zres,
               x86("comment", "IR_ASSIGN_VAR zd")
             + x86_alpha()
             + x86("note", ZOPN(0))
             + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0))
             + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1))
             + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1))
             + x86("mov", "rcx", ZOPQ(1, 8))
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
               IF(_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0, x86_alpha() + x86_bomb("bb_assign_var: needs own slot + variable/value operand slots"))
         + IF(_.op_off >= 0 && _.op_a_slot >= 0 && _.op_sa >= 0,
               x86("comment", "IR_ASSIGN_VAR")
             + x86_alpha()
             + x86("mov", "rdi", FRQ(_.op_a_slot))
             + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
             + x86("mov", "rdx", FRQ(_.op_sa))
             + x86("mov", "rcx", FRQ(_.op_sa + 8))
             + x86("call", (_.op_strict ? "rt_assign_var_strict" : "rt_assign_var"), (uint64_t)(uintptr_t)(void *)(_.op_strict ? rt_assign_var_strict : rt_assign_var))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline()));
}

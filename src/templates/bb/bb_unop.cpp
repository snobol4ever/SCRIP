#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "ast.h"
#include "descr.h"
struct DESCR_t rt_size_d(uint64_t lo, uint64_t hi);
struct DESCR_t rt_num_neg(struct DESCR_t a);
struct DESCR_t rt_num_pos(struct DESCR_t a);
struct DESCR_t rt_cset_compl(struct DESCR_t a);
struct DESCR_t rt_deref(struct DESCR_t d);
}
#include "x86_asm.h"
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unop() {
    if (_.op_zres && (_.op_node_kind == IR_UNOP))
        return x86("comment", "IR_UNOP zd")
             + x86_alpha()
             + x86("note", ZOPN(0))
             + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0))
             + x86("mov", "rsi", ZOPQ(0, 8))
             + IF((int)_.op_ival == TT_MNS,        x86("call", "rt_num_neg",   (uint64_t)(uintptr_t)(void *)rt_num_neg))
             + IF((int)_.op_ival == TT_PLS,        x86("call", "rt_num_pos",   (uint64_t)(uintptr_t)(void *)rt_num_pos))
             + IF((int)_.op_ival == TT_SIZE,       x86("call", "rt_size_d",    (uint64_t)(uintptr_t)(void *)rt_size_d))
             + IF((int)_.op_ival == TT_CSET_COMPL, x86("call", "rt_cset_compl",(uint64_t)(uintptr_t)(void *)rt_cset_compl))
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
        return !(_.op_off >= 0) ? std::string() :
               _.op_node_kind == IR_NULLTEST_VAR ?
               (_.op_sa < 0 ? x86_bomb("bb_unop lv: operand slot unresolved") :
               x86("comment", "IR_NULLTEST_VAR")
             + x86_alpha()
             + x86("mov", "eax", FR(_.op_sa))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("call", "rt_deref", (uint64_t)(uintptr_t)(void *)rt_deref)
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("cmp", "eax", (long)0)
             + (_.op_sval && !strcmp(_.op_sval, "nonnull") ? x86_omega("je") : x86_omega("jne"))
             + x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline()) :
               _.op_node_kind != IR_UNOP && _.op_node_kind != IR_UNOP_TEST ? std::string() :
               (int)_.op_ival != TT_MNS && (int)_.op_ival != TT_PLS && (int)_.op_ival != TT_SIZE
                   && (int)_.op_ival != TT_NONNULL && (int)_.op_ival != TT_NULL && (int)_.op_ival != TT_CSET_COMPL ? std::string() :
               _.op_sa < 0 ? x86_bomb("bb_unop: operand slot unresolved (LIT_F/NUL or non-slot producer)") :
               (int)_.op_ival == TT_NONNULL ?
               x86("comment", "IR_UNOP")
             + x86_alpha()
             + x86("mov", "eax", FR(_.op_sa))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("cmp", "eax", (long)0)
             + x86_omega("je")
             + x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline() :
               (int)_.op_ival == TT_NULL ?
               x86("comment", "IR_UNOP")
             + x86_alpha()
             + x86("mov", "eax", FR(_.op_sa))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("cmp", "eax", (long)0)
             + x86_omega("jne")
             + x86("mov", FRQ(_.op_off),     (long)0)
             + x86("mov", FRQ(_.op_off + 8), (long)0)
             + x86_gamma()
             + x86_beta_trampoline() :
               (int)_.op_ival == TT_CSET_COMPL ?
               x86("comment", "IR_UNOP")
             + x86_alpha()
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("call", "rt_cset_compl", (uint64_t)(uintptr_t)(void *)rt_cset_compl)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline() :
               (int)_.op_ival == TT_SIZE ?
               x86("comment", "IR_UNOP")
             + x86_alpha()
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("call", "rt_size_d", (uint64_t)(uintptr_t)(void *)rt_size_d)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline() :
               x86("comment", "IR_UNOP")
             + x86_alpha()
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + IF((int)_.op_ival == TT_MNS, x86("call", "rt_num_neg", (uint64_t)(uintptr_t)(void *)rt_num_neg))
             + IF((int)_.op_ival != TT_MNS, x86("call", "rt_num_pos", (uint64_t)(uintptr_t)(void *)rt_num_pos))
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    return std::string();
}

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
/* ZTOS-1 (Lon s21x-o "Do not put RSP references directly into the templates"): the private rspq helper is RETIRED -- its four call sites below now speak ZTOS, the sanctioned spine accessor in x86_asm.h.
 * This template is the first conversion and the measured witness: rspq spelled a bare [rsp+off] on the assumption that nothing was carved between the producer's push and this box's read, which stopped
 * being true the moment the universal per-BB carve armed this kind.  028_arith_unary_minus printed 5 for -5 because alpha's own sub rsp,16 slid the operand cell out from under rspq(0)/rspq(8): the box
 * negated its own uninitialised cell, stored the answer there, and popped it.  ZTOS adds op_zdepth, so the read follows the carve instead of being displaced by it. */
static inline int vfcu() { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0 && _.op_node_kind == IR_UNOP && ((int)_.op_ival == TT_MNS || (int)_.op_ival == TT_PLS); }   /* ZB-VAL-6b: registered value-spine unary minus/plus.  THE NET IS ZERO -- release the operand cell (16) and carve the result cell (16) cancel exactly, so the box emits NO rsp instruction and writes its result straight over its operand in place.  rt_num_neg/rt_num_pos own the string->number coercion and the null-string-is-0 identity (SPITBOL manual p.22); both are infallible calls, so there is no omega edge here */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unop() {
    if (PLATFORM_X86 && _.op_zres && (_.op_node_kind == IR_UNOP))
        return x86("comment", "IR_UNOP zd")
             + x86_alpha()
             + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("mov", "rsi", ZOPQ(0, 8))
             + IF((int)_.op_ival == TT_MNS,        x86("call", "rt_num_neg",   (uint64_t)(uintptr_t)(void *)rt_num_neg))
             + IF((int)_.op_ival == TT_PLS,        x86("call", "rt_num_pos",   (uint64_t)(uintptr_t)(void *)rt_num_pos))
             + IF((int)_.op_ival == TT_SIZE,       x86("call", "rt_size_d",    (uint64_t)(uintptr_t)(void *)rt_size_d))
             + IF((int)_.op_ival == TT_CSET_COMPL, x86("call", "rt_cset_compl",(uint64_t)(uintptr_t)(void *)rt_cset_compl))
             + x86("mov", ZRES(0), "rax")
             + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();   /* ZD-1 (Lon s21x-v): operand = the producer's suspended cell at the STAGED DIFFERENCE OF TWO DEPTHS (mode 3), result = the box's own alpha-carved cell (mode 1) -- no in-place net-zero trick, no rsp spelling, no release before gamma (the cell suspends for its consumer; omega's hook leave + op_wpop restore statement entry).  Supersedes vfcu for planner-armed nodes; the planner's whitelist is exactly this arm's op set. */
    if (PLATFORM_X86 && vfcu())
        return x86_alpha()
             + x86("comment", "IR_UNOP fc")
             + x86("mov", "rdi", ZTOS(0))
             + x86("mov", "rsi", ZTOS(8))
             + IF((int)_.op_ival == TT_MNS, x86("call", "rt_num_neg", (uint64_t)(uintptr_t)(void *)rt_num_neg))
             + IF((int)_.op_ival != TT_MNS, x86("call", "rt_num_pos", (uint64_t)(uintptr_t)(void *)rt_num_pos))
             + x86("mov", ZTOS(0), "rax")
             + x86("mov", ZTOS(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    if (PLATFORM_X86)
        return !(_.op_off >= 0) ? std::string() :
               _.op_node_kind == IR_NULLTEST_VAR ?
               (_.op_sa < 0 ? x86_bomb("bb_unop lv: operand slot unresolved") :
               x86("comment", "IR_NULLTEST_VAR")
             + x86_alpha()
             + x86("mov", "eax", FR(_.op_sa))
             + x86("cmp", "eax", (long)99)
             + x86_omega("je")
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("call", "rt_deref", (uint64_t)(uintptr_t)(void *)rt_deref)
             + x86("cmp", "eax", (long)99)
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
             + x86("cmp", "eax", (long)99)
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
             + x86("cmp", "eax", (long)99)
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

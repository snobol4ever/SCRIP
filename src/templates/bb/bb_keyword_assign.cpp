#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
struct DESCR_t rt_keyword_random_set(struct DESCR_t v);
struct DESCR_t rt_keyword_error_set(struct DESCR_t v);
struct DESCR_t rt_keyword_trace_set(struct DESCR_t v);
struct DESCR_t rt_keyword_dump_set(struct DESCR_t v);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define KA_OFFBAD() (!(_.op_off >= 0))
#define KA_SLOTBAD() (_.op_a_slot < 0)
#define KA_IS(s) (!strcmp(_.op_name2, s))
std::string bb_keyword_assign() {
    return IF(KA_OFFBAD(), x86_alpha() + x86_bomb("bb_keyword_assign: no result slot"))
         + IF(!KA_OFFBAD() && KA_SLOTBAD(), x86_alpha() + x86_bomb("bb_keyword_assign: rhs operand slot unresolved"))
         + IF(!KA_OFFBAD() && !KA_SLOTBAD() && KA_IS("pos"),
               x86("comment", "BOX ICN IR_KW_ASSIGN pos: &pos IS r14 + 1 (ARCH-ICON-RTX 10 R4) -- i <= 0 counts from the end, outside 1..Delta+1 fails, r14 = i - 1; result {DT_I, i}")
             + x86_alpha()
             + x86_icn_pos_set(_.op_a_slot, _.op_strict, 1)
             + x86("mov", FRQ(_.op_off), (long)DT_I)
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!KA_OFFBAD() && !KA_SLOTBAD() && !KA_IS("pos") && KA_IS("subject"),
               x86("comment",
                   "BOX ICN IR_KW_ASSIGN subject: cnv:str, then (r13, r15, r14) = (ptr, len, 0) -- the descriptor's bytes ARE the subject, no copy, no global (ARCH-ICON-RTX 10 R4); result the string")
             + x86_alpha()
             + x86_icn_subject_set(_.op_a_slot, _.op_strict ? 1 : 0, 1)
             + x86_icn_subject_result(_.op_off)
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!KA_OFFBAD() && !KA_SLOTBAD() && !KA_IS("pos") && !KA_IS("subject") && KA_IS("random"),
               x86("comment", "BOX ICN IR_KW_ASSIGN random [data.r kywdint: coerce int, store g_random seed, result {DT_I,n}; non-numeric->omega]")
             + x86_alpha()
             + x86("mov", "rdi", FRQ(_.op_a_slot))
             + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
             + x86("call", "rt_keyword_random_set", (uint64_t)(uintptr_t)(void *)rt_keyword_random_set)
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!KA_OFFBAD() && !KA_SLOTBAD() && !KA_IS("pos") && !KA_IS("subject") && !KA_IS("random") && KA_IS("error"),
               x86("comment", "BOX ICN IR_KW_ASSIGN error [coerce int, store g_error; non-numeric->omega]")
             + x86_alpha()
             + x86("mov", "rdi", FRQ(_.op_a_slot))
             + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
             + x86("call", "rt_keyword_error_set", (uint64_t)(uintptr_t)(void *)rt_keyword_error_set)
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!KA_OFFBAD() && !KA_SLOTBAD() && !KA_IS("pos") && !KA_IS("subject") && !KA_IS("random") && !KA_IS("error") && KA_IS("trace"),
               x86("comment", "BOX ICN IR_KW_ASSIGN trace [coerce int, store g_trace; non-numeric->omega]")
             + x86_alpha()
             + x86("mov", "rdi", FRQ(_.op_a_slot))
             + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
             + x86("call", "rt_keyword_trace_set", (uint64_t)(uintptr_t)(void *)rt_keyword_trace_set)
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!KA_OFFBAD() && !KA_SLOTBAD() && !KA_IS("pos") && !KA_IS("subject") && !KA_IS("random") && !KA_IS("error") && !KA_IS("trace") && KA_IS("dump"),
               x86("comment", "BOX ICN IR_KW_ASSIGN dump [coerce int, store g_dump; non-numeric->omega]")
             + x86_alpha()
             + x86("mov", "rdi", FRQ(_.op_a_slot))
             + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
             + x86("call", "rt_keyword_dump_set", (uint64_t)(uintptr_t)(void *)rt_keyword_dump_set)
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!KA_OFFBAD() && !KA_SLOTBAD() && !KA_IS("pos") && !KA_IS("subject") && !KA_IS("random") && !KA_IS("error") && !KA_IS("trace") && !KA_IS("dump"),
               x86_alpha() + x86_bomb("bb_keyword_assign: only &pos/&random assignment implemented (KEYWORD-LVALUE rung; &subject/:=:/<-> are follow-ons)"));
}

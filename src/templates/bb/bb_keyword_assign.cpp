#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int64_t rt_cvpos_pos(struct DESCR_t v, int64_t len);
int core_icn_int_ok_d(struct DESCR_t v);
int core_icn_error(int code, struct DESCR_t v);
typedef struct { uint64_t ptr; uint64_t len; } KwSubjRegs_t;
KwSubjRegs_t rt_icn_subject_coerce(uint64_t lo, uint64_t hi, uint64_t mode);
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
               x86("comment", "BOX ICN IR_KW_ASSIGN pos: &pos IS r14 + 1 (ARCH-ICON-RTX 10 R4) -- i <= 0 counts from the end (Delta + 1 + i), outside 1..Delta+1 fails, r14 = i - 1; result {DT_I, i}")
             + x86_alpha()
             + x86("mov", "rax", FRQ(_.op_a_slot))
             + x86("cmp", "eax", (long)DT_I)
             + x86("jne", L(1))
             + x86("mov", "rax", FRQ(_.op_a_slot + 8))
             + x86("mov", "rcx", "r15")
             + x86("add", "rcx", 1L)
             + x86("cmp", "rax", "rcx")
             + x86_omega("jg")
             + x86("mov", "rdx", "rax")
             + x86("add", "rdx", "r15")
             + x86_omega("js")
             + x86("test", "rax", "rax")
             + x86("jg", L(2))
             + x86("add", "rax", "rcx")
             + x86("jmp", L(2))
             + x86("def", L(1))
             + IF(_.op_strict,
                   x86("mov", "rdi", FRQ(_.op_a_slot))
                 + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
                 + x86("call", "core_icn_int_ok_d", (uint64_t)(uintptr_t)(void *)core_icn_int_ok_d)
                 + x86("test", "eax", "eax")
                 + x86("jne", L(3))
                 + x86("mov", "edi", 101L)
                 + x86("mov", "rsi", FRQ(_.op_a_slot))
                 + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
                 + x86("call", "core_icn_error", (uint64_t)(uintptr_t)(void *)core_icn_error)
                 + x86_omega()
                 + x86("def", L(3)))
             + x86("mov", "rdi", FRQ(_.op_a_slot))
             + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
             + x86("mov", "rdx", "r15")
             + x86("call", "rt_cvpos_pos", (uint64_t)(uintptr_t)(void *)rt_cvpos_pos)
             + x86("test", "rax", "rax")
             + x86_omega("je")
             + x86("def", L(2))
             + x86("mov", "r14", "rax")
             + x86("sub", "r14", 1L)
             + x86("mov", FRQ(_.op_off), (long)DT_I)
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!KA_OFFBAD() && !KA_SLOTBAD() && !KA_IS("pos") && KA_IS("subject"),
               x86("comment",
                   "BOX ICN IR_KW_ASSIGN subject: cnv:str, then (r13, r15, r14) = (ptr, len, 0) -- the descriptor's bytes ARE the subject, no copy, no global (ARCH-ICON-RTX 10 R4); result the string")
             + x86_alpha()
             + x86("mov", "rax", FRQ(_.op_a_slot))
             + x86("cmp", "eax", (long)DT_S)
             + x86("jne", L(1))
             + x86("shr", "rax", 32L)
             + x86("test", "eax", "eax")
             + x86("je", L(1))
             + x86("mov", "r13", FRQ(_.op_a_slot + 8))
             + x86("mov", "r15d", "eax")
             + x86("jmp", L(2))
             + x86("def", L(1))
             + x86("mov", "rdi", FRQ(_.op_a_slot))
             + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
             + x86("mov", "edx", (long)(_.op_strict ? 1 : 0))
             + x86("call", "rt_icn_subject_coerce", (uint64_t)(uintptr_t)(void *)rt_icn_subject_coerce)
             + x86_rt_gc_poll_rec_subject_new()
             + x86("test", "rax", "rax")
             + x86_omega("je")
             + x86("mov", "r13", "rax")
             + x86("mov", "r15", "rdx")
             + x86("def", L(2))
             + x86("xor", "r14d", "r14d")
             + x86("mov", "eax", "r15d")
             + x86("shl", "rax", 32L)
             + x86("or", "rax", (long)DT_S)
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "r13")
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

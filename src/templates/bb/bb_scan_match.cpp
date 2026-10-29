#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs_needle_t;
ScanSubjRegs_needle_t rt_icn_needle_coerce(uint64_t lo, uint64_t hi);
int core_icn_argtype_check(uint64_t lo, uint64_t hi, uint64_t code);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_match() {
    x86_begin();
    return (_.op_off >= 0 && !_.op_name1 && _.op_sa >= 0) ?
           x86("comment", "IR_SCAN_MATCH (var needle) [a string needle is read from its slot, int/real coerce through rt_icn_needle_coerce; result {DT_I, delta+1+len}]")
             + x86_alpha()
             + x86("mov", "rax", FRQ(_.op_sa))
             + x86("cmp", "al", (long)DT_S)
             + x86("je", L(2))
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov", "edx", (long)103)
             + x86("sub", "rsp", (long)8)
             + x86("call", "core_icn_argtype_check", (uint64_t)(uintptr_t)(void*)core_icn_argtype_check)
             + x86("add", "rsp", (long)8)
             + x86("test", "eax", "eax")
             + x86_omega("jne")
             + x86_rt_gc_poll_rec_sigma(0)
             + x86("def", L(2))
             + x86("mov", "rdx", FRQ(_.op_sa))
             + x86("cmp", "dl", (long)DT_S)
             + x86("jne", L(5))
             + x86("shr", "rdx", 32L)
             + x86("cmp", "edx", (long)-1)
             + x86("je", L(5))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("jmp", L(0))
             + x86("def", L(5))
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("sub", "rsp", (long)8)
             + x86("call", "rt_icn_needle_coerce", (uint64_t)(uintptr_t)(void*)rt_icn_needle_coerce)
             + x86("add", "rsp", (long)8)
             + x86_rt_gc_poll_rec_sigma_needle()
             + x86("mov", "rsi", "rax")
             + x86("def", L(0))
             + x86("mov", "rax", "r15")
             + x86("sub", "rax", "r14")
             + x86("cmp", "rax", "rdx")
             + x86_omega("jl")
             + x86("mov", "rdi", "r13")
             + x86("add", "rdi", "r14")
             + x86("xor", "rcx", "rcx")
             + x86("def", L(3))
             + x86("cmp", "rcx", "rdx")
             + x86("jge", L(4))
             + x86("movzx", "r10d", "[rsi+rcx]")
             + x86("movzx", "r11d", "[rdi+rcx]")
             + x86("cmp", "r10d", "r11d")
             + x86_omega("jne")
             + x86("inc", "rcx")
             + x86("jmp", L(3))
             + x86("def", L(4))
             + x86("mov", FRQ(_.op_off), (long)DT_I)
             + x86("mov", "rax", "r14")
             + x86("add", "rax", "rdx")
             + x86("add", "rax", (long)1)
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline() :
           (!(_.op_off >= 0 && _.op_name1 && _.op_ival >= 0)) ? x86_alpha() + x86_bomb("bb_scan_match: unhandled (needs literal string arg + descr flat-chain slot)") + x86_beta_trampoline() :
           x86("comment", "IR_SCAN_MATCH")
         + x86_alpha()
         + x86("mov", "rax", "r15")
         + x86("sub", "rax", "r14")
         + x86("cmp64", "rax", (long)_.op_ival)
         + x86_omega("jl")
         + x86("mov", "rsi", ROQ(0))
         + x86("mov", "rdi", "r13")
         + x86("add", "rdi", "r14")
         + x86("xor", "rcx", "rcx")
         + x86("def", L(3))
         + x86("cmp64", "rcx", (long)_.op_ival)
         + x86("jge", L(4))
         + x86("movzx", "r10d", "[rsi+rcx]")
         + x86("movzx", "r11d", "[rdi+rcx]")
         + x86("cmp", "r10d", "r11d")
         + x86_omega("jne")
         + x86("inc", "rcx")
         + x86("jmp", L(3))
         + x86("def", L(4))
         + x86("mov", FRQ(_.op_off), (long)DT_I)
         + x86("mov", "rax", "r14")
         + x86("add", "rax", (long)(1 + (long)_.op_ival))
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86_beta_trampoline()
         + x86(".quad_ro", L(0), LS(0), _.op_name1)
         + x86("label", LS(0))
         + x86(".string", _.op_name1, (unsigned long)_.op_ival);
}

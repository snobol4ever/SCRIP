#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs_needle_t;
ScanSubjRegs_needle_t rt_icn_needle_coerce(uint64_t lo, uint64_t hi);
long rt_sg_scan_nonmember(void);
int core_icn_argtype_check(uint64_t lo, uint64_t hi, uint64_t code);
}
#include "x86_asm.h"
#define RO_SEAL_Q(n, v) \
    (x86("def", L(n)) \
   + x86(".quad", (uint64_t)(v)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_many() {
    x86_begin();
    return (_.op_off >= 0 && !_.op_name1 && _.op_sa >= 0) ?
            x86_alpha()
             + x86("mov", "rax", FRQ(_.op_sa))
             + x86("cmp", "al", (long)DT_S)
             + x86("je", L(2))
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov", "edx", (long)104)
             + x86("sub", "rsp", (long)8)
             + x86("call", "core_icn_argtype_check", (uint64_t)(uintptr_t)(void*)core_icn_argtype_check)
             + x86("add", "rsp", (long)8)
             + x86("test", "eax", "eax")
             + x86_omega("jne")
             + x86_rt_gc_poll_rec_sigma(0)
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("sub", "rsp", (long)8)
             + x86("call", "rt_icn_needle_coerce", (uint64_t)(uintptr_t)(void*)rt_icn_needle_coerce)
             + x86("add", "rsp", (long)8)
             + x86_rt_gc_poll_rec_sigma_needle()
             + x86("mov", "rsi", "rax")
             + x86("mov", "edi", "r14d")
             + x86("call", "rt_sg_scan_nonmember", (uint64_t)(uintptr_t)(void *)rt_sg_scan_nonmember)
             + x86("jmp", L(1))
             + x86("def", L(2))
             + x86("shr", "rax", 32L)
             + x86("cmp", "eax", (long)-1)
             + x86("je", L(4))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov", "edx", "eax")
             + x86("mov", "edi", "r14d")
             + x86("call", "rt_sg_scan_nonmember", (uint64_t)(uintptr_t)(void *)rt_sg_scan_nonmember)
             + x86("jmp", L(1))
             + x86("def", L(4))
             + x86("mov", "rdi", FRQ(_.op_sa + 8))
             + x86("sub", "rdi", (long)32)
             + x86("mov", "eax", "r14d")
             + x86("def", L(6))
             + x86("cmp", "eax", "r15d")
             + x86("jge", L(1))
             + x86("movsxd", "rcx", "eax")
             + x86("movzx", "esi", "[r13+rcx]")
             + x86("bt", "[rdi]", "esi")
             + x86("jnc", L(1))
             + x86("add", "eax", (long)1)
             + x86("jmp", L(6))
             + x86("def", L(1))
             + x86("cmp", "eax", "r14d")
             + x86_omega("je")
             + x86("mov", FRQ(_.op_off), (long)DT_I)
             + x86("movsxd", "rcx", "eax")
             + x86("add", "rcx", (long)1)
             + x86("mov", FRQ(_.op_off + 8), "rcx")
             + x86_gamma()
             + x86_beta_trampoline() :
           (!(_.op_off >= 0 && _.op_name1)) ? x86_alpha() + x86_bomb("bb_scan_many: unhandled (needs literal cset arg + descr flat-chain slot)") :
           x86("comment", "IR_SCAN_MANY (literal cset: the box carries its own 256-bit table in read-only slots 3..6; membership is one bt, no call)")
         + x86_alpha()
         + x86_lea_rip_id("rdi", 3)
         + x86("mov", "eax", "r14d")
         + x86("def", L(0))
         + x86("cmp", "eax", "r15d")
         + x86("jge", L(1))
         + x86("movsxd", "rcx", "eax")
         + x86("movzx", "esi", "[r13+rcx]")
         + x86("bt", "[rdi]", "esi")
         + x86("jnc", L(1))
         + x86("add", "eax", (long)1)
         + x86("jmp", L(0))
         + x86("def", L(1))
         + x86("cmp", "eax", "r14d")
         + x86_omega("je")
         + x86("mov", FRQ(_.op_off), (long)DT_I)
         + x86("movsxd", "rcx", "eax")
         + x86("add", "rcx", (long)1)
         + x86("mov", FRQ(_.op_off + 8), "rcx")
         + x86_gamma()
         + x86_beta_trampoline()
         + x86(".quad_ro", L(2), LS(2), _.op_name1)
         + x86("label", LS(2))
         + x86(".string", _.op_name1, (unsigned long)_.op_ival)
         + RO_SEAL_Q(3, x86_cset_word(_.op_name1, _.op_ival, 0)) + RO_SEAL_Q(4, x86_cset_word(_.op_name1, _.op_ival, 1)) + RO_SEAL_Q(5, x86_cset_word(_.op_name1, _.op_ival, 2))
             + RO_SEAL_Q(6, x86_cset_word(_.op_name1, _.op_ival, 3));
}

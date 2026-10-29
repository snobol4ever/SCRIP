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
std::string bb_scan_find() {
    x86_begin();
    return (_.op_off >= 0 && !_.op_name1 && _.op_sa >= 0) ?
            x86_alpha()
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
             + x86("mov", FRQ(_.op_off + 16), "r14")
             + x86("def", L(7))
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
             + x86("mov", "rax", FRQ(_.op_off + 16))
             + x86("mov", "rcx", "r15")
             + x86("sub", "rcx", "rdx")
             + x86("cmp", "rax", "rcx")
             + x86_omega("jg")
             + x86("mov", "rdi", "r13")
             + x86("add", "rdi", "rax")
             + x86("xor", "rcx", "rcx")
             + x86("def", L(3))
             + x86("cmp", "rcx", "rdx")
             + x86("jge", L(4))
             + x86("movzx", "r10d", "[rsi+rcx]")
             + x86("movzx", "r11d", "[rdi+rcx]")
             + x86("cmp", "r10d", "r11d")
             + x86("jne", L(1))
             + x86("inc", "rcx")
             + x86("jmp", L(3))
             + x86("def", L(4))
             + x86("mov", FRQ(_.op_off), (long)DT_I)
             + x86("add", "rax", (long)1)
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86("def", L(1))
             + x86("inc", FRQ(_.op_off + 16))
             + x86("jmp", L(0))
             + x86_beta()
             + x86("inc", FRQ(_.op_off + 16))
             + x86("jmp", L(7)) :
           (!(_.op_off >= 0 && _.op_name1 && _.op_ival >= 1 && _.op_ival <= 32)) ?
           x86_alpha() + x86_bomb("bb_scan_find: unhandled (needs nonempty literal needle <=32 + descr flat-chain slot)") + x86_beta() + x86_bomb("bb_scan_find: unhandled") :
           x86("comment", "IR_SCAN_FIND")
         + x86_alpha()
         + x86("mov", FRQ(_.op_off + 16), "r14")
         + x86("def", L(0))
         + x86("mov", "rax", FRQ(_.op_off + 16))
         + x86("mov", "rcx", "r15")
         + x86("sub", "rcx", (long)_.op_ival)
         + x86("cmp", "rax", "rcx")
         + x86_omega("jg")
         + FOR(0, (int)_.op_ival, [&](int i) -> std::string {
               return x86("mov", "rcx", "rax")
                    + IF(i != 0, x86("add", "rcx", (long)i))
                    + x86("movzx", "esi", "[r13+rcx]")
                    + x86("cmp64", "rsi", (long)(unsigned char)_.op_name1[i])
                    + x86("jne", L(1)); })
         + x86("mov", FRQ(_.op_off), (long)DT_I)
         + x86("add", "rax", (long)1)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86("def", L(1))
         + x86("inc", FRQ(_.op_off + 16))
         + x86("jmp", L(0))
         + x86_beta()
         + x86("inc", FRQ(_.op_off + 16))
         + x86("jmp", L(0));
}

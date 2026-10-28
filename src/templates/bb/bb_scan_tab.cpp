#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int64_t core_icn_to_int_check(uint64_t lo, uint64_t hi);
int core_icn_int_operand_ok(uint64_t lo, uint64_t hi);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int tab_admit() { return _.op_off >= 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_tab() {
    x86_begin();
    return (!tab_admit()) ? x86_alpha() + x86_bomb("bb_scan_tab: no result slot (op_off)") :
           x86("comment", "IR_SCAN_TAB")
         + x86_alpha()
         + IF(_.op_sa >= 0, x86("mov", "rdi", FRQ(_.op_sa)))
         + IF(_.op_sa >= 0, x86("mov", "rsi", FRQ(_.op_sa + 8)))
         + IF(_.op_sa >= 0, x86("call", "core_icn_int_operand_ok", (uint64_t)(uintptr_t)(void*)core_icn_int_operand_ok))
         + IF(_.op_sa >= 0, x86("test", "eax", "eax"))
         + IF(_.op_sa >= 0, x86_omega("jz"))
         + IF(_.op_sa >= 0, x86_rt_gc_poll_rec_sigma(0))
         + IF(_.op_sa >= 0, x86("mov", "rdi", FRQ(_.op_sa)))
         + IF(_.op_sa >= 0, x86("mov", "rsi", FRQ(_.op_sa + 8)))
         + IF(_.op_sa >= 0, x86("call", "core_icn_to_int_check", (uint64_t)(uintptr_t)(void*)core_icn_to_int_check))
         + IF(_.op_sa >= 0, x86_rt_gc_poll_rec_sigma(1))
         + IF(_.op_sa < 0, x86("mov", "rax", (long)_.op_sb))
         + x86("cmp64", "rax", (long)1)
         + x86("jge", "L0")
         + x86("add", "rax", "r15")
         + x86("add", "rax", (long)1)
         + x86("def", "L0")
         + x86("cmp64", "rax", (long)1)
         + x86_omega("jl")
         + x86("mov", "rcx", "r15")
         + x86("add", "rcx", (long)1)
         + x86("cmp", "rax", "rcx")
         + x86_omega("jg")
         + x86("mov", FRQ(_.op_off + 16), "r14")
         + x86("mov", "rcx", "rax")
         + x86("sub", "rcx", (long)1)
         + x86("mov", "rdx", "r14")
         + x86("mov", "r14", "rcx")
         + x86("sub", "rdx", "rcx")
         + x86("jge", "L1")
         + x86("add", "rcx", "rdx")
         + x86("neg", "rdx")
         + x86("def", "L1")
         + x86("lea", "rsi", "[r13+rcx]")
         + x86("test", "rdx", "rdx")
         + x86("jnz", "L2")
         + x86("lea", "rsi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)"", x86_strtab_lbl("").c_str())
         + x86("def", "L2")
         + x86("shl", "rdx", 32L)
         + x86("or", "rdx", (long)DT_S)
         + x86("mov", FRQ(_.op_off), "rdx")
         + x86("mov", FRQ(_.op_off + 8), "rsi")
         + x86_gamma()
         + x86_beta()
         + x86("mov", "r14", FRQ(_.op_off + 16))
         + x86_omega();
}

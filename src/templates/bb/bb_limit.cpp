#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int64_t core_icn_limit_count_check(uint64_t lo, uint64_t hi);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lim_count() { return _.op_sb >= 0 ? _.op_sb : _.op_off + 16; }
static int lim_flag() { return _.op_sb >= 0 ? _.op_sb + 8 : _.op_off + 24; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_limit() {
    x86_begin();
    return IF(!(_.op_off >= 0 && _.op_sa >= 0 && _.op_sc >= 0 && _.lbl_t0), x86_alpha() + x86_bomb("bb_limit: unhandled (needs descr flat-chain, static slots, count slot, gen-β)"))
         + IF(_.op_off >= 0 && _.op_sa >= 0 && _.op_sc >= 0 && _.lbl_t0,
             x86("comment", "IR_LIMIT")
           + x86_alpha()
         + x86("mov", "rax", FRQ(lim_flag()))
           + x86("mov", FRQ(lim_flag()), (long)0)
           + x86("test", "rax", "rax")
           + x86("jnz", L(0))
           + x86("mov", FRQ(lim_count()), (long)0)
           + x86("def", L(0))
           + x86("mov", "rdi", FRQ(_.op_sc))
           + x86("mov", "rsi", FRQ(_.op_sc + 8))
           + x86("call", "core_icn_limit_count_check", (uint64_t)(uintptr_t)(void*)core_icn_limit_count_check)
           + x86("mov", "rcx", "rax")
           + x86("mov", "rax", FRQ(lim_count()))
           + x86("cmp", "rax", "rcx")
           + x86_omega("jge")
           + x86("inc", FRQ(lim_count()))
           + x86("mov", "rax", FRQ(_.op_sa))
           + x86("mov", FRQ(_.op_off), "rax")
           + x86("mov", "rax", FRQ(_.op_sa + 8))
           + x86("mov", FRQ(_.op_off + 8), "rax")
           + x86_rt_gc_poll()
           + x86_gamma()
           + x86_beta()
           + x86("mov", "rdi", FRQ(_.op_sc))
           + x86("mov", "rsi", FRQ(_.op_sc + 8))
           + x86("call", "core_icn_limit_count_check", (uint64_t)(uintptr_t)(void*)core_icn_limit_count_check)
           + x86("mov", "rcx", "rax")
           + x86("mov", "rax", FRQ(lim_count()))
           + x86("cmp", "rax", "rcx")
           + x86_omega("jge")
           + x86_rt_gc_poll()
           + x86("mov", FRQ(lim_flag()), (long)1)
           + IF(_.op_zres && _.op_t0_pop > 0, x86_add("rsp", (long)_.op_t0_pop))
           + IF(_.op_zres && _.op_t0_pop <= 0 && _.op_fc_bytes > 0, x86_add("rsp", (long)_.op_fc_bytes))
           + x86_jmp_tgt(X86T_TGT0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_limit_init() {
    x86_begin();
    return x86("mov", FRQ(lim_count()), (long)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_limit_gate() {
    x86_begin();
    return IF(!(_.op_sa >= 0), x86_alpha() + x86_bomb("bb_limit_gate: unhandled (needs the count operand slot)"))
         + IF(_.op_sa >= 0,
              x86_alpha()
           + IF(_.op_sb >= 0, x86("mov", FRQ(_.op_sb), (long)0) + x86("mov", FRQ(_.op_sb + 8), (long)0))
           + x86("mov", "rdi", FRQ(_.op_sa))
           + x86("mov", "rsi", FRQ(_.op_sa + 8))
           + x86("call", "core_icn_limit_count_check", (uint64_t)(uintptr_t)(void*)core_icn_limit_count_check)
           + x86("test", "rax", "rax")
           + x86_omega("jz")
           + x86_rt_gc_poll()
           + x86_gamma()
           + x86_beta()
           + x86_omega());
}

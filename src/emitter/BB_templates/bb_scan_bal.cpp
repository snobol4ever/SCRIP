#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline uint64_t strchr_ptr() { const char *(*fp)(const char *, int) = strchr; return (uint64_t)(uintptr_t)(void *)fp; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_scan_bal_str() {
    if (!PLATFORM_X86 || !(g_descr_flat_chain && _.op_off >= 0 && _.op_name1 && _.op_name1[0] && !strchr(_.op_name1, '(') && !strchr(_.op_name1, ')'))) return x86_bomb("bb_scan_bal: unhandled (needs nonempty bracket-free literal c1 + descr flat-chain slot)");
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", "BOX ICN IR_SCAN_BAL bal(c1) [ICN-SCAN-11 fstranl.r function{*} wave-1 c2='(' c3=')': cursor[r12+cur]<-δ cnt[r12+cnt]<-0; L0: cursor>=Δ->ω.node;"
                           " cnt==0∧s[cursor]∈c1 -> {DT_I,cursor+1}->slot->γ (state persists); L1: '('->cnt++; L2: ')'->cnt--, cnt<0->ω; L3: cursor++->L0; β RE-PUMPS: cursor++->L0 (sound:"
                             " admission excludes brackets from c1)]"))
         + x86("mov", FRQ(_.op_off + 16), "r14")
         + x86("mov", FRQ(_.op_off + 24), (long)0)
         + x86("def", L(0))
         + x86("mov", "rax", FRQ(_.op_off + 16))
         + x86("cmp", "rax", "r15")
         + x86("jge", "ω")
         + x86("mov", "rdx", FRQ(_.op_off + 24))
         + x86("cmp64", "rdx", (long)0)
         + x86("jne", L(1))
         + x86("mov", "rcx", "rax")
         + x86("movzx", "esi", "[r13+rcx]")
         + x86_ro_load_q("rdi", 4)
         + x86("push", "rax")
         + x86("push", "r10")
         + x86("call", "strchr", strchr_ptr())
         + x86("pop",  "r10")
         + x86("test", "rax", "rax")
         + x86("pop",  "rax")
         + x86("je",   L(1))
         + x86("mov", FRQ(_.op_off), (long)DT_I)
         + x86("add", "rax", (long)1)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86("jmp", "γ")
         + x86("def", L(1))
         + x86("mov", "rcx", "rax")
         + x86("movzx", "esi", "[r13+rcx]")
         + x86("cmp64", "rsi", (long)'(')
         + x86("jne", L(2))
         + x86("inc", FRQ(_.op_off + 24))
         + x86("jmp", L(3))
         + x86("def", L(2))
         + x86("cmp64", "rsi", (long)')')
         + x86("jne", L(3))
         + x86("mov", "rdx", FRQ(_.op_off + 24))
         + x86("sub", "rdx", (long)1)
         + x86("mov", FRQ(_.op_off + 24), "rdx")
         + x86("cmp64", "rdx", (long)0)
         + x86("jl",  "ω")
         + x86("def", L(3))
         + x86("inc", FRQ(_.op_off + 16))
         + x86("jmp", L(0))
         + x86("def", "β")
         + x86("inc", FRQ(_.op_off + 16))
         + x86("jmp", L(0))
         + x86_ro_seal_str(4, _.op_name1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_scan_bal(IR_t * pBB) { (void)pBB; x86_begin(); bb_emit_x86(bb_scan_bal_str()); }

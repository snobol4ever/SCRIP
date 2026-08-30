#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int rt_icn_cset_member(const char *needle, int ch);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* parens in c1 are ADMITTED: the emitted flow checks c1 membership at depth 0 BEFORE the paren-counting
   arm, which is exactly Icon's bal semantics for a bracket char in c1 (succeed where the prefix balances
   and the next char is in c1) — the old strchr rejections bombed btrees/prefix's `bal(')')`, and the
   bomb arm's missing β label then aborted the whole program at emit for a box that might never run. */
static int bal_admit() { return _.op_off >= 0 && _.op_name1 && _.op_name1[0]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_bal() {
    x86_begin();
    return (_.op_off >= 0 && !_.op_name1 && _.op_sa >= 0) ?
           x86("comment", "IR_SCAN_BAL (var c1) [fstranl.r bal: c1 cset-descr@slot, c2/c3 default ()/; counter off+24, cursor off+16; same envelope as literal arm]")
             + x86_alpha()
             + x86("mov",     FRQ(_.op_off + 16), "r14")
             + x86("mov",     FRQ(_.op_off + 24), (long)0)
             + x86("def",     L(0))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("cmp",     "rax", "r15")
             + x86_omega("jge")
             + x86("mov",     "rdx", FRQ(_.op_off + 24))
             + x86("cmp64",   "rdx", (long)0)
             + x86("jne",     L(1))
             + x86("mov",     "rcx", "rax")
             + x86("movzx",   "esi", "[r13+rcx]")
             + x86("mov",     "rdi", FRQ(_.op_sa + 8))
             + x86("push",    "rax")
             + x86("sub",     "rsp", (long)8)
             + x86("call",    "rt_icn_cset_member", (uint64_t)(uintptr_t)(void*)(int (*)(const char *, int))rt_icn_cset_member)
             + x86("add",     "rsp", (long)8)
             + x86("test",    "rax", "rax")
             + x86("pop",     "rax")
             + x86("je",      L(1))
             + x86("mov",     FRQ(_.op_off), (long)DT_I)
             + x86("add",     "rax", (long)1)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86("def",     L(1))
             + x86("mov",     "rcx", "rax")
             + x86("movzx",   "esi", "[r13+rcx]")
             + x86("cmp64",   "rsi", (long)'(')
             + x86("jne",     L(2))
             + x86("inc",     FRQ(_.op_off + 24))
             + x86("jmp",     L(3))
             + x86("def",     L(2))
             + x86("cmp64",   "rsi", (long)')')
             + x86("jne",     L(3))
             + x86("mov",     "rdx", FRQ(_.op_off + 24))
             + x86("sub",     "rdx", (long)1)
             + x86("mov",     FRQ(_.op_off + 24), "rdx")
             + x86("cmp64",   "rdx", (long)0)
             + x86_omega("jl")
             + x86("def",     L(3))
             + x86("inc",     FRQ(_.op_off + 16))
             + x86("jmp",     L(0))
             + x86_beta()
             + x86("inc",     FRQ(_.op_off + 16))
             + x86("jmp",     L(0)) :
           (!bal_admit()) ? x86_alpha() + x86_bomb("bb_scan_bal: unhandled (needs nonempty bracket-free literal c1 + descr flat-chain slot)") :
           x86("comment", "IR_SCAN_BAL")
         + x86_alpha()
         + x86("mov",     FRQ(_.op_off + 16), "r14")
         + x86("mov",     FRQ(_.op_off + 24), (long)0)
         + x86("def",     L(0))
         + x86("mov",     "rax", FRQ(_.op_off + 16))
         + x86("cmp",     "rax", "r15")
         + x86_omega("jge")
         + x86("mov",     "rdx", FRQ(_.op_off + 24))
         + x86("cmp64",   "rdx", (long)0)
         + x86("jne",     L(1))
         + x86("mov",     "rcx", "rax")
         + x86("movzx",   "esi", "[r13+rcx]")
         + x86("mov",     "rdi", ROQ(4))
         + x86("push",    "rax")
         + x86("sub",     "rsp", (long)8)
         + x86("call",    "rt_icn_cset_member", (uint64_t)(uintptr_t)(void*)(int (*)(const char *, int))rt_icn_cset_member)
         + x86("add",     "rsp", (long)8)
         + x86("test",    "rax", "rax")
         + x86("pop",     "rax")
         + x86("je",      L(1))
         + x86("mov",     FRQ(_.op_off), (long)DT_I)
         + x86("add",     "rax", (long)1)
         + x86("mov",     FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86("def",     L(1))
         + x86("mov",     "rcx", "rax")
         + x86("movzx",   "esi", "[r13+rcx]")
         + x86("cmp64",   "rsi", (long)'(')
         + x86("jne",     L(2))
         + x86("inc",     FRQ(_.op_off + 24))
         + x86("jmp",     L(3))
         + x86("def",     L(2))
         + x86("cmp64",   "rsi", (long)')')
         + x86("jne",     L(3))
         + x86("mov",     "rdx", FRQ(_.op_off + 24))
         + x86("sub",     "rdx", (long)1)
         + x86("mov",     FRQ(_.op_off + 24), "rdx")
         + x86("cmp64",   "rdx", (long)0)
         + x86_omega("jl")
         + x86("def",     L(3))
         + x86("inc",     FRQ(_.op_off + 16))
         + x86("jmp",     L(0))
         + x86_beta()
         + x86("inc",     FRQ(_.op_off + 16))
         + x86("jmp",     L(0))
         + x86("def",     L(4))
         + x86(".quad",   LS(4), _.op_name1)
         + x86("label",   LS(4))
         + x86(".string", _.op_name1);
}

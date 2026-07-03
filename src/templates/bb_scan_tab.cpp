#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t rt_substr(const char *sigma, int64_t a, int64_t b);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* op_sa is the operand-delivery discriminator (set by emit_drive's IR_SCAN_TAB arm):
     op_sa >= 0  => runtime n lives in producer slot op_sa (descr int value at +8);
     op_sa <  0  => literal n = op_sb, ANY value incl <=0 (tab(0)=to-end, tab(-k)=from-end).
   Non-positive positions are cvpos-normalized below (canonical cnv.r cvpos: p<=0 -> len+p+1),
   so a literal 0/negative is valid, not a "needs positive n" bomb. */
static int tab_admit() { return _.op_off >= 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_tab() {
    x86_begin();
    if (!PLATFORM_X86 || !tab_admit()) return x86_bomb("bb_scan_tab: no result slot (op_off)");
    return x86("comment", "IR_SCAN_TAB")
         + x86("label",   _.lbl_α)
         + IF(_.op_sa >= 0, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF(_.op_sa <  0, x86("mov", "rax", (long)_.op_sb))
         /* cvpos(n, len=r15/Δ): non-positive n -> len + n + 1; positive n passes through. */
         + x86("cmp64",   "rax", (long)1)
         + x86("jge",     "L0")
         + x86("add",     "rax", "r15")
         + x86("add",     "rax", (long)1)
         + x86("def",     "L0")
         /* range: keep iff 1 <= n <= len+1 (rejects raw n>len+1 and raw n<-len -> fail/ω). */
         + x86("cmp64",   "rax", (long)1)
         + x86("jl",      "ω")
         + x86("mov",     "rcx", "r15")
         + x86("add",     "rcx", (long)1)
         + x86("cmp",     "rax", "rcx")
         + x86("jg",      "ω")
         + x86("mov",     FRQ(_.op_off + 16), "r14")
         + x86("mov",     "rdi", "r13")
         + x86("mov",     "rsi", "r14")
         + x86("mov",     "rdx", "rax")
         + x86("sub",     "rdx", (long)1)
         + x86("mov",     "r14", "rdx")
         + x86("push",    "r10")
         + x86("push",    "r10")
         + x86("call",    "rt_substr", (uint64_t)(uintptr_t)(void*)rt_substr)
         + x86("pop",     "r10")
         + x86("pop",     "r10")
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86("jmp",     "γ")
         + x86("def",     "β")
         + x86("mov",     "r14", FRQ(_.op_off + 16))
         + x86("jmp",     "ω");
}

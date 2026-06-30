#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* IR_SUSPEND — Icon `suspend EXPR do BODY` inside a user-defined generator procedure.  operand[0] is the EXPR value
   (on-spine producer, slot in _.op_sa); operand[1] is the DO-BODY entry (off-spine, its α-label supplied by the
   chain in _.lbl_t0 / X86T_TGT0).  α (reached when the enclosing loop body runs): copy the EXPR value DESCR into
   the procedure result slot frame[0]=[r12+0], then jmp γ — which is the procedure SUCCEED port (the frame-active
   epilogue: mov eax,1; pop r12; ret), i.e. YIELD the value to the caller and return.  The persistent activation
   frame survives the return (rt_proc_call_gen / rt_proc_resume_gen).  β (reached only on RESUME: the proc prologue
   sees entry!=0 and jumps to the chain β, which propagates here): jump into the DO-BODY, which runs the side effect
   (e.g. i:=i+1) and then flows to its γ = the enclosing loop-back (re-test, suspend the next value).  r12 push/pop
   stays balanced: every entry (fresh esi=0 or resume esi!=0) pushes r12 in the prologue; every suspend-ret pops it. */
std::string bb_suspend() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (!(g_descr_flat_chain && _.op_sa >= 0 && _.lbl_t0))
        return x86_bomb("bb_suspend: unhandled (needs descr flat-chain, expr-value slot, do-body resume label)");
    return x86("comment", "IR_SUSPEND yield+resume")
         + x86("label", _.lbl_α)
         + x86("mov",   "rax", FRQ(_.op_sa))
         + x86("mov",   FRQ(0),     "rax")
         + x86("mov",   "rax", FRQ(_.op_sa + 8))
         + x86("mov",   FRQ(8),     "rax")
         + x86("jmp",   "γ")
         + x86("def",   "β")
         + x86_jmp_tgt(X86T_TGT0);
}

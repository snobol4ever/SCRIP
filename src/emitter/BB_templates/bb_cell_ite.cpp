#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "IR.h"
#include "IR_interp_state.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static const pl_gz_ite_state_t *bcit_st() { const IR_t *nd = (const IR_t *)_.bb_zn; return nd ? (const pl_gz_ite_state_t *)(intptr_t)IR_LIT(nd).ival : (const pl_gz_ite_state_t *)0; }
static int bcit_gate_off() { return GZ_CELL_OFF(bcit_st()->gate_slot); }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cell_ite_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    if (!bcit_st() || bcit_st()->gate_slot < 0) return x86_bomb("bb_cell_ite: unadmitted ifstmt shape reached the emitter");
    if (_.op_sa == 0)
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
             + x86("comment", "BOX CELL_ITE α  [paper §4.5 ifstmt VERBATIM: fall into E1 (the condition chain); E1's chain-γ = gate<-1 stub (commit, enter Then); E1's chain-ω = gate<-2 stub"
               " (exhausted pre-commit, enter Else); β = indexed goto on the frame gate (bb_cell_choice cursor-β dialect); E1's resume label is emitted UNREFERENCED = the bounded condition"
               " — Delete_Choice_Point achieved by WIRING, zero control-runtime calls]"));
    if (_.op_sa == 1)
        return IF(MEDIUM_TEXT, x86("comment", "CELL_ITE gate<-1  [E1 succeeded: commit; enter Then (δ = Then.α, or ifstmt.γ when Then is empty)]"))
             + x86("mov", FR(bcit_gate_off()), (long)1)
             + x86("jmp", PORT_DELTA);
    if (_.op_sa == 2)
        return IF(MEDIUM_TEXT, x86("comment", "CELL_ITE gate<-2  [E1 exhausted before commit: enter Else (δ = Else.α, or ifstmt.γ when Else is empty)]"))
             + x86("mov", FR(bcit_gate_off()), (long)2)
             + x86("jmp", PORT_DELTA);
    if (_.op_sa == 3)
        return IF(MEDIUM_TEXT, x86("comment", "CELL_ITE β dispatch  [redo: goto branch resume by gate — gate==1 ? Then.resume (δ) : Else.resume (ε); a det branch's resume walks its backward"
               " chain out at ifstmt.ω]"))
             + x86("def", "β")
             + x86("mov", "eax", FR(bcit_gate_off()))
             + x86("cmp", "eax", (long)1)
             + x86("je", PORT_DELTA)
             + x86("jmp", PORT_EPSILON);
    return x86_bomb("bb_cell_ite: unknown aspect");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cell_ite(void) { bb_emit_x86(bb_cell_ite_str()); }

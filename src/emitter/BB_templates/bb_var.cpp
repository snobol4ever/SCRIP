/* bb_var.cpp — x86() self-encoding (TEMPLATE-REVAMP, 2026-06-02). IR_VAR — variable read.
   pBB-free: op_sval = variable name; op_sa = var slot offset (bb_varslot_peek result, promoted at dispatch
   in walk_bb_flat IR_VAR case); op_off = own result slot (bb_slot_alloc16 result, promoted same place).
   No neighbor reads — the driver (walk_bb_flat IR_VAR) marshals all slot decisions onto _ before FILL.
   Two live arms + one fallback bomb:
     SNO flat-chain (g_gvar_flat_chain): pass-through — α: jmp γ ; def β ; jmp ω (10 bytes).
       The ASSIGN consumer reads the src variable NAME via _.op_a_sval / rt_gvar_assign_var; the IR_VAR box
       itself produces no value (SPITBOL semantics: a variable reference on the rhs of an assignment is
       resolved by the runtime name-value table at call time, not at emit time).
     ICN flat-chain (g_icn_flat_chain, op_off >= 0): GZ-7 16-byte DESCR copy (Icon variables are typed
       DESCRs — two 8-byte qwords: type tag lo + payload hi) from the named variable slot [r12+op_sa] into
       this box's own slot [r12+op_off] via two 64-bit frame loads/stores, then jmp γ ; def β ; jmp ω.
       The slot copy is register-relative [r12+off] (ζ-frame FACT RULE) — no movabs, no value stack.
       op_sa and op_off are both >= 0 only when bb_varslot_peek found the variable (assigned before read).
   Single-shot in both arms (a variable read does not re-offer): β = jmp ω. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_var_str() {
    if (!PLATFORM_X86) return std::string();
    if (g_gvar_flat_chain)
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment(emit_fmt("# BOX IR_VAR \"%s\" [SNO flat-chain: by-name pass-through]", _.op_sval ? _.op_sval : "")))
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    if (g_icn_flat_chain && _.op_off >= 0 && _.op_sa >= 0)
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment(emit_fmt("# BOX IR_VAR read(\"%s\") [GZ-7 ICN flat-chain: var slot %d -> own slot %d]", _.op_sval ? _.op_sval : "", _.op_sa, _.op_off)))
             + x86_frame_load64("rax", _.op_sa)
             + x86_frame_store64(_.op_off, "rax")
             + x86_frame_load64("rax", _.op_sa + 8)
             + x86_frame_store64(_.op_off + 8, "rax")
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    return x86_bomb("bb_var: unhandled arm (not sno/icn flat-chain or missing slot)");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_var(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_var_str()); }

/* bb_lit_scalar.cpp — grouped BB template for the scalar-literal family (IR_LIT_I/S/F/NUL).
   x86() self-encoding (TEMPLATE-REVAMP, 2026-06-02). Reads only _ (g_emit); zero bb_bin_t, zero raw-byte
   producer, zero MEDIUM_* instruction branch. PARTIAL conversion — see the IR_LIT_I note below.

   PASS-THROUGH (IR_LIT_S / IR_LIT_NUL / IR_LIT_F, and the non-flat-chain IR_LIT_I): a scalar literal is a
   READ-ONLY constant; per the RO-IP-relative model the CONSUMER box (write / assign / binop) materializes
   the value adjacent to its OWN blob and reads it [rip+disp], so this leaf has no runtime work — it only
   threads the four ports (α: jmp γ ; β: jmp ω). Byte-identical to the original IR_LIT_S arm (E9→γ ; β-def ;
   E9→ω). The old IR_LIT_F arm pushed the now-ABOLISHED value stack (rt_push_real_bits) — pass-through is its
   correct heir.

   IR_LIT_I FLAT-CHAIN (g_icn_flat_chain): the GZ-7 model stores a 16-byte DESCR {DT_I, value} into this
   box's ζ-slot so a downstream consumer reads it via bb_slot_get. Doing so RELOCATABLY requires a
   rip-relative LOAD of the sealed DESCR VALUE from an in-blob RO trailer (ICON READ-ONLY LOCALS ARE
   IP-RELATIVE FACT RULE — the value is NEVER addressed by a movabs immediate). The keystone's RO encoders
   load ADDRESSES (x86_load_ro), not sealed VALUES; the value-from-sealed-trailer encoder is the REG-RO rung
   (anticipated in x86_asm.h, not yet built). Until REG-RO lands, this ONE arm bombs LOUD rather than bend
   the RO FACT RULE with a movabs. Everything else is converted + green. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
extern int g_icn_flat_chain;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_lit_scalar_str() {
    if (PLATFORM_X86) {
        if (g_icn_flat_chain && _.op_node_kind == (int)IR_LIT_I)
            return x86_bomb("bb_lit_scalar IR_LIT_I flat-chain: needs REG-RO sealed-trailer rip-relative DESCR load");
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment("# BOX BB_LIT_scalar (pass-through; value is RO, consumer reads it [rip+disp])"))
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_lit_scalar(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_lit_scalar_str()); }

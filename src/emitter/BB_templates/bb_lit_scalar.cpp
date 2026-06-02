/* bb_lit_scalar.cpp — grouped BB template for the scalar-literal family (IR_LIT_I/S/F/NUL).
   x86() self-encoding (TEMPLATE-REVAMP, 2026-06-02). Reads only _ (g_emit); zero bb_bin_t, zero raw-byte
   producer, zero MEDIUM_* instruction branch. PARTIAL conversion — see the IR_LIT_I note below.

   PASS-THROUGH (IR_LIT_S / IR_LIT_NUL / IR_LIT_F, and the non-flat-chain IR_LIT_I): a scalar literal is a
   READ-ONLY constant; per the RO-IP-relative model the CONSUMER box (write / assign / binop) materializes
   the value adjacent to its OWN blob and reads it [rip+disp], so this leaf has no runtime work — it only
   threads the four ports (α: jmp γ ; β: jmp ω). Byte-identical to the original IR_LIT_S arm (E9→γ ; β-def ;
   E9→ω). The old IR_LIT_F arm pushed the now-ABOLISHED value stack (rt_push_real_bits) — pass-through is its
   correct heir.

   IR_LIT_I FLAT-CHAIN (driver deposits op_off>=0): the GZ-3 model stores a 16-byte DESCR {DT_I, value}
   into this box's ζ-slot [r12+off] so a downstream consumer reads it via bb_slot_get. The tag is a small
   constant (immediate); the VALUE is a compile-time RO constant sealed in an in-blob trailer (after the
   box's terminal jumps, never executed) and loaded rip-relative `mov rax,[rip+disp]` via the REG-RO encoder
   (x86_ro_load_q / x86_ro_seal_q) — disp DISCOVERED by the walker, no movabs of value-or-address (ICON
   READ-ONLY LOCALS ARE IP-RELATIVE FACT RULE). The discriminator is op_node_kind==IR_LIT_I && op_off>=0
   (the driver deposits op_off only in the Icon flat-chain), NOT g_icn_flat_chain — the flag is cleared by
   the time EMIT_PAIR_FILL->walk_bb_node lands this box. */
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
        if (g_icn_flat_chain && _.op_node_kind == (int)IR_LIT_I && _.op_off >= 0) {
            int off = _.op_off;
            return IF(MEDIUM_TEXT,
                       s_1asm(std::string(_.lbl_α) + ":")
                     + s_comment("# BOX BB_LIT_scalar IR_LIT_I [GZ-3 x86() stackless: {DT_I,val}->[r12+off]; val sealed RO [rip+disp] (REG-RO)]"))
                 + x86_frame_mov_imm64(off, (long)DT_I)
                 + x86_ro_load_q("rax", 0)
                 + x86_frame_store64(off + 8, "rax")
                 + x86("jmp", PORT_GAMMA)
                 + x86("def", PORT_BETA)
                 + x86("jmp", PORT_OMEGA)
                 + x86_ro_seal_q(0, (uint64_t) _.op_ival);
        }
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
extern "C" void bb_lit_scalar(IR_t * pBB) { (void)pBB; x86_begin(); bb_emit_x86(bb_lit_scalar_str()); }

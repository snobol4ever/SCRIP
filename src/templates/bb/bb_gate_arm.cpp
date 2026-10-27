#include <string>
#include <climits>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_zrow_gix(const IR_t * nd);
extern "C" int emit_zrow_gixr(const IR_t * nd);
std::string bb_gate_arm() {
    x86_begin();
    const int gix = emit_zrow_gix(_.node), gixr = emit_zrow_gixr(_.node);
    return IF(_.op_off < 0, x86_alpha() + x86_bomb("bb_gate_arm: no gate slot (op_off<0)"))
         + IF(_.op_off >= 0 && _.op_ival < 0, x86_alpha() + x86_bomb("bb_gate_arm: arm is not a member of its gate (op_ival<0)"))
         + IF(_.op_off >= 0 && _.op_ival >= 0,
               x86("comment", "IR_GATE_ARM")
             + x86_alpha()
             + (gix != INT_MIN ? x86("note", "the live-arm index lives on the spine in its BOUND's carve, above the record the commit pops; reached through the rbp alias "
                                             "(ARCH-PROLOG-ACTIVATION-LIFETIME 8.2)")
                               + x86("mov", RDD("rbp", gix), (int) _.op_ival)
                : gixr != INT_MIN ? x86("note", "the live-arm index lives on the spine in its BOUND's carve, at a fixed rsp distance in the BOUND's run family (ARCH-PROLOG-ACTIVATION-LIFETIME 8.2)")
                               + x86("mov", x86_zref(gixr, 0), (int) _.op_ival)
                : x86("mov", FR(_.op_off + 16), (int) _.op_ival))
             + x86_gamma()
             + x86_beta_trampoline());
}

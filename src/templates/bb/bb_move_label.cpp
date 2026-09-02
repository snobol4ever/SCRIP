#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_move_label() {
    x86_begin();
    if (_.op_off < 0) return x86_alpha() + x86_bomb("bb_move_label: no shared-slot owner (op_off<0)");
    if (!_.lbl_t0) return x86_alpha() + x86_bomb("bb_move_label: resume-target label unresolved (lbl_t0 NULL)");
    return x86("comment", "IR_MOVE_LABEL")
         + x86_alpha()
         + IF(_.op_sa >= 0 && _.op_sa != _.op_off,
               x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax"))
         + x86_lea_tgt("rax", X86T_TGT0)
         + x86("mov", FRQ(_.op_off + 16), "rax")
         + x86_gamma()
         + x86_beta_trampoline();
}

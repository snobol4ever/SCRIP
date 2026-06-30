#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_conj() {
    if (!PLATFORM_X86) return std::string();
    /* Case-result CONJ pattern: op_sa = source arm-value slot; op_off = CONJ own slot.
       When they differ, copy 2×8 bytes from arm slot into CONJ slot before γ-jumping. */
    if (_.op_sa >= 0 && _.op_off >= 0 && _.op_sa != _.op_off) {
        return x86("comment", "IR_CONJ case-result copy")
             + x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_pair_loop();
    }
    return x86_pair_loop();
}

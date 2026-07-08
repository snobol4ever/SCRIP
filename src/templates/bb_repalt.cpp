#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_repalt_clear() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    /* IR_REPALT is driven by flat_drive_repalt (emit.cpp), a hand-sequenced special case outside the ordinary
     * emit_drive/DRIVE_FILL per-node path -- but this function (bb_repalt_clear) is still the FIRST template
     * body flat_drive_repalt calls for this node (immediately after DRIVE_FILL's bb_fill_alpha sets up the
     * label object), so this node's own α is defined here, exactly where every ordinary template defines its
     * own α as its first instruction -- matching the ONE-WAY-OUTPUT fix (x86("def",...) is the sole output
     * path for a port define, in both mediums; the driver no longer calls bb_label_define(lbls[i]) directly). */
    return x86_alpha()
         + x86("comment", "IR_REPALT yielded:=0")
         + x86("mov", FRQ(_.op_off + 16), (long)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_repalt_yield() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_sa < 0) return x86_bomb("bb_repalt_yield: sub-expression value slot not materialised");
    return x86("comment", "IR_REPALT yield (copy value, yielded:=1)")
         + x86("mov", "rax", FRQ(_.op_sa))
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", "rax", FRQ(_.op_sa + 8))
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86("mov", FRQ(_.op_off + 16), (long)1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_repalt_test() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_REPALT test yielded")
         + x86("mov", "rax", FRQ(_.op_off + 16))
         + x86("cmp", "rax", (long)1);
}

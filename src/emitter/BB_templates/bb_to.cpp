#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int64_t bb_to_by() { return (_.op_node_kind == (int)IR_TO_BY && _.op_ival) ? _.op_ival : 1; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_to() {
    x86_begin();
    if (PLATFORM_X86)
        return !(g_descr_flat_chain && _.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0 && bb_to_by() > 0) ?
               x86_bomb("bb_to: unhandled (needs static int operands, positive by, descr flat-chain)") :
               x86("comment", "IR_TO")
             + x86("label",   _.lbl_α)
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("def",     L(0))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("mov",     "rcx", FRQ(_.op_sb + 8))
             + x86("cmp",     "rax", "rcx")
             + x86("jg",      "ω")
             + x86("mov",     FRQ(_.op_off),     (long)DT_I)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + IF(bb_to_by() == 1, x86("inc", FRQ(_.op_off + 16)))
             + IF(bb_to_by() != 1, x86("mov", "rax", FRQ(_.op_off + 16))
                                 + x86("add", "rax", (long)bb_to_by())
                                 + x86("mov", FRQ(_.op_off + 16), "rax"))
             + x86("jmp",     L(0));
    return std::string();
}

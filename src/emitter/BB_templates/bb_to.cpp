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
static int64_t bto_by() { return (_.op_node_kind == (int) IR_TO_BY && _.op_ival) ? _.op_ival : 1; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_to(IR_t * pBB) {
    x86_begin();
    (void)pBB;
    if (!(PLATFORM_X86 && g_descr_flat_chain && _.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0 && bto_by() > 0)) return x86_bomb("bb_to: unhandled (needs static int operands, positive by, descr flat-chain)");
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                          + x86("comment", std::string("BOX IR_TO [x86() stackless int range pump by=") + std::to_string((long long)bto_by()) + "; cursor [r12+" + std::to_string(_.op_off + 16) + "], lo slot " + std::to_string(_.op_sa) + ", hi slot " + std::to_string(_.op_sb) + " -> result " + std::to_string(_.op_off) + "]"))
         + x86("mov", "rax", FRQ(_.op_sa + 8))
         + x86("mov", FRQ(_.op_off + 16), "rax")
         + x86("def", L(0))
         + x86("mov", "rax", FRQ(_.op_off + 16))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("cmp", "rax", "rcx")
         + x86("jg",  "ω")
         + x86("mov", FRQ(_.op_off),     (long)DT_I)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86("jmp", "γ")
         + x86("def", "β")
         + IF(bto_by() == 1, x86("inc", FRQ(_.op_off + 16)))
         + IF(bto_by() != 1, x86("mov", "rax", FRQ(_.op_off + 16)) + x86("add", "rax", (long)bto_by()) + x86("mov", FRQ(_.op_off + 16), "rax"))
         + x86("jmp", L(0));
}
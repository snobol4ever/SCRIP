#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "emit_bb.h"
DESCR_t rt_list_bang_at(DESCR_t obj, int64_t idx);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_iterate_str(IR_t * pBB) {
    (void)pBB;
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return x86("comment", "no macro form — IR_LIST_BANG");
    return IF(_.op_sa < 0 || _.op_sb < 0 || _.op_off < 0,
               x86_bomb("bb_iterate: IR_LIST_BANG — operand/idx/out slot missing"))
         + IF(_.op_sa >= 0 && _.op_sb >= 0 && _.op_off >= 0,
               IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", "BOX IR_LIST_BANG !x [stackless generator: idx slot, rt_list_bang_at]"))
             + x86("mov",  FRQ(_.op_sb), 0L)
             + x86("def",  L(0))
             + x86("mov",  "rdi", FRQ(_.op_sa))
             + x86("mov",  "rsi", FRQ(_.op_sa + 8))
             + x86("mov",  "rdx", FRQ(_.op_sb))
             + x86("call", "rt_list_bang_at", (uint64_t)(uintptr_t)(void *)rt_list_bang_at)
             + x86("mov",  FRQ(_.op_off),     "rax")
             + x86("mov",  FRQ(_.op_off + 8), "rdx")
             + x86_cmp_imm64("rax", 99)
             + x86("je",   "ω")
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("inc",  FRQ(_.op_sb))
             + x86("jmp",  L(0)));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_iterate(IR_t * pBB) { x86_begin(); bb_emit_x86(bb_iterate_str(pBB)); }

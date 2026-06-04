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
    int sa  = _.op_sa;
    int sb  = _.op_sb;
    int off = _.op_off;
    if (sa < 0 || sb < 0 || off < 0)
        return x86_bomb("bb_iterate: IR_LIST_BANG — operand/idx/out slot missing");
    uint64_t fptr; { DESCR_t (*fp)(DESCR_t, int64_t) = rt_list_bang_at; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", "BOX IR_LIST_BANG !x [stackless generator: idx slot, rt_list_bang_at]"))
         + x86("mov",  FRQ(sb), 0L)
         + x86("def",  L(0))
         + x86("mov",  "rdi", FRQ(sa))
         + x86("mov",  "rsi", FRQ(sa + 8))
         + x86("mov",  "rdx", FRQ(sb))
         + x86("call", "rt_list_bang_at", fptr)
         + x86("mov",  FRQ(off),     "rax")
         + x86("mov",  FRQ(off + 8), "rdx")
         + x86_cmp_imm64("rax", 99)
         + x86("je",   PORT_OMEGA)
         + x86("jmp",  PORT_GAMMA)
         + x86("def",  PORT_BETA)
         + x86("inc",  FRQ(sb))
         + x86("jmp",  L(0));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_iterate(IR_t * pBB) { x86_begin(); bb_emit_x86(bb_iterate_str(pBB)); }

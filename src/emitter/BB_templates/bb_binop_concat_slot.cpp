/* bb_binop_concat_slot.cpp — BB box: IR_BINOP CONCAT, both operands producer slots. GZ-11+ (Icon/Raku).
   x86() self-encoding (template-revamp, 2026-06-02). pBB-FREE: reads ONLY g_emit. The driver
   (emit_bb.c case IR_BINOP, concat) deposits op_sa/op_sb (operand DESCR frame offsets) + op_off (this
   box's result DESCR slot). op_off>=0 with op_ival==BINOP_CONCAT is the driver's verdict "this is the
   slot-concat case". Loads operand a (lo->rdi, hi->rsi) and operand b (lo->rdx, hi->rcx) from
   [r12+slot], calls str_concat_d by value (SysV a=rdi:rsi b=rdx:rcx -> result DESCR rax:rdx), stores
   result into [r12+off]/[+8]. Grounded in Icon ofncs.r cat / str_concat_d heap-join. */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/interp/gen.h"
extern int g_icn_flat_chain;
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_concat_slot_str() {
    if (!PLATFORM_X86) return std::string();
    if (!(g_icn_flat_chain && _.op_off >= 0 && _.op_ival == BINOP_CONCAT)) return std::string();
    int sa = _.op_sa, sb = _.op_sb, off = _.op_off;
    if (sa < 0 || sb < 0) return std::string();
    uint64_t fptr; { DESCR_t (*fp)(DESCR_t, DESCR_t) = str_concat_d; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                          + s_comment("# BOX IR_BINOP concat [GZ-11+ x86() stackless slot->slot DESCR]"))
         + x86("mov", "rdi", FRQ(sa))
         + x86("mov", "rsi", FRQ(sa + 8))
         + x86("mov", "rdx", FRQ(sb))
         + x86("mov", "rcx", FRQ(sb + 8))
         + x86("call", "str_concat_d", fptr)
         + x86("mov", FRQ(off),     "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}

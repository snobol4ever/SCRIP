#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "ast.h"
#include "descr.h"
extern int g_descr_flat_chain;
struct DESCR_t rt_size_d(uint64_t lo, uint64_t hi);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
enum unop_op { UO_NEG, UO_POS, UO_SIZE, UO_NONNULL, UO_NULL_TEST, UO_NOT, UO_UNHANDLED };
/*--------------------------------------------------------------------------------------------------------------------*/
static unop_op bb_unop_resolve(int kind, int64_t sub) {
    switch (kind) {
    case IR_NEG:       return UO_NEG;
    case IR_POS:       return UO_POS;
    case IR_SIZE:      return UO_SIZE;
    case IR_NONNULL:   return UO_NONNULL;
    case IR_NULL_TEST: return UO_NULL_TEST;
    case IR_NOT:       return UO_NOT;
    case IR_UNOP:
        switch ((int)sub) {
        case TT_MNS:     return UO_NEG;
        case TT_PLS:     return UO_POS;
        case TT_SIZE:    return UO_SIZE;
        case TT_NONNULL: return UO_NONNULL;
        case TT_NULL:    return UO_NULL_TEST;
        case TT_NOT:     return UO_NOT;
        default:         return UO_UNHANDLED;
        }
    default: return UO_UNHANDLED;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_unop_str() {
    if (!PLATFORM_X86) return std::string();
    int off = _.op_off;
    if (!(g_descr_flat_chain && off >= 0)) return std::string();
    int sa = _.op_sa;
    unop_op uo = bb_unop_resolve(_.op_node_kind, _.op_ival);
    if (uo == UO_UNHANDLED) return std::string();
    if (uo == UO_NOT) {
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment("# BOX IR_NOT [x86() stackless: operand-fail->NULVCL->γ]"))
             + x86("mov", FRQ(off),     (long)0)
             + x86("mov", FRQ(off + 8), (long)0)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    if (uo == UO_NONNULL) {
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment("# BOX UNOP NONNULL [x86() stackless: fail/null->ω; DESCR->γ]"))
             + x86("mov", "eax", FR(sa))
             + x86("cmp", "eax", (long)99)
             + x86("je",  PORT_OMEGA)
             + x86("cmp", "eax", (long)0)
             + x86("je",  PORT_OMEGA)
             + x86("mov", "rax", FRQ(sa))
             + x86("mov", FRQ(off),     "rax")
             + x86("mov", "rax", FRQ(sa + 8))
             + x86("mov", FRQ(off + 8), "rax")
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    if (uo == UO_NULL_TEST) {
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment("# BOX UNOP NULL_TEST [x86() stackless: fail/non-null->ω; null->γ]"))
             + x86("mov", "eax", FR(sa))
             + x86("cmp", "eax", (long)99)
             + x86("je",  PORT_OMEGA)
             + x86("cmp", "eax", (long)0)
             + x86("jne", PORT_OMEGA)
             + x86("mov", FRQ(off),     (long)0)
             + x86("mov", FRQ(off + 8), (long)0)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    if (uo == UO_SIZE) {
        struct DESCR_t (*fp)(uint64_t, uint64_t) = rt_size_d;
        uint64_t fptr = (uint64_t)(uintptr_t)(void *)fp;
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment("# BOX UNOP SIZE [x86() stackless: rt_size_d->slot->γ]"))
             + x86("mov", "rdi", FRQ(sa))
             + x86("mov", "rsi", FRQ(sa + 8))
             + x86("call", "rt_size_d", fptr)
             + x86("mov", FRQ(off),     "rax")
             + x86("mov", FRQ(off + 8), "rdx")
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    int is_neg = (uo == UO_NEG);
    std::string neg_op;
    if (is_neg) {
        if (MEDIUM_BINARY) neg_op = x86_Lrec(x86_b3(0x48, 0xF7, 0xD8));
        else               neg_op = std::string(" neg rax\n");
    }
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                         + s_comment(emit_fmt("# BOX UNOP %s [x86() stackless slot->slot DESCR]",
                                              is_neg ? "NEG" : "POS")))
         + x86("mov", "rax", FRQ(sa + 8))
         + neg_op
         + x86("mov", FRQ(off),     (long)DT_I)
         + x86("mov", FRQ(off + 8), "rax")
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_unop(void) { bb_emit_x86(bb_unop_str()); }

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
static inline unop_op uop() { return bb_unop_resolve(_.op_node_kind, _.op_ival); }
static inline std::string uop_head(const char *msg) {
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", msg));
}
static inline std::string uop_tail() {
    return x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_unop_str() {
    if (!PLATFORM_X86) return std::string();
    if (!(g_descr_flat_chain && _.op_off >= 0)) return std::string();
    if (uop() == UO_UNHANDLED) return std::string();
    if (uop() != UO_NOT && _.op_sa < 0) return x86_bomb("bb_unop: operand slot unresolved (LIT_F/NUL or non-slot producer)");
    if (uop() == UO_NOT)
        return uop_head("BOX IR_NOT [x86() stackless: operand-fail->NULVCL->γ]")
             + x86("mov", FRQ(_.op_off),     (long)0)
             + x86("mov", FRQ(_.op_off + 8), (long)0)
             + uop_tail();
    if (uop() == UO_NONNULL)
        return uop_head("BOX UNOP NONNULL [x86() stackless: fail/null->ω; DESCR->γ]")
             + x86("mov", "eax", FR(_.op_sa))
             + x86("cmp", "eax", (long)99)
             + x86("je",  "ω")
             + x86("cmp", "eax", (long)0)
             + x86("je",  "ω")
             + x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + uop_tail();
    if (uop() == UO_NULL_TEST)
        return uop_head("BOX UNOP NULL_TEST [x86() stackless: fail/non-null->ω; null->γ]")
             + x86("mov", "eax", FR(_.op_sa))
             + x86("cmp", "eax", (long)99)
             + x86("je",  "ω")
             + x86("cmp", "eax", (long)0)
             + x86("jne", "ω")
             + x86("mov", FRQ(_.op_off),     (long)0)
             + x86("mov", FRQ(_.op_off + 8), (long)0)
             + uop_tail();
    if (uop() == UO_SIZE)
        return uop_head("BOX UNOP SIZE [x86() stackless: rt_size_d->slot->γ]")
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("call", "rt_size_d", (uint64_t)(uintptr_t)(void *)rt_size_d)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + uop_tail();
    return uop_head(uop() == UO_NEG ? "BOX UNOP NEG [x86() stackless slot->slot DESCR]" : "BOX UNOP POS [x86() stackless slot->slot DESCR]")
         + x86("mov", "rax", FRQ(_.op_sa + 8))
         + IF(uop() == UO_NEG, x86("neg", "rax"))
         + x86("mov", FRQ(_.op_off),     (long)DT_I)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + uop_tail();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_unop(void) { bb_emit_x86(bb_unop_str()); }

#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "ast.h"
#include "descr.h"
struct DESCR_t rt_size_d(uint64_t lo, uint64_t hi);
struct DESCR_t rt_num_neg(struct DESCR_t a);
struct DESCR_t rt_num_pos(struct DESCR_t a);
struct DESCR_t rt_cset_compl(struct DESCR_t a);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
enum unop_op { UO_NEG, UO_POS, UO_SIZE, UO_NONNULL, UO_NULL_TEST, UO_CSET_COMPL, UO_UNHANDLED };
/*--------------------------------------------------------------------------------------------------------------------*/
static unop_op bb_unop_resolve(int kind, int64_t sub) {
    switch (kind) {
    case IR_UNOP: case IR_UNOP_TEST:
        switch ((int)sub) {
        case TT_MNS:     return UO_NEG;
        case TT_PLS:     return UO_POS;
        case TT_SIZE:    return UO_SIZE;
        case TT_NONNULL: return UO_NONNULL;
        case TT_NULL:    return UO_NULL_TEST;
        case TT_CSET_COMPL: return UO_CSET_COMPL;
        default:         return UO_UNHANDLED;
        }
    default: return UO_UNHANDLED;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static inline unop_op uop() { return bb_unop_resolve(_.op_node_kind, _.op_ival); }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_unop() {
    if (PLATFORM_X86)
        return !(_.op_off >= 0) ? std::string() :
               uop() == UO_UNHANDLED ? std::string() :
               _.op_sa < 0 ? x86_bomb("bb_unop: operand slot unresolved (LIT_F/NUL or non-slot producer)") :
               uop() == UO_NONNULL ?
               x86("comment", "IR_UNOP")
             + x86("label",   _.lbl_α)
             + x86("mov", "eax", FR(_.op_sa))
             + x86("cmp", "eax", (long)99)
             + x86("je",  "ω")
             + x86("cmp", "eax", (long)0)
             + x86("je",  "ω")
             + x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86("jmp", "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω") :
               uop() == UO_NULL_TEST ?
               x86("comment", "IR_UNOP")
             + x86("label",   _.lbl_α)
             + x86("mov", "eax", FR(_.op_sa))
             + x86("cmp", "eax", (long)99)
             + x86("je",  "ω")
             + x86("cmp", "eax", (long)0)
             + x86("jne", "ω")
             + x86("mov", FRQ(_.op_off),     (long)0)
             + x86("mov", FRQ(_.op_off + 8), (long)0)
             + x86("jmp", "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω") :
               uop() == UO_CSET_COMPL ?
               x86("comment", "IR_UNOP")
             + x86("label",   _.lbl_α)
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("call", "rt_cset_compl", (uint64_t)(uintptr_t)(void *)rt_cset_compl)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86("jmp", "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω") :
               uop() == UO_SIZE ?
               x86("comment", "IR_UNOP")
             + x86("label",   _.lbl_α)
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("call", "rt_size_d", (uint64_t)(uintptr_t)(void *)rt_size_d)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86("jmp", "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω") :
               x86("comment", "IR_UNOP")
             + x86("label",   _.lbl_α)
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + IF(uop() == UO_NEG, x86("call", "rt_num_neg", (uint64_t)(uintptr_t)(void *)rt_num_neg))
             + IF(uop() != UO_NEG, x86("call", "rt_num_pos", (uint64_t)(uintptr_t)(void *)rt_num_pos))
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86("jmp", "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    return std::string();
}

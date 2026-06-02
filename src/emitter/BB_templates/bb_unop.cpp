/* bb_unop.cpp — BB template: Icon value-producing unary operators. x86() self-encoding (template-revamp).
   pBB-FREE: reads ONLY g_emit (FACT RULE — no neighbor reads, no fusion). Driver (emit_bb.c) deposits
   op_sa=bb_slot_get(nd->α) (-1 for IR_NOT), op_off=bb_slot_alloc16(nd) BEFORE FILL; op_off>=0 = "flat-chain,
   slots valid". The KIND comes from op_node_kind (promoted from nd->t at the walk_bb_node dispatch point);
   the IR_UNOP mux sub-op rides op_ival (also promoted there, from nd->ival = the raw tree_e TT_*).

   TWO PARALLEL FAMILIES, resolved to ONE operation set {NEG, POS, SIZE, NONNULL, NULL_TEST, NOT}:
   • IR_UNOP mux (live lowerer path, v_unop): op_ival ∈ {TT_MNS,TT_PLS,TT_SIZE,TT_NONNULL,…}.
   • split kinds (legacy): op_node_kind ∈ {IR_NEG,IR_POS,IR_SIZE,IR_NONNULL,IR_NULL_TEST,IR_NOT}.
   Grounded in jcon irgen.icn ir_a_Unop/ir_a_Not + Icon oarith.r(neg/pos)/ovalue.r(nonnull/null)/omisc.r(*);
   mode-2 oracle bb_exec.c IR_UNOP arm (TT_MNS=−, TT_PLS=+, TT_SIZE=size_value, TT_NONNULL: DT_SNUL→fail).

   PORT TOPOLOGIES (all four-port): β -> ω throughout.
   NEG/POS: read int payload [r12+sa+8], neg(NEG)/pass(POS), write {DT_I,res} to [r12+off] -> γ.
   SIZE:    rt_icn_size_d(rdi=[r12+sa],rsi=[r12+sa+8]) -> rax:rdx -> [r12+off] -> γ.
   NONNULL: type tag DT_FAIL(99)/DT_SNUL(0) -> ω; else copy 16-byte DESCR -> [r12+off] -> γ.
   NULL_TEST: type tag DT_FAIL(99)/non-zero -> ω; DT_SNUL(0) -> NULVCL -> [r12+off] -> γ.
   NOT:     no value operand — reached from operand.ω(fail); produce NULVCL -> [r12+off] -> γ. */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "ast.h"
#include "descr.h"
extern int g_icn_flat_chain;
struct DESCR_t rt_icn_size_d(uint64_t lo, uint64_t hi);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum unop_op { UO_NEG, UO_POS, UO_SIZE, UO_NONNULL, UO_NULL_TEST, UO_NOT, UO_UNHANDLED };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Resolve (op_node_kind, op_ival) -> one operation. The IR_UNOP mux dispatches on the TT_* sub-op in       */
/* op_ival; every split kind maps 1:1. An unhandled IR_UNOP sub-op (e.g. TT_CSET_COMPL — routed to bb_cset)*/
/* yields UO_UNHANDLED so the box emits nothing (the loud-decline path catches it).                        */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unop_str() {
    if (!PLATFORM_X86) return std::string();
    int off = _.op_off;
    if (!(g_icn_flat_chain && off >= 0)) return std::string();
    int sa = _.op_sa;
    unop_op uo = bb_unop_resolve(_.op_node_kind, _.op_ival);
    if (uo == UO_UNHANDLED) return std::string();
    if (uo == UO_NOT) {
        /* port-inversion node: reached from operand.ω(fail). Produce NULVCL {0,0} -> slot -> γ.        */
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment("# BOX IR_NOT [x86() stackless: operand-fail->NULVCL->γ]"))
             + x86("mov", FRQ(off),     (long)0)
             + x86("mov", FRQ(off + 8), (long)0)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    if (uo == UO_NONNULL) {
        /* \E: load type tag (lo dword), DT_FAIL(99)/DT_SNUL(0) -> ω; else copy DESCR -> slot -> γ.    */
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
        /* /E: load type tag; DT_FAIL(99)/non-zero -> ω; DT_SNUL(0) -> NULVCL -> slot -> γ.            */
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
        /* *E: rt_icn_size_d(lo=rdi, hi=rsi) -> rax:rdx = {DT_I, len} -> slot -> γ.                    */
        struct DESCR_t (*fp)(uint64_t, uint64_t) = rt_icn_size_d;
        uint64_t fptr = (uint64_t)(uintptr_t)(void *)fp;
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment("# BOX UNOP SIZE [x86() stackless: rt_icn_size_d->slot->γ]"))
             + x86("mov", "rdi", FRQ(sa))
             + x86("mov", "rsi", FRQ(sa + 8))
             + x86("call", "rt_icn_size_d", fptr)
             + x86("mov", FRQ(off),     "rax")
             + x86("mov", FRQ(off + 8), "rdx")
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    /* UO_NEG / UO_POS: read int payload at [r12+sa+8], negate(NEG)/pass(POS), {DT_I,res} -> slot -> γ.  */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_unop(void) { bb_emit_x86(bb_unop_str()); }

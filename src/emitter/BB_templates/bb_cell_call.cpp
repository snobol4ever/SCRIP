#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "IR.h"
#include "IR_interp_state.h"
}
#include "x86_asm.h"
extern "C" void * rt_enter(void **slot, int nslots);
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *bcc_areg(int i) { static const char * t[2] = { "rsi", "rdx" }; return t[i]; }
static std::string cc_load_args(const pl_gz_call_state_t *st) {
    return FOR(0, (st->nargs < 2 ? st->nargs : 2), [&](int i) -> std::string {
        const IR_t *a = st->args[i];
        if (!a || a->op != IR_LOGICVAR) return std::string();
        return x86("mov", bcc_areg(i), FRQ(GZ_CELL_OFF((int)IR_LIT(a).ival)));
    });
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const pl_gz_call_state_t *bcc_st() { const IR_t *nd = (const IR_t *)_.bb_zn; return nd ? (const pl_gz_call_state_t *)(intptr_t)IR_LIT(nd).ival : (const pl_gz_call_state_t *)0; }
static int bcc_nsl(const pl_gz_call_state_t *st) { return st->callee->arity + st->callee->nlocals + st->callee->nchild; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cell_call_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    if (!bcc_st() || !bcc_st()->callee || bcc_st()->nargs < 0 || bcc_st()->nargs > 2 || bcc_st()->child_slot < 0)
        return x86_bomb("bb_cell_call: unadmitted call shape reached the emitter");
    if (bcc_st()->nargs > 0 && cc_load_args(bcc_st()).empty()) return x86_bomb("bb_cell_call: non-slot arg reached the emitter");
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
             + x86("comment", "BOX CELL_CALL  [PL-GZ-5b ζ-tree: the call site OWNS a child-frame pointer slot (the seed's &ζ->p2_ζ); rt_enter = reuse-or-alloc; call δ with rdi=child"
               " rsi/rdx=arg cell POINTERS; verdict-in-rax λ-test; β re-enters callee β (ε) with rdi=child only — args already live in the child frame]"))
         + x86("lea", "rdi", FR(GZ_CELL_OFF(bcc_st()->child_slot)))
         + x86("mov32", "esi", (long)bcc_nsl(bcc_st()))
         + x86("call", "rt_enter", (uint64_t)(uintptr_t)(void *)rt_enter)
         + x86("mov", "rdi", "rax")
         + cc_load_args(bcc_st())
         + x86("call", "δ")
         + x86("def", L(0))
         + x86("test", "eax", "eax")
         + x86("jne", "γ")
         + x86("jmp", "ω")
         + x86("def", "β")
         + x86("mov", "rdi", FRQ(GZ_CELL_OFF(bcc_st()->child_slot)))
         + x86("call", "ε")
         + x86("jmp", L(0));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cell_call(void) { bb_emit_x86(bb_cell_call_str()); }

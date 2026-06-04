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
static std::string cc_load_args(const pl_gz_call_state_t *st) {
    std::string s;
    static const char * areg[2] = { "rsi", "rdx" };
    for (int i = 0; i < st->nargs && i < 2; i++) {
        const IR_t *a = st->args[i];
        if (!a || a->t != IR_LOGICVAR) return std::string();
        s += x86("mov", areg[i], FRQ(GZ_CELL_OFF((int)a->ival)));
    }
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cell_call_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    const IR_t *nd = (const IR_t *)_.bb_zn;
    const pl_gz_call_state_t *st = nd ? (const pl_gz_call_state_t *)(intptr_t)nd->ival : (const pl_gz_call_state_t *)0;
    if (!st || !st->callee || st->nargs < 0 || st->nargs > 2 || st->child_slot < 0)
        return x86_bomb("bb_cell_call: unadmitted call shape reached the emitter");
    std::string args = cc_load_args(st);
    if (st->nargs > 0 && args.empty()) return x86_bomb("bb_cell_call: non-slot arg reached the emitter");
    int nsl = st->callee->arity + st->callee->nlocals + st->callee->nchild;
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
             + x86("comment", "BOX CELL_CALL  [PL-GZ-5b ζ-tree: the call site OWNS a child-frame pointer slot (the seed's &ζ->p2_ζ); rt_enter = reuse-or-alloc; call δ with rdi=child rsi/rdx=arg cell POINTERS; verdict-in-rax λ-test; β re-enters callee β (ε) with rdi=child only — args already live in the child frame]"))
         + x86("lea", "rdi", FR(GZ_CELL_OFF(st->child_slot)))
         + x86("mov32", "esi", (long)nsl)
         + x86("call", "rt_enter", (uint64_t)(uintptr_t)(void *)rt_enter)
         + x86("mov", "rdi", "rax")
         + args
         + x86("call", PORT_DELTA)
         + x86("def", L(0))
         + x86("test", "eax", "eax")
         + x86("jne", PORT_GAMMA)
         + x86("jmp", PORT_OMEGA)
         + x86("def", PORT_BETA)
         + x86("mov", "rdi", FRQ(GZ_CELL_OFF(st->child_slot)))
         + x86("call", PORT_EPSILON)
         + x86("jmp", L(0));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cell_call(void) { bb_emit_x86(bb_cell_call_str()); }

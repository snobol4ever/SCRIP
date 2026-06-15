#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "IR.h"
#include "box_state.h"
}
#include "x86_asm.h"
extern "C" void *rt_pl_dyn_iter_begin(int functor_atom, long arity);
extern "C" int rt_pl_dyn_iter_step(void *cursor, void **arg_cell0, long arity);
extern "C" int  rt_trail_mark(void);
extern "C" void rt_trail_unwind(int mark);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const pl_gz_dyniter_state_t *bdi_st() { const IR_t *nd = (const IR_t *)_.bb_zn; return nd ? (const pl_gz_dyniter_state_t *)(intptr_t)IR_LIT(nd).ival : (const pl_gz_dyniter_state_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bdi_step() {
    return x86("mov", "rdi", FRQ(GZ_CELL_OFF(bdi_st()->cursor_slot)))
         + x86("lea", "rsi", FR(GZ_CELL_OFF(0)))
         + x86("mov32", "edx", (long)bdi_st()->arity)
         + x86("call", "rt_pl_dyn_iter_step", (uint64_t)(uintptr_t)(void *)rt_pl_dyn_iter_step)
         + x86("test", "eax", "eax")
         + x86("jne", "γ")
         + x86("jmp", "ω");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cell_dyniter() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    if (!bdi_st() || bdi_st()->arity < 0 || bdi_st()->arity > 3) return x86_bomb("bb_cell_dyniter: unadmitted dyniter shape reached the emitter");
    return x86("label", _.lbl_α)
         + x86("comment", "IR_CELL_DYNITER")
         + x86("call", "rt_trail_mark", (uint64_t)(uintptr_t)(void *)rt_trail_mark)
         + x86("mov", FR(GZ_CELL_OFF(bdi_st()->mark_slot)), "eax")
         + x86("mov32", "edi", (long)bdi_st()->functor_atom)
         + x86("mov32", "esi", (long)bdi_st()->arity)
         + x86("call", "rt_pl_dyn_iter_begin", (uint64_t)(uintptr_t)(void *)rt_pl_dyn_iter_begin)
         + x86("mov", FRQ(GZ_CELL_OFF(bdi_st()->cursor_slot)), "rax")
         + bdi_step()
         + x86("def", "β")
         + x86("mov", "edi", FR(GZ_CELL_OFF(bdi_st()->mark_slot)))
         + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind)
         + bdi_step();
}

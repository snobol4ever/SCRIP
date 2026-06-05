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
extern "C" int  rt_trail_mark(void);
extern "C" void rt_trail_unwind(int mark);
extern "C" void rt_pl_cells_init(void ** cells, int n);
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_callee_frame_str() {
    if (!PLATFORM_X86) return std::string();
    const IR_t *nd = (const IR_t *)_.bb_zn;
    const pl_gz_callee_t *ce = nd ? (const pl_gz_callee_t *)(intptr_t)nd->ival : (const pl_gz_callee_t *)0;
    if (!ce || ce->arity < 0 || ce->arity > 2 || ce->nlocals < 0)
        return x86_bomb("bb_callee_frame: unadmitted callee shape reached the emitter");
    static const char * areg[2] = { "rsi", "rdx" };
    if (_.op_sa == 0) {
        std::string s = IF(MEDIUM_TEXT,
                   x86("comment", "BOX CALLEE_FRAME α  [PL-GZ-5b ζ-tree seed ABI: rdi = THIS activation's child frame; push caller ζ (saves r12 AND restores SysV alignment), enter child"
                     " (mov r12,rdi); head vars ARE slots 0..arity-1 — arg cell POINTERS saved BEFORE rt_trail_mark (rt calls clobber arg regs); mark in [ζ+0] like the query frame; locals"
                       " = fresh cells per activation (the seed enter() law)]"))
             + x86("push", "r12")
             + x86("mov", "r12", "rdi");
        for (int i = 0; i < ce->arity; i++)
            s += x86("mov", FRQ(GZ_CELL_OFF(i)), areg[i]);
        s += x86("call", "rt_trail_mark", (uint64_t)(uintptr_t)(void *)rt_trail_mark)
           + x86("mov", FR(0), "eax");
        if (ce->nlocals > 0)
            s += x86("lea", "rdi", FR(GZ_CELL_OFF(ce->arity)))
               + x86("mov32", "esi", (long)ce->nlocals)
               + x86("call", "rt_pl_cells_init", (uint64_t)(uintptr_t)(void *)rt_pl_cells_init);
        return s + x86("jmp", PORT_GAMMA);
    }
    return IF(MEDIUM_TEXT,
               x86("comment", "BOX CALLEE_FRAME γ/ω landings + β  [verdict-in-rax: γ=1 (bindings stand); ω=trail-unwind([ζ+0])+0; pop restores CALLER ζ; β: push caller ζ, enter the SAME"
                 " child (rdi), jump into the body redo chain (δ)]"))
         + x86("def", PORT_GAMMA)
         + x86("mov32", "eax", 1L)
         + x86("pop", "r12")
         + x86("ret")
         + x86("def", PORT_OMEGA)
         + x86("mov", "edi", FR(0))
         + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind)
         + x86("mov32", "eax", 0L)
         + x86("pop", "r12")
         + x86("ret")
         + x86("def", PORT_BETA)
         + x86("push", "r12")
         + x86("mov", "r12", "rdi")
         + x86("jmp", PORT_DELTA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_callee_frame(void) { bb_emit_x86(bb_callee_frame_str()); }

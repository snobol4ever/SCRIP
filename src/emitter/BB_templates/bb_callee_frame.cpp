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
static const pl_gz_callee_t *bcf_ce() { const IR_t *nd = (const IR_t *)_.bb_zn; return nd ? (const pl_gz_callee_t *)(intptr_t)nd->ival : (const pl_gz_callee_t *)0; }
static int bcf_NC(const pl_gz_callee_t *ce) { return ce->nclauses > 0 ? ce->nclauses : 1; }
static const char *bcf_areg(int i) { static const char * t[2] = { "rsi", "rdx" }; return t[i]; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bcf_alpha(const pl_gz_callee_t *ce, int NC) {
    return IF(MEDIUM_TEXT,
               x86("comment", "BOX CALLEE_FRAME α  [PL-GZ-5b ζ-tree seed ABI: rdi = THIS activation's child frame; push caller ζ (saves r12 AND restores SysV alignment), enter child"
                 " (mov r12,rdi); head vars ARE slots 0..arity-1 — arg cell POINTERS saved BEFORE rt_trail_mark (rt calls clobber arg regs); mark in [ζ+0] like the query frame; locals"
                   " = fresh cells per activation (the seed enter() law); PL-GZ-5c: multi-clause cursor in [ζ+4] (the seed's ζ->i), 1-based, init at α]"))
         + x86("push", "r12")
         + x86("mov", "r12", "rdi")
         + FOR(0, ce->arity, [&](int i) { return x86("mov", FRQ(GZ_CELL_OFF(i)), bcf_areg(i)); })
         + x86("call", "rt_trail_mark", (uint64_t)(uintptr_t)(void *)rt_trail_mark)
         + x86("mov", FR(0), "eax")
         + (NC > 1 ? x86("mov", FR(4), (long)1) : std::string())
         + (ce->nlocals > 0 ? x86("lea", "rdi", FR(GZ_CELL_OFF(ce->arity)))
                            + x86("mov32", "esi", (long)ce->nlocals)
                            + x86("call", "rt_pl_cells_init", (uint64_t)(uintptr_t)(void *)rt_pl_cells_init)
                            : std::string())
         + x86("jmp", "γ");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bcf_beta_landings(int NC) {
    return IF(MEDIUM_TEXT,
               x86("comment", "BOX CALLEE_FRAME γ/ω landings + β  [verdict-in-rax: γ=1 (bindings stand); ω=trail-unwind([ζ+0])+0; pop restores CALLER ζ; β: push caller ζ, enter the SAME"
                 " child (rdi); single-clause: jump into the body redo chain (δ); multi-clause (PL-GZ-5c): fall into the driver-sequenced cursor cmp-chain that follows]"))
         + x86("def", "γ")
         + x86("mov32", "eax", 1L)
         + x86("pop", "r12")
         + x86("ret")
         + x86("def", "ω")
         + x86("mov", "edi", FR(0))
         + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind)
         + x86("mov32", "eax", 0L)
         + x86("pop", "r12")
         + x86("ret")
         + x86("def", "β")
         + x86("push", "r12")
         + x86("mov", "r12", "rdi")
         + (NC <= 1 ? x86("jmp", "δ") : std::string());
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_callee_frame_str() {
    if (!PLATFORM_X86) return std::string();
    if (!bcf_ce() || bcf_ce()->arity < 0 || bcf_ce()->arity > 2 || bcf_ce()->nlocals < 0)
        return x86_bomb("bb_callee_frame: unadmitted callee shape reached the emitter");
    if (_.op_sa == 0) return bcf_alpha(bcf_ce(), bcf_NC(bcf_ce()));
    if (_.op_sa == 1) return bcf_beta_landings(bcf_NC(bcf_ce()));
    if (_.op_sa == 2) return x86("mov", "eax", FR(4)) + x86("cmp", "eax", (long)_.op_off) + x86("je", "δ");
    if (_.op_sa == 3) return x86("jmp", "δ");
    if (_.op_sa == 4)
        return IF(MEDIUM_TEXT, x86("comment", "CALLEE_FRAME clause-advance  [seed fK_ω: ζ->i = k+2; trail_unwind(ζ->mark); goto fK+1_α]"))
             + x86("mov", FR(4), (long)_.op_off)
             + x86("mov", "edi", FR(0))
             + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind)
             + x86("jmp", "δ");
    return x86_bomb("bb_callee_frame: unknown aspect");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_callee_frame(void) { bb_emit_x86(bb_callee_frame_str()); }

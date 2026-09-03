#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
extern "C" void rt_pl_cut_barrier(void *);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cut_barrier() {
    if (!x86_fb_pinned()) return std::string();
    int kt = g_emit.flat_frame_bytes;
    uint64_t fp; { void (*f)(void *) = rt_pl_cut_barrier; fp = (uint64_t)(uintptr_t)(void *)f; }
    return x86("comment", "PL CUT BARRIER (rung 4, ARCH sec B.6): commit. F.CUR at [H+8] := 0 so the rung-2 clause step concedes instead of trying the next candidate clause; F.RES at [H+16] := 0 so a redo from the caller cannot land in a callee this cut just killed; B := F.B0 at [H+24] through a NAMED rtx helper, never an emitted r13 write. The trail needs NO promotion -- it is one linear arena on r12 (rung 1), so a cut-away callee's entries simply stay above the older choice's mark and are undone when IT backtracks, which is the WAM's single-trail behaviour and costs zero instructions.")
         + x86("mov", RDQ(x86_fb(), kt - 56), 0L)
         + x86("mov", RDQ(x86_fb(), kt - 48), 0L)
         + x86("lea", "rdi", RDQ(x86_fb(), kt - 64))
         + x86("call_bare", "rt_pl_cut_barrier", fp)
         + x86("comment", "and release every younger frame PHYSICALLY off the pin -- the WAM's B <- B0 reclaiming the stack (sec A.1 review C4). Nothing below the pin is reachable any more: every callee to the left of this cut had its beta made unreachable by the lowerer's cut_omega rewiring in the same rung.")
         + x86("mov", "rsp", x86_fb());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cut() {
    x86_begin();
    return x86_alpha()
                           + x86("comment", "IR_CUT")
                           + cut_barrier()
                           + x86_gamma()
                           + x86_beta_trampoline();
}

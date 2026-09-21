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
    if (_.op_ival) return std::string();
    int kt = g_emit.flat_frame_bytes;
    uint64_t fp; { void (*f)(void *) = rt_pl_cut_barrier; fp = (uint64_t)(uintptr_t)(void *)f; }
    return  x86("mov", RDQ(x86_fb(), kt - 56), 0L)
         + x86("mov", RDQ(x86_fb(), kt - 48), 0L)
         + x86("lea", "rdi", RDQ(x86_fb(), kt - 64))
         + x86("call_bare", "rt_pl_cut_barrier", fp)
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

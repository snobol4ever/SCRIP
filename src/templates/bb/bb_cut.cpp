#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cut_barrier() {
    return IF(x86_resumable_frame() && !_.op_ival,
               x86("mov", FRQ(g_emit.flat_frame_bytes - 56), 0L)
             + x86("mov", FRQ(g_emit.flat_frame_bytes - 48), 0L)
             + x86("note", "pl_cut_barrier inline (ARCH-PROLOG-C-OUT-OF-THE-BOX 2.2): B := F.B0, the caller's choice the prologue saved at [kt-40]")
             + x86("mov", "r13", FRQ(g_emit.flat_frame_bytes - 40))
             + x86_pl_tr_tidy(400)
             + x86_release_acts(false)
             + (x86_hb_on()
                 ? (_.op_zcut >= 0 ? x86("lea", "rsp", RDQ(x86_hb(), -_.op_zcut)) : std::string())
                 : x86("lea", "rsp", FRQ(0))));
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

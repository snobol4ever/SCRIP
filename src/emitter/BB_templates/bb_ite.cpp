/* bb_ite.cpp — BB template for IR_ITE: Prolog (Cond -> Then ; Else).  x86() self-encoding (template-revamp
   PL-RV-3, 2026-06-02, Opus 4.8).  The driver flat_drive_pl_ite (emit_bb.c) is byte-free: it mints Then/Else
   region labels and recursively walks each sub-region, depositing the wrapper glue — the β-tombstone (β: jmp ω;
   ITE is non-resumable from the caller's view, internal choice points inside Then/Else retry via their own β
   labels) — into g_emit.xa_bb_emit_pair_*.  This template emits that collected variable-length define/jmp-pair
   loop via the SHARED x86_pair_loop() combinator (the RESOLVED pair-loop primitive in x86_asm.h, also used by
   Prolog bb_conj and SNOBOL4 bb_pat_cat/bb_pat_alt).  ONE return, pure concat, NO bb_bin_t, pBB-free (reads
   only _ / g_emit).  Both media are produced by the ONE primitive (medium-invisible per R2): its 'E'/'F'/'L'
   records reproduce the exact define / E9+rel32 / GAS sequence the box hand-rolled before, so the byte stream
   is byte-identical to the prior MEDIUM_BINARY+MEDIUM_TEXT arms in both media (GATE-3 unchanged 111/111/86).
   x86 only per Invariant #14. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_ite_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT, s_comment("# END RESOLVE_ITE (β-tombstone via EP)  [x86() self-encoding]"))
             + x86_pair_loop();
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ite(void) { bb_emit_x86(bb_ite_str()); }

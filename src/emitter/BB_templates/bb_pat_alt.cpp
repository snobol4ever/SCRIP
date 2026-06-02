/* bb_pat_alt.cpp — BB template for IR_PAT_ALT: SNOBOL4 pattern alternation.  x86() self-encoding
   (template-revamp, 2026-06-02, Opus 4.8).  The driver (emit_bb.c) is byte-free: it mints alternation
   glue labels and recursively walks each alt arm via walk_bb_flat, depositing the collected define/jmp pairs
   into g_emit.xa_bb_emit_pair_*.  This template emits that variable-length define/jmp-pair loop via the SHARED
   x86_pair_loop() combinator (also used by Prolog bb_conj/bb_ite and SNOBOL4 bb_pat_cat) — NO bb_bin_t,
   pBB-free (reads only _ / g_emit).  ONE return, pure concat.  Both media are produced by the ONE primitive
   (medium-invisible per R2): its 'E'/'F'/'L' records reproduce the exact define / E9+rel32 / GAS sequence
   the box hand-rolled before.  x86 only. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_alt_str() {
    if (PLATFORM_X86) return x86_pair_loop();
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_alt(void) { bb_emit_x86(bb_pat_alt_str()); }

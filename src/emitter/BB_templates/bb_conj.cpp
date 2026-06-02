/* bb_conj.cpp — BB template for IR_GCONJ: Prolog conjunction (a, b, c).  x86() self-encoding (template-revamp
   PL-RV-3, 2026-06-02, Opus 4.8).  The driver flat_drive_pl_seq (emit_bb.c) owns all recursion + label minting
   (byte-free) and populates g_emit.xa_bb_emit_pair_* with the conjunction glue (the β-chain jmp into the last
   goal's β).  This template is the leaf that emits those collected define/jmp pairs via the SHARED
   x86_pair_loop() combinator (the RESOLVED variable-length pair-loop primitive in x86_asm.h, also used by
   Prolog bb_ite and SNOBOL4 bb_pat_cat/bb_pat_alt).  No XA_PL_SEQ_DRIVE opcode (that pattern does not exist —
   see GOAL-PROLOG-BB PL-2).  ONE return, pure concat, NO bb_bin_t, pBB-free (reads only _ / g_emit).  Both
   media are produced by the ONE primitive (medium-invisible per R2): its 'E'/'F'/'L' records reproduce the
   exact define / E9+rel32 / GAS sequence the box hand-rolled before, so the byte stream is byte-identical to
   the prior MEDIUM_BINARY+MEDIUM_TEXT arms in both media (GATE-3 unchanged 111/111/86).  x86 only. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_conj_str() {
    if (PLATFORM_X86) return x86_pair_loop();
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_conj(void) { bb_emit_x86(bb_conj_str()); }

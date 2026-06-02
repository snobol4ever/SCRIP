/* bb_atom.cpp — TEMPLATE-REVAMP bomb stub (Lon directive 2026-06-02).  The original box body used the
   ABOLISHED bb_bin_t / bb_emit_asm_result offset-table path.  It is replaced with a LOUD x86_bomb() stub so
   SCRIP BUILDS + LINKS (green) and ABORTS beautifully when this box is reached — the Prolog
   GOAL-*-BB session converts it to real x86() self-encoding as its own test reaches it.  pBB-free per the
   revamp FACT RULES (reads only _ / g_emit); zero bb_bin_t, zero raw-byte producer, zero MEDIUM_* branch. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_atom(IR_t * pBB) {
    (void)pBB;
    bb_emit_x86(x86_bomb("bb_atom: TEMPLATE-REVAMP not yet converted (was offset-table)"));
}

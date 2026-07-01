#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------
 * xa_coexpr_entry.cpp -- the x86 BRIDGE for `create EXPR`, expressed as an XA (stitching) template per Lon's
 * directive (2026-07-01). It closes GOAL-IR-IMMUTABLE-EMIT.md's RUNG-3 "LIMITATION 2": bb_create must load the
 * coexpression body's entry ADDRESS (a RIP-relative LEA) so scrip_coexpr_create knows where a coswitch into
 * this coexpression lands, and it must do so in BOTH mediums. bb_create alone could not: the body-entry label
 * arrived only as a NAME string (op_sval_lbl), and x86_label_for -- the sole bridge from a template's tagged
 * bytecode to a bb_label_t* the rel32 patcher consumes -- resolves only port ids (α/β/γ/ω/t0/t1), never an
 * external label name. This file is the bridge across that gap.
 *
 * WHY XA, NOT BB: this builds NO operand and produces no value DESCR -- it only STITCHES one node's emitted code
 * to another node's label (the create site to its body-entry α). That is precisely the XA role in the block-kind
 * split (BB = does work / builds operands; XA = wraps/stitches / builds no operands). It is a string-returning
 * fragment (composed into bb_create's output) rather than a void xa_dispatch section-emitter, because the seam it
 * stitches lives INSIDE a single node's code, not around a whole glob.
 *
 * HOW: the body-entry α-label's bb_label_t* is threaded into the t0 port (g_emit.lbl_t0_p) by
 * codegen_flat_chain_body's IR_CREATE resolution -- exactly the mechanism IR_LIMIT uses to thread its
 * generator-β via lbl_t0_p, the working precedent LIMITATION 2 itself named. x86_lea_tgt then emits
 * `lea dst, [rip + t0]` uniformly: in TEXT the label name, in BINARY the RIP-relative LEA opcode bytes plus a
 * 'J' rel32 patch record against port X86T_TGT0 (disp32 = target-(site+4), the identical formula the port
 * jump/call helpers already use -- valid verbatim for a LEA because disp32 is the instruction's last field).
 * No MEDIUM_* gating, no hand-encoded bytes in the template: all encoding lives in x86_lea_tgt inside x86_asm.h,
 * as TEMPLATE-ONLY EMISSION requires.
 *--------------------------------------------------------------------------------------------------------------------*/
std::string xa_coexpr_body_lea(const char * dst) {
    if (!PLATFORM_X86) return std::string();
    if (!_.lbl_t0) return x86_bomb("xa_coexpr_body_lea: coexpression body-entry target (t0 port) not threaded -- codegen_flat_chain_body's IR_CREATE resolution must set g_emit.lbl_t0_p to the body-entry α-label before bb_create runs");
    return x86_lea_tgt(dst, X86T_TGT0);
}

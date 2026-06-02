/* bb_disj.cpp — BB template for IR_DISJ: Prolog `;` disjunction (`( A ; B )`).  x86() self-encoding
   (template-revamp PL-RV-4, 2026-06-02, Opus 4.8).  Same dispatcher shape as bb_choice but n is the arm
   count (g_emit.resolve_choice_n, driver-promoted) and the arm bodies are wired by the driver
   flat_drive_pl_alt (emit_bb.c): each arm's body label cbody[i] is define+walked there, arm i's failure
   flows to the next arm's pre (this template's pre[i>0] does rt_pl_trail_unwind_top then jmps body[i]),
   the last arm's failure to ω.  This template emits ONLY the α/pre/β trail-mark scaffold: α→pre[0];
   pre[0]: trail_mark_push; jmp body[0]; pre[i>0]: trail_unwind_top; jmp body[i]; β: jmp ω.
   pBB-FREE: reads only _ (resolve_choice_id/_n already promoted by the driver; ports/labels).  The two
   rt_pl_trail_* calls go through the x86() RO-call encoder.  The pre/body labels are the driver-minted,
   name-deterministic glue (resolve_choice_clause_label(id,i,...)) the driver coordinates on — NOT ports
   and NOT box-internal L(n), so they are emitted as the GAS label scaffold the driver shares.  The dead
   MEDIUM_BINARY twin — which hand-counted its own rel32 patch offsets via b.size() — is DELETED: Prolog's
   live disjunction path is mode-4 TEXT (mode-3 routes the oracle); verified zero BINARY-arm fires across
   the full rung suite via abort-probe, so the whole box is TEXT-only and BINARY yields empty. x86 only. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" {
void rt_pl_trail_mark_push(void);
void rt_pl_trail_unwind_top(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string disj_pre (int ci) { char b[160]; resolve_choice_clause_label(b, sizeof b, _.resolve_choice_id, ci, "pre");  return std::string(b); }
static std::string disj_body(int ci) { char b[160]; resolve_choice_clause_label(b, sizeof b, _.resolve_choice_id, ci, "body"); return std::string(b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_disj_str() {
    if (PLATFORM_X86) {
        int n = _.resolve_choice_n;
        if (n <= 0)
            return IF(MEDIUM_TEXT,
                      s_1asm(std::string(_.lbl_α) + ":")
                    + s_comment("# BOX RESOLVE_ALT (empty)  [x86() self-encoding]")
                    + s_2asm("jmp", _.lbl_ω)
                    + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
        return IF(MEDIUM_TEXT,
                  s_1asm(std::string(_.lbl_α) + ":")
                + s_comment(emit_fmt("# BOX RESOLVE_ALT n=%d (mode-4 first-solution)  [x86() self-encoding]", n))
                + s_2asm("jmp", disj_pre(0).c_str())
                + s_1asm(disj_pre(0) + ":")
                + x86("call", "rt_pl_trail_mark_push", (uint64_t)(uintptr_t)(void*)rt_pl_trail_mark_push)
                + s_2asm("jmp", disj_body(0).c_str())
                + FOR(1, n, [](int i){
                      return s_1asm(disj_pre(i) + ":")
                           + x86("call", "rt_pl_trail_unwind_top", (uint64_t)(uintptr_t)(void*)rt_pl_trail_unwind_top)
                           + s_2asm("jmp", disj_body(i).c_str()); })
                + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_disj(void) { bb_emit_x86(bb_disj_str()); }

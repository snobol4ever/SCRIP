#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
void rt_goto_transfer(const char *name);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_GOTO_DEFERRED (EVAL/CODE, manual Ch.9).  A goto whose label was unknown at lower time: a label defined only
 * inside a runtime-compiled CODE fragment, a `$X` indirect name, or a variable holding a CODE value (the
 * lexer folds `:<C>` onto the plain-name form).  op_sval = the name as written, sealed .rodata adjacent to
 * the blob (the bb_match_replace/bb_call precedent).  α marshals rt_goto_transfer(name), which resolves and
 * RUNS the transferee nested; SNOBOL4 gotos never resume their source, so control returning here means the
 * transferee terminated — γ is wired (at lower time) to the graph's exit and unwinds this chain too, the
 * termination cascade.  Cannot fail from the template's view (an undefined label faults inside the runtime). */
std::string bb_goto_dyn() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    { static int _df = -1; if (_df < 0) { const char * e = getenv("SCRIP_DEFINE_FOLD"); _df = (e && *e == '0') ? 0 : 1; }   /* s55: DEFAULT ON per Lon "Complete DEFINE function constant folding to FUNCTION linkage"; =0 is the revert hatch */
    if (_df && _.op_ival == 1 && _.op_sval && _.op_sval[0] && _.op_sval[0] != '$') {
        /* ⛔⭐⭐⭐ DEFINE-FOLD COMPLETE — ONE-SHOT s55 (Lon in-chat): "Quit trying to return from the function.  We are
         * not trying to return … You are just getting the going in part."  rt_chain_enter DELETED from this arm — its
         * five callee-save pushes and fall-off landing are RETURN machinery; its real customers (EVAL/CODE/$X) keep it
         * via the transfer arm below.  The LANDING RESERVE DELETED with it — it existed solely to protect the callee's
         * γ-epilogue landing, and both that epilogue and the record it read are ERADICATED (rt.c s55).  GOING-IN is now:
         * wires already in r10/r11 (role-3 adopt, two boxes back), one jmp to the body α.  Zero C, zero frame, zero
         * globals.  BINARY arm stays on the transfer path below (per-call lookup, functional) — the sealed-cell
         * resolution is the owed slice-2, unchanged from s53's plan. */
        std::string lbl = std::string("LBL__") + _.op_sval;   /* BARE-CHAIN (Lon s62) + s112 rename (Lon in-chat): the body chain's one label IS the rt key spelling, LBL__<entry> — the <entry>_body spelling is removed */
        uint64_t body_cell = (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("body$") + _.op_sval).c_str());   /* ⭐ s111 BOTH-MEDIUM LIFT: the owed slice-2.  The MEDIUM_TEXT conjunct above sent BINARY to the rt_goto_transfer arm, which RUNS the body NESTED inside a C frame (rt_goto_transfer + rt_chain_enter callee-saves = 384B measured) — so the body never entered at its own alpha, never reached the RETURN floater at site depth, and the site's {gamma,omega} pair went unconsumed and was re-read as save-set data (rip=_rtld_global, the omega_driver signature).  x86_jmp_via_cell is medium-complete: TEXT byte-identical to the lea+jmp rax it replaces, BINARY through the ONE allocator's cell, which the m3 driver fills with &<FN>_body at main seal (scrip.c:1670, the LBL__ loop) — filled in the GVA-off regime too, which is exactly why the s110 alpha$<FN> route could not serve here. */
        return x86("comment", "IR_GOTO_DEFERRED (DEFINE-FOLD s55 ONE-SHOT: jmp body alpha, wires ride r10/r11, no chain, no reserve)")
             + x86_alpha()
             + x86("jmp", "[rip@cell + __]", body_cell, lbl.c_str())
             + x86_gamma();
    } }
    return x86("comment", "IR_GOTO_DEFERRED")
         + x86_alpha()
         + x86_align_enter()
         + x86_ro_load_q("rdi", 0)
         + x86("call", "rt_goto_transfer", (uint64_t)(uintptr_t)(void *)rt_goto_transfer)
         + x86_align_leave()
         + x86_jmp_id(1)
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "")
         + x86_deflabel_id(1)
         + x86_gamma();
}

#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
void rt_goto_transfer(const char *name);
void *rt_goto_resolve(const char *name);
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
std::string bb_goto_deferred() {
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
    { static int _gt = -1; if (_gt < 0) { const char * e = getenv("SCRIP_GOTO_TAIL"); _gt = (e && *e == '0') ? 0 : 1; }   /* ⭐⭐⭐ TAIL-TRANSFER ARM (row beauty-return-pair-shift) -- DEFAULT ON, measured; SCRIP_GOTO_TAIL=0 is the byte-identical revert hatch (the SCRIP_DEFINE_FOLD idiom).  FLIPPED ON THIS EVIDENCE, A/B at one binary: corpus board m3 332/5 -> 333/4 and m4 325/11 -> 326/10, the ONLY mover being 216_indirect_goto_computed -- the test literally named for this construct -- green in BOTH modes, with ZERO new reds across 337 programs x 2 modes; crosscheck agrees (216 leaves both fail lists, DIVERGE set identical).  beauty < one empty line goes from SIGSEGV-with-no-output to PRINTING ITS CORRECT IDENTITY LINE and then a diagnosed Error 121, and every red m1 ladder rung moves rc=139 -> rc=1.  WHY IT EXISTS: the arm below CALLS rt_goto_transfer, which runs the transferee NESTED via rt_chain_enter (five callee-save pushes + chain-scoped wires rcx/rdx pointing at its own landing).  That is right for an EVAL/CODE fragment that TERMINATES and wrong for a computed goto into a labelled statement that ends in `:(RETURN)` -- and per the manual (v3.7 p.130) RETURN is a reserved LABEL reached by a goto, so "goto into code that returns" is the ORDINARY idiom, not an exotic one.  beauty's dispatch `DIFFER(t) :S($('pp_' t))F(RETURN)` (beauty.sno:247/466) is exactly it: every pp_<t> target sits inside pp's own body and exits `:S(RETURN)`, so the transferee reaches the shared RETURN floater ~720B BELOW the activation's frontier, where the {γ,ω} pair its DEFINE α pushed is not, and the depth-exact `pop rcx` reads C save-set data -- rip=_rtld_global, the signature this very file already names two arms up.  THE SHAPE IS ARM 1'S, NOT A NEW ONE: resolve, then JUMP, wires riding r10/r11 exactly as the s55 DEFINE-FOLD arm does -- the only difference is that the address is resolved at runtime instead of baked, which is the "sealed-cell resolution / owed slice-2" this file's s111 note already planned.  ⛔ THE RTCC VENEER IS LOAD-BEARING HERE: x86("call", ...) banks r10/r11 into rtccb and restores them after the call (g_rtcc_on defaults ON), which is what keeps the wires alive across the resolver -- x86("call_bare") would silently drop them.  A NULL answer (the END sentinel, the only case rt_goto_resolve declines) falls through to γ, the same termination cascade the nested arm produced by returning. */
    if (_gt) {
        return x86("comment", "IR_GOTO_DEFERRED (TAIL-TRANSFER: resolve then jmp at the SITE's depth, wires ride r10/r11 -- arm 1's shape with a runtime-resolved target)")
             + x86_alpha()
             + x86_align_enter()
             + x86_ro_load_q("rdi", 0)
             + x86("call", "rt_goto_resolve", (uint64_t)(uintptr_t)(void *)rt_goto_resolve)
             + x86_align_leave()
             + x86("test", "rax", "rax")
             + x86_jcc_id("jz", 1)
             + IF(_.op_zgpop > 0, x86("add", "rsp", (long)_.op_zgpop))   /* ⭐ THE SECOND HALF, MEASURED: tail-jumping at the SITE's depth is still wrong, because the site is MID-STATEMENT -- beauty's dispatch statement has 3 live operand cells (the constructed name) standing between the goto box and the activation frontier, so the transferee's `:(RETURN)` popped a string DESCR tagword (rip=rcx=0x0000000800000002, tag 2 / len 8) instead of the pair.  A NORMAL goto never meets this: it rides `statement_end: add rsp,K; jmp <label>`, i.e. the statement's own terminal release happens FIRST and the jump leaves from the frontier.  The deferred goto transfers from inside the statement, so it must perform that same release itself.  op_zgpop IS the statement-terminal release the planner already staged for this node; on this arm we jump away and never reach x86_gamma(), whose X86H_JMP hook is the only other consumer, so emitting it here is a MOVE, not a duplication. */
             + x86("jmp", "rax")
             + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "")
             + x86_deflabel_id(1)
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

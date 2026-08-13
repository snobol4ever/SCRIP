#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
void rt_goto_transfer(const char *name);
void rt_chain_enter(void *fn);
}
#include "x86_asm.h"
/* DF_LANDING_RESERVE — bytes the DEFINE-FOLD site reserves for the callee's RETURN-path landing.  ⛔ THIS IS A
 * MEASURED FLOOR PLUS MARGIN, NOT A DERIVED SIZE — labelled so nobody reads it as exact.  Measured s54 by
 * bisection on roman.sno (the deepest witness: recursive ROMAN + REPLACE-of-recursive-result + a second DEFINE
 * calling it): 224 RED, 256 GREEN, every value ≥256 byte-identical to the sbl oracle.  ⭐ THE FLOOR IS EXACTLY
 * `char lname[256]` — the dominant term of the rt_goto_transfer C frame this fold deletes (runtime_eval.c, the
 * "LBL__%s" compose buffer).  That correspondence is the whole mechanism in one number: the classic arm parked
 * a 256-byte buffer between the transfer site and the chain, the callee's γ-epilogue landing wrote its frame
 * over that dead buffer, and folding the call away removed the buffer while leaving the write.  512 = 2× the
 * floor.  ⛔ The exact requirement is the max stack footprint of the whole landing path (slim epilogue 88B +
 * rt_value_trail_tidy_dead_window + the Σ/monitor restores it calls); bounding that properly is the next probe
 * and would let this constant shrink to a computed value — or vanish, if the transfer becomes a true ONE-SHOT
 * `jmp` with site-set wires instead of `call rt_chain_enter`, which interposes no frame to protect at all. */
#define DF_LANDING_RESERVE 512L
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
    { static int _df = -1; if (_df < 0) { const char * e = getenv("SCRIP_DEFINE_FOLD"); _df = (e && *e == '1') ? 1 : 0; }
    if (_df && _.op_ival == 1 && _.op_sval && _.op_sval[0] && _.op_sval[0] != '$' && MEDIUM_TEXT) {
        /* ⭐ DEFINE-FOLD slice 1 (s53, Lon directive): the DEFINE stub's entry hop is a CONSTANT -- fold the
         * per-call rt_goto_transfer string lookup into a direct chain transfer.  The AB template's body-jmp
         * is the precedent (static-direct fold, label known at emit time).  rt_chain_enter is KEPT: it is the
         * chain protocol itself (five callee-save pushes + the fall-off landing wire), not part of the lookup.
         * TEXT: gas resolves proc_LBL__<name>_α at assembly time -- zero ordering risk, zero runtime cost.
         * BINARY (slice 2, specified in GOAL-RBP-EARN s53 rung): resolve via a one-time fn cell exactly as
         * bb_func_activate's fn_cell does (rt_proc_get_fn at the post-proc-loop hook writes the JIT address);
         * until that lands m3 keeps the transfer arm below -- correctness identical, one lookup per call. */
        std::string lbl = std::string("proc_LBL__") + _.op_sval + "_\xce\xb1";
        long _res = DF_LANDING_RESERVE; { const char *ce = getenv("SCRIP_DF_RESERVE"); if (ce) _res = atol(ce); _res = (_res + 15) & ~15L; }
        return x86("comment", "IR_GOTO_DEFERRED (DEFINE-FOLD: direct chain transfer + callee-return LANDING RESERVE)")
             + x86_alpha()
             + x86_align_enter()
             + (_res ? x86("sub", "rsp", (long)_res) : std::string())   /* ⭐ THE RESERVE IS THE FOLD'S OWN COST, NOT A CUSHION (s54, gdb-convicted): the RETURN/FRETURN floater restores rsp WHOLESALE to the wire-adopt's banked blob-entry rsp B and jmps the γ wire, landing on rt_proc_call_epilogue_slim_γ, which does `sub rsp,88` from B and writes its 88B frame at [B-88,B) (rtx_call.S:255 — result at [rsp+0..15], the popped pcall record's private copy at [rsp+16..79]).  Under the classic arm rt_goto_transfer's own C frame (lname[256] + spills, ~296B) sat between the transfer site and the chain, so that landing wrote over DEAD C stack.  Folding the call away deletes that frame, and the landing then writes over rt_chain_enter's five callee-save pushes AND its return address — measured under gdb, software watchpoint on the saved-r12 slot, writer = rt_proc_call_epilogue_slim_γ+98 `movdqu [rsp+16],xmm0` at rsp == the chain's entry rsp.  The fold did not create this write; it removed the accident that was absorbing it.  So the glue reserves the landing explicitly: one `sub rsp,K` at the site, K a 16-multiple ≥ the epilogue frame (88) plus the stub blob's own live carve.  The reserve is never released here — the floater's wholesale restore to B discards it by construction, exactly as it discards the C frame it replaces (x86_align_leave below is on the fall-off path only). */
             + x86("lea", "rdi", std::string("[rip + __]"), (uint64_t)0, lbl.c_str())
             + x86("call", "rt_chain_enter", (uint64_t)(uintptr_t)(void *)rt_chain_enter)
             + (_res ? x86("add", "rsp", (long)_res) : std::string())   /* fall-off path only (chain returned through rt_chain_enter's ret rather than the floater) — exact inverse, paired */
             + x86_align_leave()
             + x86_jmp_id(1)
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

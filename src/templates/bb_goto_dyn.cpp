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
        return x86("comment", "IR_GOTO_DEFERRED (DEFINE-FOLD: direct chain transfer)")
             + x86_alpha()
             + x86_align_enter()
             + x86("lea", "rdi", std::string("[rip + __]"), (uint64_t)0, lbl.c_str())
             + x86("call", "rt_chain_enter", (uint64_t)(uintptr_t)(void *)rt_chain_enter)
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

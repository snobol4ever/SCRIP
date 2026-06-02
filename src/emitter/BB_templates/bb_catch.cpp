/* bb_catch.cpp — BB template for IR_CATCH: Prolog catch/3.  x86() self-encoding (template-revamp PL-RV-4,
   2026-06-02, Opus 4.8).  MODE-2 owns this construct (bb_exec.c IR_CATCH executor: setjmp/Resolve_CatchFrame,
   goal run, longjmp recovery, rethrow on mismatch).  Mode-3 routes the oracle.  In MODE-4 catch/3 is EXCISED
   by the rich gate (WAM-CP-13 will implement the real CP-barrier emit), so the live mode-4 emission is this
   STUB: α-label, then fail straight to ω; β re-entry also fails to ω.  The β definition is load-bearing —
   when a catch/3 is the entry node of a callee block the block's redo label jmps to .Lplpb%d_β.  pBB-FREE:
   reads only _ (ports/labels); no operands needed for the stub.  The dead MEDIUM_BINARY twin (which read the
   bb_catch_state_t* sidecar from pBB->ival, called rt_pl_catch, and hand-counted its rel32 patch offsets via
   b.size()) is DELETED: Prolog's live catch path is mode-2 (the oracle), never the mode-3/4 BINARY arm —
   verified zero BINARY-arm fires across the full rung suite via abort-probe.  x86 only. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_catch_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment("# BOX RESOLVE_CATCH (mode-4 STUB — WAM-CP-13 will implement; today fails through)  [x86() self-encoding]"))
             + x86("jmp", PORT_OMEGA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_catch(void) { bb_emit_x86(bb_catch_str()); }

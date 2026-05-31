/* bb_proc.cpp — BB template for per-procedure named label + inline body (generic, language-ignorant).
   IBB-5 / GOAL-ICON-BB. One file per BB kind per RULES.md.
   LANGUAGE-IGNORANT: a proc-level BB emits `<procname>:` as a global symbol, then inlines
   the body BB sub-graph. γ from body propagates to caller's γ; ω to caller's ω. Same
   contract for any user-defined procedure in any frontend — Icon procedure, Prolog clause,
   Raku sub, SNOBOL4 FUNCTION. No g_lang refs in the body.
   STUB at file-creation time: returns empty bytes. Real label emission + body inlining land
   in IBB-5 follow-on. x86 only — IS_JVM/JS/NET/WASM stub. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_proc_str(IR_t * pBB, bb_bin_t & bin) { (void)pBB; bin = {}; return std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_proc(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_proc_str(pBB, bin), bin); }

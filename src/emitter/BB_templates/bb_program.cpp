/* bb_program.cpp — BB template for the program-level preamble (generic, language-ignorant).
   IBB-5 / GOAL-ICON-BB. One file per BB kind per RULES.md.
   LANGUAGE-IGNORANT: in modes 3/4 the program-preamble box loads the root data pointer
   (`lea r10, [rip + Δ_root_data]`) and jmps into the root BB's α label. The shape is the
   same regardless of frontend — Icon, Prolog, Raku, SNOBOL4 all share this entry contract
   per ARCH-x86.md. No g_lang refs in the body.
   STUB at file-creation time: returns empty bytes. Real lea + jmp land in IBB-5 follow-on.
   x86 only — IS_JVM/JS/NET/WASM stub. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_program_str(BB_t * pBB, bb_bin_t & bin) { (void)pBB; bin = {}; return std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_program(BB_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_program_str(pBB, bin), bin); }

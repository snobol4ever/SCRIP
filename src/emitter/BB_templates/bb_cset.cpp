/* bb_cset.c — grouped BB template for the BB_CSET_* family.
   Per HQ Invariant #10 (grouped templates): the four character-set ops
   (IR_CSET_COMPL / IR_CSET_UNION / IR_CSET_DIFF / IR_CSET_INTER) share an
   identical emit shape — an honest no-op stub across all five backends — so a
   single grouped template fn handles all of them. The dispatch switch in
   emit_core.c already discriminates the opcode; every CSET arm routes here and
   the emitted text is identical (empty) for all four, so no per-op branch is
   needed inside the template.
   These kinds are handled today by the AST/runtime path in src/lower/bb_exec.c
   and src/lower/lower_*.c; this template slot keeps the BB layer total over
   IR_e so future native-codegen work has a place to land. Phase B fills the
   arms when a frontend lowers directly to native code for these ops. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cset_str(IR_t * pBB) { (void)pBB; return std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cset(IR_t * pBB) { bb_cset_str(pBB); }

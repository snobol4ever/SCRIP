/* bb_proc_gen.cpp — BB template for BB_PROC_GEN (ICN-T ladder).
   STUB — x86 Byrd-box inline asm not yet written. RULES.md: one file per BB kind. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_proc_gen_str(BB_t * pBB, bb_bin_t & bin) { (void)pBB; bin = {}; return std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_proc_gen(BB_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_proc_gen_str(pBB, bin), bin); }

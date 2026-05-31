/* bb_gen_alt.cpp — BB template for BB_GEN_ALT (ICN-T ladder).
   STUB — x86 Byrd-box inline asm not yet written. RULES.md: one file per BB kind. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_gen_alt_str(IR_t * pBB, bb_bin_t & bin) { (void)pBB; bin = {}; return std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_gen_alt(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_gen_alt_str(pBB, bin), bin); }

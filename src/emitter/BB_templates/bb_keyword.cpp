/* bb_keyword.cpp — BB template for BB_KEYWORD (ICN-T ladder).
   STUB — x86 Byrd-box inline asm not yet written. RULES.md: one file per BB kind. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_keyword_str(IR_t * pBB, bb_bin_t & bin) { (void)pBB; bin = {}; return std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_keyword(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_keyword_str(pBB, bin), bin); }

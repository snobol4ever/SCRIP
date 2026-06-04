#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_scan_many(IR_t * pBB) { (void)pBB; x86_begin(); bb_emit_x86(x86_bomb("bb_scan_many: stub — ICN-SCAN ladder template not yet written")); }

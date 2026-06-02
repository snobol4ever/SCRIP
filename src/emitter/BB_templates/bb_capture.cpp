#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_capture(IR_t * pBB, int imm) {
    (void)pBB;
    (void)imm;
    bb_emit_x86(x86_bomb("bb_capture: TEMPLATE-REVAMP not yet converted (was offset-table)"));
}

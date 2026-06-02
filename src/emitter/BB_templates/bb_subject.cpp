#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_subject(IR_t * pBB) {
    (void)pBB;
    bb_emit_x86(x86_bomb("bb_subject: TEMPLATE-REVAMP not yet converted (was offset-table)"));
}

#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_limit_begin(IR_t * pBB) {
    (void)pBB;
    bb_emit_x86(x86_bomb("bb_limit_begin: TEMPLATE-REVAMP not yet converted (was offset-table)"));
}
extern "C" void bb_limit_inc(IR_t * pBB) {
    (void)pBB;
    bb_emit_x86(x86_bomb("bb_limit_inc: TEMPLATE-REVAMP not yet converted (was offset-table)"));
}
extern "C" void bb_limit_more(IR_t * pBB) {
    (void)pBB;
    bb_emit_x86(x86_bomb("bb_limit_more: TEMPLATE-REVAMP not yet converted (was offset-table)"));
}
extern "C" void bb_limit(IR_t * pBB) {
    (void)pBB;
    bb_emit_x86(x86_bomb("bb_limit: TEMPLATE-REVAMP not yet converted (was offset-table)"));
}

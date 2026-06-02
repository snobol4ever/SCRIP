#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_eps(IR_t * pBB) {
    (void)pBB;
    bb_emit_x86(x86_bomb("bb_nfa_eps: TEMPLATE-REVAMP not yet converted (was offset-table)"));
}
extern "C" void bb_nfa_cap_open(IR_t * pBB) {
    (void)pBB;
    bb_emit_x86(x86_bomb("bb_nfa_cap_open: TEMPLATE-REVAMP not yet converted (was offset-table)"));
}
extern "C" void bb_nfa_cap_close(IR_t * pBB) {
    (void)pBB;
    bb_emit_x86(x86_bomb("bb_nfa_cap_close: TEMPLATE-REVAMP not yet converted (was offset-table)"));
}

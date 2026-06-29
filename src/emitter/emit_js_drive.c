#include "emit_drive.h"
#include "IR.h"
#include <stdio.h>
#include <stdlib.h>
/*====================================================================================================================*/
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_js_drive(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    (void)lbl_γ; (void)lbl_ω; (void)lbl_β;
    fprintf(stderr, "FATAL emit_js_drive: the JS backend is HIBERNATING under X86-ONLY (op=%d). This per-backend driver is a skeleton kept on disk; it is not wired into the active build. Bring up the JS arm before routing here.\n", nd ? (int)nd->op : -1);
    abort();
}

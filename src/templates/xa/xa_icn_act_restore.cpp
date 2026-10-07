#include <string>
#include <cstdint>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
extern "C" int zls_g_block_args(const IR_graph_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_icn_act_restore_call_line(int lbl) {
    (void)lbl;
    return x86_bomb("xa_icn_act_restore_call_line: g_icn_act is deleted -- the caller's line and file at a callee's return or failure need a home on the stack");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int xa_icn_block_size(void) {
    return (g_emit_cfg && g_emit_cfg->icn_cells_graph && zls_g_block_args(g_emit_cfg)) ? 16 * g_emit_cfg->nparams : 0;
}

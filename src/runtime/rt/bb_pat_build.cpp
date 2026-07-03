extern "C" {
#include "contracts/IR.h"
#include "emitter/emit.h"
#include "machine/bb_pool.h"
}
#include "emit.h"
extern "C" {
extern int g_frame_active;
void rt_gvar_assign_pat(const char *name, void *head);
void lc_γ_to(IR_t *nd, IR_t *t);
void lc_ω_to(IR_t *nd, IR_t *t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_build_len_blob(const char *name, int I) {
    bb_pool_init();
    IR_graph_t *g = IR_alloc(8);
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *nd    = IR_node_alloc(g, IR_MATCH_LEN);
    lc_γ_to(nd, PSUCC);
    lc_ω_to(nd, PFAIL);
    IR_LIT(nd).ival = (int64_t)I;
    IR_graph_t *saved_cfg = g_emit_cfg;
    int saved_fa = g_frame_active;
    g_emit_cfg   = g;
    g_frame_active = 1;
    bb_box_fn fn = bb_build_flat(nd);
    g_emit_cfg   = saved_cfg;
    g_frame_active = saved_fa;
    IR_free(g);
    if (fn) rt_gvar_assign_pat(name, (void *)fn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_build_break_blob(const char *name, const char *cset) {
    bb_pool_init();
    IR_graph_t *g = IR_alloc(8);
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *nd    = IR_node_alloc(g, IR_MATCH_BREAK);
    lc_γ_to(nd, PSUCC);
    lc_ω_to(nd, PFAIL);
    IR_LIT(nd).sval = cset;
    IR_graph_t *saved_cfg = g_emit_cfg;
    int saved_fa = g_frame_active;
    g_emit_cfg   = g;
    g_frame_active = 1;
    bb_box_fn fn = bb_build_flat(nd);
    g_emit_cfg   = saved_cfg;
    g_frame_active = saved_fa;
    IR_free(g);
    if (fn) rt_gvar_assign_pat(name, (void *)fn);
}

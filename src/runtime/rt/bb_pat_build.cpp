extern "C" {
#include "contracts/IR.h"
#include "contracts/ast.h"
#include "machine/bb_pool.h"
}
#include "emit.h"
extern "C" {
extern IR_graph_t *g_emit_cfg;
extern int g_frame_active;
void rt_gvar_assign_pat(const char *name, void *head);
void lc_γ_to(IR_t *nd, IR_t *t);
void lc_ω_to(IR_t *nd, IR_t *t);
bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
void optimizer_run(IR_graph_t * g);
void ir_drive_slot_assign(IR_graph_t * g);
void zls_reset(void);
IR_graph_t * sno_pat_tree_graph_rt(const tree_t * pat);
int zls_g_region(const IR_graph_t * g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZS-2 (Lon s58): blobs are jmp-entered NEW ACTIVATIONS — each self-allocates on rsp, so the frame size is a
 * PROLOGUE constant computed here BEFORE emission (32B wire header + zls region, rounded to 16), and the
 * rt_fn_frame_bytes registry has no consumer on this path anymore (the caller no longer allocates anything). */
static int bb_jmp_entry_ktotal(const IR_graph_t *g) {
    int rg = g ? zls_g_region(g) : -1;
    if (rg <= 0) rg = 4096;   /* PROC_FRAME_QWORDS*8 — the same default rt_fn_frame_bytes served the call-regime allocator */
    return (32 + rg + 15) & ~15;
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
    g_emit.flat_jmp_entry = 1; g_emit.flat_frame_bytes = bb_jmp_entry_ktotal(g);
    bb_box_fn fn = emit_chain(g->entry = nd, NULL, "rtlen");
    g_emit.flat_jmp_entry = 0; g_emit.flat_frame_bytes = 0;
    g_emit_cfg   = saved_cfg;
    g_frame_active = saved_fa;
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
    g_emit.flat_jmp_entry = 1; g_emit.flat_frame_bytes = bb_jmp_entry_ktotal(g);
    bb_box_fn fn = emit_chain(g->entry = nd, NULL, "rtbrk");
    g_emit.flat_jmp_entry = 0; g_emit.flat_frame_bytes = 0;
    g_emit_cfg   = saved_cfg;
    g_frame_active = saved_fa;
    if (fn) rt_gvar_assign_pat(name, (void *)fn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void *bb_compile_pat_tree(const void *tv) {
    bb_pool_init();
    zls_reset();
    IR_graph_t *g = sno_pat_tree_graph_rt((const tree_t *)tv);
    if (!g || !g->entry) return (void *)0;
    optimizer_run(g);
    ir_drive_slot_assign(g);
    IR_graph_t *saved_cfg = g_emit_cfg;
    int saved_fa = g_frame_active;
    g_emit_cfg = g;
    g_frame_active = 1;
    g_emit.flat_jmp_entry = 1; g_emit.flat_frame_bytes = bb_jmp_entry_ktotal(g);
    bb_box_fn fn = emit_chain(g->entry, NULL, "rtpat");
    g_emit.flat_jmp_entry = 0; g_emit.flat_frame_bytes = 0;
    g_emit_cfg = saved_cfg;
    g_frame_active = saved_fa;
    return (void *)fn;
}

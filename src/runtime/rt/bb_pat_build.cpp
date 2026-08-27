extern "C" {
#include "IR.h"
#include "ast.h"
#include "bb_pool.h"
}
#include "emit.h"
extern "C" void fc_tables_reset(void);
extern "C" void bb_src_reset(void);
extern "C" {
extern IR_graph_t *g_emit_cfg;
extern int g_frame_active;
void rt_gvar_assign_pat(const char *name, void *head);
void rt_gvar_assign_pat_sz(const char *name, void *fn, int64_t zsz, int32_t zstatic);
void lc_γ_to(IR_t *nd, IR_t *t);
void lc_ω_to(IR_t *nd, IR_t *t);
bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
void optimizer_run(IR_graph_t * g);
void ir_drive_slot_assign(IR_graph_t * g);
void zls_reset(void);
IR_graph_t * sno_pat_tree_graph_rt(const tree_t * pat);
int zls_g_region(const IR_graph_t * g);
int sn4_choice_rbp_off(void);
int sn4_pt_frame(void);
void sn4_blob_choice_scan(int *, int *, int *);
extern int g_flat_frame_floor;
}
#include <cstdio>
#include <cstdlib>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bb_jmp_entry_ktotal(const IR_graph_t *g) {
    int rg = g ? zls_g_region(g) : -1;
    if (rg <= 0) rg = 4096;
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
    int kt = bb_jmp_entry_ktotal(g);
    g_emit.flat_jmp_entry = 1; g_emit.flat_frame_bytes = kt;
    bb_box_fn fn = emit_chain(g->entry = nd, NULL, "rtlen");
    g_emit.flat_jmp_entry = 0; g_emit.flat_frame_bytes = 0;
    g_emit_cfg   = saved_cfg;
    g_frame_active = saved_fa;
    if (fn) rt_gvar_assign_pat_sz(name, (void *)fn, (int64_t)kt, (zls_g_region(g) > 0) ? 1 : 0);
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
    int kt = bb_jmp_entry_ktotal(g);
    g_emit.flat_jmp_entry = 1; g_emit.flat_frame_bytes = kt;
    bb_box_fn fn = emit_chain(g->entry = nd, NULL, "rtbrk");
    g_emit.flat_jmp_entry = 0; g_emit.flat_frame_bytes = 0;
    g_emit_cfg   = saved_cfg;
    g_frame_active = saved_fa;
    if (fn) rt_gvar_assign_pat_sz(name, (void *)fn, (int64_t)kt, (zls_g_region(g) > 0) ? 1 : 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bb_graph_zstatic(const IR_graph_t *g) {
    for (int k = 0; k < g->n; k++) { const IR_t *c = g->all[k]; if (!c) continue; if (c->op == IR_MATCH_DEFER || c->op == IR_MATCH_VALUE) return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void *bb_compile_pat_tree_sz(const void *tv, int64_t *zsz, int32_t *zstatic) {
    bb_pool_init();
    zls_reset();
    fc_tables_reset();
    bb_src_reset();
    IR_graph_t *g = sno_pat_tree_graph_rt((const tree_t *)tv);
    if (!g || !g->entry) { if (zsz) *zsz = 0; if (zstatic) *zstatic = 0; return (void *)0; }
    optimizer_run(g);
    ir_drive_slot_assign(g);
    IR_graph_t *saved_cfg = g_emit_cfg;
    int saved_fa = g_frame_active;
    extern int g_gva_active;
    int saved_gva = g_gva_active;
    g_emit_cfg = g;
    g_frame_active = 1;
    g_gva_active = 0;
    int kt = bb_jmp_entry_ktotal(g) + 16;
    g_emit.flat_jmp_entry = 1; g_emit.flat_frame_bytes = kt;
    g_emit.flat_pat = 1;
    if (getenv("SCRIP_RTPAT_DIAG")) { int _nc = 0, _lf = 0, _fn = 0; sn4_blob_choice_scan(&_nc, &_lf, &_fn); fprintf(stderr, "[RTPAT-DIAG] n=%d kt=%d cro=%d ptf=%d floor=%d nc=%d lf=%d fn=%d\n", g->n, kt, sn4_choice_rbp_off(), sn4_pt_frame(), g_flat_frame_floor, _nc, _lf, _fn); }
    bb_box_fn fn = emit_chain(g->entry, NULL, "rtpat");
    g_emit.flat_jmp_entry = 0; g_emit.flat_frame_bytes = 0;
    g_emit.flat_pat = 0;
    g_emit_cfg = saved_cfg;
    g_frame_active = saved_fa;
    g_gva_active = saved_gva;
    if (zsz) *zsz = (int64_t)(fn ? kt : 0);
    if (zstatic) *zstatic = (fn && zls_g_region(g) > 0) ? bb_graph_zstatic(g) : 0;
    return (void *)fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void *bb_compile_pat_tree(const void *tv) { return bb_compile_pat_tree_sz(tv, (int64_t *)0, (int32_t *)0); }

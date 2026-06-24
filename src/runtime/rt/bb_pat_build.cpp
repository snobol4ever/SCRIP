extern "C" {
#include "contracts/IR.h"
#include "emitter/emit_bb.h"
#include "emitter/emit_globals.h"
#include "machine/bb_pool.h"
}
#include "box_state.h"   /* bb_match_kids_state_t — the IR_MATCH_CAT kids channel */
extern "C" {
extern int g_frame_active;
void rt_gvar_assign_pat(const char *name, void *head);
void lc_γ_to(IR_t *nd, IR_t *t);
void lc_ω_to(IR_t *nd, IR_t *t);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_build_len_blob(const char *name, int I) {
    bb_pool_init();
    IR_graph_t *g = IR_alloc(8, 0);
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
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_build_break_blob(const char *name, const char *cset) {
    bb_pool_init();
    IR_graph_t *g = IR_alloc(8, 0);
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
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_build_break_capture_blob(const char *name, const char *cset, const char *capvar) {
    bb_pool_init();
    IR_graph_t *g = IR_alloc(8, 0);
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *inner = IR_node_alloc(g, IR_MATCH_BREAK);
    IR_t *cap   = IR_node_alloc(g, IR_MATCH_ASSIGN_COND);
    IR_LIT(inner).sval = cset;
    lc_γ_to(inner, PSUCC);
    lc_ω_to(inner, PFAIL);
    IR_LIT(cap).sval = capvar;
    IR_LIT(cap).ival = 0;
    ir_operand_push(cap, inner);
    lc_γ_to(cap, PSUCC);
    lc_ω_to(cap, PFAIL);
    IR_graph_t *saved_cfg = g_emit_cfg;
    int saved_fa = g_frame_active;
    g_emit_cfg   = g;
    g_frame_active = 1;
    bb_box_fn fn = bb_build_flat(cap);
    g_emit_cfg   = saved_cfg;
    g_frame_active = saved_fa;
    IR_free(g);
    if (fn) rt_gvar_assign_pat(name, (void *)fn);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* TR-CAT/FZ-1: shared matcher-graph constructor for the BREAK(cset) . capvar lit shape — the SAME
   IR_MATCH_* graph the inline lowerer produces for `BREAK . VAR LIT`:
     IR_MATCH_CAT  kids=[ IR_MATCH_ASSIGN_COND[capvar] -> IR_MATCH_BREAK(cset),  IR_MATCH_LIT(lit) ]
   The CAT is self-contained via its kids channel (flat_drive_cat reads bb_match_nkids/bb_match_kid,
   NOT operands); flat_drive_cat_arms sequences the two arms. Caller owns zk + kids (must outlive the
   bb_build_flat / codegen_flat_build call that follows); returns the CAT entry node. */
static IR_t *sno_break_cap_lit_graph(IR_graph_t *g, const char *cset, const char *capvar, const char *lit, bb_match_kids_state_t *zk, IR_t *kids[2]) {
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *brk   = IR_node_alloc(g, IR_MATCH_BREAK);
    IR_t *cap   = IR_node_alloc(g, IR_MATCH_ASSIGN_COND);
    IR_t *litn  = IR_node_alloc(g, IR_MATCH_LIT);
    IR_t *cat   = IR_node_alloc(g, IR_MATCH_CAT);
    IR_LIT(brk).sval = cset;
    lc_γ_to(brk, PSUCC); lc_ω_to(brk, PFAIL);
    IR_LIT(cap).sval = capvar;
    IR_LIT(cap).ival = 0;
    ir_operand_push(cap, brk);
    lc_γ_to(cap, PSUCC); lc_ω_to(cap, PFAIL);
    IR_LIT(litn).sval = lit;
    lc_γ_to(litn, PSUCC); lc_ω_to(litn, PFAIL);
    lc_γ_to(cat, PSUCC); lc_ω_to(cat, PFAIL);
    kids[0] = cap; kids[1] = litn;
    zk->kids = kids; zk->nkids = 2;
    IR_EXEC(cat).counter = (int64_t)(intptr_t)zk;
    return cat;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* RUNTIME builder (mode-3/4 current default path): build the shared graph, seal it into the RX pool via
   bb_build_flat, store the sealed head as the PAT cell's DT_P. Fired by the emitted call in bb_pattern_cat. */
extern "C" void bb_build_break_cap_lit_blob(const char *name, const char *cset, const char *capvar, const char *lit) {
    bb_pool_init();
    IR_graph_t *g = IR_alloc(8, 0);
    bb_match_kids_state_t zk; IR_t *kids[2];
    IR_t *cat = sno_break_cap_lit_graph(g, cset, capvar, lit, &zk, kids);
    IR_graph_t *saved_cfg = g_emit_cfg;
    int saved_fa = g_frame_active;
    g_emit_cfg     = g;
    g_frame_active = 1;
    bb_box_fn fn = bb_build_flat(cat);
    g_emit_cfg     = saved_cfg;
    g_frame_active = saved_fa;
    IR_free(g);
    if (fn) rt_gvar_assign_pat(name, (void *)fn);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* FZ-1 COMPILE-TIME SEAL (BINARY / mode-3 slab): seal the BREAK.cap.lit blob at compile time and return its
   head. Guards g_emit_cfg/g_frame_active AND g_flat_node_id so the seal is safe to run amid the main
   program's emission state (the relocation FZ-2 performs). No cell store here — FZ-2's IR_REF_INVARIANT
   box performs the DT_P store at the assignment site. Returns NULL on overflow (caller falls back). */
extern "C" bb_box_fn sno_freeze_break_cap_lit_bin(const char *cset, const char *capvar, const char *lit) {
    bb_pool_init();
    IR_graph_t *g = IR_alloc(8, 0);
    bb_match_kids_state_t zk; IR_t *kids[2];
    IR_t *cat = sno_break_cap_lit_graph(g, cset, capvar, lit, &zk, kids);
    IR_graph_t *saved_cfg = g_emit_cfg;
    int saved_fa = g_frame_active;
    int saved_nid = g_flat_node_id;
    g_emit_cfg     = g;
    g_frame_active = 1;
    bb_box_fn fn = bb_build_flat(cat);
    g_emit_cfg     = saved_cfg;
    g_frame_active = saved_fa;
    g_flat_node_id = saved_nid;
    IR_free(g);
    return fn;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* FZ-1 COMPILE-TIME SEAL (TEXT / mode-4 blob): emit the BREAK.cap.lit blob as assembly under <label> via the
   shared codegen_flat_body path. Guards g_emit_cfg/g_frame_active/g_flat_node_id. Returns codegen_flat_build's
   rc. FZ-2 wraps this with the adjacent RO (interned cset/lit) emission + the IR_REF_INVARIANT DT_P store. */
extern "C" int sno_freeze_break_cap_lit_text(const char *cset, const char *capvar, const char *lit, FILE *out, const char *label) {
    IR_graph_t *g = IR_alloc(8, 0);
    bb_match_kids_state_t zk; IR_t *kids[2];
    IR_t *cat = sno_break_cap_lit_graph(g, cset, capvar, lit, &zk, kids);
    IR_graph_t *saved_cfg = g_emit_cfg;
    int saved_fa = g_frame_active;
    int saved_nid = g_flat_node_id;
    g_emit_cfg     = g;
    g_frame_active = 1;
    int rc = codegen_flat_build(cat, out, label);
    g_emit_cfg     = saved_cfg;
    g_frame_active = saved_fa;
    g_flat_node_id = saved_nid;
    IR_free(g);
    return rc;
}

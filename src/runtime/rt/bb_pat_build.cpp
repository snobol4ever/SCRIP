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
/* TR-CAT: stored concatenation builder for the BREAK(cset) . capvar lit shape.
   Builds the matcher graph the INLINE lowerer produces for `BREAK . VAR LIT`:
     IR_MATCH_CAT  kids=[ IR_MATCH_ASSIGN_COND[capvar] -> IR_MATCH_BREAK(cset),  IR_MATCH_LIT(lit) ]
   The CAT is made self-contained by populating its kids channel (flat_drive_cat reads
   bb_match_nkids/bb_match_kid, NOT operands); flat_drive_cat_arms then sequences the two
   arms — the capture commits the BREAK span (e.g. "alpha"), then LIT runs after it, with
   inter-arm backtrack wired by the driver via labels. bb_build_flat's the CAT entry. */
extern "C" void bb_build_break_cap_lit_blob(const char *name, const char *cset, const char *capvar, const char *lit) {
    bb_pool_init();
    IR_graph_t *g = IR_alloc(8, 0);
    IR_t *PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t *PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t *brk   = IR_node_alloc(g, IR_MATCH_BREAK);
    IR_t *cap   = IR_node_alloc(g, IR_MATCH_ASSIGN_COND);
    IR_t *litn  = IR_node_alloc(g, IR_MATCH_LIT);
    IR_t *cat   = IR_node_alloc(g, IR_MATCH_CAT);
    /* BREAK(cset): γ→SUCCEED keeps gather_lowered_cat_arms from pulling LIT into the capture span */
    IR_LIT(brk).sval = cset;
    lc_γ_to(brk, PSUCC); lc_ω_to(brk, PFAIL);
    /* ASSIGN_COND[capvar] wrapping BREAK (ival=0 ⇒ conditional '.') */
    IR_LIT(cap).sval = capvar;
    IR_LIT(cap).ival = 0;
    ir_operand_push(cap, brk);
    lc_γ_to(cap, PSUCC); lc_ω_to(cap, PFAIL);
    /* trailing literal */
    IR_LIT(litn).sval = lit;
    lc_γ_to(litn, PSUCC); lc_ω_to(litn, PFAIL);
    /* CAT entry — kids channel = [capture, lit]; alive only across bb_build_flat */
    lc_γ_to(cat, PSUCC); lc_ω_to(cat, PFAIL);
    IR_t *kids[2] = { cap, litn };
    bb_match_kids_state_t zk; zk.kids = kids; zk.nkids = 2;
    IR_EXEC(cat).counter = (int64_t)(intptr_t)&zk;
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

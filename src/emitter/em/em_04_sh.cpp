#include "emitter/em/em_head.h"
#include "emitter/emit_state.h"
#include "emitter/emit_medium.h"
#include "emitter/emit_knobs.h"
#include "emitter/emit_regime.h"
#include "ir/bb_box.h"
#include "emitter/bb_label.h"
#include "ir/gc_frame_map.h"
#include "ir/dtp.h"
#include "driver/scrip_sm.h"
#include "ir/stage2.h"
#include "templates/xa/xa_templates.h"
#include "planner/g_planner.h"
#include "ir/ir_fwd.h"
#include "ir/ct_vec.h"
#include "ir/descr.h"
#include "ir/ir_graph.h"
#include "runtime/core/g_runtime_entry.h"
#include "runtime/core/g_runtime_vec.h"
#include "runtime/core/g_runtime_abi.h"
#include "runtime/keywords.h"
#include "ir/ir_node_index.h"
#include "templates/bb/bb_templates.h"
extern "C" {
#include "templates/x86/x86_zverify.h"
}
extern "C" {
#include "planner/zeta_plan.h"
}
extern "C" {
#include "planner/act_words.h"
}
extern "C" {
#include "lower/lower_common.h"
}
extern "C" {
#include "runtime/core/comm_var.h"
}
extern "C" {
#include "ir/stno_map.h"
}
extern "C" {
#include "optimizer/gva_collect.h"
}
extern "C" {
#include "optimizer/proc_collect.h"
}
#include "emitter/em/em_01_sh.h"
#include "emitter/em/em_02_sh.h"
#include "emitter/em/em_03_sh.h"
#include "emitter/em/em_04_sh.h"
#include "emitter/em/em_05_sh.h"
extern "C" { const char * bb_src_of(const IR_t * nd); }
extern "C" { int bb_line_of(const IR_t * nd); }
extern "C" { const char * stmt_src_get_file(void); }
extern "C" { int stmt_src_nlines(void); }
extern "C" { static int zrow_partoff(const IR_t * nd, int j); }
extern "C" { static int zrow_own(const IR_t * nd); }
extern "C" { static int zrow_xoff(const IR_t * nd); }
extern "C" { static int zrow_partvs(const IR_t * nd, int j); }
extern "C" { static int zrow_under(int ri); }
extern "C" { void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β); }
extern "C" { static int * xci_ints(cv_t * v, uint32_t n); }
extern "C" { static int binop_operand_real_static(IR_graph_t *g, IR_t *o, int depth); }
extern "C" { extern int is_global(const char *name); }
extern "C" { extern int gva_index_of(const char *name); }
extern "C" { int sn4_arbno_reentry(void); }
extern "C" { static void zop_audit_report(void); }
extern "C" {
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
FILE *emit_outf(void) { return bb_emit_out ? bb_emit_out : stdout; }
bb_buf_t bb_emit_buf = NULL;
int bb_emit_pos = 0;
int bb_patch_count = 0;
extern "C" const char * bb_kind_name(int op) { return flat_label_kind((IR_e)op); }
extern "C" void g_emit_stno_drop_range(uint64_t from, uint64_t to) {
    int a = 0, b = g_emit.stno_map_n, c;
    while (a < b) { int mid = (a + b) / 2; if (STNO_REC(mid).pc < from) a = mid + 1; else b = mid; }
    c = a;
    b = g_emit.stno_map_n;
    while (c < b) { int mid = (c + b) / 2; if (STNO_REC(mid).pc < to) c = mid + 1; else b = mid; }
    if (c > a) memmove(&STNO_REC(a), &STNO_REC(c), (size_t)(g_emit.stno_map_n - c) * sizeof(sno_stno_rec_t));
    g_emit.stno_map_n -= c - a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_limit_init(int limit_slot_off) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_emit_limit_init"); abort(); }
int g_last_flat_frame_bytes = 0;
int g_last_flat_zstatic = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_slot_register(IR_t *nd, int off) {
    bb_slotmap_push(nd, off);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int g_proc_direct_active = 0;
int g_gva_active = 0;
int g_frame_active = 0;
int g_flat_outer_nparams = 0;
int g_gen_proc_active = 0;
int g_resumable_callable_active = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * bb_child0(const IR_t *n) { return (n && n->n_operands > 0) ? n->operands[0] : NULL; }
IR_t * bb_child1(const IR_t *n) { return (n && n->n_operands > 1) ? n->operands[1] : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gz_emit_catch(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv) {
    fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gz_emit_catch"); abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_kind_is_driver_owned(int t) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_kind_is_driver_owned"); abort(); }
static int rk_land_ngroups(const IR_t * nd) { const char * s = nd ? IR_LIT(nd).sval : (const char *)0; return s ? atoi(s) : 0; }
static int64_t rk_blob_bounds(const char * blob) {
    long mn = 0, mx = 0, fl = 0;
    if (!blob || !blob[0] || sscanf(blob + 1, "%ld,%ld,%ld;", &mn, &mx, &fl) != 3) return 0;
    return (int64_t)(mn & 0xFFFF) | ((int64_t)(mx & 0xFFFF) << 16) | ((int64_t)fl << 32);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_hclr_fill(int ri) {
    const zeta_plan_t * r = g_planner.g_zd_rec_p;
    g_emit.op_hclr_n = 0; g_emit.op_hclr_v.len = 0;
    if (g_emit.op_zgpop <= 0 || ri < 0 || !r || ri >= r->n || !r->xb || r->xb[ri] == INT_MIN || !g_emit_cfg || !g_emit_cfg->cell_frame) return;
    const int hi = r->xb[ri] + r->out[ri], lo = hi - g_emit.op_zgpop;
    for (int i = 0; i < r->n; i++) {
        IR_t * m = r->nodes[i];
        if (!m || !r->on[i] || (m->op != IR_PROC_GEN && m->op != IR_CALL_PROC_STAGED && m->op != IR_CALL_VALUE) || r->zn[i].actoff < 0 || r->xb[i] == INT_MIN) continue;
        if (r->xb[i] + r->out[i] - zd_k(m) >= lo && r->xb[i] + r->out[i] <= hi) CV_PUSH(g_emit.op_hclr_v, int) = r->zn[i].actoff + 8;
    }
    g_emit.op_hclr = (const int *)g_emit.op_hclr_v.p; g_emit.op_hclr_n = (int)g_emit.op_hclr_v.len;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_prepare(IR_t *nd) {
    g_emit.bb_ls = NULL;
    g_emit.bb_rs = NULL;
    g_emit.bb_op_lbl = NULL;
    g_emit.bb_lk = -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_classify_node(IR_t *nd) {
    g_emit.op_frame_need = (zrow(nd)->aclass == ACT_CLASS_CAPTURE && zrow(nd)->aslot != -1) ? 1 : 0;
    { if (g_emitter.bb_classify_node__ed < 0) { const char * e = getenv("SCRIP_EARN_DIAG"); g_emitter.bb_classify_node__ed = (e && *e == '1') ? 1 : 0; }
      if (g_emitter.bb_classify_node__ed && nd)
          fprintf(stderr, "[EARN] op=%d need=%d haz=%d pat_static=%d n_op=%d\n", nd->op, g_emit.op_frame_need, zrow(nd)->acause, nd->pat_static, nd->n_operands); }
    { if (g_emitter.bb_classify_node__cd < 0) { const char * e = getenv("SCRIP_CLASS_DIAG"); g_emitter.bb_classify_node__cd = (e && *e == '1') ? 1 : 0; }
      if (g_emitter.bb_classify_node__cd && nd) { extern const char * bb_op_name(IR_e);
        if (emit_rec_pin() || g_emit.op_stmt_dyn) fprintf(stderr, "[CLS:NOFIX] op=%s(%d) pin=%d dyn=%d\n", bb_op_name(nd->op), (int)nd->op, emit_rec_pin(), g_emit.op_stmt_dyn);
        if (g_emit.op_frame_need && nd->op != IR_MATCH_FENCE0) fprintf(stderr, "[CLS:NOWHACK] op=%s(%d)\n", bb_op_name(nd->op), (int)nd->op); } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bb_arith_dyn_kind(IR_t *o) { return o && (o->op == IR_CALL || ir_is_call_kind(o->op)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bb_arith_materializable(IR_t *o) {
    return o && (o->op == IR_CALL || ir_is_call_kind(o->op) || o->op == IR_LIT_INTEGER || (o->op == IR_VAR && IR_LIT(o).sval));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_arith_is_dynamic(IR_t *nd) {
    if (!nd || nd->op != IR_BINOP) return 0;
    int64_t o = IR_LIT(nd).ival;
    if (!(o == BINOP_ADD || o == BINOP_SUB || o == BINOP_MUL || o == BINOP_DIV || o == BINOP_MOD)) return 0;
    IR_t *a = bb_child0(nd), *b = bb_child1(nd);
    int adyn = bb_arith_dyn_kind(a), bdyn = bb_arith_dyn_kind(b);
    if (!adyn && !bdyn) return 0;
    if (!bb_arith_materializable(a) || !bb_arith_materializable(b)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int arith_emits_descr(IR_t *o) {
    if (!o || o->op != IR_BINOP || !bb_arith_is_dynamic(o)) return 0;
    IR_t *a = bb_child0(o), *b = bb_child1(o);
    int a_ok = a && (a->op == IR_CALL || ir_is_call_kind(a->op) || arith_emits_descr(a));
    int b_ok = b && (b->op == IR_CALL || ir_is_call_kind(b->op) || arith_emits_descr(b));
    return a_ok && b_ok;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int binop_slot_kind(IR_t *nd) {
    int64_t op = nd ? IR_LIT(nd).ival : -1;
    if ((op >= BINOP_LT && op <= BINOP_NE) || (op >= BINOP_SLT && op <= BINOP_SNE) || op == BINOP_EQV || op == BINOP_NEQV) return BINOP_CAT_RELOP;
    if (binop_is_concat((long)op)) return BINOP_CAT_CONCAT;
    if (op == BINOP_XREP) return BINOP_CAT_XREP;
    if (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD || op == BINOP_POW) return BINOP_CAT_ARITH;
    if (op == BINOP_CUNION || op == BINOP_CDIFF || op == BINOP_CINTER) return BINOP_CAT_ARITH;
    return BINOP_CAT_ARITH;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xf_diag_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_XF_DIAG"); v = (e && *e == '1') ? 1 : 0; } return v; }
static uint64_t bnr_hash(const void *k, int by_name) { uint64_t h = 1469598103934665603ull; if (!by_name) return ((uint64_t)(uintptr_t)k >> 4) * 11400714819323198485ull;
    for (const unsigned char *s = (const unsigned char *)k; *s; s++) h = (h ^ *s) * 1099511628211ull; return h; }
static int bnr_same(const void *a, const void *b, int by_name) { return a == b || (by_name && strcmp((const char *)a, (const char *)b) == 0); }
static void bnr_reset(void) { g_emitter.g_bnr_gen++; g_emitter.g_bnr_nn = 0; g_emitter.g_bnr_nm = 0; }
static void bnr_grow(cv_t *v, int by_name) {
    uint32_t oc = v->len, nc = oc ? oc * 2 : 64; cv_t o = *v; cv_t n = { 0, 0, 0, 0 };
    n.p = ct_zalloc(nc, sizeof(bnr_slot_t)); n.len = nc; n.cap = nc; n.esz = (uint32_t)sizeof(bnr_slot_t);
    for (uint32_t i = 0; i < oc; i++) { bnr_slot_t s = CV_AT(o, bnr_slot_t, i); if (s.gen != g_emitter.g_bnr_gen || !s.k) continue;
        uint64_t h = bnr_hash(s.k, by_name) & (nc - 1); while (CV_AT(n, bnr_slot_t, h).gen == g_emitter.g_bnr_gen && CV_AT(n, bnr_slot_t, h).k) h = (h + 1) & (nc - 1); CV_AT(n, bnr_slot_t, h) = s; }
    *v = n;
}
static int bnr_seen(cv_t *v, uint32_t *cnt, const void *k, int by_name) {
    if ((uint64_t)(*cnt + 1) * 2 > v->len) bnr_grow(v, by_name);
    uint64_t h = bnr_hash(k, by_name) & (v->len - 1);
    for (;;) { bnr_slot_t *s = &CV_AT(*v, bnr_slot_t, h);
        if (s->gen != g_emitter.g_bnr_gen || !s->k) { s->k = k; s->gen = g_emitter.g_bnr_gen; (*cnt)++; return 0; }
        if (bnr_same(s->k, k, by_name)) return 1;
        h = (h + 1) & (v->len - 1); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bnr_node_seen(IR_t *o) { return bnr_seen(&g_emitter.g_bnr_vnode, &g_emitter.g_bnr_nn, o, 0); }
static int bnr_name_seen(const char *n) { return bnr_seen(&g_emitter.g_bnr_vname, &g_emitter.g_bnr_nm, n, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int var_assigned_real_static(IR_graph_t *g, const char *name, int depth) {
    if (!g || !g->all || !name || depth > 8) return 0;
    if (bnr_name_seen(name)) return 0;
    for (int i = 0; i < g->n; i++) {
        IR_t *m = g->all[i];
        if (!m || m->op != IR_ASSIGN || !IR_LIT(m).sval || strcmp(IR_LIT(m).sval, name)) continue;
        IR_t *rhs = (m->n_operands > 0) ? m->operands[0] : bb_child0(m);
        if (rhs && binop_operand_real_static(g, rhs, depth + 1)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int binop_operand_real_static(IR_graph_t *g, IR_t *o, int depth) {
    if (!o || depth > 8) return 0;
    if (bnr_node_seen(o)) return 0;
    if (o->op == IR_LIT_REAL) return 1;
    if (o->op == IR_VAR && IR_LIT(o).sval) return var_assigned_real_static(g, IR_LIT(o).sval, depth);
    if (o->op == IR_BINOP) {
        IR_t *c0 = bb_child0(o), *c1 = bb_child1(o);
        return binop_operand_real_static(g, c0, depth + 1) || binop_operand_real_static(g, c1, depth + 1);
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int binop_is_num_real(IR_graph_t *g, IR_t *nd) {
    if (!nd) return 0;
    int64_t op = IR_LIT(nd).ival;
    if (op == BINOP_POW || op == BINOP_POW_PROMOTE) return 1;
    int is_num = (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD || (op >= BINOP_LT && op <= BINOP_NE));
    if (!is_num) return 0;
    bnr_reset();
    int r = binop_operand_real_static(g, bb_child0(nd), 0); if (!r) { bnr_reset(); r = binop_operand_real_static(g, bb_child1(nd), 0); }
    return r;
}
static void call_fc_fill(IR_t * nd) {
    extern int fc_call_active(const IR_t *); extern long fc_vwpop(const IR_t *); extern int zls_off(const IR_t *);
    g_emit.op_seal = nd->seal;
    if (zrow(nd)->fcact) { g_emit.op_fc_wbytes = 16; g_emit.op_fc_base = zrow(nd)->owno; long _w = zrow(nd)->vw; if (_w > 0) g_emit.op_wpop += (int)_w; }
}
static void pl_block_prepare(IR_t * nd) {
    int n = (int)g_emit.op_ival, off = zrow(nd)->loff, nk = 0, nh = 0;
    g_emit.op_pb_off = off; g_emit.op_pb_act = g_emit.op_actoff >= 0 ? g_emit.op_actoff : off + 16 * (1 + n); g_emit.op_pb_idx = g_emit.op_sval ? rt_proc_index_of(g_emit.op_sval) : -1;
    g_emit.op_pb_zsp = (g_emit.op_zres && !g_emit.op_tret) ? 1 : 0; g_emit.op_pb_fr = (!g_emit.op_pb_zsp && !g_emit.op_sprec) ? 1 : 0;
    g_emit.op_pb_frs = (g_emit.op_pb_fr && g_emit.op_zres && g_emit.op_xf_off == -1 && g_emit.op_res_alias == off) ? 1 : 0;
    g_emit.op_pb_lco = ((g_emit.op_seal == 1 || g_emit.op_seal == IR_SEAL_TAIL_POP) && !(g_emit_cfg && (g_emit_cfg->root_graph || g_emit_cfg->depth_counted))) ? 1 : 0;
    g_emit.op_pb_pop = g_emit.op_seal == IR_SEAL_TAIL_POP ? 1 : 0; g_emit.op_pb_ns = g_emit_cfg ? g_emit_cfg->nparams : 0; g_emit.op_pb_static = ((nd->strict & (1 << 30)) && g_emit.op_sval) ? 1 : 0;
    for (int i = 0; i < n; i++) {
        IR_t * a = ir_call_arg(nd, i); int named = (a && a->op == IR_VAR_REF && a->sval) ? 1 : 0, vs = named ? g_emit.op_opvs[i] : -1, ok = (vs >= 0 && !x86_key_unbacked(vs)) ? 1 : 0, seen = 0;
        g_emit.op_pb_slot[i] = bcps_arg_slot(nd, i); g_emit.op_pb_vs[i] = named ? ((vs >= 0 && x86_key_unbacked(vs)) ? -1 : vs) : -1;
        g_emit.op_pb_kv[i] = (ok && a->seal == IR_SEAL_KILL_VALUE) ? vs : -1; g_emit.op_pb_kh[i] = (ok && a->seal == IR_SEAL_KILL_HOME) ? vs : -1; g_emit.op_pb_kvn[i] = 0; g_emit.op_pb_khn[i] = 0;
        if (g_emit.op_pb_kv[i] >= 0) { for (int k = 0; k < i; k++) if (g_emit.op_pb_kv[k] == vs) seen = 1; if (!seen) g_emit.op_pb_kvn[i] = ++nk; }
        seen = 0;
        if (g_emit.op_pb_kh[i] >= 0) { for (int k = 0; k < i; k++) if (g_emit.op_pb_kh[k] == vs) seen = 1; if (!seen) g_emit.op_pb_khn[i] = ++nh; }
    }
    g_emit.op_pb_kvt = n <= 30 ? nk : 0; g_emit.op_pb_kht = n <= 30 ? nh : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "walk_bb_flat"); abort(); }
static int zw_nid_listed(const char * e, int nid) { if (!e || !*e) return 0; const char * p = e; while (*p) { long v = strtol(p, (char **)&p, 10); if ((int)v == nid) return 1;
    while (*p && *p != ',') p++; if (*p) p++; } return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_dwarf_loc_on(void) { const char * e = getenv("SCRIP_DWARF_LOC"); return (e && e[0] == '0') ? 0 : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void lit_prepare(IR_t * nd, const znode_t * zn) { if (!g_emit.op_zres && zn->fcg > 0) { g_emit.op_fc_bytes = zn->fcg; g_emit.op_fc_base = g_emit.op_off; }
    g_emit.op_lit_stamp = sn4_descr_stamp() ? (long)(((g_emit.nid <= 0 || g_emit.nid > (long)DESCR_SRC_NODE_OVERFLOW - 1) ? (long)DESCR_SRC_NODE_OVERFLOW : (long)g_emit.nid) << 8) : 0L; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int walk_bb_node_inner(IR_t * nd, FILE * out) {
    const int _ri = zrow_of(nd);
    if (_ri < 0) { fprintf(stderr, "FATAL emitter: node %s has no plan row (ARCH-THREE-ZETAS-EMISSION 6.3: the row is the emitter's one input)\n", nd ? bb_op_name(nd->op) : "NONE"); abort(); }
    const zeta_plan_t * _zr = g_planner.g_zd_rec_p; const znode_t * _zn = &_zr->zn[_ri]; const zrd_t * _rdp = _zr->rd + _zr->rdo[_ri]; const int _rdn = _zr->rdo[_ri + 1] - _zr->rdo[_ri];
    g_emit.op_zrow = _ri; g_emit.op_res_live = _zn->rlive;
    if (_zn->unr) { x86_begin();
        bb_emit_x86(x86_deflabel(X86P_ALPHA) + x86_bomb("unreached box: the plan found no alpha landing on its run (UNREACHED)") + x86_deflabel(X86P_BETA) + x86_bomb("unreached box entered at beta"));
        return 0; }
    { if (g_emitter.walk_bb_node_inner__ba < 0) { const char * e = getenv("SCRIP_BB_ALLOC"); g_emitter.walk_bb_node_inner__ba = (e && *e == '0') ? 0 : 1;
        g_emitter.walk_bb_node_inner__bo = getenv("SCRIP_BB_ONLY");
        g_emitter.walk_bb_node_inner__bs = getenv("SCRIP_BB_SKIP"); }
      int _spine =
          (nd->op == IR_BINOP || nd->op == IR_ASSIGN || nd->op == IR_LIT_INTEGER || ir_is_lit_text_kind(nd->op) || nd->op == IR_LIT_ATOM || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET ||
          nd->op == IR_LIT_NAME || nd->op == IR_VAR || nd->op == IR_CMP_TEST || nd->op == IR_COERCE_NUMERIC || nd->op == IR_IDENT || nd->op == IR_DIFFER);
      { if (g_emitter.walk_bb_node_inner__all < 0) { const char * e = getenv("SCRIP_BB_ALLOC_ALL"); g_emitter.walk_bb_node_inner__all = (e && *e == '1') ? 1 : 0; }
      (void)g_emitter.walk_bb_node_inner__ba; (void)g_emitter.walk_bb_node_inner__bo; (void)g_emitter.walk_bb_node_inner__bs; (void)g_emitter.walk_bb_node_inner__all; (void)_spine;
      if (_zn->carve > 0) g_emit.op_fc_bytes = _zn->carve; } }
    g_emit.op_zdepth = x86_fc_on() ? (int)g_emit.op_fc_bytes : 0; emit_gc_rsp_reset();
    { g_emit.op_zdp_ad = _zn->zdpa; g_emit.op_zdp_bd = _zn->zdpb;
      g_emit.op_zdp_rbp = (nd && (nd->op == IR_MATCH_BEGIN || nd->op == IR_MATCH_DEFER)) ? 1 : 0; }
    g_emit.op_pair_rejoin = (nd && nd->op == IR_MATCH_ALTERNATE) ? 1 : 0;
    g_emit.op_stmt_dyn = g_emitter.g_arbk16_stmt; if (nd) { if (nd->op == IR_STATEMENT_BEGIN) { g_emitter.g_arbk16_stmt = 0; g_emit.op_stmt_dyn = 0;
        } else if (nd->op == IR_STATEMENT_END || nd->op == IR_STATEMENT) g_emitter.g_arbk16_stmt = 0; }
    g_emit.op_zcp_src = -1; g_emit.op_zcp_dst = -1; g_emit.op_xrec = 0;
    g_emit.op_zres = 0; g_emit.op_zrel = 0; g_emit.op_zgpop = 0; g_emit.op_zwhack = 0; g_emit.op_zbgrow = 0; g_emit.op_wsteal = 0; g_emit.op_ztail = 0; g_emit.op_zpat = 0; g_emit.op_zfc = 0;
        g_emit.op_xf_off = -1; g_emit.op_zdef = 1;
        g_emit.op_zcut = -1;
        emit_ops_reserve(nd ? nd->n_operands + 1 : 0);
        for (int _zi = 0; _zi < g_emit.op_zcap; _zi++) { g_emit.op_zread[_zi] = 0; g_emit.op_zkind[_zi] = -1; g_emit.op_zread_xf[_zi] = -1; g_emit.op_zread_fr[_zi] = -1; g_emit.op_zslot[_zi] = -1;
            g_emit.op_zkown[_zi] = -1; g_emit.op_zfres[_zi] = 1; g_emit.op_opvs[_zi] = -1; }
    g_emit.op_zslot_n = 0; g_emit.op_res_alias = -1;
    g_emit.op_own = _zn->owno; g_emit.op_kown = _zn->kown; g_emit.op_fres = _zn->fres; g_emit.op_xk = _zn->xk; g_emit.op_xw = _zn->xw; g_emit.op_xd = _zn->xd; g_emit.op_xa = _zn->xa;
    g_emit.op_xf_off = _zr->home[_ri]; g_emit.op_cft = _zn->cft; g_emit.op_zslid = _zr->slid ? _zr->slid[_ri] : 0;
    g_emit.op_gcell = _zn->gcell; g_emit.op_bspk = _zn->bspk; g_emit.op_bxd = _zn->bxd; g_emit.op_sprec = _zn->sprec; g_emit.op_actoff = _zn->actoff; g_emit.op_tret = _zn->tret;
    for (int _zi = 0; _zi < nd->n_operands && _zi < g_emit.op_zcap; _zi++) g_emit.op_opvs[_zi] = zrow_opvs(nd, _zi);
    for (int _zi = 0; _zi <= nd->n_operands && _zi < g_emit.op_zcap; _zi++) { const zrd_t * _rd = _zi < _rdn ? &_rdp[_zi] : (const zrd_t *)0; if (_rd) { g_emit.op_zread_xf[_zi] = _rd->xf;
        g_emit.op_zkown[_zi] = _rd->kown; g_emit.op_zfres[_zi] = _rd->fres; } }
    g_emit.op_suspend_stmt_uclaim = 0;
    if (g_emitter.g_zd_stage) { g_emit.op_suspend_stmt_uclaim = g_emitter.g_zd_suspend_uclaim;
        g_emitter.g_zd_suspend_uclaim = 0;
        if (_zr->grec_off[_ri] != -1) { g_emit.op_xrec |= 4; g_emit.op_grec = _zr->grec_off[_ri]; g_emit.op_grec_pop = _zr->grec_res[_ri]; }
        if (_zr->wrec_off[_ri] != -1) { g_emit.op_xrec |= 8; g_emit.op_wrec = _zr->wrec_off[_ri]; g_emit.op_wrec_pop = _zr->wrec_res[_ri]; }
        if (g_emitter.g_zd_cpread && _zr->cpa_dx[_ri] != -1) { g_emit.op_xrec |= 2; g_emit.op_zcp_dxf = _zr->cpa_dx[_ri]; }
        if (g_emitter.g_zd_cpread && _zr->cpa_sx[_ri] != -1) { g_emit.op_xrec |= 1; g_emit.op_zcp_sxf = _zr->cpa_sx[_ri]; }
        if (g_emitter.g_zd_arm) { g_emit.op_zcp_src = _zr->cpa_s[_ri]; g_emit.op_zcp_dst = _zr->cpa_d[_ri];
            g_emit.op_zres = 1; g_emit.op_fc_bytes = g_emitter.g_zd_k; g_emit.op_fc_base = -1; g_emit.op_zdepth = g_emitter.g_zd_k + _zr->under[_ri];
            if (g_emitter.g_zd_rdread) for (int _zi = 0; _zi <= nd->n_operands && _zi < g_emit.op_zcap; _zi++) { const zrd_t * _rd = _zi < _rdn ? &_rdp[_zi] : (const zrd_t *)0; if (!_rd) continue;
                g_emit.op_zread[_zi] = _rd->dist; g_emit.op_zkind[_zi] = _rd->kind; g_emit.op_zread_fr[_zi] = _rd->fr; }
            g_emit.op_ztail = _zr->tail[_ri]; g_emit.op_zpat = _zr->pat[_ri]; g_emit.op_zfc = _zr->fc[_ri];
            if (g_emit_cfg) { int _xn = _zn->nops; g_emit.op_zslot_n = (_xn < nd->n_operands) ? _xn : nd->n_operands;
                for (int _zi = 0; _zi < g_emit.op_zslot_n && _zi < g_emit.op_zcap; _zi++) { const zrd_t * _rd = _zi < _rdn ? &_rdp[_zi] : (const zrd_t *)0; g_emit.op_zslot[_zi] = _rd ? _rd->key : -1;
                    }
                if (_zn->ssub && g_emit.op_zslot_n == nd->n_operands && g_emit.op_zslot_n < g_emit.op_zcap) { int _zi = g_emit.op_zslot_n++;
                    const zrd_t * _rd = _zi < _rdn ? &_rdp[_zi] : (const zrd_t *)0;
                    g_emit.op_zslot[_zi] = _rd ? _rd->key : -1; }
                g_emit.op_res_alias = _zr->alias[_ri];
                if (xf_diag_on()) { fprintf(stderr, "[ALIAS] %s res=%d nops=%d:", bb_op_name(nd->op), g_emit.op_res_alias, g_emit.op_zslot_n);
                    for (int _zi = 0; _zi < g_emit.op_zslot_n && _zi < g_emit.op_zcap; _zi++) { IR_t * _po = nd->operands[_zi];
                        fprintf(stderr, " [%d %s slot=%d zread=%d fr=%d xf=%d]", _zi, _po ? bb_op_name(_po->op) : "-", g_emit.op_zslot[_zi],
                            g_emit.op_zread[_zi], g_emit.op_zread_fr[_zi], g_emit.op_zread_xf[_zi]); } fprintf(stderr, "\n"); } } }
        g_emit.op_zgpop = g_emitter.g_zd_fence_exit ? 0 : (g_emit.flat_jmp_entry && g_emit.flat_pat) ? (g_emitter.g_zd_arm ? _zn->rel : 0) : g_emitter.g_zd_gpop; g_emit.op_wpop += g_emitter.g_zd_wpop;
            g_emitter.g_zd_fence_exit = 0;
        g_emit.op_zbgrow = g_emitter.g_zd_bgrow ? g_emit.op_zgpop : 0; g_emitter.g_zd_bgrow = 0;
        if (g_planner.zd_plan__dg)
            fprintf(stderr, "[ZD-STAGE] op=%d gpop=%d wpop=%d abs=%d k=%d fc=%ld\n", (int)nd->op, g_emit.op_zgpop, g_emit.op_wpop, g_emitter.g_zd_abs, g_emitter.g_zd_k, (long)g_emit.op_fc_bytes);
            g_emit.op_wsteal = g_emitter.g_zd_wsteal; g_emitter.g_zd_stage = 0; }
    emit_hclr_fill(_ri);
    extern void bb_prepare_capture_arbno(IR_t *nd, int imm);
    extern void bb_prepare(IR_t *nd);
    extern int bb_slot_get(IR_t *nd);
    if (!nd) return 1;
    g_emit.node = nd; g_emit.op_pat_static = nd->pat_static;
    g_emit.op_zf_unpinned = 1;
    g_emit.op_zabs = g_emitter.g_zd_abs; g_emit.op_zsticky = _zn->sticky; g_emit.op_zk_own = g_emit.op_zres ? g_emitter.g_zd_k - g_emit.op_zsticky : 0; g_emit.op_zdef = g_emitter.g_zd_def;
        g_emit.op_zcut = g_emitter.g_zd_cutd;
    if (g_emit.op_zres && !zrow_under(g_emit.op_zrow)) g_emit.op_zdepth = g_emitter.g_zd_abs;
    if (g_emit.op_zres) g_emit.op_zrel = _zn->rel;
    emit_set_sink(out);
    g_emit.sid = 0;
    g_emit.nid = bb_node_id(nd);
    g_emit.x86_uid = g_flat_node_id++;
    { const char * _k = flat_label_kind_of(nd); if (!_k) _k = "(null)"; size_t _n = strlen(_k); cv_reserve(&g_emit.x86_uid_kind_buf, 1, (uint64_t)_n + 1, "x86_uid_kind_buf");
        memcpy(g_emit.x86_uid_kind_buf.p, _k, _n + 1); }
    g_emit.x86_uid_kind = (const char *)g_emit.x86_uid_kind_buf.p;
    g_emit.frame_region = g_emit_cfg ? ((32 + g_emit_cfg->jcon_value_region + 15) & ~15) : 0;
    g_emit.op_strict = nd->strict;
    g_emit.op_sval =
        (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_VAR_FRAME || nd->op == IR_ASSIGN_FRAME || nd->op == IR_ASSIGN || ir_is_lit_text_kind(nd->op) || nd->op == IR_LIT_ATOM ||
        nd->op == IR_LIT_CHARSET || nd->op == IR_LIT_NAME
                       || nd->op == IR_KW_ICON || nd->op == IR_KW_ICON_GEN || nd->op == IR_KW_SNOBOL4 || nd->op == IR_KW_ASSIGN || nd->op == IR_KW_ASSIGN_SNOBOL4 || nd->op == IR_REV_SWAP ||
                           nd->op == IR_FIELD_GET || nd->op == IR_FIELD_VAR || nd->op == IR_SUBSCRIPT || nd->op == IR_ASSIGN_VAR || nd->op == IR_ITERATE
                       || nd->op == IR_MATCH_ASSIGN_COND || nd->op == IR_MATCH_ASSIGN_SAVE || nd->op == IR_MATCH_ASSIGN_IMM || nd->op == IR_MATCH_LIT || nd->op == IR_MATCH_ANY ||
                           nd->op == IR_MATCH_NOTANY || nd->op == IR_MATCH_SPAN
                       || nd->op == IR_MATCH_BREAK || nd->op == IR_MATCH_BREAKX || nd->op == IR_MATCH_DEFER || nd->op == IR_MATCH_ATP || nd->op == IR_MATCH_REPLACE || nd->op == IR_GOTO_DEFERRED
                       || nd->op == IR_NULLTEST_VAR || nd->op == IR_PROC_GEN || nd->op == IR_PROC_VALUE || ir_norm_call_kind(nd->op) == IR_CALL)
                    ? IR_LIT(nd).sval : (const char *)0;
    { extern int g_gva_active; extern int gva_index_of(const char *);
        int nm_op = (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_ASSIGN || nd->op == IR_MATCH_ASSIGN_IMM || nd->op == IR_MATCH_ASSIGN_COND);
      int defer_op = ((nd->op == IR_MATCH_DEFER) && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '*');
      g_emit.op_gva_k = (g_gva_active && (nm_op || defer_op) && IR_LIT(nd).sval && !graph_has_local(g_emit_cfg, IR_LIT(nd).sval)) ? gva_index_of(IR_LIT(nd).sval) : -1; }
    { extern int g_proc_direct_active; extern int proc_slot_of(const char *); extern int proc_direct_eligible(const char *); int nm_op = (ir_norm_call_kind(nd->op) == IR_CALL);
      g_emit.op_proc_k = (g_proc_direct_active && nm_op && IR_LIT(nd).sval && proc_direct_eligible(IR_LIT(nd).sval)) ? proc_slot_of(IR_LIT(nd).sval) : -1; }
    if (nd->op == IR_STATEMENT_BEGIN || nd->op == IR_STATEMENT_END || nd->op == IR_STATEMENT || (nd->op == IR_STMT_MARK && nd->seal != IR_SEAL_STMT_HOOK && nd->seal != IR_SEAL_LABEL_HOOK))
        { int32_t _stno = (int32_t)IR_LIT(nd).ival;
        if (_stno != g_emit.stno_last) { emit_stno_mark(_stno); g_emit.stno_last = _stno; } g_emit.op_stno = _stno; }
    g_emit.op_ival = (ir_norm_call_kind(nd->op) == IR_CALL || nd->op == IR_PROC_GEN) ? (int64_t)nd->n_operands : (nd->op == IR_GOTO_DEFERRED ? (int64_t)nd->seal : IR_LIT(nd).ival);
    if (ir_is_define_kind(nd->op) && ir_define_sr_citizen(nd)) { int64_t r = (int64_t)_zn->srole; g_emit.op_ival = r; g_emit.op_sval = r ? (const char *)0 : IR_LIT(nd).sval; }
    g_emit.op_node_kind = (int)nd->op;
    g_emit.op_dval = IR_LIT(nd).dval;
    IR_t *op_a = (nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
    g_emit.op_a_sval = op_a ? IR_LIT(op_a).sval : (const char *)0;
    g_emit.op_a_node_kind = op_a ? (int)ir_norm_call_kind(op_a->op) : -1;
    g_emit.op_a_slot = (op_a != (IR_t *)0) ? bb_slot_get(op_a) : -1;
    if (nd->op == IR_MATCH_VALUE && op_a) { int _z = _zn->o0off; if (_z >= 0) g_emit.op_a_slot = _z; }
    if (g_emit.op_a_slot < 0 && op_a) { int _z = _zn->o0off; if (_z >= 0) g_emit.op_a_slot = _z; }
    g_emit.op_a_counter = 0;
    g_emit.op_a_ival_sg = op_a ? IR_LIT(op_a).ival : 0;
    g_emit.op_a_dval = op_a ? IR_LIT(op_a).dval : 0;
    { extern int arith_emits_descr(IR_t *nd); g_emit.op_a_descr = arith_emits_descr(op_a) ? 1 : 0; }
    { if (g_emitter.walk_bb_node_inner_bm < 0) { const char *e = getenv("SCRIP_BLOB_MAP"); g_emitter.walk_bb_node_inner_bm = (e && *e == '1') ? 1 : 0;
        } if (g_emitter.walk_bb_node_inner_bm && MEDIUM_BINARY) { extern const char *bb_op_name(IR_e);
        fprintf(stderr, "BLOBBOX %p %s n%d\n", (void *)(bb_emit_buf + bb_emit_pos), bb_op_name(nd->op), g_emit.x86_uid); } }
    { if (g_emitter.walk_bb_node_inner__sc < 0) { const char *e = getenv("SCRIP_SRC_COMMENT"); g_emitter.walk_bb_node_inner__sc = (e && *e == '0') ? 0 : 1;
        } g_emit.op_src = g_emitter.walk_bb_node_inner__sc ? bb_src_of(nd) : (const char *)0; }
    { if (g_emit.op_src && *g_emit.op_src) emit_sep_rule('=');
      if (g_emit.op_src) { std::string _c; const char * p = g_emit.op_src; while (*p) { const char * e2 = p; while (*e2 && *e2 != '\n') e2++; _c += x86("srccomment", std::string(p, (size_t)(e2 - p)));
          p = *e2 ? e2 + 1 : e2; } bb_emit_x86(_c); }
      emit_sep_rule('-'); }
    { if (g_emitter.walk_bb_node_inner__dl < 0) g_emitter.walk_bb_node_inner__dl = emit_dwarf_loc_on();
      int _raw = bb_line_of(nd); int _ln = _raw < 0 ? -_raw : _raw;
      if (_ln > 0) g_emit.op_line = _ln;
      if (g_emitter.walk_bb_node_inner__dl && _ln > 0) { const char * _sf = stmt_src_get_file(); int _nl = stmt_src_nlines();
        if (_sf && *_sf) bb_emit_x86(x86("loc", _sf, _ln, (_raw > 0 && _nl > 0 && _ln <= _nl) ? 1 : 2)); } }
    bb_classify_node(nd);
    if (ir_floater_kind(nd) == 3) { bb_emit_x86(bb_nreturn_mark()); return 0; }
    switch (nd->op) {
    case IR_LIT_INTEGER: lit_prepare(nd, _zn); bb_emit_x86(bb_lit_integer()); return 0;
    case IR_LIT_STRING: lit_prepare(nd, _zn); bb_emit_x86(bb_lit_string()); return 0;
    case IR_LIT_DSTAR: lit_prepare(nd, _zn); bb_emit_x86(bb_lit_dstar()); return 0;
    case IR_LIT_THUNK: lit_prepare(nd, _zn); bb_emit_x86(bb_lit_thunk()); return 0;
    case IR_LIT_ATOM: lit_prepare(nd, _zn); g_emit.op_ival = (int64_t)(unsigned)prolog_atom_intern_lit(g_emit.op_sval ? g_emit.op_sval : ""); bb_emit_x86(bb_lit_atom()); return 0;
    case IR_LIT_CHARSET: lit_prepare(nd, _zn); bb_emit_x86(bb_lit_charset()); return 0;
    case IR_LIT_NAME: lit_prepare(nd, _zn); bb_emit_x86(bb_lit_name()); return 0;
    case IR_LIT_REAL: lit_prepare(nd, _zn); bb_emit_x86(bb_lit_real()); return 0;
    case IR_KW_ICON:
    case IR_KW_ICON_GEN: g_emit.op_var_form = nd->pat_static; g_emit.op_name2 = kw_bare(g_emit.op_sval); bb_emit_x86(bb_keyword_icon()); g_emit.op_var_form = 0; return 0;
    case IR_KW_SNOBOL4: kw_snobol4_prepare(); bb_emit_x86(bb_keyword_snobol4()); return 0;
    case IR_KW_ASSIGN: g_emit.op_name2 = kw_bare(g_emit.op_sval); bb_emit_x86(bb_keyword_assign()); return 0;
    case IR_KW_ASSIGN_SNOBOL4: g_emit.op_imm_a = g_emit.op_sval ? rt_kw_index(g_emit.op_sval) : -1; bb_emit_x86(bb_keyword_assign_snobol4()); return 0;
    case IR_VAR: { extern int is_global(const char *);
        const char * _vn = IR_LIT(nd).sval;
        if (_vn && _vn[0] == '&') { g_emit.op_name2 = kw_bare(g_emit.op_sval); bb_emit_x86(bb_keyword_icon()); }
        else if (_vn && (is_global(_vn) && !graph_has_local(g_emit_cfg, _vn))) { { if (!g_emit.op_zres && _zn->fcg > 0) { g_emit.op_fc_bytes = _zn->fcg; g_emit.op_fc_base = g_emit.op_off;
            } } { if (_zn->vw > 0 && !g_emit.op_zres) g_emit.op_wpop += _zn->vw; } bb_emit_x86(bb_var_global()); }
        else bb_emit_x86(bb_var()); } return 0;
    case IR_VAR_FRAME: { extern void display_copy(IR_t *); display_copy(nd); } bb_emit_x86(bb_var_frame()); return 0;
    case IR_ASSIGN_FRAME: { IR_t *_fr = (nd->n_operands > 1) ? nd->operands[1] : (IR_t *)0;
        g_emit.op_a_slot = _fr ? bb_slot_get(_fr) : -1;
        if (g_emit.op_a_slot < 0 && _fr) { int _z = zrow_opoff(nd, 1); if (_z >= 0) g_emit.op_a_slot = _z; }
        { extern void display_copy(IR_t *); display_copy(nd); } bb_emit_x86(bb_assign_frame()); } return 0;
    case IR_VAR_REF: { extern int is_global(const char *); const char * _rn = IR_LIT(nd).sval; g_emit.op_var_named = nd->pat_static;
        if (nd->n_operands >= 1 && nd->operands[0] && nd->operands[0]->op == IR_LIT_NAME) { bb_prepare(nd); { extern void display_copy(IR_t *); display_copy(nd);
            } bb_emit_x86(bb_var_ref_frame()); return 0; }
        if (_rn && is_global(_rn) && !graph_has_local(g_emit_cfg, _rn)) { g_emit.op_sa = -1; g_emit.op_gva_k = g_gva_active ? gva_index_of(_rn) : -1; }
        else if (_rn) { int _vo = zrow(nd)->vso; g_emit.op_sa = _vo; g_emit.op_gva_k = -1; }
        bb_emit_x86(bb_var_ref()); } return 0;
    case IR_COERCE_STRING: { bb_prepare(nd); bb_emit_x86(bb_coerce_string()); } return 0;
    case IR_COERCE_INTEGER: { bb_prepare(nd); bb_emit_x86(bb_coerce_integer()); } return 0;
    case IR_COERCE_NUMERIC: { bb_prepare(nd);
        if (g_emit.op_zres && nd->n_operands == 1 && g_emit.op_zcap > 1) {
            g_emit.op_zread[1] = g_emit.op_zread[0];
            g_emit.op_zkind[1] = g_emit.op_zkind[0];
            g_emit.op_zread_fr[1] = g_emit.op_zread_fr[0];
            g_emit.op_zread_xf[1] = g_emit.op_zread_xf[0];
        }
        bb_emit_x86(bb_coerce_numeric()); } return 0;
    case IR_COERCE_REAL: { bb_prepare(nd); bb_emit_x86(bb_coerce_real()); } return 0;
    case IR_CMP_TEST: { bb_prepare(nd); bb_emit_x86(bb_cmp_test()); } return 0;
    case IR_IDENT: { bb_prepare(nd); bb_emit_x86(bb_ident()); } return 0;
    case IR_DIFFER: { bb_prepare(nd); bb_emit_x86(bb_differ()); } return 0;
    case IR_ASSIGN: {
        extern int is_global(const char *);
        const char * _an = IR_LIT(nd).sval;
        int _an_reassignable_builtin = _an && (!strcmp(_an, "write") || !strcmp(_an, "writes"));
        if (_an && ((is_global(_an) && !graph_has_local(g_emit_cfg, _an)) || _an_reassignable_builtin)) { bb_emit_x86(bb_assign_global()); return 0; }
        if (_an) { { if (g_emitter.walk_bb_node_inner__dd < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); g_emitter.walk_bb_node_inner__dd = (_e && _e[0] == '1') ? 1 : 0; }
                     if (g_emitter.walk_bb_node_inner__dd && !(g_emit.op_sb >= 0 && g_emit.op_off >= 0 && (g_emit.op_a_slot >= 0 || g_emit.op_a_node_kind == (int)IR_EXCISED))) {
                         IR_t *_ra = (nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
                         fprintf(stderr, "[ASSIGN-DIAG] var=%s sb=%d off=%d a_slot=%d a_kind=%d rhs_op=%s rhs_nops=%d\n", _an, g_emit.op_sb, g_emit.op_off, g_emit.op_a_slot, g_emit.op_a_node_kind,
                             _ra ? bb_op_name(_ra->op) : "NULL", _ra ? _ra->n_operands : -1); } }
                   bb_emit_x86(bb_assign_local()); return 0; }
        fprintf(out, "# [walk_bb_node: kind=%d unhandled]\n", (int)nd->op); return 1;
    }
    case IR_BINOP_TEST: bb_emit_x86(bb_binop_relop()); return 0;
    case IR_BINOP_RELOP_VAL: bb_emit_x86(bb_binop_relop_val()); return 0;
    case IR_BINOP:
        { if (_zn->vw > 0 && _zn->vbin && !g_emit.op_zres) g_emit.op_wpop += _zn->vw; }
        switch (g_emit.op_binop_kind) {
        case BINOP_CAT_RELOP: bb_emit_x86(bb_binop_relop()); return 0;
        case BINOP_CAT_CONCAT: bb_emit_x86(bb_binop_concat_slot()); return 0;
        case BINOP_CAT_XREP: bb_emit_x86(bb_binop_xrep_slot()); return 0;
        case BINOP_CAT_ARITH:
        default: bb_emit_x86(bb_binop_arith()); return 0;
        }
    case IR_SUCCEED: bb_emit_x86(bb_succeed()); return 0;
    case IR_SUSPEND: { g_emit.op_activate_proc = IR_LIT(nd).sval; g_emit.op_var_form = nd->pat_static; bb_emit_x86(bb_suspend()); g_emit.op_var_form = 0; } return 0;
    case IR_TO: { bb_prepare(nd); g_emit.op_range_int_operands = ir_range_operands_must_be_integers(nd); g_emit.op_db_walk = ir_range_kind_is(nd, "db"); g_emit.op_to_trailed = ir_range_trailed(nd);
        g_emit.op_to_db_last = ir_range_tag_has(nd, 'd', 'e', 't');
        bb_emit_x86(bb_to()); } return 0;
    case IR_MATCH_LEN: { bb_prepare(nd); { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
        g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : (const char *)0; } bb_emit_x86(bb_match_len()); } return 0;
    case IR_MATCH_LIT: { bb_prepare(nd); bb_emit_x86(bb_match_lit()); } return 0;
    case IR_MATCH_RKRUN: { bb_prepare(nd); g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_ival = rk_blob_bounds(IR_LIT(nd).sval); g_emit.op_off = _zn->aslot; bb_emit_x86(bb_match_rkrun());
        } return 0;
    case IR_MATCH_RKCAPS: { bb_prepare(nd); g_emit.op_ival = IR_LIT(nd).ival; g_emit.op_off = _zn->aslot;
        bb_emit_x86(bb_match_rkcaps()); } return 0;
    case IR_MATCH_RKCAPE: { bb_prepare(nd); g_emit.op_ival = IR_LIT(nd).ival; g_emit.op_off = _zn->aslot;
        bb_emit_x86(bb_match_rkcape()); } return 0;
    case IR_MATCH_RKLAND: { bb_prepare(nd); g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_ival = rk_land_ngroups(nd); g_emit.op_off = _zn->aslot; bb_emit_x86(bb_match_rkland()); } return 0;
    case IR_MATCH_ANY: { bb_prepare(nd); bb_emit_x86(bb_match_any()); } return 0;
    case IR_MATCH_NOTANY: { bb_prepare(nd); bb_emit_x86(bb_match_notany()); } return 0;
    case IR_MATCH_SPAN: { bb_prepare(nd); { if (_zn->leaf > 0) { g_emit.op_fc_bytes = _zn->leaf; g_emit.op_fc_base = g_emit.x86_scratch_off;
        } } bb_emit_x86(bb_match_span()); } return 0;
    case IR_MATCH_BREAK: { bb_prepare(nd); { if (_zn->leaf > 0) { g_emit.op_fc_bytes = _zn->leaf; g_emit.op_fc_base = g_emit.x86_scratch_off;
        } } bb_emit_x86(bb_match_break()); } return 0;
    case IR_MATCH_BREAKX: { bb_prepare(nd); { if (_zn->leaf > 0) { g_emit.op_fc_bytes = _zn->leaf; g_emit.op_fc_base = g_emit.x86_scratch_off;
        } } bb_emit_x86(bb_match_breakx()); } return 0;
    case IR_MATCH_TAB: { bb_prepare(nd); { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
        g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : (const char *)0; } { if (_zn->leaf > 0) { g_emit.op_fc_bytes = _zn->leaf;
        g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_tab()); } return 0;
    case IR_MATCH_RTAB: { bb_prepare(nd); { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
        g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : (const char *)0; } { if (_zn->leaf > 0) { g_emit.op_fc_bytes = _zn->leaf;
        g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_rtab()); } return 0;
    case IR_MATCH_LAMBDA: { bb_prepare(nd); bb_emit_x86(bb_match_lambda()); } return 0;
    case IR_MATCH_POS: { bb_prepare(nd); { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
        g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : (const char *)0; } bb_emit_x86(bb_match_pos()); } return 0;
    case IR_MATCH_RPOS: { bb_prepare(nd); { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
        g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : (const char *)0; } bb_emit_x86(bb_match_rpos()); } return 0;
    case IR_MATCH_REM: { bb_prepare(nd); { if (_zn->leaf > 0) { g_emit.op_fc_bytes = _zn->leaf; g_emit.op_fc_base = g_emit.x86_scratch_off;
        } } bb_emit_x86(bb_match_rem()); } return 0;
    case IR_MATCH_ATP: { bb_prepare(nd); bb_emit_x86(bb_match_atp()); } return 0;
    case IR_MATCH_CASMARK: { bb_prepare(nd); { const char * _sv = ((uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0; g_emit.op_sval = _sv;
        if (_sv) g_emit.op_ival = 0; } bb_emit_x86(bb_match_casmark()); } return 0;
    case IR_MATCH_ARB: { bb_prepare(nd); { if (_zn->leaf > 0) { g_emit.op_fc_bytes = _zn->leaf; g_emit.op_fc_base = g_emit.x86_scratch_off;
        } } bb_emit_x86(bb_match_arb()); } return 0;
    case IR_MATCH_BAL: { bb_prepare(nd); { if (_zn->leaf > 0) { g_emit.op_fc_bytes = _zn->leaf; g_emit.op_fc_base = g_emit.x86_scratch_off;
        } } bb_emit_x86(bb_match_bal()); } return 0;
    case IR_MATCH_DEFER: { bb_prepare(nd); g_emit.op_seal = nd->seal; g_emit.op_off = drive_value_slot(nd); g_emit.op_defer_leaf_susp = _zn->dsusp;
        defer_prepare();
        bb_emit_x86(bb_match_defer()); } return 0;
    case IR_MATCH_VALUE: { bb_prepare(nd); bb_emit_x86(bb_match_value()); } return 0;
    case IR_MATCH_ARBNO: { bb_prepare(nd); g_emit.op_arbno_rbp = 0; g_emit.op_arbno_frame_off = _zn->aslot; g_emit.op_arbno_win_lo = _zn->awlo; g_emit.op_arbno_win_bytes = _zn->awb;
        if (g_emitter.walk_bb_node_inner__k16e < 0) { const char * _e1 = getenv("SCRIP_ARBNO_K16"); g_emitter.walk_bb_node_inner__k16e = (_e1 ? (atoi(_e1) != 0) : 1) ? 1 : 0; }
        int _k16r = g_emitter.walk_bb_node_inner__k16e && _zn->ak16;
        if (_k16r) { g_emitter.g_arbk16_stmt = 1; g_emit.op_arbno_body_k0 = 0; g_emit.op_arbno_body_kk = _zn->apk; }
        else { g_emit.op_sa = _zn->asa; g_emit.op_sb = _zn->asb; g_emit.op_arbno_chain = _zn->ach; if (_zn->ach) g_emit.op_tail_seal = (nd->n_operands > 3 && nd->operands[3] == nd) ? 1 : 0;
            g_emit.op_arbno_body_k0 = _zn->abk0; g_emit.op_arbno_body_defer_unsafe = _zn->abdu; }
        bb_emit_x86(bb_match_arbno()); } return 0;
    case IR_MATCH_BEGIN: { int _th = _zn->hth; if (_zn->hfp >= 0 || _th) { g_emit.op_fc_wbytes = 80; g_emit.op_fc_base = g_emit.op_off;
        } if (_th) g_emit.op_tail = 1; if (g_emit.op_zres) g_emit.op_zdepth -= (int)g_emit.op_fc_bytes; g_emit.op_fc_bytes = 0; bb_emit_x86(bb_match_begin()); } return 0;
    case IR_MATCH_END: { if (getenv("SCRIP_FCDISP_DIAG")) fprintf(stderr, "[FCDISP] tail_release=%d br=%d head_fp=%d\n", _zn->hrel, _zn->hbr, _zn->hfp);
                                    if (_zn->hrel) { g_emit.op_tail = 1; g_emit.op_fc_disp = _zn->hbr; }
                                    else g_emit.op_fc_disp = _zn->hfp;
                                    if (_zn->hfp >= 0 || _zn->hth) { g_emit.op_fc_wbytes = 80; g_emit.op_fc_base = g_emit.op_off; }
                                    g_emit.op_zwhack = g_emit.op_zgpop; g_emit.op_zgpop = 0;
                                    bb_emit_x86(bb_match_end()); } return 0;
    case IR_MATCH_REPLACE: { bb_emit_x86(bb_match_replace()); } return 0;
    case IR_MATCH_ASSIGN_COND: { bb_prepare(nd); g_emit.op_fc_disp = _zn->cfp; g_emit.op_cap_frame_off = _zn->aslot;
        if (getenv("SCRIP_CAP_DIAG"))
        fprintf(stderr, "[CAP] COND nd=%p fc_disp=%d zres=%d zread0=%d\n", (void*)nd, g_emit.op_fc_disp, g_emit.op_zres, g_emit.op_zread[0]);
            bb_emit_x86(bb_match_capture()); } return 0;
    case IR_MATCH_ASSIGN_IMM: { bb_prepare(nd); g_emit.op_fc_disp = _zn->cfp; g_emit.op_cap_frame_off = _zn->aslot;
        if (getenv("SCRIP_CAP_DIAG"))
        fprintf(stderr, "[CAP] IMM nd=%p fc_disp=%d off=%d pin=%d refine=%d deep=%d pat=%d\n", (void*)nd, g_emit.op_fc_disp, g_emit.op_off,
        emit_jmp_pin_legacy(), g_emit.flat_fb_refine, g_emit.flat_deep_arrival, g_emit.flat_pat); bb_emit_x86(bb_match_capture()); } return 0;
    case IR_MATCH_ASSIGN_SAVE: { bb_prepare(nd); g_emit.op_cap_frame_off = _zn->aslot; {
        if (_zn->fcg > 0) { g_emit.op_fc_bytes = _zn->fcg; g_emit.op_fc_base = g_emit.op_off; } } {
        if (getenv("SCRIP_CAP_DIAG"))
        fprintf(stderr, "[CAP] SAVE nd=%p save_active=%d fc_bytes=%ld\n", (void*)nd, _zn->sact, (long)g_emit.op_fc_bytes)
        ; } bb_emit_x86(bb_match_capture()); } return 0;
    case IR_MATCH_ALTERNATE: { { int _M = _zn->altM; int _ok = _zn->altok; g_emit.op_alt_M = _ok == 0 ? _M : -1; g_emit.op_alt_cell = _ok != 0 ? 1 : 0; int _na = nd->n_operands / 2;
            g_emit.op_alt_pad = xci_ints(&g_emitter.g_alt_pads, (uint32_t)(_na > 0 ? _na : 1));
                for (int _j = 0; _j < _na; _j++) g_emit.op_alt_pad[_j] = _zr->v_altpad[(size_t)(_zn->padoff + _j)]; }
        g_emit.op_sa = 0; { extern const char * blob_alt_first(const IR_t *);
        g_emit.op_alt_first = blob_alt_first(nd); } bb_emit_x86(bb_match_alternate()); g_emit.op_alt_first = (const char *)0; } return 0;
    case IR_MATCH_FENCE0: { bb_prepare(nd); g_emit.op_fence0_release = zrow(nd)->f0r; g_emit.op_fence0_floor = zrow(nd)->f0f;
                                     switch ((sno_fence_lit_e)IR_LIT(nd).ival) { case SNO_FENCE_LIT_BARE: bb_emit_x86(bb_match_fence0()); break;
                                         case SNO_FENCE_LIT_FLUSH: bb_emit_x86(bb_match_flush()); break;
                                     default:
                                         fprintf(stderr,
                                         "[EMIT] FATAL IR_MATCH_FENCE0 node %p carries literal %lld, which names neither the bare FENCE nor FLUSH (sno_fence_lit_e in IR.h) -- refused, not emitted as"
                                         " a plain FENCE\n", (void *)nd, (long long)IR_LIT(nd).ival); abort(); } } return 0;
    case IR_MATCH_FENCE1: { bb_prepare(nd);
        g_emit.op_fence_body_kk = _zn->fbody;
        g_emit.op_fence_frame_off = _zn->aslot; bb_emit_x86(bb_match_fence1()); } return 0;
    case IR_MATCH_ABORT: { bb_emit_x86(bb_match_abort()); } return 0;
    case IR_MATCH_TRAP: { bb_prepare(nd); bb_emit_x86(bb_match_trap()); } return 0;
    case IR_SCAN_ALTERNATE: { bb_emit_x86(bb_scan_alternate()); } return 0;
    case IR_TO_BY: { bb_prepare(nd); g_emit.op_range_int_operands = ir_range_operands_must_be_integers(nd); bb_emit_x86(bb_to_by()); } return 0;
    case IR_MAKE_LIST: bb_emit_x86(bb_make_list()); return 0;
    case IR_CONJUNCTION: bb_emit_x86(bb_conjunction()); return 0;
    case IR_GOTO: { if (g_emitter.walk_bb_node_inner__mgt < 0) g_emitter.walk_bb_node_inner__mgt = getenv("MONITOR_GOTO_TAP") ? 1 : 0; extern void emit_mon_label_tap(int32_t);
        if (g_runtime.abi.trace_budget != 0 && g_emitter.walk_bb_node_inner__mgt && g_emit.op_stno > 0) emit_mon_label_tap(g_emit.op_stno); } bb_emit_x86(bb_goto()); return 0;
    case IR_GOTO_DEFERRED: bb_emit_x86(bb_goto_deferred()); return 0;
    case IR_DEFINE_SHIM: { if (nd->n_operands >= 2 && nd->operands[0] && nd->operands[1]) { g_emit.op_sval = IR_LIT(nd->operands[0]).sval; g_emit.lbl_t0 = IR_LIT(nd->operands[1]).sval; }
        g_emit.op_define_role = 4; sig_prepare_define(g_emit.op_sval, g_emit.lbl_t0, 4); bb_emit_x86(bb_define_shim()); return 0; }
    case IR_DEFINE_RT_SHIM: { if (nd->n_operands >= 2 && nd->operands[0] && nd->operands[1]) { g_emit.op_sval = IR_LIT(nd->operands[0]).sval; g_emit.lbl_t0 = IR_LIT(nd->operands[1]).sval; }
        g_emit.op_define_role = 7; sig_prepare_define(g_emit.op_sval, g_emit.lbl_t0, 7); bb_emit_x86(bb_define_rt_shim()); return 0; }
    case IR_DEFINE_ADOPT: { if (nd->γ.node && sr3_gamma_label(nd->γ.node)) { const char *_e = sr3_gamma_label(nd->γ.node); g_emit.op_sval = strncmp(_e, "LBL__", 5) ? _e : _e + 5; }
        g_emit.op_define_role = 3; sig_prepare_define(g_emit.op_sval, g_emit.lbl_t0, 3); bb_emit_x86(bb_define_adopt()); return 0; }
    case IR_DEFINE_CALL: { if (_zn->gcall && nd->n_operands == 1 && nd->operands[0]) { int _as = bb_slot_get(nd->operands[0]); if (_as < 0) _as = _zn->o0off;
            if (_as >= 0) { g_emit.op_fc_wbytes = 16; g_emit.op_fc_base = _as; } }
        g_emit.op_define_role = 0; sig_prepare_define(g_emit.op_sval, g_emit.lbl_t0, 0); bb_emit_x86(bb_define_call()); return 0; }
    case IR_DEFINE_RETURN: { g_emit.op_define_role = 1; sig_prepare_define(g_emit.op_sval, g_emit.lbl_t0, 1); bb_emit_x86(bb_define_return()); return 0; }
    case IR_DEFINE_FRETURN: { g_emit.op_define_role = 2; sig_prepare_define(g_emit.op_sval, g_emit.lbl_t0, 2); bb_emit_x86(bb_define_freturn()); return 0; }
    case IR_DEFINE_BIND: { g_emit.op_sval = IR_LIT(nd).sval; g_emit.lbl_t0 = (const char *)0; g_emit.op_proto = ir_define_bind_proto(nd); g_emit.op_entry = ir_define_bind_entry(nd);
        int _realstub = 0;
        if (g_emit_cfg && g_emit_cfg->n_dentry > 0) for (int _dq = 0; _dq < g_emit_cfg->n_dentry; _dq++) if (g_emit_cfg->dentry_node[_dq] == nd) { g_emit.lbl_t0 = g_emit_cfg->dentry_name[_dq];
            _realstub = g_emit_cfg->dentry_entry[_dq] ? 1 : 0; break; } g_emit.op_define_role = 6;
        sig_prepare_define(g_emit.op_sval, g_emit.lbl_t0, 6); bb_emit_x86(bb_define_bind()); int _d1st = 1;
        if (g_emit_cfg && g_emit.op_sval) for (int _dr = 0; _dr < g_emit_cfg->n_dentry; _dr++) { IR_t *_dp = g_emit_cfg->dentry_node[_dr]; if (_dp == nd) break;
            if (_dp && ir_define_is_bind(_dp) && IR_LIT(_dp).sval && !strcmp(IR_LIT(_dp).sval, g_emit.op_sval)) { _d1st = 0; break; } }
        if (g_is_text && g_emit.lbl_t0 && _d1st && _realstub) { long _sv5 = g_emit.op_ival; g_emit.op_ival = 5; g_emit.op_define_role = 5;
            sig_prepare_define(g_emit.op_sval, g_emit.lbl_t0, 5); bb_emit_x86(bb_define_shim_inline()); g_emit.op_ival = _sv5; }
        return 0; }
    case IR_STATEMENT_BEGIN: { g_emit.op_mon_stmt_tap = ((g_runtime.abi.trace_budget != 0 || x86_zdp_rbp_on()) && g_emit.op_stno > 0) ? 1 : 0;
        { if (g_emit.op_mon_stmt_tap && !g_runtime.eval.rt_fragment_emit && g_emit.op_stno > g_runtime.abi.mon_max_stno) g_runtime.abi.mon_max_stno = g_emit.op_stno; } g_emit.op_fc_bytes = 0;
        bb_emit_x86(bb_statement()); { extern std::string bb_zdp_anchor(long, long); if (g_emitter.walk_bb_node_inner__zdpa < 0) { const char * e = getenv("SCRIP_ZDP_TEARDOWN");
        g_emitter.walk_bb_node_inner__zdpa = (e && *e == '1') ? 1 : 0; } if (g_emitter.walk_bb_node_inner__zdpa) bb_emit_x86(bb_zdp_anchor((long)nd->op, (long)bb_node_id((IR_t *)nd)));
            } g_emit.op_mon_stmt_tap = 0; } return 0;
    case IR_STMT_MARK: g_emit.op_seal = nd->seal; g_emit.op_sval = nd->seal == IR_SEAL_LABEL_HOOK ? IR_LIT(nd).sval : (const char *)0;
        bb_emit_x86(bb_stmt_mark((long)IR_LIT(nd).ival, (long)nd->pat_static)); return 0;
    case IR_SETEXIT_TEST: bb_emit_x86(bb_setexit_test()); return 0;
    case IR_CLEANUP: bb_emit_x86(bb_cleanup()); return 0;
    case IR_TABLE_HDR: { g_emit.op_off = _zn->loff; g_emit.op_fc_bytes = 0; bb_emit_x86(bb_table_hdr()); } return 0;
    case IR_LINE_MARK: if ((long)nd->pat_static > 0) g_emit.icn_line_cur = (long)nd->pat_static; if (IR_LIT(nd).sval && *IR_LIT(nd).sval) g_emit.icn_file_cur = IR_LIT(nd).sval;
        if (g_emit.icn_line_cur > 0) emit_stno_row(0, (int32_t)g_emit.icn_line_cur, g_emit.icn_file_cur); bb_emit_x86(bb_line_mark((long)nd->pat_static, IR_LIT(nd).sval)); return 0;
    case IR_STATEMENT_END:
    case IR_STATEMENT: { g_emit.op_fc_bytes = 0; bb_emit_x86(bb_statement()); } return 0;
    case IR_BOUND: { g_emit.op_sb = 1; g_emit.op_catch = (IR_LIT(nd).ival & 64) && g_emit.op_catch_resume_p; g_emit.op_off = _zn->loff; g_emit.op_fc_bytes = g_emit.op_zres ? g_emitter.g_zd_k : 0; bb_emit_x86(bb_bound()); } return 0;
    case IR_UNMARK: { g_emit.op_sb = 0; g_emit.op_off = _zn->o0off; g_emit.op_fc_bytes = 0;
        { const IR_t * _mk = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0; g_emit.op_catch = _mk && _mk->op == IR_BOUND && (IR_LIT(_mk).ival & 64) && (IR_LIT(nd).ival == 4 || IR_LIT(nd).ival == 8); }
        g_emit.op_unmark_chain = g_emit.op_zres && g_emit.op_zf_unpinned; g_emit.op_upost = _zn->upost;
        g_emit.op_ival = IR_LIT(nd).ival; bb_emit_x86(bb_bound()); } return 0;
    case IR_SUBSCRIPT: g_emit.op_seal = nd->seal; bb_emit_x86(nd->n_operands == 2 ? (IR_LIT(nd).sval && !strncmp(IR_LIT(nd).sval, "lv-check", 8) ? bb_subscript_lvck() : bb_subscript())
                                             : (nd->n_operands == 3 && IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval, "nd2") || !strcmp(IR_LIT(nd).sval, "nd2-lv"))) ? bb_subscript2()
                                             : bb_section()); return 0;
    case IR_DEREF: bb_emit_x86(bb_deref()); return 0;
    case IR_UNIFY_CONST: { extern void unify_prepare(IR_t *); unify_prepare(nd); bb_emit_x86(bb_unify_const()); } return 0;
    case IR_UNIFY_STRUCT: { extern void unify_prepare(IR_t *); unify_prepare(nd); bb_emit_x86(bb_unify_struct()); } return 0;
    case IR_UNIFY_FIRST: { extern void unify_prepare(IR_t *); unify_prepare(nd); bb_emit_x86(bb_unify_first()); } return 0;
    case IR_UNIFY_VALUE: { extern void unify_prepare(IR_t *); unify_prepare(nd); bb_emit_x86(bb_unify_value()); } return 0;
    case IR_RANDOM: bb_emit_x86(bb_random()); return 0;
    case IR_ASSIGN_VAR: g_emit.op_seal = nd->seal; bb_emit_x86(nd->n_operands == 3 ? bb_assign_var_sub() : bb_assign_var()); return 0;
    case IR_REV_ASSIGN: {
        if (g_emit.op_sb == -1) { IR_t * _lv = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0; const char * _vn = _lv ? IR_LIT(_lv).sval : (const char *)0;
            g_emit.op_sval = _vn; g_emit.op_gva_k = (g_gva_active && _vn && !graph_has_local(g_emit_cfg, _vn)) ? gva_index_of(_vn) : -1;
            bb_emit_x86(bb_rev_assign_global()); return 0; }
        bb_emit_x86(bb_rev_assign()); return 0; }
    case IR_REV_SWAP: bb_emit_x86(bb_rev_swap()); return 0;
    case IR_REV_ASSIGN_VAR: bb_emit_x86(bb_rev_assign_var()); return 0;
    case IR_SWAP: bb_emit_x86(bb_swap()); return 0;
    case IR_SWAP_VAR: bb_emit_x86(bb_swap_var()); return 0;
    case IR_PROC_VALUE: bb_emit_x86(bb_proc_value()); return 0;
    case IR_CALL_VALUE: bb_emit_x86(bb_call_value()); return 0;
    case IR_LIMIT: g_emit.op_t0_pop = g_emitter.g_limit_t0_pop; bb_emit_x86(bb_limit()); return 0;
    case IR_LIMIT_GATE: bb_emit_x86(bb_limit_gate()); return 0;
    case IR_REPALT: return 0;
    case IR_ITERATE: bb_emit_x86(bb_iterate()); return 0;
    case IR_SCAN_ENTER: { g_emit.op_sb = 1; g_emit.op_sa = g_emit.op_a_slot;
        g_emit.op_wi = (nd->n_operands > 2 && nd->operands[1]) ? bb_slot_get(nd->operands[1]) : -1; g_emit.op_wj = (nd->n_operands > 2 && nd->operands[2]) ? bb_slot_get(nd->operands[2]) : -1;
        g_emit.op_ww = nd->n_operands > 2 ? 1 : 0; bb_emit_x86(bb_gen_scan()); } return 0;
    case IR_INITIAL: bb_emit_x86(bb_enter_init()); return 0;
    case IR_CREATE: { g_emit.op_activate_proc = IR_LIT(nd).sval; bb_emit_x86(bb_create()); } return 0;
    case IR_ACTIVATE: { g_emit.op_activate_proc = IR_LIT(nd).sval; bb_emit_x86(bb_activate()); } return 0;
    case IR_CORET: bb_emit_x86(bb_coret()); return 0;
    case IR_COFAIL: bb_emit_x86(bb_cofail()); return 0;
    case IR_GATE_ARM: bb_emit_x86(bb_gate_arm()); return 0;
    case IR_GATE: { g_emit.op_catch = 0; g_emit.op_catch_off = -1;
        for (int _q = 0; _q < g_planner.g_zd_n && !g_emit.op_catch; _q++) { const IR_t * _b = g_planner.codegen_flat_chain_body_nodes[_q];
            if (_b && _b->op == IR_BOUND && (IR_LIT(_b).ival & 64) && _b->ω.node == nd && zrow(_b)->loff >= 0) { g_emit.op_catch = 1; g_emit.op_catch_off = zrow(_b)->loff; } }
        bb_emit_x86(bb_gate()); } return 0;
    case IR_DISJUNCTION: { if (nd->n_operands > 0) { { if (_zn->fcg > 0) { g_emit.op_fc_bytes = _zn->fcg; g_emit.op_fc_base = g_emit.op_off; } }
        if (g_emit.op_zres && g_emitter.g_zd_djok) { g_emit.op_fc_bytes = _zn->kown; g_emit.op_fc_base = g_emit.op_off;
            } g_emit.op_dj_bfail = g_emit.op_zres && g_emitter.g_zd_djok && _zn->vdj;
            g_emitter.g_zd_djok = 0;
            g_emit.op_dj_rec = (_zn->djret && !_zn->djx && g_emit.op_xf_off != -1) ? 1 : 0;
            bb_emit_x86(bb_disjunction()); return 0; }
                                                  g_emit.op_ival = 0; bb_emit_x86(bb_gate()); } return 0;
    case IR_SCAN: { IR_t *_en = (nd->n_operands > 0) ? nd->operands[0] : NULL; IR_t *_bv = _zn->vp1;
        g_emit.op_sb = (IR_LIT(nd).dval == 4.0) ? 4 : ((nd->n_operands > 2 && !_bv) ? 2 : (IR_LIT(nd).dval == 3.0 ? 3 : 0)); g_emit.op_off = _zn->o0off;
        g_emit.op_sa = (g_emit.op_sb == 2 && _en && _en->n_operands > 0) ? _zn->go0off :
            (_bv ? ((_bv->op == IR_VAR && IR_LIT(_bv).sval) ? _zn->vp1vso : bb_slot_get(_bv) >= 0 ? bb_slot_get(_bv) : _zn->vp1off) : -1);
        g_emit.op_sc = (g_emit.op_sb == 4 && _en && _en->n_operands > 0) ? _zn->go0off : -1; g_emit.op_ival = _zn->loff;
        g_emit.lbl_t0 = g_emitter.g_scan_body_beta ? g_emitter.g_scan_body_beta->name : NULL; g_emit.lbl_t0_p = g_emitter.g_scan_body_beta; g_emit.op_t0_pop = g_emitter.g_scan_t0_pop;
        bb_emit_x86(bb_gen_scan()); } return 0;
    case IR_SCAN_TAB: bb_emit_x86(bb_scan_tab()); return 0;
    case IR_SCAN_MOVE: bb_emit_x86(bb_scan_move()); return 0;
    case IR_SCAN_UPTO: { IR_t * _ca = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        if (g_emit.op_name1 && _ca) g_emit.op_ival = (_ca->n_operands > 0 && _ca->operands[0] && _ca->operands[0]->op == IR_LIT_INTEGER) ? IR_LIT(_ca->operands[0]).ival :
        (int64_t)strlen(g_emit.op_name1); bb_emit_x86(bb_scan_upto()); } return 0;
    case IR_SCAN_ANY: { IR_t * _ca = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        if (g_emit.op_name1 && _ca) g_emit.op_ival = (_ca->n_operands > 0 && _ca->operands[0] && _ca->operands[0]->op == IR_LIT_INTEGER) ? IR_LIT(_ca->operands[0]).ival :
        (int64_t)strlen(g_emit.op_name1); bb_emit_x86(bb_scan_any()); } return 0;
    case IR_SCAN_MANY: { IR_t * _ca = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        if (g_emit.op_name1 && _ca) g_emit.op_ival = (_ca->n_operands > 0 && _ca->operands[0] && _ca->operands[0]->op == IR_LIT_INTEGER) ? IR_LIT(_ca->operands[0]).ival :
        (int64_t)strlen(g_emit.op_name1); bb_emit_x86(bb_scan_many()); } return 0;
    case IR_SCAN_FIND: { IR_t * _fa = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
                                    if (g_emit.op_name1 && _fa) g_emit.op_ival = (_fa->n_operands > 0 && _fa->operands[0] && _fa->operands[0]->op == IR_LIT_INTEGER) ? IR_LIT(_fa->operands[0]).ival :
                                        (int64_t)strlen(g_emit.op_name1);
                                    bb_emit_x86(bb_scan_find()); } return 0;
    case IR_SCAN_MATCH: { IR_t * _ma = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
                                    if (g_emit.op_name1 && _ma) g_emit.op_ival = (_ma->n_operands > 0 && _ma->operands[0] && _ma->operands[0]->op == IR_LIT_INTEGER) ? IR_LIT(_ma->operands[0]).ival :
                                        (int64_t)strlen(g_emit.op_name1);
                                    bb_emit_x86(bb_scan_match()); } return 0;
    case IR_SCAN_POS: bb_emit_x86(bb_scan_pos()); return 0;
    case IR_SCAN_BAL: bb_emit_x86(bb_scan_bal()); return 0;
    case IR_RETURN: {
        IR_t *rv = (nd->n_operands > 0 && nd->operands[0]) ? nd->operands[0] : (IR_t *)0;
        if (!rv) g_emit.op_zread[0] = -1;
        { int _sa = rv ? bb_slot_get(rv) : -1; if (_sa < 0 && rv) { int _ns = _zn->o0off; if (_ns >= 0) _sa = _ns; } g_emit.op_sa = _sa;
            } g_emit.op_dval = IR_LIT(nd).dval;
        if (g_emit.flat_gen && g_emitter.g_suspend_resume_slot >= 0) { g_emit.op_sb = g_emitter.g_suspend_resume_slot; g_emit.lbl_t1_p = g_emit.flat_fail_p;
            g_emit.lbl_t1 = g_emit.flat_fail_p ? g_emit.flat_fail_p->name : (const char *)0; }
        else { g_emit.op_sb = -1; g_emit.lbl_t1_p = (bb_label_t *)0; g_emit.lbl_t1 = (const char *)0; }
        bb_emit_x86(bb_return()); return 0; }
    case IR_CALL_PL_BLOCK: { g_emit.op_call_route = bb_call_route_classify(nd); call_fc_fill(nd); sig_prepare(g_emit.op_sval, (int)g_emit.op_ival); pl_block_prepare(nd);
        bb_emit_x86(bb_call_pl_block()); return 0; }
    case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN:
    case IR_CALL_ICON: case IR_CALL_SNOBOL4:
    case IR_PROC_GEN:
    case IR_CALL: { g_emit.op_call_route = bb_call_route_classify(nd); call_fc_fill(nd); sig_prepare(g_emit.op_sval, (int)g_emit.op_ival); bb_emit_x86(bb_call()); return 0; }
    case IR_CALL_DIRECT: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_direct); return 0;
    case IR_CALL_PL_LEAF: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_pl_leaf); return 0;
    case IR_CALL_PAS_BI: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_pas_bi); return 0;
    case IR_CALL_CALLEE_REC: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_callee_rec); return 0;
    case IR_CALL_FIELD_REC: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_field_rec); return 0;
    case IR_CALL_BYNAME: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_byname); return 0;
    case IR_CALL_TRY: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_try); return 0;
    case IR_CALL_METHOD: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_method); return 0;
    case IR_CALL_REDISP: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_redisp); return 0;
    case IR_CALL_APPLY: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_apply); return 0;
    case IR_CALL_EVAL: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_eval); return 0;
    case IR_CALL_GEN: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_gen); return 0;
    case IR_FAIL: bb_emit_x86(bb_fail()); return 0;
    case IR_CUT: bb_emit_x86(bb_cut()); return 0;
    case IR_UNOP:
    case IR_UNOP_TEST: case IR_NULLTEST_VAR: {
        { if (_zn->vw > 0 && _zn->vbin && !g_emit.op_zres) g_emit.op_wpop += _zn->vw; }
        bb_emit_x86(bb_unop()); return 0; }
    case IR_FIELD_GET: case IR_FIELD_VAR: g_emit.op_seal = nd->seal; bb_emit_x86(bb_field_get()); return 0;
    default:
        fprintf(out, "# [walk_bb_node: kind=%d unhandled]\n", (int)nd->op);
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int drive_value_slot(IR_t *nd) {
    int e = bb_slot_get(nd);
    if (e >= 0) return e;
    if (nd) {
        int slot = zrow_own(nd);
        if (slot >= 0) return slot;
        slot = zrow_xoff(nd);
        if (slot >= 0) return slot;
    }
    if (nd && ir_node_produces_value(nd->op)) {
                fprintf(stderr, "FATAL drive_value_slot: IR op=%d is a value-producer with no frame slot — planner_cells_graph never granted it. "
                        "Emit-time allocation is ERADICATED (TMP-ERADICATE); add the grant in LOWER, never patch it here. op=%d\n", (int)nd->op, (int)nd->op);
        abort();
    }
    fprintf(stderr, "[TE] GOUGE drive_value_slot(op=%d): non-value-producer asked for a slot with no LOWER grant — emit-time allocation is ERADICATED "
                    "(TMP-ERADICATE); add an planner_cells_graph grant for this op\n", nd ? (int)nd->op : -1);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_parts_reserve(int n) {
    if (n <= g_emit.op_parts_cap) return;
    int cap = g_emit.op_parts_cap > 0 ? g_emit.op_parts_cap : 32; while (cap < n) cap *= 2;
    int64_t * grown = (int64_t *) ct_grow(g_emit.op_parts_ival, (size_t) cap * sizeof(int64_t));
    if (!grown) { fprintf(stderr, "emit: op_parts table: out of memory at %d parts\n", n); exit(2); }
    g_emit.op_parts_ival = grown; g_emit.op_parts_cap = cap;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void flat_drive_gate(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t *node_γ, bb_label_t *node_ω, const unsigned char *zon, const int *zout,
    const int *xb) {
    IR_t *nd = nodes[i]; int N = nd->n_operands;
    const int * gv = &g_planner.g_zd_rec_p->v_gate[(size_t)zrow(nd)->gto];
    g_emit.xa_bb_emit_pair_n = 0;
    for (int j = 0; j < N; j++) {
        IR_t *arm = nd->operands[j];
        IR_t *rtgt = (arm && arm->n_operands > 0) ? arm->operands[0] : (IR_t *)0;
        int wantb = arm ? (int) arm->seal : 0;
        bb_label_t *t = node_ω;
        if (rtgt && rtgt->op == IR_SUCCEED) t = node_γ;
        else if (rtgt && rtgt->op != IR_FAIL) { int k = nidx(nodes, n, rtgt); if (k >= 0) t = wantb ? betas[k] : lbls[k]; }
        xa_pair_push(NULL, t);
        if (gv[j] != INT_MIN) XA_PAIR(j).rsp = gv[j];
        else if (zrow(nd)->gcell) XA_PAIR(j).rsp = 16;
    }
    g_emit.op_off = drive_value_slot(nd);
    DRIVE_FILL(nd, lbls[i], node_γ, node_ω, betas[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void flat_drive_match_alt(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t **na_s, bb_label_t **na_f, bb_label_t **na_fo, bb_label_t ***fc_sig, bb_label_t *node_γ,
    bb_label_t *node_ω, bb_label_t *chain_ω) {
    IR_t *nd = nodes[i]; int N = (nd->op == IR_MATCH_ARBNO) ? 1 : (nd->op == IR_DISJUNCTION) ? (int)IR_LIT(nd).ival : nd->n_operands / 2;
    g_emit.xa_bb_emit_pair_n = 0;
    for (int j = 0; j < N; j++) { IR_t *e = nd->operands[2 * j]; bb_label_t *t = node_ω; { int k = nidx(nodes, n, e); if (k >= 0) { t = lbls[k];
        } } if (nd->op == IR_DISJUNCTION && e && e->op == IR_FAIL && chain_ω) t = chain_ω; xa_pair_push(NULL, t); }
    for (int j = 0; j < N; j++) { IR_t *r = nd->operands[2 * j + 1]; bb_label_t *t = node_ω; { int k = nidx(nodes, n, r); if (k >= 0) { t = betas[k];
        } } if (nd->op == IR_MATCH_ARBNO && nd->n_operands > 3 && nd->operands[3] == nd) t = na_fo[i]; if (nd->op == IR_DISJUNCTION && r == nd) t = na_fo[i]; xa_pair_push(NULL, t); }
    const int * dj = nd->op == IR_DISJUNCTION ? &g_planner.g_zd_rec_p->v_dj[(size_t)zrow(nd)->djo] : (const int *)0;
    if (dj) for (int j = 0; j < N; j++) if (dj[7 * j] != INT_MIN) XA_PAIR(N + j).rsp = dj[7 * j];
    if (dj && !zrow(nd)->djx && g_emitter.g_zd_on_cur && g_emitter.g_zd_n_cur == n && g_emitter.g_zd_on_cur[i])
        for (int j = 0; j < N; j++) if (nd->operands[2 * j + 1] != nd && nidx(nodes, n, nd->operands[2 * j + 1]) < 0) XA_PAIR(N + j).rsp = g_emitter.g_zd_k + g_emitter.g_zd_wpop;
    if (dj) for (int j = 0; j < N; j++) if (dj[7 * j + 1] != INT_MIN) XA_PAIR(N + j).rsp = dj[7 * j + 1];
    { xa_pair_push(na_s[i], NULL); }
    { xa_pair_push(na_f[i], NULL); }
    if (nd->op == IR_MATCH_ARBNO) { IR_t *_t = (nd->n_operands > 2) ? nd->operands[2] : (IR_t *)0; { IR_t *_s0 = (nd->n_operands > 1) ? nd->operands[1] : (IR_t *)0; int _i0 = -1, _i1 = -1;
        for (int k = 0; k < n; k++) { if (_s0 && nodes[k] == _s0) _i0 = k; if (_t && nodes[k] == _t) _i1 = k; } if (_i0 > _i1) { int _sw = _i0; _i0 = _i1; _i1 = _sw;
        } if (_i0 >= 0) for (int k = _i1; k >= _i0; k--) if (nodes[k] && nodes[k] != nd && nodes[k]->γ.node == nd) { _t = nodes[k]; break; } } bb_label_t *_tb = node_ω;
        for (int k = 0; k < n; k++) if (_t && nodes[k] == _t) { _tb = betas[k]; break; } xa_pair_push(NULL, _tb); }
    if (nd->op == IR_MATCH_ALTERNATE && fc_sig && fc_sig[i])
        for (int j = 0; j < N; j++) { xa_pair_push(fc_sig[i][j], NULL); }
    { xa_pair_push(na_fo[i], NULL); }
    g_emit.op_off = drive_value_slot(nd); g_emit.op_ival = (int64_t)N;
    if (nd->op == IR_DISJUNCTION) { emit_parts_reserve(N); for (int j = 0; j < N; j++) { IR_t *rj = (2 * N + j < nd->n_operands) ? nd->operands[2 * N + j] : (IR_t *)0;
        { int zok = g_emitter.g_zd_on_cur && g_emitter.g_zd_n_cur == n && g_emitter.g_zd_on_cur[i] && g_planner.g_zd_zarm_p &&
              g_planner.g_zd_zarm_nodes == nodes;
          if (zok && rj && dj[7 * j + 2] >= 0 && dj[7 * j + 3]) { g_emit.op_parts_ival[j] = dj[7 * j + 2]; continue; } }
        { int _g = 0;
        while (rj && rj->op == IR_CONJUNCTION && rj->n_operands > 0 && _g++ < 16) rj = rj->operands[0]; }
        int rs = rj ? emit_binop_opnd_slot(rj, zrow_partoff(nd, j), zrow_partvs(nd, j)) : -1; g_emit.op_parts_ival[j] = rs;
        int lasn = dj[7 * j + 4];
        if (lasn) { int voff = zrow_partvs(nd, j); g_emit.op_parts_ival[j] = voff >= 0 ? voff : -1; }
        if (dj[7 * j + 5] >= 0) { g_emit.op_parts_ival[j] = dj[7 * j + 5]; continue; }
        if (rj && g_emitter.g_zd_on_cur && g_emitter.g_zd_n_cur == n && g_emitter.g_zd_on_cur[i]) { int kr = nidx(nodes, n, rj);
            if (kr >= 0 && g_emitter.g_zd_on_cur[kr] && !dj[7 * j + 6] && !lasn) g_emit.op_parts_ival[j] = -1;
            }
        if (rj && !lasn && g_emit.op_parts_ival[j] >= 0 && nidx(nodes, n, rj) < 0 && x86_key_unbacked(g_emit.op_parts_ival[j])) g_emit.op_parts_ival[j] = -1;
        } g_emit.op_parts_n = N; }
    DRIVE_FILL(nd, lbls[i], node_γ, node_ω, betas[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_reentry(void) { if (g_emitter.sn4_arbno_reentry_v < 0) { const char * e = getenv("SCRIP_ARBNO_REENTRY"); g_emitter.sn4_arbno_reentry_v = (e && *e == '1') ? 1 : 0;
    } return g_emitter.sn4_arbno_reentry_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cap_fail_retreat(void) {
     if (g_emitter.cap_fail_retreat__cf < 0) { const char * e = getenv("SCRIP_CAP_FAIL_RETREAT"); g_emitter.cap_fail_retreat__cf = (e && *e == '0') ? 0 : 1; } return g_emitter.cap_fail_retreat__cf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int * xci_ints(cv_t * v, uint32_t n) { if (v->cap < n) { uint32_t c = v->cap ? v->cap : 256; while (c < n) c *= 2; v->p = ct_alloc((size_t)c * sizeof(int)); v->cap = c;
    v->esz = (uint32_t)sizeof(int); } v->len = n; return (int *)v->p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_blob_casmark(void) {
    const zeta_plan_t * r = g_planner.g_zd_rec_p;
    if (!r) { fprintf(stderr, "FATAL sn4_blob_casmark: no published plan record to read the blob CAS-mark switch from (R-1990)\n"); abort(); }
    return r->casmark;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_defer_resume(void) {
     if (g_emitter.sn4_defer_resume__dres < 0) { const char * e = getenv("SCRIP_DEFER_RESUME"); g_emitter.sn4_defer_resume__dres = (e && *e == '0') ? 0 : 1; } return g_emitter.sn4_defer_resume__dres;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_altsib(void) { if (g_emitter.sn4_arbno_altsib_v < 0) { const char * e = getenv("SCRIP_ARBNO_ALTSIB"); g_emitter.sn4_arbno_altsib_v = (e && *e == '0') ? 0 : 1;
    } return g_emitter.sn4_arbno_altsib_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_seal_omega(void) {
    const char * e = getenv("SCRIP_ARBNO_SEAL_OMEGA"); return (e && *e == '0') ? 0 : 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_tailbeta(void) {
     if (g_emitter.sn4_arbno_tailbeta__atb < 0) { const char * e = getenv("SCRIP_ARBNO_TAILBETA"); g_emitter.sn4_arbno_tailbeta__atb = (e && *e == '0') ? 0 : 1;
         } return g_emitter.sn4_arbno_tailbeta__atb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_seam_walk(void) {
     if (g_emitter.sn4_seam_walk__sw < 0) { const char * e = getenv("SCRIP_SEAM_WALK"); g_emitter.sn4_seam_walk__sw = (e && *e == '0') ? 0 : 1; } return g_emitter.sn4_seam_walk__sw;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_alt_carrier(void) {
     if (g_emitter.sn4_alt_carrier__ac < 0) { const char * e = getenv("SCRIP_ALT_CARRIER"); g_emitter.sn4_alt_carrier__ac = (e && *e == '0') ? 0 : 1; } return g_emitter.sn4_alt_carrier__ac;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sn4_blob_choice_scan(int * n_choice, int * leaf_ok, int * has_fence) {
    int _nc = 0, _lf = 1, _fn = 0;
    if (g_emit_cfg) { _nc = g_emit_cfg->pf.choice_n; _lf = g_emit_cfg->pf.choice_leaf; _fn = g_emit_cfg->pf.choice_fence; }
    if (n_choice) *n_choice = _nc; if (leaf_ok) *leaf_ok = _lf; if (has_fence) *has_fence = _fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int blob_frame_scope(void) {
    extern int g_flat_frame_floor;
    return (g_emit.flat_jmp_entry && g_emit.flat_pat && !(g_flat_frame_floor > 0)) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int blob_carve_pad(void) { if (g_emitter.blob_carve_pad_v < 0) { const char * e = getenv("SCRIP_BLOB_CARVE_PAD"); g_emitter.blob_carve_pad_v = e ? 16 * atoi(e) : 0;
    } return g_emitter.blob_carve_pad_v; }
int blob_frame_bytes(void) { const zeta_plan_t * r = g_planner.g_zd_rec_p; return (g_emit_cfg && r && r->cfg == g_emit_cfg) ? r->blob_bytes : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int x86_diag_regs_on_c(void) { if (g_emitter.x86_diag_regs_on_c_v < 0) { const char * e = getenv("SCRIP_DIAG_REGS"); g_emitter.x86_diag_regs_on_c_v = (e && *e && *e != '0') ? 1 : 0;
    } return g_emitter.x86_diag_regs_on_c_v; }
extern "C" const char * emit_gc_map_name(int i) { return (i >= 0 && i < g_emitter.g_gc_map_names_n) ? CV_AT(g_emitter.g_gc_map_names_v, char *, i) : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const znode_t * zrow(const IR_t * nd) {
    const zeta_plan_t * r = g_planner.g_zd_rec_p;
    int ri = (r && nd && nd == g_emit.node && g_emit.op_zrow >= 0 && g_emit.op_zrow < r->n && r->nodes[g_emit.op_zrow] == nd) ? g_emit.op_zrow : zrow_of(nd);
    if (ri < 0) {
        fprintf(stderr, "FATAL zrow: node %s reaches the emitter with no plan row to copy its facts from (R-1990)\n", nd ? bb_op_name(nd->op) : "NONE");
        abort();
    }
    return &r->zn[ri];
}
int zrow_opoff(const IR_t * nd, int k) { return (k >= 0 && k < nd->n_operands) ? g_planner.g_zd_rec_p->v_opoff[(size_t)(zrow(nd)->opo + k)] : -1; }
static int zrow_partoff(const IR_t * nd, int j) { return g_planner.g_zd_rec_p->v_partoff[(size_t)(zrow(nd)->pto + j)]; }
int zrow_opvs(const IR_t * nd, int k) { return (nd && k >= 0 && k < nd->n_operands) ? g_planner.g_zd_rec_p->v_opvs[(size_t)(zrow(nd)->opo + k)] : -1; }
static int zrow_partvs(const IR_t * nd, int j) { return g_planner.g_zd_rec_p->v_partvs[(size_t)(zrow(nd)->pto + j)]; }
extern "C" int emit_zrow_actoff(const IR_t * nd) { return zrow(nd)->actoff; }
extern "C" int emit_zrow_bspk(const IR_t * nd) { return zrow(nd)->bspk; }
extern "C" int emit_zrow_adead(const IR_t * nd) { const zeta_plan_t * r = g_planner.g_zd_rec_p; int ri = (r && nd) ? zrow_of(nd) : -1; return (ri >= 0 && ri < r->n) ? r->zn[ri].adead : 0; }
extern "C" int emit_zrow_zchk(const IR_t * nd) {
    const zeta_plan_t * r = g_planner.g_zd_rec_p; if (!r || !nd || !getenv("SCRIP_ZD_RTCHECK")) return INT_MIN;
    int ri = (nd == g_emit.node && g_emit.op_zrow >= 0 && g_emit.op_zrow < r->n && r->nodes[g_emit.op_zrow] == nd) ? g_emit.op_zrow : zrow_of(nd);
    if (ri < 0 || !r->on[ri] || !r->xb || r->xb[ri] == INT_MIN || (nd->op == IR_UNMARK && (IR_LIT(nd).ival & 6))) return INT_MIN;
    return r->xb[ri] + r->out[ri];
}
extern "C" int emit_zrow_sprec(const IR_t * nd) { return zrow(nd)->sprec; }
extern "C" int emit_zrow_gcell(const IR_t * nd) { return zrow(nd)->gcell; }
extern "C" int emit_zrow_tret(const IR_t * nd) { return zrow(nd)->tret; }
extern "C" long emit_zsm_word(const IR_t * nd, int op_zres, int op_subj_cell, long owner) {
    const zeta_plan_t * r = g_planner.g_zd_rec_p;
    int i = (r && nd && r->nodes) ? nidx(r->nodes, r->n, nd) : -1;
    if (i < 0 || !r->on[i]) return 0L;
    const znode_t * z = &r->zn[i];
    long own = z->zsmown ? (long)z->zsmown - ((!op_zres && op_subj_cell) ? 16L : 0L) : 0L;
    int end = (nd->op == IR_MATCH_END || nd->op == IR_MATCH_REPLACE) && owner > 0L;
    return (z->zsmw & ~0xFFFFL) | (((z->zsmw & 0xFFFFL) + own) & 0xFFFFL) | ((long)(own != 0L) << 55) | ((long)end << 56);
}
extern "C" int emit_zrow_release(int below, int kind, const int ** offs) {
    const znode_t * z = zrow(g_emit.node);
    int w = (below && kind == 5) ? 0 : (below && kind == 4) ? 1 : (!below && kind == 4) ? 2 : -1;
    if (z->relo < 0 || w < 0) { fprintf(stderr, "FATAL emit_zrow_release: node %s has no release list %d/%d in its plan row (R-1990)\n", bb_op_name(g_emit.node->op), below, kind); abort(); }
    const int * v = g_planner.g_zd_rec_p->v_rel.data() + z->relo;
    for (int i = 0; i < w; i++) v += 1 + v[0];
    *offs = v + 1;
    return v[0];
}
int zrow_of(const IR_t * nd) {
    const zeta_plan_t * r = g_planner.g_zd_rec_p;
    if (!nd || !r || !r->home || r->n != g_planner.g_zd_n || g_planner.g_zd_nodes_p != g_planner.codegen_flat_chain_body_nodes) return -1;
    return nidx(g_planner.codegen_flat_chain_body_nodes, r->n, nd);
}
static int zrow_own(const IR_t * nd) { int ri = zrow_of(nd); return ri >= 0 ? g_planner.g_zd_rec_p->zn[ri].loff : -1; }
static int zrow_xoff(const IR_t * nd) {
    const zeta_plan_t * r = g_planner.g_zd_rec_p;
    if (!nd || !r || !r->home || r->n != g_planner.g_zd_n || g_planner.g_zd_nodes_p != g_planner.codegen_flat_chain_body_nodes) return -1;
    std::unordered_map<const IR_t *, int>::const_iterator it = r->v_xoff.find(nd);
    return it == r->v_xoff.end() ? -1 : it->second;
}
static int zrow_under(int ri) { const zeta_plan_t * r = g_planner.g_zd_rec_p; return (ri >= 0 && r && r->under) ? r->under[ri] : 0; }
extern "C" long emit_last_pkt_chain_off(void) { return g_emit.flat_pkt_chain_off; }
static void zop_audit_graph_close(void) {
    int m, frames;
    { if (!g_emitter.zop_audit_reg) { g_emitter.zop_audit_reg = 1; atexit(zop_audit_report); } }
    m = g_emit.zop_seen;
    if (m == 0) return;
    g_emit.zop_seen = 0;
    frames = ((m >> 1) & 1) + ((m >> 3) & 1) + ((m >> 4) & 1);
    g_emitter.zop_g_total++;
    if (frames > 1) g_emitter.zop_g_mixed++;
    g_emitter.zop_hist[(m >> 1) & 15]++;
}
static void zop_audit_report(void) {
    zop_audit_graph_close();
    if (!getenv("SCRIP_ZOP_AUDIT")) return;
    fprintf(stderr, "[ZOP] graphs=%ld mixed=%ld\n", g_emitter.zop_g_total, g_emitter.zop_g_mixed);
    {
        int k;
        const char * nm[16] = {"-","isle","cell","isle+cell","rsp","isle+___","cell+___","isle+cell+___","rsp","isle+rsp","cell+rsp","isle+cell+rsp","___+rsp","+","cell+___+rsp","all"};
        for (k = 0; k < 16; k++) if (g_emitter.zop_hist[k]) fprintf(stderr, "[ZOP]   %-18s %ld\n", nm[k], g_emitter.zop_hist[k]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_chain_prepare(IR_t * entry) {
    g_emit.stno_last = -1; g_emit.icn_line_cur = 0; g_emit.icn_file_cur = (const char *)0;
    g_emitter.g_bb_slotmap_n = 0; g_emitter.g_bb_slotix_gen++;
    { zop_audit_graph_close(); g_emit.zop_seen = 0; }
    g_emit.op_zres = 0; g_emit.op_zf_unpinned = 0; g_emit.op_zabs = 0; g_emit.op_zrel = 0;
    g_emit.flat_all_zd = 0;
    g_emit.flat_deep_arrival = emit_graph_has_deep_arrival(g_emit_cfg);
    { int _rc_own = (g_emit_cfg && g_emit_cfg->resumable_callable) ? 1 : 0;
        g_emit.flat_fb_refine = (g_emit.flat_deep_arrival && !g_emit.flat_pat && !g_emit.flat_gen && !_rc_own) ? emit_fb_stmt_scan(g_emit_cfg) : 0;
      { if (g_emitter.emit_chain_fbdbg2 < 0) { const char * e = getenv("SCRIP_FB_DEBUG"); g_emitter.emit_chain_fbdbg2 = (e && *e == '1') ? 1 : 0;
          } if (g_emitter.emit_chain_fbdbg2 && !g_emit.flat_fb_refine)
          fprintf(stderr, "[FB-STMT] hook skip deep=%d pat=%d gen=%d gp=%d rc=%d n=%d\n", g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, g_resumable_callable_active,
          g_emit_cfg ? g_emit_cfg->n : -1); } }
    { extern int g_flat_outer_nparams; extern int g_flat_frame_floor; g_emit.flat_outer_nparams = g_flat_outer_nparams; (void)g_flat_frame_floor;
      { int _gen_ok = !g_emit.flat_gen;
        g_emit.flat_lcl_proc = (!g_emit.flat_pat && _gen_ok && g_emit_cfg && ((g_emit.flat_jmp_entry && (g_emit_cfg->nparams > 0 || g_emit_cfg->nlocals > 0)) || g_emit_cfg->cell_frame)) ? 1 : 0;
            }
      g_emit.frame_retained = g_emit_cfg ? g_emit_cfg->pf.retains : 0;
      g_emit.zframe_graph = g_emit_cfg ? g_emit_cfg->pf.zframe : 0;
      if ((g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->cell_frame)) && g_emit.flat_frame_bytes == 0) g_emit.flat_frame_bytes = g_emit_cfg ? g_emit_cfg->pf.frame_bytes :
          ((FLAT_FRAME_ALLOWANCE + 15) & ~15); }
    g_last_flat_frame_bytes = g_emit_cfg ? g_emit_cfg->jcon_value_region : 0;
    { if (getenv("SCRIP_N2_FT_PROBE") && g_emit_cfg) { int _rg = g_emit_cfg->jcon_value_region, _np = g_emit_cfg->nparams, _nl = g_emit_cfg->nlocals;
        fprintf(stderr, "[N2-FT] EMIT gen=%d zframeA=%d icncells=%d lclproc=%d region=%d ffb=%d np=%d nl=%d zls=%d ft=%d predft=%d\n", g_emit.flat_gen ? 1 : 0, g_emit.zframe_graph ? 1 : 0,
            g_emit_cfg->cell_frame ? 1 : 0, g_emit.flat_lcl_proc ? 1 : 0, _rg, g_emit.flat_frame_bytes, _np, _nl, g_emit_cfg->pf.fp_total,
                g_emit.zframe_graph ? g_emit.flat_frame_bytes : g_emit.flat_frame_bytes + (_np + _nl) * 16, g_emit.zframe_graph ? ((48 + _rg + 15) & ~15) : ((48 + _rg + 15) & ~15) + (_np + _nl) * 16);
                    } }
    g_last_flat_zstatic = (g_emit_cfg && !g_emit.flat_layout_unknown) ? g_emit_cfg->pf.zstatic : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * kw_bare(const char * s) { return !s ? "" : (s[0] == '&' ? s + 1 : s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_defer_beta_guard(void) { if (g_emitter.sn4_defer_beta_guard__bg < 0) { const char * e = getenv("SCRIP_DEFER_BETA_GUARD"); g_emitter.sn4_defer_beta_guard__bg = (e && *e == '0') ? 0 : 1;
    } return g_emitter.sn4_defer_beta_guard__bg; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_define_fold(void) { if (g_emitter.sn4_define_fold__df < 0) { const char * e = getenv("SCRIP_DEFINE_FOLD"); g_emitter.sn4_define_fold__df = (e && *e == '0') ? 0 : 1;
    } return g_emitter.sn4_define_fold__df; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_descr_stamp(void) { if (g_emitter.sn4_descr_stamp_on < 0) { const char * e = getenv("SCRIP_DESCR_STAMP"); g_emitter.sn4_descr_stamp_on = (e && *e == '1') ? 1 : 0;
    } return g_emitter.sn4_descr_stamp_on; }
}
int bcps_arg_slot(IR_t * call, int i) {
    IR_t * a = ir_call_arg(call, i);
    if (a) { int s = bb_slot_get(a); if (s < 0) s = zrow_opoff(call, i); if (s >= 0) return s; }
    return 0;
}
int bcps_result_slot() {
    IR_t * nd = _.node;
    { int _s = nd ? zrow(nd)->loff : -1; if (_s >= 0) return _s; }
    return -1;
}
int bcps_beta_pair_idx() {
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
        if (XA_PAIR(i).define == _.lbl_β_p && XA_PAIR(i).jmp) return i;
    return -1;
}
void bcps_sig_tally(const char * arm, const char * fn, long n, int ok, const char * why, const char * opnd) {
     if (g_emitter.bcps_sig_tally__sd < 0) {
    const char * _e = getenv("SCRIP_SIG_DIAG");
    g_emitter.bcps_sig_tally__sd = (_e && *_e == '1') ? 1 : 0;
}
    if (!g_emitter.bcps_sig_tally__sd) return;
    fprintf(stderr, "[SIG] arm=%s fn=%s nargs=%ld verdict=%s why=%s opnd=%s\n", arm, fn ? fn : "?", n, ok ? "SIG" : "DECLINE", why, (opnd && *opnd) ? opnd : "-");
}
int bcfn_result_slot(IR_t * nd) {
    { int _s = nd ? zrow(nd)->loff : -1; if (_s >= 0) return _s; }
    return -1;
}

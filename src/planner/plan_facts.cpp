#include "planner_head.h"
#include "planner.h"
extern "C" int fc_tail_defer_susp_g(IR_graph_t * g, const IR_t * nd);
}
extern "C" int fc_leaf_cell(const IR_t * nd, long * k);
extern "C" long fc_vwpop(const IR_t * nd);
extern "C" int fc_vbinop_active(const IR_t * nd);
extern "C" long zw_carve_k(const IR_t * nd);
extern "C" int zc_nofc(void);
extern "C" int zdp_mode(void);
extern "C" int zdp_alpha(const IR_t * nd);
extern "C" int zdp_beta(const IR_t * nd);
extern "C" int zdp_known_alpha(const IR_t * nd);
extern "C" int zdp_known_beta(const IR_t * nd);
extern "C" int fc_tail_head(const IR_t * head);
extern "C" int fc_tail_release(const IR_t * head, int * brdisp);
extern "C" int fc_head_fp(const IR_t * nd);
extern "C" int fc_cond_fp(const IR_t * nd);
extern "C" int fc_save_active(const IR_t * nd);
extern "C" int fc_vdj_active(const IR_t * nd);
extern "C" int fc_vread_fp(const IR_t * nd);
extern "C" int fc_call_active(const IR_t * nd);
extern "C" int zls_home_why(const IR_t * nd);
extern "C" int zls_argv_off(const IR_t * nd);
extern "C" int zls_redisp_off(const IR_t * nd);
extern "C" int zls_frame_off(const IR_graph_t * g, const IR_t * nd);
extern "C" int zls_key_unbacked(const IR_graph_t * g, int off);
extern "C" int zls_g_generator_top(const IR_graph_t * g);
extern "C" int zls_act_off(const IR_t * nd);
extern "C" int planner_call_spine_record(const IR_graph_t * cg, const IR_t * nd);
extern "C" int planner_bound_spine(const IR_graph_t * g, const IR_t * nd);
extern "C" int planner_gate_cells(const IR_graph_t * g, const IR_t * gate);
extern "C" void planner_pdef_lock(const IR_graph_t * g);
extern "C" int planner_pl_definite(const IR_graph_t * g, const IR_t * nd);
extern "C" void planner_pdef_unlock(void);
extern "C" int fc_call_active(const IR_t * nd);
extern "C" int zls_dj_rbp_arms(const IR_t * nd);
extern "C" int zls_home_why(const IR_t * nd);
extern "C" int zls_g_home_frame(const IR_graph_t * g);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pf_trivial_beta(const IR_t * nd) {
    switch (nd->op) {
    case IR_MATCH_POS: case IR_MATCH_RPOS: return 1;
    case IR_MATCH_ABORT: return IR_LIT(nd).ival != 1;
    case IR_MATCH_ASSIGN_SAVE: { long k = 0; return fc_geom(nd, &k) && k > 0; }
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pf_unwind_beta(const IR_t * nd) {
    switch (nd->op) {
    case IR_VAR: case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_ATOM: case IR_BINOP: case IR_UNOP: case IR_ASSIGN: case IR_ASSIGN_VAR:
    case IR_CMP_TEST: case IR_COERCE_NUMERIC: case IR_COERCE_STRING: case IR_COERCE_INTEGER: case IR_COERCE_REAL:
    case IR_IDENT: case IR_DIFFER:
    case IR_DEREF: case IR_SUBSCRIPT: case IR_FIELD_VAR: case IR_FIELD_GET: return 1;
    case IR_MATCH_ALTERNATE: return 1;
    case IR_MATCH_ARBNO: return 1;
    default: return pf_trivial_beta(nd);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zls_g_home_frame(const IR_graph_t * g);
extern "C" int fc_seq_active(const IR_t * nd);
extern "C" int fc_cells_active(void);
extern "C" int fc_alt_fpmax(const IR_t * nd);
extern "C" int zdp_seam_tier(const IR_t * nd);
extern "C" long zw_node_k(const IR_t * nd);
extern "C" int zls_g_vslot_count(const IR_graph_t *);
extern "C" const char * zls_g_vslot_get(const IR_graph_t *, int, int *);
static int pf_local_spine(const IR_graph_t * g, const char * nm) {
    int nv = zls_g_vslot_count(g);
    for (int k = 0; k < nv; k++) {
        int o = -1;
        const char * vn = zls_g_vslot_get(g, k, &o);
        if (!vn || strcmp(vn, nm)) continue;
        int cov = k < (int)g_planner.zst_vcov.size() ? g_planner.zst_vcov[(size_t)k] : 0, use = k < (int)g_planner.zst_vuse.size() ? g_planner.zst_vuse[(size_t)k] : -1;
        return cov > 0 && cov == use;
    }
    return 0;
}
extern "C" int zls_g_release_act_offs(const IR_graph_t * g, const IR_t * scope_nd, int kind, int * out, int cap);
static void pf_rel(zeta_plan_t * rec, const IR_graph_t * g, const IR_t * sc, int kind) {
    int n = g ? zls_g_release_act_offs(g, sc, kind, (int *)0, 0) : 0;
    if (n < 0) n = 0;
    size_t at = rec->v_rel.size();
    rec->v_rel.push_back(n);
    rec->v_rel.resize(at + 1 + (size_t)n);
    if (n > 0) n = zls_g_release_act_offs(g, sc, kind, rec->v_rel.data() + at + 1, n);
    rec->v_rel[at] = n;
    rec->v_rel.resize(at + 1 + (size_t)n);
}
extern "C" { int stage2_owner_varslot(const char * proc, const char * var); const char * stage2_owner_l3_ancestor(const char * proc); }
static int pf_display_off(int lvl, const char * owner_proc) {
    if (lvl <= 3 || !owner_proc) return -1;
    const char * l3proc = stage2_owner_l3_ancestor(owner_proc);
    if (!l3proc) return -1;
    char nm[32]; snprintf(nm, sizeof nm, "__pas_display_%d", lvl);
    return stage2_owner_varslot(l3proc, nm);
}
static int pf_lay_cmp(const void * a, const void * b) { int x = GC_LAY_OFF(*(const uint64_t *)a), y = GC_LAY_OFF(*(const uint64_t *)b); return x < y ? -1 : (x > y ? 1 : 0); }
static void pf_blob_layout(zeta_plan_t * rec) {
    std::vector<uint64_t> & v = rec->v_blob_lay;
    v.clear();
    if (rec->blob_bytes <= 0) return;
    auto push = [&](int off, unsigned kind, int size) { v.push_back(GC_LAY_Q(off, kind, size)); };
    push(0, GC_LAY_RAW, 8); push(8, GC_LAY_PTR_CODE, 8); push(16, GC_LAY_PTR_CODE, 8); push(-8, GC_LAY_PTR_CODE, 8); push(-16, GC_LAY_PTR_CODE, 8); push(-32, GC_LAY_DESCR, 16);
    if (rec->casmark) { push(-40, GC_LAY_RAW, 8); push(-48, GC_LAY_RAW, 8); } else push(-40, GC_LAY_RAW, 8);
    const IR_graph_t * ag = g_planner.g_act_g;
    if (ag && g_planner.g_act_all == ag->all)
        for (int j = 0; j < ag->n; j++) {
            int units = g_planner.g_act_units[(size_t)j], slot = g_planner.g_act_slot[(size_t)j], cls = g_planner.g_act_cls[(size_t)j];
            if (units <= 0 || slot == -1) continue;
            if (cls == ACT_CLASS_FENCE) { push(slot, GC_LAY_RAW, 16); push(slot + 16, GC_LAY_RAW, 16); }
            else if (cls == ACT_CLASS_ARBNO || cls == ACT_CLASS_CAPTURE) push(slot, GC_LAY_RAW, 16);
            else if (cls == ACT_CLASS_REGEX) push(slot, GC_LAY_RAW, 16 * units);
            else push(slot, GC_LAY_DESCR, 16);
        }
    push(-(planner_blob_head_bytes() + planner_blob_slot_bytes()), GC_LAY_RAW, 8);
    qsort(v.data(), v.size(), sizeof(uint64_t), pf_lay_cmp);
}
static int pf_row(IR_t ** nodes, int n, const IR_t * x) { if (!x) return -1; for (int q = 0; q < n; q++) if (nodes[q] == x) return q; return -1; }
static void pf_dj(zeta_plan_t * rec, IR_t ** nodes, int n, int i, const IR_graph_t * g, int djx) {
    IR_t * nd = nodes[i]; int N = (int)IR_LIT(nd).ival;
    int aok = djx && g_planner.g_zd_out_p && g_planner.g_zd_nodes_p == nodes;
    int cok = zd_edge_retains_g(g, nd) && g_planner.g_zd_nodes_p == nodes && g_planner.g_zd_xb_p && g_planner.g_zd_out_p && g_planner.g_zd_plan_on && g_planner.g_zd_claim_p &&
        g_planner.g_zd_fam_p && g_planner.g_zd_fbase_p && g_planner.g_zd_gp_p;
    for (int j = 0; j < N; j++) {
        IR_t * r = nd->operands[2 * j + 1]; int k = pf_row(nodes, n, r), A = INT_MIN, C = INT_MIN;
        if (aok) A = (r == nd) ? djx : (k >= 0 ? g_planner.g_zd_out_p[i] - g_planner.g_zd_out_p[k] : 0);
        if (cok) {
            const unsigned char * zon = g_planner.g_zd_plan_on; const int * zout = g_planner.g_zd_out_p, * xb = g_planner.g_zd_xb_p, * gp = g_planner.g_zd_gp_p;
            int e = zd_dj_arm_end(nodes, n, i, j), kk = pf_row(nodes, n, zd_chase(nd->operands[2 * j + 1]));
            long rr = 0, da = 0, dk = 0;
            if (e >= 0 && zon[e]) {
                if (zd_pl_dj_arm_holds(nd, j) && kk >= 0 && zon[kk] && zd_chase(nodes[kk]->γ.node) == nd) {
                    int ca = zd_cpos(e, xb, zout, &da), ck = zd_cpos(kk, xb, zout, &dk);
                    if (ca != INT_MIN && ca == ck) C = (int)((da - gp[e]) - (dk - gp[kk]));
                }
                else if (zd_pl_dj_arm_holds(nd, j)) { if (kk >= 0 && zon[kk] && zd_gate_pair_g(g, nodes, n, e, kk, 1, zon, zout, xb, &rr)) C = (int)rr; }
                else { int ca = zd_cpos(e, xb, zout, &da), ck = zd_cpos(i, xb, zout, &dk); da -= gp[e]; if (ca != INT_MIN && ca == ck) C = (int)(da - dk); }
            }
        }
        IR_t * rj = (2 * N + j < nd->n_operands) ? nd->operands[2 * N + j] : (IR_t *)0, * rq = rj;
        int cell = rj && g_planner.g_zd_zarm_p && g_planner.g_zd_zarm_nodes == nodes &&
            ((rj->op == IR_CONJUNCTION && !zd_dj_admitted(g_planner.g_zd_zarm_n, i)) || zd_dj_cellless(nodes, n, g_planner.g_zd_zarm_p, pf_row(nodes, n, rj)));
        { int q = 0; while (rq && rq->op == IR_CONJUNCTION && rq->n_operands > 0 && q++ < 16) rq = rq->operands[0]; }
        int lasn = (rq && zd_icn_local_assign_g(g, rq) && zd_k_own_g(g, rq) == 0) ? 1 : 0, lvar = rq ? zd_icn_local_var_g(g, rq) : 0, coord = -1;
        if (rq && zls_g_home_frame(g) && (lvar || lasn)) { int sv = g_planner.g_zd_cur; g_planner.g_zd_cur = i; coord = zst_coord(IR_LIT(rq).sval); g_planner.g_zd_cur = sv; }
        int v[7] = { A, C, rj ? zls_off(rj) : -1, cell ? 1 : 0, lasn, coord, lvar };
        rec->v_dj.insert(rec->v_dj.end(), v, v + 7);
    }
}
static void pf_gate(zeta_plan_t * rec, IR_t ** nodes, int n, int i, const IR_graph_t * g) {
    IR_t * nd = nodes[i]; int N = nd->n_operands; const unsigned char * zon = rec->on; const int * zout = rec->out, * xb = rec->xb;
    int hold = xb && zd_edge_retains_g(g, nd) && g_planner.g_zd_nodes_p == nodes && g_planner.g_zd_claim_p && g_planner.g_zd_fam_p && g_planner.g_zd_fbase_p && g_planner.g_zd_gp_p;
    for (int j = 0; j < N; j++) {
        IR_t * arm = nd->operands[j], * rtgt = (arm && arm->n_operands > 0) ? arm->operands[0] : (IR_t *)0;
        int wantb = arm ? (int)arm->seal : 0, k = -1, V = INT_MIN;
        if (rtgt && rtgt->op != IR_SUCCEED && rtgt->op != IR_FAIL) k = pf_row(nodes, n, rtgt);
        { int a = arm ? pf_row(nodes, n, arm) : -1; long r = 0;
            if (hold && k >= 0 && a >= 0 && zon[a] && zon[k] && zd_gate_pair_at_g(g, nodes, n, i, a, k, wantb, zon, zout, xb, &r)) { rec->v_gate.push_back((int)r); continue; } }
        if (xb && k >= 0 && zon[i] && zon[k] && xb[i] != INT_MIN && xb[k] != INT_MIN) V = (xb[i] + zout[i]) - (xb[k] + zout[k] - (wantb ? 0 : zd_k_g(g, nodes[k])));
        else if (xb && g_planner.g_zd_fam_p && g_planner.g_zd_claim_p && g_planner.g_zd_gp_p && g_planner.g_zd_nodes_p == nodes) {
            const int * cl = g_planner.g_zd_claim_p, * fm = g_planner.g_zd_fam_p, * fb = g_planner.g_zd_fbase_p;
            int a = arm ? pf_row(nodes, n, arm) : -1, lf = (a >= 0 && zon[a] && cl[a] >= 0) ? fm[cl[a]] : -1, kk = 0, kf = -1; long kd = 0;
            if (k >= 0 && zon[k] && cl[k] >= 0 && xb[k] != INT_MIN) { kd = xb[k] + zout[k] - (wantb ? 0 : zd_k_g(g, nodes[k])); kk = 1; }
            else if (k >= 0 && zon[k] && cl[k] >= 0 && fm[cl[k]] >= 0) { kd = fb[cl[k]] + zout[k] - (wantb ? 0 : zd_k_g(g, nodes[k])); kk = 2; kf = fm[cl[k]]; }
            if (lf >= 0 && kk == 1 && zd_edge_retains_g(g, nodes[k])) { IR_t * lt = zd_chase(nodes[k]->γ.node); int lq = lt ? pf_row(nodes, n, lt) : -1;
                if (lq >= 0 && cl[lq] >= 0 && fm[cl[lq]] == lf) { kd = 0; kk = 2; kf = lf; } }
            long di = (zon[i] && cl[i] >= 0 && fm[cl[i]] >= 0 && fm[cl[i]] == lf) ? fb[cl[i]] + zout[i] : 0;
            if (lf >= 0 && kk == 2 && kf == lf) V = (int)(di - kd);
        }
        rec->v_gate.push_back(V);
    }
}
static int pf_beta_kind_keeps(const IR_graph_t * g, IR_t * nd) { int op = nd ? (int)ir_kind0(nd) : -1;
    return (nd &&
        (ir_is_generator_kind(ir_kind0(nd)) || op == IR_SUSPEND || op == IR_CALL || op == IR_CALL_PROC_STAGED || op == IR_CALL_BUILTIN_GEN || op == IR_PROC_GEN || op == IR_REPALT || op == IR_LIMIT ||
        op == IR_GOTO || op == IR_STATEMENT_BEGIN || (g && nd == g->body_root))) ? 1 : 0; }
static int pf_scan_body_beta_keeps(const IR_graph_t * g, IR_t * nd) { int op = nd ? (int)nd->op : -1; return (nd && (pf_beta_kind_keeps(g, nd) || op == IR_SCAN_TAB || op == IR_SCAN_MOVE)) ? 1 : 0; }
static int pf_scan_conduit_enters_a_do_clause(IR_t ** nodes, int n, IR_t * c) {
    for (int j = 0; j < n; j++) { IR_t * s = nodes[j]; if (!s || s->op != IR_SUSPEND || s->seal != 1 || s->n_operands < 2) continue;
        IR_t * x = s->operands[1]; int h = 0;
        while (x && x->op == IR_SCAN && x->n_operands > 2 && !x->operands[1] && x->operands[2] && x->operands[2]->op == IR_SCAN && x->operands[2]->n_operands > 2 && !x->operands[2]->operands[1] &&
            h++ < 64) x = x->operands[2];
        if (x == c) return 1; }
    return 0;
}
static void pf_beta_used(zeta_plan_t * rec, IR_t ** nodes, int n, const IR_graph_t * g, unsigned char * used) {
    for (int j = 0; j < n; j++) if (rec->zn[j].fcsa) { for (int k = 0; k < n; k++) used[k] = 1; return; }
    for (int k = 0; k < n; k++) used[k] = pf_beta_kind_keeps(g, nodes[k]);
    for (int j = 0; j < n; j++) {
        int op = (int)nodes[j]->op;
        if (op == IR_MATCH_ALTERNATE || op == IR_MATCH_ARBNO || (op == IR_MATCH_FENCE1 || op == IR_MATCH_FENCE0) || op == IR_SCAN_ALTERNATE || op == IR_DISJUNCTION || op == IR_REPALT)
            for (int a = 0; a < nodes[j]->n_operands; a++) { int k = nidx(nodes, n, nodes[j]->operands[a]); if (k >= 0) used[k] = 1; }
        if (op == IR_SCAN && nodes[j]->n_operands > 2 && nodes[j]->operands[2]) { int k = nidx(nodes, n, nodes[j]->operands[2]); if (k >= 0) used[k] = 1; }
        if (op == IR_GATE_ARM && nodes[j]->n_operands > 0 && nodes[j]->operands[0] && (int)nodes[j]->seal) { int k = nidx(nodes, n, nodes[j]->operands[0]); if (k >= 0) used[k] = 1; }
        int gib = port_sz_beta(nodes[j]->γ.sz), oib = port_sz_beta(nodes[j]->ω.sz); IR_t * gtgt = nodes[j]->γ.node, * otgt = nodes[j]->ω.node;
        { int gg = 0; IR_t * g2 = gtgt; while (g2 && g2->op == IR_GOTO && gg++ < 128) { if (!gib) gib = port_sz_beta(g2->γ.sz); gtgt = g2->γ.node; g2 = gtgt; } }
        { int gg = 0; IR_t * o2 = otgt; while (o2 && o2->op == IR_GOTO && gg++ < 128) { if (!oib) oib = port_sz_beta(o2->γ.sz); otgt = o2->γ.node; o2 = otgt; } }
        if (gib) { int k = nidx(nodes, n, gtgt); if (k >= 0) used[k] = 1; }
        if (oib) { int k = nidx(nodes, n, otgt); if (k >= 0) used[k] = 1; }
    }
}
static int pf_alpha_shared(const IR_graph_t * g, IR_t * nd) {
    if (g && g->n_balias > 0) for (int b = 0; b < g->n_balias; b++) if (g->balias_node[b] == nd) return 0;
    return ir_floater_kind(nd) ? 1 : 0;
}
static void pf_scan_t0(zeta_plan_t * rec, IR_t ** nodes, int n, int i, const IR_graph_t * g, const unsigned char * bu) {
    znode_t * z = &rec->zn[i]; IR_t * nd = nodes[i]; z->sbk = -1; z->sbb = 0; z->sbp = 0;
    if (nd->op != IR_SCAN) return;
    IR_t * bv = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0; int body = -1, bbeta = 0;
    if (bv && (ir_is_generator_kind(bv->op) || bv->op == IR_SCAN_TAB || bv->op == IR_SCAN_MOVE)) { int k = nidx(nodes, n, bv); if (k >= 0) { body = k; bbeta = 1; } }
    if (body < 0 && nd->n_operands > 2) { IR_t * bb2 = nd->operands[2]; int fk = -1;
        { int gb = 0, gg = 0; IR_t * g2 = bb2; while (g2 && g2->op == IR_GOTO && gg++ < 128) { gb |= port_sz_beta(g2->γ.sz); g2 = g2->γ.node; }
          int gk = (bb2 && bb2->op == IR_GOTO && gb && g2) ? nidx(nodes, n, g2) : -1; if (gk >= 0) { body = gk; bbeta = 1; fk = gk; } }
        for (int hops = 0; bb2 && fk < 0 && hops < 8; hops++) {
            int conduit = (IR_LIT(nd).dval == 3.0 || IR_LIT(nd).dval == 4.0) ? 1 : 0, k = nidx(nodes, n, bb2);
            if (k >= 0) { fk = k; body = k;
                bbeta = (!(hops == 0 && !bv && pf_scan_conduit_enters_a_do_clause(nodes, n, nd)) && (pf_scan_body_beta_keeps(g, nodes[k]) || (!conduit && bv && bu[k]))) ? 1 : 0; }
            if (fk < 0) bb2 = bb2->γ.node;
        } }
    int tk = -1, ta = 0;
    if (body >= 0 && bbeta) tk = body;
    if (body >= 0 && !bbeta) { tk = body;
        if (pf_alpha_shared(g, nodes[body])) for (int k = 0; k < n; k++) if (pf_alpha_shared(g, nodes[k]) && ir_floater_kind(nodes[k]) == ir_floater_kind(nodes[body])) { tk = k; break; }
        ta = rec->zn[tk].k; }
    z->sbk = body; z->sbb = bbeta;
    if (tk >= 0 && rec->xb && rec->on[i] && rec->on[tk] && rec->xb[i] != INT_MIN && rec->xb[tk] != INT_MIN) z->sbp = (rec->xb[i] + rec->out[i]) - (rec->xb[tk] + rec->out[tk] - ta);
    else if (tk >= 0 && rec->on[i] && rec->on[tk] && rec->claim[i] >= 0 && rec->claim[i] == rec->claim[tk]) z->sbp = rec->out[i] - (rec->out[tk] - ta);
}
static void pf_cells(zeta_plan_t * rec, IR_t ** nodes, int n, int i, const IR_graph_t * g) {
    znode_t * z = &rec->zn[i]; IR_t * nd = nodes[i]; zrd_t * rd = rec->rd + rec->rdo[i];
    z->fres = zls_frame_resident(nd); z->xk = -1; z->xw = 0; z->xd = -1; z->xa = 0;
    z->gcell = (nd->op == IR_GATE || (nd->op == IR_GATE_ARM && nd->n_operands > 1)) ? planner_gate_cells(g, nd->op == IR_GATE ? nd : nd->operands[1]) : 0;
    if ((nd->op == IR_BOUND || ir_node_produces_value(nd->op)) && rec->on[i] && rec->xb && getenv("SCRIP_PL_DEFCMP") && planner_pl_definite(g, nd) && rec->xb[i] == INT_MIN) {
          extern const char * zls_graph_name_get(const IR_graph_t *);
              fprintf(stderr, "[PL-DEFCMP] graph=%s i=%d %s def0=1 plan=0\n", zls_graph_name_get(g), i, bb_op_name(nd->op)); }
    for (int j = 0; j <= nd->n_operands; j++) { IR_t * op = j < nd->n_operands ? nd->operands[j] : (IR_t *)0; rd[j].kown = op ? zd_k_own_g(g, op) : -1; rd[j].fres = op ? zls_frame_resident(op) : 1; }
    IR_t * ow = nd->op == IR_UNMARK ? (nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0) : nd->op == IR_SCAN ? z->vp1 : (IR_t *)0;
    int b = ow ? nidx(nodes, n, ow) : -1;
    if (b < 0 || !rec->on[i] || !rec->on[b] || zls_off(ow) < 0) return;
    if (nd->op == IR_UNMARK && g && zls_g_home_frame(g) && zls_g_resumable_frame(g) && planner_bound_spine(g, ow) && rec->xb && rec->xb[b] != INT_MIN) {
        z->xk = zls_off(ow); z->xw = zd_k_own_g(g, ow); z->xa = rec->xb[b] + rec->out[b]; return; }
    long xi, xo;
    if (rec->xb && rec->xb[i] != INT_MIN && rec->xb[b] != INT_MIN) { xi = (long)rec->xb[i] + rec->out[i]; xo = (long)rec->xb[b] + rec->out[b]; }
    else if (rec->claim && rec->claim[i] >= 0 && rec->claim[i] == rec->claim[b]) { xi = rec->out[i]; xo = rec->out[b]; }
    else if (rec->fam && rec->fbase && rec->claim && rec->claim[i] >= 0 && rec->claim[b] >= 0 && rec->fam[rec->claim[i]] >= 0 && rec->fam[rec->claim[i]] == rec->fam[rec->claim[b]]) {
        xi = (long)rec->fbase[rec->claim[i]] + rec->out[i]; xo = (long)rec->fbase[rec->claim[b]] + rec->out[b]; }
    else return;
    if (xi < xo) return;
    z->xk = zls_off(ow); z->xw = zd_k_own_g(g, ow); z->xd = (int)(xi - xo);
}
void planner_facts(zeta_plan_t * rec, IR_t ** nodes, int n, const IR_graph_t * g) {
    struct pdef_scope { pdef_scope(const IR_graph_t * x) { planner_pdef_lock(x); } ~pdef_scope() { planner_pdef_unlock(); } } pds(g);
    rec->v_lspine.assign((size_t)(g && g->lnames ? g->nlocals : 0), 0);
    for (int i = 0; g && g->lnames && i < g->nlocals; i++) rec->v_lspine[(size_t)i] = (g->lnames[i] && pf_local_spine(g, g->lnames[i])) ? 1 : 0;
    rec->zn = (znode_t *)ct_zalloc((size_t)(n > 0 ? n : 1), sizeof(znode_t));
    for (int i = 0; i < n; i++) {
        IR_t * nd = nodes[i]; znode_t * z = &rec->zn[i]; long k = 0; int op = (int)nd->op;
        z->fcg = fc_geom(nd, &k) ? (int)k : 0; k = 0; z->leaf = fc_leaf_cell(nd, &k) ? (int)k : 0;
        z->vw = (int)fc_vwpop(nd); z->vbin = fc_vbinop_active(nd); z->aslot = planner_act_slot(nd); z->loff = zls_off(nd);
        { int zr = zls_result_off(nd); z->owni = (z->loff >= 0) ? ((zr >= 0 ? zr : z->loff) + zls_node_bytes(nd)) : 0; }
        z->rlive = zls_result_live(nd); z->carve = zc_nofc() ? 0 : (int)zw_carve_k(nd);
        z->zdpa = (zdp_mode() && zdp_known_alpha(nd)) ? zdp_alpha(nd) : -1; z->zdpb = (zdp_mode() && zdp_known_beta(nd)) ? zdp_beta(nd) : -1;
        z->nops = zd_nops_g(g, nd); z->ssub = zd_scan_subject(nd) ? 1 : 0; z->sticky = zst_s_of(nd);
        z->srole = (op == IR_DEFINE && ir_define_sr_citizen(nd)) ? zd_sr_role(nd) : 0;
        z->kown = zd_k_own_g(g, nd); z->upost = zd_upost_of(nd); z->djx = zd_djx_of(nd); z->djret = zls_dj_retains(nd); z->vdj = fc_vdj_active(nd);
        z->cfp = fc_cond_fp(nd); z->sact = fc_save_active(nd); z->o0off = (nd->n_operands > 0 && nd->operands[0]) ? zls_off(nd->operands[0]) : -1;
        z->vp1 = (op == IR_SCAN && nd->n_operands > 1) ? zd_value_producer_g(g, nd->operands[1]) : (IR_t *)0;
        z->k = zd_k_g(g, nd); z->gt = zd_chase(nd->γ.node); z->ot = zd_chase(nd->ω.node); z->isusp = zd_icn_suspend_g(g, nd); z->trivb = pf_trivial_beta(nd); z->unwb = pf_unwind_beta(nd);
        z->gcall = (nd->γ.node != (IR_t *)0 && fc_call_active(nd->γ.node)) ? 1 : 0;
        z->armrbp = (rec->arm[i] >= 0) ? zls_dj_rbp_arms(nodes[rec->arm[i]]) : 0;
        z->djadm = (op == IR_DISJUNCTION) ? zd_dj_admitted(n, i) : 0;
        z->clr = (op == IR_CLEANUP) ? zd_cl_read_g(g, nodes, n, i, rec->on, rec->out, rec->xb) : 0;
        z->yend = (rec->on[i] && z->isusp && rec->claim && g && g->home_frame && g->spine_cells) ? zd_yield_end(nodes, n, i, rec->on, rec->claim) : -1;
        z->sidx = (z->isusp && nd->n_operands > 1) ? nidx(nodes, n, zd_chase(nd->operands[1])) : -1;
        z->uwj = -1; z->uwd = 0; z->endj = -1; z->otx = -1; z->lgx = -1; z->arr = -1;
        z->hfp = -1; z->hth = 0; z->hrel = 0; z->hbr = -1; z->padoff = -1; z->altok = -1; z->altM = 0; z->fbody = -1;
        if (op == IR_MATCH_BEGIN) { z->hfp = fc_head_fp(nd); z->hth = fc_tail_head(nd); }
        if (op == IR_MATCH_END && nd->n_operands > 0 && nd->operands[0]) {
            IR_t * hd = nd->operands[0]; int br = -1; z->hrel = fc_tail_release(hd, &br); z->hbr = br; z->hfp = fc_head_fp(hd); z->hth = fc_tail_head(hd); }
        if (op == IR_MATCH_FENCE1) z->fbody = planner_body_live(g, nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0, nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0, nd);
        if (op == IR_MATCH_ALTERNATE) {
            int M = 0; z->altok = planner_alt_exit(g, nd, &M); z->altM = M; z->padoff = (int)rec->v_altpad.size();
            for (int j = 0; j < nd->n_operands / 2; j++) rec->v_altpad.push_back(z->altok == 0 ? M - planner_alt_arm_live(g, nd, j) : 0); }
        IR_t * o0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        z->aext = planner_act_extra(nd); z->aexth = o0 ? planner_act_extra(o0) : 32; z->aclass = planner_act_class_of(nd); z->acause = planner_act_cause_of(nd);
        z->vrfp = fc_vread_fp(nd); z->hwhy = zls_home_why(nd); z->fcact = fc_call_active(nd); z->argv = zls_argv_off(nd); z->redisp = zls_redisp_off(nd);
        z->curmov = zd_call_curmov(nd); z->cvn = o0 ? zd_conj_value_node_g(g, o0) : (IR_t *)0;
        z->dsave = (zls_g_generator_top(g) > 0 && z->loff >= 0 && !zls_key_unbacked(g, z->loff)) ? zls_frame_off(g, nd) + 16 : INT_MIN;
        z->trapd = (op == IR_SETEXIT_TEST && rec->on[i]) ? rec->out[i] - z->k : 0; z->zrunb = rec->out[i] >= 0 ? rec->out[i] - (rec->on[i] ? z->k : 0) : 0;
        z->xbp = (rec->out[i] >= 0 && rec->on[i] && rec->xb && rec->xb[i] != INT_MIN && rec->xb[i] > 0) ? rec->xb[i] : 0;
        z->zabs = rec->on[i] ? rec->out[i] + ((rec->xb && rec->xb[i] != INT_MIN) ? rec->xb[i] : 0) : 0; z->zdef = (rec->xb && rec->xb[i] == INT_MIN) ? 0 : 1; z->zcut = rec->cut ? rec->cut[i] : -1;
        z->actoff = zls_act_off(nd); z->sprec = planner_call_spine_record(g, nd);
            z->bspk = (op == IR_UNMARK && o0 && planner_bound_spine(g, o0)) ? zd_k_own_g(g, o0) : 0;
        z->t0x = -1; z->t0p = 0; z->sdb = -2; z->sdbb = 0;
        z->asa = -1; z->asb = -1; z->dsusp = -1;
        if (op == IR_MATCH_ARBNO) { int wl = 0, wb = 0, mo = -1, sp = 0; planner_act_window(nd, &wl, &wb); z->awlo = wl; z->awb = wb;
            z->apk = planner_body_live(g, nd->n_operands > 2 ? nd->operands[1] : (IR_t *)0, nd->n_operands > 2 ? nd->operands[2] : (IR_t *)0, nd);
            z->ak16 = (g && nd->n_operands >= 3 && z->apk > 0 && z->aslot == -1) ? 1 : 0; z->abk0 = z->apk == 0 ? 1 : 0; z->abdu = z->apk < 0 ? 1 : 0;
            if (zls_arbno_geom(nd, &mo, &sp)) { z->ach = 1; z->asa = mo; z->asb = (sp + 24 + 15) & ~15; } }
        if (op == IR_MATCH_DEFER) z->dsusp = fc_tail_defer_susp_g((IR_graph_t *)g, nd);
        if (op == IR_LIMIT) { IR_t * gen = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0, * rs = nd->n_operands > 2 ? nd->operands[2] : (IR_t *)0; int tk = -1;
            if (gen && ir_is_generator_kind(gen->op)) tk = nidx(nodes, n, gen); else if (rs) tk = nidx(nodes, n, rs);
            z->t0x = tk;
            if (tk >= 0 && rec->xb && rec->on[i] && rec->on[tk] && rec->xb[i] != INT_MIN && rec->xb[tk] != INT_MIN) z->t0p = (rec->xb[i] + rec->out[i]) - (rec->xb[tk] + rec->out[tk]);
            else if (tk >= 0 && rec->on[i] && rec->on[tk] && rec->claim && rec->claim[i] >= 0 && rec->claim[i] == rec->claim[tk]) z->t0p = rec->out[i] - rec->out[tk]; }
        if (op == IR_SUSPEND && nd->n_operands > 1 && nd->operands[1]) { IR_t * db = nd->operands[1]; int vb = 0;
            for (int g2 = 0; db->op == IR_GOTO && db->γ.node && g2 < 128; g2++) { vb |= port_sz_beta(db->γ.sz); db = db->γ.node; }
            if (db->op == IR_FAIL) z->sdb = -1;
            else { int kd = nidx(nodes, n, db);
                if (kd >= 0) { int da = nd->seal == 1 && !(db->op == IR_SCAN && db->n_operands > 2 && !db->operands[1]); z->sdb = kd;
                    z->sdbb =
                        (vb || (!da && (ir_is_generator_kind(ir_kind0(db)) || ir_kind0(db) == IR_CALL || ir_kind0(db) == IR_CALL_PROC_STAGED || db->op == IR_SCAN_TAB || db->op == IR_SCAN_MOVE))) ? 1 :
                        0; } } }
        z->djo = -1; z->gto = -1; z->relo = -1; z->dsa = -1; z->dsb = -1;
        if (op == IR_VAR_FRAME || op == IR_ASSIGN_FRAME || op == IR_VAR_REF) {
            const char * asv = o0 ? IR_LIT(o0).sval : (const char *)0, * sv = IR_LIT(nd).sval;
            z->dsa = (asv && sv) ? stage2_owner_varslot(asv, sv) : -1; z->dsb = (nd->seal >= 1 && nd->seal <= 3) ? -1 : pf_display_off(nd->seal, asv);
        }
        if (op == IR_BOUND || op == IR_UNMARK || op == IR_CUT) {
            const IR_t * sb = (nd->n_operands > 0 && nd->operands[0] && nd->operands[0]->op == IR_BOUND) ? nd->operands[0] : nd;
            z->relo = (int)rec->v_rel.size(); pf_rel(rec, g, sb, 5); pf_rel(rec, g, sb, 4); pf_rel(rec, g, nd, 4);
        }
        z->fcsa = (fc_seq_active(nd) || (op == IR_MATCH_ALTERNATE && fc_cells_active() && fc_alt_fpmax(nd) >= 0)) ? 1 : 0; z->seam = zdp_seam_tier(nd);
        { long d = 0; z->zpk = fc_geom(nd, &d) ? -1 : (int)zw_node_k(nd); }
        if (op == IR_DISJUNCTION) { z->djo = (int)rec->v_dj.size(); pf_dj(rec, nodes, n, i, g, z->djx); }
        if (op == IR_GATE && nd->n_operands > 0) { z->gto = (int)rec->v_gate.size(); pf_gate(rec, nodes, n, i, g); }
        z->opo = (int)rec->v_opoff.size();
        for (int k = 0; k < nd->n_operands; k++) rec->v_opoff.push_back(nd->operands[k] ? zls_off(nd->operands[k]) : -1);
    }
    std::vector<int> ad((size_t)(n > 0 ? n : 1), -1);
    for (int j = 0; j < n; j++) {
        IR_t * ot = rec->zn[j].ot, * gt = rec->zn[j].gt; int oi = ot ? nidx(nodes, n, ot) : -1, gi = gt ? nidx(nodes, n, gt) : -1;
        for (int p = 0; p < 2; p++) {
            int t = p == 0 ? oi : gi; if (t < 0 || (p == 1 && oi == gi)) continue;
            int e = -1;
            if (oi == t) e = rec->on[j] ? rec->out[j] - rec->zn[j].k - rec->wp[j] : -2;
            if (gi == t) { int gv = rec->on[j] ? rec->out[j] - rec->gp[j] : -2; e = (e == -1 || e == gv) ? gv : -2; }
            if (e == -1 || ad[(size_t)t] == -2) continue;
            ad[(size_t)t] = (e < 0 || (ad[(size_t)t] >= 0 && ad[(size_t)t] != e)) ? -2 : e;
        }
    }
    for (int i = 0; i < n; i++) rec->zn[i].arr = ad[(size_t)i];
    for (int i = 0; i < n; i++) {
        znode_t * z = &rec->zn[i]; int ti = z->sidx; z->suc = 0;
        if (!(rec->on[i] && z->isusp && ti >= 0 && rec->on[ti])) continue;
        int bi = (rec->xb && rec->xb[i] != INT_MIN) ? rec->xb[i] : 0, bt = (rec->xb && rec->xb[ti] != INT_MIN) ? rec->xb[ti] : 0, ke = z->yend, yd = ke >= 0 ? rec->out[ke] - rec->gp[ke] : rec->out[i];
        int ta = (z->sdb == ti && !z->sdbb) ? rec->zn[ti].k : 0;
        z->suc = (bi + yd) - (bt + rec->out[ti] - ta);
    }
    for (int i = 0; i < n; i++) {
        IR_t * nd = nodes[i]; znode_t * z = &rec->zn[i]; int op = (int)nd->op;
        if (op == IR_LIMIT && zls_g_home_frame(g)) for (int q = 2; q < nd->n_operands; q++) {
            IR_t * lg = nd->operands[q]; int x = (lg && lg->op == IR_LIMIT_GATE && zls_home_why(lg)) ? nidx(nodes, n, lg) : -1; if (x >= 0) z->lgx = x; }
        if (!rec->on[i] || rec->wp[i] <= 0 || op == IR_STATEMENT || op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END) continue;
        { int e2 = -1; for (int j = i + 1; j < n; j++) { if (nodes[j]->op == IR_STATEMENT_BEGIN) break; if (nodes[j]->op == IR_STATEMENT_END || nodes[j]->op == IR_STATEMENT) { e2 = j; break; } }
          if (e2 >= 0 && rec->gp[e2] == rec->wp[i] + z->k && z->ot && z->ot == rec->zn[e2].gt) { int kf = nidx(nodes, n, z->ot); if (kf >= 0) { z->endj = e2; z->otx = kf; } } }
        int bg = 0;
        for (int j = i - 1; j >= 0; j--) { int o = (int)nodes[j]->op; if (o == IR_STATEMENT_BEGIN) { bg = 1; break; } if (o == IR_STATEMENT_END || o == IR_STATEMENT) break; }
        if (!bg || !z->ot) continue;
        for (int j = i - 1; j >= 0; j--) {
            int o = (int)nodes[j]->op; if (o == IR_STATEMENT_BEGIN || o == IR_STATEMENT_END || o == IR_STATEMENT) break;
            if (rec->on[j] && rec->wp[j] > 0 && rec->zn[j].unwb && rec->zn[j].ot == z->ot) { z->uwj = j; z->uwd = (rec->wp[i] + z->k) - (rec->wp[j] + rec->zn[j].k); break; } }
    }
    std::unordered_map<const IR_t *, int> ra0; for (int r = 0; r < n; r++) if (nodes[r]->op == IR_REPALT && nodes[r]->n_operands > 0) ra0.emplace(nodes[r]->operands[0], r);
    for (int k = 0; k < n; k++) {
        znode_t * z = &rec->zn[k]; z->ws = 0; z->wek = ZW_NONE; z->wex = -1;
        if (beta_is_stmt_land(nodes[k])) continue;
        int cur = k, hops = 0; long sum = 0;
        while (cur >= 0 && hops++ < 64 && rec->zn[cur].trivb) {
            IR_t * c = nodes[cur], * o = c->ω.node; int ib = port_sz_beta(c->ω.sz), ip = (c->ω.sz[0] == (char)0xcf && (unsigned char)c->ω.sz[1] == 0x86); sum += rec->zn[cur].fcg;
                if (rec->wp[cur] > 0) sum += rec->wp[cur];
            { int gg = 0; IR_t * g2 = o; while (g2 && g2->op == IR_GOTO && gg++ < 128) { if (!ib) ib = port_sz_beta(g2->γ.sz);
                if (!ip) ip = (g2->γ.sz[0] == (char)0xcf && (unsigned char)g2->γ.sz[1] == 0x86); o = g2->γ.node; g2 = o; } }
            int k2 = nidx(nodes, n, o); cur = -1;
            if (k2 >= 0) { if (ip && ir_glue_box(nodes[k2], 1)) { z->wek = ZW_NAFO; z->wex = k2; } else if (ib) { z->wek = ZW_BETA; z->wex = k2; cur = k2; } else { z->wek = ZW_ALPHA; z->wex = k2; } }
            else { z->wek = (o && o->op == IR_SUCCEED) ? ZW_GAMMA : ZW_OMEGA; z->wex = -1; }
            { auto ro = ra0.find(c); if (ro != ra0.end()) { z->wek = ZW_REPALT; z->wex = ro->second; cur = -1; } }
        }
        z->ws = (int)sum;
    }
    unsigned char * bu = (unsigned char *)alloca(n > 0 ? (size_t)n : 1);
    pf_beta_used(rec, nodes, n, g, bu);
    for (int q = 0; q < 2 * n; q++) if (rec->chn[q] >= 0) bu[rec->chn[q]] = 1;
    { const char * e = getenv("SCRIP_UNWIND"); if (!(e && *e == '0')) for (int i = 0; i < n; i++) if (rec->zn[i].uwj >= 0 && rec->zn[i].uwd >= 0) bu[rec->zn[i].uwj] = 1; }
    for (int i = 0; i < n; i++) rec->zn[i].bu = bu[i];
    for (int i = 0; i < n; i++) pf_scan_t0(rec, nodes, n, i, g, bu);
    for (int i = 0; i < n; i++) pf_cells(rec, nodes, n, i, g);
}
static int pf_blob_patv_reader(const IR_t * nd) { const char * sv = (nd && nd->op == IR_VAR) ? IR_LIT(nd).sval : (const char *)0; const char * d = sv ? strstr(sv, "$V") : (const char *)0;
    if (!d || strncmp(sv, "PAT$", 4)) return 0; char * e = (char *)0; (void) strtol(d + 2, &e, 10); return (e && e != d + 2 && !*e) ? 1 : 0; }
static int pf_blob_wire_clobber_scan(const IR_graph_t * g) {
    if (!g) return 0;
    for (int j = 0; j < g->n; j++) { IR_t * m = g->all[j]; if (!m) continue; int o = (int)ir_kind0(m);
        if (o == IR_MATCH_DEFER || o == IR_CALL || o == IR_CALL_VALUE || o == IR_MATCH_VALUE || o == IR_MATCH_CALLOUT) return 1; }
    return 0;
}
static int pf_fence0_dyn_on() { if (g_planner.fence0_dyn_on_v < 0) { const char * e = getenv("SCRIP_FENCE0_DYNAMIC"); g_planner.fence0_dyn_on_v = (e && *e == '0') ? 0 : 1;
    } return g_planner.fence0_dyn_on_v; }
static int pf_fence0_dyn_floor(const zeta_plan_t * rec, const IR_t * nd) {
    int _fzd = getenv("SCRIP_FZ_DIAG") ? 1 : 0; const IR_graph_t * g = rec->cfg;
    if (!pf_fence0_dyn_on()) return 0;
    if (!nd || !g || nd->op != IR_MATCH_FENCE0) return 0;
    if (!rec->scope) { if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p dyn: not blob scope\n", (const void *)nd); return 0; }
    int n = g->n; int guard = 0; IR_t * cur = zd_chase(nd->γ.node); const IR_t ** met = (const IR_t **)alloca(sizeof(IR_t *) * (size_t)(n + 1)); int nmet = 0;
    while (cur && guard++ <= n) { int mo = (int)ir_kind0(cur);
        if (mo == IR_MATCH_END) break;
        if (mo == IR_MATCH_ALTERNATE || mo == IR_MATCH_ARBNO || mo == IR_MATCH_FENCE1 || mo == IR_MATCH_FENCE0 || mo == IR_MATCH_VALUE || mo == IR_CALL || mo == IR_CALL_VALUE ||
            mo == IR_DISJUNCTION || mo == IR_MATCH_ABORT) { if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p dyn: dynamic successor %s\n", (const void *)nd, bb_op_name((IR_e)mo)); return 0; }
        if ((mo == IR_MATCH_ASSIGN_COND || mo == IR_MATCH_ASSIGN_IMM) && cur->n_operands > 1) { int own = 0; for (int s = 0; s < nmet; s++) if (met[s] == cur->operands[1]) { own = 1; break; }
            if (!own) { if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p dyn: successor %s reads a save cell the fence would drop\n", (const void *)nd, bb_op_name((IR_e)mo)); return 0; } }
        met[nmet++] = cur;
        cur = zd_chase(cur->γ.node); }
    if (guard > n) { if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p dyn: successor chase did not terminate\n", (const void *)nd); return 0; }
    int floor = rec->blob_bytes > 0 ? rec->blob_bytes + 16 : 0;
    if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p dyn: FLOOR=%d\n", (const void *)nd, floor);
    return floor;
}
void planner_blob_facts(zeta_plan_t * rec) {
    const IR_graph_t * g = rec->cfg; IR_t ** nodes = rec->nodes_own; int n = rec->n, bytes = 0;
    if (rec->scope && g) {
        int pend = 0; if (planner_blob_casmark()) for (int i = 0; i < g->n; i++) { IR_t * m = g->all[i]; if (m && m->op == IR_MATCH_ASSIGN_COND) { pend = 1; break; } }
        int sb = planner_blob_slot_bytes();
        bytes = (sb == 0 && !pend && !pf_blob_wire_clobber_scan(g) && planner_blob_exit_live(g) >= 0) ? 0 : planner_blob_head_bytes() + sb;
    }
    rec->blob_bytes = bytes; rec->spf = (bytes <= 0 && rec->scope) ? 1 : 0; rec->sph = 0; rec->spgd = 0;
    rec->casmark = planner_blob_casmark(); pf_blob_layout(rec);
    if (rec->spf) {
        int need = planner_blob_casmark(); for (int q = 0; q < n; q++) if (pf_blob_patv_reader(nodes[q])) need = 1;
        rec->sph = need ? (planner_blob_casmark() ? 32 : 16) : 0; rec->spgd = -1;
        for (int q = 0; q < n; q++) { IR_t * gt = rec->zn[q].gt; if (gt && gt->op != IR_SUCCEED) continue; int d = rec->on[q] ? rec->out[q] : -2;
            rec->spgd = (rec->spgd == -1 || rec->spgd == d) ? d : -2; }
    }
    for (int i = 0; i < n; i++) {
        znode_t * z = &rec->zn[i]; IR_t * nd = nodes[i]; z->hrsp = (rec->sph > 0 && rec->on[i]) ? rec->out[i] + rec->sph - 8 : -1; z->dtk = -1; z->dtph = 0; z->dtd = 0;
        z->f0f = nd->op == IR_MATCH_FENCE0 ? pf_fence0_dyn_floor(rec, nd) : 0;
        if (nd->op != IR_MATCH_DEFER) continue;
        for (int a = 0; a < nd->n_operands; a++) { IR_t * so = nd->operands[a];
            if (!so || !(so->op == IR_GOTO || so->op == IR_MATCH_ARBNO || so->op == IR_MATCH_ABORT)) continue;
            int phi = 0, gg = 0; while (so && so->op == IR_GOTO && gg++ < 128) { if (!phi) phi = (so->γ.sz[0] == (char)0xcf && (unsigned char)so->γ.sz[1] == 0x86); so = so->γ.node; }
            int k = nidx(nodes, n, so);
            if (k >= 0) { z->dtk = k; z->dtph = phi;
                if (rec->spf && so->op == IR_MATCH_ABORT) { int da = rec->zn[k].arr, dj = rec->on[i] ? rec->out[i] : -1; if (da == -1) da = 0; z->dtd = (dj < 0 || da < 0 || dj < da) ? -1 : dj - da;
                    } }
            break;
        }
    }
}

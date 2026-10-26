#include "../emitter/emit_head.h"
#include "../emitter/emit_decls.h"
#include "planner.h"
int port_sz_beta(const char * sz);
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
extern "C" int planner_bound_spine(const IR_graph_t * g, const IR_t * nd);
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
void planner_facts(zeta_plan_t * rec, IR_t ** nodes, int n, const IR_graph_t * g) {
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
        z->actoff = zls_act_off(nd); z->bspk = (op == IR_UNMARK && o0 && planner_bound_spine(g, o0)) ? zd_k_own_g(g, o0) : 0;
        z->t0x = -1; z->t0p = 0; z->sdb = -2; z->sdbb = 0;
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
        z->djo = -1; z->gto = -1;
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
}

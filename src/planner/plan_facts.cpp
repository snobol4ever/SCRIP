#include "../emitter/emit_head.h"
#include "../emitter/emit_decls.h"
#include "planner.h"
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

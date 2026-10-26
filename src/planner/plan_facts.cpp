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
        z->nops = zd_nops(nd); z->ssub = zd_scan_subject(nd) ? 1 : 0; z->sticky = zst_s_of(nd);
        z->srole = (op == IR_DEFINE && ir_define_sr_citizen(nd)) ? zd_sr_role(nd) : 0;
        z->kown = zd_k_own(nd); z->upost = zd_upost_of(nd); z->djx = zd_djx_of(nd); z->djret = zls_dj_retains(nd); z->vdj = fc_vdj_active(nd);
        z->cfp = fc_cond_fp(nd); z->sact = fc_save_active(nd); z->o0off = (nd->n_operands > 0 && nd->operands[0]) ? zls_off(nd->operands[0]) : -1;
        z->vp1 = (op == IR_SCAN && nd->n_operands > 1) ? zd_value_producer(nd->operands[1]) : (IR_t *)0;
        z->hfp = -1; z->hth = 0; z->hrel = 0; z->hbr = -1; z->padoff = -1; z->altok = -1; z->altM = 0; z->fbody = -1;
        if (op == IR_MATCH_BEGIN) { z->hfp = fc_head_fp(nd); z->hth = fc_tail_head(nd); }
        if (op == IR_MATCH_END && nd->n_operands > 0 && nd->operands[0]) {
            IR_t * hd = nd->operands[0]; int br = -1; z->hrel = fc_tail_release(hd, &br); z->hbr = br; z->hfp = fc_head_fp(hd); z->hth = fc_tail_head(hd); }
        if (op == IR_MATCH_FENCE1) z->fbody = planner_body_live(g, nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0, nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0, nd);
        if (op == IR_MATCH_ALTERNATE) {
            int M = 0; z->altok = planner_alt_exit(g, nd, &M); z->altM = M; z->padoff = (int)rec->v_altpad.size();
            for (int j = 0; j < nd->n_operands / 2; j++) rec->v_altpad.push_back(z->altok == 0 ? M - planner_alt_arm_live(g, nd, j) : 0); }
    }
}

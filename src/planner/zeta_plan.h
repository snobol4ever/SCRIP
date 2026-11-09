#ifndef SCRIP_ZETA_PLAN_H
#define SCRIP_ZETA_PLAN_H
#ifndef __cplusplus
#error zeta_plan.h is C++ only (the plan record holds std::vector columns)
#endif
#include <vector>
#include <unordered_map>
#include <stdint.h>
#include "ir/ir_fwd.h"
#include "ir/stage2.h"
struct zrd_t { int dist, xf, fr, key, kind, anchor, kown, fres; };
struct znode_t {
    int fcg, leaf, vw, vbin, aslot, loff, owni;
    int rlive, carve, zdpa, zdpb, nops, ssub, sticky, srole, kown, upost, djx, djret, vdj, cfp, sact, o0off;
    int hfp, hth, hrel, hbr, altok, altM, padoff, fbody;
    int k, isusp, yend, clr, djadm, armrbp, trivb, unwb, gcall, uwj, uwd, endj, otx, sidx, lgx, arr;
    IR_t * vp1, * gt, * ot;
    int aext, aexth, aclass, acause, vrfp, hwhy, fcact, argv, curmov, dsave, redisp, opo, actoff, bspk, bxd;
    IR_t * cvn;
    int trapd, zrunb, xbp, zabs, zdef, zcut, suc, t0x, t0p, sdb, sdbb;
    int djo, gto, fcsa, seam, zpk, relo, dsa, dsb;
    int awlo, awb, apk, ak16, abk0, abdu, ach, asa, asb, dsusp;
    int ws, wek, wex, f0r, rel;
    int bu, sbk, sbb, sbp;
    int hrsp, dtk, dtph, dtd, f0f;
    int fres, xk, xw, xd, xa, sprec, gcell, tret, zsmown;
    int go0off, vp1off, cvnoff, pto, owno, vso, vp1vso, cft;
    int wkoff, wkn, wke, unr, adead;
    long zsmw;
};
enum { ZW_NONE = 0, ZW_NAFO = 1, ZW_BETA = 2, ZW_ALPHA = 3, ZW_GAMMA = 4, ZW_OMEGA = 5, ZW_REPALT = 6 };
struct zeta_plan_t {
    planner_road_t road;
    int scope, head;
    IR_t ** nodes;
    int n;
    unsigned char * on;
    int * out;
    int * gp;
    int * wp;
    int * arm;
    int * cps;
    int * cpd;
    int * chn;
    int * claim;
    int * vl;
    int * xb;
    int * cut;
    int * fam;
    int * fbase;
    int * b;
    unsigned char * kn;
    unsigned char * unr;
    unsigned char * adead;
    IR_t * entry;
    IR_graph_t * cfg;
    IR_t * key;
    int own_root;
    int zdp_on;
    int blob_bytes, spf, sph, spgd, casmark;
    int empty_fail;
    int empty_succ;
    IR_t ** nodes_own;
    int zst_n_saved;
    int zarm_published;
    int djx_nodes_published;
    unsigned int zrm_len, zrm_cap, zrm_esz, zrm_gen;
    std::vector<int> v_zst_s, v_zst_root, v_zst_vroot, v_zst_c, v_zst_j, v_zst_L, v_zst_vuse, v_zst_vcov, v_djx, v_upost, v_grec, v_wrec, v_grec_pop, v_wrec_pop;
    std::vector<const char *> v_zst_name;
    std::unordered_map<const IR_t *, int> v_zst_sm;
    std::unordered_map<const IR_t *, int> v_xoff;
    std::vector<unsigned char> zrm_bytes;
    std::vector<unsigned char> v_admit;
    int * home;
    unsigned char * zeta;
    int * lkey;
    int * alias;
    int * under;
    int * slid;
    int * pat;
    int * fc;
    int * tail;
    int * ret;
    int * grec_off;
    int * wrec_off;
    int * grec_res;
    int * wrec_res;
    int * cpa_s;
    int * cpa_d;
    int * cpa_sx;
    int * cpa_dx;
    int * rdo;
    zrd_t * rd;
    int rdn;
    znode_t * zn;
    std::vector<int> v_altpad;
    std::vector<int> v_opoff, v_partoff, v_opvs, v_partvs;
    int pso0;
    std::vector<int> v_dj;
    std::vector<int> v_gate;
    std::vector<unsigned char> v_lspine;
    std::vector<int> v_rel;
    std::vector<uint64_t> v_blob_lay;
    std::vector<uint64_t> v_wk;
};
enum { ZETA_NONE = 0, ZETA_SPINE = 1, ZETA_ACT = 2, ZETA_SPINE_ACT = 3, ZETA_PRE = 4 };
enum {
    PR_SUSPEND = 1, PR_COEXPR = 2, PR_UNKNOWN_CALLEE = 3, PR_GENERATOR_CALLEE = 4, PR_RETAINING_CALLEE = 5, PR_UNRESOLVED_CALLEE = 6, PR_DEFERRED_PATTERN = 7, PR_GATE_HOLDS = 8,
        PR_DISJUNCTION_HOLDS = 9, PR_DB_GENERATOR = 10, PR_RETAINING_TARGET = 11
};
void planner_capture(zeta_plan_t * rec);
void planner_restore(zeta_plan_t * rec);
zeta_plan_t * planner_chain_record(IR_graph_t * g, IR_t * entry, const char * prefix);
void zd_wk_fold(zeta_plan_t * rec, IR_t ** nodes, int n, const IR_graph_t * g);
void zd_wk_own(const IR_graph_t * cg, IR_t * nd, std::vector<char> & w);
void planner_pass_plan(IR_graph_t * g, IR_t * entry, const char * prefix);
void planner_table_clear(void);
void planner_table_forget(const IR_t * entry);
int planner_entry_planned(const IR_t * entry);
zeta_plan_t * planner_chain_plan(const IR_graph_t * cg, IR_t * entry, const char * prefix);
int zd_dj_admitted(int n, int ei);
int zd_dj_cellless(IR_t ** nodes, int n, const int * zarm, int k);
int zd_pl_dj_arm_holds(const IR_t * d, int a);
int zd_dj_arm_end(IR_t ** nodes, int n, int dj, int a);
IR_t * zd_scan_subject(IR_t * nd);
IR_t * zd_value_producer_g(const IR_graph_t * cg, IR_t * nd);
IR_t * zd_value_producer(IR_t * nd);
IR_t * zd_conj_value_node_g(const IR_graph_t * cg, IR_t * nd);
IR_t * zd_conj_value_node(IR_t * nd);
int zd_plan_on_of(const IR_t * nd);
int zd_sr_role(const IR_t * nd);
int zd_cpos(int i, const int *xb, const int *zout, long *d);
int zd_fam_delta(int i, int k, int * ok);
int zd_gate_pair_g(const IR_graph_t * cg, IR_t **nodes, int n, int a, int k, int wantb, const unsigned char *zon, const int *zout, const int *xb, long *rsp);
int zd_gate_pair(IR_t **nodes, int n, int a, int k, int wantb, const unsigned char *zon, const int *zout, const int *xb, long *rsp);
int zd_gate_pair_at_g(const IR_graph_t * cg, IR_t **nodes, int n, int g, int a, int k, int wantb, const unsigned char *zon, const int *zout, const int *xb, long *rsp);
int zd_gate_pair_at(IR_t **nodes, int n, int g, int a, int k, int wantb, const unsigned char *zon, const int *zout, const int *xb, long *rsp);
int zd_cl_read_g(const IR_graph_t * cg, IR_t **nodes, int n, int i, const unsigned char *on, const int *out, const int *xb);
int zd_cl_read(IR_t **nodes, int n, int i, const unsigned char *on, const int *out, const int *xb);
IR_t * zd_chase(IR_t * t);
int zd_yield_end(IR_t ** nodes, int n, int sp, const unsigned char * zon, const int * claim);
void zd_vdj_live(IR_t ** nodes, int n, int * live);
int zd_nops_g(const IR_graph_t * cg, IR_t * nd);
int zd_nops(IR_t * nd);
void zd_rec_arm(const IR_graph_t * cg, IR_t ** nodes, int i);
void zd_cp_arm(const IR_graph_t * cg, IR_t ** nodes, int i, const int * zcps, const int * zcpd, const int * zout);
int zd_icn_local_var_g(const IR_graph_t * cg, const IR_t * nd);
int zd_icn_local_var(const IR_t * nd);
int zd_icn_local_assign_g(const IR_graph_t * cg, const IR_t * nd);
int zd_icn_local_assign(const IR_t * nd);
const IR_t * zd_local_home_node(const IR_graph_t * cg, const IR_t * nd);
int zd_icn_suspend_g(const IR_graph_t * cg, const IR_t * nd);
int zd_icn_suspend(const IR_t * nd);
int zd_djx_of(const IR_t * nd);
int zd_upost_of(const IR_t * nd);
int zd_k_g(const IR_graph_t * cg, IR_t * nd);
int zd_kf_g(const IR_graph_t * cg, IR_t * nd);
int zd_mframe_g(const IR_graph_t * cg, IR_t * nd);
int zd_rel_g(const IR_graph_t * cg, IR_t * nd);
int planner_fence0_release(const IR_graph_t * cg, const IR_t * nd);
int zd_k(IR_t * nd);
int zd_call_curmov(const IR_t * nd);
int zd_k_own_g(const IR_graph_t * cg, IR_t * nd);
int zd_k_own(IR_t * nd);
int zd_omega_test_kind(const IR_t * nd);
int zd_arrival_depth_g(const IR_graph_t * cg, IR_t **nodes, int n, int i, const unsigned char *zon, const int *zout, const int *zgp, const int *zwp);
int zd_arrival_depth(IR_t **nodes, int n, int i, const unsigned char *zon, const int *zout, const int *zgp, const int *zwp);
int zd_edge_retains_g(const IR_graph_t * cg, const IR_t * nd);
int zd_edge_retains(const IR_t * nd);
#include "planner/frame_layout.h"
int planner_retains_g(const IR_graph_t * cg, const IR_t * nd);
int planner_retains(const IR_t * nd);
extern "C" int planner_env_encloses(const int * zarm, int i, int q);
int planner_retains_emitter_view(int op);
extern "C" int planner_home_dyn(const IR_graph_t * g, const IR_t * nd);
extern "C" int planner_bound_spine(const IR_graph_t * g, const IR_t * nd);
extern "C" int planner_span_retains(const IR_graph_t * g, int from_ai, int to_ai);
void zd_pl_bases(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, int *base, unsigned char *known, int *aof,
    int *zcut, int *fam, int *fbase, const unsigned char *adead);
int zst_s_of(const IR_t * nd);
void zst_finish(const IR_graph_t * cg, IR_t ** nodes, int n, const int * zout, const int * claim, const int * base, const unsigned char * known);
int zst_coord(const char * name);
void zd_plan(const IR_graph_t * cg, IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, int *zarm, const unsigned char *walls, int *zcps, int *zcpd, int *zchn, int *zclaim);
void zd_depth_census_g(const IR_graph_t * cg, IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, const char *prefix);
void zd_depth_census(IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, const char *prefix);
int zd_run_alpha_dead(const IR_graph_t * cg, IR_t ** nodes, int n, const int * claim, int h);
void zd_check_g(const IR_graph_t * cg, IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, const int *xb, const int *zchn,
    const int *zcut, const int *zarm, const char *prefix);
void zd_check(IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, const int *xb, const int *zchn, const int *zcut, const int *zarm,
    const char *prefix);
IR_graph_t * planner_published_graph(void);
static inline int planner_resumable_frame(const IR_graph_t * g) { return (g && zls_g_resumable_frame(g)) ? 1 : 0; }
#endif

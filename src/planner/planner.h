#include <vector>
#include <unordered_map>
struct zeta_plan_t {
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
    IR_t * entry;
    IR_graph_t * cfg;
    IR_t * key;
    int own_root;
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
    std::vector<unsigned char> zrm_bytes;
};
void planner_capture(zeta_plan_t * rec);
void planner_restore(zeta_plan_t * rec);
zeta_plan_t * planner_lookup_or_plan(IR_t * entry, const char * prefix);
void planner_pass_plan(IR_t * entry, const char * prefix);
void planner_table_clear(void);
void planner_table_forget(const IR_t * entry);
int planner_entry_planned(const IR_t * entry);
int planner_rt_plan(IR_t * entry, const char * prefix);
zeta_plan_t * planner_chain_plan(IR_t * entry, const char * prefix);
int zd_dj_admitted(const int * zarm, int n, int ei);
int zd_dj_cellless(IR_t ** nodes, int n, const int * zarm, int k);
int zd_pl_dj_arm_holds(const IR_t * d, int a);
int zd_dj_arm_end(IR_t ** nodes, int n, int dj, int a);
IR_t * zd_scan_subject(IR_t * nd);
IR_t * zd_value_producer(IR_t * nd);
IR_t * zd_conj_value_node(IR_t * nd);
int zd_plan_on_of(const IR_t * nd);
void zd_ops_reserve(int n);
int zd_sr_role(const IR_t * nd);
int zd_cpos(int i, const int *xb, const int *zout, long *d);
int zd_gate_pair(IR_t **nodes, int n, int a, int k, int wantb, const unsigned char *zon, const int *zout, const int *xb, long *rsp);
int zd_gate_pair_at(IR_t **nodes, int n, int g, int a, int k, int wantb, const unsigned char *zon, const int *zout, const int *xb, long *rsp);
int zd_cl_read(IR_t **nodes, int n, int i, const unsigned char *on, const int *out, const int *xb);
IR_t * zd_chase(IR_t * t);
void zd_vdj_live(IR_t ** nodes, int n, int * live);
int zd_nops(IR_t * nd);
void zd_rec_arm(IR_t ** nodes, int i);
void zd_cp_arm(IR_t ** nodes, int i, const int * zcps, const int * zcpd, const int * zout);
int zd_icn_local_var(const IR_t * nd);
int zd_icn_local_assign(const IR_t * nd);
const IR_t * zd_local_home_node(const IR_t * nd);
int zd_icn_suspend(const IR_t * nd);
int zd_djx_of(const IR_t * nd);
int zd_upost_of(const IR_t * nd);
int zd_k(IR_t * nd);
int zd_call_curmov(const IR_t * nd);
int zd_k_own(IR_t * nd);
int zd_omega_test_kind(const IR_t * nd);
int zd_arrival_depth(IR_t **nodes, int n, int i, const unsigned char *zon, const int *zout, const int *zgp, const int *zwp);
int zd_edge_retains(const IR_t * nd);
enum {
    PR_SUSPEND = 1, PR_COEXPR = 2, PR_UNKNOWN_CALLEE = 3, PR_GENERATOR_CALLEE = 4, PR_RETAINING_CALLEE = 5, PR_UNRESOLVED_CALLEE = 6, PR_DEFERRED_PATTERN = 7, PR_GATE_HOLDS = 8,
        PR_DISJUNCTION_HOLDS = 9, PR_DB_GENERATOR = 10, PR_RETAINING_TARGET = 11
};
extern "C" {
int zls_fcn_hret(const IR_t * nd);
int zls_fcn_hunres(const IR_t * nd);
}
#include "../ir/frame_layout.h"
int planner_retains(const IR_t * nd);
int planner_retains_emitter_view(int op);
extern "C" int planner_home_dyn(const IR_graph_t * g, const IR_t * nd);
void zd_pl_bases(IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, int *base, unsigned char *known, int *aof, int *zcut, int *fam,
    int *fbase);
int zst_s_of(const IR_t * nd);
void zst_finish(IR_t ** nodes, int n, const int * zout, const int * claim, const int * base, const unsigned char * known);
int zst_coord(const char * name);
void zd_plan(IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, int *zarm, const unsigned char *walls, int *zcps, int *zcpd, int *zchn, int *zclaim);
void zd_depth_census(IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, const char *prefix);
int zd_run_alpha_dead(IR_t ** nodes, int n, const int * claim, int h);
void zd_check(IR_t **nodes, int n, const unsigned char *zon, const int *zout, const int *zgpop, const int *zwpop, const int *claim, const int *xb, const int *zchn, const int *zcut, const char *prefix)
    ;

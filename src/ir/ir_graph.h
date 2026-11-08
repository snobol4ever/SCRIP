#ifndef SCRIP_IR_GRAPH_H
#define SCRIP_IR_GRAPH_H
#include <stdio.h>
#include <stdint.h>
#include "ir/descr.h"
#include "ir/ir_fwd.h"
#ifdef __cplusplus
extern "C" {
#endif
struct IR_graph_t {
    IR_t * entry;
    IR_t ** all;
    int n;
    int max;
    int nslots;
    int nvalue_slots;
    int jcon_value_region;
    int nparams;
    const char ** pnames;
    int nlocals;
    const char ** lnames;
    struct IR_vslot_t { const char * name; int off; } * vslots;
    int n_vslots;
    int * vslot_ix;
    int vslot_ix_cap;
    const char ** lname_ix;
    int lname_ix_cap, lname_ix_np, lname_ix_nl;
    const char ** lname_ix_pn;
    const char ** lname_ix_ln;
    int resume_slot;
    int resumable_callable;
    int depth_counted;
    int dl_pair;
    int dl_latent;
    int keeps_choice;
    int rt_redefinable;
    int decl_level;
    int caller_frame;
    int static_calls;
    int poll_alpha;
    int trace_taps;
    int multi_proto;
    const char * l3_ancestor_name;
    int deterministic;
    int zeta_mark_slot;
    IR_t * body_root;
    IR_t ** balias_node;
    const char ** balias_name;
    int n_balias;
    IR_t ** dentry_node;
    IR_t ** dentry_entry;
    const char ** dentry_name;
    int n_dentry;
    IR_t ** alt_entry;
    IR_t ** alt_ret;
    IR_t ** alt_redo;
    int * alt_zhi;
    int alt_zlo;
    IR_t * alt_fail;
    int n_alts;
#define AG_RING 16
    DESCR_t ring[AG_RING];
    int ring_head;
    int ring_depth;
    int zframe_graph;
    int entry_frame;
    int smx;
    int cell_frame;
    int scan_regs;
    int spine_cells;
    int home_frame;
    int root_graph;
    int standing_cells;
    int unbound_serial;
    int runtime_fragment_graph;
    int is_variadic;
    int rest_kind;
    IR_t ** dead;
    int n_dead;
    int block_args;
    int pkt_fragment;
    int pkt_cell;
    int pkt_slot;
    IR_construct_t * constructs;
    int n_constructs;
    int max_constructs;
    int serial;
    struct IR_plan_facts_t {
        int resumable_frame;
        int block_args;
        int det_block;
        int home_frame;
        int generator_top;
        int block_base;
        int locals;
        int value_region;
        int body_seam;
        int keys_only;
        int kbase;
        int retains;
        int frame_bytes;
        int fp_total;
        int flat_gen_rbp;
        int zframe;
        uint64_t * lay;
        int lay_n;
        int lay_gaps;
        int lay_conf;
        unsigned lay_flags;
        const char * name;
        int choice_n;
        int choice_leaf;
        int choice_fence;
        int region;
        int resume;
        int zeta_mark;
        int cap_min;
        const unsigned char * kcls;
        int kcls_n;
        int frame_region;
        int resident;
        int zstatic;
        int uniform;
        int callee_fp;
        int gen_ft;
    } pf;
};
int ir_graph_has_suspend(const IR_graph_t * g);
IR_graph_t * IR_alloc(int max_nodes);
uint32_t ir_name_hash(const char * s);
int ir_varslot_of(const IR_graph_t * g, const char * name);
int graph_has_local(const IR_graph_t * g, const char * name);
void IR_free(IR_graph_t * cfg);
void bb_print(const IR_graph_t * cfg, FILE * fp);
void bb_print_v(const IR_graph_t * cfg, FILE * fp, int verbose);
#ifdef __cplusplus
}
#endif
#endif

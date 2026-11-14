#ifndef EMITTER_EM_EM_HEAD_H
#define EMITTER_EM_EM_HEAD_H
#include <unordered_map>
#include "ir/ct_vec.h"
#include "runtime/rt/prolog_atom.h"
#include <string>
#include <climits>
#include "emitter/emit.h"
#include "planner/frame_layout.h"
#include "ir/gc_frame_map.h"
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "emitter/emit_state_em_03_sh_api.h"
#include "ir/ir_graph.h"
extern "C" {
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "emitter/g_emitter.h"
#include "emitter/bb_label.h"
#include "ir/dtp.h"
#include "templates/bb/bb_templates.h"
#include "driver/scrip_sm.h"
#include "templates/xa/xa_templates.h"
#include "planner/g_planner.h"
#include "ir/ir_fwd.h"
#include "runtime/core/g_runtime_entry.h"
#include "runtime/core/g_runtime_vec.h"
#include "runtime/core/g_runtime_abi.h"
#include "templates/x86/x86_call.h"
#include "runtime/rt/pl_trail_layout.h"
#define STNO_REC(i) CV_AT(g_emit.stno_map, sno_stno_rec_t, (i))
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { bb_label_t * k; int head; uint32_t gen; } bb_patch_hent_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define STNO_SRC(i) CV_AT(g_emit.stno_src, sno_stno_src_t, (i))
#define STRTAB(i) CV_AT(g_emitter.g_strtab, strtab_ent_t, (i))
#define CSETTAB(i) CV_AT(g_emitter.g_csettab, csettab_ent_t, (i))
#include "runtime/rt/rt.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#define FLAT_BUF_MAX (flat_buf_max_v())
extern "C" int zls_g_fp_total(IR_graph_t *);
extern "C" int zls_node_has_fields(const IR_t *);
extern "C" int zls_g_block_args(const IR_graph_t *); extern "C" int zls_g_det_block(const IR_graph_t *); extern "C" int zls_g_resumable_frame(const IR_graph_t *);
extern "C" int fc_tail_defer_susp_g(IR_graph_t *, const IR_t *);
extern "C" int fc_tail_dfr(const IR_t *);
extern "C" int sno_name_prologue_bound(const char *);
#define FLAT_DATA_TEXT ((char *)g_flat_data_buf.p)
static void flat_emit_arg_subchain(IR_t *entry, bb_label_t *succ, bb_label_t *fail);
static void gvar_stmt_operand_refs(IR_t *head);
static int gvar_prewalk_idx_operand(IR_t *idx, bb_label_t *lbl_ω);
extern int memcmp(const void *, const void *, size_t);
#define FILL(nd,s,f,b) do { \
    bb_fill_alpha(nd); \
    g_emit.lbl_γ=(s)->name; g_emit.lbl_ω=(f)->name; g_emit.lbl_β=(b)->name; \
    g_emit.lbl_γ_p=(s); g_emit.lbl_ω_p=(f); g_emit.lbl_β_p=(b); \
    walk_bb_node((nd), emit_outf()); } while(0)
static int is_pat_chain_elem(IR_e t);
static void emit_cat_diamond(IR_t *entry, IR_t *J, bb_label_t *fwd, bb_label_t *fail, bb_label_t *arm_beta, int id, int armidx);
static int is_pat_chain_elem(IR_e t);
static int gather_lowered_cat_arms(IR_t *entry, IR_t **arms, IR_t **joins, int cap, IR_t **cat_out, IR_t *stop);
static void gz_emit_cell(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int fc_pair_extent(const IR_t *);
extern "C" int zls_dj_arm_resumes(const IR_t * nd, int a);
extern "C" int zls_dj_rbp_arms(const IR_t * nd);
extern "C" int zls_dj_retains(const IR_t * nd);
extern "C" int zls_frame_word_why(const IR_t * nd);
extern "C" int zls_home_local_why(const IR_t * nd);
extern "C" int zls_g_home_frame(const IR_graph_t * g);
extern "C" int zls_frame_home(const IR_t * nd);
extern "C" int zls_g_value_region(const IR_graph_t * g); extern "C" int zls_g_root_value_bytes(const IR_graph_t * g);
extern "C" int zls_key_unbacked(const IR_graph_t * g, int off);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arbno_body_slot_window(const IR_t * nd, int * out_lo, int * out_bytes);
extern "C" int zls_off(const IR_t *);
extern "C" int zls_frame_off(const IR_graph_t *, const IR_t *);
extern "C" int icn_builtin_is_known(const char *);
extern "C" int icn_builtin_is_generator(const char *);
extern "C" int rt_proc_entry_pending(const char *);
extern "C++" void * dop_direct_fp(const char * fn, int64_t narg, const char ** sym);
extern "C" int dop_direct_leaf_known(const char * fn, int narg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RK_BOOL_ROUTE(a0) (((a0) && (a0)->op == IR_BINOP && IR_LIT(a0).ival >= BINOP_LT && IR_LIT(a0).ival <= BINOP_NE) ? CALL_ROUTE_RK_BOOL_RELOP : CALL_ROUTE_RK_BOOL_SLOT)
extern "C++" int bcps_arg_slot(IR_t * call, int i);
extern "C" int fc_geom(const IR_t *, long *);
extern "C" int fc_leaf_cell(const IR_t *, long *);
extern "C" int fc_cond_fp(const IR_t *);
extern "C" int fc_head_fp(const IR_t *);
extern "C" int fc_leaf_disp(const IR_t *);
extern "C" int fc_seq_active(const IR_t *);
extern "C" int fc_call_active(const IR_t *);
extern "C" int fc_vdj_active(const IR_t *);
extern "C" int zls_arbno_geom(const IR_t *, int *, int *);
extern "C" int zls_arbno_zq(const IR_t *, const int **);
extern "C" int bbprof_on(void);
extern "C" void bbprof_record(int nid, int kind, int uid, void *lo, void *hi);
extern int zls_result_live(const IR_t *nd);
#define DRIVE_FILL(nd,a,s,f,b) do { \
    g_emit.op_selfload = 0; \
g_emit.op_fc_bytes = 0; g_emit.op_fc_base = -1; g_emit.x86_fc_synth = X86_INTERNAL_MAX - 1; g_emit.op_fc_fpmax = -1; g_emit.op_fc_disp = -1; g_emit.op_fc_wbytes = 0; g_emit.op_arbno_chain = 0; \
    g_emit.op_arbno_nzq = \
    0; g_emit.op_tail = 0; g_emit.op_tail_fpb = 0; g_emit.op_tail_fpl = 0; g_emit.op_tail_seal = 0; g_emit.op_tail_ncap = 0; g_emit.op_arbno_dt = 0; g_emit.op_arbno_dt_susp = 0; g_emit. \
    op_defer_leaf_susp = -1; g_emit.op_tail_dfr = 0; g_emit.op_tail_fpr_rsp = 0; g_emit.op_body_has_arbno = 0; g_emit.op_arbno_framed = 0; g_emit.op_arbno_body_k0 = 0; g_emit.op_arbno_body_kk = 0; \
g_emit.op_fence_body_kk = 0; g_emit.op_fence_frame_off = -1; g_emit.op_arbno_win_lo = 0; g_emit.op_arbno_win_bytes = 0; g_emit.op_alt_M = -1; g_emit.op_alt_pad = (int *)0; g_emit.op_fence0_floor = 0 \
    ; g_emit.op_arbno_body_defer_unsafe = 0; g_emit.op_arbno_body_actframe = 0; \
    g_emit.op_arbno_body_fence = 0; g_emit.op_alt_cell = 0; g_emit.op_arbno_rbp = 0; g_emit.op_range_int_operands = 0; \
     \
    bb_label_t *_fs__=bb_label_fold((s)), *_ff__=bb_label_fold((f)); \
    g_emit.lbl_α=(a)->name; g_emit.lbl_γ=_fs__->name; g_emit.lbl_ω=_ff__->name; g_emit.lbl_β=(b)->name; \
    g_emit.lbl_α_p=(a); g_emit.lbl_γ_p=_fs__; g_emit.lbl_ω_p=_ff__; g_emit.lbl_β_p=(b); \
    walk_bb_node((nd), emit_outf()); } while(0)
#define DRIVE_PAIR_RESET() do { g_emit.xa_bb_emit_pair_n = 0; } while(0)
#define DRIVE_PAIR_JMP(tgt) do { xa_pair_push(NULL, (tgt)); } while(0)
#define DRIVE_PAIR_DEF_JMP(l,t) do { xa_pair_push((l), (t)); } while(0)
extern "C" void * rt_proc_get_fn(const char * name);
extern "C" int fc_alt_fpmax(const IR_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int fc_seq_active(const IR_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" const char * bb_ab_sym_name(const char * nm);
extern "C" const char * bb_ab_thunk_stem_or(const char * nm);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(bb_ab_seal_alpha, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zls_g_region(const IR_graph_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zls_g_vslot_count(const IR_graph_t *); extern "C" const char * zls_g_vslot_get(const IR_graph_t *, int, int *); extern "C" int zls_g_block_base(const IR_graph_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int xa_flat_class_c_pred(void);
extern "C" void xa_flat_chain_prologue(const char * fname, int * argkt_out);
extern "C" void xa_flat_chain_epilogue(void);
extern "C" void xa_flat_chain_epilogue_sig(int is_gamma, const char * fname);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int rt_gva_add_var(const char * name);
extern "C" void rt_gc_frame_maps_add(const gc_frame_map_t *m); extern "C" void rt_gc_frame_sites_add(const void *tab);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zls_g_resumable_frame(const IR_graph_t * g);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "runtime/rt_entry_table.inc"
struct rtx_ssr_t { const char * s; long n; };
struct rt_dcap_next_t { long fn; long how; };
#define RTX_TWIN(kind, ret, leaf, twin, args) extern "C" ret twin args;
#include "templates/x86/rtx_twin_table.inc"
#undef RTX_TWIN
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" { int stage2_owner_varslot(const char * proc, const char * var); const char * stage2_owner_l3_ancestor(const char * proc); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int prolog_atom_intern_lit(const char *);
extern "C" int prolog_functor_arity(int);
extern "C" {
int rt_dat_field_of_any(const char * name);
void * dat_find_type(const char * name);
}
extern "C" { int stage2_owner_varslot(const char *, const char *); int bb_scc_probe(const char *, int, int *, int *, int *, int *); int rt_proc_nformals(const char *); extern int g_flat_frame_floor; }
extern "C" {
DESCR_t rt_pl_throw_raise(DESCR_t *, int); DESCR_t rt_pl_exist_raise(DESCR_t *, int); DESCR_t rt_pl_catch_handle(DESCR_t *, int); DESCR_t rt_pl_dop_clause_unify(DESCR_t *,
    int); DESCR_t dop_write(DESCR_t *, int); DESCR_t dop_nl(DESCR_t *, int); DESCR_t rt_fp_model_spitbol(DESCR_t *, int); DESCR_t rt_sno_wantnm_d(DESCR_t *,
        int); DESCR_t rt_sno_chknm_d(DESCR_t *, int); DESCR_t rt_sno_patck_d(DESCR_t *, int); DESCR_t rt_sno_mkpat_d(DESCR_t *, int);
DESCR_t rt_sno_name_dl(DESCR_t *, int); DESCR_t rt_sno_pbk_d(DESCR_t *, int); DESCR_t rt_sno_pbn_d(DESCR_t *, int); DESCR_t rt_sno_pb0_d(DESCR_t *, int); DESCR_t rt_sno_pbc_d(DESCR_t *,
    int); DESCR_t rt_sno_pcur_d(DESCR_t *, int); DESCR_t rt_sno_pbalt_d(DESCR_t *, int); DESCR_t rt_sno_parb_d(DESCR_t *, int); DESCR_t rt_sno_pfen_d(DESCR_t *,
        int); DESCR_t rt_sno_pdef_d(DESCR_t *, int); DESCR_t rt_sno_kwset_d(DESCR_t *, int); DESCR_t rt_sno_mkexpr_d(DESCR_t *, int); DESCR_t rt_sno_xname_d(DESCR_t *, int);
            DESCR_t rt_sno_stmt_d(DESCR_t *,
            int); DESCR_t rt_sno_nofail_d(DESCR_t *, int); DESCR_t rt_sno_list_d(DESCR_t *, int);
DESCR_t rt_pl_dop_skip_list(DESCR_t *, int); DESCR_t rt_pl_dop_seek_list(DESCR_t *, int); DESCR_t rt_pl_dop_argv(DESCR_t *, int);
DESCR_t rt_pl_dop_compare(DESCR_t *, int); DESCR_t rt_pl_dop_functor(DESCR_t *, int); DESCR_t rt_pl_dop_arg(DESCR_t *, int); DESCR_t rt_pl_dop_univ(DESCR_t *, int);
DESCR_t rt_pl_dop_attv_set(DESCR_t *, int); DESCR_t rt_pl_dop_setarg(DESCR_t *, int); DESCR_t rt_pl_dop_nb_setarg(DESCR_t *, int); DESCR_t rt_pl_dop_nb_linkarg(DESCR_t *, int);
DESCR_t rt_pl_dop_stamp_date_time(DESCR_t *, int); DESCR_t rt_pl_dop_date_time_stamp(DESCR_t *, int); DESCR_t rt_pl_dop_format_time_text(DESCR_t *, int);
    DESCR_t rt_pl_dop_attv_atts(DESCR_t *, int); DESCR_t rt_pl_dop_attv_take(DESCR_t *, int); DESCR_t rt_pl_dop_unifiable(DESCR_t *, int);
    DESCR_t rt_pl_dop_crv_enter(DESCR_t *, int); DESCR_t rt_pl_dop_crv_adj(DESCR_t *, int); DESCR_t rt_pl_dop_crv_collect(DESCR_t *, int); DESCR_t rt_pl_dop_undo_push(DESCR_t *, int); DESCR_t rt_pl_dop_undo_take(DESCR_t *, int);
    DESCR_t rt_pl_dop_copy_keep(DESCR_t *, int); DESCR_t rt_pl_dop_factorize_term(DESCR_t *, int);
DESCR_t rt_pl_dop_scc_bnew(DESCR_t *, int); DESCR_t rt_pl_dop_scc_take(DESCR_t *, int); DESCR_t rt_pl_dop_copy_term(DESCR_t *, int); DESCR_t rt_pl_dop_copy_term_nat(DESCR_t *, int);
    DESCR_t rt_pl_dop_numbervars3(DESCR_t *, int); DESCR_t rt_pl_dop_numbervars1(DESCR_t *,
    int); DESCR_t rt_pl_dop_succ(DESCR_t *, int);
DESCR_t rt_pl_dop_wall_us(DESCR_t *, int); DESCR_t rt_pl_dop_wall_ms(DESCR_t *, int);
DESCR_t rt_pl_dop_plus(DESCR_t *, int); DESCR_t rt_pl_dop_sort(DESCR_t *, int); DESCR_t rt_pl_dop_msort(DESCR_t *, int); DESCR_t rt_pl_dop_char_type(DESCR_t *, int);
DESCR_t rt_pl_dop_findall_new(DESCR_t *, int); DESCR_t rt_pl_dop_findall_add(DESCR_t *, int); DESCR_t rt_pl_dop_findall_result(DESCR_t *, int); DESCR_t rt_pl_dop_findall_result4(DESCR_t *,
    int); DESCR_t rt_pl_dop_bagof_result(DESCR_t *, int); DESCR_t rt_pl_dop_setof_result(DESCR_t *, int);
DESCR_t rt_pl_dop_sub_atom_n(DESCR_t *, int); DESCR_t rt_pl_dop_sub_atom_at(DESCR_t *, int); DESCR_t rt_pl_dop_atom_concat_n(DESCR_t *, int); DESCR_t rt_pl_dop_atom_concat_at(DESCR_t *,
    int); DESCR_t rt_pl_dop_bagof_group_n(DESCR_t *, int); DESCR_t rt_pl_dop_bagof_group_at(DESCR_t *, int); DESCR_t rt_pl_dop_setof_group_at(DESCR_t *, int);
DESCR_t rt_pl_dop_big(DESCR_t *, int); DESCR_t rt_pl_dop_pl_engine(DESCR_t *, int); DESCR_t rt_pl_dop_db_assertz(DESCR_t *, int); DESCR_t rt_pl_dop_db_asserta(DESCR_t *, int);
    DESCR_t rt_pl_dop_db_erase(DESCR_t *,
    int); DESCR_t rt_pl_dop_db_abolish(DESCR_t *, int); DESCR_t rt_pl_dop_db_nonempty(DESCR_t *, int); DESCR_t rt_pl_dop_ball_pending(DESCR_t *, int); DESCR_t rt_pl_dop_format(DESCR_t *,
        int); DESCR_t rt_pl_dop_db_seed_once(DESCR_t *, int);
DESCR_t rt_pl_dop_nb_setval(DESCR_t *, int); DESCR_t rt_pl_dop_nb_getval(DESCR_t *, int); DESCR_t rt_pl_dop_b_setval(DESCR_t *, int); DESCR_t rt_pl_dop_nb_delete(DESCR_t *, int);
DESCR_t rt_pl_dop_trie_new(DESCR_t *, int); DESCR_t rt_pl_dop_trie_alive(DESCR_t *, int); DESCR_t rt_pl_dop_trie_destroy(DESCR_t *, int); DESCR_t rt_pl_dop_trie_insert(DESCR_t *, int);
DESCR_t rt_pl_dop_trie_lookup(DESCR_t *, int); DESCR_t rt_pl_dop_trie_delete(DESCR_t *, int); DESCR_t rt_pl_dop_trie_entries(DESCR_t *, int); DESCR_t rt_pl_dop_trie_entry_key(DESCR_t *, int);
DESCR_t rt_pl_dop_trie_count(DESCR_t *, int); DESCR_t rt_pl_dop_trie_meta_set(DESCR_t *, int); DESCR_t rt_pl_dop_trie_meta_get(DESCR_t *, int); DESCR_t rt_pl_dop_tbl_vt(DESCR_t *, int);
DESCR_t rt_pl_dop_ssu_begin(DESCR_t *, int); DESCR_t rt_pl_dop_ssu_check(DESCR_t *, int);
DESCR_t rt_pl_dop_tbh_close(DESCR_t *, int); DESCR_t rt_pl_dop_tbl_park(DESCR_t *, int); DESCR_t rt_pl_dop_tbl_sl(DESCR_t *, int); DESCR_t rt_pl_dop_tbl_resume_all(DESCR_t *, int);
    DESCR_t rt_pl_dop_tbh_get(DESCR_t *, int); DESCR_t rt_pl_dop_tbh_own(DESCR_t *, int);
DESCR_t rt_pl_dop_tbh_set(DESCR_t *, int); DESCR_t rt_pl_dop_tbh_bset(DESCR_t *, int); DESCR_t rt_pl_dop_tbl_dlget(DESCR_t *, int); DESCR_t rt_pl_dop_tbl_dlset(DESCR_t *, int);
    DESCR_t rt_pl_dop_tx_db(DESCR_t *, int); DESCR_t rt_pl_dop_mod_ext(DESCR_t *, int);
DESCR_t rt_pl_dop_db_t_guard(DESCR_t *, int); DESCR_t rt_pl_dop_db_assertz_t(DESCR_t *, int); DESCR_t rt_pl_dop_db_asserta_t(DESCR_t *, int); DESCR_t rt_pl_dop_db_consult_t(DESCR_t *, int);
    DESCR_t rt_pl_dop_db_consult_done(DESCR_t *, int); DESCR_t rt_pl_dop_db_erase_t(DESCR_t *,
    int); DESCR_t rt_pl_dop_db_abolish_t(DESCR_t *, int); DESCR_t rt_pl_dop_db_retractall_t(DESCR_t *, int); DESCR_t rt_pl_dop_db_asserta_r(DESCR_t *,
        int); DESCR_t rt_pl_dop_db_assertz_r(DESCR_t *, int); DESCR_t rt_pl_dop_db_erase_ref(DESCR_t *, int); DESCR_t rt_pl_dop_db_n_r(DESCR_t *, int); DESCR_t rt_pl_dop_db_at_r(DESCR_t *,
            int); DESCR_t rt_pl_dop_db_ref_r(DESCR_t *, int);
DESCR_t rt_pl_dop_co_start(DESCR_t *, int); DESCR_t rt_pl_dop_co_resume(DESCR_t *, int); DESCR_t rt_pl_dop_co_yield(DESCR_t *, int); DESCR_t rt_pl_dop_co_unwound(DESCR_t *, int);
DESCR_t rt_pl_dop_co_finish(DESCR_t *, int); DESCR_t rt_pl_dop_co_mark(DESCR_t *, int); DESCR_t rt_pl_dop_co_b0(DESCR_t *, int); DESCR_t rt_pl_dop_co_inside(DESCR_t *, int);
DESCR_t rt_pl_dop_engine_new(DESCR_t *, int); DESCR_t rt_pl_dop_engine_next(DESCR_t *, int); DESCR_t rt_pl_dop_engine_yield(DESCR_t *, int); DESCR_t rt_pl_dop_engine_destroy(DESCR_t *, int);
DESCR_t rt_pl_dop_engine_self(DESCR_t *, int); DESCR_t rt_pl_dop_engine_id(DESCR_t *, int); DESCR_t rt_pl_dop_engine_list(DESCR_t *, int); DESCR_t rt_pl_dop_engine_reaped(DESCR_t *, int);
    DESCR_t rt_pl_dop_engine_stats(DESCR_t *, int); DESCR_t rt_pl_dop_gc_now(DESCR_t *, int);
DESCR_t rt_pl_dop_db_store_k(DESCR_t *, int); DESCR_t rt_pl_dop_db_copy(DESCR_t *, int); DESCR_t rt_pl_dop_db_data_unify(DESCR_t *, int); DESCR_t rt_pl_dop_dl_push(DESCR_t *, int);
    DESCR_t rt_pl_dop_dl_reached(DESCR_t *, int); DESCR_t rt_pl_dop_dl_false(DESCR_t *, int); DESCR_t rt_pl_dop_dl_reset(DESCR_t *, int); DESCR_t rt_pl_dop_dl_tick(DESCR_t *, int);
    DESCR_t rt_pl_dop_pl_cp_count(DESCR_t *, int); DESCR_t rt_pl_dop_mod_get(DESCR_t *, int); DESCR_t rt_pl_dop_mod_set(DESCR_t *, int);
    DESCR_t rt_pl_dop_pl_cp_nth(DESCR_t *,
    int); DESCR_t rt_pl_dop_pl_cp_guard(DESCR_t *, int); DESCR_t rt_pl_dop_pl_pp_guard(DESCR_t *, int); DESCR_t rt_pl_dop_pl_pp_count(DESCR_t *, int); DESCR_t rt_pl_dop_pl_pp_nth(DESCR_t *,
        int); DESCR_t rt_pl_dop_halt(DESCR_t *, int);
DESCR_t pas_arr_get(DESCR_t *, int); DESCR_t pas_arr_set(DESCR_t *, int); DESCR_t pas_arr_make(DESCR_t *, int); DESCR_t pas_arr_make_raw(DESCR_t *, int); DESCR_t pas_ptr_diff(DESCR_t *, int); DESCR_t pas_pdx(DESCR_t *, int);
    DESCR_t pas_row_get(DESCR_t *, int); DESCR_t pas_row_set(DESCR_t *, int);
    DESCR_t pas_pstrlen(DESCR_t *, int); DESCR_t pas_pstr(DESCR_t *, int); DESCR_t pas_addr(DESCR_t *, int); DESCR_t pas_addr_var(DESCR_t *, int);
    DESCR_t pas_ptr_inc(DESCR_t *, int); DESCR_t pas_ptr_eq(DESCR_t *, int); DESCR_t pas_deref(DESCR_t *, int); DESCR_t pas_deref_set(DESCR_t *, int);
    DESCR_t pas_field_set(DESCR_t *, int); DESCR_t pas_alloc_rec(DESCR_t *, int); DESCR_t pas_dispose(DESCR_t *, int); DESCR_t rt_iand_d(DESCR_t *, int); DESCR_t rt_ior_d(DESCR_t *, int);
        DESCR_t rt_ixor_d(DESCR_t *, int); DESCR_t rt_ishift_d(DESCR_t *, int);
DESCR_t pas_elem_ref(DESCR_t *, int); DESCR_t pas_ref_val(DESCR_t *, int);
DESCR_t rt_quit_trap_300(DESCR_t *, int); DESCR_t rt_quit_trap_320(DESCR_t *, int);
DESCR_t rt_pl_dop_char_guard(DESCR_t *, int); DESCR_t rt_pl_dop_between_guard(DESCR_t *, int); DESCR_t rt_pl_dop_stream_guard(DESCR_t *, int); DESCR_t rt_pl_dop_curstream_guard(DESCR_t *, int);
DESCR_t rt_pl_dop_goal_guard(DESCR_t *, int); DESCR_t rt_pl_dop_list_guard(DESCR_t *, int); DESCR_t rt_pl_dop_pl_declared(DESCR_t *, int); DESCR_t rt_pl_dop_pl_dynamic(DESCR_t *, int);
DESCR_t rt_pl_dop_term_string(DESCR_t *, int); DESCR_t rt_pl_dop_term_to_atom(DESCR_t *, int);
DESCR_t rt_pl_dop_atom_length(DESCR_t *, int); DESCR_t rt_pl_dop_atom_concat(DESCR_t *, int); DESCR_t rt_pl_dop_atomic_concat(DESCR_t *,
    int); DESCR_t rt_pl_dop_atom_chars(DESCR_t *, int);
DESCR_t rt_pl_dop_atom_codes(DESCR_t *, int); DESCR_t rt_pl_dop_atom_number(DESCR_t *, int); DESCR_t rt_pl_dop_atom_string(DESCR_t *, int); DESCR_t rt_pl_dop_upcase_atom(DESCR_t *, int);
DESCR_t rt_pl_dop_downcase_atom(DESCR_t *, int); DESCR_t rt_pl_dop_string_concat(DESCR_t *, int); DESCR_t rt_pl_dop_string_length(DESCR_t *, int); DESCR_t rt_pl_dop_string_lower(DESCR_t *, int);
DESCR_t rt_pl_dop_string_upper(DESCR_t *, int); DESCR_t rt_pl_dop_string_to_atom(DESCR_t *, int); DESCR_t rt_pl_dop_string_codes(DESCR_t *, int); DESCR_t rt_pl_dop_string_chars(DESCR_t *,
    int); DESCR_t rt_pl_dop_number_string(DESCR_t *, int);
DESCR_t rt_pl_dop_atomic_list_concat(DESCR_t *, int); DESCR_t rt_pl_dop_concat_atom(DESCR_t *, int); DESCR_t rt_pl_dop_char_code(DESCR_t *, int); DESCR_t rt_pl_dop_number_codes(DESCR_t *, int);
DESCR_t rt_pl_dop_gnu_sort1(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_msort1(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_keysort1(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_line_count(DESCR_t *, int);
DESCR_t rt_pl_dop_gnu_line_position(DESCR_t *, int); DESCR_t rt_pl_dop_put_token(DESCR_t *, int); DESCR_t rt_pl_dop_sha_hash(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_character_count(DESCR_t *, int);
DESCR_t rt_pl_dop_gnu_last_read_start(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_absolute_file_name(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_prolog_file_name(DESCR_t *, int);
DESCR_t rt_pl_dop_gnu_working_directory(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_change_directory(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_make_directory(DESCR_t *, int);
DESCR_t rt_pl_dop_gnu_delete_file(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_file_exists(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_directory_files(DESCR_t *, int);
DESCR_t rt_pl_dop_gnu_environ_list(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_file_props(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_term_hash(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_term_hash4(DESCR_t *, int);
DESCR_t rt_pl_dop_pipe(DESCR_t *, int); DESCR_t rt_pl_dop_open_null_stream(DESCR_t *, int);
DESCR_t rt_pl_dop_number_chars(DESCR_t *, int); DESCR_t rt_pl_dop_name(DESCR_t *, int); DESCR_t rt_pl_dop_get_char(DESCR_t *, int); DESCR_t rt_pl_dop_peek_char(DESCR_t *, int);
DESCR_t rt_pl_dop_read(DESCR_t *, int); DESCR_t rt_pl_dop_atom_to_term(DESCR_t *, int); DESCR_t rt_pl_dop_read_term_from_atom(DESCR_t *,
    int); DESCR_t rt_pl_dop_read_term_from_chars(DESCR_t *, int); DESCR_t rt_pl_dop_read_term_from_codes(DESCR_t *, int);DESCR_t rt_pl_dop_display(DESCR_t *,
        int); DESCR_t rt_pl_dop_display_s(DESCR_t *, int); DESCR_t rt_pl_dop_unify_oc(DESCR_t *, int); DESCR_t rt_pl_dop_aggregate_reduce(DESCR_t *, int);
DESCR_t rt_pl_dop_read_term_opts(DESCR_t *, int); DESCR_t rt_pl_dop_read_term_opts_s(DESCR_t *, int);
DESCR_t rt_pl_dop_wot_open(DESCR_t *, int); DESCR_t rt_pl_dop_wot_capture(DESCR_t *, int); DESCR_t rt_pl_dop_wot_discard(DESCR_t *, int);
DESCR_t dop_pl_ground(DESCR_t *, int); DESCR_t dop_pl_is_list(DESCR_t *, int); DESCR_t dop_pl_acyclic_term(DESCR_t *, int); DESCR_t dop_pl_between_special(DESCR_t *, int);
DESCR_t dop_pl_writeq(DESCR_t *, int); DESCR_t dop_pl_write_canonical(DESCR_t *, int);
DESCR_t dop_pl_writeln(DESCR_t *, int); DESCR_t dop_pl_tab(DESCR_t *, int); DESCR_t dop_pl_put_char(DESCR_t *, int);
DESCR_t dop_pl_flush_output(DESCR_t *, int); DESCR_t dop_pl_format(DESCR_t *, int);
DESCR_t dop_pl_write_s(DESCR_t *, int); DESCR_t dop_pl_writeq_s(DESCR_t *, int); DESCR_t dop_pl_write_canonical_s(DESCR_t *, int); DESCR_t dop_pl_writeln_s(DESCR_t *,
    int); DESCR_t dop_pl_nl_s(DESCR_t *, int);
DESCR_t dop_pl_tab_s(DESCR_t *, int); DESCR_t dop_pl_put_char_s(DESCR_t *, int); DESCR_t dop_pl_flush_output_s(DESCR_t *, int); DESCR_t dop_pl_set_output(DESCR_t *,
    int); DESCR_t dop_pl_set_input(DESCR_t *, int);
DESCR_t rt_pl_dop_close(DESCR_t *, int); DESCR_t rt_pl_dop_read_s(DESCR_t *, int); DESCR_t rt_pl_dop_get_char_s(DESCR_t *, int); DESCR_t rt_pl_dop_peek_char_s(DESCR_t *,
    int); DESCR_t rt_pl_dop_current_output(DESCR_t *, int);
DESCR_t rt_pl_dop_get_code(DESCR_t *, int); DESCR_t rt_pl_dop_peek_code(DESCR_t *, int); DESCR_t rt_pl_dop_get_byte(DESCR_t *, int); DESCR_t rt_pl_dop_peek_byte(DESCR_t *,
    int); DESCR_t rt_pl_dop_unget_char(DESCR_t *, int);
DESCR_t rt_pl_dop_get_code_s(DESCR_t *, int); DESCR_t rt_pl_dop_peek_code_s(DESCR_t *, int); DESCR_t rt_pl_dop_get_byte_s(DESCR_t *, int); DESCR_t rt_pl_dop_peek_byte_s(DESCR_t *,
    int); DESCR_t rt_pl_dop_unget_char_s(DESCR_t *, int);
DESCR_t rt_pl_dop_unget_code(DESCR_t *, int); DESCR_t rt_pl_dop_unget_byte(DESCR_t *, int); DESCR_t rt_pl_dop_unget_code_s(DESCR_t *, int); DESCR_t rt_pl_dop_unget_byte_s(DESCR_t *, int);
DESCR_t dop_pl_at_end_of_stream(DESCR_t *, int); DESCR_t rt_pl_dop_at_end_of_stream_s(DESCR_t *, int); DESCR_t rt_pl_dop_get_edin(DESCR_t *, int); DESCR_t rt_pl_dop_skip(DESCR_t *, int);
DESCR_t rt_pl_dop_put_char_c(DESCR_t *, int); DESCR_t rt_pl_dop_put_char_c_s(DESCR_t *, int);
DESCR_t rt_pl_dop_current_prolog_flag(DESCR_t *, int); DESCR_t rt_pl_dop_set_prolog_flag(DESCR_t *, int); DESCR_t rt_pl_dop_telling(DESCR_t *, int); DESCR_t rt_pl_dop_seeing(DESCR_t *,
    int); DESCR_t rt_pl_dop_tell(DESCR_t *, int); DESCR_t rt_pl_dop_append1(DESCR_t *, int); DESCR_t rt_pl_dop_see(DESCR_t *, int);
DESCR_t dop_pl_told(DESCR_t *, int); DESCR_t dop_pl_seen(DESCR_t *, int);
DESCR_t rt_pl_dop_put_byte(DESCR_t *, int); DESCR_t rt_pl_dop_put_byte_s(DESCR_t *, int); DESCR_t dop_pl_put_code(DESCR_t *, int); DESCR_t dop_pl_put_code_s(DESCR_t *, int);
DESCR_t rt_pl_dop_current_input(DESCR_t *, int); DESCR_t rt_pl_dop_open(DESCR_t *, int); DESCR_t rt_pl_dop_open4(DESCR_t *, int); DESCR_t rt_pl_dop_keysort(DESCR_t *,
    int); DESCR_t rt_pl_dop_set_stream_position(DESCR_t *, int); DESCR_t rt_pl_dop_format3(DESCR_t *, int); DESCR_t rt_pl_dop_write_term(DESCR_t *,
        int); DESCR_t rt_pl_dop_write_term_s(DESCR_t *, int); DESCR_t rt_pl_dop_print(DESCR_t *, int);
        DESCR_t rt_pl_dop_print_s(DESCR_t *, int); DESCR_t rt_pl_dop_write_sb(DESCR_t *, int); DESCR_t rt_pl_dop_writeq_sb(DESCR_t *,
            int); DESCR_t rt_pl_dop_write_canonical_sb(DESCR_t *, int); DESCR_t rt_pl_dop_writeln_sb(DESCR_t *, int); DESCR_t rt_pl_dop_nl_sb(DESCR_t *,
                int); DESCR_t rt_pl_dop_tab_sb(DESCR_t *, int); DESCR_t rt_pl_dop_put_char_sb(DESCR_t *, int); DESCR_t rt_pl_dop_put_code_sb(DESCR_t *,
                    int); DESCR_t rt_pl_dop_flush_output_sb(DESCR_t *, int); DESCR_t rt_pl_dop_heap_guard(DESCR_t *, int); DESCR_t rt_pl_dop_op(DESCR_t *, int);
                        DESCR_t rt_pl_dop_pl_op_check(DESCR_t *,
                        int); DESCR_t rt_pl_dop_pl_sp_check(DESCR_t *, int); DESCR_t rt_pl_dop_pl_ioarg(DESCR_t *, int); DESCR_t rt_pl_dop_pl_op_count(DESCR_t *,
                            int); DESCR_t rt_pl_dop_pl_op_nth(DESCR_t *, int); DESCR_t rt_pl_dop_pl_sp_count(DESCR_t *, int); DESCR_t rt_pl_dop_cutcall(DESCR_t *,
                                int); DESCR_t rt_pl_dop_shc(DESCR_t *, int); DESCR_t rt_pl_dop_db_clause_src(DESCR_t *, int); DESCR_t rt_pl_dop_db_unload_file(DESCR_t *, int); DESCR_t rt_pl_dop_pl_sp_nth(DESCR_t *, int);
DESCR_t rt_pl_dop_alarm_push(DESCR_t *, int); DESCR_t rt_pl_dop_alarm_pop(DESCR_t *, int); DESCR_t rt_pl_dop_os_sleep(DESCR_t *, int); DESCR_t rt_pl_dop_sk_socket(DESCR_t *, int);
DESCR_t rt_pl_dop_sk_close(DESCR_t *, int); DESCR_t rt_pl_dop_sk_bind(DESCR_t *, int); DESCR_t rt_pl_dop_sk_listen(DESCR_t *, int); DESCR_t rt_pl_dop_sk_accept(DESCR_t *, int);
DESCR_t rt_pl_dop_sk_connect(DESCR_t *, int); DESCR_t rt_pl_dop_sk_open(DESCR_t *, int); DESCR_t rt_pl_dop_sk_setopt(DESCR_t *, int); DESCR_t rt_pl_dop_sk_fileno(DESCR_t *, int);
DESCR_t rt_pl_dop_sk_hostname(DESCR_t *, int); DESCR_t rt_pl_dop_sk_resolve(DESCR_t *, int); DESCR_t rt_pl_dop_proc_create(DESCR_t *, int); DESCR_t rt_pl_dop_proc_wait(DESCR_t *, int);
DESCR_t rt_pl_dop_proc_kill(DESCR_t *, int); DESCR_t rt_pl_dop_fx_stat(DESCR_t *, int); DESCR_t rt_pl_dop_fx_utime(DESCR_t *, int); DESCR_t rt_pl_dop_fx_symlink(DESCR_t *, int);
DESCR_t rt_pl_dop_fx_link(DESCR_t *, int); DESCR_t rt_pl_dop_fx_chmod(DESCR_t *, int); DESCR_t rt_pl_dop_fx_readlink(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_rem(DESCR_t *, int); DESCR_t rt_pl_dop_ax_fpow(DESCR_t *, int); DESCR_t rt_pl_dop_ax_pow(DESCR_t *, int); DESCR_t rt_pl_dop_ax_min(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_max(DESCR_t *, int); DESCR_t rt_pl_dop_ax_gcd(DESCR_t *, int); DESCR_t rt_pl_dop_ax_xor(DESCR_t *, int); DESCR_t rt_pl_dop_ax_shr(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_shl(DESCR_t *, int); DESCR_t rt_pl_dop_ax_band(DESCR_t *, int); DESCR_t rt_pl_dop_ax_bor(DESCR_t *, int); DESCR_t rt_pl_dop_ax_neg(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_pos(DESCR_t *, int); DESCR_t rt_pl_dop_ax_abs(DESCR_t *, int); DESCR_t rt_pl_dop_ax_sign(DESCR_t *, int); DESCR_t rt_pl_dop_ax_trunc(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_intg(DESCR_t *, int); DESCR_t rt_pl_dop_ax_flt(DESCR_t *, int); DESCR_t rt_pl_dop_ax_floor(DESCR_t *, int); DESCR_t rt_pl_dop_ax_ceil(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_round(DESCR_t *, int); DESCR_t rt_pl_dop_ax_sqrt(DESCR_t *, int); DESCR_t rt_pl_dop_ax_msb(DESCR_t *, int); DESCR_t rt_pl_dop_ax_bnot(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_sin(DESCR_t *, int); DESCR_t rt_pl_dop_ax_cos(DESCR_t *, int); DESCR_t rt_pl_dop_ax_atan(DESCR_t *, int); DESCR_t rt_pl_dop_ax_log(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_exp(DESCR_t *, int); DESCR_t rt_pl_dop_ax_fip(DESCR_t *, int); DESCR_t rt_pl_dop_ax_ffp(DESCR_t *, int); DESCR_t rt_pl_dop_ax_pi(DESCR_t *,
    int); DESCR_t rt_pl_dop_ax_e(DESCR_t *, int);
}
extern "C" {
DESCR_t pas_xr_ev_d(DESCR_t *, int); DESCR_t pas_xr_cvt_d(DESCR_t *, int); DESCR_t rt_pas_resundef(DESCR_t *, int); DESCR_t rt_pas_sin(DESCR_t *, int); DESCR_t rt_pas_cos(DESCR_t *, int);
    DESCR_t rt_pas_exp(DESCR_t *, int); DESCR_t rt_pas_sqrt(DESCR_t *, int);
DESCR_t rt_pas_ln(DESCR_t *, int); DESCR_t rt_pas_arctan(DESCR_t *, int); DESCR_t rt_pas_sqr(DESCR_t *, int);
DESCR_t rt_rk_arr_at(DESCR_t *, int); DESCR_t rt_rk_arr_set(DESCR_t *, int); DESCR_t rt_rk_elem_put(DESCR_t *, int);
DESCR_t rt_rk_param_check(DESCR_t *, int); DESCR_t rt_rk_assign_check(DESCR_t *, int); DESCR_t rt_rk_bindattr(DESCR_t *, int); DESCR_t rt_rk_box(DESCR_t *, int); DESCR_t rt_rk_blk_ref(DESCR_t *, int);
DESCR_t rt_rk_undef(DESCR_t *, int); DESCR_t rt_rk_range_arr(DESCR_t *, int); DESCR_t rt_rk_iter_src(DESCR_t *, int); DESCR_t rt_rk_map_append(DESCR_t *, int);
}
static_assert(X86_PL_TR_ENTRY_BYTES == PL_TR_ENTRY_BYTES, "x86_pl_tr_pop_head spells the trail entry size as a literal");
static_assert(X86_PL_TR_ARENA_MASK == -(long)PL_TR_ARENA_BYTES, "x86_pl_tr_top_sync spells the trail arena mask as a literal");
static_assert(X86_PL_TR_WAKE_OFF == PL_TR_WAKE_OFF && X86_PL_TR_RESIDUE_OFF == PL_TR_RESIDUE_OFF, "x86_pl_tr_tidy spells the wake and residue words as literals");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zls_argv_off(const IR_t *);
extern "C" int zls_redisp_off(const IR_t *);
extern "C" int pl_db_decls_words(long long ** out);
extern "C" int pl_ground_words(long long id, long long ** out);
extern "C" { typedef struct { long fn; long how; } call_next_rec_t; call_next_rec_t rt_call_arr_bl_try(const char *, DESCR_t *, long, DESCR_t *); call_next_rec_t rt_call_arr_bl_sn4_try(const char *,
    DESCR_t *, long, DESCR_t *); call_next_rec_t rk_callsame_open(DESCR_t *, int, void *); call_next_rec_t rk_callwith_open(DESCR_t *, int, void *); }
#endif

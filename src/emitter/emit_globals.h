#ifndef EMIT_GLOBALS_H
#define EMIT_GLOBALS_H
#include "emit_core.h"
#include "IR.h"
struct SrcLines;
typedef struct {
    int                          backend;
    int                          is_binary;
    int                          i;
    int                          n;
    IR_t *                       node;
    int                          sid;
    int                          nid;
    const char *                 op_sval;
    int64_t                      op_ival;
    int                          op_node_kind;
    const char *                 op_a_sval;
    int                          op_a_node_kind;
    int                          op_a_slot;
    int                          op_sa;
    int                          op_sb;
    int                          op_off;
    const char *                 lbl_α;
    const char *                 lbl_γ;
    const char *                 lbl_ω;
    const char *                 lbl_β;
    struct bb_label_t *          lbl_α_p;
    struct bb_label_t *          lbl_γ_p;
    struct bb_label_t *          lbl_ω_p;
    struct bb_label_t *          lbl_β_p;
    void *                       child_fn;
    const char *                 op_name1;
    const char *                 op_name2;
    const char *                 op_kind;
    int                          resolve_choice_id;
    int                          resolve_choice_n;
    int                          in_body;
    const char *                 in_my_method;
    const int *                  pc_to_fn;
    const char **                fn_names;
    const int *                  fn_pcs;
    int                          fn_count;
    const struct SrcLines *      srclines;
    int                          win_exec_pat_id;
    int                          hdr_count;
    int                          hdr_has_expr_reg;
    int                          hdr_has_reg;
    int                          reg_expr_count;
    int                          reg_count;
    char                         bb_ptr_slot_lbl[88];
    int                          bb_cs_id;
    void *                       bb_cs_zeta;
    void *                       bb_rt_obj;
    const char *                 bb_child_lbl;
    void *                       bb_child_fn;
    char                         bb_ls_buf[64];
    char                         bb_rs_buf[64];
    char                         bb_op_buf[64];
    const char *                 bb_ls;
    const char *                 bb_rs;
    const char *                 bb_op_lbl;
    int                          bb_lk;
    int64_t                      bb_li;
    int                          bb_rk;
    int64_t                      bb_ri;
    const char *                 flat_lbl_α_body;
    const char *                 flat_lbl_β;
    const char *                 flat_lbl_α;
    const char *                 flat_lbl_γ;
    const char *                 flat_lbl_ω;
    int                          flat_text_externalise;
    const char *                 enclosing_fname;
    const char *                 prev_instr_name;
    int                          flat_brokered;
    struct bb_label_t *          flat_succ_p;
    struct bb_label_t *          flat_fail_p;
    struct bb_label_t *          flat_β_p;
    const char **                xa_label_names;
    int *                        xa_label_pcs;
    int                          xa_label_count;
    const char **                xa_expr_names;
    int *                        xa_expr_pcs;
    int *                        xa_expr_str_idxs;
    int                          xa_expr_count;
    int                          xa_strtab_n;
    const char **                xa_strtab_labels;
    const char **                xa_strtab_escaped;
    const char *                 xa_cap_dlbl;
    const char *                 xa_cap_child_lbl;
    int                          xa_cap_is_arbno;
    int                          xa_cap_is_callcap;
    const char *                 xa_cap_varname_lbl;
    int                          xa_cap_immediate;
    int                          xa_pat_blob_invariant_n;
#define XA_BB_EMIT_PAIR_MAX 32
    struct bb_label_t *          xa_bb_emit_pair_define[XA_BB_EMIT_PAIR_MAX];
    struct bb_label_t *          xa_bb_emit_pair_jmp[XA_BB_EMIT_PAIR_MAX];
    int                          xa_bb_emit_pair_n;
    int                          x86_uid;
    int                          x86_scratch_off;
} sm_emit_t;
/*--------------------------------------------------------------------------------------------------------------------*/
extern sm_emit_t g_emit;
extern IR_graph_t * g_emit_cfg;
extern const char *Σ;
extern int         Σlen;
extern int         Δ;
#define TEMPLATE_ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
#define TEMPLATE_ADDR_SIGLEN  ((uint64_t)(uintptr_t)&Σlen)
#define TEMPLATE_ADDR_DELTA   ((uint64_t)(uintptr_t)&Δ)
#endif

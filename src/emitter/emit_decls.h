#include <string.h>
typedef struct { bb_label_t * k; int head; uint32_t gen; } bb_patch_hent_t;
#define STNO_SRC(i) CV_AT(g_emit.stno_src, sno_stno_src_t, (i))
#include "IR.h"
#include "emit_ir.h"
#define STRTAB(i) CV_AT(g_emitter.g_strtab, strtab_ent_t, (i))
#define CSETTAB(i) CV_AT(g_emitter.g_csettab, csettab_ent_t, (i))
#include "templates/bb/bb_templates.h"
#include "emit_ir.h"
#include "XA.h"
#include "../runtime/builtins/gen.h"
#include "IR.h"
#include "../rt/rt.h"
#include "bb_build.h"
#include "../optimizer/gva_collect.h"
#include "../optimizer/proc_collect.h"
#include "../optimizer/ir_query.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
extern const char * Σ;
extern int Σlen;
#define FLAT_BUF_MAX (flat_buf_max_v())
#define FLAT_DATA_TEXT ((char *)g_flat_data_buf.p)
#define FILL(nd,s,f,b) do { bb_fill_alpha(nd); g_emit.lbl_γ=(s)->name; g_emit.lbl_ω=(f)->name; g_emit.lbl_β=(b)->name; g_emit.lbl_γ_p=(s); g_emit.lbl_ω_p=(f); g_emit.lbl_β_p=(b); walk_bb_node( \
    (nd), emit_outf()); } while(0)
extern "C" int zls_dj_arm_resumes(const IR_t * nd, int a);
extern "C" int zls_dj_retains(const IR_t * nd);
extern "C" int zls_home_local_why(const IR_t * nd);
extern "C" int zls_home_local_test(const IR_graph_t * g, const char * vn);
extern "C" int zls_off(const IR_t *);
#define RK_BOOL_ROUTE(a0) (((a0) && (a0)->op == IR_BINOP && IR_LIT(a0).ival >= BINOP_LT && IR_LIT(a0).ival <= BINOP_NE) ? CALL_ROUTE_RK_BOOL_RELOP : CALL_ROUTE_RK_BOOL_SLOT)
extern "C" int fc_geom(const IR_t *, long *);
extern int is_global(const char *name);
extern IR_graph_t * g_emit_cfg;
#define DRIVE_FILL(nd,a,s,f,b) do { g_emit.op_selfload = 0; g_emit.op_fc_bytes = 0; g_emit.op_fc_base = -1; g_emit.x86_fc_synth = X86_INTERNAL_MAX - 1; g_emit.op_fc_fpmax = -1; g_emit.op_fc_disp = - \
    1; g_emit.op_fc_wbytes = 0; g_emit.op_arbno_chain = 0; g_emit.op_arbno_nzq = 0; g_emit.op_tail = 0; g_emit.op_tail_fpb = 0; g_emit.op_tail_fpl = 0; g_emit.op_tail_seal = 0; g_emit.op_tail_ncap = \
    0; g_emit.op_arbno_dt = 0; g_emit.op_arbno_dt_susp = 0; g_emit. op_defer_leaf_susp = -1; g_emit.op_tail_dfr = 0; g_emit.op_tail_fpr_rsp = 0; g_emit.op_body_has_arbno = 0; g_emit.op_arbno_framed \
    = 0; g_emit.op_arbno_body_k0 = 0; g_emit.op_arbno_body_kk = 0; g_emit.op_fence_body_kk = 0; g_emit.op_fence_frame_off = -1; g_emit.op_arbno_win_lo = 0; g_emit.op_arbno_win_bytes = 0; g_emit. \
    op_alt_M = -1; g_emit.op_alt_pad = (int *)0; g_emit.op_fence0_floor = 0; g_emit.op_arbno_body_defer_unsafe = 0; g_emit.op_arbno_body_actframe = 0; g_emit.op_arbno_body_fence = 0; g_emit. \
    op_alt_cell = 0; g_emit. op_arbno_rbp = 0; g_emit.op_range_int_operands = 0; bb_label_t *_fs__=bb_label_fold((s)), *_ff__=bb_label_fold((f)); g_emit.lbl_α=(a)->name; g_emit.lbl_γ=_fs__->name; \
    g_emit.lbl_ω=_ff__->name ; g_emit.lbl_β=(b)->name; g_emit.lbl_α_p=(a); g_emit.lbl_γ_p=_fs__; g_emit.lbl_ω_p=_ff__; g_emit.lbl_β_p=(b); walk_bb_node((nd), emit_outf()); } while(0)
#define DRIVE_PAIR_RESET() do { g_emit.xa_bb_emit_pair_n = 0; } while(0)
#define DRIVE_PAIR_JMP(tgt) do { xa_pair_push(NULL, (tgt)); } while(0)
#define DRIVE_PAIR_DEF_JMP(l,t) do { xa_pair_push((l), (t)); } while(0)
int xop_frame_slot(const IR_t * nd);
extern "C" int bb_proc_target_retains(const char *fname);
extern "C" int zls_g_vslot_count(const IR_graph_t *);
extern "C" const char * zls_g_vslot_get(const IR_graph_t *, int, int *);
extern "C" int zls_g_block_base(const IR_graph_t *);
extern "C" const char * zls_home_why_name(int w);
extern const char __start_rtx_entry_names[] __attribute__((visibility("hidden")));
extern const char __stop_rtx_entry_names[] __attribute__((visibility("hidden")));
#include "../templates/x86/rtx_clobber_table.inc"
struct rtx_ssr_t { const char * s; long n; };
struct rt_dcap_next_t { long fn; long how; };
#define RTX_TWIN(kind, ret, leaf, twin, args) extern "C" ret twin args;
#include "../templates/x86/rtx_twin_table.inc"
#undef RTX_TWIN
#ifdef __cplusplus
#endif
extern "C" {
int stage2_owner_varslot(const char * proc, const char * var);
const char * stage2_owner_l3_ancestor(const char * proc);
}
#include "builtin_ids.h"
#include "snobol4_system_fns.h"
#include "templates/bb/bb_pl_leaf_kinds.h"
extern "C" {
int stage2_owner_varslot(const char *, const char *);
int bb_scc_probe(const char *, int, int *, int *, int *, int *);
int rt_proc_nformals(const char *);
extern int g_flat_frame_floor;
}
#include "rt/rt_pl_trail.h"

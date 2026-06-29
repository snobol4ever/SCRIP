#include "emit_bb.h"
#include "emit_drive.h"
#include "BB_templates/bb_templates.h"
#include "emit_ir.h"
#include "emit_form.h"
#include "emit_templates.h"
#include "emit_globals.h"
#include "XA.h"
#include "../runtime/builtins/gen.h"
#include "IR.h"
#include "box_state.h"
#include "../rt/rt.h"
#include "bb_build.h"
#include "../opt/arith_fold.h"
#include "../opt/gva_collect.h"
#include "../opt/proc_collect.h"
#include "../opt/ir_query.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <gc/gc.h>
extern const char * Σ;
extern int          Σlen;
extern void rt_dcap_begin(void); extern void rt_dcap_end_ok(void); extern void rt_dcap_end_fail(void);
extern atp_t    * bb_atp_new                (const char *varname);
extern cap_t    * bb_cap_new_call           (bb_box_fn child_fn, void *child_state, const char *fnc_name, DESCR_t *fnc_args, int fnc_nargs, char **fnc_arg_names, int fnc_n_arg_names, int immediate);
extern cap_t    * bb_cap_new                (bb_box_fn child_fn, void *child_state, const char *varname, DESCR_t *var_ptr, int immediate);
void (*g_cap_fixup_cb)(void *cap_ptr, const char *child_α_label) = NULL;
const char *child_cache_get_lbl(bb_box_fn fn);
#define FLAT_BUF_MAX  (256 * 1024)
int g_flat_node_id   = 0;
static int g_flat_slot_count = 0;
int g_last_flat_frame_bytes = 0;
typedef struct { IR_t *key; int off; } bb_slotmap_ent_t;
static bb_slotmap_ent_t *g_bb_slotmap = NULL;
static int g_bb_slotmap_n = 0;
static int g_bb_slotmap_max = 0;
static void bb_slotmap_push(IR_t *nd, int off) {
    if (g_bb_slotmap_n >= g_bb_slotmap_max) {
        int new_max = g_bb_slotmap_max ? g_bb_slotmap_max * 2 : 512;
        bb_slotmap_ent_t *g = (bb_slotmap_ent_t *)realloc(g_bb_slotmap, (size_t)new_max * sizeof(bb_slotmap_ent_t));
        if (!g) return;
        g_bb_slotmap = g; g_bb_slotmap_max = new_max;
    }
    g_bb_slotmap[g_bb_slotmap_n].key = nd; g_bb_slotmap[g_bb_slotmap_n].off = off; g_bb_slotmap_n++;
}
int bb_slot_alloc16(IR_t *nd) {
    int off = g_flat_slot_count;
    g_flat_slot_count += 16;
    bb_slotmap_push(nd, off);
    return off;
}
/* Like bb_slot_alloc16 but returns the existing slot if the node already has one.
   Used when a node may be walked twice (chain pre-walk + generator re-walk). */
int bb_slot_alloc16_or_get(IR_t *nd) {
    int existing = bb_slot_get(nd);
    if (existing >= 0) return existing;
    int off = g_flat_slot_count;
    g_flat_slot_count += 16;
    bb_slotmap_push(nd, off);
    return off;
}
int bb_slot_get(IR_t *nd) {
    for (int i = 0; i < g_bb_slotmap_n; i++) if (g_bb_slotmap[i].key == nd) return g_bb_slotmap[i].off;
    return -1;
}
void bb_slot_register(IR_t *nd, int off) {
    bb_slotmap_push(nd, off);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_slot_claim(int bytes) {
    int off = g_flat_slot_count;
    g_flat_slot_count += bytes;
    return off;
}
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *name; int off; } bb_varslot_ent_t;
static bb_varslot_ent_t *g_bb_varslot = NULL;
static int g_bb_varslot_n = 0;
static int g_bb_varslot_max = 0;
int bb_varslot(const char *name) {
    if (!name) name = "";
    for (int i = 0; i < g_bb_varslot_n; i++)
        if (g_bb_varslot[i].name && strcmp(g_bb_varslot[i].name, name) == 0) return g_bb_varslot[i].off;
    int off = g_flat_slot_count;
    g_flat_slot_count += 16;
    if (g_bb_varslot_n >= g_bb_varslot_max) {
        int new_max = g_bb_varslot_max ? g_bb_varslot_max * 2 : 256;
        bb_varslot_ent_t *g = (bb_varslot_ent_t *)realloc(g_bb_varslot, (size_t)new_max * sizeof(bb_varslot_ent_t));
        if (!g) return off;
        g_bb_varslot = g; g_bb_varslot_max = new_max;
    }
    g_bb_varslot[g_bb_varslot_n].name = name; g_bb_varslot[g_bb_varslot_n].off = off; g_bb_varslot_n++;
    return off;
}
int bb_varslot_peek(const char *name) {
    if (!name) name = "";
    for (int i = 0; i < g_bb_varslot_n; i++)
        if (g_bb_varslot[i].name && strcmp(g_bb_varslot[i].name, name) == 0) return g_bb_varslot[i].off;
    return -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int g_proc_direct_active = 0;
int g_descr_flat_chain = 0;
int g_gvar_flat_chain = 0;
int g_gva_active = 0;
int g_gvar_callarg_live = 0;
int g_emit_frame_caller_dl = -1;
int g_frame_active = 0;
int g_gen_proc_active = 0;
int g_scan_regs_live = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
#define FLAT_CHAIN_SET_MAX 512
static IR_t *g_flat_chain_set[FLAT_CHAIN_SET_MAX];
static int   g_flat_chain_set_n = 0;
static bb_label_t *g_limit_gen_beta = NULL;   /* chain hands flat_drive_limit its generator's resume β */
static bb_label_t *g_suspend_dobody_beta = NULL; /* chain hands flat_drive_suspend its do-body resume label */
int                 g_subject_slot       = -1;
int                 g_match_start_slot   = -1;
bb_label_t *        g_scan_seal_lbl      = NULL;
const char *        g_match_elem_lbl     = NULL;
const char *        g_match_advance_lbl  = NULL;
struct bb_label_t * g_match_elem_p       = NULL;
struct bb_label_t * g_match_advance_p    = NULL;
#define FLAT_DATA_BUF_MAX     (32 * 1024)
#define FLAT_DATA_LBL_MAX     32
char   g_flat_data_buf[FLAT_DATA_BUF_MAX];
size_t g_flat_data_len    = 0;
static int    g_flat_data_active = 0;
#define CHILD_CACHE_MAX 64
static struct { IR_t *key; bb_box_fn fn; char text_lbl[80]; } g_child_cache[CHILD_CACHE_MAX];
static int g_child_cache_n = 0;
/*--------------------------------------------------------------------------------------------------------------------*/const char *child_cache_get_lbl(bb_box_fn fn) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "child_cache_get_lbl"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
int    g_flat_data_any    = 0;
static int    g_flat_data_just_closed = 0;
static char   g_flat_data_pending_lbl[160] = "";
static char   g_flat_data_block_lbls[FLAT_DATA_LBL_MAX][96];
static int    g_flat_data_block_nlbls = 0;void data_buf_reset(void) { g_flat_data_len = 0; g_flat_data_active = 0; g_flat_data_any = 0; g_flat_data_just_closed = 0; g_flat_data_block_nlbls = 0; g_flat_data_pending_lbl[0] = '\0'; }
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/static void data_buf_appendf(const char *fmt, ...) { if (g_flat_data_len >= FLAT_DATA_BUF_MAX) return; va_list ap; va_start(ap, fmt); int n = vsnprintf(g_flat_data_buf + g_flat_data_len, FLAT_DATA_BUF_MAX - g_flat_data_len, fmt, ap); va_end(ap); if (n > 0) { size_t left = FLAT_DATA_BUF_MAX - g_flat_data_len; g_flat_data_len += ((size_t)n < left) ? (size_t)n : left; } }
/*--------------------------------------------------------------------------------------------------------------------*/void data_buf_flush_pending_label(void) { if (!g_flat_data_pending_lbl[0]) return; data_buf_appendf("%s\n", g_flat_data_pending_lbl); g_flat_data_pending_lbl[0] = '\0'; }
#define SYM_SIGMA   "\xCE\xA3"
#define SYM_SIGLEN  "\xCE\xA3""len"
#define SYM_DELTA   "\xCE\x94"
#define ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
#define ADDR_SIGLEN  ((uint64_t)(uintptr_t)&Σlen)
#define ADDR_DELTA   ((uint64_t)(uintptr_t)&Δ)
static const char *(*g_flat_intern_str)(const char *s) = NULL;const char *emit_intern_str(const char *s) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "emit_intern_str"); abort(); }
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
static void flat_emit_arg_subchain(IR_t *entry, bb_label_t *succ, bb_label_t *fail);
static int ir_node_is_alt_arm(IR_t *nd);
static IR_t *ir_skip_alt_arms(IR_t *entry);
static void descr_chain_operand_refs(IR_t *entry);
static void gvar_stmt_operand_refs(IR_t *head);
static int gvar_prewalk_idx_operand(IR_t *idx, bb_label_t *lbl_ω);
IR_t * bb_child0(const IR_t *n) { return (n && n->n_operands > 0) ? n->operands[0] : NULL; }
IR_t * bb_child1(const IR_t *n) { return (n && n->n_operands > 1) ? n->operands[1] : NULL; }
void bb_flat_cursor_reserve(int upto) { if (upto > g_flat_slot_count) g_flat_slot_count = upto; }int bb_flat_cursor(void) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_flat_cursor"); abort(); }
extern int memcmp(const void *, const void *, size_t);
static bb_label_t g_α_ring[8];
static int        g_α_ring_i = 0;
static int        g_bb_alpha_seq = 0;void g_bb_alpha_seq_reset(void) { g_bb_alpha_seq = 0; }
void bb_fill_alpha(IR_t *nd) {
    extern int g_m4_dense_nid;
    bb_label_t *a = &g_α_ring[g_α_ring_i++ & 7];
    if (g_m4_dense_nid) emit_label_initf(a, "bb%d_α", ++g_bb_alpha_seq);
    else                    emit_label_initf(a, "bb%d_α", nd ? bb_node_id(nd) : 0);
    g_emit.lbl_α   = a->name;
    g_emit.lbl_α_p = a;
    if (nd && nd->op == IR_BINOP_GVAR_ARITH) { static char gvapool[3][64]; g_emit.op_parts_lbl[0] = NULL; g_emit.op_parts_lbl[1] = NULL; g_emit.op_parts_lbl[2] = NULL;
      if (g_emit.op_name1 && g_emit.op_name1[0]) { strtab_label(gvapool[0], 64, g_emit.op_name1); g_emit.op_parts_lbl[0] = gvapool[0]; }
      if (g_emit.op_name2 && g_emit.op_name2[0]) { strtab_label(gvapool[1], 64, g_emit.op_name2); g_emit.op_parts_lbl[1] = gvapool[1]; }
      if (g_emit.op_kind && !strcmp(g_emit.op_kind, "POW") && g_emit.op_sval && g_emit.op_sval[0]) { strtab_label(gvapool[2], 64, g_emit.op_sval); g_emit.op_parts_lbl[2] = gvapool[2]; } }
    if (nd && nd->op == IR_BINOP_GVAR_ARITH_SLOT) { static char gvspool[2][64]; g_emit.op_parts_lbl[0] = NULL; g_emit.op_parts_lbl[1] = NULL;
      if (g_emit.bb_lk == (int)IR_VAR && g_emit.op_name1 && g_emit.op_name1[0]) { strtab_label(gvspool[0], 64, g_emit.op_name1); g_emit.op_parts_lbl[0] = gvspool[0]; }
      if (g_emit.bb_rk == (int)IR_VAR && g_emit.op_name2 && g_emit.op_name2[0]) { strtab_label(gvspool[1], 64, g_emit.op_name2); g_emit.op_parts_lbl[1] = gvspool[1]; } }
    if (nd && nd->op == IR_IDX) { static char idxgpool[2][64]; g_emit.op_parts_lbl[0] = NULL; g_emit.op_parts_lbl[1] = NULL;
      if (g_emit.op_name1 && g_emit.op_name1[0]) { strtab_label(idxgpool[0], 64, g_emit.op_name1); g_emit.op_parts_lbl[0] = idxgpool[0]; }
      if (g_emit.op_name2 && g_emit.op_name2[0]) { strtab_label(idxgpool[1], 64, g_emit.op_name2); g_emit.op_parts_lbl[1] = idxgpool[1]; } }
    if (nd && nd->op == IR_IDX_SET) { static char idxspool[3][64]; g_emit.op_parts_lbl[0] = NULL; g_emit.op_parts_lbl[1] = NULL; g_emit.op_parts_lbl[2] = NULL;
      if (g_emit.op_name1 && g_emit.op_name1[0]) { strtab_label(idxspool[0], 64, g_emit.op_name1); g_emit.op_parts_lbl[0] = idxspool[0]; }
      if (g_emit.op_name2 && g_emit.op_name2[0]) { strtab_label(idxspool[1], 64, g_emit.op_name2); g_emit.op_parts_lbl[1] = idxspool[1]; }
      if (g_emit.op_parts_str[2] && g_emit.op_parts_str[2][0]) { strtab_label(idxspool[2], 64, g_emit.op_parts_str[2]); g_emit.op_parts_lbl[2] = idxspool[2]; } }
    if (nd && nd->op == IR_CALL_DEFINE) { static char defpool[64]; g_emit.op_parts_lbl[0] = NULL;
      int64_t narg = IR_LIT(nd).ival;
      IR_graph_t ** subs = (IR_graph_t **)0;
      IR_t * spec = (narg > 0 && subs && subs[0]) ? subs[0]->entry : NULL;
      const char * specstr = (spec && spec->op == IR_LIT_S && IR_LIT(spec).sval) ? IR_LIT(spec).sval : "";
      g_emit.op_sval = specstr;
      strtab_label(defpool, 64, specstr); g_emit.op_parts_lbl[0] = defpool; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define FILL(nd,s,f,b) do { \
    bb_fill_alpha(nd); \
    g_emit.lbl_γ=(s)->name; g_emit.lbl_ω=(f)->name; g_emit.lbl_β=(b)->name; \
    g_emit.lbl_γ_p=(s); g_emit.lbl_ω_p=(f); g_emit.lbl_β_p=(b); \
    walk_bb_node((nd), emit_outf()); } while(0)
#define EMIT_PAIR_RESET()        do { g_emit.xa_bb_emit_pair_n = 0; } while(0)
#define EMIT_PAIR_DEF(lbl)       do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=(lbl); g_emit.xa_bb_emit_pair_jmp[_i]=NULL; } while(0)
#define EMIT_PAIR_JMP(tgt)       do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=NULL; g_emit.xa_bb_emit_pair_jmp[_i]=(tgt); } while(0)
#define EMIT_PAIR_DEF_JMP(l,t)   do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=(l); g_emit.xa_bb_emit_pair_jmp[_i]=(t); } while(0)
#define EMIT_PAIR_FILL(nd,s,f,b) do { \
    bb_fill_alpha(nd); \
    g_emit.lbl_γ=(s)->name; g_emit.lbl_ω=(f)->name; g_emit.lbl_β=(b)->name; \
    g_emit.lbl_γ_p=(s); g_emit.lbl_ω_p=(f); g_emit.lbl_β_p=(b); \
    walk_bb_node((nd), emit_outf()); } while(0)
static int is_pat_chain_elem(IR_e t);
static void emit_cat_diamond(IR_t *entry, IR_t *J, bb_label_t *fwd, bb_label_t *fail, bb_label_t *arm_beta, int id, int armidx);
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int is_pat_chain_elem(IR_e t);
static int gather_lowered_cat_arms(IR_t *entry, IR_t **arms, IR_t **joins, int cap, IR_t **cat_out, IR_t *stop);
/* Collect γ/ω-threaded inline alt arms: arm[0]=start, arm[i+1]=arm[i]->ω.node while ω is a pat elem.
   Returns number of arms collected (1 = single arm, no alternation chain). */
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static void gz_emit_cell(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv);
/* catch box: α marks the trail then runs the goal chain; goal.γ -> catch success (next_γ);
 * goal.ω -> the ball-check handler (op_sa 1): no pending throw -> ω (plain failure); pending ->
 * unwind to mark, gzu_build the catcher, rt_pl_throw_match -> recovery chain on match, else ω
 * leaving the ball set so an outer catch re-catches. Recovery chain: γ->next_γ, ω->catch.ω. */static void gz_emit_catch(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gz_emit_catch"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/void resolve_choice_clause_label(char *dst, size_t dsz, int id, int ci, const char *suffix) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "resolve_choice_clause_label"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/int bb_kind_is_driver_owned(int t) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_kind_is_driver_owned"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/static const char *bb_intern_into(char *buf, const char *sval) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_intern_into"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
static int bb_prepare_assign(IR_t *nd) {
    if (nd->op == IR_DTP_ASSIGN) { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); return 1; }
    if (nd->op == IR_ASSIGN) {
        IR_t *oa = (nd->n_operands > 0) ? nd->operands[0] : NULL;
        g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : "");
        g_emit.bb_rs = oa ? bb_intern_into(g_emit.bb_rs_buf, IR_LIT(oa).sval) : NULL;
        return 1;
    }
    if (nd->op == IR_ASSIGN_CONCAT) {
        g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : "");
        if (g_emit.op_parts_n == 1 && g_emit.op_parts_tag[0] == 0)
            g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, g_emit.op_parts_str[0] ? g_emit.op_parts_str[0] : "");
        else if (g_emit.op_parts_n > 0) g_emit.op_off = bb_slot_claim(16 * g_emit.op_parts_n);
        return 1;
    }
    if (nd->op == IR_ASSIGN_LIT_I) { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); return 1; }
    if (nd->op == IR_ASSIGN_LIT_S) { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, g_emit.op_a_sval ? g_emit.op_a_sval : ""); return 1; }
    if (nd->op == IR_INDIRECT_ASSIGN_LIT_S) { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, g_emit.op_a_sval ? g_emit.op_a_sval : ""); return 1; }
    if (nd->op == IR_INDIRECT_ASSIGN_VAR)   { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, g_emit.op_a_sval ? g_emit.op_a_sval : ""); return 1; }
    if (nd->op == IR_ASSIGN_VAR)   { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, g_emit.op_a_sval ? g_emit.op_a_sval : ""); return 1; }
    if (nd->op == IR_ASSIGN_CALL)  { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); return 1; }
    if (nd->op == IR_ASSIGN_DESCR) { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); return 1; }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_prepare(IR_t *nd) {
    if (!PLATFORM_X86) return;
    g_emit.bb_ls = NULL;
    g_emit.bb_rs = NULL;
    g_emit.bb_op_lbl = NULL;
    g_emit.bb_lk = -1;
    if (bb_prepare_assign(nd)) return;
    if (nd->op == IR_ALT) {
        int n = 0;
        IR_t * const * arms = g_emit_cfg ? bb_operand_aux_get(g_emit_cfg, nd, &n) : ((IR_t * const *)0);
        int ok = (arms && n > 0 && n <= 5);
        for (int i = 0; ok && i < n; i++)
            if (!arms[i] || (arms[i]->op != IR_LIT_I && arms[i]->op != IR_LIT_S && arms[i]->op != IR_LIT_F)) ok = 0;
        g_emit.op_parts_n = ok ? n : 0;
        for (int i = 0; ok && i < n; i++) {
            g_emit.op_parts_tag[i]  = (arms[i]->op == IR_LIT_I) ? (int)DT_I : (arms[i]->op == IR_LIT_F) ? (int)DT_R : (int)DT_S;
            if (arms[i]->op == IR_LIT_F) { double fd = IR_LIT(arms[i]).dval; int64_t fb; memcpy(&fb, &fd, 8); g_emit.op_parts_ival[i] = fb; }
            else g_emit.op_parts_ival[i] = (int64_t)IR_LIT(arms[i]).ival;
            g_emit.op_parts_str[i]  = IR_LIT(arms[i]).sval ? IR_LIT(arms[i]).sval : "";
        }
        return;
    }
    if (nd->op == IR_ASSIGN_FRAME || nd->op == IR_ASSIGN_FRAME_REF) {
        IR_t *fa = (nd->n_operands > 0) ? nd->operands[0] : ((IR_t *)0);
        int fk = fa ? (int)ir_norm_call_kind(fa->op) : -1;
        g_emit.bb_lk = (fk == (int)IR_LIT_I) ? 1 : (fk == (int)IR_LIT_NUL) ? 2 : (fk == (int)IR_LIT_S) ? 3 : (fk == (int)IR_VAR) ? 4 :
                       (fk == (int)IR_VAR_FRAME) ? 5 : (fk == (int)IR_VAR_FRAME_REF) ? 6 : (fk == (int)IR_BINOP) ? 7 : (fk == (int)IR_CALL) ? 8 : 0;
        if (g_emit.bb_lk == 3 || g_emit.bb_lk == 4) g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(fa).sval ? IR_LIT(fa).sval : "");
        if ((g_emit.bb_lk == 5 || g_emit.bb_lk == 6) && fa) { g_emit.op_a_dval = IR_LIT(fa).dval; g_emit.op_a_ival_sg = IR_LIT(fa).ival; }
        return;
    }
    if (nd->op == IR_ATOM) {
        g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : "");
        return;
    }
    if (nd->op == IR_ARITH) {
        IR_t *l = (nd->n_operands > 0) ? nd->operands[0] : ((IR_t*)0), *r = (nd->n_operands > 1) ? nd->operands[1] : ((IR_t*)0);
        if (!l || !r) return;
        g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(l).sval);
        g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, IR_LIT(r).sval);
        g_emit.bb_op_lbl = bb_intern_into(g_emit.bb_op_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : "+");
        g_emit.bb_lk = (int)ir_norm_call_kind(l->op); g_emit.bb_li = (int64_t)IR_LIT(l).ival;
        g_emit.bb_rk = (int)ir_norm_call_kind(r->op); g_emit.bb_ri = (int64_t)IR_LIT(r).ival;
        return;
    }
    if (nd->op == IR_UNIFY || nd->op == IR_CELL_UNIFY) {
        IR_t *l = (nd->n_operands > 0) ? nd->operands[0] : ((IR_t*)0), *r = (nd->n_operands > 1) ? nd->operands[1] : ((IR_t*)0);
        if (nd->op == IR_CELL_UNIFY) { g_emit.op_parts_n = 1; g_emit.op_parts_ival[0] = -1; g_emit.op_parts_ival[1] = 0; g_emit.op_parts_ival[2] = 0; g_emit.op_parts_ival[3] = 0; g_emit.op_parts_str[0] = NULL; }
        if (!l || !r) return;
        g_emit.bb_lk = (int)l->op; g_emit.bb_li = (int64_t)IR_LIT(l).ival;
        g_emit.bb_rk = (int)r->op; g_emit.bb_ri = (int64_t)IR_LIT(r).ival;
        g_emit.bb_ln = (void *)l; g_emit.bb_rn = (void *)r;
        if (l->op == IR_ATOM) g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(l).sval ? IR_LIT(l).sval : "");
        if (r->op == IR_ATOM) g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, IR_LIT(r).sval ? IR_LIT(r).sval : "");
        if (nd->op == IR_CELL_UNIFY) {
            int lk = (int)l->op, rk = (int)r->op;
            int lc = (lk == (int)IR_ATOM || lk == (int)IR_LIT_I || lk == (int)IR_LIT_F), rc = (rk == (int)IR_ATOM || rk == (int)IR_LIT_I || rk == (int)IR_LIT_F);
            if (lk == (int)IR_STRUCT || rk == (int)IR_STRUCT || lk == (int)IR_ARITH || rk == (int)IR_ARITH) { g_emit.op_parts_ival[0] = 0; g_emit.op_parts_ival[1] = (int64_t)(intptr_t)l; g_emit.op_parts_ival[2] = (int64_t)(intptr_t)r; }
            else if (lk == (int)IR_LOGICVAR && rk == (int)IR_LOGICVAR && IR_LIT(l).ival == IR_LIT(r).ival) g_emit.op_parts_ival[0] = 1;
            else if (lk == (int)IR_LOGICVAR && rk == (int)IR_LOGICVAR) { g_emit.op_parts_ival[0] = 2; g_emit.op_parts_ival[1] = (int64_t)IR_LIT(l).ival; g_emit.op_parts_ival[2] = (int64_t)IR_LIT(r).ival; }
            else if (lk == (int)IR_LOGICVAR && rk == (int)IR_LIT_F && (int)IR_LIT(l).ival >= 0) { g_emit.op_parts_ival[0] = 3; g_emit.op_parts_ival[1] = (int64_t)(int)IR_LIT(l).ival; double fd = IR_LIT(r).dval; memcpy(&g_emit.op_parts_ival[2], &fd, 8); }
            else if (rk == (int)IR_LOGICVAR && lk == (int)IR_LIT_F && (int)IR_LIT(r).ival >= 0) { g_emit.op_parts_ival[0] = 3; g_emit.op_parts_ival[1] = (int64_t)(int)IR_LIT(r).ival; double fd = IR_LIT(l).dval; memcpy(&g_emit.op_parts_ival[2], &fd, 8); }
            else if (lk == (int)IR_LOGICVAR && (rk == (int)IR_ATOM || rk == (int)IR_LIT_I) && (int)IR_LIT(l).ival >= 0) { g_emit.op_parts_ival[0] = 4; g_emit.op_parts_ival[1] = (int64_t)(int)IR_LIT(l).ival; g_emit.op_parts_ival[2] = (int64_t)rk; g_emit.op_parts_ival[3] = (int64_t)IR_LIT(r).ival; g_emit.op_parts_str[0] = (rk == (int)IR_ATOM) ? IR_LIT(r).sval : NULL; }
            else if (rk == (int)IR_LOGICVAR && (lk == (int)IR_ATOM || lk == (int)IR_LIT_I) && (int)IR_LIT(r).ival >= 0) { g_emit.op_parts_ival[0] = 4; g_emit.op_parts_ival[1] = (int64_t)(int)IR_LIT(r).ival; g_emit.op_parts_ival[2] = (int64_t)lk; g_emit.op_parts_ival[3] = (int64_t)IR_LIT(l).ival; g_emit.op_parts_str[0] = (lk == (int)IR_ATOM) ? IR_LIT(l).sval : NULL; }
            else if (lc && rc) { int eq = 0; if (lk == rk) { if (lk == (int)IR_LIT_F) eq = (IR_LIT(l).dval == IR_LIT(r).dval); else if (lk == (int)IR_LIT_I) eq = (IR_LIT(l).ival == IR_LIT(r).ival); else eq = (IR_LIT(l).sval && IR_LIT(r).sval && strcmp(IR_LIT(l).sval, IR_LIT(r).sval) == 0); } g_emit.op_parts_ival[0] = eq ? 5 : 6; }
        }
        return;
    }
    if (nd->op == IR_BUILTIN) {
        IR_t *bu0 = ir_call_arg(nd, 0);
        if (bu0 && bu0->op == IR_ATOM)
            g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(bu0).sval ? IR_LIT(bu0).sval : "");
        IR_t *bu1 = ir_pair_arg(nd, 1);
        if (IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval, "is") == 0 && bu1 && bu1->op == IR_ARITH)
            g_emit.bb_op_lbl = bb_intern_into(g_emit.bb_op_buf, IR_LIT(bu1).sval ? IR_LIT(bu1).sval : "+");
        if (IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval, "is") == 0 && bu1 && bu1->op == IR_ATOM)
            g_emit.bb_op_lbl = bb_intern_into(g_emit.bb_op_buf, IR_LIT(bu1).sval ? IR_LIT(bu1).sval : "");
        if (IR_LIT(nd).sval && (strcmp(IR_LIT(nd).sval, "aggregate_all") == 0 || strcmp(IR_LIT(nd).sval, "nb_setval") == 0 || strcmp(IR_LIT(nd).sval, "nb_getval") == 0)) {
            IR_t *a1 = ir_call_arg(nd, 1);
            if (a1 && a1->op == IR_ATOM) g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, IR_LIT(a1).sval ? IR_LIT(a1).sval : "");
            IR_t *a2 = ir_call_arg(nd, 2);
            if (a2 && a2->op == IR_ATOM) g_emit.bb_op_lbl = bb_intern_into(g_emit.bb_op_buf, IR_LIT(a2).sval ? IR_LIT(a2).sval : "");
        }
        int n = 0;
        g_emit.op_parts_tag[3] = 0; g_emit.op_parts_ival[3] = 0; g_emit.op_parts_str[3] = NULL; g_emit.op_parts_ival[11] = 0;
        for (int j = 0; j < 3; j++) {
            IR_t *ax = ir_call_arg(nd, j);
            if (!ax) break;
            g_emit.op_parts_tag[j]      = (int)ax->op;
            g_emit.op_parts_ival[j]     = (int64_t)IR_LIT(ax).ival;
            g_emit.op_parts_str[j]      = (ax->op == IR_ATOM || ax->op == IR_STRUCT || ax->op == IR_ARITH) ? IR_LIT(ax).sval : NULL;
            g_emit.op_parts_ival[8 + j] = (int64_t)(intptr_t)ax;
            n = j + 1;
        }
        g_emit.op_parts_n = n;
        if (n >= 2) {
            IR_t *a1 = ir_call_arg(nd, 1);
            if (a1 && (a1->op == IR_STRUCT || a1->op == IR_ARITH) && IR_LIT(a1).sval) {
                IR_t *in = ir_call_arg(a1, 0);
                if (in) { g_emit.op_parts_tag[3] = (int)in->op; g_emit.op_parts_ival[3] = (int64_t)IR_LIT(in).ival;
                          g_emit.op_parts_str[3] = (in->op == IR_ATOM) ? IR_LIT(in).sval : NULL; g_emit.op_parts_ival[11] = (int64_t)(intptr_t)in; }
            }
        }
        {
            static char lblpool[4][64];
            for (int j = 0; j < 4; j++) {
                g_emit.op_parts_lbl[j] = NULL;
                if (g_emit.op_parts_str[j] && g_emit.op_parts_str[j][0]) { strtab_label(lblpool[j], 64, g_emit.op_parts_str[j]); g_emit.op_parts_lbl[j] = lblpool[j]; }
            }
        }
        {
            static char svallblbuf[64];
            const char *bfn = IR_LIT(nd).sval;
            g_emit.op_sval_lbl = NULL;
            if (bfn && bfn[0]) { strtab_label(svallblbuf, 64, bfn); g_emit.op_sval_lbl = svallblbuf; }
        }
        return;
    }
    if (nd->op == IR_GOAL) {
        const bb_goal_state_t *zc = (const bb_goal_state_t *)(intptr_t)IR_LIT(nd).ival;
        const char *_goal_nm = (zc && zc->callee && zc->callee[0]) ? zc->callee : (IR_LIT(nd).sval ? IR_LIT(nd).sval : "");
        snprintf(g_emit.bb_ls_buf, sizeof g_emit.bb_ls_buf, "%s", _goal_nm); g_emit.bb_ls = g_emit.bb_ls_buf;
        g_emit.op_sa = zc ? zc->arity : 0;
        int nn = zc ? zc->nargs : 0;
        if (nn > g_emit.op_sa) nn = g_emit.op_sa;
        g_emit.op_parts_n = nn;
        for (int gi = 0; gi < nn && gi < 16; gi++) {
            IR_t *a = (zc && zc->args) ? zc->args[gi] : NULL;
            g_emit.op_parts_tag[gi]  = a ? (int)a->op : 0;
            g_emit.op_parts_ival[gi] = (a && a->op == IR_STRUCT) ? (int64_t)(intptr_t)a : (a ? (int64_t)IR_LIT(a).ival : 0);
            g_emit.op_parts_str[gi]  = a ? IR_LIT(a).sval : NULL;
        }
        return;
    }
    if (nd->op == IR_CATCH) {
        g_emit.bb_zn = (void *)nd;
        return;
    }
    if (nd->op == IR_CELL_CHOICE) {
        g_emit.bb_zn = (void *)nd;
        return;
    }
    if (nd->op == IR_GATHER) {
        extern void bb_gather_prepare(IR_t *nd);
        bb_gather_prepare(nd);
        return;
    }
    if (nd->op == IR_MAP || nd->op == IR_GREP) {
        extern void bb_mapgrep_prepare(IR_t *nd);
        bb_mapgrep_prepare(nd);
        return;
    }
    if (nd->op == IR_CALLEE_FRAME) {
        g_emit.bb_zn = (void *)nd;
        const pl_gz_callee_t * ce = (const pl_gz_callee_t *)(intptr_t)IR_LIT(nd).ival;
        g_emit.op_parts_n = ce ? 1 : 0;
        g_emit.op_parts_ival[0] = ce ? (int64_t)ce->arity : -1;
        g_emit.op_parts_ival[1] = ce ? (int64_t)ce->nlocals : -1;
        g_emit.op_parts_ival[2] = ce ? (int64_t)(ce->nclauses > 0 ? ce->nclauses : 1) : 1;
        return;
    }
    if (nd->op == IR_CELL_CALL || nd->op == IR_CELL_ITE) {
        g_emit.bb_zn = (void *)nd;
        if (nd->op == IR_CELL_CALL) {
            const pl_gz_call_state_t * st = (const pl_gz_call_state_t *)(intptr_t)IR_LIT(nd).ival;
            g_emit.op_parts_n = st ? 1 : 0;
            g_emit.op_parts_ival[0] = st ? (int64_t)st->child_slot : -1;
            g_emit.op_parts_ival[1] = st ? (int64_t)st->nargs : -1;
            g_emit.op_parts_ival[2] = (st && st->callee) ? (int64_t)(st->callee->arity + st->callee->nlocals + st->callee->nchild) : -1;
            { int _na = st ? st->nargs : 0;
              if (_na > g_emit.gz_arg_slots_cap) { int _nc = g_emit.gz_arg_slots_cap ? g_emit.gz_arg_slots_cap : 8; while (_nc < _na) _nc *= 2; g_emit.gz_arg_slots = (int64_t *)GC_MALLOC(sizeof(int64_t) * _nc); g_emit.gz_arg_slots_cap = _nc; }
              for (int _ai = 0; _ai < _na; _ai++)
                  g_emit.gz_arg_slots[_ai] = (st->args[_ai] && st->args[_ai]->op == IR_LOGICVAR) ? IR_LIT(st->args[_ai]).ival : -2; }
        }
        if (nd->op == IR_CELL_ITE) {
            const pl_gz_ite_state_t * is = (const pl_gz_ite_state_t *)(intptr_t)IR_LIT(nd).ival;
            g_emit.op_parts_n = is ? 1 : 0;
            g_emit.op_parts_ival[0] = is ? (int64_t)is->gate_slot : -1;
        }
        return;
    }
    if (nd->op == IR_DET_THROW) {
        g_emit.op_parts_ival[0] = IR_LIT(nd).ival;   /* the ball IR node (slot-mapped at build time) */
        return;
    }
    if (nd->op == IR_CELL_CATCH) {
        const pl_gz_catch_state_t *cst = (const pl_gz_catch_state_t *)(intptr_t)IR_LIT(nd).ival;
        g_emit.op_parts_ival[0] = cst ? (int64_t)cst->mark_slot : -1;
        g_emit.op_parts_ival[1] = cst ? (int64_t)(intptr_t)cst->catcher : 0;
        return;
    }
    if (nd->op == IR_CELL_FINDALL) {
        g_emit.bb_zn = (void *)nd;
        const pl_gz_findall_state_t * fst = (const pl_gz_findall_state_t *)(intptr_t)IR_LIT(nd).ival;
        g_emit.op_parts_n = fst ? 1 : 0;
        pl_gz_call_state_t * cs = fst ? fst->call : (pl_gz_call_state_t *)0;
        g_emit.op_parts_ival[0] = cs ? (int64_t)cs->child_slot : -1;
        g_emit.op_parts_ival[1] = cs ? (int64_t)cs->nargs : 0;
        g_emit.op_parts_ival[2] = (cs && cs->callee) ? (int64_t)(cs->callee->arity + cs->callee->nlocals + cs->callee->nchild) : 0;
        g_emit.op_parts_ival[3] = (cs && cs->nargs > 0 && cs->args[0] && cs->args[0]->op == IR_LOGICVAR) ? (int64_t)IR_LIT(cs->args[0]).ival : -1;
        g_emit.op_parts_ival[4] = (cs && cs->nargs > 1 && cs->args[1] && cs->args[1]->op == IR_LOGICVAR) ? (int64_t)IR_LIT(cs->args[1]).ival : -1;
        g_emit.op_parts_ival[5] = (cs && cs->nargs > 2 && cs->args[2] && cs->args[2]->op == IR_LOGICVAR) ? (int64_t)IR_LIT(cs->args[2]).ival : -1;
        g_emit.op_parts_ival[6] = fst ? (int64_t)fst->result_slot : -1;
        g_emit.op_parts_ival[7] = fst ? (int64_t)fst->acc_slot : -1;
        g_emit.op_parts_ival[8] = fst ? (int64_t)fst->is_fail : 0;
        g_emit.op_parts_ival[9] = fst ? (int64_t)(intptr_t)fst->tmpl : 0;
        g_emit.op_parts_ival[10] = fst ? (int64_t)fst->agg_mode : 0;
        return;
    }
    if (nd->op == IR_DET_IS) {
        const IR_t * l = bb_child0(nd);
        const IR_t * r = bb_child1(nd);
        long cval = 0, rc = 0;
        int rslot = -1, bslot1 = -1, bslot2 = -1;
        const char * rop = NULL, * bop = NULL;
        g_emit.op_parts_n = 4;
        for (int i = 0; i < 4; i++) g_emit.op_parts_ival[i] = 0;
        g_emit.op_parts_str[0] = NULL;
        if (!l || l->op != IR_LOGICVAR) { g_emit.op_parts_ival[0] = -1; return; }
        g_emit.op_parts_ival[1] = (int64_t)IR_LIT(l).ival;
        if (gz_arith_const_eval(r, &cval)) { g_emit.op_parts_ival[0] = 0; g_emit.op_parts_ival[2] = (int64_t)cval; return; }
        if (gz_arith_var_plus_const(r, &rslot, &rop, &rc)) { g_emit.op_parts_ival[0] = 1; g_emit.op_parts_ival[2] = (int64_t)rslot; g_emit.op_parts_ival[3] = (int64_t)rc; g_emit.op_parts_str[0] = rop; return; }
        if (gz_arith_var_bivar(r, &bslot1, &bslot2, &bop)) { g_emit.op_parts_ival[0] = 2; g_emit.op_parts_ival[2] = (int64_t)bslot1; g_emit.op_parts_ival[3] = (int64_t)bslot2; g_emit.op_parts_str[0] = bop; return; }
        { double fval = 0.0; if (gz_arith_float_eval(r, &fval)) { g_emit.op_parts_ival[0] = 3; memcpy(&g_emit.op_parts_ival[2], &fval, 8); return; } }
        { double fval = 0.0;
          if (r && r->op == IR_ARITH && IR_LIT(r).sval && ir_pair_arg(r,0) && !ir_pair_arg(r,1)) {
              const char *op = IR_LIT(r).sval;
              int is_int_of_float = strcmp(op,"truncate")==0||strcmp(op,"integer")==0||strcmp(op,"round")==0||strcmp(op,"ceiling")==0||strcmp(op,"floor")==0;
              if (is_int_of_float && gz_arith_float_eval(ir_pair_arg(r,0), &fval)) {
                  long iv = (strcmp(op,"truncate")==0||strcmp(op,"integer")==0) ? (long)fval : strcmp(op,"round")==0 ? (long)(fval+0.5) : strcmp(op,"ceiling")==0 ? (long)ceil(fval) : (long)floor(fval);
                  g_emit.op_parts_ival[0] = 0; g_emit.op_parts_ival[2] = (int64_t)iv; return;
              }
          }
        }
        g_emit.op_parts_ival[0] = -2;
        return;
    }
    if (nd->op == IR_DET_CMP) {
        g_emit.op_sval = IR_LIT(nd).sval;
        const IR_t * l = bb_child0(nd);
        const IR_t * r = bb_child1(nd);
        const char * op = g_emit.op_sval;
        int arith = op && (!strcmp(op, "<") || !strcmp(op, ">") || !strcmp(op, ">=") || !strcmp(op, "=<") || !strcmp(op, "=:=") || !strcmp(op, "=\\="));
        int tcmp  = op && (!strcmp(op, "@<") || !strcmp(op, "@>") || !strcmp(op, "@=<") || !strcmp(op, "@>=") || !strcmp(op, "==") || !strcmp(op, "\\=="));
        g_emit.op_parts_n = 7;
        for (int i = 0; i < 7; i++) g_emit.op_parts_ival[i] = 0;
        if (!arith && !tcmp) { g_emit.op_parts_ival[0] = -1; return; }
        if (tcmp) {
            if (!l || !r) { g_emit.op_parts_ival[0] = -2; return; }
            g_emit.op_parts_ival[0] = 3;
            g_emit.op_parts_ival[1] = (l->op == IR_LOGICVAR) ? (int64_t)IR_LIT(l).ival : -1;
            g_emit.op_parts_ival[2] = (r->op == IR_LOGICVAR) ? (int64_t)IR_LIT(r).ival : -1;
            return;
        }
        if (!l || !r) { g_emit.op_parts_ival[0] = -2; return; }
        if (l->op == IR_LIT_I && r->op == IR_LIT_I) {
            long a = (long)IR_LIT(l).ival;
            long b = (long)IR_LIT(r).ival;
            int f = !strcmp(op, "<") ? (a < b) : !strcmp(op, ">") ? (a > b) : !strcmp(op, ">=") ? (a >= b) : !strcmp(op, "=<") ? (a <= b) : !strcmp(op, "=:=") ? (a == b) : (a != b);
            g_emit.op_parts_ival[0] = f ? 1 : 0;
            return;
        }
        g_emit.op_parts_ival[0] = 2;
        g_emit.op_parts_ival[1] = (l->op == IR_LOGICVAR) ? 1 : 0;
        g_emit.op_parts_ival[2] = (l->op == IR_LOGICVAR) ? (int64_t)IR_LIT(l).ival : 0;
        g_emit.op_parts_ival[3] = (l->op != IR_LOGICVAR && l->op == IR_LIT_I) ? (int64_t)IR_LIT(l).ival : 0;
        g_emit.op_parts_ival[4] = (r->op == IR_LOGICVAR) ? 1 : 0;
        g_emit.op_parts_ival[5] = (r->op == IR_LOGICVAR) ? (int64_t)IR_LIT(r).ival : 0;
        g_emit.op_parts_ival[6] = (r->op != IR_LOGICVAR && r->op == IR_LIT_I) ? (int64_t)IR_LIT(r).ival : 0;
        return;
    }
    if (nd->op == IR_DET_TYPE_TEST) {
        IR_t *a0 = bb_child0(nd);
        g_emit.op_parts_n = 1;
        g_emit.op_parts_ival[0] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        g_emit.op_parts_str[0]  = IR_LIT(nd).sval;
        return;
    }
    if (nd->op == IR_DET_FUNCTOR || nd->op == IR_DET_ARG || nd->op == IR_DET_UNIV) {
        IR_t *a0 = bb_child0(nd), *a1 = bb_child1(nd);
        IR_t *a2 = (nd->n_operands > 2) ? nd->operands[2] : NULL;
        g_emit.op_parts_n = 3;
        g_emit.op_parts_ival[0] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        g_emit.op_parts_ival[1] = a1 ? (int64_t)IR_LIT(a1).ival : -1;
        g_emit.op_parts_ival[2] = a2 ? (int64_t)IR_LIT(a2).ival : -1;
        return;
    }
    if (nd->op == IR_DET_FORMAT) {
        IR_t *a0 = bb_child0(nd);
        g_emit.op_parts_n = 2;
        g_emit.op_parts_str[0] = IR_LIT(nd).sval;
        g_emit.op_parts_ival[0] = (int64_t)IR_LIT(nd).ival;
        g_emit.op_parts_ival[1] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        return;
    }
    if (nd->op == IR_DET_SUCC_PLUS) {
        IR_t *a0 = bb_child0(nd), *a1 = bb_child1(nd);
        IR_t *a2 = (nd->n_operands > 2) ? nd->operands[2] : NULL;
        g_emit.op_parts_n = 4;
        g_emit.op_parts_ival[0] = (int64_t)IR_LIT(nd).ival;
        g_emit.op_parts_ival[1] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        g_emit.op_parts_ival[2] = a1 ? (int64_t)IR_LIT(a1).ival : -1;
        g_emit.op_parts_ival[3] = a2 ? (int64_t)IR_LIT(a2).ival : -1;
        return;
    }
    if (nd->op == IR_DET_ATOM_OP) {
        IR_t *a0 = bb_child0(nd), *a1 = bb_child1(nd);
        IR_t *a2 = (nd->n_operands > 2) ? nd->operands[2] : NULL;
        g_emit.op_parts_n = 4;
        g_emit.op_parts_str[0] = IR_LIT(nd).sval;
        g_emit.op_parts_ival[0] = (int64_t)IR_LIT(nd).ival;
        g_emit.op_parts_ival[1] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        g_emit.op_parts_ival[2] = a1 ? (int64_t)IR_LIT(a1).ival : -1;
        g_emit.op_parts_ival[3] = a2 ? (int64_t)IR_LIT(a2).ival : -1;
        return;
    }
    if (nd->op == IR_DET_CHAR_TYPE) {
        IR_t *a0 = bb_child0(nd), *a1 = bb_child1(nd);
        g_emit.op_parts_n = 2;
        g_emit.op_parts_ival[0] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        g_emit.op_parts_ival[1] = a1 ? (int64_t)IR_LIT(a1).ival : -1;
        return;
    }
    if (nd->op == IR_DET_SORT) {
        IR_t *a0 = bb_child0(nd), *a1 = bb_child1(nd);
        g_emit.op_parts_n = 3;
        g_emit.op_parts_ival[0] = IR_LIT(nd).ival;
        g_emit.op_parts_ival[1] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        g_emit.op_parts_ival[2] = a1 ? (int64_t)IR_LIT(a1).ival : -1;
        return;
    }
    if (nd->op == IR_DET_NUMBERVARS) {
        IR_t *a0 = bb_child0(nd), *a1 = bb_child1(nd), *a2 = (nd->n_operands > 2) ? nd->operands[2] : NULL;
        g_emit.op_parts_n = 3;
        g_emit.op_parts_ival[0] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        g_emit.op_parts_ival[1] = a1 ? (int64_t)IR_LIT(a1).ival : -1;
        g_emit.op_parts_ival[2] = a2 ? (int64_t)IR_LIT(a2).ival : -1;
        return;
    }
    if (nd->op == IR_DET_TERM_STRING) {
        IR_t *a0 = bb_child0(nd), *a1 = bb_child1(nd);
        g_emit.op_parts_n = 2;
        g_emit.op_parts_ival[0] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        g_emit.op_parts_ival[1] = a1 ? (int64_t)IR_LIT(a1).ival : -1;
        return;
    }
    if (nd->op == IR_DET_COPY_TERM) {
        IR_t *a0 = bb_child0(nd), *a1 = bb_child1(nd);
        g_emit.op_parts_n = 2;
        g_emit.op_parts_ival[0] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        g_emit.op_parts_ival[1] = a1 ? (int64_t)IR_LIT(a1).ival : -1;
        return;
    }
    if (nd->op == IR_DET_NB_SETVAL || nd->op == IR_DET_NB_GETVAL) {
        IR_t *a0 = bb_child0(nd), *a1 = bb_child1(nd);
        g_emit.op_parts_n = 2;
        g_emit.op_parts_ival[0] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        g_emit.op_parts_ival[1] = a1 ? (int64_t)IR_LIT(a1).ival : -1;
        g_emit.op_gva_k = (g_gva_active && IR_LIT(nd).sval) ? gva_index_of(IR_LIT(nd).sval) : -1;
        return;
    }
    if (nd->op == IR_DET_RETRACT) {
        IR_t *a0 = bb_child0(nd);
        g_emit.op_parts_n = 1;
        g_emit.op_parts_ival[0] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        return;
    }
    if (nd->op == IR_DET_ABOLISH) {
        IR_t *a0 = bb_child0(nd), *a1 = bb_child1(nd);
        g_emit.op_parts_n = 2;
        g_emit.op_parts_ival[0] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        g_emit.op_parts_ival[1] = a1 ? (int64_t)IR_LIT(a1).ival : -1;
        return;
    }
    if (nd->op == IR_DET_ASSERTZ) {
        IR_t *a0 = bb_child0(nd);
        g_emit.op_parts_n = 1;
        g_emit.op_parts_ival[0] = a0 ? (int64_t)IR_LIT(a0).ival : -1;
        g_emit.op_parts_ival[1] = (int64_t)IR_LIT(nd).ival;
        return;
    }
    if (nd->op == IR_CELL_DYNITER) {
        g_emit.bb_zn = (void *)nd;
        return;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int bb_arith_dyn_kind(IR_t *o) { return o && (o->op == IR_CALL || ir_is_call_kind(o->op) || o->op == IR_IDX); }
static int bb_arith_materializable(IR_t *o) {
    return o && (o->op == IR_CALL || ir_is_call_kind(o->op) || o->op == IR_IDX || o->op == IR_LIT_I || (o->op == IR_VAR && IR_LIT(o).sval));
}
int bb_arith_is_dynamic(IR_t *nd) {
    if (!nd || nd->op != IR_BINOP) return 0;
    int64_t o = IR_LIT(nd).ival;
    if (!(o == BINOP_ADD || o == BINOP_SUB || o == BINOP_MUL || o == BINOP_DIV || o == BINOP_MOD)) return 0;
    IR_t *a = bb_child0(nd), *b = bb_child1(nd);
    int adyn = bb_arith_dyn_kind(a), bdyn = bb_arith_dyn_kind(b);
    if (!adyn && !bdyn) return 0;
    if (!bb_arith_materializable(a) || !bb_arith_materializable(b)) return 0;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int arith_emits_descr(IR_t *o) {
    if (!o || o->op != IR_BINOP || !bb_arith_is_dynamic(o)) return 0;
    IR_t *a = bb_child0(o), *b = bb_child1(o);
    int a_ok = a && (a->op == IR_CALL || ir_is_call_kind(a->op) || a->op == IR_IDX || arith_emits_descr(a));
    int b_ok = b && (b->op == IR_CALL || ir_is_call_kind(b->op) || b->op == IR_IDX || arith_emits_descr(b));
    return a_ok && b_ok;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_e binop_slot_kind(IR_t *nd) {
    int64_t op = nd ? IR_LIT(nd).ival : -1;
    if ((op >= BINOP_LT && op <= BINOP_NE) || (op >= BINOP_SLT && op <= BINOP_SNE)) return IR_BINOP_RELOP;
    if (op == BINOP_CONCAT)               return IR_BINOP_CONCAT;
    if (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD || op == BINOP_POW) return IR_BINOP_ARITH;
    if (op == BINOP_CUNION || op == BINOP_CDIFF || op == BINOP_CINTER) return IR_BINOP_ARITH;
    return IR_BINOP;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int descr_binop_opnd_slot(IR_t *o) {
    return (o && o->op != IR_LIT_F && o->op != IR_LIT_NUL) ? bb_slot_get(o) : -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int binop_operand_real_static(IR_graph_t *g, IR_t *o, int depth);
static int var_assigned_real_static(IR_graph_t *g, const char *name, int depth) {
    if (!g || !g->all || !name || depth > 8) return 0;
    for (int i = 0; i < g->n; i++) {
        IR_t *m = g->all[i];
        if (!m || m->op != IR_ASSIGN || !IR_LIT(m).sval || strcmp(IR_LIT(m).sval, name)) continue;
        IR_t *rhs = (m->n_operands > 0) ? m->operands[0] : bb_child0(m);
        if (rhs && binop_operand_real_static(g, rhs, depth + 1)) return 1;
    }
    return 0;
}
static int binop_operand_real_static(IR_graph_t *g, IR_t *o, int depth) {
    if (!o || depth > 8) return 0;
    if (o->op == IR_LIT_F) return 1;
    if (o->op == IR_VAR && IR_LIT(o).sval) return var_assigned_real_static(g, IR_LIT(o).sval, depth);
    if (o->op == IR_BINOP) {
        IR_t *c0 = bb_child0(o), *c1 = bb_child1(o);
        int na = 0; IR_t * const *aux = (g && !c0) ? bb_operand_aux_get(g, o, &na) : (IR_t * const *)0;
        if (aux && na >= 2) { c0 = aux[0]; c1 = aux[1]; }
        return binop_operand_real_static(g, c0, depth + 1) || binop_operand_real_static(g, c1, depth + 1);
    }
    if (o->op == IR_ALT) {
        for (int k = 0; k < o->n_operands; k++) if (binop_operand_real_static(g, o->operands[k], depth + 1)) return 1;
    }
    return 0;
}
int binop_is_num_real(IR_graph_t *g, IR_t *nd) {
    if (!nd) return 0;
    int64_t op = IR_LIT(nd).ival;
    if (op == BINOP_POW) return 1;
    if (op == BINOP_CUNION || op == BINOP_CDIFF || op == BINOP_CINTER) return 1;
    int is_num = (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD || (op >= BINOP_LT && op <= BINOP_NE));
    if (!is_num) return 0;
    return binop_operand_real_static(g, bb_child0(nd), 0) || binop_operand_real_static(g, bb_child1(nd), 0);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/* Emit e's value-operands (e.g. the from/to bounds of an IR_TO) so they get frame slots before e itself is walked.
   For a single-node value producer (literal, var) e has no operands and this is a no-op.  Mirrors case_slot_binop_operands. */
/*--------------------------------------------------------------------------------------------------------------------*/
/* IR_REPALT — Icon repeated alternation `|e`.  The sub-expression e is the lowerer-set operand[0], driven INTERNALLY
   here (it is NOT on the main spine), so flat_drive_repalt owns all four of e's edges.  Modelled on JCON ir_a_RepAlt:
   a one-bit `yielded` flag (frame slot at off+16, like bb_limit's counter) toggles JCON's MoveLabel/IndirectGoto.
     restart (= REPALT α / fresh start): yielded:=0; (re-)evaluate e's bounds; run e.
     e succeeds:  copy e's value into the REPALT slot; yielded:=1; jmp γ (yield).
     e fails:     if yielded, jmp restart (re-run e from scratch — the infinite repeat); else jmp ω (|e produced nothing).
     REPALT β (consumer resume): jmp e-β (pump e for its next value, no bounds re-eval).
   This yields e's value-sequence over and over, but fails immediately if a fresh start of e produces nothing. */
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int to_inner_gen_operand_k(IR_t *gi, IR_t **nodes, int n) {
    int bk = -1;
    if (gi->op != IR_TO && gi->op != IR_TO_BY) return -1;
    for (int oi = 0; oi < gi->n_operands; oi++) for (int k = 0; k < n; k++) if (nodes[k] == (IR_t *)gi->operands[oi] && ir_is_generator_kind(nodes[k]->op) && k > bk) bk = k;
    return bk;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_call_write_route(IR_t *nd) {
    const char *fn = IR_LIT(nd).sval; int64_t narg = IR_LIT(nd).ival; IR_t *a0 = ir_call_arg(nd, 0);
    if (!(fn && !strcmp(fn, "write") && narg == 1 && a0)) return 0;
    if (g_descr_flat_chain && bb_slot_get(a0) >= 0) return 1;
    int wintexpr = (a0->op == IR_BINOP || a0->op == IR_LIT_I || a0->op == IR_TO || a0->op == IR_TO_BY || a0->op == IR_ALT || a0->op == IR_VAR || a0->op == IR_NEG || a0->op == IR_POS || a0->op == IR_NONNULL || a0->op == IR_NULL_TEST || a0->op == IR_NOT || a0->op == IR_SIZE || a0->op == IR_CALL || ir_is_call_kind(a0->op) || a0->op == IR_CASE || a0->op == IR_FIELD_GET || a0->op == IR_LIST_BANG || a0->op == IR_KEY_GEN || a0->op == IR_LIMIT || a0->op == IR_IDX);
    if (wintexpr && (a0->op == IR_BINOP || a0->op == IR_TO || a0->op == IR_TO_BY)) return (a0->op == IR_BINOP && IR_LIT(a0).ival == BINOP_CONCAT) ? 2 : 3;
    if (wintexpr) return 4;
    if (a0->op == IR_LIT_S && IR_LIT(a0).sval) return 5;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_call_route_classify(IR_t * nd) {
    const char * fn = g_emit.op_sval ? g_emit.op_sval : ""; int64_t narg = g_emit.op_ival; IR_t * a0 = ir_call_arg(nd, 0); double dv = g_emit.op_dval;
    IR_e k = nd ? nd->op : IR_CALL;
    /* op-field call-kind (resolved at pre-emit time from the proc/builtin name tables, no dval tag) wins first */
    if (k == IR_CALL_BUILTIN && fn[0] && rt_builtin_is_generator(fn)) return CALL_ROUTE_BYNAME;
    if (g_descr_flat_chain && k == IR_CALL_PROC_STAGED) return CALL_ROUTE_PROC_STAGED;
    if (g_gvar_flat_chain && k == IR_CALL_GVAR_USERPROC) return CALL_ROUTE_GVAR_USERPROC;
    if (k == IR_CALL_BUILTIN && g_emit.op_write_route == 0 && fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_FN;
    if (g_descr_flat_chain && dv == 2.0 && fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_BYNAME;
    if (g_descr_flat_chain && dv == 2.0 && !strcmp(fn, "__rk_bool")) return CALL_ROUTE_RK_BOOL_COND;
    /* A user-defined generator proc that happens to share a builtin name (e.g. `upto`) must route as a
       proc generator, not the builtin — registered+generator wins over rt_builtin_is_generator below. */
    if (g_descr_flat_chain && (dv == 2.0 || dv == 3.0) && fn[0] && rt_proc_is_registered(fn) && rt_proc_is_generator(fn)) return CALL_ROUTE_PROC_STAGED;
    if (g_descr_flat_chain && (dv == 2.0 || dv == 3.0) && fn[0] && rt_builtin_is_generator(fn)) return CALL_ROUTE_BYNAME;
    if (g_descr_flat_chain && dv == 2.0) return CALL_ROUTE_DVAL2_BOMB;
    if (g_gvar_flat_chain && (dv == 2.0 || dv == 3.0) && fn[0] && rt_proc_is_registered(fn)) return CALL_ROUTE_GVAR_USERPROC;
    if (g_descr_flat_chain && fn[0] && rt_proc_is_registered(fn)) return CALL_ROUTE_PROC_STAGED;
    if (g_gvar_flat_chain && dv == 3.0 && fn[0] && !rt_proc_is_registered(fn)) return CALL_ROUTE_BYNAME;
    if (g_gvar_flat_chain && dv == 2.0 && fn[0] && !rt_proc_is_registered(fn) && !rt_builtin_is_known(fn)) return CALL_ROUTE_BYNAME;
    if (g_descr_flat_chain && !strcmp(fn, "__rk_bool") && dv == 0.0 && narg == 1 && a0 && bb_slot_get(a0) >= 0) return CALL_ROUTE_RK_BOOL_SLOT;
    switch (g_emit.op_write_route) { case 1: return CALL_ROUTE_WRITE_SLOT; case 2: case 3: return CALL_ROUTE_WRITE_BINOP; case 4: return CALL_ROUTE_WRITE_LEGACY; case 5: return CALL_ROUTE_WRITE_EMPTY; default: break; }
    if (fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_FN;
    return CALL_ROUTE_FATAL;
}
/*--------------------------------------------------------------------------------------------------------------------*/void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "walk_bb_flat"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int ir_node_is_alt_arm(IR_t *nd) {
    if (!nd || !g_emit_cfg) return 0;
    if (!(nd->γ.node && (nd->γ.node->op == IR_ALT || nd->γ.node->op == IR_MATCH_ALT))) return 0;
    int na = 0;
    IR_t * const * arms = bb_operand_aux_get(g_emit_cfg, nd->γ.node, &na);
    for (int i = 0; i < na && arms; i++) if (arms[i] == nd) return 1;
    return 0;
}
static IR_t *ir_skip_alt_arms(IR_t *entry) {
    int guard = 0;
    while (entry && ir_node_is_alt_arm(entry) && guard++ < 512) entry = entry->γ.node;
    return entry;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*====================================================================================================================*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int codegen_flat_chain_body(IR_t *entry, const char *prefix) {
    bb_label_t lbl_α, lbl_α_body, lbl_γ, lbl_ω, lbl_β;
    emit_label_initf(&lbl_α,      "%s_α",      prefix);
    emit_label_initf(&lbl_α_body, "%s_α_body", prefix);
    emit_label_initf(&lbl_γ,       "%s_γ",      prefix);
    emit_label_initf(&lbl_ω,       "%s_ω",      prefix);
    emit_label_initf(&lbl_β,       "%s_β",       prefix);
    int text_externalise = g_is_text ? 1 : 0;
    if (text_externalise) data_buf_reset();
    g_emit.flat_lbl_α        = lbl_α.name;
    g_emit.flat_lbl_α_body   = lbl_α_body.name;
    g_emit.flat_lbl_γ         = lbl_γ.name;
    g_emit.flat_lbl_ω         = lbl_ω.name;
    g_emit.flat_lbl_β         = lbl_β.name;
    g_emit.flat_β_p           = &lbl_β;
    g_emit.flat_succ_p        = &lbl_γ;
    g_emit.flat_fail_p        = &lbl_ω;
    g_emit.flat_text_externalise = text_externalise;
    if (text_externalise && g_is_text) emit_label_define_bb(&lbl_α);
    xa_dispatch(XA_FLAT_PROLOGUE);
    if (g_is_text) g_emit_pos += 7;
    emit_label_define_bb(&lbl_α_body);
    enum { CH_MAX = 512 };
    IR_t *nodes[CH_MAX]; int n = 0;
    IR_t *queue[CH_MAX]; int qh = 0, qt = 0;
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL) && entry->γ.node && guard++ < CH_MAX) entry = entry->γ.node; }
    entry = ir_skip_alt_arms(entry);
    queue[qt++] = entry;
    while (qh < qt) {
        IR_t *c = queue[qh++];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        if (ir_node_is_alt_arm(c)) continue;
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); }
        nodes[n++] = c;
        if (c->γ.node && qt < CH_MAX) queue[qt++] = ir_skip_alt_arms(c->γ.node);
        if ((c->op == IR_BINOP) && c->ω.node && qt < CH_MAX) queue[qt++] = ir_skip_alt_arms(c->ω.node);
        if ((c->op == IR_CALL || ir_is_call_kind(c->op) || c->op == IR_CALL_DEFINE || c->op == IR_PROC_GEN) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if (c->op == IR_GATHER && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if ((c->op == IR_MAP || c->op == IR_GREP) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if (c->op == IR_SUSPEND && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = (IR_t *)c->operands[1];
    }
    for (int i = 0; i < n; i++) if (ir_is_generator_kind(nodes[i]->op) && nodes[i]->ω.node) { int present = 0; for (int j = 0; j < n; j++) if (nodes[j] == nodes[i]->ω.node) { present = 1; break; } if (!present && qt < CH_MAX) queue[qt++] = nodes[i]->ω.node; }
    while (qh < qt) {
        IR_t *c = queue[qh++];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        if (ir_node_is_alt_arm(c)) continue;
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); }
        nodes[n++] = c;
        if (c->γ.node && qt < CH_MAX) queue[qt++] = ir_skip_alt_arms(c->γ.node);
        if ((c->op == IR_BINOP) && c->ω.node && qt < CH_MAX) queue[qt++] = ir_skip_alt_arms(c->ω.node);
        if ((c->op == IR_CALL || ir_is_call_kind(c->op) || c->op == IR_CALL_DEFINE || c->op == IR_PROC_GEN) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if (c->op == IR_GATHER && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if ((c->op == IR_MAP || c->op == IR_GREP) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if (ir_is_generator_kind(c->op) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if (c->op == IR_SUSPEND && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = (IR_t *)c->operands[1];
    }
    { extern int is_global(const char *); for (int i = 0; i < n; i++) { IR_t *c = nodes[i]; if (c && (c->op == IR_ASSIGN || c->op == IR_ASSIGN_LIT_S || c->op == IR_ASSIGN_LIT_I || c->op == IR_ASSIGN_VAR || c->op == IR_ASSIGN_CONCAT || c->op == IR_ASSIGN_CALL) && IR_LIT(c).sval && !is_global(IR_LIT(c).sval)) (void)bb_varslot(IR_LIT(c).sval); if (c && c->op == IR_RASGN && c->n_operands > 0 && c->operands[0] && c->operands[0]->op == IR_VAR && IR_LIT(c->operands[0]).sval && !is_global(IR_LIT(c->operands[0]).sval)) (void)bb_varslot(IR_LIT(c->operands[0]).sval); } }
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    for (int i = 0; i < n && g_flat_chain_set_n < FLAT_CHAIN_SET_MAX; i++) g_flat_chain_set[g_flat_chain_set_n++] = nodes[i];
    int id = g_flat_node_id++;
    for (int i = 0; i < n; i++) {
        lbls[i]  = emit_label_alloc("xchain%d_n%d_α", id, i);
        betas[i] = emit_label_alloc("xchain%d_n%d_β", id, i);
    }
    for (int i = 0; i < n; i++) if (nodes[i]->op == IR_LIMIT) {
        int loff = bb_slot_alloc16_or_get(nodes[i]);
        (void)bb_slot_claim(8);
        bb_emit_limit_init(loff);
    }
    for (int i = 0; i < n; i++) {
        emit_label_define_bb(lbls[i]);
        bb_label_t *node_γ = &lbl_γ;
        bb_label_t *node_ω = &lbl_ω;
        IR_t *gtgt = ir_skip_alt_arms(nodes[i]->γ.node);
        IR_t *otgt = ir_skip_alt_arms(nodes[i]->ω.node);
        for (int k = 0; k < n; k++) if (nodes[k] == gtgt) {
            node_γ = (i > k && ir_is_generator_kind(nodes[k]->op)) ? betas[k] : lbls[k];
            break;
        }
        if (nodes[i]->γ.node == NULL || nodes[i]->γ.node->op == IR_SUCCEED) node_γ = &lbl_γ;
        if (nodes[i]->γ.node && nodes[i]->γ.node->op == IR_FAIL) node_γ = &lbl_ω;
        if (nodes[i]->op == IR_EVERY) { for (int k = 0; k < n; k++) if (nodes[k] == (IR_t *)(nodes[i]->n_operands > 0 ? nodes[i]->operands[0] : NULL)) { node_γ = lbls[k]; break; } }
        int omega_resolved = 0; int omega_k = -1;
        for (int k = 0; k < n; k++) if (nodes[k] == otgt) { node_ω = (i > k && ir_is_generator_kind(nodes[k]->op)) ? betas[k] : lbls[k]; omega_resolved = 1; omega_k = k; break; }
        if (!omega_resolved) node_ω = &lbl_ω;
        if (omega_resolved && omega_k >= 0 && i > omega_k && !ir_is_generator_kind(nodes[omega_k]->op) && nodes[omega_k]->op == IR_BINOP) { IR_t *bw = nodes[omega_k]->ω.node; if (bw) for (int g = 0; g < n; g++) if (nodes[g] == bw && ir_is_generator_kind(nodes[g]->op) && i > g) { node_ω = betas[g]; break; } }
        if (omega_resolved && nodes[i]->ω.node && nodes[i]->ω.node->op == IR_EVERY) {
            if (ir_is_generator_kind(nodes[i]->op)) { node_ω = lbls[omega_k]; int bk = to_inner_gen_operand_k(nodes[i], nodes, n); if (bk >= 0) node_ω = betas[bk]; }
            else { for (int gk = 0; gk < n; gk++) if (ir_is_generator_kind(nodes[gk]->op)) node_ω = betas[gk]; }
        }
        g_limit_gen_beta = NULL;
        if (nodes[i]->op == IR_LIMIT && nodes[i]->n_operands > 0) for (int k = 0; k < n; k++) if (nodes[k] == (IR_t *)nodes[i]->operands[0]) { g_limit_gen_beta = betas[k]; break; }
        g_suspend_dobody_beta = NULL;
        if (nodes[i]->op == IR_SUSPEND && nodes[i]->n_operands > 1) for (int k = 0; k < n; k++) if (nodes[k] == (IR_t *)nodes[i]->operands[1]) { g_suspend_dobody_beta = lbls[k]; break; }
        emit_drive(nodes[i], node_γ, node_ω, betas[i]);
    }
    emit_label_define_bb(&lbl_β);
    { bb_label_t *resume_tgt = &lbl_ω;
      for (int i = 0; i < n; i++) if (nodes[i]->op == IR_SUSPEND) { resume_tgt = betas[i]; break; }
      emit_jmp_label(resume_tgt, JMP_JMP); }
    emit_label_define_bb(&lbl_γ);
    xa_dispatch(XA_FLAT_EPILOGUE);
    if (text_externalise && g_is_text) {
        data_buf_flush_pending_label();
        xa_dispatch(XA_FLAT_DATA_SECTION);
        data_buf_reset();
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* Drive nd via walk_bb_flat, but if nd is the first arm of a γ/ω-threaded inline alt
   chain (PAT_ALT with no operand_aux), drive all arms with cascading ω-labels. */
/*--------------------------------------------------------------------------------------------------------------------*/
static int g_in_prebuild = 0;
static int g_text_child_counter = 0;static void pre_build_children_text(IR_t *nd, FILE *out, const char *base_prefix) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "pre_build_children_text"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/static void pre_build_children(IR_t *nd) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "pre_build_children"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/static void scan_set_subj_node(IR_t *n, IR_t *s) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "scan_set_subj_node"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
static int descr_chain_arity(const IR_t *n) {
    switch (n->op) {
    case IR_LIT_I: case IR_LIT_S: case IR_LIT_F: case IR_LIT_NUL:
    case IR_VAR:   case IR_KEYWORD: case IR_VAR_FRAME: case IR_VAR_FRAME_REF: return 0;
    case IR_FIELD_GET: return 1;
    case IR_FIELD_SET: return 0;
    case IR_SECTION: return 0;
    case IR_LIST_BANG: return 0;
    case IR_KEY_GEN: return 0;
    case IR_CASE: return 0;
    case IR_ALT:   return 0;
    case IR_REPALT: return 0;   /* |e — sub-expression is the lowerer-set operand[0], driven internally by flat_drive_repalt; result slot is REPALT's own (e's value copied in at yield) */
    case IR_GATHER: return 0;
    case IR_MAP: case IR_GREP: return 0;
    case IR_GEN_SCAN: return 0;
    case IR_BINOP: case IR_TO: case IR_TO_BY: return 2;
    case IR_LIMIT: return 0;   /* push LIMIT result (so a consumer wires its arg to us) without rewriting our lowerer-set operands [generator, count, gen-entry]; the generator stays on-spine */
    case IR_CONJ:  return 0;   /* (e1;..;en) value = last conjunct's value; CONJ pushes its result (consumer wires to it) and keeps its lowerer-set operand [last-conjunct value-node]; slot aliased to that conjunct at emit */
    case IR_IDX_SET: return 3;
    case IR_UNOP:  case IR_NEG: case IR_POS: case IR_NONNULL: case IR_NOT: case IR_SIZE: return 1;
    case IR_ASSIGN: case IR_ASSIGN_LIT_S: case IR_ASSIGN_LIT_I: case IR_ASSIGN_VAR: case IR_ASSIGN_CONCAT: case IR_ASSIGN_CALL: case IR_ASSIGN_FRAME: case IR_ASSIGN_FRAME_REF: return 1;
    case IR_INDIRECT_ASSIGN_LIT_S: return 1;
    case IR_INDIRECT_ASSIGN_VAR:   return 1;
    case IR_RETURN: return 1;
    case IR_CALL_DEFINE: return 0;
    case IR_SCAN_POS: case IR_SCAN_ANY: case IR_SCAN_MATCH: case IR_SCAN_MANY: case IR_SCAN_TAB: case IR_SCAN_MOVE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL:
    case IR_CALL_PROC_STAGED: case IR_CALL_USERPROC: case IR_CALL_BYNAME: case IR_CALL_BUILTIN: case IR_CALL_GVAR_USERPROC:
    case IR_CALL:  return n->n_operands;
    case IR_PROC_GEN: return 0;
    case IR_PATTERN_LIT: return 0;
    case IR_PATTERN_LEN: case IR_PATTERN_POS: case IR_PATTERN_RPOS: case IR_PATTERN_TAB: case IR_PATTERN_RTAB: return 0;
    case IR_PATTERN_ANY: case IR_PATTERN_NOTANY: case IR_PATTERN_SPAN: case IR_PATTERN_BREAK: case IR_PATTERN_BREAKX: return 0;
    case IR_PATTERN_FAIL: case IR_PATTERN_REM: case IR_PATTERN_SUCCEED: case IR_PATTERN_FENCE: case IR_PATTERN_ABORT: case IR_PATTERN_ARB: return 0;
    case IR_PATTERN_CAT: return 2;
    case IR_PATTERN_ALT: return 2;
    case IR_DTP_ASSIGN:  return 1;
    case IR_GOTO_DYN:    return 0;
    default:       return -1;
    }
}
static void descr_chain_operand_refs(IR_t *entry) {
    IR_t *chain[512]; int nc = 0;
    IR_t *seen[512]; int ns = 0;
    IR_t *stkv[512]; int sv = 0;
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL) && entry->γ.node && guard++ < 512) entry = entry->γ.node; }
    entry = ir_skip_alt_arms(entry);
    stkv[sv++] = entry;
    while (sv > 0 && nc < 512) {
        IR_t *c = stkv[--sv];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        if (ir_node_is_alt_arm(c)) continue;
        int dup = 0; for (int i = 0; i < ns; i++) if (seen[i] == c) { dup = 1; break; }
        if (dup) continue;
        seen[ns++] = c; chain[nc++] = c;
        if ((c->op == IR_BINOP) && c->ω.node && sv < 512) stkv[sv++] = ir_skip_alt_arms(c->ω.node);
        if ((c->op == IR_CALL || ir_is_call_kind(c->op) || c->op == IR_CALL_DEFINE) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if (c->op == IR_GATHER && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if ((c->op == IR_MAP || c->op == IR_GREP) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if (c->op == IR_SUSPEND && c->n_operands > 1 && c->operands[1] && sv < 512) stkv[sv++] = (IR_t *)c->operands[1];
        if (c->γ.node && sv < 512) stkv[sv++] = ir_skip_alt_arms(c->γ.node);
    }
    for (int i = 0; i < nc; i++) if (ir_is_generator_kind(chain[i]->op) && chain[i]->ω.node) { int present = 0; for (int j = 0; j < ns; j++) if (seen[j] == chain[i]->ω.node) { present = 1; break; } if (!present && sv < 512) stkv[sv++] = chain[i]->ω.node; }
    while (sv > 0 && nc < 512) {
        IR_t *c = stkv[--sv];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        if (ir_node_is_alt_arm(c)) continue;
        int dup = 0; for (int i = 0; i < ns; i++) if (seen[i] == c) { dup = 1; break; }
        if (dup) continue;
        seen[ns++] = c; chain[nc++] = c;
        if ((c->op == IR_BINOP) && c->ω.node && sv < 512) stkv[sv++] = ir_skip_alt_arms(c->ω.node);
        if ((c->op == IR_CALL || ir_is_call_kind(c->op) || c->op == IR_CALL_DEFINE) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if (c->op == IR_GATHER && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if ((c->op == IR_MAP || c->op == IR_GREP) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if (ir_is_generator_kind(c->op) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if (c->op == IR_SUSPEND && c->n_operands > 1 && c->operands[1] && sv < 512) stkv[sv++] = (IR_t *)c->operands[1];
        if (c->γ.node && sv < 512) stkv[sv++] = ir_skip_alt_arms(c->γ.node);
    }
    IR_t *stk[512]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = descr_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        if (ar == 1 && n->op == IR_SCAN && sp >= 1) { scan_set_subj_node(n, stk[sp - 1]); sp -= 1; }
        else if (ar >= 1 && sp >= ar) { if (n->n_operands < ar) { n->n_operands = 0; for (int k = ar; k >= 1; k--) ir_operand_push(n, stk[sp - k]); } sp -= ar; }
        else if (ar >= 1) { sp = 0; }
        stk[sp++] = n;
    }
}
void resolve_call_kinds_descr(IR_graph_t *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) { IR_t *nd = g->all[i]; if (!nd) continue; int iscall = (nd->op == IR_CALL || nd->op == IR_CALL_DEFINE || ir_is_call_kind(nd->op));
        if (nd->op == IR_CALL) { const char *fn = IR_LIT(nd).sval;
            if (fn && fn[0] && rt_proc_is_registered(fn) && rt_proc_is_generator(fn)) nd->op = IR_PROC_GEN;
            else if (fn && fn[0] && rt_proc_is_registered(fn)) nd->op = IR_CALL_PROC_STAGED;
            else if (fn && fn[0] && rt_builtin_is_generator(fn)) nd->op = IR_CALL_BUILTIN;
            else if (fn && fn[0] && strcmp(fn, "write") && strcmp(fn, "writes") && rt_builtin_is_known(fn)) nd->op = IR_CALL_BUILTIN; }
        if (iscall && (IR_LIT(nd).dval == 2.0 || IR_LIT(nd).dval == 3.0 || IR_LIT(nd).dval == 5.0)) { IR_graph_t **bk = (IR_graph_t **)0;
            if (bk) for (int j = 0; j < (int) IR_LIT(nd).ival; j++) if (bk[j]) resolve_call_kinds_descr(bk[j]); } }
}void resolve_call_kinds_gvar(IR_graph_t *g) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "resolve_call_kinds_gvar"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
bb_box_fn descr_flat_chain_build(IR_t *entry) {
    if (!entry) return NULL;
    descr_chain_operand_refs(entry);
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_slot_count = 0; g_flat_node_id = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    g_flat_chain_set_n = 0;
    g_descr_flat_chain = 1;
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_flat_chain_body(entry, "pat_flat");
    int nbytes = emitter_end();
    g_descr_flat_chain = 0;
    extern int bb_emit_overflow;
    if (bb_emit_overflow || nbytes <= 0 || nbytes > FLAT_BUF_MAX) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    bb_seal(buf, (size_t)nbytes);
    bb_pool_trim_last(buf, FLAT_BUF_MAX, (size_t)nbytes);
    return (bb_box_fn)buf;
}int descr_flat_chain_build_text(IR_t *entry, FILE *out, const char *prefix) {
    if (!entry) return 1;
    descr_chain_operand_refs(entry);
    g_flat_slot_count = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    g_flat_chain_set_n = 0;
    g_descr_flat_chain = 1;
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    int rc = codegen_flat_chain_body(entry, prefix);
    emitter_end();
    g_descr_flat_chain = 0;
    return rc;
}
bb_box_fn descr_flat_chain_build_proc(IR_t *entry, const char **pnames, int np) {
    if (!entry) return NULL;
    descr_chain_operand_refs(entry);
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_slot_count = 0; g_flat_node_id = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    g_descr_flat_chain = 1;
    g_flat_slot_count = 16;
    for (int i = 0; i < np && pnames; i++) if (pnames[i]) (void)bb_varslot(pnames[i]);
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_flat_chain_body(entry, "proc_flat");
    int nbytes = emitter_end();
    g_descr_flat_chain = 0;
    extern int bb_emit_overflow;
    if (bb_emit_overflow || nbytes <= 0 || nbytes > FLAT_BUF_MAX) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    bb_seal(buf, (size_t)nbytes);
    bb_pool_trim_last(buf, FLAT_BUF_MAX, (size_t)nbytes);
    return (bb_box_fn)buf;
}
/*--------------------------------------------------------------------------------------------------------------------*/int descr_flat_chain_build_proc_text(IR_t *entry, const char **pnames, int np, FILE *out, const char *pname) {
    if (!entry || !out || !pname) return 1;
    descr_chain_operand_refs(entry);
    g_flat_slot_count = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    g_descr_flat_chain = 1;
    g_flat_slot_count = 16;
    for (int i = 0; i < np && pnames; i++) if (pnames[i]) (void)bb_varslot(pnames[i]);
    char prefix[256];
    snprintf(prefix, sizeof(prefix), "proc_%s", pname);
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    fprintf(out, "  .globl %s_\316\261\n", prefix);
    int rc = codegen_flat_chain_body(entry, prefix);
    emitter_end();
    g_descr_flat_chain = 0;
    return rc;
}
/*====================================================================================================================*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/bb_box_fn gvar_flat_chain_build(IR_graph_t *g) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gvar_flat_chain_build"); abort(); }int gvar_flat_chain_build_text(IR_graph_t *g, FILE *out, const char *prefix) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gvar_flat_chain_build_text"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/bb_box_fn gvar_flat_chain_build_at(IR_graph_t *g, IR_t *entry_node, const char *prefix) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gvar_flat_chain_build_at"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/void gva_collect_graph(IR_graph_t *g) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gva_collect_graph"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
void gva_collect_icon_globals(void) {
    extern const char *global_names[]; extern int global_count;
    for (int i = 0; i < global_count; i++) if (global_names[i]) (void)gva_collect_var(global_names[i]);
}
/*--------------------------------------------------------------------------------------------------------------------*/int gvar_flat_chain_build_text_at(IR_graph_t *g, IR_t *entry_node, FILE *out, const char *prefix) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gvar_flat_chain_build_text_at"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/bb_box_fn pl_gz_build(IR_t *gz_root) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "pl_gz_build"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/int pl_gz_codegen(IR_t *gz_root, FILE *out, const char *prefix) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "pl_gz_codegen"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/bb_box_fn bb_build_flat(IR_t *nd) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_build_flat"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/int codegen_flat_build(IR_t *nd, FILE *out, const char *prefix) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "codegen_flat_build"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
#define PL_CATCH_MAX 64
static IR_t *g_pl_catch_nodes[PL_CATCH_MAX];
static int   g_pl_catch_n = 0;

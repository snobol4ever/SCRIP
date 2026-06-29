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
int bb_slot_alloc(IR_t *nd) {
    int off = g_flat_slot_count;
    g_flat_slot_count += 8;
    bb_slotmap_push(nd, off);
    return off;
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
int bb_slot_alloc24(IR_t *nd) {
    int off = g_flat_slot_count;
    g_flat_slot_count += 24;
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
static int flat_chain_set_has(IR_t *nd) {
    for (int i = 0; i < g_flat_chain_set_n; i++) if (g_flat_chain_set[i] == nd) return 1;
    return 0;
}
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
static bb_box_fn child_cache_get(IR_t *p) {
    for (int i = 0; i < g_child_cache_n; i++) if (g_child_cache[i].key == p) return g_child_cache[i].fn;
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
const char *child_cache_get_lbl(bb_box_fn fn) {
    for (int i = 0; i < g_child_cache_n; i++) if (g_child_cache[i].fn == fn && g_child_cache[i].text_lbl[0]) return g_child_cache[i].text_lbl;
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void child_cache_put(IR_t *p, bb_box_fn fn) {
    if (g_child_cache_n < CHILD_CACHE_MAX) { g_child_cache[g_child_cache_n].key = p; g_child_cache[g_child_cache_n].fn = fn; g_child_cache[g_child_cache_n].text_lbl[0] = '\0'; g_child_cache_n++; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void child_cache_set_lbl(bb_box_fn fn, const char *lbl) {
    for (int i = 0; i < g_child_cache_n; i++) if (g_child_cache[i].fn == fn) { snprintf(g_child_cache[i].text_lbl, 80, "%s", lbl ? lbl : ""); return; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int    g_flat_data_any    = 0;
static int    g_flat_data_just_closed = 0;
static char   g_flat_data_pending_lbl[160] = "";
static char   g_flat_data_block_lbls[FLAT_DATA_LBL_MAX][96];
static int    g_flat_data_block_nlbls = 0;
void data_buf_reset(void) {
    g_flat_data_len = 0;
    g_flat_data_active = 0;
    g_flat_data_any = 0;
    g_flat_data_just_closed = 0;
    g_flat_data_block_nlbls = 0;
    g_flat_data_pending_lbl[0] = '\0';
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void data_buf_appendf(const char *fmt, ...) {
    if (g_flat_data_len >= FLAT_DATA_BUF_MAX) return;
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(g_flat_data_buf + g_flat_data_len, FLAT_DATA_BUF_MAX - g_flat_data_len, fmt, ap);
    va_end(ap);
    if (n > 0) {
        size_t left = FLAT_DATA_BUF_MAX - g_flat_data_len;
        g_flat_data_len += ((size_t)n < left) ? (size_t)n : left;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void data_buf_flush_pending_label(void) {
    if (!g_flat_data_pending_lbl[0]) return;
    data_buf_appendf("%s\n", g_flat_data_pending_lbl);
    g_flat_data_pending_lbl[0] = '\0';
}
#define SYM_SIGMA   "\xCE\xA3"
#define SYM_SIGLEN  "\xCE\xA3""len"
#define SYM_DELTA   "\xCE\x94"
#define ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
#define ADDR_SIGLEN  ((uint64_t)(uintptr_t)&Σlen)
#define ADDR_DELTA   ((uint64_t)(uintptr_t)&Δ)
static const char *(*g_flat_intern_str)(const char *s) = NULL;
const char *emit_intern_str(const char *s) {
    return (g_flat_intern_str && g_is_text) ? g_flat_intern_str(s) : NULL;
}
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
static void flat_emit_arg_subchain(IR_t *entry, bb_label_t *succ, bb_label_t *fail);
static int ir_node_is_alt_arm(IR_t *nd);
static IR_t *ir_skip_alt_arms(IR_t *entry);
static void descr_chain_operand_refs(IR_t *entry);
static void gvar_stmt_operand_refs(IR_t *head);
static int gvar_prewalk_idx_operand(IR_t *idx, bb_label_t *lbl_ω);
IR_t * bb_child0(const IR_t *n) { return (n && n->n_operands > 0) ? n->operands[0] : NULL; }
IR_t * bb_child1(const IR_t *n) { return (n && n->n_operands > 1) ? n->operands[1] : NULL; }
void bb_flat_cursor_reserve(int upto) { if (upto > g_flat_slot_count) g_flat_slot_count = upto; }
int bb_flat_cursor(void) { return g_flat_slot_count; }
extern int memcmp(const void *, const void *, size_t);
static bb_label_t g_α_ring[8];
static int        g_α_ring_i = 0;
static int        g_bb_alpha_seq = 0;
void g_bb_alpha_seq_reset(void) { g_bb_alpha_seq = 0; }
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
static int gather_inline_alt_arms(IR_t *start, IR_t **arms, int cap) {
    int n = 0;
    IR_t *cur = start;
    while (cur && n < cap && is_pat_chain_elem(cur->op)) {
        arms[n++] = cur;
        IR_t *nxt = cur->ω.node;
        if (!nxt || !is_pat_chain_elem(nxt->op)) break;
        cur = nxt;
    }
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int resolve_seq_goals_em(const IR_t *nd, IR_t **out, int max) {
    if (!nd || nd->op != IR_GCONJ) return 0;
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)IR_LIT(nd).ival;
    if (!zs || !zs->goals) return 0;
    int k = zs->ngoals < max ? zs->ngoals : max;
    for (int i = 0; i < k; i++) out[i] = zs->goals[i];
    return k;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int resolve_choice_bodies_em(const IR_t *nd, IR_t **out, int max) {
    if (!nd || nd->op != IR_CHOICE) return 0;
    bb_choice_state_t *zc = (bb_choice_state_t *)(intptr_t)IR_LIT(nd).ival;
    if (!zc || !zc->bodies) return 0;
    int k = zc->nbodies < max ? zc->nbodies : max;
    for (int i = 0; i < k; i++)
        out[i] = zc->bodies[i] ? (zc->bodies[i]->body_root ? zc->bodies[i]->body_root : zc->bodies[i]->entry) : NULL;
    return k;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void gz_fill_goal(IR_t *g, bb_label_t *gγ, bb_label_t *gω, bb_label_t *gβ) {
    g_emit.op_sval = (g->op == IR_DET_WRITE || g->op == IR_BUILTIN) ? IR_LIT(g).sval : NULL;
    g_emit.op_ival = (g->op == IR_CELL_CALL) ? 0 : IR_LIT(g).ival;
    IR_t *gw0 = bb_child0(g);
    g_emit.op_sb   = (g->op == IR_DET_WRITE && gw0 && gw0->op == IR_LOGICVAR) ? 1 : 0;
    g_emit.op_off  = g_emit.op_sb ? (int)IR_LIT(gw0).ival : 0;
    if (g->op == IR_CELL_CALL) {
        pl_gz_call_state_t *cs = (pl_gz_call_state_t *)(intptr_t)IR_LIT(g).ival;
        bb_label_t *la = (cs && cs->callee) ? (bb_label_t *)cs->callee->lblA : NULL;
        bb_label_t *lb = (cs && cs->callee) ? (bb_label_t *)cs->callee->lblB : NULL;
        g_emit.lbl_t0 = la ? la->name : NULL; g_emit.lbl_t0_p = la;
        g_emit.lbl_t1 = lb ? lb->name : NULL; g_emit.lbl_t1_p = lb;
    }
    if (g->op == IR_CELL_FINDALL) {
        pl_gz_findall_state_t *fst = (pl_gz_findall_state_t *)(intptr_t)IR_LIT(g).ival;
        pl_gz_call_state_t *cs = fst ? fst->call : NULL;
        bb_label_t *la = (cs && cs->callee) ? (bb_label_t *)cs->callee->lblA : NULL;
        bb_label_t *lb = (cs && cs->callee) ? (bb_label_t *)cs->callee->lblB : NULL;
        g_emit.lbl_t0 = la ? la->name : NULL; g_emit.lbl_t0_p = la;
        g_emit.lbl_t1 = lb ? lb->name : NULL; g_emit.lbl_t1_p = lb;
    }
    FILL(g, gγ, gω, gβ);
}
static void gz_callee_labels(pl_gz_callee_t *ce, pl_gz_callee_vec_t *cv) {
    if (!ce || ce->lblA) return;
    int cid = g_flat_node_id++;
    ce->lblA = (void *)emit_label_alloc("gzp%d_α", cid);
    ce->lblB = (void *)emit_label_alloc("gzp%d_β", cid);
    pl_gz_callees_push(cv, ce);
}
static void gz_collect_callees(IR_t *head, pl_gz_callee_vec_t *cv) {
    for (IR_t *g = head; g; g = g->γ.node) {
        if (g->op == IR_CELL_CALL) {
            pl_gz_call_state_t *cs = (pl_gz_call_state_t *)(intptr_t)IR_LIT(g).ival;
            if (cs) gz_callee_labels(cs->callee, cv);
        }
        if (g->op == IR_CELL_FINDALL) {
            pl_gz_findall_state_t *fst = (pl_gz_findall_state_t *)(intptr_t)IR_LIT(g).ival;
            if (fst && fst->call) gz_callee_labels(fst->call->callee, cv);
        }
        if (g->op == IR_CELL_ITE) {
            pl_gz_ite_state_t *is = (pl_gz_ite_state_t *)(intptr_t)IR_LIT(g).ival;
            if (is) { gz_collect_callees(is->cond_head, cv); gz_collect_callees(is->then_head, cv); gz_collect_callees(is->else_head, cv); }
        }
        if (g->op == IR_CELL_CATCH) {
            pl_gz_catch_state_t *cst = (pl_gz_catch_state_t *)(intptr_t)IR_LIT(g).ival;
            if (cst) { gz_collect_callees(cst->goal_head, cv); gz_collect_callees(cst->recovery_head, cv); }
        }
    }
}
static void gz_emit_cell(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv);
static bb_label_t * gz_emit_chain(IR_t *head, bb_label_t *chain_γ, bb_label_t *chain_ω, bb_label_t *cut_ω, bb_label_t *entry, pl_gz_callee_vec_t *cv) {
    int n = 0;
    for (IR_t *g = head; g; g = g->γ.node) n++;
    if (n == 0) return NULL;
    int cid = g_flat_node_id++;
    bb_label_t **cl = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **cb = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    IR_t **gn = (IR_t **)alloca(sizeof(IR_t *) * n);
    int k = 0;
    for (IR_t *g = head; g; g = g->γ.node) gn[k++] = g;
    for (int i = 0; i < n; i++) { cl[i] = emit_label_alloc("gzi%d_g%d_α", cid, i); cb[i] = emit_label_alloc("gzi%d_g%d_β", cid, i); }
    if (entry) emit_label_define_bb(entry);
    int i = 0;
    for (IR_t *g = head; g; g = g->γ.node) {
        emit_label_define_bb(cl[i]);
        bb_label_t *next_γ = (i + 1 < n) ? cl[i + 1] : chain_γ;
        int p = i - 1;
        while (p >= 0 && gz_node_bounded(gn[p])) p--;
        bb_label_t *gw = (g->op == IR_CELL_CUT && cut_ω) ? cut_ω : (p < 0 ? chain_ω : cb[p]);
        g_emit.op_bounded = (gz_node_bounded(g) && i + 1 < n) ? 1 : 0;
        gz_emit_cell(g, next_γ, gw, cb[i], cut_ω, cv);
        g_emit.op_bounded = 0;
        i++;
    }
    return cb[n - 1];
}
static void gz_emit_ite(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv) {
    pl_gz_ite_state_t *is = (pl_gz_ite_state_t *)(intptr_t)IR_LIT(g).ival;
    g_emit.op_bounded = 0;
    int cid = g_flat_node_id++;
    bb_label_t *Lg1 = emit_label_alloc("gzi%d_c1", cid);
    bb_label_t *Lg2 = emit_label_alloc("gzi%d_c2", cid);
    bb_label_t *E2a = is && is->then_head ? emit_label_alloc("gzi%d_t", cid) : NULL;
    bb_label_t *E3a = is && is->else_head ? emit_label_alloc("gzi%d_e", cid) : NULL;
    g_emit.op_sa = 0; g_emit.op_sb = 0; g_emit.op_sval = NULL; g_emit.op_ival = 0; g_emit.op_off = 0;
    FILL(g, Lg1, Lg2, gβ);
    if (is) (void)gz_emit_chain(is->cond_head, Lg1, Lg2, cut_ω, NULL, cv);
    emit_label_define_bb(Lg1);
    g_emit.op_sa = 1;
    { bb_label_t *d = E2a ? E2a : next_γ; g_emit.lbl_t0 = d->name; g_emit.lbl_t0_p = d; }
    FILL(g, next_γ, gw, gβ);
    emit_label_define_bb(Lg2);
    g_emit.op_sa = 2;
    { bb_label_t *d = E3a ? E3a : next_γ; g_emit.lbl_t0 = d->name; g_emit.lbl_t0_p = d; }
    FILL(g, next_γ, gw, gβ);
    bb_label_t *tβ = is ? gz_emit_chain(is->then_head, next_γ, gw, cut_ω, E2a, cv) : NULL;
    bb_label_t *eβ = is ? gz_emit_chain(is->else_head, next_γ, gw, cut_ω, E3a, cv) : NULL;
    g_emit.op_sa = 3;
    { bb_label_t *d = tβ ? tβ : gw; g_emit.lbl_t0 = d->name; g_emit.lbl_t0_p = d; }
    { bb_label_t *e = eβ ? eβ : gw; g_emit.lbl_t1 = e->name; g_emit.lbl_t1_p = e; }
    FILL(g, next_γ, gw, gβ);
    g_emit.op_sa = 0;
}
/* catch box: α marks the trail then runs the goal chain; goal.γ -> catch success (next_γ);
 * goal.ω -> the ball-check handler (op_sa 1): no pending throw -> ω (plain failure); pending ->
 * unwind to mark, gzu_build the catcher, rt_pl_throw_match -> recovery chain on match, else ω
 * leaving the ball set so an outer catch re-catches. Recovery chain: γ->next_γ, ω->catch.ω. */
static void gz_emit_catch(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv) {
    pl_gz_catch_state_t *cst = (pl_gz_catch_state_t *)(intptr_t)IR_LIT(g).ival;
    g_emit.op_bounded = 0;
    int cid = g_flat_node_id++;
    bb_label_t *Lgoal  = emit_label_alloc("gzc%d_goal", cid);
    bb_label_t *Lcheck = emit_label_alloc("gzc%d_chk",  cid);
    bb_label_t *Lrec   = (cst && cst->recovery_head) ? emit_label_alloc("gzc%d_rec", cid) : NULL;
    g_emit.op_sa = 0; g_emit.op_sb = 0; g_emit.op_sval = NULL; g_emit.op_ival = 0; g_emit.op_off = 0;
    { bb_label_t *d = Lgoal; g_emit.lbl_t0 = d->name; g_emit.lbl_t0_p = d; }
    FILL(g, next_γ, gw, gβ);
    if (cst && cst->goal_head) (void)gz_emit_chain(cst->goal_head, next_γ, Lcheck, cut_ω, Lgoal, cv);
    else { emit_label_define_bb(Lgoal); }
    emit_label_define_bb(Lcheck);
    g_emit.op_sa = 1;
    { bb_label_t *d = Lrec ? Lrec : gw; g_emit.lbl_t0 = d->name; g_emit.lbl_t0_p = d; }
    FILL(g, next_γ, gw, gβ);
    if (cst && cst->recovery_head) (void)gz_emit_chain(cst->recovery_head, next_γ, gw, cut_ω, Lrec, cv);
    g_emit.op_sa = 2;
    FILL(g, next_γ, gw, gβ);
    g_emit.op_sa = 0;
}
static void gz_emit_cell(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv) {
    if (g->op == IR_CELL_ITE) { gz_emit_ite(g, next_γ, gw, gβ, cut_ω, cv); return; }
    if (g->op == IR_CELL_CATCH) { gz_emit_catch(g, next_γ, gw, gβ, cut_ω, cv); return; }
    gz_fill_goal(g, next_γ, gw, gβ);
}
static IR_t * gz_clause_head_of(pl_gz_callee_t *ce, int c) {
    return ce->nclauses > 1 ? ce->clause_head[c] : ce->body_head;
}
static void gz_emit_callee(pl_gz_callee_t *ce, pl_gz_callee_vec_t *cv) {
    if (!ce || ce->body_emitted) return;
    ce->body_emitted = 1;
    int NC = ce->nclauses > 0 ? ce->nclauses : 1;
    for (int c = 0; c < NC; c++)
        gz_collect_callees(gz_clause_head_of(ce, c), cv);
    int cid = g_flat_node_id++;
    bb_label_t *cl_γ = emit_label_alloc("gzp%d_γ", cid);
    bb_label_t *cl_ω = emit_label_alloc("gzp%d_ω", cid);
    int *nb = (int *)alloca(sizeof(int) * (NC > 0 ? NC : 1)); int nbtot = 0;
    for (int c = 0; c < NC; c++) { nb[c] = 0; for (IR_t *g = gz_clause_head_of(ce, c); g; g = g->γ.node) nb[c]++; nbtot += nb[c]; }
    bb_label_t **pgl = (bb_label_t **)alloca(sizeof(bb_label_t *) * (nbtot > 0 ? nbtot : 1));
    bb_label_t **pgb = (bb_label_t **)alloca(sizeof(bb_label_t *) * (nbtot > 0 ? nbtot : 1));
    bb_label_t **cladv = (bb_label_t **)alloca(sizeof(bb_label_t *) * (NC > 0 ? NC : 1));
    bb_label_t **redo  = (bb_label_t **)alloca(sizeof(bb_label_t *) * (NC > 0 ? NC : 1));
    int *cbase = (int *)alloca(sizeof(int) * (NC > 0 ? NC : 1));
    int j = 0;
    for (int c = 0; c < NC; c++) {
        cbase[c] = j;
        for (IR_t *g = gz_clause_head_of(ce, c); g; g = g->γ.node) { pgl[j] = emit_label_alloc("gzp%d_g%d_α", cid, j); pgb[j] = emit_label_alloc("gzp%d_g%d_β", cid, j); j++; }
        cladv[c] = (c + 1 < NC) ? emit_label_alloc("gzp%d_c%d_adv", cid, c) : cl_ω;
    }
    emit_label_define_bb((bb_label_t *)ce->lblA);
    g_emit.op_sa = 0;
    g_emit.op_sb = 0;
    FILL(ce->frame_node, (nbtot > 0 ? pgl[0] : cl_γ), cl_ω, (bb_label_t *)ce->lblB);
    for (int c = 0; c < NC; c++) {
        bb_label_t *failtgt = cladv[c];                     /* last clause: cl_ω (unwind + ret 0) — the seed's fK_ω chain */
        j = cbase[c];
        int jj = 0;
        IR_t *cgn[256]; int cgc = 0;
        for (IR_t *g = gz_clause_head_of(ce, c); g && cgc < 256; g = g->γ.node) cgn[cgc++] = g;
        for (IR_t *g = gz_clause_head_of(ce, c); g; g = g->γ.node) {
            emit_label_define_bb(pgl[j]);
            bb_label_t *next_γ = (jj + 1 < nb[c]) ? pgl[j + 1] : cl_γ;
            int pp = jj - 1; if (pp >= cgc) pp = cgc - 1;
            while (pp >= 0 && gz_node_bounded(cgn[pp])) pp--;
            bb_label_t *gw = (g->op == IR_CELL_CUT) ? cl_ω : (pp < 0 ? failtgt : pgb[cbase[c] + pp]);
            g_emit.op_bounded = (gz_node_bounded(g) && jj + 1 < nb[c]) ? 1 : 0;
            gz_emit_cell(g, next_γ, gw, pgb[j], cl_ω, cv);
            g_emit.op_bounded = 0;
            j++; jj++;
        }
        redo[c] = (nb[c] > 0) ? pgb[cbase[c] + nb[c] - 1] : failtgt;
        if (c + 1 < NC) {                                   /* clause advance: cursor=c+2; unwind(mark); jmp next clause α */
            emit_label_define_bb(cladv[c]);
            g_emit.op_sa = 4;
            g_emit.op_off = c + 2;
            bb_label_t *nxt = pgl[cbase[c + 1]];
            g_emit.lbl_t0 = nxt->name; g_emit.lbl_t0_p = nxt;
            FILL(ce->frame_node, cl_γ, cl_ω, (bb_label_t *)ce->lblB);
        }
    }
    g_emit.op_sa = 1;
    g_emit.op_sb = 0;
    if (NC <= 1) {
        bb_label_t *r0 = redo[0];
        g_emit.lbl_t0 = r0->name; g_emit.lbl_t0_p = r0;
        FILL(ce->frame_node, cl_γ, cl_ω, (bb_label_t *)ce->lblB);
        return;
    }
    FILL(ce->frame_node, cl_γ, cl_ω, (bb_label_t *)ce->lblB);      /* landings + β prologue; cmp-chain follows by fall-through */
    for (int c = 0; c + 1 < NC; c++) {
        g_emit.op_sa = 2;
        g_emit.op_off = c + 1;
        g_emit.lbl_t0 = redo[c]->name; g_emit.lbl_t0_p = redo[c];
        FILL(ce->frame_node, cl_γ, cl_ω, (bb_label_t *)ce->lblB);
    }
    g_emit.op_sa = 3;
    g_emit.lbl_t0 = redo[NC - 1]->name; g_emit.lbl_t0_p = redo[NC - 1];
    FILL(ce->frame_node, cl_γ, cl_ω, (bb_label_t *)ce->lblB);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
void resolve_choice_clause_label(char *dst, size_t dsz, int id, int ci, const char *suffix) {
    snprintf(dst, dsz, ".Lplch%d_c%d_%s", id, ci, suffix);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t *ite_branch_walk_node(IR_t *entry, IR_t *root) {
    if (root && bb_kind_is_driver_owned(root->op)) return root;
    return entry;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_kind_is_driver_owned(int t) {
    return t == IR_MATCH_CAT || t == IR_MATCH_ALT || t == IR_MATCH_FENCE || t == IR_GCONJ;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *bb_intern_into(char *buf, const char *sval) {
    if (!sval) return NULL;
    const char *lbl = emit_intern_str(sval);
    if (!lbl) {
        strtab_label(buf, 64, sval);
        return buf;
    }
    snprintf(buf, 64, "%s", lbl);
    return buf;
}
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
static void descr_binop_set_slots(IR_t *nd) {
    g_emit.op_num_real = 0;
    if (binop_is_num_real(g_emit_cfg, nd)) {
        int sa = bb_slot_get(bb_child0(nd)), sb = bb_slot_get(bb_child1(nd));
        if (sa >= 0 && sb >= 0) { g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_num_real = 1; g_emit.op_off = bb_slot_alloc16(nd); return; }
    }
    g_emit.op_sa = descr_binop_opnd_slot(bb_child0(nd));
    g_emit.op_sb = descr_binop_opnd_slot(bb_child1(nd));
    g_emit.op_off = (g_emit.op_sa >= 0 && g_emit.op_sb >= 0) ? bb_slot_alloc16(nd) : -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int ir_alt_all_literal_arms(IR_t *alt) {
    if (!alt || !g_emit_cfg) return 0;
    int n = 0; IR_t * const *arms = bb_operand_aux_get(g_emit_cfg, alt, &n);
    if (!arms || n < 1 || n > 5) return 0;
    for (int i = 0; i < n; i++) if (!arms[i] || (arms[i]->op != IR_LIT_I && arms[i]->op != IR_LIT_S && arms[i]->op != IR_LIT_F)) return 0;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int idx_operand_is_lit(IR_t *o) { return o && (o->op == IR_LIT_I || o->op == IR_LIT_S || o->op == IR_LIT_F || o->op == IR_LIT_NUL); }
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static void case_slot_binop_operands(IR_t *v, bb_label_t *lbl_ω) {
    if (!v || v->op != IR_BINOP) return;
    if (v->n_operands < 2 && g_emit_cfg) {
        int na = 0; IR_t * const * ax = bb_operand_aux_get(g_emit_cfg, v, &na);
        if (na >= 2 && ax[0] && ax[1]) { v->n_operands = 0; ir_operand_push(v, ax[0]); ir_operand_push(v, ax[1]); }
    }
    IR_t *c0 = bb_child0(v);
    IR_t *c1 = bb_child1(v);
    int id = g_flat_node_id++;
    if (c0 && c0->op != IR_LIT_NUL && descr_binop_opnd_slot(c0) < 0) {
        bb_label_t *d = emit_label_alloc("xcaseop%d_c0_done", id); bb_label_t *b = emit_label_alloc("xcaseop%d_c0_b", id);
        walk_bb_flat(c0, d, lbl_ω, b); emit_label_define_bb(d);
    }
    if (c1 && c1->op != IR_LIT_NUL && descr_binop_opnd_slot(c1) < 0) {
        bb_label_t *d = emit_label_alloc("xcaseop%d_c1_done", id); bb_label_t *b = emit_label_alloc("xcaseop%d_c1_b", id);
        walk_bb_flat(c1, d, lbl_ω, b); emit_label_define_bb(d);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/* Emit e's value-operands (e.g. the from/to bounds of an IR_TO) so they get frame slots before e itself is walked.
   For a single-node value producer (literal, var) e has no operands and this is a no-op.  Mirrors case_slot_binop_operands. */
static void repalt_emit_operand_slots(IR_t *e, bb_label_t *lbl_ω) {
    if (!e) return;
    for (int k = 0; k < e->n_operands; k++) {
        IR_t *o = e->operands[k];
        if (!o || o->op == IR_LIT_NUL) continue;
        if (descr_binop_opnd_slot(o) >= 0) continue;
        int id = g_flat_node_id++;
        bb_label_t *d = emit_label_alloc("xrepaltop%d_done", id);
        bb_label_t *b = emit_label_alloc("xrepaltop%d_b",    id);
        walk_bb_flat(o, d, lbl_ω, b);
        emit_label_define_bb(d);
    }
}
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
static int arg_entry_terminal(IR_t *ae) {
    return (ae && (!ae->γ.node || ae->γ.node->op == IR_SUCCEED)) ? 1 : 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *scan_cset_or_lit_arg(IR_t *nd) {
    extern const char *kw_cset_const_str(const char *kw);
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0] && sblks[0]->entry && arg_entry_terminal(sblks[0]->entry)) ? sblks[0]->entry : (IR_t *)0;
    if (!ae) return (const char *)0;
    if (ae->op == IR_LIT_S) return IR_LIT(ae).sval;
    if (ae->op == IR_KEYWORD) return kw_cset_const_str(IR_LIT(ae).sval);
    return (const char *)0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *scan_cset_var_arg(IR_t *nd) {
    IR_graph_t **sblks = (IR_graph_t **)0;
    IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0] && sblks[0]->entry && arg_entry_terminal(sblks[0]->entry)) ? sblks[0]->entry : (IR_t *)0;
    if (ae && ae->op == IR_VAR && IR_LIT(ae).sval && IR_LIT(ae).sval[0] != '&') return IR_LIT(ae).sval;
    return (const char *)0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_emit_arg_subchain(IR_t *entry, bb_label_t *succ, bb_label_t *fail) {
    enum { CH_MAX = 512 };
    IR_t *nodes[CH_MAX]; int n = 0;
    IR_t *queue[CH_MAX]; int qh = 0, qt = 0;
    entry = ir_skip_alt_arms(entry);
    queue[qt++] = entry;
    while (qh < qt) {
        IR_t *c = queue[qh++];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        if (ir_node_is_alt_arm(c)) continue;
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= CH_MAX) { fprintf(stderr, "[GZ-10] FATAL arg subchain exceeds CH_MAX\n"); abort(); }
        nodes[n++] = c;
        if (c->γ.node && qt < CH_MAX) queue[qt++] = ir_skip_alt_arms(c->γ.node);
        if ((c->op == IR_BINOP) && c->ω.node && qt < CH_MAX) queue[qt++] = ir_skip_alt_arms(c->ω.node);
        { extern int g_scan_regs_live; if (g_scan_regs_live && (c->op == IR_CALL || ir_is_call_kind(c->op) || ir_is_scan_kind(c->op)) && c->ω.node && qt < CH_MAX) queue[qt++] = ir_skip_alt_arms(c->ω.node); }
    }
    { extern int g_scan_regs_live; if (g_scan_regs_live) for (int i = 0; i < n && g_flat_chain_set_n < FLAT_CHAIN_SET_MAX; i++) g_flat_chain_set[g_flat_chain_set_n++] = nodes[i]; }
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * (n > 0 ? n : 1));
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * (n > 0 ? n : 1));
    int id = g_flat_node_id++;
    for (int i = 0; i < n; i++) {
        lbls[i]  = emit_label_alloc("xargsub%d_n%d_α", id, i);
        betas[i] = emit_label_alloc("xargsub%d_n%d_β", id, i);
    }
    for (int i = 0; i < n; i++) {
        emit_label_define_bb(lbls[i]);
        bb_label_t *node_γ = succ;
        bb_label_t *node_ω = fail;
        IR_t *gtgt = ir_skip_alt_arms(nodes[i]->γ.node);
        IR_t *otgt = ir_skip_alt_arms(nodes[i]->ω.node);
        for (int k = 0; k < n; k++) if (nodes[k] == gtgt) { node_γ = (i > k && subchain_node_is_generator(nodes[k])) ? betas[k] : lbls[k]; break; }
        if (nodes[i]->γ.node == NULL || nodes[i]->γ.node->op == IR_SUCCEED) node_γ = succ;
        int omega_resolved = 0;
        for (int k = 0; k < n; k++) if (nodes[k] == otgt) { node_ω = lbls[k]; omega_resolved = 1; break; }
        if (!omega_resolved) node_ω = fail;
        walk_bb_flat(nodes[i], node_γ, node_ω, betas[i]);
    }
}
static void gvar_drive_call_arg_slots(IR_t *nd, bb_label_t *lbl_ω) {
    g_emit.op_arg_slot_n = 0;
    int nargs = (int)(nd ? IR_LIT(nd).ival : 0);
    IR_graph_t **subs = nd ? (IR_graph_t **)0 : NULL;
    if (nargs > OP_ARG_SLOT_MAX) return;
    IR_t *res[OP_ARG_SLOT_MAX]; IR_t *res_last[OP_ARG_SLOT_MAX]; int nadmit = 0;
    for (int i = 0; i < nargs; i++) {
        IR_t *ae = (subs && subs[i]) ? subs[i]->entry : NULL;
        { int guard = 0; while (ae && (ae->op == IR_SUCCEED || ae->op == IR_FAIL) && ae->γ.node && guard++ < 64) ae = ae->γ.node; }
        if (ae) {
            res[i] = ae; nadmit++;
            IR_t *last = ae; int g2 = 0;
            while (last->γ.node && last->γ.node->op != IR_SUCCEED && last->γ.node->op != IR_FAIL && g2++ < 512) last = last->γ.node;
            res_last[i] = last;
        } else { res[i] = NULL; res_last[i] = NULL; }
    }
    if (!nadmit) return;
    if (g_flat_slot_count < 16) (void)bb_slot_claim(16 - g_flat_slot_count);
    int slots[OP_ARG_SLOT_MAX];
    for (int i = 0; i < nargs; i++) {
        slots[i] = -1;
        if (!res[i]) continue;
        int id = g_flat_node_id++;
        bb_label_t *arg_done = emit_label_alloc("xgvarg%d_done", id);
        bb_label_t *arg_β    = emit_label_alloc("xgvarg%d_β",    id);
        int relop_diamond = 0;
        { IR_t *rp = res[i]; int rg = 0; while (rp && rg++ < 256) { if (rp->op == IR_BINOP && IR_LIT(rp).ival >= BINOP_LT && IR_LIT(rp).ival <= BINOP_NE) { if (res_last[i] && res_last[i]->op == IR_LIT_I && IR_LIT(res_last[i]).ival == 1 && rp->ω.node && rp->ω.node->op == IR_LIT_I && IR_LIT(rp->ω.node).ival == 0) relop_diamond = 1; break; } if (!rp->γ.node || rp->γ.node->op == IR_SUCCEED || rp->γ.node->op == IR_FAIL) break; rp = rp->γ.node; } }
        int unop_arith = ((res_last[i] && res_last[i]->op == IR_UNOP && (IR_LIT(res_last[i]).ival == TT_MNS || IR_LIT(res_last[i]).ival == TT_PLS)) || (res[i] && res[i]->op == IR_UNOP && (IR_LIT(res[i]).ival == TT_MNS || IR_LIT(res[i]).ival == TT_PLS))) ? 1 : 0;
        g_gvar_callarg_live = 1;
        if (res[i] && res[i]->op == IR_SEQ && IR_LIT(res[i]).dval == 1.0) {
            /* value-concat sequence: marshal_call_arg inline-concat arm flattens it and calls rt_concat_parts_d into the arg slot; pre-computing would route the IR_SEQ to flat_drive_gvar_seq_passthrough which yields no value and claims no slot */
        } else if (arg_entry_terminal(res[i]) && !unop_arith) {
            walk_bb_flat(res[i], arg_done, lbl_ω, arg_β);
            slots[i] = bb_slot_get(res[i]);
        } else if (relop_diamond) {
            /* relop diamond: marshal_call_arg boolean-relop arm emits INTVAL(0/1) into one slot; pre-computing the true-arm slot would read 1 even when the relop is false */
        } else if (unop_arith) {
            /* unary arith chain: marshal_call_arg inline-unop handles correctly (stores DT_I tag); pre-computation would only store the 8-byte raw value missing the tag */
        } else if (res_last[i] && res_last[i]->op == IR_BINOP && (IR_LIT(res_last[i]).ival == BINOP_ADD || IR_LIT(res_last[i]).ival == BINOP_SUB || IR_LIT(res_last[i]).ival == BINOP_MUL || IR_LIT(res_last[i]).ival == BINOP_DIV || IR_LIT(res_last[i]).ival == BINOP_MOD || IR_LIT(res_last[i]).ival == BINOP_POW)) {
            /* arith/pow BINOP chain: marshal_call_arg inline-arith (or POWER_fn for POW) handles correctly (stores DT_I tag); pre-computation would only store 8-byte raw int missing the tag */
        } else {
            IR_graph_t *_save_cfg = g_emit_cfg; if (subs && subs[i]) g_emit_cfg = subs[i];
            flat_emit_arg_subchain(res[i], arg_done, lbl_ω);
            g_emit_cfg = _save_cfg;
            slots[i] = bb_slot_get(res_last[i]);
        }
        g_gvar_callarg_live = 0;
        emit_label_define_bb(arg_done);
    }
    for (int i = 0; i < nargs; i++) g_emit.op_arg_slot[i] = slots[i];
    g_emit.op_arg_slot_n = nargs;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int gvar_prewalk_idx_operand(IR_t *idx, bb_label_t *lbl_ω) {
    if (!idx || idx->op != IR_IDX) return -1;
    if (g_flat_slot_count < 16) (void)bb_slot_claim(16 - g_flat_slot_count);
    int id = g_flat_node_id++;
    bb_label_t *idx_done = emit_label_alloc("xgvidx%d_done", id);
    bb_label_t *idx_β    = emit_label_alloc("xgvidx%d_β",    id);
    g_gvar_callarg_live = 1;
    walk_bb_flat(idx, idx_done, lbl_ω, idx_β);
    g_gvar_callarg_live = 0;
    emit_label_define_bb(idx_done);
    return bb_slot_get(idx);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int gen_bb_is_gen_arg(IR_t *e) {
    if (!e) return 0;
    if (e->op == IR_ASSIGN) return gen_bb_is_gen_arg(bb_child1(e));
    switch (e->op) {
        case IR_TO: case IR_TO_BY: case IR_UPTO: case IR_ALT: case IR_REPALT:
        case IR_ITERATE: case IR_LIMIT: case IR_PROC_GEN:
        case IR_LIST_BANG: case IR_KEY_GEN: case IR_FIND_GEN: case IR_SEQ_GEN:
        case IR_GATHER: case IR_MAP: case IR_GREP:
            return 1;
        default: return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int to_inner_gen_operand_k(IR_t *gi, IR_t **nodes, int n) {
    int bk = -1;
    if (gi->op != IR_TO && gi->op != IR_TO_BY) return -1;
    for (int oi = 0; oi < gi->n_operands; oi++) for (int k = 0; k < n; k++) if (nodes[k] == (IR_t *)gi->operands[oi] && ir_is_generator_kind(nodes[k]->op) && k > bk) bk = k;
    return bk;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int call_args_single_shot(IR_t *pBB) {
    int nargs = (int)(pBB ? IR_LIT(pBB).ival : 0);
    for (int j = 0; j < nargs; j++) { IR_t *ax = ir_call_arg(pBB, j); if (!ax) break; if (gen_bb_is_gen_arg(ax)) return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int gvar_seq_flatten(IR_graph_t *g, int *n) {
    if (!g || !g->entry || *n >= 16) return 0;
    IR_t *e = g->entry;
    if (e->op == IR_LIT_S) { g_emit.op_parts_tag[*n] = 0; g_emit.op_parts_str[(*n)++] = IR_LIT(e).sval ? IR_LIT(e).sval : ""; return 1; }
    if (e->op == IR_LIT_I) { char b[40]; snprintf(b, 40, "%lld", (long long)IR_LIT(e).ival); g_emit.op_parts_tag[*n] = 0; g_emit.op_parts_str[(*n)++] = strdup(b); return 1; }
    if (e->op == IR_LIT_F) { char b[40]; gcvt(IR_LIT(e).dval, 14, b); g_emit.op_parts_tag[*n] = 0; g_emit.op_parts_str[(*n)++] = strdup(b); return 1; }
    if (e->op == IR_VAR)   { g_emit.op_parts_tag[*n] = 1; g_emit.op_parts_str[(*n)++] = IR_LIT(e).sval ? IR_LIT(e).sval : ""; return 1; }
    if (e->op == IR_SEQ)   {
        IR_graph_t *l = (IR_graph_t *) 0;
        IR_graph_t *r = (IR_graph_t *) 0;
        return gvar_seq_flatten(l, n) && gvar_seq_flatten(r, n);
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static const char * scan_pat_cat_concat(IR_graph_t *pg) {
    if (!pg || !pg->entry || pg->entry->op != IR_MATCH_LIT) return NULL;
    int nlit = 0, ncat = 0;
    for (int i = 0; i < pg->n; i++) {
        IR_e t = pg->all[i]->op;
        if (t == IR_SUCCEED || t == IR_FAIL) continue;
        if (t == IR_MATCH_LIT) { nlit++; continue; }
        if (t == IR_MATCH_CAT) { ncat++; continue; }
        return NULL;
    }
    if (nlit < 2 || ncat < 1) return NULL;
    size_t total = 0;
    for (IR_t *c = pg->entry; c && c->op == IR_MATCH_LIT; c = c->γ.node) total += IR_LIT(c).sval ? strlen(IR_LIT(c).sval) : 0;
    char *buf = (char *)GC_MALLOC_ATOMIC(total + 1);
    size_t off = 0;
    for (IR_t *c = pg->entry; c && c->op == IR_MATCH_LIT; c = c->γ.node) { const char *s = IR_LIT(c).sval ? IR_LIT(c).sval : ""; size_t n = strlen(s); memcpy(buf + off, s, n); off += n; }
    buf[off] = 0;
    return buf;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int is_pat_chain_elem(IR_e t) {
    switch (t) {
    case IR_MATCH_LIT: case IR_MATCH_ANY: case IR_MATCH_NOTANY: case IR_MATCH_SPAN: case IR_MATCH_SPAN_VAR: case IR_MATCH_BREAK: case IR_MATCH_BREAKX:
    case IR_MATCH_LEN: case IR_MATCH_POS: case IR_MATCH_TAB: case IR_MATCH_RTAB: case IR_MATCH_ATP: case IR_MATCH_REM:
    case IR_MATCH_ARB: case IR_MATCH_FENCE: case IR_MATCH_ABORT: case IR_MATCH_DEFER: case IR_MATCH_ARBNO:
    case IR_MATCH_ALT: case IR_MATCH_ASSIGN_COND: case IR_MATCH_ASSIGN_IMM:
    case IR_FAIL: case IR_SUCCEED:
        return 1;
    default: return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void emit_cat_diamond(IR_t *entry, IR_t *J, bb_label_t *fwd, bb_label_t *fail, bb_label_t *arm_beta, int id, int armidx) {
    IR_t *br[64]; int nbr = 0;
    for (IR_t *b = entry; b && nbr < 64 && is_pat_chain_elem(b->op) && b->γ.node == J; b = b->ω.node) br[nbr++] = b;
    if (nbr == 0) { emit_label_define_bb(arm_beta); emit_jmp_label(fail, JMP_JMP); return; }
    bb_label_t **bw = (bb_label_t **)alloca((size_t)nbr * sizeof(bb_label_t *));
    bb_label_t **bb_ = (bb_label_t **)alloca((size_t)nbr * sizeof(bb_label_t *));
    for (int j = 0; j < nbr; j++) { bw[j] = emit_label_alloc("xcatd%d_e%d_b%d_w", id, armidx, j); bb_[j] = emit_label_alloc("xcatd%d_e%d_b%d_b", id, armidx, j); }
    for (int j = 0; j < nbr; j++) {
        bb_label_t *bf = (j < nbr - 1) ? bw[j] : fail;
        walk_bb_flat(br[j], fwd, bf, bb_[j]);
        if (j < nbr - 1) emit_label_define_bb(bw[j]);
    }
    emit_label_define_bb(arm_beta);
    emit_jmp_label(bb_[0], JMP_JMP);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int gather_lowered_cat_arms(IR_t *entry, IR_t **arms, IR_t **joins, int cap, IR_t **cat_out, IR_t *stop) {
    int n = 0;
    IR_t *c = entry;
    while (c && c != stop && is_pat_chain_elem(c->op) && !(joins && c->op == IR_SUCCEED) && n < cap) {
        IR_t *jn = NULL;
        if (joins && c->γ.node && c->γ.node->op == IR_MATCH_ALT) {
            IR_t *J = c->γ.node;
            IR_t *sib = c->ω.node;
            if (sib && is_pat_chain_elem(sib->op) && sib->γ.node == J) jn = J;
        }
        arms[n] = c;
        if (joins) joins[n] = jn;
        n++;
        c = jn ? jn->γ.node : c->γ.node;
    }
    if (n >= 2 && c && c->op == IR_MATCH_CAT && bb_match_nkids(c) == 0) { if (cat_out) *cat_out = c; return n; }
    if (joins && n >= 2 && c && c->op == IR_SUCCEED) { if (cat_out) *cat_out = NULL; return n; }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int while_operand_simple(IR_t *o) {
    if (!o) return 0;
    switch (o->op) {
    case IR_VAR: case IR_LIT_I: case IR_LIT_S: case IR_LIT_F: case IR_LIT_NUL:
        return 1;
    case IR_BINOP:
        return while_operand_simple(bb_child0(o)) && while_operand_simple(bb_child1(o));
    case IR_ASSIGN:
        return bb_child0(o) && bb_child0(o)->op == IR_VAR && (!bb_child1(o) || while_operand_simple(bb_child1(o)));
    default:
        return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int while_cond_emittable(IR_t *cond) {
    return cond && cond->op == IR_BINOP && 0 &&
           while_operand_simple(bb_child0(cond)) && while_operand_simple(bb_child1(cond));
}
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
/*--------------------------------------------------------------------------------------------------------------------*/
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    emit_drive(nd, lbl_γ, lbl_ω, lbl_β);
}
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
static void walk_bb_flat_or_inline_alt(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (nd && nd->γ.node && nd->γ.node->op == IR_MATCH_ALT) {
        int na = 0; bb_operand_aux_get(g_emit_cfg, nd->γ.node, &na);
        if (na == 0) {
            IR_t *alt_arms[64]; int na2 = gather_inline_alt_arms(nd, alt_arms, 64);
            if (na2 >= 2) {
                int id = g_flat_node_id++;
                bb_label_t **ai_ωs = (bb_label_t **)alloca((size_t)na2 * sizeof(bb_label_t *));
                bb_label_t **ai_βs = (bb_label_t **)alloca((size_t)na2 * sizeof(bb_label_t *));
                for (int i = 0; i < na2; i++) {
                    ai_ωs[i] = emit_label_alloc("ialt%d_a%d_ω", id, i);
                    ai_βs[i] = emit_label_alloc("ialt%d_a%d_β", id, i);
                }
                for (int i = 0; i < na2; i++) {
                    bb_label_t *arm_ω = (i < na2-1) ? ai_ωs[i] : lbl_ω;
                    walk_bb_flat(alt_arms[i], lbl_γ, arm_ω, ai_βs[i]);
                    if (i < na2-1) emit_label_define_bb(ai_ωs[i]);
                }
                return;
            }
        }
    }
    walk_bb_flat(nd, lbl_γ, lbl_ω, lbl_β);
}
static int codegen_flat_body(IR_t *nd, const char *prefix, int text_externalise, int wired) {
    bb_label_t lbl_α, lbl_α_body, lbl_γ, lbl_ω, lbl_β;
    emit_label_initf(&lbl_α,      "%s_α",      prefix);
    emit_label_initf(&lbl_α_body, "%s_α_body", prefix);
    emit_label_initf(&lbl_γ,       "%s_γ",      prefix);
    emit_label_initf(&lbl_ω,       "%s_ω",      prefix);
    emit_label_initf(&lbl_β,       "%s_β",       prefix);
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
    g_emit.flat_wired         = wired;
    if (text_externalise && g_is_text) emit_label_define_bb(&lbl_α);
    xa_dispatch(XA_FLAT_PROLOGUE);
    if (g_is_text && !wired) g_emit_pos += 7;
    emit_label_define_bb(&lbl_α_body);
    nd = ir_skip_alt_arms(nd);
    walk_bb_flat_or_inline_alt(nd, &lbl_γ, &lbl_ω, &lbl_β);
    emit_label_define_bb(&lbl_γ);
    xa_dispatch(XA_FLAT_EPILOGUE);
    g_emit.flat_wired = 0;
    if (text_externalise && g_is_text) {
        data_buf_flush_pending_label();
        xa_dispatch(XA_FLAT_DATA_SECTION);
        data_buf_reset();
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int g_in_prebuild = 0;
static int g_text_child_counter = 0;
static void pre_build_children_text(IR_t *nd, FILE *out, const char *base_prefix) {
    if (!nd) return;
    if (nd->op == IR_REF_INVARIANT) {
        int n_aux = 0;
        IR_t * const * aux = bb_operand_aux_get(g_emit_cfg, nd, &n_aux);
        IR_t *ch = (n_aux > 0 && aux) ? aux[0] : NULL;
        if (ch && !child_cache_get(ch)) {
            pre_build_children_text(ch, out, base_prefix);
            char child_prefix[120];
            snprintf(child_prefix, sizeof(child_prefix), "%s_c%d", base_prefix, g_text_child_counter++);
            emitter_init_text(out, TEXT_MODE_INVOCATION);
            codegen_flat_body(ch, child_prefix, 1, 0);
            emitter_end();
            char α_lbl[128];
            snprintf(α_lbl, sizeof(α_lbl), "%s_α", child_prefix);
            bb_box_fn sentinel = (bb_box_fn)(uintptr_t)ch;
            child_cache_put(ch, sentinel);
            child_cache_set_lbl(sentinel, α_lbl);
        }
        return;
    }
    if (nd->op == IR_MATCH_ASSIGN_COND || nd->op == IR_MATCH_ASSIGN_IMM) {
        IR_t *ch = (bb_match_nkids(nd) > 0) ? bb_match_kid(nd, 0) : (nd->n_operands > 0 ? nd->operands[0] : (IR_t*)0);
        IR_t *sc = ch;
        while (sc && sc != nd && is_pat_chain_elem(sc->op)) { pre_build_children_text(sc, out, base_prefix); sc = sc->γ.node; }
        return;
    }
    if (nd->op == IR_MATCH_ARBNO || nd->op == IR_MATCH_CALLOUT) {
        IR_t *ch = NULL;
        IR_graph_t *chg = NULL;
        if (nd->op == IR_MATCH_ARBNO) {
            bb_arbno_state_t *az = (bb_arbno_state_t *) 0;
            IR_graph_t *inner = az ? az->inner : NULL;
            ch = (inner && inner->entry) ? inner->entry : NULL;
            chg = inner;
        } else {
            ch = (bb_match_nkids(nd) > 0) ? bb_match_kid(nd, 0) : ((IR_t*)0);
        }
        if (ch && !child_cache_get(ch)) {
            pre_build_children_text(ch, out, base_prefix);
            char child_prefix[120];
            snprintf(child_prefix, sizeof(child_prefix), "%s_c%d", base_prefix, g_text_child_counter++);
            IR_graph_t *save_cfg = g_emit_cfg;
            if (chg) g_emit_cfg = chg;
            emitter_init_text(out, TEXT_MODE_INVOCATION);
            codegen_flat_body(ch, child_prefix, 1, nd->op == IR_MATCH_ARBNO ? 1 : 0);
            emitter_end();
            g_emit_cfg = save_cfg;
            char α_lbl[128];
            snprintf(α_lbl, sizeof(α_lbl), "%s_α", child_prefix);
            bb_box_fn sentinel = (bb_box_fn)(uintptr_t)ch;
            child_cache_put(ch, sentinel);
            child_cache_set_lbl(sentinel, α_lbl);
        }
        return;
    }
    if (bb_match_nkids(nd) == 0) return;
    for (int i = 0; i < bb_match_nkids(nd); i++) pre_build_children_text(bb_match_kid(nd, i), out, base_prefix);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void pre_build_children(IR_t *nd) {
    if (!nd) return;
    if (nd->op == IR_REF_INVARIANT) {
        int n_aux = 0;
        IR_t * const * aux = bb_operand_aux_get(g_emit_cfg, nd, &n_aux);
        IR_t *ch = (n_aux > 0 && aux) ? aux[0] : NULL;
        if (ch && !child_cache_get(ch)) {
            pre_build_children(ch);
            bb_box_fn fn = bb_build_flat(ch);
            child_cache_put(ch, fn);
        }
        return;
    }
    if (nd->op == IR_MATCH_ASSIGN_COND || nd->op == IR_MATCH_ASSIGN_IMM) {
        IR_t *ch = (bb_match_nkids(nd) > 0) ? bb_match_kid(nd, 0) : ((IR_t*)0);
        if (ch) pre_build_children(ch);
        return;
    }
    if (nd->op == IR_MATCH_ARBNO || nd->op == IR_MATCH_CALLOUT) {
        IR_t *ch = (bb_match_nkids(nd) > 0) ? bb_match_kid(nd, 0) : ((IR_t*)0);
        if (ch && !child_cache_get(ch)) {
            pre_build_children(ch);
            bb_box_fn fn = bb_build_flat(ch);
            child_cache_put(ch, fn);
        }
        return;
    }
    if (bb_match_nkids(nd) == 0) return;
    for (int i = 0; i < bb_match_nkids(nd); i++) pre_build_children(bb_match_kid(nd, i));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void scan_set_subj_node(IR_t *n, IR_t *s) {
    while (n->n_operands < 3) ir_operand_push(n, (IR_t *)0);
    n->operands[2] = s;
}
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
}
void resolve_call_kinds_gvar(IR_graph_t *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) { IR_t *nd = g->all[i]; if (!nd) continue; int iscall = (nd->op == IR_CALL || nd->op == IR_CALL_DEFINE || ir_is_call_kind(nd->op));
        if (nd->op == IR_CALL) { const char *fn = IR_LIT(nd).sval; double dv = IR_LIT(nd).dval;
            if (fn && fn[0] && (dv == 2.0 || dv == 3.0) && rt_proc_is_registered(fn)) nd->op = IR_CALL_GVAR_USERPROC;
            else if (fn && fn[0] && dv != 3.0 && strcmp(fn, "write") && strcmp(fn, "writes") && rt_builtin_is_known(fn)) nd->op = IR_CALL_BUILTIN; }
        if (iscall && (IR_LIT(nd).dval == 2.0 || IR_LIT(nd).dval == 3.0 || IR_LIT(nd).dval == 5.0)) { IR_graph_t **bk = (IR_graph_t **)0;
            if (bk) for (int j = 0; j < (int) IR_LIT(nd).ival; j++) if (bk[j]) resolve_call_kinds_gvar(bk[j]); } }
}
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
}
int descr_flat_chain_build_text(IR_t *entry, FILE *out, const char *prefix) {
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
/*--------------------------------------------------------------------------------------------------------------------*/
int descr_flat_chain_build_proc_text(IR_t *entry, const char **pnames, int np, FILE *out, const char *pname) {
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
static IR_t *gvar_chain_resolve(IR_t *n) {
    int guard = 0;
    while (n && n->op == IR_SUCCEED && n->γ.node != NULL && guard++ < 4096) n = n->γ.node;
    return n;
}
static IR_t *gvar_chain_resolve_stmt(IR_t *n) {
    int guard = 0;
    while (n && guard++ < 4096) {
        if (n->op == IR_SUCCEED && n->γ.node != NULL) { n = n->γ.node; continue; }
        if ((n->op == IR_SEQ || n->op == IR_SEQ_EXPR) && IR_LIT(n).dval == 1.0 && n->γ.node != NULL) { n = n->γ.node; continue; }
        break;
    }
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int32_t gvar_chain_skip_stno(IR_t *n) {
    int guard = 0; int32_t st = 0;
    while (n && guard++ < 4096) {
        if (n->op == IR_SUCCEED && n->γ.node != NULL) { if (IR_LIT(n).ival != 0) st = (int32_t)IR_LIT(n).ival; n = n->γ.node; continue; }
        if ((n->op == IR_SEQ || n->op == IR_SEQ_EXPR) && IR_LIT(n).dval == 1.0 && n->γ.node != NULL) { n = n->γ.node; continue; }
        break;
    }
    return st;
}
static int gvar_chain_collect_stnos(IR_t *n, int32_t *out, int max) {
    int guard = 0; int cnt = 0;
    while (n && guard++ < 4096) {
        if (n->op == IR_SUCCEED && n->γ.node != NULL) { if (IR_LIT(n).ival != 0 && cnt < max) out[cnt++] = (int32_t)IR_LIT(n).ival; n = n->γ.node; continue; }
        if ((n->op == IR_SEQ || n->op == IR_SEQ_EXPR) && IR_LIT(n).dval == 1.0 && n->γ.node != NULL) { n = n->γ.node; continue; }
        break;
    }
    return cnt;
}
static int gvar_chain_is_real(IR_t *n) { return n && n->op != IR_SUCCEED && n->op != IR_FAIL; }
static int gvar_chain_arity(const IR_t *n) {
    if (n && (n->op == IR_SEQ || n->op == IR_SEQ_EXPR) && IR_LIT(n).dval == 1.0) return 0;
    if (n && n->op == IR_SCAN) return 1;
    if (n && n->op == IR_RETURN && (IR_LIT(n).dval == 1.0 || IR_LIT(n).dval == 2.0)) return 0;
    return descr_chain_arity(n);
}
static void gvar_stmt_operand_refs(IR_t *head) {
    IR_t *chain[512]; int nc = 0;
    IR_t *c = head;
    while (gvar_chain_is_real(c) && nc < 512) {
        int dup = 0; for (int i = 0; i < nc; i++) if (chain[i] == c) { dup = 1; break; }
        if (dup) break;
        chain[nc++] = c;
        IR_t *g = c->γ.node;
        if (!g || g->op == IR_SUCCEED || g->op == IR_FAIL) break;
        c = g;
    }
    IR_t *stk[512]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = gvar_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        if (ar == 2 && sp >= 2) { n->n_operands = 0; ir_operand_push(n, stk[sp - 2]); ir_operand_push(n, stk[sp - 1]); sp -= 2; }
        else if (ar == 1 && sp >= 1) { if (n->op != IR_SCAN) { n->n_operands = 0; ir_operand_push(n, stk[sp - 1]); } else { scan_set_subj_node(n, stk[sp - 1]); } sp -= 1; }
        else if (ar >= 1) { sp = 0; }
        stk[sp++] = n;
    }
}
static void gvar_chain_prebuild_children(IR_graph_t *g) {
    if (!g || !g->all) return;
    for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->op == IR_REF_INVARIANT) pre_build_children(g->all[i]);
}
static void gvar_chain_prebuild_children_text(IR_graph_t *g, FILE *out, const char *prefix) {
    if (!g || !g->all) return;
    for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->op == IR_REF_INVARIANT) pre_build_children_text(g->all[i], out, prefix);
    enum { RCH_MAX = 512 };
    IR_t *reached[RCH_MAX]; int nr = 0;
    IR_t *queue[RCH_MAX]; int qh = 0, qt = 0;
    IR_t *e0 = gvar_chain_resolve_stmt(g->entry);
    if (gvar_chain_is_real(e0) && qt < RCH_MAX) queue[qt++] = e0;
    while (qh < qt) {
        IR_t *c = queue[qh++];
        int dup = 0; for (int i = 0; i < nr; i++) if (reached[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (nr < RCH_MAX) reached[nr++] = c;
        IR_t *gn = gvar_chain_resolve_stmt(c->γ.node);
        IR_t *wn = gvar_chain_resolve_stmt(c->ω.node);
        if (gvar_chain_is_real(gn) && qt < RCH_MAX) queue[qt++] = gn;
        if (gvar_chain_is_real(wn) && qt < RCH_MAX) queue[qt++] = wn;
    }
    for (int i = 0; i < nr; i++) {
        IR_t *nd = reached[i];
        if (nd->op == IR_MATCH_ARBNO) { pre_build_children_text(nd, out, prefix); continue; }
        if (nd->op != IR_SCAN) continue;
        IR_graph_t *pg = (IR_graph_t *) 0;
        if (!pg || !pg->all) continue;
        for (int j = 0; j < pg->n; j++) if (pg->all[j] && pg->all[j]->op == IR_MATCH_ARBNO) pre_build_children_text(pg->all[j], out, prefix);
    }
}
static void gvar_chain_operand_refs(IR_graph_t *g) {
    if (!g || !g->all) return;
    IR_t *heads[2048]; int nh = 0;
    IR_t *e0 = gvar_chain_resolve(g->entry);
    if (gvar_chain_is_real(e0)) heads[nh++] = e0;
    for (int i = 0; i < g->n && nh < 2048; i++) {
        IR_t *L = g->all[i];
        if (!L || L->op != IR_SUCCEED || L->γ.node == NULL) continue;
        IR_t *h = gvar_chain_resolve(L);
        if (!gvar_chain_is_real(h)) continue;
        int dup = 0; for (int k = 0; k < nh; k++) if (heads[k] == h) { dup = 1; break; }
        if (!dup) heads[nh++] = h;
    }
    for (int i = 0; i < g->n && nh < 2048; i++) {
        IR_t *L = g->all[i];
        if (!L || !L->ω.node) continue;
        IR_t *h = gvar_chain_resolve(L->ω.node);
        if (!gvar_chain_is_real(h)) continue;
        int dup = 0; for (int k = 0; k < nh; k++) if (heads[k] == h) { dup = 1; break; }
        if (!dup) heads[nh++] = h;
    }
    for (int i = 0; i < nh; i++) gvar_stmt_operand_refs(heads[i]);
}
static int codegen_gvar_flat_chain_body(IR_t *entry, const char *prefix) {
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
    int nodes_cap = 512, queue_cap = 512;
    IR_t **nodes = (IR_t **)malloc(sizeof(IR_t *) * nodes_cap); int n = 0;
    int32_t *nstno = (int32_t *)malloc(sizeof(int32_t) * nodes_cap);
    int32_t *nstno_extra = (int32_t *)malloc(sizeof(int32_t) * nodes_cap * 8);
    int *nstno_cnt = (int *)malloc(sizeof(int) * nodes_cap);
    int32_t *ntail_extra = (int32_t *)malloc(sizeof(int32_t) * nodes_cap * 8);
    int *ntail_cnt = (int *)malloc(sizeof(int) * nodes_cap);
    IR_t **queue = (IR_t **)malloc(sizeof(IR_t *) * queue_cap); int qh = 0, qt = 0;
    int32_t *qstno = (int32_t *)malloc(sizeof(int32_t) * queue_cap);
    int32_t *qstno_extra = (int32_t *)malloc(sizeof(int32_t) * queue_cap * 8);
    int *qstno_cnt = (int *)malloc(sizeof(int) * queue_cap);
    int32_t e0_stbuf[8]; int e0_cnt = gvar_chain_collect_stnos(entry, e0_stbuf, 8);
    int32_t e0_st = e0_cnt > 0 ? e0_stbuf[e0_cnt - 1] : 0;
    IR_t *e0 = gvar_chain_resolve_stmt(entry);
    if (gvar_chain_is_real(e0)) { qstno[qt] = e0_st; memcpy(qstno_extra + qt*8, e0_stbuf, sizeof(int32_t)*e0_cnt); qstno_cnt[qt] = e0_cnt; queue[qt++] = e0; }
    while (qh < qt) {
        int32_t c_st = qstno[qh];
        int c_cnt = qstno_cnt[qh];
        int32_t c_extra[8]; memcpy(c_extra, qstno_extra + qh*8, sizeof(int32_t)*c_cnt);
        IR_t *c = queue[qh++];
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= nodes_cap) { nodes_cap *= 2; nodes = (IR_t **)realloc(nodes, sizeof(IR_t *) * nodes_cap); nstno = (int32_t *)realloc(nstno, sizeof(int32_t) * nodes_cap); nstno_extra = (int32_t *)realloc(nstno_extra, sizeof(int32_t) * nodes_cap * 8); nstno_cnt = (int *)realloc(nstno_cnt, sizeof(int) * nodes_cap); ntail_extra = (int32_t *)realloc(ntail_extra, sizeof(int32_t) * nodes_cap * 8); ntail_cnt = (int *)realloc(ntail_cnt, sizeof(int) * nodes_cap); }
        nstno[n] = c_st; memcpy(nstno_extra + n*8, c_extra, sizeof(int32_t)*c_cnt); nstno_cnt[n] = c_cnt; ntail_cnt[n] = 0; nodes[n++] = c;
        int32_t g_stbuf[8]; int g_cnt = gvar_chain_collect_stnos(c->γ.node, g_stbuf, 8);
        int32_t w_stbuf[8]; int w_cnt = gvar_chain_collect_stnos(c->ω.node, w_stbuf, 8);
        int32_t g_st = g_cnt > 0 ? g_stbuf[g_cnt - 1] : 0;
        int32_t w_st = w_cnt > 0 ? w_stbuf[w_cnt - 1] : 0;
        IR_t *g = gvar_chain_resolve_stmt(c->γ.node);
        IR_t *w = gvar_chain_resolve_stmt(c->ω.node);
        if (!gvar_chain_is_real(g) && !(g && g->op == IR_FAIL) && g_cnt > 0) { memcpy(ntail_extra + (n-1)*8, g_stbuf, sizeof(int32_t)*g_cnt); ntail_cnt[n-1] = g_cnt; }
        if (gvar_chain_is_real(g)) { if (qt >= queue_cap) { queue_cap *= 2; queue = (IR_t **)realloc(queue, sizeof(IR_t *) * queue_cap); qstno = (int32_t *)realloc(qstno, sizeof(int32_t) * queue_cap); qstno_extra = (int32_t *)realloc(qstno_extra, sizeof(int32_t) * queue_cap * 8); qstno_cnt = (int *)realloc(qstno_cnt, sizeof(int) * queue_cap); } qstno[qt] = g_st; memcpy(qstno_extra + qt*8, g_stbuf, sizeof(int32_t)*g_cnt); qstno_cnt[qt] = g_cnt; queue[qt++] = g; }
        if (gvar_chain_is_real(w)) { if (qt >= queue_cap) { queue_cap *= 2; queue = (IR_t **)realloc(queue, sizeof(IR_t *) * queue_cap); qstno = (int32_t *)realloc(qstno, sizeof(int32_t) * queue_cap); qstno_extra = (int32_t *)realloc(qstno_extra, sizeof(int32_t) * queue_cap * 8); qstno_cnt = (int *)realloc(qstno_cnt, sizeof(int) * queue_cap); } qstno[qt] = w_st; memcpy(qstno_extra + qt*8, w_stbuf, sizeof(int32_t)*w_cnt); qstno_cnt[qt] = w_cnt; queue[qt++] = w; }
    }
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * (n > 0 ? n : 1));
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * (n > 0 ? n : 1));
    int id = g_flat_node_id++;
    for (int i = 0; i < n; i++) {
        lbls[i]  = emit_label_alloc("snoch%d_n%d_α", id, i);
        betas[i] = emit_label_alloc("snoch%d_n%d_β", id, i);
    }
    int32_t emitted_stnos[512]; int n_emitted = 0;
    for (int i = 0; i < n; i++) {
        emit_label_define_bb(lbls[i]);
        { extern int g_monitor_bin; if (g_monitor_bin && nstno_cnt[i] > 0) { extern void emit_mon_label_tap(int32_t); for (int ki = 0; ki < nstno_cnt[i]; ki++) { int32_t s = nstno_extra[i*8+ki]; int seen=0; for(int si=0;si<n_emitted;si++) if(emitted_stnos[si]==s){seen=1;break;} if(!seen){if(n_emitted<512)emitted_stnos[n_emitted++]=s; emit_mon_label_tap(s);} } } }
        bb_label_t *node_γ = &lbl_γ;
        bb_label_t *node_ω = &lbl_ω;
        IR_t *g = gvar_chain_resolve_stmt(nodes[i]->γ.node);
        IR_t *w = gvar_chain_resolve_stmt(nodes[i]->ω.node);
        if (gvar_chain_is_real(g)) { for (int k = 0; k < n; k++) if (nodes[k] == g) { node_γ = lbls[k]; break; } }
        else if (g && g->op == IR_FAIL) node_γ = &lbl_ω;
        if (gvar_chain_is_real(w)) { for (int k = 0; k < n; k++) if (nodes[k] == w) { node_ω = lbls[k]; break; } }
        else if (w && w->op == IR_FAIL) node_ω = &lbl_ω;
        else if (w && w->op == IR_SUCCEED) node_ω = &lbl_γ;
        bb_label_t *tail_tramp = (bb_label_t *)0;
        { extern int g_monitor_bin; if (g_monitor_bin && ntail_cnt[i] > 0 && node_γ == &lbl_γ) { tail_tramp = emit_label_alloc("snoch%d_n%d_tail", id, i); node_γ = tail_tramp; } }
        walk_bb_flat(nodes[i], node_γ, node_ω, betas[i]);
        if (tail_tramp) { extern void emit_mon_label_tap(int32_t); emit_label_define_bb(tail_tramp); for (int ki = 0; ki < ntail_cnt[i]; ki++) emit_mon_label_tap(ntail_extra[i*8+ki]); emit_jmp_label(&lbl_γ, JMP_JMP); }
    }
    if (n == 0) { extern int g_monitor_bin; if (g_monitor_bin && e0_cnt > 0) { extern void emit_mon_label_tap(int32_t); for (int ki = 0; ki < e0_cnt; ki++) emit_mon_label_tap(e0_stbuf[ki]); } emit_jmp_label(&lbl_γ, JMP_JMP); }
    emit_label_define_bb(&lbl_β);
    emit_jmp_label(&lbl_ω, JMP_JMP);
    emit_label_define_bb(&lbl_γ);
    xa_dispatch(XA_FLAT_EPILOGUE);
    if (text_externalise && g_is_text) {
        data_buf_flush_pending_label();
        xa_dispatch(XA_FLAT_DATA_SECTION);
        data_buf_reset();
    }
    free(nodes); free(queue); free(nstno); free(nstno_extra); free(nstno_cnt); free(ntail_extra); free(ntail_cnt); free(qstno); free(qstno_extra); free(qstno_cnt);
    return 0;
}
bb_box_fn gvar_flat_chain_build(IR_graph_t *g) {
    if (!g || !g->entry) return NULL;
    IR_graph_t *save_cfg = g_emit_cfg; g_emit_cfg = g;
    int has_ref = 0; for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->op == IR_REF_INVARIANT) { has_ref = 1; break; }
    if (has_ref && !g_in_prebuild) { g_child_cache_n = 0; g_in_prebuild = 1; gvar_chain_prebuild_children(g); g_in_prebuild = 0; }
    gvar_chain_operand_refs(g);
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) { g_emit_cfg = save_cfg; return NULL; }
    g_flat_slot_count = 0; g_flat_node_id = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0; g_subject_slot = -1;
    if (g->nslots > 0) g_flat_slot_count = 16 + (g->nslots - 1) * 16;
    g_gvar_flat_chain = 1;
    resolve_call_kinds_gvar(g);
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_gvar_flat_chain_body(g->entry, "flat");
    g_last_flat_frame_bytes = g_flat_slot_count;
    int nbytes = emitter_end();
    g_gvar_flat_chain = 0;
    g_emit_cfg = save_cfg;
    extern int bb_emit_overflow;
    if (bb_emit_overflow || nbytes <= 0 || nbytes > FLAT_BUF_MAX) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    bb_seal(buf, (size_t)nbytes);
    bb_pool_trim_last(buf, FLAT_BUF_MAX, (size_t)nbytes);
    return (bb_box_fn)buf;
}
int gvar_flat_chain_build_text(IR_graph_t *g, FILE *out, const char *prefix) {
    if (!g || !g->entry) return 1;
    IR_graph_t *save_cfg = g_emit_cfg; g_emit_cfg = g;
    int has_ref = 0;
    for (int i = 0; i < g->n && !has_ref; i++) {
        IR_t *nd = g->all[i];
        if (!nd) continue;
        if (nd->op == IR_REF_INVARIANT || nd->op == IR_MATCH_ARBNO) { has_ref = 1; break; }
        if (nd->op == IR_SCAN) { IR_graph_t *pg = (IR_graph_t *) 0; if (pg && pg->all) { for (int j = 0; j < pg->n; j++) if (pg->all[j] && pg->all[j]->op == IR_MATCH_ARBNO) { has_ref = 1; break; } } }
    }
    if (has_ref) { g_child_cache_n = 0; g_text_child_counter = 0; gvar_chain_prebuild_children_text(g, out, prefix); }
    gvar_chain_operand_refs(g);
    g_flat_slot_count = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0; g_subject_slot = -1;
    if (g->nslots > 0) g_flat_slot_count = 16 + (g->nslots - 1) * 16;
    g_gvar_flat_chain = 1;
    resolve_call_kinds_gvar(g);
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    int rc = codegen_gvar_flat_chain_body(g->entry, prefix);
    g_last_flat_frame_bytes = g_flat_slot_count;
    emitter_end();
    g_gvar_flat_chain = 0;
    g_emit_cfg = save_cfg;
    return rc;
}
/*--------------------------------------------------------------------------------------------------------------------*/
bb_box_fn gvar_flat_chain_build_at(IR_graph_t *g, IR_t *entry_node, const char *prefix) {
    if (!g) return NULL;
    IR_t *save_entry = g->entry;
    if (entry_node) g->entry = entry_node;
    bb_box_fn fn = gvar_flat_chain_build(g);
    g->entry = save_entry;
    (void)prefix;
    return fn;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void gva_collect_graph(IR_graph_t *g) {
    if (!g || !g->all) return;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i]; if (!nd || !IR_LIT(nd).sval) continue;
        switch (nd->op) {
        case IR_VAR: case IR_ASSIGN: case IR_ASSIGN_LIT_S: case IR_ASSIGN_LIT_I: case IR_ASSIGN_VAR: case IR_ASSIGN_CONCAT: case IR_ASSIGN_CALL: case IR_BINOP_GVAR_ARITH: case IR_BINOP_GVAR_RELOP: case IR_BINOP_GVAR_ARITH_SLOT: case IR_BINOP_GVAR_CONCAT:
            (void)gva_collect_var(IR_LIT(nd).sval); break;
        default: break;
        }
        for (int q = 0; q < nd->n_operands; q++) { IR_t *o = nd->operands[q]; if (o && o->op == IR_VAR && IR_LIT(o).sval) (void)gva_collect_var(IR_LIT(o).sval); }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void gva_collect_icon_globals(void) {
    extern const char *global_names[]; extern int global_count;
    for (int i = 0; i < global_count; i++) if (global_names[i]) (void)gva_collect_var(global_names[i]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int gvar_flat_chain_build_text_at(IR_graph_t *g, IR_t *entry_node, FILE *out, const char *prefix) {
    if (!g) return 1;
    IR_t *save_entry = g->entry;
    if (entry_node) g->entry = entry_node;
    int rc = gvar_flat_chain_build_text(g, out, prefix);
    g->entry = save_entry;
    return rc;
}
/*--------------------------------------------------------------------------------------------------------------------*/
bb_box_fn pl_gz_build(IR_t *gz_root) {
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_slot_count = 16; g_flat_node_id = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    emitter_init_binary(buf, FLAT_BUF_MAX);
    bb_label_t lbl_γ, lbl_ω, lbl_β;
    emit_label_initf(&lbl_γ, "gz_main_γ");
    emit_label_initf(&lbl_ω, "gz_main_ω");
    emit_label_initf(&lbl_β, "gz_main_β");
    walk_bb_flat(gz_root, &lbl_γ, &lbl_ω, &lbl_β);
    int nbytes = emitter_end();
    extern int bb_emit_overflow;
    if (bb_emit_overflow || nbytes <= 0 || nbytes > FLAT_BUF_MAX) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    bb_seal(buf, (size_t)nbytes);
    bb_pool_trim_last(buf, FLAT_BUF_MAX, (size_t)nbytes);
    return (bb_box_fn)buf;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int pl_gz_codegen(IR_t *gz_root, FILE *out, const char *prefix) {
    g_flat_slot_count = 16; g_flat_node_id = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    bb_label_t lbl_α, lbl_γ, lbl_ω, lbl_β;
    emit_label_initf(&lbl_α, "%s_α", prefix);
    emit_label_initf(&lbl_γ, "%s_gz_γ", prefix);
    emit_label_initf(&lbl_ω, "%s_gz_ω", prefix);
    emit_label_initf(&lbl_β, "%s_gz_β", prefix);
    emit_label_define_bb(&lbl_α);
    walk_bb_flat(gz_root, &lbl_γ, &lbl_ω, &lbl_β);
    emitter_end();
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
bb_box_fn bb_build_flat(IR_t *nd) {
    if (!g_in_prebuild) { g_child_cache_n = 0; g_in_prebuild = 1; pre_build_children(nd); g_in_prebuild = 0; }
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_slot_count = 0; g_flat_node_id = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_flat_body(nd, "pat_flat", 0, 0);
    int nbytes = emitter_end();
    extern int bb_emit_overflow;
    if (bb_emit_overflow || nbytes <= 0 || nbytes > FLAT_BUF_MAX) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    bb_seal(buf, (size_t)nbytes);
    bb_pool_trim_last(buf, FLAT_BUF_MAX, (size_t)nbytes);
    return (bb_box_fn)buf;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int codegen_flat_build(IR_t *nd, FILE *out, const char *prefix) {
    g_child_cache_n = 0;
    g_text_child_counter = 0;
    pre_build_children_text(nd, out, prefix);
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    int rc = codegen_flat_body(nd, prefix, 1, 0);
    emitter_end();
    return rc;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define PL_CATCH_MAX 64
static IR_t *g_pl_catch_nodes[PL_CATCH_MAX];
static int   g_pl_catch_n = 0;

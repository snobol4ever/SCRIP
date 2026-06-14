#include "emit_bb.h"
#include "BB_templates/bb_templates.h"
#include "emit_ir.h"
#include "emit_form.h"
#include "emit_templates.h"
#include "emit_globals.h"
#include "XA.h"
#include "../runtime/builtins/gen.h"
#include "IR.h"
#include "IR_interp_state.h"
#include "../rt/rt.h"
#include "bb_build.h"
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
extern DESCR_t gen_bb_not(void*,int);
extern DESCR_t gen_bb_repalt(void*,int);
extern DESCR_t gen_bb_while_gen(void*,int);
extern DESCR_t gen_bb_until_gen(void*,int);
extern DESCR_t gen_bb_repeat_gen(void*,int);
extern DESCR_t gen_bb_case_gen(void*,int);
extern DESCR_t gen_bb_compound_gen(void*,int);
extern DESCR_t gen_bb_field_gen(void*,int);     extern field_gen_state_t   *field_gen_new(void);
extern DESCR_t gen_bb_section_gen(void*,int);   extern section_gen_state_t *section_gen_new(void);
extern DESCR_t gen_bb_key_gen(void*,int);       extern kw_gen_state_t      *kw_gen_new(void);
extern DESCR_t gen_bb_listcon_gen(void*,int);   extern listcon_state_t     *listcon_gen_new(void);
extern DESCR_t gen_bb_proc_call(void*,int);      extern proc_call_state_t   *proc_call_new(void);
extern DESCR_t gen_bb_noop(void*,int);          extern noop_state_t        *noop_new(void);
extern DESCR_t gen_bb_intlit(void*,int);        extern intlit_state_t      *intlit_new(void);
extern DESCR_t gen_bb_reallit(void*,int);       extern reallit_state_t     *reallit_new(void);
extern DESCR_t gen_bb_strlit(void*,int);        extern strlit_state_t      *strlit_new(void);
extern DESCR_t gen_bb_csetlit(void*,int);       extern csetlit_state_t     *csetlit_new(void);
extern DESCR_t gen_bb_global(void*,int);        extern global_state_t      *global_new(void);
extern DESCR_t gen_bb_if_bb(void*,int);         extern if_state_t          *if_new(void);
extern DESCR_t gen_bb_initial(void*,int);       extern initial_state_t     *initial_new(void);
extern DESCR_t gen_bb_invocable(void*,int);     extern invocable_state_t   *invocable_new(void);
extern DESCR_t gen_bb_link(void*,int);          extern link_state_t        *link_new(void);
extern DESCR_t gen_bb_record_bb(void*,int);     extern record_state_t      *record_new(void);
extern DESCR_t gen_bb_return_bb(void*,int);     extern return_state_t      *return_new(void);
extern DESCR_t gen_bb_fail_bb(void*,int);       extern fail_state_t        *fail_new(void);
extern DESCR_t gen_bb_unop(void*,int);          extern unop_state_t        *unop_new(void);
extern DESCR_t gen_bb_next_bb(void*,int);       extern next_state_t        *next_new(void);
extern DESCR_t gen_bb_break_bb(void*,int);      extern break_state_t       *break_new(void);
extern DESCR_t gen_bb_create(void*,int);        extern create_state_t      *create_new(void);
extern DESCR_t gen_bb_coexplist(void*,int);     extern coexplist_state_t   *coexplist_new(void);
extern DESCR_t gen_bb_arglist(void*,int);       extern arglist_state_t     *arglist_new(void);
extern DESCR_t gen_bb_procdecl(void*,int);      extern procdecl_state_t    *procdecl_new(void);
extern DESCR_t gen_bb_procbody(void*,int);      extern procbody_state_t    *procbody_new(void);
extern DESCR_t gen_bb_proccode(void*,int);      extern proccode_state_t    *proccode_new(void);
extern atp_t    * bb_atp_new                (const char *varname);
extern cap_t    * bb_cap_new_call           (bb_box_fn child_fn, void *child_state, const char *fnc_name, DESCR_t *fnc_args, int fnc_nargs, char **fnc_arg_names, int fnc_n_arg_names, int immediate);
extern cap_t    * bb_cap_new                (bb_box_fn child_fn, void *child_state, const char *varname, DESCR_t *var_ptr, int immediate);
void (*g_cap_fixup_cb)(void *cap_ptr, const char *child_α_label) = NULL;
const char *child_cache_get_lbl(bb_box_fn fn);
#define FLAT_BUF_MAX  (256 * 1024)
int g_flat_node_id   = 0;
static int g_flat_slot_count = 0;
#define BB_SLOTMAP_MAX 512
static struct { IR_t *key; int off; } g_bb_slotmap[BB_SLOTMAP_MAX];
static int g_bb_slotmap_n = 0;
int bb_slot_alloc(IR_t *nd) {
    int off = g_flat_slot_count;
    g_flat_slot_count += 8;
    if (g_bb_slotmap_n < BB_SLOTMAP_MAX) { g_bb_slotmap[g_bb_slotmap_n].key = nd; g_bb_slotmap[g_bb_slotmap_n].off = off; g_bb_slotmap_n++; }
    return off;
}
int bb_slot_alloc16(IR_t *nd) {
    int off = g_flat_slot_count;
    g_flat_slot_count += 16;
    if (g_bb_slotmap_n < BB_SLOTMAP_MAX) { g_bb_slotmap[g_bb_slotmap_n].key = nd; g_bb_slotmap[g_bb_slotmap_n].off = off; g_bb_slotmap_n++; }
    return off;
}
/* Like bb_slot_alloc16 but returns the existing slot if the node already has one.
   Used when a node may be walked twice (chain pre-walk + generator re-walk). */
int bb_slot_alloc16_or_get(IR_t *nd) {
    int existing = bb_slot_get(nd);
    if (existing >= 0) return existing;
    int off = g_flat_slot_count;
    g_flat_slot_count += 16;
    if (g_bb_slotmap_n < BB_SLOTMAP_MAX) { g_bb_slotmap[g_bb_slotmap_n].key = nd; g_bb_slotmap[g_bb_slotmap_n].off = off; g_bb_slotmap_n++; }
    return off;
}
int bb_slot_alloc24(IR_t *nd) {
    int off = g_flat_slot_count;
    g_flat_slot_count += 24;
    if (g_bb_slotmap_n < BB_SLOTMAP_MAX) { g_bb_slotmap[g_bb_slotmap_n].key = nd; g_bb_slotmap[g_bb_slotmap_n].off = off; g_bb_slotmap_n++; }
    return off;
}
int bb_slot_alloc32(IR_t *nd) {
    int off = g_flat_slot_count;
    g_flat_slot_count += 32;
    if (g_bb_slotmap_n < BB_SLOTMAP_MAX) { g_bb_slotmap[g_bb_slotmap_n].key = nd; g_bb_slotmap[g_bb_slotmap_n].off = off; g_bb_slotmap_n++; }
    return off;
}
int bb_slot_get(IR_t *nd) {
    for (int i = 0; i < g_bb_slotmap_n; i++) if (g_bb_slotmap[i].key == nd) return g_bb_slotmap[i].off;
    return -1;
}
void bb_slot_register(IR_t *nd, int off) {
    if (g_bb_slotmap_n < BB_SLOTMAP_MAX) { g_bb_slotmap[g_bb_slotmap_n].key = nd; g_bb_slotmap[g_bb_slotmap_n].off = off; g_bb_slotmap_n++; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_slot_claim(int bytes) {
    int off = g_flat_slot_count;
    g_flat_slot_count += bytes;
    return off;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define BB_VARSLOT_MAX 256
static struct { const char *name; int off; } g_bb_varslot[BB_VARSLOT_MAX];
static int g_bb_varslot_n = 0;
int bb_varslot(const char *name) {
    if (!name) name = "";
    for (int i = 0; i < g_bb_varslot_n; i++)
        if (g_bb_varslot[i].name && strcmp(g_bb_varslot[i].name, name) == 0) return g_bb_varslot[i].off;
    int off = g_flat_slot_count;
    g_flat_slot_count += 16;
    if (g_bb_varslot_n < BB_VARSLOT_MAX) { g_bb_varslot[g_bb_varslot_n].name = name; g_bb_varslot[g_bb_varslot_n].off = off; g_bb_varslot_n++; }
    return off;
}
int bb_varslot_peek(const char *name) {
    if (!name) name = "";
    for (int i = 0; i < g_bb_varslot_n; i++)
        if (g_bb_varslot[i].name && strcmp(g_bb_varslot[i].name, name) == 0) return g_bb_varslot[i].off;
    return -1;
}
int g_descr_flat_chain = 0;
int g_gvar_flat_chain = 0;
int g_gvar_callarg_live = 0;
int g_emit_frame_caller_dl = -1;
int g_frame_active = 0;
int g_scan_regs_live = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
#define FLAT_CHAIN_SET_MAX 512
static IR_t *g_flat_chain_set[FLAT_CHAIN_SET_MAX];
static int   g_flat_chain_set_n = 0;
static int flat_chain_set_has(IR_t *nd) {
    for (int i = 0; i < g_flat_chain_set_n; i++) if (g_flat_chain_set[i] == nd) return 1;
    return 0;
}
int                 g_subject_slot       = -1;
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
static void data_buf_three_col(const char *lbl, const char *act, const char *got) {
    const char *L = lbl ? lbl : "";
    const char *A = act ? act : "";
    const char *G = got ? got : "";
    if (g_flat_data_pending_lbl[0]) {
        data_buf_appendf("%s\n", g_flat_data_pending_lbl);
        g_flat_data_pending_lbl[0] = '\0';
    }
    char line[576]; int o = 0; int first = 1;
    if (*L) { o += snprintf(line+o, sizeof(line)-o, "%s", L); first = 0; }
    if (*A) { o += snprintf(line+o, sizeof(line)-o, "%s%s", first?"":" ", A); first = 0; }
    if (*G) { o += snprintf(line+o, sizeof(line)-o, "%s%s", first?"":" ", G); }
    data_buf_appendf("%s\n", line);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void data_buf_pend_label(const char *name) {
    if (g_flat_data_pending_lbl[0])
        data_buf_appendf("%s\n", g_flat_data_pending_lbl);
    snprintf(g_flat_data_pending_lbl, sizeof(g_flat_data_pending_lbl), "%s:", name ? name : "");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void data_buf_flush_pending_label(void) {
    if (!g_flat_data_pending_lbl[0]) return;
    data_buf_appendf("%s\n", g_flat_data_pending_lbl);
    g_flat_data_pending_lbl[0] = '\0';
}
/*--------------------------------------------------------------------------------------------------------------------*/
void lower_flat_set_cap_fixup(void (*cb)(void *cap_ptr, const char *child_α_label)) { g_cap_fixup_cb = cb; }
#define SYM_SIGMA   "\xCE\xA3"
#define SYM_SIGLEN  "\xCE\xA3""len"
#define SYM_DELTA   "\xCE\x94"
#define ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
#define ADDR_SIGLEN  ((uint64_t)(uintptr_t)&Σlen)
#define ADDR_DELTA   ((uint64_t)(uintptr_t)&Δ)
static const char *(*g_flat_intern_str)(const char *s) = NULL;
void lower_flat_set_intern_str(const char *(*fn)(const char *)) { g_flat_intern_str = fn; }
const char *emit_intern_str(const char *s) {
    return (g_flat_intern_str && g_is_text) ? g_flat_intern_str(s) : NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void data_buf_remember_label(const char *name) {
    if (g_flat_data_block_nlbls >= FLAT_DATA_LBL_MAX) return;
    snprintf(g_flat_data_block_lbls[g_flat_data_block_nlbls], sizeof(g_flat_data_block_lbls[0]), "%s", name ? name : "");
    g_flat_data_block_nlbls++;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void data_buf_emit_block_comment(void) { g_flat_data_block_nlbls = 0; }
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
static void descr_chain_operand_refs(IR_t *entry);
static void gvar_stmt_operand_refs(IR_t *head);
static IR_t * bb_child0(const IR_t *n) { return (n && n->n_operands > 0) ? n->operands[0] : NULL; }
static IR_t * bb_child1(const IR_t *n) { return (n && n->n_operands > 1) ? n->operands[1] : NULL; }
extern int memcmp(const void *, const void *, size_t);
static bb_label_t g_α_ring[8];
static int        g_α_ring_i = 0;
static int        g_bb_alpha_seq = 0;
static void bb_fill_alpha(IR_t *nd) {
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
    if (nd && nd->op == IR_CALL_DEFINE) { static char defpool[64]; g_emit.op_parts_lbl[0] = NULL;
      int64_t narg = IR_LIT(nd).ival;
      IR_graph_t ** subs = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
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
static void flat_drive_cat_arms(IR_t *pBB, IR_t * const * arms, IR_t * const * joins, int nc, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int id = g_flat_node_id++;
    bb_label_t *mid_γ   = emit_label_alloc("xcat%d_γ",       id);
    bb_label_t *right_ω = emit_label_alloc("xcat%d_right_ω", id);
    bb_label_t *left_β  = emit_label_alloc("xcat%d_left_β",  id);
    bb_label_t *right_β = emit_label_alloc("xcat%d_right_β", id);
    bb_label_t *xcat_ω  = emit_label_alloc("xcat%d_ω",       id);
    EMIT_PAIR_RESET();
    if (nc == 0) {
        EMIT_PAIR_JMP(lbl_γ);
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_DEF_JMP(xcat_ω, lbl_ω);
        EMIT_PAIR_DEF(mid_γ); EMIT_PAIR_DEF(right_ω); EMIT_PAIR_DEF(right_β); EMIT_PAIR_DEF(left_β);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    if (nc == 1) {
        if (joins && joins[0]) emit_cat_diamond(arms[0], joins[0], lbl_γ, lbl_ω, left_β, id, 0);
        else walk_bb_flat(arms ? arms[0] : bb_match_kid(pBB, 0), lbl_γ, lbl_ω, left_β);
        EMIT_PAIR_DEF_JMP(lbl_β, left_β);
        EMIT_PAIR_DEF_JMP(xcat_ω, lbl_ω);
        EMIT_PAIR_DEF(mid_γ); EMIT_PAIR_DEF(right_ω); EMIT_PAIR_DEF(right_β);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    if (joins && joins[0]) emit_cat_diamond(arms[0], joins[0], mid_γ, xcat_ω, left_β, id, 0);
    else walk_bb_flat(arms ? arms[0] : bb_match_kid(pBB, 0), mid_γ, xcat_ω, left_β);
    emit_label_define_bb(mid_γ);
    bb_label_t *last_β = right_β;
    if (nc == 2) {
        if (joins && joins[1]) emit_cat_diamond(arms[1], joins[1], lbl_γ, right_ω, right_β, id, 1);
        else walk_bb_flat(arms ? arms[1] : bb_match_kid(pBB, 1), lbl_γ, right_ω, right_β);
    } else {
        bb_label_t **mids  = (bb_label_t **)alloca(sizeof(bb_label_t *) * (nc - 1));
        bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * (nc - 1));
        for (int i = 0; i < nc - 1; i++) {
            mids[i]  = emit_label_alloc("xcat%d_mid%d_γ", id, i+1);
            betas[i] = emit_label_alloc("xcat%d_mid%d_β", id, i+1);
        }
        for (int i = 1; i < nc; i++) {
            bb_label_t *s = (i < nc-1) ? mids[i-1] : lbl_γ;
            bb_label_t *kid_ω = (i == 1) ? left_β : betas[i-2];
            if (joins && joins[i]) emit_cat_diamond(arms[i], joins[i], s, kid_ω, betas[i-1], id, i);
            else walk_bb_flat(arms ? arms[i] : bb_match_kid(pBB, i), s, kid_ω, betas[i-1]);
            if (i < nc-1) emit_label_define_bb(mids[i-1]);
        }
        last_β = betas[nc-2];
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(right_ω, left_β);
    EMIT_PAIR_DEF_JMP(lbl_β, last_β);
    EMIT_PAIR_DEF_JMP(xcat_ω, lbl_ω);
    if (!pBB) {
        emit_label_define_bb(right_ω); emit_jmp_label(left_β, JMP_JMP);
        emit_label_define_bb(lbl_β);   emit_jmp_label(last_β, JMP_JMP);
        emit_label_define_bb(xcat_ω);  emit_jmp_label(lbl_ω,  JMP_JMP);
        return;
    }
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_cat(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    flat_drive_cat_arms(pBB, NULL, NULL, pBB ? bb_match_nkids(pBB) : 0, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_alt(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int id = g_flat_node_id++;
    int nc = pBB ? bb_match_nkids(pBB) : 0;
    IR_t * const * arms = NULL;
    if (nc == 0 && pBB) { int na = 0; IR_t * const * aux = bb_operand_aux_get(g_emit_cfg, pBB, &na); if (aux && na > 0) { arms = aux; nc = na; } }
    EMIT_PAIR_RESET();
    if (nc == 0) { EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); return; }
    if (nc == 1) { walk_bb_flat(arms ? arms[0] : bb_match_kid(pBB, 0), lbl_γ, lbl_ω, lbl_β); return; }
    bb_label_t **ci_βs = (bb_label_t **)alloca((size_t)nc * sizeof(bb_label_t *));
    bb_label_t **ci_ωs = (bb_label_t **)alloca((size_t)nc * sizeof(bb_label_t *));
    for (int i = 0; i < nc; i++) {
        ci_βs[i] = emit_label_alloc("alt%d_c%d_β", id, i);
        ci_ωs[i] = emit_label_alloc("alt%d_c%d_ω", id, i);
    }
    for (int i = 0; i < nc; i++) {
        bb_label_t *f = (i < nc-1) ? ci_ωs[i] : ci_ωs[nc-1];
        walk_bb_flat(arms ? arms[i] : bb_match_kid(pBB, i), lbl_γ, f, ci_βs[i]);
        if (i < nc-1) emit_label_define_bb(ci_ωs[i]);
        else          emit_label_define_bb(ci_ωs[nc-1]);
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_JMP(lbl_ω);
    EMIT_PAIR_DEF_JMP(lbl_β, ci_βs[0]);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_fence(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || bb_match_nkids(pBB) == 0) {
        bb_label_t *seal = (pBB && IR_LIT(pBB).ival == 1 && g_scan_seal_lbl) ? g_scan_seal_lbl : lbl_ω;
        EMIT_PAIR_RESET();
        EMIT_PAIR_JMP(lbl_γ);
        EMIT_PAIR_DEF_JMP(lbl_β, seal);
        g_emit.x86_scratch_off = bb_slot_claim(4);
        EMIT_PAIR_FILL(pBB, lbl_γ, seal, lbl_β);
        return;
    }
    int id = g_flat_node_id++;
    bb_label_t *child_γ = emit_label_alloc("xfnce%d_γ", id);
    bb_label_t *child_ω = emit_label_alloc("xfnce%d_ω", id);
    walk_bb_flat(bb_match_kid(pBB, 0), child_γ, child_ω, lbl_β);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(child_γ, lbl_γ);
    EMIT_PAIR_DEF_JMP(child_ω, lbl_ω);
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    g_emit.x86_scratch_off = bb_slot_claim(4);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
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
static void flat_drive_capture(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *ch = (pBB && bb_match_nkids(pBB) > 0) ? bb_match_kid(pBB, 0) : (pBB && pBB->n_operands > 0 ? pBB->operands[0] : NULL);
    if (!ch && pBB) { int na = 0; IR_t * const * aux = bb_operand_aux_get(g_emit_cfg, pBB, &na); if (aux && na > 0) ch = aux[0]; }
    const char *vn = (pBB && IR_LIT(pBB).sval) ? IR_LIT(pBB).sval : "";
    if (!ch) {
        EMIT_PAIR_RESET();
        EMIT_PAIR_JMP(lbl_γ);
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    if (!vn[0]) { walk_bb_flat(ch, lbl_γ, lbl_ω, lbl_β); return; }
    int id = g_flat_node_id++;
    bb_label_t *cap_γ = emit_label_alloc("xcap%d_γ", id);
    int st = bb_slot_alloc16(pBB);
    EMIT_PAIR_RESET();
    g_emit.op_off = st;
    IR_LIT(pBB).ival = 0;
    EMIT_PAIR_FILL(pBB, cap_γ, lbl_ω, lbl_β);
    {
        IR_t *cat_arms[64]; IR_t *catnd = NULL;
        int catn = gather_lowered_cat_arms(ch, cat_arms, NULL, 64, &catnd, pBB);
        IR_t *stop_arms[64]; int stop_n = 0; int stop_has_arbno = 0;
        if (catn == 0) { IR_t *sc = ch; while (sc && sc != pBB && is_pat_chain_elem(sc->op) && stop_n < 64) { if (sc->op == IR_PAT_ARBNO) stop_has_arbno = 1; stop_arms[stop_n++] = sc; sc = sc->γ.node; } }
        if (catn >= 2) {
            flat_drive_cat_arms(catnd, cat_arms, NULL, catn, cap_γ, lbl_ω, lbl_β);
        } else if (stop_n >= 2 && stop_has_arbno) {
            flat_drive_cat_arms(NULL, stop_arms, NULL, stop_n, cap_γ, lbl_ω, lbl_β);
        } else if (ch->γ.node && ch->γ.node->op == IR_PAT_ALT) {
            int na = 0; IR_t * const * aux = bb_operand_aux_get(g_emit_cfg, ch->γ.node, &na);
            if (aux && na > 0) {
                walk_bb_flat(ch->γ.node, cap_γ, lbl_ω, lbl_β);
            } else {
                IR_t *alt_arms[64]; int na2 = gather_inline_alt_arms(ch, alt_arms, 64);
                if (na2 >= 2) {
                    bb_label_t **ai_ωs = (bb_label_t **)alloca((size_t)na2 * sizeof(bb_label_t *));
                    bb_label_t **ai_βs = (bb_label_t **)alloca((size_t)na2 * sizeof(bb_label_t *));
                    for (int i = 0; i < na2; i++) {
                        ai_ωs[i] = emit_label_alloc("icap%d_a%d_ω", id, i);
                        ai_βs[i] = emit_label_alloc("icap%d_a%d_β", id, i);
                    }
                    for (int i = 0; i < na2; i++) {
                        bb_label_t *arm_ω = (i < na2-1) ? ai_ωs[i] : lbl_ω;
                        walk_bb_flat(alt_arms[i], cap_γ, arm_ω, ai_βs[i]);
                        if (i < na2-1) emit_label_define_bb(ai_ωs[i]);
                    }
                } else {
                    walk_bb_flat(ch, cap_γ, lbl_ω, lbl_β);
                }
            }
        } else {
            walk_bb_flat(ch, cap_γ, lbl_ω, lbl_β);
        }
    }
    emit_label_define_bb(cap_γ);
    EMIT_PAIR_RESET();
    g_emit.op_off = st;
    IR_LIT(pBB).ival = (pBB->op == IR_PAT_ASSIGN_IMM) ? 2 : 1;
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
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
static int gz_node_bounded(const IR_t *g) {
    if (!g) return 1;
    switch (g->op) {
    case IR_CELL_CALL: case IR_CELL_CHOICE: case IR_CELL_FINDALL: case IR_CELL_ITE:
    case IR_CELL_CUT:  case IR_CUT:
        return 0;   /* generators + cut-barrier: never collapse their β */
    default: return 1;
    }
}
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
static void gz_callee_labels(pl_gz_callee_t *ce, pl_gz_callee_t **callees, int *ncallees) {
    if (!ce || ce->lblA) return;
    int cid = g_flat_node_id++;
    ce->lblA = (void *)emit_label_alloc("gzp%d_α", cid);
    ce->lblB = (void *)emit_label_alloc("gzp%d_β", cid);
    if (*ncallees < 8) callees[(*ncallees)++] = ce;
}
static void gz_collect_callees(IR_t *head, pl_gz_callee_t **callees, int *ncallees) {
    for (IR_t *g = head; g; g = g->γ.node) {
        if (g->op == IR_CELL_CALL) {
            pl_gz_call_state_t *cs = (pl_gz_call_state_t *)(intptr_t)IR_LIT(g).ival;
            if (cs) gz_callee_labels(cs->callee, callees, ncallees);
        }
        if (g->op == IR_CELL_FINDALL) {
            pl_gz_findall_state_t *fst = (pl_gz_findall_state_t *)(intptr_t)IR_LIT(g).ival;
            if (fst && fst->call) gz_callee_labels(fst->call->callee, callees, ncallees);
        }
        if (g->op == IR_CELL_ITE) {
            pl_gz_ite_state_t *is = (pl_gz_ite_state_t *)(intptr_t)IR_LIT(g).ival;
            if (is) { gz_collect_callees(is->cond_head, callees, ncallees); gz_collect_callees(is->then_head, callees, ncallees); gz_collect_callees(is->else_head, callees, ncallees); }
        }
    }
}
static void gz_emit_cell(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_t **callees, int *ncallees);
static bb_label_t * gz_emit_chain(IR_t *head, bb_label_t *chain_γ, bb_label_t *chain_ω, bb_label_t *cut_ω, bb_label_t *entry, pl_gz_callee_t **callees, int *ncallees) {
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
        gz_emit_cell(g, next_γ, gw, cb[i], cut_ω, callees, ncallees);
        g_emit.op_bounded = 0;
        i++;
    }
    return cb[n - 1];
}
static void gz_emit_ite(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_t **callees, int *ncallees) {
    pl_gz_ite_state_t *is = (pl_gz_ite_state_t *)(intptr_t)IR_LIT(g).ival;
    g_emit.op_bounded = 0;
    int cid = g_flat_node_id++;
    bb_label_t *Lg1 = emit_label_alloc("gzi%d_c1", cid);
    bb_label_t *Lg2 = emit_label_alloc("gzi%d_c2", cid);
    bb_label_t *E2a = is && is->then_head ? emit_label_alloc("gzi%d_t", cid) : NULL;
    bb_label_t *E3a = is && is->else_head ? emit_label_alloc("gzi%d_e", cid) : NULL;
    g_emit.op_sa = 0; g_emit.op_sb = 0; g_emit.op_sval = NULL; g_emit.op_ival = 0; g_emit.op_off = 0;
    FILL(g, Lg1, Lg2, gβ);
    if (is) (void)gz_emit_chain(is->cond_head, Lg1, Lg2, cut_ω, NULL, callees, ncallees);
    emit_label_define_bb(Lg1);
    g_emit.op_sa = 1;
    { bb_label_t *d = E2a ? E2a : next_γ; g_emit.lbl_t0 = d->name; g_emit.lbl_t0_p = d; }
    FILL(g, next_γ, gw, gβ);
    emit_label_define_bb(Lg2);
    g_emit.op_sa = 2;
    { bb_label_t *d = E3a ? E3a : next_γ; g_emit.lbl_t0 = d->name; g_emit.lbl_t0_p = d; }
    FILL(g, next_γ, gw, gβ);
    bb_label_t *tβ = is ? gz_emit_chain(is->then_head, next_γ, gw, cut_ω, E2a, callees, ncallees) : NULL;
    bb_label_t *eβ = is ? gz_emit_chain(is->else_head, next_γ, gw, cut_ω, E3a, callees, ncallees) : NULL;
    g_emit.op_sa = 3;
    { bb_label_t *d = tβ ? tβ : gw; g_emit.lbl_t0 = d->name; g_emit.lbl_t0_p = d; }
    { bb_label_t *e = eβ ? eβ : gw; g_emit.lbl_t1 = e->name; g_emit.lbl_t1_p = e; }
    FILL(g, next_γ, gw, gβ);
    g_emit.op_sa = 0;
}
static void gz_emit_cell(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_t **callees, int *ncallees) {
    if (g->op == IR_CELL_ITE) { gz_emit_ite(g, next_γ, gw, gβ, cut_ω, callees, ncallees); return; }
    gz_fill_goal(g, next_γ, gw, gβ);
}
static IR_t * gz_clause_head_of(pl_gz_callee_t *ce, int c) {
    return ce->nclauses > 1 ? ce->clause_head[c] : ce->body_head;
}
static void gz_emit_callee(pl_gz_callee_t *ce, pl_gz_callee_t **callees, int *ncallees) {
    if (!ce || ce->body_emitted) return;
    ce->body_emitted = 1;
    int NC = ce->nclauses > 0 ? ce->nclauses : 1;
    for (int c = 0; c < NC; c++)
        gz_collect_callees(gz_clause_head_of(ce, c), callees, ncallees);
    int cid = g_flat_node_id++;
    bb_label_t *cl_γ = emit_label_alloc("gzp%d_γ", cid);
    bb_label_t *cl_ω = emit_label_alloc("gzp%d_ω", cid);
    int nb[8]; int nbtot = 0;
    for (int c = 0; c < NC; c++) { nb[c] = 0; for (IR_t *g = gz_clause_head_of(ce, c); g; g = g->γ.node) nb[c]++; nbtot += nb[c]; }
    bb_label_t **pgl = (bb_label_t **)alloca(sizeof(bb_label_t *) * (nbtot > 0 ? nbtot : 1));
    bb_label_t **pgb = (bb_label_t **)alloca(sizeof(bb_label_t *) * (nbtot > 0 ? nbtot : 1));
    bb_label_t *cladv[8]; bb_label_t *redo[8]; int cbase[8];
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
            gz_emit_cell(g, next_γ, gw, pgb[j], cl_ω, callees, ncallees);
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
static void flat_drive_gz_query(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int id = g_flat_node_id++;
    IR_t *hd = bb_child0(pBB), *hdB = bb_child1(pBB);
    int twoseg = (IR_LIT(pBB).dval == 2.0 && hdB != NULL);
    bb_label_t *land_γ = emit_label_alloc("gzq%d_γ", id);
    bb_label_t *land_ω = emit_label_alloc("gzq%d_ω", id);
    bb_label_t *soft_ω = twoseg ? emit_label_alloc("gzq%d_s\xcf\x89", id) : land_ω;
    int n = 0;
    for (IR_t *g = hd; g; g = g->γ.node) n++;
    int nB = 0;
    for (IR_t *g = twoseg ? hdB : NULL; g; g = g->γ.node) nB++;
    bb_label_t **gl = (bb_label_t **)alloca(sizeof(bb_label_t *) * (n > 0 ? n : 1));
    bb_label_t **gb = (bb_label_t **)alloca(sizeof(bb_label_t *) * (n > 0 ? n : 1));
    bb_label_t **hl = (bb_label_t **)alloca(sizeof(bb_label_t *) * (nB > 0 ? nB : 1));
    bb_label_t **hb = (bb_label_t **)alloca(sizeof(bb_label_t *) * (nB > 0 ? nB : 1));
    pl_gz_callee_t *callees[8]; int ncallees = 0;
    int i = 0;
    for (IR_t *g = hd; g; g = g->γ.node) {
        gl[i] = emit_label_alloc("gzq%d_g%d_α", id, i); gb[i] = emit_label_alloc("gzq%d_g%d_β", id, i); i++;
    }
    i = 0;
    for (IR_t *g = twoseg ? hdB : NULL; g; g = g->γ.node) {
        hl[i] = emit_label_alloc("gzq%d_b%d_α", id, i); hb[i] = emit_label_alloc("gzq%d_b%d_β", id, i); i++;
    }
    gz_collect_callees(hd, callees, &ncallees);
    if (twoseg) gz_collect_callees(hdB, callees, &ncallees);
    g_emit.op_sa = 0;
    g_emit.op_sb = 0;
    g_emit.op_ival = IR_LIT(pBB).ival;
    FILL(pBB, (n > 0 ? gl[0] : land_γ), soft_ω, lbl_β);
    i = 0;
    IR_t *qgn[256]; int qgc = 0;
    for (IR_t *g = hd; g && qgc < 256; g = g->γ.node) qgn[qgc++] = g;
    for (IR_t *g = hd; g; g = g->γ.node) {
        emit_label_define_bb(gl[i]);
        bb_label_t *next_γ = (i + 1 < n) ? gl[i + 1] : ((twoseg && nB > 0) ? hl[0] : land_γ);
        int pp = i - 1; if (pp >= qgc) pp = qgc - 1;
        while (pp >= 0 && gz_node_bounded(qgn[pp])) pp--;
        bb_label_t *gw = (pp < 0) ? soft_ω : gb[pp];
        g_emit.op_bounded = gz_node_bounded(g) ? 1 : 0;
        gz_emit_cell(g, next_γ, gw, gb[i], NULL, callees, &ncallees);
        g_emit.op_bounded = 0;
        i++;
    }
    if (twoseg) {
        bb_label_t *bcont = (nB > 0) ? hl[0] : land_γ;
        g_emit.op_sa = 1;
        g_emit.op_sb = 2;
        g_emit.lbl_t0 = bcont->name; g_emit.lbl_t0_p = bcont;
        FILL(pBB, land_γ, soft_ω, lbl_β);
        i = 0;
        IR_t *qbn[256]; int qbc = 0;
        for (IR_t *g = hdB; g && qbc < 256; g = g->γ.node) qbn[qbc++] = g;
        for (IR_t *g = hdB; g; g = g->γ.node) {
            emit_label_define_bb(hl[i]);
            bb_label_t *next_γ = (i + 1 < nB) ? hl[i + 1] : land_γ;
            int pp = i - 1; if (pp >= qbc) pp = qbc - 1;
            while (pp >= 0 && gz_node_bounded(qbn[pp])) pp--;
            bb_label_t *gw = (pp < 0) ? land_ω : hb[pp];
            g_emit.op_bounded = gz_node_bounded(g) ? 1 : 0;
            gz_emit_cell(g, next_γ, gw, hb[i], NULL, callees, &ncallees);
            g_emit.op_bounded = 0;
            i++;
        }
    }
    g_emit.op_sa = 1;
    g_emit.op_sb = twoseg ? 0 : ((IR_LIT(pBB).dval != 0.0) ? 1 : 0);
    FILL(pBB, land_γ, land_ω, lbl_β);
    for (int k = 0; k < ncallees; k++) gz_emit_callee(callees[k], callees, &ncallees);
    (void)lbl_γ; (void)lbl_ω;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_conj(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *goals[256];
    int n = pBB ? resolve_seq_goals_em(pBB, goals, 256) : 0;
    if (n <= 0) { EMIT_PAIR_RESET(); EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); return; }
    if (n == 1) { walk_bb_flat(goals[0], lbl_γ, lbl_ω, lbl_β); return; }
    int id = g_flat_node_id++;
    bb_label_t **gα    = (bb_label_t **)alloca((size_t)n * sizeof(bb_label_t *));
    bb_label_t **gβ    = (bb_label_t **)alloca((size_t)n * sizeof(bb_label_t *));
    bb_label_t **eff_β = (bb_label_t **)alloca((size_t)n * sizeof(bb_label_t *));
    for (int i = 0; i < n; i++) {
        gα[i] = emit_label_alloc("plseq%d_g%d_α", id, i);
        gβ[i] = emit_label_alloc("plseq%d_g%d_β", id, i);
    }
    {
        IR_e t0 = goals[0]->op;
        int r0 = (t0==IR_GOAL || t0==IR_CHOICE || t0==IR_DISJ);
        eff_β[0] = r0 ? gβ[0] : lbl_ω;
        for (int i = 1; i < n; i++) {
            IR_e ti = goals[i]->op;
            int ri = (ti==IR_GOAL || ti==IR_CHOICE || ti==IR_DISJ);
            eff_β[i] = ri ? gβ[i] : eff_β[i-1];
        }
    }
    for (int i = 0; i < n; i++) {
        bb_label_t *gi_γ = (i < n-1) ? gα[i+1] : lbl_γ;
        bb_label_t *gi_ω = (i == 0)  ? lbl_ω   : eff_β[i-1];
        emit_label_define_bb(gα[i]);
        walk_bb_flat(goals[i], gi_γ, gi_ω, gβ[i]);
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, eff_β[n-1]);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void resolve_choice_clause_label(char *dst, size_t dsz, int id, int ci, const char *suffix) {
    snprintf(dst, dsz, ".Lplch%d_c%d_%s", id, ci, suffix);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_choice(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *bodies[256];
    int n = pBB ? resolve_choice_bodies_em(pBB, bodies, 256) : 0;
    if (n <= 0) { EMIT_PAIR_RESET(); EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); return; }
    int id = g_flat_node_id++;
    bb_label_t **cbody = (bb_label_t **)alloca((size_t)n * sizeof(bb_label_t *));
    bb_label_t **cpre  = (bb_label_t **)alloca((size_t)n * sizeof(bb_label_t *));
    bb_label_t **cβ    = (bb_label_t **)alloca((size_t)n * sizeof(bb_label_t *));
    char nm[160];
    for (int i = 0; i < n; i++) {
        resolve_choice_clause_label(nm, sizeof nm, id, i, "body"); cbody[i] = emit_label_intern(nm);
        resolve_choice_clause_label(nm, sizeof nm, id, i, "pre");  cpre[i]  = emit_label_intern(nm);
        resolve_choice_clause_label(nm, sizeof nm, id, i, "beta"); cβ[i]    = emit_label_alloc("%s", nm);
    }
    g_emit.op_kind = "RESOLVE_CHOICE";
    g_emit.resolve_choice_id = id;
    g_emit.resolve_choice_n  = n;
    EMIT_PAIR_RESET();
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
    bb_label_t *exit_γ_lbl;
    { char nm2[160]; snprintf(nm2, sizeof nm2, ".Lplch%d_exit_γ", id); exit_γ_lbl = emit_label_intern(nm2); }
    for (int i = 0; i < n; i++) {
        emit_label_define_bb(cbody[i]);
        walk_bb_flat(bodies[i], exit_γ_lbl, lbl_β, cβ[i]);
    }
    (void)exit_γ_lbl;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_disj(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int n = 0;
    IR_t * const * arms = pBB ? bb_operand_aux_get(g_emit_cfg, pBB, &n) : NULL;
    if (!arms || n <= 0) { EMIT_PAIR_RESET(); EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); return; }
    int id = g_flat_node_id++;
    bb_label_t **cbody = (bb_label_t **)alloca((size_t)n * sizeof(bb_label_t *));
    bb_label_t **cpre  = (bb_label_t **)alloca((size_t)n * sizeof(bb_label_t *));
    bb_label_t **cβ    = (bb_label_t **)alloca((size_t)n * sizeof(bb_label_t *));
    char nm[160];
    for (int i = 0; i < n; i++) {
        resolve_choice_clause_label(nm, sizeof nm, id, i, "body"); cbody[i] = emit_label_intern(nm);
        resolve_choice_clause_label(nm, sizeof nm, id, i, "pre");  cpre[i]  = emit_label_intern(nm);
        resolve_choice_clause_label(nm, sizeof nm, id, i, "beta"); cβ[i]    = emit_label_alloc("%s", nm);
    }
    g_emit.op_kind = "RESOLVE_ALT";
    g_emit.resolve_choice_id = id;
    g_emit.resolve_choice_n  = n;
    EMIT_PAIR_RESET();
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
    for (int i = 0; i < n; i++) {
        bb_label_t *bi_ω = (i < n - 1) ? cpre[i + 1] : lbl_ω;
        emit_label_define_bb(cbody[i]);
        walk_bb_flat(arms[i], lbl_γ, bi_ω, cβ[i]);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int resolve_ite_entries_em(const IR_t *nd, IR_t **out_cond, IR_t **out_then, IR_t **out_else) {
    if (!nd || nd->op != IR_ITE) return 0;
    bb_ite_state_t *zi = (bb_ite_state_t *)(intptr_t)IR_LIT(nd).ival;
    if (!zi) return 0;
    if (out_cond) *out_cond = zi->cond;
    if (out_then) *out_then = zi->then_;
    if (out_else) *out_else = zi->else_;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t *ite_branch_walk_node(IR_t *entry, IR_t *root) {
    if (root && bb_kind_is_driver_owned(root->op)) return root;
    return entry;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_ite(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *cond = NULL, *thn = NULL, *els = NULL;
    if (!resolve_ite_entries_em(pBB, &cond, &thn, &els) || !cond) {
        EMIT_PAIR_RESET(); EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); return;
    }
    bb_ite_state_t *zi = (bb_ite_state_t *)(intptr_t)IR_LIT(pBB).ival;
    IR_t *cond_w = ite_branch_walk_node(cond, zi ? zi->cond_root : NULL);
    IR_t *thn_w  = ite_branch_walk_node(thn,  zi ? zi->then_root : NULL);
    IR_t *els_w  = ite_branch_walk_node(els,  zi ? zi->else_root : NULL);
    int id = g_flat_node_id++;
    bb_label_t *then_α = emit_label_alloc("xite%d_then_α", id);
    bb_label_t *else_α = emit_label_alloc("xite%d_else_α", id);
    bb_label_t *cond_β = emit_label_alloc("xite%d_cond_β", id);
    bb_label_t *then_β = emit_label_alloc("xite%d_then_β", id);
    bb_label_t *else_β = emit_label_alloc("xite%d_else_β", id);
    walk_bb_flat(cond_w, then_α, else_α, cond_β);
    emit_label_define_bb(then_α);
    if (thn) walk_bb_flat(thn_w, lbl_γ, lbl_ω, then_β); else emit_jmp_label(lbl_γ, JMP_JMP);
    emit_label_define_bb(else_α);
    if (els) walk_bb_flat(els_w, lbl_γ, lbl_ω, else_β); else emit_jmp_label(lbl_ω, JMP_JMP);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void resolve_call_block_label(char *dst, size_t dsz, const char *name, int arity) {
    char san[128]; size_t j = 0;
    size_t namelen = 0;
    if (name) { const char *slash = strrchr(name, '/'); namelen = slash ? (size_t)(slash - name) : strlen(name); }
    for (size_t i = 0; name && i < namelen && j + 1 < sizeof san; i++) {
        char c = name[i];
        san[j++] = ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') ? c : '_';
    }
    san[j] = '\0';
    snprintf(dst, dsz, ".Lplpred_%s_%d", san, arity);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void sub_label(char *dst, size_t dsz, const char *name) {
    char san[128]; size_t j = 0;
    for (size_t i = 0; name && name[i] && j + 1 < sizeof san; i++) {
        char c = name[i];
        san[j++] = ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') ? c : '_';
    }
    san[j] = '\0';
    snprintf(dst, dsz, ".Lrksub_%s", san);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int resolve_emit_callee_block_body(const char *name, int arity, bb_label_t *bγ, bb_label_t *bω, bb_label_t *bβ) {
    IR_t *pentry = resolve_bb_entry_node(name, arity);
    if (!pentry) return 0;
    walk_bb_flat(pentry, bγ, bω, bβ);
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_kind_is_driver_owned(int t) {
    return t == IR_PAT_CAT || t == IR_PAT_ALT || t == IR_PAT_FENCE || t == IR_GCONJ;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_prepare_capture_arbno(IR_t *nd, int imm) {
    if (!PLATFORM_X86) return;
    bb_box_fn   child_fn = (bb_box_fn)g_emit.child_fn;
    const char *name     = g_emit.op_name1;
    const char *fnc_name = (g_emit.op_name2 && g_emit.op_name2[0]) ? g_emit.op_name2 : NULL;
    g_emit.bb_rt_obj   = NULL;
    g_emit.bb_child_lbl = NULL;
    g_emit.bb_child_fn  = (void *)child_fn;
    if (nd->op == IR_PAT_ARBNO) {
        g_emit.bb_rt_obj = NULL;
        if (MEDIUM_TEXT) {
            emit_comment("# BOX ARBNO()");
            xa_dispatch(XA_BB_PTR_SLOT);
            g_emit.bb_child_lbl = child_fn ? child_cache_get_lbl(child_fn) : NULL;
            if (g_emit.bb_child_lbl && g_cap_fixup_cb) {
                char buf[256];
                snprintf(buf, sizeof(buf), "%s|%s", g_emit.bb_ptr_slot_lbl, g_emit.bb_child_lbl);
                g_cap_fixup_cb((void*)2, buf);
            }
        }
        return;
    }
    if (!name || name[0] == 0) return;
    if (fnc_name) {
        g_emit.bb_rt_obj = bb_cap_new_call(child_fn, NULL, fnc_name, NULL, 0, NULL, 0, 0);
        if (MEDIUM_TEXT) {
            char banner[320];
            snprintf(banner, sizeof(banner), "# BOX CALLCAP(%s)", fnc_name);
            emit_comment(banner);
            xa_dispatch(XA_BB_PTR_SLOT);
            g_emit.bb_child_lbl = child_fn ? child_cache_get_lbl(child_fn) : NULL;
            if (g_emit.bb_child_lbl && g_cap_fixup_cb) {
                char buf[512];
                snprintf(buf, sizeof(buf), "%s|%s|%s|0|1", g_emit.bb_ptr_slot_lbl, g_emit.bb_child_lbl, fnc_name);
                g_cap_fixup_cb((void*)1, buf);
            }
        }
        return;
    }
    g_emit.bb_rt_obj = bb_cap_new(child_fn, NULL, name, NULL, imm);
    if (MEDIUM_TEXT) {
        char banner[320];
        snprintf(banner, sizeof(banner), "# BOX %s(%s)", imm ? "CAP_IMM" : "CAP_COND", name);
        emit_comment(banner);
        xa_dispatch(XA_BB_PTR_SLOT);
        g_emit.bb_child_lbl = child_fn ? child_cache_get_lbl(child_fn) : NULL;
        if (g_emit.bb_child_lbl && g_cap_fixup_cb) {
            char buf[512];
            snprintf(buf, sizeof(buf), "%s|%s|%s|%s|0", g_emit.bb_ptr_slot_lbl, g_emit.bb_child_lbl, name, imm ? "1" : "0");
            g_cap_fixup_cb((void*)1, buf);
        }
    }
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
static int gz_arith_const_eval(const IR_t *nd, long *out) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_I) { *out = (long)IR_LIT(nd).ival; return 1; }
    const IR_t *a0 = ir_pair_arg(nd, 0), *a1 = ir_pair_arg(nd, 1);
    if (nd->op != IR_ARITH || !a0) return 0;
    const char *op = IR_LIT(nd).sval ? IR_LIT(nd).sval : "+";
    if (!a1) {
        long a = 0;
        if (!gz_arith_const_eval(a0, &a)) return 0;
        if (strcmp(op,"-")==0) { *out = -a; return 1; }
        if (strcmp(op,"+")==0) { *out =  a; return 1; }
        if (strcmp(op,"abs")==0) { *out = (a<0)?-a:a; return 1; }
        if (strcmp(op,"sign")==0) { *out = (a>0)?1:(a<0)?-1:0; return 1; }
        if (strcmp(op,"truncate")==0||strcmp(op,"integer")==0) { *out = a; return 1; }
        if (strcmp(op,"msb")==0) { if (a<=0) return 0; int r=0; long v=a; while(v>1){v>>=1;r++;} *out=r; return 1; }
        return 0;
    }
    long a = 0, b = 0;
    if (!gz_arith_const_eval(a0, &a) || !gz_arith_const_eval(a1, &b)) return 0;
    if (strcmp(op,"+")==0) { *out = a+b; return 1; }
    if (strcmp(op,"-")==0) { *out = a-b; return 1; }
    if (strcmp(op,"*")==0) { *out = a*b; return 1; }
    if (strcmp(op,"/")==0) { if (!b) return 0; *out = a/b; return 1; }
    if (strcmp(op,"//")==0) { if (!b) return 0; *out = a/b; return 1; }
    if (strcmp(op,"mod")==0||strcmp(op,"rem")==0) { if (!b) return 0; *out = a%b; return 1; }
    if (strcmp(op,"/\\")==0) { *out = a&b; return 1; }
    if (strcmp(op,"\\/")==0) { *out = a|b; return 1; }
    if (strcmp(op,"xor")==0) { *out = a^b; return 1; }
    if (strcmp(op,">>")==0) { *out = (b>=0&&b<64)?(a>>b):0; return 1; }
    if (strcmp(op,"<<")==0) { *out = (b>=0&&b<64)?(a<<b):0; return 1; }
    if (strcmp(op,"max")==0) { *out = (a>b)?a:b; return 1; }
    if (strcmp(op,"min")==0) { *out = (a<b)?a:b; return 1; }
    if (strcmp(op,"gcd")==0) { long x=a<0?-a:a,y=b<0?-b:b; while(y){long t=y;y=x%y;x=t;} *out=x; return 1; }
    if (strcmp(op,"^")==0||strcmp(op,"**")==0) { if (b<0) return 0; long r=1; for(long i=0;i<b;i++) r*=a; *out=r; return 1; }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int gz_arith_float_eval(const IR_t *nd, double *out) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_F) { *out = IR_LIT(nd).dval; return 1; }
    if (nd->op == IR_LIT_I) { *out = (double)IR_LIT(nd).ival; return 1; }
    if (nd->op == IR_ATOM && IR_LIT(nd).sval) { const char *a = IR_LIT(nd).sval; if (strcmp(a,"pi")==0) { *out = 3.141592653589793; return 1; } if (strcmp(a,"e")==0) { *out = 2.718281828459045; return 1; } if (strcmp(a,"inf")==0||strcmp(a,"infinity")==0) { *out = 1.0/0.0; return 1; } return 0; }
    if (nd->op != IR_ARITH || !IR_LIT(nd).sval) return 0;
    const char *op = IR_LIT(nd).sval;
    const IR_t *a0 = ir_pair_arg(nd, 0), *a1 = ir_pair_arg(nd, 1);
    if (!a0 && !a1) {
        if (strcmp(op,"pi")==0) { *out = 3.141592653589793; return 1; }
        if (strcmp(op,"e")==0)  { *out = 2.718281828459045; return 1; }
        if (strcmp(op,"inf")==0||strcmp(op,"infinity")==0) { *out = 1.0/0.0; return 1; }
        return 0;
    }
    if (!a0) return 0;
    double a = 0.0;
    if (!gz_arith_float_eval(a0, &a)) return 0;
    if (!a1) {
        if (strcmp(op,"sqrt")==0)  { *out = sqrt(a);  return 1; }
        if (strcmp(op,"sin")==0)   { *out = sin(a);   return 1; }
        if (strcmp(op,"cos")==0)   { *out = cos(a);   return 1; }
        if (strcmp(op,"tan")==0)   { *out = tan(a);   return 1; }
        if (strcmp(op,"asin")==0)  { *out = asin(a);  return 1; }
        if (strcmp(op,"acos")==0)  { *out = acos(a);  return 1; }
        if (strcmp(op,"atan")==0)  { *out = atan(a);  return 1; }
        if (strcmp(op,"exp")==0)   { *out = exp(a);   return 1; }
        if (strcmp(op,"log")==0)   { if (a<=0) return 0; *out = log(a); return 1; }
        if (strcmp(op,"float")==0) { *out = a; return 1; }
        if (strcmp(op,"float_integer_part")==0) { *out = (a>=0)?floor(a):ceil(a); return 1; }
        if (strcmp(op,"float_fractional_part")==0) { double ip; *out = modf(a, &ip); return 1; }
        if (strcmp(op,"abs")==0)     { *out = a<0?-a:a; return 1; }
        if (strcmp(op,"sign")==0)    { *out = a>0?1.0:a<0?-1.0:0.0; return 1; }
        return 0;
    }
    double b = 0.0;
    if (!gz_arith_float_eval(a1, &b)) return 0;
    if (strcmp(op,"+")==0)   { *out = a+b; return 1; }
    if (strcmp(op,"-")==0)   { *out = a-b; return 1; }
    if (strcmp(op,"*")==0)   { *out = a*b; return 1; }
    if (strcmp(op,"/")==0)   { if (!b) return 0; *out = a/b; return 1; }
    if (strcmp(op,"**")==0||strcmp(op,"^")==0) { *out = pow(a,b); return 1; }
    if (strcmp(op,"atan")==0) { *out = atan2(a,b); return 1; }
    if (strcmp(op,"max")==0)  { *out = a>b?a:b; return 1; }
    if (strcmp(op,"min")==0)  { *out = a<b?a:b; return 1; }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int gz_arith_var_plus_const(const IR_t *nd, int *var_slot, const char **op_out, long *c_out) {
    if (!nd) return 0;
    if (nd->op == IR_LOGICVAR) { *var_slot = (int)IR_LIT(nd).ival; *op_out = NULL; *c_out = 0; return 1; }
    const IR_t *p0 = ir_pair_arg(nd, 0), *p1 = ir_pair_arg(nd, 1);
    if (nd->op != IR_ARITH || !IR_LIT(nd).sval || !p0 || !p1) return 0;
    const char *op = IR_LIT(nd).sval;
    if (strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"mod")==0||strcmp(op,"rem")==0||
        strcmp(op,"/\\")==0||strcmp(op,"\\/")==0||strcmp(op,"xor")==0||strcmp(op,">>")==0||strcmp(op,"<<")==0) {
        if (p0->op == IR_LOGICVAR && p1->op == IR_LIT_I) {
            *var_slot = (int)IR_LIT(p0).ival; *op_out = op; *c_out = (long)IR_LIT(p1).ival; return 1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int gz_arith_var_bivar(const IR_t *nd, int *slot1, int *slot2, const char **op_out) {
    const IR_t *b0 = ir_pair_arg(nd, 0), *b1 = ir_pair_arg(nd, 1);
    if (!nd || nd->op != IR_ARITH || !IR_LIT(nd).sval || !b0 || !b1) return 0;
    const char *op = IR_LIT(nd).sval;
    if (strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"mod")==0||strcmp(op,"rem")==0||
        strcmp(op,"/\\")==0||strcmp(op,"\\/")==0||strcmp(op,"xor")==0||strcmp(op,">>")==0||strcmp(op,"<<")==0) {
        if (b0->op == IR_LOGICVAR && b1->op == IR_LOGICVAR) {
            *slot1 = (int)IR_LIT(b0).ival; *slot2 = (int)IR_LIT(b1).ival; *op_out = op; return 1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_prepare(IR_t *nd) {
    if (!PLATFORM_X86) return;
    g_emit.bb_ls = NULL;
    g_emit.bb_rs = NULL;
    g_emit.bb_op_lbl = NULL;
    g_emit.bb_lk = -1;
    if (nd->op == IR_DTP_ASSIGN) { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); return; }
    if (nd->op == IR_ASSIGN) {
        IR_t *oa = (nd->n_operands > 0) ? nd->operands[0] : NULL;
        g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : "");
        g_emit.bb_rs = oa ? bb_intern_into(g_emit.bb_rs_buf, IR_LIT(oa).sval) : NULL;
        return;
    }
    if (nd->op == IR_ASSIGN_CONCAT) {
        g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : "");
        if (g_emit.op_parts_n == 1 && g_emit.op_parts_tag[0] == 0)
            g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, g_emit.op_parts_str[0] ? g_emit.op_parts_str[0] : "");
        else if (g_emit.op_parts_n > 0) g_emit.op_off = bb_slot_claim(16 * g_emit.op_parts_n);
        return;
    }
    if (nd->op == IR_ASSIGN_LIT_I) { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); return; }
    if (nd->op == IR_ASSIGN_LIT_S) { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, g_emit.op_a_sval ? g_emit.op_a_sval : ""); return; }
    if (nd->op == IR_ASSIGN_VAR)   { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, g_emit.op_a_sval ? g_emit.op_a_sval : ""); return; }
    if (nd->op == IR_ASSIGN_CALL)  { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); return; }
    if (nd->op == IR_ASSIGN_DESCR) { g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, IR_LIT(nd).sval ? IR_LIT(nd).sval : ""); return; }
    if (nd->op == IR_ALT) {
        int n = 0;
        IR_t * const * arms = g_emit_cfg ? bb_operand_aux_get(g_emit_cfg, nd, &n) : ((IR_t * const *)0);
        int ok = (arms && n > 0 && n <= 5);
        for (int i = 0; ok && i < n; i++)
            if (!arms[i] || (arms[i]->op != IR_LIT_I && arms[i]->op != IR_LIT_S)) ok = 0;
        g_emit.op_parts_n = ok ? n : 0;
        for (int i = 0; ok && i < n; i++) {
            g_emit.op_parts_tag[i]  = (arms[i]->op == IR_LIT_I) ? (int)DT_I : (int)DT_S;
            g_emit.op_parts_ival[i] = (int64_t)IR_LIT(arms[i]).ival;
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
        g_emit.op_call_sym = NULL; g_emit.op_call_fp = NULL;
        {
            const char *bf = IR_LIT(nd).sval ? IR_LIT(nd).sval : "";
            if (!strcmp(bf,"atom_length") || !strcmp(bf,"string_length")) { g_emit.op_call_sym = "rt_atom_length"; g_emit.op_call_fp = (void*)rt_atom_length; }
            else if (!strcmp(bf,"upcase_atom") || !strcmp(bf,"string_upper")) { g_emit.op_call_sym = "rt_upcase_atom"; g_emit.op_call_fp = (void*)rt_upcase_atom; }
            else if (!strcmp(bf,"downcase_atom") || !strcmp(bf,"string_lower")) { g_emit.op_call_sym = "rt_downcase_atom"; g_emit.op_call_fp = (void*)rt_downcase_atom; }
            else if (!strcmp(bf,"copy_term")) { g_emit.op_call_sym = "rt_copy_term"; g_emit.op_call_fp = (void*)rt_copy_term; }
            else { g_emit.op_call_sym = "rt_atom_string_pair"; g_emit.op_call_fp = (void*)rt_atom_string_pair; }
        }
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
            g_emit.op_parts_ival[3] = (st && st->nargs > 0) ? ((st->args[0] && st->args[0]->op == IR_LOGICVAR) ? IR_LIT(st->args[0]).ival : -2) : -1;
            g_emit.op_parts_ival[4] = (st && st->nargs > 1) ? ((st->args[1] && st->args[1]->op == IR_LOGICVAR) ? IR_LIT(st->args[1]).ival : -2) : -1;
            g_emit.op_parts_ival[5] = (st && st->nargs > 2) ? ((st->args[2] && st->args[2]->op == IR_LOGICVAR) ? IR_LIT(st->args[2]).ival : -2) : -1;
        }
        if (nd->op == IR_CELL_ITE) {
            const pl_gz_ite_state_t * is = (const pl_gz_ite_state_t *)(intptr_t)IR_LIT(nd).ival;
            g_emit.op_parts_n = is ? 1 : 0;
            g_emit.op_parts_ival[0] = is ? (int64_t)is->gate_slot : -1;
        }
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
        return;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_seq(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    EMIT_PAIR_RESET();
    EMIT_PAIR_JMP(lbl_γ);
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_e binop_slot_kind(IR_t *nd) {
    int64_t op = nd ? IR_LIT(nd).ival : -1;
    if ((op >= BINOP_LT && op <= BINOP_NE) || (op >= BINOP_SLT && op <= BINOP_SNE)) return IR_BINOP_RELOP;
    if (op == BINOP_CONCAT)               return IR_BINOP_CONCAT;
    if (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD) return IR_BINOP_ARITH;
    return IR_BINOP;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int descr_binop_opnd_slot(IR_t *o) {
    return (o && o->op != IR_LIT_F && o->op != IR_LIT_NUL) ? bb_slot_get(o) : -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_binop_tree(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !bb_child0(pBB) || !bb_child1(pBB)) {
        fprintf(stderr, "[IBB] FATAL flat_drive_binop_tree: missing α or β child\n");
        abort();
    }
    if ((bb_child0(pBB)->op == IR_LIT_I && bb_child1(pBB)->op == IR_LIT_I)
        || (bb_child0(pBB)->op == IR_LIT_S && bb_child1(pBB)->op == IR_LIT_S && IR_LIT(pBB).ival == BINOP_CONCAT)) {
        EMIT_PAIR_RESET();
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        { IR_e _sk = pBB->op; pBB->op = binop_slot_kind(pBB); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); pBB->op = _sk; }
        return;
    }
    int id = g_flat_node_id++;
    bb_label_t *lhs_done = emit_label_alloc("xbinop%d_lhs_done", id);
    bb_label_t *rhs_done = emit_label_alloc("xbinop%d_rhs_done", id);
    bb_label_t *lhs_β    = emit_label_alloc("xbinop%d_lhs_b",    id);
    bb_label_t *rhs_β    = emit_label_alloc("xbinop%d_rhs_b",    id);
    if (bb_child0(pBB)->op == IR_ASSIGN && bb_child0(bb_child0(pBB)) && bb_child0(bb_child0(pBB))->op == IR_VAR) {
        bb_label_t *lhs_stored = emit_label_alloc("xbinop%d_lhs_st", id);
        walk_bb_flat(bb_child0(pBB), lhs_stored, lbl_ω, lhs_β);
        emit_label_define_bb(lhs_stored);
        walk_bb_flat(bb_child0(bb_child0(pBB)), lhs_done, lbl_ω, lhs_β);
    } else {
        walk_bb_flat(bb_child0(pBB), lhs_done, lbl_ω, lhs_β);
    }
    emit_label_define_bb(lhs_done);
    if (bb_child1(pBB)->op == IR_ASSIGN && bb_child0(bb_child1(pBB)) && bb_child0(bb_child1(pBB))->op == IR_VAR) {
        bb_label_t *rhs_stored = emit_label_alloc("xbinop%d_rhs_st", id);
        walk_bb_flat(bb_child1(pBB), rhs_stored, lbl_ω, rhs_β);
        emit_label_define_bb(rhs_stored);
        walk_bb_flat(bb_child0(bb_child1(pBB)), rhs_done, lbl_ω, rhs_β);
    } else {
        walk_bb_flat(bb_child1(pBB), rhs_done, lbl_ω, rhs_β);
    }
    emit_label_define_bb(rhs_done);
    if (g_descr_flat_chain) {
        g_emit.op_sa = descr_binop_opnd_slot(bb_child0(pBB));
        g_emit.op_sb = descr_binop_opnd_slot(bb_child1(pBB));
        if (g_emit.op_sa >= 0 && g_emit.op_sb >= 0) g_emit.op_off = bb_slot_alloc16(pBB);
        else g_emit.op_off = -1;
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    { IR_e _sk = pBB->op; pBB->op = binop_slot_kind(pBB); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); pBB->op = _sk; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int binop_operand_streams(IR_t *e) {
    if (!e) return 0;
    if (e->op == IR_ASSIGN) return binop_operand_streams(bb_child1(e));
    switch (e->op) {
        case IR_ALT: case IR_TO: case IR_TO_BY: case IR_BINOP_GEN: case IR_ITERATE:
        case IR_LIMIT: case IR_PROC_GEN: case IR_LIST_BANG: case IR_KEY_GEN:
        case IR_FIND_GEN: case IR_SEQ_GEN: case IR_SUSPEND: case IR_REPEAT:
            return 1;
        case IR_CALL_DEFINE:
        case IR_SCAN_POS: case IR_SCAN_ANY: case IR_SCAN_MATCH: case IR_SCAN_MANY: case IR_SCAN_TAB: case IR_SCAN_MOVE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL:
        case IR_CALL_PROC_STAGED: case IR_CALL_USERPROC: case IR_CALL_BYNAME: case IR_CALL_BUILTIN:
        case IR_CALL:
            if (IR_LIT(e).sval && rt_proc_is_registered(IR_LIT(e).sval)) return 0;
            return 1;
        default:
            return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_binop_gen_tree(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !bb_child0(pBB) || !bb_child1(pBB)) {
        fprintf(stderr, "[IBB] FATAL flat_drive_binop_gen_tree: missing α or β child\n");
        abort();
    }
    int id = g_flat_node_id++;
    /* lhs_seeded: LHS produced a value; restart RHS from its α.
       rhs_done:  RHS produced a value; compute binop → γ.
       lhs_β:     LHS retry entry (advance LHS counter).
       rhs_β:     RHS retry entry (advance RHS counter). */
    bb_label_t *lhs_seeded = emit_label_alloc("xbgen%d_lhs_seeded", id);
    bb_label_t *rhs_done   = emit_label_alloc("xbgen%d_rhs_done",   id);
    bb_label_t *lhs_β      = emit_label_alloc("xbgen%d_lhs_β",      id);
    bb_label_t *rhs_β      = emit_label_alloc("xbgen%d_rhs_β",      id);
    /* LHS generator: success→lhs_seeded, fail→lbl_ω (whole expr fails), retry→lhs_β */
    walk_bb_flat(bb_child0(pBB), lhs_seeded, lbl_ω, lhs_β);
    emit_label_define_bb(lhs_seeded);
    /* RHS generator: success→rhs_done, fail→lhs_β (try next LHS), retry→rhs_β */
    walk_bb_flat(bb_child1(pBB), rhs_done, lhs_β, rhs_β);
    /* rhs_done: both operands ready → compute binop → γ.
       Wire β of the binop box directly to rhs_β so the arith/relop template's
       x86("def","β") lands at rhs_β and x86("jmp","ω") = overall fail.
       After the box emit lbl_β → jmp rhs_β for the outer caller's retry. */
    emit_label_define_bb(rhs_done);
    if (g_descr_flat_chain) {
        g_emit.op_sa = descr_binop_opnd_slot(bb_child0(pBB));
        g_emit.op_sb = descr_binop_opnd_slot(bb_child1(pBB));
        g_emit.op_off = (g_emit.op_sa >= 0 && g_emit.op_sb >= 0) ? bb_slot_alloc16(pBB) : -1;
    }
    EMIT_PAIR_RESET();
    { IR_e _sk = pBB->op; pBB->op = binop_slot_kind(pBB); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, rhs_β); pBB->op = _sk; }
    emit_label_define_bb(lbl_β);
    emit_jmp_label(rhs_β, JMP_JMP);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_call_intexpr(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *a0 = ir_call_arg(pBB, 0);
    if (!pBB || !a0) {
        fprintf(stderr, "[IBB] FATAL flat_drive_call_intexpr: missing arg0\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *arg_done = emit_label_alloc("xcall%d_arg_done", id);
    bb_label_t *arg_β    = emit_label_alloc("xcall%d_arg_β",    id);
    int need_walk = (IR_LIT(pBB).dval != 1.0) || g_descr_flat_chain;
    if (need_walk) {
        walk_bb_flat(a0, arg_done, lbl_ω, arg_β);
        emit_label_define_bb(arg_done);
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, need_walk ? arg_β : lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_unop(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !bb_child0(pBB)) {
        fprintf(stderr, "[IBB] FATAL flat_drive_unop: missing operand (α)\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *arg_done = emit_label_alloc("xunop%d_arg_done", id);
    bb_label_t *arg_β    = emit_label_alloc("xunop%d_arg_β",    id);
    walk_bb_flat(bb_child0(pBB), arg_done, lbl_ω, arg_β);
    emit_label_define_bb(arg_done);
    emit_jmp_label(lbl_γ, JMP_JMP);
    emit_label_define_bb(lbl_β);
    emit_jmp_label(lbl_ω, JMP_JMP);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_list_bang(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !bb_child0(pBB)) {
        fprintf(stderr, "[IBB] FATAL flat_drive_list_bang: missing iterable (α)\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *iter_done = emit_label_alloc("xbang%d_iter_done", id);
    bb_label_t *iter_β    = emit_label_alloc("xbang%d_iter_β",    id);
    walk_bb_flat(bb_child0(pBB), iter_done, lbl_ω, iter_β);
    emit_label_define_bb(iter_done);
    g_emit.op_sa  = bb_slot_get(bb_child0(pBB));
    g_emit.op_sb  = bb_slot_claim(8);
    g_emit.op_off = bb_slot_alloc16(pBB);
    FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_to(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !bb_child0(pBB) || !bb_child1(pBB)) {
        fprintf(stderr, "[IBB] FATAL flat_drive_to: missing from (α) or to (β) operand\n");
        abort();
    }
    g_emit.op_sa  = bb_slot_get(bb_child0(pBB));
    g_emit.op_sb  = bb_slot_get(bb_child1(pBB));
    /* Use bb_slot_alloc16_or_get so that if this TO node was already walked in the
       chain pre-pass (before a parent BINOP_GEN visit), the second walk reuses the
       same slot.  Both walks then operate on the same frame location. */
    int already = (bb_slot_get(pBB) >= 0);
    g_emit.op_off = bb_slot_alloc16_or_get(pBB);
    if (!already) (void)bb_slot_claim(8);  /* claim the counter byte only once */
    FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_alt_gen(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB) {
        fprintf(stderr, "[IBB] FATAL flat_drive_alt_gen: null node\n");
        abort();
    }
    g_emit.node   = pBB;
    g_emit.op_off = bb_slot_alloc16(pBB);
    (void)bb_slot_claim(8);
    FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_field_get(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *obj_box = (pBB && pBB->n_operands > 0) ? pBB->operands[0] : NULL;
    if (!obj_box || !IR_LIT(pBB).sval) {
        fprintf(stderr, "[IBB] FATAL flat_drive_field_get: IR_FIELD_GET needs operands[0] (object) and sval (field)\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *obj_done = emit_label_alloc("xfget%d_obj_done", id);
    bb_label_t *obj_β    = emit_label_alloc("xfget%d_obj_β",    id);
    walk_bb_flat(obj_box, obj_done, lbl_ω, obj_β);
    emit_label_define_bb(obj_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_field_set(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *obj_box = (pBB && pBB->n_operands > 0) ? pBB->operands[0] : NULL;
    IR_t *rhs_box = (pBB && pBB->n_operands > 1) ? pBB->operands[1] : NULL;
    if (!obj_box || !rhs_box || !IR_LIT(pBB).sval) {
        fprintf(stderr, "[IBB] FATAL flat_drive_field_set: IR_FIELD_SET needs operands[0] (object), operands[1] (rhs), sval (field)\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *rhs_done = emit_label_alloc("xfset%d_rhs_done", id);
    bb_label_t *rhs_β    = emit_label_alloc("xfset%d_rhs_β",    id);
    bb_label_t *obj_done = emit_label_alloc("xfset%d_obj_done", id);
    bb_label_t *obj_β    = emit_label_alloc("xfset%d_obj_β",    id);
    walk_bb_flat(rhs_box, rhs_done, lbl_ω, rhs_β);
    emit_label_define_bb(rhs_done);
    walk_bb_flat(obj_box, obj_done, lbl_ω, obj_β);
    emit_label_define_bb(obj_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_idx_get(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (pBB && ((IR_t*)0) && ((IR_t*)0)) {
        int id = g_flat_node_id++;
        bb_label_t *base_done = emit_label_alloc("xidx%d_base_done", id);
        bb_label_t *base_β    = emit_label_alloc("xidx%d_base_b",    id);
        bb_label_t *idx_done  = emit_label_alloc("xidx%d_idx_done",  id);
        bb_label_t *idx_β     = emit_label_alloc("xidx%d_idx_b",     id);
        walk_bb_flat(((IR_t*)0), base_done, lbl_ω, base_β);
        emit_label_define_bb(base_done);
        walk_bb_flat(((IR_t*)0), idx_done, lbl_ω, idx_β);
        emit_label_define_bb(idx_done);
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_idx_set(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (pBB && pBB->n_operands >= 2) {
        int id = g_flat_node_id++;
        IR_t *base_box = pBB->operands[0];
        IR_t *idx_box  = pBB->operands[1];
        IR_t *rhs_box  = pBB->n_operands > 2 ? pBB->operands[2] : NULL;
        bb_label_t *base_done = emit_label_alloc("xidxs%d_base_done", id);
        bb_label_t *base_β    = emit_label_alloc("xidxs%d_base_b",    id);
        bb_label_t *idx_done  = emit_label_alloc("xidxs%d_idx_done",  id);
        bb_label_t *idx_β     = emit_label_alloc("xidxs%d_idx_b",     id);
        walk_bb_flat(base_box, base_done, lbl_ω, base_β);
        emit_label_define_bb(base_done);
        walk_bb_flat(idx_box, idx_done, lbl_ω, idx_β);
        emit_label_define_bb(idx_done);
        if (rhs_box) {
            bb_label_t *rhs_done = emit_label_alloc("xidxs%d_rhs_done", id);
            bb_label_t *rhs_β    = emit_label_alloc("xidxs%d_rhs_b",    id);
            walk_bb_flat(rhs_box, rhs_done, lbl_ω, rhs_β);
            emit_label_define_bb(rhs_done);
        }
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_initial(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || pBB->n_operands < 1 || !pBB->operands[0]) {
        emit_label_define_bb(lbl_β);
        emit_jmp_label(lbl_γ, JMP_JMP);
        emit_jmp_label(lbl_ω, JMP_JMP);
        return;
    }
    int id = g_flat_node_id++;
    bb_label_t *body_entry = emit_label_alloc("xinit%d_body", id);
    EMIT_PAIR_RESET();
    EMIT_PAIR_JMP(lbl_γ);
    EMIT_PAIR_JMP(body_entry);
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
    emit_label_define_bb(body_entry);
    walk_bb_flat(pBB->operands[0], lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_case(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || pBB->n_operands < 1 || !pBB->operands[0]) {
        emit_label_define_bb(lbl_β);
        emit_jmp_label(lbl_γ, JMP_JMP);
        emit_jmp_label(lbl_ω, JMP_JMP);
        return;
    }
    IR_t *chain[256];
    int nchain = 0;
    for (IR_t *c = pBB->operands[0]->γ.node; c && nchain < 256; c = c->γ.node) chain[nchain++] = c;
    int npair      = nchain / 2;
    int has_default = (nchain & 1) != 0;
    int id = g_flat_node_id++;
    bb_label_t *sel_done = emit_label_alloc("xcase%d_sel_done", id);
    bb_label_t *sel_β    = emit_label_alloc("xcase%d_sel_β",    id);
    bb_label_t **val_entry = (bb_label_t **)alloca((size_t)(npair > 0 ? npair : 1) * sizeof(bb_label_t *));
    for (int i = 0; i < npair; i++) val_entry[i] = emit_label_alloc("xcase%d_val%d", id, i);
    walk_bb_flat(pBB->operands[0], sel_done, lbl_ω, sel_β);
    emit_label_define_bb(sel_done);
    for (int i = 0; i < npair; i++) {
        bb_label_t *key_done = emit_label_alloc("xcase%d_key%d_done", id, i);
        bb_label_t *key_β    = emit_label_alloc("xcase%d_key%d_β",    id, i);
        walk_bb_flat(chain[2*i], key_done, lbl_ω, key_β);
        emit_label_define_bb(key_done);
        EMIT_PAIR_RESET();
        EMIT_PAIR_JMP(val_entry[i]);
    }
    if (has_default) {
        bb_label_t *def_β = emit_label_alloc("xcase%d_def_β", id);
        walk_bb_flat(chain[nchain-1], lbl_γ, lbl_ω, def_β);
    } else {
        emit_jmp_label(lbl_ω, JMP_JMP);
    }
    for (int i = 0; i < npair; i++) {
        bb_label_t *val_β = emit_label_alloc("xcase%d_val%d_β", id, i);
        emit_label_define_bb(val_entry[i]);
        walk_bb_flat(chain[2*i+1], lbl_γ, lbl_ω, val_β);
    }
    emit_label_define_bb(lbl_β);
    emit_jmp_label(lbl_ω, JMP_JMP);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_limit(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || pBB->n_operands < 2 || !pBB->operands[0] || !pBB->operands[1]) {
        fprintf(stderr, "[IBB] FATAL flat_drive_limit: IR_LIMIT requires α (generator) and β (count expr)\n");
        abort();
    }
    {
        IR_t *g = pBB->operands[0];
        while (g && g->op == IR_ASSIGN) g = bb_child1(g);
        int ok = g && (g->op == IR_TO || g->op == IR_TO_BY || g->op == IR_ALT ||
                       g->op == IR_BINOP_GEN || g->op == IR_LIST_BANG);
        if (!ok) {
            fprintf(stderr, "[IBB] FATAL flat_drive_limit: generator kind=%d has no mode-3 two-port emission (limit deferred)\n",
                    (int)(g ? g->op : -1));
            abort();
        }
    }
    int id = g_flat_node_id++;
    bb_label_t *count_done = emit_label_alloc("xlimit%d_count_done", id);
    bb_label_t *count_β    = emit_label_alloc("xlimit%d_count_b",    id);
    bb_label_t *got_value  = emit_label_alloc("xlimit%d_got",        id);
    bb_label_t *gen_resume = emit_label_alloc("xlimit%d_gen_resume", id);
    walk_bb_flat(pBB->operands[1], count_done, lbl_ω, count_β);
    emit_label_define_bb(count_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_JMP(lbl_ω);
    walk_bb_flat(pBB->operands[0], got_value, lbl_ω, gen_resume);
    emit_label_define_bb(got_value);
    EMIT_PAIR_RESET();
    EMIT_PAIR_JMP(lbl_γ);
    emit_label_define_bb(lbl_β);
    EMIT_PAIR_RESET();
    EMIT_PAIR_JMP(lbl_ω);
    EMIT_PAIR_JMP(gen_resume);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_return(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    (void)lbl_γ; (void)lbl_β;
    bb_label_t *slab_succ = g_emit.flat_succ_p ? g_emit.flat_succ_p : lbl_γ;
    bb_label_t *slab_fail = g_emit.flat_fail_p ? g_emit.flat_fail_p : lbl_ω;
    if (pBB && pBB->n_operands > 0 && pBB->operands[0]) {
        int id = g_flat_node_id++;
        bb_label_t *expr_done = emit_label_alloc("xreturn%d_expr_done", id);
        bb_label_t *expr_β    = emit_label_alloc("xreturn%d_expr_β",    id);
        walk_bb_flat(pBB->operands[0], expr_done, lbl_ω, expr_β);
        emit_label_define_bb(expr_done);
    }
    emit_jmp_label((pBB && IR_LIT(pBB).dval == 2.0) ? slab_fail : slab_succ, JMP_JMP);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_call_userproc(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int nargs = (int)(pBB ? IR_LIT(pBB).ival : 0);
    bb_label_t *prev_done = NULL;
    for (int j = 0; j < nargs; j++) {
        IR_t *ax = ir_call_arg(pBB, j);
        if (!ax) break;
        int id = g_flat_node_id++;
        bb_label_t *arg_done = emit_label_alloc("xupcall%d_arg_done", id);
        bb_label_t *arg_β    = emit_label_alloc("xupcall%d_arg_β",    id);
        if (prev_done) emit_label_define_bb(prev_done);
        walk_bb_flat(ax, arg_done, lbl_ω, arg_β);
        prev_done = arg_done;
    }
    if (prev_done) emit_label_define_bb(prev_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int arg_entry_terminal(IR_t *ae) {
    return (ae && (!ae->γ.node || ae->γ.node->op == IR_SUCCEED)) ? 1 : 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int ir_is_generator_kind(IR_e t);
/*--------------------------------------------------------------------------------------------------------------------*/
static int subchain_node_is_generator(IR_t *nd) {
    if (!nd) return 0;
    if (ir_is_generator_kind(nd->op)) return 1;
    if (nd->op == IR_SCAN_UPTO || nd->op == IR_SCAN_FIND || nd->op == IR_SCAN_BAL) return 1;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_emit_arg_subchain(IR_t *entry, bb_label_t *succ, bb_label_t *fail) {
    enum { CH_MAX = 512 };
    IR_t *nodes[CH_MAX]; int n = 0;
    IR_t *queue[CH_MAX]; int qh = 0, qt = 0;
    queue[qt++] = entry;
    while (qh < qt) {
        IR_t *c = queue[qh++];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= CH_MAX) { fprintf(stderr, "[GZ-10] FATAL arg subchain exceeds CH_MAX\n"); abort(); }
        nodes[n++] = c;
        if (c->γ.node && qt < CH_MAX) queue[qt++] = c->γ.node;
        if ((c->op == IR_BINOP || c->op == IR_BINOP_GEN) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        { extern int g_scan_regs_live; if (g_scan_regs_live && (c->op == IR_CALL || ir_is_call_kind(c->op) || ir_is_scan_kind(c->op)) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node; }
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
        for (int k = 0; k < n; k++) if (nodes[k] == nodes[i]->γ.node) { node_γ = (i > k && subchain_node_is_generator(nodes[k])) ? betas[k] : lbls[k]; break; }
        if (nodes[i]->γ.node == NULL || nodes[i]->γ.node->op == IR_SUCCEED) node_γ = succ;
        int omega_resolved = 0;
        for (int k = 0; k < n; k++) if (nodes[k] == nodes[i]->ω.node) { node_ω = lbls[k]; omega_resolved = 1; break; }
        if (!omega_resolved) node_ω = fail;
        walk_bb_flat(nodes[i], node_γ, node_ω, betas[i]);
    }
}
static void gvar_drive_call_arg_slots(IR_t *nd, bb_label_t *lbl_ω) {
    g_emit.op_arg_slot_n = 0;
    int nargs = (int)(nd ? IR_LIT(nd).ival : 0);
    IR_graph_t **subs = nd ? (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter : NULL;
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
        g_gvar_callarg_live = 1;
        if (arg_entry_terminal(res[i])) {
            walk_bb_flat(res[i], arg_done, lbl_ω, arg_β);
            slots[i] = bb_slot_get(res[i]);
        } else if (relop_diamond) {
            /* relop diamond: marshal_call_arg boolean-relop arm emits INTVAL(0/1) into one slot; pre-computing the true-arm slot would read 1 even when the relop is false */
        } else if (res_last[i] && res_last[i]->op == IR_BINOP && (IR_LIT(res_last[i]).ival == BINOP_ADD || IR_LIT(res_last[i]).ival == BINOP_SUB || IR_LIT(res_last[i]).ival == BINOP_MUL || IR_LIT(res_last[i]).ival == BINOP_DIV || IR_LIT(res_last[i]).ival == BINOP_MOD)) {
            /* arith BINOP chain: marshal_call_arg inline-arith handles correctly (stores DT_I tag); pre-computation would only store 8-byte raw int missing the tag */
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
static void flat_drive_userproc(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int nargs = (int)(pBB ? IR_LIT(pBB).ival : 0);
    IR_graph_t **blks = pBB ? (IR_graph_t **)(intptr_t) IR_EXEC(pBB).counter : NULL;
    bb_label_t *prev_done = NULL;
    for (int i = 0; i < nargs && blks; i++) {
        IR_t *aentry = blks[i] ? blks[i]->entry : NULL;
        if (!aentry) continue;
        int id = g_flat_node_id++;
        bb_label_t *arg_done = emit_label_alloc("xicnarg%d_done", id);
        if (prev_done) emit_label_define_bb(prev_done);
        descr_chain_operand_refs(aentry);
        flat_emit_arg_subchain(aentry, arg_done, lbl_ω);
        prev_done = arg_done;
    }
    if (prev_done) emit_label_define_bb(prev_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t *descr_chain_terminal(IR_t *entry) {
    int guard = 0;
    while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL) && entry->γ.node && guard++ < 512) entry = entry->γ.node;
    IR_t *last = entry;
    guard = 0;
    while (last && last->γ.node && last->γ.node->op != IR_SUCCEED && last->γ.node->op != IR_FAIL && guard++ < 512) last = last->γ.node;
    return last;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_scan_glue(IR_t *pBB, int phase, int subj_slot, int regs_off, bb_label_t *γ, bb_label_t *ω, bb_label_t *β) {
    g_emit.op_sb = phase; g_emit.op_sa = subj_slot; g_emit.op_off = regs_off;
    g_emit.lbl_γ = γ->name; g_emit.lbl_ω = ω->name; g_emit.lbl_β = β->name;
    g_emit.lbl_γ_p = γ; g_emit.lbl_ω_p = ω; g_emit.lbl_β_p = β;
    walk_bb_node(pBB, emit_outf());
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_gen_scan(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_graph_t *subj_sg = (pBB && IR_LIT(pBB).dval == 1.0) ? (IR_graph_t *)(intptr_t) IR_EXEC(pBB).counter : NULL;
    IR_graph_t *body_sg = (pBB && IR_LIT(pBB).dval == 1.0) ? (IR_graph_t *)(intptr_t) IR_LIT(pBB).ival    : NULL;
    if (!subj_sg || !subj_sg->entry || !body_sg || !body_sg->entry) {
        emit_label_define_bb(lbl_β); emit_jmp_label(lbl_ω, JMP_JMP); emit_jmp_label(lbl_ω, JMP_JMP); return;
    }
    int id = g_flat_node_id++;
    bb_label_t *subj_done  = emit_label_alloc("xscan%d_subj_done",  id);
    bb_label_t *body_start = emit_label_alloc("xscan%d_body_start", id);
    bb_label_t *body_done  = emit_label_alloc("xscan%d_body_done",  id);
    bb_label_t *body_fail  = emit_label_alloc("xscan%d_body_fail",  id);
    bb_label_t *enter_β    = emit_label_alloc("xscan%d_enter_β",    id);
    bb_label_t *leaveok_β  = emit_label_alloc("xscan%d_leaveok_β",  id);
    bb_label_t *leavef_β   = emit_label_alloc("xscan%d_leavef_β",   id);
    descr_chain_operand_refs(subj_sg->entry);
    IR_t *subj_term = descr_chain_terminal(subj_sg->entry);
    flat_emit_arg_subchain(subj_sg->entry, subj_done, lbl_ω);
    int subj_slot = bb_slot_get(subj_term);
    int regs_off = bb_slot_claim(24);
    emit_label_define_bb(subj_done);
    flat_drive_scan_glue(pBB, 1, subj_slot, regs_off, body_start, lbl_ω, enter_β);
    emit_label_define_bb(body_start);
    descr_chain_operand_refs(body_sg->entry);
    int saved_scan_regs_live = g_scan_regs_live;
    g_scan_regs_live = 1;
    flat_emit_arg_subchain(body_sg->entry, body_done, body_fail);
    g_scan_regs_live = saved_scan_regs_live;
    IR_t *body_term = descr_chain_terminal(body_sg->entry);
    int body_slot = body_term ? bb_slot_get(body_term) : -1;
    if (body_slot >= 0 && bb_slot_get(pBB) < 0 && g_bb_slotmap_n < BB_SLOTMAP_MAX) { g_bb_slotmap[g_bb_slotmap_n].key = pBB; g_bb_slotmap[g_bb_slotmap_n].off = body_slot; g_bb_slotmap_n++; }
    emit_label_define_bb(body_done);
    flat_drive_scan_glue(pBB, 2, -1, regs_off, lbl_γ, lbl_ω, leaveok_β);
    emit_label_define_bb(body_fail);
    flat_drive_scan_glue(pBB, 2, -1, regs_off, lbl_ω, lbl_ω, leavef_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int gen_bb_is_gen_arg(IR_t *e) {
    if (!e) return 0;
    if (e->op == IR_ASSIGN) return gen_bb_is_gen_arg(bb_child1(e));
    switch (e->op) {
        case IR_TO: case IR_TO_BY: case IR_UPTO: case IR_ALT:
        case IR_BINOP_GEN: case IR_ITERATE: case IR_LIMIT: case IR_PROC_GEN:
        case IR_LIST_BANG: case IR_KEY_GEN: case IR_FIND_GEN: case IR_SEQ_GEN:
        case IR_GATHER: case IR_MAP: case IR_GREP:
            return 1;
        default: return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int ir_is_generator_kind(IR_e t) {
    switch (t) {
        case IR_TO: case IR_TO_BY: case IR_UPTO: case IR_ALT:
        case IR_BINOP_GEN: case IR_ITERATE: case IR_LIMIT: case IR_PROC_GEN:
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
static void flat_drive_call_builtin(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int nargs = (int)(pBB ? IR_LIT(pBB).ival : 0);
    bb_label_t *prev_done = NULL;
    for (int j = 0; j < nargs; j++) {
        IR_t *ax = ir_call_arg(pBB, j);
        if (!ax) break;
        int id = g_flat_node_id++;
        bb_label_t *arg_done = emit_label_alloc("xbicall%d_arg_done", id);
        bb_label_t *arg_β    = emit_label_alloc("xbicall%d_arg_β",    id);
        if (prev_done) emit_label_define_bb(prev_done);
        walk_bb_flat(ax, arg_done, lbl_ω, arg_β);
        prev_done = arg_done;
    }
    if (prev_done) emit_label_define_bb(prev_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static char g_seq_int_bufs[16][24];
static int gvar_seq_flatten(IR_graph_t *g, int *n) {
    if (!g || !g->entry || *n >= 16) return 0;
    IR_t *e = g->entry;
    if (e->op == IR_LIT_S) { g_emit.op_parts_tag[*n] = 0; g_emit.op_parts_str[(*n)++] = IR_LIT(e).sval ? IR_LIT(e).sval : ""; return 1; }
    if (e->op == IR_LIT_I) { int _s = (*n)++; snprintf(g_seq_int_bufs[_s], 24, "%lld", (long long)IR_LIT(e).ival); g_emit.op_parts_tag[_s] = 0; g_emit.op_parts_str[_s] = g_seq_int_bufs[_s]; return 1; }
    if (e->op == IR_VAR)   { g_emit.op_parts_tag[*n] = 1; g_emit.op_parts_str[(*n)++] = IR_LIT(e).sval ? IR_LIT(e).sval : ""; return 1; }
    if (e->op == IR_SEQ)   {
        IR_graph_t *l = (IR_graph_t *)(intptr_t)IR_EXEC(e).counter;
        IR_graph_t *r = (IR_graph_t *)(intptr_t)IR_LIT(e).ival;
        return gvar_seq_flatten(l, n) && gvar_seq_flatten(r, n);
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_gvar_assign(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    g_emit.op_parts_n = 0;
    IR_t *c0 = bb_child0(pBB);
    if (c0 && c0->op == IR_SEQ) {
        int n = 0;
        IR_graph_t *l = (IR_graph_t *)(intptr_t)IR_EXEC(c0).counter;
        IR_graph_t *r = (IR_graph_t *)(intptr_t)IR_LIT(c0).ival;
        if (gvar_seq_flatten(l, &n) && gvar_seq_flatten(r, &n)) g_emit.op_parts_n = n;
    } else if (c0 && c0->op == IR_LIT_S) {
        g_emit.op_parts_n = 1; g_emit.op_parts_tag[0] = 0; g_emit.op_parts_str[0] = IR_LIT(c0).sval ? IR_LIT(c0).sval : "";
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_gvar_seq_passthrough(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    (void)pBB;
    EMIT_PAIR_RESET();
    EMIT_PAIR_JMP(lbl_γ);
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_gvar_assign_binop(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *c0 = bb_child0(pBB);
    if (!pBB || !c0 || (c0->op != IR_BINOP && c0->op != IR_UNOP)) {
        fprintf(stderr, "[SBB] FATAL flat_drive_gvar_assign_binop: rhs is not IR_BINOP or IR_UNOP\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *rhs_done = emit_label_alloc("xsasg%d_rhs_done", id);
    bb_label_t *rhs_β    = emit_label_alloc("xsasg%d_rhs_β",    id);
    if (c0->op == IR_BINOP && IR_LIT(c0).ival == BINOP_POW && bb_child0(c0) && bb_child1(c0) && bb_child0(c0)->op == IR_LIT_I && bb_child1(c0)->op == IR_LIT_I && IR_LIT(pBB).sval) {
        g_emit.op_sa   = (int)IR_LIT(bb_child0(c0)).ival;
        g_emit.op_sb   = (int)IR_LIT(bb_child1(c0)).ival;
        g_emit.op_kind = "POW";
        g_emit.op_name1 = (const char *)0;
        g_emit.op_name2 = (const char *)0;
        g_emit.op_off  = bb_slot_alloc(c0);
        EMIT_PAIR_RESET();
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        { IR_e _sk = pBB->op; pBB->op = IR_BINOP_GVAR_ARITH; EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); pBB->op = _sk; }
        return;
    }
    if (c0->op == IR_UNOP && bb_child0(c0) && bb_child0(c0)->op == IR_LIT_I) {
        /* Constant-fold UNOP(NEG|POS, LIT_I): spoof c0 as IR_LIT_I with computed value */
        int64_t val = IR_LIT(bb_child0(c0)).ival;
        if ((int)IR_LIT(c0).ival == (int)TT_MNS) val = -val;
        IR_e save_op = c0->op; c0->op = IR_LIT_I;
        int64_t save_ival = IR_LIT(c0).ival; IR_LIT(c0).ival = val;
        EMIT_PAIR_RESET();
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        c0->op = save_op; IR_LIT(c0).ival = save_ival;
        return;
    }
    if (bb_slot_get(c0) >= 0) {
        EMIT_PAIR_RESET();
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    walk_bb_flat(c0, rhs_done, lbl_ω, rhs_β);
    emit_label_define_bb(rhs_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int scan_pat_is_single_lit(IR_graph_t *pg) {
    if (!pg || !pg->entry || pg->entry->op != IR_PAT_LIT) return 0;
    int nlit = 0;
    for (int i = 0; i < pg->n; i++) {
        IR_e t = pg->all[i]->op;
        if (t == IR_SUCCEED || t == IR_FAIL) continue;
        if (t == IR_PAT_LIT) { nlit++; continue; }
        return 0;
    }
    return nlit == 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char * scan_pat_cat_concat(IR_graph_t *pg) {
    if (!pg || !pg->entry || pg->entry->op != IR_PAT_LIT) return NULL;
    int nlit = 0, ncat = 0;
    for (int i = 0; i < pg->n; i++) {
        IR_e t = pg->all[i]->op;
        if (t == IR_SUCCEED || t == IR_FAIL) continue;
        if (t == IR_PAT_LIT) { nlit++; continue; }
        if (t == IR_PAT_CAT) { ncat++; continue; }
        return NULL;
    }
    if (nlit < 2 || ncat < 1) return NULL;
    size_t total = 0;
    for (IR_t *c = pg->entry; c && c->op == IR_PAT_LIT; c = c->γ.node) total += IR_LIT(c).sval ? strlen(IR_LIT(c).sval) : 0;
    char *buf = (char *)GC_MALLOC_ATOMIC(total + 1);
    size_t off = 0;
    for (IR_t *c = pg->entry; c && c->op == IR_PAT_LIT; c = c->γ.node) { const char *s = IR_LIT(c).sval ? IR_LIT(c).sval : ""; size_t n = strlen(s); memcpy(buf + off, s, n); off += n; }
    buf[off] = 0;
    return buf;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int scan_val_is_single_lit(IR_graph_t *g) {
    if (!g || !g->entry || g->entry->op != IR_LIT_S) return 0;
    int nlit = 0;
    for (int i = 0; i < g->n; i++) {
        IR_e t = g->all[i]->op;
        if (t == IR_SUCCEED || t == IR_FAIL) continue;
        if (t == IR_LIT_S) { nlit++; continue; }
        return 0;
    }
    return nlit == 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_subject(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
static void flat_drive_match(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
/*--------------------------------------------------------------------------------------------------------------------*/
static int scan_pat_m3_native_safe(IR_graph_t *pg) {
    if (!pg || !pg->all) return 1;
    for (int i = 0; i < pg->n; i++) {
        IR_t *nd = pg->all[i];
        if (!nd) continue;
        if (nd->op == IR_PAT_ARBNO || (nd->op == IR_PAT_FENCE && IR_LIT(nd).ival != 1) || nd->op == IR_PAT_DEFER
            || nd->op == IR_REF_INVARIANT || nd->op == IR_PATTERN_DEFER) return 0;
        if ((nd->op == IR_PAT_POS || nd->op == IR_PAT_LEN || nd->op == IR_PAT_TAB || nd->op == IR_PAT_RTAB)
            && IR_LIT(nd).dval != 0.0) return 0;
        if ((nd->op == IR_PAT_ANY || nd->op == IR_PAT_NOTANY || nd->op == IR_PAT_BREAK || nd->op == IR_PAT_BREAKX)
            && IR_LIT(nd).dval != 0.0) return 0;
        if (nd->op == IR_PAT_SPAN && IR_LIT(nd).ival == 1) return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int flat_drive_scan_native(IR_t *pBB, IR_graph_t *pg, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pg || !pg->entry) return 0;
    IR_t *subj  = IR_node_alloc(pg, IR_SUBJECT);
    IR_t *match = IR_node_alloc(pg, IR_PAT_MATCH);
    if (!subj || !match) return 0;
    IR_LIT(subj).sval = IR_LIT(pBB).sval;
    if (!ir_operand_push(match, pg->entry)) return 0;
    IR_graph_t *save_cfg = g_emit_cfg;
    int save_subject_slot = g_subject_slot;
    bb_label_t *save_seal_lbl = g_scan_seal_lbl;
    g_emit_cfg = pg;
    int id = g_flat_node_id++;
    bb_label_t *subj_γ = emit_label_alloc("xscan%d_sγ", id);
    bb_label_t *subj_β = emit_label_alloc("xscan%d_sβ", id);
    bb_label_t *dcap_ok   = emit_label_alloc("xscan%d_dok",   id);
    bb_label_t *dcap_fail = emit_label_alloc("xscan%d_dfail", id);
    g_scan_seal_lbl = dcap_fail;
    flat_drive_subject(subj, subj_γ, lbl_ω, subj_β);
    emit_label_define_bb(subj_γ);
    emit_aligned_call_rt("rt_dcap_begin", (void *)rt_dcap_begin);
    flat_drive_match(match, dcap_ok, dcap_fail, lbl_β);
    emit_label_define_bb(dcap_ok);
    emit_aligned_call_rt("rt_dcap_end_ok", (void *)rt_dcap_end_ok);
    emit_jmp_label(lbl_γ, JMP_JMP);
    emit_label_define_bb(dcap_fail);
    emit_aligned_call_rt("rt_dcap_end_fail", (void *)rt_dcap_end_fail);
    emit_jmp_label(lbl_ω, JMP_JMP);
    g_emit_cfg = save_cfg;
    g_subject_slot = save_subject_slot;
    g_scan_seal_lbl = save_seal_lbl;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_scan_stmt(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    g_emit.op_scan_pat  = pBB ? IR_EXEC(pBB).counter : 0;
    g_emit.op_scan_subj = (pBB && pBB->n_operands > 0) ? (int64_t)(intptr_t)pBB->operands[0] : 0;
    g_emit.op_scan_repl = (pBB && pBB->n_operands > 1) ? (int64_t)(intptr_t)pBB->operands[1] : 0;
    g_emit.op_scan_pat_lit = NULL; g_emit.op_scan_subj_lit = NULL; g_emit.op_scan_replace_lit = NULL;
    {
        IR_graph_t * pg = (IR_graph_t *)(intptr_t)g_emit.op_scan_pat;
        IR_graph_t * sg = (IR_graph_t *)(intptr_t)g_emit.op_scan_subj;
        IR_graph_t * rg = (IR_graph_t *)(intptr_t)g_emit.op_scan_repl;
        if (scan_pat_is_single_lit(pg))                    g_emit.op_scan_pat_lit  = IR_LIT(pg->entry).sval ? IR_LIT(pg->entry).sval : "";
        else { const char * cc = scan_pat_cat_concat(pg); if (cc)  g_emit.op_scan_pat_lit  = cc; }
        if (scan_val_is_single_lit(sg))                    g_emit.op_scan_subj_lit = IR_LIT(sg->entry).sval ? IR_LIT(sg->entry).sval : "";
        if (scan_val_is_single_lit(rg))                    g_emit.op_scan_replace_lit = IR_LIT(rg->entry).sval ? IR_LIT(rg->entry).sval : "";
        if (!g_emit.op_scan_pat_lit && pBB && IR_LIT(pBB).sval && IR_LIT(pBB).sval[0] && !IR_LIT(pBB).ival
            && (g_is_text || scan_pat_m3_native_safe(pg))) {
            if (flat_drive_scan_native(pBB, pg, lbl_γ, lbl_ω, lbl_β)) return;
        }
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_subject(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int sa = bb_slot_alloc16(pBB);
    g_emit.op_sa  = sa;
    g_subject_slot = sa;
    FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_ref_invariant(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *ch = (pBB && pBB->n_operands > 0) ? pBB->operands[0] : NULL;
    bb_box_fn cfn = ch ? child_cache_get(ch) : NULL;
    g_emit.child_fn    = (void *)cfn;
    g_emit.bb_child_fn = (void *)cfn;
    g_emit.bb_child_lbl = cfn ? child_cache_get_lbl(cfn) : NULL;
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int is_pat_chain_elem(IR_e t) {
    switch (t) {
    case IR_PAT_LIT: case IR_PAT_ANY: case IR_PAT_NOTANY: case IR_PAT_SPAN: case IR_PAT_SPAN_VAR: case IR_PAT_BREAK: case IR_PAT_BREAKX:
    case IR_PAT_LEN: case IR_PAT_POS: case IR_PAT_TAB: case IR_PAT_RTAB: case IR_PAT_ATP: case IR_PAT_REM:
    case IR_PAT_ARB: case IR_PAT_FENCE: case IR_PAT_ABORT: case IR_PAT_DEFER: case IR_PAT_ARBNO:
    case IR_PAT_ALT: case IR_PAT_ASSIGN_COND: case IR_PAT_ASSIGN_IMM:
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
        if (joins && c->γ.node && c->γ.node->op == IR_PAT_ALT) {
            IR_t *J = c->γ.node;
            IR_t *sib = c->ω.node;
            if (sib && is_pat_chain_elem(sib->op) && sib->γ.node == J) jn = J;
        }
        arms[n] = c;
        if (joins) joins[n] = jn;
        n++;
        c = jn ? jn->γ.node : c->γ.node;
    }
    if (n >= 2 && c && c->op == IR_PAT_CAT && bb_match_nkids(c) == 0) { if (cat_out) *cat_out = c; return n; }
    if (joins && n >= 2 && c && c->op == IR_SUCCEED) { if (cat_out) *cat_out = NULL; return n; }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_match(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *elem = (pBB && pBB->n_operands > 0) ? pBB->operands[0] : NULL;
    if (!elem) {
        fprintf(stderr, "[SBB] FATAL flat_drive_match: IR_PAT_MATCH has no element in operands\n");
        abort();
    }
    if (g_subject_slot < 0) {
        fprintf(stderr, "[SBB] FATAL flat_drive_match: g_subject_slot unset — a SUBJECT box must precede MATCH in the chain\n");
        abort();
    }
    IR_t *cat_arms[64];
    IR_t *cat_joins[64];
    IR_t *catnd = NULL;
    int catn = gather_lowered_cat_arms(elem, cat_arms, cat_joins, 64, &catnd, NULL);
    /* detect γ/ω-threaded inline alt chain (no operand_aux, arms linked via ω) */
    int inline_alt_n = 0;
    IR_t *inline_alt_arms[64];
    if (catn == 0 && elem->γ.node && elem->γ.node->op == IR_PAT_ALT) {
        int na = 0; bb_operand_aux_get(g_emit_cfg, elem->γ.node, &na);
        if (na == 0) inline_alt_n = gather_inline_alt_arms(elem, inline_alt_arms, 64);
    }
    if (catn == 0 && inline_alt_n < 2 && elem->γ.node) {
        IR_e gt = elem->γ.node->op;
        int real_sibling = (gt != IR_SUCCEED && gt != IR_FAIL) &&
                           (is_pat_chain_elem(gt) || (gt == IR_PAT_CAT && bb_match_nkids(elem->γ.node) == 0));
        if (real_sibling) {
            fprintf(stderr, "[SBB] FATAL flat_drive_match: lowered chain shape not gatherable (entry kind=%d, next kind=%d) — single-walk would silently drop siblings (PB-RB)\n", (int)elem->op, (int)gt);
            abort();
        }
    }
    int id = g_flat_node_id++;
    bb_label_t *match_retry = emit_label_alloc("smatch%d_retry", id);
    bb_label_t *match_adv   = emit_label_alloc("smatch%d_adv",   id);
    bb_label_t *elem_β      = emit_label_alloc("smatch%d_elemb", id);
    int st = bb_slot_alloc16(pBB);
    IR_e _sk = pBB->op;
    g_emit.op_sa = g_subject_slot; g_emit.op_off = st;
    pBB->op = IR_PAT_MATCH_HEAD;
    FILL(pBB, match_retry, lbl_ω, lbl_β);
    pBB->op = _sk;
    emit_label_define_bb(match_retry);
    g_emit.op_sa = g_subject_slot; g_emit.op_off = st;
    pBB->op = IR_PAT_MATCH_RETRY;
    FILL(pBB, match_retry, lbl_ω, elem_β);
    pBB->op = _sk;
    if (catn >= 2) {
        flat_drive_cat_arms(catnd, cat_arms, cat_joins, catn, lbl_γ, match_adv, elem_β);
    } else if (inline_alt_n >= 2) {
        /* γ/ω-threaded inline alt: emit each arm with its own β label, cascade ω to next arm */
        bb_label_t **ai_ωs = (bb_label_t **)alloca((size_t)inline_alt_n * sizeof(bb_label_t *));
        bb_label_t **ai_βs = (bb_label_t **)alloca((size_t)inline_alt_n * sizeof(bb_label_t *));
        for (int i = 0; i < inline_alt_n; i++) {
            ai_ωs[i] = emit_label_alloc("imatch%d_a%d_ω", id, i);
            ai_βs[i] = emit_label_alloc("imatch%d_a%d_β", id, i);
        }
        for (int i = 0; i < inline_alt_n; i++) {
            bb_label_t *arm_ω = (i < inline_alt_n-1) ? ai_ωs[i] : match_adv;
            walk_bb_flat(inline_alt_arms[i], lbl_γ, arm_ω, ai_βs[i]);
            if (i < inline_alt_n-1) emit_label_define_bb(ai_ωs[i]);
        }
    } else {
        walk_bb_flat(elem, lbl_γ, match_adv, elem_β);
    }
    emit_label_define_bb(match_adv);
    g_emit.op_sa = g_subject_slot; g_emit.op_off = st;
    pBB->op = IR_PAT_MATCH_ADVANCE;
    FILL(pBB, match_retry, lbl_ω, elem_β);
    pBB->op = _sk;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_program(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    prog_t *prog = (prog_t *)(intptr_t)IR_LIT(pBB).ival;
    if (!prog || prog->n <= 0) {
        EMIT_PAIR_RESET();
        EMIT_PAIR_JMP(lbl_γ);
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    int id = g_flat_node_id++;
    int n  = prog->n;
    bb_label_t **slbl  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **sbeta = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    for (int i = 0; i < n; i++) {
        slbl[i]  = emit_label_alloc("xsprog%d_s%d_α", id, i);
        sbeta[i] = emit_label_alloc("xsprog%d_s%d_β", id, i);
    }
    if (prog->entry_idx > 0 && prog->entry_idx < n) emit_jmp_label(slbl[prog->entry_idx], JMP_JMP);
    for (int i = 0; i < n; i++) {
        emit_label_define_bb(slbl[i]);
        stmt_t *st = &prog->stmts[i];
        if (st->is_terminal) {
            emit_jmp_label(lbl_γ, JMP_JMP);
            continue;
        }
        if (!st->root) {
            bb_label_t *t = (st->succ_idx >= 0 && st->succ_idx < n) ? slbl[st->succ_idx] : lbl_γ;
            emit_jmp_label(t, JMP_JMP);
            continue;
        }
        bb_label_t *succ = (st->succ_idx >= 0 && st->succ_idx < n) ? slbl[st->succ_idx] : lbl_γ;
        bb_label_t *fail = (st->fail_idx >= 0 && st->fail_idx < n) ? slbl[st->fail_idx] : lbl_ω;
        walk_bb_flat(st->root, succ, fail, sbeta[i]);
    }
    emit_label_define_bb(lbl_β);
    emit_jmp_label(lbl_ω, JMP_JMP);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_global_assign(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *c0 = bb_child0(pBB);
    g_emit.op_sa  = c0 ? bb_slot_get(c0) : -1;
    g_emit.op_off = bb_slot_alloc16(pBB);
    IR_e _sk = pBB->op; pBB->op = IR_ASSIGN_DESCR;
    FILL(pBB, lbl_γ, lbl_ω, lbl_β);
    pBB->op = _sk;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_assign(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *c0 = bb_child0(pBB);
    if (!pBB || !c0) {
        fprintf(stderr, "[IBB] FATAL flat_drive_assign: missing α (lhs IR_VAR)\n");
        abort();
    }
    if (c0->op != IR_VAR || !IR_LIT(c0).sval) {
        fprintf(stderr, "[IBB] FATAL flat_drive_assign: lhs (α) must be IR_VAR with sval (got kind=%d)\n", (int)c0->op);
        abort();
    }
    if (IR_LIT(pBB).ival != 1) {
        if (!((IR_t*)0)) {
            fprintf(stderr, "[IBB] FATAL flat_drive_assign: missing β (rhs expr) and ival != 1\n");
            abort();
        }
        int id = g_flat_node_id++;
        bb_label_t *rhs_done = emit_label_alloc("xassign%d_rhs_done", id);
        bb_label_t *rhs_β    = emit_label_alloc("xassign%d_rhs_β",    id);
        walk_bb_flat(((IR_t*)0), rhs_done, lbl_ω, rhs_β);
        emit_label_define_bb(rhs_done);
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_every(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    (void)lbl_ω;
    if (!pBB || !bb_child0(pBB)) {
        fprintf(stderr, "[IBB] FATAL flat_drive_every: missing body (bb->α)\n");
        abort();
    }
    if (IR_LIT(pBB).ival != 0) {
        fprintf(stderr, "[IBB] FATAL flat_drive_every: bodyless lower-ival=%lld not yet flat-wired (only ival=0)\n", (long long)IR_LIT(pBB).ival);
        abort();
    }
    if (flat_chain_set_has(bb_child0(pBB))) {
        EMIT_PAIR_RESET();
        EMIT_PAIR_JMP(lbl_γ);
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    int id = g_flat_node_id++;
    bb_label_t *body_β = emit_label_alloc("xevery%d_body_β", id);
    walk_bb_flat(bb_child0(pBB), body_β, lbl_γ, body_β);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_swap(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *l_var = (pBB && pBB->n_operands > 0) ? pBB->operands[0] : NULL;
    IR_t *r_var = (pBB && pBB->n_operands > 1) ? pBB->operands[1] : NULL;
    if (!l_var || !r_var ||
        l_var->op != IR_VAR || !IR_LIT(l_var).sval ||
        r_var->op != IR_VAR || !IR_LIT(r_var).sval) {
        fprintf(stderr, "[IBB] FATAL flat_drive_swap: x:=:y requires two IR_VAR operands\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *x_done = emit_label_alloc("xswap%d_x",  id);
    bb_label_t *y_done = emit_label_alloc("xswap%d_y",  id);
    bb_label_t *x_β    = emit_label_alloc("xswap%d_xb", id);
    bb_label_t *y_β    = emit_label_alloc("xswap%d_yb", id);
    walk_bb_flat(l_var, x_done, lbl_ω, x_β);
    emit_label_define_bb(x_done);
    walk_bb_flat(r_var, y_done, lbl_ω, y_β);
    emit_label_define_bb(y_done);
    g_emit.op_sa  = bb_varslot_peek(IR_LIT(l_var).sval);
    g_emit.op_sb  = bb_varslot_peek(IR_LIT(r_var).sval);
    g_emit.op_off = bb_slot_alloc16(pBB);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int while_operand_simple(IR_t *o) {
    if (!o) return 0;
    switch (o->op) {
    case IR_VAR: case IR_LIT_I: case IR_LIT_S: case IR_LIT_F: case IR_LIT_NUL:
        return 1;
    case IR_BINOP:
        return IR_EXEC(o).state == 0 && while_operand_simple(bb_child0(o)) && while_operand_simple(bb_child1(o));
    case IR_ASSIGN:
        return bb_child0(o) && bb_child0(o)->op == IR_VAR && (!bb_child1(o) || while_operand_simple(bb_child1(o)));
    default:
        return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int while_cond_emittable(IR_t *cond) {
    return cond && cond->op == IR_BINOP && IR_EXEC(cond).state >= 1 &&
           while_operand_simple(bb_child0(cond)) && while_operand_simple(bb_child1(cond));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_while(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !bb_child0(pBB)) {
        fprintf(stderr, "[IBB] FATAL flat_drive_while: missing cond (bb->α)\n");
        abort();
    }
    IR_t *cond = bb_child0(pBB);
    IR_t *body = ((IR_t*)0);
    int is_until = (pBB->op == IR_UNTIL);
    int cond_is_relop = (cond->op == IR_BINOP && IR_EXEC(cond).state >= 1);
    int id = g_flat_node_id++;
    bb_label_t *cond_entry = emit_label_alloc("xwhile%d_cond", id);
    bb_label_t *gate       = emit_label_alloc("xwhile%d_gate", id);
    bb_label_t *body_entry = emit_label_alloc("xwhile%d_body", id);
    bb_label_t *cond_β     = emit_label_alloc("xwhile%d_cond_b", id);
    bb_label_t *gate_β     = emit_label_alloc("xwhile%d_gate_b", id);
    bb_label_t *body_β     = emit_label_alloc("xwhile%d_body_b", id);
    emit_label_define_bb(cond_entry);
    if (cond_is_relop) {
        walk_bb_flat(cond, gate, lbl_ω, cond_β);
        emit_label_define_bb(gate);
        EMIT_PAIR_RESET();
        if (!is_until) { EMIT_PAIR_FILL(pBB, body_entry, lbl_γ, gate_β); }
        else           { EMIT_PAIR_FILL(pBB, lbl_γ, body_entry, gate_β); }
    } else {
        if (!is_until) walk_bb_flat(cond, body_entry, lbl_ω, cond_β);
        else           walk_bb_flat(cond, lbl_ω, body_entry, cond_β);
    }
    emit_label_define_bb(body_entry);
    if (body) walk_bb_flat(body, cond_entry, cond_entry, body_β);
    else      emit_jmp_label(cond_entry, JMP_JMP);
    emit_label_define_bb(lbl_β);
    emit_jmp_label(lbl_ω, JMP_JMP);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_gen_alt(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int bb_call_write_route(IR_t *nd) {
    const char *fn = IR_LIT(nd).sval; int64_t narg = IR_LIT(nd).ival; IR_t *a0 = ir_call_arg(nd, 0);
    if (!(fn && !strcmp(fn, "write") && narg == 1 && a0)) return 0;
    if (g_descr_flat_chain && bb_slot_get(a0) >= 0) return 1;
    int wintexpr = (a0->op == IR_BINOP || a0->op == IR_LIT_I || a0->op == IR_TO || a0->op == IR_TO_BY || a0->op == IR_ALT || a0->op == IR_BINOP_GEN || a0->op == IR_VAR || a0->op == IR_NEG || a0->op == IR_POS || a0->op == IR_NONNULL || a0->op == IR_NULL_TEST || a0->op == IR_NOT || a0->op == IR_SIZE || a0->op == IR_CALL || ir_is_call_kind(a0->op) || a0->op == IR_CASE || a0->op == IR_FIELD_GET || a0->op == IR_LIST_BANG || a0->op == IR_LIMIT || a0->op == IR_IDX);
    if (wintexpr && (a0->op == IR_BINOP || a0->op == IR_TO || a0->op == IR_TO_BY)) return (a0->op == IR_BINOP && IR_LIT(a0).ival == BINOP_CONCAT) ? 2 : 3;
    if (wintexpr) return 4;
    if (a0->op == IR_LIT_S && IR_LIT(a0).sval) return 5;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_call_route_classify(IR_t * nd) {
    const char * fn = g_emit.op_sval ? g_emit.op_sval : ""; int64_t narg = g_emit.op_ival; IR_t * a0 = ir_call_arg(nd, 0); double dv = g_emit.op_dval;
    if (g_descr_flat_chain && dv == 2.0 && fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_BYNAME;
    if (g_descr_flat_chain && dv == 2.0 && !strcmp(fn, "__rk_bool")) return CALL_ROUTE_RK_BOOL_COND;
    if (g_descr_flat_chain && dv == 2.0) return CALL_ROUTE_DVAL2_BOMB;
    if (g_gvar_flat_chain && (dv == 2.0 || dv == 3.0) && fn[0] && rt_proc_is_registered(fn)) return CALL_ROUTE_GVAR_USERPROC;
    if (g_descr_flat_chain && fn[0] && rt_proc_is_registered(fn) && dv == 3.0) return CALL_ROUTE_PROC_STAGED;
    if (g_gvar_flat_chain && dv == 3.0 && fn[0] && !rt_proc_is_registered(fn)) return CALL_ROUTE_BYNAME;
    if (g_gvar_flat_chain && dv == 2.0 && fn[0] && !rt_proc_is_registered(fn) && !rt_builtin_is_known(fn)) return CALL_ROUTE_BYNAME;
    if (g_descr_flat_chain && !strcmp(fn, "__rk_bool") && dv == 0.0 && narg == 1 && a0 && bb_slot_get(a0) >= 0) return CALL_ROUTE_RK_BOOL_SLOT;
    switch (g_emit.op_write_route) { case 1: return CALL_ROUTE_WRITE_SLOT; case 2: case 3: return CALL_ROUTE_WRITE_BINOP; case 4: return CALL_ROUTE_WRITE_LEGACY; case 5: return CALL_ROUTE_WRITE_EMPTY; default: break; }
    if (fn[0] && rt_proc_is_registered(fn)) return CALL_ROUTE_USERPROC;
    if (fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_FN;
    return CALL_ROUTE_FATAL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!nd) {
        bb_fill_alpha(nd);
        g_emit.lbl_γ = lbl_γ->name;
        g_emit.lbl_ω = lbl_ω->name;
        g_emit.lbl_β = lbl_β->name;
        g_emit.lbl_γ_p = lbl_γ; g_emit.lbl_ω_p = lbl_ω; g_emit.lbl_β_p = lbl_β;
        return;
    }
    switch (nd->op) {
    case IR_PAT_LIT:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ARB:    g_emit.x86_scratch_off = bb_slot_claim(8); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_REM:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_SPAN:   g_emit.op_name1 = IR_LIT(nd).sval ? IR_LIT(nd).sval : ""; g_emit.op_name2 = "bb_span";   g_emit.op_kind = "SPAN";   g_emit.x86_scratch_off = bb_slot_claim(8); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_SPAN_VAR: g_emit.op_name1 = IR_LIT(nd).sval ? IR_LIT(nd).sval : ""; g_emit.op_name2 = "bb_spanv"; g_emit.op_kind = "SPANV"; g_emit.x86_scratch_off = bb_slot_claim(16); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ANY:    g_emit.op_name1 = IR_LIT(nd).sval ? IR_LIT(nd).sval : ""; g_emit.op_name2 = "bb_any";    g_emit.op_kind = "ANY";    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_BREAK:  g_emit.op_name1 = IR_LIT(nd).sval ? IR_LIT(nd).sval : ""; g_emit.op_name2 = "bb_brk";    g_emit.op_kind = "BREAK";  g_emit.x86_scratch_off = bb_slot_claim(4); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_BREAKX: g_emit.op_name1 = IR_LIT(nd).sval ? IR_LIT(nd).sval : ""; g_emit.op_name2 = "bb_brkx";   g_emit.op_kind = "BREAKX"; g_emit.x86_scratch_off = bb_slot_claim(8); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_NOTANY: g_emit.op_name1 = IR_LIT(nd).sval ? IR_LIT(nd).sval : ""; g_emit.op_name2 = "bb_notany"; g_emit.op_kind = "NOTANY"; FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_LEN:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_POS:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ATP:    g_emit.op_name1 = IR_LIT(nd).sval ? IR_LIT(nd).sval : ""; FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_DEFER:  FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_TAB:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_RTAB:   FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_FENCE:  flat_drive_fence(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ABORT:  { bb_label_t *seal = g_scan_seal_lbl ? g_scan_seal_lbl : lbl_ω; FILL(nd, lbl_γ, seal, lbl_β); break; }
    case IR_PAT_CAT:    flat_drive_cat(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ALT:    flat_drive_alt(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ARBNO: {
        bb_arbno_state_t *az = (bb_arbno_state_t *)(intptr_t)IR_EXEC(nd).counter;
        IR_graph_t *inner = az ? az->inner : NULL;
        IR_t *ch = (inner && inner->entry) ? inner->entry : ((bb_match_nkids(nd) > 0) ? bb_match_kid(nd, 0) : ((IR_t*)0));
        bb_box_fn cfn = ch ? child_cache_get(ch) : NULL;
        g_emit.child_fn    = (void *)cfn;
        g_emit.bb_child_lbl = cfn ? child_cache_get_lbl(cfn) : NULL;
        g_emit.x86_scratch_off = bb_slot_claim(8);
        FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_PAT_ASSIGN_IMM:  flat_drive_capture(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ASSIGN_COND: flat_drive_capture(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_GCONJ:     flat_drive_conj(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_QUERY_FRAME: flat_drive_gz_query(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_GOAL:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_CHOICE:     flat_drive_choice(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_DISJ:     flat_drive_disj(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_ITE:     flat_drive_ite(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_CATCH: {
        EMIT_PAIR_RESET();
        EMIT_PAIR_JMP(lbl_ω);
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β);
        break;
    }
    case IR_ARITH:      FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_BUILTIN:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_UNIFY:      FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_ATOM:       FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SUCCEED: {
        EMIT_PAIR_RESET();
        EMIT_PAIR_JMP(lbl_γ);
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β);
        break;
    }
    case IR_FAIL:       FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_CUT:        FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIT_I:      if (g_descr_flat_chain || g_gvar_callarg_live) g_emit.op_off = bb_slot_alloc16_or_get(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIT_S:      if (g_descr_flat_chain || g_gvar_callarg_live) g_emit.op_off = bb_slot_alloc16(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIT_F:      if (g_descr_flat_chain || g_gvar_callarg_live) g_emit.op_off = bb_slot_alloc16(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIT_NUL:    if (g_descr_flat_chain || g_gvar_callarg_live) g_emit.op_off = bb_slot_alloc16(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_CALL_DEFINE:
    case IR_SCAN_POS: case IR_SCAN_ANY: case IR_SCAN_MATCH: case IR_SCAN_MANY: case IR_SCAN_TAB: case IR_SCAN_MOVE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL:
    case IR_CALL_PROC_STAGED: case IR_CALL_USERPROC: case IR_CALL_BYNAME: case IR_CALL_BUILTIN:
    case IR_CALL: {
        IR_t *a0 = ir_call_arg(nd, 0);
        g_emit.op_arg_slot_n = 0;
        g_emit.op_write_route = bb_call_write_route(nd);
        if (g_descr_flat_chain) {
            if (IR_LIT(nd).dval == 2.0 && IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "__rk_bool") && nd->γ.node && nd->ω.node && nd->γ.node->op == IR_LIT_I && nd->ω.node->op == IR_LIT_I && bb_slot_get(nd->γ.node) < 0 && bb_slot_get(nd->ω.node) < 0) { int _sh = bb_slot_alloc16(nd->γ.node); bb_slot_register(nd->ω.node, _sh); }
            if (nd->op == IR_SCAN_POS) {
                IR_graph_t **sblks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
                long sn = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0] && sblks[0]->entry && sblks[0]->entry->op == IR_LIT_I && arg_entry_terminal(sblks[0]->entry)) ? (long) IR_LIT(sblks[0]->entry).ival : -1;
                g_emit.op_sb  = (int) sn;
                g_emit.op_sa  = -1;
                g_emit.op_off = bb_slot_alloc16(nd);
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
                break;
            }
            if (nd->op == IR_SCAN_ANY || nd->op == IR_SCAN_MATCH) {
                IR_graph_t **sblks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
                const char *cs = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0] && sblks[0]->entry && sblks[0]->entry->op == IR_LIT_S && arg_entry_terminal(sblks[0]->entry)) ? IR_LIT(sblks[0]->entry).sval : (const char *)0;
                g_emit.op_name1 = cs;
                g_emit.op_sa  = -1;
                g_emit.op_sb  = -1;
                g_emit.op_off = bb_slot_alloc16(nd);
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
                break;
            }
            if (nd->op == IR_SCAN_MANY) {
                IR_graph_t **sblks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
                const char *cs = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0] && sblks[0]->entry && sblks[0]->entry->op == IR_LIT_S && arg_entry_terminal(sblks[0]->entry)) ? IR_LIT(sblks[0]->entry).sval : (const char *)0;
                g_emit.op_name1 = cs;
                g_emit.op_sa  = -1;
                g_emit.op_sb  = -1;
                g_emit.op_off = bb_slot_alloc16(nd);
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
                break;
            }
            if (nd->op == IR_SCAN_TAB) {
                IR_graph_t **sblks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
                IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
                long tn = (ae && ae->op == IR_LIT_I && IR_LIT(ae).ival >= 1 && arg_entry_terminal(ae)) ? (long) IR_LIT(ae).ival : -1;
                int  sa = -1;
                if (tn < 0 && ae && (ae->op == IR_CALL || ir_is_call_kind(ae->op) || ir_is_scan_kind(ae->op)) && arg_entry_terminal(ae)) {
                    sa = bb_slot_get(ae);
                    if (sa < 0) {
                        int tid = g_flat_node_id++;
                        bb_label_t *targ_done = emit_label_alloc("xscantab%d_arg_done", tid);
                        descr_chain_operand_refs(ae);
                        flat_emit_arg_subchain(ae, targ_done, lbl_ω);
                        emit_label_define_bb(targ_done);
                        sa = bb_slot_get(ae);
                    }
                }
                g_emit.op_sb  = (int) tn;
                g_emit.op_sa  = sa;
                g_emit.op_off = bb_slot_alloc16(nd);
                (void) bb_slot_claim(8);
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
                break;
            }
            if (nd->op == IR_SCAN_MOVE) {
                IR_graph_t **sblks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
                IR_t *ae = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0]) ? sblks[0]->entry : (IR_t *)0;
                int litok = (ae && ae->op == IR_LIT_I && arg_entry_terminal(ae));
                g_emit.op_sb  = litok ? (int) IR_LIT(ae).ival : 0;
                g_emit.op_sa  = litok ? 1 : -1;
                g_emit.op_off = bb_slot_alloc16(nd);
                (void) bb_slot_claim(8);
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
                break;
            }
            if (nd->op == IR_SCAN_UPTO) {
                IR_graph_t **sblks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
                const char *cs = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0] && sblks[0]->entry && sblks[0]->entry->op == IR_LIT_S && arg_entry_terminal(sblks[0]->entry)) ? IR_LIT(sblks[0]->entry).sval : (const char *)0;
                g_emit.op_name1 = cs;
                g_emit.op_sa  = -1;
                g_emit.op_sb  = -1;
                g_emit.op_off = bb_slot_alloc16(nd);
                (void) bb_slot_claim(8);
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
                break;
            }
            if (nd->op == IR_SCAN_FIND) {
                IR_graph_t **sblks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
                const char *cs = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0] && sblks[0]->entry && sblks[0]->entry->op == IR_LIT_S && arg_entry_terminal(sblks[0]->entry)) ? IR_LIT(sblks[0]->entry).sval : (const char *)0;
                g_emit.op_name1 = cs;
                g_emit.op_sa  = -1;
                g_emit.op_sb  = -1;
                g_emit.op_off = bb_slot_alloc16(nd);
                (void) bb_slot_claim(8);
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
                break;
            }
            if (nd->op == IR_SCAN_BAL) {
                IR_graph_t **sblks = (IR_graph_t **)(intptr_t) IR_EXEC(nd).counter;
                const char *cs = (sblks && (int)IR_LIT(nd).ival == 1 && sblks[0] && sblks[0]->entry && sblks[0]->entry->op == IR_LIT_S && arg_entry_terminal(sblks[0]->entry)) ? IR_LIT(sblks[0]->entry).sval : (const char *)0;
                g_emit.op_name1 = cs;
                g_emit.op_sa  = -1;
                g_emit.op_sb  = -1;
                g_emit.op_off = bb_slot_alloc16(nd);
                (void) bb_slot_claim(8);
                (void) bb_slot_claim(8);
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
                break;
            }
            if (IR_LIT(nd).dval == 3.0 && (int)IR_LIT(nd).ival > 0 && IR_LIT(nd).sval && rt_proc_is_registered(IR_LIT(nd).sval))
                flat_drive_userproc(nd, lbl_γ, lbl_ω, lbl_β);
            else
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
            break;
        }
        if (g_gvar_flat_chain && (IR_LIT(nd).dval == 2.0 || IR_LIT(nd).dval == 5.0)) { if (IR_LIT(nd).dval == 2.0) { IR_graph_t **a2subs = (IR_graph_t **)(intptr_t)IR_EXEC(nd).counter; for (int ci = 0; ci < (int)IR_LIT(nd).ival; ci++) if (a2subs && a2subs[ci] && a2subs[ci]->entry) gvar_stmt_operand_refs(a2subs[ci]->entry); gvar_drive_call_arg_slots(nd, lbl_ω); } FILL(nd, lbl_γ, lbl_ω, lbl_β); break; }
        if (g_gvar_flat_chain && IR_LIT(nd).dval == 3.0) {
            IR_graph_t **csubs = (IR_graph_t **)(intptr_t)IR_EXEC(nd).counter;
            for (int ci = 0; ci < (int)IR_LIT(nd).ival; ci++) if (csubs && csubs[ci] && csubs[ci]->entry) gvar_stmt_operand_refs(csubs[ci]->entry);
            gvar_drive_call_arg_slots(nd, lbl_ω);
            FILL(nd, lbl_γ, lbl_ω, lbl_β);
            break;
        }
        int is_intexpr_shape = (a0 && (a0->op == IR_BINOP || a0->op == IR_LIT_I || a0->op == IR_TO || a0->op == IR_TO_BY || a0->op == IR_ALT || a0->op == IR_BINOP_GEN || a0->op == IR_VAR ||
                   a0->op == IR_NEG || a0->op == IR_POS || a0->op == IR_NONNULL || a0->op == IR_NULL_TEST || a0->op == IR_NOT || a0->op == IR_SIZE || a0->op == IR_CALL || ir_is_call_kind(a0->op) || a0->op == IR_CASE || a0->op == IR_FIELD_GET || a0->op == IR_LIST_BANG || a0->op == IR_LIMIT || a0->op == IR_IDX ));
        int is_write_fn   = (IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval, "write") || !strcmp(IR_LIT(nd).sval, "writes")));
        int write_str_simple1 = (IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, "write") && (int)IR_LIT(nd).ival == 1 && a0 && a0->op == IR_LIT_S && IR_LIT(a0).sval);
        int write_simple1 = ((is_write_fn && (int)IR_LIT(nd).ival == 1 && is_intexpr_shape) || write_str_simple1);
        int builtin_ok    = (IR_LIT(nd).sval && rt_builtin_is_known(IR_LIT(nd).sval) && call_args_single_shot(nd) && !write_simple1);
        if (IR_LIT(nd).sval && rt_proc_is_registered(IR_LIT(nd).sval))
            flat_drive_call_userproc(nd, lbl_γ, lbl_ω, lbl_β);
        else if (builtin_ok)
            flat_drive_call_builtin(nd, lbl_γ, lbl_ω, lbl_β);
        else if (is_intexpr_shape)
            flat_drive_call_intexpr(nd, lbl_γ, lbl_ω, lbl_β);
        else
            FILL(nd, lbl_γ, lbl_ω, lbl_β);
        break;
    }
    case IR_BINOP: {
        g_emit.op_relop_descr = 0;
        if (g_gvar_flat_chain && nd && !bb_child0(nd) && g_emit_cfg) { int _na = 0; IR_t * const * _ax = bb_operand_aux_get(g_emit_cfg, nd, &_na); if (_na >= 2 && _ax[0] && _ax[1]) { nd->n_operands = 0; ir_operand_push(nd, _ax[0]); ir_operand_push(nd, _ax[1]); } }
        int op_is_rel = nd && ((IR_LIT(nd).ival >= BINOP_LT && IR_LIT(nd).ival <= BINOP_NE) ||
                               (IR_LIT(nd).ival >= BINOP_SLT && IR_LIT(nd).ival <= BINOP_SNE));
        int op_is_arith = nd && (IR_LIT(nd).ival == BINOP_ADD || IR_LIT(nd).ival == BINOP_SUB || IR_LIT(nd).ival == BINOP_MUL || IR_LIT(nd).ival == BINOP_DIV || IR_LIT(nd).ival == BINOP_MOD);
        int op_is_concat = nd && (IR_LIT(nd).ival == BINOP_CONCAT);
        int op_is_pow = nd && (IR_LIT(nd).ival == BINOP_POW);
        g_emit.op_off = -1;
        if (g_gvar_flat_chain && op_is_pow && bb_child0(nd) && bb_child1(nd) && nd->γ.node && nd->γ.node->op == IR_ASSIGN && IR_LIT(nd->γ.node).sval
            && (bb_child0(nd)->op == IR_LIT_I || bb_child0(nd)->op == IR_LIT_F || (bb_child0(nd)->op == IR_UNOP && bb_child0(bb_child0(nd)) && bb_child0(bb_child0(nd))->op == IR_LIT_I))
            && (bb_child1(nd)->op == IR_LIT_I || bb_child1(nd)->op == IR_LIT_F || (bb_child1(nd)->op == IR_UNOP && bb_child0(bb_child1(nd)) && bb_child0(bb_child1(nd))->op == IR_LIT_I))) {
            IR_t *asgn = nd->γ.node;
            IR_t *lc = bb_child0(nd); IR_t *rc = bb_child1(nd);
            union { double d; int64_t q; } _pl; _pl.d = (lc->op == IR_LIT_F) ? IR_LIT(lc).dval : 0.0;
            union { double d; int64_t q; } _pr; _pr.d = (rc->op == IR_LIT_F) ? IR_LIT(rc).dval : 0.0;
            g_emit.bb_lk = (lc->op == IR_LIT_F) ? (int)IR_LIT_F : (int)IR_LIT_I;
            g_emit.bb_rk = (rc->op == IR_LIT_F) ? (int)IR_LIT_F : (int)IR_LIT_I;
            g_emit.bb_li = _pl.q;
            g_emit.bb_ri = _pr.q;
            g_emit.op_sa = (lc->op == IR_UNOP) ? (int)((int)IR_LIT(lc).ival == (int)TT_MNS ? -IR_LIT(bb_child0(lc)).ival : IR_LIT(bb_child0(lc)).ival) : (lc->op == IR_LIT_I ? (int)IR_LIT(lc).ival : 0);
            g_emit.op_sb = (rc->op == IR_UNOP) ? (int)((int)IR_LIT(rc).ival == (int)TT_MNS ? -IR_LIT(bb_child0(rc)).ival : IR_LIT(bb_child0(rc)).ival) : (rc->op == IR_LIT_I ? (int)IR_LIT(rc).ival : 0);
            g_emit.op_kind  = "POW";
            g_emit.op_name1 = (const char *)0;
            g_emit.op_name2 = (const char *)0;
            g_emit.op_sval  = IR_LIT(asgn).sval;
            g_emit.op_off   = bb_slot_alloc(nd);
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = asgn->op; asgn->op = IR_BINOP_GVAR_ARITH; EMIT_PAIR_FILL(asgn, lbl_γ, lbl_ω, lbl_β); asgn->op = _sk; }
        } else if (g_gvar_flat_chain && op_is_arith && bb_child0(nd) && bb_child1(nd) && bb_child0(nd)->op == IR_LIT_I && bb_child1(nd)->op == IR_LIT_I) {
            g_emit.op_sa  = (int)IR_LIT(bb_child0(nd)).ival;
            g_emit.op_sb  = (int)IR_LIT(bb_child1(nd)).ival;
            g_emit.op_off = bb_slot_alloc(nd);
            g_emit.op_name1 = (const char *)0;
            g_emit.op_name2 = (const char *)0;
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->op; nd->op = IR_BINOP_GVAR_ARITH; EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->op = _sk; }
        } else if (g_gvar_flat_chain && op_is_arith && bb_child0(nd) && bb_child1(nd) && bb_child0(nd)->op == IR_VAR && bb_child1(nd)->op == IR_VAR && IR_LIT(bb_child0(nd)).sval && IR_LIT(bb_child1(nd)).sval) {
            g_emit.op_name1 = IR_LIT(bb_child0(nd)).sval;
            g_emit.op_name2 = IR_LIT(bb_child1(nd)).sval;
            g_emit.op_off   = bb_slot_alloc(nd);
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->op; nd->op = IR_BINOP_GVAR_ARITH; EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->op = _sk; }
        } else if (g_gvar_flat_chain && op_is_arith && bb_child0(nd) && bb_child1(nd) && bb_child0(nd)->op == IR_VAR && IR_LIT(bb_child0(nd)).sval && bb_child1(nd)->op == IR_LIT_I) {
            g_emit.op_name1 = IR_LIT(bb_child0(nd)).sval;
            g_emit.op_name2 = (const char *)0;
            g_emit.op_sb    = (int)IR_LIT(bb_child1(nd)).ival;
            g_emit.op_off   = bb_slot_alloc(nd);
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->op; nd->op = IR_BINOP_GVAR_ARITH; EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->op = _sk; }
        } else if (g_gvar_flat_chain && op_is_arith && bb_child0(nd) && bb_child1(nd) && bb_child0(nd)->op == IR_LIT_I && bb_child1(nd)->op == IR_VAR && IR_LIT(bb_child1(nd)).sval) {
            g_emit.op_name1 = (const char *)0;
            g_emit.op_name2 = IR_LIT(bb_child1(nd)).sval;
            g_emit.op_sa    = (int)IR_LIT(bb_child0(nd)).ival;
            g_emit.op_off   = bb_slot_alloc(nd);
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->op; nd->op = IR_BINOP_GVAR_ARITH; EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->op = _sk; }
        } else if (g_gvar_flat_chain && op_is_arith && bb_child0(nd) && bb_child1(nd) &&
                   ((bb_child0(nd)->op == IR_LIT_I) || (bb_child0(nd)->op == IR_VAR && IR_LIT(bb_child0(nd)).sval) || bb_slot_get(bb_child0(nd)) >= 0) &&
                   ((bb_child1(nd)->op == IR_LIT_I) || (bb_child1(nd)->op == IR_VAR && IR_LIT(bb_child1(nd)).sval) || bb_slot_get(bb_child1(nd)) >= 0)) {
            g_emit.bb_lk    = (int)ir_norm_call_kind(bb_child0(nd)->op);
            g_emit.bb_rk    = (int)ir_norm_call_kind(bb_child1(nd)->op);
            g_emit.bb_li    = (bb_child0(nd)->op == IR_LIT_I) ? IR_LIT(bb_child0(nd)).ival : 0;
            g_emit.bb_ri    = (bb_child1(nd)->op == IR_LIT_I) ? IR_LIT(bb_child1(nd)).ival : 0;
            g_emit.op_name1 = (bb_child0(nd)->op == IR_VAR) ? IR_LIT(bb_child0(nd)).sval : (const char *)0;
            g_emit.op_name2 = (bb_child1(nd)->op == IR_VAR) ? IR_LIT(bb_child1(nd)).sval : (const char *)0;
            g_emit.op_sa    = (bb_child0(nd)->op != IR_LIT_I && bb_child0(nd)->op != IR_VAR) ? bb_slot_get(bb_child0(nd)) : -1;
            g_emit.op_sb    = (bb_child1(nd)->op != IR_LIT_I && bb_child1(nd)->op != IR_VAR) ? bb_slot_get(bb_child1(nd)) : -1;
            g_emit.op_off   = bb_slot_alloc(nd);
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->op; nd->op = IR_BINOP_GVAR_ARITH_SLOT; EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->op = _sk; }
        } else if (g_gvar_flat_chain && op_is_rel) {
            int _na = 0;
            IR_t * const * _ax = g_emit_cfg ? bb_operand_aux_get(g_emit_cfg, nd, &_na) : (IR_t * const *)0;
            IR_t *_c0 = (_na >= 2 && _ax[0]) ? _ax[0] : bb_child0(nd);
            IR_t *_c1 = (_na >= 2 && _ax[1]) ? _ax[1] : bb_child1(nd);
            if (_c0 && _c1 &&
                ((_c0->op == IR_LIT_I || _c0->op == IR_LIT_NUL) || (_c0->op == IR_VAR && IR_LIT(_c0).sval) || (_c0->op == IR_LIT_S && IR_LIT(_c0).sval) || bb_slot_get(_c0) >= 0) &&
                ((_c1->op == IR_LIT_I || _c1->op == IR_LIT_NUL) || (_c1->op == IR_VAR && IR_LIT(_c1).sval) || (_c1->op == IR_LIT_S && IR_LIT(_c1).sval) || bb_slot_get(_c1) >= 0)) {
            g_emit.bb_lk    = (int)ir_norm_call_kind(_c0->op);
            g_emit.bb_rk    = (int)ir_norm_call_kind(_c1->op);
            g_emit.bb_li    = (_c0->op == IR_LIT_I) ? IR_LIT(_c0).ival : 0;
            g_emit.bb_ri    = (_c1->op == IR_LIT_I) ? IR_LIT(_c1).ival : 0;
            g_emit.op_name1 = (_c0->op == IR_VAR) ? IR_LIT(_c0).sval : (const char *)0;
            g_emit.op_name2 = (_c1->op == IR_VAR) ? IR_LIT(_c1).sval : (const char *)0;
            g_emit.op_sa    = (_c0->op != IR_LIT_I && _c0->op != IR_LIT_NUL && _c0->op != IR_VAR) ? bb_slot_get(_c0) : -1;
            g_emit.op_sb    = (_c1->op != IR_LIT_I && _c1->op != IR_LIT_NUL && _c1->op != IR_VAR) ? bb_slot_get(_c1) : -1;
            { int _isrel = (IR_LIT(nd).ival >= BINOP_LT && IR_LIT(nd).ival <= BINOP_NE); int _eqne = (IR_LIT(nd).ival == BINOP_EQ || IR_LIT(nd).ival == BINOP_NE); int _lok = (_c0->op == IR_CALL || ir_is_call_kind(_c0->op)) || (_c0->op == IR_VAR && IR_LIT(_c0).sval) || (_c0->op == IR_LIT_S && IR_LIT(_c0).sval); int _rok = (_c1->op == IR_CALL || ir_is_call_kind(_c1->op)) || (_c1->op == IR_VAR && IR_LIT(_c1).sval) || (_c1->op == IR_LIT_S && IR_LIT(_c1).sval); int _strtrig = (IR_LIT(nd).dval == 1.0) || (_c0->op == IR_LIT_S) || (_c1->op == IR_LIT_S); int _calltrig = _eqne && ((_c0->op == IR_CALL || ir_is_call_kind(_c0->op)) || (_c1->op == IR_CALL || ir_is_call_kind(_c1->op))); g_emit.op_relop_descr = (_isrel && _lok && _rok && (_strtrig || _calltrig)) ? 1 : 0; }
            g_emit.op_off   = g_emit.op_relop_descr ? bb_slot_alloc16(nd) : bb_slot_alloc(nd);
            { static char gvrpool[2][64]; static char gvrlit[2][64]; g_emit.op_parts_lbl[0] = NULL; g_emit.op_parts_lbl[1] = NULL; g_emit.op_parts_str[0] = NULL; g_emit.op_parts_str[1] = NULL;
              if (g_emit.op_name1 && g_emit.op_name1[0]) { strtab_label(gvrpool[0], 64, g_emit.op_name1); g_emit.op_parts_lbl[0] = gvrpool[0]; }
              if (g_emit.op_name2 && g_emit.op_name2[0]) { strtab_label(gvrpool[1], 64, g_emit.op_name2); g_emit.op_parts_lbl[1] = gvrpool[1]; }
              if (_c0->op == IR_LIT_S && IR_LIT(_c0).sval) { g_emit.op_parts_str[0] = IR_LIT(_c0).sval; g_emit.op_parts_lbl[0] = bb_intern_into(gvrlit[0], IR_LIT(_c0).sval); }
              if (_c1->op == IR_LIT_S && IR_LIT(_c1).sval) { g_emit.op_parts_str[1] = IR_LIT(_c1).sval; g_emit.op_parts_lbl[1] = bb_intern_into(gvrlit[1], IR_LIT(_c1).sval); } }
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->op; nd->op = IR_BINOP_GVAR_RELOP; EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->op = _sk; }
            } else { flat_drive_binop_tree(nd, lbl_γ, lbl_ω, lbl_β); }
        } else if (g_descr_flat_chain && (op_is_rel || op_is_arith || op_is_concat)) {
            int needs_walk = (bb_child0(nd) && bb_child0(nd)->op != IR_LIT_I && bb_child0(nd)->op != IR_LIT_S && descr_binop_opnd_slot(bb_child0(nd)) < 0)
                          || (bb_child1(nd) && bb_child1(nd)->op != IR_LIT_I && bb_child1(nd)->op != IR_LIT_S && descr_binop_opnd_slot(bb_child1(nd)) < 0);
            if (needs_walk) {
                flat_drive_binop_tree(nd, lbl_γ, lbl_ω, lbl_β);
            } else {
                g_emit.op_sa = descr_binop_opnd_slot(bb_child0(nd));
                g_emit.op_sb = descr_binop_opnd_slot(bb_child1(nd));
                if (g_emit.op_sa >= 0 && g_emit.op_sb >= 0) g_emit.op_off = bb_slot_alloc16(nd);
                EMIT_PAIR_RESET();
                EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
                { IR_e _sk = nd->op; nd->op = binop_slot_kind(nd); EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->op = _sk; }
            }
        } else if (!bb_child0(nd) && !bb_child1(nd)) {
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β);
        } else {
            flat_drive_binop_tree(nd, lbl_γ, lbl_ω, lbl_β);
        }
        break;
    }
    case IR_IF: {
        if (g_gvar_flat_chain || g_descr_flat_chain) {
            emit_label_define_bb(lbl_β);
            emit_jmp_label(lbl_γ, JMP_JMP);
            emit_jmp_label(lbl_γ, JMP_JMP);
            break;
        }
        EMIT_PAIR_RESET();
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β);
        break;
    }
    case IR_BINOP_GEN:
        if (bb_child0(nd) && bb_child1(nd) && !binop_operand_streams(bb_child0(nd)) && !binop_operand_streams(bb_child1(nd))) {
            IR_e saved_kind = nd->op;
            nd->op = IR_BINOP;
            flat_drive_binop_tree(nd, lbl_γ, lbl_ω, lbl_β);
            nd->op = saved_kind;
        } else {
            flat_drive_binop_gen_tree(nd, lbl_γ, lbl_ω, lbl_β);
        }
        break;
    case IR_SEQ:        if (g_gvar_flat_chain && nd && IR_LIT(nd).dval == 1.0) { flat_drive_gvar_seq_passthrough(nd, lbl_γ, lbl_ω, lbl_β); } else flat_drive_seq(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SEQ_EXPR:   flat_drive_seq(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_EVERY:      flat_drive_every(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIMIT:      flat_drive_limit(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_TO:         flat_drive_to(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_GATHER:     FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_MAP:        FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_GREP:       FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_TO_BY:      flat_drive_to(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_ALT:        if (g_descr_flat_chain) flat_drive_alt_gen(nd, lbl_γ, lbl_ω, lbl_β); else flat_drive_gen_alt(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_VAR:        if (g_descr_flat_chain && nd && IR_LIT(nd).sval && IR_LIT(nd).sval[0] == '&') { g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_sa = -1; g_emit.op_off = bb_slot_alloc16(nd); } else if (g_descr_flat_chain && nd && IR_LIT(nd).sval) { extern int is_global(const char *); if (is_global(IR_LIT(nd).sval)) { g_emit.op_sa = -1; g_emit.op_off = bb_slot_alloc16(nd); g_emit.op_sval = IR_LIT(nd).sval; } else { int voff = bb_varslot_peek(IR_LIT(nd).sval); g_emit.op_sa = voff; g_emit.op_off = (voff >= 0) ? bb_slot_alloc16(nd) : -1; } } else if (g_gvar_flat_chain && nd && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '&') { g_emit.op_sa = -1; g_emit.op_off = bb_slot_alloc16(nd); g_emit.op_sval = IR_LIT(nd).sval; } else { g_emit.op_sa = -1; g_emit.op_off = -1; } FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_ASSIGN_LIT_S: case IR_ASSIGN_LIT_I:
    case IR_ASSIGN_VAR: case IR_ASSIGN_CONCAT: case IR_ASSIGN_CALL:
    case IR_ASSIGN:     { IR_t *ac0 = bb_child0(nd);
        if (g_descr_flat_chain) { extern int is_global(const char *); if (IR_LIT(nd).sval && is_global(IR_LIT(nd).sval)) flat_drive_global_assign(nd, lbl_γ, lbl_ω, lbl_β); else { g_emit.op_sb = bb_varslot(IR_LIT(nd).sval); g_emit.op_off = bb_slot_alloc16(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); } }
        else if (IR_LIT(nd).sval && ac0 && (ac0->op == IR_LIT_S || ac0->op == IR_LIT_I || ac0->op == IR_LIT_F || ac0->op == IR_VAR || ac0->op == IR_VAR_FRAME || ac0->op == IR_VAR_FRAME_REF || ac0->op == IR_SEQ || ac0->op == IR_SEQ_EXPR || ac0->op == IR_CALL || ac0->op == IR_CALL_DEFINE)) flat_drive_gvar_assign(nd, lbl_γ, lbl_ω, lbl_β);
        else if (IR_LIT(nd).sval && ac0 && ac0->op == IR_BINOP && (int)IR_LIT(ac0).ival == (int)BINOP_POW && bb_slot_get(ac0) >= 0) { emit_jmp_label(lbl_γ, JMP_JMP); }
        else if (IR_LIT(nd).sval && ac0 && ac0->op == IR_BINOP) flat_drive_gvar_assign_binop(nd, lbl_γ, lbl_ω, lbl_β);
        else if (IR_LIT(nd).sval && ac0 && ac0->op == IR_UNOP) flat_drive_gvar_assign_binop(nd, lbl_γ, lbl_ω, lbl_β);
        else flat_drive_assign(nd, lbl_γ, lbl_ω, lbl_β); } break;
    case IR_VAR_FRAME: case IR_VAR_FRAME_REF: g_emit.op_off = bb_slot_alloc16(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_ASSIGN_FRAME: case IR_ASSIGN_FRAME_REF: if (bb_child0(nd) && bb_child0(nd)->op == IR_BINOP) flat_drive_gvar_assign_binop(nd, lbl_γ, lbl_ω, lbl_β); else flat_drive_gvar_assign(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KEYWORD:    if (g_descr_flat_chain || g_gvar_flat_chain) { g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = bb_slot_alloc16(nd); } else { g_emit.op_off = -1; } FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_GEN_SCAN:   flat_drive_gen_scan(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SCAN:       flat_drive_scan_stmt(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SUBJECT:    flat_drive_subject(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_REF_INVARIANT: flat_drive_ref_invariant(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_MATCH:  flat_drive_match(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_LIT: g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_LEN:  g_emit.op_kind = "LEN";  g_emit.op_ival = IR_LIT(nd).ival; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_POS:  g_emit.op_kind = "POS";  g_emit.op_ival = IR_LIT(nd).ival; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_RPOS: g_emit.op_kind = "RPOS"; g_emit.op_ival = IR_LIT(nd).ival; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_TAB:  g_emit.op_kind = "TAB";  g_emit.op_ival = IR_LIT(nd).ival; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_RTAB: g_emit.op_kind = "RTAB"; g_emit.op_ival = IR_LIT(nd).ival; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_ANY:    g_emit.op_kind = "ANY";    g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_NOTANY: g_emit.op_kind = "NOTANY"; g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_SPAN:   g_emit.op_kind = "SPAN";   g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_BREAK:  g_emit.op_kind = "BREAK";  g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_BREAKX: g_emit.op_kind = "BREAKX"; g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_FAIL:    g_emit.op_kind = "FAIL";    g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_REM:     g_emit.op_kind = "REM";     g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_SUCCEED: g_emit.op_kind = "SUCCEED"; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_ARB:     g_emit.op_kind = "ARB";     g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_FENCE:   g_emit.op_kind = "FENCE";   g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_ABORT:   g_emit.op_kind = "ABORT";   g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_CAT: g_emit.op_sa = (nd->n_operands > 0) ? bb_slot_get(nd->operands[0]) : -1; g_emit.op_sb = (nd->n_operands > 1) ? bb_slot_get(nd->operands[1]) : -1; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PATTERN_ALT: g_emit.op_sa = (nd->n_operands > 0) ? bb_slot_get(nd->operands[0]) : -1; g_emit.op_sb = (nd->n_operands > 1) ? bb_slot_get(nd->operands[1]) : -1; g_emit.op_off = bb_slot_alloc24(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_DTP_ASSIGN:  g_emit.op_sa = (nd->n_operands > 0) ? bb_slot_get(nd->operands[0]) : -1; FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PROG:   flat_drive_program(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_RETURN:
        if (g_descr_flat_chain) { FILL(nd, lbl_γ, lbl_ω, lbl_β); break; }
        if (g_gvar_flat_chain) {
            bb_label_t *slab_succ = g_emit.flat_succ_p ? g_emit.flat_succ_p : lbl_γ;
            bb_label_t *slab_fail = g_emit.flat_fail_p ? g_emit.flat_fail_p : lbl_ω;
            bb_label_t *exit_lbl = (nd && IR_LIT(nd).dval == 2.0) ? slab_fail : slab_succ;
            emit_label_define_bb(lbl_β);
            emit_jmp_label(exit_lbl, JMP_JMP);
            emit_jmp_label(exit_lbl, JMP_JMP);
            break;
        }
        flat_drive_return(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SWAP:       flat_drive_swap(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_WHILE:
    case IR_UNTIL:
        if (g_gvar_flat_chain || g_descr_flat_chain) {
            emit_label_define_bb(lbl_β);
            emit_jmp_label(lbl_γ, JMP_JMP);
            emit_jmp_label(lbl_γ, JMP_JMP);
        } else if (while_cond_emittable(bb_child0(nd))) {
            flat_drive_while(nd, lbl_γ, lbl_ω, lbl_β);
        } else {
            emit_label_define_bb(lbl_β);
            emit_jmp_label(lbl_ω, JMP_JMP);
            emit_jmp_label(lbl_ω, JMP_JMP);
        }
        break;
    case IR_UNOP:
    case IR_NEG:
    case IR_POS:
    case IR_NONNULL:
    case IR_NULL_TEST:
    case IR_SIZE:
    case IR_NOT:
        if (g_descr_flat_chain) {
            g_emit.op_sa   = descr_binop_opnd_slot(bb_child0(nd));
            g_emit.op_off  = bb_slot_alloc16(nd);
            FILL(nd, lbl_γ, lbl_ω, lbl_β);
        } else if (g_gvar_flat_chain && nd->op == IR_UNOP && (IR_LIT(nd).ival == TT_MNS || IR_LIT(nd).ival == TT_PLS) && bb_child0(nd)
                   && (bb_child0(nd)->op == IR_LIT_I || (bb_child0(nd)->op == IR_VAR && IR_LIT(bb_child0(nd)).sval) || bb_slot_get(bb_child0(nd)) >= 0)) {
            g_emit.bb_lk    = (int)ir_norm_call_kind(bb_child0(nd)->op);
            g_emit.bb_li    = (bb_child0(nd)->op == IR_LIT_I) ? IR_LIT(bb_child0(nd)).ival : 0;
            g_emit.op_name1 = (bb_child0(nd)->op == IR_VAR) ? IR_LIT(bb_child0(nd)).sval : (const char *)0;
            g_emit.op_sa    = (bb_child0(nd)->op != IR_LIT_I && bb_child0(nd)->op != IR_VAR) ? bb_slot_get(bb_child0(nd)) : -1;
            g_emit.op_ival  = IR_LIT(nd).ival;
            g_emit.op_off   = bb_slot_alloc(nd);
            { static char gvupool[64]; g_emit.op_parts_lbl[0] = NULL; if (g_emit.op_name1 && g_emit.op_name1[0]) { strtab_label(gvupool, 64, g_emit.op_name1); g_emit.op_parts_lbl[0] = gvupool; } }
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->op; nd->op = IR_UNOP_GVAR_SLOT; EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->op = _sk; }
        } else {
            flat_drive_unop(nd, lbl_γ, lbl_ω, lbl_β);
        }
        break;
    case IR_INITIAL:    flat_drive_initial(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_CASE:       flat_drive_case(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_FIELD_GET:  flat_drive_field_get(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_FIELD_SET:  flat_drive_field_set(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_IDX:        flat_drive_idx_get(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_IDX_SET:    flat_drive_idx_set(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIST_BANG:  flat_drive_list_bang(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_CONJ:
        emit_jmp_label(lbl_γ, JMP_JMP);
        emit_label_define_bb(lbl_β);
        emit_jmp_label(lbl_ω, JMP_JMP);
        break;
    case IR_BREAK:
    case IR_NEXT:
    case IR_REPEAT:
        emit_label_define_bb(lbl_β);
        emit_jmp_label(lbl_γ, JMP_JMP);
        emit_jmp_label(lbl_γ, JMP_JMP);
        break;
    default:
        emit_label_define_bb(lbl_β);
        emit_jmp_label(lbl_ω, JMP_JMP);
        emit_jmp_label(lbl_ω, JMP_JMP);
        break;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int ir_node_is_alt_arm(IR_t *nd) {
    if (!nd || !g_emit_cfg) return 0;
    if (!(nd->γ.node && (nd->γ.node->op == IR_ALT || nd->γ.node->op == IR_PAT_ALT))) return 0;
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
        if (c->γ.node && qt < CH_MAX) queue[qt++] = c->γ.node;
        if ((c->op == IR_BINOP || c->op == IR_BINOP_GEN) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if ((c->op == IR_CALL || c->op == IR_CALL_DEFINE) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if (c->op == IR_GATHER && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if ((c->op == IR_MAP || c->op == IR_GREP) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
    }
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    for (int i = 0; i < n && g_flat_chain_set_n < FLAT_CHAIN_SET_MAX; i++) g_flat_chain_set[g_flat_chain_set_n++] = nodes[i];
    int id = g_flat_node_id++;
    for (int i = 0; i < n; i++) {
        lbls[i]  = emit_label_alloc("xchain%d_n%d_α", id, i);
        betas[i] = emit_label_alloc("xchain%d_n%d_β", id, i);
    }
    for (int i = 0; i < n; i++) {
        emit_label_define_bb(lbls[i]);
        bb_label_t *node_γ = &lbl_γ;
        bb_label_t *node_ω = &lbl_ω;
        for (int k = 0; k < n; k++) if (nodes[k] == nodes[i]->γ.node) {
            node_γ = (i > k && ir_is_generator_kind(nodes[k]->op)) ? betas[k] : lbls[k];
            break;
        }
        if (nodes[i]->γ.node == NULL || nodes[i]->γ.node->op == IR_SUCCEED) node_γ = &lbl_γ;
        if (nodes[i]->op == IR_EVERY) { for (int k = 0; k < n; k++) if (nodes[k] == (IR_t *)(nodes[i]->n_operands > 0 ? nodes[i]->operands[0] : NULL)) { node_γ = lbls[k]; break; } }
        int omega_resolved = 0; int omega_k = -1;
        for (int k = 0; k < n; k++) if (nodes[k] == nodes[i]->ω.node) { node_ω = (i > k && ir_is_generator_kind(nodes[k]->op)) ? betas[k] : lbls[k]; omega_resolved = 1; omega_k = k; break; }
        if (!omega_resolved) node_ω = &lbl_ω;
        if (omega_resolved && nodes[i]->ω.node && nodes[i]->ω.node->op == IR_EVERY) {
            if (ir_is_generator_kind(nodes[i]->op)) { node_ω = lbls[omega_k]; int bk = to_inner_gen_operand_k(nodes[i], nodes, n); if (bk >= 0) node_ω = betas[bk]; }
            else { for (int gk = 0; gk < n; gk++) if (ir_is_generator_kind(nodes[gk]->op)) node_ω = betas[gk]; }
        }
        walk_bb_flat(nodes[i], node_γ, node_ω, betas[i]);
    }
    emit_label_define_bb(&lbl_β);
    emit_jmp_label(&lbl_ω, JMP_JMP);
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
    if (nd && nd->γ.node && nd->γ.node->op == IR_PAT_ALT) {
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
    if (nd->op == IR_PAT_ASSIGN_COND || nd->op == IR_PAT_ASSIGN_IMM) {
        IR_t *ch = (bb_match_nkids(nd) > 0) ? bb_match_kid(nd, 0) : (nd->n_operands > 0 ? nd->operands[0] : (IR_t*)0);
        IR_t *sc = ch;
        while (sc && sc != nd && is_pat_chain_elem(sc->op)) { pre_build_children_text(sc, out, base_prefix); sc = sc->γ.node; }
        return;
    }
    if (nd->op == IR_PAT_ARBNO || nd->op == IR_PAT_CALLOUT) {
        IR_t *ch = NULL;
        IR_graph_t *chg = NULL;
        if (nd->op == IR_PAT_ARBNO) {
            bb_arbno_state_t *az = (bb_arbno_state_t *)(intptr_t)IR_EXEC(nd).counter;
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
            codegen_flat_body(ch, child_prefix, 1, nd->op == IR_PAT_ARBNO ? 1 : 0);
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
    if (nd->op == IR_PAT_ASSIGN_COND || nd->op == IR_PAT_ASSIGN_IMM) {
        IR_t *ch = (bb_match_nkids(nd) > 0) ? bb_match_kid(nd, 0) : ((IR_t*)0);
        if (ch) pre_build_children(ch);
        return;
    }
    if (nd->op == IR_PAT_ARBNO || nd->op == IR_PAT_CALLOUT) {
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
    case IR_FIELD_GET: return 0;
    case IR_ALT:   return 0;
    case IR_GATHER: return 0;
    case IR_MAP: case IR_GREP: return 0;
    case IR_GEN_SCAN: return 0;
    case IR_BINOP: case IR_BINOP_GEN: case IR_TO: case IR_TO_BY: return 2;
    case IR_UNOP:  case IR_NEG: case IR_POS: case IR_NONNULL: case IR_NOT: case IR_SIZE: return 1;
    case IR_ASSIGN: case IR_ASSIGN_LIT_S: case IR_ASSIGN_LIT_I: case IR_ASSIGN_VAR: case IR_ASSIGN_CONCAT: case IR_ASSIGN_CALL: case IR_ASSIGN_FRAME: case IR_ASSIGN_FRAME_REF: return 1;
    case IR_RETURN: return 1;
    case IR_CALL_DEFINE: return 0;
    case IR_SCAN_POS: case IR_SCAN_ANY: case IR_SCAN_MATCH: case IR_SCAN_MANY: case IR_SCAN_TAB: case IR_SCAN_MOVE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL:
    case IR_CALL_PROC_STAGED: case IR_CALL_USERPROC: case IR_CALL_BYNAME: case IR_CALL_BUILTIN:
    case IR_CALL:  return (IR_LIT(n).dval == 2.0 || IR_LIT(n).dval == 3.0 || IR_LIT(n).dval == 5.0) ? 0 : (int)IR_LIT(n).ival;
    case IR_PATTERN_LIT: return 0;
    case IR_PATTERN_LEN: case IR_PATTERN_POS: case IR_PATTERN_RPOS: case IR_PATTERN_TAB: case IR_PATTERN_RTAB: return 0;
    case IR_PATTERN_ANY: case IR_PATTERN_NOTANY: case IR_PATTERN_SPAN: case IR_PATTERN_BREAK: case IR_PATTERN_BREAKX: return 0;
    case IR_PATTERN_FAIL: case IR_PATTERN_REM: case IR_PATTERN_SUCCEED: case IR_PATTERN_FENCE: case IR_PATTERN_ABORT: case IR_PATTERN_ARB: return 0;
    case IR_PATTERN_CAT: return 2;
    case IR_PATTERN_ALT: return 2;
    case IR_DTP_ASSIGN:  return 1;
    default:       return -1;
    }
}
static void descr_chain_operand_refs(IR_t *entry) {
    IR_t *chain[512]; int nc = 0;
    IR_t *seen[512]; int ns = 0;
    IR_t *stkv[512]; int sv = 0;
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL) && entry->γ.node && guard++ < 512) entry = entry->γ.node; }
    stkv[sv++] = entry;
    while (sv > 0 && nc < 512) {
        IR_t *c = stkv[--sv];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        int dup = 0; for (int i = 0; i < ns; i++) if (seen[i] == c) { dup = 1; break; }
        if (dup) continue;
        seen[ns++] = c; chain[nc++] = c;
        if ((c->op == IR_BINOP || c->op == IR_BINOP_GEN) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if ((c->op == IR_CALL || c->op == IR_CALL_DEFINE) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if (c->op == IR_GATHER && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if ((c->op == IR_MAP || c->op == IR_GREP) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if (c->γ.node && sv < 512) stkv[sv++] = c->γ.node;
    }
    IR_t *stk[512]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = descr_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        if (ar == 1 && n->op == IR_SCAN && sp >= 1) { scan_set_subj_node(n, stk[sp - 1]); sp -= 1; }
        else if (ar >= 1 && sp >= ar) { n->n_operands = 0; for (int k = ar; k >= 1; k--) ir_operand_push(n, stk[sp - k]); sp -= ar; }
        else if (ar >= 1) { sp = 0; }
        stk[sp++] = n;
    }
}
void resolve_call_kinds_descr(IR_graph_t *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) { IR_t *nd = g->all[i]; if (nd && nd->op == IR_CALL && IR_LIT(nd).dval == 3.0 && IR_LIT(nd).sval && IR_LIT(nd).sval[0] && rt_proc_is_registered(IR_LIT(nd).sval)) nd->op = IR_CALL_PROC_STAGED; }
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
        if (nd->op == IR_PAT_ARBNO) { pre_build_children_text(nd, out, prefix); continue; }
        if (nd->op != IR_SCAN) continue;
        IR_graph_t *pg = (IR_graph_t *)(intptr_t)IR_EXEC(nd).counter;
        if (!pg || !pg->all) continue;
        for (int j = 0; j < pg->n; j++) if (pg->all[j] && pg->all[j]->op == IR_PAT_ARBNO) pre_build_children_text(pg->all[j], out, prefix);
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
    enum { CH_MAX = 512 };
    IR_t *nodes[CH_MAX]; int n = 0;
    IR_t *queue[CH_MAX]; int qh = 0, qt = 0;
    IR_t *e0 = gvar_chain_resolve_stmt(entry);
    if (gvar_chain_is_real(e0)) queue[qt++] = e0;
    while (qh < qt) {
        IR_t *c = queue[qh++];
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= CH_MAX) { fprintf(stderr, "[SBB] FATAL sno chain exceeds CH_MAX\n"); abort(); }
        nodes[n++] = c;
        IR_t *g = gvar_chain_resolve_stmt(c->γ.node);
        IR_t *w = gvar_chain_resolve_stmt(c->ω.node);
        if (gvar_chain_is_real(g) && qt < CH_MAX) queue[qt++] = g;
        if (gvar_chain_is_real(w) && qt < CH_MAX) queue[qt++] = w;
    }
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * (n > 0 ? n : 1));
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * (n > 0 ? n : 1));
    int id = g_flat_node_id++;
    for (int i = 0; i < n; i++) {
        lbls[i]  = emit_label_alloc("snoch%d_n%d_α", id, i);
        betas[i] = emit_label_alloc("snoch%d_n%d_β", id, i);
    }
    for (int i = 0; i < n; i++) {
        emit_label_define_bb(lbls[i]);
        bb_label_t *node_γ = &lbl_γ;
        bb_label_t *node_ω = &lbl_ω;
        IR_t *g = gvar_chain_resolve_stmt(nodes[i]->γ.node);
        IR_t *w = gvar_chain_resolve_stmt(nodes[i]->ω.node);
        if (gvar_chain_is_real(g)) { for (int k = 0; k < n; k++) if (nodes[k] == g) { node_γ = lbls[k]; break; } }
        else if (g && g->op == IR_FAIL) node_γ = &lbl_ω;
        if (gvar_chain_is_real(w)) { for (int k = 0; k < n; k++) if (nodes[k] == w) { node_ω = lbls[k]; break; } }
        else if (w && w->op == IR_FAIL) node_ω = &lbl_ω;
        else if (w && w->op == IR_SUCCEED) node_ω = &lbl_γ;
        walk_bb_flat(nodes[i], node_γ, node_ω, betas[i]);
    }
    emit_label_define_bb(&lbl_β);
    emit_jmp_label(&lbl_ω, JMP_JMP);
    emit_label_define_bb(&lbl_γ);
    xa_dispatch(XA_FLAT_EPILOGUE);
    if (text_externalise && g_is_text) {
        data_buf_flush_pending_label();
        xa_dispatch(XA_FLAT_DATA_SECTION);
        data_buf_reset();
    }
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
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_gvar_flat_chain_body(g->entry, "flat");
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
        if (nd->op == IR_REF_INVARIANT || nd->op == IR_PAT_ARBNO) { has_ref = 1; break; }
        if (nd->op == IR_SCAN) { IR_graph_t *pg = (IR_graph_t *)(intptr_t)IR_EXEC(nd).counter; if (pg && pg->all) { for (int j = 0; j < pg->n; j++) if (pg->all[j] && pg->all[j]->op == IR_PAT_ARBNO) { has_ref = 1; break; } } }
    }
    if (has_ref) { g_child_cache_n = 0; g_text_child_counter = 0; gvar_chain_prebuild_children_text(g, out, prefix); }
    gvar_chain_operand_refs(g);
    g_flat_slot_count = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0; g_subject_slot = -1;
    if (g->nslots > 0) g_flat_slot_count = 16 + (g->nslots - 1) * 16;
    g_gvar_flat_chain = 1;
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    int rc = codegen_gvar_flat_chain_body(g->entry, prefix);
    emitter_end();
    g_gvar_flat_chain = 0;
    g_emit_cfg = save_cfg;
    return rc;
}
/*--------------------------------------------------------------------------------------------------------------------*/
bb_box_fn gvar_flat_chain_build_at(IR_graph_t *g, int entry_idx, const char *prefix) {
    if (!g) return NULL;
    IR_t *save_entry = g->entry;
    if (entry_idx >= 0 && entry_idx < g->n && g->all[entry_idx]) g->entry = g->all[entry_idx];
    bb_box_fn fn = gvar_flat_chain_build(g);
    g->entry = save_entry;
    (void)prefix;
    return fn;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int gvar_flat_chain_build_text_at(IR_graph_t *g, int entry_idx, FILE *out, const char *prefix) {
    if (!g) return 1;
    IR_t *save_entry = g->entry;
    if (entry_idx >= 0 && entry_idx < g->n && g->all[entry_idx]) g->entry = g->all[entry_idx];
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
int pl_catch_block_index(IR_t *nd) {
    for (int i = 0; i < g_pl_catch_n; i++) if (g_pl_catch_nodes[i] == nd) return i;
    return -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void walk_bb_register_child_label(IR_t *nd, const char *α_label) {
    bb_box_fn fn = child_cache_get(nd);
    if (fn) child_cache_set_lbl(fn, α_label);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void lower_flat_reset(void) { g_flat_slot_count = 0; g_flat_node_id = 0; }

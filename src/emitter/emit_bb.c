#include "emit_bb.h"
#include "BB_templates/bb_templates.h"
#include "emit_form.h"
#include "emit_templates.h"
#include "emit_globals.h"
#include "XA.h"
#include "../runtime/interp/gen.h"
#include "IR.h"
#include "../rt/rt.h"
#include "../processor/bb_build.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
extern const char * Σ;
extern int          Σlen;
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
extern void     * bb_dvar_bin_new           (const char *name);
void (*g_cap_fixup_cb)(void *cap_ptr, const char *child_α_label) = NULL;
const char *child_cache_get_lbl(bb_box_fn fn);
#define FLAT_BUF_MAX  (256 * 1024)
int g_flat_node_id   = 0;
static int g_flat_slot_count = 0;
/* GZ-3 (GROUND ZERO 3) node->slot-offset map. A box that produces a READ-WRITE result (e.g. the     */
/* stackless integer binop) claims a per-sequence frame slot via bb_slot_alloc and records it keyed by */
/* its IR_t*; a consumer box (write reading the binop's result) recovers the offset via bb_slot_get on */
/* its operand node. Both live in the ONE-REGISTER FRAME addressed by ζ=r12 ([r12+off]). Reset per     */
/* sequence alongside g_flat_slot_count in bb_build_flat/bb_build_brokered.                             */
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
int bb_slot_get(IR_t *nd) {
    for (int i = 0; i < g_bb_slotmap_n; i++) if (g_bb_slotmap[i].key == nd) return g_bb_slotmap[i].off;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Node-free per-sequence frame claim. A box whose RW slot is PRIVATE (no consumer recovers it by node */
/* key — e.g. a pattern element's own match-state counter) claims `bytes` of the ζ=r12 frame and gets  */
/* the offset, without entering the node-keyed slotmap. Same per-sequence counter as bb_slot_alloc, so */
/* offsets never collide; this keeps the box pBB-free (no IR_t* needed to allocate private scratch).   */
int bb_slot_claim(int bytes) {
    int off = g_flat_slot_count;
    g_flat_slot_count += bytes;
    return off;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* GZ-7 (GROUND ZERO 3) named-variable slot map. An Icon variable `x` is a NAMED storage location — the   */
/* x86 analog of test_sno_1.c's named box slots (`str_t POS0; ... seq = cat(seq, POS0)`). All references  */
/* to the SAME variable name resolve to ONE per-sequence frame slot [r12+off]: IR_ASSIGN(x) writes it,    */
/* IR_VAR(x) reads it. Keyed by name (not node ptr) precisely because distinct ASSIGN and VAR nodes must  */
/* share the slot. Lives in the SAME ζ=r12 frame as bb_slot_alloc's anonymous box slots; reset per        */
/* sequence alongside g_flat_slot_count. NO value stack, NO ring — the variable IS its slot.              */
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
/* GZ-7 flat-chain mode flag. When set (Icon multi-statement / variable-bearing graph emitted as a flat   */
/* goto-graph via icn_flat_chain_build), the per-box templates take their STACKLESS SLOT-LEAF arm: each   */
/* box reads its operands from the producer box's slot (bb_slot_get) or a named variable slot (bb_varslot */
/* _peek), computes, and writes its own result slot — exactly the test_sno_1.c named-slot model. Boxes    */
/* are NEVER re-walked for operands (postfix order guarantees operands precede consumers in the chain).   */
int g_icn_flat_chain = 0;
int g_sno_flat_chain = 0;  /* SBL-M3-CHAIN: SNOBOL4 flat-chain emit active — IR_VAR is a by-name pass-through (value read in bb_sno_assign_var) */
int g_frame_active = 0;
/* PB-RB-3 BB_MATCH (GOAL-SNOBOL4-BB CORRECTED PATTERN ARCHITECTURE, 2026-06-01). Cross-box emit-time values   */
/* threaded from the SUBJECT box (g_sno_subject_slot — its ζ-frame slot offset, set when bb_sno_subject emits  */
/* and read when bb_match emits, same flat sequence) and from flat_drive_match (the inline element     */
/* entry label + the match_advance handler label the template jumps to / defines). NOT a g_emit ABI change —   */
/* SNOBOL-lane dedicated globals (mirror the bb_child_lbl idiom).                                              */
int                 g_sno_subject_slot       = -1;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *child_cache_get_lbl(bb_box_fn fn) {
    for (int i = 0; i < g_child_cache_n; i++) if (g_child_cache[i].fn == fn && g_child_cache[i].text_lbl[0]) return g_child_cache[i].text_lbl;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void child_cache_put(IR_t *p, bb_box_fn fn) {
    if (g_child_cache_n < CHILD_CACHE_MAX) { g_child_cache[g_child_cache_n].key = p; g_child_cache[g_child_cache_n].fn = fn; g_child_cache[g_child_cache_n].text_lbl[0] = '\0'; g_child_cache_n++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void child_cache_set_lbl(bb_box_fn fn, const char *lbl) {
    for (int i = 0; i < g_child_cache_n; i++) if (g_child_cache[i].fn == fn) { snprintf(g_child_cache[i].text_lbl, 80, "%s", lbl ? lbl : ""); return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void data_buf_pend_label(const char *name) {
    if (g_flat_data_pending_lbl[0])
        data_buf_appendf("%s\n", g_flat_data_pending_lbl);
    snprintf(g_flat_data_pending_lbl, sizeof(g_flat_data_pending_lbl), "%s:", name ? name : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void data_buf_flush_pending_label(void) {
    if (!g_flat_data_pending_lbl[0]) return;
    data_buf_appendf("%s\n", g_flat_data_pending_lbl);
    g_flat_data_pending_lbl[0] = '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void data_buf_remember_label(const char *name) {
    if (g_flat_data_block_nlbls >= FLAT_DATA_LBL_MAX) return;
    snprintf(g_flat_data_block_lbls[g_flat_data_block_nlbls], sizeof(g_flat_data_block_lbls[0]), "%s", name ? name : "");
    g_flat_data_block_nlbls++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void data_buf_emit_block_comment(void) { g_flat_data_block_nlbls = 0; }
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
static void icn_chain_operand_refs(IR_t *entry);
extern int memcmp(const void *, const void *, size_t);
static bb_label_t g_α_ring[8];
static int        g_α_ring_i = 0;
static void bb_fill_alpha(IR_t *nd) {
    bb_label_t *a = &g_α_ring[g_α_ring_i++ & 7];
    emit_label_initf(a, "bb%d_α", nd ? bb_node_id(nd) : 0);
    g_emit.lbl_α   = a->name;
    g_emit.lbl_α_p = a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
static void flat_drive_cat(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int id = g_flat_node_id++;
    bb_label_t *mid_γ   = emit_label_alloc("xcat%d_γ",       id);
    bb_label_t *right_ω = emit_label_alloc("xcat%d_right_ω", id);
    bb_label_t *left_β  = emit_label_alloc("xcat%d_left_β",  id);
    bb_label_t *right_β = emit_label_alloc("xcat%d_right_β", id);
    bb_label_t *xcat_ω  = emit_label_alloc("xcat%d_ω",       id);
    EMIT_PAIR_RESET();
    if (!pBB || bb_pat_nkids(pBB) == 0) {
        EMIT_PAIR_JMP(lbl_γ);
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_DEF_JMP(xcat_ω, lbl_ω);
        EMIT_PAIR_DEF(mid_γ); EMIT_PAIR_DEF(right_ω); EMIT_PAIR_DEF(right_β); EMIT_PAIR_DEF(left_β);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    if (bb_pat_nkids(pBB) == 1) {
        walk_bb_flat(bb_pat_kid(pBB, 0), lbl_γ, lbl_ω, left_β);
        EMIT_PAIR_DEF_JMP(lbl_β, left_β);
        EMIT_PAIR_DEF_JMP(xcat_ω, lbl_ω);
        EMIT_PAIR_DEF(mid_γ); EMIT_PAIR_DEF(right_ω); EMIT_PAIR_DEF(right_β);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    walk_bb_flat(bb_pat_kid(pBB, 0), mid_γ, xcat_ω, left_β);
    emit_label_define_bb(mid_γ);
    bb_label_t *last_β = right_β;
    if (bb_pat_nkids(pBB) == 2) {
        walk_bb_flat(bb_pat_kid(pBB, 1), lbl_γ, right_ω, right_β);
    } else {
        int nc = bb_pat_nkids(pBB);
        bb_label_t **mids  = (bb_label_t **)alloca(sizeof(bb_label_t *) * (nc - 1));
        bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * (nc - 1));
        for (int i = 0; i < nc - 1; i++) {
            mids[i]  = emit_label_alloc("xcat%d_mid%d_γ", id, i+1);
            betas[i] = emit_label_alloc("xcat%d_mid%d_β", id, i+1);
        }
        for (int i = 1; i < nc; i++) {
            bb_label_t *s = (i < nc-1) ? mids[i-1] : lbl_γ;
            bb_label_t *kid_ω = (i == 1) ? left_β : betas[i-2];
            walk_bb_flat(bb_pat_kid(pBB, i), s, kid_ω, betas[i-1]);
            if (i < nc-1) emit_label_define_bb(mids[i-1]);
        }
        last_β = betas[nc-2];
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(right_ω, left_β);
    EMIT_PAIR_DEF_JMP(lbl_β, last_β);
    EMIT_PAIR_DEF_JMP(xcat_ω, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_alt(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int id = g_flat_node_id++;
    int nc = pBB ? bb_pat_nkids(pBB) : 0;
    EMIT_PAIR_RESET();
    if (nc == 0) { EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); return; }
    if (nc == 1) { walk_bb_flat(bb_pat_kid(pBB, 0), lbl_γ, lbl_ω, lbl_β); return; }
    bb_label_t **ci_βs = (bb_label_t **)alloca((size_t)nc * sizeof(bb_label_t *));
    bb_label_t **ci_ωs = (bb_label_t **)alloca((size_t)nc * sizeof(bb_label_t *));
    for (int i = 0; i < nc; i++) {
        ci_βs[i] = emit_label_alloc("alt%d_c%d_β", id, i);
        ci_ωs[i] = emit_label_alloc("alt%d_c%d_ω", id, i);
    }
    for (int i = 0; i < nc; i++) {
        bb_label_t *f = (i < nc-1) ? ci_ωs[i] : ci_ωs[nc-1];
        walk_bb_flat(bb_pat_kid(pBB, i), lbl_γ, f, ci_βs[i]);
        if (i < nc-1) emit_label_define_bb(ci_ωs[i]);
        else          emit_label_define_bb(ci_ωs[nc-1]);
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_JMP(lbl_ω);
    EMIT_PAIR_DEF_JMP(lbl_β, ci_βs[0]);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_fence(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || bb_pat_nkids(pBB) == 0) {
        EMIT_PAIR_RESET();
        EMIT_PAIR_JMP(lbl_γ);
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    int id = g_flat_node_id++;
    bb_label_t *child_γ = emit_label_alloc("xfnce%d_γ", id);
    bb_label_t *child_ω = emit_label_alloc("xfnce%d_ω", id);
    walk_bb_flat(bb_pat_kid(pBB, 0), child_γ, child_ω, lbl_β);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(child_γ, lbl_γ);
    EMIT_PAIR_DEF_JMP(child_ω, lbl_ω);
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int resolve_seq_goals_em(const IR_t *nd, IR_t **out, int max) {
    if (!nd || nd->t != IR_GCONJ) return 0;
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)nd->ival;
    if (!zs || !zs->goals) return 0;
    int k = zs->ngoals < max ? zs->ngoals : max;
    for (int i = 0; i < k; i++) out[i] = zs->goals[i];
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int resolve_choice_bodies_em(const IR_t *nd, IR_t **out, int max) {
    if (!nd || nd->t != IR_CHOICE) return 0;
    bb_choice_state_t *zc = (bb_choice_state_t *)(intptr_t)nd->ival;
    if (!zc || !zc->bodies) return 0;
    int k = zc->nbodies < max ? zc->nbodies : max;
    /* PLG-9d-bt: enter each clause at its body_root (the body's top GCONJ) so a multi-goal clause body    */
    /* emits ALL its goals; ->entry is only the first goal's α (walking it alone under-emits). For a       */
    /* single-element fact body, body_root is a GCONJ(n=1) whose sole goal is that element -> byte-        */
    /* identical to the prior ->entry walk. Fall back to ->entry if body_root was never recorded.          */
    for (int i = 0; i < k; i++)
        out[i] = zc->bodies[i] ? (zc->bodies[i]->body_root ? zc->bodies[i]->body_root : zc->bodies[i]->entry) : NULL;
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_pl_seq(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
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
        IR_e t0 = goals[0]->t;
        int r0 = (t0==IR_GOAL || t0==IR_CHOICE || t0==IR_DISJ);
        eff_β[0] = r0 ? gβ[0] : lbl_ω;
        for (int i = 1; i < n; i++) {
            IR_e ti = goals[i]->t;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void resolve_choice_clause_label(char *dst, size_t dsz, int id, int ci, const char *suffix) {
    snprintf(dst, dsz, ".Lplch%d_c%d_%s", id, ci, suffix);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_pl_choice(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_pl_alt(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    /* PLG-9d-bt: a Prolog `;` disjunction's arms (their PRINCIPAL nodes) live in the operand_aux sidecar  */
    /* of the owning graph — NOT in pBB->α/β (the old read saw only one arm and silently dropped the rest).*/
    /* Walk each arm principal via walk_bb_flat (a conjunction arm's GCONJ -> flat_drive_pl_seq expands    */
    /* its goals). Arm i's success flows to lbl_γ (the disjunction's continuation); arm i's failure flows   */
    /* to the NEXT arm's `pre` (the bb_disj template's pre does rt_pl_trail_unwind_top then jmps that arm's */
    /* body), the last arm's failure to lbl_ω. The α/pre/β scaffold + trail mark is emitted by bb_disj.     */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int resolve_ite_entries_em(const IR_t *nd, IR_t **out_cond, IR_t **out_then, IR_t **out_else) {
    if (!nd || nd->t != IR_ITE) return 0;
    bb_ite_state_t *zi = (bb_ite_state_t *)(intptr_t)nd->ival;
    if (!zi) return 0;
    if (out_cond) *out_cond = zi->cond;
    if (out_then) *out_then = zi->then_;
    if (out_else) *out_else = zi->else_;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PLG-9f (2026-06-01): a branch of an ITE is walked by its PRINCIPAL (wrapper) node, not its entry[0]. For a  */
/* multi-goal branch (a `,`-spine lowered via wire_seq) the principal is the IR_GCONJ wrapper that walk_bb_flat */
/* dispatches to flat_drive_pl_seq, expanding every goal; entry[0] is only the first goal, so walking it alone  */
/* dropped g2..gn (the bug). For a single-goal branch principal==entry, so this is a no-op there. The branch    */
/* root is preferred only when it is a driver-owned kind (IR_GCONJ) whose dispatch needs the wrapper; otherwise */
/* the entry node is the correct walk start (its FILL emits the single box). Falls back to entry if root NULL.  */
static IR_t *ite_branch_walk_node(IR_t *entry, IR_t *root) {
    if (root && bb_kind_is_driver_owned(root->t)) return root;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_pl_ite(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *cond = NULL, *thn = NULL, *els = NULL;
    if (!resolve_ite_entries_em(pBB, &cond, &thn, &els) || !cond) {
        EMIT_PAIR_RESET(); EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); return;
    }
    /* PLG-9f: prefer the branch PRINCIPAL (wrapper) node over entry[0] for cond/then/else, so a multi-goal    */
    /* branch (IR_GCONJ) dispatches to flat_drive_pl_seq and emits every goal. Single-goal branch -> no-op.    */
    bb_ite_state_t *zi = (bb_ite_state_t *)(intptr_t)pBB->ival;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sub_label(char *dst, size_t dsz, const char *name) {
    char san[128]; size_t j = 0;
    for (size_t i = 0; name && name[i] && j + 1 < sizeof san; i++) {
        char c = name[i];
        san[j++] = ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') ? c : '_';
    }
    san[j] = '\0';
    snprintf(dst, dsz, ".Lrksub_%s", san);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int resolve_emit_callee_block_body(const char *name, int arity, bb_label_t *bγ, bb_label_t *bω, bb_label_t *bβ) {
    IR_t *pentry = resolve_bb_entry_node(name, arity);
    if (!pentry) return 0;
    walk_bb_flat(pentry, bγ, bω, bβ);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_kind_is_driver_owned(int t) {
    return t == IR_PAT_CAT || t == IR_PAT_ALT || t == IR_PAT_FENCE || t == IR_GCONJ;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_prepare_capture_arbno(IR_t *nd, int imm) {
    if (!PLATFORM_X86) return;
    bb_box_fn   child_fn = (bb_box_fn)g_emit.child_fn;
    const char *name     = g_emit.op_name1;
    const char *fnc_name = (g_emit.op_name2 && g_emit.op_name2[0]) ? g_emit.op_name2 : NULL;
    g_emit.bb_rt_obj   = NULL;
    g_emit.bb_child_lbl = NULL;
    g_emit.bb_child_fn  = (void *)child_fn;
    if (nd->t == IR_PAT_ARBNO) {
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_prepare_pl(IR_t *nd) {
    if (!PLATFORM_X86) return;
    g_emit.bb_ls = NULL;
    g_emit.bb_rs = NULL;
    g_emit.bb_op_lbl = NULL;
    g_emit.bb_lk = -1;
    if (nd->t == IR_ATOM) {
        g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, nd->sval ? nd->sval : "");
        return;
    }
    if (nd->t == IR_ARITH) {
        if (!nd->α || !nd->β) return;
        g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, nd->α->sval);
        g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, nd->β->sval);
        g_emit.bb_op_lbl = bb_intern_into(g_emit.bb_op_buf, nd->sval ? nd->sval : "+");
        g_emit.bb_lk = (int)nd->α->t; g_emit.bb_li = (int64_t)nd->α->ival;
        g_emit.bb_rk = (int)nd->β->t; g_emit.bb_ri = (int64_t)nd->β->ival;
        return;
    }
    if (nd->t == IR_UNIFY) {
        if (!nd->α || !nd->β) return;
        if (nd->α->t == IR_LOGICVAR && nd->β->t == IR_ATOM) {
            g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, nd->β->sval ? nd->β->sval : "");
            return;
        }
        if (nd->α->t == IR_LOGICVAR && nd->β->t == IR_LOGICVAR) return;
        g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, nd->α->sval);
        g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, nd->β->sval);
        return;
    }
    if (nd->t == IR_BUILTIN) {
        if (nd->α && nd->α->t == IR_ATOM)
            g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, nd->α->sval ? nd->α->sval : "");
        if (nd->sval && strcmp(nd->sval, "is") == 0 && nd->β && nd->β->t == IR_ARITH)
            g_emit.bb_op_lbl = bb_intern_into(g_emit.bb_op_buf, nd->β->sval ? nd->β->sval : "+");
        if (nd->sval && strcmp(nd->sval, "is") == 0 && nd->β && nd->β->t == IR_ATOM)
            g_emit.bb_op_lbl = bb_intern_into(g_emit.bb_op_buf, nd->β->sval ? nd->β->sval : "");
        return;
    }
    if (nd->t == IR_GOAL) {
        g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, nd->sval ? nd->sval : "");
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bb_label_t *seq_node_label(IR_t **nodes, bb_label_t **lbls, int n, IR_t *tgt, bb_label_t *falloff) {
    if (!tgt) return falloff;
    for (int i = 0; i < n; i++) if (nodes[i] == tgt) return lbls[i];
    return falloff;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_seq(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *first = pBB ? pBB->α : NULL;
    if (!first) {
        EMIT_PAIR_RESET();
        EMIT_PAIR_JMP(lbl_γ);
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    enum { SEQ_MAX = 512 };
    IR_t *nodes[SEQ_MAX];
    int n = 0;
    IR_t *queue[SEQ_MAX];
    int qh = 0, qt = 0;
    queue[qt++] = first;
    while (qh < qt) {
        IR_t *c = queue[qh++];
        if (!c) continue;
        int seen = 0;
        for (int i = 0; i < n; i++) if (nodes[i] == c) { seen = 1; break; }
        if (seen) continue;
        if (n >= SEQ_MAX) {
            fprintf(stderr, "[IBB] FATAL flat_drive_seq: CFG exceeds SEQ_MAX nodes\n");
            abort();
        }
        nodes[n++] = c;
        if (c->t == IR_SUSPEND) {
            fprintf(stderr, "[IBB] FATAL flat_drive_seq: gather-multi-yield (IR_SUSPEND child) not yet flat-wired\n");
            abort();
        }
        if (c->γ && qt < SEQ_MAX) queue[qt++] = c->γ;
        if (c->t == IR_IF && c->ω && qt < SEQ_MAX) queue[qt++] = c->ω;
    }
    int id = g_flat_node_id++;
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    for (int i = 0; i < n; i++) {
        lbls[i]  = emit_label_alloc("xseq%d_n%d_α", id, i);
        betas[i] = emit_label_alloc("xseq%d_n%d_β", id, i);
    }
    for (int i = 0; i < n; i++) {
        emit_label_define_bb(lbls[i]);
        bb_label_t *node_γ = seq_node_label(nodes, lbls, n, nodes[i]->γ, lbl_γ);
        bb_label_t *node_ω = (nodes[i]->t == IR_IF)
            ? seq_node_label(nodes, lbls, n, nodes[i]->ω, lbl_γ)
            : lbl_ω;
        walk_bb_flat(nodes[i], node_γ, node_ω, betas[i]);
    }
    emit_label_define_bb(lbl_β);
    emit_jmp_label(lbl_ω, JMP_JMP);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_binop_tree(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α || !pBB->β) {
        fprintf(stderr, "[IBB] FATAL flat_drive_binop_tree: missing α or β child\n");
        abort();
    }
    if ((pBB->α->t == IR_LIT_I && pBB->β->t == IR_LIT_I)
        || (pBB->α->t == IR_LIT_S && pBB->β->t == IR_LIT_S && pBB->ival == BINOP_CONCAT)) {
        /* GZ-3 (GROUND ZERO 3): both operands are READ-ONLY constants. The stackless binop box bakes their */
        /* values as sealed RO data in its OWN blob and reads them [rip+disp] directly, computing into its  */
        /* frame slot [r12+off]. The operands are therefore NOT walked as separate pushing boxes (there is  */
        /* no value stack). Emit only the binop box. RK-EMIT-1 (2026-05-31): the IR_LIT_S+CONCAT case is the */
        /* string analog — bb_binop.cpp's GZ-4 concat arm bakes both string literals inline from pBB->α/β   */
        /* ->sval, so re-walking them here would re-emit their bb<id>_α labels (a duplicate-symbol assembler */
        /* error, since the γ-chain already emitted each literal box once). Short-circuit it the same way.   */
        EMIT_PAIR_RESET();
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    int id = g_flat_node_id++;
    bb_label_t *lhs_done = emit_label_alloc("xbinop%d_lhs_done", id);
    bb_label_t *rhs_done = emit_label_alloc("xbinop%d_rhs_done", id);
    bb_label_t *lhs_β    = emit_label_alloc("xbinop%d_lhs_b",    id);
    bb_label_t *rhs_β    = emit_label_alloc("xbinop%d_rhs_b",    id);
    if (pBB->α->t == IR_ASSIGN && pBB->α->α && pBB->α->α->t == IR_VAR) {
        bb_label_t *lhs_stored = emit_label_alloc("xbinop%d_lhs_st", id);
        walk_bb_flat(pBB->α, lhs_stored, lbl_ω, lhs_β);
        emit_label_define_bb(lhs_stored);
        walk_bb_flat(pBB->α->α, lhs_done, lbl_ω, lhs_β);
    } else {
        walk_bb_flat(pBB->α, lhs_done, lbl_ω, lhs_β);
    }
    emit_label_define_bb(lhs_done);
    if (pBB->β->t == IR_ASSIGN && pBB->β->α && pBB->β->α->t == IR_VAR) {
        bb_label_t *rhs_stored = emit_label_alloc("xbinop%d_rhs_st", id);
        walk_bb_flat(pBB->β, rhs_stored, lbl_ω, rhs_β);
        emit_label_define_bb(rhs_stored);
        walk_bb_flat(pBB->β->α, rhs_done, lbl_ω, rhs_β);
    } else {
        walk_bb_flat(pBB->β, rhs_done, lbl_ω, rhs_β);
    }
    emit_label_define_bb(rhs_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int binop_operand_streams(IR_t *e) {
    if (!e) return 0;
    if (e->t == IR_ASSIGN) return binop_operand_streams(e->β);
    switch (e->t) {
        case IR_ALT: case IR_TO: case IR_TO_BY: case IR_BINOP_GEN: case IR_ITERATE:
        case IR_LIMIT: case IR_PROC_GEN: case IR_LIST_BANG: case IR_KEY_GEN:
        case IR_FIND_GEN: case IR_SEQ_GEN: case IR_SUSPEND: case IR_REPEAT:
            return 1;
        case IR_CALL:
            if (e->sval && rt_proc_is_registered(e->sval)) return 0;
            return 1;
        default:
            return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_binop_gen_tree(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α || !pBB->β) {
        fprintf(stderr, "[IBB] FATAL flat_drive_binop_gen_tree: missing α or β child\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *lhs_store  = emit_label_alloc("xbgen%d_lhs_store",  id);
    bb_label_t *lhs_seeded = emit_label_alloc("xbgen%d_lhs_seeded", id);
    bb_label_t *lhs_β      = emit_label_alloc("xbgen%d_lhs_β",      id);
    bb_label_t *rhs_store  = emit_label_alloc("xbgen%d_rhs_store",  id);
    bb_label_t *rhs_β      = emit_label_alloc("xbgen%d_rhs_β",      id);
    walk_bb_flat(pBB->α, lhs_store, lbl_ω, lhs_β);
    emit_label_define_bb(lhs_seeded);
    walk_bb_flat(pBB->β, rhs_store, lhs_β, rhs_β);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lhs_store,  lhs_seeded);
    EMIT_PAIR_DEF_JMP(rhs_store,  rhs_store);
    EMIT_PAIR_JMP(lbl_γ);
    EMIT_PAIR_JMP(rhs_β);
    EMIT_PAIR_DEF_JMP(lbl_β, rhs_β);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_call_intexpr(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α) {
        fprintf(stderr, "[IBB] FATAL flat_drive_call_intexpr: missing arg0\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *arg_done = emit_label_alloc("xcall%d_arg_done", id);
    bb_label_t *arg_β    = emit_label_alloc("xcall%d_arg_β",    id);
    if (pBB->dval != 1.0) {
        walk_bb_flat(pBB->α, arg_done, lbl_ω, arg_β);
        emit_label_define_bb(arg_done);
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, (pBB->dval == 1.0) ? lbl_ω : arg_β);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_unop(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α) {
        fprintf(stderr, "[IBB] FATAL flat_drive_unop: missing operand (α)\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *arg_done = emit_label_alloc("xunop%d_arg_done", id);
    bb_label_t *arg_β    = emit_label_alloc("xunop%d_arg_β",    id);
    walk_bb_flat(pBB->α, arg_done, lbl_ω, arg_β);
    emit_label_define_bb(arg_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_list_bang(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α) {
        fprintf(stderr, "[IBB] FATAL flat_drive_list_bang: missing iterable (α)\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *iter_done = emit_label_alloc("xbang%d_iter_done", id);
    bb_label_t *iter_β    = emit_label_alloc("xbang%d_iter_β",    id);
    walk_bb_flat(pBB->α, iter_done, lbl_ω, iter_β);
    emit_label_define_bb(iter_done);
    FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_field_get(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α || !pBB->sval) {
        fprintf(stderr, "[IBB] FATAL flat_drive_field_get: IR_FIELD_GET needs α (object) and sval (field)\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *obj_done = emit_label_alloc("xfget%d_obj_done", id);
    bb_label_t *obj_β    = emit_label_alloc("xfget%d_obj_β",    id);
    walk_bb_flat(pBB->α, obj_done, lbl_ω, obj_β);
    emit_label_define_bb(obj_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_field_set(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α || !pBB->β || !pBB->sval) {
        fprintf(stderr, "[IBB] FATAL flat_drive_field_set: IR_FIELD_SET needs α (object), β (rhs), sval (field)\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *rhs_done = emit_label_alloc("xfset%d_rhs_done", id);
    bb_label_t *rhs_β    = emit_label_alloc("xfset%d_rhs_β",    id);
    bb_label_t *obj_done = emit_label_alloc("xfset%d_obj_done", id);
    bb_label_t *obj_β    = emit_label_alloc("xfset%d_obj_β",    id);
    walk_bb_flat(pBB->β, rhs_done, lbl_ω, rhs_β);
    emit_label_define_bb(rhs_done);
    walk_bb_flat(pBB->α, obj_done, lbl_ω, obj_β);
    emit_label_define_bb(obj_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_idx_get(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (pBB && pBB->α && pBB->β) {
        int id = g_flat_node_id++;
        bb_label_t *base_done = emit_label_alloc("xidx%d_base_done", id);
        bb_label_t *base_β    = emit_label_alloc("xidx%d_base_b",    id);
        bb_label_t *idx_done  = emit_label_alloc("xidx%d_idx_done",  id);
        bb_label_t *idx_β     = emit_label_alloc("xidx%d_idx_b",     id);
        walk_bb_flat(pBB->α, base_done, lbl_ω, base_β);
        emit_label_define_bb(base_done);
        walk_bb_flat(pBB->β, idx_done, lbl_ω, idx_β);
        emit_label_define_bb(idx_done);
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_idx_set(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (pBB && pBB->α && pBB->β) {
        int id = g_flat_node_id++;
        IR_t *rhs_box = pBB->β ? pBB->β->γ : NULL;
        bb_label_t *base_done = emit_label_alloc("xidxs%d_base_done", id);
        bb_label_t *base_β    = emit_label_alloc("xidxs%d_base_b",    id);
        bb_label_t *idx_done  = emit_label_alloc("xidxs%d_idx_done",  id);
        bb_label_t *idx_β     = emit_label_alloc("xidxs%d_idx_b",     id);
        walk_bb_flat(pBB->α, base_done, lbl_ω, base_β);
        emit_label_define_bb(base_done);
        walk_bb_flat(pBB->β, idx_done, lbl_ω, idx_β);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_initial(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α) {
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
    walk_bb_flat(pBB->α, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_case(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α) {
        emit_label_define_bb(lbl_β);
        emit_jmp_label(lbl_γ, JMP_JMP);
        emit_jmp_label(lbl_ω, JMP_JMP);
        return;
    }
    IR_t *chain[256];
    int nchain = 0;
    for (IR_t *c = pBB->α->γ; c && nchain < 256; c = c->γ) chain[nchain++] = c;
    int npair      = nchain / 2;
    int has_default = (nchain & 1) != 0;
    int id = g_flat_node_id++;
    bb_label_t *sel_done = emit_label_alloc("xcase%d_sel_done", id);
    bb_label_t *sel_β    = emit_label_alloc("xcase%d_sel_β",    id);
    bb_label_t **val_entry = (bb_label_t **)alloca((size_t)(npair > 0 ? npair : 1) * sizeof(bb_label_t *));
    for (int i = 0; i < npair; i++) val_entry[i] = emit_label_alloc("xcase%d_val%d", id, i);
    walk_bb_flat(pBB->α, sel_done, lbl_ω, sel_β);
    emit_label_define_bb(sel_done);
    bb_case_store(pBB);
    for (int i = 0; i < npair; i++) {
        bb_label_t *key_done = emit_label_alloc("xcase%d_key%d_done", id, i);
        bb_label_t *key_β    = emit_label_alloc("xcase%d_key%d_β",    id, i);
        walk_bb_flat(chain[2*i], key_done, lbl_ω, key_β);
        emit_label_define_bb(key_done);
        EMIT_PAIR_RESET();
        EMIT_PAIR_JMP(val_entry[i]);
        bb_case_gate(pBB);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_limit(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α || !pBB->β) {
        fprintf(stderr, "[IBB] FATAL flat_drive_limit: IR_LIMIT requires α (generator) and β (count expr)\n");
        abort();
    }
    {
        IR_t *g = pBB->α;
        while (g && g->t == IR_ASSIGN) g = g->β;
        int ok = g && (g->t == IR_TO || g->t == IR_TO_BY || g->t == IR_ALT ||
                       g->t == IR_BINOP_GEN || g->t == IR_LIST_BANG);
        if (!ok) {
            fprintf(stderr, "[IBB] FATAL flat_drive_limit: generator kind=%d has no mode-3 two-port emission (limit deferred)\n",
                    (int)(g ? g->t : -1));
            abort();
        }
    }
    int id = g_flat_node_id++;
    bb_label_t *count_done = emit_label_alloc("xlimit%d_count_done", id);
    bb_label_t *count_β    = emit_label_alloc("xlimit%d_count_b",    id);
    bb_label_t *got_value  = emit_label_alloc("xlimit%d_got",        id);
    bb_label_t *gen_resume = emit_label_alloc("xlimit%d_gen_resume", id);
    walk_bb_flat(pBB->β, count_done, lbl_ω, count_β);
    emit_label_define_bb(count_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_JMP(lbl_ω);
    bb_limit_begin(pBB);
    walk_bb_flat(pBB->α, got_value, lbl_ω, gen_resume);
    emit_label_define_bb(got_value);
    EMIT_PAIR_RESET();
    EMIT_PAIR_JMP(lbl_γ);
    bb_limit_inc(pBB);
    emit_label_define_bb(lbl_β);
    EMIT_PAIR_RESET();
    EMIT_PAIR_JMP(lbl_ω);
    EMIT_PAIR_JMP(gen_resume);
    bb_limit_more(pBB);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_return(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    (void)lbl_γ;
    bb_label_t *slab_exit = g_emit.flat_succ_p ? g_emit.flat_succ_p : lbl_γ;
    if (pBB && pBB->α) {
        int id = g_flat_node_id++;
        bb_label_t *expr_done = emit_label_alloc("xreturn%d_expr_done", id);
        bb_label_t *expr_β    = emit_label_alloc("xreturn%d_expr_β",    id);
        walk_bb_flat(pBB->α, expr_done, lbl_ω, expr_β);
        emit_label_define_bb(expr_done);
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, slab_exit, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_call_userproc(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int nargs = (int)(pBB ? pBB->ival : 0);
    IR_t *ax = pBB ? pBB->α : NULL;
    bb_label_t *prev_done = NULL;
    for (int j = 0; j < nargs && ax; j++) {
        int id = g_flat_node_id++;
        bb_label_t *arg_done = emit_label_alloc("xupcall%d_arg_done", id);
        bb_label_t *arg_β    = emit_label_alloc("xupcall%d_arg_β",    id);
        if (prev_done) emit_label_define_bb(prev_done);
        walk_bb_flat(ax, arg_done, lbl_ω, arg_β);
        prev_done = arg_done;
        ax = ax->γ;
    }
    if (prev_done) emit_label_define_bb(prev_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* GZ-10 (modes 3/4) STACKLESS Icon user-procedure call WITH ARGS. The dval==3.0 IR_CALL carries its       */
/* arguments as ISOLATED value sub-graphs in the IR_graph_t* array on pBB->counter (lower.c TT_FNC arm),    */
/* NOT on the γ-chain — exactly the SNOBOL4/Raku isolated-subgraph idiom. For each arg i this DRIVER walks  */
/* that sub-graph inline as a flat chain (so each producer box is emitted ONCE and claims its own ζ=r12     */
/* result slot via bb_slot_alloc16); the sub-graph's TERMINAL producer (the node lower_value_subgraph left  */
/* with γ==NULL) holds the arg value in its slot. The bb_call template (dval==3.0 arm) then reads each      */
/* terminal-producer slot and emits `mov edi,i; mov rsi,[r12+slot]; mov rdx,[r12+slot+8]; call             */
/* rt_icn_arg_stage` before the rt_icn_call_proc_descr call — NO value stack, NO ring (the staging buffer   */
/* g_icn_call_args is a transient single-call vector consumed by the helper on entry, the no-value-stack    */
/* FACT RULE's permitted per-activation marshalling). Each arg sub-graph runs bounded: its success falls    */
/* through to the next arg (arg_done chaining, like flat_drive_call_userproc), its failure routes to lbl_ω  */
/* (a failing argument fails the whole call — jcon ir_a_Call's L[1].ir.failure -> p.ir.failure).            */
/* GZ-10 (modes 3/4): emit ONE Icon argument value sub-graph as a complete inline flat chain. The arg      */
/* sub-graph (lower_value_subgraph) is a postfix γ-chain of producer boxes terminating in a node with      */
/* γ==NULL (its slot is the arg's value). walk_bb_flat on a single node only emits THAT node, so a          */
/* multi-node arg like `n - 1` (VAR→LIT→BINOP) needs the SAME BFS-over-(γ + BINOP.ω) chain walk            */
/* codegen_flat_chain_body uses for a proc body — every box emitted once, wired by its native γ/ω ports,    */
/* each operand/consumer reading the producer's ζ slot. succ = where the chain's value flows when done      */
/* (the next arg's entry, or the call box); fail = lbl_ω (a failing argument fails the whole call, jcon     */
/* ir_a_Call L[1].ir.failure -> p.ir.failure). The caller runs icn_chain_operand_refs(entry) first so the   */
/* binop/relop operand refs (α/β) are set before this walk allocates and reads their slots.                 */
static void flat_emit_arg_subchain(IR_t *entry, bb_label_t *succ, bb_label_t *fail) {
    enum { CH_MAX = 512 };
    IR_t *nodes[CH_MAX]; int n = 0;
    IR_t *queue[CH_MAX]; int qh = 0, qt = 0;
    queue[qt++] = entry;
    while (qh < qt) {
        IR_t *c = queue[qh++];
        if (!c || c->t == IR_SUCCEED || c->t == IR_FAIL) continue;
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= CH_MAX) { fprintf(stderr, "[GZ-10] FATAL arg subchain exceeds CH_MAX\n"); abort(); }
        nodes[n++] = c;
        if (c->γ && qt < CH_MAX) queue[qt++] = c->γ;
        if ((c->t == IR_BINOP || c->t == IR_BINOP_GEN) && c->ω && qt < CH_MAX) queue[qt++] = c->ω;
    }
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
        for (int k = 0; k < n; k++) if (nodes[k] == nodes[i]->γ) { node_γ = lbls[k]; break; }
        if (nodes[i]->γ == NULL || nodes[i]->γ->t == IR_SUCCEED) node_γ = succ;
        int omega_resolved = 0;
        for (int k = 0; k < n; k++) if (nodes[k] == nodes[i]->ω) { node_ω = lbls[k]; omega_resolved = 1; break; }
        if (!omega_resolved) node_ω = fail;
        walk_bb_flat(nodes[i], node_γ, node_ω, betas[i]);
    }
}
static void flat_drive_icn_userproc(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int nargs = (int)(pBB ? pBB->ival : 0);
    IR_graph_t **blks = pBB ? (IR_graph_t **)(intptr_t) pBB->counter : NULL;
    bb_label_t *prev_done = NULL;
    for (int i = 0; i < nargs && blks; i++) {
        IR_t *aentry = blks[i] ? blks[i]->entry : NULL;
        if (!aentry) continue;
        int id = g_flat_node_id++;
        bb_label_t *arg_done = emit_label_alloc("xicnarg%d_done", id);
        if (prev_done) emit_label_define_bb(prev_done);
        icn_chain_operand_refs(aentry);
        flat_emit_arg_subchain(aentry, arg_done, lbl_ω);
        prev_done = arg_done;
    }
    if (prev_done) emit_label_define_bb(prev_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gen_bb_is_gen_arg(IR_t *e) {
    if (!e) return 0;
    if (e->t == IR_ASSIGN) return gen_bb_is_gen_arg(e->β);
    switch (e->t) {
        case IR_TO: case IR_TO_BY: case IR_UPTO: case IR_ALT:
        case IR_BINOP_GEN: case IR_ITERATE: case IR_LIMIT: case IR_PROC_GEN:
        case IR_LIST_BANG: case IR_KEY_GEN: case IR_FIND_GEN: case IR_SEQ_GEN:
        case IR_GATHER:
            return 1;
        default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int call_args_single_shot(IR_t *pBB) {
    int nargs = (int)(pBB ? pBB->ival : 0);
    IR_t *ax = pBB ? pBB->α : NULL;
    for (int j = 0; j < nargs && ax; j++, ax = ax->γ)
        if (gen_bb_is_gen_arg(ax)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_call_builtin(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int nargs = (int)(pBB ? pBB->ival : 0);
    IR_t *ax = pBB ? pBB->α : NULL;
    bb_label_t *prev_done = NULL;
    for (int j = 0; j < nargs && ax; j++) {
        int id = g_flat_node_id++;
        bb_label_t *arg_done = emit_label_alloc("xbicall%d_arg_done", id);
        bb_label_t *arg_β    = emit_label_alloc("xbicall%d_arg_β",    id);
        if (prev_done) emit_label_define_bb(prev_done);
        walk_bb_flat(ax, arg_done, lbl_ω, arg_β);
        prev_done = arg_done;
        ax = ax->γ;
    }
    if (prev_done) emit_label_define_bb(prev_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_sno_assign(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    /* SBL-M3-STACKLESS: SNOBOL4 `name = 'literal'` — single-shot bounded, NO value stack. The rhs literal
       and target name are baked as RO immediates inside bb_sno_assign (reached via emit_core IR_ASSIGN
       branch when α==IR_LIT_S); no operand subtree to walk, β = jmp ω. */
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_sno_assign_binop(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    /* SBL-M3-ARITH (2026-05-31): SNOBOL4 `name = lit op lit`. Mirrors flat_drive_call_intexpr: walk the rhs
       binop FIRST (it allocates its ζ-frame result slot via bb_slot_alloc during emission), then emit the
       assign box, which recovers the slot offset via bb_slot_get(rhs) and reads [r12+off]. Bounded single-
       shot, NO value stack. β of the assign jumps ω (an assign does not resume). */
    if (!pBB || !pBB->α || pBB->α->t != IR_BINOP) {
        fprintf(stderr, "[SBB] FATAL flat_drive_sno_assign_binop: rhs is not IR_BINOP\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *rhs_done = emit_label_alloc("xsasg%d_rhs_done", id);
    bb_label_t *rhs_β    = emit_label_alloc("xsasg%d_rhs_β",    id);
    walk_bb_flat(pBB->α, rhs_done, lbl_ω, rhs_β);
    emit_label_define_bb(rhs_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_sno_scan(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    /* SBL-M3-SCAN (2026-05-31): SNOBOL4 pattern-match statement — single-shot bounded, NO value stack. The
       subject name, replacement literal, and pattern sub-graph pointer are baked as RO immediates inside
       bb_sno_scan (reached via emit_core IR_SCAN branch); the box tests rt_sno_exec_scan's result and jmps
       γ (match) or ω (fail). No operand subtree to walk here (the replacement/subject literal are folded
       onto pBB->α and consumed inside the box); β = jmp ω. */
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_sno_subject(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    /* PB-0 SUBJECT phase (GOAL-SNOBOL4-BB SBL-PAT-BB, 2026-05-31). SNOBOL4 match-statement SUBJECT box —
       single-shot bounded, NO value stack. The subject variable name OR literal is baked RO inside
       bb_sno_subject (reached via the emit_core IR_SUBJECT branch); the box calls rt_sno_subject_load and
       stores Σ (base) / Δ (length) into its ζ-frame slot, then jmps γ (loaded) / ω. No operand subtree to
       walk (the subject value-expr rides on pBB->α and is consumed inside the box); β = jmp ω — same shape
       as flat_drive_sno_scan. */
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_sno_ref_invariant(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    /* PB-RB-1 REF_INVARIANT (GOAL-SNOBOL4-BB CORRECTED PATTERN ARCHITECTURE, 2026-06-01). The box loads a
       SEALED element bb_box_fn head (the EXISTING IR_PAT_LIT matcher box for an invariant literal — referenced
       via operand_aux per the PEERS RULE) into its ζ-frame slot, then jmps γ. NO runtime construction (Fork
       A/E): the sealed-head address is an emit-time constant (movabs in BINARY / [rip+disp] in TEXT). No
       operand subtree to control-thread (the sealed element is referenced, not run, here — running is PB-RB-3
       BB_MATCH); β = jmp ω — same bounded single-shot shape as flat_drive_sno_subject. The sealed child was
       emitted once by pre_build_children / pre_build_children_text (keyed in the child cache); resolve it via
       operand_aux (NOT bb_pat_kid) and hand its head to the box through g_emit.child_fn (BINARY fn ptr) /
       g_emit.bb_child_lbl (TEXT α-label). */
    int n_aux = 0;
    IR_t * const * aux = bb_operand_aux_get(g_emit_cfg, pBB, &n_aux);
    IR_t *ch = (n_aux > 0 && aux) ? aux[0] : NULL;
    bb_box_fn cfn = ch ? child_cache_get(ch) : NULL;
    g_emit.child_fn    = (void *)cfn;
    g_emit.bb_child_fn = (void *)cfn;
    g_emit.bb_child_lbl = cfn ? child_cache_get_lbl(cfn) : NULL;
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_match(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    /* PB-RB-3 BB_MATCH phase 3 (GOAL-SNOBOL4-BB CORRECTED PATTERN ARCHITECTURE, 2026-06-01, Opus 4.8). The
       MATCH box DRIVES the pattern element graph over Σ/δ/Δ with the SPITBOL Manual ch.18 unanchored OUTER
       start-loop. INLINE-JUMP model (Lon, verbatim: "jump to box's alpha, return from box's omega") — NO
       (ζ,int entry) C call. The element entry rides on operand_aux[0] (PEERS RULE), resolved as the inline
       element the way flat_drive_cat inline-emits its kids. The MATCH template (bb_match) emits the
       outer-loop prologue + retry + advance handler around a forward jmp to elem_entry; the driver then
       defines elem_entry and inline-emits the element via walk_bb_flat: element γ -> lbl_γ (chain success),
       element ω -> match_advance (try next start), element β -> element_β. */
    int n_aux = 0;
    IR_t * const * aux = bb_operand_aux_get(g_emit_cfg, pBB, &n_aux);
    IR_t *elem = (n_aux > 0 && aux) ? aux[0] : NULL;
    if (!elem) {
        fprintf(stderr, "[SBB] FATAL flat_drive_match: IR_PAT_MATCH has no element in operand_aux\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *elem_entry = emit_label_alloc("smatch%d_elem", id);
    bb_label_t *match_adv  = emit_label_alloc("smatch%d_adv",  id);
    bb_label_t *elem_β     = emit_label_alloc("smatch%d_elemb", id);
    g_match_elem_p      = elem_entry; g_match_elem_lbl    = elem_entry->name;
    g_match_advance_p   = match_adv;  g_match_advance_lbl = match_adv->name;
    EMIT_PAIR_RESET();
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
    emit_label_define_bb(elem_entry);
    walk_bb_flat(elem, lbl_γ, match_adv, elem_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_sno_program(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    /* SBL-M3-MULTISTMT (2026-05-31): the whole SNOBOL4 program as a sequence of four-port statement BBs.
       sno_ring_to_tree (scrip.c) folded each statement into a tree root and recorded its success/failure
       target statement index (the SPITBOL :S/:F/:(L) goto field). Here we allocate one label per statement,
       then emit each statement's body threading its γ→succ-statement-label and ω→fail-statement-label, so
       both ports flow to the next statement (or a goto target) exactly as SNOBOL4 control flow requires.
       A terminal landing (END / fall-off / RETURN) jumps to the program success label. Stackless. */
    sno_prog_t *prog = (sno_prog_t *)(intptr_t)pBB->ival;
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
        sno_stmt_t *st = &prog->stmts[i];
        if (st->is_terminal) {
            emit_jmp_label(lbl_γ, JMP_JMP);
            continue;
        }
        if (!st->root) {
            /* pass-through / bare goto: transfer directly to the target statement label. */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_assign(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α) {
        fprintf(stderr, "[IBB] FATAL flat_drive_assign: missing α (lhs IR_VAR)\n");
        abort();
    }
    if (pBB->α->t != IR_VAR || !pBB->α->sval) {
        fprintf(stderr, "[IBB] FATAL flat_drive_assign: lhs (α) must be IR_VAR with sval (got kind=%d)\n", (int)pBB->α->t);
        abort();
    }
    if (pBB->ival != 1) {
        if (!pBB->β) {
            fprintf(stderr, "[IBB] FATAL flat_drive_assign: missing β (rhs expr) and ival != 1\n");
            abort();
        }
        int id = g_flat_node_id++;
        bb_label_t *rhs_done = emit_label_alloc("xassign%d_rhs_done", id);
        bb_label_t *rhs_β    = emit_label_alloc("xassign%d_rhs_β",    id);
        walk_bb_flat(pBB->β, rhs_done, lbl_ω, rhs_β);
        emit_label_define_bb(rhs_done);
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_every(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    (void)lbl_ω;
    if (!pBB || !pBB->α) {
        fprintf(stderr, "[IBB] FATAL flat_drive_every: missing body (bb->α)\n");
        abort();
    }
    if (pBB->β) {
        if (pBB->ival == 0 && pBB->β &&
            (pBB->α->t == IR_FIELD_SET || pBB->α->t == IR_IDX_SET) &&
            gen_bb_is_gen_arg(pBB->α->β)) {
            IR_t *lval_node = pBB->α;
            IR_t *inner_gen = lval_node->β;
            int id0 = g_flat_node_id++;
            bb_label_t *gen_resume  = emit_label_alloc("xev0%d_gen_resume",  id0);
            bb_label_t *store_α     = emit_label_alloc("xev0%d_store_α",     id0);
            bb_label_t *body_αw     = emit_label_alloc("xev0%d_body_α",      id0);
            bb_label_t *store_βw    = emit_label_alloc("xev0%d_store_β",     id0);
            bb_label_t *body_βw     = emit_label_alloc("xev0%d_body_β",      id0);
            walk_bb_flat(inner_gen, store_α, lbl_γ, gen_resume);
            emit_label_define_bb(store_α);
            if (lval_node->t == IR_FIELD_SET) {
                bb_label_t *obj_done = emit_label_alloc("xev0%d_obj_done", id0);
                bb_label_t *obj_β    = emit_label_alloc("xev0%d_obj_β",    id0);
                walk_bb_flat(lval_node->α, obj_done, lbl_γ, obj_β);
                emit_label_define_bb(obj_done);
                EMIT_PAIR_RESET();
                EMIT_PAIR_DEF_JMP(store_βw, lbl_γ);
                EMIT_PAIR_FILL(lval_node, body_αw, lbl_γ, store_βw);
            } else {
                fprintf(stderr, "[IBB] FATAL flat_drive_every: ival=0 IDX_SET-wrapped gen not yet wired\n");
                abort();
            }
            emit_label_define_bb(body_αw);
            walk_bb_flat(pBB->β, gen_resume, gen_resume, body_βw);
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
            return;
        }
        if (pBB->ival != 1 && pBB->ival != 2) {
            fprintf(stderr, "[IBB] FATAL flat_drive_every: do-body ival=%lld not yet flat-wired (only ival=1/2)\n", (long long)pBB->ival);
            abort();
        }
        if (pBB->ival == 2 && pBB->α && pBB->α->t == IR_ASSIGN &&
            pBB->α->α && pBB->α->α->t == IR_VAR && pBB->α->β && gen_bb_is_gen_arg(pBB->α->β)) {
            IR_t *gen_assign = pBB->α;
            IR_t *gen        = gen_assign->β;
            int idw = g_flat_node_id++;
            bb_label_t *gen_resume = emit_label_alloc("xevery%d_gen_resume", idw);
            bb_label_t *store_α    = emit_label_alloc("xevery%d_store_α",    idw);
            bb_label_t *body_αw    = emit_label_alloc("xevery%d_body_α",     idw);
            bb_label_t *body_βw    = emit_label_alloc("xevery%d_body_β",     idw);
            bb_label_t *store_βw   = emit_label_alloc("xevery%d_store_β",    idw);
            (void)gen;
            walk_bb_flat(gen_assign->β, store_α, lbl_γ, gen_resume);
            emit_label_define_bb(store_α);
            int64_t saved_ival = gen_assign->ival;
            gen_assign->ival = 1;
            walk_bb_flat(gen_assign, body_αw, lbl_γ, store_βw);
            gen_assign->ival = saved_ival;
            emit_label_define_bb(body_αw);
            walk_bb_flat(pBB->β, gen_resume, gen_resume, body_βw);
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
            return;
        }
        int id2 = g_flat_node_id++;
        bb_label_t *β  = emit_label_alloc("xevery%d_gen_β",  id2);
        bb_label_t *body_α = emit_label_alloc("xevery%d_body_α", id2);
        bb_label_t *body_β = emit_label_alloc("xevery%d_body_β", id2);
        walk_bb_flat(pBB->α, body_α, lbl_γ, β);
        emit_label_define_bb(body_α);
        if (pBB->β && pBB->β->t == IR_ASSIGN && pBB->β->ival == 1 && pBB->β->γ) {
            bb_label_t *actual_body_α = emit_label_alloc("xevery%d_abody_α", id2);
            bb_label_t *store_ω       = emit_label_alloc("xevery%d_store_ω",  id2);
            walk_bb_flat(pBB->β, actual_body_α, lbl_γ, store_ω);
            emit_label_define_bb(actual_body_α);
            walk_bb_flat(pBB->β->γ, β, β, body_β);
        } else {
            walk_bb_flat(pBB->β, β, β, body_β);
        }
        EMIT_PAIR_RESET();
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    if (pBB->ival != 0) {
        fprintf(stderr, "[IBB] FATAL flat_drive_every: bodyless lower-ival=%lld not yet flat-wired (only ival=0)\n", (long long)pBB->ival);
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *body_β = emit_label_alloc("xevery%d_body_β", id);
    walk_bb_flat(pBB->α, body_β, lbl_γ, body_β);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_swap(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α || !pBB->β ||
        pBB->α->t != IR_VAR || !pBB->α->sval ||
        pBB->β->t != IR_VAR || !pBB->β->sval) {
        fprintf(stderr, "[IBB] FATAL flat_drive_swap: x:=:y requires two IR_VAR operands\n");
        abort();
    }
    int id = g_flat_node_id++;
    bb_label_t *x_done = emit_label_alloc("xswap%d_x",  id);
    bb_label_t *y_done = emit_label_alloc("xswap%d_y",  id);
    bb_label_t *x_β    = emit_label_alloc("xswap%d_xb", id);
    bb_label_t *y_β    = emit_label_alloc("xswap%d_yb", id);
    walk_bb_flat(pBB->α, x_done, lbl_ω, x_β);
    emit_label_define_bb(x_done);
    walk_bb_flat(pBB->β, y_done, lbl_ω, y_β);
    emit_label_define_bb(y_done);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int while_operand_simple(IR_t *o) {
    if (!o) return 0;
    switch (o->t) {
    case IR_VAR: case IR_LIT_I: case IR_LIT_S: case IR_LIT_F: case IR_LIT_NUL:
        return 1;
    case IR_BINOP:
        return o->state == 0 && while_operand_simple(o->α) && while_operand_simple(o->β);
    case IR_ASSIGN:
        return o->α && o->α->t == IR_VAR && (!o->β || while_operand_simple(o->β));
    default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int while_cond_emittable(IR_t *cond) {
    return cond && cond->t == IR_BINOP && cond->state >= 1 &&
           while_operand_simple(cond->α) && while_operand_simple(cond->β);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_while(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α) {
        fprintf(stderr, "[IBB] FATAL flat_drive_while: missing cond (bb->α)\n");
        abort();
    }
    IR_t *cond = pBB->α;
    IR_t *body = pBB->β;
    int is_until = (pBB->t == IR_UNTIL);
    int cond_is_relop = (cond->t == IR_BINOP && cond->state >= 1);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_alt_icn(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *arms[64];
    int n = 0;
    for (IR_t *a = pBB ? pBB->α : NULL; a && n < 64; a = a->ω) arms[n++] = a;
    if (n == 0) {
        EMIT_PAIR_RESET();
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    int id = g_flat_node_id++;
    bb_label_t **arm_α = (bb_label_t **)alloca((size_t)n * sizeof(bb_label_t *));
    bb_label_t **arm_β = (bb_label_t **)alloca((size_t)n * sizeof(bb_label_t *));
    for (int i = 0; i < n; i++) {
        arm_α[i] = emit_label_alloc("xalt%d_a%d_α", id, i);
        arm_β[i] = emit_label_alloc("xalt%d_a%d_β", id, i);
    }
    EMIT_PAIR_RESET();
    for (int i = 0; i < n; i++) EMIT_PAIR_JMP(arm_α[i]);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
    for (int i = 0; i < n; i++) {
        emit_label_define_bb(arm_α[i]);
        walk_bb_flat(arms[i], lbl_γ, lbl_ω, arm_β[i]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!nd) {
        bb_fill_alpha(nd);
        g_emit.lbl_γ = lbl_γ->name;
        g_emit.lbl_ω = lbl_ω->name;
        g_emit.lbl_β = lbl_β->name;
        g_emit.lbl_γ_p = lbl_γ; g_emit.lbl_ω_p = lbl_ω; g_emit.lbl_β_p = lbl_β;
        bb_eps(NULL);
        return;
    }
    switch (nd->t) {
    case IR_PAT_LIT:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ARB:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_REM:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_SPAN:   g_emit.op_name1 = nd->sval ? nd->sval : ""; g_emit.op_name2 = "bb_span";   g_emit.op_kind = "SPAN";   FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ANY:    g_emit.op_name1 = nd->sval ? nd->sval : ""; g_emit.op_name2 = "bb_any";    g_emit.op_kind = "ANY";    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_BREAK:  g_emit.op_name1 = nd->sval ? nd->sval : ""; g_emit.op_name2 = "bb_brk";    g_emit.op_kind = "BREAK";  FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_NOTANY: g_emit.op_name1 = nd->sval ? nd->sval : ""; g_emit.op_name2 = "bb_notany"; g_emit.op_kind = "NOTANY"; FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_LEN:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_POS:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ATP:    g_emit.op_name1 = nd->sval ? nd->sval : ""; FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_DEFER:  FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_TAB:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_FENCE:  flat_drive_fence(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ABORT:  FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_CAT:    flat_drive_cat(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ALT:    flat_drive_alt(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_ARBNO: {
        IR_t *ch = (bb_pat_nkids(nd) > 0) ? bb_pat_kid(nd, 0) : nd->α;
        bb_box_fn cfn = ch ? child_cache_get(ch) : NULL;
        g_emit.child_fn = (void *)cfn;
        FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_PAT_ASSIGN_IMM: {
        IR_t *ch = (bb_pat_nkids(nd) > 0) ? bb_pat_kid(nd, 0) : nd->α;
        bb_box_fn cfn = ch ? child_cache_get(ch) : NULL;
        const char *vn = nd->sval ? nd->sval : "";
        g_emit.child_fn = (void *)cfn; g_emit.op_name1 = vn; g_emit.op_name2 = NULL;
        FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_PAT_ASSIGN_COND: {
        IR_t *ch = (bb_pat_nkids(nd) > 0) ? bb_pat_kid(nd, 0) : nd->α;
        bb_box_fn cfn = ch ? child_cache_get(ch) : NULL;
        const char *vn = nd->sval ? nd->sval : "";
        g_emit.child_fn = (void *)cfn; g_emit.op_name1 = vn; g_emit.op_name2 = NULL;
        FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GCONJ:     flat_drive_pl_seq(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_GOAL:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_CHOICE:     flat_drive_pl_choice(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_DISJ:     flat_drive_pl_alt(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_ITE:     flat_drive_pl_ite(nd, lbl_γ, lbl_ω, lbl_β); break;
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
    case IR_LIT_I:
    case IR_LIT_S:
    case IR_LIT_F:
    case IR_LIT_NUL:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_CALL: {
        IR_t *a0 = nd->α;
        if (g_icn_flat_chain) {
            /* GZ-10 (modes 3/4): a dval==3.0 Icon user-procedure call WITH arguments needs each arg        */
            /* sub-graph (on nd->counter) walked inline so its producer boxes are emitted and claim ζ slots */
            /* the bb_call template then stages into g_icn_call_args before rt_icn_call_proc_descr. The      */
            /* zero-arg case (and every non-user-proc flat-chain call: write(operand), dval==2.0 RK) keeps   */
            /* the bare FILL — their args (if any) are leaves materialised in-template or the chain's own    */
            /* sibling producers. rt_proc_is_registered gates strictly to a known user proc.                 */
            if (nd->dval == 3.0 && (int)nd->ival > 0 && nd->sval && rt_proc_is_registered(nd->sval))
                flat_drive_icn_userproc(nd, lbl_γ, lbl_ω, lbl_β);
            else
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
            break;
        }
        int is_intexpr_shape = (a0 && (a0->t == IR_BINOP || a0->t == IR_LIT_I || a0->t == IR_TO || a0->t == IR_TO_BY || a0->t == IR_ALT || a0->t == IR_BINOP_GEN || a0->t == IR_VAR ||
                   a0->t == IR_NEG || a0->t == IR_POS || a0->t == IR_NONNULL || a0->t == IR_NULL_TEST || a0->t == IR_NOT || a0->t == IR_SIZE || a0->t == IR_CALL || a0->t == IR_CASE || a0->t == IR_FIELD_GET || a0->t == IR_LIST_BANG || a0->t == IR_LIMIT || a0->t == IR_IDX ));
        int is_write_fn   = (nd->sval && (!strcmp(nd->sval, "write") || !strcmp(nd->sval, "writes")));
        int write_str_simple1 = (nd->sval && !strcmp(nd->sval, "write") && (int)nd->ival == 1 && a0 && a0->t == IR_LIT_S && a0->sval);
        int write_simple1 = ((is_write_fn && (int)nd->ival == 1 && is_intexpr_shape) || write_str_simple1);
        int builtin_ok    = (nd->sval && rt_builtin_is_known(nd->sval) && call_args_single_shot(nd) && !write_simple1);
        if (nd->sval && rt_proc_is_registered(nd->sval))
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
        int op_is_rel = nd && ((nd->ival >= BINOP_LT && nd->ival <= BINOP_NE) ||
                               (nd->ival >= BINOP_SLT && nd->ival <= BINOP_SNE));
        int op_is_arith = nd && (nd->ival == BINOP_ADD || nd->ival == BINOP_SUB || nd->ival == BINOP_MUL || nd->ival == BINOP_DIV || nd->ival == BINOP_MOD);
        int op_is_concat = nd && (nd->ival == BINOP_CONCAT);
        /* TEMPLATE-REVAMP (Icon, x86() self-encoding): bb_binop_arith is pBB-free — it reads NO neighbor. */
        /* The driver resolves the operand slots (already collected by the flat-chain BFS) and the result   */
        /* slot HERE and deposits them as g_emit scalars; the box trusts op_off>=0 as "this is the arith     */
        /* case" (the op-code decision lives ONLY here, not duplicated in the box). Reset to -1 covers all   */
        /* three sub-branches so a stale positive can never make the arith box fire for a relop/concat/Raku. */
        g_emit.op_off = -1;
        if (g_icn_flat_chain && (op_is_rel || op_is_arith || op_is_concat)) {
            if (op_is_arith || op_is_rel || op_is_concat) {
                g_emit.op_sa = bb_slot_get(nd->α);
                g_emit.op_sb = bb_slot_get(nd->β);
                if (g_emit.op_sa >= 0 && g_emit.op_sb >= 0) g_emit.op_off = bb_slot_alloc16(nd);
            }
            /* GZ-8 relop / GZ-9 arith / GZ-11+ concat: operands are sibling boxes already collected by the   */
            /* flat-chain BFS (each wrote its own DESCR slot). Emit ONLY this box (FILL) — do NOT re-walk     */
            /* operands via flat_drive_binop_tree, which would duplicate them with fresh slots (and in mode-4 */
            /* TEXT double-define their bb<id>_α labels → assembler error) and clobber the slotmap. The       */
            /* GZ-11+ slot-concat arm in bb_binop.cpp reads each operand via bb_slot_get, so FILL is correct.  */
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β);
        } else if (!nd->α && !nd->β) {
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β);
        } else {
            flat_drive_binop_tree(nd, lbl_γ, lbl_ω, lbl_β);
        }
        break;
    }
    case IR_IF: {
        EMIT_PAIR_RESET();
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β);
        break;
    }
    case IR_BINOP_GEN:
        if (nd->α && nd->β && !binop_operand_streams(nd->α) && !binop_operand_streams(nd->β)) {
            IR_e saved_kind = nd->t;
            nd->t = IR_BINOP;
            flat_drive_binop_tree(nd, lbl_γ, lbl_ω, lbl_β);
            nd->t = saved_kind;
        } else {
            flat_drive_binop_gen_tree(nd, lbl_γ, lbl_ω, lbl_β);
        }
        break;
    case IR_SEQ:        flat_drive_seq(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SEQ_EXPR:   flat_drive_seq(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_EVERY:      flat_drive_every(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIMIT:      flat_drive_limit(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_TO:         FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_GATHER:     FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_TO_BY:      FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_ALT:        flat_drive_alt_icn(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_VAR:        if (g_icn_flat_chain && nd && nd->sval) { int voff = bb_varslot_peek(nd->sval); g_emit.op_sa = voff; g_emit.op_off = (voff >= 0) ? bb_slot_alloc16(nd) : -1; } else { g_emit.op_sa = -1; g_emit.op_off = -1; } FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_ASSIGN:     if (g_icn_flat_chain) FILL(nd, lbl_γ, lbl_ω, lbl_β); else if (nd->sval && nd->α && (nd->α->t == IR_LIT_S || nd->α->t == IR_VAR || nd->α->t == IR_SEQ || nd->α->t == IR_SEQ_EXPR)) flat_drive_sno_assign(nd, lbl_γ, lbl_ω, lbl_β); else if (nd->sval && nd->α && nd->α->t == IR_BINOP) flat_drive_sno_assign_binop(nd, lbl_γ, lbl_ω, lbl_β); else flat_drive_assign(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SCAN:       flat_drive_sno_scan(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SUBJECT:    flat_drive_sno_subject(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_REF_INVARIANT: flat_drive_sno_ref_invariant(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_MATCH:  flat_drive_match(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SNO_PROG:   flat_drive_sno_program(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_RETURN:
        /* GZ-10 (modes 3/4): in the flat γ-chain the return-value producer is a SIBLING box already      */
        /* BFS-collected (its DESCR is in its own slot); RETURN.α is the postfix reference to it. FILL     */
        /* ONLY this box (do NOT re-walk α via flat_drive_return — that double-emits with a fresh slot,    */
        /* the GZ-8/GZ-9 pattern). bb_return reads [r12+slot(α)] and writes it to the frame's return slot. */
        if (g_icn_flat_chain) { FILL(nd, lbl_γ, lbl_ω, lbl_β); break; }
        flat_drive_return(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SWAP:       flat_drive_swap(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_WHILE:
    case IR_UNTIL:
        if (while_cond_emittable(nd->α)) {
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
        /* TEMPLATE-REVAMP (Icon, x86() self-encoding): bb_unop is pBB-free — reads NO neighbor.             */
        /* Driver resolves operand slot and result slot HERE and deposits as g_emit scalars; the box trusts   */
        /* op_off>=0 as "flat-chain, slots valid". The KIND discriminator is op_node_kind (promoted from      */
        /* nd->t at the single dispatch point in walk_bb_node); the IR_UNOP mux sub-op (TT_MNS/TT_PLS) rides   */
        /* op_ival (also re-promoted there from nd->ival). IR_NOT has no value operand (op_sa==-1); all others */
        /* read the operand DESCR at [r12+op_sa].                                                             */
        if (g_icn_flat_chain) {
            g_emit.op_sa   = (nd->α) ? bb_slot_get(nd->α) : -1;
            g_emit.op_off  = bb_slot_alloc16(nd);
            FILL(nd, lbl_γ, lbl_ω, lbl_β);
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
        /* RK-EMIT-1 (2026-05-31): a Raku/Icon block `{ ... }` / `e1 & e2` wrapper (wire_seq IR_CONJ). In  */
        /* the flat chain its element boxes already ran as γ-chain nodes; the wrapper itself is a pure      */
        /* pass-through — α -> γ (continue to whatever the lowerer wired on the wrapper's γ, i.e. the       */
        /* statement AFTER the block), β -> ω. Without this it fell to the default (α,β -> ω), which dropped */
        /* every statement following an `if`/block because the continuation rides the CONJ's γ port. The α  */
        /* label is already defined by the chain BFS (or FILL elsewhere); emit only the two forwarders.     */
        emit_jmp_label(lbl_γ, JMP_JMP);
        emit_label_define_bb(lbl_β);
        emit_jmp_label(lbl_ω, JMP_JMP);
        break;
    case IR_BREAK:
    case IR_NEXT:
    case IR_REPEAT:
        /* GZ-9 (Model B) flat-chain: pure unconditional forwarders. IR_BREAK/IR_NEXT jump to the loop  */
        /* exit / re-entry (wired onto γ by the lowerer); IR_REPEAT jumps back to the body start (γ).   */
        /* lbl_γ is resolved by the chain BFS from node->γ. β is a dead landing (jmp γ as well).         */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* GZ-7 chain body: same prologue/epilogue shell as codegen_flat_body, but the body is the WHOLE Icon     */
/* gamma-chain emitted as a flat goto-graph (every box once, wired by its native gamma/omega ports). BFS  */
/* the reachable boxes from entry, give each a label, then emit each via walk_bb_flat with its real       */
/* successor/failure labels resolved against the collected set (falling off to the sequence gamma/omega   */
/* sentinels). g_icn_flat_chain is already set by the caller so the per-box slot-leaf arms fire.          */
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
    /* GZ-7 (chain-entry sentinel fix): a `local`/`static` declaration (or an empty leading statement) makes the lowerer prepend one or more IR_SUCCEED/IR_FAIL PRELUDE sentinels whose own γ threads to the real first box (`[6] IR_SUCCEED γ=5` for `local x; x:=42; write(x)`). The BFS below treats a SUCCEED/FAIL node as a chain TERMINATOR (the γ/ω sink, γ==NULL) and skips it WITHOUT following γ — correct for a sink, but if the graph ENTRY is itself such a prelude sentinel the skip dropped the entire chain (n=0 → empty body, a silent miscompile). Advance entry forward through any run of leading prelude sentinels (a sentinel WITH a non-NULL γ) to the first real box; a terminal sentinel (γ==NULL) leaves entry unchanged so a genuinely-empty body still collects nothing. */
    { int guard = 0; while (entry && (entry->t == IR_SUCCEED || entry->t == IR_FAIL) && entry->γ && guard++ < CH_MAX) entry = entry->γ; }
    queue[qt++] = entry;
    while (qh < qt) {
        IR_t *c = queue[qh++];
        if (!c || c->t == IR_SUCCEED || c->t == IR_FAIL) continue;
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); }
        nodes[n++] = c;
        if (c->γ && qt < CH_MAX) queue[qt++] = c->γ;
        if ((c->t == IR_BINOP || c->t == IR_BINOP_GEN) && c->ω && qt < CH_MAX) queue[qt++] = c->ω;
        /* GZ-10 PROC-FAIL (else-branch follow): an IR_CALL to a user proc may route its ω to an else-     */
        /* continuation (e.g. `if positive(0) then write(2) else write(3)` → IR_CALL.ω = IR_LIT_I[3]).    */
        /* The γ-only BFS above would never collect the else node; its α label stays unresolved → the      */
        /* FAIL-CHECK `je main_ω` hits the epilogue instead of the else branch. Follow ω for IR_CALL:      */
        /* non-terminal ω nodes (non-SUCCEED/FAIL) become chain members with proper α labels. Terminals   */
        /* (ω = IR_SUCCEED/IR_FAIL) are filtered at the loop's continue above — zero impact on normal calls.*/
        if (c->t == IR_CALL && c->ω && qt < CH_MAX) queue[qt++] = c->ω;
        /* goto-chain whose CONTINUATION (the statement after the loop, e.g. `say('done')`) hangs off the   */
        /* generator's ω port (gen drained ⇒ for completes ⇒ run the continuation). The γ-only BFS would    */
        /* never reach it (only IR_TO-style generators consumed by Icon's flat_drive_every wrapper, which   */
        /* stitches its own continuation, avoid this). Following ω for the Raku-only IR_GATHER kind makes    */
        /* the continuation a chain node so the gather box's ω-port resolves to its α label and emits it.    */
        /* RAKU-ONLY KIND ⇒ ZERO blast radius on Icon/SNOBOL/Prolog chains (their generators are not        */
        /* IR_GATHER). IR_MAP/IR_GREP are deliberately NOT added here — that rung is not yet built.          */
        if (c->t == IR_GATHER && c->ω && qt < CH_MAX) queue[qt++] = c->ω;
    }
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    int id = g_flat_node_id++;
    for (int i = 0; i < n; i++) {
        lbls[i]  = emit_label_alloc("xchain%d_n%d_α", id, i);
        betas[i] = emit_label_alloc("xchain%d_n%d_β", id, i);
    }
    for (int i = 0; i < n; i++) {
        emit_label_define_bb(lbls[i]);
        bb_label_t *node_γ = &lbl_γ;
        bb_label_t *node_ω = &lbl_ω;
        for (int k = 0; k < n; k++) if (nodes[k] == nodes[i]->γ) { node_γ = lbls[k]; break; }
        if (nodes[i]->γ == NULL || nodes[i]->γ->t == IR_SUCCEED) node_γ = &lbl_γ;
        int omega_resolved = 0;
        for (int k = 0; k < n; k++) if (nodes[k] == nodes[i]->ω) { node_ω = lbls[k]; omega_resolved = 1; break; }
        if (!omega_resolved) node_ω = &lbl_ω;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int codegen_flat_body(IR_t *nd, const char *prefix, int text_externalise, int brokered) {
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
    if (text_externalise && g_is_text) emit_label_define_bb(&lbl_α);
    xa_dispatch(XA_FLAT_PROLOGUE);
    if (g_is_text) g_emit_pos += 7;
    emit_label_define_bb(&lbl_α_body);
    walk_bb_flat(nd, &lbl_γ, &lbl_ω, &lbl_β);
    g_emit.flat_brokered = brokered;
    emit_label_define_bb(&lbl_γ);
    xa_dispatch(XA_FLAT_EPILOGUE);
    if (text_externalise && g_is_text) {
        data_buf_flush_pending_label();
        xa_dispatch(XA_FLAT_DATA_SECTION);
        data_buf_reset();
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int g_in_prebuild = 0;
static int g_text_child_counter = 0;
static void pre_build_children_text(IR_t *nd, FILE *out, const char *base_prefix) {
    if (!nd) return;
    if (nd->t == IR_REF_INVARIANT) {
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
    if (nd->t == IR_PAT_ARBNO || nd->t == IR_PAT_ASSIGN_COND || nd->t == IR_PAT_ASSIGN_IMM || nd->t == IR_PAT_CALLOUT) {
        IR_t *ch = (bb_pat_nkids(nd) > 0) ? bb_pat_kid(nd, 0) : nd->α;
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
    if (bb_pat_nkids(nd) == 0) return;
    for (int i = 0; i < bb_pat_nkids(nd); i++) pre_build_children_text(bb_pat_kid(nd, i), out, base_prefix);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pre_build_children(IR_t *nd) {
    if (!nd) return;
    if (nd->t == IR_REF_INVARIANT) {
        int n_aux = 0;
        IR_t * const * aux = bb_operand_aux_get(g_emit_cfg, nd, &n_aux);
        IR_t *ch = (n_aux > 0 && aux) ? aux[0] : NULL;
        if (ch && !child_cache_get(ch)) {
            pre_build_children(ch);
            bb_box_fn fn = bb_build_flat(ch);     /* sealed matcher element (e.g. bb_lit) — a plain four-port box, NOT brokered */
            child_cache_put(ch, fn);
        }
        return;
    }
    if (nd->t == IR_PAT_ARBNO || nd->t == IR_PAT_ASSIGN_COND || nd->t == IR_PAT_ASSIGN_IMM || nd->t == IR_PAT_CALLOUT) {
        IR_t *ch = (bb_pat_nkids(nd) > 0) ? bb_pat_kid(nd, 0) : nd->α;
        if (ch && !child_cache_get(ch)) {
            pre_build_children(ch);
            bb_box_fn fn = (nd->t == IR_PAT_ARBNO || nd->t == IR_PAT_ASSIGN_COND || nd->t == IR_PAT_ASSIGN_IMM) ? bb_build_brokered(ch) : bb_build_flat(ch);
            child_cache_put(ch, fn);
        }
        return;
    }
    if (bb_pat_nkids(nd) == 0) return;
    for (int i = 0; i < bb_pat_nkids(nd); i++) pre_build_children(bb_pat_kid(nd, i));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* GZ-7 (GROUND ZERO 3) ICON FLAT-CHAIN EMITTER. The unified lowerer emits an Icon procedure body as a    */
/* postfix gamma-chain (operands precede their consumer; every node alpha=beta=NULL; branches diverge at  */
/* a relop's omega and re-join). A multi-statement / variable-bearing graph (e.g. `x := 42; write(x)`) is */
/* NOT a single expression tree, so icn_ring_to_tree (which un-flattens ONE tree) returns NULL for it.    */
/* This builder emits the graph as a FLAT GOTO-GRAPH in the test_sno_*.c model: every box is emitted      */
/* EXACTLY ONCE, wired by its native gamma/omega ports (jmp rel32), and every box reads its operands from  */
/* the producer box's SLOT (bb_slot_get) or a named variable slot (bb_varslot) and writes its own result  */
/* slot. NO value stack, NO ring (the ring is the mode-2 oracle's model only). Two passes:                 */
/*   (1) postfix operand-ref pass — walk the gamma-chain; a stack tracks value-producing boxes; when a    */
/*       consumer of arity k is reached, its k preceding producers are recorded on its alpha/beta as       */
/*       OPERAND REFERENCES (so the slot-leaf template can bb_slot_get them) — the boxes are NOT re-walked.*/
/*   (2) flat emission — BFS the chain from entry following gamma (and omega at branch nodes), assign each */
/*       box a label, and emit each via walk_bb_flat with its real gamma/omega target labels.              */
/* g_icn_flat_chain gates the per-box slot-leaf arms during emission.                                      */
static int icn_chain_arity(const IR_t *n) {
    switch (n->t) {
    case IR_LIT_I: case IR_LIT_S: case IR_LIT_F: case IR_LIT_NUL:
    case IR_VAR:   case IR_KEYWORD: return 0;
    /* RK-EMIT-GATHER (2026-06-01): IR_GATHER is a self-contained resumable Seq producer — its take         */
    /* payloads live on `counter` (extracted at emit time), NOT as γ-chain operands, so from the postfix    */
    /* operand-ref walk's view it is a LEAF producer (arity 0). Its yielded element is the chain producer    */
    /* the following bind (IR_ASSIGN) reads from its ζ slot. Reporting >0 here would wrongly pop chain       */
    /* producers; reporting -1 (the old default) reset the postfix stack so the consuming bind's α went     */
    /* unset (NULL) → the bb_assign FATAL. Arity 0 makes it push itself for the bind to reference.           */
    case IR_GATHER: return 0;
    case IR_BINOP: case IR_BINOP_GEN: case IR_TO: case IR_TO_BY: return 2;
    case IR_UNOP:  case IR_NEG: case IR_POS: case IR_NONNULL: case IR_NOT: case IR_SIZE: return 1;
    case IR_ASSIGN: return 1;
    case IR_RETURN: return 1;
    /* RK-EMIT-2 (2026-05-31): a dval==2.0 deterministic call (Raku __rk_arr / elems / __rk_jct_* / …)     */
    /* marshals its OWN arguments from isolated value sub-graphs on `counter`; those args are NOT operands */
    /* on the γ-chain, so from the postfix operand-ref walk's view this call is a self-contained LEAF       */
    /* producer (arity 0). Reporting nargs here would wrongly pop that many chain producers and corrupt the */
    /* operand references of every later consumer (e.g. the relop in `$x == any(..)`).                      */
    /* GZ-10 (2026-06-01): the Icon dval==3.0 user-proc call is ALSO a LEAF producer (arity 0) in the flat- */
    /* chain model — its arguments live on `counter` and are emitted as inline sub-chains by                */
    /* flat_emit_arg_subchain, NOT as γ-chain operands. The old `(int)n->ival` here belonged to the legacy  */
    /* value-stack flat_drive_call_userproc path (args γ-chained); under g_icn_flat_chain that path is gone, */
    /* so reporting ival made a 1-arg call like `fact(n-1)` wrongly pop the sibling `n` off the operand      */
    /* stack — corrupting the enclosing `n * fact(n-1)` binop's α (it resolved to a RETURN/garbage slot).   */
    /* Both marshalled call flavours are leaves; their RESULT is the chain producer a following box reads.   */
    case IR_CALL:  return (n->dval == 2.0 || n->dval == 3.0) ? 0 : (int)n->ival;
    default:       return -1;
    }
}
static void icn_chain_operand_refs(IR_t *entry) {
    IR_t *chain[512]; int nc = 0;
    IR_t *seen[512]; int ns = 0;
    IR_t *stkv[512]; int sv = 0;
    /* GZ-7 (chain-entry sentinel fix): mirror codegen_flat_chain_body — a `local`/`static` declaration prepends IR_SUCCEED/IR_FAIL PRELUDE sentinels whose γ threads to the real first box. This postfix operand-ref walk skips SUCCEED/FAIL (terminators) without following γ, so a sentinel ENTRY dropped the whole chain (nc=0 → no operand refs wired → IR_ASSIGN/IR_UNOP α left NULL → bb_assign/bb_unop FATAL). Advance entry past any run of leading prelude sentinels (γ non-NULL) to the first real box; a terminal sentinel (γ==NULL) is left as-is. */
    { int guard = 0; while (entry && (entry->t == IR_SUCCEED || entry->t == IR_FAIL) && entry->γ && guard++ < 512) entry = entry->γ; }
    stkv[sv++] = entry;
    while (sv > 0 && nc < 512) {
        IR_t *c = stkv[--sv];
        if (!c || c->t == IR_SUCCEED || c->t == IR_FAIL) continue;
        int dup = 0; for (int i = 0; i < ns; i++) if (seen[i] == c) { dup = 1; break; }
        if (dup) continue;
        seen[ns++] = c; chain[nc++] = c;
        if ((c->t == IR_BINOP || c->t == IR_BINOP_GEN) && c->ω && sv < 512) stkv[sv++] = c->ω;
        /* GZ-10 PROC-FAIL: mirror the emit-BFS ω-follow for IR_CALL so the else-continuation node        */
        /* gets its operand refs set (else-branch producers have their α/β wired here). Without this,     */
        /* the else-branch producer node is never visited → its α stays NULL → FATAL in its template.    */
        if (c->t == IR_CALL && c->ω && sv < 512) stkv[sv++] = c->ω;
        /* CONTINUATION (on the gather's ω) gets its operand refs (α/β) set by this postfix walk — else its */
        /* consuming call (e.g. say('done') reading a separate IR_LIT_S producer box) has a NULL α and is   */
        /* mis-routed to the builtin-dispatch arm. Pushed BEFORE γ (LIFO) so the loop body linearizes first */
        /* and the continuation forms a separate trailing segment. RAKU-ONLY kind ⇒ zero peer impact.       */
        if (c->t == IR_GATHER && c->ω && sv < 512) stkv[sv++] = c->ω;
        if (c->γ && sv < 512) stkv[sv++] = c->γ;
    }
    IR_t *stk[512]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = icn_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        if (ar == 2 && sp >= 2) { n->β = stk[sp - 1]; n->α = stk[sp - 2]; sp -= 2; }
        else if (ar == 1 && sp >= 1) { n->α = stk[sp - 1]; sp -= 1; }
        else if (ar >= 1) { sp = 0; }
        stk[sp++] = n;
    }
}
bb_box_fn icn_flat_chain_build(IR_t *entry) {
    if (!entry) return NULL;
    icn_chain_operand_refs(entry);
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_slot_count = 0; g_flat_node_id = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    g_icn_flat_chain = 1;
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_flat_chain_body(entry, "pat_flat");
    int nbytes = emitter_end();
    g_icn_flat_chain = 0;
    extern int bb_emit_overflow;
    if (bb_emit_overflow || nbytes <= 0 || nbytes > FLAT_BUF_MAX) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    bb_seal(buf, (size_t)nbytes);
    bb_pool_trim_last(buf, FLAT_BUF_MAX, (size_t)nbytes);
    return (bb_box_fn)buf;
}
int icn_flat_chain_build_text(IR_t *entry, FILE *out, const char *prefix) {
    if (!entry) return 1;
    icn_chain_operand_refs(entry);
    g_flat_slot_count = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    /* g_flat_node_id is NOT reset here — it persists across all proc slabs already emitted so main        */
    /* gets a unique xchainN_* label range that cannot collide with any prior proc slab's labels.          */
    g_icn_flat_chain = 1;
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    int rc = codegen_flat_chain_body(entry, prefix);
    emitter_end();
    g_icn_flat_chain = 0;
    return rc;
}
/* GZ-10 (modes 3/4): build a user-PROCEDURE body as a flat goto-graph slab with the stackless calling     */
/* convention used by rt_icn_call_proc_descr — the ζ=r12 frame's [0,16) is RESERVED for the return-value   */
/* DESCR slot (IR_RETURN writes it), and parameter i is the named-variable slot at [16*(i+1)] (pre-seeded   */
/* here so the body's IR_VAR(param)/IR_ASSIGN(param) resolve to the same offset the helper binds the arg    */
/* into). The proc body's own anonymous/local slots follow. Identical to icn_flat_chain_build otherwise.    */
bb_box_fn icn_flat_chain_build_proc(IR_t *entry, const char **pnames, int np) {
    if (!entry) return NULL;
    icn_chain_operand_refs(entry);
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_slot_count = 0; g_flat_node_id = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    g_icn_flat_chain = 1;
    g_flat_slot_count = 16;
    for (int i = 0; i < np && pnames; i++) if (pnames[i]) (void)bb_varslot(pnames[i]);
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_flat_chain_body(entry, "proc_flat");
    int nbytes = emitter_end();
    g_icn_flat_chain = 0;
    extern int bb_emit_overflow;
    if (bb_emit_overflow || nbytes <= 0 || nbytes > FLAT_BUF_MAX) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    bb_seal(buf, (size_t)nbytes);
    bb_pool_trim_last(buf, FLAT_BUF_MAX, (size_t)nbytes);
    return (bb_box_fn)buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* GZ-10 mode-4 (MEDIUM_TEXT): emit a user-procedure body as a named GAS asm slab with the same stackless  */
/* calling convention as icn_flat_chain_build_proc (BINARY). The prefix is "icn_proc_<name>" so the entry  */
/* point is emitted as the globally-visible label "icn_proc_<name>_α:" that the startup registration stub  */
/* (emitted by the mode-4 driver in scrip.c) calls rt_proc_set_fn(<name>, icn_proc_<name>_α) to wire.      */
/* Frame layout is identical to BINARY: [r12+0]=return DESCR slot, [r12+16*(i+1)]=param i slot.            */
int icn_flat_chain_build_proc_text(IR_t *entry, const char **pnames, int np, FILE *out, const char *pname) {
    if (!entry || !out || !pname) return 1;
    icn_chain_operand_refs(entry);
    g_flat_slot_count = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    /* NOTE: g_flat_node_id is NOT reset — it persists across all proc slabs + main so each slab gets a   */
    /* unique xchainN_* label namespace and collisions between proc and main slabs cannot occur.           */
    g_icn_flat_chain = 1;
    g_flat_slot_count = 16;
    for (int i = 0; i < np && pnames; i++) if (pnames[i]) (void)bb_varslot(pnames[i]);
    char prefix[256];
    snprintf(prefix, sizeof(prefix), "icn_proc_%s", pname);
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    fprintf(out, "  .globl %s_\316\261\n", prefix);
    int rc = codegen_flat_chain_body(entry, prefix);
    emitter_end();
    g_icn_flat_chain = 0;
    return rc;
}
/*====================================================================================================================================================================================================*/
/* SBL-M3-CHAIN (2026-05-31, Opus 4.8) — SNOBOL4 FLAT-CHAIN EMITTER (PB-0 substrate; the sno_ring_to_tree   */
/* replacement). LOWER (lower_program.c) emits a SNOBOL4 program as a four-port graph whose statements are  */
/* threaded through IR_SUCCEED LANDING nodes (g->entry is land[0], itself an IR_SUCCEED whose .γ is the     */
/* first statement). This builder consumes THAT graph directly — NO ring->tree adapter (banned, VIOLATION) */
/* — emitting it as a FLAT GOTO-GRAPH in the test_sno_1.c model: every box emitted EXACTLY ONCE, wired by   */
/* its native γ/ω ports (jmp rel32), reading operands from the producer box's slot or baking RO constants.  */
/* NO value stack, NO ring. It differs from the Icon chain builder (icn_flat_chain_build) in two ways: (1)  */
/* it RESOLVES landing nodes transitively — a port to an IR_SUCCEED-with-γ follows through to its target,   */
/* so the per-statement landings are transparent; a terminal IR_SUCCEED (γ==NULL = PSUCC) maps to the       */
/* success epilogue and IR_FAIL (PFAIL) to the failure epilogue; (2) it does NOT set g_icn_flat_chain, so   */
/* walk_bb_flat takes the SNOBOL4 arms (IR_ASSIGN -> bb_sno_assign, IR_SCAN -> bb_sno_scan, ...) not the    */
/* Icon slot-leaf arm. The operand-ref pass (sno_chain_operand_refs) is the SAME postfix-arity model as     */
/* Icon's — it derives OPERAND REFERENCES (which producer a consumer reads), NOT topology; topology is from */
/* LOWER. PER-BOX LOCAL STORAGE FACT RULE: every box-local read is [rip+disp] (RO) or [ζ=r12+off] (RW).     */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Resolve a port target through chained IR_SUCCEED landing nodes (each a pass-through whose .γ is the next  */
/* real node). Stops at the first non-landing node, at a terminal IR_SUCCEED (γ==NULL = program success),   */
/* or at IR_FAIL (program failure). Bounded guard against a malformed cyclic landing chain.                 */
static IR_t *sno_chain_resolve(IR_t *n) {
    int guard = 0;
    while (n && n->t == IR_SUCCEED && n->γ != NULL && guard++ < 4096) n = n->γ;
    return n;
}
static int sno_chain_is_real(IR_t *n) { return n && n->t != IR_SUCCEED && n->t != IR_FAIL; }
/* SNOBOL4 chain arity: a SNOBOL concat IR_SEQ/IR_SEQ_EXPR (dval==1.0) carries its two operands in ISOLATED  */
/* sub-graphs (on counter/ival), NOT on the γ-chain, so from the chain's perspective it is a LEAF producer  */
/* (arity 0) whose value the consuming IR_ASSIGN reads via bb_sno_assign_concat. Everything else delegates   */
/* to the shared icn_chain_arity.                                                                            */
static int sno_chain_arity(const IR_t *n) {
    if (n && (n->t == IR_SEQ || n->t == IR_SEQ_EXPR) && n->dval == 1.0) return 0;
    /* IR_SCAN consumes its single γ-predecessor: the subject value-node (plain form) or the replacement    */
    /* value-node (repl form, subject is by-name on sval). bb_sno_scan reads it off α. SPITBOL Manual ch.6.  */
    if (n && n->t == IR_SCAN) return 1;
    return icn_chain_arity(n);
}
/* Postfix operand-ref pass for ONE statement's γ-chain: starting at the statement's first real node, walk  */
/* γ (the linear value-flow) until it exits the statement (into a landing IR_SUCCEED, a terminal, or IR_FAIL */
/* — failure/backtrack edges are NOT value-flow), pushing value producers and recording each consumer's k    */
/* preceding producers on its α/β. Cycle-guarded.                                                            */
static void sno_stmt_operand_refs(IR_t *head) {
    IR_t *chain[512]; int nc = 0;
    IR_t *c = head;
    while (sno_chain_is_real(c) && nc < 512) {
        int dup = 0; for (int i = 0; i < nc; i++) if (chain[i] == c) { dup = 1; break; }
        if (dup) break;
        chain[nc++] = c;
        IR_t *g = c->γ;
        if (!g || g->t == IR_SUCCEED || g->t == IR_FAIL) break;   /* γ leaves the statement */
        c = g;
    }
    IR_t *stk[512]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = sno_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        if (ar == 2 && sp >= 2) { n->β = stk[sp - 1]; n->α = stk[sp - 2]; sp -= 2; }
        else if (ar == 1 && sp >= 1) { n->α = stk[sp - 1]; sp -= 1; }
        else if (ar >= 1) { sp = 0; }
        stk[sp++] = n;
    }
}
/* PB-RB-1: the SNOBOL flat-chain builders (sno_flat_chain_build / _text) emit the whole program graph     */
/* directly (not via bb_build_flat). A NEW IR_REF_INVARIANT node references its sealed IR_PAT_LIT element  */
/* via operand_aux and needs that element emitted ONCE as its own box + keyed in the child cache BEFORE the */
/* REF box is filled. We pre-build ONLY IR_REF_INVARIANT here — the pre-existing ARBNO/capture/CALLOUT      */
/* kinds keep their established emission-time child handling (bb_build_brokered inside their own path), so  */
/* this addition is byte-neutral to every prior shape. pre_build_children self-guards via child_cache_get.  */
static void sno_chain_prebuild_children(IR_graph_t *g) {
    if (!g || !g->all) return;
    for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->t == IR_REF_INVARIANT) pre_build_children(g->all[i]);
}
static void sno_chain_prebuild_children_text(IR_graph_t *g, FILE *out, const char *prefix) {
    if (!g || !g->all) return;
    for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->t == IR_REF_INVARIANT) pre_build_children_text(g->all[i], out, prefix);
}
/* Run sno_stmt_operand_refs for EVERY statement in the SNOBOL4 program graph. A statement head is the       */
/* landing-resolved γ-target of the program entry AND of each landing node (IR_SUCCEED-with-γ); deduped.     */
/* This covers statements reachable only via a failure (ω) edge or a goto, which a γ-only walk from the      */
/* entry would miss (then their consumer's α would be unset and flat_drive_assign would abort on emit).      */
static void sno_chain_operand_refs(IR_graph_t *g) {
    if (!g || !g->all) return;
    IR_t *heads[2048]; int nh = 0;
    IR_t *e0 = sno_chain_resolve(g->entry);
    if (sno_chain_is_real(e0)) heads[nh++] = e0;
    for (int i = 0; i < g->n && nh < 2048; i++) {
        IR_t *L = g->all[i];
        if (!L || L->t != IR_SUCCEED || L->γ == NULL) continue;
        IR_t *h = sno_chain_resolve(L);
        if (!sno_chain_is_real(h)) continue;
        int dup = 0; for (int k = 0; k < nh; k++) if (heads[k] == h) { dup = 1; break; }
        if (!dup) heads[nh++] = h;
    }
    for (int i = 0; i < nh; i++) sno_stmt_operand_refs(heads[i]);
}
/* Chain body: same prologue/epilogue shell as codegen_flat_chain_body, but the BFS and the per-node γ/ω    */
/* target resolution both run through sno_chain_resolve so the landing nodes are transparent.               */
static int codegen_sno_flat_chain_body(IR_t *entry, const char *prefix) {
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
    IR_t *e0 = sno_chain_resolve(entry);
    if (sno_chain_is_real(e0)) queue[qt++] = e0;
    while (qh < qt) {
        IR_t *c = queue[qh++];           /* already resolved + real on enqueue */
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= CH_MAX) { fprintf(stderr, "[SBB] FATAL sno chain exceeds CH_MAX\n"); abort(); }
        nodes[n++] = c;
        IR_t *g = sno_chain_resolve(c->γ);
        IR_t *w = sno_chain_resolve(c->ω);
        if (sno_chain_is_real(g) && qt < CH_MAX) queue[qt++] = g;
        if (sno_chain_is_real(w) && qt < CH_MAX) queue[qt++] = w;
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
        bb_label_t *node_γ = &lbl_γ;     /* default: program success epilogue */
        bb_label_t *node_ω = &lbl_ω;     /* default: program failure epilogue */
        IR_t *g = sno_chain_resolve(nodes[i]->γ);
        IR_t *w = sno_chain_resolve(nodes[i]->ω);
        if (sno_chain_is_real(g)) { for (int k = 0; k < n; k++) if (nodes[k] == g) { node_γ = lbls[k]; break; } }
        else if (g && g->t == IR_FAIL) node_γ = &lbl_ω;     /* resolved to PFAIL */
        if (sno_chain_is_real(w)) { for (int k = 0; k < n; k++) if (nodes[k] == w) { node_ω = lbls[k]; break; } }
        else if (w && w->t == IR_FAIL) node_ω = &lbl_ω;     /* resolved to PFAIL */
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
bb_box_fn sno_flat_chain_build(IR_graph_t *g) {
    if (!g || !g->entry) return NULL;
    IR_graph_t *save_cfg = g_emit_cfg; g_emit_cfg = g;
    int has_ref = 0; for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->t == IR_REF_INVARIANT) { has_ref = 1; break; }
    if (has_ref && !g_in_prebuild) { g_child_cache_n = 0; g_in_prebuild = 1; sno_chain_prebuild_children(g); g_in_prebuild = 0; }
    sno_chain_operand_refs(g);
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) { g_emit_cfg = save_cfg; return NULL; }
    g_flat_slot_count = 0; g_flat_node_id = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    g_sno_flat_chain = 1;
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_sno_flat_chain_body(g->entry, "sno_flat");
    int nbytes = emitter_end();
    g_sno_flat_chain = 0;
    g_emit_cfg = save_cfg;
    extern int bb_emit_overflow;
    if (bb_emit_overflow || nbytes <= 0 || nbytes > FLAT_BUF_MAX) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    bb_seal(buf, (size_t)nbytes);
    bb_pool_trim_last(buf, FLAT_BUF_MAX, (size_t)nbytes);
    return (bb_box_fn)buf;
}
int sno_flat_chain_build_text(IR_graph_t *g, FILE *out, const char *prefix) {
    if (!g || !g->entry) return 1;
    IR_graph_t *save_cfg = g_emit_cfg; g_emit_cfg = g;
    int has_ref = 0; for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->t == IR_REF_INVARIANT) { has_ref = 1; break; }
    if (has_ref) { g_child_cache_n = 0; g_text_child_counter = 0; sno_chain_prebuild_children_text(g, out, prefix); }
    sno_chain_operand_refs(g);
    g_flat_slot_count = 0; g_flat_node_id = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    g_sno_flat_chain = 1;
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    int rc = codegen_sno_flat_chain_body(g->entry, prefix);
    emitter_end();
    g_sno_flat_chain = 0;
    g_emit_cfg = save_cfg;
    return rc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_box_fn bb_build_brokered(IR_t *nd) {
    if (!g_in_prebuild) { g_child_cache_n = 0; g_in_prebuild = 1; pre_build_children(nd); g_in_prebuild = 0; }
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_slot_count = 0; g_flat_node_id = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    emit_mode_set(EMIT_BINARY_BROKERED, NULL);
    emitter_init_binary(buf, FLAT_BUF_MAX);
    bb_emit_byte(0x55); bb_emit_byte(0x48); bb_emit_byte(0x89); bb_emit_byte(0xE5);
    codegen_flat_body(nd, "pat_brok", 0, 1);
    int nbytes = emitter_end();
    emit_mode_set(EMIT_BINARY_WIRED, NULL);
    extern int bb_emit_overflow;
    if (bb_emit_overflow || nbytes <= 0 || nbytes > FLAT_BUF_MAX) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    bb_seal(buf, (size_t)nbytes);
    bb_pool_trim_last(buf, FLAT_BUF_MAX, (size_t)nbytes);
    return (bb_box_fn)buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int codegen_flat_build(IR_t *nd, FILE *out, const char *prefix) {
    g_child_cache_n = 0;
    g_text_child_counter = 0;
    pre_build_children_text(nd, out, prefix);
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    int rc = codegen_flat_body(nd, prefix, 1, 0);
    emitter_end();
    return rc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PLG-9d (2026-06-01) — PROLOG PREDICATE-REGISTRY TEXT EMIT. bb_goal.cpp's IR_GOAL arm calls a callee     */
/* predicate by `call .Lplpred_<name>_<arity>` (fresh entry) and `call .Lplpred_<name>_<arity>_redo` (β    */
/* re-entry on backtrack), then `call rt_last_ok` to learn whether the callee body reached success or      */
/* failure. The flat-tier mode-4 arm (PLG-9a..9c) emitted only main_α; nothing defined those callee labels */
/* (the SM_BB_PL_INVOKE callee-sweep that did so pre-Ground-Zero is severed). This emits each registered   */
/* predicate's body as a TEXT block whose:                                                                  */
/*   .Lplpred_<name>_<arity>:        — fresh-entry α; predecessor `call` lands here, body runs              */
/*   .Lplpred_<name>_<arity>_redo:   — β re-entry; bb_goal.cpp `call`s this to re-drive a live CP           */
/* and whose γ (body success) → `rt_set_last_ok(1); ret` and ω (body failure / exhausted) →                */
/* `rt_set_last_ok(0); ret`. The body itself is walked by walk_bb_flat (the SAME dispatch the flat tier     */
/* uses), so IR_CHOICE -> flat_drive_pl_choice, IR_GCONJ -> flat_drive_pl_seq, IR_UNIFY/IR_BUILTIN -> their */
/* TEXT arms emit the bytes (FACT-clean: every byte still comes from a template fn via the dispatch). The   */
/* callee runs on the SAME ζ-frame as main (rt_frame passes one frame for the whole activation chain in     */
/* the flat model); g_frame_active is already set by the caller so the prologue does push r12/mov r12,rdi.  */
/* The α and redo entries are distinct labels but the redo entry simply falls into the body's β path: a CP- */
/* bearing predicate (IR_CHOICE/IR_GOAL body) re-drives via its own β; a deterministic body has no live CP  */
/* so a redo immediately fails to ω (set_last_ok(0); ret), which bb_goal's β handles (resolve_cp_current    */
/* NULL -> ω).                                                                                              */
static int codegen_pl_callee_block(IR_graph_t *g, const char *name, int arity, FILE *out) {
    if (!g || !g->entry || !name) return 1;
    /* PLG-9d-bt: operand_aux is keyed by (graph, node); set g_emit_cfg to THIS callee's graph so a DISJ   */
    /* (or any operand_aux read) inside the clause body resolves against the body that owns it.            */
    IR_graph_t *save_cfg = g_emit_cfg; g_emit_cfg = g;
    /* The clause graph's entry is the FIRST body element (e.g. the first head-unify), not the conjunction  */
    /* wrapper — lower2_clause_body_entry returns entry[0] as α while the IR_GCONJ wraps ALL head-unifies +  */
    /* body goals. Walking the raw entry emits ONLY the first element (its γ wired straight to γ), dropping  */
    /* the rest (a 2-arg fact would unify only arg0). So pick the body ROOT the same way the main arm does:  */
    /* the principal IR_GCONJ (flat_drive_pl_seq walks its goals[] in order). For an IR_CHOICE-headed pred   */
    /* (multi-clause) the entry IS the choice node (no GCONJ wrapper); for a bare single-element body the    */
    /* entry is that element. Choose: the GCONJ whose first goal == g->entry (the wrapper for THIS body),    */
    /* else g->entry.                                                                                        */
    IR_t *body_root = g->entry;
    if (g->body_root) {
        /* PLG-9d-bt: the lowerer recorded the clause-body root (the top GCONJ) on the graph. Prefer it —  */
        /* the goals[0]==entry scan below is ambiguous when the body has a top-level disjunction (the DISJ */
        /* and its left-arm GCONJ share goals[0]==entry), and walking the wrong one drops an arm.          */
        body_root = g->body_root;
    } else if (g->all) {
        for (int i = 0; i < g->n; i++) {
            IR_t *nd = g->all[i];
            if (!nd || nd->t != IR_GCONJ) continue;
            bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)nd->ival;
            if (zs && zs->goals && zs->ngoals > 0 && zs->goals[0] == g->entry) { body_root = nd; break; }
        }
    }
    char blbl[160]; resolve_call_block_label(blbl, sizeof blbl, name, arity);
    char redo_lbl[200]; snprintf(redo_lbl, sizeof redo_lbl, "%s_redo", blbl);
    g_child_cache_n = 0;
    g_text_child_counter = 0;
    pre_build_children_text(body_root, out, blbl);
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    /* labels: the α entry (bb_goal `call`s it), the β redo entry, and the γ/ω body sinks. */
    bb_label_t *lbl_α    = emit_label_intern(blbl);
    bb_label_t *lbl_redo = emit_label_intern(redo_lbl);
    bb_label_t lbl_γ, lbl_ω, lbl_β;
    emit_label_initf(&lbl_γ, "%s_γ", blbl);
    emit_label_initf(&lbl_ω, "%s_ω", blbl);
    emit_label_initf(&lbl_β, "%s_β", blbl);
    g_emit.flat_succ_p = &lbl_γ;
    g_emit.flat_fail_p = &lbl_ω;
    g_emit.flat_β_p    = &lbl_β;
    /* α: fresh entry. Body's α IS this label (predecessor call lands here). The body graph's entry node    */
    /* gets its own α label from walk_bb_flat; we define .Lplpred_… right before so the call resolves.      */
    emit_label_define_bb(lbl_α);
    walk_bb_flat(body_root, &lbl_γ, &lbl_ω, &lbl_β);
    /* redo: β re-entry. Jumps to the body's β (lbl_β, DEFINED by the walk above — the body's outermost     */
    /* resume edge targets it), which a CP-bearing body uses to re-drive its CP; a deterministic body's β   */
    /* chains to ω -> set_last_ok(0); ret.                                                                   */
    emit_label_define_bb(lbl_redo);
    emit_jmp_label(&lbl_β, JMP_JMP);
    /* γ: body succeeded → rt_set_last_ok(1); ret. */
    emit_label_define_bb(&lbl_γ);
    { const char *s = "  mov edi, 1\n  call rt_set_last_ok@PLT\n  ret\n"; emit_text_n(s, strlen(s)); }
    /* ω: body failed / exhausted → rt_set_last_ok(0); ret. */
    emit_label_define_bb(&lbl_ω);
    { const char *s = "  mov edi, 0\n  call rt_set_last_ok@PLT\n  ret\n"; emit_text_n(s, strlen(s)); }
    emitter_end();
    g_emit_cfg = save_cfg;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* codegen_pl_program — PLG-9d driver entry. Emit every registered Prolog predicate's callee block (so the */
/* .Lplpred_<name>_<arity> labels bb_goal.cpp calls are defined), then return 0. main_α is emitted          */
/* separately by the scrip.c arm via codegen_flat_build (unchanged from PLG-9c). Predicate iteration is the */
/* resolve_bb_* table (resolve_bb_pred_count / _name_at / _arity_at / resolve_bb_graph_at). A predicate     */
/* whose body has an unemittable node was already rejected by pl_rich_body_root (the caller's gate), so by  */
/* the time we get here every block is emittable.                                                           */
extern int resolve_bb_pred_count(void);
extern const char *resolve_bb_pred_name_at(int idx);
extern int resolve_bb_pred_arity_at(int idx);
extern IR_graph_t *resolve_bb_graph_at(int idx);
int codegen_pl_program(FILE *out) {
    int npred = resolve_bb_pred_count();
    for (int i = 0; i < npred; i++) {
        const char *nm = resolve_bb_pred_name_at(i);
        if (!nm) continue;
        int ar = resolve_bb_pred_arity_at(i);
        /* main is emitted separately by the driver as main_α (the C-ABI entry); skip it here so we do not  */
        /* also emit a redundant .Lplpred_main_0 block (nothing calls it, and it would double-walk the main */
        /* graph). The registry stores the FULL key "name/arity" as the name, so match "main/" prefix (or   */
        /* exact "main"). Every OTHER predicate is a callee reached via bb_goal's .Lplpred_<name>_<arity>.   */
        if (ar == 0 && (strcmp(nm, "main") == 0 || strcmp(nm, "main/0") == 0)) continue;
        IR_graph_t *pg = resolve_bb_graph_at(i);
        if (!pg) continue;
        int rc = codegen_pl_callee_block(pg, nm, ar, out);
        if (rc) return rc;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void walk_bb_register_child_label(IR_t *nd, const char *α_label) {
    bb_box_fn fn = child_cache_get(nd);
    if (fn) child_cache_set_lbl(fn, α_label);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern bb_mode_t g_bb_mode;
bb_box_fn bb_build_pure_mode(IR_t *nd) {
    if (g_bb_mode == BB_MODE_LIVE) return bb_build_flat(nd);
    return bb_build_brokered(nd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_flat_reset(void) { g_flat_slot_count = 0; g_flat_node_id = 0; }

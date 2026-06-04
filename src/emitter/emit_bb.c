#include "emit_bb.h"
#include "BB_templates/bb_templates.h"
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
#include <gc/gc.h>
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
int g_frame_active = 0;
int g_icn_scan_regs_live = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
#define FLAT_CHAIN_SET_MAX 512
static IR_t *g_flat_chain_set[FLAT_CHAIN_SET_MAX];
static int   g_flat_chain_set_n = 0;
static int flat_chain_set_has(IR_t *nd) {
    for (int i = 0; i < g_flat_chain_set_n; i++) if (g_flat_chain_set[i] == nd) return 1;
    return 0;
}
int                 g_subject_slot       = -1;
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
extern int memcmp(const void *, const void *, size_t);
static bb_label_t g_α_ring[8];
static int        g_α_ring_i = 0;
static int        g_bb_alpha_seq = 0;
static void bb_fill_alpha(IR_t *nd) {
    extern int g_sno_m4_dense_nid;
    bb_label_t *a = &g_α_ring[g_α_ring_i++ & 7];
    if (g_sno_m4_dense_nid) emit_label_initf(a, "bb%d_α", ++g_bb_alpha_seq);
    else                    emit_label_initf(a, "bb%d_α", nd ? bb_node_id(nd) : 0);
    g_emit.lbl_α   = a->name;
    g_emit.lbl_α_p = a;
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
int resolve_seq_goals_em(const IR_t *nd, IR_t **out, int max) {
    if (!nd || nd->t != IR_GCONJ) return 0;
    bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)nd->ival;
    if (!zs || !zs->goals) return 0;
    int k = zs->ngoals < max ? zs->ngoals : max;
    for (int i = 0; i < k; i++) out[i] = zs->goals[i];
    return k;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int resolve_choice_bodies_em(const IR_t *nd, IR_t **out, int max) {
    if (!nd || nd->t != IR_CHOICE) return 0;
    bb_choice_state_t *zc = (bb_choice_state_t *)(intptr_t)nd->ival;
    if (!zc || !zc->bodies) return 0;
    int k = zc->nbodies < max ? zc->nbodies : max;
    for (int i = 0; i < k; i++)
        out[i] = zc->bodies[i] ? (zc->bodies[i]->body_root ? zc->bodies[i]->body_root : zc->bodies[i]->entry) : NULL;
    return k;
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
    if (!nd || nd->t != IR_ITE) return 0;
    bb_ite_state_t *zi = (bb_ite_state_t *)(intptr_t)nd->ival;
    if (!zi) return 0;
    if (out_cond) *out_cond = zi->cond;
    if (out_then) *out_then = zi->then_;
    if (out_else) *out_else = zi->else_;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t *ite_branch_walk_node(IR_t *entry, IR_t *root) {
    if (root && bb_kind_is_driver_owned(root->t)) return root;
    return entry;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_ite(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    IR_t *cond = NULL, *thn = NULL, *els = NULL;
    if (!resolve_ite_entries_em(pBB, &cond, &thn, &els) || !cond) {
        EMIT_PAIR_RESET(); EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); return;
    }
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
void bb_prepare(IR_t *nd) {
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
        g_emit.bb_lk = (int)nd->α->t; g_emit.bb_li = (int64_t)nd->α->ival;
        g_emit.bb_rk = (int)nd->β->t; g_emit.bb_ri = (int64_t)nd->β->ival;
        g_emit.bb_ln = (void *)nd->α; g_emit.bb_rn = (void *)nd->β;
        if (nd->α->t == IR_ATOM) g_emit.bb_ls = bb_intern_into(g_emit.bb_ls_buf, nd->α->sval ? nd->α->sval : "");
        if (nd->β->t == IR_ATOM) g_emit.bb_rs = bb_intern_into(g_emit.bb_rs_buf, nd->β->sval ? nd->β->sval : "");
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
/*--------------------------------------------------------------------------------------------------------------------*/
static bb_label_t *seq_node_label(IR_t **nodes, bb_label_t **lbls, int n, IR_t *tgt, bb_label_t *falloff) {
    if (!tgt) return falloff;
    for (int i = 0; i < n; i++) if (nodes[i] == tgt) return lbls[i];
    return falloff;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_e binop_slot_kind(IR_t *nd) {
    int64_t op = nd ? nd->ival : -1;
    if (op >= BINOP_LT && op <= BINOP_NE) return IR_BINOP_RELOP;
    if (op == BINOP_CONCAT)               return IR_BINOP_CONCAT;
    if (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD) return IR_BINOP_ARITH;
    return IR_BINOP;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_binop_tree(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α || !pBB->β) {
        fprintf(stderr, "[IBB] FATAL flat_drive_binop_tree: missing α or β child\n");
        abort();
    }
    if ((pBB->α->t == IR_LIT_I && pBB->β->t == IR_LIT_I)
        || (pBB->α->t == IR_LIT_S && pBB->β->t == IR_LIT_S && pBB->ival == BINOP_CONCAT)) {
        EMIT_PAIR_RESET();
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        { IR_e _sk = pBB->t; pBB->t = binop_slot_kind(pBB); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); pBB->t = _sk; }
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
    { IR_e _sk = pBB->t; pBB->t = binop_slot_kind(pBB); EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β); pBB->t = _sk; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
    g_emit.op_sa  = bb_slot_get(pBB->α);
    g_emit.op_sb  = bb_slot_claim(8);
    g_emit.op_off = bb_slot_alloc16(pBB);
    FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_to(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB || !pBB->α || !pBB->β) {
        fprintf(stderr, "[IBB] FATAL flat_drive_to: missing from (α) or to (β) operand\n");
        abort();
    }
    g_emit.op_sa  = bb_slot_get(pBB->α);
    g_emit.op_sb  = bb_slot_get(pBB->β);
    g_emit.op_off = bb_slot_alloc16(pBB);
    (void)bb_slot_claim(8);
    FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_alt_icn_gen(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!pBB) {
        fprintf(stderr, "[IBB] FATAL flat_drive_alt_icn_gen: null node\n");
        abort();
    }
    g_emit.node   = pBB;
    g_emit.op_off = bb_slot_alloc16(pBB);
    (void)bb_slot_claim(8);
    FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
    walk_bb_flat(pBB->α, got_value, lbl_ω, gen_resume);
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
    if (pBB && pBB->α) {
        int id = g_flat_node_id++;
        bb_label_t *expr_done = emit_label_alloc("xreturn%d_expr_done", id);
        bb_label_t *expr_β    = emit_label_alloc("xreturn%d_expr_β",    id);
        walk_bb_flat(pBB->α, expr_done, lbl_ω, expr_β);
        emit_label_define_bb(expr_done);
    }
    emit_jmp_label((pBB && pBB->dval == 2.0) ? slab_fail : slab_succ, JMP_JMP);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
static void flat_drive_userproc(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int nargs = (int)(pBB ? pBB->ival : 0);
    IR_graph_t **blks = pBB ? (IR_graph_t **)(intptr_t) pBB->counter : NULL;
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
    while (entry && (entry->t == IR_SUCCEED || entry->t == IR_FAIL) && entry->γ && guard++ < 512) entry = entry->γ;
    IR_t *last = entry;
    guard = 0;
    while (last && last->γ && last->γ->t != IR_SUCCEED && last->γ->t != IR_FAIL && guard++ < 512) last = last->γ;
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
    IR_graph_t *subj_sg = (pBB && pBB->dval == 1.0) ? (IR_graph_t *)(intptr_t) pBB->counter : NULL;
    IR_graph_t *body_sg = (pBB && pBB->dval == 1.0) ? (IR_graph_t *)(intptr_t) pBB->ival    : NULL;
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
    int saved_scan_regs_live = g_icn_scan_regs_live;
    g_icn_scan_regs_live = 1;
    flat_emit_arg_subchain(body_sg->entry, body_done, body_fail);
    g_icn_scan_regs_live = saved_scan_regs_live;
    emit_label_define_bb(body_done);
    flat_drive_scan_glue(pBB, 2, -1, regs_off, lbl_γ, lbl_ω, leaveok_β);
    emit_label_define_bb(body_fail);
    flat_drive_scan_glue(pBB, 2, -1, regs_off, lbl_ω, lbl_ω, leavef_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static int ir_is_generator_kind(IR_e t) {
    switch (t) {
        case IR_TO: case IR_TO_BY: case IR_UPTO: case IR_ALT:
        case IR_BINOP_GEN: case IR_ITERATE: case IR_LIMIT: case IR_PROC_GEN:
        case IR_LIST_BANG: case IR_KEY_GEN: case IR_FIND_GEN: case IR_SEQ_GEN:
        case IR_GATHER:
            return 1;
        default: return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int call_args_single_shot(IR_t *pBB) {
    int nargs = (int)(pBB ? pBB->ival : 0);
    IR_t *ax = pBB ? pBB->α : NULL;
    for (int j = 0; j < nargs && ax; j++, ax = ax->γ)
        if (gen_bb_is_gen_arg(ax)) return 0;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_gvar_assign(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
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
    if (!pBB || !pBB->α || pBB->α->t != IR_BINOP) {
        fprintf(stderr, "[SBB] FATAL flat_drive_gvar_assign_binop: rhs is not IR_BINOP\n");
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
/*--------------------------------------------------------------------------------------------------------------------*/
static int scan_pat_is_single_lit(IR_graph_t *pg) {
    if (!pg || !pg->entry || pg->entry->t != IR_PAT_LIT) return 0;
    int nlit = 0;
    for (int i = 0; i < pg->n; i++) {
        IR_e t = pg->all[i]->t;
        if (t == IR_SUCCEED || t == IR_FAIL) continue;
        if (t == IR_PAT_LIT) { nlit++; continue; }
        return 0;
    }
    return nlit == 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char * scan_pat_cat_concat(IR_graph_t *pg) {
    if (!pg || !pg->entry || pg->entry->t != IR_PAT_LIT) return NULL;
    int nlit = 0, ncat = 0;
    for (int i = 0; i < pg->n; i++) {
        IR_e t = pg->all[i]->t;
        if (t == IR_SUCCEED || t == IR_FAIL) continue;
        if (t == IR_PAT_LIT) { nlit++; continue; }
        if (t == IR_PAT_CAT) { ncat++; continue; }
        return NULL;
    }
    if (nlit < 2 || ncat < 1) return NULL;
    size_t total = 0;
    for (IR_t *c = pg->entry; c && c->t == IR_PAT_LIT; c = c->γ) total += c->sval ? strlen(c->sval) : 0;
    char *buf = (char *)GC_MALLOC_ATOMIC(total + 1);
    size_t off = 0;
    for (IR_t *c = pg->entry; c && c->t == IR_PAT_LIT; c = c->γ) { const char *s = c->sval ? c->sval : ""; size_t n = strlen(s); memcpy(buf + off, s, n); off += n; }
    buf[off] = 0;
    return buf;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int scan_val_is_single_lit(IR_graph_t *g) {
    if (!g || !g->entry || g->entry->t != IR_LIT_S) return 0;
    int nlit = 0;
    for (int i = 0; i < g->n; i++) {
        IR_e t = g->all[i]->t;
        if (t == IR_SUCCEED || t == IR_FAIL) continue;
        if (t == IR_LIT_S) { nlit++; continue; }
        return 0;
    }
    return nlit == 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_scan_stmt(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    int n_aux = 0;
    IR_t * const * aux = bb_operand_aux_get(g_emit_cfg, pBB, &n_aux);
    g_emit.op_scan_pat  = pBB ? pBB->counter : 0;
    g_emit.op_scan_subj = (n_aux > 0 && aux) ? (int64_t)(intptr_t)aux[0] : 0;
    g_emit.op_scan_repl = (n_aux > 1 && aux) ? (int64_t)(intptr_t)aux[1] : 0;
    g_emit.op_scan_pat_lit = NULL; g_emit.op_scan_subj_lit = NULL; g_emit.op_scan_replace_lit = NULL;
    {
        IR_graph_t * pg = (IR_graph_t *)(intptr_t)g_emit.op_scan_pat;
        IR_graph_t * sg = (IR_graph_t *)(intptr_t)g_emit.op_scan_subj;
        IR_graph_t * rg = (IR_graph_t *)(intptr_t)g_emit.op_scan_repl;
        if (scan_pat_is_single_lit(pg))                    g_emit.op_scan_pat_lit  = pg->entry->sval ? pg->entry->sval : "";
        else { const char * cc = scan_pat_cat_concat(pg); if (cc)  g_emit.op_scan_pat_lit  = cc; }
        if (scan_val_is_single_lit(sg))                    g_emit.op_scan_subj_lit = sg->entry->sval ? sg->entry->sval : "";
        if (scan_val_is_single_lit(rg))                    g_emit.op_scan_replace_lit = rg->entry->sval ? rg->entry->sval : "";
    }
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_subject(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_ref_invariant(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
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
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_match(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
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
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_program(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    prog_t *prog = (prog_t *)(intptr_t)pBB->ival;
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
static void flat_drive_icn_global_assign(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    g_emit.op_sa  = (pBB && pBB->α) ? bb_slot_get(pBB->α) : -1;
    g_emit.op_off = bb_slot_alloc16(pBB);
    FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
    if (flat_chain_set_has(pBB->α)) {
        EMIT_PAIR_RESET();
        EMIT_PAIR_JMP(lbl_γ);
        EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
        EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
        return;
    }
    int id = g_flat_node_id++;
    bb_label_t *body_β = emit_label_alloc("xevery%d_body_β", id);
    walk_bb_flat(pBB->α, body_β, lbl_γ, body_β);
    EMIT_PAIR_RESET();
    EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
    EMIT_PAIR_FILL(pBB, lbl_γ, lbl_ω, lbl_β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static int while_cond_emittable(IR_t *cond) {
    return cond && cond->t == IR_BINOP && cond->state >= 1 &&
           while_operand_simple(cond->α) && while_operand_simple(cond->β);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_gen_alt(IR_t *pBB, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
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
    case IR_GCONJ:     flat_drive_conj(nd, lbl_γ, lbl_ω, lbl_β); break;
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
    case IR_LIT_I:      if (g_descr_flat_chain) g_emit.op_off = bb_slot_alloc16(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIT_S:      if (g_descr_flat_chain) g_emit.op_off = bb_slot_alloc16(nd); FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIT_F:
    case IR_LIT_NUL:    FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_CALL: {
        IR_t *a0 = nd->α;
        if (g_descr_flat_chain) {
            if (g_icn_scan_regs_live && nd->dval == 3.0 && nd->sval && !strcmp(nd->sval, "pos")) {
                IR_graph_t **sblks = (IR_graph_t **)(intptr_t) nd->counter;
                long sn = (sblks && (int)nd->ival == 1 && sblks[0] && sblks[0]->entry && sblks[0]->entry->t == IR_LIT_I) ? (long) sblks[0]->entry->ival : -1;
                g_emit.op_sb  = (int) sn;
                g_emit.op_sa  = -1;
                g_emit.op_off = bb_slot_alloc16(nd);
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
                break;
            }
            if (g_icn_scan_regs_live && nd->dval == 3.0 && nd->sval && (!strcmp(nd->sval, "any") || !strcmp(nd->sval, "match"))) {
                IR_graph_t **sblks = (IR_graph_t **)(intptr_t) nd->counter;
                const char *cs = (sblks && (int)nd->ival == 1 && sblks[0] && sblks[0]->entry && sblks[0]->entry->t == IR_LIT_S) ? sblks[0]->entry->sval : (const char *)0;
                g_emit.op_name1 = cs;
                g_emit.op_sa  = -1;
                g_emit.op_sb  = -1;
                g_emit.op_off = bb_slot_alloc16(nd);
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
                break;
            }
            if (nd->dval == 3.0 && (int)nd->ival > 0 && nd->sval && rt_proc_is_registered(nd->sval))
                flat_drive_userproc(nd, lbl_γ, lbl_ω, lbl_β);
            else
                FILL(nd, lbl_γ, lbl_ω, lbl_β);
            break;
        }
        if (g_gvar_flat_chain && nd->dval == 2.0) { FILL(nd, lbl_γ, lbl_ω, lbl_β); break; }
        if (g_gvar_flat_chain && nd->dval == 3.0) {
            IR_graph_t **csubs = (IR_graph_t **)(intptr_t)nd->counter;
            for (int ci = 0; ci < (int)nd->ival; ci++) if (csubs && csubs[ci] && csubs[ci]->entry) gvar_stmt_operand_refs(csubs[ci]->entry);
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
        g_emit.op_off = -1;
        if (g_gvar_flat_chain && op_is_arith && nd->α && nd->β && nd->α->t == IR_LIT_I && nd->β->t == IR_LIT_I) {
            g_emit.op_sa  = (int)nd->α->ival;
            g_emit.op_sb  = (int)nd->β->ival;
            g_emit.op_off = bb_slot_alloc(nd);
            g_emit.op_name1 = (const char *)0;
            g_emit.op_name2 = (const char *)0;
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->t; nd->t = IR_BINOP_GVAR_ARITH; EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->t = _sk; }
        } else if (g_gvar_flat_chain && op_is_arith && nd->α && nd->β && nd->α->t == IR_VAR && nd->β->t == IR_VAR && nd->α->sval && nd->β->sval) {
            g_emit.op_name1 = nd->α->sval;
            g_emit.op_name2 = nd->β->sval;
            g_emit.op_off   = bb_slot_alloc(nd);
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->t; nd->t = IR_BINOP_GVAR_ARITH; EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->t = _sk; }
        } else if (g_gvar_flat_chain && op_is_arith && nd->α && nd->β && nd->α->t == IR_VAR && nd->α->sval && nd->β->t == IR_LIT_I) {
            g_emit.op_name1 = nd->α->sval;
            g_emit.op_name2 = (const char *)0;
            g_emit.op_sb    = (int)nd->β->ival;
            g_emit.op_off   = bb_slot_alloc(nd);
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->t; nd->t = IR_BINOP_GVAR_ARITH; EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->t = _sk; }
        } else if (g_gvar_flat_chain && op_is_arith && nd->α && nd->β && nd->α->t == IR_LIT_I && nd->β->t == IR_VAR && nd->β->sval) {
            g_emit.op_name1 = (const char *)0;
            g_emit.op_name2 = nd->β->sval;
            g_emit.op_sa    = (int)nd->α->ival;
            g_emit.op_off   = bb_slot_alloc(nd);
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->t; nd->t = IR_BINOP_GVAR_ARITH; EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->t = _sk; }
        } else if (g_gvar_flat_chain && op_is_rel && nd->α && nd->β &&
                   ((nd->α->t == IR_LIT_I) || (nd->α->t == IR_VAR && nd->α->sval) || bb_slot_get(nd->α) >= 0) &&
                   ((nd->β->t == IR_LIT_I) || (nd->β->t == IR_VAR && nd->β->sval) || bb_slot_get(nd->β) >= 0)) {
            g_emit.bb_lk    = (int)nd->α->t;
            g_emit.bb_rk    = (int)nd->β->t;
            g_emit.bb_li    = (nd->α->t == IR_LIT_I) ? nd->α->ival : 0;
            g_emit.bb_ri    = (nd->β->t == IR_LIT_I) ? nd->β->ival : 0;
            g_emit.op_name1 = (nd->α->t == IR_VAR) ? nd->α->sval : (const char *)0;
            g_emit.op_name2 = (nd->β->t == IR_VAR) ? nd->β->sval : (const char *)0;
            g_emit.op_sa    = (nd->α->t != IR_LIT_I && nd->α->t != IR_VAR) ? bb_slot_get(nd->α) : -1;
            g_emit.op_sb    = (nd->β->t != IR_LIT_I && nd->β->t != IR_VAR) ? bb_slot_get(nd->β) : -1;
            g_emit.op_off   = bb_slot_alloc(nd);
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->t; nd->t = IR_BINOP_GVAR_RELOP; EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->t = _sk; }
        } else if (g_descr_flat_chain && (op_is_rel || op_is_arith || op_is_concat)) {
            if (op_is_arith || op_is_rel || op_is_concat) {
                g_emit.op_sa = bb_slot_get(nd->α);
                g_emit.op_sb = bb_slot_get(nd->β);
                if (g_emit.op_sa >= 0 && g_emit.op_sb >= 0) g_emit.op_off = bb_slot_alloc16(nd);
            }
            EMIT_PAIR_RESET();
            EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω);
            { IR_e _sk = nd->t; nd->t = binop_slot_kind(nd); EMIT_PAIR_FILL(nd, lbl_γ, lbl_ω, lbl_β); nd->t = _sk; }
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
        if (g_gvar_flat_chain) {
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
        if (nd->α && nd->β && !binop_operand_streams(nd->α) && !binop_operand_streams(nd->β)) {
            IR_e saved_kind = nd->t;
            nd->t = IR_BINOP;
            flat_drive_binop_tree(nd, lbl_γ, lbl_ω, lbl_β);
            nd->t = saved_kind;
        } else {
            flat_drive_binop_gen_tree(nd, lbl_γ, lbl_ω, lbl_β);
        }
        break;
    case IR_SEQ:        if (g_gvar_flat_chain && nd && nd->dval == 1.0) { flat_drive_gvar_seq_passthrough(nd, lbl_γ, lbl_ω, lbl_β); } else flat_drive_seq(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SEQ_EXPR:   flat_drive_seq(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_EVERY:      flat_drive_every(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIMIT:      flat_drive_limit(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_TO:         flat_drive_to(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_GATHER:     FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_TO_BY:      flat_drive_to(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_ALT:        if (g_descr_flat_chain) flat_drive_alt_icn_gen(nd, lbl_γ, lbl_ω, lbl_β); else flat_drive_gen_alt(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_VAR:        if (g_descr_flat_chain && nd && nd->sval && nd->sval[0] == '&') { g_emit.op_sval = nd->sval; g_emit.op_sa = -1; g_emit.op_off = bb_slot_alloc16(nd); } else if (g_descr_flat_chain && nd && nd->sval) { extern int g_icn_globals_nv; if (nd->state == 1 && g_icn_globals_nv) { g_emit.op_sa = -1; g_emit.op_off = bb_slot_alloc16(nd); } else { int voff = bb_varslot_peek(nd->sval); g_emit.op_sa = voff; g_emit.op_off = (voff >= 0) ? bb_slot_alloc16(nd) : -1; } } else { g_emit.op_sa = -1; g_emit.op_off = -1; } FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_ASSIGN:     if (g_descr_flat_chain) { extern int g_icn_globals_nv; extern int is_global(const char *); if (g_icn_globals_nv && nd->sval && is_global(nd->sval)) flat_drive_icn_global_assign(nd, lbl_γ, lbl_ω, lbl_β); else FILL(nd, lbl_γ, lbl_ω, lbl_β); } else if (nd->sval && nd->α && (nd->α->t == IR_LIT_S || nd->α->t == IR_LIT_I || nd->α->t == IR_VAR || nd->α->t == IR_SEQ || nd->α->t == IR_SEQ_EXPR || nd->α->t == IR_CALL)) flat_drive_gvar_assign(nd, lbl_γ, lbl_ω, lbl_β); else if (nd->sval && nd->α && nd->α->t == IR_BINOP) flat_drive_gvar_assign_binop(nd, lbl_γ, lbl_ω, lbl_β); else flat_drive_assign(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KEYWORD:    if (g_descr_flat_chain) { g_emit.op_sval = nd->sval; g_emit.op_off = bb_slot_alloc16(nd); } else { g_emit.op_off = -1; } FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_GEN_SCAN:   flat_drive_gen_scan(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SCAN:       flat_drive_scan_stmt(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SUBJECT:    flat_drive_subject(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_REF_INVARIANT: flat_drive_ref_invariant(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PAT_MATCH:  flat_drive_match(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_PROG:   flat_drive_program(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_RETURN:
        if (g_descr_flat_chain) { FILL(nd, lbl_γ, lbl_ω, lbl_β); break; }
        flat_drive_return(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SWAP:       flat_drive_swap(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_WHILE:
    case IR_UNTIL:
        if (g_gvar_flat_chain) {
            emit_label_define_bb(lbl_β);
            emit_jmp_label(lbl_γ, JMP_JMP);
            emit_jmp_label(lbl_γ, JMP_JMP);
        } else if (while_cond_emittable(nd->α)) {
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
    if (!(nd->γ && nd->γ->t == IR_ALT)) return 0;
    int na = 0;
    IR_t * const * arms = bb_operand_aux_get(g_emit_cfg, nd->γ, &na);
    for (int i = 0; i < na && arms; i++) if (arms[i] == nd) return 1;
    return 0;
}
static IR_t *ir_skip_alt_arms(IR_t *entry) {
    int guard = 0;
    while (entry && ir_node_is_alt_arm(entry) && guard++ < 512) entry = entry->γ;
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
    { int guard = 0; while (entry && (entry->t == IR_SUCCEED || entry->t == IR_FAIL) && entry->γ && guard++ < CH_MAX) entry = entry->γ; }
    entry = ir_skip_alt_arms(entry);
    queue[qt++] = entry;
    while (qh < qt) {
        IR_t *c = queue[qh++];
        if (!c || c->t == IR_SUCCEED || c->t == IR_FAIL) continue;
        if (ir_node_is_alt_arm(c)) continue;
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); }
        nodes[n++] = c;
        if (c->γ && qt < CH_MAX) queue[qt++] = c->γ;
        if ((c->t == IR_BINOP || c->t == IR_BINOP_GEN) && c->ω && qt < CH_MAX) queue[qt++] = c->ω;
        if (c->t == IR_CALL && c->ω && qt < CH_MAX) queue[qt++] = c->ω;
        if (c->t == IR_GATHER && c->ω && qt < CH_MAX) queue[qt++] = c->ω;
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
        for (int k = 0; k < n; k++) if (nodes[k] == nodes[i]->γ) {
            node_γ = (i > k && ir_is_generator_kind(nodes[k]->t)) ? betas[k] : lbls[k];
            break;
        }
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static void pre_build_children(IR_t *nd) {
    if (!nd) return;
    if (nd->t == IR_REF_INVARIANT) {
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
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static int descr_chain_arity(const IR_t *n) {
    switch (n->t) {
    case IR_LIT_I: case IR_LIT_S: case IR_LIT_F: case IR_LIT_NUL:
    case IR_VAR:   case IR_KEYWORD: return 0;
    case IR_ALT:   return 0;
    case IR_GATHER: return 0;
    case IR_BINOP: case IR_BINOP_GEN: case IR_TO: case IR_TO_BY: return 2;
    case IR_UNOP:  case IR_NEG: case IR_POS: case IR_NONNULL: case IR_NOT: case IR_SIZE: return 1;
    case IR_ASSIGN: return 1;
    case IR_RETURN: return 1;
    case IR_CALL:  return (n->dval == 2.0 || n->dval == 3.0) ? 0 : (int)n->ival;
    default:       return -1;
    }
}
static void descr_chain_operand_refs(IR_t *entry) {
    IR_t *chain[512]; int nc = 0;
    IR_t *seen[512]; int ns = 0;
    IR_t *stkv[512]; int sv = 0;
    { int guard = 0; while (entry && (entry->t == IR_SUCCEED || entry->t == IR_FAIL) && entry->γ && guard++ < 512) entry = entry->γ; }
    stkv[sv++] = entry;
    while (sv > 0 && nc < 512) {
        IR_t *c = stkv[--sv];
        if (!c || c->t == IR_SUCCEED || c->t == IR_FAIL) continue;
        int dup = 0; for (int i = 0; i < ns; i++) if (seen[i] == c) { dup = 1; break; }
        if (dup) continue;
        seen[ns++] = c; chain[nc++] = c;
        if ((c->t == IR_BINOP || c->t == IR_BINOP_GEN) && c->ω && sv < 512) stkv[sv++] = c->ω;
        if (c->t == IR_CALL && c->ω && sv < 512) stkv[sv++] = c->ω;
        if (c->t == IR_GATHER && c->ω && sv < 512) stkv[sv++] = c->ω;
        if (c->γ && sv < 512) stkv[sv++] = c->γ;
    }
    IR_t *stk[512]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = descr_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        if (ar == 2 && sp >= 2) { n->β = stk[sp - 1]; n->α = stk[sp - 2]; sp -= 2; }
        else if (ar == 1 && sp >= 1) { n->α = stk[sp - 1]; sp -= 1; }
        else if (ar >= 1) { sp = 0; }
        stk[sp++] = n;
    }
}
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
    snprintf(prefix, sizeof(prefix), "icn_proc_%s", pname);
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
    while (n && n->t == IR_SUCCEED && n->γ != NULL && guard++ < 4096) n = n->γ;
    return n;
}
static int gvar_chain_is_real(IR_t *n) { return n && n->t != IR_SUCCEED && n->t != IR_FAIL; }
static int gvar_chain_arity(const IR_t *n) {
    if (n && (n->t == IR_SEQ || n->t == IR_SEQ_EXPR) && n->dval == 1.0) return 0;
    if (n && n->t == IR_SCAN) return 1;
    if (n && n->t == IR_RETURN && (n->dval == 1.0 || n->dval == 2.0)) return 0;
    return descr_chain_arity(n);
}
static void gvar_stmt_operand_refs(IR_t *head) {
    IR_t *chain[512]; int nc = 0;
    IR_t *c = head;
    while (gvar_chain_is_real(c) && nc < 512) {
        int dup = 0; for (int i = 0; i < nc; i++) if (chain[i] == c) { dup = 1; break; }
        if (dup) break;
        chain[nc++] = c;
        IR_t *g = c->γ;
        if (!g || g->t == IR_SUCCEED || g->t == IR_FAIL) break;
        c = g;
    }
    IR_t *stk[512]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = gvar_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        if (ar == 2 && sp >= 2) { n->β = stk[sp - 1]; n->α = stk[sp - 2]; sp -= 2; }
        else if (ar == 1 && sp >= 1) { n->α = stk[sp - 1]; sp -= 1; }
        else if (ar >= 1) { sp = 0; }
        stk[sp++] = n;
    }
}
static void gvar_chain_prebuild_children(IR_graph_t *g) {
    if (!g || !g->all) return;
    for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->t == IR_REF_INVARIANT) pre_build_children(g->all[i]);
}
static void gvar_chain_prebuild_children_text(IR_graph_t *g, FILE *out, const char *prefix) {
    if (!g || !g->all) return;
    for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->t == IR_REF_INVARIANT) pre_build_children_text(g->all[i], out, prefix);
}
static void gvar_chain_operand_refs(IR_graph_t *g) {
    if (!g || !g->all) return;
    IR_t *heads[2048]; int nh = 0;
    IR_t *e0 = gvar_chain_resolve(g->entry);
    if (gvar_chain_is_real(e0)) heads[nh++] = e0;
    for (int i = 0; i < g->n && nh < 2048; i++) {
        IR_t *L = g->all[i];
        if (!L || L->t != IR_SUCCEED || L->γ == NULL) continue;
        IR_t *h = gvar_chain_resolve(L);
        if (!gvar_chain_is_real(h)) continue;
        int dup = 0; for (int k = 0; k < nh; k++) if (heads[k] == h) { dup = 1; break; }
        if (!dup) heads[nh++] = h;
    }
    for (int i = 0; i < g->n && nh < 2048; i++) {
        IR_t *L = g->all[i];
        if (!L || !L->ω) continue;
        IR_t *h = gvar_chain_resolve(L->ω);
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
    IR_t *e0 = gvar_chain_resolve(entry);
    if (gvar_chain_is_real(e0)) queue[qt++] = e0;
    while (qh < qt) {
        IR_t *c = queue[qh++];
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= CH_MAX) { fprintf(stderr, "[SBB] FATAL sno chain exceeds CH_MAX\n"); abort(); }
        nodes[n++] = c;
        IR_t *g = gvar_chain_resolve(c->γ);
        IR_t *w = gvar_chain_resolve(c->ω);
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
        IR_t *g = gvar_chain_resolve(nodes[i]->γ);
        IR_t *w = gvar_chain_resolve(nodes[i]->ω);
        if (gvar_chain_is_real(g)) { for (int k = 0; k < n; k++) if (nodes[k] == g) { node_γ = lbls[k]; break; } }
        else if (g && g->t == IR_FAIL) node_γ = &lbl_ω;
        if (gvar_chain_is_real(w)) { for (int k = 0; k < n; k++) if (nodes[k] == w) { node_ω = lbls[k]; break; } }
        else if (w && w->t == IR_FAIL) node_ω = &lbl_ω;
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
    int has_ref = 0; for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->t == IR_REF_INVARIANT) { has_ref = 1; break; }
    if (has_ref && !g_in_prebuild) { g_child_cache_n = 0; g_in_prebuild = 1; gvar_chain_prebuild_children(g); g_in_prebuild = 0; }
    gvar_chain_operand_refs(g);
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) { g_emit_cfg = save_cfg; return NULL; }
    g_flat_slot_count = 0; g_flat_node_id = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    g_gvar_flat_chain = 1;
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_gvar_flat_chain_body(g->entry, "sno_flat");
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
    int has_ref = 0; for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->t == IR_REF_INVARIANT) { has_ref = 1; break; }
    if (has_ref) { g_child_cache_n = 0; g_text_child_counter = 0; gvar_chain_prebuild_children_text(g, out, prefix); }
    gvar_chain_operand_refs(g);
    g_flat_slot_count = 0; g_bb_slotmap_n = 0; g_bb_varslot_n = 0;
    g_gvar_flat_chain = 1;
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    int rc = codegen_gvar_flat_chain_body(g->entry, prefix);
    emitter_end();
    g_gvar_flat_chain = 0;
    g_emit_cfg = save_cfg;
    return rc;
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
static int codegen_callee_block(IR_graph_t *g, const char *name, int arity, FILE *out) {
    if (!g || !g->entry || !name) return 1;
    IR_graph_t *save_cfg = g_emit_cfg; g_emit_cfg = g;
    IR_t *body_root = g->entry;
    if (g->body_root) {
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
    bb_label_t *lbl_α    = emit_label_intern(blbl);
    bb_label_t *lbl_redo = emit_label_intern(redo_lbl);
    bb_label_t lbl_γ, lbl_ω, lbl_β;
    emit_label_initf(&lbl_γ, "%s_γ", blbl);
    emit_label_initf(&lbl_ω, "%s_ω", blbl);
    emit_label_initf(&lbl_β, "%s_β", blbl);
    g_emit.flat_succ_p = &lbl_γ;
    g_emit.flat_fail_p = &lbl_ω;
    g_emit.flat_β_p    = &lbl_β;
    emit_label_define_bb(lbl_α);
    walk_bb_flat(body_root, &lbl_γ, &lbl_ω, &lbl_β);
    emit_label_define_bb(lbl_redo);
    emit_jmp_label(&lbl_β, JMP_JMP);
    emit_label_define_bb(&lbl_γ);
    { const char *s = "  mov edi, 1\n  call rt_set_last_ok@PLT\n  ret\n"; emit_text_n(s, strlen(s)); }
    emit_label_define_bb(&lbl_ω);
    { const char *s = "  mov edi, 0\n  call rt_set_last_ok@PLT\n  ret\n"; emit_text_n(s, strlen(s)); }
    emitter_end();
    g_emit_cfg = save_cfg;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern int resolve_bb_pred_count(void);
extern const char *resolve_bb_pred_name_at(int idx);
extern int resolve_bb_pred_arity_at(int idx);
extern IR_graph_t *resolve_bb_graph_at(int idx);
int codegen_clause_dispatch(FILE *out) {
    int npred = resolve_bb_pred_count();
    for (int i = 0; i < npred; i++) {
        const char *nm = resolve_bb_pred_name_at(i);
        if (!nm) continue;
        int ar = resolve_bb_pred_arity_at(i);
        if (ar == 0 && (strcmp(nm, "main") == 0 || strcmp(nm, "main/0") == 0)) continue;
        IR_graph_t *pg = resolve_bb_graph_at(i);
        if (!pg) continue;
        int rc = codegen_callee_block(pg, nm, ar, out);
        if (rc) return rc;
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int codegen_pl_pred_table(FILE *out) {
    int npred = resolve_bb_pred_count();
    int nrows = 0;
    fprintf(out, "  .section .data\n  .align 8\n.Lpl_pred_table:\n");
    for (int i = 0; i < npred; i++) {
        const char *nm = resolve_bb_pred_name_at(i);
        if (!nm) continue;
        int ar = resolve_bb_pred_arity_at(i);
        if (ar == 0 && (strcmp(nm, "main") == 0 || strcmp(nm, "main/0") == 0)) continue;
        if (!resolve_bb_graph_at(i)) continue;
        char blbl[160]; resolve_call_block_label(blbl, sizeof blbl, nm, ar);
        fprintf(out, "  .quad .Lpl_pname_%d\n  .quad %d\n  .quad %s\n  .quad %s_redo\n", i, ar, blbl, blbl);
        nrows++;
    }
    fprintf(out, "  .section .rodata\n");
    for (int i = 0; i < npred; i++) {
        const char *nm = resolve_bb_pred_name_at(i);
        if (!nm) continue;
        int ar = resolve_bb_pred_arity_at(i);
        if (ar == 0 && (strcmp(nm, "main") == 0 || strcmp(nm, "main/0") == 0)) continue;
        if (!resolve_bb_graph_at(i)) continue;
        const char *slash = strrchr(nm, '/'); int namelen = slash ? (int)(slash - nm) : (int)strlen(nm);
        fprintf(out, ".Lpl_pname_%d:\n  .asciz \"%.*s\"\n", i, namelen, nm);
    }
    fprintf(out, "  .text\n");
    return nrows;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void walk_bb_register_child_label(IR_t *nd, const char *α_label) {
    bb_box_fn fn = child_cache_get(nd);
    if (fn) child_cache_set_lbl(fn, α_label);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern bb_mode_t g_bb_mode;
bb_box_fn bb_build_pure_mode(IR_t *nd) {
    if (g_bb_mode == BB_MODE_LIVE) return bb_build_flat(nd);
    return bb_build_brokered(nd);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void lower_flat_reset(void) { g_flat_slot_count = 0; g_flat_node_id = 0; }

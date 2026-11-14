#include "emitter/em/em_head.h"
#include "emitter/emit_state_xa_pair.h"
#include "emitter/emit_state_em_03_sh_api.h"
#include "emitter/emit_medium.h"
#include "emitter/emit_knobs.h"
#include "emitter/emit_regime.h"
#include "emitter/emit_walk.h"
#include "emitter/emit_str.h"
#include "lower/lower_prolog.h"
#include "ir/dtp.h"
#include "ir/ir_node_index.h"
#include "templates/bb/bb_templates.h"
#include "runtime/core/g_runtime_eval.h"
#include "ir/stage2_proc_thunk_pattern.h"
#include "ir/stage2_procentry.h"
#include "ir/stage2_sm_prog_api.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "ir/ct_arena.h"
extern "C" {
#include "templates/x86/x86_rodata.h"
}
extern "C" {
#include "planner/zeta_plan.h"
}
extern "C" {
#include "lower/lower_common.h"
}
extern "C" {
#include "ir/call_forms.h"
}
extern "C" {
#include "optimizer/ir_query.h"
}
#include "templates/bb/bb_pl_leaf_kinds.h"
#include "emitter/em/em_01_sh.h"
#include "emitter/em/em_02_sh.h"
#include "emitter/em/em_03_sh.h"
#include "emitter/em/em_04_sh.h"
extern "C" { __attribute__((noreturn)) void rt_ab_undef_fn_stub(void); }
extern "C" { DESCR_t rt_call_bid_sn4(const char *, DESCR_t *, int, int); }
extern "C" { DESCR_t rt_call_name_sn4(const char *, DESCR_t *, int, int); }
extern "C" { DESCR_t rt_call_arr_bl_strict(const char *, DESCR_t *, int, int); }
extern "C" { DESCR_t rt_call_arr_bl_sn4(const char *, DESCR_t *, int, int); }
extern "C" { DESCR_t rt_call_arr_bl(const char * fn, DESCR_t * args, int nargs, int bidlen); }
extern "C" { DESCR_t rt_call_fld_sn4(DESCR_t * args, int nargs, sno_callee_rec_t * r); }
extern "C" { DESCR_t rt_call_callee_sn4(DESCR_t * args, int nargs, sno_callee_rec_t * r); }
extern "C" {             DESCR_t rt_sno_bynmark_d(DESCR_t *, int); }
extern "C" {             DESCR_t rt_sno_xthunk_d(DESCR_t *, int); }
extern "C" {             DESCR_t rt_sno_xlv_d(DESCR_t *, int); }
extern "C" {     DESCR_t rt_pl_dop_create_prolog_flag(DESCR_t *, int); }
extern "C" {     DESCR_t rt_pl_dop_flag_meta(DESCR_t *, int); }
extern "C" {     DESCR_t rt_pl_dop_pl_backtrace(DESCR_t *, int); }
extern "C" {     DESCR_t rt_pl_dop_term_variables(DESCR_t *, int); }
extern "C" {     DESCR_t rt_pl_dop_tbh_leading(DESCR_t *, int); }
extern "C" { DESCR_t rt_pl_dop_trie_slice(DESCR_t *, int); }
extern "C" { DESCR_t rt_pl_dop_cpu_ms(DESCR_t *, int); }
extern "C" { DESCR_t rt_pl_dop_db_retractall(DESCR_t *, int); }
extern "C" { DESCR_t rt_pl_dop_db_alive(DESCR_t *, int); }
extern "C" { DESCR_t rt_pl_dop_ax_eguard(DESCR_t *, int); }
extern "C" {     DESCR_t pas_pchar_of(DESCR_t *, int); }
extern "C" { DESCR_t pas_view_make(DESCR_t *, int); }
extern "C" { DESCR_t rt_pl_dop_nb_getval_guard(DESCR_t *, int); }
extern "C" {     DESCR_t rt_pl_dop_gnu_stream_line_column(DESCR_t *, int); }
extern "C" { DESCR_t rt_pl_dop_gnu_builtin(DESCR_t *, int); }
extern "C" {     DESCR_t rt_pl_dop_gnu_os(DESCR_t *, int); }
extern "C" {     DESCR_t rt_pl_dop_compound0(DESCR_t *, int); }
extern "C" {     DESCR_t rt_pl_dop_set_stream(DESCR_t *, int); }
extern "C" { DESCR_t rt_pl_dop_gnu_prolog_pid(DESCR_t *, int); }
extern "C" { DESCR_t rt_pl_dop_set_prolog_flag_declare(DESCR_t *, int); }
extern "C" { DESCR_t rt_pl_dop_put_edin(DESCR_t *, int); }
extern "C" {                                     DESCR_t rt_pl_dop_pl_cs_count(DESCR_t *, int); }
extern "C" {                                     DESCR_t rt_pl_dop_pl_cs_nth(DESCR_t *, int); }
extern "C" {     DESCR_t rt_rk_grep_append(DESCR_t *, int); }
extern "C" { DESCR_t rt_rk_byref_assign(DESCR_t *, int); }
extern "C" {
void xa_pair_push(struct bb_label_t * d, struct bb_label_t * j) {
    cv_reserve(&g_emit.xa_bb_emit_pair, (uint32_t)sizeof(xa_pair_t), (uint64_t)g_emit.xa_bb_emit_pair_n + 1, "xa_bb_emit_pair");
    xa_pair_t * e = &XA_PAIR(g_emit.xa_bb_emit_pair_n);
    e->define = d;
    e->jmp = j;
    e->rsp = 0;
    g_emit.xa_bb_emit_pair_n++;
}
extern "C" int emit_resumable_frame(void) { return (g_emit_cfg && g_emit_cfg->pf.resumable_frame) ? 1 : 0; }
static bb_ab_thunk_stems_t * bb_ab_thunk_stems(void) {
    bb_ab_thunk_stems_t * t = (bb_ab_thunk_stems_t *) g_emit.thunk_stems; if (!t) { t = new bb_ab_thunk_stems_t(); g_emit.thunk_stems = t; } return t;
}
extern "C" void bb_ab_thunk_stem_note(const char * nm, int kind) {
    if (!nm || kind <= PROC_THUNK_NONE || kind > PROC_THUNK_EXPR) return;
    bb_ab_thunk_stems_t * t = bb_ab_thunk_stems();
    if (t->stem.count(nm)) return;
    int * n = kind == PROC_THUNK_PATTERN ? &t->np : &t->nx;
    char b[fmt_len(".LT%c%d", 'x', *n)]; snprintf(b, sizeof b, ".LT%c%d", kind == PROC_THUNK_PATTERN ? 'p' : 'x', *n); (*n)++;
    t->stem.emplace(std::string(nm), std::string(b)); t->stems.emplace(std::string(b));
}
static const char * bb_ab_thunk_stem(const char * nm) {
    const bb_ab_thunk_stems_t * t = (const bb_ab_thunk_stems_t *) g_emit.thunk_stems; if (!t || !nm) return (const char *)0;
    auto it = t->stem.find(nm); return it == t->stem.end() ? (const char *)0 : it->second.c_str();
}
extern "C" int bb_ab_sym_is_thunk_stem(const char * sym) { const bb_ab_thunk_stems_t * t = (const bb_ab_thunk_stems_t *) g_emit.thunk_stems; return (t && sym && t->stems.count(sym)) ? 1 : 0; }
extern "C" const char * bb_ab_fn_sym(const char * nm) {
    bb_ab_thunk_stems_t * t = bb_ab_thunk_stems(); const char * st = bb_ab_thunk_stem(nm);
    t->fn = st ? std::string(st) : std::string("FN__") + bb_ab_sym_name(nm); return t->fn.c_str();
}
extern "C" const char * bb_ab_thunk_stem_or(const char * nm) { const char * st = bb_ab_thunk_stem(nm); return st ? st : nm; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_label_lookup_offset(const char * name)
{
    if (!name) return -1;
    for (int i = 0; i < g_emitter.g_label_pool_n; i++) if (g_emitter.g_label_pool[i] && g_emitter.g_label_pool[i]->offset != BB_LABEL_UNRESOLVED && strcmp(g_emitter.g_label_pool[i]->name, name) == 0)
        return g_emitter.g_label_pool[i]->offset;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int to_inner_gen_operand_k(IR_t *gi, IR_t **nodes, int n) {
    int bk = -1;
    if (gi->op != IR_TO && gi->op != IR_TO_BY) return -1;
    for (int oi = 0; oi < gi->n_operands; oi++) { int k = nidx(nodes, n, (IR_t *)gi->operands[oi]); if (k >= 0 && (ir_is_generator_kind(nodes[k]->op) && k > bk)) bk = k; }
    return bk;
}
int bb_call_route_classify(IR_t * nd) {
    int r = nd ? lower_call_route(nd) : CALL_ROUTE_FATAL;
    return r == CALL_ROUTE_RK_BOOL ? RK_BOOL_ROUTE(ir_call_arg(nd, 0)) : r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void flat_drive_cleanup(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t *node_γ, bb_label_t *node_ω, int zread) {
    IR_t *nd = nodes[i];
    int kind = (int) IR_LIT(nd).ival;
    IR_t *op0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
    IR_t *open = kind == CLEANUP_OPEN ? nd : (kind == CLEANUP_PROBE || kind == CLEANUP_WAKE || kind == CLEANUP_UNDO) ? (IR_t *)0 : kind == CLEANUP_BANK ? nd : op0;
    g_emit.xa_bb_emit_pair_n = 0;
    if (kind != CLEANUP_PROBE && kind != CLEANUP_WAKE && kind != CLEANUP_UNDO && !emit_resumable_frame())
        { fprintf(stderr, "FATAL emit: a setup_call_cleanup record (IR_CLEANUP kind %d) in a graph that is not a pinned RBP frame\n", kind); abort(); }
    if (kind == CLEANUP_RESUME) {
        IR_t *g = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0;
        IR_t *x = nd->n_operands > 2 ? nd->operands[2] : (IR_t *)0;
        int kg = nidx(nodes, n, g), kx = nidx(nodes, n, x);
        xa_pair_push(NULL, kg >= 0 ? betas[kg] : node_ω);
        xa_pair_push(NULL, kx >= 0 ? lbls[kx] : node_ω);
    }
    g_emit.op_ival = kind;
    g_emitter.g_zd_cutd = kind == CLEANUP_RESUME ? zrow(nd)->zcut : -1;
    g_emit.op_off = (kind == CLEANUP_PROBE || kind == CLEANUP_WAKE || kind == CLEANUP_UNDO || kind == CLEANUP_DET) ? 0 : (open ? (open == nd ? zrow(nd)->loff : zrow(nd)->o0off) : -1);
    g_emit.op_sa = -1;
    if (kind == CLEANUP_RESUME && nd->n_operands > 3 && nd->operands[3] && nd->operands[3]->op == IR_BOUND && nidx(nodes, n, nd->operands[3]) >= 0) g_emit.op_sa = zrow(nd->operands[3])->loff;
    if (kind == CLEANUP_OPEN && nd->n_operands > 1 && nd->operands[1]) g_emit.op_sa = drive_value_slot(nd->operands[1]);
    g_emit.op_sb = kind == CLEANUP_OPEN ? zread : -1;
    if (kind == CLEANUP_PROBE && op0) g_emit.op_sa = zrow(nd)->o0off;
    if (kind == CLEANUP_BANK) g_emit.op_sa = zrow(nd)->loff;
    if (kind == CLEANUP_DET) g_emit.op_sa = op0 ? zrow(nd)->o0off : -1;
    DRIVE_FILL(nd, lbls[i], node_γ, node_ω, betas[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void drive_arg_slots_reserve(int n) {
    if (n <= g_emit.op_arg_slot_cap) return;
    int cap = g_emit.op_arg_slot_cap > 0 ? g_emit.op_arg_slot_cap : 16;
    while (cap < n) cap *= 2;
    int *grown = (int *)ct_grow(g_emit.op_arg_slot, (size_t)cap * sizeof(int));
    if (!grown) { fprintf(stderr, "FATAL emit_drive: arg-slot staging realloc to %d failed\n", cap); abort(); }
    g_emit.op_arg_slot = grown; g_emit.op_arg_slot_cap = cap;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned ab_fn_hash(const char * s) { unsigned h = 2166136261u; for (const unsigned char * p = (const unsigned char *)s; *p; p++) { h ^= *p; h *= 16777619u; } return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ** ab_cell(int slot) { return (void **)CV_AT(g_emitter.g_ab_dir, void *, slot >> 10) + (slot & 1023); }
static void ab_hix_put(const char * name, int slot) {
    uint32_t m = g_emitter.g_ab_hix.len - 1, h = ab_fn_hash(name) & m;
    while (CV_AT(g_emitter.g_ab_hix, ab_ent_t, h).name) h = (h + 1) & m;
    CV_AT(g_emitter.g_ab_hix, ab_ent_t, h).name = name; CV_AT(g_emitter.g_ab_hix, ab_ent_t, h).slot = slot;
}
static void ab_hix_grow(void) {
    uint32_t c = g_emitter.g_ab_hix.len ? g_emitter.g_ab_hix.len * 2 : 1024;
    cv_t v = { 0, 0, 0, 0 }; v.p = ct_zalloc(c, sizeof(ab_ent_t)); v.len = c; v.cap = c; v.esz = (uint32_t)sizeof(ab_ent_t); g_emitter.g_ab_hix = v;
    for (int s = 0; s < g_emitter.g_ab_fn_cell_n; s++) ab_hix_put(CV_AT(g_emitter.g_ab_names, const char *, s), s);
}
static int ab_slot_new(const char * fname) {
    int s = g_emitter.g_ab_fn_cell_n;
    if ((s & 1023) == 0) CV_PUSH(g_emitter.g_ab_dir, void *) = ct_zalloc(1024, sizeof(void *));
    CV_PUSH(g_emitter.g_ab_names, const char *) = ct_strdup(fname);
    *ab_cell(s) = (void *)(uintptr_t)rt_ab_undef_fn_stub;
    g_emitter.g_ab_fn_cell_n = s + 1;
    if ((uint64_t)g_emitter.g_ab_fn_cell_n * 2 > (uint64_t)g_emitter.g_ab_hix.len) ab_hix_grow(); else ab_hix_put(CV_AT(g_emitter.g_ab_names, const char *, s), s);
    return s;
}
static int bb_ab_slot_for(const char * fname) {
    if (g_emitter.g_ab_hix.len) { uint32_t m = g_emitter.g_ab_hix.len - 1, h = ab_fn_hash(fname) & m;
        for (;; h = (h + 1) & m) { ab_ent_t * e = &CV_AT(g_emitter.g_ab_hix, ab_ent_t, h); if (!e->name) break; if (!strcmp(e->name, fname)) return e->slot; } }
    return ab_slot_new(fname);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * bb_ab_cell_addr(const char * fname) { return MEDIUM_BINARY ? (void *)ab_cell(bb_ab_slot_for(fname)) : (void *)0; }
void * bb_ab_fn_cell_ptr(const char * fname) { return (void *)ab_cell(bb_ab_slot_for(fname)); }
extern "C" const char * bb_ab_sym_name(const char * nm) {
    enum { MAXN = 48, KEEP = 31 }; static_assert(KEEP + 17 <= MAXN, "a long name is its kept units, a dollar and 16 hex"); static char b[MAXN + 1]; int j = 0, bnd = 0, over = 0;
    std::string star_stem; if (nm && nm[0] == '*' && bb_ab_thunk_stem_or(nm + 1) != nm + 1) { star_stem = std::string("*") + bb_ab_thunk_stem_or(nm + 1); nm = star_stem.c_str(); }
    nm = bb_ab_thunk_stem_or(nm);
    unsigned long long h = 1469598103934665603ULL; for (const char * c = nm ? nm : ""; *c; c++) { unsigned char u = (unsigned char) *c; char e[4]; int k; h = (h ^ u) * 1099511628211ULL;
        if ((u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z') || (u >= '0' && u <= '9') || u == '_' || u == '$' || u == '.') { e[0] = (char) u; k = 1; } else k = snprintf(e, sizeof e, "$%02X", u);
        if (!over && j + k <= MAXN) { memcpy(b + j, e, (size_t) k); j += k; if (j <= KEEP) bnd = j; } else over = 1; }
    if (over) j = bnd + snprintf(b + bnd, sizeof b - (size_t) bnd, "$%016llx", h); b[j] = 0; return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ab_seal_entry_cells(const char * pname, void * fnbase, int alpha_face) {
    extern int emit_label_lookup_offset(const char *);
    if (!pname || !fnbase) return;
    const char * sn = alpha_face ? pname : bb_ab_sym_name(pname); char lbl[fmt_len(alpha_face ? "%s_\xce\xb1" : "LBL__%s", sn)], cell[fmt_len(alpha_face ? "alpha$%s" : "entry$%s", pname)];
    if (alpha_face) { snprintf(lbl, sizeof lbl, "%s_\xce\xb1", pname); snprintf(cell, sizeof cell, "alpha$%s", pname); }
    else { snprintf(lbl, sizeof lbl, "LBL__%s", sn); snprintf(cell, sizeof cell, "entry$%s", pname); }
    int off = emit_label_lookup_offset(lbl); if (off < 0) { if (getenv("SCRIP_SEAL_DIAG")) fprintf(stderr, "[SEAL] MISS lbl=%s cell=%s\n", lbl, cell); return; }
    *(void **)bb_ab_fn_cell_ptr(cell) = (void *)((char *)fnbase + off);
}
extern "C" void bb_ab_seal_alpha(const char * pname, void * alpha) {
    if (!pname || !alpha) return;
    char cell[fmt_len("alpha$%s", pname)]; snprintf(cell, sizeof cell, "alpha$%s", pname);
    *(void **)bb_ab_fn_cell_ptr(cell) = alpha;
}
int bb_ab_slot_index(const char * fname) { return bb_ab_slot_for(fname); }
void * bb_ab_cell_at(int slot) { return (slot >= 0 && slot < g_emitter.g_ab_fn_cell_n) ? (void *)ab_cell(slot) : (void *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static emit_dir_ref thk_dir(void) { emit_dir_ref d = { &g_emit.thk_chunks, &g_emit.thk_names, &g_emit.thk_hix, &g_emit.thk_n, sizeof(sno_thunk_rec_t) }; return d; }
static emit_dir_ref callee_dir(void) { emit_dir_ref d = { &g_emit.callee_chunks, &g_emit.callee_names, &g_emit.callee_hix, &g_emit.callee_n, sizeof(sno_callee_rec_t) }; return d; }
static emit_dir_ref dstar_dir(void) { emit_dir_ref d = { &g_emit.dstar_chunks, &g_emit.dstar_names, &g_emit.dstar_hix, &g_emit.dstar_n, sizeof(sno_dstar_rec_t) }; return d; }
static void * dir_rec(emit_dir_ref d, int slot) { return (char *)CV_AT(*d.chunks, void *, slot >> 8) + (size_t)(slot & 255) * d.esz; }
static void dir_hix_put(emit_dir_ref d, const char * name, int slot) {
    uint32_t m = d.hix->len - 1, h = ab_fn_hash(name) & m;
    while (CV_AT(*d.hix, ab_ent_t, h).name) h = (h + 1) & m;
    CV_AT(*d.hix, ab_ent_t, h).name = name; CV_AT(*d.hix, ab_ent_t, h).slot = slot;
}
static void dir_hix_grow(emit_dir_ref d) {
    uint32_t c = d.hix->len ? d.hix->len * 2 : 256;
    cv_t v = { 0, 0, 0, 0 }; v.p = ct_zalloc(c, sizeof(ab_ent_t)); v.len = c; v.cap = c; v.esz = (uint32_t)sizeof(ab_ent_t); *d.hix = v;
    for (int s = 0; s < *d.n; s++) dir_hix_put(d, CV_AT(*d.names, const char *, s), s);
}
static int dir_slot_for(emit_dir_ref d, const char * name) {
    if (d.hix->len) { uint32_t m = d.hix->len - 1, h = ab_fn_hash(name) & m;
        for (;; h = (h + 1) & m) { ab_ent_t * e = &CV_AT(*d.hix, ab_ent_t, h); if (!e->name) break; if (!strcmp(e->name, name)) return e->slot; } }
    int s = *d.n;
    if ((s & 255) == 0) CV_PUSH(*d.chunks, void *) = ct_zalloc(256, d.esz);
    CV_PUSH(*d.names, const char *) = ct_strdup(name);
    *d.n = s + 1;
    if ((uint64_t)*d.n * 2 > (uint64_t)d.hix->len) dir_hix_grow(d); else dir_hix_put(d, CV_AT(*d.names, const char *, s), s);
    return s;
}
extern "C" void * bb_thunk_rec_addr(const char * name) { return (MEDIUM_BINARY && name) ? dir_rec(thk_dir(), dir_slot_for(thk_dir(), name)) : (void *)0; }
extern "C" void bb_thunk_rec_fill(const char * name, void * fn, int32_t frame_bytes, int32_t zstatic) {
    if (!name) return;
    sno_thunk_rec_t * r = (sno_thunk_rec_t *)dir_rec(thk_dir(), dir_slot_for(thk_dir(), name)); r->fn = fn; r->frame_bytes = frame_bytes; r->zstatic = zstatic;
}
extern "C" void * bb_callee_rec_addr(const char * name) {
    if (!name) return (void *)0;
    int s = dir_slot_for(callee_dir(), name);
    if (!MEDIUM_BINARY) return (void *)0;
    sno_callee_rec_t * r = (sno_callee_rec_t *)dir_rec(callee_dir(), s);
    if (!r->name) { r->name = CV_AT(g_emit.callee_names, const char *, s); r->gen1 = 0; r->fi = -1; }
    return (void *)r;
}
static const ProcEntry * emit_thunk_entry_of(const char * star) {
    if (!star || star[0] != '*') return (const ProcEntry *)0;
    for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].thunk_flags && g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, star + 1)) return &
        g_stage2.proc_table[i];
    return (const ProcEntry *)0;
}
static uint32_t emit_thunk_flags_of(const char * star) { const ProcEntry * pe = emit_thunk_entry_of(star); return pe ? pe->thunk_flags : 0u; }
extern "C" int emit_star_is_thunk(const char * star) { return (emit_thunk_flags_of(star) & SNO_DSTAR_THUNK) ? 1 : 0; }
static const char * emit_thunk_star_text(const char * star) {
    const ProcEntry * pe = emit_thunk_entry_of(star);
    if (!pe) return star;
    if (!pe->stage_var) return (const char *)0;
    size_t n = strlen(pe->stage_var); char * t = (char *)ct_alloc(n + 2); t[0] = '*'; memcpy(t + 1, pe->stage_var, n + 1); return t;
}
extern "C" void * bb_dstar_rec_addr(const char * star) {
    if (!star || star[0] != '*' || !star[1]) return (void *)0;
    if ((unsigned char)star[1] == 1) return MEDIUM_BINARY ? (void *)star : (void *)0;
    int s = dir_slot_for(dstar_dir(), star);
    if (!MEDIUM_BINARY) return (void *)0;
    sno_dstar_rec_t * r = (sno_dstar_rec_t *)dir_rec(dstar_dir(), s);
    if (!r->mark[0]) { r->mark[0] = '*'; r->mark[1] = 1; r->star = emit_thunk_star_text(CV_AT(g_emit.dstar_names, const char *, s)); r->pidx = -1; r->flags = emit_thunk_flags_of(star); }
    return (void *)r;
}
extern "C" void emit_thunk_star_bind(const char * thunk_name) {
    extern int rt_proc_index_of(const char *);
    if (!thunk_name) return;
    std::string star = std::string("*") + thunk_name;
    sno_dstar_rec_t * r = (sno_dstar_rec_t *)bb_dstar_rec_addr(star.c_str());
    if (r && (r->flags & SNO_DSTAR_THUNK)) r->pidx = (int32_t)rt_proc_index_of(thunk_name);
}
extern "C" const char * emit_thunk_star_label(const char * thunk_name) {
    extern const char * bb_ab_sym_name(const char *);
    if (!thunk_name) return (const char *)0;
    std::string star = std::string("*") + thunk_name;
    (void)dir_slot_for(dstar_dir(), star.c_str());
    std::string lbl = std::string(".Ldstar_") + bb_ab_sym_name(star.c_str());
    cv_reserve(&g_emit.star_label_buf, 1, (uint64_t)lbl.size() + 1, "star_label_buf");
    memcpy(g_emit.star_label_buf.p, lbl.c_str(), lbl.size() + 1);
    return (const char *)g_emit.star_label_buf.p;
}
extern "C" void * bb_dstar_rec_intern(const char * key, uint32_t flags) {
    if (!key || !key[0] || !key[1]) return (void *)0;
    int s = dir_slot_for(dstar_dir(), key);
    sno_dstar_rec_t * r = (sno_dstar_rec_t *)dir_rec(dstar_dir(), s);
    if (!r->mark[0]) { r->mark[0] = '*'; r->mark[1] = 1; r->star = CV_AT(g_emit.dstar_names, const char *, s); r->pidx = -1; r->flags = flags; }
    return (void *)r;
}
static void emit_dstar_records_data(void) {
    if (!g_emit.dstar_n) return;
    extern const char * bb_ab_sym_name(const char *); extern void x86_asm_str_escape_c(const char *, char *, unsigned long);
    emit_textf("  .section .data\n");
    for (int k = 0; k < g_emit.dstar_n; k++) { const char * nm = CV_AT(g_emit.dstar_names, const char *, k); std::string sym = bb_ab_sym_name(nm);
        if (emit_thunk_star_text(nm))
            emit_textf("  .p2align 3\n.Ldstar_%s:\n  .byte 42, 1, 0, 0, 0, 0, 0, 0\n  .quad .Ldstarn_%s\n  .long -1, %u\n", sym.c_str(), sym.c_str(), (unsigned)emit_thunk_flags_of(nm));
        else emit_textf("  .p2align 3\n.Ldstar_%s:\n  .byte 42, 1, 0, 0, 0, 0, 0, 0\n  .quad 0\n  .long -1, %u\n", sym.c_str(), (unsigned)emit_thunk_flags_of(nm)); }
    emit_textf("  .section .rodata\n");
    for (int k = 0; k < g_emit.dstar_n; k++) { const char * nm = CV_AT(g_emit.dstar_names, const char *, k); const char * st = emit_thunk_star_text(nm); if (!st) continue;
        std::string sym = bb_ab_sym_name(nm);
        size_t cap = 4 * strlen(st) + 1; std::string esc(cap, '\0'); x86_asm_str_escape_c(st, &esc[0], cap);
        emit_textf(".Ldstarn_%s: .string \"%s\"\n", sym.c_str(), esc.c_str()); }
    emit_textf("  .section .text\n  .intel_syntax noprefix\n");
}
extern "C" void emit_callee_records_data(void) {
    emit_dstar_records_data();
    if (!g_emit.callee_n) return;
    extern const char * bb_ab_sym_name(const char *); extern void x86_asm_str_escape_c(const char *, char *, unsigned long);
    emit_textf("  .section .data\n");
    for (int k = 0; k < g_emit.callee_n; k++) { const char * nm = CV_AT(g_emit.callee_names, const char *, k); std::string sym = bb_ab_sym_name(nm);
        emit_textf("  .p2align 3\n.Lcallee_%s:\n  .quad .Lcalleen_%s\n  .long 0, -1\n  .quad 0\n  .long 0, 0, 0, 0\n  .quad 0, 0\n", sym.c_str(), sym.c_str()); }
    emit_textf("  .section .rodata\n");
    for (int k = 0; k < g_emit.callee_n; k++) { const char * nm = CV_AT(g_emit.callee_names, const char *, k); std::string sym = bb_ab_sym_name(nm); size_t cap = 4 * strlen(nm) + 1;
        std::string esc(cap, '\0'); x86_asm_str_escape_c(nm, &esc[0], cap);
        emit_textf(".Lcalleen_%s: .string \"%s\"\n", sym.c_str(), esc.c_str()); }
    emit_textf("  .section .text\n  .intel_syntax noprefix\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string icn_gen_line_save(void) {
    return x86("comment",
        "suspend: the generator's own line and file are kept in its header at +48 and +56 for the resume (through rdx, the frame copy the gamma below already uses), then the caller's come back")
         + x86("mov", "rdx", "rbp")
         + x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
         + x86("mov", "rcx", RDQ("rcx", GRT_AT(line)))
         + x86("mov", RDQ("rdx", 48), "rcx")
         + x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
         + x86("mov", "rcx", RDQ("rcx", GRT_AT(file)))
         + x86("mov", RDQ("rdx", 56), "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string icn_gen_line_resume(void) {
    return x86("comment", "resume: the generator's own line and file, kept at its last suspend, come back (read through rdx, a copy of the frame the resume hands over in rax)")
         + x86("mov", "rdx", "rax")
         + x86("mov", "rcx", RDQ("rdx", 48))
         + x86("test", "rcx", "rcx")
         + x86("je", L(238))
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
         + x86("mov", RDQ("rax", GRT_AT(line)), "rcx")
         + x86("mov", "rcx", RDQ("rdx", 56))
         + x86("test", "rcx", "rcx")
         + x86("je", L(238))
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
         + x86("mov", RDQ("rax", GRT_AT(file)), "rcx")
         + x86("def", L(238))
         + x86("mov", "rax", "rdx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void x86_asm_str_escape_c(const char * s, char * out, unsigned long outsz) {
    if (!out || !outsz) return;
    std::string e = x86_asm_str_escape(s ? s : "");
    unsigned long n = (unsigned long)e.size(); if (n >= outsz) n = outsz - 1;
    memcpy(out, e.c_str(), (size_t)n); out[n] = 0;
}
void display_copy(IR_t *nd) {
    g_emit.op_seal = nd ? nd->seal : 0;
    g_emit.op_name1 = g_emit.op_seal == 1 ? "r13" : g_emit.op_seal == 2 ? "r14" : g_emit.op_seal == 3 ? "r15" : (const char *)0;
    g_emit.op_sa = zrow(nd)->dsa;
    g_emit.op_sb = zrow(nd)->dsb;
}
}
int emit_knob_if_one(const char * name) { const char * e = getenv(name); return (e && *e == '1') ? 1 : 0; }
const char * sn4_byname_sym(const char * fn, int strict) {
    switch (sn4_byname_kind(fn, strict)) {
        case 1: return "rt_call_bid_sn4";
        case 2: return "rt_call_name_sn4";
        case 3: return "rt_call_arr_bl_sn4";
        case 4: return "rt_call_arr_bl_strict";
        default: return "rt_call_arr_bl";
    }
}
uint64_t sn4_byname_fp(const char * fn, int strict) {
    switch (sn4_byname_kind(fn, strict)) {
        case 1: return (uint64_t)(uintptr_t)(void *)rt_call_bid_sn4;
        case 2: return (uint64_t)(uintptr_t)(void *)rt_call_name_sn4;
        case 3: return (uint64_t)(uintptr_t)(void *)rt_call_arr_bl_sn4;
        case 4: return (uint64_t)(uintptr_t)(void *)rt_call_arr_bl_strict;
        default: return (uint64_t)(uintptr_t)(void *)rt_call_arr_bl;
    }
}
const char * bb_callee_baked_sym(int k) { return k == 2 ? "rt_call_callee_sn4" : "rt_call_fld_sn4"; }
uint64_t bb_callee_baked_fp(int k) { return k == 2 ? (uint64_t)(uintptr_t)(void *)rt_call_callee_sn4 : (uint64_t)(uintptr_t)(void *)rt_call_fld_sn4; }
int elem_access_kind(const IR_t * nd) { return nd ? nd->elem_access : 0; }
int xa_flat_sig_names(const char * fname, int * nf_out, int * nsave_out, int * gk_out, int * res_gk_out) {
    if (!fname || !fname[0] || g_rt_eval.rt_fragment_emit) return 0;
    int nf = rt_proc_nformals(fname); if (nf < 0) nf = 0;
    int np = 0, nsave = 0, res_gk = -1;
    int have = bb_scc_probe(fname, nf, &np, &nsave, gk_out, &res_gk);
    if (!have || nsave <= 0 || nsave > 29) return 0;
    for (int k = 0; k < nsave; k++) if (gk_out[k] < 0) return 0;
    if (res_gk < 0) return 0;
    *nf_out = nf; *nsave_out = nsave;
    if (res_gk_out) *res_gk_out = res_gk;
    return 1;
}
const char * xa_icn_trace_intern(const char * s) {
    if (!s) return s;
    return g_emitter.xa_icn_trace_intern_pool.insert(std::string(s)).first->c_str();
}
int xa_flat_graph_holds_a_frame_retry(void) {
    if (!g_emit_cfg) return 0;
    for (int i = 0; i < g_emit_cfg->n; i++) { const IR_t * nd = g_emit_cfg->all[i];
        if (nd && (nd->op == IR_DISJUNCTION || nd->op == IR_TO || nd->op == IR_TO_BY)) return 1; }
    return 0;
}
long bcps_parse_rsp(const char * t) {
    const char * p = strstr(t, "[rsp");
    if (!p) return -1;
    p += 4; if (*p == '#' || *p == '$') p++;
    while (*p == ' ') p++;
    if (*p == ']') return 0;
    if (*p != '+') return -1;
    p++; while (*p == ' ') p++;
    if (*p < '0' || *p > '9') return -1;
    long v = 0; while (*p >= '0' && *p <= '9') v = v * 10 + (*p++ - '0');
    return (*p == ']') ? v : -1;
}
void * dop_direct_fp(const char * fn, int64_t narg, const char ** sym) {
    static const struct { char nm[34]; int ar; char sy[34]; DESCR_t (*fp)(DESCR_t *, int); } t[] = {
        { "$clause_unify", 2, "rt_pl_dop_clause_unify", rt_pl_dop_clause_unify },
        { "$db_data_unify", 3, "rt_pl_dop_db_data_unify", rt_pl_dop_db_data_unify },
        { "$dl_push", 1, "rt_pl_dop_dl_push", rt_pl_dop_dl_push }, { "$dl_reached", 1, "rt_pl_dop_dl_reached", rt_pl_dop_dl_reached }, { "$dl_false", 1, "rt_pl_dop_dl_false", rt_pl_dop_dl_false },
            { "$dl_reset", 1, "rt_pl_dop_dl_reset", rt_pl_dop_dl_reset }, { "$dl_tick", 1, "rt_pl_dop_dl_tick", rt_pl_dop_dl_tick },
        { "$throw", 1, "rt_pl_throw_raise", rt_pl_throw_raise }, { "$existence_error", 1, "rt_pl_exist_raise", rt_pl_exist_raise },
        { "$catch_handle", 1, "rt_pl_catch_handle", rt_pl_catch_handle },
        { "$write", 1, "dop_write", dop_write }, { "$nl", 0, "dop_nl", dop_nl },
        { "$fp_model_spitbol", 0, "rt_fp_model_spitbol", rt_fp_model_spitbol }, { "SNO$WANTNM", -1, "rt_sno_wantnm_d", rt_sno_wantnm_d }, { "SNO$CHKNM", -1, "rt_sno_chknm_d", rt_sno_chknm_d },
            { "SNO$PATCK", 2, "rt_sno_patck_d", rt_sno_patck_d }, { "SNO$XNAME", -1, "rt_sno_xname_d", rt_sno_xname_d }, { "SNO$BYNAME", 1, "rt_sno_bynmark_d", rt_sno_bynmark_d }, { "SNO$XTHUNK", 1,
                "rt_sno_xthunk_d", rt_sno_xthunk_d },
            { "SNO$XLV", 1, "rt_sno_xlv_d", rt_sno_xlv_d },
            { "SNO$MKPAT", -1, "rt_sno_mkpat_d", rt_sno_mkpat_d },
        { "SNO$NAME", 1, "rt_sno_name_dl", rt_sno_name_dl }, { "SNO$PBK", 2, "rt_sno_pbk_d", rt_sno_pbk_d }, { "SNO$PBN", 2, "rt_sno_pbn_d", rt_sno_pbn_d }, { "SNO$PB0", 1, "rt_sno_pb0_d",
            rt_sno_pb0_d },
        { "SNO$PBC", 3, "rt_sno_pbc_d", rt_sno_pbc_d }, { "SNO$PCUR", 1, "rt_sno_pcur_d", rt_sno_pcur_d }, { "SNO$PBALT", 2, "rt_sno_pbalt_d", rt_sno_pbalt_d }, { "SNO$PARB", 1,
            "rt_sno_parb_d", rt_sno_parb_d },
        { "SNO$PFEN", 1, "rt_sno_pfen_d", rt_sno_pfen_d }, { "SNO$PDEF", 1, "rt_sno_pdef_d", rt_sno_pdef_d }, { "SNO$KWSET", 2, "rt_sno_kwset_d", rt_sno_kwset_d }, { "SNO$MKEXPR", 1,
            "rt_sno_mkexpr_d", rt_sno_mkexpr_d },
        { "SNO$STMT", -1, "rt_sno_stmt_d", rt_sno_stmt_d }, { "SNO$NOFAIL", 0, "rt_sno_nofail_d", rt_sno_nofail_d }, { "SNO$LIST", 1, "rt_sno_list_d", rt_sno_list_d },
        { "$compare", 3, "rt_pl_dop_compare", rt_pl_dop_compare }, { "$functor", 3, "rt_pl_dop_functor", rt_pl_dop_functor }, { "$arg", 3, "rt_pl_dop_arg", rt_pl_dop_arg },
        { "$skip_list", 3, "rt_pl_dop_skip_list", rt_pl_dop_skip_list }, { "$seek_list", 4, "rt_pl_dop_seek_list", rt_pl_dop_seek_list }, { "$argv", 1, "rt_pl_dop_argv", rt_pl_dop_argv },
        { "$univ", 2, "rt_pl_dop_univ", rt_pl_dop_univ }, { "$copy_term", 2, "rt_pl_dop_copy_term", rt_pl_dop_copy_term },
        { "$copy_term_nat", 2, "rt_pl_dop_copy_term_nat", rt_pl_dop_copy_term_nat },
        { "$term_variables", 2, "rt_pl_dop_term_variables", rt_pl_dop_term_variables }, { "$numbervars3", 3, "rt_pl_dop_numbervars3", rt_pl_dop_numbervars3 },
        { "$numbervars1", 1, "rt_pl_dop_numbervars1", rt_pl_dop_numbervars1 }, { "$scc_bnew", 1, "rt_pl_dop_scc_bnew", rt_pl_dop_scc_bnew }, { "$scc_take", 2, "rt_pl_dop_scc_take",
            rt_pl_dop_scc_take }, { "$succ", 2, "rt_pl_dop_succ", rt_pl_dop_succ }, { "$plus", 3, "rt_pl_dop_plus", rt_pl_dop_plus },
        { "$attv_set", 2, "rt_pl_dop_attv_set", rt_pl_dop_attv_set }, { "$setarg", 3, "rt_pl_dop_setarg", rt_pl_dop_setarg }, { "$nb_setarg", 3, "rt_pl_dop_nb_setarg", rt_pl_dop_nb_setarg },
            { "$nb_linkarg", 3, "rt_pl_dop_nb_linkarg", rt_pl_dop_nb_linkarg },
            { "$stamp_date_time", 3, "rt_pl_dop_stamp_date_time", rt_pl_dop_stamp_date_time }, { "$date_time_stamp", 2, "rt_pl_dop_date_time_stamp", rt_pl_dop_date_time_stamp },
            { "$format_time_text", 3, "rt_pl_dop_format_time_text", rt_pl_dop_format_time_text },
        { "$create_prolog_flag", 4, "rt_pl_dop_create_prolog_flag", rt_pl_dop_create_prolog_flag }, { "$flag_meta", 3, "rt_pl_dop_flag_meta", rt_pl_dop_flag_meta }, { "$attv_atts", 2,
            "rt_pl_dop_attv_atts", rt_pl_dop_attv_atts },
        { "$attv_take", 1, "rt_pl_dop_attv_take", rt_pl_dop_attv_take }, { "$unifiable", 3, "rt_pl_dop_unifiable", rt_pl_dop_unifiable },
        { "$crv_enter", 1, "rt_pl_dop_crv_enter", rt_pl_dop_crv_enter }, { "$crv_adj", 1, "rt_pl_dop_crv_adj", rt_pl_dop_crv_adj },
        { "$crv_collect", 2, "rt_pl_dop_crv_collect", rt_pl_dop_crv_collect }, { "$undo_push", 1, "rt_pl_dop_undo_push", rt_pl_dop_undo_push },
        { "$undo_take", 1, "rt_pl_dop_undo_take", rt_pl_dop_undo_take },
        { "$copy_keep", 4, "rt_pl_dop_copy_keep", rt_pl_dop_copy_keep }, { "$pl_backtrace", 2, "rt_pl_dop_pl_backtrace", rt_pl_dop_pl_backtrace }, { "$factorize_term", 3, "rt_pl_dop_factorize_term",
            rt_pl_dop_factorize_term },
        { "$wall_us", 1, "rt_pl_dop_wall_us", rt_pl_dop_wall_us }, { "$wall_ms", 1, "rt_pl_dop_wall_ms", rt_pl_dop_wall_ms },
        { "$sort", 2, "rt_pl_dop_sort", rt_pl_dop_sort }, { "$msort", 2, "rt_pl_dop_msort", rt_pl_dop_msort }, { "$char_type", 2, "rt_pl_dop_char_type", rt_pl_dop_char_type },
        { "$pl_big", 1, "rt_pl_dop_big", rt_pl_dop_big }, { "$pl_engine", 1, "rt_pl_dop_pl_engine", rt_pl_dop_pl_engine },{ "$findall_new", 0, "rt_pl_dop_findall_new", rt_pl_dop_findall_new },
            { "$findall_add", 2, "rt_pl_dop_findall_add", rt_pl_dop_findall_add },
        { "$findall_result", 2, "rt_pl_dop_findall_result", rt_pl_dop_findall_result }, { "$findall_result4", 3, "rt_pl_dop_findall_result4", rt_pl_dop_findall_result4 }, { "$bagof_result",
            2, "rt_pl_dop_bagof_result", rt_pl_dop_bagof_result },
        { "$setof_result", 2, "rt_pl_dop_setof_result", rt_pl_dop_setof_result },
        { "$sub_atom_n", 5, "rt_pl_dop_sub_atom_n", rt_pl_dop_sub_atom_n }, { "$sub_atom_at", 6, "rt_pl_dop_sub_atom_at", rt_pl_dop_sub_atom_at },
        { "$atom_concat_n", 3, "rt_pl_dop_atom_concat_n", rt_pl_dop_atom_concat_n }, { "$atom_concat_at", 4, "rt_pl_dop_atom_concat_at", rt_pl_dop_atom_concat_at },
        { "$bagof_group_n", 1, "rt_pl_dop_bagof_group_n", rt_pl_dop_bagof_group_n }, { "$bagof_group_at", 4, "rt_pl_dop_bagof_group_at", rt_pl_dop_bagof_group_at }, { "$setof_group_at", 4,
            "rt_pl_dop_setof_group_at", rt_pl_dop_setof_group_at },
        { "$db_assertz", 2, "rt_pl_dop_db_assertz", rt_pl_dop_db_assertz }, { "$db_asserta", 2, "rt_pl_dop_db_asserta", rt_pl_dop_db_asserta },
        { "$db_erase", 2, "rt_pl_dop_db_erase", rt_pl_dop_db_erase }, { "$db_abolish", 1, "rt_pl_dop_db_abolish", rt_pl_dop_db_abolish }, { "$db_nonempty", 1, "rt_pl_dop_db_nonempty",
            rt_pl_dop_db_nonempty }, { "$ball_pending", 0, "rt_pl_dop_ball_pending", rt_pl_dop_ball_pending }, { "$db_seed_once", 5, "rt_pl_dop_db_seed_once", rt_pl_dop_db_seed_once },
        { "$db_retractall", 2, "rt_pl_dop_db_retractall", rt_pl_dop_db_retractall },
        { "$db_alive", 2, "rt_pl_dop_db_alive", rt_pl_dop_db_alive },
        { "$co_start", 4, "rt_pl_dop_co_start", rt_pl_dop_co_start }, { "$co_resume", 5, "rt_pl_dop_co_resume", rt_pl_dop_co_resume },
        { "$co_yield", 2, "rt_pl_dop_co_yield", rt_pl_dop_co_yield }, { "$co_unwound", 2, "rt_pl_dop_co_unwound", rt_pl_dop_co_unwound },
        { "$co_finish", 2, "rt_pl_dop_co_finish", rt_pl_dop_co_finish }, { "$co_mark", 0, "rt_pl_dop_co_mark", rt_pl_dop_co_mark }, { "$co_b0", 0, "rt_pl_dop_co_b0", rt_pl_dop_co_b0 },
        { "$co_inside", 0, "rt_pl_dop_co_inside", rt_pl_dop_co_inside },
        { "$engine_new", 4, "rt_pl_dop_engine_new", rt_pl_dop_engine_new }, { "$engine_next", 2, "rt_pl_dop_engine_next", rt_pl_dop_engine_next },
        { "$engine_yield", 1, "rt_pl_dop_engine_yield", rt_pl_dop_engine_yield }, { "$engine_destroy", 1, "rt_pl_dop_engine_destroy", rt_pl_dop_engine_destroy },
        { "$engine_self", 1, "rt_pl_dop_engine_self", rt_pl_dop_engine_self }, { "$engine_id", 2, "rt_pl_dop_engine_id", rt_pl_dop_engine_id },
        { "$engine_list", 1, "rt_pl_dop_engine_list", rt_pl_dop_engine_list }, { "$engine_reaped", 1, "rt_pl_dop_engine_reaped", rt_pl_dop_engine_reaped },
        { "$engine_stats", 4, "rt_pl_dop_engine_stats", rt_pl_dop_engine_stats }, { "$gc_now", 0, "rt_pl_dop_gc_now", rt_pl_dop_gc_now },
        { "$db_store_k", 1, "rt_pl_dop_db_store_k", rt_pl_dop_db_store_k }, { "$db_copy", 2, "rt_pl_dop_db_copy", rt_pl_dop_db_copy }, { "$mod_get", 3, "rt_pl_dop_mod_get", rt_pl_dop_mod_get },
            { "$mod_set", 3, "rt_pl_dop_mod_set", rt_pl_dop_mod_set }, { "$pl_cp_count", 1, "rt_pl_dop_pl_cp_count",
            rt_pl_dop_pl_cp_count }, { "$pl_cp_nth", 3, "rt_pl_dop_pl_cp_nth", rt_pl_dop_pl_cp_nth }, { "$pl_pp_guard", 1, "rt_pl_dop_pl_pp_guard", rt_pl_dop_pl_pp_guard }, { "$pl_pp_count",
                2, "rt_pl_dop_pl_pp_count", rt_pl_dop_pl_pp_count }, { "$pl_pp_nth", 3, "rt_pl_dop_pl_pp_nth", rt_pl_dop_pl_pp_nth }, { "$pl_cp_guard", 1, "rt_pl_dop_pl_cp_guard",
                    rt_pl_dop_pl_cp_guard },
        { "$db_t_guard", 2, "rt_pl_dop_db_t_guard", rt_pl_dop_db_t_guard }, { "$db_assertz_t", 3, "rt_pl_dop_db_assertz_t", rt_pl_dop_db_assertz_t }, { "$db_asserta_t", 1,
            "rt_pl_dop_db_asserta_t", rt_pl_dop_db_asserta_t },
        { "$db_consult_t", 3, "rt_pl_dop_db_consult_t", rt_pl_dop_db_consult_t }, { "$db_consult_done", 1, "rt_pl_dop_db_consult_done", rt_pl_dop_db_consult_done },
        { "$db_erase_t", 2, "rt_pl_dop_db_erase_t", rt_pl_dop_db_erase_t }, { "$db_abolish_t", 1, "rt_pl_dop_db_abolish_t", rt_pl_dop_db_abolish_t }, { "$db_retractall_t", 1,
            "rt_pl_dop_db_retractall_t", rt_pl_dop_db_retractall_t }, { "$db_asserta_r", 2, "rt_pl_dop_db_asserta_r", rt_pl_dop_db_asserta_r }, { "$db_assertz_r", 2, "rt_pl_dop_db_assertz_r",
                rt_pl_dop_db_assertz_r }, { "$db_erase_ref", 1, "rt_pl_dop_db_erase_ref", rt_pl_dop_db_erase_ref }, { "$db_n_r", 2, "rt_pl_dop_db_n_r", rt_pl_dop_db_n_r }, { "$db_at_r", 3,
                    "rt_pl_dop_db_at_r", rt_pl_dop_db_at_r }, { "$db_ref_r", 3, "rt_pl_dop_db_ref_r", rt_pl_dop_db_ref_r },
        { "$nb_setval", 2, "rt_pl_dop_nb_setval", rt_pl_dop_nb_setval }, { "$nb_getval", 2, "rt_pl_dop_nb_getval", rt_pl_dop_nb_getval }, { "$b_setval", 2, "rt_pl_dop_b_setval", rt_pl_dop_b_setval },
        { "$nb_delete", 2, "rt_pl_dop_nb_delete", rt_pl_dop_nb_delete },
        { "$trie_new", 1, "rt_pl_dop_trie_new", rt_pl_dop_trie_new }, { "$trie_alive", 1, "rt_pl_dop_trie_alive", rt_pl_dop_trie_alive },
        { "$trie_destroy", 1, "rt_pl_dop_trie_destroy", rt_pl_dop_trie_destroy }, { "$trie_insert", 5, "rt_pl_dop_trie_insert", rt_pl_dop_trie_insert },
        { "$trie_lookup", 3, "rt_pl_dop_trie_lookup", rt_pl_dop_trie_lookup }, { "$trie_delete", 3, "rt_pl_dop_trie_delete", rt_pl_dop_trie_delete },
        { "$trie_entries", 2, "rt_pl_dop_trie_entries", rt_pl_dop_trie_entries }, { "$trie_entry_key", 3, "rt_pl_dop_trie_entry_key", rt_pl_dop_trie_entry_key },
        { "$trie_count", 2, "rt_pl_dop_trie_count", rt_pl_dop_trie_count }, { "$trie_meta_set", 3, "rt_pl_dop_trie_meta_set", rt_pl_dop_trie_meta_set },
        { "$trie_meta_get", 3, "rt_pl_dop_trie_meta_get", rt_pl_dop_trie_meta_get }, { "$tbl_vt", 1, "rt_pl_dop_tbl_vt", rt_pl_dop_tbl_vt },
        { "$ssu_begin", 1, "rt_pl_dop_ssu_begin", rt_pl_dop_ssu_begin }, { "$ssu_check", 1, "rt_pl_dop_ssu_check", rt_pl_dop_ssu_check },
        { "$tbh_close", 0, "rt_pl_dop_tbh_close", rt_pl_dop_tbh_close }, { "$tbl_park", 1, "rt_pl_dop_tbl_park", rt_pl_dop_tbl_park }, { "$tbl_sl", 3, "rt_pl_dop_tbl_sl", rt_pl_dop_tbl_sl },
            { "$tbl_resume_all", 1, "rt_pl_dop_tbl_resume_all",
            rt_pl_dop_tbl_resume_all }, { "$tbh_leading", 0, "rt_pl_dop_tbh_leading", rt_pl_dop_tbh_leading },
        { "$tbh_get", 2, "rt_pl_dop_tbh_get", rt_pl_dop_tbh_get }, { "$tbh_own", 2, "rt_pl_dop_tbh_own", rt_pl_dop_tbh_own }, { "$tbh_set", 2, "rt_pl_dop_tbh_set", rt_pl_dop_tbh_set },
        { "$tbh_bset", 2, "rt_pl_dop_tbh_bset", rt_pl_dop_tbh_bset }, { "$tbl_dlget", 1, "rt_pl_dop_tbl_dlget", rt_pl_dop_tbl_dlget },
        { "$tbl_dlset", 1, "rt_pl_dop_tbl_dlset", rt_pl_dop_tbl_dlset }, { "$tx_db", 2, "rt_pl_dop_tx_db", rt_pl_dop_tx_db }, { "$mod_ext", 4, "rt_pl_dop_mod_ext", rt_pl_dop_mod_ext },
        { "$trie_slice", 4, "rt_pl_dop_trie_slice", rt_pl_dop_trie_slice },
        { "$cpu_ms", 1, "rt_pl_dop_cpu_ms", rt_pl_dop_cpu_ms },
        { "$pl_nb_getval_guard", 2, "rt_pl_dop_nb_getval_guard", rt_pl_dop_nb_getval_guard },
        { "$ax_eguard", 1, "rt_pl_dop_ax_eguard", rt_pl_dop_ax_eguard },
        { "$quit_trap_300", 0, "rt_quit_trap_300", rt_quit_trap_300 }, { "$quit_trap_320", 0, "rt_quit_trap_320", rt_quit_trap_320 },
        { "$pl_char_guard", 1, "rt_pl_dop_char_guard", rt_pl_dop_char_guard },
        { "$pl_between_guard", 3, "rt_pl_dop_between_guard", rt_pl_dop_between_guard },
        { "$pl_stream_guard", 2, "rt_pl_dop_stream_guard", rt_pl_dop_stream_guard },
        { "$pl_curstream_guard", 1, "rt_pl_dop_curstream_guard", rt_pl_dop_curstream_guard },
        { "$pl_goal_guard", 1, "rt_pl_dop_goal_guard", rt_pl_dop_goal_guard }, { "$pl_list_guard", 1, "rt_pl_dop_list_guard", rt_pl_dop_list_guard }, { "$pl_declared", 2,
            "rt_pl_dop_pl_declared", rt_pl_dop_pl_declared }, { "$pl_dynamic", 1, "rt_pl_dop_pl_dynamic", rt_pl_dop_pl_dynamic },
        { "$term_string", 2, "rt_pl_dop_term_string", rt_pl_dop_term_string }, { "$term_to_atom", 2, "rt_pl_dop_term_to_atom", rt_pl_dop_term_to_atom },
        { "$atom_length", 2, "rt_pl_dop_atom_length", rt_pl_dop_atom_length },
        { "$atom_concat", 3, "rt_pl_dop_atom_concat", rt_pl_dop_atom_concat }, { "$atomic_concat", 3, "rt_pl_dop_atomic_concat", rt_pl_dop_atomic_concat }, { "$atom_chars", 2,
            "rt_pl_dop_atom_chars", rt_pl_dop_atom_chars },
        { "$atom_codes", 2, "rt_pl_dop_atom_codes", rt_pl_dop_atom_codes }, { "$atom_number", 2, "rt_pl_dop_atom_number", rt_pl_dop_atom_number },
        { "$atom_string", 2, "rt_pl_dop_atom_string", rt_pl_dop_atom_string }, { "$upcase_atom", 2, "rt_pl_dop_upcase_atom", rt_pl_dop_upcase_atom },
        { "$downcase_atom", 2, "rt_pl_dop_downcase_atom", rt_pl_dop_downcase_atom }, { "$string_concat", 3, "rt_pl_dop_string_concat", rt_pl_dop_string_concat },
        { "$string_length", 2, "rt_pl_dop_string_length", rt_pl_dop_string_length }, { "$string_lower", 2, "rt_pl_dop_string_lower", rt_pl_dop_string_lower },
        { "$string_upper", 2, "rt_pl_dop_string_upper", rt_pl_dop_string_upper }, { "$string_to_atom", 2, "rt_pl_dop_string_to_atom", rt_pl_dop_string_to_atom }, { "$string_codes", 2,
            "rt_pl_dop_string_codes", rt_pl_dop_string_codes }, { "$string_chars", 2, "rt_pl_dop_string_chars", rt_pl_dop_string_chars },
        { "$number_string", 2, "rt_pl_dop_number_string", rt_pl_dop_number_string }, { "$atomic_list_concat", 2, "rt_pl_dop_atomic_list_concat", rt_pl_dop_atomic_list_concat },
        { "$atomic_list_concat", 3, "rt_pl_dop_atomic_list_concat", rt_pl_dop_atomic_list_concat }, { "$concat_atom", 2, "rt_pl_dop_concat_atom", rt_pl_dop_concat_atom },
        { "$concat_atom", 3, "rt_pl_dop_concat_atom", rt_pl_dop_concat_atom }, { "$char_code", 2, "rt_pl_dop_char_code", rt_pl_dop_char_code },
        { "$gnu_sort1", 1, "rt_pl_dop_gnu_sort1", rt_pl_dop_gnu_sort1 }, { "$gnu_msort1", 1, "rt_pl_dop_gnu_msort1", rt_pl_dop_gnu_msort1 },
        { "$gnu_keysort1", 1, "rt_pl_dop_gnu_keysort1", rt_pl_dop_gnu_keysort1 }, { "$gnu_line_count", 2, "rt_pl_dop_gnu_line_count", rt_pl_dop_gnu_line_count },
        { "$gnu_line_position", 2, "rt_pl_dop_gnu_line_position", rt_pl_dop_gnu_line_position }, { "$put_token", 2, "rt_pl_dop_put_token", rt_pl_dop_put_token }, { "$sha_hash", 3,
            "rt_pl_dop_sha_hash", rt_pl_dop_sha_hash }, { "$gnu_character_count", 2,
            "rt_pl_dop_gnu_character_count", rt_pl_dop_gnu_character_count },
        { "$gnu_stream_line_column", 3, "rt_pl_dop_gnu_stream_line_column", rt_pl_dop_gnu_stream_line_column },
        { "$gnu_last_read_start", 2, "rt_pl_dop_gnu_last_read_start", rt_pl_dop_gnu_last_read_start },
        { "$gnu_absolute_file_name", 2, "rt_pl_dop_gnu_absolute_file_name", rt_pl_dop_gnu_absolute_file_name },
        { "$gnu_prolog_file_name", 2, "rt_pl_dop_gnu_prolog_file_name", rt_pl_dop_gnu_prolog_file_name }, { "$gnu_builtin", 2, "rt_pl_dop_gnu_builtin", rt_pl_dop_gnu_builtin },
        { "$gnu_working_directory", 1, "rt_pl_dop_gnu_working_directory", rt_pl_dop_gnu_working_directory },
        { "$gnu_change_directory", 1, "rt_pl_dop_gnu_change_directory", rt_pl_dop_gnu_change_directory },
        { "$gnu_make_directory", 1, "rt_pl_dop_gnu_make_directory", rt_pl_dop_gnu_make_directory },
        { "$gnu_os", 3, "rt_pl_dop_gnu_os", rt_pl_dop_gnu_os },
        { "$compound0", 3, "rt_pl_dop_compound0", rt_pl_dop_compound0 },
        { "$set_stream", 2, "rt_pl_dop_set_stream", rt_pl_dop_set_stream },
        { "$gnu_delete_file", 1, "rt_pl_dop_gnu_delete_file", rt_pl_dop_gnu_delete_file },
        { "$gnu_file_exists", 1, "rt_pl_dop_gnu_file_exists", rt_pl_dop_gnu_file_exists },
        { "$gnu_directory_files", 2, "rt_pl_dop_gnu_directory_files", rt_pl_dop_gnu_directory_files },
        { "$gnu_environ_list", 1, "rt_pl_dop_gnu_environ_list", rt_pl_dop_gnu_environ_list },
        { "$gnu_file_props", 2, "rt_pl_dop_gnu_file_props", rt_pl_dop_gnu_file_props },
        { "$gnu_term_hash", 2, "rt_pl_dop_gnu_term_hash", rt_pl_dop_gnu_term_hash },
        { "$gnu_term_hash4", 4, "rt_pl_dop_gnu_term_hash4", rt_pl_dop_gnu_term_hash4 },
        { "$gnu_prolog_pid", 1, "rt_pl_dop_gnu_prolog_pid", rt_pl_dop_gnu_prolog_pid },
        { "$pipe", 2, "rt_pl_dop_pipe", rt_pl_dop_pipe },
        { "$open_null_stream", 1, "rt_pl_dop_open_null_stream", rt_pl_dop_open_null_stream },
        { "$number_codes", 2, "rt_pl_dop_number_codes", rt_pl_dop_number_codes }, { "$number_chars", 2, "rt_pl_dop_number_chars", rt_pl_dop_number_chars },
        { "$name", 2, "rt_pl_dop_name", rt_pl_dop_name }, { "$get_char", 1, "rt_pl_dop_get_char", rt_pl_dop_get_char }, { "$peek_char", 1, "rt_pl_dop_peek_char", rt_pl_dop_peek_char },
        { "$get_code", 1, "rt_pl_dop_get_code", rt_pl_dop_get_code }, { "$peek_code", 1, "rt_pl_dop_peek_code", rt_pl_dop_peek_code },
        { "$get_byte", 1, "rt_pl_dop_get_byte", rt_pl_dop_get_byte }, { "$peek_byte", 1, "rt_pl_dop_peek_byte", rt_pl_dop_peek_byte },
        { "$unget_char", 1, "rt_pl_dop_unget_char", rt_pl_dop_unget_char }, { "$unget_code", 1, "rt_pl_dop_unget_code", rt_pl_dop_unget_code }, { "$unget_byte", 1, "rt_pl_dop_unget_byte",
            rt_pl_dop_unget_byte }, { "$put_code", 1, "dop_pl_put_code", dop_pl_put_code },
        { "$put_byte", 1, "rt_pl_dop_put_byte", rt_pl_dop_put_byte },
        { "$current_prolog_flag", 2, "rt_pl_dop_current_prolog_flag", rt_pl_dop_current_prolog_flag }, { "$set_prolog_flag", 2, "rt_pl_dop_set_prolog_flag", rt_pl_dop_set_prolog_flag },
            { "$telling", 1, "rt_pl_dop_telling", rt_pl_dop_telling }, { "$seeing", 1, "rt_pl_dop_seeing", rt_pl_dop_seeing },
        { "$tell", 1, "rt_pl_dop_tell", rt_pl_dop_tell }, { "$append1", 1, "rt_pl_dop_append1", rt_pl_dop_append1 }, { "$see", 1, "rt_pl_dop_see", rt_pl_dop_see },
        { "$told", 0, "dop_pl_told", dop_pl_told }, { "$seen", 0, "dop_pl_seen", dop_pl_seen },
        { "$put_char_c", 1, "rt_pl_dop_put_char_c", rt_pl_dop_put_char_c }, { "$put_char_c_s", 2, "rt_pl_dop_put_char_c_s", rt_pl_dop_put_char_c_s },
        { "$at_end_of_stream", 0, "dop_pl_at_end_of_stream", dop_pl_at_end_of_stream }, { "$at_end_of_stream_s", 1, "rt_pl_dop_at_end_of_stream_s", rt_pl_dop_at_end_of_stream_s },
        { "$get_edin", 1, "rt_pl_dop_get_edin", rt_pl_dop_get_edin }, { "$put_edin", 1, "rt_pl_dop_put_edin", rt_pl_dop_put_edin }, { "$skip", 1, "rt_pl_dop_skip", rt_pl_dop_skip },
        { "$get_code_s", 2, "rt_pl_dop_get_code_s", rt_pl_dop_get_code_s }, { "$peek_code_s", 2, "rt_pl_dop_peek_code_s", rt_pl_dop_peek_code_s },
        { "$get_byte_s", 2, "rt_pl_dop_get_byte_s", rt_pl_dop_get_byte_s }, { "$peek_byte_s", 2, "rt_pl_dop_peek_byte_s", rt_pl_dop_peek_byte_s },
        { "$unget_char_s", 2, "rt_pl_dop_unget_char_s", rt_pl_dop_unget_char_s }, { "$unget_code_s", 2, "rt_pl_dop_unget_code_s", rt_pl_dop_unget_code_s }, { "$unget_byte_s", 2,
            "rt_pl_dop_unget_byte_s", rt_pl_dop_unget_byte_s }, { "$put_byte_s", 2, "rt_pl_dop_put_byte_s", rt_pl_dop_put_byte_s },
        { "$put_code_s", 2, "rt_pl_dop_put_code_sb", rt_pl_dop_put_code_sb },
        { "$read", 1, "rt_pl_dop_read", rt_pl_dop_read }, { "$atom_to_term", 3, "rt_pl_dop_atom_to_term", rt_pl_dop_atom_to_term },
        { "$display", 1, "rt_pl_dop_display", rt_pl_dop_display }, { "$display_s", 2, "rt_pl_dop_display_s", rt_pl_dop_display_s }, { "$unify_oc", 2, "rt_pl_dop_unify_oc", rt_pl_dop_unify_oc },
        { "$aggregate_reduce", 3, "rt_pl_dop_aggregate_reduce", rt_pl_dop_aggregate_reduce },
        { "$read_term_opts", 2, "rt_pl_dop_read_term_opts", rt_pl_dop_read_term_opts }, { "$read_term_opts_s", 3, "rt_pl_dop_read_term_opts_s", rt_pl_dop_read_term_opts_s },
        { "$wot_open", 3, "rt_pl_dop_wot_open", rt_pl_dop_wot_open }, { "$wot_capture", 4, "rt_pl_dop_wot_capture", rt_pl_dop_wot_capture }, { "$wot_discard", 3, "rt_pl_dop_wot_discard",
            rt_pl_dop_wot_discard },
        { "$set_prolog_flag_declare", 2, "rt_pl_dop_set_prolog_flag_declare", rt_pl_dop_set_prolog_flag_declare },
        { "$read_term_from_atom", 3, "rt_pl_dop_read_term_from_atom", rt_pl_dop_read_term_from_atom }, { "$read_term_from_chars", 3, "rt_pl_dop_read_term_from_chars",
            rt_pl_dop_read_term_from_chars }, { "$read_term_from_codes", 3, "rt_pl_dop_read_term_from_codes", rt_pl_dop_read_term_from_codes },
        { "$ground", 1, "dop_pl_ground", dop_pl_ground }, { "$is_list", 1, "dop_pl_is_list", dop_pl_is_list }, { "$acyclic_term", 1, "dop_pl_acyclic_term", dop_pl_acyclic_term }, { "$between_special",
            2, "dop_pl_between_special", dop_pl_between_special },
        { "$writeq", 1, "dop_pl_writeq", dop_pl_writeq }, { "$write_canonical", 1, "dop_pl_write_canonical", dop_pl_write_canonical }, { "$writeln", 1, "dop_pl_writeln", dop_pl_writeln },
        { "$tab", 1, "dop_pl_tab", dop_pl_tab }, { "$put_char", 1, "dop_pl_put_char", dop_pl_put_char }, { "$halt", 0, "rt_pl_dop_halt", rt_pl_dop_halt }, { "$halt", 1, "rt_pl_dop_halt",
            rt_pl_dop_halt },
        { "$flush_output", 0, "dop_pl_flush_output", dop_pl_flush_output }, { "$format", 1, "rt_pl_dop_format", rt_pl_dop_format }, { "$format", 2, "rt_pl_dop_format", rt_pl_dop_format },
        { "$write_s", 2, "rt_pl_dop_write_sb", rt_pl_dop_write_sb }, { "$writeq_s", 2, "rt_pl_dop_writeq_sb", rt_pl_dop_writeq_sb }, { "$write_canonical_s", 2, "rt_pl_dop_write_canonical_sb",
            rt_pl_dop_write_canonical_sb },
        { "$writeln_s", 2, "rt_pl_dop_writeln_sb", rt_pl_dop_writeln_sb }, { "$nl_s", 1, "rt_pl_dop_nl_sb", rt_pl_dop_nl_sb }, { "$tab_s", 2, "rt_pl_dop_tab_sb", rt_pl_dop_tab_sb },
            { "$put_char_s", 2, "rt_pl_dop_put_char_sb", rt_pl_dop_put_char_sb },
        { "$flush_output_s", 1, "rt_pl_dop_flush_output_sb", rt_pl_dop_flush_output_sb }, { "$set_output", 1, "dop_pl_set_output", dop_pl_set_output }, { "$set_input", 1, "dop_pl_set_input",
            dop_pl_set_input },
        { "$close", 1, "rt_pl_dop_close", rt_pl_dop_close }, { "$close", 2, "rt_pl_dop_close", rt_pl_dop_close }, { "$read_s", 2, "rt_pl_dop_read_s", rt_pl_dop_read_s }, { "$get_char_s", 2,
            "rt_pl_dop_get_char_s", rt_pl_dop_get_char_s },
        { "$peek_char_s", 2, "rt_pl_dop_peek_char_s", rt_pl_dop_peek_char_s }, { "$current_output", 1, "rt_pl_dop_current_output", rt_pl_dop_current_output }, { "$current_input", 1,
            "rt_pl_dop_current_input", rt_pl_dop_current_input },
        { "$open", 3, "rt_pl_dop_open", rt_pl_dop_open }, { "$open4", 4, "rt_pl_dop_open4", rt_pl_dop_open4 }, { "$keysort", 2, "rt_pl_dop_keysort", rt_pl_dop_keysort },
            { "$set_stream_position", 2, "rt_pl_dop_set_stream_position", rt_pl_dop_set_stream_position }, { "$format3", 3, "rt_pl_dop_format3", rt_pl_dop_format3 }, { "$write_term", 2,
                "rt_pl_dop_write_term", rt_pl_dop_write_term }, { "$write_term_s", 3, "rt_pl_dop_write_term_s", rt_pl_dop_write_term_s }, { "$op", 3, "rt_pl_dop_op", rt_pl_dop_op },
                    { "$pl_op_check", 3, "rt_pl_dop_pl_op_check", rt_pl_dop_pl_op_check }, { "$heap_guard", 2, "rt_pl_dop_heap_guard", rt_pl_dop_heap_guard }, { "$pl_sp_check", 2,
                        "rt_pl_dop_pl_sp_check", rt_pl_dop_pl_sp_check }, { "$pl_ioarg", 2,
                        "rt_pl_dop_pl_ioarg", rt_pl_dop_pl_ioarg }, { "$cutcall", 2, "rt_pl_dop_cutcall", rt_pl_dop_cutcall }, { "$shc", 1, "rt_pl_dop_shc", rt_pl_dop_shc }, { "$db_clause_src", 3,
                            "rt_pl_dop_db_clause_src", rt_pl_dop_db_clause_src }, { "$db_unload_file", 1, "rt_pl_dop_db_unload_file", rt_pl_dop_db_unload_file },
        { "$alarm_push", 2, "rt_pl_dop_alarm_push", rt_pl_dop_alarm_push }, { "$alarm_pop", 1, "rt_pl_dop_alarm_pop", rt_pl_dop_alarm_pop },
        { "$os_sleep", 1, "rt_pl_dop_os_sleep", rt_pl_dop_os_sleep }, { "$sk_socket", 2, "rt_pl_dop_sk_socket", rt_pl_dop_sk_socket }, { "$sk_close", 2, "rt_pl_dop_sk_close", rt_pl_dop_sk_close },
        { "$sk_bind", 3, "rt_pl_dop_sk_bind", rt_pl_dop_sk_bind }, { "$sk_listen", 3, "rt_pl_dop_sk_listen", rt_pl_dop_sk_listen }, { "$sk_accept", 2, "rt_pl_dop_sk_accept", rt_pl_dop_sk_accept },
        { "$sk_connect", 3, "rt_pl_dop_sk_connect", rt_pl_dop_sk_connect }, { "$sk_open", 2, "rt_pl_dop_sk_open", rt_pl_dop_sk_open },
        { "$sk_setopt", 4, "rt_pl_dop_sk_setopt", rt_pl_dop_sk_setopt }, { "$sk_fileno", 2, "rt_pl_dop_sk_fileno", rt_pl_dop_sk_fileno },
        { "$sk_hostname", 1, "rt_pl_dop_sk_hostname", rt_pl_dop_sk_hostname }, { "$sk_resolve", 2, "rt_pl_dop_sk_resolve", rt_pl_dop_sk_resolve },
        { "$proc_create", 7, "rt_pl_dop_proc_create", rt_pl_dop_proc_create }, { "$proc_wait", 3, "rt_pl_dop_proc_wait", rt_pl_dop_proc_wait },
        { "$proc_kill", 3, "rt_pl_dop_proc_kill", rt_pl_dop_proc_kill }, { "$fx_stat", 2, "rt_pl_dop_fx_stat", rt_pl_dop_fx_stat }, { "$fx_utime", 4, "rt_pl_dop_fx_utime", rt_pl_dop_fx_utime },
        { "$fx_symlink", 3, "rt_pl_dop_fx_symlink", rt_pl_dop_fx_symlink }, { "$fx_link", 3, "rt_pl_dop_fx_link", rt_pl_dop_fx_link }, { "$fx_chmod", 3, "rt_pl_dop_fx_chmod", rt_pl_dop_fx_chmod },
        { "$fx_readlink", 2, "rt_pl_dop_fx_readlink", rt_pl_dop_fx_readlink },
        { "$print", 1, "rt_pl_dop_print", rt_pl_dop_print }, { "$print_s", 2, "rt_pl_dop_print_s", rt_pl_dop_print_s },
        { "$pl_op_count", 1, "rt_pl_dop_pl_op_count", rt_pl_dop_pl_op_count }, { "$pl_op_nth", 4, "rt_pl_dop_pl_op_nth", rt_pl_dop_pl_op_nth },
        { "$pl_sp_count", 1, "rt_pl_dop_pl_sp_count", rt_pl_dop_pl_sp_count }, { "$pl_sp_nth", 3, "rt_pl_dop_pl_sp_nth", rt_pl_dop_pl_sp_nth },
        { "$pl_cs_count", 1, "rt_pl_dop_pl_cs_count", rt_pl_dop_pl_cs_count }, { "$pl_cs_nth", 4, "rt_pl_dop_pl_cs_nth", rt_pl_dop_pl_cs_nth },
        { "arr_get", 2, "pas_arr_get", pas_arr_get }, { "arr_set_pure", 3, "pas_arr_set", pas_arr_set },
        { "arr_make", 1, "pas_arr_make", pas_arr_make }, { "arr_make", 2, "pas_arr_make", pas_arr_make }, { "arr_make_raw", 4, "pas_arr_make_raw", pas_arr_make_raw },
        { "__pas_deref", 1, "pas_deref", pas_deref }, { "__pas_deref_set", 2, "pas_deref_set", pas_deref_set }, { "__pas_field_set", 3, "pas_field_set", pas_field_set },
        { "__pas_alloc_rec", 1, "pas_alloc_rec", pas_alloc_rec }, { "__pas_dispose", 1, "pas_dispose", pas_dispose },
        { "__pas_ptr_diff", 2, "pas_ptr_diff", pas_ptr_diff }, { "__pas_pdx", 4, "pas_pdx", pas_pdx }, { "__pas_row_get", 5, "pas_row_get", pas_row_get }, { "__pas_row_set", 6, "pas_row_set",
            pas_row_set }, { "__pas_pchar_of", 1, "pas_pchar_of", pas_pchar_of }, { "__pas_pstrlen", 1, "pas_pstrlen",
            pas_pstrlen }, { "__pas_pstr", 1, "pas_pstr",
            pas_pstr }, { "__pas_addr", 2, "pas_addr", pas_addr }, { "__pas_addr_var", 1, "pas_addr_var", pas_addr_var }, { "__pas_ptr_inc", 2, "pas_ptr_inc", pas_ptr_inc }, { "__pas_ptr_eq", 2,
            "pas_ptr_eq",
            pas_ptr_eq },
        { "__pas_iand", 2, "rt_iand_d", rt_iand_d }, { "__pas_ior", 2, "rt_ior_d", rt_ior_d }, { "__pas_ixor", 2, "rt_ixor_d", rt_ixor_d }, { "__pas_ishift", 2, "rt_ishift_d", rt_ishift_d },
        { "__pas_elem_ref", 3, "pas_elem_ref", pas_elem_ref }, { "__pas_ref_val", 1, "pas_ref_val", pas_ref_val },
        { "__pas_view", 4, "pas_view_make", pas_view_make },
        { "__pas_xr_ev", -1, "pas_xr_ev_d", pas_xr_ev_d }, { "__pas_xr_cvt", -1, "pas_xr_cvt_d", pas_xr_cvt_d }, { "__pas_resundef", 1, "rt_pas_resundef", rt_pas_resundef }, { "__pas_sin", 1,
            "rt_pas_sin", rt_pas_sin }, { "__pas_cos", 1, "rt_pas_cos", rt_pas_cos },
        { "__pas_exp", 1, "rt_pas_exp", rt_pas_exp }, { "__pas_sqrt", 1, "rt_pas_sqrt", rt_pas_sqrt }, { "__pas_ln", 1, "rt_pas_ln", rt_pas_ln }, { "__pas_arctan", 1, "rt_pas_arctan", rt_pas_arctan },
        { "__pas_sqr", 1, "rt_pas_sqr", rt_pas_sqr },
        { "__rk_arr_at", 2, "rt_rk_arr_at", rt_rk_arr_at }, { "__rk_arr_set", 3, "rt_rk_arr_set", rt_rk_arr_set }, { "__rk_elem_put", 4, "rt_rk_elem_put", rt_rk_elem_put },
        { "__param_check", -1, "rt_rk_param_check", rt_rk_param_check }, { "__rk_assign_check", 4, "rt_rk_assign_check", rt_rk_assign_check },
        { "nqp::bindattr", 4, "rt_rk_bindattr", rt_rk_bindattr }, { "nqp::bindattr_n", 4, "rt_rk_bindattr", rt_rk_bindattr }, { "nqp::bindattr_i", 4, "rt_rk_bindattr", rt_rk_bindattr },
        { "nqp::bindattr_s", 4, "rt_rk_bindattr", rt_rk_bindattr }, { "__rk_box", 1, "rt_rk_box", rt_rk_box }, { "__blk_ref", 1, "rt_rk_blk_ref", rt_rk_blk_ref },
        { "__rk_undef", 0, "rt_rk_undef", rt_rk_undef }, { "__rk_undef", -1, "rt_rk_undef", rt_rk_undef }, { "__rk_range_arr", 2, "rt_rk_range_arr", rt_rk_range_arr },
        { "__rk_range_arr", 3, "rt_rk_range_arr", rt_rk_range_arr }, { "__rk_iter_src", 0, "rt_rk_iter_src", rt_rk_iter_src }, { "__rk_iter_src", 1, "rt_rk_iter_src", rt_rk_iter_src },
        { "__rk_map_append", 2, "rt_rk_map_append", rt_rk_map_append }, { "__rk_grep_append", 2, "rt_rk_grep_append", rt_rk_grep_append },
        { "__rk_byref_assign", 2, "rt_rk_byref_assign", rt_rk_byref_assign },
        { "", 0, "", 0 } };
    for (int i = 0; t[i].nm[0]; i++) if (!strcmp(fn, t[i].nm) && (t[i].ar < 0 ? narg >= 1 : narg == t[i].ar)) {
        const char * nd = getenv("SCRIP_NO_DOP"); if (nd && nd[0] == '1') return (void *)0;
        const char * sk = getenv("SCRIP_DOP_SKIP"); if (sk && sk[0] && strstr(sk, t[i].nm + 1)) return (void *)0;
        *sym = t[i].sy; return (void *)(uintptr_t)t[i].fp;
    }
    return (void *)0;
}
extern "C" {
extern "C" int dop_direct_leaf_known(const char * fn, int narg) {
    const char * s = 0;
    return (dop_direct_fp(fn, (int64_t)narg, &s) || pl_leaf_inline_known(fn, narg)) ? 1 : 0;
}
}
void defer_prepare(void) {
    extern int g_gva_active;
    int vslot = -1;
    { const char * sv = g_emit.op_sval, * d = sv ? strstr(sv, "$V") : 0;
      if (d && d[2] >= '0' && d[2] <= '9') { char * e = 0; long k = strtol(d + 2, &e, 10); if (e && !*e) vslot = (int)k; } }
    int gva_road = (vslot < 0 && g_gva_active && g_emit.op_gva_k >= 0);
    int star = (vslot < 0 && !gva_road && g_emit.op_sval && g_emit.op_sval[0] == '*' && g_emit.op_sval[1]) ? 1 : 0;
    if (gva_road || vslot >= 0 || star) g_emit.op_df_b[0] = 0; else strtab_label(g_emit.op_df_b, sizeof g_emit.op_df_b, g_emit.op_sval ? g_emit.op_sval : "");
    int merged = (vslot < 0 && !(g_gva_active && g_emit.op_gva_k >= 0));
    int msite = (merged && !star) ? (g_emitter.defer_prepare_g_defer_site_n < 1024 ? g_emitter.defer_prepare_g_defer_site_n++ : -1) : -1;
    g_emit.op_df_vslot = vslot;
    g_emit.op_df_site = msite;
}
void emit_diag_arbno_arm(const char * arm) { if (emit_knob_if_one("SCRIP_ARBNO_DIAG")) fprintf(stderr, "[ARBNO-ARM] %s\n", arm); }
long emit_lit_d(const char * s, long k) { uint32_t w; memcpy(&w, s + k, 4); return (long)(int32_t)w; }
uint64_t emit_double_bits(double d) { uint64_t b = 0; memcpy(&b, &d, 8); return b; }
const char * emit_rec_or_empty(void * r) { return r ? (const char *)r : ""; }
static void call_margs_reserve(int n) { cv_reserve(&g_emit.op_marg, (uint32_t)sizeof(marg_t), (uint64_t)(n > 0 ? n : 1), "op_marg"); }
static const char * call_cv_text(cv_t * buf, const std::string & s) { cv_reserve(buf, 1, (uint64_t)s.size() + 1, "call_cv_text"); memcpy(buf->p, s.c_str(), s.size() + 1); return (const char *)buf->p;
    }
static void call_fnlbl_set(const std::string & fl, int guard) {
    g_emit.op_call_fnlbl = call_cv_text(&g_emit.op_call_fnlbl_buf, fl);
    if (!guard) { g_emit.op_call_fnlbl_fresh = 1; return; }
    { bb_label_t * dm = emit_label_intern((fl + "$def").c_str()); g_emit.op_call_fnlbl_fresh = (!dm || !bb_label_defined(dm)) ? 1 : 0; if (g_emit.op_call_fnlbl_fresh && dm) dm->offset = 0; }
}
static void call_pl_prepare(IR_t * nd, const char * fn, int narg, int fnroute) {
    const char * op = 0; int k = pl_leaf_kind(fn, narg, &op); IR_t * a0 = fnroute ? ir_call_arg(nd, 0) : (IR_t *)0;
    g_emit.op_pl_kind = k; g_emit.op_pl_op = op ? op : "";
    g_emit.op_pl_sub = k == PLK_AX ? pl_ax_sub(op) : k == PLK_TYPE ? pl_type_sub(op) : k == PLK_ATOP ? pl_atop_sub(op) : 0;
    g_emit.op_pl_tags = k == PLK_TYPE ? pl_type_tags(op) : "";
    g_emit.op_pl_rat = k == PLK_TYPE ? pl_type_rat(op) : 0;
    g_emit.op_pl_jcc = (k == PLK_CMP || k == PLK_ATOP) ? pl_cmp_fail_jcc(op) : "je";
    if (k == PLK_ANUM) g_emit.op_pl_rules = pl_anum_rules((a0 && a0->op == IR_LIT_STRING) ? IR_LIT(a0).sval : (const char *)0, narg - 1);
    if (k == PLK_MKC) { g_emit.op_pl_fid_ok = (a0 && a0->op == IR_LIT_INTEGER) ? 1 : 0; g_emit.op_pl_fid = g_emit.op_pl_fid_ok ? (long)IR_LIT(a0).ival : 0L; }
    if (k == PLK_DBDECLS) { long long * w = 0; int n = pl_db_decls_words(&w); g_emit.op_pl_db_words = w; g_emit.op_pl_db_n = n; }
    if (k == PLK_MKGROUND) { long long * w = 0; int n = (a0 && a0->op == IR_LIT_INTEGER) ? pl_ground_words((long long)IR_LIT(a0).ival, &w) : 0; g_emit.op_pl_gw = w; g_emit.op_pl_gw_n = n; }
    if (k == PLK_AX || k == PLK_CMP || k == PLK_TYPE || k == PLK_ATOP) g_emit.op_pl_oplbl =
        call_cv_text(&g_emit.op_pl_oplbl_buf, std::string(".L") + x86_boxkind() + "_plop" + std::to_string(g_flat_node_id++));
}
extern "C" {
void call_prepare(IR_t * nd) {
    const char * fn = g_emit.op_sval ? g_emit.op_sval : ""; int narg = (int)g_emit.op_ival; int strict = g_emit.op_strict; IR_e kind = nd->op; int route = lower_call_route(nd);
    int fnroute = (route == CALL_ROUTE_FN); int zd = (kind == IR_CALL_GEN) ? 0 : g_emit.op_zres;
    g_emit.op_call_blk = zrow(nd)->curmov ? 32 : 16;
    g_emit.op_call_rec = lower_call_node_has_rec(nd) ? g_emit.op_call_blk + 16 * (int)g_emit.op_ival : -1;
    g_emit.op_call_route = route; g_emit.op_call_refuse = CALL_REFUSE_NONE; g_emit.op_call_try = 0; g_emit.op_call_sym = ""; g_emit.op_call_fp = 0; g_emit.op_call_try_sym = "";
        g_emit.op_call_try_fp = 0;
    g_emit.op_call_open_sym = ""; g_emit.op_call_open_fp = 0; g_emit.op_call_bid = -1; g_emit.op_call_idx = -1; g_emit.op_call_argbase = -1; g_emit.op_call_resoff = -1; g_emit.op_call_rec_off = -1;
    g_emit.op_call_genoff = -1; g_emit.op_call_dsave = -1; g_emit.op_call_scansync = 0; g_emit.op_call_curmov = 0; g_emit.op_call_elem_access = 0; g_emit.op_call_sb_exit = 0;
    g_emit.op_call_fnlbl_fresh = 0; g_emit.op_call_fnlbl = ""; g_emit.op_call_rec_addr = 0; g_emit.op_call_rec_lbl = ""; g_emit.op_marg_n = 0; g_emit.op_pl_kind = PLK_NONE; g_emit.op_pl_sub = 0;
    g_emit.op_pl_rat = 0; g_emit.op_pl_op = ""; g_emit.op_pl_tags = ""; g_emit.op_pl_jcc = "je"; g_emit.op_pl_oplbl = ""; g_emit.op_pl_rules = (const int *)0; g_emit.op_pl_fid_ok = 0;
    g_emit.op_pl_fid = 0; g_emit.op_pl_db_words = (const long long *)0; g_emit.op_pl_db_n = 0; g_emit.op_pl_gw = (const long long *)0; g_emit.op_pl_gw_n = 0;
    if (!zd) {
        g_emit.op_call_resoff = fnroute ? bcfn_result_slot(nd) : zrow(nd)->loff;
        if (g_emit.op_call_resoff < 0) g_emit.op_call_refuse = CALL_REFUSE_NO_SLOT;
        else if (narg > nd->n_operands) g_emit.op_call_refuse = CALL_REFUSE_ARGC;
        g_emit.op_call_argbase = zrow(nd)->argv; if (g_emit.op_call_argbase < 0) g_emit.op_call_argbase = g_emit.op_call_resoff + 16;
        g_emit.op_call_genoff = g_emit.op_call_resoff + 16 * (1 + narg);
    }
    if (fnroute && !g_emit.op_call_refuse && nd->seal == IR_SEAL_CALL_DET_LEAF && kind != IR_CALL_DIRECT && kind != IR_CALL_PL_LEAF) g_emit.op_call_refuse = CALL_REFUSE_DET_LEAF;
    if (!zd && !g_emit.op_call_refuse) { call_margs_reserve(narg); g_emit.op_marg_n = narg;
        for (int i = narg - 1; i >= 0; i--) call_marg_plan(i, fnroute ? ir_call_arg(nd, i) : (IR_t *)0, g_emit.op_call_argbase + 16 * i, nd); }
    g_emit.op_call_bid = bid_bake_of(fn);
    g_emit.op_call_elem_access = fnroute ? elem_access_kind(nd) : 0;
    g_emit.op_call_sb_exit = (fnroute && zd && g_emit.op_sb) ? 1 : 0;
    if (!fnroute && !zd && kind != IR_CALL_GEN) { g_emit.op_call_scansync = lower_call_is_scan_builtin(fn); g_emit.op_call_curmov = lower_call_is_curmov(fn);
        g_emit.op_call_dsave = g_emit.op_call_argbase + 16 * narg; }
    if (zd && !fnroute && kind == IR_CALL_BYNAME) { g_emit.op_call_scansync = lower_call_is_scan_builtin(fn); if (zrow(nd)->curmov) { g_emit.op_call_curmov = 1;
        g_emit.op_call_dsave = zrow(nd)->dsave; } }
    if (kind == IR_CALL_GEN) g_emit.op_call_scansync = lower_call_is_scan_builtin(fn);
    switch (kind) {
    case IR_CALL_DIRECT: { const char * s = 0; g_emit.op_call_fp = (uint64_t)(uintptr_t)dop_direct_fp(fn, (int64_t)narg, &s); g_emit.op_call_sym = s ? s : ""; break; }
    case IR_CALL_PL_LEAF: call_pl_prepare(nd, fn, narg, fnroute); break;
    case IR_CALL_PAS_BI: g_emit.op_call_idx = lower_call_pas_bi(fn); break;
    case IR_CALL_CALLEE_REC: case IR_CALL_FIELD_REC: {
        g_emit.op_call_rec_lbl = call_cv_text(&g_emit.op_call_rec_lbl_buf, std::string(".Lcallee_") + bb_ab_sym_name(fn)); g_emit.op_call_rec_addr = (uint64_t)(uintptr_t)bb_callee_rec_addr(fn); break;
            }
    case IR_CALL_BYNAME: case IR_CALL_TRY: case IR_CALL_METHOD: case IR_CALL_REDISP: case IR_CALL_APPLY: case IR_CALL_EVAL: {
        g_emit.op_call_sym = sn4_byname_sym(fn, strict); g_emit.op_call_fp = sn4_byname_fp(fn, strict); g_emit.op_call_try = lower_call_tail_try(fn, strict);
        g_emit.op_call_try_sym = sn4_byname_kind(fn, strict) == 3 ? "rt_call_arr_bl_sn4_try" : "rt_call_arr_bl_try";
        g_emit.op_call_try_fp = sn4_byname_kind(fn, strict) == 3 ? (uint64_t)(uintptr_t)(void *)rt_call_arr_bl_sn4_try : (uint64_t)(uintptr_t)(void *)rt_call_arr_bl_try;
        if (kind == IR_CALL_REDISP) { int with = (!strcmp(fn, "callwith") || !strcmp(fn, "nextwith")); g_emit.op_call_open_sym = with ? "rk_callwith_open" : "rk_callsame_open";
            g_emit.op_call_open_fp = with ? (uint64_t)(uintptr_t)(void *)rk_callwith_open : (uint64_t)(uintptr_t)(void *)rk_callsame_open; }
        if (kind == IR_CALL_METHOD || kind == IR_CALL_REDISP) g_emit.op_call_rec_off = zrow(nd)->redisp >= 0 ? zrow(nd)->redisp : zrow(nd)->argv >= 0 ? zrow(nd)->argv + 16 * narg : -1;
        if (fnroute) call_fnlbl_set(std::string(".L") + x86_boxkind() + (zd ? "_rkfnzd" : "_rkfn") + std::to_string(g_flat_node_id++), 0);
        else call_fnlbl_set(std::string(".L") + x86_boxkind() + (zd ? "_bynamefnzd" : "_bynamefn") + std::to_string((long long)g_emit.nid), 1);
        break; }
    case IR_CALL_GEN: call_fnlbl_set(std::string(".L") + x86_boxkind() + "_bynamegenfn" + std::to_string((long long)g_emit.nid), 1); break;
    default: break;
    }
}
}

#include "emitter/em/em_head.h"
#include "emitter/em/em_01_sh.h"
#include "emitter/em/em_02_sh.h"
#include "emitter/em/em_03_sh.h"
#include "emitter/em/em_04_sh.h"
#include "emitter/em/em_05_sh.h"
#include "emitter/em/em_07_sh.h"
extern "C" { static int emit_fb_bit_of(IR_t * nd); }
extern "C" { extern int is_global(const char *name); }
extern "C" { extern const char __start_rtx_entry_names[] __attribute__((visibility("hidden"))); }
extern "C" { extern const char __stop_rtx_entry_names[] __attribute__((visibility("hidden"))); }
extern "C" {
static inline void stno_rec_push(uint64_t pc, int32_t stno, int32_t line, const char * file) {
    cv_reserve(&g_emit.stno_map, (uint32_t)sizeof(sno_stno_rec_t), (uint64_t)g_emit.stno_map_n + 1, "stno_map");
    int at = g_emit.stno_map_n;
    while (at > 0 && STNO_REC(at - 1).pc > pc) at--;
    if (at < g_emit.stno_map_n) memmove(&STNO_REC(at + 1), &STNO_REC(at), (size_t)(g_emit.stno_map_n - at) * sizeof(sno_stno_rec_t));
    sno_stno_rec_t * e = &STNO_REC(at);
    e->pc = pc;
    e->stno = stno;
    e->line = line;
    e->file = file;
    g_emit.stno_map_n++;
}
g_emitter_t g_emitter;
bb_emit_mode_t bb_emit_mode = EMIT_BINARY_WIRED;
int g_sm_native_unsupported = 0;
FILE *bb_emit_out = NULL;
bb_medium_t g_medium = BB_MEDIUM_BINARY;
void (*g_emit_chain_posthook)(void) = NULL;
int g_use_sm_macros = 0;
int g_use_bb_macros = 0;
sm_emit_t g_emit;
IR_graph_t * g_emit_cfg = (IR_graph_t *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void port_exit_prepass_build(void) {
    extern const char * bb_ab_sym_name(const char *);
    for (int i = 0; i < g_stage2.proc_count; i++) {
        if (!g_stage2.proc_table[i].port_exit) continue;
        g_emitter.g_port_exit_promoting_labels.insert(std::string(bb_ab_sym_name(g_stage2.proc_table[i].name)) + "_γ");
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_port_exit_label_promotes(const char * label) {
    if (!label) return 0;
    port_exit_prepass_build();
    return g_emitter.g_port_exit_promoting_labels.count(label) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * flat_label_kind(IR_e op) { const char * n = bb_op_name(op); int j = 0; if (n && n[0] == 'I' && n[1] == 'R' && n[2] == '_') n += 3;
    if (!n) { snprintf(g_emitter.flat_label_kind_b, sizeof g_emitter.flat_label_kind_b, "op%d", (int)op); return g_emitter.flat_label_kind_b;
        } for (; n[j] && j < 47; j++) g_emitter.flat_label_kind_b[j] = (n[j] >= 'A' && n[j] <= 'Z') ? (char)(n[j] - 'A' + 'a') : n[j]; g_emitter.flat_label_kind_b[j] = 0;
        return g_emitter.flat_label_kind_b; }
const char * flat_label_kind_of(IR_t * nd) {
    if (g_emit_cfg && g_emit_cfg->n_balias > 0) for (int _bi = 0; _bi < g_emit_cfg->n_balias; _bi++) if (g_emit_cfg->balias_node[_bi] == nd) return g_emit_cfg->balias_name[_bi];
    return flat_label_kind(nd->op);
}
extern "C" void bb_label_alias_reset(void) { g_emitter.g_lblfold_n = 0; }
int g_emit_text_mode = TEXT_MODE_INVOCATION;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emitter_init_binary(bb_buf_t buf, int size)
{
    g_is_text = 0; g_emit_text_mode = TEXT_MODE_INVOCATION; g_emit_pos = 0;
    g_emitter.bb_emit_overflow = 0;
    bb_label_alias_reset();
    bb_emit_mode = EMIT_BINARY_WIRED;
    g_medium = BB_MEDIUM_BINARY;
    bb_emit_begin(buf, size);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emitter_init_text(FILE *out, int mode)
{
    g_is_text = 1; g_emit_text_mode = mode; g_emit_pos = 0;
    bb_label_alias_reset();
    bb_emit_mode = EMIT_TEXT;
    g_medium = BB_MEDIUM_TEXT;
    bb_emit_out = out ? out : stdout;
    emit_set_sink(out ? out : stdout);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void ef_text_jmp(const char *mnem, const char *target)
{ emit_textf("%s %s\n", mnem ? mnem : "", target ? target : ""); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_label_define_bb(bb_label_t *lbl)
{
    if (g_is_text) {
        emit_textf("%s:\n", lbl->name);
    } else {
        bb_emit_mode_t s = bb_emit_mode; bb_emit_mode = EMIT_BINARY_WIRED;
        bb_label_define(lbl);
        bb_emit_mode = s;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_label_initf(bb_label_t *lbl, const char *fmt, ...)
{
    va_list ap; va_start(ap, fmt);
    char nb[vfmt_len(fmt, ap)]; vsnprintf(nb, sizeof nb, fmt, ap);
    va_end(ap);
    bb_label_t *own = emit_label_alloc("%s", nb);
    lbl->name = own ? own->name : ""; lbl->name_cap = 0;
    lbl->offset = BB_LABEL_UNRESOLVED;
}
extern "C" void emit_stno_src_note(long long stno, int line, const char * file) {
    if (stno <= 0 || stno >= 0x7FFFFFFFLL) return;
    if (stno >= g_emit.stno_src_n) { cv_reserve(&g_emit.stno_src, (uint32_t)sizeof(sno_stno_src_t), (uint64_t)stno + 1, "stno_src");
        for (int64_t k = g_emit.stno_src_n; k <= stno; k++) { STNO_SRC(k).line = 0; STNO_SRC(k).file = (const char *)0; }
        g_emit.stno_src_n = stno + 1; }
    STNO_SRC(stno).line = line > 0 ? line : 0;
    STNO_SRC(stno).file = (file && *file) ? file : (const char *)0;
}
static void emit_stno_file_label(const char * file) {
    if (g_emit.stno_file_last && !strcmp(g_emit.stno_file_last, file)) return;
    g_emit.stno_file_last = file; int lb = ++g_emit.stno_file_lbl;
    std::string q;
    for (const unsigned char * c = (const unsigned char *)file; *c; c++) {
        if (*c == '"' || *c == '\\') { q += '\\'; q += (char)*c; }
        else if (*c < 32 || *c > 126) { char ob[8]; snprintf(ob, sizeof ob, "\\%03o", (unsigned)*c); q += ob; }
        else q += (char)*c; }
    emit_textf("  .pushsection .rodata\n.Lstnof%d:\n  .string \"%s\"\n  .popsection\n", lb, q.c_str());
}
void emit_stno_row(int32_t stno, int32_t line, const char * file) {
    if (g_is_text) {
        char fq[32]; if (file) { emit_stno_file_label(file); snprintf(fq, sizeof fq, ".Lstnof%d", g_emit.stno_file_lbl); } else snprintf(fq, sizeof fq, "0");
        const char * kn = flat_label_kind(g_emit.node->op); int uid = g_emit.x86_uid;
        emit_textf(".L%s_α_%d_stno:\n  .pushsection scrip_stno_map,\"aw\",@progbits; .p2align 3\n", kn, uid);
        emit_textf("  .quad .L%s_α_%d_stno\n  .long %d\n  .long %d\n  .quad %s\n  .popsection\n", kn, uid, (int)stno, (int)line, fq);
    } else {
        stno_rec_push((uint64_t)(uintptr_t)(bb_emit_buf + bb_emit_pos), stno, line, file ? icn_trace_intern(file) : (const char *)0);
    }
}
void emit_stno_mark(int32_t stno) {
    int32_t line = 0; const char * file = (const char *)0;
    if (stno > 0 && stno < g_emit.stno_src_n) { line = STNO_SRC(stno).line; file = STNO_SRC(stno).file; }
    emit_stno_row(stno, line, file);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" const sno_stno_rec_t * scrip_emit_stno_table(uint32_t * out_count) {
    *out_count = (uint32_t)g_emit.stno_map_n;
    return (const sno_stno_rec_t *)g_emit.stno_map.p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void g_emit_stno_drop_above(uint64_t addr) {
    int lo = 0, hi = g_emit.stno_map_n;
    while (lo < hi) { int mid = (lo + hi) / 2; if (STNO_REC(mid).pc < addr) lo = mid + 1; else hi = mid; }
    g_emit.stno_map_n = lo;
}
static void bb_slotix_insert(IR_t *nd, int i) { uint64_t h = (((uint64_t)(uintptr_t)nd >> 4) * 11400714819323198485ull) & (g_emitter.g_bb_slotix_cap - 1);
    while (g_emitter.g_bb_slotix[h].gen == g_emitter.g_bb_slotix_gen) { if (g_emitter.g_bb_slotix[h].key == nd) return; h = (h + 1) & (g_emitter.g_bb_slotix_cap - 1); }
    g_emitter.g_bb_slotix[h].key = nd; g_emitter.g_bb_slotix[h].idx = i; g_emitter.g_bb_slotix[h].gen = g_emitter.g_bb_slotix_gen; }
static void bb_slotix_add(IR_t *nd, int i) {
    if ((uint64_t)g_emitter.g_bb_slotmap_n * 2 > g_emitter.g_bb_slotix_cap) { uint32_t nc = g_emitter.g_bb_slotix_cap ? g_emitter.g_bb_slotix_cap * 2 : 1024;
        while ((uint64_t)nc < (uint64_t)g_emitter.g_bb_slotmap_n * 2) nc *= 2;
        ct_drop(g_emitter.g_bb_slotix); g_emitter.g_bb_slotix = (bb_slotix_t *)ct_zalloc(nc, sizeof(bb_slotix_t)); g_emitter.g_bb_slotix_cap = nc; g_emitter.g_bb_slotix_gen = 1;
        for (int k = 0; k < g_emitter.g_bb_slotmap_n; k++) bb_slotix_insert(g_emitter.g_bb_slotmap[k].key, k); return; }
    bb_slotix_insert(nd, i); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_slotmap_push(IR_t *nd, int off) {
    if (g_emitter.g_bb_slotmap_n >= g_emitter.g_bb_slotmap_max) {
        int new_max = g_emitter.g_bb_slotmap_max ? g_emitter.g_bb_slotmap_max * 2 : 512;
        bb_slotmap_ent_t *g = (bb_slotmap_ent_t *)ct_grow(g_emitter.g_bb_slotmap, (size_t)new_max * sizeof(bb_slotmap_ent_t));
        if (!g) return;
        g_emitter.g_bb_slotmap = g; g_emitter.g_bb_slotmap_max = new_max;
    }
    g_emitter.g_bb_slotmap[g_emitter.g_bb_slotmap_n].key = nd; g_emitter.g_bb_slotmap[g_emitter.g_bb_slotmap_n].off = off; g_emitter.g_bb_slotmap_n++;
    bb_slotix_add(nd, g_emitter.g_bb_slotmap_n - 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_slot_get(IR_t *nd) {
    if (!g_emitter.g_bb_slotix || !g_emitter.g_bb_slotmap_n) { for (int i = 0; i < g_emitter.g_bb_slotmap_n; i++) if (g_emitter.g_bb_slotmap[i].key == nd) return g_emitter.g_bb_slotmap[i].off;
        return -1; }
    uint64_t h = (((uint64_t)(uintptr_t)nd >> 4) * 11400714819323198485ull) & (g_emitter.g_bb_slotix_cap - 1);
    while (g_emitter.g_bb_slotix[h].gen == g_emitter.g_bb_slotix_gen) { if (g_emitter.g_bb_slotix[h].key == nd) return g_emitter.g_bb_slotmap[g_emitter.g_bb_slotix[h].idx].off;
        h = (h + 1) & (g_emitter.g_bb_slotix_cap - 1); }
    return -1;
}
int g_emit_frame_caller_dl = -1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_binop_opnd_slot(IR_t *o, int own, int vs) {
    if (!o) return -1;
    extern int is_global(const char *);
    if (o->op == IR_VAR && IR_LIT(o).sval && IR_LIT(o).sval[0] != '&' && (!is_global(IR_LIT(o).sval) || graph_has_local(g_emit_cfg, IR_LIT(o).sval))) { int voff = vs;
        if (voff >= 0) return voff; }
    int s = bb_slot_get(o); if (s >= 0) return s;
    return own;
}
void call_box_emit(std::string (*box)(void)) {
    int sw = g_emit.op_wpop, sz = g_emit.op_zgpop;
    if (g_emit.op_call_sb_exit && !(g_emit_cfg && g_emit_cfg->spine_cells)) { g_emit.op_wpop = 0; g_emit.op_zgpop = 0; }
    bb_emit_x86(box());
    g_emit.op_wpop = sw; g_emit.op_zgpop = sz;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int walk_bb_node(IR_t * nd, FILE * out) {
    { if (g_emitter.walk_bb_node__zp < 0) { const char * e = getenv("SCRIP_ZPROBE"); g_emitter.walk_bb_node__zp = (e && *e == '1') ? 1 : 0; }
      if (g_emitter.walk_bb_node__zp && nd) { long _k = zrow(nd)->zpk;
          fprintf(stderr, "[ZPROBE] nest=%d nid=%d op=%d k=%ld\n", g_emitter.g_zprobe_depth, bb_node_id(nd), (int)nd->op, _k);
          g_emitter.g_zprobe_depth++; int _rc = walk_bb_node_inner(nd, out); g_emitter.g_zprobe_depth--; return _rc; } }
    if (!(nd && MEDIUM_BINARY && bbprof_on())) return walk_bb_node_inner(nd, out);
    int lo = bb_emit_pos;
    int rc = walk_bb_node_inner(nd, out);
    if (bb_emit_pos > lo) bbprof_record(bb_node_id(nd), (int)nd->op, g_emit.x86_uid, (void *)(bb_emit_buf + lo), (void *)(bb_emit_buf + bb_emit_pos));
    return rc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_sep_on(void) { if (g_emitter.emit_sep_on__sep < 0) { const char *e = getenv("SCRIP_ASM_SEP"); g_emitter.emit_sep_on__sep = (e && *e == '0') ? 0 : 1;
    } return g_emitter.emit_sep_on__sep; }
void emit_sep_rule(char ch) { if (emit_sep_on()) bb_emit_x86(x86("commentrule", std::string(119, ch))); }
extern "C" void emit_sep_rule_c(char ch) { emit_sep_rule(ch); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void flat_drive_repalt(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t **ra_y, bb_label_t **ra_t, bb_label_t *node_γ, bb_label_t *node_ω) {
    IR_t *e_root = nodes[i]->n_operands > 0 ? nodes[i]->operands[0] : NULL;
    IR_t *e_entry = (nodes[i]->n_operands > 1 && nodes[i]->operands[1]) ? nodes[i]->operands[1] : e_root;
    int off = drive_value_slot(nodes[i]);
    int e_sa = e_root ? bb_slot_get(e_root) : -1;
    if (e_sa < 0 && e_root) e_sa = drive_value_slot(e_root);
    bb_label_t *e_lbl = node_ω; bb_label_t *e_beta = ra_t[i];
    if (e_entry) { int k = nidx(nodes, n, e_entry); if (k >= 0) { e_lbl = lbls[k]; } }
    if (e_root && ir_is_generator_kind(e_root->op)) { int k = nidx(nodes, n, e_root); if (k >= 0) { e_beta = betas[k]; } }
    if (nodes[i]->n_operands > 3) { IR_t *eb = nodes[i]->operands[3]; int k = eb ? nidx(nodes, n, eb) : -1; e_beta = (k >= 0) ? betas[k] : ra_t[i]; }
    g_emit.op_off = off; g_emit.op_sa = e_sa;
    DRIVE_FILL(nodes[i], lbls[i], node_γ, node_ω, betas[i]);
    bb_emit_x86(bb_repalt_clear());
    emit_jmp_label(e_lbl, JMP_JMP);
    emit_label_define_bb(ra_y[i]);
    g_emit.op_off = off; g_emit.op_sa = e_sa;
    bb_emit_x86(bb_repalt_yield());
    emit_jmp_label(node_γ, JMP_JMP);
    emit_label_define_bb(ra_t[i]);
    g_emit.op_off = off;
    bb_emit_x86(bb_repalt_test());
    emit_jmp_label(lbls[i], JMP_JE);
    emit_jmp_label(node_ω, JMP_JMP);
    emit_label_define_bb(betas[i]);
    emit_jmp_label(e_beta, JMP_JMP);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string pl_clause_zero(int ak) {
    const IR_graph_t * g = g_emit_cfg;
    if (!g || !g->alt_zhi || ak < 1 || ak >= g->n_alts || g->alt_zhi[ak] <= g->alt_zlo) return std::string();
    int b = g->alt_zhi[ak] - g->alt_zlo;
    std::string r = x86("note", "the clause overlay: every clause's construct slots share one region, so a clause entered after another zeroes its own extent, as the prologue zeroes the first");
    if (b <= 128) { for (int q = 0; q < b / 8; q++) r += x86("mov", RDQ(emit_pl_fb(), g->alt_zlo + 8 * q), 0L); return r; }
    return r + x86("lea", "rdi", RDQ(emit_pl_fb(), g->alt_zlo)) + x86("xor", "eax", "eax") + x86("mov32", "ecx", (long)(b / 8)) + x86("rep_stosq");
}
std::string pl_step_reseed_locals(void) {
    std::string r;
    if (!g_emit_cfg || !g_emit_cfg->lnames) return r;
    int kt = g_emit.flat_frame_bytes;
    for (int i = 0; i < g_emit_cfg->nlocals; i++) {
        const char * nm = g_emit_cfg->lnames[i];
        if (!nm) continue;
        int off = ir_varslot_of(g_emit_cfg, nm);
        if (off < 0 || (kt > 0 && off + 16 > kt) || ((size_t)i < g_planner.g_zd_rec_p->v_lspine.size() && g_planner.g_zd_rec_p->v_lspine[(size_t)i])) continue;
        r += x86("mov", RDQ(emit_pl_fb(), off), 0L) + x86("mov", RDQ(emit_pl_fb(), off + 8), 0L);
    }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void drive_guard_refused(IR_t *nd, int line) {
    fprintf(stderr,
        "FATAL emit_drive: IR op=%d HAS a template and its own case REFUSED AT A GUARD -- emit.cpp:%d. This is NOT a missing template: do NOT implement op=%d. Read the guard on that line and find wh"
        "y its precondition did not hold for this node -- a slot that came back < 0, a child that was null, an operand count short of what the case needs.\n",
            nd ? (int)nd->op : -1, line, nd ? (int)nd->op : -1);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int resume_sealed_defer_on_root(IR_t ** nodes, int n, IR_t * br) {
    for (int i = 0; i < n; i++) { IR_t * x = nodes[i];
        if (!x || (int) x->op != IR_MATCH_DEFER || x->seal != 1) continue;
        if (x == br) return 1;
        if (br) for (int q = 0; q < br->n_operands; q++) if (br->operands[q] == x) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int resume_carrier_ok(const IR_t * nd, int sealed_defer) {
    int st = zrow(nd)->seam;
    if (st == 1) return 1;
    if (st == 3) { int _nc = 0, _lf = 0, _fn = 0; sn4_blob_choice_scan(&_nc, &_lf, &_fn); return (_nc == 1 && !sealed_defer && !nd->seal) ? 1 : 0; }
    return st == 2 ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int blob_carve_bytes(void) { int b = blob_frame_bytes(); return b > 0 ? b + 16 : 0; }
void blob_layout_build(void) {
    const zeta_plan_t * r = g_planner.g_zd_rec_p;
    g_emitter.g_blob_lay.len = 0;
    for (size_t i = 0; r && i < r->v_blob_lay.size(); i++) CV_PUSH(g_emitter.g_blob_lay, uint64_t) = r->v_blob_lay[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_diag_regs_suppress(void) {
    return (g_emit.flat_jmp_entry && g_emit.flat_pat && (!blob_frame_scope() || !g_emit_cfg)) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_register_local_offsets(const char * pname) {
    extern void rt_proc_set_locals(const char *, const char **, int);
    extern void rt_proc_set_pname(const char *, int, const char *);
    extern void rt_proc_set_local_offs(const char *, const int *, int);
    if (!pname || !g_emit_cfg || !g_emit_cfg->lnames || g_emit_cfg->nlocals <= 0) return;
    rt_proc_set_locals(pname, g_emit_cfg->lnames, g_emit_cfg->nlocals);
    for (int k = 0; k < g_emit_cfg->nparams; k++) if (g_emit_cfg->pnames && g_emit_cfg->pnames[k]) rt_proc_set_pname(pname, k, g_emit_cfg->pnames[k]);
    { extern void rt_proc_set_loc_params(const char *, const char **, int);
      if (g_emit_cfg->pnames && g_emit_cfg->nparams > 0) {
          const char ** pc = (const char **) ct_alloc(sizeof(const char *) * (size_t) g_emit_cfg->nparams);
          if (pc) { for (int k = 0; k < g_emit_cfg->nparams; k++) pc[k] = g_emit_cfg->pnames[k] ? ct_strdup(g_emit_cfg->pnames[k]) : (const char *) 0;
              rt_proc_set_loc_params(pname, pc, g_emit_cfg->nparams); } } }
    if (g_emit_cfg->n_vslots <= 0) return;
    int nl = g_emit_cfg->nlocals;
    int * offs = (int *) ct_alloc(sizeof(int) * (size_t) nl); if (!offs) return;
    for (int k = 0; k < nl; k++) {
        offs[k] = -1;
        const char * ln = g_emit_cfg->lnames[k]; if (!ln) continue;
        offs[k] = ir_varslot_of(g_emit_cfg, ln);
    }
    rt_proc_set_local_offs(pname, offs, nl);
}
const char * gc_map_entry_fam(const char * fam) { return (fam && !strcmp(fam, "pat_flat")) ? "main" : fam; }
static int gc_maps_knob(const char * k) { const char * e = getenv(k); return (e && *e == '1') ? 1 : 0; }
static int gc_maps_report_on(void) { if (g_emitter.gc_maps_report_on_v < 0) g_emitter.gc_maps_report_on_v = gc_maps_knob("SCRIP_GC_MAPS_REPORT"); return g_emitter.gc_maps_report_on_v; }
static int emit_zsm_frame_on(void) {
    if (g_emitter.zsm_frame_on < 0) { const char * e = getenv("SCRIP_ZSM"); g_emitter.zsm_frame_on = (e && *e == '1') ? 1 : 0; }
    return g_emitter.zsm_frame_on;
}
static void emit_zsm_frame_report(void) {
    for (const auto & r : g_emitter.zsm_frames)
        fprintf(stderr, "[ZSM-FRAME] graph=%s total=%d region=%d resident=%d frames=%d addrs=%d word=%d resident_addrs=%d outside=%d unbacked=%d rsp_form=%d\n", r.name.c_str(), r.total,
            r.region, r.resident, r.frames, r.addrs, r.cls[1], r.cls[2], r.cls[3], r.cls[0], r.rsp_form);
}
static g_emitter_t::zsm_frame_rec_t * emit_zsm_frame_rec(const IR_graph_t * g) {
    if (!g) return (g_emitter_t::zsm_frame_rec_t *)0;
    int L = g_emitter.zsm_frames_last;
    if (L >= 0 && L < (int)g_emitter.zsm_frames.size() && g_emitter.zsm_frames[(size_t)L].g == g) return &g_emitter.zsm_frames[(size_t)L];
    for (size_t k = 0; k < g_emitter.zsm_frames.size(); k++) if (g_emitter.zsm_frames[k].g == g) { g_emitter.zsm_frames_last = (int)k; return &g_emitter.zsm_frames[k]; }
    if (g_emitter.zsm_frames.empty()) atexit(emit_zsm_frame_report);
    g_emitter_t::zsm_frame_rec_t r = { g, std::string(g->pf.name ? g->pf.name : "?"), g->pf.frame_region, g->pf.resident, 0, 0, { 0, 0, 0, 0 }, 0, 0 };
    g_emitter.zsm_frames.push_back(r);
    g_emitter.zsm_frames_last = (int)g_emitter.zsm_frames.size() - 1;
    return &g_emitter.zsm_frames.back();
}
extern "C" void emit_zsm_frame_addr(const IR_graph_t * g, const IR_t * nd, int off, int cls, int rsp_form) {
    if (!emit_zsm_frame_on()) return;
    g_emitter_t::zsm_frame_rec_t * r = emit_zsm_frame_rec(g);
    if (!r) return;
    int c = cls >= 0 ? cls : (off >= 0 && off / 8 < g->pf.kcls_n) ? g->pf.kcls[off / 8] : 0;
    if (c == 0 && off >= r->region + 16 && off < r->total) c = 1;
    r->addrs++;
    r->cls[c & 3]++;
    r->rsp_form += rsp_form ? 1 : 0;
}
static void emit_zsm_frame_carve(const IR_graph_t * g, int frame_bytes) {
    if (!emit_zsm_frame_on()) return;
    g_emitter_t::zsm_frame_rec_t * r = emit_zsm_frame_rec(g);
    if (!r) return;
    r->frames++;
    if (frame_bytes > r->total) r->total = frame_bytes;
}
}
extern "C++" std::string emit_gc_map_register(int value_bytes, int frame_bytes, int header_bytes, unsigned flags, int frame_rel, uint64_t link) {
    if (frame_rel == 2 ? (value_bytes < 16 || value_bytes != frame_bytes || (flags & GC_FRAME_MAP_BLOB) == 0) : (value_bytes < 0 || (value_bytes & 15) || value_bytes + 16 > frame_bytes))
        { fprintf(stderr,
        "FATAL emit_gc_map_register: value_bytes=%d frame_bytes=%d frame_rel=%d (the value region is 16-aligned and the 16 reserved bytes after it lie inside the frame; a blob's region is the wh"
            "ole frame below its base "
        "and carries GC_FRAME_MAP_BLOB)\n", value_bytes, frame_bytes, frame_rel); abort(); }
    emit_zsm_frame_carve(g_emit_cfg, frame_bytes);
    g_emitter.g_gc_map_pending = 1; g_emitter.g_gc_value_bytes = value_bytes; g_emitter.g_gc_map_link = link; g_emitter.g_gc_map_fb = frame_bytes; g_emitter.g_gc_map_hdr = header_bytes;
        g_emitter.g_gc_map_flags = flags;
    emit_label_initf(&g_emitter.g_gc_map_lbl, ".Lgcmap_%s", g_emit.flat_fam ? g_emit.flat_fam : "chain");
    return x86("note", std::string("GCANCHOR ") + g_emitter.g_gc_map_lbl.name + " " + (frame_rel == 2 ? std::string(RDQ("rbp", -value_bytes + 8))
                                                                          : frame_rel == 1 &&
                                                                              (g_emit_cfg && g_emit_cfg->pf.keys_only) ? std::string(x86_fr64_prefix()) + std::to_string(value_bytes + 8) + "]"
                                                                          : frame_rel ? std::string(FRQ(value_bytes + 8)) : std::string(RDQ("rsp", value_bytes + 8))))
         + IF(frame_rel != 2 || blob_cont_copy(), x86("mov", "eax", 0L));
}
extern "C" {
void emit_gc_map_data(const char * fam) {
    { const char * ef = gc_map_entry_fam(fam);
      if (ef && !strcmp(ef, "main")) fam = ef;
      else { const char * gn = g_emit_cfg ? g_emit_cfg->pf.name : (const char *)0; fam = (gn && *gn) ? gn : fam; } }
    if (!g_emitter.g_gc_map_pending) { g_emitter.g_gc_map_last_off = -1; emit_gc_sites_data(fam, 1); return; }
    g_emitter.g_gc_map_pending = 0;
    const uint64_t * lay = (const uint64_t *)0; int lay_n = 0; g_emitter.g_zls_lay_gaps = 0; g_emitter.g_zls_lay_conf = 0;
    if (g_emitter.g_gc_map_flags & GC_FRAME_MAP_BLOB) { lay = (const uint64_t *)g_emitter.g_blob_lay.p; lay_n = (int)g_emitter.g_blob_lay.len; }
    else if (g_emit_cfg && g_emit_cfg->pf.lay_n >= 0) {
        if (g_emit_cfg->root_graph && g_emitter.g_gc_value_bytes != zls_g_root_value_bytes(g_emit_cfg)) {
            fprintf(stderr, "FATAL emit_gc_map_data: root graph %s value_bytes=%d but the plan says %d\n", fam ? fam : "?", g_emitter.g_gc_value_bytes, zls_g_root_value_bytes(g_emit_cfg));
            abort();
        }
        lay = g_emit_cfg->pf.lay; lay_n = g_emit_cfg->pf.lay_n; g_emitter.g_gc_map_flags |= GC_FRAME_MAP_LAYOUT | g_emit_cfg->pf.lay_flags;
        g_emitter.g_zls_lay_gaps = g_emit_cfg->pf.lay_gaps; g_emitter.g_zls_lay_conf = g_emit_cfg->pf.lay_conf;
    }
    emit_sep_rule('-');
    if (g_is_text) emit_textf(".pushsection .data.rel.ro,\"aw\",@progbits\n .p2align 3\n");
    emit_label_define_bb(&g_emitter.g_gc_map_lbl);
    uint64_t q0 = (uint64_t)GC_FRAME_MAP_MAGIC | ((uint64_t)(uint32_t)g_emitter.g_gc_map_fb << 32);
    uint64_t q1 = (uint64_t)(uint32_t)g_emitter.g_gc_map_hdr | ((uint64_t)g_emitter.g_gc_map_flags << 32);
    const int anon = g_emit.flat_pat ? 1 : 0;
    if (g_is_text) {
        char eb[2 * strlen(fam ? fam : "?") + 1]; { int ei = 0; for (const char * q = fam ? fam : "?"; *q; q++) { if (*q == '\\' || *q == '"') eb[ei++] = '\\'; eb[ei++] = *q; } eb[ei] = 0; }
        char b[fmt_len(" .quad %llu\n .quad %llu\n .quad %s_s\n .quad %d\n%s_s: .string \"%s\"\n", (unsigned long long)q0, (unsigned long long)q1, g_emitter.g_gc_map_lbl.name,
            g_emitter.g_gc_value_bytes,
            g_emitter.g_gc_map_lbl.name,
            eb)]; int n = anon ? snprintf(b, sizeof b, " .quad %llu\n .quad %llu\n .quad 0\n .quad %d\n", (unsigned long long)q0, (unsigned long long)q1, g_emitter.g_gc_value_bytes)
                                   :
                                       snprintf(b, sizeof b, " .quad %llu\n .quad %llu\n .quad %s_s\n .quad %d\n", (unsigned long long)q0, (unsigned long long)q1, g_emitter.g_gc_map_lbl.name,
                                       g_emitter.g_gc_value_bytes);
        emit_text_n(b, (size_t)n);
        if (lay) { n = snprintf(b, sizeof b, " .quad %d\n", lay_n); emit_text_n(b, (size_t)n); for (int i = 0; i < lay_n; i++) { n = snprintf(b, sizeof b, " .quad %llu\n", (unsigned long long)lay[i]);
            emit_text_n(b, (size_t)n); } }
        if (!anon) { n = snprintf(b, sizeof b, "%s_s: .string \"%s\"\n", g_emitter.g_gc_map_lbl.name, eb); emit_text_n(b, (size_t)n); }
        emit_textf(".popsection\n");
        g_emitter.g_gc_map_last_off = 0;
    } else {
        bb_emit_u64(q0); bb_emit_u64(q1); bb_emit_u64(anon ? (uint64_t)0 : (uint64_t)(uintptr_t)ct_strdup(fam ? fam : "?")); bb_emit_u64((uint64_t)(uint32_t)g_emitter.g_gc_value_bytes);
        if (lay) { bb_emit_u64((uint64_t)lay_n); for (int i = 0; i < lay_n; i++) bb_emit_u64(lay[i]); }
        g_emitter.g_gc_map_last_off = g_emitter.g_gc_map_lbl.offset;
    }
    if (gc_maps_report_on() && lay) { fprintf(stderr, "[GC-MAP-LAYOUT] graph=%s n=%d", fam ? fam : "?", lay_n);
        for (int i = 0; i < lay_n; i++) fprintf(stderr, " %d:%u:%d", GC_LAY_OFF(lay[i]), GC_LAY_KIND(lay[i]), GC_LAY_SIZE(lay[i]));
        fprintf(stderr, " gaps=%d conflicts=%d\n", g_emitter.g_zls_lay_gaps, g_emitter.g_zls_lay_conf); }
    if (g_emitter.g_gc_map_flags & GC_FRAME_MAP_BLOB) g_emitter.g_blob_lay.len = 0;
    CV_PUSH(g_emitter.g_gc_map_names_v, char *) = ct_strdup(g_emitter.g_gc_map_lbl.name); g_emitter.g_gc_map_names_n++;
    if (gc_maps_report_on())
        fprintf(stderr, "[GC-MAP] graph=%s frame_bytes=%d header_bytes=%d value_bytes=%d flags=%u map=%s\n", fam ? fam : "?", g_emitter.g_gc_map_fb, g_emitter.g_gc_map_hdr, g_emitter.g_gc_value_bytes,
            g_emitter.g_gc_map_flags, g_emitter.g_gc_map_lbl.name);
    emit_gc_sites_data(fam, 0);
}
extern "C" int emit_gc_map_last_off(void) { return g_emitter.g_gc_map_last_off; }
extern "C" void emit_gc_tables_register(const void *fn) { if (!fn) return;
    if (g_emitter.g_gc_map_last_off >= 0) rt_gc_frame_maps_add((const gc_frame_map_t *)((const char *)fn + g_emitter.g_gc_map_last_off));
    for (uint32_t k = 0; k < g_emitter.g_gc_sites_offs_v.len; k++) rt_gc_frame_sites_add((const char *)fn + CV_AT(g_emitter.g_gc_sites_offs_v, int, k)); }
extern "C" int emit_gc_map_names_n(void) { return g_emitter.g_gc_map_names_n; }
static int * emit_grow_one(int * p, int o, int cap, int fill) { int * q = (int *)ct_alloc((size_t)cap * sizeof(int)); if (o) memcpy(q, p, (size_t)o * sizeof(int));
    for (int k = o; k < cap; k++) q[k] = fill; return q; }
void emit_ops_reserve(int n) {
    if (n < ZD_NOPS_MIN) n = ZD_NOPS_MIN;
    if (n <= g_emit.op_zcap) return;
    int o = g_emit.op_zcap, cap = o > 0 ? o : ZD_NOPS_MIN; while (cap < n) cap *= 2;
    g_emit.op_zread = emit_grow_one(g_emit.op_zread, o, cap, 0); g_emit.op_zkind = emit_grow_one(g_emit.op_zkind, o, cap, -1); g_emit.op_zread_xf = emit_grow_one(g_emit.op_zread_xf, o, cap, -1);
    g_emit.op_zread_fr = emit_grow_one(g_emit.op_zread_fr, o, cap, -1); g_emit.op_zslot = emit_grow_one(g_emit.op_zslot, o, cap, -1); g_emit.op_zkown = emit_grow_one(g_emit.op_zkown, o, cap, -1);
        g_emit.op_zfres = emit_grow_one(g_emit.op_zfres, o, cap, 1); g_emit.op_opvs = emit_grow_one(g_emit.op_opvs, o, cap, -1);
    g_emit.op_pb_slot = emit_grow_one(g_emit.op_pb_slot, o, cap, -1); g_emit.op_pb_vs = emit_grow_one(g_emit.op_pb_vs, o, cap, -1); g_emit.op_pb_kv = emit_grow_one(g_emit.op_pb_kv, o, cap, -1);
    g_emit.op_pb_kh = emit_grow_one(g_emit.op_pb_kh, o, cap, -1); g_emit.op_pb_kvn = emit_grow_one(g_emit.op_pb_kvn, o, cap, 0); g_emit.op_pb_khn = emit_grow_one(g_emit.op_pb_khn, o, cap, 0);
    g_emit.op_zcap = cap;
}
static uint32_t rtxe_hash(const char * s) { uint32_t h = 2166136261u; for (const unsigned char * p = (const unsigned char *)s; *p; p++) { h ^= *p; h *= 16777619u; } return h; }
extern "C" unsigned emit_rtx_clob_mask(const char * sym) {
    if (!sym) return 0u;
    for (int i = 0; i < RT_ENTRIES; i++) if (g_rt_entries[i].sym[0] == sym[0] && strcmp(g_rt_entries[i].sym, sym) == 0) return g_rt_entries[i].clobbers & ~(unsigned)RT_CLOB_STATED;
    return 0u;
}
static const struct { char sym[36]; char cname[36]; void * caddr; int kind; } rtx_twin_tab[] = {
#define RTX_TWIN(kind, ret, leaf, twin, args) { #leaf, #twin, (void *)twin, RTX_VERDICT_##kind },
#include "templates/x86/rtx_twin_table.inc"
#undef RTX_TWIN
};
extern "C" void * emit_rtx_twin(const char * sym, const char ** cname, int * kind) {
    if (!sym) return 0;
    for (size_t i = 0; i < sizeof rtx_twin_tab / sizeof rtx_twin_tab[0]; i++) {
        if (rtx_twin_tab[i].sym[0] != sym[0] || strcmp(rtx_twin_tab[i].sym, sym) != 0) continue;
        *cname = rtx_twin_tab[i].cname; *kind = rtx_twin_tab[i].kind; return rtx_twin_tab[i].caddr;
    }
    return 0;
}
extern "C" const char * emit_rtx_twin_sym(const char * sym) { const char * cname = 0; int kind = 0; return emit_rtx_twin(sym, &cname, &kind) ? cname : sym; }
extern "C" void * emit_rtx_twin_addr(const char * sym, void * addr) { const char * cname = 0; int kind = 0; void * caddr = emit_rtx_twin(sym, &cname, &kind); return caddr ? caddr : addr; }
int emit_rtx_entry_is(const char * sym) {
    if (!sym) return 0;
    if (!g_emitter.g_rtxe_cap) { uint32_t n = 0, c = 64; for (const char * p = __start_rtx_entry_names; p < __stop_rtx_entry_names; p += strlen(p) + 1) n++;
        while (c < n * 2 + 2) c *= 2; const char ** t = (const char **)ct_zalloc(c, sizeof(const char *));
        for (const char * p = __start_rtx_entry_names; p < __stop_rtx_entry_names; p += strlen(p) + 1) { uint32_t h = rtxe_hash(p) & (c - 1);
            while (t[h] && strcmp(t[h], p) != 0) h = (h + 1) & (c - 1); t[h] = p; }
        g_emitter.g_rtxe_tab = t; g_emitter.g_rtxe_cap = c; }
    for (uint32_t h = rtxe_hash(sym) & (g_emitter.g_rtxe_cap - 1); g_emitter.g_rtxe_tab[h]; h = (h + 1) & (g_emitter.g_rtxe_cap - 1)) if (strcmp(g_emitter.g_rtxe_tab[h], sym) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_jmp_entry_arm_region(IR_graph_t *g) {
    int rg = g ? g->pf.region : -1;
    int so = g ? g->pf.resume : -1;
    if (so < 0 && g) so = g->pf.zeta_mark;
    if (so < 0 && g) { so = g->pf.locals; if (so < 0) so = rg; }
    int capen; { if (g_emitter.emit_jmp_entry_arm_region_v < 0) { const char *e = getenv("SCRIP_CAP_NOFILL"); g_emitter.emit_jmp_entry_arm_region_v = e ? (atoi(e) != 0) : 1;
        } capen = g_emitter.emit_jmp_entry_arm_region_v; }
    if (!capen && g && g->pf.cap_min >= 0 && (so < 0 || g->pf.cap_min < so)) so = g->pf.cap_min;
    g_emit.flat_layout_unknown = 0;
    if (rg <= 0) { rg = 4096; so = -1; g_emit.flat_layout_unknown = 1; }
    { extern int g_flat_frame_floor; if (g_flat_frame_floor > 0 && rg < g_flat_frame_floor) { rg = g_flat_frame_floor; so = -1; g_emit.flat_layout_unknown = 1; } }
    g_emit.flat_jmp_entry = 1; g_emit.flat_frame_bytes = g_emit_cfg ? g_emit_cfg->pf.frame_bytes : ((FLAT_FRAME_ALLOWANCE + 15) & ~15);
    g_emit.flat_seed_off = (so >= 16 && so <= rg) ? so : 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_jmp_entry_for_road(const planner_road_t *rd, IR_graph_t *g, const char *pname) {
    if (rd->pat) { g_emit.flat_pat = rd->pat; return emit_jmp_entry_arm_region(g); }
    if (!rd->jmp_entry) return 0;
    g_emit.flat_lex = rd->lex;
    g_emit.flat_gen = rd->gen;
    int _r = emit_jmp_entry_arm_region(g);
    if (getenv("SCRIP_FLOOR_DIAG")) { extern int g_flat_frame_floor;
        fprintf(stderr, "[ARM-REGION] pname=%s r=%d floor=%d frame_bytes=%d seed_off=%d layout_unknown=%d jcon_value_region=%d\n", pname ? pname : "(null)", _r, g_flat_frame_floor,
        g_emit.flat_frame_bytes, g_emit.flat_seed_off, g_emit.flat_layout_unknown, g ? g->jcon_value_region : -1); }
    return _r;
}
int g_flat_frame_floor = 0;
int g_flat_dc_np = -1;
extern "C" void emit_jmp_entry_clear(void) { extern int g_flat_frame_floor; g_flat_frame_floor = 0; g_emit.flat_jmp_entry = 0; g_emit.flat_frame_bytes = 0; g_emit.flat_seed_off = 0;
    g_emit.flat_layout_unknown = 0; g_emit.flat_pat = 0; g_emit.flat_lex = 0; g_emit.flat_gen = 0; g_emit.flat_deep_arrival = 0; g_flat_dc_np = -1; g_emit.flat_lcl_proc = 0;
    g_emit.zframe_graph = 0; g_emit.frame_retained = 0; }
int emit_graph_has_suspend(IR_graph_t *g) { return ir_graph_has_suspend(g); }
int emit_graph_has_deep_arrival(IR_graph_t *g) { if (!g) return 1; for (int i = 0; i < g->n; i++) { IR_t *c = g->all[i]; if (!c) continue;
    switch (c->op) { case IR_SUSPEND: case IR_SCAN: case IR_SCAN_ENTER: case IR_SCAN_ALTERNATE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL: case IR_SCAN_MATCH:
    case IR_SCAN_MOVE: case IR_SCAN_TAB: case IR_TO: case IR_TO_BY: case IR_LIMIT: case IR_REPALT: case IR_PROC_GEN: case IR_CREATE: case IR_ITERATE: case IR_DISJUNCTION: case IR_CALL_BUILTIN_GEN:
    case IR_KW_ICON_GEN: case IR_MATCH_FENCE1: case IR_MATCH_ABORT: case IR_MATCH_ARBNO: case IR_MATCH_CALLOUT: case IR_MATCH_VALUE: return 1; case IR_MATCH_DEFER: return 1; default: break;
    } } return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_fb_bit_of(IR_t * nd) { int i = g_emitter.g_fbm_n > 0 ? ir_index_of(&g_emitter.g_fbm_nd, nd) : -1;
    return (i >= 0 && i < g_emitter.g_fbm_n) ? CV_AT(g_emitter.g_fbm_bit, unsigned char, i) : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_fb_stmt_scan(IR_graph_t * g) {
     if (g_emitter.emit_fb_stmt_scan_fbdbg < 0) { const char * e = getenv("SCRIP_FB_DEBUG"); g_emitter.emit_fb_stmt_scan_fbdbg = (e && *e == '1') ? 1 : 0; }
    g_emitter.g_fbm_n = 0; if (!g || g->n <= 0) { if (g_emitter.emit_fb_stmt_scan_fbdbg) fprintf(stderr, "[FB-STMT] bail size g=%p n=%d\n", (void *)g, g ? g->n : -1); return 0; }
    for (int i = 0; i < g->n; i++) { IR_t * c = g->all[i]; if (!c) continue; switch (c->op) {
        case IR_SUSPEND: case IR_SCAN: case IR_SCAN_ENTER: case IR_SCAN_ALTERNATE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL: case IR_SCAN_MATCH: case IR_SCAN_MOVE:
            case IR_SCAN_TAB:
        case IR_TO: case IR_TO_BY: case IR_LIMIT: case IR_REPALT: case IR_PROC_GEN: case IR_CREATE: case IR_ITERATE: case IR_DISJUNCTION: case IR_CALL_BUILTIN_GEN: case IR_KW_ICON_GEN:
        case IR_MATCH_ABORT: case IR_MATCH_CALLOUT: { if (g_emitter.emit_fb_stmt_scan_fbdbg) { extern const char * bb_op_name(IR_e);
            fprintf(stderr, "[FB-STMT] bail kind %s @%d n=%d\n", bb_op_name(c->op), i, g->n); } return 0; }
        default: break; } }
    ir_index_free(&g_emitter.g_fbm_nd); ir_index_build(&g_emitter.g_fbm_nd, g); cv_reserve(&g_emitter.g_fbm_bit, 1u, (uint64_t)g->n, "g_fbm_bit");
        for (int i = 0; i < g->n; i++) { IR_t * c = g->all[i];
        CV_AT(g_emitter.g_fbm_bit, unsigned char, g_emitter.g_fbm_n) = (c && ir_is_matcher(c->op)) ? 1 : 0; g_emitter.g_fbm_n++; }
    for (int h = 0; h < g->n; h++) { IR_t * hn = g->all[h]; if (!hn || hn->op != IR_MATCH_BEGIN) continue; int e = -1, deep = 0;
        for (int k = h + 1; k < g->n; k++) { IR_t * c = g->all[k]; if (!c) continue;
            if (c->op == IR_MATCH_DEFER || c->op == IR_MATCH_ARBNO || (c->op == IR_MATCH_FENCE1 || c->op == IR_MATCH_FENCE0) || c->op == IR_MATCH_VALUE) deep = 1;
            if (c->op == IR_MATCH_END || c->op == IR_MATCH_REPLACE) { e = k; break; } }
        if (e < 0) { if (g_emitter.emit_fb_stmt_scan_fbdbg) fprintf(stderr, "[FB-STMT] bail unterminated HEAD @%d n=%d\n", h, g->n); return 0; }
        if (deep) { for (int k = h; k <= e; k++) CV_AT(g_emitter.g_fbm_bit, unsigned char, k) = 1; } }
    if (g_emitter.emit_fb_stmt_scan_fbdbg) { int z = 0; for (int i = 0; i < g_emitter.g_fbm_n; i++) if (!CV_AT(g_emitter.g_fbm_bit, unsigned char, i)) z++;
        fprintf(stderr, "[FB-STMT] refine=1 n=%d rsp_nodes=%d\n", g_emitter.g_fbm_n, z); }
    return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_icn_n2_gen_region_ft(const char *pname, int is_generator, IR_graph_t *g) {
    if (!pname || !is_generator || !g || !g->cell_frame || g->zframe_graph || !emit_graph_has_suspend(g)) return 0;
    return g->pf.gen_ft > 0 ? g->pf.gen_ft : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_jmp_entry_for_chain(IR_graph_t *g) {
    extern int g_flat_dc_np; g_flat_dc_np = -1;
    return emit_jmp_entry_arm_region(g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_box_fn emit_chain(zeta_plan_t * rec, FILE *out, const char *prefix) {
    if (!rec || !rec->key) return NULL;
    emit_chain_prepare(rec->key);
    if (out) { emitter_init_text(out, TEXT_MODE_INVOCATION); int rc = codegen_flat_chain_body(rec, prefix); emitter_end();
        return rc == 0 ? (bb_box_fn)1 : NULL; }
    long flat_cap = FLAT_BUF_MAX; if (g_emit_cfg && (long)g_emit_cfg->n * 2048L > flat_cap) flat_cap = (long)g_emit_cfg->n * 2048L;
    int holes_ok = g_emit_cfg && g_emit_cfg->pkt_fragment; size_t hole_min = 0, need = 0;
    size_t mark = bb_pool_mark();
    for (int pass = 0;; pass++) {
        size_t hcap = 0; bb_buf_t hole = holes_ok ? bb_alloc_hole(hole_min, &hcap) : NULL; bb_buf_t buf = hole; long cap = hole ? (long)hcap : flat_cap;
        if (!hole && flat_cap > 2147483647L)
            { fprintf(stderr, "[IBB] emit_chain('%s') FAILED: the chain wants %ld bytes, more than the binary medium addresses (pass %d)\n", prefix ? prefix : "", flat_cap, pass); return NULL; }
        if (!hole) buf = bb_alloc(flat_cap);
        if (!hole && !buf && bb_pool_free() >= need && bb_pool_free() > 0 && (long)bb_pool_free() < flat_cap) { flat_cap = cap = (long)bb_pool_free(); buf = bb_alloc(flat_cap); }
        if (!buf)
            { fprintf(stderr, "[IBB] emit_chain('%s') FAILED: the code pool has no room for %ld bytes (pass %d; the pool is BB_POOL_SIZE, raised by SCRIP_CODE_POOL_MB)\n", prefix ? prefix : "",
            flat_cap, pass); return NULL; }
        g_flat_node_id = 0;
        if (pass) emit_chain_prepare(rec->key);
        emitter_init_binary(buf, cap);
        codegen_flat_chain_body(rec, prefix);
        if (g_emit_chain_posthook && !g_emitter.bb_emit_overflow) { void (*h)(void) = g_emit_chain_posthook; g_emit_chain_posthook = NULL; h(); }
        int nbytes = emitter_end();
        long want = g_emitter.bb_emit_want;
        if (!g_emitter.bb_emit_overflow && nbytes > 0 && nbytes <= cap) {
            bb_seal(buf, (size_t)nbytes);
        { const char *_pm = getenv("SCRIP_PERF_MAP");
          if (_pm && *_pm && *_pm != '0') {
              char _pmp[64]; snprintf(_pmp, sizeof _pmp, "/tmp/perf-%d.map", (int)getpid());
              FILE *_pmf = fopen(_pmp, "a");
              if (_pmf) { fprintf(_pmf, "%llx %llx m3_%s\n", (unsigned long long)(uintptr_t)buf, (unsigned long long)(size_t)nbytes,
                                  (prefix && *prefix) ? prefix : "chain"); fclose(_pmf); } } }
        { const char *_ds = getenv("SCRIP_DUMP_SLAB");
          if (_ds && *_ds) {
              char _hx[32]; snprintf(_hx, sizeof _hx, "%llx", (unsigned long long)(uintptr_t)buf);
              std::string _fn = std::string("m3_") + _hx + "_" + ((prefix && *prefix) ? prefix : "chain") + ".bin";
              FILE *_df = fopen((std::string(_ds) + "/" + _fn).c_str(), "wb");
              if (_df) { fwrite(buf, 1, (size_t)nbytes, _df); fclose(_df); }
              FILE *_mf = fopen((std::string(_ds) + "/MANIFEST").c_str(), "a");
              if (_mf) { fprintf(_mf, "%s %d %s\n", _fn.c_str(), nbytes, _hx); fclose(_mf); } } }
            if (hole) bb_hole_return(buf, (size_t)nbytes, hcap); else bb_pool_trim_last(buf, flat_cap, (size_t)nbytes);
            bb_pool_last_set(buf, (size_t)nbytes);
            return (bb_box_fn)buf;
        }
        if (hole) bb_hole_abort(buf, hcap); else bb_pool_release(mark);
        if (!g_emitter.bb_emit_overflow || want <= cap) {
            extern int bb_emit_pos; extern int bb_patch_count;
            fprintf(stderr, "[IBB] emit_chain('%s') FAILED: overflow=%d nbytes=%d pos=%d cap=%ld patches=%d — empty or invalid emission\n", prefix ? prefix : "", g_emitter.bb_emit_overflow, nbytes,
                bb_emit_pos, cap, bb_patch_count);
            return NULL;
        }
        need = hole_min = (size_t)want;
        flat_cap = want + (want >> 3) + 4096L;
    }
}
extern "C" void emit_flat_road(planner_road_t * rd) {
    extern int g_flat_frame_floor;
    rd->jmp_entry = g_emit.flat_jmp_entry; rd->pat = g_emit.flat_pat; rd->lex = g_emit.flat_lex; rd->gen = g_emit.flat_gen; rd->floor = g_flat_frame_floor;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_proc_prepare(stage2_t * s2, int pi, const planner_road_t * rd, int * isp_out) {
    ProcEntry * pe = &s2->proc_table[pi]; const char * pname = pe->name;
    if (pe->bb_idx < 0 || pe->bb_idx >= s2->bbp.count)
        { fprintf(stderr, "scrip: emit_proc_prepare: proc %s has no graph (bb_idx %d of %d): ir_delete_all invalidated it\n", pe->name ? pe->name : "?", pe->bb_idx, s2->bbp.count); exit(2); }
    IR_graph_t * g = s2->bbp.table[pe->bb_idx];
    g_emit_cfg = g;
    g_gen_proc_active = rd->gen_proc; g_flat_frame_floor = rd->floor;
    int isp = rd->pat; emit_jmp_entry_for_road(rd, g, pname);
    g_flat_dc_np = rd->dc_np;
    g_emit_frame_caller_dl = rd->caller_dl;
    if (isp_out) *isp_out = isp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_proc_unprepare(void) { emit_jmp_entry_clear(); g_emit_frame_caller_dl = -1; g_gen_proc_active = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_collect_icon_globals(void) {
    extern const char ** lc_global_names(void); extern int lc_global_count(void);
    extern int gva_name_hidden(const char *);
    const char ** global_names = lc_global_names();
    int global_count = lc_global_count();
    for (int i = 0; i < global_count; i++) if (global_names[i] && !gva_name_hidden(global_names[i])) (void)gva_collect_var(global_names[i]);
    for (int i = 0; i < global_count; i++) if (global_names[i] && gva_name_hidden(global_names[i])) (void)gva_collect_var(global_names[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_set_sink(FILE * out) { g_emitter.g_emit_sink = out; }
long emit_text_count(void) { return g_emitter.g_emit_text_count; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_text_raw_n(const char * s, size_t n) {
    if (!s || n == 0) return;
    g_emitter.g_emit_text_count += (long)n;
    if (g_emitter.g_emit_sink) fwrite(s, 1, n, g_emitter.g_emit_sink);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_text_n(const char * s, size_t n) {
    if (!s || n == 0) return;
    if (MEDIUM_BINARY) { emit_text_raw_n(s, n); return; }
    g_emitter.g_text_acc.append(s, n);
    size_t last = g_emitter.g_text_acc.rfind('\n');
    if (last == std::string::npos) return;
    std::string done = x86_4col(g_emitter.g_text_acc.substr(0, last + 1));
    g_emitter.g_text_acc.erase(0, last + 1);
    emit_text_raw_n(done.data(), done.size());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_text_flush(void) {
    if (g_emitter.g_text_acc.empty()) return;
    std::string done = x86_4col(g_emitter.g_text_acc); g_emitter.g_text_acc.clear();
    emit_text_raw_n(done.data(), done.size());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_textf(const char * fmt, ...) {
    va_list ap; va_start(ap, fmt); char sb[4096]; int need = vsnprintf(sb, sizeof sb, fmt, ap); va_end(ap);
    if (need <= 0) return;
    if (need < (int) sizeof sb) emit_text_n(sb, (size_t) need);
    else { char * hb = (char *) ct_alloc((size_t) need + 1); if (!hb) return; va_start(ap, fmt); vsnprintf(hb, (size_t) need + 1, fmt, ap); va_end(ap); emit_text_n(hb, (size_t) need); ct_drop(hb); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_textf_flush(void) { emit_text_flush(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_u2_fence(void) { if (g_emitter.sn4_u2_fence_v < 0) { const char * e = getenv("SCRIP_U2_FENCE"); g_emitter.sn4_u2_fence_v = (e && e[0] == '0') ? 0 : 1; } return g_emitter.sn4_u2_fence_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_nret_cap(void) { if (g_emitter.sn4_nret_cap_v < 0) { const char * e = getenv("SCRIP_NRET_CAP"); g_emitter.sn4_nret_cap_v = e ? (e[0] != '0') : 1; } return g_emitter.sn4_nret_cap_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_cap_gva(void) { if (g_emitter.sn4_cap_gva_v < 0) { const char * e = getenv("SCRIP_CAP_GVA"); g_emitter.sn4_cap_gva_v = (e && *e == '0') ? 0 : 1; } return g_emitter.sn4_cap_gva_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_opt_binimm_off(void) { if (g_emitter.sn4_opt_binimm_off_v < 0) { const char * e = getenv("SCRIP_OPT_BINIMM"); g_emitter.sn4_opt_binimm_off_v = (e && e[0] == '0') ? 1 : 0;
    } return g_emitter.sn4_opt_binimm_off_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void kw_snobol4_prepare(void) {
    int soff = 0; const void * cbase = (const void *)0;
    g_emit.op_imm_a = g_emit.op_sval ? rt_kw_index(g_emit.op_sval) : -1;
    g_emit.op_name1 = (g_emit.op_imm_a >= 0) ? rt_kw_direct_sym((int)g_emit.op_imm_a, &soff, &cbase) : (const char *)0;
    g_emit.op_imm_b = soff;
    g_emit.op_addr = (uint64_t)(uintptr_t)cbase;
}
void unify_prepare(IR_t *nd) {
    IR_t * src = (nd && nd->n_operands >= 2) ? nd->operands[0] : (IR_t *)0;
    IR_t * ix = (nd && nd->n_operands >= 2) ? nd->operands[1] : (IR_t *)0;
    IR_t * k = (nd && nd->n_operands > 2) ? nd->operands[2] : (IR_t *)0;
    const char * vn = nd ? IR_LIT(nd).sval : (const char *)0;
    int ok = 0, kid = 0, slot = -1; long idx = 0;
    if (nd && nd->n_operands >= 2 && src && ix && ix->op == IR_LIT_INTEGER && (long)IR_LIT(ix).ival >= 0) {
        idx = (long)IR_LIT(ix).ival;
        if (src->op == IR_VAR_REF) { slot = IR_LIT(src).sval ? zrow_opvs(nd, 0) : -1; ok = slot >= 0; }
        else if (src->op == IR_UNIFY_STRUCT) { slot = bb_slot_get(src); ok = slot >= 0 || g_emit.op_zres; kid = 1; }
    }
    g_emit.op_u_kid = kid; g_emit.op_u_slot = slot; g_emit.op_u_idx = idx;
    g_emit.op_u_why = 0; g_emit.op_u_atom = 0; g_emit.op_u_ktag = 0; g_emit.op_u_kval = 0; g_emit.op_u_fid = 0; g_emit.op_u_vo = -1;
    if (nd && nd->op == IR_UNIFY_CONST) {
        g_emit.op_u_why = (ok && k && (k->op == IR_LIT_ATOM || k->op == IR_LIT_INTEGER)) ? 0 : 1;
        if (g_emit.op_u_why == 0) {
            g_emit.op_u_atom = (k->op == IR_LIT_ATOM);
            g_emit.op_u_ktag = g_emit.op_u_atom ? (long)DT_PLATOM : (long)DT_I;
            g_emit.op_u_kval = g_emit.op_u_atom ? (uint64_t)(unsigned)prolog_atom_intern_lit(IR_LIT(k).sval ? IR_LIT(k).sval : "") : (uint64_t)IR_LIT(k).ival;
        }
    }
    if (nd && nd->op == IR_UNIFY_STRUCT) {
        g_emit.op_u_fid = (k && k->op == IR_LIT_INTEGER) ? (long)IR_LIT(k).ival : 0;
        g_emit.op_u_why = !(ok && k && k->op == IR_LIT_INTEGER) ? 1 : (g_emit.op_off < 0 && !g_emit.op_zres) ? 2 : prolog_functor_arity((int)g_emit.op_u_fid) < 0 ? 3 : 0;
    }
    if (nd && (nd->op == IR_UNIFY_FIRST || nd->op == IR_UNIFY_VALUE)) {
        g_emit.op_u_vo = (ok && vn) ? zrow(nd)->vso : -1;
        g_emit.op_u_why = !(ok && vn) ? 1 : g_emit.op_u_vo < 0 ? 4 : 0;
    }
}
}
void emit_text_s(const std::string & s) { if (!s.empty()) emit_text_n(s.data(), s.size()); }
void emit_write_file_s(const char * path, const std::string & s) { FILE * f = fopen(path, "w"); if (f) { fputs(s.c_str(), f); fclose(f); } }
int emit_knob_unless_zero(const char * name) { const char * e = getenv(name); return (e && *e == '0') ? 0 : 1; }
int emit_knob_nonzero(const char * name) { const char * e = getenv(name); return e ? (atoi(e) != 0) : 1; }
int zf_display_level(void) {
    if (!g_emit_cfg || g_emit_cfg->cell_frame) return 0;
    int dl = g_emit_cfg->decl_level;
    return (dl >= 1) ? dl : 0;
}
int zf_pas_nest_graph(void) {
    if (!g_emit_cfg || g_emit_cfg->cell_frame) return 0;
    return g_emit_cfg->decl_level >= 1;
}
int zf_display_mem_off(int dl) {
    if (!g_emit_cfg || !g_emit_cfg->l3_ancestor_name) return -1;
    char nm[32]; snprintf(nm, sizeof nm, "__pas_display_%d", dl);
    return stage2_owner_varslot(g_emit_cfg->l3_ancestor_name, nm);
}
int xa_flat_class_zf(void) {
    if (g_emit.zframe_graph) return 1;
    if (g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc) return 1;
    return 0;
}
void call_marg_plan(int i, IR_t * lf, int aoff, IR_t * owner) {
    marg_t * m = &((marg_t *)g_emit.op_marg.p)[i]; m->kind = MARG_NONE; m->src = -1; m->imm = 0; m->str = 0;
    if (owner && owner == g_emit.node && i >= 0 && i < g_emit.op_arg_slot_n && g_emit.op_arg_slot[i] >= 0) { m->src = g_emit.op_arg_slot[i]; m->kind = m->src == aoff ? MARG_DIRECT : MARG_SLOT; return;
        }
    if (!lf) return;
    { const char * sv = IR_LIT(lf).sval;
      if (lf->op == IR_LIT_INTEGER) { m->kind = MARG_LIT_I; m->imm = (uint64_t)IR_LIT(lf).ival; return; }
      if (lf->op == IR_LIT_REAL) { m->kind = MARG_LIT_F; m->imm = emit_double_bits(IR_LIT(lf).dval); return; }
      if (lf->op == IR_LIT_ATOM) { m->kind = MARG_LIT_ATOM; m->imm = (uint64_t)(unsigned)prolog_atom_intern_lit(sv ? sv : ""); return; }
      if (lf->op == IR_LIT_DSTAR) { std::string star = std::string("*") + (sv ? sv : ""); m->kind = MARG_DSTAR;
          m->str = ct_strdup((std::string(".Ldstar_") + bb_ab_sym_name(star.c_str())).c_str()); m->imm = (uint64_t)(uintptr_t)bb_dstar_rec_addr(star.c_str()); return; }
      if (lf->op == IR_LIT_THUNK) { m->kind = MARG_THUNK; m->str = ct_strdup((std::string(".Lthk_") + bb_ab_sym_name(sv ? sv : "")).c_str());
          m->imm = (uint64_t)(uintptr_t)bb_thunk_rec_addr(sv); return; }
      if (lf->op == IR_LIT_STRING) { m->kind = MARG_LIT_S; m->str = sv ? sv : ""; return; }
      if ((ir_kind0(lf) == IR_CALL && (IR_LIT(lf).dval == 2.0 || IR_LIT(lf).dval == 3.0)) || ir_is_call_kind(ir_kind0(lf))) {
          int staged = (ir_kind0(lf) == IR_CALL_PROC_STAGED || lf->op == IR_PROC_GEN);
          if (owner && owner == g_emit.node && staged && bb_slot_get(lf) >= 0) { m->kind = MARG_SLOT; m->src = bb_slot_get(lf); return; }
          m->kind = MARG_BOMB_TRAMPOLINE; return; }
      if (lf->op == IR_VAR && sv && sv[0] != '&' && is_global(sv)) { m->kind = MARG_BOMB_GLOBAL; return; }
      { int is_local_var = (lf->op == IR_VAR && sv && sv[0] != '&' && !is_global(sv)); int ps = is_local_var ? -1 : bb_slot_get(lf); if (ps < 0 && !is_local_var) ps = zrow_opoff(owner, i);
        if (ps >= 0) { m->src = ps; m->kind = ps == aoff ? MARG_DIRECT : MARG_SLOT; return; } }
      { int voff = zrow_opvs(owner, i); if (voff < 0) { m->kind = MARG_BOMB_VARSLOT; return; } m->kind = MARG_SLOT; m->src = voff; } }
}

#include "emit_head.h"
#include "../planner/planner.h"
g_emitter_t g_emitter;
const char * bb_src_of(const IR_t * nd);
int bb_line_of(const IR_t * nd);
const char * stmt_src_get_file(void);
int stmt_src_nlines(void);
bb_emit_mode_t bb_emit_mode = EMIT_BINARY_WIRED;
int g_sm_native_unsupported = 0;
FILE *bb_emit_out = NULL;
bb_medium_t g_medium = BB_MEDIUM_BINARY;
void (*g_emit_chain_posthook)(void) = NULL;
int g_use_sm_macros = 0;
int g_use_bb_macros = 0;
sm_emit_t g_emit;
IR_graph_t * g_emit_cfg = (IR_graph_t *)0;
static void emit_stno_mark(int32_t stno);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void port_exit_prepass_build(void) {
    extern const char * bb_ab_sym_name(const char *);
    for (int i = 0; i < g_stage2.proc_count; i++) {
        ProcEntry * pe = &g_stage2.proc_table[i];
        if (!pe->name || pe->bb_idx < 0 || pe->bb_idx >= g_stage2.bbp.count) continue;
        IR_graph_t * g = g_stage2.bbp.table[pe->bb_idx];
        if (!g) continue;
        int flat_pat = (pe->thunk_kind == PROC_THUNK_PATTERN) ? 1 : 0;
        int flat_gen = (pe->is_generator && emit_graph_has_suspend(g)) ? 1 : 0;
        int gen_ok = !flat_gen;
        int flat_lcl_proc = (!flat_pat && gen_ok && ((g->nparams > 0 || g->nlocals > 0) || g->cell_frame)) ? 1 : 0;
        int zframe_graph_flag = (g->zframe_graph && !g->cell_frame) ? 1 : 0;
        if (!(zframe_graph_flag || (g->cell_frame && flat_lcl_proc))) continue;
        g_emitter.g_port_exit_promoting_labels.insert(std::string(bb_ab_sym_name(pe->name)) + "_γ");
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_port_exit_label_promotes(const char * label) {
    if (!label) return 0;
    port_exit_prepass_build();
    return g_emitter.g_port_exit_promoting_labels.count(label) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
FILE *emit_outf(void) { return bb_emit_out ? bb_emit_out : stdout; }
#include <string.h>
bb_buf_t bb_emit_buf = NULL;
int bb_emit_pos = 0;
int bb_emit_size = 0;
cv_t bb_patch_list;
int bb_patch_count = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_label_pool_reset(void)
{
    for (int i = 0; i < g_emitter.g_label_pool_n; i++) { bb_label_t *l = g_emitter.g_label_pool[i]; if (l->name_cap) ct_drop((void *)l->name); ct_drop(l); }
    g_emitter.g_label_pool_n = 0; g_emitter.g_label_map.clear();
    g_emitter.g_flt_lbl[1] = g_emitter.g_flt_lbl[2] = g_emitter.g_flt_lbl[3] = (bb_label_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_label_lookup_offset(const char * name)
{
    if (!name) return -1;
    for (int i = 0; i < g_emitter.g_label_pool_n; i++) if (g_emitter.g_label_pool[i] && g_emitter.g_label_pool[i]->offset != BB_LABEL_UNRESOLVED && strcmp(g_emitter.g_label_pool[i]->name, name) == 0)
        return g_emitter.g_label_pool[i]->offset;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * flat_label_kind(IR_e op) { const char * n = bb_op_name(op); int j = 0; if (n && n[0] == 'I' && n[1] == 'R' && n[2] == '_') n += 3;
    if (!n) { snprintf(g_emitter.flat_label_kind_b, sizeof g_emitter.flat_label_kind_b, "op%d", (int)op); return g_emitter.flat_label_kind_b;
        } for (; n[j] && j < 47; j++) g_emitter.flat_label_kind_b[j] = (n[j] >= 'A' && n[j] <= 'Z') ? (char)(n[j] - 'A' + 'a') : n[j]; g_emitter.flat_label_kind_b[j] = 0;
        return g_emitter.flat_label_kind_b; }
static const char * flat_label_kind_of(IR_t * nd) {
    if (g_emit_cfg && g_emit_cfg->n_balias > 0) for (int _bi = 0; _bi < g_emit_cfg->n_balias; _bi++) if (g_emit_cfg->balias_node[_bi] == nd) return g_emit_cfg->balias_name[_bi];
    return flat_label_kind(nd->op);
}
extern "C" const char * bb_kind_name(int op) { return flat_label_kind((IR_e)op); }
extern "C" int emit_patzeta_frame_reserve(const char * name, int * bytes);
int emit_floater_kind(const IR_t * n) { if (!n) return 0; if (n->op == IR_DEFINE && IR_LIT(n).ival == 1) return 1; if (n->op == IR_DEFINE && IR_LIT(n).ival == 2) return 2;
    if (n->op == IR_LIT_STRING && n->γ.node && ir_norm_call_kind(n->γ.node->op) == IR_CALL && IR_LIT(n->γ.node).sval && !strcmp(IR_LIT(n->γ.node).sval, "SNO$NRET")) return 3; return 0; }
int emit_floater_member(const IR_t * n) { if (!n) return 0; if (emit_floater_kind(n)) return 1;
    if (ir_norm_call_kind(n->op) == IR_CALL && IR_LIT(n).sval && !strcmp(IR_LIT(n).sval, "SNO$NRET")) return 1;
    return 0; }
static bb_label_t * emit_floater_label(int k) { static const char * const fn[4] = {0, "RETURN", "FRETURN", "NRETURN"}; if (k < 1 || k > 3) return (bb_label_t *)0;
    if (!g_emitter.g_flt_lbl[k]) g_emitter.g_flt_lbl[k] = g_emitter.g_flt_fam ? emit_label_alloc("%s_%s", fn[k], g_emitter.g_flt_fam) : emit_label_alloc("%s", fn[k]); return g_emitter.g_flt_lbl[k]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_label_t *emit_label_alloc(const char *fmt, ...)
{
    if (g_emitter.g_label_pool_n >= g_emitter.g_label_pool_max) {
        int new_max = g_emitter.g_label_pool_max ? g_emitter.g_label_pool_max * 2 : 64;
        bb_label_t **g = (bb_label_t **)ct_grow(g_emitter.g_label_pool, (size_t)new_max * sizeof(bb_label_t *));
        if (!g) return NULL;
        g_emitter.g_label_pool = g;
        g_emitter.g_label_pool_max = new_max;
    }
    bb_label_t *lbl = (bb_label_t *)ct_zalloc(1, sizeof(bb_label_t));
    if (!lbl) return NULL;
    va_list ap; va_start(ap, fmt);
    char nb[vfmt_len(fmt, ap)]; vsnprintf(nb, sizeof nb, fmt, ap);
    va_end(ap);
    bb_label_name_set(lbl, nb);
    lbl->offset = BB_LABEL_UNRESOLVED;
    g_emitter.g_label_pool[g_emitter.g_label_pool_n++] = lbl;
    g_emitter.g_label_map.emplace(std::string(lbl->name), lbl);
    return lbl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_label_t *emit_label_intern(const char *name)
{
    if (!name) return NULL;
    { auto it = g_emitter.g_label_map.find(std::string(name)); if (it != g_emitter.g_label_map.end() && it->second) return it->second; }
    return emit_label_alloc("%s", name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { bb_label_t * k; int head; uint32_t gen; } bb_patch_hent_t;
static uint32_t bb_patch_hslot(const bb_label_t * l, uint32_t n) {
    uint64_t x = (uint64_t)(uintptr_t)l;
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    return (uint32_t)x & (n - 1);
}
static bb_patch_hent_t * bb_patch_hfind(cv_t * h, const bb_label_t * l, int make) {
    if (h->len == 0) return (bb_patch_hent_t *)0;
    for (uint32_t i = bb_patch_hslot(l, h->len);; i = (i + 1) & (h->len - 1)) {
        bb_patch_hent_t * e = &CV_AT(*h, bb_patch_hent_t, i);
        if (e->gen == g_emitter.bb_patch_gen && e->k == l) return e;
        if (e->gen != g_emitter.bb_patch_gen) {
            if (!make) return (bb_patch_hent_t *)0;
            e->k = (bb_label_t *)l;
            e->head = 0;
            e->gen = g_emitter.bb_patch_gen;
            return e;
        }
    }
}
static void bb_patch_hgrow(void) {
    cv_t * h = &g_emitter.bb_patch_hix, * t = &g_emitter.bb_patch_hix2;
    uint32_t n = h->len ? 2 * h->len : 256;
    cv_reserve(t, (uint32_t)sizeof(bb_patch_hent_t), n, "bb_patch_hix");
    memset(t->p, 0, (size_t)n * sizeof(bb_patch_hent_t));
    t->len = n;
    for (uint32_t i = 0; i < h->len; i++) {
        bb_patch_hent_t * e = &CV_AT(*h, bb_patch_hent_t, i);
        if (e->gen == g_emitter.bb_patch_gen) *bb_patch_hfind(t, e->k, 1) = *e;
    }
    cv_t sw = *h;
    *h = *t;
    *t = sw;
}
static void bb_patch_reset(void) {
    bb_patch_count = 0;
    g_emitter.bb_patch_n = 0;
    g_emitter.bb_patch_hused = 0;
    g_emitter.bb_patch_gen++;
}
static void bb_patch_add(bb_label_t * lbl, bb_patch_kind_t kind) {
    if (2 * (g_emitter.bb_patch_hused + 1) > g_emitter.bb_patch_hix.len) bb_patch_hgrow();
    bb_patch_hent_t * e = bb_patch_hfind(&g_emitter.bb_patch_hix, lbl, 0);
    if (!e) {
        e = bb_patch_hfind(&g_emitter.bb_patch_hix, lbl, 1);
        g_emitter.bb_patch_hused++;
    }
    cv_reserve(&bb_patch_list, (uint32_t)sizeof(bb_patch_t), (uint64_t)g_emitter.bb_patch_n + 1, "bb_patch_list");
    bb_patch_t * pp = &CV_AT(bb_patch_list, bb_patch_t, g_emitter.bb_patch_n);
    pp->site = bb_emit_pos;
    pp->label = lbl;
    pp->kind = kind;
    pp->next = e->head;
    e->head = ++g_emitter.bb_patch_n;
    bb_patch_count++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_emit_begin(bb_buf_t buf, int size)
{
    bb_emit_buf = buf;
    bb_emit_pos = 0;
    bb_emit_size = size;
    bb_patch_reset();
    emit_label_pool_reset();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_emit_end(void)
{
    if (g_emitter.bb_emit_overflow) {
        bb_patch_reset();
        return -1;
    }
    if (bb_patch_count > 0) {
        fprintf(stderr, "bb_emit_end: %d unresolved forward reference(s):\n",
                bb_patch_count);
        for (int i = 0; i < g_emitter.bb_patch_n; i++)
            if (CV_AT(bb_patch_list, bb_patch_t, i).label)
                fprintf(stderr, "  site=%d label='%s'\n",
                        CV_AT(bb_patch_list, bb_patch_t, i).site,
                        CV_AT(bb_patch_list, bb_patch_t, i).label->name);
        abort();
    }
    return bb_emit_pos;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int seq_fold_on(void) { if (g_emitter.seq_fold_on_on < 0) { const char *e = getenv("SCRIP_SEQ_FOLD"); g_emitter.seq_fold_on_on = (e && *e == '0') ? 0 : 1; } return g_emitter.seq_fold_on_on; }
extern "C" void bb_label_alias_reset(void) { g_emitter.g_lblfold_n = 0; }
extern "C" int g_emit_module_pic = 0;
extern "C" void emit_module_mode(int on) { g_emit_module_pic = on ? 1 : 0; }
extern "C" void bb_label_alias(bb_label_t *stub, bb_label_t *tgt) { if (!seq_fold_on() || !stub || !tgt || stub == tgt || g_emitter.g_lblfold_n >= 4096) return;
    g_emitter.g_lblfold_from[g_emitter.g_lblfold_n] = stub;
    g_emitter.g_lblfold_to[g_emitter.g_lblfold_n] = tgt; g_emitter.g_lblfold_n++; }
extern "C" bb_label_t *bb_label_fold(bb_label_t *l) { if (!seq_fold_on() || !l) return l; for (int hop = 0; hop < 16; hop++) { int hit = 0;
    for (int i = 0; i < g_emitter.g_lblfold_n; i++) if (g_emitter.g_lblfold_from[i] == l) { l = g_emitter.g_lblfold_to[i]; hit = 1; break; } if (!hit) break; } return l; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_emit_patch_rel32(bb_label_t *lbl)
{
    lbl = bb_label_fold(lbl);
    if (!MEDIUM_BINARY) {
        fprintf(stderr,
                "bb_emit_patch_rel32: TEXT-mode reach (target='%s') — "
                "use bb_insn_*_rel32 mnemonic helpers\n",
                lbl->name);
        abort();
    }
    if (bb_label_defined(lbl)) {
        int disp = lbl->offset - (bb_emit_pos + 4);
        bb_emit_i32(disp);
        return;
    }
    bb_patch_add(lbl, PATCH_REL32);
    bb_emit_u32(0x00000000);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_emit_patch_abs64(bb_label_t *lbl)
{
    lbl = bb_label_fold(lbl);
    if (!MEDIUM_BINARY) { fprintf(stderr, "bb_emit_patch_abs64: TEXT-mode reach (target='%s')\n", lbl->name); abort(); }
    if (bb_label_defined(lbl)) { bb_emit_u64((uint64_t)(uintptr_t)(bb_emit_buf + lbl->offset)); return; }
    bb_patch_add(lbl, PATCH_ABS64);
    bb_emit_u64(0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_emit_byte(uint8_t b)
{
    if (bb_emit_mode != EMIT_BINARY_WIRED) {
        fprintf(stderr,
                "bb_emit_byte: non-BINARY-mode reach (mode=%d, b=0x%02x) — "
                "convert caller to a named bb_insn_* helper\n",
                (int)bb_emit_mode, (unsigned)b);
        abort();
    }
    if (bb_emit_pos >= bb_emit_size) {
        g_emitter.bb_emit_overflow = 1;
        return;
    }
    bb_emit_buf[bb_emit_pos++] = b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_emit_bytes(const uint8_t * p, size_t k)
{
    if (bb_emit_mode != EMIT_BINARY_WIRED) {
        fprintf(stderr, "bb_emit_bytes: non-BINARY-mode reach (mode=%d, n=%d) — convert caller to a named bb_insn_* helper\n", (int)bb_emit_mode, (int)k);
        abort();
    }
    size_t room = bb_emit_pos < bb_emit_size ? (size_t)(bb_emit_size - bb_emit_pos) : 0, n = k < room ? k : room;
    if (n) memcpy(bb_emit_buf + bb_emit_pos, p, n);
    bb_emit_pos += (int)n;
    if (n < k) g_emitter.bb_emit_overflow = 1;
}
void bb_emit_u32(uint32_t v) { uint8_t b[4] = { (uint8_t)(v), (uint8_t)(v >> 8), (uint8_t)(v >> 16), (uint8_t)(v >> 24) }; bb_emit_bytes(b, 4); }
void bb_emit_u64(uint64_t v) { uint8_t b[8]; memcpy(b, &v, 8); bb_emit_bytes(b, 8); }
void bb_emit_i32(int32_t v) { uint32_t u; memcpy(&u, &v, 4); bb_emit_u32(u); }
int g_is_text = 0;
int g_emit_text_mode = TEXT_MODE_INVOCATION;
int g_emit_pos = 0;
static void emit_gc_rsp_reset(void);
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
int emitter_end(void) { return g_is_text ? g_emit_pos : bb_emit_end(); }
static void ef_b1 (uint8_t a) { bb_emit_byte(a); }
static void ef_b2 (uint8_t a, uint8_t b) { bb_emit_byte(a); bb_emit_byte(b); }
static void ef_b3 (uint8_t a, uint8_t b, uint8_t c) { bb_emit_byte(a); bb_emit_byte(b); bb_emit_byte(c); }
static void ef_b4 (uint8_t a, uint8_t b, uint8_t c, uint8_t d) { bb_emit_byte(a); bb_emit_byte(b); bb_emit_byte(c); bb_emit_byte(d); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ef_text_jmp(const char *mnem, const char *target)
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
void emit_jmp_label(bb_label_t *target, jmp_kind_t kind)
{
    target = bb_label_fold(target);
    static const char * const mn[] = {"jmp","je","jne","jl","jge","jg"};
    static const uint8_t ops[6][2] = {{0xE9,0x00},{0x0F,0x84},{0x0F,0x85},{0x0F,0x8C},{0x0F,0x8D},{0x0F,0x8F}};
    int k = (int)kind < 6 ? (int)kind : 0;
    if (g_is_text) { ef_text_jmp(mn[k], target->name); g_emit_pos += 6; }
    else { if (k==0) ef_b1(0xE9); else ef_b2(ops[k][0], ops[k][1]); bb_emit_patch_rel32(target); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_label_define(bb_label_t *lbl)
{
    if (!MEDIUM_BINARY) {
        emit_textf("%s:\n", lbl->name);
        return;
    }
    if (g_emitter.bb_emit_overflow) return;
    lbl->offset = bb_emit_pos;
    { if (g_emitter.bb_label_define_bm < 0) { const char *e = getenv("SCRIP_BLOB_MAP"); g_emitter.bb_label_define_bm = (e && *e == '1') ? 1 : 0;
        } if (g_emitter.bb_label_define_bm) fprintf(stderr, "BLOBMAP %p %s\n", (void *)(bb_emit_buf + bb_emit_pos), lbl->name); }
    bb_patch_hent_t * e = bb_patch_hfind(&g_emitter.bb_patch_hix, lbl, 0);
    for (int i = e ? e->head - 1 : -1; i >= 0; i = CV_AT(bb_patch_list, bb_patch_t, i).next - 1) {
        bb_patch_t *p = &CV_AT(bb_patch_list, bb_patch_t, i);
        int target = lbl->offset;
        if (p->kind == PATCH_REL8) {
            int disp = target - (p->site + 1);
            if (disp < -128 || disp > 127) {
                fprintf(stderr, "bb_label_define: rel8 overflow for '%s': disp=%d\n",
                        lbl->name, disp);
                abort();
            }
            bb_emit_buf[p->site] = (uint8_t)(int8_t)disp;
        } else if (p->kind == PATCH_ABS64) {
            uint64_t a = (uint64_t)(uintptr_t)(bb_emit_buf + target);
            for (int j = 0; j < 8; j++) bb_emit_buf[p->site + j] = (uint8_t)(a >> (8 * j));
        } else {
            int disp = target - (p->site + 4);
            uint32_t u;
            memcpy(&u, &disp, 4);
            bb_emit_buf[p->site + 0] = (uint8_t)(u );
            bb_emit_buf[p->site + 1] = (uint8_t)(u >> 8);
            bb_emit_buf[p->site + 2] = (uint8_t)(u >> 16);
            bb_emit_buf[p->site + 3] = (uint8_t)(u >> 24);
        }
        p->label = (bb_label_t *)0;
        bb_patch_count--;
    }
    if (e) e->head = 0;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define STNO_SRC(i) CV_AT(g_emit.stno_src, sno_stno_src_t, (i))
static const char * icn_trace_intern(const char * s);
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
static void emit_stno_row(int32_t stno, int32_t line, const char * file) {
    if (g_is_text) {
        char fq[32]; if (file) { emit_stno_file_label(file); snprintf(fq, sizeof fq, ".Lstnof%d", g_emit.stno_file_lbl); } else snprintf(fq, sizeof fq, "0");
        const char * kn = flat_label_kind(g_emit.node->op); int uid = g_emit.x86_uid;
        emit_textf(".L%s_α_%d_stno:\n  .pushsection scrip_stno_map,\"aw\",@progbits; .p2align 3\n", kn, uid);
        emit_textf("  .quad .L%s_α_%d_stno\n  .long %d\n  .long %d\n  .quad %s\n  .popsection\n", kn, uid, (int)stno, (int)line, fq);
    } else {
        stno_rec_push((uint64_t)(uintptr_t)(bb_emit_buf + bb_emit_pos), stno, line, file ? icn_trace_intern(file) : (const char *)0);
    }
}
static void emit_stno_mark(int32_t stno) {
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
#include "IR.h"
#include "emit_ir.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_limit_init(int limit_slot_off) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_emit_limit_init"); abort(); }
int g_m4_dense_nid = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_node_id(IR_t * nd) {
    if (!nd) return 0; if (!g_m4_dense_nid) return (int)((uintptr_t)nd % 100000u);
    if ((uint64_t)(g_emitter.g_nid_count + 1) * 2 > g_emitter.g_nid.len) { uint32_t nc = g_emitter.g_nid.len ? g_emitter.g_nid.len * 2 : 8; cv_t o = g_emitter.g_nid, n = { 0, 0, 0, 0 };
        n.p = ct_zalloc(nc, sizeof(nid_slot_t)); n.len = nc; n.cap = nc;
        n.esz = (uint32_t)sizeof(nid_slot_t); g_emitter.g_nid = n;
        for (uint32_t i = 0; i < o.len; i++) { nid_slot_t q = CV_AT(o, nid_slot_t, i); if (!q.key) continue; uintptr_t j = ((uintptr_t)q.key >> 4) & (g_emitter.g_nid.len - 1);
            while (CV_AT(g_emitter.g_nid, nid_slot_t, j).key) j = (j + 1u) & (g_emitter.g_nid.len - 1); CV_AT(g_emitter.g_nid, nid_slot_t, j) = q; } }
    uintptr_t h = ((uintptr_t)nd >> 4) & (g_emitter.g_nid.len - 1);
        while (CV_AT(g_emitter.g_nid, nid_slot_t, h).key) { if (CV_AT(g_emitter.g_nid, nid_slot_t, h).key == nd) return CV_AT(g_emitter.g_nid, nid_slot_t, h).val;
        h = (h + 1u) & (g_emitter.g_nid.len - 1); }
    CV_AT(g_emitter.g_nid, nid_slot_t, h).key = nd; return CV_AT(g_emitter.g_nid, nid_slot_t, h).val = ++g_emitter.g_nid_count;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void xa_dispatch(XA_op_t op)
{
    switch (op) {
    case XA_MACRO_LIBRARY_OPEN: xa_macro_library_open(); return;
    case XA_MACRO_LIBRARY_CLOSE: xa_macro_library_close(); return;
    case XA_BB_MACRO_LIBRARY: xa_bb_macro_library(); return;
    case XA_EXEC_STMT_BLOB: xa_exec_stmt_blob(); return;
    case XA_FILE_HEADER: xa_file_header(); return;
    case XA_FILE_FOOTER: xa_file_footer(); return;
    case XA_BB_PTR_SLOT: xa_bb_ptr_slot(); return;
    case XA_ENTRY_DISPATCH: xa_entry_dispatch(); return;
    case XA_FLAT_DATA_SECTION: xa_flat_data_section(); return;
    case XA_PROLOGUE: xa_prologue(); return;
    case XA_EPILOGUE: xa_epilogue(); return;
    case XA_EXPRESSION_REGISTRY: xa_expression_registry(); return;
    case XA_KIDS_RODATA: return;
    case XA_SUB_BUILDER: return;
    case XA_BUILDER: return;
    case XA_REGISTRY_TABLE: return;
    case XA_STRTAB_RODATA: xa_strtab_rodata(); return;
    case XA_CSETTAB_RODATA: xa_csettab_rodata(); return;
    case XA_CAP_FIXUP: xa_cap_fixup(); return;
    case XA_PATTERN_BLOBS: xa_pattern_blobs(); return;
    default: return;
    }
}
#define STRTAB(i) CV_AT(g_emitter.g_strtab, strtab_ent_t, (i))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void strtab_reset(void) { for (int i = 0; i < g_emitter.g_strtab_n; i++) ct_drop((void *)STRTAB(i).s); g_emitter.g_strtab_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int strtab_intern(const char *s)
{
    if (!s) s = "";
    for (int i = 0; i < g_emitter.g_strtab_n; i++)
        if (STRTAB(i).s == s || strcmp(STRTAB(i).s, s) == 0) return STRTAB(i).idx;
    int idx = g_emitter.g_strtab_n;
    cv_reserve(&g_emitter.g_strtab, (uint32_t)sizeof(strtab_ent_t), (uint64_t)g_emitter.g_strtab_n + 1, "g_strtab");
    STRTAB(g_emitter.g_strtab_n).s = ct_strdup(s); STRTAB(g_emitter.g_strtab_n).idx = idx; g_emitter.g_strtab_n++;
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void strtab_label(char *buf, size_t bufsz, const char *s)
{
    if (!s) s = "";
    int idx = strtab_intern(s);
    snprintf(buf, bufsz, ".S%d", idx);
}
static size_t rodata_chars_put(cv_t * chars, const char * t) { size_t n = strlen(t) + 1, off = chars->len; cv_reserve(chars, 1, (uint64_t)chars->len + n, "rodata_chars");
    memcpy((char *)chars->p + off, t, n); chars->len += (uint32_t)n; return off; }
static void rodata_chars_point(cv_t * chars, cv_t * offs) { for (uint32_t i = 0; i < offs->len; i++) CV_AT(*offs, const char *, i) = (const char *)chars->p + CV_AT(*offs, size_t, i); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void xa_emit_strtab_rodata(void)
{
    if (g_emitter.g_strtab_n <= 0) { strtab_reset(); return; }
    g_emitter.xa_emit_strtab_rodata_chars.len = 0; g_emitter.xa_emit_strtab_rodata_labels.len = 0; g_emitter.xa_emit_strtab_rodata_escaped.len = 0;
    for (int i = 0; i < g_emitter.g_strtab_n; i++) {
        char lb[24]; snprintf(lb, sizeof lb, ".S%d:", STRTAB(i).idx);
        CV_PUSH(g_emitter.xa_emit_strtab_rodata_labels, size_t) = rodata_chars_put(&g_emitter.xa_emit_strtab_rodata_chars, lb);
        CV_PUSH(g_emitter.xa_emit_strtab_rodata_escaped, size_t) = rodata_chars_put(&g_emitter.xa_emit_strtab_rodata_chars, gas_escape_str(STRTAB(i).s ? STRTAB(i).s : "").c_str());
    }
    rodata_chars_point(&g_emitter.xa_emit_strtab_rodata_chars, &g_emitter.xa_emit_strtab_rodata_labels);
        rodata_chars_point(&g_emitter.xa_emit_strtab_rodata_chars, &g_emitter.xa_emit_strtab_rodata_escaped);
    g_emit.xa_strtab_n = g_emitter.g_strtab_n;
    g_emit.xa_strtab_labels = (const char **)g_emitter.xa_emit_strtab_rodata_labels.p;
    g_emit.xa_strtab_escaped = (const char **)g_emitter.xa_emit_strtab_rodata_escaped.p;
    xa_dispatch(XA_STRTAB_RODATA);
    g_emit.xa_strtab_n = 0;
    g_emit.xa_strtab_labels = NULL;
    g_emit.xa_strtab_escaped = NULL;
    strtab_reset();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_cset32(void) { if (g_emitter.sn4_cset32__c < 0) { const char * e = getenv("SCRIP_CSET32"); g_emitter.sn4_cset32__c = (e && *e == '1') ? 1 : 0; } return g_emitter.sn4_cset32__c; }
#define CSETTAB(i) CV_AT(g_emitter.g_csettab, csettab_ent_t, (i))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void csettab_reset(void) { g_emitter.g_csettab_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const void *csettab_label(char *buf, size_t bufsz, const char *cset)
{
    if (!cset) cset = "";
    for (int i = 0; i < g_emitter.g_csettab_n; i++)
        if (CSETTAB(i).s == cset || strcmp(CSETTAB(i).s, cset) == 0) { snprintf(buf, bufsz, ".C%d", CSETTAB(i).idx); return sn4_cset32() ? (const void *)CSETTAB(i).bits : (const void *)CSETTAB(i).tbl;
            }
    int idx = g_emitter.g_csettab_n;
    cv_reserve(&g_emitter.g_csettab, (uint32_t)sizeof(csettab_ent_t), (uint64_t)g_emitter.g_csettab_n + 1, "g_csettab");
    { unsigned char * pl = (unsigned char *)ct_zalloc(256 + 32, 1); CSETTAB(idx).s = cset; CSETTAB(idx).idx = idx; CSETTAB(idx).tbl = pl; CSETTAB(idx).bits = pl + 256; }
    for (const unsigned char *p = (const unsigned char *)cset; *p; ++p) { CSETTAB(idx).tbl[*p] = 1; CSETTAB(idx).bits[*p >> 3] |= (unsigned char)(1u << (*p & 7)); }
    g_emitter.g_csettab_n++;
    snprintf(buf, bufsz, ".C%d", idx);
    return sn4_cset32() ? (const void *)CSETTAB(idx).bits : (const void *)CSETTAB(idx).tbl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void xa_emit_csettab_rodata(void)
{
    if (g_emitter.g_csettab_n <= 0) { csettab_reset(); return; }
    g_emitter.xa_emit_csettab_rodata_chars.len = 0; g_emitter.xa_emit_csettab_rodata_rows.len = 0; g_emitter.xa_emit_csettab_rodata_labels.len = 0;
    for (int i = 0; i < g_emitter.g_csettab_n; i++) {
        char lb[24]; snprintf(lb, sizeof lb, ".C%d:", CSETTAB(i).idx);
        CV_PUSH(g_emitter.xa_emit_csettab_rodata_labels, size_t) = rodata_chars_put(&g_emitter.xa_emit_csettab_rodata_chars, lb);
        std::string r;
        if (sn4_cset32()) { for (int j = 0; j < 32; j += 16) { r += " .byte "; for (int k = 0; k < 16; k++) { r += std::to_string((int)CSETTAB(i).bits[j + k]); r += (k < 15) ? "," : "\n"; } } }
        else for (int j = 0; j < 256; j += 16) {
            r += " .byte ";
            for (int k = 0; k < 16; k++) { r += std::to_string((int)CSETTAB(i).tbl[j + k]); r += (k < 15) ? "," : "\n"; }
        }
        CV_PUSH(g_emitter.xa_emit_csettab_rodata_rows, size_t) = rodata_chars_put(&g_emitter.xa_emit_csettab_rodata_chars, r.c_str());
    }
    rodata_chars_point(&g_emitter.xa_emit_csettab_rodata_chars, &g_emitter.xa_emit_csettab_rodata_labels);
        rodata_chars_point(&g_emitter.xa_emit_csettab_rodata_chars, &g_emitter.xa_emit_csettab_rodata_rows);
    g_emit.xa_csettab_n = g_emitter.g_csettab_n;
    g_emit.xa_csettab_labels = (const char **)g_emitter.xa_emit_csettab_rodata_labels.p;
    g_emit.xa_csettab_rows = (const char **)g_emitter.xa_emit_csettab_rodata_rows.p;
    xa_dispatch(XA_CSETTAB_RODATA);
    g_emit.xa_csettab_n = 0;
    g_emit.xa_csettab_labels = NULL;
    g_emit.xa_csettab_rows = NULL;
    csettab_reset();
}
int bb_slot_get(IR_t *nd);
static int zrow_of(const IR_t * nd);
static int zrow_under(int ri);
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
extern atp_t * bb_atp_new (const char *varname);
extern cap_t * bb_cap_new_call (bb_box_fn child_fn, void *child_state, const char *fnc_name, DESCR_t *fnc_args, int fnc_nargs, char **fnc_arg_names, int fnc_n_arg_names, int immediate);
extern cap_t * bb_cap_new (bb_box_fn child_fn, void *child_state, const char *varname, DESCR_t *var_ptr, int immediate);
void (*g_cap_fixup_cb)(void *cap_ptr, const char *child_α_label) = NULL;
const char *child_cache_get_lbl(bb_box_fn fn);
static long flat_buf_max_v() { if (g_emitter.flat_buf_max_v_v < 0) { const char * e = getenv("SCRIP_FLAT_BUF_MB"); long mb = e && *e ? atol(e) : 4; if (mb < 4) mb = 4; if (mb > 256) mb = 256;
    g_emitter.flat_buf_max_v_v = mb * 1024L * 1024L; } return g_emitter.flat_buf_max_v_v; }
#define FLAT_BUF_MAX (flat_buf_max_v())
int g_flat_node_id = 0;
int g_last_flat_frame_bytes = 0;
int g_last_flat_zstatic = 0;
int g_last_flat_fp = 0;
int g_last_flat_uniform = 0;
extern "C" int zls_g_fp_total(IR_graph_t *);
extern "C" int zls_node_has_fields(const IR_t *);
extern "C" int zls_g_block_args(const IR_graph_t *); extern "C" int zls_g_det_block(const IR_graph_t *); extern "C" int zls_g_resumable_frame(const IR_graph_t *);
extern "C" int emit_patzeta_lookup(const char *, int *);
extern "C" void zls_fct_finalize(IR_graph_t *, int);
extern "C" int fc_tail_defer_susp_g(IR_graph_t *, const IR_t *);
extern "C" int fc_tail_dfr(const IR_t *);
extern "C" int sno_name_prologue_bound(const char *);
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
static void bb_slotmap_push(IR_t *nd, int off) {
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_slot_register(IR_t *nd, int off) {
    bb_slotmap_push(nd, off);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_varslot_peek(const char *name) {
    int c = zst_coord(name); if (c >= 0) return c;
    return ir_varslot_of(g_emit_cfg, name);
}
static int drive_local_slot(const char *vn) {
    int voff = bb_varslot_peek(vn);
    if (voff < 0 || !g_emit.op_zres || g_emit.op_zf_unpinned) return voff;
    if (g_emit.zv_home_cfg != (const void *)g_emit_cfg) { g_emit.zv_home.len = 0; g_emit.zv_home_cfg = (const void *)g_emit_cfg; }
    for (uint32_t k = 0; k < g_emit.zv_home.len; k++) { zv_home_t * h = &CV_AT(g_emit.zv_home, zv_home_t, k); if (!strcmp(h->name, vn)) return voff + g_emit.op_zrun - h->depth; }
    CV_PUSH(g_emit.zv_home, zv_home_t) = (zv_home_t){ vn, g_emit.op_zrun };
    return voff;
}
int g_proc_direct_active = 0;
int g_gva_active = 0;
int g_emit_frame_caller_dl = -1;
int g_frame_active = 0;
int g_flat_outer_nparams = 0;
int g_gen_proc_active = 0;
int g_resumable_callable_active = 0;
int g_scan_regs_live = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fbdiv_report(void) { fprintf(stderr, "FB-DIVERGE TOTAL=%d\n", g_emitter.fbdiv_n); }
static int emit_fb_bit_of(IR_t * nd);
static int emit_fb_stmt_scan(IR_graph_t * g);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_fb_divergence_check(void) { if (g_emitter.fbdiv_mode < 0) { const char * e = getenv("SCRIP_FB_DIVERGE"); g_emitter.fbdiv_mode = (e && *e == '1') ? 1 : 0; } if (!g_emitter.fbdiv_mode) return;
    if (!g_emitter.fbdiv_hooked) { g_emitter.fbdiv_hooked = 1; atexit(fbdiv_report); } if (emit_jmp_pin_legacy() == emit_rec_pin()) return; g_emitter.fbdiv_n++;
    fprintf(stderr, "FB-DIVERGE #%d graph=%s data_fb=%s rec_fb=%s deep=%d pat=%d gen=%d genproc=%d rescall=%d\n", g_emitter.fbdiv_n, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "<anon>",
    emit_jmp_pin_legacy() ? "rsp" : "rsp", emit_rec_pin() ? "rsp" : "rsp", g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, g_resumable_callable_active); }
static bb_label_t *g_create_body_entry = NULL;
cv_t g_flat_data_buf;
#define FLAT_DATA_TEXT ((char *)g_flat_data_buf.p)
size_t g_flat_data_len = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *child_cache_get_lbl(bb_box_fn fn) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "child_cache_get_lbl"); abort(); }
int g_flat_data_any = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void data_buf_reset(void) { g_flat_data_len = 0; g_emitter.g_flat_data_active = 0; g_flat_data_any = 0; g_emitter.g_flat_data_just_closed = 0; g_emitter.g_flat_data_pending_lbl = ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void data_buf_appendf(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt); int n = vfmt_len(fmt, ap);
    cv_reserve(&g_flat_data_buf, 1, (uint64_t)g_flat_data_len + (uint64_t)n, "g_flat_data_buf"); vsnprintf(FLAT_DATA_TEXT + g_flat_data_len, (size_t)n, fmt, ap); va_end(ap);
    g_flat_data_len += (size_t)n - 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void data_buf_flush_pending_label(void) { if (!*g_emitter.g_flat_data_pending_lbl) return; data_buf_appendf("%s\n", g_emitter.g_flat_data_pending_lbl); g_emitter.g_flat_data_pending_lbl = ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *emit_intern_str(const char *s) { (void)s; return NULL; }
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
static void flat_emit_arg_subchain(IR_t *entry, bb_label_t *succ, bb_label_t *fail);
static void emit_chain_operand_refs(IR_t *entry);
static void gvar_stmt_operand_refs(IR_t *head);
static int gvar_prewalk_idx_operand(IR_t *idx, bb_label_t *lbl_ω);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * bb_child0(const IR_t *n) { return (n && n->n_operands > 0) ? n->operands[0] : NULL; }
IR_t * bb_child1(const IR_t *n) { return (n && n->n_operands > 1) ? n->operands[1] : NULL; }
extern int memcmp(const void *, const void *, size_t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void g_bb_alpha_seq_reset(void) { g_emitter.g_bb_alpha_seq = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_fill_alpha(IR_t *nd) {
    extern int g_m4_dense_nid;
    bb_label_t *a = &g_emitter.g_α_ring[g_emitter.g_α_ring_i++ & 7];
    { int _q = g_m4_dense_nid ? ++g_emitter.g_bb_alpha_seq : (nd ? bb_node_id(nd) : 0); char nb[fmt_len("bb%d_α", _q)]; snprintf(nb, sizeof nb, "bb%d_α", _q);
      bb_label_name_set(a, nb); a->offset = BB_LABEL_UNRESOLVED; }
    g_emit.lbl_α = a->name;
    g_emit.lbl_α_p = a;
}
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
static void gz_emit_catch(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv) {
    fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gz_emit_catch"); abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_kind_is_driver_owned(int t) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_kind_is_driver_owned"); abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int fc_pair_extent(const IR_t *);
inline int nidx(IR_t **nodes, int n, const IR_t *e);
extern "C" int zls_dj_arm_resumes(const IR_t * nd, int a);
extern "C" int zls_dj_rbp_arms(const IR_t * nd);
extern "C" int zls_dj_retains(const IR_t * nd);
extern "C" int zls_frame_word_why(const IR_t * nd);
extern "C" int zls_home_local_why(const IR_t * nd);
extern "C" int zls_g_home_frame(const IR_graph_t * g);
extern "C" int zls_frame_home(const IR_t * nd);
extern "C" int zls_g_value_region(const IR_graph_t * g);
extern "C" int zls_key_unbacked(const IR_graph_t * g, int off);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arbno_body_slot_window(const IR_t * nd, int * out_lo, int * out_bytes);
int blob_frame_scope(void);
static int blob_head_bytes(void);
static int * xci_ints(cv_t * v, uint32_t n);
static int rk_land_ngroups(const IR_t * nd) { const char * s = nd ? IR_LIT(nd).sval : (const char *)0; return s ? atoi(s) : 0; }
static int64_t rk_blob_bounds(const char * blob) {
    long mn = 0, mx = 0, fl = 0;
    if (!blob || !blob[0] || sscanf(blob + 1, "%ld,%ld,%ld;", &mn, &mx, &fl) != 3) return 0;
    return (int64_t)(mn & 0xFFFF) | ((int64_t)(mx & 0xFFFF) << 16) | ((int64_t)fl << 32);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fence0_whack_on() { if (g_emitter.fence0_whack_on_v < 0) { const char * e = getenv("SCRIP_FENCE0_WHACK"); g_emitter.fence0_whack_on_v = (e && *e == '0') ? 0 : 1;
    } return g_emitter.fence0_whack_on_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fence0_dyn_on() { if (g_emitter.fence0_dyn_on_v < 0) { const char * e = getenv("SCRIP_FENCE0_DYNAMIC"); g_emitter.fence0_dyn_on_v = (e && *e == '0') ? 0 : 1;
    } return g_emitter.fence0_dyn_on_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fence0_dyn_floor(const IR_t * nd) {
    int blob_frame_scope(void); int blob_carve_bytes(void);
    int _fzd = getenv("SCRIP_FZ_DIAG") ? 1 : 0;
    if (!fence0_dyn_on()) return 0;
    if (!nd || !g_emit_cfg || nd->op != IR_MATCH_FENCE0) return 0;
    if (!blob_frame_scope()) { if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p dyn: not blob scope\n", (const void *)nd); return 0; }
    int n = g_emit_cfg->n; int guard = 0; IR_t * cur = zd_chase(nd->γ.node); const IR_t ** met = (const IR_t **)alloca(sizeof(IR_t *) * (size_t)(n + 1)); int nmet = 0;
    while (cur && guard++ <= n) { int mo = (int)ir_kind0(cur);
        if (mo == IR_MATCH_END) break;
        if (mo == IR_MATCH_ALTERNATE || mo == IR_MATCH_ARBNO || mo == IR_MATCH_FENCE1 || mo == IR_MATCH_FENCE0 || mo == IR_MATCH_VALUE || mo == IR_CALL || mo == IR_CALL_VALUE ||
            mo == IR_DISJUNCTION || mo == IR_MATCH_ABORT) { if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p dyn: dynamic successor %s\n", (const void *)nd, bb_op_name((IR_e)mo)); return 0; }
        if ((mo == IR_MATCH_ASSIGN_COND || mo == IR_MATCH_ASSIGN_IMM) && cur->n_operands > 1) { int own = 0; for (int s = 0; s < nmet; s++) if (met[s] == cur->operands[1]) { own = 1; break; }
            if (!own) { if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p dyn: successor %s reads a save cell the fence would drop\n", (const void *)nd, bb_op_name((IR_e)mo)); return 0; } }
        met[nmet++] = cur;
        cur = zd_chase(cur->γ.node); }
    if (guard > n) { if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p dyn: successor chase did not terminate\n", (const void *)nd); return 0; }
    if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p dyn: FLOOR=%d\n", (const void *)nd, blob_carve_bytes());
    return blob_carve_bytes();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fence0_release_bytes(const IR_t * nd) {
    if (!fence0_whack_on()) return 0;
    if (!nd || !g_emit_cfg || nd->op != IR_MATCH_FENCE0) return 0;
    int n = g_emit_cfg->n; if (n <= 0) return 0;
    int _fzd = getenv("SCRIP_FZ_DIAG") ? 1 : 0;
    IR_t ** seq = (IR_t **)alloca(sizeof(IR_t *) * (size_t)n); int ns = -1;
    for (int b = 0; b < n && ns < 0; b++) { IR_t * mb = g_emit_cfg->all[b]; if (!mb || mb->op != IR_MATCH_BEGIN) continue;
        int cnt = 0, guard = 0; IR_t * cur = zd_chase(mb->γ.node);
        while (cur && guard++ <= n) { if (cur == nd) { ns = cnt; break; } if (cnt >= n) break; seq[cnt++] = cur; cur = zd_chase(cur->γ.node); } }
    { int has_mb = 0; for (int b = 0; b < n; b++) if (g_emit_cfg->all[b] && g_emit_cfg->all[b]->op == IR_MATCH_BEGIN) { has_mb = 1; break; }
      if (ns < 0 && !has_mb && g_emit_cfg->entry) { int cnt = 0, guard = 0; IR_t * cur = zd_chase(g_emit_cfg->entry);
        while (cur && guard++ <= n) { if (cur == nd) { ns = cnt; break; } if (cnt >= n) break; seq[cnt++] = cur; cur = zd_chase(cur->γ.node); } } }
    if (ns < 0) { if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p not on any MATCH_BEGIN or thunk-entry forward chain -- bill 0\n", (const void *)nd); return 0; }
    if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p executed-prefix len=%d (cfg_n=%d)\n", (const void *)nd, ns, n);
    int total = 0;
    for (int j = ns - 1; j >= 0; j--) { IR_t * m = seq[j]; int mo = (int)ir_kind0(m);
        if (_fzd) fprintf(stderr, "[FZ-DIAG]   step j=%-3d op=%-22s zd_k=%-3d total=%d\n", j, bb_op_name((IR_e)mo), zd_k(m), total);
        if (mo == IR_MATCH_BEGIN || mo == IR_STATEMENT_BEGIN) break;
        if (mo == IR_MATCH_ALTERNATE || mo == IR_MATCH_ARBNO || mo == IR_MATCH_FENCE1 || mo == IR_MATCH_FENCE0 || mo == IR_MATCH_DEFER || mo == IR_MATCH_VALUE || mo == IR_CALL ||
            mo == IR_CALL_VALUE || mo == IR_DISJUNCTION || mo == IR_MATCH_ABORT) break;
        if (mo == IR_MATCH_SPAN || mo == IR_MATCH_BREAK || mo == IR_MATCH_BREAKX || mo == IR_MATCH_TAB || mo == IR_MATCH_RTAB || mo == IR_MATCH_REM || mo == IR_MATCH_BAL) { total += zd_k(m); continue;
            }
        if (zd_k(m) == 0) continue;
        break; }
    if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p RELEASE=%d\n", (const void *)nd, total);
    return total;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_prepare(IR_t *nd) {
    g_emit.bb_ls = NULL;
    g_emit.bb_rs = NULL;
    g_emit.bb_op_lbl = NULL;
    g_emit.bb_lk = -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_classify_node(IR_t *nd) {
    g_emit.op_frame_need = (planner_act_class_of(nd) == ACT_CLASS_CAPTURE && planner_act_slot(nd) != -1) ? 1 : 0;
    { if (g_emitter.bb_classify_node__ed < 0) { const char * e = getenv("SCRIP_EARN_DIAG"); g_emitter.bb_classify_node__ed = (e && *e == '1') ? 1 : 0; }
      if (g_emitter.bb_classify_node__ed && nd)
          fprintf(stderr, "[EARN] op=%d need=%d haz=%d pat_static=%d n_op=%d\n", nd->op, g_emit.op_frame_need, planner_act_cause_of(nd), nd->pat_static, nd->n_operands); }
    { if (g_emitter.bb_classify_node__cd < 0) { const char * e = getenv("SCRIP_CLASS_DIAG"); g_emitter.bb_classify_node__cd = (e && *e == '1') ? 1 : 0; }
      if (g_emitter.bb_classify_node__cd && nd) { extern const char * bb_op_name(IR_e);
        if (emit_rec_pin() || g_emit.op_stmt_dyn) fprintf(stderr, "[CLS:NOFIX] op=%s(%d) pin=%d dyn=%d\n", bb_op_name(nd->op), (int)nd->op, emit_rec_pin(), g_emit.op_stmt_dyn);
        if (g_emit.op_frame_need && nd->op != IR_MATCH_FENCE0) fprintf(stderr, "[CLS:NOWHACK] op=%s(%d)\n", bb_op_name(nd->op), (int)nd->op); } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bb_arith_dyn_kind(IR_t *o) { return o && (o->op == IR_CALL || ir_is_call_kind(o->op)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bb_arith_materializable(IR_t *o) {
    return o && (o->op == IR_CALL || ir_is_call_kind(o->op) || o->op == IR_LIT_INTEGER || (o->op == IR_VAR && IR_LIT(o).sval));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int arith_emits_descr(IR_t *o) {
    if (!o || o->op != IR_BINOP || !bb_arith_is_dynamic(o)) return 0;
    IR_t *a = bb_child0(o), *b = bb_child1(o);
    int a_ok = a && (a->op == IR_CALL || ir_is_call_kind(a->op) || arith_emits_descr(a));
    int b_ok = b && (b->op == IR_CALL || ir_is_call_kind(b->op) || arith_emits_descr(b));
    return a_ok && b_ok;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int binop_slot_kind(IR_t *nd) {
    int64_t op = nd ? IR_LIT(nd).ival : -1;
    if ((op >= BINOP_LT && op <= BINOP_NE) || (op >= BINOP_SLT && op <= BINOP_SNE) || op == BINOP_EQV || op == BINOP_NEQV) return BINOP_CAT_RELOP;
    if (binop_is_concat((long)op)) return BINOP_CAT_CONCAT;
    if (op == BINOP_XREP) return BINOP_CAT_XREP;
    if (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD || op == BINOP_POW) return BINOP_CAT_ARITH;
    if (op == BINOP_CUNION || op == BINOP_CDIFF || op == BINOP_CINTER) return BINOP_CAT_ARITH;
    return BINOP_CAT_ARITH;
}
extern "C" int zls_off(const IR_t *);
extern "C" int zls_frame_off(const IR_graph_t *, const IR_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int nd_slot(const IR_t *nd) { return nd ? zls_off(nd) : -1; }
static int xf_diag_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_XF_DIAG"); v = (e && *e == '1') ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_binop_opnd_slot(IR_t *o) {
    if (!o) return -1;
    extern int is_global(const char *);
    if (o->op == IR_VAR && IR_LIT(o).sval && IR_LIT(o).sval[0] != '&' && (!is_global(IR_LIT(o).sval) || graph_has_local(g_emit_cfg, IR_LIT(o).sval))) { int voff = bb_varslot_peek(IR_LIT(o).sval);
        if (voff >= 0) return voff; }
    int s = bb_slot_get(o); if (s >= 0) return s;
    return nd_slot(o);
}
static int binop_operand_real_static(IR_graph_t *g, IR_t *o, int depth);
static uint64_t bnr_hash(const void *k, int by_name) { uint64_t h = 1469598103934665603ull; if (!by_name) return ((uint64_t)(uintptr_t)k >> 4) * 11400714819323198485ull;
    for (const unsigned char *s = (const unsigned char *)k; *s; s++) h = (h ^ *s) * 1099511628211ull; return h; }
static int bnr_same(const void *a, const void *b, int by_name) { return a == b || (by_name && strcmp((const char *)a, (const char *)b) == 0); }
static void bnr_reset(void) { g_emitter.g_bnr_gen++; g_emitter.g_bnr_nn = 0; g_emitter.g_bnr_nm = 0; }
static void bnr_grow(cv_t *v, int by_name) {
    uint32_t oc = v->len, nc = oc ? oc * 2 : 64; cv_t o = *v; cv_t n = { 0, 0, 0, 0 };
    n.p = ct_zalloc(nc, sizeof(bnr_slot_t)); n.len = nc; n.cap = nc; n.esz = (uint32_t)sizeof(bnr_slot_t);
    for (uint32_t i = 0; i < oc; i++) { bnr_slot_t s = CV_AT(o, bnr_slot_t, i); if (s.gen != g_emitter.g_bnr_gen || !s.k) continue;
        uint64_t h = bnr_hash(s.k, by_name) & (nc - 1); while (CV_AT(n, bnr_slot_t, h).gen == g_emitter.g_bnr_gen && CV_AT(n, bnr_slot_t, h).k) h = (h + 1) & (nc - 1); CV_AT(n, bnr_slot_t, h) = s; }
    *v = n;
}
static int bnr_seen(cv_t *v, uint32_t *cnt, const void *k, int by_name) {
    if ((uint64_t)(*cnt + 1) * 2 > v->len) bnr_grow(v, by_name);
    uint64_t h = bnr_hash(k, by_name) & (v->len - 1);
    for (;;) { bnr_slot_t *s = &CV_AT(*v, bnr_slot_t, h);
        if (s->gen != g_emitter.g_bnr_gen || !s->k) { s->k = k; s->gen = g_emitter.g_bnr_gen; (*cnt)++; return 0; }
        if (bnr_same(s->k, k, by_name)) return 1;
        h = (h + 1) & (v->len - 1); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bnr_node_seen(IR_t *o) { return bnr_seen(&g_emitter.g_bnr_vnode, &g_emitter.g_bnr_nn, o, 0); }
static int bnr_name_seen(const char *n) { return bnr_seen(&g_emitter.g_bnr_vname, &g_emitter.g_bnr_nm, n, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int var_assigned_real_static(IR_graph_t *g, const char *name, int depth) {
    if (!g || !g->all || !name || depth > 8) return 0;
    if (bnr_name_seen(name)) return 0;
    for (int i = 0; i < g->n; i++) {
        IR_t *m = g->all[i];
        if (!m || m->op != IR_ASSIGN || !IR_LIT(m).sval || strcmp(IR_LIT(m).sval, name)) continue;
        IR_t *rhs = (m->n_operands > 0) ? m->operands[0] : bb_child0(m);
        if (rhs && binop_operand_real_static(g, rhs, depth + 1)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int binop_operand_real_static(IR_graph_t *g, IR_t *o, int depth) {
    if (!o || depth > 8) return 0;
    if (bnr_node_seen(o)) return 0;
    if (o->op == IR_LIT_REAL) return 1;
    if (o->op == IR_VAR && IR_LIT(o).sval) return var_assigned_real_static(g, IR_LIT(o).sval, depth);
    if (o->op == IR_BINOP) {
        IR_t *c0 = bb_child0(o), *c1 = bb_child1(o);
        return binop_operand_real_static(g, c0, depth + 1) || binop_operand_real_static(g, c1, depth + 1);
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int binop_is_num_real(IR_graph_t *g, IR_t *nd) {
    if (!nd) return 0;
    int64_t op = IR_LIT(nd).ival;
    if (op == BINOP_POW || op == BINOP_POW_PROMOTE) return 1;
    int is_num = (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD || (op >= BINOP_LT && op <= BINOP_NE));
    if (!is_num) return 0;
    bnr_reset();
    int r = binop_operand_real_static(g, bb_child0(nd), 0); if (!r) { bnr_reset(); r = binop_operand_real_static(g, bb_child1(nd), 0); }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void nidx_build(IR_t **nodes, int n) {
    size_t want = 16; while (want < (size_t)n * 2 + 2) want <<= 1;
    if (want > g_emitter.g_nidx_cap) { ct_drop(g_emitter.g_nidx_keys); ct_drop(g_emitter.g_nidx_vals); g_emitter.g_nidx_keys = (IR_t **)ct_alloc(want * sizeof(IR_t *));
        g_emitter.g_nidx_vals = (int *)ct_alloc(want * sizeof(int)); g_emitter.g_nidx_cap = want; }
    memset(g_emitter.g_nidx_keys, 0, g_emitter.g_nidx_cap * sizeof(IR_t *));
    for (int i = 0; i < n; i++) { IR_t *k = nodes[i]; if (!k) continue; size_t h = ((size_t)(uintptr_t)k >> 4) & (g_emitter.g_nidx_cap - 1);
        while (g_emitter.g_nidx_keys[h] && g_emitter.g_nidx_keys[h] != k) h = (h + 1) & (g_emitter.g_nidx_cap - 1); if (!g_emitter.g_nidx_keys[h]) { g_emitter.g_nidx_keys[h] = k;
            g_emitter.g_nidx_vals[h] = i; } }
    g_emitter.g_nidx_nodes = nodes; g_emitter.g_nidx_n = n;
}
inline int nidx(IR_t **nodes, int n, const IR_t *e) {
    if (!e) return -1;
    if (nodes != g_emitter.g_nidx_nodes || n != g_emitter.g_nidx_n) { int k = 0; while (k < n) { if (nodes[k] == e) return k; k++; } return -1; }
    size_t h = ((size_t)(uintptr_t)e >> 4) & (g_emitter.g_nidx_cap - 1); while (g_emitter.g_nidx_keys[h]) { if (g_emitter.g_nidx_keys[h] == e) return g_emitter.g_nidx_vals[h];
        h = (h + 1) & (g_emitter.g_nidx_cap - 1); } return -1;
}
static int to_inner_gen_operand_k(IR_t *gi, IR_t **nodes, int n) {
    int bk = -1;
    if (gi->op != IR_TO && gi->op != IR_TO_BY) return -1;
    for (int oi = 0; oi < gi->n_operands; oi++) { int k = nidx(nodes, n, (IR_t *)gi->operands[oi]); if (k >= 0 && (ir_is_generator_kind(nodes[k]->op) && k > bk)) bk = k; }
    return bk;
}
extern "C" int icn_builtin_is_known(const char *);
extern "C" int icn_builtin_is_generator(const char *);
extern "C" int rt_proc_entry_pending(const char *);
extern "C++" void * dop_direct_fp(const char * fn, int64_t narg, const char ** sym);
extern "C" int dop_direct_leaf_known(const char * fn, int narg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RK_BOOL_ROUTE(a0) (((a0) && (a0)->op == IR_BINOP && IR_LIT(a0).ival >= BINOP_LT && IR_LIT(a0).ival <= BINOP_NE) ? CALL_ROUTE_RK_BOOL_RELOP : CALL_ROUTE_RK_BOOL_SLOT)
int bb_call_route_classify(IR_t * nd) {
    int r = nd ? lower_call_route(nd) : CALL_ROUTE_FATAL;
    return r == CALL_ROUTE_RK_BOOL ? RK_BOOL_ROUTE(ir_call_arg(nd, 0)) : r;
}
void call_prepare(IR_t * nd);
static void call_fc_fill(IR_t * nd) {
    extern int fc_call_active(const IR_t *); extern long fc_vwpop(const IR_t *); extern int zls_off(const IR_t *);
    g_emit.op_seal = nd->seal;
    if (fc_call_active(nd)) { g_emit.op_fc_wbytes = 16; g_emit.op_fc_base = zls_off(nd); long _w = fc_vwpop(nd); if (_w > 0) g_emit.op_wpop += (int)_w; }
}
static void call_box_emit(std::string (*box)(void)) {
    int sw = g_emit.op_wpop, sz = g_emit.op_zgpop;
    if (g_emit.op_call_sb_exit && !(g_emit_cfg && g_emit_cfg->spine_cells)) { g_emit.op_wpop = 0; g_emit.op_zgpop = 0; }
    bb_emit_x86(box());
    g_emit.op_wpop = sw; g_emit.op_zgpop = sz;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "walk_bb_flat"); abort(); }
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
static int walk_bb_node_inner(IR_t * nd, FILE * out);
extern "C" int bbprof_on(void);
extern "C" void bbprof_record(int nid, int kind, int uid, void *lo, void *hi);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int walk_bb_node(IR_t * nd, FILE * out) {
    { if (g_emitter.walk_bb_node__zp < 0) { const char * e = getenv("SCRIP_ZPROBE"); g_emitter.walk_bb_node__zp = (e && *e == '1') ? 1 : 0; }
      if (g_emitter.walk_bb_node__zp && nd) { extern int fc_geom(const IR_t *, long *); extern long zw_node_k(const IR_t *); long _d; long _k = fc_geom(nd, &_d) ? -1 : zw_node_k(nd);
          fprintf(stderr, "[ZPROBE] nest=%d nid=%d op=%d k=%ld\n", g_emitter.g_zprobe_depth, bb_node_id(nd), (int)nd->op, _k);
          g_emitter.g_zprobe_depth++; int _rc = walk_bb_node_inner(nd, out); g_emitter.g_zprobe_depth--; return _rc; } }
    if (!(nd && MEDIUM_BINARY && bbprof_on())) return walk_bb_node_inner(nd, out);
    int lo = bb_emit_pos;
    int rc = walk_bb_node_inner(nd, out);
    if (bb_emit_pos > lo) bbprof_record(bb_node_id(nd), (int)nd->op, g_emit.x86_uid, (void *)(bb_emit_buf + lo), (void *)(bb_emit_buf + bb_emit_pos));
    return rc;
}
static int drive_value_slot(IR_t *nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_sep_on(void) { if (g_emitter.emit_sep_on__sep < 0) { const char *e = getenv("SCRIP_ASM_SEP"); g_emitter.emit_sep_on__sep = (e && *e == '0') ? 0 : 1;
    } return g_emitter.emit_sep_on__sep; }
static void emit_sep_rule(char ch) { if (emit_sep_on()) bb_emit_x86(x86("commentrule", std::string(119, ch))); }
extern "C" void emit_sep_rule_c(char ch) { emit_sep_rule(ch); }
static int zw_nid_listed(const char * e, int nid) { if (!e || !*e) return 0; const char * p = e; while (*p) { long v = strtol(p, (char **)&p, 10); if ((int)v == nid) return 1;
    while (*p && *p != ',') p++; if (*p) p++; } return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cap_ok_at(IR_t * base, int bi) { for (int _j = 0; _j < g_emit_cfg->n; _j++) { IR_t * _o = g_emit_cfg->all[_j]; if (_o && _o->op == IR_MATCH_ARBNO && _o->n_operands >= 3) { int _a = -1,
    _b = -1; for (int _k = 0; _k < g_emit_cfg->n; _k++) { if (g_emit_cfg->all[_k] == _o->operands[1]) _a = _k; if (g_emit_cfg->all[_k] == _o->operands[2]) _b = _k; } if (_a > _b) { int _t = _a;
    _a = _b; _b = _t; } if (_a >= 0 && bi >= _a && bi <= _b) return 0; } } (void)base; return 1; }
static const char * sr3_gamma_label(const IR_t * g) { if (g_emitter.sr3_gamma_label__off < 0) { const char * e = getenv("SCRIP_SR3_FIX"); g_emitter.sr3_gamma_label__off = (e && *e == '0') ? 1 : 0;
    } if (getenv("SCRIP_SR3_CENSUS")) fprintf(stderr, "SR3 gamma op=%d\n", (int)g->op); if (!g_emitter.sr3_gamma_label__off && g->op != IR_GOTO_DEFERRED) return (const char *)0; return IR_LIT(g).sval;
        }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_dwarf_loc_on(void) { const char * e = getenv("SCRIP_DWARF_LOC"); return (e && e[0] == '0') ? 0 : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int walk_bb_node_inner(IR_t * nd, FILE * out) {
    { extern int zls_result_live(const IR_t *); g_emit.op_res_live = zls_result_live(nd); }
    { if (g_emitter.walk_bb_node_inner__ba < 0) { const char * e = getenv("SCRIP_BB_ALLOC"); g_emitter.walk_bb_node_inner__ba = (e && *e == '0') ? 0 : 1;
        g_emitter.walk_bb_node_inner__bo = getenv("SCRIP_BB_ONLY");
        g_emitter.walk_bb_node_inner__bs = getenv("SCRIP_BB_SKIP"); }
      int _spine =
          (nd->op == IR_BINOP || nd->op == IR_ASSIGN || nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_ATOM || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET ||
          nd->op == IR_LIT_NAME || nd->op == IR_VAR || nd->op == IR_CMP_TEST || nd->op == IR_COERCE_NUMERIC || nd->op == IR_IDENT || nd->op == IR_DIFFER);
      { if (g_emitter.walk_bb_node_inner__all < 0) { const char * e = getenv("SCRIP_BB_ALLOC_ALL"); g_emitter.walk_bb_node_inner__all = (e && *e == '1') ? 1 : 0; }
      (void)g_emitter.walk_bb_node_inner__ba; (void)g_emitter.walk_bb_node_inner__bo; (void)g_emitter.walk_bb_node_inner__bs; (void)g_emitter.walk_bb_node_inner__all; (void)_spine;
      { extern long zw_carve_k(const IR_t *); extern int zc_nofc(void); long _k = zc_nofc() ? 0 : zw_carve_k(nd);
        if (_k > 0) g_emit.op_fc_bytes = _k; } } }
    g_emit.op_zdepth = x86_fc_on() ? (int)g_emit.op_fc_bytes : 0; emit_gc_rsp_reset();
    { extern int zdp_alpha(const IR_t *); extern int zdp_beta(const IR_t *); extern int zdp_known_alpha(const IR_t *); extern int zdp_known_beta(const IR_t *); extern int zdp_mode(void);
      g_emit.op_zdp_ad = (nd && zdp_mode() && zdp_known_alpha(nd)) ? zdp_alpha(nd) : -1;
      g_emit.op_zdp_bd = (nd && zdp_mode() && zdp_known_beta(nd)) ? zdp_beta(nd) : -1;
      g_emit.op_zdp_rbp = (nd && (nd->op == IR_MATCH_BEGIN || nd->op == IR_MATCH_DEFER)) ? 1 : 0; }
    g_emit.op_pair_rejoin = (nd && nd->op == IR_MATCH_ALTERNATE) ? 1 : 0;
    g_emit.op_stmt_dyn = g_emitter.g_arbk16_stmt; if (nd) { if (nd->op == IR_STATEMENT_BEGIN) { g_emitter.g_arbk16_stmt = 0; g_emit.op_stmt_dyn = 0;
        } else if (nd->op == IR_STATEMENT_END || nd->op == IR_STATEMENT) g_emitter.g_arbk16_stmt = 0; }
    g_emit.op_zcp_src = -1; g_emit.op_zcp_dst = -1; g_emit.op_xrec = 0;
    g_emit.op_zres = 0; g_emit.op_zgpop = 0; g_emit.op_zbgrow = 0; g_emit.op_wsteal = 0; g_emit.op_ztail = 0; g_emit.op_zpat = 0; g_emit.op_zfc = 0; g_emit.op_xf_off = -1; g_emit.op_zdef = 1;
        g_emit.op_zcut = -1;
        zd_ops_reserve(nd ? nd->n_operands + 1 : 0);
        for (int _zi = 0; _zi < g_emit.op_zcap; _zi++) { g_emit.op_zread[_zi] = 0; g_emit.op_zkind[_zi] = -1; g_emit.op_zread_xf[_zi] = -1; g_emit.op_zread_fr[_zi] = -1; g_emit.op_zslot[_zi] = -1; }
    g_emit.op_zslot_n = 0; g_emit.op_res_alias = -1;
    { int _ri = nd ? zrow_of(nd) : -1; g_emit.op_zrow = _ri;
        if (_ri >= 0) { const zeta_plan_t * _zr = g_emitter.g_zd_rec_p; g_emit.op_xf_off = _zr->home[_ri];
            for (int _zi = 0; _zi <= nd->n_operands && _zi < g_emit.op_zcap; _zi++) { const zrd_t * _rd = planner_rec_read(_ri, _zi); if (_rd) g_emit.op_zread_xf[_zi] = _rd->xf; } }
        else if (nd) { int _xn = zd_nops(nd); g_emit.op_xf_off = planner_frame_slot(g_emit_cfg, nd);
            for (int _zi = 0; _zi < _xn && _zi < nd->n_operands; _zi++) { IR_t * _po = nd->operands[_zi], * _pv = zd_value_producer(_po); g_emit.op_zread_xf[_zi] = planner_frame_slot(g_emit_cfg, _po);
                if (g_emit.op_zread_xf[_zi] == -1 && _pv != _po) g_emit.op_zread_xf[_zi] = planner_frame_slot(g_emit_cfg, _pv); }
            if (zd_scan_subject(nd) && _xn == nd->n_operands && _xn < g_emit.op_zcap) g_emit.op_zread_xf[_xn] = planner_frame_slot(g_emit_cfg, zd_scan_subject(nd)); } }
    g_emit.op_suspend_stmt_uclaim = 0;
    if (g_emitter.g_zd_stage) { const zeta_plan_t * _zr = g_emitter.g_zd_rec_p; int _ri = g_emit.op_zrow; g_emit.op_suspend_stmt_uclaim = g_emitter.g_zd_suspend_uclaim;
        g_emitter.g_zd_suspend_uclaim = 0;
        if (_ri < 0) { fprintf(stderr, "FATAL emitter: staged node %s has no plan row (ARCH-THREE-ZETAS-EMISSION 6.3: the row is the emitter's one input)\n", nd ? bb_op_name(nd->op) : "NONE");
            abort(); }
        if (_zr->grec_off[_ri] != -1) { g_emit.op_xrec |= 4; g_emit.op_grec = _zr->grec_off[_ri]; g_emit.op_grec_pop = _zr->grec_res[_ri]; }
        if (_zr->wrec_off[_ri] != -1) { g_emit.op_xrec |= 8; g_emit.op_wrec = _zr->wrec_off[_ri]; g_emit.op_wrec_pop = _zr->wrec_res[_ri]; }
        if (g_emitter.g_zd_cpread && _zr->cpa_dx[_ri] != -1) { g_emit.op_xrec |= 2; g_emit.op_zcp_dxf = _zr->cpa_dx[_ri]; }
        if (g_emitter.g_zd_cpread && _zr->cpa_sx[_ri] != -1) { g_emit.op_xrec |= 1; g_emit.op_zcp_sxf = _zr->cpa_sx[_ri]; }
        if (g_emitter.g_zd_arm) { g_emit.op_zcp_src = _zr->cpa_s[_ri]; g_emit.op_zcp_dst = _zr->cpa_d[_ri];
            g_emit.op_zres = 1; g_emit.op_fc_bytes = g_emitter.g_zd_k; g_emit.op_fc_base = -1; g_emit.op_zdepth = g_emitter.g_zd_k + _zr->under[_ri];
            if (g_emitter.g_zd_rdread) for (int _zi = 0; _zi <= nd->n_operands && _zi < g_emit.op_zcap; _zi++) { const zrd_t * _rd = planner_rec_read(_ri, _zi); if (!_rd) continue;
                g_emit.op_zread[_zi] = _rd->dist; g_emit.op_zkind[_zi] = _rd->kind; g_emit.op_zread_fr[_zi] = _rd->fr; }
            g_emit.op_ztail = _zr->tail[_ri]; g_emit.op_zpat = _zr->pat[_ri]; g_emit.op_zfc = _zr->fc[_ri];
            if (g_emit_cfg) { int _xn = zd_nops(nd); g_emit.op_zslot_n = (_xn < nd->n_operands) ? _xn : nd->n_operands;
                for (int _zi = 0; _zi < g_emit.op_zslot_n && _zi < g_emit.op_zcap; _zi++) { const zrd_t * _rd = planner_rec_read(_ri, _zi); g_emit.op_zslot[_zi] = _rd ? _rd->key : -1; }
                if (zd_scan_subject(nd) && g_emit.op_zslot_n == nd->n_operands && g_emit.op_zslot_n < g_emit.op_zcap) { int _zi = g_emit.op_zslot_n++; const zrd_t * _rd = planner_rec_read(_ri, _zi);
                    g_emit.op_zslot[_zi] = _rd ? _rd->key : -1; }
                g_emit.op_res_alias = _zr->alias[_ri];
                if (xf_diag_on()) { fprintf(stderr, "[ALIAS] %s res=%d nops=%d:", bb_op_name(nd->op), g_emit.op_res_alias, g_emit.op_zslot_n);
                    for (int _zi = 0; _zi < g_emit.op_zslot_n && _zi < g_emit.op_zcap; _zi++) { IR_t * _po = nd->operands[_zi];
                        fprintf(stderr, " [%d %s slot=%d on=%d k=%d zread=%d fr=%d xf=%d]", _zi, _po ? bb_op_name(_po->op) : "-", g_emit.op_zslot[_zi], _po ? zd_plan_on_of(_po) : -1,
                            _po ? zd_k_own(_po) : -1, g_emit.op_zread[_zi], g_emit.op_zread_fr[_zi], g_emit.op_zread_xf[_zi]); } fprintf(stderr, "\n"); } } }
        g_emit.op_zgpop = ((g_emit.flat_jmp_entry && g_emit.flat_pat) || g_emitter.g_zd_fence_exit) ? 0 : g_emitter.g_zd_gpop; g_emit.op_wpop += g_emitter.g_zd_wpop;
            g_emitter.g_zd_fence_exit = 0;
        g_emit.op_zbgrow = g_emitter.g_zd_bgrow ? g_emit.op_zgpop : 0; g_emitter.g_zd_bgrow = 0;
        if (g_emitter.zd_plan__dg)
            fprintf(stderr, "[ZD-STAGE] op=%d gpop=%d wpop=%d abs=%d k=%d fc=%ld\n", (int)nd->op, g_emit.op_zgpop, g_emit.op_wpop, g_emitter.g_zd_abs, g_emitter.g_zd_k, (long)g_emit.op_fc_bytes);
            g_emit.op_wsteal = g_emitter.g_zd_wsteal; g_emitter.g_zd_stage = 0; }
    extern void bb_prepare_capture_arbno(IR_t *nd, int imm);
    extern void bb_prepare(IR_t *nd);
    extern int bb_slot_get(IR_t *nd);
    if (!nd) return 1;
    g_emit.node = nd; g_emit.op_pat_static = nd->pat_static;
    g_emit.op_zf_unpinned = 1;
    g_emit.op_zabs = g_emitter.g_zd_abs; g_emit.op_zsticky = zst_s_of(nd); g_emit.op_zk_own = g_emit.op_zres ? g_emitter.g_zd_k - g_emit.op_zsticky : 0; g_emit.op_zdef = g_emitter.g_zd_def;
        g_emit.op_zcut = g_emitter.g_zd_cutd;
    if (g_emit.op_zres && !zrow_under(g_emit.op_zrow)) g_emit.op_zdepth = g_emitter.g_zd_abs;
    emit_set_sink(out);
    g_emit.sid = 0;
    g_emit.nid = bb_node_id(nd);
    g_emit.x86_uid = g_flat_node_id++;
    { const char * _k = flat_label_kind_of(nd); if (!_k) _k = "(null)"; size_t _n = strlen(_k); cv_reserve(&g_emit.x86_uid_kind_buf, 1, (uint64_t)_n + 1, "x86_uid_kind_buf");
        memcpy(g_emit.x86_uid_kind_buf.p, _k, _n + 1); }
    g_emit.x86_uid_kind = (const char *)g_emit.x86_uid_kind_buf.p;
    g_emit.frame_region = g_emit_cfg ? ((32 + g_emit_cfg->jcon_value_region + 15) & ~15) : 0;
    g_emit.op_strict = nd->strict;
    g_emit.op_sval =
        (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_VAR_FRAME || nd->op == IR_ASSIGN_FRAME || nd->op == IR_ASSIGN || nd->op == IR_LIT_STRING || nd->op == IR_LIT_ATOM ||
        nd->op == IR_LIT_CHARSET || nd->op == IR_LIT_NAME
                       || nd->op == IR_KW_ICON || nd->op == IR_KW_ICON_GEN || nd->op == IR_KW_SNOBOL4 || nd->op == IR_KW_ASSIGN || nd->op == IR_KW_ASSIGN_SNOBOL4 || nd->op == IR_REV_SWAP ||
                           nd->op == IR_FIELD_GET || nd->op == IR_FIELD_VAR || nd->op == IR_SUBSCRIPT || nd->op == IR_ASSIGN_VAR || nd->op == IR_ITERATE
                       || nd->op == IR_MATCH_ASSIGN_COND || nd->op == IR_MATCH_ASSIGN_SAVE || nd->op == IR_MATCH_ASSIGN_IMM || nd->op == IR_MATCH_LIT || nd->op == IR_MATCH_ANY ||
                           nd->op == IR_MATCH_NOTANY || nd->op == IR_MATCH_SPAN
                       || nd->op == IR_MATCH_BREAK || nd->op == IR_MATCH_BREAKX || nd->op == IR_MATCH_DEFER || nd->op == IR_MATCH_ATP || nd->op == IR_MATCH_REPLACE || nd->op == IR_GOTO_DEFERRED
                       || nd->op == IR_NULLTEST_VAR || nd->op == IR_PROC_GEN || nd->op == IR_PROC_VALUE || ir_norm_call_kind(nd->op) == IR_CALL)
                    ? IR_LIT(nd).sval : (const char *)0;
    { extern int g_gva_active; extern int gva_index_of(const char *);
        int nm_op = (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_ASSIGN || nd->op == IR_MATCH_ASSIGN_IMM || nd->op == IR_MATCH_ASSIGN_COND);
      int defer_op = ((nd->op == IR_MATCH_DEFER) && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '*');
      g_emit.op_gva_k = (g_gva_active && (nm_op || defer_op) && IR_LIT(nd).sval && !graph_has_local(g_emit_cfg, IR_LIT(nd).sval)) ? gva_index_of(IR_LIT(nd).sval) : -1; }
    { extern int g_proc_direct_active; extern int proc_slot_of(const char *); extern int proc_direct_eligible(const char *); int nm_op = (ir_norm_call_kind(nd->op) == IR_CALL);
      g_emit.op_proc_k = (g_proc_direct_active && nm_op && IR_LIT(nd).sval && proc_direct_eligible(IR_LIT(nd).sval)) ? proc_slot_of(IR_LIT(nd).sval) : -1; }
    if (nd->op == IR_STATEMENT_BEGIN || nd->op == IR_STATEMENT_END || nd->op == IR_STATEMENT || (nd->op == IR_STMT_MARK && nd->seal != IR_SEAL_STMT_HOOK && nd->seal != IR_SEAL_LABEL_HOOK))
        { int32_t _stno = (int32_t)IR_LIT(nd).ival;
        if (_stno != g_emit.stno_last) { emit_stno_mark(_stno); g_emit.stno_last = _stno; } g_emit.op_stno = _stno; }
    g_emit.op_ival = (ir_norm_call_kind(nd->op) == IR_CALL || nd->op == IR_PROC_GEN) ? (int64_t)nd->n_operands : (nd->op == IR_GOTO_DEFERRED ? (int64_t)nd->seal : IR_LIT(nd).ival);
    if (nd->op == IR_DEFINE && ir_define_sr_citizen(nd)) { int64_t r = (int64_t)zd_sr_role(nd); g_emit.op_ival = r; g_emit.op_sval = r ? (const char *)0 : IR_LIT(nd).sval; }
    g_emit.op_node_kind = (int)nd->op;
    g_emit.op_dval = IR_LIT(nd).dval;
    g_emit.op_counter = 0;
    IR_t *op_a = (nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
    g_emit.op_a_sval = op_a ? IR_LIT(op_a).sval : (const char *)0;
    g_emit.op_a_node_kind = op_a ? (int)ir_norm_call_kind(op_a->op) : -1;
    g_emit.op_a_slot = (op_a != (IR_t *)0) ? bb_slot_get(op_a) : -1;
    if (nd->op == IR_MATCH_VALUE && op_a) { int _z = nd_slot(op_a); if (_z >= 0) g_emit.op_a_slot = _z; }
    if (g_emit.op_a_slot < 0 && op_a) { int _z = nd_slot(op_a); if (_z >= 0) g_emit.op_a_slot = _z; }
    g_emit.op_a_counter = 0;
    g_emit.op_a_ival_sg = op_a ? IR_LIT(op_a).ival : 0;
    g_emit.op_a_dval = op_a ? IR_LIT(op_a).dval : 0;
    { extern int arith_emits_descr(IR_t *nd); g_emit.op_a_descr = arith_emits_descr(op_a) ? 1 : 0; }
    { if (g_emitter.walk_bb_node_inner_bm < 0) { const char *e = getenv("SCRIP_BLOB_MAP"); g_emitter.walk_bb_node_inner_bm = (e && *e == '1') ? 1 : 0;
        } if (g_emitter.walk_bb_node_inner_bm && MEDIUM_BINARY) { extern const char *bb_op_name(IR_e);
        fprintf(stderr, "BLOBBOX %p %s n%d\n", (void *)(bb_emit_buf + bb_emit_pos), bb_op_name(nd->op), g_emit.x86_uid); } }
    { if (g_emitter.walk_bb_node_inner__sc < 0) { const char *e = getenv("SCRIP_SRC_COMMENT"); g_emitter.walk_bb_node_inner__sc = (e && *e == '0') ? 0 : 1;
        } g_emit.op_src = g_emitter.walk_bb_node_inner__sc ? bb_src_of(nd) : (const char *)0; }
    { if (g_emit.op_src && *g_emit.op_src) emit_sep_rule('=');
      if (g_emit.op_src) { std::string _c; const char * p = g_emit.op_src; while (*p) { const char * e2 = p; while (*e2 && *e2 != '\n') e2++; _c += x86("srccomment", std::string(p, (size_t)(e2 - p)));
          p = *e2 ? e2 + 1 : e2; } bb_emit_x86(_c); }
      emit_sep_rule('-'); }
    { if (g_emitter.walk_bb_node_inner__dl < 0) g_emitter.walk_bb_node_inner__dl = emit_dwarf_loc_on();
      int _raw = bb_line_of(nd); int _ln = _raw < 0 ? -_raw : _raw;
      if (_ln > 0) g_emit.op_line = _ln;
      if (g_emitter.walk_bb_node_inner__dl && _ln > 0) { const char * _sf = stmt_src_get_file(); int _nl = stmt_src_nlines();
        if (_sf && *_sf) bb_emit_x86(x86("loc", _sf, _ln, (_raw > 0 && _nl > 0 && _ln <= _nl) ? 1 : 2)); } }
    bb_classify_node(nd);
    if (emit_floater_kind(nd) == 3) { bb_emit_x86(bb_nreturn_mark()); return 0; }
    switch (nd->op) {
    case IR_LIT_INTEGER:
    case IR_LIT_STRING:
    case IR_LIT_ATOM:
    case IR_LIT_CHARSET:
    case IR_LIT_NAME:
    case IR_LIT_REAL: { g_emit.op_seal = nd->seal; { long fck; if (!g_emit.op_zres && fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off;
        } } bb_emit_x86(bb_lit_scalar()); } return 0;
    case IR_KW_ICON:
    case IR_KW_ICON_GEN: g_emit.op_var_form = nd->pat_static; g_emit.op_name2 = kw_bare(g_emit.op_sval); bb_emit_x86(bb_keyword_icon()); g_emit.op_var_form = 0; return 0;
    case IR_KW_SNOBOL4: kw_snobol4_prepare(); bb_emit_x86(bb_keyword_snobol4()); return 0;
    case IR_KW_ASSIGN: g_emit.op_name2 = kw_bare(g_emit.op_sval); bb_emit_x86(bb_keyword_assign()); return 0;
    case IR_KW_ASSIGN_SNOBOL4: g_emit.op_imm_a = g_emit.op_sval ? rt_kw_index(g_emit.op_sval) : -1; bb_emit_x86(bb_keyword_assign_snobol4()); return 0;
    case IR_VAR: { extern int is_global(const char *);
        const char * _vn = IR_LIT(nd).sval;
        if (_vn && _vn[0] == '&') { g_emit.op_name2 = kw_bare(g_emit.op_sval); bb_emit_x86(bb_keyword_icon()); }
        else if (_vn && (is_global(_vn) && !graph_has_local(g_emit_cfg, _vn))) { { long fck; if (!g_emit.op_zres && fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off;
            } } { extern long fc_vwpop(const IR_t *); long _w = fc_vwpop(nd); if (_w > 0 && !g_emit.op_zres) g_emit.op_wpop += (int)_w; } bb_emit_x86(bb_var_global()); }
        else bb_emit_x86(bb_var()); } return 0;
    case IR_VAR_FRAME: { extern void frame_display_prepare(IR_t *); frame_display_prepare(nd); } bb_emit_x86(bb_var_frame()); return 0;
    case IR_ASSIGN_FRAME: { IR_t *_fr = (nd->n_operands > 1) ? nd->operands[1] : (IR_t *)0;
        g_emit.op_a_slot = _fr ? bb_slot_get(_fr) : -1;
        if (g_emit.op_a_slot < 0 && _fr) { int _z = nd_slot(_fr); if (_z >= 0) g_emit.op_a_slot = _z; }
        { extern void frame_display_prepare(IR_t *); frame_display_prepare(nd); } bb_emit_x86(bb_assign_frame()); } return 0;
    case IR_VAR_REF: { extern int is_global(const char *); const char * _rn = IR_LIT(nd).sval; g_emit.op_var_named = nd->pat_static;
        if (nd->n_operands >= 1 && nd->operands[0] && nd->operands[0]->op == IR_LIT_NAME) { bb_prepare(nd); { extern void frame_display_prepare(IR_t *); frame_display_prepare(nd);
            } bb_emit_x86(bb_var_ref_frame()); return 0; }
        if (_rn && is_global(_rn) && !graph_has_local(g_emit_cfg, _rn)) { g_emit.op_sa = -1; g_emit.op_gva_k = g_gva_active ? gva_index_of(_rn) : -1; }
        else if (_rn) { int _vo = bb_varslot_peek(_rn); g_emit.op_sa = _vo; g_emit.op_gva_k = -1; }
        bb_emit_x86(bb_var_ref()); } return 0;
    case IR_COERCE_STRING: { bb_prepare(nd); bb_emit_x86(bb_coerce_string()); } return 0;
    case IR_COERCE_INTEGER: { bb_prepare(nd); bb_emit_x86(bb_coerce_integer()); } return 0;
    case IR_COERCE_NUMERIC: { bb_prepare(nd);
        if (g_emit.op_zres && nd->n_operands == 1 && g_emit.op_zcap > 1) {
            g_emit.op_zread[1] = g_emit.op_zread[0];
            g_emit.op_zkind[1] = g_emit.op_zkind[0];
            g_emit.op_zread_fr[1] = g_emit.op_zread_fr[0];
            g_emit.op_zread_xf[1] = g_emit.op_zread_xf[0];
        }
        bb_emit_x86(bb_coerce_numeric()); } return 0;
    case IR_COERCE_REAL: { bb_prepare(nd); bb_emit_x86(bb_coerce_real()); } return 0;
    case IR_CMP_TEST: { bb_prepare(nd); bb_emit_x86(bb_cmp_test()); } return 0;
    case IR_IDENT: { bb_prepare(nd); bb_emit_x86(bb_ident()); } return 0;
    case IR_DIFFER: { bb_prepare(nd); bb_emit_x86(bb_differ()); } return 0;
    case IR_ASSIGN: {
        extern int is_global(const char *);
        const char * _an = IR_LIT(nd).sval;
        int _an_reassignable_builtin = _an && (!strcmp(_an, "write") || !strcmp(_an, "writes"));
        if (_an && ((is_global(_an) && !graph_has_local(g_emit_cfg, _an)) || _an_reassignable_builtin)) { bb_emit_x86(bb_assign_global()); return 0; }
        if (_an) { { if (g_emitter.walk_bb_node_inner__dd < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); g_emitter.walk_bb_node_inner__dd = (_e && _e[0] == '1') ? 1 : 0; }
                     if (g_emitter.walk_bb_node_inner__dd && !(g_emit.op_sb >= 0 && g_emit.op_off >= 0 && (g_emit.op_a_slot >= 0 || g_emit.op_a_node_kind == (int)IR_EXCISED))) {
                         IR_t *_ra = (nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
                         fprintf(stderr, "[ASSIGN-DIAG] var=%s sb=%d off=%d a_slot=%d a_kind=%d rhs_op=%s rhs_nops=%d\n", _an, g_emit.op_sb, g_emit.op_off, g_emit.op_a_slot, g_emit.op_a_node_kind,
                             _ra ? bb_op_name(_ra->op) : "NULL", _ra ? _ra->n_operands : -1); } }
                   bb_emit_x86(bb_assign_local()); return 0; }
        fprintf(out, "# [walk_bb_node: kind=%d unhandled]\n", (int)nd->op); return 1;
    }
    case IR_BINOP_TEST: bb_emit_x86(g_emit.op_zres ? bb_binop_relop_val() : bb_binop_relop()); return 0;
    case IR_BINOP_RELOP_VAL: bb_emit_x86(bb_binop_relop_val()); return 0;
    case IR_BINOP:
        { extern int fc_vbinop_active(const IR_t *); extern long fc_vwpop(const IR_t *); long _w = fc_vwpop(nd); if (_w > 0 && fc_vbinop_active(nd) && !g_emit.op_zres) g_emit.op_wpop += (int)_w; }
        switch (g_emit.op_binop_kind) {
        case BINOP_CAT_RELOP: bb_emit_x86(bb_binop_relop()); return 0;
        case BINOP_CAT_CONCAT: bb_emit_x86(bb_binop_concat_slot()); return 0;
        case BINOP_CAT_XREP: bb_emit_x86(bb_binop_xrep_slot()); return 0;
        case BINOP_CAT_ARITH:
        default: bb_emit_x86(bb_binop_arith()); return 0;
        }
    case IR_SUCCEED: bb_emit_x86(bb_succeed()); return 0;
    case IR_SUSPEND: { g_emit.op_activate_proc = IR_LIT(nd).sval; g_emit.op_var_form = nd->pat_static; bb_emit_x86(bb_suspend()); g_emit.op_var_form = 0; } return 0;
    case IR_TO: { bb_prepare(nd); g_emit.op_range_int_operands = ir_range_operands_must_be_integers(nd); g_emit.op_db_walk = ir_range_kind_is(nd, "db"); g_emit.op_to_trailed = ir_range_trailed(nd);
        bb_emit_x86(bb_to()); } return 0;
    case IR_MATCH_LEN: { bb_prepare(nd); { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
        g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : (const char *)0; } bb_emit_x86(bb_match_len()); } return 0;
    case IR_MATCH_LIT: { bb_prepare(nd); bb_emit_x86(bb_match_lit()); } return 0;
    case IR_MATCH_RKRUN: { bb_prepare(nd); g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_ival = rk_blob_bounds(IR_LIT(nd).sval); g_emit.op_off = planner_act_slot(nd); bb_emit_x86(bb_match_rkrun());
        } return 0;
    case IR_MATCH_RKCAPS: { bb_prepare(nd); g_emit.op_ival = IR_LIT(nd).ival; g_emit.op_off = planner_act_slot(nd);
        bb_emit_x86(bb_match_rkcaps()); } return 0;
    case IR_MATCH_RKCAPE: { bb_prepare(nd); g_emit.op_ival = IR_LIT(nd).ival; g_emit.op_off = planner_act_slot(nd);
        bb_emit_x86(bb_match_rkcape()); } return 0;
    case IR_MATCH_RKLAND: { bb_prepare(nd); g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_ival = rk_land_ngroups(nd); g_emit.op_off = planner_act_slot(nd); bb_emit_x86(bb_match_rkland()); } return 0;
    case IR_MATCH_ANY: { bb_prepare(nd); bb_emit_x86(bb_match_any()); } return 0;
    case IR_MATCH_NOTANY: { bb_prepare(nd); bb_emit_x86(bb_match_notany()); } return 0;
    case IR_MATCH_SPAN: { bb_prepare(nd); { long fck; if (fc_leaf_cell(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off;
        } } bb_emit_x86(bb_match_span()); } return 0;
    case IR_MATCH_BREAK: { bb_prepare(nd); { long fck; if (fc_leaf_cell(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off;
        } } bb_emit_x86(bb_match_break()); } return 0;
    case IR_MATCH_BREAKX: { bb_prepare(nd); { long fck; if (fc_leaf_cell(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off;
        } } bb_emit_x86(bb_match_breakx()); } return 0;
    case IR_MATCH_TAB: { bb_prepare(nd); { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
        g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : (const char *)0; } { long fck; if (fc_leaf_cell(nd, &fck)) { g_emit.op_fc_bytes = fck;
        g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_tab()); } return 0;
    case IR_MATCH_RTAB: { bb_prepare(nd); { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
        g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : (const char *)0; } { long fck; if (fc_leaf_cell(nd, &fck)) { g_emit.op_fc_bytes = fck;
        g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_rtab()); } return 0;
    case IR_MATCH_LAMBDA: { bb_prepare(nd); bb_emit_x86(bb_match_lambda()); } return 0;
    case IR_MATCH_POS: { bb_prepare(nd); { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
        g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : (const char *)0; } bb_emit_x86(bb_match_pos()); } return 0;
    case IR_MATCH_RPOS: { bb_prepare(nd); { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
        g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : (const char *)0; } bb_emit_x86(bb_match_rpos()); } return 0;
    case IR_MATCH_REM: { bb_prepare(nd); { long fck; if (fc_leaf_cell(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off;
        } } bb_emit_x86(bb_match_rem()); } return 0;
    case IR_MATCH_ATP: { bb_prepare(nd); bb_emit_x86(bb_match_atp()); } return 0;
    case IR_MATCH_ARB: { bb_prepare(nd); { long fck; if (fc_leaf_cell(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off;
        } } bb_emit_x86(bb_match_arb()); } return 0;
    case IR_MATCH_BAL: { bb_prepare(nd); { long fck; if (fc_leaf_cell(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off;
        } } bb_emit_x86(bb_match_bal()); } return 0;
    case IR_MATCH_DEFER: { bb_prepare(nd); g_emit.op_seal = nd->seal; g_emit.op_off = drive_value_slot(nd); g_emit.op_defer_leaf_susp = g_emit_cfg ? fc_tail_defer_susp_g(g_emit_cfg, nd) : -1;
        defer_prepare();
        bb_emit_x86(bb_match_defer()); } return 0;
    case IR_MATCH_VALUE: { bb_prepare(nd); bb_emit_x86(bb_match_value()); } return 0;
    case IR_MATCH_ARBNO: { bb_prepare(nd); g_emit.op_arbno_rbp = 0;
                                    g_emit.op_arbno_frame_off = planner_act_slot(nd); planner_act_window(nd, &g_emit.op_arbno_win_lo, &g_emit.op_arbno_win_bytes);
                                    int _pk = planner_body_live(g_emit_cfg, nd->n_operands > 2 ? nd->operands[1] : (IR_t *)0, nd->n_operands > 2 ? nd->operands[2] : (IR_t *)0, nd);
                                    extern int fc_tail_arbno(const IR_t *, int *, int *, int *, int *); extern int fc_tail_ncap(const IR_t *); int _fpb = 0, _fpl = 0, _osb = 0, _hdr = 0;
                                    int _tailc = (fc_tail_arbno(nd, &_fpb, &_fpl, &_osb, &_hdr)) ? 1 : 0;
                                    int _k16r = 0; { if (g_emitter.walk_bb_node_inner__k16e < 0) { const char * _e1 = getenv("SCRIP_ARBNO_K16");
                                        g_emitter.walk_bb_node_inner__k16e = (_e1 ? (atoi(_e1) != 0) : 1) ? 1 : 0;
                                        } if (g_emitter.walk_bb_node_inner__k16e && g_emit_cfg && nd->n_operands >= 3) { int _ni=-1;
                                            for(int _j=0;_j<g_emit_cfg->n;_j++) if(g_emit_cfg->all[_j]==nd){_ni=_j;break;} int _fr=0;
                                        if(_ni>=0) for(int _j=0;_j<g_emit_cfg->n&&!_fr;_j++){IR_t*_o=g_emit_cfg->all[_j]; if(_o&&_o!=nd&&_o->op==IR_MATCH_ARBNO&&_o->n_operands>=3){int _a=-1,_b=-1;
                                        for(int _k=0;_k<g_emit_cfg->n;_k++){if(g_emit_cfg->all[_k]==_o->operands[1])_a=_k;if(g_emit_cfg->all[_k]==_o->operands[2])_b=_k;} if(_a>_b){int _t=_a;_a=_b;
                                        _b=_t;} if(_a>=0&&_ni>=_a&&_ni<=_b)_fr=1;}} int _k0=1,_kk=0,_sq=1; { int _s0=-1,_s1=-1;
                                        for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_s0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_s1=_j;} if(_s0>_s1){int _t=_s0;
                                        _s0=_s1;_s1=_t;} if(_s0<0){_k0=0;_sq=0;} else for(int _j=_s0;_j<=_s1;_j++){IR_t*_m=g_emit_cfg->all[_j]; if(_m&&_m!=nd){int _mo2=(int)_m->op;
                                        if(_mo2==IR_MATCH_ALTERNATE||_mo2==IR_MATCH_ARBNO||(_mo2==IR_MATCH_FENCE1 || _mo2==IR_MATCH_FENCE0)||_mo2==IR_MATCH_DEFER||_mo2==IR_MATCH_VALUE||_mo2==IR_CALL||
                                        _mo2==IR_CALL_VALUE||_mo2==IR_DISJUNCTION||_mo2==IR_MATCH_ABORT)_sq=0; int _mk=zd_k(_m); if(_mk!=0)_k0=0; _kk+=_mk;}} } int _osv=0,_dmb=-2,_dme=-2;
                                        { int _mb=-1,_hi=g_emit_cfg->n; for(int _j=_ni;_j>=0;_j--){if(g_emit_cfg->all[_j]&&g_emit_cfg->all[_j]->op==IR_MATCH_BEGIN){_mb=_j;break;
                                        }} if(_mb>=0) for(int _j=_mb+1;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]&&g_emit_cfg->all[_j]->op==IR_MATCH_BEGIN){_hi=_j;break;}} if(_mb>=0){ int _s0=-1,
                                        _s1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_s0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_s1=_j;
                                        } if(_s0>_s1){int _t=_s0;_s0=_s1;_s1=_t;} for(int _j=_mb+1;_j<_hi;_j++){ if(_j>=_s0&&_j<=_s1) continue; IR_t*_m=g_emit_cfg->all[_j];
                                        if(_m&&_m->op==IR_MATCH_ASSIGN_SAVE)_osv=1; } } else _osv=1; _dmb=_mb; _dme=_hi; } if (_pk > 0 && g_emit.op_arbno_frame_off == -1) { _k16r = 1;
                                            g_emitter.g_arbk16_stmt = 1;
                                        g_emit.op_arbno_framed = 0; g_emit.op_arbno_body_k0 = 0; g_emit.op_arbno_body_kk = _pk; } {
                                        if(g_emitter.walk_bb_node_inner__ad<0){const char*_e=getenv("SCRIP_ARBNO_DIAG"); g_emitter.walk_bb_node_inner__ad=(_e&&*_e=='1')?1:0;
                                        } if(g_emitter.walk_bb_node_inner__ad)
                                        fprintf(stderr,"[ARBNO-K16] framed=%d k0=%d sq=%d kk=%d osv=%d mb=%d me=%d route=%s\n", _fr, _k0, _sq, _kk, _osv, _dmb, _dme, _k16r?"FRAMELESS_K":"legacy");
                                        } } }
                                    if (!_k16r && 0 && _tailc) { g_emit.op_tail = 1; g_emit.op_sb = _osb; g_emit.op_sa = _hdr; g_emit.op_tail_fpb = _fpb; g_emit.op_tail_fpl = _fpl;
                                        g_emit.op_tail_ncap = fc_tail_ncap(nd); g_emit.op_tail_seal = (nd->n_operands > 3 && nd->operands[3] == nd) ? 1 : 0; g_emit.op_tail_dfr = fc_tail_dfr(nd);
                                        { extern int fc_tail_fpr_rsp(const IR_t *); g_emit.op_tail_fpr_rsp = fc_tail_fpr_rsp(nd); } }
                                    else if (!_k16r) { int _mo = -1, _sp = 0; if (zls_arbno_geom(nd, &_mo, &_sp)) { g_emit.op_sa = _mo; int _chain = 1; g_emit.op_arbno_chain = _chain;
                                        g_emit.op_sb = (_sp + (_chain ? 24 : 16) + 15) & ~15; if (_chain) { g_emit.op_arbno_nzq = zls_arbno_zq(nd, &g_emit.op_arbno_zq);
                                        if (_chain && g_emit_cfg && nd->n_operands >= 3) { int _i0=-1,_i1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_i0=_j;
                                        if(g_emit_cfg->all[_j]==nd->operands[2])_i1=_j;} if(_i0>_i1){int _t=_i0;_i0=_i1;_i1=_t;} int _bha=0;
                                        for(int _j=_i0;_j<=_i1&&_j>=0&&!_bha;_j++) if(g_emit_cfg->all[_j]&&g_emit_cfg->all[_j]->op==IR_MATCH_ARBNO&&g_emit_cfg->all[_j]!=nd) _bha=1;
                                        g_emit.op_body_has_arbno=_bha; } }
                                           if (_chain && g_emit_cfg) { int _ni=-1; for(int _j=0;_j<g_emit_cfg->n;_j++) if(g_emit_cfg->all[_j]==nd){_ni=_j;break;} int _fr=0;
                                               if(_ni>=0) for(int _j=0;_j<g_emit_cfg->n&&!_fr;_j++){IR_t*_o=g_emit_cfg->all[_j]; if(_o&&_o!=nd&&_o->op==IR_MATCH_ARBNO&&_o->n_operands>=3){int _a=-1,
                                               _b=-1; for(int _k=0;_k<g_emit_cfg->n;_k++){if(g_emit_cfg->all[_k]==_o->operands[1])_a=_k;if(g_emit_cfg->all[_k]==_o->operands[2])_b=_k;
                                               } if(_a>_b){int _t=_a;_a=_b;_b=_t;} if(_a>=0&&_ni>=_a&&_ni<=_b)_fr=1;}} g_emit.op_arbno_framed=_fr; int _k0=1; { int _s0=-1,_s1=-1;
                                               for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_s0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_s1=_j;
                                               } if(_s0>_s1){int _t=_s0;_s0=_s1;_s1=_t;} if(_s0<0)_k0=0; else for(int _j=_s0;_j<=_s1&&_k0;_j++){IR_t*_m=g_emit_cfg->all[_j];
                                               if(_m&&_m!=nd){int _mo3=(int)_m->op;
                                               if(zd_k(_m)!=0||_mo3==IR_MATCH_DEFER||_mo3==IR_MATCH_ALTERNATE||_mo3==IR_MATCH_ARBNO||(_mo3==IR_MATCH_FENCE1 || _mo3==IR_MATCH_FENCE0)||
                                               _mo3==IR_MATCH_VALUE||_mo3==IR_CALL||_mo3==IR_CALL_VALUE||_mo3==IR_DISJUNCTION||_mo3==IR_MATCH_ABORT)_k0=0;}} } g_emit.op_arbno_body_k0=_k0; }
   { int _du=0; int _ds0=-1,_ds1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_ds0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_ds1=_j;} if(_ds0>_ds1){int _t=_ds0;
       _ds0=_ds1;_ds1=_t;} int _alt=0,_ref=0,_dfr=0,_arb=0,_altnest=0; if(_ds0>=0) for(int _j=_ds0;_j<=_ds1;_j++){IR_t*_m=g_emit_cfg->all[_j];
       if(_m&&_m!=nd&&(int)_m->op==IR_MATCH_DEFER&&!_m->pat_static)_du=1; if(!_m||_m==nd)continue; int _o=(int)_m->op; if(_o==IR_MATCH_ALTERNATE||_o==IR_DISJUNCTION){ if(_m->seal)_ref=1;
       else { _alt++; for(int _q=0;_q<_m->n_operands;_q++){IR_t*_a=_m->operands[_q]; if(_a&&((int)_a->op==IR_MATCH_ALTERNATE||(int)_a->op==IR_DISJUNCTION))_altnest=1;} } continue;
       } if(_o==IR_MATCH_DEFER){ extern int sn4_alt_carrier(void); if(sn4_alt_carrier()&&_m->seal!=1)_dfr++; else _ref=1; continue;} if(_o==IR_MATCH_ARBNO){ extern int sn4_arbno_tailbeta(void);
       if(sn4_arbno_tailbeta()&&_m->γ.node==nd)_arb++; else _ref=1; continue;
       } switch(_o){case IR_MATCH_LIT:case IR_MATCH_LEN:case IR_MATCH_ANY:case IR_MATCH_NOTANY:case IR_MATCH_POS:case IR_MATCH_RPOS:case IR_MATCH_TAB:case IR_MATCH_RTAB:case IR_MATCH_SPAN:
       case IR_MATCH_BREAK:case IR_MATCH_REM:break;default:_ref=1;break;} } int _fseal=(nd->n_operands>3&&nd->operands[3]==nd)?1:0; g_emit.op_tail_seal=_fseal;
       g_emit.op_arbno_body_defer_unsafe=_du; { int _bf=0; if(_ds0>=0) for(int _j=_ds0;_j<=_ds1;_j++){IR_t*_m=g_emit_cfg->all[_j];
       if(_m&&_m!=nd&&((int)_m->op==IR_MATCH_FENCE1||(int)_m->op==IR_MATCH_FENCE0))_bf=1;} g_emit.op_arbno_body_fence=_bf; } { extern int sn4_arbno_altsib(void);
       int _altok=sn4_arbno_altsib()?(_alt>=1&&!_altnest):(_alt==1);
       g_emit.op_arbno_body_actframe=(((_altok&&_dfr==0&&_arb==0)||(_alt==0&&_dfr==1&&_arb==0)||(_alt==0&&_dfr==0&&_arb==1))&&!_ref&&!_fseal)?0:1; } } } else { g_emit.op_sa = -1; g_emit.op_sb = -1;
       g_emit.op_arbno_chain = 0; } }
                                    if (g_emit.op_arbno_chain && g_emit.op_off >= 0 && !(nd->n_operands > 3 && nd->operands[3] == nd) && g_emit_cfg && nd->n_operands >= 3) {
                                         if (g_emitter.walk_bb_node_inner__dtl < 0) { const char * _e = getenv("SCRIP_ARBNO_LATCH"); g_emitter.walk_bb_node_inner__dtl = _e ? (atoi(_e) != 0) : 0; }
                                        if (g_emitter.walk_bb_node_inner__dtl) {
                                            int _i0 = -1, _i1 = -1; for (int _j = 0; _j < g_emit_cfg->n; _j++) { if (g_emit_cfg->all[_j] == nd->operands[1]) _i0 = _j;
                                                if (g_emit_cfg->all[_j] == nd->operands[2]) _i1 = _j; }
                                            if (_i0 > _i1) { int _t = _i0; _i0 = _i1; _i1 = _t; }
                                            IR_t * _dn = NULL; int _ok = (_i0 >= 0 && _i1 >= 0);
                                            for (int _j = _i0; _ok && _j <= _i1; _j++) { IR_t * _x = g_emit_cfg->all[_j]; if (!_x) continue;
                                                if (_x->op == IR_MATCH_DEFER) { if (_dn || _x->seal != 2) _ok = 0; else _dn = _x; }
                                                else if (_x->op == IR_LIT_STRING || _x->op == IR_GOTO) { }
                                                else _ok = 0; }
                                            const char * _pn = NULL;
                                            if (_ok && _dn) for (int _j = 0; _j < _dn->n_operands; _j++) { IR_t * _o = _dn->operands[_j];
                                                if (_o && _o->op == IR_LIT_STRING && IR_LIT(_o).sval && !strncmp(IR_LIT(_o).sval, "PAT$", 4)) { _pn = IR_LIT(_o).sval; break; } }
                                            int _susp = 0;
                                             if (g_emitter.walk_bb_node_inner__pk < 0) { const char * _e2 = getenv("SCRIP_ARBNO_PEEK"); g_emitter.walk_bb_node_inner__pk = _e2 ? (atoi(_e2) != 0) : 1; }
                                            if (getenv("SCRIP_DT_DIAG"))
                                                fprintf(stderr, "[DT] arbno: ok=%d pn=%s var=%s pro=%d uni=%d pk=%d\n", _ok, _pn ? _pn : "-", (_dn && IR_LIT(_dn).sval) ? IR_LIT(_dn).sval : "-",
                                                (_dn && IR_LIT(_dn).sval) ? sno_name_prologue_bound(IR_LIT(_dn).sval) : -1, _pn ? emit_patzeta_lookup(_pn, &_susp) : -1,
                                                    g_emitter.walk_bb_node_inner__pk);
                                            if (_ok && _dn && IR_LIT(_dn).sval && sno_name_prologue_bound(IR_LIT(_dn).sval) &&
                                                (g_emitter.walk_bb_node_inner__pk ? 1 : (_pn && emit_patzeta_lookup(_pn, &_susp) != 0)))
                                                { g_emit.op_arbno_dt = 1; g_emit.op_arbno_dt_susp = g_emitter.walk_bb_node_inner__pk ? 0 : _susp; }
                                        }
                                    }
                                    if (!_k16r) { g_emit.op_arbno_body_k0 = (_pk == 0) ? 1 : 0; g_emit.op_arbno_body_defer_unsafe = (_pk < 0) ? 1 : 0; }
                                    bb_emit_x86(bb_match_arbno()); } return 0;
    case IR_MATCH_BEGIN: { extern int fc_tail_head(const IR_t *); int _th = (fc_tail_head(nd)); if (fc_head_fp(nd) >= 0 || _th) { g_emit.op_fc_wbytes = 80; g_emit.op_fc_base = g_emit.op_off;
        } if (_th) g_emit.op_tail = 1; bb_emit_x86(bb_match_begin()); } return 0;
    case IR_MATCH_END: { IR_t * _hd = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0; extern int fc_tail_release(const IR_t *, int *); int _br = -1;
                                    { int _tr = _hd ? fc_tail_release(_hd, &_br) : 0; int _fp = _hd ? fc_head_fp(_hd) : -999;
                                      if (getenv("SCRIP_FCDISP_DIAG")) fprintf(stderr, "[FCDISP] tail_release=%d br=%d head_fp=%d\n", _tr, _br, _fp); }
                                    if (_hd && fc_tail_release(_hd, &_br)) { g_emit.op_tail = 1; g_emit.op_fc_disp = _br; }
                                    else g_emit.op_fc_disp = _hd ? fc_head_fp(_hd) : -1;
                                    { extern int fc_tail_head(const IR_t *); if (_hd && (fc_head_fp(_hd) >= 0 || fc_tail_head(_hd))) { g_emit.op_fc_wbytes = 80; g_emit.op_fc_base = g_emit.op_off; } }
                                    bb_emit_x86(bb_match_end()); } return 0;
    case IR_MATCH_REPLACE: { bb_emit_x86(bb_match_replace()); } return 0;
    case IR_MATCH_ASSIGN_COND: { bb_prepare(nd); g_emit.op_fc_disp = fc_cond_fp(nd); g_emit.op_cap_frame_off = planner_act_slot(nd);
        if (getenv("SCRIP_CAP_DIAG"))
        fprintf(stderr, "[CAP] COND nd=%p fc_disp=%d zres=%d zread0=%d\n", (void*)nd, g_emit.op_fc_disp, g_emit.op_zres, g_emit.op_zread[0]);
            bb_emit_x86(bb_match_capture()); } return 0;
    case IR_MATCH_ASSIGN_IMM: { bb_prepare(nd); g_emit.op_fc_disp = fc_cond_fp(nd); g_emit.op_cap_frame_off = planner_act_slot(nd);
        if (getenv("SCRIP_CAP_DIAG"))
        fprintf(stderr, "[CAP] IMM nd=%p fc_disp=%d off=%d pin=%d refine=%d deep=%d pat=%d\n", (void*)nd, g_emit.op_fc_disp, g_emit.op_off,
        emit_jmp_pin_legacy(), g_emit.flat_fb_refine, g_emit.flat_deep_arrival, g_emit.flat_pat); bb_emit_x86(bb_match_capture()); } return 0;
    case IR_MATCH_ASSIGN_SAVE: { bb_prepare(nd); g_emit.op_cap_frame_off = planner_act_slot(nd); { long fck;
        if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off; } } { extern int fc_save_active(const IR_t *);
        if (getenv("SCRIP_CAP_DIAG"))
        fprintf(stderr, "[CAP] SAVE nd=%p save_active=%d fc_bytes=%ld\n", (void*)nd, fc_save_active(nd), (long)g_emit.op_fc_bytes)
        ; } bb_emit_x86(bb_match_capture()); } return 0;
    case IR_MATCH_ALTERNATE: { { int _M = 0; int _ok = planner_alt_exit(g_emit_cfg, nd, &_M); g_emit.op_alt_M = _ok == 0 ? _M : -1; g_emit.op_alt_cell = _ok != 0 ? 1 : 0; int _na = nd->n_operands / 2;
            g_emit.op_alt_pad = xci_ints(&g_emitter.g_alt_pads, (uint32_t)(_na > 0 ? _na : 1));
                for (int _j = 0; _j < _na; _j++) g_emit.op_alt_pad[_j] = _ok == 0 ? _M - planner_alt_arm_live(g_emit_cfg, nd, _j) : 0; }
        g_emit.op_sa = 0; { extern const char * blob_alt_first(const IR_t *);
        g_emit.op_alt_first = blob_alt_first(nd); } bb_emit_x86(bb_match_alternate()); g_emit.op_alt_first = (const char *)0; } return 0;
    case IR_MATCH_FENCE0: { bb_prepare(nd); g_emit.op_fence0_release = fence0_release_bytes(nd); g_emit.op_fence0_floor = fence0_dyn_floor(nd);
                                     switch ((sno_fence_lit_e)IR_LIT(nd).ival) { case SNO_FENCE_LIT_BARE: bb_emit_x86(bb_match_fence0()); break;
                                         case SNO_FENCE_LIT_FLUSH: bb_emit_x86(bb_match_flush()); break;
                                     default:
                                         fprintf(stderr,
                                         "[EMIT] FATAL IR_MATCH_FENCE0 node %p carries literal %lld, which names neither the bare FENCE nor FLUSH (sno_fence_lit_e in IR.h) -- refused, not emitted as"
                                         " a plain FENCE\n", (void *)nd, (long long)IR_LIT(nd).ival); abort(); } } return 0;
    case IR_MATCH_FENCE1: { bb_prepare(nd);
        g_emit.op_fence_body_kk = planner_body_live(g_emit_cfg, nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0, nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0, nd);
        g_emit.op_fence_frame_off = planner_act_slot(nd); bb_emit_x86(bb_match_fence1()); } return 0;
    case IR_MATCH_ABORT: { bb_emit_x86(bb_match_abort()); } return 0;
    case IR_SCAN_ALTERNATE: { bb_emit_x86(bb_scan_alternate()); } return 0;
    case IR_TO_BY: { bb_prepare(nd); g_emit.op_range_int_operands = ir_range_operands_must_be_integers(nd); bb_emit_x86(bb_to_by()); } return 0;
    case IR_MAKE_LIST: bb_emit_x86(bb_make_list()); return 0;
    case IR_CONJUNCTION: bb_emit_x86(bb_conjunction()); return 0;
    case IR_GOTO: { if (g_emitter.walk_bb_node_inner__mgt < 0) g_emitter.walk_bb_node_inner__mgt = getenv("MONITOR_GOTO_TAP") ? 1 : 0; extern void emit_mon_label_tap(int32_t);
        if (g_runtime.abi.trace_budget != 0 && g_emitter.walk_bb_node_inner__mgt && g_emit.op_stno > 0) emit_mon_label_tap(g_emit.op_stno); } bb_emit_x86(bb_goto()); return 0;
    case IR_GOTO_DEFERRED: bb_emit_x86(bb_goto_deferred()); return 0;
    case IR_DEFINE: {
        if (ir_define_sr_citizen(nd)) { extern int fc_call_active(const IR_t *);
            if (g_emit.op_ival == 4 && nd->n_operands >= 2 && nd->operands[0] && nd->operands[1]) { g_emit.op_sval = IR_LIT(nd->operands[0]).sval; g_emit.lbl_t0 = IR_LIT(nd->operands[1]).sval;
            } if (g_emit.op_ival == 3 && nd->γ.node && sr3_gamma_label(nd->γ.node)) { const char *_e = sr3_gamma_label(nd->γ.node); g_emit.op_sval = strncmp(_e, "LBL__", 5) ? _e : _e + 5;
            } if (g_emit.op_ival == 0 && nd->γ.node && fc_call_active(nd->γ.node) && nd->n_operands == 1 && nd->operands[0]) { extern int zls_off(const IR_t *);
            int _as = bb_slot_get(nd->operands[0]); if (_as < 0) _as = zls_off(nd->operands[0]); if (_as >= 0) { g_emit.op_fc_wbytes = 16; g_emit.op_fc_base = _as;
            } } g_emit.op_define_role = (g_emit.op_ival == 4 && nd->n_operands >= 3) ? 7 : (int)g_emit.op_ival; extern std::string bb_define(); bb_emit_x86(bb_define()); return 0; }
        if (ir_define_is_bind(nd)) { g_emit.op_sval = IR_LIT(nd).sval; g_emit.lbl_t0 = (const char *)0; g_emit.op_proto = ir_define_bind_proto(nd); g_emit.op_entry = ir_define_bind_entry(nd);
            int _realstub = 0;
            if (g_emit_cfg && g_emit_cfg->n_dentry > 0) for (int _dq = 0; _dq < g_emit_cfg->n_dentry; _dq++) if (g_emit_cfg->dentry_node[_dq] == nd) { g_emit.lbl_t0 = g_emit_cfg->dentry_name[_dq];
            _realstub = g_emit_cfg->dentry_entry[_dq] ? 1 : 0; break; } g_emit.op_define_role = 6; extern std::string bb_define(); bb_emit_x86(bb_define()); int _d1st = 1;
            if (g_emit_cfg && g_emit.op_sval) for (int _dr = 0; _dr < g_emit_cfg->n_dentry; _dr++) { IR_t *_dp = g_emit_cfg->dentry_node[_dr]; if (_dp == nd) break;
            if (_dp && ir_define_is_bind(_dp) && IR_LIT(_dp).sval && !strcmp(IR_LIT(_dp).sval, g_emit.op_sval)) { _d1st = 0; break;
            } } if (g_is_text && g_emit.lbl_t0 && _d1st && _realstub) { long _sv5 = g_emit.op_ival; g_emit.op_ival = 5; g_emit.op_define_role = 5; bb_emit_x86(bb_define()); g_emit.op_ival = _sv5;
            } return 0; }
        fprintf(stderr, "IR_DEFINE %s with %d operand(s) is neither an SR citizen nor a bind: no lowerer builds this shape since the activation road was retired\n",
            IR_LIT(nd).sval ? IR_LIT(nd).sval : "", (int)nd->n_operands);
        abort();
    } return 0;
    case IR_STATEMENT_BEGIN: { g_emit.op_mon_stmt_tap = ((g_runtime.abi.trace_budget != 0 || x86_zdp_rbp_on()) && g_emit.op_stno > 0) ? 1 : 0;
        { if (g_emit.op_mon_stmt_tap && !g_runtime.eval.rt_fragment_emit && g_emit.op_stno > g_runtime.abi.mon_max_stno) g_runtime.abi.mon_max_stno = g_emit.op_stno; } g_emit.op_fc_bytes = 0;
        bb_emit_x86(bb_statement()); { extern std::string bb_zdp_anchor(long, long); if (g_emitter.walk_bb_node_inner__zdpa < 0) { const char * e = getenv("SCRIP_ZDP_TEARDOWN");
        g_emitter.walk_bb_node_inner__zdpa = (e && *e == '1') ? 1 : 0; } if (g_emitter.walk_bb_node_inner__zdpa) bb_emit_x86(bb_zdp_anchor((long)nd->op, (long)bb_node_id((IR_t *)nd)));
            } g_emit.op_mon_stmt_tap = 0; } return 0;
    case IR_STMT_MARK: g_emit.op_seal = nd->seal; g_emit.op_sval = nd->seal == IR_SEAL_LABEL_HOOK ? IR_LIT(nd).sval : (const char *)0;
        bb_emit_x86(bb_stmt_mark((long)IR_LIT(nd).ival, (long)nd->pat_static)); return 0;
    case IR_SETEXIT_TEST: bb_emit_x86(bb_setexit_test()); return 0;
    case IR_CLEANUP: bb_emit_x86(bb_cleanup()); return 0;
    case IR_TABLE_HDR: { g_emit.op_off = zls_off(nd); g_emit.op_fc_bytes = 0; bb_emit_x86(bb_table_hdr()); } return 0;
    case IR_LINE_MARK: if ((long)nd->pat_static > 0) g_emit.icn_line_cur = (long)nd->pat_static; if (IR_LIT(nd).sval && *IR_LIT(nd).sval) g_emit.icn_file_cur = IR_LIT(nd).sval;
        if (g_emit.icn_line_cur > 0) emit_stno_row(0, (int32_t)g_emit.icn_line_cur, g_emit.icn_file_cur); bb_emit_x86(bb_line_mark((long)nd->pat_static, IR_LIT(nd).sval)); return 0;
    case IR_STATEMENT_END:
    case IR_STATEMENT: { g_emit.op_fc_bytes = 0; bb_emit_x86(bb_statement()); } return 0;
    case IR_BOUND: { g_emit.op_sb = 1; g_emit.op_off = zls_off(nd); g_emit.op_fc_bytes = g_emit.op_zres ? g_emitter.g_zd_k : 0; bb_emit_x86(bb_bound()); } return 0;
    case IR_UNMARK: { IR_t * _mk = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0; g_emit.op_sb = 0; g_emit.op_off = _mk ? zls_off(_mk) : -1; g_emit.op_fc_bytes = 0;
        g_emit.op_unmark_chain = g_emit.op_zres && g_emit.op_zf_unpinned; g_emit.op_upost = zd_upost_of(nd);
        g_emit.op_ival = IR_LIT(nd).ival; bb_emit_x86(bb_bound()); } return 0;
    case IR_SUBSCRIPT: g_emit.op_seal = nd->seal; bb_emit_x86(nd->n_operands == 2 ? (IR_LIT(nd).sval && !strncmp(IR_LIT(nd).sval, "lv-check", 8) ? bb_subscript_lvck() : bb_subscript())
                                             : (nd->n_operands == 3 && IR_LIT(nd).sval && (!strcmp(IR_LIT(nd).sval, "nd2") || !strcmp(IR_LIT(nd).sval, "nd2-lv"))) ? bb_subscript2()
                                             : bb_section()); return 0;
    case IR_DEREF: bb_emit_x86(bb_deref()); return 0;
    case IR_UNIFY_CONST: { extern void unify_prepare(IR_t *); unify_prepare(nd); bb_emit_x86(bb_unify_const()); } return 0;
    case IR_UNIFY_STRUCT: { extern void unify_prepare(IR_t *); unify_prepare(nd); bb_emit_x86(bb_unify_struct()); } return 0;
    case IR_UNIFY_FIRST: { extern void unify_prepare(IR_t *); unify_prepare(nd); bb_emit_x86(bb_unify_first()); } return 0;
    case IR_UNIFY_VALUE: { extern void unify_prepare(IR_t *); unify_prepare(nd); bb_emit_x86(bb_unify_value()); } return 0;
    case IR_RANDOM: bb_emit_x86(bb_random()); return 0;
    case IR_ASSIGN_VAR: g_emit.op_seal = nd->seal; bb_emit_x86(nd->n_operands == 3 ? bb_assign_var_sub() : bb_assign_var()); return 0;
    case IR_REV_ASSIGN: {
        if (g_emit.op_sb == -1) { IR_t * _lv = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0; const char * _vn = _lv ? IR_LIT(_lv).sval : (const char *)0;
            g_emit.op_sval = _vn; g_emit.op_gva_k = (g_gva_active && _vn && !graph_has_local(g_emit_cfg, _vn)) ? gva_index_of(_vn) : -1;
            bb_emit_x86(bb_rev_assign_global()); return 0; }
        bb_emit_x86(bb_rev_assign()); return 0; }
    case IR_REV_SWAP: bb_emit_x86(bb_rev_swap()); return 0;
    case IR_REV_ASSIGN_VAR: bb_emit_x86(bb_rev_assign_var()); return 0;
    case IR_SWAP: bb_emit_x86(bb_swap()); return 0;
    case IR_SWAP_VAR: bb_emit_x86(bb_swap_var()); return 0;
    case IR_PROC_VALUE: bb_emit_x86(bb_proc_value()); return 0;
    case IR_CALL_VALUE: bb_emit_x86(bb_call_value()); return 0;
    case IR_LIMIT: g_emit.op_t0_pop = g_emitter.g_limit_t0_pop; bb_emit_x86(bb_limit()); return 0;
    case IR_LIMIT_GATE: bb_emit_x86(bb_limit_gate()); return 0;
    case IR_REPALT: return 0;
    case IR_ITERATE: bb_emit_x86(bb_iterate()); return 0;
    case IR_SCAN_ENTER: { g_emit.op_sb = 1; g_emit.op_sa = g_emit.op_a_slot; bb_emit_x86(bb_gen_scan()); } return 0;
    case IR_INITIAL: bb_emit_x86(bb_enter_init()); return 0;
    case IR_CREATE: { g_emit.op_activate_proc = IR_LIT(nd).sval; bb_emit_x86(bb_create()); } return 0;
    case IR_ACTIVATE: { g_emit.op_activate_proc = IR_LIT(nd).sval; bb_emit_x86(bb_activate()); } return 0;
    case IR_CORET: bb_emit_x86(bb_coret()); return 0;
    case IR_COFAIL: bb_emit_x86(bb_cofail()); return 0;
    case IR_GATE_ARM: bb_emit_x86(bb_gate_arm()); return 0;
    case IR_GATE: bb_emit_x86(bb_gate()); return 0;
    case IR_DISJUNCTION: { if (nd->n_operands > 0) { { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off; } }
        if (g_emit.op_zres && g_emitter.g_zd_djok) { g_emit.op_fc_bytes = zd_k_own(nd); g_emit.op_fc_base = g_emit.op_off;
            } g_emit.op_dj_bfail = g_emit.op_zres && g_emitter.g_zd_djok && fc_vdj_active(nd);
            g_emitter.g_zd_djok = 0;
            g_emit.op_dj_rec = (zls_dj_retains(nd) && !zd_djx_of(nd) && g_emit.op_xf_off != -1) ? 1 : 0;
            bb_emit_x86(bb_disjunction()); return 0; }
                                                  g_emit.op_ival = 0; bb_emit_x86(bb_gate()); } return 0;
    case IR_SCAN: { IR_t *_en = (nd->n_operands > 0) ? nd->operands[0] : NULL; IR_t *_bv = (nd->n_operands > 1) ? zd_value_producer(nd->operands[1]) : NULL;
        g_emit.op_sb = (IR_LIT(nd).dval == 4.0) ? 4 : ((nd->n_operands > 2 && !_bv) ? 2 : (IR_LIT(nd).dval == 3.0 ? 3 : 0)); g_emit.op_off = nd_slot(_en);
        g_emit.op_sa = (g_emit.op_sb == 2 && _en && _en->n_operands > 0) ? nd_slot(_en->operands[0]) :
            (_bv ? ((_bv->op == IR_VAR && IR_LIT(_bv).sval) ? drive_local_slot(IR_LIT(_bv).sval) : bb_slot_get(_bv) >= 0 ? bb_slot_get(_bv) : nd_slot(_bv)) : -1);
        g_emit.op_sc = (g_emit.op_sb == 4 && _en && _en->n_operands > 0) ? nd_slot(_en->operands[0]) : -1; g_emit.op_ival = zls_off(nd);
        g_emit.lbl_t0 = g_emitter.g_scan_body_beta ? g_emitter.g_scan_body_beta->name : NULL; g_emit.lbl_t0_p = g_emitter.g_scan_body_beta; g_emit.op_t0_pop = g_emitter.g_scan_t0_pop;
        bb_emit_x86(bb_gen_scan()); } return 0;
    case IR_SCAN_TAB: bb_emit_x86(bb_scan_tab()); return 0;
    case IR_SCAN_MOVE: bb_emit_x86(bb_scan_move()); return 0;
    case IR_SCAN_UPTO: { IR_t * _ca = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        if (g_emit.op_name1 && _ca) g_emit.op_ival = (_ca->n_operands > 0 && _ca->operands[0] && _ca->operands[0]->op == IR_LIT_INTEGER) ? IR_LIT(_ca->operands[0]).ival :
        (int64_t)strlen(g_emit.op_name1); bb_emit_x86(bb_scan_upto()); } return 0;
    case IR_SCAN_ANY: { IR_t * _ca = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        if (g_emit.op_name1 && _ca) g_emit.op_ival = (_ca->n_operands > 0 && _ca->operands[0] && _ca->operands[0]->op == IR_LIT_INTEGER) ? IR_LIT(_ca->operands[0]).ival :
        (int64_t)strlen(g_emit.op_name1); bb_emit_x86(bb_scan_any()); } return 0;
    case IR_SCAN_MANY: { IR_t * _ca = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        if (g_emit.op_name1 && _ca) g_emit.op_ival = (_ca->n_operands > 0 && _ca->operands[0] && _ca->operands[0]->op == IR_LIT_INTEGER) ? IR_LIT(_ca->operands[0]).ival :
        (int64_t)strlen(g_emit.op_name1); bb_emit_x86(bb_scan_many()); } return 0;
    case IR_SCAN_FIND: { IR_t * _fa = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
                                    if (g_emit.op_name1 && _fa) g_emit.op_ival = (_fa->n_operands > 0 && _fa->operands[0] && _fa->operands[0]->op == IR_LIT_INTEGER) ? IR_LIT(_fa->operands[0]).ival :
                                        (int64_t)strlen(g_emit.op_name1);
                                    bb_emit_x86(bb_scan_find()); } return 0;
    case IR_SCAN_MATCH: { IR_t * _ma = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
                                    if (g_emit.op_name1 && _ma) g_emit.op_ival = (_ma->n_operands > 0 && _ma->operands[0] && _ma->operands[0]->op == IR_LIT_INTEGER) ? IR_LIT(_ma->operands[0]).ival :
                                        (int64_t)strlen(g_emit.op_name1);
                                    bb_emit_x86(bb_scan_match()); } return 0;
    case IR_SCAN_POS: bb_emit_x86(bb_scan_pos()); return 0;
    case IR_SCAN_BAL: bb_emit_x86(bb_scan_bal()); return 0;
    case IR_RETURN: {
        IR_t *rv = (nd->n_operands > 0 && nd->operands[0]) ? nd->operands[0] : (IR_t *)0;
        if (!rv) g_emit.op_zread[0] = -1;
        { int _sa = rv ? bb_slot_get(rv) : -1; if (_sa < 0 && rv) { int _ns = nd_slot(rv); if (_ns >= 0) { bb_slot_register(rv, _ns); _sa = _ns; } } g_emit.op_sa = _sa;
            } g_emit.op_dval = IR_LIT(nd).dval;
        if (g_emit.flat_gen && g_emitter.g_suspend_resume_slot >= 0) { g_emit.op_sb = g_emitter.g_suspend_resume_slot; g_emit.lbl_t1_p = g_emit.flat_fail_p;
            g_emit.lbl_t1 = g_emit.flat_fail_p ? g_emit.flat_fail_p->name : (const char *)0; }
        else { g_emit.op_sb = -1; g_emit.lbl_t1_p = (bb_label_t *)0; g_emit.lbl_t1 = (const char *)0; }
        bb_emit_x86(bb_return()); return 0; }
    case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN:
    case IR_CALL_ICON: case IR_CALL_SNOBOL4:
    case IR_PROC_GEN:
    case IR_CALL: { g_emit.op_call_route = bb_call_route_classify(nd); call_fc_fill(nd); bb_emit_x86(bb_call()); return 0; }
    case IR_CALL_DIRECT: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_direct); return 0;
    case IR_CALL_PL_LEAF: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_pl_leaf); return 0;
    case IR_CALL_PAS_BI: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_pas_bi); return 0;
    case IR_CALL_CALLEE_REC: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_callee_rec); return 0;
    case IR_CALL_FIELD_REC: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_field_rec); return 0;
    case IR_CALL_BYNAME: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_byname); return 0;
    case IR_CALL_TRY: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_try); return 0;
    case IR_CALL_METHOD: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_method); return 0;
    case IR_CALL_REDISP: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_redisp); return 0;
    case IR_CALL_APPLY: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_apply); return 0;
    case IR_CALL_EVAL: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_eval); return 0;
    case IR_CALL_GEN: call_fc_fill(nd); call_prepare(nd); call_box_emit(bb_call_gen); return 0;
    case IR_FAIL: bb_emit_x86(bb_fail()); return 0;
    case IR_CUT: bb_emit_x86(bb_cut()); return 0;
    case IR_UNOP:
    case IR_UNOP_TEST: case IR_NULLTEST_VAR: {
        { extern int fc_vbinop_active(const IR_t *); extern long fc_vwpop(const IR_t *); long _w = fc_vwpop(nd); if (_w > 0 && fc_vbinop_active(nd) && !g_emit.op_zres) g_emit.op_wpop += (int)_w; }
        bb_emit_x86(bb_unop()); return 0; }
    case IR_FIELD_GET: case IR_FIELD_VAR: g_emit.op_seal = nd->seal; bb_emit_x86(bb_field_get()); return 0;
    default:
        fprintf(out, "# [walk_bb_node: kind=%d unhandled]\n", (int)nd->op);
        return 1;
    }
}
extern int bb_slot_get(IR_t *nd);
extern void bb_slot_register(IR_t *nd, int off);
extern int bb_varslot_peek(const char *name);
extern int is_global(const char *name);
extern int zls_result_live(const IR_t *nd);
extern int gva_index_of(const char *name);
extern int g_gva_active;
extern IR_graph_t * g_emit_cfg;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int drive_value_slot(IR_t *nd) {
    int e = bb_slot_get(nd);
    if (e >= 0) return e;
    if (nd) {
        int slot = nd_slot(nd);
        if (slot >= 0) { bb_slot_register(nd, slot); return slot; }
    }
    if (nd && ir_node_produces_value(nd->op)) {
                fprintf(stderr, "FATAL drive_value_slot: IR op=%d is a value-producer with no frame slot — ir_drive_slot_assign never granted it. "
                        "Emit-time allocation is ERADICATED (TMP-ERADICATE); add the grant in LOWER, never patch it here. op=%d\n", (int)nd->op, (int)nd->op);
        abort();
    }
    fprintf(stderr, "[TE] GOUGE drive_value_slot(op=%d): non-value-producer asked for a slot with no LOWER grant — emit-time allocation is ERADICATED "
                    "(TMP-ERADICATE); add an ir_drive_slot_assign grant for this op\n", nd ? (int)nd->op : -1);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_repalt(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t **ra_y, bb_label_t **ra_t, bb_label_t *node_γ, bb_label_t *node_ω) {
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
extern "C" int fc_alt_fpmax(const IR_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_alt_active(const IR_t *nd) { extern int fc_cells_active(void); return nd && nd->op == IR_MATCH_ALTERNATE && fc_cells_active() && fc_alt_fpmax(nd) >= 0; }
extern "C" int fc_seq_active(const IR_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_seq_on(const IR_t *nd) { return nd && fc_seq_active(nd); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_seq_elem_of(IR_t **nodes, int n, int k, int src) {
    int N = (int)(nodes[k]->n_operands / 2), el = 0, sp = -1;
    for (int j = 0; j < N; j++) if (nodes[k]->operands[2 * j] == nodes[src] || nodes[k]->operands[2 * j + 1] == nodes[src]) return j;
    { int p = nidx(nodes, n, nodes[src]); if (p >= 0) { sp = p; } }
    for (int j = 0; j < N; j++) { IR_t *e = nodes[k]->operands[2 * j]; { int p = nidx(nodes, n, e); if (p >= 0) { if (p <= sp) el = j; } } }
    return el;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bb_label_t * fc_seq_sigma_tgt(IR_t **nodes, int n, int k, int src, bb_label_t **lbls, bb_label_t *dflt) {
    int N = (int)(nodes[k]->n_operands / 2), el = fc_seq_elem_of(nodes, n, k, src);
    if (el >= N - 1) return dflt;
    IR_t *nx = nodes[k]->operands[2 * (el + 1)];
    { int p = nidx(nodes, n, nx); if (p >= 0) return lbls[p]; }
    return dflt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_clause_zero(int ak) {
    const IR_graph_t * g = g_emit_cfg;
    if (!g || !g->alt_zhi || ak < 1 || ak >= g->n_alts || g->alt_zhi[ak] <= g->alt_zlo) return std::string();
    int b = g->alt_zhi[ak] - g->alt_zlo;
    std::string r = x86("note", "the clause overlay: every clause's construct slots share one region, so a clause entered after another zeroes its own extent, as the prologue zeroes the first");
    if (b <= 128) { for (int q = 0; q < b / 8; q++) r += x86("mov", RDQ(emit_pl_fb(), g->alt_zlo + 8 * q), 0L); return r; }
    return r + x86("lea", "rdi", RDQ(emit_pl_fb(), g->alt_zlo)) + x86("xor", "eax", "eax") + x86("mov32", "ecx", (long)(b / 8)) + x86("rep_stosq");
}
static std::string pl_step_reseed_locals(void) {
    std::string r;
    if (!g_emit_cfg || !g_emit_cfg->lnames) return r;
    for (int i = 0; i < g_emit_cfg->nlocals; i++) {
        const char * nm = g_emit_cfg->lnames[i];
        if (!nm) continue;
        int off = ir_varslot_of(g_emit_cfg, nm);
        if (off < 0) continue;
        r += x86("mov", RDQ(emit_pl_fb(), off), 0L) + x86("mov", RDQ(emit_pl_fb(), off + 8), 0L);
    }
    return r;
}
static bb_label_t * fc_seq_phi_tgt(IR_t **nodes, int n, int k, int src, bb_label_t **betas, bb_label_t *dflt) {
    int el = fc_seq_elem_of(nodes, n, k, src);
    if (el <= 0) return dflt;
    IR_t *pv = nodes[k]->operands[2 * (el - 1) + 1];
    { int p = nidx(nodes, n, pv); if (p >= 0) return betas[p]; }
    return dflt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_parts_reserve(int n) {
    if (n <= g_emit.op_parts_cap) return;
    int cap = g_emit.op_parts_cap > 0 ? g_emit.op_parts_cap : 32; while (cap < n) cap *= 2;
    int64_t * grown = (int64_t *) ct_grow(g_emit.op_parts_ival, (size_t) cap * sizeof(int64_t));
    if (!grown) { fprintf(stderr, "emit: op_parts table: out of memory at %d parts\n", n); exit(2); }
    g_emit.op_parts_ival = grown; g_emit.op_parts_cap = cap;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_gate(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t *node_γ, bb_label_t *node_ω, const unsigned char *zon, const int *zout,
    const int *xb) {
    IR_t *nd = nodes[i]; int N = nd->n_operands;
    int hold = xb && zd_edge_retains(nd) && g_emitter.g_zd_nodes_p == nodes && g_emitter.g_zd_claim_p && g_emitter.g_zd_fam_p && g_emitter.g_zd_fbase_p && g_emitter.g_zd_gp_p;
    g_emit.xa_bb_emit_pair_n = 0;
    for (int j = 0; j < N; j++) {
        IR_t *arm = nd->operands[j];
        IR_t *rtgt = (arm && arm->n_operands > 0) ? arm->operands[0] : (IR_t *)0;
        int wantb = arm ? (int) arm->seal : 0;
        bb_label_t *t = node_ω;
        int k = -1;
        if (rtgt && rtgt->op == IR_SUCCEED) t = node_γ;
        else if (rtgt && rtgt->op != IR_FAIL) { k = nidx(nodes, n, rtgt); if (k >= 0) t = wantb ? betas[k] : lbls[k]; }
        xa_pair_push(NULL, t);
        { int a = arm ? nidx(nodes, n, arm) : -1; long r = 0;
            if (hold && k >= 0 && a >= 0 && zon[a] && zon[k] && zd_gate_pair_at(nodes, n, i, a, k, wantb, zon, zout, xb, &r)) { XA_PAIR(j).rsp = r; continue; } }
        if (xb && k >= 0 && zon[i] && zon[k] && xb[i] != INT_MIN && xb[k] != INT_MIN) XA_PAIR(j).rsp = (xb[i] + zout[i]) - (xb[k] + zout[k] - (wantb ? 0 : zd_k(nodes[k])));
        else if (xb && g_emitter.g_zd_fam_p && g_emitter.g_zd_claim_p && g_emitter.g_zd_gp_p && g_emitter.g_zd_nodes_p == nodes) {
            const int * cl = g_emitter.g_zd_claim_p, * fm = g_emitter.g_zd_fam_p, * fb = g_emitter.g_zd_fbase_p;
            int a = arm ? nidx(nodes, n, arm) : -1, lf = (a >= 0 && zon[a] && cl[a] >= 0) ? fm[cl[a]] : -1, kk = 0, kf = -1; long kd = 0;
            if (k >= 0 && zon[k] && cl[k] >= 0 && xb[k] != INT_MIN) { kd = xb[k] + zout[k] - (wantb ? 0 : zd_k(nodes[k])); kk = 1; }
            else if (k >= 0 && zon[k] && cl[k] >= 0 && fm[cl[k]] >= 0) { kd = fb[cl[k]] + zout[k] - (wantb ? 0 : zd_k(nodes[k])); kk = 2; kf = fm[cl[k]]; }
            if (lf >= 0 && kk == 1 && zd_edge_retains(nodes[k])) { IR_t * lt = zd_chase(nodes[k]->γ.node); int lq = lt ? nidx(nodes, n, lt) : -1;
                if (lq >= 0 && cl[lq] >= 0 && fm[cl[lq]] == lf) { kd = 0; kk = 2; kf = lf; } }
            long di = (zon[i] && cl[i] >= 0 && fm[cl[i]] >= 0 && fm[cl[i]] == lf) ? fb[cl[i]] + zout[i] : 0;
            if (lf >= 0 && kk == 2 && kf == lf) XA_PAIR(j).rsp = di - kd;
        }
    }
    g_emit.op_off = drive_value_slot(nd);
    DRIVE_FILL(nd, lbls[i], node_γ, node_ω, betas[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_cleanup(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t *node_γ, bb_label_t *node_ω, int zread) {
    IR_t *nd = nodes[i];
    int kind = (int) IR_LIT(nd).ival;
    IR_t *op0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
    IR_t *open = kind == CLEANUP_OPEN ? nd : (kind == CLEANUP_PROBE || kind == CLEANUP_WAKE) ? (IR_t *)0 : kind == CLEANUP_BANK ? nd : op0;
    g_emit.xa_bb_emit_pair_n = 0;
    if (kind != CLEANUP_PROBE && kind != CLEANUP_WAKE && !emit_resumable_frame())
        { fprintf(stderr, "FATAL emit: a setup_call_cleanup record (IR_CLEANUP kind %d) in a graph that is not a pinned RBP frame\n", kind); abort(); }
    if (kind == CLEANUP_RESUME) {
        IR_t *g = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0;
        IR_t *x = nd->n_operands > 2 ? nd->operands[2] : (IR_t *)0;
        int kg = nidx(nodes, n, g), kx = nidx(nodes, n, x);
        xa_pair_push(NULL, kg >= 0 ? betas[kg] : node_ω);
        xa_pair_push(NULL, kx >= 0 ? lbls[kx] : node_ω);
    }
    g_emit.op_ival = kind;
    g_emit.op_off = (kind == CLEANUP_PROBE || kind == CLEANUP_WAKE || kind == CLEANUP_DET) ? 0 : (open ? zls_off(open) : -1);
    g_emit.op_sa = -1;
    if (kind == CLEANUP_OPEN && nd->n_operands > 1 && nd->operands[1]) g_emit.op_sa = drive_value_slot(nd->operands[1]);
    g_emit.op_sb = kind == CLEANUP_OPEN ? zread : -1;
    if (kind == CLEANUP_PROBE && op0) g_emit.op_sa = zls_off(op0);
    if (kind == CLEANUP_BANK) g_emit.op_sa = zls_off(nd);
    if (kind == CLEANUP_DET) g_emit.op_sa = op0 ? zls_off(op0) : -1;
    DRIVE_FILL(nd, lbls[i], node_γ, node_ω, betas[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_match_alt(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t **na_s, bb_label_t **na_f, bb_label_t **na_fo, bb_label_t ***fc_sig, bb_label_t *node_γ,
    bb_label_t *node_ω, bb_label_t *chain_ω) {
    IR_t *nd = nodes[i]; int N = (nd->op == IR_MATCH_ARBNO) ? 1 : (nd->op == IR_DISJUNCTION) ? (int)IR_LIT(nd).ival : nd->n_operands / 2;
    g_emit.xa_bb_emit_pair_n = 0;
    for (int j = 0; j < N; j++) { IR_t *e = nd->operands[2 * j]; bb_label_t *t = node_ω; { int k = nidx(nodes, n, e); if (k >= 0) { t = lbls[k];
        } } if (nd->op == IR_DISJUNCTION && e && e->op == IR_FAIL && chain_ω) t = chain_ω; xa_pair_push(NULL, t); }
    for (int j = 0; j < N; j++) { IR_t *r = nd->operands[2 * j + 1]; bb_label_t *t = node_ω; { int k = nidx(nodes, n, r); if (k >= 0) { t = betas[k];
        } } if (nd->op == IR_MATCH_ARBNO && nd->n_operands > 3 && nd->operands[3] == nd) t = na_fo[i]; if (nd->op == IR_DISJUNCTION && r == nd) t = na_fo[i]; xa_pair_push(NULL, t); }
    if (nd->op == IR_DISJUNCTION && zd_djx_of(nd) && g_emitter.g_zd_out_p && g_emitter.g_zd_nodes_p == nodes)
        for (int j = 0; j < N; j++) { IR_t * r = nd->operands[2 * j + 1]; int k = nidx(nodes, n, r);
            XA_PAIR(N + j).rsp = (r == nd) ? zd_djx_of(nd) : (k >= 0 ? g_emitter.g_zd_out_p[i] - g_emitter.g_zd_out_p[k] : 0); }
    if (nd->op == IR_DISJUNCTION && !zd_djx_of(nd) && g_emitter.g_zd_on_cur && g_emitter.g_zd_n_cur == n && g_emitter.g_zd_on_cur[i])
        for (int j = 0; j < N; j++) if (nd->operands[2 * j + 1] != nd && nidx(nodes, n, nd->operands[2 * j + 1]) < 0) XA_PAIR(N + j).rsp = g_emitter.g_zd_k + g_emitter.g_zd_wpop;
    if (nd->op == IR_DISJUNCTION && zd_edge_retains(nd) && g_emitter.g_zd_nodes_p == nodes && g_emitter.g_zd_xb_p && g_emitter.g_zd_out_p && g_emitter.g_zd_plan_on &&
        g_emitter.g_zd_claim_p && g_emitter.g_zd_fam_p && g_emitter.g_zd_fbase_p && g_emitter.g_zd_gp_p) {
        const unsigned char * zon = g_emitter.g_zd_plan_on; const int * zout = g_emitter.g_zd_out_p, * xb = g_emitter.g_zd_xb_p;
        for (int j = 0; j < N; j++) {
            int e = zd_dj_arm_end(nodes, n, i, j), k = nidx(nodes, n, zd_chase(nd->operands[2 * j + 1]));
            long r = 0, da = 0, dk = 0;
            if (e < 0 || !zon[e]) continue;
            if (zd_pl_dj_arm_holds(nd, j) && k >= 0 && zon[k] && zd_chase(nodes[k]->γ.node) == nd) {
                int ca = zd_cpos(e, xb, zout, &da), ck = zd_cpos(k, xb, zout, &dk);
                if (ca != INT_MIN && ca == ck) XA_PAIR(N + j).rsp = (da - g_emitter.g_zd_gp_p[e]) - (dk - g_emitter.g_zd_gp_p[k]);
                continue;
            }
            if (zd_pl_dj_arm_holds(nd, j)) { if (k >= 0 && zon[k] && zd_gate_pair(nodes, n, e, k, 1, zon, zout, xb, &r)) XA_PAIR(N + j).rsp = r; continue; }
            { int ca = zd_cpos(e, xb, zout, &da), ck = zd_cpos(i, xb, zout, &dk); da -= g_emitter.g_zd_gp_p[e]; if (ca != INT_MIN && ca == ck) XA_PAIR(N + j).rsp = da - dk; }
        }
    }
    { xa_pair_push(na_s[i], NULL); }
    { xa_pair_push(na_f[i], NULL); }
    if (nd->op == IR_MATCH_ARBNO) { IR_t *_t = (nd->n_operands > 2) ? nd->operands[2] : (IR_t *)0; { IR_t *_s0 = (nd->n_operands > 1) ? nd->operands[1] : (IR_t *)0; int _i0 = -1, _i1 = -1;
        for (int k = 0; k < n; k++) { if (_s0 && nodes[k] == _s0) _i0 = k; if (_t && nodes[k] == _t) _i1 = k; } if (_i0 > _i1) { int _sw = _i0; _i0 = _i1; _i1 = _sw;
        } if (_i0 >= 0) for (int k = _i1; k >= _i0; k--) if (nodes[k] && nodes[k] != nd && nodes[k]->γ.node == nd) { _t = nodes[k]; break; } } bb_label_t *_tb = node_ω;
        for (int k = 0; k < n; k++) if (_t && nodes[k] == _t) { _tb = betas[k]; break; } xa_pair_push(NULL, _tb); }
    if (nd->op == IR_MATCH_ALTERNATE && fc_sig && fc_sig[i])
        for (int j = 0; j < N; j++) { xa_pair_push(fc_sig[i][j], NULL); }
    { xa_pair_push(na_fo[i], NULL); }
    g_emit.op_off = drive_value_slot(nd); g_emit.op_ival = (int64_t)N;
    if (nd->op == IR_DISJUNCTION) { emit_parts_reserve(N); for (int j = 0; j < N; j++) { IR_t *rj = (2 * N + j < nd->n_operands) ? nd->operands[2 * N + j] : (IR_t *)0;
        { int zok = g_emitter.g_zd_on_cur && g_emitter.g_zd_n_cur == n && g_emitter.g_zd_on_cur[i] && g_emitter.g_zd_zarm_p &&
              g_emitter.g_zd_zarm_nodes == nodes;
          if (zok && rj && nd_slot(rj) >= 0 && ((rj->op == IR_CONJUNCTION && !zd_dj_admitted(g_emitter.g_zd_zarm_n, i)) ||
              zd_dj_cellless(nodes, n, g_emitter.g_zd_zarm_p, nidx(nodes, n, rj)))) { g_emit.op_parts_ival[j] = nd_slot(rj); continue; } }
        { int _g = 0;
        while (rj && rj->op == IR_CONJUNCTION && rj->n_operands > 0 && _g++ < 16) rj = rj->operands[0]; }
        int rs = rj ? emit_binop_opnd_slot(rj) : -1; g_emit.op_parts_ival[j] = rs;
        int lasn = (rj && zd_icn_local_assign(rj) && zd_k_own(rj) == 0) ? 1 : 0;
        if (lasn) { int voff = bb_varslot_peek(IR_LIT(rj).sval); g_emit.op_parts_ival[j] = voff >= 0 ? voff : -1; }
        if (rj && zls_g_home_frame(g_emit_cfg) && (zd_icn_local_var(rj) || lasn)) { extern int zst_coord(const char *); int sv = g_emitter.g_zd_cur; g_emitter.g_zd_cur = i;
            int c = zst_coord(IR_LIT(rj).sval); g_emitter.g_zd_cur = sv; if (c >= 0) { g_emit.op_parts_ival[j] = c; continue; } }
        if (rj && g_emitter.g_zd_on_cur && g_emitter.g_zd_n_cur == n && g_emitter.g_zd_on_cur[i]) { int kr = nidx(nodes, n, rj);
            if (kr >= 0 && g_emitter.g_zd_on_cur[kr] && !zd_icn_local_var(rj) && !lasn) g_emit.op_parts_ival[j] = -1;
            }
        } g_emit.op_parts_n = N; }
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
__attribute__((noreturn)) void rt_ab_undef_fn_stub(void);
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
static void drive_no_template(IR_t *nd) {
    fprintf(stderr,
        "FATAL emit_drive: IR op=%d has NO TEMPLATE in the universal driver -- the switch carries no case for it at all. Every op must be handled; the driver never refuses silently. Implement op=%d."
        "\n",
            nd ? (int)nd->op : -1, nd ? (int)nd->op : -1);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void drive_guard_refused(IR_t *nd, int line) {
    fprintf(stderr,
        "FATAL emit_drive: IR op=%d HAS a template and its own case REFUSED AT A GUARD -- emit.cpp:%d. This is NOT a missing template: do NOT implement op=%d. Read the guard on that line and find wh"
        "y its precondition did not hold for this node -- a slot that came back < 0, a child that was null, an operand count short of what the case needs.\n",
            nd ? (int)nd->op : -1, line, nd ? (int)nd->op : -1);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_drive(IR_t *nd, bb_label_t *lbl_α, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!nd) { drive_guard_refused(nd, __LINE__); return; }
    { extern int g_scan_regs_live; g_scan_regs_live = nd->in_scan ? 1 : 0; }
    switch (nd->op) {
    case IR_LIT_STRING: case IR_LIT_INTEGER: case IR_LIT_REAL:
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIT_CHARSET:
    case IR_LIT_NAME:
    case IR_LIT_ATOM:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KW_ICON:
    case IR_KW_ICON_GEN:
    case IR_KW_SNOBOL4:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KW_ASSIGN:
    case IR_KW_ASSIGN_SNOBOL4:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_VAR: {
        const char *vn = IR_LIT(nd).sval;
        if (vn && vn[0] == '&') { g_emit.op_sval = vn; g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); }
        else if (vn && is_global(vn) && !graph_has_local(g_emit_cfg, vn)) { g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); g_emit.op_sval = vn;
            g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1; }
        else if (vn) { int voff = drive_local_slot(vn); g_emit.op_sa = voff; g_emit.op_off = (voff != -1) ? drive_value_slot(nd) : -1; }
        else { g_emit.op_sa = -1; g_emit.op_off = -1; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_VAR_REF: {
        const char *vn = IR_LIT(nd).sval; g_emit.op_var_named = nd->pat_static;
        if (vn && is_global(vn) && !graph_has_local(g_emit_cfg, vn)) { g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); g_emit.op_sval = vn;
            g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1; }
        else if (vn) { int voff = drive_local_slot(vn); g_emit.op_sa = voff; g_emit.op_off = drive_value_slot(nd); }
        else { g_emit.op_sa = -1; g_emit.op_off = -1; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_BINOP: case IR_BINOP_TEST: case IR_BINOP_RELOP_VAL: {
        g_emit.op_relop_descr = 0; g_emit.op_num_real = 0; g_emit.op_arith_descr = 0; g_emit.op_gva_k1 = -1; g_emit.op_gva_k2 = -1;
        g_emit.op_imm_a_ok = 0; g_emit.op_imm_b_ok = 0; g_emit.op_imm_a = 0; g_emit.op_imm_b = 0;
        g_emit.op_snul_a_ok = ir_value_is_null_string(bb_child0(nd)); g_emit.op_snul_b_ok = ir_value_is_null_string(bb_child1(nd));
        int sa = -1, sb = -1;
        if (binop_is_num_real(g_emit_cfg, nd)) { int ra = bb_slot_get(bb_child0(nd)), rb = bb_slot_get(bb_child1(nd)); if (ra >= 0 && rb >= 0) { sa = ra; sb = rb; g_emit.op_num_real = 1; } }
        if (!g_emit.op_num_real) { sa = emit_binop_opnd_slot(bb_child0(nd)); sb = emit_binop_opnd_slot(bb_child1(nd)); }
        if (sa < 0 || sb < 0) { drive_guard_refused(nd, __LINE__); break; }
        if (!g_emit.op_num_real) {
            IR_t * la = bb_child0(nd), * lb = bb_child1(nd);
            if (la && la->op == IR_LIT_INTEGER && IR_LIT(la).ival >= -2147483648LL && IR_LIT(la).ival <= 2147483647LL) { g_emit.op_imm_a_ok = 1; g_emit.op_imm_a = (long)IR_LIT(la).ival; }
            if (lb && lb->op == IR_LIT_INTEGER && IR_LIT(lb).ival >= -2147483648LL && IR_LIT(lb).ival <= 2147483647LL) { g_emit.op_imm_b_ok = 1; g_emit.op_imm_b = (long)IR_LIT(lb).ival; }
        }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd); g_emit.op_binop_kind = (int)binop_slot_kind(nd);
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_UNOP: case IR_UNOP_TEST: case IR_NULLTEST_VAR: {
        int sa = emit_binop_opnd_slot(bb_child0(nd));
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_FIELD_GET: case IR_FIELD_VAR: {
        IR_t * obj = bb_child0(nd);
        int sa = obj ? emit_binop_opnd_slot(obj) : -1;
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_a_slot = sa; g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); g_emit.op_seal = nd->seal;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ASSIGN: {
        const char *vn = IR_LIT(nd).sval;
        if (!vn) {
            fprintf(stderr, "FATAL emit_drive IR_ASSIGN guard: nameless %d-operand assign (assign-through-lvalue-producer: !x/?x element-variable or "
                            "s[i:j] section) — LOWER's TT_ASSIGN terminal arm minted a placeholder; not a missing template. ASSIGN-LV rung, GOAL-IR-IMMUTABLE-EMIT.md.\n", nd->n_operands);
            abort();
        }
        if (is_global(vn) && !graph_has_local(g_emit_cfg, vn)) {
            g_emit.op_sb = -1; g_emit.op_sval = vn; g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1;
            g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
        }
        { int voff = drive_local_slot(vn);
          if (voff == -1) { fprintf(stderr, "[TE-4] IR_ASSIGN local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", vn); abort();
              }
          g_emit.op_sb = voff; }
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_VAR_FRAME:
    case IR_ASSIGN_FRAME:
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_CALL: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN: case IR_CALL_PROC_STAGED: case IR_PROC_GEN:
    case IR_CALL_DIRECT: case IR_CALL_PL_LEAF: case IR_CALL_PAS_BI: case IR_CALL_CALLEE_REC: case IR_CALL_FIELD_REC: case IR_CALL_BYNAME: case IR_CALL_TRY: case IR_CALL_METHOD: case IR_CALL_REDISP:
    case IR_CALL_APPLY: case IR_CALL_EVAL: case IR_CALL_GEN:
    case IR_CALL_ICON: case IR_CALL_SNOBOL4: {
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = ir_call_arg(nd, i); g_emit.op_arg_slot[i] = nd_slot(a); }
        g_emit.op_arg_slot_n = na;
        { IR_t *_gt = nd->γ.node; int _gg = 0; while (_gt && _gt->op == IR_GOTO && _gg++ < 128) _gt = _gt->γ.node; IR_t *_ot = nd->ω.node; int _og = 0;
            while (_ot && _ot->op == IR_GOTO && _og++ < 128) _ot = _ot->γ.node;
            g_emit.op_sb = (g_emit_cfg && g_emit_cfg->spine_cells && _gt && _ot && _gt == _ot && (unsigned char)nd->ω.sz[0] == 0xce && (unsigned char)nd->ω.sz[1] == 0xb2) ? 1 : 0; }
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_BEGIN: {
        IR_t *subj = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = subj ? drive_value_slot(subj) : -1;
        { extern int fc_vread_fp(const IR_t *); g_emit.op_subj_cell = (subj && fc_vread_fp(nd) >= 0) ? 1 : 0; }
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_frame_extra = planner_act_extra(nd);
        DRIVE_PAIR_RESET();
        if (g_emit.lbl_t0_p) { for (int _q = 0; _q < 3; _q++) { xa_pair_push(NULL, NULL); } xa_pair_push(g_emit.lbl_t0_p, NULL); g_emit.lbl_t0_p = NULL; g_emit.lbl_t0 = NULL;
          xa_pair_push(g_emit.lbl_t0o_p, NULL); g_emit.lbl_t0o_p = NULL; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_END: {
        IR_t *hd = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = hd ? drive_value_slot(hd) : -1;
        g_emit.op_frame_extra = hd ? planner_act_extra(hd) : 32;
        if (getenv("SCRIP_EDRIVE_END_DIAG")) fprintf(stderr, "[EDRIVE-END] op_off=%d op_fc_disp(pre)=%d op_tail(pre)=%d\n", g_emit.op_off, g_emit.op_fc_disp, g_emit.op_tail);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GOTO_DEFERRED: {
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_ival = nd->seal; DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_REPLACE: {
        IR_t *hd = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        IR_t *rp = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0;
        IR_t *sb = nd->n_operands > 2 ? nd->operands[2] : (IR_t *)0;
        g_emit.op_off = hd ? drive_value_slot(hd) : -1;
        g_emit.op_frame_extra = hd ? planner_act_extra(hd) : 32;
        g_emit.op_sb = rp ? drive_value_slot(rp) : -1;
        g_emit.op_sa = sb ? drive_value_slot(sb) : -1;
        g_emit.op_sval = IR_LIT(nd).sval;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ASSIGN_COND: {
        IR_t *box = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0;
        g_emit.op_off = box ? drive_value_slot(box) : -1; g_emit.op_phase = 1;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ASSIGN_IMM: {
        IR_t *box = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0;
        g_emit.op_off = box ? drive_value_slot(box) : -1; g_emit.op_phase = 2;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ASSIGN_SAVE: {
        g_emit.op_off = drive_value_slot(nd); g_emit.op_phase = 0;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_FENCE0: {
        g_emit.op_off = drive_value_slot(nd); DRIVE_PAIR_RESET(); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β);
        break;
    }
    case IR_MATCH_FENCE1: {
        drive_guard_refused(nd, __LINE__);
        break;
    }
    case IR_MATCH_ABORT: {
        g_emit.op_off = -1; DRIVE_PAIR_RESET(); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ALTERNATE: case IR_MATCH_ARBNO: case IR_SCAN_ALTERNATE: {
        drive_guard_refused(nd, __LINE__);
        break;
    }
    case IR_COERCE_STRING: case IR_COERCE_INTEGER: case IR_COERCE_REAL: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        int sa = a0 ? emit_binop_opnd_slot(a0) : -1;
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_COERCE_NUMERIC: case IR_CMP_TEST: case IR_IDENT: case IR_DIFFER: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        IR_t * a1 = nd->n_operands > 1 ? nd->operands[1] : (nd->op == IR_COERCE_NUMERIC ? a0 : (IR_t *)0);
        int sa = a0 ? emit_binop_opnd_slot(a0) : -1; int sb = a1 ? emit_binop_opnd_slot(a1) : -1;
        if (sa < 0 || sb < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_LEN: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1; if (a0 && g_emit.op_sa < 0) { extern int zls_off(const IR_t *); int _zsa = zls_off(a0); if (_zsa >= 0) { g_emit.op_sa = _zsa; g_emit.op_zres = 1;
            } } g_emit.op_off = -1;
        if (a0 && g_emit.op_sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
            g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : NULL; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_LIT: {
        g_emit.op_sa = -1; g_emit.op_off = -1;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ATP: {
        g_emit.op_sa = -1; g_emit.op_off = -1;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ANY: case IR_MATCH_NOTANY: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1; g_emit.op_off = -1;
        if (a0 && g_emit.op_sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_SPAN: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1;
        if (a0 && g_emit.op_sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.x86_scratch_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_RKRUN: case IR_MATCH_RKCAPS: case IR_MATCH_RKCAPE: { DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break; }
    case IR_MATCH_RKLAND: { DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break; }
    case IR_MATCH_BREAK: case IR_MATCH_BREAKX: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1;
        if (a0 && g_emit.op_sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.x86_scratch_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_TAB: case IR_MATCH_RTAB: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = -1;
        if (!a0) { g_emit.op_sb = (int)IR_LIT(nd).ival; g_emit.op_sa = -1; }
        else if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { extern int zls_off(const IR_t *); int _zsa = zls_off(a0); if (_zsa >= 0) { sl = _zsa; g_emit.op_zres = 1;
            } } if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
            g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : NULL; }
        g_emit.x86_scratch_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_LAMBDA: {
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_POS: case IR_MATCH_RPOS: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = -1;
        if (!a0) { g_emit.op_sb = (int)IR_LIT(nd).ival; g_emit.op_sa = -1; }
        else if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { extern int zls_off(const IR_t *); int _zsa = zls_off(a0); if (_zsa >= 0) { sl = _zsa; g_emit.op_zres = 1;
            } } if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0;
            g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : NULL; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_REM: case IR_MATCH_ARB: case IR_MATCH_BAL: {
        g_emit.x86_scratch_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_VALUE: {
        g_emit.op_sa = -1; g_emit.op_off = -1;
        g_emit.x86_scratch_off = drive_value_slot(nd);
        g_emit.bb_child_fn = (void *) 0; g_emit.bb_child_lbl = (const char *) 0;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_DEFER: {
        g_emit.op_sa = -1; g_emit.op_off = -1;
        g_emit.x86_scratch_off = drive_value_slot(nd);
        g_emit.bb_child_fn = (void *) 0; g_emit.bb_child_lbl = (const char *) 0;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_TO: {
        if (!bb_child0(nd) || !bb_child1(nd)) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = drive_value_slot(bb_child0(nd)); g_emit.op_sb = drive_value_slot(bb_child1(nd));
        g_emit.op_sc = -1;
        g_emit.op_num_real = (IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval, "ar") == 0) ? 1 : 0;
        g_emit.op_db_walk = ir_range_kind_is(nd, "db"); g_emit.op_to_trailed = ir_range_trailed(nd);
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_TO_BY: {
        if (!bb_child0(nd) || !bb_child1(nd) || nd->n_operands < 3 || !nd->operands[2]) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = drive_value_slot(bb_child0(nd)); g_emit.op_sb = drive_value_slot(bb_child1(nd));
        g_emit.op_sc = drive_value_slot(nd->operands[2]);
        g_emit.op_num_real = (IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval, "ar") == 0) ? 1 : 0;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MAKE_LIST: {
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = nd->operands[i]; g_emit.op_arg_slot[i] = a ? drive_value_slot(a) : -1; }
        g_emit.op_arg_slot_n = na;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REV_ASSIGN: {
        IR_t * lv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        const char * vn = lv ? IR_LIT(lv).sval : NULL;
        if (!vn || nd->n_operands < 1 || !nd->operands[0]) { drive_guard_refused(nd, __LINE__); break; }
        { extern int is_global(const char *);
          if (vn[0] == '&' || (is_global(vn) && !graph_has_local(g_emit_cfg, vn))) { g_emit.op_sb = -1; g_emit.op_sval = vn; g_emit.op_gva_k = (vn[0] != '&' && g_gva_active) ? gva_index_of(vn) : -1; }
          else { int voff = bb_varslot_peek(vn);
            if (voff == -1) { fprintf(stderr,
                "[TE-4] IR_REV_ASSIGN '%s': not a global (is_global checked, so not the "
                "bb_rev_assign_global path) and no LOWER-granted varslot as a local — "
                "grant it in ir_drive_slot_assign, never in the emitter\n", vn); abort(); }
            g_emit.op_sb = voff; } }
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sc = g_emit.op_off + 16;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REV_SWAP: {
        const char * ln = IR_LIT(nd).sval;
        const char * rn = (nd->n_operands > 0 && nd->operands[0]) ? IR_LIT(nd->operands[0]).sval : NULL;
        if (!ln || !rn) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sval = ln; g_emit.op_name2 = rn;
        g_emit.op_sb = -1; g_emit.op_sa = -1;
        { extern int is_global(const char *);
          if (ln[0] != '&' && !(is_global(ln) && !graph_has_local(g_emit_cfg, ln))) { int voff = bb_varslot_peek(ln);
              if (voff == -1)
              { fprintf(stderr,
              "[TE-4] IR_REV_SWAP lhs '%s' is neither a global (is_global says no, so not the NV_PTR_fn by-name path) nor a local with a LOWER-granted varslot — grant it in ir_drive_slot_assign (s"
              "crip_ir.c), never allocate in the emitter\n", ln); abort(); } g_emit.op_sb = voff; }
          if (rn[0] != '&' && !(is_global(rn) && !graph_has_local(g_emit_cfg, rn))) { int voff = bb_varslot_peek(rn);
              if (voff == -1)
              { fprintf(stderr,
              "[TE-4] IR_REV_SWAP rhs '%s' is neither a global (is_global says no, so not the NV_PTR_fn by-name path) nor a local with a LOWER-granted varslot — grant it in ir_drive_slot_assign (s"
              "crip_ir.c), never allocate in the emitter\n", rn); abort(); } g_emit.op_sa = voff; } }
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REV_ASSIGN_VAR: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * r = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = v ? drive_value_slot(v) : -1; int sb = r ? drive_value_slot(r) : -1;
        if (sa < 0 || sb < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sb;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sc = g_emit.op_off + 16;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GOTO:
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_BOUND: {
        g_emit.op_sb = 1; g_emit.op_off = zls_off(nd); g_emit.op_fc_bytes = 0;
        if (g_emit.op_off < 0) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_TABLE_HDR: {
        g_emit.op_off = zls_off(nd); g_emit.op_fc_bytes = 0;
        if (g_emit.op_off < 0) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_UNMARK: {
        IR_t * _mk = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sb = 0; g_emit.op_off = _mk ? zls_off(_mk) : -1; g_emit.op_fc_bytes = 0;
        if (g_emit.op_off < 0) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CONJUNCTION:
        if (nd->n_operands > 0 && nd->operands[0] && bb_slot_get(nd) < 0) { int voff = bb_slot_get(nd->operands[0]); if (voff >= 0) bb_slot_register(nd, voff); }
        { int dst = nd_slot(nd); if (dst >= 0 && nd->n_operands > 0 && nd->operands[0]) {
            int src = bb_slot_get(nd->operands[0]);
            if (src >= 0 && dst >= 0 && src != dst && !(g_emit_cfg && g_emit_cfg->spine_cells && zls_g_home_frame(g_emit_cfg) && zd_k_own(nd) == 0)) {
                g_emit.op_sa = src; g_emit.op_off = dst;
            } else {
                g_emit.op_sa = -1; g_emit.op_off = -1;
            }
        } else {
            g_emit.op_sa = -1; g_emit.op_off = -1;
        } }
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SUCCEED:
    case IR_CUT:
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_DEFINE:
        if (ir_define_sr_citizen(nd)) {
        { int64_t v = IR_LIT(nd).ival; int64_t r = (v == 1 || v == 2 || v == 3 || v == 4) ? v : 0; g_emit.op_ival = r; g_emit.op_sval = r ? (const char *)0 : IR_LIT(nd).sval; }
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = nd->operands[i]; int s = a ? bb_slot_get(a) : -1; if (s < 0 && a) s = zls_off(a); g_emit.op_arg_slot[i] = s; }
        g_emit.op_arg_slot_n = na;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
        if (!ir_define_is_bind(nd)) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_STATEMENT_BEGIN:
    case IR_STATEMENT_END:
    case IR_STMT_MARK:
    case IR_SETEXIT_TEST:
    case IR_STATEMENT:
        g_emit.op_fc_bytes = 0;
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LINE_MARK:
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SUSPEND: {
        g_emit.op_activate_proc = IR_LIT(nd).sval;
        IR_t * ev = bb_child0(nd); int sa = ev ? bb_slot_get(ev) : -1;
        if (sa < 0 && ev) sa = nd_slot(ev);
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.lbl_t0 = g_emitter.g_suspend_dobody_beta ? g_emitter.g_suspend_dobody_beta->name : NULL; g_emit.lbl_t0_p = g_emitter.g_suspend_dobody_beta;
        g_emit.lbl_t1_p = lbl_β; g_emit.lbl_t1 = lbl_β ? lbl_β->name : NULL;
        g_emit.op_sb = g_emitter.g_suspend_resume_slot;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SUBSCRIPT: {
        if (nd->n_operands == 2) {
            IR_t * vb = nd->operands[0]; IR_t * vi = nd->operands[1];
            int va = vb ? drive_value_slot(vb) : -1; int vs = vi ? drive_value_slot(vi) : -1;
            if (va < 0 || vs < 0) { drive_guard_refused(nd, __LINE__); break; }
            g_emit.op_a_slot = va; g_emit.op_sa = vs; g_emit.op_off = drive_value_slot(nd); g_emit.op_seal = nd->seal;
            DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
        }
        IR_t * base = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * i1 = nd->n_operands > 1 ? nd->operands[1] : NULL;
        IR_t * i2 = nd->n_operands > 2 ? nd->operands[2] : NULL;
        int sa = base ? drive_value_slot(base) : -1;
        int sb = i1 ? drive_value_slot(i1) : -1;
        int sc = i2 ? drive_value_slot(i2) : -1;
        if (sa < 0 || sb < 0 || sc < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_sb = sc;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_RANDOM: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = v ? drive_value_slot(v) : -1;
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_a_slot = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_UNIFY_CONST: case IR_UNIFY_STRUCT: case IR_UNIFY_FIRST: case IR_UNIFY_VALUE:
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_DEREF: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = v ? drive_value_slot(v) : -1;
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_a_slot = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ASSIGN_VAR: {
        if (nd->n_operands == 3) {
            IR_t * b = nd->operands[0]; IR_t * ix = nd->operands[1]; IR_t * v = nd->operands[2];
            int sb2 = b ? drive_value_slot(b) : -1; int si = ix ? drive_value_slot(ix) : -1; int sv = v ? drive_value_slot(v) : -1;
            if (sb2 < 0 || si < 0 || sv < 0) { drive_guard_refused(nd, __LINE__); break; }
            g_emit.op_a_slot = sb2; g_emit.op_sa = si; g_emit.op_sb = sv; g_emit.op_off = drive_value_slot(nd);
            DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
        }
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * r = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = v ? drive_value_slot(v) : -1; int sb = r ? drive_value_slot(r) : -1;
        if (sa < 0 || sb < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SWAP: {
        IR_t * xv = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * yv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = xv ? bb_varslot_peek(IR_LIT(xv).sval) : -1;
        int sb = yv ? bb_varslot_peek(IR_LIT(yv).sval) : -1;
        if (sa < 0 || sb < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_PROC_VALUE: {
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CALL_VALUE: {
        IR_t * cal = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sc = cal ? drive_value_slot(cal) : -1;
        int na2 = nd->n_operands - 1;
        if (sc < 0 || na2 < 0) { drive_guard_refused(nd, __LINE__); break; }
        drive_arg_slots_reserve(na2);
        for (int i = 0; i < na2; i++) { IR_t * a = nd->operands[i + 1]; g_emit.op_arg_slot[i] = a ? drive_value_slot(a) : -1; }
        g_emit.op_arg_slot_n = na2;
        g_emit.op_sval = IR_LIT(nd).sval;
        g_emit.op_sa = sc; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SWAP_VAR: {
        IR_t * xv = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * yv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = xv ? drive_value_slot(xv) : -1; int sb = yv ? drive_value_slot(yv) : -1;
        if (sa < 0 || sb < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_LIMIT_GATE: {
        IR_t * cnt = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sc = cnt ? drive_value_slot(cnt) : -1;
        if (sc < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sc; g_emit.op_off = drive_value_slot(nd); g_emit.op_sb = zls_g_home_frame(g_emit_cfg) ? g_emit.op_off + 16 : -1;
        g_emit.op_sc = (g_emit.op_sb >= 0 && zls_home_why(nd)) ? g_emit.op_sb + 16 : -1;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_LIMIT: {
        IR_t * gen = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * cnt = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = gen ? drive_value_slot(gen) : -1;
        int sc = cnt ? drive_value_slot(cnt) : -1;
        if (sa < 0 || sc < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.op_sc = sc;
        g_emit.lbl_t0 = g_emitter.g_limit_gen_beta ? g_emitter.g_limit_gen_beta->name : (lbl_ω ? lbl_ω->name : NULL);
        g_emit.lbl_t0_p = g_emitter.g_limit_gen_beta ? g_emitter.g_limit_gen_beta : lbl_ω;
        g_emit.op_t0_pop = g_emitter.g_limit_t0_pop;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sb = -1;
        for (int _gk = 2; zls_g_home_frame(g_emit_cfg) && _gk < nd->n_operands; _gk++) if (nd->operands[_gk] && nd->operands[_gk]->op == IR_LIMIT_GATE) g_emit.op_sb =
            drive_value_slot(nd->operands[_gk]) + 16;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CREATE: {
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_activate_proc = IR_LIT(nd).sval;
        g_emit.lbl_t0 = g_create_body_entry ? g_create_body_entry->name : NULL;
        g_emit.lbl_t0_p = g_create_body_entry;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ACTIVATE: {
        IR_t * ce = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * xv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = ce ? bb_slot_get(ce) : -1;
        if (sa < 0 && ce) sa = nd_slot(ce);
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa;
        g_emit.op_sb = xv ? bb_slot_get(xv) : -1;
        if (g_emit.op_sb < 0 && xv) g_emit.op_sb = nd_slot(xv);
        g_emit.op_activate_proc = IR_LIT(nd).sval;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CORET: {
        IR_t * pv = nd->n_operands > 0 ? zd_conj_value_node(nd->operands[0]) : NULL;
        int sa = pv ? bb_slot_get(pv) : -1;
        if (sa < 0 && pv) { int _ns = nd_slot(pv); if (_ns >= 0) { bb_slot_register(pv, _ns); sa = _ns; } }
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        if (pv->op == IR_CONJUNCTION && nd_slot(pv) >= 0) sa = nd_slot(pv);
        g_emit.op_sa = sa;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_COFAIL: {
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GATE: {
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GATE_ARM: {
        IR_t * ig = nd->n_operands > 1 ? nd->operands[1] : NULL;
        g_emit.op_off = ig ? drive_value_slot(ig) : -1;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_DISJUNCTION: {
        if (nd->n_operands > 0) { drive_guard_refused(nd, __LINE__); break; }
        int off = drive_value_slot(nd);
        g_emit.op_off = off;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ITERATE: {
        IR_t * obj = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = obj ? bb_slot_get(obj) : -1;
        if (sa < 0 && obj) sa = nd_slot(obj);
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sb = g_emit.op_off + 16;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_INITIAL: {
        g_emit.op_off = nd_slot(nd);
        if (g_emit.op_off < 0) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_ENTER: {
        IR_t * subj = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = subj ? bb_slot_get(subj) : -1;
        if (sa < 0 && subj) sa = nd_slot(subj);
        if (sa < 0) { drive_guard_refused(nd, __LINE__); break; }
        g_emit.op_sa = sa; g_emit.op_sb = 1;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN: {
        g_emit.op_sb = 0;
        IR_t * enter_nd = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = nd_slot(enter_nd);
        if (g_emit.op_off < 0) { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_TAB: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_guard_refused(nd, __LINE__); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) sl = nd_slot(a0); if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MOVE: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_guard_refused(nd, __LINE__); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) sl = nd_slot(a0); if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_POS: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_guard_refused(nd, __LINE__); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) sl = nd_slot(a0); if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_UPTO: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) sl = nd_slot(a0); if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_ANY: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) sl = nd_slot(a0); if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MANY: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { g_emit.op_name1 = NULL; g_emit.op_sa = -1; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_FIND: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        int64_t _fl = (a0 && a0->op == IR_LIT_STRING && IR_LIT(a0).sval) ?
            ((a0->n_operands > 0 && a0->operands[0] && a0->operands[0]->op == IR_LIT_INTEGER) ? IR_LIT(a0->operands[0]).ival : (int64_t)strlen(IR_LIT(a0).sval)) : -1;
        if (_fl >= 1 && _fl <= 32) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) sl = nd_slot(a0); if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MATCH: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && a0->op == IR_LIT_STRING && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) sl = nd_slot(a0); if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_BAL: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) sl = nd_slot(a0); if (sl < 0) { drive_guard_refused(nd, __LINE__); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_guard_refused(nd, __LINE__); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REPALT: {
        drive_guard_refused(nd, __LINE__); break;
    }
    case IR_FAIL:
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_RETURN:
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    default:
        drive_no_template(nd); break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flat_trivial_beta(const IR_t *nd) {
    switch (nd->op) {
    case IR_MATCH_POS: case IR_MATCH_RPOS: return 1;
    case IR_MATCH_ABORT: return IR_LIT(nd).ival != 1;
    case IR_MATCH_ASSIGN_SAVE: { long _k = 0; return fc_geom(nd, &_k) && _k > 0; }
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flat_unwind_beta(const IR_t *nd) {
    switch (nd->op) {
    case IR_VAR: case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_ATOM: case IR_BINOP: case IR_UNOP: case IR_ASSIGN: case IR_ASSIGN_VAR:
    case IR_CMP_TEST: case IR_COERCE_NUMERIC: case IR_COERCE_STRING: case IR_COERCE_INTEGER: case IR_COERCE_REAL:
    case IR_IDENT: case IR_DIFFER:
    case IR_DEREF: case IR_SUBSCRIPT: case IR_FIELD_VAR: case IR_FIELD_GET: return 1;
    case IR_MATCH_ALTERNATE: return 1;
    case IR_MATCH_ARBNO: return 1;
    default: return flat_trivial_beta(nd);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int port_sz_beta(const char * sz) { return sz[0] == (char)0xce && (unsigned char)sz[1] == 0xb2; }
int beta_is_stmt_land(const IR_t * tgt) { return tgt && tgt->op == IR_STATEMENT_BEGIN; }
int emit_glue_box(const IR_t * t, int phi) { int o = (int)t->op;
    return ((o == IR_MATCH_ALTERNATE || o == IR_MATCH_ARBNO || o == IR_MATCH_FENCE1 || o == IR_SCAN_ALTERNATE || o == IR_DISJUNCTION) && t->n_operands > 0) ||
        (phi && o == IR_MATCH_BEGIN); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_graph_has_deep_arrival(IR_graph_t *g);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_reentry(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_reentry(void) { if (g_emitter.sn4_arbno_reentry_v < 0) { const char * e = getenv("SCRIP_ARBNO_REENTRY"); g_emitter.sn4_arbno_reentry_v = (e && *e == '1') ? 1 : 0;
    } return g_emitter.sn4_arbno_reentry_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cap_fail_retreat(void) {
     if (g_emitter.cap_fail_retreat__cf < 0) { const char * e = getenv("SCRIP_CAP_FAIL_RETREAT"); g_emitter.cap_fail_retreat__cf = (e && *e == '0') ? 0 : 1; } return g_emitter.cap_fail_retreat__cf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int blob_patv_reader(const IR_t * nd) { const char * sv = (nd && nd->op == IR_VAR) ? IR_LIT(nd).sval : (const char *)0; const char * d = sv ? strstr(sv, "$V") : (const char *)0;
    if (!d || strncmp(sv, "PAT$", 4)) return 0; char * e = (char *)0; (void) strtol(d + 2, &e, 10); return (e && e != d + 2 && !*e) ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int * xci_ints(cv_t * v, uint32_t n) { if (v->cap < n) { uint32_t c = v->cap ? v->cap : 256; while (c < n) c *= 2; v->p = ct_alloc((size_t)c * sizeof(int)); v->cap = c;
    v->esz = (uint32_t)sizeof(int); } v->len = n; return (int *)v->p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int blob_frame_scope(void);
int sn4_alt_carrier(void);
void sn4_blob_choice_scan(int * n_choice, int * leaf_ok, int * has_fence);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_blob_casmark(void) { if (g_emitter.sn4_blob_casmark_v < 0) { const char * e = getenv("SCRIP_BLOB_CASMARK"); g_emitter.sn4_blob_casmark_v = (e && *e == '0') ? 0 : 1;
    } return g_emitter.sn4_blob_casmark_v; }
static int blob_head_bytes(void) { return sn4_blob_casmark() ? 56 : 40; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_defer_resume(void) {
     if (g_emitter.sn4_defer_resume__dres < 0) { const char * e = getenv("SCRIP_DEFER_RESUME"); g_emitter.sn4_defer_resume__dres = (e && *e == '0') ? 0 : 1; } return g_emitter.sn4_defer_resume__dres;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_altsib(void) { if (g_emitter.sn4_arbno_altsib_v < 0) { const char * e = getenv("SCRIP_ARBNO_ALTSIB"); g_emitter.sn4_arbno_altsib_v = (e && *e == '0') ? 0 : 1;
    } return g_emitter.sn4_arbno_altsib_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_seal_omega(void) {
    const char * e = getenv("SCRIP_ARBNO_SEAL_OMEGA"); return (e && *e == '0') ? 0 : 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_tailbeta(void) {
     if (g_emitter.sn4_arbno_tailbeta__atb < 0) { const char * e = getenv("SCRIP_ARBNO_TAILBETA"); g_emitter.sn4_arbno_tailbeta__atb = (e && *e == '0') ? 0 : 1;
         } return g_emitter.sn4_arbno_tailbeta__atb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_seam_walk(void) {
     if (g_emitter.sn4_seam_walk__sw < 0) { const char * e = getenv("SCRIP_SEAM_WALK"); g_emitter.sn4_seam_walk__sw = (e && *e == '0') ? 0 : 1; } return g_emitter.sn4_seam_walk__sw;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_alt_carrier(void) {
     if (g_emitter.sn4_alt_carrier__ac < 0) { const char * e = getenv("SCRIP_ALT_CARRIER"); g_emitter.sn4_alt_carrier__ac = (e && *e == '0') ? 0 : 1; } return g_emitter.sn4_alt_carrier__ac;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sn4_blob_choice_scan(int * n_choice, int * leaf_ok, int * has_fence) {
    int _nc = 0, _lf = 1, _fn = 0;
    if (g_emit_cfg && g_emitter.g_bcs_g == g_emit_cfg && g_emitter.g_bcs_all == g_emit_cfg->all && g_emitter.g_bcs_n == g_emit_cfg->n && g_emitter.g_bcs_ep == g_emitter.g_xci_epoch) { _nc =
        g_emitter.g_bcs_nc; _lf = g_emitter.g_bcs_lf; _fn = g_emitter.g_bcs_fn; }
    else if (g_emit_cfg) { for (int j = 0; j < g_emit_cfg->n; j++) { IR_t * m = g_emit_cfg->all[j]; if (!m) continue; int o = (int)m->op;
        if (o == IR_MATCH_ALTERNATE || o == IR_DISJUNCTION) { if (m->seal) _lf = 0; else _nc++; continue; }
        if ((o == IR_MATCH_FENCE1 || o == IR_MATCH_FENCE0)) { _fn = 1; continue; }
        if (o == IR_SUCCEED || o == IR_FAIL) continue;
        { extern int zdp_scan_pure(const IR_t *); if (!zdp_scan_pure(m)) _lf = 0; } }
        g_emitter.g_bcs_g = g_emit_cfg; g_emitter.g_bcs_all = g_emit_cfg->all; g_emitter.g_bcs_n = g_emit_cfg->n; g_emitter.g_bcs_ep = g_emitter.g_xci_epoch; g_emitter.g_bcs_nc = _nc;
            g_emitter.g_bcs_lf = _lf; g_emitter.g_bcs_fn = _fn; }
    if (n_choice) *n_choice = _nc; if (leaf_ok) *leaf_ok = _lf; if (has_fence) *has_fence = _fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int resume_sealed_defer_on_root(IR_t ** nodes, int n, IR_t * br) {
    for (int i = 0; i < n; i++) { IR_t * x = nodes[i];
        if (!x || (int) x->op != IR_MATCH_DEFER || x->seal != 1) continue;
        if (x == br) return 1;
        if (br) for (int q = 0; q < br->n_operands; q++) if (br->operands[q] == x) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int resume_carrier_ok(const IR_t * nd, int sealed_defer) { extern int zdp_seam_tier(const IR_t *);
    if (zdp_seam_tier(nd) == 1) return 1;
    if (zdp_seam_tier(nd) == 3) { int _nc = 0, _lf = 0, _fn = 0; sn4_blob_choice_scan(&_nc, &_lf, &_fn); return (_nc == 1 && !sealed_defer && !nd->seal) ? 1 : 0; }
    return zdp_seam_tier(nd) == 2 ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int blob_frame_scope(void) {
    extern int g_flat_frame_floor;
    return (g_emit.flat_jmp_entry && g_emit.flat_pat && !(g_flat_frame_floor > 0)) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int blob_wire_clobber_scan(void) {
    if (!g_emit_cfg) return 0;
    for (int j = 0; j < g_emit_cfg->n; j++) { IR_t * m = g_emit_cfg->all[j]; if (!m) continue; int o = (int)ir_kind0(m);
        if (o == IR_MATCH_DEFER || o == IR_CALL || o == IR_CALL_VALUE || o == IR_MATCH_VALUE || o == IR_MATCH_CALLOUT) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int blob_carve_pad(void) { if (g_emitter.blob_carve_pad_v < 0) { const char * e = getenv("SCRIP_BLOB_CARVE_PAD"); g_emitter.blob_carve_pad_v = e ? 16 * atoi(e) : 0;
    } return g_emitter.blob_carve_pad_v; }
int blob_frame_bytes(void) {
    if (!blob_frame_scope() || !g_emit_cfg) return 0;
    int _pend = 0; if (sn4_blob_casmark()) for (int _i = 0; _i < g_emit_cfg->n; _i++) { IR_t * _m = g_emit_cfg->all[_i]; if (_m && _m->op == IR_MATCH_ASSIGN_COND) { _pend = 1; break; } }
    int _sb = planner_blob_slot_bytes();
    if (_sb == 0 && !_pend && !blob_wire_clobber_scan() && planner_blob_exit_live(g_emit_cfg) >= 0) return 0;
    return blob_head_bytes() + _sb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int blob_carve_bytes(void) { int b = blob_frame_bytes(); return b > 0 ? b + 16 : 0; }
extern "C" int zls_g_layout_q(const IR_graph_t *, uint64_t *, int, int *, int *); extern "C" int zls_g_region(const IR_graph_t *);
static void blob_lay_push(int off, unsigned kind, int size) { CV_PUSH(g_emitter.g_blob_lay, uint64_t) = GC_LAY_Q(off, kind, size); }
static void blob_layout_slot(const IR_t * m, int slot, int units, int cls) {
    (void)m;
    if (cls == ACT_CLASS_FENCE) { blob_lay_push(slot, GC_LAY_RAW, 16); blob_lay_push(slot + 16, GC_LAY_RAW, 16); return; }
    if (cls == ACT_CLASS_ARBNO || cls == ACT_CLASS_CAPTURE) { blob_lay_push(slot, GC_LAY_RAW, 16); return; }
    if (cls == ACT_CLASS_REGEX) { blob_lay_push(slot, GC_LAY_RAW, 16 * units); return; }
    blob_lay_push(slot, GC_LAY_DESCR, 16);
}
static int blob_lay_cmp(const void * a, const void * b) { int x = GC_LAY_OFF(*(const uint64_t *)a), y = GC_LAY_OFF(*(const uint64_t *)b); return x < y ? -1 : (x > y ? 1 : 0); }
int blob_cont_copy(void) { if (g_emitter.blob_cont_copy_v < 0) { const char * e = getenv("SCRIP_BLOB_CONT_COPY"); g_emitter.blob_cont_copy_v = (e && *e == '1') ? 1 : 0;
    } return g_emitter.blob_cont_copy_v; }
static int blob_zf_gc_only(void) { if (g_emitter.blob_zf_gc_only_v < 0) { const char * e = getenv("SCRIP_BLOB_ZF_GC_ONLY"), * d = getenv("SCRIP_DIAG");
    g_emitter.blob_zf_gc_only_v = e ? (*e == '1') : (d && *d == '0'); } return g_emitter.blob_zf_gc_only_v; }
static int blob_zf_rep(void) { if (g_emitter.blob_zf_rep_v < 0) { const char * e = getenv("SCRIP_BLOB_ZF_REP"); g_emitter.blob_zf_rep_v = (e && *e == '1') ? 1 : 0; } return g_emitter.blob_zf_rep_v; }
static int blob_omega_ret(void) { if (g_emitter.blob_omega_ret_v < 0) { const char * e = getenv("SCRIP_BLOB_OMEGA_RET"); g_emitter.blob_omega_ret_v = (e && *e == '1') ? 1 : 0;
    } return g_emitter.blob_omega_ret_v; }
static std::string blob_zero_fill(int bfb) {
    int top = sn4_blob_casmark() ? -32 : -24, o = -bfb;
    if (top - o <= 0) return std::string();
    if (blob_zf_rep() || top - o > 1200) return x86("lea", "rdi", RDQ("rbp", o)) + x86("xor", "eax", "eax") + x86("mov32", "ecx", (long)(top - o)) + x86("rep_stosb");
    if (blob_zf_gc_only()) { std::string t; int any = 0;
        for (uint32_t i = 0; i < g_emitter.g_blob_lay.len; i++) { uint64_t q = CV_AT(g_emitter.g_blob_lay, uint64_t, i); unsigned k = GC_LAY_KIND(q); int lo = GC_LAY_OFF(q), sz = GC_LAY_SIZE(q);
            if ((k != GC_LAY_DESCR && k != GC_LAY_PTR_GC) || lo < o || lo + sz > top) continue;
            if (!any) { t += x86("xorps", "xmm0", "xmm0") + x86("xor", "eax", "eax"); any = 1; }
            if (sz == 16) t += x86("movups", RDQ("rbp", lo), "xmm0"); else for (int w = lo; w + 8 <= lo + sz; w += 8) t += x86("mov", RDQ("rbp", w), "rax"); }
        return t; }
    std::string s = x86("xorps", "xmm0", "xmm0");
    for (; o + 16 <= top; o += 16) s += x86("movups", RDQ("rbp", o), "xmm0");
    if (o < top) s += x86("xor", "eax", "eax") + x86("mov", RDQ("rbp", o), "rax");
    return s;
}
static int blob_fg_on(void) { if (g_emitter.blob_fg_on_v < 0) { const char * e = getenv("SCRIP_BLOB_FIRST_GUARD"); g_emitter.blob_fg_on_v = (e && *e == '0') ? 0 : 1; } return g_emitter.blob_fg_on_v; }
static blob_fg_bind_t * blob_fg_bind_find(const char * var) {
    for (uint32_t i = 0; i < g_emit.fg_bind.len; i++) { blob_fg_bind_t * e = &CV_AT(g_emit.fg_bind, blob_fg_bind_t, i); if (!strcmp(e->var, var)) return e; }
    return (blob_fg_bind_t *)0;
}
static void blob_fg_bind_build(void) {
    extern stage2_t g_stage2;
    g_emit.fg_bind_built = 1; g_emit.fg_bind.len = 0;
    for (int gi = 0; gi < g_stage2.bbp.count; gi++) { IR_graph_t * g = g_stage2.bbp.table[gi]; if (!g || !g->all) continue;
        for (int i = 0; i < g->n; i++) { IR_t * x = g->all[i]; if (!x || x->op != IR_ASSIGN || !IR_LIT(x).sval || x->n_operands < 1 || !x->operands[0]) continue;
            IR_t * v = x->operands[0]; const char * t = (const char *)0, * f = (const char *)0;
            if (ir_norm_call_kind(v->op) == IR_CALL && IR_LIT(v).sval && !strcmp(IR_LIT(v).sval, "SNO$MKPAT") && v->n_operands >= 1 && v->operands[0] && v->operands[0]->op == IR_LIT_STRING &&
                IR_LIT(v->operands[0]).sval && !strncmp(IR_LIT(v->operands[0]).sval, "PAT$", 4)) t = IR_LIT(v->operands[0]).sval;
            else if (v->op == IR_VAR && IR_LIT(v).sval) f = IR_LIT(v).sval;
            else continue;
            blob_fg_bind_t * e = blob_fg_bind_find(IR_LIT(x).sval);
            if (!e) { e = &CV_PUSH(g_emit.fg_bind, blob_fg_bind_t); e->var = IR_LIT(x).sval; e->thunk = t; e->from = f; e->conflict = 0; continue; }
            if ((t && e->thunk && strcmp(t, e->thunk)) || (f && e->from && strcmp(f, e->from)) || (t && e->from) || (f && e->thunk)) e->conflict = 1;
            if (t && !e->thunk) e->thunk = t; if (f && !e->from) e->from = f; } }
}
static const char * blob_fg_bound_thunk(const char * var) {
    if (!g_emit.fg_bind_built) blob_fg_bind_build();
    for (int hop = 0; hop < 8 && var; hop++) { blob_fg_bind_t * e = blob_fg_bind_find(var); if (!e || e->conflict) return (const char *)0; if (e->thunk) return e->thunk; var = e->from; }
    return (const char *)0;
}
static IR_graph_t * blob_fg_thunk_graph(const char * name) {
    extern stage2_t g_stage2;
    for (int i = 0; i < g_stage2.proc_count; i++) { ProcEntry * pe = &g_stage2.proc_table[i];
        if (pe->name && !strcmp(pe->name, name) && pe->bb_idx >= 0 && pe->bb_idx < g_stage2.bbp.count) return g_stage2.bbp.table[pe->bb_idx]; }
    return (IR_graph_t *)0;
}
static void blob_fg_seq(const blob_fg_t * a, const blob_fg_t * cont, blob_fg_t * out) {
    *out = *a; if (!a->nempty) return;
    for (int i = 0; i < 256; i++) out->set[i] |= cont->set[i];
    out->nempty = a->nempty * cont->nempty > 2 ? 2 : a->nempty * cont->nempty; out->impure |= cont->impure; out->unknown |= cont->unknown;
}
static void blob_fg_from(const IR_t * n, const IR_t * stop, const blob_fg_t * cont, blob_fg_t * out, int depth, blob_fg_deps_t * deps) {
    static const blob_fg_t eps = { {0}, 1, 0, 0 };
    memset(out, 0, sizeof *out);
    if (!n || depth > 64) { out->unknown = 1; return; }
    if (n == stop) { *out = *cont; return; }
    if (n->seal || (n->ω.node && n->ω.node->op == IR_MATCH_ABORT)) { out->unknown = 1; return; }
    const char * s = IR_LIT(n).sval; int lit = (n->n_operands == 0 && !n->pat_static && s);
    switch ((int)n->op) {
    case IR_MATCH_LIT: if (!lit) { out->unknown = 1; return; } if (!s[0]) { blob_fg_from(n->γ.node, stop, cont, out, depth + 1, deps); return; } out->set[(unsigned char)s[0]] = 1; return;
    case IR_MATCH_ANY: case IR_MATCH_SPAN: if (!lit) { out->unknown = 1; return; } for (const char * p = s; *p; p++) out->set[(unsigned char)*p] = 1; return;
    case IR_MATCH_NOTANY: if (!lit) { out->unknown = 1; return; } for (int i = 0; i < 256; i++) out->set[i] = 1; for (const char * p = s; *p; p++) out->set[(unsigned char)*p] = 0; return;
    case IR_MATCH_POS: case IR_MATCH_RPOS: case IR_MATCH_ASSIGN_SAVE: case IR_MATCH_ASSIGN_COND:
        blob_fg_from(n->γ.node, stop, cont, out, depth + 1, deps); if (out->nempty) out->impure = 1; return;
    case IR_MATCH_ALTERNATE: { blob_fg_t after, a; if (n->n_operands < 2 || (n->n_operands & 1)) { out->unknown = 1; return; } blob_fg_from(n->γ.node, stop, cont, &after, depth + 1, deps);
        for (int k = 0; k + 1 < n->n_operands; k += 2) { blob_fg_from(n->operands[k], n, &after, &a, depth + 1, deps); for (int i = 0; i < 256; i++) out->set[i] |= a.set[i];
            out->nempty = out->nempty + a.nempty > 2 ? 2 : out->nempty + a.nempty; out->impure |= a.impure; out->unknown |= a.unknown; } return; }
    case IR_MATCH_FENCE1: { blob_fg_t after, in; if (n->n_operands < 1) { out->unknown = 1; return; } blob_fg_from(n->γ.node, stop, cont, &after, depth + 1, deps);
        blob_fg_from(n->operands[0], n, &eps, &in, depth + 1, deps); if (in.nempty > 1) in.nempty = 1; blob_fg_seq(&in, &after, out); return; }
    case IR_MATCH_DEFER: {
        extern int g_gva_active; extern int gva_index_of(const char *);
        const char * th = (s && s[0] != '*' && g_gva_active && gva_index_of(s) >= 0) ? blob_fg_bound_thunk(s) : (const char *)0;
        IR_graph_t * tg = th ? blob_fg_thunk_graph(th) : (IR_graph_t *)0;
        if (!tg || !tg->entry || !deps || deps->var.len >= 4) { out->unknown = 1; return; }
        { int seen = 0; for (uint32_t i = 0; i < deps->var.len; i++) if (!strcmp(CV_AT(deps->var, const char *, i), s)) seen = 1; if (!seen) { CV_PUSH(deps->var, const char *) = s;
            CV_PUSH(deps->thunk, const char *) = th; } }
        { blob_fg_t sub, after; blob_fg_from(tg->entry, (const IR_t *)0, &eps, &sub, depth + 1, deps); blob_fg_from(n->γ.node, stop, cont, &after, depth + 1, deps); blob_fg_seq(&sub, &after, out); }
        return; }
    case IR_SUCCEED: out->nempty = 1; return;
    default: out->unknown = 1; return;
    }
}
static int blob_alt_guard_on(void) { if (g_emitter.blob_alt_guard_on_v < 0) { const char * e = getenv("SCRIP_ALT_FIRST_GUARD"); g_emitter.blob_alt_guard_on_v = (e && *e == '0') ? 0 : 1;
    } return g_emitter.blob_alt_guard_on_v; }
const char * blob_alt_first(const IR_t * nd) {
    extern int sn4_cset32(void);
    if (!blob_alt_guard_on() || sn4_cset32() || !nd || nd->seal || nd->n_operands < 4 || (nd->n_operands & 1)) return (const char *)0;
    static const blob_fg_t eps = { {0}, 1, 0, 0 };
    blob_fg_t u; memset(&u, 0, sizeof u);
    for (int k = 0; k + 1 < nd->n_operands; k += 2) { blob_fg_t a; blob_fg_from(nd->operands[k], nd, &eps, &a, 0, (blob_fg_deps_t *)0);
        if (a.unknown || a.nempty) return (const char *)0;
        for (int i = 0; i < 256; i++) u.set[i] |= a.set[i]; }
    if (u.set[0]) return (const char *)0;
    int k = 0; char * set = (char *)ct_alloc(256); for (int i = 1; i < 256; i++) if (u.set[i]) set[k++] = (char)i; set[k] = 0;
    return (k > 0 && k < 200) ? set : (const char *)0;
}
static int blob_fg_bind_on(void) { if (g_emitter.blob_fg_bind_on_v < 0) { const char * e = getenv("SCRIP_BLOB_FIRST_BIND"); g_emitter.blob_fg_bind_on_v = (e && *e == '0') ? 0 : 1;
    } return g_emitter.blob_fg_bind_on_v; }
static void blob_first_guard(void) {
    extern int sn4_cset32(void); extern int g_gva_active; extern int gva_index_of(const char *);
    if (!blob_fg_on() || sn4_cset32() || !g_emit_cfg || !g_emit_cfg->entry) return;
    static const blob_fg_t eps = { {0}, 1, 0, 0 };
    blob_fg_t r; blob_fg_deps_t deps; memset(&deps, 0, sizeof deps);
    blob_fg_from(g_emit_cfg->entry, (const IR_t *)0, &eps, &r, 0, &deps);
    int fast_empty = (r.nempty == 1 && !r.impure);
    if (r.unknown || r.set[0] || (r.nempty && !fast_empty) || (deps.var.len && !blob_fg_bind_on())) return;
    int k = 0; char * set = (char *)ct_alloc(256); for (int i = 1; i < 256; i++) if (r.set[i]) set[k++] = (char)i; set[k] = 0;
    if (k == 0 && !fast_empty) return;
     int id = g_emitter.blob_first_guard_seq++;
    bb_label_t * none = emit_label_alloc(".Lfg_none_%d", id); bb_label_t * ok = emit_label_alloc(".Lfg_ok_%d", id); bb_label_t * fire = emit_label_alloc(".Lfg_fire_%d", id);
    bb_emit_x86(x86("cmp", "r14d", "r15d")); emit_jmp_label(deps.var.len ? fire : none, JMP_JGE);
    if (k > 0) {
        bb_emit_x86(x86("movsxd", "rcx", "r14d") + x86("movzx", "eax", "[r13+rcx]") + x86("lea", "rcx", "[rip + __]", x86_csettab_ptr(set), x86_csettab_lbl(set).c_str()) +
            x86("movzx", "eax", "[rcx+rax]") + x86("test", "eax", "eax"));
        emit_jmp_label(ok, JMP_JNE);
    }
    emit_label_define_bb(fire);
    for (uint32_t i = 0; i < deps.var.len; i++) {
        const char * dv = CV_AT(deps.var, const char *, i), * dt = CV_AT(deps.thunk, const char *, i); int gk = gva_index_of(dv);
        bb_label_t * q = emit_label_alloc(".Lfg_rec_%d_%u", id, i), * over = emit_label_alloc(".Lfg_over_%d_%u", id, i);
        bb_emit_x86(x86("mov", "rax", (g_runtime.abi.rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 0) : ABSQ(RT_GVA_VA + gk * 16)) + x86("cmp", "al", (long)DT_P));
        emit_jmp_label(ok, JMP_JNE);
        bb_emit_x86(x86("mov", "rax", (g_runtime.abi.rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 8) : ABSQ(RT_GVA_VA + gk * 16 + 8)) + x86("mov", "rax", RDQ("rax", 0)) + x86_lea_ext("rcx", q) +
            x86("mov", "rcx", RDQ("rcx", 0)) + x86("mov", "rcx", RDQ("rcx", 0)) + x86("cmp", "rax", "rcx"));
        emit_jmp_label(ok, JMP_JNE);
        emit_jmp_label(over, JMP_JMP);
        { extern const char * bb_ab_sym_name(const char *); std::string lbl = std::string(".Lthk_") + bb_ab_sym_name(dt); void * rec = bb_thunk_rec_addr(dt);
            if (g_is_text) emit_textf("  .pushsection .data.rel.ro,\"aw\",@progbits; .p2align 3; %s: .quad %s; .popsection\n", q->name, lbl.c_str());
            else { emit_label_define_bb(q); bb_emit_x86(x86(".quad", lbl.c_str(), rec ? (const char *)rec : "")); } }
        emit_label_define_bb(over);
    }
    emit_label_define_bb(none);
    if (!fast_empty) bb_emit_x86(blob_omega_ret() ? x86("add", "rsp", 8L) + x86("ret") : x86_rsp_load64("rcx", 8) + x86("add", "rsp", 16L) + x86("jmp", "rcx"));
    else {
        bb_label_t * stub = emit_label_alloc(".Lfg_stub_%d", id);
        bb_emit_x86(x86_rsp_load64("rax", 0) + x86_rsp_load64("rcx", 8) + x86("push", "rbp") + x86("push", "rcx") + x86("push", "rax") + x86_lea_ext("rcx", stub) + x86("push", "rcx") +
            x86("jmp", "rax"));
        emit_label_define_bb(stub);
        bb_emit_x86(x86_rsp_load64("rbp", 24) + x86_rsp_load64("rcx", 16) + x86("add", "rsp", 48L) + x86("jmp", "rcx"));
    }
    emit_label_define_bb(ok);
}
static int blob_layout_build(int bfb) {
    g_emitter.g_blob_lay.len = 0;
    blob_lay_push(0, GC_LAY_RAW, 8); blob_lay_push(8, GC_LAY_PTR_CODE, 8); blob_lay_push(16, GC_LAY_PTR_CODE, 8);
    blob_lay_push(-8, GC_LAY_PTR_CODE, 8); blob_lay_push(-16, GC_LAY_PTR_CODE, 8); blob_lay_push(-32, GC_LAY_DESCR, 16);
    if (sn4_blob_casmark()) { blob_lay_push(-40, GC_LAY_RAW, 8); blob_lay_push(-48, GC_LAY_RAW, 8); } else blob_lay_push(-40, GC_LAY_RAW, 8);
    planner_act_visit(blob_layout_slot);
    blob_lay_push(-(blob_head_bytes() + planner_blob_slot_bytes()), GC_LAY_RAW, 8);
    qsort(g_emitter.g_blob_lay.p, (size_t)g_emitter.g_blob_lay.len, sizeof(uint64_t), blob_lay_cmp);
    { int expect = -bfb; for (int i = 0; i < (int)g_emitter.g_blob_lay.len; i++) { int o = GC_LAY_OFF(CV_AT(g_emitter.g_blob_lay, uint64_t, i));
        if (o != expect)
        { fprintf(stderr,
        "FATAL blob_layout_build: graph %s entry %d at rbp%+d, expected rbp%+d -- the static layout does not tile the blob frame [%d, 24) and a layout that does not cover its frame is a guess (ARCH-"
        "GC section 6.2e)\n", g_emit.flat_fam ? g_emit.flat_fam : "?", i, o, expect, -bfb); abort(); } expect += GC_LAY_SIZE(CV_AT(g_emitter.g_blob_lay, uint64_t, i)); }
      if (expect != 24) { fprintf(stderr, "FATAL blob_layout_build: graph %s layout ends at rbp%+d, expected rbp+24 (frame [%d, 24))\n", g_emit.flat_fam ? g_emit.flat_fam : "?", expect, -bfb);
          abort(); } }
    return (int)g_emitter.g_blob_lay.len;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_diag_regs_suppress(void) {
    return (g_emit.flat_jmp_entry && g_emit.flat_pat && (!blob_frame_scope() || !g_emit_cfg)) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int x86_diag_regs_on_c(void) { if (g_emitter.x86_diag_regs_on_c_v < 0) { const char * e = getenv("SCRIP_DIAG_REGS"); g_emitter.x86_diag_regs_on_c_v = (e && *e && *e != '0') ? 1 : 0;
    } return g_emitter.x86_diag_regs_on_c_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static zdo_slot_t *zdo_probe(const IR_t *k) { uint64_t h = (((uint64_t)(uintptr_t)k >> 4) * 11400714819323198485ull) & (g_emitter.g_zdo.len - 1);
    for (;;) { zdo_slot_t *q = &CV_AT(g_emitter.g_zdo, zdo_slot_t, h); if (q->gen != g_emitter.g_zdo_gen || q->k == k) return q; h = (h + 1) & (g_emitter.g_zdo.len - 1); } }
void zdo_build(IR_t **nodes, int n) {
    g_emitter.g_zdo_gen++; g_emitter.g_zdo_cnt = 0; g_emitter.g_zdo_nodes = nodes; g_emitter.g_zdo_n = n;
    uint32_t need = 64; while (need < (uint32_t)n * 2 + 2) need <<= 1;
    if (g_emitter.g_zdo.len < need) { if (g_emitter.g_zdo.p) ct_drop(g_emitter.g_zdo.p); g_emitter.g_zdo.p = ct_zalloc(need, sizeof(zdo_slot_t)); g_emitter.g_zdo.len = need;
        g_emitter.g_zdo.cap = need; g_emitter.g_zdo.esz = (uint32_t)sizeof(zdo_slot_t); }
    for (int k = 0; k < n; k++) if (zd_omega_test_kind(nodes[k])) { IR_t *t = zd_chase(nodes[k]->ω.node); if (!t) continue; zdo_slot_t *q = zdo_probe(t);
        if (q->gen != g_emitter.g_zdo_gen) { q->k = t;
        q->v = k; q->gen = g_emitter.g_zdo_gen; g_emitter.g_zdo_cnt++; } }
}
int zdo_first(IR_t **nodes, int n, IR_t *t) {
    if (t && nodes == g_emitter.g_zdo_nodes && n == g_emitter.g_zdo_n && g_emitter.g_zdo.len) { zdo_slot_t *q = zdo_probe(t); return q->gen == g_emitter.g_zdo_gen ? q->v : -1; }
    for (int k = 0; k < n; k++) if (zd_omega_test_kind(nodes[k]) && zd_chase(nodes[k]->ω.node) == t) return k; return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static zrm_slot_t * zrm_probe(const IR_t * nd) {
    uint32_t m = g_emitter.g_zrm_map.len - 1; uint64_t h = (((uint64_t)(uintptr_t)nd >> 4) * 0xff51afd7ed558ccdull) & m;
    for (;;) { zrm_slot_t * t = &CV_AT(g_emitter.g_zrm_map, zrm_slot_t, h); if (t->gen != g_emitter.g_zrm_gen || t->nd == nd) return t; h = (h + 1) & m; }
}
void zrm_build(IR_t ** nodes, const int * run, int rl) {
    g_emitter.g_zrm_gen++;
    if ((uint64_t)g_emitter.g_zrm_map.len < (uint64_t)rl * 2 + 2) { uint32_t c = 256; while ((uint64_t)c < (uint64_t)rl * 2 + 2) c *= 2; cv_t v = { 0, 0, 0, 0 };
        v.p = ct_zalloc(c, sizeof(zrm_slot_t));
        v.len = c; v.cap = c; v.esz = (uint32_t)sizeof(zrm_slot_t); g_emitter.g_zrm_map = v; }
    for (int k = 0; k < rl; k++) { zrm_slot_t * t = zrm_probe(nodes[run[k]]); t->nd = nodes[run[k]]; t->gen = g_emitter.g_zrm_gen; t->maxk = k; }
}
int zrm_maxk(const IR_t * nd) { zrm_slot_t * t = zrm_probe(nd); return t->gen == g_emitter.g_zrm_gen ? t->maxk : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int bb_proc_target_retains(const char *fname);
extern "C" int zls_g_vslot_count(const IR_graph_t *); extern "C" const char * zls_g_vslot_get(const IR_graph_t *, int, int *); extern "C" int zls_g_block_base(const IR_graph_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flat_beta_kind_keeps(IR_t * nd) { int op = nd ? (int)ir_kind0(nd) : -1;
    return (nd &&
    (ir_is_generator_kind(ir_kind0(nd)) || op == IR_SUSPEND || op == IR_CALL || op == IR_CALL_PROC_STAGED || op == IR_CALL_BUILTIN_GEN || op == IR_PROC_GEN || op == IR_REPALT || op == IR_LIMIT ||
    op == IR_GOTO || op == IR_STATEMENT_BEGIN || (g_emit_cfg && nd == g_emit_cfg->body_root))) ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_body_beta_keeps(IR_t * nd) { int op = nd ? (int)nd->op : -1; return (nd && (flat_beta_kind_keeps(nd) || op == IR_SCAN_TAB || op == IR_SCAN_MOVE)) ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scan_conduit_enters_a_do_clause(IR_t ** nodes, int n, IR_t * c) {
    for (int j = 0; j < n; j++) { IR_t * s = nodes[j]; if (!s || s->op != IR_SUSPEND || s->seal != 1 || s->n_operands < 2) continue;
        IR_t * x = s->operands[1]; int h = 0;
        while (x && x->op == IR_SCAN && x->n_operands > 2 && !x->operands[1] && x->operands[2] && x->operands[2]->op == IR_SCAN && x->operands[2]->n_operands > 2 && !x->operands[2]->operands[1] &&
            h++ < 64) x = x->operands[2];
        if (x == c) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_beta_used_scan(IR_t **nodes, int n, unsigned char *used) {
    for (int j = 0; j < n; j++) if (fc_seq_on(nodes[j]) || fc_alt_active(nodes[j])) { for (int k = 0; k < n; k++) used[k] = 1; return; }
    for (int k = 0; k < n; k++) {
        used[k] = flat_beta_kind_keeps(nodes[k]);
    }
    for (int j = 0; j < n; j++) {
        int op = (int)nodes[j]->op;
        if (op == IR_MATCH_ALTERNATE || op == IR_MATCH_ARBNO || (op == IR_MATCH_FENCE1 || op == IR_MATCH_FENCE0) || op == IR_SCAN_ALTERNATE || op == IR_DISJUNCTION ||
            op == IR_REPALT)
            for (int a = 0; a < nodes[j]->n_operands; a++) { int k = nidx(nodes, n, nodes[j]->operands[a]); if (k >= 0) used[k] = 1; }
        if (op == IR_SCAN && nodes[j]->n_operands > 2 && nodes[j]->operands[2]) { int k = nidx(nodes, n, nodes[j]->operands[2]); if (k >= 0) used[k] = 1; }
        if (op == IR_GATE_ARM && nodes[j]->n_operands > 0 && nodes[j]->operands[0] && (int)nodes[j]->seal)
            { int k = nidx(nodes, n, nodes[j]->operands[0]); if (k >= 0) used[k] = 1; }
        int gib = port_sz_beta(nodes[j]->γ.sz);
        int oib = port_sz_beta(nodes[j]->ω.sz);
        IR_t *gtgt = nodes[j]->γ.node;
        IR_t *otgt = nodes[j]->ω.node;
        { int _gg = 0; IR_t * _g = gtgt; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!gib) gib = port_sz_beta(_g->γ.sz); gtgt = _g->γ.node; _g = gtgt; } }
        { int _gg = 0; IR_t * _o = otgt; while (_o && _o->op == IR_GOTO && _gg++ < 128) { if (!oib) oib = port_sz_beta(_o->γ.sz); otgt = _o->γ.node; _o = otgt; } }
        if (gib) { int k = nidx(nodes, n, gtgt); if (k >= 0) used[k] = 1; }
        if (oib) { int k = nidx(nodes, n, otgt); if (k >= 0) used[k] = 1; }
    }
}
extern "C" int xa_flat_class_c_pred(void);
extern "C" void xa_flat_chain_prologue(const char * fname, int * argkt_out);
extern "C" void xa_flat_chain_epilogue(void);
extern "C" void xa_flat_chain_epilogue_sig(int is_gamma, const char * fname);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * icn_trace_intern(const char * s) {
    if (!s) return s;
    return g_emitter.icn_trace_intern_pool.insert(std::string(s)).first->c_str();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string flat_zero_fill(int lo, int len) {
    if (len <= 0) return std::string();
    if (len > 1200 || (len & 7)) {
        std::string s = x86("mov", "rdi", "rsp") + x86("add", "rdi", (long)lo) + x86("xor", "eax", "eax");
        if (len >> 3) s += x86("mov32", "ecx", (long)(len >> 3)) + x86("rep_stosq");
        if (len & 7) s += x86("mov32", "ecx", (long)(len & 7)) + x86("rep_stosb");
        return s;
    }
    std::string s = x86("mov", "rdi", "rsp") + x86("xorps", "xmm0", "xmm0") + x86("xor", "eax", "eax");
    int o = lo;
    for (; o + 16 <= lo + len; o += 16) s += x86("movups", RDQ("rdi", o), "xmm0");
    if (o < lo + len) s += x86("mov", RDQ("rdi", o), "rax");
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string icn_entry_gva(void) {
    return x86("comment", "entry: r9 (the global-cell base the RTCC bank keeps live across Icon code) is reloaded from its home -- a C road (a generator "
                           "entered on its coroutine thread, a by-name or apply entry) hands over whatever C left in it, and no call precedes the first read")
         + x86_rtx_reestablish(RTCC_C_R9);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string icn_gen_line_save(void) {
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
static std::string icn_gen_line_resume(void) {
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
static std::string icn_main_args_inline(int np, int blk) {
    if (blk || np <= 0) return std::string();
    if (np > 10) return x86_bomb("emit: an Icon root graph with more than ten parameters has no inline argument load");
    std::string s =
        x86("comment", "the root graph keeps its parameters in its own frame: g_call_args[i] -> [rsp+16+16i] inline while i < cap (the frame was zeroed, so a short medium leaves a null DESCR)")
                  + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_call_args, "g_call_args")
                  + x86("mov", "ecx", RDD("rax", (int)offsetof(gv_t, cap)))
                  + x86("mov", "rax", RDQ("rax", (int)offsetof(gv_t, p)));
    for (int i = 0; i < np; i++)
        s += x86("cmp", "ecx", (long)i) + x86("jbe", L(220 + i))
           + x86("mov", "rdx", RDQ("rax", 16 * i)) + x86("mov", RDQ("rsp", 16 + 16 * i), "rdx")
           + x86("mov", "rdx", RDQ("rax", 16 * i + 8)) + x86("mov", RDQ("rsp", 24 + 16 * i), "rdx")
           + x86("def", L(220 + i));
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_register_local_offsets(const char * pname) {
    extern void rt_proc_set_locals(const char *, const char **, int);
    extern void rt_proc_set_pname(const char *, int, const char *);
    extern void rt_proc_set_local_offs(const char *, const int *, int);
    extern const char * zls_g_vslot_get(const IR_graph_t *, int, int *);
    extern int zls_g_vslot_count(const IR_graph_t *);
    if (!pname || !g_emit_cfg || !g_emit_cfg->lnames || g_emit_cfg->nlocals <= 0) return;
    rt_proc_set_locals(pname, g_emit_cfg->lnames, g_emit_cfg->nlocals);
    for (int k = 0; k < g_emit_cfg->nparams; k++) if (g_emit_cfg->pnames && g_emit_cfg->pnames[k]) rt_proc_set_pname(pname, k, g_emit_cfg->pnames[k]);
    { extern void rt_proc_set_loc_params(const char *, const char **, int);
      if (g_emit_cfg->pnames && g_emit_cfg->nparams > 0) {
          const char ** pc = (const char **) ct_alloc(sizeof(const char *) * (size_t) g_emit_cfg->nparams);
          if (pc) { for (int k = 0; k < g_emit_cfg->nparams; k++) pc[k] = g_emit_cfg->pnames[k] ? ct_strdup(g_emit_cfg->pnames[k]) : (const char *) 0;
              rt_proc_set_loc_params(pname, pc, g_emit_cfg->nparams); } } }
    int nv = zls_g_vslot_count(g_emit_cfg); if (nv <= 0) return;
    int nl = g_emit_cfg->nlocals;
    int * offs = (int *) ct_alloc(sizeof(int) * (size_t) nl); if (!offs) return;
    for (int k = 0; k < nl; k++) {
        offs[k] = -1;
        const char * ln = g_emit_cfg->lnames[k]; if (!ln) continue;
        for (int v = 0; v < nv; v++) { int off = -1; const char * vn = zls_g_vslot_get(g_emit_cfg, v, &off); if (vn && off >= 0 && !strcmp(vn, ln)) { offs[k] = off; break; } }
    }
    rt_proc_set_local_offs(pname, offs, nl);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string icn_trace_tap(const char * pname, int kind, int np, int aoff = 16) {
    if (!x86_trace_hooks_on()) return std::string();
    extern void rt_trace_call_hook_f(const char *, int, void *); extern void rt_trace_return_hook(const char *, DESCR_t); extern void rt_trace_fail_hook(const char *);
    extern void rt_trace_gen_fail_hook(const char *, void *);
    extern int g_flat_node_id;
    if (!pname) return std::string();
    pname = icn_trace_intern(pname);
    std::string id = std::to_string(g_flat_node_id++);
    std::string sk = (kind <= 3) ? "L24" + std::to_string(6 + kind) : "L" + std::to_string(235 + kind); std::string fl = ".Licn_trace_nm" + id;
    std::string s = x86("push", "rax") + x86("push", "rdx") + x86_align_call_enter()
        + IF(kind == 2 || kind == 3 || kind == 5, x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
        + x86("mov", "rax", RDQ("rax", GRT_AT(trace))) + x86("cmp", "rax", (long)0) + x86("je", sk))
        + x86("directive", ".section .rodata") + x86("directive", (fl + ": .string \"" + pname + "\"").c_str()) + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix")
        + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)pname, fl.c_str());
    if (kind == 1) s += x86("mov32", "esi", (long)np) + x86("lea", "rdx", RDQ("r11", aoff)) + x86("call", "rt_trace_call_hook_f", (uint64_t)(uintptr_t)(void *)rt_trace_call_hook_f);
    else if (kind == 2) s += x86("mov", "rsi", RDQ("r11", 8)) + x86("mov", "rdx", RDQ("r11", 0)) + x86("call", "rt_trace_return_hook", (uint64_t)(uintptr_t)(void *)rt_trace_return_hook);
    else if (kind == 5) s += x86("mov", "rsi", "rbp") + x86("call", "rt_trace_gen_fail_hook", (uint64_t)(uintptr_t)(void *)rt_trace_gen_fail_hook);
    else s += x86("call", "rt_trace_fail_hook", (uint64_t)(uintptr_t)(void *)rt_trace_fail_hook);
    s += x86_rt_gc_poll();
    s += x86("def", sk) + x86_align_call_leave() + x86("pop", "rdx") + x86("pop", "rax");
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int rt_gva_add_var(const char * name);
static int gc_sites_report_on(void) { if (g_emitter.gc_sites_report_on_v < 0) { const char * e = getenv("SCRIP_GC_SITES_REPORT"); g_emitter.gc_sites_report_on_v = (e && *e == '1') ? 1 : 0;
    } return g_emitter.gc_sites_report_on_v; }
static int gc_site_rule_pick(int adj, int * depth) {
    if (g_emit.flat_pat && blob_frame_bytes() > 0) { *depth = 0; return 3; }
    if (x86_hb_on()) { *depth = 0; return 1; }
    if (g_emit.flat_gen_rbp > 0) { *depth = g_emit.flat_gen_rbp; return 2; }
    if (adj == X86_ADJ_ALIGNED) { *depth = g_emit.op_zdepth + g_emit.op_vdj_live; return 4; }
    *depth = g_emit.op_zrun + g_emit.op_zxb + adj + g_emit.op_vdj_live;
    return 0;
}
extern "C++" int emit_gc_site_rsp_based(void) { int d; return !g_emit.site_shim9 && gc_site_rule_pick(0, &d) == 0; }
extern "C++" int emit_gc_site_new(int kind, int adj) {
    if (!g_emitter.g_gc_sites_armed) return -1;
    int rule, depth;
    rule = gc_site_rule_pick(adj, &depth);
    if (g_emit.site_shim9) { rule = 9; depth = (int)(g_emit.site_shim9 - 1); }
    gc_site_t * e = &CV_PUSH(g_emitter.g_gc_sites_v, gc_site_t); e->pc = 0; e->depth = depth; e->kind = (uint16_t)kind; e->rule = (uint16_t)rule; e->zdepth = g_emit.op_zdepth;
        e->fcb = (int32_t)g_emit.op_fc_bytes; e->fcbase = (int32_t)g_emit.op_fc_base; e->demit = depth; e->bk = x86_boxkind(); e->mn = 0; e->nd = g_emit.node;
    return (int)g_emitter.g_gc_sites_v.len - 1;
}
static void emit_gc_rsp_reset(void) { x86_rsp_reset_all(); }
extern "C++" int emit_gc_site_raw(int kind, int rule, int depth) {
    if (!g_emitter.g_gc_sites_armed) return -1;
    gc_site_t * e = &CV_PUSH(g_emitter.g_gc_sites_v, gc_site_t); e->pc = 0; e->depth = depth; e->kind = (uint16_t)kind; e->rule = (uint16_t)rule; e->zdepth = 0; e->fcb = 0; e->fcbase = 0;
        e->demit = depth;
        e->bk = x86_boxkind(); e->mn = 0; e->nd = g_emit.node;
    return (int)g_emitter.g_gc_sites_v.len - 1;
}
static int emit_gc_match_reaches(const IR_t * b, const IR_t * nd) {
    int cap = (g_emit_cfg && g_emit_cfg->n > 0) ? g_emit_cfg->n : 0, n = 0, h = 0;
    if (!cap || !b || !nd || !b->γ.node) return 0;
    const IR_t * q[cap + 1];
    q[n++] = b->γ.node;
    while (h < n) {
        const IR_t * x = q[h++];
        if (x == nd) return 1;
        if (x == b || x->op == IR_MATCH_END || !ir_is_matcher(x->op)) continue;
        const IR_t * nx[2] = { x->γ.node, x->ω.node };
        for (int k = 0; k < 2 + x->n_operands; k++) {
            const IR_t * y = k < 2 ? nx[k] : x->operands[k - 2];
            int seen = 0;
            if (!y) continue;
            for (int i = 0; i < n && !seen; i++) seen = (q[i] == y);
            if (!seen && n < cap) q[n++] = y;
        }
    }
    return 0;
}
static const gc_site_t * emit_gc_match_owner(const IR_t * nd, int * levels) {
    const gc_site_t * outer = (const gc_site_t *)0;
    *levels = 0;
    for (uint32_t i = 0; i < g_emitter.g_gc_sites_v.len; i++) {
        const gc_site_t * m = &CV_AT(g_emitter.g_gc_sites_v, gc_site_t, i);
        if (m->kind != X86_SITE_MATCH_ENTER || !m->mn || !m->nd || !emit_gc_match_reaches(m->nd, nd)) continue;
        (*levels)++;
        if (!outer) outer = m;
    }
    return outer;
}
extern "C++" void emit_gc_site_play(int idx, const void * pc, int delta, int aligned, int unknown) {
    if (idx < 0 || (uint32_t)idx >= g_emitter.g_gc_sites_v.len) return;
    gc_site_t * e = &CV_AT(g_emitter.g_gc_sites_v, gc_site_t, idx);
    if (pc) e->pc = (uint64_t)(uintptr_t)pc;
    else e->pc = ++g_emitter.g_gc_site_play_seq;
    if (e->kind == X86_SITE_MATCH_ENTER) { int ek = e->rule == 3 ? 3 : (e->rule == 1 ? 1 : (e->rule == 0 && !aligned && !unknown ? 0 : 5)); int ed = e->rule == 0 ? e->depth + delta : 0;
        if (g_emitter.g_gc_match_n == 0) { g_emitter.g_gc_match_out.enc_kind = ek; g_emitter.g_gc_match_out.enc_depth = ed; g_emitter.g_gc_match_out.nd = e->nd; } g_emitter.g_gc_match_n++;
            e->depth = ed; e->mn = ek + 1; return; }
    if (e->kind == X86_SITE_MATCH_LEAVE) { if (g_emitter.g_gc_match_n > 0) g_emitter.g_gc_match_n--; return; }
    if (e->rule == 9) {
        const x86_rsp_st_t & K = x86_rsp_S();
        int m9 = K.mark9_set ? K.mark9 : K.mark, m9u = K.mark9_set ? K.mark9_unknown : K.mark_unknown;
        int d = delta - m9 - 16;
        if (aligned || unknown || m9u || d < 0 || (d & 7) || d / 8 > 255 || (e->kind & 0xFF00)) {
            fprintf(stderr,
                "FATAL emit_gc_site_play: a rule-9 SHIM-SELF site in %s needs a known distance from R to the shim's base, a multiple of 8 of at most 255 slots (delta=%d mark=%d d=%d al=%d unk=%d mar"
                "k_unk=%d)\n",
                g_emit.flat_fam ? g_emit.flat_fam : "chain", delta, K.mark, d, aligned, unknown, K.mark_unknown);
            abort();
        }
        e->kind = (uint16_t)((e->kind & 255) | ((d / 8) << 8));
    }
    if (e->rule == 0) { e->depth += delta; if (aligned) e->rule = 4; if (unknown) e->rule = 5; }
    if (g_emitter.g_gc_match_n > 0 && (e->rule == 0 || e->rule == 1 || e->rule == 3 || e->rule == 5) &&
        (ir_is_matcher(e->nd ? (int)e->nd->op : 0) || emit_gc_match_reaches(g_emitter.g_gc_match_out.nd, e->nd))) {
        const gc_match_lvl_t & o = g_emitter.g_gc_match_out;
        if (o.enc_kind == 5) e->rule = 5; else { e->rule = 6; e->kind = (uint16_t)((e->kind & 255) | ((g_emitter.g_gc_match_n & 15) << 8) | ((o.enc_kind & 15) << 12)); e->depth = o.enc_depth; } }
    else if (g_emitter.g_gc_match_n == 0 && e->nd && ir_is_matcher(e->nd->op) && e->nd->op != IR_MATCH_BEGIN && e->nd->op != IR_MATCH_END
        && (e->rule == 0 || e->rule == 1 || e->rule == 3 || e->rule == 5)) {
        int lv = 0;
        const gc_site_t * o = emit_gc_match_owner(e->nd, &lv);
        if (o) {
            int ek = o->mn - 1;
            if (ek == 5) e->rule = 5;
            else { e->rule = 6; e->kind = (uint16_t)((e->kind & 255) | ((lv & 15) << 8) | ((ek & 15) << 12)); e->depth = o->depth; }
        }
    }
    if (gc_sites_report_on())
        fprintf(stderr, "[GC-SITE] %s #%d kind=%d rule=%d zdepth=%d fc=%d/%d zrun=%d delta=%d al=%d unk=%d -> depth=%d off=%ld buf=%p box=%s matchn=%d pc=%p nd_op=%d nd_idx=%d \n",
        g_emit.flat_fam ? g_emit.flat_fam : "chain", idx, (int)e->kind, (int)e->rule, (int)e->zdepth, (int)e->fcb, (int)e->fcbase, (int)e->demit, delta, aligned, unknown, (int)e->depth,
        pc ? (long)((const char *)pc - (const char *)bb_emit_buf) : -1L, (const void *)bb_emit_buf, e->bk ? e->bk : "?", e->mn, pc, e->nd ? (int)e->nd->op : -1,
        [&]() { if (!g_emit_cfg || !e->nd) return -1; for (int q = 0; q < g_emit_cfg->n; q++) if (g_emit_cfg->all[q] == e->nd) return q; return -2; }());
}
extern "C++" std::string emit_gc_site_label(int idx) { return std::string(".Lgcsite_") + (g_emit.flat_fam ? g_emit.flat_fam : "chain") + "_" + std::to_string(idx); }
extern "C" int emit_gc_sites_last_off(void) { return g_emitter.g_gc_sites_last_off; }
extern "C" int emit_gc_sites_offs_n(void) { return (int)g_emitter.g_gc_sites_offs_v.len; }
extern "C" int emit_gc_sites_off(int k) { return (k >= 0 && (uint32_t)k < g_emitter.g_gc_sites_offs_v.len) ? CV_AT(g_emitter.g_gc_sites_offs_v, int, k) : -1; }
extern "C" int emit_gc_sites_names_n(void) { return g_emitter.g_gc_sites_names_n; }
extern "C" const char * emit_gc_sites_name(int k) { return (k >= 0 && k < g_emitter.g_gc_sites_names_n) ? CV_AT(g_emitter.g_gc_sites_names_v, char *, k) : "?"; }
static void emit_gc_sites_data(const char * fam, int frameless) {
     int n = (int)g_emitter.g_gc_sites_v.len; bb_label_t tl; emit_label_initf(&tl, ".Lgcsites_%s_%d", g_emit.flat_fam ? g_emit.flat_fam : "chain", g_emitter.emit_gc_sites_data_seq++);
    if (frameless && n == 0) { g_emitter.g_gc_sites_last_off = -1; return; }
    const uint64_t ckt = frameless ? ((xa_flat_class_c_pred()) ? (uint64_t)g_emit.flat_frame_bytes : 0u) : g_emitter.g_gc_map_link;
    if (g_is_text) {
        std::string t =
            std::string(".pushsection .data.rel.ro,\"aw\",@progbits\n .p2align 3\n") + tl.name + ":\n .quad " + std::to_string(n) + "\n .quad " +
                (frameless ? std::string("0") : std::string(g_emitter.g_gc_map_lbl.name)) + "\n .quad " + std::to_string(ckt) +
                "\n .quad "
            + (g_emit.gc_code_lbl.name ? std::string(g_emit.gc_code_lbl.name) : std::string("0")) + "\n";
            emit_text_n(t.data(), t.size());
        int ord[n > 0 ? n : 1];
        for (int i = 0; i < n; i++) ord[i] = i;
        std::stable_sort(ord, ord + n, [](int a, int b) { return CV_AT(g_emitter.g_gc_sites_v, gc_site_t, a).pc < CV_AT(g_emitter.g_gc_sites_v, gc_site_t, b).pc; });
        for (int k = 0; k < n; k++) { int i = ord[k]; gc_site_t * e = &CV_AT(g_emitter.g_gc_sites_v, gc_site_t, i);
            uint64_t w = (uint64_t)e->kind | ((uint64_t)e->rule << 16) | ((uint64_t)(uint32_t)e->depth << 32);
            t = " .quad " + emit_gc_site_label(i) + (g_emit.gc_code_lbl.name ? std::string("-") + g_emit.gc_code_lbl.name : std::string()) + "\n .quad "
                + std::to_string((unsigned long long)w) + "\n"; emit_text_n(t.data(), t.size()); }
        emit_textf(".popsection\n");
        g_emitter.g_gc_sites_last_off = 0;
    } else {
        emit_label_define_bb(&tl); g_emitter.g_gc_sites_last_off = tl.offset; CV_PUSH(g_emitter.g_gc_sites_offs_v, int) = tl.offset;
        bb_emit_u64((uint64_t)n); bb_emit_u64(frameless ? (uint64_t)0 : (uint64_t)(uintptr_t)(bb_emit_buf + g_emitter.g_gc_map_lbl.offset)); bb_emit_u64(ckt);
        bb_emit_u64(g_emit.gc_code_lbl.name ? (uint64_t)(uintptr_t)(bb_emit_buf + g_emit.gc_code_lbl.offset) : (uint64_t)0);
        if (n > 1)
            qsort(&CV_AT(g_emitter.g_gc_sites_v, gc_site_t, 0), (size_t)n, sizeof(gc_site_t), [](const void * a, const void * b) -> int { uint64_t x = ((const gc_site_t *)a)->pc,
                y = ((const gc_site_t *)b)->pc;
            return (x > y) - (x < y); });
        for (int i = 0; i < n; i++) { gc_site_t * e = &CV_AT(g_emitter.g_gc_sites_v, gc_site_t, i); uint64_t w = (uint64_t)e->kind | ((uint64_t)e->rule << 16) | ((uint64_t)(uint32_t)e->depth << 32);
            bb_emit_u64(e->pc && g_emit.gc_code_lbl.name ? e->pc - (uint64_t)(uintptr_t)(bb_emit_buf + g_emit.gc_code_lbl.offset) : e->pc); bb_emit_u64(w); }
    }
    CV_PUSH(g_emitter.g_gc_sites_names_v, char *) = ct_strdup(tl.name); g_emitter.g_gc_sites_names_n++;
    g_emitter.g_gc_sites_v.len = 0;
    (void)fam;
}
static const char * gc_map_entry_fam(const char * fam) { return (fam && !strcmp(fam, "pat_flat")) ? "main" : fam; }
static int gc_maps_knob(const char * k) { const char * e = getenv(k); return (e && *e == '1') ? 1 : 0; }
static int gc_maps_report_on(void) { if (g_emitter.gc_maps_report_on_v < 0) g_emitter.gc_maps_report_on_v = gc_maps_knob("SCRIP_GC_MAPS_REPORT"); return g_emitter.gc_maps_report_on_v; }
extern "C++" std::string emit_gc_map_register(int value_bytes, int frame_bytes, int header_bytes, unsigned flags, int frame_rel, uint64_t link) {
    if (frame_rel == 2 ? (value_bytes < 16 || value_bytes != frame_bytes || (flags & GC_FRAME_MAP_BLOB) == 0) : (value_bytes < 0 || (value_bytes & 15) || value_bytes + 16 > frame_bytes))
        { fprintf(stderr,
        "FATAL emit_gc_map_register: value_bytes=%d frame_bytes=%d frame_rel=%d (the value region is 16-aligned and the 16 reserved bytes after it lie inside the frame; a blob's region is the wh"
            "ole frame below its base "
        "and carries GC_FRAME_MAP_BLOB)\n", value_bytes, frame_bytes, frame_rel); abort(); }
    zsm_frame_carve(frame_bytes);
    g_emitter.g_gc_map_pending = 1; g_emitter.g_gc_value_bytes = value_bytes; g_emitter.g_gc_map_link = link; g_emitter.g_gc_map_fb = frame_bytes; g_emitter.g_gc_map_hdr = header_bytes;
        g_emitter.g_gc_map_flags = flags;
    emit_label_initf(&g_emitter.g_gc_map_lbl, ".Lgcmap_%s", g_emit.flat_fam ? g_emit.flat_fam : "chain");
    return x86("note", std::string("GCANCHOR ") + g_emitter.g_gc_map_lbl.name + " " + (frame_rel == 2 ? std::string(RDQ("rbp", -value_bytes + 8))
                                                                          : frame_rel == 1 && zls_g_keys_only(g_emit_cfg) ? std::string(x86_fr64_prefix()) + std::to_string(value_bytes + 8) + "]"
                                                                          : frame_rel ? std::string(FRQ(value_bytes + 8)) : std::string(RDQ("rsp", value_bytes + 8))))
         + IF(frame_rel != 2 || blob_cont_copy(), x86("mov", "eax", 0L));
}
static void emit_gc_map_data(const char * fam) {
    { extern const char * zls_graph_name_get(const IR_graph_t *); const char * ef = gc_map_entry_fam(fam);
      if (ef && !strcmp(ef, "main")) fam = ef;
      else { const char * gn = g_emit_cfg ? zls_graph_name_get(g_emit_cfg) : (const char *)0; fam = (gn && *gn) ? gn : fam; } }
    if (!g_emitter.g_gc_map_pending) { g_emitter.g_gc_map_last_off = -1; emit_gc_sites_data(fam, 1); return; }
    g_emitter.g_gc_map_pending = 0;
    const uint64_t * lay = (const uint64_t *)0; int lay_n = 0; g_emitter.g_zls_lay_gaps = 0; g_emitter.g_zls_lay_conf = 0;
    if (g_emitter.g_gc_map_flags & GC_FRAME_MAP_BLOB) { lay = (const uint64_t *)g_emitter.g_blob_lay.p; lay_n = (int)g_emitter.g_blob_lay.len; }
    else if (g_emit_cfg) { int gaps = 0, conf = 0;
        if (g_emitter.g_zls_lay_v.cap < 32770) cv_reserve(&g_emitter.g_zls_lay_v, (uint32_t)sizeof(uint64_t), 32770, "g_zls_lay");
        int n = zls_g_layout_q(g_emit_cfg, (uint64_t *)g_emitter.g_zls_lay_v.p, (int)g_emitter.g_zls_lay_v.cap - 2, &gaps, &conf);
        while (n > (int)g_emitter.g_zls_lay_v.cap - 2) { cv_reserve(&g_emitter.g_zls_lay_v, (uint32_t)sizeof(uint64_t), (uint64_t)g_emitter.g_zls_lay_v.cap * 2, "g_zls_lay");
            n = zls_g_layout_q(g_emit_cfg, (uint64_t *)g_emitter.g_zls_lay_v.p, (int)g_emitter.g_zls_lay_v.cap - 2, &gaps, &conf); }
        uint64_t * g_zls_lay = (uint64_t *)g_emitter.g_zls_lay_v.p;
        if (n >= 0 && g_emit_cfg->root_graph) { int rg = zls_g_value_region(g_emit_cfg), nc = g_emit_cfg->standing_cells, clo = g_emitter.g_gc_value_bytes - 8 * nc;
            if (rg < 0 || clo < rg || nc < 0)
                { fprintf(stderr,
                "FATAL emit_gc_map_data: root graph %s region=%d cells=%d cells_lo=%d value_bytes=%d -- the standing cells must lie between the value region and the reserved cell (gc_frame_map.h"
                    " FLAT_FRAME_A"
                "LLOWANCE_ROOT)\n", fam ? fam : "?", rg, nc, clo, g_emitter.g_gc_value_bytes); abort(); }
            if (clo > rg) g_zls_lay[n++] = GC_LAY_Q(rg, GC_LAY_RAW, clo - rg);
            if (nc > 0) g_zls_lay[n++] = GC_LAY_Q(clo, GC_LAY_PTR_GC, 8 * nc); }
        if (n >= 0) { lay = g_zls_lay; lay_n = n; g_emitter.g_gc_map_flags |= GC_FRAME_MAP_LAYOUT; g_emitter.g_zls_lay_gaps = gaps; g_emitter.g_zls_lay_conf = conf; } }
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
extern "C" void rt_gc_frame_maps_add(const gc_frame_map_t *m); extern "C" void rt_gc_frame_sites_add(const void *tab);
extern "C" void emit_gc_tables_register(const void *fn) { if (!fn) return;
    if (g_emitter.g_gc_map_last_off >= 0) rt_gc_frame_maps_add((const gc_frame_map_t *)((const char *)fn + g_emitter.g_gc_map_last_off));
    for (uint32_t k = 0; k < g_emitter.g_gc_sites_offs_v.len; k++) rt_gc_frame_sites_add((const char *)fn + CV_AT(g_emitter.g_gc_sites_offs_v, int, k)); }
extern "C" int emit_gc_map_names_n(void) { return g_emitter.g_gc_map_names_n; }
extern "C" const char * emit_gc_map_name(int i) { return (i >= 0 && i < g_emitter.g_gc_map_names_n) ? CV_AT(g_emitter.g_gc_map_names_v, char *, i) : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_dump_homes_on(void) { g_emitter.dump_homes = 1; }
extern "C" void emit_dump_plan_on(void) { g_emitter.dump_plan = 1; }
static int dh_callee_graph(const IR_t * nd) {
    if (!ir_is_call_kind(ir_kind0(nd)) || ir_kind0(nd) == IR_CALL_VALUE || !IR_LIT(nd).sval) return -1;
    auto it = g_emitter.dh_gidx.find(IR_LIT(nd).sval);
    return it == g_emitter.dh_gidx.end() ? -1 : it->second;
}
extern "C" int zls_g_resumable_frame(const IR_graph_t * g);
static const char * dh_box_dyn(const IR_t * nd) {
    switch ((int)ir_kind0(nd)) {
        case IR_SUSPEND:
        return "suspend";
        case IR_CREATE:
        case IR_ACTIVATE:
        case IR_CORET:
        case IR_COFAIL:
        return "coexpression";
        case IR_CALL_VALUE:
        return "unknown-callee";
        case IR_CALL_GEN:
        case IR_PROC_GEN:
        return "generator-callee";
        default:
        break;
    }
    int h = dh_callee_graph(nd);
    if (h < 0 && ir_kind0(nd) == IR_CALL_PROC_STAGED && emit_resumable_frame()) return "unresolved-callee";
    return (h >= 0 && g_emitter.dh_rt[h]) ? "retaining-callee" : (const char *)0;
}
static void dh_retains_build(void);
static void dh_retains_build(void) {
    extern const char * zls_graph_name_get(const IR_graph_t *);
    int G = g_stage2.bbp.count;
    if ((int)g_emitter.dh_rt.size() == G) return;
    g_emitter.dh_gidx.clear();
    for (int k = 0; k < G; k++) { const char * nm = g_stage2.bbp.table[k] ? zls_graph_name_get(g_stage2.bbp.table[k]) : (const char *)0; if (nm) g_emitter.dh_gidx.emplace(nm, k); }
    g_emitter.dh_rt.assign((size_t)G, 0);
    for (int k = 0; k < G; k++) {
        const IR_graph_t * g = g_stage2.bbp.table[k];
        int nsucc = 0;
        for (int i = 0; g && i < g->n; i++) {
            const IR_t * nd = g->all[i];
            if (!nd) continue;
            if (nd->op == IR_SUCCEED) nsucc++;
            int o = (int)ir_kind0(nd);
            if (o == IR_SUSPEND || o == IR_CREATE || o == IR_ACTIVATE || o == IR_CORET || o == IR_COFAIL || o == IR_CALL_VALUE || o == IR_CALL_GEN || o == IR_PROC_GEN) g_emitter.dh_rt[k] = 1;
        }
        if (nsucc > 1) g_emitter.dh_rt[k] = 1;
    }
    for (int changed = 1; changed;) {
        changed = 0;
        for (int k = 0; k < G; k++) {
            const IR_graph_t * g = g_stage2.bbp.table[k];
            for (int i = 0; g && !g_emitter.dh_rt[k] && i < g->n; i++) {
                int h = g->all[i] ? dh_callee_graph(g->all[i]) : -1;
                if (h >= 0 && g_emitter.dh_rt[h]) { g_emitter.dh_rt[k] = 1; changed = 1; }
            }
        }
    }
}
static const char * xop_frame_why(const IR_t * nd) {
    return zls_dj_retains(nd) ? "retaining-disjunction" : nd->op == IR_TO ? "generator-state" : "read-across-retaining-edge";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zls_g_keys_only(const IR_graph_t * g);
extern "C" int zls_home_why(const IR_t * nd);
extern "C" int zls_home_local_test(const IR_graph_t * g, const char * vn);
extern "C" int zls_g_hf_resident(const IR_graph_t * g, const IR_t * nd);
extern "C" int zd_dj_on_spine(const IR_t * nd);
extern "C" const char * zls_home_why_name(int w);
static void emit_dump_homes(IR_t ** nodes, int n, const unsigned char * zon) {
    extern const char * zls_graph_name_get(const IR_graph_t *); extern int zls_g_vslot_count(const IR_graph_t *); extern const char * zls_g_vslot_get(const IR_graph_t *, int, int *);
    const IR_graph_t * g = g_emit_cfg;
    const char * gn = g ? zls_graph_name_get(g) : (const char *)0;
    int gi = -1;
    for (int k = 0; g && k < g_stage2.bbp.count; k++) if (g_stage2.bbp.table[k] == g) { gi = k; break; }
    for (int i = 0; i < n; i++) {
        IR_t * nd = nodes[i];
        if (!nd) continue;
        int ai = -1;
        for (int k = 0; g && k < g->n; k++) if (g->all[k] == nd) { ai = k; break; }
        int off = bb_slot_get(nd); if (off < 0) off = nd_slot(nd); if (zls_key_unbacked(g, off)) off = -1;
        int xf = planner_rec_home(nd), rbp = xf != -1, esc = nd->op == IR_TABLE_HDR && off >= 0;
        int hs = !esc && !rbp && off >= 0 && zls_home_why(nd) && !ir_node_produces_value(nd->op);
        int hr = !esc && !rbp && !hs && off >= 0 && g && zls_g_hf_resident(g, nd) && (nd->op != IR_DISJUNCTION || zls_dj_retains(nd) || !zd_dj_on_spine(nd));
        const char * home = (esc || hs || hr) ? "RBP" : zon[i] ? (zd_k(nd) > 0 ? "SPINE" : "NONE") : off < 0 ? "NONE" : rbp ? "RBP" : "RSP_CARVED";
        if (esc || hs || hr) xf = off;
        fprintf(stderr, "[HOMES] g=%d graph=%s all=%d kind=%s home=%s off=%d xf=%d xfwhy=%s\n", gi, gn ? gn : "?", ai, bb_op_name(nd->op), home, off, xf,
            esc ? "address-escapes" : hs ? zls_home_why_name(zls_home_why(nd)) : hr ? (ir_node_produces_value(nd->op) ? "resident-result" : "construct-record") : rbp ? xop_frame_why(nd) : "-");
        if (esc) fprintf(stderr, "[HOMETEST] g=%d graph=%s all=%d kind=%s test=state:address-escapes\n", gi, gn ? gn : "?", ai, bb_op_name(nd->op));
        else fprintf(stderr, "[HOMETEST] g=%d graph=%s all=%d kind=%s test=%s%s\n", gi, gn ? gn : "?", ai, bb_op_name(nd->op), zls_home_why(nd) ? "operand:" : "", zls_home_why_name(zls_home_why(nd)));
    }
    int nv = g ? zls_g_vslot_count(g) : 0;
    for (int k = 0; k < nv; k++) {
        int off = -1;
        const char * vn = zls_g_vslot_get(g, k, &off);
        if (zls_g_keys_only(g)) off = -1;
        int blk = g && g->block_args ? zls_g_block_base(g) : -1; int cov = (k < (int)g_emitter.zst_vcov.size()) ? g_emitter.zst_vcov[(size_t)k] : 0,
            use = (k < (int)g_emitter.zst_vuse.size()) ? g_emitter.zst_vuse[(size_t)k] : -1;
        int soff = off; const char * home = off < 0 ? "NONE" : (blk >= 0 && off >= blk) ? "SPINE" : use == 0 ? "NONE" : (cov > 0 && cov == use) ? "SPINE" : "RSP_CARVED";
        if (cov > 0 && cov == use) for (size_t t = 0; t < g_emitter.zst_name.size(); t++) if (vn && g_emitter.zst_L[t] != INT_MIN && !strcmp(g_emitter.zst_name[t], vn)) { soff = -g_emitter.zst_L[t];
            break; }
        fprintf(stderr, "[HOMES] g=%d graph=%s local=%s home=%s off=%d\n", gi, gn ? gn : "?", vn ? vn : "?", home, soff);
        fprintf(stderr, "[HOMETEST] g=%d graph=%s local=%s test=%s%s\n", gi, gn ? gn : "?", vn ? vn : "?", zls_home_local_test(g, vn) ? "local:" : "", zls_home_why_name(zls_home_local_test(g, vn)));
    }
    dh_retains_build();
    for (int k = 0; g && k < g->n; k++) {
        const IR_t * nd = g->all[k];
        if (!nd) continue;
        int lo = (int)nd->op;
        int names_local = lo == IR_VAR || lo == IR_VAR_REF || lo == IR_ASSIGN || lo == IR_ASSIGN_VAR || lo == IR_REV_ASSIGN_VAR || lo == IR_NULLTEST_VAR || lo == IR_FIELD_VAR ||
            lo == IR_UNIFY_FIRST || lo == IR_UNIFY_VALUE;
        if (names_local && IR_LIT(nd).sval) {
            for (int v = 0; v < nv; v++) {
                int voff = -1;
                const char * vn = zls_g_vslot_get(g, v, &voff);
                if (vn && !strcmp(vn, IR_LIT(nd).sval)) { fprintf(stderr, "[LOCALUSE] g=%d graph=%s all=%d kind=%s local=%s\n", gi, gn ? gn : "?", k, bb_op_name(nd->op), vn); break; }
            }
        }
        const char * why = dh_box_dyn(nd);
        if (why) fprintf(stderr, "[RSPFX] g=%d graph=%s all=%d kind=%s rsp=dynamic why=%s callee=%s\n", gi, gn ? gn : "?", k, bb_op_name(nd->op), why,
            (ir_is_call_kind(ir_kind0(nd)) && IR_LIT(nd).sval) ? IR_LIT(nd).sval : "-");
        else if (nd->op == IR_CUT) fprintf(stderr, "[RSPFX] g=%d graph=%s all=%d kind=%s rsp=renormal why=cut-restores-the-frame-base callee=-\n", gi, gn ? gn : "?", k, bb_op_name(nd->op));
    }
    if (g && g->root_graph && g->standing_cells > 0)
        fprintf(stderr, "[HOMES] g=%d graph=%s standing=%d home=STANDING why=dynamic-predicate-root\n", gi, gn ? gn : "?", g->standing_cells);
}
static int zrow_of(const IR_t * nd) {
    const zeta_plan_t * r = g_emitter.g_zd_rec_p;
    if (!nd || !r || !r->home || r->n != g_emitter.g_zd_n || g_emitter.g_zd_nodes_p != g_emitter.codegen_flat_chain_body_nodes) return -1;
    return nidx(g_emitter.codegen_flat_chain_body_nodes, r->n, nd);
}
static int zrow_under(int ri) { const zeta_plan_t * r = g_emitter.g_zd_rec_p; return (ri >= 0 && r && r->under) ? r->under[ri] : 0; }
static void dp_col(long v, long none) { if (v == none) printf(" -"); else printf(" %ld", v); }
static void emit_dump_plan(const zeta_plan_t * pl, const char * prefix) {
    extern const char * zls_graph_name_get(const IR_graph_t *);
    const IR_graph_t * g = g_emit_cfg;
    const char * gn = g ? zls_graph_name_get(g) : (const char *)0;
    IR_t ** nodes = g_emitter.codegen_flat_chain_body_nodes;
    int n = pl->n;
    if (!g_emitter.dump_plan_cols) { printf("PLAN_COLUMNS E X K definite zeta off depth retains_code owner fenced gpop wpop restore act\n"); g_emitter.dump_plan_cols = 1; }
    printf("PLAN graph=%s serial=%d chain=%s n=%d constructs=%d resumable=%d\n", gn ? gn : "?", g ? g->serial : -1, prefix ? prefix : "-", n, (g && pl->own_root) ? g->n_constructs : 0,
        g ? zls_g_resumable_frame(g) : 0);
    for (int c = 0; g && pl->own_root && c < g->n_constructs; c++) {
        const IR_construct_t * k = &g->constructs[c];
        int bi = -1, ei = -1;
        for (int q = 0; q < g->n; q++) { if (g->all[q] == k->begin) bi = q; if (k->end && g->all[q] == k->end) ei = q; }
        printf("PLAN_CONSTRUCT graph=%s serial=%d k=%d kind=%s begin=%d end=%d lo=%d hi=%d\n", gn ? gn : "?", g->serial, c, ir_construct_name(k->kind), bi, ei, k->lo, k->hi);
    }
    for (int i = 0; i < n; i++) {
        IR_t * nd = nodes[i];
        int ai = -1;
        for (int k = 0; g && k < g->n; k++) if (g->all[k] == nd) { ai = k; break; }
        int on = pl->on[i], K = zd_k(nd), X = pl->out[i], gp = pl->gp[i], wp = pl->wp[i];
        int off = pl->lkey ? pl->lkey[i] : -1, xf = pl->home ? pl->home[i] : -1, esc = nd->op == IR_TABLE_HDR && off >= 0;
        const char * zeta = zeta_name(pl->zeta ? (int)pl->zeta[i] : ZETA_NONE);
        long zoff = esc ? off : xf != -1 ? xf : (!on && off >= 0) ? off : LONG_MIN;
        int known = pl->xb && pl->xb[i] != INT_MIN;
        printf("PLAN_ROW i=%d all=%d kind=%s", i, ai, bb_op_name(nd->op));
        dp_col(on ? X - K : LONG_MIN, LONG_MIN);
        dp_col(on ? X : LONG_MIN, LONG_MIN);
        dp_col(K, LONG_MIN);
        dp_col(on ? known : LONG_MIN, LONG_MIN);
        printf(" %s", zeta);
        dp_col(zoff, LONG_MIN);
        dp_col((on && known) ? (long)pl->xb[i] + X : LONG_MIN, LONG_MIN);
        dp_col(pl->ret ? pl->ret[i] : planner_retains(nd), LONG_MIN);
        dp_col(pl->claim[i], -1);
        dp_col(on ? gp > 0 : LONG_MIN, LONG_MIN);
        dp_col(on ? gp : LONG_MIN, LONG_MIN);
        dp_col(on ? wp : LONG_MIN, LONG_MIN);
        dp_col((pl->cut && pl->cut[i] >= 0) ? pl->cut[i] : LONG_MIN, LONG_MIN);
        printf(" %s", planner_act_cause_name(planner_act_cause_of(nd)));
        printf("\n");
    }
}
static int codegen_flat_chain_body(IR_t *entry, const char *prefix) {
    planner_act_build(g_emit_cfg, blob_frame_scope(), blob_head_bytes());
    bb_label_t lbl_α, lbl_α_body, lbl_γ, lbl_ω, lbl_β, lbl_res;
    bb_label_t * lbl_α_orig_p = (bb_label_t *)0;
    int _top_hoist = 0;
    int _flt_hoisted[4] = {0, 0, 0, 0};
    int _flt_uid_burn[4] = {0, 0, 0, 0};
    const char *fam = (strncmp(prefix, "proc_", 5) == 0) ? prefix + 5 : prefix;
    g_emit.flat_fam = fam;
    g_emitter.g_gc_sites_v.len = 0; g_emitter.g_gc_sites_armed = 1; g_emitter.g_gc_sites_last_off = -1; g_emitter.g_gc_sites_offs_v.len = 0; g_emitter.g_gc_match_n = 0; emit_gc_rsp_reset();
    emit_label_initf(&g_emit.gc_code_lbl, ".Lgccode_%s_%d", fam ? fam : "chain", g_emit.gc_code_seq++);
    if (g_is_text) { std::string _cl = std::string(g_emit.gc_code_lbl.name) + ":\n"; emit_text_n(_cl.data(), _cl.size()); } else emit_label_define_bb(&g_emit.gc_code_lbl);
    g_emit.op_zdepth = 0; g_emit.op_fc_bytes = 0;
    if (gc_sites_report_on()) fprintf(stderr, "[GC-GRAPH] fam=%s prefix=%s buf=%p text=%d\n", fam ? fam : "?", prefix ? prefix : "?", (const void *)bb_emit_buf, g_is_text);
    g_emitter.g_flt_fam = (strncmp(prefix, "proc_", 5) == 0) ? fam : (const char *)0; g_emitter.g_flt_lbl[1] = g_emitter.g_flt_lbl[2] = g_emitter.g_flt_lbl[3] = (bb_label_t *)0;
    g_emit.x86_uid_kind = (const char *)0;
    emit_label_initf(&lbl_α, "%s_α", fam);
    emit_label_initf(&lbl_α_body, "%s_α_body", fam);
    emit_label_initf(&lbl_γ, "%s_γ", fam);
    emit_label_initf(&lbl_ω, "%s_ω", fam);
    emit_label_initf(&lbl_β, "%s_β", fam);
    emit_label_initf(&lbl_res, "%s_res", fam);
       bb_label_t * alt_step_lbl = (bb_label_t *)0; int n_alt = 0;
    bb_label_t * ix_switch = (bb_label_t *)0; int * ix_code = (int *)0; int ix_n = 0; int ix_fbr = -2, ix_fba = -2, ix_fbi = -2; int ix_slot = -1; int * ix_tag = (int *)0;
        int64_t * ix_val = (int64_t *)0;
    int * ix_sets = (int *)0; int * ix_set_beg = (int *)0; int * ix_set_len = (int *)0; int ix_nset = 0;
    g_emit.flat_alt1_p = (bb_label_t *)0; g_emit.flat_altdet_p = (bb_label_t *)0;
    if (g_emit_cfg && g_emit_cfg->n_alts > 0) {
        n_alt = g_emit_cfg->n_alts;
        if (g_emitter.codegen_flat_chain_body_alt_tr_cap < n_alt) { g_emitter.codegen_flat_chain_body_alt_tr =
            (bb_label_t **)ct_grow(g_emitter.codegen_flat_chain_body_alt_tr, (size_t)n_alt * sizeof(bb_label_t *));
                                  g_emitter.codegen_flat_chain_body_ret_tr = (bb_label_t **)ct_grow(g_emitter.codegen_flat_chain_body_ret_tr, (size_t)n_alt * sizeof(bb_label_t *));
                                      g_emitter.codegen_flat_chain_body_alt_tr_cap = n_alt; }
        for (int _ak = 0; _ak < n_alt; _ak++) g_emitter.codegen_flat_chain_body_alt_tr[_ak] = emit_label_alloc("%s_alt%d", fam, _ak);
        for (int _ak = 0; _ak < n_alt; _ak++) g_emitter.codegen_flat_chain_body_ret_tr[_ak] = emit_label_alloc("%s_ret%d", fam, _ak);
        alt_step_lbl = emit_label_alloc("%s_step", fam);
        g_emit.flat_altdet_p = emit_label_alloc("%s_altdet", fam);
        if (n_alt > 1) g_emit.flat_alt1_p = g_emitter.codegen_flat_chain_body_alt_tr[1];
    }
    int bare = g_emit.flat_bare_chain;
    if (strncmp(prefix, "proc_LBL__", 10) == 0) emit_label_initf(&lbl_α, "%s", prefix + 5);
    else if (strncmp(prefix, "proc_", 5) == 0 && !bare) {
        { extern int bb_ab_sym_is_thunk_stem(const char *); if (bb_ab_sym_is_thunk_stem(prefix + 5)) emit_label_initf(&lbl_α, "%s", prefix + 5); else emit_label_initf(&lbl_α, "FN__%s", prefix + 5);
            }
        lbl_α_orig_p = emit_label_alloc("%s_α", prefix + 5);
    }
    int fn_face_dead = bare && strncmp(prefix, "proc_", 5) == 0 && strncmp(prefix, "proc_LBL__", 10) != 0;
    if (bare) { emit_label_initf(&lbl_γ, "main_\xcf\x89"); emit_label_initf(&lbl_ω, "main_\xcf\x89"); emit_label_initf(&lbl_β, "main_\xcf\x89"); emit_label_initf(&lbl_res, "main_\xcf\x89"); }
    int text_externalise = g_is_text ? 1 : 0;
    if (text_externalise) data_buf_reset();
    g_emit.flat_lbl_α = lbl_α.name;
    g_emit.flat_lbl_α_body = lbl_α_body.name;
    g_emit.flat_lbl_γ = lbl_γ.name;
    g_emit.flat_lbl_ω = lbl_ω.name;
    g_emit.flat_lbl_β = lbl_β.name;
    g_emit.flat_β_p = &lbl_β;
    g_emit.flat_res_p = &lbl_res;
    g_emit.flat_succ_p = &lbl_γ;
    g_emit.flat_fail_p = &lbl_ω;
    g_emit.flat_text_externalise = text_externalise;
    g_resumable_callable_active = (g_emit_cfg && g_emit_cfg->resumable_callable) ? 1 : 0;
    g_emitter.g_nidx_nodes = 0; g_emitter.g_nidx_n = 0;
    zeta_plan_t * _pl = planner_lookup_or_plan(entry, prefix); entry = _pl->entry; int n = _pl->n; int entry_is_own_graph_root = _pl->own_root; int flat_empty_body_fail = _pl->empty_fail,
        flat_empty_body_succ = _pl->empty_succ;
    unsigned char * zd_on = _pl->on; int * zd_out = _pl->out; int * zd_gp = _pl->gp; int * zd_wp = _pl->wp; int * zd_arm = _pl->arm; int * zd_cps = _pl->cps; int * zd_cpd = _pl->cpd;
        int * zd_chn = _pl->chn;
    int * zd_claim = _pl->claim; int * zd_vl = _pl->vl; int * zd_xb = _pl->xb; int * zd_cut = _pl->cut; (void)entry_is_own_graph_root; (void)flat_empty_body_succ; (void)zd_vl; (void)zd_chn;
        int _sph = 0, _spgd = 0, _spf = 0, _wpn = 0;
        bb_label_t ** _wpl = (bb_label_t **)alloca(sizeof(bb_label_t *) * (size_t)(n + 1)); int * _wpd = (int *)alloca(sizeof(int) * (size_t)(n + 1));
        bb_label_t ** _tpl = (bb_label_t **)alloca(sizeof(bb_label_t *) * (size_t)(n + 1)); bb_label_t ** _tpt = (bb_label_t **)alloca(sizeof(bb_label_t *) * (size_t)(n + 1));
        int * _tpd = (int *)alloca(sizeof(int) * (size_t)(n + 1)); int _tpn = 0;
    zd_depth_census(g_emitter.codegen_flat_chain_body_nodes, n, zd_on, zd_out, zd_gp, zd_wp, prefix);
    zd_check(g_emitter.codegen_flat_chain_body_nodes, n, zd_on, zd_out, zd_gp, zd_wp, zd_claim, zd_xb, zd_chn, zd_cut, zd_arm, prefix);
    if (g_emitter.dump_homes) emit_dump_homes(g_emitter.codegen_flat_chain_body_nodes, n, zd_on);
    if (g_emitter.dump_plan) emit_dump_plan(_pl, prefix);
    {
    if (g_emitter.codegen_flat_chain_body__zk0_on < 0) { const char * _e = getenv("SCRIP_ZD_CENSUS"); g_emitter.codegen_flat_chain_body__zk0_on = (_e && *_e == '1') ? 1 : 0; }
    if (g_emitter.codegen_flat_chain_body__zk0_on && g_emit_cfg && g_emit_cfg->spine_cells) {
        long _k_total = 0; int _armed = 0; int _unpriced_runs = 0;
        int _blocker_op[IR_OP_COUNT]; int _blocker_cnt[IR_OP_COUNT]; int _n_blockers = 0;
        int _globals_on_stack = 0;
        { extern int is_global(const char *); for (int _i = 0; _i < n; _i++) { if (zd_on[_i]) { _k_total += zd_k(g_emitter.codegen_flat_chain_body_nodes[_i]); _armed++;
            int _op5 = (int)g_emitter.codegen_flat_chain_body_nodes[_i]->op;
            if ((_op5 == IR_VAR || _op5 == IR_ASSIGN) && IR_LIT(g_emitter.codegen_flat_chain_body_nodes[_i]).sval && is_global(IR_LIT(g_emitter.codegen_flat_chain_body_nodes[_i]).sval) &&
                !graph_has_local(g_emit_cfg, IR_LIT(g_emitter.codegen_flat_chain_body_nodes[_i]).sval)) _globals_on_stack++; } } }
        for (int _i = 0; _i < n; _i++) {
            if (zd_on[_i]) continue;
            {
                int _op = (int)g_emitter.codegen_flat_chain_body_nodes[_i]->op; int _found = 0;
                for (int _b = 0; _b < _n_blockers; _b++) if (_blocker_op[_b] == _op) { _blocker_cnt[_b]++; _found = 1; break; }
                if (!_found && _n_blockers < IR_OP_COUNT) { _blocker_op[_n_blockers] = _op; _blocker_cnt[_n_blockers++] = 1; }
                _unpriced_runs++;
            }
        }
        int _bss_bytes = g_emit_cfg->jcon_value_region;
        fprintf(stderr, "[ZK-CENSUS] graph=%s icn_cells=1 n=%d armed=%d K_total=%ld refused_nodes=%d bss_arena=%d globals_on_stack=%d\n",
                prefix ? prefix : "?", n, _armed, _k_total, _unpriced_runs, _bss_bytes, _globals_on_stack);
        for (int _b = 0; _b < _n_blockers && _b < 8; _b++) {
            const char * _nm = bb_op_name((IR_e)_blocker_op[_b]);
            fprintf(stderr, "[ZK-CENSUS-BLOCKER]   %-30s count=%d\n", _nm ? _nm : "<unknown>", _blocker_cnt[_b]);
        }
    } }
    {
    if (g_emitter.codegen_flat_chain_body__plzk0_on < 0) { const char * _e = getenv("SCRIP_ZD_CENSUS"); g_emitter.codegen_flat_chain_body__plzk0_on = (_e && *_e == '1') ? 1 : 0; }
    if (g_emitter.codegen_flat_chain_body__plzk0_on) {
        long _k_total = 0; int _armed = 0; int _refused_nodes = 0;
        int _blocker_op[IR_OP_COUNT]; int _blocker_cnt[IR_OP_COUNT]; int _n_blockers = 0;
        for (int _i = 0; _i < n; _i++) {
            if (zd_on[_i]) { _k_total += zd_k(g_emitter.codegen_flat_chain_body_nodes[_i]); _armed++; }
        }
        for (int _i = 0; _i < n; _i++) {
            if (zd_on[_i]) continue;
            {
                int _op = (int)g_emitter.codegen_flat_chain_body_nodes[_i]->op; int _found = 0;
                for (int _b = 0; _b < _n_blockers; _b++) if (_blocker_op[_b] == _op) { _blocker_cnt[_b]++; _found = 1; break; }
                if (!_found && _n_blockers < IR_OP_COUNT) { _blocker_op[_n_blockers] = _op; _blocker_cnt[_n_blockers++] = 1; }
                _refused_nodes++;
            }
        }
        int _bss_bytes = g_emit_cfg->jcon_value_region;
        fprintf(stderr, "[PL-ZK-CENSUS] graph=%s pl_cells=1 n=%d armed=%d K_total=%ld refused_nodes=%d bss_arena=%d\n",
                prefix ? prefix : "?", n, _armed, _k_total, _refused_nodes, _bss_bytes);
        for (int _b = 0; _b < _n_blockers && _b < 8; _b++) {
            const char * _nm = bb_op_name((IR_e)_blocker_op[_b]);
            fprintf(stderr, "[PL-ZK-CENSUS-BLOCKER]   %-30s count=%d\n", _nm ? _nm : "<unknown>", _blocker_cnt[_b]);
        }
    } }
    struct zw5_stmt_s { int idx, cnt, base, stno; }; cv_t zw5_dv = { 0, 0, 0, 0 }; cv_t zw5_sv = { 0, 0, 0, 0 };
    bb_label_t * zw5_pool = (bb_label_t *)0; int zw5_base = 0;
    int * zw5_depth = (int *)0; struct zw5_stmt_s * zw5_stmt = (struct zw5_stmt_s *)0; int zw5_pool_stmts = 0;
     if (g_emitter.codegen_flat_chain_body__zw5_on < 0) { const char *_e = getenv("SCRIP_ZW5"); g_emitter.codegen_flat_chain_body__zw5_on = (_e && *_e == '1') ? 1 : 0; }
    if (g_emitter.codegen_flat_chain_body__zw5_on) { for (int _i = 0; _i < n; _i++) { if (g_emitter.codegen_flat_chain_body_nodes[_i]->op != IR_STATEMENT || !zd_on[_i]) continue;
        int _stno = (int)IR_LIT(g_emitter.codegen_flat_chain_body_nodes[_i]).ival; int _ndepths = 0;
        for (int _j = _i - 1; _j >= 0; _j--) { if (zd_on[_j] && zd_wp[_j] > 0) { int _w = zd_wp[_j]; int _dup = 0;
        for (int _d = 0; _d < _ndepths; _d++) if (CV_AT(zw5_dv, int, zw5_base + _d) == _w) { _dup = 1; break;
        } if (!_dup) { cv_reserve(&zw5_dv, (uint32_t)sizeof(int), (uint64_t)zw5_base + _ndepths + 1, "zw5_depth"); CV_AT(zw5_dv, int, zw5_base + _ndepths) = _w; _ndepths++;
        } } if (g_emitter.codegen_flat_chain_body_nodes[_j]->op == IR_STATEMENT) break; }
        if (_ndepths == 0) continue;
        cv_reserve(&zw5_sv, (uint32_t)sizeof(struct zw5_stmt_s), (uint64_t)zw5_pool_stmts + 1, "zw5_stmt"); CV_AT(zw5_sv, struct zw5_stmt_s, zw5_pool_stmts) = zw5_stmt_s{ _i, _ndepths, zw5_base,
            _stno }; zw5_pool_stmts++;
        zw5_base += _ndepths; }
      zw5_depth = (int *)zw5_dv.p; zw5_stmt = (struct zw5_stmt_s *)zw5_sv.p; zw5_pool = (bb_label_t *)ct_zalloc((size_t)(zw5_base > 0 ? zw5_base : 1), sizeof(bb_label_t));
          for (int _s = 0; _s < zw5_pool_stmts; _s++) for (int _d = 0; _d < zw5_stmt[_s].cnt; _d++)
          emit_label_initf(&zw5_pool[zw5_stmt[_s].base + _d], "%s_zw5s%d_ω_d%d", fam, zw5_stmt[_s].stno, zw5_depth[zw5_stmt[_s].base + _d]); }
    { g_emit.flat_all_zd = 0; { int _gen_ok = !g_emit.flat_gen; int _jmp_ok = !g_emit.flat_jmp_entry; if (!g_emit.flat_pat && _gen_ok && _jmp_ok && n > 0) { int _azd = 1;
        for (int _i = 0; _i < n; _i++) if (!zd_on[_i]) { _azd = 0; break; } g_emit.flat_all_zd = _azd;
            } } { if (g_emitter.codegen_flat_chain_body__lpd < 0) { const char * _e = getenv("SCRIP_LP_DIAG");
        g_emitter.codegen_flat_chain_body__lpd = (_e && *_e == '1') ? 1 : 0; } if (g_emitter.codegen_flat_chain_body__lpd) { int _arm = 0; for (int _i = 0; _i < n; _i++) if (zd_on[_i]) _arm++;
        fprintf(stderr, "[LP] prefix=%s n=%d armed=%d all_zd=%d region=%d jmp=%d pat=%d gen=%d\n", prefix, n, _arm, g_emit.flat_all_zd, (g_emit_cfg ? g_emit_cfg->jcon_value_region : -1),
        g_emit.flat_jmp_entry, g_emit.flat_pat, g_emit.flat_gen); } } } { extern void emit_fb_divergence_check(void); emit_fb_divergence_check(); }
    { if (g_emitter.codegen_flat_chain_body__tp < 0) { const char * e = getenv("SCRIP_TOP_PORTS"); g_emitter.codegen_flat_chain_body__tp = (e && *e == '1') ? 1 : 0; }
      int _cls_o =
          (!bare && !g_emit.flat_jmp_entry && !g_emit.zframe_graph && !g_emit.flat_lcl_proc && !g_emit.flat_pat && !g_emit.flat_gen && !g_gen_proc_active &&
          !(g_emit_cfg && g_emit_cfg->cell_frame) && !(g_emit_cfg && g_emit_cfg->body_root) && g_emit.flat_outer_nparams == 0) ? 1 : 0;
      if (_cls_o) for (int _i = 0; _i < n; _i++) if (g_emitter.codegen_flat_chain_body_nodes[_i]->op == IR_SUSPEND) { _cls_o = 0; break; }
      _top_hoist = (g_emitter.codegen_flat_chain_body__tp && _cls_o) ? 1 : 0;
      if (_top_hoist) {
          g_emit.flat_main_body_p = &lbl_α_body; g_emit.flat_main_ω_p = &lbl_ω;
          bb_emit_x86(bb_main_entry_bridge());
          emit_sep_rule('-'); emit_label_define_bb(&lbl_β); bb_emit_x86(bb_main_β());
          emit_sep_rule('-'); emit_label_define_bb(&lbl_γ); bb_emit_x86(bb_glue_outer_γ());
          emit_sep_rule('-'); emit_label_define_bb(&lbl_ω); bb_emit_x86(bb_glue_outer_ω());
          for (int _k = 1; _k <= 3; _k++) { int _has = 0; for (int _i = 0; _i < n; _i++) if (emit_floater_kind(g_emitter.codegen_flat_chain_body_nodes[_i]) == _k) { _has = 1; break;
              } if (!_has) continue; _flt_hoisted[_k] = 1;
              bb_label_t * _fa = emit_floater_label(_k); bb_label_t * _fg = emit_floater_label(1);
              g_emit.lbl_α_p = _fa; g_emit.lbl_α = _fa->name; g_emit.lbl_γ_p = _fg; g_emit.lbl_γ = _fg->name; g_emit.lbl_ω_p = &lbl_ω; g_emit.lbl_ω = lbl_ω.name; g_emit.lbl_β_p = _fa;
                  g_emit.lbl_β = _fa->name;
              g_emit.op_ival = _k; g_emit.op_define_role = (_k == 3) ? 0 : _k;
              int _uid0 = g_flat_node_id; emit_sep_rule('-'); bb_emit_x86(bb_main_floater(_k)); _flt_uid_burn[_k] = g_flat_node_id - _uid0; g_flat_node_id = _uid0; }
          g_emit.flat_main_body_p = (bb_label_t *)0; g_emit.flat_main_ω_p = (bb_label_t *)0;
      } }
    if (text_externalise && g_is_text && !fn_face_dead) emit_label_define_bb(&lbl_α);
    else if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc)) emit_label_define_bb(&lbl_α);
    if (g_is_text) g_emit_pos += 7;
    g_emit.flat_carve_total = 0; g_emit.flat_gen_rbp = 0; g_emit.flat_gen_above = 0; g_emit.op_zxb = 0;
    bb_label_t lbl_αblk; emit_label_initf(&lbl_αblk, "%s_αblk", fam);
    int _blk_graph = (g_emit.zframe_graph && !emit_resumable_frame() && g_emit_cfg && zls_g_block_args(g_emit_cfg) && !zls_g_det_block(g_emit_cfg)) ? 1 : 0;
    if (_blk_graph) { extern void xa_flat_block_staged_entry(void); xa_flat_block_staged_entry(); emit_label_define_bb(&lbl_αblk); }
    if (prefix && !strcmp(gc_map_entry_fam(prefix), "main") && !g_runtime.eval.rt_fragment_emit && g_emit_cfg && !g_emit.zframe_graph && !(icn_gen_regime() && g_emit.flat_gen) &&
        !g_emit.flat_lcl_proc &&
        !g_emit.flat_pat) { int _rg = g_emit_cfg->jcon_value_region;
        if (_rg >= 0 && !(_rg & 15)) { bb_emit_x86(emit_gc_map_register(_rg, _rg + 16, 0, GC_FRAME_MAP_ROOT | (g_emit_cfg->root_graph ? GC_FRAME_MAP_ROOT : 0u), 1, 0u)); } }
    g_emit.flat_pkt = (g_emit.zframe_graph && g_emit_cfg && g_emit_cfg->pkt_fragment) ? 1 : 0; g_emit.flat_pkt_cell = g_emit.flat_pkt ? g_emit_cfg->pkt_cell : -1;
        g_emit.flat_pkt_slot = g_emit.flat_pkt ? g_emit_cfg->pkt_slot : -1; g_emit.flat_pkt_chain_off = -1;
    g_emit.flat_pkt_walk_p = (bb_label_t *)0; g_emit.flat_pkt_chainω_p = (bb_label_t *)0;
    if (g_emit.zframe_graph) {
        if (g_emit.flat_pkt) {
            g_emit.flat_pkt_walk_p = emit_label_alloc("%s_pkwalk", fam); g_emit.flat_pkt_chainω_p = emit_label_alloc("%s_pkchω", fam);
            { extern void xa_flat_pkt_fresh_entry(void); xa_flat_pkt_fresh_entry(); }
            { bb_label_t lbl_pc; emit_label_initf(&lbl_pc, "%s_pkα", fam); emit_sep_rule('-'); emit_label_define_bb(&lbl_pc); if (!g_is_text) g_emit.flat_pkt_chain_off = (long)lbl_pc.offset; } }
        { extern void xa_flat_zframe_prologue(void); xa_flat_zframe_prologue(); }
    } else if (icn_gen_regime() && g_emit.flat_gen) {
        int kt2 = g_emit.flat_frame_bytes;
        int np = g_emit_cfg ? g_emit_cfg->nparams : 0;
        int nl = g_emit_cfg ? g_emit_cfg->nlocals : 0;
        int frame_total = kt2 + (np + nl) * 16;
        g_emit.flat_carve_total = frame_total; g_emit.flat_gen_rbp = frame_total;
        extern void rt_lcl_proc_args_install(void *, int, int);
        extern void rt_icn_zframe_args_install(void *, int, int);
        int _use_zframe_install = (g_emit_cfg && g_emit_cfg->cell_frame) ? 1 : 0;
        int carve = ((frame_total + 15) & ~15) + 48 + 8 + 16;
        std::string _gseed;
        { extern int zls_g_locals(const IR_graph_t *); extern int zls_g_region(const IR_graph_t *);
           if (g_emitter.codegen_flat_chain_body__en < 0) { const char * _e = getenv("SCRIP_LCL_SEED"); g_emitter.codegen_flat_chain_body__en = (_e && *_e == '0') ? 0 : 1; }
          int _lo = (g_emitter.codegen_flat_chain_body__en && g_emit_cfg) ? zls_g_locals(g_emit_cfg) : -1;
          int _rg = (g_emitter.codegen_flat_chain_body__en && g_emit_cfg) ? zls_g_value_region(g_emit_cfg) : -1;
          if (_lo >= 0 && _rg > 0) _lo = 0;
          if (_lo >= 0 && _rg > _lo && _rg <= frame_total)
              _gseed = flat_zero_fill(_lo, _rg - _lo);
          if (_use_zframe_install && _gseed.empty() && _rg > 0 && _rg <= frame_total)
              _gseed = x86("comment", "the block protocol: no C install runs, so the value region is zeroed here -- every local reads a null DESCR")
                     + flat_zero_fill(0, _rg);
          }
        extern int zls_g_block_args(const IR_graph_t *); extern int zls_g_block_base(const IR_graph_t *);
        int _gblk = (_use_zframe_install && zls_g_block_args(g_emit_cfg)) ? 1 : 0, _gw = _gblk ? 16 * np : 0;
        g_emit.flat_gen_above = carve - frame_total + _gw + 40;
        if (_gblk && zls_g_block_base(g_emit_cfg) != carve)
            { fprintf(stderr, "FATAL emit: generator %s's argument block lies at +%d by the layout and +%d by the carve -- the two must agree to the byte\n", prefix ? prefix : "?",
            zls_g_block_base(g_emit_cfg), carve); abort(); }
        bb_emit_x86( IF(_gblk,
            x86("comment",
            "the block protocol: the caller's argument block is at this entry rsp, A[i] = [entry+16i]; the five entry words lie above it, so gamma, omega and the anchor are read 16n higher"))
                  + x86("lea", "rax", RDQ("rsp", 0 - carve))
                  + x86("mov", RDQ("rax", frame_total + 0), "rbp")
                  + x86("mov", "rcx", RDQ("rsp", _gw + 0)) + x86("mov", RDQ("rax", frame_total + 8), "rcx")
                  + x86("mov", "rcx", RDQ("rsp", _gw + 8)) + x86("mov", RDQ("rax", frame_total + 16), "rcx")
                  + x86("lea", "rcx", RDQ("rsp", _gw + 40)) + x86("mov", RDQ("rax", frame_total + 24), "rcx")
                  + IF(x86_trace_hooks_on(), x86("mov", RDQ("rax", frame_total + 64), 0L))
                  + x86("lea", "rbp", RDQ("rax", frame_total))
                  + x86("mov", "rsp", "rax")
                  +
                      emit_gc_map_register(g_emit_cfg ? g_emit_cfg->jcon_value_region : 0, frame_total, frame_total - (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) - 16 + 32,
                      GC_FRAME_MAP_GEN_ANCHOR | GC_FRAME_MAP_ICN_PROC | (_gblk ? GC_FRAME_MAP_ICN_BLOCK : 0u) | (g_emit_cfg && g_emit_cfg->root_graph ? GC_FRAME_MAP_ROOT : 0u), 0,
                      GC_LINK_Q(frame_total + 8, frame_total, frame_total + 24, GC_LINK_R_IND))
                  + _gseed
                  + IF(_gblk, icn_entry_gva())
                  + IF(!_gblk, x86("mov", "rdi", "rsp") + x86("mov32", "esi", (long)np) + x86("mov32", "edx", (long)nl)
                  + x86("call", _use_zframe_install ? "rt_icn_zframe_args_install" : "rt_lcl_proc_args_install",
                        (uint64_t)(uintptr_t)(void *)(_use_zframe_install ? rt_icn_zframe_args_install : rt_lcl_proc_args_install))));
        if (_use_zframe_install) {
            const char * _gn = (prefix && strncmp(prefix, "proc_", 5) == 0) ? prefix + 5 : prefix;
            icn_register_local_offsets(_gn);
        }
    } else if (g_emit.flat_lcl_proc) {
        int kt2 = g_emit.flat_frame_bytes;
        int np = g_emit_cfg ? g_emit_cfg->nparams : 0;
        int nl = g_emit_cfg ? g_emit_cfg->nlocals : 0;
        int frame_total = kt2 + (np + nl) * 16;
        int host_frame_base = frame_total;
        g_emit.flat_carve_total = frame_total;
        (void)host_frame_base;
        std::string _pin;
        if (x86_hb_on()) _pin = x86("mov", RDQ("rsp", frame_total - 8), "rbp") + x86("mov", "rbp", "rsp");
        extern void rt_lcl_proc_args_install(void *, int, int);
        extern void rt_icn_zframe_args_install(void *, int, int);
        int _use_zframe_install = (g_emit_cfg && g_emit_cfg->cell_frame) ? 1 : 0;
        std::string _lseed;
         if (g_emitter.codegen_flat_chain_body__iws < 0) { const char * _e = getenv("SCRIP_ICN_WIRE_STACK"); g_emitter.codegen_flat_chain_body__iws = (_e && *_e == (char)48) ? 0 : 1; }
        { extern int zls_g_locals(const IR_graph_t *); extern int zls_g_region(const IR_graph_t *);
           if (g_emitter.codegen_flat_chain_body__en_0 < 0) { const char * _e = getenv("SCRIP_LCL_SEED"); g_emitter.codegen_flat_chain_body__en_0 = (_e && *_e == '0') ? 0 : 1; }
          int _lo = (g_emitter.codegen_flat_chain_body__en_0 && g_emit_cfg) ? zls_g_locals(g_emit_cfg) : -1;
          int _rg = (g_emitter.codegen_flat_chain_body__en_0 && g_emit_cfg) ? zls_g_value_region(g_emit_cfg) : -1, _hz = frame_total - (g_emitter.codegen_flat_chain_body__iws ? 8 : 24);
          if (_lo >= 0 && _rg > 0) _lo = 0;
          if (_lo >= 0 && _rg > _lo && _rg <= frame_total - 32)
              _lseed =
                  x86("comment",
                  "LCL-SEED: NULVCL the value region AND the header up to the saved rbp [___+0, ___+hz), so no dead activation's pointer survives inside this frame, the header's unwritten words "
                      "are DT_SNUL cells (ARCH-GC section 13), and lexical locals read unbound")
                     + flat_zero_fill(_lo, _hz - _lo);
          if (_use_zframe_install && _lseed.empty() && _hz > 0)
              _lseed = x86("comment", "the block protocol: no C install runs, so the frame [0, hz) is zeroed here -- every local reads a null DESCR")
                     + flat_zero_fill(0, _hz);
          }
        extern int zls_g_block_args(const IR_graph_t *); extern int zls_g_block_base(const IR_graph_t *);
        int _blk = (_use_zframe_install && zls_g_block_args(g_emit_cfg)) ? 1 : 0;
        int _aoff = _blk ? frame_total : 16;
        if (_blk && zls_g_block_base(g_emit_cfg) != frame_total)
            { fprintf(stderr, "FATAL emit: %s's argument block lies at +%d by the layout and +%d by the prologue -- the two must agree to the byte\n", prefix ? prefix : "?",
            zls_g_block_base(g_emit_cfg), frame_total); abort(); }
        std::string _inst = _use_zframe_install ? icn_entry_gva() + icn_main_args_inline(np, _blk) : std::string();
        if (g_is_text) {
            char _lp[512]; int _lz = 0;
            _lz += g_emitter.codegen_flat_chain_body__iws ? snprintf(_lp + _lz, (int)sizeof(_lp) - _lz, "sub rsp, %d\n", frame_total)
                        : snprintf(_lp + _lz, (int)sizeof(_lp) - _lz, "sub rsp, %d\nmov qword ptr [rsp + %d], rcx\nmov qword ptr [rsp + %d], rdx\n", frame_total, frame_total - 24, frame_total - 16);
            emit_text_n(_lp, strlen(_lp));
            if (!_lseed.empty()) bb_emit_x86(_lseed);
                bb_emit_x86(emit_gc_map_register(g_emit_cfg ? g_emit_cfg->jcon_value_region : 0, frame_total, frame_total - (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) - 16,
                ((g_emit_cfg && g_emit_cfg->root_graph) ? GC_FRAME_MAP_ROOT : 0u) |
                (_use_zframe_install ? (GC_FRAME_MAP_ICN_PROC | (_blk ? GC_FRAME_MAP_ICN_BLOCK : 0u) | (x86_hb_on() ? GC_FRAME_MAP_ICN_PINNED : 0u)) : 0u), 0,
                GC_LINK_Q(g_emitter.codegen_flat_chain_body__iws ? frame_total + (_blk ? 16 * np : 0) : frame_total - 24, frame_total - 8,
                    g_emitter.codegen_flat_chain_body__iws ? frame_total + (_blk ? 16 * np : 0) : frame_total, _pin.empty() ? GC_LINK_RBP_KEPT : 0u))
                ); if (!_pin.empty()) bb_emit_x86(_pin);
            if (_use_zframe_install) bb_emit_x86(_inst);
            else { _lz = 0; _lz += snprintf(_lp + _lz, (int)sizeof(_lp) - _lz, "mov rdi, rsp\nmov esi, %d\nmov edx, %d\ncall %s@PLT\n", np, nl, "rt_lcl_proc_args_install");
                   emit_text_n(_lp, strlen(_lp)); }
        } else {
            if (frame_total <= 127) { ef_b3(0x48, 0x83, 0xEC); ef_b1((uint8_t)frame_total); } else { ef_b3(0x48, 0x81, 0xEC); bb_emit_u32((uint32_t)frame_total); }
            if (!g_emitter.codegen_flat_chain_body__iws) {
            { int _d = frame_total - 24; ef_b2(0x48, 0x89); if (_d >= -128 && _d <= 127) { ef_b3(0x4C, 0x24, (uint8_t)(int8_t)_d); } else { ef_b2(0x8C, 0x24); bb_emit_u32((uint32_t)_d); } }
            { int _d = frame_total - 16; ef_b2(0x48, 0x89); if (_d >= -128 && _d <= 127) { ef_b3(0x54, 0x24, (uint8_t)(int8_t)_d); } else { ef_b2(0x94, 0x24); bb_emit_u32((uint32_t)_d); } }
            { if (g_emitter.codegen_flat_chain_body__hd < 0) { const char * _e = getenv("SCRIP_ICN_HDR_DEAD"); g_emitter.codegen_flat_chain_body__hd = (_e && *_e == '1') ? 1 : 0;
                } if (g_emitter.codegen_flat_chain_body__hd) { int _d = frame_total - 8; ef_b2(0x48, 0x89);
                if (_d >= -128 && _d <= 127) { ef_b3(0x6C, 0x24, (uint8_t)(int8_t)_d); } else { ef_b2(0xAC, 0x24); bb_emit_u32((uint32_t)_d); } } }
            }
            if (!_lseed.empty()) bb_emit_x86(_lseed);
                bb_emit_x86(emit_gc_map_register(g_emit_cfg ? g_emit_cfg->jcon_value_region : 0, frame_total, frame_total - (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) - 16,
                ((g_emit_cfg && g_emit_cfg->root_graph) ? GC_FRAME_MAP_ROOT : 0u) |
                (_use_zframe_install ? (GC_FRAME_MAP_ICN_PROC | (_blk ? GC_FRAME_MAP_ICN_BLOCK : 0u) | (x86_hb_on() ? GC_FRAME_MAP_ICN_PINNED : 0u)) : 0u), 0,
                GC_LINK_Q(g_emitter.codegen_flat_chain_body__iws ? frame_total + (_blk ? 16 * np : 0) : frame_total - 24, frame_total - 8,
                    g_emitter.codegen_flat_chain_body__iws ? frame_total + (_blk ? 16 * np : 0) : frame_total, _pin.empty() ? GC_LINK_RBP_KEPT : 0u))
                ); if (!_pin.empty()) bb_emit_x86(_pin);
            if (_use_zframe_install) bb_emit_x86(_inst);
            else {
            ef_b3(0x48, 0x89, 0xE7);
            ef_b1(0xBE); bb_emit_u32((uint32_t)np);
            ef_b1(0xBA); bb_emit_u32((uint32_t)nl);
            { uint64_t _fn = (uint64_t)(uintptr_t)(void *)rt_lcl_proc_args_install; ef_b2(0x48, 0xB8); bb_emit_u64(_fn); ef_b2(0xFF, 0xD0); } }
        }
        if (g_emitter.codegen_flat_chain_body__iws && _use_zframe_install) {
            const char * _apn = (g_emit_cfg && g_emit_cfg->root_graph) ? "main" : (strncmp(prefix, "proc_", 5) == 0) ? prefix + 5 : prefix;
            extern int * const rt_k_level_p;
            bb_emit_x86( x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
                     + x86("mov", "rax", RDQ("rax", 0))
                     + x86("add", RDD("rax", 0), (long)1)
                     + x86("mov", "ecx", RDD("rax", 0))
                     + x86("movsxd", "rcx", "ecx")
                     + x86("sub", "rcx", (long)1)
                     + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
                     + x86("mov", RDQ("rax", GRT_AT(kw_fnclevel)), "rcx"));
            bb_emit_x86(icn_trace_tap(_apn, 1, np, _aoff));
            icn_register_local_offsets(_apn);
        }
    }
    if (!bare) emit_label_define_bb(&lbl_α_body);
    if (lbl_α_orig_p && xa_flat_class_c_pred() && !g_runtime.eval.rt_fragment_emit) emit_label_define_bb(lbl_α_orig_p);
    { extern std::string bb_zdp_origin(long); extern int x86_zdp_on_c(void); if (x86_zdp_on_c()) bb_emit_x86(bb_zdp_origin((long)0)); } { if (x86_zdp_rbp_on()) bb_emit_x86(x86_zsm_ev(0)); }
    int _sigk = 0;
    if (xa_flat_class_c_pred()) xa_flat_chain_prologue(fam, &_sigk);
    if (!bare && blob_frame_scope()) blob_first_guard();
    { int _bfb = blob_frame_bytes(); if (_bfb > 0) { blob_layout_build(_bfb);
        bb_emit_x86( x86("push", "rbp") + x86("mov", "rbp", "rsp") + x86("sub", "rsp", (long)(blob_carve_bytes() + blob_carve_pad())) +
        emit_gc_map_register(_bfb + 16, _bfb + 16, 24, GC_FRAME_MAP_BLOB, 2, GC_LINK_Q(8, 0, 8, 0u)) + blob_zero_fill(_bfb) +
        IF(blob_cont_copy(), x86("mov", "rcx", RDQ("rbp", 8)) + x86("mov", RDQ("rbp", -8), "rcx") + x86("mov", "rcx", RDQ("rbp", 16)) + x86("mov", RDQ("rbp", -16), "rcx")) +
        x86("mov", RDQ("rbp", -32), (long)DT_P) + x86("mov", RDQ("rbp", -24), "rdx") + IF(sn4_blob_casmark(), x86("mov", RDQ("rbp", -40), "r12"))); } else if (blob_frame_scope()) { _spf = 1;
        int _need = sn4_blob_casmark(); for (int _q = 0; _q < n; _q++) if (blob_patv_reader(g_emitter.codegen_flat_chain_body_nodes[_q])) _need = 1; _sph = _need ? (sn4_blob_casmark() ? 32 : 16) : 0;
            _spgd = -1;
        for (int _q = 0; _q < n; _q++) { IR_t * _gt = zd_chase(g_emitter.codegen_flat_chain_body_nodes[_q]->γ.node); if (_gt && _gt->op != IR_SUCCEED) continue; int _d = zd_on[_q] ? zd_out[_q] : -2;
        _spgd = (_spgd == -1 || _spgd == _d) ? _d : -2;
        } if (_sph)
        bb_emit_x86(x86("sub", "rsp", (long)_sph) + x86("mov", RDQ("rsp", _sph - 16), (long)DT_P) + x86("mov", RDQ("rsp", _sph - 8), "rdx") +
        IF(_sph == 32, x86("mov", RDQ("rsp", 0), (long)DT_I) + x86("mov", RDQ("rsp", 8), "r12"))); }
    }
    { extern int g_flat_outer_nparams; int _legacy = (!g_emit.flat_jmp_entry && !g_emit.flat_pat && !g_emit.flat_gen && !g_gen_proc_active && !g_emit.zframe_graph && !g_emit.flat_lcl_proc);
        extern int g_glue_entered; g_glue_entered = (g_emit.flat_outer_nparams == 0 && _legacy) ? 1 : 0; if (g_glue_entered) bb_emit_x86(x86_main_prologue()); {
        if (g_emitter.codegen_flat_chain_body__gluo < 0) { const char * e = getenv("SCRIP_GLUEO"); g_emitter.codegen_flat_chain_body__gluo = (e && *e == '0') ? 0 : 1;
            } if (g_emitter.codegen_flat_chain_body__gluod < 0) { const char * e = getenv("SCRIP_GLUEO_DIAG");
        g_emitter.codegen_flat_chain_body__gluod = (e && *e == '1') ? 1 : 0; } extern int g_glue_o_sup;
            g_glue_o_sup = (g_emitter.codegen_flat_chain_body__gluo && g_glue_entered && !emit_rec_pin()) ? 1 : 0; if (g_glue_o_sup) g_glue_entered = 0;
        if (g_emitter.codegen_flat_chain_body__gluod)
        fprintf(stderr, "[GLUEO] graph=%s entered=%d rec_pin=%d deep=%d pat=%d gen=%d -> closed_loop_suppressed=%d\n", g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "<anon>", g_glue_entered,
        emit_rec_pin() ? 1 : 0, g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_glue_o_sup); } }
    nidx_build(g_emitter.codegen_flat_chain_body_nodes, n);
    bb_label_t **lbls = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_y = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_t = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **na_s = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t ***fc_sig = (bb_label_t ***)alloca(sizeof(bb_label_t **) * n);
    bb_label_t **na_f = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **na_fo = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **bxs = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    g_emitter.g_suspend_resume_slot = -1;
    if (g_gen_proc_active && g_emit_cfg && g_emit_cfg->resume_slot >= 0)
        for (int _si = 0; _si < n; _si++) if (g_emitter.codegen_flat_chain_body_nodes[_si]->op == IR_SUSPEND) { g_emitter.g_suspend_resume_slot = g_emit_cfg->resume_slot; break; }
    if (g_emitter.g_suspend_resume_slot < 0 && g_resumable_callable_active && g_emit_cfg && g_emit_cfg->resume_slot >= 0)
        g_emitter.g_suspend_resume_slot = g_emit_cfg->resume_slot;
     int id = g_emitter.codegen_flat_chain_body__diag_chain_seq++;
    int own_mark = -1;
    { extern int zls_off(const IR_t *);
        for (int _oi = 0; _oi < n; _oi++) if (g_emitter.codegen_flat_chain_body_nodes[_oi]->op == IR_MATCH_BEGIN) { int _ho = zls_off(g_emitter.codegen_flat_chain_body_nodes[_oi]);
        if (_ho >= 0) own_mark = _ho + 16; break; } }
    for (int i = 0; i < n; i++) {
        int _uid = g_flat_node_id++; const char * _kn = flat_label_kind(g_emitter.codegen_flat_chain_body_nodes[i]->op);
        lbls[i] = emit_label_alloc("n%d_%s_α", _uid, _kn);
        { int _flk = emit_floater_kind(g_emitter.codegen_flat_chain_body_nodes[i]); if (_flk) lbls[i] = emit_floater_label(_flk); }
        if (g_emit_cfg && g_emit_cfg->n_balias > 0) { for (int _bi = 0; _bi < g_emit_cfg->n_balias; _bi++) if (g_emit_cfg->balias_node[_bi] == g_emitter.codegen_flat_chain_body_nodes[i]) { lbls[i] =
            emit_label_alloc("%s", g_emit_cfg->balias_name[_bi]); break; } }
        if (g_emit_cfg && g_emit_cfg->n_dentry > 0) { for (int _dq = 0; _dq < g_emit_cfg->n_dentry; _dq++) if (g_emit_cfg->dentry_entry[_dq] == g_emitter.codegen_flat_chain_body_nodes[i])
            g_emit_cfg->dentry_name[_dq] = lbls[i]->name;
            }
        betas[i] = emit_label_alloc("n%d_%s_β", _uid, _kn);
        ra_y[i] = (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_REPALT) ? emit_label_alloc(".L%s_γ_%d_ry", _kn, _uid) : NULL;
        ra_t[i] = (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_REPALT) ? emit_label_alloc(".L%s_ω_%d_rt", _kn, _uid) : NULL;
        na_s[i] = emit_glue_box(g_emitter.codegen_flat_chain_body_nodes[i], 0) ? emit_label_alloc(".L%s_γ_%d_as", _kn, _uid) : NULL;
        { int _naf_on = emit_glue_box(g_emitter.codegen_flat_chain_body_nodes[i], 1);
          na_f[i] = _naf_on ? emit_label_alloc(".L%s_γ_%d_af", _kn, _uid) : NULL;
          na_fo[i] = _naf_on ? emit_label_alloc(".L%s_ω_%d_af", _kn, _uid) : NULL; }
        bxs[i] = emit_label_alloc("n%d_%s_bx", _uid, _kn);
        fc_sig[i] = NULL;
        if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_ALTERNATE && g_emitter.codegen_flat_chain_body_nodes[i]->n_operands > 0) {
            int _N = (int)(g_emitter.codegen_flat_chain_body_nodes[i]->n_operands / 2);
            fc_sig[i] = (bb_label_t **)alloca(sizeof(bb_label_t *) * _N);
            for (int _j = 0; _j < _N; _j++) fc_sig[i][_j] = emit_label_alloc(".L%s_γ_%d_s%d", _kn, _uid, _j);
        }
    }
    unsigned char *bused = (unsigned char *)alloca(n > 0 ? n : 1);
    flat_beta_used_scan(g_emitter.codegen_flat_chain_body_nodes, n, bused);
    for (int _q = 0; _q < 2 * n; _q++) if (zd_chn[_q] >= 0) bused[zd_chn[_q]] = 1;
    { if (g_emitter.codegen_flat_chain_body__uwl < 0) { const char *_e = getenv("SCRIP_UNWIND"); g_emitter.codegen_flat_chain_body__uwl = (_e && *_e == '0') ? 0 : 1; }
      if (g_emitter.codegen_flat_chain_body__uwl) for (int _i = 0; _i < n; _i++) {
          int _hi = _i;
          if (!zd_on[_i] || zd_wp[_i] <= 0) continue;
          if (g_emitter.codegen_flat_chain_body_nodes[_i]->op == IR_STATEMENT || g_emitter.codegen_flat_chain_body_nodes[_i]->op == IR_STATEMENT_BEGIN ||
              g_emitter.codegen_flat_chain_body_nodes[_i]->op == IR_STATEMENT_END) continue;
          { int _bg = 0; for (int _j = _i - 1; _j >= 0; _j--) { int _o = (int)g_emitter.codegen_flat_chain_body_nodes[_j]->op; if (_o == IR_STATEMENT_BEGIN) { _bg = 1; break;
              } if (_o == IR_STATEMENT_END || _o == IR_STATEMENT) break;
              } if (!_bg) continue; }
          { IR_t *_ti = zd_chase(g_emitter.codegen_flat_chain_body_nodes[_i]->ω.node); if (!_ti) continue;
            for (int _j = _i - 1; _j >= 0; _j--) {
                int _o = (int)g_emitter.codegen_flat_chain_body_nodes[_j]->op; if (_o == IR_STATEMENT_BEGIN || _o == IR_STATEMENT_END || _o == IR_STATEMENT) break;
                if (zd_on[_j] && zd_wp[_j] > 0 && flat_unwind_beta(g_emitter.codegen_flat_chain_body_nodes[_j]) && zd_chase(g_emitter.codegen_flat_chain_body_nodes[_j]->ω.node) == _ti) {
                    if ((zd_wp[_i] + zd_k(g_emitter.codegen_flat_chain_body_nodes[_i])) - (zd_wp[_j] + zd_k(g_emitter.codegen_flat_chain_body_nodes[_j])) >= 0) bused[_j] = 1; break; } } } } }
    bb_label_t *resume_init_lbl = NULL;
    if (g_emitter.g_suspend_resume_slot >= 0 && g_gen_proc_active) {
        for (int _si = 0; _si < n; _si++) if (g_emitter.codegen_flat_chain_body_nodes[_si]->op == IR_SUSPEND) { resume_init_lbl = betas[_si]; break; }
    }
    if (resume_init_lbl) {
        if (g_is_text) {
            char _init[fmt_len("lea rax, [rip + %s]\n", resume_init_lbl->name)];
            snprintf(_init, sizeof _init, "lea rax, [rip + %s]\n", resume_init_lbl->name);
            emit_text_n(_init, strlen(_init));
        } else {
            ef_b3(0x48, 0x8D, 0x05); bb_emit_patch_rel32(resume_init_lbl);
        }
        bb_emit_x86(x86("mov", FRQ(g_emitter.g_suspend_resume_slot), "rax"));
    }
    for (int _li = 0; _li < n && !zls_g_home_frame(g_emit_cfg); _li++) if (g_emitter.codegen_flat_chain_body_nodes[_li]->op == IR_LIMIT) {
        g_emit.op_off = drive_value_slot(g_emitter.codegen_flat_chain_body_nodes[_li]);
        bb_emit_x86(bb_limit_init());
    }
    if (g_emit_cfg && n_alt > 1 && n_alt <= 4096 && emit_resumable_frame() && !g_emit_cfg->root_graph && !g_emit.flat_pkt && g_emit_cfg->nparams >= 1 && g_emit_cfg->pnames && g_emit_cfg->pnames[0] &&
        g_emit_cfg->alt_entry) {
        const char * _p0 = g_emit_cfg->pnames[0]; ix_slot = bb_varslot_peek(_p0);
        int * _ktag = (int *)alloca(sizeof(int) * (size_t)n_alt); int64_t * _kval = (int64_t *)alloca(sizeof(int64_t) * (size_t)n_alt); int _nvar = 0, _nvc = -1;
        for (int _ak = 0; _ak < n_alt; _ak++) { IR_t * _e = g_emit_cfg->alt_entry[_ak]; _ktag[_ak] = 0; _kval[_ak] = 0;
            { int _sg = 0; while (_e && _e->op == IR_SUCCEED && _e->γ.node && _sg++ < 4096) _e = _e->γ.node; }
            if (_e && (_e->op == IR_UNIFY_CONST || _e->op == IR_UNIFY_STRUCT) && _e->n_operands >= 3 && _e->operands[0] && _e->operands[0]->op == IR_VAR_REF && IR_LIT(_e->operands[0]).sval &&
                !strcmp(IR_LIT(_e->operands[0]).sval, _p0) && _e->operands[2]) {
                IR_t * _k = _e->operands[2];
                if (_e->op == IR_UNIFY_CONST && _k->op == IR_LIT_ATOM) { _ktag[_ak] = DT_PLATOM; _kval[_ak] = (int64_t)(unsigned)prolog_atom_intern_lit(IR_LIT(_k).sval ? IR_LIT(_k).sval : ""); }
                else if (_e->op == IR_UNIFY_CONST && _k->op == IR_LIT_INTEGER) { _ktag[_ak] = DT_I; _kval[_ak] = (int64_t)IR_LIT(_k).ival; }
                else if (_e->op == IR_UNIFY_STRUCT && _k->op == IR_LIT_INTEGER && prolog_functor_arity((int)IR_LIT(_k).ival) >= 1) { _ktag[_ak] = DT_PLREF; _kval[_ak] = (int64_t)IR_LIT(_k).ival; } }
            if (!_ktag[_ak]) { _nvar++; _nvc = _ak; } }
        if (ix_slot >= 0 && _nvar < n_alt) {
            ix_tag = (int *)alloca(sizeof(int) * (size_t)n_alt); ix_val = (int64_t *)alloca(sizeof(int64_t) * (size_t)n_alt); ix_code = (int *)alloca(sizeof(int) * (size_t)n_alt);
            const int _scap = n_alt * 8 + 64; int _nsi = 0, _vset = -9;
            ix_sets = (int *)alloca(sizeof(int) * (size_t)_scap); ix_set_beg = (int *)alloca(sizeof(int) * (size_t)(n_alt + 2)); ix_set_len = (int *)alloca(sizeof(int) * (size_t)(n_alt + 2));
            auto _setcode = [&](int ct, int64_t kv, int keyed) -> int {
                int _m = 0, _who = -1, _b0 = _nsi;
                for (int _b = 0; _b < n_alt; _b++) if (!_ktag[_b] || (keyed && _ktag[_b] == ct && _kval[_b] == kv)) { _m++; _who = _b; }
                if (_m == 0) return -1;
                if (_m == 1) return _who;
                if (_m == n_alt || _nsi + _m > _scap || ix_nset >= n_alt + 2) return -2;
                for (int _b = 0; _b < n_alt; _b++) if (!_ktag[_b] || (keyed && _ktag[_b] == ct && _kval[_b] == kv)) ix_sets[_nsi++] = _b;
                ix_set_beg[ix_nset] = _b0; ix_set_len[ix_nset] = _m; return -3 - ix_nset++; };
            for (int _c = 0; _c < 3; _c++) { int _nk = 0, _n0 = ix_n, _ns0 = ix_nset, _si0 = _nsi; const int _ct = _c == 0 ? (int)DT_PLREF : _c == 1 ? (int)DT_PLATOM : (int)DT_I;
                for (int _ak = 0; _ak < n_alt; _ak++) { if (_ktag[_ak] != _ct) continue; int _dup = 0;
                    for (int _q = _n0; _q < ix_n && !_dup; _q++) if (ix_val[_q] == _kval[_ak]) _dup = 1;
                    if (_dup) continue;
                    ix_tag[ix_n] = _ct; ix_val[ix_n] = _kval[_ak]; ix_code[ix_n] = _setcode(_ct, _kval[_ak], 1); ix_n++; _nk++; }
                { int _fb; if (_nk > 64) { ix_n = _n0; ix_nset = _ns0; _nsi = _si0; _fb = -2; } else { if (_vset == -9) _vset = _setcode(0, 0, 0); _fb = _vset; }
                  if (_c == 0) ix_fbr = _fb; else if (_c == 1) ix_fba = _fb; else ix_fbi = _fb; } }
            ix_switch = emit_label_alloc("%s_switch", fam); } }
    if (ix_switch) emit_jmp_label(ix_switch, JMP_JMP);
    else if (flat_empty_body_fail && n > 0) emit_jmp_label(alt_step_lbl && n_alt > 1 ? alt_step_lbl : &lbl_ω, JMP_JMP);
    int _bx_open = -1;
    int *repalt_of = (int *)alloca(sizeof(int) * (n > 0 ? n : 1)); for (int _q = 0; _q < n; _q++) repalt_of[_q] = -1;
    for (int _r = 0; _r < n; _r++) if (g_emitter.codegen_flat_chain_body_nodes[_r]->op == IR_REPALT && g_emitter.codegen_flat_chain_body_nodes[_r]->n_operands > 0) { int _k =
        nidx(g_emitter.codegen_flat_chain_body_nodes, n, g_emitter.codegen_flat_chain_body_nodes[_r]->operands[0]); if (_k >= 0 && repalt_of[_k] < 0) repalt_of[_k] = _r;
        }
    std::unordered_map<const IR_t *, int> repalt_by_fail, repalt_by_op0;
    for (int _r = 0; _r < n; _r++) {
        IR_t * _rn = g_emitter.codegen_flat_chain_body_nodes[_r];
        if (_rn->op != IR_REPALT) continue;
        if (_rn->n_operands > 2) repalt_by_fail.emplace(_rn->operands[2], _r);
        if (_rn->n_operands > 0) repalt_by_op0.emplace(_rn->operands[0], _r);
    }
    std::unordered_map<const bb_label_t *, int> beta_of; beta_of.reserve((size_t)n * 2 + 1); for (int _q = 0; _q < n; _q++) if (betas[_q]) beta_of.emplace(betas[_q], _q);
    int _hk_c = -1, _hk_upto = 0;
    if (g_emit_cfg && n_alt > 1 && g_emit_cfg->alt_entry && !g_emit_cfg->root_graph) {
        IR_t * _e0 = g_emit_cfg->alt_entry[0];
        { int _sg = 0; while (_e0 && _e0->op == IR_SUCCEED && _e0->γ.node && _sg++ < 4096) _e0 = _e0->γ.node; }
        if (_e0 && _e0->op == IR_SUCCEED && nidx(g_emitter.codegen_flat_chain_body_nodes, n, _e0) < 0) {
            emit_jmp_label(g_emit_cfg->alt_redo[0] ? g_emitter.codegen_flat_chain_body_ret_tr[0] : &lbl_γ, JMP_JMP);
        }
    }
    for (int i = 0; i < n; i++) {
        { int _fk = emit_floater_kind(g_emitter.codegen_flat_chain_body_nodes[i]); if (_fk && _flt_hoisted[_fk]) { g_flat_node_id += _flt_uid_burn[_fk]; continue; } }
        { if (g_emitter.codegen_flat_chain_body__sz < 0) { const char *_e = getenv("SCRIP_ASM_SYMSIZE"); g_emitter.codegen_flat_chain_body__sz = (_e && _e[0] == '0') ? 0 : 1; }
          if (g_emitter.codegen_flat_chain_body__sz && g_is_text) {
              if (_bx_open >= 0) emit_textf(".size %s, .-%s\n", bxs[_bx_open]->name, bxs[_bx_open]->name);
              emit_textf(".type %s, @function\n%s:\n", bxs[i]->name, bxs[i]->name); _bx_open = i; } }
        { if (g_emitter.codegen_flat_chain_body__beo < 0) { const char *_e = getenv("SCRIP_BETA_ELIDE_OFF"); g_emitter.codegen_flat_chain_body__beo = (_e && _e[0] == '1') ? 1 : 0;
            } g_emit.op_beta_dead = (g_emitter.codegen_flat_chain_body__beo || bused[i]) ? 0 : 1; }
        g_emit.op_wpop = 0;
        { extern int zls_off(const IR_t *); extern int zls_result_off(const IR_t *); extern int zls_node_bytes(const IR_t *); int _zo = zls_off(g_emitter.codegen_flat_chain_body_nodes[i]);
            int _zr = zls_result_off(g_emitter.codegen_flat_chain_body_nodes[i]);
          g_emit.op_own_mark = own_mark; g_emit.op_own_ci = (_zo >= 0) ? ((_zr >= 0 ? _zr : _zo) + zls_node_bytes(g_emitter.codegen_flat_chain_body_nodes[i])) : 0; }
        bb_label_t *node_γ = &lbl_γ;
        bb_label_t *node_ω = &lbl_ω;
        IR_t *gtgt = g_emitter.codegen_flat_chain_body_nodes[i]->γ.node;
        IR_t *otgt = g_emitter.codegen_flat_chain_body_nodes[i]->ω.node;
        int gamma_is_beta = port_sz_beta(g_emitter.codegen_flat_chain_body_nodes[i]->γ.sz);
        int omega_is_beta = port_sz_beta(g_emitter.codegen_flat_chain_body_nodes[i]->ω.sz);
        int gamma_is_sig = (g_emitter.codegen_flat_chain_body_nodes[i]->γ.sz[0] == (char)0xcf && (unsigned char)g_emitter.codegen_flat_chain_body_nodes[i]->γ.sz[1] == 0x83);
        int gamma_is_phi = (g_emitter.codegen_flat_chain_body_nodes[i]->γ.sz[0] == (char)0xcf && (unsigned char)g_emitter.codegen_flat_chain_body_nodes[i]->γ.sz[1] == 0x86);
        int omega_is_phi = (g_emitter.codegen_flat_chain_body_nodes[i]->ω.sz[0] == (char)0xcf && (unsigned char)g_emitter.codegen_flat_chain_body_nodes[i]->ω.sz[1] == 0x86);
        { int _gg = 0; IR_t * _g = gtgt; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!gamma_is_beta) gamma_is_beta = port_sz_beta(_g->γ.sz);
            if (!gamma_is_sig) gamma_is_sig = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x83);
            if (!gamma_is_phi) gamma_is_phi = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x86); gtgt = _g->γ.node; _g = gtgt; } }
        { int _gg = 0; IR_t * _o = otgt; while (_o && _o->op == IR_GOTO && _gg++ < 128) { if (!omega_is_beta) omega_is_beta = port_sz_beta(_o->γ.sz);
            if (!omega_is_phi) omega_is_phi = (_o->γ.sz[0] == (char)0xcf && (unsigned char)_o->γ.sz[1] == 0x86); otgt = _o->γ.node; _o = otgt; } }
        { int _op0 = (int)g_emitter.codegen_flat_chain_body_nodes[i]->op; int _always_exits = (_op0 == IR_RETURN || _op0 == IR_SUSPEND || _op0 == IR_CORET || _op0 == IR_COFAIL);
          if (!_always_exits) { int _sg = 0; while (gtgt && gtgt->op == IR_SUCCEED && gtgt->γ.node && _sg++ < 4096) gtgt = gtgt->γ.node; }
          if (!_always_exits) { int _sg = 0; while (otgt && otgt->op == IR_SUCCEED && otgt->γ.node && _sg++ < 4096) otgt = otgt->γ.node; } }
        int omega_is_retry = omega_is_beta && !beta_is_stmt_land(otgt);
        { int k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, gtgt); if (k >= 0) {
            node_γ = (gamma_is_phi && na_f[k]) ? na_f[k] : (gamma_is_sig && na_s[k]) ? na_s[k] : gamma_is_beta ? betas[k] : lbls[k];
            if (getenv("SCRIP_NAF_DIAG") && gamma_is_phi && na_f[k])
                fprintf(stderr, "[NAF-DIAG] GAMMA i=%d(%s) -> na_f[k=%d](%s) label=%s\n", i, bb_op_name(g_emitter.codegen_flat_chain_body_nodes[i]->op), k,
                    bb_op_name(g_emitter.codegen_flat_chain_body_nodes[k]->op), na_f[k]->name);
            if (gamma_is_sig && fc_sig[k]) {
                int _N = (int)(g_emitter.codegen_flat_chain_body_nodes[k]->n_operands / 2), _arm = 0, _src = -1, _best = -1;
                { int _p = nidx(g_emitter.codegen_flat_chain_body_nodes, n, g_emitter.codegen_flat_chain_body_nodes[i]); if (_p >= 0) { _src = _p; } }
                for (int _j = 0; _j < _N; _j++) { IR_t *_e = g_emitter.codegen_flat_chain_body_nodes[k]->operands[2 * _j]; { int _p = nidx(g_emitter.codegen_flat_chain_body_nodes, n, _e);
                    if (_p >= 0) { if (_p <= _src && _p > _best) { _best = _p; _arm = _j; } } } }
                node_γ = fc_sig[k][_arm];
            }
            if (gamma_is_sig) { node_γ = node_γ; }
            if (gamma_is_phi) { node_γ = node_γ; }
        } }
        { int _flk = emit_floater_kind(gtgt); if (_flk) node_γ = emit_floater_label(_flk); }
        { if (gtgt && ir_norm_call_kind(gtgt->op) == IR_CALL && emit_floater_member(gtgt) && node_γ == &lbl_γ) node_γ = emit_floater_label(1); }
        if (gtgt == NULL || gtgt->op == IR_SUCCEED) node_γ = &lbl_γ;
        if (gtgt && gtgt->op == IR_FAIL) node_γ = &lbl_ω;
        int omega_resolved = 0;
        { int k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, otgt); if (k >= 0) { node_ω = (omega_is_phi && na_fo[k]) ? na_fo[k] : omega_is_beta ? betas[k] : lbls[k]; omega_resolved = 1;
            if (getenv("SCRIP_NAF_DIAG") && omega_is_phi && na_fo[k])
            fprintf(stderr, "[NAF-DIAG] OMEGA i=%d(%s) -> na_fo[k=%d](%s) label=%s\n", i, bb_op_name(g_emitter.codegen_flat_chain_body_nodes[i]->op), k,
                bb_op_name(g_emitter.codegen_flat_chain_body_nodes[k]->op), na_fo[k]->name); } }
        { int _flk = emit_floater_kind(otgt); if (_flk) { node_ω = emit_floater_label(_flk); omega_resolved = 1; } }
        if (!omega_resolved) node_ω = (otgt && otgt->op == IR_SUCCEED) ? &lbl_γ : &lbl_ω;
        if (zd_chn[2 * i] >= 0) node_γ = betas[zd_chn[2 * i]];
        if (zd_chn[2 * i + 1] >= 0) node_ω = betas[zd_chn[2 * i + 1]];
        g_emit.op_omega_is_death = (!omega_resolved && !(otgt && otgt->op == IR_SUCCEED)) ? 1 : 0;
        if (g_emit_cfg && n_alt > 0) {
              if (otgt && otgt == g_emit_cfg->alt_fail && alt_step_lbl) node_ω = alt_step_lbl;
              if (gtgt && gtgt == g_emit_cfg->alt_fail && alt_step_lbl) node_γ = alt_step_lbl;
              if (!g_emit_cfg->root_graph) for (int _ak = 0; _ak < n_alt; _ak++) if (gtgt && gtgt == g_emit_cfg->alt_ret[_ak] && g_emit_cfg->alt_redo[_ak]) { node_γ =
                  g_emitter.codegen_flat_chain_body_ret_tr[_ak]; break; } }
        if (getenv("SCRIP_OMEGA_DIAG"))
            fprintf(stderr, "[OMEGA-DIAG] i=%d node_op=%s omega_resolved=%d otgt_op=%s op_omega_is_death=%d\n", i, bb_op_name(g_emitter.codegen_flat_chain_body_nodes[i]->op), omega_resolved,
                otgt ? bb_op_name(otgt->op) : "NULL",
            g_emit.op_omega_is_death);
        { int r = repalt_of[i]; if (r >= 0) { node_γ = ra_y[r]; if (g_emitter.codegen_flat_chain_body_nodes[r]->n_operands < 3) node_ω = ra_t[r]; } }
        if (g_emitter.codegen_flat_chain_body_nodes[i]->ω.node && g_emitter.codegen_flat_chain_body_nodes[i]->ω.node->op == IR_FAIL) {
            auto _rf = repalt_by_fail.find(g_emitter.codegen_flat_chain_body_nodes[i]->ω.node);
            if (_rf != repalt_by_fail.end()) node_ω = ra_t[_rf->second];
        }
        if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_CONJUNCTION && g_emitter.codegen_flat_chain_body_nodes[i]->n_operands > 0 && g_emitter.codegen_flat_chain_body_nodes[i]->operands[0]) {
            IR_t * op0 = g_emitter.codegen_flat_chain_body_nodes[i]->operands[0];
            { int _s0 = nd_slot(op0); if (_s0 >= 0 && bb_slot_get(g_emitter.codegen_flat_chain_body_nodes[i]) < 0) bb_slot_register(g_emitter.codegen_flat_chain_body_nodes[i], _s0); }
        }
        g_emitter.g_limit_gen_beta = NULL;
        g_emitter.g_scan_body_beta = NULL;
        g_emitter.g_scan_t0_pop = 0;
        g_create_body_entry = NULL;
        g_emitter.g_suspend_dobody_beta = NULL;
        if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_SUSPEND && g_emitter.codegen_flat_chain_body_nodes[i]->n_operands > 1 && g_emitter.codegen_flat_chain_body_nodes[i]->operands[1]) {
            IR_t *dobody = g_emitter.codegen_flat_chain_body_nodes[i]->operands[1];
            int via_beta = 0;
            for (int _g = 0; dobody->op == IR_GOTO && dobody->γ.node && _g < 128; _g++) { via_beta |= port_sz_beta(dobody->γ.sz); dobody = dobody->γ.node; }
            if (dobody->op == IR_FAIL) { g_emitter.g_suspend_dobody_beta = &lbl_ω; }
            else { { int k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, dobody);
                if (k >= 0) { int do_alpha = g_emitter.codegen_flat_chain_body_nodes[i]->seal == 1 && !(dobody->op == IR_SCAN && dobody->n_operands > 2 && !dobody->operands[1]);
                g_emitter.g_suspend_dobody_beta = (via_beta || (!do_alpha && (ir_is_generator_kind(ir_kind0(dobody)) || ir_kind0(dobody) == IR_CALL || ir_kind0(dobody) == IR_CALL_PROC_STAGED))) ?
                    betas[k] :
                    lbls[k]; } } }
        }
        if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_DEFER) { g_emit.lbl_t1_p = NULL; g_emit.lbl_t1 = NULL;
            for (int _aj = 0; _aj < g_emitter.codegen_flat_chain_body_nodes[i]->n_operands; _aj++) { IR_t *_so = g_emitter.codegen_flat_chain_body_nodes[i]->operands[_aj];
                if (!_so || !(_so->op == IR_GOTO || _so->op == IR_MATCH_ARBNO || _so->op == IR_MATCH_ABORT)) continue;
                int _phi = 0, _gg = 0; while (_so && _so->op == IR_GOTO && _gg++ < 128) { if (!_phi) _phi = (_so->γ.sz[0] == (char)0xcf && (unsigned char)_so->γ.sz[1] == 0x86); _so = _so->γ.node; }
                { int k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, _so); if (k >= 0) { bb_label_t *_sl = (_phi && na_fo[k]) ? na_fo[k] : betas[k]; g_emit.lbl_t1_p = _sl;
                    if (_spf && _sl && _so->op == IR_MATCH_ABORT) {
                        int _da = zd_arrival_depth(g_emitter.codegen_flat_chain_body_nodes, n, k, zd_on, zd_out, zd_gp, zd_wp), _dj = zd_on[i] ? zd_out[i] : -1;
                        if (_da == -1) _da = 0;
                        if (_dj < 0 || _da < 0 || _dj < _da) g_emit.lbl_t1_p = (bb_label_t *)0;
                        else if (_dj > _da) { _tpl[_tpn] = emit_label_alloc("%s_trap%d", prefix, _tpn); _tpd[_tpn] = _dj - _da; _tpt[_tpn] = _sl; g_emit.lbl_t1_p = _tpl[_tpn++]; }
                    }
                    g_emit.lbl_t1 = g_emit.lbl_t1_p ? g_emit.lbl_t1_p->name : NULL; } } break; } }
        if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_CREATE && g_emitter.codegen_flat_chain_body_nodes[i]->n_operands > 0 && g_emitter.codegen_flat_chain_body_nodes[i]->operands[0]) {
            IR_t *body_entry = g_emitter.codegen_flat_chain_body_nodes[i]->operands[0];
            g_emit.op_sval_lbl = NULL;
            g_create_body_entry = NULL;
            { int k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, body_entry); if (k >= 0) { g_emit.op_sval_lbl = lbls[k]->name; g_create_body_entry = lbls[k]; } }
        }
        if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_GATE_ARM && g_emitter.codegen_flat_chain_body_nodes[i]->n_operands > 1 && g_emitter.codegen_flat_chain_body_nodes[i]->operands[1] &&
            !g_emitter.codegen_flat_chain_body_nodes[i]->γ.node) {
            IR_t *igp = g_emitter.codegen_flat_chain_body_nodes[i]->operands[1];
            IR_t *sg = igp->γ.node;
            int sg_is_beta = port_sz_beta(igp->γ.sz);
            int sg_is_sig = (igp->γ.sz[0] == (char)0xcf && (unsigned char)igp->γ.sz[1] == 0x83);
            int sg_is_phi = (igp->γ.sz[0] == (char)0xcf && (unsigned char)igp->γ.sz[1] == 0x86);
            if (sg == NULL || sg->op == IR_SUCCEED) node_γ = &lbl_γ;
            else if (sg->op == IR_FAIL) node_γ = &lbl_ω;
            else { int k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, sg);
                if (k >= 0) { node_γ = (sg_is_phi && na_f[k]) ? na_f[k] : (sg_is_sig && na_s[k]) ? na_s[k] : sg_is_beta ? betas[k] : lbls[k]; } }
            if (g_emit_cfg && n_alt > 0 && !g_emit_cfg->root_graph)
                for (int _ak = 0; _ak < n_alt; _ak++) if (sg && sg == g_emit_cfg->alt_ret[_ak] && g_emit_cfg->alt_redo[_ak]) { node_γ = g_emitter.codegen_flat_chain_body_ret_tr[_ak]; break; }
        }
        if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_REPALT) {
            { if (g_emitter.codegen_flat_chain_body__dd < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); g_emitter.codegen_flat_chain_body__dd = (_e && _e[0] == '1') ? 1 : 0; }
              long _p0 = g_emitter.codegen_flat_chain_body__dd ? emit_text_count() : 0;
              flat_drive_repalt(g_emitter.codegen_flat_chain_body_nodes, n, i, lbls, betas, ra_y, ra_t, node_γ, node_ω);
              if (g_emitter.codegen_flat_chain_body__dd && emit_text_count() == _p0)
                  fprintf(stderr, "[DRIVE-DIAG] ZERO-EMIT chain=%d i=%d op=%s n_operands=%d (repalt path)\n", id, i, bb_op_name(g_emitter.codegen_flat_chain_body_nodes[i]->op),
                  g_emitter.codegen_flat_chain_body_nodes[i]->n_operands); }
            continue;
        }
        if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_GATE && g_emitter.codegen_flat_chain_body_nodes[i]->n_operands > 0) {
            if (zd_xb && (zd_gp[i] != 0 || zd_wp[i] != 0)) { g_emitter.g_zd_stage = 1; g_emitter.g_zd_arm = 0; g_emitter.g_zd_gpop = zd_gp[i]; g_emitter.g_zd_wpop = zd_wp[i];
                g_emitter.g_zd_wsteal = 0; g_emitter.g_zd_cpread = 0; g_emitter.g_zd_rdread = 0; }
            flat_drive_gate(g_emitter.codegen_flat_chain_body_nodes, n, i, lbls, betas, node_γ, node_ω, zd_on, zd_out, zd_xb);
            continue;
        }
        if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_CLEANUP) {
            flat_drive_cleanup(g_emitter.codegen_flat_chain_body_nodes, n, i, lbls, betas, node_γ, node_ω, zd_cl_read(g_emitter.codegen_flat_chain_body_nodes, n, i, zd_on, zd_out, zd_xb));
            continue;
        }
        if ((g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_ALTERNATE || g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_ARBNO ||
            g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_FENCE1 ||
            g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_SCAN_ALTERNATE ||
            g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_DISJUNCTION) && g_emitter.codegen_flat_chain_body_nodes[i]->n_operands > 0) {
            { if (g_emitter.codegen_flat_chain_body__dd_0 < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); g_emitter.codegen_flat_chain_body__dd_0 = (_e && _e[0] == '1') ? 1 : 0; }
              long _p0 = g_emitter.codegen_flat_chain_body__dd_0 ? emit_text_count() : 0;
              if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_DISJUNCTION && zd_on[i] && zd_dj_admitted(n, i) && zls_dj_retains(g_emitter.codegen_flat_chain_body_nodes[i])) {
                  g_emitter.g_zd_stage = 1;
                  g_emitter.g_zd_arm = 0;
                  g_emitter.g_zd_gpop = zd_gp[i];
                  g_emitter.g_zd_wpop = zd_wp[i];
                  g_emitter.g_zd_wsteal = 0;
                  g_emitter.g_zd_cpread = (zd_cpd && zd_cpd[i] <= -2) ? 1 : 0; g_emitter.g_zd_rdread = 0;
              }
              else if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_DISJUNCTION && zd_on[i]) {
                  g_emitter.g_zd_stage = 1;
                  g_emitter.g_zd_arm = 1;
                  g_emitter.g_zd_djok = zd_dj_admitted(n, i);
                  g_emitter.g_zd_gpop = zd_gp[i];
                  g_emitter.g_zd_wpop = zd_wp[i];
                  g_emitter.g_zd_wsteal = 0;
                  g_emitter.g_zd_k = zd_k(g_emitter.codegen_flat_chain_body_nodes[i]);
                  g_emitter.g_zd_abs = zd_out[i] + ((zd_xb && zd_xb[i] != INT_MIN) ? zd_xb[i] : 0); g_emitter.g_zd_cur = i; g_emitter.g_zd_def = (zd_xb && zd_xb[i] == INT_MIN) ? 0 : 1;
                      g_emitter.g_zd_cutd = zd_cut ? zd_cut[i] : -1; g_emitter.g_zd_cpread = 1; g_emitter.g_zd_rdread = 0;
              }
              { g_emitter.g_zd_on_cur = zd_on; g_emitter.g_zd_n_cur = n;
                int _awp = g_emit.op_wpop;
                if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_ALTERNATE || g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_FENCE1)
                    g_emit.op_wpop = (zd_on[i] && zd_wp[i] > 0) ? zd_wp[i] : 0;
                flat_drive_match_alt(g_emitter.codegen_flat_chain_body_nodes, n, i, lbls, betas, na_s, na_f, na_fo, fc_sig, node_γ, node_ω, &lbl_ω); g_emitter.g_zd_on_cur = NULL;
                g_emit.op_wpop = _awp; }
              if (g_emitter.codegen_flat_chain_body__dd_0 && emit_text_count() == _p0)
                  fprintf(stderr, "[DRIVE-DIAG] ZERO-EMIT chain=%d i=%d op=%s n_operands=%d (match_alt path)\n", id, i, bb_op_name(g_emitter.codegen_flat_chain_body_nodes[i]->op),
                  g_emitter.codegen_flat_chain_body_nodes[i]->n_operands)
                  ; }
            continue;
        }
        if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_LIMIT) {
            IR_t *gen = g_emitter.codegen_flat_chain_body_nodes[i]->n_operands > 0 ? g_emitter.codegen_flat_chain_body_nodes[i]->operands[0] : NULL;
            IR_t *rs = g_emitter.codegen_flat_chain_body_nodes[i]->n_operands > 2 ? g_emitter.codegen_flat_chain_body_nodes[i]->operands[2] : NULL;
            g_emitter.g_limit_gen_beta = NULL;
            g_emitter.g_limit_t0_pop = 0;
            int tk = -1;
            if (gen && ir_is_generator_kind(gen->op)) { { int k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, gen); if (k >= 0) { g_emitter.g_limit_gen_beta = betas[k]; tk = k; } } }
            else if (rs) { int k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, rs); if (k >= 0) { g_emitter.g_limit_gen_beta = betas[k]; tk = k; } }
            if (tk >= 0 && zd_xb && zd_on[i] && zd_on[tk] && zd_xb[i] != INT_MIN && zd_xb[tk] != INT_MIN) g_emitter.g_limit_t0_pop = (zd_xb[i] + zd_out[i]) - (zd_xb[tk] + zd_out[tk]);
            else if (tk >= 0 && zd_on[i] && zd_on[tk] && zd_claim[i] >= 0 && zd_claim[i] == zd_claim[tk]) g_emitter.g_limit_t0_pop = zd_out[i] - zd_out[tk];
            g_emit.lbl_t1_p = (bb_label_t *)0; g_emit.lbl_t1 = (const char *)0;
            for (int _gk = 2; zls_g_home_frame(g_emit_cfg) && _gk < g_emitter.codegen_flat_chain_body_nodes[i]->n_operands; _gk++) {
                IR_t * _gt = g_emitter.codegen_flat_chain_body_nodes[i]->operands[_gk];
                int _k = (_gt && _gt->op == IR_LIMIT_GATE && zls_home_why(_gt)) ? nidx(g_emitter.codegen_flat_chain_body_nodes, n, _gt) : -1;
                if (_k >= 0 && betas[_k]) { g_emit.lbl_t1_p = betas[_k]; g_emit.lbl_t1 = betas[_k]->name; }
            }
        }
        if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_SCAN) {
            IR_t *bv = g_emitter.codegen_flat_chain_body_nodes[i]->n_operands > 1 ? g_emitter.codegen_flat_chain_body_nodes[i]->operands[1] : NULL;
            g_emitter.g_scan_body_beta = NULL;
            if (bv && (ir_is_generator_kind(bv->op) || bv->op == IR_SCAN_TAB || bv->op == IR_SCAN_MOVE)) { int k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, bv);
                if (k >= 0) { g_emitter.g_scan_body_beta = betas[k]; } }
            if (!g_emitter.g_scan_body_beta && g_emitter.codegen_flat_chain_body_nodes[i]->n_operands > 2) { IR_t *bb2 = g_emitter.codegen_flat_chain_body_nodes[i]->operands[2]; int _fk = -1;
                { int _gb = 0, _gg = 0; IR_t * _g2 = bb2; while (_g2 && _g2->op == IR_GOTO && _gg++ < 128) { _gb |= port_sz_beta(_g2->γ.sz); _g2 = _g2->γ.node; }
                  int _gk = (bb2 && bb2->op == IR_GOTO && _gb && _g2) ? nidx(g_emitter.codegen_flat_chain_body_nodes, n, _g2) : -1;
                  if (_gk >= 0 && betas[_gk]) { g_emitter.g_scan_body_beta = betas[_gk]; _fk = _gk; } }
                for (int _hops = 0; bb2 && _fk < 0 && _hops < 8; _hops++) {
                    { int _conduit = (IR_LIT(g_emitter.codegen_flat_chain_body_nodes[i]).dval == 3.0 || IR_LIT(g_emitter.codegen_flat_chain_body_nodes[i]).dval == 4.0) ? 1 : 0;
                    { int k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, bb2); if (k >= 0) { _fk = k;
                        g_emitter.g_scan_body_beta =
                        (betas[k] && !(_hops == 0 && !bv && scan_conduit_enters_a_do_clause(g_emitter.codegen_flat_chain_body_nodes, n, g_emitter.codegen_flat_chain_body_nodes[i])) &&
                            (scan_body_beta_keeps(g_emitter.codegen_flat_chain_body_nodes[k]) || (!_conduit && bv && bused[k]))) ? betas[k] :
                        lbls[k]; } } }
                    if (_fk < 0) bb2 = bb2->γ.node;
                }
                if (getenv("SCRIP_SCAN3_DIAG"))
                    fprintf(stderr, "[SCAN3] i=%d found_k=%d dval=%g nops=%d bv=%d -> t0=%s (alpha=%s beta=%s keeps=%d used=%d)\n", i, _fk, IR_LIT(g_emitter.codegen_flat_chain_body_nodes[i]).dval,
                        g_emitter.codegen_flat_chain_body_nodes[i]->n_operands, bv ? 1 : 0,
                    g_emitter.g_scan_body_beta ? g_emitter.g_scan_body_beta->name : "-", (_fk >= 0 && lbls[_fk]) ? lbls[_fk]->name : "-", (_fk >= 0 && betas[_fk]) ? betas[_fk]->name : "-",
                    _fk >= 0 ? scan_body_beta_keeps(g_emitter.codegen_flat_chain_body_nodes[_fk]) : -1, _fk >= 0 ? (int)bused[_fk] : -1); }
            { int tk = -1, ta = 0;
              for (int k = 0; k < n && g_emitter.g_scan_body_beta; k++) if (betas[k] == g_emitter.g_scan_body_beta) { tk = k; break; }
              for (int k = 0; tk < 0 && k < n && g_emitter.g_scan_body_beta; k++) if (lbls[k] == g_emitter.g_scan_body_beta) { tk = k; ta = zd_k(g_emitter.codegen_flat_chain_body_nodes[k]); }
              if (tk >= 0 && zd_xb && zd_on[i] && zd_on[tk] && zd_xb[i] != INT_MIN && zd_xb[tk] != INT_MIN) g_emitter.g_scan_t0_pop = (zd_xb[i] + zd_out[i]) - (zd_xb[tk] + zd_out[tk] - ta);
              else if (tk >= 0 && zd_on[i] && zd_on[tk] && zd_claim[i] >= 0 && zd_claim[i] == zd_claim[tk]) g_emitter.g_scan_t0_pop = zd_out[i] - (zd_out[tk] - ta); }
        }
        { if (g_emitter.codegen_flat_chain_body__zpf < 0) { const char *_e = getenv("SCRIP_ZPOP_FOLD_OFF"); g_emitter.codegen_flat_chain_body__zpf = (_e && _e[0] == '1') ? 1 : 0; }
          if (!g_emitter.codegen_flat_chain_body__zpf) {
              long _sum = 0; int _fk = -1, _hops = 0;
              { auto _bit = beta_of.find(node_ω); if (_bit != beta_of.end()) _fk = _bit->second; }
              if (_fk >= 0 && beta_is_stmt_land(g_emitter.codegen_flat_chain_body_nodes[_fk])) { _fk = -1; }
              while (_fk >= 0 && _hops++ < 64 && flat_trivial_beta(g_emitter.codegen_flat_chain_body_nodes[_fk])) {
                  { long _fck = 0; if (fc_geom(g_emitter.codegen_flat_chain_body_nodes[_fk], &_fck)) _sum += _fck; if (zd_wp[_fk] > 0) _sum += zd_wp[_fk]; }
                  IR_t *_c = g_emitter.codegen_flat_chain_body_nodes[_fk]; IR_t *_o = _c->ω.node;
                  int _ib = port_sz_beta(_c->ω.sz);
                  int _ip = (_c->ω.sz[0] == (char)0xcf && (unsigned char)_c->ω.sz[1] == 0x86);
                  { int _gg = 0; IR_t *_g = _o; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!_ib) _ib = port_sz_beta(_g->γ.sz);
                      if (!_ip) _ip = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x86); _o = _g->γ.node; _g = _o; } }
                  bb_label_t *_next = NULL; int _k2 = -1;
                  { int _k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, _o); if (_k >= 0) { _k2 = _k; } }
                  if (_k2 >= 0) { _next = (_ip && na_fo[_k2]) ? na_fo[_k2] : _ib ? betas[_k2] : lbls[_k2]; }
                  else _next = (_o && _o->op == IR_SUCCEED) ? &lbl_γ : &lbl_ω;
                  { auto _ro = repalt_by_op0.find(_c); if (_ro != repalt_by_op0.end()) _next = ra_t[_ro->second]; }
                  node_ω = _next;
                  _fk = -1; { auto _bit = beta_of.find(_next); if (_bit != beta_of.end()) _fk = _bit->second; }
              }
              g_emit.op_wpop = (int)_sum;
          } }
        { if (g_emitter.codegen_flat_chain_body__dd_1 < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); g_emitter.codegen_flat_chain_body__dd_1 = (_e && _e[0] == '1') ? 1 : 0; }
          long _p0 = g_emitter.codegen_flat_chain_body__dd_1 ? emit_text_count() : 0;
          g_emit.op_scan = 0; g_emit.op_scan_head_off = -1;
          if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_DEFER) {
              for (; _hk_c < 0 && _hk_upto < n; _hk_upto++) if (g_emitter.codegen_flat_chain_body_nodes[_hk_upto]->op == IR_MATCH_BEGIN) _hk_c = _hk_upto;
              int _hk = _hk_c;
              if (_hk >= 0) { IR_t *_gt = g_emitter.codegen_flat_chain_body_nodes[_hk]->γ.node; { int _gg = 0; while (_gt && _gt->op == IR_GOTO && _gg++ < 128) _gt = _gt->γ.node; }
                  if (_gt == g_emitter.codegen_flat_chain_body_nodes[i] && node_ω == betas[_hk]) { g_emit.op_scan = 1;
                      g_emit.op_scan_head_off = (int)drive_value_slot(g_emitter.codegen_flat_chain_body_nodes[_hk]); } }
          }
          int _uw_stolen = 0; long _uw_pop = 0;
          { if (g_emitter.codegen_flat_chain_body__uw < 0) { const char *_e = getenv("SCRIP_UNWIND"); g_emitter.codegen_flat_chain_body__uw = (_e && *_e == '0') ? 0 : 1; }
            int _h3 = i;
            if (g_emitter.codegen_flat_chain_body__uw && g_emitter.codegen_flat_chain_body_nodes[i]->op != IR_STATEMENT && g_emitter.codegen_flat_chain_body_nodes[i]->op != IR_STATEMENT_BEGIN &&
                g_emitter.codegen_flat_chain_body_nodes[i]->op != IR_STATEMENT_END && zd_on[i] && zd_wp[i] > 0 && g_emit.op_wpop == 0 && !omega_is_retry &&
                !omega_is_phi) {
                int _bg = 0; for (int _j = i - 1; _j >= 0; _j--) { int _o = (int)g_emitter.codegen_flat_chain_body_nodes[_j]->op; if (_o == IR_STATEMENT_BEGIN) { _bg = 1; break;
                    } if (_o == IR_STATEMENT_END || _o == IR_STATEMENT) break; }
                IR_t *_ti = zd_chase(g_emitter.codegen_flat_chain_body_nodes[i]->ω.node);
                if (_bg && _ti) for (int _j = i - 1; _j >= 0; _j--) {
                    int _o = (int)g_emitter.codegen_flat_chain_body_nodes[_j]->op; if (_o == IR_STATEMENT_BEGIN || _o == IR_STATEMENT_END || _o == IR_STATEMENT) break;
                    if (zd_on[_j] && zd_wp[_j] > 0 && flat_unwind_beta(g_emitter.codegen_flat_chain_body_nodes[_j]) && zd_chase(g_emitter.codegen_flat_chain_body_nodes[_j]->ω.node) == _ti) {
                        long _d = (zd_wp[i] + zd_k(g_emitter.codegen_flat_chain_body_nodes[i])) - (zd_wp[_j] + zd_k(g_emitter.codegen_flat_chain_body_nodes[_j]));
                        if (_d >= 0) { node_ω = betas[_j]; _uw_stolen = 1; _uw_pop = _d; } break; } } } }
          int _endj_stolen = 0;
          {
            int _h2 = i;
            if (!_uw_stolen && g_emitter.codegen_flat_chain_body_nodes[i]->op != IR_STATEMENT &&
                g_emitter.codegen_flat_chain_body_nodes[i]->op != IR_STATEMENT_BEGIN && g_emitter.codegen_flat_chain_body_nodes[i]->op != IR_STATEMENT_END && zd_on[i] && zd_wp[i] > 0 &&
                g_emit.op_wpop == 0 &&
                !omega_is_retry && !omega_is_phi) {
                int _e2 = -1; for (int _j = i + 1; _j < n; _j++) { if (g_emitter.codegen_flat_chain_body_nodes[_j]->op == IR_STATEMENT_BEGIN) break;
                    if (g_emitter.codegen_flat_chain_body_nodes[_j]->op == IR_STATEMENT_END || g_emitter.codegen_flat_chain_body_nodes[_j]->op == IR_STATEMENT) { _e2 = _j;
                    break; } }
                if (_e2 >= 0 && zd_gp[_e2] == zd_wp[i] + zd_k(g_emitter.codegen_flat_chain_body_nodes[i])) {
                    IR_t *_tf = zd_chase(g_emitter.codegen_flat_chain_body_nodes[i]->ω.node); IR_t *_te = zd_chase(g_emitter.codegen_flat_chain_body_nodes[_e2]->γ.node);
                    int _kf = -1; if (_tf) { int _k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, _tf); if (_k >= 0) { _kf = _k; } }
                    if (_tf && _tf == _te && _kf >= 0 && node_ω == lbls[_kf]) { node_ω = lbls[_e2]; _endj_stolen = 1; g_emit.op_wpop = 0; } } } }
          bb_label_t *_zw5_saved_omega = NULL; int _zw5_wpop_stolen = 0;
          { int _h = i;
          if (g_emitter.codegen_flat_chain_body__zw5_on && !_endj_stolen && !_uw_stolen && g_emitter.codegen_flat_chain_body_nodes[i]->op != IR_STATEMENT &&
              g_emitter.codegen_flat_chain_body_nodes[i]->op != IR_STATEMENT_END && zd_on[i] && zd_wp[i] > 0 && !omega_is_retry && !omega_is_phi) { int _lo =
              0, _hi = zw5_pool_stmts; while (_lo < _hi) { int _m = (_lo + _hi) / 2; if (zw5_stmt[_m].idx > i) _hi = _m; else _lo = _m + 1; } if (_lo < zw5_pool_stmts) { int _sb = zw5_stmt[_lo].base;
              for (int _d = 0; _d < zw5_stmt[_lo].cnt; _d++) { if (zw5_depth[_sb + _d] == zd_wp[i]) { _zw5_saved_omega = node_ω; node_ω = &zw5_pool[_sb + _d]; _zw5_wpop_stolen = zd_wp[i]; break;
              } } } } }
          g_emit.op_trap_drop = (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_SETEXIT_TEST && zd_on[i]) ? (long)(zd_out[i] - zd_k(g_emitter.codegen_flat_chain_body_nodes[i])) : 0;
          g_emit.op_zrun = (zd_out[i] >= 0 ? (int)(zd_out[i] - (zd_on[i] ? zd_k(g_emitter.codegen_flat_chain_body_nodes[i]) : 0)) : 0) + _sigk + _sph;
          if (zd_out[i] >= 0 && zd_on[i] && zd_xb && zd_xb[i] != INT_MIN && zd_xb[i] > 0 && g_emit.flat_gen) g_emit.op_zrun += zd_xb[i];
          g_emit.op_zxb = (zd_out[i] >= 0 && zd_on[i] && zd_xb && zd_xb[i] != INT_MIN && zd_xb[i] > 0 && !g_emit.flat_gen) ? zd_xb[i] : 0;
          g_emit.op_vdj_live = zd_vl[i];
          g_emitter.g_zd_fence_exit =
              (zd_arm && zd_arm[i] >= 0 && g_emitter.codegen_flat_chain_body_nodes[zd_arm[i]]->op == IR_MATCH_FENCE1 &&
              zd_chase(g_emitter.codegen_flat_chain_body_nodes[i]->γ.node) == g_emitter.codegen_flat_chain_body_nodes[zd_arm[i]]) ? 1 : 0;
          if (zd_on[i] || zd_gp[i] > 0 || zd_wp[i] > 0) { g_emitter.g_zd_stage = 1; g_emitter.g_zd_arm = zd_on[i] ? 1 : 0; g_emitter.g_zd_gpop = zd_gp[i];
              g_emitter.g_zd_bgrow = zd_on[i] && zd_arm[i] >= 0 &&
                  (g_emitter.codegen_flat_chain_body_nodes[zd_arm[i]]->op == IR_MATCH_ALTERNATE || zls_dj_rbp_arms(g_emitter.codegen_flat_chain_body_nodes[zd_arm[i]])) &&
                  zd_chase(g_emitter.codegen_flat_chain_body_nodes[i]->γ.node) == g_emitter.codegen_flat_chain_body_nodes[zd_arm[i]];
              g_emitter.g_zd_wpop = (_uw_stolen ? (int)_uw_pop : ((_zw5_wpop_stolen || _endj_stolen) ? 0 : zd_wp[i])); g_emitter.g_zd_wsteal = _endj_stolen || _uw_stolen;
              g_emitter.g_zd_k = zd_on[i] ? zd_k(g_emitter.codegen_flat_chain_body_nodes[i]) : 0;
              if (zd_on[i] && zd_icn_suspend(g_emitter.codegen_flat_chain_body_nodes[i])) { IR_t * _sn = g_emitter.codegen_flat_chain_body_nodes[i];
                  int _ti = (_sn->n_operands > 1) ? nidx(g_emitter.codegen_flat_chain_body_nodes, n, zd_chase(_sn->operands[1])) : -1;
                  int _bi = (zd_xb && zd_xb[i] != INT_MIN) ? zd_xb[i] : 0, _bt = (_ti >= 0 && zd_xb && zd_xb[_ti] != INT_MIN) ? zd_xb[_ti] : 0;
                  int _ta = (_ti >= 0 && zd_on[_ti] && g_emitter.g_suspend_dobody_beta == lbls[_ti]) ? zd_k(g_emitter.codegen_flat_chain_body_nodes[_ti]) : 0;
                  int _ke = (g_emitter.g_zd_claim_p && g_emit_cfg && g_emit_cfg->home_frame && g_emit_cfg->spine_cells) ?
                      zd_yield_end(g_emitter.codegen_flat_chain_body_nodes, n, i, zd_on, g_emitter.g_zd_claim_p) : -1;
                  int _yd = _ke >= 0 ? zd_out[_ke] - zd_gp[_ke] : zd_out[i];
                  g_emitter.g_zd_suspend_uclaim = (_ti >= 0 && zd_on[_ti]) ? (_bi + _yd) - (_bt + zd_out[_ti] - _ta) : 0; }
              { g_emitter.g_zd_abs = zd_on[i] ? zd_out[i] + ((zd_xb && zd_xb[i] != INT_MIN) ? zd_xb[i] : 0) : 0; g_emitter.g_zd_cur = i;
                  g_emitter.g_zd_def = (zd_xb && zd_xb[i] == INT_MIN) ? 0 : 1; g_emitter.g_zd_cutd = zd_cut ? zd_cut[i] : -1; g_emitter.g_zd_cpread = 1; g_emitter.g_zd_rdread = 1; } }
          if (g_emit_cfg && n_alt > 0) {
              if (otgt && otgt == g_emit_cfg->alt_fail && alt_step_lbl) node_ω = alt_step_lbl;
              if (gtgt && gtgt == g_emit_cfg->alt_fail && alt_step_lbl) node_γ = alt_step_lbl;
              if (!g_emit_cfg->root_graph) for (int _ak = 0; _ak < n_alt; _ak++) if (gtgt && gtgt == g_emit_cfg->alt_ret[_ak] && g_emit_cfg->alt_redo[_ak]) { node_γ =
                  g_emitter.codegen_flat_chain_body_ret_tr[_ak]; break; } }
          if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_BEGIN) { g_emit.lbl_t0 = na_f[i] ? na_f[i]->name : NULL; g_emit.lbl_t0_p = na_f[i]; g_emit.lbl_t0o_p = na_fo[i]; }
          if (_spf && node_ω == &lbl_ω && zd_on[i]) { int _wd = zd_out[i] - zd_k(g_emitter.codegen_flat_chain_body_nodes[i]) - g_emit.op_wpop - (g_emitter.g_zd_stage ? g_emitter.g_zd_wpop : 0);
              if (_wd > 0) { int _k = 0;
              while (_k < _wpn && _wpd[_k] != _wd) _k++; if (_k == _wpn) { _wpd[_wpn] = _wd; _wpl[_wpn++] = emit_label_alloc("%s_\xcf\x89%d", prefix, _wd); } node_ω = _wpl[_k];
              } }
          if (_spf && node_ω == &lbl_ω && !zd_on[i]) { int _wd = zd_arrival_depth(g_emitter.codegen_flat_chain_body_nodes, n, i, zd_on, zd_out, zd_gp, zd_wp);
              if (_wd > 0) { int _k = 0; while (_k < _wpn && _wpd[_k] != _wd) _k++; if (_k == _wpn) { _wpd[_wpn] = _wd; _wpl[_wpn++] = emit_label_alloc("%s_\xcf\x89%d", prefix, _wd);
                  } node_ω = _wpl[_k]; } }
          g_emit.op_head_spine = _sph > 0; g_emit.op_head_rsp = (_sph > 0 && zd_on[i]) ? zd_out[i] + _sph - 8 : -1;
                  emit_drive(g_emitter.codegen_flat_chain_body_nodes[i], lbls[i], node_γ, node_ω, betas[i]);
              g_emit.op_head_spine = 0;
          if (_zw5_saved_omega) node_ω = _zw5_saved_omega;
          if (g_emitter.codegen_flat_chain_body__zw5_on && g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_STATEMENT) { int _lo = 0, _hi = zw5_pool_stmts;
              while (_lo < _hi) { int _m = (_lo + _hi) / 2; if (zw5_stmt[_m].idx >= i) _hi = _m; else _lo = _m + 1;
              } if (_lo < zw5_pool_stmts && zw5_stmt[_lo].idx == i) { int _sb = zw5_stmt[_lo].base; for (int _d = 0; _d < zw5_stmt[_lo].cnt; _d++) { emit_label_define_bb(&zw5_pool[_sb + _d]);
              bb_emit_x86(x86("add", "rsp", (long)zw5_depth[_sb + _d]) + x86("jmp", "extlbl", (uint64_t)(uintptr_t)node_ω)); } } }
          if (g_emitter.codegen_flat_chain_body__dd_1 && emit_text_count() == _p0)
              fprintf(stderr, "[DRIVE-DIAG] ZERO-EMIT chain=%d i=%d op=%s n_operands=%d\n", id, i, bb_op_name(g_emitter.codegen_flat_chain_body_nodes[i]->op),
              g_emitter.codegen_flat_chain_body_nodes[i]->n_operands); }
    }
    { if (g_emitter.codegen_flat_chain_body__sz_0 < 0) { const char *_e = getenv("SCRIP_ASM_SYMSIZE"); g_emitter.codegen_flat_chain_body__sz_0 = (_e && _e[0] == '0') ? 0 : 1; }
      if (g_emitter.codegen_flat_chain_body__sz_0 && g_is_text && _bx_open >= 0) emit_textf(".size %s, .-%s\n", bxs[_bx_open]->name, bxs[_bx_open]->name); }
    g_emit.x86_uid_kind = (const char *)0;
    g_emit.op_beta_dead = 0;
    g_emit.op_wpop = 0;
    g_emit.op_vdj_live = 0; g_emitter.g_zd_plan_on = (const unsigned char *)0; g_emitter.g_zd_plan_nodes = (IR_t **)0; g_emitter.g_zd_plan_n = 0;
    if (n == 0) emit_jmp_label(flat_empty_body_fail ? &lbl_ω : &lbl_γ, JMP_JMP);
    auto pl_alt_target = [&](int ak) -> bb_label_t * {
        IR_t * _e = g_emit_cfg->alt_entry[ak];
        { int _sg = 0; while (_e && _e->op == IR_SUCCEED && _e->γ.node && _sg++ < 4096) _e = _e->γ.node; }
        if (_e) { int _k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, _e); if (_k >= 0) return lbls[_k]; }
        if (_e && _e->op == IR_SUCCEED) return g_emit_cfg->alt_redo[ak] ? g_emitter.codegen_flat_chain_body_ret_tr[ak] : &lbl_γ;
        return &lbl_ω; };
    if (g_emit_cfg && n_alt > 0) {
        int _kt0 = g_emit.flat_frame_bytes; int _np0 = g_emit_cfg->nparams; int _nl0 = g_emit_cfg->nlocals;
        for (int _ak = 0; _ak < n_alt; _ak++) {
            IR_t * _rd = g_emit_cfg->alt_redo[_ak]; bb_label_t * _rb = (bb_label_t *)0;
            if (!_rd || g_emit_cfg->root_graph) continue;
            { int _k = nidx(g_emitter.codegen_flat_chain_body_nodes, n, _rd); if (_k >= 0) { _rb = betas[_k]; } }
            if (!_rb) continue;
            emit_sep_rule('-'); emit_label_define_bb(g_emitter.codegen_flat_chain_body_ret_tr[_ak]);
            bb_emit_x86( x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)_rb)
                      + x86("mov", RDQ(emit_pl_fb(), _kt0 - 48), "rcx")
                      + x86("mov32", "eax", (long)DT_I)
                      + x86("mov32", "edx", 1L));
            emit_jmp_label(&lbl_γ, JMP_JMP);
        }
        emit_sep_rule('-'); emit_label_define_bb(alt_step_lbl);
        if (emit_pl_scc_armed()) {
            bb_label_t * sccd_ok = emit_label_alloc("%s_sccd", fam);
            bb_emit_x86(x86("mov", "rax", "r12") + x86("and", "rax", X86_PL_TR_ARENA_MASK) + x86("mov", "rcx", RDQ("rax", 32)) + x86("test", "rcx", "rcx"));
            emit_jmp_label(sccd_ok, JMP_JE);
            bb_emit_x86(x86("lea", "rdx", RDQ(emit_pl_fb(), _kt0)) + x86("cmp", "rcx", "rdx"));
            emit_jmp_label(sccd_ok, JMP_JGE);
            bb_emit_x86(x86_bomb("setup_call_cleanup: a clause retry reuses a frame that still holds a linked cleanup record, or one younger"));
            emit_label_define_bb(sccd_ok);
        }
        bb_label_t * pl_step_ball_lbl = emit_label_alloc("%s_step_ball", fam);
        bb_emit_x86(x86("test", "r15", "r15"));
        emit_jmp_label(pl_step_ball_lbl, JMP_JNE);
        bb_label_t * pl_uw_top = emit_label_alloc("%s_uw", fam), * pl_uw_done = emit_label_alloc("%s_uwd", fam);
        bb_emit_x86(x86("mov", "rsi", RDQ(emit_pl_fb(), _kt0 - 64)));
        emit_label_define_bb(pl_uw_top);
        bb_emit_x86(x86("cmp", "rsi", "r12"));
        emit_jmp_label(pl_uw_done, JMP_JGE);
        bb_emit_x86(x86_pl_tr_pop_head());
        emit_jmp_label(pl_uw_top, JMP_JE);
        bb_emit_x86(x86_pl_tr_pop_restore());
        emit_jmp_label(pl_uw_top, JMP_JMP);
        emit_label_define_bb(pl_uw_done);
        bb_emit_x86( x86_pl_tr_top_sync()
                  + x86("mov", RDQ(emit_pl_fb(), _kt0 - 48), 0L)
                  + pl_step_reseed_locals()
                  + x86("mov", "rax", RDQ(emit_pl_fb(), _kt0 - 56))
                  + x86("test", "rax", "rax"));
        emit_jmp_label(&lbl_ω, JMP_JE);
        bb_emit_x86(x86_jmp_reg("rax"));
        emit_sep_rule('-'); emit_label_define_bb(pl_step_ball_lbl);
        bb_emit_x86(x86("comment",
            "PL EXCEPTION IN FLIGHT (r15 armed): a thrown ball received at a clause's own failure port must propagate past every remaining sibling clause, never retry one -- restore the outer choice"
            " barrier this predicate's own choice point had saved, then concede.")
                  + x86("mov", "r13", RDQ(emit_pl_fb(), _kt0 - 40)));
        emit_jmp_label(&lbl_ω, JMP_JMP);
        for (int _ak = 1; _ak < n_alt; _ak++) {
            int _kt = g_emit.flat_frame_bytes;
            emit_sep_rule('-'); emit_label_define_bb(g_emitter.codegen_flat_chain_body_alt_tr[_ak]);
            bb_emit_x86(x86("comment",
                "PL ALTERNATIVE TRAMPOLINE (rung 2, Jcon ir_MoveLabel): advance F.CUR to the NEXT alternative, or zero it on the last one so the step concedes, then enter this clause head unificatio"
                "n.")
                      + pl_clause_zero(_ak)
                      + (_ak + 1 < n_alt ? x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)g_emitter.codegen_flat_chain_body_alt_tr[_ak + 1]) : x86("xor", "eax", "eax"))
                      + x86("mov", RDQ(emit_pl_fb(), _kt - 56), "rax")
                      + IF(_ak + 1 >= n_alt,
                            x86("mov", "r13", RDQ(emit_pl_fb(), _kt - 40))));
            emit_jmp_label(pl_alt_target(_ak), JMP_JMP);
        }
        if (ix_switch) {
            int _kt = g_emit.flat_frame_bytes; bb_label_t * _chain = pl_alt_target(0);
            bb_label_t ** _det = (bb_label_t **)alloca(sizeof(bb_label_t *) * (size_t)n_alt); for (int _ak = 0; _ak < n_alt; _ak++) _det[_ak] = (bb_label_t *)0;
            bb_label_t * _fail = (bb_label_t *)0;
            bb_label_t ** _sent = (bb_label_t **)alloca(sizeof(bb_label_t *) * (size_t)(ix_nset + 1)); for (int _s = 0; _s < ix_nset; _s++) _sent[_s] = (bb_label_t *)0;
            auto _to = [&](int code) -> bb_label_t * {
                if (code == -2) return _chain;
                if (code <= -3) { int _s = -3 - code; if (!_sent[_s]) _sent[_s] = emit_label_alloc("%s_ixset%d", fam, _s); return _sent[_s]; }
                if (code == -1) { if (!_fail) _fail = emit_label_alloc("%s_ixfail", fam); return _fail; }
                if (!_det[code]) _det[code] = (code == n_alt - 1) ? g_emitter.codegen_flat_chain_body_alt_tr[code] : emit_label_alloc("%s_ixdet%d", fam, code); return _det[code]; };
            pl_ix_arm_t * _arms = (pl_ix_arm_t *)alloca(sizeof(pl_ix_arm_t) * (size_t)(ix_n > 0 ? ix_n : 1));
            for (int _q = 0; _q < ix_n; _q++) { _arms[_q].tag = ix_tag[_q]; _arms[_q].val = ix_val[_q]; _arms[_q].to = _to(ix_code[_q]); }
            bb_label_t * _fbr = _to(ix_fbr), * _fba = _to(ix_fba), * _fbi = _to(ix_fbi);
            emit_sep_rule('-'); emit_label_define_bb(ix_switch);
            { extern void xa_pl_switch(int, const pl_ix_arm_t *, int, bb_label_t *, bb_label_t *, bb_label_t *, bb_label_t *); xa_pl_switch(ix_slot, _arms, ix_n, _fbr, _fba, _fbi, _chain); }
            for (int _ak = 0; _ak < n_alt - 1; _ak++) {
                if (!_det[_ak]) continue;
                emit_sep_rule('-'); emit_label_define_bb(_det[_ak]);
                bb_emit_x86(x86("comment",
                    "PL SWITCH: this clause is the only one the first argument's key can match -- entered as the chain's last alternative is: no next alternative, B back to the outer choice")
                          + x86("mov", RDQ(emit_pl_fb(), _kt - 56), 0L)
                          + x86("mov", "r13", RDQ(emit_pl_fb(), _kt - 40)));
                emit_jmp_label(pl_alt_target(_ak), JMP_JMP);
            }
            if (_fail) {
                emit_sep_rule('-'); emit_label_define_bb(_fail);
                bb_emit_x86(x86("comment",
                    "PL SWITCH: no clause's first head argument can match the first argument's key -- concede through the step with no next alternative and B back to the outer choice")
                          + x86("mov", RDQ(emit_pl_fb(), _kt - 56), 0L)
                          + x86("mov", "r13", RDQ(emit_pl_fb(), _kt - 40)));
                emit_jmp_label(alt_step_lbl, JMP_JMP);
            }
            for (int _s = 0; _s < ix_nset; _s++) {
                if (!_sent[_s]) continue;
                const int _m = ix_set_len[_s]; const int * _it = ix_sets + ix_set_beg[_s];
                bb_label_t ** _tl = (bb_label_t **)alloca(sizeof(bb_label_t *) * (size_t)_m);
                for (int _j = 1; _j < _m; _j++) _tl[_j] = emit_label_alloc("%s_ixset%d_%d", fam, _s, _j);
                emit_sep_rule('-'); emit_label_define_bb(_sent[_s]);
                bb_emit_x86(x86("comment", "PL SWITCH V2 (ARCH-PROLOG-C-OUT-OF-THE-BOX 11.4): the first argument's key leaves two or more candidate clauses -- the choice stays open (B is this "
                                           "frame's header) and the step walks ONLY this set, in source order, through its own trampolines")
                          + x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)_tl[1])
                          + x86("mov", RDQ(emit_pl_fb(), _kt - 56), "rax"));
                emit_jmp_label(pl_alt_target(_it[0]), JMP_JMP);
                for (int _j = 1; _j < _m; _j++) {
                    emit_sep_rule('-'); emit_label_define_bb(_tl[_j]);
                    bb_emit_x86(pl_clause_zero(_it[_j])
                              + (_j + 1 < _m ? x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)_tl[_j + 1]) : x86("xor", "eax", "eax"))
                              + x86("mov", RDQ(emit_pl_fb(), _kt - 56), "rax")
                              + IF(_j + 1 >= _m, x86("mov", "r13", RDQ(emit_pl_fb(), _kt - 40))));
                    emit_jmp_label(pl_alt_target(_it[_j]), JMP_JMP);
                }
            }
        }
    }
    if (g_emit.flat_jmp_entry) {
        if (!bare) {
        if (g_emit.flat_pat) {
            emit_sep_rule('-'); emit_label_define_bb(&lbl_res);
            bb_emit_x86(IF(blob_frame_bytes() > 0, x86_rsp_load64("rbp", 24)) + x86("add", "rsp", 32L));
        } else if (icn_gen_regime() && g_emit.flat_gen) {
            emit_sep_rule('-'); emit_label_define_bb(&lbl_res);
            {
              bb_emit_x86( x86("mov", "rbp", "rax")
                         +
                             IF(x86_trace_hooks_on(),
                             x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime") + x86("mov", "rcx", RDQ("rcx", GRT_AT(line))) +
                                 x86("shl", "rcx", 1L) +
                             x86("mov", "rdx", RDQ("rax", 64)) + x86("and", "rdx", 1L) + x86("or", "rcx", "rdx") + x86("mov", RDQ("rax", 64), "rcx"))
                         + icn_gen_line_resume() + icn_entry_gva()); }
        }
        }
    }
    if (!bare && !_top_hoist) {
    emit_sep_rule('-'); emit_label_define_bb(&lbl_β);
    if (g_emit_cfg && g_emit_cfg->n_alts > 0) {
        int _kt = g_emit.flat_frame_bytes;
        bb_label_t * _step = alt_step_lbl;
        bb_label_t * _resume = emit_label_alloc("%s_βres", fam);
        if (emit_pl_scc_armed()) {
            bb_label_t * _go = emit_label_alloc("%s_βgo", fam);
            bb_emit_x86(x86("test", "r15", "r15"));
            emit_jmp_label(_go, JMP_JE);
            bb_emit_x86(x86("mov", "rax", "r12") + x86("and", "rax", X86_PL_TR_ARENA_MASK) + x86("mov", "rax", RDQ("rax", 32)) + x86("test", "rax", "rax"));
            emit_jmp_label(&lbl_ω, JMP_JE);
            bb_emit_x86(x86("lea", "rdx", RDQ(emit_pl_fb(), _kt)) + x86("cmp", "rax", "rdx"));
            emit_jmp_label(&lbl_ω, JMP_JGE);
            emit_label_define_bb(_go);
        } else {
            bb_emit_x86( x86("test", "r15", "r15"));
            emit_jmp_label(&lbl_ω, JMP_JNE);
        }
        bb_emit_x86(x86("mov", "rax", RDQ(emit_pl_fb(), _kt - 48)) + x86("mov", RDQ(emit_pl_fb(), _kt - 48), 0L) + x86("test", "rax", "rax"));
        emit_jmp_label(_resume, JMP_JNE);
        emit_jmp_label(_step ? _step : &lbl_ω, JMP_JMP);
        emit_label_define_bb(_resume);
        bb_emit_x86(x86_jmp_reg("rax"));
    } else {
        bb_label_t *resume_tgt = &lbl_ω;
        for (int i = 0; i < n; i++) if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_SUSPEND) { resume_tgt = betas[i]; break; }
        if (g_emit_cfg && g_emit_cfg->body_root)
            { int i = nidx(g_emitter.codegen_flat_chain_body_nodes, n, g_emit_cfg->body_root); if (i >= 0 && (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_DISJUNCTION)) { resume_tgt = lbls[i];
                } }
        if (g_emit_cfg && g_emit_cfg->body_root)
            { int i = nidx(g_emitter.codegen_flat_chain_body_nodes, n, g_emit_cfg->body_root);
                if (i >= 0 && (ir_kind0(g_emitter.codegen_flat_chain_body_nodes[i]) == IR_CALL_BUILTIN_GEN)) { resume_tgt = betas[i]; } }
        { extern int sn4_defer_resume(void); const int _dres = sn4_defer_resume();
          if (getenv("SCRIP_RESUME_WHY"))
              fprintf(stderr, "[RESUME-GATE] dres=%d flat_pat=%d cfg=%d body_root=%p\n", _dres, g_emit.flat_pat, g_emit_cfg ? 1 : 0, (void *)(g_emit_cfg ? g_emit_cfg->body_root : (IR_t *)0));
          if (_dres && g_emit.flat_pat && g_emit_cfg && g_emit_cfg->body_root) {
            int _f1 = 0; for (int i = 0; i < n; i++) if (g_emitter.codegen_flat_chain_body_nodes[i]->op == IR_MATCH_FENCE1) { _f1 = 1; break; }
            int _sd = resume_sealed_defer_on_root(g_emitter.codegen_flat_chain_body_nodes, n, g_emit_cfg->body_root);
            int _f1r = _f1; { extern int zdp_seam_tier(const IR_t *); const char * _fre = getenv("SCRIP_FENCE_RESUME");
                if (!(_fre && *_fre == '0') && g_emit_cfg->body_root && zdp_seam_tier(g_emit_cfg->body_root) != 0) _f1r = 0; }
            if (getenv("SCRIP_RESUME_WHY")) { extern int zdp_seam_tier(const IR_t *); const IR_t * _br = g_emit_cfg->body_root; int _found = 0;
                { int i = nidx(g_emitter.codegen_flat_chain_body_nodes, n, _br); if (i >= 0) _found = 1; }
                int _wnc = 0, _wlf = 0, _wfn = 0; sn4_blob_choice_scan(&_wnc, &_wlf, &_wfn);
                fprintf(stderr, "[RESUME-WHY] flat_pat=%d dres=%d f1=%d f1r=%d sd=%d body_root_op=%d tier=%d seal=%d in_nodes=%d n=%d nc=%d lf=%d fn=%d bfb=%d spgd=%d sph=%d exitlive=%d fam=%s\n",
                    g_emit.flat_pat, _dres, _f1, _f1r,
                    _sd, _br ? (int)_br->op : -1, _br ? zdp_seam_tier(_br) : -1, _br ? (int)_br->seal : -1, _found, n, _wnc, _wlf, _wfn, blob_frame_bytes(), _spgd, _sph,
                        planner_blob_exit_live(g_emit_cfg), fam ? fam : "?"); }
            if (!_f1r) { int i = nidx(g_emitter.codegen_flat_chain_body_nodes, n, g_emit_cfg->body_root);
                if (i >= 0 && (resume_carrier_ok(g_emitter.codegen_flat_chain_body_nodes[i], _sd))) { resume_tgt = betas[i]; } }
            if (!_f1 && !_sd && resume_tgt == &lbl_ω && sn4_alt_carrier() && g_emit_cfg->body_root &&
                (g_emit_cfg->body_root->op == IR_MATCH_ALTERNATE || g_emit_cfg->body_root->op == IR_DISJUNCTION) && !g_emit_cfg->body_root->seal) {
                int _nc = 0, _lf = 0, _fn = 0; sn4_blob_choice_scan(&_nc, &_lf, &_fn);
                if (_nc == 1 && !_fn) { int i = nidx(g_emitter.codegen_flat_chain_body_nodes, n, g_emit_cfg->body_root); if (i >= 0) { resume_tgt = betas[i];
                    } } }
            if (!_sd && resume_tgt == &lbl_ω && sn4_alt_carrier()) {
                int _nc = 0, _lf = 0, _fn = 0; sn4_blob_choice_scan(&_nc, &_lf, &_fn);
                if (_nc >= 1) {
                    int _mok = 1;
                    for (int i = 0; i < n && _mok; i++) { int _o = (int)g_emitter.codegen_flat_chain_body_nodes[i]->op;
                        if (_o == IR_DISJUNCTION) _mok = 0;
                        else if (_o == IR_MATCH_ALTERNATE && g_emitter.codegen_flat_chain_body_nodes[i]->seal) _mok =
                            0; }
                    int _ti = -1;
                    if (_mok) for (int i = 0; i < n; i++) { int _o = (int)g_emitter.codegen_flat_chain_body_nodes[i]->op;
                        if (_o == IR_GOTO || _o == IR_SUCCEED || _o == IR_FAIL) continue;
                        IR_t * _tt = zd_chase(g_emitter.codegen_flat_chain_body_nodes[i]->γ.node); if (_tt && _tt->op == IR_SUCCEED) { if (_ti >= 0) { _ti = -2; break; } _ti = i; } }
                    if (_mok && _ti >= 0 && g_emitter.codegen_flat_chain_body_nodes[_ti]->op != IR_MATCH_FENCE1 && g_emitter.codegen_flat_chain_body_nodes[_ti]->op != IR_MATCH_FENCE0) resume_tgt =
                        betas[_ti];
                } } } }
        long _rpop = 0;
        if (g_emit_cfg && g_emit_cfg->body_root && g_emit_cfg->body_root->op == IR_MATCH_ABORT) { int i = nidx(g_emitter.codegen_flat_chain_body_nodes, n, g_emit_cfg->body_root);
            if (i >= 0) { resume_tgt = lbls[i]; int _da = zd_arrival_depth(g_emitter.codegen_flat_chain_body_nodes, n, i, zd_on, zd_out, zd_gp, zd_wp);
                if (_spf && _spgd >= 0 && _da >= 0 && _spgd > _da) _rpop = _spgd - _da; } }
        if (resume_tgt == &lbl_ω && _spf && _spgd > 0 && _rpop == 0) _rpop = _spgd;
        if (icn_gen_regime() && g_emit.flat_gen && g_emitter.g_suspend_resume_slot >= 0)
            bb_emit_x86( x86("mov", "rax", FRQ(g_emitter.g_suspend_resume_slot)) + x86_jmp_reg("rax"));
        else { if (_rpop > 0) bb_emit_x86(x86("add", "rsp", _rpop)); emit_jmp_label(resume_tgt, JMP_JMP); }
    }
    }
    { extern int g_flat_frame_floor; int _wire_stub = ((g_emit.flat_jmp_entry && g_flat_frame_floor > 0) || (g_emit.flat_lcl_proc && !(g_emit_cfg && g_emit_cfg->cell_frame))) ? 1 : 0;
    int _blob_wire = (!_wire_stub && g_emit.flat_jmp_entry && g_emit.flat_pat) ? 1 : 0;
    if (!bare && !_top_hoist) {
    emit_sep_rule('-'); emit_label_define_bb(&lbl_γ);
    if (xa_flat_class_c_pred()) {
        int _has_nret = 0; for (int _ni = 0; _ni < n; _ni++) if (emit_floater_kind(g_emitter.codegen_flat_chain_body_nodes[_ni]) == 3) { _has_nret = 1; break; }
        if (_has_nret) emit_label_define_bb(emit_floater_label(1));
    }
    if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc)) { extern void xa_flat_zframe_epilogue_γ(void); xa_flat_zframe_epilogue_γ(); }
    else if (_blob_wire) {
        int _bfb = blob_frame_bytes(); int _sd = (_bfb > 0) ? 0 : (_spgd >= 0) ? _spgd + _sph : -1;
            if (_bfb <= 0 && _sd < 0) bb_emit_x86(x86_bomb("frameless stored-pattern thunk: its gamma exits leave the spine at different or unknown depths"));
        bb_emit_x86(IF(_bfb > 0,
            x86("mov", "rcx", RDQ("rbp", blob_cont_copy() ? -16 : 16)) + x86("push", "rbp") + x86("push", "rcx") + x86("mov", "rcx", RDQ("rbp", blob_cont_copy() ? -8 : 8)) + x86("push", "rcx") +
            x86_lea_ext("rax", &lbl_res) + x86("push", "rax") + x86("mov", "rbp", RDQ("rbp", 0)) + x86_jmp_reg("rcx")) +
            IF(_bfb <= 0,
            x86("mov", "rdx", RDQ("rsp", _sd + 8)) + x86("mov", "rcx", RDQ("rsp", _sd)) + x86("sub", "rsp", 8L) + x86("push", "rdx") + x86("push", "rcx") + x86_lea_ext("rax", &lbl_res) +
            x86("push", "rax") + x86_jmp_reg("rcx")));
    }
    else if (icn_gen_regime() && g_emit.flat_gen) {
        bb_emit_x86( icn_gen_line_save()
                  + x86("mov", "rdx", "rbp")
                  + x86_lea_ext("rax", &lbl_res) + x86("mov", RDQ("rdx", 32), "rax")
                  + x86("mov", "rcx", RDQ("rdx", 8))
                  + x86("mov", "rbp", RDQ("rdx", 0))
         + x86("mov32", "eax", (long)DT_S) + x86_jmp_reg("rcx"));
    }
    else if (xa_flat_class_c_pred() && !g_runtime.eval.rt_fragment_emit) { xa_flat_chain_epilogue_sig(1, fam); }
    else { if (xa_flat_class_c_pred()) xa_flat_chain_epilogue(); bb_emit_x86((_wire_stub ? bb_glue_wire_γ() : bb_glue_outer_γ())); }
    }
    if (!bare && !_top_hoist) {
    emit_sep_rule('-'); emit_label_define_bb(&lbl_ω);
    if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc)) { extern void xa_flat_zframe_epilogue_ω(void); xa_flat_zframe_epilogue_ω(); }
    else if (_blob_wire) { extern int sn4_blob_casmark(void);
        bb_emit_x86(IF(blob_frame_bytes() > 0, IF(sn4_blob_casmark(), x86("mov", "r12", RDQ("rbp", -40))) + x86("mov", "rsp", "rbp") + x86("pop", "rbp")) +
        IF(blob_frame_bytes() <= 0 && _sph > 0, IF(_sph == 32, x86("mov", "r12", RDQ("rsp", 8))) + x86("add", "rsp", (long)_sph)) +
        (blob_omega_ret() ? x86("add", "rsp", 8L) + x86("ret") : x86_rsp_load64("rcx", 8) + x86("add", "rsp", 16L) + x86("jmp", "rcx")));
        for (int _k = 0; _k < _wpn; _k++) { emit_label_define_bb(_wpl[_k]); bb_emit_x86(x86("add", "rsp", (long)_wpd[_k])); emit_jmp_label(&lbl_ω, JMP_JMP); }
        for (int _k = 0; _k < _tpn; _k++) { emit_label_define_bb(_tpl[_k]); bb_emit_x86(x86("add", "rsp", (long)_tpd[_k])); emit_jmp_label(_tpt[_k], JMP_JMP); } }
    else if (icn_gen_regime() && g_emit.flat_gen) {
        { bb_emit_x86( icn_trace_tap((strncmp(prefix, "proc_", 5) == 0) ? prefix + 5 : prefix, 5, 0)); }
        bb_emit_x86( x86("mov", "rcx", RDQ("rbp", 8))
                  + x86("mov", "rsp", RDQ("rbp", 24))
                  + x86("mov", "rbp", RDQ("rbp", 0))
                  + x86("mov32", "eax", (long)DT_FAIL) + x86_jmp_reg("rcx"));
    }
    else if (xa_flat_class_c_pred() && !g_runtime.eval.rt_fragment_emit) { xa_flat_chain_epilogue_sig(0, fam); }
    else { if (xa_flat_class_c_pred()) xa_flat_chain_epilogue(); bb_emit_x86((_wire_stub ? bb_glue_wire_ω() : bb_glue_outer_ω())); } } }
    { extern int g_flat_dc_np; extern long g_last_dc_off; g_last_dc_off = -1;
      if (g_flat_dc_np >= 0) {
          if (!g_emit.flat_jmp_entry || !g_emit.flat_lex || g_emit.flat_gen || g_emit.flat_pat)
              { fprintf(stderr, "FATAL PL-DC: driver-armed graph is not det-lexical jmp-entry (jmp=%d lex=%d gen=%d pat=%d)\n", g_emit.flat_jmp_entry, g_emit.flat_lex, g_emit.flat_gen,
              g_emit.flat_pat); abort(); }
          bb_label_t lbl_dc; emit_label_initf(&lbl_dc, "%s_dcα", fam);
          emit_sep_rule('-'); emit_label_define_bb(&lbl_dc);
          g_emit.flat_dc_body_p = _blk_graph ? &lbl_αblk : (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc)) ? &lbl_α : &lbl_α_body;
          { extern void xa_flat_dc_stub(void); xa_flat_dc_stub(); }
          g_emit.flat_dc_body_p = (bb_label_t *)0;
          if (!g_is_text) g_last_dc_off = (long)lbl_dc.offset;
      } }
    emit_gc_map_data(fam); g_emitter.g_gc_sites_armed = 0;
    if (text_externalise && g_is_text) {
        data_buf_flush_pending_label();
        xa_dispatch(XA_FLAT_DATA_SECTION);
        data_buf_reset();
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_chain_arity(const IR_t *n) {
    switch (n->op) {
    case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_ATOM: case IR_LIT_REAL:
    case IR_VAR: case IR_KW_ICON: case IR_KW_ICON_GEN: case IR_KW_SNOBOL4: return 0;
    case IR_BINOP: case IR_TO: return 2;
    case IR_TO_BY: return 3;
    case IR_MAKE_LIST: return n->n_operands;
    case IR_CONJUNCTION: case IR_GOTO: return 0;
    case IR_UNOP: case IR_UNOP_TEST: return 1;
    case IR_ASSIGN: return 1;
    case IR_RETURN: return 1;
    case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN:
    case IR_CALL_ICON: case IR_CALL_SNOBOL4: case IR_CALL_DIRECT: case IR_CALL_PL_LEAF: case IR_CALL_PAS_BI: case IR_CALL_CALLEE_REC: case IR_CALL_FIELD_REC: case IR_CALL_BYNAME:
    case IR_CALL_TRY: case IR_CALL_METHOD: case IR_CALL_REDISP: case IR_CALL_APPLY: case IR_CALL_EVAL: case IR_CALL_GEN:
    case IR_CALL: return n->n_operands;
    case IR_PROC_GEN: return 0;
    case IR_SCAN_TAB: case IR_SCAN_MOVE: case IR_SCAN_POS:
    case IR_SCAN_UPTO: case IR_SCAN_ANY: case IR_SCAN_MANY:
    case IR_SCAN_FIND: case IR_SCAN_MATCH: case IR_SCAN_BAL: return 0;
    default: return -1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern const char __start_rtx_entry_names[] __attribute__((visibility("hidden")));
extern const char __stop_rtx_entry_names[] __attribute__((visibility("hidden")));
static uint32_t rtxe_hash(const char * s) { uint32_t h = 2166136261u; for (const unsigned char * p = (const unsigned char *)s; *p; p++) { h ^= *p; h *= 16777619u; } return h; }
#include "../templates/x86/rtx_clobber_table.inc"
extern "C" unsigned emit_rtx_clob_mask(const char * sym) {
    if (!sym) return 0u;
    for (int i = 0; i < RTX_CLOB_TAB_N; i++) if (rtx_clob_tab[i].sym[0] == sym[0] && strcmp(rtx_clob_tab[i].sym, sym) == 0) return rtx_clob_tab[i].mask;
    return 0u;
}
struct rtx_ssr_t { const char * s; long n; };
struct rt_dcap_next_t { long fn; long how; };
#define RTX_TWIN(kind, ret, leaf, twin, args) extern "C" ret twin args;
#include "../templates/x86/rtx_twin_table.inc"
#undef RTX_TWIN
static const struct { char sym[36]; char cname[36]; void * caddr; int kind; } rtx_twin_tab[] = {
#define RTX_TWIN(kind, ret, leaf, twin, args) { #leaf, #twin, (void *)twin, RTX_VERDICT_##kind },
#include "../templates/x86/rtx_twin_table.inc"
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
static ocr_slot_t * ocr_probe(const IR_t * nd) {
    uint32_t m = g_emitter.g_ocr_map.len - 1; uint64_t h = (((uint64_t)(uintptr_t)nd >> 4) * 0xff51afd7ed558ccdull) & m;
    for (;;) { ocr_slot_t * t = &CV_AT(g_emitter.g_ocr_map, ocr_slot_t, h); if (t->gen != g_emitter.g_ocr_gen || t->nd == nd) return t; h = (h + 1) & m; }
}
static void ocr_table(uint32_t c) {
    cv_t old = g_emitter.g_ocr_map; cv_t v = { 0, 0, 0, 0 }; v.p = ct_zalloc(c, sizeof(ocr_slot_t)); v.len = c; v.cap = c; v.esz = (uint32_t)sizeof(ocr_slot_t); g_emitter.g_ocr_map = v;
    for (uint32_t i = 0; i < old.len; i++) { ocr_slot_t * o = &CV_AT(old, ocr_slot_t, i); if (o->gen == g_emitter.g_ocr_gen) { ocr_slot_t * t = ocr_probe(o->nd); t->nd = o->nd;
        t->gen = g_emitter.g_ocr_gen; } }
}
static void ocr_push(cv_t * v, int * n, IR_t * x) { cv_reserve(v, (uint32_t)sizeof(IR_t *), (uint64_t)*n + 1, "ocr"); CV_AT(*v, IR_t *, *n) = x; (*n)++; }
static void ocr_clear(void) { g_emitter.g_ocr_gen++; g_emitter.g_ocr_cnt = 0; if (!g_emitter.g_ocr_map.len) ocr_table(1024); }
static int ocr_has(const IR_t * nd) { return ocr_probe(nd)->gen == g_emitter.g_ocr_gen; }
static void ocr_add(const IR_t * nd) { ocr_slot_t * t = ocr_probe(nd); if (t->gen == g_emitter.g_ocr_gen) return; t->nd = nd; t->gen = g_emitter.g_ocr_gen;
    if ((uint64_t)++g_emitter.g_ocr_cnt * 2 >= (uint64_t)g_emitter.g_ocr_map.len) ocr_table(g_emitter.g_ocr_map.len * 2); }
static void emit_chain_operand_refs(IR_t *entry) {
    int nc = 0;
    ocr_clear();
    int sv = 0;
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL) && entry->γ.node && guard++ < 8192) entry = entry->γ.node; }
    entry = entry;
    ocr_push(&g_emitter.g_ocr_stkv, &sv, entry);
    while (sv > 0) {
        IR_t *c = CV_AT(g_emitter.g_ocr_stkv, IR_t *, --sv);
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        int dup = ocr_has(c);
        if (dup) continue;
        ocr_add(c); ocr_push(&g_emitter.g_ocr_chain, &nc, c);
        if ((c->op == IR_BINOP) && c->ω.node) ocr_push(&g_emitter.g_ocr_stkv, &sv, c->ω.node);
        if ((c->op == IR_CALL || ir_is_call_kind(c->op)) && c->ω.node) ocr_push(&g_emitter.g_ocr_stkv, &sv, c->ω.node);
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_UNIFY_CONST || c->op == IR_UNIFY_STRUCT || c->op == IR_UNIFY_FIRST || c->op == IR_UNIFY_VALUE ||
            c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_KW_ASSIGN || c->op == IR_SCAN_TAB || c->op == IR_SCAN_MOVE || c->op == IR_SCAN_POS || c->op == IR_SCAN_MATCH ||
            c->op == IR_SCAN_ANY
             || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE || c->op == IR_VAR) && c->ω.node)
            ocr_push(&g_emitter.g_ocr_stkv, &sv, c->ω.node);
        if (c->γ.node) ocr_push(&g_emitter.g_ocr_stkv, &sv, c->γ.node);
    }
    for (int i = 0; i < nc; i++) if (ir_is_generator_kind(CV_AT(g_emitter.g_ocr_chain, IR_t *, i)->op) && CV_AT(g_emitter.g_ocr_chain, IR_t *, i)->ω.node) {
        int present = ocr_has(CV_AT(g_emitter.g_ocr_chain, IR_t *, i)->ω.node);
        if (!present) ocr_push(&g_emitter.g_ocr_stkv, &sv, CV_AT(g_emitter.g_ocr_chain, IR_t *, i)->ω.node); }
    while (sv > 0) {
        IR_t *c = CV_AT(g_emitter.g_ocr_stkv, IR_t *, --sv);
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        int dup = ocr_has(c);
        if (dup) continue;
        ocr_add(c); ocr_push(&g_emitter.g_ocr_chain, &nc, c);
        if ((c->op == IR_BINOP) && c->ω.node) ocr_push(&g_emitter.g_ocr_stkv, &sv, c->ω.node);
        if ((c->op == IR_CALL || ir_is_call_kind(c->op)) && c->ω.node) ocr_push(&g_emitter.g_ocr_stkv, &sv, c->ω.node);
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_UNIFY_CONST || c->op == IR_UNIFY_STRUCT || c->op == IR_UNIFY_FIRST || c->op == IR_UNIFY_VALUE ||
            c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_KW_ASSIGN || c->op == IR_SCAN_TAB || c->op == IR_SCAN_MOVE || c->op == IR_SCAN_POS || c->op == IR_SCAN_MATCH ||
            c->op == IR_SCAN_ANY
             || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE || c->op == IR_VAR) && c->ω.node)
            ocr_push(&g_emitter.g_ocr_stkv, &sv, c->ω.node);
        if (ir_is_generator_kind(c->op) && c->ω.node) ocr_push(&g_emitter.g_ocr_stkv, &sv, c->ω.node);
        if (c->γ.node) ocr_push(&g_emitter.g_ocr_stkv, &sv, c->γ.node);
    }
    int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = CV_AT(g_emitter.g_ocr_chain, IR_t *, i);
        switch (n->op) { case IR_RETURN: case IR_SUSPEND: case IR_CORET: case IR_COFAIL: sp = 0; continue; default: break; }
        int ar = emit_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        else if (ar >= 1 && sp >= ar) { if (n->n_operands < ar) { n->n_operands = 0; for (int k = ar; k >= 1; k--) ir_operand_push(n, CV_AT(g_emitter.g_ocr_stk, IR_t *, sp - k)); } sp -= ar; }
        else if (ar >= 1) { sp = 0; }
        ocr_push(&g_emitter.g_ocr_stk, &sp, n);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_jmp_entry_arm_region(IR_graph_t *g) {
    extern int zls_g_region(const IR_graph_t *);
    extern int zls_g_resume(const IR_graph_t *);
    extern int zls_g_zeta_mark(const IR_graph_t *);
    int rg = g ? zls_g_region(g) : -1;
    int so = g ? zls_g_resume(g) : -1;
    if (so < 0 && g) so = zls_g_zeta_mark(g);
    if (so < 0 && g) { extern int zls_g_locals(const IR_graph_t *); so = zls_g_locals(g); if (so < 0) so = rg; }
    int capen; { if (g_emitter.emit_jmp_entry_arm_region_v < 0) { const char *e = getenv("SCRIP_CAP_NOFILL"); g_emitter.emit_jmp_entry_arm_region_v = e ? (atoi(e) != 0) : 1;
        } capen = g_emitter.emit_jmp_entry_arm_region_v; }
    g_emit.flat_cap_n = 0; int capovf = !capen;
    if (g) for (int i = 0; i < g->n; i++) { IR_t *nd = g->all[i]; if (nd && nd->op == IR_MATCH_ASSIGN_SAVE) { int off = nd_slot(nd); if (off < 0) continue;
        if (!capovf) { cv_reserve(&g_emit.flat_cap_off, (uint32_t)sizeof(int), (uint64_t)g_emit.flat_cap_n + 1, "flat_cap_off"); CV_AT(g_emit.flat_cap_off, int, g_emit.flat_cap_n) = off;
            g_emit.flat_cap_n++; continue; }
        if (so < 0 || off < so) so = off; } }
    if (g_emit.flat_cap_n > 1) { int * co = &CV_AT(g_emit.flat_cap_off, int, 0); int u = 1;
        qsort(co, (size_t)g_emit.flat_cap_n, sizeof(int), [](const void * a, const void * b) -> int { int x = *(const int *)a, y = *(const int *)b; return (x > y) - (x < y); });
        for (int k = 1; k < g_emit.flat_cap_n; k++) if (co[k] != co[u - 1]) co[u++] = co[k]; g_emit.flat_cap_n = u; }
    g_emit.flat_layout_unknown = 0;
    if (rg <= 0) { rg = 4096; so = -1; g_emit.flat_layout_unknown = 1; }
    { extern int g_flat_frame_floor; if (g_flat_frame_floor > 0 && rg < g_flat_frame_floor) { rg = g_flat_frame_floor; so = -1; g_emit.flat_layout_unknown = 1; } }
    g_emit.flat_jmp_entry = 1; { extern int zls_g_frame_bytes(const IR_graph_t *); g_emit.flat_frame_bytes = g_emit_cfg ? zls_g_frame_bytes(g_emit_cfg) : ((FLAT_FRAME_ALLOWANCE + 15) & ~15); }
    g_emit.flat_seed_off = (so >= 16 && so <= rg) ? so : 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_jmp_entry_for_patproc(int thunk_kind, IR_graph_t *g) {
    if (thunk_kind != PROC_THUNK_PATTERN) return 0;
    g_emit.flat_pat = 1;
    return emit_jmp_entry_arm_region(g);
}
int g_flat_frame_floor = 0;
int g_flat_dc_np = -1;
long g_last_dc_off = -1;
extern "C" long emit_last_pkt_chain_off(void) { return g_emit.flat_pkt_chain_off; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zop_audit_seen(void) { return g_emit.zop_seen; }
extern "C" void zop_audit_seen_clear(void) { g_emit.zop_seen = 0; }
extern "C" void emit_jmp_entry_clear(void) { extern int g_flat_frame_floor; g_flat_frame_floor = 0; g_emit.flat_jmp_entry = 0; g_emit.flat_frame_bytes = 0; g_emit.flat_seed_off = 0;
    g_emit.flat_layout_unknown = 0; g_emit.flat_pat = 0; g_emit.flat_lex = 0; g_emit.flat_gen = 0; g_emit.flat_deep_arrival = 0; g_emit.flat_cap_n = 0; g_flat_dc_np = -1; g_emit.flat_lcl_proc = 0;
    g_emit.zframe_graph = 0; g_emit.frame_retained = 0; }
int emit_graph_has_suspend(IR_graph_t *g) { if (!g) return 0; for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->op == IR_SUSPEND) return 1; return 0; }
static int emit_graph_has_deep_arrival(IR_graph_t *g) { if (!g) return 1; for (int i = 0; i < g->n; i++) { IR_t *c = g->all[i]; if (!c) continue;
    switch (c->op) { case IR_SUSPEND: case IR_SCAN: case IR_SCAN_ENTER: case IR_SCAN_ALTERNATE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL: case IR_SCAN_MATCH:
    case IR_SCAN_MOVE: case IR_SCAN_TAB: case IR_TO: case IR_TO_BY: case IR_LIMIT: case IR_REPALT: case IR_PROC_GEN: case IR_CREATE: case IR_ITERATE: case IR_DISJUNCTION: case IR_CALL_BUILTIN_GEN:
    case IR_KW_ICON_GEN: case IR_MATCH_FENCE1: case IR_MATCH_ABORT: case IR_MATCH_ARBNO: case IR_MATCH_CALLOUT: case IR_MATCH_VALUE: return 1; case IR_MATCH_DEFER: return 1; default: break;
    } } return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_fb_bit_of(IR_t * nd) { int i = g_emitter.g_fbm_n > 0 ? ir_index_of(&g_emitter.g_fbm_nd, nd) : -1;
    return (i >= 0 && i < g_emitter.g_fbm_n) ? CV_AT(g_emitter.g_fbm_bit, unsigned char, i) : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_fb_stmt_scan(IR_graph_t * g) {
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
static int emit_stmt_frame_scan(IR_graph_t * g, IR_t * entry) {
    if (!g || !entry || g->n <= 0 || g->n > 8192) return 0;
     if (g_emitter.emit_stmt_frame_scan__sd < 0) { const char * e = getenv("SCRIP_STF_DEBUG"); g_emitter.emit_stmt_frame_scan__sd = (e && *e == '1') ? 1 : 0; }
    enum { STF_MAX = 8192 };
    IR_t ** q = (IR_t **)alloca(sizeof(IR_t *) * STF_MAX); IR_t ** seen = (IR_t **)alloca(sizeof(IR_t *) * STF_MAX); int qh = 0, qt = 0, sn = 0;
    q[qt++] = entry;
    while (qh < qt) { IR_t * c = q[qh++];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        { int dup = 0; for (int i = 0; i < sn; i++) if (seen[i] == c) { dup = 1; break; } if (dup) continue; }
        if (sn >= STF_MAX || qt >= STF_MAX - 2) { if (g_emitter.emit_stmt_frame_scan__sd) fprintf(stderr, "[STF] reject overflow sn=%d qt=%d\n", sn, qt); return 0; }
        seen[sn++] = c;
        if ((c->γ.node && c->γ.node == entry) || (c->ω.node && c->ω.node == entry)) { if (g_emitter.emit_stmt_frame_scan__sd)
            fprintf(stderr, "[STF] reject BACKWARD EDGE TO FIRST HEAD sn=%d\n", sn); return 0; }
        switch (c->op) { case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_ATOM: case IR_ASSIGN: break;
            case IR_VAR: case IR_BINOP: if (c->ω.node) q[qt++] = c->ω.node; break;
            default: if (c->ω.node) q[qt++] = c->ω.node; break; }
        { extern int fc_geom(const IR_t *, long *); long _fk = 0; int _fc = fc_geom(c, &_fk); if (g_emitter.emit_stmt_frame_scan__sd) { extern const char * bb_op_name(IR_e);
            fprintf(stderr, "[STF] node %s fc=%d k=%ld\n", bb_op_name(c->op), _fc, _fk); }
           if (g_emitter.emit_stmt_frame_scan__c2c < 0) { const char * e2 = getenv("SCRIP_CALL2BB"); g_emitter.emit_stmt_frame_scan__c2c = (e2 && *e2 == '1') ? 1 : 0; }
          int _callfam =
              (g_emitter.emit_stmt_frame_scan__c2c && (ir_kind0(c) == IR_CALL || ir_kind0(c) == IR_CALL_PROC_STAGED || (c->op == IR_DEFINE && ir_define_sr_citizen(c)) || c->op == IR_GOTO_DEFERRED)) ?
              1 : 0;
          { extern int fc_vread_fp(const IR_t *); if (!_fc && c->op == IR_MATCH_BEGIN && fc_vread_fp(c) >= 0) _fc = 1;
            if (!_fc && c->op == IR_MATCH_END && c->n_operands > 0 && c->operands[0] && fc_vread_fp(c->operands[0]) >= 0) _fc = 1;
            if (!_fc && c->op == IR_MATCH_LIT) _fc = 1; }
          int _stfwire = (c->op == IR_GOTO || c->op == IR_STATEMENT_BEGIN || c->op == IR_STATEMENT_END) ? 1 : 0;
          if (!_fc && !(c->op == IR_BINOP || c->op == IR_ASSIGN || _stfwire || _callfam)) { if (g_emitter.emit_stmt_frame_scan__sd) { extern const char * bb_op_name(IR_e);
              fprintf(stderr, "[STF] reject UNREGISTERED %s sn=%d\n", bb_op_name(c->op), sn); } return 0; } }
        if (c->γ.node) q[qt++] = c->γ.node; }
    if (g_emitter.emit_stmt_frame_scan__sd) fprintf(stderr, "[STF] ADMIT reachable=%d (g->n=%d)\n", sn, g->n);
    return sn > 0 ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_graph_zstatic(IR_graph_t *g) { if (!g) return 0; for (int i = 0; i < g->n; i++) { IR_t *c = g->all[i]; if (c && (c->op == IR_MATCH_DEFER || c->op == IR_MATCH_VALUE)) return 0;
    } return 1; }
static int emit_graph_uniform(IR_graph_t *g) { if (!emit_graph_zstatic(g)) return 0; for (int i = 0; i < g->n; i++) { IR_t *c = g->all[i]; if (c && c->op == IR_MATCH_ARBNO) return 0; } return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_jmp_entry_for_proc(const char *pname, int dyn_scope, int is_generator, IR_graph_t *g) {
    if (pname && strncmp(pname, "gram__", 6) == 0) return 0;
    int is_lbl = pname && strncmp(pname, "LBL__", 5) == 0;
    g_emit.flat_lex = (!dyn_scope && !is_lbl) ? 1 : 0;
    g_emit.flat_gen = (is_generator && emit_graph_has_suspend(g)) ? 1 : 0;
    int _r = emit_jmp_entry_arm_region(g);
    if (getenv("SCRIP_FLOOR_DIAG")) { extern int g_flat_frame_floor;
        fprintf(stderr, "[ARM-REGION] pname=%s r=%d floor=%d frame_bytes=%d seed_off=%d layout_unknown=%d jcon_value_region=%d\n", pname ? pname : "(null)", _r, g_flat_frame_floor,
        g_emit.flat_frame_bytes, g_emit.flat_seed_off, g_emit.flat_layout_unknown, g ? g->jcon_value_region : -1); }
    return _r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_icn_n2_gen_region_ft(const char *pname, int is_generator, IR_graph_t *g) {
    if (!pname || !is_generator || !g || !g->cell_frame || g->zframe_graph || !emit_graph_has_suspend(g)) return 0;
    { int ft = 0; if (!emit_patzeta_frame_reserve(pname, &ft) || ft <= 0) return 0; return ft; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_jmp_entry_for_chain(IR_graph_t *g) {
    extern int g_flat_dc_np; g_flat_dc_np = -1;
    return emit_jmp_entry_arm_region(g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_chain_prepare(IR_t * entry) {
    g_emitter.g_xci_epoch++;
    g_emit.stno_last = -1; g_emit.icn_line_cur = 0; g_emit.icn_file_cur = (const char *)0;
    emit_chain_operand_refs(entry);
    if (g_emit_cfg) zls_fct_finalize(g_emit_cfg, 1);
    g_emitter.g_bb_slotmap_n = 0; g_emitter.g_bb_slotix_gen++;
    { extern int g_scan_regs_live; g_scan_regs_live = 0; }
    { extern void zop_audit_graph_close(void); zop_audit_graph_close(); g_emit.zop_seen = 0; }
    g_emit.op_zres = 0; g_emit.op_zf_unpinned = 0; g_emit.op_zabs = 0;
    g_emit.flat_all_zd = 0;
    g_emit.flat_deep_arrival = emit_graph_has_deep_arrival(g_emit_cfg);
    { int _rc_own = (g_emit_cfg && g_emit_cfg->resumable_callable) ? 1 : 0;
        g_emit.flat_fb_refine = (g_emit.flat_deep_arrival && !g_emit.flat_pat && !g_emit.flat_gen && !_rc_own) ? emit_fb_stmt_scan(g_emit_cfg) : 0;
      { if (g_emitter.emit_chain_fbdbg2 < 0) { const char * e = getenv("SCRIP_FB_DEBUG"); g_emitter.emit_chain_fbdbg2 = (e && *e == '1') ? 1 : 0;
          } if (g_emitter.emit_chain_fbdbg2 && !g_emit.flat_fb_refine)
          fprintf(stderr, "[FB-STMT] hook skip deep=%d pat=%d gen=%d gp=%d rc=%d n=%d\n", g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, g_resumable_callable_active,
          g_emit_cfg ? g_emit_cfg->n : -1); } }
    { extern int g_flat_outer_nparams; extern int g_flat_frame_floor; g_emit.flat_outer_nparams = g_flat_outer_nparams; (void)g_flat_frame_floor;
      { int _gen_ok = !g_emit.flat_gen;
        g_emit.flat_lcl_proc = (!g_emit.flat_pat && _gen_ok && g_emit_cfg && ((g_emit.flat_jmp_entry && (g_emit_cfg->nparams > 0 || g_emit_cfg->nlocals > 0)) || g_emit_cfg->cell_frame)) ? 1 : 0;
            }
      g_emit.frame_retained = planner_graph_retains(g_emit_cfg);
      g_emit.zframe_graph = (g_emit_cfg && g_emit_cfg->zframe_graph && !g_emit_cfg->cell_frame) ? 1 : 0;
      if ((g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->cell_frame)) && g_emit.flat_frame_bytes == 0) { extern int zls_g_frame_bytes(const IR_graph_t *);
          g_emit.flat_frame_bytes = g_emit_cfg ? zls_g_frame_bytes(g_emit_cfg) : ((FLAT_FRAME_ALLOWANCE + 15) & ~15); } }
    g_last_flat_frame_bytes = g_emit_cfg ? g_emit_cfg->jcon_value_region : 0;
    { if (getenv("SCRIP_N2_FT_PROBE") && g_emit_cfg) { int _rg = g_emit_cfg->jcon_value_region, _np = g_emit_cfg->nparams, _nl = g_emit_cfg->nlocals;
        fprintf(stderr, "[N2-FT] EMIT gen=%d zframeA=%d icncells=%d lclproc=%d region=%d ffb=%d np=%d nl=%d zls=%d ft=%d predft=%d\n", g_emit.flat_gen ? 1 : 0, g_emit.zframe_graph ? 1 : 0,
            g_emit_cfg->cell_frame ? 1 : 0, g_emit.flat_lcl_proc ? 1 : 0, _rg, g_emit.flat_frame_bytes, _np, _nl, zls_g_fp_total(g_emit_cfg),
                g_emit.zframe_graph ? g_emit.flat_frame_bytes : g_emit.flat_frame_bytes + (_np + _nl) * 16, g_emit.zframe_graph ? ((48 + _rg + 15) & ~15) : ((48 + _rg + 15) & ~15) + (_np + _nl) * 16);
                    } }
    g_last_flat_zstatic = (g_emit_cfg && !g_emit.flat_layout_unknown) ? emit_graph_zstatic(g_emit_cfg) : 0;
    g_last_flat_fp = (g_emit_cfg && !g_emit.flat_layout_unknown) ? (g_emit.zframe_graph ? 0 : (g_emit.flat_gen ? (g_emit_cfg->nparams + g_emit_cfg->nlocals) * 16 : zls_g_fp_total(g_emit_cfg))) : 0;
    g_last_flat_uniform = (g_emit_cfg && !g_emit.flat_layout_unknown) ? emit_graph_uniform(g_emit_cfg) : 0;
    planner_act_build(g_emit_cfg, blob_frame_scope(), blob_head_bytes());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_box_fn emit_chain(IR_t *entry, FILE *out, const char *prefix) {
    if (!entry) return NULL;
    emit_chain_prepare(entry);
    int rtplan = planner_rt_plan(entry, prefix);
    if (out) { emitter_init_text(out, TEXT_MODE_INVOCATION); int rc = codegen_flat_chain_body(entry, prefix); emitter_end(); if (rtplan) planner_table_forget(entry);
        return rc == 0 ? (bb_box_fn)1 : NULL; }
    long flat_cap = FLAT_BUF_MAX; if (g_emit_cfg && (long)g_emit_cfg->n * 2048L > flat_cap) { flat_cap = (long)g_emit_cfg->n * 2048L;
        if (flat_cap > 1024L * 1024L * 1024L) flat_cap = 1024L * 1024L * 1024L; }
    bb_buf_t buf = bb_alloc(flat_cap);
    if (!buf) return NULL;
    g_flat_node_id = 0;
    emitter_init_binary(buf, flat_cap);
    codegen_flat_chain_body(entry, prefix);
    if (rtplan) planner_table_forget(entry);
    if (g_emit_chain_posthook) { void (*h)(void) = g_emit_chain_posthook; g_emit_chain_posthook = NULL; h(); }
    int nbytes = emitter_end();
    if (g_emitter.bb_emit_overflow || nbytes <= 0 || nbytes > flat_cap) {
        extern int bb_emit_pos; extern int bb_patch_count;
        fprintf(stderr, "[IBB] emit_chain('%s') FAILED: overflow=%d nbytes=%d pos=%d cap=%d patches=%d — %s\n",
                prefix ? prefix : "", g_emitter.bb_emit_overflow, nbytes, bb_emit_pos, flat_cap, bb_patch_count,
                (bb_emit_pos >= flat_cap) ? "flat_cap exceeded (graph too large for the 1MB flat buffer)" : "empty or invalid emission");
        bb_free(buf, flat_cap); return NULL;
    }
    bb_seal(buf, (size_t)nbytes);
    { const char *_pm = getenv("SCRIP_PERF_MAP");
      if (_pm && *_pm && *_pm != '0') {
          char _pmp[64]; snprintf(_pmp, sizeof _pmp, "/tmp/perf-%d.map", (int)getpid());
          FILE *_pmf = fopen(_pmp, "a");
          if (_pmf) { fprintf(_pmf, "%llx %llx m3_%s\n", (unsigned long long)(uintptr_t)buf, (unsigned long long)(size_t)nbytes,
                              (prefix && *prefix) ? prefix : "chain"); fclose(_pmf); } } }
    bb_pool_trim_last(buf, flat_cap, (size_t)nbytes);
    return (bb_box_fn)buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_chain_plan(IR_t * entry, const char * prefix) { if (!entry) return; emit_chain_prepare(entry); planner_pass_plan(entry, prefix); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" {
void rt_proc_set_generator(const char *, int); void rt_proc_set_jmpentry(const char *, int); void rt_proc_set_pinned(const char *, int); void rt_proc_set_variadic(const char *, int);
void rt_proc_set_rest_kind(const char *, int); void rt_proc_set_named_rest(const char *, int); void rt_proc_set_dyn_scope(const char *, int); void rt_proc_set_result_name(const char *, const char *);
    void rt_proc_set_thunk(const char *, int);
void rt_proc_register(const char *, const char **, int); void rt_proc_set_nformals(const char *, int); void rt_proc_set_frame_bytes(const char *, int); void rt_proc_set_fn(const char *, bb_box_fn);
    void rt_proc_set_dcfn(const char *, void *); void rt_proc_set_zstatic(const char *, int);
void rt_proc_set_gen_region_ft(const char *, int); void bb_ab_seal_entry_cells(const char *, void *, int); void emit_patzeta_register(const char *, int, int, int); int rt_pl_dc_ok(const char *, int);
int zls_g_entry_block(const IR_graph_t *); int zls_g_region(const IR_graph_t *); void zls_graph_name(const IR_graph_t *, const char *); int proc_role3_kind(const IR_graph_t *);
    const char * bb_ab_sym_name(const char *);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_register_proc(stage2_t * s2, int pi) {
    ProcEntry * pe = &s2->proc_table[pi]; int np = pe->nparams; const char ** pn = NULL;
    if (np > 0) { pn = (const char **) ct_zalloc((size_t) np, sizeof(const char *)); for (int k = 0; k < np && k < pe->lower_sc.n; k++) pn[k] = pe->lower_sc.e[k].name; }
    rt_proc_register(pe->name, pn, np); rt_proc_set_nformals(pe->name, pe->nformals);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_proc_props(stage2_t * s2, int pi) {
    ProcEntry * pe = &s2->proc_table[pi]; const char * pname = pe->name; IR_graph_t * g = s2->bbp.table[pe->bb_idx];
    rt_proc_set_generator(pname, pe->is_generator); rt_proc_set_jmpentry(pname, !g->caller_frame && strncmp(pname, "gram__", 6) != 0); rt_proc_set_pinned(pname, zls_g_entry_block(g));
    rt_proc_set_variadic(pname, pe->is_variadic); rt_proc_set_rest_kind(pname, pe->rest_kind); rt_proc_set_named_rest(pname, pe->named_rest); rt_proc_set_dyn_scope(pname, pe->dyn_scope);
    if (pe->result_name) rt_proc_set_result_name(pname, pe->result_name);
    rt_proc_set_thunk(pname, pe->thunk_kind != PROC_THUNK_NONE);
    if (pe->thunk_kind == PROC_THUNK_EXPR) emit_thunk_star_bind(pname);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_proc_prepare(stage2_t * s2, int pi, int with_props, int * isp_out) {
    ProcEntry * pe = &s2->proc_table[pi]; const char * pname = pe->name; IR_graph_t * g = s2->bbp.table[pe->bb_idx];
    int is_lbl = pname && strncmp(pname, "LBL__", 5) == 0;
    g_emit_cfg = g;
    if (with_props) emit_proc_props(s2, pi);
    g_gen_proc_active = pe->is_generator; g_flat_frame_floor = 0;
    int floor_hit = (is_lbl || (g->entry && ((g->entry->op == IR_DEFINE && IR_LIT(g->entry).ival == 3) || g->entry->op == IR_GOTO_DEFERRED)));
    if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[FLOOR-DIAG] pname=%s is_lbl=%d entry_op=%d hit=%d\n", pname ? pname : "(null)", is_lbl, g->entry ? (int) g->entry->op : -1, floor_hit);
    if (floor_hit) for (int mi = 0; mi < s2->proc_count; mi++) if (s2->proc_table[mi].name && !strcmp(s2->proc_table[mi].name, "main")) {
        int mx = s2->proc_table[mi].bb_idx; if (mx >= 0 && mx < s2->bbp.count && s2->bbp.table[mx]) g_flat_frame_floor = zls_g_region(s2->bbp.table[mx]); break; }
    if (getenv("SCRIP_FLOOR_DIAG") && floor_hit) fprintf(stderr, "[FLOOR-DIAG] -> g_flat_frame_floor=%d\n", g_flat_frame_floor);
    int isp = emit_jmp_entry_for_patproc(pe->thunk_kind, g); if (!isp) emit_jmp_entry_for_proc(pname, pe->dyn_scope, pe->is_generator, g);
    g_flat_dc_np = (!isp && rt_pl_dc_ok(pname, pe->nparams)) ? pe->nparams : -1;
    zls_graph_name(g, pname); g_emit_frame_caller_dl = (g->caller_frame && g->nslots > 0) ? pe->decl_level : -1;
    if (isp_out) *isp_out = isp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_proc_unprepare(void) { emit_jmp_entry_clear(); g_emit_frame_caller_dl = -1; g_gen_proc_active = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void * emit_install_proc(stage2_t * s2, int pi, const emit_install_hooks_t * hk) {
    ProcEntry * pe = &s2->proc_table[pi]; const char * pname = pe->name; IR_graph_t * g = s2->bbp.table[pe->bb_idx];
    int is_lbl = pname && strncmp(pname, "LBL__", 5) == 0;
    int isp = 0; emit_proc_prepare(s2, pi, 1, &isp);
    char * pfx = (char *) alloca(strlen(pname) + 6); snprintf(pfx, strlen(pname) + 6, "proc_%s", pname);
    IR_t * pent = pe->proc_entry_node ? pe->proc_entry_node : g->entry;
    if (!is_lbl && hk && hk->before) hk->before(hk->ctx, pi, pe->bb_idx);
    if (getenv("SCRIP_PL_RTASM") && !is_lbl) { fprintf(stderr, "[RTASM] ---- compile-time proc %s ----\n", pname); emit_chain(pent, stderr, pfx); fprintf(stderr, "[RTASM] ---- end %s ----\n", pname);
        }
    bb_box_fn pfn = is_lbl ? NULL : emit_chain(pent, NULL, pfx);
    if (pfn && hk && hk->after) hk->after(hk->ctx, pi, pe->bb_idx, (void *) pfn);
    emit_gc_tables_register((const void *) pfn);
    emit_proc_unprepare();
    if (!is_lbl) rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes);
    if (getenv("SCRIP_N2_FB_PREPASS")) fprintf(stderr, "[N2-FB] POSTEMIT mode=3 proc=%s idx=%d fb=%d\n", pname ? pname : "(null)", pi, g_last_flat_frame_bytes);
    if (pfn) rt_proc_set_fn(pname, pfn);
    if (pfn && isp) bb_thunk_rec_fill(pname, (void *) pfn, g_last_flat_frame_bytes, g_last_flat_zstatic);
    if (pfn) bb_ab_seal_entry_cells(pname, (void *) pfn, 1);
    if (pfn) rt_proc_set_zstatic(pname, g_last_flat_zstatic);
    if (!is_lbl) emit_patzeta_register(pname, g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform);
    int gft = emit_icn_n2_gen_region_ft(pname, pe->is_generator, g); if (gft > 0) rt_proc_set_gen_region_ft(pname, gft);
    if (pfn && g_last_dc_off >= 0) rt_proc_set_dcfn(pname, (void *) ((char *) pfn + g_last_dc_off));
    if (pfn && g_last_flat_frame_bytes > 0) rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes);
    return (void *) pfn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int proc_role3_kind(const IR_graph_t *g) {
    if (!g || !g->entry) return 0;
    if (g->entry->op == IR_GOTO_DEFERRED) return 1;
    const IR_t *e = (g->entry->op == IR_DEFINE && IR_LIT(g->entry).ival == 3) ? g->entry : (const IR_t *)0;
    return !e ? 0 : (e->γ.node && e->γ.node->op == IR_GOTO_DEFERRED) ? 1 : 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_plan_procs(stage2_t * s2, int from_pi, int m4) {
    for (int pi = from_pi; pi < s2->proc_count; pi++) {
        ProcEntry * pe = &s2->proc_table[pi]; const char * pname = pe->name; int idx = pe->bb_idx;
        if (!pname || !strcmp(pname, "main") || idx < 0 || idx >= s2->bbp.count || !s2->bbp.table[idx] || !s2->bbp.table[idx]->entry) continue;
        IR_graph_t * g = s2->bbp.table[idx]; int is_lbl = strncmp(pname, "LBL__", 5) == 0; int bare = m4 && proc_role3_kind(g) == 1;
        emit_proc_prepare(s2, pi, 0, (int *) 0);
        const char * sym = m4 ? bb_ab_sym_name(pname) : pname; char * pfx = (char *) alloca(strlen(sym) + 6); snprintf(pfx, strlen(sym) + 6, "proc_%s", sym);
        if (!is_lbl && !bare) emit_chain_plan(pe->proc_entry_node ? pe->proc_entry_node : g->entry, pfx);
        emit_proc_unprepare();
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_plan_main(IR_graph_t * bbg, const char * prefix) {
    extern int g_flat_outer_nparams; IR_graph_t * cfg_sv = g_emit_cfg; int np_sv = g_flat_outer_nparams;
    g_emit_cfg = bbg; g_flat_outer_nparams = bbg->nparams; emit_chain_plan(bbg->entry, prefix); g_flat_outer_nparams = np_sv; g_emit_cfg = cfg_sv;
}
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
#ifdef __cplusplus
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void x86_asm_str_escape_c(const char * s, char * out, unsigned long outsz) {
    if (!out || !outsz) return;
    std::string e = x86_asm_str_escape(s ? s : "");
    unsigned long n = (unsigned long)e.size(); if (n >= outsz) n = outsz - 1;
    memcpy(out, e.c_str(), (size_t)n); out[n] = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" { int stage2_owner_varslot(const char * proc, const char * var); const char * stage2_owner_l3_ancestor(const char * proc); }
static int frame_display_mem_off(int lvl, const char * owner_proc) {
    if (lvl <= 3 || !owner_proc) return -1;
    const char * l3proc = stage2_owner_l3_ancestor(owner_proc);
    if (!l3proc) return -1;
    char nm[32]; snprintf(nm, sizeof nm, "__pas_display_%d", lvl);
    return stage2_owner_varslot(l3proc, nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void frame_display_prepare(IR_t *nd) {
    g_emit.op_seal = nd ? nd->seal : 0;
    g_emit.op_name1 = g_emit.op_seal == 1 ? "r13" : g_emit.op_seal == 2 ? "r14" : g_emit.op_seal == 3 ? "r15" : (const char *)0;
    g_emit.op_sa = (g_emit.op_a_sval && g_emit.op_sval) ? stage2_owner_varslot(g_emit.op_a_sval, g_emit.op_sval) : -1;
    g_emit.op_sb = g_emit.op_name1 ? -1 : frame_display_mem_off(g_emit.op_seal, g_emit.op_a_sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * kw_bare(const char * s) { return !s ? "" : (s[0] == '&' ? s + 1 : s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_defer_beta_guard(void) { if (g_emitter.sn4_defer_beta_guard__bg < 0) { const char * e = getenv("SCRIP_DEFER_BETA_GUARD"); g_emitter.sn4_defer_beta_guard__bg = (e && *e == '0') ? 0 : 1;
    } return g_emitter.sn4_defer_beta_guard__bg; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_define_fold(void) { if (g_emitter.sn4_define_fold__df < 0) { const char * e = getenv("SCRIP_DEFINE_FOLD"); g_emitter.sn4_define_fold__df = (e && *e == '0') ? 0 : 1;
    } return g_emitter.sn4_define_fold__df; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_descr_stamp(void) { if (g_emitter.sn4_descr_stamp_on < 0) { const char * e = getenv("SCRIP_DESCR_STAMP"); g_emitter.sn4_descr_stamp_on = (e && *e == '1') ? 1 : 0;
    } return g_emitter.sn4_descr_stamp_on; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_u2_fence(void) { if (g_emitter.sn4_u2_fence_v < 0) { const char * e = getenv("SCRIP_U2_FENCE"); g_emitter.sn4_u2_fence_v = (e && e[0] == '0') ? 0 : 1; } return g_emitter.sn4_u2_fence_v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int x86_zdp_on_c(void) { if (g_emitter.x86_zdp_on_c_v < 0) { const char * e = getenv("SCRIP_ZDP_TEARDOWN"); g_emitter.x86_zdp_on_c_v = (e && *e == '1') ? 1 : 0; } return g_emitter.x86_zdp_on_c_v; }
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int prolog_atom_intern_lit(const char *);
extern "C" int prolog_functor_arity(int);
void unify_prepare(IR_t *nd) {
    IR_t * src = (nd && nd->n_operands >= 2) ? nd->operands[0] : (IR_t *)0;
    IR_t * ix = (nd && nd->n_operands >= 2) ? nd->operands[1] : (IR_t *)0;
    IR_t * k = (nd && nd->n_operands > 2) ? nd->operands[2] : (IR_t *)0;
    const char * vn = nd ? IR_LIT(nd).sval : (const char *)0;
    int ok = 0, kid = 0, slot = -1; long idx = 0;
    if (nd && nd->n_operands >= 2 && src && ix && ix->op == IR_LIT_INTEGER && (long)IR_LIT(ix).ival >= 0) {
        idx = (long)IR_LIT(ix).ival;
        if (src->op == IR_VAR_REF) { slot = IR_LIT(src).sval ? bb_varslot_peek(IR_LIT(src).sval) : -1; ok = slot >= 0; }
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
        g_emit.op_u_vo = (ok && vn) ? bb_varslot_peek(vn) : -1;
        g_emit.op_u_why = !(ok && vn) ? 1 : g_emit.op_u_vo < 0 ? 4 : 0;
    }
}
void emit_text_s(const std::string & s) { if (!s.empty()) emit_text_n(s.data(), s.size()); }
void emit_write_file_s(const char * path, const std::string & s) { FILE * f = fopen(path, "w"); if (f) { fputs(s.c_str(), f); fclose(f); } }
extern "C" {
#include "builtin_ids.h"
#include "snobol4_system_fns.h"
DESCR_t rt_call_bid_sn4(const char *, DESCR_t *, int, int);
DESCR_t rt_call_name_sn4(const char *, DESCR_t *, int, int);
DESCR_t rt_call_arr_bl_strict(const char *, DESCR_t *, int, int);
DESCR_t rt_call_arr_bl_sn4(const char *, DESCR_t *, int, int);
DESCR_t rt_call_arr_bl(const char * fn, DESCR_t * args, int nargs, int bidlen);
DESCR_t rt_call_fld_sn4(DESCR_t * args, int nargs, sno_callee_rec_t * r);
DESCR_t rt_call_callee_sn4(DESCR_t * args, int nargs, sno_callee_rec_t * r);
int rt_dat_field_of_any(const char * name);
void * dat_find_type(const char * name);
}
int emit_knob_unless_zero(const char * name) { const char * e = getenv(name); return (e && *e == '0') ? 0 : 1; }
int emit_knob_nonzero(const char * name) { const char * e = getenv(name); return e ? (atoi(e) != 0) : 1; }
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
#include "templates/bb/bb_pl_leaf_kinds.h"
int elem_access_kind(const IR_t * nd) { return nd ? nd->elem_access : 0; }
extern "C" { int stage2_owner_varslot(const char *, const char *); int bb_scc_probe(const char *, int, int *, int *, int *, int *); int rt_proc_nformals(const char *); extern int g_flat_frame_floor; }
IR_t * bb_chain_terminal_staged(IR_t * entry);
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
int xa_flat_class_c(void) {
    if (!g_emit.flat_jmp_entry) { if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[CLASS-C] nid=%d bail=no_jmp_entry\n", g_emit.nid); return 0; }
    if (g_emit.flat_pat || g_emit.flat_gen || g_emit.flat_lcl_proc || g_emit.zframe_graph) {
        if (getenv("SCRIP_FLOOR_DIAG"))
            fprintf(stderr, "[CLASS-C] nid=%d bail=pat/gen/lcl/zframe pat=%d gen=%d lcl=%d zframe=%d\n", g_emit.nid, g_emit.flat_pat, g_emit.flat_gen, g_emit.flat_lcl_proc, g_emit.zframe_graph);
        return 0; }
    { extern int g_flat_frame_floor; if (g_flat_frame_floor > 0) {
        if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[CLASS-C] nid=%d bail=floor floor=%d\n", g_emit.nid, g_flat_frame_floor);
        return 0; } }
    { int _r = (g_emit.flat_frame_bytes >= 48) ? 1 : 0;
      if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[CLASS-C] nid=%d PASS frame_bytes=%d -> %d\n", g_emit.nid, g_emit.flat_frame_bytes, _r);
      return _r; }
}
int xa_flat_sig_names(const char * fname, int * nf_out, int * nsave_out, int * gk_out, int * res_gk_out) {
    if (!fname || !fname[0] || g_runtime.eval.rt_fragment_emit) return 0;
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
int bcps_arg_slot(IR_t * call, IR_graph_t ** argblks, int i) {
    IR_t * a = ir_call_arg(call, i);
    if (a) {
    int s = bb_slot_get(a);
    if (s < 0) s = zls_off(a);
    if (s >= 0) return s;
}
    IR_t * prod = bb_chain_terminal_staged(argblks && argblks[i] ? argblks[i]->entry : NULL); int s = prod ? bb_slot_get(prod) : -1; return s < 0 ? 0 : s;
}
IR_t * bb_chain_terminal_staged(IR_t * entry) {
    IR_t * n = entry; int guard = 0;
    while (n && n->γ.node && n->γ.node->op != IR_SUCCEED && n->γ.node->op != IR_FAIL && guard++ < 4096) n = n->γ.node;
    return n;
}
int bcps_result_slot() {
    IR_t * nd = _.node;
    { int _s = nd ? zls_off(nd) : -1; if (_s >= 0) { if (bb_slot_get(nd) < 0) bb_slot_register(nd, _s); return _s; } }
    return -1;
}
int bcps_beta_pair_idx() {
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
        if (XA_PAIR(i).define == _.lbl_β_p && XA_PAIR(i).jmp) return i;
    return -1;
}
void bcps_sig_tally(const char * arm, const char * fn, long n, int ok, const char * why, const char * opnd) {
     if (g_emitter.bcps_sig_tally__sd < 0) {
    const char * _e = getenv("SCRIP_SIG_DIAG");
    g_emitter.bcps_sig_tally__sd = (_e && *_e == '1') ? 1 : 0;
}
    if (!g_emitter.bcps_sig_tally__sd) return;
    fprintf(stderr, "[SIG] arm=%s fn=%s nargs=%ld verdict=%s why=%s opnd=%s\n", arm, fn ? fn : "?", n, ok ? "SIG" : "DECLINE", why, (opnd && *opnd) ? opnd : "-");
}
int bcfn_result_slot(IR_t * nd) {
    { int _s = nd ? zls_off(nd) : -1; if (_s >= 0) { if (bb_slot_get(nd) < 0) bb_slot_register(nd, _s); return _s; } }
    return -1;
}
extern "C" int xa_flat_class_c_pred(void) { return xa_flat_class_c(); }
extern "C" {
DESCR_t rt_pl_throw_raise(DESCR_t *, int); DESCR_t rt_pl_exist_raise(DESCR_t *, int); DESCR_t rt_pl_catch_handle(DESCR_t *, int); DESCR_t rt_pl_dop_clause_unify(DESCR_t *,
    int); DESCR_t dop_write(DESCR_t *, int); DESCR_t dop_nl(DESCR_t *, int); DESCR_t rt_fp_model_spitbol(DESCR_t *, int); DESCR_t rt_sno_wantnm_d(DESCR_t *,
        int); DESCR_t rt_sno_chknm_d(DESCR_t *, int); DESCR_t rt_sno_patck_d(DESCR_t *, int); DESCR_t rt_sno_mkpat_d(DESCR_t *, int);
DESCR_t rt_sno_name_dl(DESCR_t *, int); DESCR_t rt_sno_pbk_d(DESCR_t *, int); DESCR_t rt_sno_pbn_d(DESCR_t *, int); DESCR_t rt_sno_pb0_d(DESCR_t *, int); DESCR_t rt_sno_pbc_d(DESCR_t *,
    int); DESCR_t rt_sno_pcur_d(DESCR_t *, int); DESCR_t rt_sno_pbalt_d(DESCR_t *, int); DESCR_t rt_sno_parb_d(DESCR_t *, int); DESCR_t rt_sno_pfen_d(DESCR_t *,
        int); DESCR_t rt_sno_pdef_d(DESCR_t *, int); DESCR_t rt_sno_kwset_d(DESCR_t *, int); DESCR_t rt_sno_mkexpr_d(DESCR_t *, int); DESCR_t rt_sno_xname_d(DESCR_t *, int);
            DESCR_t rt_sno_bynmark_d(DESCR_t *, int);
            DESCR_t rt_sno_xthunk_d(DESCR_t *, int);
            DESCR_t rt_sno_xlv_d(DESCR_t *, int);
            DESCR_t rt_sno_stmt_d(DESCR_t *,
            int); DESCR_t rt_sno_nofail_d(DESCR_t *, int); DESCR_t rt_sno_list_d(DESCR_t *, int);
DESCR_t rt_pl_dop_skip_list(DESCR_t *, int); DESCR_t rt_pl_dop_argv(DESCR_t *, int);
DESCR_t rt_pl_dop_compare(DESCR_t *, int); DESCR_t rt_pl_dop_functor(DESCR_t *, int); DESCR_t rt_pl_dop_arg(DESCR_t *, int); DESCR_t rt_pl_dop_univ(DESCR_t *, int);
DESCR_t rt_pl_dop_attv_set(DESCR_t *, int); DESCR_t rt_pl_dop_setarg(DESCR_t *, int); DESCR_t rt_pl_dop_nb_setarg(DESCR_t *, int); DESCR_t rt_pl_dop_nb_linkarg(DESCR_t *, int);
DESCR_t rt_pl_dop_stamp_date_time(DESCR_t *, int); DESCR_t rt_pl_dop_date_time_stamp(DESCR_t *, int); DESCR_t rt_pl_dop_format_time_text(DESCR_t *, int);
    DESCR_t rt_pl_dop_create_prolog_flag(DESCR_t *, int);
    DESCR_t rt_pl_dop_attv_atts(DESCR_t *, int); DESCR_t rt_pl_dop_attv_take(DESCR_t *, int); DESCR_t rt_pl_dop_unifiable(DESCR_t *, int);
    DESCR_t rt_pl_dop_crv_enter(DESCR_t *, int); DESCR_t rt_pl_dop_crv_adj(DESCR_t *, int); DESCR_t rt_pl_dop_crv_collect(DESCR_t *, int);
    DESCR_t rt_pl_dop_copy_keep(DESCR_t *, int); DESCR_t rt_pl_dop_factorize_term(DESCR_t *, int);
    DESCR_t rt_pl_dop_pl_backtrace(DESCR_t *, int);
DESCR_t rt_pl_dop_scc_bnew(DESCR_t *, int); DESCR_t rt_pl_dop_scc_take(DESCR_t *, int); DESCR_t rt_pl_dop_copy_term(DESCR_t *, int); DESCR_t rt_pl_dop_copy_term_nat(DESCR_t *, int);
    DESCR_t rt_pl_dop_term_variables(DESCR_t *, int);
    DESCR_t rt_pl_dop_numbervars3(DESCR_t *, int); DESCR_t rt_pl_dop_numbervars1(DESCR_t *,
    int); DESCR_t rt_pl_dop_succ(DESCR_t *, int);
DESCR_t rt_pl_dop_wall_us(DESCR_t *, int); DESCR_t rt_pl_dop_wall_ms(DESCR_t *, int);
DESCR_t rt_pl_dop_plus(DESCR_t *, int); DESCR_t rt_pl_dop_sort(DESCR_t *, int); DESCR_t rt_pl_dop_msort(DESCR_t *, int); DESCR_t rt_pl_dop_char_type(DESCR_t *, int);
DESCR_t rt_pl_dop_findall_new(DESCR_t *, int); DESCR_t rt_pl_dop_findall_add(DESCR_t *, int); DESCR_t rt_pl_dop_findall_result(DESCR_t *, int); DESCR_t rt_pl_dop_findall_result4(DESCR_t *,
    int); DESCR_t rt_pl_dop_bagof_result(DESCR_t *, int); DESCR_t rt_pl_dop_setof_result(DESCR_t *, int);
DESCR_t rt_pl_dop_sub_atom_n(DESCR_t *, int); DESCR_t rt_pl_dop_sub_atom_at(DESCR_t *, int); DESCR_t rt_pl_dop_atom_concat_n(DESCR_t *, int); DESCR_t rt_pl_dop_atom_concat_at(DESCR_t *,
    int); DESCR_t rt_pl_dop_bagof_group_n(DESCR_t *, int); DESCR_t rt_pl_dop_bagof_group_at(DESCR_t *, int); DESCR_t rt_pl_dop_setof_group_at(DESCR_t *, int);
DESCR_t rt_pl_dop_big(DESCR_t *, int); DESCR_t rt_pl_dop_db_assertz(DESCR_t *, int); DESCR_t rt_pl_dop_db_asserta(DESCR_t *, int); DESCR_t rt_pl_dop_db_erase(DESCR_t *,
    int); DESCR_t rt_pl_dop_db_abolish(DESCR_t *, int); DESCR_t rt_pl_dop_db_nonempty(DESCR_t *, int); DESCR_t rt_pl_dop_ball_pending(DESCR_t *, int); DESCR_t rt_pl_dop_format(DESCR_t *,
        int); DESCR_t rt_pl_dop_db_seed_once(DESCR_t *, int);
DESCR_t rt_pl_dop_nb_setval(DESCR_t *, int); DESCR_t rt_pl_dop_nb_getval(DESCR_t *, int); DESCR_t rt_pl_dop_b_setval(DESCR_t *, int);
DESCR_t rt_pl_dop_trie_new(DESCR_t *, int); DESCR_t rt_pl_dop_trie_alive(DESCR_t *, int); DESCR_t rt_pl_dop_trie_destroy(DESCR_t *, int); DESCR_t rt_pl_dop_trie_insert(DESCR_t *, int);
DESCR_t rt_pl_dop_trie_lookup(DESCR_t *, int); DESCR_t rt_pl_dop_trie_delete(DESCR_t *, int); DESCR_t rt_pl_dop_trie_entries(DESCR_t *, int); DESCR_t rt_pl_dop_trie_entry_key(DESCR_t *, int);
DESCR_t rt_pl_dop_trie_count(DESCR_t *, int); DESCR_t rt_pl_dop_trie_meta_set(DESCR_t *, int); DESCR_t rt_pl_dop_trie_meta_get(DESCR_t *, int); DESCR_t rt_pl_dop_tbl_vt(DESCR_t *, int);
DESCR_t rt_pl_dop_tbh_close(DESCR_t *, int); DESCR_t rt_pl_dop_tbh_leading(DESCR_t *, int); DESCR_t rt_pl_dop_tbh_get(DESCR_t *, int); DESCR_t rt_pl_dop_tbh_own(DESCR_t *, int);
DESCR_t rt_pl_dop_tbh_set(DESCR_t *, int); DESCR_t rt_pl_dop_tbh_bset(DESCR_t *, int); DESCR_t rt_pl_dop_tbl_dlget(DESCR_t *, int); DESCR_t rt_pl_dop_tbl_dlset(DESCR_t *, int);
    DESCR_t rt_pl_dop_tx_db(DESCR_t *, int); DESCR_t rt_pl_dop_mod_ext(DESCR_t *, int);
DESCR_t rt_pl_dop_trie_slice(DESCR_t *, int);
DESCR_t rt_pl_dop_cpu_ms(DESCR_t *, int);
DESCR_t rt_pl_dop_db_retractall(DESCR_t *, int);
DESCR_t rt_pl_dop_db_alive(DESCR_t *, int);
DESCR_t rt_pl_dop_db_t_guard(DESCR_t *, int); DESCR_t rt_pl_dop_db_assertz_t(DESCR_t *, int); DESCR_t rt_pl_dop_db_asserta_t(DESCR_t *, int); DESCR_t rt_pl_dop_db_erase_t(DESCR_t *,
    int); DESCR_t rt_pl_dop_db_abolish_t(DESCR_t *, int); DESCR_t rt_pl_dop_db_retractall_t(DESCR_t *, int); DESCR_t rt_pl_dop_db_asserta_r(DESCR_t *,
        int); DESCR_t rt_pl_dop_db_assertz_r(DESCR_t *, int); DESCR_t rt_pl_dop_db_erase_ref(DESCR_t *, int); DESCR_t rt_pl_dop_db_n_r(DESCR_t *, int); DESCR_t rt_pl_dop_db_at_r(DESCR_t *,
            int); DESCR_t rt_pl_dop_db_ref_r(DESCR_t *, int);
DESCR_t rt_pl_dop_co_start(DESCR_t *, int); DESCR_t rt_pl_dop_co_resume(DESCR_t *, int); DESCR_t rt_pl_dop_co_yield(DESCR_t *, int); DESCR_t rt_pl_dop_co_unwound(DESCR_t *, int);
DESCR_t rt_pl_dop_co_finish(DESCR_t *, int); DESCR_t rt_pl_dop_co_mark(DESCR_t *, int); DESCR_t rt_pl_dop_co_inside(DESCR_t *, int);
DESCR_t rt_pl_dop_engine_new(DESCR_t *, int); DESCR_t rt_pl_dop_engine_next(DESCR_t *, int); DESCR_t rt_pl_dop_engine_yield(DESCR_t *, int); DESCR_t rt_pl_dop_engine_destroy(DESCR_t *, int);
DESCR_t rt_pl_dop_engine_self(DESCR_t *, int);
DESCR_t rt_pl_dop_db_store_k(DESCR_t *, int); DESCR_t rt_pl_dop_db_copy(DESCR_t *, int); DESCR_t rt_pl_dop_pl_cp_count(DESCR_t *, int); DESCR_t rt_pl_dop_pl_cp_nth(DESCR_t *,
    int); DESCR_t rt_pl_dop_pl_cp_guard(DESCR_t *, int); DESCR_t rt_pl_dop_pl_pp_guard(DESCR_t *, int); DESCR_t rt_pl_dop_pl_pp_count(DESCR_t *, int); DESCR_t rt_pl_dop_pl_pp_nth(DESCR_t *,
        int); DESCR_t rt_pl_dop_halt(DESCR_t *, int);
DESCR_t rt_pl_dop_ax_eguard(DESCR_t *, int);
DESCR_t pas_arr_get(DESCR_t *, int); DESCR_t pas_arr_set(DESCR_t *, int);
DESCR_t pas_elem_ref(DESCR_t *, int); DESCR_t pas_ref_val(DESCR_t *, int);
DESCR_t pas_view_make(DESCR_t *, int);
DESCR_t rt_quit_trap_300(DESCR_t *, int); DESCR_t rt_quit_trap_320(DESCR_t *, int);
DESCR_t rt_pl_dop_char_guard(DESCR_t *, int); DESCR_t rt_pl_dop_between_guard(DESCR_t *, int); DESCR_t rt_pl_dop_stream_guard(DESCR_t *, int); DESCR_t rt_pl_dop_curstream_guard(DESCR_t *, int);
DESCR_t rt_pl_dop_nb_getval_guard(DESCR_t *, int);
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
    DESCR_t rt_pl_dop_gnu_stream_line_column(DESCR_t *, int);
DESCR_t rt_pl_dop_gnu_last_read_start(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_absolute_file_name(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_prolog_file_name(DESCR_t *, int);
DESCR_t rt_pl_dop_gnu_builtin(DESCR_t *, int);
DESCR_t rt_pl_dop_gnu_working_directory(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_change_directory(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_make_directory(DESCR_t *, int);
    DESCR_t rt_pl_dop_gnu_os(DESCR_t *, int);
    DESCR_t rt_pl_dop_compound0(DESCR_t *, int);
    DESCR_t rt_pl_dop_set_stream(DESCR_t *, int);
DESCR_t rt_pl_dop_gnu_delete_file(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_file_exists(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_directory_files(DESCR_t *, int);
DESCR_t rt_pl_dop_gnu_environ_list(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_file_props(DESCR_t *, int); DESCR_t rt_pl_dop_gnu_term_hash(DESCR_t *, int);
DESCR_t rt_pl_dop_gnu_prolog_pid(DESCR_t *, int);
DESCR_t rt_pl_dop_pipe(DESCR_t *, int); DESCR_t rt_pl_dop_open_null_stream(DESCR_t *, int);
DESCR_t rt_pl_dop_number_chars(DESCR_t *, int); DESCR_t rt_pl_dop_name(DESCR_t *, int); DESCR_t rt_pl_dop_get_char(DESCR_t *, int); DESCR_t rt_pl_dop_peek_char(DESCR_t *, int);
DESCR_t rt_pl_dop_read(DESCR_t *, int); DESCR_t rt_pl_dop_atom_to_term(DESCR_t *, int); DESCR_t rt_pl_dop_read_term_from_atom(DESCR_t *,
    int); DESCR_t rt_pl_dop_read_term_from_chars(DESCR_t *, int); DESCR_t rt_pl_dop_read_term_from_codes(DESCR_t *, int);DESCR_t rt_pl_dop_display(DESCR_t *,
        int); DESCR_t rt_pl_dop_display_s(DESCR_t *, int); DESCR_t rt_pl_dop_unify_oc(DESCR_t *, int); DESCR_t rt_pl_dop_aggregate_reduce(DESCR_t *, int);
DESCR_t rt_pl_dop_read_term_opts(DESCR_t *, int); DESCR_t rt_pl_dop_read_term_opts_s(DESCR_t *, int);
DESCR_t rt_pl_dop_wot_open(DESCR_t *, int); DESCR_t rt_pl_dop_wot_capture(DESCR_t *, int); DESCR_t rt_pl_dop_wot_discard(DESCR_t *, int);
DESCR_t rt_pl_dop_set_prolog_flag_declare(DESCR_t *, int);
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
DESCR_t rt_pl_dop_put_edin(DESCR_t *, int);
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
                                int); DESCR_t rt_pl_dop_shc(DESCR_t *, int); DESCR_t rt_pl_dop_pl_sp_nth(DESCR_t *, int); DESCR_t rt_pl_dop_pl_cs_count(DESCR_t *, int);
                                    DESCR_t rt_pl_dop_pl_cs_nth(DESCR_t *, int);
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
DESCR_t rt_pas_resundef(DESCR_t *, int); DESCR_t rt_pas_sin(DESCR_t *, int); DESCR_t rt_pas_cos(DESCR_t *, int); DESCR_t rt_pas_exp(DESCR_t *, int); DESCR_t rt_pas_sqrt(DESCR_t *, int);
DESCR_t rt_pas_ln(DESCR_t *, int); DESCR_t rt_pas_arctan(DESCR_t *, int); DESCR_t rt_pas_sqr(DESCR_t *, int);
DESCR_t rt_rk_arr_at(DESCR_t *, int); DESCR_t rt_rk_arr_set(DESCR_t *, int); DESCR_t rt_rk_elem_put(DESCR_t *, int);
DESCR_t rt_rk_param_check(DESCR_t *, int); DESCR_t rt_rk_assign_check(DESCR_t *, int); DESCR_t rt_rk_bindattr(DESCR_t *, int); DESCR_t rt_rk_box(DESCR_t *, int); DESCR_t rt_rk_blk_ref(DESCR_t *, int);
DESCR_t rt_rk_undef(DESCR_t *, int); DESCR_t rt_rk_range_arr(DESCR_t *, int); DESCR_t rt_rk_iter_src(DESCR_t *, int); DESCR_t rt_rk_map_append(DESCR_t *, int);
    DESCR_t rt_rk_grep_append(DESCR_t *, int);
DESCR_t rt_rk_byref_assign(DESCR_t *, int);
}
void * dop_direct_fp(const char * fn, int64_t narg, const char ** sym) {
    static const struct { char nm[34]; int ar; char sy[34]; DESCR_t (*fp)(DESCR_t *, int); } t[] = {
        { "$clause_unify", 2, "rt_pl_dop_clause_unify", rt_pl_dop_clause_unify },
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
        { "$skip_list", 3, "rt_pl_dop_skip_list", rt_pl_dop_skip_list }, { "$argv", 1, "rt_pl_dop_argv", rt_pl_dop_argv },
        { "$univ", 2, "rt_pl_dop_univ", rt_pl_dop_univ }, { "$copy_term", 2, "rt_pl_dop_copy_term", rt_pl_dop_copy_term },
        { "$copy_term_nat", 2, "rt_pl_dop_copy_term_nat", rt_pl_dop_copy_term_nat },
        { "$term_variables", 2, "rt_pl_dop_term_variables", rt_pl_dop_term_variables }, { "$numbervars3", 3, "rt_pl_dop_numbervars3", rt_pl_dop_numbervars3 },
        { "$numbervars1", 1, "rt_pl_dop_numbervars1", rt_pl_dop_numbervars1 }, { "$scc_bnew", 1, "rt_pl_dop_scc_bnew", rt_pl_dop_scc_bnew }, { "$scc_take", 2, "rt_pl_dop_scc_take",
            rt_pl_dop_scc_take }, { "$succ", 2, "rt_pl_dop_succ", rt_pl_dop_succ }, { "$plus", 3, "rt_pl_dop_plus", rt_pl_dop_plus },
        { "$attv_set", 2, "rt_pl_dop_attv_set", rt_pl_dop_attv_set }, { "$setarg", 3, "rt_pl_dop_setarg", rt_pl_dop_setarg }, { "$nb_setarg", 3, "rt_pl_dop_nb_setarg", rt_pl_dop_nb_setarg },
            { "$nb_linkarg", 3, "rt_pl_dop_nb_linkarg", rt_pl_dop_nb_linkarg },
            { "$stamp_date_time", 3, "rt_pl_dop_stamp_date_time", rt_pl_dop_stamp_date_time }, { "$date_time_stamp", 2, "rt_pl_dop_date_time_stamp", rt_pl_dop_date_time_stamp },
            { "$format_time_text", 3, "rt_pl_dop_format_time_text", rt_pl_dop_format_time_text },
        { "$create_prolog_flag", 2, "rt_pl_dop_create_prolog_flag", rt_pl_dop_create_prolog_flag }, { "$attv_atts", 2, "rt_pl_dop_attv_atts", rt_pl_dop_attv_atts },
        { "$attv_take", 1, "rt_pl_dop_attv_take", rt_pl_dop_attv_take }, { "$unifiable", 3, "rt_pl_dop_unifiable", rt_pl_dop_unifiable },
        { "$crv_enter", 1, "rt_pl_dop_crv_enter", rt_pl_dop_crv_enter }, { "$crv_adj", 1, "rt_pl_dop_crv_adj", rt_pl_dop_crv_adj },
        { "$crv_collect", 2, "rt_pl_dop_crv_collect", rt_pl_dop_crv_collect },
        { "$copy_keep", 4, "rt_pl_dop_copy_keep", rt_pl_dop_copy_keep }, { "$pl_backtrace", 2, "rt_pl_dop_pl_backtrace", rt_pl_dop_pl_backtrace }, { "$factorize_term", 3, "rt_pl_dop_factorize_term",
            rt_pl_dop_factorize_term },
        { "$wall_us", 1, "rt_pl_dop_wall_us", rt_pl_dop_wall_us }, { "$wall_ms", 1, "rt_pl_dop_wall_ms", rt_pl_dop_wall_ms },
        { "$sort", 2, "rt_pl_dop_sort", rt_pl_dop_sort }, { "$msort", 2, "rt_pl_dop_msort", rt_pl_dop_msort }, { "$char_type", 2, "rt_pl_dop_char_type", rt_pl_dop_char_type },
        { "$pl_big", 1, "rt_pl_dop_big", rt_pl_dop_big }, { "$findall_new", 0, "rt_pl_dop_findall_new", rt_pl_dop_findall_new }, { "$findall_add", 2, "rt_pl_dop_findall_add", rt_pl_dop_findall_add },
        { "$findall_result", 2, "rt_pl_dop_findall_result", rt_pl_dop_findall_result }, { "$findall_result4", 3, "rt_pl_dop_findall_result4", rt_pl_dop_findall_result4 }, { "$bagof_result",
            2, "rt_pl_dop_bagof_result", rt_pl_dop_bagof_result },
        { "$setof_result", 2, "rt_pl_dop_setof_result", rt_pl_dop_setof_result },
        { "$sub_atom_n", 5, "rt_pl_dop_sub_atom_n", rt_pl_dop_sub_atom_n }, { "$sub_atom_at", 6, "rt_pl_dop_sub_atom_at", rt_pl_dop_sub_atom_at },
        { "$atom_concat_n", 3, "rt_pl_dop_atom_concat_n", rt_pl_dop_atom_concat_n }, { "$atom_concat_at", 4, "rt_pl_dop_atom_concat_at", rt_pl_dop_atom_concat_at },
        { "$bagof_group_n", 1, "rt_pl_dop_bagof_group_n", rt_pl_dop_bagof_group_n }, { "$bagof_group_at", 4, "rt_pl_dop_bagof_group_at", rt_pl_dop_bagof_group_at }, { "$setof_group_at", 4,
            "rt_pl_dop_setof_group_at", rt_pl_dop_setof_group_at },
        { "$db_assertz", 2, "rt_pl_dop_db_assertz", rt_pl_dop_db_assertz }, { "$db_asserta", 2, "rt_pl_dop_db_asserta", rt_pl_dop_db_asserta },
        { "$db_erase", 2, "rt_pl_dop_db_erase", rt_pl_dop_db_erase }, { "$db_abolish", 1, "rt_pl_dop_db_abolish", rt_pl_dop_db_abolish }, { "$db_nonempty", 1, "rt_pl_dop_db_nonempty",
            rt_pl_dop_db_nonempty }, { "$ball_pending", 0, "rt_pl_dop_ball_pending", rt_pl_dop_ball_pending }, { "$db_seed_once", 3, "rt_pl_dop_db_seed_once", rt_pl_dop_db_seed_once },
        { "$db_retractall", 2, "rt_pl_dop_db_retractall", rt_pl_dop_db_retractall },
        { "$db_alive", 2, "rt_pl_dop_db_alive", rt_pl_dop_db_alive },
        { "$co_start", 4, "rt_pl_dop_co_start", rt_pl_dop_co_start }, { "$co_resume", 5, "rt_pl_dop_co_resume", rt_pl_dop_co_resume },
        { "$co_yield", 2, "rt_pl_dop_co_yield", rt_pl_dop_co_yield }, { "$co_unwound", 2, "rt_pl_dop_co_unwound", rt_pl_dop_co_unwound },
        { "$co_finish", 2, "rt_pl_dop_co_finish", rt_pl_dop_co_finish }, { "$co_mark", 0, "rt_pl_dop_co_mark", rt_pl_dop_co_mark },
        { "$co_inside", 0, "rt_pl_dop_co_inside", rt_pl_dop_co_inside },
        { "$engine_new", 6, "rt_pl_dop_engine_new", rt_pl_dop_engine_new }, { "$engine_next", 2, "rt_pl_dop_engine_next", rt_pl_dop_engine_next },
        { "$engine_yield", 1, "rt_pl_dop_engine_yield", rt_pl_dop_engine_yield }, { "$engine_destroy", 3, "rt_pl_dop_engine_destroy", rt_pl_dop_engine_destroy },
        { "$engine_self", 1, "rt_pl_dop_engine_self", rt_pl_dop_engine_self },
        { "$db_store_k", 1, "rt_pl_dop_db_store_k", rt_pl_dop_db_store_k }, { "$db_copy", 2, "rt_pl_dop_db_copy", rt_pl_dop_db_copy }, { "$pl_cp_count", 1, "rt_pl_dop_pl_cp_count",
            rt_pl_dop_pl_cp_count }, { "$pl_cp_nth", 3, "rt_pl_dop_pl_cp_nth", rt_pl_dop_pl_cp_nth }, { "$pl_pp_guard", 1, "rt_pl_dop_pl_pp_guard", rt_pl_dop_pl_pp_guard }, { "$pl_pp_count",
                2, "rt_pl_dop_pl_pp_count", rt_pl_dop_pl_pp_count }, { "$pl_pp_nth", 3, "rt_pl_dop_pl_pp_nth", rt_pl_dop_pl_pp_nth }, { "$pl_cp_guard", 1, "rt_pl_dop_pl_cp_guard",
                    rt_pl_dop_pl_cp_guard },
        { "$db_t_guard", 2, "rt_pl_dop_db_t_guard", rt_pl_dop_db_t_guard }, { "$db_assertz_t", 1, "rt_pl_dop_db_assertz_t", rt_pl_dop_db_assertz_t }, { "$db_asserta_t", 1,
            "rt_pl_dop_db_asserta_t", rt_pl_dop_db_asserta_t },
        { "$db_erase_t", 2, "rt_pl_dop_db_erase_t", rt_pl_dop_db_erase_t }, { "$db_abolish_t", 1, "rt_pl_dop_db_abolish_t", rt_pl_dop_db_abolish_t }, { "$db_retractall_t", 1,
            "rt_pl_dop_db_retractall_t", rt_pl_dop_db_retractall_t }, { "$db_asserta_r", 2, "rt_pl_dop_db_asserta_r", rt_pl_dop_db_asserta_r }, { "$db_assertz_r", 2, "rt_pl_dop_db_assertz_r",
                rt_pl_dop_db_assertz_r }, { "$db_erase_ref", 1, "rt_pl_dop_db_erase_ref", rt_pl_dop_db_erase_ref }, { "$db_n_r", 2, "rt_pl_dop_db_n_r", rt_pl_dop_db_n_r }, { "$db_at_r", 3,
                    "rt_pl_dop_db_at_r", rt_pl_dop_db_at_r }, { "$db_ref_r", 3, "rt_pl_dop_db_ref_r", rt_pl_dop_db_ref_r },
        { "$nb_setval", 2, "rt_pl_dop_nb_setval", rt_pl_dop_nb_setval }, { "$nb_getval", 2, "rt_pl_dop_nb_getval", rt_pl_dop_nb_getval }, { "$b_setval", 2, "rt_pl_dop_b_setval", rt_pl_dop_b_setval },
        { "$trie_new", 1, "rt_pl_dop_trie_new", rt_pl_dop_trie_new }, { "$trie_alive", 1, "rt_pl_dop_trie_alive", rt_pl_dop_trie_alive },
        { "$trie_destroy", 1, "rt_pl_dop_trie_destroy", rt_pl_dop_trie_destroy }, { "$trie_insert", 5, "rt_pl_dop_trie_insert", rt_pl_dop_trie_insert },
        { "$trie_lookup", 3, "rt_pl_dop_trie_lookup", rt_pl_dop_trie_lookup }, { "$trie_delete", 3, "rt_pl_dop_trie_delete", rt_pl_dop_trie_delete },
        { "$trie_entries", 2, "rt_pl_dop_trie_entries", rt_pl_dop_trie_entries }, { "$trie_entry_key", 3, "rt_pl_dop_trie_entry_key", rt_pl_dop_trie_entry_key },
        { "$trie_count", 2, "rt_pl_dop_trie_count", rt_pl_dop_trie_count }, { "$trie_meta_set", 3, "rt_pl_dop_trie_meta_set", rt_pl_dop_trie_meta_set },
        { "$trie_meta_get", 3, "rt_pl_dop_trie_meta_get", rt_pl_dop_trie_meta_get }, { "$tbl_vt", 1, "rt_pl_dop_tbl_vt", rt_pl_dop_tbl_vt },
        { "$tbh_close", 0, "rt_pl_dop_tbh_close", rt_pl_dop_tbh_close }, { "$tbh_leading", 0, "rt_pl_dop_tbh_leading", rt_pl_dop_tbh_leading },
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
                        "rt_pl_dop_pl_ioarg", rt_pl_dop_pl_ioarg }, { "$cutcall", 2, "rt_pl_dop_cutcall", rt_pl_dop_cutcall }, { "$shc", 1, "rt_pl_dop_shc", rt_pl_dop_shc },
        { "$print", 1, "rt_pl_dop_print", rt_pl_dop_print }, { "$print_s", 2, "rt_pl_dop_print_s", rt_pl_dop_print_s },
        { "$pl_op_count", 1, "rt_pl_dop_pl_op_count", rt_pl_dop_pl_op_count }, { "$pl_op_nth", 4, "rt_pl_dop_pl_op_nth", rt_pl_dop_pl_op_nth },
        { "$pl_sp_count", 1, "rt_pl_dop_pl_sp_count", rt_pl_dop_pl_sp_count }, { "$pl_sp_nth", 3, "rt_pl_dop_pl_sp_nth", rt_pl_dop_pl_sp_nth },
        { "$pl_cs_count", 1, "rt_pl_dop_pl_cs_count", rt_pl_dop_pl_cs_count }, { "$pl_cs_nth", 4, "rt_pl_dop_pl_cs_nth", rt_pl_dop_pl_cs_nth },
        { "arr_get", 2, "pas_arr_get", pas_arr_get }, { "arr_set_pure", 3, "pas_arr_set", pas_arr_set },
        { "__pas_elem_ref", 3, "pas_elem_ref", pas_elem_ref }, { "__pas_ref_val", 1, "pas_ref_val", pas_ref_val },
        { "__pas_view", 4, "pas_view_make", pas_view_make },
        { "__pas_resundef", 1, "rt_pas_resundef", rt_pas_resundef }, { "__pas_sin", 1, "rt_pas_sin", rt_pas_sin }, { "__pas_cos", 1, "rt_pas_cos", rt_pas_cos },
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
extern "C" int dop_direct_leaf_known(const char * fn, int narg) {
    const char * s = 0;
    return (dop_direct_fp(fn, (int64_t)narg, &s) || pl_leaf_inline_known(fn, narg)) ? 1 : 0;
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
#include "rt/rt_pl_trail.h"
static_assert(X86_PL_TR_ENTRY_BYTES == PL_TR_ENTRY_BYTES, "x86_pl_tr_pop_head spells the trail entry size as a literal");
static_assert(X86_PL_TR_ARENA_MASK == -(long)PL_TR_ARENA_BYTES, "x86_pl_tr_top_sync spells the trail arena mask as a literal");
static_assert(X86_PL_TR_WAKE_OFF == PL_TR_WAKE_OFF && X86_PL_TR_RESIDUE_OFF == PL_TR_RESIDUE_OFF, "x86_pl_tr_tidy spells the wake and residue words as literals");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zls_argv_off(const IR_t *);
extern "C" int zls_redisp_off(const IR_t *);
extern "C" int pl_db_decls_words(long long ** out);
extern "C" { typedef struct { long fn; long how; } call_next_rec_t; call_next_rec_t rt_call_arr_bl_try(const char *, DESCR_t *, long, DESCR_t *); call_next_rec_t rt_call_arr_bl_sn4_try(const char *,
    DESCR_t *, long, DESCR_t *); call_next_rec_t rk_callsame_open(DESCR_t *, int, void *); call_next_rec_t rk_callwith_open(DESCR_t *, int, void *); }
static void call_margs_reserve(int n) { cv_reserve(&g_emit.op_marg, (uint32_t)sizeof(marg_t), (uint64_t)(n > 0 ? n : 1), "op_marg"); }
static const char * call_cv_text(cv_t * buf, const std::string & s) { cv_reserve(buf, 1, (uint64_t)s.size() + 1, "call_cv_text"); memcpy(buf->p, s.c_str(), s.size() + 1); return (const char *)buf->p;
    }
static void call_marg_plan(int i, IR_t * lf, int aoff, IR_t * owner) {
    marg_t * m = &((marg_t *)g_emit.op_marg.p)[i]; m->kind = MARG_NONE; m->src = -1; m->imm = 0; m->str = 0;
    if (owner && owner == g_emit.node && i >= 0 && i < g_emit.op_arg_slot_n && g_emit.op_arg_slot[i] >= 0) { m->src = g_emit.op_arg_slot[i]; m->kind = m->src == aoff ? MARG_DIRECT : MARG_SLOT; return;
        }
    if (!lf) return;
    { const char * sv = IR_LIT(lf).sval;
      if (lf->op == IR_LIT_INTEGER) { m->kind = MARG_LIT_I; m->imm = (uint64_t)IR_LIT(lf).ival; return; }
      if (lf->op == IR_LIT_REAL) { m->kind = MARG_LIT_F; m->imm = emit_double_bits(IR_LIT(lf).dval); return; }
      if (lf->op == IR_LIT_ATOM) { m->kind = MARG_LIT_ATOM; m->imm = (uint64_t)(unsigned)prolog_atom_intern_lit(sv ? sv : ""); return; }
      if (lf->op == IR_LIT_STRING && lf->seal == IR_SEAL_DSTAR_REF) { std::string star = std::string("*") + (sv ? sv : ""); m->kind = MARG_DSTAR;
          m->str = ct_strdup((std::string(".Ldstar_") + bb_ab_sym_name(star.c_str())).c_str()); m->imm = (uint64_t)(uintptr_t)bb_dstar_rec_addr(star.c_str()); return; }
      if (lf->op == IR_LIT_STRING && lf->seal == IR_SEAL_THUNK_REF) { m->kind = MARG_THUNK; m->str = ct_strdup((std::string(".Lthk_") + bb_ab_sym_name(sv ? sv : "")).c_str());
          m->imm = (uint64_t)(uintptr_t)bb_thunk_rec_addr(sv); return; }
      if (lf->op == IR_LIT_STRING) { m->kind = MARG_LIT_S; m->str = sv ? sv : ""; return; }
      if ((ir_kind0(lf) == IR_CALL && (IR_LIT(lf).dval == 2.0 || IR_LIT(lf).dval == 3.0)) || ir_is_call_kind(ir_kind0(lf))) {
          int staged = (ir_kind0(lf) == IR_CALL_PROC_STAGED || lf->op == IR_PROC_GEN);
          if (owner && owner == g_emit.node && staged && bb_slot_get(lf) >= 0) { m->kind = MARG_SLOT; m->src = bb_slot_get(lf); return; }
          m->kind = MARG_BOMB_TRAMPOLINE; return; }
      if (lf->op == IR_VAR && sv && sv[0] != '&' && is_global(sv)) { m->kind = MARG_BOMB_GLOBAL; return; }
      { int is_local_var = (lf->op == IR_VAR && sv && sv[0] != '&' && !is_global(sv)); int ps = is_local_var ? -1 : bb_slot_get(lf); if (ps < 0 && !is_local_var) ps = zls_off(lf);
        if (ps >= 0) { m->src = ps; m->kind = ps == aoff ? MARG_DIRECT : MARG_SLOT; return; } }
      { int voff = bb_varslot_peek(sv ? sv : ""); if (voff < 0) { m->kind = MARG_BOMB_VARSLOT; return; } m->kind = MARG_SLOT; m->src = voff; } }
}
static void call_fnlbl_set(const std::string & fl, int guard) {
    g_emit.op_call_fnlbl = call_cv_text(&g_emit.op_call_fnlbl_buf, fl);
    if (!guard) { g_emit.op_call_fnlbl_fresh = 1; return; }
    { bb_label_t * dm = emit_label_intern((fl + "$def").c_str()); g_emit.op_call_fnlbl_fresh = (!dm || !bb_label_defined(dm)) ? 1 : 0; if (g_emit.op_call_fnlbl_fresh && dm) dm->offset = 0; }
}
static void call_pl_prepare(IR_t * nd, const char * fn, int narg, IR_graph_t ** subs, int fnroute) {
    const char * op = 0; int k = pl_leaf_kind(fn, narg, &op); IR_t * a0 = (subs && subs[0]) ? subs[0]->entry : (fnroute ? ir_call_arg(nd, 0) : (IR_t *)0);
    g_emit.op_pl_kind = k; g_emit.op_pl_op = op ? op : "";
    g_emit.op_pl_sub = k == PLK_AX ? pl_ax_sub(op) : k == PLK_TYPE ? pl_type_sub(op) : k == PLK_ATOP ? pl_atop_sub(op) : 0;
    g_emit.op_pl_tags = k == PLK_TYPE ? pl_type_tags(op) : "";
    g_emit.op_pl_rat = k == PLK_TYPE ? pl_type_rat(op) : 0;
    g_emit.op_pl_jcc = (k == PLK_CMP || k == PLK_ATOP) ? pl_cmp_fail_jcc(op) : "je";
    if (k == PLK_ANUM) g_emit.op_pl_rules = pl_anum_rules((a0 && a0->op == IR_LIT_STRING) ? IR_LIT(a0).sval : (const char *)0, narg - 1);
    if (k == PLK_MKC) { g_emit.op_pl_fid_ok = (a0 && a0->op == IR_LIT_INTEGER) ? 1 : 0; g_emit.op_pl_fid = g_emit.op_pl_fid_ok ? (long)IR_LIT(a0).ival : 0L; }
    if (k == PLK_DBDECLS) { long long * w = 0; int n = pl_db_decls_words(&w); g_emit.op_pl_db_words = w; g_emit.op_pl_db_n = n; }
    if (k == PLK_AX || k == PLK_CMP || k == PLK_TYPE || k == PLK_ATOP) g_emit.op_pl_oplbl =
        call_cv_text(&g_emit.op_pl_oplbl_buf, std::string(".L") + x86_boxkind() + "_plop" + std::to_string(g_flat_node_id++));
}
void call_prepare(IR_t * nd) {
    const char * fn = g_emit.op_sval ? g_emit.op_sval : ""; int narg = (int)g_emit.op_ival; int strict = g_emit.op_strict; IR_e kind = nd->op; int route = lower_call_route(nd);
    int fnroute = (route == CALL_ROUTE_FN); int zd = (kind == IR_CALL_GEN) ? 0 : g_emit.op_zres; IR_graph_t ** subs = (IR_graph_t **)(intptr_t)g_emit.op_counter;
    g_emit.op_call_route = route; g_emit.op_call_refuse = CALL_REFUSE_NONE; g_emit.op_call_try = 0; g_emit.op_call_sym = ""; g_emit.op_call_fp = 0; g_emit.op_call_try_sym = "";
        g_emit.op_call_try_fp = 0;
    g_emit.op_call_open_sym = ""; g_emit.op_call_open_fp = 0; g_emit.op_call_bid = -1; g_emit.op_call_idx = -1; g_emit.op_call_argbase = -1; g_emit.op_call_resoff = -1; g_emit.op_call_rec_off = -1;
    g_emit.op_call_genoff = -1; g_emit.op_call_dsave = -1; g_emit.op_call_scansync = 0; g_emit.op_call_curmov = 0; g_emit.op_call_elem_access = 0; g_emit.op_call_sb_exit = 0;
    g_emit.op_call_fnlbl_fresh = 0; g_emit.op_call_fnlbl = ""; g_emit.op_call_rec_addr = 0; g_emit.op_call_rec_lbl = ""; g_emit.op_marg_n = 0; g_emit.op_pl_kind = PLK_NONE; g_emit.op_pl_sub = 0;
    g_emit.op_pl_rat = 0; g_emit.op_pl_op = ""; g_emit.op_pl_tags = ""; g_emit.op_pl_jcc = "je"; g_emit.op_pl_oplbl = ""; g_emit.op_pl_rules = (const int *)0; g_emit.op_pl_fid_ok = 0;
    g_emit.op_pl_fid = 0; g_emit.op_pl_db_words = (const long long *)0; g_emit.op_pl_db_n = 0;
    if (!zd) {
        g_emit.op_call_resoff = fnroute ? bcfn_result_slot(nd) : zls_off(nd);
        if (g_emit.op_call_resoff < 0) g_emit.op_call_refuse = CALL_REFUSE_NO_SLOT;
        else if (narg > nd->n_operands) g_emit.op_call_refuse = CALL_REFUSE_ARGC;
        g_emit.op_call_argbase = zls_argv_off(nd); if (g_emit.op_call_argbase < 0) g_emit.op_call_argbase = g_emit.op_call_resoff + 16;
        g_emit.op_call_genoff = g_emit.op_call_resoff + 16 * (1 + narg);
    }
    if (fnroute && !g_emit.op_call_refuse && nd->seal == IR_SEAL_CALL_DET_LEAF && kind != IR_CALL_DIRECT && kind != IR_CALL_PL_LEAF) g_emit.op_call_refuse = CALL_REFUSE_DET_LEAF;
    if (!zd && !g_emit.op_call_refuse) { call_margs_reserve(narg); g_emit.op_marg_n = narg;
        for (int i = narg - 1; i >= 0; i--) call_marg_plan(i, (subs && subs[i]) ? subs[i]->entry : (fnroute ? ir_call_arg(nd, i) : (IR_t *)0), g_emit.op_call_argbase + 16 * i, nd); }
    g_emit.op_call_bid = bid_bake_of(fn);
    g_emit.op_call_elem_access = fnroute ? elem_access_kind(nd) : 0;
    g_emit.op_call_sb_exit = (fnroute && zd && g_emit.op_sb) ? 1 : 0;
    if (!fnroute && !zd && kind != IR_CALL_GEN) { g_emit.op_call_scansync = lower_call_is_scan_builtin(fn); g_emit.op_call_curmov = lower_call_is_curmov(fn);
        g_emit.op_call_dsave = g_emit.op_call_argbase + 16 * narg; }
    if (zd && !fnroute && kind == IR_CALL_BYNAME) { g_emit.op_call_scansync = lower_call_is_scan_builtin(fn); if (zd_call_curmov(nd)) { g_emit.op_call_curmov = 1;
        g_emit.op_call_dsave = (zls_g_generator_top(g_emit_cfg) > 0 && zls_off(nd) >= 0 && !zls_key_unbacked(g_emit_cfg, zls_off(nd))) ? zls_frame_off(g_emit_cfg, nd) + 16 : INT_MIN; } }
    if (kind == IR_CALL_GEN) g_emit.op_call_scansync = lower_call_is_scan_builtin(fn);
    switch (kind) {
    case IR_CALL_DIRECT: { const char * s = 0; g_emit.op_call_fp = (uint64_t)(uintptr_t)dop_direct_fp(fn, (int64_t)narg, &s); g_emit.op_call_sym = s ? s : ""; break; }
    case IR_CALL_PL_LEAF: call_pl_prepare(nd, fn, narg, subs, fnroute); break;
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
        if (kind == IR_CALL_METHOD || kind == IR_CALL_REDISP) g_emit.op_call_rec_off = zls_redisp_off(nd) >= 0 ? zls_redisp_off(nd) : zls_argv_off(nd) >= 0 ? zls_argv_off(nd) + 16 * narg : -1;
        if (fnroute) call_fnlbl_set(std::string(".L") + x86_boxkind() + (zd ? "_rkfnzd" : "_rkfn") + std::to_string(g_flat_node_id++), 0);
        else call_fnlbl_set(std::string(".L") + x86_boxkind() + (zd ? "_bynamefnzd" : "_bynamefn") + std::to_string((long long)g_emit.nid), 1);
        break; }
    case IR_CALL_GEN: call_fnlbl_set(std::string(".L") + x86_boxkind() + "_bynamegenfn" + std::to_string((long long)g_emit.nid), 1); break;
    default: break;
    }
}

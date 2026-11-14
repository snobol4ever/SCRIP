#include "emitter/em/em_head.h"
#include "emitter/emit_state_em_03_sh_api.h"
#include "emitter/emit_medium.h"
#include "emitter/emit_data.h"
#include "emitter/emit_knobs.h"
#include "emitter/emit_regime.h"
#include "emitter/emit_walk.h"
#include "emitter/emit_str.h"
#include "lower/lower_prolog.h"
#include "ir/ir_node_index.h"
#include "ir/stage2_procentry.h"
#include "ir/stage2.h"
#include "ir/stage2_sm_prog_api.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "ir/ir_graph.h"
#include "ir/bb_box_fn.h"
#include "ir/descr_tags_dt_platom.h"
#include "ir/ct_arena.h"
extern "C" {
#include "templates/xa/xa_templates.h"
}
extern "C" {
#include "templates/x86/x86_frame.h"
}
extern "C" {
#include "templates/x86/x86_gc.h"
}
extern "C" {
#include "templates/x86/x86_rtcc.h"
}
extern "C" {
#include "templates/x86/x86_rodata.h"
}
extern "C" {
#include "ir/pin_va.h"
}
extern "C" {
#include "runtime/core/comm_var.h"
}
extern "C" {
#include "optimizer/gva_collect.h"
}
#include "emitter/em/em_01_sh.h"
#include "emitter/em/em_02_sh.h"
#include "emitter/em/em_03_sh.h"
#include "emitter/em/em_04_sh.h"
extern "C" { const char *child_cache_get_lbl(bb_box_fn fn); }
extern "C" { extern int gva_index_of(const char *name); }
extern "C" {
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void bb_label_name_set(bb_label_t * l, const char * src) {
    size_t n = src ? strlen(src) : 0;
    char * p = (char *) l->name;
    if (n >= UINT32_MAX) { fprintf(stderr, "emit: a label name of %zu bytes cannot be stored\n", n); _exit(3); }
    if (n + 1 > l->name_cap) { uint32_t c = 2 * l->name_cap; if (c < n + 1) c = (uint32_t) (n + 1); p = (char *) ct_alloc(c); l->name = p; l->name_cap = c; }
    if (n) memcpy(p, src, n);
    p[n] = '\0';
}
int bb_emit_size = 0;
cv_t bb_patch_list;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_label_pool_reset(void)
{
    for (int i = 0; i < g_emitter.g_label_pool_n; i++) { bb_label_t *l = g_emitter.g_label_pool[i]; if (l->name_cap) ct_drop((void *)l->name); ct_drop(l); }
    g_emitter.g_label_pool_n = 0; g_emitter.g_label_map.clear();
    g_emitter.g_flt_lbl[1] = g_emitter.g_flt_lbl[2] = g_emitter.g_flt_lbl[3] = (bb_label_t *)0;
}
bb_label_t * emit_floater_label(int k) { static const char * const fn[4] = {0, "RETURN", "FRETURN", "NRETURN"}; if (k < 1 || k > 3) return (bb_label_t *)0;
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
    g_emitter.bb_emit_want = 0;
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
    g_emitter.bb_emit_want++;
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
    g_emitter.bb_emit_want += (long)k;
    bb_emit_pos += (int)n;
    if (n < k) g_emitter.bb_emit_overflow = 1;
}
void bb_emit_u32(uint32_t v) { uint8_t b[4] = { (uint8_t)(v), (uint8_t)(v >> 8), (uint8_t)(v >> 16), (uint8_t)(v >> 24) }; bb_emit_bytes(b, 4); }
void bb_emit_u64(uint64_t v) { uint8_t b[8]; memcpy(b, &v, 8); bb_emit_bytes(b, 8); }
void bb_emit_i32(int32_t v) { uint32_t u; memcpy(&u, &v, 4); bb_emit_u32(u); }
int g_is_text = 0;
int g_emit_pos = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emitter_end(void) { return g_is_text ? g_emit_pos : bb_emit_end(); }
void ef_b1 (uint8_t a) { bb_emit_byte(a); }
void ef_b2 (uint8_t a, uint8_t b) { bb_emit_byte(a); bb_emit_byte(b); }
void ef_b3 (uint8_t a, uint8_t b, uint8_t c) { bb_emit_byte(a); bb_emit_byte(b); bb_emit_byte(c); }
static void ef_b4 (uint8_t a, uint8_t b, uint8_t c, uint8_t d) { bb_emit_byte(a); bb_emit_byte(b); bb_emit_byte(c); bb_emit_byte(d); }
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
void (*g_cap_fixup_cb)(void *cap_ptr, const char *child_α_label) = NULL;
long flat_buf_max_v() { if (g_emitter.flat_buf_max_v_v < 0) { const char * e = getenv("SCRIP_FLAT_BUF_MB"); long mb = e && *e ? atol(e) : 4; if (mb < 4) mb = 4; if (mb > 256) mb = 256;
    g_emitter.flat_buf_max_v_v = mb * 1024L * 1024L; } return g_emitter.flat_buf_max_v_v; }
int g_flat_node_id = 0;
extern "C" void emit_program_open(void) { strtab_reset(); g_emitter.g_gc_map_names_n = 0; g_emitter.g_gc_map_names_v.len = 0; g_emitter.g_gc_sites_names_n = 0; g_emitter.g_gc_sites_names_v.len = 0;
    g_flat_node_id = 0; g_emit.gc_code_seq = 0; g_emit.x86_vd_seq = 0; g_emitter.emit_gc_sites_data_seq = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fbdiv_report(void) { fprintf(stderr, "FB-DIVERGE TOTAL=%d\n", g_emitter.fbdiv_n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_fb_divergence_check(void) { if (g_emitter.fbdiv_mode < 0) { const char * e = getenv("SCRIP_FB_DIVERGE"); g_emitter.fbdiv_mode = (e && *e == '1') ? 1 : 0; } if (!g_emitter.fbdiv_mode) return;
    if (!g_emitter.fbdiv_hooked) { g_emitter.fbdiv_hooked = 1; atexit(fbdiv_report); } if (emit_jmp_pin_legacy() == emit_rec_pin()) return; g_emitter.fbdiv_n++;
    fprintf(stderr, "FB-DIVERGE #%d graph=%s data_fb=%s rec_fb=%s deep=%d pat=%d gen=%d genproc=%d rescall=%d\n", g_emitter.fbdiv_n, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "<anon>",
    emit_jmp_pin_legacy() ? "rsp" : "rsp", emit_rec_pin() ? "rsp" : "rsp", g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, g_resumable_callable_active); }
bb_label_t *g_create_body_entry = NULL;
cv_t g_flat_data_buf;
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
int blob_cont_copy(void) { if (g_emitter.blob_cont_copy_v < 0) { const char * e = getenv("SCRIP_BLOB_CONT_COPY"); g_emitter.blob_cont_copy_v = (e && *e == '1') ? 1 : 0;
    } return g_emitter.blob_cont_copy_v; }
static int blob_zf_gc_only(void) { if (g_emitter.blob_zf_gc_only_v < 0) { const char * e = getenv("SCRIP_BLOB_ZF_GC_ONLY"), * d = getenv("SCRIP_DIAG");
    g_emitter.blob_zf_gc_only_v = e ? (*e == '1') : (d && *d == '0'); } return g_emitter.blob_zf_gc_only_v; }
static int blob_zf_rep(void) { if (g_emitter.blob_zf_rep_v < 0) { const char * e = getenv("SCRIP_BLOB_ZF_REP"); g_emitter.blob_zf_rep_v = (e && *e == '1') ? 1 : 0; } return g_emitter.blob_zf_rep_v; }
int blob_omega_ret(void) { if (g_emitter.blob_omega_ret_v < 0) { const char * e = getenv("SCRIP_BLOB_OMEGA_RET"); g_emitter.blob_omega_ret_v = (e && *e == '1') ? 1 : 0;
    } return g_emitter.blob_omega_ret_v; }
std::string blob_zero_fill(int bfb) {
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
            if (ir_norm_call_kind(v->op) == IR_CALL && IR_LIT(v).sval && !strcmp(IR_LIT(v).sval, "SNO$MKPAT") && v->n_operands >= 1 && v->operands[0] && v->operands[0]->op == IR_LIT_THUNK &&
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
void blob_first_guard(void) {
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * icn_trace_intern(const char * s) {
    if (!s) return s;
    return g_emitter.icn_trace_intern_pool.insert(std::string(s)).first->c_str();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string flat_zero_fill(int lo, int len) {
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
std::string icn_entry_gva(void) {
    return x86("comment", "entry: r9 (the global-cell base the RTCC bank keeps live across Icon code) is reloaded from its home -- a C road (a generator "
                           "entered on its coroutine thread, a by-name or apply entry) hands over whatever C left in it, and no call precedes the first read")
         + x86_rtx_reestablish(RTCC_C_R9);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string icn_trace_tap(const char * pname, int kind, int np, int aoff) {
    if (!x86_trace_taps_on()) return std::string();
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
}
